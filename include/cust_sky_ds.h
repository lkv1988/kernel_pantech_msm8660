#ifndef __CUST_SKY_DS_H__
#define __CUST_SKY_DS_H__


/*****************************************************
    SKY Android 모델 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_COMMON
******************************************************/
#ifdef T_SKY_MODEL_TARGET_COMMON

#endif /* T_SKY_MODEL_TARGET_COMMON */


/* ######################################################################### */
/*****************************************************
    SKT/KT 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_WCDMA
    CUST_SKY.h에 include 해야 함.
******************************************************/
#ifdef T_SKY_MODEL_TARGET_WCDMA
/************************************************************************************************************************
**    1. Related 3G-connection
************************************************************************************************************************/

/* 
 - Phone Interface를 접근 못하는 APP를 위하여 AIDL을 추가
 - aidl 선언 : ISkyDataConnection.aidl
 - 인터페이스 구현 : MMDataConnectionTracker.java
 - 추가 구현 파일 : DataPhone.java, Phone.java , SkyDataConInterfaceManager.java
 - aidl 서비스 추가 : service_manager.c에 .aidl 추가
 - make 파일 수정 : android/framework/base/Android.mk 수정
 - telephony/java/com/android/internal/telephony/ISkyDataConnection.aidl 추가
*/
#define FEATURE_SKY_DS_ADD_DATA_AIDL

/* 
 - startUsingNetworkFeature에서 Reconnect 호출시 Fail이 발생하여도 Phone.APN_REQUEST_STARTED을 리턴하여 Application에서 혼동을 제공함
 - reconnect 실패시 APN_REQUEST_FAILED를 리턴하도록 수정
*/
#define FEATURE_SKY_DS_BUG_FIX_STARTUSINGNETWORKFEATURE

// TEMP BLOCK
/*
 - Fast Dormancy 기능 규현
 - 시나리오 : LCD ON시 19초 후 Dormant 진입, LCD OFF시 5초 후 Dormant 진입
 - aArm : LCD ON/OFF 정보를 mArm으로 전달하도록 수정
 - mArm : FEATURE_SKY_DS_FAST_DORMANCY 참고
*/
#define FEATURE_SKY_DS_FAST_DORMANCY

/*
 - 약전계에서 pdp connection fail 후 retry timer 증가하면서 강전계 이동시에 증가된 timer 값으로 retry 하여 연결이 안되는것으로 
   보이는 현상발생하여 screen on 상태에서는 timer 증가시키지 않고, off->on 될 경우 바로 try 하도록수정
*/
#define FEATURE_SKY_DS_IMMEDIATE_SETUP

/* 
 - permanentfail 시 alert pop-up 및 data disable 시킴=> 변경 
 - permanent failure 발생할 경우 Notify 영역에 네트웍 오류임을 알리고, 3G 설정은 그대로 유지
 - 3G Off->On 될 때까지 Noti 유지함.
*/
#define FEATURE_SKY_DS_PDP_REJECT_POPUP

/* 
 - no service event 가 발생할 경우 실제 data 종료되지 않고 disconnected 로 broadcast 되어 default route 삭제되고 
   바로 in service 오게되면 아래단으로는 data 연결되어 있지만 상위에는 연결되지 않은 것으로 보이는 문제 
 - dataconnectiontracker.java , BSP 별로 주석처리되어 필요 없는 경우 잇음. 
*/
#define FEATURE_SKY_DS_NO_SERVICE_BUG_FIX

/* 
 - TIMEOUT_INITIAL을 1초로 줄임.
*/
#define FEATURE_SKY_DS_DHCP_DISCOVER_TIMER

/* 
 - CNE enable 되어 WIFI연결시 3G 종료되지 않는 문제점 발생.
 - CNE 부분 임시로 주석 처리. 
 - 칩에 따라 CNE가 삭제되어 있을 수 있음.
 - LINUX\android\devices\qcom\msmxxxx_surf\system.prop
*/ 
#define FEATURE_SKY_DS_CNE_DISABLE

/*
- datausage 메뉴에서 background data 차단 시 for loop 를 돌며 UID 별로 ip table 에 set 해 주어 
   app 에서 시간 내 받지 못해 anr 발생 및 background data 차단 설정 시 booting 중 system 에서 
   anr 발생하는 문제 수정   =>각 UID 별로 event 로 처리되도록 함.
- android_filesystem_config.h  에 Define 된 UID 로 system UID 값 변경. 
- system/bin/iptables 는 system 권한으로 되어 잇으나 ip6tables 는 shell 로 되어 잇어 system 으로 변경.
   -NetworkPolicyManagerService.java
*/
#define FEATURE_SKY_DS_BACKGROUND_RESTRICT_BUG_FIX

/*
- system/bin/iptables 는 system 권한으로 되어 잇으나 ip6tables 는 shell 로 되어 잇어 system 으로 변경.
   -android_filesystem_config.h
*/

#define FEATURE_SKY_DS_IP6TABLE_UID_BUG_FIX

/*
- LINUX\android\kernel\arch\arm\configs\칩이름_프로젝트이름_하드웨어차수_deconfig
*/
#define FEATURE_SKY_DS_DEFAULT_CONGESTION_CONTROL

/*
 - ISkyDataConnection에 3rd parth app에서 데이터 사용량 초과 제한을 판단할 수 있는 API 추가
*/ 
#define FEATURE_SKY_DS_NET_OVERLIMIT_API

#define FEATURE_SKY_DS_SKIP_3GPP2_PROFILE_PROCESS

#define FEATURE_SKY_DS_ICS_IPTYPE_QC_ERROR_FIX

#define FEATURE_SKY_DS_FIX_RADIO_TECH_UNKOWN_IN_CS

/************************************************************************************************************************
**    2. Related MENU / UI
************************************************************************************************************************/
/*
 - APN Settings Menu를 Hidden Code(##276#7388464#)로 실행 할 수 있도록 수정
*/
#define FEATURE_SKY_DS_ADD_APN_SETTING_HIDDEN_CODE

/*
 - 모바일 네트워크 설정 메뉴에 데이터 사용 메뉴 삭제 
*/
#define FEATURE_SKY_DS_DATA_USAGE_MENU_BLOCKING

/* 
 - APN 메뉴 사용자 영역으로 이동 (DS1_DATA_REMOVE_APN_SETTING_MENU 제거)
 - default apn에 대하여 수정 불가 하도록 수정
 - Hidden Code와 일반 사용자 메뉴 구별하여 동작하도록 수정
 - Defualt apn이 수정 될 경우, 원복시키도록 수정함. => 시나리오 변경.
   3G on 될 경우, apn이 "web.sktelecom.com"이 아니면 apn을 원복시킴.
*/
#define FEATURE_SKY_DS_PREVENT_EDIT_DEFAULT_APN

