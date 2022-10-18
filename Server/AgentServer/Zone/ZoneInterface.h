#ifndef AGENTSERVER_ZONE_INTERFACE_H
#define AGENTSERVER_ZONE_INTERFACE_H
#pragma once
//==================================================================================================
/// ZoneInterface 클래스
/**
    @author    Kim Min Wook < taiyo@webzen.com >
    @since    2004. 12. 29
    @remark
        - 
    @note
        - 
    @history 
        - 
    @usage
        - Create()    : 마을생성시 호출된다.
        - Destroy() : 
        - _CanJoinEach() : Room, Zone 별로 달라지는 입장 조건. (재정의)
        - _CanJoinAll()  : 모든 Room들이 공통으로 체크하는 조건.  (재정의)
        -
        - |09.05.20|waverix|SetMapCode, GetMapCode Common Utilization
        - |10.03.02|waverix|changes coding style related naming, class member ordering and inlining
                            TODO : 'SolarHashtable' change to other generic data structure, and
                            changes related operations
*/
//==================================================================================================

#include <SolarHashTable.h>
#include "ZoneCommon.h"

class User;
class ServerSession;
class Party;

enum ZONE_CHECK_TYPE
{
    CHECK_RETURN_PARTY, // 비정상종료 유저의 파티복귀가 가능한 존인지 체크
};

//==================================================================================================
//    <ZoneInterface>
//
class ZoneInterface
{
public:
    typedef STLX_MAP<DWORD, User*> ZONE_USER_LIST; // similar to USER_HASH={UserKey, User*}

    ZoneInterface();
    virtual ~ZoneInterface();

    void Create();
    void Destroy();
    void EnterUser(User* user);
    void LeaveUser(User* user,
                   eSEND_TYPE send_type, // whether migration type places between field and battle
                   eZONETYPE to_zone_type, DWORD to_link_server_index); // link info.

    virtual void ReadyUser(User* user, eSEND_TYPE send_type,
                           MAPCODE field_code = 0, DWORD area_id = 0,
                           WzVector* start_pos = NULL, bool is_first_join = false);
    void SendToAll(MSG_BASE* msg, WORD size, DWORD except_user_key = 0) const;
    //임시
    void DestroyUser(User* user);

    virtual BOOL Process();

    virtual eZONETYPE GetType() = 0;
    // (WAVERIX) (090520) Change 'GetMapCode()' To abstract|CHANGES=invalidate comment since 100302|
    MAPCODE GetMapCode() const;
    // (CHANGES) (WAVERIX) (090520) sMAPINFO control
    BOOLEAN SetMapCode(MAPCODE map_code);
    const sMAPINFO* GetMapInfo() const;
    virtual BOOL IsRoomType() const; // (CHANGES) (WAVERIX) (090522)

    virtual BOOL CanShowAtRoomList(User* user, eZONEPUBLIC room_public_config, TCHAR* room_password);

    RC::eROOM_RESULT CanJoin(User* user, eZONEPUBLIC room_public_config, TCHAR* room_password);

    template<class UserOperator>
    void Foreach(UserOperator& op);

    KEYTYPE GetKey() const;
    void SetKey(KEYTYPE room_key);
    void SetReusedKey(BOOL reused);
    BOOL IsReusedKey() const;
    void SetReusedServerSession(BOOL reused);
    BOOL IsReusedServerSession() const;

    // 파티관련
    virtual void LinkParty(WORD party_key);
    virtual WORD GetLinkParty();
    virtual void Invite(User* user);
    // filter routine by unexpected closing state
    BOOL FromZone_Check(ZONE_CHECK_TYPE checkType);
    // filter routine by unexpected closing state
    BOOL ToZone_Check(ZONE_CHECK_TYPE checkType);
    // enable use room contents whether player has chao state
    BOOL GetServerOptChaoStateUseZone(eZONETYPE zoneType);

    DWORD GetUserNum() const;
    User* FindUser(const DWORD player_key) const;
    User* GetFirstUser() const;
    void RemoveUser(const DWORD player_key);
    void AddUser(User* const user, const DWORD player_key);

    const ZONE_USER_LIST& GetUserList() const;

//--------------------------------------------------------------------------------------------------
//  session control
    ServerSession* GetLinkedServer() const;
    void SetLinkedServer(ServerSession* linked_session);
    DWORD GetLinkedServerIndex();
    void SendToLinkedServer(MSG_BASE_FORWARD* msg, WORD size);

//--------------------------------------------------------------------------------------------------
protected:
    virtual void On_create();
    virtual void On_destroy();
    virtual void On_readyuser(User* user);
    virtual void On_enteruser(User* user);
    virtual void On_leaveuser(User* user);
    //< user와 방에 대한 체크 : 인자 체크는 외부에서
    virtual RC::eROOM_RESULT CanCreate(User* user);
    //< user와 방에 대한 체크 : 인자 체크는 외부에서
    virtual RC::eROOM_RESULT CanJoin(User* user);
    virtual RC::eROOM_RESULT _CanJoinEach(User* user,
                                          eZONEPUBLIC room_public_config, TCHAR* room_password);
    virtual RC::eROOM_RESULT _CanJoinAll(User* user,
                                         eZONEPUBLIC room_public_config, TCHAR* room_password);

    virtual DWORD GetCurUserNum();
//--------------------------------------------------------------------------------------------------
//  <FIELDs>
private:
    KEYTYPE         m_Key;
    MAPCODE         m_MapCode;  // (CHANGES) (WAVERIX) (090520) move to this from RoomInterface
    const sMAPINFO* m_pMapInfo;
    ServerSession*  m_LinkedServerSession;
    uint8_t         __paddings_for_debug[sizeof(USER_HASH)];
    // assert __paddings_for_debug + sizeof(USER_HASH) == &m_UserList
    ZONE_USER_LIST  m_UserList;
    BOOLEAN         m_bReusedKey;
    BOOLEAN         m_bReusedServerSession;
    WORD            m_LinkPartyKey;

    friend class ZoneManager;    //for GetLinkedServer(), GetUserNum()
};

//==================================================================================================
// inlining functions
inline /*virtual*/ BOOL ZoneInterface::Process()
{
    return true;
}

inline MAPCODE ZoneInterface::GetMapCode() const
{
    return m_MapCode;
}

inline const sMAPINFO* ZoneInterface::GetMapInfo() const
{
    return m_pMapInfo;
}

inline /*virtual*/ BOOL ZoneInterface::IsRoomType() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
// related key accessors : room key & session key
inline KEYTYPE ZoneInterface::GetKey() const
{
    return m_Key;
}

inline void ZoneInterface::SetKey(KEYTYPE room_key)
{
    m_Key = room_key;
}

inline void ZoneInterface::SetReusedKey(BOOL reused)
{
    m_bReusedKey = reused ? true : false;
}

inline BOOL ZoneInterface::IsReusedKey() const
{
    return m_bReusedKey;
}

inline void ZoneInterface::SetReusedServerSession(BOOL reused)
{
    m_bReusedServerSession = reused ? true : false;
}

inline BOOL ZoneInterface::IsReusedServerSession() const
{
    return m_bReusedServerSession;
}

//--------------------------------------------------------------------------------------------------
// related party
inline /*virtual*/ void ZoneInterface::LinkParty(WORD party_key)
{
    m_LinkPartyKey = party_key;
}

inline /*virtual*/ WORD ZoneInterface::GetLinkParty()
{
    return m_LinkPartyKey;
}

inline /*virtual*/ void ZoneInterface::Invite(User* user)
{
    __UNUSED(user); // similar interface to composition
}

//--------------------------------------------------------------------------------------------------
// related session control

inline ServerSession* ZoneInterface::GetLinkedServer() const
{
    return m_LinkedServerSession;
}

inline void ZoneInterface::SetLinkedServer(ServerSession* linked_session)
{
    m_LinkedServerSession = linked_session;
}

//--------------------------------------------------------------------------------------------------
// user table control operations
inline /*virtual*/ DWORD ZoneInterface::GetCurUserNum()
{
    return static_cast<DWORD>(m_UserList.size());
}

inline DWORD ZoneInterface::GetUserNum() const
{
    return static_cast<DWORD>(m_UserList.size());
}

inline const ZoneInterface::ZONE_USER_LIST&
ZoneInterface::GetUserList() const
{
    return m_UserList;
}

inline User*
ZoneInterface::FindUser(const DWORD player_key) const
{
    ZONE_USER_LIST::const_iterator it = m_UserList.find(player_key);
    return it != m_UserList.end() ?   it->second
                                  :   NULL;
}

inline User*
ZoneInterface::GetFirstUser() const
{
    if (m_UserList.empty()) {
        return NULL;
    }
    ZONE_USER_LIST::const_iterator it = m_UserList.begin();
    return it->second;
#if SUN_CODE_BACKUP
    return m_UserList.empty() ? NULL
                              : m_UserList.begin()->second;
#endif
}

//--------------------------------------------------------------------------------------------------
// zone control with related user

inline /*virtual*/ void ZoneInterface::On_create()
{
    // similar interface to composition
}

inline /*virtual*/ void ZoneInterface::On_destroy()
{
    // similar interface to composition
}

inline /*virtual*/ void ZoneInterface::On_readyuser(User* user)
{
    __UNUSED(user); // similar interface to composition
}

//void ZoneInterface::On_enteruser(User* user);

inline /*virtual*/ void ZoneInterface::On_leaveuser(User* user)
{
    __UNUSED(user); // similar interface to composition
}

inline /*virtual*/ RC::eROOM_RESULT
ZoneInterface::CanCreate(User* user)
{
    __UNUSED(user); // similar interface to composition
    return RC::RC_ROOM_SUCCESS;
}

inline /*virtual*/ RC::eROOM_RESULT
ZoneInterface::CanJoin(User* user)
{
    __UNUSED(user); // similar interface to composition
    return RC::RC_ROOM_SUCCESS;
}

inline /*virtual*/ RC::eROOM_RESULT
ZoneInterface::_CanJoinEach(User* user, eZONEPUBLIC room_public_config, TCHAR* room_password)
{
    __UNUSED(user); // similar interface to composition
    __UNUSED((room_public_config, room_password));
    return RC::RC_ROOM_SUCCESS;
}

inline /*virtual*/ RC::eROOM_RESULT
ZoneInterface::_CanJoinAll(User* user, eZONEPUBLIC room_public_config, TCHAR* room_password)
{
    __UNUSED(user); // similar interface to composition
    __UNUSED((room_public_config, room_password));
    return RC::RC_ROOM_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// other utilities

template<class UserOperator>
void
ZoneInterface::Foreach(UserOperator& op)
{
    ZONE_USER_LIST::iterator it = m_UserList.begin(),
                             itend = m_UserList.end();
    while (it != itend)
    {
        User* const user = it->second; //for sync iterator
        ++it;
        op(user);
    }
    //FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, m_UserList, ZONE_USER_LIST)
    //{
    //    User* const user = rNode.second;
    //    op(user);
    //}
}

#endif //AGENTSERVER_ZONE_INTERFACE_H
