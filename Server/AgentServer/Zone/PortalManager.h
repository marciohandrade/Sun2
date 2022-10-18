#ifndef __AGENTSERVER_PORTAL_MANAGER_H__
#define __AGENTSERVER_PORTAL_MANAGER_H__

//==================================================================================================
/// PortalManager Å¬·¡½º
/**
    @author	waverix < waverix@webzen.co.kr >
    @since	2009-02-10
    @remark
    - 
    @note
    - 
    @history 
    - 09/02/10 - Created
    @usage
    - 
*/
//==================================================================================================

#include <Zone/ZoneCommon.h>

// declared by __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

struct QuestPortalInfoInAgent;
class User;
class ZoneInterface;
class RoomInterface;
class AgentPartyManager;
struct MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK;

class PortalManager : public util::Singleton<PortalManager>
{
    typedef STLX_HASH_MAP<PORTALKEY, QuestPortalInfoInAgent*>   PortalTable;
public:
    PortalManager();
    ~PortalManager();

    static BOOLEAN IsValidRequesterForCreatePortal(User* pUser);
    static BOOLEAN IsValidCreatingZone(eZONETYPE zoneType);

    // by AG_ZONE_QUEST_PORTAL_REQUEST_SYN & ZoneManager
    static PORTALKEY CreatePortal(const QuestPortalInfo& rQuestPortalInfo,
                                  BOOLEAN isTempPortal,
                                  const RoomArgumentForRoomSetup& rRoomArgument);
    // 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN
    static RC::eROOM_RESULT RequestEnterZoneTempPortal(User* pUser, PORTALKEY portalKey);
    // 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN
    static RC::eROOM_RESULT RequestEnterZoneFixedPortal(User* pUser, WORD enterancePortalIndex);
    // 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN (first user)
    // 2) EnterUser Event in ZoneInterface (other users)
    static RC::eROOM_RESULT EnterZoneTempPortal(User* pUser, PORTALKEY portalKey);
    // 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN (first user)
    // 2) EnterUser Event in ZoneInterface (other users)
    static RC::eROOM_RESULT EnterZoneFixedPortal(User* pUser, PORTALKEY portalKey);

    static BOOLEAN RequestClosePortal(User* pUser);
#ifdef _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
    static bool RequestWithdrawPortalMember(User* user, RC::eROOM_RESULT* const error_code);
#endif
    static BOOLEAN OnPortalEnterResult(const MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK* pMsg, User* pUser);
    static BOOLEAN JoinRoom(User* pUser, MSG_CG_ZONE_EX_ROOM_JOIN_ACK* OUT pJoinInfo);
    static VOID OnEnteredRoom(User* pUser, RoomInterface* pRoom); // post event support
    static VOID OnRoomEvent(ZoneInterface* pZone, BOOLEAN isCreate);
    // 
    static BOOLEAN CanJoinParty(User* pRequester, User* pResponser);
    // LeaveParty Event in PartyManager
    static VOID OnLeaveParty(User* pUser);
    // Process Event in RoomInterface
    static VOID RoomLifetimeTimeout(RoomInterface* pRoomInterface);
    // Disconnect Event in ~~
    static VOID ClearUserInfo(User* pUser);
    // AgentServer Update Process
    static VOID Update();

private:
    //static VOID OnChangePortal(User* pUser, PortalKey portalKey);
    inline QuestPortalInfoInAgent* FindQuestPortalInfo(PORTALKEY portalKey);
    VOID PortalClose(QuestPortalInfoInAgent* pQuestPortalInfo);
    BOOLEAN IsAcceptableUserByPointedPortal(QuestPortalInfoInAgent* pQuestPortalInfo, User* pUser,
                                            BOOLEAN isTempPortal);

    PortalTable m_PortalTable;
    UserManager* const m_pUserManager;
    AgentPartyManager* const m_pPartyManager;
    __DISABLE_COPY(PortalManager);
};

#endif //__AGENTSERVER_PORTAL_MANAGER_H__
