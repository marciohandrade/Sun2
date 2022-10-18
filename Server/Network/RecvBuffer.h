#pragma once
#ifndef NETWORK_MODULE_RECV_BUFFER_H
#define NETWORK_MODULE_RECV_BUFFER_H

//==================================================================================================
// <RecvBuffer>
//  병목 현상 감소용, 버그 수정용
class RecvBuffer
{
public:
    RecvBuffer();
    ~RecvBuffer();

    void Create(int buffer_size, DWORD max_packet_size);
    bool UseDualBuffer(DWORD dual_buffer_size);
    void Clear();
    //
    bool HasBeenReadWorking() const;
    // get buffer status a lockless check
    void GetBufferSizeInfo(DWORD& max_buffer_size, DWORD& current_used_size) const;
    void OnCompleted(int number_of_received_bytes);
    bool PrepareRecv(module::RingBufferInfo* const writable_info);
    void GetRecvParam(BYTE** ppRecvPtr, int &nLength);
    //bool Write(const BYTE* pMsg, WORD size);
    bool PopHeadMessage(int size);
    //                        HEADER_SIZE_BODY
    BYTE* GetFirstPacketPtr(HEADER_SIZE_TYPE type, DWORD* if_incorrect_packet_size);

private:
    typedef module::WxRingBuffer netbuffer_t;
    //
    void PumpTo2ndBuffer();
    bool GetReceivableInfo(module::RingBufferInfo* const reading_info); // 50% of remains
    //
    long completed_;
    long buffer_contacted_;
    netbuffer_t* buffer_;
    netbuffer_t* dual_buffer_; // external control buffer
    DWORD max_packet_size_;
    BYTE* external_buffer_;
    BYTE* external_dual_buffer_;
};

//==================================================================================================

inline bool RecvBuffer::HasBeenReadWorking() const
{
    return (completed_ == false || buffer_contacted_ != false);
};

inline void RecvBuffer::OnCompleted(int number_of_received_bytes) 
{
    // this method called in IO thread.
    buffer_->Enqueue(NULL, number_of_received_bytes);
    ::InterlockedExchange(&completed_, true);
}

#endif //NETWORK_MODULE_RECV_BUFFER_H
