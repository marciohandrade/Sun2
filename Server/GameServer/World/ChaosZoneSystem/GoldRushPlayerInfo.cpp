#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRushPlayerInfo.h"
#include <../ProgramCommon/ChaosZoneSystem/GoldRushInfoParser.h>
#include <GameInstanceDungeon.h>

#include <AreaConquestCommonHeader.h>
#include "QuestManager.h"
#include "QuestManager_Concrete.h"

void GoldRushPlayerInfo::Init( GoldRushRecord* const record )
{
    goldrush_record_ = record;
    series_kill_ = 0;
    max_series_kill_ = 0;
    team_key_ = kChaosZoneTeamNone;
    SetGoldPoint(0);

    if (player_->GetChaosZoneType() == eCHAOSZONE_TYPE_GOLDRUSH)
    {
        MAPCODE map_code = player_->GetCurrentChaosZone()->GetInstanceDungeon()->GetMapCode();
        const GoldRushInfoParser* const goldrush_info_parser = GoldRushInfoParser::Instance();
        const GoldRushInfo* goldrush_info = goldrush_info_parser->FindData(map_code);

        if( goldrush_info) {
            goldrush_info_ = goldrush_info;
        }
    }
}

void GoldRushPlayerInfo::IncreasePlayTimes()
{
    if (goldrush_record_)
        goldrush_record_->total_count++;
}

void GoldRushPlayerInfo::IncreaseWinTimes()
{
    if (goldrush_record_)
        goldrush_record_->win_count++;

    //AC
    player_->GetACManager()->IncConditionValue(ACHIEVEMENT_OBJECT_BATTLEGROUND_WIN, 
        goldrush_record_->GetMapCode());

    //Quest

    QUEST_MISSION_CLEAR_MSG quest_msg;
    quest_msg.mission_code = goldrush_record_->GetMapCode();
    player_->GetQuestManager()->Event(QUEST_EVENT_MISSION_CLEAR, &quest_msg);
}

void GoldRushPlayerInfo::IncreaseDrawTimes()
{
    if (goldrush_record_)
        goldrush_record_->draw_count++;
}

void GoldRushPlayerInfo::IncreaseLoseTimes()
{
    if (goldrush_record_)
        goldrush_record_->lose_count++;
}

void GoldRushPlayerInfo::IncreaseDisTimes()
{
    if (goldrush_record_)
        goldrush_record_->dis_count++;
}

void GoldRushPlayerInfo::DecreaseDisTimes()
{
    if (goldrush_record_)
    {
        if (goldrush_record_->dis_count > 0)
            goldrush_record_->dis_count--;
    }
}

void GoldRushPlayerInfo::IncreaseKill( const WORD kill_point /*= 1*/ )
{
    if (goldrush_record_ == NULL)
        return;

    goldrush_record_->kill_count += kill_point;

    ++series_kill_;
    if (max_series_kill_ < series_kill_){
        max_series_kill_ = series_kill_;
    }
    if (goldrush_record_->max_series_kill < max_series_kill_) {
        goldrush_record_->max_series_kill = max_series_kill_;
    }
}

void GoldRushPlayerInfo::IncreaseDeath()
{
    series_kill_ = 0;
    if (goldrush_record_)
        goldrush_record_->death_count++;
}

void GoldRushPlayerInfo::IncreaseAssistTimes()
{
    if (goldrush_record_)
        goldrush_record_->assist_count++;
}

WORD GoldRushPlayerInfo::LoseGoldPointRatio( const BYTE lose_ratio )
{
    WORD cur_point = GetGoldPoint();
    WORD lose_point =  static_cast<WORD>(static_cast<float>(cur_point) * (static_cast<float>(lose_ratio) / 100.0f));

    if (goldrush_record_) {
        if (lose_point > goldrush_record_->gold_point) {
            goldrush_record_->gold_point = 0;
        }
        else {
            goldrush_record_->gold_point -= lose_point;
        }
        _AdjustGoldGrade();
#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
        // 적플레이어 킬에 의한 추가골드포인트 획득값
        GAMELOG->LogGoldRush(ACT_GOLDRUSH_GOLDPOINT_LOSE, player_, lose_point, GetGoldPoint());
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    }

    return lose_point;
}

void GoldRushPlayerInfo::AddGoldPoint( const WORD gold )
{
    if (goldrush_record_)
    {
        goldrush_record_->gold_point += gold;
        _AdjustGoldGrade();
    }

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    // 적플레이어 킬에 의한 추가골드포인트 획득값
    GAMELOG->LogGoldRush(ACT_GOLDRUSH_GOLDPOINT_GET, player_, gold, GetGoldPoint());
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
}

void GoldRushPlayerInfo::SetGoldPoint( const WORD gold )
{
    if (goldrush_record_)
    {
        goldrush_record_->gold_point = gold;
        _AdjustGoldGrade();
    }
}

WORD GoldRushPlayerInfo::GetGoldPoint()
{
    if (goldrush_record_) {
        return goldrush_record_->gold_point;
    }
    return 0;
}

void GoldRushPlayerInfo::_AdjustGoldGrade()
{
    if (goldrush_record_ != NULL && goldrush_info_ != NULL)
    {
        WORD gold_point = GetGoldPoint();

        for(int i=0; i < GoldRushInfo::eGOLDLEVEL_SIZE; ++i)
        {
            if (gold_point <= goldrush_info_->gold_grade[i]) 
            {
                goldrush_record_->gold_grade = static_cast<GOLDGRADE>(i);
                return;
            }
        }
        goldrush_record_->gold_grade = eGOLDGRADE6;
    }
}

const GOLDGRADE GoldRushPlayerInfo::GetGoldGrade()
{
    if (goldrush_record_) {
        return goldrush_record_->gold_grade;
    }
    return eGOLDGRADE0;
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH