#include "StdAfx.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

#include ".\AccumulatePointImplement.h"
#include "AccumulatePoint.h"
#include <SCItemSlotContainer.h>
#include <DropRatioInfoList.h>
#include <ItemTypeList.h>
#include ".\ItemManager.h"
#include ".\Player.h"
#include ".\RankUpImplement.h"

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

AccumulatePointImplement::AccumulatePointImplement()
:accumulate_point_(NULL)
{
}

AccumulatePointImplement::~AccumulatePointImplement()
{
}

BOOL AccumulatePointImplement::OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult )
{
    ItemManager* item_manager = m_pPlayer->GetItemManager();
    ASSERT(item_manager);
    if (item_manager == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnCheck(%d) Not Found ItemManager [ID:%s]", 
               receive_protocol_,
               m_pPlayer->GetUserID());
        dwResult = RC::RC_ACCUMULATE_POINT_FAILED;
        return FALSE;
    }

    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    ASSERT(inventory);
    if (inventory == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnCheck(%d) Not Found Inventory [ID:%s]", 
               receive_protocol_,
               m_pPlayer->GetUserID());
        dwResult = RC::RC_ACCUMULATE_POINT_FAILED;
        return FALSE;
    }

    accumulate_point_ = m_pPlayer->GetAccumulatePoint();
    ASSERT(accumulate_point_);
    if (!accumulate_point_)
    {
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnCheck(%d) Not Found AccumulatePoint [ID:%s]", 
               receive_protocol_,
               m_pPlayer->GetUserID());
        dwResult = RC::RC_ACCUMULATE_POINT_FAILED;
        return FALSE;
    }

    if (inventory->GetEmptySlotNum() <= 0)
    {
        SUNLOG(eCRITICAL_LOG,
               "AccumulatePointImplement::OnCheck(%d) Inventory is Full [ID:%s]",
               receive_protocol_,
               m_pPlayer->GetUserID());
        dwResult = RC::RC_ACCUMULATE_POINT_NOSPACEININVENTORY;
        return FALSE;
    }
    return TRUE;
}

VOID AccumulatePointImplement::OnFailed( DWORD dwResult )
{
    switch (receive_protocol_)
    {
    case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN:
        {  
            MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK nak_message;
            nak_message.m_dwErrorCode = dwResult;
            m_pPlayer->SendPacket(&nak_message, sizeof(nak_message));
        }
        break;
    case CG_ITEM_USE_ACCUMULATE_POINT_SYN:
        {
            MSG_CG_ITEM_USE_ACCUMULATE_POINT_NAK nak_message;
            nak_message.m_dwErrorCode = dwResult;
            m_pPlayer->SendPacket(&nak_message, sizeof(nak_message));
        }
        break;
    }
}

VOID AccumulatePointImplement::OnImplement( MSG_BASE* pRecvMessage )
{
    switch (receive_protocol_)
    {
    case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN:
        {
            OnImplement((MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN*)pRecvMessage);
        }
        break;
    case CG_ITEM_USE_ACCUMULATE_POINT_SYN:
        {
            OnImplement((MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN*)pRecvMessage);
        }
        break;
    }
}


void AccumulatePointImplement::OnImplement(MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN* syn_message)
{
    BASE_PLAYERINFO* playerinfo = m_pPlayer->GetCharInfo();
    ASSERT(playerinfo);
    if (!playerinfo)
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) BASE_PLAYERINFO is NULL [ID:%s]", 
               receive_protocol_,
               m_pPlayer->GetUserID());
        return;
    }

    // 보상 그룹 설정.
    if (!accumulate_point_->GenerateReward(syn_message->m_First ? true : false))
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) GenerateReward(%s) is Fail [ID:%s]", 
               receive_protocol_,
               syn_message->m_First ? "First" : "Not First",
               m_pPlayer->GetUserID());
        return;
    }

    // 필요 적립 포인트 
    if (accumulate_point_->get_accumulate_point() < accumulate_point_->get_require_point())
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_HAVENOTPOINT);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) Not Enough Coin [ID:%s, Require:%d Have:%d]", 
               receive_protocol_,
               m_pPlayer->GetUserID(),
               accumulate_point_->get_require_point(),
               accumulate_point_->get_accumulate_point());
        return;
    }

    DropRatioInfoList* drop_ratio_list = DropRatioInfoList::Instance();
    ASSERT(drop_ratio_list);
    if (drop_ratio_list == NULL)
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) DropRatioInfoList is NULL", 
               receive_protocol_);
        return;
    }

    int in_count = MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK::ACCUMULATE_POINT_ITEM_COUNT;

    MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK ack_message;
    memset(ack_message.m_ItemCodeArray, 0, sizeof(SLOTCODE) * in_count);

    int out_count = drop_ratio_list->PopDropItemCodeArray(accumulate_point_->get_reward_code(), 
                                                          ack_message.m_ItemCodeArray, 
                                                          in_count);

    if (in_count == out_count)
    {
        m_pPlayer->SendPacket(&ack_message, sizeof(ack_message));
    }
    else
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) PopDropItemCodeArray is Fail [RewardGroupCode:%d]", 
               receive_protocol_, 
               accumulate_point_->get_reward_code());
    }
}


void AccumulatePointImplement::OnImplement(MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN* syn_message)
{
    BASE_PLAYERINFO* playerinfo = m_pPlayer->GetCharInfo();
    ASSERT(playerinfo);
    if (!playerinfo)
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) BASE_PLAYERINFO is NULL [ID:%s]", 
               receive_protocol_,
               m_pPlayer->GetUserID());
        return;
    }

    if (accumulate_point_->get_reward_code() == 0)
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) RewardGroupCode is 0 [ID:%s]", 
                receive_protocol_,
                m_pPlayer->GetUserID());
        return;
    }

    if (accumulate_point_->get_accumulate_point() < accumulate_point_->get_require_point())
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_HAVENOTPOINT);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) Not Enough Coin [ID:%s, Require:%d Have:%d]", 
               receive_protocol_,
               m_pPlayer->GetUserID(),
               accumulate_point_->get_require_point(),
               accumulate_point_->get_accumulate_point());
        return;
    }


    uchar drop_item_array_order;
    SCItemSlot output_item_slot;

    RC::eITEM_RESULT result = GenerateRewardItem(accumulate_point_->get_reward_code(),
                                                 output_item_slot,
                                                 drop_item_array_order);

    if (RC::RC_ITEM_SUCCESS != result)
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) GenerateRewardItem is Fail [ID:%s RewardGroupCode:%d]", 
               receive_protocol_,
               m_pPlayer->GetUserID(),
               accumulate_point_->get_reward_code());
        return;
    }

    MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK ack_message;

    // 새로 생성된 아이템 인벤토리에 추가.
    DURATYPE num = output_item_slot.GetNum();
    output_item_slot.SetNum(1);

    ItemManager* item_manager = m_pPlayer->GetItemManager();
    result = item_manager->Obtain(output_item_slot, num, &(ack_message.m_ItemInfo));
    if (RC::RC_ITEM_SUCCESS == result)
    {
        // 마일리지 코인을 감소 시킨 후 로그.
        ushort minus_point = accumulate_point_->get_require_point();
        accumulate_point_->MinusAccumulatePoint(minus_point);
        GAMELOG->LogAccumulatePoint(ACCUM_CONSUMPTION_ROULETTE, 
                                    m_pPlayer, 
                                    minus_point, 
                                    output_item_slot.GetItemCode());

        ack_message.m_wAccumulatePoint = playerinfo->m_wAccumulatePoint;
        ack_message.m_byDropItemOrder = drop_item_array_order;
        m_pPlayer->SendPacket(&ack_message, ack_message.GetSize());

        // 아이템 보상 리스트 그룹 초기화
        accumulate_point_->ResetReward();

#ifdef _NA_001990_ACSYSTEM_ADD
        const BASE_ITEMINFO* item_info = output_item_slot.GetItemInfo();
        if (item_info->m_wACCode)
            m_pPlayer->GetACManager()->ProcessItemObtain(item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, num);
#endif //_NA_001990_ACSYSTEM_ADD
    }
    else
    {
        OnFailed(RC::RC_ACCUMULATE_POINT_FAILED);
        SUNLOG(eCRITICAL_LOG, 
               "AccumulatePointImplement::OnImplement(%d) Item Obtain Fail [ID:%s RewardItemCode:%d]", 
               receive_protocol_, 
               m_pPlayer->GetUserID(),
               output_item_slot.GetItemCode());
    }
}


RC::eITEM_RESULT AccumulatePointImplement::GenerateRewardItem(uint reward_group_code, 
                                                              SCItemSlot& OUT output_item,
                                                              uchar& OUT drop_item_array_order)
{
    DropItemObj* drop_item = static_cast<DropItemObj*>
                    (DropRatioInfoList::Instance()->PopRandomItemTypeIndex(reward_group_code));
    ASSERT(drop_item);
    if (!drop_item)
    {
        SUNLOG( eCRITICAL_LOG, 
                "AccumulatePointImplement::GenerateRewardItem() Can't Find Item [reward code:%d]", 
                reward_group_code );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    if (drop_item->GetType() != DROP_OBJ_ITEM)
    {
        SUNLOG( eCRITICAL_LOG, 
                "AccumulatePointImplement::GenerateRewardItem() Not Object Item [reward code:%d]", 
                reward_group_code );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    const BASE_ITEMINFO* output_item_info = 
                                ItemInfoParser::Instance()->GetItemInfo(drop_item->GetItemCode());
    ASSERT(output_item_info);
    if (!output_item_info)
    {
        SUNLOG( eCRITICAL_LOG, 
                "AccumulatePointImplement::GenerateRewardItem() Can't Find Item [drop item code:%d]", 
                drop_item->GetItemCode() );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    // 아이템 설정
    output_item.SetCode((SLOTCODE)drop_item->GetItemCode());

    drop_item_array_order = drop_item->GetDropItemOrder();

    const nsSlot::ItemRuleInfo& item_rule = output_item.GetItemRuleInfo();
    if (item_rule.is_valid_item == 0)
    {
        SUNLOG( eCRITICAL_LOG, 
                "AccumulatePointImplement::GenerateRewardItem() Can't Find Item Rule [drop item code:%d]", 
                drop_item->GetItemCode() );
        return RC::RC_ITEM_INVALIDSTATE;
    }
    // 아이템 갯수 지정.
    if (output_item.IsOverlap())
    {
        // TODO(WAVERIX) 관련 사항 검증할 필요가 있어 보인다.
        uint count = drop_item->GetReserved();
        const DURATYPE max_count = output_item.GetMaxNum();
        if (max_count < count)
            output_item.SetNum(max_count);
        else if (count)
            output_item.SetNum(count);
    }
    else
    {
        ItemType* item_type = ItemTypeList::Instance()->FindItemType(drop_item->GetReserved());
        ASSERT(item_type);
        if (!item_type)
            return RC::RC_ITEM_SUCCESS;

        if (item_rule.IsEnableEnchant())
            output_item.SetEnchant(item_type->GetEnchant());
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        const BYTE number_of_socket = item_type->GetRank();
        if (number_of_socket != 0 && item_rule.IsEnableSocket())
        {
            if (output_item.CreateSocketItem(number_of_socket, false) == false)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|소켓 개수 설정에 실패했습니다.|아이템코드 = %u, 소켓개수 = &u"), 
                       __FUNCTION__, 
                       drop_item->GetItemCode(), 
                       number_of_socket);
                return RC::RC_ITEM_INVALIDSTATE;
            }
        }
#else
            // (CHANGES) (WAVERIX) itemslot에서 엘리트 룰(Rankup)이 제거되었다.
            if (output_item_info->IsElite() == 0 && item_rule.IsEnableRank())
            {
                output_item.SetRank(RANK_E);
                RankUpImplement RankUpImp;
                uchar rank = item_type->GetRank();
                for (uchar rank_index = RANK_E ; rank_index < rank ; ++rank_index)
                {
                    if (!RankUpImp.DoRankUp(output_item))
                    {
                        SUNLOG( eCRITICAL_LOG, 
                            "AccumulatePointImplement::GenerateRewardItem() RankUp Failed [output item code:%d]", 
                            output_item.GetItemCode() );
                        return RC::RC_ITEM_INVALIDSTATE;
                    }
                }
            }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

        nsSlot::ItemTypeChanger::ChangeItemByType(&output_item, item_type->GetItemType());
    }

    return RC::RC_ITEM_SUCCESS;
}

#endif //_NA_1419_20100121_ACCUMULATE_POINT
