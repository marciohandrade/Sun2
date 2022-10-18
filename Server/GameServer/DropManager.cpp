#include "stdafx.h"
#include "./DropManager.h"

//==================================================================================================

#include <SCItemSlot.h>
#include <NumericValues.h>
//
#include <ServerOptionParserEx.h>
#include "StatisticsManager.h"

#include "FunctionalRestrictManager.h"

#include "GameZone.h"
#include "GameField.h"
#include "GameHuntingRoom.h"

#include "ItemManager.h"
#include "StatusManager.h"
//
#include "QuestInfoManagerEx.h"
#include "QuestManager_Concrete.h"
#include "GameParty/NewGamePartyManager.h"
#include <GameInstanceDungeon.h>
#include <GameMissionRoom.h>


#include "DropRatioInfoList.h"

//==================================================================================================

DropManager g_DropManager;

//==================================================================================================

namespace ns_functor {
;

inline static BOOL _CheckValidPlayer(Player* IN player, GameZone* IN drop_zone)
{
    GameZone* pPlayerZone = player->GetGameZonePtr();
    return (pPlayerZone && (pPlayerZone->GetKey() == drop_zone->GetKey()) && player->IsAlive());
}

inline static BOOL _CheckFindQuest(Player* IN player, QCODE questCode)
{
    return player && player->GetQuestManager()->FindQuest(questCode);
}


class QuestItemDropControl
{
    typedef DWORD QUEST_IDX_BITSET;
    typedef QCODE QUEST_CODE_TYPE;
    typedef DWORD QUEST_DROPIDX_TYPE;
    static const DWORD MAX_QUEST_ITEM_DROP_NUM = BASE_NPCINFO::MAX_QUEST_ITEM_DROP_NUM;
    static const QUEST_IDX_BITSET QUEST_IDX_BITSET_EMPTY = 0;

public:
    QuestItemDropControl(const BASE_NPCINFO* pInfo)
        : m_pNpcInfo(pInfo)
    {
#ifdef _NA_004509_20120314_QUESTDROPINDEX10
        BOOST_STATIC_ASSERT(MAX_QUEST_ITEM_DROP_NUM==10);
#else
        BOOST_STATIC_ASSERT(MAX_QUEST_ITEM_DROP_NUM==5);
#endif //_NA_004509_20120314_QUESTDROPINDEX10
        m_QuestCodes = m_pNpcInfo->m_QuestCode;
        m_QuestDrops = m_pNpcInfo->m_dwQuestDropIndex;
        ZeroMemory(m_Quests, sizeof(m_Quests));
    }

    BOOL CheckAndAlignQuest(const DWORD index)
    {
        if (MAX_QUEST_ITEM_DROP_NUM > index)
        {
            if (m_QuestCodes[index] && m_QuestDrops[index])
            {
                m_CheckIndex = index;
                return true;
            }
        }
        m_CheckIndex = MAX_QUEST_ITEM_DROP_NUM;
        return false;
    }

    QCODE GetCurCheckQuestCode() {
        return MAX_QUEST_ITEM_DROP_NUM > m_CheckIndex ? m_QuestCodes[m_CheckIndex] : 0;
    }
    DWORD GetCurCheckQuestDrop() {
        return MAX_QUEST_ITEM_DROP_NUM > m_CheckIndex ? m_QuestDrops[m_CheckIndex] : 0;
    }

private:
    const BASE_NPCINFO* m_pNpcInfo;
    const QUEST_CODE_TYPE* m_QuestCodes;
    const QUEST_DROPIDX_TYPE* m_QuestDrops;
    DWORD m_CheckIndex;
    Quest* m_Quests[MAX_QUEST_ITEM_DROP_NUM];
};

class SearchPartyHasQuest : public std::unary_function<Character*, BOOL>
{
public:
    SearchPartyHasQuest(QuestItemDropControl& IN rDropControl, Player* IN pMember)
        : m_rDropControl(rDropControl), m_pMember(pMember)
    {
        m_bSatisfy = false;
    }

    void Reuse() { m_bSatisfy = false; }
    BOOL IsSatisfy() const { return m_bSatisfy; }

    BOOL operator () (Player* player) const
    {
        if (m_bSatisfy) {
            return true;
        };
        if (_CheckValidPlayer(player, m_pMember->GetGameZonePtr()) &&
            _CheckFindQuest(player, m_rDropControl.GetCurCheckQuestCode()))
        {
            m_bSatisfy = true;
            return true;
        }
        return false;
    }
private:
    QuestItemDropControl& m_rDropControl;
    mutable BOOL m_bSatisfy;
    Player* m_pMember;
};

//==================================================================================================

#if defined(_YJ_TOOLTIP_ON_TEXT_001277) && !defined(__NA_0_20081124_PARTY_REFACTORING_GAMESERVER)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

template<typename _PackType>
class AcquireQuestItem : public std::unary_function<Character*, void>
{
public:
    AcquireQuestItem(QCODE aQuestCode, ZONEKEY aZoneKey, SCItemSlot* pQuestItem)
        : m_QuestCode(aQuestCode)  // 퀘스트 코드
        , m_ZoneKey(aZoneKey)      // 소유권자의 존키
        , m_pQuestItem(pQuestItem) // 생성된 퀘스트아이템 슬롯
    {       
    }

