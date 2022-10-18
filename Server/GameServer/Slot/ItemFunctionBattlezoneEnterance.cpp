#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "GameZone.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionBattlezoneEnterance) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionBattlezoneEnterance::CanUseItemForTopOfTrial(
    POSTYPE item_pos, MAPCODE& OUT result_map_code, BOOL& OUT use_charge_item)
{
    SCSlotContainer* slot_container = root_->GetItemSlotContainer(SI_INVENTORY);
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (root_->ValidPos(SI_INVENTORY, item_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;  
    }
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot_container->GetSlot(item_pos));
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    //시련의 탑 입장 아이템이 있는가?
    if ((item_info->m_wType != eITEMTYPE_TOP_OF_TRIAL &&
         item_info->m_wType != eITEMTYPE_TOP_OF_TRIAL_CHARGE) ||
        (item_slot.GetNum() ==0))
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    if (item_info->m_wType == eITEMTYPE_TOP_OF_TRIAL_CHARGE) {
        use_charge_item = true;
    }
    else {
        use_charge_item = false;
    }
    // hummm...
#if NATION_BUILD_VERSION >= 1101
    // f110118.2L, changes rule related to instance dungeon enterance using key.
    // the zone filter is only used in village (+...tutorial)
    // currently, I don't know whether this rule is generalized.
    GameField* game_field = root_->player_->GetField();
    if (game_field == NULL) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    }
    else
    {
        GameZone* game_zone = game_field->GetGameZone();
        eZONETYPE zone_type = game_zone->GetZoneType();
        MAPCODE current_map_code = game_zone->GetMapCode();
        if (zone_type != eZONETYPE_VILLAGE) {
            if (current_map_code != HardCode::MAPCODE_for_TUTORIAL_FIELD) {
                return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
            }
        }
    }
#endif

    ;{  // 쿨타임 체크
        RC::eITEM_RESULT result = root_->player_->CanUseItem(item_info);
        if (result != RC::RC_ITEM_SUCCESS) {
            return result;
        }
    };

    ;{
        ePLAYER_BEHAVE_STATE behave = root_->player_->GetBehave();
        // f090410, lst1024 중국 아이템 복사 버그 수정
        if (behave != PLAYER_BEHAVE_IDLE_STATE)
        {
            SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|"
                   "[%s]는 존 이동 아이템을 사용할수 없는 상태이다. 현재 상태[%d]",
                   root_->player_->GetCharName(), behave);
            return RC::RC_ITEM_UNUSABLE_FUNCTION;
        }
    };

    if (WORD wCoolTimeType = item_info->m_wCoolTimeType) {
        root_->player_->ResetUseItem(wCoolTimeType);
    };

    //성공이면 Lock을 걸자.
    item_slot.SetLock(true);
    result_map_code = item_info->m_wSkillCode;

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionBattlezoneEnterance::CanUseItemForTopOfTrial(SLOTCODE item_code, 
                                                                 BYTE item_num)
{
    SCSlotContainer* slot_container = root_->GetItemSlotContainer(SI_INVENTORY);
    if (root_->ValidState() == false) 
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }

    ItemManager* item_manager = root_->player_->GetItemManager();
    bool existed = (item_manager->ExistItem(SI_INVENTORY, item_code, item_num) == item_num);
    if (existed == false)
    {
        return RC::RC_ITEM_LESS_FOR_USE;
    }

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    RC::eITEM_RESULT result = root_->player_->CanUseItem(item_info);
    if (result != RC::RC_ITEM_SUCCESS)
    {
        return result;
    }

    if ((item_info->m_wType != eITEMTYPE_TOP_OF_TRIAL &&
        item_info->m_wType != eITEMTYPE_TOP_OF_TRIAL_CHARGE))
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    ePLAYER_BEHAVE_STATE behave = root_->player_->GetBehave();
    if (behave != PLAYER_BEHAVE_IDLE_STATE)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : player not used item : %s : %d",
               root_->player_->GetCharName(), item_code);
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }

    if (WORD wCoolTimeType = item_info->m_wCoolTimeType) 
    {
        root_->player_->ResetUseItem(wCoolTimeType);
    }
    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionBattlezoneEnterance::CanUseItemForPortalQuest(
    POSTYPE ItemPos, MAPCODE& OUT rMapCode)
{
    SCSlotContainer* pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        if (!root_->ValidState()) 
            return RC::RC_ITEM_INVALIDSTATE;

        if (!root_->ValidPos(SI_INVENTORY, ItemPos))
            return RC::RC_ITEM_INVALIDPOS;  

        SCItemSlot& rItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
        const BASE_ITEMINFO* pITEM_INFO = rItemSlot.GetItemInfo();
        if (!pITEM_INFO)
            return RC::RC_ITEM_NOINFO;

        //포탈 + 퀘스트용 아이템이 있는가? (ex: 100레벨 퀘스트)
        IfFailThenBreak(pITEM_INFO->m_wType == eITEMTYPE_PORTAL_QUEST_ITEM, 50);
        //여기서만 레벨 검사하는 것도 우습긴 하다... 그래도 레벨별 컨트롤이 필요할 수도 있으므로...
        IfFailThenBreak(!(pITEM_INFO->m_LV > root_->player_->GetLevel()), 60);

        //쿨타임 체크
        RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rItemSlot.GetItemInfo());
        if (RC::RC_ITEM_SUCCESS != rcResult)
            return rcResult;

        //(lst1024)(090410) 중국 아이템 복사 버그 수정
        if (root_->player_->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
        {
            SUNLOG(eCRITICAL_LOG,"[%s][%s]는 존 이동 아이템을 사용할수 없는 상태이다. 현재 상태[%d]",
                __FUNCTION__, root_->player_->GetCharName(), root_->player_->GetBehave());
            return RC::RC_ITEM_UNUSABLE_FUNCTION;
        }

        WORD wCoolTimeType = rItemSlot.GetItemInfo()->m_wCoolTimeType;
        root_->player_->ResetUseItem(wCoolTimeType);

        //성공이면 Lock을 걸자.
        rItemSlot.SetLock(true);    //이건 좀 더 생각해 볼 필요 있다.
        //#error "recheck!"

        rMapCode = (MAPCODE)pITEM_INFO->m_wSkillCode;

        return RC::RC_ITEM_SUCCESS;
    }
    FLOWCONTROL_END;

    switch (flow.GetErrorCode())
    {
    case 50: case 60: //있어서는 안되는 상황, 1)유저 노림수 2)스크립트 버그
        SUNLOG(eCRITICAL_LOG, _T("CanUseItemForPortalQuest : [%u]{%s}"),
            flow.GetErrorCode(), flow.GetErrorString());
        return RC::RC_ITEM_INVALID_TYPE;
    default:
        break;
    }
    return RC::RC_ITEM_FAILED;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionBattlezoneEnterance::CanUseItemForSSQField(
    POSTYPE ItemPos, MAPCODE& OUT rMapCode,
    SLOTCODE& OUT rItemCode, const MAPCODE expectedMapCode)
{
    SCSlotContainer* const pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        if (!root_->ValidState()) 
            return RC::RC_ITEM_INVALIDSTATE;

        if (!root_->ValidPos(SI_INVENTORY, ItemPos))
            return RC::RC_ITEM_INVALIDPOS;  

        SCItemSlot& rItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
        const BASE_ITEMINFO* const pITEM_INFO = rItemSlot.GetItemInfo();
        if (!pITEM_INFO)
            return RC::RC_ITEM_NOINFO;

        //포탈 + 퀘스트용 아이템이 있는가? (ex: 100레벨 퀘스트)
        IfFailThenBreak(pITEM_INFO->m_wType == eITEMTYPE_SSQ, 50);

        //rMapCode = (MAPCODE)pITEM_INFO->m_wSkillCode;
        //if (expectedMapCode != rMapCode)
        //  return RC::RC_ITEM_FAILED;

        //쿨타임 체크
        RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(const_cast<BASE_ITEMINFO*>(pITEM_INFO));
        if (RC::RC_ITEM_SUCCESS != rcResult)
            return rcResult;

        const WORD wCoolTimeType = pITEM_INFO->m_wCoolTimeType;
        root_->player_->ResetUseItem(wCoolTimeType);

        rItemCode = pITEM_INFO->m_Code;
        ISSQHandlerInGame& rRewarder = root_->player_->SSQHandler;
        rRewarder.SetUsedItemCode(rItemCode);

        //성공이면 Lock을 걸자.
        rItemSlot.SetLock(true);    //이건 좀 더 생각해 볼 필요 있다.
        //#error "recheck!"

        return RC::RC_ITEM_SUCCESS;
    }
    FLOWCONTROL_END;

    switch (flow.GetErrorCode())
    {
    case 50: case 60: //있어서는 안되는 상황, 1)유저 노림수 2)스크립트 버그
        SUNLOG(eCRITICAL_LOG, _T("CanUseItemForPortalQuest : [%u]{%s}"),
            flow.GetErrorCode(), flow.GetErrorString());
        return RC::RC_ITEM_INVALID_TYPE;
    default:
        break;
    }
    return RC::RC_ITEM_FAILED;
}
