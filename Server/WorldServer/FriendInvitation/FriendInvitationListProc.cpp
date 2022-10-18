#include "StdAfx.h"
#include ".\friendinvitationlistproc.h"

#include "../UserManager.h"
#include "../User.h"


FriendInvitationListProc::FriendInvitationListProc(void)
{
}

FriendInvitationListProc::~FriendInvitationListProc(void)
{
}

BOOL FriendInvitationListProc::IsSetFriendsNormalSts(User* pUser)
{
    //내가 추천받은 친구이면 나를 추천한 모든 캐릭터의 친구 관계 코드를 일반 친구로 변경한다.
    STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetFirstFriend();
    if(NULL == pFriendInfo) return FALSE;

    do 
    {	
        User *pFriendUser = UserManager::Instance()->FindUserByCharGuid( pFriendInfo->dwFriendGuid ); //나를 추천한 친구 객체에서 목록 제거
        if(pFriendUser) 
        {
            RemoveInvitateFriend(User::FRIEND_INVIATE, pFriendUser, pUser->GetCharGuid());
        }		
    } while( pFriendInfo = pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetNextFriend() );

    SendInvitateFriendList(User::FRIEND_BEINVIATED, pUser, INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL, SEND_BOTH); //나를 추천한 친구 목록 제거를 서버에 알린다.
    pUser->GetFriendMap(User::FRIEND_BEINVIATED)->Clear(); //내 객체에서 나를 추천한 친구 목록 제거

    return TRUE;
}

VOID FriendInvitationListProc::RemoveInvitateFriend(User::FRIEND_TYPE type, User* pUser, DWORD FriendGUID)
{
    if(pUser->GetFriendMap(type)->RemoveFriend(FriendGUID))//목록에 있어서 삭제했다.
    {
        //개발시 필요한 로그 삭제해야한다.
        SUNLOG( eDEV_LOG, "FRIEND_NORMAL Count[%u]", pUser->GetFriendMap(User::FRIEND_NORMAL)->GetFriendNum() );
        SUNLOG( eDEV_LOG, "FRIEND_INVIATE Count[%u]", pUser->GetFriendMap(User::FRIEND_INVIATE)->GetFriendNum() );
        SUNLOG( eDEV_LOG, "FRIEND_BEINVIATED Count[%u]", pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetFriendNum() );

        INVITATEFRIENDLIST_INFO InvitateFriendList;
        GetInvitateFriendList(type, pUser, InvitateFriendList, FriendGUID);	

        m_NetWorkSessionFactory.GetNetWorkSession(NetWorkSessionFactory::AGENTSVR)->SendPacket(pUser, InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL);
        m_NetWorkSessionFactory.GetNetWorkSession(NetWorkSessionFactory::CLIENT)->SendPacket(pUser, InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL);        
    }
}

void FriendInvitationListProc::SendInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType)
{	    
    INVITATEFRIENDLIST_INFO InvitateFriendList;
    GetInvitateFriendList(SetType, pUser, InvitateFriendList);	

    SendInvitateFriendList(pUser, InvitateFriendList, action , ServerType);
}

void FriendInvitationListProc::SendInvitateFriendList(User* pUser, INVITATEFRIENDLIST_INFO& FriendList, INVITATEFRIENDLIST_INFO::FRIEND_ACTION action , DWORD ServerType)
{
    if(ServerType & SEND_AGENTSERVER)
    {
        m_NetWorkSessionFactory.GetNetWorkSession(NetWorkSessionFactory::AGENTSVR)->SendPacket(pUser, FriendList, action);        
    }

    if(ServerType & SEND_CLIENT)
    {
        m_NetWorkSessionFactory.GetNetWorkSession(NetWorkSessionFactory::CLIENT)->SendPacket(pUser, FriendList, action);        
    }    
    //개발시 필요한 로그 삭제해야한다.
    SUNLOG( eDEV_LOG, "FRIEND_NORMAL Count[%u]", pUser->GetFriendMap(User::FRIEND_NORMAL)->GetFriendNum() );
    SUNLOG( eDEV_LOG, "FRIEND_INVIATE Count[%u]", pUser->GetFriendMap(User::FRIEND_INVIATE)->GetFriendNum() );
    SUNLOG( eDEV_LOG, "FRIEND_BEINVIATED Count[%u]", pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetFriendNum() );
}

VOID FriendInvitationListProc::GetInvitateFriendList(User::FRIEND_TYPE SetType, User* pUser, INVITATEFRIENDLIST_INFO& FriendList)
{    
    FriendList.m_dwCharGuid = pUser->GetCharGuid();
    FriendList.m_byListCount = 0;
    FriendList.m_FriendSts = SetType;

    // (NOTE) protocol specifiaction rule
    BOOST_STATIC_ASSERT(_countof(FriendList.m_GuidList) == MAX_FRIEND_STAT_NUM);
    // (CHANGES) (f100624.2L) prevent a stack overflow possibility
    FriendMap* friends = pUser->GetFriendMap(SetType);
    DWORD* guid_it = FriendList.m_GuidList;
    int count = 0;
    for (STRUCT_FRIEND_INFO* friend_info = friends->GetFirstFriend();
         count < _countof(FriendList.m_GuidList) && friend_info != NULL;
         ++count, ++guid_it)
    {
        *guid_it = friend_info->dwFriendGuid;
        // 3rd increment
        friend_info = friends->GetNextFriend();
    }
    FriendList.m_byListCount = static_cast<BYTE>(count);
#if SUN_CODE_BACKUP
    STRUCT_FRIEND_INFO* pFriend = pUser->GetFriendMap(SetType)->GetFirstFriend();

    while(pFriend)
    {
        FriendList.m_GuidList[FriendList.m_byListCount++] = pFriend->dwFriendGuid;

        pFriend = pUser->GetFriendMap(SetType)->GetNextFriend();
    }
#endif
}

VOID FriendInvitationListProc::GetInvitateFriendList(User::FRIEND_TYPE type, User* pUser, INVITATEFRIENDLIST_INFO& FriendList, DWORD FriendGUID)
{    
    FriendList.m_dwCharGuid = pUser->GetCharGuid();
    FriendList.m_byListCount = 1;
    FriendList.m_FriendSts = type;

    FriendList.m_GuidList[0] = FriendGUID;
}
