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
        //�̼� Ŭ���� Ÿ�� ac�� ������ ������ ������ �����̴�
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
    //�̼� Ŭ���� Ÿ���� ������ ������ ������ �����̴�
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

    //���� �޼���ġ���� ū�� ?        
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
        //�� ������ ������ true �� �����Ѵ�.
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
    // ������ ������ ����� ����������� ����Ʈ�� ����Ʈ�� �����.
    SetParentMap(achievement);

    // �ڽ��� �������� ���� ������ ����� �����.
    SetChildMap(achievement);
}

void AchievementInfo::SetParentMap(Achievement* achievement)
{
    // ��������� ��������� Ÿ�� ���鼭 ������ �״´�.
    parent_ac_map_.clear();
    int depth = 0;

    MakeParentMap(achievement, &parent_ac_map_, depth);

    ACHIEVEMENT_INFO_MAP_ITER map_iter = parent_ac_map_.find(GetIndex());
    if (map_iter != parent_ac_map_.end())
    {
        assert(!"������� ����Ʈ �����߿� �ڽ��� ����Ʈ�� ���ԵǾ����ϴ�.\nACScript.txt�� Ȯ���� �ּ���");
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
        assert(!"ac���� Parent ���� ����� �� ���̰� 100�̻� �����Ͽ����ϴ�. �ǵ����� Ȯ�����ּ���");
#else
        assert(!"ac���� Parent ���� ����� �� ���̰� 30�̻� �����Ͽ����ϴ�. �ǵ����� Ȯ�����ּ���");
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
        // �ε����� Info Ž��
        AchievementInfo* achievement_info = achievement->FindAchievementToIndex(parent_code);
        if (achievement_info)
        {
            ACHIEVEMENT_INFO_MAP_ITER map_iter = map_pointer->find(parent_code);
            if (map_iter == map_pointer->end())
            {
                map_pointer->insert(
                    ACHIEVEMENT_INFO_MAP::value_type(parent_code, achievement_info));
                //�ش� Info �� �ٽ� �����ڵ尡 �ִ������� ���ȣ���Ŵ recursive call
                achievement_info->MakeParentMap(achievement, map_pointer, depth + 1);
            }
        }
    }
}

void AchievementInfo::SetChildMap(Achievement* achievement)
{
    // �����ڵ尡 ������ �����ڵ��� AchievementInfo�� ã�ư� �ش� AchievementInfo�� �Ǽ�������� ���� �����Ѵ�.
    
    for (int i = 0; i < MAX_ACHIEVEMENT_PARENT_CODE; ++i)
    {
        if (parent_ac_code_[i] == 0)
        {
            return;
        }
        // �ε����� AchievementInfoŽ��
        AchievementInfo* achievement_info = achievement->FindAchievementToIndex(parent_ac_code_[i]);
        if (achievement_info)
        {
            // �ش� AchievementInfo �� �� �ε��� ����
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
    //2011.12.2 �����ư�� ���϶� ����� ������ �ʵ��� �����Ǹ鼭 �ش� �Լ��� ������� �ʰԵ�.
    // ������� �����ؼ� �����ư ������
    if ((IsAccomplishment() == true) &&
        (IsReward() == false))
    {
        return true;
    }

    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter = parent_ac_map_.begin();
    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter_end = parent_ac_map_.end();

    for(; parent_map_iter != parent_map_iter_end; ++parent_map_iter)
    {
        // ���⿡ �Դٴ°� ����������� �Ϸᰡ ���ִٴ� ���̹Ƿ� �Ϸ�üũ�� ����
        if ((*parent_map_iter).second->IsReward() == false)
        {
            return true;
        }
    }

    return false;
}
AchievementInfo* AchievementInfo::GetRewardToParentMap()
{
    //2011.12.2 �����ư�� ���϶� ����� ������ �ʵ��� �����Ǹ鼭 �ش� �Լ��� ������� �ʰԵ�.
    // ������� �����ؼ� ������� ����
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
        // ���⿡ �Դٴ°� ����������� �Ϸᰡ ���ִٴ� ���̹Ƿ� �Ϸ�üũ�� ����
        if ((*parent_map_iter).second->IsReward() == false)
        {
            // �ε����� ������ �켱���� ����.
            if ((reward_ac_info == NULL) ||
                ((*parent_map_iter).first < reward_ac_info->GetIndex()))
            {
                reward_ac_info = (*parent_map_iter).second;
            }
        }
    }

    assert((reward_ac_info != NULL) && "������ ����");

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