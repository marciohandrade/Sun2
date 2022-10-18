#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifndef DBPROXY_SUN_RANKING_SETTLE_H
#define DBPROXY_SUN_RANKING_SETTLE_H

class ChildServerSession;

enum eSUN_RANKING_SETTLE_STATE
{
    eSUN_RANKING_SETTLE_STATE_INIT = 0,                 // 비정산 상태
    eSUN_RANKING_SETTLE_STATE_INIT_GRADE,               // 등급 초기화
    eSUN_RANKING_SETTLE_STATE_SETTLE_GRADE,             // 등급 정산
    eSUN_RANKING_SETTLE_STATE_INIT_RANK,                // 랭킹 초기화
    eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR,       // 동상 캐릭터 등록
    eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR_ITEM,  // 동상 캐릭터 아이템 등록
    eSUN_RANKING_SETTLE_STATE_SETTLE_COMPLETE,          // 정산 완료
    eSUN_RANKING_SETTLE_STATE_MAX,
};

class SunRankingSettle : public util::Singleton<SunRankingSettle>
{
public:

    SunRankingSettle();
    ~SunRankingSettle();

    void Initialize();

    void SetGradeBoundary(const int const grade_boundary[eSUN_RANKING_TYPE_END][eSUN_RANKING_GRADE_END]);
    void SetStatueInfo(CHARGUID char_guid, SCORE equip_score);
    LoadControl& GetLoadControl();
    void SetState( eSUN_RANKING_SETTLE_STATE state );

    void InitGrade(ChildServerSession* child_server_session);
    void NextGradeSettle(eSUN_RANKING_TYPE rank_type);
    void InitRanking();
    void NewStatueChar();
    void NewStatueCharItem();
    void Complete();

private:
    ChildServerSession* m_child_server_session;

    eSUN_RANKING_SETTLE_STATE m_sun_ranking_state;
    LoadControl m_load_control;

    int m_grade_boundary [eSUN_RANKING_TYPE_END][eSUN_RANKING_GRADE_END];
    eSUN_RANKING_GRADE_TYPE m_cur_sun_ranking_grade[eSUN_RANKING_TYPE_END];

    CHARGUID m_statue_char_guid;
    SCORE    m_statue_char_equip_score;

};

#endif // DBPROXY_SUN_RANKING_SETTLE_H
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM