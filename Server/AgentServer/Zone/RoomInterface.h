#pragma once
//==================================================================================================
/// RoomInterface 클래스
/**
    @author    Kim Min Wook < taiyo@webzen.com >
    @since    2005. 9. 20
    @remark
        - 
    @note
        - 
    @history 
        - 
    @usage
        - 
*/
//==================================================================================================

#include <Timer.h>
#include "ZoneInterface.h"


enum eROOMSTATE_TYPE
{
    eROOMSTATE_NONE,        //< 아무것도 아닌 상태
    eROOMSTATE_READY,       //< 준비상태
    eROOMSTATE_GENERNAL,    //< 일반적인상태
    eROOMSTATE_DESTROY,     //< 파괴된 방
};

class Party;
struct MSG_AG_ZONE_CAN_CREATE_ROOM_ACK;

typedef util::SolarHashTable<Party *>::iterator        PARTY_ITERATOR;

//==================================================================================================
//

class RoomInterface : public ZoneInterface
{
    friend class ZoneManager; //< CanJoin()
public:
    enum {
        MAX_DESTROY_TIMEOUT_TIME    = 10 * 60 * 1000,
        MAX_RELAY_TIMEOUT_TIME      = 10 * 60 * 1000,
    };

    RoomInterface(void);
    virtual ~RoomInterface(void);

    inline User*    GetMaster() { return m_pMasterUser;    }
    inline eZONEPUBLIC GetZonePublic() const { return (eZONEPUBLIC)m_byRoomPublic; }
    inline TCHAR*   GetRoomTitle() { return m_pszRoomTitle; }
    inline TCHAR*   GetRoomPWD() { return m_pszRoomPWD; }
    inline eROOMSTATE_TYPE GetRoomState() const { return m_RoomState; }
    inline void     SetRoomState(eROOMSTATE_TYPE sts) { m_RoomState = sts; }
    virtual BOOL    IsRoomType() const { return TRUE; }

    inline void     SetLimitMaxUserNum(BYTE num) { m_MaxLimitUserNum = num; }
    inline BYTE     GetLimitMaxUserNum() const { return m_MaxLimitUserNum; }

    virtual BOOL    RecommendLevel(LEVELTYPE level) { __UNUSED(level); return FALSE; }
    //virtual MAPCODE GetMapCode() { return m_MapCode; }

    RC::eROOM_RESULT IsRecommendRoom(User* pUser);
    void    VestUserWithMaster(User* pExpectedMaster);
    User*   GetNextPriorityMaster(RoomInterface* pRoom, DWORD curMasterKey,
                                  eROOM_SPECIAL_TYPE* OUT pRoomSpecialTypeByUser);
    User*   ChangeSpecialType(BOOLEAN isEnterEvent, User* pMaster);
    // 평균 레벨, 승률
    LEVELTYPE GetAvgLevel();
    void    GetCHuntingScore(BYTE& byAvgWinPercent, DWORD& dwTotCount);

protected:
    void    Create(User* pUser,
                   KEYTYPE key, MAPCODE MapCode,
                   eZONEPUBLIC ePublic, TCHAR* pszRoomTitle, TCHAR* pszRoomPWD,
                   BASE_ROOMINFO& IN baseRoomInfo,
                   ServerSession* pServerSession);

    RC::eROOM_RESULT CanCreate(User* pUser,
                               MAPCODE MapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                               BASE_ROOMINFO& IN baseRoomInfo, BOOL CheckOpenTime = TRUE);
    //{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    // Setting by ZoneManager
    void    ChangePolicy(PolicymentForRoom::POLICY_T newPolicySet);
    virtual RC::eROOM_RESULT MigratePortalUserFirstEnter(KEYTYPE key,
                                                const RoomArgumentForRoomSetup& rRoomArgument,
                                                User* pUser, ServerSession* pServerSession);
    //}
public:
    virtual RC::eROOM_RESULT _CanJoinAll(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);

#if defined(__NA_000000_20070528_TAIWAN_GUILD_PVP) && defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)
    // 대만 길드전 체크는 헌팅룸에만 속하지 않을수 있고, 룸의 타입체크는 룸정보 구조체를 가진 this클래스에서 체크
    RC::eROOM_RESULT CanJoinGuildPVP(User* pUser);
    BOOL IsSpecialType(eROOM_SPECIAL_TYPE SpecialType) { return m_RoomInfo.IsSpecialType(SpecialType); }
#endif//__NA_000000_20070528_TAIWAN_GUILD_PVP

