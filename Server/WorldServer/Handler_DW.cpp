#include "stdafx.h"
#include "Handler_DW.h"
#include "GameDBProxySession.h"
#include <Protocol_DW.h>
#include <PacketStruct_DW.h>

// ģ�� ��� ó�� ����
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>
#include "User.h"
#include "UserManager.h"

// ������(����ģ��) ����Ʈ ó�� ����-Agent�� ��� �ʿ�
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include <PacketStruct_AW.h>
#include <ResultCode.h>

//ģ���ʴ� �̺�Ʈ�� ���� �ʿ�
#include "FriendInvitation/WFriendInvitationController.h"

#include "WorldGuild.h"
#include "WorldGuildManager.h"

#include "WorldServer.h"
#include <PacketStruct_MW.h>
#include "GroupManager.h"

#include "Lotto/Lotto.h"

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
#include "BaseZone.h"
#include "ObjectKeyStream.h"
#endif  // NA_004694_20120412_1202_PARTY_ROOM_KEY

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationManager.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

VOID Handler_DW::OnDW_CONNECTION_SERVER_INFO_CMD( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MessageOut(eCRITICAL_LOG, "OnDW_CONNECTION_SERVER_INFO_CMD");
}

// ģ�� ��� ����
VOID Handler_DW::OnDW_FRIEND_GETLIST_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_GETLIST_ACK*	pRecvMsg = (MSG_DW_FRIEND_GETLIST_ACK*)pMsg;

    //	MSG_CW_FRIEND_FRIENDLIST_ACK	sendFriend;
    //	MSG_CW_FRIEND_BLOCKLIST_ACK		sendBlock;
    User*						pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );
	

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_FRIEND_GETLIST_ACK Error - User Already Logout[CharGuid:%u]", pRecvMsg->m_CharGuid );
        return;
    }

    User*	pFriend = NULL;

    //	sendFriend.byFriendStatNum = 0;
    //	sendBlock.byBlockStatNum = 0;

    STRUCT_FRIEND_INFO sFriendInfo;
    STRUCT_BLOCK_INFO sBlockInfo;
    STRUCT_REVENGE_INFO	sRevengeInfo;
    //int nFriendCnt=0, nBlockCnt=0, nRevengeCnt=0;

    // ģ���� �������� �и��Ѵ�
    const uint16_t number_of_friends = pRecvMsg->number_of_friends;
    for(USHORT i=0; i<number_of_friends; i++)
    {
        // ģ��
        switch( pRecvMsg->m_sFriendInfo[i].Sts )
        {
        //_KR_0_20091021_FIRENDINVITATION_EXTEND //ģ�� �߰� = {
		case STRUCT_FRIEND_DB_INFO::INFO_INVIAT:
		case STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED:
        //}
        case STRUCT_FRIEND_DB_INFO::INFO_FRIEND:
            {
                //				sendFriend.byFriendStatNum++;
                //�� ĳ���� ������ ã�� ? GUID �� ã�� 
                //pFriend = UserManager::Instance()->FindUser( pRecvMsg->m_sFriendInfo[nFriendCnt].ptszFriendName );
                pFriend = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_sFriendInfo[i].dwFriendGuid );
                // ģ���� �� �¶����� �� �ֳ� Ȯ��~!
                if( pFriend && !pFriend->FindBlock( pRecvMsg->m_CharGuid ) )	// �¶����̰� ���ܴ����� �ʾ�����
                {
                    sFriendInfo.sPacketInfo.bOnline = TRUE;
                    sFriendInfo.sPacketInfo.byChannel = static_cast<BYTE>(pFriend->GetChannelID());
                    // ���� ģ�� �����Ϳ� �ٸ� �� �����Ƿ� ������Ʈ
                    pFriend->CheckOnlineLevel( pRecvMsg->m_sFriendInfo[i].Level );
                    pFriend->AddOnlineFriend( pUser, FALSE );	// ���� ���� ���� ��Ȳ�� �ƴϹǷ� 2��° �Ű����� FALSE ����
                }
                else
                {
                    sFriendInfo.sPacketInfo.bOnline = FALSE;
                    sFriendInfo.sPacketInfo.byChannel = 0;
                }
                memset( sFriendInfo.sPacketInfo.ptszFriendName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
                _tcsncpy( sFriendInfo.sPacketInfo.ptszFriendName, 
                    pRecvMsg->m_sFriendInfo[i].ptszFriendName, 
                    MAX_CHARNAME_LENGTH );
                sFriendInfo.sPacketInfo.ptszFriendName[MAX_CHARNAME_LENGTH] = '\0';
                sFriendInfo.dwFriendGuid = pRecvMsg->m_sFriendInfo[i].dwFriendGuid;
                sFriendInfo.sPacketInfo.byClass = pRecvMsg->m_sFriendInfo[i].byClass;
                sFriendInfo.sPacketInfo.Level = pRecvMsg->m_sFriendInfo[i].Level;                
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
                sFriendInfo.sPacketInfo.gender = pRecvMsg->m_sFriendInfo[i].gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
                //ģ�� �߰�, _KR_0_20091021_FIRENDINVITATION_EXTEND
                WFriendInvitationController::Instance()->OnAddFriend(sFriendInfo, pRecvMsg->m_sFriendInfo[i] ,pUser);                
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
				pUser->GetFriendMap()->AddFriendInfo( &sFriendInfo );
#else
				pUser->AddFriendInfo( &sFriendInfo );
#endif			
                //				memcpy( &sendFriend.sFriendStat[nFriendCnt], &sFriendInfo, sizeof(STRUCT_FRIEND_INFO) );
                //nFriendCnt++;
                break;
            }
        case STRUCT_FRIEND_DB_INFO::INFO_BLOCK:
            {
                //				sendBlock.byBlockStatNum++;
                _tcsncpy( sBlockInfo.ptszBlockName, 
                    pRecvMsg->m_sFriendInfo[i].ptszFriendName, 
                    sizeof(TCHAR) * MAX_CHARNAME_LENGTH );
                sBlockInfo.ptszBlockName[MAX_CHARNAME_LENGTH] = '\0';
                sBlockInfo.dwBlockGuid = pRecvMsg->m_sFriendInfo[i].dwFriendGuid;
                pUser->AddBlockInfo( &sBlockInfo );
                //				memcpy( &sendBlock.sBlockStat[nBlockCnt], &sBlockInfo, sizeof(STRUCT_BLOCK_INFO) );
//                nBlockCnt++;
                break;
            }
        case STRUCT_FRIEND_DB_INFO::INFO_REVENGE:
            {
                pFriend = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_sFriendInfo[i].dwFriendGuid );
                //pFriend = UserManager::Instance()->FindUser( pRecvMsg->m_sFriendInfo[nFriendCnt].ptszFriendName );

                // ������ ���� �� �¶����� �� �ֳ� Ȯ��~!
                if( pFriend )	// �¶����̸�
                {
                    sRevengeInfo.sPacketInfo.bOnline = TRUE;
                    // ���� �����Ϳ� �ٸ� �� �����Ƿ� ������Ʈ
                    pFriend->CheckOnlineLevel( pRecvMsg->m_sFriendInfo[i].Level );

                    // 20061121_�߷�_������ ��� ä�ΰ� �ʿ�
                    sRevengeInfo.sPacketInfo.byChannelID = static_cast<BYTE>(pFriend->GetChannelID());

                    pFriend->AddOnlineRevenge( pUser, FALSE );	// ���� ���� ���� ��Ȳ�� �ƴϹǷ� 2��° �Ű����� FALSE ����
                }
                else
                {
                    sRevengeInfo.sPacketInfo.bOnline = FALSE;
                }
                memset( sRevengeInfo.sPacketInfo.ptszRevengeName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
                _tcsncpy( sRevengeInfo.sPacketInfo.ptszRevengeName, 
                    pRecvMsg->m_sFriendInfo[i].ptszFriendName, 
                    MAX_CHARNAME_LENGTH );
                sRevengeInfo.dwRevengeCharGuid = pRecvMsg->m_sFriendInfo[i].dwFriendGuid;
                sRevengeInfo.sPacketInfo.byClass = pRecvMsg->m_sFriendInfo[i].byClass;
                sRevengeInfo.sPacketInfo.Level = pRecvMsg->m_sFriendInfo[i].Level;

                pUser->AddRevengeInfo( &sRevengeInfo );
//                nRevengeCnt++;
                break;
            }
        case STRUCT_FRIEND_DB_INFO::INFO_REVENGE_POINT:
            {
                // ��ġ�� ������ ����Ʈ�� ����
                pUser->SetRevengePoint( pRecvMsg->m_sFriendInfo[i].RevengePoint );
                break;
            }
        default:
            {
                //				pUser->SendPacket( (BYTE*)&sendFriend, sendFriend.GetSize() );
                //				pUser->SendPacket( (BYTE*)&sendBlock, sendBlock.GetSize() );
                MessageOut( eCRITICAL_LOG, "[%s]DW_FRIEND_GETLIST Error - No Friend/Block Type[Sts:%u]", pUser->GetCharName().c_str(), pRecvMsg->m_sFriendInfo[i].Sts );
                //2010-01-05 ������ ������ �ּ� ó�� �Ѵ�. (����Ʈ �ϳ� �߸� �Ǿ��ٰ� ��ü ģ������� �̻��� ���� �ȵȴ�)
                //return;
            }
        }
    }

    UserManager::Instance()->AddFriend( pUser );	// ģ�� ��� ������Ʈ
    UserManager::Instance()->AddRevenge( pUser );

    // ������ ���� Agent�� ����
    ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );

    if( !pAgentServer )
    {
        MessageOut( eCRITICAL_LOG, "Revenge ADD Error - No AgentServer!!![%u]", pUser->GetChannelID() );
    }
    else
    {
        MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK ackMsg;
        ackMsg.m_dwCharGuid = pUser->GetCharGuid();
        ackMsg.m_byRevengePoint = pUser->GetRevengePoint();
        ackMsg.m_byRevengeStatNum = pUser->GetRevengeNum();

        if( ackMsg.m_byRevengeStatNum > 0 )
        {
            STRUCT_REVENGE_INFO* pRevengeInfo = pUser->GetFirstRevenge();

            memcpy( ackMsg.m_sRevengeUserInfo, pRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );

            int i=1;
            while( pRevengeInfo = pUser->GetNextRevenge() )
            {
                memcpy( &ackMsg.m_sRevengeUserInfo[i], pRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );
                i++;
            }
        }

        pAgentServer->Send( (BYTE*)&ackMsg, ackMsg.GetSize() );
        MessageOut( eFULL_LOG, "[%s][CharGuid:%u] Revenge List Send to Agent[%u/%u]", pUser->GetCharName().c_str(), 
            pUser->GetCharGuid(), 
            pUser->GetRevengeNum(), 
            pUser->GetRevengePoint() );
    }
    MessageOut( eFULL_LOG, "[%s][Guid:%u] Friend/Block List Recv", pUser->GetCharName().c_str(), pUser->GetGUID() );

    // _KR_0_20091021_FIRENDINVITATION_EXTEND, ������Ʈ ������ ģ����õ ����� �����Ѵ�. (W->A->G)
	WFriendInvitationController::Instance()->SendInvitateFriendList(pUser, SEND_AGENTSERVER);
}

