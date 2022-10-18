#ifndef __SOLAR_LOG_H__
#define __SOLAR_LOG_H__

#pragma once

#ifdef SOLARLOG_EXPORTS
#define SOLARLOG_API __declspec(dllexport)
#else
#define SOLARLOG_API __declspec(dllimport)
#endif

//=======================================================================================================================
/// Overlapped 방식으로 Log를 FileWrite 할 수 있는 로그 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 11 ( 빼빼로 Day ^^ )
	@remarks
			- DLL로 제공
			- Overlapped File I/O 방식
			- File I/O Worker 스레드를 따로 두어 File기록시 블럭 되는 오버헤드 줄였다.
			- 날짜가 바뀌면 새로운 파일을 생성해서 기록한다.
			- SolarLog 인스턴스마다 하나의 스레드 생성된다.
	@note
			- 확장자는 wzl로 생성되며 pszFileNamePrefix__Date_20041123.wzl 형식으로 파일이 생성된다.
	@todo
			- 암호화된 형식으로 저장하는 모드를 추가한다.
			- 2007.5.8  : 에러 로그 큐잉 : thread-unsafe
	@history
			- 2005.2.18 : Error Message Handler Function 추가
			- 2005.11.16: Ringbuffer 교체 및 버퍼링된 로그를 완전히 기록할시에 스레드 종료하도록 수정
			- 2005.11.23: TCHAR로 교체, LOGxx()함수에 BOOL 리턴값 추가
			- 2005.11.24: 파일끝에 Append되게 처리
			- 2007.5.8  : internal 작업 처리 : header 줄임
*/
//=======================================================================================================================

typedef void (*fnCallBackError) ( TCHAR * pszErrorMsg );

class SOLARLOG_API SolarLog 
{
	friend class LogWorkerThread;
public:
	SolarLog();
	~SolarLog();
	BOOL								Create( TCHAR * pszDirName, TCHAR * pszFileNamePrefix );
	BOOL								LOG( BYTE * pBuffer, DWORD dwSize );
	BOOL								LOG( TCHAR * pszMessage );
	BOOL								LOG_Arg( TCHAR * pszMessage, ... );
	BOOL								LOG_Date( TCHAR * pszMessage );

	VOID								SetErrorHandler( fnCallBackError fnFunc );

private:
	class Internal;
	Internal *							_IN;

};


#endif // __SOLAR_LOG_H__