#include "stdafx.h"
#include "Player.h"

#include <SCSlotContainer.h>
#include <SCItemSlotContainer.h>
#include <SCSKillSlotContainer.h>

#include <PacketStruct_DG.h>
#include <PacketStruct_AG.h>
#include <PacketStruct_GZ.h>
#include <MapInfoParser.h>
#include <CharInfoParser.h>
#include <ExpValueInfoParser.h>
#include <CharacterFormula.h>

//==================================================================================================
// GameServer
#include "GameServerEx.h"

#include "ItemManager.h"
#include "WarehouseSlotContainer.h"
#include "EquipmentSlotContainer.h"
#include "InventorySlotContainer.h"
#include "VendorSlotContainer.h"

#include "Map.h"
#include "GameZone.h"

// Player
#include "MissionManager.h"
#include "QuestManager.h"
#include "QuickManager.h"
#include "QuickStyleManager.h"
#include "./SkillSystem/PerkManager.h"
#include "SKillSlotManager.h"
#include "StatusManager.h"
#include "QuestManager_Concrete.h"
#include <AreaConquestCommonHeader.h>
#include "QuestInfoManagerEx.h"
#include "StatisticsManager.h"
#include "GameCHuntingRoom.h"
#include "GameInstanceDungeon.h"
#include "CHuntingManager.h"
#include "ChaoState.h"
#include "MoveStateControl.h"
#include "PlayerRider.h"
// Object
#include "Collection.h"
#include "CollectedList.h"
// GameGuild
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "GameGuildExtension.h"
//
#include "GMCmdManager.h"
//
#include "DragonTransStatus.h"
// Party
#include "GameParty/NewGameParty.h"
#include "GameParty/NewGamePartyManager.h"
//
//==================================================================================================
// Contents
//
// Domination
#include "GameDominationManager.h" //_NA_0_20100208_DOMINATION_AREA_INFO
#include "FriendInvitation/GFriendInvitationController.h"

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
#include "./SkillSystem/SkillCoolTimeAccessor.h"
#endif

#include "ItemExpDistributor.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include <ServerOptionParserEx.h>
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
#include "PeakTimeEvent.h"
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT


// CHANGES, f100827.4L, this is extracted from the SCPlayer
// this method is called by a 'DG_CHARINFO_CHAR_ITEM_CMD' protocol handler
void Player::SetCharInfo(BASE_PLAYERINFO* char_info)
{
    // NOTE, a next flow sequence is as follows.
    //  DG_CHARINFO_CHAR_ITEM_CMD ->
    //      Player::SetCharInfo ->
    //          (this flow is copied from a custom logic. removable)
    //          PlayerAttributeLinker::SetCharInfo ->
    //              Player::OnSetCharInfo (next method)
    attributes_linker_.SetCharInfo(char_info);
}

void Player::OnSetCharInfo()
{
    const eCHAR_TYPE char_type = GetCharType();
    const eCHAR_CONDITION char_condition = GetStatusManager()->GetCondition();
    //
    ;{
        // HP, MP, SD 회복률
        int recover_hp = CalcHPRecover(char_type, char_info_.m_sVitality,
                                       char_condition, char_info_.m_LV);
        int recover_mp = CalcMPRecover(char_type, char_info_.m_sSpirit,
                                       char_condition);
        int recover_sd = CalcSDRecover(char_type,
                                       char_condition,
                                       CHAR_ACTION_CONDITION_NONE,
                                       char_info_.m_LV);

        m_Attr.SetCharInfo(&char_info_, recover_hp, recover_mp, recover_sd);
        m_Attr.Update();
    };
    //
#if !defined(_NA002217_100820_EP2_SKILL_SYSTEM_VERSION_)
    m_pSkillSlotManager->ApplyStyleBuff(GetBaseSelectedStyle());
#endif
    //
    if(char_type == eCHAR_BERSERKER)
    {
        SetMP(0);

        // 자신에게 알려준다.
        MSG_CG_STATUS_CHANGE_MP_BRD MPMsg;
        MPMsg.m_dwObjectKey = GetObjectKey();
        MPMsg.m_dwTargetMP = GetMP();
        SendPacket(&MPMsg, sizeof(MPMsg));
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if (char_type == eCHAR_WITCHBLADE)
    {
        SetFP(0);

        // 알려준다.
        MSG_CG_STATUS_CHANGE_FP_CMD FPMsg;
        FPMsg.object_key = GetObjectKey();
        FPMsg.m_dwTargetFP = GetFP();
        SendPacket(&FPMsg, sizeof(FPMsg));
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // 다음 렙업 경험치를 저장해둔다.
    SetNextExp();

    // 피로도 시스템 초기화
#ifndef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    m_pFatigueManager->Init(this);
#endif

    // 인벤토리 Max Size결정 = TabNum* Num Of Slot Per Tab
    // (CHANGES) (WAVERIX) induce the local reference for the inventory of a player
    CInventorySlotContainer* inventory = static_cast<CInventorySlotContainer*>(\
        GetItemManager()->GetItemSlotContainer(SI_INVENTORY));
    // assert(MAX_INVENTORY_TAB_NUM + pInfo->m_ExtraInventoryTabCount <= 10)
    inventory->SetMaxSlotNum(\
        (MAX_INVENTORY_TAB_NUM + char_info_.m_ExtraInventoryTabCount) * MAX_INVENTORY_SLOT_PER_TAB);
    // (CHANGES) (f100328.6L) (WAVERIX) set inventory sorting insert configuration
    inventory->UpdateInventoryConfiguration(char_info_.m_InventoryConfig);
    
    GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT)->SetMaxSlotNum(MAX_EQUIPMENT_SLOT_NUM);
    // (CHANGES) (f100714.2L) pre-fetch to support a delayed event query process

#ifdef _NA_20100307_BETAKEY_SYSTEM
    SetBetaKeyInfo(char_info_.beta_key_);
#endif

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    //처음 입장했을때, gm경험치이벤트 진행중이면 경험치율 정보 보내준다
    PeakTimeEvent::Instance()->SendProgressEventExpRateInfo(this);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
    /*
    // 응모한 이벤트 정보를 요청
    MSG_DG_EVENT_ITEM_STATUS_SYN sendMsg;

    sendMsg.m_dwKey = char_info_.m_UserGuid;
    sendMsg.m_CharGUID = char_info_.m_CharGuid;

    SendToGameDBPServer(&sendMsg, sizeof(sendMsg));
    //MessageOut(eFULL_LOG, "User EventInfo Req[%u][C:%u]", sendMsg.m_dwKey, sendMsg.m_CharGUID);
    */
}

void
Player::SerializeSkillTotalInfo(SKILL_TOTAL_INFO& rSkillInfo, eSERIALIZE eType)
{
    // CHANGES f100825.2L, change a routine to a total serializing skill stream converter
    m_pSkillSlotManager->SerializeSkillStreamTotal(&rSkillInfo, eType);
/*
    if(eType == SERIALIZE_LOAD)
    {
        rSkillInfo.m_Count = 0;
        SKILLSLOT* pSkillSlot = rSkillInfo.m_Slot;
        POSTYPE total = 0;
        for(POSTYPE i=0;i<MAX_SKILL_SLOT_NUM;++i)
        {
            if(!m_pSkillSlotManager->IsEmpty(i))
            {
                pSkillSlot[total].m_Pos = i;
                m_pSkillSlotManager->SerializeSkillStream(pSkillSlot[total].m_Pos, &pSkillSlot[total].m_Stream,SERIALIZE_LOAD);
                ++total;
                ++rSkillInfo.m_Count;
            }
        }
    }
    else if(eType == SERIALIZE_STORE)
    {
        SKILLSLOT* pSlot = rSkillInfo.m_Slot;
        POSTYPE start = 0;
        POSTYPE total = rSkillInfo.m_Count;
        ASSERT(m_pSkillSlotManager->GetSkillSlotContainer()->GetSlotNum() == 0);
        for(POSTYPE i=start;i<total;++i)
            m_pSkillSlotManager->SerializeSkillStream(pSlot[i].m_Pos, &pSlot[i].m_Stream,SERIALIZE_STORE);
    }
*/
}

#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================
// CHANGES: f110420.5L, moved to the default condition routine
// to support extented renderable item selection
bool Player::IsRenderEquipItem(POSTYPE pos) const
{
    bool renderabled = CEquipmentSlotContainer::IsDefaultRenderableEquipItem(pos);
    //
    return renderabled;
}
#endif

//==================================================================================================
POSTYPE
Player::SerializeRenderEquipItemBaseInfo(EQUIP_ITEM_BASEINFO::data_type* const OUT result_slot_list) const
{
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    typedef CEquipmentSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;

    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
        m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT));
    //
    const ns_formula::EquipSlotsManageInfo* manage_info = equip_container->GetEquipmentManageInfo();
    //----------------------------------------------------------------------------------------------
    EQUIP_ITEM_BASEINFO::data_type* dest_it = result_slot_list;
    //
    const SLOT_LINEAR_LIST& slot_list = equip_container->GetSlotList();
    SLOT_LINEAR_LIST::const_iterator slot_it = slot_list.begin();
    const SLOT_LINEAR_LIST::const_iterator slot_end = slot_list.end();
    for (int pos = 0; slot_it != slot_end; ++pos, ++slot_it)
    {
        const SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot_it->slot);
        if (item_slot->GetItemCode() == 0) {
            continue;
        };
        if (CEquipmentSlotContainer::kDefaultRenderableEquipItems[pos] == false) {
            continue;
        };
        if (const ns_formula::EquipSlotsManageInfo::Node* node = &manage_info->slots[pos]) {
            if (node->activated == false) {
                continue;
            };
        };
        //-------------------------------------------
        if (EQUIP_ITEM_BASEINFO::data_type* target = dest_it++)
        {
            target->m_Pos = static_cast<POSTYPE>(pos);
            item_slot->CopyOut(target->m_Stream);
        };
        //-------------------------------------------
    };//end 'foreach slot_list'
    //
    const POSTYPE number_of_real_renderable_equips =
        static_cast<POSTYPE>(dest_it - result_slot_list);
    return number_of_real_renderable_equips;
    //----------------------------------------------------------------------------------------------
#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    POSTYPE total = 0;
    for (POSTYPE i = 0; i < MAX_EQUIPMENT_SLOT_NUM; ++i)
    {
        if(m_pItemManager->IsEmpty(SI_EQUIPMENT, i) == false && IsRenderEquipItem(i))
        {
            result_slot_list[total].m_Pos = i;
            m_pItemManager->SerializeItemStream(SI_EQUIPMENT,
                result_slot_list[total].m_Pos, &result_slot_list[total].m_Stream, SERIALIZE_LOAD);
            ++total;
        }
    }
    return total;
#endif
}
//
//==================================================================================================
//
int
Player::GetPlayerRenderInfo(BOOLEAN is_player_render, PLAYER_RENDER_INFO_EX* OUT pack_info)
{
    const WzVector* pPos = GetPosPtr();
    const BASE_PLAYERINFO* pInfo = &char_info_;
    const Attributes& rAttr = GetAttr();

    PLAYER_RENDER_INFO_EX render_temp;
    // (WARNING) C_COLLECTKEY의 생성자 주의할 필요 있음...-.-;;;;
    ZeroMemory(&render_temp, sizeof(render_temp));
    render_temp.is_player_render = !!is_player_render;

    render_temp.player_key = (WORD)GetObjectKey();
    render_temp.hp      = (PLAYERHP)GetHP();
    render_temp.max_hp  = (PLAYERHP)GetMaxHP();
    render_temp.level   = pInfo->m_LV;

    strncpy(render_temp.char_name, pInfo->m_tszCharName, _countof(render_temp.char_name));
    render_temp.char_name[_countof(render_temp.char_name) - 1] = '\0';

#ifdef _NA_000000_20121120_NONAME_IN_SSQ
    if (GameZone* const pGameZone = GetGameZonePtr())
    {
        if( pGameZone->GetZoneType() == eZONETYPE_INSTANCE )
        {
            GameInstanceDungeon* const pDungeon = static_cast<GameInstanceDungeon*>( pGameZone );
            if( pDungeon && pDungeon->GetTypeOfIndun() == INDUN_KIND_SS_QUEST ) {
                memset(render_temp.char_name, 0, sizeof(render_temp.char_name));
            }
        }
    }
#endif

    render_temp.position = *pPos;
    render_temp.selected_style_code = GetSelectedStyle();
    render_temp.move_speed_ratio    = (WORD)rAttr.GetMoveSpeedRatio();  // 이동속도(100을 곱한 %수치)
    render_temp.attack_speed_ratio  = (WORD)rAttr.GetAttSpeedRatio();   // 공격속도(100을 곱한 %수치)

    StatusManager* const pStatusManager = GetStatusManager();
    const StatusField& status_field = pStatusManager->Status;
    const STATE_GM_INFO& rGMInfo = GetGMStateInfo();
    //
    render_temp.hair_style = pInfo->m_byHair;     //6 = 6     머리모양

    //render_temp.transparent;    //2 = 8     투명( 0:일반, 1:GM투명상태, 2:섀도우스킬하이드)
    if(rGMInfo.m_TransOn)
        render_temp.transparent = 1;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (GetBattleGroundPlayerInfo().GetPlayerState() == kPlayerStateStandbyResurection)
        render_temp.transparent = 3;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    else if (GetGoldRushPlayerInfo().GetPlayerState() == kPlayerStateStandbyResurection)
        render_temp.transparent = 3;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    else if(status_field.IsPlayer_TransOn())
        render_temp.transparent = 2;
    else
        render_temp.transparent = 0;

    render_temp.height = pInfo->m_byHeight;  //3 = 11    키
    render_temp.face   = pInfo->m_byFace;   //4 = 13    얼굴모양
    render_temp.helmet = GetHelmetOption();  //1 = 14    투구 옵션(0: 보이기, 1: 안보이기)
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    render_temp.wing = GetWingOption();  //1 = 15    날개 (0: 보이기, 1: 안보이기)
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING


    //render_temp.gm_grade;       //3 = 17    GM 등급( 0: 일반, 1이상: GM, 4 : 가상아이디 )
    switch(const BYTE sts = GetSts())
    {
    case eGM_GRADE_FIRST: case eGM_GRADE_SECOND: case eGM_GRADE_THIRD:
        render_temp.gm_grade = sts - eGM_GRADE_BASE;
        break;
    case eGM_GRADE_VIRTUAL:
        render_temp.gm_grade = 4;
        break;
    }

    render_temp.pcroom          = !!GetReservedValue();//1 = 18    PC방 유저(0: 일반, 1 : PC방)
    render_temp.char_condition  = (BYTE)pStatusManager->GetCondition(); //1 = 19    현재상태(앉기/서기) eCHAR_CONDITION
    render_temp.char_class      = pInfo->m_byClassCode;     //3 = 22    eCHAR_TYPE

    ChaoState& rChaoState = GetChaoState();
    render_temp.boradori     = !!rChaoState.IsBoradori();       //1 = 23    보라돌이 상태 (t/f)
    render_temp.chao_state   = (BYTE)rChaoState.GetChaoState();//2 = 25    eCHAO_STATE = { 0, 1, 2 }
    render_temp.behave_state = (BYTE)GetBehave();   //5 = 30    ePLAYER_BEHAVE_STATE
#ifdef _NA_002050_20110216_ADD_GENDER
    render_temp.gender = pInfo->gender;
#endif
#ifdef _NA001955_110210_WING_COSTUME
    render_temp.spread_wings = (status_field.FIELD & status_field.ENABLE_SPREAD_WINGS) != 0;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    render_temp.honor_title = pInfo->honor_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM

    //_NA_004035_20120227_COSTUME_ITEM
    //메인코스튬
    render_temp.is_equipped_costume = pInfo->is_equipped_costume_;
    if (render_temp.is_equipped_costume)
    {
        CInventorySlotContainer* const inventory_container = 
            static_cast<CInventorySlotContainer*>(GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    

        const SCItemSlot& item_slot = 
            static_cast<SCItemSlot&>( inventory_container->GetSlot(pInfo->costume_item_pos_) );

        const BASE_ITEMINFO* const costume_item_info = item_slot.GetItemInfo();

        if (costume_item_info)
        {
            render_temp.costume_item_code = costume_item_info->m_Code;
        }
    }
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    //데코 코스튬
    render_temp.is_equipped_decorate_costume = pInfo->is_equipped_decorate_costume_;
    if (render_temp.is_equipped_decorate_costume)
    {
        CInventorySlotContainer* const inventory_container = 
            static_cast<CInventorySlotContainer*>(GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    
        const SCItemSlot& item_slot = 
            static_cast<SCItemSlot&>( inventory_container->GetSlot(pInfo->decorate_costume_item_pos_) );

        const BASE_ITEMINFO* const deco_costume_item_info = item_slot.GetItemInfo();

        if (deco_costume_item_info)
        {
            render_temp.decorate_costume_item_code = deco_costume_item_info->m_Code;
        }
    }
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    render_temp.costume = GetCostumeOption();
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    render_temp.spa_ticket_code = GetPlayerSpaInfo()->GetSpaTicketCode();
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    CEquipmentSlotContainer* const equip_container = 
        static_cast<CEquipmentSlotContainer*>(GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    
    const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
        equip_container->GetEquipmentManageInfo();
    
    const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
        equip_manage_info->item_score_info;

    render_temp.total_item_score_ = item_score_info.GetTotalScore();
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE
    //

    //----------------------------------------------------------------------------------------------
    // <REF> 렌더로 보여야 할 다양한 비주얼 효과 정의
    render_temp.special_effects = RENDER_EFFECT.GetEffect();

    //----------------------------------------------------------------------------------------------
    //__NA_0_PET_SYSTEM, __NA_1131_PET_NAME_SETTING(901)
    render_temp.is_summoned_pet = !!m_PetManager.AlreadySummonedPet();
    if(render_temp.is_summoned_pet) {                   // (f100524.2L)
        const sPet_Script_Info* script_info = m_PetManager.GetImagePetScriptInfo();
        render_temp.pet_index    = script_info->m_wIndexCode;
        render_temp.pet_fullness = m_PetManager.GetFullness();
        render_temp.pet_intimacy = m_PetManager.GetIntimacy();
        if(m_PetManager.IsSetPetName()) {
            const char* pet_name = m_PetManager.GetPetName();
            if(pet_name) { // (NOTE) 일전에 null이 발생한 사례가 있다. 일단 곳곳에 이런 처리를 수행한지라 일단 유지한다.
                strncpy(render_temp.pet_name, pet_name, _countof(render_temp.pet_name));
                render_temp.pet_name[_countof(render_temp.pet_name) - 1] = '\0';
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    //__NA_1119_CHANGE_ETHER_EXPEND_STYLE
    if(m_EtherManager.IsActive()) {
        tagEtherBulletInfo* pBulletInfo = m_EtherManager.GetBulletOption();
        if(pBulletInfo)
            render_temp.ether_device_index  = pBulletInfo->m_byEffectCode;
    }

    //----------------------------------------------------------------------------------------------
    if(render_temp.is_riding = status_field.IsRidingRider()) {
        Rider->GetRiderItemStream(&render_temp.rider_item_slot);
    }
    //----------------------------------------------------------------------------------------------
    //render_temp.collection_render_info;
    if(m_pCollectedList->IsCollecting()) {
        Collection* pCollection =
            (Collection*)GetField()->FindObject(COLLECTION_OBJECT,
                                                m_pCollectedList->GetObjKeyCollecting());
        if(pCollection) {
            COLLECT_PLAYER_RENDER_INFO& collection_render_info = render_temp.collection_render_info;
            pCollection->GetRenderInfo(collection_render_info);
            SUNLOG(eDEV_LOG,
                   "[Collect COLLECT_PLAYER_RENDER_INFO 정보 보냄] "
                   "케릭명[%s] ObjectKey[%d] CollectionID[%d] RemainTime[%d]",
                   render_temp.char_name,
                   collection_render_info.m_dwObjectKey,
                   collection_render_info.m_CollectionID,
                   collection_render_info.m_dwRemainTime);
        }
    }
    //----------------------------------------------------------------------------------------------
    // STATE_INFO를 채운다. - 마을에서는 드래곤 변신 외에는 상태 정보를 보내지 않는다.
    pStatusManager->SerialzieStatusInfo(render_temp.state_total_info, !is_player_render);

    int written_len = render_temp.ConvertForSend(is_player_render, render_temp, pack_info);

    return written_len;
}

//{__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
BOOL
Player::GetGuildRenderInfo(GUILD_RENDER_INFO* OUT pRenderInfo, GUILDGUID other_guildIndex) const
{
    const BASE_PLAYERINFO* pInfo = &char_info_;
    GameGuild* pGuild = g_GameGuildManager.FindGuild(pInfo->m_GuildGuid);
    if( NULL == pGuild)
        return FALSE;

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    pRenderInfo->m_dwObjectKey  = GetObjectKey();
    pRenderInfo->m_GuildGrade   = pGuild->GetGuildGrade();
    //_NA_0_20100208_DOMINATION_AREA_INFO
    pRenderInfo->m_GuildDuty    = (BYTE)pInfo->m_eGuildDuty;
    pRenderInfo->m_GuildGuid    = pInfo->m_GuildGuid;
    pRenderInfo->m_RelationType = GuildSmallContentEntry::RelationSystem::GetRelationType(
        other_guildIndex, pInfo->m_GuildGuid);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    pRenderInfo->sonnenschein_phase = pGuild->GetSonnenScheinRewardPhase();
#endif //_NA_008334_20150608_SONNENSCHEIN
    
    GuildSmallContentEntry::GuildMark::GetGuildMark(pInfo->m_GuildGuid,
        pRenderInfo->m_BackGroundIndex, pRenderInfo->m_PatternIndex);
    util::GUILDNAMECOPY(pRenderInfo->m_tszGuildName, pInfo->m_tszGuildName);

    //_NA_0_20100208_DOMINATION_AREA_INFO
    domination_manager->SerializeAreaInfo(pInfo->m_GuildGuid, pRenderInfo->m_stateTotalInfo);
    return TRUE;
}
//}


void
Player::GetVendorRenderInfo(VENDOR_RENDER_INFO* OUT pRenderInfo)
{
    pRenderInfo->m_byTitleLength = 0;
    if(GetBehave() == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
    {
        VendorSlotContainer* pVenderSlotContainer = GetVendorContainer();
        if(pVenderSlotContainer == 0)
        {
            SUNLOG(eCRITICAL_LOG, "[Error]GetVendorContainer() == NULL");
        }
        else
        {
            strncpy(pRenderInfo->m_tszVendorTitle, pVenderSlotContainer->GetTitle(),
                    _countof(pRenderInfo->m_tszVendorTitle));
            pRenderInfo->m_tszVendorTitle[_countof(pRenderInfo->m_tszVendorTitle) - 1] = '\0';
            pRenderInfo->m_byTitleLength = strlen(pRenderInfo->m_tszVendorTitle);
        }
    }
}
//
//==================================================================================================

BOOL
Player::SerializeInfoToDBProxy(eGUILDWAREHOUSE_UPDATE_FACTOR updateFactor)
{
    // 주기적으로 DBProxy로 보내는 정보 : 1. 캐릭터 정보(m_CharInfo)    2.아이템   3. 스킬
    // 최적화 할 필요가 있는 부분!
    // - 변경된 부분을 보내는 방법 고려!
    // changes, f100818.2L, code arrangement
    ePLAYER_STATE player_lifetime_state = ePLAYER_STATE(GetState());
    if (player_lifetime_state < PLAYER_LOADING || player_lifetime_state == PLAYER_DESTROYED) {
        return true;
    }
    //
    m_DBPUpdateTimer.Reset();
    //
    // CHANGES: f101115.3L, make an independent method
    // for stable player closing process using shared data.
    if (updateFactor == UPDATE_FACTOR_LOGOUT) {
        CloseSharedTransactions();
        // CHANGES: f110103.2L, the result of IsTransaction is true only
        // when a interfield moving is failed.
        if (zone_transaction_info_.IsTransaction()) {
            zone_transaction_info_.RollbackTransaction();
        }
    }
    //
    //////////////////////////////////////////////////////////////////////////
    //  Serialize Temporary Buffer
    static BYTE busy_access_buffer_static[1024 * 64];
    //////////////////////////////////////////////////////////////////////////
    const BYTE channel_id = g_pGameServer->GetKey().GetChannelID();
    const DWORD user_key = GetUserKey();
    const DWORD char_guid = GetCharGuid();
    //
    MSG_DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN msg_transaction;
    ;{  // start transaction for an update locking
        msg_transaction.m_byTransControl = msg_transaction.SERIALIZE_LOCKING;
        msg_transaction.m_ChannelID = channel_id;
        msg_transaction.char_guid_ = char_guid; // CHANGES: f110412.2L
        if (SendToGameDBPServer(&msg_transaction, sizeof(msg_transaction)) == false)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|Msg=can't send to the database proxy server, network problem|"),
                   __FUNCTION__);
            return false;
        }
    };
    //
    ;{
        MSG_DG_CHARINFO_CHAR_ITEM_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_CHAR_ITEM_CMD;
        msg_cmd->m_ChannelID = channel_id;
        msg_cmd->m_CharacterInfo = char_info_;
        m_pItemManager->SerializeItemTotalInfo(msg_cmd->m_ItemInfo, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
#if defined(_NA002217_100728_EP2_SKILL_SYSTEM_DB_) && \
    defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    ;{
        //MSG_DG_CHARINFO_SKILL_LIST_CMD msg_cmd;
    };
    ;{
        MSG_DG_CHARINFO_SKILL_PERK_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_SKILL_PERK_CMD;
        msg_cmd->char_guid_ = char_guid;
        this->GetPerkManager()->LoadPerk(&msg_cmd->container_);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
#endif
    // { reserve deletion
    ;{
        MSG_DG_CHARINFO_SKILL_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_SKILL_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        SerializeSkillTotalInfo(msg_cmd->m_SkillInfo, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
    ;{
        MSG_DG_CHARINFO_QUICK_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_QUICK_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        m_pQuickManager->SerializeQuickTotalInfo(msg_cmd->m_QuickInfo, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    ;{
        MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        SkillCoolTimeAccessor::Instance()->GetSkillCoolTime(this, &(msg_cmd->skill_cool_time));
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
#endif

    // } reserve deletion
    ;{
        MSG_DG_CHARINFO_QUEST_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_QUEST_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        //
        BOOST_STATIC_ASSERT(sizeof(msg_cmd->m_pProgrQStream) == MAX_PROGR_QUESTSTREAM_SIZE);
        BOOST_STATIC_ASSERT(sizeof(msg_cmd->m_pComplQStream) == MAX_COMPL_QUESTSTREAM_SIZE);
        BOOST_STATIC_ASSERT(sizeof(msg_cmd->m_pMissionStream) == MAX_MISSIONSTREAM_SIZE);
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        BOOST_STATIC_ASSERT(sizeof(msg_cmd->m_pACStream) == MAX_ACSTREAM_SIZE);
#endif
        //
        m_pQuestManager->SerializeProgrStream(\
            msg_cmd->m_pProgrQStream, sizeof(msg_cmd->m_pProgrQStream), QSERIALIZE_LOAD);
        m_pQuestManager->SerializeComplStream(\
            msg_cmd->m_pComplQStream, sizeof(msg_cmd->m_pComplQStream), QSERIALIZE_LOAD);
        m_pMissionManager->SerializeStream(\
            msg_cmd->m_pMissionStream, sizeof(msg_cmd->m_pMissionStream), SERIALIZE_LOAD);
#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED	
        m_pACManager->Serialize(\
            msg_cmd->m_pACStream, sizeof(msg_cmd->m_pACStream), ACSERIALIZE_LOAD);
#endif
        //
        SendToGameDBPServer(msg_cmd, sizeof(*msg_cmd));
    };
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED	
	;{
		MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD* msg_cmd = \
			new (busy_access_buffer_static) MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->channel_id_ = channel_id;
		m_pACManager->Serialize(msg_cmd->achievement_info_, ACSERIALIZE_DBPROXY_LOAD);
		SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
	};
#endif
    ;{
        MSG_DG_CHARINFO_EXTRA_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_EXTRA_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        msg_cmd->m_LoginTime_t = GetLoginTime_t();
        SerialzieStatusGMInfo(msg_cmd->m_GMStateInfo, SERIALIZE_LOAD);
        m_pStatusManager->SerialzieStatusDetailInfo(msg_cmd->m_TotalInfo, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
    //
    ;{
        // 창고가 시작중이면 DB로 현재까지의 정보를 저장하도록 한다.
        CWarehouseSlotContainer* const warehouse = GetWarehouseContainer();
        warehouse->SaveWarehouseToDBP();
    };
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    ;{
        MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->m_ChannelID = channel_id;
        msg_cmd->m_RepurchasItem.m_Count = 0;
        m_pItemManager->GetTradingImpl()->SerializeRepurchaseItems(
            msg_cmd->m_RepurchasItem, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496
    ;{  // implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
        MSG_DG_CHARINFO_INVITATEDFRIEND_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARINFO_INVITATEDFRIEND_CMD;
        msg_cmd->char_guid_ = char_guid;
        msg_cmd->channel_id_ = channel_id;
        GFriendInvitationController* invite_controller = GFriendInvitationController::Instance();
        invite_controller->GetInvitatedFrindInfo(this, msg_cmd, SERIALIZE_LOAD);
        SendToGameDBPServer(msg_cmd, msg_cmd->GetSize());
    };
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    ;{
        MSG_DG_OTHER_FATIGUE_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_OTHER_FATIGUE_CMD;
        msg_cmd->m_wPlayTime = GetPlayTime();
        msg_cmd->m_wOfflineTime = GetOfflineTime();
        SendToGameDBPServer(msg_cmd, sizeof(*msg_cmd));
    };
#endif  //__CN_OO0510__FATIGUE_MATCHING_UPDATE
    ;{
        MSG_DG_CHARACTER_EXTRA_INFO_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_CHARACTER_EXTRA_INFO_CMD;

#ifdef _NA_006372_20130108_BADGE_ITEM
        ToggleItemInfos& toggle_item_info = GetExtraInfoManager().GetToggleItemInfos();
        memcpy(&msg_cmd->toggle_item_info_, &toggle_item_info, sizeof(msg_cmd->toggle_item_info_));
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        const BattleGroundRecordInfos& battle_ground_record_infos = \
            GetExtraInfoManager().GetBattleGroundRecordInfos();
        memcpy(&msg_cmd->battle_ground_record_infos_, &battle_ground_record_infos, 
                sizeof(msg_cmd->battle_ground_record_infos_));
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        const GoldRushRecordInfos& goldrush_record_infos = \
            GetExtraInfoManager().GetGoldRushRecordInfos();
        memcpy(&msg_cmd->goldrush_record_infos_, &goldrush_record_infos, 
            sizeof(msg_cmd->goldrush_record_infos_));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
        msg_cmd->daily_quest_reset_item_use_time = GetExtraInfoManager().GetDailyQuestResetUseTime();
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        msg_cmd->minigame_bingo_infos = GetExtraInfoManager().GetMiniGameBingoInfos();
#endif //_NA_008404_20150712_MINIGAME_SYSTEM

        SendToGameDBPServer(msg_cmd, sizeof(*msg_cmd));
    };

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    ;{
        MSG_DG_USER_EXTRA_INFO_CMD* msg_cmd = \
            new (busy_access_buffer_static) MSG_DG_USER_EXTRA_INFO_CMD;
        // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        const AttendanceInfo& attendance_infos = \
            GetExtraInfoManager().GetAttendanceInfos();
        memcpy(&msg_cmd->attendance_infos, &attendance_infos, 
            sizeof(msg_cmd->attendance_infos));
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        SendToGameDBPServer(msg_cmd, sizeof(*msg_cmd));
    };
#endif // _NA_007932_20150106_USER_EXTRA_INFO

    //
    //
    BYTE end_trans_type_selector = msg_transaction.SERIALIZE_UNLOCKING;
    if (CGuildWarehouseSlotContainer* guild_warehouse = GetGuildWarehouseContainer())
    {
        // 1. 창고를 오픈한 상태로 오랬동안 있는 경우
        // 2. 길드 창고를 열고 있는 상태에서 업데이트 하는 경우
        // 3. 길드 창고를 열고 있는 상태에서 로그아웃을 하려는 경우(서버선택/캐릭선택 화면으로)
        // 4. 서버 선택창 IsLogOut == true 일때는 DBP Disconnect 패킷이 전송되어 DBP에서 바로 업데이트가 되므로 길드 창고 트랜젝션을 걸 필요가 없다.
        // 5. 캐릭 선택창 IsLogOut == true 일때는 MSG_DG_CHARINFO_UPDATE_SYN 패킷이 전송되어 DBP에서 바로 업데이트가 되므로 길드 창고 트랜젝션을 걸 필요가 없다.
        // TODO : the name of a UPDATE_RESULT enumeration is very generic
        UPDATE_RESULT guild_update_result = guild_warehouse->SaveWarehouseToDBP(this, updateFactor);
        if (guild_update_result == UPDATE_BY_CHNAGE_SUCCESS)
        {
            if (UPDATE_FACTOR_LOGOUT != updateFactor) {
                end_trans_type_selector = msg_transaction.SERIALIZE_UNLOCKING_WITH_GUILDWAREHOUSE;
            }
            SUNLOG(eCRITICAL_LOG, "SerializeInfoToDBProxy With GuildWarehouse(%u)(%u)",
                   char_guid, GetGuildGuid());
        }
    };
    //------------------------------------------------------------------------------------------
    // WARNING!! MUST be set LAST position a below section about an unlocking transaction message
    ;{
        // send to the end transaction message for an update unlocking
        // 이 패킷은 맨 마지막에 보내도록 할 것.
        msg_transaction.m_byTransControl = end_trans_type_selector;
        msg_transaction.m_ChannelID = channel_id;
        SendToGameDBPServer(&msg_transaction, sizeof(msg_transaction));
    };
    //
    return true;
}

void
Player::CopyToCharInfo(PLAYERINFO_FOR_PACKET& rCharInfoOut)
{
    // Hero(자기자신)인 경우에 수치들은 Base수치를 보낸다.
    // 클라에서 스킬, 아이템, 스타일 등등에 의한 수치를 다시 계산한다.
    const BASE_PLAYERINFO* pInfo = &char_info_;
    const Attributes& rAttr = GetAttr();

    rCharInfoOut.m_i64Exp = pInfo->m_i64Exp;
    rCharInfoOut.m_MaxHP  = rAttr.GetBaseMaxHP();
    rCharInfoOut.m_HP     = pInfo->m_iHP;
    rCharInfoOut.m_MaxMP  = rAttr.GetBaseMaxMP();
    rCharInfoOut.m_MP     = pInfo->m_iMP;
    rCharInfoOut.m_Money  = pInfo->m_Money;
    rCharInfoOut.m_dwRemainStat  = pInfo->m_iRemainStat;
    rCharInfoOut.m_dwRemainSkill = pInfo->m_iRemainSkill;
    // CHANGES: f110704.3L, synchronize the default style.
    // The Episode II skill system does not use the passive skill.
    // NOTE: the side-effect of the SynchronizeStyleBasedOnWeapon is
    // to change char_info_.m_iSelectStyleCode.
    rCharInfoOut.m_SelectStyleCode = this->SynchronizeStyleBasedOnWeapon();

    rCharInfoOut.m_byPKState   = pInfo->m_byPKState;
    rCharInfoOut.m_byCharState = pInfo->m_byCharState;
    rCharInfoOut.m_StateTime   = pInfo->m_StateTime;
    // no null terminated?
    strncpy(rCharInfoOut.m_tszTitleID, pInfo->m_tszTitleID, _countof(rCharInfoOut.m_tszTitleID));
    //rCharInfoOut.m_tszTitleID[_countof(rCharInfoOut.m_tszTitleID) - 1] = '\0';
    rCharInfoOut.m_TitleTime = pInfo->m_TitleTime;

    ProtectorInventory& rProtector = GetWarehouseInventoryProtector().INVENTORY();
    int iFlag = (rProtector.IsEmptyPassword() ? rCharInfoOut.EMPTY_PWD : rCharInfoOut.EXIST_PWD);
        iFlag |= rProtector.IsUsable() ? rCharInfoOut.ENABLE : rCharInfoOut.DISABLED;
    rCharInfoOut.m_iInventoryLock = (PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE)iFlag;

    rCharInfoOut.m_sStrength    = rAttr.GetBaseSTR();
    rCharInfoOut.m_sDexterity   = rAttr.GetBaseDEX();
    rCharInfoOut.m_sVitality    = rAttr.GetBaseVIT();
    rCharInfoOut.m_sInteligence = rAttr.GetBaseINT();
    rCharInfoOut.m_sSpirit      = rAttr.GetBaseSPR();
    rCharInfoOut.m_sSkillStat1  = pInfo->m_sSkillStat1;
    rCharInfoOut.m_sSkillStat2  = pInfo->m_sSkillStat2;
    rCharInfoOut.m_UserPoint    = pInfo->m_UserPoint;
    rCharInfoOut.m_wMoveSpeedRatio = rAttr.GetMoveSpeedRatio();
    rCharInfoOut.m_wAttSpeedRatio  = rAttr.GetAttSpeedRatio();

    // GM 등급 셋팅
    switch(GetSts())
    {
    case eGM_GRADE_FIRST:
    case eGM_GRADE_SECOND:
    case eGM_GRADE_THIRD:
        {
            rCharInfoOut.m_byGMGrade = GetSts() - eGM_GRADE_BASE;
        }break;
    case eGM_GRADE_VIRTUAL:
        {
            rCharInfoOut.m_byGMGrade = 4;
        }break;
    default:
        {
            rCharInfoOut.m_byGMGrade = 0;
        }
    }

    // 현재 상태
    rCharInfoOut.m_byCondition  = GetStatusManager()->GetCondition();

    // PC방 유저
    rCharInfoOut.m_byPCBangUser = GetReservedValue() > 0 ? 1 : 0;

    // 키, 얼굴, 머리
    //rCharInfoOut.m_byHeight = pInfo->m_byHeight;
    //rCharInfoOut.m_byFace   = pInfo->m_byFace;
    //rCharInfoOut.m_byHair   = pInfo->m_byHair;
    rCharInfoOut.m_PlayLimitedTime    = pInfo->m_PlayLimitedTime;
    rCharInfoOut.m_byInvisibleOptFlag = pInfo->m_byInvisibleOptFlag;

    rCharInfoOut.m_GuildGuid = pInfo->m_GuildGuid;
    rCharInfoOut.m_GuildDuty = pInfo->m_eGuildDuty;
    _tcsncpy(rCharInfoOut.m_tszGuildName, pInfo->m_tszGuildName, MAX_GUILDNAME_LENGTH);

    rCharInfoOut.m_ChaoState   = pInfo->m_ChaoState;    // Chao
    rCharInfoOut.m_byPCRoomSts = GetPCRoomSts();

    rCharInfoOut.m_ExtraInventoryTabCount  = pInfo->m_ExtraInventoryTabCount;
    rCharInfoOut.m_ExtraEquipCashSlotCount = pInfo->m_ExtraEquipCashSlotCount;
    rCharInfoOut.m_bTradeFlag   = pInfo->m_bTradeFlag;
    rCharInfoOut.m_bWhisperFlag = pInfo->m_bWhisperFlag;
    rCharInfoOut.m_byFreeInitCount = pInfo->m_byFreeInitCount;

    rCharInfoOut.m_bSummonedPet = pInfo->m_bSummonedPet;
    rCharInfoOut.m_bMAXIntimacy = pInfo->m_bMAXIntimacy;
    rCharInfoOut.m_PetItemPos   = pInfo->m_PetItemPos;
    rCharInfoOut.m_bMAXOptionRemainTime = pInfo->m_bMAXOptionRemainTime;

    //rCharInfoOut.m_bActiveEtherDevice = m_EtherManager.IsActive();
    //클라는 다르게 보내기 위해 패킷을 중간에 변경시킴
    if (m_EtherManager.IsActive())
    {
        tagEtherBulletInfo* pBulletInfo = m_EtherManager.GetBulletOption();
        if (pBulletInfo) {
            rCharInfoOut.m_bActiveEtherDevice = pBulletInfo->m_byEffectCode;
        }
        else {
            rCharInfoOut.m_bActiveEtherDevice = 1;
        }
    }
    else
    {
        rCharInfoOut.m_bActiveEtherDevice = 0;
    }
    if (rCharInfoOut.m_bActiveEtherDevice) {
        rCharInfoOut.m_byEtherBulletPos  = m_EtherManager.GetBulletItemPos();
    }
    rCharInfoOut.m_MaxSD				= GetAttr().GetBaseMaxSD();
    rCharInfoOut.m_SD					= pInfo->m_iSD;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    rCharInfoOut.m_wHonorTitle               = pInfo->honor_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
}

const BASE_ITEMINFO*
Player::GetEquipItemInfo(eEQUIPCONTAINER_POS pos) const
{
    SASSERT(m_pItemManager != NULL, "[Player::GetEquipItemInfo] m_pItemManager == NULL");

    SCSlotContainer* pContainer = m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT);
    SASSERT(pContainer != NULL, "[Player::GetEquipItemInfo] pContainer == NULL");

    if(!pContainer->IsEmpty(pos))
    {
        SCItemSlot& rSlot = static_cast<SCItemSlot&>(pContainer->GetSlot(pos));
        return rSlot.GetItemInfo();
    }

    return NULL;
}

SCItemSlot*
Player::GetEquipItemSlot(eEQUIPCONTAINER_POS pos)
{
    SCSlotContainer* pContainer = m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT);
    if(pContainer == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[Player::GetEquipItemInfo] pContainer == NULL");
        return NULL;
    }

    if(!pContainer->IsEmpty(pos))
    {
        SCItemSlot& rSlot = static_cast<SCItemSlot&>(pContainer->GetSlot(pos));
        return &rSlot;
    }

    return NULL;
}

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
eARMOR_TYPE
Player::GetArmorType() const
{
    const BASE_ITEMINFO* pInfo = GetEquipItemInfo(eEQUIPCONTAINER_ARMOR);

    if(pInfo != NULL)
    {
        return (eARMOR_TYPE)pInfo->m_byArmorType;
    }

    return eARMOR_UNARMOR;
}

eMELEE_TYPE
Player::GetMeleeType() const
{
    const BASE_ITEMINFO* pInfo = GetEquipItemInfo(eEQUIPCONTAINER_WEAPON);

    if(pInfo != NULL)
        return (eMELEE_TYPE)pInfo->m_byMeleeType;

    return eMELEE_TYPE_HIT;
}
#endif

float
Player::GetAttackRange() const
{
    float fAttackRange = GetNormalRangeBonus();

    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    CEquipmentSlotContainer* pEquipSlotContainer =
        (CEquipmentSlotContainer*)m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT);
    SCItemSlot& rSlot = static_cast<SCItemSlot&>(pEquipSlotContainer->GetSlot(eEQUIPCONTAINER_WEAPON));
    if(pEquipSlotContainer->IsEmpty(eEQUIPCONTAINER_WEAPON) || 
       pEquipSlotContainer->IsActivated(eEQUIPCONTAINER_WEAPON) == false ||
       rSlot.GetDura() == 0 ) 
    {
        // 맨주먹
        fAttackRange += 2.5f;
    }
    else
    {
        const BASE_ITEMINFO* pInfo = GetEquipItemInfo(eEQUIPCONTAINER_WEAPON);
        if(pInfo)
            fAttackRange += pInfo->m_fAttRange;
    }

    return fAttackRange;
}

EXPTYPE
Player::AddExp(EXPTYPE exp, DWORD dwTargetObjKey, int iBonusRatio, BOOL bSendPacket, eGAME_CONTENT_TYPE ContentType)
{
    LEVELTYPE cur_level = GetLevel();
    if(cur_level >= Generic::GetMaxLevel(this))
    {
        exp = 0;
        OnAddExp(dwTargetObjKey, iBonusRatio, bSendPacket);
    }

    if(exp == 0)
        return exp;

    EXPTYPE curExp = GetExp();
    EXPTYPE newExp = curExp + exp;

#ifdef _NA_008333_20150615_FIXED_MAX_LEVEL_EXP
    if (cur_level == Generic::GetMaxLevel(this) - 1
        && GetNextExp() && newExp >= GetNextExp())
    {
        newExp = GetNextExp();
    }
#endif // _NA_008333_20150615_FIXED_MAX_LEVEL_EXP

    // 경험치 로그 남기기(10등분or전체)
    MAPCODE mapcode = 0;
    if(GetGameZonePtr())
        mapcode = GetGameZonePtr()->GetMapCode();
    GAMELOG->LogExp(this, cur_level, curExp, newExp, mapcode);
    SetExp(newExp);

    OnAddExp( dwTargetObjKey, iBonusRatio, bSendPacket, ContentType );
    //현재 경험치가 다음 레벨의 경험치보다 높으면.. 레벨업이다.
    while(GetNextExp() && GetExp() >= GetNextExp())
    {
        if(!Generic::CheckMaxLevel(this, GetLevel()+1))
        {
            SUNLOG(eCRITICAL_LOG, "[Player::AddExp] LEVEL Error(%d) CharName(%20s) !!",
                   GetLevel(), GetCharName());
            return exp;
        }
        // 레벨업 로그 
        GAMELOG->WriteLevelUp(this, GetLevel(), GetLevel() + 1);
        SUNLOG(eFULL_LOG, "[Player::AddExp] [레벨 업] ID = %s, NAME = %s, Level = %d ",
            m_pszUserID, char_info_.m_tszCharName, GetLevel()+1);
        // 레벨업 로그 끝..

        // level up!  --> 현재 경험치에 맞게 level과 nextExp를 다시 설정한다.
        SetLevel(GetLevel() + 1);

        OnLevelUp();
    }

    return exp;
}

DWORD
Player::GetWeaponKind() const
{
    CEquipmentSlotContainer* pEquipSlotContainer =
        (CEquipmentSlotContainer*)m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT);


    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    if(pEquipSlotContainer && !pEquipSlotContainer->IsEmpty(eEQUIPCONTAINER_WEAPON))
    {
        SCItemSlot& rSlot = static_cast<SCItemSlot&>(pEquipSlotContainer->GetSlot(eEQUIPCONTAINER_WEAPON));
        //활성화여부와 내구도를 같이 검사한다
        if (pEquipSlotContainer->IsActivated(eEQUIPCONTAINER_WEAPON) == true && rSlot.GetDura() != 0)
        {
            SCItemSlot& rSlot = static_cast<SCItemSlot&>(pEquipSlotContainer->GetSlot(eEQUIPCONTAINER_WEAPON));

            return rSlot.GetItemInfo()->m_wType;
        }
    }

    // 맨주먹
    return eWEAPONTYPE_PUNCH;
}

eATTACK_TYPE
Player::GetWeaponBaseAttType() const
{
    eATTACK_TYPE eAttackType = eATTACK_TYPE_MELEE;

    if(!m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT)->IsEmpty(eEQUIPCONTAINER_WEAPON))
    {       
        SCItemSlot& rSlot = static_cast<SCItemSlot&>(
            m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT)->GetSlot(eEQUIPCONTAINER_WEAPON));
        eAttackType = (eATTACK_TYPE)rSlot.GetItemInfo()->m_byBaseAttType;
    }

    return eAttackType;
}

eATTACK_TYPE
Player::GetWeaponMagicalAttType() const
{
    eATTACK_TYPE eAttackType = eATTACK_TYPE_INVALID;

    if(!m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT)->IsEmpty(eEQUIPCONTAINER_WEAPON))
    {       
        SCItemSlot& rSlot = static_cast<SCItemSlot&>(
            m_pItemManager->GetItemSlotContainer(SI_EQUIPMENT)->GetSlot(eEQUIPCONTAINER_WEAPON));
        eAttackType = (eATTACK_TYPE)rSlot.GetItemInfo()->m_byMagicalAttType;
    }

    return eAttackType;
}

void
Player::UpdateCalcRecover(BOOL hp_updated, BOOL mp_updated, BOOL sd_updated)
{
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(char_info_.m_byClassCode);
    eCHAR_CONDITION char_condition = GetStatusManager()->GetCondition();
    //
    int hp_recovery_quantity = 0;
    int mp_recovery_quantity = 0;
    int sd_recovery_quantity = 0;
    //
    if (hp_updated)
    {
        const int vitality = m_Attr.GetVIT();
        hp_recovery_quantity = CalcHPRecover(char_type, vitality, char_condition, char_info_.m_LV);
    };
    if (mp_updated)
    {
        const int spirit = m_Attr.GetSPR();
        mp_recovery_quantity = CalcMPRecover(char_type, spirit, char_condition);
    };
    if (sd_updated)
    {
        const uint8_t moving_flag = IsMoving() ? CHAR_ACTION_CONDITION_MOVING
                                               : CHAR_ACTION_CONDITION_NONE;
        // TODO: f110413.1L, changes the method to get fighting_flag.
        const uint8_t fighting_flag = GetStatusManager()->FindStatus(eCHAR_STATE_ETC_FIGHTING)
            ?   CHAR_ACTION_CONDITION_FIGHTING
            :   CHAR_ACTION_CONDITION_NONE;

        sd_recovery_quantity = CalcSDRecover(
            char_type, char_condition, (moving_flag | fighting_flag), char_info_.m_LV);
    };

    if (hp_updated || mp_updated || sd_updated)
    {
        // CHANGES: f110413.1L, change logic to reduce a busy big update process
        // because HP, MP and SD recovery update routine is often invoked by a changed value.
        m_Attr.UpdateChangedRecoveries(&char_info_,
            (hp_updated != false), hp_recovery_quantity,
            (mp_updated != false), mp_recovery_quantity,
            (sd_updated != false), sd_recovery_quantity);
    };
}

//-------------------------------------------------------------------------------------------------
// 파라미터 변화
//-------------------------------------------------------------------------------------------------

// stat
BOOL
Player::CanSelectStat(eATTR_TYPE attrType) const
{
    if(char_info_.m_iRemainStat == 0)
    {
        return FALSE;
    }

    // attrType이 정상인가?
    switch (attrType)
    {
    case eATTR_STR:
    case eATTR_DEX:
    case eATTR_VIT:
    case eATTR_INT:
    case eATTR_SPR:
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    case eATTR_EXPERTY1:
    case eATTR_EXPERTY2:
#endif
        break;

    default :
        return FALSE;
    }

    // TODO: 이미 attrType이 MAX에 도달한 경우는 불가! MAX는 어디에? --;

    return TRUE;
}

#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION    
// 힘,민첩,체력,지력,정신력
RC::eSTAT_POINT_RESULT Player::ApplyStat(ushort* INOUT stat_point_array, uint array_count)
{
    if (eATTR_BASE_STAT_COUNT != array_count)
        return RC::RC_STAT_POINT_FAILED;

    //변신 상태에서는 스텟 변경을 못하게 한다.
    if(GetCharType() == eCHAR_DRAGON)
    {
        StatusManager* const pStatusManager = GetStatusManager();
        if( pStatusManager->Status.IsDragonTransforming() )
        {         
            return RC::RC_STAT_POINT_DRAGON_CHANGED;
        }        
    }  
    
    ushort stat_point = 0;
    uint idx = 0;

    int original_stat_point[eATTR_BASE_STAT_COUNT] = {0,};
    int desired_stat_point[eATTR_BASE_STAT_COUNT] = {0,};

    for (idx = eATTR_BASE_STAT_START_INDEX ; idx < eATTR_BASE_STAT_COUNT ; ++idx)
    {
        eATTR_TYPE attribute_type = (eATTR_TYPE)idx;
        switch (attribute_type)
        {
        case eATTR_STR:
        case eATTR_DEX:
        case eATTR_VIT:
        case eATTR_INT:
        case eATTR_SPR:
            break;
        default :
            ASSERT(! "[Player::SelectStat] Invalid attrType");
            return RC::RC_STAT_POINT_FAILED;
        }

        stat_point = stat_point_array[attribute_type];

        // 스텟 포인트 부족.
        if (char_info_.m_iRemainStat < stat_point)
            return RC::RC_STAT_POINT_IS_NOT_ENOUGH;

        const AttrValue& attr_value = m_Attr[attribute_type];
        original_stat_point[attribute_type] = attr_value[eATTR_KIND_BASE];
        desired_stat_point[attribute_type] = original_stat_point[attribute_type] + stat_point;

        // 실제로 stat을 적용하자
        char_info_.m_iRemainStat -= stat_point;
        m_Attr[attribute_type][eATTR_KIND_BASE] += stat_point;
    }

    //스탯이 바뀌었으므로 스탯만 업데이트 하지말고 장비아이템 까지 체크 해보자.
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    if (ItemManager* item_manager = GetItemManager()) {
        item_manager->UpdateItemAttributes(true, false);
    };
#else
    m_Attr.UpdateEx();
    GetItemManager()->UpdateAttrForEquipItem();
#endif
    for (idx = eATTR_BASE_STAT_START_INDEX ; idx < eATTR_BASE_STAT_COUNT ; ++idx)
    {
        eATTR_TYPE attribute_type = (eATTR_TYPE)idx;
        const AttrValue& attr_value = m_Attr[attribute_type];
        int result_value = attr_value[eATTR_KIND_BASE];
        if (desired_stat_point[attribute_type] != result_value)
        {
            if (desired_stat_point[attribute_type] > result_value)
                char_info_.m_iRemainStat += (desired_stat_point[attribute_type] - result_value);
            else
                char_info_.m_iRemainStat -= (result_value - desired_stat_point[attribute_type]);
        }

        stat_point_array[attribute_type] = (ushort)result_value;
    }

    char_info_.m_iRemainStat = BOUND_CHECK(0, char_info_.m_iRemainStat, SHRT_MAX);
    ((PlayerAttributes&)GetAttr()).Display();
    return RC::RC_STAT_POINT_SUCCESS;
}
#endif

DWORD
Player::SelectStat(eATTR_TYPE attrType, int iIncValue)
{
    SASSERT(char_info_.m_dwRemainStat > 0, "[Player::SelectStat] m_dwRemainStat <= 0");

    if(attrType != eATTR_STR && attrType != eATTR_DEX &&
       attrType != eATTR_VIT && attrType != eATTR_INT &&
       attrType != eATTR_SPR
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
       && attrType != eATTR_EXPERTY1 && attrType !=eATTR_EXPERTY2
#endif
       )
    {
        ASSERT(! "[Player::SelectStat] Invalid attrType");
    }

    // Stat point 줄인다
    if (char_info_.m_iRemainStat < iIncValue) {
        iIncValue = char_info_.m_iRemainStat;
    }
    // (f100722.2L) fit in a boundary about stat point value calculator
    // (NOTE) calculate a lowest boundary
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    const int stat_lowest_limit = 1;
#else
    const int stat_lowest_limit = \
        (attrType != eATTR_EXPERTY1 && attrType != eATTR_EXPERTY2) ? 1 : 0;
#endif
    // CHANGES: f110415.2L, fixed negative value calculating error when GM command used
    // NOTE from jaykang "지금은 iIncValue가 음수일때 체크 없이 남은 스탯 수를 올려 버린다"
    const AttrValue& attr_value = m_Attr[attrType];
    const int original_stat = attr_value[eATTR_KIND_BASE];
    int desired_stat = original_stat + iIncValue;
    if (desired_stat < stat_lowest_limit) {
        iIncValue = (stat_lowest_limit - original_stat);
    }
    desired_stat = original_stat + iIncValue;
    if (iIncValue == 0) {
        return original_stat;
    }
    char_info_.m_iRemainStat -= iIncValue;

    // 실제로 stat을 적용하자
    m_Attr[attrType][eATTR_KIND_BASE] += iIncValue;

    //스탯이 바뀌었으므로 스탯만 업데이트 하지말고 장비아이템 까지 체크 해보자.
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    if (ItemManager* item_manager = GetItemManager()) {
        item_manager->UpdateItemAttributes(true, false);
    };
#else
    m_Attr.UpdateEx();
    GetItemManager()->UpdateAttrForEquipItem();
#endif
    int result_value = attr_value[eATTR_KIND_BASE];
    if (desired_stat != result_value)
    {
        assert(desired_stat > result_value);
        char_info_.m_iRemainStat += (desired_stat - result_value);
    }

    char_info_.m_iRemainStat = BOUND_CHECK(0, char_info_.m_iRemainStat, SHRT_MAX);
    ((PlayerAttributes&)GetAttr()).Display();
    return result_value;
}


// GM 명령어 처리
void
Player::LevelUp(DWORD dwPlusLevel)
{
    if(!Generic::CheckMaxLevel(this, dwPlusLevel) ||
       !Generic::CheckMaxLevel(this, GetLevel() + dwPlusLevel)) 
    {
        return;
    }

    for(DWORD i = 0 ; i < dwPlusLevel ; ++i)
    {
        SetLevel(GetLevel() + 1);

        // GM명령어로 올린 레벨에 대한 경험치를 설정해 줘야 한다.
        SetExp(GetAccumExp(GetLevel()));
        OnLevelUp();
    }
}

// GM 명령어 처리(레벨다운)
// 레벨다운은 레벨다운데 의한 경험치, HP등의 속성에 영향을 미치지 않고, 
// 단순히 레벨만 내려간다.
void
Player::LevelDown(int nDownLevel)
{
    if(nDownLevel >= 0)
        return;

    for(int i = 0 ; i > nDownLevel ; --i)
    {
        SetLevel(GetLevel() - 1);

        // GM명령어로 올린 레벨에 대한 경험치를 설정해 줘야 한다.
        SetExp(GetAccumExp(GetLevel()));
        OnAddExp(GetObjectKey(), 0, TRUE);
        OnLevelDown();
    }
}


void
Player::StatPointUp(int iBonusStat)
{
    // (f100722.2L) fit in a boundary about the remains stat point value
    int remains = char_info_.m_iRemainStat + iBonusStat;
    ;   remains = BOUND_CHECK(0, remains, SHRT_MAX);
    char_info_.m_iRemainStat = remains;

    MSG_CG_GM_STAT_UP_ACK sendMsg;
    sendMsg.m_byCategory = CG_GM;
    sendMsg.m_byProtocol = CG_GM_STAT_UP_ACK;
    sendMsg.m_dwRemainStat = static_cast<uint16_t>(remains);  // 남은 스탯포인트

    SendPacket(&sendMsg, sizeof(sendMsg));
}

//현재 남아있는 스텟포인트를 얻는다.
int
Player::GetStatPoint()
{
    return char_info_.m_iRemainStat;
}

void
Player::SkillPointUp(int iBonusSkill)
{
    char_info_.m_iRemainSkill += iBonusSkill;
    if(char_info_.m_iRemainSkill < 0)
        char_info_.m_iRemainSkill = 0;

    MSG_CG_GM_SKILLPOINT_UP_ACK sendMsg;
    sendMsg.m_byCategory = CG_GM;
    sendMsg.m_byProtocol = CG_GM_SKILLPOINT_UP_ACK;
    sendMsg.m_dwRemainSkill = (WORD)char_info_.m_iRemainSkill; // 남은 스킬포인트

    SendPacket(&sendMsg, sizeof(sendMsg));
}

//현재 남아있는 스킬포인트를 얻는다.
int
Player::GetRemainSkillPoint()
{
    return char_info_.m_iRemainSkill;
}

void
Player::AddPlayerTitle(ePLAYER_TITLE title) 
{ 
    m_PlayerTitle |= title; 

    // 자신
    MSG_CG_STATUS_TITLE_CHANGED_CMD cmsg;
    cmsg.m_PlayerTitle  = GetPlayerTitle();
    SendPacket(&cmsg, sizeof(cmsg));

    // 주위
    MSG_CG_STATUS_TITLE_CHANGED_BRD bmsg;
    bmsg.m_dwPlayerKey  = GetObjectKey();
    bmsg.m_PlayerTitle  = GetPlayerTitle();
    SendPacketAround(&bmsg, sizeof(bmsg), FALSE);
}

void
Player::RemovePlayerTitle(ePLAYER_TITLE title) 
{ 
    m_PlayerTitle &= ~title;

    // 자신
    MSG_CG_STATUS_TITLE_CHANGED_CMD cmsg;
    cmsg.m_PlayerTitle  = GetPlayerTitle();
    SendPacket(&cmsg, sizeof(cmsg));

    // 주위
    MSG_CG_STATUS_TITLE_CHANGED_BRD bmsg;
    bmsg.m_dwPlayerKey  = GetObjectKey();
    bmsg.m_PlayerTitle  = GetPlayerTitle();
    SendPacketAround(&bmsg, sizeof(bmsg), FALSE);
}
//-------------------------------------------------------------------------------------------------
// Private 멤버함수
//-------------------------------------------------------------------------------------------------

void
Player::SetNextExp()
{
    // m_Level을 보고... 다음 경험치를 설정한다. 
    m_NextExp = GetAccumExp(GetLevel() + 1);
}

EXPTYPE
Player::GetAccumExp(LEVELTYPE level)
{
    BASE_ACCUM_EXP_INFO* pAccumExpInfo = ExpValueInfoParser::Instance()->GetAccumExpInfo(level);
    if(pAccumExpInfo == 0)
    {
        //  (WAVERIX) (081028) (BUG-FIX) - 매크로 제거 작업하면서 로직이 변경된 사례 중 하나가 될 듯...-.-;;;
        //  - changes: source version 333->334 넘어가는 단계에서 문제 발생
        if(level <= Generic::GetMaxLevel(this))
            SUNLOG(eCRITICAL_LOG, "Level[%d]pAccumExpInfo is NULL!!", level);
        return 0;
    }
    return pAccumExpInfo->m_AccumExp;
}

void
Player::FixLevel()
{
    LEVELTYPE level = GetLevel();
    if(!Generic::CheckMaxLevel(this, level))
    {
        SUNLOG(eCRITICAL_LOG, "[Player::FixLevel] [Player_Parameter] LEVEL Error(%d) CharName(%20s) !!", GetLevel(), GetCharName());
        return;
    }

    if(Generic::GetMaxLevel(this) == level)
    {
        return;
    }

    EXPTYPE curMinExp = GetAccumExp(level);
    EXPTYPE nextExp = GetAccumExp(level + 1);

    if(GetExp() >= curMinExp && GetExp() < nextExp)
    {
        // 정상적인 레벨
        return;
    }

    // 현재 경험치가 다음 레벨의 경험치보다 높은 경우
    while(GetNextExp() && GetExp() >= GetNextExp())
    {
        if(!Generic::CheckMaxLevel(this, GetLevel()+1))
        {
            SUNLOG(eCRITICAL_LOG, "[Player::FixLevel] [Player_Parameter] LEVEL Error(%d) CharName(%20s) !!", GetLevel(), GetCharName());
            return;
        }
        GAMELOG->WriteLevelUp(this, GetLevel(), GetLevel() + 1);
        SetLevel(GetLevel() + 1);
        OnLevelUp();
    }
}


// implemented by __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
// level up과 각종 상태값 변화를 알려준다.
// 공격 패킷을 보내줘야한다!!
void
Player::OnAddExp(DWORD dwTargetObjKey, int iBonusRatio, BOOL bSendPacket, eGAME_CONTENT_TYPE eContentType)
{
    if (!bSendPacket)
        return;

    switch (eContentType)
    {
    case GAME_CONTENT_NORMAL:
        {
            MSG_CG_STATUS_EXP_CMD sendMsg;
            sendMsg.m_dwTargetObjKey = dwTargetObjKey;
            sendMsg.m_i64Exp = GetExp();
            sendMsg.m_BonusRatio = iBonusRatio;

            SendPacket(&sendMsg, sizeof(sendMsg));
        }
        break;

    case GAME_CONTENT_GROUPING:
        {
            MSG_CG_STATUS_GROUPEXP_CMD sendMsg;
            sendMsg.m_dwTargetObjKey = dwTargetObjKey;
            sendMsg.m_i64Exp = GetExp();

            SendPacket(&sendMsg, sizeof(sendMsg));
        }
        break;
    }        
}

void
Player::OnLevelUp()
{
    BASE_PLAYERINFO* pPlayerInfo = &char_info_;
    // Stat bonus
    DWORD bonusStat = GetStatPointPerLevel();
    pPlayerInfo->m_iRemainStat += bonusStat;

    // Skill Bonus!
    DWORD bonusSkill = GetSkillPoint(GetLevel());
    pPlayerInfo->m_iRemainSkill += bonusSkill;

    // 다음 레벨업 경험치 설정
    SetNextExp();

    // HP, MP 회복
    SetHP(GetMaxHP());
    if(GetCharType() != eCHAR_BERSERKER)
        SetMP(GetMaxMP());

    SetSD(GetMaxSD());
    // Agent로 알림
    MSG_AG_STATUS_LEVEL_UP_CMD cmsg;
    cmsg.m_Level        = GetLevel();
    cmsg.m_wRemainStat  = (WORD)pPlayerInfo->m_iRemainStat;  // 남은 스탯포인트
    cmsg.m_wRemainSkill = (WORD)pPlayerInfo->m_iRemainSkill; // 남은 스킬포인트
    cmsg.m_dwCurHP      = GetHP();
    cmsg.m_dwMaxMP      = GetMaxMP();
    cmsg.m_dwCurMP      = GetMP();
    cmsg.m_dwMaxAndCurSD = GetSD();
    SendPacket(&cmsg, sizeof(cmsg));

    // level up과 각종 상태값 변화를 알려준다.
    // 공격 패킷을 보내줘야한다!!
    MSG_CG_STATUS_LEVEL_UP_BRD sendMsg;
    sendMsg.m_dwObjectKey  = GetObjectKey();
    sendMsg.m_Level        = GetLevel();
    sendMsg.m_wRemainStat  = (WORD)pPlayerInfo->m_iRemainStat;  // 남은 스탯포인트
    sendMsg.m_wRemainSkill = (WORD)pPlayerInfo->m_iRemainSkill; // 남은 스킬포인트
    sendMsg.m_dwCurHP      = GetHP();
    sendMsg.m_dwCurMP      = GetMP();

    //!!!!!임시!!!!!!!!!!!!!!!!!!!
    //!!!!!임시!!!!!!!!!!!!!!!!!!!
    SendPacketAround(&sendMsg, sizeof(sendMsg), FALSE);

//길드서버로 레벨업을 알린다.
    // 길드가 존재하면..
    if(pPlayerInfo->m_GuildGuid != 0)
    {
        MSG_GZ_GUILD_CHAR_LEVELUP_SYN   synMsg;
        synMsg.m_GuildGuid = pPlayerInfo->m_GuildGuid;
        synMsg.m_CharGuid = GetCharGuid();
        synMsg.m_Level = GetLevel();
        SendToGuildServer(&synMsg, sizeof(synMsg)); 
    }

    // 퀘스트 자동 처리한다.
    FindAutoAcceptQuest opr(this);
    g_QuestInfoManager.for_each_auto(opr);
    
    // 파티원 레벨업 처리를 한다.
    NewGamePartyManager::Instance()->MemberLevelUp(GetPartyState().GetPartyKey(), GetObjectKey());

    // 경쟁헌팅 레벨업 처리
    if(ePRS_AT_CHUNTING == GetGameZoneState())
    {
        GameCHuntingRoom* pCHuntingRoom = (GameCHuntingRoom *)GetGameZonePtr();
        if(pCHuntingRoom)
            pCHuntingRoom->GetCHuntingManager()->LevelUp(this);
    }

#ifdef _NA_001990_ACSYSTEM_ADD
    AchievementManager* pACManager = GetACManager();
    if( pACManager )
        pACManager->ProcessLevelUp(GetLevel());
#endif //_NA_001990_ACSYSTEM_ADD
}

void
Player::OnLevelDown()
{
    // 다음 레벨업 경험치 설정
    SetNextExp();

    // HP, MP 회복
    SetHP(GetMaxHP());
    if(GetCharType() != eCHAR_BERSERKER)
        SetMP(GetMaxMP());

    SetSD(GetMaxSD());

    const BASE_PLAYERINFO* pPlayerInfo = &char_info_;
    // Agent로 알림
    MSG_AG_STATUS_LEVEL_UP_CMD cmsg;
    cmsg.m_Level        = GetLevel();
    cmsg.m_wRemainStat  = (WORD)pPlayerInfo->m_iRemainStat;  // 남은 스탯포인트
    cmsg.m_wRemainSkill = (WORD)pPlayerInfo->m_iRemainSkill; // 남은 스킬포인트
    cmsg.m_dwCurHP      = GetHP();
    cmsg.m_dwMaxMP      = GetMaxMP();
    cmsg.m_dwCurMP      = GetMP();
    cmsg.m_dwMaxAndCurSD = GetSD();
    SendPacket(&cmsg, sizeof(cmsg));

    // level up과 각종 상태값 변화를 알려준다.
    // 공격 패킷을 보내줘야한다!!
    MSG_CG_STATUS_LEVEL_UP_BRD sendMsg;
    sendMsg.m_dwObjectKey  = GetObjectKey();
    sendMsg.m_Level        = GetLevel();
    sendMsg.m_wRemainStat  = (WORD)pPlayerInfo->m_iRemainStat;  // 남은 스탯포인트
    sendMsg.m_wRemainSkill = (WORD)pPlayerInfo->m_iRemainSkill; // 남은 스킬포인트
    sendMsg.m_dwCurHP      = GetHP();
    sendMsg.m_dwCurMP      = GetMP();

    //!!!!!임시!!!!!!!!!!!!!!!!!!!
    //!!!!!임시!!!!!!!!!!!!!!!!!!!
    SendPacketAround(&sendMsg, sizeof(sendMsg), FALSE);
}

DWORD
Player::IncreaseMP(DWORD dwIncreament)
{
    dwIncreament = Character::IncreaseMP(dwIncreament);

    // 스피릿 데미지 스킬의 경우 MP 조건에 따라 적용유무가 바뀐다.
    m_pSkillSlotManager->UpdatePassiveSkills();

    return dwIncreament;
}

DWORD
Player::DecreaseMP(DWORD dwDecrement)
{
    dwDecrement = Character::DecreaseMP(dwDecrement);

    // 스피릿 데미지 스킬의 경우 MP 조건에 따라 적용유무가 바뀐다.
    m_pSkillSlotManager->UpdatePassiveSkills();

    return dwDecrement;
}

DWORD
Player::IncreaseHP(DWORD dwIncreament)
{
    dwIncreament = Character::IncreaseHP(dwIncreament);

    if(dwIncreament)
    {
        m_pSkillSlotManager->UpdatePassiveSkills();
    }

    return dwIncreament;
}

DWORD
Player::DecreaseHP(DWORD dwDecrement, int fLimitHP)
{
    // 무적상태 체크
    if(GetHP() <= dwDecrement && GetGMStateInfo().m_UndeadOn)
        return 0;

    dwDecrement = Character::DecreaseHP(dwDecrement, fLimitHP);
    m_pSkillSlotManager->UpdatePassiveSkills();

    return dwDecrement;
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
PLAYERFP Player::IncreaseFP( PLAYERFP valueFP )
{
    const PLAYERFP curFP = GetFP();
    const PLAYERFP calcedFP = curFP + valueFP;
    const PLAYERFP maxFP = GetMaxFP();

    PLAYERFP fp = min(maxFP, calcedFP);
    valueFP = fp - curFP;

    SetFP(fp);

    return valueFP;
}

PLAYERFP Player::DecreaseFP( PLAYERFP valueFP )
{
    PLAYERFP curFP = GetFP();
    if (curFP <= valueFP)
    {
        valueFP = curFP;
        SetFP(0);		
    }
    else
    {
        PLAYERFP fp = curFP - valueFP;
        SetFP(fp);
    }

    return valueFP;
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

void
Player::SaveStartLocation()
{
    GameZone* pZone = GetGameZonePtr();
    if(pZone == 0)
        return;

    const WzVector* pCurPos = GetPosPtr();

    eZONETYPE zoneType = pZone->GetZoneType();
    if (zoneType == eZONETYPE_VILLAGE ||
        zoneType == eZONETYPE_DOMINATION_FIELD  ||
        zoneType == eZONETYPE_FIELD ||
        zoneType == eZONETYPE_SPA_FIELD) //_NA_008069_20150210_ADD_SPA_SYSTEM
    {
        char_info_.m_iRegion    = pZone->GetMapCode();
        char_info_.m_sLocationX = (SHORT)pCurPos->x;
        char_info_.m_sLocationY = (SHORT)pCurPos->y;
        char_info_.m_sLocationZ = (SHORT)pCurPos->z;
    }
    else
    {
        char_info_.m_sLocationX = 0;
        char_info_.m_sLocationY = 0;
        char_info_.m_sLocationZ = 0;

        //비정상 종료 시 복구를 위해...
        Field* pField = GetField();
        if(pField == 0)
            return;

        MSG_AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD Cmd;
        Cmd.m_ReturnPos       = *pCurPos;
        Cmd.m_ReturnFieldCode = pField->GetFieldCode();
        Cmd.m_ReturnZoneType  = pZone->GetZoneType();
        Cmd.m_ReturnZoneKey = pZone->GetKey();
        Cmd.m_PartyKey = GetPartyState().GetPartyKey();
        Cmd.m_CharGUID = GetCharGuid();
        
        SendPacket(&Cmd, sizeof(Cmd));
    }
}

BOOL
Player::CanPlusMoney(MONEY plus_money)
{
    return TRUE;
}

BOOL
Player::CanMinusMoney(MONEY minus_money)
{
    if(GetMoney() < minus_money)
        return FALSE;
    return TRUE;
}
BOOL
Player::PlusMoney(MONEY plus_money, int type)
{
    if(!CanPlusMoney(plus_money)) //< 4294967295 : 42억을 넘지 않기
    {
        SUNLOG(eCRITICAL_LOG, "[%u:%u=%I64u,%I64u]돈추가오류", GetUserGuid(), GetCharGuid(), GetMoney(), plus_money);
        return FALSE;
    }

    SetMoney(GetMoney() + plus_money);

    switch(type)
    {
    case eMONEY_GENERNAL:
        g_StatisticsManager.PlusStatisticType(eSTATISTIC_MONEY_PLUS, plus_money);
        break;
    case eMONEY_CIRCULATING:
        g_StatisticsManager.PlusStatisticType(eSTATISTIC_MONEY_CIRCULATION, plus_money);
        break;
    //__KR_001355_20090624_GROUPINGSYSTEM_ALL
    case eMONEY_GROUP://그룹 보너스 머니일경우 유저에게 알린다.
        NotifyGroupMoneyToPlayer(plus_money);        
        break;
    }

    return TRUE;
}

BOOL
Player::MinusMoney(MONEY minus_money, int type)
{
    if(!CanMinusMoney(minus_money))
    {
        SUNLOG(eCRITICAL_LOG, "[%u:%u=%I64u,%I64u]돈차감오류", GetUserGuid(), GetCharGuid(), GetMoney(), minus_money);
        return FALSE;
    }

    SetMoney(GetMoney() - minus_money);

    switch(type)
    {
    case eMONEY_GENERNAL:
        g_StatisticsManager.PlusStatisticType(eSTATISTIC_MONEY_MINUS, minus_money);
        break;
    }

    return TRUE;
}

#if defined(__NA001336_090514_PARTY_NAVI_SUPPORT__) && !defined(__NA_0_20081124_PARTY_REFACTORING_GAMESERVER)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif


//{__NA001336_090514_PARTY_NAVI_SUPPORT__
//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
VOID Player::OnRecover( INT recoverHP, INT recoverMP, INT recoverSD, eRECOVER_TYPE recoverType, Character* pAttacker )
{   
    DWORD curHP = GetHP();
    DWORD curMP = GetMP();
    DWORD curSD = GetSD();

    if(recoverHP > 0)
    {
        DWORD maxHP = GetMaxHP();
        DWORD reservedHP = GetReserveHP();

        // HP가 자동 회복중이면....
        if(recoverType == eRECOVER_TYPE_AUTO_HP ||
           recoverType == eRECOVER_TYPE_AUTO_HPMP)
        {   // assert(recoverHP > 0)
            //플레이어가 현재 서 있는 상태이면, 현재 적립된HP 이상으로 회복될 수 없다.
            if(GetStatusManager()->GetCondition() == eCHAR_CONDITION_STANDUP)
                if(reservedHP < DWORD(recoverHP))
                    recoverHP = reservedHP;

            if(reservedHP > DWORD(recoverHP)) // (WAVERIX) (090823) (CHANGES) from '>=' to '>'
                SetReserveHP(reservedHP - recoverHP);
            else
                SetReserveHP(0);    
        }
        else if(recoverType == eRECOVER_TYPE_NORMAL)
        {
            //1. 회복량이 많으면 줄인다.
            if((curHP + recoverHP) > maxHP)     
            {
                recoverHP = maxHP - curHP;
                SetReserveHP(0);
            }
            else if((curHP + recoverHP + reservedHP) > maxHP)
                SetReserveHP(maxHP - curHP - recoverHP);
        }
    }

    // 실제 회복을 시킨다.
// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    Character::OnRecover( recoverHP, recoverMP, recoverSD, recoverType, pAttacker );

    //BOOLEAN isPartyMember = !!GetPartyState().IsPartyMember();
    // HP 변화 -> 주위의 사람들이 알아야 한다. 파티와 관계 되므로 Agent에 보내서 처리한다.
    DWORD changedHP = GetHP();
    if(curHP != changedHP)
    {
        //HP회복이 일어 났으면.. 그에 따른 적립된 HP가 감소된다.
        //이것을 Character에서 안 해주는것은 
        //플레이어와 몬스터의 ReserveHP가 다르기 때문이다.
        NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    }

    // MP 변화 -> 파티원일 경우에는 알려준다.
    DWORD changedMP = GetMP();
    if(curMP != changedMP)
    {
        NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
    }

    DWORD changedSD = GetSD();
    if(curSD != changedSD)
    {
        NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
    }
}

//}__NA001336_090514_PARTY_NAVI_SUPPORT__

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
VOID Player::OnRecoverFP( PLAYERFP recoverFP )
{
    PLAYERFP curFP = GetFP();

    if (recoverFP > 0)
    {
        //라이징포스 상태에서는 FP회복을 하지 못한다
        StatusManager* status_manager = GetStatusManager();
        BaseStatus* rising_force = status_manager->FindStatus(eCHAR_STATE_RISING_FORCE);
        if (rising_force != NULL) {
            return;
        }

        IncreaseFP(recoverFP);

        SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
        BaseStatus* fp_status = GetStatusManager()->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_FP);
        if (fp_status != NULL) {
            fp_status->Init(this, eCHAR_STATE_ETC_AUTO_RECOVER_FP, BASE_EXPIRE_TIME_INFINITY, 
                            stServerOptionInfo.player_fp_regen_period);
        }
    }
    else if(recoverFP < 0)
    {
        DecreaseFP(-recoverFP);
    }

    PLAYERFP changed_FP = GetFP();
    if (curFP != changed_FP)
    {
        NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedFP);
        //FP이펙트 동기화를 위한 패킷
        if (curFP == GetMaxFP() || changed_FP == GetMaxFP())
        {
            MSG_CG_STATUS_CHANGE_FP_EFFECT_BRD msg_brd;
            msg_brd.object_key = GetObjectKey();
            msg_brd.current_fp = changed_FP;
            SendPacketAround(&msg_brd, sizeof(msg_brd), FALSE);
        }
    }
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

void
Player::SetAttr(eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, int iValue) 
{
    BOOLEAN isValid = eATTR_TYPE_INVALID < eAttrType &&
                                           eAttrType < eATTR_MAX;
    if(isValid == 0)
    {
        DISPMSG("[Player::SetAttr] eAttrType[%d] is Invalid", eAttrType);
        return;
    }
    m_Attr[eAttrType][eAttrKind] = iValue; 
    m_Attr.UpdateEx(); 
}

//{_NA_001231_20081216_ADD_OPTION_KIND
// 상태 면역 확률 값 리턴 
// pvp시 에만 특정 상태에 대한 저항값을 적용함.
int
Player::GetResistBadStatusRatio(WORD wStateID)
{
    int resist = m_Attr.GetResistBadStatusRatio();

#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
    //쉐도우의 지속데미지형 스킬은 상태면역옵션을 적용받지않는다.
    switch(wStateID)
    {
    case eCHAR_STATE_HOLDING:   resist += (int)(m_Attr.GetResistHolding()); break;
    case eCHAR_STATE_DOWN:      resist += (int)(m_Attr.GetResistDown());  break;
    case eCHAR_STATE_STUN:      resist += (int)(m_Attr.GetResistStun());  break;
    case eCHAR_STATE_SLEEP:     resist += (int)(m_Attr.GetResistSleep()); break;

    case eCHAR_STATE_THRUST:
    case eCHAR_STATE_THRUST3:
    case eCHAR_STATE_THRUST4:
    case eCHAR_STATE_THRUST5:
    case eCHAR_STATE_THRUST2:
    case eCHAR_STATE_STYLE_THRUST:
        resist += (int)(m_Attr.GetResistKnockBack());
        break;

    case eCHAR_STATE_POISON:
    case eCHAR_STATE_POISON2:
    case kCharStatePoison3:
    case eCHAR_STATE_WOUND:
    case eCHAR_STATE_FIRE_WOUND:
    case eCHAR_STATE_FIRE_WOUND2:
    case eCHAR_STATE_PAIN:
    case eCHAR_STATE_PAIN2:
    case eCHAR_STATE_PHOENIX_BURN:
    case eCHAR_STATE_ELECTRICSHOCK:
    case eCHAR_STATE_HP_SUCTION:
    case eCHAR_STATE_MP_SUCTION:
        resist = 0;
        break;
    }
#else 
    switch(wStateID)
    {
    case eCHAR_STATE_HOLDING:   resist += (int)(m_Attr.GetResistHolding()); break;
    case eCHAR_STATE_DOWN:      resist += (int)(m_Attr.GetResistDown());  break;
    case eCHAR_STATE_STUN:      resist += (int)(m_Attr.GetResistStun());  break;
    case eCHAR_STATE_SLEEP:     resist += (int)(m_Attr.GetResistSleep()); break;
    case eCHAR_STATE_POISON:
    case eCHAR_STATE_POISON2:
        resist += (int)(m_Attr.GetResistPoison());
        break;
    case eCHAR_STATE_THRUST:
    case eCHAR_STATE_THRUST3:
    case eCHAR_STATE_THRUST4:
    case eCHAR_STATE_THRUST5:
    case eCHAR_STATE_THRUST2:
    case eCHAR_STATE_STYLE_THRUST:
        resist += (int)(m_Attr.GetResistKnockBack());
        break;
    }
#endif //_NA_007123_20140324_SHADOW_SKILL_RENEWAL

    return resist;
}
//}_NA_001231_20081216_ADD_OPTION_KIND

void
Player::SetBaseSelectedStyle(WORD wStyleCode)
{ 
    if(wStyleCode == eSTYLE_DRAGON_TRANSFORM1 ||
       wStyleCode == eSTYLE_DRAGON_TRANSFORM2)
        return;

    char_info_.m_iSelectStyleCode = wStyleCode;
    SetLastAttackSeq(eATTACK_SEQUENCE_NONE);
}

SLOTCODE
Player::GetSelectedStyle()
{
    StatusManager* const pStatusManager = GetStatusManager();
    if(pStatusManager->Status.IsDragonTransforming())
    {
        DragonTransStatus* const pDragonTransStatus =
            (DragonTransStatus*)GetStatusManager()->FindStatus(eCHAR_STATE_TRANSFORMATION);
        // CHANGES: f110428.6L, prevent a skipping previous style buff release process
        // because at this point, player is already transform.
        if (pDragonTransStatus->CheckStarting() == false)
        {
            const eCHAR_TYPE char_type = pDragonTransStatus->GetCharType();
            if (char_type == eTRANSFORM_TYPE_DRAGON1) {
                return eSTYLE_DRAGON_TRANSFORM1;
            }
            else if (char_type == eTRANSFORM_TYPE_DRAGON2) {
                return eSTYLE_DRAGON_TRANSFORM2;
            };
        };
    };

    return (SLOTCODE)char_info_.m_iSelectStyleCode;
}

// CHANGES: f110704.3L, synchronize the default style.
// The Episode II skill system does not use the passive skill and the style skill.
// this method is called once in Player::CopoToCharInfo() per the user
// since continous agent connected.
// returns: synchronized style code
SLOTCODE Player::SynchronizeStyleBasedOnWeapon()
{
    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
        this->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    const SCItemSlot& item_slot = static_cast<SCItemSlot&>(\
        equip_container->GetSlot(eEQUIPCONTAINER_WEAPON));
    SLOTCODE item_code = item_slot.GetItemCode();
    // if the item_code is 0, this result will get the punch style.
    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    // if the weapon is inactivated, select punch style.
    if (equip_container->IsActivated(eEQUIPCONTAINER_WEAPON) == false ||
        item_slot.GetDura() == 0) 
    {
        GetSkillSlotManager()->DelWeapon();
    }
    else {
        char_info_.m_iSelectStyleCode = this->GetDefaultStyle(item_code);
    }

    // don't use "this->GetSkillSlotManager()->AddWeapon();"
    return static_cast<SLOTCODE>(char_info_.m_iSelectStyleCode);
}

void
Player::SerialzieStatusGMInfo(STATE_GM_INFO& INOUT rStateGMInfo, eSERIALIZE se)
{
    if(se == SERIALIZE_LOAD)
    {
        rStateGMInfo = m_GMStateInfo;
    }
    else if(se == SERIALIZE_STORE)
    {
        m_GMStateInfo = rStateGMInfo;
    }
}

void
Player::ApplyGMState()
{
    // 속도 적용
    if(GetGMStateInfo().m_MoveSpeedLevel)
        GMCmdManager::Instance()->ApplySpeed(this, GetGMStateInfo().m_MoveSpeedLevel);

    // 투명 적용
    if(GetGMStateInfo().m_TransOn)
        GMCmdManager::Instance()->ApplyTransparent(this, TRUE);
}

// NOTE, r100828.3L, a heavy routine after a sd calculation based on a profiling test result.
// currently, 100828, a SD update policy is the points to be considered
// that need check a moving and a fighting status... terrible
// DONE: f110413.1L, waverix, need to desirable upgrade the SD(ShielD point) design.
void Player::UpdateCharStat()
{
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(char_info_.m_byClassCode);
    eCHAR_CONDITION char_condition = GetStatusManager()->GetCondition();
    const int hp_recovery_quantity = CalcHPRecover(\
        char_type, char_info_.m_sVitality, char_condition, char_info_.m_LV);
    const int mp_recovery_quantity = CalcMPRecover(\
        char_type, char_info_.m_sSpirit, char_condition);

    const uint8_t moving_flag = IsMoving() ? CHAR_ACTION_CONDITION_MOVING
                                           : CHAR_ACTION_CONDITION_NONE;
    // TODO: f110413.1L, changes the method to get fighting_flag.
    const uint8_t fighting_flag = GetStatusManager()->FindStatus(eCHAR_STATE_ETC_FIGHTING)
        ?   CHAR_ACTION_CONDITION_FIGHTING
        :   CHAR_ACTION_CONDITION_NONE;

    const int sd_recovery_quantity = CalcSDRecover(
        char_type, char_condition, (moving_flag | fighting_flag), char_info_.m_LV);

    m_Attr.SetCharInfo(&char_info_, hp_recovery_quantity, mp_recovery_quantity, sd_recovery_quantity);
    m_Attr.UpdateEx();
}

BOOL
Player::InitStat(BYTE eInitStat, WORD& OUT DecreaseValue)
{
    // CHANGES: f110701.2L, added the rule is to prevent irregula stat calcuation
    // like the dragon transformed state.
    if (this->GetStatusManager()->Status.IsDragonTransforming()) {
        return false; //RC::RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION;
    };
    switch(eInitStat)
    {
    case eItemWASTE_INIT_STR:
    case eItemWASTE_INIT_DEX:
    case eItemWASTE_INIT_VIT:
    case eItemWASTE_INIT_SPR:
    case eItemWASTE_INIT_INT:
    case eItemWASTE_INIT_Experty1:
    case eItemWASTE_INIT_Experty2:
        if(!InitStatAKind(eInitStat, DecreaseValue))
            return FALSE;
        break;
    case eItemWASTE_INIT_ALL:
        if(!InitStatAll())
            return FALSE;
        GAMELOG->WriteInitStatAll(this, ACT_INIT_STAT_ALL);
        break;
    case eItemWASTE_INIT_BASIC_STAT_ALL:
        if(!CanInitBasicStatAll())
            return FALSE;
        InitBasicStatAll();
        GAMELOG->WriteInitStatAll(this, ACT_INIT_BASIC_STAT_ALL);
        break;
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    case eItemWASTE_INIT_EXPERTY_ALL:
        InitSkillStatAll();
        GAMELOG->WriteInitStatAll(this, ACT_INIT_EXPERTY_STAT_ALL);
        break;
#else
    case eItemWASTE_INIT_EXPERTY_ALL:
        if(!CanInitExpertyStatAll())
            return FALSE;
        InitExpertyStatAll();
        GAMELOG->WriteInitStatAll(this, ACT_INIT_EXPERTY_STAT_ALL);
        break;
#endif //_NA_0_20100901_SKILLTREE_SYSTEM
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    case eItemWASTE_INIT_PASSIVE_ALL:
        InitPassiveStatAll();
        GAMELOG->WriteInitStatAll(this, ACT_INIT_PASSIVE_SKILL_ALL);
        break;
        // next logging and fails
#endif
    default:
        SUNLOG(eCRITICAL_LOG, "ItemManager::UseStatInitItem : 없는 WasteType");
        return FALSE;       
    }       

    if(eInitStat == eItemWASTE_INIT_ALL ||
       eInitStat == eItemWASTE_INIT_BASIC_STAT_ALL ||
       eInitStat == eItemWASTE_INIT_EXPERTY_ALL)
    {   
        const BASE_PLAYERINFO* pBasePlayerInfo = &char_info_;
        //클라로 통보
        MSG_CG_ITEM_STAT_ALL_CHANGE_CMD msgCMD;
        msgCMD.m_sStrength    = pBasePlayerInfo->m_sStrength;
        msgCMD.m_sDexterity   = pBasePlayerInfo->m_sDexterity;
        msgCMD.m_sVitality    = pBasePlayerInfo->m_sVitality;
        msgCMD.m_sInteligence = pBasePlayerInfo->m_sInteligence;
        msgCMD.m_sSpirit      = pBasePlayerInfo->m_sSpirit;
        msgCMD.m_sSkillStat1  = pBasePlayerInfo->m_sSkillStat1;
        msgCMD.m_sSkillStat2  = pBasePlayerInfo->m_sSkillStat2;
        msgCMD.m_iRemainStat  = pBasePlayerInfo->m_iRemainStat;
        SendPacket(&msgCMD, sizeof(msgCMD));
    }
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    this->m_pItemManager->UpdateItemAttributes(true, false);
#endif
    return TRUE;
}

BOOL
Player::InitStatAKind(BYTE init_char_type, WORD& OUT result_decrease_value)
{
    // CHANGES, f100828.2L, waverix, code arrangement
    const INIT_CHARINFO* init_char_info = \
        CharInfoParser::Instance()->GetInitCharInfo(char_info_.m_byClassCode);

    short min_value  = 0;
    short* change_value_field  = NULL;
    eATTR_TYPE attr_type = eATTR_TYPE_INVALID;
    switch (init_char_type)
    {
    case eItemWASTE_INIT_STR:
        min_value = init_char_info->m_sStrength;
        change_value_field = &(char_info_.m_sStrength);
        attr_type = eATTR_STR;
        break;
    case eItemWASTE_INIT_DEX:
        min_value = init_char_info->m_sDexterity;
        change_value_field = &(char_info_.m_sDexterity);
        attr_type = eATTR_DEX;
        break;
    case eItemWASTE_INIT_VIT:
        min_value = init_char_info->m_sVitality;
        change_value_field = &(char_info_.m_sVitality);
        attr_type = eATTR_VIT;
        break;
    case eItemWASTE_INIT_SPR:
        min_value = init_char_info->m_sSpirit;
        change_value_field = &(char_info_.m_sSpirit);
        attr_type = eATTR_SPR;
        break;
    case eItemWASTE_INIT_INT:
        min_value = init_char_info->m_sInteligence;
        change_value_field = &(char_info_.m_sInteligence);
        attr_type = eATTR_INT;
        break;
    case eItemWASTE_INIT_Experty1:
        min_value = init_char_info->m_sSkillStat1;
        change_value_field = &(char_info_.m_sSkillStat1);
        attr_type = eATTR_EXPERTY1;
        m_pSkillSlotManager->InitSkill();
        break;
    case eItemWASTE_INIT_Experty2:
        min_value = init_char_info->m_sSkillStat2;
        change_value_field = &(char_info_.m_sSkillStat2);
        attr_type = eATTR_EXPERTY2;
        m_pSkillSlotManager->InitSkill();
        break;
    default:
        return false;
    }

    if (change_value_field == NULL)
        return false;

    if (attr_type == eATTR_TYPE_INVALID)
        return false;

    //캐릭터 스탯에 이상이 있음
    if ((*change_value_field) < min_value)
        return false;

    //캐릭터의 초기값보다 작아지면 안된다.
    if (((*change_value_field) - result_decrease_value) < min_value)
        result_decrease_value = (*change_value_field) - min_value;

    if (result_decrease_value == 0)
        return false;

    //스탯 감소, 포인트 증가
    (*change_value_field) -= result_decrease_value;
    char_info_.m_iRemainStat += result_decrease_value;

    //갱신
    UpdateCharStat();

    //Player에게 스탯 변경을 통보한다.
    MSG_CG_ITEM_STAT_ONE_CHANGE_CMD msgCMD; 
    msgCMD.m_AttrType = attr_type;
    msgCMD.m_dwValue  = (*change_value_field);
    msgCMD.m_iRemainStat = GetRemainStatPoint();
    SendPacket(&msgCMD, sizeof(msgCMD));

    //LOG
    GAMELOG->WriteDecreaseStat(this, attr_type, result_decrease_value);

    return true;
}
BOOL
Player::InitStatAll()
{
    if(!CanInitBasicStatAll())
        return FALSE;

    if(!CanInitExpertyStatAll())
        return FALSE;
    
    InitBasicStatAll();
    InitExpertyStatAll();    
#ifdef _JP_20100114_CHANGE_ATTRIBUTE_LETECRARFE    
    InitPassiveStatAll();   
#endif

    return TRUE;
}

BOOL
Player::CanInitBasicStatAll()
{
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo =
        CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

    if(pPlayerInfo->m_sStrength < pInitCharInfo->m_sStrength   ||
       pPlayerInfo->m_sDexterity < pInitCharInfo->m_sDexterity ||
       pPlayerInfo->m_sVitality < pInitCharInfo->m_sVitality   ||
       pPlayerInfo->m_sSpirit < pInitCharInfo->m_sSpirit       ||
       pPlayerInfo->m_sInteligence < pInitCharInfo->m_sInteligence)
    {
        return FALSE;
    }

    return TRUE;
}
void
Player::InitBasicStatAll()
{
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo = CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
    //STR
    if(pPlayerInfo->m_sStrength > pInitCharInfo->m_sStrength)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sStrength - pInitCharInfo->m_sStrength);
    pPlayerInfo->m_sStrength = pInitCharInfo->m_sStrength;

    //DEX
    if(pPlayerInfo->m_sDexterity > pInitCharInfo->m_sDexterity)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sDexterity - pInitCharInfo->m_sDexterity);
    pPlayerInfo->m_sDexterity = pInitCharInfo->m_sDexterity;

    //VIT
    if(pPlayerInfo->m_sVitality > pInitCharInfo->m_sVitality)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sVitality - pInitCharInfo->m_sVitality);
    pPlayerInfo->m_sVitality = pInitCharInfo->m_sVitality;

    //SPR
    if(pPlayerInfo->m_sSpirit > pInitCharInfo->m_sSpirit)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSpirit - pInitCharInfo->m_sSpirit);
    pPlayerInfo->m_sSpirit = pInitCharInfo->m_sSpirit;

    //int
    if(pPlayerInfo->m_sInteligence > pInitCharInfo->m_sInteligence)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sInteligence - pInitCharInfo->m_sInteligence);
    pPlayerInfo->m_sInteligence = pInitCharInfo->m_sInteligence;
#else
        //STR
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sStrength - pInitCharInfo->m_sStrength);
        pPlayerInfo->m_sStrength = pInitCharInfo->m_sStrength;

        //DEX
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sDexterity - pInitCharInfo->m_sDexterity);
        pPlayerInfo->m_sDexterity = pInitCharInfo->m_sDexterity;

        //VIT
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sVitality - pInitCharInfo->m_sVitality);
        pPlayerInfo->m_sVitality = pInitCharInfo->m_sVitality;

        //SPR
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSpirit - pInitCharInfo->m_sSpirit);
        pPlayerInfo->m_sSpirit = pInitCharInfo->m_sSpirit;

        //int
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sInteligence - pInitCharInfo->m_sInteligence);
        pPlayerInfo->m_sInteligence = pInitCharInfo->m_sInteligence;    
#endif
    //갱신
    UpdateCharStat();
}

BOOL
Player::CanInitExpertyStatAll()
{
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo =
        CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

    if(pPlayerInfo->m_sSkillStat1 < pInitCharInfo->m_sSkillStat1 ||
       pPlayerInfo->m_sSkillStat2 < pInitCharInfo->m_sSkillStat2)
    {
        return FALSE;
    }

    return TRUE;
}

void
Player::InitExpertyStatAll()
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo =
        CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

  #ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
    //SKILL1
    if(pPlayerInfo->m_sSkillStat1 > pInitCharInfo->m_sSkillStat1)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSkillStat1 - pInitCharInfo->m_sSkillStat1);
    pPlayerInfo->m_sSkillStat1 = pInitCharInfo->m_sSkillStat1;

    //SKILL2
    if(pPlayerInfo->m_sSkillStat2 > pInitCharInfo->m_sSkillStat2)
        pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSkillStat2 - pInitCharInfo->m_sSkillStat2);
    pPlayerInfo->m_sSkillStat2 = pInitCharInfo->m_sSkillStat2;
  #else
    //SKILL1
    pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSkillStat1 - pInitCharInfo->m_sSkillStat1);
    pPlayerInfo->m_sSkillStat1 = pInitCharInfo->m_sSkillStat1;

    //SKILL2
    pPlayerInfo->m_iRemainStat += (pPlayerInfo->m_sSkillStat2 - pInitCharInfo->m_sSkillStat2);
    pPlayerInfo->m_sSkillStat2 = pInitCharInfo->m_sSkillStat2;
  #endif
    //갱신
    UpdateCharStat();
