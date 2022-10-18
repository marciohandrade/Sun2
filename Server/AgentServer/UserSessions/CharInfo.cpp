#include "stdafx.h"
#include "./CharInfo.h"

#include <time.h>

#include <SCStream.h>
#include <LogTimeFormat.h>
#include <WZ_MD5.h>

#include "User.h"
#include "TempUser.h"
#include "UserManager.h"
#include <AgentServer.h>
//
#include <PacketHandler/PacketHandler.h>

//==================================================================================================
ns_object::CharacterList::PoolInstance*
ns_object::CharacterList::pool_instance_s = NULL;

struct ns_object::CharacterList::PoolInstance
{
    PoolInstance()
    {
        pool_instance.Initialize(20, 20, "CharacterInfoList", NULL);
        ZeroMemory(&empty_info, sizeof(empty_info));
    }

    ~PoolInstance()
    {
        pool_instance.Release();
    }
    //
    util::CMemoryPoolFactory<CharacterInfoList> pool_instance;
    CharacterInfoList empty_info;
};

//==================================================================================================

ns_object::CharacterList::CharacterList()
{
    static ns_object::CharacterList::PoolInstance pool_instance;
    pool_instance_s = &pool_instance;
    //
    ZeroMemory(this, sizeof(*this));
};

ns_object::CharacterList::~CharacterList()
{
    if (character_infos)
    {
        pool_instance_s->pool_instance.Free(character_infos);
        character_infos = NULL;
    };
}

void ns_object::CharacterList::Init() // called by User::Init
{
    ASSERT(character_infos == NULL);
    ZeroMemory(this, sizeof(*this));
    character_infos = reinterpret_cast<CharacterInfoList*>(pool_instance_s->pool_instance.Alloc());
    ZeroMemory(character_infos, sizeof(*character_infos));
}

void ns_object::CharacterList::Release() // called by User::Release
{
    if (FlowControl::FCAssert(character_infos != NULL))
    {
        pool_instance_s->pool_instance.Free(character_infos);
        character_infos = NULL;
    };
}

bool ns_object::CharacterList::SelectCharacter(int selecting_char_slot_index)
{
    BOOST_STATIC_ASSERT(_countof(character_infos->nodes) == MAX_CHARACTER_LIST_NUM);
    bool in_ranges = (selecting_char_slot_index >= 0 &&
                      selecting_char_slot_index < MAX_CHARACTER_LIST_NUM);
    bool validated_char = in_ranges && occupied_slots[selecting_char_slot_index] &&
        (character_infos->nodes[selecting_char_slot_index].m_CharGuid != 0);
    if (in_ranges == false)
    {
        CharacterInfoList* backup = character_infos;
        ZeroMemory(this, sizeof(*this));
        this->selected_char_slot_index = MAX_CHARACTER_LIST_NUM;
        character_infos = backup;
        ZeroMemory(character_infos, sizeof(*character_infos));
        return true;
    };
    //
    this->selected_char = NULL;
    this->selected_char_slot_index = MAX_CHARACTER_LIST_NUM;

    ZeroMemory(character_infos, static_cast<size_t>(
        reinterpret_cast<char*>(character_infos->char_name_hashes) -
        reinterpret_cast<char*>(character_infos)));
    //
    if (validated_char == false) {
        return false;
    }
    //
    this->selected_char = &character_infos->nodes[selecting_char_slot_index];
    this->selected_char_slot_index = selecting_char_slot_index;
    return true;
}

//==================================================================================================

void ns_object::CharacterList::SerializeCharacterPart(BYTE slot_index,
    SERVER_CHARACTER_PART& INOUT parts, eSERIALIZE se)
{
    if (FlowControl::FCAssert(slot_index < MAX_CHARACTER_SLOT_NUM) == false) {
        return;
    };
    if (se == SERIALIZE_LOAD)
    {
        assert(!"currently, default game flow can't enter at this point");
        parts = character_infos->nodes[slot_index];
        return;
    };

    ASSERT(se == SERIALIZE_STORE);
    // CHANGES: f110517.3L, the calling case of 'SetOccupiedSlot' changes to post this serialization
    ASSERT(this->occupied_slots[slot_index] == false);
    SERVER_CHARACTER_PART* const dest_part = &character_infos->nodes[slot_index];
    *dest_part = parts;
    if (dest_part->m_DelChk != 0)
    {
        dest_part->m_tszDelDate[_countof(dest_part->m_tszDelDate) - 1] = _T('\0');
    };
}

void ns_object::CharacterList::LoadClientCharacterPart(BYTE slot_index,
    CLIENT_CHARACTER_PART* const result_parts) const
{
    if (FlowControl::FCAssert(slot_index < MAX_CHARACTER_SLOT_NUM) == false) {
        return;
    };

    const SERVER_CHARACTER_PART& source_part = character_infos->nodes[slot_index];
    result_parts->m_bySlot = source_part.m_bySlot;
    //source_part.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
    BOOST_STATIC_ASSERT(
        sizeof(result_parts->m_tszCharName) == sizeof(source_part.m_tszCharName) &&
        _countof(result_parts->m_tszCharName) == _countof(source_part.m_tszCharName));

    int len = _sntprintf(result_parts->m_tszCharName, _countof(result_parts->m_tszCharName),
                         "%s", source_part.m_tszCharName);
    //                    ^ : prevent security problem
    assert(len > 0);
    result_parts->m_tszCharName[_countof(result_parts->m_tszCharName) - 1] = _T('\0');

    result_parts->m_byHeight = source_part.m_byHeight;
    result_parts->m_byFace = source_part.m_byFace;
    result_parts->m_byHair = source_part.m_byHair;
    result_parts->m_byClass = source_part.m_byClass;
    result_parts->m_LV = source_part.m_LV;
    result_parts->m_iRegion = source_part.m_iRegion;
    result_parts->m_sX = source_part.m_sX;
    result_parts->m_sY = source_part.m_sY;
    result_parts->m_sZ = source_part.m_sZ;
    result_parts->m_ChangeOfClassStep = source_part.m_ChangeOfClassStep;

    result_parts->m_PetSlotStream = source_part.m_PetSlotStream;
    result_parts->m_bSummoned = source_part.m_bSummoned;

    ASSERT(source_part.m_EquipItemInfo.m_Count <= MAX_EQUIPMENT_SLOT_NUM);
    ;{
        BOOST_STATIC_ASSERT(_countof(result_parts->m_EquipItemInfo.m_Slot) == 
                            _countof(source_part.m_EquipItemInfo.m_Slot));
        int number_of_items = source_part.m_EquipItemInfo.m_Count;
        ;   number_of_items = min(number_of_items, MAX_EQUIPMENT_SLOT_NUM);
        EQUIP_ITEM_INFO::SLOT_TYPE* dest_it = result_parts->m_EquipItemInfo.m_Slot;
        const _EQUIP_TOTAL_INFO::SLOT_TYPE* src_it = source_part.m_EquipItemInfo.m_Slot;
        const _EQUIP_TOTAL_INFO::SLOT_TYPE* src_end = \
            &source_part.m_EquipItemInfo.m_Slot[number_of_items];

        for (; src_it != src_end; ++src_it, ++dest_it)
        {
            dest_it->m_Pos = src_it->m_Pos;
            //       RENDER_ITEMSTREAMEX <- ITEMSTREAMEX
            CopyStream(dest_it->m_Stream, src_it->m_Stream);
        };
        result_parts->m_EquipItemInfo.m_Count = static_cast<POSTYPE>(number_of_items);
        //
    };

    result_parts->m_DelChk = source_part.m_DelChk;
    if (result_parts->m_DelChk != 0)
    {
        // make to pre-check and prevention
        //source_part.m_tszDelDate[MAX_SMALLDATETIME_SIZE] ='\0';
        strncpy(result_parts->m_tszDelDate, source_part.m_tszDelDate,
                _countof(result_parts->m_tszDelDate));
        result_parts->m_tszDelDate[_countof(result_parts->m_tszDelDate) - 1] = '\0';
    }

#ifdef _NA_002050_20110216_ADD_GENDER
    result_parts->gender = source_part.gender;
#endif 
    //_NA_004035_20120227_COSTUME_ITEM
    result_parts->is_equipped_costume = source_part.is_equipped_costume;
    result_parts->costume_item_pos = source_part.costume_item_pos;
    result_parts->costume_item_code = source_part.costume_item_code;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    result_parts->is_equipped_decorate_costume = source_part.is_equipped_deco_costume;
    result_parts->decorate_costume_item_pos = source_part.deco_costume_item_pos;
    result_parts->decorate_costume_item_code = source_part.deco_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
}

//==================================================================================================

void User::SetCharName(BYTE slot_index, const TCHAR* char_name)
{
    if (FlowControl::FCAssert(slot_index < MAX_CHARACTER_LIST_NUM) == false) {
        return;
    };
    SERVER_CHARACTER_PART* const char_part = &character_list_.character_infos->nodes[slot_index];
    _tcsncpy(char_part->m_tszCharName, char_name, _countof(char_part->m_tszCharName));
    char_part->m_tszCharName[_countof(char_part->m_tszCharName) - 1] = _T('\0');
}

// 유효한 슬롯인지 여부 처리
void User::SetOccupiedSlot(BYTE slot_index, bool val)
{
    if (bool out_range = (slot_index >= MAX_CHARACTER_LIST_NUM)) {
        return;
    };

    character_list_.occupied_slots[slot_index] = val;
    if (val)
    {
        // CHANGES: f110517.3L, added to configure hash value for the character name.
        const SERVER_CHARACTER_PART& src_part = character_list_.character_infos->nodes[slot_index];
        TCHAR lower_char_name[_countof(src_part.m_tszCharName)];
        _tcsncpy(lower_char_name, src_part.m_tszCharName, _countof(lower_char_name));
        lower_char_name[_countof(lower_char_name) - 1] = _T('\0');
        _tcslwr(lower_char_name);
        //
        util::HashKey hash_value = util::StreamHashKey::GetStringHashKey(lower_char_name);
        character_list_.character_infos->char_name_hashes[slot_index] = hash_value;
    }
    else
    {
        character_list_.character_infos->char_name_hashes[slot_index] = 0;
    }
}

