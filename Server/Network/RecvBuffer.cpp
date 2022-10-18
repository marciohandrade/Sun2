#include "stdafx.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer()
    : completed_(false)
    , buffer_contacted_(false)
    , buffer_(NULL)
    , dual_buffer_(NULL)
    , max_packet_size_(0)
    , external_buffer_(NULL)
    , external_dual_buffer_(NULL)
{
}

RecvBuffer::~RecvBuffer()
{
    if (buffer_) {
        module::TAllocator<netbuffer_t>::DeAllocate(buffer_);
        buffer_ = NULL;
    };
    if (dual_buffer_) {
        module::TAllocator<netbuffer_t>::DeAllocate(dual_buffer_);
        dual_buffer_ = NULL;
    };
    if (external_buffer_) {
        ::free(external_buffer_);
        external_buffer_ = NULL;
    };
    if (external_dual_buffer_) {
        ::free(external_dual_buffer_);
        external_dual_buffer_ = NULL;
    };
};

void RecvBuffer::Create(int buffer_size, DWORD max_packet_size)
{
    assert(buffer_size > (int)max_packet_size);
    assert(buffer_ == NULL && dual_buffer_ == NULL &&
           external_buffer_ == NULL && external_dual_buffer_ == NULL);
    max_packet_size_ = max_packet_size;
    DWORD max_buffer_size = buffer_size + max_packet_size_ + 1;
    external_buffer_ = (BYTE*)::malloc(max_buffer_size);
    buffer_ = new (::malloc(sizeof(netbuffer_t))) \
        netbuffer_t(netbuffer_t::eSync_Read | netbuffer_t::eSync_Write,
                    external_buffer_, buffer_size);
    //
    completed_ = true;
}

void RecvBuffer::Clear() 
{
    buffer_->Reset();
    if (dual_buffer_)
    {
        module::TAllocator<netbuffer_t>::DeAllocate(dual_buffer_);
        dual_buffer_ = NULL;
        ::free(external_dual_buffer_);
        external_dual_buffer_ = NULL;
    };
    completed_ = true;
}

bool RecvBuffer::UseDualBuffer(DWORD dual_buffer_size)
{
    if (dual_buffer_size == 0) {
        return false;
    };

    netbuffer_t* dual_buffer = dual_buffer_;
    if (dual_buffer)
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
        InterlockedExchangePointer((PVOID*)&dual_buffer_, NULL);
        //
        module::TAllocator<netbuffer_t>::DeAllocate(dual_buffer);
        dual_buffer = NULL;
    };
    DWORD max_buffer_size = dual_buffer_size + max_packet_size_ + 1;
    external_dual_buffer_ = (BYTE*)::malloc(max_buffer_size);
    dual_buffer_ = new (::malloc(sizeof(netbuffer_t))) \
        netbuffer_t(netbuffer_t::eSync_Read | netbuffer_t::eSync_Write,
                    external_dual_buffer_, dual_buffer_size);
    return true;
}

// 50% of remains
bool RecvBuffer::GetReceivableInfo(module::RingBufferInfo* const writable_info)
{
    // worked in io_thread#n
    if (buffer_->IsFull()) {
        return false; 
    };
    if (buffer_->WritableInfo(writable_info) == false) {
        return false;
    };

    DWORD usable_size = writable_info->first.len + writable_info->second.len;
    if (usable_size <= 1) {
        // WARNING: danger, next calling may occur fault.
        // however, until next calling of this routine, main thread bring the stacked buffer.
        // according to a test result, the stability of a server processing and
        // the performance of it showed a significant improvement.
        return true;
    };
    usable_size >>= 1;
    if (writable_info->first.len)
    {
        if (writable_info->first.len >= usable_size)
        {
            DWORD remains = writable_info->first.len - usable_size;
            writable_info->first.len -= remains;
            writable_info->second.buf = NULL;
            writable_info->second.len = 0;
            return true;
        };
        usable_size -= writable_info->first.len;
    };
    if (writable_info->second.len)
    {
        assert(writable_info->second.len > usable_size);
        DWORD remains = writable_info->second.len - usable_size;
        writable_info->second.len -= remains;
    }
    return true;
}

bool RecvBuffer::PrepareRecv(module::RingBufferInfo* const writable_info)
{
    // worked in io_thread#n
    if (completed_ == false) {
        return false;
    };
    struct Interlock {
        Interlock(long* volatile lock) : lock_(lock) {
            while (::InterlockedCompareExchange(lock_, 1, 0)) { continue; }
        };
        ~Interlock() {
            ::InterlockedExchange(lock_, 0);
        };
        long* volatile lock_;
    } inter_lock(&buffer_contacted_);
    if (dual_buffer_) {
        PumpTo2ndBuffer();
    };
    if (this->GetReceivableInfo(writable_info) == false) {
        return false;
    };
    //
    ::InterlockedExchange(&completed_, false);
    return true;
}

void RecvBuffer::PumpTo2ndBuffer()
{
    // worked in io_thread#n
    if (dual_buffer_ == NULL) {
        return;
    };
    module::RingBufferInfo writable_info;
    if (dual_buffer_->WritableInfo(&writable_info) == false) {
        return;
    };
    module::RingBufferInfo readable_info;
    if (buffer_->ReadableInfo(&readable_info) == false) {
        return;
    };
    DWORD total_writable_size = writable_info.first.len + writable_info.second.len;
    DWORD total_written_size = 0;
    if (readable_info.first.len)
    {
        DWORD written_size = (total_writable_size >= readable_info.first.len) ?
            readable_info.first.len : total_writable_size;
        total_writable_size -= written_size;
        dual_buffer_->Enqueue(reinterpret_cast<BYTE*>(readable_info.first.buf), written_size);
        total_written_size += written_size;
    };
    if (readable_info.first.len == total_written_size &&
        readable_info.second.len)
    {
        DWORD written_size = (total_writable_size >= readable_info.second.len) ?
            readable_info.second.len : total_writable_size;
        total_writable_size -= written_size;
        dual_buffer_->Enqueue(reinterpret_cast<BYTE*>(readable_info.second.buf), written_size);
        total_written_size += written_size;
    };
    if (total_written_size) {
        buffer_->Dequeue(NULL, total_written_size);
    };
};

void RecvBuffer::GetRecvParam(BYTE** recv_ptr, int& length) 
{
    module::RingBufferInfo reading_info;
    if (buffer_->WritableInfo(&reading_info) == false) {
        assert(!"false");
        return;
    };
    *recv_ptr = reinterpret_cast<BYTE*>(reading_info.first.buf);
    length = reading_info.first.len;
}

bool RecvBuffer::PopHeadMessage(int size)
{
    netbuffer_t* buffer = dual_buffer_ ? dual_buffer_ : buffer_;
    return (buffer->Dequeue(NULL, size) != false);
};

BYTE* RecvBuffer::GetFirstPacketPtr(HEADER_SIZE_TYPE type, DWORD* incorrect_packet_size) 
{
    // worked in logic thread
    netbuffer_t* buffer = dual_buffer_ ? dual_buffer_ : buffer_;
    PACKET_HEADER header;
    // 버퍼의 데이터 길이가 헤더 길이보다 작으면 NULL 리턴
    if (buffer->Peek((BYTE*)&header, sizeof(header)) == false) {
        return NULL;
    };
    // 버퍼의 데이터 길이가 패킷 길이보다 작으면 NULL 리턴(패킷 완성 안됨)
    DWORD required_msg_size = (DWORD)(sizeof(header) + header.size);
    if (type == HEADER_SIZE_TOTAL) {
        required_msg_size = header.size;
    };
    // CHANGES: f110810.3L, prevent buffer overrun cases
    // that may occur when grater than desired max packet size.
    if (required_msg_size <= sizeof(header)) {
        *incorrect_packet_size = ULONG_MAX;
        return NULL;
    };
    if (required_msg_size > max_packet_size_) {
        *incorrect_packet_size = required_msg_size;
        return NULL;
    };
    //
    module::RingBufferInfo reading_info;
    buffer->ReadableInfo(&reading_info); // assertion
    //
    DWORD current_readable_size = reading_info.first.len + reading_info.second.len;
    if (current_readable_size < required_msg_size) {
        return NULL;
    };
    if (reading_info.first.len >= required_msg_size) {
        return reinterpret_cast<BYTE*>(reading_info.first.buf);
    };
    required_msg_size -= reading_info.first.len;
    // NOTE: f110616.4L, this routine is based on the premise
    // that usable the out of ranges of (reading_info.first.buf + len)
    CopyMemory(reading_info.first.buf + reading_info.first.len,
               reading_info.second.buf, required_msg_size);
    return reinterpret_cast<BYTE*>(reading_info.first.buf);
}

// get buffer status a lockless check
void RecvBuffer::GetBufferSizeInfo(DWORD& max_buffer_size, DWORD& current_used_size) const
{
    netbuffer_t* buffer = dual_buffer_ ? dual_buffer_ : buffer_;
    max_buffer_size = buffer->BufferLength();
    module::RingBufferInfo readable_info;
    if (buffer->ReadableInfo(&readable_info) == false) {
        current_used_size = 0;
        return;
    };
    current_used_size = (readable_info.first.len + readable_info.second.len);
}