// wzqueue.h

#ifndef _PROGRAMCOMMON_WZQUEUE_H_
#define _PROGRAMCOMMON_WZQUEUE_H_

#include "wzlist.h"

//------------------------------------------------------------------------------
/**
    @class CWzQueue

    - Private 상속이다. 외부에서 CWzList멤버 함수 접근 불가
*/
template <class T>
class CWzQueue : private CWzList<T>
{
public:
    // 생성자/소멸자
    CWzQueue( void );
    virtual ~CWzQueue( void );

    // 데이타 추가(맨 끝)
    BOOL Insert( const T& data );
    // 데이타 꺼내기(맨 앞)
    T Remove( void );

    // 데이타 찾기
    BOOL Find( const T& findData );

    // 전체 데이타 제거
    void CleanUp( void );
    // 전체 데이타 수 얻기
    int GetCount( void ) const;
};

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzQueue<T>::CWzQueue( void )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzQueue<T>::~CWzQueue( void )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzQueue<T>::Insert( const T& data )
{
    return ( AddTail( data ) != NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
T CWzQueue<T>::Remove( void )
{
    return RemoveHead();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzQueue<T>::Find( const T& findData )
{
    return ( FindNode( findData ) != NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzQueue<T>::CleanUp( void )
{
    RemoveAll();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzQueue<T>::GetCount( void ) const
{
    return CWzList<T>::GetCount();
}

#endif // _PROGRAMCOMMON_WZQUEUE_H_