#include "stdafx.h"
#include "./GFriendInvitationController.h"

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND

#include "./GameParty/GamePartyOperator.h"
#include "./GameParty/NewGamePartyManager.h"
#include "ItemManager.h"
#include "PacketStruct_DG.h"
#include "SolarDateTime.h"

GFriendInvitationController::GFriendInvitationController(void)
{
}

GFriendInvitationController::~GFriendInvitationController(void)
{
}

VOID GFriendInvitationController::OnInvitatedFriendListSelectCMD(LPVOID , MSG_BASE * pMsg, WORD)
{
    MSG_AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD* pRecvMsg = 
                                            (MSG_AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD*)pMsg;

    Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );

    SUNLOG( eDEV_LOG, "Rev SendInvitateFriendList Info - GameServer Count[%u] GUID[%u]", 
            pRecvMsg->m_InvitateFriendListInfo.m_byListCount , pRecvMsg->m_dwKey );

    if( !pPlayer )
    {
        MessageOut( eCRITICAL_LOG, "Not Found User: %u",
                    pRecvMsg->m_InvitateFriendListInfo.m_dwCharGuid );
        return;
    }

    InvitateFriendInfo::FRIEND_TYPE type = 
            (InvitateFriendInfo::FRIEND_TYPE)(BYTE)pRecvMsg->m_InvitateFriendListInfo.m_FriendSts;        

    switch(pRecvMsg->m_InvitateFriendListInfo.m_FreindAction)
    {
    case INVITATEFRIENDLIST_INFO::FRIENDLIST_INIT:
        {            
            pPlayer->GetInvitateFriend().ClearFriendList(type);
            pPlayer->GetInvitateFriend().AddFriend(type, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_GuidList, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_byListCount);
            //리콜타임 얻기            
            RecallProc.SendRecallTimeSelect(pRecvMsg->m_InvitateFriendListInfo.m_dwCharGuid, pPlayer);
        }
        break;
    case INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL:
        {
            for(int i = 0 ; i < pRecvMsg->m_InvitateFriendListInfo.m_byListCount; i++)
            {
                pPlayer->GetInvitateFriend().RemoveFriend(type, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_GuidList[i]);
                //파티내에 친구추천 관계가 있나 ?
                if(pPlayer->GetInvitateFriend().IsInvitateFriendParty()) 
                {
                    pPlayer->GetInvitateFriend().RemoveFriend(
                                                InvitateFriendInfo::FRIEND_INVIATEINPARTY, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_GuidList[i]);
                }
            }                
        }
        break;
    case INVITATEFRIENDLIST_INFO::FRIENDLIST_ADD:
        {
            pPlayer->GetInvitateFriend().AddFriend(type, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_GuidList, 
                                                pRecvMsg->m_InvitateFriendListInfo.m_byListCount);
        }
        break;
    }

    MessageOut( eDEV_LOG, "FRIEND_INVIATEINPARTY Count[%u]", 
                pPlayer->GetInvitateFriend().GetSize(InvitateFriendInfo::FRIEND_INVIATEINPARTY) );
    MessageOut( eDEV_LOG, "FRIEND_INVIATE Count[%u]", 
                pPlayer->GetInvitateFriend().GetSize(InvitateFriendInfo::FRIEND_INVIATE) );
    MessageOut( eDEV_LOG, "FRIEND_BEINVIATED Count[%u]", 
                pPlayer->GetInvitateFriend().GetSize(InvitateFriendInfo::FRIEND_BEINVIATED) );
}

VOID GFriendInvitationController::OnRecallQuestionACK(LPVOID , MSG_BASE * pMsg, WORD)
{
    const MSG_CG_EVENT_INVITATEFRIEND_RECALL_SYN* pRecvMsg = 
                                                    (MSG_CG_EVENT_INVITATEFRIEND_RECALL_SYN*)pMsg;

    Player* pRecallPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pRecallPlayer );
    if( !pRecallPlayer )
    {
        MessageOut( eCRITICAL_LOG, "CG_EVENT_INVITATEFRIEND_RECALL_SYN Not Found Recall User: %u", 
                    pRecvMsg->m_dwKey );
        return;
    } 

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->FriendGUID );
    ASSERT( pPlayer );
    if( !pPlayer )
    {
        MessageOut( eCRITICAL_LOG, "CG_EVENT_INVITATEFRIEND_RECALL_SYN Not Found User: %u", 
                    pRecvMsg->FriendGUID );
        return;
    }       

    RecallProc.ReCallUser(pPlayer, pRecallPlayer);
}

VOID GFriendInvitationController::OnRecallQuestionNAK(LPVOID , MSG_BASE * pMsg, WORD)
{
    SUNLOG( eCRITICAL_LOG, "CMD_CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK" );  

    const MSG_CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK* pRecvMsg = 
                                            (MSG_CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->FriendGUID );
    ASSERT( pPlayer );
    if( !pPlayer )
    {
        MessageOut( eCRITICAL_LOG, 
                    "OnCG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK Not Found User: %u", 
                    pMsg->m_dwKey );
        return;
    }

    RecallProc.SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::FRIEND_REJECT);
}

VOID GFriendInvitationController::OnRecallSYN(LPVOID , MSG_BASE * pMsg, WORD)
{
    SUNLOG( eDEV_LOG, "CMD_CG_EVENT_INVITATEFRIEND_RECALL_SYN" );  

    const MSG_CG_EVENT_INVITATEFRIEND_RECALL_SYN* pRecvMsg = 
                                                (MSG_CG_EVENT_INVITATEFRIEND_RECALL_SYN*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer )
    {
        MessageOut( eCRITICAL_LOG, "CG_EVENT_INVITATEFRIEND_RECALL_SYN Not Found User: %u", 
                    pMsg->m_dwKey );
        return;
    }      

    //버그수정:5847 문상현 필드 서버 이외에서는 소환을 금지한다.
    if (g_pGameServer->GetServerType() != FIELD_SERVER)
    {
        RecallProc.SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERZONE_ME);
        return;
    }

	//소환을 요청한 친구가 나와 친구초대 관계인지 확인한다.
	if(!pPlayer->GetInvitateFriend().IsInvitateFriend(pRecvMsg->FriendGUID))
	{
		RecallProc.SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::NOTFOUND_USER);
		return;
	}

    Player* pRecallPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->FriendGUID );
    ASSERT( pRecallPlayer );
    if( !pRecallPlayer )
    {
        RecallProc.SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::NOTFOUND_USER);
        return;
    }   

    if(!GFriendInvitationController::Instance()->CheckFriendInvitationEvent())
    {
        RecallProc.SendRecallFailMsg(pPlayer, 
                                     MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::TERMINATE_EVENT);
        return;
    }    

    //소환한지 1시간 됐는지 확인
    if(!RecallProc.IsRecalltimeExpired(pPlayer, pRecallPlayer->GetCharGuid()))
    {
        return;
    }

    //같은 소환 요청이 있었는지 확인
    //버그수정:5890 문상현 2010-02-05 [모든국가공통] 친구 소환 요청 메시지가 
    //화면에 출력된 상태에서는 더이상 소환 요청을 보내지 않는다.
    if(RecallProc.IsRequestRecall(pPlayer, pRecallPlayer->GetCharGuid()))
    {
        RecallProc.SendRecallFailMsg(pPlayer, 
                                     MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERREQUESTEXIST);
    }
    else
    {
        RecallProc.SendRecallQuestionMsg(pPlayer, pRecallPlayer);
    }
}

VOID GFriendInvitationController::OnRecallInfoSelectACK(LPVOID , MSG_BASE * pMsg, WORD)
{
    MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK* pRecvMsg = 
                                            (MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if(!pPlayer)
    {
        MessageOut( eCRITICAL_LOG, 
                    "CMD_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK Not Found User Count[%u]", 
                    pRecvMsg->m_dwKey );
    }
    else
    {
        pPlayer->GetInvitateFriend().InitReCallTime(pRecvMsg);        
    }
}

VOID GFriendInvitationController::OnRecallInfoSelectNAK(LPVOID , MSG_BASE * pMsg, WORD)
{
    SUNLOG( eCRITICAL_LOG, "[CMD_DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK" );
}

VOID GFriendInvitationController::OnRewardItemUpdateACK(LPVOID , MSG_BASE * pMsg, WORD)
{
    MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK* pRecvMsg = 
                                            (MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if(!pPlayer) 
    {
        MessageOut( eCRITICAL_LOG, "DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK Not Found User: %u", 
                    pRecvMsg->m_dwKey );
        return;
    }

    if( !pPlayer->GetField() )
    {
        SUNLOG( eCRITICAL_LOG, " Not Exist GameField (OnRewardItemUpdateACK)");
        return;
    }

    ReWardProc.ReceiveRewardItem(pPlayer, pRecvMsg->ReWardType);
}

VOID GFriendInvitationController::OnRewardItemUpdateNAK(LPVOID , MSG_BASE * pMsg, WORD)
{
    MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK* pRecvMsg = 
                                            (MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if(!pPlayer)
    {
        MessageOut( eCRITICAL_LOG, "DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK Not Found User: %u", 
                    pRecvMsg->m_dwKey );
        return;
    }

    ReWardProc.SendRewardFailMsg(pPlayer, pRecvMsg->m_dwErrorCode);
}

VOID GFriendInvitationController::OnRewardSYN(LPVOID , MSG_BASE * pMsg, WORD)
{
    SUNLOG( eDEV_LOG, "OnCG_EVENT_INVITATEFRIEND_REWARD_SYN" );

    MSG_CG_EVENT_INVITATEFRIEND_REWARD_SYN * pRecvMsg = 
                                                    (MSG_CG_EVENT_INVITATEFRIEND_REWARD_SYN *)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer ) 
    {
        MessageOut( eCRITICAL_LOG, "OnRewardSYN Not Found User: %u", 
            pRecvMsg->m_dwKey );
        return ;
    }

    if( !pPlayer->GetField() )
    {
        SUNLOG( eCRITICAL_LOG, " Not Exist GameField (OnRewardSYN)" );
        return;
    }

    ReWardProc.ReQuestReWardItem(pPlayer, pRecvMsg->FriendGUID, pRecvMsg->ReWardType);
}


VOID GFriendInvitationController::OnParty_Join(WORD PartyKey, DWORD MemberKey)
{
    CHECK_FRIENDINVITATION_EVENT_RETRUN;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(MemberKey);
    ASSERT(pPlayer); 
    if( !pPlayer ) 
    {
        MessageOut( eCRITICAL_LOG, "OnRewardSYN Not Found User: %u", 
            MemberKey );
        return;
    }
 
    static CheckJoinInvitateFriendInParty Opr;
    Opr.Init(pPlayer);

    NewGamePartyManager::Instance()->For_eachMembers(PartyKey, Opr);
}

VOID GFriendInvitationController::OnParty_Leave(WORD PartyKey, DWORD MemberKey)
{
    CHECK_FRIENDINVITATION_EVENT_RETRUN;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(MemberKey);
    // 파티원이 비정상적인 접속 종료로 끊기는 경우 pPlayer가 NULL일 수 있음
    if( !pPlayer ) {
        return;
    }
    
    static CheckLeaveInvitateFriendInParty Opr;
    Opr.Init(pPlayer);

    NewGamePartyManager::Instance()->For_eachMembers(PartyKey, Opr);
    
}

VOID GFriendInvitationController::OnAGRecallAck(LPVOID , MSG_BASE * pMsg, WORD )
{
    const MSG_AG_EVENT_RECALL_ACK* pRecvMsg = (const MSG_AG_EVENT_RECALL_ACK*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if(!pPlayer)
    {
        MessageOut( eCRITICAL_LOG, "AG_EVENT_RECALL_ACK Not Found User [%u]", pRecvMsg->m_dwKey );
    }
    else
    {
        RecallProc.ReCallACK(pPlayer, pRecvMsg->CalleeGUID);
    }
}

VOID GFriendInvitationController::OnAGRecallNak(LPVOID , MSG_BASE * pMsg, WORD )
{
    const MSG_AG_EVENT_RECALL_NAK* pRecvMsg = (const MSG_AG_EVENT_RECALL_NAK*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if(!pPlayer)
    {
        MessageOut( eCRITICAL_LOG, "AG_EVENT_RECALL_NAK Not Found User [%u]", pRecvMsg->m_dwKey );
    }
    else
    {
        RecallProc.ReCallNAK(pPlayer, pRecvMsg->ErrorCode);
    }
}

VOID GFriendInvitationController::GetInvitatedFrindInfo(const Player* player,
                                    MSG_BASE * pMsg,
                                    const eSERIALIZE eType)
{    
    MSG_DG_CHARINFO_INVITATEDFRIEND_CMD* InvitatedFriend_Info = 
                                                (MSG_DG_CHARINFO_INVITATEDFRIEND_CMD*)pMsg;

    if( eType == SERIALIZE_LOAD )
    {     
        DWORD friend_guid[MAX_FRIEND_STAT_NUM];
        InvitatedFriend_Info->friend_list_.friend_count_ = 0;

        const BYTE friend_type_count = 2;

        InvitateFriendInfo::FRIEND_TYPE friend_type[friend_type_count] = 
                                                            {InvitateFriendInfo::FRIEND_BEINVIATED,
                                                             InvitateFriendInfo::FRIEND_INVIATE};

        for (int type_index = 0 ; type_index < friend_type_count ; type_index++)
        {
            UINT friendlist_count;
            // (CHANGES) (f100515.2L) change the interface usage specification
            friendlist_count = player->GetInvitateFriend().GetFriendList(
                friend_type[type_index], friend_guid, _countof(friend_guid));
            for(int i = 0 ; i < friendlist_count ; i++)
            {
                InvitatedFriend_Info->friend_list_.friend_[InvitatedFriend_Info->friend_list_.friend_count_].friend_state_
                                                                 = friend_type[type_index];
                InvitatedFriend_Info->friend_list_.friend_[InvitatedFriend_Info->friend_list_.friend_count_++].character_guid_
                                                                 = friend_guid[i];                
            }
        }        
    }
    else if( eType == SERIALIZE_STORE )
    {
        player->GetInvitateFriend().ClearAllFriendList();
        for(int i = 0 ; i < InvitatedFriend_Info->friend_list_.friend_count_ ; i++)
        {
            player->GetInvitateFriend().AddFriend(static_cast<InvitateFriendInfo::FRIEND_TYPE>(
                                                  InvitatedFriend_Info->friend_list_.friend_[i].friend_state_),
                                                  InvitatedFriend_Info->friend_list_.friend_[i].character_guid_);
        }
    }
    else
    {
        ASSERT("로딩도 아니고 저장도 아니고~ 있을수 없는일...");
    }
}
