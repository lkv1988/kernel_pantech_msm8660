#ifndef __CUST_PANTECH_GOTA_H
#define __CUST_PANTECH_GOTA_H
/* ========================================================================
FILE: cust_pantech_gota.h

Copyright (c) 2010 by PANTECH Incorporated.  All Rights Reserved.

=========================================================================== */

/*************************************************************************/
/*                           FEATURE DESCRIPTION                         */
/*************************************************************************/
/* ========================================================================
FEATURE_GOTA_UPGRADE
	GOTA main FEATURE
FEATURE_GOTA_1BLK_PASS_PATCH
	desc
		SKY Station 1 block pass writing patch
	source
		mtdutils/mtdutils.c
		updater/install.c
FEATURE_GOTA_MEMORY_LEAK_PATCH
	desc
		Memory Leak patch during applypatch in recovery mode.
	source
		applypatch/applypatch.c
FEATURE_GOTA_CONSOLE_LOG
	desc
		Print log to seial in recovery mode
	source
		recovery_log.c
		recovery_log.h
	usage
		#include "recovery_log.h"
		Console_log_init();
		CONSOLE_LOG(const char *fmt, ...);
		Console_log_close();
FEATURE_GOTA_SBL_POWER_LOSS
	desc
		Load each image from FOTA partition if the image is damaged due to power loss when the SBL or LK updating in the recovery mode.
	source
		modem_proc/core/.../sky_rawdata.h
		pantech/frameworks/.../sky_rawdata.h
		boot_images/core/boot/secboot3/common/boot_loader.h
		boot_images/core/boot/secboot3/common/boot_loader.c
		boot_images/core/boot/secboot3/common/boot_gpt_partition_id.c
		boot_images/core/boot/secboot3/common/boot_config.c
		applypatch/applypatch.c
FEATURE_GOTA_PANTECH_DISPLAY
	desc
		Show string and percent when updating in the recovery mode.
	source
		recovery_ui.h
		ui.c
		recovery.c
FEATURE_GOTA_UPDATE_INFO
	desc
		Store update info for AT&T
	source
		modem_proc/core/.../sky_rawdata.h
		pantech/frameworks/.../sky_rawdata.h
		bootable/bootloader/lk/app/pdl/pdl.h
		bootable/bootloader/lk/app/pdl/dloadstatus.c
		pantech/frameworks/pam_proc/
		gota_jni/GotaJni.java
		gota_jni/gota_jni.cpp
		fota_rawdata/fota_rawdata.c
		gota_status.h
		recovery.c
FEATURE_GOTA_PERMANENT_MEMORY
	desc
		Functions to deal with permanent memory.
		It is used only by the SKT models
	source
		pantech/frameworks/pam_proc/pam_proc.c
		pantech/frameworks/pam_proc/pam_proc.h

=========================================================================== */


/*************************************************************************/
/*                           PANTECH GOTA                                */
/*************************************************************************/


#define FEATURE_GOTA_UPGRADE
#define FEATURE_GOTA_1BLK_PASS_PATCH
#define FEATURE_GOTA_MEMORY_LEAK_PATCH
#define FEATURE_GOTA_CONSOLE_LOG
#define FEATURE_GOTA_SDCARD_UPDATE
#define FEATURE_GOTA_SBL_POWER_LOSS
#define FEATURE_GOTA_PANTECH_DISPLAY
#define FEATURE_GOTA_PERMANENT_MEMORY
// #define FEATURE_GOTA_UPDATE_INFO


#endif /* __CUST_PANTECH_GOTA_H */
