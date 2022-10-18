/*

작성자: 문상현
제작일: 2009-10-28

간단한설명: 썬의 친구 초대 이벤트를 지원하기 위한 컨트롤러 클래스 (월드 서버용)

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

    //친구추천관계의 친구가 생성되었을때
    VOID OnAddInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //유저가 레벨업 했을때
    VOID OnCharacterLevelUp(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //친구목록을 삭제 했을때 
    VOID OnFriendRemove(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //친구가 차단 했을때
    VOID OnFriendBlock(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
	//친구 목록을 저장할때 친구초대 목록도 같이 저장한다.
	VOID OnAddFriend(STRUCT_FRIEND_INFO& sFriendInfo, STRUCT_FRIEND_DB_INFO& sFriendDBInfo,                      User* pUser);
	//에이전트 서버에 친구추천 목록을 전달한다.
	VOID SendInvitateFriendList(User* pUser, DWORD ServerType);
	//친구 상태 코드를 반환한다.
	BYTE GetFriendSts(User* pUser, DWORD FriendGUID);	         
protected:
private:
    FriendInvitationListProc ListProc;
    FriendInvitationLevelChangedProc LevelChangedProc;

    //친구 관계를 해제 한다.
    VOID InviateFriendRemove(CHARGUID CharacterGUIDFrom, CHARGUID CharacterGUIDTo);
};
