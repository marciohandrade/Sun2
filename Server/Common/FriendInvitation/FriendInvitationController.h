// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

작성자: 문상현
제작일: 2009-10-28

간단한설명: 썬의 친구 초대 이벤트를 지원하기 위한 컨트롤러 클래스

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

	//현재 친구초대 이벤트가 진행중인지 체크 한다.
	BOOL CheckFriendInvitationEvent();	
	//이벤트 종료 레벨을 알려준다.
	LEVELTYPE GetEventMaxLevel();
	//이벤트 시작 레벨
	LEVELTYPE GetEventMinLevel();
    //추가 경험치 증가율
    float GetAddExpRatio(LEVELTYPE level);    
    
    //프로토콜 핸들러 수행한다.
    VOID ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

protected:
	EVENT_INFO* GetFriendEvent();
    VOID SetInvokerPtr(FriendInvitationInvoker* invoker);   

protected:
    FriendInvitationInvoker* m_invoker;

private:
    EVENT_INFO* event_info_;
	
};