#endif
    //
    m_pSkillSlotManager->InitSkill();
}

void
Player::InitPassiveStatAll()
{
    m_pSkillSlotManager->InitPassiveSkillStyle();
}

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
void
Player::InitSkillStatAll()
{
    m_pSkillSlotManager->InitSkill();
}
#endif //_NA_0_20100901_SKILLTREE_SYSTEM

BOOL
Player::CheckValidStatPoint() 
{
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo =
        CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

    m_bCheckedStat = TRUE;

    //if initialize character status bigger than created character then Bug!!!
    if(pInitCharInfo->m_sStrength > pPlayerInfo->m_sStrength)
        return FALSE;

    if(pInitCharInfo->m_sDexterity > pPlayerInfo->m_sDexterity)
        return FALSE;

    if(pInitCharInfo->m_sVitality > pPlayerInfo->m_sVitality)
        return FALSE;

    if(pInitCharInfo->m_sInteligence > pPlayerInfo->m_sInteligence)
        return FALSE;

    if(pInitCharInfo->m_sSpirit > pPlayerInfo->m_sSpirit)
        return FALSE;

    if(pInitCharInfo->m_sSkillStat1 > pPlayerInfo->m_sSkillStat1)
        return FALSE;

    if(pInitCharInfo->m_sSkillStat2 > pPlayerInfo->m_sSkillStat2)
        return FALSE;

    SHORT sumStatPoint = 0;
    SHORT mustHavePoint = 0;

    // sumStatPoint
    sumStatPoint += pPlayerInfo->m_sStrength    - pInitCharInfo->m_sStrength;
    sumStatPoint += pPlayerInfo->m_sDexterity   - pInitCharInfo->m_sDexterity;
    sumStatPoint += pPlayerInfo->m_sVitality    - pInitCharInfo->m_sVitality;
    sumStatPoint += pPlayerInfo->m_sInteligence - pInitCharInfo->m_sInteligence;
    sumStatPoint += pPlayerInfo->m_sSpirit      - pInitCharInfo->m_sSpirit;
    sumStatPoint += pPlayerInfo->m_sSkillStat1  - pInitCharInfo->m_sSkillStat1;
    sumStatPoint += pPlayerInfo->m_sSkillStat2  - pInitCharInfo->m_sSkillStat2;
    
    sumStatPoint += pPlayerInfo->m_iRemainStat;
    
    // mustHavePoint    - (WAVERIX) (081208) Restrict Warning
    mustHavePoint += static_cast<SHORT>((pPlayerInfo->m_LV - 1) * GetStatPointPerLevel());

    // check
    if(mustHavePoint > sumStatPoint)
        return FALSE;   

    if(mustHavePoint + (MAX_AC_SECTION_NUM * (2) * 5) < sumStatPoint)
        return FALSE;

    return TRUE;
}

void
Player::RecoveryStatPoint()
{
    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();
    INIT_CHARINFO* pInitCharInfo =
        CharInfoParser::Instance()->GetInitCharInfo(pPlayerInfo->m_byClassCode);

    pPlayerInfo->m_sStrength    = pInitCharInfo->m_sStrength;
    pPlayerInfo->m_sDexterity   = pInitCharInfo->m_sDexterity;
    pPlayerInfo->m_sVitality    = pInitCharInfo->m_sVitality;
    pPlayerInfo->m_sInteligence = pInitCharInfo->m_sInteligence;
    pPlayerInfo->m_sSpirit      = pInitCharInfo->m_sSpirit;
    pPlayerInfo->m_sSkillStat1  = pInitCharInfo->m_sSkillStat1;
    pPlayerInfo->m_sSkillStat2  = pInitCharInfo->m_sSkillStat2;

    pPlayerInfo->m_iRemainStat  = (pPlayerInfo->m_LV - 1) * GetStatPointPerLevel();

    //갱신
    UpdateCharStat();

    //클라로 통보
    MSG_CG_ITEM_STAT_ALL_CHANGE_CMD CmdMsg;
    CmdMsg.m_sStrength    = pPlayerInfo->m_sStrength;
    CmdMsg.m_sDexterity   = pPlayerInfo->m_sDexterity;
    CmdMsg.m_sVitality    = pPlayerInfo->m_sVitality;
    CmdMsg.m_sInteligence = pPlayerInfo->m_sInteligence;
    CmdMsg.m_sSpirit      = pPlayerInfo->m_sSpirit;
    CmdMsg.m_sSkillStat1  = pPlayerInfo->m_sSkillStat1;
    CmdMsg.m_sSkillStat2  = pPlayerInfo->m_sSkillStat2;   
    CmdMsg.m_iRemainStat  = pPlayerInfo->m_iRemainStat;   
    SendPacket(&CmdMsg, sizeof(CmdMsg));
}

