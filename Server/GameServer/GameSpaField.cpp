#include "StdAfx.h"

#include "GameSpaField.h"
#include "TriggerManagerEx.h"
#include "StatusManager.h"

__IMPL_CUSTOMPOOL_PTR(GameSpaField)

GameSpaField::GameSpaField() : field_pvp_rule_(PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE), check_filters_(NULL)
{
    //
}

void GameSpaField::OnCreate()
{
    ban_item_type[0] = eITEMTYPE_PET;
    check_filters_ = new MatchCheckCodeFilters();
    new (&check_filters_->ItemTypeFilter)
        MatchCheckCodeFilterArray(1, ban_item_type);

    ChangeHandler( eZONETYPE_SPA_FIELD );
    GameZone::OnCreate();
}

void GameSpaField::SetZone( Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
    pObject->SetGameZone( ePRS_AT_SPA_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

void GameSpaField::Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
    DEBUG_CODE(
        GameZone * pEnterZone	= NULL;
    MAPCODE EnterFieldCode	= 0;
    eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
    ASSERT( !(eRoomState & ePRS_AT_ZONE) );
    );

    pPlayer->SetGameZone( ePRS_BEFORE_ENTER_SPA_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameSpaField::Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
    if (FALSE == (ePRS_BEFORE_ENTER_SPA_FIELD & pPlayer->GetGameZoneState()))
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] eRoomState[%d] ZoneType[%d]",
            pPlayer->GetGameZoneState(), GetZoneType() );

        return FALSE;
    }

    SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if (!GameZone::Join(pPlayer, FieldCode, AreaID, pwvStartPos))
    {
        pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
        return FALSE;
    }

    // 필드 정책관련 처리
    if (Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION) {
        pPlayer->SaveStartLocation();
    }
    if (Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        GameField* pField = FindGameField(FieldCode);
        pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
    }

    //탈것 해제
    pPlayer->Rider.Command_SummonRelease(IVehicleEvent::EnterField());

    //필드필터
    StatusManager* const status_manager = pPlayer->GetStatusManager();   
    status_manager->Status.On_FilterControl(FALSE, TRUE, check_filters_);

    // 펫을 소환해제 한다.
    CPetManager& pet_manager = pPlayer->m_PetManager;
    pet_manager.ReturnPet(TRUE);

    return TRUE;
}

BOOL GameSpaField::MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
    DEBUG_CODE(
        eZONE_STATE eRoomState	= pPlayer->GetGameZoneState();
        ASSERT( (eRoomState & ePRS_BEFORE_ENTER_DOMINATION_FIELD) );
    );

    SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if( !GameZone::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
    {
        pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
        return FALSE;
    }

    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION) {
        pPlayer->SaveStartLocation();
    }
    if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        GameField* pField = FindGameField(FieldCode);
        pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
    }

    return TRUE;
}

void GameSpaField::Leave( Player * pPlayer )
{
    StatusManager* const pStatusManager = pPlayer->GetStatusManager();
    pStatusManager->Status.On_FilterControl( FALSE, FALSE, NULL );

    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION) {
        pPlayer->SaveStartLocation();
    }

    GameZone::Leave(pPlayer);
}

void GameSpaField::LeaveForWarp( Player* pPlayer )
{
    //필드필터
    StatusManager* const status_manager = pPlayer->GetStatusManager();   
    status_manager->Status.On_FilterControl(FALSE, TRUE, check_filters_);

    //위치저장
    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION) {
        pPlayer->SaveStartLocation();
    }

    GameZone::Leave(pPlayer);
}

BOOL GameSpaField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode )
{
    return FALSE;
}

BOOL GameSpaField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
    return FALSE;
}

GameSpaField::~GameSpaField()
{
    SAFE_DELETE(check_filters_);
}
