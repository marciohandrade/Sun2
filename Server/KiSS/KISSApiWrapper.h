#pragma once

#include <Windows.h>
#include "KISSApi.h"


class CKISSApiWrapper
{
public:
	CKISSApiWrapper();
	~CKISSApiWrapper();

private:
	HMODULE				m_hLib;
	LPMAIN_HANDLE		m_pMainHandle;

	LPFN_GET_HANDLE		m_fpGetHandle;

public:
	UINT		Open(LPCTSTR _lpszPath = NULL);
	void		Close();

	bool		IsOpened() const;

	DWORD		NotifyNumber(const LPNOTIFY_INFO_NUM _lpInfo);
	DWORD		NotifyStringA(const LPNOTIFY_INFO_STRA _lpInfo);
	DWORD		NotifyStringW(const LPNOTIFY_INFO_STRW _lpInfo);
	DWORD		NotifyHeartbeat();
	DWORD		NotifyConcurrentUserCount(const DWORD _dwPortNo, const float _fVal);

	DWORD		GetNumber(LPNOTIFY_INFO_NUM _lpInfo);
	DWORD		GetStringW(LPNOTIFY_INFO_STRW _lpInfo);

private:
	DWORD		LoadModule(LPCTSTR _lpszPath = NULL);
	void		UnloadModule();

	CKISSApiWrapper(const CKISSApiWrapper& _rhs)				{ UNREFERENCED_PARAMETER(_rhs); }
	CKISSApiWrapper& operator=(const CKISSApiWrapper& _rhs)		{ UNREFERENCED_PARAMETER(_rhs); return *this; }

	friend LPMAIN_HANDLE GetMainHandle(CKISSApiWrapper* pAPIWrapper) ;

};

void	InitNotifyInfoNumber(	LPNOTIFY_INFO_NUM _lpInfo, const int _nActivityId = 0,
								const float _fValue = MEANINGLESS_VALUE, const float _fMin = MEANINGLESS_VALUE, const float _fMax = MEANINGLESS_VALUE, const DWORD _dwPortNo = 0 );
void	InitNotifyInfoStringA(	LPNOTIFY_INFO_STRA _lpInfo,	const int _nActivityId = 0, LPCSTR _lpszString = "" );
void	InitNotifyInfoStringW(	LPNOTIFY_INFO_STRW _lpInfo, const int _nActivityId = 0, LPCWSTR _lpszString = L"" );
void	ConvertNotifyInfoStringAtoW(const LPNOTIFY_INFO_STRA _lpInfoSrc, LPNOTIFY_INFO_STRW _lpInfoDst);

LPMAIN_HANDLE GetMainHandle(CKISSApiWrapper* pAPIWrapper) ;