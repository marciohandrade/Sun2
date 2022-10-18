#include "stdafx.h"
#include "ItemManager.h"

#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"

//==================================================================================================

#include "InventorySlotContainer.h"
#include "EquipmentSlotContainer.h"
#include "AppliedItemDependencyChain.h"

#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
#include "PacketHandler/Handler_CG_ITEM.h"
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE

//==================================================================================================

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemManagerUpdateImpl) == sizeof(ItemManager*));

union AbuseSlotNode
{
    DWORD composed;
    struct
    {
        POSTYPE pos_in_slots;
        SLOTIDX slot_index;
        bool exist;
        uint8_t __dummy;
    };
};

//// result type : WORD
//#define MAKE_ABUSEDATA(a, b) \
//    (WORD)(((a & 0xFF) << 8) | (b & 0xFF))


}; //end of namespace

//==================================================================================================
//== item serial key management routines
//==================================================================================================

bool nsSlot::ItemManagerUpdateImpl::IsDupSerialCode(DWORD64 world_serial_key) const
{
    typedef ItemManager::ABUSESERIAL_HASH   ABUSESERIAL_HASH;
    const ABUSESERIAL_HASH& serial_table = root_->serial_check_table_;
    ABUSESERIAL_HASH::const_iterator find_it = serial_table.find(world_serial_key);
    if (find_it == serial_table.end()) {
        return false;
    };

    AbuseSlotNode abuse_node = { find_it->second };
    if (abuse_node.exist == false) {
        return false;
    };
    return true;
}

void nsSlot::ItemManagerUpdateImpl::RemoveSerialCode(
    SLOTIDX slot_index, POSTYPE pos_in_slots, DWORD64 world_serial_key)
{
    typedef ItemManager::ABUSESERIAL_HASH   ABUSESERIAL_HASH;
    ABUSESERIAL_HASH* const serial_table = &root_->serial_check_table_;
    ABUSESERIAL_HASH::iterator find_it = serial_table->find(world_serial_key);
    if (find_it != serial_table->end()) {
        find_it->second = 0;
        return;
    };
    // error reporting
    SUNLOG(eCRITICAL_LOG, _T("|[C:%u][Slot:%u,Pos:%u]|can't find the serial(%I64u) in table|"),
           root_->player_->GetCharGuid(), slot_index, pos_in_slots, world_serial_key);
}

void nsSlot::ItemManagerUpdateImpl::AddSerialCode(
    eSERIAL_ACTION serial_action,
    SLOTIDX slot_index, POSTYPE pos_in_slots,
    DWORD64 world_serial_key)
{
    bool is_empty_serial = static_cast<ulong>(world_serial_key & DBSERIAL_MASK_VALUE) ==
                           TEMP_DBSERIAL_VALUE;
    Player* const player = root_->player_;
    const DWORD user_guid = player->GetUserGuid();

    if (is_empty_serial)
    {
        SCItemSlotContainer* const item_container = root_->GetItemSlotContainer(slot_index);
        assert(item_container);
        if (slot_index == SI_INVENTORY ||
            slot_index == SI_EQUIPMENT)
        {
            // 시리얼 발급
            item_container->SetLock(pos_in_slots, true);
            // request a serial issue to database (proxy)
            ;{
                MSG_DG_ITEM_SERIAL_SYN msg_syn;
                msg_syn.m_dwKey = user_guid;
                msg_syn.m_SlotIdx = slot_index;
                msg_syn.m_ItemPos = pos_in_slots;
                g_pGameServer->SendToServer(GAME_DBPROXY, &msg_syn, sizeof(msg_syn));
            };
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[C:%u][Slot:%u,Pos:%u]|Warnings!! can't request serial issuance|"),
                   user_guid, slot_index, pos_in_slots);
        };
        //
        SCItemSlot& issue_item_slot =
            static_cast<SCItemSlot&>(item_container->GetSlot(pos_in_slots));
        ASSERT(issue_item_slot.GetSerial() == TEMP_DBSERIAL_VALUE);
        //------------------------------------------------------------
        // configure item options
        this->SetupPostItemOptionOnSerialIssued(issue_item_slot);
        //------------------------------------------------------------
    }
    else if (IsDupSerialCode(world_serial_key))
    {
        this->SetupPostItemEventOnDuplicatedSerialIssued(
            serial_action, slot_index, pos_in_slots, world_serial_key);
    }
    else
    {
        typedef ItemManager::ABUSESERIAL_HASH   ABUSESERIAL_HASH;
        ABUSESERIAL_HASH& serial_table = root_->serial_check_table_;
        AbuseSlotNode abuse_node = { 0, };
        abuse_node.pos_in_slots = pos_in_slots;
        abuse_node.slot_index = slot_index;
        abuse_node.exist = true;
        serial_table[world_serial_key] = abuse_node.composed;
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool nsSlot::ItemManagerUpdateImpl::SetupPostItemOptionOnSerialIssued(
    SCItemSlot& issue_item_slot)
{
    const nsSlot::ItemRuleInfo& item_rule_info = issue_item_slot.GetItemRuleInfo();
    const BASE_ITEMINFO* const item_info = issue_item_slot.GetItemInfo();
    __TOUCHED((item_rule_info, item_info));
    // NOTE: post item configurations = { limited }
    ;{
        issue_item_slot.CalLimited(); // calculate item using by prefetched limited index
    };
    // NOTE: post item configurations = { pet }
    if (item_rule_info.is_pet)
    {
        SCPetInfo* pet_info_in_slot = issue_item_slot.GetPetInfo();
        pet_info_in_slot->SetFullness(MAX_PET_FULLNESS);
    };
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // NOTE: post item configurations = { socket }
    ;{
        // EP2에서 소켓은 엘리트, 럭키, 리미티드, 일반 등의 구분이 아니라 모두 동일한 규칙을 이용하여
        // 생성한다. 소켓 생성 로직이 EP2 이전 룰을 따라서 흩어져 있어서 어떤 경로를 사용해서 아이템을
        // 생성하더라도 같은 방식으로 소켓을 생성할 수 있도록 이곳으로 옮긴다.
        enum { kRamdomSocketNumber = 0 };
        issue_item_slot.CreateSocketItem(kRamdomSocketNumber, false);
    };
    // NOTE: post item configurations = { zard item }
    if (issue_item_slot.IsZardItem())
    {
        // 옵션이 설정되지 않았다면
        if (issue_item_slot.GetExtendData() == SocketOptionParser::kInvalidSocketOptionIndex)
        {
            // 램덤으로 옵션을 설정한다.
            const SocketOptionParser* socket_option_parser = SocketOptionParser::Instance();
            const SOCKETINDEX zard_option_index = 
                socket_option_parser->GetRandomZardOption(item_info->m_Code);
            if (zard_option_index != SocketOptionParser::kInvalidSocketOptionIndex)
            {
                issue_item_slot.SetExtendData(zard_option_index);
            }
            else
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|can't configure zard option")
                       _T("|CharGuid=%u, Slot:%u, Pos:%u|"),
                       root_->player_->GetCharGuid(),
                       issue_item_slot.GetSlotIdx(),
                       issue_item_slot.GetPos());
            }
        }
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_003740_20111122_RANDOM_ITEM
    const RandomValueTypeGrade::Value type_grade = RandomValueTypeGrade::kGrade1;
    if (issue_item_slot.Randomize(type_grade) == true)
    {
        TCHAR add_info[MAX_ADDINFO_SIZE] = { 0 };
        issue_item_slot.RandomOptionToString<_countof(add_info)>(type_grade, add_info);
        GAMELOG->LogItem(ITEM_RANDOMIZE_OPTION, root_->player_, &issue_item_slot, 0, 1, NULL, NULL, add_info);
    }
#endif
    //!~_NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    if (item_rule_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2) {
        issue_item_slot.FireUp_DateTime();
    }
    //~!_NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    //
    return true;
};

bool nsSlot::ItemManagerUpdateImpl::SetupPostItemEventOnDuplicatedSerialIssued(
    eSERIAL_ACTION serial_action,
    const SLOTIDX slot_index, const POSTYPE pos_in_slots,
    const DWORD64 world_serial_key)
{
    typedef ItemManager::ABUSESERIAL_HASH   ABUSESERIAL_HASH;
    //
    const ABUSESERIAL_HASH& serial_table = root_->serial_check_table_;
    const ABUSESERIAL_HASH::const_iterator abuse_node_it = serial_table.find(world_serial_key);
#ifdef _DEBUG
    // pre checked
    if (abuse_node_it != serial_table.end()) {
        AbuseSlotNode abuse_node = { abuse_node_it->second };
        ASSERT(abuse_node.exist);
    }
#endif
    //
    Player* const player = root_->player_;
    eABUSE_SUB_TYPE abuse_sub_type = eABUSE_SUB_DUPLICATE_ITEM_SERIAL_EXSIT;

    if (SCItemSlotContainer* const item_container = root_->GetItemSlotContainer(slot_index))
    {
        SCItemSlot& blocking_item_slot =
            static_cast<SCItemSlot&>(item_container->GetSlot(pos_in_slots));
        //
        if (blocking_item_slot.IsBlocked() == false)
        {
            blocking_item_slot.SetBlocked(true);
            ;{ // notify blocking state to client to synchronize information
                MSG_CG_ITEM_BLOCKC_CMD msg_cmd;
                msg_cmd.m_SlotIdx = slot_index;
                msg_cmd.m_SlotPos = pos_in_slots;
                player->SendPacket(&msg_cmd, sizeof(msg_cmd));
            };
            //
            abuse_sub_type = eABUSE_SUB_DUPLICATE_ITEM_SERIAL_GENERATE;
        }
    };
    //
    //----------------------------------------------------------------------------------------------
    const AbuseSlotNode abuse_node = { abuse_node_it->second };

    if (abuse_node.exist &&
        abuse_node.slot_index == slot_index &&
        abuse_node.pos_in_slots == pos_in_slots)
    {
        ASSERT(!"unexpected status");
        return false;
    };

    SCItemSlotContainer* const item_container =
        root_->GetItemSlotContainer(abuse_node.slot_index);
    if (FlowControl::FCAssert(item_container != NULL) == false) {
        return false;
    };
    if (item_container->ValidPos(abuse_node.pos_in_slots) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=invalid serial data in serial table|Serial=%I64u, Slot=%d, Pos=%d|"),
               world_serial_key, abuse_node.slot_index, abuse_node.pos_in_slots);
    }
    else
    {
        SCItemSlot& abuse_source_item_slot =
            static_cast<SCItemSlot&>(item_container->GetSlot(abuse_node.pos_in_slots));
        if (abuse_source_item_slot.GetItemCode() == 0)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|Msg=invalid operation, try serial issue to empty slot")
                   _T("|Serial=%I64u, Slot=%d, Pos=%d|"),
                   world_serial_key, abuse_node.slot_index, abuse_node.pos_in_slots);
        }
        else if (abuse_source_item_slot.IsBlocked() == false)
        {
            // abuse_source_item_slot.IsBlocked() -> Nothing to do
            abuse_source_item_slot.SetBlocked(true);
            SUNLOG(eCRITICAL_LOG,
                    _T("|[C:%u][Serial:%I64u][Slot:%u,Pos:%u][Slot:%u,Pos:%u]|blocked|"),
                    player->GetCharGuid(),
                    world_serial_key,
                    slot_index, pos_in_slots,
                    abuse_node.slot_index, abuse_node.pos_in_slots);
            //
            abuse_sub_type = eABUSE_SUB_DUPLICATE_ITEM_SERIAL_GENERATE;

            ;{ // notify blocking state to client to synchronize information
                MSG_CG_ITEM_BLOCKC_CMD msg_cmd;
                msg_cmd.m_SlotIdx = abuse_node.slot_index;
                msg_cmd.m_SlotPos = abuse_node.pos_in_slots;
                player->SendPacket(&msg_cmd, sizeof(msg_cmd));
            };
            GAMELOG->LogItem(ITEM_DUPLICATE_SERIAL, player, &abuse_source_item_slot);
        }
    };

    SECURELOG(SECURELOG_PLAYERINFO(player, eABUSE_DUPLICATE_ITEM_SERIAL, abuse_sub_type),
              _T("[eSERIAL_ACTION:%u][Serial:%I64u,Slot:%u,Pos:%u]=[Slot:%u,Pos:%u]"),
              (BYTE)serial_action, world_serial_key,
              slot_index, pos_in_slots,
              abuse_node.slot_index, abuse_node.pos_in_slots);
    //
    return true;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace nsSlot {
;

enum {
    eTimeExpirationItemTags =
        ( (1 << eCHARGE_SUB_FIXED_AMOUNT)
        | (1 << eCHARGE_SUB_FIXED_AMOUNT_EX)
        | (1 << eCHARGE_SUB_FIXED_QUANTITY)
        | (1 << eCHARGE_SUB_FIXED_AMOUNT_WAIT)
        | (1 << eCHARGE_SUB_FATE)
        | (1 << eCHARGE_PC_ROOM)
        | (1 << eCHARGE_SUB_COORDINATE_EX)
        | (1 << eCHARGE_SUB_RIDER_WITH_PART)
        | (1 << eCHARGE_SUB_RIDER_WITH_PART_NOTIMER)
        | (1 << eCHARGE_SUB_FIXED_AMOUNT_EX2)
        ), //eTimeExpirationItemTags
};

}; //end of namespace

