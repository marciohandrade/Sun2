#include "stdafx.h"
#include ".\handler_dg_charinfo.h"
#include <PacketStruct_DG.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>
#include "PlayerManager.h"
#include "Player.h"
#include <Macro.h>
#include <ServerSession.h>
#include <VarPacket.h>
#include "ItemManager.h"
#include "SCItemSlotContainer.h"
#include "DropManager.h"
#include "ObjectFactory.h"
#include "QuestManager.h"
#include "MissionManager.h"
#include "Skill.h"
#include "SkillManager.h"
#include <SkillSystem/PerkManager.h> //_NA002217_100728_EP2_PERK_SYSTEM_
#include "GameZone.h"
#include "GameZoneManager.h"
#include "WarehouseSlotContainer.h"
#include "GuildWarehouseSlotContainer.h"
#include "QuickManager.h"
#include "QuickStyleManager.h"
#include "EventInventorySlotContainer.h"
#include "GameServerEx.h"
#include "MissionManager.h"
#include "QuestManager_Concrete.h"
#include <AreaConquestCommonHeader.h>
#include <SunEventInfoParser.h>
#include <WayPoint.h>
#include "StatusManager.h"
#include <BitStream.hpp>
#include "GameShopManager.h"
#include ".\FriendInvitation\GFriendInvitationController.h"

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
#include <FatigueOptionParser.h>
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

#include "RankingSystemCtl.h"

#include "GameGuildManager.h"
#include <PacketStruct_GZ.h>
#include <World/SSQ/SSQHandlerInGame.h>

#include "CollectedList.h"
#include "GameDominationManager.h"
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    #include "AccumulatePoint.h"
#endif
#include "SystemMemo.h"
//_NA_20100604_BETTER_DOMINATION_MANAGER
#include ".\GameGuild.h"

#include "ComposeOrDecomposeImplement.h"

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    #include "./SkillSystem/SkillCoolTimeAccessor.h"
#endif
#include "ImplementUtil.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include <HonorSystem/HonorSystem.h>
#include <HonorSystem/HonorSystemPacket.h>
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    #include "./GameShop/GameShop.Manager.h"
    #include "./GameShop/GameShop.EventProcessor.h"
#endif

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
#include "PeakTimeEvent.h"
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#include <ExtraNpcManager.h>
#include <PacketStruct_MonsterBook.h>

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#include "SunRanking/SunRankingGradeManager.h"
#include "StatueManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_DG_CHARINFO::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_DG_CHARINFO::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(DG_ITEM, DG_ITEM_SERIAL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_ITEM, DG_ITEM_SERIAL_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_CHAR_ITEM_CMD, PHASE_SERVICE),
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD, PHASE_SERVICE),
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496
        //_KR_0_20091021_FIRENDINVITATION_EXTEND
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_INVITATEDFRIEND_CMD, PHASE_SERVICE),
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED	
		HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_CHAR_ACHIEVEMENT_CMD, PHASE_SERVICE),
#endif

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_COOL_TIME_CMD, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_QUICK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_STYLE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_QUEST_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_EXTRA_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_UPDATE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_UPDATE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_QUERY_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_QUERY_NAK, PHASE_SERVICE),
        //_NA002217_100728_EP2_PERK_SYSTEM_
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_LIST_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CONTROL_CMD, PHASE_SERVICE),
        //

        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_START_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_START_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_END_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_END_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_PWD_SETTING_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_ITEM, DG_ITEM_INVENTORY_FLAG_STREAM_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_FLAG_STREAM_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SELECT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SELECT_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_UPDATE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_UPDATE_NAK, PHASE_SERVICE),

        // 오픈베타 에테르 이벤트
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_ITEM_STATUS_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_ITEM_STATUS_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_ITEM_REG_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_ITEM_REG_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_USETIME_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_QUERY_EXIST_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_STATUS_UPDATE_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SSQ_REWARD_EXIST_CMD, PHASE_SERVICE),
        //					  DG_EVENT_SSQ_REWARD_UPDATE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_ACK, PHASE_SERVICE),
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK, PHASE_SERVICE),    
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK, PHASE_SERVICE),
        //}
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_LOTTO_REWARD_AND_DEL_ACK, PHASE_SERVICE),
#endif////_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK, PHASE_SERVICE),
        // 미션 랭킹 관련
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_RECORD_SELECT_CMD, PHASE_SERVICE),
        //	HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_CLEAR_UPDATE_ACK, PHASE_SERVICE),
        //HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_CLEAR_UPDATE_NAK, PHASE_SERVICE),
        //HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_RECORD_UPDATE_ACK, PHASE_SERVICE),
        //HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_RECORD_UPDATE_NAK, PHASE_SERVICE),
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXIST_CHARNAME_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXIST_CHARNAME_NAK, PHASE_SERVICE),
#else
    #if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXIST_CHARNAME_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXIST_CHARNAME_NAK, PHASE_SERVICE),
    #endif
#endif
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_SHOP_RECV_GIFT_NTF, PHASE_SERVICE),
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_FATIGUE_CMD, PHASE_SERVICE),
#endif


        HANDLER_NODE_INFO(DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_NAK, PHASE_SERVICE),
        //{__NA001164_20081024_CHAR_WAY_POINT_PACKET
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_LOAD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_SAVE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_DEL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_NAK, PHASE_SERVICE),
        //}
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        HANDLER_NODE_INFO(DG_MISSION, DG_MISSION_RANKING_POINT_REQ_ACK, PHASE_SERVICE),
#endif // _NA_008012_20150130_RANKING_SYSTEM

        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_NAK, PHASE_SERVICE),
        //{__NA001283_20090227_COLLECTION_LIST
        HANDLER_NODE_INFO(DG_COLLECT, DG_COLLECT_LIST_LOAD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_COLLECT, DG_COLLECT_ADD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_COLLECT, DG_COLLECT_DEL_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(DG_COLLECT, DG_COLLECT_NAK, PHASE_SERVICE),
        //}
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_HEIM_LOTTO_PICK_NTF, PHASE_SERVICE),
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK, PHASE_SERVICE), 
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_ACK, PHASE_SERVICE), 

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_HONORSYSTEM, PHASE_UNLIMITED),
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_POINTWALLET_SYSTEM, PHASE_UNLIMITED),
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARNAME_CHANGE_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_CMD, PHASE_UNLIMITED), // _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SMARTNPC_SYSTEM, PHASE_UNLIMITED),
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_007086_20140318_MONSTERBOOK
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_CMD, PHASE_UNLIMITED),
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
        HANDLER_NODE_INFO(DG_CHARINFO, DG_USER_EXTRA_INFO_CMD, PHASE_UNLIMITED),
#endif // _NA_007932_20150106_USER_EXTRA_INFO
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
        HANDLER_NODE_INFO(DG_EVENT, DG_GMEVENT_PEAKTIME_REGISTER_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_GMEVENT_PEAKTIME_TERMINATE_ACK, PHASE_UNLIMITED),
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_UPDATE_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_SELECT_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_START_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_COMPLETE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_COMPLETE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD, PHASE_UNLIMITED),
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_DG_CHARINFO_IMPL( DG_CHARINFO_CHAR_ITEM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_CHAR_ITEM_CMD * pRecvMsg = (MSG_DG_CHARINFO_CHAR_ITEM_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	// Agent에 플레이어 HP 정보를 보내준다.
	MSG_AG_CHARINFO_CHAR_CMD CharInfoMsg;
	CharInfoMsg.m_MaxHP = pRecvMsg->m_CharacterInfo.m_iMaxHP;
	CharInfoMsg.m_HP    = pRecvMsg->m_CharacterInfo.m_iHP;
	CharInfoMsg.m_MaxMP = pRecvMsg->m_CharacterInfo.m_iMaxMP;
	CharInfoMsg.m_MP    = pRecvMsg->m_CharacterInfo.m_iMP;

	CharInfoMsg.m_dwCHuntingVCount   = pRecvMsg->m_CharacterInfo.m_iCHuntingVCount;
	CharInfoMsg.m_dwCHuntingTotCount = pRecvMsg->m_CharacterInfo.m_iCHuntingTotCount;
	CharInfoMsg.m_byChaoState        = pRecvMsg->m_CharacterInfo.m_ChaoState;

    CharInfoMsg.m_MaxSD = pRecvMsg->m_CharacterInfo.m_iMaxSD;
    CharInfoMsg.m_SD    = pRecvMsg->m_CharacterInfo.m_iSD;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    CharInfoMsg.max_FP = pRecvMsg->m_CharacterInfo.max_FP_;
    CharInfoMsg.cur_FP = pRecvMsg->m_CharacterInfo.cur_FP_;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	pPlayer->SendPacket( &CharInfoMsg, sizeof(CharInfoMsg) );
	// 캐릭터 정보 셋팅
	pPlayer->SetCharInfo( &pRecvMsg->m_CharacterInfo );

	ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
	rPROTECT.SetPasswordFromDB( pRecvMsg->m_CharacterInfo.m_InventoryPwd );
	BOOL bPwdSet = rPROTECT.IsEmptyPassword() ? FALSE : TRUE;
	rPROTECT.Block( bPwdSet, FALSE );

	{
		eSPECIAL_RENDER_EFFECT effect = eSPECIAL_RENDER_EFFECT_NONE;
		switch( pRecvMsg->m_CharacterInfo.m_ChangeOfClassStep )
		{
		case eCHAREX_BERSERKER_100LEVEL:
		case eCHAREX_DRAGON_KNIGHT_100LEVEL:
		case eCHAREX_SHADOW_100LEVEL:
		case eCHAREX_VALKYRIE_100LEVEL:
		case eCHAREX_ELEMENTALIST_100LEVEL:
        case eCHAREX_MYSTIC_100LEVEL:
        case eCHAREX_HELLROID_100LEVEL:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAREX_WITCHBLADE_100LEVEL:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			effect = eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st;
		}
		pPlayer->RENDER_EFFECT.AddEffect( effect );
	};

	// 아이템 정보 셋팅
//	pPlayer->GetItemManager()->DBLoad( &pRecvMsg->m_ItemInfo );
	pPlayer->GetItemManager()->SerializeItemTotalInfo( pRecvMsg->m_ItemInfo, SERIALIZE_STORE );

    if (pRecvMsg->m_CharacterInfo.m_bActiveEtherDevice) {
        pPlayer->m_EtherManager.ActiveBulletItem( pRecvMsg->m_CharacterInfo.m_byEtherBulletPos );
    }
}

#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD * pRecvMsg = (MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD *)pMsg;
    DWORD UserKey = pRecvMsg->m_dwKey;
    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
    if( !pPlayer ) return ;

    pPlayer->GetItemManager()->GetTradingImpl()->SerializeRepurchaseItems(
        pRecvMsg->m_RepurchasItem, SERIALIZE_STORE);
}
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_INVITATEDFRIEND_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_CHARINFO_INVITATEDFRIEND_CMD * pRecvMsg = (MSG_DG_CHARINFO_INVITATEDFRIEND_CMD *)pMsg;
    DWORD UserKey = pRecvMsg->m_dwKey;
    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
    if( !pPlayer ) return ;

    GFriendInvitationController::Instance()->GetInvitatedFrindInfo(pPlayer, 
                                                                pRecvMsg, SERIALIZE_STORE); 
}

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED	
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_CHAR_ACHIEVEMENT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD * pRecvMsg = (MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->GetACManager()->Serialize(pRecvMsg->achievement_info_, ACHIEVEMENT_SAVE);
}
#endif

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_SKILL_COOL_TIME_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD * pRecvMsg = (MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD *)pMsg;
    Player * player = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(player == NULL) 
        return;

    SkillCoolTimeAccessor::Instance()->ApplySkillCoolTime(player, &(pRecvMsg->skill_cool_time));
}
#endif

Handler_DG_CHARINFO_IMPL( DG_CHARINFO_SKILL_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_SKILL_CMD * pRecvMsg = (MSG_DG_CHARINFO_SKILL_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->SerializeSkillTotalInfo( pRecvMsg->m_SkillInfo, SERIALIZE_STORE );
}
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_QUICK_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_QUICK_CMD * pRecvMsg = (MSG_DG_CHARINFO_QUICK_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->GetQuickManager()->SerializeQuickTotalInfo( pRecvMsg->m_QuickInfo, SERIALIZE_STORE );
	pPlayer->GetQuickManager()->UpdateOriginalItems();
}

Handler_DG_CHARINFO_IMPL( DG_CHARINFO_STYLE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_STYLE_CMD * pRecvMsg = (MSG_DG_CHARINFO_STYLE_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->GetStyleManager()->SerializeStyleTotalInfo( pRecvMsg->m_StyleInfo, SERIALIZE_STORE );
}

Handler_DG_CHARINFO_IMPL( DG_CHARINFO_QUEST_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_QUEST_CMD * pRecvMsg = (MSG_DG_CHARINFO_QUEST_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->GetMissionManager()->SerializeStream( pRecvMsg->m_pMissionStream, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE );
	pPlayer->GetQuestManager()->SerializeProgrStream( pRecvMsg->m_pProgrQStream, MAX_PROGR_QUESTSTREAM_SIZE, QSERIALIZE_SAVE );
	pPlayer->GetQuestManager()->SerializeComplStream( pRecvMsg->m_pComplQStream, MAX_COMPL_QUESTSTREAM_SIZE, QSERIALIZE_SAVE );
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
	pPlayer->GetACManager()->Serialize( pRecvMsg->m_pACStream, MAX_ACSTREAM_SIZE, ACSERIALIZE_SAVE );
#endif

	// Agent로 QUEST정보 보냄
	/*MSG_AG_CHARINFO_QUEST_CMD msg;
	memcpy( msg.m_pMissionStream, pRecvMsg->m_pMissionStream, MAX_MISSIONSTREAM_SIZE );
	memcpy( msg.m_pQuestStream, pRecvMsg->m_pQuestStream, MAX_MISSIONSTREAM_SIZE );
	pPlayer->SendPacket( &msg, sizeof(msg) );*/
}

Handler_DG_CHARINFO_IMPL( DG_CHARINFO_EXTRA_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_EXTRA_CMD * pRecvMsg = (MSG_DG_CHARINFO_EXTRA_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;

	pPlayer->GetStatusManager()->SerialzieStatusDetailInfo( pRecvMsg->m_TotalInfo, SERIALIZE_STORE );
	pPlayer->SerialzieStatusGMInfo( pRecvMsg->m_GMStateInfo, SERIALIZE_STORE );
}

//==================================================================================================

Handler_DG_CHARINFO_IMPL(DG_CHARINFO_SKILL_LIST_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    ASSERT(!"Current version not supported");
    return;
#endif
}

Handler_DG_CHARINFO_IMPL(DG_CHARINFO_SKILL_PERK_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    ASSERT(!"Current version not supported");
    return;
#endif

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    const MSG_DG_CHARINFO_SKILL_PERK_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_SKILL_PERK_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const CHARGUID char_guid = recv_msg->char_guid_;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid);
    if (!FlowControl::FCAssert(player && player->GetCharGuid() == char_guid)) {
        return;
    }

    nsSkill::PerkManager* perk_manager = player->GetPerkManager();
    perk_manager->StorePerk(recv_msg->container_, char_guid);
#endif
}

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
    extern void __WxTest_Receive_Perk_Control_DG(const MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD* msg_cmd);
    #define DEBUG_REPORT_RECEIVE_PERK_CONTROL(msg) __WxTest_Receive_Perk_Control_DG(msg)
#else
    #define DEBUG_REPORT_RECEIVE_PERK_CONTROL(msg)
#endif

Handler_DG_CHARINFO_IMPL(DG_CHARINFO_SKILL_PERK_CONTROL_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    ASSERT(!"Current version not supported");
    return;
#endif
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    const MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD* recv_msg = \
        static_cast<MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (!FlowControl::FCAssert(player != NULL)) {
        return;
    }
    //
    GameField* const game_field = player->GetField();
    if (game_field == NULL) {
        return;
    }
    //
    nsSkill::PerkManager* perk_manager = player->GetPerkManager();
    const PerkControl& req_info = recv_msg->header;
    switch (req_info.request)
    {
    case req_info.eReq_InsertS2S:
    case req_info.eReq_DeleteS2S:
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=unauthrization request|UserGuid=%u|Requester=%s|"),
               __FUNCTION__, user_key, player->GetCharName());
        return;
    }
    //
    MSG_CG_SKILL_PERK_CONTROL_CMD msg_cmd;
    nsSkill::PerkManager::eRequestResult result = \
        perk_manager->ProcessRequestedMessage(req_info, &msg_cmd.header, &msg_cmd.data);
    if (result == perk_manager->eReqResult_NeedForward)
    {
        ASSERT(!"unexprected status");
    }
    else if (result == perk_manager->eReqResult_ReplyResult)
    {
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    }
    DEBUG_REPORT_RECEIVE_PERK_CONTROL(recv_msg);
#endif //_NA002217_100728_EP2_PERK_SYSTEM_
}

//==================================================================================================

class PassiveSkillListOpr
{
	BYTE		m_byCount;
	SLOTCODE	*m_SkillCodeArray;
public:
	PassiveSkillListOpr( SLOTCODE *SkillCodeArray ) : m_SkillCodeArray(SkillCodeArray), m_byCount(0) {}
	BYTE		GetCount()		{ return m_byCount; }

	VOID operator () ( Skill *pSkill ) 
	{
		m_SkillCodeArray[m_byCount] = pSkill->GetSkillCode();
		++m_byCount;
	}
};
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_UPDATE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_UPDATE_ACK * pRecvMsg = (MSG_DG_CHARINFO_UPDATE_ACK *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return;

	//SUNLOG( eFULL_LOG, "DG_CHARINFO_UPDATE_ACK[%s][%d]", pPlayer->GetCharName(), pPlayer->GetBehave() );
	
	g_GameGuildManager.DestroyMember( pPlayer->GetCharInfo()->m_GuildGuid, pPlayer->GetCharGuid() );

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingManager::Instance()->ProcessDisconnect(pPlayer);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

    MSG_DG_CHARINFO_DISCONNECT_CHAR_CMD cmd;
	pPlayer->SendToGameDBPServer( &cmd, sizeof(cmd) );

	MSG_AG_ZONE_ROOM_LEAVE_ACK msg;
	pPlayer->SendPacket( &msg, sizeof(msg) );

    PlayerManager::Instance()->RemovePlayer( pPlayer );
}
Handler_DG_CHARINFO_IMPL( DG_CHARINFO_UPDATE_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_UPDATE_NAK * pRecvMsg = (MSG_DG_CHARINFO_UPDATE_NAK *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return;

	//SUNLOG( eCRITICAL_LOG, "DG_CHARINFO_UPDATE_NAK[%u][%u][%u]", UserKey, pPlayer->GetUserID(), pRecvMsg->m_dwErrorCode );

	//2007.02.14 발렌타인데이 쓸쓸하게 추가함.
	MSG_AG_ZONE_ROOM_LEAVE_NAK nmsg;
	nmsg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &nmsg, sizeof(nmsg) );	
}

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
extern void __WxTest_Receive_Perk_List(const MSG_CG_CHARINFO_SKILL_PERK_CMD* msg_cmd);
#define DEBUG_REPORT_RECEIVE_PERK_LIST(msg) __WxTest_Receive_Perk_List(msg)
#else
#define DEBUG_REPORT_RECEIVE_PERK_LIST(msg)
#endif

Handler_DG_CHARINFO_IMPL(DG_CHARINFO_QUERY_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DG_CHARINFO_QUERY_ACK* recv_msg = static_cast<MSG_DG_CHARINFO_QUERY_ACK*>(pMsg);
    PlayerManager* const player_manager = PlayerManager::Instance();
    const DWORD user_key = recv_msg->m_dwKey;
    Player* const player = player_manager->FindPlayerByUserKey(user_key);
    if (player == NULL) {
        return;
    }
    player_manager->AddPlayerName(player);
    //
    const ePLAYER_STATE player_state = static_cast<ePLAYER_STATE>(player->GetState());
    if (player_state == PLAYER_CHARINFO_FIRST_REQUESTED)
    {
        static char busy_access_buffer[1024 * 64];
        ;{
            MSG_CG_CHARINFO_CHAR_ITEM_CMD& msg_items = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_CHAR_ITEM_CMD;
            //
            player->CopyToCharInfo(msg_items.m_CharacterInfo);
            // TODO: need check about size cacluation
            msg_items.m_Size = sizeof(RENDER_ITEMSTREAMEX) *
                               (MAX_INVENTORY_SLOT_NUM + MAX_TEMPINVENTORY_SLOT_NUM);
            BitStream stream(msg_items.m_BitStreamBuffer, msg_items.m_Size, true);
            //// LOAD
            // removed by __001347_LTJ_REMOVE_TEMP_INVENTORY
            //GetItemSlotContainer(SI_TEMPINVENTORY)->Serialize(stream, eSLOT_SERIALIZE_CLIENT_LOAD);
            //// 유료화탭은 별도로 분리해야함
            SCItemSlotContainer* const inventory = \
                player->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
            inventory->Serialize(stream, eSLOT_SERIALIZE_CLIENT_LOAD);
            msg_items.m_Size = stream.GetNumberOfBytesUsed();

            msg_items.Encode();
            player->SendPacket(&msg_items, msg_items.GetSize());
        };
        ;{
            MSG_CG_CHARINFO_SKILL_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_SKILL_CMD;
            player->SerializeSkillTotalInfo(msg_cmd.m_SkillInfo, SERIALIZE_LOAD);
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        };
        ;{
            MSG_CG_CHARINFO_QUICK_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_QUICK_CMD;
            player->GetQuickManager()->SerializeQuickTotalInfo(msg_cmd.m_QuickInfo, SERIALIZE_LOAD);
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        };
        ;{
            MSG_CG_CHARINFO_STYLE_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_STYLE_CMD;
            player->GetStyleManager()->SerializeStyleTotalInfo(msg_cmd.m_StyleInfo, SERIALIZE_LOAD);
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        };

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
        ;{
            MSG_CG_CHARINFO_SKILL_COOL_TIME_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_SKILL_COOL_TIME_CMD;
            SkillCoolTimeAccessor::Instance()->GetSkillCoolTime(player, &(msg_cmd.skill_cool_time));
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        };
#endif

    #ifdef _NA002217_100728_EP2_PERK_SYSTEM_
        ;{
            //MSG_CG_CHARINFO_SKILL_LIST_CMD& msg_cmd = \
            //    *new (busy_access_buffer) MSG_CG_CHARINFO_SKILL_LIST_CMD;
        };
        ;{
            MSG_CG_CHARINFO_SKILL_PERK_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_CHARINFO_SKILL_PERK_CMD;
            nsSkill::PerkManager* const perk_manager = player->GetPerkManager();
            perk_manager->LoadPerk(&msg_cmd.container_);
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
            DEBUG_REPORT_RECEIVE_PERK_LIST(&msg_cmd);
        };
    #endif
        ;{
            MSG_AG_MISSION_STREAM_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_AG_MISSION_STREAM_CMD;
            player->GetMissionManager()->SerializeStream(msg_cmd.m_TotalInfo, SERIALIZE_LOAD);
            if (0 != msg_cmd.m_TotalInfo.m_Count)
                player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        };
        ;{
            // 퀘스트 정보 보냄
            player->GetQuestManager()->SendQuestStream();
        };
        ;{
            // AC 정보 보냄
            MSG_CG_AREACONQUEST_STREAM_CMD& msg_cmd = \
                *new (busy_access_buffer) MSG_CG_AREACONQUEST_STREAM_CMD;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
			player->GetACManager()->Serialize(msg_cmd.achievement_info_, ACSERIALIZE_CLIENT_LOAD);
            player->SendPacket(&msg_cmd, msg_cmd.GetSize());
#else
            player->GetACManager()->Serialize(msg_cmd.m_ACStream, MAX_ACSTREAM_SIZE, ACSERIALIZE_LOAD);
            player->SendPacket(&msg_cmd, sizeof(msg_cmd));
#endif            
        };

    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        // 적립 포인트 보냄
        player->GetAccumulatePoint()->SendPacketAccumulatePoint();
    #endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        {
            WORD normalQ_count, dailyQ_count;
            player->GetQuestManager()->GetCompleteQuestCount(normalQ_count, dailyQ_count);
            player->GetACManager()->ProcessQuestCount(normalQ_count, dailyQ_count);
        }
#endif//_NA_003027_20111013_HONOR_SYSTEM
//#ifdef _NA_006372_20130108_BADGE_ITEM
//        ;{
//            MSG_CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD& msg_cmd = \
//                *new (busy_access_buffer)MSG_CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD;
//
//            const nsSlot::TOGGLE_ITEMS* toggle_item_info = player->GetToggleItemInfo();
//            memcpy(msg_cmd.toggle_items_info_, toggle_item_info, sizeof(msg_cmd.toggle_items_info_));
//            player->SendPacket(&msg_cmd, sizeof(msg_cmd));
//        };
//#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        ;{
            MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD& cmd_msg = \
                *new (busy_access_buffer)MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD;

            // 전장전적..
            const BattleGroundRecordInfos record_infos = \
                player->GetExtraInfoManager().GetBattleGroundRecordInfos();
            memcpy(cmd_msg.chaos_zone_record_infos.battle_ground_record, 
                   record_infos.battle_ground_record, 
                   sizeof(cmd_msg.chaos_zone_record_infos.battle_ground_record));
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
            const GoldRushRecordInfos goldrush_infos = \
                player->GetExtraInfoManager().GetGoldRushRecordInfos();
            memcpy(cmd_msg.chaos_zone_record_infos.goldrush_record, goldrush_infos.goldrush_record,
                sizeof(cmd_msg.chaos_zone_record_infos.goldrush_record));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

            player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        };
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007086_20140318_MONSTERBOOK
        ;{
            MonsterBook& monsterbook = player->GetMonsterBook();
            MSG_CG_MONSTERBOOK_SERIALIZE_CMD& cmd_msg = 
                *new (busy_access_buffer) MSG_CG_MONSTERBOOK_SERIALIZE_CMD;
            cmd_msg.monster_code_count = monsterbook.monster_code_count;
            memcpy(&cmd_msg.monster_code, &monsterbook.monster_code, sizeof(cmd_msg.monster_code));
            player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        };
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        ;{
            AttendanceManager& attendance_manager = player->GetExtraInfoManager().GetAttendanceManager();
            attendance_manager.ToAttend(player);

            MSG_CG_CHARINFO_ATTENDANCE_INFO_PACKET msg;
            memcpy(&msg.attendance_infos, &attendance_manager.GetAttendanceInfos(), 
                sizeof(msg.attendance_infos));
            player->SendPacket(&msg, sizeof(msg));
        };
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        ;{
            MSG_CG_CHARINFO_SUN_RANKING_GRADE_PACKET msg;
            SunRankingGradeManager::Instance()->GetTopGrade(player->GetCharGuid(), msg.grade, msg.ranking_type);
            player->SendPacket(&msg, sizeof(msg));
        };
        ;{
            MSG_CG_ETC_STATUE_INFO_ACK msg;
            StatueInfo* statue_info = StatueManager::Instance()->GetStatue(eSTATUE_TYPE_BEST_OF_BEST, eSTATUE_INDEX_FIRST);
            if (statue_info != NULL)
            {
                msg.info = *statue_info;
                player->SendPacket(&msg, sizeof(msg));
            }
        };
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        ;{
            if (player->GetExtraInfoManager().GetMiniGameManager().IsEventProgress(eMINIGAME_TYPE_BINGO))
            {
                MSG_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET info_packet;
                memcpy(&info_packet.minigame_bingo_infos,
                    &(player->GetExtraInfoManager().GetMiniGameManager().GetMiniGameBingoInfos()), 
                    sizeof(info_packet.minigame_bingo_infos));
                player->SendPacket(&info_packet, sizeof(info_packet));
            }
        };
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    }
    else if (player_state == PLAYER_CHARINFO_REQUESTED)
    {
        // nothing to do
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|[U:%d,ID:%s]InvalidState"),
               __FUNCTION__, user_key, player->GetUserID());
    };
    //
    //---------------------------------------------------------------------------------------------
    // extra control
    ;{
        //캐릭터 정보를 처음 세팅하는 곳이므로... 적립된 HP는 maxhp의 30%로 한다.
        const DWORD max_hp = player->GetMaxHP();
        const DWORD hp = player->GetHP();
        DWORD reserve_hp = static_cast<DWORD>(double(max_hp) * 0.3f);
        DWORD calc = hp + reserve_hp;
        if (calc > max_hp) {
            reserve_hp = calc - max_hp;
        }
        player->SetReserveHP(reserve_hp); // range = [0, max_hp * 30%]
    };
    //---------------------------------------------------------------------------------------------
    // manipulate logic related to guild
    if (player->GetGuildGuid() != 0)
    {
        if (player_state == PLAYER_CHARINFO_FIRST_REQUESTED)
        {
            // 최초 접속이면 길드서버에 로그인 준비를 시킨다.
            // 여기서 로그인을 못하는 이유는 아직 클라이언트가 로딩중이기 때문이다.
            // 로그인 패킷이 있는데 이 패킷을 보내는 이유는 현재 최초 접속인데 길드 서버에
            // 로그인한 상태의 유저가 있을지 모르기 때문이다.
            // 길드 서버에 로그인한 상태의 유저가 있으면 해당 유저는 길드정보를 받지 못하기 때문에 안된다.
            MSG_GZ_GUILD_PRE_LOGIN_CMD msg_pre_login;
            msg_pre_login.m_GuildGuid = player->GetGuildGuid();
            msg_pre_login.m_CharGuid  = player->GetCharGuid();
            if (!player->SendToGuildServer(&msg_pre_login, sizeof(msg_pre_login)))
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|MSG_GZ_GUILD_PRE_LOGIN_CMD message send failed, ")
                       _T("GuildGuid = %u, CharGuid = %u|"),
                       __FUNCTION__, msg_pre_login.m_GuildGuid, msg_pre_login.m_CharGuid);
            }
#ifdef _NA_008334_20150608_SONNENSCHEIN
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(player);  // 보상옵션 적용
#endif //_NA_008334_20150608_SONNENSCHEIN
        }
        else if (player_state == PLAYER_CHARINFO_REQUESTED)
        {
            //최초 접속이 아니면 게임서버간 이동이므로...
            MSG_GZ_GUILD_ENTER_SERVER_SYN msg_enter_server;
            msg_enter_server.m_GuildGuid = player->GetGuildGuid();
            msg_enter_server.m_CharGuid  = player->GetCharGuid();
            if (!player->SendToGuildServer(&msg_enter_server, sizeof(msg_enter_server)))
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|MSG_GZ_GUILD_ENTER_SERVER_SYN message send failed, ")
                       _T("GuildGuid = %u, CharGuid = %u|"),
                       __FUNCTION__, msg_enter_server.m_GuildGuid, msg_enter_server.m_CharGuid);
            }
#ifdef _NA_008334_20150608_SONNENSCHEIN
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(player);  // 보상옵션 적용
#endif //_NA_008334_20150608_SONNENSCHEIN
        }
    }
    //---------------------------------------------------------------------------------------------
    // state control
    player->SetState(PLAYER_LOADING);
    //---------------------------------------------------------------------------------------------
    // last section
    //  #1. latest information update logic
    //
    GameZone* game_zone = player->GetGameZonePtr();
    GameZone* enter_zone = NULL;
    MAPCODE field_code = 0;  
    WzID area_id = 0;
    WzVector start_pos;
    const eZONE_STATE eRoomState = player->GetGameZone(enter_zone, field_code, &area_id, &start_pos);
    const KEYTYPE room_key = enter_zone->GetKey();
    enter_zone = g_GameZoneManager.GetZone(room_key);
    //
    ;{
        MSG_AG_ZONE_ROOM_JOIN_ACK msg_join_ack;
        if (game_zone && enter_zone)
        {
            player->GetPos(&msg_join_ack.ReturnPos);
            msg_join_ack.ReturnFieldCode = field_code;
            msg_join_ack.ReturnZoneType = game_zone->GetZoneType();
            msg_join_ack.ReturnZoneKey = game_zone->GetKey();
        }
        else
        {
            msg_join_ack.ReturnPos.x     = 0;
            msg_join_ack.ReturnPos.y     = 0;
            msg_join_ack.ReturnPos.z     = 0;
            msg_join_ack.ReturnFieldCode = 0;
            msg_join_ack.ReturnZoneType  = eZONETYPE_MAX;
            msg_join_ack.ReturnZoneKey   = 0;
        }
        player->SendPacket(&msg_join_ack, sizeof(msg_join_ack));
    };

    //
    //---------------------------------------------------------------------------------------------
}

