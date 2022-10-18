// wzbintree.h

#ifndef _PROGRAMCOMMON_WZBINTREE_H_
#define _PROGRAMCOMMON_WZBINTREE_H_

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzBinTree
*/
template <class T>
class CWzBinTree
{
public:
    // 생성자/소멸자
    CWzBinTree( void );
    ~CWzBinTree( void );

    // 왼쪽에 추가(추가된 트리 반환)
    CWzBinTree<T>* AddLeft( void );
    // 오른쪽에 추가(추가된 트리 반환)
    CWzBinTree<T>* AddRight( void );

    // 왼쪽 트리 설정
    void SetLeft( CWzBinTree<T>* left );
    // 왼쪽 트리 얻기
    CWzBinTree<T>* GetLeft( void ) const;

    // 오른쪽 트리 설정
    void SetRight( CWzBinTree<T>* right );
    // 오른쪽 트리 얻기
    CWzBinTree<T>* GetRight( void ) const;

    // 부모 트리 얻기
    CWzBinTree<T>* GetParent( void ) const;

    // 데이타 설정
    void SetData( const T& data );
    // 데이타 얻기
    const T& GetData( void ) const;

    // 전체 제거
    void Destroy( void );

private:
    // 왼쪽 트리 제거
    void DeleteLeft( void );
    // 오른쪽 트리 제거
    void DeleteRight( void );

private:
    CWzBinTree<T>* m_left;
    CWzBinTree<T>* m_right;
    CWzBinTree<T>* m_parent;

    T m_data;
};

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>::CWzBinTree( void )
: m_left( NULL )
, m_right( NULL )
, m_parent( NULL )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>::~CWzBinTree( void )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::Destroy( void )
{
    DeleteLeft();
    DeleteRight();
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::DeleteLeft( void )
{
    if( m_left )
    {
        m_left->Destroy();

        delete m_left;
        m_left = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::DeleteRight( void )
{
    if( m_right )
    {
        m_right->Destroy();

        delete m_right;
        m_right = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>* CWzBinTree<T>::AddLeft( void )
{
    CWzBinTree<T>* newTree = new CWzBinTree<T>;
    WzAssert( newTree );

    SetLeft( newTree );

    return newTree;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>* CWzBinTree<T>::AddRight( void )
{
    CWzBinTree<T>* newTree = new CWzBinTree<T>;
    WzAssert( newTree );

    SetRight( newTree );

    return newTree;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::SetLeft( CWzBinTree<T>* left )
{
    DeleteLeft();

    if( left )
    {
        m_left = left;
        left->m_parent = this; 
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>* CWzBinTree<T>::GetLeft( void ) const
{ 
    return m_left;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::SetRight( CWzBinTree<T>* right )
{ 
    DeleteRight();

    if( right )
    {
        m_right = right;
        right->m_parent = this; 
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>* CWzBinTree<T>::GetRight( void ) const
{ 
    return m_right;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzBinTree<T>* CWzBinTree<T>::GetParent( void ) const
{ 
    return m_parent; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzBinTree<T>::SetData( const T& data )
{ 
    m_data = data; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
const T& CWzBinTree<T>::GetData( void ) const
{
    return m_data;
}

#endif // _PROGRAMCOMMON_WZBINTREE_H_