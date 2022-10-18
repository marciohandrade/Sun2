#include "stdafx.h"
#include "./AchievementPart.h"

//==================================================================================================
// CHANGES: f110526.5L, changes to shared pool each AchievementInfo
namespace util {
;
struct AchievementInfoPool
{
public:
    static AchievementInfoPool* Instance();
    util::CMemoryPoolFactory<AchievementInfo> info_pool_;
    //
private:
    AchievementInfoPool()
    {
        info_pool_.Initialize(20, 20);
    };
    ~AchievementInfoPool()
    {
        info_pool_.Release();
    }
    static AchievementInfoPool* CreateInstance();
    //
    static AchievementInfoPool* instance_static_;
    __DISABLE_COPY(AchievementInfoPool);
};

};

util::AchievementInfoPool* util::AchievementInfoPool::instance_static_ = NULL;

util::AchievementInfoPool* util::AchievementInfoPool::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(AchievementInfoPool** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        AchievementInfoPool** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    AchievementInfoPool* provider = new AchievementInfoPool;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

inline util::AchievementInfoPool* util::AchievementInfoPool::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

AchievementInfo* AchievementPart::AllocInfo()
{
    return reinterpret_cast<AchievementInfo*>(
        util::AchievementInfoPool::Instance()->info_pool_.Alloc());
}

BOOL AchievementPart::FreeInfo(AchievementInfo* delete_pointer)
{
    return util::AchievementInfoPool::Instance()->info_pool_.Free(delete_pointer);
};

//==================================================================================================

AchievementPart::AchievementPart(void) : notify_(NULL)
{
	parser_ = AchievementParser::Instance();
}

AchievementPart::~AchievementPart(void)
{
    ratios_.clear();
    infos_.clear();    
}

void AchievementPart::Init(AchievementNotify* notify)
{
	notify_ = notify;
	ratio_sum_ = 0;
    show_ratio_ = false;

    ACHIEVEMENT_ITER iter = Begin();
    for(; iter != End() ; ++iter)
    {        
        iter->second->SetNotifyObject(NULL);
        FreeInfo(iter->second);
    }

    infos_.clear();
    ratios_.clear();    

#ifdef _SERVER
    collect_condition_info_.can_collect_treasurebox_ = false;
    BOOST_STATIC_ASSERT(_countof(collect_condition_info_.is_accomplishment_) ==
                        _countof(collect_condition_info_.condition_) &&
                        _countof(collect_condition_info_.condition_) ==
                        Collect_Condition_Accomplishment_Info::MAX_COLLECT_CONDITION);
    for(int i = 0 ; i < Collect_Condition_Accomplishment_Info::MAX_COLLECT_CONDITION ; ++i)
    {
        collect_condition_info_.is_accomplishment_[i] = false;
        collect_condition_info_.condition_[i] = 0;
    }
#endif
}

AchievementInfo* AchievementPart::AddAchievement(WORD index_number)
{
	Achievement_ParserInfo* achievement_parser = 
		parser_->GetAchievement_ParserInfo(index_number);
	if(achievement_parser == NULL)
	{
		return NULL;
		//SUNLOG(eCRITICAL_LOG, "Not Find AchievementParser %u\n", index_number);
	}

	return AddAchievement(achievement_parser);
}

void AchievementPart::InitAchievement()
{
	ACHIEVEMENT_ITER iter = Begin();
	for(; iter != End() ; ++iter)
	{
		iter->second->InitValue();
	}
}

AchievementInfo* AchievementPart::AddAchievement(Achievement_ParserInfo* achievement_parser_info)
{
	AchievementInfo* achievement_info = AllocInfo();

	achievement_info->Init(achievement_parser_info, notify_);

	ACHIEVEMENT_RESULT result;

	result = infos_.insert(ACHIEVEMENT_MAP::value_type(
		achievement_info->GetIndex(), achievement_info));

	if(!result.second) return NULL;

	if(achievement_parser_info->object_type_ == ACHIEVEMENT_OBJECT_ACCOMPLISHMENT) //달성률 업적
	{
		ACHIEVEMENT_RESULT result_ptr = ratios_.insert(ACHIEVEMENT_MAP::value_type(
			achievement_info->GetIndex(), achievement_info));
	}		
        
    if(achievement_parser_info->object_type_ == ACHIEVEMENT_OBJECT_ACCOMPLISHMENT ||            //달성률 모으는 업적이거나
        achievement_parser_info->rewards_[0].reward_type_ == ACHIEVEMENT_REWARD_ACCOMPLISHMENT) //달성률 보상이면
    {
        show_ratio_ = true;
    }

    return achievement_info;
}

AchievementInfo* AchievementPart::GetAchievement(WORD index_number)
{
	ACHIEVEMENT_ITER iter = infos_.find(index_number);

	if(iter == infos_.end())
	{
		return AddAchievement(index_number);
	}

	return iter->second;
}

bool AchievementPart::SetAchievementValue(WORD index, BYTE object_index, 
										  WORD value , ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	AchievementInfo* achievement_info = GetAchievement(index);
	if(!achievement_info) return false;

	if(achievement_info->SetValue(object_index, value, type))
    {
#ifdef _SERVER
        CheckTreasureBoxCondition(achievement_info, type);
#endif
    }
    
	return true;
}

bool AchievementPart::SetAchievementValue(Character_Achievement& char_achievement_info,
										  ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	AchievementInfo* achievement_info = GetAchievement(char_achievement_info.index_number_);
	if(!achievement_info) return false;

	achievement_info->SetValue(char_achievement_info.object_value_, type);
	achievement_info->SetReward(char_achievement_info.is_reward_, type);
#ifdef _SERVER
    CheckTreasureBoxCondition(achievement_info, type);
#endif

	return true;
}

void AchievementPart::InitAccomplishmentRatio()
{
	ACHIEVEMENT_ITER iter = Begin();
	for(; iter != End() ; ++iter)
	{
		if(iter->second->IsAccomplishment() && iter->second->IsReward())
		{
			ratio_sum_ += iter->second->GetAccomplishmentRatio();
		}
	}
	InitAccomplishmentAchievement(ACHIEVEMENT_VALUE_INIT);
}

void AchievementPart::LoadAchievement(WORD& index, Character_Achievements& achievement_info, ACHIEVEMENT_SERIALIZE eType)
{
	ACHIEVEMENT_ITER iter = Begin();
	for(; iter != End() ; ++iter)
	{
#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
        if(index == MAX_ACHIEVEMENT_COUNT)
            return;
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX

		if(eType == ACSERIALIZE_CLIENT_LOAD)
		{
			if(iter->second->IsEmpty())
			{
				continue;
			}
		}
		else if(eType == ACSERIALIZE_DBPROXY_LOAD)
		{
			if(!iter->second->IsModification())
			{
				continue;
			}
		}	
		else
		{
			continue;
		}
		achievement_info.achievement_info_[index].is_reward_ = iter->second->IsReward();
		achievement_info.achievement_info_[index].index_number_ = iter->second->GetIndex();
		iter->second->GetValue(achievement_info.achievement_info_[index].object_value_);
		index++;
	}
}

bool AchievementPart::IsAccomplishment(WORD index)
{
	AchievementInfo* achievement_info = GetAchievement(index);
	if(!achievement_info) return false;
	return achievement_info->IsAccomplishment();
}

bool AchievementPart::RewardAchievement(WORD index_number, bool is_reward)
{
	AchievementInfo* achievement_info = GetAchievement(index_number);
	if(!achievement_info) return false;

    //보상 해도 되는지 확인한다.
    if(achievement_info->GetObjectType() == ACHIEVEMENT_OBJECT_ACCOMPLISHMENT)
    {
#ifdef _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK
        AchievementObjectInfo ac_info = achievement_info->GetObjectInfo(0);
        if(ac_info.Achievement_ > ratio_sum_)
            return false;
#else
        if(achievement_info->GetObjectInfo(0).value_ > ratio_sum_)
            return false;
#endif //_NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK

    }
    else
    {
        if(!achievement_info->IsAccomplishment())
            return false;
    }

	achievement_info->SetReward(is_reward, ACHIEVEMENT_VALUE_SET);
	return true;
}

bool AchievementPart::Force_RewardAchievement(WORD index_number, bool is_reward)
{
    AchievementInfo* achievement_info = GetAchievement(index_number);
    if(!achievement_info) return false;

    achievement_info->SetReward(is_reward, ACHIEVEMENT_VALUE_SET);
    return true;
}

WORD AchievementPart::AddAccomplishmentRation(WORD accomplishment_ration)
{
	ratio_sum_ += accomplishment_ration;
	InitAccomplishmentAchievement(ACHIEVEMENT_VALUE_SET);
	return ratio_sum_;
}

void AchievementPart::InitAccomplishmentAchievement(ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	ACHIEVEMENT_ITER iter = ratios_.begin();
	for(; iter != ratios_.end() ; ++iter)
	{
		if(iter->second->GetObjectInfo(0).Achievement_ <= ratio_sum_)
		{
			iter->second->SetValue(0, 
								   iter->second->GetObjectInfo(0).Achievement_,
								   type);
			//달성률 업적 달성
		}
	}	
}

void AchievementPart::AccomplishAchievement()
{
    ACHIEVEMENT_ITER iter = Begin();

    for(; iter != End() ; ++iter)
    {
        if(iter->second->IsAccomplishmentReward() && iter->second->IsAccomplishment())
        {
            iter->second->SetReward(true, ACHIEVEMENT_VALUE_SET);           
        }        
    }    
}

#ifdef _SERVER
void AchievementPart::CheckTreasureBoxCondition(AchievementInfo* achievement_info, ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
    if(collect_condition_info_.can_collect_treasurebox_) //이미 보물상자 채취가 가능한 상태이면 비교할 필요 없다.
        return;

    for(int i = 0 ; i < Collect_Condition_Accomplishment_Info::MAX_COLLECT_CONDITION ; ++ i)
    {
        if(collect_condition_info_.is_accomplishment_[i]) 
            continue;

        if(collect_condition_info_.condition_[i] == achievement_info->GetIndex())
        {
            if(achievement_info->IsAccomplishment()) //해당 업적이 달성 되었나 ?
            {
                collect_condition_info_.is_accomplishment_[i] = true;
                collect_condition_info_.CalcConditionAccomplish();

                if(type != ACHIEVEMENT_VALUE_INIT && collect_condition_info_.can_collect_treasurebox_)
                {
                    //보물상자 채취 상태로 변경되어 통보 한다.
                    notify_->OnCanCollectTreasureBox();
                }
                break;
            }
        }
    }    
}

void AchievementPart::SetTreasureBoxConditionInfo(Collect_Condition_Info* collect_condition_info)
{
    for(int i = 0 ; i < Collect_Condition_Info::MAX_COLLECT_CONDITION ; ++i)
    {
        collect_condition_info_.condition_[i] = collect_condition_info->condition_[i];
    }
}
#endif

#if !defined(_SERVER)
//_SCJ_ACSYSTEM_OVERLAP_PARENT
void AchievementPart::SetAchievementPartParentMap(Achievement* achievement)
{
    ACHIEVEMENT_ITER iter = Begin();

    for(; iter != End() ; ++iter)
    {
        iter->second->SetAchievementInfoParentMap(achievement);
    }
}
AchievementInfo* AchievementPart::FindAchievementInfoToIndex(WORD index_number)
{
    ACHIEVEMENT_ITER iter = infos_.find(index_number);

    if(iter == infos_.end())
    {
        return 0;
    }

    return iter->second;
}
#endif //!defined(_SERVER)

