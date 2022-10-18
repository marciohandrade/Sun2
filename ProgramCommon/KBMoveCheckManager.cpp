#include "stdafx.h"
#include "./KBMoveCheckManager.h"

#include <StreamHashKey.h>
#include "PacketStruct_CG.h"
#include "PacketControl/WarMessages_Sync.h"

//==================================================================================================

nsSync::KBMoveCheckManager::~KBMoveCheckManager()
{
    if (client_send_tick_id_) {
        TAllocFree(client_send_tick_id_);
        client_send_tick_id_ = NULL;
    };
    if (client_send_tick_) {
        TAllocFree(client_send_tick_);
        client_send_tick_ = NULL;
    }
}

bool nsSync::KBMoveCheckManager::StartLocalTickTest()
{
    local_test_interval_.SetTimer(20 * 1000);
    return true;
}

//==================================================================================================

nsSync::ServerTimerSynchronier* nsSync::ServerTimerSynchronier::instance_static_ = NULL;

nsSync::ServerTimerSynchronier* nsSync::ServerTimerSynchronier::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ServerTimerSynchronier** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_)
            {
                TAllocDelete(ServerTimerSynchronier, *static_table_);
                *static_table_ = NULL;
            };
        }
        ServerTimerSynchronier** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ServerTimerSynchronier* provider = TAllocNew(ServerTimerSynchronier);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

// contorller for MSGSUB_SYNC_SERVER_TIMER_SYNCHRONIZE_BRD
bool nsSync::ServerTimerSynchronier::CheckServerTimerSynchronizationTest(
    const nsSync::MSGSUB_SYNC_SERVER_TIMER_SYNCHRONIZE_BRD& msg_brd)
{
    ServerTimerSynchronier* instance = instance_static_;
    if (instance == NULL) {
        return false;
    };
    ServerTimerSynchronier* realloc_object = TAllocNew(ServerTimerSynchronier);
    realloc_object->milestone_counter_.Move(&instance->milestone_counter_);
    realloc_object->timestamp_.Move(&instance->timestamp_);
    TAllocDelete(ServerTimerSynchronier, instance);
    instance = instance_static_ = realloc_object;

    ulong milestone_counter = instance->milestone_counter_.Get();
    if (milestone_counter == 0)
    {   // first received
        instance->milestone_counter_.Set(msg_brd.milestone_counter);
        instance->timestamp_.Set(::GetTickCount());
        return true;
    };
    int64_t timestamp = instance->timestamp_.Get();
    const ulong interval = (msg_brd.milestone_counter - milestone_counter) * (5 * 1000);
    ulong cur_tick = ::GetTickCount();
    int64_t compare_tick = cur_tick;
    if (compare_tick < timestamp) {
        compare_tick += ULONG_MAX;
    };
    const int64_t diff = compare_tick - timestamp;
    if (diff < (interval - (interval >> 2)) || diff > (interval + (interval >> 2))) {
        return false;
    };
    if ((msg_brd.milestone_counter - milestone_counter) > 100) {
        instance->milestone_counter_.Set(msg_brd.milestone_counter);
        instance->timestamp_.Set(cur_tick);
    };
    return true;
}

void nsSync::ServerTimerSynchronier::EnterField()
{
    milestone_counter_.Set(0);
    MemoryHackMonitor::Instance()->EnterField();
}

void nsSync::ServerTimerSynchronier::LeaveField()
{
    milestone_counter_.Set(0);
}

//==================================================================================================

nsSync::MemoryHackMonitor* nsSync::MemoryHackMonitor::instance_static_ = NULL;

