#include "SunClientPrecompiledHeader.h"
#include "AreaConquestInfo.h"
#include "AreaConquestDefine.h"
#include "hero.h"

namespace
{
    const int kAllAreaCode = 601; // 전체

    class SortByIndex
    {
    public:
        bool operator()(AchievementInfo*& Info_left_ref, AchievementInfo*& Info_right_ref) const
        {
            return Info_left_ref->GetIndex() < Info_right_ref->GetIndex();
        }
    };
};

AreaConquestInfo::AreaConquestInfo():
achievement_(NULL),
show_ratio_(false)
{
}
AreaConquestInfo::~AreaConquestInfo()
{
}

void AreaConquestInfo::InitAreaCodeList()
{
    if (achievement() == NULL)
    {
        return;
    }

    // AC 지역 리스트업
    // 전체 추가
    WORD area_code = kAllAreaCode;
    area_code_list_.clear();
    area_code_list_.push_back(kAllAreaCode);

    // Achievement 정보에 따른 지역코드 추가
    Achievement::ACHIEVEMENT_PART_ITER achievement_part_itr = achievement()->Begin();
    Achievement::ACHIEVEMENT_PART_ITER achievement_part_itr_end = achievement()->End();
    for ( ; achievement_part_itr != achievement_part_itr_end; ++achievement_part_itr)
    {
        area_code = (achievement_part_itr->first);
        area_code_list_.push_back(area_code);
    }
    std::sort(area_code_list_.begin(), area_code_list_.end());
}

void AreaConquestInfo::RefreshAchievementInfo(WORD selected_area_code, WORD selected_info_state)
{
    if (achievement() == NULL)
    {
        return;
    }

    AchievementInfo_List progress_info_list;
    AchievementInfo_List complete_info_list;
    achievement_info_list_.clear();
    complete_ratio_ = 0;
    info_total_count_ = 0;
    info_complete_count_ = 0;
    show_ratio_ = false;
    // 지역 및 정렬 상태에 따라 AchievementInfo 취합
    // 전체 또는 특정지역
    Achievement::ACHIEVEMENT_PART_ITER achievement_part_itr = achievement()->Begin();
    Achievement::ACHIEVEMENT_PART_ITER achievement_part_itr_end = achievement()->End();
    for ( ; achievement_part_itr != achievement_part_itr_end; ++achievement_part_itr)
    {
        AchievementPart* achievement_part = achievement_part_itr->second;
        if (achievement_part == NULL)
        {
            continue;
        }

        if (selected_area_code != kAllAreaCode)
        {
            if (selected_area_code != achievement_part->GetLocationCode())
            {
                continue;
            }
            complete_ratio_ = achievement_part->GetAccomplishmentRatio();
            show_ratio_ = achievement_part->show_ratio();
        }

        AchievementPart::ACHIEVEMENT_ITER info_itr = achievement_part->Begin();
        AchievementPart::ACHIEVEMENT_ITER info_itr_end = achievement_part->End();
        for ( ; info_itr != info_itr_end; ++info_itr)
        {
            AchievementInfo* info_ptr = info_itr->second;
            if (info_ptr == NULL)
            {
                continue;
            }
            if (info_ptr->GetClassType() != eCHAR_NONE && g_pHero != NULL)
            {
                if (g_pHero->GetClass() != info_ptr->GetClassType())
                {
                    continue;
                } 
            }
            if (IsAccomplishmentParent(info_ptr) == false)
            {
                continue;
            }
#ifdef _SCJ_ACSYSTEM_OVERLAP_PARENT
            if (IsOverlap(info_ptr) == true)
            {
                // 곂쳐서 숨길지
                continue;
            }
#endif //_SCJ_ACSYSTEM_OVERLAP_PARENT
            
            // 상태에 따른 취합
            bool is_complete = info_ptr->IsAccomplishment() == true;

            switch (selected_info_state)
            {
            case uiAreaConquest::kSelect_All:
                {
                    achievement_info_list_.push_back(info_ptr);
                }
                break;
            case uiAreaConquest::kSelect_Progress:
            case uiAreaConquest::kSelect_Complete:
                {
                    if (is_complete == true)
                    {
                        complete_info_list.push_back(info_ptr);
                    }
                    else
                    {
                        progress_info_list.push_back(info_ptr);
                    }
                }
                break;
            }

            ++info_total_count_;
            if (is_complete == true)
            {
                ++info_complete_count_;
            }
        }
    }
    
    // 상태선택에 따른 정렬
    switch (selected_info_state)
    {
    case uiAreaConquest::kSelect_All:
        {
            std::sort(achievement_info_list_.begin(), achievement_info_list_.end(), SortByIndex());
        }
        break;
    case uiAreaConquest::kSelect_Progress:
        {
            std::sort(progress_info_list.begin(), progress_info_list.end(), SortByIndex());
            std::sort(complete_info_list.begin(), complete_info_list.end(), SortByIndex());
            achievement_info_list_.insert(achievement_info_list_.end(), 
                progress_info_list.begin(), progress_info_list.end());
            achievement_info_list_.insert(achievement_info_list_.end(), 
                complete_info_list.begin(), complete_info_list.end());
        }
        break;
    case uiAreaConquest::kSelect_Complete:
        {
            std::sort(progress_info_list.begin(), progress_info_list.end(), SortByIndex());
            std::sort(complete_info_list.begin(), complete_info_list.end(), SortByIndex());
            achievement_info_list_.insert(achievement_info_list_.end(), 
                complete_info_list.begin(), complete_info_list.end());
            achievement_info_list_.insert(achievement_info_list_.end(), 
                progress_info_list.begin(), progress_info_list.end());
        }
        break;
    }
}

WORD AreaConquestInfo::GetAreaCode( WORD list_index )
{
    WORD area_code = 0;
    if (list_index < area_code_list_.size())
    {
        area_code = area_code_list_[list_index];
    }

    return area_code;
}

WORD AreaConquestInfo::GetStartAreaCode()
{
    WORD area_code = area_code_list_[uiAreaConquest::kFirstAreaCodeIndex];
    return area_code;
}

DWORD AreaConquestInfo::GetAccomplishmentRatio()
{
    return complete_ratio_;
}

DWORD AreaConquestInfo::GetAchievementInfoTotalCount()
{
    return info_total_count_;
}

DWORD AreaConquestInfo::GetAccomplishmentCount()
{
    return info_complete_count_;
}

bool AreaConquestInfo::IsShowRatio()
{
    return show_ratio_;
}

bool AreaConquestInfo::IsAccomplishmentParent(AchievementInfo* achivement_ptr)
{
    for (int i = 0; i < MAX_ACHIEVEMENT_PARENT_CODE; ++i)
    {
        if (achivement_ptr->GetParentCode(i) == 0 )
        {
            return true;
        }

        bool is_accomplishment = achievement()->IsAccomplishmentByIndex(achivement_ptr->GetParentCode(i));
        if (is_accomplishment == false)
        {
            return false;
        }
    }

    return true;
}
#ifdef _SCJ_ACSYSTEM_OVERLAP_PARENT
bool AreaConquestInfo::IsOverlap(AchievementInfo* achivement_ptr)   // return true : 숨김 / false : 안숨김
{
    if ((achivement_ptr->IsAccomplishment() == true) &&
        (achivement_ptr->IsReward() == false))
    {
        // 완료가 됬으나 보상을 받지 않았으면 숨기지 않음
        return false;
    }

    if ((achivement_ptr == NULL) ||
        (achivement_ptr->GetChildMap().size() == 0))
    {
        // 피선행과제가 없으면 숨기지 않음
        return false;
    }

    ACHIEVEMENT_INFO_MAP_ITER begin = achivement_ptr->GetChildMap().begin();
    ACHIEVEMENT_INFO_MAP_ITER end = achivement_ptr->GetChildMap().end();

    AchievementInfo* child_achievement;
    for (; begin != end; ++begin)
    {
        child_achievement = (*begin).second;

        // 클래스가 다르면 비교대상에서 제외
        if (child_achievement->GetClassType() != eCHAR_NONE && g_pHero != NULL)
        {
            if (g_pHero->GetClass() != child_achievement->GetClassType())
            {
                continue;
            }
        }

        if (IsAccomplishmentParent(child_achievement) == false)
        {
            // 완료 안된 피선행과제가 있으면 숨기지 않음
            return false;
        }
    }

    return true;
}
#endif //_SCJ_ACSYSTEM_OVERLAP_PARENT
