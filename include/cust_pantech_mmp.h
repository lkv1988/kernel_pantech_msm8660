#ifndef __CUST_PANTECH_MMP_H__
#define __CUST_PANTECH_MMP_H__

/*
  2011/03/02 권오윤
  PANTECH multimedia 개발 관련 공통 최상위 feature.
  세부 feature를 정의하기 어려운 부분 또는 multimedia관련 소스가 아닌 부분을
  수정하는 경우 사용. (make file 등인 경우 #주석 부분에 추가)
*/
#define FEATURE_PANTECH_MMP

/*
  2011/03/02 권오윤
  VisuialOn VOME engine관련 /external/vome/... 외의 부분을 수정하거나
  VisualOn 소스를 일부 수정해서 사용하는 부분에 대해 feature작업
*/
#define FEATURE_PANTECH_MMP_VOME
/*****************************************************************/
/*
   2012/04/03 Heekyoung Seo.
   VisualOn Engine Enable Features..
*/
#define FEATURE_PANTECH_MMP_VOME_AVI
#define FEATURE_PANTECH_MMP_VOME_ASF
#define FEATURE_PANTECH_MMP_VOME_MKV
/*#define FEATURE_PANTECH_MMP_VOME_FLAC // Using Stagefrightplayer */
#define FEATURE_PANTECH_MMP_VOME_QTIME

/*#define FEATURE_PANTECH_MMP_VOME_RTSP // We don't use this feature, now.
RTSP Engine is hard coded by apk team in MediaPlayerService.cpp*/
#define FEATURE_PANTECH_MMP_VOME_DLNA

#if defined(T_STARQ)
#define FEATURE_PANTECH_MMP_VOME_ADPCM_WAV
#endif
/******************************************************************/

/*we use voplayer for camera shutter sound ogg file
Using SF, there is memory curruption for stability test */
#define FEATURE_PANTECH_MMP_VOME_CAMERA_OGG

/* 
  2011/03/10 최병주
  QualComm Patch 적용한 부분에 대한 frature 작업
*/
#define FEATURE_PANTECH_MMP_QCOM_CR

/* 
  2011/03/11 권오윤
  StageFright 관련 수정하는 부분들을 찾기 쉽게 하기 위해 사용
*/
#define FEATURE_PANTECH_MMP_STAGEFRIGHT
 
/* 
  2011/03/11 권오윤
  OpenCORE 관련 수정하는 부분들을 찾기 쉽게 하기 위해 사용
*/
//#define FEATURE_PANTECH_MMP_OPENCORE//ICS_Update LYJ_20120228

/*
  2011/04/29 Heekyoung Seo
  Add WMA S/W Decoder and code to use VC-1 H/W Decoder with Stagefright for 
  SKT HOPPIN Service.
  If Don't need HOPPIN Service, it also need to modify 
  frameworks/base/media/libstagefright/Android.mk. (remove 
  BUILD_WITH_WMA_SW_DECODER:=true)
  */
#define FEATURE_PANTECH_MMP_HOPPIN

// p12455 ++ (for Hoppin)
#define FEATURE_FRAMEWORK_HOPPIN
// p12455 -- (for Hoppin)


#define FEATURE_WIDEVINE_VISUALON

/* 
  2011/04/16 이선중
  MP4 QuickTime(ftypqt) 포맷이 SF에서 지원되지 않아 PV로 변경 사용(KakaoTok issue)
*/
#define FEATURE_PANTECH_MMP_QUICKTIME

/*
  2011/05/13 Heekyoung Seo
  Add Qualcomm VC-1 Patch.
  Fis timeStamp order is up so down with a few WMV8 streams.
  */
#define FEATURE_PANTECH_MMP_VC1_DEC_PATCH


/*
 2011/06/26 최병주

 TestSBA_M8260AAABQNLZA3040_Pantech_EF33S-EF34K_05252011_Case00518255

 Qcom H/W Dec을 사용하여 XivD 파일 재생시 화면이 일그러지는 문제를 해결하기 위한 Test SBA
 
 ( simple profile B-frame )
 */
#define FEATURE_PANTECH_MMP_XVID_QCOM_HWDEC_SBA

/*
 2011/07/12 최병주

 SBA_M8660AAABQNLYA109020_Pantech_EF33S_07122011_Case00522374_00522374

 */
#define FEATURE_PANTECH_MMP_QCOM_SBA_TIMESTAMP

/*
  2011/mm/dd who
  ...description...
*/
#define FEATURE_PANTECH_MMP_xxx

/*
  2011/mm/dd who
  ...description...
*/
#define FEATURE_PANTECH_MMP_zzz

#define FEATURE_SKYSND_LPA  // for QSound LPA //temp

#endif/* __CUST_PANTECH_MMP_H__ */
