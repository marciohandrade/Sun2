/*

�ۼ���: ������
������: 2009-11-10

�����Ѽ���: Ŭ���̾�Ʈ ��Ʈ�� ����

*/

#pragma once

#include "INetworkSession.hpp"

class ClientNetWorkSession : public INetWorkSession
{
public:
    BOOL SendPacket(User* pUser, INVITATEFRIENDLIST_INFO& InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION Action)
    {        
        InvitateFriendList.m_FreindAction = Action;

        MSG_CW_FRIENDINVITATE_INVITATELIST_CMD Invitatecmd;	

        if(INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL == InvitateFriendList.m_FreindAction)
        {
            InvitateFriendList.m_FriendSts = User::FRIEND_NORMAL;
        }
        // (CHANGES) (f100624.2L)
        Invitatecmd.m_InvitateFriendInfo = InvitateFriendList;
        //memcpy(&Invitatecmd.m_InvitateFriendInfo, &InvitateFriendList, sizeof(INVITATEFRIENDLIST_INFO));                

        return pUser->SendPacket((BYTE*)&Invitatecmd , Invitatecmd.GetSize());
    };
};
