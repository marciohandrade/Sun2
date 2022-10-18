#pragma once
#ifndef NETWORK_MODULE_RINGBUFFER_EX_H
#define NETWORK_MODULE_RINGBUFFER_EX_H

namespace module {
;

//==================================================================================================
//  RingBufferEx class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
    - ������ ������(RingBuffer)�� �ٸ� ����� ������
    @note
    - 
    @history 
    - 
*/
//==================================================================================================
//
// Socket���� data�� �ְ�ޱ� ���� �����صδ� buffer(circular queue)
//
// m_Head==m_Tail : empty
// m_Head==m_Tail+1 : full (m_Head > m_Tail)
// m_Head==0, m_Tail==m_BufferSize-1 : full (m_Head < m_Tail)
// (!) m_Tail �ڸ��� �׻� empty�̴�.
//
class RingBufferEx
{
public:
    RingBufferEx(int buffer_size = 0, int iExtraSize = 0);
    virtual ~RingBufferEx();

    void Init(int buffer_size, int iExtraSize);
    void Release();
    void Recycle();

    int Write(const BYTE* pBuffer, int iLen); // ���� buffer�� ����.
    int Read(BYTE* pBuffer, int iLen); // ���� buffer���� �о�´�.
    int Peek(BYTE* pBuffer, int iLen); // Read�ε� head, tail�� ��ȭ����.

    bool IsEmpty() const         { return m_Head == m_Tail; }
    bool IsFull() const          { return GetFreeSize() == 0; }
    int GetBufferSize() const   { return m_BufferSize; }

    int GetDataSize() const     { return m_Head <= m_Tail? m_Tail-m_Head : m_BufferSize-m_Head+m_Tail; }
    int GetFreeSize() const     { return GetBufferSize() - GetDataSize() - 1; } // -1 for tail position

    int SkipHead(const int iLen);
    int SkipTail(const int iLen);
    int GetHead() { return m_Head; }
    int GetTail() { return m_Tail; }
    BYTE* GetBufferPtr() { return buffer_; }
    //  <WARNING>
    //  iSize�� �ܺο��� ExtraBuffer�� ������� �ʰ� ��û�ϰ� �Ǹ� ������ �����.
    //  �� ���� �ܺ� ���� ���������� ���ܵд�.
    BYTE* GetContigousPtr(IN int iSize);

private:
    int _OffsetCtrl(const int curPole, const int offset)
    {
        const int iOffset = (curPole + offset);
        return iOffset < 0 ? iOffset+m_BufferSize : iOffset%m_BufferSize;
    }

protected :
    BYTE* buffer_;
    int m_BufferSize;
    int m_ExtraSize;
    int m_Head;
    int m_Tail;
};

//==================================================================================================
//
class CCriticalSection 
{
public:
    CCriticalSection(DWORD dwSpinCount = 0);
    virtual ~CCriticalSection();

    void Lock();
    void Unlock();

private:
    CRITICAL_SECTION    m_cs;
};

//==================================================================================================
//
class CSyncCriticalSection
{
public:
    CSyncCriticalSection(CCriticalSection& cs);
    virtual ~CSyncCriticalSection();

    void Lock();
    void Unlock();
private:
    bool m_bLocked;
    CCriticalSection&   m_cs;

private:
    CSyncCriticalSection(const CSyncCriticalSection&);
    void operator=(const CSyncCriticalSection&);
};

//==================================================================================================

}; //end of namespace 'module'

#endif //NETWORK_MODULE_RINGBUFFER_EX_H
