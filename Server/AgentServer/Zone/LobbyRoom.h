#pragma once
//=============================================================================================================================
/// LobbyRoom 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 9. 20
	@remark
		- 로비방에 대한 존 정보 관리
	@note
		- 
	@history 
		- 
	@usage
		- 
*/
//=============================================================================================================================

//==================================================================================================
//  <INCLUDEs>
//
#include "RoomInterface.h"

//==================================================================================================
// speical factility for lobby broadcasting
namespace ns_functor {
;

struct LobbyChangedStatusNotificationFilter
{
    typedef uint16_t MemberInfoArray[MAX_PARTYMEMBER_NUM];
    //
    bool Check(User* user, User* requester) const;
    template<size_t _Size>
    MemberInfoArray& GetRedirectedArray(uint16_t (&array)[_Size]) const;
};

struct PvPLobbyChangedStatusNotificationFilter
{
    typedef PVP_LOBBY_INFO MemberInfoArray[MAX_PARTYMEMBER_NUM];
    bool Check(User* user, User* requester) const;
    template<size_t _Size>
    MemberInfoArray& GetRedirectedArray(PVP_LOBBY_INFO (&array)[_Size]) const;
};

};//end of namespace

//==================================================================================================
//  <LobbyRoom>
//
class LobbyRoom : public RoomInterface
{
private:
    _DECLARE_POOL(LobbyRoom)
    friend class HuntingRoom;
    friend class MissionRoom;
    friend class PVPRoom;
    friend class CHuntingRoom;
    friend class ZoneManager;   //AssignTeam();

public:
    LobbyRoom(void);
    virtual	~LobbyRoom(void);

    RC::eROOM_RESULT    Create(User* pUser,
                               KEYTYPE key, MAPCODE MapCode, eZONETYPE eLobbyType,
                               eZONEPUBLIC ePublic, TCHAR* pszRoomTitle, TCHAR* pszRoomPWD,
                               BASE_ROOMINFO& IN BaseRoomInfo,
                               ServerSession* pServerSession, KEYTYPE LinkHuntingKey);

    inline eZONETYPE    GetLobbyType() { return m_LobbyType; }
    inline virtual eZONETYPE GetType() { return eZONETYPE_LOBBY; }
    virtual VOID    Invite(User* IN pUser);

    VOID    SerializeLoad(HUNTING_ROOMINFO& OUT rRoomInfo);
    VOID    SerializeLoad(MISSION_ROOMINFO& OUT rRoomInfo);
    VOID    SerializeLoad(PVP_ROOMINFO& OUT rRoomInfo);
    VOID    SerializeLoad(CHUNTING_ROOMINFO& OUT rRoomInfo);
    BOOL    IsAllReady();
    BOOL    IsPrepareUserNum();

    VOID    ExpelAll();    // 유저 모두 추방
    BOOL    CheckAbusing(CHuntingRoom* IN pCHuntingRoom);
    VOID    SetWaitTimer(const DWORD dwWaitTime) { m_WaitTimer.SetTimer(dwWaitTime); }

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    RC::eROOM_RESULT CanMigrateToChunting();
#endif
    BOOL    ChangeRoomTitle(const TCHAR* pszRoomTitle);
    User*   Get2ndTeamMaster();

    // 추가적으로 로비안에서 변경할 수 있는 파라미터
    RC::eROOM_RESULT ChangeMap(MAPCODE MapCode);

    void SetAdditionalRoomInfo(ADDITIONAL_ROOMINFO* pInfo) { ASSERT(pInfo); m_AdditionalRoomInfo = *pInfo;    }
    void SetAdditionalPVPInfo(ADDITIONAL_PVPINFO* pInfo) { ASSERT(pInfo); m_AdditionalPVPInfo = *pInfo;    }
    void SetAdditionalCHuntingInfo(ADDITIONAL_CHUNTINGINFO* pInfo) { ASSERT(pInfo); memcpy(&m_AdditionalCHuntingExInfo, pInfo, sizeof(ADDITIONAL_CHUNTINGINFO));    }
    void SetLinkHuntingKey(KEYTYPE LinkHuntingKey) { m_AdditionalCHuntingExInfo.m_LinkHuntingKey = LinkHuntingKey; }
    const ADDITIONAL_ROOMINFO* GetAdditionalInfo() const { return &m_AdditionalRoomInfo; }
    const ADDITIONAL_PVPINFO* GetAdditionalPVPInfo() const { return &m_AdditionalPVPInfo; }
    const ADDITIONAL_CHUNTINGINFO_EX* GetAdditionalCHuntingInfo() const { return &m_AdditionalCHuntingExInfo; }
    bool IsPVPTeamMode() const { return (m_AdditionalPVPInfo.m_Mode == ePVP_TEAM_MODE); }
    virtual BOOL CanShowAtRoomList(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);
    // CHANGES: f110608.6L, this method added to reply against CG_ZONE_LOBBY_REQUEST_INFO_SYN
    bool SendLobbyInfoForRequest(User* requester) const;
    // CHANGES: f110609.3L, it added to make shared part data.
    // returns: number of aligned information
    uint8_t GetListMemberInfo(
        ns_functor::LobbyChangedStatusNotificationFilter::MemberInfoArray&,
        User* requester) const;
    uint8_t GetListMemberInfo(
        ns_functor::PvPLobbyChangedStatusNotificationFilter::MemberInfoArray& result_array,
        User* requester) const;
protected:
    VOID    AssignTeam(User* pTargetUser = NULL);

    inline  VOID SetLobbyType(eZONETYPE type) { m_LobbyType = type; }
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_readyuser(User* pUser);
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);
    virtual BOOL RecommendLevel(LEVELTYPE LV);

    RC::eROOM_RESULT CanCreate(User* pUser, MAPCODE MapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic, BASE_ROOMINFO& IN BaseRoomInfo, KEYTYPE LinkHuntingKey);
    virtual RC::eROOM_RESULT _CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);
public:

private:
    eZONETYPE   m_LobbyType;
    ADDITIONAL_ROOMINFO         m_AdditionalRoomInfo;
    ADDITIONAL_PVPINFO          m_AdditionalPVPInfo;
    ADDITIONAL_CHUNTINGINFO_EX  m_AdditionalCHuntingExInfo;
    util::Timer m_WaitTimer;    // 미션릴레이로 로비를 만들면 일정시간동안 파티원이 들어올 수 있도록 기다려준다.
};


//==================================================================================================
//==================================================================================================
// speical factility for lobby broadcasting

inline bool ns_functor::LobbyChangedStatusNotificationFilter::Check(
    User* user, User* requester) const
{
    if (requester && requester == user) {
        return false;
    };
    if (user->IsMaster() || user->IsReady() == false) {
        return false;
    };
    return true;
}

template<>
inline ns_functor::LobbyChangedStatusNotificationFilter::MemberInfoArray&
ns_functor::LobbyChangedStatusNotificationFilter::GetRedirectedArray<MAX_PARTYMEMBER_NUM>(
    uint16_t (&array)[MAX_PARTYMEMBER_NUM]) const
{
    MemberInfoArray* convert = &array;
    return *convert;
}

inline bool ns_functor::PvPLobbyChangedStatusNotificationFilter::Check(
    User* user, User* requester) const
{
    if (requester && requester == user) {
        return false;
    };
    return true;
}

template<>
inline ns_functor::PvPLobbyChangedStatusNotificationFilter::MemberInfoArray&
ns_functor::PvPLobbyChangedStatusNotificationFilter::GetRedirectedArray<MAX_PARTYMEMBER_NUM>(
    PVP_LOBBY_INFO (&array)[MAX_PARTYMEMBER_NUM]) const
{
    MemberInfoArray* convert = &array;
    return *convert;
}

//==================================================================================================
//==================================================================================================
