#pragma once

/********************************************************************
	created:	2010/11/23
	created:	23:11:2010   11:56
	filename: 	d:\Svn\trunk\Server\Common\Pms\PMSConn.h
	file path:	d:\Svn\trunk\Server\Common\Pms
	file base:	PMSConn
	file ext:	h
	author:		youngmoon
	
	purpose:	PMS 모듈(DLL)을 사용하기 위한 Method 정의
*********************************************************************/

#ifdef _JP_0_20101123_PMS

#include "IPMSObject.h"
#include "PMSCodes.h"

#ifdef PMSCONN_EXPORTS
#define PMSCONN_API __declspec(dllexport)
#else
#define PMSCONN_API __declspec(dllimport)
#endif

/*
PMSInitConn : PMSConn 초기화를 하기 위한 함수, Console 모드일때는 main함수의 인자를 , Service Mode일때는 ServiceMain 함수의 인자를 전달
@param1 : 실행 인자 개수
@param2 : 실행인자의 포인터 배열
@return value : PMSCONN_ERRORCODE 참조
*/
extern PMSCONN_API	DWORD	PMSInitConn(DWORD argc, LPTSTR argv[]);
/*
PMSRunConn : PMSConn을 실제적으로 동작 시키는 함수
@param1 : Call Back을위한 IPMSObject 포인터 IPMSObject는 IPMSObject관련 설명 참조
@return value : PMSCONN_ERRORCODE 참조
*/
extern PMSCONN_API  DWORD	PMSRunConn(IPMSObject *pObj);
/*
PMSStopConn : PMSConn을 종료 하기 위한 함수
*/
extern PMSCONN_API  void	PMSStopConn();
/*
PMSSendWarningMsg : PMS로 Warning 메시지를 보내기 위한 함수
@param1 : 장애 레벨 FAULT_LEVEL 참조
@param2 : 장애 내용
@param3 : 장애 처리 방법
@param4 : 장애 발생 게임 Serial
@param5 : 장애 발생 카테고리
*/
extern PMSCONN_API	BOOL	PMSSendWarningMsg(DWORD dwErrLvl, LPCSTR pszWarningMsg, LPCSTR pszTreatMsg, DWORD dwSSN, DWORD dwCategory);

/*
PMSGetConfigFileName PMS로 넘겨진 성능 정보 Config 파일의 이름을 가져 오는 함수
@return value PMS인자중 /CNFGFILE:xxx.xxx 중 xxx.xxx값을 리턴 
*/
extern PMSCONN_API	LPCTSTR	PMSGetConfigFileName(); //사용하지 않음
/*
PMSGetStatus PMSConn의 현재 상태를 가져 오기 위한 함수
@return value : PMSCONN_STATUS_CODE 값을 리턴  
*/
extern PMSCONN_API	DWORD	PMSGetStatus();

#endif //_JP_0_20101123_PMS
