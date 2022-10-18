/*

�ۼ���: ������
������: 2009-11-10

�����Ѽ���: ������Ʈ ���� ��Ʈ�� ����

*/

#pragma once

#include "INetworkSession.hpp"

class AgentNetWorkSession : public INetWorkSession
{
public:
    BOOL SendPacket(User* pUser, INVITATEFRIENDLIST_INFO& InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION Action)
    {
        InvitateFriendList.m_FreindAction = static_cast<uint8_t>(Action);

        MSG_AW_FRIENDINVITATE_INVITATELIST_CMD Invitatecmd;	
        // (CHANGES) (f100624.2L)
        Invitatecmd.m_InvitateFriendListInfo = InvitateFriendList;
        //memcpy(&Invitatecmd.m_InvitateFriendListInfo, &InvitateFriendList, sizeof(INVITATEFRIENDLIST_INFO));

        ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );

        if( !pAgentServer )
        {
            MessageOut( eCRITICAL_LOG, "Send InvitateFriendList fail - No AgentServer!!![%u]", pUser->GetChannelID() );
            return FALSE;
        }
        return pAgentServer->Send( (BYTE*)&Invitatecmd , Invitatecmd.GetSize() );        
    };
};