Handler_DG_CHARINFO_IMPL( DG_CHARINFO_QUERY_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHARINFO_QUERY_NAK * pRecvMsg = (MSG_DG_CHARINFO_QUERY_NAK *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	// 캐릭터 정보들 저장
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return;

	SUNLOG( eCRITICAL_LOG,  
		"[DG_CHARINFO_ALL_REQ_NAK][U:%d,ID:%s,E:%u]DG_CHARINFO_ALL_REQ_NAK", 
		pPlayer->GetUserKey(), pPlayer->GetUserID(), pRecvMsg->m_dwErrorCode );

	MSG_AG_ZONE_ROOM_JOIN_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_ITEM_SERIAL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_ITEM_SERIAL_ACK * pRecvMsg = (MSG_DG_ITEM_SERIAL_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	SCSlotContainer *pContainer = pPlayer->GetItemManager()->GetItemSlotContainer(pRecvMsg->m_SlotIdx);

	if( !pContainer->IsEmpty(pRecvMsg->m_ItemPos) )
	{
		SCItemSlot & slot = (SCItemSlot &)pContainer->GetSlot(pRecvMsg->m_ItemPos);
		ASSERT( slot.GetSerial() == TEMP_DBSERIAL_VALUE );
		slot.SetType( pRecvMsg->m_Type );
		slot.SetSerial( pRecvMsg->m_DBSerial );
		pContainer->SetLock(pRecvMsg->m_ItemPos,FALSE);

        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
                pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->AddSerialCode( eSERIAL_MAKE, pRecvMsg->m_SlotIdx, pRecvMsg->m_ItemPos, slot.GetWorldSerial() );
            item_updater_impl->AddExpirationItem( slot );
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
            item_updater_impl->AddImmediatelyUseItem( slot );
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
        }

		if( pRecvMsg->m_SlotIdx == SI_INVENTORY
			|| pRecvMsg->m_SlotIdx == SI_TEMPINVENTORY
			|| pRecvMsg->m_SlotIdx == SI_EQUIPMENT 
			|| pRecvMsg->m_SlotIdx == SI_WAREHOUSE )
		{
			GAMELOG->LogItem( ITEM_MAKE_SERIAL, pPlayer, (SCItemSlot*)&slot, 0, slot.GetNum() );
		}

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        if (pRecvMsg->m_SlotIdx == SI_INVENTORY)
        {
            GameShop::EventProcessor* event_processor = GameShop::Manager::Instance()->FindEventProcessor(pPlayer->GetUserGuid());
            if (event_processor)
            {
                event_processor->Item_Serial_Update(slot);
            }
        }
#endif // _NA_0_20110613_GAME_SHOP_RENEWAL
	}
}
Handler_DG_CHARINFO_IMPL( DG_ITEM_SERIAL_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_ITEM_SERIAL_NAK * pRecvMsg = (MSG_DG_ITEM_SERIAL_NAK *)pMsg;

	//Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	//ASSERT( pPlayer );
	//if( !pPlayer ) return ;

	//SCSlotContainer *pContainer = pPlayer->GetItemManager()->GetItemSlotContainer(pRecvMsg->m_SlotIdx);
	//SCSlot & slot = pContainer->GetSlot(pRecvMsg->m_ItemPos);
	//ASSERT( slot.GetSerial() == TEMP_DBSERIAL_VALUE );
	//pContainer->SetLock( pRecvMsg->m_ItemPos, TRUE );

	//pPlayer->GetItemManager()->AddSerialCode( pRecvMsg->m_SlotIdx, pRecvMsg->m_ItemPos, pRecvMsg->m_DBSerial );

	/*if( pRecvMsg->m_SlotIdx == SI_INVENTORY
		|| pRecvMsg->m_SlotIdx == SI_TEMPINVENTORY
		|| pRecvMsg->m_SlotIdx == SI_EQUIPMENT 
		|| pRecvMsg->m_SlotIdx == SI_WAREHOUSE )
	{
		GAMELOG->LogItem( ITEM_MAKE_SERIAL, pPlayer, (SCItemSlot*)&slot );
	}*/

	SUNLOG( eCRITICAL_LOG, "Error:MSG_DG_ITEM_SERIAL_NAK" );
}

#if SUN_CODE_BACKUP
// CHANGES: f110428.x, removed block, __NA002610_110428_REMOVE_INFINITY_USING_LUCKY
Handler_DG_CHARINFO_IMPL( DG_ITEM_FATE_ITEM_IDENTIFY_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_ITEM_FATE_ITEM_IDENTIFY_ACK* pRecvMsg = (MSG_DG_ITEM_FATE_ITEM_IDENTIFY_ACK*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_ITEM_VALIDATION_OF_REIHALT_NAK NakMsg;
	SCSlotContainer * pContainer = pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );

	SCItemSlot & rTargetItemSlot = (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_ItemPos );
	if( !rTargetItemSlot.GetItemInfo() )
	{
		NakMsg.m_dwErrorCode = RC::RC_ITEM_NOINFO;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	//Fate아이템인가?
	if( !rTargetItemSlot.IsFateItem() )
	{
		NakMsg.m_dwErrorCode = RC::RC_ITEM_NOT_FATE_ITEM;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );		
		rTargetItemSlot.SetLock( FALSE );
		return;
	}

	//이미 감정 했는가?
	if( rTargetItemSlot.GetFateIdentify() )
	{
		NakMsg.m_dwErrorCode = RC::RC_ITEM_ALREADY_IDENTYFIED_FATE_ITEM;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );		
		rTargetItemSlot.SetLock( FALSE );
		return;
	}
	
	//무제한 발급
	if( 0 == pRecvMsg->m_Result )
	{
		if( !rTargetItemSlot.CalFateInfo() )
		{			
			NakMsg.m_dwErrorCode = RC::RC_ITEM_FAILED;
			pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );		
			rTargetItemSlot.SetLock( FALSE );
			return;			
		}
		rTargetItemSlot.SetDateTime( INFINITE_ITEM_DAY * 24 * 60 * 60 );
		rTargetItemSlot.SetFatePeriod( INFINITE_ITEM_DAY );

		GAMELOG->LogItem( ITEM_IDENTIFY_LUCKY_ITEM, pPlayer, &rTargetItemSlot );
	}	
	
	//무제한 발급 초과
	else if( 2 == pRecvMsg->m_Result )
	{
		DWORD FateDateTime = 0;		
		do
		{
			if( !rTargetItemSlot.CalFateDateTime( FateDateTime ) )
			{
				NakMsg.m_dwErrorCode = RC::RC_ITEM_FAILED;
				pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
				rTargetItemSlot.SetLock( FALSE );				
				return;
			}
		}while( FateDateTime == INFINITE_ITEM_DAY );

		if( !rTargetItemSlot.CalFateInfo() )
		{			
			NakMsg.m_dwErrorCode = RC::RC_ITEM_FAILED;
			pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );		
			rTargetItemSlot.SetLock( FALSE );
			return;			
		}
		rTargetItemSlot.SetDateTime( FateDateTime  * 24 * 60 * 60 );
		rTargetItemSlot.SetFatePeriod( FateDateTime );
		GAMELOG->LogItem( ITEM_IDENTIFY_LUCKY_ITEM, pPlayer, &rTargetItemSlot );
	}
	// 시스템 에러 1 == pRecvMsg->m_Result
	else
	{
		NakMsg.m_dwErrorCode = RC::RC_ITEM_FAILED;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );		
		rTargetItemSlot.SetLock( FALSE );
		return;	
	}

	rTargetItemSlot.SetLock( FALSE );
				
	MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK AckMsg;
	AckMsg.m_TargetItemPos = pRecvMsg->m_ItemPos;
	SCItemSlot & rSlot	= (SCItemSlot &)pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot( pRecvMsg->m_ItemPos );
	rSlot.CopyOut(AckMsg.m_TargetItemStream);
	pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ));
}
#endif //SUN_CODE_BACKUP

