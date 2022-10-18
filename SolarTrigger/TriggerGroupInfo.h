#ifndef _SOLARTRIGGER_TRIGGERGROUPINFO_H
#define _SOLARTRIGGER_TRIGGERGROUPINFO_H
#pragma once
//==================================================================================================
/**
    @author	Kim Min Wook < taiyo@webzen.com >
    @since	2004. 12. 29
    @remark
    - 여러 컨디션과 액션 트리거 정보를 가지고 있는 트리거 그룹 랩퍼 클래스
    @note
    - 
    @history 
    - 
    - 2007. 10. xx  waverix, rearrange
    - 2009. 06. 13  waverix, rearrange
    - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================

#include <ProgramCommon/WzDataType.h>

class CWzArchive;
class TriggerInfo;
class TriggerInfoManager;

typedef STLX_VECTOR<TriggerInfo*>   TRIGGER_INFO_LIST;

//==================================================================================================
//
class TriggerGroupInfo
{
private:
    friend class TriggerManager;
    enum { _MAX_GROUP_NUM = 10, _MAX_GROUP_NAME_LENGTH = 255, };

public:
    TriggerGroupInfo();
    ~TriggerGroupInfo();

public:
    void Load(CWzArchive& IN archive, TriggerInfoManager* pInfoMgr = NULL);
    void Release();
    const TCHAR* GroupName() const;
    const TRIGGER_INFO_LIST& GetTriggerInfos() const;
    //
    TriggerInfo* FindTriggerInfo(WzID TriggerID);
private:
    //
    TRIGGER_INFO_LIST trigger_info_list_;
    TCHAR group_name_[_MAX_GROUP_NAME_LENGTH];
    //
    __DISABLE_COPY(TriggerGroupInfo);
};

//==================================================================================================

inline const TCHAR* TriggerGroupInfo::GroupName() const
{
    return group_name_;
}

inline const TRIGGER_INFO_LIST& TriggerGroupInfo::GetTriggerInfos() const
{
    return trigger_info_list_;
}

#endif //_SOLARTRIGGER_TRIGGERGROUPINFO_H
