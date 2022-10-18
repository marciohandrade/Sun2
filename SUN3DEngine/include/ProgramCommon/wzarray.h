// wzarray.h

#ifndef _PROGRAMCOMMON_WZARRAY_H_
#define _PROGRAMCOMMON_WZARRAY_H_

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzArray
*/
template <class T>
class CWzArray
{
public:
    // ������/�Ҹ���
    CWzArray( void );
    CWzArray( const CWzArray<T>& rhs );
    ~CWzArray( void );

    // ��� ����Ÿ �ʱ�ȭ
    void Clear( void );

    // ����Ÿ ������ �ʱ�ȭ
    void InvalidateAll( void );
    // ���� ����Ÿ ���� �� �⺻ ������� ���Ҵ�
    void InvalidateAndShrink( void );
    // �ش� �ε������� ��� ����Ÿ ��ȿȭ
    void InvalidateFrom( int index );

    // �ش� �����ŭ �޸� Ȯ��(2�� �¼�)
    void EnlargeSize( int size );

    // �ش� �ε����� ����Ÿ ����
    void Set( int index, const T& data );
    // xxx: ���������� �� index ���̴� NULL�� �ʱ�ȭ
    void SetE( int index, const T& data );
    // �ش� �ε����� ����Ÿ ���
    T& Get( int index ) const;

    // ���� ����Ÿ �߰�
    int Add( const T& data );
    // ���� 3�� ����Ÿ �߰�
    int Adds( const T& data1, const T& data2, const T& data3 );
    // ���� �ش� ����ŭ ����Ÿ�� �߰�
    int Adds( int cnt, const T* data );

    // �ش� �ε����� ����Ÿ ����
    void Remove( int index );

    // ����Ÿ ã��(�ε��� ��ȯ)
    int Find( const T& data ) const;
    // ��ü ����Ÿ �� ���
    int GetCount( void ) const;

    // ����Ÿ �迭 ������ ���
    T* GetBufferPtr( void ) const;

    // operator =
    CWzArray<T>& operator = ( const CWzArray<T>& rhs );

private:
    // �ش� �ε�����ŭ �޸� Ȯ��
    void MakeSureDimensionSize( int index );
    // ����Ÿ ����
    void Release( void );

private:
    // xxx: �����ϰ� �̷� ������ ����� ���� ������
    // ������ �̹� ������ ���� �����ؾ� �ϴ� ��찡 �ֱ� ������
    // ���� �� ����� ���� �� ���� �̷��� ����.
    static T m_dummyData;

private:
    int m_numData;
    int m_size;

    T* m_data;
};

template <class T>
T CWzArray<T>::m_dummyData;

//------------------------------------------------------------------------------
/**
    �����ڿ��� ������ �����Ѵٴ� ���� ���� �������� ������
    ������ �̹� �׷��� �� �Ա� ������ �׳� ����.
*/
template <class T>
CWzArray<T>::CWzArray( void )
: m_numData( 0 )
, m_size( 16 )
{
    m_data = new T[m_size];
    WzAssert( m_data );

    memset(m_data, 0, sizeof( T ) * m_size);
    memset(&(CWzArray<T>::m_dummyData), 0, sizeof( T ));

}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzArray<T>::CWzArray( const CWzArray<T>& rhs )
: m_numData( 0 )
, m_size( 0 )
, m_data( NULL )
{
    *this = rhs;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzArray<T>::~CWzArray( void )
{
    Release();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::Release( void )
{
    if( m_data )
    {
        delete [] m_data;
        m_data = NULL;
    }

    m_size = 0;
    m_numData = 0;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::Clear( void )
{
    WzAssert( m_data );

    memset( m_data, 0, sizeof( T ) * m_size );

    m_numData = 0;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::InvalidateAll( void )
{
    m_numData = 0;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::InvalidateAndShrink( void )
{
    Release();

    m_numData = 0;
    m_size = 16;
    m_data = new T[m_size];

    memset(m_data, 0, sizeof( T ) * m_size);

    WzAssert( m_data );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::InvalidateFrom( int index )
{
    m_numData = min( index, m_numData );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::MakeSureDimensionSize( int index )
{
    if( index >= m_size )
    {   
        // �������� ũ�⸦ �÷��ش�.
        int newSize = ( m_size > 0 ? m_size * 2 : 16 );
        while( newSize <= index )
        {
            newSize *= 2;
        }

        // ���ο� �����ŭ �ӽ� ���� ����
        T* tmpBuf = new T[newSize];
        WzAssert( tmpBuf );

        memset(tmpBuf, 0, sizeof( T ) * newSize);

        // ���� ����Ÿ�� �ִ� ��� ����
        if( m_data )
        {
            memcpy( tmpBuf, m_data, sizeof( T ) * m_size );
            delete [] m_data;
        }

        m_data = tmpBuf;
        m_size = newSize;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::EnlargeSize( int size )
{
    MakeSureDimensionSize( size );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::Set( int index, const T& data )
{
    MakeSureDimensionSize( index );

    WzAssert( m_data );

    m_data[index] = data;
    m_numData = max( index + 1, m_numData );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::SetE( int index, const T& data )
{
    // ���� ����Ÿ �� ����
    int prevNum = m_numData;
    WzAssert( prevNum >= 0 );

    // ����Ÿ ����
    Set( index, data );

    // �� index�� ���� ����Ÿ ������ ū ���
    if( index > prevNum )
    {
        // �߰��� NULL�� ����
        for( int i = prevNum; i < index; ++i )
        {
            m_data[i] = NULL;
        }
    }
}

//------------------------------------------------------------------------------
/**
    xxx : index�� ��ȿ ������ ��𼭺��� �������ΰ�?

    1. (0 ~ m_numData) : ������ ���� ������ ������
    2. (0 ~ m_size) : ���� ũ�����
    
    �ϴ� 1���� ��� ���� ���ο��� �������ϴ� �ٶ��� �������� 2������ üũ�ߴ�.
*/
template <class T>
T& CWzArray<T>::Get( int index ) const
{
    // WzAssert( index >= 0 && index < m_numData );

    // TODO: �߸��� Object�� �־ ��� WzAssert �� �߻��Ͽ� �ϴ� ���Ƶξ���(���߿� Ǯ���ּ��� by kiki)
    // WzAssert( index >= 0 && index < m_size );

    // if( index >= 0 && index < m_numData )
    if( index >= 0 && index < m_size )
    {
        WzAssert( m_data );

        return m_data[index];
    }

    // WZLOG( WZWAR, "CWzArray::Get() - ����!! ������ ����Ÿ ��ȯ (�ε���:%d, ����Ÿ��:%d)", index, m_numData );
    WZLOG( WZWAR, "CWzArray::Get() - ����!! ������ ����Ÿ ��ȯ (�ε���:%d, ����ũ��:%d)", index, m_size );

    return m_dummyData;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzArray<T>::Add( const T& data )
{
    Set( m_numData, data );

    return ( m_numData - 1 );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzArray<T>::Adds( const T& data1, const T& data2, const T& data3 )
{
    MakeSureDimensionSize( m_numData + 2 );

    WzAssert( m_data );

    T* refData = &m_data[m_numData];
    WzAssert( refData );

    refData[0] = data1;
    refData[1] = data2;
    refData[2] = data3;

    m_numData += 3;

    return ( m_numData - 1 );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzArray<T>::Adds( int cnt, const T* data )
{
    WzAssert( data );

    MakeSureDimensionSize( m_numData + cnt - 1 );

    WzAssert( m_data );

    T* refData = &m_data[m_numData];
    WzAssert( refData );

    for( int i = 0; i < cnt; ++i )
    {
        refData[i] = data[i];
    }

    m_numData += cnt;

    return ( m_numData - 1 );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzArray<T>::Find( const T& data ) const
{
    WzAssert( m_data );

    for( int i = 0; i < m_numData; ++i )
    {
        if( m_data[i] == data )
        {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzArray<T>::Remove( int index )
{
    WzAssert( index >= 0 && index < m_numData );

    if( index >= 0 && index < m_numData )
    {
        WzAssert( m_data );

        memmove( m_data + index, m_data + index + 1, sizeof( T ) * ( m_numData - index - 1 ) );
        --m_numData;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzArray<T>::GetCount( void ) const
{ 
    return m_numData; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
T* CWzArray<T>::GetBufferPtr( void ) const
{ 
    return m_data; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzArray<T>& CWzArray<T>::operator = ( const CWzArray<T>& rhs )
{
    if( &rhs == this )
    {
        return *this;
    }

    Release();

    int size = ( rhs.m_size > 0 ? rhs.m_size : 16 );
    WzAssert( rhs.m_numData <= size );

    m_data = new T[size];
    WzAssert( m_data );

    memset(m_data, 0, sizeof( T ) * size);

    memcpy( m_data, rhs.m_data, sizeof( T ) * rhs.m_numData );

    m_size = size;
    m_numData = rhs.m_numData;

    return *this;
}

#endif // _PROGRAMCOMMON_WZARRAY_H_