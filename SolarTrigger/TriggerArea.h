#pragma once
#ifndef _SOLARTRIGGER_TRIGGER_AREA_H
#define _SOLARTRIGGER_TRIGGER_AREA_H

#include "./TriggerMsg.h"

class TriggerAreaManager;
class TriggerAreaManagerForNpc;

//==================================================================================================
// CHANGES: f110311.3L, changes functor type
struct TriggerAreaLinkForeachFunctor
{
    enum eControl { eStop, eContinue, };
    virtual eControl ProcessNode(const TriggerPlayerLink* trigger_player_link) const = 0;
};

struct TriggerAreaLinkForeachFunctorForNpc
{
    enum eControl { eStop, eContinue, };
    virtual eControl ProcessNode(const TriggerNpcLink* trigger_npc_link) const = 0;
};

//==================================================================================================
//

class TriggerArea
{
public:
    TriggerArea(DWORD area_id)
        : area_id_(area_id)
    {
        util::LList::Init(&area_conditions_); // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    }
    ~TriggerArea() {}

    void Clear();
    static bool EnterArea(TriggerAreaManager* area_manager, const TRIGGER_ENTER_AREA* msg);
    static bool LeaveArea(TriggerAreaManager* area_manager, const TRIGGER_LEAVE_AREA* msg);
    static bool ForceLeave(TriggerAreaManager* area_manager, TriggerPlayerLink* link);
    DWORD GetPlayerNumber() const;

    // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    void InsertAreaCondition(TriggerAreaLocalStorage* area_item);
    //void DeleteAreaCondition(TriggerAreaLocalStorage* area_item);
    // CHANGES: f110311.3L, changes functor type
    void Foreach(TriggerAreaLinkForeachFunctor* functor) const;

private:
    const DWORD area_id_;
    TriggerPlayerLink  char_list_root_;
    TriggerAreaLocalStorage area_conditions_;   // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    //
    friend class TriggerAreaManager;
    __DISABLE_COPY(TriggerArea);
};

//==================================================================================================
//

class TriggerAreaForNpc
{
public:
    TriggerAreaForNpc(DWORD area_id)
        : area_id_(area_id)
    {
        util::LList::Init(&area_conditions_); // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    }
    ~TriggerAreaForNpc() {}

    void Clear();
    static bool EnterArea(TriggerAreaManagerForNpc* area_manager, const TRIGGER_NPC_ENTER_AREA* msg);
    static bool LeaveArea(TriggerAreaManagerForNpc* area_manager, const TRIGGER_NPC_LEAVE_AREA* msg);
    static bool ForceLeave(TriggerAreaManagerForNpc* area_manager, TriggerNpcLink* link);
    DWORD GetNpcNumber() const;

    void InsertAreaCondition(TriggerAreaLocalStorageForNpc* area_item);
    // CHANGES: f110311.3L, changes functor type
    void Foreach(TriggerAreaLinkForeachFunctorForNpc* functor) const;

private:
    const DWORD area_id_;
    TriggerNpcLink char_list_root_;
    TriggerAreaLocalStorageForNpc area_conditions_;
    //
    friend class TriggerAreaManagerForNpc;
    __DISABLE_COPY(TriggerAreaForNpc);
};

//==================================================================================================
// inline implements

// CHANGES: f110311.3L, changes functor type
inline void TriggerArea::Foreach(TriggerAreaLinkForeachFunctor* functor) const
{
    TriggerPlayerLink* it = static_cast<TriggerPlayerLink*>(char_list_root_.next);
    while (it != &char_list_root_)
    {
        TriggerPlayerLink* link = it;
        it = static_cast<TriggerPlayerLink*>(it->next); // for safely iteration
        if (functor->ProcessNode(link) == functor->eStop) {
            break;
        }
    }
}

// CHANGES: f110311.3L, changes functor type
inline void TriggerAreaForNpc::Foreach(TriggerAreaLinkForeachFunctorForNpc* functor) const
{
    TriggerNpcLink* it = static_cast<TriggerNpcLink*>(char_list_root_.next);
    while (it != &char_list_root_)
    {
        TriggerNpcLink* link = it;
        it = static_cast<TriggerNpcLink*>(it->next); // for safely iteration
        if (functor->ProcessNode(link) == functor->eStop) {
            break;
        }
    }
}

#endif //_SOLARTRIGGER_TRIGGER_AREA_H
