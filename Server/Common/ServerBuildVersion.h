/************************************************************************/
/* ServerBuildVersion.h : Server간의 빌드 버전
/************************************************************************/

#ifndef	__SERVER_BUILD_VERSION_H__
#define	__SERVER_BUILD_VERSION_H__

#pragma once

#include <Version.h>

// (WAVERI) (090401) (LOGIC-CHANGE) 빌드시마다 전체 빌드는 아무래도 힘들다.
// 간이 빌드를 하려고 할 때는 빌드 버전 못올리고 빌드하는 경우도 비일비재,
// 따라서 해더에 있는 내용을 일부 코드로 옮긴다.
// NEW ADDED : ServerBuildVersion.cpp

// Server에서 릴리즈 됐을 때 올리는 버전
struct SERVER_VERSION_INFO
{
    static DWORD BASE_NUMBER();
    static DWORD RELEASE_BUILD_NUMBER();
};

#define	SERVER_RELEASE_BUILD_NO     SERVER_VERSION_INFO::RELEASE_BUILD_NUMBER()

// Server <-> Server간 버전 관리
#define S2S_HIGH_VERSION_NO		( 1 )			// 클베 1차, 클베 2차, 오픈베타, 상용화, 대규모 업데이트(공성전) 등등
#define S2S_MIDDLE_VERSION_NO	( 4 )			// ODBC 변경, 서버간 프로토콜 추가, 변경 등등 => 반드시 패치 후 접속이 필요한 경우
#define	S2S_LOW_VERSION_NO		( 0 )			// 버그 수정, 스크립트 업데이트(개발시에만-> 차후 중간버전번호) 등등


// S/S Version History
//------------------------------------------------------------------------------------------------------------------
// 2006-01-12(목) Ver 1.3.5 : WOPS와 연동해서 서버 자동 시작 및 자동 종료 처리
// 2005-12-23(금) Ver 1.3.4 : 길드서버 추가
// 2005-11-18(금) Ver 1.3.3 : 마을채팅 월드서버에서 전체 브로드캐스팅 처리
// 2005-10-24(월) Ver 1.3.2 : 클로즈 베타 2차 시작버전
// 2005-10-21(금) Ver 1.3.1 : WOPS Agent와 연동 처리
// 2005-09-28(수) Ver 1.2.3 : AgentServer와 WorldServer연결, 서버간 프로토콜 수정
// 2005-09-28(수) Ver 1.2.2 : 서버간 패킷 전송시 데이터 무결성 체크(스트링 NULL 체크)
// 2005-09-25(일) Ver 1.2.1 : Agent <-> DBP 프로토콜 변경
// 2005-09-02(토) Ver 1.1.1 : 버전 체크 시작
//------------------------------------------------------------------------------------------------------------------


#endif //__SERVER_BUILD_VERSION_H__