nsSync::MemoryHackMonitor* nsSync::MemoryHackMonitor::Instance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(MemoryHackMonitor** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_)
            {
                TAllocDelete(MemoryHackMonitor, *static_table_);
                *static_table_ = NULL;
            };
        }
        MemoryHackMonitor** static_table_;
    } auto_closure(&instance_static_);
    //
    MemoryHackMonitor* provider = TAllocNew(MemoryHackMonitor);
    if (instance_static_ != NULL)
    {
        CopyMemory(provider, instance_static_, sizeof(*provider));
        TAllocDelete(MemoryHackMonitor, instance_static_);
    };
    // initialization...
    instance_static_ = provider;
    //
    return provider;
}

//==================================================================================================

nsSync::InorderValue::InorderValue()
{
    inorder_array_ = TAllocNew(InorderArray);
    uint8_t stream[sizeof(ulong)] = { 0, };
    inorder_array_->push_back(0);
    inorder_array_->push_back((void*)util::StreamHashKey::GetMemoryHashKey(stream, sizeof(stream)));
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        cast_type first_crc = static_cast<cast_type>(__int64(&first_part));
        cast_type second_crc = static_cast<cast_type>(__int64(&second_part));
        *((cast_type*)(&first_part)) ^= second_crc;
        *((cast_type*)(&second_part)) ^= first_crc;
    };
}

nsSync::InorderValue::~InorderValue()
{
    if (inorder_array_) {
        TAllocDelete(InorderArray, inorder_array_);
        inorder_array_ = NULL;
    };
}

void nsSync::InorderValue::Init()
{
    if (inorder_array_ != NULL) {
        return;
    };
    inorder_array_ = TAllocNew(InorderArray);
    uint8_t stream[sizeof(ulong)] = { 0, };
    inorder_array_->push_back(0);
    inorder_array_->push_back((void*)util::StreamHashKey::GetMemoryHashKey(stream, sizeof(stream)));
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        cast_type first_crc = static_cast<cast_type>(__int64(&first_part));
        cast_type second_crc = static_cast<cast_type>(__int64(&second_part));
        *((cast_type*)(&first_part)) ^= second_crc;
        *((cast_type*)(&second_part)) ^= first_crc;
    };
}

void nsSync::InorderValue::Realloc()
{
    if (inorder_array_ == NULL) {
        this->Init();
    };
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        cast_type first_crc = static_cast<cast_type>(__int64(&first_part));
        cast_type second_crc = static_cast<cast_type>(__int64(&second_part));
        *((cast_type*)(&first_part)) ^= second_crc;
        *((cast_type*)(&second_part)) ^= first_crc;
    };
    InorderArray* realloc_object = TAllocNew(InorderArray);
    FOREACH_CONTAINER(const InorderArray::value_type& node, (*inorder_array_), InorderArray)
    {
        realloc_object->push_back(node);
    };
    TAllocDelete(InorderArray, inorder_array_);
    inorder_array_ = realloc_object;
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        cast_type first_crc = static_cast<cast_type>(__int64(&first_part));
        cast_type second_crc = static_cast<cast_type>(__int64(&second_part));
        *((cast_type*)(&first_part)) ^= second_crc;
        *((cast_type*)(&second_part)) ^= first_crc;
    };
};

bool nsSync::InorderValue::Move(InorderValue* inorder_value)
{
    if (inorder_array_ == NULL) {
        this->Init();
    };
    if (this == inorder_value) {
        return false;
    };
    if (inorder_array_) {
        TAllocDelete(InorderArray, inorder_array_);
        inorder_array_ = NULL;
    };
    inorder_array_ = inorder_value->inorder_array_;
    inorder_value->inorder_array_ = NULL;
    this->Realloc();
    return true;
}

