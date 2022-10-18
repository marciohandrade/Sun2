#include "stdafx.h"
#include <gtest/gtest.h>

#include "MapInfoParser.h"
#include "BattleGroundInfoParser.h"
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>

class ChaosZoneTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        TableData::CreateEnvironment();
        MapInfoParser::Instance()->Load( \
            SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt", \
            FALSE, SEPERATOR_WHITESPACE, NULL, FALSE );
        MapInfoParser::Instance()->Load( \
            SCRIPT_WORLD, "data\\World.txt", \
            FALSE, SEPERATOR_WHITESPACE, NULL, FALSE );
        BattleGroundInfoParser::Instance()->Load( \
            SCRIPT_BATTLE_GROUND_INFO, "data\\BattleGroundInfo.txt", \
            FALSE, SEPERATOR_WHITESPACE, NULL, FALSE );
    }

    void Reservation(USERGUID guid, MAPCODE map_code, int equip_item_score, \
                     eCHAR_TYPE class_code, bool is_healer)
    {
        BattleGroundManager* battle_ground_manager = BattleGroundManager::Instance();
        EXPECT_TRUE( battle_ground_manager->InsertReservationMember( \
                     guid, map_code, equip_item_score, class_code, is_healer));

        BattleGroundMemberInfo* const member = battle_ground_manager->FindBattleGroundMember(guid);
        ASSERT_TRUE(member != NULL);

        battle_ground_manager->InsertMemberStandbyGroup(member);
        
        const BattleGroundMemberList* const member_list = \
            battle_ground_manager->FindStandbyGroupMemberList(map_code);
        ASSERT_TRUE(member_list != NULL);
    }
};

TEST_F(ChaosZoneTest, DISABLED_battle_ground_matching_test)
//TEST_F(ChaosZoneTest, battle_ground_matching_test)
{
    //////////////////////////////////////////////////////////////////////////
    //
    BattleGroundMatchingSystem* battle_ground_matching_system = BattleGroundMatchingSystem::Instance();
    MAPCODE map_code = 40050;
    ;{
        Reservation(10001, map_code, 6000, eCHAR_BERSERKER, 1);
        Reservation(10002, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10003, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10004, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10005, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10006, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10007, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10008, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10009, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10010, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10011, map_code, 6000, eCHAR_BERSERKER, 0);
        Reservation(10012, map_code, 6000, eCHAR_BERSERKER, 1);
        
        EXPECT_TRUE(battle_ground_matching_system->CheckMatching(map_code));
    };
    ;{
        MatchingGroup* matching_group = battle_ground_matching_system->FindMatchingGroup(18001);
        ASSERT_TRUE(battle_ground_matching_system->OnTeamMatching(matching_group));
    };
}
