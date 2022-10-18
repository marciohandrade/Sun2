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
//  - ���� ���� �Լ��� �ƴ� ���� Ȯ�� �Լ����� ����ȭ �Լ��� �ƴϴ�.
//  - ex) BufferLength(), IsFull(), IsEmpty()
//  -  �� ���� �Լ����� ����ȭ ó���� �ص� ������ �̰��� ����� ����
//    �ٸ� ó���� ������ ��� �� ����� �޶��� �� �ֱ� �����̴�.
//  -  ��, ���� ������ ����Ű�� �ʴ� �Լ����� ������� WxRingBufferEx�� ��ü
//    ����ȭ ó���ϴ� ���� ���ǹ��� ������.
//     �̷��� �Լ��� �ٸ� �Լ� (En/Dequeue...)���� ����� ��ġ�Ϸ���, �ش�
//    WxRingBufferEx�� ����ϴ� ��⿡ ����ȭ ó���� �䱸�ȴ�. �׷� �����,
//    WxRingBuffer�� ����ϴ� �͸����ε� ����ϴ�.
//  -  WxRingBufferEx�� WxRingBufferEx�� ���� ��� (En/Dequeue, ...)���� �� �ѹ�
//    ȣ�� ���·� ó���� �� ���ڼ��� �����ϴµ� ������ �ִ�.
//  - BufferLength(), IsFull(), IsEmpty()���� ������ ���� �˻翡�� ����� ��.
//  ----------------------------------------------------------------------------
//  - WxRingBufferEx Methods: Stable(O), Unstable(X),
//                          Status Check(-) : ���� Ȯ�� ����(O), ���� �۾�(X)
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
