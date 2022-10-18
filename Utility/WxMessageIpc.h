#pragma once
#ifndef UTILITY_WX_MESSAGE_IPC_H
#define UTILITY_WX_MESSAGE_IPC_H

#include "WxRingBuffer.h"

//==============================================================================
// HISTORY:
//  f110830.2L, waverix, version 0.1.0.0, created to support the game shop.
//
#pragma pack(push)
#pragma pack()
namespace util {
;
struct WX_INTER_MSG_HEADER;
struct IWxMainMessageHandler;
struct IWxWorkerMessageHandler;
class WxMessageIpc;


struct WX_INTER_MSG_HEADER
{
    ulong msg_size; // with WX_INTER_MSG_HEADER
    ulong protocol; // configuration

    WX_INTER_MSG_HEADER() : msg_size(0), protocol(0) { };
};

// NOTE: f110830.2L,
// design concept
//  SingleReader : SR
//  SingleWriter : SW
//  MultiWriter : MW
//
//  MainThread <- WxRingBuffer<SR, MW> <- module threads
//  MainThread -> WxRingBuffer<SR, SW> -> message io thread (in WxMessageIpc)
//  message io thread (in WxMessageIpc) -> module threads
//
class WxMessageIpc
{
public:
    static const ulong kVersion = 0x0100;
    enum eReactingLevel {
        eReacting_None = 0,
        eReacting_Slow,     // 10000 milli-seconds (10s)
        eReacting_Medium,   //  1000 milli-seconds ( 1s)
        eReacting_Fast,     //   500 milli-seconds
        eReacting_MoreFast, //   100 milli-seconds
        eReacting_TooFast,  //    16 milli-seconds
        // NOTE: this same with eReacting_TooFast on high-load state
        eReacting_Realtime, //     1 milli-seconds,
        eReacting_Counts
    };
    static const ulong kMaxMessageSize = 60 * 1024;
    typedef util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>
        WorkersToMainGateway;
    typedef util::WxRingBuffer MainToMessageIoGateway;
#pragma warning(push)
#pragma warning(disable: 4512)
    struct WorkerHandler
    {
        WorkerHandler();
        WorkerHandler(WxMessageIpc* root);
        bool SendMessageToMain(
            WX_INTER_MSG_HEADER* msg, size_t msg_size) const;
        //
        WxMessageIpc* const root_;
    };

    struct MainLogicHandler
    {
        MainLogicHandler();
        MainLogicHandler(WxMessageIpc* root);
        bool SendMessageToWorkers(
            WX_INTER_MSG_HEADER* msg, size_t msg_size) const;
        void Update(IWxMainMessageHandler* const receiver) const;
        //
        WxMessageIpc* const root_;
    };
#pragma warning(pop)
    //--------------------------------------------------------------------------

    WxMessageIpc(size_t max_worker_to_main_buffer_size,
                size_t max_main_to_worker_buffer_size,
                size_t max_message_size);
    virtual ~WxMessageIpc();

    virtual bool StartWorking(
        eReactingLevel message_pumping_level,
        IWxWorkerMessageHandler* worker_message_handler);
    void StopWorking();

    const WorkerHandler GetWorkerHandler() const;
    const MainLogicHandler GetMainLogicHandler() const;
    //--------------------------------------------------------------------------
private:
    //
    static unsigned int __stdcall message_io_thread(void* passing_parameter);
    bool ParseMessagesFromMain(WorkerHandler* const worker);
    bool ParseMessagesFromWorkers(MainLogicHandler* const main_logic,
        IWxMainMessageHandler* const receiver);
    //
    HANDLE message_io_thread_;
    HANDLE wakeup_event_;
    long serviced_;
    long stop_requested_;
    eReactingLevel message_pumping_level_;
    MainToMessageIoGateway* main_to_message_io_gateway_;
    WorkersToMainGateway* workers_to_main_gateway_;
    IWxWorkerMessageHandler* worker_message_handler_;
    size_t max_worker_to_main_buffer_size_;
    size_t max_main_to_worker_buffer_size_;
    size_t max_message_size_;
    //
    friend struct WorkerHandler;
    friend struct MainLogicHandler;
    friend struct MessageIoHandler;
    __DISABLE_COPY(WxMessageIpc);
};

struct IWxMainMessageHandler
{
    virtual bool OnReceive(
        util::WxMessageIpc::MainLogicHandler* const handler,
        const util::WX_INTER_MSG_HEADER* msg_base, size_t msg_size) = 0;
};

struct IWxWorkerMessageHandler
{
    virtual bool OnReceive(
        util::WxMessageIpc::WorkerHandler* const handler,
        const util::WX_INTER_MSG_HEADER* msg_base, size_t msg_size) = 0;
};

}; //end of namespace
#pragma pack(pop)
//==============================================================================

inline util::WxMessageIpc::WorkerHandler::WorkerHandler() : root_(NULL) {}
inline util::WxMessageIpc::WorkerHandler::WorkerHandler(
    util::WxMessageIpc* root)
    : root_(root)
{
}

inline util::WxMessageIpc::MainLogicHandler::MainLogicHandler() : root_(NULL) {}
inline util::WxMessageIpc::MainLogicHandler::MainLogicHandler(
    util::WxMessageIpc* root)
: root_(root)
{
}


inline const util::WxMessageIpc::WorkerHandler
util::WxMessageIpc::GetWorkerHandler() const
{
    return WorkerHandler(const_cast<WxMessageIpc*>(this));
}

inline const util::WxMessageIpc::MainLogicHandler
util::WxMessageIpc::GetMainLogicHandler() const
{
    return MainLogicHandler(const_cast<WxMessageIpc*>(this));
}


//==============================================================================

#endif //UTILITY_WX_MESSAGE_IPC_H