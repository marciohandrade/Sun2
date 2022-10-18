#include "StdAfx.h"
#include "CHuntingManager.h"
#include "PVPInfo.h"
#include "Summoned.h"
#include "PacketStruct_AG.h"
#include "GameServerEx.h"
#include "GameCHuntingRoom.h"
#include "CompetitionInfo.h"
#include "ServerOptionParser.h"
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	#include "PacketStruct_GZ.h"
#endif
#include "GameServerEx.h"
#include <ServerOptionParserEx.h>

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "CHuntingBattleResult.h"

CHuntingManager::CHuntingManager()
{
	m_CompetionKeyGenerater.Create( 1, MAX_COMPETITION_INFO_NUM );
	m_CompetionTeamPool.Initialize( MAX_COMPETITION_INFO_NUM, 0, "m_CompetionTeamPool", AssertionLogger4Pool_Server );
	m_CompetionTeamHash.Initialize( MAX_COMPETITION_INFO_NUM );
	m_TempTeamHash.Initialize( MAX_COMPETITION_INFO_NUM );
}

CHuntingManager::~CHuntingManager()
{
	m_CompetionTeamPool.Release();
}

VOID CHuntingManager::Release()
{
	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		m_CompetionTeamPool.Free( pTeam );
		m_CompetionKeyGenerater.RestoreKey( pTeam->GetTeamKey() );
	}
	m_CompetionTeamHash.RemoveAll();
}

VOID CHuntingManager::Init( GameCHuntingRoom *pCHuntingZone, ADDITIONAL_CHUNTINGINFO * pInfo )
{
	m_pCHuntingZone = pCHuntingZone;
	m_pAdditionalInfo = pInfo;
	m_eCompetitionState = eCOMPETITION_NONE;
	m_WinnerKey = 0;
	m_TimeExpired = FALSE;
}

VOID CHuntingManager::Update()
{
	if( m_CompetionTeamHash.GetDataNum() > 2 )
	{
		static BOOL bLog = FALSE;

		if( !bLog )
		{
			SUNLOG( eCRITICAL_LOG, "[CHuntingManager] m_CompetionTeamHash Count[%d] Access!! Rule[%d]", 
				m_CompetionTeamHash.GetDataNum(), m_pAdditionalInfo->m_Rule );
			bLog = TRUE;
		}
	}

	CompetitionTeam *pTeam; 
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		pTeam->Update();
	}

    switch( m_eCompetitionState ) 
	{
	case eCOMPETITION_ALL_ENTER_WAIT:
		{
			// ���� ����� �� ������
			if( IsAllPlayerEntered() )
			{
				OnStartWait();
				break;
			}

			// �����ð� ��ٷ��� ������ �ʴ°��
			if( m_StartWaitTimer.IsExpiredManual( FALSE ) )
			{
				CompetitionTeam *pHeadTeam = m_CompetionTeamHash.GetHeadData();
				CompetitionTeam *pTailTeam = m_CompetionTeamHash.GetTailData();
				SUNLOG( eMIDDLE_LOG, "[CHuntingManager::Update] is not AllPlayerEntered! PlayerNum[%d:%d]", pHeadTeam->GetMemberNum(), pTailTeam->GetMemberNum() );

				// �ι�° ���� �ο��� �Ѹ��̶� ������ ��ŸƮ ��Ų��.
				if( pTailTeam->GetMemberNum() )
				{
					OnStartWait();
				}
				else
				{
					m_WinnerKey = pHeadTeam->GetTeamKey();
					m_eCompetitionState = eCOMPETITION_END_WAIT;
				}
			}
		}
		break;
	case eCOMPETITION_START_WAIT:
		{
			// 20�ʰ� ����� �������� ��ŸƮ!
			if( m_StartWaitTimer.IsExpiredManual( FALSE ) )
			{
				OnStart();
                m_eCompetitionState = eCOMPETITION_MATCH;
			}
		}
		break;
	case eCOMPETITION_MATCH:
		{
			// �ð��� �� �ǰų� ���������� �Ǹ� ���ó���Ѵ�..
			if( IsExpired() )
			{
				OnResult();
				m_eCompetitionState = eCOMPETITION_END_WAIT;
			}

			// ������ġ���� ������� ��� ������ ������ġ���� ��Ȱ
			if( IsReMatchCondition() )
			{
				for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
				{
					pTeam = *it;
					pTeam->AllResurrectionAtSafezone();
				}
			}
		}
		break;
	case eCOMPETITION_END_WAIT:
		{
			if( IsAllLoseTeamLeft() )
			{
				m_eCompetitionState = eCOMPETITION_NONE;

				// ������ �Ϸ�Ǿ����� Agent�� �˸���.
				MSG_AG_ZONE_CHUNTING_COMPETITION_END_CMD msg;
				msg.m_RoomKey = m_pCHuntingZone->GetKey();
				g_pGameServer->SendToServer( AGENT_SERVER, &msg, sizeof(msg) );

				// �� ���� ��ü�Ѵ�.
				CompetitionTeam *pLoseTeam;
				for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end();  )
				{
					pLoseTeam = *it;
					if( pLoseTeam->GetTeamKey() == m_WinnerKey )	
                    {
                        ++it;
                    }
                    else
                    {
                        m_CompetionTeamPool.Free( pLoseTeam );
                        m_CompetionKeyGenerater.RestoreKey( pLoseTeam->GetTeamKey() );
                        it = m_CompetionTeamHash.erase(it);
                    }
				}
			}
			m_TimeExpired = FALSE;
		}
		break;
	}
}

