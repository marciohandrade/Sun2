#include "StdAfx.h"
#include ".\gamehuntingroom.h"
#include ".\Player.h"
#include ".\Map.h"
#include ".\GameField.h"
#include "ChangeMonsterAbility.h"
#include "GameParty/NewGamePartyManager.h"

__IMPL_CUSTOMPOOL_PTR(GameHuntingRoom)
GameHuntingRoom::GameHuntingRoom(void)
{
}

GameHuntingRoom::~GameHuntingRoom(void)
{
}

void GameHuntingRoom::OnCreate()
{
	GameRoom::OnCreate();
}
void GameHuntingRoom::OnDestroy()
{
}

VOID GameHuntingRoom::Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_HUNTING, this, FieldCode, AreaID, pwvStartPos );
}

void GameHuntingRoom::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_HUNTING, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameHuntingRoom::Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_HUNTING ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameHuntingRoom::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
		return FALSE;
	}

	return TRUE;
}


BOOL GameHuntingRoom::MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if( !GameRoom::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, 0, NULL );
		return FALSE;
	}

	return TRUE;
}

VOID GameHuntingRoom::Leave( Player * pPlayer )
{
	GameRoom::Leave( pPlayer );
}

RC::ePARTY_RESULT GameHuntingRoom::WarpAtPartyMaster( Player *pPlayer )
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

RC::ePARTY_RESULT GameHuntingRoom::SummonReqAtPartyMaster( Player *pPlayer, DWORD dwObjKey )
{
	if( !pPlayer->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	if( !NewGamePartyManager::Instance()->IsMaster( pPlayer ) )
		return RC::RC_PARTY_ISNOT_MASTEROFPARTY;

	// 소환대상 검사
	Player *pSummonMember = PlayerManager::Instance()->FindPlayerByObjKey( dwObjKey );
	if( !pSummonMember ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST;

	if( !pSummonMember->GetField() ) 
		return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;

	// 소환을 요청한다.
	MSG_CG_PARTY_MEMBER_SUMMON_REQ_CMD sendMsg;
	pSummonMember->SendPacket( &sendMsg, sizeof(sendMsg) );

	return RC::RC_PARTY_SUCCESS;
}






























