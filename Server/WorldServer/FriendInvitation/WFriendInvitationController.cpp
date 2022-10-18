#include "StdAfx.h"
#include ".\wfriendinvitationcontroller.h"

#include "ServerSession.h"
#include "ServerSessionManager.h"
#include "UserManager.h"
#include "PacketStruct_DW.h"
#include "PacketStruct_AW.h"
#include "Handler_DW.h"
#include "Handler_CW.h"

WFriendInvitationController::WFriendInvitationController(void)
{
    
}

WFriendInvitationController::~WFriendInvitationController(void)
{
    
}

VOID WFriendInvitationController::OnAddInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                          WORD wSize)
{
    MSG_AW_FRIEND_INVITATIONR_INSERT_CMD* msg = (MSG_AW_FRIEND_INVITATIONR_INSERT_CMD*)pMsg;

    SUNLOG(eDEV_LOG, "******************************************");
    SUNLOG(eDEV_LOG, "OnFriendInvitationInsert Recv (%u)", msg->InvitateInfo.dwGuid);
    SUNLOG(eDEV_LOG, "******************************************");


    User* pUser = UserManager::Instance()->FindUserByCharGuid( msg->InvitateInfo.dwGuid );

    if( pUser )// 내가 온라인 상태라면
    {
        //나에게 친구가 추가됐다고 알려준다.

        MSG_DW_FRIEND_ADD_ACK MsgAdd;
        MsgAdd.m_CharGuidFrom = msg->InvitateInfo.dwGuid;

        MsgAdd.m_ToFriendInfo.dwFriendGuid = msg->InvitateInfo.dwFriendGuid;
        MsgAdd.m_ToFriendInfo.sPacketInfo.bOnline = FALSE;
        MsgAdd.m_ToFriendInfo.sPacketInfo.byChannel = 0;
        MsgAdd.m_ToFriendInfo.sPacketInfo.byClass = msg->InvitateInfo.byClass;
        MsgAdd.m_ToFriendInfo.sPacketInfo.Level = 1;

        ZeroMemory(MsgAdd.m_ToFriendInfo.sPacketInfo.ptszFriendName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH);            
        memcpy( MsgAdd.m_ToFriendInfo.sPacketInfo.ptszFriendName, msg->InvitateInfo.ptszFriendName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );                

        ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
        Handler_DW::OnDW_FRIEND_ADD_ACK( (GameDBProxySession*)pGameDBProxy, (MSG_BASE*)&MsgAdd, sizeof(MsgAdd));      

        pUser->GetFriendMap(User::FRIEND_INVIATE)->AddFriendInfo(&MsgAdd.m_ToFriendInfo);

        //친구 추가 상태인지 알려준다.
        INVITATEFRIENDLIST_INFO InvitateFriendList;
        ListProc.GetInvitateFriendList(User::FRIEND_INVIATE, pUser, InvitateFriendList, msg->InvitateInfo.dwFriendGuid);	

        ListProc.SendInvitateFriendList(pUser, InvitateFriendList, INVITATEFRIENDLIST_INFO::FRIENDLIST_ADD, SEND_BOTH);            

        //쪽지가왔다고 알려준다. (다시 요청한다.)
        MSG_CW_MEMO_LIST_REQ msgMemo;
        Handler_CW::OnCW_MEMO_LIST_REQ(pUser, (MSG_BASE*)&msgMemo, sizeof(msgMemo));

    }
}

VOID WFriendInvitationController::OnCharacterLevelUp(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                     WORD wSize)
{
    if(!WFriendInvitationController::Instance()->CheckFriendInvitationEvent())
    {
        //SUNLOG( eCRITICAL_LOG, "Friend Invitated Event Ended !!!");
        return;
    }

    MSG_AW_USER_LEVELUP_CMD* pRecvMsg = (MSG_AW_USER_LEVELUP_CMD*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );

    if( !pUser )
    {
        SUNLOG( eCRITICAL_LOG, "CMD_AW_USER_LEVELUP_CMD Error - No Levelup User[CharGuid:%u]", pRecvMsg->m_CharGuid );
        return;
    }

    switch(pRecvMsg->m_Level) //레벨 20, 40, 60, 80 달성시 선물 증정
    {
    case eINVITATION_REWARDSECTION_1:
    case eINVITATION_REWARDSECTION_2:
    case eINVITATION_REWARDSECTION_3:
    case eINVITATION_REWARDSECTION_4:
        LevelChangedProc.ChangedEventLevel(pUser, pRecvMsg->m_Level);
        break;
    }
}

VOID WFriendInvitationController::OnFriendRemove(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    if(!WFriendInvitationController::Instance()->CheckFriendInvitationEvent())
    {
        SUNLOG( eCRITICAL_LOG, "Friend Invitated Event Ended !!!");
        return;
    }

    MSG_DW_FRIEND_DEL_ACK*	pRecvMsg	= (MSG_DW_FRIEND_DEL_ACK*)pMsg;

    InviateFriendRemove(pRecvMsg->m_CharGuidFrom, pRecvMsg->m_CharGuidTo);

    /*
    User*					pUser		= UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        SUNLOG( eFULL_LOG, "[CharGuid:%u] FRIEND_DEL_ACK - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap()->FindFriend( pRecvMsg->m_CharGuidTo );

    if( !pFriendInfo )
    {
        SUNLOG( eCRITICAL_LOG, "[%s/%u] FRIEND_DEL Error - Friend Already Deleted", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidTo );
        return;
    }

    ListProc.RemoveInvitateFriend(User::FRIEND_INVIATE, pUser, pFriendInfo->dwFriendGuid);
    ListProc.RemoveInvitateFriend(User::FRIEND_BEINVIATED, pUser, pFriendInfo->dwFriendGuid);

    User *pFriendUser = UserManager::Instance()->FindUserByCharGuid( pFriendInfo->dwFriendGuid );
    if( pFriendUser ) 
    {
        ListProc.RemoveInvitateFriend(User::FRIEND_INVIATE, pFriendUser, pUser->GetCharGuid());
        ListProc.RemoveInvitateFriend(User::FRIEND_BEINVIATED, pFriendUser, pUser->GetCharGuid());        
    }
    */
}

