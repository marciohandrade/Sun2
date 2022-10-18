#pragma once

//==================================================================================================
// <SendBuffer>
//  병목 현상 감소용, 버그 수정용
class SendBuffer
{
public:
    SendBuffer();
    ~SendBuffer();

    void Create(DWORD buffer_size, DWORD max_packet_size);
    bool UsePendingBuffer(DWORD pending_buffer_size);
    void Clear();
    //
    bool HasBeenSendWorking() const;
    void ReleaseSendingLock();
    //
    bool IsEmpty() const;
    // get buffer status a lockless check
    void GetBufferSizeInfo(DWORD& max_buffer_size, DWORD& current_used_size) const;
    void OnCompleted(int number_of_sent_bytes);
    bool PrepareSend(module::RingBufferInfo* const sending_info);
    bool WriteMessages(const BYTE** msg_array, WORD* size_array, DWORD count_of_msgs,
                       HEADER_SIZE_TYPE type = HEADER_SIZE_BODY);
    //
private:
    typedef module::WxRingBuffer netbuffer_t;
    //
    bool ValidSizeCheck(HEADER_SIZE_TYPE type, const PACKET_HEADER& header) const;
    void PumpPendingBuffer();
    //
    long completed_;
    long buffer_contacted_;
    netbuffer_t* buffer_; // Registered -> Send thread context
    netbuffer_t* pending_buffer_; // external control buffer
    DWORD max_packet_size_;
    DWORD pending_buffer_size_;
};

//==================================================================================================

// NOTE: f110615.1L, added new checker to prevent the removing session while sending.
inline bool SendBuffer::HasBeenSendWorking() const
{
    return (buffer_contacted_ != false);
};

inline void SendBuffer::ReleaseSendingLock()
{
    ::InterlockedExchange(&completed_, true);
}

inline void SendBuffer::OnCompleted(int number_of_sent_bytes) 
{
    // this method called in IO thread.
    buffer_->Dequeue(NULL, number_of_sent_bytes);
    ::InterlockedExchange(&completed_, true);
}

