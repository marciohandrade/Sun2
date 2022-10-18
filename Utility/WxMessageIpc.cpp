#include "UtilityCommon.h"
#include "./WxMessageIpc.h"

#include <process.h>

//==============================================================================

namespace util {
;

static const ulong reacting_interval[util::WxMessageIpc::eReacting_Counts] =
{
    0,
    10 * 1000, //eReacting_Slow,     // 10000 milli-seconds (10s)
     1 * 1000, //eReacting_Medium,   //  1000 milli-seconds ( 1s)
          500, //eReacting_Fast,     //   500 milli-seconds
          100, //eReacting_MoreFast, //   100 milli-seconds
           16, //eReacting_TooFast,  //    16 milli-seconds
    // NOTE: this same with eReacting_TooFast on high-load state
            1, //eReacting_Realtime, //     1 milli-seconds,
};

}; //end of namespace



util::WxMessageIpc::WxMessageIpc(
    size_t max_worker_to_main_buffer_size,
    size_t max_main_to_worker_buffer_size,
    size_t max_message_size)
    : message_io_thread_(INVALID_HANDLE_VALUE)
    , wakeup_event_(INVALID_HANDLE_VALUE)
    , serviced_(false)
    , stop_requested_(false)
    , message_pumping_level_(eReacting_Medium)
    , main_to_message_io_gateway_(NULL)
    , workers_to_main_gateway_(NULL)
    , worker_message_handler_(NULL)
    , max_worker_to_main_buffer_size_(max_worker_to_main_buffer_size)
    , max_main_to_worker_buffer_size_(max_main_to_worker_buffer_size)
    , max_message_size_(max_message_size)
{
    ASSERT(max_worker_to_main_buffer_size >= max_message_size &&
           max_main_to_worker_buffer_size >= max_message_size &&
           max_message_size_ <= kMaxMessageSize);
}

util::WxMessageIpc::~WxMessageIpc()
{
    this->StopWorking();
    SAFE_DELETE(main_to_message_io_gateway_);
    SAFE_DELETE(workers_to_main_gateway_);
}

bool util::WxMessageIpc::StartWorking(
    eReactingLevel message_pumping_level,
    IWxWorkerMessageHandler* worker_message_handler)
{
    if (serviced_ != false) {
        return false;
    };
    serviced_ = false;
    if (message_io_thread_ != INVALID_HANDLE_VALUE)
    {
        stop_requested_ = true;
        assert(wakeup_event_ != INVALID_HANDLE_VALUE);
        ::SetEvent(wakeup_event_);
        ::WaitForSingleObject(message_io_thread_, INFINITE);
        ::CloseHandle(message_io_thread_);
        message_io_thread_ = INVALID_HANDLE_VALUE;
    };
    if (wakeup_event_ != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(wakeup_event_);
        wakeup_event_ = INVALID_HANDLE_VALUE;
    };

    if (main_to_message_io_gateway_ == NULL)
    {
        main_to_message_io_gateway_ = new MainToMessageIoGateway(\
            MainToMessageIoGateway::eSync_Mask,
            max_main_to_worker_buffer_size_);
    };
    if (workers_to_main_gateway_ == NULL)
    {
        workers_to_main_gateway_ = new WorkersToMainGateway(\
            WorkersToMainGateway::kFlagDefault,
            max_worker_to_main_buffer_size_);
    };
    assert(main_to_message_io_gateway_ != NULL &&
           workers_to_main_gateway_ != NULL);
    main_to_message_io_gateway_->Reset();
    workers_to_main_gateway_->Reset();
    //
    stop_requested_ = false;
    message_pumping_level_ = message_pumping_level;
    worker_message_handler_ = worker_message_handler;

    if (wakeup_event_ == INVALID_HANDLE_VALUE) //assert
    {
        wakeup_event_ = ::CreateEvent(NULL, false, false, NULL);
    };
    if (message_io_thread_ == INVALID_HANDLE_VALUE) // assert
    {
        unsigned int thread_id;
        message_io_thread_ = (HANDLE)_beginthreadex(\
            NULL, 0, message_io_thread, this, 0, &thread_id);
    };

    serviced_ = true;
    return true;
}

void util::WxMessageIpc::StopWorking()
{
    if (serviced_ == false) {
        return;
    };
    InterlockedExchange(&serviced_, false);
    InterlockedExchange(&stop_requested_, true);
    if (message_io_thread_ != INVALID_HANDLE_VALUE)
    {
        ::SetEvent(wakeup_event_);
        ::WaitForSingleObject(message_io_thread_, INFINITE);
        ::CloseHandle(message_io_thread_);
        message_io_thread_ = INVALID_HANDLE_VALUE;
    };
    if (wakeup_event_ != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(wakeup_event_);
        wakeup_event_ = INVALID_HANDLE_VALUE;
    };
}

unsigned int __stdcall util::WxMessageIpc::message_io_thread(
    void* passing_parameter)
{
    WxMessageIpc* const root = \
        reinterpret_cast<WxMessageIpc*>(passing_parameter);

    WorkerHandler worker = root->GetWorkerHandler();
    //ulong 
    while (root->serviced_ != false)
    {
        if (root->stop_requested_) {
            ::Sleep(util::reacting_interval[root->eReacting_Realtime]);
            continue;
        };

        // parse messages
        while (root->ParseMessagesFromMain(&worker) != false) {
            continue;
        };

        if (root->stop_requested_) {
            continue;
        };
        ::WaitForSingleObject(root->wakeup_event_,
            util::reacting_interval[root->message_pumping_level_]);
    };
    return 0;
}

bool util::WxMessageIpc::ParseMessagesFromMain(
    WorkerHandler* const worker)
{
    static uint8_t message_stream[USHRT_MAX];
    //
    WX_INTER_MSG_HEADER* const msg_header = \
        reinterpret_cast<WX_INTER_MSG_HEADER*>(message_stream);

    if (main_to_message_io_gateway_->Peek(message_stream, sizeof(*msg_header))
            == false)
    {
        return false;
    };
    // thread unsafe
    FlowControl::FCAssert(msg_header->msg_size < max_message_size_);

    main_to_message_io_gateway_->Dequeue(message_stream, msg_header->msg_size);

    if (worker_message_handler_)
    {
        worker_message_handler_->OnReceive(
            worker, msg_header, msg_header->msg_size);
    };
    return true;
}

bool util::WxMessageIpc::ParseMessagesFromWorkers(
    MainLogicHandler* const main_logic, IWxMainMessageHandler* const receiver)
{
    static uint8_t message_stream[USHRT_MAX];
    //
    WX_INTER_MSG_HEADER* const msg_header = \
        reinterpret_cast<WX_INTER_MSG_HEADER*>(message_stream);

    if (workers_to_main_gateway_->Peek(message_stream, sizeof(*msg_header))
        == false)
    {
        return false;
    };
    // thread unsafe
    FlowControl::FCAssert(msg_header->msg_size <= max_message_size_);

    workers_to_main_gateway_->Dequeue(message_stream, msg_header->msg_size);

    receiver->OnReceive(main_logic, msg_header, msg_header->msg_size);
    return true;
};

//==============================================================================

bool util::WxMessageIpc::WorkerHandler::SendMessageToMain(
    WX_INTER_MSG_HEADER* msg, size_t msg_size) const
{
    if (root_->serviced_ == false || root_->stop_requested_ != false) {
        return false;
    };
    msg->msg_size = msg_size;
    root_->workers_to_main_gateway_->Enqueue(
        reinterpret_cast<uint8_t*>(msg), msg_size);

    return true;
}

bool util::WxMessageIpc::MainLogicHandler::SendMessageToWorkers(
    WX_INTER_MSG_HEADER* msg, size_t msg_size) const
{
    if (root_->serviced_ == false || root_->stop_requested_ != false) {
        return false;
    };

    msg->msg_size = msg_size;
    root_->main_to_message_io_gateway_->Enqueue(
        reinterpret_cast<uint8_t*>(msg), msg_size);

    return true;
}

//==============================================================================

void util::WxMessageIpc::MainLogicHandler::Update(
    IWxMainMessageHandler* const receiver) const
{
    while (root_->ParseMessagesFromWorkers(\
        const_cast<MainLogicHandler*>(this), receiver) != false) {
        continue;
    };
}