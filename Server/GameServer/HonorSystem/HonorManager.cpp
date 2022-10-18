#include "StdAfx.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorManager.h"
#include <HonorSystem/HonorSystemPacket.h>
#include "AchievementManager.h"
#include "FameListParser.h"
#include "ReputeListParser.h"
#include "HonorSystem.h"
#include "QuestPacket.h"

//////////////////////////////////////////////////////////////////////////
//
HonorManager::HonorManager( void )
    : player_(NULL), playerinfo_(NULL)
{
    
}

HonorManager::~HonorManager( void )
{

}

bool HonorManager::init(Player* player)
{
    player_ = player;
    playerinfo_ = player->GetCharInfo();

    return true;
}

WORD HonorManager::GetHonorTitle()
{
    return playerinfo_->honor_title_;
}

bool HonorManager::SetHonorTitle( WORD honor_title )
{
    if(honor_title != 0)
    {
        TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(honor_title);
        if(!title_info)
            return false;

        HonorTitleMap::iterator it = honor_titles_.find(title_info->title_code);
        if(it == honor_titles_.end())
            return false;
    }

    playerinfo_->honor_title_ = honor_title;

    return true;
}

void HonorManager::RequestAddHonorTitle( WORD honor_title )
{
    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(honor_title);
    if(!title_info)
        return;

    WORD title_code = title_info->title_code;

    if (honor_titles_.size() >= MAX_HONORTITLE_COUNT ||
        honor_titles_.find(title_code) != honor_titles_.end())
    {
        PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
        send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_ADD_FAIL;
        player_->SendPacket(&send_msg, sizeof(send_msg));

        return;
    }

    MSG_DG_HONORTITLES_ADD_REMOVE_CMD msg;
    msg.sub_type = DG_HONORSYSTEM_TITLES_ADD_SYN;
    msg.honor_title_index = title_code;
    player_->SendToGameDBPServer(&msg, sizeof(msg));
}

bool HonorManager::AddHonorTitle( WORD honor_title, bool packet_send )
{
    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(honor_title);
    if(!title_info)
        return false;

    if(honor_titles_.size() >= MAX_HONORTITLE_COUNT)
        return false;

    WORD title_code = title_info->title_code;

    if( !(honor_titles_.insert(HonorTitleMap::value_type(title_code, title_info))).second )
    {
        PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
        send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_ADD_FAIL;
        player_->SendPacket(&send_msg, sizeof(send_msg));

        return false;
    }

    if(packet_send)
    {
        PACKET_CG_HONORSYSTEM_TITLE_ADD_ACK packet(title_code);
        player_->SendPacket(&packet, sizeof(packet));
    }

    return true;
}

void HonorManager::RequestRemoveHonorTitle( WORD honor_title )
{
    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(honor_title);
    if(!title_info)
        return;

    WORD title_code = title_info->title_code;

    if (honor_titles_.empty() ||
        honor_titles_.find(title_code) == honor_titles_.end())
    {
        PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
        send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_REMOVE_FAIL;
        player_->SendPacket(&send_msg, sizeof(send_msg));

        return;
    }

    MSG_DG_HONORTITLES_ADD_REMOVE_CMD msg;
    msg.sub_type = DG_HONORSYSTEM_TITLES_REMOVE_SYN;
    msg.honor_title_index = title_code;
    player_->SendToGameDBPServer(&msg, sizeof(msg));
}

bool HonorManager::RemoveHonorTitle( WORD honor_title, bool packet_send )
{
    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(honor_title);
    if(!title_info)
        return false;

    WORD title_code = title_info->title_code;

    HonorTitleMap::iterator it = honor_titles_.find(title_code);
    if(it == honor_titles_.end())
    {
        PACKET_CG_HONORSYSTEM_TITLE_NAK send_msg;
        send_msg.error_code_ = HONORSYSTEM_TITLE_ERROR_REMOVE_FAIL;
        player_->SendPacket(&send_msg, sizeof(send_msg));

        return false;
    }

    honor_titles_.erase(it);

    if( packet_send )
    {
        PACKET_CG_HONORSYSTEM_TITLE_REMOVE_ACK packet(title_info->title_code);
        player_->SendPacket(&packet, sizeof(packet));
    }

    return true;
}

void HonorManager::SerializeTitles( HonorTitleTable& titles_, eSERIALIZE serialize_type )
{
    if(serialize_type == SERIALIZE_LOAD)
    {
        for(HonorTitleMap::iterator it = honor_titles_.begin(); it != honor_titles_.end(); ++it)
        {
            titles_.title_[titles_.count_++].title_index_ = (it->second)->title_code;
        }
    }
    else if(serialize_type == SERIALIZE_STORE)
    {
        honor_titles_.clear();
        for(int i = 0; i < titles_.count_; ++i)
        {
            AddHonorTitle(titles_.title_[i].title_index_, false);
        }
    }
}

void HonorManager::SerializeFameRepute( HonorPointTable& points, eSERIALIZE serialize_type )
{
    if(serialize_type == SERIALIZE_LOAD)
    {
        for(HonorPointsMap::iterator it = honor_points_.begin(); it != honor_points_.end(); ++it)
        {
            memcpy(&points.point_table_[points.count_++], &(it->second), sizeof(HONOR_POINT_INFO));
        }
    }
    else if(serialize_type == SERIALIZE_STORE)
    {
        honor_points_.clear();
        for(int i = 0; i < points.count_; ++i)
        {
            HONOR_POINT_INFO& source = points.point_table_[i];

            SetFameRepute(source.type, source.honor_point_code_, source.honor_point_, false);
        }
    }
}

bool HonorManager::SetFameRepute( BYTE type, WORD code, DWORD point, bool packet_send )
{
    if( HONOR_POINT_INFO::FAME != type && HONOR_POINT_INFO::REPUTE != type)
        return false;

    HonorPointsKey key(type, code);

    HonorPointsMap::iterator it = honor_points_.find(key);
    if(it == honor_points_.end())
    {
        if(honor_points_.size() >= MAX_HONOR_POINT_INFO_COUNT)
            return false;

        HONOR_POINT_VALUE honor_point;
        honor_point.point_info.type = type;
        honor_point.point_info.honor_point_code_ = code;
        honor_point.point_info.honor_point_ = point;
        honor_point.grade = HonorSystem::Instance()->GetFameReputeGrade(type, code, point);

        if(!honor_points_.insert(HonorPointsMap::value_type(key, honor_point)).second)
        {
            return false;
        }
    }
    else
    {
        (it->second).point_info.honor_point_ = point;
        (it->second).grade = HonorSystem::Instance()->GetFameReputeGrade(type, code, point);
    }

    if(packet_send)
    {
        MSG_DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD dg_msg(type, code, point);
        player_->SendToGameDBPServer(&dg_msg, sizeof(dg_msg));

        PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD cg_msg(type, code, point);
        player_->SendPacket(&cg_msg, sizeof(cg_msg));
    }

    return true;
}

int HonorManager::GetFameReputePoint( BYTE type, WORD code )
{
    if( HONOR_POINT_INFO::FAME != type && HONOR_POINT_INFO::REPUTE != type)
        return 0;

    HonorPointsKey key(type, code);

    HonorPointsMap::iterator it = honor_points_.find(key);

    return (it == honor_points_.end()) ? 0 : (it->second).point_info.honor_point_;
}

int HonorManager::GetFameReputeGrade( BYTE type, WORD code )
{
    if( HONOR_POINT_INFO::FAME != type && HONOR_POINT_INFO::REPUTE != type)
        return 0;

    HonorPointsKey key(type, code);

    HonorPointsMap::iterator it = honor_points_.find(key);

    return (it == honor_points_.end()) ? 0 : (it->second).grade;
}

int HonorManager::GetFameReputeGradeCount( BYTE type, WORD grade )
{
    int count = 0;
    for(HonorPointsMap::iterator it = honor_points_.begin(); it != honor_points_.end(); ++it)
    {
        HONOR_POINT_VALUE& point_value = it->second;

        if(point_value.point_info.type != type)
            continue;

        if(point_value.grade < grade)
            continue;

        ++count;
    }

    return count;
}

void HonorManager::AddFamePoint(WORD code, WORD point)
{
    BYTE type = HONOR_POINT_INFO::FAME;
    int current_point = GetFameReputePoint(type, code);
    int current_grade = GetFameReputeGrade(type, code);

    if(SetFameRepute(type, code, current_point + point))
    {
        int grade = GetFameReputeGrade(type, code);
        if( grade > current_grade)
        {
            player_->GetACManager()->ProcessFameRepute(type, code);
        }
    }
}

void HonorManager::AddReputePoint(WORD code, WORD point)
{
    BYTE type = HONOR_POINT_INFO::REPUTE;
    int current_point = GetFameReputePoint(type, code);
    int current_grade = GetFameReputeGrade(type, code);

    if(SetFameRepute(type, code, current_point + point))
    {
        int grade = GetFameReputeGrade(type, code);
        if( grade > current_grade)
        {
            player_->GetACManager()->ProcessFameRepute(type, code); // AC process

            ReputeListParser::ReputeInfo* repute_info = \
                ReputeListParser::Instance()->GetReputeInfo(code);
            if( repute_info && 
                (grade <= ReputeListParser::kMaxReputeRating && grade >= 1))
            {
                WORD fame_code = repute_info->fame_area_code;
                WORD fame_point = repute_info->reward_fame_point[grade-1];
                AddFamePoint(fame_code, fame_point);
            }
        }
    }
}

void HonorManager::GMTodayQuestReset()
{
    HonorSystem::Instance()->ChoiceTodayQuest(true);
}

void HonorManager::GMDailyQuestClear()
{
    WORD count_;
    QUEST_DAILY_INFO daily_quest_data[MAX_DAILYQUEST_INFO_COUNT];

    QuestManager_Concrete* quest_manager = player_->GetQuestManager();
    quest_manager->SerializeDailyQuestStream(daily_quest_data, count_, SERIALIZE_LOAD);

    for(int i = 0; i < count_; ++i)
    {
        QUEST_DAILY_INFO& quest_info = daily_quest_data[i];

        quest_info.status_ = QS_COMPLETED;
        quest_info.start_datetime_ = 20110101000000;
        quest_info.end_datetime_   = 20110101000000;
        quest_manager->SetDailyQuestData(&quest_info);

        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        memcpy(&msg.quest_data, &quest_info, sizeof(QUEST_DAILY_INFO));
        player_->SendToGameDBPServer(&msg, sizeof(msg));

        MSG_CG_QUEST_DAILYQUEST_INFO_CMD packet;
        memcpy(&packet.daily_quest_info, &quest_info, sizeof(QUEST_DAILY_INFO));
        player_->SendPacket(&packet, sizeof(packet));
    }
}

#endif //_NA_003027_20111013_HONOR_SYSTEM

void HonorManager::DailyQuestReset()
{
    CTime current_time = util::TimeSync::_time64(NULL);
    CTime last_daily_init_time = CTime(current_time.GetYear(), 
        current_time.GetMonth(),
        current_time.GetDay(), 
        4,
        0,
        0);
    if (last_daily_init_time > current_time) {
        last_daily_init_time -= CTimeSpan(1, 0, 0, 0);
    }

    DWORD64 last_init_time = util::GetDateTime_YYYYMMDDHHMMSS(last_daily_init_time.GetTime());

    WORD count_;
    QUEST_DAILY_INFO daily_quest_data[MAX_DAILYQUEST_INFO_COUNT];

    QuestManager_Concrete* quest_manager = player_->GetQuestManager();
    quest_manager->SerializeDailyQuestStream(daily_quest_data, count_, SERIALIZE_LOAD);

    for(QUEST_DAILY_INFO* it = &daily_quest_data[0]; it != &daily_quest_data[count_]; ++it)
    {
        QUEST_DAILY_INFO& daily_info = (*it);

        if (daily_info.status_ != QS_COMPLETED)
            continue;

        if (daily_info.end_datetime_ < last_init_time)
            continue;

        // 오늘 완료한 퀘스트를 초기화 한다.

#ifdef _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
        daily_info.status_ = QS_NONE;
#else
        daily_info.status_ = QS_COMPLETED;
#endif //_NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
        daily_info.start_datetime_ = 20110101000000;
        daily_info.end_datetime_   = 20110101000000;
        quest_manager->SetDailyQuestData(&daily_info);

        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        memcpy(&msg.quest_data, &daily_info, sizeof(QUEST_DAILY_INFO));
        player_->SendToGameDBPServer(&msg, sizeof(msg));

        MSG_CG_QUEST_DAILYQUEST_INFO_CMD packet;
        memcpy(&packet.daily_quest_info, &daily_info, sizeof(QUEST_DAILY_INFO));
        player_->SendPacket(&packet, sizeof(packet));
    }
}

void HonorManager::SendDailyQuestResetUseTime()
{
    MSG_CG_QUEST_DAILYQUEST_RESET_ACK msg;
    msg.item_use_time = player_->GetExtraInfoManager().GetDailyQuestResetUseTime();
    player_->SendPacket(&msg, sizeof(msg));
}
