#pragma once
#ifndef WORLDSERVER_SYSTEM_MEMO_H
#define WORLDSERVER_SYSTEM_MEMO_H

//WorldSystemMemo.h - 시스템 메모와 관련된 클레스

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

namespace ns_world {
;

class WorldSystemMemo
{
public:
    WorldSystemMemo();
    ~WorldSystemMemo();

    // NOTE: f110819.5L, added the system memo type acquisition interface
    static const TCHAR* GetSystemMemoStringByType(const eSYSTEM_MEMO_TYPE memo_type);
    static bool IsMemoTransaction(ns_world::WorldUser* user);
    static bool StartMemoTransaction(ns_world::WorldUser* user);
    static bool EndMemoTransaction(ns_world::WorldUser* user);
    static bool EnabledMemoListRequest(ns_world::WorldUser* user);

    void SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, DWORD memo_info_list_num,
                        eSYSTEM_MEMO_TYPE system_memo_type);

private:
    //-- DB로 시스템 메모를 보낸다.
    void SendSystemMemoOfLotto(const SystemMemoInfoOfLotto* memo_info_list, DWORD list_num);
    void SendSystemMemoToDBProxy(const BasicSystemMemoInfo* memo_info_list, DWORD list_num);
};

}; //end of namespace

//==================================================================================================

inline ns_world::WorldSystemMemo::WorldSystemMemo()
{
}

inline ns_world::WorldSystemMemo::~WorldSystemMemo()
{
}

//==================================================================================================

#endif //WORLDSERVER_SYSTEM_MEMO_H