/* 
 - overylay 폴더에 framework단 resource 등록 => KT
 - strings_dp.xml 사용 (framework/base/core, package/apps/phone, package/apps/settings) => SKT
*/
#define FEATURE_SKY_DS_RESOURCE

/*
 - easy setting 기능 추가로, socket data moe 설정하는 부분에서 intent broadcast 해줘야 함.  
 - SKT 와 KT 규격 차이로 data mode setting 하는 부분 차이.
*/
#define FEATURE_SKY_DS_EASY_SETTING

/*
 - 데이터로밍 체크 후 팝업에서 조명 off 되고 홀드 해제하여 팝업 허용 선택시 체크되지 않는 현상 수정
*/
#define FEATURE_SKY_DS_FIX_ROAM_CHECK_UI_BUG

#define FEATURE_SKY_DS_ICON_NO_SRV_CR347576

/************************************************************************************************************************
**     3. CTS / PortBridge / DUN / Log /vpn
************************************************************************************************************************/


/* 
- Data Manager 관련 처리. 
- AT$SKYLINK command 로 data manager 실행시키고, 
- /dev/pantech/close file 에 내용이 삭제되면 data manager 가 종료된 것으로 처리한다. 
-/port_bridge/ , skydunservice.java
*/
#define FEATURE_SKY_DS_DATAMANAGER 

/* 
-AT*OBEX*EXEC
-AT*SYNC*UMS
-AT*SIM*INFO
*/
#define FEATURE_SKT_DS_SYNC_MANAGER

/* 
-EF39S부터, Qualcomm이 제공하는 ATFWD_daemon을 사용함.
-SkyDunService.java 삭제 및 Service 실행 막음.
*/
#define FEATURE_SKY_DS_ATFWD_PROCESS

/* 
 - root process issue 로 devices.c, init.rc, init.qcom.rc 에서 수정된 내용. 
 - cnd, netmgrd, port-bridge, qmuxd
  port_bridge 의 경우 수정된 code 사용되어야 한다.
 - init.rc 혹은 init_모델명.rc 파일에서 /dev/pantech directory 를 system 권한으로 생성해줘야한다. 
*/
#define FEATURE_SKY_DS_CTS_ROOT_PROCESS

/*
 - 특정 VPN 서버 접속 안되는 문제 (참고 : android 는  SSL VPN 과 Cisco VPN 미지원)
 - external\mtpd\L2tp.c 및 kernel config(kernel\arch\arm\configs\칩이름_프로젝트이름_하드웨어차수_deconfig) 
   에서 이미 define된 내용은 주석처리하고 y로 설정 )
 - ICS 업그레이드 하면서 일단 Kernel config는 BLOCK
*/
#define FEATURE_SKY_DS_VPN_FIX

/*
- CTS testTrafficStatsForLocalhost test 를 위한 kernel config(kernel\arch\arm\configs\칩이름_프로젝트이름_하드웨어차수_deconfig) 에 CONFIG_UID_STAT=y 로 수정. 
- ICS 업그레이드 하면서 일단 Kernel config는 BLOCK
*/
#define FEATURE_SKY_DS_CTS_LOCALHOST

/*
-  usb tethering/wifi hotspot 시 google dns 서버 사용으로 일부 사이트 연결되지 않는 문제 
- ConnectivityService.java  tethering.java
*/
#define FEATURE_SKY_DS_SET_TETHERED_DNS

/*
- SUID File Finder에서 ip가 SUID로 Check되어 Fail되는 현상
- 80-N6956-2_A 문서 참고
*/
#define FEATURE_SKY_DS_CTS_SUID_CHECK

#define FEATURE_SKY_DS_REMOVE_DBG_RMNET

/* ######################################################################### */


/*****************************************************
    SKT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_SKT
******************************************************/

#ifdef T_SKY_MODEL_TARGET_KT
#error Occured !!  This section is SKT only !!
#endif

/************************************************************************************************************************
**    1. Related 3G-connection
************************************************************************************************************************/
/*
 - 3G(GPRS) Data Suspend/Resume 함수
 - GSMPhone의 disableDataConnectivity/enableDataConnectivity는 PLMN 수동 검색시 활용하면 문제가 있어 새로운 함수 추가
*/
#define FEATURE_SKT_DS_ADD_DATA_SUSPEND_FUNC

/*
 - Roaming시 APN 변경 (lte.sktelecom.com-> roaming.sktelecom.com)
   -> Roaming 지역에 진입하면 telephony.db의 해당 APN을 roaming.sktelecom.com으로 변경
      국내로 들어오면 다시 lte.sktelecom.com으로 변경
      유저가 수정 불가능한 Default APN을 DB에서 직접 수정함.
*/
#define FEATURE_SKT_DS_ROAMING_APN_CHANGE

/*
 - SKT : WCDMA 단말 QA 요구사항 
 - CS Call 중에는 PS Data를 Block하여 Call Drop을 방지 한다.
*/
#define FEATURE_SKT_DS_VOICE_CALL_PROTECTION_IN_CS_CALL


/************************************************************************************************************************
**    2. Related MENU / UI
************************************************************************************************************************/
/*====================== (1) 3G 접속 관련 UI ======================*/

/*
 - 3G Data On/Off 메뉴 추가
 - AlwaysOnSettings.java, AndroidManifest.xml, wireless_settings.xml, strings.xml(KO)
 - CheckBoxPreference 형식으로 변경 (100326, WirelessSettings.java)
 - 시나리오 변경 (요구 사항 변경, 0510)
   : 접속, 차단, 부팅시 물어봄 (Checkbox) 형식으로 변경
*/
#define FEATURE_SKT_DS_ADD_ALWAYSON_MENU
   
/*
 - PS Disable 메뉴 추가 (SKT 요구 사항)
 - System Value 추가 (SKY System)
 - PS Detach/Attach 구문 추가
 - PDP Activate/Deactivate 구문 추가
 - mMasterDataEnabled 변경 (Eclair 구조 변경으로 추가)
   : Disable 상태에서 폰 부팅후 다시 On을 시키면 mMasterDataEnabled와 enabled가 동일하여 호가 올라가지 않는다.
   : Eclipse에서 dataEnabled 변수가 setDataEnabled와 출동 나서 enableApnType 변경
   : 함수 변경 후 mMasterDataEnabled 값은 의미가 없어 기존 수정 코드 제거
 - Concurrent시 문제가 dataEnabled 변수와 충돌되는 문제점 있음
   : isDataConnectivityPossible에 getSocketDataEnableSKT()으로 비교 하도록 수정
   : isDataConnectivityPossible은 Android 기본으로 두고 아래 코드에서 막는다.
 - WIFI OFF 또는 WIFI ON상태 폰 부팅(WIFI 연결안된 상태)의 경우 reconnect으로 시도하는 경우가 있어
   enableApnType에서 mms를 제외하고 getSocketDataEnableSKT을 검사하여 막음
 - MobileDataStateTracker에 mEnabled 변수는 reconnect 시도시에만 True가 되는데 
   Framework 단에서 enableApnType 혹은 setDataEnabled를 이용하여 PDP를 올릴 경우 해당 변수가 true로 셋팅이 안되어
   ConnectivityService에서 State Change Event를 받지 못함
   이 경우 WIFI가 붙어 있는 상태에서 Default Network을 Always On 메뉴로 붙일 경우 새로 붙은 Network이 Disconnect이
   안되는 문제점 발생
   Default를 제외한 다른 Network(MMS, SUPL 등)은 reconnect으로만 접속 가능하므로 해당 문제와 상관 없음
   -> MobileDataStateTracker에서 Default Network에 대한 Connecting, Connected Event가 발생할 경우 mEnabled를 True로 설정하도록 수정
 - API rename : void setDataConnectionMode( boolean mode) / boolean getDataConnectionMode() : 10.08.19
 - 3G OFF에서 MMS 수발신 후 3G OFF되지 않는 현상 수정
   : 3G ON상태로 재부팅 후 3G ON/OFF 팝업에서 3G 차단 버튼을 누르면 DataServiceType중 DEFAULT 타입이 enabled로 남아있음(EF39S에서 disableApnType 함수 변경으로 인함)
   : 위 문제로 인해 3G OFF상태에서 MMS수/발신이 끝난 후에도 StopUsingNetworkFeatureOEM에 의해 3G OFF가 되지 않아 disableApnType 수정함.
*/
//#define FEATURE_SKT_DS_PS_DISABLE_FOR_SKT
#define FEATURE_SKT_DS_DATA_CONNECTION_API // 이름 변경 (EF39S)
 
/*
 - SKT 요구사항 : 3G Data 시 Popup으로 사용자에 Noti하여야 함
 - trySetupData시 Popup 처리
 - Always On Menu 변경
 - GsmDataConnectionTracker에서 WIFI Status를 볼수 있는 함수 추가
   : 기존에 있는 mIsWifiConnected는 wifi broadcast로 설정되는 이는 trySetupData 이후에 처리되어 Timing Issue 있음
   : 상세 시나리오 확정시 isWifiConnected 함수를 이용하여 WIFI 상태 검사 루틴 추가 예정
 - 메뉴 Popup 선택시 바로 Popup Value를 설정하지 않고 Data호 시도 후 설정하도록 수정
   (Popup 2회 발생 문제점)
 - Connection Popup에서 '아니오' 선택시 Disable 상태가 Suspend 상태가 되도록 수정
 - SKT 협으로 POP UP 처리 안하고 연결/해제 2개의 시나리오만 동작 하도록 수정 (2010.03.25)
   -> EFS 삭제후 최초 Data 시도 시에만 Pop-UP 발생 하도록 수정 (NEW_100326)
   -> GSM setupwizard에 넣는 것이 가장 좋으나 apk 형태로 받아 현재로는 불가능 (DS2팀 문의중)
      : Google에서 불가 통보
 - PLM : 7661 문제점(SetupWizard 죽는 현상)에 의해 Boot Complete 이후로 이동
 - Boot Complete가 안오는 경우 1회 발생 (폰 재부팅 전까지 Data Call 안됨)
   : Menu에서 Enable시 Boot-up 관련 Flag Reset하도록 수정
 - SKT 로밍 시나리오 변경으로 attach 이후로 팝업 발생하도록 수정.(2010.10.05)
*/
#define FEATURE_SKT_DS_ADD_3G_DATA_POPUP

/*
 - Booting시 POP-UP으로 공장에서 문제가 생김
 - 생기 요청으로 IMEI가 없을 경우 POP-UP 발생 안하도록 수정
 - Call Part 작업 필요 (qcril_other.c, ds1_cp_pantech_factory_req)
 - 개통 모드 추가
 - PS attach 이후 팝업이 뜨므로 sim 체크 삭제함 (11.04.11)
*/
#define FEATURE_SKT_DS_NO_CONNECTION_POPUP_IN_NOIMEI

/*
 - Silent Boot시 Popup 보이지 않도록 수정
 - FEATURE_SW_RESET_RELEASE_MODE가 DS2팀에서 선 구현 되어야 한다.
*/
#define FEATURE_SKT_DS_SW_RESET_RELEASE_MODE_NO_DATA_POPUP

/*
 - 3G Data 시도중 Toat Popup 출력 하도록 수정 
*/
#define FEATURE_SKT_DS_MAKE_PDP_CONNETION_TOAST

/*
 - PS attach reject 받고 3G 접속 시도시 reject 팝업
 - detach 시도시 ps reject cause 리셋 => SBSM 요구사항 변경으로 detach 하지 않음.(10.10.19)
 - 3G only app이 없으므로 startUsingNetworkFeature() 함수에서 체크하는 것을 제거하고, RTS 테스트를 위해서 
   데이터 로밍 설정 팝업에서 허용을 선택할 경우나 설정 메뉴에서 로밍 설정을 체크할 경우 PS reject cause 팝업(10.10.19) 
*/
#define FEATURE_SKT_DS_PS_REJECT
 
/*====================== (2) 로밍 관련 ======================*/

/*
 - 부팅 시 로밍 허용 팝업 
 - 로밍일 경우, 로밍 체크 여부만 확인 함.
 - 기존 소스에서 3G 접속 허용 여부만을 체크하는 부분에 로밍일 경우, 로밍 체크 여부를 확인 하도록 추가함.
 - 차단 상태에서 로밍 지역에서 부팅할 경우, PS attach 하기 위해 모드 전환함.
 - DS1_DATA_FRAMEWORK_GPRS_DIABLE 제거함.
   로밍 시 3G 연결 설정 값은 의미가 없어지고, 로밍 설정 값으로 always on 동작을 하게 되므로, GSM 지역에서도 enable 함. 
   PS reject를 받으면 메뉴가 disable 되고, 3G 접속 (MMS)를 하면 3G 연결 설정을 하라는 팝업이 발생함.  
   PS attach reject 받으면 메뉴가 disable 되고, 이 경우, 호 시도를 하지 않아 SKT RTS 테스트가 fail 됨. 
 - 차단 상태에서 로밍 지역에서 부팅할 경우, PS attach 하기 위해 모드 전환함. 
 - RTS 장비 테스트 중 PDP accept, reject을 반복하며 접속 테스트를 할 경우, 망으로 부터 permanent reject cause가 내려오면
   설정을 체크, 언체크해도 접속을 시도하지 않아 항목이 fail됨. (2011.01.28)
   => setDataRoamingEnabled()에서 EVENT_ROAMING_ON을 발생 시키고, onRoamingOn()에서 roaming일 경우, 설정 값에 따라
      data 접속을 연결, 해제 하도록 수정함.
   => onRoamingOff()가 부팅시 불려지기 때문에 onRoamingOff()에 접속 해지 코드를 넣고, EVENT_ROAMING_ON/OFF를 따로 발생 시킬 경우,
      부팅 시 호접속이 되지 않음.
*/
#define FEATURE_SKT_DS_ROAMING

