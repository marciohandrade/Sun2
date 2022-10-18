#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_RECALPROC_H
#define GAMESERVER_FRIEND_INVITATION_RECALPROC_H
/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: ��õ ģ���� ���� ���� ���ν�����.

*/

class FriendInvitationRecallProc
{
public:
    FriendInvitationRecallProc(void);
    ~FriendInvitationRecallProc(void);

public:
    //DB RPOXY �� ���� �ð� ����� ��û�Ѵ�.
    VOID SendRecallTimeSelect(CHARGUID charGUID, Player *pPlayer);    
    //��ȯ�� ��󿡰� ��ȯ �ص� �Ǵ��� ���´�.
    VOID SendRecallQuestionMsg(Player* pPlayer, Player* pRecallPlayer);
    //��ȯ�� ��û�� �������� ��ȯ ���� ��Ŷ�� ������.
    VOID SendRecallFailMsg(Player* pPlayer, BYTE ErrCode);
    //ģ������ ��ȯ ��û�� ������ ������ �޽��� �ڽ��� �����ִ��� Ȯ���Ѵ�. 
    //TRUE �� ���� �޽��� �ڽ��� �����ִ°��̴�.
    BOOL IsRequestRecall(Player* pPlayer, DWORD FriendGUID);
    //��ȯ ��Ÿ���� �������� �˻��Ѵ�.
    BOOL IsRecalltimeExpired(Player* pPlayer, DWORD FriendGUID);    
    //������ �����Ѵ�.
    VOID ReCallUser(Player* pPlayer, Player* pRecallPlayer);
    //������Ʈ�� ���� ���� ���� �޽��� ����
    VOID ReCallACK(const Player* Caller, const CHARGUID CalleeGUID);
    //������Ʈ�� ���� ���� ���� �޽��� ����
    VOID ReCallNAK(Player* Caller, const BYTE ErrorCode);

private:
    //��ȯ�ϴ� ����� ��ȯ ���ϴ� ����� ��ȯ ������ ��ġ�� �ִ��� Ȯ������.
    BOOL IsPossibileReCallLocation(Player* pPlayer, Player* pRecallPlayer);
    //��ȯ ���ϴ� ����ڿ��� ��ȯ ��ġ�� �����Ѵ�.
    BOOL SendRecallPosition(Player* pPlayer, Player* pRecallPlayer);
    //��ȯ�� �ð��� ������Ʈ�Ѵ�.
    VOID SendRecallTimeUpdate(Player* pPlayer, Player* pRecallPlayer);
    //��ȯ�� �������� ���� ��Ŷ�� ������.
    VOID SendRecallSucceedMsg(Player* pPlayer);
};

#endif //GAMESERVER_FRIEND_INVITATION_RECALPROC_H