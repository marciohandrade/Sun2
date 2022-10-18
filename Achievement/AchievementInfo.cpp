#include "StdAfx.h"
#include ".\AchievementInfo.h"
#include "Achievement.h"

AchievementInfo::AchievementInfo() : notify_(NULL) , status_(EMPTY), is_reward_(false)
{
	
}

AchievementInfo::~AchievementInfo(void)
{
}

bool AchievementInfo::IsAccomplishment() const
{
	for(int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{
#ifdef _NA_001990_ACSYSTEM_ADD
        //미션 클리어 타임 ac는 설정된 값보다 작으면 성공이다
        if (object_type_ == ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME)
        {
            if (obejct_value_[i].Achievement_ != 0)
            {
                if ((obejct_value_[i].Achievement_ < obejct_value_[i].value_) || 
                    (obejct_value_[i].value_ == 0))
                {
                    return false;
                }
            }
        }
        else
#endif //_NA_001990_ACSYSTEM_ADD
		if (obejct_value_[i].Achievement_ > obejct_value_[i].value_)
        {
			return false;
        }
	}
	return true;
}

bool AchievementInfo::IsAccomplishment(BYTE object_index)
{
#ifdef _NA_001990_ACSYSTEM_ADD
    //미션 클리어 타임은 설정된 값보다 작으면 성공이다
    if (object_type_ == ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME)
    {
        if (obejct_value_[object_index].Achievement_ != 0)
        {
            if ((obejct_value_[object_index].Achievement_ < obejct_value_[object_index].value_) || 
                (obejct_value_[object_index].value_ == 0))
            {
                return false;
            }
        }
    }
    else
#endif //_NA_001990_ACSYSTEM_ADD
	if (obejct_value_[object_index].Achievement_ > obejct_value_[object_index].value_)
    {
		return false;
    }
	return true;
}

WORD AchievementInfo::GetAccomplishmentRatio()
{	
	WORD accomplishment_ratio = 0;
	for(int i = 0 ; i < MAX_ACHIEVEMENT_REWARD ; ++i)
	{
		if(rewards_[i].reward_type_ == ACHIEVEMENT_REWARD_ACCOMPLISHMENT)
		{
			if(IsAccomplishment(0))
			{
				accomplishment_ratio += rewards_[i].reward_code_;
			}			
		}		
	}
	return accomplishment_ratio;
}

bool AchievementInfo::IsAccomplishmentReward()
{	
	for(int i = 0 ; i < MAX_ACHIEVEMENT_REWARD ; ++i)
	{
		if(rewards_[i].reward_type_ == ACHIEVEMENT_REWARD_ACCOMPLISHMENT)
			return true;
	}
	return false;
}

void AchievementInfo::InitValue()
{		
	for(int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{	
		SetValue(i,0,ACHIEVEMENT_VALUE_SET);
	}
	SetReward(false, ACHIEVEMENT_VALUE_SET);
}

void AchievementInfo::Init(Achievement_ParserInfo* achievement_parser_info, AchievementNotify* notify)
{
    status_ = EMPTY; // CHANGES: f110613.3L
    notify_                 = notify;
	index_number_			= achievement_parser_info->index_;		
	is_reward_				= false;
	location_code_			= achievement_parser_info->location_code_;
    object_type_	        = achievement_parser_info->object_type_;
#ifdef _NA_001990_ACSYSTEM_ADD
    class_type_             = achievement_parser_info->class_type_;
    for(int i = 0 ; i < MAX_ACHIEVEMENT_PARENT_CODE ; ++i)
    {
        parent_ac_code_[i] = achievement_parser_info->parent_ac_code_[i];
    }    
#endif //_NA_001990_ACSYSTEM_ADD
#if !defined(_SERVER)
	object_string_code_	= achievement_parser_info->object_string_code_;
	tooltip_string_code_	= achievement_parser_info->tooltip_string_code_;
	icon_code_				= achievement_parser_info->icon_code_;
    parent_ac_map_.clear();
    child_ac_map_.clear();
#endif

	for(int i = 0 ; i < MAX_ACHIEVEMENT_REWARD ; ++i)
	{
		rewards_[i].reward_type_ = achievement_parser_info->rewards_[i].reward_type_;
		rewards_[i].reward_code_ = achievement_parser_info->rewards_[i].reward_code_;
	}

	for(int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{
		obejct_value_[i].Achievement_ = achievement_parser_info->obejcts_[i].object_number_;
		obejct_value_[i].value_ = 0;	
	}
}

bool AchievementInfo::SetValue(BYTE object_index, WORD object_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{	
    if(type == ACHIEVEMENT_VALUE_APPEND)
    {
        object_value += obejct_value_[object_index].value_;
    }		

    //값이 달성수치보다 큰가 ?        
    if ( 
#ifdef _NA_001990_ACSYSTEM_ADD
        ( object_type_ != ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME ) &&
#endif //_NA_001990_ACSYSTEM_ADD
        object_value > obejct_value_[object_index].Achievement_ )
    {
        object_value = obejct_value_[object_index].Achievement_;			
    }    

    if(obejct_value_[object_index].value_ != object_value)
    {
        if(type != ACHIEVEMENT_VALUE_INIT)
        {
            NotifyChangedValue(object_index, obejct_value_[object_index].value_, object_value);
        }
        else
        {
            if(status_ == EMPTY)
            {
                status_ = NORMAL;
            }
        }

        obejct_value_[object_index].value_ = object_value;
        //값 변경이 있으면 true 를 리턴한다.
        return true;
    }
    return false;
}

void AchievementInfo::SetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT], ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	for(BYTE i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{
		SetValue(i, object_value[i], type);
	}
}

void AchievementInfo::GetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT])
{
	for(BYTE i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
	{
		object_value[i] = obejct_value_[i].value_;
	}
}

void AchievementInfo::SetReward(bool reward_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
	if(is_reward_ != reward_value)
	{
		is_reward_ = reward_value;
		if(type != ACHIEVEMENT_VALUE_INIT)
		{
			Notifyreward(reward_value, RC::RC_AC_SUCCESS);
		}	
		else
		{
			if(status_ == EMPTY)
			{
				status_ = NORMAL;
			}
		}
	}		
}


void AchievementInfo::NotifyChangedValue(BYTE object_index, WORD previous_value, WORD current_value)
{
	status_ = MODIFICATION;
	if(notify_ != NULL)
	{
		notify_->OnValueChanged(index_number_, location_code_, object_index, previous_value, current_value);
	}
}

void AchievementInfo::Notifyreward(bool is_reward, RC::eAC_RESULT rc)
{
	status_ = MODIFICATION;
	if(notify_ != NULL)
	{
		notify_->Onreward(index_number_, location_code_, rewards_, is_reward, rc);
	}
}

#if !defined(_SERVER)
//_SCJ_ACSYSTEM_OVERLAP_PARENT
void AchievementInfo::SetAchievementInfoParentMap(Achievement* achievement)
{
    // 디테일 정보에 출력할 선행과제들의 제목스트일 리스트를 만든다.
    SetParentMap(achievement);

    // 자신을 선행으로 가진 과제의 목록을 만든다.
    SetChildMap(achievement);
}

void AchievementInfo::SetParentMap(Achievement* achievement)
{
    // 선행과제의 선행과제를 타고 들어가면서 가져와 쌓는다.
    parent_ac_map_.clear();
    int depth = 0;

    MakeParentMap(achievement, &parent_ac_map_, depth);

    ACHIEVEMENT_INFO_MAP_ITER map_iter = parent_ac_map_.find(GetIndex());
    if (map_iter != parent_ac_map_.end())
    {
        assert(!"선행과제 리스트 설정중에 자신이 리스트에 포함되었습니다.\nACScript.txt를 확인해 주세요");
        parent_ac_map_.erase(map_iter);
    }
}

void AchievementInfo::MakeParentMap(Achievement* achievement, ACHIEVEMENT_INFO_MAP* map_pointer, int depth)
{
    // recursive call
    if (map_pointer == NULL)
    {
        return;
    }

    if (depth >= PARENT_LIMIT_DEPTH)
    {
#ifdef _NA_008226_AC_PARENTMAP_LIST_SKIP
        assert(!"ac과제 Parent 맵을 만들던 중 깊이가 100이상 증가하였습니다. 의도인지 확인해주세요");
#else
        assert(!"ac과제 Parent 맵을 만들던 중 깊이가 30이상 증가하였습니다. 의도인지 확인해주세요");
#endif //#ifdef _NA_008226_AC_PARENTMAP_LIST_SKIP
        return;
    }


    for (int index = 0; index < MAX_ACHIEVEMENT_PARENT_CODE; ++index)
    {
        DWORD parent_code = parent_ac_code_[index];
        if (parent_code == 0)
        {
            return;
        }
        // 인덱스로 Info 탐색
        AchievementInfo* achievement_info = achievement->FindAchievementToIndex(parent_code);
        if (achievement_info)
        {
            ACHIEVEMENT_INFO_MAP_ITER map_iter = map_pointer->find(parent_code);
            if (map_iter == map_pointer->end())
            {
                map_pointer->insert(
                    ACHIEVEMENT_INFO_MAP::value_type(parent_code, achievement_info));
                //해당 Info 에 다시 선행코드가 있는지보고 재귀호출시킴 recursive call
                achievement_info->MakeParentMap(achievement, map_pointer, depth + 1);
            }
        }
    }
}

void AchievementInfo::SetChildMap(Achievement* achievement)
{
    // 선행코드가 있으면 선행코드의 AchievementInfo로 찾아가 해당 AchievementInfo의 피선행과제를 나로 지정한다.
    
    for (int i = 0; i < MAX_ACHIEVEMENT_PARENT_CODE; ++i)
    {
        if (parent_ac_code_[i] == 0)
        {
            return;
        }
        // 인덱스로 AchievementInfo탐색
        AchievementInfo* achievement_info = achievement->FindAchievementToIndex(parent_ac_code_[i]);
        if (achievement_info)
        {
            // 해당 AchievementInfo 에 내 인덱스 설정
            achievement_info->AddChildMap(this);
        }
    }
}

void AchievementInfo::AddChildMap(AchievementInfo* achievement_info)
{
    child_ac_map_.insert(
        ACHIEVEMENT_INFO_MAP::value_type(achievement_info->GetIndex(), achievement_info));
}

bool AchievementInfo::IsShowRewardToParentMap()
{
    //2011.12.2 보상버튼이 보일땐 목록을 숨기지 않도록 수정되면서 해당 함수는 사용하지 않게됨.
    // 선행과제 포함해서 보상버튼 보일지
    if ((IsAccomplishment() == true) &&
        (IsReward() == false))
    {
        return true;
    }

    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter = parent_ac_map_.begin();
    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter_end = parent_ac_map_.end();

    for(; parent_map_iter != parent_map_iter_end; ++parent_map_iter)
    {
        // 여기에 왔다는건 선행과제들이 완료가 되있다는 것이므로 완료체크는 생략
        if ((*parent_map_iter).second->IsReward() == false)
        {
            return true;
        }
    }

    return false;
}
AchievementInfo* AchievementInfo::GetRewardToParentMap()
{
    //2011.12.2 보상버튼이 보일땐 목록을 숨기지 않도록 수정되면서 해당 함수는 사용하지 않게됨.
    // 선행과제 포함해서 보상받을 과제
    AchievementInfo* reward_ac_info = NULL;

    if ((IsAccomplishment() == true) &&
            (IsReward() == false))
    {
        reward_ac_info = this;
    }

    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter = parent_ac_map_.begin();
    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter_end = parent_ac_map_.end();

    for(; parent_map_iter != parent_map_iter_end; ++parent_map_iter)
    {
        // 여기에 왔다는건 선행과제들이 완료가 되있다는 것이므로 완료체크는 생략
        if ((*parent_map_iter).second->IsReward() == false)
        {
            // 인덱스가 낮은게 우선순위 높다.
            if ((reward_ac_info == NULL) ||
                ((*parent_map_iter).first < reward_ac_info->GetIndex()))
            {
                reward_ac_info = (*parent_map_iter).second;
            }
        }
    }

    assert((reward_ac_info != NULL) && "보상이 없다");

    return reward_ac_info;
}
#endif //!defined(_SERVER)

bool AchievementInfo::_Sort_Index(AchievementInfo*& info_left, AchievementInfo*& info_right)
{
    return (info_left->GetIndex() < info_right->GetIndex());
}


const AchievementObjectInfo* const AchievementInfo::GetAchievementObjectFormIndex( BYTE object_index )
{
	return &obejct_value_[object_index];
}