#ifndef _WX_UTILITY_RING_BUFFER_H_MIGRATIONED_
#define _WX_UTILITY_RING_BUFFER_H_MIGRATIONED_

//==============================================================================
//  date: 09/06/29
//  author: waverix
//  reference: waverix framework 090629 version
//  history:
//      09/01/01 : create
//      ~~
//      09/06/29 : migration to SUN AuthSystem (downgrade version)
//                 change name RingBuffer to WxRingBuffer
//      10/06/16 : move to a utility project and
//                 change a namespace from waverix::wx_util to util
//
//==============================================================================
//	(WxNote) (waverix::wx_util::)
//
#pragma pack(push)
#pragma pack()
namespace util {

//------------------------------------------------------------------------------
// Message Buffer Info for Tokenized Buffers
//  - corresponding with WSABUF
struct NetBufferInfo
{
    DWORD size;
    BYTE* buffer;
};

struct RingBufferInfo
{
    NetBufferInfo first;
    NetBufferInfo second;
};

}; // end of namespace 'waverix.wx_util'
//
//
//==============================================================================
//  @WxNote (waverix::wx_util::)
//
namespace util {

//------------------------------------------------------------------------------
//  ()
//  @name  
//  @description :
//  - designed for single-reader and single-writer
class WxRingBuffer
{
public:
    enum eFlagBase {
        eFlag_Null    = 0,
        eSync_Read    = 1 << 0,
        eSync_Write   = 1 << 1,
        eSync_Mask    = eSync_Read
                      | eSync_Write,
    };
    enum eReadWrite
    {
        //eMode_SR_SW   = eSync_Mask | (1 << 4), // default
        // NOTE: f110830.1L, to support GameShop.EventHandling
        eMode_SR_MW   = eSync_Mask | (1 << 5),
        //eMode_MR_SW   = eSync_Mask | (1 << 6), // reserved
        //eMode_MR_MW   = eSync_Mask | (1 << 7), // reserved
        eMode_Counts
    };
    WxRingBuffer(DWORD flag = eFlag_Null);
    WxRingBuffer(DWORD flag, DWORD buffer_size);
    //WxRingBuffer(DWORD flag, BYTE* data, DWORD buffer_size);
    virtual ~WxRingBuffer();

    void Reset();
    // if(data == 0) only offset control
    bool Enqueue(const BYTE* data, DWORD size);
    // if(data == 0) only offset control
    bool Dequeue(BYTE* OUT data, DWORD size);
    bool Peek(BYTE* OUT data, DWORD size);
    DWORD Flag() const;
    DWORD BufferLength() const;
    bool IsFull() const;
    bool IsEmpty() const;

    bool WritableInfo(RingBufferInfo* buffer_info) const;
    bool ReadableInfo(RingBufferInfo* buffer_info) const;

protected:
    DWORD flag_;
    BYTE* data_;
    DWORD buffer_size_;
    DWORD writePos_;
    DWORD readPos_;

    friend class NetworkBuffer;
    friend class SendBuffer;
    friend class RecvBuffer;
};

template<WxRingBuffer::eReadWrite>
class WxRingBufferEx;

template<>
class WxRingBufferEx<WxRingBuffer::eMode_SR_MW>;

//==============================================================================
//  @WxNote (waverix::wx_util::)
//
template<>
class WxRingBufferEx<WxRingBuffer::eMode_SR_MW> : public util::WxRingBuffer
{
public:
    static const DWORD kFlagDefault = eSync_Mask | eMode_SR_MW;
    WxRingBufferEx(DWORD flag = eSync_Mask);
    WxRingBufferEx(DWORD flag, DWORD buffer_size);
    //WxRingBufferEx(DWORD flag, BYTE* data, DWORD buffer_size);
    virtual ~WxRingBufferEx();

    void Reset();
    // if(data == 0) only offset control
    bool Enqueue(const BYTE* data, DWORD size);
    // if(data == 0) only offset control
    bool Dequeue(BYTE* OUT data, DWORD size);
    bool Peek(BYTE* OUT data, DWORD size);
    bool IsFull() const;
    bool IsEmpty() const;

    bool ReadableInfo(RingBufferInfo* buffer_info) const;
private:
    bool WritableInfo(RingBufferInfo* buffer_info) const; // can't used

private:
    CRITICAL_SECTION cs_;
};


}; //end of namespace
//
//
//==============================================================================
//  @WxNote (util::)
//  - 상태 변경 함수가 아닌 상태 확인 함수들은 동기화 함수가 아니다.
//  - ex) BufferLength(), IsFull(), IsEmpty()
//  -  와 같은 함수들은 동기화 처리를 해도 어차피 이것을 사용한 이후
//    다른 처리를 수행할 경우 그 결과가 달라질 수 있기 때문이다.
//  -  즉, 상태 변경을 일으키지 않는 함수들을 대상으로 WxRingBufferEx가 자체
//    동기화 처리하는 것은 무의미한 문제다.
//     이러한 함수와 다른 함수 (En/Dequeue...)등의 결과가 일치하려면, 해당
//    WxRingBufferEx를 사용하는 모듈에 동기화 처리가 요구된다. 그럴 경우라면,
//    WxRingBuffer를 사용하는 것만으로도 충분하다.
//  -  WxRingBufferEx는 WxRingBufferEx의 단일 기능 (En/Dequeue, ...)등의 단 한번
//    호출 형태로 처리시 그 원자성을 보장하는데 목적이 있다.
//  - BufferLength(), IsFull(), IsEmpty()들은 간단한 상태 검사에만 사용할 것.
//  ----------------------------------------------------------------------------
//  - WxRingBufferEx Methods: Stable(O), Unstable(X),
//                          Status Check(-) : 상태 확인 수준(O), 연동 작업(X)
//
//     Name      | SR_WR SR_MW MR_SW MR_MW
//  Reset        |   O     O     O     O
//  Enqueue      |   O     O     O     O
//  Dequeue      |   O     O     O     O
//  Peek         |   O     O     X     X
//  BufferLength |   -     -     -     -
//  IsFull       |   -     -     -     -
//  IsEmpty      |   -     -     -     -
//  WritableInfo |   O     X     O     X
//  ReadableInfo |   O     O     X     X
//  
//==============================================================================
//  @WxNote (util::)
//
inline DWORD
util::WxRingBuffer::Flag() const
{
  return flag_;
}

inline DWORD
util::WxRingBuffer::BufferLength() const
{
  return buffer_size_;
}

inline bool
util::WxRingBuffer::IsFull() const
{
  return (writePos_ + 1) % buffer_size_ == readPos_;
}

inline bool
util::WxRingBuffer::IsEmpty() const
{
  return writePos_ == readPos_;
}

//==============================================================================

inline bool
util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::IsFull() const
{
    DWORD writePos = writePos_, readPos = readPos_;
    return (writePos + 1) % buffer_size_ == readPos;
}

inline bool
util::WxRingBufferEx<util::WxRingBuffer::eMode_SR_MW>::IsEmpty() const
{
    DWORD writePos = writePos_, readPos = readPos_;
    return writePos == readPos;
}


#pragma pack(pop)
//
//
//==============================================================================
#endif //_WX_UTILITY_RING_BUFFER_H_MIGRATIONED_
