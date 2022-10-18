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
namespace module {
;
//------------------------------------------------------------------------------
// Message Buffer Info for Tokenized Buffers
//  - corresponding with WSABUF
typedef WSABUF NetBufferInfo;

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
namespace module {

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
        eSync_ExternalBuffer = 1 << 2,
    };
    WxRingBuffer(DWORD flag = eFlag_Null);
    WxRingBuffer(DWORD flag, DWORD buffer_size); // for SendBuffer
    WxRingBuffer(DWORD flag, BYTE* data, DWORD buffer_size); // for RecvBuffer
    virtual ~WxRingBuffer();

    void Reset();
    // if(data == 0) only offset control
    BOOLEAN Enqueue(const BYTE* data, DWORD size);
    // if(data == 0) only offset control
    BOOLEAN Dequeue(BYTE* OUT data, DWORD size);
    BOOLEAN Peek(BYTE* OUT data, DWORD size);
    DWORD Flag() const;
    DWORD BufferLength() const;
    BOOLEAN IsFull() const;
    BOOLEAN IsEmpty() const;

    BOOLEAN WritableInfo(RingBufferInfo* buffer_info) const;
    BOOLEAN ReadableInfo(RingBufferInfo* buffer_info) const;

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


}; //end of namespace
//
//
//==============================================================================
//  @WxNote (util::)
//  - ���� ���� �Լ��� �ƴ� ���� Ȯ�� �Լ����� ����ȭ �Լ��� �ƴϴ�.
//  - ex) BufferLength(), IsFull(), IsEmpty()
//  -  �� ���� �Լ����� ����ȭ ó���� �ص� ������ �̰��� ����� ����
//    �ٸ� ó���� ������ ��� �� ����� �޶��� �� �ֱ� �����̴�.
//  -  ��, ���� ������ ����Ű�� �ʴ� �Լ����� ������� RingBufferEx�� ��ü
//    ����ȭ ó���ϴ� ���� ���ǹ��� ������.
//     �̷��� �Լ��� �ٸ� �Լ� (En/Dequeue...)���� ����� ��ġ�Ϸ���, �ش�
//    RingBufferEx�� ����ϴ� ��⿡ ����ȭ ó���� �䱸�ȴ�. �׷� �����,
//    WxRingBuffer�� ����ϴ� �͸����ε� ����ϴ�.
//  -  RingBufferEx�� RingBufferEx�� ���� ��� (En/Dequeue, ...)���� �� �ѹ�
//    ȣ�� ���·� ó���� �� ���ڼ��� �����ϴµ� ������ �ִ�.
//  - BufferLength(), IsFull(), IsEmpty()���� ������ ���� �˻翡�� ����� ��.
//  ----------------------------------------------------------------------------
//  - RingBufferEx Methods: Stable(O), Unstable(X),
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
namespace module {

inline DWORD
WxRingBuffer::Flag() const
{
  return flag_;
}

inline DWORD
WxRingBuffer::BufferLength() const
{
  return buffer_size_;
}

inline BOOLEAN
WxRingBuffer::IsFull() const
{
  return (writePos_ + 1) % buffer_size_ == readPos_;
}

inline BOOLEAN
WxRingBuffer::IsEmpty() const
{
  return writePos_ == readPos_;
}

}; //end of namespace
#pragma pack(pop)
//
//
//==============================================================================
#endif //_WX_UTILITY_RING_BUFFER_H_MIGRATIONED_
