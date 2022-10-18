#pragma once

/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: ������ ����Ǿ����� ģ�� �ʴ� �̺�Ʈ ���� ó���� �Ѵ�.

*/

#include "FriendInvitationListProc.h"

class FriendInvitationLevelChangedProc
{
public:
    FriendInvitationLevelChangedProc(void);
    ~FriendInvitationLevelChangedProc(void);

public:
    //ĳ������ ������ ���� �Ǿ��� ��~
    VOID ChangedEventLevel(User* pUser, LEVELTYPE level);

private:    
    //���� ������ DB Proxy �� �����Ѵ�.
    VOID SendReWardInfo(CHARGUID Charguid, const char* Charname, User* pSendUser, LEVELTYPE level, eSYSTEM_MEMO_TYPE Memotype);
    //�̺�Ʈ ���� ó��
    VOID EventEnd(User* pUser);

private:
    FriendInvitationListProc ListPorc;
    
};