inline DWORD
nsSlot::ItemManagerUpdateImpl::GetExpirationItemType(const SCItemSlot& item_slot) const
{
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    // [1] - ChargeSubType에 의해 기간제 아이템으로 설정되는 경우
    // [2] - 이것이 특수 아이템 (X-Mas Event Time Expiration Item)인 경우 기간제로 설정할 필요 있다.
    //  - Special Event Handler에 의한 기간제 아이템인지 아닌지를 검사한다.

    DWORD selector = SELECTED_DEFAULTITEM;
    if (item_rule_info.charge_sub_type &&
        (nsSlot::eTimeExpirationItemTags & (1 << item_rule_info.charge_sub_type)))
    {
        selector = item_rule_info.charge_sub_type;
    }
    else if (item_info->time_expiration_item)
    {
        selector = SELECTED_TIMEEXPIRATIONITEM;
    }
    return selector;
}

//==================================================================================================
// (NOTE) 1st : 기간제 아이템 처리 목적
//        2nd : 유료화 관련 아이템
//        3rd : 기간 존재하는 아이템
//        4th : ItemSlot::Update를 호출해야 하는 아이템
// item_slot: slot in container
void nsSlot::ItemManagerUpdateImpl::AddExpirationItem(SCItemSlot& item_slot)
{
    if (item_slot.GetSerial() == TEMP_DBSERIAL_VALUE) {
        return;
    };
    // [1] - ChargeSubType에 의해 기간제 아이템으로 설정되는 경우
    // [2] - 이것이 특수 아이템 (X-Mas Event Time Expiration Item)인 경우 기간제로 설정할 필요 있다.
    //  - Special Event Handler에 의한 기간제 아이템인지 아닌지를 검사한다.

    const DWORD selector = GetExpirationItemType(item_slot);
    if (selector == SELECTED_DEFAULTITEM) {
        return;
    };
    //
    // SELECTED_TIMEEXPIRATIONITEM | eCHARGE_SUB_TYPE(with time check)
    typedef ItemManager::EXPIRATIONITEM_HASH EXPIRATIONITEM_HASH;
    EXPIRATIONITEM_HASH& time_table = root_->expiration_item_table_;
    std::pair<EXPIRATIONITEM_HASH::iterator, bool> result = time_table.insert(&item_slot);
    ASSERT(result.second);
}