VOID CHuntingManager::OnStartWait()
{
	// 20�� ���ð� ī��Ʈ�� �����Ѵ�.
	m_eCompetitionState = eCOMPETITION_START_WAIT;
	m_StartWaitTimer.SetTimer( START_WAIT_TIME );

	if( IsDualPartyRule() )
	{
		// ���ο� ��Ƽ�� ���������� �˸���.
		MSG_CG_CHUNTING_ENTER_NEW_PARTY_BRD brdMsg;
		SendPacketToAll( &brdMsg, sizeof(brdMsg) );

		// ���� �������� ������ġ�� �̵���Ų��.
		CompetitionTeam *pTeam = m_CompetionTeamHash.GetHeadData();
		if( pTeam )		pTeam->AllResurrectionAtSafezone();
	}

    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    if( m_pCHuntingZone )
    {
        GameBattleResult* pResult = m_pCHuntingZone->GetGameBattleResult();
        if( pResult )
            pResult->Clear();
    }
}

VOID CHuntingManager::OnStart()
{
	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		pTeam->Start();
	}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	if( IsTimeAttackMode() )
	{
		if( IsGuildPVPMode() )
//			m_ExpireTimer.SetTimer( 120*1000 );
			m_ExpireTimer.SetTimer( CHUNTING_TIMEATTACK_EXPIRE_TIME_GUILD_PVP );
		else
			m_ExpireTimer.SetTimer( CHUNTING_TIMEATTACK_EXPIRE_TIME );
	}
	else if( IsSurvivalMode() || IsDeathMatchMode() )
	{
		if( IsGuildPVPMode() )
//			m_ExpireTimer.SetTimer( 120*1000 );
			m_ExpireTimer.SetTimer( CHUNTING_SURVIVAL_EXPIRE_TIME_GUILD_PVP );
		else
			m_ExpireTimer.SetTimer( CHUNTING_SURVIVAL_EXPIRE_TIME );
	}
#else

	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	// Ÿ�Ӿ��� ����� ��� �ð� ��ŸƮ
	if( IsTimeAttackMode() )
	{
		m_ExpireTimer.SetTimer( stServerOptionInfo.m_dwTIMEATTACK_PLAY_TIME );
	}
	else if( IsSurvivalMode() || IsDeathMatchMode() )
	{
		m_ExpireTimer.SetTimer( stServerOptionInfo.m_dwSURVIVAL_PLAY_TIME );
	}
#endif
}

BOOL CHuntingManager::IsExpired()
{
	// Ÿ�Ӿ��� �ð�����
	if( m_ExpireTimer.IsExpiredManual( FALSE ) )
	{
		m_TimeExpired = TRUE;
		return TRUE;
	}

	// �� ���ܿ� ��� ���������� �Ǹ� ��ü����
	BYTE byExpireCount = 0;
	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		if( !pTeam->IsExpired() )
			byExpireCount++;
	}

	if( byExpireCount <= 1 )
	{
		m_TimeExpired = FALSE;
		return TRUE;
	}

	return FALSE;
}

BOOL CHuntingManager::IsReMatchCondition()
{
	if( !IsDeathMatchMode() )
		return FALSE;

	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		if( pTeam->IsAllDied() )
			return TRUE;
	}
	return FALSE;
}

CompetitionTeam * CHuntingManager::DecideTie()
{
	// �� ���ܿ� ��� ������ ���� ���� �̱�ų� �ּ��� ���º�!
	BYTE byRemainCount = 0;
	CompetitionTeam *pRemainTeam = NULL;
	CompetitionTeam *pTeam = NULL;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		if( pTeam->GetExpireState() != eEXPIRE_STATE_ALL_LEAVE )
		{
			byRemainCount++;
			pRemainTeam = pTeam;
		}
	}

	// ���� ������ ų����Ʈ�� ���� ���� �ʴٸ� ���º��̴�.
	if( byRemainCount == 1 )
	{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		// ����������� 20���� ä���� ������ ������ ���ºΰ� �ȴ�.
		if( IsGuildPVPMode() )
		{
			if( !m_TimeExpired )
				return pRemainTeam;
		}
#endif

		pTeam = DecideKillPointWinner( &m_CompetionTeamHash );
		if( !pTeam )	
		{
			// ų����Ʈ ���ڰ� ��� ���º�
			return pRemainTeam;
		}

		if( pRemainTeam->GetKillPoint() < pTeam->GetKillPoint() )
		{
			// ���� ���� ų����Ʈ ���ڰ� �ƴ� ��� ���º�
			return pRemainTeam;
		}
	}

	return NULL;
}

VOID CHuntingManager::OnResult()
{
	// ���º��ΰ�?
	if( CompetitionTeam *pRemainTeam = DecideTie() )
	{
		if( pRemainTeam )
		{
			m_WinnerKey = pRemainTeam->GetTeamKey();

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
			for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
			{
				CompetitionTeam *pTeam = *it;
				pTeam->ResultTie();

				// �𽺿� ���� ���縦 ���Ѵ�.
				if( IsGuildPVPMode() && !pTeam->GetMemberNum() )
				{
					// ��� ������ �𽺸� �˸���.
					MSG_GZ_GUILD_CHUNTING_PVP_RESULT_CMD cmdMsg;
					cmdMsg.m_WinGuildGuid = pRemainTeam->GetGuildGuid();
					cmdMsg.m_LoseGuildGuid = 0;
					cmdMsg.m_DisGuildGuid = pTeam->GetGuildGuid();
					g_pGameServer->SendToServer( GUILD_SERVER, &cmdMsg, sizeof(cmdMsg) );
				}
			}
#else
			pRemainTeam->ResultTie();
#endif
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "[CHuntingManager::OnResult] pRemainTeam is NULL" );
		}
		return;
	}

	// �¸��� ��(����)�� �����Ѵ�.
	CompetitionTeam *pWinTeam = DecideWinner();
	if( !pWinTeam )	return;
	m_WinnerKey = pWinTeam->GetTeamKey();

	// �й��� ������ ���ó���� �Ѵ�.
	CompetitionTeam *pLoseTeam = NULL;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		CompetitionTeam *pTeam = *it;
		if( pTeam->GetTeamKey() == m_WinnerKey )	continue;

		pLoseTeam = pTeam;
		pLoseTeam->ResultLose( pWinTeam );
	}

	if( NULL == pLoseTeam )
	{
		SUNLOG(eCRITICAL_LOG, "LoseTeam is not Exist!!");
		return;
	}

	// �¸��� ���� ���ó���� �Ѵ�.
	pWinTeam->ResultWin( pLoseTeam, (WORD)(m_ExpireTimer.GetProcessTime()/1000) );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	if( IsGuildPVPMode() )
	{
		// ��� ������ ����� �˸���.
		MSG_GZ_GUILD_CHUNTING_PVP_RESULT_CMD cmdMsg;
		cmdMsg.m_WinGuildGuid = pWinTeam->GetGuildGuid();
		cmdMsg.m_LoseGuildGuid = pLoseTeam->GetGuildGuid();
		cmdMsg.m_DisGuildGuid = 0;
		g_pGameServer->SendToServer( GUILD_SERVER, &cmdMsg, sizeof(cmdMsg) );
	}
