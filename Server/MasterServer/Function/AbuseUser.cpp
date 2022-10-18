#include "stdafx.h"
#include "AbusingOptionParser.h"
#include "AbuseUser.h"
#include "AbuseUserManager.h"
#include "PacketStruct_MO.h"
#include <SolarDateTime.h>
#include <Sessions/ServerSessionManager.h>


VOID AbusePointInfo::Init()
{
	m_AbusingPoint = 0;
	m_ViolationCount = 0;
}

BOOL AbusePointInfo::IncViolationCount()
{
	m_ViolationCount++;

	BOOL bMilestone = FALSE;

	if( !m_pSharedAbusingOptionInfo )	return bMilestone;

	WORD DiffViolationCount = m_ViolationCount % m_pSharedAbusingOptionInfo->m_MilestoneCount;

	if( DiffViolationCount == 0 )
	{
		m_AbusingPoint += m_pSharedAbusingOptionInfo->m_IncAbusingPoint;
		m_AbusingPoint = min( m_pSharedAbusingOptionInfo->m_MaxAbusingPoint, m_AbusingPoint );
		bMilestone = TRUE;
	}

	return bMilestone;
}

AbuseUser::AbuseUser()
{
	for( int AbuseSubType = 0; AbuseSubType < eABUSE_SUB_MAX; ++AbuseSubType )
	{
		ABUSING_OPTION_INFO *pOtionInfo = AbusingOptionParser::Instance()->GetAbusingOptionInfo( AbuseSubType );

		if( !pOtionInfo )
			SUNLOG( eCRITICAL_LOG, "[AbuseUser::AbuseUser] Invalid AbuseSubType(%d)", AbuseSubType );

		m_AbusePointInfo[AbuseSubType].SetSharedAbusingOptionInfo( pOtionInfo );
	}
}

VOID AbuseUser::Init( DWORD UserGuid, TCHAR * pszUserID )
{
	m_UserGUID = UserGuid;
	_tcsncpy( m_pszUserID, pszUserID, AU_LEN_MAX_ID );

	m_ServerKey = 0;
	m_ConnectedIP = 0;
	m_CharGUID = 0;
	ZeroMemory( m_pszCharName, sizeof(m_pszCharName) );

	m_PointClearTimer.SetNextTimeoutByCurTickBased( POINT_CLEAR_TIME );

	m_DisconnectCount = 0;

	for( int AbuseSubType = 0; AbuseSubType < eABUSE_SUB_MAX; ++AbuseSubType )
		m_AbusePointInfo[AbuseSubType].Init();
}

VOID AbuseUser::Update()
{
	// 주기적으로 어뷰징 포인트를 초기화한다.
	if( m_PointClearTimer.IsExpired() )
	{
		m_PointClearTimer.SetNextTimeoutByCurTickBased( POINT_CLEAR_TIME );
	}
}

BOOL AbuseUser::UpdateAbuseInfo( eABUSE_SUB_TYPE AbuseSubType, SERVER_KEY & ServerKey, DWORD ConnectedIP, DWORD CharGuid, TCHAR *pszCharName )
{
	if( AbuseSubType >= eABUSE_SUB_MAX )
	{
		SUNLOG( eCRITICAL_LOG, "[AbuseUser::UpdateAbuseInfo] Invalid AbuseSubType[%u]", AbuseSubType );
		return FALSE;
	}

	m_ServerKey = ServerKey;
	m_ConnectedIP = ConnectedIP;
	m_CharGUID = CharGuid;
	_tcsncpy( m_pszCharName, pszCharName, AU_LEN_MAX_CHARNAME );
	m_pszCharName[AU_LEN_MAX_CHARNAME-1] = '\0';

#ifdef _CHINA
	// 중국은 네트워크 특수성으로 인해 좌표이상 관련 어뷰징 처리는 하지 않는다.
	switch( AbuseSubType )
	{
	case eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SPEED:
	case eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE:
	case eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION:
	case eABUSE_SUB_LOGIC_VIOLATION_NOT_SYNC_POSITION:
	case eABUSE_SUB_LOGIC_VIOLATION_MOVE_RANGE:
	case eABUSE_SUB_LOGIC_VIOLATION_MOVE_SPEED:
		return FALSE;
	}
#endif

	// 해당 어뷰징 정보를 갱신하고 Milestone 도달 여부를 리턴한다.
	BOOL bMilestone = m_AbusePointInfo[AbuseSubType].IncViolationCount();

	return bMilestone;
}

WORD AbuseUser::GetTotAbusingPoint()
{
	WORD m_AbusingPoint = 0;

	for( int AbuseSubType = 0; AbuseSubType < eABUSE_SUB_MAX; ++AbuseSubType )
		m_AbusingPoint += m_AbusePointInfo[AbuseSubType].GetAbusingPoint();

	return m_AbusingPoint;
}

BOOL AbuseUser::IsBlock()
{ 
	return m_DisconnectCount >= MAX_USER_DISCONNECT_COUNT; 
}

VOID AbuseUser::ForcedDisconnect( BOOL bNotifyToWops )
{
	m_DisconnectCount++;

	// Wops에 보고한다.
	if( bNotifyToWops )
	{
        // (CHANGES) (f100607.4L) fix up a buffer overrun problem
        TCHAR szMessage[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH];
        _sntprintf(szMessage, _countof(szMessage),
                   _T("[AbuseUser::ForcedDisconnect] Disconnect:%u"),
                   GetDisconnectCount());
        szMessage[_countof(szMessage) - 1] = _T('\0');
        NotifyViolation(eABUSE_SUB_FORCED_DISCONNECT, szMessage);
	}

	// m_AbusingPoint는 초기화 하지만 m_ViolationCount는 나중에 확인을 위해 초기화하지 않는다.
	for( int AbuseSubType = 0; AbuseSubType < eABUSE_SUB_MAX; ++AbuseSubType )
		m_AbusePointInfo[AbuseSubType].InitAbusingPoint();

	// AgentServer에게 해당 유저를 종료시키도록 명령한다.
	SERVER_KEY skServerKey( m_ServerKey.GetWorldID(), m_ServerKey.GetChannelID(), AGENT_SERVER, 1 );
	ServerSessionEx* pServerSession = ServerSessionManager::FindServer( skServerKey );
	if( pServerSession )
	{
#ifndef _CHINA
		MSG_AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD msgCMD;
		msgCMD.UserGUID() = GetUserGUID();
		pServerSession->SendPacket( &msgCMD, sizeof(msgCMD) );
#endif
	}
}

VOID AbuseUser::TemporaryBlock()
{
    // (CHANGES) (f100607.4L) fix up a buffer overrun problem
    TCHAR szMessage[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH];
    _sntprintf(szMessage, _countof(szMessage),
               _T("[AbuseUser::TemporaryBlock] Disconnect:%u"),
               GetDisconnectCount());
    szMessage[_countof(szMessage) - 1] = _T('\0');
    NotifyViolation(eABUSE_SUB_TEMPORARY_BLOCK, szMessage);

	// 이후 해당유저가 다시 접속을 하면 AM_CONNECTION_ENTERSERVER_CMD에서 블럭유무를 확인한 후 ForcedDisconnect 시킨다.
}

VOID AbuseUser::NotifyViolation( eABUSE_SUB_TYPE AbuseSubType, TCHAR *pMessage )
{
    // (CHANGES) (f100607.4L) fix up a buffer overrun problem
    TCHAR szMessage[MSG_MO_RTTG_ABUSING_NTF::MAX_DESC_LENGTH];
    _sntprintf(szMessage, _countof(szMessage),
               _T("%s [ViolationCount:%u][AbusingPoint:%u][TotAbusingPoint:%u]"),
               pMessage, m_AbusePointInfo[AbuseSubType].GetViolationCount(),
               m_AbusePointInfo[AbuseSubType].GetAbusingPoint(), GetTotAbusingPoint());
    szMessage[_countof(szMessage) - 1] = _T('\0');
    AbuseUserManager::Instance()->NotifyViolation(AbuseSubType, szMessage,
        m_ServerKey, m_ConnectedIP, GetUserGUID(), GetCharGUID(), m_pszUserID, m_pszCharName );
}


