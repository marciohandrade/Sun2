#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <Player.h>
#include <World/ChaosZoneSystem/BattleGroundPlayerInfo.h>

#ifdef _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
#include <AreaConquestCommonHeader.h>
#endif

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
#include "QuestManager.h"
#include "QuestManager_Concrete.h"
#endif

void BattleGroundPlayerInfo::Init(BattleGroundRecord* const record)
{
    battle_ground_record_ = record;
    WORD series_kill_ = 0;
    WORD max_series_kill_ =0;
}

void BattleGroundPlayerInfo::Release()
{
}
void BattleGroundPlayerInfo::IncreasePlayTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->total_count++;
}

void BattleGroundPlayerInfo::IncreaseWinTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->win_count++;

#ifdef _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
    player_->GetACManager()->IncConditionValue(ACHIEVEMENT_OBJECT_BATTLEGROUND_WIN, 
                                               battle_ground_record_->GetMapCode());
#endif //_NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    QUEST_MISSION_CLEAR_MSG quest_msg;
    quest_msg.mission_code = battle_ground_record_->GetMapCode();
    player_->GetQuestManager()->Event(QUEST_EVENT_MISSION_CLEAR, &quest_msg);
#endif //_NA_006607_20130402_ADD_QUESTTYPE
}

void BattleGroundPlayerInfo::IncreaseDrawTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->draw_count++;
}

void BattleGroundPlayerInfo::IncreaseLoseTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->lose_count++;
}

void BattleGroundPlayerInfo::IncreaseDisTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->dis_count++;
}

void BattleGroundPlayerInfo::DecreaseDisTimes()
{
    if(battle_ground_record_)
    {
        if (battle_ground_record_->dis_count > 0)
            battle_ground_record_->dis_count--;
    }
}

void BattleGroundPlayerInfo::IncreaseKill(const WORD kill_point)
{
    if(battle_ground_record_ == NULL)
    {
        return;
    }

    battle_ground_record_->kill_count += kill_point;

    ++series_kill_;
    if (max_series_kill_ < series_kill_)
    {
        max_series_kill_ = series_kill_;
    }

    if (battle_ground_record_->max_series_kill < max_series_kill_)
    {
        battle_ground_record_->max_series_kill = max_series_kill_; 
    }
}

void BattleGroundPlayerInfo::IncreaseDeath()
{
    series_kill_ = 0;
    if(battle_ground_record_)
        battle_ground_record_->death_count++;
}

void BattleGroundPlayerInfo::IncreaseStrongpointAttack()
{
    if(battle_ground_record_)
        battle_ground_record_->strongpoint_attack++;
    
#ifdef _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
    player_->GetACManager()->IncConditionValue(ACHIEVEMENT_OBJECT_BATTLEGROUND_STRONGPOINT_ATTACK, 
                                               battle_ground_record_->GetMapCode());
#endif
}

void BattleGroundPlayerInfo::IncreaseStrongpointDefense()
{
    if(battle_ground_record_)
        battle_ground_record_->strongpoint_defense++;

#ifdef _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
    player_->GetACManager()->IncConditionValue(ACHIEVEMENT_OBJECT_BATTLEGROUND_STRONGPOINT_DEFENSE, 
                                               battle_ground_record_->GetMapCode());
#endif
}

void BattleGroundPlayerInfo::ClearScore()
{
    if(battle_ground_record_)
        ZeroMemory(battle_ground_record_, sizeof(BattleGroundRecord));
}

#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
void BattleGroundPlayerInfo::IncreaseAssistTimes()
{
    if(battle_ground_record_)
        battle_ground_record_->assist_count++;
}
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
WORD BattleGroundPlayerInfo::GetKillTimes()
{
    return battle_ground_record_->kill_count;
}

WORD BattleGroundPlayerInfo::GetAssistTimes()
{
    return battle_ground_record_->assist_count;
}

WORD BattleGroundPlayerInfo::GetWintimes()
{
    return battle_ground_record_->win_count;
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR