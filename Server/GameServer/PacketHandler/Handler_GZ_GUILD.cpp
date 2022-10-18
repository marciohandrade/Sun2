#include "StdAfx.h"
#include ".\handler_gz_guild.h"
#include <PacketStruct_GZ.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>
#include <PacketStruct_DG.h>
#include <BitStream.hpp>
#include <SCItemSlot.h>
#include <GuildRankInfoParser.h>

#include ".\Player.h"
#include ".\GameGuildManager.h"
#include ".\GameGuild.h"
#include ".\GameGuildMember.h"

#include "GameGuildRelation.h"
#include "GameGuildExtension.h"

#include "GameParty/NewGamePartyManager.h"

#include "ItemManager.h"
#include "GameDominationManager.h"
#include <ResultCode.h>
#include "GameDominationManager.h"
#include "DominationArea.h"
#include "GuildInfoParser.h"
//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
#include "DominationMember.h"

#include <PointSystem/PacketStruct_CG_PointSystem.h>
#include <PacketStruct_PointSystem.h>
#include <CurrencyInfoParser.h>
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "GameInstanceDungeon.h"
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#include "GameDominationField.h"
#include "GameZoneManager.h"
#include "GuildFacilities.h"
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#include "SunRanking/SunRankingGradeManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_GZ_GUILD::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_GZ_GUILD::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
    #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
        HANDLER_NODE_INFO(DG_CONNECTION, DG_CONNECTION_GROUND_CMD, PHASE_SERVICE),
    #endif
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INFO_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_SELECT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WITHDRAW_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_SELECT_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WITHDRAW_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_KICKPLAYER_NAK, PHASE_SERVICE),

        // 길드포인트 관련
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DONATE_UP_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DONATE_UP_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RANKUP_BRD, PHASE_SERVICE),

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHUNTING_PVP_RESULT_BRD, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_INFO_BRD, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_BOTH_RELATION_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_DELETE_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_DELETE_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_DUTY_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_DUTY_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_MASTER_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_MASTER_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_CORPS_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_CORPS_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_CORPS_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_CORPS_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_BRD, PHASE_SERVICE),		// 길드 군단장 변경
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_CORPS_BRD, PHASE_SERVICE),			// 길드 군단 이동
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_CORPS_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_BRD, PHASE_SERVICE),		// 길드 훈련캠프 창설
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_BRD, PHASE_SERVICE),		// 길드 훈련캠프 해산
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INVITE_ACK, PHASE_SERVICE),					// 길드 초대 
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INVITE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_LOGIN_CHAR_BRD, PHASE_SERVICE),				// 길드원 로그인
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_LOGIN_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_LOGOUT_CHAR_BRD, PHASE_SERVICE),				// 길드원 로그아웃
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHAR_LEVELUP_BRD, PHASE_SERVICE),			// 길드원 레벨업
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_KICKPLAYER_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_ACK, PHASE_SERVICE),			// 길드정보 요청
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_BRD, PHASE_SERVICE),			// GM명령어 길드포인트 세팅
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_BRD, PHASE_SERVICE),		// 길드원 카오상태 변경.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_ENTER_SERVER_ACK, PHASE_SERVICE),			// 길드원 서버간 이동.

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_WAIT_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_NAME_CHANGE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_NAME_CHANGE_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_NAME_CHANGE_NAK, PHASE_SERVICE),
        //{_KR_001385_20091013_DOMINATION_BASICINFO_SETTING
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_AUCTION_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_UPDATE_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_ACCEP_HEIM_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_INIT_ACTIONINFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_BATTING_HEIM_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_RETURNHIME_ACK, PHASE_SERVICE),
        //}
        //_NA_20100604_BETTER_DOMINATION_MANAGER
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_UPDATE_GUILDINFO_ACK, PHASE_SERVICE),
        //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD, PHASE_SERVICE),
        //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK, PHASE_SERVICE),

        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_JOIN_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_LEAVE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_ANS, PHASE_SERVICE),
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILDPOINTSYSTEM, PHASE_SERVICE),
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INTRODUCTION_SAVE_ACK, PHASE_SERVICE),
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RENAME_CHAR_CMD, PHASE_SERVICE),

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_CONTINENT_LORD_INFO_ACK, PHASE_SERVICE),
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_REJECT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELOAD_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_FACILITY_UPGRADE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_FACILITY_SELECT_ACK, PHASE_SERVICE),
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_UPDATE_ACK, PHASE_UNLIMITED),
#endif //_NA_008334_20150608_SONNENSCHEIN

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_GZ_GUILD_IMPL(DG_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pRecvMsg, wSize));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_INFO_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_INFO_BRD * pMsg = (MSG_GZ_GUILD_INFO_BRD *)pRecvMsg;
}

//길드 해산 
Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_BRD* receive_msg = static_cast<MSG_GZ_GUILD_DESTROY_BRD*>(pRecvMsg);

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_BRD)|MSG_GZ_GUILD_DESTROY_BRD 받음|PlayerKey:%u, GuildGuid:%d|"), 
        receive_msg->m_dwKey, receive_msg->m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

	// 현재 명령을 내린 플레이어가 있든 없든 길드 해산 시켜야 한다.
	g_GameGuildManager.DestroyGuild(receive_msg->m_GuildGuid);

	//(shogen)(20090819)
	//Player* player = PlayerManager::Instance()->FindPlayerByUserKey( receive_msg->m_dwKey );
	Player* player = PlayerManager::Instance()->FindPlayerByCharKey(receive_msg->m_dwKey);
	if (player == NULL)
    {
        return;
    }
    
	player->GetCharInfo()->m_GuildGuid = 0;
	player->GetCharInfo()->m_eGuildDuty = eGUILD_DUTY_NONE;

	MSG_AG_GUILD_DESTROY_ACK msg;
	player->SendPacket(&msg, sizeof(msg));

	MSG_CG_GUILD_DESTROY_BRD brd_msg;
	brd_msg.m_MasterPlayerKey = player->GetObjectKey();
	
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_BRD)|MSG_CG_GUILD_DESTROY_BRD 보냄|PlayerGuildGuid:%d, PlayerGuildDuty:%d, MasterPlayerKey:%u|"), 
           player->GetCharInfo()->m_GuildGuid, 
           player->GetCharInfo()->m_eGuildDuty, 
           brd_msg.m_MasterPlayerKey);
#endif // _USING_GUILD_DESTROY_LOGGING

    player->SendPacketAround(&brd_msg, sizeof(brd_msg), FALSE);
}

//길드 해산 실패
Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_NAK * pMsg = (MSG_GZ_GUILD_DESTROY_NAK *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	MSG_CG_GUILD_DESTROY_NAK msg;
	msg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

//길드 가입
Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_JOIN_BRD* pMsg = (MSG_GZ_GUILD_JOIN_BRD*)pRecvMsg;

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    if (SunRankingGradeManager::Instance()->IsSettleChannel())
    {
        SunRankingManager::Instance()->SetGuildGuid(pMsg->m_MemberInfo.m_CharGuid, pMsg->m_GuildGuid);
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	//현 서버에 해당 길드가 존재하지 않으면 실행할 필요 없다.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )		return;

	//명령자가 존재하면..
	Player* pOperPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pOperPlayer )
	{
		MSG_CG_GUILD_JOIN_ACK ackMsg;
		pOperPlayer->SendPacket( &ackMsg, sizeof(ackMsg) );
	}

	//가입자가 존재하면..
	Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByName( pMsg->m_MemberInfo.m_tszCharName );
	if( pTargetPlayer )
	{
		RC::eGUILD_RESULT rc = g_GameGuildManager.JoinGuild( pMsg->m_GuildGuid, pMsg->m_MemberInfo );
		if( rc != RC::RC_GUILD_SUCCESS )
		{
			SUNLOG( eCRITICAL_LOG, "Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_ACK) Fail, rc = %d", rc );
			return;
		}

		// 해당 유저의 정보에 세팅한다.
		pTargetPlayer->GetCharInfo()->m_GuildGuid = pMsg->m_GuildGuid;
		util::GUILDNAMECOPY(pTargetPlayer->GetCharInfo()->m_tszGuildName, pGuild->GetGuildName());
		

		// GuildServer로 부터 ack를 받은후 ack보냄
		// 길드 번호를 BRD에서 전송하는 것도 괜찮을 듯..우선은 이대로...
		MSG_AG_GUILD_JOIN_SUCCESS_CMD msg;
		msg.m_GuildGuid = pMsg->m_GuildGuid;
		pTargetPlayer->SendPacket( &msg, sizeof(msg) );

		// 길드 관계 리스트를 보내서 주변 유저들중에 관계를 맺고 있는 유저들의 렌더정보를 표시해주도록한다.
		MSG_CG_GUILD_ADD_INFO_ACK msgAddInfo;
		msgAddInfo.m_AddInfo.m_GuildGuid = pMsg->m_GuildGuid;
		GuildSmallContentEntry::RelationSystem::LoadRelationInfo(&msgAddInfo.m_AddInfo);
		msgAddInfo.m_byRelCnt = msgAddInfo.m_AddInfo.m_byRelationGuildCnt;
		pTargetPlayer->SendPacket(&msgAddInfo, msgAddInfo.GetSize());

        // 길드 가입 후 주변 유저들이 길드 가입에 관련된 최소 정보
        // (길드 이름, 길드 마크, 길드 마크테두리, 길드 등급)를 볼수 있도록 해야한다.
        pTargetPlayer->FirstGuildMemberEnter();

		// 길드가입한 놈이 길드정보가 필요하면, 자동으로 G를 누르게 해서 관련 정보요청 패킷을 클라에서 보내도록 하자.
		MSG_CG_GUILD_JOIN_CMD	cmdMsg;
		cmdMsg.m_GuildGuid = pMsg->m_GuildGuid;
		pTargetPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		// 파티중일때 초대 받을 경우 파티멤버중에 현재 가입한 길드와 
		// 적대 관계인 길드 멤버가 존재하면  파티에서 나간다.
		WORD PartyKey = pTargetPlayer->GetPartyState().GetPartyKey();
		BOOL bIsExistHostilityMember = FALSE;

		bIsExistHostilityMember = NewGamePartyManager::Instance()->IsExistHostilityMember( PartyKey, pTargetPlayer->GetGuildGuid() );

		if( bIsExistHostilityMember )
		{
			MSG_AG_PARTY_LEAVE_BY_GUILDRELATION_SYN msg;
			msg.m_dwMemberKey =pTargetPlayer->GetObjectKey(); 
			msg.m_wPartykey = PartyKey;
			msg.m_bReason= eREASON_LEAVE_PARTY_BY_GUILDRELATION;
			g_pGameServer->SendToServer(AGENT_SERVER, &msg, sizeof(msg));
		}
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        g_GameGuildManager.ApplyGuildFacilityPassive(pTargetPlayer, true);
        g_GameGuildManager.SendGuildFacilityTotalInfo(pTargetPlayer, pMsg->m_GuildGuid);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
        pGuild->ApplyGuildRewardOption(pTargetPlayer);
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pTargetPlayer);  // 길드 가입 보상옵션 적용
#endif //_NA_008334_20150608_SONNENSCHEIN
        GameDominationManager::Instance()->SendAreaInfoToPlayer(pTargetPlayer);

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        // 카오스 존 내부에서 길드 가입시 인장 효과 제거..
        if(pTargetPlayer->GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)
        {
            GameInstanceDungeon* const instance_dungeon = \
                static_cast<GameInstanceDungeon*>(pTargetPlayer->GetGameZonePtr());
            if (instance_dungeon->GetTypeOfIndun() == INDUN_KIND_CHAOS_ZONE)
            {
#ifdef _NA_008334_20150608_SONNENSCHEIN
                pGuild->ApplyGuildRewardOption(pTargetPlayer, FALSE);
#else
                GameDominationManager::Instance()->ApplyAllRewardOption(pTargetPlayer, false);
#endif //_NA_008334_20150608_SONNENSCHEIN
                GameDominationManager::Instance()->SendAreaInfoToPlayer(pTargetPlayer, false);
            }
        }
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

        GameField* pField = pTargetPlayer->GetField();
        if (NULL != pField)
        {
            const FIELDCODE fCode = pField->GetFieldCode();
            GameDominationManager::Instance()->ApplyRewardField(pTargetPlayer,fCode);
        }
    }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    else
    {
        RC::eGUILD_RESULT rc = g_GameGuildManager.JoinGuild( pMsg->m_GuildGuid, pMsg->m_MemberInfo );
        if( rc != RC::RC_GUILD_SUCCESS )
        {
            SUNLOG( eCRITICAL_LOG, "Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_ACK) Fail, rc = %d", rc );
            return;
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	//길드원이 가입 했다고 길드원에게 알려준다.( 이번에 가입한 놈한테는 보내지 않는다. )
	MSG_CG_GUILD_JOIN_BRD brdMsg;
	brdMsg.m_MemberInfo = pMsg->m_MemberInfo;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg), pMsg->m_MemberInfo.m_CharGuid );
}



Handler_GZ_GUILD_IMPL(GZ_GUILD_WITHDRAW_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_WITHDRAW_BRD * pMsg = (MSG_GZ_GUILD_WITHDRAW_BRD *)pRecvMsg;

	g_GameGuildManager.LeaveGuild( pMsg->m_GuildGuid, pMsg->m_CharGuid );

	CHARGUID TargetGuid = 0;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        g_GameGuildManager.ApplyGuildFacilityPassive(pPlayer, false);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
        GameGuild* player_guild = pPlayer->GetGuild();
        if (player_guild != NULL) {
            player_guild->ApplyGuildRewardOption(pPlayer, FALSE);
        }
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pPlayer,false);  // 길드 탈퇴 보상옵션 제거
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        // 기부한 길드코인을 삭제
        PointWalletManager* const pointwallet_manager = pPlayer->GetPointWalletManager();
        const DWORD code = CURRENCY_INFO_DONATION_GUILDCOIN;
        INT donation_coin = pointwallet_manager->GetPointWallet(code);
        if (donation_coin > 0) {
            pointwallet_manager->ChangeWalletPoint(code, -donation_coin);
        }
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

        GameField*  pField = pPlayer->GetField();
        if (NULL != pField)
        {
            const FIELDCODE fCode = pField->GetFieldCode();
            GameDominationManager::Instance()->ApplyRewardField(pPlayer,fCode,false);
        }

		pPlayer->GetCharInfo()->m_GuildGuid		= 0;
		pPlayer->GetCharInfo()->m_eGuildDuty		= (eGUILD_DUTY)0;
		pPlayer->GetCharInfo()->m_GuildPenaltyTime = pMsg->m_MemberPenaltyTime;
		pPlayer->GetCharInfo()->m_ePenaltyType	= eGUILD_WITHDRAW;
		TargetGuid = pPlayer->GetCharGuid();

		// 이미 길드에서 탈퇴했기 때문에 당사자 한테 알려준다.
		MSG_CG_GUILD_WITHDRAW_ACK msgACK;
		msgACK.m_byPenaltyType = pMsg->m_byPenaltyType;
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );

		//AgentServer의 해당 유저의 GuildGuid를 0으로 만들어 줘야 한다.
		MSG_AG_GUILD_WITHDRAW_ACK ACK;
		pPlayer->SendPacket( &ACK, sizeof(ACK) );

		//Render 정보 때문에 강퇴당한놈 주위에 
		MSG_CG_GUILD_LEAVE_BRD		brdMsg;
		brdMsg.m_LeaverObjectKey	= pPlayer->GetObjectKey();
		pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );
	}

	//해당 길드원들에게 멤버가 탈퇴 했음을 알린다.
	MSG_CG_GUILD_WITHDRAW_BRD msgBRD;
    _tcsncpy(
        msgBRD.m_szCharName, 
        pMsg->m_szCharName, 
        _countof(msgBRD.m_szCharName)
        );
	msgBRD.m_byPenaltyType = pMsg->m_byPenaltyType;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &msgBRD, sizeof(msgBRD), TargetGuid );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_ACK * pMsg = (MSG_GZ_GUILD_CREATE_ACK *)pRecvMsg;

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingManager::Instance()->SetGuildGuid(pMsg->m_dwKey, pMsg->m_GameGuildInfo.m_GuildGuid);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	// ack가 오기전에 나가면???어떻게 되지?? 돈 소비 없이 생성.
	// 미리 돈 차감 시킴
	// ack가 오기전에 나가도 Guild나 DB에 해당 정보가 저장되어 있기 때문에 다시 접속시 정상적인 정보를 받을수 있다.

	RC::eGUILD_RESULT rc = g_GameGuildManager.CreateGuild( pPlayer, pMsg->m_GameGuildInfo, pMsg->m_BaseMemberInfo );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_CG_GUILD_CREATE_NAK msg;
		msg.m_byErrorCode = rc;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		SUNLOG( eCRITICAL_LOG, "[Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_ACK)] Fail, Guid = %u, rc = %d", pMsg->m_GameGuildInfo.m_GuildGuid, rc );
		return;
	}

	// 에이전트로 알림 : guild guid 정보만 알린다
	MSG_AG_GUILD_CREATE_ACK msg;
	msg.m_GuildGuid = pMsg->m_GameGuildInfo.m_GuildGuid;
	pPlayer->SendPacket( &msg, sizeof(msg) );

	// 원래 AgentServer에서 보내 줬으나( 그게 더 맞긴한데, 사람이 명확하게 처리 하기 위해서 GUILD관련 패킷은 AGENT에서 처리하는게 없도록 하자. )
	MSG_CG_GUILD_CREATE_ACK msgACK;
	msgACK.m_GuildGuid = pMsg->m_GameGuildInfo.m_GuildGuid;
	pPlayer->SendPacket( &msgACK, sizeof(msgACK) );

	MSG_CG_GUILD_CREATE_BRD bmsg;
	bmsg.m_MasterPlayerKey = pPlayer->GetObjectKey();
	memcpy( bmsg.m_tszGuildName, pPlayer->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
	pPlayer->SendPacketAround( &bmsg, sizeof(bmsg), FALSE );

    //게임 서버에 길드가 생성된 후에 길드 시설 정보 요청
    MSG_GZ_GUILD_FACILITY_SELECT_SYN facility_msg;
    facility_msg.guild_guid = pMsg->m_GameGuildInfo.m_GuildGuid;
    pPlayer->SendToGuildServer(&facility_msg, sizeof(facility_msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_NAK * pMsg = (MSG_GZ_GUILD_CREATE_NAK *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	//최초 생성 길드 정보를 가지고 온다.
	const GuildRankInfo* pGuildInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( 1 );
	if( !pGuildInfo )		return;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    MONEY return_heim = GuildInfoParser::Instance()->GetGuildInfo().guild_create_heim_;
#else
    MONEY return_heim = pGuildInfo->m_dwHeim;
#endif

    pPlayer->PlusMoney(return_heim);

	MSG_CG_GUILD_CREATE_NAK msg;
	msg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

// 이미 길드정보를 받아온 상태이기 때문에
// 길드 정보를 쿼리하여 클라이언트에게 전달하면 된다.
Handler_GZ_GUILD_IMPL(GZ_GUILD_SELECT_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_SELECT_ACK * pMsg = (MSG_GZ_GUILD_SELECT_ACK *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	GameGuild * pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	ASSERT( pGuild ); if( !pGuild ) return;
}
//------------------------------------------------------------------------------------------------------
// NAK 
//------------------------------------------------------------------------------------------------------
// 길드서버에서 바로 길드 정보를 받아 오지 못했다.
// 클라이언트는 적절한 메세지를 출력한다.
Handler_GZ_GUILD_IMPL(GZ_GUILD_SELECT_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_SELECT_NAK* pMsg = (MSG_GZ_GUILD_SELECT_NAK *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;
}



Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_JOIN_NAK * pMsg = (MSG_GZ_GUILD_JOIN_NAK *)pRecvMsg;

	Player* pOperPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pOperPlayer )
	{
		//길드 가입이 실패 했으면 가입시키려는 자와, 가입하려는자에게 모두 보내야 한다.
		MSG_CG_GUILD_JOIN_NAK nakMsg;
		nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
		pOperPlayer->SendPacket( &nakMsg, sizeof(nakMsg) ); 
	}

	Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByName( pMsg->m_szTargetCharName );
	if( pTargetPlayer )
	{
		MSG_CG_GUILD_JOIN_FAILED_CMD cmdMsg;
		cmdMsg.m_byErrorCode = pMsg->m_byErrorCode;
		pTargetPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );
	}
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_WITHDRAW_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_WITHDRAW_NAK * pMsg = (MSG_GZ_GUILD_WITHDRAW_NAK *)pRecvMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	MSG_CG_GUILD_WITHDRAW_NAK msg;
	msg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_KICKPLAYER_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_KICKPLAYER_NAK *pMsg = (MSG_GZ_GUILD_KICKPLAYER_NAK *)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		MSG_CG_GUILD_KICKPLAYER_NAK msg;
		msg.m_dwKey = pMsg->m_dwKey;
        _tcsncpy(
            msg.m_ptszKickCharName, 
            pMsg->m_ptszCharName, 
            _countof(msg.m_ptszKickCharName)
            );
        msg.m_ptszKickCharName[_countof(msg.m_ptszKickCharName) - 1] = '\0';

		msg.m_dwErrorCode = pMsg->m_dwErrorCode;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
}

// 길드 공지 변경

// 길드에 UP 헌납
Handler_GZ_GUILD_IMPL(GZ_GUILD_DONATE_UP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DONATE_UP_BRD* pMsg = (MSG_GZ_GUILD_DONATE_UP_BRD*)pRecvMsg;

	// 해당 유저가 있을 경우 성공했다고 알려준다
	CHARGUID TargetGuid = 0;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		// 가지고 있는 포인터에서 빼주어야 함!!
        UPTYPE UpPoint = pPlayer->GetCharInfo()->m_UserPoint;
		if( UpPoint >= pMsg->m_DonateUP )
			pPlayer->GetCharInfo()->m_UserPoint = UpPoint - pMsg->m_DonateUP;
		else
			SUNLOG( eCRITICAL_LOG, "[GZ_GUILD_DONATE_UP_BRD] Error!![CharGuid:%u][DonateUP:%u]", pPlayer->GetCharGuid(), pMsg->m_DonateUP );

		TargetGuid = pPlayer->GetCharGuid();

		MSG_CG_GUILD_DONATE_UP_ACK msg;
		msg.m_RemainUP = pPlayer->GetCharInfo()->m_UserPoint;
		msg.m_GuildUP = pMsg->m_TotalUP;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	if( !g_GameGuildManager.FindGuild( pMsg->m_GuildGuid ) )
	{
		SUNLOG( eCRITICAL_LOG, "[GZ_GUILD_DONATE_UP_BRD] Guild Not Found [GuildGuid:%u]", pMsg->m_GuildGuid );
		return;
	}

	MSG_CG_GUILD_DONATE_UP_BRD	brdMsg;
	brdMsg.m_GuildUP = pMsg->m_TotalUP;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg), TargetGuid );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_DONATE_UP_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DONATE_UP_NAK* pMsg = (MSG_GZ_GUILD_DONATE_UP_NAK*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		MSG_CG_GUILD_DONATE_UP_NAK msg;
		msg.m_byErrorCode = pMsg->m_byErrorCode;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	MessageOut( eFULL_LOG, "GZ_GUILD_DONATE_UP_NAK[UserKey:%u]", pRecvMsg->m_dwKey );
}

// UP->GP로 변환
Handler_GZ_GUILD_IMPL(GZ_GUILD_CONVERT_UP2GP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CONVERT_UP2GP_BRD* pMsg = (MSG_GZ_GUILD_CONVERT_UP2GP_BRD*)pRecvMsg;
	CHARGUID TargetGuid = 0;

	// 해당 유저가 있을 경우 성공했다고 알려준다
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		MSG_CG_GUILD_CONVERT_UP2GP_ACK msg;
		msg.m_RemainUP = pMsg->m_RemainUP;
		msg.m_NowGP = pMsg->m_TotalGP;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		TargetGuid = pPlayer->GetCharGuid();
	}

	// 현재 게임서버에 해당 길드가 존재하면 패킷을 보낸다.
	if( !g_GameGuildManager.FindGuild( pMsg->m_GuildGuid ) )
		return;

	MSG_CG_GUILD_CONVERT_UP2GP_BRD msg;
	msg.m_RemainUP = pMsg->m_RemainUP;
	msg.m_NowGP = pMsg->m_TotalGP;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &msg, sizeof(msg), TargetGuid );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CONVERT_UP2GP_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CONVERT_UP2GP_NAK* pMsg = (MSG_GZ_GUILD_CONVERT_UP2GP_NAK*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		MSG_CG_GUILD_CONVERT_UP2GP_NAK msg;
		msg.m_byErrorCode = pMsg->m_byErrorCode;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}

	MessageOut( eFULL_LOG, "GZ_GUILD_CONVERT_UP2GP_NAK[UserKey:%u][ErrorCode:%u]", pRecvMsg->m_dwKey, pMsg->m_byErrorCode );
}

// 길드 랭크업
Handler_GZ_GUILD_IMPL(GZ_GUILD_RANKUP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_RANKUP_BRD* pMsg = (MSG_GZ_GUILD_RANKUP_BRD*)pRecvMsg;

	CHARGUID OperCharGuid = 0;
	g_GameGuildManager.RankUpGuild( pMsg->m_GuildGuid, pMsg->m_byLevel );

#if !defined(_NA004034_20120102_GUILD_POINT_SYSTEM)
	// 해당 유저가 있을 경우 성공했다고 알려준다
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		MSG_CG_GUILD_RANKUP_ACK msg;
		msg.m_byNowLevel = pMsg->m_byLevel;
		msg.m_RemainGP = pMsg->m_RemainGP;
		msg.m_RemainUP = pMsg->m_RemainUP;
		msg.m_RemainMoney = pPlayer->GetMoney();
		pPlayer->SendPacket( &msg, sizeof(msg) );
		OperCharGuid = pPlayer->GetCharGuid();
	}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

	MSG_CG_GUILD_RANKUP_BRD	brdMsg;
	brdMsg.m_byNowLevel = pMsg->m_byLevel;
	brdMsg.m_RemainGP = pMsg->m_RemainGP;
	brdMsg.m_RemainUP = pMsg->m_RemainUP;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg), OperCharGuid );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