// ģ�� ��� ���� ����
VOID Handler_DW::OnDW_FRIEND_GETLIST_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_GETLIST_NAK*	pRecvMsg = (MSG_DW_FRIEND_GETLIST_NAK*)pMsg;

    // �ش� ������ ������?
    User	*pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );

    if( !pUser )
    {
        MessageOut( eCRITICAL_LOG, "DW_FRIEND_GETLIST_NAK Error - Already Logout[CharGuid:%u]", pRecvMsg->m_CharGuid );
        return;
    }

    MSG_CW_FRIEND_NEWLIST_NAK	sendMsg;

    // �ٷ� �˷��ش�!
    sendMsg.dwResult = pRecvMsg->m_byResult;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "[%s][Guid:%u] Can't Get Friend List(%u)", pUser->GetCharName().c_str(), pUser->GetGUID(), pRecvMsg->m_byResult );
}

// ģ�� �߰� ����
VOID Handler_DW::OnDW_FRIEND_ADD_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{    
    MSG_DW_FRIEND_ADD_ACK*	pRecvMsg = (MSG_DW_FRIEND_ADD_ACK*)pMsg;
    User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] Friend ADD - User Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    STRUCT_FRIEND_INFO	sFriendInfo;

    memset( &sFriendInfo, 0, sizeof(STRUCT_FRIEND_INFO) );

    memcpy( sFriendInfo.sPacketInfo.ptszFriendName, pRecvMsg->m_ToFriendInfo.sPacketInfo.ptszFriendName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );

    User* pFriend = UserManager::Instance()->FindUser( sFriendInfo.sPacketInfo.ptszFriendName );

    //ģ���� ���� ���� ������ ��� �߰� 2009-11-09 youngmoon ���׹�ȣ:5315
    if( pFriend && !pFriend->FindBlock( pRecvMsg->m_CharGuidFrom ) )
    {
        sFriendInfo.sPacketInfo.bOnline = TRUE;
        // ���� ģ�� �����Ϳ� �ٸ� �� �����Ƿ� ������Ʈ
        pFriend->CheckOnlineLevel( pRecvMsg->m_ToFriendInfo.sPacketInfo.Level );
        pFriend->AddOnlineFriend( pUser, FALSE );	// ���� ���� ���� ��Ȳ�� �ƴϹǷ� 2��° �Ű����� FALSE ����
        sFriendInfo.sPacketInfo.byChannel = static_cast<BYTE>(pFriend->GetChannelID());
    }
    else
    {        
        sFriendInfo.sPacketInfo.bOnline = FALSE;
        sFriendInfo.sPacketInfo.byChannel = 0;
    }    

    sFriendInfo.sPacketInfo.ptszFriendName[MAX_CHARNAME_LENGTH] = '\0';
    sFriendInfo.sPacketInfo.byClass = pRecvMsg->m_ToFriendInfo.sPacketInfo.byClass;
    sFriendInfo.sPacketInfo.Level = pRecvMsg->m_ToFriendInfo.sPacketInfo.Level;
    sFriendInfo.dwFriendGuid = pRecvMsg->m_ToFriendInfo.dwFriendGuid;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    sFriendInfo.sPacketInfo.gender = pRecvMsg->m_ToFriendInfo.sPacketInfo.gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	if( !pUser->GetFriendMap()->AddFriendInfo( &sFriendInfo ) )
#else
	if( !pUser->AddFriendInfo( &sFriendInfo ) )
#endif    
    {
        // ģ�� ���� �߰� ����
        MessageOut( eCRITICAL_LOG, "[%s] Friend ADD Error - Can't Add FriendInfo", pUser->GetCharName().c_str() );
        return;
    }

    // ģ�� �߰������� �˷��ش�
    MSG_CW_FRIEND_ADD_ACK	sendMsg;
    sendMsg.sFriendStat = sFriendInfo;

    // ��� ������ Gm �ϰ�� �ڽ��� Gm�� �ƴϸ� ������ ���� �ʴ´�./
    if( pFriend && 
        TRUE  == pFriend->IsGM() &&
        FALSE == pUser->IsGM() )
    {
        sendMsg.sFriendStat.sPacketInfo.bOnline   = FALSE;
        sendMsg.sFriendStat.sPacketInfo.byChannel = 0;
    }

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );


    UserManager::Instance()->AddNewFriend( sFriendInfo.dwFriendGuid, pUser );

    MessageOut( eFULL_LOG, "[%s->%s] Friend ADD", pUser->GetCharName().c_str(), sFriendInfo.sPacketInfo.ptszFriendName );
}

// ģ�� ��û ����
VOID Handler_DW::OnDW_FRIEND_ADD_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_ADD_NAK*	pRecvMsg = (MSG_DW_FRIEND_ADD_NAK*)pMsg;
    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] FRIEND_ADD_NAK Error - ADD Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    MSG_CW_FRIEND_ADD_NAK	sendMsg;

    sendMsg.m_byResult = pRecvMsg->m_byResult;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "[%s] FRIEND_ADD_NAK", pUser->GetCharName().c_str() );
}

// ģ�� ���� ����
VOID Handler_DW::OnDW_FRIEND_DEL_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_DEL_ACK*	pRecvMsg	= (MSG_DW_FRIEND_DEL_ACK*)pMsg;
    User*					pUser		= UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] FRIEND_DEL_ACK - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );
    // �¶��� ����Ʈ���� �����
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap()->FindFriend( pRecvMsg->m_CharGuidTo );
#else
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->FindFriend( pRecvMsg->m_CharGuidTo );
#endif    
    if( !pFriendInfo )
    {
        // �̷� ��쵵 ������? �Ƹ��� �ߺ� ������ ���ư��� ������ ��쿡�� �߻��� ���̴�
        MessageOut( eCRITICAL_LOG, "[%s/%u] FRIEND_DEL Error - Friend Already Deleted", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidTo );
        // �̷� ��Ȳ�̶�� �ص� DB������ Ȯ���� ������ ���̹Ƿ� ACK ��� �Ѵ�
        MSG_CW_FRIEND_DEL_ACK	sendMsg;

        memset( sendMsg.ptszDeletedName, 0, sizeof(sendMsg.ptszDeletedName) );

        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
        return;
    }
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //ģ�� ���� ����
	//���� ģ�� ��õ�����϶� ģ���� ���ʿ��� �����ϸ� ģ�� ��õ���谡 ����ȴ�.
    WFriendInvitationController::Instance()->OnFriendRemove(pGameDBProxy, pMsg, wSize);	
    UserManager::Instance()->RemoveNewFriend( pFriendInfo->dwFriendGuid, pUser );

    // �˷��ش�
    MSG_CW_FRIEND_DEL_ACK	sendMsg;

    _tcsncpy( sendMsg.ptszDeletedName, pFriendInfo->sPacketInfo.ptszFriendName, MAX_CHARNAME_LENGTH );
    sendMsg.ptszDeletedName[MAX_CHARNAME_LENGTH] = '\0';

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "[%s/%u->%s] FRIEND_DEL_ACK", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidFrom, pFriendInfo->sPacketInfo.ptszFriendName );
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	pUser->GetFriendMap()->RemoveFriend(pRecvMsg->m_CharGuidTo );
#else
	pUser->RemoveFriend(pRecvMsg->m_CharGuidTo );
#endif
}

// ģ�� ���� ����
VOID Handler_DW::OnDW_FRIEND_DEL_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_DEL_NAK*	pRecvMsg	= (MSG_DW_FRIEND_DEL_NAK*)pMsg;
    User*					pUser		=  UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] FRIEND_DEL_NAK - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    // �˷��ش�
    MSG_CW_FRIEND_DEL_NAK	sendMsg;

    sendMsg.m_byResult = pRecvMsg->m_byResult;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_DEL_NAK) );

    MessageOut( eFULL_LOG, "[%s][CharGuid:%u] FRIEND_DEL_NAK", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidFrom );
}

// ���� ����
VOID Handler_DW::OnDW_FRIEND_BLOCK_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_BLOCK_ACK*	pRecvMsg = (MSG_DW_FRIEND_BLOCK_ACK*)pMsg;
    User* pBlock = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pBlock )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] Block[%s] ADD User Already Logout", pRecvMsg->m_CharGuidFrom, pRecvMsg->m_BlockInfo.ptszBlockName );
        return;
    }

    pBlock->SetFriendTransaction( FALSE );

    // �¶����̸� ����ڿ� ���� �߰�
    STRUCT_BLOCK_INFO	sBlockInfo;

    memset( &sBlockInfo, 0, sizeof(STRUCT_BLOCK_INFO) );

    sBlockInfo.dwBlockGuid = pRecvMsg->m_BlockInfo.dwBlockGuid;
    _tcsncpy( sBlockInfo.ptszBlockName, pRecvMsg->m_BlockInfo.ptszBlockName, MAX_CHARNAME_LENGTH );
    sBlockInfo.ptszBlockName[MAX_CHARNAME_LENGTH] = '\0';

    if( !pBlock->AddBlockInfo( &sBlockInfo ) )
    {
        // ���� �߰� ����
        MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

        sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;

        pBlock->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );

        MessageOut( eCRITICAL_LOG, "[%s] Block ADD Error - Can't Add BlockInfo", pBlock->GetCharName().c_str() );
        return;
    }

    // ��ϵ� ���� Ȥ�ó� �¶��ο� ������ �� ������ �����!
    User *pBlocked = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_BlockInfo.dwBlockGuid );

    if( pBlocked )
    {
        pBlock->RemoveOnlineFriend( pBlocked );
    }

    // ��� ���������� �˷��ش�
    MSG_CW_FRIEND_BLOCK_ACK	sendMsg;
    _tcsncpy( sendMsg.ptszBlockedName, pRecvMsg->m_BlockInfo.ptszBlockName, MAX_CHARNAME_LENGTH );
    sendMsg.ptszBlockedName[MAX_CHARNAME_LENGTH] = '\0';

    pBlock->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_ACK) );

    MessageOut( eFULL_LOG, "[%s->%s] Block ADD", pBlock->GetCharName().c_str(), pRecvMsg->m_BlockInfo.ptszBlockName );

    //_KR_0_20091021_FIRENDINVITATION_EXTEND //ģ�� ���� ����
	//���� ģ�� ��õ�����϶� ģ���� ���ʿ��� �����ϸ� ģ�� ��õ���谡 ����ȴ�.
    WFriendInvitationController::Instance()->OnFriendBlock(pGameDBProxy, pMsg, wSize);	
}

// ���� ����
VOID Handler_DW::OnDW_FRIEND_BLOCK_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_BLOCK_NAK*	pRecvMsg = (MSG_DW_FRIEND_BLOCK_NAK*)pMsg;
    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] FRIEND_BLOCK_NAK Error - ADD Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

    sendMsg.m_byResult = pRecvMsg->m_byResult;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );

    MessageOut( eFULL_LOG, "[%s] FRIEND_BLOCK_NAK", pUser->GetCharName().c_str() );
}

// ���� ���� ����
VOID Handler_DW::OnDW_FRIEND_BLOCK_FREE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_BLOCK_FREE_ACK*	pRecvMsg = (MSG_DW_FRIEND_BLOCK_FREE_ACK*)pMsg;
    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_FRIEND_BLOCK_FREE_ACK Error - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    STRUCT_BLOCK_INFO* pBlockInfo = pUser->FindBlock( pRecvMsg->m_CharGuidTo );

    // ���� ������ ������
    if( !pBlockInfo)
    {
        // ������
        MSG_CW_FRIEND_BLOCK_FREE_NAK	sendMsg;

        sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_FREE_NAK::NOT_BLOCKED;

        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_NAK) );
        return;
    }

    MSG_CW_FRIEND_BLOCK_FREE_ACK	sendMsg;

    _tcsncpy( sendMsg.ptszBlockedName, pBlockInfo->ptszBlockName, MAX_CHARNAME_LENGTH );
    sendMsg.ptszBlockedName[MAX_CHARNAME_LENGTH] = '\0';

    // Ȥ�� �̳��� �¶����̰� ���� ģ���� �߰��߾��ٸ� �α��� ���·� �ٲ��ش�
    User* pBlocked = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidTo );

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	if( pBlocked && pBlocked->GetFriendMap()->FindFriend( pUser->GetCharGuid() ) )
#else
	if( pBlocked && pBlocked->FindFriend( pUser->GetCharGuid() ) )
#endif
    {
        // �α��� ���·� ����!!
        pUser->AddOnlineFriend( pBlocked );
    }

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_ACK) );

    MessageOut( eFULL_LOG, "[%s->%s] FRIEND_BLOCK_FREE_ACK", pUser->GetCharName().c_str(), pBlockInfo->ptszBlockName );
    pUser->RemoveBlock( pBlockInfo->dwBlockGuid );
}

// ���� ���� ����
VOID Handler_DW::OnDW_FRIEND_BLOCK_FREE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_FRIEND_BLOCK_FREE_NAK*	pRecvMsg = (MSG_DW_FRIEND_BLOCK_FREE_NAK*)pMsg;
    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_FRIEND_BLOCK_FREE_NAK Error - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    MSG_CW_FRIEND_BLOCK_FREE_NAK	sendMsg;

    sendMsg.m_byResult = pRecvMsg->m_byResult;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_NAK) );

    MessageOut( eFULL_LOG, "[%s] MSG_DW_FRIEND_BLOCK_FREE_NAK", pUser->GetCharName().c_str() );
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
VOID Handler_DW::OnDW_FRIEND_DATAIL_INFO_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_FRIEND_DETAIL_INFO_ACK* pRecvMsg = (MSG_DW_FRIEND_DETAIL_INFO_ACK*)pMsg;
    User* pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);

    if (pUser == NULL)
        return;

    MSG_CW_FRIEND_STATUS_ANS ansMsg;

    User* pToUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwToCharGuid);
    if (pToUser != NULL)
    {
        ansMsg.m_byChannel = (BYTE)pToUser->GetChannelID();
        ansMsg.m_byZoneType = pToUser->GetStatus();
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
        if (pToUser->GetParty() != NULL && pToUser->GetStatus() == eZONETYPE_MISSION)
        {
            ansMsg.m_dwZoneKey = pToUser->GetParty()->GetRoomKey() - nsKeyInfo::PARTY_ROOM_KEY_START;
        }
        else
        {
			ansMsg.m_dwZoneKey = pToUser->GetZoneKey();
        }
#else
        ansMsg.m_dwZoneKey = pToUser->GetZoneKey();
#endif  // NA_004694_20120412_1202_PARTY_ROOM_KEY

        ansMsg.m_MapCode = pToUser->GetMapCode();
    }
    else
    {
        ansMsg.m_byChannel = 0;
        ansMsg.m_byZoneType = eZONETYPE_MAX;
        ansMsg.m_dwZoneKey = 0;
        ansMsg.m_MapCode = 0;
    }

    _tcsncpy(ansMsg.m_ptszCharName, pRecvMsg->m_ptszToCharName, _countof(ansMsg.m_ptszCharName));
    ansMsg.m_ptszCharName[_countof(ansMsg.m_ptszCharName) - 1] = _T('\0');
    _tcsncpy(ansMsg.m_tszGuildName, pRecvMsg->m_ptszToCharGuildName, _countof(ansMsg.m_tszGuildName));
    ansMsg.m_tszGuildName[_countof(ansMsg.m_tszGuildName) - 1] = _T('\0');
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    _tcsncpy(ansMsg.m_ptszMemo, pRecvMsg->m_ptszMemo, _countof(ansMsg.m_ptszMemo));
    ansMsg.m_ptszMemo[_countof(ansMsg.m_ptszMemo) - 1] = _T('\0');
#endif

    ansMsg.m_byDuty = pRecvMsg->m_byToCharGuildDuty;

    pUser->SendPacket((BYTE*)&ansMsg, sizeof(ansMsg));
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
VOID Handler_DW::OnDW_FRIEND_DETAIL_INFO_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_FRIEND_DETAIL_INFO_NAK* pRecvMsg = (MSG_DW_FRIEND_DETAIL_INFO_NAK*)pMsg;

    User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

    if (pUser == NULL)
        return;

    MSG_DW_FRIEND_DETAIL_INFO_NAK sendMsg;
    sendMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;

    pUser->SendPacket((BYTE*)&sendMsg, sizeof(sendMsg));

    MessageOut(eFULL_LOG, "[%s][%d] OnDW_FRIEND_DETAIL_INFO_NAK",
               pUser->GetCharName().c_str(), pRecvMsg->m_byErrorCode);
}

VOID Handler_DW::OnDW_FRIEND_MEMO_UPDATE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_FRIEND_MEMO_UPDATE_ACK* pRecvMsg = (MSG_DW_FRIEND_MEMO_UPDATE_ACK*)pMsg;
    User* pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);

    if(!pUser)
        return;

    MSG_CW_FRIEND_MEMO_UPDATE_ACK sendMsg;
    sendMsg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;
    pUser->SendPacket((BYTE*)&sendMsg, sizeof(sendMsg));
}

VOID Handler_DW::OnDW_FRIEND_MEMO_UPDATE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_FRIEND_MEMO_UPDATE_NAK* pRecvMsg = (MSG_DW_FRIEND_MEMO_UPDATE_NAK*)pMsg;

    User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

    if(!pUser)
        return;

    MSG_CW_FRIEND_MEMO_UPDATE_NAK sendMsg;
    sendMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "[%s][%d] OnDW_FRIEND_MEMO_UPDATE_NAK",
        pUser->GetCharName().c_str(), pRecvMsg->m_byErrorCode );
}



// ä�ñ���
VOID Handler_DW::OnDW_GM_CHATBLOCK_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATBLOCK_ACK* pRecvMsg = (MSG_DW_GM_CHATBLOCK_ACK*)pMsg;
    MSG_CW_GM_USERCHAT_BLOCK_ACK sendMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);

    if(!pUser)
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_GM_CHATBLOCK_ACK Error - Already Logout", pRecvMsg->m_dwKey );
        return;
    }

    sendMsg.m_CharGuid = pRecvMsg->m_CharGuid;
    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
    sendMsg.m_bBlock = TRUE;
    sendMsg.m_dwBlockTime = pRecvMsg->m_dwBlockTime;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    User *pTarget = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );

    if( pTarget )
    {
        // ���� ���� ����
        //__NA_000313_20070223_REALTIME_CHAT_BLOCK
        UserManager::Instance()->InsertChatBlockUser(pTarget, pRecvMsg->m_dwBlockTime);

#define _WHATTHEHELL_BLOCKED_20060517

#ifdef _WHATTHEHELL_BLOCKED_20060517	// ä�ñ��� �뺸 ��� �׽�Ʈ��
        // ���� ����� �˷��ش�
        MSG_CW_CHAT_BLOCKED_CMD	blockMsg;

        blockMsg.m_dwRemainTime = pRecvMsg->m_dwBlockTime;

        pTarget->SendPacket( (BYTE*)&blockMsg, sizeof(blockMsg) );
#endif
    }

    // ä�� ���� ����
    MessageOut( eFULL_LOG, "[%u] ChatBlock Success[%u]", pRecvMsg->m_CharGuid, pRecvMsg->m_dwBlockTime );
}

