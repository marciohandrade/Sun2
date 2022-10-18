#include "StdAfx.h"
#include "QuestManagerForUnit.h"

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
namespace batch_unit {
;

QuestScriptLoader quest_script_parser_global;
QuestInfoManager quest_info_manager_global_;
IEventRegisterDummy quest_event_register_dummy_;
QuestManagerForUnit quest_manager_static_(&quest_info_manager_global_);

QuestManagerForUnit::QuestManagerForUnit(QuestInfoManager* pmgr) : QuestManager(pmgr)
{
 
}

QuestCondition* CreateQuestCondition(QuestConditionInfo* pInfo) 
{
    __UNUSED(pInfo);
    return 0; 
}

QuestAction* CreateQuestAction(QuestActionInfo* pInfo)
{
    __UNUSED(pInfo);
    return 0; 
}

void DestroyQuestCondition(QuestCondition* pCondition)
{ 
    __UNUSED(pCondition);
    return; 
}

void DestroyQuestAction(QuestAction* pAction)
{
    __UNUSED(pAction);
    return;
}

void OnEvent(eQUEST_EVENT evt, QUEST_MSG* pMsg, Quest* pQuest)
{
    __UNUSED((evt, pMsg, pQuest));
    return; 
}

}; //end of namespace

#endif //DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT