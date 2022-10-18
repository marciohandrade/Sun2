#include <stdafx.h>
#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "HonorSystem.h"
#include <HonorSystem/HonorSystemPacket.h>
#include "TitleListParser.h"
#include "ReputeListParser.h"
#include "FameListParser.h"
#include "QuestManager_Concrete.h"
#include "QuestPacket.h"
#include "AchievementManager.h"

//==================================================================================================
//==================================================================================================
//==================================================================================================

TodayQuestSystem::TodayQuestSystem()
{
    today_quest_data.clear();
};

TodayQuestSystem::~TodayQuestSystem()
{
};

const TodayQuestSystem::QUEST_TODAY_QUEST_INFO_EX*
TodayQuestSystem::GetTodayQuest(WORD group) const
{
    TodayQuestData::const_iterator it = today_quest_data.find(group);
    if (it == today_quest_data.end()) {
        return NULL;
    };
    return &(it->second);
}

bool TodayQuestSystem::IsTodayQuest(WORD group, WORD code) const
{
    const QUEST_TODAY_QUEST_INFO_EX* today_quest_info = GetTodayQuest(group);
    if(!today_quest_info) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Daily Quest Error! GroupCode Error[Group:%d]", group);
        return false;
    }

    if(today_quest_info->quest_code != code) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Daily Quest Error! QuestCode Error[Group:%d][ServerQuestCode:%d]", 
               group, today_quest_info->quest_code);
        return false;
    }

    return true;
}

CTime TodayQuestSystem::GetNextUpdateTime(WORD qcode) const
{
    QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(qcode);
    if (quest_info == NULL)
        return -1; // quest info empty

    int daily_quest_init_day = quest_info->GetSetBackDay();
    if (daily_quest_init_day == 0)
        return -1; // 초기화가 되지 않는 일일퀘스트

    CTime current_time = util::TimeSync::_time64(NULL);
    CTime daily_init_time = CTime(current_time.GetYear(), 
        current_time.GetMonth(), 
        current_time.GetDay(), 
        quest_info->GetSetBackTime() / 100, 
        quest_info->GetSetBackTime() % 100, 
        0);

    if (daily_init_time < current_time)
    {
        daily_init_time += CTimeSpan(1, 0, 0, 0);
    }

    if ( daily_quest_init_day >= 2 && daily_quest_init_day <= 8)
    {
        int init_day_of_week = (daily_quest_init_day == 8) ? 1 :daily_quest_init_day;
        init_day_of_week = init_day_of_week - daily_init_time.GetDayOfWeek();
        if (init_day_of_week < 0)
            init_day_of_week += 7;

        daily_init_time += CTimeSpan(init_day_of_week, 0, 0, 0);
    }

    return daily_init_time;
}

bool TodayQuestSystem::UpdateTodayQuest(bool force)
{
    bool updated = false;

    CTime current_time = util::TimeSync::_time64(NULL);

    QUEST_GROUP_MAP& quest_group_map = g_QuestInfoManager.quest_group_map();
    QUEST_GROUP_MAP::iterator group_map_it = quest_group_map.begin(),
                              group_map_end = quest_group_map.end();
    for( ; group_map_it != group_map_end; ++group_map_it)
    {
        QUEST_CODE_VECTOR& quest_code_vector = group_map_it->second;
        int group_code = group_map_it->first;
        //////////////////////////////////////////////////////////////////////////
        TodayQuestData::iterator it = today_quest_data.find(group_code);
        if(it == today_quest_data.end())
        {
            // CHANGES: f110831.2L, prevent the divide by zero exception when code_count is zero.
            // and prevent the index out of range exception when the max value less than the min.
            QUEST_CODE_VECTOR::size_type code_count = quest_code_vector.size();
            if (code_count == 0) {
                continue;
            };
            WORD quest_code = quest_code_vector[random(1, code_count) - 1];

            QUEST_TODAY_QUEST_INFO_EX today_quest_info;
            today_quest_info.quest_group = group_code;
            today_quest_info.quest_code  = quest_code;
            today_quest_info.next_update_time = GetNextUpdateTime(quest_code);

            today_quest_data.insert(TodayQuestData::value_type(group_code, today_quest_info));

            SUNLOG(eCRITICAL_LOG, _T("TODAY QUEST : %d-%d"), group_code, quest_code);

            updated = true;
        }
        else
        {
            QUEST_TODAY_QUEST_INFO_EX& today_quest_info = it->second;
            if( today_quest_info.next_update_time < current_time ||
                force == true )
            {
                int code_count  = quest_code_vector.size();
                WORD quest_code = quest_code_vector[random(1, code_count)-1];

                today_quest_info.quest_group = group_code;
                today_quest_info.quest_code  = quest_code;
                today_quest_info.next_update_time = GetNextUpdateTime(quest_code);

                SUNLOG(eCRITICAL_LOG, _T("TODAY QUEST : %d-%d"), group_code, quest_code);

                updated = true;
            }
        }
    }

    return updated;
}

void TodayQuestSystem::Serialize(QUEST_TODAY_QUEST_INFO* _quest_info, WORD& _count)
{
    _count = 0;

    for (TodayQuestData::iterator it = today_quest_data.begin();
         it != today_quest_data.end();
         ++it)
    {
        _quest_info[_count].quest_group = (it->second).quest_group;
        _quest_info[_count].quest_code  = (it->second).quest_code;

        ++_count;

        if(_count >= MAX_TODAYQUEST_COUNT) {
            SUNLOG(eCRITICAL_LOG, _T("[%s][%d] TODAY QUEST count >= MAX_TODAYQUEST_COUNT"), 
                __FUNCTION__, __LINE__);
        }
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

HonorSystem::HonorSystem()
{
    update_timer_.SetTimer(kUpdateInterval);
}

HonorSystem::~HonorSystem()
{

}

void HonorSystem::ParseCGPacket(PVOID packet)
{
    MSG_CG_HONORSYSTEM_PACKET* packet2 = static_cast<MSG_CG_HONORSYSTEM_PACKET*>(packet);
    switch(packet2->packet_type)
    {
        case CG_HONORSYSTEM_TITLE:
            {
                ParseCGPacketTitle(packet2); 
            } break;
    }
}

void HonorSystem::ParseCGPacketTitle( PVOID packet )
{
    CG_HONORSYSTEM_TITLE_PACKET* packet2 = static_cast<CG_HONORSYSTEM_TITLE_PACKET*>(packet);
    switch(packet2->packet_type2)
    {
        case CG_HONORSYSTEM_TITLE_CHANGE_CMD:
            {
                OnPACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD(packet2);
            } break;
    }
}

void HonorSystem::OnPACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD(PVOID packet)
{
    PACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD* msg = \
        static_cast<PACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD*>(packet);

    Player * player = PlayerManager::Instance()->FindPlayerByUserKey( msg->m_dwKey );
    ASSERT( player );
    if( !player ) 
        return;

    WORD honor_title = msg->honor_title_index_;

    if(player->GetHonorManager()->SetHonorTitle(honor_title))
    {
        PACKET_CG_HONORSYSTEM_TITLE_CHANGE_BRD send_msg( \
            player->GetObjectKey(), player->GetHonorManager()->GetHonorTitle());
        player->SendPacketAround(&send_msg, sizeof(send_msg));
    }
    else
    {
        PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
        send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_CHANGE_FAIL;
        player->SendPacket(&send_msg, sizeof(send_msg));
    }
}

void HonorSystem::ParseDGPacket( PVOID packet )
{
    MSG_DG_CHARINFO_HONORSYSTEM* recv_msg = \
        static_cast<MSG_DG_CHARINFO_HONORSYSTEM*>(packet);

    switch(recv_msg->packet_type)
    {
    case recv_msg->DG_HONORSYSTEM_TITLES:
        ParseDGPacketTitle(packet);
        break;
    case recv_msg->DG_HONORSYSTEM_FAME_REPUTE:
        ParseDGPacketFameRepute(packet);
        break;
    case recv_msg->DG_HONORSYSTEM_DAILYQUEST:
        ParseDGPacketDailyQuest(packet);
        break;
    default:
        ASSERT(!"received an unexpected message");
        break;
    };
}

void HonorSystem::ParseDGPacketTitle( PVOID packet )
{
    MSG_DG_HONORSYSTEM_TITLES* recv_msg = static_cast<MSG_DG_HONORSYSTEM_TITLES*>(packet);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( recv_msg->m_dwKey );
    if( !player ) return ;
    
    HonorManager* const honor_manager = player->GetHonorManager();

    switch(recv_msg->sub_type)
    {
        case DG_HONORSYSTEM_TITLES_SELECT_ACK:
            {
                MSG_DG_HONORTITLES_SELECT_ACK* recv_msg2 = \
                    static_cast<MSG_DG_HONORTITLES_SELECT_ACK*>(packet);

                honor_manager->SerializeTitles(recv_msg2->titles, SERIALIZE_STORE);

                PACKET_CG_HONORSYSTEM_TITLE_LIST_ACK send_msg;
                honor_manager->SerializeTitles(send_msg.titles_, SERIALIZE_LOAD);
                player->SendPacket(&send_msg, send_msg.GetSize());
            } break;
        case DG_HONORSYSTEM_TITLES_ADD_ACK:
            {
                MSG_DG_HONORTITLES_ADD_REMOVE_ACK* recv_msg2 = \
                    static_cast<MSG_DG_HONORTITLES_ADD_REMOVE_ACK*>(packet);

                if(recv_msg2->result == 0) // 0 is no error
                {
                    honor_manager->AddHonorTitle(recv_msg2->honor_title_index);
                }
                else
                {
                    PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
                    send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_ADD_FAIL;
                    player->SendPacket(&send_msg, sizeof(send_msg));
                }
            } break;
        case DG_HONORSYSTEM_TITLES_REMOVE_ACK:
            {
                MSG_DG_HONORTITLES_ADD_REMOVE_ACK* recv_msg2 = \
                    static_cast<MSG_DG_HONORTITLES_ADD_REMOVE_ACK*>(packet);

                if(recv_msg2->result == 0) // 0 is no error
                {
                    honor_manager->RemoveHonorTitle(recv_msg2->honor_title_index);
                }
                else
                {
                    PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
                    send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_REMOVE_FAIL;
                    player->SendPacket(&send_msg, sizeof(send_msg));
                }
            } break;
    }
}

void HonorSystem::ParseDGPacketFameRepute( PVOID packet )
{
    MSG_DG_HONORSYSTEM_FAME_REPUTE* recv_msg = static_cast<MSG_DG_HONORSYSTEM_FAME_REPUTE*>(packet);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( recv_msg->m_dwKey );
    if( !player ) return ;

    HonorManager* const honor_manager = player->GetHonorManager();

    switch(recv_msg->sub_type)
    {
        case DG_HONORSYSTEM_FAME_REPUTE_SELECT_ACK:
            {
                MSG_DG_FAME_REPUTE_SELECT_ACK* recv_msg2 = \
                    static_cast<MSG_DG_FAME_REPUTE_SELECT_ACK*>(packet);

                honor_manager->SerializeFameRepute(recv_msg2->honor_point_table, SERIALIZE_STORE);

                PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK send_msg;
                honor_manager->SerializeFameRepute(send_msg.honor_point_table_, SERIALIZE_LOAD);
                player->SendPacket(&send_msg, send_msg.GetSize());
            } break;
    }
}

WORD HonorSystem::GetFameReputeGrade( BYTE type, WORD code, DWORD point )
{
    WORD grade = 0;
    
    if(type == HONOR_POINT_INFO::FAME)
    {
        FameListParser::FameInfo* info = FameListParser::Instance()->GetFameInfo(code);
        grade = FameListParser::GetFameRating(info, point);
    }
    else if(type == HONOR_POINT_INFO::REPUTE)
    {
        ReputeListParser::ReputeInfo* info = ReputeListParser::Instance()->GetReputeInfo(code);
        grade = ReputeListParser::GetReputeRating(info, point);
    }

    return grade;
}

void HonorSystem::ParseDGPacketDailyQuest( PVOID packet )
{
    MSG_DG_HONORSYSTEM_DAILYQUEST* recv_msg = static_cast<MSG_DG_HONORSYSTEM_DAILYQUEST*>(packet);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( recv_msg->m_dwKey );
    if( !player ) return ;

    switch(recv_msg->sub_type)
    {
        case DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK:
            {
                MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK* recv_msg2 = \
                    static_cast<MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK*>(packet);

                QuestManager_Concrete* quest_manager = player->GetQuestManager();
                quest_manager->SerializeDailyQuestStream( \
                    recv_msg2->daily_quest_info_, recv_msg2->count_, SERIALIZE_STORE);

                MSG_CG_QUEST_DAILYQUEST_INFO_ALL_ACK send_msg;
                quest_manager->SerializeDailyQuestStream( \
                    send_msg.daily_quest_data, send_msg.count_, SERIALIZE_LOAD);
                player->SendPacket(&send_msg, sizeof(send_msg));

                MSG_CG_QUEST_TODAYQUEST_INFO_CMD msg_today_quest;
                today_quests_.Serialize(msg_today_quest.today_quest_info, msg_today_quest.count_);
                player->SendPacket(&msg_today_quest, sizeof(msg_today_quest));

                ;{
                    // update dailyquest status
                    const int TODAY = 0, YESTERDAY = -1;
                    AchievementManager* achevement_manager = player->GetACManager();

                    if (!quest_manager->HaveDoneDailyQuest(TODAY) &&
                        !quest_manager->HaveDoneDailyQuest(YESTERDAY))
                    {
                        achevement_manager->ProcessDailyQuest(FALSE);
                    }
                };
                ;{
                    // update dailyquest count
                    WORD normalQ_count, dailyQ_count;
                    player->GetQuestManager()->GetCompleteQuestCount(normalQ_count, dailyQ_count);
                    player->GetACManager()->ProcessQuestCount(normalQ_count, dailyQ_count);
                };
            } break;
    }
}

void HonorSystem::Update()
{
    // CHANGES: f110831.2L, fixed custom version update interval problem.
    // NOTE: custom version problem issue :
    //  sample:
    //      interval = after 1minutes -> __time64_t v;
    //      the valid check "v + 60 < current", not "v + 60 * 1000 < current"
    if (update_timer_.IsExpired() == false) {
        return;
    };
    // WARNING: f110831.2L, foreign versions differ from Korean version
    // whether using of the solar auth system. it may occur a scripting loading time problem

    this->ChoiceTodayQuest();
}

void HonorSystem::ChoiceTodayQuest(bool force)
{
    // choice today's quest...
    if( today_quests_.UpdateTodayQuest(force) )
    {
        // let online users know today's quest...
        MSG_AG_EVENT_TODAYQUEST_INFO_BRD msg;
        today_quests_.Serialize(msg.today_quest_info, msg.count_);
        g_pGameServer->SendToServer(AGENT_SERVER, &msg, sizeof(msg));
    }
}

bool HonorSystem::IsTodayQuest( WORD group, WORD code )
{
    return today_quests_.IsTodayQuest(group, code);
}

#endif //_NA_003027_20111013_HONOR_SYSTEM