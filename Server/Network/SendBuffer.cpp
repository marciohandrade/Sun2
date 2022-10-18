#include "stdafx.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer()
    : completed_(false)
    , buffer_contacted_(false)
    , buffer_(NULL)
    , pending_buffer_(NULL)
    , max_packet_size_(0)
    , pending_buffer_size_(0)
{
}

SendBuffer::~SendBuffer()
{
    if (buffer_) {
        module::TAllocator<netbuffer_t>::DeAllocate(buffer_);
        buffer_ = NULL;
    };
    if (pending_buffer_) {
        module::TAllocator<netbuffer_t>::DeAllocate(pending_buffer_);
        pending_buffer_ = NULL;
    };
}

void SendBuffer::Create(DWORD buffer_size, DWORD max_packet_size) 
{
    buffer_ = new (::malloc(sizeof(netbuffer_t)))
        netbuffer_t(netbuffer_t::eSync_Read | netbuffer_t::eSync_Write, buffer_size);
    pending_buffer_ = NULL;
    pending_buffer_size_ = 0;
    max_packet_size_ = max_packet_size;
    completed_ = true;
}

bool SendBuffer::UsePendingBuffer(DWORD pending_buffer_size)
{
    if (pending_buffer_size == 0) {
        return false;
    };
    netbuffer_t* pending_buffer = pending_buffer_;
    if (pending_buffer)
    {
        struct Interlock {
            Interlock(long* volatile lock) : lock_(lock) {
                while (::InterlockedCompareExchange(lock_, 1, 0)) { continue; }
            };
            ~Interlock() {
                ::InterlockedExchange(lock_, 0);
            };
            long* volatile lock_;
        } inter_lock(&buffer_contacted_);
        InterlockedExchangePointer((PVOID*)&pending_buffer_, NULL);
        //
        module::TAllocator<netbuffer_t>::DeAllocate(pending_buffer);
        pending_buffer = NULL;
    };

    pending_buffer_size_ = pending_buffer_size;
    pending_buffer = new (::malloc(sizeof(netbuffer_t))) \
        netbuffer_t(netbuffer_t::eSync_Read | netbuffer_t::eSync_Write, pending_buffer_size_);
    pending_buffer->Reset();

    InterlockedExchangePointer((PVOID*)&pending_buffer_, pending_buffer);
    return true;
}

void SendBuffer::Clear() 
{ 
    assert(this->buffer_contacted_ == false);
    buffer_->Reset();
    if (pending_buffer_)
    {
        module::TAllocator<netbuffer_t>::DeAllocate(pending_buffer_);
        pending_buffer_ = NULL;
        pending_buffer_size_ = NULL;
    };
    buffer_contacted_ = false;
    completed_ = true;
}

__inline bool SendBuffer::ValidSizeCheck(
    HEADER_SIZE_TYPE type, const PACKET_HEADER& header) const
{
    DWORD msg_body_size = header.size;
    if (type == HEADER_SIZE_TOTAL) {
        msg_body_size -= sizeof(header);
    }
    if (msg_body_size > max_packet_size_) {
        return false;
    }
    else {
        return true;
    }
}

bool SendBuffer::PrepareSend(module::RingBufferInfo* const sending_info)
{
    if (completed_ == false) {
        return false;
    };
    // NOTE: f110615.2L, it may occur that session release process has been working.
    struct Interlock {
        Interlock(long* volatile lock) : lock_(lock) {
            result_ = ::InterlockedCompareExchange(lock_, 1, 0);
        };
        ~Interlock() {
            ::InterlockedExchange(lock_, 0);
        };
        long* volatile lock_;
        long result_;
    } inter_lock(&buffer_contacted_);
    // this method worked in send_thread
    if (inter_lock.result_ != 0) {
        return false;
    };
    if (pending_buffer_) {
        PumpPendingBuffer();
    };
    if (buffer_->IsEmpty()) {
        return false;
    };
    if (buffer_->ReadableInfo(sending_info) == false) {
        return false;
    };
    //
    ::InterlockedExchange(&completed_, false);
    return true;
}

