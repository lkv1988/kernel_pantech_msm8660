/* arch/arm/mach-msm/smd_smd_vt.c
 *    FEATURE_PANTECH_VT_SUPPORT
 *
 * SMD smd_vt Driver -- Provides a smd_vt SMD port interface.
 *
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <asm/ioctls.h>
#include <mach/msm_smd.h>
#include "smd_private.h"

#define MAX_RX_BUF_SIZE (8*1024)
#define MAX_TX_BUF_SIZE (1*1024)

static DEFINE_MUTEX(smd_vt_ch_lock);
static DEFINE_MUTEX(smd_vt_rx_buf_lock);
static DEFINE_MUTEX(smd_vt_tx_buf_lock);
static DEFINE_SPINLOCK(smd_vt_read_lock);

static DECLARE_WAIT_QUEUE_HEAD(smd_vt_wait_queue);

struct smd_vt_device_t {
    struct miscdevice misc;

    struct smd_channel *ch;

    unsigned char tx_buf[MAX_TX_BUF_SIZE];
    unsigned char rx_buf[MAX_RX_BUF_SIZE];
    unsigned int read_avail;
    unsigned int open_count;
    unsigned int open_flag;    
};

struct smd_vt_device_t *smd_vt_devp;

static void smd_vt_notify(void *priv, unsigned event)
{
    unsigned long flags;

    switch (event) {
        case SMD_EVENT_DATA: {
            int sz;
         
            if( (smd_vt_devp->open_count != 0) && (smd_vt_devp->ch != 0)){    // SMD bug fix. 0811
                sz = smd_cur_packet_size(smd_vt_devp->ch);
                if ((sz > 0) && (sz <= smd_read_avail(smd_vt_devp->ch))) {
                    /* queue_work(smd_vt_wq, &smd_vt_work); */
                    spin_lock_irqsave(&smd_vt_read_lock, flags);
                    if (smd_vt_devp->read_avail == 0) {
                        smd_vt_devp->read_avail = sz;
                        wake_up_interruptible(&smd_vt_wait_queue);
                    }
                    spin_unlock_irqrestore(&smd_vt_read_lock, flags);
                }
            }
            break;
        }
        case SMD_EVENT_OPEN:
            smd_vt_devp->open_flag =1;
            printk(KERN_INFO "smd_vt: smd opened\n");
            break;
        case SMD_EVENT_CLOSE:
            printk(KERN_INFO "smd_vt: smd closed\n");
            break;
    }
}

static ssize_t smd_vt_read(struct file *fp, char __user *buf,
			 size_t count, loff_t *pos)
{
    int r = 0;
    int bytes_read = 0;
    unsigned long flags;
    int sz, avail;

    if(count > MAX_RX_BUF_SIZE)   count = MAX_RX_BUF_SIZE;
        
    for (;;) {
        mutex_lock(&smd_vt_rx_buf_lock);
  
        if( (smd_vt_devp->open_count == 0) || (smd_vt_devp->ch == 0)
            || (smd_vt_devp->open_flag==0) || (count <=0)){
            mutex_unlock(&smd_vt_rx_buf_lock);
            return 0;
        }

        spin_lock_irqsave(&smd_vt_read_lock, flags);
        smd_vt_devp->read_avail = 0;
        spin_unlock_irqrestore(&smd_vt_read_lock, flags);

        sz = smd_cur_packet_size(smd_vt_devp->ch);

        if (sz > MAX_RX_BUF_SIZE) {    // garbage  data
            smd_read(smd_vt_devp->ch, 0, sz);
            mutex_unlock(&smd_vt_rx_buf_lock);
            continue;
        }
        avail = smd_read_avail(smd_vt_devp->ch);
        
        if((sz == avail) && (sz != 0)){
            if( sz > count) sz = count; // output bffer size check. 
            bytes_read = smd_read(smd_vt_devp->ch, buf, sz);
            break;
        }else if( sz < avail ){  // garbage data 
            smd_read(smd_vt_devp->ch, 0, avail);
            mutex_unlock(&smd_vt_rx_buf_lock);
            continue;    
        }
        // else is data not ready wait data. 

        mutex_unlock(&smd_vt_rx_buf_lock);
        r = wait_event_interruptible_timeout(smd_vt_wait_queue,
						 smd_vt_devp->read_avail,HZ*5);
        if (r <= 0) {    // timeout or error
            /* qualify error message */
            if (r != -ERESTARTSYS) {
                /* we get this anytime a signal comes in */
                printk(KERN_ERR "ERROR:%s:%i:%s: "
                            "wait_event_interruptible ret %i\n",
                                __FILE__,
                                __LINE__,
                                __func__,
                                r
                                );
            }
            return 0; //r;
        }
    }
    mutex_unlock(&smd_vt_rx_buf_lock);
    return bytes_read;
}

static ssize_t smd_vt_write(struct file *file,
				const char __user *buf,
				size_t count,
				loff_t *ppos)
{
    int ret=0; 
    int avail=0;
    int len=0;
        
    mutex_lock(&smd_vt_tx_buf_lock);
	
    // SMD bug fix. 0811
    if( (smd_vt_devp->open_count == 0) || (smd_vt_devp->ch == 0)
        ||(smd_vt_devp->open_flag ==0)|| (count <=0)){
        mutex_unlock(&smd_vt_tx_buf_lock);
        return 0;
    }

    avail = smd_write_avail(smd_vt_devp->ch);
    if(avail < count){
        len =avail;
    } else{
        len =count;
    }
    if(len <= 0){  
        mutex_unlock(&smd_vt_tx_buf_lock);
        return 0;
    }
    
    ret = smd_write(smd_vt_devp->ch, buf, len);
    mutex_unlock(&smd_vt_tx_buf_lock);
    return ret;
}

static int smd_vt_open(struct inode *ip, struct file *fp)
{
    int r = 0;
    int try_count =0;
    
    printk(KERN_INFO "smd_vt_open\n");

    mutex_lock(&smd_vt_ch_lock);
    if ((smd_vt_devp->ch == 0) || (smd_vt_devp->open_count == 0)) {
        r = smd_open("DATA11", &smd_vt_devp->ch,0, smd_vt_notify);
        while((r == 0) && (smd_vt_devp->open_flag ==0)){
            if(100 < try_count++){
                r = -ENODEV;
                break;
            }
            msleep(10);
        }
    }

    if( r ==  0){    // opened success or already opened.
        smd_vt_devp->open_count++;
    }
    mutex_unlock(&smd_vt_ch_lock);
    return r;
}

static int smd_vt_release(struct inode *ip, struct file *fp)
{
    int r = 0;
    int sz;
    unsigned long flags;

    printk(KERN_INFO "smd_vt_release\n");
    
    mutex_lock(&smd_vt_ch_lock);
    
    smd_vt_devp->open_count--;
    
    if ((smd_vt_devp->open_count == 0) && (smd_vt_devp->ch != 0)) {

        mutex_lock(&smd_vt_rx_buf_lock);
  
        spin_lock_irqsave(&smd_vt_read_lock, flags);
        smd_vt_devp->read_avail = 0;
        spin_unlock_irqrestore(&smd_vt_read_lock, flags);

        sz = smd_cur_packet_size(smd_vt_devp->ch);

        while((sz != 0) && (sz <= smd_read_avail(smd_vt_devp->ch))) {
            if (sz > MAX_RX_BUF_SIZE) {
                smd_read(smd_vt_devp->ch, smd_vt_devp->rx_buf, MAX_RX_BUF_SIZE);
                sz= sz -MAX_RX_BUF_SIZE;
            }
            else{
                smd_read(smd_vt_devp->ch, smd_vt_devp->rx_buf, sz);
                sz =0;
            }
        }
        mutex_unlock(&smd_vt_rx_buf_lock);


        r = smd_close(smd_vt_devp->ch);
        smd_vt_devp->ch = 0;
        smd_vt_devp->open_flag=0;
    }
    mutex_unlock(&smd_vt_ch_lock);

    return r;
}

static long smd_vt_ioctl(struct file *file,
		    unsigned int cmd,
		    unsigned long arg)
{
    int ret;

    // SMD bug fix. 0811
    if((smd_vt_devp->open_count == 0) ||( smd_vt_devp->ch == 0)) return -1;

    switch (cmd) {
        case TIOCMGET:
            ret = smd_tiocmget(smd_vt_devp->ch);
            break;
        case TIOCMSET:
            ret = smd_tiocmset(smd_vt_devp->ch, arg, ~arg);
            break;
        default:
            ret = -1;
    }

    return ret;
}

static struct file_operations smd_vt_fops = {
    .owner = THIS_MODULE,
    .read = smd_vt_read,
    .write = smd_vt_write,
    .open = smd_vt_open,
    .release = smd_vt_release,
    .unlocked_ioctl = smd_vt_ioctl,
};

static struct smd_vt_device_t smd_vt_device = {
    .misc = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "smd_vt",
        .fops = &smd_vt_fops,
    }
};

static void __exit smd_vt_exit(void)
{
    misc_deregister(&smd_vt_device.misc);
}

static int __init smd_vt_init(void)
{
    int ret;

    smd_vt_devp = &smd_vt_device;
    
    smd_vt_devp->ch =0;
    smd_vt_devp->open_count =0;
    smd_vt_devp->open_flag =0;
    
    ret = misc_register(&smd_vt_device.misc);
    return ret;
}

module_init(smd_vt_init);
module_exit(smd_vt_exit);

MODULE_DESCRIPTION("MSM Shared Memory smd_vt Driver");
MODULE_LICENSE("GPL v2");

