#pragma once
#ifndef SERVER_GAMESERVER_SUNRANKINGGRADEMANAGER_H
#define SERVER_GAMESERVER_SUNRANKINGGRADEMANAGER_H

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
enum eLOAD_KIND
{
    eLOAD_KIND_START = 0,
    eLOAD_KIND_RANKING_SELECT = eLOAD_KIND_START,
    eLOAD_KIND_GRADE_SELECT,
    eLOAD_KIND_END,
};

static eLOAD_KIND& operator++(eLOAD_KIND& object)
{
    if (object != eLOAD_KIND_END)
    {
        object = static_cast<eLOAD_KIND>(object + 1);
    }
    return object;
}

typedef std::map<eSUN_RANKING_TYPE, SunRankingGradeInfo> SunRankingGradeInfos;
typedef std::map<CHARGUID, SunRankingGradeInfos> SunRankingGrades;

class SunRankingGradeManager : public util::Singleton<SunRankingGradeManager>
{
public:
    SunRankingGradeManager();
    ~SunRankingGradeManager();

    void Initialize();
    void Update();
    void Display();

    eSUN_RANKING_STATE GetState(){ return m_sun_ranking_state; };
    void SetState(eSUN_RANKING_STATE state);

    void Settle();
    void Settle_StartForGM();
    void Settle_SendMemo();
    void Settle_Complete();

    void SetGradeInfo(SunRankingGradeInfo& grade_info);
    bool GetGradeInfo(CHARGUID char_guid, SunRankingGradeInfos& infos);
    void GetTopGrade(CHARGUID char_guid, eSUN_RANKING_GRADE_TYPE& top_grade, eSUN_RANKING_TYPE& top_ranking_type);

    void SetLoadComplete(eLOAD_KIND load_kind);
    bool IsLoadComplete(eLOAD_KIND load_kind);

    bool IsSettleChannel();
    void ReUse();

private:
    bool _IsSettleTime();

    SunRankingGrades   m_sun_ranking_grades;
    eSUN_RANKING_STATE m_sun_ranking_state;
    util::Timer m_update_timer_;
    util::Timer m_reload_timer;
    int  m_last_settle_day;
    bool m_load_complete[eLOAD_KIND_END];

    enum
    {
        UPDATE_TIME = 1000 * 10,                // 라이브 업데이트 주기
        DEV_UPDATE_TIME = 1000,                 // 개발 업데이트 주기
        RELOAD_TIME = 1000 * 60 * 2             // 라이브 리로드 시간
    };
};

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // SERVER_GAMESERVER_SUNRANKINGGRADEMANAGER_H