#include "stdafx.h"
#include "PortalManager.h"
#include <PacketStruct_AG.h>
#include <MapInfoParser.h>

#include <Zone/LobbyRoom.h>

// declared by __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>
#include <AgentParty/AgentPartyOperator.h>

struct QuestPortalInfoInAgent : public QuestPortalInfo
{
    enum ePortalState {
        ePortalState_Init,
        ePortalState_RoomCreated,
        ePortalState_Closed,
    };
    typedef STLX_VECTOR<DWORD /*ObjectKey*/> UserList;

    QuestPortalInfoInAgent()
        : m_PortalState(ePortalState_Init)
        , m_IsTempPortal(false)
        , m_PartyKey(0)
        , m_MasterPlayerKey(0)
        , m_RoomKey(0)
    {   // no virtual function
        ZeroMemory(this, sizeof(*(QuestPortalInfo*)this));
    }
    ~QuestPortalInfoInAgent()
    {
        //g_ZoneManager.ClosePortal(m_pLobby, m_RoomKey);
    }
    void CreatePortalInfo(const QuestPortalInfo& rQuestPortalInfo,
                          BOOLEAN isTempPortal,
                          PORTALKEY portalKey,
                          WORD partyKey,
                          DWORD masterPlayerKey,
                          const RoomArgumentForRoomSetup& rRoomArgument)
    {
        *(QuestPortalInfo*)this = rQuestPortalInfo;
        m_IsTempPortal = isTempPortal;
        this->portalKey = portalKey;
        m_PortalState = ePortalState_Init;
        m_PartyKey = partyKey;
        m_MasterPlayerKey = masterPlayerKey;
        // 유쾌하진 못하다...
        CopyMemory(&m_RoomArgument, &rRoomArgument, sizeof(m_RoomArgument));
        m_LifeTimeout.SetTimer(portalLifetime, true, false);
    }

    BOOLEAN IsPortalQuestMember(DWORD playerKey) const
    {
        UserList::const_iterator it = std::find(m_UserList.begin(), m_UserList.end(), playerKey);
        return (it != m_UserList.end());
    }

    void RemoveQuestPortalMember(DWORD playerKey)
    {
        UserList::iterator it = std::find(m_UserList.begin(), m_UserList.end(), playerKey);
        if(it != m_UserList.end())
            m_UserList.erase(it);
    }

    inline BOOLEAN IsFirstEnterance() const
    {
        assert(m_RoomKey    ? m_PortalState == ePortalState_RoomCreated
                            : m_PortalState != ePortalState_Init);
        return m_PortalState == ePortalState_Init;
    }

    ePortalState    m_PortalState;
    BOOLEAN         m_IsTempPortal;
    WORD            m_PartyKey;
    DWORD           m_MasterPlayerKey; // party가 없어도 설정되는 값.
    KEYTYPE         m_RoomKey;
    ITimerBase      m_LifeTimeout;
    UserList        m_UserList;
    RoomArgumentForRoomSetup m_RoomArgument;
};

static MemberQueryResultOperator partyPortalOperator;
typedef MemberQueryResultOperator::UserState       UserState;
typedef MemberQueryResultOperator::UserStateList   UserStateList;

PortalManager::PortalManager()
    : m_pUserManager(UserManager::Instance())
    , m_pPartyManager(AgentPartyManager::Instance())
{
}

PortalManager::~PortalManager()
{
}

BOOLEAN
PortalManager::IsValidRequesterForCreatePortal(User* pUser)
{
    KEYTYPE zoneKey = pUser->GetZoneKey();
    ZoneInterface* pRequestedZone = g_ZoneManager.FindZone(zoneKey);
    if(pRequestedZone == 0)
        return FALSE;

    eZONE_STATE zoneState = pUser->GetZoneState();
    if((zoneState & ePRS_AT_ZONE) == FALSE)
        return FALSE;

    eZONETYPE zoneType = pRequestedZone->GetType();
    if (zoneType != eZONETYPE_VILLAGE &&
        zoneType != eZONETYPE_DOMINATION_FIELD &&
        zoneType != eZONETYPE_FIELD &&
        zoneType != eZONETYPE_SPA_FIELD) //_NA_008069_20150210_ADD_SPA_SYSTEM
        return FALSE;

    return TRUE;
}

BOOLEAN
PortalManager::IsValidCreatingZone(eZONETYPE zoneType)
{
    if(zoneType == eZONETYPE_MISSION ||
       zoneType == eZONETYPE_INSTANCE)
        return TRUE;

    return FALSE;
}

