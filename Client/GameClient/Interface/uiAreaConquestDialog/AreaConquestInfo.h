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
    bool IsAccomplishmentParent(AchievementInfo* achivement_ptr); // ��������� �Ϸ����� Ȯ���Ѵ�.
#ifdef _SCJ_ACSYSTEM_OVERLAP_PARENT
    bool IsOverlap(AchievementInfo* achivement_ptr); // �Ǽ�������� Ȱ��ȭ���� üũ �� ������
#endif //_SCJ_ACSYSTEM_OVERLAP_PARENT
private:
    Achievement* achievement_;

    AreaCode_List area_code_list_;  // �ѹ�������Ʈ��, ��ü+AC��ũ��Ʈ(ù��°�� StartCode�� ����)
    AchievementInfo_List achievement_info_list_; // ���� �� ���¿� ���� ����/���ĵ� Info����Ʈ

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