/* - 데이터 로밍이 안되는 지역에서 3G 설정 메뉴 disable함. (SKT 요구사항)
 - GSM / GPRS 망에서 disable 됨.
 - 로밍 요구사항이 변경되어 로밍일 경우, 무조건 disable 됨.
 - 부팅 시 보내 intent를 easy setting이 받지 못하는 경우가 발생하여 팝업 처리 버튼 click 될 때 intent를 다시 발생시킴
*/
#define FEATURE_SKT_DS_ALWAYSON_MENU_DISABLED_IN_GPRS

/*====================== (3) Application 관련 ====================== */

/* - DUN Profile에 경우 startUsingNetworkFeature호출시 끊어지지 않아야 함
   * DUN Profile에 경우 EVENT_RESTORE_DEFAULT_NETWORK를 호출 하지 않도록 수정   

 - Froyo 버전에서 config.xml에 networkAttributes arryay에 dun이 삭제되어 SKAF 접속이 안되어 mobile_dun 추가함.(10.09.08)
   * fetchDunApn() 함수를 사용하는 부분 막음.
     => EF30S의 경우, 해당 부분 없음.
*/
#define FEATURE_SKT_DS_SKAF_SUPPORT

/*
 - SKT 요구사항 : 3G Data가 Disable 되어도 MMS 송/수신은 정상 동작 해야 함
 - Data Diable로 설정되어 있어도 MMS 수신시 PDP를 올릴수 있도록 수정
 - 요구 사항 변경으로 해당 기능 삭제
 - Data Disable시 MMS이외에 다른 Network 사용 불가하도록 처리 가능하므로 다시 적용
 - 2010.04.08 요구사항 변경으로 OEM MMS만 허용 하도록 변경 : Disable시 OEM MMS를 제외한 다른 모든 Feature Disable 하도록 수정
 - ISkyDataConnectivity.adld에 requestRouteToHostOEM() 추가(2010.09.06)
*/
#define FEATURE_SKT_DS_ALLOW_MMS_IN_DATA_DISABLE

/*
 - HD app 구동을 위해서 VT / Voice call type 구분이 필요함.
 - HD app 업체에서 isVideoCall() 함수 제공을 요청하여 구현함.
 - HD영상통화APK 추가 (Pre-load 형식)
   * android/pantech/apps/HDVideoCall 디렉토리 추가
   * HDVideoCall_LINK.apk와 HDVideoCall.apk 업체 Signing만 사용 (AIR 다운로드시 Signing이 일치해야됨)
   * 메뉴 -> HD영상통화 최초 클릭 시, HD영상통화 Re-Install 절차에서 '설치가 차단됨' 메세지 뜨지않도록 처리함.
     - android\packages\apps\PackageInstaller\src\com\android\packageinstaller\PackageInstallerActivity.java
 - EF33S부터는 T service안에도 적용되도록 했음.
   * LINUX\android\pantech\apps\Tservice\src\com\pantech\apps\tservice\TServiceActivity.java에 activity실행되도록 추가
   * LINUX\android\pantech\apps\Tservice\res폴더의 draw 및 values 폴더 등에 string 및 icon 이미지 적용했음.
     적용된 icon 및 string은 TServiceActivity.java에 있는 내용을 참고하도록..
 - EF39S, EF40S는 LTE 초기 단말로서, 미탑재로 결정되어서 해당 Anroid.mk파일 수정과 featuring된 부분 주석 처리함.
   단, T serive에 적용된 string과 icon은 그대로 존재함.
*/
#define FEATURE_SKT_DS_CALL_TYPE_FOR_HD_SERVICE


/*====================== 4. 그외 메뉴 ======================*/

/* 
 - hidden menu에서 RRC version 선택에 따라 category 선택이 되도록 수정함.
 - HSUPA category는 NV 처리가 되어 있지 않아서 추가함.
 - R4 only 선택 시 HSDPA category / HSUPA category 항목 disable로 변경함.
 - R5(HSDPA) 선택 시 HSDPA category enable / HSUPA category disable로 변경함.
 - R6(HSDPA) 선택 시 HSDPA category disable / HSUPA category enable로 변경함.
 - HSUPA를 지원하지 않는 칩일 경우, R6 설정 시 폰이 리셋되므로 rrc version 설정에서 R6를 반드시 제거해야 함. 
*/
#define FEATURE_SKT_DS_HSUPA

/************************************************************************************************************************
**     3. CTS / PortBridge / DUN / Log /vpn
************************************************************************************************************************/


/************************************************************************************************************************
**     4. VT
************************************************************************************************************************/
#define FEATURE_PANTECH_VT_SUPPORT

#define FEATURE_PANTECH_VT_SUPPORT_QMI

/************************************************************************************************************************
**     5. VoIP(SKT VoIP/TAPS)
************************************************************************************************************************/

/*
- TAPS를 위한 피쳐
- LINUX\android\pantech\apps\SkyTAPSClient 폴더를 이외의 TAPS관련 기능 피쳐링
- 현재(2010/11/11)까지는 TAPS 구동 시점(VENT_INTERFACE_CONFIGURATION_SUCCEEDED)에 관한 내용만 있음.
- 추후 기능 추가 시 feature명 분리필요
*/
#define FEATURE_SKT_DS_TAPS

/*
- VoIP registring/registered 상태에서는 WiFi 전원절약 동작하지 않도록함
*/
#define FEATURE_SKY_DS_SKT_VOIP_BLOCK_WIFI_CUT_DOWN

/*
- 전원 Off 시 3초간 block하면서 voip deregistering시도
- user가 wifi off 시 3초간 blocking하면서 voip deregistering시도
*/
#define FEATURE_SKY_DS_SKT_VOIP_USER_POWER_OFF_HANDLE

/*
- Airplane mode진입 시 3초간 block하면서 voip deregistering시도
*/
#define FEATURE_SKY_DS_SKT_VOIP_USER_AIRPLANE_MODE_HANDLE

/*
- VoIP 디버그스크린. 따로화면을 구성하지않고 WifiScerrn.java에 추가함
*/
#define FEATURE_SKT_DS_VOIP_DEBUG_SCREEN

/*
- VoIP Test Menu 진입코드만 피쳐링되어있고, LINUX\android\pantech\apps\SkyVoipTest 폴더가 추가되어야동작함
- 출시이전에는, hidden string을 block시키던지, SkyVoipTest에서 xml을 수정해서 해당 메뉴를 disable시킴.
- EF13S는 아예 진입 하지 못하도록 했고, EF30S는 진입은 되지만 해당 메뉴가 disable됨. BMT기간 중에 따로 voiptest.apk를 제공해서
  해당 메뉴를 활성화시키게 해줌.
- 진입코드는, ##362#(##fmc#)
*/
#define FEATURE_SKT_DS_VOIP_HIDDEN_MENU

