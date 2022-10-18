#include "stdafx.h"
#include "AgentParty/AgentPartyOperator.h"
#include "Zone/PortalManager.h"
#include "UserSessions/User.h"

VOID
MemberQueryResultOperator::PortalCreate(User* pUser)
{
    PortalUserInfo& portalInfo = pUser->GetPortalUserInfo();
    PORTALKEY prevPortalKey = INVALID_PORTAL_KEY;
    const PortalUserInfo::eState state =
        portalInfo.TryChangeState(portalInfo.eCommand_CreatePortal, m_PortalKey, &prevPortalKey);
    UserState userState = { pUser, state, prevPortalKey };
    m_UserStateList.push_back(userState);

    if (m_IsTempPortal)
    {
        BOOLEAN isSendClosePortal  = (state == portalInfo.eState_ChangedPortal);
        BOOLEAN isSendCreatePortal = (state == portalInfo.eState_Success || isSendClosePortal);

        if(isSendClosePortal)
        {
            MSG_CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD msg;
            msg.portalKey = prevPortalKey;
            pUser->SendPacket(&msg, sizeof(msg));
        }

        if(isSendCreatePortal)
        {
            MSG_CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD msg;
            msg.questPortalInfo = *m_pQuestPortalInfo;
            msg.questPortalInfo.portalKey = m_PortalKey;
            //party loop
            pUser->SendPacket(&msg, sizeof(msg));
        };
    }
#ifdef _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
    else
    {
        MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD msg_cmd;
        msg_cmd.portal_key = m_PortalKey;
        msg_cmd.open_state = msg_cmd.eOpenState_Created;
        msg_cmd.portal_lifetime = this->m_pQuestPortalInfo->portalLifetime;
        pUser->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
#endif
}

VOID
MemberQueryResultOperator::PortalChange(User* pUser)
{
    PortalUserInfo& portalInfo = pUser->GetPortalUserInfo();
    PORTALKEY prevPortalKey = INVALID_PORTAL_KEY;
    const PortalUserInfo::eState state =
        portalInfo.TryChangeState(portalInfo.eCommand_ChangePortal, m_PortalKey, &prevPortalKey);
    UserState userState = { pUser, state, prevPortalKey };
    m_UserStateList.push_back(userState);

    // current not used
}

VOID
MemberQueryResultOperator::PortalClose(User* pUser)
{
    PortalUserInfo& portalInfo = pUser->GetPortalUserInfo();
    PORTALKEY prevPortalKey = INVALID_PORTAL_KEY;
    const PortalUserInfo::eState state =
        portalInfo.TryChangeState(portalInfo.eCommand_ClosePortal, m_PortalKey, &prevPortalKey);
    UserState userState = { pUser, state, prevPortalKey };
    m_UserStateList.push_back(userState);

    if(m_IsTempPortal)
    {
        MSG_CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD msg;
        msg.portalKey = m_PortalKey;
        pUser->SendPacket(&msg, sizeof(msg));
    }
#ifdef _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
    else
    {
        MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD msg_cmd;
        msg_cmd.portal_key = m_PortalKey;
        msg_cmd.open_state = msg_cmd.eOpenState_Closed;
        msg_cmd.portal_lifetime = 0;
        pUser->SendPacket(&msg_cmd, sizeof(msg_cmd));
    }
#endif
}

VOID
MemberQueryResultOperator::PortalGetState(User* pUser)
{
    PortalUserInfo& portalInfo = pUser->GetPortalUserInfo();
    PORTALKEY prevPortalKey = INVALID_PORTAL_KEY;
    const PortalUserInfo::eState state =
        portalInfo.TryChangeState(portalInfo.eCommand_GetState, m_PortalKey, &prevPortalKey);
    UserState userState = { pUser, state, prevPortalKey };
    m_UserStateList.push_back(userState);
}

VOID
MemberQueryResultOperator::MigrateTowerOfCurse(User* pUser)
{
    ZONEKEY zoneKey = ZONEKEY(pUser->GetZoneKey());

    UserState userState = { pUser, zoneKey != 0, INVALID_PORTAL_KEY };
    m_UserStateList.push_back(userState);

    //pFromZone->LeaveUser( pUser, eSEND_SYN, m_pZone->GetType(), m_pZone->GetLinkedServerIndex() );
    //pUser->BeginTransaction( (eBEHAVE_TRANSACTION)m_Param1 );
    //pUser->SetTRRoomKey( m_pZone->GetKey() );
}

