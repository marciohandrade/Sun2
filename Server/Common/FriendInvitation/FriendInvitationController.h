// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

�ۼ���: ������
������: 2009-10-28

�����Ѽ���: ���� ģ�� �ʴ� �̺�Ʈ�� �����ϱ� ���� ��Ʈ�ѷ� Ŭ����

*/

#include <SunEventInfoParser.h>
#include "FriendInvitationInvoker.h"

#pragma once

#define CHECK_FRIENDINVITATION_EVENT_RETRUN if(!CheckFriendInvitationEvent()) return

class FriendInvitationController
{
public:
	FriendInvitationController(void);
	~FriendInvitationController(void);

	//���� ģ���ʴ� �̺�Ʈ�� ���������� üũ �Ѵ�.
	BOOL CheckFriendInvitationEvent();	
	//�̺�Ʈ ���� ������ �˷��ش�.
	LEVELTYPE GetEventMaxLevel();
	//�̺�Ʈ ���� ����
	LEVELTYPE GetEventMinLevel();
    //�߰� ����ġ ������
    float GetAddExpRatio(LEVELTYPE level);    
    
    //�������� �ڵ鷯 �����Ѵ�.
    VOID ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

protected:
	EVENT_INFO* GetFriendEvent();
    VOID SetInvokerPtr(FriendInvitationInvoker* invoker);   

protected:
    FriendInvitationInvoker* m_invoker;

private:
    EVENT_INFO* event_info_;
	
};

