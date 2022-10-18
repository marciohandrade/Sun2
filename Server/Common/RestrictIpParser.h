#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>


class RestrictIpParser : public util::Singleton<RestrictIpParser>, public IParser
{
public:
	RestrictIpParser(void);
	~RestrictIpParser(void);

public:
	VOID		Release();
	BOOL		Load( TCHAR* pszFileName, BOOL bReload = FALSE );

	BOOL		CheckRestrictIP( const TCHAR* pszIP );
	BYTE		IsUseRestrictIP(){ return m_byUseRestrictIP; }
	BYTE		GetBasicSts(){ return m_byBasicSts; }

private:
	DWORD		ConvertIP( const TCHAR* pszIP );
	BOOL		_Load( const TCHAR* pszFileName );

#ifdef __NA_000252_20070208_VERSION_GMCMD
	virtual BOOL			ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize );
	virtual	BOOL			_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload );
#else
	virtual BOOL			ILoad( SCRIPT_CODE_T scriptCode, BYTE* pSTREAM, DWORD dwSize );
#endif

private:
	TCHAR		m_pszFileName[MAX_PATH];
	DWORD		m_dwStartAllowIP;
	DWORD		m_dwEndAllowIP;
	BYTE		m_byUseRestrictIP;
	BYTE		m_byBasicSts;

};