#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
BOOL
Player::InitAllStatByChangeUp()
{
    InitBasicStatAll();
    InitExpertyStatAll();
    InitPassiveStatAll();

    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();

    //클라로 통보
    MSG_CG_ITEM_STAT_ALL_CHANGE_CMD CmdMsg;
    CmdMsg.m_sStrength    = pPlayerInfo->m_sStrength;
    CmdMsg.m_sDexterity   = pPlayerInfo->m_sDexterity;
    CmdMsg.m_sVitality    = pPlayerInfo->m_sVitality;
    CmdMsg.m_sInteligence = pPlayerInfo->m_sInteligence;
    CmdMsg.m_sSpirit      = pPlayerInfo->m_sSpirit;
    CmdMsg.m_sSkillStat1  = pPlayerInfo->m_sSkillStat1;
    CmdMsg.m_sSkillStat2  = pPlayerInfo->m_sSkillStat2;
    CmdMsg.m_iRemainStat  = pPlayerInfo->m_iRemainStat;
    SendPacket(&CmdMsg, sizeof(CmdMsg));

    return TRUE;
}
#endif

// update all inventory insert rule configures
bool Player::UpdateInventoryConfig(const nsSlot::InventoryConfig& changed_info)
{
    //
    const nsSlot::InventoryConfig& inven_config = char_info_.m_InventoryConfig;
    const nsSlot::InventoryConfig::SetType* src_it = inven_config.configures,
                                          * const src_end = &inven_config.configures[\
                                                                _countof(inven_config.configures)];
    const nsSlot::InventoryConfig::SetType* change_it = changed_info.configures;
    for ( ; src_it != src_end; ++src_it, ++change_it)
    {
        if (*src_it != *change_it) {
            break;
        }
    }

    if (src_it == src_end) { // no changes, don't need update
        return false;
    }
    CInventorySlotContainer* inventory = static_cast<CInventorySlotContainer*>(\
        m_pItemManager->GetItemSlotContainer(SI_INVENTORY));
    inventory->UpdateInventoryConfiguration(changed_info);
    return true;
}

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
RC::eCHANGE_CHARACTER_APPEARANCE_RESULT Player::ChangeCharacterAppearance(
    CharacterAppearanceInfo& INOUT character_appearance) // 캐릭터 외모를 변경한다.
{
    // 변경 종류를 검사한다.
    if (character_appearance.appearance_type_ == ChangeCharacterAppearanceTypeConst::kNone)
    {
        return RC::RC_CHANGE_CHARACTER_APPEARANCE_FAIL;
    }

    // 외모를 변경한다.
    // BASE_PLAYERINFO* const player = SCPlayer::GetCharInfo(); // backup a previous aliasing code
    // NOTE, ... player? this is no big deal in a short term block,
    //  but the name called player is customarily ambigous
    //  because of most of codes are used to mean a alias of an instance of the 'Player' class
    //  so that I replace to char_info_ changed by the f100827.4L. - written by waverix
    {
#ifdef _NA_002050_20110216_ADD_GENDER
        // 성별 변경 여부를 검사한다.
        if (ChangeCharacterAppearanceTypeConst::IsChangeGender(character_appearance.appearance_type_) && 
            character_appearance.gender != char_info_.gender)
        {
            // 남성 또는 여성인지 검사한다.
            if (character_appearance.gender != GenderType::kMale && 
                character_appearance.gender != GenderType::kFemale)
            {
                return RC::RC_CHANGE_CHARACTER_APPEARANCE_FAIL;
            }
            char_info_.gender = character_appearance.gender;
        }
#endif
        // 얼굴 변경 여부를 검사한다.
        if (ChangeCharacterAppearanceTypeConst::IsChangeFace(character_appearance.appearance_type_) &&
            character_appearance.face_ != char_info_.m_byFace)
        {
            char_info_.m_byFace = character_appearance.face_;
        }
        
        // 헤어 변경 여부를 검사한다.
        if (ChangeCharacterAppearanceTypeConst::IsChangeHair(character_appearance.appearance_type_) &&
            character_appearance.hair_ != char_info_.m_byHair)
        {
            char_info_.m_byHair = character_appearance.hair_;
        }
        
        // 키 변경 여부를 검사한다.
        if (ChangeCharacterAppearanceTypeConst::IsChangeHeight(character_appearance.appearance_type_) &&
            character_appearance.height_ != char_info_.m_byHeight)
        {
            char_info_.m_byHeight = character_appearance.height_;
        }
    }

    return RC::RC_CHANGE_CHARACTER_APPEARANCE_SUCCESS;
}

// implemented by _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE
BOOL Player::PickMoney(MONEY nMoney, DWORD dwMonsterKey, Player* pOwner)
{
    MONEY DropMoney = nMoney;

    //////////////////////////////////////////////////////////////////////////
    //

    if( dwMonsterKey != 0 )
    {
        // 하임증가 아이템이 있는지 체크
        if( GetBonusMoneyRatio() )
        {
            DropMoney = (MONEY)(ceil)( DropMoney * ( 1.f + GetBonusMoneyRatio() ) );
        }
    }


#ifdef __NA_000632_20070817_CHINA_FATIGUE_HEIM
    BYTE byPickRatio = GetFatigueManager()->GetShareHeimPickupRatio();
    DropMoney = ( DropMoney / (float)(100.0/byPickRatio) );
#endif

    PlusMoney( DropMoney );
    GAMELOG->LogMoney( MONEY_PICK, this, DropMoney );

#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY
    if (GetGroupHighMemberInfo() == GROUP_HAVE_ONLINE_HIGH ||
        GetGroupHighMemberInfo() == GROUP_HAVE_OFFLINE_HIGH)
    {
        GroupBonusAcceptForMoney( nMoney, DropMoney, eHEIM_ACQUISITION_PICKUP );
    }
#else
    if( GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW ) //한명의 그룹원이라도 온라인이라면
    {
        GroupBonusAcceptForMoney( nMoney, DropMoney, eHEIM_ACQUISITION_PICKUP );
    }
#endif

    MSG_CG_ITEM_PICK_MONEY_ACK sendMsg;
    sendMsg.m_Money			= GetMoney();
    sendMsg.m_BonusRatio	= (WORD)ceil( GetBonusMoneyRatio() * 100 );
    SendPacket( &sendMsg, sizeof( sendMsg ) );

    return TRUE;
}