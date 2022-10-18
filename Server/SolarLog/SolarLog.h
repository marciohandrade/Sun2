#ifndef __SOLAR_LOG_H__
#define __SOLAR_LOG_H__

#pragma once

#ifdef SOLARLOG_EXPORTS
#define SOLARLOG_API __declspec(dllexport)
#else
#define SOLARLOG_API __declspec(dllimport)
#endif

//=======================================================================================================================
/// Overlapped ������� Log�� FileWrite �� �� �ִ� �α� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 11 ( ������ Day ^^ )
	@remarks
			- DLL�� ����
			- Overlapped File I/O ���
			- File I/O Worker �����带 ���� �ξ� File��Ͻ� �� �Ǵ� ������� �ٿ���.
			- ��¥�� �ٲ�� ���ο� ������ �����ؼ� ����Ѵ�.
			- SolarLog �ν��Ͻ����� �ϳ��� ������ �����ȴ�.
	@note
			- Ȯ���ڴ� wzl�� �����Ǹ� pszFileNamePrefix__Date_20041123.wzl �������� ������ �����ȴ�.
	@todo
			- ��ȣȭ�� �������� �����ϴ� ��带 �߰��Ѵ�.
			- 2007.5.8  : ���� �α� ť�� : thread-unsafe
	@history
			- 2005.2.18 : Error Message Handler Function �߰�
			- 2005.11.16: Ringbuffer ��ü �� ���۸��� �α׸� ������ ����ҽÿ� ������ �����ϵ��� ����
			- 2005.11.23: TCHAR�� ��ü, LOGxx()�Լ��� BOOL ���ϰ� �߰�
			- 2005.11.24: ���ϳ��� Append�ǰ� ó��
			- 2007.5.8  : internal �۾� ó�� : header ����
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