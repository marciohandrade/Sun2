/*

�ۼ���: ������
������: 2009-10-28

�����Ѽ���: ���� ģ�� �ʴ� �̺�Ʈ�� �����ϱ� ���� ��Ʈ�ѷ� Ŭ���� (���� ������)

*/


#pragma once

#include "../Common/FriendInvitation/FriendInvitationController.h"
#include "User.h"
#include "GameDBProxySession.h"
#include "FriendInvitationListProc.h"
#include "FriendInvitationLevelChangedProc.h"

class WFriendInvitationController : public util::Singleton<WFriendInvitationController>, 
                                    public FriendInvitationController
{
public:
	WFriendInvitationController(void);
	~WFriendInvitationController(void);	

    //ģ����õ������ ģ���� �����Ǿ�����
    VOID OnAddInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //������ ������ ������
    VOID OnCharacterLevelUp(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //ģ������� ���� ������ 
    VOID OnFriendRemove(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //ģ���� ���� ������
    VOID OnFriendBlock(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
	//ģ�� ����� �����Ҷ� ģ���ʴ� ��ϵ� ���� �����Ѵ�.
	VOID OnAddFriend(STRUCT_FRIEND_INFO& sFriendInfo, STRUCT_FRIEND_DB_INFO& sFriendDBInfo,                      User* pUser);
	//������Ʈ ������ ģ����õ ����� �����Ѵ�.
	VOID SendInvitateFriendList(User* pUser, DWORD ServerType);
	//ģ�� ���� �ڵ带 ��ȯ�Ѵ�.
	BYTE GetFriendSts(User* pUser, DWORD FriendGUID);	         
protected:
private:
    FriendInvitationListProc ListProc;
    FriendInvitationLevelChangedProc LevelChangedProc;

    //ģ�� ���踦 ���� �Ѵ�.
    VOID InviateFriendRemove(CHARGUID CharacterGUIDFrom, CHARGUID CharacterGUIDTo);
};