#endif
}

VOID CHuntingManager::EnterPlayer( Player * pPlayer )
{
	CompetitionTeam *pTeam = NULL;
	if( IsFreeForAllRule() )
	{
		// ���� ���������� �ϳ��� ������ �� �������Ʈ�� �߰��Ѵ�.
		CompetitionTeam *pTeam = CreateTeam();
		pTeam->AddMember( pPlayer );
	}
	else if( IsDualPartyRule() )
	{
		// �������Ʈ�� �߰�
		pTeam = FindReadyTeam( pPlayer );

		if( pTeam )	pTeam->AddMember( pPlayer );
		else		SUNLOG( eCRITICAL_LOG, "[CHuntingManager::EnterPlayer] pTeam is NULL" );
	}
}

VOID CHuntingManager::LeavePlayer( Player * pPlayer )
{
	--m_byReadyPlayerNum;

	WORD wTeamKey = pPlayer->GetCompetitionInfo().GetTeamKey();
	CompetitionTeam *pTeam = FindTeam( wTeamKey );
	if( !pTeam )	return;

	// �������Ʈ���� ����
	pTeam->RemoveMember( pPlayer->GetObjectKey() );

	// PVP ���� ����
	pPlayer->GetPVPInfo().Clear();
	pPlayer->GetPVPInfo().SetPVPState( ePVP_USER_NONE );

	// ���� �߰��� ���� �������� �г�Ƽ�� �ش�.
	if( m_eCompetitionState == eCOMPETITION_START_WAIT || m_eCompetitionState == eCOMPETITION_MATCH )
	{
		pPlayer->GetPVPInfo().MinusPVPPoint( DISCONNECT_PENALTY_PVPPOINT );
		pPlayer->GetCharInfo()->m_iCHuntingDisCount++;
	}
}

BOOL CHuntingManager::CanUsePotion( Player *pPlayer )
{
	// ������ ����� �� �ְ� ���ش�.
	return TRUE;

/*	CompetitionTeam *pTeam = FindTeam( pPlayer->GetCompetitionInfo().GetTeamKey() );
	if( !pTeam )	return TRUE;

	if( pTeam->GetAvilablePotion() )
		return TRUE;
	else
		return FALSE;*/
}

VOID CHuntingManager::UsePotion( Player *pPlayer )
{
	CompetitionTeam *pTeam = FindTeam( pPlayer->GetCompetitionInfo().GetTeamKey() );
	if( pTeam )
	{
//		pTeam->UsePotion();
	}
}

VOID CHuntingManager::LevelUp( Player * pPlayer )
{
	CompetitionTeam *pTeam = FindTeam( pPlayer->GetCompetitionInfo().GetTeamKey() );
	if( pTeam )
	{
		pTeam->LevelUp();
	}
}

VOID CHuntingManager::IncWinCount( Player * pPlayer, int iIncVCount )
{
	CompetitionTeam *pTeam = FindTeam( pPlayer->GetCompetitionInfo().GetTeamKey() );
	if( pTeam )
	{
		pTeam->IncWinCount( pPlayer, iIncVCount );
	}
}

VOID CHuntingManager::DeadCharacter( Character *pDeadChar, Character *pKillChar )
{
	if( !IsCompetition() ) 
		return; 

	// ų���� ã�´�.
	Player *pKiller = NULL;
	if( pKillChar->GetPlayerOwner() )
		pKiller = FindPlayer( pKillChar->GetPlayerOwner()->GetObjectKey() );

	// ���� ����� �÷��̾���
	if(pDeadChar->IsEqualObjectKind(PLAYER_OBJECT))
	{
		if( pKiller )
		{
			// ų����Ʈ ����
			CompetitionTeam *pTeam = FindTeam( pKiller->GetCompetitionInfo().GetTeamKey() );
			if( pTeam )
			{
				pTeam->IncPlayerKillPoint( pKiller, (Player*)pDeadChar );
			}
		}
	}
	// ���� ����� NPC���
	else if(pDeadChar->IsEqualObjectKind(NPC_OBJECT))
	{
		// ų����Ʈ ����
		if( pKiller )
		{
			CompetitionTeam *pTeam = FindTeam( pKiller->GetCompetitionInfo().GetTeamKey() );
			if( pTeam )
				pTeam->IncNPCKillPoint( pKiller, (NPC*)pDeadChar );
		}
	}
}

CompetitionTeam* CHuntingManager::CreateTeam()
{
	WORD wTeamKey = (WORD)m_CompetionKeyGenerater.GetKey();
	CompetitionTeam *pTeam = (CompetitionTeam *)m_CompetionTeamPool.Alloc();

	pTeam->Init( this, wTeamKey, (eCOMPETITION_RULE_TYPE)m_pAdditionalInfo->m_Rule, (eCOMPETITION_MODE_TYPE)m_pAdditionalInfo->m_Mode );
	m_CompetionTeamHash.Add( pTeam, wTeamKey );

	return pTeam;
}

VOID CHuntingManager::DestroyTeam( WORD wTeamKey )
{
	CompetitionTeam *pTeam = FindTeam( wTeamKey );
	if( !pTeam )
	{
		return;
	}

	m_CompetionTeamHash.Remove( wTeamKey );
	m_CompetionTeamPool.Free( pTeam );
	m_CompetionKeyGenerater.RestoreKey( wTeamKey );
}

VOID CHuntingManager::SendPacketToAll( MSG_BASE_FORWARD * pMsg, WORD wSize, DWORD dwMyObjKey )
{
	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		pTeam->SendPacketToAll( pMsg, wSize, dwMyObjKey );
	}
}

BOOL CHuntingManager::IsAllPlayerEntered()
{
	if( IsDualPartyRule() )
	{
		// �ι�° ��Ƽ�׷��� �� ������ �ȴ�.
		if( m_CompetionTeamHash.GetDataNum() == 2 )
		{
			CompetitionTeam *pTeam = m_CompetionTeamHash.GetTailData();

			if( pTeam->IsAllPlayerEntered() )
				return TRUE;
		}
	}
	else if( IsFreeForAllRule() )
	{
		// �� ���θ��� CompetitionTeam�� �ϳ��� �����Ƿ�, CompetitionTeam ���� üũ�Ѵ�.
		if( m_CompetionTeamHash.GetDataNum() >= m_byReadyPlayerNum )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CHuntingManager::IsAllLoseTeamLeft()
{
	CompetitionTeam *pLoseTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pLoseTeam = *it;
		if( pLoseTeam->GetTeamKey() == m_WinnerKey )	continue;

		if( pLoseTeam->GetMemberNum() )
			return FALSE;
	}

	return TRUE;
}

CompetitionTeam *CHuntingManager::DecideWinner()
{
	CompetitionTeam *pWinTeam = NULL;
	CompetitionTeam *pTeam = NULL;

	if( IsSurvivalMode() )
	{
		// ����ִ� ������� ���� ���� ���Ѵ�. ������ �� �����Ƿ� list�� �߰��Ѵ�.
		DecideMemberNumWinner();

		// ����ִ� ������� ������ ų ����Ʈ�� Winner�� �����Ѵ�.
		pWinTeam = DecideKillPointWinner( &m_TempTeamHash );
	}
	else if( IsTimeAttackMode() || IsDeathMatchMode() )
	{
		pWinTeam = DecideKillPointWinner( &m_CompetionTeamHash );
	}

	if( !pWinTeam )
	{
		pWinTeam = m_CompetionTeamHash.GetHeadData();
		for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
		{
			pTeam = *it;
			if( pTeam->GetExpireState() == eEXPIRE_STATE_ALL_DEAD )	continue;

			pWinTeam = pTeam;
			break;
		}
	}

	return pWinTeam;
}

CompetitionTeam *CHuntingManager::DecideKillPointWinner( COMPETITION_INFO_HASH *pTeamHash )
{
	WORD wMaxKillPoint = 0;
	CompetitionTeam *pWinTeam = NULL;
	CompetitionTeam *pTeam = NULL;

	for( COMPETITION_INFO_HASH_ITR it = pTeamHash->begin(); it != pTeamHash->end(); ++it )
	{
		pTeam = *it;
		if( pTeam->GetExpireState() == eEXPIRE_STATE_ALL_DEAD )	continue;

		if( pTeam->GetKillPoint() > wMaxKillPoint )
		{
			pWinTeam = pTeam;
			wMaxKillPoint = pTeam->GetKillPoint();
		}
	}
	return pWinTeam;
}

VOID CHuntingManager::DecideMemberNumWinner()
{
	BYTE byAlivePlayerNum = 0;	
	CompetitionTeam *pTeam = NULL;
	m_TempTeamHash.clear();

	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		if( pTeam->GetExpireState() == eEXPIRE_STATE_ALL_DEAD )	continue;

		if( pTeam->GetAliveMemberNum() == byAlivePlayerNum )
		{
			m_TempTeamHash.Add( pTeam, pTeam->GetTeamKey() );
		}
		if( pTeam->GetAliveMemberNum() > byAlivePlayerNum )
		{
			// �ִ� �������� �ٲ����Ƿ� ���������� ������ �߰��Ѵ�.
			m_TempTeamHash.clear();
			m_TempTeamHash.Add( pTeam, pTeam->GetTeamKey() );
			byAlivePlayerNum = pTeam->GetAliveMemberNum();
		}
	}
}

Player* CHuntingManager::FindPlayer( DWORD dwObjKey )
{
	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;

		Player *pPlayer = pTeam->FindMember( dwObjKey );
		if( pPlayer )
			return pPlayer;
	}

	return NULL;
}

BYTE CHuntingManager::GetMaxTeamMemberNum()
{
	BYTE byMaxMemberNum = 0;

	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;

		if( pTeam->GetMemberNum() > byMaxMemberNum )
			byMaxMemberNum = pTeam->GetMemberNum();
	}

	return byMaxMemberNum;
}

VOID CHuntingManager::EnterTeam( BYTE byReadyPlayerNum, DWORD * IN pUserKeyList, GUILDGUID GuildGuid )
{
	if( IsFreeForAllRule() )
	{
		m_StartWaitTimer.SetTimer( ALL_ENTER_WAIT_TIME );
		m_eCompetitionState = eCOMPETITION_ALL_ENTER_WAIT;
	}
	else if( IsDualPartyRule() )
	{
		CompetitionTeam *pTeam = CreateTeam();

		pTeam->SetReadyPlayer( byReadyPlayerNum, pUserKeyList );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		pTeam->SetGuildGuid( GuildGuid );
#endif

		if( m_CompetionTeamHash.GetDataNum() == 1 )
		{
			// ���� ���� ���� ���ؼ� �ϴ� è�ǿ����� �����Ѵ�.
			pTeam->SetChampion( TRUE );
		}
		else if( m_CompetionTeamHash.GetDataNum() == 2 )
		{
			m_StartWaitTimer.SetTimer( ALL_ENTER_WAIT_TIME );
			m_eCompetitionState = eCOMPETITION_ALL_ENTER_WAIT;
		}
	}
	else
	{
		SUNLOG( eMIDDLE_LOG, "[CHuntingManager::EnterTeam] Invalid Rule[%d]", m_pAdditionalInfo->m_Rule );
	}

	SetReadyPlayerNum( byReadyPlayerNum );
}

BYTE CHuntingManager::GetAllPointInfo( WORD TeamKey, CompetitionPoint * OUT pPointInfo )
{
	BYTE byCount = 0;

	CompetitionTeam *pTeam;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;

		byCount += pTeam->GetAllPointInfo( &pPointInfo[byCount] );
	}

	return byCount;
}

RC::eSTATUS_RESULT CHuntingManager::ResurrectionAtSafezone( Player *pPlayer, BOOL bModeCheck )
{
	CompetitionTeam *pTeam = FindTeam( pPlayer->GetCompetitionInfo().GetTeamKey() );
	if( !pTeam )	return RC::RC_STATUS_TEAM_IS_NULL;

	return pTeam->ResurrectionAtSafezone( pPlayer, bModeCheck );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
BOOL CHuntingManager::IsGuildPVPMode()
{
	BASE_ROOMINFO *pRoomInfo = (BASE_ROOMINFO*)m_pCHuntingZone->GetBaseRoomInfo();
	return pRoomInfo->IsSpecialType(eROOM_SPECIAL_GUILD_PVP);
}
#endif

CompetitionTeam * CHuntingManager::FindReadyTeam( Player *pPlayer )
{
	CompetitionTeam *pTeam = NULL;
	for( COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin(); it != m_CompetionTeamHash.end(); ++it )
	{
		pTeam = *it;
		if( pTeam->IsReadyPlayer( pPlayer->GetUserKey() ) )
			return pTeam;
	}

	return NULL;
}

eUSER_RELATION_TYPE	CHuntingManager::IsFriendPVP( Player *pAttacker,Player *pTarget )	
{
	if( IsFreeForAllRule() )		return IsFriendFreeForAll(pAttacker,pTarget);
	else if( IsDualPartyRule() )	return IsFriendDualParty(pAttacker,pTarget);
	else
		SUNLOG( eCRITICAL_LOG, 
				"[CHuntingManager::IsFriend] "
				"Invalid Rule[%d]", m_pAdditionalInfo->m_Rule );

	return USER_RELATION_NEUTRALIST;
}

eUSER_RELATION_TYPE	CHuntingManager::IsFriendFreeForAll( Player *pAttacker, Player *pTarget )
{
	switch(m_pAdditionalInfo->m_Mode)
	{
	case eCOMPETITION_MODE_SURVIVAL:
	case eCOMPETITION_MODE_DEATH_MATCH:	return USER_RELATION_ENEMY;
	case eCOMPETITION_MODE_TIME_ATTACK:	return USER_RELATION_NEUTRALIST;

	default:
		SUNLOG( eCRITICAL_LOG, 
				"[CHuntingManager::IsFriendFreeForAll] "
				"Rule[%d] : Invalid Mode[%d]",
				m_pAdditionalInfo->m_Rule, m_pAdditionalInfo->m_Mode );
		break;
	}		

	return USER_RELATION_NEUTRALIST;
}

eUSER_RELATION_TYPE	CHuntingManager::IsFriendDualParty( Player *pAttacker, Player *pTarget )
{
	switch(m_pAdditionalInfo->m_Mode)
	{
	case eCOMPETITION_MODE_SURVIVAL:
	case eCOMPETITION_MODE_DEATH_MATCH:
		if( pAttacker->GetPartyState() == pTarget->GetPartyState() )
		{
			return USER_RELATION_FRIEND;
		}
		return USER_RELATION_ENEMY; 

	case eCOMPETITION_MODE_TIME_ATTACK:
		if( pAttacker->GetPartyState() == pTarget->GetPartyState() )
		{
			return USER_RELATION_FRIEND;
		}
		return USER_RELATION_NEUTRALIST; 

	default:
		SUNLOG( eCRITICAL_LOG, 
			"[CHuntingManager::IsFriendDualParty] "
			"Rule[%d] : Invalid Mode[%d]",
			m_pAdditionalInfo->m_Rule, m_pAdditionalInfo->m_Mode );
		break;
	}		

	return USER_RELATION_NEUTRALIST;
}

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
VOID CHuntingManager::GetTeamWinCount(BYTE* OUT HomeTeamWinCount, 
                                      BYTE* OUT AwayTeamWinCount, 
                                      WORD HomeTeamKey)
{
    CompetitionTeam* pTeam = NULL;
    COMPETITION_INFO_HASH_ITR it = m_CompetionTeamHash.begin();
    for( ; it != m_CompetionTeamHash.end(); ++it )
    {
        pTeam = *it;

        if (pTeam->GetTeamKey() == HomeTeamKey)
            *HomeTeamWinCount = pTeam->GetWinCount();
        else
            *AwayTeamWinCount = pTeam->GetWinCount();
    }
}
















