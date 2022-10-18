#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <ItemInfoParser.h>
#include <ItemTypeList.h>
#include <SocketSystem/ZardComposeInfoParser.h> //_NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002253_20100811_CUBE_SYSTEM
    #include <CubeSystem/CubeSystemParser.h>
    #include "ImplementUtil.h"
#endif //
//
#include <SCItemSlot.h>

#include "DropManager.h"
#include "ItemManager.h"

#include "WarehouseSlotContainer.h"

#include "PacketHandler/Handler_CG_ITEM.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool ProcessPickUp(DWORD player_key, const GmCmdTokens& cmd_array)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    GameField* game_field = NULL;
    if (player == NULL || (game_field = player->GetField()) == NULL) {
        return false;
    }
    // (CHANGES) skip check routine to extension
    /*if (cmd_array.size() != 1)
    {
    MSG_CG_GM_STRING_CMD_NAK msg_nak;
    msg_nak.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
    player->SendPacket(&msg_nak, sizeof(msg_nak));
    return false;
    }*/
    const GM_CMD_STRING::size_type number_of_args = cmd_array.size();
    bool use_duplicable_slot_mode = false;
    if (number_of_args >= 2) {
        use_duplicable_slot_mode = (atoi(cmd_array[1].c_str()) != 0);
    }

    RC::eGM_RESULT gm_result = RC::RC_GM_CMD_SUCCESS;
    if (game_field->PickupAllItems(player, use_duplicable_slot_mode) == false) {
        gm_result = RC::RC_GM_USER_STATE_INVALID;
    }
    else {
        gm_result = RC::RC_GM_CMD_FAILED;
    }
    MSG_CG_GM_PICKUP_ACK msg_ack;
    msg_ack.m_byErrorCode = gm_result;
    player->SendPacket(&msg_ack, sizeof(msg_ack));
    return true;
}

//하임 생성
static bool ProcessCreateHime(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nMoney = atoi(vecCmd[1].c_str());

    if (nMoney > GM_MAX_CREATE_MONEY || nMoney <= 0)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_NOTEXIST_USER; // USER가 존재하지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 떨어뜨릴 위치를 찾는다.
    WzVector vDropPos = pPlayer->GetRandomPosInAround(2);
    // 돈을 떨어뜨린다.
    MONEY money = (MONEY)nMoney;

    if (false == g_DropManager.DropMoneyToField(pField,&vDropPos,NULL,0,money))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_NOTEXIST_USER; // 하임 생성에 실폐하였습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GAMELOG->WriteCreateHeim(pPlayer, money);

    return true;
}

#ifdef UNUSED_SECTION
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
//성공하면 pItemSlot에 RankUp된 아이템이 들어간다.
bool GetRankItem(SCItemSlot* pItemSlot, int nOptionIndex)
{
    // +S 랭크이상 랭크 불가능
    BYTE curRank = pItemSlot->GetRank();

    if (curRank == RANK_PS)
        return false;

    sRANK_OPTION* pRankOption = RankOptionParser::Instance()->GetRankOption(pItemSlot->GetItemInfo()->m_wType, nOptionIndex);
    if (!pRankOption)
        return false;

    if (pRankOption->m_iOptionIndex == 0)
        return false;

    if (pRankOption->m_OptionNCode == 0)
        return false;

    if (pRankOption->m_Value[curRank+1] == 0)
        return false;

    // 랭크 옵션 설정
    pItemSlot->SetRank((eRANK)(curRank + 1));
    pItemSlot->SetRankAttr((eRANK)(curRank + 1), pRankOption);

    // 소켓 옵션 설정
    ItemManager::CreateRankItemSocketHole(pItemSlot, curRank);

    return true;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#endif //

// 인첸트 아이템 생성
static bool ProcessCreateEnchantItem(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    if (vecCmd.size() != 4)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }   

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    int nItemCode = atoi(vecCmd[1].c_str());
    int nItemCount = atoi(vecCmd[2].c_str());
    int nItemLevel = atoi(vecCmd[3].c_str());

    if (nItemCount > GM_MAX_CREATE_ITEM_NUM || nItemCount <= 0)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    if (!ItemInfoParser::Instance()->GetItemInfo(nItemCode))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    //인첸트 아이템 레벨 체크
    if (nItemLevel < 0 || nItemLevel > MAX_ENCHANT_GRADE)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 개수만큼 인첸트 아이템을 필드에 떨어뜨린다.
    for (int i = 0; i < nItemCount; ++i)
    {
        WzVector vDropPos = pPlayer->GetRandomPosInAround(2);
        SCItemSlot slot;
        slot.SetSerial(TEMP_DBSERIAL_VALUE);
        slot.SetCode(nItemCode);
        slot.SetEnchant(nItemLevel);
        g_DropManager.DropItemToField(pField, &vDropPos, NULL, 0, slot);
    }

    return true;
}

static bool ProcessCreateFateItem(DWORD player_key, const GmCmdTokens& vecCmd)
{
    SCItemSlot itemSlot;
    itemSlot.SetSerial(TEMP_DBSERIAL_VALUE);

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;
    const int kMaxNumberOfArgs = 7;
    //인자개수 체크
    int nArgSize = (int)vecCmd.size();
    if (nArgSize < 3 || nArgSize > kMaxNumberOfArgs)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }   

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    //아이템코드 체크
    int nItemCode = atoi(vecCmd[1].c_str()); 
    BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(nItemCode);
    if (!pItemInfo)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }
    else
        itemSlot.SetCode(nItemCode);

    //아이템수량 체크
    int nItemCount = atoi(vecCmd[2].c_str());
    if (nItemCount <= 0 || nItemCount > GM_MAX_CREATE_ITEM_NUM)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 내구도 등급
    int DuraRatio = 0;
    if (nArgSize > 3)
    {
        DuraRatio = atoi(vecCmd[3].c_str()); 
        if (DuraRatio <=0 || DuraRatio > MAX_FATE_DURA_RATIO_COUNT)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_NOT_MATCH_FATE_RATE;
            pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
            return false;
        }
        //명령어는 1~x 배열은 0~x이므로
        DuraRatio--;
    }
    else
    {
        DuraRatio = random(0, MAX_FATE_DURA_RATIO_COUNT-1);
    }   

    // 방어/뎀 등급
    int AtkDefRatio = 0;
    if (nArgSize > 4)
    {
        AtkDefRatio = atoi(vecCmd[4].c_str()); 
        if (AtkDefRatio <=0 || AtkDefRatio > MAX_FATE_ATK_DEF_RATIO_COUNT)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_NOT_MATCH_FATE_RATE; // 없는 리미티드 인덱스
            pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
            return false;
        }
        //명령어는 1~x 배열은 0~x이므로
        AtkDefRatio--;
    }
    else
    {
        AtkDefRatio = random(0, MAX_FATE_ATK_DEF_RATIO_COUNT-1);
    }

    //기간 등급  
    // (f100716.1L) changes a period value rule to support expiriation item test
    bool use_period_ratio = true;
    uint16_t period_second = 60 * 60;
    int PeriodRatio = 0;
    if (nArgSize > 5)
    {
        PeriodRatio = atoi(vecCmd[5].c_str());
        if (PeriodRatio < 0 || PeriodRatio > MAX_FATE_USING_TIME_RATIO_COUNT)
        {
            if(PeriodRatio == 4) // 10초(기간만료 테스트용)
            {
                use_period_ratio = false;
                period_second = 10;
            }
            else
            {
                MSG_CG_GM_STRING_CMD_NAK NakMsg;
                NakMsg.m_byErrorCode    =  RC::RC_GM_NOT_MATCH_FATE_RATE; // 없는 리미티드 인덱스
                pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
                return false;
            }
        }
        if (PeriodRatio == 0) {
            use_period_ratio = false;
        }
        PeriodRatio--;
    }
    else
    {
        PeriodRatio = random(0, MAX_FATE_USING_TIME_RATIO_COUNT-1);
    }

    sFATE_ITEM_INFO* pFateItemInfo = FateItemInfoParser::Instance()->GetFateItemInfoByRefItemCode(nItemCode);
    if (pFateItemInfo == NULL)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_CANNOT_FIND_FATE_INDEX; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    INT iRandomValue = 0;
    iRandomValue = random(pFateItemInfo->m_wDura_Min[DuraRatio], pFateItemInfo->m_wDura_Max[DuraRatio]);
    itemSlot.SetFateDuraMax((WORD)(pItemInfo->m_Dura * (float)iRandomValue / 100.0f));
    itemSlot.SetDura(itemSlot.GetMaxDura());

    iRandomValue = random(pFateItemInfo->m_wAtkDef_Min[AtkDefRatio], pFateItemInfo->m_wAtkDef_Max[AtkDefRatio]);
    itemSlot.SetFateAtkDefRatio(iRandomValue); 
    // (f100716.1L) changes a period value rule to support expiriation item test
    if (use_period_ratio)
    {
        DWORD period = pFateItemInfo->m_dwUsingTimeValue[PeriodRatio];
        itemSlot.SetDateTime(period * 24 * 60 * 60);
        itemSlot.SetFatePeriod(static_cast<WORD>(period));
    }
    else
    {
        itemSlot.SetDateTime(period_second);
        itemSlot.SetFatePeriod(period_second);
    }

    itemSlot.SetFateIdentify(true);
    if (nArgSize > 6)
    {
        int item_type = atoi(vecCmd[6].c_str());
        nsSlot::ItemTypeChanger::ChangeItemByType(&itemSlot, item_type);
    }
    // 개수만큼 인첸트 아이템을 필드에 떨어뜨린다.
    int nFailCount = 0;

    for (int i = 0; i < nItemCount; ++i)
    {
        WzVector vDropPos = pPlayer->GetRandomPosInAround(2);

        if (false == g_DropManager.DropItemToField(pField,&vDropPos,NULL,0,itemSlot))
            ++nFailCount;
    }

    GAMELOG->WriteCreateItem(pPlayer, nItemCode, nItemCount-nFailCount);

    return true;
}

//생성
//
static bool ProcessCreateItemEx(DWORD player_key, const GmCmdTokens& cmd_list)
{
    // GM 명령어 형식 : make 키워드 다음에 아래 순서대로 입력
    // 예제 : make 1001 1 0 0 0 1
    enum GMCmdIndex 
    {
        kItemCode = 1, // 아이템 코드
        kNumberOfItem = 2, // 아이템 개수
        kEnableOverlapped = 3, // 겹치기 여부(아니오:0) (생략가능)
        kItemType = 4, kDeprecatedItemGrade = kItemType, // (생략가능)
        kEnchantLevel = 5, // 강화 수준 (생략가능)
        // _NA_000251_20100727_SOCKET_SYSTEM 기준 소켓 개수 또는 랭크(삭제예정) (생략가능)
        kNumberOfSocket = 6, kDeprecatedRank = kNumberOfSocket,
        // CHANGES: f110327.1L
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
		kAwakeningGrade = 7,
		kTimerType = 8,
#else
        kTimerType = 7,
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        // upperbound
        kGmItemCmdArgCounts
    };

    // @history:
    //  - 100523|waverix|code arrangement with the item_type controller for the item customizing|
    //                  |changes an overlapped item packaging logic|Sample='make 4001 1000 1'
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
    //
    GameField* const game_field = player->GetField();
    if (game_field == NULL) {
        return false;
    }
    //
    struct ErrorHandler {
        ErrorHandler(Player* player)
            : player_(player),
            gm_result(RC::RC_GM_CMD_SUCCESS)
        {}
        ~ErrorHandler() {
            if (gm_result == RC::RC_GM_CMD_SUCCESS) {
                return;
            }
            MSG_CG_GM_STRING_CMD_NAK msg_nak;
            msg_nak.m_byErrorCode = static_cast<BYTE>(gm_result);
            player_->SendPacket(&msg_nak, sizeof(msg_nak));
        }

        Player* player_;
        RC::eGM_RESULT gm_result;
    } gm_parse_error(player);
    // #rule : check number of arguments
    const GM_CMD_STRING::difference_type number_of_args = cmd_list.size();
    if (number_of_args <= kNumberOfItem || number_of_args > kGmItemCmdArgCounts) {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false;
    };
    //
    SCItemSlot item_slot;
    item_slot.SetSerial(TEMP_DBSERIAL_VALUE);

    // #rule : check the making item code
    CODETYPE item_code4 = atoi(cmd_list[kItemCode].c_str());
    const BASE_ITEMINFO* const item_info = ItemInfoParser::Instance()->GetItemInfo(item_code4);
    if (item_info == NULL) {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false;
    }
    const SLOTCODE item_code = static_cast<SLOTCODE>(item_code4);
    item_slot.SetCode(item_code);
    //
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false;
    }
#if SUN_CODE_BACKUP
    // #rule : the default time expiration information
    switch (item_info->m_ChargeSubType)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
    case eCHARGE_SUB_FIXED_QUANTITY:
    case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
    case eCHARGE_SUB_FATE:
    case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_RIDER_WITH_PART:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        // 60분 적용
        item_slot.SetDateTime(60 * 60);
        break;
    }
#endif
    // #rule : check number of items
    const int number_of_items = atoi(cmd_list[kNumberOfItem].c_str());
    if (number_of_items <= 0 || number_of_items > GM_MAX_CREATE_ITEM_NUM) {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false;
    }

    // -----------------------------
    // configure variable arguments
    // #rule : make the package item if a item can overlapped.
    bool need_packaging = (number_of_args > kEnableOverlapped) ?
        (atoi(cmd_list[kEnableOverlapped].c_str()) != 0) : false;
    const bool enable_overlapped = item_slot.IsOverlap() != false;
    if (need_packaging) {
        need_packaging = enable_overlapped;
    }

    //
    int item_type = nsSlot::ItemTypeChanger::Default;
    if (number_of_args > kItemType) {
        item_type = atoi(cmd_list[kItemType].c_str());
    }
    if (enable_overlapped == false) {
        nsSlot::ItemTypeChanger::ChangeItemByType(&item_slot, item_type);
    }

    if (enable_overlapped == false && item_rule_info.IsEnableEnchant())
    {
        int enchant_level = 0; //인첸트등급 기본 0
        if (number_of_args > kEnchantLevel) //인첸트정보가 포함되어 있으면..
        {
            enchant_level = atoi(cmd_list[kEnchantLevel].c_str()); //인첸트 등급
            //인첸트 아이템 레벨 체크
            if (enchant_level < 0 || enchant_level > MAX_ENCHANT_GRADE) {
                gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
                return false;
            }
            item_slot.SetEnchant(static_cast<BYTE>(enchant_level));
        }
    };

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // 소켓을 생성할 수 있는 아이템이면 소켓을 입력한 개수만큼 생성한다.
    // 입력하지 않으면 원래 규칙대로 램덤하게 생성된다.
    if (number_of_args > kNumberOfSocket)
    {
        if (enable_overlapped == false && item_rule_info.IsEnableSocket())
        {
            const int num_of_socket = atoi(cmd_list[kNumberOfSocket].c_str());
            if (num_of_socket < 0 || num_of_socket > SOCKET_MAX)
            {
                gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
                return false;
            }
            if (num_of_socket == 0)
            {
                // 개수에 0을 입력하면 소켓 미확인 상태로 생성한다.
                item_slot.SetSocketIdentifyType(eSOCKET_UNIDENTIFIED_ITEM);
            }
            else
            {
                item_slot.SetSocketNum(static_cast<BYTE>(num_of_socket));
                item_slot.SetSocketIdentifyType(eSOCKET_IDENTIFIED_ITEM);
            }
        }
    }
#else
    if (enable_overlapped == false &&
        item_info->IsElite() == false &&
        item_rule_info.IsEnableRank())
    {
        if (number_of_args > kDeprecatedRank)
        {
            const int max_rank = atoi(cmd_list[kDeprecatedRank].c_str());
            if (max_rank < 0 || max_rank >= RANK_MAX) {
                gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
                return false;
            }
            ItemManager* item_manager = player->GetItemManager();
            for (int rank = RANK_E; rank < max_rank; ++rank) {
                item_manager->ItemRankUp(&item_slot);
            }
        }
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
	if (number_of_args > kAwakeningGrade)
	{
		int awakening = atoi(cmd_list[kAwakeningGrade].c_str());
		if (awakening < 0 || awakening > MAX_AWAKENING_GRADE)
		{
			gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
			return false;
		}
		item_slot.SetAwakening(awakening);
	}
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#if SUN_CODE_RENEWAL
    // #rule : the default time expiration information
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    DWORD timeout_interval = 60 * 60; // 1 hour
    //
    if (number_of_args > kTimerType)
    {
        int making_timeout = atoi(cmd_list[kTimerType].c_str());
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        if (making_timeout != 0)
        {
            if (making_timeout < 0 || (item_slot.ChangeItemToUsingTimer() == false)) {
                gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
                return false;
            };
            timeout_interval = making_timeout;
        }
#else
        if (making_timeout < 0 || (item_slot.ChangeItemToUsingTimer() == false)) {
            gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
            return false;
        };
        if (making_timeout != 0) {
            timeout_interval = making_timeout;
        };
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    }
    switch (item_rule_info.charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
    case eCHARGE_SUB_FIXED_QUANTITY:
    case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
    case eCHARGE_SUB_FATE:
    case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_RIDER_WITH_PART:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        // 60분 적용
        item_slot.SetDateTime(timeout_interval);
        break;
    }
#endif

    if (need_packaging)
    {
        const DURATYPE max_contains_of_items = item_slot.GetMaxNum();
        item_slot.SetNum(max_contains_of_items);
        int remains = number_of_items;
        for (; remains >= max_contains_of_items; remains -= max_contains_of_items)
        {
            const WzVector* drop_pos = &player->GetRandomPosInAround(2);
            if (g_DropManager.DropItemToField(game_field, drop_pos, 0, 0, item_slot) == false) {
                continue;
            }
            GAMELOG->WriteCreateItem(player, item_code, max_contains_of_items);
        }
        if (remains)
        {
            item_slot.SetNum(static_cast<DURATYPE>(remains));
            const WzVector* drop_pos = &player->GetRandomPosInAround(2);
            if (g_DropManager.DropItemToField(game_field, drop_pos, 0, 0, item_slot)) {
                GAMELOG->WriteCreateItem(player, item_code, static_cast<DURATYPE>(remains));
            }
        }
    }
    else
    {
        for (int loop = number_of_items; loop; --loop)
        {
            const WzVector* drop_pos = &player->GetRandomPosInAround(2);
            if (g_DropManager.DropItemToField(game_field, drop_pos, 0, 0, item_slot)) {
                GAMELOG->WriteCreateItem(player, item_code, 1);
            }
        }
    }

    return true;
}

static bool ProcessCreateLimitedItem(DWORD player_key, const GmCmdTokens& cmd_list)
{
    // 형식(한글) : 리미티드생성 [아이템 코드] [리미티드 인덱스] [수량] (디바인 여부 0|1) (인챈트 수준) (랭크) (내구도)
    // 형식(영문) : createlimited [item code] [limited index] [quantity] (divine flag 0|1) (enchant level) (rank) (durability)
    enum CmdIndex 
    {
        kItemCode = 1, // 아이템 코드
        KLimitedIndex = 2, // 리미티드 인덱스
        kQuantity = 3, // 수량
        kDivineFlag = 4, // 디바인 여부
        kEnchantLevel = 5, // 인첸트 수준
        // _NA_000251_20100727_SOCKET_SYSTEM 기준 소켓 개수 또는 랭크(삭제예정)
        kNumberOfSocket = 6, kDeprecatedRank = kNumberOfSocket, 
        kDurability = 7, // 내구도
        kMaxNumberOfCmd
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (gm == NULL)
    {
        return false;
    }
    GameField* const field = gm->GetField();
    if (field == NULL)
    {
        return false;
    }

    struct ErrorHandler {
        ErrorHandler(Player* player)
            : player_(player),
            gm_result(RC::RC_GM_CMD_SUCCESS)
        {}
        ~ErrorHandler() {
            if (gm_result == RC::RC_GM_CMD_SUCCESS) {
                return;
            }
            MSG_CG_GM_STRING_CMD_NAK msg_nak;
            msg_nak.m_byErrorCode = static_cast<BYTE>(gm_result);
            player_->SendPacket(&msg_nak, sizeof(msg_nak));
        }

        Player* player_;
        RC::eGM_RESULT gm_result;
    } gm_parse_error(gm);

    const int cmd_size = cmd_list.size();
    if (cmd_size < 4 || cmd_size > kMaxNumberOfCmd)
    {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false; 
    }

    // 1. 아이템 코드
    const CODETYPE item_code = atoi(cmd_list[kItemCode].c_str()); 
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false;
    }
    SCItemSlot drop_item;
    drop_item.SetSerial(TEMP_DBSERIAL_VALUE);
    drop_item.SetCode(static_cast<SLOTCODE>(item_code));
        
    // 2. 리미티드 인덱스
    const int limited_index = atoi(cmd_list[KLimitedIndex].c_str());
    if (LimitedItemInfoParser::Instance()->GetLimitedItemInfo(limited_index) == NULL)
    {
        gm_parse_error.gm_result = RC::RC_GM_NOT_EXIST_LIMITED_INDEX;
        return false;
    }
    drop_item.SetLimitedIndex(limited_index);

    // 3. 수량
    const int quantity = atoi(cmd_list[kQuantity].c_str());
    if (quantity <= 0 || quantity > GM_MAX_CREATE_ITEM_NUM)
    {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false; 
    }

    // 4. 디바인 여부
    if (cmd_size > kDivineFlag)
    {
        const int divine_flag = atoi(cmd_list[kDivineFlag].c_str());
        if (divine_flag < 0 || divine_flag > MAX_ITEM_GRADE)
        {
            gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
            return false;
        }
        drop_item.SetSet(static_cast<BYTE>(divine_flag));
    }

    // 5. 인첸트 레벨
    if (cmd_size > kEnchantLevel)
    {
        const int enchant_level = atoi(cmd_list[kEnchantLevel].c_str());
        if (enchant_level < 0 || enchant_level > MAX_ENCHANT_GRADE)
        {
            gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
            return false; 
        }
        drop_item.SetEnchant(enchant_level);
    }

    // 6. 소켓 개수 / 랭크
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    if (cmd_size > kNumberOfSocket)
    {
        const int num_of_socket = atoi(cmd_list[kNumberOfSocket].c_str());
        if (num_of_socket < 0 || num_of_socket > SOCKET_MAX)
        {
            gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
            return false;
        }
        if (num_of_socket == 0)
        {
            drop_item.SetSocketIdentifyType(eSOCKET_UNIDENTIFIED_ITEM);
        }
        else
        {
            drop_item.SetSocketNum(static_cast<BYTE>(num_of_socket));
            drop_item.SetSocketIdentifyType(eSOCKET_IDENTIFIED_ITEM);
        }
    }
#else
    if (cmd_size > kDeprecatedRank)
    {
        const int rank = atoi(cmd_list[kDeprecatedRank].c_str());
        if (rank < RANK_F || rank >= RANK_MAX)
        {
            gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
            return false;
        }
        drop_item.SetRank(static_cast<eRANK>(rank));
    }
#endif

    // 7. 내구도
    if (cmd_size > kDurability)
    {       
        drop_item.SetDura(atoi(cmd_list[kDurability].c_str()));
    }

    int fail_count = 0;
    for (int i = 0; i < quantity; ++i)
    {
        const WzVector drop_pos = gm->GetRandomPosInAround(2);
        if (g_DropManager.DropItemToField(field, &drop_pos, NULL, 0, drop_item) == false)
        {
            ++fail_count;
        }
    }
    GAMELOG->WriteCreateItem(gm, static_cast<SLOTCODE>(item_code), quantity-fail_count);

    return true;
}

static bool ProcessExItem(DWORD player_key, const GmCmdTokens& vecCmd)
{
    ASSERT(!"Function not implementation");
    return false;
}

static bool ProcessRemovalItem(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    // 반경 30m 내의 아이템들을 삭제한다.
    pField->KillAllItem(pPlayer, 30);

    GAMELOG->WriteRemovalItem(pPlayer);

    return true;
}

static bool ProcessRemoveInven(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 1)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    //.//

    MSG_CG_ITEM_LOSE_ACK loseMsg;

    if (false == pPlayer->GetItemManager()->DeleteAllInvenItem(&loseMsg.m_ItemInfo))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    pPlayer->SendPacket(&loseMsg, loseMsg.GetSize());

    return true;
}

//내구도 [내구도] [아이템위치]
static bool ProcessStrengthCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//

    INT Dura = 1;
    eEQUIPCONTAINER_POS eEquipcontainerPos = eEQUIPCONTAINER_WEAPON;

    if (nArgSize > 1)        
        Dura = atoi(vecCmd[1].c_str());

    if (nArgSize > 2)        
        eEquipcontainerPos  = (eEQUIPCONTAINER_POS)atoi(vecCmd[2].c_str());

    if (nsSlot::DurabilityImpl* const impl = pPlayer->GetItemManager()->GetDurabilityImpl()) {
        impl->ChangeDurabilityForGM(eEquipcontainerPos, Dura);
    };

    return true;
}

// 하임제거 [인벤토리하임셋팅값]
static bool ProcessRemoveHimeCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    MONEY InventoryMoney = 0;
    if (nArgSize > 1)
        InventoryMoney = (MONEY)atoi(vecCmd[1].c_str());

    //.//

    pPlayer->MinusMoney(pPlayer->GetMoney());
    pPlayer->PlusMoney(InventoryMoney);

    //.//

    MSG_CG_WAREHOUSE_MONEY_ACK msg;
    msg.m_InventoryMoney = pPlayer->GetMoney();
    msg.m_WarehouseMoney = pPlayer->GetWarehouseContainer()->GetCurMoney();
    pPlayer->SendPacket(&msg, sizeof(msg));

    //.//
    if (InventoryMoney) 
        GAMELOG->WriteCreateHeim(pPlayer, InventoryMoney);

    return true;
}


// //인챈트 [인챈수] [인벤위치]
static bool ProcessEnchandCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    int     nEnchant  = 1;
    POSTYPE TargetPos = 0;

    if (nArgSize > 1)        
        nEnchant = atoi(vecCmd[1].c_str());

    if (nArgSize > 2)        
        TargetPos = (POSTYPE)atoi(vecCmd[2].c_str());

    SCSlotContainer * pInventory = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot & rTargetSlot = (SCItemSlot &)pInventory->GetSlot(TargetPos);

    nEnchant = rTargetSlot.GetEnchant() + nEnchant;

    if (nEnchant < 0 || 
        nEnchant > MAX_ENCHANT_GRADE)   
        return false;

    //(lst1024)(080918)(2447)인챈트시 내구도를 다시 셋팅 해준다.
    rTargetSlot.SetEnchantUpdateDura(nEnchant);
    pPlayer->GetItemManager()->SendItemChange(rTargetSlot);

    return true;
}

static bool ProcessComposition(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (!util::internal::scoped_check_input_in_gm(
        gm, 
        gm_cmd_arguments, 
        2, 
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kMaxMaterialSize + 1, 
        false))
    {
        return false;
    }

    // GM 커맨드 형식 : composition 키워드 다음에 아래 순서대로 입력
    enum GMCmdIndex 
    { 
        kRequestType = 1, // 요청종류(쟈드:1, 큐브:2)
        kIsComposition = 2, // 합성여부(합성:1, 분해:2)
        kReservedCode = 3, // 예약코드(쟈드합성일때만 첨가제 위치, 그 외는 0)
        kUsingItemPosition = 4, // 아이템을 사용해서 요청했다면 아이템 위치
        kMaterialPosition = 5 // 재료 위치
    };

    // 요청 종류를 얻는다.
    int request_type = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kRequestType], request_type);
    if (request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest && 
        request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest)
    {
        return false;
    }

    // 합성 분해 여부를 얻는다.
    int is_composition = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kIsComposition], is_composition);
    if (is_composition != 1 && is_composition != 2)
    {
        return false;
    }

    // 예약코드를 얻는다.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::ReservedCode reserved_code = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kReservedCode], reserved_code);

    // 사용 아이템 위치를 얻는다.
    POSTYPE using_item_position = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kUsingItemPosition], using_item_position);

    // 사용자가 입력한 재료 개수를 얻는다.
    const int user_inputed_material_size = gm_cmd_arguments.size() - kMaterialPosition;
    if (user_inputed_material_size < 1)
    {
        return false;
    }

    // 인벤토리를 얻는다.
    ItemManager* const item_manager = gm->GetItemManager();
    if (!item_manager->ValidState())
    {
        return false;
    }
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    // 요청을 얻는다.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN request;
    request.m_dwKey = gm->GetUserKey();

    switch (request_type)
    {
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
case MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest:
    {
        // 재료 개수를 검사한다.
        if (user_inputed_material_size != ZardComposeInfoParser::kMaterialSize)
        {
            return false;
        }

        // 쟈드의 인벤토리 위치를 얻는다.
        POSTYPE zard_position_list[ZardComposeInfoParser::kMaterialSize];
        {
            for (int zard_index = 0; zard_index < user_inputed_material_size; ++zard_index)
            {
                const int gm_cmd_index = zard_index + kMaterialPosition;
                util::internal::scoped_string_to_number(gm_cmd_arguments[gm_cmd_index], zard_position_list[zard_index]);
            }
        }

        const ZardComposeInfoParser* const zard_compose_parser = ZardComposeInfoParser::Instance();
        if (!zard_compose_parser->GetZardCompositionRequest(
            *inventory, zard_position_list, static_cast<POSTYPE>(reserved_code), request))
        {
            return false;
        }
    }
    break;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002253_20100811_CUBE_SYSTEM
case MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest:
    {
        // 재료 개수를 검사한다.
        if (user_inputed_material_size > CompositionInfo::MaterialInfo::kMaxSize)
        {
            return false;
        }

        // 재료의 인벤토리 위치 목록을 얻는다.
        MaterialInventoryPositionList inventory_position_list;
        {
            for (MaterialInventoryPositionList::size_type material_index = 0; 
                material_index < user_inputed_material_size; 
                ++material_index)
            {
                const int gm_cmd_index = material_index + kMaterialPosition;
                POSTYPE inventory_position = 0;
                util::internal::scoped_string_to_number(gm_cmd_arguments[gm_cmd_index], inventory_position);
                inventory_position_list.push_back(inventory_position);
            }
        }

        const CubeSystemParser* const cube_parser = CubeSystemParser::Instance();
        if (!cube_parser->GetRequest(
            *inventory, (is_composition == 1), inventory_position_list, using_item_position, request))
        {
            return false;
        }
    }
    break;
#endif // _NA_002253_20100811_CUBE_SYSTEM
default:
    return false;
    }

    // 처리기를 호출한다.
    Handler_CG_ITEM::OnCG_ITEM_COMPOSE_OR_DECOMPOSE_SYN(NULL, &request, request.GetSize());

    return true;
}

static bool ProcessCompositionList(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
    enum { kArgumentSize = 3 };
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (!util::internal::scoped_check_input_in_gm(gm, gm_cmd_arguments, kArgumentSize+1, kArgumentSize+1, false))
    {
        return false;
    }

    // GM 명령어 형식 : composition_list 키워드 다음에 아래 순서대로 입력
    enum GMCmdIndex 
    { 
        kRequestType = 1, // 요청 종류(쟈드:1, 큐브:2)
        kIsComposition = 2, // 합성 여부(아니오:0, 예:1)
        kIsModifyTest = 3, // 수정 기능 테스트 여부(아니오:0, 예:1)
    };

    // 요청 종류를 얻는다.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN::RequestType request_type = 0;
    {
        util::internal::scoped_string_to_number(gm_cmd_arguments[kRequestType], request_type);
        if (request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest && 
            request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest)
        {
            return false;
        }
    }

    // 합성 여부를 얻는다.
    bool is_composition = false;
    {
        int cmd_is_composition = 0;
        util::internal::scoped_string_to_number(gm_cmd_arguments[kIsComposition], cmd_is_composition);
        if (cmd_is_composition != 0 && cmd_is_composition != 1)
        {
            return false;
        }
        is_composition = (cmd_is_composition == 1);
    }

    // 수정 기능 테스트 여부를 얻는다.
    bool is_modify_test = false;
    {
        int cmd_is_modify_test = 0;
        util::internal::scoped_string_to_number(gm_cmd_arguments[kIsModifyTest], cmd_is_modify_test);
        if (cmd_is_modify_test != 0 && cmd_is_modify_test != 1)
        {
            return false;
        }
        is_modify_test = (cmd_is_modify_test == 1);
    }

    // 내역 수정 기능을 테스트한다.
    if (is_modify_test)
    {
        // 수정 메시지를 만든다.
        MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD modify_histories_cmd;
        {
            modify_histories_cmd.Init();
            modify_histories_cmd.m_dwKey = gm->GetUserKey();
            modify_histories_cmd.condition_list_size = 2;

            ItemCompositionOrDecompositionHistory::ModifyCondition& condition1 = 
                modify_histories_cmd.modify_condition_list[0];
            condition1.history_id = 7;
            condition1.is_memorize = false;

            ItemCompositionOrDecompositionHistory::ModifyCondition& condition2 = 
                modify_histories_cmd.modify_condition_list[1];
            condition2.history_id = 9;
            condition2.is_memorize = false;
        }

        // 처리기를 호출한다.
        Handler_CG_ITEM::OnCG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD(
            NULL, &modify_histories_cmd, modify_histories_cmd.GetSize());
        return true;
    }
    
    // 메시지를 만든다.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN get_histories_syn;
    {
        get_histories_syn.Init();
        get_histories_syn.m_dwKey = gm->GetUserKey();
        get_histories_syn.request_type = request_type;
        get_histories_syn.is_composition = is_composition;
    }

    // 처리기를 호출한다.
    Handler_CG_ITEM::OnCG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN(
        NULL, &get_histories_syn, get_histories_syn.GetSize());

    return true;
}

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
static bool ProcessIdentifySocket(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (!util::internal::scoped_check_input_in_gm(gm, gm_cmd_arguments, 1, 2, false))
    {
        return false;
    }

    // GM 커맨드 형식 : identify_socket 키워드 다음에 아래 순서대로 입력
    enum GMCmdIndex 
    {
        kUnidentifiedItemPos = 1, // 미확인 아이템 위치
        kIdentifyItemPos = 2 // 아이템을 사용하여 확인할 때 확인 아이템 위치 (옵션)
    };

    MSG_CG_ITEM_IDENTIFY_SOCKET_CMD request;
    {
        request.m_dwKey = gm->GetUserKey();
        util::internal::scoped_string_to_number(
            gm_cmd_arguments[kUnidentifiedItemPos], request.unidentified_item_pos_);

        if (gm_cmd_arguments.size() == 3)
        {
            // 아이템을 사용했다면 확인 아이템 위치를 얻는다.
            request.use_identify_item_ = true;
            util::internal::scoped_string_to_number(
                gm_cmd_arguments[kIdentifyItemPos], request.identify_item_pos_);
        }
        else
        {
            request.use_identify_item_ = false;
            request.identify_item_pos_ = INVALID_POSTYPE_VALUE;
        }
    }

    // 처리기를 호출한다.
    Handler_CG_ITEM::OnCG_ITEM_IDENTIFY_SOCKET_CMD(NULL, &request, request.GetSize());

    return false;
}

static bool ProcessRecreateSocket(DWORD player_key, const GmCmdTokens& cmd_args)
{
    // 명령어 형식 : recreate_socket 키워드 다음에 아래 순서대로 입력
    enum CmdIndex 
    {
        kTargetItemPos = 1, // 소켓을 다시 생성할 아이템의 인벤토리 위치
        kSocketNumber, // [생략가능] 다시 설정할 소켓 개수 (0이면 규칙에 따라 램덤 생성)
        kRecreateItemPos, // [생략가능] 소켓다시생성 아이템의 인벤토리 위치
        kNumOfMaxArgs
    };

    // 명령어 동작 방식
    enum CmdMode
    {
        kUnknownMode, 

        // 소켓을 규칙에 따라 다시 생성한다. 
        // 예제> recreate_socket 2 // 인벤토리 2번 위치에 있는 아이템의 소켓을 다시 생성한다.
        kRecreateSocket, 
        
        // 소켓 개수를 다시 설정한다.
        // 예제> recreate_socket 2 3 // 인벤토리 2번 위치에 있는 아이템의 소켓 개수를 3개로 설정한다.
        kSetSocketNumber, 
        
        // 소켓을 아이템을 사용하여 다시 생성한다.
        // recreate_socket 2 0 3 // 인벤토리 2번 위치에 있는 아이템의 소켓을, 
        // 인벤토리 3번 위치에 있는 소켓다시생성아이템(헤르메시즘의 성서 등)을 사용해서 다시 생성한다.
        kRecreateSocketByItem 
    };

    using namespace util::internal;

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (scoped_check_input_in_gm(gm, cmd_args, 1, kNumOfMaxArgs, false) == false)
    {
        return false;
    }

    POSTYPE target_item_pos = INVALID_SLOT_POS;
    scoped_string_to_number(cmd_args[kTargetItemPos], target_item_pos);
    if (target_item_pos == INVALID_SLOT_POS)
    {
        return false;
    }

    CmdMode cmd_mode = kUnknownMode;
    {
        const uint8_t args_size = cmd_args.size();
        if (args_size == kTargetItemPos+1)
        {
            cmd_mode = kRecreateSocket;
        }
        else if (args_size == kSocketNumber+1)
        {
            cmd_mode = kSetSocketNumber;
        }
        else if (args_size == kRecreateItemPos+1)
        {
            cmd_mode = kRecreateSocketByItem;
        }
        else
        {
            return false;
        }
    }

    switch (cmd_mode)
    {
    case kRecreateSocket:
    case kSetSocketNumber:
        {
            ItemManager* const item_manager = gm->GetItemManager();
            if (item_manager->ValidPos(SI_INVENTORY, target_item_pos) == false)
            {
                return false;
            }
            BYTE num_of_socket = 0;
            if (cmd_mode == kSetSocketNumber)
            {
                scoped_string_to_number(cmd_args[kSocketNumber], num_of_socket);
            }
            SCItemSlotContainer* const inven = item_manager->GetItemSlotContainer(SI_INVENTORY);
            SCItemSlot& target_slot = static_cast<SCItemSlot&>(inven->GetSlot(target_item_pos));
            if (target_slot.CreateSocketItem(num_of_socket, true) == false)
            {
                return false;
            }
            item_manager->SendItemChange(target_slot);
        }
        break;
    case kRecreateSocketByItem:
        {
            MSG_CG_ITEM_RECREATE_SOCKET_SYN request;
            request.Init();
            request.m_dwKey = gm->GetUserKey();
            scoped_string_to_number(cmd_args[kRecreateItemPos], request.request_item_pos);
            request.target_item_pos = target_item_pos;
            Handler_CG_ITEM::OnCG_ITEM_RECREATE_SOCKET_SYN(NULL, &request, request.GetSize());
        }
        break;
    default:
        return false;
    }

    return true;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

static bool ProcessExRider(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (pPlayer == 0)
        return false;

    nsGM::CmdTokenizer::Route(pPlayer, vecCmd);
    return true;
}

#ifdef _NA_002253_20100811_CUBE_SYSTEM
static bool ProcessCreateCubeMaterial(DWORD player_key, const GmCmdTokens& cmd_args)
{
    // GM 커맨드 형식 : create_cube_material 키워드 다음에 아래 순서대로 입력
    enum GMCmdIndex 
    {
        kCompositionCode = 1, // 합성코드
        kNumOfMaxArgs
    };

    using namespace util::internal;

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (scoped_check_input_in_gm(gm, cmd_args, 1, kNumOfMaxArgs, false) == false)
    {
        return false;
    }

    CompositionInfo::CompositionCode composition_code(CompositionInfo::kUnused);
    scoped_string_to_number(cmd_args[kCompositionCode], composition_code);

    const CubeSystemParser* const cube_parser = CubeSystemParser::Instance();
    const CompositionInfo* const composition = cube_parser->FindComposition(composition_code);
    if (composition == NULL)
    {
        return false;
    }

    typedef ItemCompositionOrDecompositionReward MaterialRewards;
    MaterialRewards material_rewards;
    if (material_rewards.Init(gm, MaterialRewards::kItemCodeReward) == false)
    {
        return false;
    }

    FOREACH_CONTAINER(
        const CompositionInfo::MaterialInfo& material, 
        composition->material_list_, 
        CompositionInfo::MaterialList)
    {
        if (material.is_used_ == false)
        {
            continue;
        }        
        MaterialRewards::ItemReward material_reward;
        {
            if (material.code_ != CompositionInfo::kUnused)
            {
                material_reward.item_code = static_cast<MaterialRewards::RewardItemCode>(material.code_);
            }
            else if (material.type_code_ != CompositionInfo::kUnused)
            {
                // 타입에 일치하는 아이템 코드를 찾는다.
                bool is_find = false;
                {
                    const ItemInfoParser* const item_parser = ItemInfoParser::Instance();
                    item_parser->SetFirst();
                    const BASE_ITEMINFO* material_info(NULL);
                    while ((material_info = item_parser->GetNext()) != NULL)
                    {
                        if (material.type_code_ == material_info->m_wType)
                        {
                            material_reward.item_code = material_info->m_Code;
                            is_find = true;
                            break;
                        }
                    }
                }
                if (is_find == false)
                {
                    return false;
                }
            }
            else if (material.level_ != CompositionInfo::kUsed)
            {
                // 레벨에 일치하는 아이템 코드를 찾는다.
                bool is_find = false;
                {
                    const ItemInfoParser* const item_parser = ItemInfoParser::Instance();
                    item_parser->SetFirst();
                    const BASE_ITEMINFO* material_info(NULL);
                    while ((material_info = item_parser->GetNext()) != NULL)
                    {
                        if (material.level_ == material_info->m_LV)
                        {
                            material_reward.item_code = material_info->m_Code;
                            is_find = true;
                            break;
                        }
                    }
                }
                if (is_find == false)
                {
                    return false;
                }
            }
            material_reward.number_of_item = material.number_;
            material_reward.ig_type = material.ig_type_;
        }
        if (material_rewards.AddItemReward(material_reward) == false)
        {
            return false;
        }
    }
    // CHANGES: f110216.3L
    MSG_CG_ITEM_OBTAIN_ITEM_CMD obtain_cmd;
    if (INVENTORY_TOTAL_INFO* inven_total_info = &obtain_cmd.m_tagInventory)
    {
        inven_total_info->m_InvenCount = 0;
        inven_total_info->m_TmpInvenCount = 0;
    }
    //
    const RC::eITEM_RESULT obtain_result = material_rewards.ObtainRewardToInventory(
        MaterialRewards::kDoNotUseLog, obtain_cmd.m_tagInventory);
    if (obtain_result != RC::RC_ITEM_SUCCESS)
    {
        return false;
    }
    gm->SendPacket(&obtain_cmd, obtain_cmd.GetSize());

    return true;
}
#endif // _NA_002253_20100811_CUBE_SYSTEM

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterSlot(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "생성", "make",  &ProcessCreateItemEx), // 아이템 생성
        GMHANDLER_MAPPING(true, "하임", "heim",  &ProcessCreateHime), // 하임생성
        GMHANDLER_MAPPING(true, "아이템제거", "removei",  &ProcessRemovalItem), // 아이템 제거
        GMHANDLER_MAPPING(true, "인첸트만들기", "createenchant",  &ProcessCreateEnchantItem), // 인첸트 아이템 생성
        GMHANDLER_MAPPING(true, "줍기", "pickup",  &ProcessPickUp),
        GMHANDLER_MAPPING(true, "리미티드생성", "createlimited",  &ProcessCreateLimitedItem),
        GMHANDLER_MAPPING(true, "럭키생성", "createlucky",  &ProcessCreateFateItem),
        GMHANDLER_MAPPING(true, "인벤제거", "removeinven",  &ProcessRemoveInven),
        GMHANDLER_MAPPING(true, "내구도", "strength",  &ProcessStrengthCmd),
        GMHANDLER_MAPPING(true, "하임제거", "removeh",  &ProcessRemoveHimeCmd),
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessEnchandCmd),
        GMHANDLER_MAPPING(true, NULL, "rider",  &ProcessExRider),
        GMHANDLER_MAPPING(true, "아이템", "item", &ProcessExItem),
        GMHANDLER_MAPPING(false, NULL, "composition",  &ProcessComposition), // EP2 아이템 합성/분해
        // unused
        GMHANDLER_MAPPING(false, NULL, "composition_list", &ProcessCompositionList), // EP2 최근에 성공한 합성 목록 얻기
    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        GMHANDLER_MAPPING(false, NULL, "identify_socket",  &ProcessIdentifySocket), 
        GMHANDLER_MAPPING(true, "소켓다시생성", "recreate_socket", &ProcessRecreateSocket), 
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
    #ifdef _NA_002253_20100811_CUBE_SYSTEM
        GMHANDLER_MAPPING(true, "큐브재료생성", "create_cube_material", &ProcessCreateCubeMaterial), 
    #endif // _NA_002253_20100811_CUBE_SYSTEM
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
