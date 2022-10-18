#include "stdafx.h"
#include "DBQueryProxyNode.h"

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include <VersionControl.h>

#include "QueryObjects/DBHandler.h"

//==================================================================================================

namespace nsQuery {
;

//==================================================================================================

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_Achievement_Select& query_result)
{
    DBCharacter_Achievements* const achievement_info = db_character_->GetAchievementInfo();    
	achievement_info->init();    

    const int result_row_count = query_result.GetResultRowNum();    

    for (int i = 0; i < result_row_count; ++i)
    {   
        const Query_Char_Achievement_Select::sQueryResult& result_record = \
            query_result.result_records_[i];
#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
        if (achievement_info->achievement_max_index_ == MAX_ACHIEVEMENT_COUNT)
            return true;

        DBCharacter_Achievement* const dest = \
            &achievement_info->achievement_info_[achievement_info->achievement_max_index_++];
#else
		if(MAX_ACHIEVEMENT_COUNT < result_record.index_number_ || result_record.index_number_ < 1)
		{
			ASSERT(!"업적 인덱스가 최소 최대값 범위에서 벗어 납니다.");
			continue;
		}

		if(result_record.index_number_ > achievement_info->achievement_max_index_)
		{
			achievement_info->achievement_max_index_ = result_record.index_number_;
		}

		DBCharacter_Achievement* const dest = \
						&achievement_info->achievement_info_[result_record.index_number_ - 1];
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
		
		dest->set_normal();
        dest->index_number_ = result_record.index_number_;
        dest->is_reward_ = result_record.is_reward_;        

        BOOST_STATIC_ASSERT(\
            sizeof(dest->object_value_[0]) == sizeof(result_record.object_value_[0]) &&
            sizeof(dest->object_value_) == sizeof(result_record.object_value_));
        //
        CopyMemory(dest->object_value_, result_record.object_value_, sizeof(dest->object_value_));

		
    }
    return true;
}


bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_Achievement_Update& query_result)
{
    const Query_Char_Achievement_Update::sQueryParam* parameter = &query_result.parameters_[0];

#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
    DBCharacter_Achievements* achievement_info = db_character_->GetAchievementInfo();

    DBCharacter_Achievement* achievement_elem = achievement_info->find(parameter->index_number_);
    if (achievement_elem != NULL)
        achievement_elem->unlock();
#else
    DBCharacter_Achievements* achievement_info = NULL;

    if(MAX_ACHIEVEMENT_COUNT < parameter->index_number_ || parameter->index_number_ < 1)
    {
        ASSERT(!"업적 인덱스가 최소 최대값 범위에서 벗어 납니다.");			
    }
    else
    {
        achievement_info = db_character_->GetAchievementInfo();
        achievement_info->achievement_info_[parameter->index_number_ - 1].unlock();
    }
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
    
    if (query_result.IsError() || !query_result.ResultSuccess())
    {
        if (query_result.IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                   query_result.GetQuery(), query_result.GetErrorMsg());

        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : ErrorCode[ %d ]", 
                   query_result.parameters_[0].result_);
        }
    }
	else
	{	
#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
        if (achievement_elem)
            achievement_elem->set_normal();
#else
        if(achievement_info)
        {
			if(parameter->index_number_ > achievement_info->achievement_max_index_)
			{
				achievement_info->achievement_max_index_ = parameter->index_number_;
			}

			achievement_info->achievement_info_[parameter->index_number_ - 1].set_normal();
		}
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
	}

    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateAchievement()
{
    const bool loaded_ac = (shared_.state & this->eState_Loaded_Achievement) != 0;
    if (loaded_ac == false) {
        return true;
    };
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // 강제..transaction success..
    }
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    DBCharacter_Achievements* achievement_info = db_character_->GetAchievementInfo();
    bool changed_events = false;
    
    STRING_SAFER_QUERY128 query_buffer;
#ifdef _NA_001990_ACSYSTEM_ADD
    query_buffer.MakeString("{?=call S_ACSystem_Update(?,?,?,?,?,?,?,?,?,?,?,?,?)}");
#else
    query_buffer.MakeString("{?=call S_ACSystem_Update(?,?,?,?,?,?,?,?,?,?,?)}");
#endif //_NA_001990_ACSYSTEM_ADD

    for (int i = 0; i < achievement_info->achievement_max_index_; ++i)
    {
		DBCharacter_Achievement& achievement = achievement_info->achievement_info_[i];
		if(achievement.is_modification() && !achievement.is_update_lock())
		{
			Query_Char_Achievement_Update* query = Query_Char_Achievement_Update::ALLOC();

			query->SetQuery(query_buffer);
			query->SetUserKey(db_user_->GetUserGuid());
			query->set_char_guid(db_character_->GetCharGuid());
			//
			Query_Char_Achievement_Update::sQueryParam* const parameter = &query->parameters_[0];

			parameter->index_number_ = achievement.index_number_;
			parameter->is_reward_ = achievement.is_reward_;
			parameter->character_guid_ = db_character_->GetCharGuid();

			BOOST_STATIC_ASSERT(\
				sizeof(parameter->object_value_[0]) == sizeof(achievement.object_value_[0]) &&
				sizeof(parameter->object_value_) == sizeof(achievement.object_value_));
			// CHANGES: f101112.5L, fixed a misusing macro, changes '_countof' to 'sizeof'
			CopyMemory(parameter->object_value_, achievement.object_value_,
				sizeof(parameter->object_value_));

			db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_ACHIEVEMENT_DBR, query);

            achievement.lock();
            changed_events = true;
            SUNLOG(eDEV_LOG, "achievement update charguid(%d) index(%d)",parameter->character_guid_,parameter->index_number_);
		}
    }
    //
    if (changed_events) {
        return false;
    };
    return true;
}
//==================================================================================================

}; //end of namespace

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

