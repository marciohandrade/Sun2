#include "stdafx.h"

#ifdef __PCBANG_IP_UPDATE

#include "PCRoomIpManager.h"

PCRoomIPList::PCRoomIPList()
{
    list_status_ = list_empty;
}

void 
PCRoomIPList::Clear()
{
    pc_room_ip_.clear();
    SetListStatus(list_empty);
};

void 
PCRoomIPList::SetListStatus(ListStatus list_status)
{
    list_status_ = list_status;
}

PCRoomIPList::ListStatus 
PCRoomIPList::GetListStatus()
{
    return list_status_;
}

void 
PCRoomIPList::AddIPList(DWORD ip_value)
{
    pc_room_ip_.insert(ip_value);
}

bool
PCRoomIPList::IsPCROOMIP(DWORD ip_value)
{
    SetPCRoomIP::iterator iter = pc_room_ip_.find(ip_value);
    if(pc_room_ip_.end() == iter)
        return false;

    return true;
}


PCRoomIPManager::PCRoomIPManager()
{
    pc_roomip_pool_.Initialize(10, 10);
}

PCRoomIPManager::~PCRoomIPManager()
{
    pc_roomip_list_.clear();
    pc_roomip_pool_.Release();
}

PCRoomIPList* const PCRoomIPManager::CreatePCROOMIPList()
{
    PCRoomIPList* const pc_roomip_list = (PCRoomIPList*)pc_roomip_pool_.Alloc();

    pc_roomip_list_.push_back(pc_roomip_list);    

    return pc_roomip_list;
}

PCRoomIPList* const PCRoomIPManager::GetPCROOMIPList()
{
    if(pc_roomip_list_.empty())
        return NULL;

    PCRoomIPList* const pc_roomip_list = pc_roomip_list_.front();
    if(pc_roomip_list->GetListStatus() == PCRoomIPList::list_loaded)
    {
        pc_roomip_list_.pop_front();
        return pc_roomip_list;
    }
    return NULL;
    
}

void PCRoomIPManager::RemovePCROOMList(PCRoomIPList* const pc_roomip_list)
{
    if(pc_roomip_list)
    {
        pc_roomip_list->Clear();
        pc_roomip_pool_.Free(pc_roomip_list);
    }    
}

#endif //__PCBANG_IP_UPDATE

