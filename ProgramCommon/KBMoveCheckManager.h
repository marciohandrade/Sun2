#pragma once
#ifndef PROGRAMCOMMON_KBMOVECHECK_MANAGER_H
#define PROGRAMCOMMON_KBMOVECHECK_MANAGER_H

#include <ITimeout.h>
#include "PacketStruct_CG.h"

//==================================================================================================

//==================================================================================================
// the reason of 'TAllocMalloc' and 'TAllocFree'
// to support other memory acquisition

namespace nsSync {
;

struct MSGSUB_SYNC_SERVER_TIMER_SYNCHRONIZE_BRD;
//

class KBMoveCheckManager
{
public:
    KBMoveCheckManager();
    virtual ~KBMoveCheckManager();
    //
    bool ReceiveOrders(const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD& command,
                       MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK* const reply_msg);
    bool IssueNextSerial(ulong* const next_serial_key);
    bool IssueNextStopSerial(ulong* const next_stop_serial_key);
    //
    bool StartLocalTickTest();
    template<size_t LineNo>
    bool CheckLocalTestRequest(MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN* const msg_syn);
    //
    ulong GetClientSendTick() const;
    bool CheckReceivedLocalTest(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg);
    virtual bool ReceiveReplyFromServer(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg) = 0;
private:
    //
    util::ITimerBase local_test_interval_;
    nsSync::KBMoveSerialNode kbmove_node_;
    ulong* client_send_tick_id_;
    ulong* client_send_tick_;
    ulong* server_timer_selected_index_;
    ulong* server_timer_;
    //
    __DISABLE_COPY(KBMoveCheckManager);
};

class InorderValue
{
public:
    InorderValue();
    ~InorderValue();
    ulong Set(ulong value);
    ulong Set(const InorderValue& rhs);
    ulong Get();
    void Realloc();
    bool Move(InorderValue* inorder_value);
    void Swap(InorderValue* inorder_value);
private:
    typedef mpl::if_c<sizeof(void*) == sizeof(ulong), ulong,
            mpl::if_c<sizeof(void*) == sizeof(int64), int64,
                      void>::type >::type cast_type;
    typedef STLX_LIST<void*> InorderArray;
    void Init();
    //
    InorderArray* inorder_array_;
    //
    __DISABLE_COPY(InorderValue);
};

class ServerTimerSynchronier
{
public:
    static const ulong kServerTimerCrc = ulong( ((PROTOCOL_GENERATOR_CHARINFO_041 << 24))
                                              | ((PROTOCOL_GENERATOR_MAP_008 << 16))
                                              | ((PROTOCOL_GENERATOR_SYNC_018 << 8))
                                              | ((PROTOCOL_GENERATOR_SYNC_099)) );
    static ServerTimerSynchronier* Instance();
    void EnterField(); // client
    void LeaveField(); // client
    // contorller for MSGSUB_SYNC_SERVER_TIMER_SYNCHRONIZE_BRD
    static bool CheckServerTimerSynchronizationTest(
        const nsSync::MSGSUB_SYNC_SERVER_TIMER_SYNCHRONIZE_BRD& msg_brd);
private:
    //
    ServerTimerSynchronier();
    ~ServerTimerSynchronier();
    static ServerTimerSynchronier* CreateInstance();
    // fields...
    InorderValue milestone_counter_;
    InorderValue timestamp_;
    // last order...
    static ServerTimerSynchronier* instance_static_;
    __DISABLE_COPY(ServerTimerSynchronier);
};

class MemoryHackMonitor
{
public:
    static MemoryHackMonitor* Instance();
    void EnterField(); // client
    ulong GetNextClientTestSerial();
    ulong GetCurrentClientTestSerial();
    void ForceIncreaseClientTestSerial();
private:
    MemoryHackMonitor();
    ~MemoryHackMonitor();
    static MemoryHackMonitor* CreateInstance();

#pragma pack(push, 1)
    uint16_t __dummy0;
    struct BattleSerial {
        uint16_t __dummy0;
        ulong serial_counter;
        uint16_t __dummy1;
    } battle_serial_;
    uint16_t __dummy1;
#pragma pack(pop)
    // last order...
    static MemoryHackMonitor* instance_static_;
    __DISABLE_COPY(MemoryHackMonitor);
};

#ifdef _SERVER
struct KBMoveCheckManagerInServer
{
    enum eKBMoveKey {
        eKBMoveKey_None = 0,
        eKBMoveKey_Issued,
        eKBMoveKey_Used,
    };

    static void Initialize(KBMoveCheckManagerInServer* manager);
    //
    void EnterField(MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd);
    bool ReceiveReply(const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK& reply_msg);
    bool EnabledMoving(ulong received_serial_key) const;
    bool EnabledStopping(ulong received_stop_serial_key) const;
    // NOTE: f110801.2L, abusing check routine in client
    bool CheckClientBattleSerial(ulong client_battle_serial);
    bool UpdateKBMoveCheckRefresh(MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd);
    //
    bool ReceiveClientLocalTest(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN& test_msg);
    bool UpdateClientLocatTestRefresh(MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK* const reply_msg);
    //
    util::ITimerBase serial_issue_timer; // change type
    util::ITimerBase client_local_test_timer; // change type
    eKBMoveKey kbmove_state;
    KBMoveSerialNode* active_node;
    KBMoveSerialNode kbmove_nodes[2];
    bool client_battle_serial_failed;
    ulong client_battle_serial_counter;
    util::ITimerBase client_battle_serial_failed_pending_timer;
    ulong received_tick_in_server;
    ulong received_client_local_tick_id;
    ulong received_client_local_tick;
};
#endif

}; //end of namespace

//==================================================================================================
// code spreading for hack prevention

__forceinline nsSync::KBMoveCheckManager::KBMoveCheckManager()
{
    ZeroMemory(&kbmove_node_, sizeof(kbmove_node_));
    client_send_tick_id_ = static_cast<ulong*>(TAllocMalloc(sizeof(ulong)));
    client_send_tick_ = static_cast<ulong*>(TAllocMalloc(sizeof(ulong)));
    *client_send_tick_ = 0;
}

__forceinline bool nsSync::KBMoveCheckManager::ReceiveOrders(
    const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD& command,
    MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK* const reply_msg)
{
    kbmove_node_ = command.serial_block;
    reply_msg->serial_guid = kbmove_node_.serial_guid;
    return true;
}

__forceinline bool nsSync::KBMoveCheckManager::IssueNextSerial(ulong* const next_serial_key)
{
    if (kbmove_node_.process_index >= kbmove_node_.kNumberOfMoveKeys) {
        return false;
    };
    *next_serial_key = kbmove_node_.serial_key_begin + kbmove_node_.process_index;
    ++kbmove_node_.process_index;
    return true;
}

__forceinline bool nsSync::KBMoveCheckManager::IssueNextStopSerial(
    ulong* const next_stop_serial_key)
{
    if (kbmove_node_.process_stop_index >= kbmove_node_.kNumberOfStopKeys) {
        return false;
    };
    *next_stop_serial_key = kbmove_node_.stop_serial_key_begin + kbmove_node_.process_stop_index;
    ++kbmove_node_.process_stop_index;
    return true;
}

inline ulong nsSync::KBMoveCheckManager::GetClientSendTick() const
{
    return (*client_send_tick_) ^ (*client_send_tick_id_);
}

__forceinline bool nsSync::KBMoveCheckManager::CheckReceivedLocalTest(
    const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg)
{
    if (client_send_tick_id_ == NULL || client_send_tick_ == NULL) {
        return false;
    };
    if (*client_send_tick_id_ != reply_msg.client_send_tick_id) {
        return false;
    };
    return true;
}

template<size_t LineNo>
__forceinline bool nsSync::KBMoveCheckManager::CheckLocalTestRequest(
    MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN* const msg_syn)
{
    if (local_test_interval_.IsExpired() == false) {
        return false;
    };
#pragma warning(push)
#pragma warning(disable : 4307 4308)
    const ulong kBuildSeed = ulong( ((PROTOCOL_GENERATOR_CHARINFO_040 << 24) + LineNo)
                                  | ((PROTOCOL_GENERATOR_MAP_004 << 16) + LineNo)
                                  | ((PROTOCOL_GENERATOR_SYNC_008 << 8) + LineNo)
                                  | ((PROTOCOL_GENERATOR_SYNC_083) + LineNo) );
    const ulong kIncSeed = ulong( ((PROTOCOL_GENERATOR_BATTLE_002 << 8) + LineNo)
                                | ((PROTOCOL_GENERATOR_STYLE_011) + LineNo) ) + 199;
#pragma warning(pop)
    TAllocFree(client_send_tick_id_);
    TAllocFree(client_send_tick_);
    client_send_tick_id_ = static_cast<ulong*>(TAllocMalloc(sizeof(ulong)));
    client_send_tick_ = static_cast<ulong*>(TAllocMalloc(sizeof(ulong)));
    //
    const ulong cur_tick = GetTickCount();
    *client_send_tick_id_ = ((ulong(__int64(client_send_tick_id_)) >> 2) ^ kBuildSeed);
    *client_send_tick_id_ += RandomNumberGenerator::GetRandomNumberRange(1, kIncSeed);
    *client_send_tick_ = cur_tick ^ *client_send_tick_id_;
    msg_syn->client_send_tick_id = *client_send_tick_id_;
    msg_syn->client_send_tick = *client_send_tick_;
    return true;
}

//==================================================================================================

inline nsSync::ServerTimerSynchronier* nsSync::ServerTimerSynchronier::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

__forceinline nsSync::ServerTimerSynchronier::ServerTimerSynchronier()
{
}

__forceinline nsSync::ServerTimerSynchronier::~ServerTimerSynchronier()
{
}

//==================================================================================================

__forceinline nsSync::MemoryHackMonitor::MemoryHackMonitor()
{
    battle_serial_.serial_counter = 0;
}

__forceinline nsSync::MemoryHackMonitor::~MemoryHackMonitor()
{
}

inline void nsSync::MemoryHackMonitor::EnterField() // client
{
    battle_serial_.serial_counter = 0;
}

inline ulong nsSync::MemoryHackMonitor::GetNextClientTestSerial()
{
    return ++battle_serial_.serial_counter;
}

inline ulong nsSync::MemoryHackMonitor::GetCurrentClientTestSerial()
{
    return battle_serial_.serial_counter;
}

__forceinline void nsSync::MemoryHackMonitor::ForceIncreaseClientTestSerial()
{
    int64_t copied = *(int64_t*)&battle_serial_;
    BattleSerial copied2 = *(BattleSerial*)&copied;
    ++copied2.serial_counter;
    battle_serial_ = copied2;
}

//==================================================================================================

#ifdef _SERVER

inline void nsSync::KBMoveCheckManagerInServer::Initialize(
    nsSync::KBMoveCheckManagerInServer* manager)
{
    ZeroMemory(manager, sizeof(*manager));
}

#endif //_SERVER
//==================================================================================================

#endif //PROGRAMCOMMON_KBMOVECHECK_MANAGER_H