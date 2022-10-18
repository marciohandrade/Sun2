// wzhandlemanager.h

#ifndef _PROGRAMCOMMON_WZHANDLEMANAGER_H_
#define _PROGRAMCOMMON_WZHANDLEMANAGER_H_

#include "wzarray.h"

#define MAX_NUM_HANDLES     65535
#define MAX_MAGIC_NO        65534
#define MIN_NUM_FOR_CLEAR   1000

struct St_Handle
{
    union
    {
        struct
        {
            unsigned short m_index;
            unsigned short m_magicNo;
        };

        HANDLE m_handle;
    };
};

//------------------------------------------------------------------------------
/**
    @class CWzHandleManager
*/
template <class T>
class CWzHandleManager
{
public:
    // ������/�Ҹ���
    CWzHandleManager( void );
    virtual ~CWzHandleManager( void );

    // �ʱ�ȭ
    void Clear( void );

    // ����Ÿ �߰�(�ڵ� ��ȯ)
    HANDLE Add( const T& data );
    // ����Ÿ ����
    BOOL Release( HANDLE handle );

    // ����Ÿ ���
    const T& GetData( HANDLE handle ) const;
    // ����Ÿ �ٲٱ�
    BOOL Replace( HANDLE handle, const T& data );

    // ��ü ����Ÿ ��
    int Idx_GetCount( void ) const;
    // �ε����� ����Ÿ ���
    const T& Idx_GetData( int index ) const;
    // �ε����� ���� �ѹ� ���
    unsigned short Idx_GetMagicNumber( int index ) const;
    // �ε����� ����Ÿ ����
    void Idx_Release( int index );

    // ��ȿ�� �ڵ��ΰ�?
    BOOL IsValidHandle( HANDLE handle ) const;

protected:
    // xxx: �����ϰ� �̷� ������ ����� ���� ������
    // ������ �̹� ������ ���� �����ؾ� �ϴ� ��찡 �ֱ� ������
    // ���� �� ����� ���� �� ���� �̷��� ����.
    static T m_dummyData;
    static T m_nullData;

protected:
    CWzArray<T> m_dataList;
    CWzArray<unsigned short> m_magicNoList;

private:
    CWzArray<unsigned short> m_freeIdxList;
    unsigned short m_numFreeIdx;

    unsigned short m_maxIndex;
    unsigned short m_newMagicNo;
};

template <class T>
T CWzHandleManager<T>::m_dummyData;

template <class T>
T CWzHandleManager<T>::m_nullData;

//------------------------------------------------------------------------------
/**
    - m_newMagicNo( 1 )
    - ���� �ѹ��� 1���� �����Ѵٴ� ���� �����ϱ� ���� 1�� �ʱ�ȭ ��
*/
template <class T>
CWzHandleManager<T>::CWzHandleManager( void )
{
    // xxx: T�� ���� �� �� �𸣴� ��Ȳ���� memset�� �Ѵٴ� ��ü��
    // ������ ����������, ������ �̹� �׷� ��Ȳ�� �������� ������
    // ���� �� ����� �ּ��� �� ����, ��� ������ �ΰ� ���⼭ �ʱ�ȭ ��
    memset( &m_nullData, 0, sizeof( T ) );
    memset( &m_dummyData, 0, sizeof( T ) );

    // ���� ����
    //m_dataList.EnlargeSize( MAX_NUM_HANDLES );
    //m_magicNoList.EnlargeSize( MAX_NUM_HANDLES );
    //m_freeIdxList.EnlargeSize( MAX_NUM_HANDLES );

    // �ʱ�ȭ
    Clear();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzHandleManager<T>::~CWzHandleManager( void )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzHandleManager<T>::Clear( void )
{
    m_numFreeIdx = 0;
    m_maxIndex = 0;
    m_newMagicNo = 1;

    m_dataList.Clear();
    m_magicNoList.Clear();

    m_freeIdxList.Clear();
}

//------------------------------------------------------------------------------
/**
    ����Ÿ �߰�

    - index : 0 ~ 65534
    - magic No : 1 ~ 65534
*/
template <class T>
HANDLE CWzHandleManager<T>::Add( const T& data )
{
    St_Handle sth;

    // ������ �ε����� �ִ� ���
    if( m_numFreeIdx > 0 )
    {
        sth.m_index = m_freeIdxList.Get( --m_numFreeIdx );
    }
    else
    {
        // ���� �Ҵ��� ���
        if( m_maxIndex >= MAX_NUM_HANDLES )
        {
            return INVALID_HANDLE_VALUE;
        }

        sth.m_index = m_maxIndex++;
    }

    // ���� �ѹ�
    if( m_newMagicNo > MAX_MAGIC_NO )
    {
        // ���� �ѹ��� 0 �� �ȵǰ� 0xFFFF �� �ȵǾ� �������� üũ�� �� �ִ�.
        m_newMagicNo = 1;
    }

    sth.m_magicNo = m_newMagicNo++;

    // ���� ����
    m_dataList.Set( sth.m_index, data );
    m_magicNoList.Set( sth.m_index, sth.m_magicNo );

    return sth.m_handle;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzHandleManager<T>::Release( HANDLE handle )
{
    // �ڵ� ��ȿ�� �˻�
    if( !IsValidHandle( handle ) )
    {
        return FALSE;
    }

    St_Handle sth;
    sth.m_handle = handle;

    // ���� �ѹ� �ʱ�ȭ
    m_magicNoList.Set( sth.m_index, 0 );

    // xxx : ��~ �� ������;
    // ��ư ���� �ȵ����� �ϴ��� �̷��� ����.
    // ���� �ѹ��� 0�� ���� ������ ���ε� �ܺο��� �̰� �����ϰ�
    // Idx_ �迭 �Լ��� ���� ����Ÿ�� ��� ��찡 �־,
    // ��¿ �� ���� ���⼭ �ʱ�ȭ�� �� �ش�.
    m_dataList.Set( sth.m_index, m_nullData );

    // ���� �ε��� ����Ʈ�� �߰�
    WzAssert( m_numFreeIdx < m_maxIndex );

    m_freeIdxList.Set( m_numFreeIdx, sth.m_index );
    ++m_numFreeIdx;

    // ������ ���� �ִ� �ε����� ũ�ų� ���� ���, �ʱ�ȭ
    // ��, �ʹ� ���� ȣ��Ǵ� ���� ���� ���� �ּ����� ���� �д�.
    if( m_numFreeIdx > MIN_NUM_FOR_CLEAR && m_numFreeIdx >= m_maxIndex )
    {
        Clear();
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
const T& CWzHandleManager<T>::GetData( HANDLE handle ) const
{
    if( !IsValidHandle( handle ) )
    {
        return m_dummyData;
    }

    St_Handle sth;
    sth.m_handle = handle;

    return m_dataList.Get( sth.m_index );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzHandleManager<T>::Replace( HANDLE handle, const T& data )
{
    if( !IsValidHandle( handle ) )
    {
        return FALSE;
    }
    
    St_Handle sth;
    sth.m_handle = handle;

    m_dataList.Set( sth.m_index, data );

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzHandleManager<T>::IsValidHandle( HANDLE handle ) const
{
    St_Handle sth;
    sth.m_handle = handle;

    // ��ȿ �ڵ� ����, ���� �ѹ� üũ
    if( handle == INVALID_HANDLE_VALUE || sth.m_magicNo == 0 )
    {
        return FALSE;
    }

    // �ش� �ε����� ���� �ѹ��� ���
    unsigned short magicNo = m_magicNoList.Get( sth.m_index );

    // �־��� �ڵ��� �����ѹ��� ����� ���� �ѹ��� ��
    if( sth.m_magicNo != magicNo )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzHandleManager<T>::Idx_GetCount( void ) const
{
    return m_dataList.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
const T& CWzHandleManager<T>::Idx_GetData( int index ) const
{
    return m_dataList.Get( index );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
unsigned short CWzHandleManager<T>::Idx_GetMagicNumber( int index ) const
{
    return m_magicNoList.Get( index );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzHandleManager<T>::Idx_Release( int index )
{
    St_Handle sth;
    sth.m_index = index; 
    sth.m_magicNo = Idx_GetMagicNumber( index );
    
    Release( sth.m_handle );
}

#endif // _PROGRAMCOMMON_WZHANDLEMANAGER_H_