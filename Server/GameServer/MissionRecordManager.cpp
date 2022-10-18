#include "StdAfx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include ".\missionrecordmanager.h"
#include <time.h>
#include "Player.h"
#include <PacketStruct_CG.h>
#include "GameServerEx.h"
#include <SolarDateTime.h>
#include "GameRankingManager.h"
#include "GameZone.h"

MissionRecordManager::MissionRecordManager(void)
{
	m_bStartMissionTime = FALSE;
}

MissionRecordManager::~MissionRecordManager(void)
{
	m_bStartMissionTime = FALSE;
}

void MissionRecordManager::Clear()
{
	SetCurMissionNo(0);
	SetCurMissionStartTime(0);
	SetTotalMonsterKillNum(0);
	SetSatisfyCondition( FALSE );
	SetMinLimitedMonsterKillNum(0);

	ZeroMemory( m_MonsterKillNum, sizeof(DWORD)*COUNTING_MONSTER_TYPE_NUM );
}

DWORD MissionRecordManager::DecreasingTime( DWORD desc_time )
{
	DWORD CurTick = GetTickCount();
	m_CurMissionStartTime += desc_time;
	if( CurTick < m_CurMissionStartTime )
		m_CurMissionStartTime = CurTick;

	return (CurTick - m_CurMissionStartTime);
}

void MissionRecordManager::KillMonster( eNPC_GRADE grade, GameZone * pZone )
{
	switch( grade )
	{
	case eNPC_GENERAL:
		// 100 >= 100
		if( m_TotalMonsterKillNum >= GetMinLimitedMonsterKillNum() )
			++m_MonsterKillNum[COUNTING_MONSTER_TYPE_GENERAL];
		break;
	case eNPC_LEADER:
		if( m_TotalMonsterKillNum >= GetMinLimitedMonsterKillNum() )
			++m_MonsterKillNum[COUNTING_MONSTER_TYPE_LEADER];
		break;
	default:
		return;
	}

	++m_TotalMonsterKillNum;
	ASSERT(SAFE_NUMERIC_TYPECAST( DWORD, m_TotalMonsterKillNum, WORD ));

    // TODO: waverix, too many instanced, thinks about quantity update mechanism
	MSG_CG_MISSION_TIMER_CHANGED_BRD msg;
	msg.m_TotalMonsterKillNum = (WORD)m_TotalMonsterKillNum;
	pZone->SendPacketToAll( &msg, sizeof(msg) );
}


void MissionRecordManager::StartMission( RANKINGNO wMissionNo )
{
/*
	if( wMissionNo >= MAX_MISSION_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "StartMission() MissionNo[%u]Error", wMissionNo );
		return;
	}
*/

	//시작 상태면 시작할 수 없다.
	if( m_bStartMissionTime )
	{		
		return;
	}
	m_bStartMissionTime = TRUE;
	
		
	Clear();

	SetCurMissionNo( wMissionNo );
	SetCurMissionStartTime( GetTickCount() );
	SetSatisfyCondition( FALSE );

	RankingManager::LEVEL_AREA * pLEVEL_AREA = g_GameRankingMgr.FindLVArea(GetCurMissionNo());
	if( pLEVEL_AREA )
		SetMinLimitedMonsterKillNum( pLEVEL_AREA->m_MaxMonsterKill );
	else
		SUNLOG( eCRITICAL_LOG, "MissionNo%u:is not LEVEL_AREA or PortalServer isn't connected", GetCurMissionNo() );

	// 초기화
	m_MissionRankingMsg.m_totalInfo.m_Info.m_MissionNo = GetCurMissionNo();
	m_MissionRankingMsg.m_totalInfo.m_Count = 0;
	
}

void MissionRecordManager::ClearMission( GameZone * pZone )
{
	MSG_CG_MISSION_CLEAR_RECORD_CMD cmsg;
	cmsg.m_MonsterKillNum_General	= m_MonsterKillNum[COUNTING_MONSTER_TYPE_GENERAL];
	cmsg.m_MonsterKillNum_Leader	= m_MonsterKillNum[COUNTING_MONSTER_TYPE_LEADER];
	pZone->SendPacketToAll( &cmsg, sizeof(cmsg) );

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //몬스터킬로 인한 클리어타임보상을 감소시킨다. 보상이 너무 커서 0초기록이 발생한다.
	DWORD MinusTime = m_MonsterKillNum[COUNTING_MONSTER_TYPE_GENERAL]*20 + m_MonsterKillNum[COUNTING_MONSTER_TYPE_LEADER]*40;
#else
	DWORD MinusTime = m_MonsterKillNum[COUNTING_MONSTER_TYPE_GENERAL]*1000 + m_MonsterKillNum[COUNTING_MONSTER_TYPE_LEADER]*2000;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	m_MissionClearTime = (GetTickCount() - m_CurMissionStartTime);
	if( MinusTime > m_MissionClearTime )
		m_MissionClearTime = 0;
	else
		m_MissionClearTime -= MinusTime;

	DWORD64 RegisterTime=0;
	util::GetDateTime_YYYYMMDDHHMMSSNNN( RegisterTime );
	m_MissionRankingMsg.m_totalInfo.m_Info.m_MissionNo = GetCurMissionNo();
	m_MissionRankingMsg.m_totalInfo.m_Info.m_ClearTime = GetMissionClearTime();
	m_MissionRankingMsg.m_totalInfo.m_Info.m_RegisterTime = RegisterTime;
	m_MissionRankingMsg.m_totalInfo.m_Count = 0;

	m_MissionRankingMsg.m_Key = 0;
		
	if( IsSatisfyCondition() )
	{
		if( m_TotalMonsterKillNum < GetMinLimitedMonsterKillNum() )
		{
			SetSatisfyCondition( FALSE );
			SetConditionErrorCode( RC::RC_RANKING_UNSATISFY_KILL_MONSTER_NUM );
		}
	}

	DISPMSG( "MissionNo[%u]ClearTime:%u,RegisterTime:%I64u,MinusTime:%d,Mon1:%u,Mon2:%u\n", 
		GetCurMissionNo(), GetMissionClearTime(), RegisterTime, MinusTime, m_MonsterKillNum[COUNTING_MONSTER_TYPE_GENERAL], m_MonsterKillNum[COUNTING_MONSTER_TYPE_LEADER] );
}

void MissionRecordManager::SendElapesedTime( Player * pPlayer )
{
	DWORD elapsed_time = (GetTickCount() - m_CurMissionStartTime);

	MSG_CG_MISSION_RECORD_ELAPSEDTIME_CMD msg;
	msg.m_MissionElapsedTimePerSecond = (DWORD)(elapsed_time/1000);
	msg.m_MissionBestClearTimePerSecond = 0;

	MissionRanking * pRanking = g_GameRankingMgr.FindMissionRanking( eRANKING_TODAYWEEK, GetCurMissionNo() );
	if( pRanking )
	{
		MissionRankingRecord * pRecord = pRanking->GetRecordOrder1();
		if( pRecord )
		{
			msg.m_MissionBestClearTimePerSecond = (DWORD)(pRecord->GetClearTime()/1000);
		}
	}
	pPlayer->SendPacket( &msg, sizeof(msg) );

	// 플레이어 정보 셋팅
	UpdateClearTime( pPlayer );

	// 미션 랭킹 조건 만족 여부 체크
	RC::eRANKING_RESULT rt = g_GameRankingMgr.CanRegisterMissionRecord( m_MissionRankingMsg.m_totalInfo );
	if( rt == RC::RC_RANKING_SUCCESS )
	{
		SetSatisfyCondition( TRUE );
	}
	else
	{
		SetSatisfyCondition( FALSE );
		SetConditionErrorCode( rt );
	}
}

void MissionRecordManager::UpdateClearTime( Player * pPlayer )
{
	if( pPlayer->IsDead() ) return;

/*
	if( GetCurMissionNo() >= MAX_MISSION_NUM ) return;
*/

	if( m_MissionRankingMsg.m_totalInfo.m_Count >= _MISSION_RANKING_TOTAL_INFO::MAX_SLOT_NUM ) 
	{
		SUNLOG( eCRITICAL_LOG, _T("[MissionRecordManager::UpdateClearTime] Over Party Count[%d]"), m_MissionRankingMsg.m_totalInfo.m_Count );
		return;
	}

	BYTE count = m_MissionRankingMsg.m_totalInfo.m_Count;
	m_MissionRankingMsg.m_totalInfo.m_Member[count].m_UserGuid	= pPlayer->GetUserGuid();
	m_MissionRankingMsg.m_totalInfo.m_Member[count].m_Class		= pPlayer->GetCharInfo()->m_byClassCode;
	m_MissionRankingMsg.m_totalInfo.m_Member[count].m_LV		= pPlayer->GetLevel();
	_tcsncpy( m_MissionRankingMsg.m_totalInfo.m_Member[count].m_tszCharName, pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_MissionRankingMsg.m_totalInfo.m_Member[count].m_tszCharName[MAX_CHARNAME_LENGTH]= '\0';

	if( 0 != pPlayer->GetCharInfo()->m_GuildGuid )
	{
		_tcsncpy( m_MissionRankingMsg.m_totalInfo.m_Member[count].m_tszGuildName, pPlayer->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH );
		m_MissionRankingMsg.m_totalInfo.m_Member[count].m_tszGuildName[MAX_GUILDNAME_LENGTH]= '\0';
	}
	else
	{
		m_MissionRankingMsg.m_totalInfo.m_Member[count].m_tszGuildName[0]= '\0';
	}

	++m_MissionRankingMsg.m_totalInfo.m_Count;

}

void MissionRecordManager::UpdateClearTimeParty( GameZone * pZone )
{
	RC::eRANKING_RESULT rt = RC::RC_RANKING_SUCCESS;

	if( IsSatisfyCondition() )
	{
		// 포탈 서버가 켜져있는지 체크?
		if( g_pGameServer->IsConnected(RANKING_SERVER) )
		{
			rt = g_GameRankingMgr.CanRegisterMissionRecord( m_MissionRankingMsg.m_totalInfo );
			if( rt == RC::RC_RANKING_SUCCESS )
			{
				DWORD TmpKey = g_GameRankingMgr.RegisterMissionRecordInTemp( m_MissionRankingMsg.m_totalInfo );
				ASSERT( 0 != TmpKey );
				m_MissionRankingMsg.m_Key = TmpKey;
				g_pGameServer->SendToServer( RANKING_SERVER, &m_MissionRankingMsg, m_MissionRankingMsg.GetSize() );

				return;
				// RETURN ----------------------------------------------------------------------------------------------
			}
		}
		else
		{
			rt = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
		}
	}
	else
	{
		rt = (RC::eRANKING_RESULT)(GetConditionErrorCode());
	}
	
	
	// 랭킹 전송 안함
	if( pZone )
	{
		MSG_CG_MISSION_RANKING_FAILED_BRD msg;
		msg.m_ClearTime = GetMissionClearTime();
		msg.m_dwErrorCode	= rt;
		pZone->SendPacketToAll( &msg, sizeof(msg) );
	}
}

void MissionRecordManager::OnCreate()
{
	m_bStartMissionTime = FALSE;
}

void MissionRecordManager::OnDestroy()
{
	m_bStartMissionTime = FALSE;
}

//---------------------------------------------------------------------------------------
/// 테스트 용도
//---------------------------------------------------------------------------------------
void MissionRecordManager::ClearMissionTest( DWORD time, INT64 RegisterTime )
{
	m_MissionClearTime = time;

	m_MissionRankingMsg.m_totalInfo.m_Info.m_MissionNo = GetCurMissionNo();
	m_MissionRankingMsg.m_totalInfo.m_Info.m_ClearTime = GetMissionClearTime();
	m_MissionRankingMsg.m_totalInfo.m_Info.m_RegisterTime = RegisterTime;
	m_MissionRankingMsg.m_totalInfo.m_Count = 0;

	m_MissionRankingMsg.m_Key = 0;

	DISPMSG( "미션[%u]클리어시간:%u,등록시간:%I64u\n", GetCurMissionNo(), GetMissionClearTime(), RegisterTime );
}
#endif // _NA_008012_20150130_RANKING_SYSTEM