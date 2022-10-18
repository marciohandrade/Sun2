#pragma once

/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: 친구 추천 관련 목록들을 처리한다.

*/

#include "../FriendInvitation/NetworkSession/NetworkSessionFactory.hpp"

class FriendInvitationListProc
{
public:
    FriendInvitationListProc(void);
    ~FriendInvitationListProc(void);

public:
    //나한테 속한 모든 친구 추천관계를 일반 친구 관계로 변경한다.
    BOOL IsSetFriendsNormalSts(User* pUser);	
    //친구추천 리스트에서 삭제하고 결과를 서버에 전송한다.
    VOID RemoveInvitateFriend(User::FRIEND_TYPE type, User* pUser, DWORD FriendGUID);    
    //에이전트 서버에 친구추천 목록을 전달한다. (W->A->G)
    void SendInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType);
    void SendInvitateFriendList(User* pUser, INVITATEFRIENDLIST_INFO& FriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType);
    //전송할 패킷을 만든다.
    VOID GetInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO& FriendList);
    VOID GetInvitateFriendList(User::FRIEND_TYPE type, User* pUser, INVITATEFRIENDLIST_INFO& FriendList, DWORD FriendGUID);

private:
    NetWorkSessionFactory m_NetWorkSessionFactory;
};