Handler_DG_CHARINFO_IMPL( DG_ITEM_INVENTORY_FLAG_STREAM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_ITEM_INVENTORY_FLAG_STREAM_CMD* pRecvMsg = (MSG_DG_ITEM_INVENTORY_FLAG_STREAM_CMD*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
	rPROTECT.SetControlFieldStream( pRecvMsg->m_dwFlagStream );
	if( rPROTECT.IsEmptyPassword() && !rPROTECT.IsUsable() )
		rPROTECT.Block( FALSE );
}


/*
Handler_DG_CHARINFO_IMPL( DG_ITEM_WAREHOUSEITEMALL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_ITEM_WAREHOUSEITEMALL_ACK * pRecvMsg = (MSG_DG_ITEM_WAREHOUSEITEMALL_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	ItemManager * pMGR = pPlayer->GetItemManager();
	pPlayer->SerializeWarehouseTotalInfo( pRecvMsg->m_ItemInfo, SERIALIZE_STORE );
	pMGR->SetWarehouseMoney( pRecvMsg->m_Money );

	MSG_CG_ITEM_WAREHOUSEITEMALL_ACK msg;
	msg.m_WarehouseMoney	= pMGR->GetWarehouseMoney();
	pPlayer->SerializeWarehouseTotalInfo( msg.m_ItemInfo, SERIALIZE_LOAD );
	pPlayer->SendPacket( &msg, msg.GetSize() );
}
*/
Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_START_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_START_ACK * pRecvMsg = (MSG_DG_WAREHOUSE_START_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	// 창고 시작 상태로 바꿈
	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
	ASSERT( !pContainer->IsStarted() );
	pContainer->SerializeItemInfo( pRecvMsg->m_ItemInfo, SERIALIZE_STORE );
	pContainer->SerializeMoney( pRecvMsg->m_WarehouseMoney );
	pContainer->StartWarehouse();

	MSG_CG_WAREHOUSE_START_ACK msg;

	ProtectorWarehouse& rPROTECT = pPlayer->GetWarehouseInventoryProtector().WAREHOUSE();
	rPROTECT.SetPasswordFromDB( pRecvMsg->m_WarehousePwd );
	rPROTECT.Initialized();

	INT iFlag = (rPROTECT.IsEmptyPassword() ? MSG_CG_WAREHOUSE_START_ACK::EMPTY_PWD : MSG_CG_WAREHOUSE_START_ACK::EXIST_PWD );

	if( rPROTECT.IsEmptyPassword() )
	{
		rPROTECT.Block( FALSE );
		//msg.m_byWarehouseLock = MSG_CG_WAREHOUSE_START_ACK::UNLOCK;
		iFlag |= (rPROTECT.IsUsable() ? MSG_CG_WAREHOUSE_START_ACK::ENABLE : MSG_CG_WAREHOUSE_START_ACK::DISABLED);
		msg.m_byWarehouseLock = (BYTE)iFlag;
	}
	else
	{
		iFlag |= (rPROTECT.IsUsable() ? MSG_CG_WAREHOUSE_START_ACK::ENABLE : MSG_CG_WAREHOUSE_START_ACK::DISABLED);
		// else의 경우에는 Block이 TRUE, FALSE인 경우가 있을 것이다.
		// 창고를 한두번 여나? ㅋㅋ
		msg.m_byWarehouseLock = (BYTE)iFlag;
		//msg.m_byWarehouseLock = rPROTECT.IsUsable() ?
		//	MSG_CG_WAREHOUSE_START_ACK::UNLOCK : MSG_CG_WAREHOUSE_START_ACK::LOCK;
	}

	msg.m_WarehouseMoney	= pContainer->GetCurMoney();	

	msg.m_Size = sizeof(RENDER_ITEMSTREAMEX)*(MAX_WAREHOUSE_SLOT_NUM);
	BitStream stream( msg.m_BitStreamBuffer, msg.m_Size, TRUE );
	pContainer->Serialize( stream, eSLOT_SERIALIZE_CLIENT_LOAD );
	msg.m_Size = stream.GetNumberOfBytesUsed();

	pPlayer->SendPacket( &msg, msg.GetSize() );

	//창고 정보 로그 남기기
	GAMELOG->WriteWareHouseInfo( pPlayer, TRUE );
	//GAMELOG->LogInvenInfo( SNAPSHOT_WAREHOUSE_OPEN_INVEN, pPlayer );
	GAMELOG->LogSnapShot( pPlayer );
}

Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_FLAG_STREAM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_FLAG_STREAM_CMD* pRecvMsg = (MSG_DG_WAREHOUSE_FLAG_STREAM_CMD*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
	rPROTECTOR.SSN_ENCODED_STREAM().CopyFrom( pRecvMsg->m_EncodedSSN.GetBUFFER() );
	rPROTECTOR.SSN_ENCODED_STREAM().MakeSafeString();
	ProtectorWarehouse& rPROTECT = rPROTECTOR.WAREHOUSE();
	rPROTECT.SetControlFieldStream( pRecvMsg->m_dwFlagStream );
	//memcpy( &pPlayer->GetWarehouseInventoryProtector().CONTROL_FIELD(), &pRecvMsg->m_dwFlagStream, sizeof(DWORD) );
	// 아래의 경우는 있을 수 없는 상황이다. 하지만 최초 접속자에 한해서 발생할 수 있다.
	// 창고 비번은 창고를 불러들여야만 가능하기 때문... 차라리 최초 접속상황이 아닌 경우를 처리 할 수 있으면
	if( rPROTECT.IsInitialized() )
	{
		if( rPROTECT.IsEmptyPassword() && !rPROTECT.IsUsable() )
			rPROTECT.Block( FALSE );
	}
}

Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_PWD_SETTING_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
	rPROTECTOR.OnRecvDG_WAREHOUSE_PWD_SETTING_ACK( (MSG_DG_WAREHOUSE_PWD_SETTING_ACK*)pMsg, wSize );
}


Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_START_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_START_NAK * pRecvMsg = (MSG_DG_WAREHOUSE_START_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	SUNLOG( eCRITICAL_LOG, "[%s:%d]창고열기에러", pPlayer->GetCharName(), pRecvMsg->m_dwErrorCode );

	MSG_CG_WAREHOUSE_START_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );

	ProtectorWarehouse& rPROTECT = pPlayer->GetWarehouseInventoryProtector().WAREHOUSE();
	rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
}

Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_END_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_END_ACK * pRecvMsg = (MSG_DG_WAREHOUSE_END_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();


	MSG_CG_WAREHOUSE_END_ACK msg;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_WAREHOUSE_END_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_END_NAK * pRecvMsg = (MSG_DG_WAREHOUSE_END_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
		
	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
	// 시작상태로 되돌림
	pContainer->SetStarted(true);

	MSG_CG_WAREHOUSE_END_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_SELECT_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_SELECT_ACK * pRecvMsg = (MSG_DG_EVENT_SELECT_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	pPlayer->GetEventSlotContainer()->OnSelect( TRUE, &pRecvMsg->m_TotalInfo );

	MSG_CG_EVENT_SELECT_ACK msg;
	pPlayer->GetEventSlotContainer()->Serialize( msg.m_TotalInfo, SERIALIZE_LOAD );
	pPlayer->SendPacket( &msg, msg.GetSize() );

}
Handler_DG_CHARINFO_IMPL( DG_EVENT_SELECT_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_SELECT_NAK * pRecvMsg = (MSG_DG_EVENT_SELECT_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	pPlayer->GetEventSlotContainer()->OnSelect( FALSE, NULL );

	MSG_CG_EVENT_SELECT_NAK msg;
	msg.m_byErrorCode = RC::RC_ITEM_NOTEXIST_ITEM;	//pRecvMsg->m_byErrorCode;//< 아이템이 없을 경우만 로직적으로 nak가 오게되어있음
	pPlayer->SendPacket( &msg, sizeof(msg) );

}

Handler_DG_CHARINFO_IMPL( DG_EVENT_UPDATE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_UPDATE_ACK * pRecvMsg = (MSG_DG_EVENT_UPDATE_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;


	MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_ACK msg;
	pPlayer->GetEventSlotContainer()->OnMoveItemToInventory( TRUE, pRecvMsg->m_ItemPos, &msg.m_TotalInfo );

	pPlayer->SendPacket( &msg, msg.GetSize() );

}
Handler_DG_CHARINFO_IMPL( DG_EVENT_UPDATE_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_UPDATE_NAK * pRecvMsg = (MSG_DG_EVENT_UPDATE_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
    MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_ACK msg_dummy; // TODO: need modify
	pPlayer->GetEventSlotContainer()->OnMoveItemToInventory( FALSE, 0, &msg_dummy.m_TotalInfo);

	MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_NAK msg;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

// 이벤트 응모 현황(?) 수신
Handler_DG_CHARINFO_IMPL( DG_EVENT_ITEM_STATUS_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_ITEM_STATUS_ACK* pRecvMsg = ( MSG_DG_EVENT_ITEM_STATUS_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
//	ASSERT( pPlayer );
	if( !pPlayer ) return ;

#ifdef _WHATTHEHELL_SUNEVENT_20060727
	EVENT_INFO *pEventInfo = NULL;

	for( WORD i = 0; i < pRecvMsg->m_wEventNum && i < _MAX_EVENT_ROW_NUM ; i++ )
	{
		pEventInfo = SunEventInfoParser::Instance()->FindEventInfo( pRecvMsg->m_EventStatus[i].m_dwEventID );

		// 현재 유효한 이벤트만 등록(지나간 이벤트 내용은 저장하지 않는다)
		if( pEventInfo )
		{
			pPlayer->AddEventStatus( pRecvMsg->m_EventStatus[i] );
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "Passed Event Status[%u][CharGuid:%u]", pRecvMsg->m_EventStatus[i].m_dwEventID, pPlayer->GetCharGuid() );
		}
	}
#else
		const DWORD eventNum = pRecvMsg->m_wEventNum;
		IPlayerEventD& rEventD = pPlayer->PlayerEvent();
		for( DWORD idx=0 ; idx<eventNum && idx<_MAX_EVENT_ROW_NUM ; ++idx )
		{
			EVENT_STATUS_EX& rInfo = pRecvMsg->m_EventStatus[idx];
			rEventD.AddEventStatus( rInfo );
		}
#endif


//	MessageOut( eFULL_LOG, "Event Status Check[%u][C:%u][Event:%u]", pRecvMsg->m_dwKey, pPlayer->GetCharGuid(), pRecvMsg->m_wEventNum );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_ITEM_STATUS_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_ITEM_STATUS_NAK* pRecvMsg = (MSG_DG_EVENT_ITEM_STATUS_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	//ASSERT( pPlayer );
	if( !pPlayer )
	{
		MessageOut( eFULL_LOG, "Event Status Check Failed[%u]", pRecvMsg->m_dwKey );
	   return ;
	}
/*
	MSG_CG_EVENT_ITEM_STATUS_NAK msg;
	msg.m_dwKey = pPlayer->GetUserGuid();
	msg.m_dwErrorCode = 0;				// 일단 임시로 0에 맞춘다

	pPlayer->SendPacket( &msg, sizeof(msg) );
*/
//	MessageOut( eFULL_LOG, "Event Status Check Failed[%u][C:%u]", pRecvMsg->m_dwKey, pPlayer->GetCharGuid() );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_ITEM_REG_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_ITEM_REG_ACK* pRecvMsg = (MSG_DG_EVENT_ITEM_REG_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_EVENT_ITEM_REG_ACK msg;

	msg.m_dwKey = pPlayer->GetUserGuid();
	msg.m_dwEventID = pRecvMsg->m_dwEventID;

	pPlayer->SendPacket( &msg, sizeof(msg) );
	MessageOut( eFULL_LOG, "Event_Reg Success[%u][C:%u][Event:%u]", pRecvMsg->m_dwKey, pPlayer->GetCharGuid(), pRecvMsg->m_dwEventID );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_ITEM_REG_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_ITEM_REG_NAK* pRecvMsg = (MSG_DG_EVENT_ITEM_REG_NAK *)pMsg;
	MSG_CG_EVENT_ITEM_REG_NAK msg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	msg.m_dwErrorCode = 0;				// 일단 임시로 0에 맞춘다

	// 삭제된 아이템 다시 복구해 줘야 된다!!!
	EVENT_INFO *			pEventInfo		= SunEventInfoParser::Instance()->FindEventInfo( pRecvMsg->m_dwEventID );

	if( pEventInfo )
	{
		SLOTCODE delItem[_MAX_EVENT_REGITEM]={0,};
		POSTYPE delpos[_MAX_EVENT_REGITEM]={0,};

        int i = 0;
		for( ; i<_MAX_EVENT_REGITEM; i++ )
		{
			delItem[i] = (SLOTCODE)pEventInfo->m_dwRegisterItemIndex[i];
			delpos[i] = (POSTYPE)pEventInfo->m_wRegisterItemNum[i];

			if( !delItem[i] || !delpos[i] )
			{
				break;
			}
		}

		pPlayer->GetItemManager()->ObtainAllKindOfItem( i, delItem, delpos, &msg.m_TotalInfo );
		// 게임 로그 : 복구 처리 로그
		GAMELOG->LogMultiItem( ITEM_EVENT_ETHER_REG_FAILED, pPlayer, i, delItem, delpos );
	}
	else
	{
		msg.m_TotalInfo.m_InvenCount = 0;
		msg.m_TotalInfo.m_TmpInvenCount = 0;
	}
	pPlayer->SendPacket( &msg, msg.GetSize() );

	MessageOut( eFULL_LOG, "Event_Reg Failed[%u][C:%u][Event:%u]", pRecvMsg->m_dwKey, pPlayer->GetCharGuid() );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_USETIME_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_USETIME_CMD* pRecvMsg = (MSG_DG_EVENT_USETIME_CMD*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_EVENT_TIME_CMD	cmdMsg;

	cmdMsg.m_EventUseTime = pRecvMsg->m_EventUseTime;

	pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_QUERY_EXIST_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_QUERY_EXIST_ACK* pRecvMsg = (MSG_DG_EVENT_QUERY_EXIST_ACK *)pMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )
		return ;

	pPlayer->GetEventSlotContainer()->OnSelect( FALSE, NULL );

	MSG_CG_EVENT_QUERY_EXIST_ACK msgACK;
	msgACK.m_bExist = pRecvMsg->m_bExist;
	pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
}


Handler_DG_CHARINFO_IMPL(DG_EVENT_SSQ_REWARD_EXIST_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DG_EVENT_SSQ_REWARD_EXIST_CMD* recv_msg =
        static_cast<MSG_DG_EVENT_SSQ_REWARD_EXIST_CMD*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL || player->GetField() == NULL) {
        return;
    }
    //  (Work To Do)
    //  1. Player 입장시 미션 보상 처럼 처리할 수 있도록 하자.
    ISSQHandlerInGame& ssq_handler = player->SSQHandler;
    const BOOLEAN is_valid_state = ssq_handler.IsValidState();
    if (is_valid_state == false) {
        ssq_handler.Init();
    }
    ssq_handler->SetRewardInfo(recv_msg->m_RewardInfo);
    // 나중에 유저가 입장시 패킷을 전송한다.
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    // (CHANGES) (f100714.2L) pre-fetch to support a delayed event query process
    // Old comments -
    //  (WAVERIX) (090813) (CHANGES) 클라이언트 패킷 수신에 의한 것이 아니라 정확한 Scene동기화
    //  처리를 확신할 수 없는 상황이 존재한다. 'CG_SHOP_IS_RECV_GIFT_SYN'요청시 처리하도록 변경해 봄.
    //  reference : CG_SHOP_IS_RECV_GIFT_SYN handler
    bool is_acceptable_status = false;
    ;{
        GameZone* enter_zone = 0;
        MAPCODE   field_code = 0;  
        WzID      area_id = 0;
        WzVector  start_pos;

        eZONE_STATE zone_state = player->GetGameZone(enter_zone, field_code, &area_id, &start_pos);
        if (ePRS_AT_ZONE & zone_state) {
            is_acceptable_status = true;
        }
    };
    //
    if (is_acceptable_status == false) {
        return;
    }
    //
    if (FIELD_SERVER == g_pGameServer->GetServerType()) {
        ssq_handler->Send_RewardProcess(player, true);
    }
}


Handler_DG_CHARINFO_IMPL( DG_EVENT_SSQ_REWARD_UPDATE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_EVENT_SSQ_REWARD_UPDATE_ACK* const pRecvMsg = static_cast<MSG_DG_EVENT_SSQ_REWARD_UPDATE_ACK*>( pMsg );

	Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )
		return ;

	//	(Work To Do)
	//	1. 보상 처리할 것, 그 결과 클라로 전송할 것.
	//  2. 보상 트랜젝션 해제 처리할 것.
	//	(080917)
	//	- 보상 처리는 통보식이며, 응답후 처리는 하지 않는다.
	//	이유? 성공후 바로 떠나도록 유도할 예정이니까...
#if WAVERIX_CODE_BACKUP
	ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
	if( !rHandler.IsValidState() )
	{
		ASSERT( !"transaction이 꼬이는 현상인가? 확인이 필요하다." );
		return;
	}

	const BOOLEAN isUpdated = pRecvMsg->m_UpdateResult;
	if( !isUpdated )
	{
		ASSERT( !"어지간해서는 여기에 도달하지 않기를 바라는 마음뿐..." );
		return;
	}

	//SSQ_RESERVED_REWARD_INFO info;
	//if( !rHandler->GetRewardInfo( info ) )
	//	return;

	//rHandler->Rewarded();
#endif
}

#ifdef _NA_008012_20150130_RANKING_SYSTEM
Handler_DG_CHARINFO_IMPL( DG_MISSION_RECORD_SELECT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_MISSION_RECORD_SELECT_CMD* pRecvMsg = (MSG_DG_MISSION_RECORD_SELECT_CMD *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	// 초기화, 기록 저장
	MISSION_RECORD_INFO * pRecordInfo = pRecvMsg->m_MissionRecord;
	for( BYTE i = 0 ; i < MAX_MISSION_NUM+1 ; ++i )
	{
		if( i < pRecvMsg->m_byCount && pRecordInfo[i].m_byMissionNo != 0 )
			pPlayer->SetBestMissionTime( pRecordInfo[i].m_byMissionNo, pRecordInfo[i].m_iClearTime );
		else
			pPlayer->SetBestMissionTime( i, INT_MAX );
	}

}
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
Handler_DG_CHARINFO_IMPL( DG_OTHER_EXIST_CHARNAME_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_OTHER_EXIST_CHARNAME_ACK * pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_ACK *)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if( !pPlayer ) return ;

    USERGUID user_guid = pPlayer->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
        proc->Receive_CharacterNameCheck(pRecvMsg->m_UserGuid, 
                                         pRecvMsg->m_CharGuid, 
                                         pRecvMsg->character_class_, 
                                         pRecvMsg->character_level_, 
                                         pRecvMsg->m_tszCharName, "");
    else
        GameShop::EventProcessor::SendFail_CharacterNameCheck(pPlayer, RC::RC_SHOP_FAILED);
}

Handler_DG_CHARINFO_IMPL( DG_OTHER_EXIST_CHARNAME_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_OTHER_EXIST_CHARNAME_NAK * pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_NAK *)pMsg;
    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if( !pPlayer ) return ;

    GameShop::EventProcessor::SendFail_CharacterNameCheck(pPlayer, RC::RC_SHOP_NOT_EXIST_CHARACTER_NAME);
}

#else

#if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)

Handler_DG_CHARINFO_IMPL( DG_OTHER_EXIST_CHARNAME_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_OTHER_EXIST_CHARNAME_ACK * pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	switch( pRecvMsg->m_Type )
	{
	case eCHARNAME_ACTION_SHOP_SEND_GIFT:
		{
			RC::eSHOP_RESULT rt = g_GameShopManager.OnSendGiftNDoRequest(TRUE, pPlayer, pRecvMsg->m_UserGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_tszCharName);
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_SEND_GIFT_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}
		}break;
	}
}

Handler_DG_CHARINFO_IMPL( DG_OTHER_EXIST_CHARNAME_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_OTHER_EXIST_CHARNAME_NAK * pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	switch( pRecvMsg->m_Type )
	{
	case eCHARNAME_ACTION_SHOP_SEND_GIFT:
		{
			RC::eSHOP_RESULT rt = g_GameShopManager.OnSendGiftNDoRequest(FALSE, pPlayer, 0, 0, NULL);
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_SEND_GIFT_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}
		}break;
	}
}
#endif //!__NA000000_SHOP_USERID_50_PASSING_PROC__
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL


Handler_DG_CHARINFO_IMPL( DG_OTHER_SHOP_RECV_GIFT_NTF )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_OTHER_SHOP_RECV_GIFT_NTF * pRecvMsg = (MSG_DG_OTHER_SHOP_RECV_GIFT_NTF *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    MSG_CG_SHOP_EXIST_GIFT_NTF ntf_message;
    pPlayer->SendPacket(&ntf_message, sizeof(ntf_message));
#else
	MSG_CG_SHOP_SEND_GIFT_NTF ntf;
	pPlayer->SendPacket( &ntf, sizeof(ntf) );
#endif
}

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE

// DB에서 피로도 정보를 준다. 이 정보는 존을 이동할 때 마다 날라올 것이다
Handler_DG_CHARINFO_IMPL( DG_OTHER_FATIGUE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_OTHER_FATIGUE_CMD*	pRecvMsg = (MSG_DG_OTHER_FATIGUE_CMD*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	//1. 플레이어의 피로도 시간을 세팅한다.
	pPlayer->SetPlayTime( pRecvMsg->m_wPlayTime );
	pPlayer->SetOfflineTime( pRecvMsg->m_wOfflineTime );

	//2. 피로도 정보 초기화
	pPlayer->GetFatigueManager()->Init( pPlayer );

	//2. 클라이어트에 현재 피로도 정보를 보내준다.
	MSG_CG_CHARINFO_FATIGUE_CMD	cmd;
	cmd.m_wPlayTime = pRecvMsg->m_wPlayTime;
	cmd.m_wOfflineTime = pRecvMsg->m_wOfflineTime;

	if( FatigueOptionParser::Instance()->IsUse() == TRUE )
	{
		if( (pPlayer->GetAge() >= 18) && (pPlayer->GetRealNameAuth() == 1) )	
			cmd.m_byType = 0;
		else
			cmd.m_byType = 1;
	}
	else
		cmd.m_byType = 0;

	pPlayer->SendPacket( &cmd, sizeof(cmd) );
}

#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

Handler_DG_CHARINFO_IMPL( DG_WAYPOINT_INFO_LOAD_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAYPOINT_INFO_LOAD_ACK * pRecvMsg = (MSG_DG_WAYPOINT_INFO_LOAD_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	pPlayer->GetWayPoint()->Serialize( pRecvMsg->m_Info, SERIALIZE_STORE );

	MSG_CG_WAYPOINT_INFO_LOAD_ACK msg;
	pPlayer->GetWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
	pPlayer->SendPacket( &msg, msg.GetSize() );
}

Handler_DG_CHARINFO_IMPL( DG_WAYPOINT_INFO_LOAD_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAYPOINT_INFO_LOAD_NAK * pRecvMsg = (MSG_DG_WAYPOINT_INFO_LOAD_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_WAYPOINT_INFO_LOAD_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );

}

Handler_DG_CHARINFO_IMPL( DG_WAYPOINT_INFO_SAVE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAYPOINT_INFO_SAVE_ACK * pRecvMsg = (MSG_DG_WAYPOINT_INFO_SAVE_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	BOOL rt = pPlayer->GetWayPoint()->Insert( pRecvMsg->m_MapCode );
	ASSERT( rt && "WAYPOINT에 중복등록할수없다." );

	MSG_CG_WAYPOINT_INFO_SAVE_ACK msg;
//	msg.m_WPIndex = pRecvMsg->m_WPIndex;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_WAYPOINT_INFO_SAVE_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAYPOINT_INFO_SAVE_NAK * pRecvMsg = (MSG_DG_WAYPOINT_INFO_SAVE_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_WAYPOINT_INFO_SAVE_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}


Handler_DG_CHARINFO_IMPL( DG_CHAR_WAYPOINT_INFO_LOAD_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_LOAD_ACK * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_LOAD_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	pPlayer->GetCharWayPoint()->Serialize( pRecvMsg->m_Info, SERIALIZE_STORE );

	MSG_CG_CHAR_WAYPOINT_INFO_LOAD_ACK msg;
	pPlayer->GetCharWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
	pPlayer->SendPacket( &msg, msg.GetSize() );
}

Handler_DG_CHARINFO_IMPL( DG_CHAR_WAYPOINT_INFO_SAVE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_SAVE_ACK * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_SAVE_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	BOOL rt = pPlayer->GetCharWayPoint()->Insert( pRecvMsg->m_Info );
	ASSERT( rt && "CharWAYPOINT에 중복등록할수없다." );

	MSG_CG_CHAR_WAYPOINT_INFO_SAVE_ACK msg;
	msg.m_Info = pRecvMsg->m_Info;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_CHAR_WAYPOINT_INFO_DEL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_DEL_ACK * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_DEL_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	BOOL rt = pPlayer->GetCharWayPoint()->Delete( pRecvMsg->m_IdxWayPoint );
	ASSERT( rt && "WAYPOINT에 가 존재하지 않는다." );


	//item 삭제시간 다시셋팅
	ItemManager* pItemManager = pPlayer->GetItemManager();
	if(!pItemManager)	
		return;

	SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
	if(!pContainer)	
		return;

	for(int pos = 0; pos < MAX_INVENTORY_SLOT_NUM; ++pos)
	{
		if( pContainer->IsEmpty(pos) )
			continue;

		SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

		if( rItemSlot.GetSerial() != 0 && 
			rItemSlot.IsWayPointItem() &&
			rItemSlot.IsProgressTime() )
		{
			rItemSlot.ResetWayPointDelete();
			pPlayer->GetItemManager()->SendItemChange(rItemSlot);

			break;
		}
	}

	// 삭제중 아이템이 사라졌더라도 NAK아님 (DB기준)
	MSG_CG_CHAR_WAYPOINT_INFO_DEL_ACK msg;
	msg.m_IdxWayPoint = pRecvMsg->m_IdxWayPoint;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

Handler_DG_CHARINFO_IMPL( DG_CHAR_WAYPOINT_INFO_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_NAK * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_CHAR_WAYPOINT_INFO_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
}

#ifdef _NA_008012_20150130_RANKING_SYSTEM
Handler_DG_CHARINFO_IMPL( DG_MISSION_RANKING_POINT_REQ_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_MISSION_RANKING_POINT_REQ_ACK * pRecvMsg = (MSG_DG_MISSION_RANKING_POINT_REQ_ACK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->Point.UserGUID );
	ASSERT( pPlayer);
	if(!pPlayer) return;

	if( !(pPlayer->GetRankingSystemCtl()) )
	{
		ASSERT(FALSE);
		SUNLOG(eCRITICAL_LOG, "[SetPointRanking] Player's RankingSystemCtl is NULL[%u]", pRecvMsg->Point.UserGUID );
		return;
	}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    pPlayer->GetRankingSystemCtl()->SetPlayerPoint( pRecvMsg->Point.UserGUID, pRecvMsg->Point.Point, 
                                                    pRecvMsg->Point.DBKey, pRecvMsg->Point.ranking_type );
#else
    pPlayer->GetRankingSystemCtl()->SetPlayerPoint( pRecvMsg->Point.UserGUID, pRecvMsg->Point.Point, pRecvMsg->Point.DBKey );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}
#endif // _NA_008012_20150130_RANKING_SYSTEM

Handler_DG_CHARINFO_IMPL( DG_EVENT_STATUS_UPDATE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	const MSG_DG_EVENT_STATUS_UPDATE_ACK* pRecvMsg = (const MSG_DG_EVENT_STATUS_UPDATE_ACK*)pMsg;
	// 트랜잭션의 상태를 확인할 필요 있다.
	//ASSERT( !"Not Implementation - check transaction state" );
}

Handler_DG_CHARINFO_IMPL(DG_GUILD_WAREHOUSE_CLOSE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_CLOSE_ACK, pRecvMsg, pMsg);
	CGuildWarehouseSlotContainer* pContainer = g_GameGuildManager.GetGuildWarehouse(pRecvMsg->m_GuildGuid);
	if(pContainer) 
	{
        // 중국 복사 버그 수정 (유저 창고의 방식 사용) [10/12/2009 lst1024]
		if (pContainer->IsOpened())
            pContainer->CloseWarehouse();
	}
	else
	{
		SUNLOG(eCRITICAL_LOG, "DG_GUILD_WAREHOUSE_CLOSE_ACK NONE GUILDWAREHOUSE(%d)(%u)", pRecvMsg->m_GuildGuid, pRecvMsg->m_dwKey);
	}
}

Handler_DG_CHARINFO_IMPL(DG_GUILD_WAREHOUSE_CLOSE_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_CLOSE_NAK, pRecvMsg, pMsg);
	CGuildWarehouseSlotContainer* pContainer = g_GameGuildManager.GetGuildWarehouse(pRecvMsg->m_GuildGuid);
	if(pContainer) 
	{
		if(!pContainer->IsOpened()) 
		{
			SUNLOG(eCRITICAL_LOG, "DG_GUILD_WAREHOUSE_CLOSE_NAK ERROR(%d)(%u)", pRecvMsg->m_GuildGuid, pRecvMsg->m_dwKey);
			return;
		}

		pContainer->CloseWarehouse();
	}
	else
	{
		SUNLOG(eCRITICAL_LOG, "DG_GUILD_WAREHOUSE_CLOSE_ACK NONE GUILDWAREHOUSE(%d)(%u)", pRecvMsg->m_GuildGuid, pRecvMsg->m_dwKey);
	}
}

Handler_DG_CHARINFO_IMPL(DG_GUILD_WAREHOUSE_TAPEX_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_TAPEX_ACK, pRecvMsg, pMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	CGuildWarehouseSlotContainer* pGuildWareHouse = g_GameGuildManager.GetGuildWarehouse(pRecvMsg->m_GuildGuid);
	if(pGuildWareHouse) 
	{
        // 길드창고 확장재 아이템 사용 로그
        const ItemManager* const item_manager = pPlayer->GetItemManager();
        SCSlotContainer* const slot_container = \
            static_cast<SCSlotContainer*>( item_manager->GetItemSlotContainer(SI_INVENTORY) );
        const SCItemSlot& item_slot = \
            static_cast<const SCItemSlot&>(slot_container->GetSlot(pRecvMsg->m_SlotPos));

        GAMELOG->LogItem(ITEM_USE, pPlayer, &item_slot); 

		pPlayer->GetItemManager()->DeleteWastedItem(pRecvMsg->m_SlotIndex, pRecvMsg->m_SlotPos);
		pGuildWareHouse->SetSlotTabNum(pRecvMsg->m_TabCount);
		
		// 클라에서 이 패킷을 처리 안하기 때문에 없앰
		//MSG_CG_GUILD_WAREHOUSE_TAPEX_ACK msg;
		//msg.m_SlotTabNum = pGuildWareHouse->GetSlotTabNum();
		//pPlayer->SendPacket(&msg, sizeof(msg));
	}
}

Handler_DG_CHARINFO_IMPL(DG_GUILD_WAREHOUSE_TAPEX_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_TAPEX_NAK, pRecvMsg, pMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	// 클라에서 이 패킷을 처리 안하기 때문에 없앰
	//MSG_CG_GUILD_WAREHOUSE_TAPEX_NAK msg;
	//msg.m_bErrorCode = pRecvMsg->m_bErrorCode;
	//pPlayer->SendPacket(&msg, sizeof(msg));
}

//{__NA001283_20090227_COLLECTION_LIST
Handler_DG_CHARINFO_IMPL( DG_COLLECT_LIST_LOAD_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_COLLECT_LIST_LOAD_ACK * pRecvMsg = (MSG_DG_COLLECT_LIST_LOAD_ACK*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	CollectedList* pCollectedList = pPlayer->GetCollectedList();
	if( NULL == pCollectedList )
	{
		SUNLOG(eCRITICAL_LOG, "DG_COLLECT_LIST_LOAD_ACK ERROR(%d)(%u) 채집 리스트가 존재 하지 않는다.", pPlayer->GetCharGuid(), pRecvMsg->m_dwKey);
		return;
	}
	pCollectedList->SerializeStream( pRecvMsg->m_Info, SERIALIZE_STORE );
	pCollectedList->SetIsLoad(true);		
}

Handler_DG_CHARINFO_IMPL( DG_COLLECT_ADD_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_COLLECT_ADD_ACK* pRecvMsg = (MSG_DG_COLLECT_ADD_ACK*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	CollectedList* pCollectedList = pPlayer->GetCollectedList();
	if( NULL == pCollectedList )
	{
		SUNLOG(eCRITICAL_LOG, "MSG_DG_COLLECT_ADD_ACK ERROR(%d)(%u) 채집 리스트가 존재 하지 않는다.",
                                pPlayer->GetCharGuid(), pRecvMsg->m_dwKey);
		return;
	}

	if( false == pCollectedList->IsLoaded() )
	{
		SUNLOG(eCRITICAL_LOG, "MSG_DG_COLLECT_ADD_ACK ERROR(%d)(%u) 아직 채집 리스트를 DB에서 로드 하지 않았다.",
                                pPlayer->GetCharGuid(), pRecvMsg->m_dwKey);
		return;
	}

    if( pRecvMsg->m_wCount != pCollectedList->GetCount(pRecvMsg->m_CollectionID) )
    {
        SUNLOG(eCRITICAL_LOG, "MSG_DG_COLLECT_ADD_ACK ERROR(%d)(%u) 채집 횟수가 틀리다. DBA[%u] GAME[%u]",
                                pPlayer->GetCharGuid(), pRecvMsg->m_dwKey, pRecvMsg->m_wCount, 
                                pCollectedList->GetCount(pRecvMsg->m_CollectionID));
        return;
    }
}


Handler_DG_CHARINFO_IMPL( DG_COLLECT_DEL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_COLLECT_DEL_ACK* pRecvMsg = (MSG_DG_COLLECT_DEL_ACK*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	CollectedList* pCollectedList = pPlayer->GetCollectedList();
	if( NULL == pCollectedList )
	{
		SUNLOG(eCRITICAL_LOG, "DG_COLLECT_DEL_ACK ERROR(%d)(%u) 채집 리스트가 존재 하지 않는다.", pPlayer->GetCharGuid(), pRecvMsg->m_dwKey);
		return;
	}

	if( false == pCollectedList->IsLoaded() )
	{
		SUNLOG(eCRITICAL_LOG, "DG_COLLECT_DEL_ACK ERROR(%d)(%u) 아직 채집 리스트를 DB에서 로드 하지 않았다.", pPlayer->GetCharGuid(), pRecvMsg->m_dwKey);
		return;
	}

	pPlayer->GetCollectedList()->Delete( pRecvMsg->m_CollectionID );
}

Handler_DG_CHARINFO_IMPL( DG_COLLECT_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_COLLECT_NAK* pRecvMsg = (MSG_DG_COLLECT_NAK *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	SUNLOG( eCRITICAL_LOG, "[DG_COLLECT_NAK] [%s], ErrorCode[%d]", pPlayer->GetCharName(), pRecvMsg->m_dwErrorCode );
}
//}__NA001283_20090227_COLLECTION_LIST

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM
Handler_DG_CHARINFO_IMPL( DG_OTHER_HEIM_LOTTO_PICK_NTF )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_OTHER_HEIM_LOTTO_PICK_NTF* pRecvMsg = (MSG_DG_OTHER_HEIM_LOTTO_PICK_NTF*)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer || pPlayer->GetCharGuid() != pRecvMsg->iCharGuid ) { 
        return;
    }

    TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1];
    _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "%d(%d): %d-%d-%d-%d", 
        pRecvMsg->wDrawNum, pRecvMsg->bIsAuto, 
        pRecvMsg->bPickedNum[0], pRecvMsg->bPickedNum[1], pRecvMsg->bPickedNum[2], pRecvMsg->bPickedNum[3]
        );
    tszAddInfo[MAX_ADDINFO_SIZE] = '\0';
    GAMELOG->LogSimpleAction(ACT_HEIM_LOTTO_PICK, pPlayer, tszAddInfo);
}

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
Handler_DG_CHARINFO_IMPL( DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF* pRecvMsg = (MSG_DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF*)pMsg;
    AccumulatePoint::SyncMatchDate(pRecvMsg->wYear, pRecvMsg->wDayOfWeek, pRecvMsg->dwSecond);
}
#endif

Handler_DG_CHARINFO_IMPL(DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    __UNUSED(pServerSession);
    ComposeOrDecomposeImplement implement;
    implement.HandleGetHistoriesResponseFromDBP(pMsg, wSize);
}

Handler_DG_CHARINFO_IMPL(DG_OTHER_EXTEND_CHAR_SLOT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    __UNUSED(pServerSession);

    using namespace util::internal;

    const MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK* dbp_ack(NULL);
    Player* requester(NULL);
    if (CheckRequestMessage(pMsg, wSize, dbp_ack, requester) == false)
    {
        return;
    }

    int result_code = 0;
    if (CheckRequesterState(requester, __FUNCTION__, result_code) == false)
    {
        return;
    }

    if (dbp_ack->result_code != MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK::kSuccess)
    {
        MSG_CG_ITEM_USE_NAK msg;
        msg.m_ErrorCode = RC::RC_ITEM_UNUSABLE_FUNCTION;
        requester->SendPacket(&msg, sizeof(msg));
        return;
    }

    requester->GetItemManager()->DeleteWastedItem(SI_INVENTORY, dbp_ack->extend_item_position);
}

//_KR_0_20091021_FIRENDINVITATION_EXTEND
Handler_DG_CHARINFO_IMPL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK )
{
    GFriendInvitationController::Instance()->OnRewardItemUpdateACK(pServerSession, pMsg, wSize);
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK )
{
    GFriendInvitationController::Instance()->OnRewardItemUpdateNAK(pServerSession, pMsg, wSize);
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK )
{
    GFriendInvitationController::Instance()->OnRecallInfoSelectACK(pServerSession, pMsg, wSize);
}

Handler_DG_CHARINFO_IMPL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK )
{
    GFriendInvitationController::Instance()->OnRecallInfoSelectNAK(pServerSession, pMsg, wSize);
}

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
Handler_DG_CHARINFO_IMPL( DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK* recv_msg = 
        static_cast<MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->request_char_guid_);
    if (player == NULL)
    {
        assert(!"Player Not Fount");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%u]Player Not Found", 
            recv_msg->request_char_guid_);
        return;
    }
    //--gamelulu debuging log
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Reward Start[Name:%s][Cost:%%I64u]",
           player->GetCharName(),
           recv_msg->tender_cost_);

    //-- 보상 조건 체크
    if (recv_msg->result_value_ != 0)
    {
        assert(!"DB Query Result Fail");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]DB Query Result Fail", 
            recv_msg->request_char_guid_);
        return;
    }
    else if (recv_msg->request_char_guid_ != recv_msg->db_result_char_guid_)
    {
        assert(!"CharGuid Not Same");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]CharGuid Not Same[DB:%u]!=[REQ:%u]", 
            recv_msg->db_result_char_guid_, 
            recv_msg->request_char_guid_);
        return;
    }
    else if (recv_msg->request_reward_type_ != recv_msg->db_result_reward_type_)
    {
        assert(!"Money Type Not Same");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]Money Type Not Same[DB:%d]!=[REQ:%d]", 
            recv_msg->request_char_guid_, 
            recv_msg->db_result_reward_type_, 
            recv_msg->request_reward_type_);
        return;
    }
    else if (recv_msg->request_map_code_ != recv_msg->db_result_map_code_)
    {
        assert(!"MapCode Not Same");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]MapCode Not Same[DB:%u]!=[REQ:%u]",
            recv_msg->request_char_guid_, 
            recv_msg->db_result_map_code_, 
            recv_msg->request_map_code_);
        return;
    }
    else
    {
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        //gamelulu say : 보상을 받을때 중복갯수의 최대치 때문에 여러게의 슬롯을 사용해야 한다.
        //현재 최대치 이상이 삽입 가능한지가 체크 되지 않기 때문에 최대치까지만 보상을 하며
        //보상의 횟수를 늘린다. 다만 중간에 실패 했을 경우 DB에 그 내용을 저장하여 차후 다시
        //보상을 시도하게 한다.
        MONEY amount_heim = 
            DominationInfoParser::Instance()->GetAreaLimitMoney(recv_msg->request_map_code_);
        //--gamelulu debuging log
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Script Heim[Name:%s][Heim:%I64u]",
               player->GetCharName(), amount_heim);

        SLOTCODE item_code = 
            DominationInfoParser::Instance()->GetBattingItemCodeByMapCode(recv_msg->request_map_code_);

        //--보상 분할 처리
        int temp_cost = static_cast<int>(recv_msg->tender_cost_);//기존 Money패킷을 그대로 쓰기 때문
        int number_of_insertable_item;//한개의 슬롯을 대상으로 습득 할 수 있는 아이템 갯수
        for(;temp_cost > 0 ; temp_cost -= number_of_insertable_item)
        {
            //--습득 가능 여부 & 습득 할 수 있는 아이템 갯수(OUT)
            RC::eITEM_RESULT result_value = 
                player->GetPostManager()->CheckRewardItem(player, temp_cost, item_code, 
                                                          number_of_insertable_item);

            if (result_value != RC::RC_ITEM_SUCCESS)
            {
                MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD nak_msg;
                nak_msg.memo_guid_ = recv_msg->memo_guid_;
                nak_msg.result_value_ = false;

                player->SendPacket(&nak_msg, sizeof(nak_msg));

                //--보상 DB 롤백
                MSG_DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN send_msg;
                send_msg.char_guid_ = recv_msg->db_result_char_guid_;
                send_msg.map_code_ = recv_msg->db_result_map_code_;
                send_msg.reward_type_ = recv_msg->db_result_reward_type_;
                send_msg.tender_cost_ = temp_cost;

                player->SendToGameDBPServer(&send_msg, sizeof(send_msg));

                SUNLOG(eFULL_LOG, "["__FUNCTION__"]Inventory Is Full:%u]", 
                    recv_msg->request_char_guid_);

                return;
            }

            //--아이템 보상
            RC::eITEM_RESULT reward_result_value = 
                player->GetPostManager()->RewardItem(player, number_of_insertable_item, item_code);

            if (reward_result_value != RC::RC_ITEM_SUCCESS)
            {
                MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD nak_msg;
                nak_msg.memo_guid_ = recv_msg->memo_guid_;
                nak_msg.result_value_ = false;

                player->SendPacket(&nak_msg, sizeof(nak_msg));

                //--보상 DB 롤백
                MSG_DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN send_msg;
                send_msg.char_guid_ = recv_msg->db_result_char_guid_;
                send_msg.map_code_ = recv_msg->db_result_map_code_;
                send_msg.reward_type_ = recv_msg->db_result_reward_type_;
                send_msg.tender_cost_ = temp_cost;

                player->SendToGameDBPServer(&send_msg, sizeof(send_msg));

                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Domination Item Reward Fail[User:%u]", 
                    recv_msg->request_char_guid_);

                return;
            }
        }
        //--최종 하임 보상및 보상 결과 알림
        //2010.07.13 : 보상 금액인 1억으로 MAX를 정한다. 비정상적인 하임 습득에 대한 일시적 처리
		//현재 중국은 1억으로 일괄 보상
        const ULONGLONG max_reward_money = 100000000;

        if (amount_heim > max_reward_money)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Reward Heim[Name:%s][Heim:%I64u]",
                player->GetCharName(), amount_heim);

            amount_heim = max_reward_money;
        }

        player->GetPostManager()->RewardHeim(player, amount_heim, recv_msg->request_reward_type_);

        MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD cmd_msg;
        cmd_msg.memo_guid_ = recv_msg->memo_guid_;
        cmd_msg.result_value_ = true;

        player->SendPacket(&cmd_msg, sizeof(cmd_msg));
#else
        //--보상 로직
        player->GetPostManager()->RewardHeim(player, recv_msg->tender_cost_, 
                                             recv_msg->db_result_reward_type_);
        //--보상 완료 알림
        MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD cmd_msg;
        cmd_msg.memo_guid_ = recv_msg->memo_guid_;
        cmd_msg.result_value_ = true;

        player->SendPacket(&cmd_msg, sizeof(cmd_msg));
#endif
    }
}
Handler_DG_CHARINFO_IMPL( DG_EVENT_LOTTO_REWARD_AND_DEL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_ACK* recv_msg = 
        static_cast<MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_ACK*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->request_char_guid_);
    if (player == NULL)
    {
        assert(!"Player Not Fount");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%u]Player Not Found", 
            recv_msg->request_char_guid_);
        return;
    }

    //--보상 조건
    if (recv_msg->result_value_ != 0)
    {
        assert(!"DB Query Result Fail");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][User:%u]DB Query Result Fail", 
            recv_msg->request_char_guid_);
        return;
    }
    else if (recv_msg->request_char_guid_ != recv_msg->db_result_char_guid_)
    {
        assert(!"CharGuid Not Same");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]CharGuid Not Same[DB:%u]!=[REQ:%u]", 
            recv_msg->db_result_char_guid_, recv_msg->request_char_guid_);
        return;
    }
    else
    {
        //--보상 로직
        player->GetPostManager()->RewardHeim(player, recv_msg->db_result_prize_money_,
                                             SYSMEMO_LOTTO_WIN);
    }
}
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
Handler_DG_CHARINFO_IMPL( DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK* recv_msg = 
        static_cast<MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->char_guid_);
    if (player == NULL)
    {
        assert(!"Player Not Fount");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%u]Player Not Found", recv_msg->char_guid_);
        return;
    }

    GameDominationManager* domination_manager = GameDominationManager::Instance();
    RC::eITEM_RESULT result_value = RC::RC_ITEM_SUCCESS;

    if(recv_msg->db_result_value_ == true)
    {
        //-- 아이템 보상
        result_value = domination_manager->GetSystemMemo()->RewardSysMemoItem(player, recv_msg);
    }
    else
    {
        assert(!"Reward item check fail");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Reward Item check fail");

        result_value = RC::RC_ITEM_FAILED;
    }

    MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_ANS ans_msg;
    {
        ans_msg.memo_guid_ = recv_msg->memo_guid_;
        ans_msg.result_value_ = result_value;
    }
    player->SendPacket(&ans_msg, sizeof(ans_msg));
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
Handler_DG_CHARINFO_IMPL(DG_CHARINFO_HONORSYSTEM)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    HonorSystem::Instance()->ParseDGPacket(pMsg);
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
Handler_DG_CHARINFO_IMPL(DG_CHARINFO_POINTWALLET_SYSTEM)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_CHARINFO_POINTWALLET_SYSTEM* packet = \
        static_cast<MSG_DG_CHARINFO_POINTWALLET_SYSTEM*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return ;

    PointWalletManager* const pointwallet_manager = player->GetPointWalletManager();
    pointwallet_manager->ParseDGPacket(packet);
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

