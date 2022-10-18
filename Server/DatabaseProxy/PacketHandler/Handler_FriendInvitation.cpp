#include "stdafx.h"
#include "./Handler_FriendInvitation.h"

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
#include "Protocol_AD.h"
#include "PacketStruct_AD.h"
#include "PacketStruct_DG.h"
#include "PacketHandler/Handler_FromWorldServer.h"
#include "SolarDateTime.h"

DFriendInvitationController::DFriendInvitationController(void)
{
}

DFriendInvitationController::~DFriendInvitationController(void)
{
}

VOID DFriendInvitationController::OnCharacterCreateDBR(LPVOID pServerSession, MSG_BASE * pMsg,
                                                       WORD wSize)
{
    if(!DFriendInvitationController::Instance()->CheckFriendInvitationEvent())
    {
        SUNLOG(eDEV_LOG, "Friend Invitated Event Ended !!!");
        return;
    }

    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Char_Create * pQResult = (Query_Char_Create *)pResult->m_pData;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_InvitationFriend_Insert(%u, %u)}" , 
                pQResult->GetUserKey(), pQResult->pResult[0].m_CharGuid);
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    //친구 추천 정보 생성
    Query_Friend_InvitationInsert * pQuery = Query_Friend_InvitationInsert::ALLOC();

    pQuery->SetCharGuid(pQResult->pResult[0].m_CharGuid);
    pQuery->SetUserGuid(pQResult->GetUserKey());
    pQuery->SetCharName(pQResult->pResult[0].result_info_char_name_);
    pQuery->SetClass(pQResult->pResult[0].m_byClass);

    pQuery->SetQuery(szQueryBuff);     
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetIndex( MAKEDWORD( (WORD)AD_EVENT, (WORD)AD_EVENT_INVITATIONRELATION_INSERT_DBR ) );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
}

VOID DFriendInvitationController::OnRewardItemListSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                            WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;	
    Query_Friend_InvitationReward_Select * pQResult = 
                                    (Query_Friend_InvitationReward_Select *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
        }	

        MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK msg;
        msg.CharGUID = pQResult->GetCharGuid();
        msg.ErrorCode = 1;

        ((ChildServerSession*)pServerSession)->SendPacket(&msg, sizeof(msg));
    }	
    else
    {
        MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK msg;

        msg.Count = pQResult->GetResultRowNum();
        msg.CharGUID = pQResult->GetCharGuid();

        for(int i = 0 ; i < pQResult->GetResultRowNum() ; i++)
        {            
            msg.RewardInfo[i].InvitateFriendGuid = pQResult->pResult[i].SendRewardCharGuid;
            msg.RewardInfo[i].Level = pQResult->pResult[i].Level;
            msg.RewardInfo[i].RewardSts = pQResult->pResult[i].RewardSts;
            memcpy( msg.RewardInfo[i].CharName, pQResult->pResult[i].CharName, 
                    sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
            msg.RewardInfo[i].CharName[MAX_CHARNAME_LENGTH] = '\0';
        }        

        ((ChildServerSession*)pServerSession)->SendPacket(&msg, msg.GetSize());                                  
    }

    Query_Friend_InvitationReward_Select::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnRewardItemListSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                            WORD wSize)
{
    
    MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN* pRecvMsg = 
                                            (MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN*)pMsg;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_InvitationFriend_Reward_Select(%u)}", 
                pRecvMsg->CharGUID);
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';


    Query_Friend_InvitationReward_Select * pQuery = Query_Friend_InvitationReward_Select::ALLOC();
    pQuery->SetCharGuid(pRecvMsg->CharGUID);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex( MAKEDWORD( (WORD)AD_EVENT, (WORD)AD_EVENT_INVITATEFRIEND_REWARD_LIST_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
}

VOID DFriendInvitationController::OnRewardItemUpdateSYN(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN* pRecvMsg = 
                                            (MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN*)pMsg;

    DBUser* const pUser = g_DBUserManager.FindUser( pRecvMsg->m_dwKey );

    if(pUser)
    {
        TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
        _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
            "{?=call S_InvitationFriend_Reward_Update(%u,%u,%u)}" , 
            pRecvMsg->CharGUID, pRecvMsg->FriendGUID, pRecvMsg->ReWardType );
        szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

        Query_Friend_InvitationReward_Update* pQuery = Query_Friend_InvitationReward_Update::ALLOC();
        pQuery->SetUserKey(pRecvMsg->m_dwKey);

        pQuery->SetLevel(pRecvMsg->ReWardType);
        pQuery->SetCharGuid(pRecvMsg->CharGUID);
        pQuery->SetFriendGuid(pRecvMsg->FriendGUID);

        pQuery->SetQuery(szQueryBuff);
        pQuery->SetIndex(MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_INVITATEFRIEND_REWARDUPDATE_DBR));
        pQuery->SetVoidObject( pServerSession );
        ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
    }
    else
    {
        MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK msg;
        msg.m_dwErrorCode = MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK::DB_FAIL;
        ((ChildServerSession*)pServerSession)->SendPacket(&msg, sizeof(msg));
    }   
}

VOID DFriendInvitationController::OnRewardItemUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;	
    Query_Friend_InvitationReward_Update * pQResult = 
                                        (Query_Friend_InvitationReward_Update *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {      
        MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK msg;

        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
            msg.m_dwErrorCode = MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK::DB_FAIL;
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
            msg.m_dwErrorCode = MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK::RECVREWARDED;
        }		

        msg.m_dwKey = pQResult->GetUserKey();            
        ((ChildServerSession*)pServerSession)->SendPacket(&msg, sizeof(msg));
    }	
    else
    {
        MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK msg;
        msg.m_dwKey = pQResult->GetUserKey();
        msg.CharGUID = pQResult->GetCharGuid();
        msg.FriendGUID = pQResult->GetFreindGuid();
        msg.ReWardType = pQResult->GetLevel();
        ((ChildServerSession*)pServerSession)->SendPacket(&msg, sizeof(msg));      
    }

    Query_Friend_InvitationReward_Update::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnRewardInfoInsertCMD(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    SUNLOG(eDEV_LOG, "Rev: DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD");

    MSG_DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD* pRecvMsg = 
                                        (MSG_DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD *)pMsg;    

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
                "{?=call S_InvitationFriend_Reward_Insert(%u,%u,%u)}" , 
                pRecvMsg->m_SendGuid, pRecvMsg->m_RevGuid, pRecvMsg->m_Level );
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Friend_InvitationReward_Insert* pQuery = Query_Friend_InvitationReward_Insert::ALLOC();
    pQuery->SetRecvGuid(pRecvMsg->m_RevGuid);
    pQuery->SetSendGuid(pRecvMsg->m_SendGuid);
    pQuery->SetKey(pRecvMsg->m_dwKey);
    pQuery->SetLevel(pRecvMsg->m_Level);
    pQuery->SetCharName(pRecvMsg->m_ptszRecvName);
    pQuery->SetMemo(pRecvMsg->m_ptszMemo);
    pQuery->SetSession(pServerSession);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_INVITATIONRELATION_REWARDINSERT_DBR));
    pQuery->SetVoidObject( pServerSession );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
}

VOID DFriendInvitationController::OnRewardInfoInsertDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;	
    Query_Friend_InvitationReward_Insert * pQResult = 
                                        (Query_Friend_InvitationReward_Insert *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
        }		
    }	
    else
    {
        MSG_DW_MEMO_SEND_SYN MemoMsg;

        MemoMsg.m_dwKey     = 0;
        MemoMsg.m_SendGuid  = 0;
        // _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
        MemoMsg.m_typeSystemMemo = NORMAL_MEMO;
        MemoMsg.m_recvItemCode = 0;
        MemoMsg.m_amountCost = 0;
        MemoMsg.m_recvGuid = 0;
        // }
        _tcsncpy( MemoMsg.m_ptszRecvName, pQResult->GetCharName(), MAX_CHARNAME_LENGTH );
        MemoMsg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';

        _tcsncpy( MemoMsg.m_ptszMemo, pQResult->GetMemo(), MAX_MEMO_LENGTH );
        MemoMsg.m_ptszRecvName[MAX_MEMO_LENGTH] = '\0';

        Handler_FromWorldServer::OnDW_MEMO_SEND_SYN((ChildServerSession*)pQResult->GetSession(), 
                                            (MSG_BASE*)&MemoMsg, sizeof(MSG_DW_MEMO_SEND_SYN)); 
        //보상 아이템 습득 쪽지를 전송한다.0
    }

    Query_Friend_InvitationReward_Insert::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnRecallInfoSelectSYN(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN * pResult = 
                                            (MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN *)pMsg;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_GetRecallTime(%u)}" , 
                pResult->CharGUID);
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Friend_Recall_Select * pQuery = Query_Friend_Recall_Select::ALLOC();

    pQuery->SetCharGuid(pResult->CharGUID);        

    pQuery->SetUserKey(pResult->m_dwKey);
    pQuery->SetQuery(szQueryBuff);     
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_INVITATEFRIEND_RECALLSELECT_DBR ) );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );  
}

VOID DFriendInvitationController::OnRecallInfoSelectDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Friend_Recall_Select * pQResult = (Query_Friend_Recall_Select *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                    pQResult->pParam[0].m_Result);
        }	

        MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK msg;

        msg.m_dwKey = pQResult->GetUserKey();
        msg.CharGUID = pQResult->GetCharGuid();

        ((ChildServerSession*)pServerSession)->SendPacket(&msg, sizeof(msg));

    }	
    else
    {
        MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK msg;        

        msg.Count = pQResult->GetResultRowNum();
        msg.m_dwKey = pQResult->GetUserKey();
        msg.CharGUID = pQResult->GetCharGuid();

        for(int i = 0 ; i < pQResult->GetResultRowNum() ; i++)
        {
            msg.m_Recall_Info[i].FriendGUID = pQResult->pResult[i].FriendGUID;
            memcpy( msg.m_Recall_Info[i].tszRecallTime, pQResult->pResult[i].tszRecallTime, 
                    sizeof(TCHAR)*MAX_MIDDATETIME_SIZE );
            msg.m_Recall_Info[i].tszRecallTime[MAX_MIDDATETIME_SIZE] = '\0';
        }        

        if(msg.Count > 0)
        {
            ((ChildServerSession*)pServerSession)->SendPacket(&msg, msg.GetSize());
        }        
    }

    Query_Friend_Recall_Select::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnRecallInfoUpdateCMD(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD * pResult = 
                                             (MSG_DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD *)pMsg;

    SYSTEMTIME RecallTime;
    util::YYYYMMDDHHMMSSToSYSTEMTIME(pResult->RecallTime, RecallTime);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
                "{?=call S_CharRelation_SetRecallTime(%u, %u, \'%u-%u-%u %u:%u:%u\')}" , 
                pResult->CharGUID, pResult->FriendGUID,
                RecallTime.wYear, RecallTime.wMonth, RecallTime.wDay, RecallTime.wHour, 
                RecallTime.wMinute, 
                RecallTime.wSecond);
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Friend_Recall_Update * pQuery = Query_Friend_Recall_Update::ALLOC();

    pQuery->SetQuery(szQueryBuff);     
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_INVITATEFRIEND_RECALLUPDATE_DBR ) );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
}

VOID DFriendInvitationController::OnRecallInfoUpdateDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                        WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Friend_Recall_Update * pQResult = (Query_Friend_Recall_Update *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
        } 
    }	

    Query_Friend_Recall_Update::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnAddInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                          WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Friend_InvitationInsert * pQResult = (Query_Friend_InvitationInsert *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
        }	
    }	
    else
    {
        //초대한 유저의 캐릭터 GUID 목록을 받아서 월드서버에 보낸다.
        //월드서버에는 현재 로그인 했는지 체크 하고 로그인 되어있다면 클라이언트에게 알린다.
        MSG_AD_EVENT_INVITATIONR_INSERT_CMD msg;

        msg.m_CharGuidCount = pQResult->GetResultRowNum();
      

        for( int i = 0 ; i < msg.m_CharGuidCount ; i++)
        {
            msg.InvitateInfo[i].dwGuid = pQResult->pResult[i].m_FriendCharGuid;
            msg.InvitateInfo[i].byClass = pQResult->GetClass();
            msg.InvitateInfo[i].dwFriendGuid = pQResult->GetCharGuid();
            // NOTE: copy not null terminated -> null terminated buffer
            strncpy(msg.InvitateInfo[i].ptszFriendName, pQResult->GetCharName(),
                    _countof(msg.InvitateInfo[i].ptszFriendName));
            msg.InvitateInfo[i].ptszFriendName[
                _countof(msg.InvitateInfo[i].ptszFriendName) - 1] = '\0';
        }

        if(msg.m_CharGuidCount > 0)
        {
            ((ChildServerSession*)pServerSession)->SendPacket( &msg, msg.GetSize());
            if(((ChildServerSession*)pServerSession)->GetServerType() == AGENT_SERVER)
            {
                
            }
        }
    }

    Query_Friend_InvitationInsert::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::OnRemoveInvitatedFriendCMD(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                             WORD wSize)
{
    
    MSG_DW_FRIEND_INVITATIONRELATION_FREE_CMD* pRecvMsg = 
                                                (MSG_DW_FRIEND_INVITATIONRELATION_FREE_CMD *)pMsg;        

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
                "{?=call S_CharRelation_CancelInvitationFriendAll(%u)}" , pRecvMsg->m_CharGuid );
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Friend_InvitationFree * pQuery = Query_Friend_InvitationFree::ALLOC();
    pQuery->SetCharGuid(pRecvMsg->m_CharGuid);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_INVITATIONRELATION_FREE_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    ((ChildServerSession*)pServerSession)->DBQuery( pQuery );
}

VOID DFriendInvitationController::OnRemoveInvitatedFriendDBR(LPVOID pServerSession, MSG_BASE * pMsg, 
                                                             WORD wSize)
{
    
    MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;	
    Query_Friend_InvitationFree * pQResult		= (Query_Friend_InvitationFree *)pResult->m_pData;

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), 
                   pQResult->pParam[0].m_Result);
        }		
    }	

    Query_Friend_InvitationFree::FREE(pQResult); pQResult = NULL;
}

VOID DFriendInvitationController::GetFriendDelQuery(char szQuery[], MSG_DW_FRIEND_DEL_SYN * pRecvMsg)
{
	if(STRUCT_FRIEND_DB_INFO::INFO_INVIAT == pRecvMsg->m_Sts 
        || STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED == pRecvMsg->m_Sts)
	{
		_sntprintf( szQuery, MAX_QUERY_LENGTH_128, 
            "{?=call S_CharRelation_CancelInvitationFriend(%d,%d,%d)}" , 
            pRecvMsg->m_CharGuidFrom, pRecvMsg->m_CharGuidTo, pRecvMsg->m_Sts );	
	}
	else
	{
		_sntprintf( szQuery, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Cancel(%d,%d,1)}" , 
            pRecvMsg->m_CharGuidFrom, pRecvMsg->m_CharGuidTo );	
	}	
}