    BOOL    ValidCharClass(eCHAR_TYPE type);
    BOOL    ValidLevel(LEVELTYPE LV);
    BOOL    ValidJoinMap(User* pUser);
    virtual BOOL CanShowAtRoomList(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);
    virtual RC::eROOM_RESULT Migrate(KEYTYPE key, RoomInterface* pPreviousRoom,
                                     ServerSession* pServerSession);
    void    RelayUser(RoomInterface* pPreviousRoom);

    virtual BOOL Process();
    virtual void On_create();
    virtual void On_destroy();
    virtual void On_readyuser(User* pUser);
    virtual void On_enteruser(User* pUser);
    virtual void On_leaveuser(User* pUser);

    //void  SetMapCode(MAPCODE code);
    inline void  SetZonePublic(eZONEPUBLIC pub);
    inline void  SetRoomPWD(TCHAR* pw);
    inline void  SetRoomInfo(const BASE_ROOMINFO* pRoomInfo);
    inline void  SetMaster(User* pUser);
    inline void  SetRoomTitle(const TCHAR* title);

    inline BASE_ROOMINFO* GetRoomInfo() { return &m_RoomInfo; }
    // (WAVERIX) (NOTE) 하기 interface는 자료구조 변경 이후 수정하기로 한다.
    inline virtual DWORD GetCurUserNum() { return ZoneInterface::GetUserNum() + m_RelayUserHash.GetDataNum(); }
    inline util::SolarHashTable<User*>& GetRelayHash()  { return m_RelayUserHash; }
    inline User* FindRelayUser(DWORD PlayerKey)         { return m_RelayUserHash.GetData(PlayerKey); }
    inline void  RemoveRelayUser(DWORD playerKey)       { m_RelayUserHash.Remove(playerKey); }
    inline void  AddRelayUser(User* pUser, DWORD playerKey) { m_RelayUserHash.Add(pUser, playerKey); }
    //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    inline BYTE  GetNumberOfExpectedPlayers() const { return m_NumberOfExpectedPlayers; }
    inline void  SetNumberOfExpectedPlayers(BYTE number_of_players);
    bool ExceptionProcessByLobbyMember(const MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* check_msg, User* request_user);

    PolicymentForRoom RoomPolicyment; // Property
private:
    virtual BOOL CheckLimitUser(User* pUser);

    //////////////////////////////////////////////////////////////////////////
    // Fields
    eROOMSTATE_TYPE    m_RoomState;
    util::Timer        m_DestroyTimeout;
    util::Timer        m_MasterEnterTimeout;
    util::SolarHashTable<User*>    m_RelayUserHash;

    // 방정보
    User*   m_pMasterUser;
    BYTE    m_NumberOfExpectedPlayers; //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    BYTE    m_byRoomPublic;                             //< eZONEPUBLIC
    TCHAR   m_pszRoomTitle[MAX_ROOMTITLE_LENGTH + 1];   //< 안전한 문자열 처리를 위해 +1
    TCHAR   m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH + 1];  //< 안전한 문자열 처리를 위해 +1
    BYTE    m_MaxLimitUserNum;
    BASE_ROOMINFO m_RoomInfo;                           //< 생성할 방 정보
};

//
//==================================================================================================
//

inline void
RoomInterface::SetNumberOfExpectedPlayers(BYTE number_of_players) {
    //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    m_NumberOfExpectedPlayers = number_of_players;
}

inline void
RoomInterface::SetZonePublic(eZONEPUBLIC pub)
{
    m_byRoomPublic = static_cast<uint8_t>(pub);
}

inline void
RoomInterface::SetRoomPWD(TCHAR* pw)
{
    if(pw == 0)
        return;

    _tcsncpy(m_pszRoomPWD, pw, _countof(m_pszRoomPWD));
    m_pszRoomPWD[_countof(m_pszRoomPWD) - 1] = '\0';
}

inline void
RoomInterface::SetRoomInfo(const BASE_ROOMINFO* pRoomInfo)
{
    m_RoomInfo = *pRoomInfo;
}

inline void
RoomInterface::SetMaster(User* pUser)
{
    m_pMasterUser = pUser;
}

inline void 
RoomInterface::SetRoomTitle(const TCHAR* title)
{
    _tcsncpy(m_pszRoomTitle, title, _countof(m_pszRoomTitle));
    m_pszRoomTitle[_countof(m_pszRoomTitle) - 1] = '\0';
}

//==================================================================================================
//

template <class SRC_ROOM, class TARGET_ROOM, int Transaction>
class RoomMigrater
{
    SRC_ROOM*       m_SrcRoom;
    TARGET_ROOM*    m_pTargetRoom;
public:
    RoomMigrater(SRC_ROOM* pSrc, TARGET_ROOM* pTarget) : m_SrcRoom(pSrc), m_pTargetRoom(pTarget) {}
    ~RoomMigrater() {}

    void operator()(User* pUser)
    {
        pUser->BeginTransaction((eBEHAVE_TRANSACTION)Transaction);

        m_SrcRoom->LeaveUser(pUser, eSEND_SYN, m_pTargetRoom->GetType(), m_pTargetRoom->GetLinkedServerIndex());

        // 헌팅키를 저장해둠!!!!
        pUser->SetTRRoomKey(m_pTargetRoom->GetKey());
        DISPMSG("[U:%s]room(%u)_Leave_Syn\n", pUser->GetUserID(), m_pTargetRoom->GetKey());
    }


};
//_NA_20110630_BATTLE_ZONE_RENEWAL
template <class TARGET_ROOM, int Transaction>
class RoomMigraterByParty
{
    TARGET_ROOM* target_room_;

public:
    RoomMigraterByParty(TARGET_ROOM* target_room) : target_room_(target_room){}
    ~RoomMigraterByParty(){}

    void operator()(_PARTY_MEMBER_INFO* member_info)
    {
        User* user = UserManager::Instance()->GetUserByObjKey(member_info->m_dwMemberKey);
        if (user == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", member_info->m_dwMemberKey);
            return;
        }
        WORD party_key = user->GetPartyState().GetPartyKey();
        DWORD master_key = AgentPartyManager::Instance()->GetMasterKey(party_key);

        if (user->GetPlayerKey() == master_key)
        {
            return;
        }

        ZoneInterface* current_zone_interface = g_ZoneManager.FindZone(user->GetZoneKey());
        if (current_zone_interface == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Not found zone : %d", user->GetUserGUID());
            return;
        }
        user->BeginTransaction((eBEHAVE_TRANSACTION)Transaction);
        current_zone_interface->LeaveUser(user, eSEND_SYN, target_room_->GetType(), 
                                          target_room_->GetLinkedServerIndex());

        user->SetTRRoomKey(target_room_->GetKey());
    }
};

//==================================================================================================
//
struct ChangeRoomName : public std::unary_function<User*, void>
{
    MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_ACK m_SendData;

    explicit ChangeRoomName(const char* pszRoomName)
    {
        BOOST_STATIC_ASSERT(_countof(m_SendData.m_pszRoomTitle) == MAX_ROOMTITLE_LENGTH);
        strncpy(m_SendData.m_pszRoomTitle, pszRoomName, _countof(m_SendData.m_pszRoomTitle));
        m_SendData.m_pszRoomTitle[_countof(m_SendData.m_pszRoomTitle) - 1] = '\0';
    }

    inline void operator()(User* pUser)
    {
        pUser->SendPacket(&m_SendData, sizeof(m_SendData));
    }
};





