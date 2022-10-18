#include "stdafx.h"

#include <PacketHandler/PacketHandler.h>
#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/ServerSessionManager.h>

LinkedServerSession::LinkedServerSession( UserSession* pUser, DWORD dwServerSessionIndex )
{
	LinkUserWithServer( pUser, dwServerSessionIndex );
}

BOOL	LinkedServerSession::IsAcceptableUser( UserSession* pUser, BOOL bLogging )
{
	if( pUser && (pUser->GetUserType() != UT_TEMP_USER) )
		return TRUE;

	if( bLogging )
	{
		MessageOut( eCRITICAL_LOG, _T("[LinkUserWithServer] Try to mapping Invalid User [%u:%s]"),
			(pUser ? 1 : 0), (pUser ? _T("TEMP_USER") : _T("")) );
	}

	return FALSE;
}

BOOL	LinkedServerSession::LinkUserWithServer( UserSession* pUser, DWORD dwServerSessionIndex )
{
	ServerSessionEx* pServerSession = ServerSessionManager::Instance()->FindServer( dwServerSessionIndex );
	return LinkUserWithServer( pUser, pServerSession );
}

BOOL	LinkedServerSession::LinkUserWithServer( UserSession* pUser, ServerSessionEx* pServerSession )
{
	if( !IsAcceptableUser( pUser, TRUE ) )
		return FALSE;

	m_pUser = (User*)pUser;

	ASSERT( (pServerSession || !"[LinkUserWithServer]") );
	if( !pServerSession )
		return FALSE;

	ASSERT( m_pServerSession != pServerSession );

	DWORD dwSessionIndex = pServerSession->GetSessionIndex();

	if( !dwSessionIndex )
	{
		SUNLOG( eCRITICAL_LOG, _T("[User::LinkServer] LinkedServer SessionIndex is Invalid!") );
		m_dwSessionIndex = 0;
		m_pServerSession = NULL;

		return FALSE;
	}

	m_dwSessionIndex = dwSessionIndex;
	m_pServerSession = pServerSession;

	return TRUE;
}

BOOL	LinkedServerSession::SendToServer( MSG_BASE_FORWARD* pMsg, WORD wSize )
{
	if( m_pUser && GetLinkedServer() )
	{
		pMsg->m_dwKey = m_pUser->GetUserGUID();

		SHOW_PACKET_STREAM( pMsg, wSize, "A2G" );

		return GetLinkedServer()->Send( (BYTE*)pMsg, wSize );
	}

	SHOW_PACKET_STREAM( pMsg, wSize, "A2G FAIL!" );

	if( m_pUser )
	{
		BYTE byCategory = pMsg->m_byCategory;
		switch( byCategory )
		{
		case CG_SYNC:
		case CG_CONVERSATION:
		case CG_TRIGGER:
		case CG_BATTLE:
			{
				//SUNLOG( eCRITICAL_LOG,
				//	_T("[LinkedServerIsNULL][ID:%u] : Continuous Session Connection [C:P=%03u:%03u]"),
				//	userOP.GetUserGUID(), pMsg->m_byCategory, pMsg->m_byProtocol );

				DEBUG_INTERRUPT();
			}
			return FALSE;
		}

		SECURELOG( SECURELOG_USERINFO(m_pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_UNEXPECTED_PACKET),
			_T("[LinkedServerIsNULL][ID:%u] : Unexpected Packet. [C:P=%03u:%03u]"), byCategory, pMsg->m_byProtocol );

		SUNLOG( eCRITICAL_LOG, _T("[LinkedServerIsNULL][ID:%u] : Unexpected Packet. [C:P=%03u:%03u]"), byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

#ifdef _DEBUG
	SUNLOG( eCRITICAL_LOG, _T("ASSERTION SYNC VIOLATION: LINKED SESSION") );
#endif

	return FALSE;
}


//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

PortalUserInfo::~PortalUserInfo()
{
    PORTALKEY portalKey;
    TryChangeState(eCommand_UserClose, 0, &portalKey);
}

BOOLEAN
PortalUserInfo::IsValidState()
{
    if(m_pUser->IsBeginTransaction())
        return FALSE;

    const eZONE_STATE zoneState = m_pUser->GetZoneState();
    const BOOLEAN acceptableState = (ePRS_AT_ZONE & zoneState) &&
                                    (ePRS_AT_CHARSELECT != zoneState ||
                                     ePRS_AT_LOBBY != zoneState);
    if(!acceptableState)
        return FALSE;

    return TRUE;
}

PortalUserInfo::eState
PortalUserInfo::TryChangeState(eCommand command, PORTALKEY portalKey, PORTALKEY* OUT prevPortalKey)
{
    if(m_pUser == 0)
        return eState_Fail;
    assert(prevPortalKey);

    eState result = eState_Success;
    switch(command)
    {
    case eCommand_CreatePortal:
        if(!IsValidState())
        {
            *prevPortalKey = m_PortalKey;
            m_PortalKey = INVALID_PORTAL_KEY;
            return eState_Fail;
        }
        if(m_PortalKey != INVALID_PORTAL_KEY)
        {
            *prevPortalKey = m_PortalKey;
            result = eState_ChangedPortal;
        }
        m_PortalKey = portalKey;

        return result;
    case eCommand_ChangePortal:
        if(!IsValidState())
        {
            *prevPortalKey = m_PortalKey;
            m_PortalKey = INVALID_PORTAL_KEY;
            return eState_Fail;
        }
        *prevPortalKey = m_PortalKey;
        if(m_PortalKey != INVALID_PORTAL_KEY)
            m_PortalKey = portalKey;
        else
            result = eState_NoRelevant;

        return result;
    case eCommand_EnterPortal:
        if(!IsValidState())
        {
            m_PortalKey = INVALID_PORTAL_KEY;
            return eState_Fail;
        }
        if(!FlowControl::FCAssert(m_PortalKey == portalKey))
            result = eState_Fail;

        return result;
    case eCommand_ClosePortal:
        // Thinking!!
        *prevPortalKey = m_PortalKey;
        m_PortalKey = INVALID_PORTAL_KEY;
        return eState_Success;
    case eCommand_GetState:
        *prevPortalKey = m_PortalKey;
        return eState_Success;
    case eCommand_UserClose:
        m_PortalKey = INVALID_PORTAL_KEY;
        return eState_Success;
    }
    return eState_Fail;
}
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
namespace nsPacket {

ReAuthInfo::ReAuthInfo()
    : m_bUseReAuth(0)
    , m_ReAuthKey(0)
    , m_pReAuthStream(0)
{
    m_Timeout.Disabled();
    ZeroMemory(&m_ReAuthSteps, sizeof(m_ReAuthSteps));
}

ReAuthInfo::~ReAuthInfo()
{
    if (m_pReAuthStream) {
        TAllocDelete(ReAuthTokenStream, m_pReAuthStream);
    }
}

void
ReAuthInfo::Clear()
{
    if (m_pReAuthStream) {
        TAllocDelete(ReAuthTokenStream, m_pReAuthStream);
    }
    new (this) ReAuthInfo;
}

void
ReAuthInfo::SetReAuthStream(const ReAuthTokenStream* arg)
{
    if(m_pReAuthStream == 0)
        m_pReAuthStream = TAllocNew(ReAuthTokenStream);

    CopyMemory(m_pReAuthStream, arg, sizeof(*m_pReAuthStream));
}

}; //end of namespace
#endif


#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__

BOOLEAN
ShopUserQuerySlot::QueryStart(const MSG_CG_SHOP_SEND_GIFT_SYN* message)
{
    if(FlowControl::FCAssert(message_ == 0))
    {
        message_ = TAllocNew(MSG_CG_SHOP_SEND_GIFT_SYN);
        CopyMemory(message_, message, sizeof(*message_));
        message_->m_tszCharName[_countof(message_->m_tszCharName) - 1] = '\0';
        //_strupr(message_->m_tszCharName);
        transaction_ = true;
        return true;
    }

    return false;
}

void
ShopUserQuerySlot::QueryEnd()
{
    // QueryStart를 실패하고 들어올 수 있다는 가정으로 처리한다.
    if(message_) {
        TAllocDelete(MSG_CG_SHOP_SEND_GIFT_SYN, message_);
        message_ = 0;
    }
    transaction_ = false;
}

#endif

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
