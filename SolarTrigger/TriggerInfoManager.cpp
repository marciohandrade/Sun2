#include "StdAfx.h"
#include "./TriggerInfoManager.h"
#include "TriggerGroupInfo.h"

TriggerInfoManager::TriggerInfoManager()
{
}

TriggerInfoManager::~TriggerInfoManager()
{
    Release();
}

void TriggerInfoManager::Release()
{
    FOREACH_CONTAINER(const TRIGGER_GROUPINFO_MAP::value_type& group_node,
                      trigger_group_info_table_, TRIGGER_GROUPINFO_MAP)
    {
        TriggerGroupInfo* trigger_group_info = group_node.second;
        if (trigger_group_info != NULL) {
            TAllocDelete(TriggerGroupInfo, trigger_group_info);
        };
    }

    trigger_group_info_table_.clear();
}

BOOL TriggerInfoManager::InsertTriggerGroupInfo(TriggerGroupInfo* trigger_group_info)
{
    if (trigger_group_info == NULL) {
        return false;
    };
    const TCHAR* group_name = trigger_group_info->GroupName();
    if (group_name == NULL || this->FindTriggerGruopInfo(group_name)) {
        return false;
    };
    trigger_group_info_table_.insert(\
        TRIGGER_GROUPINFO_MAP::value_type(group_name, trigger_group_info));
    return true;
}

TriggerGroupInfo*
TriggerInfoManager::FindTriggerGruopInfo(const TCHAR* group_name) const
{
    if (group_name == NULL) {
        return NULL;
    };
    TRIGGER_GROUPINFO_MAP::const_iterator it = trigger_group_info_table_.find(group_name);
    if (it != trigger_group_info_table_.end())
    {
        TriggerGroupInfo* trigger_group_info = it->second;
        return trigger_group_info;
    }
    return 0;
}

void TriggerInfoManager::LoadGroupInfos(CWzArchive& IN archive,
    IEventRegisterForNpcKillByTrigger* event_register)
{
    __TOUCHED(event_register);
    this->CheckChunk(archive);

    // 그룹의 개수
    WORD wNumberOfGroups = 0;
    archive >> wNumberOfGroups;
    //#ifndef   __WAVERIX_UPDATE_ENGINE__
    INT iLastIDGroupIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDGroupIndex;
    }

#if defined(_SERVER)
    event_register_ = event_register;
    for (WORD idx = 0; idx < wNumberOfGroups; ++idx)
    {
        TriggerGroupInfo* trigger_group_info = TAllocNew(TriggerGroupInfo);
        trigger_group_info->Load(archive, this);
        if (InsertTriggerGroupInfo(trigger_group_info) == false) {
            TAllocDelete(TriggerGroupInfo, trigger_group_info);
        };
    }
    event_register_ = NULL;
#else
    for (WORD idx = 0; idx < wNumberOfGroups; ++idx)
    {
        TriggerGroupInfo* trigger_group_info = TAllocNew(TriggerGroupInfo);
        trigger_group_info->Load(archive);
        if (InsertTriggerGroupInfo(trigger_group_info) == false) {
            TAllocDelete(TriggerGroupInfo, trigger_group_info);
        };
    }
#endif
}


void
TriggerInfoManager::CheckChunk(CWzArchive& IN archive)
{
    St_ChunkInfo chunk_info;
    if (archive.GetVersion() < 145) {
        return;
    };

    BOOL bTriggerChunk = false;
    while(true)
    {
        chunk_info = archive.ReadChunk();
        if (chunk_info.m_nID == 0x1786)
        {
            bTriggerChunk = true;
            break;
        }
        archive.SkipCurrentChunk(chunk_info);
    }
    ASSERT(bTriggerChunk);
}

