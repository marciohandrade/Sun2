#pragma once

//==================================================================================================


inline bool User::SelectChar(BYTE slot_index)
{
    return character_list_.SelectCharacter(slot_index);
}

inline BYTE User::GetSelectedCharIndex() const {
    if (character_list_.selected_char) {
        return character_list_.selected_char_slot_index;
    }
    return MAX_CHARACTER_LIST_NUM; // = _countof(character_list_.character_infos->nodes);
}

inline void User::SetNumberOfCreatedChars(BYTE number_of_chars)
{
    character_list_.number_of_characters = number_of_chars;
}

inline BYTE User::GetNumberOfCreatedChars() const
{
    return character_list_.number_of_characters;
}

inline DWORD User::GetCharGUID(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return character_list_.character_infos->nodes[slot_index].m_CharGuid;
    };
    return 0;
}

inline bool User::IsCharDelete(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return (character_list_.character_infos->nodes[slot_index].m_DelChk != 0);
    };
    return true; // default result
}

// UserSession inteface: TempUser시는 Null, User일 때 : 선택된 캐릭터
inline BYTE User::GetCharClass() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_byClass;
    };
    return 0;
}

// UserSession inteface:
inline LEVELTYPE User::GetCharLV() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_LV;
    };
    return 0;
}

// UserSession inteface:
inline const TCHAR* User::GetCharName() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_tszCharName;
    };
    return "";
}

inline LEVELTYPE User::GetCharLV(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return character_list_.character_infos->nodes[slot_index].m_LV;
    };
    return 0;
}

inline const TCHAR* User::GetCharName(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return character_list_.character_infos->nodes[slot_index].m_tszCharName;
    };
    return "";
}


// CHANGES: f110517.3L, added hash finding utility for char_name
inline DWORD User::GetCharNameHash(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return character_list_.character_infos->char_name_hashes[slot_index];
    };
    return 0;
}

// CHANGES: f110517.3L, added hash finding utility for char_name
inline DWORD User::GetSelectedCharNameHash() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->char_name_hashes[
            character_list_.selected_char_slot_index];
    };
    return 0;
}

inline const TCHAR* User::GetSelectedCharName() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_tszCharName;
    };
    return "";
}

inline CHARGUID User::GetSelectedCharGuid() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_CharGuid;
    };
    return 0;
}

inline LEVELTYPE User::GetSelectedCharLV() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_LV;
    };
    return 0;
}

inline BYTE User::GetSelectedCharClass() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_byClass;
    };
    return 0;
}

inline MAPCODE User::GetSelectedCharMapCode() const
{
    if (character_list_.selected_char) {
        return static_cast<MAPCODE>(character_list_.selected_char->m_iRegion);
    };
    return 0;
}

inline void User::SetSelectedCharMapCode(MAPCODE map_code)
{
    if (character_list_.selected_char)
    {
        character_list_.selected_char->m_iRegion = map_code;
    };
}

inline GUILDGUID User::GetSelectedCharGuildGuid() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_GuildGuid;
    };
    return 0;
}

inline void User::SetSelectedCharGuildGuid(GUILDGUID guild_guid)
{
    if (character_list_.selected_char) {
        character_list_.selected_char->m_GuildGuid = guild_guid;
    };
}

inline BYTE User::GetSelectedCharState() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->m_CharState;
    };
    return 0;
}


inline void User::SetSelectedCharMaxHP(DWORD max_hp)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->max_hp = max_hp;
    };
}

inline DWORD User::GetSelectedCharMaxHP() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->max_hp;
    };
    return 0;
}

inline void User::SetSelectedCharCurHP(DWORD cur_hp)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->cur_hp = cur_hp;
    };
}

inline DWORD User::GetSelectedCharCurHP() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->cur_hp;
    };
    return 0;
}

inline bool User::IsDead() const
{
    return (GetSelectedCharCurHP() == 0) ? true : false;
}


inline void User::SetSelectedCharMaxMP(DWORD max_mp)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->max_mp = max_mp;
    };
}

inline DWORD User::GetSelectedCharMaxMP() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->max_mp;
    };
    return 0;
}

inline void User::SetSelectedCharCurMP(DWORD cur_mp)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->cur_mp = cur_mp;
    };
}

inline DWORD User::GetSelectedCharCurMP() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->cur_mp;
    };
    return 0;
}


inline void User::SetSelectedCharMaxSD(DWORD max_sd)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->max_sd = max_sd;
    };
}

inline DWORD User::GetSelectedCharMaxSD() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->max_sd;
    };
    return 0;
}

inline void User::SetSelectedCharCurSD(DWORD cur_sd)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->cur_sd = cur_sd;
    };
}

inline DWORD User::GetSelectedCharCurSD() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->cur_sd;
    };
    return 0;
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
inline void User::SetSelectedCharMaxFP(PLAYERFP max_fp) {
    if (character_list_.selected_char) {
        character_list_.character_infos->max_fp = max_fp;
    };
}
inline PLAYERFP User::GetSelectedCharMaxFP() const {
    if (character_list_.selected_char) {
        return character_list_.character_infos->max_fp;
    };
    return 0;
}
inline void User::SetSelectedCharCurFP(PLAYERFP cur_fp) {
    if (character_list_.selected_char) {
        character_list_.character_infos->cur_fp = cur_fp;
    };
}
inline PLAYERFP User::GetSelectedCharCurFP() const {
    if (character_list_.selected_char) {
        return character_list_.character_infos->cur_fp;
    };
    return 0;
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

inline void User::SetSelectedCharChaoState(BYTE chao_state)
{
    if (character_list_.selected_char) {
        character_list_.character_infos->chao_state = chao_state;
    };
}

inline BYTE User::GetSelectedCharChaoState() const
{
    if (character_list_.selected_char) {
        return character_list_.character_infos->chao_state;
    };
    return 0;
}

inline bool User::IsChaoState() const
{
    if (character_list_.selected_char) {
        return (character_list_.character_infos->chao_state == eCHAO_STATE_CHAO);
    };
    return false;
}

inline void User::SetCharLevel(BYTE slot_index, LEVELTYPE LV)
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        character_list_.character_infos->nodes[slot_index].m_LV = LV;
    };
}

inline void User::SetCharDelChk(BYTE slot_index, BYTE chk)
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        character_list_.character_infos->nodes[slot_index].m_DelChk = chk;
    };
}

#ifdef _NA_002050_20110216_ADD_GENDER
inline BYTE User::GetSelectedCharGender() const
{
    if (character_list_.selected_char) {
        return character_list_.selected_char->gender;
    };
    return GenderType::kAll;
}
#endif


inline void User::SerializeCharacterPart(BYTE slot_index, SERVER_CHARACTER_PART& INOUT parts, eSERIALIZE se)
{
    character_list_.SerializeCharacterPart(slot_index, parts, se);
}

inline void User::LoadClientCharacterPart(BYTE slot_index,
    CLIENT_CHARACTER_PART* const result_parts) const
{
    character_list_.LoadClientCharacterPart(slot_index, result_parts);
}

inline bool User::IsValidSlot(BYTE slot_index) const
{
    if (slot_index < MAX_CHARACTER_LIST_NUM) {
        return character_list_.occupied_slots[slot_index];
    };
    return false;
}


//==================================================================================================

inline VOID
User::SetNeedChangedStatus(PlayerStatEvent::eFlag statEvent)
{
    if(statEvent == PlayerStatEvent::eFlag_None)
        m_PlayerStateEvent = PlayerStatEvent::eFlag_None;
    else
        m_PlayerStateEvent = m_PlayerStateEvent | statEvent;
}

inline BYTE
User::GetNeedChangedStatus() const
{
    return m_PlayerStateEvent;
}

//==================================================================================================
// NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
inline void User::ChangeWorldServiceEnabledState(bool enabled)
{
    world_service_enabled_state_ = enabled;
}

// NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
inline bool User::EnabledWorldService() const
{
    return world_service_enabled_state_;
}
