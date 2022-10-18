#include "StdAfx.h"
#include "CompetitionTeam.h"
#include "CompetitionFormula.h"
#include "PVPInfo.h"
#include "PacketStruct_AG.h"
#include "CHuntingRewardParser.h"
#include "GameZone.h"
#include "RewardManager.h"
#include "CompetitionInfo.h"
#include "CHuntingManager.h"
#include "MapInfoParser.h"
#include "GameCHuntingRoom.h"
#include "ServerOptionParser.h"
#include "StatusManager.h"
#include "ItemManager.h"
#include <ServerOptionParserEx.h>
#include <RewardInfoList.h>
#include <GameServerEx.h>


CompetitionTeam::CompetitionTeam()
{
}

CompetitionTeam::~CompetitionTeam()
{
}

VOID CompetitionTeam::Init( CHuntingManager *pMgr, WORD wTeamKey, eCOMPETITION_RULE_TYPE eRule, eCOMPETITION_MODE_TYPE eMode )
{
	m_pCHuntngMgr = pMgr;
	m_wTeamKey = wTeamKey;
	m_Rule = eRule;
	m_Mode = eMode;
	m_State = eTEAM_STATE_NONE;
	m_wKillPoint = 0;
	m_wWinPoint = 0;
	m_byInitMemberNum = 0;
	m_TotLevel = 0;
	m_eExpireState = eEXPIRE_STATE_NONE;
	m_bChampion = FALSE;

	m_PlayerHash.clear();
	m_wPartyKey = 0;
	m_setReadyUserKeys.clear();
	m_bResultTie = FALSE;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	m_GuildGuid = 0;
#endif
}

VOID CompetitionTeam::Update()
{
	Player* pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;

		if( pPlayer->IsAlive() )	continue;

		if( pPlayer->GetCompetitionInfo().IsExpiredResurrection() )
			pPlayer->OnResurrection( 0, 1.f, 1.f );
	}

	switch( m_State )
	{
	case eTEAM_STATE_MATCH:
		{
			if( OnIsExpired() )
			{
				OnEnd();
			}
		}
		break;
	case eTEAM_STATE_END:
		{
			if( m_EndWaitTimer.IsExpiredManual( FALSE ) )
			{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
				OnExpels();
#else
				OnExpel();
#endif
			}
		}
		break;
	}
}

VOID CompetitionTeam::Start()
{
	m_State = eTEAM_STATE_MATCH;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	// 소멸시켜야할 입장권 개수
	POSTYPE EntranceCount = 0;	SLOTCODE ItemCode = 0;
	if( m_pCHuntngMgr->IsGuildPVPMode() )
	{
		sMAPINFO* pInfo = (sMAPINFO*)MapInfoParser::Instance()->FindMapInfo( m_pCHuntngMgr->GetCHunting()->GetMapCode() );
		if( pInfo )
		{
			ItemCode = pInfo->EntranceItemCode;
			EntranceCount = pInfo->EntranceItemCount;
		}
	}
#endif

	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;

		// 경쟁헌팅 참가횟수를 증가시킨다.
		pPlayer->GetCompetitionInfo().IncPlayTimes();

		if( !IsTimeAttackMode() )
		{
			// 각 파티멤버들이 PVP가 가능하도록 상태를 변경한다.
			pPlayer->GetPVPInfo().Clear();
			pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_MATCH );
		}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		// 이전 판이 무승부였으면 입장권을 소멸시키지 않는다.
		if( !m_pCHuntngMgr->IsGuildPVPMode() || m_bResultTie )	continue;


		// 필요한 개수만큼 입장권을 가지고 있으면 소멸시키고 없으면 퇴장시킨다.
		if( pPlayer->GetItemOwnFlag().GetItemNum( eITEMTYPE_GUILD_PVP_ENTRANCE ) >= EntranceCount )
		{
			MSG_CG_ITEM_LOSE_ACK msg;
			if( RC::RC_ITEM_SUCCESS == pPlayer->GetItemManager()->DeleteAllKindOfItem( 1, &ItemCode, &EntranceCount, &(msg.m_ItemInfo) ) )
			{
				if( msg.m_ItemInfo.m_Count != 0 )
					pPlayer->SendPacket( &msg, msg.GetSize() );
			}
			else
				OnExpel( pPlayer );
		}
		// 없으면 퇴장시킨다.
		else
		{
			OnExpel( pPlayer );
		}
#endif
	}
}

BOOL CompetitionTeam::OnIsExpired()
{
	// 멤버수가 0이면 종료
	if( m_PlayerHash.size() == 0 )
	{
		m_eExpireState = eEXPIRE_STATE_ALL_LEAVE;
		return TRUE;
	}

	if( IsSurvivalMode() )
	{
		// 멤버들이 모두 죽으면 종료
		if( IsAllDied() )
		{
			m_eExpireState = eEXPIRE_STATE_ALL_DEAD;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CompetitionTeam::IsAllDied()
{
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;
		if( pPlayer->IsAlive() )	return FALSE;
	}

	return TRUE;
}

VOID CompetitionTeam::OnEnd()
{
	m_State = eTEAM_STATE_END;

	// 일정시간 기다린 후에 추방시킨다.
	m_EndWaitTimer.SetTimer( END_WAIT_TIME );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
VOID CompetitionTeam::OnExpel( Player *pPlayer )
{
	MSG_AG_ZONE_CHUNTING_LEAVE_CMD msg;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}
VOID CompetitionTeam::OnExpels()
{
	// 강제로 추방시킨다.
	Player * pPlayer = NULL;
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != m_PlayerHash.end(); ++it )
	{
	#ifndef __TW_0_20080521_GUILD_PVP_BUG_FIX	//자료구조 변경되었음.
		pPlayer = *it;	
	#else
		pPlayer = it->second;
	#endif
		OnExpel( pPlayer );
	}

	m_State = eTEAM_STATE_NONE;
}
#else
VOID CompetitionTeam::OnExpel()
{
	MSG_AG_ZONE_CHUNTING_LEAVE_CMD msg;

	// 강제로 추방시킨다.
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	m_State = eTEAM_STATE_NONE;
}
#endif

VOID CompetitionTeam::ResultTie()
{
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;
		// PVP 해제
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// 죽었던 플레이어 부활
		if( pPlayer->IsDead() )
		{
			pPlayer->OnResurrection( 0, 1.f, 1.f );
		}

		// 경쟁 포인트 초기화
		pPlayer->GetCompetitionInfo().PointInit();

		// 무승부 메세지를 날린다.
		MSG_CG_CHUNTING_TIE_COMPETITION_BRD msg;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	m_State = eTEAM_STATE_NONE;
	m_wKillPoint = 0;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	m_bResultTie = TRUE;
#endif
}

VOID CompetitionTeam::ResultWin( CompetitionTeam *pLoseTeam, WORD wProcessTime )
{
	// 개인 등수 구하기
	OnCalcRanking();

	BOOL bGetItem = FALSE;

	int iPreVictoryNum = (int)(m_wWinPoint / 100);

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    if ( pLoseTeam )
        ++m_byTeamWinCount;

    WORD wPlusWinPoint = min( 100, 100 * wProcessTime / OnGetMaxPlayTimes() );
	m_wWinPoint += wPlusWinPoint;
	m_wWinPoint = min( MAX_WIN_POINT, m_wWinPoint );

	// m_wWinPoint 값이 100단위의 변화가 있어야 아이템을 받는다.
	int iVictoryNumDiff = m_wWinPoint / 100 - iPreVictoryNum;
	if( iVictoryNumDiff > 0 )
		bGetItem = TRUE;

	// 보상 처리
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;

		// PVP 해제
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// 죽었던 플레이어 부활
		if( pPlayer->IsDead() )
		{
			pPlayer->OnResurrection( 0, 1.f, 1.f );
		}

		// 보상 처리
		OnRewards( pPlayer, pLoseTeam->GetAvgLevel(), wProcessTime, bGetItem );

		// 경쟁 포인트 초기화
		pPlayer->GetCompetitionInfo().PointInit();

		// 승리 전적 추가
		pPlayer->GetCompetitionInfo().IncWinTimes();
	}

	// 연승횟수 증가 및 변수 초기화
	m_State = eTEAM_STATE_NONE;
	m_wKillPoint = 0;
	SetChampion( TRUE );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	m_bResultTie = FALSE;

	// 길드전은 한 판이 끝나면 다시 시작한다.
	if( m_pCHuntngMgr->IsGuildPVPMode() )
	{
		OnEnd();
	}
#endif

	// 30연승을 기록했을 때 강제로 추방시킨다.
	if( m_wWinPoint == MAX_WIN_POINT )
		OnEnd();
}

VOID CompetitionTeam::OnRewards( Player *pPlayer, LEVELTYPE EnemyPartyAvgLV, WORD wProcessTime, BOOL bGetItem )
{
	MSG_CG_CHUNTING_WIN_COMPETITION_BRD msg;

	memset( &msg.m_ItemInfo, 0, sizeof(INVENTORY_TOTAL_INFO) );

	EXPTYPE OldExp = pPlayer->GetExp();

	// 경험치 획득
	BYTE byMemberNum = GetMemberNum();
	if( !byMemberNum )
	{
		byMemberNum = m_byInitMemberNum;
	}

#ifdef __NA_0_20070329_CHUNTING_FORMULA_BUG
	EXPTYPE RewardExp = (DWORD)CalcCompetitionExp( m_Mode, wProcessTime, byMemberNum, pPlayer->GetLevel(), GetTotLevel(), GetAvgLevel(), EnemyPartyAvgLV, m_wWinPoint );
#else
	WORD RewardExp = (WORD)CalcCompetitionExp( m_Mode, wProcessTime, byMemberNum, pPlayer->GetLevel(), GetTotLevel(), GetAvgLevel(), EnemyPartyAvgLV, m_wWinPoint );
#endif

	pPlayer->AddExp( RewardExp, 0, 0, FALSE );

	// 하임 획득
	MONEY RewardMoney = CalcCompetitionMoney( m_Mode, wProcessTime, byMemberNum, pPlayer->GetLevel(), GetAvgLevel(), EnemyPartyAvgLV, m_wWinPoint );
	pPlayer->PlusMoney( RewardMoney );

	GAMELOG->LogExp( EXP_COMPETITION, pPlayer, pPlayer->GetExp(), OldExp );
	GAMELOG->LogMoney( MONEY_REWARD_COMPETITION, pPlayer, RewardMoney );

	// PVP 포인트 획득(팀승리 포인트)
	WORD wPVPPoint = GetMemberNum() * 3;

	// PVP 포인트 획득(개인등수 포인트)
	switch( pPlayer->GetCompetitionInfo().GetRanking() )
	{
	case 1:		wPVPPoint += GetMemberNum() * 3;	break;
	case 2:		wPVPPoint += GetMemberNum() * 2;	break;
	case 3:		wPVPPoint += GetMemberNum() * 1;	break;
	}

	pPlayer->GetPVPInfo().PlusPVPPoint( wPVPPoint );

	// 아이템 획득
	DWORD dwRewardIndex = 0;
	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone && bGetItem )
	{
		BYTE VictoryNum = (BYTE)(m_wWinPoint / 100);

		MAPCODE FieldCode = 0;	
		if( pPlayer->GetField() )	FieldCode = pPlayer->GetField()->GetFieldCode();

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		REWARD_INFO *pRewardInfo = CHuntingRewardParser::Instance()->GetRewardInfo( pZone->GetMapCode(), FieldCode, m_pCHuntngMgr->IsGuildPVPMode() , m_Rule, m_Mode, VictoryNum );
#else
		REWARD_INFO *pRewardInfo = CHuntingRewardParser::Instance()->GetRewardInfo( pZone->GetMapCode(), FieldCode, m_Rule, m_Mode, VictoryNum, 0 );
#endif
		if( pRewardInfo )
		{
			dwRewardIndex = CHuntingRewardParser::Instance()->GetRandomRewardIndex( pRewardInfo );
            uint index = RewardInfo::MAX_DROP_INDEX_NUM - 1;
            BYTE SelectItemBits = ( 1 << random(0,index) );
			g_RewardManager.RewardItem( RewardManager::eREWARD_COMPETITION, dwRewardIndex, SelectItemBits, pPlayer, msg.m_ItemInfo );
		}
	}

	// 승리 메세지를 날린다.
	msg.m_wWinPoint = m_wWinPoint;
	msg.m_i64Exp = pPlayer->GetExp();
	msg.m_CurMoney = pPlayer->GetMoney();
	msg.m_PlusPVPPoint = wPVPPoint;
	pPlayer->SendPacket( &msg, msg.GetSize() );
}

VOID CompetitionTeam::ResultLose( CompetitionTeam *pWinnerTeam )
{
    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    if ( pWinnerTeam )
        m_byTeamWinCount = 0;

	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;

		// PVP 해제
		pPlayer->GetPVPInfo().Clear();
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// PVP 포인트 획득(팀승리 포인트)
		WORD wPVPPoint = GetMemberNum();
		pPlayer->GetPVPInfo().PlusPVPPoint( wPVPPoint );

		// 패배 전적 추가
		pPlayer->GetCompetitionInfo().IncLoseTimes();

		// 패배 메세지를 날린다.
		MSG_CG_CHUNTING_LOSE_COMPETITION_BRD msg;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	m_wKillPoint = 0;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	m_bResultTie = FALSE;
#endif

	OnEnd();
}

VOID CompetitionTeam::IncNPCKillPoint( Player *pKiller, NPC *pNPC )
{
	if( IsTimeAttackMode() )
	{
		WORD wBonusKillPoint = CalcNPCKillPoint( (eNPC_GRADE)pNPC->GetGrade() );
		m_wKillPoint += wBonusKillPoint;

		// 개인 킬포인트 증가
		pKiller->GetCompetitionInfo().IncKill( wBonusKillPoint );

		// 킬포인트 갱신을 알린다.
		MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD brdMsg;
		brdMsg.m_wTeamKey = m_wTeamKey;
		brdMsg.m_wKillPoint = m_wKillPoint;
		m_pCHuntngMgr->SendPacketToAll( &brdMsg, sizeof(brdMsg) );
	}
}

VOID CompetitionTeam::IncPlayerKillPoint( Player *pKiller, Player *pDeadPlayer )
{
	// PVP 킬수 증가
	pKiller->GetPVPInfo().IncKill();

	// die수 증가
	pDeadPlayer->GetPVPInfo().IncDie();
	pDeadPlayer->GetCompetitionInfo().IncDie();

	if( IsSurvivalMode() || IsDeathMatchMode() )
	{
		int iLevelDiff = pDeadPlayer->GetLevel() - pKiller->GetLevel();

		WORD wBonusKillPoint = CalcPlayerKillPoint( iLevelDiff );
		m_wKillPoint += wBonusKillPoint;

		// 개인 킬포인트 증가
		pKiller->GetCompetitionInfo().IncKill( wBonusKillPoint );

		// 킬포인트 갱신을 알린다.
		MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD brdMsg;
		brdMsg.m_wTeamKey = m_wTeamKey;
		brdMsg.m_wKillPoint = m_wKillPoint;
		m_pCHuntngMgr->SendPacketToAll( &brdMsg, sizeof(brdMsg) );
	}
	else if( IsTimeAttackMode() )
	{
		// 연승 콤포 증가
		pKiller->GetCompetitionInfo().IncKill( 0 );
	}
}

VOID CompetitionTeam::UsePotion()
{
}

VOID CompetitionTeam::AddMember( Player *pPlayer )
{
	if( !(m_PlayerHash.size() < CHUNTING_MAX_MEMBER_NUM) )
	{
		SUNLOG( eMIDDLE_LOG, "[CompetitionTeam::AddMember] Player Num Access!" );
		return;
	}

	DWORD dwObjectKey = pPlayer->GetObjectKey();
	Player *pMember = FindMember( dwObjectKey );
	if( pMember )
	{
		SUNLOG( eMIDDLE_LOG, "[CompetitionTeam::AddMember] Player already exists" );
		return;
	}

	m_PlayerHash.insert( std::make_pair( dwObjectKey, pPlayer ) );

	// 플레이어의 경쟁인포 초기화 및 키 셋팅
	pPlayer->GetCompetitionInfo().Init();
	pPlayer->GetCompetitionInfo().SetTeamKey( m_wTeamKey );

	// 초기 멤버의 레벨, 수
	m_TotLevel += pPlayer->GetLevel();
	m_byInitMemberNum = (BYTE)m_PlayerHash.size();

	// 팀정보 Send
	MSG_CG_CHUNTING_TEAM_INFO_CMD cmdMsg;
	cmdMsg.m_wTeamKey = m_wTeamKey;
	pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

	m_setReadyUserKeys.erase( pPlayer->GetUserKey() );
}

VOID CompetitionTeam::RemoveMember( DWORD dwObjKey )
{
	Player *pMember = FindMember( dwObjKey );
	if( !pMember )
	{
		SUNLOG( eMIDDLE_LOG, "[CompetitionTeam::RemoveMember] Player doesn't exist" );
		return;
	}

	m_PlayerHash.erase( dwObjKey );
	// 플레이어의 경쟁인포 초기화
	pMember->GetCompetitionInfo().Init();

	m_setReadyUserKeys.erase( pMember->GetUserKey() );
}

Player*		CompetitionTeam::FindMember( DWORD dwObjKey )
{
	PLAYER_HASH_ITR itFIND = m_PlayerHash.find(dwObjKey);
	Player* pPlayer = itFIND != m_PlayerHash.end() ? itFIND->second : NULL;
	return pPlayer;
}

BYTE		CompetitionTeam::GetMemberNum()
{
	return (BYTE)m_PlayerHash.size();
}

BYTE CompetitionTeam::GetAliveMemberNum()
{
	BYTE byAliveNum = 0;

	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;
		if( pPlayer->IsAlive() )
			++byAliveNum;
	}

	return byAliveNum;
}

void CompetitionTeam::SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size, DWORD sender_object_key)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER::InstanceOfBase();
    msg_brd.Init(msg, msg_size);
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();

    const BOOLEAN check_skip_object = !sender_object_key;
    FOREACH_CONTAINER(const PLAYER_HASH::value_type& node, m_PlayerHash, PLAYER_HASH)
    {
        Player* const player = node.second;
        const DWORD object_key = player->GetObjectKey();
        if (!(check_skip_object || (object_key != sender_object_key))) {
            continue;
        };

        DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
        __TOUCHED(bound_check); //assert(bound_check != 0);
    };

    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

BYTE CompetitionTeam::GetAllPointInfo( CompetitionPoint * OUT pPointInfo )
{
	BYTE byCount = 0;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		Player* pPlayer = it->second;
		pPointInfo[byCount].m_dwObjKey = pPlayer->GetObjectKey();
		pPointInfo[byCount].m_wKillPoint = pPlayer->GetCompetitionInfo().GetKillPoint();
		pPointInfo[byCount].m_byDieNum = pPlayer->GetCompetitionInfo().GetDieNum();
		pPointInfo[byCount].m_byCombo = pPlayer->GetCompetitionInfo().GetMaxSeries();

		byCount++;
	}

	return byCount;
}


class KillPointGreater
{
public:
	bool operator () ( Player *pLeft, Player *pRight ) const
	{
		return pLeft->GetCompetitionInfo().GetKillPoint() > pRight->GetCompetitionInfo().GetKillPoint();
	}
};

VOID CompetitionTeam::OnCalcRanking()
{
	static PLAYER_SORT_VECTOR m_vecSortPlayer;
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;
		m_vecSortPlayer.push_back( pPlayer );
	}

	BYTE bySortCount = min( 3, (BYTE)m_PlayerHash.size() );
	partial_sort( m_vecSortPlayer.begin(), m_vecSortPlayer.begin()+bySortCount, m_vecSortPlayer.end(), KillPointGreater() );

	int iRanking = -1;		WORD wPreKillPoint = 0;		
	PLAYER_SORT_VECTOR_ITR endit = m_vecSortPlayer.begin()+bySortCount;

	for( PLAYER_SORT_VECTOR_ITR vit = m_vecSortPlayer.begin(); vit != endit; ++vit )
	{
		pPlayer = *vit;

		// 이전 킬포인트 보다 낮으면 랭킹이 내려간다.(동률일 경우를 위한 코드)
		if( wPreKillPoint > pPlayer->GetCompetitionInfo().GetKillPoint() )
		{
			++iRanking;
			wPreKillPoint = pPlayer->GetCompetitionInfo().GetKillPoint();
		}

		pPlayer->GetCompetitionInfo().SetRanking( max(0, iRanking) );
	}
}

VOID CompetitionTeam::AllResurrectionAtSafezone()
{
	const sMAPINFO *psMAPINFO = MapInfoParser::Instance()->FindMapInfo( m_pCHuntngMgr->GetCHunting()->GetMapCode() );
	if( !psMAPINFO )
	{
		SUNLOG( eCRITICAL_LOG, "[AllResurrection] psMAPINFO is NULL" );
		return;
	}

	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		Player* pPlayer = it->second;
		ResurrectionAtSafezone( pPlayer, FALSE );
	}
}

RC::eSTATUS_RESULT CompetitionTeam::ResurrectionAtSafezone( Player *pPlayer, BOOL bModeCheck )
{
	GameField *pField = pPlayer->GetField();
	if( !pField )
		return RC::RC_STATUS_FIELD_IS_NULL;

	// Mode 체크( 경쟁모드에 따라서 부활이 가능할 때도 있고, 불가능할 때도 있다. )
	if( bModeCheck )
	{
		if( IsCompetition() )
		{
			// 경쟁중일 때는 타임어택에서만 부활위치에서 부활할 수 있다.
			if( !IsTimeAttackMode() )
				return RC::RC_STATUS_RESURRECTION_FAIL;
		}
	}

	// SafeZone으로 이동한다.
	RC::eSTATUS_RESULT rcResult = OnMoveToSafezone( pPlayer );
	if( rcResult != RC::RC_STATUS_SUCCESS )
		return rcResult;

	//죽은상태일 때만 부활한다.
	if( pPlayer->IsDead() )
	{
		if( !pPlayer->OnResurrection( 0, 0.15f, 0.15f ) )
		{
			return RC::RC_STATUS_RESURRECTION_FAIL;
		}
	}

	return RC::RC_STATUS_SUCCESS;
}

RC::eSTATUS_RESULT CompetitionTeam::OnMoveToSafezone( Player *pPlayer )
{
	GameField *pField = pPlayer->GetField();
	if( !pField )
		return RC::RC_STATUS_FIELD_IS_NULL;

	const sMAPINFO *psMAPINFO = MapInfoParser::Instance()->FindMapInfo( m_pCHuntngMgr->GetCHunting()->GetMapCode() );
	if( !psMAPINFO )
	{
		SUNLOG( eCRITICAL_LOG, "[CompetitionTeam::MoveToSafezone] psMAPINFO is NULL" );
		return RC::RC_STATUS_MAPINFO_IS_NULL;
	}

	DWORD dwAreaID = 0;
	if( m_bChampion )	dwAreaID = psMAPINFO->wStartAreaID;
	else				dwAreaID = psMAPINFO->wStartAreaID2;

	WzVector vDestPos;
	if( !pField->GetAreaRandomStartingVector( dwAreaID, vDestPos ) )
		return RC::RC_STATUS_IS_NOT_SAFEZONE;

	// 시작위치로 텔레포트 시킨다.
	if( !pField->TeleportObject( pPlayer, &vDestPos, TRUE ) )
		return RC::RC_STATUS_TELEPORT_FAIL;

	// 강제워프시 잠시 다른 이동을 막는다.
	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	pPlayer->GetStatusManager()->AllocStatus( eCHAR_STATE_ETC_FORCED_WARP, stServerOptionInfo.m_wFORCED_WARP_MOVE_DELAY );

	return RC::RC_STATUS_SUCCESS;
}

LEVELTYPE CompetitionTeam::GetAvgLevel()
{
	BYTE byMemberNum = GetMemberNum();
	if( !byMemberNum )
	{
		byMemberNum = m_byInitMemberNum;
	}

	LEVELTYPE AvgLevel = 0;
	if( byMemberNum )
		AvgLevel = GetTotLevel() / byMemberNum;
	else
		AvgLevel = 1;

	AvgLevel = max( 1, AvgLevel );

	return AvgLevel;
}

LEVELTYPE CompetitionTeam::GetTotLevel()
{
	// 팀원이 모두 나갔을 경우 TotLevel은 초기 레벨값을 참조한다.
	if( !GetMemberNum() )
	{
		return m_TotLevel;
	}

	LEVELTYPE TotLevel = 0;

	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		Player* pPlayer = it->second;
		TotLevel += pPlayer->GetLevel();
	}

	return TotLevel;
}

// GM 명령어 용도
VOID CompetitionTeam::IncWinCount( Player * pPlayer, int iIncVCount )
{
	// iIncVCount가 0 이상 MAX_VICTORY_NUM 값을 넘지 않도록 수치를 보정한다.
	iIncVCount = max( 0, iIncVCount );
	WORD wWinPoint = m_wWinPoint + iIncVCount*100;
	wWinPoint = min( MAX_WIN_POINT, wWinPoint );
	iIncVCount = (wWinPoint - m_wWinPoint) / 100;

	m_wWinPoint += iIncVCount * 100;

	// 승리 전적 추가
	for( int i = 0; i < iIncVCount; ++i )
	{
		pPlayer->GetCompetitionInfo().IncPlayTimes();
		pPlayer->GetCompetitionInfo().IncWinTimes();
	}

	MSG_CG_CHUNTING_WIN_COMPETITION_BRD msg;

	memset( &msg.m_ItemInfo, 0, sizeof(INVENTORY_TOTAL_INFO) );

	// 아이템 획득, 사용가능한 물약개수 증가
	DWORD dwRewardIndex = 0;
	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		BYTE VictoryNum = (BYTE)(m_wWinPoint / 100);

		MAPCODE FieldCode = 0;	
		if( pPlayer->GetField() )	FieldCode = pPlayer->GetField()->GetFieldCode();
		
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		REWARD_INFO *pRewardInfo = CHuntingRewardParser::Instance()->GetRewardInfo( pZone->GetMapCode(), FieldCode, m_pCHuntngMgr->IsGuildPVPMode() , m_Rule, m_Mode, VictoryNum );
#else
		REWARD_INFO *pRewardInfo = CHuntingRewardParser::Instance()->GetRewardInfo( pZone->GetMapCode(), FieldCode, m_Rule, m_Mode, VictoryNum, 0 );
#endif
		if( pRewardInfo )
		{
			dwRewardIndex = CHuntingRewardParser::Instance()->GetRandomRewardIndex( pRewardInfo );

			BYTE SelectItemBits = ( 1 << random(0,3) );

			g_RewardManager.RewardItem( RewardManager::eREWARD_COMPETITION, dwRewardIndex, SelectItemBits, pPlayer, msg.m_ItemInfo );
		}
	}

	msg.m_wWinPoint = m_wWinPoint;
	msg.m_i64Exp = pPlayer->GetExp();
	msg.m_CurMoney = pPlayer->GetMoney();
	msg.m_PlusPVPPoint = 0;
	pPlayer->SendPacket( &msg, msg.GetSize() );
}

WORD CompetitionTeam::OnGetMaxPlayTimes()
{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	return m_pCHuntngMgr->OnGetMaxPlayTimes();
#else

	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	if( IsTimeAttackMode() )
		return (WORD)(stServerOptionInfo.m_dwTIMEATTACK_PLAY_TIME/1000);
	else
		return (WORD)(stServerOptionInfo.m_dwSURVIVAL_PLAY_TIME/1000);
#endif
}

VOID CompetitionTeam::SetReadyPlayer( BYTE byReadyPlayerNum, DWORD * IN pUserKeyList )
{
    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_byTeamWinCount = 0;            // 연승 횟수

	m_byReadyPlayerNum = byReadyPlayerNum;
	m_setReadyUserKeys.clear();

	for( int i = 0; i < byReadyPlayerNum; ++i )
	{
		m_setReadyUserKeys.insert( pUserKeyList[i] );
	}
}

BOOL CompetitionTeam::IsAllPlayerEntered()
{
	if( m_setReadyUserKeys.size() == 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL CompetitionTeam::IsReadyPlayer( DWORD UserKey )
{
	STLX_SET<DWORD>::iterator it = m_setReadyUserKeys.find( UserKey );
	if( it != m_setReadyUserKeys.end() )
		return TRUE;

	return FALSE;
}

























