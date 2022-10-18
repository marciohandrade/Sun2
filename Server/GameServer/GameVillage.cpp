#include "StdAfx.h"
#include ".\gamevillage.h"
#include "Player.h"
#include "Map.h"
#include <MapInfoParser.h>

__IMPL_CUSTOMPOOL_PTR(GameVillage)

GameVillage::GameVillage(void)
: FieldPvPRule( PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE ) 
{
}

GameVillage::~GameVillage(void)
{
}

VOID
GameVillage::OnCreate()
{
	ChangeHandler( eZONETYPE_VILLAGE );
	GameZone::OnCreate();
}

VOID GameVillage::Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_VILLAGE, this, FieldCode, AreaID, pwvStartPos );
}

void GameVillage::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_VILLAGE, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameVillage::Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_VILLAGE ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameVillage::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameZone::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
		return FALSE;
	}

	pPlayer->SaveStartLocation();

	// 마을로 이동하였으므로 이전마을을 저장한다.
	pPlayer->SetPreVillageMapCode( GetMapCode() );

	return TRUE;
}

VOID GameVillage::Leave( Player * pPlayer )
{
	pPlayer->SaveStartLocation();

	GameZone::Leave( pPlayer );
}

BOOL GameVillage::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT*  pErrCode ) 
{ 
    return FieldPvPRule.CanPVP(pAttacker,pTarget,pErrCode); 
}
BOOL GameVillage::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT*   pErrCode )
{ 
    return FieldPvPRule.CanPVP(pAttacker,pTarget,pErrCode);
}

eUSER_RELATION_TYPE	GameVillage::IsFriendPVP( Player* pAttacker,Player* pTarget ) 
{ 
    return FieldPvPRule.IsFriendPVP(pAttacker,pTarget);
}
//_NA_008706_20160229_TEXTLOG_PK
VOID GameVillage::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/) 
{ 
    FieldPvPRule.KillPlayer(pAttacker,pDeadPlayer,bSummoned, damage_value);
}

