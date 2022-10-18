#pragma once
#include "uiQuestListDlg/uiQuestListDlg.h"

class Quest;
class QuestCondition;

class uiContinentNavigation
{
public:
    uiContinentNavigation();
    ~uiContinentNavigation();

    void InitializeNaviPoint();
    void SetNavigationPoint();
    void RenderNavigationPoint();
    void RenderHeroLocation();

protected:
    void RenderNavigationTooltip(int navigation_index);
    void AddTooltip_QuestState(Quest* quest);
    void AddTooltipLine_ConditionItem(QuestCondition* quest_condition);
    void AddTooltipLine_ConditionMoney(QuestCondition* quest_condition);
    void AddTooltipLine_ConditionKillMon(QuestCondition* quest_condition);
    void AddTooltipLine_ConditionMission(QuestCondition* quest_condition);
    void AddTooltipLine_ConditionQuest(QuestCondition* quest_condition);

private:
    int navigation_point_[MAX_NAVI_CNT];

};

inline void uiContinentNavigation::InitializeNaviPoint()
{
    for (int index = 0; index < MAX_NAVI_CNT; ++index)
    {
        navigation_point_[index] = -1;
    }
}
