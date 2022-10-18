#pragma once

#ifndef PRAGRAMCOMMON_SUN_RANKING_PARSER_H
#define PRAGRAMCOMMON_SUN_RANKING_PARSER_H
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include "Struct.h"

class SunRankingParser : public util::Singleton<SunRankingParser>, public IParser
{
public:
    SunRankingParser(void){};
    ~SunRankingParser(void){};

public:
    /** Parser **/
    virtual void Release(){};
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    float GetWeight( eSUN_RANKING_TYPE ranking_type );
    int   GetBoundary( eSUN_RANKING_TYPE ranking_type, eSUN_RANKING_GRADE_TYPE grade );
    void  GetBoundary( int(* grade_boundary)[eSUN_RANKING_GRADE_END] );

    int GetSettleDateType() const { return m_settle_date_type; };
    int GetSettleDateDay() const { return m_settle_date_day; };
    int GetSettleDateTime() const { return m_settle_date_time; };
    
private:
    std::map<eSUN_RANKING_TYPE, float> m_sun_ranking_weights;
    std::map<std::pair<eSUN_RANKING_TYPE, eSUN_RANKING_GRADE_TYPE>, int> m_grade_boundaries;

    int m_settle_date_type;
    int m_settle_date_day;
    int m_settle_date_time;
};

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#endif // PRAGRAMCOMMON_SUN_RANKING_PARSER_H