    void operator() (Character* character) const
    {
        Player* player = static_cast<Player*>(character);
        if (player == NULL) {
            return;
        };
        GameZone* game_zone = character->GetGameZonePtr();
        if (game_zone == NULL) {
            return;
        };
        if (!(player->IsDead()) && // 생존
            (game_zone->GetKey() == m_ZoneKey) &&   // 소유권자와 같은 존
            (player->GetQuestManager()->FindQuest(m_QuestCode)))   // 퀘스트 소유
        {
        #ifdef _YJ_TOOLTIP_ON_TEXT_001277
            WORD itemCode = m_pQuestItem->GetItemCode();
            POSTYPE itemNum = m_pQuestItem->GetNum();
            m_Packet.m_QuestCode = m_QuestCode;

//!~ #3669 퀘스트 아이템 수집 완료시, 더이상 퀘스트 아이템을 입수하지 않음 - arycoat 2011.11.15
            ItemManager* item_manager = player->GetItemManager();
            QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(m_QuestCode);
            if(!quest_info) {
                SUNLOG(eCRITICAL_LOG, _T("[%s:%d] cannot find quest info : %d"), 
                    __FUNCTION__, __LINE__, m_QuestCode);
                return;
            }

            QUEST_CONDITION_ITEM_INFO* quest_condition_item_info = \
                static_cast<QUEST_CONDITION_ITEM_INFO*>(
                    quest_info->FindCompleteCondition(eQUEST_CONDITION_ITEM));
            if(!quest_condition_item_info) {
                SUNLOG(eCRITICAL_LOG, _T("[%s:%d] cannot find quest complete condition item info : %d"), 
                    __FUNCTION__, __LINE__, m_QuestCode);
                return;
            }
            
            for(int i = 0; i < quest_condition_item_info->GetItemArrayCount(); ++i)
            {
                if(quest_condition_item_info->GetItemCode(i) == itemCode)
                {
                    // 충분한 개수의 아이템이 있는지 체크
                    int item_count = quest_condition_item_info->GetItemNum(i);
                    int ExistCount = item_manager->ExistItem(SI_INVENTORY, itemCode, item_count);
                    if (ExistCount >= item_count)
                        return;
                }
            }
//~!

            RC::eITEM_RESULT ret = player->GetItemManager()->Obtain( \
                (*m_pQuestItem), itemNum, &m_Packet.m_TotalInfo);
            if (ret == RC::RC_ITEM_SUCCESS)
                player->SendPacket(&m_Packet, m_Packet.GetSize());
        #else
            static SCItemSlot ItemSlot;
            ItemSlot = *m_pQuestItem;
            if (RC::RC_ITEM_SUCCESS == player->GetItemManager()->Pick(1, ItemSlot, &m_Packet.m_ItemInfo))
            {
                player->SendPacket(&m_Packet, m_Packet.GetSize());
            }
        #endif
        }
    }

private:
    QCODE m_QuestCode;
    ZONEKEY m_ZoneKey;
    SCItemSlot* m_pQuestItem;
    mutable _PackType m_Packet;
};

}; //end of namespace 'ns_functor'

//==================================================================================================
//
//{__NA000000_20090320_DELAY_ITEM_DROP_TIME
struct DropManager::DelayDropInfo
{
    DelayDropInfo() 
    {
        ZeroMemory(this, static_cast<size_t>(\
            reinterpret_cast<char*>(&drop_timer_) - reinterpret_cast<char*>(this)));
        monster_grade_ = eNPC_GENERAL;
    }
    //
    GameField* game_field_;
    DWORD sector_index_;
    WzVector drop_center_pos_;
    //
    Player* owner_;
    DWORD monster_key_;
    const BASE_NPCINFO* monster_info_;
    eNPC_GRADE monster_grade_;
    LEVELTYPE monster_display_level_;
    //
    util::Timer drop_timer_;
};

struct DropManager::DelayedFieldObject
{
    GameField* game_field_;
    util::SimpleTimer delayed_drop_timer_;
    DWORD monster_key_;
    PLAYERKEY player_key_;
    bool is_item_object;
    WzVector drop_center_pos_;
    //
    MONEY scheduled_drop_money_;
    SCItemSlot scheduled_drop_item_;
};

//==================================================================================================
//
void DropManager::Init(DWORD pool_size)
{
    __UNUSED(pool_size);
}

void DropManager::OnFieldRelease(ZONEKEY zone_key)
{
    DELAY_DROPINFO_LIST::_Pairii ranges = delay_drop_info_list_.equal_range(zone_key);
    if (ranges.first == ranges.second) { // changes comparision
        return;
    };

    DELAY_DROPINFO_LIST::const_iterator it = ranges.first;
    for (; it != ranges.second; ++it)
    {
        DelayDropInfo* delay_drop_info = it->second;
        if (FlowControl::FCAssert(delay_drop_info != NULL) == false) {
            continue;
        };
        TAllocDelete(DelayDropInfo, delay_drop_info);
    };

    delay_drop_info_list_.erase(ranges.first, ranges.second);
    //
}

void DropManager::DelayedDrop_(GameField* game_field, DWORD sector_index, const WzVector* origin_pos,
    Player* owner, DWORD monster_key, const BASE_NPCINFO* monster_info,
    eNPC_GRADE npc_grade, LEVELTYPE monster_display_level)
{
    static const DWORD& kDelayInterval = \
        ServerOptionParserEx::Instance()->GetServerOption().m_DelayDropTime;
    if (game_field == NULL) {
        return;
    };

    DelayDropInfo* const delay_drop_info = TAllocNew(DelayDropInfo);
    delay_drop_info->game_field_ = game_field;
    delay_drop_info->sector_index_ = sector_index;
    delay_drop_info->owner_ = owner;
    delay_drop_info->monster_key_ = monster_key;
    delay_drop_info->monster_info_ = monster_info;
    delay_drop_info->monster_grade_ = npc_grade;
    delay_drop_info->monster_display_level_ = monster_display_level;

    delay_drop_info->drop_center_pos_ = *origin_pos;
    delay_drop_info->drop_timer_.SetTimer(kDelayInterval);

    ZONEKEY zone_key = game_field->GetGameZone()->GetKey();
    delay_drop_info_list_.insert(DELAY_DROPINFO_LIST::value_type(zone_key, delay_drop_info));
}

void DropManager::UpdateDelayedItemDrop()
{
    DELAY_DROPINFO_LIST::iterator it = delay_drop_info_list_.begin();

    while (it != delay_drop_info_list_.end())
    {
        DelayDropInfo* delay_drop_info = it->second;
        if (FlowControl::FCAssert(delay_drop_info != NULL) == false) {
            it = delay_drop_info_list_.erase(it);
            continue;
        };

        const bool dropped = (delay_drop_info->drop_timer_.IsExpiredManual(false) != false);
        if (dropped)
        {
            this->InstantDrop_(delay_drop_info->game_field_, delay_drop_info->sector_index_,
                &delay_drop_info->drop_center_pos_, delay_drop_info->owner_,
                delay_drop_info->monster_key_, delay_drop_info->monster_info_,
                delay_drop_info->monster_grade_, delay_drop_info->monster_display_level_);
            //
            TAllocDelete(DelayDropInfo, delay_drop_info);
            it = delay_drop_info_list_.erase(it);
            continue;
        };
        //
        ++it;
        //
    }; //end 'while'
}
//}__NA000000_20090320_DELAY_ITEM_DROP_TIME

//==================================================================================================
//==================================================================================================
//==================================================================================================

DropManager::DropManager()
{

}

DropManager::~DropManager()
{
}

void DropManager::Release()
{
    DELAYEDFOBJ_LIST::const_iterator it = delayed_field_object_list_.begin();
    const DELAYEDFOBJ_LIST::const_iterator end = delayed_field_object_list_.end();
    // CHANGES: f110715.2L, fixed the infinite loop
    for (; it != end; ++it)
    {
        DelayedFieldObject* delayed_field_object = *it;
        TAllocDelete(DelayedFieldObject, delayed_field_object);
    }
    delayed_field_object_list_.clear();

    drop_core_.Unload();
}

void DropManager::InstantDrop_(GameField* game_field, DWORD sector_index,
    const WzVector* origin_pos, Player* owner,
    DWORD monster_key, const BASE_NPCINFO* monster_info,
    eNPC_GRADE npc_grade, LEVELTYPE monster_display_level)
{
    __UNUSED(sector_index);
    if (game_field == NULL) {
        return;
    };
    GameZone* const game_zone = game_field->GetGameZone();
    if (game_zone == NULL) {
        return;
    };

    if (g_FunctionalRestrictMgr.IsRestrictFunction(eFUNCTIONALRESTRICT_USE_MONSTERDROP))
    {
        static bool reported = false;
        if (reported == false)
        {
            SUNLOG(eCRITICAL_LOG, "몬스터 드랍 서비스가 중지중입니다");
            reported = true;
        };
        return;
    };
    if (FlowControl::FCAssert(npc_grade < eNPC_GRADE_MAX) == false) {
        return;
    };

    // NOTE: f110414.4L, apply weight value about heim based on number of party members.
    float heim_weight_based_on_party_members = 1.0f;
    DWORD owner_key = 0;
    //----------------------------------------------------------------------------------------------
    if (owner)
    {
        // 딜레이 드랍에 따라 간단한 유효성 체크 한다.
        if (0 == owner->GetObjectKey())
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|유효하지 않은 플레이어키|"), __FUNCTION__);
            return;
        };
        GameZone* const game_zone_of_owner = owner->GetGameZonePtr();
        if (game_zone_of_owner == NULL)
        {
            // 몬스터 잡은후 접속 종료시
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|유효하지 않은 Owner|"), __FUNCTION__);
            return;
        };
        if (game_zone_of_owner != game_zone)
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Owner의 존키와 필드의 존키가 일치하지 않는다"), __FUNCTION__);
            return;
        };
        // 파티 상태이면 아이템 분배 시스템을 적용 시키고 있는 시점이다.
        const WORD party_key = owner->GetPartyState().GetPartyKey();
        if (party_key)
        {
            owner_key = owner->GetObjectKey();
        #ifdef _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT // _CHINA
            int party_members = NewGamePartyManager::Instance()->FindNearPartyUserCount(owner);
            heim_weight_based_on_party_members = ns_formula::GetRoomHeimPartyBonusFactor(party_members);
        #else
            // CHANGES: f110414.4L, apply weight value about heim based on number of party members.
            const BaseParty* party = static_cast<const BaseParty*>(\
                NewGamePartyManager::Instance()->FindParty(party_key));

            if (party != NULL)
            {
                int number_of_party_members = party->GetMemberNum();

#ifdef _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02
                bool zone_type_top_of_trial = false;
                if (game_zone->GetZoneType() == eZONETYPE_INSTANCE) 
                {
                    GameInstanceDungeon* instance_dungeon = \
                        static_cast<GameInstanceDungeon*>(game_zone);
                    if (instance_dungeon && \
                        instance_dungeon->GetTypeOfIndun() == INDUN_KIND_TOP_OF_TRIAL) 
                    {
                        zone_type_top_of_trial = true;
                    }
                }

/* // 빛의 시련 하임 드랍 방식을 미션 드랍 방식을 따르도록 함 - arycoat 2014.08.11
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                if (game_zone->GetZoneType() == eZONETYPE_MISSION)
                {
                    GameMissionRoom* mission_room = static_cast<GameMissionRoom*>(game_zone);
                    if (mission_room) {
                        PartyZoneType party_zone_type = mission_room->GetPartyZoneType();
                        if (party_zone_type == PartyOfTopOfTrialLight_normal ||
                            party_zone_type == PartyOfTopOfTrialLight_cash) {
                            zone_type_top_of_trial = true;
                        }
                    }
                }
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
*/
                
                heim_weight_based_on_party_members = \
                    (zone_type_top_of_trial) ? \
                        ns_formula::GetRoomHeimPartyBonusFactorType2(number_of_party_members) : \
                        ns_formula::GetRoomHeimPartyBonusFactor(number_of_party_members);
#else
                heim_weight_based_on_party_members =
                    ns_formula::GetRoomHeimPartyBonusFactor(number_of_party_members);
#endif //_NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02
            };
        #endif //_NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT
        }
        else
        {
            const SERVER_COMMON_OPTION& server_option = \
                ServerOptionParserEx::Instance()->GetServerOption();
            if (server_option.m_byApplyItemOwnerShipByLevel != false)
            {
                //레벨차가 10이하만 소유권 존재.
                int LVInterval = (owner->GetLevel() - monster_display_level);
                if (LVInterval < DONT_DROP_LEVEL_INTERVAL) {
                    owner_key = owner->GetObjectKey();
                };
            }
            else
            {
                owner_key = owner->GetObjectKey();
            };
        }
    }
    //----------------------------------------------------------------------------------------------
#if SUN_CODE_BACKUP
    FLOAT PlusBonusPercent = 1.0f;
    if (game_zone->GetZoneType() == eZONETYPE_HUNTING)
    {
        // 헌팅 보너스 가중치
        // ???? -> 경쟁헌팅으로 바꿔야 하지 않나?
        GameHuntingRoom* pHuntingRoom = static_cast<GameHuntingRoom*>(game_zone);
        if (pHuntingRoom->GetAdditionalInfo()->m_Bonus == eHUNTING_BONUS_TYPE_ITEM) {
            PlusBonusPercent = 1.1f;
        };
    }
#endif
    // CHANGES: f110715.2L, removed the unused section
    // CHANGES: f110414.4L, apply HEIM weight value about heim based on number of party members.
    // subtract default percentage to prevent the point that is percentage of a percentage.
    float added_bonus_percentage = heim_weight_based_on_party_members;
    //----------------------------------------------------------------------------------------------
    // 퀘스트 아이템 드랍
    DropQuest(game_field, monster_info, owner,
              npc_grade, added_bonus_percentage,
              game_zone, origin_pos, monster_key, owner_key);
    //----------------------------------------------------------------------------------------------
    static SCItemSlot dropping_item_slot_static;
    //
    typedef const DWORD DropIndexArray[_countof(monster_info->m_dwFieldDropIndex)];
    const DropIndexArray* __drop_index_array
        = (monster_info->m_bRegion == 0) ?                   &monster_info->m_dwDropIndex // 공통 Drop
        : (g_pGameServer->GetServerType() == FIELD_SERVER) ? &monster_info->m_dwFieldDropIndex // 필드 Drop
        :                                                    &monster_info->m_dwBattleDropIndex; // 배틀존 Drop
    //
    const DropIndexArray& drop_index_array = *__drop_index_array;
    bool gm_drop_process = false;
    ;{
        const StatusField& status_field = owner->GetStatusManager()->Status;
        if (status_field.FIELD & status_field.ENABLE_GM_DROP_LIST_ALL) {
            gm_drop_process = true;
        };
    };
    // 기본 아이템 드랍
    for (int i = 0; i < _countof(drop_index_array); ++i)
    {
        const DWORD drop_index = drop_index_array[i];
        if (0 == drop_index) {
            continue;
        };
        BOOST_STATIC_ASSERT(\
            _countof(drop_index_array) == _countof(monster_info->m_bOwnerShipForDropIndex));
        //------------------------------------------------------------------------------------------
        if (gm_drop_process)
        {
            DWORD temp_player_key = owner_key;
            if (monster_info->m_bOwnerShipForDropIndex[i] == false) {
                temp_player_key = 0;
            };
            this->GMDrop(game_field, origin_pos, owner,
                         monster_key, monster_info, game_zone->GetZoneType(), drop_index,
                         npc_grade, added_bonus_percentage, temp_player_key);
            continue;
        };
        //------------------------------------------------------------------------------------------
        // NOTE: default drop processing
        int number_of_drop_items = 0;
        MONEY dropping_money = 0;
        DWORD decided_seed_value = 0;
        eDROP_TYPE_ dropping_type;
        SCItemSlot& dropping_item_slot = dropping_item_slot_static;

        bool does_drop_exist = (drop_core_.Drop(game_zone->GetZoneType(),
                drop_index, npc_grade, added_bonus_percentage,
                dropping_type, dropping_money, dropping_item_slot,
                decided_seed_value, &number_of_drop_items) != false);
        if (does_drop_exist == false) {
            continue;
        };

        DWORD temp_player_key = owner_key;
        // 소유권이 없다면
        if (monster_info->m_bOwnerShipForDropIndex[i] == false) {
            temp_player_key = 0;
        };
        //------------------------------------------------------------------------------------------
        ;;;; if (dropping_type == DROP_TYPE_ITEM)
        {
            if (temp_player_key != 0) {
                temp_player_key = GetSecondItemOwnershipUserKey(owner, dropping_item_slot);
            };
            // NOTE: f110401.3L
            for (int counter = 0; counter < number_of_drop_items; ++counter)
            {
                this->DropItem(game_field, origin_pos,
                    monster_key, temp_player_key, monster_info, dropping_item_slot);
            };
        }
        else if (dropping_type == DROP_TYPE_MONEY)
        {
            ASSERT(decided_seed_value != 0 && number_of_drop_items == 1);
            const int log_type = (decided_seed_value > 1) ?
                MONEY_MONSTER_JACKPOT_DROP : MONEY_MONSTER_DROP;
            // 몬스터 돈 드랍 로그
            GAMELOG->LogMonsterMoneyDrop(log_type,
                dropping_money, monster_info->m_MonsterCode,
                npc_grade, game_zone->GetMapCode(), *origin_pos, game_zone->GetKey());
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
            GameBattleResult* battle_result = game_zone->GetGameBattleResult();
            if (battle_result != NULL)
            {
                battle_result->AddToTalDropHeim(dropping_money);
            }
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

            if (temp_player_key != 0) {
                temp_player_key = GetSecondHeimOwnershipUserKey(owner);
            };
            this->DropMoney((log_type == MONEY_MONSTER_JACKPOT_DROP),
                game_field, origin_pos, monster_key, temp_player_key, dropping_money);
        };
        //------------------------------------------------------------------------------------------
    }; //end 'for'
}

DWORD DropManager::GetSecondItemOwnershipUserKey(Player* pre_owner, SCItemSlot& dropped_item)
{
    // NOTE: f110424.1L, logic improvements
    DWORD default_owner_key = pre_owner->GetObjectKey();

    GameField* const game_field = pre_owner->GetField();
    if (game_field == NULL) {
        return 0;
    };
    const BASE_ITEMINFO* const item_info = dropped_item.GetItemInfo();
    // REMOVED: rules = 배틀서버는 아이템별로 아이템분배 적용여부를 확인한다.
    //if (g_pGameServer->GetServerType() == BATTLE_SERVER && \
    //    item_info->IsApplyItemDistribution() == false) {
    //    return default_owner_key;
    //};

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    if(item_info)
    {
        // 주사위 방식 획득 아이템도 분배규칙을 적용하지 않음.
        if(BYTE dicecast_type = item_info->dice_cast_type > 0)
        {
            bool all_party_member_obtain_type = false;
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            // 파티원 전원 획득 존에서는 주사위 적용안함. arycoat.2012.02.08
            sPOLICY_ZONE& Policyment = game_field->GetGameZone()->Policyment;
            if(Policyment.FIELD & Policyment.ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER) {
                all_party_member_obtain_type = true;
            }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

            eITEM_DISTRIBUTION_TYPE type = \
                static_cast<eITEM_DISTRIBUTION_TYPE>( \
                    NewGamePartyManager::Instance()->GetDistributionType( \
                        pre_owner->GetPartyState().GetPartyKey()));

            if( all_party_member_obtain_type == false &&
               (type == eITEM_DISTRIBUTION_DICE_AND_DEFAULT || \
                type == eITEM_DISTRIBUTION_DICE_AND_FIFO))
            {
                dropped_item.SetDiceCastType(dicecast_type);

                return default_owner_key;
            }
        }
    }
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    if(item_info)
    {
        // AC아이템은 분배규칙을 적용하지 않음. arycoat - 2011.07.20
        const eITEMTYPE type = static_cast<eITEMTYPE>(item_info->m_wType);
        if (type == eITEMTYPE_AC)
        {
            return default_owner_key;
        }
    }

    DWORD next_owner_key = default_owner_key;
    if (WORD party_key = pre_owner->GetPartyState().GetPartyKey())
    {
        const eCHAR_TYPE char_type = item_info->GetFirstAvailableClass();
        if (NewGamePartyManager* party_manager = NewGamePartyManager::Instance()) {
            next_owner_key = party_manager->GetDistributionMemberKey(party_key,
                default_owner_key, char_type);

            if(next_owner_key != 0) {
                // 아이템 소유권자가 죽었거나 다른맵에 있을경우 자유획득 - arycoat 2011.07.20
                Player* player = PlayerManager::Instance()->FindPlayerByObjKey(next_owner_key);
                if(player) {
                    if(player->IsDead() || player->GetField() != game_field) {
                        next_owner_key = 0;
                    }
                }
                else {
                    next_owner_key = 0;
                }
            }
        };
    }
#else
    DWORD next_owner_key = default_owner_key;
    if (WORD party_key = pre_owner->GetPartyState().GetPartyKey())
    {
        const eCHAR_TYPE char_type = item_info->GetFirstAvailableClass();
        if (NewGamePartyManager* party_manager = NewGamePartyManager::Instance()) {
            next_owner_key = party_manager->GetDistributionMemberKey(party_key,
                default_owner_key, char_type);
            // no-check member_key integrity
        };
    }
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE

    return next_owner_key;
}

DWORD DropManager::GetSecondHeimOwnershipUserKey(Player* pre_owner)
{
    GameField *game_field = pre_owner->GetField();
    if (game_field == NULL) {
        return 0;
    };
    const DWORD default_owner_key = pre_owner->GetObjectKey();

#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    // 하임은 분배규칙을 적용하지 않음. arycoat - 2011.07.20
#else
    const WORD party_key = pre_owner->GetPartyState().GetPartyKey();
    if (party_key == 0) {
        return default_owner_key;
    };
    //
    DWORD member_key = NewGamePartyManager::Instance()->GetDistributionMemberKey(
        party_key, default_owner_key, eCHAR_TYPE_MAX);
    if (Player* member = PlayerManager::Instance()->FindPlayerByObjKey(member_key)) {
        return member_key;
    }
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE

    return default_owner_key;
}


void DropManager::DropQuest(GameField* game_field, const BASE_NPCINFO* monster_info,
    Player* owner,
    eNPC_GRADE npc_grade, float added_bonus_percentage, GameZone* game_zone,
    const WzVector* creating_pos, DWORD monster_key, DWORD unused__owner_object_key)
{
    __UNUSED((game_field, creating_pos, monster_key, unused__owner_object_key));
    // assert(owner->GetPlayerKey() == dwPlayerKey)
    GameZone* game_zone_of_owner = owner->GetGameZonePtr();
    if (game_zone_of_owner == NULL) {
        return;
    };
    ns_functor::QuestItemDropControl questControl(monster_info);
    ns_functor::SearchPartyHasQuest searchControl(questControl, owner);

    WORD PartyKey = owner->GetPartyState().GetPartyKey();

    MONEY DroppedMoney  = 0;
    DWORD dwDecidedSeed = 0;
    static SCItemSlot QuestItemSlot;
    eDROP_TYPE_ DroppedType;
    int number_of_drop_items = 0;

    for (DWORD index = 0 ; index < BASE_NPCINFO::MAX_QUEST_ITEM_DROP_NUM; ++index)
    {
        if (false == questControl.CheckAndAlignQuest(index)) {
            continue;
        };
        if (!NewGamePartyManager::Instance()->For_eachPlayers(PartyKey, searchControl)) {
            searchControl(owner);
        };
        if (searchControl.IsSatisfy() && 
            drop_core_.Drop(game_zone->GetZoneType(), questControl.GetCurCheckQuestDrop(),
                npc_grade,
                added_bonus_percentage, DroppedType, DroppedMoney,
                QuestItemSlot, dwDecidedSeed, &number_of_drop_items))
        {
            if (DROP_TYPE_ITEM != DroppedType || number_of_drop_items != 1)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("Drop of QuestItem is WRONG!!! : ")
                       _T("MONSTER_NAME[%s], MONSTER_CODE[%d], QUEST_NUM[%d], ")
                       _T("number of separately paid items = %d"),
                       monster_info->m_pszName, monster_info->m_NCode, index,
                       number_of_drop_items);
                return; 
            }

            // 아이템 획득
        #ifdef _YJ_TOOLTIP_ON_TEXT_001277
            ns_functor::AcquireQuestItem<MSG_CG_QUEST_ITEM_OBTAIN_CMD> Opr(\
                questControl.GetCurCheckQuestCode(),
                game_zone_of_owner->GetKey(), &QuestItemSlot);
            if (!NewGamePartyManager::Instance()->For_eachPlayers(PartyKey, Opr))
            {
                Opr(owner);
            }
        #else
            AcquireQuestItem<MSG_CG_ITEM_PICK_ACK> Opr(questControl.GetCurCheckQuestCode(), game_zone_of_owner->GetKey(), &QuestItemSlot);
            if (!NewGamePartyManager::Instance()->For_eachPlayers(PartyKey, Opr))
            {
                Opr(owner);
            }
        #endif
        }

        searchControl.Reuse();
    }
}

void DropManager::DropMoney(bool jackpot,
    GameField* game_field, const WzVector* origin_pos, DWORD monster_key, DWORD player_key,
    MONEY& dropped_money)
{
    // 드랍 하임율 2배
    if (eSERVER_TESTLIVE == g_pGameServer->GetMode()) {
        dropped_money <<= 1;
    };
    if (dropped_money == 0) {
        SUNLOG(eCRITICAL_LOG, "몬스터 드랍되는 돈이 0이다.\n");
    };
    WzVector droppable_pos;
    if (jackpot == false)
    {
        if (game_field->GetRandomPosInAround(origin_pos, 2, droppable_pos) == false) {
            droppable_pos = *origin_pos;
        };
        bool succeeded_dropped = this->DropMoneyToField(\
            game_field, &droppable_pos, player_key, monster_key, dropped_money);
        assert(succeeded_dropped);__TOUCHED(succeeded_dropped);
        return;
    };
    assert(jackpot != false);
    //< 1만 이상의 금액일 때 잭팟
    // NOTE: delayed dropping jackpot playing
    MONEY remains_money = dropped_money;
    DWORD sliced_money_tokens[4] =
    {
        util::LL_Round(remains_money * 0.03f),
        util::LL_Round(remains_money * 0.04f),
        util::LL_Round(remains_money * 0.05f),
        util::LL_Round(remains_money * 0.06f),
    };
    for (int interval_index = 0; remains_money > 0; ++interval_index, (interval_index &= 0x1F))
    {
        BOOST_STATIC_ASSERT(util::bit_count_<_countof(sliced_money_tokens)>::value == 1);
        MONEY sliced_money = sliced_money_tokens[\
            interval_index & (_countof(sliced_money_tokens) - 1)];
        if (remains_money < sliced_money) {
            sliced_money = remains_money;
        };
        remains_money -= sliced_money;
        //
        if (game_field->GetRandomPosInAround(origin_pos, 3, droppable_pos) == false) {
            droppable_pos = *origin_pos;
        };
        bool succeeded_dropped = DropMoneyToField(\
            game_field, &droppable_pos, player_key, monster_key, sliced_money,
            (100 * interval_index));
        assert(succeeded_dropped);__TOUCHED(succeeded_dropped);
    };

    // 잭팟 통계로 기록(성현창)
    //g_pGameServer->AddJackpot(dropped_money);
    g_StatisticsManager.PlusStatisticType(eSTATISTIC_JACKPOT_COUNT, 1);
    g_StatisticsManager.PlusStatisticType(eSTATISTIC_JACKPOT_MONEY, dropped_money);
}

void DropManager::DropItem(GameField* game_field, const WzVector* creating_pos, 
    DWORD monster_key, DWORD player_key, const BASE_NPCINFO* npc_info,
    SCItemSlot& dropped_item_slot)
{
    // (CHANGES) (WAVERIX) add item rule check
    const nsSlot::ItemRuleInfo& item_rule_info = dropped_item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        return;
    };
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#else  // _NA_000251_20100727_SOCKET_SYSTEM
    // (CHANGES) (WAVERIX) itemslot에서 엘리트 룰이 제거되었다.
    const BASE_ITEMINFO* item_info = dropped_item_slot.GetItemInfo();
    if (item_info->IsElite() == 0 && item_rule_info.IsEnableRank())
    {
        // 랭크 아이템일 경우 랭크 옵션 붙이기
        BYTE MaxRank = dropped_item_slot.GetRank();
        dropped_item_slot.SetRank(RANK_E);
        for (INT RankIndex = RANK_E ; RankIndex < MaxRank ; ++RankIndex)
        {
            if (ItemManager::ItemRankUp(&dropped_item_slot))
                ItemManager::CreateRankItemSocketHole(&dropped_item_slot, RankIndex);
        }
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

    WzVector droppable_pos;
    if (game_field->GetRandomPosInAround(creating_pos, 3, droppable_pos) == false) {
        droppable_pos = *creating_pos;
    };
    BOOL dropped_result = DropItemToField(game_field, &droppable_pos,
        player_key, monster_key, dropped_item_slot);
    if (dropped_result != false)
    {
        GameZone* game_zone = game_field->GetGameZone();
        // 몬스터 아이템 드랍 로그 남기기
        GAMELOG->LogMonsterItemDrop(dropped_item_slot,
            npc_info->m_MonsterCode, npc_info->m_Level,
            game_zone->GetMapCode(), droppable_pos, game_zone->GetKey());
        // 몬스터 아이템 드랍 로그 남기기 끝
    };
}

