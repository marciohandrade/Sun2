#include "stdafx.h"
#include "KISSApiWrapper.h"
#include <tchar.h>
#include <stdlib.h>
#include <strsafe.h>


#ifndef _M_X64
	#ifdef _DEBUG
		#define KISS_API_MODULE_NAME		_T("KISSApi_x86d.dll")
	#else
		#define KISS_API_MODULE_NAME		_T("KISSApi_x86.dll")
	#endif	// #ifdef _DEBUG
#else
	#ifdef _DEBUG
		#define KISS_API_MODULE_NAME		_T("KISSApi_x64d.dll")
	#else
		#define KISS_API_MODULE_NAME		_T("KISSApi_x64.dll")
	#endif	// #ifdef _DEBUG
#endif	// #ifndef _M_X64


CKISSApiWrapper::CKISSApiWrapper()
	: m_hLib(NULL)
	, m_pMainHandle(NULL)
	, m_fpGetHandle(NULL)
{
}

CKISSApiWrapper::~CKISSApiWrapper()
{
	Close();
}

DWORD CKISSApiWrapper::LoadModule(LPCTSTR _lpszPath/* = NULL*/)
{
	if(NULL != m_hLib)
		return ERROR_SUCCESS;

	TCHAR szLibPath[MAX_PATH * 2] = {0,};

	if(NULL != _lpszPath)
		StringCchPrintf(szLibPath, MAX_PATH * 2, _T("%s\\%s"), _lpszPath, KISS_API_MODULE_NAME);
	else
		StringCchPrintf(szLibPath, MAX_PATH * 2, _T("%s"), KISS_API_MODULE_NAME);

	m_hLib = ::LoadLibrary(szLibPath);

	if(NULL == m_hLib)
		return ::GetLastError();

	m_fpGetHandle = (LPFN_GET_HANDLE)::GetProcAddress(m_hLib, "GetHandle");

	if(NULL == m_fpGetHandle)
	{
		DWORD dwErr = ::GetLastError();

		UnloadModule();

		return dwErr;
	}

	m_pMainHandle = m_fpGetHandle();

	return ERROR_SUCCESS;
}

void CKISSApiWrapper::UnloadModule()
{
	if(NULL != m_hLib)
	{
		::FreeLibrary(m_hLib);

		m_hLib			= NULL;
		m_pMainHandle	= NULL;
		m_fpGetHandle	= NULL;
	}
}

UINT CKISSApiWrapper::Open(LPCTSTR _lpszPath/* = NULL*/)
{
	DWORD dwRet = LoadModule(_lpszPath);

	if(ERROR_SUCCESS != dwRet)
		return dwRet;

	if( NULL == m_pMainHandle			||
		NULL == m_pMainHandle->lpfnOpen	)
	{
		return ERROR_INVALID_HANDLE;
	}

	return m_pMainHandle->lpfnOpen();
}

void CKISSApiWrapper::Close()
{
	if(NULL != m_pMainHandle)
	{
		if(NULL != m_pMainHandle->lpfnClose )
			m_pMainHandle->lpfnClose();
	}

	UnloadModule();
}

bool CKISSApiWrapper::IsOpened() const
{
	if( NULL == m_pMainHandle				||
		NULL == m_pMainHandle->lpfnIsOpened	)
	{
		return false;
	}

	return m_pMainHandle->lpfnIsOpened();
}

DWORD CKISSApiWrapper::NotifyNumber(const LPNOTIFY_INFO_NUM _lpInfo)
{
	if( NULL == m_pMainHandle					||
		NULL == m_pMainHandle->lpfnNotifyNumber	)
	{
		return ERROR_INVALID_HANDLE;
	}

	return m_pMainHandle->lpfnNotifyNumber(_lpInfo);
}

DWORD CKISSApiWrapper::NotifyStringA(const LPNOTIFY_INFO_STRA _lpInfo)
{
	if( NULL == m_pMainHandle					 ||
		NULL == m_pMainHandle->lpfnNotifyStringW )
	{
		return ERROR_INVALID_HANDLE;
	}

	NOTIFY_INFO_STRW info;
	ConvertNotifyInfoStringAtoW(_lpInfo, &info);

	return m_pMainHandle->lpfnNotifyStringW(&info);
}

DWORD CKISSApiWrapper::NotifyStringW(const LPNOTIFY_INFO_STRW _lpInfo)
{
	if( NULL == m_pMainHandle					 ||
		NULL == m_pMainHandle->lpfnNotifyStringW )
	{
		return ERROR_INVALID_HANDLE;
	}

	return m_pMainHandle->lpfnNotifyStringW(_lpInfo);
}

DWORD CKISSApiWrapper::NotifyHeartbeat()
{
	if( NULL == m_pMainHandle					||
		NULL == m_pMainHandle->lpfnNotifyNumber	)
	{
		return ERROR_INVALID_HANDLE;
	}

	NOTIFY_INFO_NUM info;
	InitNotifyInfoNumber(&info, KAC_HEART_BEAT, 0);

	return m_pMainHandle->lpfnNotifyNumber(&info);
}

DWORD CKISSApiWrapper::NotifyConcurrentUserCount(const DWORD _dwPortNo, const float _fVal)
{
	if( NULL == m_pMainHandle					||
		NULL == m_pMainHandle->lpfnNotifyNumber	)
	{
		return ERROR_INVALID_HANDLE;
	}

	NOTIFY_INFO_NUM info;
	InitNotifyInfoNumber(&info, KAC_CONC_USER_COUNT, _fVal,MEANINGLESS_VALUE,MEANINGLESS_VALUE,_dwPortNo);

	return m_pMainHandle->lpfnNotifyNumber(&info);
}

DWORD CKISSApiWrapper::GetNumber(LPNOTIFY_INFO_NUM _lpInfo)
{
	if( NULL == m_pMainHandle				 ||
		NULL == m_pMainHandle->lpfnGetNumber )
	{
		return ERROR_INVALID_HANDLE;
	}

	return m_pMainHandle->lpfnGetNumber(_lpInfo);
}

DWORD CKISSApiWrapper::GetStringW(LPNOTIFY_INFO_STRW _lpInfo)
{
	if( NULL == m_pMainHandle					||
		NULL == m_pMainHandle->lpfnGetStringW	)
	{
		return ERROR_INVALID_HANDLE;
	}

	return m_pMainHandle->lpfnGetStringW(_lpInfo);
}

void InitNotifyInfoNumber(LPNOTIFY_INFO_NUM _lpInfo, const int _nActivityId/* = 0*/,
						  const float _fValue/* = MEANINGLESS_VALUE*/, const float _fMin/* = MEANINGLESS_VALUE*/, const float _fMax/* = MEANINGLESS_VALUE*/ ,
						  const DWORD _dwPortNo /*= 0*/ )
{
	if(NULL == _lpInfo)
		return;

	::GetLocalTime(&_lpInfo->registered_time);

	_lpInfo->activity_id	= _nActivityId;
	_lpInfo->max_val		= _fMax;
	_lpInfo->min_val		= _fMin;
	_lpInfo->val			= _fValue;
	_lpInfo->port_no		= _dwPortNo;
}

void InitNotifyInfoStringA(LPNOTIFY_INFO_STRA _lpInfo, const int _nActivityId/* = 0*/, LPCSTR _lpszString/* = ""*/)
{
	if(NULL == _lpInfo)
		return;

	::GetLocalTime(&_lpInfo->registered_time);

	_lpInfo->activity_id = _nActivityId;

	StringCchCopyA(_lpInfo->content, sizeof(_lpInfo->content), _lpszString);
}

void InitNotifyInfoStringW(LPNOTIFY_INFO_STRW _lpInfo, const int _nActivityId/* = 0*/, LPCWSTR _lpszString/* = L""*/)
{
	if(NULL == _lpInfo)
		return;

	::GetLocalTime(&_lpInfo->registered_time);

	_lpInfo->activity_id = _nActivityId;

	StringCchCopyW(_lpInfo->content, sizeof(_lpInfo->content), _lpszString);
}

void ConvertNotifyInfoStringAtoW(const LPNOTIFY_INFO_STRA _src, LPNOTIFY_INFO_STRW _dst)
{
	if(NULL == _src || NULL == _dst)
		return;

	_dst->registered_time	= _src->registered_time;
	_dst->activity_id		= _src->activity_id;

	ZeroMemory(_dst->content, sizeof(_dst->content));
	::MultiByteToWideChar(CP_ACP, 0, _src->content, -1, _dst->content, _countof(_dst->content));
}

LPMAIN_HANDLE GetMainHandle(CKISSApiWrapper* pAPIWrapper)
{
	if(NULL == pAPIWrapper)
		return NULL ;

	return pAPIWrapper->m_pMainHandle ;
}