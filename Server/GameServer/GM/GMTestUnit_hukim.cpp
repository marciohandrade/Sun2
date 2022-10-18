#include "stdafx.h"
#include "GMCmdManager.h"
//
#include "ItemManager.h"
#include "InventorySlotContainer.h"
#include "PacketHandler/Handler_CG_ITEM.h"
#include "PacketHandler/Handler_CG_Skill.h"
#include "RandomItemParser.h"
#include "DominationInfoParser.h"
//
//
// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool TestUnit4hukim(DWORD player_key, const GmCmdTokens& /*cmd_tokens*/)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
    DISPMSG("My identification is hukim, gooooooooooooooooogle\n");
    return true;
}

// �����ۿ� ���� ��带 �����Ѵ�.
static bool ExtractZard(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    // GM ��ɾ� ���� : extract_zard Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    { 
        kExtractItemPosition = 1, // ��� ���� �������� �κ��丮 ��ġ
        kZardOwnerPosition = 2, // ������ ��尡 ���� �ִ� �������� �κ��丮 ��ġ
        kSocketNumber = 3, // ��带 ������ ���� ��ġ
        kArgumentSize
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (!scoped_check_input_in_gm(gm, cmd_tokens, kArgumentSize, kArgumentSize, true))
    {
        return false;
    }

    MSG_CG_ITEM_SOCKET_EXTRACT_SYN extract_syn;
    {
        extract_syn.m_dwKey = gm->GetUserKey();
        scoped_string_to_number(cmd_tokens[kExtractItemPosition], extract_syn.m_ChargeIntemPos);
        scoped_string_to_number(cmd_tokens[kZardOwnerPosition], extract_syn.m_TargetPos);
        scoped_string_to_number(cmd_tokens[kSocketNumber], extract_syn.m_SocketNum);
    }    
    Handler_CG_ITEM::OnCG_ITEM_SOCKET_EXTRACT_SYN(NULL, &extract_syn, sizeof(extract_syn));
    return true;
}

// �κ��丮�� �����Ѵ�.
static bool SortInventory(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const cmd_manager = GMCmdManager::Instance();
    Player* const gm = cmd_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(gm, cmd_tokens, 1, 1, true) == false)
    {
        return false;
    }

    ItemManager* const item_manager = gm->GetItemManager();
    SCItemSlot item_slots[nsSlot::ItemQuery::kMaxNumberOfPosSlots];
    POSTYPE number_of_items = 0;    
    
    // �κ��丮���� ������ �����۵��� ���� �� �����Ѵ�.
    {
        MSG_CG_ITEM_LOSE_ACK item_lose_ack;
        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
        const POSTYPE max_slot_num = inventory->GetMaxSlotNum();
        for (POSTYPE i = 0; i < max_slot_num; ++i)
        {
            if (inventory->IsEmpty(i))
            {
                continue;
            }
            const SCItemSlot& before_slot = static_cast<const SCItemSlot&>(inventory->GetSlot(i));
            if (before_slot.IsLocked() || before_slot.IsBlocked() || before_slot.IsUserBlocked())
            {
                continue;
            }
            SCItemSlot& item_slot = item_slots[number_of_items];
            if (inventory->DeleteSlot(i, &item_slot) == false) 
            {
                continue;
            }
            INVEN_DELETE_TOTAL_INFO::SLOT_TYPE& inventory_slot = 
                item_lose_ack.m_ItemInfo.m_Slot[number_of_items];
            {
                inventory_slot.m_Pos = item_slot.GetPos();
                inventory_slot.m_Num = item_slot.GetNum();
            }
            ++number_of_items;
            if (number_of_items > _countof(item_slots))
            {
                break;
            }
        }
        if (number_of_items < 1)
        {
            return true;
        }
        item_lose_ack.m_ItemInfo.m_Count = number_of_items;
        gm->SendPacket(&item_lose_ack, item_lose_ack.GetSize());
    }

    // ������ �����۵��� ������ �ڵ����� ���ĵǾ� ����.
    // �κ� ���� ���� ���� �߻� X -> ��ġ�Ⱑ �����ϸ� �������� ������ �ʿ� ������ �پ���.
    // �ٽ� ��⿡ �����ؼ� �ȵȴ�. � ��쿡 ������ �� ������?
    {
        using nsSlot::ItemQuery;
        ItemQuery query;
        if (ItemQuery::MakeQueryInverseSelect(&query, gm, SI_INVENTORY) == false) 
        {
            return false;
        }
        for (POSTYPE i = 0; i < number_of_items; ++i)
        {
            const SCItemSlot& item_slot = item_slots[i];
            const int registered_index = query.AddArg_ItemSlot(item_slot, item_slot.GetNum(), false);
            if (registered_index < 0) 
            {
                continue;
            }
        }
        const nsSlot::ItemQueryResult result = item_manager->Query(&query);
        if (result.result_of_inverse_select == 0) 
        {
            return false;
        }
        if (item_manager->ObtainAllKindOfItemByQuery(query) == false) 
        {
            return false;
        }
        // �ӽ÷� Pick ���� ���
        MSG_CG_ITEM_PICK_ACK msg_ack;
        query.MakeSerializedStream(&msg_ack.m_ItemInfo);
        gm->SendPacket(&msg_ack, msg_ack.GetSize());
    }

    return true;
}

// �������� ����ȭ�Ѵ�.
static bool RandomizeItem(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    // GM ��ɾ� ���� : randomize_item Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    { 
        kRequestPos = 1, // ����ȭ �������� �κ��丮 ��ġ
        kTargetPos = 2, // ���� ��� �������� �κ��丮 ��ġ
        kArgumentSize
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (!scoped_check_input_in_gm(gm, cmd_tokens, kArgumentSize, kArgumentSize, true))
    {
        return false;
    }

    MSG_CG_ITEM_RANDOMIZE_SYN request;
    {
        request.m_dwKey = gm->GetUserKey();
        POSTYPE request_item_pos = 0;
        POSTYPE target_item_pos = 0;
        scoped_string_to_number(cmd_tokens[kRequestPos], request_item_pos);
        scoped_string_to_number(cmd_tokens[kTargetPos], target_item_pos);

        ItemManager* const item_manager = gm->GetItemManager();
        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

        const RandomItemParser* const parser = RandomItemParser::Instance();
        if (parser->GetItemRandomizeRequest(
            *inventory, request_item_pos, target_item_pos, request) == false)
        {
            return false;
        }
    }
    Handler_CG_ITEM::OnCG_ITEM_RANDOMIZE_SYN(NULL, &request, request.GetSize());
    return true;
}

// ��þƮ ����� �׽�Ʈ�Ѵ�.
static bool TestEnchant(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
#ifdef _NA_003966_20111227_ADD_ENCHANT
    // GM ��ɾ� ���� : test_enchant Ű���� ������ �Ʒ� ������� �Է�
    enum GMCmdIndex 
    { 
        kNPCCode = 1, 
        kTargetPos = 2, 
        kSuccessRateIndex = 3, 
        kDownGuardItemPos = 4, 
        kArgumentSize
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (!scoped_check_input_in_gm(gm, cmd_tokens, kArgumentSize, kArgumentSize, true))
    {
        return false;
    }

    MSG_CG_ITEM_ENCHANT_SYN request;
    {
        request.Init();
        request.m_dwKey = gm->GetUserKey();
        scoped_string_to_number(cmd_tokens[kNPCCode], request.m_NPCCode);
        scoped_string_to_number(cmd_tokens[kTargetPos], request.m_TargetPos);
        scoped_string_to_number(cmd_tokens[kSuccessRateIndex], request.m_SuccessRateIndex);
        scoped_string_to_number(cmd_tokens[kDownGuardItemPos], request.down_guard_item_pos);
    }
    Handler_CG_ITEM::OnCG_ITEM_ENCHANT_SYN(NULL, &request, request.GetSize());
#endif
    return true;
}

// ��ų�� ����.
static bool LearnSkill(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 1, 
        kMaxArgumentSize = MAX_SKILL_COUNT
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(gm, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    MSG_CG_SKILL_SELECT_SKILLPOINT_SYN request;
    const int skill_code_size = cmd_tokens.size()-1;
    if (skill_code_size > _countof(request.skill_code_array))
    {
        return false;
    }

    // �޽����� �����.
    {
        ::ZeroMemory(&request, sizeof(request));
        request.m_dwKey = gm->GetUserKey();
        
        for (int i = 1; i <= skill_code_size; ++i)
        {
            int skill_code = 0;
            scoped_string_to_number(cmd_tokens[i], skill_code);
            if (skill_code != 0)
            {
                request.skill_code_array[i-1] = skill_code;
            }
        }
        request.skill_count = skill_code_size;
    }
    Handler_CG_SKILL::OnCG_SKILL_SELECT_SKILLPOINT_SYN(NULL, &request, request.GetSize());
    
    return true;
}

static bool TestDominationRewardOption(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgs = 1, 
        kMaxArgs = 50
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(gm, cmd_tokens, kMinArgs, kMaxArgs, true) == false)
    {
        return false;
    }
    
    const size_t taken_map_size = cmd_tokens.size()-1;
    if (taken_map_size == 0)
    {
        return false;
    }

    // ������ �������� �Է��Ѵ�.
    DominationInfoParser::TakenMaps taken_maps;
    {
        for (size_t i = 1; i <= taken_map_size; ++i)
        {
            int temp_map_code = 0;
            scoped_string_to_number(cmd_tokens[i], temp_map_code);
            if (temp_map_code != 0)
            {
                MAPCODE taken_map_code = static_cast<MAPCODE>(temp_map_code);
                taken_maps.push_back(taken_map_code);
            }
        }
    }
    if (taken_maps.size() < 1)
    {
        return false;
    }

    // ������ ���� �ɼ��� ��´�.
    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    const DominationInfoParser::SelectedRewardOptions& sel_options = 
        parser->GetRewardOption(taken_maps);
    if (sel_options.size() < 1)
    {
        return false;
    }
    
    // ������ ���� �ɼ��� �����Ѵ�.
    for (DominationInfoParser::SelectedRewardOptions::const_iterator it = sel_options.begin(); 
        it != sel_options.end(); ++it)
    {
        const DominationInfoParser::SelectedRewardOptionKind option_kind = it->first;
        if (option_kind == 0)
        {
            return false;
        }
        const DominationInfoParser::SelectedRewardOption& option = it->second;
        if (option.domination_info == NULL)
        {
            return false;
        }
        if (option.option_index == 0)
        {
            return false;
        }
        if (option.option_info == NULL)
        {
            return false;
        }
    }
    
    return true;
}

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterTestUnit_hukim(const HandlerNode** node_array,
                                          size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "������", "hukim", &TestUnit4hukim), 
        GMHANDLER_MAPPING(true, NULL, "extract_zard", &ExtractZard), // �����ۿ� ���� ��带 �����Ѵ�.
        GMHANDLER_MAPPING(true, NULL, "sort_inventory", &SortInventory),
        GMHANDLER_MAPPING(true, NULL, "randomize_item", &RandomizeItem),
        GMHANDLER_MAPPING(true, NULL, "test_enchant", &TestEnchant), 
        GMHANDLER_MAPPING(true, NULL, "learn_skill", &LearnSkill), 
        GMHANDLER_MAPPING(true, NULL, "test_domination_reward_option", &TestDominationRewardOption), 
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}