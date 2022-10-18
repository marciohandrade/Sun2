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
	// �Ҹ���Ѿ��� ����� ����
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

		// �������� ����Ƚ���� ������Ų��.
		pPlayer->GetCompetitionInfo().IncPlayTimes();

		if( !IsTimeAttackMode() )
		{
			// �� ��Ƽ������� PVP�� �����ϵ��� ���¸� �����Ѵ�.
			pPlayer->GetPVPInfo().Clear();
			pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_MATCH );
		}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		// ���� ���� ���ºο����� ������� �Ҹ��Ű�� �ʴ´�.
		if( !m_pCHuntngMgr->IsGuildPVPMode() || m_bResultTie )	continue;


		// �ʿ��� ������ŭ ������� ������ ������ �Ҹ��Ű�� ������ �����Ų��.
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
		// ������ �����Ų��.
		else
		{
			OnExpel( pPlayer );
		}
#endif
	}
}

BOOL CompetitionTeam::OnIsExpired()
{
	// ������� 0�̸� ����
	if( m_PlayerHash.size() == 0 )
	{
		m_eExpireState = eEXPIRE_STATE_ALL_LEAVE;
		return TRUE;
	}

	if( IsSurvivalMode() )
	{
		// ������� ��� ������ ����
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

	// �����ð� ��ٸ� �Ŀ� �߹��Ų��.
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
	// ������ �߹��Ų��.
	Player * pPlayer = NULL;
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != m_PlayerHash.end(); ++it )
	{
	#ifndef __TW_0_20080521_GUILD_PVP_BUG_FIX	//�ڷᱸ�� ����Ǿ���.
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

	// ������ �߹��Ų��.
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
		// PVP ����
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// �׾��� �÷��̾� ��Ȱ
		if( pPlayer->IsDead() )
		{
			pPlayer->OnResurrection( 0, 1.f, 1.f );
		}

		// ���� ����Ʈ �ʱ�ȭ
		pPlayer->GetCompetitionInfo().PointInit();

		// ���º� �޼����� ������.
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
	// ���� ��� ���ϱ�
	OnCalcRanking();

	BOOL bGetItem = FALSE;

	int iPreVictoryNum = (int)(m_wWinPoint / 100);

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    if ( pLoseTeam )
        ++m_byTeamWinCount;

    WORD wPlusWinPoint = min( 100, 100 * wProcessTime / OnGetMaxPlayTimes() );
	m_wWinPoint += wPlusWinPoint;
	m_wWinPoint = min( MAX_WIN_POINT, m_wWinPoint );

	// m_wWinPoint ���� 100������ ��ȭ�� �־�� �������� �޴´�.
	int iVictoryNumDiff = m_wWinPoint / 100 - iPreVictoryNum;
	if( iVictoryNumDiff > 0 )
		bGetItem = TRUE;

	// ���� ó��
	Player * pPlayer = NULL;
	PLAYER_HASH_ITR itend = m_PlayerHash.end();
	for( PLAYER_HASH_ITR it = m_PlayerHash.begin(); it != itend ; ++it )
	{
		pPlayer = it->second;

		// PVP ����
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// �׾��� �÷��̾� ��Ȱ
		if( pPlayer->IsDead() )
		{
			pPlayer->OnResurrection( 0, 1.f, 1.f );
		}

		// ���� ó��
		OnRewards( pPlayer, pLoseTeam->GetAvgLevel(), wProcessTime, bGetItem );

		// ���� ����Ʈ �ʱ�ȭ
		pPlayer->GetCompetitionInfo().PointInit();

		// �¸� ���� �߰�
		pPlayer->GetCompetitionInfo().IncWinTimes();
	}

	// ����Ƚ�� ���� �� ���� �ʱ�ȭ
	m_State = eTEAM_STATE_NONE;
	m_wKillPoint = 0;
	SetChampion( TRUE );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	m_bResultTie = FALSE;

	// ������� �� ���� ������ �ٽ� �����Ѵ�.
	if( m_pCHuntngMgr->IsGuildPVPMode() )
	{
		OnEnd();
	}
#endif

	// 30������ ������� �� ������ �߹��Ų��.
	if( m_wWinPoint == MAX_WIN_POINT )
		OnEnd();
}

VOID CompetitionTeam::OnRewards( Player *pPlayer, LEVELTYPE EnemyPartyAvgLV, WORD wProcessTime, BOOL bGetItem )
{
	MSG_CG_CHUNTING_WIN_COMPETITION_BRD msg;

	memset( &msg.m_ItemInfo, 0, sizeof(INVENTORY_TOTAL_INFO) );

	EXPTYPE OldExp = pPlayer->GetExp();

	// ����ġ ȹ��
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

	// ���� ȹ��
	MONEY RewardMoney = CalcCompetitionMoney( m_Mode, wProcessTime, byMemberNum, pPlayer->GetLevel(), GetAvgLevel(), EnemyPartyAvgLV, m_wWinPoint );
	pPlayer->PlusMoney( RewardMoney );

	GAMELOG->LogExp( EXP_COMPETITION, pPlayer, pPlayer->GetExp(), OldExp );
	GAMELOG->LogMoney( MONEY_REWARD_COMPETITION, pPlayer, RewardMoney );

	// PVP ����Ʈ ȹ��(���¸� ����Ʈ)
	WORD wPVPPoint = GetMemberNum() * 3;

	// PVP ����Ʈ ȹ��(���ε�� ����Ʈ)
	switch( pPlayer->GetCompetitionInfo().GetRanking() )
	{
	case 1:		wPVPPoint += GetMemberNum() * 3;	break;
	case 2:		wPVPPoint += GetMemberNum() * 2;	break;
	case 3:		wPVPPoint += GetMemberNum() * 1;	break;
	}

	pPlayer->GetPVPInfo().PlusPVPPoint( wPVPPoint );

	// ������ ȹ��
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

	// �¸� �޼����� ������.
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

		// PVP ����
		pPlayer->GetPVPInfo().Clear();
		pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

		// PVP ����Ʈ ȹ��(���¸� ����Ʈ)
		WORD wPVPPoint = GetMemberNum();
		pPlayer->GetPVPInfo().PlusPVPPoint( wPVPPoint );

		// �й� ���� �߰�
		pPlayer->GetCompetitionInfo().IncLoseTimes();

		// �й� �޼����� ������.
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

		// ���� ų����Ʈ ����
		pKiller->GetCompetitionInfo().IncKill( wBonusKillPoint );

		// ų����Ʈ ������ �˸���.
		MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD brdMsg;
		brdMsg.m_wTeamKey = m_wTeamKey;
		brdMsg.m_wKillPoint = m_wKillPoint;
		m_pCHuntngMgr->SendPacketToAll( &brdMsg, sizeof(brdMsg) );
	}
}

VOID CompetitionTeam::IncPlayerKillPoint( Player *pKiller, Player *pDeadPlayer )
{
	// PVP ų�� ����
	pKiller->GetPVPInfo().IncKill();

	// die�� ����
	pDeadPlayer->GetPVPInfo().IncDie();
	pDeadPlayer->GetCompetitionInfo().IncDie();

	if( IsSurvivalMode() || IsDeathMatchMode() )
	{
		int iLevelDiff = pDeadPlayer->GetLevel() - pKiller->GetLevel();

		WORD wBonusKillPoint = CalcPlayerKillPoint( iLevelDiff );
		m_wKillPoint += wBonusKillPoint;

		// ���� ų����Ʈ ����
		pKiller->GetCompetitionInfo().IncKill( wBonusKillPoint );

		// ų����Ʈ ������ �˸���.
		MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD brdMsg;
		brdMsg.m_wTeamKey = m_wTeamKey;
		brdMsg.m_wKillPoint = m_wKillPoint;
		m_pCHuntngMgr->SendPacketToAll( &brdMsg, sizeof(brdMsg) );
	}
	else if( IsTimeAttackMode() )
	{
		// ���� ���� ����
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

	// �÷��̾��� �������� �ʱ�ȭ �� Ű ����
	pPlayer->GetCompetitionInfo().Init();
	pPlayer->GetCompetitionInfo().SetTeamKey( m_wTeamKey );

	// �ʱ� ����� ����, ��
	m_TotLevel += pPlayer->GetLevel();
	m_byInitMemberNum = (BYTE)m_PlayerHash.size();

	// ������ Send
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
	// �÷��̾��� �������� �ʱ�ȭ
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

		// ���� ų����Ʈ ���� ������ ��ŷ�� ��������.(������ ��츦 ���� �ڵ�)
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

	// Mode üũ( �����忡 ���� ��Ȱ�� ������ ���� �ְ�, �Ұ����� ���� �ִ�. )
	if( bModeCheck )
	{
		if( IsCompetition() )
		{
			// �������� ���� Ÿ�Ӿ��ÿ����� ��Ȱ��ġ���� ��Ȱ�� �� �ִ�.
			if( !IsTimeAttackMode() )
				return RC::RC_STATUS_RESURRECTION_FAIL;
		}
	}

	// SafeZone���� �̵��Ѵ�.
	RC::eSTATUS_RESULT rcResult = OnMoveToSafezone( pPlayer );
	if( rcResult != RC::RC_STATUS_SUCCESS )
		return rcResult;

	//���������� ���� ��Ȱ�Ѵ�.
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

	// ������ġ�� �ڷ���Ʈ ��Ų��.
	if( !pField->TeleportObject( pPlayer, &vDestPos, TRUE ) )
		return RC::RC_STATUS_TELEPORT_FAIL;

	// ���������� ��� �ٸ� �̵��� ���´�.
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
	// ������ ��� ������ ��� TotLevel�� �ʱ� �������� �����Ѵ�.
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

// GM ��ɾ� �뵵
VOID CompetitionTeam::IncWinCount( Player * pPlayer, int iIncVCount )
{
	// iIncVCount�� 0 �̻� MAX_VICTORY_NUM ���� ���� �ʵ��� ��ġ�� �����Ѵ�.
	iIncVCount = max( 0, iIncVCount );
	WORD wWinPoint = m_wWinPoint + iIncVCount*100;
	wWinPoint = min( MAX_WIN_POINT, wWinPoint );
	iIncVCount = (wWinPoint - m_wWinPoint) / 100;

	m_wWinPoint += iIncVCount * 100;

	// �¸� ���� �߰�
	for( int i = 0; i < iIncVCount; ++i )
	{
		pPlayer->GetCompetitionInfo().IncPlayTimes();
		pPlayer->GetCompetitionInfo().IncWinTimes();
	}

	MSG_CG_CHUNTING_WIN_COMPETITION_BRD msg;

	memset( &msg.m_ItemInfo, 0, sizeof(INVENTORY_TOTAL_INFO) );

	// ������ ȹ��, ��밡���� ���ళ�� ����
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
    m_byTeamWinCount = 0;            // ���� Ƚ��

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

























