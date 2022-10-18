#include "stdafx.h"
#include "GMCmdManager.h"
//
#include "PacketHandler/Handler_CG_ITEM.h"
//
// Internal utilities
#include "./GMHandler_InterUtils.hxx"
#include "ItemManager.h"
#include "SCItemSlotContainer.h"
#include "StatusManager.h"
#include "GameInstanceDungeon.h"
#include <World/ChaosZoneSystem/ChaosZoneManager.h>
#include "./GMHandler_InterUtils.hxx"
#include <./ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>

namespace nsGM {
;

static bool TestUnit4jaykang(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
    DISPMSG("My identification is jaykang, gooooooooooooooooogle?\n");
    return true;
}



static bool ItemUse(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 2,    // 사용할 아이템 인벤토리 위치..
        kMaxArgumentSize = 2,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;

    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    MSG_CG_ITEM_USE_SYN request;
    {
        request.m_dwKey = player->GetUserKey();
        request.m_atIndex = SI_INVENTORY;
        scoped_string_to_number(cmd_tokens[1], request.m_atPos);
    }
    Handler_CG_ITEM::OnCG_ITEM_USE_SYN(NULL, &request, sizeof(MSG_CG_ITEM_USE_SYN));
    
    return true;
}

static bool ReleaseLock(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 2,    // 해제할 아이템 인벤토리 위치..
        kMaxArgumentSize = 2,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;

    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    POSTYPE inventory_pos = INVALID_POSTYPE_VALUE;
    scoped_string_to_number(cmd_tokens[1], inventory_pos);

    ItemManager* const item_manager = player->GetItemManager();
    SCSlotContainer* slot_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& slot = (SCItemSlot&)slot_container->GetSlot(inventory_pos);
    
    slot.SetBlocked(false);
    slot.SetLock(false);

    return true;
}

static bool ApplyStatus(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 2,    // 해제할 아이템 인벤토리 위치..
        kMaxArgumentSize = 3,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;

    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    int status_type = 0;
    scoped_string_to_number(cmd_tokens[1], status_type);

    int expire_time = 0;
    scoped_string_to_number(cmd_tokens[2], expire_time);

    if (expire_time == 0) {
        expire_time = 5000;
    }

    StatusManager* const status_manager = player->GetStatusManager();
    ALLOC_STATUS_PAIR steel = 
        status_manager->AllocStatus(static_cast<eCHAR_STATE_TYPE>(status_type), expire_time);        
    if (steel.second)
        steel.second->SendStatusAddBRD();

    return true;
}

static bool RepairAll(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 1,
        kMaxArgumentSize = 1,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    RC::eITEM_RESULT rt = RC::RC_ITEM_FAILED;
    if (nsSlot::DurabilityImpl* const impl = player->GetItemManager()->GetDurabilityImpl()) {
        rt = impl->Repair(true, SI_EQUIPMENT, 0);
    };

    if (rt == RC::RC_ITEM_SUCCESS)
    {
        MSG_CG_ITEM_REPAIR_DURA_ACK ackMsg;
        ackMsg.m_bAllRepair = true;
        ackMsg.m_SlotIndex = SI_EQUIPMENT;
        ackMsg.m_SlotPos = 0;
        ackMsg.m_Money = player->GetMoney();
        player->SendPacket( &ackMsg, sizeof(ackMsg) );
    }
    else
    {
        MSG_CG_ITEM_REPAIR_DURA_NAK msg;
        ProtectorInventory& rPROTECT = player->GetWarehouseInventoryProtector().INVENTORY();
        if( !rPROTECT.IsUsable() )
            msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        else
            msg.m_byErrorCode = rt;
        player->SendPacket(&msg,sizeof(msg));
    }

    return true;
}


static bool BattleGroundVote(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 1,
        kMaxArgumentSize = 3,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    enum cmd_mode {
        initiate,
        vote,
        van,
        max_cmd_mode,
    };

    cmd_mode mode = max_cmd_mode;
    const char* cmd_str = cmd_tokens[1].c_str();

    if (strcmp(cmd_str, "발의") == 0 || strcmp(cmd_str, "initiate") == 0)
        mode = initiate;
    
    else if (strcmp(cmd_str, "투표") == 0 || strcmp(cmd_str, "vote") == 0)
        mode = vote;

    else if (strcmp(cmd_str, "추방") == 0 || strcmp(cmd_str, "van_player") == 0)
        mode = van;


    switch(mode)
    {
    case initiate:
        {
            const TCHAR* van_payer_name = cmd_tokens[2].c_str();
            
            if (player->IsThereBattleGroundZone() == false) {
                return false;
            }

            ChaosZone* chaos_zone = player->GetCurrentChaosZone();
            if (chaos_zone == NULL) {
                return false;
            }

            BattleGround* const battle_ground = static_cast<BattleGround*>(chaos_zone);
            if (battle_ground == NULL) {
                return false;
            }

            Player* van_player = PlayerManager::Instance()->FindPlayerByName(van_payer_name);
            if (van_player == NULL) {
                return false;
            }

            if (battle_ground->FindBattleGroundMember(van_player->GetCharGuid()) == NULL) {
                return false;
            }

            RC::eBATTLE_GROUND_RESULT result = RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
            result = battle_ground->OnInitiateVote(player, van_player);
        }break;

    case vote:
        {
            const bool vote_result = static_cast<bool>(cmd_tokens[2].c_str());

            if (player->IsThereBattleGroundZone() == false) {
                return false;
            }

            ChaosZone* chaos_zone = player->GetCurrentChaosZone();
            if (chaos_zone == NULL) {
                return false;
            }

            BattleGround* const battle_ground = static_cast<BattleGround*>(chaos_zone);
            if (battle_ground == NULL) {
                return false;
            }
            battle_ground->OnVote(player, vote_result);
        }break;

    case van:
        {
            const TCHAR* van_payer_name = cmd_tokens[2].c_str();

            if (player->IsThereBattleGroundZone() == false) {
                return false;
            }

            ChaosZone* chaos_zone = player->GetCurrentChaosZone();
            if (chaos_zone == NULL) {
                return false;
            }

            BattleGround* const battle_ground = static_cast<BattleGround*>(chaos_zone);
            if (battle_ground == NULL) {
                return false;
            }

            Player* const van_player = PlayerManager::Instance()->FindPlayerByName(van_payer_name);
            if (van_player == NULL) {
                return false;
            }

            if (battle_ground->FindBattleGroundMember(van_player->GetCharGuid()) == NULL) {
                return false;
            }

            MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD cmd_msg;
            van_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        }break;
    }
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    return true;
}


}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterTestUnit_jaykang(const HandlerNode** node_array,
                                            size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "강재준", "jaykang", &TestUnit4jaykang),
        GMHANDLER_MAPPING(true, "아이템사용", "item_use", &ItemUse),
        GMHANDLER_MAPPING(true, "락해제", "release_lock", &ReleaseLock),
        GMHANDLER_MAPPING(true, "상태", "status", &ApplyStatus),
        GMHANDLER_MAPPING(true, "수리", "RepairAll", &RepairAll),
        GMHANDLER_MAPPING(true, "전장투표", "battle_ground_vote", &BattleGroundVote),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}