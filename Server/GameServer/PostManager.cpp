#include "stdafx.h"

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)

#include "./PostManager.h"
#include "Player.h"
#include "PacketStruct_CG.h"
#include "ItemManager.h"

PostManager::PostManager()
{

}

PostManager::~PostManager()
{

}


RC::eITEM_RESULT 
PostManager::CheckRewardItem(Player* player, int amount_cost, SLOTCODE item_code,
                             OUT int& number_of_insertable_items)
{
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return RC::RC_ITEM_FAILED;
    }
    
    //--인벤 락 체크
    const ProtectorInventory& is_protect = player->GetWarehouseInventoryProtector().INVENTORY();
    DWORD check_invenLock = is_protect.IsUsable() ? ULONG_MAX : RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
    if (check_invenLock == RC::RC_ITEM_INVENTORY_NEED_PASSWORD)
    {
        return RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
    }

    //--인벤 insert 체크
    number_of_insertable_items = 
        player->GetItemManager()->CheckInsertItem(SI_INVENTORY, item_code, amount_cost);

    if (number_of_insertable_items < amount_cost)
    {
        return RC::RC_ITEM_NOSPACEININVENTORY;
    }
    else if (number_of_insertable_items > amount_cost)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Incorrect insertable num : requst : %u, result : %u",
               amount_cost, number_of_insertable_items);
        return RC::RC_ITEM_FAILED;
    }

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT 
PostManager::RewardItem(Player* player, int amount_cost, SLOTCODE item_code)
{
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return RC::RC_ITEM_FAILED;
    }

    //--아이템 보상 알림
    MSG_CG_ITEM_OBTAIN_ITEM_CMD send_msg;

    RC::eITEM_RESULT result_code = 
        player->GetItemManager()->Obtain(item_code, amount_cost, &send_msg.m_tagInventory);

    if (result_code != RC::RC_ITEM_SUCCESS)
    {
        return result_code;
    }

    player->SendPacket(&send_msg, send_msg.GetSize());

    return result_code;
}
bool 
PostManager::RewardHeim(Player* player, ULONGLONG amount_cost, eSYSTEM_MEMO_TYPE memo_type )
{
    //--하임 보상
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return false;
    }
    player->PlusMoney(amount_cost);
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Reward Heim[Name:%s][Heim:%I64u]",player->GetCharName(),
           amount_cost);

    //--하임 습득 알림
    MSG_CG_ITEM_PICK_MONEY_ACK notify_msg;
    notify_msg.m_Money = player->GetMoney();
    notify_msg.m_BonusRatio = static_cast<WORD>(0);
    player->SendPacket(&notify_msg, sizeof(notify_msg));

    return true;
}

//전달된 subject_memo 에서 memo_type과 map_code값을 추출한다.
//현재(2010.05.04)까지 지역 점령전 보상 시스템 쪽지의 로직을 변경하지 않는다.(추후 통합 처리 예정)
bool 
PostManager::GetValueInMemo(const TCHAR* subject_memo, eSYSTEM_MEMO_TYPE& OUT memo_type,
                                       MAPCODE& OUT map_code)
{ 
    std::string string_memo;
    string_memo.assign(subject_memo);

    std::string::size_type first_point;
    std::string::size_type second_point;

    //-- 메모 타입
    first_point = 0;
    second_point = string_memo.find('|');

    if (first_point != std::string::npos) 
    {
        std::string temp_string = string_memo.substr(first_point, second_point);
        memo_type = static_cast<eSYSTEM_MEMO_TYPE>(atoi(temp_string.c_str()));
    }
    else
    {
        assert(!"Not Found Memo Type In SystemMemo");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Found Memo Type In SystemMemo");
        return false;
    }

    //--타입 검사
    if (memo_type != SYSMEMO_DOMINATION_RETURN_MONEY && 
        memo_type != SYSMEMO_DOMINATION_REWARD_MONEY)
    {
        assert(!"System Memo Type Is Incorrect");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]System Memo Type[%d] Is Incorrect", memo_type);
        return false;
    }

    //--맵 코드
    first_point = string_memo.find('|');
    second_point = string_memo.find('|', first_point+1);
    
    if (second_point != std::string::npos)
    {
        std::string temp_string = string_memo.substr(first_point+1, second_point-1);
        map_code = atoi(temp_string.c_str());
    }
    else
    {
        assert(!"Not Found MapCode In SystemMemo");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Fount mapcode");
        return false;
    }

    return true;
}
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC