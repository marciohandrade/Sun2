#include "StdAfx.h"
#include ".\handler_client.h"
#include ".\Player.h"
#include ".\PlayerManager.h"
#include <PacketStruct_CG.h>
#include <PacketStruct_GP.h>
#include ".\GameRankingManager.h"
#include ".\GameServerEx.h"
#include ".\ItemManager.h"
#include <SCItemSlotContainer.h>
#include <ArchiveOfStream.h>

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "GameZone.h"
#include "GameBattleResult.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/GameChaosZoneManaager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ExtraNPCManager.h>

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#include "SunRankingParser.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_Client::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_Client::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        HANDLER_NODE_INFO(CG_MISSION,  CG_MISSION_RANKING_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_MISSION,  CG_MISSION_RANKING_MEMBER_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_MISSION,  CG_MISSION_RANKING_REWARD_SYN, PHASE_SERVICE),
#endif // _NA_008012_20150130_RANKING_SYSTEM

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
        HANDLER_NODE_INFO(CG_ZONE,  CG_ZONE_PLAYER_BATTLE_RESULT_SYN, PHASE_SERVICE),


#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_CASH_REQ_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_ITEM_LIST_REQ_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_ITEM_REQ_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_ITEM_BUY_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_BUY_HISTORY_SYN, PHASE_SERVICE),

        //HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_PACKAGE_BUY_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_CHECK_CHAR_NAME_SYN, PHASE_SERVICE),
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_SEND_GIFT_RELAY_SYN, PHASE_SERVICE),
    #else
        HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_SEND_GIFT_SYN, PHASE_SERVICE),
    #endif
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_ACTION_RECV_GIFT_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_IS_RECV_GIFT_SYN, PHASE_SERVICE),
    HANDLER_NODE_INFO(CG_SHOP, CG_SHOP_RECV_GIFT_LIST_SYN, PHASE_SERVICE),
#endif //!defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        HANDLER_NODE_INFO(CG_CHARINFO, CG_HONORSYSTEM_PACKET, PHASE_SERVICE),
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        HANDLER_NODE_INFO(CG_CHARINFO, CG_POINTWALLET_PACKET, PHASE_SERVICE),
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        HANDLER_NODE_INFO(CG_PARTY, CG_CHAOS_ZONE_PACKET, PHASE_SERVICE),
#endif  // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        HANDLER_NODE_INFO(CG_MISSION,  CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN, PHASE_SERVICE),
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
        HANDLER_NODE_INFO(CG_CHARINFO, CG_CHARINFO_SMARTNPC_PACKET, PHASE_SERVICE),
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        HANDLER_NODE_INFO(CG_ETC, CG_ETC_SUN_RANKING_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ETC, CG_ETC_SUN_RANKING_SETTLE_INFO_SYN, PHASE_SERVICE),
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

#ifdef _NA_008012_20150130_RANKING_SYSTEM
Handler_Client_IMPL( CG_MISSION_RANKING_INFO_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_MISSION_RANKING_INFO_SYN * pMsg = (MSG_CG_MISSION_RANKING_INFO_SYN *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RankingManager::LEVEL_AREA* level_base = g_GameRankingMgr.FindLVArea_ID(pMsg->m_RankingID);
    if ( level_base == NULL || g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
    {
        MSG_CG_MISSION_RANKING_INFO_NAK nmsg;
        nmsg.m_dwErrorCode = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
        return;
    }	
#else
	if( g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL)
	{
		MSG_CG_MISSION_RANKING_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}	
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

    //ACK Message
	MSG_CG_MISSION_RANKING_INFO_ACK msg;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    msg.myranking = 0;
#else
    //결산일 패킷을 분리했으므로 주석처리한다.
    msg.m_SettledDate = g_GameRankingMgr.GetSettledDate();
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	msg.m_Size = sizeof(msg.m_pBUFFER);

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    // 랭킹 정보를 가져옴.
    switch(level_base->m_RankingType)
    {
    case eRANKING_TYPE_TIME_ATTACK:
        //미션 랭킹
        if (pMsg->m_Type == eRANKING_MYRANKING)
        {
            g_GameRankingMgr.SerializeForClient_MyRanking(pPlayer->GetCharName(), pMsg->m_RankingID, 
                                                          msg.m_pBUFFER, msg.m_Size, msg.myranking);
        }
        else
        {
            g_GameRankingMgr.SerializeForClient( (eRANKING_TYPE)pMsg->m_Type, level_base->m_MissionNo, 
                pMsg->m_Page, msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_CLIENT_LOAD );
        }
        break;
    case eRANKING_TYPE_POINT:
        //포인트 랭킹
        if (pMsg->m_Type == eRANKING_MYRANKING)
        {
            g_GameRankingMgr.Serialize_MyRanking(pPlayer->GetCharName(), pMsg->m_RankingID, msg.m_pBUFFER, 
                                                 msg.m_Size, msg.myranking);
        }
        else
        {
            g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, level_base->m_MissionNo, pMsg->m_Page,
                msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_LOAD );
        }
        break;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        //전장 승리
        //포인트 랭킹
        if (pMsg->m_Type == eRANKING_MYRANKING)
        {
            g_GameRankingMgr.Serialize_MyRanking(pPlayer->GetCharName(), pMsg->m_RankingID, msg.m_pBUFFER, 
                msg.m_Size, msg.myranking);
        }
        else
        {
            RANKINGTYPE ranking_type = static_cast<RANKINGTYPE>(level_base->m_RankingType);
            g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, level_base->m_MissionNo, pMsg->m_Page,
                msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_LOAD,  ranking_type);
        }
        break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    default:
        ASSERT(FALSE);
        return;
    }

#else
    //타임 어택
    if( g_GameRankingMgr.GetRankingType( pMsg->m_MissionNo ) == eRANKING_TYPE_TIME_ATTACK )
    {
        g_GameRankingMgr.SerializeForClient( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page,
            msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_CLIENT_LOAD );		
    }

    //포인트
    else if( g_GameRankingMgr.GetRankingType( pMsg->m_MissionNo ) == eRANKING_TYPE_POINT )
    {
        g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page,
            msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_LOAD );
    }
    else
    {
        ASSERT(FALSE);
        return;
    }
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	

	pPlayer->SendPacket( &msg, msg.GetSize() );
}

Handler_Client_IMPL( CG_MISSION_RANKING_MEMBER_INFO_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_MISSION_RANKING_MEMBER_INFO_SYN * pMsg = (MSG_CG_MISSION_RANKING_MEMBER_INFO_SYN *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	if( g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
	{
		MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //'내 순위'에 대한 멤버요청일 경우의 분기문을 적용
    MissionRankingRecord* pRecord = NULL;
    if (pMsg->m_Type == eRANKING_MYRANKING)
    {
        RankingManager::LEVEL_AREA* level_base = g_GameRankingMgr.FindLVArea(pMsg->m_MissionNo);
        if (level_base == NULL)
        {
            MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK nak_msg;
            nak_msg.m_dwErrorCode = RC::RC_RANKING_INVALID_STATE;
            pPlayer->SendPacket( &nak_msg, sizeof(nak_msg) );
            return;
        }

        pRecord = g_GameRankingMgr.FindMyBestRanking(pPlayer->GetCharName(), eRANKING_TODAYWEEK, 
            pMsg->m_MissionNo,(eRANKING_SYSTEM_TYPE)level_base->m_RankingType);
        if (pRecord == NULL)
        {
            MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK nak_msg;
            nak_msg.m_dwErrorCode = RC::RC_RANKING_INVALID_STATE;
            pPlayer->SendPacket( &nak_msg, sizeof(nak_msg) );
            return;
        }
    }
    else
    {
        pRecord = g_GameRankingMgr.FindMissionRecord( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Key );
        if ( pRecord == NULL )
        {
            MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK nak_msg;
            nak_msg.m_dwErrorCode = RC::RC_RANKING_INVALID_STATE;
            pPlayer->SendPacket( &nak_msg, sizeof(nak_msg) );
            return;
        }
    }
#else
	MissionRankingRecord * pRecord = g_GameRankingMgr.FindMissionRecord( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Key );
	if( !pRecord )
	{
		MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_RANKING_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

	MSG_CG_MISSION_RANKING_MEMBER_INFO_ACK msg;
	ArchiveOfStream lArchive(msg.m_pBUFFER, sizeof(msg.m_pBUFFER));
	pRecord->SerializeMember( lArchive, RANKING_SERIALIZE_CLIENT_LOAD );
	msg.m_Size = lArchive.GetSize();
	pPlayer->SendPacket( &msg, msg.GetSize() );
}

Handler_Client_IMPL( CG_MISSION_RANKING_REWARD_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_MISSION_RANKING_REWARD_SYN * pMsg = (MSG_CG_MISSION_RANKING_REWARD_SYN *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	//피로도 때문에 현재 보상을 받지 못하는 거라면...
	if( pPlayer->GetFatigueManager()->GetItemReward() == FALSE )
	{
		MSG_CG_MISSION_RANKING_REWARD_NAK msg;
		msg.m_dwErrorCode = RC::RC_RANKING_REWARD_FAIL_FATIGUE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

    //============================================================================================
	// 트랜젝션 체크?-> OK
	// portal 서버 연결 여부 체크
	// 보상을 받을 수 있는 상태 체크
	// 보상을 받을 수 있는 공간 체크
	// 보상 성공시 -> 아이템 보상 + 로그
	// 보상 실패시 -> 로그
	// 보상 실패시 -> db에는 보상을받았다고 나왔지만, 실제 보상을 받지 못한경우가 생길 수 있다.

	RC::eRANKING_RESULT rt = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
	if( g_pGameServer->IsConnected( RANKING_SERVER ) )
	{
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        RankingManager::LEVEL_AREA* level_area = g_GameRankingMgr.FindLVArea_ID(pMsg->ranking_id);
        if (level_area != NULL) 
        {
            rt = g_GameRankingMgr.DoReward( level_area->m_MissionNo, pMsg->m_Key, pPlayer->GetCharName(), 
                level_area->m_RankingType );
        }
        else // if (level_area == NULL)
        {
            rt = RC::RC_RANKING_FAILED;
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Error! RankingID[%d] Level_Area Not Exist!", pMsg->ranking_id);
        }
#else
        rt = g_GameRankingMgr.DoReward( pMsg->m_MissionNo, pMsg->m_Key, pPlayer->GetCharName() );
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
		if( rt == RC::RC_RANKING_SUCCESS )
		{
			if( pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetEmptySlotNum() > 1 )
			{
				MSG_GP_RANKING_MISSION_REWARD_SYN msg;
				msg.m_dwKey			= pMsg->m_dwKey;
				msg.m_Key			= pMsg->m_Key;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
                msg.ranking_type    = level_area->m_RankingType;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
                msg.m_MissionNo		= level_area->m_MissionNo;
#else
                msg.m_MissionNo		= pMsg->m_MissionNo;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
				_tcsncpy( msg.m_tszCharName, pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
				msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';

				g_pGameServer->SendToServer( RANKING_SERVER, &msg, msg.GetSize() );

				return ;
				//------------------------------------------------------------------------------------------------------------
				//------------------------------------------------------------------------------------------------------------
			}
			else
			{
				rt = RC::RC_RANKING_NOT_SPACE_FOR_REWARD;
				// TRANSACTION 취소
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
                g_GameRankingMgr.OnReward( FALSE, level_area->m_MissionNo, pMsg->m_Key, pPlayer->GetCharName(), 
                                           level_area->m_RankingType );
#else
                g_GameRankingMgr.OnReward( FALSE, pMsg->m_MissionNo, pMsg->m_Key, pPlayer->GetCharName() );
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
			}
		}
	}

	MSG_CG_MISSION_RANKING_REWARD_NAK msg;
	msg.m_dwErrorCode = rt;
	pPlayer->SendPacket( &msg, sizeof(msg) );

}

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
Handler_Client_IMPL( CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN * pMsg =
        static_cast<MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN*>(pRecvMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    ASSERT( player );
    if( player == NULL) return ;
    if( player->GetField() == NULL ) return;

    if ( g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
    {
        MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_NAK nak_msg;
        nak_msg.error_code = RC::RC_RANKING_NETWORK_ERROR_FOR_PORTAL;
        player->SendPacket( &nak_msg, sizeof(nak_msg) );
        return;
    }	

    MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK ack_msg;
    // 결산일
    ack_msg.settled_date = g_GameRankingMgr.GetSettledDate();
    //보상받을수있는 미션리스트
    g_GameRankingMgr.GetRewardableMissionList(player->GetCharName(), ack_msg.reward_mission_list);

    player->SendPacket( &ack_msg, sizeof(ack_msg) );
}

#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#endif // _NA_008012_20150130_RANKING_SYSTEM

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
Handler_Client_IMPL( CG_ZONE_PLAYER_BATTLE_RESULT_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_ZONE_PLAYER_BATTLE_RESULT_SYN * pMsg = (MSG_CG_ZONE_PLAYER_BATTLE_RESULT_SYN *)pRecvMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer ) 
        return;

    if( !pPlayer->GetField() ) 
    {
        MSG_CG_ZONE_PLAYER_BATTLE_RESULT_NAK message_nak;
        pPlayer->SendPacket(&message_nak, sizeof(message_nak));
        return;
    }

    GameZone* pZone = pPlayer->GetGameZonePtr();
    if( !pZone )
    {
        MSG_CG_ZONE_PLAYER_BATTLE_RESULT_NAK message_nak;
        pPlayer->SendPacket(&message_nak, sizeof(message_nak));
        return;
    }

    GameBattleResult* pBattleResult = pZone->GetGameBattleResult();
    if( !pBattleResult )
    {
        MSG_CG_ZONE_PLAYER_BATTLE_RESULT_NAK message_nak;
        pPlayer->SendPacket(&message_nak, sizeof(message_nak));
        return;
    }

    if( !pBattleResult->SendBattleResult(pPlayer, pMsg->m_OwnerGuild) )
    {
        MSG_CG_ZONE_PLAYER_BATTLE_RESULT_NAK message_nak;
        pPlayer->SendPacket(&message_nak, sizeof(message_nak));
    }
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
Handler_Client_IMPL(CG_HONORSYSTEM_PACKET)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));

    HonorSystem::Instance()->ParseCGPacket(pRecvMsg);
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
Handler_Client_IMPL(CG_POINTWALLET_PACKET)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_POINTWALLET_PACKET* packet = static_cast<MSG_CG_POINTWALLET_PACKET*>(pRecvMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return;

    PointWalletManager* const pointwallet_manager = player->GetPointWalletManager();
    pointwallet_manager->ParseCGPacket(packet);
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
Handler_Client_IMPL(CG_CHAOS_ZONE_PACKET)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    GameChaosZoneManager::Instance()->ParseCGPacket(pServerSession, pRecvMsg, wSize);
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
Handler_Client_IMPL(CG_CHARINFO_SMARTNPC_PACKET)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    g_ExtraNPCManager.ParseCGPacket(pRecvMsg);
}
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
Handler_Client_IMPL(CG_ETC_SUN_RANKING_SYN)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_ETC_SUN_RANKING_SYN* packet = static_cast<MSG_CG_ETC_SUN_RANKING_SYN*>(pRecvMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return;

    MSG_CG_ETC_SUN_RANKING_ACK msg;
    
    RC::eSUN_RANKING_RESULT result = SunRankingManager::Instance()->GetSunRankingInfos(
        msg, 
        packet->ranking_type, 
        player->GetCharGuid(),
        MSG_CG_ETC_SUN_RANKING_ACK::MAX_INFO_COUNT,
        packet->is_guild_ranking, 
        player->GetGuildGuid());
    if (result != RC::RC_SUN_RANKING_SUCCESS)
    {
        MSG_CG_ETC_SUN_RANKING_NAK nak_msg;
        nak_msg.error_code = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    player->SendPacket(&msg, sizeof(msg));
}

Handler_Client_IMPL(CG_ETC_SUN_RANKING_SETTLE_INFO_SYN)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN* packet =
        static_cast<MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN*>(pRecvMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return;

    MSG_CG_ETC_SUN_RANKING_SETTLE_INFO_ACK msg;
    msg.type = SunRankingParser::Instance()->GetSettleDateType();
    msg.day = SunRankingParser::Instance()->GetSettleDateDay();
    msg.time = SunRankingParser::Instance()->GetSettleDateTime();
    player->SendPacket(&msg, sizeof(msg));
}
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM