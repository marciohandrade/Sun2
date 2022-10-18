#include "StdAfx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRankingParser.h"

BOOL SunRankingParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));
    const int MAX_RANKING_STR = 64;
    const char ranking_str[eSUN_RANKING_TYPE_END][MAX_RANKING_STR] = {
        "BEST_OF_BEST",
        "ELETE_WARRIOR",
        "LEVEL_KING",
        "LEGEND_EQUIP",
        "STRONG_ONE_SHOT",
        "BEST_ATTACKER",
        "MASTER_OF_HEAL"
    };

    // weight
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        float temp_weight = GetINIDataFloat("SUN_RANKING_WEIGHT", ranking_str[type]);
        m_sun_ranking_weights.insert(std::make_pair(type, temp_weight));
    }

    // settel date
    m_settle_date_type = GetINIDataInt("SUN_RANKING_SETTLE_DATE", "DATE_TYPE");
    m_settle_date_day  = GetINIDataInt("SUN_RANKING_SETTLE_DATE", "DATE_DAY");
    m_settle_date_time = GetINIDataInt("SUN_RANKING_SETTLE_DATE", "DATE_TIME");

    // grade info
    char temp_buffer[128];
    ZeroMemory(&temp_buffer, sizeof(temp_buffer));
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START; grade < eSUN_RANKING_GRADE_END; ++grade)
        {
            _snprintf(temp_buffer, _countof(temp_buffer), "%s_GRADE_BOUNDARY_%d", ranking_str[type], grade);
            int boundary = GetINIDataFloat("SUN_RANKING_GRADE_INFO", temp_buffer);
            m_grade_boundaries.insert(std::make_pair(std::make_pair(type, grade), boundary));
        }
    }

    return TRUE;
}

float SunRankingParser::GetWeight( eSUN_RANKING_TYPE ranking_type )
{
    if (ranking_type < eSUN_RANKING_TYPE_START || eSUN_RANKING_TYPE_END <= ranking_type)
    {
        // eSUN_RANKING_TYPE_END
        return 0.0f;
    }
    
    std::map<eSUN_RANKING_TYPE, float>::iterator cur_iter = m_sun_ranking_weights.find(ranking_type);
    if (cur_iter == m_sun_ranking_weights.end())
    {
        return 0.0f;
    }
    
    return cur_iter->second;
}

int SunRankingParser::GetBoundary( eSUN_RANKING_TYPE ranking_type, eSUN_RANKING_GRADE_TYPE grade )
{
    std::map<std::pair<eSUN_RANKING_TYPE, eSUN_RANKING_GRADE_TYPE>, int>::iterator cur_iter = m_grade_boundaries.find(std::make_pair(ranking_type, grade));

    if (cur_iter == m_grade_boundaries.end())
    {
        return 0;
    }

    return cur_iter->second;
}

void SunRankingParser::GetBoundary( int(* grade_boundary)[eSUN_RANKING_GRADE_END] )
{
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START; grade < eSUN_RANKING_GRADE_END; ++grade)
        {
            grade_boundary[type][grade] = GetBoundary(type, grade);
        }
    }
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
