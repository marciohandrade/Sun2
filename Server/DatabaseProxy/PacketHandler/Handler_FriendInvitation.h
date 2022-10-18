#pragma once
#ifndef DBPROXY_HANDLER_FRIEND_INVITATION_H
#define DBPROXY_HANDLER_FRIEND_INVITATION_H

// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

작성자: 문상현
제작일: 2009-10-28

간단한설명: 썬의 친구 초대 이벤트를 지원하기 위한 컨트롤러 클래스 (디비 PROXY 서버용)

*/

#include "PacketStruct_DW.h"
#include "../Common/FriendInvitation/FriendInvitationController.h"

class DFriendInvitationController : public util::Singleton<DFriendInvitationController>, 
                                    public FriendInvitationController
{
public:
	DFriendInvitationController(void);
	~DFriendInvitationController(void);	

    //캐릭터 생성시
    VOID OnCharacterCreateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    
    //보상목록 리스트 반환
    VOID OnRewardItemListSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardItemListSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //보상목록 정보 업데이트 (보상을 받았을 경우)
    VOID OnRewardItemUpdateSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardItemUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //보상목록 추가
    VOID OnRewardInfoInsertCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRewardInfoInsertDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

    //소환 시간 정보 반환
    VOID OnRecallInfoSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRecallInfoSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    //소환한 시간 정보 변경
    VOID OnRecallInfoUpdateCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRecallInfoUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    
    //친구추천관계 추가
    VOID OnAddInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

    //친구 추천관계 해제
    VOID OnRemoveInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    VOID OnRemoveInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);

	//친구 삭제 쿼리를 만든다.
	VOID GetFriendDelQuery(char szQuery[], MSG_DW_FRIEND_DEL_SYN * pRecvMsg);    
protected:
private:    
    
};

#endif //DBPROXY_HANDLER_FRIEND_INVITATION_H