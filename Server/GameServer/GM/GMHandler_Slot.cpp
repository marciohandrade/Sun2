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

//���� ����
static bool ProcessCreateHime(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nMoney = atoi(vecCmd[1].c_str());

    if (nMoney > GM_MAX_CREATE_MONEY || nMoney <= 0)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_NOTEXIST_USER; // USER�� �������� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // ����߸� ��ġ�� ã�´�.
    WzVector vDropPos = pPlayer->GetRandomPosInAround(2);
    // ���� ����߸���.
    MONEY money = (MONEY)nMoney;

    if (false == g_DropManager.DropMoneyToField(pField,&vDropPos,NULL,0,money))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_NOTEXIST_USER; // ���� ������ �����Ͽ����ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GAMELOG->WriteCreateHeim(pPlayer, money);

    return true;
}

#ifdef UNUSED_SECTION
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
//�����ϸ� pItemSlot�� RankUp�� �������� ����.
bool GetRankItem(SCItemSlot* pItemSlot, int nOptionIndex)
{
    // +S ��ũ�̻� ��ũ �Ұ���
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

    // ��ũ �ɼ� ����
    pItemSlot->SetRank((eRANK)(curRank + 1));
    pItemSlot->SetRankAttr((eRANK)(curRank + 1), pRankOption);

    // ���� �ɼ� ����
    ItemManager::CreateRankItemSocketHole(pItemSlot, curRank);

    return true;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#endif //

// ��þƮ ������ ����
static bool ProcessCreateEnchantItem(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    if (vecCmd.size() != 4)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
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
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    if (!ItemInfoParser::Instance()->GetItemInfo(nItemCode))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    //��þƮ ������ ���� üũ
    if (nItemLevel < 0 || nItemLevel > MAX_ENCHANT_GRADE)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // ������ŭ ��þƮ �������� �ʵ忡 ����߸���.
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
    //���ڰ��� üũ
    int nArgSize = (int)vecCmd.size();
    if (nArgSize < 3 || nArgSize > kMaxNumberOfArgs)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }   

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    //�������ڵ� üũ
    int nItemCode = atoi(vecCmd[1].c_str()); 
    BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(nItemCode);
    if (!pItemInfo)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }
    else
        itemSlot.SetCode(nItemCode);

    //�����ۼ��� üũ
    int nItemCount = atoi(vecCmd[2].c_str());
    if (nItemCount <= 0 || nItemCount > GM_MAX_CREATE_ITEM_NUM)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // ������ ���
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
        //��ɾ�� 1~x �迭�� 0~x�̹Ƿ�
        DuraRatio--;
    }
    else
    {
        DuraRatio = random(0, MAX_FATE_DURA_RATIO_COUNT-1);
    }   

    // ���/�� ���
    int AtkDefRatio = 0;
    if (nArgSize > 4)
    {
        AtkDefRatio = atoi(vecCmd[4].c_str()); 
        if (AtkDefRatio <=0 || AtkDefRatio > MAX_FATE_ATK_DEF_RATIO_COUNT)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_NOT_MATCH_FATE_RATE; // ���� ����Ƽ�� �ε���
            pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
            return false;
        }
        //��ɾ�� 1~x �迭�� 0~x�̹Ƿ�
        AtkDefRatio--;
    }
    else
    {
        AtkDefRatio = random(0, MAX_FATE_ATK_DEF_RATIO_COUNT-1);
    }

    //�Ⱓ ���  
    // (f100716.1L) changes a period value rule to support expiriation item test
    bool use_period_ratio = true;
    uint16_t period_second = 60 * 60;
    int PeriodRatio = 0;
    if (nArgSize > 5)
    {
        PeriodRatio = atoi(vecCmd[5].c_str());
        if (PeriodRatio < 0 || PeriodRatio > MAX_FATE_USING_TIME_RATIO_COUNT)
        {
            if(PeriodRatio == 4) // 10��(�Ⱓ���� �׽�Ʈ��)
            {
                use_period_ratio = false;
                period_second = 10;
            }
            else
            {
                MSG_CG_GM_STRING_CMD_NAK NakMsg;
                NakMsg.m_byErrorCode    =  RC::RC_GM_NOT_MATCH_FATE_RATE; // ���� ����Ƽ�� �ε���
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
        NakMsg.m_byErrorCode    =  RC::RC_GM_CANNOT_FIND_FATE_INDEX; // ���ڰ� ���� �ʽ��ϴ�.
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
    // ������ŭ ��þƮ �������� �ʵ忡 ����߸���.
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

//����
//
static bool ProcessCreateItemEx(DWORD player_key, const GmCmdTokens& cmd_list)
{
    // GM ��ɾ� ���� : make Ű���� ������ �Ʒ� ������� �Է�
    // ���� : make 1001 1 0 0 0 1
    enum GMCmdIndex 
    {
        kItemCode = 1, // ������ �ڵ�
        kNumberOfItem = 2, // ������ ����
        kEnableOverlapped = 3, // ��ġ�� ����(�ƴϿ�:0) (��������)
        kItemType = 4, kDeprecatedItemGrade = kItemType, // (��������)
        kEnchantLevel = 5, // ��ȭ ���� (��������)
        // _NA_000251_20100727_SOCKET_SYSTEM ���� ���� ���� �Ǵ� ��ũ(��������) (��������)
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
        // 60�� ����
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
        int enchant_level = 0; //��þƮ��� �⺻ 0
        if (number_of_args > kEnchantLevel) //��þƮ������ ���ԵǾ� ������..
        {
            enchant_level = atoi(cmd_list[kEnchantLevel].c_str()); //��þƮ ���
            //��þƮ ������ ���� üũ
            if (enchant_level < 0 || enchant_level > MAX_ENCHANT_GRADE) {
                gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
                return false;
            }
            item_slot.SetEnchant(static_cast<BYTE>(enchant_level));
        }
    };

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // ������ ������ �� �ִ� �������̸� ������ �Է��� ������ŭ �����Ѵ�.
    // �Է����� ������ ���� ��Ģ��� �����ϰ� �����ȴ�.
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
                // ������ 0�� �Է��ϸ� ���� ��Ȯ�� ���·� �����Ѵ�.
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
        // 60�� ����
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
    // ����(�ѱ�) : ����Ƽ����� [������ �ڵ�] [����Ƽ�� �ε���] [����] (����� ���� 0|1) (��æƮ ����) (��ũ) (������)
    // ����(����) : createlimited [item code] [limited index] [quantity] (divine flag 0|1) (enchant level) (rank) (durability)
    enum CmdIndex 
    {
        kItemCode = 1, // ������ �ڵ�
        KLimitedIndex = 2, // ����Ƽ�� �ε���
        kQuantity = 3, // ����
        kDivineFlag = 4, // ����� ����
        kEnchantLevel = 5, // ��þƮ ����
        // _NA_000251_20100727_SOCKET_SYSTEM ���� ���� ���� �Ǵ� ��ũ(��������)
        kNumberOfSocket = 6, kDeprecatedRank = kNumberOfSocket, 
        kDurability = 7, // ������
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

    // 1. ������ �ڵ�
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
        
    // 2. ����Ƽ�� �ε���
    const int limited_index = atoi(cmd_list[KLimitedIndex].c_str());
    if (LimitedItemInfoParser::Instance()->GetLimitedItemInfo(limited_index) == NULL)
    {
        gm_parse_error.gm_result = RC::RC_GM_NOT_EXIST_LIMITED_INDEX;
        return false;
    }
    drop_item.SetLimitedIndex(limited_index);

    // 3. ����
    const int quantity = atoi(cmd_list[kQuantity].c_str());
    if (quantity <= 0 || quantity > GM_MAX_CREATE_ITEM_NUM)
    {
        gm_parse_error.gm_result = RC::RC_GM_INVALID_ARGUMENT;
        return false; 
    }

    // 4. ����� ����
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

    // 5. ��þƮ ����
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

    // 6. ���� ���� / ��ũ
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

    // 7. ������
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

    if (vecCmd.size() != 1)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    // �ݰ� 30m ���� �����۵��� �����Ѵ�.
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

    //���ڰ��� üũ
    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 1)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    //.//

    MSG_CG_ITEM_LOSE_ACK loseMsg;

    if (false == pPlayer->GetItemManager()->DeleteAllInvenItem(&loseMsg.m_ItemInfo))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    pPlayer->SendPacket(&loseMsg, loseMsg.GetSize());

    return true;
}

//������ [������] [��������ġ]
static bool ProcessStrengthCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
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

// �������� [�κ��丮���Ӽ��ð�]
static bool ProcessRemoveHimeCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
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


// //��æƮ [��æ��] [�κ���ġ]
static bool ProcessEnchandCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
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

    //(lst1024)(080918)(2447)��æƮ�� �������� �ٽ� ���� ���ش�.
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

    // GM Ŀ�ǵ� ���� : composition Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    { 
        kRequestType = 1, // ��û����(���:1, ť��:2)
        kIsComposition = 2, // �ռ�����(�ռ�:1, ����:2)
        kReservedCode = 3, // �����ڵ�(����ռ��϶��� ÷���� ��ġ, �� �ܴ� 0)
        kUsingItemPosition = 4, // �������� ����ؼ� ��û�ߴٸ� ������ ��ġ
        kMaterialPosition = 5 // ��� ��ġ
    };

    // ��û ������ ��´�.
    int request_type = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kRequestType], request_type);
    if (request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest && 
        request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest)
    {
        return false;
    }

    // �ռ� ���� ���θ� ��´�.
    int is_composition = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kIsComposition], is_composition);
    if (is_composition != 1 && is_composition != 2)
    {
        return false;
    }

    // �����ڵ带 ��´�.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::ReservedCode reserved_code = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kReservedCode], reserved_code);

    // ��� ������ ��ġ�� ��´�.
    POSTYPE using_item_position = 0;
    util::internal::scoped_string_to_number(gm_cmd_arguments[kUsingItemPosition], using_item_position);

    // ����ڰ� �Է��� ��� ������ ��´�.
    const int user_inputed_material_size = gm_cmd_arguments.size() - kMaterialPosition;
    if (user_inputed_material_size < 1)
    {
        return false;
    }

    // �κ��丮�� ��´�.
    ItemManager* const item_manager = gm->GetItemManager();
    if (!item_manager->ValidState())
    {
        return false;
    }
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    // ��û�� ��´�.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN request;
    request.m_dwKey = gm->GetUserKey();

    switch (request_type)
    {
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
case MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest:
    {
        // ��� ������ �˻��Ѵ�.
        if (user_inputed_material_size != ZardComposeInfoParser::kMaterialSize)
        {
            return false;
        }

        // ����� �κ��丮 ��ġ�� ��´�.
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
        // ��� ������ �˻��Ѵ�.
        if (user_inputed_material_size > CompositionInfo::MaterialInfo::kMaxSize)
        {
            return false;
        }

        // ����� �κ��丮 ��ġ ����� ��´�.
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

    // ó���⸦ ȣ���Ѵ�.
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

    // GM ��ɾ� ���� : composition_list Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    { 
        kRequestType = 1, // ��û ����(���:1, ť��:2)
        kIsComposition = 2, // �ռ� ����(�ƴϿ�:0, ��:1)
        kIsModifyTest = 3, // ���� ��� �׽�Ʈ ����(�ƴϿ�:0, ��:1)
    };

    // ��û ������ ��´�.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN::RequestType request_type = 0;
    {
        util::internal::scoped_string_to_number(gm_cmd_arguments[kRequestType], request_type);
        if (request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest && 
            request_type != MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest)
        {
            return false;
        }
    }

    // �ռ� ���θ� ��´�.
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

    // ���� ��� �׽�Ʈ ���θ� ��´�.
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

    // ���� ���� ����� �׽�Ʈ�Ѵ�.
    if (is_modify_test)
    {
        // ���� �޽����� �����.
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

        // ó���⸦ ȣ���Ѵ�.
        Handler_CG_ITEM::OnCG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD(
            NULL, &modify_histories_cmd, modify_histories_cmd.GetSize());
        return true;
    }
    
    // �޽����� �����.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN get_histories_syn;
    {
        get_histories_syn.Init();
        get_histories_syn.m_dwKey = gm->GetUserKey();
        get_histories_syn.request_type = request_type;
        get_histories_syn.is_composition = is_composition;
    }

    // ó���⸦ ȣ���Ѵ�.
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

    // GM Ŀ�ǵ� ���� : identify_socket Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    {
        kUnidentifiedItemPos = 1, // ��Ȯ�� ������ ��ġ
        kIdentifyItemPos = 2 // �������� ����Ͽ� Ȯ���� �� Ȯ�� ������ ��ġ (�ɼ�)
    };

    MSG_CG_ITEM_IDENTIFY_SOCKET_CMD request;
    {
        request.m_dwKey = gm->GetUserKey();
        util::internal::scoped_string_to_number(
            gm_cmd_arguments[kUnidentifiedItemPos], request.unidentified_item_pos_);

        if (gm_cmd_arguments.size() == 3)
        {
            // �������� ����ߴٸ� Ȯ�� ������ ��ġ�� ��´�.
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

    // ó���⸦ ȣ���Ѵ�.
    Handler_CG_ITEM::OnCG_ITEM_IDENTIFY_SOCKET_CMD(NULL, &request, request.GetSize());

    return false;
}

static bool ProcessRecreateSocket(DWORD player_key, const GmCmdTokens& cmd_args)
{
    // ��ɾ� ���� : recreate_socket Ű���� ������ �Ʒ� ������� �Է�
    enum CmdIndex 
    {
        kTargetItemPos = 1, // ������ �ٽ� ������ �������� �κ��丮 ��ġ
        kSocketNumber, // [��������] �ٽ� ������ ���� ���� (0�̸� ��Ģ�� ���� ���� ����)
        kRecreateItemPos, // [��������] ���ϴٽû��� �������� �κ��丮 ��ġ
        kNumOfMaxArgs
    };

    // ��ɾ� ���� ���
    enum CmdMode
    {
        kUnknownMode, 

        // ������ ��Ģ�� ���� �ٽ� �����Ѵ�. 
        // ����> recreate_socket 2 // �κ��丮 2�� ��ġ�� �ִ� �������� ������ �ٽ� �����Ѵ�.
        kRecreateSocket, 
        
        // ���� ������ �ٽ� �����Ѵ�.
        // ����> recreate_socket 2 3 // �κ��丮 2�� ��ġ�� �ִ� �������� ���� ������ 3���� �����Ѵ�.
        kSetSocketNumber, 
        
        // ������ �������� ����Ͽ� �ٽ� �����Ѵ�.
        // recreate_socket 2 0 3 // �κ��丮 2�� ��ġ�� �ִ� �������� ������, 
        // �κ��丮 3�� ��ġ�� �ִ� ���ϴٽû���������(�츣�޽����� ���� ��)�� ����ؼ� �ٽ� �����Ѵ�.
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
    // GM Ŀ�ǵ� ���� : create_cube_material Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    {
        kCompositionCode = 1, // �ռ��ڵ�
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
                // Ÿ�Կ� ��ġ�ϴ� ������ �ڵ带 ã�´�.
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
                // ������ ��ġ�ϴ� ������ �ڵ带 ã�´�.
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
        GMHANDLER_MAPPING(true, "����", "make",  &ProcessCreateItemEx), // ������ ����
        GMHANDLER_MAPPING(true, "����", "heim",  &ProcessCreateHime), // ���ӻ���
        GMHANDLER_MAPPING(true, "����������", "removei",  &ProcessRemovalItem), // ������ ����
        GMHANDLER_MAPPING(true, "��þƮ�����", "createenchant",  &ProcessCreateEnchantItem), // ��þƮ ������ ����
        GMHANDLER_MAPPING(true, "�ݱ�", "pickup",  &ProcessPickUp),
        GMHANDLER_MAPPING(true, "����Ƽ�����", "createlimited",  &ProcessCreateLimitedItem),
        GMHANDLER_MAPPING(true, "��Ű����", "createlucky",  &ProcessCreateFateItem),
        GMHANDLER_MAPPING(true, "�κ�����", "removeinven",  &ProcessRemoveInven),
        GMHANDLER_MAPPING(true, "������", "strength",  &ProcessStrengthCmd),
        GMHANDLER_MAPPING(true, "��������", "removeh",  &ProcessRemoveHimeCmd),
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessEnchandCmd),
        GMHANDLER_MAPPING(true, NULL, "rider",  &ProcessExRider),
        GMHANDLER_MAPPING(true, "������", "item", &ProcessExItem),
        GMHANDLER_MAPPING(false, NULL, "composition",  &ProcessComposition), // EP2 ������ �ռ�/����
        // unused
        GMHANDLER_MAPPING(false, NULL, "composition_list", &ProcessCompositionList), // EP2 �ֱٿ� ������ �ռ� ��� ���
    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        GMHANDLER_MAPPING(false, NULL, "identify_socket",  &ProcessIdentifySocket), 
        GMHANDLER_MAPPING(true, "���ϴٽû���", "recreate_socket", &ProcessRecreateSocket), 
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
    #ifdef _NA_002253_20100811_CUBE_SYSTEM
        GMHANDLER_MAPPING(true, "ť��������", "create_cube_material", &ProcessCreateCubeMaterial), 
    #endif // _NA_002253_20100811_CUBE_SYSTEM
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
