#ifndef _SOLARTRIGGER_TRIGGERINFOMANAGER_H
#define _SOLARTRIGGER_TRIGGERINFOMANAGER_H
#pragma once

/************************************************************************/
/*                                                                      */
/*                TriggerGroupInfo를 관리하는 클래스                */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#include <chunk_stl.h>
#include <ProgramCommon/wzarchive.h>

//==================================================================================================
class TriggerGroupInfo;

#if defined(_SERVER)
class UnitRegenInfoManager;
class IEventRegisterForNpcKillByTrigger // 좀더 일반화 시켜야 하는데...
{
public:
    virtual ~IEventRegisterForNpcKillByTrigger() {}
    virtual BOOL RegisterNpcKillInfo(const MONSTERCODE npc_code) = 0;
    virtual BOOL RegisterUnitKillInfo(const REGENID unit_id) = 0;

    //  Unit ID 등록은 WUT, WMO 분리 상의 문제로 임시공간이 요구되므로 이에 대한 초기화 작업이 요구된다.
    virtual void    PrepareRegistering() = 0;
    //add spec. by __NA001277_090205_NAVI_SYSTEM_SUPPORT__
    virtual void    BuildRegisterInfo(MAPCODE mapCode, FIELDCODE fieldCode,
                                      UnitRegenInfoManager* const pUnitRegenInfoManager) = 0;
};
#else
class IEventRegisterForNpcKillByTrigger;
#endif

//==================================================================================================


//==================================================================================================

class TriggerInfoManager
{
public:
    typedef STLX_MAP<STLX_STRING, TriggerGroupInfo*> TRIGGER_GROUPINFO_MAP;

    TriggerInfoManager();
    ~TriggerInfoManager();

    BOOL InsertTriggerGroupInfo(TriggerGroupInfo* pGroupInfo);
    TriggerGroupInfo* FindTriggerGruopInfo(const TCHAR* pszGroupName) const;
    void Release();
    void LoadGroupInfos(CWzArchive& IN archive,
        IEventRegisterForNpcKillByTrigger* event_register = NULL);
#if defined(_SERVER)
    bool NotifyTriggerRegisterNpcKill(const MONSTERCODE npc_code);
    bool NotifyTriggerRegisterUnitKill(const REGENID unit_id);
#endif

private:
    void CheckChunk(CWzArchive& IN archive);

    TRIGGER_GROUPINFO_MAP trigger_group_info_table_;
#if defined(_SERVER)
    IEventRegisterForNpcKillByTrigger* event_register_;
#endif
    __DISABLE_COPY(TriggerInfoManager);
};

#if defined(_SERVER)
inline bool TriggerInfoManager::NotifyTriggerRegisterNpcKill(const MONSTERCODE npc_code)
{
    if (event_register_) {
        return (event_register_->RegisterNpcKillInfo(npc_code) != false);
    };
    return true;
}

inline bool TriggerInfoManager::NotifyTriggerRegisterUnitKill(const REGENID unit_id)
{
    if (event_register_) {
        return (event_register_->RegisterUnitKillInfo(unit_id) != false);
    };
    return true;
}
#endif

#endif //_SOLARTRIGGER_TRIGGERINFOMANAGER_H
