#include "StdAfx.h"
#include ".\AchievementSearch.h"

AchievementSearch::AchievementSearch(void)
{
	achievement_infos_.clear();
	achievement_cursetower_.clear();
}

AchievementSearch::~AchievementSearch(void)
{
	achievement_infos_.clear();
	achievement_cursetower_.clear();
}

void AchievementSearch::AddAchievementParserInfo(Achievement_ParserInfo* parser_info)
{
	for( int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{
		if(parser_info->object_type_ == ACHIEVEMENT_OBJECT_ACCOMPLISHMENT)
		{
            if(parser_info->obejcts_[i].object_number_ > 0)
            {                
                Achievement_SearchInfo search_info;
                search_info.index_ = i;
                search_info.parser_info_ = parser_info;

                ACHIEVEMENT_ACCOMPLISHMENT_RESULT result;

                DWORD accomplishment_key = MAKEDWORD(parser_info->location_code_,parser_info->obejcts_[i].object_number_);

                result = achievement_accomplishment_.insert(ACHIEVEMENT_ACCOMPLISHMENT_MAP::value_type(
                    accomplishment_key, 
                    search_info));

                if(!result.second)
                {
                    SUNLOG(eCRITICAL_LOG, "ACScript.txt 에 중복된 키가 존재 합니다. (index: %u, %u 번째 objectcode)",parser_info->index_, i+1);
                    ASSERT(!"ACScript.txt 데이터에 오류가 있습니다. 로그를 확인하세요");
                }
            }
		}
		else if(parser_info->object_type_ == ACHIEVEMENT_OBJECT_CURSETOWER)
		{
			if(parser_info->obejcts_[i].object_code_ > 0)
			{
				Achievement_SearchInfo search_info;
				search_info.index_ = i;
				search_info.parser_info_ = parser_info;
			
				achievement_cursetower_.push_back(search_info);
			}			
		}
#ifdef _NA_001990_ACSYSTEM_ADD
        else if(parser_info->object_type_ == ACHIEVEMENT_OBJECT_ITEM ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_QUEST ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_ITEM_GET ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_ITEM_EQUIP ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_PC_KILL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_PC_KILL_ACCUMULATED ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_NPC_KILL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_NPC_KILL_ACCUMULATED ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_KILL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_KILL_ACCUMULATED ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_WIN ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_FALL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_LEVELUP ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_AC_CLEARE ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_HEAL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TOP ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TO_MONSTER ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_BY_MONSTER ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_KILL ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLE_DEAD ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_SKILL_STEP ||
                parser_info->object_type_ == ACHIEVEMENT_OBJECT_PERK_LEVEL
#ifdef _NA_003027_20111013_HONOR_SYSTEM
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_FAME
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_REPUTE
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_FAME_COUNT
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_REPUTE_COUNT
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_COMPLETE_NORMALQUEST
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST_CONTINOUS
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_KILL_ALL
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_DEAD_ALL
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_LOTTO
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_ITEM_SCORE
#endif // _NA_002935_20110704_ITEM_SCORE
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_ENCHANT
#ifdef _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLEGROUND_STRONGPOINT_ATTACK
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLEGROUND_WIN
                || parser_info->object_type_ == ACHIEVEMENT_OBJECT_BATTLEGROUND_STRONGPOINT_DEFENSE
#endif
                )
        {
            if((parser_info->obejcts_[i].object_code_ == 0 && parser_info->obejcts_[i].object_number_ > 0) ||
                parser_info->obejcts_[i].object_code_ != 0 && parser_info->obejcts_[i].object_number_ == 0 )
            {
                SUNLOG(eCRITICAL_LOG, "ACScript.txt에 키값에 오류가 존재 합니다. (index: %u, %u 번째 objectcode)",
                    parser_info->index_, i+1);
                ASSERT(!"ACScript.txt 데이터에 오류가 있습니다. (code == 0 and number > 0)");
                return;
            }

            if(/*parser_info->obejcts_[i].object_code_ == 0 ||*/ parser_info->obejcts_[i].object_number_ == 0)
                return;

            ACHIEVEMENT_OBJECT_TYPE object_type = (ACHIEVEMENT_OBJECT_TYPE)parser_info->object_type_;
            ACHIEVEMENT_INFO_RACK_OF_VECTOR::iterator iter = achievement_info_rack_of_vector_.find(object_type);

            if(iter == achievement_info_rack_of_vector_.end())
            {
                std::pair<ACHIEVEMENT_INFO_RACK_OF_VECTOR::iterator, bool> pr = \
                    achievement_info_rack_of_vector_.insert( \
                        ACHIEVEMENT_INFO_RACK_OF_VECTOR::value_type( \
                            object_type, ACHIEVEMENT_INFO_MAP_OF_VECTOR()));

                if(!pr.second)
                {
                    SUNLOG(eCRITICAL_LOG, "ACScript.txt 에 중복된 키가 존재 합니다. (index: %u, %u 번째 objectcode)",
                        parser_info->index_, i+1);
                    ASSERT(!"ACScript.txt 데이터에 오류가 있습니다. 로그를 확인하세요");   
                    
                    return;
                }

                iter = pr.first;
            }

            WORD object_code = parser_info->obejcts_[i].object_code_;
            ACHIEVEMENT_INFO_MAP_OF_VECTOR::iterator iter2 = (iter->second).find(object_code);
            if(iter2 == (iter->second).end())
            {
                std::pair<ACHIEVEMENT_INFO_MAP_OF_VECTOR::iterator, bool> pr2 = \
                    (iter->second).insert( \
                        ACHIEVEMENT_INFO_MAP_OF_VECTOR::value_type( \
                            object_code, ACHIEVEMENT_INFO_VECTOR()));

                if(!pr2.second)
                {
                    SUNLOG(eCRITICAL_LOG, "ACScript.txt 에 중복된 키가 존재 합니다. (index: %u, %u 번째 objectcode)",
                        parser_info->index_, i+1);
                    ASSERT(!"ACScript.txt 데이터에 오류가 있습니다. 로그를 확인하세요");   

                    return;
                }

                iter2 = pr2.first;
            }

            Achievement_SearchInfo search_info;
            search_info.index_ = i;
            search_info.parser_info_ = parser_info;

            (iter2->second).push_back(search_info);
        }
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        else if(parser_info->object_type_ >= ACHIEVEMENT_OBJECT_MAX)
        {
        }
#endif //_NA_003027_20111013_HONOR_SYSTEM
		else
		{
			if(parser_info->obejcts_[i].object_code_ > 0)
			{
				Achievement_SearchInfo search_info;
				search_info.index_ = i;
				search_info.parser_info_ = parser_info;

				ACHIEVEMENT_INFO_RESULT result;

				result = achievement_infos_.insert(ACHIEVEMENT_INFO_MAP::value_type(
					parser_info->obejcts_[i].object_code_, 
					search_info));

				if(!result.second)
				{
                    SUNLOG(eCRITICAL_LOG, "ACScript.txt 에 중복된 키가 존재 합니다. (index: %u, %u 번째 objectcode)",
                        parser_info->index_, i+1);
                    ASSERT(!"ACScript.txt 데이터에 오류가 있습니다. 로그를 확인하세요");
				}
			}			
		}
	}
}

void AchievementSearch::SetFirst()
{
	achievement_cursetower_iter_ = achievement_cursetower_.begin();    
}

bool AchievementSearch::GetNext(Achievement_SearchInfo** search_info)
{
	if(achievement_cursetower_iter_ == achievement_cursetower_.end())
		return false;

	*search_info = (Achievement_SearchInfo*)(&(*achievement_cursetower_iter_));

    ++achievement_cursetower_iter_;

	return true;
}

Achievement_SearchInfo* AchievementSearch::GetAchievement_SearchInfo(WORD object_key)
{
	ACHIEVEMENT_INFO_ITER iter = achievement_infos_.find(object_key);
	if(iter == achievement_infos_.end())
		return NULL;

	return (Achievement_SearchInfo*)&iter->second;
}

Achievement_SearchInfo* AchievementSearch::GetAccomplishment_SearchInfo(DWORD object_key)
{
    ACHIEVEMENT_ACCOMPLISHMENT_ITER iter = achievement_accomplishment_.find(object_key);
    if(iter == achievement_accomplishment_.end())
        return NULL;

    return (Achievement_SearchInfo*)&iter->second;
}

#ifdef _NA_001990_ACSYSTEM_ADD
AchievementSearch::ACMapElem* AchievementSearch::GetSearchInfo_find( ACHIEVEMENT_OBJECT_TYPE object_type )
{
    ACHIEVEMENT_INFO_RACK_OF_VECTOR::iterator iter = achievement_info_rack_of_vector_.find(object_type);
    if( iter == achievement_info_rack_of_vector_.end() )
        return NULL;

    return &iter->second;
}

AchievementSearch::ACVectorElem* AchievementSearch::GetSearchInfoVector_find( ACHIEVEMENT_OBJECT_TYPE object_type, WORD object_key )
{
    ACHIEVEMENT_INFO_RACK_OF_VECTOR::iterator iter = achievement_info_rack_of_vector_.find(object_type);
    if( iter == achievement_info_rack_of_vector_.end() )
        return NULL;

    ACHIEVEMENT_INFO_MAP_OF_VECTOR::iterator iter2 = (iter->second).find(object_key);
    if( iter2 == (iter->second).end() )
        return NULL;

    return &iter2->second;
}
#endif //_NA_001990_ACSYSTEM_ADD