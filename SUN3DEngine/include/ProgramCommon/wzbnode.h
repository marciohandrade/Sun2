// wzbnode.h

#ifndef _PROGRAMCOMMON_WZBNODE_H_
#define _PROGRAMCOMMON_WZBNODE_H_

#include "wztypedef.h"

template <class T, class S> class CWzBTree;

//------------------------------------------------------------------------------
/**
    @class CWzBNode

    - �ܵ����δ� �� ���� ����, CWzBTree�� ���� ��
*/
template <class T, class S>
class CWzBNode
{
public:
    // ������/�Ҹ���
    CWzBNode( void );
    CWzBNode( const T& data, const S& cmpVal );
    ~CWzBNode( void );

    // ����Ÿ ����
    void SetData( const T& data );
    // ����Ÿ ���
    const T& GetData( void ) const;

    // �񱳰� ���
    const S& GetValue( void ) const;

private:
    // �񱳰� ����
    void SetValue( const S& cmpVal );

    // ���� ��� ����
    void SetLeft( CWzBNode<T, S>* left );
    // ���� ��� ���
    CWzBNode<T, S>* GetLeft( void ) const;

    // ������ ��� ����
    void SetRight( CWzBNode<T, S>* right );
    // ������ ��� ���
    CWzBNode<T, S>* GetRight( void ) const;

    // �θ� ��� ���
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