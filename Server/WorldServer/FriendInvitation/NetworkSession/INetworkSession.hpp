/*

�ۼ���: ������
������: 2009-11-10

�����Ѽ���: ��Ʈ�� ���� �������̽�

*/

#pragma once

#include "PacketStruct_AW.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include "User.h"

class INetWorkSession
{
public:
    virtual BOOL SendPacket(User* pUser, INVITATEFRIENDLIST_INFO& InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION Action) = 0;
};
