#pragma once

#ifdef __PCBANG_IP_UPDATE

#include <map>
#include <MemoryPoolFactory.h>

class PCRoomIPList
{
private:
    typedef STLX_SET<DWORD>  SetPCRoomIP;
public:
    enum ListStatus {list_empty, list_loading, list_loaded};
    //�Ǿ��� ������ ����� map�� ��� ä�������� ���¸� Ȯ��

public:
    PCRoomIPList();
    void Clear();

    void SetListStatus(ListStatus list_status);
    ListStatus GetListStatus();

    void AddIPList(DWORD ip_value);
    bool IsPCROOMIP(DWORD ip_value);

    DWORD GetPCRoomGUID() {return pc_room_guid_;};
    void SetPCRoomGUID(DWORD pc_room_guid) {pc_room_guid_ = pc_room_guid;};

private:    

    SetPCRoomIP pc_room_ip_;
    ListStatus  list_status_;
    DWORD       pc_room_guid_;
};

class PCRoomIPManager
{
private:
    typedef STLX_DEQUE<PCRoomIPList*> QuePCRoomIPList;
    typedef util::CMemoryPoolFactory<PCRoomIPList>  PCROOMIPList_POOL;

public:
    PCRoomIPManager();
    ~PCRoomIPManager();

    PCRoomIPList* const CreatePCROOMIPList();
    PCRoomIPList* const GetPCROOMIPList();
    void RemovePCROOMList(PCRoomIPList* const pc_roomip_list);
    
private:
    QuePCRoomIPList pc_roomip_list_;
    PCROOMIPList_POOL pc_roomip_pool_;
};

#endif //__PCBANG_IP_UPDATE









