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

// ���� ���ۿ��ٰ� pBuffer�� memcpy
int RingBufferEx::Write(const BYTE* pBuffer, int iLen)
{
    // Tail�ڿ��ٰ� pBuffer�� len��ŭ �߰��ؾ� �ϴµ�.. 
    // �ϴ�.. buffer�� �����ϸ� ����!
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

        // tail ������ ����..
        int iRightFreeSize = m_BufferSize - m_Tail; // 0��° �ڸ��� ��������Ƿ� ������ ä���� �ȴ�.(tail -1 �����൵ �ȴ�.)
        int iWriteLen = min(iRightFreeSize, iLen);

        memcpy(buffer_+m_Tail, pBuffer, iWriteLen);
        m_Tail = (m_Tail + iWriteLen) % m_BufferSize;

        // ���� �������� ���ʺ��� ����.
        int iRemainLen = iLen - iWriteLen;
        if (iRemainLen > 0)
        {
            assert(m_Tail == 0); // ���� ���� ���� tail�� 0�� �ȴ�.
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

        // FreeSize�� üũ�����Ƿ� tail���� ���� ���� �ȴ�.
        memcpy(buffer_ + m_Tail, pBuffer, iLen);
        m_Tail += iLen;
    } 

    return iLen;
}

// ���� ���ۿ��� pBuffer�� memcpy
int RingBufferEx::Read(BYTE* pBuffer, int iLen)
{
    if (GetDataSize() < iLen)
    {
        // len�� ũ�� �ƿ� ���з� ó���Ѵ�.
        return -1;
    }

    if (m_Head < m_Tail)
    {
        // HooooooooooooT
        // ......HT......
        // ....HooooT....
        // HoooooooooT...

        // dataSize�� ������ üũ�����Ƿ� ����ϴ�.
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

        // head���� ������?
        int iWriteLen = min(iLen, m_BufferSize-m_Head);
        memcpy(pBuffer, buffer_+m_Head, iWriteLen);
        m_Head = (m_Head + iWriteLen) % m_BufferSize;

        // ���� �� ��ߵǸ� ���ʺ��� ����.
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


// ���� ���ۿ��� pBuffer�� memcpy, �� ���� head, tail�� ��ȭ���� �ʴ´�.
int RingBufferEx::Peek(BYTE* pBuffer, int iLen)
{
    if (GetDataSize() < iLen)
    {
        // len�� ũ�� �ƿ� ���з� ó���Ѵ�.
        return -1;
    }

    if (m_Head < m_Tail)
    {
        // HooooooooooooT
        // ......HT......
        // ....HooooT....
        // HoooooooooT...

        // dataSize�� ������ üũ�����Ƿ� ����ϴ�.
        memcpy(pBuffer, buffer_+m_Head, iLen); 
    }
    else
    {
        // oooooooTHooooo   
        // T............H
        // T........Hoooo
        // ooooT........H
        // ooooT....Hoooo       

        // head���� ������?
        int iWriteLen = min(iLen, m_BufferSize-m_Head);
        memcpy(pBuffer, buffer_+m_Head, iWriteLen);

        // ���� �� ��ߵǸ� ���ʺ��� ����.
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
        // m_Head�� �ִ� DataSize ��ŭ +�� ������ �� �ִ�.
        if (GetDataSize() < iLen) {
            return -1;
        }
    }
    else
    {
        // m_Head�� �ִ� FreeSize ��ŭ -�� ������ �� �ִ�.
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
        // m_Tail�� �ִ� FreeSize ��ŭ +�� ������ �� �ִ�.
        if (GetFreeSize() < iLen) {
            return -1;
        }
    }
    else
    {
        // m_Tail�� �ִ� DataSize ��ŭ -�� ������ �� �ִ�.
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
            //  ExtraBuffer Ȱ��
            //  buffer_+m_BufferSize��ġ���ٰ� buffer_(ó��������)�� ���ڶ�� ������ ��ŭ ���� : ����� �����͸� ����� ����
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

