#include "stdafx.h"
#include "./ItemManager.h"

#include "ItemTypeList.h"
#include "ItemCrystalizeListParser.h"

#include "InventorySlotContainer.h"
#include "SlotManager.h"

#include "GameZone.h"

#include "RewardManager.h"
#include "AchievementManager.h"

#include "ChaoState.h"

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
#include "SCItemSlotContainer.h"
#include "SCItemSlot.h"
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

//--------------------------------------------------------------------------------------------------
// @history
//  09/06/25|~~~
//  09/07/16|shogen|enchant facility uniformity|<- 적용하긴 하는 걸까?
//  09/10/22|namka199|rank facility improvement|(WARNING) 인첸트 일원화 작업이 일부 누락된 듯.
//  10/01/28|waverix|code arrangement
//                  |delete macros  '__NA_676_PK_SYSTEM_IMPROVEMENT'
//                  |               '__NA_977_PREVENTION_PARTY_RETURN_ABUSE'
//                  |               '__NA_1339_ENCHANT_RENEWAL_EX'
//                  |               '__NA_1168_ADD_CONTENTS_ADD_LOG'
//                  |               '__BUG_35162_COMPOSE_BUG_BY_FULL_IVEN'
//                  |               '__CN_1194__ELITE_1_5_ITEM'
//                  |               '__MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM'
//  10/02/23|hukim|remove warnings level 4... etc...
//                |(waverix: i think this changes have fragmentary knowledge)
//  10/02/24|waverix|rollback to revision 46727|
//                  |현웅씨, 다시 한 번 재검토 해보시구랴. 물론 경고를 무시하는 것은 나쁜 거지만,
//                  warning level 4라는 녀석의 함정에 빠져 무리한 변경을 수행한 걸로 생각됩니다.
//--------------------------------------------------------------------------------------------------


//==================================================================================================
//==================================================================================================
//==================================================================================================

void ItemManager::SendItemChange(SCItemSlot& target_slot)
{
    MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD cmdMsg;
    cmdMsg.m_SlotIndex = SI_INVENTORY;
    cmdMsg.m_SlotPos = target_slot.GetPos();
    target_slot.CopyOut(cmdMsg.m_ItemStream);
    player_->SendPacket(&cmdMsg, sizeof(cmdMsg));
}

#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
// 랭크 아이템 소켓 생성
BOOL ItemManager::CreateRankItemSocketHole(SCItemSlot* item_slot, BYTE current_rank)
{
    const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
    if (item_slot->CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false) {
        return false;
    }
    //
    BOOL socket_created = false;
    switch (item_info->m_wType)
    {
        // 무기
    case eITEMTYPE_TWOHANDAXE:  
    case eITEMTYPE_TWOHANDSWORD:    
    case eITEMTYPE_ONEHANDSWORD:
    case eITEMTYPE_SPEAR:
    case eITEMTYPE_WHIP:        
    case eITEMTYPE_DAGGER:  
    case eITEMTYPE_ONEHANDCROSSBOW:
    case eITEMTYPE_ETHERWEAPON:
    case eITEMTYPE_STAFF:
    case eITEMTYPE_ORB:
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_HELLROID_1:
    case eITEMTYPE_HELLROID_2:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ARCBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // 방어구
    case eITEMTYPE_ARMOR:
    case eITEMTYPE_PANTS:
    case eITEMTYPE_BOOTS:
    case eITEMTYPE_GLOVE:
    case eITEMTYPE_HELMET:
    case eITEMTYPE_PROTECTOR:
    case eITEMTYPE_BELT:
    case eITEMTYPE_SHIRTS:
        {
            BYTE number_of_sockets = item_slot->GetSocketNum();
            if (number_of_sockets < 3 && (current_rank + 1) <= RANK_B)
            {
                BYTE real_rank = static_cast<BYTE>(item_slot->GetRank());
                ASSERT(real_rank == (current_rank + 1));
                BYTE selected_index = real_rank - RANK_D; //formula to get index
                BYTE seed = random(1, 100);
                if (seed <= g_Ratio.m_SocketRatio[selected_index].Ratio[number_of_sockets])
                {   //success
                    item_slot->SetSocketNum(number_of_sockets + 1);
                    socket_created = true;
                }
            }
    #if SUN_CODE_BACKUP
            BYTE SocketNum  = item_slot->GetSocketNum();
            if ((current_rank+1) <= RANK_B && SocketNum < 3)
            {
                ASSERT(item_slot->GetRank() == (current_rank+1));
                BYTE SelectRank = item_slot->GetRank() - RANK_D;
                BYTE seed = random(1, 100);
                if (seed <= g_Ratio.m_SocketRatio[SelectRank].Ratio[SocketNum])
                {
                    // 성공
                    item_slot->SetSocketNum(item_slot->GetSocketNum()+1);
                    socket_created = true;
                }
            }
    #endif //SUN_CODE_BACKUP
        }
        break;
    }

    return socket_created;
}

//--------------------------------------------------------------------------------------------------

BOOL ItemManager::ItemRankDown(SCItemSlot* item_slot)
{
    const BYTE current_rank = static_cast<BYTE>(item_slot->GetRank());
    if (FlowControl::FCAssert(current_rank > RANK_E) == false) {
        return false;
    }
    item_slot->DelRankAttr(static_cast<eRANK>(current_rank));
    return true;
}

//--------------------------------------------------------------------------------------------------

RC::eITEM_RESULT
ItemManager::RankDown(POSTYPE rank_down_item_pos)
{
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (ValidPos(SI_INVENTORY, rank_down_item_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }

    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& target_slot = static_cast<SCItemSlot&>(inventory->GetSlot(rank_down_item_pos));
    const BASE_ITEMINFO* item_info = target_slot.GetItemInfo();

    // (NOTE) disabled rank down item type = { elite, unique, under RANK_E }
    if (item_info->IsElite()) {
        return RC::RC_ITEM_CANNOT_RANKDOWN_ELITE;
    }
    if (item_info->IsUnique()) {
        return RC::RC_ITEM_CANNOT_RANKDOWN_UNIQUE;
    }
    if (target_slot.GetRank() <= RANK_E) {
        return RC::RC_ITEM_NOT_RANK_DOWN_TO_E_RANK_ITEM;
    }
    // check acceptable item type
    if (item_info->IsWeapon() || item_info->IsArmor()) {
        ; // nothing to do
    }
    //else if (item_info->IsAccessory()) {
    //    return RC::RC_ITEM_INVALID_CONDITION;
    //}
    else {
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    // CHANGES: f101018.2L, prevent the rank down operation
    // if a item combined an etheria with an other type shell.
    // because the rank system will be removed.
    // additionally, the rank up operation is already blocked.
    if (target_slot.CanUseMixture(MIXTURE_RANKUP) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    };

    GAMELOG->LogItem(ITEM_RANKDOWN_BEFORE, player_, &target_slot);
    if (ItemRankDown(&target_slot) == false) {
        return RC::RC_ITEM_FAILED;
    }
    GAMELOG->LogItem(ITEM_RANKDOWN_AFTER, player_, &target_slot);

    return RC::RC_ITEM_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// 한단계 랭크업을 한다.

BOOL ItemManager::ItemRankUp(SCItemSlot* item_slot)
{
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot->GetItemRuleInfo();
    if (item_rule_info.IsEnableRank() == false) {
        return false;
    }
    BYTE current_rank = static_cast<BYTE>(item_slot->GetRank());
    if (FlowControl::FCAssert(current_rank <= RANK_PS) == false) {
        return false;
    }

    const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
    RankOptionParser* const rank_option_parser = RankOptionParser::Instance();

    const sPTION_SELECTOR_FOR_RANK* selector = rank_option_parser->GetRankOptionSelector(\
        item_info->m_wType, static_cast<eRANK>(current_rank + 1));
    if (selector == NULL) {
        return false;
    }

    DWORD option_type = 0;  //...what is option type empty value?...
    //                        yes, 0 is converted 0 (=eATTR_TYPE_INVALID)
    {
        BYTE seed = random(0, selector->m_OptionCount - 1);
        // (CHANGES) (WAVERIX) add check rule, overrun protector, but _countof(m_OptionIndex) == 100
        if (FlowControl::FCAssert(_countof(selector->m_OptionIndex) > seed) == false) {
            return false;
        }
        option_type = selector->m_OptionIndex[seed];
    };

    const sRANK_OPTION* rank_option = \
        rank_option_parser->GetRankOption(item_info->m_wType, option_type);
    if (rank_option == NULL) {
        return false;
    }

    ASSERT(rank_option->m_iOptionIndex != 0);
    ASSERT(rank_option->m_OptionNCode != 0);
    ASSERT(rank_option->m_Value[current_rank + 1] != 0);

    { //result process: set rank & options
        eRANK desired_rank = static_cast<eRANK>(current_rank + 1);
        item_slot->SetRank(desired_rank);
        item_slot->SetRankAttr(desired_rank, rank_option);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

RC::eITEM_RESULT
ItemManager::RankUp(POSTYPE target_pos1, POSTYPE target_pos2, 
                    POSTYPE& OUT rankup_item_pos, POSTYPE& OUT deleted_item_pos,
                    MONEY& OUT rankup_fare, BOOL& OUT socket_created)

{
    if (IsRestrictFunction(eFUNCTIONALRESTRICT_RANKUP)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (player_->GetLevel() < LIMIT_LEVEL_RANKUP) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
    }
    if (player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }
    if (!player_->GetChaoState().CanUseFuction(CHAO_USE_RANKUP)) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (target_pos1 == target_pos2) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (ValidPos(SI_INVENTORY, target_pos1) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (ValidPos(SI_INVENTORY, target_pos2) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    //
    SCSlotContainer* const inventory = GetItemSlotContainer(SI_INVENTORY);

    SCItemSlot& target_slot1 = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos1));
    const BASE_ITEMINFO* target_item_info1 = target_slot1.GetItemInfo();
    //
    RC::eITEM_RESULT enable_rankup_result \
        = target_slot1.CanUseMixture(MIXTURE_RANKUP) == false ? RC::RC_ITEM_NOT_MIXTURE
        : target_slot1.GetRank() == RANK_PS                   ? RC::RC_ITEM_NO_MORE_RANK
        : target_slot1.IsLimited()                            ? RC::RC_ITEM_CANNOT_RANKUP_LIMITED
        :                                                       RC::RC_ITEM_SUCCESS;
    if (enable_rankup_result != RC::RC_ITEM_SUCCESS) {
        return enable_rankup_result;
    }
    // get '2nd item'
    SCItemSlot& target_slot2 = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos2));
    const BASE_ITEMINFO* target_item_info2 = target_slot2.GetItemInfo();

    // based on 1st item rule, reduce problem space
    if (target_slot1.GetItemCode() != target_slot2.GetItemCode() ||
        target_slot1.GetRank() != target_slot2.GetRank() ||
        target_slot1.IsDivine() != target_slot2.IsDivine())
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    if (target_slot2.IsLimited()) {
        return RC::RC_ITEM_CANNOT_RANKUP_LIMITED;
    }
    //
    if (target_item_info1->IsWeapon() || target_item_info1->IsArmor())
    {
        // 동급 인첸트 인지 체크
        eENCHANTLEVEL enchant1 = target_slot1.GetEnchantLV();
        if (enchant1 != target_slot2.GetEnchantLV()) {
            return RC::RC_ITEM_INVALID_CONDITION;
        }

        switch (target_slot1.GetRank())
        {
        case RANK_B: case RANK_MA: case RANK_A:
            // 중급이상의 인첸트여야 함
            if (enchant1 < ENCHANT_MIDDLE) {
                return RC::RC_ITEM_INVALID_CONDITION;
            }
            break;
        case RANK_PA: case RANK_MS: case RANK_S:
            // 상급이상의 인첸트여야 함
            if (enchant1 < ENCHANT_HIGH) {
                return RC::RC_ITEM_INVALID_CONDITION;
            }
            break;
        }
    }
    else if (target_item_info1->IsAccessory())
    {
        // 조건 없음
        // (WARNING) depend on client? accessory must be can't rank up
    }
    else
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    //--------------------------------------------------

    SCItemSlot* selected_item_slot = NULL;
    SCItemSlot* deleted_item_slot = NULL;

    MONEY rankup_fare1 = target_slot1.GetPriceForBuy() * target_item_info1->m_LV;
    MONEY rankup_fare2 = target_slot2.GetPriceForBuy() * target_item_info2->m_LV;
    // (CHANGES) (WAVERIX) may be, I think more correct rule which '>=' is better than '>'
    //                     prevent swapping if 1st item rank up fare equals 2nd item rank up fare
    if (rankup_fare1 >= rankup_fare2)
    {
        rankup_fare = rankup_fare1;
        selected_item_slot = &target_slot1;
        deleted_item_slot = &target_slot2;
    }
    else
    {
        rankup_fare = rankup_fare2;
        selected_item_slot = &target_slot2;
        deleted_item_slot = &target_slot1;
    }
    // 랭크업 요금은 있는지 체크
    if (player_->GetMoney() < rankup_fare) {
        return RC::RC_ITEM_INSUFFICIENT_MONEY;
    }
    // 우선 첫번째 아이템을 랭크업 아이템으로 선택
    { // calculate ratio
        int seed = random(0, 99);
        if (seed < 30)
        {
            // 2번 째 아이템 선택
            SCItemSlot* temp = selected_item_slot;
            selected_item_slot = deleted_item_slot;
            deleted_item_slot  = temp;
        }
    };

    rankup_item_pos  = selected_item_slot->GetPos();
    deleted_item_pos = deleted_item_slot->GetPos();

    BYTE current_rank = selected_item_slot->GetRank();

    // 로그
    GAMELOG->LogItemEx(ITEM_RANKUP_BEFORE, player_, *selected_item_slot, *deleted_item_slot);

    //일본 랭크업 실패 확률 설정
#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
    const BYTE rankup_ratio = current_rank == RANK_E  ? 100
                            : current_rank == RANK_D  ? 100
                            : current_rank == RANK_C  ?  90
                            : current_rank == RANK_B  ?  80
                            : current_rank == RANK_MA ?  70
                            : current_rank == RANK_A  ?  60
                            : current_rank == RANK_PA ?  50
                            : current_rank == RANK_MS ?  40
                            :                             0;
    { //calculate rank up fail ratio
        BYTE rankup_seed = random(0, 99); // Seed가 100이 나오면 rankup_ratio가 100일시 실패한다.
        if (rankup_seed > rankup_ratio)
        {
            //1. 아이템 소멸시키고
            Delete(deleted_item_slot->GetPos());
            //2. 돈도 소모 시킨다.
            BOOL ret = player_->MinusMoney(rankup_fare);
            ASSERT(ret);
            return RC::RC_ITEM_RANKUP_FAILED_AND_DESTROY;
        }
    };
#endif //__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE

    if (!ItemRankUp(selected_item_slot)) {
        return RC::RC_ITEM_CANNT_RANKUP_ITEM;
    }
    //__NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP
    if ((selected_item_slot->IsPossessionItem() && selected_item_slot->GetIdentifyPossession()) ||
        (deleted_item_slot->IsPossessionItem() && deleted_item_slot->GetIdentifyPossession()))
    {
        selected_item_slot->SetIdentifyPossession(true);
    }

    // 소켓 개수 생성
    socket_created = CreateRankItemSocketHole(selected_item_slot, current_rank);

    // 삭제할 아이템 삭제
    inventory->DeleteSlot(deleted_item_pos, NULL);
    // 소모되는 돈 소비
    BOOL ret = player_->MinusMoney(rankup_fare);
    ASSERT(ret);

    GAMELOG->LogMoney(MONEY_RANKUP, player_, rankup_fare);
    g_pGameServer->NotifyHeimConsumption(player_->GetCharGuid(),
                                        eHEIM_CONSUMPTION_RANKUP, rankup_fare);

    return RC::RC_ITEM_SUCCESS;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------

RC::eITEM_RESULT
ItemManager::Compose(SLOTCODE compose_list_code, BYTE making_num,
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                     INVENTORY_TOTAL_INFO* OUT inven_total_info, BYTE& success_num_out, BYTE& byPreventNum)
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                     INVENTORY_TOTAL_INFO* OUT inven_total_info, BYTE& success_num_out)
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
{
    if (IsRestrictFunction(eFUNCTIONALRESTRICT_COMPOSE)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (player_->GetChaoState().CanUseFuction(CHAO_USE_COMPOSE) == false) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }

    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (0 == making_num) {
        return RC::RC_ITEM_INVALID_VALUE;
    }

    ItemComposeListParser* compose_list_parser = ItemComposeListParser::Instance();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    const sITEM_COMPOSE_LIST* compose_list_from_parser = \
        compose_list_parser->GetComposeListInfo(compose_list_code);
    if (compose_list_from_parser == NULL) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    sITEM_COMPOSE_LIST temp_compose_list = *compose_list_from_parser;
    sITEM_COMPOSE_LIST* compose_list = &temp_compose_list;
    //제작 비용 감소 효과 적용
    compose_list->m_Money = \
        player_->GetAttr().GetValueAppliedAttr(compose_list_from_parser->m_Money, eATTR_CRAFT_COST_RATIO);
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    const sITEM_COMPOSE_LIST* compose_list = \
        compose_list_parser->GetComposeListInfo(compose_list_code);
    if (compose_list == NULL) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 제한 클래스 여부
    //  if (0 != pListInfo->m_LimitCharClass && pListInfo->m_LimitCharClass == player_->GetCharType())
    //      return RC::RC_ITEM_COMPOSE_LIMIT_CLASS;

    switch (compose_list->m_ItemType)
    {
    case eITEMTYPE_RING:
    case eITEMTYPE_NECKLACE:
        if (player_->GetLevel() < LIMIT_LEVEL_ACCESSORY)
            return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
        break;
        // 클래스별 특수 악세서리로 변경
    case eITEMTYPE_BERSERKER_SACCESSORY:
    case eITEMTYPE_DRAGON_SACCESSORY:
    case eITEMTYPE_VALKYRIE_SACCESSORY:
    case eITEMTYPE_SHADOW_SACCESSORY:
    case eITEMTYPE_ELEMENTALIST_SACCESSORY:
    case eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ALLCLASS_CHAOSEYE:              //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_BERSERKER_KALVUS_ACCESSORY:    //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_DRAGON_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_MYSTIC_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
    case eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        if (player_->GetLevel() < LIMIT_LEVEL_SACCESSORY)
            return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
        break;
    // (WARNING) it is has not default logic... thinking... miss?
    }

    // 돈 체크
    if (player_->GetMoney() < compose_list->m_Money * making_num) {
        return RC::RC_ITEM_HAVENOTMONEY;
    }

    GameZone* game_zone = player_->GetGameZonePtr();
    if (game_zone == NULL) {
        return RC::RC_ITEM_COMPOSE_INVALID_LOCATION;
    }

    // 맵 체크
    {
        const MAPCODE current_map_code = game_zone->GetMapCode();
        int index = 0;
        for ( ; index < _countof(compose_list->m_MapCode); ++index)
        {
            MAPCODE map_code = compose_list->m_MapCode[index];
            if (map_code != 0 && map_code == current_map_code) {
                break;
            }
        }

        if (index == _countof(compose_list->m_MapCode)) {
            return RC::RC_ITEM_COMPOSE_INVALID_LOCATION;
        }
    };

    // 재료 체크
    const sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE* materials = compose_list->m_pMaterial;
    for (POSTYPE i = 0; i < compose_list->m_byMatCount; ++i)
    {
        const sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE& material = materials[i];
        if (0 == material.m_ItemCode || 0 == material.m_byItemNum) {
            continue;
        }
        ns_functor::ComposeMaterialItemCheck empty_check(\
            material.m_ItemCode, material.m_ItemTypeIndex,
            material.m_byItemNum * making_num);
        if (ForeachSlot(SI_INVENTORY, &empty_check) == false) {
            return RC::RC_ITEM_ENCHANT_HAVENOTMETERIALS;
        }
    }

    RC::eITEM_RESULT compose_result = RC::RC_ITEM_COMPOSE_SUCCESS;

    //조합 요청 갯수만큼 루풍~
    success_num_out = 0;
    for (BYTE loop = making_num; loop; --loop)
    {   //조합      
#ifdef _NA_000000_20130612_RANDOMIZER_CHANGE
        RandomManager* const random_manager = player_->GetRandomManager();
        int random_result = random_manager->randomizer_compose_.Rand();
        int success_ratio = player_->GetAttr().GetCompositeIncRatio();
        ;   success_ratio += compose_list->m_byCreateRatio;
        if (random_result <= success_ratio*10) {
            ++success_num_out;
        }
#else
        int ratio = random(1, 100);
        // CHANGES: f110125.3L, rollback from sun:r51762 because this attribute is a ratio type.
        // the solving methodology is to modify script data not access a ratio section.
        int success_ratio = player_->GetAttr().GetCompositeIncRatio();
        ;   success_ratio += compose_list->m_byCreateRatio;
        // 결과물 아이템 습득
        if (ratio <= success_ratio) {
            ++success_num_out;
        }
#endif //_NA_000000_20130612_RANDOMIZER_CHANGE
    }

    if (success_num_out != 0)
    {
        // 성공
        if (compose_list->m_byResultType == compose_list->RESULTTYPE_ITEMCODE)
        {
            // 결과물 아이템 습득
            // (CHANGES) (100129) (WAVERIX) fix up invalid result, if not success status
            // (WARNING) occurred large block code renewal
            RC::eITEM_RESULT obtain_result = RC::RC_ITEM_INVALID_VALUE;
            if (compose_list->m_Result.m_ItemTypeIndex == 0)
            {
                obtain_result = Obtain(compose_list->m_Result.m_ItemCode,
                                       compose_list->m_Result.m_byItemNum * success_num_out,
                                       inven_total_info);
            }
            else
            {
                ItemTypeList* const item_type_list = ItemTypeList::Instance();
                ItemType* item_type = \
                    item_type_list->FindItemType(compose_list->m_Result.m_ItemTypeIndex);
                if (item_type == NULL)
                {
                    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Msg=can't find index|value=%d",
                           compose_list->m_Result.m_ItemTypeIndex);
                    obtain_result = RC::RC_ITEM_INVALID_VALUE;
                }
                else
                {
                    BYTE enchant = item_type->GetEnchant();
                    // (CHANGES) (f100525.1L)
                    BYTE item_type_value = item_type->GetItemType();
                    BYTE rank = item_type->GetRank();
                    obtain_result = ObtainAllKindOfItem(1,
                        &compose_list->m_Result.m_ItemCode, &success_num_out,
                        &enchant, &item_type_value, &rank, inven_total_info);
                }
            }
            //
            if (obtain_result != RC::RC_ITEM_SUCCESS) {
                return obtain_result;
            }
        }
        else if (compose_list->m_byResultType == compose_list->RESULTTYPE_REWARDCODE)
        {
            if (making_num > 1) {
                return RC::RC_ITEM_INVALID_VALUE;
            }

            RC::eREWARD_RESULT reward_result;
            reward_result = g_RewardManager.RewardItem(RewardManager::eREWARD_COMPOSE, \
                compose_list->m_Result.m_ItemCode, 0, player_, *inven_total_info);

            if (reward_result != RC::RC_REWARD_SUCCESS) {
                return RC::RC_ITEM_ISNOTEMPTYSLOT;
            }
        }
        else
        {
            return RC::RC_ITEM_FAILED;
        }

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        if (NotificationInfoParser::Instance()->IsNotificationItem(
            compose_list->m_Result.m_ItemCode, eNotificationCraft, 0, 0) == true)
        {
            MSG_DG_NOTIFICATION_ITEM_SYN msg;
            msg.info.SetInfo(player_->GetCharName(), 
                (player_->GetGameZonePtr() != NULL) ? player_->GetGameZonePtr()->GetMapCode() : 0,
                GetTickCount(), eNotificationCraft, compose_list->m_Result.m_ItemCode);
            g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
        }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

        //(lst1024) 비정상 조합로그 수정
        // (CHANGES) (100131) (WAVERIX) change pointer calculation that \
        // slot position array is continuous sequence
        POSTYPE total = 0;
    #if SUN_CODE_BACKUP
        SCSlotContainer* temp_inventory = GetItemSlotContainer(SI_TEMPINVENTORY);
    #endif
        SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);

        INVENTORY_TOTAL_INFO::SLOT_TYPE* slot = inven_total_info->m_Slot;
        for (POSTYPE loop = inven_total_info->m_InvenCount; loop; --loop)
        {
            SCItemSlot& target_item = static_cast<SCItemSlot&>(inventory->GetSlot(slot->m_Pos));
#ifdef _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT
            GAMELOG->LogCompose(true, player_, &target_item, 0, NULL, compose_list->m_Money, NULL, success_num_out);
#else
            GAMELOG->LogCompose(true, player_, &target_item, 0, NULL, compose_list->m_Money);
#endif //_NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT
            ++slot;
        }
    #if SUN_CODE_BACKUP
        for (POSTYPE loop = inven_total_info->m_TmpInvenCount; loop; --loop)
        {
            SCItemSlot& target_item = static_cast<SCItemSlot&>(temp_inventory->GetSlot(slot->m_Pos));
            GAMELOG->LogCompose(true, player_, &target_item, 0, NULL, compose_list->m_Money);
            ++slot;
        }
    #endif
        // 성공 로그
    }

    //삭제되는 아이템 시리얼 담을려고...
    DBWLDSERIAL item_serial_static[compose_list->MAX_COMPOSE_ITEM_NUM];
    // (CHANGES) (WAVERIX) fix up clear stream based on invalid size
    // old code: clear size = { sizeof(DWORD) * MAX_COMPOSE_ITEM_NUM }
    ZeroMemory(item_serial_static, sizeof(item_serial_static));

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 제작 실패 시 재료 소멸 방지 어트리뷰트 적용
    int prevent_delete_ratio = player_->GetAttr().GetCraftPreventExtinctionMaterialRatio();
    int delete_num = making_num;
    int prevent_num = 0;
    if (prevent_delete_ratio > 0 
        && (making_num - success_num_out) > 0)
    {
        for (int i = 0; i < making_num - success_num_out; ++i)
        {
            if (random_percent(prevent_delete_ratio))
            {
                ++prevent_num;
            }
        }
        delete_num -= prevent_num;
        byPreventNum = prevent_num;
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    // 재료 없애기
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    MSG_CG_ITEM_LOSE_ACK delete_msg;
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    for (POSTYPE i = 0; i < compose_list->m_byMatCount; ++i)
    {
        const sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE& material = materials[i];
        if (0 == material.m_ItemCode || 0 == material.m_byItemNum) {
            continue;
        }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        ns_functor::DeleteComposeMaterialItem delete_operator(\
            material.m_ItemCode, material.m_ItemTypeIndex,
            material.m_byItemNum * delete_num);
#else
        ns_functor::DeleteComposeMaterialItem delete_operator(\
            material.m_ItemCode, material.m_ItemTypeIndex,
            material.m_byItemNum * making_num);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        BOOL delete_result = DeleteComposeMaterials(&delete_operator, &delete_msg.m_ItemInfo);
#else
        BOOL delete_result = ForeachSlot(SI_INVENTORY, &delete_operator);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        ASSERT(delete_result);
        item_serial_static[i] = delete_operator.GetItemSerial();
    }
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    player_->SendPacket( &delete_msg, sizeof(delete_msg) );
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

    if (success_num_out != 0)
    {
        GAMELOG->LogCompose(true, player_, NULL,
                            compose_list->m_byMatCount,
                            const_cast<sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE*>(materials),
                            compose_list->m_Money,
                            item_serial_static, success_num_out);
    }

    // 실패 로그
    // TODO(WAVERIX) (WARNING) 하기 실패 로그 뭐냥? item_serial_static의 연산 부분 문제있다.
    assert(making_num >= success_num_out);
    BYTE fail_num = making_num - success_num_out;
    if (fail_num > 0) {
        GAMELOG->LogCompose(false, player_, NULL,
                            compose_list->m_byMatCount,
                            const_cast<sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE*>(materials),
                            compose_list->m_Money,
                            item_serial_static, fail_num);
    }

    if (making_num == fail_num) {
        compose_result = RC::RC_ITEM_COMPOSE_FAILED;
    }

    // 돈차감 (CHANGES) change type 'DWORD64'->'MONEY'
    MONEY composition_fee = compose_list->m_Money * making_num;
    {
        player_->MinusMoney(composition_fee);
        GAMELOG->LogMoney(MONEY_COMPOSE, player_, composition_fee);
    };

    g_pGameServer->NotifyHeimConsumption(player_->GetCharGuid(),
                                         eHEIM_CONSUMPTION_COMPOSITION, composition_fee);

#ifdef _NA_001990_ACSYSTEM_ADD
    if(compose_list->m_byResultType != compose_list->RESULTTYPE_REWARDCODE)
    {
        const BASE_ITEMINFO* item_info = \
            ItemInfoParser::Instance()->GetItemInfo(compose_list->m_Result.m_ItemCode);

        if (item_info && item_info->m_wACCode)
        {
            player_->GetACManager()->ProcessItemObtain(item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, compose_list->m_Result.m_byItemNum * success_num_out);
        }
    }
#endif //_NA_001990_ACSYSTEM_ADD

    return compose_result;
}

//--------------------------------------------------------------------------------------------------

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
// ItemManager::Crystalize는 사용하지 않는 함수이다 (CrystallizeImplement를 사용함)
#else
RC::eITEM_RESULT
ItemManager::Crystalize(SLOTCODE crystalize_list_code, POSTYPE target_pos,
                        INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    if (IsRestrictFunction(eFUNCTIONALRESTRICT_CRYSTALIZE)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (!player_->GetChaoState().CanUseFuction(CHAO_USE_CRYSTALIZE)) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }
    if (player_->GetLevel() < LIMIT_LEVEL_CRYSTALIZE) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
    }
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (ValidPos(SI_INVENTORY, target_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }

    ItemCrystalizeListParser* const crystalize_list_parser = ItemCrystalizeListParser::Instance();
    const sITEM_CRYSTALIZE_LIST* crystalize_list = \
        crystalize_list_parser->GetCrystalizeListInfo(crystalize_list_code);
    if (crystalize_list == NULL) {
        return RC::RC_ITEM_INVALID_VALUE;
    }

    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& target_item = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos));
    const BASE_ITEMINFO* item_info = target_item.GetItemInfo();
    //FASSERT(item_info);  remove 'FASSERT', assert(item_info)

    if (crystalize_list->m_ItemType != item_info->m_wType) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    if (target_item.CanUseMixture(MIXTURE_CRYSTALIZE) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    // 일반적인 아이템인 경우 1레벨 0랭크 E인챈트인 무기는 분해 불가능
    if (item_info->m_wSubType == eITEMSUBTYPE_GENERAL)
    {
        if (item_info->m_LV == 1 && target_item.GetRank() == RANK_E && target_item.GetEnchant() == 0)
        {
            return RC::RC_ITEM_INVALID_VALUE;
        }
    }

    // 기본적으로 pListInfo->m_ItemLevel = 0값을 가지며, 악세사리의 경우 1, 2의 Level값을 가진다.
    if (crystalize_list->m_ItemLevel && item_info->m_LV != crystalize_list->m_ItemLevel) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    if (crystalize_list->m_EnchantLv != target_item.GetEnchant()) {
        return RC::RC_ITEM_INVALID_VALUE;
    }

    static SLOTCODE obtain_code_list[MAX_INVENTORY_SLOT_NUM];
    static POSTYPE obtain_counts_list[MAX_INVENTORY_SLOT_NUM];
    ZeroMemory(obtain_code_list, sizeof(obtain_code_list));
    ZeroMemory(obtain_counts_list, sizeof(obtain_counts_list));
    // TODO(WAVERIX) (NOTE) #97 하단부 로직이 buffer boundary에 적합한지 정적 분석만으론 확인되지 않았다. 검증할 것.

    POSTYPE total_count = 0;
    MONEY number_of_obtains = 0;

    // 1차 결정체 획득 확률
    for (int i = 0; i < crystalize_list->MAX_RESULT; ++i)
    {
        const sITEM_CRYSTALIZE_LIST::sITEM_FOR_CRYSTALIZE& crystal_result = \
            crystalize_list->m_pResult[i];

        DWORD seed = random(1, 100);
        if (seed > crystal_result.m_ResultRatio) {
            continue;
        }
        if (crystal_result.m_MaterialCode == 0) {
            continue;
        }
        number_of_obtains = crystal_result.m_MaterialCount;

        // 결정체의 금액이 0 이 아닌 경우 아이템의 상점 구입 가격/
        // MaterialMoney로 추가되는 결정체의 갯수가 결정된다.
        if (0 != crystal_result.m_MaterialMoney) {
            number_of_obtains += target_item.GetPriceForBuy() / crystal_result.m_MaterialMoney;
        }

        // 결정체의 갯수가 255가 넘을 경우 다음 슬롯으로 넘긴다.
        // 한 슬롯당 최대 255개의 동일 아이템 수용 가능.
        // 또한 최대 가질 수 있는 결정체의 갯수를 총 2개의 슬롯으로 제한 시킨다.
        number_of_obtains = min(number_of_obtains, UCHAR_MAX*2);
        while(number_of_obtains > UCHAR_MAX)
        {
            obtain_code_list[total_count] = crystal_result.m_MaterialCode;
            obtain_counts_list[total_count] = UCHAR_MAX;
            number_of_obtains -= UCHAR_MAX;
            ++total_count;
        }

        if (number_of_obtains > 0)
        {
            obtain_code_list[total_count] = crystal_result.m_MaterialCode;
            obtain_counts_list[total_count] = static_cast<POSTYPE>(number_of_obtains);
            ++total_count;
        }
    }

    // 랜덤 결정체 획득 확률
    for (int i = 0; i < crystalize_list->MAX_RANDOMRESULT; ++i)
    {
        const sITEM_CRYSTALIZE_LIST::sITEM_FOR_RANDOM_CRYSTALIZE& crystal_random_result = \
            crystalize_list->m_pRandomResult[i];

        DWORD seed = (DWORD)dRandom(1, crystal_random_result.m_RandomStandardRatio);
        if (seed > crystal_random_result.m_RandomRatio) {
            continue;
        }
        if (crystal_random_result.m_MaterialCode == 0) {
            continue;
        }
        // 결정체의 갯수가 255가 넘을 경우 다음 슬롯으로 넘긴다.
        // 한 슬롯당 최대 255개의 동일 아이템 수용 가능.
        // 또한 최대 가질 수 있는 결정체의 갯수를 총 1개의 슬롯으로 제한 시킨다.
        number_of_obtains = min(crystal_random_result.m_MaterialCount, UCHAR_MAX);

        if (number_of_obtains > 0)
        {
            obtain_code_list[total_count] = crystal_random_result.m_MaterialCode;
            obtain_counts_list[total_count] = number_of_obtains;
            ++total_count;
        }
    }

    ASSERT(total_count <= MAX_INVENTORY_SLOT_NUM);

    RC::eITEM_RESULT crystalize_result = RC::RC_ITEM_CRYSTALIZE_FAILED;
    if (0 == total_count)
    {
        // 결정화 실패 로그
        GAMELOG->LogItem(ITEM_CRYSTALIZE_FAILED, player_, &target_item);
        inventory->DeleteSlot(target_pos, NULL);
    }
    else
    {
        crystalize_result = ObtainAllKindOfItem(total_count,
            obtain_code_list, obtain_counts_list,
            inven_total_info);
        if (crystalize_result == RC::RC_ITEM_SUCCESS)
        {
            // 결정화 성공 로그
            GAMELOG->LogCrystalizeSuccess(player_, target_item,
                total_count, obtain_code_list, obtain_counts_list);
            inventory->DeleteSlot(target_pos, NULL);

            crystalize_result =  RC::RC_ITEM_CRYSTALIZE_SUCCESS;
        }
    }

    return crystalize_result;
}
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
bool Sorting_PairVector(const std::pair<POSTYPE, INT> &fst, const std::pair<POSTYPE, INT> &sec)
{
    return (fst.second < sec.second);
}
BOOL ItemManager::DeleteComposeMaterials( ns_functor::DeleteComposeMaterialItem* functor, 
                                         INVEN_DELETE_TOTAL_INFO* OUT delete_item_info)
{
    SLOTIDX slot_index = SI_INVENTORY;

    SCItemSlotContainer* slot_container = static_cast<SCItemSlotContainer*>(slot_manager_->GetSlotContainer(slot_index));
    const SLOTCODE item_code = functor->GetItemCode();
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);

    if(item_info == NULL) {
        return false;
    }


    if (item_info->m_wEqPos == eEQUIPCONTAINER_INVALID)
    {
        // 장비아이템이 아닌 경우
        typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
        const SLOT_LINEAR_LIST& slot_list = slot_container->GetSlotList();
        SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
        for ( ; it != slot_list.end(); ++it)
        {
            const SCItemSlot& item_slot = static_cast<const SCItemSlot&>(*it->slot);
            if (item_slot.GetItemCode() == 0) {
                continue;
            };
            if (functor->operator()(slot_container, item_slot, delete_item_info) == false) {
                continue;
            };
            return true;
        }
    }
    else
    {
        STLX_VECTOR< std::pair<POSTYPE, INT> > found_item;
        
        // 장비아이템인 경우
        const DWORD item_type_index = functor->GetItemTypeIndex();
        INT minimum_enchant_value = 0;

        ItemType* item_type = ItemTypeList::Instance()->FindItemType(item_type_index);
        if (item_type != NULL) {
            // NULL이면 0 유지
            minimum_enchant_value = item_type->GetEnchant();
        }
        if (FlowControl::FCAssert(slot_container != NULL) == false) {
            return false;
        };

        const POSTYPE max_number_of_slots = slot_container->GetMaxSlotNum();
        // 인챈트 조건의 맞으면서 가장 낮은값을 가진 아이템의 좌표를 찾는다
        for (POSTYPE pos = 0; pos < max_number_of_slots; ++pos)
        {
            if (slot_container->IsEmpty(pos)) {
                continue;
            }
            SCItemSlot& slot = static_cast<SCItemSlot&>(slot_container->GetSlot(pos));
            if (slot.GetItemCode() == item_code && slot.GetEnchant() >= minimum_enchant_value)
            {
                // 조건에 맞는 아이템들을 찾아서 vector에 삽입
                found_item.push_back(std::make_pair(pos, slot.GetEnchant()));
            }
        }
        sort(found_item.begin(), found_item.end(), Sorting_PairVector);
        STLX_VECTOR< std::pair<POSTYPE,INT> >::iterator viter = found_item.begin();
        for (viter; viter != found_item.end(); ++viter)
        {
            POSTYPE targetpos = viter->first;
            SCItemSlot& target_slot = static_cast<SCItemSlot&>( slot_container->GetSlot(targetpos) );
            if (functor->operator()(slot_container, target_slot, delete_item_info) == true) {
                return true;
            }
        }
    }
    return false;
}
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION