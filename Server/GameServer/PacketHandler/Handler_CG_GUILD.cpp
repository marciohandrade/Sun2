#include "StdAfx.h"
#include ".\handler_cg_guild.h"
#include <PacketStruct_CG.h>
#include <BitStream.hpp>
#include <SCItemSlot.h>
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "PlayerManager.h"
#include "Player.h"
#include <PacketStruct_GZ.h>
#include <PacketStruct_DG.h>
#include "GameInstanceDungeon.h"

#include <ProhibitionWordParser.h>

#include "GameGuildExtension.h"

#include "GameDominationField.h"
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
#include <ServerOptionParserEx.h>
#endif

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInfoParser.h"
#endif
//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_GUILD::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_GUILD::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CREATE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_DESTROY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_INVITE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_WITHDRAW_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_KICKPLAYER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_DONATE_UP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CONVERT_UP2GP_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD,  CG_GUILD_ADD_INFO_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RELATION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RELATION_ANSWER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RELATION_DELETE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RELATION_BOTH_AGAINST_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CHANGE_DUTY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CHANGE_MASTER_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CREATE_CORPS_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_DESTROY_CORPS_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_EMPTY_COMMANDER_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CHANGE_COMMANDER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CHANGE_CORPS_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_CREATE_REAR_CAMP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_DESTROY_REAR_CAMP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_INVITE_ANSWER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_JOIN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_TOTALINFO_REQ_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_RECOVERY_GUILD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_WAREHOUSE_OPEN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_WAREHOUSE_MONEY_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_NAME_CHANGE_SYN, PHASE_SERVICE),
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ, PHASE_SERVICE),
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_INTRODUCTION_SAVE_SYN, PHASE_SERVICE),
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_JOIN_REQUEST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_JOIN_REQUEST_APPROVE_SYN,PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_JOIN_REQUEST_REJECT_SYN,PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_FACILITY_UPGRADE_SYN,PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_GUILD, CG_GUILD_FACILITY_ACTIVE_SYN,PHASE_SERVICE),
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CREATE_SYN* receive_message = static_cast<MSG_CG_GUILD_CREATE_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);
	Player* player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	ASSERT(player != NULL);
	if (player == NULL)
    {
        return;
    }
    
	if (!player->GetField())
	{
		SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_SYN )] Not Exist GameField"));
		return;
	}

	MONEY create_guild_require_heim = 0;
	RC::eGUILD_RESULT can_create_guild_result = g_GameGuildManager.CanCreateGuild(player, 
        create_guild_require_heim);

    // (BUGFIX) (WAVERIX) (091103) 필터링 추가
//#if defined(_ENGLAND) || defined(_USA) || defined(_GLOBAL)
    util::cSTRING_REF_RW_WITH_BUFFER<MAX_GUILDNAME_LENGTH + 1> guild_name;
    guild_name.CopyFromString(receive_message->m_tszGuildName);
    guild_name.MakeSafeString();
    if (can_create_guild_result == RC::RC_GUILD_SUCCESS) 
    {
        ProhibitionWordParser* prohibition_filter = ProhibitionWordParser::Instance();
        #ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
            BYTE Language = ServerOptionParserEx::Instance()->GetServerOption().m_byLanguageOfFilter;
            if (prohibition_filter->VerifyCharName(guild_name, Language) == 0)
            {
                can_create_guild_result = RC::RC_GUILD_NAME_BAN_WORD;
            }
        #else
            if (prohibition_filter->VerifyCharName(guild_name) == 0)
            {
                can_create_guild_result = RC::RC_GUILD_NAME_BAN_WORD;
            }
        #endif
    }
//#endif

	if (can_create_guild_result != RC::RC_GUILD_SUCCESS)
	{
		MSG_CG_GUILD_CREATE_NAK send_message;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, can_create_guild_result, BYTE));
		send_message.m_byErrorCode = static_cast<BYTE>(can_create_guild_result);
		player->SendPacket(&send_message,sizeof(send_message));
		return;
	}

	MSG_GZ_GUILD_CREATE_SYN send_message;
    _tcsncpy(send_message.m_tszGuildName, guild_name.GetBUFFER(),
             _countof(send_message.m_tszGuildName)); // guild_name is null terminated
    send_message.m_MasterCharGuid = player->GetCharGuid();
    _tcsncpy(send_message.m_tszMasterCharName, player->GetCharName(),
             _countof(send_message.m_tszMasterCharName));
    send_message.m_tszMasterCharName[_countof(send_message.m_tszMasterCharName) - 1] = _T('\0');
    const eCHAR_TYPE character_type = player->GetCharType();
    ASSERT(SAFE_NUMERIC_TYPECAST(eCHAR_TYPE, character_type, BYTE));
    send_message.m_byMasterClass = static_cast<BYTE>(character_type);
	send_message.m_MasterLV	= player->GetLevel();
	
    if (player->SendToGuildServer(&send_message, sizeof(send_message)) == FALSE)
	{
		MSG_CG_GUILD_CREATE_NAK send_message;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, RC::RC_GUILD_DISCONNECTED_GUILD_SERVER, BYTE));
		send_message.m_byErrorCode = static_cast<BYTE>(RC::RC_GUILD_DISCONNECTED_GUILD_SERVER);
		player->SendPacket(&send_message, sizeof(send_message));
		return;
	}

	// 돈 미리 빼준다 - 만에 하나 DB까지 갔다 오는 사이에 해당 플레이어가 나갔을 때를 대비한다
	player->MinusMoney(create_guild_require_heim);	
}

Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_DESTROY_SYN* receive_message = static_cast<MSG_CG_GUILD_DESTROY_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_SYN )|MSG_CG_GUILD_DESTROY_SYN 받음|PlayerKey:%u|"), 
        receive_message->m_dwKey);
#endif // _USING_GUILD_DESTROY_LOGGING
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	ASSERT(player != NULL);
    if (player == NULL)
    {
        return;
    }
    
	if (!player->GetField())
	{
		SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_SYN )] Not Exist GameField"));
		return;
	}

	RC::eGUILD_RESULT can_destroy_guild_result = g_GameGuildManager.CanDestroyGuild(player);
	if (can_destroy_guild_result != RC::RC_GUILD_SUCCESS)
	{
		MSG_CG_GUILD_DESTROY_NAK send_message;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, can_destroy_guild_result, BYTE));
		send_message.m_byErrorCode = static_cast<BYTE>(can_destroy_guild_result);
#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산신청|Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_SYN )|MSG_CG_GUILD_DESTROY_NAK 보냄|eGUILD_RESULT:%d|"), 
            send_message.m_byErrorCode);
#endif // _USING_GUILD_DESTROY_LOGGING
		player->SendPacket(&send_message, sizeof(send_message));
		return;
	}

	MSG_GZ_GUILD_DESTROY_SYN send_message;
	send_message.m_GuildGuid = player->GetCharInfo()->m_GuildGuid;
	send_message.m_MasterCharGuid = player->GetCharGuid();
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    send_message.m_DestroyWaitTime = 
        GuildInfoParser::Instance()->GetGuildInfo().time_of_disbanded_latency_;
#else
    send_message.m_DestroyWaitTime = GUILD_DESTROY_WAIT_TIME;
#endif
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(
        eCRITICAL_LOG, 
        __FUNCTION__" : Request guild destroy : GuildGuid=%d, MasterCharGuid=%d, DestroyWaitTime=%u|", 
        send_message.m_GuildGuid, 
        send_message.m_MasterCharGuid, 
        send_message.m_DestroyWaitTime
        );
#endif // _USING_GUILD_DESTROY_LOGGING

	player->SendToGuildServer(&send_message, sizeof(send_message));
}

//탈퇴
Handler_CG_GUILD_IMPL( CG_GUILD_WITHDRAW_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_WITHDRAW_SYN * pMsg = (MSG_CG_GUILD_WITHDRAW_SYN *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) 
        return;

    GameField* pGameField = pPlayer->GetField();
    if (!pGameField) 
        return;

	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )
	{
		MSG_CG_GUILD_WITHDRAW_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	// 길드 창고 사용자일 경우
	Player* pGuildWarehouseUsedPlayer = pGuild->GetGuildWareHouse()->GetUsedPlayer();
	if(pGuildWarehouseUsedPlayer && 
		pGuildWarehouseUsedPlayer->GetCharGuid() == pPlayer->GetCharGuid())
	{
		MSG_CG_GUILD_WITHDRAW_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

    // 지역 점령전 중일 경우
    GameZone* pGameZone = pPlayer->GetGameZonePtr();
    if (pGameZone == NULL) {
        return;
    }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    ChaosZoneType chaoszone_type = kChaosZoneTypeNone;
    if (eZONETYPE_INSTANCE == pGameZone->GetZoneType())
    {
        GameInstanceDungeon* indun = static_cast<GameInstanceDungeon*>(pGameZone);
        chaoszone_type = indun->GetChaosZoneType();
    }

    if (eZONETYPE_DOMINATION_FIELD == pGameZone->GetZoneType() ||
        eCHAOSZONE_TYPE_SONNENSCHEIN == chaoszone_type ||
		eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION == chaoszone_type)
    {
        MSG_CG_GUILD_WITHDRAW_NAK nmsg;
        nmsg.m_byErrorCode = RC::RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD;
        pPlayer->SendPacket(&nmsg,sizeof(nmsg));
        return;
    }
#else

    if (eZONETYPE_DOMINATION_FIELD == pGameZone->GetZoneType())
    {
        GameDominationField* pDomi = (GameDominationField*)pGameZone;
        pDomi->ReqKickPlayer(pPlayer);

        MSG_CG_GUILD_WITHDRAW_NAK nmsg;
        nmsg.m_byErrorCode = RC::RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD;
        pPlayer->SendPacket(&nmsg,sizeof(nmsg));
        return;
    }
#endif //_NA_008334_20150608_SONNENSCHEIN

    

	MSG_GZ_GUILD_WITHDRAW_SYN msgSYN;
	msgSYN.m_GuildGuid = pGuild->GetGuildGuid();
	msgSYN.m_CharGuid  = pPlayer->GetCharGuid();
	if( FALSE == pPlayer->SendToGuildServer( &msgSYN, sizeof(msgSYN) ) )
	{
		MSG_CG_GUILD_WITHDRAW_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}
}

// 길드장이 길드멤버 강제 탈퇴
Handler_CG_GUILD_IMPL( CG_GUILD_KICKPLAYER_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_KICKPLAYER_SYN* pMsg = (MSG_CG_GUILD_KICKPLAYER_SYN*)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "Guild_KickPlayer Error - Not Connected Master[%u]", pMsg->m_dwKey );
		return;
	}

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_KICKPLAYER_SYN )] Not Exist GameField" );
		return;
	}

	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )
	{
		MSG_CG_GUILD_KICKPLAYER_NAK	nakMsg;
		memcpy( nakMsg.m_ptszKickCharName, pMsg->m_ptszKickCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszKickCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_dwErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		SUNLOG( eCRITICAL_LOG, "Guild_KickPlayer Error - KickMember Failed[%u]!", pMsg->m_dwKey );
		return;
	}

	// 길드 창고 사용중인 유저는 제명 시키지 못함
	Player* pKickPlayer = PlayerManager::Instance()->FindPlayerByName(pMsg->m_ptszKickCharName);
    if (NULL != pKickPlayer)
    {
	    Player* pUsedPlayer = pGuild->GetGuildWareHouse()->GetUsedPlayer();
	    if (NULL != pUsedPlayer &&
		    pUsedPlayer->GetCharGuid() == pKickPlayer->GetCharGuid())
	    {
		    MSG_CG_GUILD_KICKPLAYER_NAK	nakMsg;
		    nakMsg.m_dwErrorCode = RC::RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING;
		    memcpy( nakMsg.m_ptszKickCharName, pMsg->m_ptszKickCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		    nakMsg.m_ptszKickCharName[MAX_CHARNAME_LENGTH] = '\0';
		    pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		    return;
	    }

        // 지역 점령전일 경우, 소넨샤인일 경우
        GameZone* pGameZone = pKickPlayer->GetGameZonePtr();
        if ((pGameZone != NULL))
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            ChaosZoneType chaoszone_type = kChaosZoneTypeNone;
            if (eZONETYPE_INSTANCE == pGameZone->GetZoneType())
            {
                GameInstanceDungeon* indun = static_cast<GameInstanceDungeon*>(pGameZone);
                chaoszone_type = indun->GetChaosZoneType();
            }

            if ((eZONETYPE_DOMINATION_FIELD == pGameZone->GetZoneType()) ||
                (eCHAOSZONE_TYPE_SONNENSCHEIN == chaoszone_type) || eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION == chaoszone_type)
            {
                MSG_CG_GUILD_KICKPLAYER_NAK	nakMsg;
                nakMsg.m_dwErrorCode = RC::RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD;
                memcpy( nakMsg.m_ptszKickCharName, pMsg->m_ptszKickCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
                nakMsg.m_ptszKickCharName[MAX_CHARNAME_LENGTH] = '\0';
                pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
                return;
            }
#else
            if ((eZONETYPE_DOMINATION_FIELD == pGameZone->GetZoneType()))
            {
                GameDominationField* pDomi = (GameDominationField*)pGameZone;
                pDomi->ReqKickPlayer(pKickPlayer);

                MSG_CG_GUILD_KICKPLAYER_NAK	nakMsg;
                nakMsg.m_dwErrorCode = RC::RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD;
                memcpy( nakMsg.m_ptszKickCharName, pMsg->m_ptszKickCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
                nakMsg.m_ptszKickCharName[MAX_CHARNAME_LENGTH] = '\0';
                pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
                return;
            }
#endif //_NA_008334_20150608_SONNENSCHEIN
            
        }
    }

	MSG_GZ_GUILD_KICKPLAYER_SYN msg;
	msg.m_GuildGuid = pGuild->GetGuildGuid();
	msg.m_OperCharGuid = pPlayer->GetCharGuid();
	_tcsncpy( msg.m_ptszCharName, pMsg->m_ptszKickCharName, MAX_CHARNAME_LENGTH );
	msg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( FALSE == pPlayer->SendToGuildServer( &msg, sizeof(msg) ) )
	{
		MSG_CG_GUILD_KICKPLAYER_NAK	nakMsg;
		memcpy( nakMsg.m_ptszKickCharName, pMsg->m_ptszKickCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszKickCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_dwErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
	}
}

// UP 헌납
Handler_CG_GUILD_IMPL( CG_GUILD_DONATE_UP_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_DONATE_UP_SYN* pMsg = (MSG_CG_GUILD_DONATE_UP_SYN*)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "CG_GUILD_DONATE_UP_SYN Error - Not Connected[%u]", pMsg->m_dwKey );
		return;
	}

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_DONATE_UP_SYN )] Not Exist GameField" );
		return;
	}

	// UP 즉 UserPoint는 T_Character 테이블에 있다.
	if( pPlayer->GetCharInfo()->m_UserPoint < pMsg->m_DonateUP )
	{
		MSG_CG_GUILD_DONATE_UP_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_ENOUGH_UP;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	// GuildServer에 UP 헌납 요청
	MSG_GZ_GUILD_DONATE_UP_SYN	sendMsg;
	sendMsg.m_GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	sendMsg.m_CharGuid = pPlayer->GetCharGuid();
	sendMsg.m_DonateUP = pMsg->m_DonateUP;
	if( !pPlayer->SendToGuildServer( &sendMsg, sizeof(sendMsg) ) )
	{
		MSG_CG_GUILD_DONATE_UP_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}

// UP->GP 변환
Handler_CG_GUILD_IMPL( CG_GUILD_CONVERT_UP2GP_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CONVERT_UP2GP_SYN* pMsg = (MSG_CG_GUILD_CONVERT_UP2GP_SYN*)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "CG_GUILD_CONVERT_UP2GP_SYN Error - Not Connected[%u]", pMsg->m_dwKey );
		return;
	}

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CONVERT_UP2GP_SYN )] Not Exist GameField" );
		return;
	}

	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )
	{
		MSG_CG_GUILD_CONVERT_UP2GP_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		SUNLOG( eCRITICAL_LOG, "CG_GUILD_CONVERT_UP2GP_SYN Error[Key:%u][Error:%u]", pMsg->m_dwKey, RC::RC_GUILD_NOT_EXIST_GUILD );
		return;
	}

	// GuildServer에 변환 요청
	MSG_GZ_GUILD_CONVERT_UP2GP_SYN	sendMsg;
	sendMsg.m_GuildGuid = GuildGuid;
	sendMsg.m_OperCharGuid = pPlayer->GetCharGuid();
	sendMsg.m_ConvertGP = pMsg->m_ConvertGP;
	pPlayer->SendToGuildServer( &sendMsg, sizeof(sendMsg) );	
}

// 길드 직책 변경
Handler_CG_GUILD_IMPL(CG_GUILD_CHANGE_DUTY_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CHANGE_DUTY_SYN* receive_message = static_cast<MSG_CG_GUILD_CHANGE_DUTY_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);

	Player* player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	if (player == NULL)
    {
        return;
    }
    
	if (!player->GetField())
	{
		SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_DUTY_SYN )] Not Exist GameField"));
		return;
	}

	CHARGUID target_player_guid = 0;
	Player* target_player = PlayerManager::Instance()->FindPlayerByName(receive_message->m_szTargetCharName);
	if (target_player != NULL)
    {
        target_player_guid = target_player->GetCharGuid();
    }
        
	RC::eGUILD_RESULT change_duty_result = g_GameGuildManager.CanChangeDuty( 
        player->GetCharInfo()->m_GuildGuid, player->GetCharGuid(), target_player_guid, 
        static_cast<eGUILD_DUTY>(receive_message->m_byDuty));
	if (change_duty_result != RC::RC_GUILD_SUCCESS)
	{
		MSG_CG_GUILD_CHANGE_DUTY_NAK send_message;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, change_duty_result, BYTE));
		send_message.m_byErrorCode = static_cast<BYTE>(change_duty_result);
		player->SendPacket(&send_message, sizeof(send_message));
		return;
	}

	MSG_GZ_GUILD_CHANGE_DUTY_SYN send_message;
	send_message.m_GuildGuid = player->GetCharInfo()->m_GuildGuid;
	send_message.m_OperCharGuid	= player->GetCharGuid();
	strncpy(send_message.m_szTargetCharName, receive_message->m_szTargetCharName, 
        sizeof(TCHAR) * MAX_CHARNAME_LENGTH);
	send_message.m_szTargetCharName[MAX_CHARNAME_LENGTH] = _T('\0');
	send_message.m_byDuty = receive_message->m_byDuty;

	if (!player->SendToGuildServer(&send_message, sizeof(MSG_GZ_GUILD_CHANGE_DUTY_SYN)))
	{
		MSG_CG_GUILD_CHANGE_DUTY_NAK send_message;
		send_message.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		player->SendPacket(&send_message, sizeof(send_message));
		return;
	}
} 

Handler_CG_GUILD_IMPL( CG_GUILD_RIGHT_DELEGATION_LIST_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_RIGHT_DELEGATION_LIST_SYN* pMsg = (MSG_CG_GUILD_RIGHT_DELEGATION_LIST_SYN*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RIGHT_DELEGATION_LIST_SYN )] Not Exist GameField" );
		return;
	}

	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN sendMsg;
	sendMsg.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	sendMsg.m_OperCharGuid		= pPlayer->GetCharGuid();
	strncpy( sendMsg.m_szTargetCharName, pMsg->m_szTargetCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	sendMsg.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( !pPlayer->SendToGuildServer(&sendMsg, sizeof(MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN) ) )
	{
		MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMsg, sizeof(nakMsg));
		return;
	}
}

Handler_CG_GUILD_IMPL( CG_GUILD_RIGHT_DELEGATION_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_RIGHT_DELEGATION_SYN* pMsg = (MSG_CG_GUILD_RIGHT_DELEGATION_SYN*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RIGHT_DELEGATION_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_GZ_GUILD_RIGHT_DELEGATION_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_GZ_GUILD_RIGHT_DELEGATION_SYN sendMsg;
	sendMsg.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	sendMsg.m_OperCharGuid		= pPlayer->GetCharGuid();
	strncpy( sendMsg.m_szTargetCharName, pMsg->m_szTargetCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	sendMsg.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	CopyMemory( sendMsg.m_DelegateRight, pMsg->m_DelegateRight, sizeof(sendMsg.m_DelegateRight) );
	pPlayer->SendToGuildServer( &sendMsg, sizeof( sendMsg ) );
}

//길드마스터 변경
Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_MASTER_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CHANGE_MASTER_SYN* pMsg = (MSG_CG_GUILD_CHANGE_MASTER_SYN*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_MASTER_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_GZ_GUILD_CHANGE_MASTER_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_GZ_GUILD_CHANGE_MASTER_SYN sendMsg;
	sendMsg.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	sendMsg.m_OperCharGuid		= pPlayer->GetCharGuid();
	strncpy( sendMsg.m_szTargetCharName, pMsg->m_szTargetCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	sendMsg.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( !pPlayer->SendToGuildServer( &sendMsg, sizeof( sendMsg ) ) )
	{
		MSG_GZ_GUILD_CHANGE_MASTER_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}

// 길드 군단 생성
Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_CORPS_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	//군단 생성에 군단 번호가 없는 것은 새로 생성되는 군단은 현재 남아 있는 군단중 제일 앞 군단이기 때문이다.(순차적 생성)
	MSG_CG_GUILD_CREATE_CORPS_SYN *pMsg = (MSG_CG_GUILD_CREATE_CORPS_SYN *)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_CORPS_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_CREATE_CORPS_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	strncpy( synMSG.m_szTargetCharName, pMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	synMSG.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( !pPlayer->SendToGuildServer(&synMSG, sizeof(synMSG)) )
	{
		MSG_CG_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}
}

//군단해산.
Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_CORPS_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	//군단 생성에 군단 번호가 없는 것은 새로 생성되는 군단은 현재 남아 있는 군단중 제일 앞 군단이기 때문이다.(순차적 생성)
	MSG_CG_GUILD_DESTROY_CORPS_SYN* pMsg = (MSG_CG_GUILD_DESTROY_CORPS_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_CORPS_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_DESTROY_CORPS_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	synMSG.m_byCorpsNum			= pMsg->m_byCorpsNum;
	if( !pPlayer->SendToGuildServer(&synMSG, sizeof(synMSG)) )
	{
		MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}
}

Handler_CG_GUILD_IMPL( CG_GUILD_EMPTY_COMMANDER_INFO_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_EMPTY_COMMANDER_INFO_SYN* pMsg = (MSG_CG_GUILD_EMPTY_COMMANDER_INFO_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_EMPTY_COMMANDER_INFO_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_EMPTY_COMMANDER_INFO_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	pPlayer->SendToGuildServer(&synMSG, sizeof(synMSG));
}

Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_COMMANDER_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CHANGE_COMMANDER_SYN* pMsg = (MSG_CG_GUILD_CHANGE_COMMANDER_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_COMMANDER_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_CHANGE_COMMANDER_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	synMSG.m_byCorpsNum			= pMsg->m_byCorpsNum;
	strncpy( synMSG.m_szTargetCharName, pMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	synMSG.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	pPlayer->SendToGuildServer( &synMSG, sizeof(synMSG) );
}

Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_CORPS_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CHANGE_CORPS_SYN* pMsg = (MSG_CG_GUILD_CHANGE_CORPS_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CHANGE_CORPS_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_CHANGE_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_CHANGE_CORPS_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	synMSG.m_byCorpsNum			= pMsg->m_byCorpsNum;
	strncpy( synMSG.m_szTargetCharName, pMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	synMSG.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( !pPlayer->SendToGuildServer( &synMSG, sizeof(synMSG) ) )
	{
		MSG_CG_GUILD_CHANGE_CORPS_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}
}

Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_REAR_CAMP_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_CREATE_REAR_CAMP_SYN* pMsg = (MSG_CG_GUILD_CREATE_REAR_CAMP_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_CREATE_REAR_CAMP_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_CREATE_REAR_CAMP_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	strncpy( synMSG.m_szTargetCharName, pMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	synMSG.m_szTargetCharName[MAX_CHARNAME_LENGTH] = '\0';
	if( !pPlayer->SendToGuildServer(&synMSG, sizeof(synMSG)) )
	{
		MSG_CG_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;		
	}
}

Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_REAR_CAMP_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_DESTROY_REAR_CAMP_SYN* pMsg = (MSG_CG_GUILD_DESTROY_REAR_CAMP_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_DESTROY_REAR_CAMP_SYN )] Not Exist GameField" );
		return;
	}

	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	MSG_GZ_GUILD_DESTROY_REAR_CAMP_SYN synMSG;
	synMSG.m_dwKey				= pMsg->m_dwKey;
	synMSG.m_GuildGuid			= pPlayer->GetCharInfo()->m_GuildGuid;
	synMSG.m_OperCharGuid		= pPlayer->GetCharInfo()->m_CharGuid;
	synMSG.m_byCorpsNum			= pMsg->m_byCorpsNum;
	if( !pPlayer->SendToGuildServer(&synMSG, sizeof(synMSG)) )
	{
		MSG_CG_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;	
	}
}

//길드 초대
Handler_CG_GUILD_IMPL( CG_GUILD_INVITE_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_INVITE_SYN* recv_msg = static_cast<MSG_CG_GUILD_INVITE_SYN*>(pRecvMsg);
    ASSERT(recv_msg != NULL);

	Player* request_player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
	if (request_player == NULL || request_player->GetField() == NULL)
    {
        return;
    }

	TCHAR target_player_name[MAX_CLIENT_CHARNAME_LENGTH]={0, };
	_tcsncpy(target_player_name, recv_msg->m_TargetCharName, MAX_CHARNAME_LENGTH);

	Player* target_player = PlayerManager::Instance()->FindPlayerByName(target_player_name);
	if (target_player == NULL)
	{
		MSG_CG_GUILD_INVITE_NAK nak_msg;
		nak_msg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_TARGET_STATE;
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	RC::eGUILD_RESULT result_code = g_GameGuildManager.CanJoinGuild(request_player, target_player);
	if (result_code != RC::RC_GUILD_SUCCESS)
	{
		MSG_CG_GUILD_INVITE_NAK nak_msg;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, result_code, BYTE));
		nak_msg.m_byErrorCode = static_cast<BYTE>(result_code);
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	MSG_GZ_GUILD_INVITE_SYN	send_msg;
	send_msg.m_GuildGuid = request_player->GetCharInfo()->m_GuildGuid;
	send_msg.m_OperCharGuid = request_player->GetCharGuid();
	send_msg.m_TargetCharGuid = target_player->GetCharGuid();
	send_msg.m_TargetUserKey = target_player->GetUserKey();
	send_msg.m_Level = target_player->GetLevel();
	if (request_player->SendToGuildServer(&send_msg, sizeof(send_msg)) == FALSE)
	{
		MSG_CG_GUILD_INVITE_NAK nak_msg;
		nak_msg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}
}

//길드 초대 응답.
Handler_CG_GUILD_IMPL( CG_GUILD_INVITE_ANSWER_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_INVITE_ANSWER_SYN* pMsg = (MSG_CG_GUILD_INVITE_ANSWER_SYN*)pRecvMsg;

	Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pTargetPlayer || !pTargetPlayer->GetField() ) return ;

	if( !pTargetPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_INVITE_ANSWER_SYN )] Not Exist GameField" );
		return;
	}

	Player* pOperPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_OperUserKey );
	if( !pOperPlayer || !pOperPlayer->GetField() ) 
	{
		MSG_CG_GUILD_INVITE_ANSWER_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_TARGET_STATE;
		pTargetPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	//*** 기본 체크만 하자. ***

	//길드가 존재해야 한다.
	if( pOperPlayer->GetCharInfo()->m_GuildGuid == 0 )
	{
		MSG_CG_GUILD_INVITE_ANSWER_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_FAILED;
		pTargetPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	//길드가 없어여 한다.
	if( pTargetPlayer->GetCharInfo()->m_GuildGuid != 0 )
	{
		MSG_CG_GUILD_INVITE_ANSWER_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_GUILD_FAILED;
		pTargetPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	MSG_CG_GUILD_INVITE_ANSWER_CMD	cmdMsg;
	cmdMsg.m_bInveite = pMsg->m_bInveite;
	cmdMsg.m_TargetObjKey	= pTargetPlayer->GetObjectKey();
	_tcsncpy(cmdMsg.m_TargetCharName, pTargetPlayer->GetCharName(), MAX_CHARNAME_LENGTH);
	pOperPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );
}

Handler_CG_GUILD_IMPL( CG_GUILD_JOIN_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_JOIN_SYN* recv_msg = static_cast<MSG_CG_GUILD_JOIN_SYN*>(pRecvMsg);
    ASSERT(recv_msg != NULL);

	Player* request_player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
	if (request_player == NULL || request_player->GetField() == NULL)
    {
        return;
    }
        
	Player* targe_player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->m_TargetObjKey);
	if (targe_player == NULL) 
	{
		MSG_CG_GUILD_JOIN_NAK nak_msg;
		nak_msg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_TARGET_STATE;
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	RC::eGUILD_RESULT result_code = g_GameGuildManager.CanJoinGuild(request_player, targe_player);
	if (result_code != RC::RC_GUILD_SUCCESS)
	{
		MSG_CG_GUILD_JOIN_NAK nak_msg;
        ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, result_code, BYTE));
		nak_msg.m_byErrorCode = static_cast<BYTE>(result_code);
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	MSG_GZ_GUILD_JOIN_SYN send_msg;
	send_msg.m_GuildGuid = request_player->GetCharInfo()->m_GuildGuid;
	send_msg.m_OperCharGuid	= request_player->GetCharGuid();
	send_msg.m_TargetCharGuid = targe_player->GetCharGuid();
    _tcsncpy(send_msg.m_tszCharName, targe_player->GetCharName(),
             _countof(send_msg.m_tszCharName));
    send_msg.m_tszCharName[_countof(send_msg.m_tszCharName) - 1] = _T('\0');
	send_msg.m_byClass = targe_player->GetCharInfo()->m_byClassCode;
	send_msg.m_Level = targe_player->GetLevel();
	if (!request_player->SendToGuildServer(&send_msg, sizeof(send_msg)))
	{
		MSG_CG_GUILD_JOIN_NAK nak_msg;
		nak_msg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}
}

// 이 패킷은 처음 클라이언트에서 딱 1번만 보낸다.
Handler_CG_GUILD_IMPL( CG_GUILD_TOTALINFO_REQ_SYN )
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_TOTALINFO_REQ_SYN* pMsg = (MSG_CG_GUILD_TOTALINFO_REQ_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer || !pPlayer->GetField() ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_TOTALINFO_REQ_SYN )] Not Exist GameField" );
		return;
	}

	// 사용자 정보에 가입된 길드가 없으면
	if( pPlayer->GetCharInfo()->m_GuildGuid == 0 )	
	{
		MSG_CG_GUILD_TOTALINFO_REQ_NAK	nak;
		nak.m_byErrorCode = RC::RC_GUILD_NOT_JOIN_GUILD;
		pPlayer->SendPacket( &nak, sizeof(nak) );
		return;
	}

	MSG_GZ_GUILD_TOTALINFO_REQ_SYN sycMsg;
	sycMsg.m_GuildGuid	= pPlayer->GetCharInfo()->m_GuildGuid;
	sycMsg.m_CharGuid	= pPlayer->GetCharGuid();
	if( !pPlayer->SendToGuildServer(&sycMsg, sizeof(sycMsg) ) )
	{
		MSG_CG_GUILD_TOTALINFO_REQ_NAK	nak;
		nak.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pPlayer->SendPacket( &nak, sizeof(nak) );
		return;
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_RELATION_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

    PACKET_PTR_CONVERT(MSG_CG_GUILD_RELATION_SYN, recv_msg, pRecvMsg);
    PLAYER_PTR_USERKEY(Player, request_player, pRecvMsg->m_dwKey);

    if (request_player->GetField() == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not exist game field : name : %s",
            request_player->GetCharName()
            );
        return;
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (GameDominationManager::Instance()->CheckDominationIsProgressing()) {
        MSG_CG_GUILD_RELATION_NAK msg;
        msg.m_byErrorCode = RC::RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION;
        request_player->SendPacket(&msg, sizeof(msg));
        return;
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    if (eGUILD_RELATION_ALLIENCE == recv_msg->m_eType ||//동맹 신청
        eGUILD_RELATION_CANCEL_BOTH_HOSTILITY == recv_msg->m_eType)//쌍방적대 취소
    {
        MSG_GZ_GUILD_BOTH_RELATION_SYN msg;
        msg.m_dwKey = recv_msg->m_dwKey;
        msg.m_eRelationType = recv_msg->m_eType;
        msg.m_GuildGuid = request_player->GetCharInfo()->m_GuildGuid;
        msg.m_RequestPlayerIndex = request_player->GetCharInfo()->m_CharGuid;
        util::GUILDNAMECOPY(msg.m_tszRelationGuildName, recv_msg->m_szRelationGuildName);
        if (request_player->SendToGuildServer(&msg, sizeof(msg)) == FALSE)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Fail send to guild server : name : %s",
                request_player->GetCharName()
                );
            return;
        }
    }
    else
    {
        MSG_GZ_GUILD_RELATION_SYN msg;
        msg.m_dwKey = recv_msg->m_dwKey;
        msg.m_eRelationType = eGUILD_RELATION_PROCLAIM_HOSTILITY;
        msg.m_RequestUserKey = request_player->GetUserKey();
        msg.m_RequestCharGuid = request_player->GetCharGuid();
        msg.m_ResponseGuildGuid = 0;
        msg.m_RequestGuildGuid = request_player->GetCharInfo()->m_GuildGuid;
        util::GUILDNAMECOPY(msg.m_tszRelationGuildName, recv_msg->m_szRelationGuildName);
        if (request_player->SendToGuildServer(&msg, sizeof(msg)) == FALSE)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Fail send to guild server : name : %s",
                request_player->GetCharName()
                );
            return;
        }
    }
}

Handler_CG_GUILD_IMPL(CG_GUILD_RELATION_ANSWER_SYN)	
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	PACKET_PTR_CONVERT(MSG_CG_GUILD_RELATION_ANSWER_SYN, pRecvSynMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pAnswerPlayer, pRecvMsg->m_dwKey);
	PLAYER_PTR_USERKEY(Player, pRequestPlayer, pRecvSynMsg->m_RequestUserKey);

	if( !pAnswerPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RELATION_ANSWER_SYN )] Not Exist GameField" );
		return;
	}

	if(eGUILD_ANSWER_YES == pRecvSynMsg->m_eAnswerType)
	{	
		MSG_GZ_GUILD_RELATION_SYN msg;
		msg.m_dwKey = pRecvSynMsg->m_dwKey;
		msg.m_eRelationType = pRecvSynMsg->m_eRelationType;
		msg.m_RequestUserKey = pRecvSynMsg->m_RequestUserKey;
		msg.m_RequestCharGuid = pRequestPlayer->GetCharGuid();
		msg.m_RequestGuildGuid = pRequestPlayer->GetCharInfo()->m_GuildGuid;
		msg.m_ResponseGuildGuid = pAnswerPlayer->GetCharInfo()->m_GuildGuid;
		if(FALSE == pAnswerPlayer->SendToGuildServer(&msg, sizeof(msg)))
			return;
	}
	else
	{
		GUILDGUID GuildGuid = pAnswerPlayer->GetGuildGuid();
		GameGuild* pAckGuild = g_GameGuildManager.FindGuild(GuildGuid);

		MSG_CG_GUILD_RELATION_REJECT_ACK msg;
        msg.m_eRelationType = pRecvSynMsg->m_eRelationType;
		ZeroMemory(msg.m_szRelGuildName, MAX_GUILDNAME_LENGTH);
        if (pAckGuild) {
            util::GUILDNAMECOPY(msg.m_szRelGuildName, pAckGuild->GetGuildName());
        };
		pRequestPlayer->SendPacket(&msg, sizeof(msg));
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_RELATION_BOTH_AGAINST_SYN)	
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	PACKET_PTR_CONVERT(MSG_CG_GUILD_RELATION_BOTH_AGAINST_SYN, pRecvSynMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pRequestPlayer, pRecvMsg->m_dwKey);

	if( !pRequestPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RELATION_BOTH_AGAINST_SYN )] Not Exist GameField" );
		return;
	}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (GameDominationManager::Instance()->CheckDominationIsProgressing()) {
        MSG_CG_GUILD_RELATION_NAK msg;
        msg.m_byErrorCode = RC::RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION;
        pRequestPlayer->SendPacket(&msg, sizeof(msg));
        return;
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

	MSG_GZ_GUILD_RELATION_BOTH_AGAINST_SYN msg;
	msg.m_dwKey = pRecvSynMsg->m_dwKey;
	msg.m_MasterCharGuid =pRequestPlayer->GetCharGuid();
	msg.m_GuildGuid = pRequestPlayer->GetCharInfo()->m_GuildGuid;
	msg.m_RelGuildGuid =pRecvSynMsg->m_BothAgainstGuildGuid;
	
	if(!pRequestPlayer->SendToGuildServer(&msg, sizeof(msg)))
	{
		MSG_CG_GUILD_RELATION_NAK msg;
		msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pRequestPlayer->SendPacket(&msg, sizeof(msg));
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_RELATION_DELETE_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	PACKET_PTR_CONVERT(MSG_CG_GUILD_RELATION_DELETE_SYN, pRecvSynMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pRequestPlayer, pRecvMsg->m_dwKey);

	if( !pRequestPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RELATION_DELETE_SYN )] Not Exist GameField" );
		return;
	}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (GameDominationManager::Instance()->CheckDominationIsProgressing()) {
        MSG_CG_GUILD_RELATION_NAK msg;
        msg.m_byErrorCode = RC::RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION;
        pRequestPlayer->SendPacket(&msg, sizeof(msg));
        return;
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

	MSG_GZ_GUILD_RELATION_DELETE_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_eType = pRecvSynMsg->m_eType;
	msg.m_GuildGuid	= pRequestPlayer->GetCharInfo()->m_GuildGuid;
	msg.m_RelGuildGuid	= pRecvSynMsg->m_eDeleteGuildGuid;
	msg.m_RequestPlayerIndex = pRequestPlayer->GetCharGuid();
	
	if(!pRequestPlayer->SendToGuildServer(&msg, sizeof(msg)))
	{
		MSG_CG_GUILD_RELATION_NAK msg;
		msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pRequestPlayer->SendPacket(&msg, sizeof(msg));
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_ADD_INFO_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_ADD_INFO_SYN* receive_message = static_cast<MSG_CG_GUILD_ADD_INFO_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);

	Player* player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	if (player == NULL)
    {
        return;
    }
    
	if (player->GetField() == NULL)
	{
		SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_GUILD_IMPL( CG_GUILD_ADD_INFO_SYN )] Not Exist GameField"));
		return;
	}
	
	GUILDGUID guild_guid = player->GetCharInfo()->m_GuildGuid;

	MSG_CG_GUILD_ADD_INFO_ACK send_message;
	send_message.m_AddInfo.m_GuildGuid = guild_guid;
	GuildSmallContentEntry::RelationSystem::LoadRelationInfo(&send_message.m_AddInfo);
	send_message.m_byRelCnt = send_message.m_AddInfo.m_byRelationGuildCnt;
    int send_message_size = send_message.GetSize();
    ASSERT(SAFE_NUMERIC_TYPECAST(int, send_message_size, WORD));
    player->SendPacket(&send_message, static_cast<WORD>(send_message_size));
}

Handler_CG_GUILD_IMPL(CG_GUILD_RECOVERY_GUILD_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_RECOVERY_GUILD_SYN *pMSG = 
		static_cast<MSG_CG_GUILD_RECOVERY_GUILD_SYN*>(pRecvMsg);

	Player *pMaster = PlayerManager::Instance()->FindPlayerByUserKey(pMSG->m_dwKey);

	if(pMaster)
	{
		if( !pMaster->GetField() )
		{
			SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_RECOVERY_GUILD_SYN )] Not Exist GameField" );
			return;
		}

		GUILDGUID guild_index = pMaster->GetCharInfo()->m_GuildGuid;
		//-Cast GuildServer
		MSG_GZ_GUILD_RECOVERY_GUILD_SYN msg;
		msg.m_dwKey = pMSG->m_dwKey;
		msg.m_CharGuid = pMaster->GetCharInfo()->m_CharGuid;
		msg.m_GuildGuid = guild_index;
		pMaster->SendToGuildServer(&msg, sizeof(msg));
	}

	//TODO : 로그 남기자..
}

Handler_CG_GUILD_IMPL(CG_GUILD_GUILDMARK_REGISTER_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_GUILDMARK_REGISTER_SYN* receive_message = 
        static_cast<MSG_CG_GUILD_GUILDMARK_REGISTER_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);
	
	Player* request_player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	if (request_player != NULL)
	{
		if (request_player->GetField() == NULL)
		{
			SUNLOG(eCRITICAL_LOG, 
                _T("[Handler_CG_GUILD_IMPL( CG_GUILD_RECOVERY_GUILD_SYN )] Not Exist GameField"));
			return;
		}

		GuildSmallContentEntry::GuildMark::GuildMarkInfo guild_mark;
		guild_mark.posType = receive_message->m_SlotPos;
		guild_mark.slot_Index = receive_message->m_SlotIndex;
		guild_mark.guild_Index = receive_message->m_GuildIndex;
		guild_mark.pattern_index = receive_message->m_PatternIndex;
		guild_mark.background_index = receive_message->m_BackGroundIndex;

		RC::eGUILD_RESULT register_result = GuildSmallContentEntry::GuildMark::CanRegister(
            request_player, guild_mark);
		if (RC::RC_GUILD_SUCCESS != register_result)
		{
			MSG_CG_GUILD_GUILDMARK_REGISTER_NAK nak_send_message;
            ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, register_result, BYTE));
			nak_send_message.m_bErrorCode = static_cast<BYTE>(register_result);
			request_player->SendPacket(&nak_send_message, sizeof(nak_send_message));
			SUNLOG(eCRITICAL_LOG, _T("길드마크 삭제 오류[%u][%u]"), request_player->GetCharGuid(), 
                request_player->GetGuildGuid());
			return;
		}

		MSG_GZ_GUILD_GUILDMARK_REGISTER_SYN send_message;
		send_message.m_dwKey = receive_message->m_dwKey;
		send_message.m_SlotIdx= receive_message->m_SlotIndex;
		send_message.m_SlotPos = receive_message->m_SlotPos;
		send_message.m_GuildIndex = guild_mark.guild_Index;
		send_message.m_PatternIndex = guild_mark.pattern_index;
		send_message.m_BackGroundIndex = guild_mark.background_index;
		send_message.m_RequestPlayerIndex = request_player->GetCharInfo()->m_CharGuid;

		if (!request_player->SendToGuildServer(&send_message, sizeof(send_message)))
        {
            MessageOut(eDEV_LOG, _T("RegisterCastToGuildServer Error"));
        }
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_GUILDMARK_DELETE_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_GUILDMARK_DELETE_SYN* receive_message = 
		static_cast<MSG_CG_GUILD_GUILDMARK_DELETE_SYN*>(pRecvMsg);
    ASSERT(receive_message != NULL);

	Player* request_player = PlayerManager::Instance()->FindPlayerByUserKey(receive_message->m_dwKey);
	if (request_player != NULL)
	{  
		if (request_player->GetField() == NULL)
		{
			SUNLOG(eCRITICAL_LOG, 
                _T("[Handler_CG_GUILD_IMPL( CG_GUILD_GUILDMARK_DELETE_SYN )] Not Exist GameField"));
			return;
		}

		GuildSmallContentEntry::GuildMark::GuildMarkInfo guild_mark;
		guild_mark.guild_Index = receive_message->m_GuildIndex;
		guild_mark.pattern_index = receive_message->m_PatternIndex;
		guild_mark.background_index = receive_message->m_BackGroundIndex;
		
        RC::eGUILD_RESULT guild_mark_delete_result =
            GuildSmallContentEntry::GuildMark::CanDelete(request_player, guild_mark);
		if (guild_mark_delete_result != RC::RC_GUILD_SUCCESS)
		{
			MSG_CG_GUILD_GUILDMARK_DELETE_NAK nak_send_message;
            ASSERT(SAFE_NUMERIC_TYPECAST(RC::eGUILD_RESULT, guild_mark_delete_result, BYTE));
			nak_send_message.m_bErrorCode = static_cast<BYTE>(guild_mark_delete_result);
			request_player->SendPacket(&nak_send_message, sizeof(nak_send_message));
			return;
		}
		
		MSG_GZ_GUILD_GUILDMARK_DELETE_SYN send_message;
		send_message.m_GuildIndex = guild_mark.guild_Index;
		send_message.m_PatternIndex = guild_mark.pattern_index;
		send_message.m_BackGroundIndex = guild_mark.background_index;
		send_message.m_RequestPlayerIndex =request_player->GetCharInfo()->m_CharGuid;

		if (!request_player->SendToGuildServer(&send_message, sizeof(send_message)))
        {
            MessageOut(eDEV_LOG, _T("DeleteCastToGuildServer Error"));
        }
	}

	MessageOut(eDEV_LOG, _T("CG_GUILD_GUILDMARK_REGISTER_SYN Error - None Player"));
}

#include "ItemManager.h"
#include "FunctionalRestrictManager.h"
#include "GuildWareHouseSlotContainer.h"

////////////////////////////////////////////////////////////////////////////////////
// CG_GUILD_WAREHOUSE_OPEN_SYN (유저에 길드창고 ADD) ->
// GZ_GUILD_WAREHOUSE_OPEN_BRD (창고에 유저 연결 (DBP정보로 셋팅) )

Handler_CG_GUILD_IMPL(CG_GUILD_WAREHOUSE_OPEN_SYN)
{
    // CHANGES: f110627.2L, code rearrangement
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

    PACKET_PTR_CONVERT(MSG_CG_GUILD_WAREHOUSE_OPEN_SYN, pMsg, pRecvMsg);
    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player->GetField() == NULL) {
        return;
    };
    //----------------------------------------------------------------------------------------------
    struct ErrorHandling {
        ErrorHandling(Player* player, const char* function_name)
            : player_(player)
            , result_(RC::RC_GUILD_SUCCESS)
            , function_name_(function_name)
            , extra_information_(0)
        {}
        ~ErrorHandling()
        {
            if (result_ == RC::RC_GUILD_SUCCESS) {
                return;
            };
            //
            MSG_CG_GUILD_WAREHOUSE_OPEN_NAK msg_nak;
            msg_nak.m_bErrorCode = result_;
            player_->SendPacket(&msg_nak, sizeof(msg_nak));
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|error occurred, guild=%d,char_guid=%d, position=%d"),
                   function_name_, player_->GetCharInfo()->m_GuildGuid,
                   player_->GetCharGuid(), extra_information_);
        };

        Player* const player_;
        RC::eGUILD_RESULT result_;
        const char* function_name_;
        int extra_information_;
        //
    private:
        ErrorHandling(const ErrorHandling& rhs) : player_(rhs.player_) {}
        void operator=(const ErrorHandling&){}
    } error_handling(player, __FUNCTION__);
    //----------------------------------------------------------------------------------------------
    GUILDGUID guild_index = player->GetCharInfo()->m_GuildGuid;
    GameGuild* const guild = (guild_index != 0) ?
        g_GameGuildManager.FindGuild(guild_index) : NULL;
    if (guild == NULL) {
        error_handling.result_ = RC::RC_GUILD_INVALID_STATE;
        error_handling.extra_information_ = 10;
        return;
    };

    // CHANGES: f110627.2L, prevent unauthorized invader's request.
    // NOTE: the binding of the guild warehouse has will always bind from them of the guild.
    ItemManager* const item_manager = player->GetItemManager();
    CGuildWarehouseSlotContainer* const guild_warehouse = guild->GetGuildWareHouse();
    item_manager->BindGuildWarehouseContainer(guild_warehouse);
    //
    error_handling.result_ = guild_warehouse->IsEnableService(guild);
    if (error_handling.result_ != RC::RC_GUILD_SUCCESS) {
        error_handling.extra_information_ = 20;
        return;
    }

    MSG_GZ_GUILD_WAREHOUSE_OPEN_SYN msg_link_event;
    msg_link_event.m_dwKey = pMsg->m_dwKey;
    msg_link_event.m_GuildGuid = guild_index;
    msg_link_event.m_OperCharGuid = player->GetCharGuid();
    //msg_link_event.m_ServerSessionIdx = player->GetServerSession()->GetSessionIndex();
    // NOTE: if the gameserver disconnected with the database proxy server,
    // it enable to prevent on the agent server.
    if (player->SendToGuildServer(&msg_link_event, sizeof(msg_link_event)) == false)
    {
        error_handling.result_ = RC::RC_GUILD_FAILED;
        error_handling.extra_information_ = 30;
        return;
    };
}


Handler_CG_GUILD_IMPL(CG_GUILD_WAREHOUSE_CLOSE_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	PACKET_PTR_CONVERT(MSG_CG_GUILD_WAREHOUSE_CLOSE_SYN, pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_WAREHOUSE_CLOSE_SYN )] Not Exist GameField" );
		return;
	}

	GUILDGUID guild_index = pPlayer->GetCharInfo()->m_GuildGuid;
	GameGuild* pGuild = g_GameGuildManager.FindGuild(guild_index);
	if(!pGuild)
	{
		MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK msg;
		msg.m_bErrorCode = RC::RC_GUILD_INVALID_STATE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		SUNLOG(eCRITICAL_LOG,  "[CG_GUILD_WAREHOUSE_CLOSE_SYN Guild State Err[%d]", pPlayer->GetCharGuid());
		return;
	}

	CGuildWarehouseSlotContainer* pContainer = pGuild->GetGuildWareHouse();

	if(!pContainer || !pContainer->IsOpened())
	{
		MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK msg;
		msg.m_bErrorCode = RC::RC_GUILD_INVALID_STATE;
		pPlayer->SendPacket(&msg, sizeof(msg));
		SUNLOG(eCRITICAL_LOG,  "[CG_GUILD_WAREHOUSE_CLOSE_SYN Open State Err[%d]", pPlayer->GetCharGuid());
		return;
	}

	if(pContainer->IsChanged())
	{
		if(!pPlayer->SerializeInfoToDBProxy(UPDATE_FACTOR_NORMAL_CLOSE))
		{
			MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK msg;
			msg.m_bErrorCode = RC::RC_GUILD_INVALID_STATE;
			pPlayer->SendPacket(&msg, sizeof(msg));
			SUNLOG(eCRITICAL_LOG,  "[CG_GUILD_WAREHOUSE_CLOSE_SYN SaveDBP Err[%d]", pPlayer->GetCharGuid());
			return;
		}
	}
	else //길드 창고에 변경사항이 없으면 
	{
		{
			MSG_CG_GUILD_WAREHOUSE_CLOSE_ACK msg;
			pPlayer->SendPacket(&msg, sizeof(msg));
		}

		MSG_DG_GUILD_WAREHOUSE_CLOSE_SYN msg;
		msg.m_dwKey = pPlayer->GetUserKey();
		msg.m_GuildGuid = pGuild->GetGuildGuid();
		pPlayer->SendToGameDBPServer(&msg, sizeof(msg));

        pContainer->CloseWarehouse();

        // 중국 복사 버그 수정 (유저 창고의 방식 사용) [10/12/2009 lst1024]

        // CLOSE_SYN 에서 현재 정보 저장
        // CLOSE_ACK 에서 유저와 창고 사이의 링크해제(유저포인트,슬롯정보).

        // 문제...
        // CLOSE_SYN 과 CLOSE_ACK 사이에 MONEY_SYN 발생.

        // 발생한 MONEY_SYN 정보갱신은 다음 CLOSE_SYN이나 케릭터 정보 갱신에서 수행되나,
        // CLOSE_SYN 은 이미 수행 중에 있음.
        // 다음 OPEN_SYN에 의해 DBP가 가진 MONEY_SYN 정보가 없는 스트림으로 갱신.
        // MONEY_SYN 정보 날라감.
        
        //
        // 트랜잭션 처리가 필요하나 유저 창고 방식을 따름.

        // 유저 창고의 경우 게임서버의 창고 부터 Close.
        // 길드 창고의 경우도 길드서버와 DBP에서 NAK가 없으므로,
        // 게임서버의 창고 부터 Close.
	}
}

Handler_CG_GUILD_IMPL(CG_GUILD_WAREHOUSE_MONEY_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

    PACKET_PTR_CONVERT(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN, recv_msg, pRecvMsg);
    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);
    if (player->GetField() == NULL) {
        return;
    };

    CGuildWarehouseSlotContainer* const guild_warehouse = player->GetGuildWarehouseContainer();
    if (guild_warehouse == NULL) {
        return;
    };

    RC::eGUILD_RESULT result = RC::RC_GUILD_SUCCESS;

    const MONEY money = recv_msg->m_Money;
    const BYTE trade_type = recv_msg->m_byType;
    // CHANGES: f110627.2L, prevent unauthorized invader's request.
    const bool valid_request = (trade_type == recv_msg->PUTMONEY ||
                                trade_type == recv_msg->GETMONEY);
    //----------------------------------------------------------------------------------------------
    if (valid_request == false) {
        result = RC::RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT;
    }
    else if ((trade_type == recv_msg->PUTMONEY) &&
             (guild_warehouse->IsUseRight(player, false) != RC::RC_ITEM_SUCCESS))
    {
        result = RC::RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT;
    }
    else if ((trade_type == recv_msg->GETMONEY) &&
             (guild_warehouse->IsUseRight(player, true) != RC::RC_ITEM_SUCCESS))
    {
        result = RC::RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT;
    }
    else if (trade_type == recv_msg->PUTMONEY)
    {
        //길드 창고에 돈을 넣음
        if (money > player->GetMoney()) {
            result = RC::RC_GUILD_WAREHOUSE_INVENMONEY_LOW;
        }
        else if (guild_warehouse->DepositMoney(money))
        {
            player->MinusMoney(money, Player::eMONEY_CIRCULATING);
            guild_warehouse->MoneyLogDBSave(money, MONEY_IN);
            GAMELOG->LogMoney(MONEY_GUILD_WAREHOUSE_PUT, player, money);
        }
        else {
            result = RC::RC_GUILD_WAREHOUSE_INVENMONEY_LOW;
        }
    }
    else if (trade_type == recv_msg->GETMONEY)
    {
        if (player->CanPlusMoney(money) == false) {
            result = RC::RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT;
        }
        else if (guild_warehouse->WithDrawMoney(money)) // 창고에서 돈을 찾음
        {
            // 출금할 돈이 있다.
            player->PlusMoney(money, Player::eMONEY_CIRCULATING);
            guild_warehouse->MoneyLogDBSave(money, MONEY_OUT);

            GAMELOG->LogMoney(MONEY_GUILD_WAREHOUSE_GET, player, money);
        }
        else {
            result = RC::RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT;
        }
    }
    else {
        result = RC::RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT;
    };
    //----------------------------------------------------------------------------------------------
    if (RC::RC_GUILD_SUCCESS == result)
    {
        MSG_CG_GUILD_WAREHOUSE_MONEY_ACK msg;
        msg.m_InventoryMoney = player->GetMoney();
        msg.m_WarehouseMoney = guild_warehouse->GetCurMoney();
        player->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        MSG_CG_GUILD_WAREHOUSE_MONEY_NAK msg;
        msg.m_dwErrorCode = result;
        player->SendPacket(&msg, sizeof(msg));
    }
}

Handler_CG_GUILD_IMPL(CG_GUILD_NAME_CHANGE_SYN)
{
    UNREFERENCED_PARAMETER(pServerSession);
    UNREFERENCED_PARAMETER(wSize);

	MSG_CG_GUILD_NAME_CHANGE_SYN* pMsg = (MSG_CG_GUILD_NAME_CHANGE_SYN*)pRecvMsg;
	
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_GUILD_IMPL( CG_GUILD_NAME_CHANGE_SYN )] Not Exist GameField" );
		return;
	}

	ItemManager* pItem = pPlayer->GetItemManager();

	SLOTIDX	atIndex = pMsg->m_atIndex;
	POSTYPE	atPos   = pMsg->m_atPos;

	//아이템 체크
	
	if(  SI_INVENTORY != atIndex || !pItem->ValidPos(atIndex,atPos,TRUE) )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = RC::RC_ITEM_INVALIDPOS;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	
	SCSlotContainer * pAtContainer = pItem->GetItemSlotContainer( atIndex );
	SCItemSlot & rAtSlot = (SCItemSlot &)pAtContainer->GetSlot( atPos );

	RC::eITEM_RESULT rcResult = pPlayer->CanUseItem( rAtSlot.GetItemInfo() );
	if( rcResult != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = rcResult;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	const BASE_ITEMINFO* const pInfo = rAtSlot.GetItemInfo();

	if( eITEMTYPE_CHANGE_GUILD_NAME != pInfo->m_wType )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = RC::RC_ITEM_INVALIDSTATE;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	//.// 길드체크

	RC::eGUILD_RESULT rcGuildResult = g_GameGuildManager.CanChangeGuildName( pPlayer->GetGuildGuid(), pPlayer->GetCharGuid(), pMsg->m_tszGuildName );
	if( rcGuildResult != RC::RC_GUILD_SUCCESS  )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = rcGuildResult;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	//.// 슬롯 락

	if( pAtContainer->IsLocked(atPos) )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = RC::RC_ITEM_UNUSABLE_FUNCTION;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));
		return;
	}

	pAtContainer->SetLock( atPos, TRUE );

	//.//

	MSG_GZ_GUILD_NAME_CHANGE_SYN msg;
	msg.m_TargetPos		= atPos;
	msg.m_GuildGuid		= pPlayer->GetGuildGuid();
	msg.m_OperCharGuid  = pPlayer->GetCharGuid();
	memcpy( msg.m_tszGuildName, pMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
	msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	if( FALSE == pPlayer->SendToGuildServer( &msg, sizeof(msg) ) )
	{
		MSG_CG_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_bErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pPlayer->SendPacket(&nmsg,sizeof(nmsg));

		pAtContainer->SetLock( atPos, FALSE );
		return;
	}

    //__CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
	WORD wCoolTimeType = rAtSlot.GetItemInfo()->m_wCoolTimeType;
	pPlayer->ResetUseItem( wCoolTimeType );
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL

Handler_CG_GUILD_IMPL(CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ)
{
    MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ* recv_msg = 
        static_cast<MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);
    if (player->GetField() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not exist field");
        return;
    }

    GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == 0)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Is not guild member : name = %s",
            player->GetCharName()
            );
        return;
    }

    MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ send_msg;
    send_msg.request_guild_guid_ = player->GetGuildGuid();
    send_msg.request_char_guid_ = player->GetCharGuid();
    player->SendToGuildServer(&send_msg, sizeof(send_msg));
}
#endif

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
Handler_CG_GUILD_IMPL(CG_GUILD_INTRODUCTION_SAVE_SYN)
{
    MSG_CG_GUILD_INTRODUCTION_SAVE_SYN* packet = \
        static_cast<MSG_CG_GUILD_INTRODUCTION_SAVE_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    MSG_GZ_GUILD_INTRODUCTION_SAVE_SYN msg;
    msg.init();
    msg.char_guid = player->GetCharGuid();
    msg.guild_guid = player->GetGuildGuid();
    memcpy(&msg.guild_introductions_, &packet->guild_introductions_, \
        sizeof(packet->guild_introductions_));
    player->SendToGuildServer(&msg, sizeof(msg));
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION


#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

Handler_CG_GUILD_IMPL(CG_GUILD_JOIN_REQUEST_SYN)
{
    MSG_CG_GUILD_JOIN_REQUEST_SYN* packet = \
        static_cast<MSG_CG_GUILD_JOIN_REQUEST_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player->GetGuildGuid() != 0)
    {
        MSG_CG_GUILD_JOIN_REQUEST_ACK ack_msg;
        ack_msg.guild_guid = packet->guild_guid;
        ack_msg.result_ = RC::RC_GUILD_JOIN_REQUEST_GUILD_EXIST;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    GUILD_JOIN_REQUEST_INFO guild_join_request_info;
    guild_join_request_info.m_guild_guid = packet->guild_guid;
    guild_join_request_info.m_char_guid  = player->GetCharGuid();
    guild_join_request_info.m_char_level = player->GetLevel();
    guild_join_request_info.m_char_class = player->GetCharType();
    _tcsncpy(guild_join_request_info.m_char_name, player->GetCharName(), MAX_CHARNAME_LENGTH);

    MSG_GZ_GUILD_JOIN_REQUEST_SYN msg;
    msg.request_info = guild_join_request_info;
    player->SendToGuildServer(&msg, sizeof(msg));
}

Handler_CG_GUILD_IMPL(CG_GUILD_JOIN_REQUEST_APPROVE_SYN)
{
    MSG_CG_GUILD_JOIN_REQUEST_APPROVE_SYN* packet = \
        static_cast<MSG_CG_GUILD_JOIN_REQUEST_APPROVE_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player == NULL)
    {
        return;
    }

    if (player->GetField() == NULL || player->GetField()->GetGameZone() == NULL)
    {
        return;
    }

    if (player->GetGuildGuid() == 0)
    {
        return;
    }

    MSG_GZ_GUILD_JOIN_SYN send_msg;
    send_msg.m_GuildGuid = player->GetCharInfo()->m_GuildGuid;
    send_msg.m_OperCharGuid	= player->GetCharGuid();
    send_msg.m_TargetCharGuid = packet->char_guid;
    _tcsncpy(send_msg.m_tszCharName, packet->char_name, _countof(send_msg.m_tszCharName));
    send_msg.m_tszCharName[_countof(send_msg.m_tszCharName) - 1] = _T('\0');
    send_msg.m_byClass = packet->char_class;
    send_msg.m_Level = packet->char_level;
    player->SendToGuildServer(&send_msg, sizeof(send_msg));
}

Handler_CG_GUILD_IMPL(CG_GUILD_JOIN_REQUEST_REJECT_SYN)
{
    MSG_CG_GUILD_JOIN_REQUEST_REJECT_SYN* packet = \
        static_cast<MSG_CG_GUILD_JOIN_REQUEST_REJECT_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player == NULL)
    {
        return;
    }

    if (player->GetField() == NULL || player->GetField()->GetGameZone() == NULL)
    {
        return;
    }

    if (player->GetGuildGuid() == 0)
    {
        return;
    }

    MSG_GZ_GUILD_JOIN_REQUEST_REJECT_SYN msg;
    msg.reject_type = eGUILD_JOIN_REJECT_TYPE_NORMAL;
    msg.char_guid = player->GetCharGuid();
    msg.guild_guid = player->GetGuildGuid();
    msg.target_guid = packet->char_guid;
    player->SendToGuildServer(&msg, sizeof(msg));
}


Handler_CG_GUILD_IMPL(CG_GUILD_FACILITY_UPGRADE_SYN)
{
    MSG_CG_GUILD_FACILITY_UPGRADE_SYN* packet = \
        static_cast<MSG_CG_GUILD_FACILITY_UPGRADE_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player == NULL)
    {
        return;
    }
    if (player->GetField() == NULL || player->GetField()->GetGameZone() == NULL)
    {
        return;
    }
    if (player->GetGuildGuid() == 0)
    {
        return;
    }

    GameGuild* game_guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
    if (game_guild == NULL)
    {
        return;
    }

    RC::eGUILD_RESULT result = game_guild->CanUpdateGuildFacility(player, packet->facility_code);
    if (result != RC::RC_GUILD_SUCCESS)
    {
        MSG_CG_GUILD_FACILITY_UPGRADE_ACK ack_msg;
        ack_msg.facility_code = packet->facility_code;
        ack_msg.result = result;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    MSG_GZ_GUILD_FACILITY_UPGRADE_SYN msg;
    msg.facility_code = packet->facility_code;
    msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.char_guid = player->GetCharGuid();
    msg.guild_guid = player->GetGuildGuid();
    player->SendToGuildServer(&msg, sizeof(msg));
}

Handler_CG_GUILD_IMPL(CG_GUILD_FACILITY_ACTIVE_SYN)
{
    MSG_CG_GUILD_FACILITY_ACTIVE_SYN* packet = \
        static_cast<MSG_CG_GUILD_FACILITY_ACTIVE_SYN*>(pRecvMsg);

    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    if (player == NULL)
    {
        return;
    }
    if (player->GetField() == NULL || player->GetField()->GetGameZone() == NULL)
    {
        return;
    }
    if (player->GetGuildGuid() == 0)
    {
        return;
    }

    DWORD result = g_GameGuildManager.ApplyGuildFacilityActive(player, packet->facility_code);
    MSG_CG_GUILD_FACILITY_ACTIVE_ACK ack_msg;
    ack_msg.facility_code = packet->facility_code;
    ack_msg.result = result;
    player->SendPacket(&ack_msg, sizeof(ack_msg));
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION