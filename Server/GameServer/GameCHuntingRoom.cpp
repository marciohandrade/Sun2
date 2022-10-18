#include "StdAfx.h"
#include "GameCHuntingRoom.h"
#include "Player.h"
#include "Map.h"
#include "GameField.h"
#include "CHuntingManager.h"
#include "ChangeMonsterAbility.h"
#include "MapInfoParser.h"
#include "CompetitionInfo.h"
#include "UnitRegenInfoManager.h"
#include "ResrcManager.h"

#include "GameParty/NewGamePartyManager.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "CHuntingBattleResult.h"

__IMPL_CUSTOMPOOL_PTR(GameCHuntingRoom)
GameCHuntingRoom::GameCHuntingRoom()
{
	m_pCHuntingManager = new CHuntingManager;

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    CHuntingBattleResult* pBattleResult = new CHuntingBattleResult;
    pBattleResult->set_competition_manager(m_pCHuntingManager);
    m_pBattleResult = pBattleResult;    
}

GameCHuntingRoom::~GameCHuntingRoom()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);
    SAFE_DELETE(m_pCHuntingManager);
}

void GameCHuntingRoom::OnCreate()
{
	ChangeHandler( eZONETYPE_CHUNTING );

	GameRoom::OnCreate();

    m_pCHuntingManager->Init( this, &m_AdditionalInfo );

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    bool bEnablePVP = (m_AdditionalInfo.m_Mode == eCOMPETITION_MODE_TIME_ATTACK) ? FALSE : TRUE;
    m_pBattleResult->Init(bEnablePVP);
}

void GameCHuntingRoom::OnDestroy()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Release();

    GameRoom::OnDestroy();

    m_pCHuntingManager->Release();
}

VOID GameCHuntingRoom::Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_CHUNTING, this, FieldCode, AreaID, pwvStartPos );
}

void GameCHuntingRoom::SetZone( Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_CHUNTING, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameCHuntingRoom::Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_CHUNTING ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameCHuntingRoom::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}

	m_pCHuntingManager->EnterPlayer( pPlayer );

	return TRUE;
}

BOOL GameCHuntingRoom::MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::MoveField( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}
	m_pCHuntingManager->EnterPlayer( pPlayer );

	return TRUE;
}


VOID GameCHuntingRoom::Leave( Player* pPlayer )
{
    m_pCHuntingManager->LeavePlayer( pPlayer );

	GameRoom::Leave( pPlayer );
}

VOID GameCHuntingRoom::Update( DWORD dwDeltaTick )
{
	GameRoom::Update( dwDeltaTick );

	m_pCHuntingManager->Update();
}

BOOL GameCHuntingRoom::IsApplyPenalty()
{
	return m_pCHuntingManager->IsApplyPenalty();
}

RC::eSTATUS_RESULT GameCHuntingRoom::ResurrectionAtSafezone( Player* pPlayer )
{
	return m_pCHuntingManager->ResurrectionAtSafezone( pPlayer, TRUE );
}

RC::ePARTY_RESULT GameCHuntingRoom::WarpAtPartyMaster( Player* pPlayer )
{
	if( !pPlayer->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	Player *pMasterPlayer = NewGamePartyManager::Instance()->FindMasterPlayer( pPlayer->GetPartyState().GetPartyKey() );
	if( !pMasterPlayer ) 
		return RC::RC_PARTY_MASTER_NOTEXIST;

	if( !pMasterPlayer->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	// 파티장 근처의 좌표를 찾아 텔레포트 시킨다.
	WzVector vcPos = pMasterPlayer->GetRandomPosInAround( PARTY_MASTER_SUMMON_AROUND_DIST );

	pPlayer->GetField()->TeleportObject( pPlayer, &vcPos );

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT GameCHuntingRoom::SummonReqAtPartyMaster( Player* pPlayer, DWORD dwObjKey )
{
	if( !pPlayer->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	if( !NewGamePartyManager::Instance()->IsMaster( pPlayer ) )
		return RC::RC_PARTY_ISNOT_MASTEROFPARTY;

	// 소환대상 검사
	Player* pSummonMember = PlayerManager::Instance()->FindPlayerByObjKey( dwObjKey );
	if( !pSummonMember ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST;

	if( !pSummonMember->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	// 소환을 요청한다.
	MSG_CG_PARTY_MEMBER_SUMMON_REQ_CMD sendMsg;
	pSummonMember->SendPacket( &sendMsg, sizeof(sendMsg) );

	return RC::RC_PARTY_SUCCESS;
}

eUSER_RELATION_TYPE	GameCHuntingRoom::IsFriendPVP( Player *pAttacker, Player *pTarget ) 
{
	return m_pCHuntingManager->IsFriendPVP(pAttacker,pTarget); 
}
















