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
	// �ֱ������� ��� ������ �ʱ�ȭ�Ѵ�.
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

	// �ش� IP�� ��ϻ������� üũ�Ѵ�.
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

	// �ش� ������ ��ϻ������� üũ�Ѵ�.
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

	// ���� ���¡�̸� ������ �����Ѵ�.
	if( !pAbuseUser )
	{
		pAbuseUser = (AbuseUser*)m_AbuseUserPool.Alloc();
		pAbuseUser->Init( pMsgNTF->m_dwUserGUID, pMsgNTF->m_pUserID );

		m_AbuseUserHash[pMsgNTF->m_dwUserGUID] = pAbuseUser;
	}

	// �ش� IP, ������ �̹� ��ϻ����� ��� ���̻��� �α׸� ������ �ʴ´�.
	if( IsIPBlock( pMsgNTF->m_IP ) || pAbuseUser->IsBlock() )
	{
		return;
	}

	// �ش� ���¡ ������ �����ϰ� Milestone�� �����ϸ� Wops�� �����Ѵ�.
	if( pAbuseUser->UpdateAbuseInfo( (eABUSE_SUB_TYPE)pMsgNTF->m_AbuseSubType, pNOTIFY->m_ServerKey, pMsgNTF->m_IP, pMsgNTF->m_dwCharGUID, pMsgNTF->m_pCharName ) )
	{
		pAbuseUser->NotifyViolation( (eABUSE_SUB_TYPE)pMsgNTF->m_AbuseSubType, pNOTIFY->m_pMessage );
	}

	// ���¡����Ʈ�� MAX_ABUSING_POINT�� �Ѿ�� Disconnect ó���� ���� Wops�� �����Ѵ�.
	if( pAbuseUser->GetTotAbusingPoint() >= MAX_ABUSING_POINT )
	{
		pAbuseUser->ForcedDisconnect();

		// Disconnect ȸ���� MAX_USER_DISCONNECT_COUNT�� �Ѿ�� �ӽú��̴�.
		if( pAbuseUser->IsBlock() )
		{
			pAbuseUser->TemporaryBlock();
		}

		BOOL bIPBlockBeforeInsert = IsIPBlock( pMsgNTF->m_IP );

		// ���� IP�� ���� ���¡ �������� �����Ѵ�.
		m_AbuseIPGroupHash[pMsgNTF->m_IP].insert( pMsgNTF->m_dwUserGUID );

		// IP�� �ӽú� �����̸� �ش�IP �������� ��� Disconnect ó���ϰ� Wops�� �����Ѵ�. ���� �ѹ���!
		if( bIPBlockBeforeInsert == FALSE && IsIPBlock( pMsgNTF->m_IP ) )
		{
            // (CHANGES) (f100607.4L) fix up a buffer overrun problem
            TCHAR szMessage[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH];
            _sntprintf(szMessage, _countof(szMessage),
                       _T("IP[%s] Blocked!"), IPOperation::ConvertDWORD2StrIP(pMsgNTF->m_IP));
            szMessage[_countof(szMessage) - 1] = _T('\0');
            NotifyViolation(eABUSE_SUB_WORKPLACE_IP_BLOCK,
                            szMessage, pNOTIFY->m_ServerKey, pMsgNTF->m_IP);

			// �ش�IP �������� ��� Disconnect �Ѵ�.
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

	// ���� IP�� ���� ���¡ �������� MAX_IP_DISCONNECT_COUNT�� �Ѿ�� IP�� �ӽú��ȴ�.
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
	// �ʹ� ���� ���� ������ ���������� ���ܼ� Ȯ���Ѵ�.
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

	// Wops�� �����ϱ�
	OpServerSession* pOpServer = MasterServerEx::GetMainFrame()->ConnectSessions.SessionOpServer();
	if( pOpServer && pOpServer->IsConnected() )
	{
		pOpServer->Send( (BYTE*)pSendNTF, pSendNTF->GetSize() );
	}

	// ���Ϸα� �����
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



