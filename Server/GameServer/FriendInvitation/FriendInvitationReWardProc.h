#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H
#define GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H
/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: ģ�� ��õ�ý����� ���� ������ ���� ó��
// NOTE: declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
*/

class FriendInvitationReWardProc
{
public:
    FriendInvitationReWardProc(void);
    ~FriendInvitationReWardProc(void);

public:    
    //ĳ���Ͱ� ������ ��û�ߴ�.
    VOID ReQuestReWardItem(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType);
    //����������� �����ϰ� �������� ������ �����Ѵ�.
    VOID ReceiveRewardItem(Player* pPlayer, LEVELTYPE RewardType);

    //���� ���� �޽����� ĳ���Ϳ� ������.
    VOID SendRewardFailMsg(Player* pPlayer, BYTE ErrorCode);
private:
    //����������� �߰��ɼ� �ִ��� Ȯ���Ѵ�.
    BOOL CheckInsertITem(Player* pPlayer, LEVELTYPE level);
    //�ش� ���� �޼��� ���� ��������� �ڵ带 ��ȯ�Ѵ�.
    SLOTCODE GetRewardItemCode(LEVELTYPE level);
    //�ش� ������ ��������� Index �� ��ȯ�Ѵ�.
    int ConvertItemIndexToLevel(LEVELTYPE level);
    //��� ���� ���� �޽����� �����Ѵ�.
    VOID SendRewardRequestMsg(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType);    
};

#endif //GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H