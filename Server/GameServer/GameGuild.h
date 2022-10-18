 #pragma once
#ifndef GAMESERVER_GUILD_H
#define GAMESERVER_GUILD_H

//==================================================================================================
/// GameGuild class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2005. 12. 27
    @remark
        - 길드 정보를 활용하는 목적의 게임 길드용 클래스
    @note
        - 
    @history 
        - 05.12.27, taiyo, created
        - 08.01.02~08.04.29, shaketake, renewaled guild system
        - 11.05.06, waverix, rearrangements, changes data structure and unified packet broadcaster.
        -                    f110506.8L, enhancemant ratio specification
        -                    Space Complexity: G->A, O(n^2) -> O(n)
        -                    Time Complexity: A->C, O(n^2) -> O(n)
*/
//==================================================================================================

#include "Player.h"
#include "GameGuildCommon.h"
#include "GameGuildMember.h"
#include "GameGuildRelation.h"
#include "GuildWareHouseSlotContainer.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilities.h"
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosZoneSystem\SonnenScheinRewardInfoParser.h"
#include "DominationInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
//==================================================================================================

class Player;
class GameGuildMember;
struct PACKET_FOR_GUILD;

//==================================================================================================

#ifdef _NA_008334_20150608_SONNENSCHEIN
//GuildRewardOption 관련
class GuildRewardOptionManager
{
public:
    GuildRewardOptionManager(GameGuild* master) {game_guild_ = master;}
    ~GuildRewardOptionManager() {}

    VOID AddDominationRewardOption(const DominationInfoParser::SelectedRewardOptions& sel_options);
    VOID AddSonnenScheinRewardOption(const SScheinRewardInfo* reward_info);

    //domination, sonnenschein 보상옵션들을 option_map_에 추가한다
    VOID SetRewardOptionMap(const GUILDGUID& guild_guid, const BYTE& sschein_phase);  
    VOID ApplyRewardOption(Player* const player, BOOL is_turn_on = TRUE );

private:
    std::map<WORD/*option_kind*/, AttrOptionInfo> option_map_;

    VOID _AddOption(WORD option_kind, BYTE option_type, INT option_value);
    VOID _ResetOptionMap();

    GameGuild* game_guild_;
};
#endif //_NA_008334_20150608_SONNENSCHEIN
//==================================================================================================

class GameGuild
{
    __DECL_GUILDPOOL(GameGuild);
    enum { _MAX_GUILDMEMBER_POOL_SIZE = 50, };
public:
    GameGuild();
    virtual ~GameGuild();

    void Create(const GAME_GUILD_INFO& guild_info, const BASE_GUILD_MEMBER_INFO& guild_member_info);
    void Destroy();

    void Withdraw(CHARGUID char_guid);
    GUILDGUID GetGuildGuid() const;
    const TCHAR* GetGuildName() const;
    void ChangeGuildName(const TCHAR* change_guild_name);

    DWORD GetMemberNum() const;
    GameGuildMember* FindMember(CHARGUID char_guid) const;
    inline const GUILDMEMBER_HASH& GetMemberHash() const;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    void SetChuntingPVPInfo(CHUNTING_PVP_INFO & IN pvp_info);
#endif
    GameGuildMember* FindMaster() const;

    void SetMemberDuty(CHARGUID char_guid, eGUILD_DUTY guild_duty);

    BOOL IsCreateRearCamp(BYTE number_of_corps);
    BYTE GetGuildGrade() const;

    void CreateRearCamp(BYTE number_of_corps);
    void DestroyRearCamp(BYTE number_of_corps);
    void DestroyMember(CHARGUID char_guid);
    void Join(const BASE_GUILD_MEMBER_INFO& guild_member_info);
    void AddMember(const BASE_GUILD_MEMBER_INFO& guild_member_info);
    
    void SetGuildGrade(BYTE byGrade);
    void PartyToLeaveByRelation(DWORD hostility_guild_guid);

    int GetRelationGuid(STLX_VECTOR<GUILDGUID>& result_guild_guid_list,
                        eGUILD_RELATION relation_type = eGUILD_RELATION_NONE);
    eGUILD_RELATION GetRelationType(GUILDGUID guild_guid);

    void GetGuildMark(GUILDMARKIDX& background, GUILDMARKIDX& pattern) const;
    void DeleteGuildMark();
    void RegisterGuildMark(GUILDMARKIDX bImage, GUILDMARKIDX pImage);
    CGuildWarehouseSlotContainer* GetGuildWareHouse();
    GuildRelationSystem::GuildRelationEntry* GetRelationEntry();

    //!~ _NA_000000_20130429_DOMINATION_PENALTY
    int GetMemberCountInDominataionFiled(const MAPCODE map_code);
    //~! _NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    Player* GetGuildMaster();
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilities*const GetGuildFacilities();
    const RC::eGUILD_RESULT CanUpdateGuildFacility(Player* player, FCODE code, FLEVEL add_level = 1);
    const void UpdateGuildFacility(CHARGUID char_guid, GUILD_FACILITY_INFO info, FLEVEL cur_level, FLEVEL add_level);
    const RC::eGUILD_RESULT ApplyGuildFacilityActive(Player* player, FCODE code);
    const void ApplyGuildFacilityPassive(Player* player, const bool is_add, const GUILD_FACILITY_MAP*  facility_map);
    const void ApplyGuildFacilityPassiveAllMember(const bool is_add, const GUILD_FACILITY_MAP*  facility_map);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //-----
    // packet broadcaster
    void SendPacketAll(MSG_BASE* msg, int msg_size, CHARGUID except_char_guid = 0);
    template<typename _MsgTypeForMember>
    void SendPacketMember(_MsgTypeForMember& member_msg);
    template<typename _MsgType>
    void SendPacketMemberAround(_MsgType& msg, GUILDGUID other_guild_guid);
    template<typename _MsgType>
    void SendPacketFieldMember(_MsgType& msg, const FIELDCODE field_code);
    template<typename _MsgTypeForMember, typename _MsgTypeArounding> 
    void SendPacketMemberAndAroundAll(_MsgTypeForMember& member_msg,
                                      _MsgTypeArounding& arounding_msg);

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID ApplyGuildRewardOption(Player* const player, BOOL is_turn_on = TRUE);
    VOID ApplyGuildRewardOptionForAllMembers(BOOL is_turn_on);    

    VOID InitializeSonnenScheinRewardPhase();
    VOID SetSonnenScheinRewardPhase(BYTE phase) {guild_info_.sonnenschein_phase_ = phase;}
    BYTE GetSonnenScheinRewardPhase() {return guild_info_.sonnenschein_phase_;}
#endif //_NA_008334_20150608_SONNENSCHEIN
private:
    //
    void RemoveMember(CHARGUID char_guid);
    void AddMember(GameGuildMember* guild_member, CHARGUID char_guid);
    //
    void _SendPacketMember(MSG_BASE* msg, uint16_t msg_size);
    void _SendPacketMemberAround(MSG_BASE* msg, uint16_t msg_size,
                                 DWORD* const msg_object_key_slot);
    void _SendPacketFieldMember(MSG_BASE* msg, uint16_t msg_size, FIELDCODE field_code);
    void _SendPacketMemberAndAroundAll(MSG_BASE* member_msg, uint16_t member_msg_size,
                                       MSG_BASE* arounding_msg, uint16_t arounding_msg_size,
                                       DWORD* const arounding_msg_object_key_slot);
    //
    // data fields...
    MONEY               rankup_money_;
    GUILDMEMBER_HASH    member_table_;
    GAME_GUILD_INFO     guild_info_;

    bool checked_rear_camps_[MAX_GUILD_CORPS_NUM];
    CGuildWarehouseSlotContainer guild_warehouse_;
    GuildRelationSystem::GuildRelationEntry guild_relation_entry_;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilities* guild_facilities_;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    GuildRewardOptionManager* reward_option_mgr_;
#endif //_NA_008334_20150608_SONNENSCHEIN
};

//==================================================================================================
// inlining implements
inline GUILDGUID GameGuild::GetGuildGuid() const {
    return guild_info_.m_GuildGuid;
}

inline const TCHAR* GameGuild::GetGuildName() const {
    return guild_info_.m_tszGuildName;
}

// 길드 직책 설정
inline GameGuildMember* GameGuild::FindMaster() const 
{
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        if (member->GetDuty() == 1) {
            return member;
        }
    }
    return NULL;
}

inline DWORD GameGuild::GetMemberNum() const {
    return static_cast<DWORD>(member_table_.size());
}

inline const GUILDMEMBER_HASH& GameGuild::GetMemberHash() const {
    return member_table_;
}

inline BYTE GameGuild::GetGuildGrade() const {
    return guild_info_.m_GuildGrade;
}

inline eGUILD_RELATION GameGuild::GetRelationType(GUILDGUID GuildGuid) {
    return guild_relation_entry_.GetRelationType(GuildGuid);
}

inline void GameGuild::GetGuildMark(GUILDMARKIDX& background, GUILDMARKIDX& pattern) const
{
    pattern = guild_info_.m_PatternIdx;
    background = guild_info_.m_BackGroundIdx;
}

inline void GameGuild::DeleteGuildMark() 
{ 
    guild_info_.m_PatternIdx = 0; 
    guild_info_.m_BackGroundIdx= 0; 
}

inline void GameGuild::RegisterGuildMark(GUILDMARKIDX bImage, GUILDMARKIDX pImage) 
{ 
    guild_info_.m_BackGroundIdx = bImage; 
    guild_info_.m_PatternIdx = pImage; 
}

inline CGuildWarehouseSlotContainer* GameGuild::GetGuildWareHouse() {
    return &guild_warehouse_;
}

inline GuildRelationSystem::GuildRelationEntry* GameGuild::GetRelationEntry() {
    return &guild_relation_entry_;
}
//-----

//==================================================================================================


template<typename _MsgType> 
void GameGuild::SendPacketMemberAround(_MsgType& msg, GUILDGUID other_guild_guid)
{
    __UNUSED(other_guild_guid);
    _SendPacketMemberAround(&msg, msg.GetSize(), &msg.m_dwPlayerKey);
}

template<typename _MsgTypeForMember> 
void GameGuild::SendPacketMember(_MsgTypeForMember& member_related_msg)
{
    _SendPacketMember(&member_related_msg, member_related_msg.GetSize());
}


template<typename _MsgTypeForMember, typename _MsgTypeArounding> 
void GameGuild::SendPacketMemberAndAroundAll(_MsgTypeForMember& member_related_msg,
                                             _MsgTypeArounding& arrounding_msg)
{
    _SendPacketMemberAndAroundAll(&member_related_msg, member_related_msg.GetSize(),
        &arrounding_msg, arrounding_msg.GetSize(), &arrounding_msg.m_dwPlayerKey);
}

template<typename _MsgType> 
void GameGuild::SendPacketFieldMember(_MsgType& msg_brd, const FIELDCODE field_code)
{
    _SendPacketFieldMember(&msg_brd, msg_brd.GetSize(), field_code);
}


#endif //GAMESERVER_GUILD_H