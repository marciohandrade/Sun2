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
    // 생성자/소멸자
    CWzArray( void );
    CWzArray( const CWzArray<T>& rhs );
    ~CWzArray( void );

    // 모든 데이타 초기화
    void Clear( void );

    // 데이타 갯수만 초기화
    void InvalidateAll( void );
    // 기존 데이타 제거 후 기본 사이즈로 재할당
    void InvalidateAndShrink( void );
    // 해당 인덱스부터 모든 데이타 무효화
    void InvalidateFrom( int index );

    // 해당 사이즈만큼 메모리 확보(2의 승수)
    void EnlargeSize( int size );

    // 해당 인덱스에 데이타 설정
    void Set( int index, const T& data );
    // xxx: 끝에서부터 새 index 사이는 NULL로 초기화
    void SetE( int index, const T& data );
    // 해당 인덱스의 데이타 얻기
    T& Get( int index ) const;

    // 끝에 데이타 추가
    int Add( const T& data );
    // 끝에 3개 데이타 추가
    int Adds( const T& data1, const T& data2, const T& data3 );
    // 끝에 해당 수만큼 데이타들 추가
    int Adds( int cnt, const T* data );

    // 해당 인덱스의 데이타 제거
    void Remove( int index );

    // 데이타 찾기(인덱스 반환)
    int Find( const T& data ) const;
    // 전체 데이타 수 얻기
    int GetCount( void ) const;

    // 데이타 배열 포인터 얻기
    T* GetBufferPtr( void ) const;

    // operator =
    CWzArray<T>& operator = ( const CWzArray<T>& rhs );

private:
    // 해당 인덱스만큼 메모리 확보
    void MakeSureDimensionSize( int index );
    // 데이타 해제
    void Release( void );

private:
    // xxx: 구차하게 이런 변수를 만들고 싶진 않지만
    // 기존에 이미 쓰레기 값을 리턴해야 하는 경우가 있기 때문에
    // 차라리 이 방법이 좋은 것 같아 이렇게 간다.
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
    생성자에서 뭔가를 생성한다는 것이 좋아 보이지는 않지만
    기존에 이미 그렇게 써 왔기 때문에 그냥 간다.
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
        // 넘쳤으면 크기를 늘려준다.
        int newSize = ( m_size > 0 ? m_size * 2 : 16 );
        while( newSize <= index )
        {
            newSize *= 2;
        }

        // 새로운 사이즈만큼 임시 버퍼 생성
        T* tmpBuf = new T[newSize];
        WzAssert( tmpBuf );

        memset(tmpBuf, 0, sizeof( T ) * newSize);

        // 기존 데이타가 있는 경우 복사
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
    // 기존 데이타 수 저장
    int prevNum = m_numData;
    WzAssert( prevNum >= 0 );

    // 데이타 설정
    Set( index, data );

    // 새 index가 기존 데이타 수보다 큰 경우
    if( index > prevNum )
    {
        // 중간을 NULL로 설정
        for( int i = prevNum; i < index; ++i )
        {
            m_data[i] = NULL;
        }
    }
}

//------------------------------------------------------------------------------
/**
    xxx : index의 유효 범위는 어디서부터 어디까지인가?

    1. (0 ~ m_numData) : 유저가 값을 설정한 곳까지
    2. (0 ~ m_size) : 버퍼 크기까지
    
    일단 1번의 경우 엔진 내부에서 오동작하는 바람에 기존데로 2번으로 체크했다.
*/
template <class T>
T& CWzArray<T>::Get( int index ) const
{
    // WzAssert( index >= 0 && index < m_numData );

    // TODO: 잘못된 Object가 있어서 계속 WzAssert 가 발생하여 일단 막아두었음(나중에 풀어주세요 by kiki)
    // WzAssert( index >= 0 && index < m_size );

    // if( index >= 0 && index < m_numData )
    if( index >= 0 && index < m_size )
    {
        WzAssert( m_data );

        return m_data[index];
    }

    // WZLOG( WZWAR, "CWzArray::Get() - 실패!! 쓰레기 데이타 반환 (인덱스:%d, 데이타수:%d)", index, m_numData );
    WZLOG( WZWAR, "CWzArray::Get() - 실패!! 쓰레기 데이타 반환 (인덱스:%d, 버퍼크기:%d)", index, m_size );

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