/*
- FMC Configuration 메뉴에서 TAPS/수동개통 지원가능하도록 구현
- 수동개통 시 TAPS관련 체크하지않도록 수정
- TAPS선택 시 TAPS 초기화 후 재시작하도록 수정
- fmcconfig.xml 에서 TAPS 선택 체크박스 disable시킨부분 삭제
*/
#define FEATURE_SKT_DS_VOIP_MANUAL_CFG

/*
- GingerBread에서 탑재된 SIP을 사용하지 않도록 막음.
- LINUX\android\device\qcom\common\common.mk 또는 LINUX\android\device\qcom\msm7627_surf\msm7627_surf.mk에 적용한다.
- EF31S는 MSM7627 이므로, msm7627_surf.mk에 해당된다. 모델마다 surf mk 파일이 다를 수 있다.
*/
#define FEATURE_SKY_DS_BLOCK_GB_BUILTIN_SIP

/*
- VoIP provider를 사용하기 위해서 LINUX/android/framework/base/Android.mk 파일에 추가
- VoIP provider는 별도의 경로에 실제 적용되어 있음.
*/
#define FEATURE_SKY_DS_APPLY_VOIP_PROVIDER

/*
- voip application은 부팅후 실행한 후 계속 존재하는 process이다. 단, applicaton process가 어떤
이유에서 다시 실행될 때는, 처음 부팅할 때와 다른 routine을 타야 한다.
따라서, 처음 부팅할 때 실행되었는지, 아니면 재 실행되었는지를 check하기 위해서,
file system의 /data/voipdone/voip_done의 파일 유무로서 알 수 있게끔 해 놓았다. 이 부분은, init.rc와 같은 계열의
file에 설정되어 있다. VoIPApp.java의 onCreate부분에서 voip_done file의 유무를 체크하는 flow가 있다.
*/

/*
- EF31S, 39S, 40S는 VoIP 미탑재로 출시됨.
- 따라서, 위의 featuring부분을 막던지, 폴더로 구현되어 있는 부분은 Android.mk에서 빌드를 막음.
- Android.mk에서 막은 부분은 아래 feature로 찾으면 됨.
*/
#define FEATURE_SKY_DS_BLOCK_VOIP

/************************************************************************************************************************
**     6. GingerBread SIP
************************************************************************************************************************/
/*
 - SKB에 regi를 위해 SIP header중 from/to header의 port처리 삭제함.
*/
#define FEATURE_SKT_DS_SIP_PORT_DEL

/*
 - SKB regi 유지를 위해 REFRESH를 구현함.
*/
#define FEATURE_SKT_DS_SIP_REGI_REFRESH



/* ######################################################################### */
/*****************************************************
    KT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_KT
******************************************************/
#ifdef T_SKY_MODEL_TARGET_KT
#ifdef T_SKY_MODEL_TARGET_SKT
#error Occured !!  This section is KT only !!
#endif

/************************************************************************************************************************
**    1. Related 3G-connection
************************************************************************************************************************/

/*
 - 3G(GPRS) Data Suspend/Resume 함수
 - GSMPhone의 disableDataConnectivity/enableDataConnectivity는 PLMN 수동 검색시 활용하면 문제가 있어 새로운 함수 추가
 - getDataConnectionState() 에서 connected가 아니면 전부 disconnected 로 return 해서 connecting 상태에서 수동검색 진행되는 문제로 connecting 추가.
*/
#define FEATURE_KT_DS_SUSPEND_RESUME_FUNC

/*
 - KTF 요구사항 : 3G Data가 Disable 되어도 MMS 송/수신은 정상 동작 해야 함. 
 - Data Diable로 설정되어 있어도 MMS 수신시 PDP를 올릴수 있도록 수정
*/
#define FEATURE_KT_DS_ALLOW_MMS_IN_DATA_DISABLE

/*
- 해당 값이 없을 경우 true로 처리 하도록 수정
*/
#define FEATURE_KT_DS_CHANGE_SDC

/*
 -KT 3G Data 접속 시나리오 적용 =>> system property 에서 db 의 SOCKET_DATA_CALL_MODE 를 생성해서 적용. kaf oem api 0.9.0 에 default 값을 popup 으로 요구됨.
- setup alert popup 에서 설정 선택시 data mode setup 화면으로 이동.
- data mode setup 화면은 skydatamodesettings.java 로 구현됨. => \packages\apps\Phone\src\com\android\phone\settings.java 로 변경.
*/
#define FEATURE_KT_DS_DATA_SETUP

/*
 - data/dun setup 가능한지 체크시 KT PS reject cause 체크 및 toast. 
 - 네트워크 등록중인 경우에는 toast 처리하고 발신시도해야함. skydunservice.java
*/
#define FEATURE_KT_DS_PS_REJECT

/*
 - 3G OFF 상태에서 MMS 전송시 DNS 쿼리  IP add  결과가 NULL로 넘어 오는 현상 발생. 
 - unknownhost일 경우 cache 에 저장되지 않도록 주석 처리. 
*/
#define FEATURE_SKY_DS_JAVA_CACHE

/************************************************************************************************************************
**    2. Related MENU / UI
************************************************************************************************************************/
/*
  packages\apps\Phone\src\com\android\phone\settings.java 및 network_settings.xml 에서 처리하도록 변경됨. 
*/
#define FEATURE_KT_DS_ADD_ALWAYSON_MENU

/*
 - EF18K IOT 1차 LMS 1, SBSM 8 요청 사항에 대한 MMS TestMenu 추가 요청에 대해 ##46632874# > 엔지니어모드에 추가 함.
 - \LINUX\android\packages\apps\Settings\src\com\android\settings\skyhiddenmenu\KtHiddenMenu.java
 - \LINUX\android\packages\apps\Settings\res\xml\kt_hidden_menu.xml
 - \LINUX\android\packages\apps\Settings\res\values\strings_cp.xml
*/
#define FEATURE_KT_MMS_TESTMENU

/*
  - Lab Test Menu에 vt emulator 메뉴 추가하여 ext 324 NV 설정기능 추가
*/
#define FEATURE_KT_DS_VT_EMULATOR

/*
- connectivityservice.java 에서 tryFailover() 에서 reconnect 할 때 mobile data enable 되어있는지 체크하는 부분 kt data mode 로 변경함.
*/
#define FEATURE_KT_DS_DATAMODE_CHECK_MODIFY

/*
- lock 상태에서 incoming call 수신된 상태에서 wifi 연결 종료되어 data pop 표시되고 설정 선택하면 lock 화면 보이는 문제.
- telephonyintent 에 ringcall start/end 추가하여 broadcast 하면 data popup에서 ringing 이면 설정메뉴 진입못하게함. 
*/
#define FEATURE_KT_DS_POPUP_IN_RINGSCREEN_BUG

/* 
- Roaming 시 data roaming enable uncheck 상태이면 notification 영역에 보여주고 지워지지 않도록 한다.
- 설명이 두줄 로 보이도록 함, 선택시 설정 메뉴로 이동. 
*/
#define FEATURE_KT_DS_ROAMING_SETUP



/************************************************************************************************************************
**     3. CTS / PortBridge / DUN / Log
************************************************************************************************************************/
/*
: KT OTA command 처리 "AT*KTF*OPENING" ./port_bridge/ , skydunservice.java
*/
#define FEATURE_KT_DS_EIF_OTA 

/*
  - CS only 인 경우 DUN 연결 시도하지 못하도록 함.
  - mmdataconnectiontracker.java 에서 ps restricted 인 경우 
  gsm.net.psrestricted property 에 1을 써주고 skydunservice 에서 이 값을 체크한다.
*/
#define FEATURE_KT_DS_DUN_BLOCKING_IN_CS_ONLY

/************************************************************************************************************************
**     4. VT
************************************************************************************************************************/

/*
- KT 영상전화 단말 규격 disconnect cause 추가, qcril 에서만 undefine 해 주면 된다. 
- local ringback tone 관련 call progress info(#1,#2,#3,#8) 체크 값 추가. 
*/
#define FEATURE_PANTECH_VT_SUPPORT

#define FEATURE_PANTECH_VT_SUPPORT_QMI

#endif/* T_SKY_MODEL_TARGET_KT */


/************************************************************************************************************************
** 삭제된 feature
************************************************************************************************************************/

/*  
#define FEATURE_SKY_DS_PRESERVATION_WAKEUP_BUG_FIX
=> FEATURE_SKY_DS_NO_SERVICE_BUG_FIX 로 변경. 
- Preservation 상태에서  RA update 중에 rrc abort 로 limited event 수신 시 이후 Preservation 벗나어지 못하는 문제 수정 
*/

/*  
#define FEATURE_SKY_DS_CNE_DISABLE ==> 풀어야 함. 
 => CNE 삭제됨. 
 - 2030 버전에서 CNE enable 되어 WIFI연결시 3G 종료되지 않는 문제점 발생.
 - CNE 부분 임시로 주석 처리. 
*/

/*
#define  FEATURE_SKY_DS_STABILITY_TEST
 =>  cust_sky.h 에 FEATURE_PANTECH_STABILITY 로 featuring 됨. 
*/

/*
#defien FEATURE_SKY_DS_ADD_DATA_LOG
- framework 단 로그 추가 내용, 삭제함. 
*/

/*
#define FEATURE_SKY_DS_CHANGE_MASTER_VALUE_TURE
 - mMasterDataEnabled이 false가 되어 Data호를 시도 하지 못하는 현상 수정
 - 재현이 어렵고 mMasterDataEnabled의 경우 eclair 이후 그 존재 의미가 없어 Data호 시도시 해당 Value 체크 구문 삭제 
   =>3G 설정 값이나 로밍 설정 값을 반영하려고 했으나, 차단시 MMS 발신 때문에 무조건 true로 리턴하도록 수정.
*/

/*
#define FEATURE_SKY_DS_FAIL_CAUSE
 - PDP Fail cause 를 가져오지 않는 문제로 이전 코드 적용.
 - EF39S 에서 수정됨
*/

/*
#define FEATURE_SKY_DS_CHANGE_IN_AVAILABLE_CHECK_IN_STARTUSINGNETWORK
 - 간헐적으로 dataConnectionChanged Action에서 저장되는 Network Info의 isAvailable 값이 false로 저장되어 있는 경우 발생
 - 위 조건은 1.sim, 2.roaming, 3.서비스 상태, 4.PDP 실패 상태 (Default Enable , DISCONNECTED)를 Check하는데 
   4번 조건의 경우 NATE 등에서 PDP를 다시 Activate를 시키는 방안이 사용자 편의를 위해 좋으므로 조건 변경 
*/

/*
#define FEATURE_SKY_DS_CHANGE_DEFAULT_ROUTE_FOR_MOBILE
 - WIFI 중 3G 연결시 default router가 2개 잡히는 현상 수정
 - WIFI, Data 설정 상태를 모두 확인 하기 위하여 dhcpclient에서 add route하는 구문을 framework로 이동 (ConnectivityService)  
 - 3G Data중 WIFI 연결시 기존 3G Default Network 삭제
 - default route add/remove function MobileDataStateTracker.java 에서 처리하도록 수정.
 - 3G connected event broadcast 된 직 후config 읽으면 interface up 되지 않는 문제로 broadcast 전에 interface up check. 
   (개인정보 상태에서 초기화 후 마켓 자동 다운로드 되지 않는 문제.)
 - 3G 종료 후에도 config 값 읽으면 한참 후 interface down 으로 변경되어 check 하는 부분 추가. (tethering 에서 문제)
 -[KTF] remove dns route 동작하지 않는 문제로 mIPv4InterfaceName, mIPv6InterfaceName 을 disconnect 에서 null 로 만드는 부분 삭제. 
 
 - addDefaultRoute()에서 mDefaultRouteSet[index] == false를 체크 하지 않도록 수정(2011. 02. 11)
   => Disconnect state로 변경되지 않는 경우가 발생하여, handleConnectivityChange() 함수를 타지 않는 경우, mDefaultRouteSet[index]
      값이 false로 변경되지 않아 이후 데이터 접속이 되지 않음.
*/

/* 
#define FEATURE_SKY_DS_AVOID_FATAL_EXCEPTION
 - PDP Fail 시 loggable fail cause 이면 logging 남길때 단말 죽는 문제로 isEventLoggable() 에서 항상 false 리턴하도록함.
*/

/*
#define FEATURE_SKY_DS_DISCONNECT_CHECK
 - radio off event 발생할 경우(rild dead or lpm or rpc reset..etc) data disconnect 시켜서 radio on 될 경우 정상동작 하도록 보완코드 추가.
 - mmdataconnectiontracker.java onRadioOff()
 - FEATURE_KT_DS_DISCONNECT_CHECK 에서 공통 feature 로 변경. 
*/

/*
#define FEATURE_SKY_DS_DNS_CONFIG_BUG_FIX
- starUsingNetworkFeature를 같은 Feature로 2개의 APP에서 호출할 경우
  2번째 APP부터 DNS Query가 3G로 나가지 못하는 현상 수정
*/

/*
#define FEATURE_SKY_DS_PARAMS_BUG_FIX
- tearDownData()의 파라미터가 잘못 되어 수정함. 
*/

/*
#define FEATURE_SKY_DS_APN_MMS_BUGFIX
 - 단말 설정 메뉴내 "새 APN"(default.ktfwing.com) 추가 설정, 
 - MMS 메시지 발신 후 (WIFI ON/3G ON) APN 확인 시 기본 APN을 유지하고 있는 문제 수정.
*/

/*
#define FEATURE_SKY_DS_ANY_DATA_INTENT_RECOVERY
- ACTION_ANY_DATA_CONNECTION_STATE_CHANGED intent 가 전달되지 않는 경우가 발생하여
   intent broadcast(notifyDataConnection) 후 connectivityservice 에 update 가 되엇는지 체크.
 - dataconnectiontracker.java mmdataconnectiontracker.java
*/

/*
#define FEATURE_SKY_DS_TETHERING_HELP
 - 테더링 관련 도움말에 나와 있는 URL이 Nexus One 내용이므로 해당 내용 삭제(상기협의)
 - Wifi hotspot에 대한 도움말이 Wifi 테터링으로 설명되어 있어 수정함.(한글 수정, QE 문제점)
*/

/*
#define FEATURE_SKY_DS_ADD_NETTEST
 - network interface, APN, socket, Concurrent(3G/WIFI) 관련 테스트 APP 추가
 - WIFI Debug Screen 추가
 - android/pantech/apps/skyLabTest 폴더 
 - android/pantech/apps/NetTest 폴더
*/

/*
#define FEATURE_SKY_DS_TETHER_STATE_INIT
 - tether setting 에서 oncreate 시 초기값 update   
    tethersettings.java
*/

/*
#define FEATURE_SKY_DS_DEFAULT_DATA_ICON
- Data 연결 ICON default 값을 3G 로 설정하여 G 아이콘이 보이지 않도록 수정. 
*/

/*
#define FEATURE_SKY_DS_PORTBRIDGE_EXTERNAL
 - port_bridge(dun 관련) 처리 공통 feature. ./port_bridge/ , skydunservice.java 
 - 초기 부팅시 dun_ext_smd_ctrl에서 TIOCMGET에 대한 정보가 없으면 dun_monitor_ports스레드가 시작되지 않음. (TIOCMGE정보 보다 스레드가 먼저 생성) 
 - dun_port_dataxfr_up 스레드가 멈추지 않고 돌면서 CPU의 대부분을 점유함.
 - MODEM의 TIOCMGET정보가 생성되기 전에 dun_monitor_ports 스레드가 생성 되면 다시 스레드가 생성 되게 수정.
 - EF39S부터 더 이상 사용하지 않음.
*/

/*
#define FEATURE_SKY_DS_IDLE_PDL
-AT*PHONEINFO command 에 PDL command 실행시킴. system servier로 command 전달.
-idle download support
*/

/*
#define FEATURE_SKY_DS_DUN_USER_MODE 
 - 문제점 : QualcommSetting 디렉토리에 있는 파일들은 user mode 시 실행되지 않으므로, dun, sync manager, data manger, curi explore가 실행되지 않음.
 - DunService.java 대신 LINUX\android\packages\apps\Phone\src\com\android\phone\SkyDunService.java를 추가함.
 - SkyPhoneBroadcastReceiver.java 에서 dun service start 하고, Dun_Autoboot.java에서는 서비스 start 하는 부분을 막음.
 - QualcommSetting\AndroidManifest.xml에서 dun service를 제거하고, packages\apps\Phone\AndroidManifest.xml에 sky dun service 추가
 - EF39S부터 더 이상 사용하지 않음.
*/

/*
#define FEATURE_SKY_DS_DUN_SERVICE
  - dun 연결상태를 mmdataconnectiontracker.java 에서 저장하고 state 를 return 한다.
*/

/*
#define FEATURE_SKY_DS_RELATED_DUN_UI
 - 전화접속 연결 시 연결/해제 시 toast로 연결/해제 알림
 - notification 영역에 연결 중 상태임을 알림
 - port_bridge에서 SkyDunService.java에서 ui 처리를 하기 위해서 DUN_EVENT_RMNET_DOWN일 경우에도 dun_disable_data_connection()에서 dun file에 DUN_INITIATED를 write 함.
 - port_bridge에서 SkyDunService.java에서 ui 처리를 하기 위해서 DUN_EVENT_RMNET_UP일 경우에도 dun_enable_data_connection()에서 dun file에 DUN_END를 write 함.
 - 3G 접속과 전화접속이 동시에 일어날 경우, DUN_STATE_CONNECTED 상태에서 3G 접속이 먼저 일어날 경우, 전화접속은 연결되지 않고, UI는 연결 중이 된다. 
   이를 해결하기 위해서 DUN_STATE_CONNECTED에서 DUN_EVENT_RMNET_UP 올 경우, 전화접속을 종료한다.
*/

/*
#define FEATURE_SKY_DS_DUN_TEST_MENU
 - Froyo 버전에서 Tethering 기능이 추가되어 전화 접속은 nettest 메뉴에서 설정해야만 연결할 수 있도록 구현함.
 - \LINUX\android\pantech\apps\NetTest\src\com\pantech\app\nettest\DunTest.java 추가 
 - \LINUX\android\pantech\apps\NetTest\res\layout\duntest.xml 추가 
 - [kt] 전화 접속 테더링 동시 지원으로 삭제 -  android\pantech\apps\nettest\AndroidManifest.xml 에서만 주석 처리 
*/

/*
#define FEATURE_SKY_DS_WIFI_USB_TETHERING
 - USB tethering 에서 wifi 연결 된 경우 wifi 우선 사용하도록 config.xml 및 tethering.java 에 수정 및 추가. 
*/

/*
#define FEATURE_SKY_DS_AVOID_UNKNOWN_RADIO_TECH
- Radio Tech Unknown(Connected)일경우 통신은 가능하나 Brower에서 Error Pop-up 발생하는 현상 수정
*/

/*
#define FEATURE_SKY_DS_BAD_IP_CHECK
- IP, GW, DNS 정보 없는 문제점 수정
- Dataconnection.java에서 해당 정보 없을 경우 TearDown하도록 수정
*/

/*
#define FEATURE_SKT_DS_NOT_SUPPORTED_IPV6_CONNECT
 - SUPPORT_IPV4, SUPPORT_IPV6가 모두 true로 되어 있어서, trySetupDataCall()이 두 번 불려지게 됨.
 - 막지 않아도 모뎀단에서 두번째 호는 fail이 나기 때문에 trySetupDataCall()이 한번만 되도록 수정함.
*/

/*
#define FEATURE_SKY_DS_MOBILE_INFO_BUG_FIX
 - mobiledatastatetracker.java 에 default route, dns 정보가 null 로 전달 되는 경우 data reconnect 하도록 보완코드 추가. 
*/

/*
#define FEATURE_SKT_DS_DISCONNECT_ALL_NETWORK_FEATURE
 - AlwaysOnSetting 메뉴에서 접속안함을 선택했을 경우 MMS, GPS, NATE 등이 사용중이면 PDP 안 끊어지는 문제
 - startUsingNetwrokFeature를 사용한 모든 APP에 대하여 stopUsingNetworkFeature를 호출한 후에 disableDataConnectivitySKT을 호출 하도록 수정 
*/

/*
#define FEATURE_SKT_DS_IF_DOWN_IN_DOWN_STATE
 - 문제 현상
    bring_down와 bring_up이 동시에 불리면서 NET_LINK가 DOWN->UP이 됨.
    -> 실제 Net이 다운되어 netmgr state는 down이 되어도 실제 rmnet0의 iface는 up 상태를 유지함
    -> 맨 마지막에서 bring_up을 하고 rmnet0의 interface를 IFF_UP을 시켜도 UP -> UP 으로 변경되어 Kernel Event가 발생하지 않음.
    -> netmgr은 down state고 framework는 connecting state로 남아 있어 폰 재 부팅 전까지 3G Data가 되지 않음.
 - 수정 사항
    Modem Interface가 Down되었을 때 현재 netmgr이 Down State임에도 불구하고 IFF_UP 상태 일 경우는 IFF_UP mask를 해지하도록 수정

*/

/*
#define FEATURE_SKY_DS_DEFAULT_ROUTE_RESTORE
 - mDefaultRouteSet[index]는 true이고, defaultGatewayAddr = 0 인 경우가 자주 발생하여, SCREEN ON 시에 체크하여 
   defaultGatewayAddr가 0일 경우는 teardown => reconnect 함. 
*/

/*
#define FEATURE_SKT_DS_DISABLE_MASTER_VALUE_IN_TRY_SETUP_DATA
 - mMasterDataEnabled이 false가 되어 Data호를 시도 하지 못하는 현상 수정
 - 재현이 어렵고 mMasterDataEnabled의 경우 eclair 이후 그 존재 의미가 없어 Data호 시도시 해당 Value 체크 구문 삭제 
   =>3G 설정 값이나 로밍 설정 값을 반영하려고 했으나, 차단시 MMS 발신 때문에 무조건 true로 리턴하도록 수정. 
*/

/*
 - SKT : WCDMA 단말 QA 요구사항 
 - 한도 소진된 경우, Background 데이터 호 시도 금지.
 - RRC release cause가 Pre-emptive release일 경우, 설정 -> 계정 및 동기화 설정 -> 백그라운드 데이터 uncheck함.
 - notification 영역에 한도 제한 안내 -> RRC release의 cause가 Pre-emptive 가 아닐 경우, 노티 제거
 - WIFI connected 상태일 경우, 사용자 설정대로 원복 시킴.
*/
/* #define FEATURE_SKT_DS_PRE_EMPTIVE_RELEASE */

/*
#define FEATURE_SKT_DS_DNSMASQ_DNS_FORWARD
 - SKT : WCDMA 단말 QA 요구사항 
 - DNS MASQ를 이용한 DNS Forward기능 구현
*/

/*
#define FEATURE_SKT_DS_CHANGE_MTU
 - SKT : 품보 요구사항 (EF39S)
 - 망에서 Fragmentation 방지를 위해 MTU Size를 1440으로 변경
 - 속도 문제로 1500으로 원복 (규격서 Update되면 수정 예정, 110930)
*/

/*
#define FEATURE_SKT_DS_NOT_USE_THROTTLE_NTP
 - 불필요한 패킷 발생을 줄이기 위해 ThrottleService에서 Sntp 사용하지 않도록 함.
*/

/*
#define FEATURE_SKT_DS_MOVE_NETWORK_SETTINGS_MENU
 - 사용상 편의를 위해 VPN 설정 메뉴를 모바일 네트워크 다음으로 정렬함.
*/

/*
#define FEATURE_SKT_DS_GSM_NOT_SUPPORT_GPRS
 - SKT 모델 중 GPRS를 지원하지 않는 모델만 적용해야 함. 
 - GPRS 망일 경우 로밍 설정 팝업만 띄우고, 접속 시도는 하지 않도록 함.
*/

/*
#define FEATURE_SKT_DS_ALLOW_MMSONLY_APN_IN_DATA_DISABED
 - 3G차단 상태 MMS 송수신 허용관련 Test Firmware SKT 요청사항 구현
 - 단말에서 3G 접속 차단을 설정한 경우, 기존 접속된 web.sktelecom.com APN을 해제하고, mmsonly.sktelecom.com APN을 사용
 - MMS 송수신이 시도 시에 PDP context를 설정하고, 송수신이 완료되면 PDP context도 해제
 - MMS client외의 application에 의해 mmsonly.sktelecom.com APN을 통한 무선데이터 접속은 차단
 - Test Binary에서는 MMS client외의 App도 mmsonly.sktelecom.com APN을 통한 무선데이터 접속 허용
 - OME MMS만 접속 허용하므로 FEATURE_SKT_DS_ALLOW_MMS_IN_DATA_DISABLE 반드시 포팅해야 함.
 - EF47S : OMA MMS 적용으로 starUsingxxxOem 제거, 3rth Party MMS도 3G OFF상태에서 MMS전송 가능하도록 수정 (SKT Sim)
         : isAvailable 함수 변경 (SKT SIM, 국내 일경우 MMS에 대하여 3G OFF에서도 true return 하도록 수정)
*/

/*
#define FEATURE_SKT_DS_HSPA_PLUS
 - R7(HSPA) 선택 시 HSDPA category enable / HSUPA category enable 되도록 구현. 
*/

/*
#define FEATURE_SKT_DS_POPUP_ROTATION
 - 1. 설정 - 무선 및 네트워크 - 데이터 네트워크 설정에서 접속 허용과 허용하지 않음을 선택시에 뜨는 팝업이 가로모드로 바뀌면 사라지는 현상 수정 
 - 2. 설정 - 무선 및 네트워크 - 모바일 네트워크 - 데이터 로밍 체크 시 뜨는 팝업이 가로모드로 바뀌면 사라지는 현상 수정
*/

/*
#define FEATURE_SKT_DS_DUN_NUMBER
 - 1501로도 전화접속이 되게 함.
*/ 

/*
#define FEATURE_SKY_DS_DEBUG_SCREEN
 - Data RX, TX count 하여 debug screen에 표시함.
 - AARM, MARM 따로 표시함.
*/

/*
#define FEATURE_SKT_DS_REMOVE_WIFI_EVENT_LOG
 - Becon 주기마다 WIFI Driver에서 올라오는 Event로 ADB Log가 과다하여 디버깅이 어려움
 - 필요 없는 로그 제거
 - Atheros Eclair Driver Patch 파일에 Port-bridge와 DSC에서 Message 막는 구문이 추가 되었음
   해당 Patch후 Message가 막혀있는지 조사하여 적용 유무 판단
   => wifi 로그 나오지 않음. 
*/

/*
#define FEATURE_SKT_DS_VT_EMULATOR
- VT 지원 단말의 경우, hidden 메뉴에서 External 3G-324M을 지원하기 위해 수정함.
- 모뎀 쪽은 FEATURE_SKY_DS_VT_EMULATOR가 define 되어야 함.
*/

#endif /* T_SKY_MODEL_TARGET_WCDMA */

#endif /* __CUST_SKY_DS_H__ */