Handler_GZ_GUILD_IMPL(GZ_GUILD_CHUNTING_PVP_RESULT_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHUNTING_PVP_RESULT_BRD * pMsg = (MSG_GZ_GUILD_CHUNTING_PVP_RESULT_BRD *)pRecvMsg;

	g_GameGuildManager.SetChuntingPVPInfo( pMsg->m_GuildGuid, pMsg->m_PVPInfo );
}

#endif



//길드 직책
Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_DUTY_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_DUTY_BRD *pMsg = (MSG_GZ_GUILD_CHANGE_DUTY_BRD *)pRecvMsg;

	// 현재 타겟이 되는 멤버는 접속해 있을수도 있고 없을수도 있다.
	// 따라서 현재 PlayerManager에서 이름으로 검색할 수 없을 경우, 현재 게임서버에서는 해당 타겟멤버가 없는 것이다.
	// 당연히 게임길드에도 타켓 멤버 없는것으로 한다.( 없어야 한다. )
	Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByName( pMsg->m_szTargetCharName );
	if( pTargetPlayer )
	{
		// 현재 게임서버에 현재 길드가 있으면 해당 정보를 바꾼다.
		g_GameGuildManager.ChangeMemberDuty( pMsg->m_GuildGuid, pTargetPlayer->GetCharGuid(), (eGUILD_DUTY)pMsg->m_byDuty );
	}

	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )		return;

	MSG_CG_GUILD_CHANGE_DUTY_BRD	brdMsg;
	brdMsg.m_byDuty = pMsg->m_byDuty;
	_tcsncpy(
        brdMsg.m_szTargetCharName, 
        pMsg->m_szTargetCharName, 
        _countof(brdMsg.m_szTargetCharName)
        );
    brdMsg.m_szTargetCharName[_countof(brdMsg.m_szTargetCharName) - 1] = '\0';
	pGuild->SendPacketAll( &brdMsg, sizeof(brdMsg) );
    
    /* 클라이언트에서 처리하지 않음을 확인하여 주석 처리
	Player* pOperPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( pOperPlayer )
	{
		MSG_CG_GUILD_CHANGE_DUTY_ACK	Ack;
		Ack.m_byDuty = pMsg->m_byDuty;
		strncpy( Ack.m_szTargetCharName, pMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
		pOperPlayer->SendPacket( &Ack, sizeof(Ack) );
	}
    */
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_DUTY_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_DUTY_NAK *pMsg = (MSG_GZ_GUILD_CHANGE_DUTY_NAK *)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer) return;

	MSG_CG_GUILD_CHANGE_DUTY_NAK	Nak;
	Nak.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &Nak, sizeof(Nak) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RIGHT_DELEGATION_LIST_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_ACK *pMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_ACK *)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer) return;

	MSG_CG_GUILD_RIGHT_DELEGATION_LIST_ACK	Ack;
    _tcsncpy(
        Ack.m_szTargetCharName, 
        pMsg->m_szTargetCharName, 
        _countof(Ack.m_szTargetCharName)
        );
    Ack.m_szTargetCharName[_countof(Ack.m_szTargetCharName) - 1] = '\0';
	CopyMemory( Ack.m_DelegateRight, pMsg->m_DelegateRight, sizeof(Ack.m_DelegateRight) );
	pPlayer->SendPacket( &Ack, sizeof(Ack) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RIGHT_DELEGATION_LIST_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_NAK *pMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_NAK *)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer) return;

	MSG_CG_GUILD_RIGHT_DELEGATION_LIST_NAK	Nak;
	Nak.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &Nak, sizeof(Nak) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RIGHT_DELEGATION_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_RIGHT_DELEGATION_BRD* pMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_BRD*)pRecvMsg;

	Player* pOperPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( pOperPlayer )
	{
		// 명령한 놈한테도 보내주고
		MSG_CG_GUILD_RIGHT_DELEGATION_ACK	Ack;
        _tcsncpy(
            Ack.m_szTargetCharName, 
            pMsg->m_szTargetCharName, 
            _countof(Ack.m_szTargetCharName)
            );
        Ack.m_szTargetCharName[_countof(Ack.m_szTargetCharName) - 1] = '\0';
		pOperPlayer->SendPacket( &Ack, sizeof(Ack) );
	}

	Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByName( pMsg->m_szTargetCharName );
	if( pTargetPlayer )
	{
		// 권한을 받은 놈 한테도 보내 준다.
		MSG_CG_GUILD_RIGHT_DELEGATION_CMD	cmdMsg;
		pTargetPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );
	}
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RIGHT_DELEGATION_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_RIGHT_DELEGATION_NAK* pMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_NAK*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer) return;

	MSG_CG_GUILD_RIGHT_DELEGATION_NAK	Nak;
	Nak.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &Nak, sizeof(Nak) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_MASTER_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	//이 패킷을 받은 게임서버는 해당 유저가 있든 없든지... 무조건 해당 길드정보를 세팅해야 한다.
	MSG_GZ_GUILD_CHANGE_MASTER_BRD* pMsg = (MSG_GZ_GUILD_CHANGE_MASTER_BRD*)pRecvMsg;

	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( pGuild )
	{
		g_GameGuildManager.ChangeMaster( pMsg->m_GuildGuid, pMsg->m_OldMasterCharGuid, pMsg->m_NewMasterCharGuid );
		MSG_CG_GUILD_CHANGE_MASTER_BRD	brdMsg;

        _tcsncpy(
            brdMsg.m_szOldMasterName, 
            pMsg->m_szOldMasterCharName, 
            _countof(brdMsg.m_szOldMasterName)
            );
        brdMsg.m_szOldMasterName[_countof(brdMsg.m_szOldMasterName) - 1] = '\0';

        _tcsncpy(
            brdMsg.m_szNewMasterName, 
            pMsg->m_szNewMasterCharName, 
            _countof(brdMsg.m_szNewMasterName)
            );
        brdMsg.m_szNewMasterName[_countof(brdMsg.m_szNewMasterName) - 1] = '\0';

        pGuild->SendPacketAll( &brdMsg, sizeof(brdMsg) );

        GameDominationManager::Instance()->ChangeGuildMaster( pMsg->m_GuildGuid, pMsg->m_szNewMasterCharName );
	}    
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_MASTER_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_MASTER_NAK* pMsg = (MSG_GZ_GUILD_CHANGE_MASTER_NAK*)pRecvMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer) return;

	MSG_CG_GUILD_CHANGE_MASTER_NAK	Nak;
	Nak.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &Nak, sizeof(Nak) );
}


//길드 군단 생성
Handler_GZ_GUILD_IMPL(GZ_GUILD_EMPTY_COMMANDER_INFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_ACK* pMsg = (MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_ACK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_EMPTY_COMMANDER_INFO_ACK	AckMsg;
	AckMsg.m_byEmptyCommanderCount = pMsg->m_byEmptyCommanderCount;
	CopyMemory( AckMsg.m_byCorpsNum, pMsg->m_byCorpsNum, sizeof(BYTE)*MAX_GUILD_CORPS_NUM );
	pPlayer->SendPacket( &AckMsg, AckMsg.GetSize() );	
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_EMPTY_COMMANDER_INFO_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_NAK* pMsg = (MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_EMPTY_COMMANDER_INFO_NAK	NakMsg;
	NakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_CORPS_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_CORPS_BRD* pMsg = (MSG_GZ_GUILD_CREATE_CORPS_BRD*)pRecvMsg;

	// 전 길드 멤버한테 알려준다.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_CORPS_BRD)] Guild Not Exist, Guid = %d", pMsg->m_GuildGuid );
		return;
	}

	Player* pTarget = PlayerManager::Instance()->FindPlayerByName( pMsg->m_szTargetCharName );
	if( pTarget )
	{
		//게임 서버는 현재 게임서버에 접속중이 멤버의 정보만 가지고 있다.
		//따라서 새로운 군단장의 직책과 군단번호를 바꾸어 준다.
		//게임서버에서는 CharGuid로 Player를 검색할 수 없다. 따라서 이름을 패킷으로 가져온다.
		g_GameGuildManager.ChangeCommander( pMsg->m_GuildGuid, pTarget->GetCharGuid(), pMsg->m_byCorpsNum );
	}

	MSG_CG_GUILD_CREATE_CORPS_BRD	brdMsg;
	brdMsg.m_byCorpsNum = pMsg->m_byCorpsNum;
	_tcsncpy(
        brdMsg.m_szTargetCharName, 
        pMsg->m_szTargetCharName, 
        _countof(brdMsg.m_szTargetCharName)
        );
    // NOTE: cg message is not null terminated.
	pGuild->SendPacketAll( &brdMsg, sizeof(brdMsg) );
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_CORPS_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_CORPS_NAK* pMsg = (MSG_GZ_GUILD_CREATE_CORPS_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_CREATE_CORPS_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );	
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_CORPS_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_CORPS_BRD* pMsg = (MSG_GZ_GUILD_DESTROY_CORPS_BRD*)pRecvMsg;

	// 전 길드 멤버한테 알려준다.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_CORPS_BRD)] Guild Not Exist, Guid = %d", pMsg->m_GuildGuid );
		return;
	}

	//게임서버에서 군단 정보는 따로 관리 하지 않으므로, 육성캠프만 관리한다.
	pGuild->DestroyRearCamp( pMsg->m_byCorpsNum );

	MSG_CG_GUILD_DESTROY_CORPS_BRD	brdMsg;
	brdMsg.m_byCorpsNum = pMsg->m_byCorpsNum;
	pGuild->SendPacketAll( &brdMsg, sizeof(brdMsg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_CORPS_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_CORPS_NAK* pMsg = (MSG_GZ_GUILD_DESTROY_CORPS_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_DESTROY_CORPS_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );	
}

// 길드 군단장 변경
Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_COMMANDER_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_COMMANDER_BRD* pMsg = (MSG_GZ_GUILD_CHANGE_COMMANDER_BRD*)pRecvMsg;

	//** 명령을 내린놈의 게임서버와 군단장이 된놈의 게임서버가 다를 경우가 있다.

	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )		return;
    // NOTE: f110502.4L, fixed a problem that is not found player if full name guild.
	Player* pTarget = PlayerManager::Instance()->FindPlayerByName( pMsg->m_szTargetCharName );
	if( pTarget )
	{
		//게임 서버는 현재 게임서버에 접속중이 멤버의 정보만 가지고 있다.
		//따라서 새로운 군단장의 직책과 군단번호를 바꾸어 준다.
		//게임서버에서는 CharGuid로 Player를 검색할 수 없다. 따라서 이름을 패킷으로 가져온다.
		g_GameGuildManager.ChangeCommander( pMsg->m_GuildGuid, pTarget->GetCharGuid(), pMsg->m_byCorpsNum );
	}

	MSG_CG_GUILD_CHANGE_COMMANDER_BRD	brdMsg;
	brdMsg.m_byCorpsNum	= pMsg->m_byCorpsNum;
    _tcsncpy(
        brdMsg.m_szTargetCharName, 
        pMsg->m_szTargetCharName,
        _countof(brdMsg.m_szTargetCharName)
        );
    // NOTE: cg message is not null-terminated.
	pGuild->SendPacketAll( &brdMsg, sizeof( brdMsg ) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_COMMANDER_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_COMMANDER_NAK* pMsg = (MSG_GZ_GUILD_CHANGE_COMMANDER_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_CHANGE_COMMANDER_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );	
}

// 길드 군단 이동
Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_CORPS_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_CORPS_BRD* pMsg = (MSG_GZ_GUILD_CHANGE_CORPS_BRD*)pRecvMsg;

	g_GameGuildManager.ChangeCorps( pMsg->m_GuildGuid, pMsg->m_TargetCharGuid, pMsg->m_byCorpsNum );

	MSG_CG_GUILD_CHANGE_CORPS_BRD	brdMsg;
	brdMsg.m_byCorpsNum	= pMsg->m_byCorpsNum;
    _tcsncpy(
        brdMsg.m_szTargetCharName, 
        pMsg->m_szTargetCharName,
        _countof(brdMsg.m_szTargetCharName)
        );
    // NOTE: cg message is not null terminated.
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );

}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_CORPS_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_CORPS_NAK* pMsg = (MSG_GZ_GUILD_CHANGE_CORPS_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_CHANGE_CORPS_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
}

// 길드 훈련캠프 창설
Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_REAR_CAMP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_REAR_CAMP_BRD* pMsg = (MSG_GZ_GUILD_CREATE_REAR_CAMP_BRD*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( !pPlayer )		return;

	//모든 게임서버의 해당 길드 군단의 훈련캠프 상태를 바꾼다.
	g_GameGuildManager.CreateRearCamp( pMsg->m_GuildGuid, pMsg->m_byCorpsNum );

	MSG_CG_GUILD_CREATE_REAR_CAMP_BRD	brdMsg;
	brdMsg.m_byCorpsNum = pMsg->m_byCorpsNum;
    _tcsncpy(
        brdMsg.m_szTargetCharName, 
        pMsg->m_szTargetCharName,
        _countof(brdMsg.m_szTargetCharName)
        );
    // NOTE: cg message is not null terminated.
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CREATE_REAR_CAMP_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK* pMsg = (MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_CREATE_REAR_CAMP_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
}

// 길드 훈련캠프 해산
Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_REAR_CAMP_BRD* pMsg = (MSG_GZ_GUILD_DESTROY_REAR_CAMP_BRD*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( !pPlayer )		return;

	//모든 게임서버의 해당 길드 군단의 훈련캐프 상태를 바꾼다.
	g_GameGuildManager.DestroyRearCamp( pMsg->m_GuildGuid, pMsg->m_byCorpsNum );

	MSG_CG_GUILD_DESTROY_REAR_CAMP_BRD	brdMsg;
	brdMsg.m_byCorpsNum = pMsg->m_byCorpsNum;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK* pMsg = (MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_GUILD_DESTROY_REAR_CAMP_NAK	nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_INVITE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_INVITE_ACK* recv_msg = (MSG_GZ_GUILD_INVITE_ACK*)pRecvMsg;

	Player* request_player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
	if (request_player == NULL)
    {
        return;
    }

	Player* target_player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_TargetUserKey);
	if (target_player == NULL)
	{
		MSG_CG_GUILD_INVITE_NAK	nak_msg;
		nak_msg.m_byErrorCode = RC::RC_GUILD_FAILED;
		request_player->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	//초대 패킷을 보낸다.
	MSG_CG_GUILD_INVITE_CMD	cmd_msg;
    _tcsncpy(
        cmd_msg.m_OperCharName, 
        request_player->GetCharName(), 
        _countof(cmd_msg.m_OperCharName)
        );
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GUILD_RENDER_INFO guild_render_info;
    if (request_player->GetGuildRenderInfo(&guild_render_info, request_player->GetGuildGuid()))
    {
        _tcsncpy(
            cmd_msg.m_OperGuildName, 
            guild_render_info.m_tszGuildName,
            _countof(cmd_msg.m_OperGuildName)
            );
        cmd_msg.m_OperGuildName[_countof(cmd_msg.m_OperGuildName) - 1] = '\0';
    }
    else
    {
        cmd_msg.m_OperCharName[0] = '\0';
    }
#endif
	cmd_msg.m_OperUserKey = request_player->GetUserKey();
	target_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_INVITE_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_INVITE_NAK* pMsg = (MSG_GZ_GUILD_INVITE_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)		return;

	MSG_CG_GUILD_INVITE_NAK	nak;
	nak.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nak, sizeof(nak) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_LOGIN_CHAR_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	// 로그인 성공 했을때 게임서버에서 할일..
	// 해당 유저의 길드가 존재하면 멤버만 추가하고,
	// 존재하지 않으면 길드 생성후 멤버 추가한다.
	MSG_GZ_GUILD_LOGIN_CHAR_BRD* pMsg = (MSG_GZ_GUILD_LOGIN_CHAR_BRD*)pRecvMsg;

	// 로그인한 멤버가 있는 게임서버이면 길드에 멤버정보를 세팅한다.
	CHARGUID TargetGuid = 0;
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if( pPlayer )
	{
		if(g_GameGuildManager.LoginChar( pMsg->m_GuildInfo, pMsg->m_MemberInfo))
		{
            //길드 정보가 로드 되지 않은 상태라면 길드를 생성한 뒤에 관계 정보를 요청
			MSG_GZ_GUILD_RELATION_INFO_SYN msg;
			msg.m_dwKey = pMsg->m_dwKey;
			msg.m_GuildGuid =  pMsg->m_GuildInfo.m_GuildGuid;
			pPlayer->SendToGuildServer(&msg, sizeof(msg));

            //__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
            //(lst1024)(090715)서버구동후 최초 접속시점 길드 정보가 없음. 
            // Enter 시점에는 길드 찾을수 없음, 길드 생성후 보냄.
            pPlayer->FirstGuildMemberEnter();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            //게임 서버에 길드가 생성된 후에 길드 시설 정보 요청
            MSG_GZ_GUILD_FACILITY_SELECT_SYN facility_msg;
            facility_msg.guild_guid = pMsg->m_GuildInfo.m_GuildGuid;
            pPlayer->SendToGuildServer(&facility_msg, sizeof(facility_msg));
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		}
		else
		{
            //이미 길드정보가 존재 하면 길드에 길드 멤버를 ADD한뒤에 ADDINFO를 응답한다..
			MSG_CG_GUILD_ADD_INFO_ACK msg;
			msg.m_AddInfo.m_GuildGuid = pMsg->m_GuildInfo.m_GuildGuid;
			GuildSmallContentEntry::RelationSystem::LoadRelationInfo(&msg.m_AddInfo);
			msg.m_byRelCnt = msg.m_AddInfo.m_byRelationGuildCnt;
			pPlayer->SendPacket(&msg, sizeof(msg));

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            g_GameGuildManager.SendGuildFacilityTotalInfo(pPlayer, pMsg->m_GuildInfo.m_GuildGuid);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		}

		TargetGuid = pPlayer->GetCharGuid();

		MSG_CG_GUILD_LOGIN_CHAR_CMD	cmdMsg;
		cmdMsg.m_GuildInfo = pMsg->m_GuildInfo;
		cmdMsg.m_CorpsTotalInfo = pMsg->m_CorpsTotalInfo;
		cmdMsg.m_MemberInfo = pMsg->m_MemberInfo;
		pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

        //_NA_0_20100208_DOMINATION_AREA_INFO
        GameDominationManager::Instance()->SendAreaInfoToPlayer(pPlayer);
	}

	//현재 게임서버에 해당 길드가 존재하는지 체크.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildInfo.m_GuildGuid );
	if( !pGuild )	return;

	// 해당 유저가 로그인 했다는 것을 현재 길드원들에게 알려준다.
	MSG_CG_GUILD_LOGIN_CHAR_BRD	brdMsg;
	_tcsncpy(
        brdMsg.m_szCharName, 
        pMsg->m_MemberInfo.m_tszCharName, 
        _countof(brdMsg.m_szCharName)
        );
    
	g_GameGuildManager.SendPacket( pMsg->m_GuildInfo.m_GuildGuid, &brdMsg, sizeof(brdMsg), TargetGuid );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_LOGIN_CHAR_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_LOGIN_CHAR_NAK* pMsg = (MSG_GZ_GUILD_LOGIN_CHAR_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(!pPlayer)		return;

	//로그인 실패 했을때 게임서버에서 할일..
	SUNLOG( eCRITICAL_LOG, "[Handler_GZ_GUILD_IMPL(GZ_GUILD_LOGIN_CHAR_NAK)] GuildMember Login Fail, name = %s", pPlayer->GetCharName() );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_LOGOUT_CHAR_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_LOGOUT_CHAR_BRD* pMsg = (MSG_GZ_GUILD_LOGOUT_CHAR_BRD*)pRecvMsg;

	//현재 게임서버에 해당 길드가 존재하는지 체크.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )	return;

	g_GameGuildManager.Logout( pMsg->m_GuildGuid, pMsg->m_CharGuid );

	// 해당 유저가 로그인 했다는 것을 현재 길드원들에게 알려준다.
	MSG_CG_GUILD_LOGOUT_CHAR_BRD	brdMsg;
    _tcsncpy(brdMsg.m_szCharName, pMsg->m_szCharName, _countof(brdMsg.m_szCharName));
    // NOTE: cg message is not null terminated
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_CHAR_LEVELUP_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHAR_LEVELUP_BRD* pMsg = (MSG_GZ_GUILD_CHAR_LEVELUP_BRD*)pRecvMsg;

	//현재 게임서버에 해당 길드가 존재하는지 체크.
	GameGuild* pGuild = g_GameGuildManager.FindGuild( pMsg->m_GuildGuid );
	if( !pGuild )	return;

	MSG_CG_GUILD_CHAR_LEVELUP_BRD	brdMsg;
    _tcsncpy(brdMsg.m_szCharName, pMsg->m_szCharName, _countof(brdMsg.m_szCharName));
    // NOTE: cg message is not null terminated.
	brdMsg.m_Level = pMsg->m_Level;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );
}


//  [7/24/2009 lst1024] Agent의 User 정보 갱신시 주의 사항
// 유저가 케릭터 선택창에 있을경우 
// Game  Server의 Player 객체는 생성전 혹은 삭제후이며, PlayerManager 에서 찾을수 없다.
// Agent Server의 User 객체는 생성된 상태이며, UserGUID로 찾을수 있다.
// -- CharName이나 CharGUID로 찾을수 없다.--

// 수정 : DZ_GUILD_KICKPLAYER_DBR 에서 DB결과에 강퇴 UserGUID 필요.


// 길드멤버 강퇴 결과
Handler_GZ_GUILD_IMPL(GZ_GUILD_KICKPLAYER_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_KICKPLAYER_BRD *pMsg = (MSG_GZ_GUILD_KICKPLAYER_BRD *)pRecvMsg;

	// 일단 제명
	g_GameGuildManager.LeaveGuild( pMsg->m_GuildGuid, pMsg->m_CharGuid );

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( pPlayer )
	{
		//Agent의 User 정보에 길드관련 정보를 초기화 시킨다.
		MSG_AG_GUILD_KICKPLAYER_ACK msgACK;
		msgACK.m_dwKey = pMsg->m_dwKey;
        _tcsncpy(
            msgACK.m_ptszCharName, 
            pMsg->m_ptszCharName, 
            _countof(msgACK.m_ptszCharName)
            );
        msgACK.m_ptszCharName[_countof(msgACK.m_ptszCharName) - 1] = '\0';
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
	}

	Player* pKickedPlayer = PlayerManager::Instance()->FindPlayerByName( pMsg->m_ptszCharName );
	if( pKickedPlayer )
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        g_GameGuildManager.ApplyGuildFacilityPassive(pKickedPlayer, false);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
        GameGuild* player_guild = pKickedPlayer->GetGuild();
        if (player_guild != NULL) {
            player_guild->ApplyGuildRewardOption(pKickedPlayer, FALSE);
        }
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pKickedPlayer,false);  // 길드 탈퇴 보상옵션 제거
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        // 기부한 길드코인을 삭제
        PointWalletManager* const pointwallet_manager = pKickedPlayer->GetPointWalletManager();
        const DWORD code = CURRENCY_INFO_DONATION_GUILDCOIN;
        INT donation_coin = pointwallet_manager->GetPointWallet(code);
        if (donation_coin > 0) {
            pointwallet_manager->ChangeWalletPoint(code, -donation_coin);
        }
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

        GameField*  pField = pKickedPlayer->GetField();
        if (NULL != pField)
        {
            const FIELDCODE fCode = pField->GetFieldCode();
            GameDominationManager::Instance()->ApplyRewardField(pKickedPlayer,fCode,false);
        }

		pKickedPlayer->GetCharInfo()->m_GuildGuid		= 0;
		pKickedPlayer->GetCharInfo()->m_eGuildDuty	= (eGUILD_DUTY)0;
		pKickedPlayer->GetCharInfo()->m_GuildPenaltyTime	= pMsg->m_MemberPenaltyTime;
		pKickedPlayer->GetCharInfo()->m_ePenaltyType		= eGUILD_KICK;

		MSG_CG_GUILD_KICKPLAYER_CMD	cmdMsg;
		pKickedPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		//Render 정보 때문에 강퇴당한놈 주위에 
		MSG_CG_GUILD_LEAVE_BRD brdMsg;
		brdMsg.m_LeaverObjectKey = pKickedPlayer->GetObjectKey();
		pKickedPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );
	}

	// 현 게임서버의 모든 길드원들 에게 알린다.
	MSG_CG_GUILD_KICKPLAYER_BRD msgBRD;
    _tcsncpy(
        msgBRD.m_szCharName, 
        pMsg->m_ptszCharName, 
        _countof(msgBRD.m_szCharName)
        );
    msgBRD.m_szCharName[_countof(msgBRD.m_szCharName) - 1] = '\0';
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &msgBRD, sizeof(msgBRD), pMsg->m_CharGuid );
}

Handler_GZ_GUILD_IMPL( GZ_GUILD_TOTALINFO_REQ_ACK )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	//단순희 전달용이다.
	MSG_GZ_GUILD_TOTALINFO_REQ_ACK* pMsg = (MSG_GZ_GUILD_TOTALINFO_REQ_ACK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )		return;

	MSG_CG_GUILD_TOTALINFO_REQ_ACK ackMsg;
	ackMsg.m_GuildInfo = pMsg->m_GuildInfo;
	ackMsg.m_CorpsTotalInfo = pMsg->m_CropsTotalInfo;
	ackMsg.m_MemberTotalInfo = pMsg->m_MemberTotalInfo;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    memcpy(&ackMsg.guild_info_extra_, &pMsg->guild_info_extra_, sizeof(pMsg->guild_info_extra_));
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ackMsg.m_GuildJoinRequestTotalInfo = pMsg->m_GuildJoinRequestTotalInfo;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	pPlayer->SendPacket( &ackMsg, ackMsg.GetSize() );
}

Handler_GZ_GUILD_IMPL( GZ_GUILD_TOTALINFO_REQ_NAK )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	//단순희 전달용이다.
	MSG_GZ_GUILD_TOTALINFO_REQ_NAK* pMsg = (MSG_GZ_GUILD_TOTALINFO_REQ_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )		return;

	MSG_CG_GUILD_TOTALINFO_REQ_NAK nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GM_GUILD_POINT_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_GM_GUILD_POINT_BRD* pMsg = (MSG_GZ_GUILD_GM_GUILD_POINT_BRD*)pRecvMsg;

	MSG_CG_GUILD_GM_GUILD_POINT_BRD brdMsg;
	brdMsg.m_GuildGuid	= pMsg->m_GuildGuid;
	brdMsg.m_UP			= pMsg->m_UP;
	brdMsg.m_GP			= pMsg->m_GP;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GM_GUILD_POINT_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_GM_GUILD_POINT_NAK* pMsg = (MSG_GZ_GUILD_GM_GUILD_POINT_NAK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer )		return;

	MSG_CG_GUILD_GM_GUILD_POINT_NAK nakMsg;
	nakMsg.m_byErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_CHANGE_CHAOSTATE_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_CHANGE_CHAOSTATE_BRD* pMsg = (MSG_GZ_GUILD_CHANGE_CHAOSTATE_BRD*)pRecvMsg;

	//그냥 현재 상태를 전달한다.

	MSG_CG_GUILD_CHANGE_CHAOSTATE_BRD brdMsg;
    _tcsncpy(brdMsg.m_szCharName, pMsg->m_szCharName, _countof(brdMsg.m_szCharName));
    // NOTE: cg message is not null terminated.
	brdMsg.m_byChaoState = pMsg->m_byChaoState;
	g_GameGuildManager.SendPacket( pMsg->m_GuildGuid, &brdMsg, sizeof(brdMsg) );	
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_ENTER_SERVER_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_ENTER_SERVER_ACK* pMsg = (MSG_GZ_GUILD_ENTER_SERVER_ACK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION 
    if( pPlayer )
    {
        if (g_GameGuildManager.LoginChar(pMsg->m_GameGuildInfo, pMsg->m_BaseMemberInfo))
        {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            MSG_GZ_GUILD_FACILITY_SELECT_SYN facility_msg;
            facility_msg.guild_guid = pMsg->m_GameGuildInfo.m_GuildGuid;
            g_pGameServer->SendToServer(GAME_DBPROXY, &facility_msg, sizeof(facility_msg));
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
    }
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION 
    if( pPlayer )
        g_GameGuildManager.EnterServer( pMsg->m_GameGuildInfo, pMsg->m_BaseMemberInfo );
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION 
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_BOTH_RELATION_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_BOTH_RELATION_ACK, pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pMsg->m_dwKey);
	
	GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(pMsg->m_RequestGuildGuid);
	if(!pRequestGuild) return;

	GameGuild* pResponseGuild = g_GameGuildManager.FindGuild(pMsg->m_ResponseGuildGuid);
	if(pResponseGuild)
	{
		GameGuildMember* pMember = 
			pResponseGuild->FindMember(pMsg->m_ResponsePlayerIndex);

		if(pMember)
		{
			Player* pRelationRightPlayer = 
				PlayerManager::Instance()->FindPlayerByName(pMember->GetCharName());
			
			if(pRelationRightPlayer)
			{
				MSG_CG_GUILD_RELATION_REQUEST_CMD msg;
				msg.m_eType = pMsg->m_eRelationType;
				msg.m_RequestUserKey = pPlayer->GetUserKey();
				util::GUILDNAMECOPY(msg.m_RequestGuildName, pRequestGuild->GetGuildName());
				pRelationRightPlayer->SendPacket(&msg, sizeof(msg));
				return;
			}
		}
	}
	
	MSG_CG_GUILD_RELATION_NAK msgNAK;
	msgNAK.m_byErrorCode = RC::RC_GUILD_CURRENT_NONE_MASTER;
	pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
	return;
	
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_NAK, pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pRequestPlayer, pMsg->m_dwKey);

	MSG_CG_GUILD_RELATION_NAK msg;
	msg.m_byErrorCode = pMsg->m_byErrorCode;
	pRequestPlayer->SendPacket(&msg, sizeof(msg));

	PLAYER_PTR_USERKEY(Player, pRelationMaster, pMsg->m_OtherGuildMasterUid);
	pRelationMaster->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_BRD, pMsg, pRecvMsg);
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
    if (pMsg->m_eRelationType == eGUILD_RELATION_CANCEL_BOTH_HOSTILITY)
    {
        GuildSmallContentEntry::RelationSystem::RemoveRelation(
            pMsg->m_GuildGuid,
            pMsg->m_RelationGuildGuid,
            pMsg->m_eRelationType
            );
    }
    else
    {
        GuildSmallContentEntry::RelationSystem::AddRelation(
            pMsg->m_GuildGuid, 
            pMsg->m_RelationGuildGuid, 
            pMsg->m_eRelationType, 
            pMsg->m_RelationApplyTime,
            pMsg->m_RelationChangeTime,
            pMsg->m_tszRelationGuildName, 
            pMsg->m_tszRequestGuildName
            );
    }
#else
    GuildSmallContentEntry::RelationSystem::AddRelation(
        pMsg->m_GuildGuid, pMsg->m_RelationGuildGuid, 
        pMsg->m_eRelationType, pMsg->m_tszRelationGuildName, pMsg->m_tszRequestGuildName);
#endif
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_BOTH_AGAINST_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_BOTH_AGAINST_BRD, pMsg, pRecvMsg);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GuildSmallContentEntry::RelationSystem::AddRelation(
        pMsg->m_GuildGuid, 
        pMsg->m_RelGuildGuid, 
        eGUILD_RELATION_BOTH_HOSTILITY, 
        pMsg->m_RelationApplyTime,
        pMsg->m_RelationChangeTime,
        "");
#else
    GuildSmallContentEntry::RelationSystem::AddRelation(
        pMsg->m_GuildGuid, pMsg->m_RelGuildGuid, eGUILD_RELATION_BOTH_HOSTILITY, "");
#endif
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_DELETE_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_DELETE_BRD, pMsg, pRecvMsg);
	GuildSmallContentEntry::RelationSystem::RemoveRelation(
        pMsg->m_GuildGuid, pMsg->m_RelGuildGuid, pMsg->m_eType);
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_BOTH_AGAINST_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK, pMsg, pRecvMsg);
	Player *pRequestPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(pRequestPlayer)
	{
		MSG_CG_GUILD_RELATION_NAK msg;
		msg.m_byErrorCode = pMsg->m_byErrorCode;
		pRequestPlayer->SendPacket(&msg, sizeof(msg));
	}
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_DELETE_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_DELETE_NAK , pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pRequestPlayer, pMsg->m_dwKey );

	MSG_CG_GUILD_RELATION_DELETE_NAK msg;
	msg.m_byErrorCode = pMsg->m_byErrorCode;
	pRequestPlayer->SendPacket(&msg, sizeof(msg));
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_RELATION_INFO_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_INFO_BRD, pMsg, pRecvMsg);	

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	GuildSmallContentEntry::RelationSystem::SaveRelationInfo(&pMsg->m_AddInfo);
	if(pPlayer)
	{		
		MSG_CG_GUILD_ADD_INFO_ACK msg;
		msg.m_AddInfo.m_GuildGuid = pMsg->m_AddInfo.m_GuildGuid;
		GuildSmallContentEntry::RelationSystem::LoadRelationInfo(&msg.m_AddInfo);
		msg.m_byRelCnt = msg.m_AddInfo.m_byRelationGuildCnt;
		pPlayer->SendPacket(&msg, msg.GetSize());
	}
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_WAIT_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_DESTROY_WAIT_BRD, receive_msg, pRecvMsg);	
	
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_WAIT_BRD)|MSG_GZ_GUILD_DESTROY_WAIT_BRD 받음|Key:%u, GuildGuid:%d|"), 
           receive_msg->m_dwKey, 
           receive_msg->m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
	if (player != NULL)
	{
		MSG_CG_GUILD_DESTROY_WAIT_ACK msg;
		msg.m_GuildGuid = receive_msg->m_GuildGuid;

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산신청|Handler_GZ_GUILD_IMPL(GZ_GUILD_DESTROY_WAIT_BRD)|MSG_CG_GUILD_DESTROY_WAIT_ACK 보냄|GuildGuid:%d|"), msg.m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

		player->SendPacket(&msg, sizeof(msg));
	}

	GuildSmallContentEntry::RelationSystem::RemainRelationDestroy(receive_msg->m_GuildGuid);
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RECOVERY_GUILD_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RECOVERY_GUILD_BRD, pMsg, pRecvMsg);	
	PLAYER_PTR_USERKEY(Player, pPlayer, pMsg->m_dwKey );

	MSG_CG_GUILD_RECOVERY_GUILD_ACK msg;
	msg.m_GuildGuid = pMsg->m_GuildGuid;
	pPlayer->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RECOVERY_GUILD_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RECOVERY_GUILD_NAK, pMsg, pRecvMsg);	
	PLAYER_PTR_USERKEY(Player, pPlayer, pMsg->m_dwKey );
	
	MSG_CG_GUILD_RECOVERY_GUILD_NAK msg;
	msg.m_byErrorCode =  RC::RC_GUILD_RECOVERY_FAILED;
	pPlayer->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GUILDMARK_REGISTER_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_GUILDMARK_REGISTER_BRD , pMsg, pRecvMsg);
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);

	GuildSmallContentEntry::GuildMark::GuildMarkInfo info;
	info.guild_Index = pMsg->m_GuildIndex;
	info.pattern_index = pMsg->m_PatternIndex;
	info.background_index = pMsg->m_BackGroundIndex;
	info.posType =pMsg->m_SlotPos;
	info.slot_Index = pMsg->m_SlotIdx;
	GuildSmallContentEntry::GuildMark::RegisterMark(pPlayer, info);

    GameDominationManager::Instance()->ChangeGuildMark( info.guild_Index, info.pattern_index, info.background_index );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GUILDMARK_DELETE_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_GUILDMARK_DELETE_BRD, pMsg, pRecvMsg);
	
	GuildSmallContentEntry::GuildMark::GuildMarkInfo info;
	info.guild_Index = pMsg->m_GuildIndex;
	info.pattern_index = pMsg->m_PatternIndex;
	info.background_index = pMsg->m_BackGroundIndex;
	GuildSmallContentEntry::GuildMark::DeleteMark(info);

    GameDominationManager::Instance()->ChangeGuildMark( info.guild_Index, info.pattern_index, info.background_index );
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GUILDMARK_REGISTER_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_GUILDMARK_REGISTER_NAK, pMsg, pRecvMsg);	
	PLAYER_PTR_USERKEY(Player, pPlayer, pMsg->m_dwKey );

	MSG_CG_GUILD_GUILDMARK_REGISTER_NAK msg;
	msg.m_bErrorCode = pMsg->m_Result;
	pPlayer->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_GUILDMARK_DELETE_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_GUILDMARK_DELETE_NAK, pMsg, pRecvMsg);	
	PLAYER_PTR_USERKEY(Player, pPlayer, pMsg->m_dwKey );

	MSG_CG_GUILD_GUILDMARK_DELETE_NAK msg;
	msg.m_bErrorCode = pMsg->m_Result;
	pPlayer->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_WAREHOUSE_OPEN_BRD)
{	
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_WAREHOUSE_OPEN_BRD, pMsg, pRecvMsg);
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if(pPlayer && pPlayer->GetField())
	{	
		CGuildWarehouseSlotContainer* pContainer = g_GameGuildManager.GetGuildWarehouse(pMsg->m_GuildGuid);
		if(pContainer && pPlayer->GetGuildWarehouseContainer()) 
		{
			pContainer->OpenInit(pPlayer, pMsg->m_SlotTabNum, pMsg->m_WarehouseRight);
			pContainer->SerializeItemInfo(pMsg->m_ItemInfo, SERIALIZE_STORE);
			pContainer->SerializeMoney(pMsg->m_WarehouseMoney);
			pContainer->OpenWarehouse();

			MSG_CG_GUILD_WAREHOUSE_OPEN_ACK  msg;
			msg.m_SlotTabNum = pContainer->GetSlotTabNum();
			msg.m_WarehouseMoney	= pContainer->GetCurMoney();	
			msg.m_Size = sizeof(RENDER_ITEMSTREAMEX)*(MAX_GUILD_WAREHOUSE_SLOT_NUM);

			BitStream stream(msg.m_BitStreamBuffer, msg.m_Size, TRUE);
			pContainer->Serialize(stream, eSLOT_SERIALIZE_CLIENT_LOAD);
			msg.m_Size = stream.GetNumberOfBytesUsed();
			pPlayer->SendPacket( &msg, msg.GetSize() );
            SUNLOG(eDEV_LOG, "WAREHOUSE_OPEN SUCCESS char[%u] guild[%u]", 
                   pMsg->m_OperCharGuid, pMsg->m_GuildGuid);
		}
		else
		{
			pContainer->ClearAll();
			MSG_CG_GUILD_WAREHOUSE_OPEN_NAK msg;
			msg.m_bErrorCode = RC::RC_GUILD_WAREHOUSE_GET_CANNOT_DB;
			pPlayer->SendPacket(&msg, sizeof(msg));
            SUNLOG(eCRITICAL_LOG, "WAREHOUSE_OPEN ERROR char[%u] guild[%u]", 
                   pMsg->m_OperCharGuid, pMsg->m_GuildGuid);
		}
	}
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_WAREHOUSE_OPEN_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_WAREHOUSE_OPEN_NAK , pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);
	
	CGuildWarehouseSlotContainer* pContainer = g_GameGuildManager.GetGuildWarehouse(pMsg->m_GuildGuid);
	if(pContainer) 
	{
		//pContainer->ClearAll();
	}
	else
	{
		SUNLOG(eCRITICAL_LOG, "GZ_GUILD_WAREHOUSE_OPEN_NAK GuildWarehouse ERROR[%u]", pMsg->m_GuildGuid);
	}
	
	MSG_CG_GUILD_WAREHOUSE_OPEN_NAK msg;
	msg.m_bErrorCode = pMsg->m_bErrorCode;
	pPlayer->SendPacket(&msg, sizeof(msg));
}


Handler_GZ_GUILD_IMPL(GZ_GUILD_NAME_CHANGE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_NAME_CHANGE_ACK* pMsg = (MSG_GZ_GUILD_NAME_CHANGE_ACK*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	if (!pPlayer) 
	{
		SUNLOG(eCRITICAL_LOG, "[GZ_GUILD_NAME_CHANGE_ACK] 아이템 삭제 실패");
		return;
	}

	// 아이템의 변경을 알림
    SCSlotContainer* inventory = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);

    //_NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG = {
    const TCHAR* add_info = NULL;

    SCItemSlot& item_slot = (SCItemSlot &)inventory->GetSlot(pMsg->m_TargetPos);

    if (item_slot.GetItemInfo()) 
    {
        TCHAR buf[MAX_ADDINFO_SIZE];
        _sntprintf(buf, MAX_ADDINFO_SIZE, "BGuildName:%s, AGuildName:%s, GuildGuid:%d",
                                           pMsg->m_tszCurrentGuildName, pMsg->m_tszchangeGuildName,
                                           pPlayer->GetGuildGuid());
        buf[MAX_ADDINFO_SIZE - 1] = '\0';
        add_info = buf;
    }

    GAMELOG->LogItem(ITEM_USE, pPlayer, &item_slot, 0, 1, NULL, NULL, add_info);
    //}
	inventory->DeleteSlot(pMsg->m_TargetPos, NULL);

	MSG_CG_ITEM_DELETE_CMD msg;
	msg.m_byReason = RC::RC_ITEM_DELETE_SUCCESS;
	msg.m_atIndex = SI_INVENTORY;
	msg.m_atPos = pMsg->m_TargetPos;
	pPlayer->SendPacket(&msg, sizeof(msg));
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_NAME_CHANGE_BRD)
{	
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GZ_GUILD_NAME_CHANGE_BRD* pMsg = (MSG_GZ_GUILD_NAME_CHANGE_BRD*)pRecvMsg;

	// 현재 게임 서버의 길드명을 바꾼다.
	g_GameGuildManager.ChangeGuildName( pMsg->m_GuildGuid, pMsg->m_tszGuildName );

    // 지역점령전 정보를 갱신한다.
    GameDominationManager::Instance()->ChangeGuildName( pMsg->m_GuildGuid, pMsg->m_tszGuildName );
	//.//

	GameGuild* pGameGuild = g_GameGuildManager.FindGuild(pMsg->m_GuildGuid);
	if( !pGameGuild )
		return;

	MSG_CG_GUILD_NAME_CHANGE_BRD MemberMsgBRD;
	MSG_CG_GUILD_NAME_CHANGE_SECTOR_BRD AroundMsgBRD;

	util::GUILDNAMECOPY(MemberMsgBRD.m_tszGuildName, pMsg->m_tszGuildName);
	util::GUILDNAMECOPY(AroundMsgBRD.m_tszGuildName, pMsg->m_tszGuildName);

    pGameGuild->SendPacketMemberAndAroundAll(MemberMsgBRD, AroundMsgBRD);
	
	//.//

	STLX_VECTOR<GUILDGUID> vGuildGuid;
	if( 0 == pGameGuild->GetRelationGuid(vGuildGuid) )
		return;

	MSG_CG_GUILD_NAME_CHANGE_RELATION_BRD RelationMsgBRD;
	RelationMsgBRD.m_GuildGuid = pMsg->m_GuildGuid;
	util::GUILDNAMECOPY(RelationMsgBRD.m_tszGuildName, pMsg->m_tszGuildName);

	STLX_VECTOR<GUILDGUID>::iterator it = vGuildGuid.begin();
	for( ; it != vGuildGuid.end() ; ++it )
	{
		GameGuild* pGameGuild = g_GameGuildManager.FindGuild( *it );
		if( pGameGuild )
		{
			RelationMsgBRD.m_RelationType = pGameGuild->GetRelationType(pMsg->m_GuildGuid);	// 클라에서 정보 필요.
			pGameGuild->SendPacketMember( RelationMsgBRD );
		}
	}
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_NAME_CHANGE_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_NAME_CHANGE_NAK , pMsg, pRecvMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	MSG_CG_GUILD_NAME_CHANGE_NAK msg;
	msg.m_bErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket(&msg, sizeof(msg));

	SCSlotContainer* pInventory = pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
	pInventory->SetLock( pMsg->m_TargetPos, FALSE );
}

//지역 점령전 기본 정보 저장 요청
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_INFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_INFO_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_INFO_ACK*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    const DOMINATION_INFO& domination_info = recv_msg->m_DominationInfo;
    GameDominationManager::Instance()->UpdateDominationInfo(domination_info, DOMINATION_INIT);
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
//지역 점령전 대륙 영주 정보 저장 요청
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_CONTINENT_LORD_INFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_CONTINENT_LORD_INFO_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_CONTINENT_LORD_INFO_ACK*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    GameDominationManager::Instance()->SetDominationContinentLordInfo(recv_msg->m_lordInfo);
}
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

//지역 점령전 경매 정보 저장 요청
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_AUCTION_INFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_GZ_DOMINATION_AUCTION_INFO_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_AUCTION_INFO_ACK*>(pRecvMsg);

    const int data_count = recv_msg->m_DataCount;
    const MAPCODE map_code = recv_msg->m_MapCode;

    if (data_count < 0 || data_count > MAX_DOMINATION_AUCTION)
    {
        assert(!"Incorrect auction data count");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Incorrect auction data count : %d", data_count);
        return;
    }

    //gamelulu_debug
    SUNLOG (eFULL_LOG, __FUNCTION__":Auction info ack : %d", recv_msg->m_MapCode);
    GameDominationManager::Instance()->SetAuctionData(map_code, recv_msg->m_ActionInfo, data_count);

}

//_KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_UPDATE_INFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_UPDATE_INFO_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_UPDATE_INFO_ACK*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    if (recv_msg->m_ResultCode == RC::DOMINATION_RESULT_FAIL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Update fail[map:%d]",
               recv_msg->m_DominationInfo.m_FieldCode);

        return;
    }

    const DOMINATION_INFO& domination_info = recv_msg->m_DominationInfo;
    domination_manager->UpdateDominationInfo(domination_info, recv_msg->m_eSetType);
    SUNLOG(eFULL_LOG, "["__FUNCTION__"]Success update[map:%d]",domination_info.m_FieldCode);

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    if (FIELD_SERVER == g_pGameServer->GetServerType()) 
    {
        if(recv_msg->m_eSetType == DOMINATION_SUCCESS_DEFENCE)
        {
            domination_manager->UpdateGuildInfoByDomination(
                recv_msg->m_DominationInfo.m_inGuildInfo.m_GuildGuid, DOMINATION_DEFENSE_COUNT);
        }
        else if(recv_msg->m_eSetType == DOMINATION_DISGUILD_SET)
        {
            domination_manager->UpdateGuildInfoByDomination(
                recv_msg->m_DominationInfo.m_disGuildInfo.m_GuildGuid, DOMINATION_JOIN_COUNT);
        }
        else if(recv_msg->m_eSetType == DOMINATION_SUCCESS_OFFENCE)
        {
            domination_manager->UpdateGuildInfoByDomination(
                recv_msg->m_DominationInfo.m_disGuildInfo.m_GuildGuid, DOMINATION_MAP_COUNT);

            domination_manager->UpdateGuildInfoByDomination(
                recv_msg->m_DominationInfo.m_inGuildInfo.m_GuildGuid, DOMINATION_MAP_COUNT);
        }
    }
}

Handler_GZ_GUILD_IMPL(GZ_DOMINATION_ACCEP_HEIM_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_ACCEP_HEIM_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ACCEP_HEIM_ACK*>(pRecvMsg);

    Player *player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    const DominationAuctionInfo& curActionInfo = recv_msg->m_DominationActionInfo;

    if( recv_msg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS )
    {
        MSG_CG_DOMINATION_ACCEP_HEIM_ACK nak_msg;
        //경매 아이템을 지운 시점, DB저장 실패시 삭제한 아이템을 복구
        nak_msg.m_ResultCode = recv_msg->m_ResultCode;
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":DB Result Error : %d", recv_msg->m_ResultCode);
        player->SendPacket(&nak_msg, sizeof(nak_msg));

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        MSG_CG_ITEM_OBTAIN_ITEM_CMD send_msg;

        MAPCODE map_code = curActionInfo.m_ActionInfo.m_FieldCode;
        SLOTCODE item_code = DominationInfoParser::Instance()->GetBattingItemCodeByMapCode(map_code);
        const int amount_cost = recv_msg->number_of_betting_item;
        RC::eITEM_RESULT result_code = player->GetItemManager()->Obtain(item_code, 
                                                                        amount_cost, 
                                                                        &send_msg.m_tagInventory);
        if (result_code == RC::RC_ITEM_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Reward item[%d]", amount_cost);
            player->SendPacket(&send_msg, send_msg.GetSize());
        }
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    }
    else if( recv_msg->m_ResultCode == RC::DOMINATION_RESULT_SUCCESS )
    {
        MAPCODE map_code = curActionInfo.m_ActionInfo.m_FieldCode;
        bool result_flag = domination_manager->UpdateAuctionData(player, map_code,curActionInfo);
        if (result_flag == false)
        {
            return;
        }

        MSG_CG_DOMINATION_ACCEP_HEIM_ACK send_msg;
        send_msg.m_ResultCode = recv_msg->m_ResultCode;
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        send_msg.betting_item_count_ = recv_msg->number_of_betting_item;
        send_msg.m_TokenPos = recv_msg->m_TokenPos;
    #else
        send_msg.m_Cost = player->GetMoney();
    #endif
        player->SendPacket(&send_msg, sizeof(send_msg));
    }
}

Handler_GZ_GUILD_IMPL(GZ_DOMINATION_INIT_ACTIONINFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_INIT_ACTIONINFO_ACK* recv_msg = static_cast<MSG_GZ_DOMINATION_INIT_ACTIONINFO_ACK*>(pRecvMsg);

    if( recv_msg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS )
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Error Code [%d]\n", recv_msg->m_ResultCode);
    }
    else if( recv_msg->m_ResultCode == RC::DOMINATION_RESULT_SUCCESS )
    {
        GameDominationManager::Instance()->ClearAuctionInfo(recv_msg->m_MapCode);
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Success ActionInfo Init" );
    }
}

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_BATTING_HEIM_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_BATTING_HEIM_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_BATTING_HEIM_ACK*>(pRecvMsg);

    Player *player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not Found User[%d]", recv_msg->m_dwKey);
        return;
    }

    const GameField* const current_field_Data = player->GetField();
    if (current_field_Data == NULL)
    {
        assert(!"Not Found Player Field Data");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Found Player Field Data[%d]\n", 
            recv_msg->m_dwKey);
        return;
    }

    if(recv_msg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS)
    {
        MSG_CG_DOMINATION_BATTING_HEIM_ACK nak_msg;

        nak_msg.m_ResultCode = recv_msg->m_ResultCode;
        nak_msg.m_RemainCost = 0;
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
        if (recv_msg->m_ResultCode == RC::DOMINATION_RESULT_PENALTY_TIME)
            nak_msg.penalty_time = recv_msg->penalty_time;
#endif //_NA_000000_20130429_DOMINATION_PENALTY

        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }
    
    const GameDominationManager* const domination_manager = GameDominationManager::Instance();
    
    const MAPCODE map_code = recv_msg->m_mapCode;
    if (domination_manager->FindAreaInfo(map_code) == NULL)
    {
        MSG_CG_DOMINATION_BATTING_HEIM_ACK nak_msg;
        nak_msg.m_ResultCode = RC::DOMINATION_RESULT_FAIL;
        nak_msg.m_RemainCost = 0;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    const GUILDGUID guild_guid = player->GetGuildGuid();
    const MONEY tender_cost = 
        domination_manager->GetTenderCost(map_code, guild_guid);

    const RC::eDOMINATION_RESULT result_code = (tender_cost) ? RC::DOMINATION_RESULT_OVERAP_APPLY:
                                                         RC::DOMINATION_RESULT_SUCCESS;
    MSG_CG_DOMINATION_BATTING_HEIM_ACK send_msg;

    send_msg.m_ResultCode = result_code;
    send_msg.m_RemainCost = player->GetMoney();

    player->SendPacket(&send_msg, sizeof(send_msg));
}
#else//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_BATTING_HEIM_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_BATTING_HEIM_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_BATTING_HEIM_ACK*>(pRecvMsg);

    Player *player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not Found User[%d]", recv_msg->m_dwKey);
        return;
    }

    if(recv_msg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS)
    {
        MSG_CG_DOMINATION_BATTING_HEIM_ACK nak_msg;

        nak_msg.m_ResultCode = recv_msg->m_ResultCode;
        nak_msg.m_RemainCost = 0;

        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MSG_CG_DOMINATION_BATTING_HEIM_ACK send_msg;

    send_msg.m_ResultCode = recv_msg->m_ResultCode;
    send_msg.m_RemainCost = player->GetMoney();

    player->SendPacket(&send_msg, sizeof(send_msg));
}
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY


Handler_GZ_GUILD_IMPL(GZ_DOMINATION_RETURNHIME_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_RETURNHIME_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_RETURNHIME_ACK*>(pRecvMsg);

    GameDominationManager::Instance()->SendInitActionInfo( recv_msg->m_MapCode  );
}

//_NA_20100604_BETTER_DOMINATION_MANAGER
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_UPDATE_GUILDINFO_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_UPDATE_GUILDINFO_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_UPDATE_GUILDINFO_ACK*>(pRecvMsg);
    
    if (recv_msg->db_result_ == MSG_GZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_FAIL)
    {
        assert(!"Fail~! Increase Domination Guild Info Count");
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Fail~! Increase Domination Guild Info Count");

        return;
    }

    GameGuild* game_guild = g_GameGuildManager.FindGuild(recv_msg->guild_guid_);
    if (game_guild == NULL) return;

    //--점령 지역수
    WORD number_of_domination_map = 
        GameDominationManager::Instance()->GetAreaCount(recv_msg->guild_guid_);

    MSG_CG_DOMINATION_UPDATE_GUILDINFO_BRD brd_msg;
    {
        brd_msg.domination_map_count_ = number_of_domination_map;
        brd_msg.domination_join_count_ = recv_msg->domination_join_count_;
        brd_msg.domination_defense_count_ = recv_msg->domination_defense_count_;
        brd_msg.domination_hunting_count_ = recv_msg->domination_hunting_count_;
    }
    game_guild->SendPacketMember(brd_msg);
}

// _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
// GM 명령에 의한 길드 창고 강제 닫기 공지
// 개별 게임 서버에서 이 길드 창고가 사용되고 있다면 사용자가 닫기를 실행한 것처럼 행동한다.
Handler_GZ_GUILD_IMPL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD* msg = \
        static_cast<MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD*>(pRecvMsg);

    GameGuild* guild = g_GameGuildManager.FindGuild(msg->guild_guid);
    if (guild == NULL) {
        return;
    }
    CGuildWarehouseSlotContainer* guild_warehouse = guild->GetGuildWareHouse();
    if (guild_warehouse == NULL) {
        return;
    }

    Player* player = guild_warehouse->GetUsedPlayer();
    if (player == NULL && guild_warehouse->IsOpened() == FALSE) {
        // 사용중인 플레이어도 없고 열려 있지도 않다면 클리어할 것이 없다.
        return;
    }

    // 닫기 처리는 일단 클라이언트로부터의 CG_GUILD_WAREHOUSE_CLOSE_SYN 패킷 처리에 따르지만,
    // 정리가 필요할 것 같다.
    if (guild_warehouse->IsChanged())
    {
        if (player != NULL) {
            player->SerializeInfoToDBProxy(UPDATE_FACTOR_NORMAL_CLOSE);
        }
    }
    else
    {
        if (player != NULL)
        {
            MSG_CG_GUILD_WAREHOUSE_CLOSE_ACK ack;
            player->SendPacket(&ack, sizeof(ack));

            MSG_DG_GUILD_WAREHOUSE_CLOSE_SYN syn;
            syn.m_dwKey = player->GetUserKey();
            syn.m_GuildGuid = guild->GetGuildGuid();
            player->SendToGameDBPServer(&syn, sizeof(syn));
        }

        guild_warehouse->CloseWarehouse();
    }
}

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    Player *player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    DominationArea* domination_area = 
        domination_manager->GetAreaInfo(recv_msg->domination_info_.m_FieldCode);
    if (domination_area == NULL)
    {
        return;
    }

    domination_area->SetTransactionToDeffenseWall(false);

    if (recv_msg->db_result_ == false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Failed update to DB, start reward item");
        
        if (domination_manager->RewardItem(player, 1, recv_msg->item_code ) == RC::RC_ITEM_FAILED)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Failed to reward item");
        }

        MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK nak_msg;
        nak_msg.result_code_ = RC::DOMINATION_RESULT_DBERROR;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    domination_manager->UpdateDominationInfo(recv_msg->domination_info_, DOMINATION_INFO_SET);

    MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK ack_msg;
    ack_msg.result_code_ = RC::DOMINATION_RESULT_SUCCESS;
    player->SendPacket(&ack_msg, sizeof(ack_msg));

}

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
Handler_GZ_GUILD_IMPL(GZ_DOMINATION_MEMBER_JOIN_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_MEMBER_JOIN_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_JOIN_ACK*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    Player* player = 
        PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->member_info_.member_guid_);
    if (player == NULL)
    {
        return;
    }

    if (recv_msg->db_result_ == false)
    {
        MSG_CG_DOMINATION_MEMBER_JOIN_ACK nak_msg;
        nak_msg.result_code_ = RC::DOMINATION_RESULT_DBERROR;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    DominationArea* domination_area = domination_manager->GetAreaInfo(recv_msg->map_code_);
    if (domination_area == NULL)
    {
        return;
    }

    domination_area->GetMember()->MemberInsert(&recv_msg->member_info_);

    MSG_CG_DOMINATION_MEMBER_JOIN_ACK ack_msg;
    ack_msg.result_code_ = RC::DOMINATION_RESULT_SUCCESS;
    player->SendPacket(&ack_msg, sizeof(ack_msg));

    domination_manager->SendMemberList(player, recv_msg->map_code_);
}

Handler_GZ_GUILD_IMPL(GZ_DOMINATION_MEMBER_LEAVE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_MEMBER_LEAVE_ACK* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_LEAVE_ACK*>(pRecvMsg);

    // 디비 처리가 실패했으면 자신이 요청했을때만 실패 응답을 보낸다.
    if (recv_msg->db_result_ == false)
    {
        if (recv_msg->member_guid_ == recv_msg->requester_guid)
        {
            Player* const leave_member = 
                PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->member_guid_);
            if (leave_member == NULL)
            {
                return;
            }
            MSG_CG_DOMINATION_MEMBER_LEAVE_ACK nak_msg;
            nak_msg.result_code_ = RC::DOMINATION_RESULT_DBERROR;
            leave_member->SendPacket(&nak_msg, sizeof(nak_msg));
        }
        return;
    }

    // 디비 처리가 성공했으면 동기화를 위해 우선적으로 대상자를 제거한다.
    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    DominationArea* domination_area = domination_manager->GetAreaInfo(recv_msg->map_code_);
    if (domination_area == NULL)
    {
        return;
    }
    domination_area->GetMember()->MemberDelete(recv_msg->member_guid_);

    // 길드 마스터가 요청했으면 참여 인원 목록만 보낸다.
    if (recv_msg->member_guid_ != recv_msg->requester_guid)
    {
        Player* const guild_master = 
            PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->requester_guid);
        if (guild_master == NULL)
        {
            return;
        }
        domination_manager->SendMemberList(guild_master, recv_msg->map_code_);
        return;
    }
    
    // 자신이 요청했으면 성공 응답과 인원 목록 둘다 보낸다.
    Player* const leave_member = 
        PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->member_guid_);
    if (leave_member == NULL)
    {
        return;
    }
    MSG_CG_DOMINATION_MEMBER_LEAVE_ACK ack_msg;
    ack_msg.result_code_ = RC::DOMINATION_RESULT_SUCCESS;
    ack_msg.memeber_guid_ = recv_msg->member_guid_;
    leave_member->SendPacket(&ack_msg, sizeof(ack_msg));
    domination_manager->SendMemberList(leave_member, recv_msg->map_code_);
}

Handler_GZ_GUILD_IMPL(GZ_DOMINATION_MEMBERLIST_ANS)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_GZ_DOMINATION_MEMBERLIST_ANS* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBERLIST_ANS*>(pRecvMsg);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    DominationArea* domination_area = domination_manager->GetAreaInfo(recv_msg->map_code_);
    if (domination_area == NULL)
    {
        return;
    }

    domination_area->GetMember()->SetInfo(recv_msg->member_list_, recv_msg->data_count_);
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
Handler_GZ_GUILD_IMPL(GZ_GUILDPOINTSYSTEM)
{
    const MSG_GZ_GUILDPOINTSYSTEM* packet = \
        static_cast<MSG_GZ_GUILDPOINTSYSTEM*>(pRecvMsg);
    switch(packet->packet_type)
    {
        case ZG_GUILDPOINT_CHANGE_BRD:
        {
            const MSG_ZG_GUILDPOINT_CHANGE_BRD* packet2 = \
                static_cast<MSG_ZG_GUILDPOINT_CHANGE_BRD*>(pRecvMsg);

            MSG_CG_POINTWALLET_GUILDPOINT_CHANGE_BRD brd_msg( \
                packet2->guildpoint.guid_, packet2->guildpoint.code_,
                packet2->guildpoint.uuid_, packet2->guildpoint.point_);

            g_GameGuildManager.SendPacket(packet2->guildpoint.guid_, &brd_msg, sizeof(brd_msg));
        } break;
    }
}
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
Handler_GZ_GUILD_IMPL(GZ_GUILD_INTRODUCTION_SAVE_ACK)
{
    const MSG_GZ_GUILD_INTRODUCTION_SAVE_ACK* packet = \
        static_cast<MSG_GZ_GUILD_INTRODUCTION_SAVE_ACK*>(pRecvMsg);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    DWORD result = packet->result_;

    if(result == RC::RC_GUILD_SUCCESS)
    {
        MSG_CG_GUILD_INTRODUCTION_CHANGE_BRD brd_msg;
        memset(brd_msg.introduction_strings_, 0, sizeof(brd_msg.introduction_strings_));
        memcpy(brd_msg.introduction_strings_, \
            packet->introduction_strings_, MAX_GUILD_INTRODUCTION_LENGTH);
        g_GameGuildManager.SendPacket( packet->guild_guid, &brd_msg, sizeof(brd_msg));
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if (player && player->GetField())
    { 
        MSG_CG_GUILD_INTRODUCTION_SAVE_ACK msg;
        msg.result_ = result;
        player->SendPacket(&msg, sizeof(msg));
    }
#else  //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    PLAYER_PTR_USERKEY(Player, player, pRecvMsg->m_dwKey);

    DWORD result = packet->result_;

    MSG_CG_GUILD_INTRODUCTION_SAVE_ACK msg;
    msg.result_ = result;
    player->SendPacket(&msg, sizeof(msg));

    if(result == RC::RC_GUILD_SUCCESS)
    {
        MSG_CG_GUILD_INTRODUCTION_CHANGE_BRD brd_msg;
        memset(brd_msg.introduction_strings_, 0, sizeof(brd_msg.introduction_strings_));
        memcpy(brd_msg.introduction_strings_, \
            packet->introduction_strings_, MAX_GUILD_INTRODUCTION_LENGTH);
        g_GameGuildManager.SendPacket( player->GetGuildGuid(), &brd_msg, sizeof(brd_msg));
    }
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_REQUEST_ACK)
{
    const MSG_GZ_GUILD_JOIN_REQUEST_ACK* packet = \
        static_cast<MSG_GZ_GUILD_JOIN_REQUEST_ACK*>(pRecvMsg);

    DWORD result = packet->result_;

    if(result == RC::RC_GUILD_SUCCESS)
    {
        //길드원들에게 길드가입요청 목록을 추가하도록 패킷을 보냄
        MSG_CG_GUILD_JOIN_REQUEST_BRD brd_msg;
        brd_msg.request_info = packet->request_info;
        g_GameGuildManager.SendPacket( packet->request_info.m_guild_guid, &brd_msg, sizeof(brd_msg));
    }

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(packet->request_info.m_char_guid);
    if (player && player->GetField())
    { 
        MSG_CG_GUILD_JOIN_REQUEST_ACK msg;
        msg.guild_guid = packet->request_info.m_guild_guid;
        msg.result_ = result;
        player->SendPacket(&msg, sizeof(msg));
    }
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_JOIN_REQUEST_REJECT_ACK)
{
    const MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK* packet = \
        static_cast<MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK*>(pRecvMsg);

    DWORD result = packet->result;

    if(result == RC::RC_GUILD_SUCCESS)
    {
        // 길드원들에게 길드가입요청 목록에서 삭제하도록 패킷을 보냄
        MSG_CG_GUILD_JOIN_REQUEST_REJECT_BRD brd_msg;
        brd_msg.char_guid = packet->target_guid;
        g_GameGuildManager.SendPacket( packet->guild_guid, &brd_msg, sizeof(brd_msg));

        // 길드 가입 당사자에게 패킷을 보낸다
        Player* player = PlayerManager::Instance()->FindPlayerByCharKey(packet->target_guid);
        if (player == NULL || player->GetField() == NULL)
        {
            return;
        }

        MSG_CG_GUILD_JOIN_REQUEST_REJECT_ACK msg;
        msg.Init();
        util::GUILDNAMECOPY(msg.guild_name, packet->guild_name);
        msg.guild_guid = packet->guild_guid;

        switch(packet->reject_type)
        {
        case eGUILD_JOIN_REJECT_TYPE_NORMAL:
            msg.result_ = RC::RC_GUILD_JOIN_REQUEST_REJECT;
            break;
        case eGUILD_JOIN_REJECT_TYPE_TIMEOUT:
            msg.result_ = RC::RC_GUILD_JOIN_REQUEST_OVER_WAIT_TIME;
            break;
        case eGUILD_JOIN_REJECT_TYPE_FAILED: // 다른 길드에 가입하였거나 길드를 창설하였을 경우 패킷을 보내지 않는다
            return;
        default:
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Invalid reject type(%d)", packet->reject_type);
            return;
        }

        player->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        // 실패하였을 경우 해당 권한자에게 패킷을 보낸다
        Player* player = PlayerManager::Instance()->FindPlayerByCharKey(packet->char_guid);
        if (player && player->GetField())
        { 
            MSG_CG_GUILD_JOIN_REQUEST_REJECT_ACK msg;
            msg.Init();
            util::GUILDNAMECOPY(msg.guild_name, packet->guild_name);
            msg.guild_guid = packet->guild_guid;
            msg.result_ = result;
            player->SendPacket(&msg, sizeof(msg));
        }
    }
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_RELOAD_ACK)
{
    const MSG_GZ_GUILD_RELOAD_ACK* packet = \
        static_cast<MSG_GZ_GUILD_RELOAD_ACK*>(pRecvMsg);

    //현 서버에 해당 길드가 존재하지 않으면 실행할 필요 없다.
    GameGuild* pGuild = g_GameGuildManager.FindGuild( packet->guild_guid );
    if( !pGuild )		return;

    //가입자가 존재하면..
    Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( pTargetPlayer )
    {
        // 해당 유저의 정보에 세팅한다.
        pTargetPlayer->GetCharInfo()->m_GuildGuid = packet->guild_guid;
        util::GUILDNAMECOPY(pTargetPlayer->GetCharInfo()->m_tszGuildName, pGuild->GetGuildName());

        // GuildServer로 부터 ack를 받은후 ack보냄
        // 길드 번호를 BRD에서 전송하는 것도 괜찮을 듯..우선은 이대로...
        MSG_AG_GUILD_JOIN_SUCCESS_CMD msg;
        msg.m_GuildGuid = packet->guild_guid;
        pTargetPlayer->SendPacket( &msg, sizeof(msg) );

        // 길드 관계 리스트를 보내서 주변 유저들중에 관계를 맺고 있는 유저들의 렌더정보를 표시해주도록한다.
        MSG_CG_GUILD_ADD_INFO_ACK msgAddInfo;
        msgAddInfo.m_AddInfo.m_GuildGuid = packet->guild_guid;
        GuildSmallContentEntry::RelationSystem::LoadRelationInfo(&msgAddInfo.m_AddInfo);
        msgAddInfo.m_byRelCnt = msgAddInfo.m_AddInfo.m_byRelationGuildCnt;
        pTargetPlayer->SendPacket(&msgAddInfo, msgAddInfo.GetSize());

        // 길드 가입 후 주변 유저들이 길드 가입에 관련된 최소 정보
        // (길드 이름, 길드 마크, 길드 마크테두리, 길드 등급)를 볼수 있도록 해야한다.
        pTargetPlayer->FirstGuildMemberEnter();

        // 길드가입한 놈이 길드정보가 필요하면, 자동으로 G를 누르게 해서 관련 정보요청 패킷을 클라에서 보내도록 하자.
        MSG_CG_GUILD_JOIN_CMD	cmdMsg;
        cmdMsg.m_GuildGuid = packet->guild_guid;
        pTargetPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

        // 파티중일때 초대 받을 경우 파티멤버중에 현재 가입한 길드와 
        // 적대 관계인 길드 멤버가 존재하면  파티에서 나간다.
        WORD PartyKey = pTargetPlayer->GetPartyState().GetPartyKey();
        BOOL bIsExistHostilityMember = FALSE;

        bIsExistHostilityMember = NewGamePartyManager::Instance()->IsExistHostilityMember( PartyKey, pTargetPlayer->GetGuildGuid() );

        if( bIsExistHostilityMember )
        {
            MSG_AG_PARTY_LEAVE_BY_GUILDRELATION_SYN msg;
            msg.m_dwMemberKey =pTargetPlayer->GetObjectKey(); 
            msg.m_wPartykey = PartyKey;
            msg.m_bReason= eREASON_LEAVE_PARTY_BY_GUILDRELATION;
            g_pGameServer->SendToServer(AGENT_SERVER, &msg, sizeof(msg));
        }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        g_GameGuildManager.ApplyGuildFacilityPassive(pTargetPlayer, true);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
        pGuild->ApplyGuildRewardOption(pTargetPlayer);
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pTargetPlayer);  // 길드 가입 보상옵션 적용
#endif //_NA_008334_20150608_SONNENSCHEIN
        GameDominationManager::Instance()->SendAreaInfoToPlayer(pTargetPlayer);

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        // 카오스 존 내부에서 길드 가입시 인장 효과 제거..
        if(pTargetPlayer->GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)
        {
            GameInstanceDungeon* const instance_dungeon = \
                static_cast<GameInstanceDungeon*>(pTargetPlayer->GetGameZonePtr());
            if (instance_dungeon->GetTypeOfIndun() == INDUN_KIND_CHAOS_ZONE)
            {
#ifdef _NA_008334_20150608_SONNENSCHEIN
                pGuild->ApplyGuildRewardOption(pTargetPlayer, FALSE);
#else
                GameDominationManager::Instance()->ApplyAllRewardOption(pTargetPlayer, false);
#endif //_NA_008334_20150608_SONNENSCHEIN
                GameDominationManager::Instance()->SendAreaInfoToPlayer(pTargetPlayer, false);
            }
        }
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

        GameField* pField = pTargetPlayer->GetField();
        if (NULL != pField)
        {
            const FIELDCODE fCode = pField->GetFieldCode();
            GameDominationManager::Instance()->ApplyRewardField(pTargetPlayer,fCode);
        }
    }
}
Handler_GZ_GUILD_IMPL(GZ_GUILD_FACILITY_UPGRADE_ACK)
{
    const MSG_GZ_GUILD_FACILITY_UPGRADE_ACK* recv_msg =
        static_cast<MSG_GZ_GUILD_FACILITY_UPGRADE_ACK*>(pRecvMsg);

    if (recv_msg->result == RC::RC_GUILD_SUCCESS)
    {
        GameGuild* game_guild = g_GameGuildManager.FindGuild(recv_msg->guild_guid);
        if (game_guild != NULL)
        {
            MSG_CG_GUILD_FACILITY_UPGRADE_BRD brd_msg;
            brd_msg.facility_info = recv_msg->facility_info;
            game_guild->SendPacketAll(&brd_msg, sizeof(brd_msg));
            
            //시설 업그레이드 처리
            game_guild->UpdateGuildFacility(recv_msg->char_guid, recv_msg->facility_info, recv_msg->cur_level, recv_msg->add_level);
        }
    }

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->char_guid);
    if (player != NULL)
    {
        MSG_CG_GUILD_FACILITY_UPGRADE_ACK ack_msg;
        ack_msg.facility_code = recv_msg->facility_code;
        ack_msg.result = recv_msg->result;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
    }
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_FACILITY_SELECT_ACK)
{
    const MSG_GZ_GUILD_FACILITY_SELECT_ACK* recv_msg =
        static_cast<MSG_GZ_GUILD_FACILITY_SELECT_ACK*>(pRecvMsg);

    GameGuild* game_guild = g_GameGuildManager.FindGuild(recv_msg->guild_guid);
    if (game_guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
            "["__FUNCTION__"] 길드 시설 정보를 저장하는데 게임서버에 해당길드가 없다(%d)", 
            recv_msg->guild_guid);
        return;
    }
    GuildFacilities*const guild_facilities = game_guild->GetGuildFacilities();
    if (guild_facilities == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
            "["__FUNCTION__"] 길드 시설 정보를 저장하는데 해당길드에 길드시설 클래스가 없다(%d)", 
            recv_msg->guild_guid);
        return;
    }
    for (int i = 0; i < recv_msg->total_info.m_count; ++i)
    {
        guild_facilities->SetGuildFacility(recv_msg->total_info.m_GuildFacilityInfo[i]);
    }

    guild_facilities->SetLoaded(true);

    //모든 길드 시설 정보가 로드된 후에 모든 길드원들에게 보내준다
    MSG_CG_GUILD_FACILITY_TOTALINFO_ACK total_msg;
    total_msg.guild_facility_total_info = recv_msg->total_info;
    game_guild->SendPacketAll(&total_msg, sizeof(total_msg));

    //모든 길드 시설 패시브를 모든 길드원들에게 적용시킨다.
    g_GameGuildManager.ApplyGuildFacilityPassiveAllmember(recv_msg->guild_guid, true);
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_003923_20120130_GUILD_RENEWAL
Handler_GZ_GUILD_IMPL(GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS)
{
    const MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS* recv_msg =
        static_cast<MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS*>(pRecvMsg);

    Player* const request_player = 
        PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->request_char_guid_);
    if (request_player == NULL)
    {
        return;
    }

    MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS send_msg;
    send_msg.log_category_ = recv_msg->log_category_;
    send_msg.log_count_ = recv_msg->log_count_;
    CopyMemory(send_msg.log_data_, recv_msg->log_data_, sizeof(send_msg.log_data_));

    request_player->SendPacket(&send_msg, send_msg.GetSize());
}
#endif

Handler_GZ_GUILD_IMPL(GZ_GUILD_RENAME_CHAR_CMD)
{
    const MSG_GZ_GUILD_RENAME_CHAR_CMD* recv_msg = \
        static_cast<MSG_GZ_GUILD_RENAME_CHAR_CMD*>(pRecvMsg);

    GUILDGUID guild_guid = recv_msg->guildguid;
    CHARGUID char_guid = recv_msg->charguid;
    const TCHAR* character_name = recv_msg->character_name_;

    GameGuild* pGameGuild = g_GameGuildManager.FindGuild(guild_guid);
    if (pGameGuild) 
    {
        GameGuildMember* guild_member = pGameGuild->FindMember(char_guid);
        if (guild_member) {
            guild_member->SetCharName(character_name);
        }
    }

    GameDominationManager::Instance()->GuildMasterNameChange( \
        guild_guid, char_guid, character_name);
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
Handler_GZ_GUILD_IMPL(GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD)
{
    MSG_GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD* ack_msg = \
        static_cast<MSG_GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD*>(pRecvMsg);

    g_GameGuildManager.InitializeAllSonnenScheinPhase();
}

Handler_GZ_GUILD_IMPL(GZ_GUILD_SONNENSCHEIN_UPDATE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));

    MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_ACK* ack_msg = \
        static_cast<MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_ACK*>(pRecvMsg);

    // 사실 GameServer정보를 갱신 후, 소넨샤인 종료 시, GameServer정보를 DB로 전송하는 것이기 때문에,
    // 구지 이곳에서 GameServer Guild의 정보를 재갱신 시켜줄 필요는 없다.
    // 하지만 혹시모를 쿼리 실패등을 고려해서 DB에 업데이트된 정보를 GameGuild에 갱신시켜준다
    g_GameGuildManager.SetGuildSonnenScheinPhase(ack_msg->guild_guid, ack_msg->sonnenschein_phase);

    GameGuild* guild = g_GameGuildManager.FindGuild(ack_msg->guild_guid);
    if (NULL != guild)
    {
        MSG_CG_GUILD_SONNENSCHEIN_UPDATE_CMD cmd_msg;
        cmd_msg.sonnenschein_phase = ack_msg->sonnenschein_phase;
        cmd_msg.m_dwPlayerKey = 0;
        guild->SendPacketMemberAround(cmd_msg, INVALID_GUILDGUID);
    }
}
#endif //_NA_008334_20150608_SONNENSCHEIN