#ifndef _SOLARTRIGGER_TRIGGER_MANAGER_H
#define _SOLARTRIGGER_TRIGGER_MANAGER_H
#pragma once

//==================================================================================================
// TriggerManager class
/**
    @author	Cho Jae Sik < jscho@webzen.com >
    @since	2007. 04. 04
    @remark
    - 트리거와 트리거인포를 관리하는 클래스.
    @note
    - 
    @history 
    - 2007. 04. 04  최초 생성.
    - 2007. 10. xx  waverix, rearrange
    - 2009. 06. 13  waverix, rearrange
    - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================

#include "BaseTrigger.h"
#include "TriggerAreaManager.h"


class CWzArchive;

namespace ns_trigger {
;

struct TriggerListNode
{
    WzID first;
    BaseTrigger* second;
};

}; //end of namespace

typedef STLX_VECTOR<ns_trigger::TriggerListNode> TRIGGER_LIST;

//==================================================================================================
// (TriggerManager)
//
class TriggerManager
{
public:
    typedef std::pair<WzID, BaseTrigger*>   sTRIGGER_LIST_NODE;
    typedef STLX_LIST<ns_trigger::TriggerListNode>   ACTIVE_TRIGGER_LIST;

    TriggerManager();
    virtual ~TriggerManager();

    void Release();
    bool Disconnected(TriggerPlayerLink* const link);

    const TRIGGER_LIST& GetTriggerList() const;
    const TriggerAreaManager* GetAreaManager() const;
    //////////////////////////////////////////////////////////////////////////
    //  (TRIGGER SECTION)
    bool InsertTrigger(BaseTrigger* trigger);
    BaseTrigger* FindTrigger(const WzID trigger_id) const;
    BaseTrigger* FindNextTrigger(WzID current_trigger_id);
    int GetTriggerCount() const;

    //////////////////////////////////////////////////////////////////////////
    //  (TRIGGER_AREA SECTION)
    bool CreateArea(int area_id, TriggerAreaLocalStorage* area_item);
    bool OnLeaveArea(TRIGGER_LEAVE_AREA* trigger_msg);
    bool OnEnterArea(TRIGGER_ENTER_AREA* trigger_msg);
    DWORD GetPlayerNumForAreaID(int area_id) const;
    int  GetTriggerAreaCount() const;
    //////////////////////////////////////////////////////////////////////////
    //  <MESSAGE HANDLER>
    //  클라이언트로 부터 오는 이벤트 메세지
    //  - TRIGGER_ENTER_AREA
    //  - TRIGGER_LEAVE_AREA
    //  - TRIGGER_CLICK_OBJECT
    //  서버에서 오는 이벤트 메세지
    //  - TRIGGER_DAMAGE_OBJECT
    //  - TRIGGER_KILL_MONSTER
    void OnMsg(TRIGGER_MSG* trigger_msg);

    //////////////////////////////////////////////////////////////////////////
    //  <ACTIVE TRIGGER SECTION>
    bool InsertActiveTrigger(WzID trigger_id);
    // CHANGES: f110720.3L, prevent the corrupted list container state
    void RequestRemoveActiveTrigger(WzID trigger_id) const;
    // WARNING: f110721.2L, this method must be used in GameDominationField.
    // side-effect: it call the Trigger::Release(triggers cleared)
    void RemoveAllActiveTrigger();
    BaseTrigger* FindActiveTrigger(WzID trigger_id) const;
    bool ActiveNextTrigger(WzID current_trigger_id);

    //////////////////////////////////////////////////////////////////////////
    //  <OTHER METHODs>
protected:
    virtual BaseTrigger* AllocTrigger() = 0;
    virtual void FreeTrigger(BaseTrigger* trigger) = 0;
private:
    typedef util::SolarHashTable<BaseTrigger*, WzID> TriggerSearcher;
    //////////////////////////////////////////////////////////////////////////
    //  <FIELDs>
protected:
    TRIGGER_LIST m_listTrigger;
    //ACTIVE_TRIGGER_LIST active_trigger_list_; //< 활성화된 트리거
    TriggerAreaManager  trigger_area_manager_;
    util::__LList<ns_trigger::ActiveTriggerLink> active_trigger_list_;
private:
    TriggerSearcher trigger_searcher_;
};

//==================================================================================================

inline const TRIGGER_LIST& TriggerManager::GetTriggerList() const {
    return m_listTrigger;
}

inline const TriggerAreaManager* TriggerManager::GetAreaManager() const {
    return &trigger_area_manager_;
}

inline int TriggerManager::GetTriggerCount() const {
    return int(m_listTrigger.size());
}

inline int TriggerManager::GetTriggerAreaCount() const {
    return trigger_area_manager_.GetTriggerAreaCount();
}

inline BaseTrigger* TriggerManager::FindTrigger(const WzID trigger_id) const
{
    BaseTrigger* found = trigger_searcher_.GetData(trigger_id);
    return found;
}

inline BaseTrigger* TriggerManager::FindActiveTrigger(WzID trigger_id) const
{
    BaseTrigger* found = this->FindTrigger(trigger_id);
    if ((found == NULL)) {
        return NULL;
    };
    if (util::LList::IsEmpty(&found->active_link_)) {
        return NULL;
    };
    assert(found->activated_ != false);
    return found;
}

#endif //_SOLARTRIGGER_TRIGGER_MANAGER_H
