#include "stdafx.h"
#include "./SCItemSlotContainer.h"
#include "./SCItemSlot.h"
#include <SetItemOptionInfoParser.h>

//==================================================================================================

SCItemSlotContainer::SCItemSlotContainer(void)
    : valid_state_(false)
{
}

SCItemSlotContainer::~SCItemSlotContainer(void)
{
}

void
SCItemSlotContainer::ClearAll()
{
    SCSlotContainer::ClearAll();
    valid_state_ = true;
}

SCSlot*
SCItemSlotContainer::CreateSlot()
{
    return new SCItemSlot;
}

void
SCItemSlotContainer::Init(POSTYPE max_slot_size, SLOTIDX slot_index)
{
    SCSlotContainer::Init(max_slot_size, slot_index);
    valid_state_ = true;
}

void
SCItemSlotContainer::UpdateSlot(POSTYPE at_pos, int changed_num)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (slot == NULL || IsEmpty(at_pos)) {
        return;
    }
    //
    SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
    int calculated = item_slot->GetNum() + changed_num;
    if (calculated > item_slot->GetMaxNum()) {
        return;
    }
    //
    SCSlotContainer::UpdateSlot(at_pos, changed_num);
}

BOOL
SCItemSlotContainer::IsLocked(POSTYPE at_pos)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (slot == NULL) {
        // (WARNING) (f100311.3) changed policy for securable operation of a invalid slot.
        // traditional custom logic is returns no-locked state \
        // (or returns unknown result by garbage data), changes to locked state
        return true;
    }
    // (NOTE) that means of 'IsLocked' in a container, present internal locking mechanism
    // (CHANGES) item slot has been so enabled that multi-locking concept.
    // so that add the 'UserBlocked' check routine.
    SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
    if (item_slot->IsLocked() || item_slot->IsBlocked()) {
        return true; // internal locking mechanism, related with server inter-process communications
    }
    // (f100317.1L) changes to rollback version.
    ////      added (f100311.4)
    //if (item_slot->IsUserBlocked()) {
    //    return true; // related with summoning for pet or mount (presented by rider)
    //}
    // stacked lock to support ether-device, etc... it is multi-purpose lock based on case research
    // (NOTE) I don't know this lock whether act exactly.
    if (item_slot->GetLaxLock()->IsLock()) {
        return true;
    }
    return false;   
}

void
SCItemSlotContainer::SetLock(POSTYPE at_pos, BOOL is_setup)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    const bool if_fails_slot_pos_is_occurred_index_out_of_range = (slot != NULL);
    if (FlowControl::FCAssert(if_fails_slot_pos_is_occurred_index_out_of_range) == false) {
        return;
    }
    //
    SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
    item_slot->SetLock(!!is_setup);
}

// implemented by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
SCItemSlotContainer::SameSetItemSlotsResult
SCItemSlotContainer::GetSameSetItemSlots(SCItemSlot** result_of_set_items,
                                         const POSTYPE number_of_result_array,
                                         const BASE_ITEMINFO* related_reference_item,
                                         BOOL bEquip)
{
    // premise #1: current call stack is called in ApplySetItem... blah~
    // premise #2: special accessory can apply only in full set items equipped status
    SameSetItemSlotsResult result = { 0, };
    // (CHANGES) the querying special accessory must be the item using set item script,
    // even if the querying special accessory are not matched 'set item group' related real set items.
    result.set_item_code = related_reference_item->set_group_code; // enable changes in calculating
    if (result.set_item_code == 0) {
        return result;
    }

    const bool is_special_accessory = related_reference_item->m_bySetOptionType ==
                                      related_reference_item->eSetItem_Special;
    bool changeable_state = is_special_accessory;
    bool retry_state = false;

    const SLOT_LINEAR_LIST& slots = GetSlotList(); // (f100311.4)
    SLOT_LINEAR_LIST::const_iterator it = slots.begin(),
                                     end = slots.end();
    for (; it != end; (retry_state == false ? ++it : it))
    {
        retry_state = false;
        //if (IsEmpty(pos)) { continue; }
        const SlotNode& slot_node = *it;
        SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot_node.slot);
        if (item_slot->GetItemCode() == 0) {
            // SCItemSlot is empty if code equals zero. that's enough don't have call 'IsEmpty'
            continue;
        }
        // purpose : 현재 세트 아이템이 활성화 되어 있고, 같은 세트 아이템 타입을 가지고 온다. 
        // find related set item, and \
        // do loop process which had O(2) time complexity to support special accessory multi-link
        const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
        if (item_info->set_group_code == 0) {
            continue;
        }
        //
        if (item_info->m_bySetOptionType == item_info->eSetItem_Active)
        { // current : skipped if no set items
            if (item_info->set_group_code == result.set_item_code)
            { 
                // 동일한 아이템을 두 개 이상 장비한 것은 세트 장착으로 인정하지 않는다
                if(bEquip)
                {
                    POSTYPE i = 0;
                    for ( ; i < result.number_of_same_set_items; ++i)
                    {

                        if (item_info->m_Code == result_of_set_items[i]->GetItemCode()) {
                            break;
                        }
                    }
                    if (i >= result.number_of_same_set_items)
                    {
                        result_of_set_items[result.number_of_same_set_items] = item_slot;
                        ++result.number_of_same_set_items;
                    }
                }
                else
                {
                    if (item_info->m_Code != related_reference_item->m_Code) 
                    {
                        result_of_set_items[result.number_of_same_set_items] = item_slot;
                        ++result.number_of_same_set_items;
                    }
                }
            }
            else if (changeable_state && is_special_accessory)
            {   // entered if special accessory different set item group.
                // find linked accessory code in set item information
                const SETITEM_OPTION_INFO* set_option = \
                    SetItemOptionInfoParser::Instance()->Find(item_info->set_group_code);
                if (set_option == NULL) {
                    continue;
                }
                changeable_state = false;
                if (related_reference_item->m_Code ==
                    set_option->m_SpecialEquipOptionItem.m_wItemCode)
                {
                    // enabled state: the special accessory of 2nd elite is matched 1st elite's it.
                    result.set_item_code = item_info->set_group_code;
                    result.number_of_same_set_items = 0;
                    ZeroMemory(result_of_set_items, sizeof(SCItemSlot*) * number_of_result_array);

                    it = slots.begin();
                    retry_state = true;
                    // start researching
                    continue;
                }
            }
        } //end true condition if activated set item condition'
    }
    return result;
}

SCItemSlotContainer::SameSetItemSlotsResult
SCItemSlotContainer::GetSameSetItemTotalSlots( 
	SCItemSlot** result_of_set_items, 
	const POSTYPE number_of_result_array, 
	const BASE_ITEMINFO* related_reference_item,
	BOOL bEquip /*= TRUE*/ )
{
	// premise #2: special accessory can apply only in full set items equipped status
	SameSetItemSlotsResult result = { 0, };
	// (CHANGES) the querying special accessory must be the item using set item script,
	// even if the querying special accessory are not matched 'set item group' related real set items.
	result.set_item_code = related_reference_item->set_group_code; // enable changes in calculating
	if (result.set_item_code == 0) {
		return result;
	}

	const bool is_special_accessory = related_reference_item->m_bySetOptionType ==
		related_reference_item->eSetItem_Special;
	bool changeable_state = is_special_accessory;
	bool retry_state = false;

	const SLOT_LINEAR_LIST& slots = GetSlotList(); // (f100311.4)
	SLOT_LINEAR_LIST::const_iterator it = slots.begin(),
		end = slots.end();
	for (; it != end; (retry_state == false ? ++it : it))
	{
		retry_state = false;
		//if (IsEmpty(pos)) { continue; }
		const SlotNode& slot_node = *it;
		SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot_node.slot);
		if (item_slot->GetItemCode() == 0) {
			// SCItemSlot is empty if code equals zero. that's enough don't have call 'IsEmpty'
			continue;
		}
		// purpose : 현재 세트 아이템이 활성화 되어 있고, 같은 세트 아이템 타입을 가지고 온다. 
		// find related set item, and \
		// do loop process which had O(2) time complexity to support special accessory multi-link
		const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
		if (item_info->set_group_code == 0) {
			continue;
		}
		//
		if (item_info->m_bySetOptionType == item_info->eSetItem_Active)
		{ // current : skipped if no set items
			if (item_info->set_group_code == result.set_item_code)
			{ 
				//착용중 장비중에 같은 셋트가 몇개가 존재하는가?
				if(bEquip)
				{
						result_of_set_items[result.number_of_same_set_items] = item_slot;
						++result.number_of_same_set_items;
				}
			}
			//스테이트 변경및 스패셜 악세사리 경우임.
			else if (changeable_state && is_special_accessory)
			{   // entered if special accessory different set item group.
				// find linked accessory code in set item information
				const SETITEM_OPTION_INFO* set_option = \
					SetItemOptionInfoParser::Instance()->Find(item_info->set_group_code);
				if (set_option == NULL) {
					continue;
				}
				changeable_state = false;
				if (related_reference_item->m_Code ==
					set_option->m_SpecialEquipOptionItem.m_wItemCode)
				{
					// enabled state: the special accessory of 2nd elite is matched 1st elite's it.
					result.set_item_code = item_info->set_group_code;
					result.number_of_same_set_items = 0;
					ZeroMemory(result_of_set_items, sizeof(SCItemSlot*) * number_of_result_array);

					it = slots.begin();
					retry_state = true;
					// start researching
					continue;
				}
			}
		} //end true condition if activated set item condition'
	}
	return result;
}

//==================================================================================================
// implemented by _NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION
namespace nsSlot {
;

// 장착 아이템을 유형별로 검색하기 위한 검색 범위를 제공하는 유틸리티 클래스
// CHANGES: f110418.4L, f101124.1L, The used algorithm to search equipment groups is changed
// from O(nlogn) time complexity to O(1) time complexity
class EquipSearchspace
{
public:
    enum EquipGroup // 장비 그룹 상수
    {
        EQUIPGROUP_INVALID = 0,         // 유효하지 않은 장비 그룹
        EQUIPGROUP_WEAPON,              // 무기
        EQUIPGROUP_ARMOR,               // 방어구
        EQUIPGROUP_RING,                // 반지
        EQUIPGROUP_SPECIAL_ACCESSORY,   // 특수악세사리
        EQUIPGROUP_CASH_ITEM,           // 캐시 아이템
        EQUIPGROUP_PC_ROOM_ITEM,        // PC방 아이템
        EQUIPGROUP_MAX
    };

    struct SearchList {
        POSTYPE* first;
        POSTYPE* second;
    };
    //
    // 생성자에서 장비 그룹 구성 초기화
    EquipSearchspace()
    {
        ZeroMemory(search_list_, sizeof(search_list_));
        memset(data_list_, eEQUIPCONTAINER_MAX, sizeof(data_list_));
        //
        POSTYPE* dest_it = data_list_;
        if (SearchList* node = &search_list_[EQUIPGROUP_INVALID])
        {
            node->first = node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_WEAPON])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_WEAPON;
            node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_ARMOR])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_ARMOR;
            *dest_it++ = eEQUIPCONTAINER_PROTECTOR;
            *dest_it++ = eEQUIPCONTAINER_HELMET;
            *dest_it++ = eEQUIPCONTAINER_PANTS;
            *dest_it++ = eEQUIPCONTAINER_BOOTS;
            *dest_it++ = eEQUIPCONTAINER_GLOVE;
            *dest_it++ = eEQUIPCONTAINER_BELT;
            *dest_it++ = eEQUIPCONTAINER_SHIRTS;
            node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_RING])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_RING1;
            *dest_it++ = eEQUIPCONTAINER_RING2;
            node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_SPECIAL_ACCESSORY])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_SACCESSORY1;
            *dest_it++ = eEQUIPCONTAINER_SACCESSORY2;
            *dest_it++ = eEQUIPCONTAINER_SACCESSORY3;
            node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_CASH_ITEM])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_CHARGE1;
            *dest_it++ = eEQUIPCONTAINER_CHARGE2;
            *dest_it++ = eEQUIPCONTAINER_CHARGE3;
            *dest_it++ = eEQUIPCONTAINER_CHARGE4;
            *dest_it++ = eEQUIPCONTAINER_CHARGE5;
            node->second = dest_it;
        };
        if (SearchList* node = &search_list_[EQUIPGROUP_PC_ROOM_ITEM])
        {
            node->first = dest_it;
            *dest_it++ = eEQUIPCONTAINER_PC_ROOM1;
            *dest_it++ = eEQUIPCONTAINER_PC_ROOM2;
            *dest_it++ = eEQUIPCONTAINER_PC_ROOM3;
            node->second = dest_it;
        };
        //
        if (FlowControl::FCAssert(dest_it <= &data_list_[_countof(data_list_)]) == false) {
            __debugbreak();
        };
    }

    // 주어진 참조 장착 위치와 같은 그룹 아이템들의 장착 위치 벡터를 리턴
    const SearchList& GetSearchVector(POSTYPE reference_pos) const
    {
        if (bool in_range = (reference_pos < _countof(data_list_)))
        {
            EquipGroup equip_group = static_cast<EquipGroup>(kConvertPosToEquipGroup[reference_pos]);
            return search_list_[equip_group];
        }
        return search_list_[EQUIPGROUP_INVALID];
    }

private:
    //
    SearchList search_list_[EQUIPGROUP_MAX];
    POSTYPE data_list_[eEQUIPCONTAINER_MAX];
    static const uint8_t kConvertPosToEquipGroup[eEQUIPCONTAINER_MAX];
};

const uint8_t EquipSearchspace::kConvertPosToEquipGroup[eEQUIPCONTAINER_MAX] =
{
    EQUIPGROUP_WEAPON,  // eEQUIPCONTAINER_WEAPON      = 0,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_ARMOR       = 1,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_PROTECTOR   = 2,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_HELMET      = 3,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_PANTS       = 4,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_BOOTS       = 5,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_GLOVE       = 6,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_BELT        = 7,
    EQUIPGROUP_ARMOR,   // eEQUIPCONTAINER_SHIRTS      = 8,
    EQUIPGROUP_RING,    // eEQUIPCONTAINER_RING1       = 9,
    EQUIPGROUP_RING,    // eEQUIPCONTAINER_RING2       = 10,
    EQUIPGROUP_INVALID, // eEQUIPCONTAINER_NECKLACE    = 11,
    EQUIPGROUP_INVALID, // eEQUIPCONTAINER_BOW         = 12,
    EQUIPGROUP_SPECIAL_ACCESSORY, // eEQUIPCONTAINER_SACCESSORY1 = 13,
    EQUIPGROUP_SPECIAL_ACCESSORY, // eEQUIPCONTAINER_SACCESSORY2 = 14,
    EQUIPGROUP_SPECIAL_ACCESSORY, // eEQUIPCONTAINER_SACCESSORY3 = 15,
    EQUIPGROUP_CASH_ITEM, // eEQUIPCONTAINER_CHARGE1     = 16,
    EQUIPGROUP_CASH_ITEM, // eEQUIPCONTAINER_CHARGE2     = 17,
    EQUIPGROUP_CASH_ITEM, // eEQUIPCONTAINER_CHARGE3     = 18,
    EQUIPGROUP_CASH_ITEM, // eEQUIPCONTAINER_CHARGE4     = 19,
    EQUIPGROUP_CASH_ITEM, // eEQUIPCONTAINER_CHARGE5     = 20,
    EQUIPGROUP_PC_ROOM_ITEM, // eEQUIPCONTAINER_PC_ROOM1    = 21,
    EQUIPGROUP_PC_ROOM_ITEM, // eEQUIPCONTAINER_PC_ROOM2    = 22,
    EQUIPGROUP_PC_ROOM_ITEM, // eEQUIPCONTAINER_PC_ROOM3    = 23,
    ////
};

}; //end of namespace
//==================================================================================================

const SCItemSlot& 
SCItemSlotContainer::GetItemOfType(POSTYPE reference_pos, SLOTCODE item_code)
{
    // Note that this lazy static initialization is NOT thread-safe.
    static const nsSlot::EquipSearchspace search_space;
    typedef nsSlot::EquipSearchspace::SearchList SearchList;

    const SearchList& search_vector = search_space.GetSearchVector(reference_pos);
    for (POSTYPE* it = search_vector.first; it != search_vector.second; ++it)
    {
        const SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(*it));
        if (item_slot.GetItemCode() == item_code) {
            return item_slot;
        };
    };
    // can't find or exist group
    ;{
        const SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(reference_pos));
        return item_slot;
    };
}

