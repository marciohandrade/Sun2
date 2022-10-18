#include "stdafx.h"
#include "RingBufferEx.h"

namespace module {
;
//==================================================================================================
//  <RingBufferEx>
RingBufferEx::RingBufferEx(int buffer_size, int iExtraSize)
    : buffer_(NULL), m_BufferSize(buffer_size), m_Head(0), m_Tail(0)
{
    assert(buffer_size >= 0);

    if (buffer_size > 0) {
        Init(buffer_size, iExtraSize);
    }
}

RingBufferEx::~RingBufferEx()
{
    Release();
}

void RingBufferEx::Init(int buffer_size, int iExtraSize)
{
    assert(buffer_size > 0);
    assert(buffer_ == NULL);
    const size_t total_size = buffer_size + iExtraSize;
    buffer_ = new (::malloc(total_size)) BYTE;
    m_BufferSize = buffer_size;
    m_ExtraSize = iExtraSize;
    m_Head = 0;
    m_Tail = 0;
}

void RingBufferEx::Release()
{
    if (buffer_)
    {
        ::free(buffer_); //delete(buffer_); // valid control... delete []...
        buffer_ = NULL;
    }
    m_BufferSize = 0;
    m_Head = 0;
    m_Tail = 0;
}

void RingBufferEx::Recycle()
{
    m_Head = 0;
    m_Tail = 0;
}

// 내부 버퍼에다가 pBuffer를 memcpy
int RingBufferEx::Write(const BYTE* pBuffer, int iLen)
{
    // Tail뒤에다가 pBuffer를 len만큼 추가해야 하는데.. 
    // 일단.. buffer가 부족하면 낭패!
    if (GetFreeSize() < iLen)
    {
        // HooooooooooooT
        // oooooooTHooooo
        return -1;
    }

    if (m_Head <= m_Tail)
    {
        // ......HT......
        // ....HooooT....

        // tail 뒤쪽을 쓰고..
        int iRightFreeSize = m_BufferSize - m_Tail; // 0번째 자리가 비어있으므로 끝까지 채워도 된다.(tail -1 안해줘도 된다.)
        int iWriteLen = min(iRightFreeSize, iLen);

        memcpy(buffer_+m_Tail, pBuffer, iWriteLen);
        m_Tail = (m_Tail + iWriteLen) % m_BufferSize;

        // 쓸게 남았으면 앞쪽부터 쓴다.
        int iRemainLen = iLen - iWriteLen;
        if (iRemainLen > 0)
        {
            assert(m_Tail == 0); // 쓸게 남은 경우는 tail은 0이 된다.
            memcpy(buffer_, pBuffer+iWriteLen, iRemainLen);
            m_Tail = iRemainLen;
        }
    }
    else
    {   
        // T............H
        // T........Hoooo
        // ooooT........H
        // ooooT....Hoooo
        // HoooooooooT...

        // FreeSize는 체크했으므로 tail부터 맘껏 쓰면 된다.
        memcpy(buffer_ + m_Tail, pBuffer, iLen);
        m_Tail += iLen;
    } 

    return iLen;
}

// 내부 버퍼에서 pBuffer로 memcpy
int RingBufferEx::Read(BYTE* pBuffer, int iLen)
{
    if (GetDataSize() < iLen)
    {
        // len이 크면 아예 실패로 처리한다.
        return -1;
    }

    if (m_Head < m_Tail)
    {
        // HooooooooooooT
        // ......HT......
        // ....HooooT....
        // HoooooooooT...

        // dataSize는 위에서 체크했으므로 충분하다.
        memcpy(pBuffer, buffer_+m_Head, iLen);
        m_Head += iLen;
    }
    else
    {
        // oooooooTHooooo   
        // T............H
        // T........Hoooo
        // ooooT........H
        // ooooT....Hoooo

        // head부터 끝까지?
        int iWriteLen = min(iLen, m_BufferSize-m_Head);
        memcpy(pBuffer, buffer_+m_Head, iWriteLen);
        m_Head = (m_Head + iWriteLen) % m_BufferSize;

        // 아직 더 써야되면 앞쪽부터 쓴다.
        int iRemainLen = iLen - iWriteLen;
        if (iRemainLen > 0)
        {
            assert(m_Head == 0);
            memcpy(pBuffer+iWriteLen, buffer_, iRemainLen);
            m_Head += iRemainLen;
        }
    } 

    return iLen;
}


// 내부 버퍼에서 pBuffer로 memcpy, 단 내부 head, tail은 변화하지 않는다.
int RingBufferEx::Peek(BYTE* pBuffer, int iLen)
{
    if (GetDataSize() < iLen)
    {
        // len이 크면 아예 실패로 처리한다.
        return -1;
    }

    if (m_Head < m_Tail)
    {
        // HooooooooooooT
        // ......HT......
        // ....HooooT....
        // HoooooooooT...

        // dataSize는 위에서 체크했으므로 충분하다.
        memcpy(pBuffer, buffer_+m_Head, iLen); 
    }
    else
    {
        // oooooooTHooooo   
        // T............H
        // T........Hoooo
        // ooooT........H
        // ooooT....Hoooo       

        // head부터 끝까지?
        int iWriteLen = min(iLen, m_BufferSize-m_Head);
        memcpy(pBuffer, buffer_+m_Head, iWriteLen);

        // 아직 더 써야되면 앞쪽부터 쓴다.
        int iRemainLen = iLen - iWriteLen;
        if (iRemainLen > 0)
        {
            memcpy(pBuffer+iWriteLen, buffer_, iRemainLen); 
        }
    } 

    return iLen;
}

int RingBufferEx::SkipHead(const int iLen)
{
    if (iLen >= 0)
    {
        // m_Head는 최대 DataSize 만큼 +로 움직일 수 있다.
        if (GetDataSize() < iLen) {
            return -1;
        }
    }
    else
    {
        // m_Head는 최대 FreeSize 만큼 -로 움직일 수 있다.
        if (GetFreeSize() < -iLen) {
            return -1;
        }
    }
    m_Head = _OffsetCtrl(m_Head, iLen);

    return iLen;
}

int RingBufferEx::SkipTail(const int iLen)
{
    if (iLen >= 0)
    {
        // m_Tail는 최대 FreeSize 만큼 +로 움직일 수 있다.
        if (GetFreeSize() < iLen) {
            return -1;
        }
    }
    else
    {
        // m_Tail는 최대 DataSize 만큼 -로 움직일 수 있다.
        if (GetDataSize() < -iLen) {
            return -1;
        }
    }
    m_Tail = _OffsetCtrl(m_Tail, iLen);

    return iLen;
}

BYTE* RingBufferEx::GetContigousPtr(IN int iSize)
{
    if (GetDataSize() < iSize) {
        return NULL;
    };
    if (m_Head <= m_Tail)
    {
        return (buffer_+m_Head);
    }
    else
    {
        int iRightUsedBufSize = (m_BufferSize-m_Head);
        if (iSize > iRightUsedBufSize)
        {
            //  ExtraBuffer 활용
            //  buffer_+m_BufferSize위치에다가 buffer_(처음데이터)를 모자라는 사이즈 만큼 복사 : 연결된 포인터를 만들기 위해
            memcpy(buffer_+m_BufferSize, buffer_, iSize - iRightUsedBufSize);
        }
        return (buffer_+m_Head);
    }
}


//==================================================================================================
//  <CCriticalSection>
CCriticalSection::CCriticalSection(DWORD dwSpinCount)
{
    if (0 != dwSpinCount) {
        ::InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount);
    }
    else {
        ::InitializeCriticalSection(&m_cs);
    }
}

CCriticalSection::~CCriticalSection()
{
    ::DeleteCriticalSection(&m_cs);
}

void CCriticalSection::Lock()
{
    ::EnterCriticalSection(&m_cs);
}

void CCriticalSection::Unlock()
{
    ::LeaveCriticalSection(&m_cs);
}


//==================================================================================================
//  <CSyncCriticalSection>
CSyncCriticalSection::CSyncCriticalSection(CCriticalSection& cs) : m_cs(cs)
{
    Lock();
}

CSyncCriticalSection::~CSyncCriticalSection()
{
    Unlock();
}

void CSyncCriticalSection::Lock()
{
    m_cs.Lock();
    m_bLocked = true;
}

void CSyncCriticalSection::Unlock()
{
    if (m_bLocked)
    {
        m_bLocked = false;
        m_cs.Unlock();
    }
}

} // end namespace module

