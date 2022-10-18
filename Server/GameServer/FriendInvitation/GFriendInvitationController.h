#pragma once
#ifndef GAMESERVER_FRIEND_INVITATION_CONTROLLER_H
#define GAMESERVER_FRIEND_INVITATION_CONTROLLER_H
/*

작성자: 문상현
제작일: 2009-10-28

간단한설명: 썬의 친구 초대 이벤트를 지원하기 위한 컨트롤러 클래스 (게임 서버용)
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
    //친구초대 관계의 친구 정보
    VOID OnInvitatedFriendListSelectCMD(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //소환 요청에 응했음
    VOID OnRecallQuestionACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //소환 요청에 거부했음
    VOID OnRecallQuestionNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //소환 요청이 왓을 때
    VOID OnRecallSYN(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //소환한 시간 정보를 받을 때
    VOID OnRecallInfoSelectACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //소환 시간 정보 받기 실패
    VOID OnRecallInfoSelectNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //보상 요청이 디비에서 정상적으로 기록 되었을때
    VOID OnRewardItemUpdateACK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //보상 요청이 디비에 기록 실패
    VOID OnRewardItemUpdateNAK(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //보상 아이템을 보상받겠다는 요청이 왔을때
    VOID OnRewardSYN(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);

    //파티원 추가 되었을때
    VOID OnParty_Join(WORD PartyKey, DWORD MemberKey);
    //파티원 제거 되었을때
    VOID OnParty_Leave(WORD PartyKey, DWORD MemberKey);    

    //에이전트에서 리콜 처리 성공
    VOID OnAGRecallAck(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);
    //에이전트에서 리콜 처리 실패
    VOID OnAGRecallNak(LPVOID /*pServerSession*/, MSG_BASE * pMsg, WORD /*wSize*/);


    //필드, 배틀 서버간 이동시 친구 초대 정보를 DBPROX 에 전송 하기위해 친구정보를 준다.
    VOID GetInvitatedFrindInfo(const Player* player,
                               MSG_BASE * pMsg,
                               const eSERIALIZE eType);

    EVENT_INFO* GetFriendEvent() {return FriendInvitationController::GetFriendEvent();};   
private:
    FriendInvitationRecallProc RecallProc;
    FriendInvitationReWardProc ReWardProc;    
};

#endif //GAMESERVER_FRIEND_INVITATION_CONTROLLER_H