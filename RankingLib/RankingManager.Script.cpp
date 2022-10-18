#include "stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "./RankingManager.h"

//==================================================================================================

// IParser interface
BOOL RankingManager::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    __UNUSED(reloaded);
    switch (script_code)
    {
    case SCRIPT_MISSIONRANKINGREWARD:
        return _LoadRewardInfo();
    case SCRIPT_MISSIONRANKINGLVAREA:
        return _LoadLVAreaInfo();
    }

    return false;
}

void RankingManager::UnLoad()
{
    FOREACH_CONTAINER(const MISSION_LEVEL_AREA_HASH::value_type& node, mission_level_area_table_,
                      MISSION_LEVEL_AREA_HASH)
    {
        RankingManager::LEVEL_AREA_BASE* level_area =
            const_cast<RankingManager::LEVEL_AREA_BASE*>(node.second);
        delete level_area;
    };
    mission_reward_table_.clear();
    mission_level_area_table_.clear();
    //
    if (mission_ranking_reward_array_) {
        ::free(mission_ranking_reward_array_);
        mission_ranking_reward_array_ = NULL;
    };
}

bool RankingManager::_LoadLVAreaInfo()
{
    const int max_rows = GetRowSize();

    for (int row = 0; row < max_rows; row++)
    {
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
        RANKINGID ranking_id = GetDataBYTE("Id", row);
        RANKINGNO mission_no = GetDataWORD("No", row);
        LEVEL_AREA_BASE* level_area = const_cast<LEVEL_AREA_BASE*>(FindLVArea_ID(ranking_id));

        if (level_area != NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg='Id(%d)' already exists"), ranking_id); 
            FASSERT(!"'No' already exists");
            return false;
        };
        level_area = new LEVEL_AREA_BASE;
        mission_level_area_table_[ranking_id] = level_area;
        level_area->m_Id = ranking_id;
        //
        level_area->m_MissionNo = mission_no;
        level_area->m_MinLV = GetDataWORD("MinLV", row);
        level_area->m_MaxLV = GetDataWORD("MaxLV", row);
        level_area->m_MaxMonsterKill = GetDataDWORD("MaxMonsterKill", row);
        level_area->m_RankingType = GetDataBYTE("RankingType", row);
        level_area->m_PointRankingIndex = GetDataWORD("PointRankingIndex", row);
        level_area->m_GroupString = GetDataWORD("GroupString", row);
        level_area->m_GroupOrder  = GetDataBYTE("GroupOrder", row);
        level_area->m_MissionString = GetDataWORD("MissionString", row);
        level_area->m_MissionOrder = GetDataBYTE("MissionOrder", row);
    }

    return true;
#else
        RANKINGNO mission_no = GetDataWORD("No", row);
        LEVEL_AREA_BASE* level_area = const_cast<LEVEL_AREA_BASE*>(FindLVArea(mission_no));
        

        if (level_area != NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg='No(%d)' already exists"), mission_no);             
            FASSERT(!"'No' already exists");
            return false;            
        };
        level_area = new LEVEL_AREA_BASE;
        mission_level_area_table_[mission_no] = level_area;
        //
        level_area->m_MissionNo = mission_no;
        level_area->m_MinLV = GetDataWORD("MinLV", row);
        level_area->m_MaxLV = GetDataWORD("MaxLV", row);
        level_area->m_MaxMonsterKill = GetDataDWORD("MaxMonsterKill", row);
        level_area->m_RankingType = GetDataBYTE("RankingType", row);
        level_area->m_PointRankingIndex = GetDataWORD("PointRankingIndex", row);
    }

    return true;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
}

bool RankingManager::_LoadRewardInfo()
{
    const int max_rows = GetRowSize();
    if (FlowControl::FCAssert(mission_ranking_reward_array_ == NULL &&
                              mission_reward_table_.empty()) == false)
    {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*mission_ranking_reward_array_) * max_rows;
        mission_ranking_reward_array_ = reinterpret_cast<REWARD_BASE*>(::malloc(buffer_size));
        ZeroMemory(mission_ranking_reward_array_, buffer_size);
    };

    for (int row = 0; row < max_rows; row++)
    {
        RANKINGNO mission_no = GetDataWORD("No", row);
        BYTE class_type = GetDataBYTE("Class", row);
        WORD Ranking = GetDataWORD("Ranking", row);
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        BYTE reward_type = GetDataBYTE("RewardType", row);

        REWARD_BASE* reward = const_cast<REWARD_BASE*>(FindReward(mission_no, class_type, Ranking, 
                                                                  reward_type));
        if (reward != NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                _T("Msg='pair of the No:class:Ranking:reward_type=(%d:%d:%d:%d)' already exists"),
                mission_no, class_type, Ranking, reward_type);
            FASSERT(!"pair of the 'No:class:Ranking' already exists");
            return false;
        };
#else
        REWARD_BASE* reward = const_cast<REWARD_BASE*>(FindReward(mission_no, class_type, Ranking));
        if (reward != NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                _T("Msg='pair of the No:class:Ranking=(%d:%d:%d)' already exists"),
                mission_no, class_type, Ranking);
            FASSERT(!"pair of the 'No:class:Ranking' already exists");
            return false;
        };
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        //
        reward = &mission_ranking_reward_array_[row];
        //
        reward->m_MissionNo = mission_no;
        reward->m_Class = class_type;
        reward->m_Ranking = Ranking;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        reward->reward_type_ = reward_type;
        DWORD key = Make_Mission_RewardKey(reward->m_MissionNo, reward->m_Class, reward->m_Ranking, reward->reward_type_);
#else
        DWORD key = Make_Mission_RewardKey(reward->m_MissionNo, reward->m_Class, reward->m_Ranking);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        mission_reward_table_[key] = reward;
        reward->m_RewardCode = GetDataDWORD("RewardCode", row);
    };

    return true;
}

#endif // _NA_008012_20150130_RANKING_SYSTEM