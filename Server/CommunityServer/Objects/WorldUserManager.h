#pragma once
#ifndef WORLDSERVER_WORLD_USER_MANAGER_H
#define WORLDSERVER_WORLD_USER_MANAGER_H

namespace util {
template<class T> class MemoryPoolFactory;
};

namespace ns_functor {
;
struct LogoutOption;
}; //end of namespace

struct ns_functor::LogoutOption
{
    bool notification_;
    struct Reuse;
    struct Free;
};

struct ns_functor::LogoutOption::Reuse : public ns_functor::LogoutOption
{
    Reuse(bool notification) { notification_ = notification; }
    __DISABLE_COPY(Reuse);
};

struct ns_functor::LogoutOption::Free : public ns_functor::LogoutOption
{
    Free(bool notification) { notification_ = notification; }
    __DISABLE_COPY(Free);
};


//==================================================================================================
namespace ns_world {
;
class WorldUser;


//==================================================================================================

class WorldUserManager : public ns_world::IShutdown
                       , public ns_world::ICharStatusDataNodeAccessor
{
public:
    typedef STLX_HASH_MAP<DWORD, ns_world::WorldUser*> UserTable;
    typedef STLX_HASH_MAP<NAMEHASH, NAMEHASH> NameHashTable;
    typedef STLX_HASH_MAP<CHARGUID, ns_world::CharStatusDataNode> CharStatusDataTable;
    //
    static WorldUserManager* Instance();
    // ns_world::IShutdown interface
    virtual void Shutdown();

    //void Init();
    //void Release();
    void UpdateServices(
        const util::ServerTimeInfoPerFrame& time_info,
        ns_world::ServerSessionInWorld* const dbproxy);
    ns_world::WorldUser* FindUser(DWORD user_guid) const;
    ns_world::WorldUser* FindUserByCharName(const util::StrArrayForCharName& char_name) const;
    ns_world::WorldUser* FindUserByCharGuid(DWORD char_guid) const;
    //----------------------------------------------------------------------------------------------
    // NOTE: name hash finding table controllers
    static NAMEHASH MakeNameHash(const util::StrArrayForCharName& char_name);
    static bool FindNameHashByCharGuid(const CHARGUID char_guid, NAMEHASH* const result_name_hash);
    static bool FindCharGuidByNameHash(const NAMEHASH name_hash, CHARGUID* const result_char_guid);
    static bool FindCharGuidByCharName(
        const util::StrArrayForCharName& char_name, CHARGUID* const result_char_guid);
    static bool UpdateNameHash(const CHARGUID char_guid,
        const util::StrArrayForCharName& char_name, NAMEHASH* const result_name_hash);
    //----------------------------------------------------------------------------------------------
    enum UpdateCharStatusDataEnum {
        eUpdateCharStatus_Online, eUpdateCharStatus_Offline, eUpdateCharStatus_Refresh
    };
    static void UpdateCharStatusData(
        ns_world::WorldUser* user, UpdateCharStatusDataEnum update_type);
    //----------------------------------------------------------------------------------------------

    void SendToAll(MSG_BASE* msg_base, uint16_t msg_size);
    // use in { AW_PREPARE_NEW_USER_SYN, AW_USER_RESTORE_CMD }
    ns_world::WorldUser* AllocNewUser();
    // use in { AW_PREPARE_NEW_USER_SYN, AW_USER_RESTORE_CMD }
    bool AddUser(ns_world::WorldUser* user);
    //bool RemoveUser(ns_world::WorldUser* user);
    //
    void OnCloseChannel(ns_world::community::WorldChannel* closing_channel);
    template<class _LogoutOption>
    static bool Logout(ns_world::WorldUser* user, const _LogoutOption& option);
    //
    bool ChangeUserAccountInfo(ns_world::WorldUser* user,
        const ns_world::UserAccountInfo& user_info);
    // NOTE: f110922.2L, side-effects = {
    //  MSG_DW_FRIEND_GETLIST_SYN,
    //  MSG_DW_GM_CHATBLOCKTIME_REQ,
    //  MSG_DW_STATUS_INTERCEPTION_REQ
    // }
    bool ChangeConnectedCharInfo(ns_world::WorldUser* user,
        const ns_world::ConnectedCharInfo& char_info);
    bool ChangeChattingMode(ns_world::WorldUser* user,
        const ns_world::ChattingMode& chatting_mode);

private:
    typedef util::CMemoryPoolFactory<ns_world::WorldUser> WorldUserPool;
    //
    WorldUserManager();
    virtual ~WorldUserManager();
    static WorldUserManager* CreateInstance();
    //
    static bool LogoutInternal(ns_world::WorldUser* user);
    // NOTE: ns_world::ICharStatusDataNodeAccessor interface
    // if the character is not found, new data structure is inserted.
    virtual ns_world::CharStatusDataNode* GetCharStatusData(const CHARGUID char_guid);
    // data fields...
    UserTable user_key_table_;
    UserTable char_key_table_;
    UserTable char_name_hash_table_;
    WorldUserPool* world_user_pool_;
    NameHashTable cache_namehash_to_charguid_table_;
    NameHashTable cache_charguid_to_namehash_table_;
    CharStatusDataTable char_status_data_table_;
    //
    util::ITimerBase update_service_execute_timer_;
    util::ITimerBase chatting_block_update_timer_;
    // last order...
    static WorldUserManager* instance_static_;
    //
    friend class ns_world::community::WorldChannel;
    __DISABLE_COPY(WorldUserManager);
}; //end of class

}; //end of namespace