bool DropManager::DropItemToField(GameField* game_field, const WzVector* creating_pos,
    PLAYERKEY player_key, DWORD from_monster_key,
    SCItemSlot& drop_item_slot, DWORD delaying_interval)
{
    if (game_field == NULL || game_field->IsReady() == false) {
        return false;
    }
    else if (delaying_interval == 0)
    {   // instant drop
        Item* field_item = game_field->CreateItem(creating_pos,
            player_key, from_monster_key, drop_item_slot);
        if (field_item != NULL) {
            return true;
        };
    }
    else if (DelayedFieldObject* scheduled_item = TAllocNew(DelayedFieldObject))
    {   // delayed drop
        scheduled_item->drop_center_pos_  = *creating_pos;
        scheduled_item->game_field_ = game_field;
        scheduled_item->delayed_drop_timer_.SetTimer(delaying_interval);
        scheduled_item->monster_key_ = from_monster_key;
        scheduled_item->player_key_ = player_key;
        scheduled_item->is_item_object = true;
        scheduled_item->scheduled_drop_item_.Copy(drop_item_slot);
        delayed_field_object_list_.push_back(scheduled_item);
        return true;
    }
    return false;
}

bool DropManager::DropMoneyToField(GameField* game_field, const WzVector* creating_pos,
    PLAYERKEY player_key, DWORD from_monster_key, const MONEY& money, DWORD delaying_interval)
{
    if (game_field == NULL || game_field->IsReady() == false) {
        return false;
    }
    else if (delaying_interval == 0)
    {   // instant drop
        Item* field_item = game_field->CreateMoney(creating_pos,
            player_key, from_monster_key, money);
        if (field_item != NULL) {
            return true;
        };
    }
    else if (DelayedFieldObject* scheduled_item = TAllocNew(DelayedFieldObject))
    {   // delayed drop
        scheduled_item->drop_center_pos_  = *creating_pos;
        scheduled_item->game_field_ = game_field;
        scheduled_item->delayed_drop_timer_.SetTimer(delaying_interval);
        scheduled_item->monster_key_ = from_monster_key;
        scheduled_item->player_key_ = player_key;
        scheduled_item->is_item_object = false;
        scheduled_item->scheduled_drop_money_ = money;
        delayed_field_object_list_.push_back(scheduled_item);
        return true;
    }
    return false;
}


void DropManager::Process()
{
    UpdateDelayedItemDrop();

    DELAYEDFOBJ_LIST::iterator it = delayed_field_object_list_.begin();
    const DELAYEDFOBJ_LIST::const_iterator end = delayed_field_object_list_.end();
    while (it != end)
    {
        DelayedFieldObject* scheduled_item = *it;
        if (scheduled_item->delayed_drop_timer_.IsExpired() == false) {
            ++it;
            continue;
        };
        //
        if (scheduled_item->is_item_object)
        {
            scheduled_item->game_field_->CreateItem(\
                &scheduled_item->drop_center_pos_, scheduled_item->player_key_,
                scheduled_item->monster_key_, scheduled_item->scheduled_drop_item_);
        }
        else
        {
            scheduled_item->game_field_->CreateMoney(\
                &scheduled_item->drop_center_pos_, scheduled_item->player_key_,
                scheduled_item->monster_key_, scheduled_item->scheduled_drop_money_);
        };
        TAllocDelete(DelayedFieldObject, scheduled_item);
        it = delayed_field_object_list_.erase(it);
    }
}

BOOL DropManager::Load(eSCRIPT_CODE script_code,
    const TCHAR* file_name, BOOL bBigData, eSEPERATOR_MODE mode,
    const TCHAR* pack_file_name, BOOL bReload)
{
    // NOTE: f110401.1L, added by _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    drop_core_.SetupDropRatioTable(g_pGameServer->GetServerType() != FIELD_SERVER);
    //
    return drop_core_.Load(script_code, file_name, bBigData, mode, pack_file_name, bReload);
}

BOOL DropManager::SolarLoad(eSCRIPT_CODE script_code, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData)
{
    // NOTE: f110401.1L, added by _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    drop_core_.SetupDropRatioTable(g_pGameServer->GetServerType() != FIELD_SERVER);
    //
    const char* file_name = FindFileName(script_code);
    drop_core_.InsertFileName(script_code,  file_name);
    return drop_core_.SolarLoad(script_code, pSTREAM, dwSize, bBigData);
}

//==================================================================================================
//==================================================================================================