VOID Handler_DW::OnDW_GM_CHATBLOCK_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATBLOCK_NAK* pRecvMsg = (MSG_DW_GM_CHATBLOCK_NAK*)pMsg;
    MSG_CW_GM_USERCHAT_BLOCK_NAK	sendMsg;

    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_GM_CHATBLOCK_NAK Error - Already Logout", pRecvMsg->m_dwKey );
        return;
    }

    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
    sendMsg.m_wErrorCode = MSG_CW_GM_USERCHAT_BLOCK_NAK::_ERR_DEFAULT;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    // ä�� ���� ����
    MessageOut( eFULL_LOG, "[%u] ChatBlock Fail[%u]", pRecvMsg->m_dwKey, pRecvMsg->m_wErrorCode );
}

// ä�����
VOID Handler_DW::OnDW_GM_CHATALLOW_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATALLOW_ACK* pRecvMsg = (MSG_DW_GM_CHATALLOW_ACK*)pMsg;
    MSG_CW_GM_USERCHAT_BLOCK_ACK	sendMsg;

    User *pAllowUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_CharGuid);

    if(!pAllowUser)
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_GM_CHATALLOW_ACK Error - Already Logout", pRecvMsg->m_dwKey );
        return;
    }	

    if(!UserManager::Instance()->RemoveChatBlockUser( pAllowUser ))
    {
        MessageOut(eCRITICAL_LOG, "[%d] ChatAllow Failed", pRecvMsg->m_CharGuid);
        return;
    }

    // ä�� ��� ����
    sendMsg.m_CharGuid = pRecvMsg->m_CharGuid;
    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
    sendMsg.m_bBlock = FALSE;
    sendMsg.m_dwBlockTime = 0;

    pAllowUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eCRITICAL_LOG, "[%u] ChatAllow Success", pRecvMsg->m_CharGuid );
}

VOID Handler_DW::OnDW_GM_CHATALLOW_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATALLOW_NAK* pRecvMsg = (MSG_DW_GM_CHATALLOW_NAK*)pMsg;
    MSG_CW_GM_USERCHAT_BLOCK_NAK	sendMsg;

    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eCRITICAL_LOG, "[CharGuid:%u] MSG_DW_GM_CHATALLOW_NAK Error - Already Logout", pRecvMsg->m_dwKey );
        return;
    }

    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
    sendMsg.m_wErrorCode = MSG_CW_GM_USERCHAT_BLOCK_NAK::_ERR_DEFAULT;

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    // ä�� ��� ����
    MessageOut( eCRITICAL_LOG, "[%u] ChatAllow Fail[%u]", pRecvMsg->m_dwKey, pRecvMsg->m_wErrorCode );
}

// �����ð� ������Ʈ
VOID Handler_DW::OnDW_GM_CHATBLOCK_UPDATE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATBLOCK_UPDATE_NAK* pRecvMsg = (MSG_DW_GM_CHATBLOCK_UPDATE_NAK*)pMsg;

    MessageOut( eFULL_LOG, "[%u]ChatBlock_Update Fail[%u]", pRecvMsg->m_dwKey, pRecvMsg->m_wErrorCode );
}

// ä�ñ��� �ð� ��û(ó�� ������ ��)
VOID Handler_DW::OnDW_GM_CHATBLOCKTIME_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GM_CHATBLOCKTIME_ANS* pRecvMsg = (MSG_DW_GM_CHATBLOCKTIME_ANS*)pMsg;

    // �ش� ������� ChatBlock ����
    User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

    if(!pUser)
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eFULL_LOG, "[CharGuid:%u] OnDW_GM_CHATBLOCKTIME_ANS Error - No User", pRecvMsg->m_dwKey );
        return;
    }

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
    if(pRecvMsg->m_dwRemainTime > 0)
    {
        UserManager::Instance()->InsertChatBlockUser( pUser, pRecvMsg->m_dwRemainTime );	
    }
    else
    {
        pUser->FreeChatBlock();
    }

    MessageOut( eFULL_LOG, "[CharGuid:%u] User ChatBlock Status Recv[%u]", pUser->GetCharGuid(), pRecvMsg->m_dwRemainTime );
}

VOID Handler_DW::OnDW_GM_CHATBLOCKTIME_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MessageOut( eCRITICAL_LOG, "[CharGuid:%u] User ChatBlock Status Recv Failed", pMsg->m_dwKey );

    // �ش� ������� ChatBlock ����
    User*	pUser = UserManager::Instance()->FindUserByCharGuid( pMsg->m_dwKey );

    if( !pUser )
    {
        // �̹� ��û�� ĳ���Ͱ� �������ȴٸ� ������ ���� �ʿ� ����
        MessageOut( eFULL_LOG, "[CharGuid:%u] OnDW_GM_CHATBLOCKTIME_ANS Error - No User", pMsg->m_dwKey );
        return;
    }

    pUser->FreeChatBlock();
}

////////////////////////////
// ������ ���� �ڵ鷯

VOID Handler_DW::OnDW_CHAO_ADD_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    // ������ �߰� �Ϸ�
    MSG_DW_CHAO_ADD_REVENGE_CHAR_ACK* pRecvMsg = (MSG_DW_CHAO_ADD_REVENGE_CHAR_ACK*)pMsg;

    // DBProxy�� ��û�� �� �־�� m_dwKey�� Agent ã�´�
    ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pRecvMsg->m_dwKey );

    if( !pAgentServer )
    {
        MessageOut( eCRITICAL_LOG, "Revenge ADD Error - No AgentServer!!![%u]", pRecvMsg->m_dwKey );
        return;
    }

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] Revenge ADD - User Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    STRUCT_REVENGE_INFO	sRevengeInfo;

    memcpy( &sRevengeInfo, &pRecvMsg->m_sRevengeUserInfo, sizeof(STRUCT_REVENGE_INFO) );

    User *pRevenge = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_sRevengeUserInfo.dwRevengeCharGuid );

    if( pRevenge )
    {
        sRevengeInfo.sPacketInfo.bOnline = TRUE;
        pRevenge->CheckOnlineLevel( sRevengeInfo.sPacketInfo.Level );
        pRevenge->AddOnlineRevenge( pUser, FALSE );
    }
    else
    {
        sRevengeInfo.sPacketInfo.bOnline = FALSE;
    }

    // ������ ����Ʈ ���� - Ȥ���� ����ȭ ���з� 0 ���ϴ��� DB���� �� ���� �ϴ´�
    pUser->DecRevengePoint();

    if( !pUser->AddRevengeInfo( &sRevengeInfo ) )
    {
        // ������ ���� �߰� ����
        MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK	nakMsg;

        nakMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
        nakMsg.m_byErrorCode = RC::RC_REVENGE_FAILED;

        pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );
        MessageOut( eCRITICAL_LOG, "[%u][%s] Revenge ADD Error - Can't Add RevengeInfo", pRecvMsg->m_CharGuidFrom, pUser->GetCharName().c_str() );
        return;
    }

    // ���� ��� �߰������� �˷��ش�
    MSG_CW_REVENGE_ADD_CMD	sendMsg;
    sendMsg.m_byRevengePoint = pUser->GetRevengePoint();
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    memcpy( &sendMsg.m_sRevengeStat, &sRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );

    STRUCT_REVENGE_PACKET_INFO& sRevengeStat = sendMsg.m_sRevengeStat.sPacketInfo;
#else
    memcpy( &sendMsg.m_sRevengeStat, &sRevengeInfo.sPacketInfo, sizeof(STRUCT_REVENGE_PACKET_INFO) );

    STRUCT_REVENGE_PACKET_INFO& sRevengeStat = sendMsg.m_sRevengeStat;
#endif

    // ��� ������ Gm �ϰ�� �ڽ��� Gm�� �ƴϸ� ������ ���� �ʴ´�./
    if( pRevenge && 
        TRUE  == pRevenge->IsGM() &&
        FALSE == pUser->IsGM() )
    {
        sRevengeStat.bOnline = FALSE;
        sRevengeStat.byChannelID = 0;
    }

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    UserManager::Instance()->AddNewRevenge( sRevengeInfo.dwRevengeCharGuid, pUser );

    // Agent�� ��� ����
    MSG_AW_CHAO_ADD_REVENGE_CHAR_ACK ackMsg;

    ackMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
    ackMsg.m_dwRevengeCharGuid = pRecvMsg->m_sRevengeUserInfo.dwRevengeCharGuid;
    memcpy( ackMsg.m_ptszRevengeName, sRevengeStat.ptszRevengeName, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );

    pAgentServer->Send( (BYTE*)&ackMsg, sizeof(ackMsg) );

    MessageOut( eFULL_LOG, "[%s(%u)->%s(%u)] Revenge ADD", pUser->GetCharName().c_str(), 
        ackMsg.m_dwCharGuid, 
        sRevengeInfo.sPacketInfo.ptszRevengeName, 
        ackMsg.m_dwRevengeCharGuid );
}

VOID Handler_DW::OnDW_CHAO_ADD_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_CHAO_ADD_REVENGE_CHAR_NAK* pRecvMsg = (MSG_DW_CHAO_ADD_REVENGE_CHAR_NAK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] Revenge ADD NAK - User Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK	nakMsg;

    nakMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

    // DBProxy�� ��û�� �� �־�� m_dwKey�� Agent ã�Ƽ� ��� ������
    ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pRecvMsg->m_dwKey );

    if( !pAgentServer )
    {
        MessageOut( eCRITICAL_LOG, "Revenge ADD NAK Error - No Agent Server!!![%u]", pRecvMsg->m_dwKey );
        return;
    }

    pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

    MessageOut( eFULL_LOG, "[%u] Revenge ADD NAK[Error:%u]", pRecvMsg->m_CharGuidFrom, pRecvMsg->m_byErrorCode );
}