inline QuestPortalInfoInAgent*
PortalManager::FindQuestPortalInfo(PORTALKEY portalKey)
{
    PortalTable::iterator it = m_PortalTable.find(portalKey);
    return (it != m_PortalTable.end())
            ?   it->second
            :   0;
}

PORTALKEY
PortalManager::CreatePortal(const QuestPortalInfo& rQuestPortalInfo,
                            BOOLEAN isTempPortal,
                            const RoomArgumentForRoomSetup& rRoomArgument)
{
    PortalManager* const this_ = PortalManager::Instance();
    User* pUser = this_->m_pUserManager->GetUserByObjKey(rQuestPortalInfo.requesterKey);
    if(pUser == 0)
        return INVALID_PORTAL_KEY;

    const PORTALKEY portalKey = (PORTALKEY)ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_PORTALKEY);

    PartyState& partyState = pUser->GetPartyState();
    WORD partyKey = partyState.GetPartyKey();
    KEYTYPE zoneKey = pUser->GetZoneKey();

    partyPortalOperator.Init(ePARTY_OPERATOR_PORTAL_CREATE, 0, zoneKey);
    partyPortalOperator.SetQuestPortalInfo(isTempPortal, portalKey, &rQuestPortalInfo);
    if(partyKey)
        this_->m_pPartyManager->For_eachMembers(partyKey, partyPortalOperator);
    else
        partyPortalOperator.PortalCreate(pUser);

    const PortalUserInfo& stateEnum = pUser->GetPortalUserInfo();

    DWORD masterPlayerKey = partyKey ? this_->m_pPartyManager->GetMasterKey(partyKey)
                                     : pUser->GetPlayerKey();

    QuestPortalInfoInAgent* pPortalInfo = TAllocNew(QuestPortalInfoInAgent);
    pPortalInfo->CreatePortalInfo(rQuestPortalInfo, isTempPortal,
                                  portalKey, partyKey, masterPlayerKey, rRoomArgument);
    QuestPortalInfoInAgent::UserList& rUserList = pPortalInfo->m_UserList;
    rUserList.reserve(MAX_PARTYMEMBER_NUM);

    DWORD successors = 0;
    const UserStateList& stateList = partyPortalOperator.GetUserStateList();
    FOREACH_CONTAINER(const UserState& state, stateList, UserStateList)
    {
        const DWORD playerKey = state.pUser->GetPlayerKey();
        switch(state.state)
        {
        case stateEnum.eState_Success:
            ++successors;
            rUserList.push_back(playerKey);
            break;
        case stateEnum.eState_ChangedPortal:
            ++successors;
            rUserList.push_back(playerKey);
            if(state.prevPortalKey != portalKey)
            {   // old info remove process
                QuestPortalInfoInAgent* pOldInfo = this_->FindQuestPortalInfo(state.prevPortalKey);
                if(pOldInfo)
                    pOldInfo->RemoveQuestPortalMember(playerKey);
            }
            break;
        case stateEnum.eState_Transaction:
        case stateEnum.eState_NoRelevant:
        case stateEnum.eState_Fail: //실패 사례들이 뭔지 고민할 것.
        //case stateEnum.eState_DuplicatedPortal:
        //case stateEnum.eState_NotFoundPortal:
        //case stateEnum.eState_DisabledAction:
        default:
            break;
        }
    }

    // 제거는 Update를 통해서...
    this_->m_PortalTable[portalKey] = pPortalInfo;

    return portalKey;
}


// 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN
RC::eROOM_RESULT
PortalManager::RequestEnterZoneTempPortal(User* pUser, PORTALKEY portalKey)
{
    if(pUser->IsBeginTransaction())
        return RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

    PortalManager* const this_ = PortalManager::Instance();
    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(pQuestPortalInfo == 0)
        return RC::RC_ROOM_FAILED;  // can't find portal
    PortalUserInfo& rPortalUserInfo = pUser->GetPortalUserInfo();
    if(rPortalUserInfo.GetPortalKey() != portalKey)
        return RC::RC_ROOM_FAILED;  // can't match portal

    pUser->BeginTransaction(TR_QUERY_PORTAL_UNTIL_GAMESERVER_ACK);
    MSG_AG_ZONE_QUEST_PORTAL_ENTER_SYN msg;
    //
    msg.syncType = msg.eSYNC_TEMP_PORTAL;
    msg.portalKey = portalKey;
    msg.mapEnterancePortalIndex = 0;
    msg.enterableMapCode = pQuestPortalInfo->enterableMapCode;
    msg.questCode = pQuestPortalInfo->questCode;
    msg.portalPosision = pQuestPortalInfo->portalPosition;

    pUser->SendToLinkedServer(&msg, msg.GetSize());

    return RC::RC_ROOM_SUCCESS;
}

//BOOLEAN
//PortalManager::IsAcceptableUserByPointedPortal(QuestPortalInfoInAgent* pQuestPortalInfo,
//                                               User* pUser, BOOLEAN isTempPortal)
//{
//    PortalManager* const this_ = PortalManager::Instance();
//    BOOLEAN validPortalUser = this_->IsValidRequesterForCreatePortal(pUser);
//    validPortalUser = validPortalUser &&
//                      pQuestPortalInfo->enterableMapCode == pMapPortal->wWorldTo;
//    validPortalUser = validPortalUser &&
//                      pQuestPortalInfo->IsPortalQuestMember(pUser->GetPlayerKey());
//    if(isTempPortal == FALSE)
//    {
//        WORD partyKey = pUser->GetPartyState().GetPartyKey();
//        validPortalUser = validPortalUser &&
//                          partyKey && partyKey == pQuestPortalInfo->m_PartyKey;
//    }
//
//    return validPortalUser;
//}

// 1) CreateRoom Event by MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN
RC::eROOM_RESULT
PortalManager::RequestEnterZoneFixedPortal(User* pUser, WORD enterancePortalIndex)
{
    if(pUser->IsBeginTransaction())
        return RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

    // 1) Check PortalIndex
    // 2) CG_QUEST_PORTAL_ENTER_SYN
    // 2-1) Check User Portal State
    // 2-2) Send AG_QUEST_PORTAL_ENTER_SYN
    // 3) AG_QUEST_PORTAL_ENTER_ACK + 임시 포탈시 처리되는 내용들을 여기에 설정
    // 3-1) INIT: Create Room

    const sMAP_ENTERENCE_PORTAL* pMapPortal =
        MapInfoParser::Instance()->GetMapPortal(enterancePortalIndex);
    
    if (pMapPortal == NULL || 
        pMapPortal->byMapType == 0)
        return RC::RC_ROOM_INVALID_ROOM_TYPE;

    PortalManager* const this_ = PortalManager::Instance();

    QuestPortalInfoInAgent* pQuestPortalInfo = 0;
    DWORD playerKey = pUser->GetPlayerKey();
    BOOLEAN newPortalCreate = TRUE;

    PortalUserInfo& rPortalUserInfo = pUser->GetPortalUserInfo();
    PORTALKEY portalKey = rPortalUserInfo.GetPortalKey();
    if(portalKey != INVALID_PORTAL_KEY)
    {
        pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
        if(!FlowControl::FCAssert(pQuestPortalInfo))
            return RC::RC_ROOM_FAILED;  // can't find portal... invalid user state
        BOOLEAN validPortalUser = this_->IsValidRequesterForCreatePortal(pUser);
        validPortalUser = pQuestPortalInfo->enterableMapCode == pMapPortal->wWorldTo &&
                          pQuestPortalInfo->IsPortalQuestMember(playerKey);

        if(validPortalUser)
        {
            WORD partyKey = pUser->GetPartyState().GetPartyKey();
            if(partyKey == 0) // 1인 남은 상황
                validPortalUser = TRUE;
            else
                validPortalUser = partyKey == pQuestPortalInfo->m_PartyKey;
        }
        if(validPortalUser)
            validPortalUser = pQuestPortalInfo->m_PortalState !=
                              pQuestPortalInfo->ePortalState_Closed;

        //////////////////////////////////////////////////////////////////////////
        newPortalCreate = validPortalUser == FALSE;
    }

    if(newPortalCreate)
    {
        QuestPortalInfo portalInfo;
        portalInfo.portalKey = INVALID_PORTAL_KEY;
        portalInfo.enterableMapCode = pMapPortal->wWorldTo;
        portalInfo.requesterKey = (WORD)playerKey;
        portalInfo.questCode = 0;
        portalInfo.portalLifetime = 10 * 60 * 1000;
        SetVector(&portalInfo.portalPosition, 0.0f, 0.0f, 0.0f);

        RC::eROOM_RESULT result = g_ZoneManager.CreatePortal(portalInfo, pUser, false);
        if(result != RC::RC_ROOM_SUCCESS)
            return result;
        portalKey = rPortalUserInfo.GetPortalKey();
        pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
        if(!FlowControl::FCAssert(pQuestPortalInfo))
            return RC::RC_ROOM_FAILED;  // can't find portal... invalid user state
    }    

    pUser->BeginTransaction(TR_QUERY_PORTAL_UNTIL_GAMESERVER_ACK);
    MSG_AG_ZONE_QUEST_PORTAL_ENTER_SYN msg;
    //
    msg.syncType = msg.eSYNC_FIXED_PORTAL;
    msg.portalKey = portalKey;
    msg.mapEnterancePortalIndex = enterancePortalIndex;
    msg.enterableMapCode = pQuestPortalInfo->enterableMapCode;
    msg.questCode = pQuestPortalInfo->questCode;
    msg.portalPosision = pQuestPortalInfo->portalPosition;

    pUser->SendToLinkedServer(&msg, msg.GetSize());

    return RC::RC_ROOM_SUCCESS;
}

// EnterUser Event in ZoneInterface
RC::eROOM_RESULT
PortalManager::EnterZoneTempPortal(User* pUser, PORTALKEY portalKey)
{
    if(!FlowControl::FCAssert(pUser->IsBeginTransaction() == FALSE))
        return RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

    PortalManager* const this_ = PortalManager::Instance();
    PortalUserInfo& rPortalUserInfo = pUser->GetPortalUserInfo();
    // 그 사이에 바뀔 수 있다. 요청 자체는 동시에 할 경우, 나중에 요청한 유저의 포탈키로
    //전환된 상황... 이 경우, 처음 요청한 유저의 경우만 문제가 야기된다. 전환!
    //if(rPortalUserInfo.GetPortalKey() != portalKey)
    //    return RC::RC_ROOM_FAILED;
    PORTALKEY userPortalKey = rPortalUserInfo.GetPortalKey();
    if(userPortalKey != portalKey)
    {
        SUNLOG(eCRITICAL_LOG, "TempLog, 1st User Changed Portal by 2nd User { %u != %u }",
               userPortalKey, portalKey);
        portalKey = userPortalKey;
    }

    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(pQuestPortalInfo == 0)
        return RC::RC_ROOM_FAILED;

    const DWORD playerKey = pUser->GetPlayerKey();
    if(pQuestPortalInfo->IsPortalQuestMember(playerKey) == FALSE)
    {
        // Problem? close portal & clear info.
        return RC::RC_ROOM_FAILED;
    }

    BOOLEAN isCreated = pQuestPortalInfo->ePortalState_RoomCreated ==
                        pQuestPortalInfo->m_PortalState;

    RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
    // first step
    switch(pQuestPortalInfo->m_PortalState)
    {
    case pQuestPortalInfo->ePortalState_Init:
        result = g_ZoneManager.CreateRoomByPortal(pQuestPortalInfo->portalKey,
                                                  pUser, pQuestPortalInfo->m_RoomArgument);
        return result;
    case pQuestPortalInfo->ePortalState_RoomCreated:
        result = g_ZoneManager.EnterPortal(portalKey, pUser,
                                           pQuestPortalInfo->m_RoomArgument,
                                           pQuestPortalInfo->m_RoomKey);
        return result;
    case pQuestPortalInfo->ePortalState_Closed:
        // need process error result
        return RC::RC_ROOM_FAILED;
    }

    return result;
}

RC::eROOM_RESULT
PortalManager::EnterZoneFixedPortal(User* pUser, PORTALKEY portalKey)
{
    return EnterZoneTempPortal(pUser, portalKey);
}

//==================================================================================================
namespace ns_functor {
;

struct OnPortalEnterResultErrorHandling
{
    OnPortalEnterResultErrorHandling(MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK* msg_nak, User* user);
    ~OnPortalEnterResultErrorHandling();
    //
    MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK* msg_nak_;
    User* user_;
};

}; //end of namespace

inline ns_functor::OnPortalEnterResultErrorHandling::OnPortalEnterResultErrorHandling(
    MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK* msg_nak, User* user)
    : msg_nak_(msg_nak), user_(user)
{
}

ns_functor::OnPortalEnterResultErrorHandling::~OnPortalEnterResultErrorHandling()
{
    if (msg_nak_->errorCode == RC::RC_ROOM_SUCCESS) {
        return;
    };
    user_->SendPacket(msg_nak_, sizeof(*msg_nak_));
    PortalManager::ClearUserInfo(user_);
}

//==================================================================================================
// CHANGES: f110803.5L, withdraw from portal members
BOOLEAN PortalManager::OnPortalEnterResult(
    const MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK* recv_result_msg, User* user)
{
    MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK msg_nak;
    msg_nak.errorType = recv_result_msg->errorType;
    msg_nak.errorCode = recv_result_msg->errorCode;

    ns_functor::OnPortalEnterResultErrorHandling fail_notifier(&msg_nak, user);

    if (bool invalid_transaction = \
            (user->GetTransactionState() != TR_QUERY_PORTAL_UNTIL_GAMESERVER_ACK))
    {
        msg_nak.errorType = recv_result_msg->eRESULT_ROOM;
        msg_nak.errorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        return false;
    };
    // leave transaction state
    user->EndTransaction();
    //
    if (bool error_correction = \
            (recv_result_msg->errorType != recv_result_msg->eRESULT_ROOM))
    {
        msg_nak.errorType = recv_result_msg->eRESULT_ROOM;
        msg_nak.errorCode = RC::RC_ROOM_FAILED;
        return false;
    };
    // ROOM_RESULT 에러 세분화 할 필요가 있을 지도 모른다. 일단 forward 처리
    if (bool error_occured = (msg_nak.errorCode != RC::RC_ROOM_SUCCESS)) {
        return false;
    };

    // (NOTE) 누구라도 AG에 의한 검증은 거쳐야 한다. 이 시점에 
    typedef MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN   SYNC_TYPE;
    if (recv_result_msg->syncType == SYNC_TYPE::eSYNC_TEMP_PORTAL)
    {
        msg_nak.errorType = recv_result_msg->eRESULT_ROOM;
        msg_nak.errorCode = EnterZoneTempPortal(user, recv_result_msg->portalKey);
    };
    if (recv_result_msg->syncType == SYNC_TYPE::eSYNC_FIXED_PORTAL)
    {
        msg_nak.errorType = recv_result_msg->eRESULT_ROOM;
        msg_nak.errorCode = EnterZoneFixedPortal(user, recv_result_msg->portalKey);
        //이 시점에 포탈키는 이미 존재하고 ACK에 포함되어 있어야 한다.
    };
    // ROOM_RESULT 에러 세분화 할 필요가 있을 지도 모른다. 일단 forward 처리
    if (bool error_occured = (msg_nak.errorCode != RC::RC_ROOM_SUCCESS)) {
        return false;
    };

    return true;
}

BOOLEAN
PortalManager::JoinRoom(User* pUser, MSG_CG_ZONE_EX_ROOM_JOIN_ACK* OUT pJoinInfo)
{
    PortalUserInfo& rPortalUserInfo = pUser->GetPortalUserInfo();
    PORTALKEY portalKey = rPortalUserInfo.GetPortalKey();
    if(portalKey == INVALID_PORTAL_KEY)
        return FALSE;

    PORTALKEY prevPortalKey = 0;

    PortalManager* const this_ = PortalManager::Instance();
    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(pQuestPortalInfo == 0)
    {
        rPortalUserInfo.TryChangeState(rPortalUserInfo.eCommand_ClosePortal,
                                       portalKey, &prevPortalKey);
        return FALSE;
    }

    KEYTYPE roomKey = pQuestPortalInfo->m_RoomKey;
    const DWORD playerKey = pUser->GetPlayerKey();
    if(roomKey == 0 || pQuestPortalInfo->IsPortalQuestMember(playerKey) == FALSE)
    {
        // Problem? close portal & clear info.
        rPortalUserInfo.TryChangeState(rPortalUserInfo.eCommand_ClosePortal,
                                       portalKey, &prevPortalKey);
        pQuestPortalInfo->RemoveQuestPortalMember(playerKey);

        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"][%u] Invalid Status Check Again [%u]", __LINE__, roomKey == 0);
        return FALSE;
    }

    MAPCODE enterableMapCode = pQuestPortalInfo->enterableMapCode;
    const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(enterableMapCode);
    {   // make message : MSG_CG_ZONE_EX_ROOM_JOIN_ACK
        pJoinInfo->m_ktROOMKEY = roomKey;
        pJoinInfo->m_MapCode = enterableMapCode;
        pJoinInfo->m_byZONETYPE = pMapInfo->byMKind;
        pJoinInfo->m_fcCODE = pMapInfo->FCode[0];
    }
    // NOTE, __NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__
    // 1) 이 시점에는 해당 유저는 반드시 파티가 존재한다. (파티 생성/참가 등이 이미 이뤄진 상태)
    // 2) 실제 배틀존 입장시에는 더 이상 포탈이 존재하지 않을 수 있다. 따라서 해당 배틀존의
    //정보에 몇몇 기능은 이전되어야 한다. (ex: Master Player)
    // 3) 반대로 해당 배틀존에 연결된 포탈키는 2)에 의거 유효하지 않을 수 있다.

    // Remove Info
    rPortalUserInfo.TryChangeState(rPortalUserInfo.eCommand_ClosePortal, portalKey, &prevPortalKey);
    pQuestPortalInfo->RemoveQuestPortalMember(playerKey);

    return TRUE;
}