VOID WFriendInvitationController::OnFriendBlock(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    if(!WFriendInvitationController::Instance()->CheckFriendInvitationEvent())
    {
        SUNLOG( eCRITICAL_LOG, "Friend Invitated Event Ended !!!");
        return;
    }

    MSG_DW_FRIEND_BLOCK_ACK*	pRecvMsg	= (MSG_DW_FRIEND_BLOCK_ACK*)pMsg;
    

    InviateFriendRemove(pRecvMsg->m_CharGuidFrom, pRecvMsg->m_BlockInfo.dwBlockGuid);
}

VOID WFriendInvitationController::InviateFriendRemove(CHARGUID CharacterGUIDFrom, CHARGUID CharacterGUIDTo)
{
    User*					pUser		= UserManager::Instance()->FindUserByCharGuid( CharacterGUIDFrom );

    if( !pUser )
    {
        SUNLOG( eFULL_LOG, "[CharGuid:%u] InviateFriendRemove - Already Logout", CharacterGUIDFrom );
        return;
    }

    STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap()->FindFriend( CharacterGUIDTo );

    if( !pFriendInfo )
    {
        SUNLOG( eCRITICAL_LOG, "[%s/%u] InviateFriendRemove Error - Friend Already Deleted", pUser->GetCharName().c_str(), CharacterGUIDTo );
        return;
    }

    ListProc.RemoveInvitateFriend(User::FRIEND_INVIATE, pUser, pFriendInfo->dwFriendGuid);
    ListProc.RemoveInvitateFriend(User::FRIEND_BEINVIATED, pUser, pFriendInfo->dwFriendGuid);

    User *pFriendUser = UserManager::Instance()->FindUserByCharGuid( pFriendInfo->dwFriendGuid );
    if( pFriendUser ) 
    {
        ListProc.RemoveInvitateFriend(User::FRIEND_INVIATE, pFriendUser, pUser->GetCharGuid());
        ListProc.RemoveInvitateFriend(User::FRIEND_BEINVIATED, pFriendUser, pUser->GetCharGuid());        
    }	
}

//디비에서 친구 목록 받기 성공
void WFriendInvitationController::OnAddFriend(STRUCT_FRIEND_INFO& sFriendInfo, STRUCT_FRIEND_DB_INFO& sFriendDBInfo, User* pUser)
{	
	CHECK_FRIENDINVITATION_EVENT_RETRUN;	

	if(sFriendDBInfo.Sts == STRUCT_FRIEND_DB_INFO::INFO_INVIAT)
	{
		//내가 추천한 친구가 이벤트 종료 레벨 이상이었는데도 일반 친구가 아니면 디비 정보가 잘못된거야
		if(sFriendDBInfo.Level >= GetEventMaxLevel())
		{			
			SUNLOG( eFULL_LOG, 
				"DW_FRIEND_GETLIST_ACK Error - Friend Invitation Reletion Code fail[InviatCharGuid:%u, BeInviatedCharGuid:%u]", 
				pUser->GetCharGuid(), sFriendDBInfo.dwFriendGuid);
		}
		else
		{            
			pUser->GetFriendMap(User::FRIEND_INVIATE)->AddFriendInfo(&sFriendInfo);
		}
	}
	else if(sFriendDBInfo.Sts == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED)
	{
		//내가 친구 추천을 받았는데 내가 이벤트 종료 레벨 이상이었는데도 일반 친구가 아니면 디비 정보가 잘못된거야
		if(pUser->GetCharLevel() >= GetEventMaxLevel())
		{			
			SUNLOG( eFULL_LOG, 
				"DW_FRIEND_GETLIST_ACK Error - Friend Invitation Reletion Code fail[InviatCharGuid:%u, BeInviatedCharGuid:%u]", 
				sFriendDBInfo.dwFriendGuid, pUser->GetCharGuid());
		}
		else
		{            
			pUser->GetFriendMap(User::FRIEND_BEINVIATED)->AddFriendInfo(&sFriendInfo);
		}
	}
}


BYTE WFriendInvitationController::GetFriendSts(User* pUser, DWORD FriendGUID)
{
	if(NULL != pUser->GetFriendMap(User::FRIEND_INVIATE)->FindFriend(FriendGUID)) return STRUCT_FRIEND_DB_INFO::INFO_INVIAT;
	else if(NULL != pUser->GetFriendMap(User::FRIEND_BEINVIATED)->FindFriend(FriendGUID)) return STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED;
	return STRUCT_FRIEND_DB_INFO::INFO_FRIEND;
}

void WFriendInvitationController::SendInvitateFriendList(User* pUser, DWORD ServerType)
{	
    CHECK_FRIENDINVITATION_EVENT_RETRUN;	

    ListProc.SendInvitateFriendList(User::FRIEND_INVIATE, pUser, INVITATEFRIENDLIST_INFO::FRIENDLIST_INIT, ServerType);
    ListProc.SendInvitateFriendList(User::FRIEND_BEINVIATED, pUser, INVITATEFRIENDLIST_INFO::FRIENDLIST_INIT, ServerType);
}
