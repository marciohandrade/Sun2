#include "stdafx.h"

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM

#include "Services/SunRankingSettle.h"
#include "PacketHandler\Handler_FromGameServer.h"
#include "Sessions/ChildServerSession.h"

SunRankingSettle::SunRankingSettle()
{
    Initialize();
}

SunRankingSettle::~SunRankingSettle()
{
}

void SunRankingSettle::Initialize()
{
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        m_cur_sun_ranking_grade[type] = eSUN_RANKING_GRADE_START;
    }

    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START; grade < eSUN_RANKING_GRADE_END; ++grade)
        {
            m_grade_boundary[type][grade] = 0;
        }
    }
    SetState(eSUN_RANKING_SETTLE_STATE_INIT);
    m_child_server_session = NULL;
    m_load_control.init(eSUN_RANKING_GRADE_END * eSUN_RANKING_TYPE_END);
}

void SunRankingSettle::SetGradeBoundary( const int const grade_boundary[eSUN_RANKING_TYPE_END][eSUN_RANKING_GRADE_END] )
{
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START; grade < eSUN_RANKING_GRADE_END; ++grade)
        {
            m_grade_boundary[type][grade] = grade_boundary[type][grade];
        }
    }
}

void SunRankingSettle::InitGrade( ChildServerSession* child_server_session )
{
    if (child_server_session == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| settle failed", __FUNCTION__);
        return;
    }

    m_child_server_session = child_server_session;
    SetState(eSUN_RANKING_SETTLE_STATE_INIT_GRADE);

    STRING_SAFER_QUERY1024 query_buffer;
    query_buffer.MakeString("{?=call S_SunRankingGrade_Init}");
    Query_SunRankingGrade_Init* query = Query_SunRankingGrade_Init::ALLOC();
    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_GRADE_INIT_DBR));
    query->SetVoidObject(child_server_session);
    child_server_session->DBQuery(query);
}

void SunRankingSettle::NextGradeSettle( eSUN_RANKING_TYPE rank_type )
{
    if (m_child_server_session == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| settle failed", __FUNCTION__);
        return;
    }

    if (GetLoadControl().IsLoaded())
    {
        InitRanking();
        return;
    }

    if(rank_type < eSUN_RANKING_TYPE_START || eSUN_RANKING_TYPE_END <= rank_type)
    {
        return;
    }

    if (m_cur_sun_ranking_grade[rank_type] < eSUN_RANKING_GRADE_START || eSUN_RANKING_GRADE_END <= m_cur_sun_ranking_grade[rank_type])
    {
        return;
    }

    STRING_SAFER_QUERY1024 query_buffer;
    query_buffer.MakeString("{?=call S_SunRankingGrade_Insert(%d, %d, %d)}",
        rank_type, m_cur_sun_ranking_grade[rank_type], m_grade_boundary[rank_type][m_cur_sun_ranking_grade[rank_type]]);
    Query_SunRankingGrade_Insert* query = Query_SunRankingGrade_Insert::ALLOC();
    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_GRADE_INSERT_DBR));
    query->SetVoidObject(m_child_server_session);
    query->SetSunRankingType(rank_type);
    query->SetSunRankingGrade(m_cur_sun_ranking_grade[rank_type]);
    m_child_server_session->DBQuery(query);
    ++m_cur_sun_ranking_grade[rank_type];
}

void SunRankingSettle::InitRanking()
{
    if (m_child_server_session == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| settle failed", __FUNCTION__);
        return;
    }

    SetState(eSUN_RANKING_SETTLE_STATE_INIT_RANK);

    STRING_SAFER_QUERY1024 query_buffer;
    query_buffer.MakeString("{?=call S_SunRanking_Init}");
    Query_SunRankingGrade_Init* query = Query_SunRankingGrade_Init::ALLOC();
    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_INIT_DBR));
    query->SetVoidObject(m_child_server_session);
    m_child_server_session->DBQuery(query);
}

LoadControl& SunRankingSettle::GetLoadControl()
{
    return m_load_control;
}

void SunRankingSettle::NewStatueChar()
{
    if (m_statue_char_guid == 0)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| settle failed", __FUNCTION__);
        return;
    }
    SetState(eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR);

    MSG_DG_STATUE_CHAR_INSERT_SYN insert_msg;
    insert_msg.statue_type = eSTATUE_TYPE_BEST_OF_BEST;
    insert_msg.statue_index = eSTATUE_INDEX_FIRST;
    insert_msg.char_guid = m_statue_char_guid;
    insert_msg.score = m_statue_char_equip_score;
    Handler_FromGameServer::OnDG_STATUE_CHAR_INSERT_SYN(m_child_server_session, &insert_msg, sizeof(insert_msg));

}

void SunRankingSettle::NewStatueCharItem()
{
    if (m_statue_char_guid == 0)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| settle failed", __FUNCTION__);
        return;
    }
    SetState(eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR_ITEM);

    MSG_DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN syn_msg;
    syn_msg.statue_type = eSTATUE_TYPE_BEST_OF_BEST;
    syn_msg.statue_index = eSTATUE_INDEX_FIRST;
    syn_msg.char_guid = m_statue_char_guid;;
    Handler_FromGameServer::OnDG_STATUE_ITEM_SLOT_INFO_INSERT_SYN(m_child_server_session, &syn_msg, sizeof(syn_msg));
}

void SunRankingSettle::Complete()
{
    SetState(eSUN_RANKING_SETTLE_STATE_SETTLE_COMPLETE);

    MSG_DG_SUN_RANKING_SETTLE_COMPLETE_CMD cmd_msg;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &cmd_msg, sizeof(cmd_msg));

    Initialize();
}

void SunRankingSettle::SetState( eSUN_RANKING_SETTLE_STATE state )
{
    m_sun_ranking_state = state;

    static const int MAX_RANKING_STATE_STR = 64;
    static const char ranking_state_str[eSUN_RANKING_SETTLE_STATE_MAX][MAX_RANKING_STATE_STR] = {
        "eSUN_RANKING_SETTLE_STATE_INIT",
        "eSUN_RANKING_SETTLE_STATE_INIT_GRADE",
        "eSUN_RANKING_SETTLE_STATE_GRADE",
        "eSUN_RANKING_SETTLE_STATE_INIT_RANK",
        "eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR",
        "eSUN_RANKING_SETTLE_STATE_REGIST_STATUE_CHAR_ITEM",
        "eSUN_RANKING_SETTLE_STATE_COMPLETE",
    };

    SUNLOG(eCRITICAL_LOG, "[SunRankingSettle] - State : %s", ranking_state_str[state]);
}

void SunRankingSettle::SetStatueInfo( CHARGUID char_guid, SCORE equip_score )
{
    m_statue_char_guid = char_guid;
    m_statue_char_equip_score = equip_score;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM