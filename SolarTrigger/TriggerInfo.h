#ifndef _SOLARTRIGGER_TRIGGERINFO_H
#define _SOLARTRIGGER_TRIGGERINFO_H
#pragma once

//==================================================================================================/// TriggerInfo class
//
//  @author   Kim Min Wook < taiyo@webzen.com >
//  @since    2004. 12. 29
//  @remark
//  - 트리거 정보를 내포하고 있는 래퍼 클래스
//  @note
//  - 
//  @history 
//  - 2007. 10. xx  waverix, rearrange
//  - 2009. 06. 13  waverix, rearrange
//  - 2011. 07. 20, waverix, code improvement
//*/
//==================================================================================================

class CWzArchive;
class ConditionInfo;
class ActionInfo;
class TriggerInfoManager;

namespace ns_trigger {
;

typedef STLX_VECTOR<ConditionInfo*> CONDITION_INFO_LIST;
typedef STLX_VECTOR<ActionInfo*>    ACTION_INFO_LIST;

}; //end of namespace

//==================================================================================================
//
class TriggerInfo
{
public:
    enum { _MAX_CATEGORY_NAME_LENGTH = 0xFF, };
    enum eATTRIBUTE_BITs
    {
        OR_BIT          = (1<<0),
        ACTIVE_BIT      = (1<<1),
        LOOP_BIT        = (1<<2),
        NEXT_ACTIVE_BIT = (1<<3),
    };
    //
    TriggerInfo();
    ~TriggerInfo();

    void Load(CWzArchive& IN archive, TriggerInfoManager* trigger_info_manager = NULL);
    void Release();

#ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
    void set_condition_index(WORD condition_index);
    WORD condition_index() const;
    const TCHAR* group_name() const;
    void _setGroupName(const TCHAR* group_name);
#endif //
    const TCHAR* CategoryName() const;
    WzID TriggerID() const;
    bool IsAND() const;
    bool IsActive() const;
    bool IsLoop() const;
    bool IsNextActive() const;
    bool IsAlways() const;

    const ns_trigger::CONDITION_INFO_LIST& GetConditionInfoList() const;
    const ns_trigger::ACTION_INFO_LIST& GetActionInfoList() const;
private:
    ConditionInfo*  _ParseCondition(CWzArchive& IN archive, WORD condition_type,
                                    TriggerInfoManager* trigger_info_manager = NULL);
    ActionInfo*     _ParseAction(CWzArchive& IN archive, WORD action_type,
                                 TriggerInfoManager* trigger_info_manager = NULL);

    void _setCategoryName(const TCHAR* category_name);
    //----------------------------------------------------------------------------------------------
    // (Fields)
    WzID trigger_id_;
    WORD trigger_attributes_;
    bool always_trigger_;
    ns_trigger::CONDITION_INFO_LIST condition_info_list_;
    ns_trigger::ACTION_INFO_LIST    action_info_list_;

    TCHAR category_name_[_MAX_CATEGORY_NAME_LENGTH];
#ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
    TCHAR group_name_[_MAX_CATEGORY_NAME_LENGTH];
    WORD condition_index_;
#endif //
    //
    friend class BaseTrigger;
    friend class TriggerGroupInfo;
    __DISABLE_COPY(TriggerInfo);
};

//==================================================================================================

inline const TCHAR* TriggerInfo::CategoryName() const
{
    return category_name_;
}

inline WzID TriggerInfo::TriggerID() const
{
    return trigger_id_;
}

inline bool TriggerInfo::IsAND() const
{
    return  !(trigger_attributes_ & OR_BIT);
}

inline bool TriggerInfo::IsActive() const
{
    return !!(trigger_attributes_ & ACTIVE_BIT);
}

inline bool TriggerInfo::IsLoop() const
{
    return !!(trigger_attributes_ & LOOP_BIT);
}

inline bool TriggerInfo::IsNextActive() const
{
    return !!(trigger_attributes_ & NEXT_ACTIVE_BIT);
}

inline bool TriggerInfo::IsAlways() const
{
    return always_trigger_ != false;
}

inline const ns_trigger::CONDITION_INFO_LIST& TriggerInfo::GetConditionInfoList() const
{
    return condition_info_list_;
}

inline const ns_trigger::ACTION_INFO_LIST& TriggerInfo::GetActionInfoList() const
{
    return action_info_list_;
}

#ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
inline void TriggerInfo::_setGroupName(const TCHAR* group_name)
{
    _tcsncpy(group_name_, group_name, _countof(group_name_));
    group_name_[_countof(group_name_) - 1] = _T('\0');
}

inline void TriggerInfo::set_condition_index(WORD condition_index)
{
    condition_index_ = condition_index;
}

inline WORD TriggerInfo::condition_index() const
{
    return condition_index_;
}

inline const TCHAR* TriggerInfo::group_name() const
{
    return group_name_;
}

#endif //

inline void TriggerInfo::_setCategoryName(const TCHAR* category_name)
{
    _tcsncpy(category_name_, category_name, _countof(category_name_));
    category_name_[_countof(category_name_) - 1] = _T('\0');
}

#endif //_SOLARTRIGGER_TRIGGERINFO_H

