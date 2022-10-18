#pragma once

#include "Achievement.h"
#include "AchievementParser.h"
#include "AchievementInfo.h"
typedef std::vector<WORD> AreaCode_List;
typedef std::vector<WORD>::iterator AreaCode_Iterator;

typedef std::vector<AchievementInfo*> AchievementInfo_List;
typedef std::vector<AchievementInfo*>::iterator AchievementInfo_Iterator;

class AreaConquestInfo
{
public:
    AreaConquestInfo();
    ~AreaConquestInfo();

    void set_achievement(Achievement* achievement_ptr) { achievement_ = achievement_ptr; }
    Achievement* achievement() { return achievement_; }

    void InitAreaCodeList();
    void RefreshAchievementInfo(WORD selected_area_code, WORD selected_info_state);

    WORD GetAreaCode(WORD list_index);
    WORD GetStartAreaCode();

    AreaCode_Iterator GetAreaCodeBegin();
    AreaCode_Iterator GetAreaCodeEnd();

    AchievementInfo_Iterator GetAchievementInfoBegin();
    AchievementInfo_Iterator GetAchievementInfoEnd();
    AchievementInfo* GetAchievementInfo(int list_index);

    WORD GeAchievementInfoSize();
    AchievementInfo* GetACInfo(WORD achievement_index);

    DWORD GetAccomplishmentRatio();
    DWORD GetAchievementInfoTotalCount();
    DWORD GetAccomplishmentCount();
    bool  IsShowRatio();

private:
    bool IsAccomplishmentParent(AchievementInfo* achivement_ptr); // 선행과제가 완료됬는지 확인한다.
#ifdef _SCJ_ACSYSTEM_OVERLAP_PARENT
    bool IsOverlap(AchievementInfo* achivement_ptr); // 피선행과제가 활성화인지 체크 후 숨길지
#endif //_SCJ_ACSYSTEM_OVERLAP_PARENT
private:
    Achievement* achievement_;

    AreaCode_List area_code_list_;  // 한번만리스트업, 전체+AC스크립트(첫번째는 StartCode로 지정)
    AchievementInfo_List achievement_info_list_; // 지역 및 상태에 따라 선별/정렬된 Info리스트

    DWORD complete_ratio_;
    WORD info_total_count_;
    WORD info_complete_count_;
    bool show_ratio_;
};

inline AreaCode_Iterator AreaConquestInfo::GetAreaCodeBegin()
{
    return area_code_list_.begin();
}

inline AreaCode_Iterator AreaConquestInfo::GetAreaCodeEnd()
{
    return area_code_list_.end();
}

inline AchievementInfo_Iterator AreaConquestInfo::GetAchievementInfoBegin()
{
    return achievement_info_list_.begin();
}

inline AchievementInfo_Iterator AreaConquestInfo::GetAchievementInfoEnd()
{
    return achievement_info_list_.end();
}

inline AchievementInfo* AreaConquestInfo::GetAchievementInfo(int list_index)
{
    return achievement_info_list_[list_index];
}

inline WORD AreaConquestInfo::GeAchievementInfoSize()
{
    return static_cast<WORD>(achievement_info_list_.size());
}