VOID
PortalManager::OnEnteredRoom(User* pUser, RoomInterface* pRoom)
{
    const eZONETYPE zoneType = pRoom->GetType();
    if(!IsValidCreatingZone(zoneType))
        return;

    PolicymentForRoom& rPolicymentForRoom = pRoom->RoomPolicyment;
    if((rPolicymentForRoom.FIELD & rPolicymentForRoom.QUEST_PORTAL_CONTROL) == FALSE)
        return;

    // 이 시점에는 portal이 존재하지 않을 수 있다.
    DWORD playerKey = pUser->GetPlayerKey();
    if(playerKey == rPolicymentForRoom.GetReservedPortalMaster())
    {
        WORD partyKey = pUser->GetPartyState().GetPartyKey();
        PortalManager* const this_ = PortalManager::Instance();
        this_->m_pPartyManager->ChangeMaster(partyKey, playerKey);
        pRoom->ChangeSpecialType(true, pUser);
    }
    else
    {
        MSG_CG_ZONE_CHANGE_SPECIALTYPE_CMD msg;
        msg.m_SpecialType = pRoom->GetRoomInfo()->m_SpecialType;
        pUser->SendPacket(&msg, sizeof(msg));
    }
}

VOID
PortalManager::OnRoomEvent(ZoneInterface* pZone, BOOLEAN isCreate)
{
    const eZONETYPE zoneType = pZone->GetType();
    if(!IsValidCreatingZone(zoneType))
        return;
    assert(pZone->IsRoomType());

    RoomInterface* pRoom = static_cast<RoomInterface*>(pZone);
    PolicymentForRoom& rPolicymentForRoom = pRoom->RoomPolicyment;
    if((rPolicymentForRoom.FIELD & rPolicymentForRoom.QUEST_PORTAL_CONTROL) == FALSE)
        return;

    PortalManager* const this_ = PortalManager::Instance();
    PORTALKEY portalKey = rPolicymentForRoom.GetPortalKey();
    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    // 방이 파괴될 때는 pQuestPortalInfo이 없을 가능성이 지극히 높으므로, Assert제거
    if(pQuestPortalInfo == 0)
        return;

    KEYTYPE roomKey = pRoom->GetKey();

    // 유저 관련 처리도 고민해야 할 것이다. 
    if(isCreate)
    {
        pQuestPortalInfo->m_PortalState = pQuestPortalInfo->ePortalState_RoomCreated;
        pQuestPortalInfo->m_RoomKey = roomKey;
        rPolicymentForRoom.SetReservedPortalMaster(pQuestPortalInfo->m_MasterPlayerKey);
    }
    else
    {
        // timeout이어야 한다.
        pQuestPortalInfo->m_PortalState = pQuestPortalInfo->ePortalState_Closed;
    }
}

BOOLEAN
PortalManager::CanJoinParty(User* pRequester, User* pResponser)
{
    {
        PortalUserInfo& rPortalUserInfo = pRequester->GetPortalUserInfo();
        PORTALKEY portalKey = rPortalUserInfo.GetPortalKey();
        if(portalKey != INVALID_PORTAL_KEY)
            return FALSE;
    };
    {
        PortalUserInfo& rPortalUserInfo = pResponser->GetPortalUserInfo();
        PORTALKEY portalKey = rPortalUserInfo.GetPortalKey();
        if(portalKey != INVALID_PORTAL_KEY)
            return FALSE;
    };

    return TRUE;
}

// LeaveParty Event in PartyManager
VOID
PortalManager::OnLeaveParty(User* pUser)
{
    PortalManager* const this_ = PortalManager::Instance();
    PortalUserInfo& rPortalUserInfo = pUser->GetPortalUserInfo();
    PORTALKEY portalKey = rPortalUserInfo.GetPortalKey();
    if(portalKey == INVALID_PORTAL_KEY)
        return;

    if(pUser->IsBeginTransaction())
    {   // irregular로 밖에 볼 수 없다.
        return;
    }

    const eZONE_STATE zoneState = pUser->GetZoneState();
    const BOOLEAN acceptableState =
        (ePRS_AT_ZONE & zoneState) && (ePRS_AT_CHARSELECT != zoneState);
    if(!acceptableState)
    {   // irregular
        return;
    }

    PORTALKEY prevPortalKey = 0;

    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(pQuestPortalInfo == 0)
    {
        rPortalUserInfo.TryChangeState(rPortalUserInfo.eCommand_ClosePortal,
                                       portalKey, &prevPortalKey);
        return;
    }

    KEYTYPE roomKey = pQuestPortalInfo->m_RoomKey;
    const DWORD playerKey = pUser->GetPlayerKey();
    if(pQuestPortalInfo->IsPortalQuestMember(playerKey) == FALSE)
    {
        // Problem? close portal & clear info.
        rPortalUserInfo.TryChangeState(rPortalUserInfo.eCommand_ClosePortal,
                                       portalKey, &prevPortalKey);
        pQuestPortalInfo->RemoveQuestPortalMember(playerKey);

        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"][%u] Invalid Status Check Again [%u]",
                __LINE__, roomKey == 0);
        return;
    }

    partyPortalOperator.Init(ePARTY_OPERATOR_PORTAL_CLOSE, 0, pUser->GetZoneKey());
    partyPortalOperator.SetQuestPortalInfo(pQuestPortalInfo->m_IsTempPortal,
                                           portalKey, pQuestPortalInfo);
    partyPortalOperator.PortalClose(pUser);
    pQuestPortalInfo->RemoveQuestPortalMember(playerKey);
}

// Process Event in RoomInterface
VOID
PortalManager::RoomLifetimeTimeout(RoomInterface* pRoomInterface)
{
    const eZONETYPE zoneType = pRoomInterface->GetType();
    if(!IsValidCreatingZone(zoneType))
        return;

    PolicymentForRoom& rPolicymentForRoom = pRoomInterface->RoomPolicyment;
    if((rPolicymentForRoom.FIELD & rPolicymentForRoom.QUEST_PORTAL_CONTROL) == FALSE)
        return;

    PortalManager* const this_ = PortalManager::Instance();
    PORTALKEY portalKey = rPolicymentForRoom.GetPortalKey();
    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(!FlowControl::FCAssert(pQuestPortalInfo != 0))
        return;
}

// cases = {
//      Disconnect Event in ~~,
//      CHANGES: f110803.5L, withdraw from portal members
// }
// CHANGES: f110803.5L, fixed a garbage data remained problem after the enterance fails
void PortalManager::ClearUserInfo(User* user)
{
    PortalUserInfo& portal_user_info = user->GetPortalUserInfo();
    PORTALKEY portal_key = portal_user_info.GetPortalKey();
    if (portal_key == INVALID_PORTAL_KEY) {
        return;
    };
    PORTALKEY prev_portal_key = 0;
    portal_user_info.TryChangeState(\
        portal_user_info.eCommand_UserClose, portal_key, &prev_portal_key);
    //
    PortalManager* const this_ = PortalManager::Instance();
    QuestPortalInfoInAgent* quest_portal_info = this_->FindQuestPortalInfo(portal_key);
    if (quest_portal_info == NULL) {
        return; //problem?
    };

    DWORD player_key = user->GetPlayerKey();
    quest_portal_info->RemoveQuestPortalMember(player_key);
}

VOID
PortalManager::PortalClose(QuestPortalInfoInAgent* pQuestPortalInfo)
{
    typedef QuestPortalInfoInAgent::UserList UserList;
    QuestPortalInfoInAgent::UserList& rUserList = pQuestPortalInfo->m_UserList;

    PORTALKEY portalKey = pQuestPortalInfo->portalKey;
    partyPortalOperator.Init(ePARTY_OPERATOR_PORTAL_CLOSE, 0, 0);
    partyPortalOperator.SetQuestPortalInfo(pQuestPortalInfo->m_IsTempPortal, portalKey,
                                           pQuestPortalInfo);

    const BOOLEAN isUserEmpty = rUserList.empty();

    FOREACH_CONTAINER(UserList::value_type playerKey, rUserList, UserList)
    {
        User* pUser = m_pUserManager->GetUserByObjKey(playerKey);
        if(pUser)
            partyPortalOperator.PortalClose(pUser);
    }

    const UserStateList& stateList = partyPortalOperator.GetUserStateList();
    FOREACH_CONTAINER(const UserState& state, stateList, UserStateList)
    {
        const DWORD playerKey = state.pUser->GetPlayerKey();
        // 특별히 현 시점 상태 결과에 대한 구분 처리를 할 필요가 없지만...
        switch(state.state)
        {
        case PortalUserInfo::eState_Success:
            pQuestPortalInfo->RemoveQuestPortalMember(playerKey);
            break;
        }
    }
}

BOOLEAN
PortalManager::RequestClosePortal(User* pUser)
{
    PortalUserInfo& rPortalInfo = pUser->GetPortalUserInfo();
    PORTALKEY portalKey = rPortalInfo.GetPortalKey();
    if(portalKey == INVALID_PORTAL_KEY)
        return FALSE;

    PortalManager* const this_ = PortalManager::Instance();
    QuestPortalInfoInAgent* pQuestPortalInfo = this_->FindQuestPortalInfo(portalKey);
    if(pQuestPortalInfo == NULL)
        return FALSE;

    PartyState& partyState = pUser->GetPartyState();
    WORD partyKey = partyState.GetPartyKey();

    partyPortalOperator.Init(ePARTY_OPERATOR_PORTAL_CLOSE, 0, 0);
    partyPortalOperator.SetQuestPortalInfo(pQuestPortalInfo->m_IsTempPortal, portalKey,
                                           pQuestPortalInfo);
    if(partyKey)
        this_->m_pPartyManager->For_eachMembers(partyKey, partyPortalOperator);
    else
        partyPortalOperator.PortalClose(pUser);

    pQuestPortalInfo->m_UserList.clear();

    return TRUE;
}

#ifdef _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
bool PortalManager::RequestWithdrawPortalMember(User* user, RC::eROOM_RESULT* const error_code)
{
    if ((user->GetZoneType() == eZONETYPE_LOBBY) ||
        (ePRS_AT_ZONE != (ePRS_AT_ZONE & user->GetZoneState())))
    {
        *error_code = RC::RC_ROOM_INVALIDROOMTYPE;
        return false;
    };
    PortalUserInfo* const portal_user_info = &user->GetPortalUserInfo();
    PORTALKEY portal_key = portal_user_info->GetPortalKey();
    if (portal_key == INVALID_PORTAL_KEY) {
        *error_code = RC::RC_ROOM_INVALIDROOMTYPE;
        return false;
    };
    if (user->IsBeginTransaction()) {
        *error_code = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        return false;
    };
    PortalManager* const this_ = PortalManager::Instance();
    QuestPortalInfoInAgent* quest_portal_info = this_->FindQuestPortalInfo(portal_key);
    if (quest_portal_info == NULL || quest_portal_info->m_IsTempPortal) {
        *error_code = RC::RC_ROOM_INVALIDROOMTYPE;
        return false;
    };

    const DWORD player_key = user->GetPlayerKey();
    if (quest_portal_info->IsPortalQuestMember(player_key) == false) {
        *error_code = RC::RC_ROOM_NOT_EXIST_MEMBER;
        return false;
    };
    if (PartyState* party_state = &user->GetPartyState())
    {
        if (WORD party_key = party_state->GetPartyKey())
        {
            RC::ePARTY_RESULT withdraw_result = AgentPartyManager::Instance()->LeaveParty(\
                user, eREASON_LEAVE_PARTY_INTEND);
            if (withdraw_result != RC::RC_PARTY_SUCCESS) {
                *error_code = RC::RC_ROOM_INVALIDROOMTYPE;
                return false;
            };
        };
    };
    partyPortalOperator.Init(ePARTY_OPERATOR_PORTAL_CLOSE, 0, 0);
    partyPortalOperator.SetQuestPortalInfo(quest_portal_info->m_IsTempPortal, portal_key,
                                           quest_portal_info);
    //if (partyKey)
    //    this_->m_pPartyManager->For_eachMembers(partyKey, partyPortalOperator);
    //else
    partyPortalOperator.PortalClose(user);
    quest_portal_info->RemoveQuestPortalMember(player_key);
    //
    *error_code = RC::RC_ROOM_SUCCESS;
    return true;
}
#endif

// AgentServer Update Process
VOID
PortalManager::Update()
{
    PortalManager* const this_ = PortalManager::Instance();
    PortalTable& rPortalTable = this_->m_PortalTable;
    PortalTable::iterator it = rPortalTable.begin();
    PortalTable::iterator itend = rPortalTable.end();
    while(it != itend)
    {
        QuestPortalInfoInAgent* pQuestPortalInfo = it->second;
        BOOLEAN needDelete = pQuestPortalInfo->m_UserList.empty() ||
                             pQuestPortalInfo->m_LifeTimeout.IsExpired();
        if(needDelete)
        {
            g_ZoneManager.ClosePortal(*pQuestPortalInfo,
                                      //static_cast<LobbyRoom*>(pQuestPortalInfo->m_pLobby),
                                      pQuestPortalInfo->m_RoomKey);
            this_->PortalClose(pQuestPortalInfo);
            // need release resource, Mission|InstanceDungeon의 경우는 자동 타임아웃을 이용한다.
            // 정상적인 경우 이미 유저들은 해당 방에 들어가 플레이를 하고 있을 것이다.
            // lobby는 별도로 제거할 필요가 있다.
            ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_PORTALKEY, pQuestPortalInfo->portalKey);
            TAllocDelete(QuestPortalInfoInAgent, pQuestPortalInfo);
            it = rPortalTable.erase(it);
            continue;
        }
        ++it;
    }
}


