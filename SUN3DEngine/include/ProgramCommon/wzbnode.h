// wzbnode.h

#ifndef _PROGRAMCOMMON_WZBNODE_H_
#define _PROGRAMCOMMON_WZBNODE_H_

#include "wztypedef.h"

template <class T, class S> class CWzBTree;

//------------------------------------------------------------------------------
/**
    @class CWzBNode

    - 단독으로는 쓸 일이 없고, CWzBTree랑 같이 씀
*/
template <class T, class S>
class CWzBNode
{
public:
    // 생성자/소멸자
    CWzBNode( void );
    CWzBNode( const T& data, const S& cmpVal );
    ~CWzBNode( void );

    // 데이타 설정
    void SetData( const T& data );
    // 데이타 얻기
    const T& GetData( void ) const;

    // 비교값 얻기
    const S& GetValue( void ) const;

private:
    // 비교값 설정
    void SetValue( const S& cmpVal );

    // 왼쪽 노드 설정
    void SetLeft( CWzBNode<T, S>* left );
    // 왼쪽 노드 얻기
    CWzBNode<T, S>* GetLeft( void ) const;

    // 오른쪽 노드 설정
    void SetRight( CWzBNode<T, S>* right );
    // 오른쪽 노드 얻기
    CWzBNode<T, S>* GetRight( void ) const;

    // 부모 노드 얻기
    CWzBNode<T, S>* GetParent( void ) const;

private:
    friend class CWzBTree<T, S>;

    CWzBNode<T, S>* m_left;
    CWzBNode<T, S>* m_right;
    CWzBNode<T, S>* m_parent;

    T m_data;
    S m_cmpVal;
};

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>::CWzBNode( void )
: m_left( NULL )
, m_right( NULL )
, m_parent( NULL )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>::CWzBNode( const T& data, const S& cmpVal )
: m_left( NULL )
, m_right( NULL )
, m_parent( NULL )
{
    m_data = data;
    m_cmpVal = cmpVal;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>::~CWzBNode( void )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBNode<T, S>::SetData( const T& data )
{
    m_data = data; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const T& CWzBNode<T, S>::GetData( void ) const
{ 
    return m_data;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBNode<T, S>::SetValue( const S& cmpVal )
{
    m_cmpVal = cmpVal; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const S& CWzBNode<T, S>::GetValue( void ) const
{
    return m_cmpVal; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBNode<T, S>::SetLeft( CWzBNode<T, S>* left )
{
    m_left = left; 

    if( left )
    {
        left->m_parent = this; 
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBNode<T, S>::GetLeft( void ) const
{
    return m_left; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBNode<T, S>::SetRight( CWzBNode<T, S>* right )
{
    m_right = right;

    if( right )
    {
        right->m_parent = this;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBNode<T, S>::GetRight( void ) const
{
    return m_right; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBNode<T, S>::GetParent( void ) const
{
    return m_parent; 
}

#endif // _PROGRAMCOMMON_WZBNODE_H_