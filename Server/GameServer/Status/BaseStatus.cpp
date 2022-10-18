#include "stdafx.h"
#include "BaseStatus.h"
#include "Character.h"
#include <PacketControl/WarMessages_Sync.h>

void BaseStatus::Init(Character* owner, WORD state_id, int application_time, int period)
{
    owner_ = owner;
    requested_remove_ = false; // (f100309.2L)
    state_id_ = state_id;
    application_time_ = application_time;
    period_ = period;
    excute_time_ = GetTickCount();

    if(application_time == BASE_EXPIRE_TIME_INFINITY)
    {
        expire_time_ = BASE_EXPIRE_TIME_INFINITY;
    }
    else
    {   
        // (CHANGES) (WAVERIX) 'GetTickCount()' -> 'excute_time_' : valid logic condition
        expire_time_ = excute_time_ + application_time;
    }
}

BOOL BaseStatus::Update(DWORD cur_time)
{
    // (f100309.2L) (WAVERIX) changes remove condition, using this instead of expired time
    if (requested_remove_) 
    {
        return false;
    }
    if (IsExpired(cur_time))
    {
        return FALSE;
    }
    if (IsExecuteTime(cur_time))
    {
        Execute();
    }
        
    return TRUE;
}

void BaseStatus::Execute()
{
    excute_time_ = GetTickCount() + period_;
}

int BaseStatus::GetLeavedTime() const
{
    const DWORD cur_tick = GetTickCount();
    if (IsExpired(cur_tick))
    {
        return 0;
    }
    else if (expire_time_ == BASE_EXPIRE_TIME_INFINITY)
    {
        return BASE_EXPIRE_TIME_INFINITY;
    }
    else
    {
        return (int)(expire_time_ - cur_tick);
    }        
}

BOOL BaseStatus::SendStatusAddBRD()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        MSGSUB_SYNC_STATUS_ADD_BRD add_brd;
        add_brd.m_dwTargetKey = owner->GetObjectKey();
        add_brd.m_wStatusCode = state_id_;
        owner->SendPacketAroundForSync(&add_brd, TRUE);
        return TRUE;
    }
    return FALSE;
}

BOOL BaseStatus::SendStatusDelBRD()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        MSGSUB_SYNC_STATUS_REMOVE_BRD remove_brd;
        remove_brd.m_dwTargetKey = owner->GetObjectKey();
        remove_brd.m_wStatusCode = state_id_;
        owner->SendPacketAroundForSync(&remove_brd, TRUE);
        return TRUE;
    }
    return FALSE;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
VOID BaseStatus::ResetStatus()
{
    requested_remove_ = false; // (f100309.2L)
    excute_time_ = GetTickCount();

    if(application_time_ == BASE_EXPIRE_TIME_INFINITY) {
        expire_time_ = BASE_EXPIRE_TIME_INFINITY;
    }
    else {   
        expire_time_ = excute_time_ + application_time_;
    }
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL