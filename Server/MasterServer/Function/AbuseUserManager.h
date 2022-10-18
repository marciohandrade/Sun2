#pragma once

#include <set>
#include <ITimeout.h>
#include <MemoryPoolFactory.h>
#include "AbuseUser.h"
#include <Singleton.h>


#define MAX_ABUSING_POINT				100
#define MAX_USER_DISCONNECT_COUNT		5

#ifdef _KOREA
	#define MAX_IP_DISCONNECT_COUNT		30
#else
	#define MAX_IP_DISCONNECT_COUNT		50
#endif

#define ALL_CLEAR_TIME			7*24*60*60*1000

class AbuseUserManager : public util::Singleton<AbuseUserManager>
{
	typedef STLX_HASH_MAP<DWORD, AbuseUser*>					ABUSE_USER_HASH;
	typedef ABUSE_USER_HASH::iterator					ABUSE_USER_HASH_IT;

	typedef STLX_SET<DWORD>								ABUSE_USERS_IN_IP_KEY;
	typedef STLX_HASH_MAP<DWORD, ABUSE_USERS_IN_IP_KEY>		ABUSE_USERS_IN_IP_HASH;
	typedef ABUSE_USERS_IN_IP_HASH::iterator			ABUSE_USERS_IN_IP_HASH_IT;

public:		
	AbuseUserManager();
	~AbuseUserManager();

	BOOL			Init();
	BOOL			Release();
	BOOL			Update();

	VOID			EnterServer( DWORD UserGuid, TCHAR * pszUserID, DWORD IP );
	VOID			OnMsg( sSIG_SECURE_LOG_NOTIFY* pNOTIFY );

	BOOL			IsIPBlock( DWORD IP );
	DWORD			GetUserCountForIP( DWORD IP );
	
	AbuseUser*		FindUser( DWORD UserGuid );
	VOID			RemoveUser( DWORD dwUserGUID );

	VOID			WriteFileLog( eABUSE_TYPE eAbuseType, eABUSE_SUB_TYPE eAbuseSubType, SERVER_KEY & rServerKey, TCHAR *pszUserID, TCHAR *pszCharName, TCHAR *pszUserIP, TCHAR *pszMessage );
	VOID			NotifyViolation( eABUSE_SUB_TYPE AbuseSubType, TCHAR *pMessage, SERVER_KEY & rServerKey, DWORD ConnectedIP, DWORD UserGuid=0, DWORD CharGuid=0, TCHAR *pszUserID="", TCHAR *pszCharName="" );

	VOID			DisplayAbuseInfo();

private:
	ABUSE_USER_HASH							m_AbuseUserHash;
	ABUSE_USERS_IN_IP_HASH					m_AbuseIPGroupHash;
	util::CMemoryPoolFactory<AbuseUser>		m_AbuseUserPool;

	ITimeout								m_AllClearTimer;		// 모든 정보를 초기화한다.
};