Handler_DG_CHARINFO_IMPL(DG_CHARNAME_CHANGE_ACK)
{
    MSG_DG_CHARNAME_CHANGE_ACK* packet = \
        static_cast<MSG_DG_CHARNAME_CHANGE_ACK*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return ;

    POSTYPE item_pos   = packet->item_pos;
    SLOTIDX slot_index = packet->slot_index;

    ItemManager* item_manager = player->GetItemManager();
    if (item_manager == NULL) {
        return;
    }
    SCSlotContainer* container = (SCSlotContainer*)item_manager->GetItemSlotContainer(slot_index);
    if (container == NULL) {
        return;
    }
    SCItemSlot& item_slot = (SCItemSlot&)container->GetSlot(item_pos);
    if (item_slot.GetSerial() == 0) {
        return;
    }
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info == NULL) {
        return;
    }

    item_slot.SetLock(FALSE);

    if (packet->result == 0) // success ~~
    {
        // 성공의 경우에는 먼저 아이템 사용 로그부터 남긴다
        GAMELOG->LogItem(ITEM_USE, player, &item_slot);

        // 소모성 아이템 삭제
        if (item_info->m_wType == eITEMTYPE_CHARACTER_NAME_CHANGE)
        {
            if(item_slot.GetNum() <= 1) {
                container->DeleteSlot(item_pos, NULL);
            }
            else {
                container->UpdateSlot(item_pos, -1);
            }
        }

        player->ResetUseItem(item_info->m_wCoolTimeType);

        MSG_CG_ITEM_USE_ACK ack;
        ack.m_atIndex = slot_index;
        ack.m_atPos   = item_pos;
        player->SendPacket(&ack, (WORD)sizeof(ack));
    }

    MSG_CG_CHARNAME_CHANGE_REQUEST_ACK msg;
    msg.result = packet->result;
    player->SendPacket(&msg, sizeof(msg));
}

Handler_DG_CHARINFO_IMPL(DG_CHARACTER_EXTRA_INFO_CMD)
{
    MSG_DG_CHARACTER_EXTRA_INFO_CMD* packet = \
        static_cast<MSG_DG_CHARACTER_EXTRA_INFO_CMD*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return ;

#ifdef _NA_006372_20130108_BADGE_ITEM
    ;{
        // store toggle item info (D->G)
        ToggleItemInfos& toggle_item_info_container = \
            player->GetExtraInfoManager().GetToggleItemInfos();
        memcpy(&toggle_item_info_container, &packet->toggle_item_info_, \
            sizeof(packet->toggle_item_info_));
    };
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ;{
        player->GetExtraInfoManager().SetBattleGroundRecordInfos(packet->battle_ground_record_infos_);
    };
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    ;{
        player->GetExtraInfoManager().SetGoldRushRecordInfos(packet->goldrush_record_infos_);
    };
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    ;{
        player->GetExtraInfoManager().SetDailyQuestResetUseTime(\
            packet->daily_quest_reset_item_use_time);
    };
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    ;{
        player->GetExtraInfoManager().SetMiniGameBingoInfos(\
            packet->minigame_bingo_infos);
    };
#endif //_NA_008404_20150712_MINIGAME_SYSTEM

    //player->GetExtraInfoManager().OnEnterField(); // 옵션 중복 적용 문제 생겨서 제거함 - arycoat.2013.04.03.
}

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
Handler_DG_CHARINFO_IMPL(DG_USER_EXTRA_INFO_CMD)
{
    MSG_DG_USER_EXTRA_INFO_CMD* packet = \
        static_cast<MSG_DG_USER_EXTRA_INFO_CMD*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player ) return ;

    // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ;{
        player->GetExtraInfoManager().SetAttendanceInfos(\
            packet->attendance_infos);
    };
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
}
#endif // _NA_007932_20150106_USER_EXTRA_INFO

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
Handler_DG_CHARINFO_IMPL(DG_EVENT_SMARTNPC_SYSTEM)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    g_ExtraNPCManager.ParseDGPacket(pMsg);
}
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007086_20140318_MONSTERBOOK
Handler_DG_CHARINFO_IMPL(DG_CHARACTER_MONSTERBOOK_CMD)
{
    MSG_DG_CHARACTER_MONSTERBOOK_CMD* packet = \
        static_cast<MSG_DG_CHARACTER_MONSTERBOOK_CMD*>(pMsg);

    switch(packet->packet_type)
    {
        case DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD:
        {
            const MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD* packet2 = \
                static_cast<MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD*>(pMsg);
            Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet2->m_dwKey );
            if (!player) return ;

            MonsterBook& monster_book = player->GetMonsterBook();
            memcpy(&monster_book, &packet2->monster_book, sizeof(monster_book));
        } break;
        case DG_CHARACTER_MONSTERBOOK_ADD_ACK:
        {
            const MSG_DG_CHARACTER_MONSTERBOOK_ADD_ACK* packet2 = \
                static_cast<MSG_DG_CHARACTER_MONSTERBOOK_ADD_ACK*>(pMsg);

            Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet2->m_dwKey );
            if (!player) return ;
            DWORD monster_code  = packet2->monster_code;

            MonsterBook& monster_book = player->GetMonsterBook();
            if (monster_book.find(monster_code) == 0) {
                monster_book.push_back(packet2->monster_code);

                MSG_CG_MONSTERBOOK_ADD_CMD cmd;
                cmd.monster_code_ = monster_code;
                player->SendPacket(&cmd, sizeof(cmd));

                GAMELOG->LogMonsterBook(ACT_MONSTERBOOK_REGISTER, player, cmd.monster_code_);
            }
        } break;
    }
}
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
Handler_DG_CHARINFO_IMPL(DG_GMEVENT_PEAKTIME_REGISTER_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_GMEVENT_PEAKTIME_REGISTER_ACK* packet = \
        static_cast<MSG_DG_GMEVENT_PEAKTIME_REGISTER_ACK*>(pMsg);

    PeakTimeEvent::Instance()->AddGmEvent(packet->register_event);
}

Handler_DG_CHARINFO_IMPL(DG_GMEVENT_PEAKTIME_TERMINATE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_GMEVENT_PEAKTIME_TERMINATE_ACK* packet = \
        static_cast<MSG_DG_GMEVENT_PEAKTIME_TERMINATE_ACK*>(pMsg);

    PeakTimeEvent::Instance()->DeleteProgressEvent();
}
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_UPDATE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_UPDATE_ACK* packet = \
        static_cast<MSG_DG_SUN_RANKING_UPDATE_ACK*>(pMsg);

    SunRankingManager::Instance()->UpdateFromSunRankingPlayer(packet->info);
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_SELECT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_SELECT_ACK* packet = \
        static_cast<MSG_DG_SUN_RANKING_SELECT_ACK*>(pMsg);

    for (int i = 0; i < packet->count; ++i)
    {
        SunRankingManager::Instance()->UpdateFromSunRankingPlayer(packet->infos[i]);
    }
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_GRADE_SELECT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_GRADE_SELECT_ACK* packet = \
        static_cast<MSG_DG_SUN_RANKING_GRADE_SELECT_ACK*>(pMsg);

    for (int i = 0; i < packet->count; ++i)
    {
        SunRankingGradeManager::Instance()->SetGradeInfo(packet->infos[i]);
    }
}

Handler_DG_CHARINFO_IMPL(DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK* packet = \
        static_cast<MSG_DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK*>(pMsg);
}

Handler_DG_CHARINFO_IMPL(DG_STATUE_SELECT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_STATUE_SELECT_ACK* packet = \
        static_cast<MSG_DG_STATUE_SELECT_ACK*>(pMsg);

    if (packet->count > 0)
    {
        StatueManager::Instance()->SetStatue(packet->statue_info.statue_type, packet->statue_info.statue_index, &packet->statue_info);
    }
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_SETTLE_START_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_SETTLE_START_CMD* packet = \
        static_cast<MSG_DG_SUN_RANKING_SETTLE_START_CMD*>(pMsg);
    
    SunRankingGradeManager::Instance()->Settle();
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_SETTLE_COMPLETE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_SETTLE_COMPLETE_CMD* packet = \
        static_cast<MSG_DG_SUN_RANKING_SETTLE_COMPLETE_CMD*>(pMsg);

    SunRankingGradeManager::Instance()->Settle_Complete();
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_SELECT_COMPLETE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_SELECT_COMPLETE_CMD* packet = \
        static_cast<MSG_DG_SUN_RANKING_SELECT_COMPLETE_CMD*>(pMsg);

    SunRankingGradeManager::Instance()->SetLoadComplete(eLOAD_KIND_RANKING_SELECT);
}

Handler_DG_CHARINFO_IMPL(DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD* packet = \
        static_cast<MSG_DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD*>(pMsg);

    SunRankingGradeManager::Instance()->SetLoadComplete(eLOAD_KIND_GRADE_SELECT);
}
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM