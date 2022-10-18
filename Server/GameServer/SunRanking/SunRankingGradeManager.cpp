#include "stdafx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include "SunRankingPlayer.h"
#include "SunRankingGradeManager.h"
#include "SunRankingParser.h"
#include "SunRankingManager.h"
#include "ServerOptionParserEx.h"
#include "StatueManager.h"
#include "PacketStruct_DW.h"

//////////////////////////////////////////////////////////////////////////
// Compare to Ranking
struct CompareRankingObject 
{
    eSUN_RANKING_TYPE m_ranking_type;
    eSUN_RANKING_GRADE_TYPE m_ranking_grade;

    CompareRankingObject()
    {
        m_ranking_type = eSUN_RANKING_TYPE_END;
        m_ranking_grade = eSUN_RANKING_GRADE_NONE;
    }

    CompareRankingObject(eSUN_RANKING_TYPE ranking_type, eSUN_RANKING_GRADE_TYPE grade)
    {
        m_ranking_type = ranking_type;
        m_ranking_grade = grade;
    }

    bool operator < (const CompareRankingObject& object) const
    {
        const eSUN_RANKING_GRADE_TYPE high_boundary = eSUN_RANKING_GRADE_3;
        const eSUN_RANKING_GRADE_TYPE row_boundary = eSUN_RANKING_GRADE_4;

        if (m_ranking_grade == eSUN_RANKING_GRADE_NONE)
        {
            return true;
        }

        if (object.m_ranking_grade == eSUN_RANKING_GRADE_NONE)
        {
            return false;
        }

        if (m_ranking_type == eSUN_RANKING_TYPE_BEST_OF_BEST
            || object.m_ranking_type  == eSUN_RANKING_TYPE_BEST_OF_BEST)
        {
            if (object.m_ranking_type  != eSUN_RANKING_TYPE_BEST_OF_BEST)
            {
                if (m_ranking_grade < high_boundary)
                {
                    return false;
                }
                else if (object.m_ranking_grade >= row_boundary)
                {
                    return false;
                }
            }

            if (m_ranking_type != eSUN_RANKING_TYPE_BEST_OF_BEST)
            {
                if (object.m_ranking_grade < high_boundary)
                {
                    return true;
                }
                else if (m_ranking_grade >= row_boundary)
                {
                    return true;
                }
            }
        }

        if (object.m_ranking_grade < m_ranking_grade)
        {
            return true;
        }
        else if (object.m_ranking_grade == m_ranking_grade && object.m_ranking_type < m_ranking_type)
        {
            return true;
        }

        return false;
    }

    bool operator > (const CompareRankingObject& object) const
    {
        return (object < *this);
    }
};

class CompareRanking
{
public:
    CompareRanking()
    {
        _GenerateCompare();
    }

    int GetCompareScore(eSUN_RANKING_TYPE ranking_type, 
        eSUN_RANKING_GRADE_TYPE ranking_grade)
    {
        CompareMap::const_iterator find_iter =
            m_compare_map.find(std::make_pair(ranking_type, ranking_grade));
        if (find_iter == m_compare_map.end())
        {
            return 0;
        }

        return find_iter->second;
    }

private:
    typedef std::map<std::pair<eSUN_RANKING_TYPE, eSUN_RANKING_GRADE_TYPE>, int> CompareMap;
    CompareMap m_compare_map;

    void _GenerateCompare()
    {
        for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START;
            type < eSUN_RANKING_TYPE_END; ++type)
        {
            for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START;
                grade < eSUN_RANKING_GRADE_END; ++grade)
            {
                m_compare_map.insert(std::make_pair(std::make_pair(type, grade), 
                    _GetCompareScore(type, grade)));
            }
        }
    }

    int _GetCompareScore(eSUN_RANKING_TYPE ranking_type, 
        eSUN_RANKING_GRADE_TYPE ranking_grade)
    {
        int score = 0;
        CompareRankingObject target(ranking_type, ranking_grade);
        for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START;
            type < eSUN_RANKING_TYPE_END; ++type)
        {
            for (eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_START;
                grade < eSUN_RANKING_GRADE_END; ++grade)
            {
                CompareRankingObject compare_object(type, grade);
                if (target > compare_object)
                {
                    ++score;
                }
            }
        }
        return score;
    }
};
// Compare to Ranking
//////////////////////////////////////////////////////////////////////////

SunRankingGradeManager::SunRankingGradeManager()
{
    if (g_pGameServer->GetMode() == eSERVER_DEV)
    {
        m_update_timer_.SetTimer(DEV_UPDATE_TIME);
    }
    else
    {
        m_update_timer_.SetTimer(UPDATE_TIME);
    }

    Initialize();

    m_last_settle_day = 0;
}

SunRankingGradeManager::~SunRankingGradeManager()
{
}

void SunRankingGradeManager::Initialize()
{
    SetState(eSUN_RANKING_STATE_INIT);

    for (eLOAD_KIND kind = eLOAD_KIND_START; kind < eLOAD_KIND_END; ++kind)
    {
        m_load_complete[kind] = false;
    }
    m_sun_ranking_grades.clear();
}

void SunRankingGradeManager::Update()
{
    if (m_update_timer_.IsExpired())
    {
        switch (m_sun_ranking_state)
        {
        case eSUN_RANKING_STATE_INIT:
            if (g_pGameServer->IsConnected(GAME_DBPROXY) == TRUE)
            {
                {
                    MSG_DG_SUN_RANKING_SELECT_SYN msg;
                    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
                }
                {
                    MSG_DG_SUN_RANKING_GRADE_SELECT_SYN msg;
                    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
                }
                {
                    MSG_DG_STATUE_SELECT_SYN msg;
                    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
                }

                SetState(eSUN_RANKING_STATE_LOAD);

                m_load_complete[eLOAD_KIND_GRADE_SELECT] = false;
                m_load_complete[eLOAD_KIND_RANKING_SELECT] = false;

                m_reload_timer.SetTimer(RELOAD_TIME);
            }
            break;
        case eSUN_RANKING_STATE_LOAD:
            {
                if (IsLoadComplete(eLOAD_KIND_GRADE_SELECT) && IsLoadComplete(eLOAD_KIND_RANKING_SELECT))
                {
                    SetState(eSUN_RANKING_STATE_RUN);
                }
                else
                {
                    // 로드가 완료되지 않았고 로드 대기 시간이 지났다면 재요청한다
                    if (m_reload_timer.IsExpired()) 
                    {
                        if (IsLoadComplete(eLOAD_KIND_RANKING_SELECT) == false)
                        {
                            MSG_DG_SUN_RANKING_SELECT_SYN msg;
                            g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));

                            SUNLOG(eCRITICAL_LOG, "[SunRanking] - Reload Ranking");
                        }
                        if (IsLoadComplete(eLOAD_KIND_GRADE_SELECT) == false)
                        {
                            MSG_DG_SUN_RANKING_GRADE_SELECT_SYN msg;
                            g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));

                            SUNLOG(eCRITICAL_LOG, "[SunRanking] - Reload Grade");
                        }
                    }
                }
            }
            break;
        case eSUN_RANKING_STATE_RUN:
            if (_IsSettleTime() == true)
            {
                //Settle();
                //바로 Settle상태로 돌리지 않고, DBP에게 요청 후, DBP의 시작 통지를 받아서 Settle()이 동작하도록 하자.
                //각 채널의 FieldServer 정산 상태 동기화를 위함
                if (IsSettleChannel() == true)
                {
                    MSG_DG_SUN_RANKING_SETTLE_START_SYN msg;
                    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
                }
                return;
            }
            break;
        case eSUN_RANKING_STATE_SETTLE_WAIT:
            {
            }
            break;
        case eSUN_RANKING_STATE_SEND_MEMO:
            {
                if (IsLoadComplete(eLOAD_KIND_GRADE_SELECT))
                {
                    m_load_complete[eLOAD_KIND_GRADE_SELECT] = false;
                    Settle_SendMemo();
                    ReUse();
                }
            }
            break;
        default:
            break;
        }
    }
}

void SunRankingGradeManager::SetGradeInfo( SunRankingGradeInfo& grade_info )
{
    SunRankingGrades::iterator find_iter = m_sun_ranking_grades.find(grade_info.char_guid);
    if (find_iter == m_sun_ranking_grades.end())
    {
        SunRankingGradeInfos infos;
        eSUN_RANKING_TYPE ranking_type = 
            static_cast<eSUN_RANKING_TYPE>(grade_info.ranking_type);
        infos.insert(std::make_pair(ranking_type, grade_info));
        m_sun_ranking_grades.insert(std::make_pair(grade_info.char_guid, infos));
    }
    else
    {
        SunRankingGradeInfos& infos = find_iter->second;
        eSUN_RANKING_TYPE ranking_type = 
            static_cast<eSUN_RANKING_TYPE>(grade_info.ranking_type);
        SunRankingGradeInfos::iterator cur_iter = infos.find(ranking_type);
        if (cur_iter != infos.end())
        {
            infos.erase(cur_iter);
        }
        infos.insert(std::make_pair(ranking_type, grade_info));
    }
}

bool SunRankingGradeManager::_IsSettleTime()
{
    if (GetState() != eSUN_RANKING_STATE_RUN)
    {
        return false;
    }

    time_t t;
    util::TimeSync::time(&t);
    struct tm* cur_time_struct = localtime(&t);
    if (cur_time_struct == NULL)
    {
        return false;
    }

    if (SunRankingParser::Instance()->GetSettleDateType() == eSUN_RANKING_SETTLE_MONTH
        && SunRankingParser::Instance()->GetSettleDateDay() != cur_time_struct->tm_mday)
    {
        return false;
    }

    if (SunRankingParser::Instance()->GetSettleDateType() == eSUN_RANKING_SETTLE_WEEK
        && SunRankingParser::Instance()->GetSettleDateDay() != cur_time_struct->tm_wday)
    {
        return false;
    }

    if (SunRankingParser::Instance()->GetSettleDateTime() !=
        cur_time_struct->tm_hour * 100 + cur_time_struct->tm_min)
    {
        return false;
    }

    if (m_last_settle_day == cur_time_struct->tm_mday)
    {
        return false;
    }
    m_last_settle_day = cur_time_struct->tm_mday;

    return true;
}

bool SunRankingGradeManager::GetGradeInfo( CHARGUID char_guid, SunRankingGradeInfos& infos )
{
    SunRankingGrades::iterator find_iter = m_sun_ranking_grades.find(char_guid);
    if (find_iter == m_sun_ranking_grades.end())
    {
        return false;
    }

    infos = find_iter->second;

    return true;
}

void SunRankingGradeManager::GetTopGrade( CHARGUID char_guid, eSUN_RANKING_GRADE_TYPE& top_grade, eSUN_RANKING_TYPE& top_ranking_type )
{
    SunRankingGradeInfos infos;
    if (GetGradeInfo(char_guid, infos) == false)
    {
        top_grade = eSUN_RANKING_GRADE_NONE;
        top_ranking_type = eSUN_RANKING_TYPE_END;
        return;
    }

    eSUN_RANKING_GRADE_TYPE grade = eSUN_RANKING_GRADE_NONE;
    eSUN_RANKING_TYPE ranking_type = eSUN_RANKING_TYPE_END;

    SunRankingGradeInfos::iterator cur_iter = infos.begin();
    for (; cur_iter != infos.end(); ++cur_iter)
    {
        SunRankingGradeInfo& info = cur_iter->second;
        eSUN_RANKING_GRADE_TYPE iter_grade = 
            static_cast<eSUN_RANKING_GRADE_TYPE>(info.grade);
        eSUN_RANKING_TYPE iter_ranking_type = 
            static_cast<eSUN_RANKING_TYPE>(info.ranking_type);
        static CompareRanking compare_ranking;
        if (compare_ranking.GetCompareScore(iter_ranking_type, iter_grade)
            > compare_ranking.GetCompareScore(ranking_type, grade))
        {
            grade = iter_grade;
            ranking_type = iter_ranking_type;
        }
    }
    top_grade = grade;
    top_ranking_type = ranking_type;

    if (top_grade >= eSUN_RANKING_GRADE_4)
    {
        top_grade = eSUN_RANKING_GRADE_NONE;
    }
}

void SunRankingGradeManager::ReUse()
{
    Initialize();

    SunRankingManager::Instance()->Release();
    SunRankingManager::Instance()->Initialize();
}

void SunRankingGradeManager::Settle_SendMemo()
{
    SunRankingGrades::iterator cur_iter = m_sun_ranking_grades.begin();
    SunRankingGrades::iterator end_iter = m_sun_ranking_grades.end();
    MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD cmd_msg;
    cmd_msg.Clear();
    const int max_system_memo_size = 20;    // MAX_SYSTEM_MEMO_LIST 는 패킷 크기가 너무 크다
    for (; cur_iter != end_iter; ++cur_iter)
    {
        SunRankingGradeInfos& infos = cur_iter->second;

        SunRankingGradeInfos::iterator infos_cur_iter = infos.begin();
        SunRankingGradeInfos::iterator infos_end_iter = infos.end();
        for (; infos_cur_iter != infos_end_iter; ++infos_cur_iter)
        {
            SunRankingGradeInfo& grade_info = infos_cur_iter->second;

            if (grade_info.ranking_type < eSUN_RANKING_TYPE_START || grade_info.ranking_type >= eSUN_RANKING_TYPE_END)
            {
                continue;
            }

            if (grade_info.grade >= eSUN_RANKING_GRADE_4)
            {
                continue;
            }

            int rank = 0;
            eSUN_RANKING_TYPE rank_type = static_cast<eSUN_RANKING_TYPE>(grade_info.ranking_type);
            SunRankingManager::Instance()->GetRanking(rank_type, grade_info.char_guid, rank);

            BasicSystemMemoInfo& memo = cmd_msg.system_memo_info_[cmd_msg.system_memo_list_num_];
            memo.recv_user_guid_ = grade_info.char_guid;
            memo.system_memo_type_ = SYSMEMO_SUN_RANKING_SETTLE_RESULT;
            _sntprintf(memo.system_memo_contents_, MAX_MEMO_LENGTH, "%d|%d|%d|", 
                SYSMEMO_SUN_RANKING_SETTLE_RESULT, grade_info.ranking_type, rank);

            if (cmd_msg.system_memo_list_num_ == max_system_memo_size)
            {
                g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, cmd_msg.GetSize());
                cmd_msg.Clear();
            }
            else
            {
                ++cmd_msg.system_memo_list_num_;
            }
        }
    }

    if (cmd_msg.system_memo_list_num_ != 0)
    {
        g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, cmd_msg.GetSize());
    }
}

bool SunRankingGradeManager::IsSettleChannel()
{
    BYTE channel_id = ServerOptionParserEx::Instance()->GetServerOption().sun_ranking_settle_channel;

    if (g_pGameServer->GetKey().GetChannelID() == channel_id 
        && g_pGameServer->GetServerType() == FIELD_SERVER)
    {
        return true;
    }
    return false;
}

void SunRankingGradeManager::Settle_StartForGM()
{
    if (GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    MSG_DG_SUN_RANKING_SETTLE_START_SYN msg;
    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
}

void SunRankingGradeManager::SetState( eSUN_RANKING_STATE state )
{
    m_sun_ranking_state = state;

    static const int MAX_RANKING_STATE_STR = 64;
    static const char ranking_state_str[eSUN_RANKING_STATE_MAX][MAX_RANKING_STATE_STR] = {
        "eSUN_RANKING_STATE_INIT",
        "eSUN_RANKING_STATE_LOAD",
        "eSUN_RANKING_STATE_RUN",
        "eSUN_RANKING_STATE_SETTLE_WAIT",
        "eSUN_RANKING_STATE_SEND_MEMO"
    };

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - State : %s", ranking_state_str[state]);
}

void SunRankingGradeManager::Settle()
{
    if (GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    SetState(eSUN_RANKING_STATE_SETTLE_WAIT);

    if (IsSettleChannel() != true)
    {
        return;
    }

    MSG_DG_SUN_RANKING_SETTLE_SYN msg;
    SunRankingParser::Instance()->GetBoundary(msg.grade_boundary);
    SunRankingManager::Instance()->GetTopRankerEquipmentScore(msg.statue_char_equip_score, msg.statue_char_guid);
    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
}

void SunRankingGradeManager::Settle_Complete()
{
    if (IsSettleChannel() == true)
    {
        m_sun_ranking_grades.clear();   // 이전 등급 정보를 삭제한다

        MSG_DG_SUN_RANKING_GRADE_SELECT_SYN msg;
        g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
        SetState(eSUN_RANKING_STATE_SEND_MEMO);
        m_load_complete[eLOAD_KIND_GRADE_SELECT] = false;
    }
    else
    {
        ReUse();
    }
}

void SunRankingGradeManager::Display()
{
    static const int MAX_RANKING_STATE_STR = 64;
    static const char ranking_state_str[eSUN_RANKING_STATE_MAX][MAX_RANKING_STATE_STR] = {
        "eSUN_RANKING_STATE_INIT",
        "eSUN_RANKING_STATE_LOAD",
        "eSUN_RANKING_STATE_RUN",
        "eSUN_RANKING_STATE_SETTLE_WAIT",
        "eSUN_RANKING_STATE_SEND_MEMO"
    };

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - State : %s", ranking_state_str[m_sun_ranking_state]);
    SUNLOG(eCRITICAL_LOG, "[SunRanking] - Grades Count : %d", m_sun_ranking_grades.size());

    if (m_sun_ranking_state == eSUN_RANKING_STATE_LOAD)
    {
        SUNLOG(eCRITICAL_LOG, "[SunRanking] - Ranking Load State : %d",
            m_load_complete[eLOAD_KIND_RANKING_SELECT]);
        SUNLOG(eCRITICAL_LOG, "[SunRanking] - Grade Load State : %d",
            m_load_complete[eLOAD_KIND_GRADE_SELECT]);
    }
}

void SunRankingGradeManager::SetLoadComplete( eLOAD_KIND load_kind )
{
    m_load_complete[load_kind] = true;
}

bool SunRankingGradeManager::IsLoadComplete( eLOAD_KIND load_kind )
{
    return m_load_complete[load_kind];
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM