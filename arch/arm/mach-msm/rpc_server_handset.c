/* arch/arm/mach-msm/rpc_server_handset.c
 *
 * Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/switch.h>

#include <asm/mach-types.h>

#include <mach/msm_rpcrouter.h>
#include <mach/board.h>
#include <mach/rpc_server_handset.h>

#include <CUST_SKY.h>

#ifdef MODEL_SKY
#include <mach/mpp.h>          
// N1037 20120222 porting (+)
#include <linux/pmic8058-xoadc.h>
#include <mach/board-msm8660.h> 
// N1037 20120222 porting (-)
#include <mach/vreg.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>                   
#include <linux/regulator/pmic8058-regulator.h>         
#include <linux/pmic8058-xoadc.h>
#include <asm/irq.h>
#include <linux/workqueue.h>
#include <linux/errno.h>
#include <linux/msm_adc.h>
#include <mach/msm_xo.h>
#include <mach/msm_hsusb.h>
#include <linux/msm-charger.h>
#include <linux/spinlock.h>
#include "mpm.h"
#include <linux/mutex.h>

/* -------------------------------------------------------------------- */
/* debug option */
/* -------------------------------------------------------------------- */
//#define EARJACK_DBG
#ifdef EARJACK_DBG
#define dbg_earjack(fmt, args...)   printk("[EARJACK]" fmt, ##args)
#else
#define dbg_earjack(fmt, args...)
#endif
#define dbg_func_in()       dbg_earjack("[FUNC_IN] %s\n", __func__)
#define dbg_func_out()      dbg_earjack("[FUNC_OUT] %s\n", __func__)
#define dbg_line()          dbg_earjack("[LINE] %d(%s)\n", __LINE__, __func__)
/* -------------------------------------------------------------------- */
#endif

#define DRIVER_NAME	"msm-handset"

#define HS_SERVER_PROG 0x30000062
#define HS_SERVER_VERS 0x00010001

#define HS_RPC_PROG 0x30000091

#define HS_PROCESS_CMD_PROC 0x02
#define HS_SUBSCRIBE_SRVC_PROC 0x03
#define HS_REPORT_EVNT_PROC    0x05
#define HS_EVENT_CB_PROC	1
#define HS_EVENT_DATA_VER	1

#define RPC_KEYPAD_NULL_PROC 0
#define RPC_KEYPAD_PASS_KEY_CODE_PROC 2
#define RPC_KEYPAD_SET_PWR_KEY_STATE_PROC 3

#ifdef MODEL_SKY
#define HS_NONE_K       0x00    /* nothing  */
#endif
#define HS_PWR_K		0x6F	/* Power key */
#define HS_END_K		0x51	/* End key or Power key */
#define HS_STEREO_HEADSET_K	0x82
#define HS_HEADSET_SWITCH_K	0x84
#define HS_HEADSET_SWITCH_2_K	0xF0
#define HS_HEADSET_SWITCH_3_K	0xF1
#define HS_HEADSET_HEADPHONE_K	0xF6
#define HS_HEADSET_MICROPHONE_K 0xF7
#define HS_REL_K		0xFF	/* key release */

#define SW_HEADPHONE_INSERT_W_MIC 1 /* HS with mic */

#define HS_KEY(hs_key, input_key) ((hs_key << 24) | input_key)

#ifdef MODEL_SKY
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) )

#define uint32 unsigned long
#define uint16 unsigned short
#define uint8  unsigned char

#define int32  long
#define int16  short
#define int8   char
#define TRUE  1
#define FALSE 0


#define EARJACK_DET     125
#define REMOTEKEY_DET   215

typedef enum{
  SKY_HS_JACK_STATE_OFF,
  SKY_HS_JACK_STATE_ON_4POLAR,
  SKY_HS_JACK_STATE_ON_3POLAR,
  SKY_HS_JACK_STATE_ON_3POLAR_CHECK,
  SKY_HS_JACK_STATE_ON_CHECK,
  SKY_HS_JACK_STATE_MAX,
  SKY_HS_JACK_STATE_INIT,
}SKY_HS_JACK_STATE_E;
typedef struct {
    SKY_HS_JACK_STATE_E    state;
   
} SKY_HS_JACK_T;

static struct regulator *hs_jack_l8; // N1037 20120216 porting 

typedef enum{
  REMOTE_3P5PI_KEY_NONE,
  REMOTE_3P5PI_KEY_01, // send,end
  REMOTE_3P5PI_KEY_02, // vol up
  REMOTE_3P5PI_KEY_03, // vol down
  REMOTE_3P5PI_KEY_04, // car kit 
  REMOTE_3P5PI_KEY_MAX,
} ext_3p5Pi_remode_key;

typedef enum {
  SKY_HS_NONE,
//#if defined(FEATURE_SKY_20PIN_CONNECTOR) || defined(FEATURE_SKY_3P5PI_EARJACK)
  SKY_HS_EAR_JACK_ON,         /* ear jack is placed */
  SKY_HS_EAR_JACK_OFF,        /* ear jack is off */
  SKY_HS_AV_CABLE_ON,
  SKY_HS_AV_CABLE_OFF,
  SKY_HS_EAR_CLICK,           /* ear jack button is clicked */
  SKY_HS_EAR_DOUBLE_CLICK,    /* ear jack button is double-clicked */
  SKY_HS_EAR_JACK_VOLUME_UP,
  SKY_HS_EAR_JACK_VOLUME_DOWN,
  SKY_HS_REMOTE_PLAY,
  SKY_HS_REMOTE_DMB,
  SKY_HS_REMOTE_REV,
  SKY_HS_REMOTE_FF,
  SKY_HS_REMOTE_REC,
  SKY_HS_REMOTE_STOP,
  SKY_HS_RESERVED_KEY1,
  SKY_HS_RESERVED_KEY2,
  SKY_HS_RESERVED_KEY3,


  SKY_HS_MAX = 0xFFFF
} SKY_HS_EVENT_E;


#define TEST_HANDSET

#ifdef TEST_HANDSET
#define HS_VOL_UP_K                          0x8F
#define HS_VOL_DOWN_K                        0x90
#define HS_HEADSET_K                         0x7E
#define HS_SKY_20PIN_STEREO_HEADSET_K        0xE0
#define HS_SKY_3P5PI_STEREO_HEADSET_K        0xE1
#define HS_SKY_3P5PI_STEREO_HEADSET_NO_MIC_K 0xE2
#define HS_SKY_MICROUSB_TDMB_K                   0xE3
#ifdef FEATURE_SKY_CHG_LOGO
#define HS_SKY_CHARGING_CABLE_REMOVED_K      0xE4
#endif //FEATURE_SKY_CHG_LOGO
#endif //TEST_HANDSET


typedef struct{
    ext_3p5Pi_remode_key remode_3p5Pi_key;
    uint16 min;
    uint16 max;
    SKY_HS_EVENT_E event;
    int  keyevent;
    uint8 bPressed;
} remode_3p5Pi_key_type;
//static boolean Is_3p5pi_earjack_have_MIC;

// N1037 20120216 porting (+) 
static remode_3p5Pi_key_type remode_3p5Pi_key_event[] ={                        // reffer to EF18
    { REMOTE_3P5PI_KEY_NONE, 2400, 2600, SKY_HS_NONE, HS_NONE_K, FALSE},
    { REMOTE_3P5PI_KEY_01,     60,  130, SKY_HS_EAR_CLICK, HS_HEADSET_SWITCH_K, FALSE},
    { REMOTE_3P5PI_KEY_02,    420,  620, SKY_HS_EAR_JACK_VOLUME_DOWN, HS_VOL_DOWN_K, FALSE},
    { REMOTE_3P5PI_KEY_03,    230,  390, SKY_HS_EAR_JACK_VOLUME_UP, HS_VOL_UP_K, FALSE},
	{ REMOTE_3P5PI_KEY_04,     0,   50, SKY_HS_EAR_CLICK, HS_HEADSET_SWITCH_K, FALSE},	// car kit
};
// N1037 20120216 porting (-)

static SKY_HS_JACK_T sky_hs_3p5pi_jack_ctrl;                                    // static variety setting

// N1037 20120216 proting (+)
static struct delayed_work earjack_work;
static struct delayed_work remotekey_work;

static void earjack_det_func(struct work_struct * earjack_work);
static void remotekey_det_func(struct work_struct * remotekey_work);
// N1037 20120216 porting (-)
static int conn_headset_type = 0;

// N1037 20120216 porting (+) 
static  uint8 remote_id = REMOTE_3P5PI_KEY_NONE;
static  uint8 remote_key = REMOTE_3P5PI_KEY_NONE;
static  int released=0;
// N1037 20120216 porting (-) 

#endif

enum hs_event {
	HS_EVNT_EXT_PWR = 0,	/* External Power status        */
	HS_EVNT_HSD,		/* Headset Detection            */
	HS_EVNT_HSTD,		/* Headset Type Detection       */
	HS_EVNT_HSSD,		/* Headset Switch Detection     */
	HS_EVNT_KPD,
	HS_EVNT_FLIP,		/* Flip / Clamshell status (open/close) */
	HS_EVNT_CHARGER,	/* Battery is being charged or not */
	HS_EVNT_ENV,		/* Events from runtime environment like DEM */
	HS_EVNT_REM,		/* Events received from HS counterpart on a
				remote processor*/
	HS_EVNT_DIAG,		/* Diag Events  */
	HS_EVNT_LAST,		 /* Should always be the last event type */
	HS_EVNT_MAX		/* Force enum to be an 32-bit number */
};

enum hs_src_state {
	HS_SRC_STATE_UNKWN = 0,
	HS_SRC_STATE_LO,
	HS_SRC_STATE_HI,
};

struct hs_event_data {
	uint32_t	ver;		/* Version number */
	enum hs_event	event_type;     /* Event Type	*/
	enum hs_event	enum_disc;     /* discriminator */
	uint32_t	data_length;	/* length of the next field */
	enum hs_src_state	data;    /* Pointer to data */
	uint32_t	data_size;	/* Elements to be processed in data */
};

enum hs_return_value {
	HS_EKPDLOCKED     = -2,	/* Operation failed because keypad is locked */
	HS_ENOTSUPPORTED  = -1,	/* Functionality not supported */
	HS_FALSE          =  0, /* Inquired condition is not true */
	HS_FAILURE        =  0, /* Requested operation was not successful */
	HS_TRUE           =  1, /* Inquired condition is true */
	HS_SUCCESS        =  1, /* Requested operation was successful */
	HS_MAX_RETURN     =  0x7FFFFFFF/* Force enum to be a 32 bit number */
};

struct hs_key_data {
	uint32_t ver;        /* Version number to track sturcture changes */
	uint32_t code;       /* which key? */
	uint32_t parm;       /* key status. Up/down or pressed/released */
};

enum hs_subs_srvc {
	HS_SUBS_SEND_CMD = 0, /* Subscribe to send commands to HS */
	HS_SUBS_RCV_EVNT,     /* Subscribe to receive Events from HS */
	HS_SUBS_SRVC_MAX
};

enum hs_subs_req {
	HS_SUBS_REGISTER,    /* Subscribe   */
	HS_SUBS_CANCEL,      /* Unsubscribe */
	HS_SUB_STATUS_MAX
};

enum hs_event_class {
	HS_EVNT_CLASS_ALL = 0, /* All HS events */
	HS_EVNT_CLASS_LAST,    /* Should always be the last class type   */
	HS_EVNT_CLASS_MAX
};

enum hs_cmd_class {
	HS_CMD_CLASS_LCD = 0, /* Send LCD related commands              */
	HS_CMD_CLASS_KPD,     /* Send KPD related commands              */
	HS_CMD_CLASS_LAST,    /* Should always be the last class type   */
	HS_CMD_CLASS_MAX
};

/*
 * Receive events or send command
 */
union hs_subs_class {
	enum hs_event_class	evnt;
	enum hs_cmd_class	cmd;
};

struct hs_subs {
	uint32_t                ver;
	enum hs_subs_srvc	srvc;  /* commands or events */
	enum hs_subs_req	req;   /* subscribe or unsubscribe  */
	uint32_t		host_os;
	enum hs_subs_req	disc;  /* discriminator    */
	union hs_subs_class      id;
};

struct hs_event_cb_recv {
	uint32_t cb_id;
	uint32_t hs_key_data_ptr;
	struct hs_key_data key;
};
enum hs_ext_cmd_type {
	HS_EXT_CMD_KPD_SEND_KEY = 0, /* Send Key */
	HS_EXT_CMD_KPD_BKLT_CTRL, /* Keypad backlight intensity	*/
	HS_EXT_CMD_LCD_BKLT_CTRL, /* LCD Backlight intensity */
	HS_EXT_CMD_DIAG_KEYMAP, /* Emulating a Diag key sequence */
	HS_EXT_CMD_DIAG_LOCK, /* Device Lock/Unlock */
	HS_EXT_CMD_GET_EVNT_STATUS, /* Get the status for one of the drivers */
	HS_EXT_CMD_KPD_GET_KEYS_STATUS,/* Get a list of keys status */
	HS_EXT_CMD_KPD_SET_PWR_KEY_RST_THOLD, /* PWR Key HW Reset duration */
	HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD, /* Set pwr key threshold duration */
	HS_EXT_CMD_LAST, /* Should always be the last command type */
	HS_EXT_CMD_MAX = 0x7FFFFFFF /* Force enum to be an 32-bit number */
};

struct hs_cmd_data_type {
	uint32_t hs_cmd_data_type_ptr; /* hs_cmd_data_type ptr length */
	uint32_t ver; /* version */
	enum hs_ext_cmd_type id; /* command id */
	uint32_t handle; /* handle returned from subscribe proc */
	enum hs_ext_cmd_type disc_id1; /* discriminator id */
	uint32_t input_ptr; /* input ptr length */
	uint32_t input_val; /* command specific data */
	uint32_t input_len; /* length of command input */
	enum hs_ext_cmd_type disc_id2; /* discriminator id */
	uint32_t output_len; /* length of output data */
	uint32_t delayed; /* execution context for modem
				true - caller context
				false - hs task context*/
};

static const uint32_t hs_key_map[] = {
	HS_KEY(HS_PWR_K, KEY_POWER),
	HS_KEY(HS_END_K, KEY_END),
	HS_KEY(HS_STEREO_HEADSET_K, SW_HEADPHONE_INSERT_W_MIC),
	HS_KEY(HS_HEADSET_HEADPHONE_K, SW_HEADPHONE_INSERT),
	HS_KEY(HS_HEADSET_MICROPHONE_K, SW_MICROPHONE_INSERT),
	HS_KEY(HS_HEADSET_SWITCH_K, KEY_MEDIA),

    #ifdef FEATURE_SKY_3_5PHIEARJACK
    HS_KEY(HS_VOL_UP_K, KEY_VOLUMEUP),
    HS_KEY(HS_VOL_DOWN_K, KEY_VOLUMEDOWN),
    HS_KEY(HS_SKY_20PIN_STEREO_HEADSET_K, SW_HEADPHONE_INSERT), //20pin headset
    HS_KEY(HS_SKY_3P5PI_STEREO_HEADSET_K, SW_HEADPHONE_INSERT), // 3.5pi headset(mic)
    HS_KEY(HS_SKY_3P5PI_STEREO_HEADSET_NO_MIC_K, SW_HEADPHONE_INSERT), // 3.5pi headset(no mic)
    HS_KEY(HS_SKY_MICROUSB_TDMB_K, SW_HEADPHONE_INSERT), //20pin tdmb
    #else
	HS_KEY(HS_HEADSET_SWITCH_2_K, KEY_VOLUMEUP),
	HS_KEY(HS_HEADSET_SWITCH_3_K, KEY_VOLUMEDOWN),
    #endif
    #ifdef FEATURE_SKY_CHG_LOGO
    HS_KEY(HS_SKY_CHARGING_CABLE_REMOVED_K, KEY_BATTERY), //charging cable key
    #endif
	0
};

enum {
	NO_DEVICE	= 0,
	MSM_HEADSET	= 1,
};
/* Add newer versions at the top of array */
static const unsigned int rpc_vers[] = {
	0x00030001,
	0x00020001,
	0x00010001,
};
/* hs subscription request parameters */
struct hs_subs_rpc_req {
	uint32_t hs_subs_ptr;
	struct hs_subs hs_subs;
	uint32_t hs_cb_id;
	uint32_t hs_handle_ptr;
	uint32_t hs_handle_data;
};

static struct hs_subs_rpc_req *hs_subs_req;

struct msm_handset {
	struct input_dev *ipdev;
	struct switch_dev sdev;
	struct msm_handset_platform_data *hs_pdata;
	bool mic_on, hs_on;
};

// N1037 20120222 porting  (+) 
struct pm8xxx_mpp_info {
	unsigned			mpp;
	struct pm8xxx_mpp_config_data	config;
};

static void  pm8058_mpp_config_DIG(void)
{
	int ret;

       struct pm8xxx_mpp_config_data sky_handset_digital_adc = {
			.type	= PM8XXX_MPP_TYPE_D_INPUT,
			.level	= PM8058_MPP_DIG_LEVEL_S3,
			.control = PM8XXX_MPP_DIN_TO_INT,	
		};

	ret = pm8xxx_mpp_config(PM8058_MPP_PM_TO_SYS(XOADC_MPP_3), &sky_handset_digital_adc);

	if (ret < 0) 
		pr_err("%s: pm8058_mpp_config_DIG ret=%d\n",__func__, ret);
}

static void  pm8058_mpp_config_AMUX(void)
{
	int ret;

       struct pm8xxx_mpp_config_data sky_handset_analog_adc = {
			.type	= PM8XXX_MPP_TYPE_A_INPUT,
			.level	= PM8XXX_MPP_AIN_AMUX_CH5,
			.control = PM8XXX_MPP_AOUT_CTRL_DISABLE,	
		};	
     
	ret = pm8xxx_mpp_config(PM8058_MPP_PM_TO_SYS(XOADC_MPP_3), &sky_handset_analog_adc);	
       
    	if (ret)
   		pr_err("%s: pm8058_mpp_config_AMUX ret=%d\n",__func__, ret);	
}
// N1037 20120222 porting (-) 

#ifdef FEATURE_SKY_3_5PHIEARJACK //PS2 P13106 Kang,Yoonkoo
//static int conn_headset_type = 0;

static ssize_t show_headset(struct device *dev, struct device_attribute *attr
, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", conn_headset_type);
}

static ssize_t set_headset(struct device *dev, struct device_attribute *attr, 
const char *buf, size_t count)
{
    return 0;
}

static DEVICE_ATTR(headset, S_IRUGO | S_IWUSR, show_headset, set_headset);

static struct attribute *dev_attrs[] = {
    &dev_attr_headset.attr,
    NULL,
};
static struct attribute_group dev_attr_grp = {
    .attrs = dev_attrs,
};

static void
report_headset_switch(struct input_dev *dev, int key, int value)
{
    struct msm_handset *hs = input_get_drvdata(dev);

    input_report_switch(dev, key, value);
    switch_set_state(&hs->sdev, value);
}

#endif  //FEATURE_SKY_3_5PHIEARJACK

static struct msm_rpc_client *rpc_client;
static struct msm_handset *hs;

static int hs_find_key(uint32_t hscode)
{
	int i, key;

	key = HS_KEY(hscode, 0);

	for (i = 0; hs_key_map[i] != 0; i++) {
		if ((hs_key_map[i] & 0xff000000) == key)
			return hs_key_map[i] & 0x00ffffff;
	}
	return -1;
}

static void update_state(void)
{
	int state;

	if (hs->mic_on && hs->hs_on)
		state = 1 << 0;
	else if (hs->hs_on)
		state = 1 << 1;
	else if (hs->mic_on)
		state = 1 << 2;
	else
		state = 0;

	switch_set_state(&hs->sdev, state);
}

/*
 * tuple format: (key_code, key_param)
 *
 * old-architecture:
 * key-press = (key_code, 0)
 * key-release = (0xff, key_code)
 *
 * new-architecutre:
 * key-press = (key_code, 0)
 * key-release = (key_code, 0xff)
 */
static void report_hs_key(uint32_t key_code, uint32_t key_parm)
{
	int key, temp_key_code;
    #ifdef FEATURE_SKY_3_5PHIEARJACK
    static int connType;
    #endif

	if (key_code == HS_REL_K)
		key = hs_find_key(key_parm);
	else
		key = hs_find_key(key_code);

	temp_key_code = key_code;

	if (key_parm == HS_REL_K)
		key_code = key_parm;

	switch (key) {
	case KEY_POWER:
	case KEY_END:
	case KEY_MEDIA:
#ifdef FEATURE_SKY_3_5PHIEARJACK
	case KEY_VOLUMEUP:
	case KEY_VOLUMEDOWN:
       case KEY_SEND: // N1037 20120216 porting 
#endif
#ifdef FEATURE_SKY_CHG_LOGO
    case KEY_BATTERY:
#endif
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		break;
	case SW_HEADPHONE_INSERT_W_MIC:
		hs->mic_on = hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, SW_HEADPHONE_INSERT,
							hs->hs_on);
		input_report_switch(hs->ipdev, SW_MICROPHONE_INSERT,
							hs->mic_on);
		update_state();
		break;

	case SW_HEADPHONE_INSERT:
#ifdef FEATURE_SKY_3_5PHIEARJACK
        if(key_code == HS_REL_K) { // release
            switch(key_parm) {
                case HS_SKY_20PIN_STEREO_HEADSET_K:
                    conn_headset_type &= ~(1 << 0);
                    connType &= ~(1 << 0);
                    break;
                case HS_SKY_3P5PI_STEREO_HEADSET_K:
                    conn_headset_type &= ~(1 << 1);
                    connType &= ~(1 << 1);
                    break;
                case HS_SKY_3P5PI_STEREO_HEADSET_NO_MIC_K:
                    conn_headset_type &= ~(1 << 2);
                    connType &= ~(1 << 2);
                    break;
                case HS_SKY_MICROUSB_TDMB_K:
                    //conn_headset_type &= ~(1 << 3);
                    connType &= ~(1 << 3);
                    break;
            }
        }
        else { // insert
            switch(key_code) {
                case HS_SKY_20PIN_STEREO_HEADSET_K:
                    conn_headset_type |= (1 << 0);
                    connType |= (1 << 0);
                    break;
                case HS_SKY_3P5PI_STEREO_HEADSET_K:
                    conn_headset_type |= (1 << 1);
                    connType |= (1 << 1);
                    break;
                case HS_SKY_3P5PI_STEREO_HEADSET_NO_MIC_K:
                    conn_headset_type |= (1 << 2);
                    connType |= (1 << 2);
                    break;
                case HS_SKY_MICROUSB_TDMB_K:
                    //conn_headset_type |= (1 << 3);
                    connType |= (1 << 3);
                    break;
            }
        }

        report_headset_switch(hs->ipdev, key, connType);
        printk(KERN_ERR "(SKY-HEADSET) Headset Detection(key:0x%x, conn_headset_type:0x%x, connType:0x%x)\n", key, conn_headset_type, connType);
#else
		hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->hs_on);
		update_state();
#endif
		break;
	case SW_MICROPHONE_INSERT:
		hs->mic_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->mic_on);
		update_state();
		break;
	case -1:
		printk(KERN_ERR "%s: No mapping for remote handset event %d\n",
				 __func__, temp_key_code);
		return;
	}
	input_sync(hs->ipdev);
}

static int handle_hs_rpc_call(struct msm_rpc_server *server,
			   struct rpc_request_hdr *req, unsigned len)
{
	struct rpc_keypad_pass_key_code_args {
		uint32_t key_code;
		uint32_t key_parm;
	};

	switch (req->procedure) {
	case RPC_KEYPAD_NULL_PROC:
		return 0;

	case RPC_KEYPAD_PASS_KEY_CODE_PROC: {
		struct rpc_keypad_pass_key_code_args *args;

		args = (struct rpc_keypad_pass_key_code_args *)(req + 1);
		args->key_code = be32_to_cpu(args->key_code);
		args->key_parm = be32_to_cpu(args->key_parm);

		report_hs_key(args->key_code, args->key_parm);

		return 0;
	}

	case RPC_KEYPAD_SET_PWR_KEY_STATE_PROC:
		/* This RPC function must be available for the ARM9
		 * to function properly.  This function is redundant
		 * when RPC_KEYPAD_PASS_KEY_CODE_PROC is handled. So
		 * input_report_key is not needed.
		 */
		return 0;
	default:
		return -ENODEV;
	}
}

static struct msm_rpc_server hs_rpc_server = {
	.prog		= HS_SERVER_PROG,
	.vers		= HS_SERVER_VERS,
	.rpc_call	= handle_hs_rpc_call,
};

static int process_subs_srvc_callback(struct hs_event_cb_recv *recv)
{
	if (!recv)
		return -ENODATA;

	report_hs_key(be32_to_cpu(recv->key.code), be32_to_cpu(recv->key.parm));

	return 0;
}

static void process_hs_rpc_request(uint32_t proc, void *data)
{
	if (proc == HS_EVENT_CB_PROC)
		process_subs_srvc_callback(data);
	else
		pr_err("%s: unknown rpc proc %d\n", __func__, proc);
}

static int hs_rpc_report_event_arg(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	struct hs_event_rpc_req {
		uint32_t hs_event_data_ptr;
		struct hs_event_data data;
	};

	struct hs_event_rpc_req *req = buffer;

	req->hs_event_data_ptr	= cpu_to_be32(0x1);
	req->data.ver		= cpu_to_be32(HS_EVENT_DATA_VER);
	req->data.event_type	= cpu_to_be32(HS_EVNT_HSD);
	req->data.enum_disc	= cpu_to_be32(HS_EVNT_HSD);
	req->data.data_length	= cpu_to_be32(0x1);
	req->data.data		= cpu_to_be32(*(enum hs_src_state *)data);
	req->data.data_size	= cpu_to_be32(sizeof(enum hs_src_state));

	return sizeof(*req);
}

static int hs_rpc_report_event_res(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	enum hs_return_value result;

	result = be32_to_cpu(*(enum hs_return_value *)buffer);
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	if (result == HS_SUCCESS)
		return 0;

	return 1;
}

void report_headset_status(bool connected)
{
	int rc = -1;
	enum hs_src_state status;

	if (connected == true)
		status = HS_SRC_STATE_HI;
	else
		status = HS_SRC_STATE_LO;

	rc = msm_rpc_client_req(rpc_client, HS_REPORT_EVNT_PROC,
				hs_rpc_report_event_arg, &status,
				hs_rpc_report_event_res, NULL, -1);

	if (rc)
		pr_err("%s: couldn't send rpc client request\n", __func__);
}
EXPORT_SYMBOL(report_headset_status);

static int hs_rpc_pwr_cmd_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	struct hs_cmd_data_type *hs_pwr_cmd = buffer;

	hs_pwr_cmd->hs_cmd_data_type_ptr = cpu_to_be32(0x01);

	hs_pwr_cmd->ver = cpu_to_be32(0x03);
	hs_pwr_cmd->id = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->handle = cpu_to_be32(hs_subs_req->hs_handle_data);
	hs_pwr_cmd->disc_id1 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->input_ptr = cpu_to_be32(0x01);
	hs_pwr_cmd->input_val = cpu_to_be32(hs->hs_pdata->pwr_key_delay_ms);
	hs_pwr_cmd->input_len = cpu_to_be32(0x01);
	hs_pwr_cmd->disc_id2 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->output_len = cpu_to_be32(0x00);
	hs_pwr_cmd->delayed = cpu_to_be32(0x00);

	return sizeof(*hs_pwr_cmd);
}

static int hs_rpc_pwr_cmd_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_rpc_register_subs_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	hs_subs_req = buffer;

	hs_subs_req->hs_subs_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.ver	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.srvc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.req	= cpu_to_be32(HS_SUBS_REGISTER);
	hs_subs_req->hs_subs.host_os	= cpu_to_be32(0x4); /* linux */
	hs_subs_req->hs_subs.disc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.id.evnt	= cpu_to_be32(HS_EVNT_CLASS_ALL);

	hs_subs_req->hs_cb_id		= cpu_to_be32(0x1);

	hs_subs_req->hs_handle_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_handle_data	= cpu_to_be32(0x0);

	return sizeof(*hs_subs_req);
}

static int hs_rpc_register_subs_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_cb_func(struct msm_rpc_client *client, void *buffer, int in_size)
{
	int rc = -1;

	struct rpc_request_hdr *hdr = buffer;

	hdr->type = be32_to_cpu(hdr->type);
	hdr->xid = be32_to_cpu(hdr->xid);
	hdr->rpc_vers = be32_to_cpu(hdr->rpc_vers);
	hdr->prog = be32_to_cpu(hdr->prog);
	hdr->vers = be32_to_cpu(hdr->vers);
	hdr->procedure = be32_to_cpu(hdr->procedure);

	process_hs_rpc_request(hdr->procedure,
			    (void *) (hdr + 1));

	msm_rpc_start_accepted_reply(client, hdr->xid,
				     RPC_ACCEPTSTAT_SUCCESS);
	rc = msm_rpc_send_accepted_reply(client, 0);
	if (rc) {
		pr_err("%s: sending reply failed: %d\n", __func__, rc);
		return rc;
	}

	return 0;
}

static int __init hs_rpc_cb_init(void)
{
	int rc = 0, i, num_vers;

	num_vers = ARRAY_SIZE(rpc_vers);

	for (i = 0; i < num_vers; i++) {
		rpc_client = msm_rpc_register_client("hs",
			HS_RPC_PROG, rpc_vers[i], 0, hs_cb_func);

		if (IS_ERR(rpc_client))
			pr_debug("%s: RPC Client version %d failed, fallback\n",
				 __func__, rpc_vers[i]);
		else
			break;
	}

	if (IS_ERR(rpc_client)) {
		pr_err("%s: Incompatible RPC version error %ld\n",
			 __func__, PTR_ERR(rpc_client));
		return PTR_ERR(rpc_client);
	}

	rc = msm_rpc_client_req(rpc_client, HS_SUBSCRIBE_SRVC_PROC,
				hs_rpc_register_subs_arg, NULL,
				hs_rpc_register_subs_res, NULL, -1);
	if (rc) {
		pr_err("%s: RPC client request failed for subscribe services\n",
						__func__);
		goto err_client_req;
	}

	rc = msm_rpc_client_req(rpc_client, HS_PROCESS_CMD_PROC,
			hs_rpc_pwr_cmd_arg, NULL,
			hs_rpc_pwr_cmd_res, NULL, -1);
	if (rc)
		pr_err("%s: RPC client request failed for pwr key"
			" delay cmd, using normal mode\n", __func__);
	return 0;
err_client_req:
	msm_rpc_unregister_client(rpc_client);
	return rc;
}

static int __devinit hs_rpc_init(void)
{
	int rc;

	rc = hs_rpc_cb_init();
	if (rc) {
		pr_err("%s: failed to initialize rpc client, try server...\n",
						__func__);

		rc = msm_rpc_create_server(&hs_rpc_server);
		if (rc) {
			pr_err("%s: failed to create rpc server\n", __func__);
			return rc;
		}
	}

	return rc;
}

static void __devexit hs_rpc_deinit(void)
{
	if (rpc_client)
		msm_rpc_unregister_client(rpc_client);
}

static ssize_t msm_headset_print_name(struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(&hs->sdev)) {
	case NO_DEVICE:
		return sprintf(buf, "No Device\n");
	case MSM_HEADSET:
		return sprintf(buf, "Headset\n");
	}
	return -EINVAL;
}

static int car_kit=0;
#define CS_REQUIRED


#ifdef CS_REQUIRED

#define EARJACK_DETECT_POLLING_TIME					50		// 500msec
#define EARJACK_CARKIT_EVENT_POLLING_TIME		30		// 300msec
#define EARJACK_REMOTE_KEY_EVENT_POLLING_TIME		3		// 30msec
typedef enum{
	NONE_EARJACK_INSERTED = 0,
	POLAR3_EARJACK_INSERTED = 1,
	POLAR4_EARJACK_INSERTED = 2,
} Earjack_current_type;

typedef struct{
    Earjack_current_type type;
    uint16 min;
    uint16 max;   
} Earjack_current_state;

static Earjack_current_state current_earjack_type[] ={                       
    { NONE_EARJACK_INSERTED, 2110, 2300},
    { POLAR4_EARJACK_INSERTED,1600,  2070},
    { POLAR3_EARJACK_INSERTED, 0,  40},    
};

//static int earjack_state =0;
static int before_earjack_state=0;
static int current_earjack_state_check=0;
static int current_earjack_state=0;
static bool earjack_state_changed=false;
static bool remote_key_irq_State=false;

static int Earjack_FET_State = 0 ; // if 0 => failed , 1 => normal

static struct delayed_work CS_earjack_work;
static struct delayed_work CS_remotekey_work;

static void CS_earjack_det_func(struct work_struct * CS_earjack_work);
static void CS_remotekey_det_func(struct work_struct * CS_remotekey_work);
static irqreturn_t CS_Remotekey_Det_handler(int irq, void *dev_id);
static int check_analog_mpp(int channel,int *mv_reading);
static int key_adc_sensing(void);    

static struct wake_lock CS_earjack_wake_lock;
static struct wake_lock CS_remotekey_wake_lock;
static struct mutex CS_hs_mutex;

static irqreturn_t CS_Remotekey_Det_handler(int irq, void *dev_id)                 // isr_CS_Remotekey_Det_handler
{
  dbg_func_in();
  if(sky_hs_3p5pi_jack_ctrl.state!= SKY_HS_JACK_STATE_ON_4POLAR){
		dbg_func_out();
  	return IRQ_HANDLED;
  }
  disable_irq_nosync(gpio_to_irq(REMOTEKEY_DET));
	wake_lock_timeout(&CS_remotekey_wake_lock, 50);        
	schedule_delayed_work(&CS_remotekey_work,0);
	dbg_func_out();
  return IRQ_HANDLED;        
}


static void CS_earjack_det_func( struct work_struct *test_earjack)															// check earjack state          
{
	int nAdcValue;											// store adc value
	int i;															// for()
	int err;														// store return value
	bool state_changed = false;					 
	bool remotekey_irq_disabled = false;
	dbg_func_in();
	wake_lock(&CS_earjack_wake_lock);			// set wakelock
	mutex_lock(&CS_hs_mutex);							// set mutex_lock 

	if(released){																																								// if remote key press state
		dbg_earjack("[EARJACK] CS_earjack_det_func but earjack key press state\n");
		schedule_delayed_work(&CS_earjack_work,	EARJACK_DETECT_POLLING_TIME);
		wake_unlock(&CS_earjack_wake_lock);
		mutex_unlock(&CS_hs_mutex);
		return;		
	}

	if(sky_hs_3p5pi_jack_ctrl.state == SKY_HS_JACK_STATE_ON_4POLAR){
		remotekey_irq_disabled=true;
		disable_irq_nosync(gpio_to_irq(REMOTEKEY_DET));																							// because mpp3 mode change make remote_key irq
	}

       // N1037 20120306 proting 
	//pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);			// set mpp3 analog mode
	pm8058_mpp_config_AMUX();
	check_analog_mpp(CHANNEL_ADC_HDSET,&nAdcValue);                     													// read analog input mpp_3   
	// N1037 20120306 porting 
	//pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);
       pm8058_mpp_config_DIG();
	dbg_earjack("CS_earjack_det_func adc value => %d\n",nAdcValue);

	for( i = 0; i< ARR_SIZE( current_earjack_type ); i++ ) {																			// set current earjack state
  	if( nAdcValue >= current_earjack_type[i].min && nAdcValue <= current_earjack_type[i].max ){
			current_earjack_state=i;
			dbg_earjack("@@@@CS_earjack_det_func start~!! Earjack_state=> %d\n",sky_hs_3p5pi_jack_ctrl.state);
			break;
		}
	}
	if(i!=3){
		
		if(current_earjack_state==before_earjack_state){																							// if before = current
			earjack_state_changed=false;
		}
		else{																																													// if before != current
			if(!earjack_state_changed){
				current_earjack_state_check=current_earjack_state;																				// store temp state
				earjack_state_changed=true;
			}else{
				if(current_earjack_state == current_earjack_state_check){
					earjack_state_changed=false;
					state_changed = true;
					conn_headset_type = 0;
					current_earjack_state_check=-1;
					before_earjack_state=current_earjack_state;
					sky_hs_3p5pi_jack_ctrl.state=current_earjack_state;
					// to input event earjack state
					switch(before_earjack_state){
						case SKY_HS_JACK_STATE_OFF :
							report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);					// conn_headset_type = 0
	            dbg_earjack("@@@@ earjack conn_headset_type value => %d\n",conn_headset_type);
							printk("[EARJACK] NONE EARJACK STATE\n");
							break;
						case SKY_HS_JACK_STATE_ON_4POLAR :
							//conn_headset_type=(1 << SKY_HS_JACK_STATE_ON_4POLAR); 	// conn_headset_type =2
							conn_headset_type=(1 << 0); 	// conn_headset_type =1
							report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
							remote_key_irq_State=1;
	            dbg_earjack("@@@@ earjack conn_headset_type value => %d\n",conn_headset_type);
							printk("[EARJACK] SKY_HS_JACK_STATE_ON_4POLAR STATE\n");
							break;
						case SKY_HS_JACK_STATE_ON_3POLAR :
							//conn_headset_type=(1 << SKY_HS_JACK_STATE_ON_3POLAR);	     // conn_headset_type =4
							conn_headset_type=(1 << 1);	     // conn_headset_type =2
							report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
	            dbg_earjack("@@@@ earjack conn_headset_type value => %d\n",conn_headset_type);
							printk("[EARJACK] SKY_HS_JACK_STATE_ON_3POLAR STATE\n");
							break;
						default :
							break;
					}							
				}
				else{
					earjack_state_changed=false;
					current_earjack_state_check=sky_hs_3p5pi_jack_ctrl.state;
				}				
			}				
		}
	}
	else{
		printk("\n\n earjack function none\n");
	}
		
	
	if(state_changed){
		if(sky_hs_3p5pi_jack_ctrl.state == SKY_HS_JACK_STATE_ON_4POLAR){
			err=request_threaded_irq(gpio_to_irq(REMOTEKEY_DET),NULL,CS_Remotekey_Det_handler, IRQF_TRIGGER_FALLING, "remote_det-irq", hs);
			if(err) dbg_earjack("request_threaded_irq failed\n");
		}
		else if(remote_key_irq_State){
			free_irq(gpio_to_irq(REMOTEKEY_DET),hs);
		}
	}
	if(remotekey_irq_disabled){
  	enable_irq(gpio_to_irq(REMOTEKEY_DET));																												// enable remote key irq	
	}
	schedule_delayed_work(&CS_earjack_work,	EARJACK_DETECT_POLLING_TIME);
	wake_unlock(&CS_earjack_wake_lock);
	mutex_unlock(&CS_hs_mutex);
	dbg_func_out();							
}

static void CS_remotekey_det_func(struct work_struct * test_CS_remotekey_work)
{
	int nAdcValue=0;											// store adc value
	int key_first=0,key_second=0;    

	dbg_func_in();	
	mutex_lock(&CS_hs_mutex);
	if(Earjack_FET_State){
		car_kit = 0;
		released = 0;
		mutex_unlock(&CS_hs_mutex);
              disable_irq_nosync(gpio_to_irq(REMOTEKEY_DET)); // N1037 20120309 정상 상황에서 에러 수정 
		enable_irq(gpio_to_irq(REMOTEKEY_DET));
	}

       // N1037 20120306 proting 
	//pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);		// set mpp3 analog mode
	pm8058_mpp_config_AMUX();
	check_analog_mpp(CHANNEL_ADC_HDSET,&nAdcValue);   
       // N1037 20120306 porting 
	//pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);				// set mpp3 digital mode
	pm8058_mpp_config_DIG();
	
	if(nAdcValue > current_earjack_type[NONE_EARJACK_INSERTED].min){														// if earjack is released
		car_kit = 0;
		released = 0;
		mutex_unlock(&CS_hs_mutex);
		enable_irq(gpio_to_irq(REMOTEKEY_DET));                     
		return ;
	}
	
///////////////////////////////////////////////
// 							if car_kit

	if(car_kit){
		dbg_earjack(" car kit =1 ");        
		input_report_key(hs->ipdev, KEY_MEDIA, released);		// car kit button down
		car_kit=0;
		if(gpio_get_value_cansleep(REMOTEKEY_DET)==TRUE){
			released=0;
			input_report_key(hs->ipdev, KEY_MEDIA, released);	// car kit button up
			dbg_earjack("@@@@ remote KKEY_MEDIA(car kit) key released~!!\n");
			mutex_unlock(&CS_hs_mutex);
			enable_irq(gpio_to_irq(REMOTEKEY_DET));
			dbg_func_out();
			return ;
  	}
		schedule_delayed_work(&CS_remotekey_work,EARJACK_REMOTE_KEY_EVENT_POLLING_TIME);		// polling 100ms
		wake_lock_timeout(&CS_remotekey_wake_lock, 50); 
		mutex_unlock(&CS_hs_mutex);
		return ;				
	}
	else {
		car_kit=0;			
	}

///////////////////////////////////////////////
// 							check if long key event
	if(released){																																				// check if long key down
		key_first=key_adc_sensing();
         	if(key_first == remote_id){
			schedule_delayed_work(&CS_remotekey_work,EARJACK_REMOTE_KEY_EVENT_POLLING_TIME);		// polling 100ms
			wake_lock_timeout(&CS_remotekey_wake_lock, 50); 
			mutex_unlock(&CS_hs_mutex);            
			return ;
		}
	}

//////////////////////////////////////////////
//							first event
	if(released==0){																																						// if key down
	// N1037 20120306 poting 
       //pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);		// set mpp3 analog mode
       pm8058_mpp_config_AMUX();
	key_first=key_adc_sensing();		     
	key_second=key_adc_sensing();     
       // N1037 20120306 porting 
	//pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);				// set mpp3 digital mode
	pm8058_mpp_config_DIG();
	if((key_first==key_second)){
    remote_id = key_first;
		remote_key = remode_3p5Pi_key_event[key_first].remode_3p5Pi_key;
    released=1;
    switch (remote_id){
        case 1 :    {
            dbg_earjack("@@@@ remote KKEY_MEDIA key insert~!!\n");
            input_report_key(hs->ipdev, KEY_MEDIA, released);
						schedule_delayed_work(&CS_remotekey_work,EARJACK_REMOTE_KEY_EVENT_POLLING_TIME);		// polling 30ms
						wake_lock_timeout(&CS_remotekey_wake_lock, 50); 
						mutex_unlock(&CS_hs_mutex);
						return;
            break;
            }
        case 2 :    {
           dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key insert~!!\n");
           input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
					 schedule_delayed_work(&CS_remotekey_work,EARJACK_REMOTE_KEY_EVENT_POLLING_TIME);		// polling 30ms
					 wake_lock_timeout(&CS_remotekey_wake_lock, 50); 
					 mutex_unlock(&CS_hs_mutex);
					 return;
           break;
            }
        case 3 :    {
            dbg_earjack("@@@@ remote KEY_VOLUMEUP key insert~!!\n");
            input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
						schedule_delayed_work(&CS_remotekey_work,EARJACK_REMOTE_KEY_EVENT_POLLING_TIME);		// polling 30ms
						wake_lock_timeout(&CS_remotekey_wake_lock, 50); 
						mutex_unlock(&CS_hs_mutex);
						return;
            break;
            }
				case 4 :    {
            dbg_earjack("@@@@ remote KKEY_MEDIA(car kit) key insert~!!\n");
						schedule_delayed_work(&CS_remotekey_work,EARJACK_CARKIT_EVENT_POLLING_TIME);				// for car kit test delay 300ms
						wake_lock_timeout(&CS_remotekey_wake_lock, 350); 
						car_kit=1;
						mutex_unlock(&CS_hs_mutex);
						return;
            break;
            }
        default :   {
						released=0;
            dbg_earjack("@@@@ remote NONE key insert~!!\n");
            }
   		}
  }
	else{		
		enable_irq(gpio_to_irq(REMOTEKEY_DET));
		mutex_unlock(&CS_hs_mutex);
		dbg_func_out();
    return ;
	}	     

	if(gpio_get_value_cansleep(REMOTEKEY_DET)==TRUE && released){															// 
		released=0;
		cancel_delayed_work(&CS_remotekey_work);
    switch(remote_id){
			case 1 :    {
          dbg_earjack("@@@@ remote KEY_MEDIA key released~!!\n");
          input_report_key(hs->ipdev, KEY_MEDIA, released);
          break;
          }
      case 2 :    {
          dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key released~!!\n");
          input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
          break;
          }
      case 3 :    {
          dbg_earjack("@@@@ remote KEY_VOLUMEUP key released~!!\n");
          input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
          break;
          }
			case 4 :    {
          dbg_earjack("@@@@ remote KEY_MEDIA (car kit) key released~!!\n");
          input_report_key(hs->ipdev, KEY_MEDIA, released);
          break;
          }
  		default :   {
          break;
          }
      }
    }
  }
///////////////////////////////////////////////
// 						key release event
  else if(released==1){
    released=0;
    switch(remote_id){
        case 1 :    {
            dbg_earjack("@@@@ remote KEY_MEDIA key released~!!\n");
            input_report_key(hs->ipdev, KEY_MEDIA, released);
            break;
            }
        case 2 :    {
            dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key released~!!\n");
            input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
            break;
            }
        case 3 :    {
            dbg_earjack("@@@@ remote KEY_VOLUMEUP key released~!!\n");
            input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
            break;
            }
				case 4 :    {
            dbg_earjack("@@@@ remote KEY_MEDIA (car kit) key released~!!\n");
            input_report_key(hs->ipdev, KEY_MEDIA, released);
            break;
            }
        default :   {
            break;
            }
        }
  }        
  else{
    dbg_earjack("CS_remotekey_det_func released error end\n");        
  	mutex_unlock(&CS_hs_mutex);    
		enable_irq(gpio_to_irq(REMOTEKEY_DET));
		dbg_func_out();
		return;
  }
  input_sync(hs->ipdev);
  dbg_earjack("CS_remotekey_det_func end\n");    
	mutex_unlock(&CS_hs_mutex);    
	enable_irq(gpio_to_irq(REMOTEKEY_DET));
	dbg_func_out();
  return;
}


#endif

#ifdef FEATURE_SKY_3_5PHIEARJACK
// N1037 20120216 porting (+)
static irqreturn_t Earjack_Det_handler(int irq, void *dev_id); 
static irqreturn_t Remotekey_Det_handler(int irq, void *dev_id);
// N1037 20120216 porting (-)
static struct wake_lock earjack_wake_lock;
static struct wake_lock remotekey_wake_lock;

// N1037 20120216 porting (+)
static struct mutex hs_mutex; 
static struct mutex hs_remotekey_mutex;
//static int car_kit=0;
// N1037 20120216 porting (-)

static irqreturn_t Earjack_Det_handler(int irq, void *dev_id)
{
    
        disable_irq_nosync(gpio_to_irq(EARJACK_DET));
        wake_lock(&earjack_wake_lock);
        schedule_delayed_work(&earjack_work,10);                        // after 100ms start function of earjack_det_func
        dbg_earjack("earjack_det_handler end\n");
        return IRQ_HANDLED;
}


static void earjack_det_func( struct work_struct *test_earjack)         
{
    int err;
    if(!Earjack_FET_State && !gpio_get_value(EARJACK_DET)){
    	mutex_lock(&CS_hs_mutex);							// set mutex_lock 
    	printk("[EARJACK] earjack board fet is OK\n");
    	Earjack_FET_State=true;
    	if(sky_hs_3p5pi_jack_ctrl.state==SKY_HS_JACK_STATE_ON_4POLAR){
    		free_irq(gpio_to_irq(REMOTEKEY_DET),hs);
              cancel_delayed_work(&CS_remotekey_work);
    	}
    	conn_headset_type = 0;
    	report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);	
    	sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;
    	err=regulator_is_enabled(hs_jack_l8);															// bias_disable
       dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
       if(err>0) regulator_disable(hs_jack_l8);
    	cancel_delayed_work(&CS_earjack_work);														// disable polling
    	//schedule_delayed_work(&earjack_work,10);
    	mutex_unlock(&CS_hs_mutex);							// set mutex_lock     	
    }
			
		
    dbg_earjack("@@@@earjack_det_func start~!! Earjack_state=> %d\n",sky_hs_3p5pi_jack_ctrl.state);
    switch(sky_hs_3p5pi_jack_ctrl.state){
        case    SKY_HS_JACK_STATE_INIT :    {
           dbg_earjack("@@@@earjack_insert_func SKY_HS_JACK_STATE_INIT START\n");
#if IS_EF40K /*yjw*/
           if(gpio_get_value(EARJACK_DET)==TRUE){
#else
           #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
           if(gpio_get_value(EARJACK_DET)==TRUE){
           #else
           if(gpio_get_value(EARJACK_DET)!=TRUE){
           #endif
#endif		   
                sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_ON_CHECK;
                err=regulator_is_enabled(hs_jack_l8);
               if(err<=0)  err = regulator_enable(hs_jack_l8);
               
               // N1037 20120222 porting
               //pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);
                pm8058_mpp_config_DIG();
               schedule_delayed_work(&earjack_work,5);
            }           
			else {
				sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;
                enable_irq(gpio_to_irq(EARJACK_DET));
                wake_unlock(&earjack_wake_lock);
			}
			break;
            }
        case    SKY_HS_JACK_STATE_OFF : {
            dbg_earjack("@@@@earjack_insert_func SKY_HS_JACK_STATE_OFF START\n");
#if IS_EF40K /*yjw*/
            if(gpio_get_value(EARJACK_DET)==TRUE){
#else
            #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
            if(gpio_get_value(EARJACK_DET)==TRUE){
            #else
            if(gpio_get_value(EARJACK_DET)!=TRUE){
            #endif
#endif			
            	sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_ON_CHECK;
                err=regulator_is_enabled(hs_jack_l8);
                if(err<=0)  err = regulator_enable(hs_jack_l8);                
                
                // N1037 20120222 porting
                //pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);
                pm8058_mpp_config_DIG();
                schedule_delayed_work(&earjack_work,5);
			}
            else{                
                enable_irq(gpio_to_irq(EARJACK_DET));      
                wake_unlock(&earjack_wake_lock);
                }
			break;
            }
        case    SKY_HS_JACK_STATE_ON_CHECK  :   {
            dbg_earjack("@@@@earjack_insert_func SKY_HS_JACK_STATE_ON_CHECK START\n");   
#if IS_EF40K /*yjw*/
            if(gpio_get_value(EARJACK_DET)==TRUE){
#else
            #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
            if(gpio_get_value(EARJACK_DET)==TRUE){
            #else
            if(gpio_get_value(EARJACK_DET)!=TRUE){
            #endif
#endif			
                
                if (gpio_cansleep(REMOTEKEY_DET)){
                 	err=gpio_get_value_cansleep(REMOTEKEY_DET);
                    dbg_earjack("gpio_get_value_cansleep(REMOTEKEY_DET) start\n");                    
                 }else{
                 	err=gpio_get_value(REMOTEKEY_DET);
                    dbg_earjack("gpio_get_value(REMOTEKEY_DET) start\n");
                }
               
                dbg_earjack("@@@@ PM8058_MPP3 value => %d\n",err);
                if(!err){
                    dbg_earjack("@@@@ 3pole earjack insert~!!\n");
                    sky_hs_3p5pi_jack_ctrl.state= SKY_HS_JACK_STATE_ON_3POLAR_CHECK;
                    conn_headset_type |= (1 << 1); // N1037 20120222 3±Ø conn_headset_type = 2 
                    schedule_delayed_work(&earjack_work,60);                                   // CHECK IF 4POLAR EARJACK IS INSERTING
                    report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
                 	dbg_earjack("@@@@conn_headset_type value => %d\n",conn_headset_type);
					}
                else {
                    dbg_earjack("@@@@ 4pole earjack insert~!!\n");
                    sky_hs_3p5pi_jack_ctrl.state= SKY_HS_JACK_STATE_ON_4POLAR;
                    err=request_threaded_irq(gpio_to_irq(REMOTEKEY_DET),NULL,Remotekey_Det_handler,IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "remote_det-irq", hs);
                    if(err) dbg_earjack("request_threaded_irq failed\n");
                    conn_headset_type |= (1 << 0); // N1037 20120222 4±Ø conn_headset_type = 1 
					report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
                 	dbg_earjack("@@@@conn_headset_type value => %d\n",conn_headset_type);
					enable_irq(gpio_to_irq(EARJACK_DET));
                    wake_unlock(&earjack_wake_lock);
                    
					}
                 
                }
            else{
                dbg_earjack("@@@@earjack_rel_func SKY_HS_JACK_STATE_ON_CHECK START\n");
                sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;                
				enable_irq(gpio_to_irq(EARJACK_DET));
                wake_unlock(&earjack_wake_lock);
				}
            
            break;   
            }

            case    SKY_HS_JACK_STATE_ON_3POLAR_CHECK  :   {                        // CHECKING IF 4POLAR EARJACK IS INSERTIND?
            dbg_earjack("@@@@earjack_insert_func SKY_HS_JACK_STATE_ON_3POLAR_CHECK START\n");   
#if IS_EF40K /*yjw*/
            if(gpio_get_value(EARJACK_DET)==TRUE){
#else
            #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
            if(gpio_get_value(EARJACK_DET)==TRUE){
            #else
            if(gpio_get_value(EARJACK_DET)!=TRUE){
            #endif
#endif			
               
                if (gpio_cansleep(REMOTEKEY_DET)){
                 	err=gpio_get_value_cansleep(REMOTEKEY_DET);              
                 }else{
                 	err=gpio_get_value(REMOTEKEY_DET);
                }
           
                if(!err){
                    dbg_earjack("@@@@ 3pole earjack insert~!!\n");
                    
                    sky_hs_3p5pi_jack_ctrl.state= SKY_HS_JACK_STATE_ON_3POLAR;                   
                    err=regulator_is_enabled(hs_jack_l8);
                    dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
                    if(err>0) regulator_disable(hs_jack_l8);
                                 
                    }
                else {
                    dbg_earjack("@@@@ 4pole earjack insert~!!\n");
                    
                    sky_hs_3p5pi_jack_ctrl.state= SKY_HS_JACK_STATE_ON_4POLAR;
                    err=request_threaded_irq(gpio_to_irq(REMOTEKEY_DET),NULL,Remotekey_Det_handler,IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "remote_det-irq", hs);
                    if(err) dbg_earjack("request_threaded_irq failed\n");
                    conn_headset_type &= ~(3 << 0); // N1037 20120222  conn_headset_type = 0 
                    conn_headset_type |= (1 << 0); // N1037 20120222 4±Ø conn_headset_type = 1 
                    report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
                    dbg_earjack("@@@@conn_headset_type value => %d\n",conn_headset_type);
                    }                 
                 }
            else{
                dbg_earjack("@@@@earjack_rel_func SKY_HS_JACK_STATE_ON_CHECK START\n");
                 
                err=regulator_is_enabled(hs_jack_l8);
                dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
                if(err>0) regulator_disable(hs_jack_l8);
                sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;
				conn_headset_type &= ~(3 << 0); // N1037 20120222  conn_headset_type = 0 
               	report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
				
                }
			
            enable_irq(gpio_to_irq(EARJACK_DET));
            wake_unlock(&earjack_wake_lock);
            break;   
            }
            
        case SKY_HS_JACK_STATE_ON_3POLAR    :   {
            dbg_earjack("@@@@earjack_rel_func SKY_HS_JACK_STATE_ON_3POLAR START\n");
#if IS_EF40K /*yjw*/
            if(gpio_get_value(EARJACK_DET)==FALSE){
#else
            #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
            if(gpio_get_value(EARJACK_DET)==FALSE){
            #else
            if(gpio_get_value(EARJACK_DET)!=FALSE){
            #endif
#endif			
                sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;
                conn_headset_type &= ~(3 << 0); // N1037 20120222  conn_headset_type = 0 
                report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type); 
                dbg_earjack("@@@@conn_headset_type value => %d\n",conn_headset_type);
                }
            enable_irq(gpio_to_irq(EARJACK_DET));
            wake_unlock(&earjack_wake_lock);
    	    break;
            }                    
        case SKY_HS_JACK_STATE_ON_4POLAR    :   {
            dbg_earjack("@@@@earjack_rel_func SKY_HS_JACK_STATE_ON_4POLAR START\n");
#if IS_EF40K /*yjw*/
            if(gpio_get_value(EARJACK_DET)==FALSE){
#else
            #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
            if(gpio_get_value(EARJACK_DET)==FALSE){
            #else
            if(gpio_get_value(EARJACK_DET)!=FALSE){
            #endif
            #endif
                free_irq(gpio_to_irq(REMOTEKEY_DET), hs);
                err=regulator_is_enabled(hs_jack_l8);
                dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
                if(err>0){
                    regulator_disable(hs_jack_l8);
                    }                 
                if(released==1){
                    released=0;
                    switch(remote_id){
                        case 1 :    {
                            dbg_earjack("@@@@ remote KEY_MEDIA key released~!!\n");
                            input_report_key(hs->ipdev, KEY_MEDIA, released);
                            break;
                            }
                        case 2 :    {
                            dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key released~!!\n");
                            input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
                            break;
                            }
                        case 3 :    {
                            dbg_earjack("@@@@ remote KEY_VOLUMEUP key released~!!\n");
                            input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
                            break;
                            }
                        default :   {
                            break;
                            }
                        }
                    input_sync(hs->ipdev);
                }                            
                sky_hs_3p5pi_jack_ctrl.state = SKY_HS_JACK_STATE_OFF;
                conn_headset_type &= ~(3 << 0); // N1037 20120222  conn_headset_type = 0 
                report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, conn_headset_type);
                dbg_earjack("@@@@conn_headset_type value => %d\n",conn_headset_type);
                dbg_earjack("@@@@earjack_release_4POLAR earjack\n");
                //disable_irq(REMOTEKEY_DET);
                
			    } 
            enable_irq(gpio_to_irq(EARJACK_DET));    
            wake_unlock(&earjack_wake_lock);
			break;
            }
        default :   {
            dbg_earjack("@@@@earjack_rel_func default START\n");
            enable_irq(gpio_to_irq(EARJACK_DET));
            wake_unlock(&earjack_wake_lock);
            }
        }
    dbg_earjack("earjack_det_func end\n");
    return;
}


static int check_analog_mpp(int channel,int *mv_reading)                   // read adc value 
{	
       int ret;
	void *h;
	struct adc_chan_result adc_chan_result;
	struct completion  conv_complete_evt;
    
    //dbg_func_in();
    
    ret = adc_channel_open(channel, &h);
    if (ret) {
		pr_err("%s: couldnt open channel %d ret=%d\n",
					__func__, channel, ret);
		goto out;
	}
	init_completion(&conv_complete_evt);
	ret = adc_channel_request_conv(h, &conv_complete_evt);
	if (ret) {
		pr_err("%s: couldnt request conv channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}

	wait_for_completion(&conv_complete_evt);
/*
	ret = wait_for_completion_interruptible(&conv_complete_evt);
	if (ret) {
		pr_err("%s: wait interrupted channel %d ret=%d\n",
						__func__, channel, ret);
		printk("wait interrupted channel \n");
		goto out;
	}
*/
	ret = adc_channel_read_result(h, &adc_chan_result);
	if (ret) {
		pr_err("%s: couldnt read result channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
	ret = adc_channel_close(h);
	if (ret) {
		pr_err("%s: couldnt close channel %d ret=%d\n",
						__func__, channel, ret);
	}
	if (mv_reading)
		*mv_reading = adc_chan_result.measurement;

	pr_debug("%s: done for %d\n", __func__, channel);
	//dbg_func_out();
	

	return adc_chan_result.physical;
out:
	pr_debug("%s: done for %d\n", __func__, channel);
	dbg_func_out();
	return -EINVAL;
}


static int key_adc_sensing(void)                                                // detect key sensing                                 
{	
    int nAdcValue,i;
    dbg_func_in();

    check_analog_mpp(CHANNEL_ADC_HDSET,&nAdcValue);                     // read analog input mpp_3    
    dbg_earjack("@@@@analog intput mpp_03 value => %d\n",nAdcValue);
//printk("@@@@analog intput mpp_03 value => %d\n",nAdcValue);
    for( i = 1; i< ARR_SIZE( remode_3p5Pi_key_event ); i++ ) {
		if( nAdcValue >= remode_3p5Pi_key_event[i].min && nAdcValue <= remode_3p5Pi_key_event[i].max ) {
			dbg_func_out();
			return i;
		    }
	    }        
				return 0 ;
}


static void remotekey_det_func(struct work_struct * test_remotekey_work)
{
    	int nAdcValue=0;		
       int key_first=0,key_second=0; 
        
	dbg_func_in();
    if(sky_hs_3p5pi_jack_ctrl.state!= SKY_HS_JACK_STATE_ON_4POLAR){
        enable_irq(gpio_to_irq(REMOTEKEY_DET));
        //wake_unlock(&remotekey_wake_lock);
		dbg_func_out();
        return ;
        }
    
       // N1037 20120306 proting 
	//pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);		// set mpp3 analog mode
	pm8058_mpp_config_AMUX();
	check_analog_mpp(CHANNEL_ADC_HDSET,&nAdcValue);      
       // N1037 20120306 porting 
	//pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);				// set mpp3 digital mode
	pm8058_mpp_config_DIG();
	
	if(nAdcValue > remode_3p5Pi_key_event[REMOTE_3P5PI_KEY_NONE].min){														// if earjack is released
		car_kit = 0;
		released = 0;		
		enable_irq(gpio_to_irq(REMOTEKEY_DET));             
		return ;
	}
       
	if(car_kit ){
#if IS_EF40K /*yjw*/
    if(gpio_get_value_cansleep(EARJACK_DET)==TRUE){
#else	
    #if (EF33S_BDVER_GE(WS20) | EF34K_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20) | EF35L_BDVER_GE(WS20))
    if(gpio_get_value_cansleep(EARJACK_DET)==TRUE){
    #else
    if(gpio_get_value_cansleep(EARJACK_DET)!=TRUE){
    #endif     
#endif
		dbg_earjack(" car kit =1 ");        
		input_report_key(hs->ipdev, KEY_MEDIA, released);		// car kit button down
		car_kit=0;
		if(gpio_get_value_cansleep(REMOTEKEY_DET)==TRUE){
			released=0;
			input_report_key(hs->ipdev, KEY_MEDIA, released);	// car kit button up
			}
	    }
		car_kit=0;
		enable_irq(gpio_to_irq(REMOTEKEY_DET));
		dbg_func_out();
		return ;
	}
		
	//mutex_lock(&hs_remotekey_mutex);
    dbg_earjack("remotekey_det_func start\n");    
   
    if(released==0){
        dbg_earjack("!gpio_get_value(REMOTEKEY_DET) && released==FALSE && !gpio_get_value(EARJACK_DET)\n");
        
        // N1037 20120222 porting
        //pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);
        pm8058_mpp_config_AMUX();

		key_first=key_adc_sensing();
		key_second=key_adc_sensing();        
        pm8058_mpp_config_DIG();
		if((key_first==key_second) && (gpio_get_value_cansleep(EARJACK_DET)==TRUE)){
            remote_id = key_first;
			remote_key = remode_3p5Pi_key_event[key_first].remode_3p5Pi_key;
            released=1;
            switch (remote_id){
                case 1 :    {
                    dbg_earjack("@@@@ remote KKEY_MEDIA key insert~!!\n");
                    input_report_key(hs->ipdev, KEY_MEDIA, released);
                    break;
                    }
                case 2 :    {
                   dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key insert~!!\n");
                   input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
                   break;
                    }
                case 3 :    {
                    dbg_earjack("@@@@ remote KEY_VOLUMEUP key insert~!!\n");
                    input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
                    break;
                    }
				case 4 :    {
                    dbg_earjack("@@@@ remote KKEY_MEDIA(car kit) key insert~!!\n");
					schedule_delayed_work(&remotekey_work,20);				// for car kit test delay 300ms
					car_kit=1;
                    //input_report_key(hs->ipdev, KEY_MEDIA, released);
                    break;
                    }
                default :   {
					released=0;
                    dbg_earjack("@@@@ remote NONE key insert~!!\n");
                    }
                }
            
            }
		else{			
                     // N1037 20120222 porting
                     //pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);                     
			enable_irq(gpio_to_irq(REMOTEKEY_DET));
			//wake_unlock(&remotekey_wake_lock);
			dbg_func_out();
	        return ;
		}	
        
         // N1037 20120222 porting
         //pm8058_mpp_config_digital_in(XOADC_MPP_3,PM8058_MPP_DIG_LEVEL_S3, PM_MPP_DIN_TO_INT);
	if(gpio_get_value_cansleep(REMOTEKEY_DET)==TRUE && released){
			released=0;
        switch(remote_id){
            case 1 :    {
                dbg_earjack("@@@@ remote KEY_MEDIA key released~!!\n");
                input_report_key(hs->ipdev, KEY_MEDIA, released);
                break;
                }
            case 2 :    {
                dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key released~!!\n");
                input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
                break;
                }
            case 3 :    {
                dbg_earjack("@@@@ remote KEY_VOLUMEUP key released~!!\n");
                input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
                break;
                }
			case 4 :    {
                dbg_earjack("@@@@ remote KEY_MEDIA (car kit) key released~!!\n");
                input_report_key(hs->ipdev, KEY_MEDIA, released);
                break;
                }
            default :   {
                break;
                }
            }
        }
    }
    else if(released==1){
        released=0;
        switch(remote_id){
            case 1 :    {
                dbg_earjack("@@@@ remote KEY_MEDIA key released~!!\n");
                input_report_key(hs->ipdev, KEY_MEDIA, released);
                break;
                }
            case 2 :    {
                dbg_earjack("@@@@ remote KEY_VOLUMEDOWN key released~!!\n");
                input_report_key(hs->ipdev, KEY_VOLUMEDOWN, released);
                break;
                }
            case 3 :    {
                dbg_earjack("@@@@ remote KEY_VOLUMEUP key released~!!\n");
                input_report_key(hs->ipdev, KEY_VOLUMEUP, released);
                break;
                }
			case 4 :    {
                dbg_earjack("@@@@ remote KEY_MEDIA (car kit) key released~!!\n");
                input_report_key(hs->ipdev, KEY_MEDIA, released);
                break;
                }
            default :   {
                break;
                }
            }
        }        
    else{
        dbg_earjack("remotekey_det_func released error end\n");        
        //wake_unlock(&remotekey_wake_lock);
        //mutex_unlock(&hs_remotekey_mutex);
		enable_irq(gpio_to_irq(REMOTEKEY_DET));
		dbg_func_out();
        return;
        }

	
    input_sync(hs->ipdev);
    dbg_earjack("remotekey_det_func end\n");    
    //wake_unlock(&remotekey_wake_lock);
    //mutex_unlock(&hs_remotekey_mutex);
	enable_irq(gpio_to_irq(REMOTEKEY_DET));
	dbg_func_out();
    return;
}


static irqreturn_t Remotekey_Det_handler(int irq, void *dev_id)                 // isr_Remotekey_Det_handler
{
        dbg_func_in();
        if(sky_hs_3p5pi_jack_ctrl.state!= SKY_HS_JACK_STATE_ON_4POLAR){
			dbg_func_out();
            return IRQ_HANDLED;
        }
        dbg_earjack("@@@@Remote_Int_handler start~!! \n");
        disable_irq_nosync(gpio_to_irq(REMOTEKEY_DET));
		wake_lock_timeout(&remotekey_wake_lock, 50);        
		//wake_lock(&remotekey_wake_lock);
        schedule_delayed_work(&remotekey_work,0);
		dbg_func_out();
        return IRQ_HANDLED;        
}

static void isInserted(void)                                                        // check 3.5 earjack inserted in boot
{    
    int err;
    dbg_earjack("isInserted func start\n");
    disable_irq_nosync(gpio_to_irq(EARJACK_DET));
    if(gpio_get_value(EARJACK_DET)){
        enable_irq(gpio_to_irq(EARJACK_DET));
        // If earjack detect pin is high, FET IC is failed or .
        INIT_DELAYED_WORK(&CS_earjack_work,CS_earjack_det_func);          // INIT WORK
        INIT_DELAYED_WORK(&CS_remotekey_work,CS_remotekey_det_func); 
        err=regulator_is_enabled(hs_jack_l8);
        dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
        if(err<=0) regulator_enable(hs_jack_l8);													// P13106 CS_REQUIRED

        wake_lock_init(&CS_earjack_wake_lock, WAKE_LOCK_SUSPEND, "CS_earjack_wake_lock_init");
        wake_lock_init(&CS_remotekey_wake_lock, WAKE_LOCK_SUSPEND, "CS_remotekey_wake_lock_init");
        mutex_init(&CS_hs_mutex);                      // Init Mutex

        // N1037 20120306 porting 
        //pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);		// set mpp3 analog mode for first adc read
        pm8058_mpp_config_AMUX();
        schedule_delayed_work(&CS_earjack_work,20);   

    }else{
        printk("\n\n earjack detection is low & FET is OK\n");
        // If earjack detect pin is low, FET IC is OK 
        Earjack_FET_State = 1;
        wake_lock(&earjack_wake_lock);
        schedule_delayed_work(&earjack_work,10);                        // after 100ms start function of earjack_det_func
        dbg_earjack("earjack_det_handler end\n");
        return ;
    }
}
// jeongeubin 20120216 porting (-)

static int hs_earjack_suspend(struct platform_device * pdev, pm_message_t state)
{
    dbg_earjack("hs_earjack_suspend start~!!\n");
	if(!Earjack_FET_State){
		cancel_delayed_work(&CS_earjack_work);
		if(sky_hs_3p5pi_jack_ctrl.state != SKY_HS_JACK_STATE_ON_4POLAR){
			regulator_disable(hs_jack_l8);
		}	
	earjack_state_changed=false;
	}
    return 0;
}

#ifdef MODEL_SKY	// FOR ICE-SCREAM
static int hs_earjack_resume(struct platform_device * pdev)
#else
static int hs_earjack_resume(struct platform_device * pdev, pm_message_t state)
#endif
{
	int err;
    dbg_earjack("hs_earjack_resume start~!!\n");
	if(!Earjack_FET_State){
		err=regulator_is_enabled(hs_jack_l8);
  	dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
		if(err<=0) regulator_enable(hs_jack_l8);										// P13106 CS_REQUIRED
		schedule_delayed_work(&CS_earjack_work,5);   
	}
    return 0;
}
#endif

static int __devinit hs_probe(struct platform_device *pdev)
{
	int rc = 0;
       int err = 0;
       int ret = 0;
	struct input_dev *ipdev;

#ifdef FEATURE_SKY_3_5PHIEARJACK
    sky_hs_3p5pi_jack_ctrl.state=SKY_HS_JACK_STATE_INIT;
#endif
	hs = kzalloc(sizeof(struct msm_handset), GFP_KERNEL);
	if (!hs)
		return -ENOMEM;

//#ifdef MODEL_SKY
// change from "h2w because same name pmic8058-othc.c PS2 P13106
//	hs->sdev.name	= "hw2";                        
//#else
	hs->sdev.name	= "h2w";
//#endif
	hs->sdev.print_name = msm_headset_print_name;

	rc = switch_dev_register(&hs->sdev);
	if (rc)
		goto err_switch_dev_register;

	ipdev = input_allocate_device();
	if (!ipdev) {
		rc = -ENOMEM;
		goto err_alloc_input_dev;
	}
	input_set_drvdata(ipdev, hs);

	hs->ipdev = ipdev;

	if (pdev->dev.platform_data)
		hs->hs_pdata = pdev->dev.platform_data;

	if (hs->hs_pdata->hs_name)
		ipdev->name = hs->hs_pdata->hs_name;
	else
		ipdev->name	= DRIVER_NAME;

#ifdef TEST_HANDSET // N1037 20120216 porting (+)
    {        
    dbg_earjack("@@@@TEST_HANDSET_START!!!\n");
    
    INIT_DELAYED_WORK(&earjack_work,earjack_det_func);          // INIT WORK
    INIT_DELAYED_WORK(&remotekey_work,remotekey_det_func);    
    hs_jack_l8 = regulator_get(NULL, "8058_l8");
    regulator_set_voltage(hs_jack_l8,2400000,2400000);					// P13106 CS_REQUIRED changed 2.4v from 2.7v
	err=regulator_is_enabled(hs_jack_l8);
    dbg_earjack("regulator_is_enabled(hs_jack_l8) value => %d\n",err);
    if(err>0) regulator_disable(hs_jack_l8);

	wake_lock_init(&earjack_wake_lock, WAKE_LOCK_SUSPEND, "earjack_wake_lock_init");
    wake_lock_init(&remotekey_wake_lock, WAKE_LOCK_SUSPEND, "remotekey_wake_lock_init");
    gpio_request(EARJACK_DET, "earjack_det");
    gpio_request(REMOTEKEY_DET, "remotekey_det");
    gpio_tlmm_config(GPIO_CFG(EARJACK_DET, 0, GPIO_CFG_INPUT,GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    ret = request_irq(gpio_to_irq(EARJACK_DET), Earjack_Det_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "earjack_det-irq", hs);
 
    if (ret) {
	pr_err("%s: request irq, ret=%d\n",__func__, ret);       
    }

    mutex_init(&hs_mutex);                      // Init Mutex
    mutex_init(&hs_remotekey_mutex);
    
    mutex_lock(&hs_mutex);
    irq_set_irq_wake(gpio_to_irq(EARJACK_DET), 1);
    irq_set_irq_wake(gpio_to_irq(REMOTEKEY_DET), 1);
    mutex_unlock(&hs_mutex);
    isInserted();                                               // checking is 3.5pi inserted in boot


    }
#endif // N1037 20120216 porting (-)

	ipdev->id.vendor	= 0x0001;
	ipdev->id.product	= 1;
	ipdev->id.version	= 1;

	input_set_capability(ipdev, EV_KEY, KEY_MEDIA);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEUP);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEDOWN);
    input_set_capability(ipdev, EV_KEY, KEY_END); // N1037 20120216 porting
	input_set_capability(ipdev, EV_SW, SW_HEADPHONE_INSERT);
	input_set_capability(ipdev, EV_SW, SW_MICROPHONE_INSERT);
	input_set_capability(ipdev, EV_KEY, KEY_POWER);
	input_set_capability(ipdev, EV_KEY, KEY_END);

#ifdef FEATURE_SKY_CHG_LOGO
    input_set_capability(ipdev, EV_KEY, KEY_BATTERY);
#endif

	rc = input_register_device(ipdev);
	if (rc) {
		dev_err(&ipdev->dev,
				"hs_probe: input_register_device rc=%d\n", rc);
		goto err_reg_input_dev;
	}

	platform_set_drvdata(pdev, hs);

	rc = hs_rpc_init();
	if (rc) {
		dev_err(&ipdev->dev, "rpc init failure\n");
		goto err_hs_rpc_init;
	}

#ifdef FEATURE_SKY_3_5PHIEARJACK
    rc = sysfs_create_group(&pdev->dev.kobj, &dev_attr_grp);
    if (rc) {
        dev_err(&ipdev->dev,
                "hs_probe: sysfs_create_group rc=%d\n", rc);
        goto err_hs_rpc_init;
    }
#endif

    dbg_earjack("@@@@hs_probe success!!!\n");
	return 0;

err_hs_rpc_init:
	input_unregister_device(ipdev);
	ipdev = NULL;
err_reg_input_dev:
	input_free_device(ipdev);
err_alloc_input_dev:
	switch_dev_unregister(&hs->sdev);
err_switch_dev_register:
	kfree(hs);
	return rc;
}

static int __devexit hs_remove(struct platform_device *pdev)
{
	struct msm_handset *hs = platform_get_drvdata(pdev);

	input_unregister_device(hs->ipdev);
	switch_dev_unregister(&hs->sdev);
	kfree(hs);
	hs_rpc_deinit();
#ifdef FEATURE_SKY_3_5PHIEARJACK
	wake_lock_destroy(&earjack_wake_lock);
    wake_lock_destroy(&remotekey_wake_lock);
#endif
	return 0;
}

static struct platform_driver hs_driver = {
	.probe		= hs_probe,
	.remove		= __devexit_p(hs_remove),
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
#ifdef FEATURE_SKY_3_5PHIEARJACK
	.resume = hs_earjack_resume,
	.suspend = hs_earjack_suspend,
#endif	
};

static int __init hs_init(void)
{
	return platform_driver_register(&hs_driver);
}
late_initcall(hs_init);

static void __exit hs_exit(void)
{
	platform_driver_unregister(&hs_driver);
}
module_exit(hs_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:msm-handset");
