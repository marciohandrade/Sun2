/*

작성자: 문상현
제작일: 2009-11-10

간단한설명: 네트웍 세션 인터페이스

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
