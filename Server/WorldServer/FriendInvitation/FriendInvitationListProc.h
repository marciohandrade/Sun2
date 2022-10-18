#pragma once

/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: ģ�� ��õ ���� ��ϵ��� ó���Ѵ�.

*/

#include "../FriendInvitation/NetworkSession/NetworkSessionFactory.hpp"

class FriendInvitationListProc
{
public:
    FriendInvitationListProc(void);
    ~FriendInvitationListProc(void);

public:
    //������ ���� ��� ģ�� ��õ���踦 �Ϲ� ģ�� ����� �����Ѵ�.
    BOOL IsSetFriendsNormalSts(User* pUser);	
    //ģ����õ ����Ʈ���� �����ϰ� ����� ������ �����Ѵ�.
    VOID RemoveInvitateFriend(User::FRIEND_TYPE type, User* pUser, DWORD FriendGUID);    
    //������Ʈ ������ ģ����õ ����� �����Ѵ�. (W->A->G)
    void SendInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType);
    void SendInvitateFriendList(User* pUser, INVITATEFRIENDLIST_INFO& FriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType);
    //������ ��Ŷ�� �����.
    VOID GetInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO& FriendList);
    VOID GetInvitateFriendList(User::FRIEND_TYPE type, User* pUser, INVITATEFRIENDLIST_INFO& FriendList, DWORD FriendGUID);

private:
    NetWorkSessionFactory m_NetWorkSessionFactory;
};

