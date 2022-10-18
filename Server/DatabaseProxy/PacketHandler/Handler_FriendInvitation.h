#pragma once
#ifndef DBPROXY_HANDLER_FRIEND_INVITATION_H
#define DBPROXY_HANDLER_FRIEND_INVITATION_H

// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

�ۼ���: ������
������: 2009-10-28

�����Ѽ���: ���� ģ�� �ʴ� �̺�Ʈ�� �����ϱ� ���� ��Ʈ�ѷ� Ŭ���� (��� PROXY ������)

*/

#include "PacketStruct_DW.h"
#include "../Common/FriendInvitation/FriendInvitationController.h"

class DFriendInvitationController : public util::Singleton<DFriendInvitationController>, 
                                    public FriendInvitationController
{
public:
	DFriendInvitationController(void);
	~DFriendInvitationController(void);	

    //ĳ���� ������
    VOID OnCharacterCreateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    
    //������ ����Ʈ ��ȯ
    VOID OnRewardItemListSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardItemListSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //������ ���� ������Ʈ (������ �޾��� ���)
    VOID OnRewardItemUpdateSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardItemUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //������ �߰�
    VOID OnRewardInfoInsertCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardInfoInsertDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

    //��ȯ �ð� ���� ��ȯ
    VOID OnRecallInfoSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRecallInfoSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //��ȯ�� �ð� ���� ����
    VOID OnRecallInfoUpdateCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRecallInfoUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    
    //ģ����õ���� �߰�
    VOID OnAddInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

    //ģ�� ��õ���� ����
    VOID OnRemoveInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRemoveInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

	//ģ�� ���� ������ �����.
	VOID GetFriendDelQuery(char szQuery[], MSG_DW_FRIEND_DEL_SYN * pRecvMsg);    
protected:
private:    
    
};

#endif //DBPROXY_HANDLER_FRIEND_INVITATION_H