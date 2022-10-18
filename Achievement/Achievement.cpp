#include "StdAfx.h"
#include ".\Achievement.h"
#include <algorithm>

//==================================================================================================
// CHANGES: f110526.5L, changes to shared pool each AchievementInfo
namespace util {

struct AchievementPartPool
{
private:
    AchievementPartPool()
    {
        part_pool_.Initialize(10, 10);
    }
    ~AchievementPartPool()
    {
        part_pool_.Release();
    }
    
public:
    CMemoryPoolFactory<AchievementPart> part_pool_;

    static AchievementPartPool* Instance()
    {
        static AchievementPartPool instance;
        return &instance;
    }
}; // AchievementPartPool

} // util

AchievementPart* Achievement::AllocPart()
{
    return reinterpret_cast<AchievementPart*>(
        util::AchievementPartPool::Instance()->part_pool_.Alloc());
}

BOOL Achievement::FreePart(AchievementPart* delete_pointer)
{
    return util::AchievementPartPool::Instance()->part_pool_.Free(delete_pointer);
};

//==================================================================================================

Achievement::Achievement(void) : notify_(NULL)
{
	achievement_parser_ = AchievementParser::Instance();
}

Achievement::~Achievement(void)
{    
    Init();
}

void Achievement::Init(AchievementNotify* notify)
{		
    notify_ = notify;        

    ACHIEVEMENT_PART_ITER iter = Begin();
    for(; iter != End() ; ++iter)
    {
        iter->second->Init(NULL);
        FreePart(iter->second);
    }

    achievement_part_.clear();    
}

AchievementPart* Achievement::GetAchievementPart(WORD location_code)
{
	ACHIEVEMENT_PART_ITER part_iter = achievement_part_.find(location_code);
	if(part_iter == End())
	{
		AchievementPart* achievement_part = AllocPart();
		achievement_part->Init(notify_);
		achievement_part->SetLocationCode(location_code);
		ACHIEVEMENT_PART_RESULT part_result;

		part_result = achievement_part_.insert(ACHIEVEMENT_PART_MAP::value_type(
			location_code, achievement_part));

		if(!part_result.second) return NULL;

		return achievement_part;
	}

	return part_iter->second;
}

bool Achievement::AddAchievement(Achievement_ParserInfo* achievement_parser_info)
{
	AchievementPart* achievement_part = GetAchievementPart(achievement_parser_info->location_code_);
	if(achievement_part == NULL) return false;
	return achievement_part->AddAchievement(achievement_parser_info) == NULL ? false : true;
}

void Achievement::AddAllAchievement()
{
	ACHIEVEMENTINFO_ITERATOR achievement_info = achievement_parser_->Begin();

	for(; achievement_info != achievement_parser_->End() ; ++achievement_info)
	{
		AddAchievement((*achievement_info));
	}
}

bool Achievement::SetAchievementValue(WORD location_code, WORD index_number,
									  BYTE object_index, WORD object_value,
									  ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	AchievementPart* achievement_part = GetAchievementPart(location_code);
	if(achievement_part == NULL) return false;
	return achievement_part->SetAchievementValue(index_number, object_index,
		object_value, type);
}

bool Achievement::SetAchievementValue(Achievement_ParserInfo* achievement_parser_info,
									  BYTE index, WORD value,
									  ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	AchievementPart* achievement_part = GetAchievementPart(achievement_parser_info->location_code_);
	if(achievement_part == NULL) return false;
	return achievement_part->SetAchievementValue(achievement_parser_info->index_, index,
												 value, type);
}

bool Achievement::SetAchievementValue(Character_Achievement& char_achievement_info)
{	
	Achievement_ParserInfo* achievement_parser = 
		achievement_parser_->GetAchievement_ParserInfo(char_achievement_info.index_number_);
	if(achievement_parser == NULL)
	{
		return false;
	}

	AchievementPart* achievement_part = GetAchievementPart(achievement_parser->location_code_);
	if(achievement_part == NULL) return false;
	return achievement_part->SetAchievementValue(char_achievement_info, ACHIEVEMENT_VALUE_INIT);
}

void Achievement::InitAccomplishmentRatio()
{
	ACHIEVEMENT_PART_ITER iter = Begin();
	for(; iter != End() ; ++iter)
	{
		iter->second->InitAccomplishmentRatio();
	}
}

void Achievement::Serialize( Character_Achievements& achievement_info, ACHIEVEMENT_SERIALIZE eType )
{
	if(eType == ACHIEVEMENT_SAVE)
	{
		Init(notify_);	

		AddAllAchievement();
#if !defined(_SERVER) && defined(_SCJ_ACSYSTEM_OVERLAP_PARENT)
        SetAchievementParentMap();
#endif //#if !defined(_SERVER) && defined(_SCJ_ACSYSTEM_OVERLAP_PARENT)

#ifdef _SERVER
        AddTreasureConditionInfo();
#endif

		for(int i = 0  ; i < achievement_info.achievement_count_ ; ++i)
		{		
			SetAchievementValue(achievement_info.achievement_info_[i]);			
		}
		InitAccomplishmentRatio();
	}
	else//ACSERIALIZE_CLIENT_LOAD ACSERIALIZE_CLIENT_LOAD
	{
#ifndef _SERVER
		ASSERT(FALSE);		
#endif	
		achievement_info.achievement_count_ = 0;

		ACHIEVEMENT_PART_ITER iter = Begin();		

		for(; iter != End() ; ++iter)
		{
			iter->second->LoadAchievement(achievement_info.achievement_count_, achievement_info, eType);			
		}
	}
}

bool Achievement::InitAchievementPart(WORD location_code)
{
	AchievementPart* achievement_part = GetAchievementPart(location_code);
	if(achievement_part == NULL) return false;
	achievement_part->InitAchievement();
	return true;
}

bool Achievement::IsAccomplishment(WORD location_code, WORD index_number)
{
	AchievementPart* achievement_part = GetAchievementPart(location_code);
	if(achievement_part == NULL) return false;
	return achievement_part->IsAccomplishment(index_number);
}

bool Achievement::IsAccomplishment(Achievement_ParserInfo* achievement_parser_info)
{
	AchievementPart* achievement_part = GetAchievementPart(achievement_parser_info->location_code_);
	if(achievement_part == NULL) return false;
	return achievement_part->IsAccomplishment(achievement_parser_info->index_);
}
#ifdef _NA_001990_ACSYSTEM_ADD
bool Achievement::IsAccomplishmentByIndex(WORD index)
{
    Achievement_ParserInfo* achievement_parserinfo;
    achievement_parserinfo = achievement_parser_->GetAchievement_ParserInfo(index);

    if (achievement_parserinfo != NULL && IsAccomplishment(achievement_parserinfo))
    {
        return true;
    }
    return false;

}
#endif //_NA_001990_ACSYSTEM_ADD
bool Achievement::RewardAchievement(WORD index_number, WORD location_code,bool is_reward)
{
	AchievementPart* achievement_part = GetAchievementPart(location_code);
	if(achievement_part == NULL) return false;
	return achievement_part->RewardAchievement(index_number, is_reward);	
}

bool Achievement::Force_RewardAchievement(WORD index_number, WORD location_code,bool is_reward)
{
    AchievementPart* achievement_part = GetAchievementPart(location_code);
    if(achievement_part == NULL) return false;
    return achievement_part->Force_RewardAchievement(index_number, is_reward);	
}

WORD Achievement::AddAccomplishmentRation(WORD location_code, WORD accomplishment_ration)
{
	AchievementPart* achievement_part = GetAchievementPart(location_code);
	if(achievement_part == NULL) return false;
	return achievement_part->AddAccomplishmentRation(accomplishment_ration);	
}

void Achievement::AccomplishAchievement()
{
    ACHIEVEMENT_PART_ITER iter = Begin();
    for(; iter != End() ; ++iter)
    {
        iter->second->AccomplishAchievement();
    }
}

#ifdef _SERVER
bool Achievement::CanCollectTreasureBox(WORD location_code)
{
    AchievementPart* achievement_part = GetAchievementPart(location_code);
    if(achievement_part == NULL) return false;
    return achievement_part->CanCollectTreasureBox();	
}

void Achievement:: AddTreasureConditionInfo()
{
    ACHIEVEMENT_PART_ITER iter = Begin();
    for(; iter != End() ; ++iter)
    {
        AchievementPart* achievement_part = iter->second;

        Collect_Condition_Info* collect_condition_info = 
            CollectTreasureBoxInfo::Instance()->GetCollectCondition(achievement_part->GetLocationCode());

        if(collect_condition_info)
        {
            achievement_part->SetTreasureBoxConditionInfo(collect_condition_info);
        }        
    }
}
#endif

#if !defined(_SERVER)
//_SCJ_ACSYSTEM_OVERLAP_PARENT
AchievementInfo* Achievement::FindAchievementToIndex(WORD index_number)
{
    Achievement_ParserInfo* achievement_parser =
        achievement_parser_->GetAchievement_ParserInfo(index_number);
    if(achievement_parser == NULL)
    {
        return 0;
    }

    AchievementPart* achievement_part = GetAchievementPart(achievement_parser->location_code_);
    if(achievement_part == NULL)
    {
        return 0;
    }

    return achievement_part->FindAchievementInfoToIndex(index_number);

}

void Achievement::SetAchievementParentMap()
{
    // 선행코드 리스트를 셋팅한다.
    ACHIEVEMENT_PART_ITER part_iterator = Begin();
    for(; part_iterator != End() ; ++part_iterator)
    {
        AchievementPart* achievement_part = part_iterator->second;
        if (achievement_part)
        {
            achievement_part->SetAchievementPartParentMap(this);
        }
    }

}
#endif //!defined(_SERVER)


