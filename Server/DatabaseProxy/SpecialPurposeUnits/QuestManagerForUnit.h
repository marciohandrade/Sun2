#ifndef __DBPROXY_AC_UNIT_UTIL_H
#define __DBPROXY_AC_UNIT_UTIL_H

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

#include <QuestScriptLoader.h>
#include <QuestManager.h>
#include <QuestInfoManager.h>


namespace batch_unit {   

class IEventRegisterDummy : public IEventRegisterForNpcKillByQuest	// 좀더 일반화 시켜야 하는데...
{
public:
    virtual ~IEventRegisterDummy() {}
    virtual BOOL RegisterNpcKillInfo(WORD/*QCODE*/ questCode, MONSTERCODE npcCode) {
        __UNUSED((questCode, npcCode));
        return true;
    }
};

class QuestManagerForUnit : public QuestManager
{
public:
    QuestManagerForUnit(QuestInfoManager* pmgr);

    virtual QuestCondition* CreateQuestCondition(QuestConditionInfo*) { return 0; }
    virtual QuestAction* CreateQuestAction(QuestActionInfo*) { return 0; }
    virtual void DestroyQuestCondition(QuestCondition*) { return; }
    virtual void DestroyQuestAction(QuestAction*) { return; }

    // 퀘스트 완료, 실패에 따른 이벤트 알림
    virtual void OnEvent(eQUEST_EVENT, QUEST_MSG*, Quest*) { return; }

};

// CHANGES: f101217.1L, fixed invalid data declaration
// because it's duplicable that a plain field static member in a header file is relocated
// each of code section address space ('module' expressed on the assembly level)
extern QuestScriptLoader quest_script_parser_global;
extern QuestInfoManager quest_info_manager_global_;
extern IEventRegisterDummy quest_event_register_dummy_;
extern QuestManagerForUnit quest_manager_static_;

}; //end of namespace

#endif //DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#endif //__DBPROXY_AC_UNIT_UTIL_H