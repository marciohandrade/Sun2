/**
 * @brief HanReportForClient.h
 * This file declares HanReportForClient api
 * 
 * Copyright (c) 2008 NHN Corporation. All Rights Reserved.
 *
 * @author	: nhn
 */

#ifndef __HanReportForClient__
#define __HanReportForClient__

#ifdef HANREPORTFORCLIENT_EXPORTS
#define HANREPORT_API __declspec(dllexport)
#else
#define HANREPORT_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "HanReportForClientD.lib") 
#else
#pragma comment(lib, "HanReportForClient.lib") 
#endif
#endif

#define SERVICE_NATION  0x000000FF
#define SERVICE_KOR		0x00000001
#define SERVICE_USA		0x00000002
#define SERVICE_JPN		0x00000003
#define SERVICE_CHN		0x00000004

#define SERVICE_TYPE    0x00000F00
#define SERVICE_ALPHA	0x00000100
#define SERVICE_REAL	0x00000200
#define SERVICE_BETA	0x00000300

#define SERVICE_SITE	0x0000F000
#define SERVICE_HANGAME 0x00001000
#define SERVICE_ASOBLOG 0x00002000

/*
Return value Info:
0 : Ok
- : fault from  function / this system 
    -1 : general fault.
*/

#define HAN_REPORTCLI_OK								0
#define HAN_REPORTCLI_ARGUMENT_INVALID					-1
#define HAN_REPORTCLI_INITED_ALREADY					-2
#define HAN_REPORTCLI_INITED_NOT						-3
#define HAN_REPORTCLI_INITED_FAIL						-4
#define HAN_REPORTCLI_GAMESTRING_IDINVALID				-14
#define HAN_REPORTCLI_GAMESTRING_USNINVALID				-15
#define HAN_REPORTCLI_GAMESTRING_GAMEIDINVALID			-16
#define HAN_REPORTCLI_GAMESTRING_SERVICECODEINVALID		-17
#define HAN_REPORTCLI_SERVICECODEINVALID				-20
#define HAN_REPORTCLI_PL2FILE_INVALID					-21

#define HAN_REPORTCLI_DLL_UNEXPECTED					-100

#define HAN_REPORTCLI_PARAM_INVALID						1
#define HAN_REPORTCLI_IPADDR_INVALID					2
#define HAN_REPORTCLI_SYSTEM_ERROR						7


HANREPORT_API int __stdcall HanReportInit(/*IN*/ char* szGameId,
										  /*IN*/ int   nServiceCode,
										  /*IN*/ bool  bSendDump=true
										  );

HANREPORT_API int __stdcall HanReportInitGameString(/*IN*/ char* szGameString, /*IN*/bool bSendDump=true);

HANREPORT_API int __stdcall HanReportSendPosLog(/*IN*/ char* szReportString);

HANREPORT_API int __stdcall HanReportSendPosLogSync(/*IN*/ char* szReportString);


HANREPORT_API int __stdcall HanReportSend(/*IN*/ char* szSubject,
										  /*IN*/ char* szReportString);


HANREPORT_API int __stdcall HanReportConvertMD5(/*IN*/ char* szUserId,/*OUT*/ char* szMD5USerId);

HANREPORT_API int __stdcall HanReportGetNationCode(/*IN*/ char *szGameString,
												   /*OUT*/int *pNationCode);

HANREPORT_API int __stdcall HanReportGetServiceTypeCode(/*IN*/ char *szGameString,
														/*OUT*/int *pServiceTypeCode);

HANREPORT_API int __stdcall HanReportGetSiteCode(/*IN*/ char *szGameString,
												 /*OUT*/int *pSiteCode);

void ConvertHexToString(char*  szMD5String, unsigned char* szHex, int nHex );





// 호환성을 위해 남겨둠, 사용하지 않음
HANREPORT_API int __stdcall HanReportInitWithDump(/*IN*/ char* szGameId, /*IN*/ int nServiceCode );

// 호환성을 위해 남겨둠, 사용하지 않음
HANREPORT_API int __stdcall HanReportInitGameStringWithDump(/*IN*/ char* szGameString);

#endif