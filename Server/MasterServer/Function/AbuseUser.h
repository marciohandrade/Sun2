#pragma once

#include "AbusingOptionParser.h"

#define POINT_CLEAR_TIME		2*24*60*60*1000

#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
const DWORD	AU_LEN_MAX_ID = ID_MAX_LEN + 1;
#else
static const DWORD	AU_LEN_MAX_ID = ((ID_MAX_LEN>>1)+1);
#endif
static const DWORD	AU_LEN_MAX_CHARNAME = (MAX_CHARNAME_LENGTH+1);

class AbusePointInfo
{
public:
	VOID	Init();

	ABUSING_OPTION_INFO * GetSharedAbusingOptionInfo()						{ return m_pSharedAbusingOptionInfo; }
	VOID	SetSharedAbusingOptionInfo( ABUSING_OPTION_INFO *pOtionInfo )	{ m_pSharedAbusingOptionInfo = pOtionInfo; }

	BOOL	IncViolationCount();
	VOID	InitAbusingPoint()		{ m_AbusingPoint = 0; }

	WORD	GetViolationCount()		{ return m_ViolationCount; }
	WORD	GetAbusingPoint()		{ return m_AbusingPoint; }

private:
	ABUSING_OPTION_INFO *	m_pSharedAbusingOptionInfo;
	WORD					m_AbusingPoint;		// 최대 한도가 되기 전까지 일정 포인트씩 계속 누적되며 Disconnect 후에는 다시 초기화된다.
	WORD					m_ViolationCount;	// 위반회수를 의미하며 Disconnect 후에도 계속 누적된다
};

class AbuseUser
{
public:
	AbuseUser();
	~AbuseUser() {}

	VOID				Init( DWORD UserGuid, TCHAR * pszUserID );
	VOID				Update();

	BOOL				UpdateAbuseInfo( eABUSE_SUB_TYPE AbuseSubType, SERVER_KEY & ServerKey, DWORD ConnectedIP, DWORD CharGuid, TCHAR *pszCharName );
	VOID				NotifyViolation( eABUSE_SUB_TYPE AbuseSubType, TCHAR *pMessage );
	VOID				ForcedDisconnect( BOOL bNotifyToWops = TRUE );
	VOID				TemporaryBlock();

	inline DWORD		GetUserGUID()			{ return m_UserGUID; }
	inline DWORD		GetCharGUID()			{ return m_CharGUID; }
	inline TCHAR *		GetUserID()				{ return m_pszUserID; }
	inline DWORD		GetConnectedIP()		{ return m_ConnectedIP; }
	inline DWORD		GetDisconnectCount()	{ return m_DisconnectCount; }
	WORD				GetTotAbusingPoint();

	BOOL				IsBlock();

private:
	DWORD				m_UserGUID;
	TCHAR				m_pszUserID[AU_LEN_MAX_ID];

	SERVER_KEY			m_ServerKey;
	DWORD				m_ConnectedIP;
	DWORD				m_CharGUID;
	TCHAR				m_pszCharName[AU_LEN_MAX_CHARNAME];

	ITimeout			m_PointClearTimer;

	DWORD				m_DisconnectCount;
	AbusePointInfo		m_AbusePointInfo[eABUSE_SUB_MAX];
};