//==================================================================================================
inline ns_world::WorldUserManager* ns_world::WorldUserManager::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

#if WORLD_CONTENTS_BLOCK
class GuidUserList;
class CharNameUserList;

struct STRUCT_FRIEND_INFO;
struct STRUCT_BLOCK_INFO;
struct STRUCT_REVENGE_INFO;

//==================================================================================================
namespace ns_world {
;

class WorldUserManager
{
public:
    static WorldUserManager* Instance();

    void Init();
    void Release();
    ns_world::WorldUser* FindUser(DWORD user_guid) const;
    ns_world::WorldUser* FindUser(std::string strCharName);
    ns_world::WorldUser* FindUserByCharGuid(DWORD char_guid) const;

    void SendToAll(MSG_BASE* msg_base, uint16_t msg_size);

    BOOL SetUserCharGuid(DWORD dwGuid, DWORD dwCharGuid);
    BOOL SetUserCharName(DWORD dwGuid, std::string strCharName);
    BOOL UnSetCharGuid(DWORD dwGuid);
    BOOL UnsetUserCharName(DWORD dwGuid);
    DWORD GetNumberOfGuidUsers();
    DWORD GetNumberOfCharNameUsers();

    // ģ�� ����
    BOOL AddNewFriend(DWORD FriendCharGuid, ns_world::WorldUser* world_user); // ���� �߰��� ģ���� ��Ͽ� �߰�
    BOOL AddFriend(ns_world::WorldUser* pUser); // �� ģ���� ��� �߰��ϰ�, pUser���� �¶����� ģ�� ���� �־��ش�
    void RemoveNewFriend(DWORD FriendCharGuid, ns_world::WorldUser* world_user); // ���� ������ ģ���� ��Ͽ��� ����
    void RemoveFriend(ns_world::WorldUser* pUser); // �� ģ���� ��� �����Ѵ�.

    // ������ ����
    BOOL AddNewRevenge(DWORD RevengeCharGuid, ns_world::WorldUser* world_user); // ���� �߰��� ���� ����� ��Ͽ� �߰�
    BOOL AddRevenge(ns_world::WorldUser* pUser); // �� ���� ���� ��� �߰��ϰ�, pUser���� �¶����� ������ ���� �־��ش�
    void RemoveNewRevenge(DWORD RevengeCharGuid, ns_world::WorldUser* world_user); // ���� ������ ������ ����� ��Ͽ��� ����
    void RemoveRevenge(ns_world::WorldUser* pUser); // �� ������ ��� �����Ѵ�.

    // ��ȣȭ ����
    void AddCryptoUser(ns_world::WorldUser* world_user); // ��ȣȭ ��� �߰�
    void UpdateCryptoUser(); // ��ȣȭ ��� �߿� ���� ���� ������ �����ϸ� �ҹ� ������ �����ϰ� ������� �Ѵ�

    // ä�� ���� ����
    void InsertChatBlockUser(ns_world::WorldUser* world_user, DWORD dwRemainTime);
    BOOL RemoveChatBlockUser(ns_world::WorldUser* world_user);
    void UpdateChatBlockUser();

    // ģ��/������/���� ����
    STRUCT_FRIEND_INFO*     AllocFriendInfo();
    void FreeFriendInfo(STRUCT_FRIEND_INFO *pFriendInfo);
    STRUCT_REVENGE_INFO*    AllocRevengeInfo();
    void FreeRevengeInfo(STRUCT_REVENGE_INFO *pRevengeInfo);
    STRUCT_BLOCK_INFO*      AllocBlockInfo();
    void FreeBlockInfo(STRUCT_BLOCK_INFO *pBlockInfo);

    BOOL SetUserCharLevel(DWORD dwGuid, LEVELTYPE CharLevel);
    BOOL SetUserCharClass(DWORD dwGuid, BYTE byClass);

private:
    //----------------------------------------------------------------------------------------------
    typedef STLX_MAP<DWORD, ns_world::WorldUser*>              FRIEND_ATOM_MAP;
    typedef FRIEND_ATOM_MAP::iterator           FRIEND_ATOM_MAP_ITER;
    typedef std::pair< DWORD, ns_world::WorldUser* >           FRIEND_ATOM_MAP_PAIR;

    typedef FRIEND_ATOM_MAP                     CHATBLOCK_USER_MAP;
    typedef FRIEND_ATOM_MAP_ITER                CHATBLOCK_USER_MAP_ITER;
    typedef FRIEND_ATOM_MAP_PAIR                CHATBLOCK_USER_MAP_PAIR;

    typedef STLX_MAP<DWORD, FRIEND_ATOM_MAP>    FRIEND_USER_MAP;
    typedef FRIEND_USER_MAP::iterator           FRIEND_USER_MAP_ITER;
    typedef std::pair< DWORD, FRIEND_ATOM_MAP > FRIEND_USER_MAP_PAIR;

    typedef STLX_VECTOR<ns_world::WorldUser*>                  CRYPTO_USER_VECTOR;
    typedef CRYPTO_USER_VECTOR::iterator        CRYPTO_USER_VECTOR_ITER;
    //----------------------------------------------------------------------------------------------

    WorldUserManager();
    ~WorldUserManager();
    static WorldUserManager* CreateInstance();
    // fields...
    void AddUser(DWORD dwGuid, ns_world::WorldUser* world_user);
    void RemoveUser(DWORD dwGuid);

    GuidUserList            *m_pGuidUserList;
    GuidUserList            *m_pCharGuidList;
    CharNameUserList        *m_pCharNameUserList;   // Add�� Find���� �ݵ�� �빮�ڷ� �ٲپ� ����Ѵ� - ������ ��

    FRIEND_USER_MAP         m_mapUserFriend;    // ģ�� ���� ����� ���� ���޿� ��
    FRIEND_USER_MAP         m_mapUserRevenge;   // ���� ����� ����� ���� ���޿� ��

    CRYPTO_USER_VECTOR      m_vecUserCrypt;     // ��ȣ Ű ���� ����� ������ ������ ��
    CHATBLOCK_USER_MAP      m_mapChatBlocked;   // ä�ñ����� ����� ���

    util::CMemoryPoolFactory<STRUCT_FRIEND_INFO>    *m_pFriendPool;
    util::CMemoryPoolFactory<STRUCT_BLOCK_INFO>     *m_pBlockPool;
    util::CMemoryPoolFactory<STRUCT_REVENGE_INFO>   *m_pRevengePool;
}; //end of class

}; //end of namespace
#endif //WORLD_CONTENTS_BLOCK
#endif //WORLDSERVER_WORLD_USER_MANAGER_H