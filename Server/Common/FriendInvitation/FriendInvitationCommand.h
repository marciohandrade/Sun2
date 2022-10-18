// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: //프로토콜 호출을 위한 인터페이스

*/

#pragma once

#include "PacketStruct.h"

//프로토콜 호출을 위한 인터페이스
class FriendInvitationCommand
{
public:
    virtual void Execute(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize) = 0;
};

