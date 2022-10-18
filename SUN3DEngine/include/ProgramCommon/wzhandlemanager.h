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
    // 생성자/소멸자
    CWzHandleManager( void );
    virtual ~CWzHandleManager( void );

    // 초기화
    void Clear( void );

    // 데이타 추가(핸들 반환)
    HANDLE Add( const T& data );
    // 데이타 제거
    BOOL Release( HANDLE handle );

    // 데이타 얻기
    const T& GetData( HANDLE handle ) const;
    // 데이타 바꾸기
    BOOL Replace( HANDLE handle, const T& data );

    // 전체 데이타 수
    int Idx_GetCount( void ) const;
    // 인덱스로 데이타 얻기
    const T& Idx_GetData( int index ) const;
    // 인덱스로 매직 넘버 얻기
    unsigned short Idx_GetMagicNumber( int index ) const;
    // 인덱스로 데이타 해제
    void Idx_Release( int index );

    // 유효한 핸들인가?
    BOOL IsValidHandle( HANDLE handle ) const;

protected:
    // xxx: 구차하게 이런 변수를 만들고 싶진 않지만
    // 기존에 이미 쓰레기 값을 리턴해야 하는 경우가 있기 때문에
    // 차라리 이 방법이 좋은 것 같아 이렇게 간다.
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
    - 매직 넘버가 1부터 시작한다는 것을 강조하기 위해 1로 초기화 함
*/
template <class T>
CWzHandleManager<T>::CWzHandleManager( void )
{
    // xxx: T가 뭐가 될 지 모르는 상황에서 memset을 한다는 자체가
    // 굉장히 위험하지만, 기존에 이미 그런 상황이 벌어졌기 때문에
    // 차라리 이 방법이 최선인 것 같아, 멤버 변수로 두고 여기서 초기화 함
    memset( &m_nullData, 0, sizeof( T ) );
    memset( &m_dummyData, 0, sizeof( T ) );

    // 버퍼 생성
    //m_dataList.EnlargeSize( MAX_NUM_HANDLES );
    //m_magicNoList.EnlargeSize( MAX_NUM_HANDLES );
    //m_freeIdxList.EnlargeSize( MAX_NUM_HANDLES );

    // 초기화
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
    데이타 추가

    - index : 0 ~ 65534
    - magic No : 1 ~ 65534
*/
template <class T>
HANDLE CWzHandleManager<T>::Add( const T& data )
{
    St_Handle sth;

    // 해제된 인덱스가 있는 경우
    if( m_numFreeIdx > 0 )
    {
        sth.m_index = m_freeIdxList.Get( --m_numFreeIdx );
    }
    else
    {
        // 새로 할당할 경우
        if( m_maxIndex >= MAX_NUM_HANDLES )
        {
            return INVALID_HANDLE_VALUE;
        }

        sth.m_index = m_maxIndex++;
    }

    // 매직 넘버
    if( m_newMagicNo > MAX_MAGIC_NO )
    {
        // 매직 넘버는 0 도 안되고 0xFFFF 도 안되야 정상인지 체크할 수 있다.
        m_newMagicNo = 1;
    }

    sth.m_magicNo = m_newMagicNo++;

    // 정보 저장
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
    // 핸들 유효성 검사
    if( !IsValidHandle( handle ) )
    {
        return FALSE;
    }

    St_Handle sth;
    sth.m_handle = handle;

    // 매직 넘버 초기화
    m_magicNoList.Set( sth.m_index, 0 );

    // xxx : 아~ 골 때리네;
    // 하튼 맘에 안들지만 일단은 이렇게 가자.
    // 매직 넘버가 0인 것은 쓰레기 값인데 외부에서 이걸 무시하고
    // Idx_ 계열 함수를 통해 데이타를 얻는 경우가 있어서,
    // 어쩔 수 없이 여기서 초기화를 해 준다.
    m_dataList.Set( sth.m_index, m_nullData );

    // 해제 인덱스 리스트에 추가
    WzAssert( m_numFreeIdx < m_maxIndex );

    m_freeIdxList.Set( m_numFreeIdx, sth.m_index );
    ++m_numFreeIdx;

    // 해제된 수와 최대 인덱스가 크거나 같은 경우, 초기화
    // 단, 너무 자주 호출되는 것을 막기 위해 최소한의 값을 둔다.
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

    // 유효 핸들 여부, 매직 넘버 체크
    if( handle == INVALID_HANDLE_VALUE || sth.m_magicNo == 0 )
    {
        return FALSE;
    }

    // 해당 인덱스의 매직 넘버를 얻고
    unsigned short magicNo = m_magicNoList.Get( sth.m_index );

    // 주어진 핸들의 매직넘버와 저장된 매직 넘버와 비교
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