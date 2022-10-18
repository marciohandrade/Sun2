#pragma once

#include <SunLog.h>
#include <NetworkObject.h>
#include <ServerConst.h>
#include <ServerStruct.h>

#include <PacketStruct_ServerCommon.h>

class SecureLog : public CSunLog
{
public:
	SecureLog() : CSunLog() { m_pSession = NULL; }
	~SecureLog();

public:
	VOID			RegisterSession( NetworkObject* pSession ) { m_pSession = pSession; }
	NetworkObject*	GetRegisteredSession() { return m_pSession; }

public:
	virtual BOOL	Init(
						const BYTE byLogOption,
						const BYTE byLogLevel,
						const TCHAR* pszFilePath,
						const TCHAR* pszFileName,
						BOOL bUseSolarLog = TRUE
						);

	VOID			SetUserID( const TCHAR* pUserID )
	{
		if( pUserID )
			_tcsncpy( m_APPEND_LOG_INFO.m_pUserID, pUserID, sizeof(m_APPEND_LOG_INFO.m_pUserID) );
		else
			m_APPEND_LOG_INFO.m_pUserID[0] = _T('\0');
	}
	VOID			SetCharName( const TCHAR* pCharName )
	{
		if( pCharName )
			_tcsncpy( m_APPEND_LOG_INFO.m_pCharName, pCharName, sizeof(m_APPEND_LOG_INFO.m_pCharName) );
		else
			m_APPEND_LOG_INFO.m_pCharName[0] = _T('\0');
	}
	VOID			SetConnectedIP( const DWORD dwIP ) { m_APPEND_LOG_INFO.m_IP = dwIP; }
	VOID			SetUserGUID( const DWORD dwUserGUID ) { m_APPEND_LOG_INFO.m_dwUserGUID = dwUserGUID; }
	VOID			SetCharGUID( const DWORD dwCharGUID ) { m_APPEND_LOG_INFO.m_dwCharGUID = dwCharGUID; }

	VOID			SetAbuseType( const eABUSE_TYPE AbuseType )				{ m_APPEND_LOG_INFO.m_AbuseType = BYTE(AbuseType); }
	VOID			SetAbuseSubType( const eABUSE_SUB_TYPE AbuseSubType )	{ m_APPEND_LOG_INFO.m_AbuseSubType = BYTE(AbuseSubType); }
public:
	static BOOL WriteMessage_Network( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist );

private:
	VOID			GetUserID( TCHAR* OUT rpUserID )
	{
		if( !rpUserID || m_APPEND_LOG_INFO.m_pUserID[0] == _T('\0') )
			return;

		_tcsncpy( rpUserID, m_APPEND_LOG_INFO.m_pUserID, (DWORD)(sizeof(m_APPEND_LOG_INFO.m_pUserID)/sizeof(TCHAR)) );
	}
	VOID			GetCharName( TCHAR* OUT rpCharName )
	{
		if( !rpCharName || m_APPEND_LOG_INFO.m_pCharName[0] == _T('\0') )
			return;

		_tcsncpy( rpCharName, m_APPEND_LOG_INFO.m_pCharName, (DWORD)(sizeof(m_APPEND_LOG_INFO.m_pCharName)/sizeof(TCHAR)) );
	}
	VOID			GetConnectedIP( DWORD& rdwConnectedIP ) { rdwConnectedIP = m_APPEND_LOG_INFO.m_IP; }
	DWORD			GetUserGUID()		{ return m_APPEND_LOG_INFO.m_dwUserGUID; }
	DWORD			GetCharGUID()		{ return m_APPEND_LOG_INFO.m_dwCharGUID; }

	eABUSE_TYPE		GetAbuseType()		{ return (eABUSE_TYPE)m_APPEND_LOG_INFO.m_AbuseType; }
	eABUSE_SUB_TYPE	GetAbuseSubType()	{ return (eABUSE_SUB_TYPE)m_APPEND_LOG_INFO.m_AbuseSubType; }

private:
	NetworkObject*		m_pSession;

	MSG_SERVERCOMMON_SECURE_LOG_NTF	m_APPEND_LOG_INFO;
};








