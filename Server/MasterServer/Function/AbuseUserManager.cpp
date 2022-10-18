#include "stdafx.h"
#include "AbuseUserManager.h"
#include <SolarDateTime.h>
#include <MasterServerEx.h>
#include <Sessions/ServerSessionManager.h>
#include <Sessions/OpServerSession.h>
#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_MO.h>
#include "AbusingOptionParser.h"


static AbuseUserManager* s_pAbuseUserManager = NULL;

AbuseUserManager::AbuseUserManager()
{
}

AbuseUserManager::~AbuseUserManager()
{
}

BOOL AbuseUserManager::Init()
{
	m_AllClearTimer.SetNextTimeoutByCurTickBased( ALL_CLEAR_TIME );
	m_AbuseUserPool.Initialize( 100, 100 );

	return TRUE;
}

BOOL AbuseUserManager::Release()
{
	m_AbuseUserHash.clear();
	m_AbuseUserPool.Release();
	m_AbuseIPGroupHash.clear();

	return TRUE;
}

BOOL AbuseUserManager::Update()
{
	// 주기적으로 모든 정보를 초기화한다.
	if( m_AllClearTimer.IsExpired() )
	{
		m_AllClearTimer.SetNextTimeoutByCurTickBased( ALL_CLEAR_TIME );
		
		FOREACH_CONTAINER( ABUSE_USER_HASH::value_type& rNode, m_AbuseUserHash, ABUSE_USER_HASH )
		{
			AbuseUser* pAbuseUser = rNode.second;
			m_AbuseUserPool.Free( pAbuseUser );
		}
		m_AbuseUserHash.clear();
		m_AbuseIPGroupHash.clear();
	}

	ABUSE_USER_HASH_IT it = m_AbuseUserHash.begin(), itend = m_AbuseUserHash.end();
	for( ; it != itend; ++it )
	{
		AbuseUser* pAbuseUser = it->second;
		pAbuseUser->Update();
	}

	return TRUE;
}

VOID AbuseUserManager::EnterServer( DWORD UserGuid, TCHAR * pszUserID, DWORD IP )
{
	AbuseUser *pAbuseUser = FindUser( UserGuid );

	// 해당 IP가 블록상태인지 체크한다.
	if( IsIPBlock( IP ) )
	{
		if( !pAbuseUser )
		{
			pAbuseUser = (AbuseUser*)m_AbuseUserPool.Alloc();
			pAbuseUser->Init( UserGuid, pszUserID );
			m_AbuseUserHash[UserGuid] = pAbuseUser;
		}
		pAbuseUser->ForcedDisconnect( FALSE );
		return;
	}

	// 해당 유저가 블록상태인지 체크한다.
	if( pAbuseUser && pAbuseUser->IsBlock() )
	{
		pAbuseUser->ForcedDisconnect( FALSE );
	}
}

VOID AbuseUserManager::OnMsg( sSIG_SECURE_LOG_NOTIFY* pNOTIFY )
{
	MSG_SERVERCOMMON_SECURE_LOG_NTF* pMsgNTF = pNOTIFY->m_pRecvNTF;
	
	ABUSING_OPTION_INFO *pAbuseOptionInfo = AbusingOptionParser::Instance()->GetAbusingOptionInfo( pMsgNTF->m_AbuseSubType );
	if( !pAbuseOptionInfo )
	{
		SUNLOG( eCRITICAL_LOG, "[AbuseUserManager::OnMsg] Invalid AbuseSubType(%d)", pMsgNTF->m_AbuseSubType );
		return;
	}

	AbuseUser* pAbuseUser = FindUser( pMsgNTF->m_dwUserGUID );

	// 최초 어뷰징이면 유저를 생성한다.
	if( !pAbuseUser )
	{
		pAbuseUser = (AbuseUser*)m_AbuseUserPool.Alloc();
		pAbuseUser->Init( pMsgNTF->m_dwUserGUID, pMsgNTF->m_pUserID );

		m_AbuseUserHash[pMsgNTF->m_dwUserGUID] = pAbuseUser;
	}

	// 해당 IP, 유저가 이미 블록상태일 경우 더이상의 로그를 남기지 않는다.
	if( IsIPBlock( pMsgNTF->m_IP ) || pAbuseUser->IsBlock() )
	{
		return;
	}

	// 해당 어뷰징 정보를 누적하고 Milestone에 도달하면 Wops에 보고한다.
	if( pAbuseUser->UpdateAbuseInfo( (eABUSE_SUB_TYPE)pMsgNTF->m_AbuseSubType, pNOTIFY->m_ServerKey, pMsgNTF->m_IP, pMsgNTF->m_dwCharGUID, pMsgNTF->m_pCharName ) )
	{
		pAbuseUser->NotifyViolation( (eABUSE_SUB_TYPE)pMsgNTF->m_AbuseSubType, pNOTIFY->m_pMessage );
	}

	// 어뷰징포인트가 MAX_ABUSING_POINT를 넘어가면 Disconnect 처리를 한후 Wops에 보고한다.
	if( pAbuseUser->GetTotAbusingPoint() >= MAX_ABUSING_POINT )
	{
		pAbuseUser->ForcedDisconnect();

		// Disconnect 회수가 MAX_USER_DISCONNECT_COUNT를 넘어가면 임시블럭이다.
		if( pAbuseUser->IsBlock() )
		{
			pAbuseUser->TemporaryBlock();
		}

		BOOL bIPBlockBeforeInsert = IsIPBlock( pMsgNTF->m_IP );

		// 동일 IP에 대한 어뷰징 유저수를 관리한다.
		m_AbuseIPGroupHash[pMsgNTF->m_IP].insert( pMsgNTF->m_dwUserGUID );

		// IP가 임시블럭 상태이면 해당IP 유저들을 모두 Disconnect 처리하고 Wops에 보고한다. 최초 한번만!
		if( bIPBlockBeforeInsert == FALSE && IsIPBlock( pMsgNTF->m_IP ) )
		{
            // (CHANGES) (f100607.4L) fix up a buffer overrun problem
            TCHAR szMessage[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH];
            _sntprintf(szMessage, _countof(szMessage),
                       _T("IP[%s] Blocked!"), IPOperation::ConvertDWORD2StrIP(pMsgNTF->m_IP));
            szMessage[_countof(szMessage) - 1] = _T('\0');
            NotifyViolation(eABUSE_SUB_WORKPLACE_IP_BLOCK,
                            szMessage, pNOTIFY->m_ServerKey, pMsgNTF->m_IP);

			// 해당IP 유저들을 모두 Disconnect 한다.
			ABUSE_USER_HASH_IT it = m_AbuseUserHash.begin(), itend = m_AbuseUserHash.end();
			for( ; it != itend; ++it )
			{
				AbuseUser* pAbuseUser = it->second;
				if( pAbuseUser->GetConnectedIP() == pMsgNTF->m_IP )
				{
					pAbuseUser->ForcedDisconnect();
				}
			}
		}
	}
}

DWORD AbuseUserManager::GetUserCountForIP( DWORD IP )
{
	ABUSE_USERS_IN_IP_HASH_IT it = m_AbuseIPGroupHash.find( IP );

	if( it != m_AbuseIPGroupHash.end() )
	{
		ABUSE_USERS_IN_IP_KEY& rKEYTABLE = it->second;
		DWORD dwCount = (DWORD)rKEYTABLE.size();
		return dwCount;
	}

	return 0;
}

BOOL AbuseUserManager::IsIPBlock( DWORD IP )
{
	DWORD dwCount = GetUserCountForIP( IP );

	// 동일 IP에 대한 어뷰징 유저수가 MAX_IP_DISCONNECT_COUNT를 넘어가면 IP가 임시블럭된다.
	if( dwCount >= MAX_IP_DISCONNECT_COUNT )
		return TRUE;

	return FALSE;
}

VOID AbuseUserManager::RemoveUser( DWORD dwUserGUID )
{
	AbuseUser* pAbuseUser = FindUser( dwUserGUID );

	if( pAbuseUser )
	{
		m_AbuseUserPool.Free( pAbuseUser );
		m_AbuseUserHash.erase( dwUserGUID );
	}
}

AbuseUser* AbuseUserManager::FindUser( DWORD UserGuid )
{
	ABUSE_USER_HASH_IT it = m_AbuseUserHash.find( UserGuid );

	if( it != m_AbuseUserHash.end() )
		return it->second;

	return NULL;
}

VOID AbuseUserManager::WriteFileLog( eABUSE_TYPE eAbuseType, eABUSE_SUB_TYPE eAbuseSubType, SERVER_KEY & rServerKey, TCHAR *pszUserID, TCHAR *pszCharName, TCHAR *pszUserIP, TCHAR *pszMessage )
{
	// 너무 많이 남기 때문에 국내에서만 남겨서 확인한다.
#ifndef _KOREA
	return;
#endif

	SYSTEMTIME LocalTime;	util::TimeSync::GetLocalTime(&LocalTime);

	MessageOutEx
	( 
		g_pSecureCollectLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_ABUSE,
		_T("%02u\t%02u\t%02u\t%02u\t%s\t%-13s\t%-17s\t%-16s\tTIME(%02u:%02u:%02u)\t%s"), 
		eAbuseType, eAbuseSubType,
		rServerKey.GetWorldID(), rServerKey.GetChannelID(), GetServerType2String( (eSERVER_TYPE)rServerKey.GetServerType() ),
		pszUserID, pszCharName, pszUserIP, 
		LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, 
		pszMessage 
	);
}

VOID AbuseUserManager::NotifyViolation( eABUSE_SUB_TYPE AbuseSubType, TCHAR *pMessage, SERVER_KEY & rServerKey, DWORD ConnectedIP,
										DWORD UserGuid, DWORD CharGuid, TCHAR *pszUserID, TCHAR *pszCharName )
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_ABUSING_NTF* pSendNTF = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_ABUSING_NTF;
#else
	MSG_MO_RTTG_ABUSING_NTF pSendNTF[1];
#endif

	pSendNTF->m_ptszDesc[0] = _T('\0');

	util::GetDateTime_YYYYMMDDHHMMSS(pSendNTF->m_dw64Time);
	pSendNTF->m_ServerKey	= rServerKey;
	pSendNTF->m_dwUserGuid	= UserGuid;
	pSendNTF->m_dwCharGuid	= CharGuid;

#ifdef __WAVERIX_ABUSE_EXTENDED_FORMAT__
	_tcsncpy( pSendNTF->m_pszUserID, pszUserID, AU_LEN_MAX_ID );
	pSendNTF->m_pszUserID[AU_LEN_MAX_ID-1] = '\0';
#endif

	_tcsncpy( pSendNTF->m_pCharName, pszCharName, AU_LEN_MAX_CHARNAME );
	pSendNTF->m_pCharName[AU_LEN_MAX_CHARNAME-1] = '\0';

	memcpy( pSendNTF->m_IP, IPOperation::ConvertDWORD2StrIP(ConnectedIP), sizeof(pSendNTF->m_IP) );

	ABUSING_OPTION_INFO *pSharedAbusingOptionInfo = AbusingOptionParser::Instance()->GetAbusingOptionInfo( AbuseSubType );
	if( pSharedAbusingOptionInfo )
	{
		pSendNTF->m_AbuseType = pSharedAbusingOptionInfo->m_AbuseType;
		pSendNTF->m_Level = pSharedAbusingOptionInfo->m_Level;
		_tcsncpy( pSendNTF->m_ptszSubTypeName, pSharedAbusingOptionInfo->m_szViolationName, MSG_MO_RTTG_ABUSING_NTF::MAX_SUBTYPE_NAME_LEN );
		pSendNTF->m_ptszSubTypeName[MSG_MO_RTTG_ABUSING_NTF::MAX_SUBTYPE_NAME_LEN] = '\0';
	}
	else
	{
		pSendNTF->m_AbuseType = 0;
		pSendNTF->m_ptszSubTypeName[0] = _T('\0');
		pSendNTF->m_Level = 0;
	}

	pSendNTF->m_wLength = _sntprintf( pSendNTF->m_ptszDesc, MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH, "%s", pMessage );
	pSendNTF->m_ptszDesc[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH] = '\0';

	// Wops에 보고하기
	OpServerSession* pOpServer = MasterServerEx::GetMainFrame()->ConnectSessions.SessionOpServer();
	if( pOpServer && pOpServer->IsConnected() )
	{
		pOpServer->Send( (BYTE*)pSendNTF, pSendNTF->GetSize() );
	}

	// 파일로그 남기기
	WriteFileLog( (eABUSE_TYPE)pSendNTF->m_AbuseType, AbuseSubType, rServerKey, pszUserID, pszCharName, pSendNTF->m_IP, pSendNTF->m_ptszDesc );
}


VOID AbuseUserManager::DisplayAbuseInfo()
{
	int AbuseUserNum = (int)m_AbuseUserHash.size();
	int ForcedDisUserNum = 0, BlockUserNum = 0, AvgDisCount = 0;

	ABUSE_USER_HASH_IT it = m_AbuseUserHash.begin(), itend = m_AbuseUserHash.end();
	for( ; it != itend; ++it )
	{
		AbuseUser* pAbuseUser = it->second;
		
		if( pAbuseUser->IsBlock() )
			BlockUserNum++;

		if( pAbuseUser->GetDisconnectCount() )
		{
			ForcedDisUserNum++;
			AvgDisCount += pAbuseUser->GetDisconnectCount();
		}
	}

	if( ForcedDisUserNum )	AvgDisCount /= ForcedDisUserNum;

	SUNLOG( eCRITICAL_LOG, "[AbuseUserManager] AbuseUserNum[%u] ForcedDisUserNum[%u] AvgDisCount[%u] BlockUserNum[%u]", AbuseUserNum, ForcedDisUserNum, AvgDisCount, BlockUserNum );

	ABUSE_USERS_IN_IP_HASH_IT it2 = m_AbuseIPGroupHash.begin(), itend2 = m_AbuseIPGroupHash.end();
	for( ; it2 != itend2; ++it2 )
	{
		ABUSE_USERS_IN_IP_KEY& rKEYTABLE = it2->second;
		DWORD dwCount = (DWORD)rKEYTABLE.size();

		if( dwCount >= 1 ) 
			SUNLOG( eCRITICAL_LOG, "[AbuseUserManager] IP[%s] IPBlockMaxUserNum[%u]", IPOperation::ConvertDWORD2StrIP(it2->first), dwCount );
	}
}