void nsSync::InorderValue::Swap(InorderValue* inorder_value)
{
    ulong value_r = 0;
    if (InorderArray* working = inorder_value->inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        const cast_type first_crc = static_cast<const cast_type>(const __int64(&first_part));
        const cast_type second_crc = static_cast<const cast_type>(const __int64(&second_part));
        //
        value_r = *((const cast_type*)(&first_part)) ^ second_crc;
        ulong checksum = *((const cast_type*)(&second_part)) ^ first_crc;
        ulong value_checksum = util::StreamHashKey::GetMemoryHashKey(&value_r, sizeof(value_r));
        if (checksum != value_checksum) {
            value_r = 0;
        }
    };
    ulong value_l = 0;
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        const cast_type first_crc = static_cast<const cast_type>(const __int64(&first_part));
        const cast_type second_crc = static_cast<const cast_type>(const __int64(&second_part));
        //
        value_l = *((const cast_type*)(&first_part)) ^ second_crc;
        ulong checksum = *((const cast_type*)(&second_part)) ^ first_crc;
        ulong value_checksum = util::StreamHashKey::GetMemoryHashKey(&value_l, sizeof(value_l));
        if (checksum != value_checksum) {
            value_l = 0;
        }
    };
    this->Set(value_r);
    inorder_value->Set(value_l);
}

ulong nsSync::InorderValue::Set(ulong value)
{
    if (inorder_array_ == NULL) {
        this->Init();
    };
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        cast_type first_crc = static_cast<cast_type>(__int64(&first_part));
        cast_type second_crc = static_cast<cast_type>(__int64(&second_part));
        //
        *((cast_type*)(&first_part)) = value;
        *((cast_type*)(&second_part)) =
            util::StreamHashKey::GetMemoryHashKey(&value, sizeof(value));
        *((cast_type*)(&first_part)) ^= second_crc;
        *((cast_type*)(&second_part)) ^= first_crc;
    };
    this->Realloc();

    return value;
}

ulong nsSync::InorderValue::Set(const InorderValue& rhs)
{
    ulong value = 0;
    if (InorderArray* working = rhs.inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        const cast_type first_crc = static_cast<const cast_type>(const __int64(&first_part));
        const cast_type second_crc = static_cast<const cast_type>(const __int64(&second_part));
        //
        value = *((const cast_type*)(&first_part)) ^ second_crc;
        ulong checksum = *((const cast_type*)(&second_part)) ^ first_crc;
        ulong value_checksum = util::StreamHashKey::GetMemoryHashKey(&value, sizeof(value));
        if (checksum != value_checksum) {
            value = 0;
        }
    };
    return this->Set(value);
}

ulong nsSync::InorderValue::Get()
{
    if (inorder_array_ == NULL) {
        this->Init();
    };
    this->Realloc();
    if (InorderArray* working = inorder_array_)
    {
        void*& first_part = working->front();
        void*& second_part = working->back();
        //
        const cast_type first_crc = static_cast<const cast_type>(const __int64(&first_part));
        const cast_type second_crc = static_cast<const cast_type>(const __int64(&second_part));
        //
        ulong value =    *((const cast_type*)(&first_part)) ^ second_crc;
        ulong checksum = *((const cast_type*)(&second_part)) ^ first_crc;
        ulong value_checksum = util::StreamHashKey::GetMemoryHashKey(&value, sizeof(value));
        if (checksum == value_checksum) {
            return value;
        }
    };
    return ulong();
}

//==================================================================================================

#ifdef _SERVER

//
void nsSync::KBMoveCheckManagerInServer::EnterField(
    MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd)
{
    ZeroMemory(this, sizeof(*this));
    serial_issue_timer.SetTimer(5 * 1000);
    client_local_test_timer.Clear();
    active_node = &kbmove_nodes[0];
    active_node->Create();

    msg_cmd->serial_block = *active_node;
    // NOTE: fake.
    active_node = &kbmove_nodes[1];
    kbmove_state = this->eKBMoveKey_Issued;
}

// NOTE: f110801.2L, abusing check routine in client
bool nsSync::KBMoveCheckManagerInServer::CheckClientBattleSerial(ulong client_battle_serial)
{
    if (client_battle_serial_failed != false) {
        // process once
        return true;
    };
    ++client_battle_serial_counter;
    if (client_battle_serial_counter == client_battle_serial) {
        return true;
    };
    client_battle_serial_failed_pending_timer.SetTimer(30 * 1000);
    client_battle_serial_failed = true;
    return false;
}

bool nsSync::KBMoveCheckManagerInServer::ReceiveReply(
    const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK& reply_msg)
{
    if (active_node == NULL) {
        return false;
    };
    if (kbmove_state != eKBMoveKey_Issued) {
        return false;
    };
    KBMoveSerialNode* next_node = (active_node != &kbmove_nodes[0]) ?
        &kbmove_nodes[0] : &kbmove_nodes[1];
    if (next_node->serial_guid != reply_msg.serial_guid) {
        return false;
    };
    active_node = next_node;
    kbmove_state = this->eKBMoveKey_Used;
    return true;
}

bool nsSync::KBMoveCheckManagerInServer::EnabledMoving(ulong received_serial_key) const
{
    if (active_node == NULL) {
        return false;
    };
    if (active_node->process_index >= active_node->kNumberOfMoveKeys) {
        return false;
    };
    if ((active_node->serial_key_begin + active_node->process_index) !=
        (received_serial_key))
    {
        return false;
    };
    ++active_node->process_index;
    return true;
}

bool nsSync::KBMoveCheckManagerInServer::EnabledStopping(ulong received_stop_serial_key) const
{
    if (active_node == NULL) {
        return false;
    };
    if (active_node->process_stop_index >= active_node->kNumberOfStopKeys) {
        return false;
    };
    if ((active_node->stop_serial_key_begin + active_node->process_stop_index) !=
        (received_stop_serial_key))
    {
        return false;
    };
    ++active_node->process_stop_index;
    return true;
}

bool nsSync::KBMoveCheckManagerInServer::UpdateKBMoveCheckRefresh(
    MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd)
{
    if (active_node == NULL) {
        return false;
    };
    if (kbmove_state == this->eKBMoveKey_None) {
        return false;
    }
    else if (kbmove_state == this->eKBMoveKey_Issued) {
        return false;
    };
    if (serial_issue_timer.IsExpired() == false) {
        return false;
    };
    //
    KBMoveSerialNode* next_node = (active_node != &kbmove_nodes[0]) ?
        &kbmove_nodes[0] : &kbmove_nodes[1];
    next_node->Create();
    msg_cmd->serial_block = *next_node;
    kbmove_state = this->eKBMoveKey_Issued;
    return true;
}

bool nsSync::KBMoveCheckManagerInServer::ReceiveClientLocalTest(
    const MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN& test_msg)
{
    if (received_client_local_tick != 0) {
        return false;
    };
    client_local_test_timer.SetTimer(10 * 1000);
    received_tick_in_server = GetTickCount();
    received_client_local_tick_id = test_msg.client_send_tick_id;
    received_client_local_tick = test_msg.client_send_tick;
    return true;
}

bool nsSync::KBMoveCheckManagerInServer::UpdateClientLocatTestRefresh(
    MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK* const reply_msg)
{
    if (client_local_test_timer.IsExpired() == false) {
        return false;
    };
    client_local_test_timer.Clear();
    if (this->received_client_local_tick == 0) {
        return false;
    };
    const ulong cur_tick = GetTickCount();
    reply_msg->client_send_tick_id = this->received_client_local_tick_id;
    reply_msg->client_send_tick = this->received_client_local_tick;
    if (received_tick_in_server <= cur_tick)
    {
        reply_msg->server_elapsed_tick = cur_tick - received_tick_in_server;
    }
    else
    {
        int64_t temp = cur_tick;
        ;       temp += ULONG_MAX;
        reply_msg->server_elapsed_tick = static_cast<ulong>(temp - received_tick_in_server);
    };
    this->received_client_local_tick_id = 0;
    this->received_client_local_tick = 0;
    return true;
}

#endif //_SERVER

//==================================================================================================
