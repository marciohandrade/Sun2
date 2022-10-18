#include "stdafx.h"
#include "./TriggerGroupInfo.h"

#include <ProgramCommon/WzArchive.h>

#include "./TriggerInfo.h"

//==================================================================================================

TriggerGroupInfo::TriggerGroupInfo()
{
}

TriggerGroupInfo::~TriggerGroupInfo()
{
    Release();
}

void TriggerGroupInfo::Release()
{
    FOREACH_CONTAINER(const TRIGGER_INFO_LIST::value_type& info_node,
                      trigger_info_list_, TRIGGER_INFO_LIST)
    {
        TriggerInfo* trigger_info = info_node;
        TAllocDelete(TriggerInfo, trigger_info);
    }
    trigger_info_list_.clear();
}


void TriggerGroupInfo::Load(CWzArchive& IN archive, TriggerInfoManager* trigger_info_manager)
{
    // 그룹 이름
    archive.ReadName(group_name_);

    // 카테고리 개수
    WORD wNumberOfCategorys = 0;
    WORD wNumberOfTriggers = 0;
    TCHAR pszCategoryName[0xFF] = {0,};
    archive >> wNumberOfCategorys;

    INT iLastIDCategoryIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDCategoryIndex;
    }

    for (WORD i = 0 ; i < wNumberOfCategorys ; ++i)
    {
        // 카테고리 정보
        archive.ReadName(pszCategoryName);
        archive >> wNumberOfTriggers;
        INT iLastIDTriggerIndex = 0;
        if (159 < archive.GetVersion())
        {
            archive >> iLastIDTriggerIndex;
            //ASSERT(iLastIDTriggerIndex >= UN_USABLE_WZID);
        }

        for (WORD j = 0 ; j < wNumberOfTriggers ; ++j)
        {
            TriggerInfo* trigger_info = TAllocNew(TriggerInfo);
            trigger_info->_setCategoryName(pszCategoryName);
#ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
            trigger_info->_setGroupName(group_name_);
#endif //
            trigger_info->Load(archive, trigger_info_manager);
            if (FindTriggerInfo(trigger_info->TriggerID()))
            {
                TAllocDelete(TriggerInfo, trigger_info);
                continue;
            }
            else
            {
                trigger_info_list_.push_back(trigger_info);
            };
        }
    }
}

TriggerInfo* TriggerGroupInfo::FindTriggerInfo(WzID triggerID)
{
    FOREACH_CONTAINER(const TRIGGER_INFO_LIST::value_type& info_node,
                      trigger_info_list_, TRIGGER_INFO_LIST)
    {
        TriggerInfo* trigger_info = info_node;
        if (triggerID == trigger_info->TriggerID()) {
            return trigger_info;
        };
    }
    return 0;
}



