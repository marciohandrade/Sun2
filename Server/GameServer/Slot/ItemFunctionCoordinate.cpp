#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "GameZone.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "GameInstanceDungeon.h"
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionCoordinate) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionCoordinate::SaveCoord(SCItemSlot & rItemSlot)
{
    // 좌표 저장 아이템인지 체크
    if (rItemSlot.GetItemInfo()->m_wType != eITEMTYPE_SAVE_COORD)
        return RC::RC_ITEM_INVALID_TYPE;

    // 한번만 좌표를 저장할 수 있다.
    if (rItemSlot.GetMapCode())
        return RC::RC_ITEM_ALREADY_STORE_COORD;

    if (!root_->player_->GetField())
        return RC::RC_ITEM_FIELD_IS_NULL;

    // 마을과 필드존에서만 사용가능하다.
    GameZone *pZone = root_->player_->GetGameZonePtr();
    if (!pZone || (pZone->GetZoneType() != eZONETYPE_VILLAGE &&
        pZone->GetZoneType() != eZONETYPE_FIELD))
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    };
    // add rule by __NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP
    MAPCODE CurMapCode = pZone->GetMapCode();
    if (CurMapCode == HardCode::MAPCODE_for_GM ||
        CurMapCode == HardCode::MAPCODE_for_TUTORIAL_FIELD)
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    }

    // 맵코드와 좌표를 저장한다.
    WzVector vCurPos;   root_->player_->GetPos(&vCurPos);
    vCurPos.x = (SHORT)vCurPos.x;
    vCurPos.y = (SHORT)vCurPos.y;
    vCurPos.z = (SHORT)vCurPos.z;

    if (!root_->player_->GetField()->IsMovableCoordinates(&vCurPos))
        return RC::RC_ITEM_CANNOT_STORE_COORD_AT_THIS_AREA;

    rItemSlot.SetMapCode(pZone->GetMapCode());
    rItemSlot.SetX((SHORT)vCurPos.x);
    rItemSlot.SetY((SHORT)vCurPos.y);
    rItemSlot.SetZ((SHORT)vCurPos.z);

    // 아이템 슬롯정보 변화를 알린다.
    MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD cmdMsg;
    cmdMsg.m_SlotIndex = SI_INVENTORY;
    cmdMsg.m_SlotPos = rItemSlot.GetPos();
    rItemSlot.CopyOut(cmdMsg.m_ItemStream);
    root_->player_->SendPacket(&cmdMsg, sizeof(cmdMsg));

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionCoordinate::ClearCoord(POSTYPE ClearPos, POSTYPE CoordPos)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ClearPos) || !root_->ValidPos(SI_INVENTORY, CoordPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rClearItemSlot = (SCItemSlot &)pContainer->GetSlot(ClearPos);
    SCItemSlot & rCoordItemSlot = (SCItemSlot &)pContainer->GetSlot(CoordPos);

    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rClearItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    // 좌표 초기화 아이템인지 체크
    if (rClearItemSlot.GetItemInfo()->m_wType != eITEMTYPE_CLEAR_COORD)
        return RC::RC_ITEM_INVALID_TYPE;

    // 좌표 저장 아이템인지 체크
    if (rCoordItemSlot.GetItemInfo()->m_wType != eITEMTYPE_SAVE_COORD)
        return RC::RC_ITEM_INVALID_TYPE;

    // 좌표가 저장되어 있는 아이템인지 체크
    if (!rCoordItemSlot.GetMapCode())
        return RC::RC_ITEM_NOT_STORE_COORD;

    WORD wCoolTimeType = rClearItemSlot.GetItemInfo()->m_wCoolTimeType;

    // 좌표 초기화 아이템 삭제
    rcResult = root_->Delete(SI_INVENTORY, ClearPos, 1, ITEM_DELETE);
    if (rcResult == RC::RC_ITEM_SUCCESS)
    {
        MSG_CG_ITEM_DELETE_CMD CmdMsg;
        CmdMsg.m_byReason = RC::RC_ITEM_DELETE_CLEAR_COORD;
        CmdMsg.m_atIndex = SI_INVENTORY;
        CmdMsg.m_atPos = ClearPos;
        root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "[ClearCoord] Cannot Delete ClearItem! ErrorCode[%d]", rcResult);
        return rcResult;
    }

    // 좌표를 초기화한다.
    rCoordItemSlot.SetMapCode(0);
    rCoordItemSlot.SetX(0);
    rCoordItemSlot.SetY(0);
    rCoordItemSlot.SetZ(0);

    // 아이템 슬롯정보 변화를 알린다.
    MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD cmdMsg;
    cmdMsg.m_SlotIndex = SI_INVENTORY;
    cmdMsg.m_SlotPos = rCoordItemSlot.GetPos();
    rCoordItemSlot.CopyOut(cmdMsg.m_ItemStream);
    root_->player_->SendPacket(&cmdMsg, sizeof(cmdMsg));

    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionCoordinate::Portal(POSTYPE CoordPos, POSTYPE PortalPos)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, CoordPos) || !root_->ValidPos(SI_INVENTORY, PortalPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rCoordItemSlot = (SCItemSlot &)pContainer->GetSlot(CoordPos);
    SCItemSlot & rPortalItemSlot = (SCItemSlot &)pContainer->GetSlot(PortalPos);

    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rPortalItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    // 좌표 저장, 이동 아이템인지 체크
    if (rCoordItemSlot.GetItemInfo()->m_wType != eITEMTYPE_SAVE_COORD ||
        rPortalItemSlot.GetItemInfo()->m_wType != eITEMTYPE_PORTAL)
    {
        return RC::RC_ITEM_INVALID_TYPE;
    };
    // 스크롤 아이템에 좌표가 저장되어 있는지 체크
    if (!rCoordItemSlot.GetMapCode())
        return RC::RC_ITEM_NOT_STORE_COORD;

    //__NA001164_20081024_CHAR_WAY_POINT_PORTAL
    // 플레이어 데이터의 체크는 플레이어 자신이 한다.
    const RC::ePORTAL_RESULT retValue = root_->player_->CanPortal();
    switch (retValue)
    {
    case RC::RC_PORTAL_UNABLE_CUR_ZONE: return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    case RC::RC_PORTAL_FIELD_IS_NULL:   return RC::RC_ITEM_FIELD_IS_NULL;
    case RC::RC_PORTAL_INVALIDSTATE:    return RC::RC_ITEM_INVALIDSTATE;
    }

    WORD wCoolTimeType = rPortalItemSlot.GetItemInfo()->m_wCoolTimeType;

    WzVector vDestPos;
    SetVector(&vDestPos, rCoordItemSlot.GetX(), rCoordItemSlot.GetY(), rCoordItemSlot.GetZ());
    FIELDCODE ToFieldCode = rCoordItemSlot.GetMapCode();

    // 같은 필드면 바로 이동하고, 아니면 Agent에 요청한다.
    RC::ePORTAL_RESULT rcResult2 = root_->player_->Portal(
        ePORTAL_TYPE_ITEM, ToFieldCode, vDestPos, PortalPos);
    if (rcResult2 == RC::RC_PORTAL_INSTANT_MOVE)
    {
        // 해당 아이템 삭제
        rcResult = root_->Delete(SI_INVENTORY, PortalPos, 1, ITEM_DELETE_ITEM_PORTAL);
        if (rcResult == RC::RC_ITEM_SUCCESS)
        {
            MSG_CG_ITEM_DELETE_CMD CmdMsg;
            CmdMsg.m_byReason = RC::RC_ITEM_DELETE_PORTAL_SUCCESS;
            CmdMsg.m_atIndex = SI_INVENTORY;
            CmdMsg.m_atPos = PortalPos;
            root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "[Portal] Cannot Delete PortalItem! ErrorCode[%d]", rcResult);
            return rcResult;
        }
    }
    else if (rcResult2 == RC::RC_PORTAL_FAIL)
    {
        return RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA;
    }

    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionCoordinate::EnabledRecallAlliance()
{
    // 마을, 로비인지 체크
    if (root_->player_->GetGameZoneState() == ePRS_AT_VILLAGE ||
        root_->player_->GetGameZoneState() == ePRS_AT_LOBBY) 
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    };
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // 전장 내부에서 사용 불가능..
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    ChaosZoneType chaoszone_type = root_->player_->GetChaosZoneType();
    if ( chaoszone_type == kBattleGround || chaoszone_type == eCHAOSZONE_TYPE_GOLDRUSH ) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    }
#else
    if ( root_->player_->IsThereBattleGroundZone() ) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // 혼자서도 사용할 수 있도록 변경
    // (WAVERIX) (090715) (BUG-FIX) 혼자 살아있는데, 파티가 없는 경우라면 사용이 안되도록 하는
    // 편이 관점상 더 덜 버그로 보인단다.
    // Alive  Party
    //   0      0    -> Can use
    //   0      1    -> Can use
    //   1      0    -> Can't use
    //   1      1    -> Can use
    if (root_->player_->IsAlive() && root_->player_->GetPartyState().IsPartyMember() == 0)
        return RC::RC_ITEM_UNUSABLE_FUNCTION;

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionCoordinate::FireUpDateTime(SCItemSlot & rItemSlot)
{
    if (false != rItemSlot.FireUp_DateTime())
    {
        root_->SendItemChange(rItemSlot);
        // 룬스톤 게임로그 추가 [10/21/2009 lst1024]
        GAMELOG->LogItem(ITEM_CHARGE_FIRST_EQUIP, root_->player_, &rItemSlot);

        return RC::RC_ITEM_SUCCESS;
    }
    // RC_ITEM_ALREADY_USE_CHARWAYPOINT 클라이언트 Commit 요청시 변경. [11/5/2009 lst1024]
    return RC::RC_ITEM_UNUSABLE_FUNCTION;
}
