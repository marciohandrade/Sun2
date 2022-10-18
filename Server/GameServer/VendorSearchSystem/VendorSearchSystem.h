#ifndef _SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHSYSTEM_H
#define _SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHSYSTEM_H

#include <queue>
#include "VendorSearch_PacketStruct.h"

class MutualEx
{
    CRITICAL_SECTION critical_section_;

public:
    MutualEx()
    {
        InitializeCriticalSection(&critical_section_);
    };

    ~MutualEx()
    {
        DeleteCriticalSection(&critical_section_);
    };

    void enter()
    {
        EnterCriticalSection(&critical_section_);
    };

    void leave()
    {
        LeaveCriticalSection(&critical_section_);
    };
};

class AutoLock
{
    MutualEx* mutual_ex_;
    
public:
    explicit AutoLock(MutualEx& lock)
        : mutual_ex_(&lock)
    {
        if (mutual_ex_)
            mutual_ex_->enter();
    };

    ~AutoLock()
    {
        if (mutual_ex_)
            mutual_ex_->leave();
    };
};

struct sqlite3;

class VendorSearchSystem
{
    enum { MAX_QUERY_SIZE = 1024 };

    sqlite3 *db;
    
    struct 
    {
        volatile bool destroy_thread_;
        HANDLE search_thread_;
        UINT   search_thread_id_;
        std::deque<VENDOR_SEARCH_BUFFER*>	send_queue_;
        std::deque<VENDOR_SEARCH_BUFFER*>	recv_queue_;
        MutualEx send_lock_;
        MutualEx recv_lock_;
        util::SafeMemoryPoolFactory<VENDOR_SEARCH_BUFFER> message_buffer_;
    };

public:
    VendorSearchSystem();
    ~VendorSearchSystem();

    static UINT WINAPI SearchThread(PVOID pvParam);
    void SendToVendorSearchSystem(PVOID packet, size_t size);
    bool RecvFromVendorManager(PVOID buffer);
    void SendToVendorManager(PVOID packet, size_t size);
    bool RecvFromVendorSearchSystem(PVOID buffer);

    bool initialize();
    void close();

    int  execute(const wchar_t* query);

    void insert(DWORD guid, const char* item_name, /*POSTYPE*/BYTE pos, /*MONEY*/ULONGLONG price);
    bool select(DWORD guid, const char* search_name, int page);
    void erase(DWORD guid, BYTE pos);

    bool select_intro_page(DWORD guid, int page_type);

    // test
    void print_sql_result(const char* query);
};

#endif //_SERVER_GAMESERVER_VENDORSEARCHSYSTEM_VENDORSEARCHSYSTEM_H