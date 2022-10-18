#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_CONTROLLER_H
#define GAMESERVER_FRIEND_INVITATION_CONTROLLER_H
/*

�ۼ���: ������
������: 2009-10-28

�����Ѽ���: ���� ģ�� �ʴ� �̺�Ʈ�� �����ϱ� ���� ��Ʈ�ѷ� Ŭ���� (���� ������)
// NOTE: declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
*/

#include "../Common/FriendInvitation/FriendInvitationController.h"
#include "friendinvitationReWardproc.h"
#include "friendinvitationRecallproc.h"

class GFriendInvitationController : public util::Singleton<GFriendInvitationController>, public FriendInvitationController
{
public:
    GFriendInvitationController(void);
    ~GFriendInvitationController(void);

public:    
    //ģ���ʴ� ������ ģ�� ����
    VOID OnInvitatedFriendListSelectCMD(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //��ȯ ��û�� ������
    VOID OnRecallQuestionACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //��ȯ ��û�� �ź�����
    VOID OnRecallQuestionNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //��ȯ ��û�� ���� ��
    VOID OnRecallSYN(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //��ȯ�� �ð� ������ ���� ��
    VOID OnRecallInfoSelectACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //��ȯ �ð� ���� �ޱ� ����
    VOID OnRecallInfoSelectNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //���� ��û�� ��񿡼� ���������� ��� �Ǿ�����
    VOID OnRewardItemUpdateACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //���� ��û�� ��� ��� ����
    VOID OnRewardItemUpdateNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //���� �������� ����ްڴٴ� ��û�� ������
    VOID OnRewardSYN(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);

    //��Ƽ�� �߰� �Ǿ�����
    VOID OnParty_Join(WORD PartyKey, DWORD MemberKey);
    //��Ƽ�� ���� �Ǿ�����
    VOID OnParty_Leave(WORD PartyKey, DWORD MemberKey);    

    //������Ʈ���� ���� ó�� ����
    VOID OnAGRecallAck(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //������Ʈ���� ���� ó�� ����
    VOID OnAGRecallNak(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);


    //�ʵ�, ��Ʋ ������ �̵��� ģ�� �ʴ� ������ DBPROX �� ���� �ϱ����� ģ�������� �ش�.
    VOID GetInvitatedFrindInfo(const Player* player,
                               MSG_BASE * pMsg,
                               const eSERIALIZE eType);

    EVENT_INFO* GetFriendEvent() {return FriendInvitationController::GetFriendEvent();};   
private:
    FriendInvitationRecallProc RecallProc;
    FriendInvitationReWardProc ReWardProc;    
};

#endif //GAMESERVER_FRIEND_INVITATION_CONTROLLER_H