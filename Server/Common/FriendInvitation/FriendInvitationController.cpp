#include "stdafx.h"
#include "./FriendInvitationController.h"
// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND

FriendInvitationController::FriendInvitationController(void) : m_invoker(NULL), event_info_(NULL)
{
    SunEventInfoParser* const pEventParser = SunEventInfoParser::Instance();    
    event_info_ = pEventParser->FindEventInfoFromMode(eEVENT_FRIENDINVATION_EVENT);
}

FriendInvitationController::~FriendInvitationController(void)
{    
    if(NULL != m_invoker)
    {        
        delete m_invoker;
    }
}

VOID FriendInvitationController::SetInvokerPtr(FriendInvitationInvoker* invoker) 
{
    m_invoker = invoker;
    m_invoker->InsertConCreateCommads();
};   

EVENT_INFO* FriendInvitationController::GetFriendEvent()
{
    return event_info_;	
}

BOOL FriendInvitationController::CheckFriendInvitationEvent()
{
	if(GetFriendEvent())
    {
        if(GetFriendEvent()->CheckEvent()) 
        {
            return TRUE;//친구초대 이벤트 진행중
        }
    }
	return FALSE;	
}

LEVELTYPE FriendInvitationController::GetEventMaxLevel()
{
	if(!CheckFriendInvitationEvent()) return 0;

	return GetFriendEvent()->m_byApplyToLvRng[EVENT_INFO::LEVEL_MAX];
}

LEVELTYPE FriendInvitationController::GetEventMinLevel()
{
	if(!CheckFriendInvitationEvent()) return 0;

	return GetFriendEvent()->m_byApplyToLvRng[EVENT_INFO::LEVEL_MIN];
}

float FriendInvitationController::GetAddExpRatio(LEVELTYPE level)
{
    if(!CheckFriendInvitationEvent()) return 0.0f;    

    if(GetFriendEvent()->CheckLevel(level))
    {
        return GetFriendEvent()->m_fRewardExpRate;
    }    
    return 0.0f;
}

VOID FriendInvitationController::ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    ASSERT(m_invoker);
    if(NULL == m_invoker)
    {
         SUNLOG(eCRITICAL_LOG, "Invoker Not Created");
    }

    m_invoker->ProcessCommand(pServerSession, pMsg, wSize);
}

