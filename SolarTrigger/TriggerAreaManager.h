#pragma once
#ifndef _SOLARTRIGGER_TRIGGERAREAMANAGER_H
#define _SOLARTRIGGER_TRIGGERAREAMANAGER_H

#include "TriggerMsg.h"
#include "TriggerArea.h"

//==================================================================================================


//==================================================================================================
//
class TriggerAreaManager
{
public:
    typedef TriggerArea AreaType;
    typedef STLX_HASH_MAP<int, AreaType*> TRIGGER_AREA_MAP;

    TriggerAreaManager();
    ~TriggerAreaManager();

    void    Init();
    void    Release();
    AreaType* FindArea(int area_id) const;
    bool    EnterArea(TRIGGER_ENTER_AREA* msg);
    bool    LeaveArea(TRIGGER_LEAVE_AREA* msg);
    DWORD   GetPlayerNumber(int area_id) const;
    bool    InsertNewArea(int area_id, TriggerAreaLocalStorage* area_item);
    int     GetTriggerAreaCount() const;

    const TRIGGER_AREA_MAP& GetAreas() const;

private:
    TRIGGER_AREA_MAP trigger_areas_;
};

//==================================================================================================
// NOTE: f110311.4L, _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
class TriggerAreaManagerForNpc
{
public:
    typedef TriggerAreaForNpc AreaType;
    typedef STLX_HASH_MAP<int, AreaType*> TRIGGER_AREA_MAP;

    TriggerAreaManagerForNpc();
    ~TriggerAreaManagerForNpc();

    void    Init();
    void    Release();
    AreaType* FindArea(int area_id) const;
    bool    EnterArea(TRIGGER_NPC_ENTER_AREA* msg);
    bool    LeaveArea(TRIGGER_NPC_LEAVE_AREA* msg);
    DWORD   GetNpcNumber(int area_id);
    bool    InsertNewArea(int area_id, TriggerAreaLocalStorageForNpc* area_item);
    int     GetTriggerAreaCount() const;

    const TRIGGER_AREA_MAP& GetAreas() const;

private:
    TRIGGER_AREA_MAP trigger_areas_;
};

//==================================================================================================

inline int TriggerAreaManager::GetTriggerAreaCount() const {
    return static_cast<int>(trigger_areas_.size());
}

inline const TriggerAreaManager::TRIGGER_AREA_MAP&
TriggerAreaManager::GetAreas() const {
    return trigger_areas_;
}

inline TriggerAreaManager::AreaType*
TriggerAreaManager::FindArea(int area_id) const
{
    TRIGGER_AREA_MAP::const_iterator it = trigger_areas_.find(area_id);
    if (it != trigger_areas_.end()) {
        return it->second;
    }
    return NULL;
};

inline int TriggerAreaManagerForNpc::GetTriggerAreaCount() const {
    return static_cast<int>(trigger_areas_.size());
}

inline const TriggerAreaManagerForNpc::TRIGGER_AREA_MAP&
TriggerAreaManagerForNpc::GetAreas() const {
    return trigger_areas_;
}

inline TriggerAreaManagerForNpc::AreaType*
TriggerAreaManagerForNpc::FindArea(int area_id) const
{
    TRIGGER_AREA_MAP::const_iterator it = trigger_areas_.find(area_id);
    if (it != trigger_areas_.end()) {
        return it->second;
    }
    return NULL;
};

#endif //_SOLARTRIGGER_TRIGGERAREAMANAGER_H

