#include "stdafx.h"
#include ".\FriendInvitationInvoker.h"
// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND

FriendInvitationInvoker::FriendInvitationInvoker(void)
{    
}

FriendInvitationInvoker::~FriendInvitationInvoker(void)
{
    RemoveConCreateCommads();
}

void FriendInvitationInvoker::InsertConCreateCommad(DWORD key, FriendInvitationCommand* value)
{	
    CONCRETE_COMMAND_MAP_RESULT result = 
		m_ConcreteCommand.insert(CONCRETE_COMMAND_MAP_PAIR(key,value));
    
    if(!result.second)
    {
        SUNLOG( eCRITICAL_LOG, "FriendInvitationInvoker Insert Command Fail" );
    }
}

void FriendInvitationInvoker::RemoveConCreateCommads()
{
    for( CONCRETE_COMMAND_MAP_ITER iter = m_ConcreteCommand.begin() ; iter != m_ConcreteCommand.end() ; ++iter )
    {
        FriendInvitationCommand* pCommand = iter->second;
        if( NULL != pCommand )
        {
            delete pCommand;
            pCommand = NULL;
        }
    }
    m_ConcreteCommand.clear();
}

void FriendInvitationInvoker::ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    CONCRETE_COMMAND_MAP::iterator iterFind = 
        m_ConcreteCommand.find(MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol));

    if(m_ConcreteCommand.end() == iterFind)
    {
        SUNLOG( eCRITICAL_LOG, "Unkonw FriendInvitation Command ID (%u, %u)",  pMsg->m_byCategory, pMsg->m_byProtocol);
    }
    else
    {
        ((FriendInvitationCommand*)iterFind->second)->Execute(pServerSession, pMsg, wSize);
    }
}