// ������ ������ ����Ʈ���� ����
VOID Handler_DW::OnDW_CHAO_REMOVE_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK*	pRecvMsg = (MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK*)pMsg;
    MSG_CW_REVENGE_DEL_ACK					sendMsg;

    // Agent�� ���� ���� - ����Ʈ���� �����ؾ� �ϱ� ����
    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK	ackMsg;

    ackMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
    ackMsg.m_dwRevengeCharGuid = pRecvMsg->m_RevengeGuidTo;

    // DBProxy�� ��û�� �� �־�� m_dwKey�� Agent ã�Ƽ� ��� ������
    ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pRecvMsg->m_dwKey );

    if( !pAgentServer )
    {
        MessageOut( eCRITICAL_LOG, "Revenge Remove ACK Error - No Agent Server!!![%u]", pRecvMsg->m_dwKey );
        return;
    }

    pAgentServer->Send( (BYTE*)&ackMsg, sizeof(ackMsg) );

    // �������Ե� �˷��ش�
    memset( sendMsg.m_ptszCharName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eCRITICAL_LOG, "DW_CHAO_REMOVE_REVENGE_CHAR_ACK Error - Not Connected User[%u]", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    // �¶��� ����Ʈ���� �����
    STRUCT_REVENGE_INFO* pRevengeInfo = pUser->FindRevenge( pRecvMsg->m_RevengeGuidTo );
    if( !pRevengeInfo )
    {
        // �̷� ��쵵 ������? �Ƹ��� �ߺ� ������ ���ư��� ������ ��쿡�� �߻��� ���̴�
        MessageOut( eCRITICAL_LOG, "[%s/%u] DW_CHAO_REMOVE_REVENGE_CHAR_ACK Error - Already Deleted", pUser->GetCharName().c_str(), 
            pRecvMsg->m_RevengeGuidTo );
        // �̷� ��Ȳ�̶�� �ص� DB������ Ȯ���� ������ ���̹Ƿ� ACK ��� �Ѵ�
        memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );

        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
        return;
    }

    UserManager::Instance()->RemoveNewRevenge( pRevengeInfo->dwRevengeCharGuid, pUser );

    // �˷��ش�
    memcpy( sendMsg.m_ptszCharName, pRevengeInfo->sPacketInfo.ptszRevengeName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
    sendMsg.m_ptszCharName[MAX_CHARNAME_LENGTH]='\0';

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "[%s/%u->%s] REVENGE_DEL_ACK", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidFrom, 
        pRevengeInfo->sPacketInfo.ptszRevengeName );
    pUser->RemoveRevenge( pRecvMsg->m_RevengeGuidTo );
}

VOID Handler_DW::OnDW_CHAO_REMOVE_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK* pRecvMsg = (MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK*)pMsg;

    User*					pUser		=  UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "[CharGuid:%u] REVENGE_DEL_NAK - Already Logout", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    // �˷��ش�
    MSG_CW_REVENGE_DEL_NAK	sendMsg;

    sendMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;
    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_REVENGE_DEL_NAK) );

    MessageOut( eFULL_LOG, "[%s][CharGuid:%u] REVENGE_DEL_NAK", pUser->GetCharName().c_str(), pRecvMsg->m_CharGuidFrom );
}

// PK�� ���� Agent���� ����Ʈ ���� ��û
VOID Handler_DW::OnDW_CHAO_PK_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_CHAO_PK_REVENGE_CHAR_ACK* pRecvMsg = (MSG_DW_CHAO_PK_REVENGE_CHAR_ACK*)pMsg;

    ServerSession *pAgent = ServerSessionManager::Instance()->FindAgentServer( pRecvMsg->m_dwKey );

    if( !pAgent )
    {
        MessageOut( eCRITICAL_LOG, "DW_CHAO_PK_REVENGE_CHAR_ACK Error - No Agent Server!![C:%u][CharGuid:%u]", pRecvMsg->m_dwKey, 
            pRecvMsg->m_CharGuidFrom );
        return;
    }

    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK nakMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( !pUser )
    {
        nakMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
        nakMsg.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;

        pAgent->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

        MessageOut( eCRITICAL_LOG, "DW_CHAO_PK_REVENGE_CHAR_ACK Error - Not Connected User[CharGuid:%u]", pRecvMsg->m_CharGuidFrom );
        return;
    }

    pUser->SetFriendTransaction( FALSE );

    STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pRecvMsg->m_RevengeGuidTo );

    if( !pRevengeInfo )
    {
        nakMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
        nakMsg.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;

        pAgent->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

        MessageOut( eCRITICAL_LOG, "DW_CHAO_PK_REVENGE_CHAR_ACK Error - Not Connected User[CharGuid:%u]", pRecvMsg->m_CharGuidFrom );
        return;
    }

    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK ackMsg;
    MSG_CW_REVENGE_DEL_ACK				sendMsg;

    // Agent�� �˷��ְ�
    ackMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
    ackMsg.m_dwRevengeCharGuid = pRecvMsg->m_RevengeGuidTo;

    pAgent->Send( (BYTE*)&ackMsg, sizeof(ackMsg) );

    // �������Ե� �˷��ش�(����Ʈ���� ������ �ϱ� ����)
    memcpy( sendMsg.m_ptszCharName, pRevengeInfo->sPacketInfo.ptszRevengeName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
    sendMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    MessageOut( eFULL_LOG, "DW_CHAO_PK_REVENGE_CHAR_ACK[Channel:%u][CharGuid:%u][Target:%u]", pRecvMsg->m_dwKey, pRecvMsg->m_CharGuidFrom, 
        pRecvMsg->m_RevengeGuidTo );
}

VOID Handler_DW::OnDW_CHAO_PK_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_CHAO_PK_REVENGE_CHAR_NAK* pRecvMsg = (MSG_DW_CHAO_PK_REVENGE_CHAR_NAK*)pMsg;

    ServerSession *pAgent = ServerSessionManager::Instance()->FindAgentServer( pRecvMsg->m_dwKey );

    if( !pAgent )
    {
        MessageOut( eCRITICAL_LOG, "DW_CHAO_PK_REVENGE_CHAR_NAK Error - No Agent Server!![C:%u][CharGuid:%u]", pRecvMsg->m_dwKey, 
            pRecvMsg->m_CharGuidFrom );
        return;
    }

    // ģ�� ���� ������ �����ϵ��� ��� Ǭ��
    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuidFrom );

    if( pUser )
    {
        pUser->SetFriendTransaction( FALSE );
    }

    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK	nakMsg;

    nakMsg.m_dwCharGuid = pRecvMsg->m_CharGuidFrom;
    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

    pAgent->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

    MessageOut( eFULL_LOG, "DW_CHAO_PK_REVENGE_CHAR_NAK[Channel:%u][CharGuid:%u][Error:%u]", pRecvMsg->m_dwKey, pRecvMsg->m_CharGuidFrom, 
        pRecvMsg->m_byErrorCode );
}

// ���� ����
VOID Handler_DW::OnDW_MEMO_SEND_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_SEND_ACK* pRecvMsg = (MSG_DW_MEMO_SEND_ACK*)pMsg;
    if (FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize) == false) {
        return;
    };
    const TCHAR* senderName = "System"; // �ý��� ������ ����Ʈ �۽��� �̸�
    TCHAR receiverName[MAX_CHARNAME_LENGTH + 1];

    _tcsncpy(receiverName, pRecvMsg->m_ptszRecvName, MAX_CHARNAME_LENGTH);
    receiverName[MAX_CHARNAME_LENGTH] = '\0';

    // �ý��� ������ �ƴϸ�
    if (pRecvMsg->m_SendGuid != 0)
    {
        User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_SendGuid );

        if( !pUser )
        {
            MessageOut( eFULL_LOG, "DW_MEMO_SEND_ACK Error - No User[%u]", pRecvMsg->m_SendGuid );
            return;
        }
        senderName = pUser->GetCharName().c_str();

        pUser->SetMemoTransaction( FALSE );

        MSG_CW_MEMO_SEND_ACK ackMsg;

        _tcsncpy( ackMsg.m_ptszRecvName, receiverName, MAX_CHARNAME_LENGTH );
        ackMsg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';

        pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );
    }
    else
    {
        // �ý��� �����̸� Ÿ�Կ� ���� �۽��� �̸��� �ٸ��� ������ �� �ִ�
        std::string s;
        s.assign(pRecvMsg->m_Subject.m_ptszSubject);
        std::string::size_type pos = s.find('|');
        if (pos != std::string::npos) {
            std::string t = s.substr(0, pos);
            int type = atoi(t.c_str());
            switch (type)
            {
            // added filter to support _KR_0_20091021_FIRENDINVITATION_EXTEND //�ý��� ���� �̸� ����
			case SYSMEMO_FRIENDINVITATION_INVATES_TART:
			case SYSMEMO_FRIENDINVITATION_INVATE_END:
			case SYSMEMO_FRIENDINVITATION_INVATE_PRESENT:
			case SYSMEMO_FRIENDINVITATION_BEINVATED_START:
			case SYSMEMO_FRIENDINVITATION_BEINVATED_END:
			case SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT:
				senderName = "Event Manager";
				break;
            //
            //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
            case SYSMEMO_LOTTO_WIN:
            case SYSMEMO_LOTTO_WIN_NEW:
                senderName = "Lotto";
                break;
            //}
            //__KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
            case SYSMEMO_GROUP_MGR:
                senderName = "Group";
                break;
            case SYSMEMO_DOMINATION_RETURN_MONEY:
            case SYSMEMO_DOMINATION_REWARD_MONEY:
            case SYSMEMO_DOMINATION_RETURN_MONEY_NEW:
            case SYSMEMO_DOMINATION_REWARD_MONEY_NEW:
                senderName = "Domination";
                break;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            case SYSMEMO_WORLD_RANKING_GRADE:
                senderName = "WorldRanking";
                break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            case SYSMEMO_GUILD_JOIN_REQUEST:
                senderName = "GuildJoinReq";
                break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
            case SYSMEMO_SUN_RANKING_SETTLE_RESULT:
                senderName = "SunRanking";
                break;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
            default:
                SUNLOG(eCRITICAL_LOG, "Unknown system memo type : %d", type);
                break;
            }
            // changed logic to support _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
            // �ý��� ���� �߼� �α�
            User* recipient = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_RecvGuid);
            TCHAR additional_info[MAX_ADDINFO_SIZE + 1];
            _sntprintf(additional_info, MAX_ADDINFO_SIZE, "%I64u %s", 
                pRecvMsg->m_Subject.m_MemoGuid, pRecvMsg->m_Subject.m_ptszSubject);
            additional_info[MAX_ADDINFO_SIZE] = '\0';
            GAMELOG->LogAction(ACT_SYSTEM_MEMO_RECEIVE, recipient, receiverName, additional_info);

            SUNLOG(eMIDDLE_LOG, "[Memo] Sent system memo to character '%s' : %s", 
                receiverName, additional_info);
        }
    }

    User *pTarget = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( pTarget )
    {
        MSG_CW_MEMO_NEWMEMO_BRD	brdMsg;

        _tcsncpy( pRecvMsg->m_Subject.m_SendName, senderName, MAX_CHARNAME_LENGTH );
        pRecvMsg->m_Subject.m_SendName[MAX_CHARNAME_LENGTH] = '\0';
        memcpy( &brdMsg.m_Subject, &pRecvMsg->m_Subject, sizeof(STRUCT_SUBJECT_PACKETINFO) );

        pTarget->SendPacket( (BYTE*)&brdMsg, sizeof(brdMsg) );
    }

    MessageOut( eFULL_LOG, "DW_MEMO_SEND_ACK[%s][%u]->[%s]", senderName, pRecvMsg->m_SendGuid, receiverName );
}

VOID Handler_DW::OnDW_MEMO_SEND_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_SEND_NAK* pRecvMsg = (MSG_DW_MEMO_SEND_NAK*)pMsg;

    // �ý��� ������ �ƴϸ�
    if (pRecvMsg->m_SendGuid != 0)
    {
        User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_SendGuid );

        if( !pUser )
        {
            MessageOut( eFULL_LOG, "DW_MEMO_SEND_NAK Error - No User[%u]", pRecvMsg->m_SendGuid );
            return;
        }

        pUser->SetMemoTransaction( FALSE );

        MSG_CW_MEMO_SEND_NAK nakMsg;

        // 20061124_YARU_���� �����ڵ� �߰��� ���� ����
        if(pRecvMsg->m_byErrorCode == 2)	nakMsg.m_byErrorCode = RC::RC_MEMO_NOUSER;
        else	nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

        pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

        MessageOut( eFULL_LOG, "DW_MEMO_SEND_NAK[%s][%u][Error:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid(), nakMsg.m_byErrorCode );
    }
}

VOID Handler_DW::OnDW_MEMO_LIST_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_LIST_ANS* pRecvMsg = (MSG_DW_MEMO_LIST_ANS*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_LIST_ANS Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    MSG_CW_MEMO_LIST_ANS sendMsg;

    sendMsg.m_byMemoCnt = pRecvMsg->m_byMemoCnt;
    sendMsg.m_byMemoPage = pRecvMsg->m_byMemoPage;
    sendMsg.m_byMemoPageCnt = pRecvMsg->m_byMemoPageCnt;
    memcpy( sendMsg.m_Subject, pRecvMsg->m_Memo, sizeof(STRUCT_SUBJECT_PACKETINFO) * pRecvMsg->m_byMemoCnt );

    pUser->SendPacket( (BYTE*)&sendMsg, sendMsg.GetSize() );

    if( sendMsg.m_byMemoPage >= sendMsg.m_byMemoPageCnt )
    {
        pUser->SetMemoTransaction( FALSE );
    }

    MessageOut( eFULL_LOG, "DW_MEMO_LIST_ANS[%s][%u][Memo:%u][Page:%u/%u]", pUser->GetCharName().c_str(), 
        pUser->GetCharGuid(), 
        sendMsg.m_byMemoCnt, 
        sendMsg.m_byMemoPage, 
        sendMsg.m_byMemoPageCnt );
}

VOID Handler_DW::OnDW_MEMO_LIST_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_LIST_NAK* pRecvMsg = (MSG_DW_MEMO_LIST_NAK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_LIST_NAK Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    pUser->SetMemoTransaction( FALSE );

    MSG_CW_MEMO_LIST_NAK nakMsg;

    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

    MessageOut( eFULL_LOG, "DW_MEMO_LIST_NAK[%s][%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
}

VOID Handler_DW::OnDW_MEMO_VIEW_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_VIEW_ANS* pRecvMsg = (MSG_DW_MEMO_VIEW_ANS*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_VIEW_ANS Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    pUser->SetMemoTransaction( FALSE );

    MSG_CW_MEMO_VIEW_ANS sendMsg;

    sendMsg.m_MemoGuid = pRecvMsg->m_MemoGuid;
    _tcsncpy( sendMsg.m_ptszMemo, pRecvMsg->m_ptszMemo, MAX_MEMO_LENGTH );
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    sendMsg.m_typeSystemMemo = pRecvMsg->m_typeSystemMemo;
    sendMsg.m_recvItemCode = pRecvMsg->m_recvItemCode;
    sendMsg.m_amountCost = pRecvMsg->m_amountCost;
    //}
    pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    if (pRecvMsg->m_typeSystemMemo != NORMAL_MEMO)
    {
        SUNLOG(eMIDDLE_LOG, "[Memo] Read system memo %I64u of character '%s' : Code=%d Amount=%I64u", 
            pRecvMsg->m_MemoGuid, pUser->GetCharName().c_str(), pRecvMsg->m_recvItemCode, pRecvMsg->m_amountCost);
    }
    //}
    MessageOut( eFULL_LOG, "DW_MEMO_VIEW_ANS[%s][%u][Memo:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid(), sendMsg.m_MemoGuid );
}

VOID Handler_DW::OnDW_MEMO_VIEW_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_VIEW_NAK* pRecvMsg = (MSG_DW_MEMO_VIEW_NAK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_VIEW_NAK Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    pUser->SetMemoTransaction( FALSE );

    MSG_DW_MEMO_VIEW_NAK nakMsg;

    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

    MessageOut( eFULL_LOG, "DW_MEMO_VIEW_NAK[%s][%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
}

VOID Handler_DW::OnDW_MEMO_DELETE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_DELETE_ACK* pRecvMsg = (MSG_DW_MEMO_DELETE_ACK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    // added logic to support _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    int log_type = 0;
    if (pRecvMsg->m_MemoGuid == 0)
    {
        log_type = ACT_ALL_MEMO_DELETE; // ��ü ���� �ϰ� ����
        SUNLOG(eMIDDLE_LOG, "[Memo] Deleted all memo of character %d", pRecvMsg->m_RecvGuid);
    }
    else if (pRecvMsg->m_MemoType ==  STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
    {
        log_type = ACT_SYSTEM_MEMO_DELETE; // �ý��� ���� ����
        SUNLOG(eMIDDLE_LOG, "[Memo] Deleted system memo %I64u of character %d", pRecvMsg->m_MemoGuid, pRecvMsg->m_RecvGuid);
    }
    if (log_type != 0)
    {
        // ���� ���� �α�
        TCHAR additional_info[MAX_ADDINFO_SIZE + 1];
        _sntprintf(additional_info, MAX_ADDINFO_SIZE, "%I64u CharID:%d", pRecvMsg->m_MemoGuid, pRecvMsg->m_RecvGuid);
        additional_info[MAX_ADDINFO_SIZE] = '\0';
        GAMELOG->LogAction(log_type, pUser, NULL, additional_info);
    }

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_DELETE_ACK Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    pUser->SetMemoTransaction( FALSE );

    MSG_CW_MEMO_DELETE_ACK ackMsg;

    ackMsg.m_MemoGuid = pRecvMsg->m_MemoGuid;

    pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );

    MessageOut( eFULL_LOG, "DW_MEMO_DELETE_ACK[%s][%u][Memo:%u]", pUser->GetCharName().c_str(), 
        pUser->GetCharGuid(), 
        ackMsg.m_MemoGuid );
}

VOID Handler_DW::OnDW_MEMO_DELETE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MEMO_DELETE_NAK* pRecvMsg = (MSG_DW_MEMO_DELETE_NAK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvGuid );

    if( !pUser )
    {
        MessageOut( eFULL_LOG, "DW_MEMO_DELETE_NAK Error - No User[%u]", pRecvMsg->m_RecvGuid );
        return;
    }

    pUser->SetMemoTransaction( FALSE );

    MSG_CW_MEMO_DELETE_NAK nakMsg;

    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

    MessageOut( eFULL_LOG, "DW_MEMO_DELETE_NAK[%s][%u][Error:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid(), nakMsg.m_byErrorCode );
}


VOID Handler_DW::OnDW_STATUS_INTERCEPTION_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_STATUS_INTERCEPTION_ANS *pRecvMsg = (MSG_DW_STATUS_INTERCEPTION_ANS *)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_CharGuid);
    if(!pUser)	return;	//gameserver�� ���� �� �̰����� gameserver�� ������ ��û����

    pUser->SetWhisper(pRecvMsg->m_bWhisperFlag);
}

VOID Handler_DW::OnDW_STATUS_INTERCEPTION_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_STATUS_INTERCEPTION_NAK *pRecvMsg = (MSG_DW_STATUS_INTERCEPTION_NAK *)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_CharGuid);
    if(!pUser)	return;

    MessageOut( eFULL_LOG, "DW_STATUS_INTERCEPTION_NAK_[CHARGUID:%u]", pRecvMsg->m_CharGuid);
}

VOID Handler_DW::OnDW_GUILD_WAREHOUSE_LOGLIST_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS* pRecvMsg = (MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_ReqCharGuid);

    if( !pUser )
    {
        SUNLOG(eDEV_LOG, "DW_GUILD_WAREHOUSE_LOGLIST_ANS Error - No User[%u]", pRecvMsg->m_ReqCharGuid);
        return;
    }

    MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS msg;
    msg.m_byLogCount = pRecvMsg->m_byLogCount;
    msg.m_byCurrentPage = pRecvMsg->m_byCurrentPage;
    msg.m_byLogPageCount = pRecvMsg->m_byLogPageCount;
    memcpy(msg.m_LogList, pRecvMsg->m_LogList, sizeof(STRUCT_GUILDWAREHOUSE_LOGINFO) * pRecvMsg->m_byLogCount);
    pUser->SendPacket((BYTE*)&msg, msg.GetSize());

    //if( msg.m_byMemoPage >= msg.m_byMemoPageCnt )
    //{
    //	pUser->SetMemoTransaction( FALSE );
    //}

#ifdef _DEBUG
    SUNLOG(eDEV_LOG, "DW_GUILD_WAREHOUSE_LOGLIST_ANS[%s][%u][Memo:%u][Page:%u/%u]", pUser->GetCharName().c_str(), 
        pUser->GetCharGuid(), 
        msg.m_byLogCount, 
        msg.m_byCurrentPage, 
        msg.m_byLogPageCount);
#endif
}

VOID Handler_DW::OnDW_GUILD_WAREHOUSE_LOGLIST_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GUILD_WAREHOUSE_LOGLIST_NAK* pRecvMsg = (MSG_DW_GUILD_WAREHOUSE_LOGLIST_NAK*)pMsg;

    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_ReqCharGuid );
    if(!pUser)
    {
#ifdef _DEBUG
        SUNLOG(eDEV_LOG, "DW_GUILD_WAREHOUSE_LOGLIST_NAK Error - No User[%u]", pRecvMsg->m_ReqCharGuid);
#endif
        return;
    }

    //pUser->SetMemoTransaction( FALSE );

    MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK nakMsg;
    nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;
    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

#ifdef _DEBUG
    SUNLOG(eDEV_LOG, "DW_MEMO_LIST_NAK[%s][%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid());
#endif
}


VOID Handler_DW::OnDW_GUILD_SELECT_NOTICE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    PACKET_PTR_CONVERT(MSG_DW_GUILD_SELECT_NOTICE_ACK, pRecvMsg, pMsg);
    User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_OperCharGuid);
    if( !pUser )
    {
        SUNLOG(eDEV_LOG, "DW_GUILD_WAREHOUSE_LOGLIST_ANS Error - No User[%u]", pRecvMsg->m_OperCharGuid);
        return;
    }

    WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);
    if(pGuild)
    {
        pGuild->SetGuildNotice(pRecvMsg->m_szGuildNotice);

        MSG_CW_GUILD_SELECT_NOTICE_ACK msg;
        msg.m_NoticeType = pRecvMsg->m_GuildNoticeType;
        msg.m_wNoticeLen = strlen(pGuild->GetGuildNotice());
        memcpy(msg.m_ptszNotice, pGuild->GetGuildNotice(), sizeof(TCHAR)*MAX_GUILDNOTICE_LENGTH );
        msg.m_ptszNotice[MAX_GUILDNOTICE_LENGTH]='\0';
        pUser->SendPacket( (BYTE*)&msg, sizeof(msg) );
        return;
    }

    MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK nakMsg;
    nakMsg.m_byErrorCode = RC::RC_GUILD_INVALID_STATE;
    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
}

VOID Handler_DW::OnDW_GUILD_SELECT_NOTICE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    PACKET_PTR_CONVERT(MSG_DW_GUILD_SELECT_NOTICE_NAK, pRecvMsg, pMsg);/*
                                                                       User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_OperCharGuid);
                                                                       if( !pUser )
                                                                       {
                                                                       SUNLOG(eDEV_LOG, "OnDW_GUILD_SELECT_NOTICE_NAK Error - No User[%u]", pRecvMsg->m_OperCharGuid);
                                                                       return;
                                                                       }

                                                                       MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK nakMsg;
                                                                       nakMsg.m_byErrorCode = RC::RC_GUILD_INVALID_STATE;
                                                                       pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );*/
}

//{__NA001254_20090109_RTTG_FUNC_ADD
// ��Ƽ ����
VOID Handler_DW::OnDW_MULTIMEMO_SEND_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_MULTIMEMO_SEND_ACK* pRecvMsg = (MSG_DW_MULTIMEMO_SEND_ACK*)pMsg;

    ServerSession *pMasterServer = WorldServer::Instance()->GetMasterServerSession();

    if( pMasterServer )
    {
        MSG_MW_MULTIMEMO_SEND_ACK sendMsg;

        sendMsg.m_dwKey = pRecvMsg->m_dwKey;
        sendMsg.m_dwCount = pRecvMsg->m_dwCount;
        for( DWORD idx = 0; idx < pRecvMsg->m_dwCount; ++idx )
            sendMsg.m_arrResultInfoList[idx] = pRecvMsg->m_arrResultInfoList[idx].m_ErrResult;

        pMasterServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MW_MULTIMEMO_SEND_ACK) );
    }

    //.//

    for( DWORD idx = 0; idx < pRecvMsg->m_dwCount; ++idx)
    {
        User *pTarget = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_arrResultInfoList[idx].m_RecvGuid );

        if( pTarget )
        {
            MSG_CW_MEMO_NEWMEMO_BRD	brdMsg;

            brdMsg.m_Subject.m_byRead = 0;
            brdMsg.m_Subject.m_MemoGuid = pRecvMsg->m_arrResultInfoList[idx].m_MemoGuid;
            brdMsg.m_Subject.m_MemoType = STRUCT_SUBJECT_PACKETINFO::NORMAL_MEMO;
            _tcsncpy( brdMsg.m_Subject.m_SendName, pRecvMsg->m_SendName, MAX_CHARNAME_LENGTH );
            brdMsg.m_Subject.m_tszCreateDate[MAX_CHARNAME_LENGTH]='\0';

            _tcsncpy( brdMsg.m_Subject.m_tszCreateDate, pRecvMsg->m_arrResultInfoList[idx].m_tszCreateDate, 20 );
            brdMsg.m_Subject.m_tszCreateDate[20]='\0';

            _tcsncpy( brdMsg.m_Subject.m_ptszSubject, pRecvMsg->m_arrResultInfoList[idx].m_ptszSubject, MAX_SUBJECT_LENGTH );
            brdMsg.m_Subject.m_ptszSubject[MAX_SUBJECT_LENGTH] = '\0';

            pTarget->SendPacket( (BYTE*)&brdMsg, sizeof(brdMsg) );
        }
    }
}
//}__NA001254_20090109_RTTG_FUNC_ADD

//////////////////////////////////////////////////////////////////////////
//�׷� �α� �ý���
//////////////////////////////////////////////////////////////////////////
//�׷� �ɹ� �߰� ���� ��Ŷ ó�� : ���� ����� �׷� ������ ���� �ɹ��� �߰��ϰ� ������ ������� ������ �˸���.
VOID Handler_DW::OnDW_GROUP_ADD_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{  
    const MSG_DW_GROUP_ADD_ACK* pRecvMsg = static_cast<MSG_DW_GROUP_ADD_ACK*>(pMsg);

    //���� ������� ���� ����� ������ �˷��ش�.(�ɹ� �߰� ��û�� �����ߴ� m_tempMemberInfo ���)
    //[2010.02.26]gameluluWorking : m_tempMemberInfo���뿡 ���� �ŷڵ��� ���� ���� �ذ� ����
    User* pUser = UserManager::Instance()-> FindUserByCharGuid(pRecvMsg->m_fromCharGUID);
    if (pUser) 
    {
        const STRUCT_GROUP_MEMBER_INFO UserMemberInfo = *(pUser->GetStructMemberInfo());

        GroupManager::Instance()->AddMemberInfo(pUser, UserMemberInfo);
        GroupManager::Instance()->UpdateGroupInfo(pUser);
        GroupManager::Instance()->SendGroupMemberInfo(pUser, UserMemberInfo, GROUP_LEVEL_LOW);  //_NA_0_20100527_ADD_CHAR_RELATION_MEMO ���� ����

        pUser->SetGroupTransaction(FALSE);
    }

    //���� ������� ���� ����� ������ �˷��ش�.
    User* toUser = UserManager::Instance()-> FindUserByCharGuid(pRecvMsg->m_toCharGUID);
    if (toUser) 
    {
        const STRUCT_GROUP_MEMBER_INFO toUserMemberInfo = *(toUser->GetStructMemberInfo());

        GroupManager::Instance()->AddMemberInfo(toUser, toUserMemberInfo);
        GroupManager::Instance()->UpdateGroupInfo(toUser);
        GroupManager::Instance()->SendGroupMemberInfo(toUser, toUserMemberInfo, GROUP_LEVEL_HIGH);

        toUser->SetGroupTransaction(FALSE);
    }
}
//�׷� �ɹ� �߰� ���� ��Ŷ ó��
VOID Handler_DW::OnDW_GROUP_ADD_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GROUP_ADD_NAK* pRecvMsg = (MSG_DW_GROUP_ADD_NAK*)pMsg;

    User* fromUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_fromCharGUID );
    if( !fromUser ) 
        return;

    fromUser->SetGroupTransaction( FALSE );

    User* toUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_toCharGUID );
    if( !toUser ) 
        return;

    toUser->SetGroupTransaction( FALSE );

    MSG_CW_GROUP_ADD_NAK sendMsg;
    sendMsg.m_byResult = MSG_CW_GROUP_ADD_NAK::ERR_DEFAULT;

    fromUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
    MessageOut( eCRITICAL_LOG, "["__FUNCTION__"][%d]DB InsertFail", pRecvMsg->m_fromCharGUID );
}

//�׷� �ɹ� ���� ���� ��Ŷ ó��
VOID Handler_DW::OnDW_GROUP_DEL_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GROUP_DEL_ACK* pRecvMsg = (MSG_DW_GROUP_DEL_ACK*)pMsg;
 

    User* fromUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_fromCharGUID );
    if(fromUser)
    {
        //��û �������� ���� ����� �˸�
        MSG_CW_GROUP_DEL_ACK sendMsg;
        
        STRUCT_GROUP_MEMBER_INFO* fromUserInfo;//�����ϱ� ���� �ɹ� ������ �̿��Ͽ� ��û �������� ��������� �˸�

        fromUserInfo = GroupManager::Instance()->FindGroupMember( fromUser, pRecvMsg->m_toCharGUID );
        if(fromUserInfo)
        {
            memset( sendMsg.ptszMemeberName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );//��û�� �ɸ��� �̸�
            _tcsncpy( sendMsg.ptszMemeberName, fromUserInfo->GroupMemberInfo.ptszMemberName, MAX_CHARNAME_LENGTH );
            sendMsg.ptszMemeberName[MAX_CHARNAME_LENGTH] = '\0';

            fromUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_DEL_ACK) );
            
            GroupManager::Instance()->RemoveMemberInfo( fromUser, pRecvMsg->m_toCharGUID);
        }
        else
        {
            MessageOut( eCRITICAL_LOG, "OnDW_GROUP_DEL_ACK Error - Not Found Member[fromUserInfo]" );
        }
        GroupManager::Instance()->UpdateGroupInfo( fromUser );
    }

    User* toUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_toCharGUID );
    if(toUser) 
    {
        //������ �������� ���� ����� �˸�
        MSG_CW_GROUP_DEL_BRD brdMsg;

        STRUCT_GROUP_MEMBER_INFO* toUserInfo;//������ �ɹ� ������ ����
        toUserInfo = GroupManager::Instance()->FindGroupMember( toUser, pRecvMsg->m_fromCharGUID );

        if( toUserInfo )
        {
            memset( brdMsg.ptszMemeberName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );//��û�� �ɸ��� �̸�
            _tcsncpy( brdMsg.ptszMemeberName,  toUserInfo->GroupMemberInfo.ptszMemberName, MAX_CHARNAME_LENGTH );
            brdMsg.ptszMemeberName[MAX_CHARNAME_LENGTH] = '\0';

            toUser->SendPacket( (BYTE*)&brdMsg, sizeof(MSG_CW_GROUP_DEL_BRD) );

            GroupManager::Instance()->RemoveMemberInfo( toUser, pRecvMsg->m_fromCharGUID);
        }
        else
        {
            MessageOut( eCRITICAL_LOG, "OnDW_GROUP_DEL_ACK Error - Not Found Member[toUserInfo]" );
        }

        GroupManager::Instance()->UpdateGroupInfo( toUser );
    }
}

//�׷� �ɹ� ���� ���� ��Ŷ ó��
VOID Handler_DW::OnDW_GROUP_DEL_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_GROUP_DEL_NAK* pRecvMsg = (MSG_DW_GROUP_DEL_NAK*)pMsg;

    User* fromUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_fromCharGUID );
    if( !fromUser ) 
        return;

    fromUser->SetGroupTransaction( FALSE );

    User* toUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_toCharGUID );
    if( !toUser )
        return;

    toUser->SetGroupTransaction( FALSE );

    MSG_CW_GROUP_DEL_NAK sendMsg;

    memset( sendMsg.ptszMemeberName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );//��û�� �ɸ��� �̸�
    _tcsncpy( sendMsg.ptszMemeberName,  fromUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
    sendMsg.ptszMemeberName[MAX_CHARNAME_LENGTH] = '\0';

    sendMsg.m_byResult = pRecvMsg->m_byResult;

    fromUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_DEL_NAK) );
    fromUser->SetGroupTransaction( FALSE );
}

//�׷� �ɹ� ����Ʈ ��û ���� ��Ŷ ó��
VOID Handler_DW::OnDW_GROUP_LIST_ACK(GameDBProxySession *game_db_proxy, MSG_BASE *msg, WORD size)
{
    const MSG_DW_GROUP_LIST_ACK* recv_msg = static_cast<MSG_DW_GROUP_LIST_ACK*>(msg);

    User* user = UserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGUID);
    if (user == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]User Already Logout[%u]", recv_msg->m_CharGUID);
        return;
    }

    // (CHANGES) (f100425.3L) (WAVERIX) prevent buffer overrun process
    BOOST_STATIC_ASSERT(_countof(recv_msg->m_GroupInfo) == MAX_GROUP_MEMBER);
    const bool failed__number_of_node_has_index_out_of_range = \
        recv_msg->m_DataNum > MAX_GROUP_MEMBER;
    assert(failed__number_of_node_has_index_out_of_range == false);

    const int number_of_node = failed__number_of_node_has_index_out_of_range
                             ?  _countof(recv_msg->m_GroupInfo)
                             :  recv_msg->m_DataNum;
    if (failed__number_of_node_has_index_out_of_range)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Group Member Count Over[%u][Count:%d]", 
               recv_msg->m_CharGUID, recv_msg->m_DataNum);
    }

    //-- �ɹ� ���� ����
    for (int i = 0; i < number_of_node; ++i)
    {
        STRUCT_GROUP_MEMBER_INFO Info = recv_msg->m_GroupInfo[i];

        User* pGroupMember = UserManager::Instance()->FindUserByCharGuid(Info.charGUID);

        if(pGroupMember != NULL)
        {
            //-- �¶��� ����
            Info.GroupMemberInfo.bOnline = TRUE;
            Info.GroupMemberInfo.byChannel = static_cast<BYTE>(pGroupMember->GetChannelID());
        }
        else
        {
            //-- �������� ����
            Info.GroupMemberInfo.bOnline = FALSE;
            Info.GroupMemberInfo.byChannel = 0;
        }

        if (GroupManager::Instance()->IsOverBonusDate(Info.lastDate, MAX_DATE_GROUPBONUS))
        {
            user->AddCurAccumEXP(Info.accumEXP); 
            SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not apply accumexp because date is over");
        }

        GroupManager::Instance()->SetMemberInfo(user, Info);
    }

    GroupManager::Instance()->UpdateGroupInfo(user);
    GroupManager::Instance()->SendAroundMember(user, GroupManager::LOG_IN);

    if (user->GetCurAccumEXP() > 0)
    {
        //���� ����ġ ȹ���� �˸�
        ServerSession* agent_server = 
            ServerSessionManager::Instance()->FindAgentServer(user->GetChannelID());

        if (agent_server == NULL)
        {
            MessageOut(eFULL_LOG, "["__FUNCTION__"]Can Not Found AgentServer");
            return;
        }

        MSG_AW_STATUS_GROUP_ADDEXP_CMD add_msg;
        {
            add_msg.m_CharGuid = user->GetCharGuid();
            add_msg.m_Amount = user->GetCurAccumEXP();
        }
        agent_server->Send((BYTE*)&add_msg, sizeof(add_msg));

        GroupManager::Instance()->SendAccumEXPMemo(user);
    }

    //���� ����ġ�� �ʱ�ȭ ��Ų��.
    MSG_DW_GROUP_INIT_ACCUMEXP_CMD init_msg;
    {
        init_msg.m_CharGUID = user->GetCharGuid();
    }
    game_db_proxy->Send((BYTE*)&init_msg, sizeof(init_msg));
}

VOID Handler_DW::OnDW_LOTTO_LOAD_ENTRY_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_LOTTO_LOAD_ENTRY_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_LOAD_ENTRY_ACK*>(pMsg);
    ASSERT(sizeof(*pRecvMsg) >= wSize);
    if (sizeof(*pRecvMsg) < wSize) { return; }

    Lotto::GetInstance().OnEntryLoad(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_LOAD_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnLoad(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_WINNER_LIST_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_LOTTO_WINNER_LIST_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_WINNER_LIST_ACK*>(pMsg);
    ASSERT(sizeof(*pRecvMsg) >= wSize);
    if (sizeof(*pRecvMsg) < wSize) { return; }

    Lotto::GetInstance().OnWinnerList(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_SAVE_ENTRY_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnEntrySave(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_ROLLOVER_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnRollOver(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_ADD_TICKET_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnTicketAcquisition(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_REWARD_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnReward(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_NOTICE_LIST_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_LOTTO_NOTICE_LIST_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_NOTICE_LIST_ACK*>(pMsg);
    ASSERT(sizeof(*pRecvMsg) >= wSize);
    if (sizeof(*pRecvMsg) < wSize) { return; }

    Lotto::GetInstance().OnNoticeList(pMsg);
}

VOID Handler_DW::OnDW_LOTTO_NOTICE_DEL_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    Lotto::GetInstance().OnNoticeDelete(pMsg);
}


VOID Handler_DW::OnDW_DOMINATION_FINDMONEY_ACK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize)
{
    MSG_DW_DOMINATION_FINDMONEY_ACK* pRecvMsg = static_cast<MSG_DW_DOMINATION_FINDMONEY_ACK*>(pMsg);

    User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_RecvCharGuid );

    if (pUser == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]User Already Logout[%u]", pRecvMsg->m_RecvCharGuid );
        return;
    }

    //���� ���� ���ϴ� ������ ���� �α� ��ȭ
    if(pRecvMsg->m_Result != 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]DB Query Result Fail", 
               pRecvMsg->m_RecvCharGuid);
        return;
    }
    else if(pRecvMsg->m_RecvCharGuid != pRecvMsg->m_DBCharGuid)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Guid Not Match[DB:%u]!=[REQ:%u]", 
            pRecvMsg->m_DBCharGuid, pRecvMsg->m_RecvCharGuid);
        return;
    }
    else if(pRecvMsg->m_CostType != pRecvMsg->m_DBCostType)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]Money Type Not Same[DB:%d]!=[REQ:%d]", 
               pRecvMsg->m_RecvCharGuid, pRecvMsg->m_DBCostType, pRecvMsg->m_CostType);
        return;
    }
    else if(pRecvMsg->m_RequestMapCode != pRecvMsg->m_DBMapCode)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]MapCode Not Match[DB:%u]!=[REQ:%u]", 
               pRecvMsg->m_RecvCharGuid, pRecvMsg->m_DBMapCode, pRecvMsg->m_RequestMapCode);
        return;
    }

    ServerSession* pAgentServer = 
        ServerSessionManager::Instance()->FindAgentServer(pUser->GetChannelID());

    if (pAgentServer == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No AgentServer");
    }
    else
    {
        MSG_AW_USER_DOMINATION_RETURNHIEM_CMD sendMsg;

        sendMsg.m_CharGuid = pRecvMsg->m_DBCharGuid;
        sendMsg.m_Type = pRecvMsg->m_CostType;
        sendMsg.m_TenderCost = pRecvMsg->m_TenderCost;
        pAgentServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]%d Type(return:9,reward:10)%d Heim Give to User[%d]",
            sendMsg.m_Type, sendMsg.m_TenderCost, sendMsg.m_CharGuid);
    }
}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
VOID Handler_DW::OnDW_NOTIFICATION_ITEM_SYN( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_NOTIFICATION_ITEM_SYN* pRecvMsg = (MSG_DW_NOTIFICATION_ITEM_SYN*)pMsg;
    if (FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize) == false) {
        return;
    };

    NotificationManager::Instance()->ProcessNotification(pRecvMsg->info);
}

VOID Handler_DW::OnDW_NOTIFICATION_CLEAR_SYN( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize )
{
    MSG_DW_NOTIFICATION_CLEAR_SYN* pRecvMsg = (MSG_DW_NOTIFICATION_CLEAR_SYN*)pMsg;
    if (FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize) == false) {
        return;
    };

    NotificationManager::Instance()->ClearNotification();
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM