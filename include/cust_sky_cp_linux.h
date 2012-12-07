#ifndef __CUST_SKY_CP_H__
#define __CUST_SKY_CP_H__


/*****************************************************
    SKY Android 모델 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_COMMON
******************************************************/
#ifdef T_SKY_MODEL_TARGET_COMMON


/*
    DDMS Log에 radio log 를 볼수있도록 함. 
    Test 편의를 위해 사용하는 것이기 때문에
    envsetup.mk 의 TARGET_BUILD_VARIANT 가 user ( user mode compile )
    일때 만 FEATURE_BUILD_USER 가 define됨.
     (PS1팀 작업 필수: cust_sky.h 에 사용여부 확인.)
    android/system/core/liblog/logd_write.c
*/
#ifndef FEATURE_BUILD_USER
#undef FEATURE_SKY_CP_RADIO_LOG_DDMS

/* 
   qcril_qmi 로그가 RILC tag로 DDMS상에서 보이도록 함.
   RILC 로그가 많이 생성되기 때문에 필요할때만 define하도록 합시다.
*/
#undef FEATURE_SKY_CP_RADIO_RILC_LOG_DDMS
#endif/* FEATURE_BUILD_USER */

/* 
  QCRIL RPC 초기화 추가하함. 
  System VOB에서의 지원을 위해서 target feature를 qcril_qmi.mk 에도 선언해둠.
*/
#ifndef FEATURE_SKY_CP_RPC_FUSION_SUPPORT
#define FEATURE_SKY_CP_RPC_FUSION_SUPPORT
#endif/* FEATURE_SKY_CP_RPC_FUSION_SUPPORT */

/*
     OEM RAPI 에서 CM RPC를 등록하여 사용함.
     CM PH Event 와 CM Call Event 를 oem_rapi 를 통해서 linux로 전달받기 위함.
*/
#define FEATURE_SKY_CP_OEM_RAPI_CM_EVENT_SUPPORT

/*
    OEM RAPI 에서 mmgsdi RPC 등록.
    linux에서 mmgsdi API 호출 위해서 필요함.
*/
#define FEATURE_SKY_CP_OEM_RAPI_MMGSDI_RPC_SUPPORT

/*
    OEM RAPI 에서 gsdi_exp RPC 등록.
    linux에서 gsdi API 호출 위해서 필요함.
*/
#define FEATURE_SKY_CP_OEM_RAPI_GSDI_EXP_RPC_SUPPORT

/*
   Qualcomm OEM Hook 기능을 이용한 확장.  ( NV access, SIM access etc..)
*/
#define FEATURE_SKY_CP_QCRILHOOK_INTERFACE

#ifdef FEATURE_SKY_CP_QCRILHOOK_INTERFACE
/* oemhook및 qcril에서 error exception처리를 위한 선언. */
#define FEATURE_SKY_CP_EXCEPTION_DEFINITION

#define FEATURE_SKY_QCRILHOOK_EVENT_PROCESSED_BY_SUB_THREAD

/* OEM Hook 기반으로 SIM access를 GSDI를 이용할때 sync방식으로 access하기 위한 I/F */
#define FEATURE_SKY_CP_GSDI_SYNC_ACCESS_BY_REMOTE_PROCESSOR

#ifdef FEATURE_SKY_CP_GSDI_SYNC_ACCESS_BY_REMOTE_PROCESSOR
#ifdef FEATURE_SKY_CP_OEM_RAPI_GSDI_EXP_RPC_SUPPORT
#define FEATURE_SKY_CP_USING_GSDI_RPC_CLIENT
#endif/* FEATURE_SKY_CP_OEM_RAPI_GSDI_EXP_RPC_SUPPORT */
#endif/* FEATURE_SKY_CP_GSDI_SYNC_ACCESS_BY_REMOTE_PROCESSOR */

#endif/* FEATURE_SKY_CP_QCRILHOOK_INTERFACE */

/* 
   SkyTelephonyInterfaceManager.java, Service_Manager.c, ISkyTelephony.aidl 
*/
#define FEATURE_SKY_CP_TELEPHONYMANAGER
#ifdef FEATURE_SKY_CP_TELEPHONYMANAGER
/* Java에서 Modem API 호출을 위한 Daemon ( NV access, 자동응답, VE 기능, etc.. ) */
#define FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE
#ifdef FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE
/* Voice Call connection sound event */

#define FEATURE_SKY_CP_CS_CALL_CONNECTION_SND_START

/* SKY 자동응답기능 Interface */
#define FEATURE_SKY_CP_AUTOANSWER_INTERFACE

#endif/* FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE */
#endif/* FEATURE_SKY_CP_TELEPHONYMANAGER */

/*
   Qualcomm Feature [FEATURE_QCRIL_UIM_QMI] 사용
*/
#define FEATURE_SKY_CP_SUPPORT_FEATURE_QCRIL_UIM_QMI
#ifdef FEATURE_SKY_CP_SUPPORT_FEATURE_QCRIL_UIM_QMI
/* qcril 에서 mmgsdi 및 gsdi function 사용시 RPC callback 이 필요함. */
#define FEATURE_SKY_CP_GSDI_RPC_CALLBACK_SUPPORT
#endif/* FEATURE_SKY_CP_SUPPORT_FEATURE_QCRIL_UIM_QMI */

/* QMI 를 통해서 CM system selection preference 호출 */
#define FEATURE_SKY_CP_CM_SYS_SELECTION_PREF

/* UMTS SMS MO PS/CS Domain setting menu support */
#define FEATURE_SKY_CP_SMS_CFG_SET_GW_DOMAIN

/* Phone Operation Mode setting (lpm, offline, online .. ) */
#define FEATURE_SKY_CP_PHONE_OPERATION_MODE

/* RIL state 에 상관없이 항상 ril socket open상태에서는 RIL_REQUEST_XXX 가 동작하도록 함. */
#define FEATURE_SKY_CP_FORCE_ALLOW_RIL_REQUEST

/* RPC를 통해서 공정커맨드 처리 ( 프로토콜 담당 커맨드 처리 ) */
#define FEATURE_SKY_CP_PROCESS_CMD_WITH_RPC

/* Airplane mode 적용/해제를 위한 공정 커맨드 처리 */
#define FEATURE_SKY_CP_FACTORY_AIRPLANE_MODE_I

/* QMI 를 통해서 CM PH event info. 처리 */
#define FEATURE_SKY_CP_OEM_PH_EVENT_WITH_QMI

/*
    QMI를 이용하여 Modem에 접근 하기 위한 코드로 
    기존 Sar Test를 위해서 구현된 코드에 OEM 코드를 추가함.
*/
#define FEATURE_SKY_CP_OEM_QMI_ACCESS

/* 
   1. qcril_data_init() 과정에서 5초 delay되는 코드 삭제.
       5초 delay발생으로 부팅 초기에 PIN lock 화면이 늦게 발생하고 
       노서비스 상태가 오래 지속됨.
   2. 1번에 대한 side effect발생으로 부팅 초기에 RIL_REQUEST_RADIO_POWER 가 
       Generic failure가 발생하여 정상동작을 하지 못하는 case 보완.
*/
#define FEATURE_SKY_CP_QCRIL_DATA_INIT_DELAY_REMOVE

#endif/* T_SKY_MODEL_TARGET_COMMON */


/* ######################################################################### */



/*****************************************************
    SKT/KT 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_WCDMA
******************************************************/
#ifdef T_SKY_MODEL_TARGET_WCDMA


/* OEM RAPI 를 이용하기 위한 Protocol Feature */
#define FEATURE_SKY_RPC_OEM_RAPI_NEW_INTERFACE

#define FEATURE_SKY_CP_ADDITIONAL_NV_ITEMS
#define FEATURE_SKY_CP_RPC_XDR_NV_ACCESS

#define FEATURE_SKY_CP_LOCAL_DB
#ifdef FEATURE_SKY_CP_LOCAL_DB
#define FEATURE_SKY_CP_LOCAL_DB_LTE

/* local_db get/put 동작에 대해서 QMI를 이용하도록 수정함. */
#define FEATURE_SKY_CP_LOCAL_DB_WITH_QMI
#endif/* FEATURE_SKY_CP_LOCAL_DB */

/* QMI 을 이용하여 NV access */
#define FEATURE_SKY_CP_NV_ACCESS_WITH_QMI

#define FEATURE_SKT_CP_SUBSCRIPTION_MODE

#define FEATURE_SKY_CP_OTA_KT_SUBSCRIPTION

#define FEATURE_SKY_USIM_CARD_TYPE

#define FEATURE_SKY_USIM_GET_CARD_MODE_AS_APP_TYPE

#define FEATURE_SKY_CP_ACQ_DB_DELETE

/* #define FEATURE_SKY_CP_END_DUN_INTERFACE */

#ifdef FEATURE_SKY_CP_QCRILHOOK_INTERFACE
/* OEM Hook 기반으로 NV access */
#define FEATURE_SKY_CP_NV_ACCESS_OEMHOOK

#ifdef FEATURE_SKY_CP_LOCAL_DB
/* OEM Hook 기반으로 Local DB access */
#define FEATURE_SKY_CP_LOCAL_DB_ACCESS_OEMHOOK
#endif/* FEATURE_SKY_CP_LOCAL_DB */
#endif/* FEATURE_SKY_CP_QCRILHOOK_INTERFACE */

#ifdef FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE
/* cpmgrif.c daemon을 통한 NV Access */
#define FEATURE_SKY_CP_NV_ACCESS_CPMGRIF

#ifdef FEATURE_SKY_CP_LOCAL_DB
/* cpmgrif.c daemon을 통한 Local DB Access */
#define FEATURE_SKY_CP_LOCAL_DB_ACCESS_CPMGRIF
#endif/* FEATURE_SKY_CP_LOCAL_DB */
#endif/* FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE */

/*
    SKT/KT 공통 Me Personalization ( With QMI interface )
*/
#define FEATURE_SKY_USIM_ME_PERSONALIZATION

/* 
   1. qcril 에서 default voice tech.를 3GPP2가 이닌 3GPP로 설정.
     --> 부팅 초기에 3GPP2로 radio technology가 천이되었다가 3GPP로 바뀌어서 3GPP2인 CDMA SIM 에서 SIM ready가 
            발생하여 PUK및 PIN 설정이 제대로 안되거나 SIM Record함수들이 CDMA용으로 동작하는 경우가 발생함.
   2. java class에서 Phone type이 GSMPhone으로 create 되도록 설정함.
*/
#define FEATURE_SKY_CP_DEFAULT_NETWORK_UMTS_IN_RIL


/*
    -. WMS Link control을 사용하며 MMS app.로 부터 발신 총 개수를 넘겨받아 WMS에서 처리하도록 RIL I/F 수정
*/
#define FEATURE_SKY_CP_MULTI_SMS_REQUIREMENT

/* CNAP 지원을 위한 RIL 및 framework 수정. */
#define FEATURE_SKY_CP_CNAP_SUPPORT

/* 국내 사업자 (SKT, KT) 망에서 KSC5601 format의 USSD msg.와 CNAP 지원을 위한 수정. */
#define FEATURE_SKY_CP_USSD_CNAP_8BIT_CHAR_SUPPORT

/* WCDMA, GSM 의 RSSI level 기준 변경,  H/W 요청에 의한 변경 */
#define FEATURE_SKY_CP_GW_RSSI_LEVEL

/* 
    RSSI report 가 편차없이 이루어 질 수 있도록 delta threadhold 값을 수정
    modem코드에서 cmph.h 와 동일 적용.
 */
#define FEATURE_SKY_CP_RSSI_REPORT_VARIATION_FIX

/*
    음성호 발신을 위해서 CSFB동작 시 또는 음성호 종료 후 LTE로 이동할때 
    급격한 Ant. bar의 변화가 없게 하기 위한 처리.
    추후 보강
*/
/* #define FEATURE_SKY_CP_RSSI_REPORT_TIMING_GAP_FIX */

/*
- Limited Service상태에서는 RSSI Bar의 개수를 0으로 표시하도록 수정.
- SIM없이 부팅되거나 부팅초기 reject없이 limited service가 유지되면 no service로 인식되기 때문에 
   qcril_cm.c 에서 qcril_cm_util_srv_sys_info_to_reg_state() 함수내 limited service 를 전달해주는 조건 수정이 필요함.
*/
#define FEATURE_SKY_CP_RSSI_BAR_ZERO_DISPLAY

/*
   서비스 상태 에서만 MCC/MNC 가 update 되던 부분을 Limited service상태에서도 update가 가능하도록 수정함.
*/
#define FEATURE_SKY_CP_GET_MCCMNC_UPDATE_IN_LIMITED_SRV

/* 
  단말 PS ONLY 모드로 설정시 android단에서 Reg state를 No Service로 처리하는 오류를 수정 
  CS reject, PS accept 되는 reject cause시험에서도 동일 증상 발생.
*/
#define FEATURE_SKY_CP_SUPPORT_PS_ONLY_MODE

/* qcril 에서 ui 로 올라가는 reg_status에 오류가 있어 이를 수정함 */
#define FEATURE_SKY_CP_REG_STATUS_UPDATE_CORRECTION

/*
   -. 단말의 service status가 SYS_SRV_STATUS_PWR_SAVE 일때 QCRIL_CM_SCREEN_STATE_ON 상태가 될때 즉
      Key interrup 발생으로 LCD가 on될때 단말이 power save상태라면 modem을 wakeup시키도록 되나, 
      qualcomm bug로 동작 안되는 현상 수정 
*/
#define FEATURE_SKY_CP_MODEM_PWR_SAVE_WAKEUP_FIX

/*
   -. card power down 커맨드 처리하지 않도록 함.
      < 미 수정시 발생하는 주요 문제점 >
      1. 비행중 모드 진입시 USIM card access되지 않는 문제
      2. 비행중 모드 진입시 SIM card가 인식되지 않는다는 문구 표시됨.
      3. on chip sim mode로 동작중 비행중 모드 들어가면 card가 없는 상태에서 UIM으로 card power down커맨드가 전달되어
*/
#define FEATURE_SKY_CP_CARD_POWER_DOWN_NOT_SUPPORT

/*
   통화중 대기음 설정을 위해 Setup ind. 에서 signal value를 설정함.
   AMSS에서 FEATURE_SKY_CP_SETUP_IND_SIGNAL_VALUE 와 반드시 연동되어야 함.
*/   
#define FEATURE_SKY_CP_SETUP_IND_SIGNAL_VALUE

/*
   ETC(상대방연결) 서비스 수행시 call end에 의해서 CM_EVENT_CALL_MNG_CALLS_CONF 가 처리되지 못하고 
   pending되어 있는경우 call end화면 처리를 하지 못하고 연결 실패 팝업이 발생하는 문제 수정.
*/
#define FEATURE_SKY_CP_ETC_BUG_FIX

/*
   음성호 종료 후 Disconnecting 에서 Disconnected 로 상태 천이 가 되지 않는 경우
   RIL 을 통해서 call end가 정상적으로 호출 되었는지 보기 위해 property를 설정해 놓음.
*/
#define FEATURE_SKY_CP_CALL_END_REQ_DEBUGGING

/* 
  Network preferred mode 설정 Menu
  Automatic, WCDMA only, GSM only
  국내에서는 메뉴 진입 못하도록 설정.
*/
#define FEATURE_SKY_CP_PREFERRED_NETWORK_MODE_SELECTION

/** GPS Feature **/
#define FEATURE_SKY_CP_GPS_TEST_SUPPORT

/* SIM 에 대한 인식 여부와 상태를 알아보기위한 테스트 코드 */
#define FEATURE_SKY_USIM_CARD_STATUS_PROPERTY

/*
   RIL_REQUEST_GET_NEIGHBORING_CELL_IDS 커맨드가 호출 되었을때 
   Neighbor Cell정보가 실제로 있는 경우 system halt를 유발함.
   불필요한 커맨드로 처리하지 않도록 수정.
*/
#define FEATURE_SKY_CP_GET_NEIGHBORING_CELL_IDS_NOT_SUPPORT

/* Limited service상태를 구분하기 위해서 ServiceState 설정. */
#define FEATURE_SKY_CP_SERVICE_STATUS_DETAIL_SUBSTATE


/* 
    Network Name을 수신했음에도 불구하고, "nas_cached_info.current_plmn" 값이 Invalid로 처리되어 
   해당 Network Name이 아닌 Table의 name을 표시하는 문제를 위해 Qcril에서 관리하는 system info를 
   state 변화시마다 update하도록 함.
   QCT는 이 system info를 LCD enable시에만 update 하도록 하고 있음
*/
#define FEATURE_SKY_CP_QMI_SYS_INFO_ALWAYS_UPDATE

/* OEM QMI commands set */
#define FEATURE_SKY_CP_OEM_COMMANDS_WITH_QMI

/*
   해외 로밍시 (52501: Sing Tel) NITZ 정보가 내려오는 망에서 OP NAME 정보가 NULL은 아니지만 length가 (0)인 경우에 해당 됨.
   MCCMNC Table에서 해당 OP NAME을 뿌리다가 MM INFO 수신 이후 NAME이 안보이는 경우 수정함.
   MM INFO로 Long Name은 NULL이고, Short Name만 내려오는 경우 위의 qcril_qmi_nas_persist_entry_update 함수의
   리턴 값으로 E_FAILURE으로 전달되더라도 Short Name에 대해 처리하도록 함.   
*/
#define FEATURE_SKY_CP_FIX_OPERATOR_NAME_DISPLAY

/*
   비행중 모드 진입 후 언어 변경 시 Lock Screen, status bar의 spn이 변경되지 않는 현상 수정함.
*/
#define FEATURE_SKY_CP_SPN_UPDATE_IN_AIRPLANE_MODE

/*
    NFC 확장 API 관련하여 Get ATR 함수를 QMI 연결
*/
#define FEATURE_SKY_USIM_GET_ATR_QMI

/*
  Data 서비스시에 RSSI BAR가 0으로 되는 현상 발생 Background로 TAU올라 가면서 이에 대한 
  Srv State change 가 변경 되지 않느 Linux단은 Limited로 인식한 상태 지속 ..Data 서비스 상태에서는
  Sig strength 만 올라오는 상황임..이때 Sig info 와 더불어 Srv State change 도 같이 올라 오도록 함
*/
#define FEATURE_SKY_CP_REPORT_STATE_CHANGE_WITH_SIG_INFO_DURING_DATA_SVC

/*
     Opening Day implementation
*/    
#define FEATURE_SKY_CP_NEW_OPENING_DAY

/*
  Data 서비스시에 RSSI BAR가 0으로 되는 현상 발생 Background로 TAU올라 가면서 이에 대한 
  Srv State change 가 변경 되지 않느 Linux단은 Limited로 인식한 상태 지속 ..Data 서비스 상태에서는
  Sig strength 만 올라오는 상황임..이때 Sig info 와 더불어 Srv State change 도 같이 올라 오도록 함
*/
#define FEATURE_SKY_CP_REPORT_STATE_CHANGE_WITH_SIG_INFO_DURING_DATA_SVC

/*
  illegal SIM 상태가 되면 SIM state가 not ready 상태가 되어 SIM menu 진입이 되지 않는등의 
  사소한 문제가 발생함. ( 모뎀쪽이  illegal sim 상태가 되더라도 ME perso. 설정 변경이 가능해야 함. )
*/
#define FEATURE_SKY_CP_MAINTAIN_SIM_READY_ILLEGAL_SIM

/* 20120228 ithong.
   sim inserted 에 의한 sim status indication 정보내에 실제 할당된 slot 이 없음에도. gw provision index가 설정되어 linux 에 수신됨
   해당 정보가 그대로 안드로이드 단으로 전달되어 초기 card 객체와 app 객체 생성에 영향을 미칠 우려가 되어 방지 코드 적용함.
   즉 할당된 slot이 없는 상태에서의 sim status indication은 안드로이드 단으로 전달하지 않도록 함.
*/
#define FEATURE_SKY_CP_SKIP_SIM_STAT_INT_FOR_NON_SLOT_STATE

/* 20120314 ithong
   isim application 지원을 위해 feature.
   모뎀단에서는 isim app init 을 진행하지 않기에 isim app detected 이후 usim ready되면,
   isim app 역시 ready 로 처리하여 안드로이드단으로 전달하도록 함.
*/
#define FEATURE_SKY_CP_OEM_SUPPORT_FOR_ISIM_APP_IN_QCRIL

/* 	
	Qualcomm Test SBA
	단말 최초 부팅 후 MT call 수신 상태에서 통화추가 시 망에서 Hold reject 이 내려올 경우 
	이전 call 화면으로 전환되지 않고 Managed call로 변경되는 현상.
	qualcomm SR 진행하여 Test SBA를 적용하여 fail_cause 의 valid값을 초기화 시켜주도록 수정함. 
*/
#define FEATURE_SKY_CP_LAST_CALL_FAILURE_FORCED_INIT

/* 	
	USSD Send 후 망에서 응답이 없을 경우 처리
*/
#define FEATURE_SKY_CP_USSD_NEWORK_NO_RESPONSE

/*
   20120423 Lee Jonghwan
   30145 patch후 qcril.c의 android_request_id가 QMI_RIL_FW_ANDROID_REQUEST_HNDL_MAX_EVT_ID (127)로 제한됨.
   Ril.h에 추가된 event id값을 추가하여 132 값보다 큰 값으로 설정함.
*/
#define FEATURE_SKY_CP_RIL_FW_ANDROID_REQUEST_HNDL_MAX_EVT_ID_FIX

/*
   20120424 Lee Jonghwan
   30145 patch후 qcril.c 추가된 RIL관련 ITEM 처리 구조의 변화로 간헐적으로 이밴트 처리에 문제가 됨.
   Voice & Data REG STATE, Auto & Malual Network Selection 관련 RIL event는 30145 patch 이전과 같이 처리되도록 
   qmi_ril_fw_dedicated_thrd_exec_android_requests_set 테이블에서 제외함.
*/
#define FEATURE_SKY_CP_FW_DEDICATED_ANDROID_REQ_BUG_FIX

#define FEATURE_SKY_CP_SUPPORT_RSSI_IN_SERVICE_PS_ONLY_MODE

#endif/* T_SKY_MODEL_TARGET_WCDMA */


/* ######################################################################### */


/*****************************************************
    SKT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_SKT
******************************************************/
#ifdef T_SKY_MODEL_TARGET_SKT

#ifdef T_SKY_MODEL_TARGET_KT
#error Occured !!  This section is SKT only !!
#endif

#ifdef FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE

/* USIM card removed event */
#define FEATURE_SKY_CP_USIM_CARD_REMOVED_EVENT

#endif/* FEATURE_SKY_CP_MANAGER_DAEMON_INTERFACE */

/*
   GPS requirement ( hidden Menu, SUPL, android Provider, Etc... )
*/
#define FEATURE_SKT_CP_GPS_REQUIREMENT_ANDROID
#ifdef FEATURE_SKT_CP_GPS_REQUIREMENT_ANDROID
/* gps.conf 파일에 SUPL. host설정을 google로 변경. */
#define FEATURE_SKT_CP_GOOGLE_SUPL_SUPPORT
#endif/* FEATURE_SKT_CP_GPS_REQUIREMENT_ANDROID */

/* Network 자동선택,수동선택 항목 아래에 현재 망정보 ( RAT, PLMN, name ) 을 표시함 */
#define FEATURE_SKT_CP_NETWOTK_INFO_DISPLAY

/* SKT 사업자용 PLMN Table */
#define FEATURE_SKT_CP_PLMN_TABLE_LIST_SEARCH

/* SKT IOT Problem fix.
    PLMN과 network name 이 property에 저장되어 RTS 시험중 
    MM info.에 network name 포함 / 미포함 시험에서 fail되는 현상.
*/
#define FEATURE_SKT_CP_INIT_NITZ_INFO_PROPERTY

/* SKT 넘버플러스 서비스 지원 */
#define FEATURE_SKT_CP_NUMBER_PLUS_SUPPORT
#ifdef FEATURE_SKT_CP_NUMBER_PLUS_SUPPORT
#define FEATURE_SKT_CP_NUM_PLUS_WITH_QMI
#endif/* FEATURE_SKT_CP_NUMBER_PLUS_SUPPORT */

/*
  reg reject 내용 표시 가능하도록 system property 값 적용함.
  reg reject 내용이 keyguard 화면에 표시되도록 함.
  AMSS관련 feature : FEATURE_SKY_CP_REJECT_CAUSE_DISPLAY
  reject cause관리를 ServiceState 에서 하도록 수정.
*/
#define FEATURE_SKT_CP_REJECT_CAUSE_DISPLAY

/* SKT 영상 컬러링 지원 Interface */
#define FEATURE_SKT_CP_VE_SUPPORT

/*
   SKT향 PLMN Selection(자동/수동) 관련 구현사항.
*/
#define FEATURE_SKT_CP_NETWORK_PLMN_MANUAL_SELECTION

/* 
   IMEI mismatch Lock applet 만으로는 폰의 Lock 이 걸리지 않기 때문에
   USIM Lock을 위해서 추가 
 */
/* #define FEATURE_SKT_USIM_IMEI_LOCK */

/*
    SKT 요구사항.   
    LTE reject cause #10, #40 에 대해서 no service로 표시하지 않도록 함.
*/
#define FEATURE_SKT_CP_LTE_RSSI_DISPLAY_REQUIREMENT

/*
    SKT USIM 요구사항. (WS-USIM-1.75)   
    ME should not cache EF CFIS/ EF MBI/ EF MWIS/ EF MBDN in case of SKT USIM inserted.
    Except for SKT USIM, handset shall work with CPHS or 3GPP for interoperability issue.
*/
#define FEATURE_SKT_USIM_NOT_CACHE_EF_FIELD

/*
  GUAM에서의 시간정보 표시 오류 관련 수정 사항..
*/
#define FEATURE_SKY_CP_TIMEZONE_FIX

#endif/* T_SKY_MODEL_TARGET_SKT */


/* ######################################################################### */


/*****************************************************
    KT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_KT
******************************************************/
#ifdef T_SKY_MODEL_TARGET_KT

#ifdef T_SKY_MODEL_TARGET_SKT
#error Occured !!  This section is KT only !!
#endif


#endif/* T_SKY_MODEL_TARGET_KT */





/***********************************************************
   JAVA code에만 적용된 Feature 및 code 확인용...
   실제로 C/C++ 코드에는 적용되지 않는 사항으로 
   코드 검색및 확인용도로 사용하기 위함.
************************************************************/

/* import 문이나 여러 기능에 공통으로 들어가는 코드 표시 */
#define FEATURE_SKY_CP_COMMON_JAVA_DECLARATION

/* EF File 접근을 Path 를 이용하기 위해서 사용된 코드 */
#define FEATURE_SKY_USIM_EF_FILE_ACCESS_BY_PATH

/* USIM Card 제거시 재부팅 noti. */
#define FEATURE_SKY_CP_CARD_REMOVED

/* 로밍상태에서 UI동작 점검을 위한 fake roaming setting */
#define FEATURE_SKY_CP_ANDROID_FAKE_ROAMING_SETTING

/* Hidden Code 설정. 및 불필요한 code접근 막음. */
#define FEATURE_SKY_CP_HIDDEN_CODE_CONTROL

/* SKT USIM에서 UA Field생성에 필요한 MIN/IRM 추출. */
#define FEATURE_SKT_CP_CDMA_MIN_EF_ACCESS

/* GCF mode 설정 여부 */
#define FEATURE_SKT_CP_CHECK_GCF_MODE

/* 
   android Settings.DEVICE_PROVISIONED 가 항상 1로 설정되어있도록 함. 
   개발 초기에 GSM 음성 착신시 착신 거부되어 테스트 진행에 문제를 야기 시킴..
*/
#define FEATURE_SKY_CP_DISABLE_CHECK_DEVICE_PROVISIONED

/* Qualcomm Auto Answer 설정 오류 수정. ( SKY 자동응답기능 아님 !!) */
#define FEATURE_SKY_CP_AUTO_ANSWER_BUG_FIX

/* SKT Emergency Call requirement. */
#define FEATURE_SKT_CP_CHECK_EMERGENCY_CALL

/* 네트웍네임 표시 방법 - Short Name 표시 우선. */
#define FEATURE_SKT_CP_DISPLAY_NETWORK_NAME

/* Roming indicator표시 오류 보완. */
#define FEATURE_SKY_CP_UPDATING_ROAMING_INDICATOR


/* Lock Screen에 표시되는 network name on-off 기능  */
//#define FEATURE_SKT_CP_NETWORK_NAME_DISPLAY_ONOFF

/* SIM state check 오류 수정. patch시 해당 코드 확인요망. */
#define FEATURE_SKY_USIM_GET_SIM_STATE

/* 
   Power off 수행중 PS가 release되기 전에 android power off sequence가 완료되어 
   망으로 Detach를 올리지 못하는 경우가 있어 동작을 1초 정도 delay시킴.
*/
#define FEATURE_SKY_CP_DELAYED_POWER_OFF

/* KT SIM MSISDN표시 및 SKT SIM MSISDN 표시형식 수정. */
#define FEATURE_SKY_CP_DISPLAY_PHONE_NUMBER_WITH_TOA

/* Android Setting 메뉴에서 통화설정 메뉴 삭제.  */
#define FEATURE_SKY_CP_REMOVE_ANDROID_CALL_FEATURE_SETTING_MENU

/*
    framework/base/core/res/res/config.xml
    network location provider 설정변경 
    초기값이 null 로 되어있어 wifi, cell 기반 위치측위가 안됨.
*/
#define FEATURE_SKY_CP_NETWORK_LOCATION_PROVIDER_SETTING

/*
   SKT requirement. ( 국내 발신용. )
   *23# 과 같은 feature code를 사용한 호발신이 sups. service로 인식되지 않도록 함.
*/
#define FEATURE_SKT_CP_SUPS_SERVICE_REQUEST

/* Roaming Dual Clock 과 RAD 설정을 위한 이벤트 발생. */
#define FEATURE_SKT_CP_EVENT_TIME_AND_RAD

/* lockscreen에서 긴급호 버튼, 문구 등의 표시및 위치 조정을 위한 구성. */
#define FEATURE_SKT_CP_EMERGENCY_BUTTON_LOCKSCREEN

/* 
   블루투스 인증관련 수정사항.
   A와 B가 통화중 C에서 전화가 왔을때 A와 B의 통화를 중단하면 C가 자동으로 연결되어 
   문제가 됨.  Alert상태를 유지하도록 수정.
*/
#define FEATURE_SKY_CP_BLUEBOOTH_CERTIFICATION

/* USIM PUK permanent block 처리를 위한 추가사항  */
#define FEATURE_SKY_CP_ADD_PERM_BLOCKED_CARD_STATUS

/*
    Absend, Ready, Error 등의 card state를 return받기 위한 interface
    망으로부터 ilegal MS: Reject (3)을 받는 USIM인 경우 원본소스는 SIM_STATE UNKNOWN이지만 STATE를 READY로 수정.
*/
#define FEATURE_SKY_CP_GET_ICC_CARD_STATE

/* 위치및 보안 메뉴 설정에서 AGPS( GPS 도우미 사용 ) 설정시 경고팝업 */
#define FEATURE_SKT_CP_ALERT_FOR_USING_AGPS

/*  
   Airplane mode 진입시 network mode menu 진입이 순간적으로 진입이 가능해지는 문제 발생  
   이전 EF30S의 이전 버전의 경우 service state를 보고서 airplane mode check box를 
   disable 시키도록 하는 부분이   존재..하지만 현재 버전에는 해당 부분 없음...
   EF30S 소스기반으로 해당 함수 수정 이후 6140도 해당 부분이  존재 하지 않음..
*/
#define FEATURE_SKY_CP_AIRPLANE_MODE_CHECK_BOX_CONTROL

/*
  PLMN Selection 설정 상태가 Manual 상태에서 FPLMN에 있는 망 ( KT망 선택)
  을 선택해서 reject을 받고 단말을 재부팅하면 RPLMN(SKT) 으로 등록을 한후
  곧바로 이전에 reject 받았던 망으로 재등록을 시도하는것을 방지.
*/
#define FEATURE_SKY_CP_BLOCK_RESTORING_NETWORK_SELECTION

/*
   부팅 초기 socket open fail되고 4초간격으로 재시도하는 것을 1초 간격으로 8회 반복하도록 수정.
   PIN lock 화면에 조금더 빨리 진입되도록 하기 위함. 
   기타 사유....
*/
#define FEATURE_SKY_CP_RIL_SOCKET_OPEN_RETRY_MILLIS

/*
    Illegal ME reject cause 등 특정 reject cause수신 후 gsm.sim.operator.numeric property가 
    null 로 초기화 되는 현상 수정.
*/
#define FEATURE_SKY_USIM_ICC_OPERATOR_PROPERTY_NOT_RESET

/*
    USIM EF_CFIS 값과 상관없이 Call Forwarding 관련 indicator 표시하지 않도록 수정함.
*/
#define FEATURE_SKY_USIM_CALL_FORWARDING_EF_CFIS_NOT_SUPPORT

/*
    Config.xml 에 csp_enable 이 true로 설정되어 있다면 operator selection menu가 발생하지 않는다.
*/
#define FEATURE_SKY_CP_DISABLE_CSP_FLAG

#endif/* __CUST_SKY_CP_H__ */
