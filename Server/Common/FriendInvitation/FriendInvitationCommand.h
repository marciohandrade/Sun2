// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: //�������� ȣ���� ���� �������̽�

*/

#pragma once

#include "PacketStruct.h"

//�������� ȣ���� ���� �������̽�
class FriendInvitationCommand
{
public:
    virtual void Execute(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize) = 0;
};

