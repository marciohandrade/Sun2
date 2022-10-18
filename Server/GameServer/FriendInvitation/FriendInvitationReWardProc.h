#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H
#define GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H
/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: 친구 추천시스템의 보상 아이템 관련 처리
// NOTE: declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
*/

class FriendInvitationReWardProc
{
public:
    FriendInvitationReWardProc(void);
    ~FriendInvitationReWardProc(void);

public:    
    //캐릭터가 보상을 요청했다.
    VOID ReQuestReWardItem(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType);
    //보상아이템을 지급하고 유저에게 정보를 전송한다.
    VOID ReceiveRewardItem(Player* pPlayer, LEVELTYPE RewardType);

    //보상 실패 메시지를 캐릭터에 보낸다.
    VOID SendRewardFailMsg(Player* pPlayer, BYTE ErrorCode);
private:
    //보상아이템이 추가될수 있는지 확인한다.
    BOOL CheckInsertITem(Player* pPlayer, LEVELTYPE level);
    //해당 레벨 달성에 대한 보상아이템 코드를 반환한다.
    SLOTCODE GetRewardItemCode(LEVELTYPE level);
    //해당 레벨을 보상아이템 Index 로 변환한다.
    int ConvertItemIndexToLevel(LEVELTYPE level);
    //디비에 보상 받을 메시지를 전송한다.
    VOID SendRewardRequestMsg(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType);    
};

#endif //GAMESERVER_FRIEND_INVITATION_REWARD_PROC_H