//==================================================================================================
// item_slot: slot in container
void nsSlot::ItemManagerUpdateImpl::RemoveExpirationItem(SCItemSlot& item_slot) // 기간제 아이템 처리 목적 - 유료화 관련 아이템, 또는 아니더라도 기간 존재하는 아이템
{
    if (item_slot.GetSerial() == TEMP_DBSERIAL_VALUE) {
        return;
    };
    // [1] - ChargeSubType에 의해 기간제 아이템으로 설정되는 경우
    // [2] - 이것이 특수 아이템 (X-Mas Event Time Expiration Item)인 경우 기간제로 설정할 필요 있다.
    //  - Special Event Handler에 의한 기간제 아이템인지 아닌지를 검사한다.

    const DWORD selector = GetExpirationItemType(item_slot);
    if (selector == SELECTED_DEFAULTITEM) {
        return;
    };
    //
    // SELECTED_TIMEEXPIRATIONITEM | eCHARGE_SUB_TYPE(with time check)
    typedef ItemManager::EXPIRATIONITEM_HASH EXPIRATIONITEM_HASH;
    EXPIRATIONITEM_HASH& time_table = root_->expiration_item_table_;
    time_table.erase(&item_slot);
}

//==================================================================================================
//==================================================================================================
//  <_UpdateExpirationItem>
//      Update 처리 구조를 변경하기로 하자.
//  @History
//  - (WAVERIX) (091002) code rearrangement
//      delete : __NA_1017_LUCKY_ITEM_IDENTIFY_LOG
//               __NA_0_PET_SYSTEM
bool nsSlot::ItemManagerUpdateImpl::_UpdateExpirationItem() // ChargeItem, etc Time Expiration Items
{
    if (root_->item_update_timer_.IsExpired() == false) {
        return true;
    }
    //----------------------------------------------------------------------------------------------
    static SCItemSlot deleted_slot_s;
    // to reduce code address size
    SCItemSlot& deleted_slot = deleted_slot_s;
    //----------------------------------------------------------------------------------------------
    Player* const player = root_->player_;
    const bool is_pcroom_user = player->CanApplyPCBangBenefit(true);
    //----------------------------------------------------------------------------------------------
    // 유료화 아이템의 만료기간 처리
    // eCHARGE_SUB_FIXED_AMOUNT     < 정액제 : 일정 기간제 (2007-06-07 24:12:30)
    // eCHARGE_SUB_FIXED_AMOUNT_EX  < 확장정액제 : 착용시 부터 일정 기간 사용 (30시간 -> 2007-06-07 24:12:30)
    // eCHARGE_SUB_FIXED_AMOUNT_WAIT< 기한 만료시 일주일 대기, 대기 시간도 지나면 삭제
    // eCHARGE_SUB_FIXED_QUANTITY   < 정량제 : 사용 기간제 (30시간)
    // eCHARGE_SUB_FATE             < 럭키 아이템 형식: { 무제한, 시간 제한, 시간 만료시 사용 불가 }
    // eCHARGE_SUB_COORDINATE_EX    < 이중 타이머: 사용 기간 만료시 삭제 + 등록된 웨이포인트 삭제시 다음 삭제 가능 시점 설정
    // eCHARGE_SUB_RIDER_WITH_PART  < 연관 타이머:
    //                              - 라이더: 입수 시점부터 일정 기간 사용 후, 사용 불가 형태로 설정
    //                              - 파츠: 장착 시점부터 일정 기간 사용 후, 사용 불가 형태로 설정
    // eCHARGE_SUB_RIDER_WITH_PART_NOTIMER < 연관 타이머:
    //                              - 라이더: 기간 없음(eCHARGE_SUB_GENERAL), 장착된 파츠는 기간 존재할 수 있음.
    //                              - 파츠: 기간 없음, 라이더에 장착된 다른 파츠는 기간 존재 가능
    //                                  장착되지 않고 독립적으로 존재시 'eCHARGE_SUB_GENERAL'와 동일
    // eCHARGE_SUB_FIXED_AMOUNT_EX2 < 확장정액제2 : 획득시부터 일정기간 사용
    typedef ItemManager::EXPIRATIONITEM_HASH EXPIRATIONITEM_HASH;
    EXPIRATIONITEM_HASH& item_time_table = root_->expiration_item_table_;
    EXPIRATIONITEM_HASH::iterator it = item_time_table.begin(),
                                  end = item_time_table.end();
    // (WAVERIX) (WARNING) 한 번에 제거할 것이 있을 경우 하나씩만 제거하는 루틴이다. 주의할 것.
    // - 부연: 제거 이벤트가 발생시(한번이라도)에는 루프가 종료된다.
    for (; it != end; ++it)
    {
        SCItemSlot& item_slot = *(*it);
        //------------------------------------------------------------------------------------------
        //  (NOTE) Expiration Item Section
        //  - 모처럼 slot updater (ISlotUpdaterExpirationItem)를 만들어 놓고 뭔 짓인가?
        //    하지만 현재(071202) 너저분 상태의 update를 정리한 후에 작업하기로 하자.
        const DWORD selector = GetExpirationItemType(item_slot);
        if (selector == SELECTED_TIMEEXPIRATIONITEM)
        {
            if (AppliedItemDependencyChain* item_chain = AppliedItemDependencyChain::Instance())
            {
                if (item_chain->IsExpiredItem(item_slot) == false) {
                    continue;
                };
                if (DeleteItemOnUpdate(item_slot.GetSlotIdx(), item_slot.GetPos(),
                        item_slot, RC::RC_ITEM_DELETE_SUCCESS, ITEM_DELETE, &deleted_slot))
                {
                    break;
                }
            }
            continue;
        }

        //------------------------------------------------------------------------------------------
        //  (NOTE) Charge Item Section - controlled by 'eCHARGE_SUB_TYPE'
        //  (WAVERIX) (NOTE) (071201)
        //  - 골치 아픈 구조... 불필요한 빈번 체크... 수차례 증분 업데이트로 인한 너저분 정도... 정리하자.
        //  (WAVERIX) (NOTE) (091002)
        //  - 정리는 수행, 구조 자체가 변경되지는 않았다.
        
        bool update_result = (item_slot.Update(is_pcroom_user) != false);
        if (update_result != false) {
            continue;
        }

        //-------------------------------
        // delete operation by game rule
        // (NOTE) 대기 시간 관련 | 펫 관련 등등 고려할 것들이 몇 가지 존재한다.
        // (NOTE) one-time event로 처리된다. 즉, SCItemSlot내부적으로 한 번만 false가 발생하도록 해준다.
        // 이 때 이벤트 처리를 완료해야 하며, 되도록 실패가 없는 편이 필요하다.

        const SLOTIDX slot_index = item_slot.GetSlotIdx();
        const POSTYPE item_pos = item_slot.GetPos();

        // case #1
        // 일주일 대기시간이 설정된 경우 (eCHARGE_SUB_FIXED_AMOUNT_WAIT)
        // 그냥 사용 불가 설정 (eCHARGE_SUB_FATE, eCHARGE_SUB_RIDER_WITH_PART(_NOTIMER))
        // 
        const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
        if (item_rule_info.is_rider || item_rule_info.is_rider_parts)
        {
            if (slot_index == SI_INVENTORY) {
                IPlayerRiderD& rider = player->Rider;
                rider.Command_ItemSync(player, item_slot);
            };

            ChangeItemOnUpdate(slot_index, item_pos, item_slot,
                               true, ITEM_CHARGE_EXPIRED);
            continue;
        }

        bool is_wait_after_expired = (item_slot.IsExpiredWaitTime() != false);
        if (is_wait_after_expired != false)
        {
            // old comments: 럭키 아이템은 기간 만료시 로그로... (일반적으로 삭제시 로그 남김)
            // old comments: (091002) (ADDIN) Rider|Rider Parts도 그렇게 할까?
            // CHANGES: waverix, 장착된 아아템인 경우에만 'UpdateItemAttributes()' 수행한다.
            // CHANGES: f110408.3L, moved to the upper position about attributes update toggling
            //
            ChangeItemOnUpdate(slot_index, item_pos, item_slot,
                               item_rule_info.is_fate != false, ITEM_CHARGE_EXPIRED);
            // CHANGES: f110408.3L, time attribute changed item is controlled by time_table
            // on runtime since _NA_0_20110318_ITEM_EXPIRATION_PERIOD applied.
            if (slot_index == SI_EQUIPMENT) {
            #ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
                root_->UpdateItemAttributes(true, false);
            #else
                root_->UpdateAttrForEquipItem();
            #endif
            }

            continue;
        }
        // case #2
        // 일반적인 삭제 - 예외: 국내의 경우는 시간 제한이 없는 Pet|Rider로 유지될 것이다.
        // 하지만 해외는 그렇지 않다. 문제는 Rider의 경우는 Parts도 존재한다는 점에서 Pet과는
        // 처리 방식 자체가 다르다. 문제가 대단히 복잡해진다. (ex:Parts를 추출할 인벤공간이 없다면?)
        // 따라서 Rider삭제는 없도록 해야 할 것이다.
        if (is_wait_after_expired == 0) {
            // 여기서 erase를 하면, RemoveChargeItem()에서 아이템을 제거 하지 못한다.
            // ref: 루프 시작 위치 '(WARNING)' <- 제거 이벤트가 발생시에는 루프가 종료된다.
            
            //-----------------------------------------------------------------------
            // check nested event { pet } - 비활성 시에는 소환 해제부터 수행한다.
            if (slot_index == SI_INVENTORY &&
                item_slot.IsPetItem() && player->m_PetManager.AlreadySummonedPet())
            {
                player->m_PetManager.ReturnPet(&item_slot, true /*forced*/);
            }
            // delete operation : default
            if (DeleteItemOnUpdate(slot_index, item_pos, item_slot,
                    RC::RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM, ITEM_CHARGE_EXPIRED,
                    &deleted_slot))
            {
                break;
            }
        }
        // (NOTE) 이곳에 걸린다는 것은 위에서 처리 실패인 경우나
        // 또는 처리 성공 이후 continue가 없는 경우. 긴 루프인 관계로 구분하기 위해서 선언
        assert(update_result == 0);
    }; //end 'for'
    //
    // update attributes by 'last_auto_update'
    //
    return true;
}

bool nsSlot::ItemManagerUpdateImpl::_UpdateEtcItem()
{
    return true;
}

//==================================================================================================

bool nsSlot::ItemManagerUpdateImpl::DeleteItemOnUpdate(
    SLOTIDX slot_index, POSTYPE pos_in_slots,
    SCItemSlot& item_slot,
    uint8_t reason_based_on_item_result,
    uint16_t reason_based_on_game_log,
    SCItemSlot* const deleted_item_slot)
{
#ifdef _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info != NULL && 
        (item_info->m_wType == eITEMTYPE_PCBANG || item_info->m_wType == eITEMTYPE_RENDER_EFFECT_ITEM) )
    {
        CharacterInfoExManager& extrainfo_manager = root_->player_->GetExtraInfoManager();
        extrainfo_manager.TurnOnOffToggleItems((eITEMTYPE)item_info->m_wType, false);
    }
#endif //_NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE

    SCItemSlotContainer* item_container = root_->GetItemSlotContainer(slot_index);
    if (item_container->DeleteSlot(pos_in_slots, deleted_item_slot) == false) {
        return false;
    };

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    PlayerSpaInfo* spa_info = root_->player_->GetPlayerSpaInfo();
    spa_info->CheckAndResetSpaTicket(pos_in_slots);

#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

    MSG_CG_ITEM_DELETE_CMD msg_del_cmd;
    msg_del_cmd.m_byReason = reason_based_on_item_result;
    msg_del_cmd.m_atIndex  = slot_index;
    msg_del_cmd.m_atPos    = pos_in_slots;
    root_->player_->SendPacket(&msg_del_cmd, sizeof(msg_del_cmd));

    GAMELOG->LogItem(reason_based_on_game_log, root_->player_, deleted_item_slot);
    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|[IDX:%u][POS:%u]|"),
           slot_index, pos_in_slots);
    //
    return true;
}

bool nsSlot::ItemManagerUpdateImpl::ChangeItemOnUpdate(
    SLOTIDX slot_index, POSTYPE pos_in_slots,
    SCItemSlot& item_slot,
    bool enabled_game_logging, uint16_t reason_based_on_game_log)
{
    MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD msg_change_cmd;
    msg_change_cmd.m_SlotIndex = slot_index;
    msg_change_cmd.m_SlotPos   = pos_in_slots;
    item_slot.CopyOut(msg_change_cmd.m_ItemStream);
    root_->player_->SendPacket(&msg_change_cmd, sizeof(msg_change_cmd));

    if (enabled_game_logging) {
        GAMELOG->LogItem(reason_based_on_game_log, root_->player_, &item_slot);
    };
    //
    return true;
}

#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
void nsSlot::ItemManagerUpdateImpl::AddImmediatelyUseItem( SCItemSlot& item_slot )
{
    if (item_slot.GetSerial() == TEMP_DBSERIAL_VALUE) {
        return;
    };

    if (IsImmediatelyUseItem(item_slot) == false)
    {
        return;
    }

    root_->immediately_use_item_table_.insert(&item_slot).second;
}

void nsSlot::ItemManagerUpdateImpl::RemoveImmediatelyUseItem( SCItemSlot& item_slot )
{
    if (item_slot.GetSerial() == TEMP_DBSERIAL_VALUE) {
        return;
    };

    if (IsImmediatelyUseItem(item_slot) == false)
    {
        return;
    }

    root_->immediately_use_item_table_.erase(&item_slot);
}

bool nsSlot::ItemManagerUpdateImpl::IsImmediatelyUseItem( SCItemSlot& item_slot )
{
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info == NULL)
    {
        return false;
    }

    return item_info->IsImmediatelyUseItem();
}

bool nsSlot::ItemManagerUpdateImpl::UpdateImmediatelyUseItem()
{
    if (root_ == NULL)
    {
        return false;
    }

    if (root_->immediately_use_item_table_.empty() == true)
    {
        return true;
    }
    
    std::vector<POSTYPE> delete_list;
    for (std::set<SCItemSlot*>::iterator cur_iter = root_->immediately_use_item_table_.begin();
        cur_iter != root_->immediately_use_item_table_.end(); ++cur_iter)
    {
        SCItemSlot& item_slot = *(*cur_iter);
        if ( IsImmediatelyUseItem(item_slot) == false )
        {
            continue;
        }

        SLOTCODE slot_code = item_slot.GetCode();
        MSG_CG_ITEM_USE_SYN msg;
        msg.m_dwKey = root_->player_->GetUserKey();
        msg.m_atIndex = item_slot.GetSlotIdx();
        msg.m_atPos = item_slot.GetPos();
        Handler_CG_ITEM::OnCG_ITEM_USE_SYN(root_->player_->GetServerSession(), &msg, sizeof(msg));

        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if (item_info == NULL)
        {
            continue;
        }

        if (item_info->m_ChargeSubType == eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE)
        {
            delete_list.push_back(msg.m_atPos);
        }
    }
    
    for (std::vector<POSTYPE>::iterator delete_iter = delete_list.begin();
        delete_iter != delete_list.end(); ++delete_iter)
    {
        POSTYPE pos = *delete_iter;
        RC::eITEM_RESULT rt = root_->player_->GetItemManager()->Delete(pos);
        if (RC::RC_ITEM_SUCCESS == rt) 
        {
            MSG_CG_ITEM_DELETE_CMD CmdMsg;
            CmdMsg.m_byReason = RC::RC_ITEM_DELETE_SUCCESS;
            CmdMsg.m_atIndex = SI_INVENTORY;
            CmdMsg.m_atPos = pos;
            root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));
        }
    }

    root_->immediately_use_item_table_.clear();

    return true;
}

#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE

//==================================================================================================
