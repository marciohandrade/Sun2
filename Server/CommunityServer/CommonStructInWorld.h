#pragma once
#ifndef WORLDSERVER_COMMON_STRUCT_IN_WORLD_H
#define WORLDSERVER_COMMON_STRUCT_IN_WORLD_H

//==================================================================================================

namespace ns_world {
;

typedef ulong NAMEHASH; // NAMEHASH = util::StreamHashKey::GetStringHashKey(char_name)

struct UserAccountInfo
{
    DWORD user_guid;
    ns_world::community::WorldChannel* channel;
    //uint8_t channel_id;
    uint8_t gm_grade;
    DWORD pc_room_id;
    TCHAR account_id[ID_MAX_LEN + 1];
    char ip_address[MAX_IP_STRING_LEN + 1];
};

struct ConnectedCharInfo
{
    USERGUID user_guid;
    CHARGUID char_guid;
    GUILDGUID guild_guid;
    LEVELTYPE char_level;
    uint8_t char_class;
    uint8_t zone_type;
    DWORD zone_key;
    MAPCODE map_code;
    FIELDCODE field_code;
    uint16_t party_key;
    WzVector position;
    util::StrArrayForCharName char_name;
    ns_world::NAMEHASH char_name_hash;
};

struct ChattingMode
{
    bool whispered_mode;
    bool chatting_blocked;
    bool friend_chatting_blocked;
    bool whispering_intercepted;
    ulong chatting_blocking_time; // server aging counter + blocking time (second)
};

// NOTE: group data in user
class GroupLink
{
public:
    typedef STLX_MAP<CHARGUID, STRUCT_GROUP_MEMBER_INFO> GROUP_MEMBER_MAP;

    GroupLink();
    ~GroupLink();

    void Init(ns_world::WorldUser* user);
    bool Linked() const;
    bool IsTransaction() const;
    bool IsAddingPrevented() const;
    const MONEY& GetStackedBonusMoney() const;
    const EXPTYPE& GetStackedBonusExp() const;
    const EXPTYPE& GetAccumulatedExp() const;
private:
    void Clear();

    ns_world::WorldUser* user_;
    GROUP_MEMBER_MAP* link_;

    eGROUP_STAT group_state_high_member_; //상위 맴버의 상태
    eGROUP_STAT group_state_low_member_; //하위 맴버의 상태
    //STRUCT_GROUP_MEMBER_INFO m_tempMemberInfo; //맴버 정보를 임시로 저장하는 변수
    bool transaction_; //DB작업 트렌젝션 용
    bool adding_prevented_; //현재 그룹 맴버 추가 토글이 켜있는가?
    MONEY stacked_bonus_money_; // NOTE: (stack + stack + stack + ) -> flush, 'm_curBonusMoney'
    EXPTYPE stacked_bonus_exp_; // NOTE: (stack + stack + stack + ) -> flush, 'm_curBonusEXP'
    EXPTYPE accumulated_exp_; // NOTE: updated info. from db stored data, 'm_curAccumEXP', <= MAX_GROUP_EXP
    //
    struct {
        STRUCT_GROUP_MEMBER_INFO* member_addition_data; // group connecting data
    } transaction_data_;
    //
    friend class ns_world::GroupManager;
    __DISABLE_COPY(GroupLink);
};

struct CharStatusDataNode
{
    static const DWORD kCharStatusDataTag = ('S') | ('t' << 8) | ('A' << 16) | ('t' << 24);
    DWORD char_status_data_tag;
    ns_world::WorldUser* online_user;
    bool friend_configured;
    bool block_configured;
    bool revenge_configured;
    STRUCT_FRIEND_INFO friend_info;
    STRUCT_BLOCK_INFO block_info;
    STRUCT_REVENGE_INFO revenge_info;
    //STRUCT_GROUP_MEMBER_INFO group_info;
    //
    void Initialize();
    static CharStatusDataNode* GetContainerObject(const STRUCT_FRIEND_INFO* contained_item);
    static CharStatusDataNode* GetContainerObject(const STRUCT_BLOCK_INFO* contained_item);
    static CharStatusDataNode* GetContainerObject(const STRUCT_REVENGE_INFO* contained_item);
};

struct ICharStatusDataNodeAccessor
{
    // if the character is not found, new data structure is inserted.
    virtual ns_world::CharStatusDataNode* GetCharStatusData(const CHARGUID char_guid) = 0;
};

class FriendLink
{
public:
    typedef STLX_MAP<CHARGUID, STRUCT_FRIEND_INFO*> FRIEND_INFO_MAP;

    FriendLink();
    ~FriendLink();

    void Init(ns_world::WorldUser* user);
    //
private:
    void Clear();
    //
    ns_world::WorldUser* user_;
    FRIEND_INFO_MAP* link_;
    bool transaction_; // db transaction = { FriendLink, BlockCharLink, RevengeLink }
    //
    friend class ns_world::FriendMap;
    __DISABLE_COPY(FriendLink);
};


class BlockCharLink
{
public:
    typedef STLX_MAP<CHARGUID, STRUCT_BLOCK_INFO*> BLOCK_INFO_MAP;

    BlockCharLink();
    ~BlockCharLink();

    void Init(ns_world::WorldUser* user);
    //
private:
    void Clear();
    //
    ns_world::WorldUser* user_;
    BLOCK_INFO_MAP* link_;
    //
    friend class ns_world::FriendMap;
    __DISABLE_COPY(BlockCharLink);
};


class RevengeLink
{
public:
    typedef STLX_MAP<CHARGUID, STRUCT_REVENGE_INFO*> REVENGE_INFO_MAP;

    RevengeLink();
    ~RevengeLink();

    void Init(ns_world::WorldUser* user);
    //
private:
    void Clear();
    //
    ns_world::WorldUser* user_;
    REVENGE_INFO_MAP* link_;
    //
    uint8_t revenge_point_;
    //
    friend class ns_world::FriendMap;
    __DISABLE_COPY(RevengeLink);
};

class MemoLink
{
public:
    MemoLink();
    ~MemoLink();

    void Init(ns_world::WorldUser* user);
private:
    void Clear();
    //
    ns_world::WorldUser* user_;
    bool transaction_;
    ulong blocking_memo_list_request_; // aging timer (seconds based)
    //
    friend class ns_world::WorldSystemMemo;
    __DISABLE_COPY(MemoLink);
};

struct WorldShoutingArg
{
    bool whole_shouting;
    bool using_item;
};

// 
struct IShutdown
{
    virtual void Shutdown() = 0;
};

//==================================================================================================
}; //end of namespace
//==================================================================================================

inline ns_world::GroupLink::GroupLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline ns_world::GroupLink::~GroupLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline void ns_world::GroupLink::Init(ns_world::WorldUser* user)
{
    assert(link_ == NULL && transaction_data_.member_addition_data == NULL);
    ZeroMemory(this, sizeof(*this));
    user_ = user;
}

inline bool ns_world::GroupLink::Linked() const {
    return (user_ != NULL);
};

inline bool ns_world::GroupLink::IsTransaction() const {
    return transaction_;
};

inline bool ns_world::GroupLink::IsAddingPrevented() const {
    return adding_prevented_;
};

inline const MONEY& ns_world::GroupLink::GetStackedBonusMoney() const {
    return stacked_bonus_money_;
};

inline const EXPTYPE& ns_world::GroupLink::GetStackedBonusExp() const {
    return stacked_bonus_exp_;
};

inline const EXPTYPE& ns_world::GroupLink::GetAccumulatedExp() const {
    return accumulated_exp_;
};

inline void ns_world::GroupLink::Clear() {
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
};


inline ns_world::FriendLink::FriendLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline ns_world::FriendLink::~FriendLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline void ns_world::FriendLink::Init(ns_world::WorldUser* user)
{
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
    user_ = user;
}

inline void ns_world::FriendLink::Clear() {
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
};


inline ns_world::BlockCharLink::BlockCharLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline ns_world::BlockCharLink::~BlockCharLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline void ns_world::BlockCharLink::Init(ns_world::WorldUser* user)
{
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
    user_ = user;
}

inline void ns_world::BlockCharLink::Clear() {
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
};



inline ns_world::RevengeLink::RevengeLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline ns_world::RevengeLink::~RevengeLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline void ns_world::RevengeLink::Init(ns_world::WorldUser* user)
{
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
    user_ = user;
}

inline void ns_world::RevengeLink::Clear() {
    assert(link_ == NULL);
    ZeroMemory(this, sizeof(*this));
};


inline ns_world::MemoLink::MemoLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline ns_world::MemoLink::~MemoLink()
{
    ZeroMemory(this, sizeof(*this));
}

inline void ns_world::MemoLink::Init(ns_world::WorldUser* user)
{
    ZeroMemory(this, sizeof(*this));
    user_ = user;
}

inline void ns_world::MemoLink::Clear() {
    ZeroMemory(this, sizeof(*this));
};


//==================================================================================================
#endif //WORLDSERVER_COMMON_STRUCT_IN_WORLD_H