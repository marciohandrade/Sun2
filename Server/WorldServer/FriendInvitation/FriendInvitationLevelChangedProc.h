#pragma once

/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: 레벨이 변경되었을때 친구 초대 이벤트 관련 처리를 한다.

*/

#include "FriendInvitationListProc.h"

class FriendInvitationLevelChangedProc
{
public:
    FriendInvitationLevelChangedProc(void);
    ~FriendInvitationLevelChangedProc(void);

public:
    //캐릭터의 레벨이 변경 되었을 때~
    VOID ChangedEventLevel(User* pUser, LEVELTYPE level);

private:    
    //보상 정보를 DB Proxy 에 전송한다.
    VOID SendReWardInfo(CHARGUID Charguid, const char* Charname, User* pSendUser, LEVELTYPE level, eSYSTEM_MEMO_TYPE Memotype);
    //이벤트 종료 처리
    VOID EventEnd(User* pUser);

private:
    FriendInvitationListProc ListPorc;
    
};