void SendBuffer::PumpPendingBuffer()
{
    // this method worked in send_thread
    if (pending_buffer_ == NULL) {
        return;
    };

    module::RingBufferInfo readable_info;
    if (pending_buffer_->ReadableInfo(&readable_info) == false) {
        return;
    };

    module::RingBufferInfo writable_info;
    if (buffer_->WritableInfo(&writable_info) == false) {
        return;
    };
    DWORD total_writable_size = writable_info.first.len + writable_info.second.len;
    DWORD total_written_size = 0;
    if (readable_info.first.len)
    {
        DWORD written_size = (total_writable_size >= readable_info.first.len) ?
            readable_info.first.len : total_writable_size;
        total_writable_size -= written_size;
        buffer_->Enqueue(reinterpret_cast<BYTE*>(readable_info.first.buf), written_size);
        total_written_size += written_size;
    };
    if (readable_info.first.len == total_written_size &&
        readable_info.second.len)
    {
        DWORD written_size = (total_writable_size >= readable_info.second.len) ?
            readable_info.second.len : total_writable_size;
        total_writable_size -= written_size;
        buffer_->Enqueue(reinterpret_cast<BYTE*>(readable_info.second.buf), written_size);
        total_written_size += written_size;
    };
    if (total_written_size) {
        pending_buffer_->Dequeue(NULL, total_written_size);
    };
}

// Old comments:
//  동기화 문제 해결을 위한 확장 인터페이스, 사실 RingBuffer의 성질을 가지는 인터페이스는 아니지만
//  내부에서만 사용하는 것이므로 추가한다. Write는 동기화 문제가 존재하므로 사용을 배제할 것이다.
//  기존보다 임계영역에 있는 시간이 길어졌지만, 동기화 문제 해결을 위한 처리가 단순해지며 임계영역에
//  들어가는 회수가 급감하므로 더 좋은 방안이 될 것이다.
bool SendBuffer::WriteMessages(
    const BYTE** msg_array, WORD* size_array, DWORD count_of_msgs,
    HEADER_SIZE_TYPE type)
{
    PACKET_HEADER header = { 0 };
    if (type == HEADER_SIZE_TOTAL) {
        header.size = sizeof(header);
    };
    WORD* const size_end = &size_array[count_of_msgs];
    for (WORD* size_it = size_array; size_it != size_end; ++size_it) {
        header.size += *size_it;
    };
    if (ValidSizeCheck(type, header) == false) {
        return false;
    };

    netbuffer_t* const writing_buffer = pending_buffer_ ? pending_buffer_ : buffer_;
    //
    module::RingBufferInfo writable_info;
    if (writing_buffer->WritableInfo(&writable_info) == false) {
        return false;
    };
    DWORD writable_len = (writable_info.first.len + writable_info.second.len);
    if (header.size > writable_len) {
        return false;
    };
    // writes header
    writing_buffer->Enqueue(reinterpret_cast<BYTE*>(&header), sizeof(header));
    // write messages
    for (; size_array != size_end; ++msg_array, ++size_array) {
        writing_buffer->Enqueue(*msg_array, *size_array);
    };
    return true;
}

bool SendBuffer::IsEmpty() const
{
    if (buffer_->IsEmpty() == false) {
        return false;
    };
    if (pending_buffer_ && pending_buffer_->IsEmpty() == false) {
        return false;
    };
    return true;
}

// get buffer status a lockless check
void SendBuffer::GetBufferSizeInfo(DWORD& max_buffer_size, DWORD& current_used_size) const
{
    netbuffer_t* const buffer = pending_buffer_ ? pending_buffer_ : buffer_;
    max_buffer_size = buffer->BufferLength();
    module::RingBufferInfo writable_info;
    if (buffer->WritableInfo(&writable_info) == false) {
        current_used_size = max_buffer_size;
        return;
    };
    current_used_size = max_buffer_size -
        (writable_info.first.len + writable_info.second.len + 1);
}

