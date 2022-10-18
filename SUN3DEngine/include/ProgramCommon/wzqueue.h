// wzqueue.h

#ifndef _PROGRAMCOMMON_WZQUEUE_H_
#define _PROGRAMCOMMON_WZQUEUE_H_

#include "wzlist.h"

//------------------------------------------------------------------------------
/**
    @class CWzQueue

    - Private ����̴�. �ܺο��� CWzList��� �Լ� ���� �Ұ�
*/
template <class T>
class CWzQueue : private CWzList<T>
{
public:
    // ������/�Ҹ���
    CWzQueue( void );
    virtual ~CWzQueue( void );

    // ����Ÿ �߰�(�� ��)
    BOOL Insert( const T& data );
    // ����Ÿ ������(�� ��)
    T Remove( void );

    // ����Ÿ ã��
    BOOL Find( const T& findData );

    // ��ü ����Ÿ ����
    void CleanUp( void );
    // ��ü ����Ÿ �� ���
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