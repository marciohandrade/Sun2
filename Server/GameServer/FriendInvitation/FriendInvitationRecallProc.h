#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_RECALPROC_H
#define GAMESERVER_FRIEND_INVITATION_RECALPROC_H
/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: 추천 친구의 리콜 관련 프로시져들.

*/

class FriendInvitationRecallProc
{
public:
    FriendInvitationRecallProc(void);
    ~FriendInvitationRecallProc(void);

public:
    //DB RPOXY 에 리콜 시간 목록을 요청한다.
    VOID SendRecallTimeSelect(CHARGUID charGUID, Player *pPlayer);    
    //소환할 대상에게 소환 해도 되는지 묻는다.
    VOID SendRecallQuestionMsg(Player* pPlayer, Player* pRecallPlayer);
    //소환을 요청한 유저에게 소환 실패 패킷을 보낸다.
    VOID SendRecallFailMsg(Player* pPlayer, BYTE ErrCode);
    //친구에게 소환 요청을 보낼때 이전에 메시지 박스가 남아있는지 확인한다. 
    //TRUE 면 이전 메시지 박스가 남아있는것이다.
    BOOL IsRequestRecall(Player* pPlayer, DWORD FriendGUID);
    //소환 쿨타임이 지났는지 검사한다.
    BOOL IsRecalltimeExpired(Player* pPlayer, DWORD FriendGUID);    
    //유저를 리콜한다.
    VOID ReCallUser(Player* pPlayer, Player* pRecallPlayer);
    //에이전트로 부터 리콜 성공 메시지 수신
    VOID ReCallACK(const Player* Caller, const CHARGUID CalleeGUID);
    //에이전트로 부터 리콜 실패 메시지 수신
    VOID ReCallNAK(Player* Caller, const BYTE ErrorCode);

private:
    //소환하는 사람과 소환 당하는 사람이 소환 가능한 위치에 있는지 확인힌다.
    BOOL IsPossibileReCallLocation(Player* pPlayer, Player* pRecallPlayer);
    //소환 당하는 사용자에게 소환 위치를 전송한다.
    BOOL SendRecallPosition(Player* pPlayer, Player* pRecallPlayer);
    //소환한 시간을 업데이트한다.
    VOID SendRecallTimeUpdate(Player* pPlayer, Player* pRecallPlayer);
    //소환한 유저에게 성공 패킷을 보낸다.
    VOID SendRecallSucceedMsg(Player* pPlayer);
};

#endif //GAMESERVER_FRIEND_INVITATION_RECALPROC_H