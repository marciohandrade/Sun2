#include "stdafx.h"
#include ".\Handler_CG_BATTLE.h"

#include <PacketStruct_CG.h>
#include <PacketStruct_GZ.h>
#include <PacketControl/WarMessages_Sync.h>
#include <SCSkillSlotContainer.h>

#include "PublicMath.h"
#include "SkillInfoParser.h"
#include "AIParamParser.h"


#include "PlayerManager.h"
#include "Player.h"
#include "./MovingPlanner.h"
#include "CharacterFormula.h"
#include "TriggerManager.h"
#include "TriggerMsg.h"
#include "MapObject.h"
#include "Skill.h"
#include "ChaoState.h"
#include "SkillManager.h"
#include "EnchantPoisonStatus.h"
#include "StatusManager.h"
#include "GameDominationManager.h"
#include <ResultCode.h>
#include "GameGuild.h"
#include "GameGuildManager.h"
#include "GameGuildMember.h"
#include "DominationArea.h"

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
#include "ItemManager.h"
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY

#ifdef _NA_20100322_AGGRO_DISPLAY
#include "TargetManager.h"
#include "BattleRecord.h"
#endif
#include "MovementChecker.h"


Handler_CG_BATTLE::Handler_CG_BATTLE()
{
}

Handler_CG_BATTLE::~Handler_CG_BATTLE()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_BATTLE::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_BATTLE::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        // CG_BATTLE
        //HANDLER_NODE_INFO(CG_BATTLE, CG_BATTLE_VKR_ATTACK_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CG_BATTLE, CG_BATTLE_VKR_RELOAD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_BATTLE_PLAYER_ATTACK_SYN, PHASE_SERVICE),
        // CG_DOMINATION
        //_KR_001385_20091013_DOMINATION_BASICINFO_SETTING
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_MINIMAP_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_APPLY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_NOTICE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_BATTING_HEIM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_MANAGE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_ACCEP_HEIM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_SAVE_GATESTAT_SYN, PHASE_SERVICE),
        // _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN, PHASE_SERVICE),

#ifdef _NA_20100322_AGGRO_DISPLAY
        HANDLER_NODE_INFO(CG_BATTLE,  CG_MONSTER_SELECT_INFO_SYN, PHASE_SERVICE),        
#endif
        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_MEMBER_JOIN_SYN, PHASE_SERVICE),    
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_MEMBER_LEAVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_BATTLE,  CG_DOMINATION_REFRESH_MEMBERLIST_REQ, PHASE_SERVICE),
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



// CHANGES, f100830.4L, code arrangement and replace an arguement block for a skill to a SkillInfo.
//
void Handler_CG_BATTLE::OnCG_BATTLE_PLAYER_ATTACK_SYN(ServerSession* pServerSession,
                                                      MSG_BASE* pMsg, WORD wSize)
{
    __UNUSED(pServerSession);
    const DWORD ERRCODE_HACKED = 1000000UL;
    const bool packet_integrity = (sizeof(MSG_CG_BATTLE_PLAYER_ATTACK_SYN) == wSize);
    //
    const MSG_CG_BATTLE_PLAYER_ATTACK_SYN* recv_msg =
        static_cast<const MSG_CG_BATTLE_PLAYER_ATTACK_SYN*>(pMsg);
    // NOTE, an offset pointer of a decode routine start at point of 'client serial'
    // so that a field of a 'm_dwKey' can read a plain stream.
    const DWORD user_key = recv_msg->m_dwKey;
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    GameField* const game_field = player ? player->GetField() : NULL;
    if (player == NULL || game_field == NULL) {
        return;
    }
    //
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(packet_integrity, ERRCODE_HACKED);

        BYTE decode_stream[sizeof(MSG_CG_BATTLE_PLAYER_ATTACK_SYN) << 1];
        CopyMemory(decode_stream, recv_msg, wSize);

        recv_msg = reinterpret_cast<const MSG_CG_BATTLE_PLAYER_ATTACK_SYN*>(decode_stream);
        const_cast<MSG_CG_BATTLE_PLAYER_ATTACK_SYN*>(recv_msg)->Decode();
        // NOTE, can use a full data of a recv_msg since at this line.
        //
        SkillInfo skill_info_data(player, *recv_msg); // editable data structure
        //
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        player->GetSyncTuner()->CheckClientAbusedData(recv_msg->client_test_result);
    #endif

        RC::eBATTLE_RESULT battle_result = player->CanStyleAttack(&skill_info_data);
        IfFailThenBreak(battle_result == RC::RC_BATTLE_SUCCESS, battle_result);
        //
        const SLOTCODE style_code = skill_info_data.skill_code_;
        assert(skill_info_data.root_skill_info_ != NULL); // aligned by 'CanStyleAttack'
        //
        Character* pTargetChar = game_field->FindCharacter(skill_info_data.main_target_key_);
        IfFailThenBreak(pTargetChar, RC::RC_BATTLE_INVALID_MAINTARGET);

        //added by _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
        const bool isPlayer = pTargetChar->IsEqualObjectKind(PLAYER_OBJECT);
        const bool is_summon = isPlayer == false && pTargetChar->IsEqualObjectKind(SUMMON_OBJECT);
        const bool isMob = isPlayer == false && is_summon == false && pTargetChar->IsEqualObjectKind(NPC_OBJECT);

        sPOLICY_CHARACTER& policy = player->Policyment;
        const bool bApplyGeneralForcedAtkRule = !(policy.FIELD & policy.PCC_IN_SSQ_FIELD);

        // (NOTE) PvP의 경우는 기존대로 PvPRule에 따르도록 처리하고 몬스터와의 관계만 따지도록 하자.
        if (bApplyGeneralForcedAtkRule && isMob) {
            // (NOTE) 안전지대에서 몬스터 공격하는 상황 방지
            const nsAI::TileEvent* tile_event = 0;
            IfFailThenBreak(player->PlanEvent->GetTileEvent(&tile_event) != false &&
                            (tile_event->Attr & PTA_SAFETY_ZONE) == 0,
                            RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY);
        }
        //                                add rule      add rule
        if (bApplyGeneralForcedAtkRule && (isPlayer || is_summon))
        {
            bool befForcedAttack = player->IsForceAttack();
            // CHANGES, f100830.5L, waverix, fix up an invalid comparision
            // which compare an attack_sequence with an attack_propensity.
            player->SetForceAttack(skill_info_data.attack_propensity_ == ATTACK_PROPENSITY_FORCE);
            //SUNLOG(eDEV_LOG, "[Handler_CG_BATTLE::OnCG_BATTLE_PLAYER_ATTACK_SYN] Force Attack = %d",
            //       recv_msg->byAttackPropensity);
            battle_result = player->ChkAttackPvP(pTargetChar);
            IfFailThen(battle_result == RC::RC_BATTLE_SUCCESS, battle_result)
            {
                player->SetForceAttack(befForcedAttack);
                break;
            }
        }
        // 액션 딜레이 설정
        player->SetAttackDelay(skill_info_data.attack_sequence_, style_code);
        // synchronize between a client and server
        player->ModifyPosition(&skill_info_data);

        //----------------------------------------------------------------------------
        // 1, 2타와 3타 구분해서 처리
        //----------------------------------------------------------------------------
        const BASE_STYLEINFO* base_style_info = \
            static_cast<const BASE_STYLEINFO*>(skill_info_data.root_skill_info_);
        eSKILL_TYPE select_skill_type = SKILL_TYPE_NORMAL;

        if (skill_info_data.attack_sequence_ < eATTACK_SEQUENCE_THIRD &&
            (base_style_info->m_SkillClassCode != eSTYLE_TWOHANDSWORD_NORMAL &&
#ifdef _NA_008702_20160224_CHANGING_HIT_POPULATIONS
			base_style_info->m_SkillClassCode != eSTYLE_TWOHANDAXE_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_ONEHANDSWORD_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_DRAGON_TRANSFORM1 &&
			base_style_info->m_SkillClassCode != eSTYLE_DRAGON_TRANSFORM2 &&
			base_style_info->m_SkillClassCode != eSTYLE_SHADOW_DAGGER_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_MYSTIC_GUNTLET_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_MYSTIC_POLEARM_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_ONEHANDCROSSBOW_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_STAFF_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_ORB_NORMAL &&
			base_style_info->m_SkillClassCode != eSTYLE_HELLROID_WEAPON1 &&
			base_style_info->m_SkillClassCode != eSTYLE_WITCHBLADE_ARCBLADE &&
#endif //(_NA_008702_20160224_CHANGING_HIT_POPULATIONS)
             base_style_info->m_SkillClassCode != eSTYLE_SPEAR_NORMAL &&
             base_style_info->m_SkillClassCode != eSTYLE_SHADOW_WHIP_NORMAL &&
             base_style_info->m_SkillClassCode != eSTYLE_ETHER_NORMAL))
        {
            select_skill_type = SKILL_TYPE_NORMAL;

            // 일반 범위 공격 확률이 있는지 체크
            if (player->GetAttr().GetAreaAttackRatio())
            {
                if (random(1, 100) <= player->GetAttr().GetAreaAttackRatio()) {
                    select_skill_type = SKILL_TYPE_NORMAL_AREA;
                }
            }
        }
        else
        {
            select_skill_type = SKILL_TYPE_STYLE;
        }

        //SkillInfo SkillInfoData(player, stylecode, 
        //    recv_msg->dwClientSerial, 
        //    recv_msg->dwTargetKey, 
        //    &wvResultCurPos, 
        //    &wvResultDestPos, 
        //    NULL, 
        //    recv_msg->byAttackType);

        player->GetSkillManager()->RegisterSkill(select_skill_type, &skill_info_data);
        if (StatusManager* status_manager = player->GetStatusManager())
        {  // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
            // 현재 인첸트 포이즌 상태이면 
            EnchantPoisonStatus* poison_status = static_cast<EnchantPoisonStatus*>(\
                status_manager->FindStatus(eCHAR_STATE_ENCHANT_POISON));
            if (poison_status) {
                poison_status->ExecuteSkill(pTargetChar);
            }
        };

        return /*RETURN true VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    const DWORD errCode = flow.GetErrorCode();
    switch (errCode)
    {
    case ERRCODE_HACKED:
        SUNLOG(eDEV_LOG, "[RNDPROTOCOL_REPORT][SIZE] MSG_CG_BATTLE_PLAYER_ATTACK_SYN %u!=%u", 
            sizeof(MSG_CG_BATTLE_PLAYER_ATTACK_SYN), wSize);
        return;
    }

    assert(SAFE_NUMERIC_TYPECAST(DWORD, errCode, BYTE));

    // 밀리 공격시의 위치 보정
    player->StopMoving();
    player->SetMoveState(CMS_STOP);

    MSGSUB_SYNC_STOP_BRD msg;
    msg.m_dwObjectKey   = player->GetObjectKey();
    msg.m_wvCurPos      = *player->GetPosPtr();
    player->SendPacketAroundForSync(&msg);

    MSG_CG_BATTLE_PLAYER_ATTACK_NAK NakMsg;
    NakMsg.m_byErrorCode = (BYTE)errCode;
    player->SendPacket(&NakMsg, sizeof(NakMsg));
}

VOID Handler_CG_BATTLE::OnCG_BATTLE_VKR_ATTACK_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize) //사용안함
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//	너무 사용도 안하는 코드가 장기간 존재하고 있다. 이 기회에 막아버린다.
	MSG_CG_BATTLE_VKR_ATTACK_SYN* pRecvMsg = (MSG_CG_BATTLE_VKR_ATTACK_SYN*)pMsg;
	SUNLOG( eCRITICAL_LOG, "Impossible Logic Reached [" __FUNCTION__ "] UKey:%u", pRecvMsg->m_dwKey );
	return;
}

VOID Handler_CG_BATTLE::OnCG_BATTLE_VKR_RELOAD_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize) //사용안함
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//	너무 사용도 안하는 코드가 장기간 존재하고 있다. 이 기회에 막아버린다.
	MSG_CG_BATTLE_VKR_RELOAD_SYN* pRecvMsg = (MSG_CG_BATTLE_VKR_RELOAD_SYN *)pMsg;
	SUNLOG( eCRITICAL_LOG, "Impossible Logic Reached [" __FUNCTION__ "] UKey:%u", pRecvMsg->m_dwKey );
	return;
}

//미니맵 관련 정보요청에 관한 처리를 한다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_MINIMAP_LIST_SYN(ServerSession * pServerSession, 
                                                         MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_DOMINATION_MINIMAP_LIST_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_MINIMAP_LIST_SYN*>(pMsg);
    
    assert(sizeof(*recv_msg) >= wSize);
    if (sizeof(*recv_msg) < wSize) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    
    MSG_CG_DOMINATION_MINIMAP_LIST_ACK sendMsg;
    sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    sendMsg.m_NumDominationGuild = 
        domination_manager->GetMiniMapInfo(sendMsg.m_GuildInfo, _countof(sendMsg.m_GuildInfo));

    player->SendPacket(&sendMsg, sendMsg.GetSize());
}

//지역 점령 타워를 클릭했을때 경우의 수를 따져서 제한 값을 리턴한다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_NOTICE_SYN(ServerSession * server_session, 
                                                   MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_NOTICE_SYN* recv_msg = static_cast<MSG_CG_DOMINATION_NOTICE_SYN*>(msg);

    assert(sizeof(*recv_msg) >= size);
    if (sizeof(*recv_msg) < size) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player : %d", recv_msg->m_dwKey);
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG( eCRITICAL_LOG, __FUNCTION__":Not found field by plyer");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->m_MapCode;

    domination_manager->SendDominationNotice(player, map_code);
}

//경매에 참여 할 경우 리스트를 응답하여준다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_APPLY_SYN(ServerSession * server_session, 
                                                  MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_APPLY_SYN* recv_msg = static_cast<MSG_CG_DOMINATION_APPLY_SYN*>(msg);

    assert(sizeof(*recv_msg) >= size);
    if (sizeof(*recv_msg) < size) 
    {
        return;
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* const field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER 
    MAPCODE map_code = recv_msg->m_MapCode;

    domination_manager->ResponseAuctionApply(player, map_code);
}

//하임을 배팅하기 위한 창을 연다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_BATTING_HEIM_SYN(ServerSession * server_session, 
                                                         MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_BATTING_HEIM_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_BATTING_HEIM_SYN*>(msg);

    assert(sizeof(*recv_msg) >= size);
    if (sizeof(*recv_msg) < size) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }
    
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->m_MapCode;

    MSG_GZ_DOMINATION_BATTING_HEIM_SYN send_msg;

    send_msg.m_dwKey = recv_msg->m_dwKey;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    send_msg.m_mapCode = map_code;
#endif
    send_msg.m_guildGuid = player->GetGuildGuid();

    g_pGameServer->SendToServer( GUILD_SERVER, &send_msg, sizeof(send_msg) );
}

//하임을 배팅한다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_ACCEP_HEIM_SYN(ServerSession * pServerSession, 
                                                       MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_DOMINATION_ACCEP_HEIM_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_ACCEP_HEIM_SYN*>(pMsg);

    assert(sizeof(*recv_msg) >= wSize);
    if (sizeof(*recv_msg) < wSize) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }
    
    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->m_MapCode;
    if (domination_manager->FindAreaInfo(map_code) == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found domination area info : %d", map_code);
        return;
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    const int amount_cost = static_cast<int>(recv_msg->betting_item_count_);

    MONEY tender_cost = 
        domination_manager->GetTenderCost(map_code, player->GetGuildGuid());

    RC::eDOMINATION_RESULT result_value = 
        domination_manager->CanEntryAuctionCost(player, 
                                                map_code, 
                                                amount_cost, 
                                                recv_msg->m_TokenPos);

    if (result_value == RC::DOMINATION_RESULT_SUCCESS)
    {
        SLOTCODE auction_item_code = 
            DominationInfoParser::Instance()->GetBattingItemCodeByMapCode(map_code);
        if(!(domination_manager->RemoveAuctionItem(player, recv_msg->m_TokenPos, 
                                                   amount_cost, auction_item_code)))
        {
            SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Fail remove auction item[%s]",
                   player->GetCharName());
            return;
        }

        //기존 입찰과 신규 입찰 게임 로그 구분
        if (tender_cost == 0)
        {
            GAMELOG->LogDomination(ITEM_DOMINATION_AUCTION_APPLY, player, 
                                   auction_item_code, amount_cost, map_code);
        }
        else
        {
            GAMELOG->LogDomination(ITEM_DOMINATION_AUCTION_DUP_APPLY, player, 
                                   auction_item_code, amount_cost, map_code);
        }
    }
    else
    {
        MSG_CG_DOMINATION_ACCEP_HEIM_ACK nak_msg;
        nak_msg.m_ResultCode = result_value;
        player->SendPacket(&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Error code[%d]", result_value);
        return;
    }
#else
    RC::eDOMINATION_RESULT result_value = 
        domination_manager->CanEntryAuctionCost(player, 
                                                map_code, 
                                                recv_msg->m_TenderCost);
    if (result_value != RC::DOMINATION_RESULT_SUCCESS)
    {
        MSG_CG_DOMINATION_ACCEP_HEIM_ACK nak_msg;
        nak_msg.m_ResultCode = result_value;
        player->SendPacket(&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Error code[%d]", result_value);
        return;
    }

    MONEY tender_cost = 0;//하임 입찰의 경우 기존 금액을 가져오지 않는다.
    const int amount_cost = static_cast<int>(recv_msg->m_TenderCost);
#endif

    GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
    if(guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found guild[%s]", player->GetCharName());
        return;
    }

    MSG_GZ_DOMINATION_ACCEP_HEIM_SYN send_msg;
    {
        send_msg.m_dwKey = recv_msg->m_dwKey;
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        send_msg.m_TokenPos = recv_msg->m_TokenPos;
        send_msg.number_of_betting_item = amount_cost;
    #endif

        DominationAuctionInfo& db_auction_info = send_msg.m_DominationActionInfo;
        {//--기타 DB정보 입력
            guild->GetGuildMark(db_auction_info.m_BackGroundIndex, db_auction_info.m_PatternIndex);
            db_auction_info.m_MasterGuid = player->GetCharGuid();
            strncpy(db_auction_info.m_tszGuildMaster, player->GetCharName(), 
                    _countof(db_auction_info.m_tszGuildMaster));
            db_auction_info.m_tszGuildMaster[_countof(db_auction_info.m_tszGuildMaster) - 1] = '\0';
        }

        DOMINATION_ACTION_INFO& auction_info = db_auction_info.m_ActionInfo ;
        {//--경매 정보 입력
            auction_info.m_TenderCost = amount_cost + tender_cost;
            auction_info.m_FieldCode = map_code;
            auction_info.m_GuilGuid = player->GetGuildGuid();
            domination_manager->GetCurrentDateTime(auction_info.m_tszActionApplyTime,
                                                   _countof(auction_info.m_tszActionApplyTime));
            strncpy(auction_info.m_tszGuildName, guild->GetGuildName(), 
                    _countof(auction_info.m_tszGuildName ) );
            auction_info.m_tszGuildName[_countof(auction_info.m_tszGuildName) - 1] = '\0';
        }
    }
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

//지역 점령전 관리 버튼을 눌렀을 경우
VOID Handler_CG_BATTLE::OnCG_DOMINATION_MANAGE_SYN(ServerSession* server_session, 
                                                   MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_MANAGE_SYN* recv_msg = static_cast<MSG_CG_DOMINATION_MANAGE_SYN*>(msg);
    assert(sizeof(*recv_msg) >= size);
    if (sizeof(*recv_msg) < size) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->m_MapCode;

    BYTE deffensWall[MAX_DOMINATION_DEFENSEWALL_NUM];

    MSG_CG_DOMINATION_MANAGE_ACK sendMsg;
    sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    sendMsg.m_TotalPoint = domination_manager->GetDefenseWall(map_code, deffensWall);
    memcpy(&(sendMsg.m_DefenseWall), &deffensWall, sizeof(deffensWall));
    player->SendPacket( &sendMsg, sizeof(sendMsg) );
}

//지역 점령전 관리를 끝내고 저장을 시도한다.
VOID Handler_CG_BATTLE::OnCG_DOMINATION_SAVE_GATESTAT_SYN(ServerSession* server_session, 
                                                          MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_SAVE_GATESTAT_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_SAVE_GATESTAT_SYN*>(msg);

    assert(sizeof(*recv_msg) >= size);
    if (sizeof(*recv_msg) < size) 
    { 
        return; 
    }

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->m_MapCode;

    RC::eDOMINATION_RESULT result_value = 
        domination_manager->CanSaveGateState(player, map_code, recv_msg->m_DefenseWall);
    if (result_value != RC::DOMINATION_RESULT_SUCCESS)
    {
        MSG_CG_DOMINATION_SAVE_GATESTAT_ACK nak_msg;
        nak_msg.m_ResultCode = result_value;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Fail save to gate state[%d]", result_value);
        return;
    }

    domination_manager->SetDefenseWall(map_code, recv_msg->m_DefenseWall);

    GAMELOG->LogDomination(ACT_DOMINATION_SET_GATE, player, NULL, recv_msg->m_DefenseWall[0],
                                                                  recv_msg->m_DefenseWall[1],
                                                                  recv_msg->m_DefenseWall[2]);
}

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
VOID Handler_CG_BATTLE::OnCG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN(ServerSession* server_session, 
                                                                   MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN*>(msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found field");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    MAPCODE map_code = recv_msg->map_code_;

    RC::eDOMINATION_RESULT result_value = 
        domination_manager->CanIncreaseDeffenseWallPoint(player, map_code);
    if (result_value != RC::DOMINATION_RESULT_SUCCESS)
    {
        MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK nak_msg;
        nak_msg.result_code_ = result_value;
        player->SendPacket(&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, __FUNCTION__":Fail increase deffense point : %d", result_value);
        return;
    }

    DominationArea* domination_area = domination_manager->GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    domination_area->SetTransactionToDeffenseWall(true);

    if (domination_manager->IncreaseDeffenseWallPoint(player, recv_msg->item_code_, map_code) == true)
    {
        GAMELOG->LogDomination(ACT_DOMINATION_ADD_DEFFENSEWALL_POINT, player, 
                               recv_msg->item_code_, 1, map_code);
    }
    else
    {
        MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK nak_msg;
        nak_msg.result_code_ = result_value;
        player->SendPacket(&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, __FUNCTION__":Fail increase deffense point : %d", result_value);

        domination_area->SetTransactionToDeffenseWall(false);
    }
}


#ifdef _NA_20100322_AGGRO_DISPLAY
VOID Handler_CG_BATTLE::OnCG_MONSTER_SELECT_INFO_SYN(ServerSession* server_session, MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_MONSTER_SELECT_INFO_SYN* recv_msg = 
        static_cast<MSG_CG_MONSTER_SELECT_INFO_SYN*>(msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found field");
        return;
    }

    Character* const character = field->FindCharacter(recv_msg->monster_key);
    if (character == NULL || character->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        return;
    }

    NPC* const target_monster = static_cast<NPC*>(character);
    if (!target_monster)
    {
        return;
    }

    TargetManager* const target_manager = target_monster->GetTargetManager();
    // assertion (target_manager != NULL);
    BattleRecord* const battle_recode = target_manager->GetBattleRecord();
    battle_recode->SendAggroDisPlayInfo(player);
}
#endif

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
VOID Handler_CG_BATTLE::OnCG_DOMINATION_MEMBER_JOIN_SYN(ServerSession* server_session, 
                                                        MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_MEMBER_JOIN_SYN* recv_msg = 
        static_cast<MSG_CG_DOMINATION_MEMBER_JOIN_SYN*>(msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found field by player");
        return;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    RC::eDOMINATION_RESULT result_code = 
        domination_manager->CanJoinMember(player, recv_msg->map_code_);

    if (result_code != RC::DOMINATION_RESULT_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__"Can't Join Domination Area [%d]", result_code);

        MSG_CG_DOMINATION_MEMBER_JOIN_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MSG_GZ_DOMINATION_MEMBER_JOIN_SYN send_msg;
    send_msg.map_code_ = recv_msg->map_code_;
    if (!domination_manager->MakeMemberInfo(player, recv_msg->map_code_, send_msg.member_info_))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail make to member data : %d", player->GetCharGuid());
        return;
    }
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

VOID Handler_CG_BATTLE::OnCG_DOMINATION_MEMBER_LEAVE_SYN(ServerSession* server_session, 
                                                         MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    
    const MSG_CG_DOMINATION_MEMBER_LEAVE_SYN* request = 
        static_cast<MSG_CG_DOMINATION_MEMBER_LEAVE_SYN *>(msg);

    Player* const requester = PlayerManager::Instance()->FindPlayerByUserKey(request->m_dwKey);
    if (requester == NULL)
    {
        return;
    }
    if (requester->GetField() == NULL)
    {
        return;
    }

    struct ResponseSender 
    {
        explicit ResponseSender(Player& requester, const MSG_CG_DOMINATION_MEMBER_LEAVE_SYN& request
        ) : requester_(requester), request_(request), result(RC::DOMINATION_RESULT_SUCCESS)
        {
        }
        ~ResponseSender()
        {
            if (result == RC::DOMINATION_RESULT_SUCCESS)
            {
                MSG_GZ_DOMINATION_MEMBER_LEAVE_SYN response;
                response.map_code_ = request_.map_code_;
                response.member_guid_ = request_.memeber_guid_;
                response.requester_guid = requester_.GetCharGuid();
                g_pGameServer->SendToServer(GUILD_SERVER, &response, sizeof(response));
            }
            else
            {
                MSG_CG_DOMINATION_MEMBER_LEAVE_ACK response;
                response.result_code_ = result;
                response.memeber_guid_ = request_.memeber_guid_;
                requester_.SendPacket(&response, sizeof(response));
            }
        }
        RC::eDOMINATION_RESULT result;
    private:
        Player& requester_;
        const MSG_CG_DOMINATION_MEMBER_LEAVE_SYN& request_;
        void operator=(const ResponseSender&) {}
    };
    ResponseSender response_sender(*requester, *request);

    const CHARGUID requester_guid = requester->GetCharGuid();
    const CHARGUID leave_char_guid = request->memeber_guid_;
    const bool requester_is_master = (requester_guid != leave_char_guid);

    const GameDominationManager* const domination_manager = GameDominationManager::Instance();
    const DominationArea* const domination_area = domination_manager->FindAreaInfo(request->map_code_);
    if (domination_area == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Domination area is not found.|map code = %d|"), 
            __FUNCTION__, 
            request->map_code_
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }

    const DominationMember* const domination_member = domination_area->GetMember();
    if (domination_member == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Domination member is not found.|map code = %d|"), 
            __FUNCTION__, 
            request->map_code_
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }

    const DominationMemberInfo* const request_member_info = 
        domination_member->FindMember(requester_guid);
    if (request_member_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Request member is not found.|char guid = %d|"), 
            __FUNCTION__, 
            requester_guid
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }
    
    const DominationMemberInfo* leave_member_info(NULL);
    if (requester_is_master)
    {
        leave_member_info = domination_member->FindMember(leave_char_guid);        
    }
    else
    {
        leave_member_info = request_member_info;
    }
    if (leave_member_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Leave member is not found.|char guid = %d|"), 
            __FUNCTION__, 
            leave_char_guid
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }

    const GUILDGUID requester_guild_guid = requester->GetGuildGuid();
    if (requester_is_master)
    {        
        if (leave_member_info->guild_type_)
        {
            if (domination_area->IsAttackGuild(requester_guild_guid) == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|[Domination] Guild is different.|requester guild guid = %d, requester guid = %d, leave char guid = %d|"), 
                    __FUNCTION__, 
                    requester_guild_guid, 
                    requester_guid, 
                    leave_char_guid
                );
                response_sender.result = RC::DOMINATION_RESULT_FAIL;
                return;
            }
        }
        else
        {
            if (domination_area->IsDeffenseGuild(requester_guild_guid) == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|[Domination] Guild is different.|requester guild guid = %d, requester guid = %d, leave char guid = %d|"), 
                    __FUNCTION__, 
                    requester_guild_guid, 
                    requester_guid, 
                    leave_char_guid
                );
                response_sender.result = RC::DOMINATION_RESULT_FAIL;
                return;
            }
        }
        if (requester->GetCharInfo()->m_eGuildDuty != eGUILD_DUTY_MASTER)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Requester is not guild master.|guild guid = %d, requester guid = %d|"), 
                __FUNCTION__, 
                requester_guild_guid, 
                requester_guid
            );
            response_sender.result = RC::DOMINATION_RESULT_FAIL;
            return;
        }
    }
    
    const GameGuild* const requester_guild = g_GameGuildManager.FindGuild(requester_guild_guid);
    if (requester_guild == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Guild is not found.|guild guid = %d, requester guid = %d|"), 
            __FUNCTION__, 
            requester_guild_guid, 
            requester_guid
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }
    const GameGuildMember* const requester_guild_member = requester_guild->FindMember(requester_guid);
    if (requester_guild_member == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Requester is not guild member.|guild guid = %d, requester guid = %d|"), 
            __FUNCTION__, 
            requester_guild_guid, 
            requester_guid
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }

    if (requester_is_master)
    {
        if (requester_guild_member->GetDuty() != eGUILD_DUTY_MASTER)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Requester is not guild master.|guild guid = %d, requester guid = %d|"), 
                __FUNCTION__, 
                requester_guild_guid, 
                requester_guid
            );
            response_sender.result = RC::DOMINATION_RESULT_FAIL;
            return;
        }   
        const GameGuildMember* const leave_guild_member = requester_guild->FindMember(leave_char_guid);
        if (leave_guild_member == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Leave char is not guild member.|guild guid = %d, char guid = %d|"), 
                __FUNCTION__, 
                requester_guild_guid, 
                leave_char_guid
            );
            response_sender.result = RC::DOMINATION_RESULT_FAIL;
            return;
        }
    }
    
    if (domination_manager->IsDeleteMember(request->map_code_) == false)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Domination event state is not delete member.|map code = %d, requester guid = %d|"), 
            __FUNCTION__, 
            request->map_code_, 
            requester_guid
        );
        response_sender.result = RC::DOMINATION_RESULT_FAIL;
        return;
    }    
}

VOID Handler_CG_BATTLE::OnCG_DOMINATION_REFRESH_MEMBERLIST_REQ(ServerSession* server_session, 
                                                               MSG_BASE* msg, WORD size)
{
    __TOUCHED((server_session, msg, size));
    const MSG_CG_DOMINATION_REFRESH_MEMBERLIST_REQ* recv_msg = 
        static_cast<MSG_CG_DOMINATION_REFRESH_MEMBERLIST_REQ *>(msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player");
        return;
    }

    GameField* field = player->GetField();
    if (field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found field by player");
        return;
    }

    const GameDominationManager* const domination_manager = GameDominationManager::Instance();

    domination_manager->SendMemberList(player, recv_msg->map_code_);
}
