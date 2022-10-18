// wzbtree.h

#ifndef _PROGRAMCOMMON_WZBTREE_H_
#define _PROGRAMCOMMON_WZBTREE_H_

#include "wzbnode.h"

//------------------------------------------------------------------------------
/**
    @class CWzBTree
*/
template <class T, class S>
class CWzBTree
{
private:
    struct ProcParam
    {
        T* data;
        S* cmpVal;
        long cnt;
    };

public:
    // ������/�Ҹ���
    CWzBTree( void );
    CWzBTree( const CWzBTree<T, S>& rhs );
    ~CWzBTree( void );

    // operator =
    CWzBTree<T, S>& operator = ( const CWzBTree<T, S>& rhs );

    // ����Ÿ �߰�(�߰��� ��� ��ȯ)
    CWzBNode<T, S>* Add( const T& data, const S& cmpVal );

    // ��� ����(�ش� ��常)
    T RemoveNode( CWzBNode<T, S>* node );
    // ��� ����(�ڽĵ� ����)
    void ReleaseNode( CWzBNode<T, S>* node );

    // ��� ��� ����
    void RemoveAll( void );

    // ��� ��� ã��
    CWzBNode<T, S>* FindHead( void ) const;
    // �񱳰� �̿� ��� ã��
    CWzBNode<T, S>* FindNode( const S& cmpVal ) const;

    // �ش� ����� ���� ��� ���
    CWzBNode<T, S>* GetLeft( CWzBNode<T, S>* node ) const;
    // �ش� ����� ������ ��� ���
    CWzBNode<T, S>* GetRight( CWzBNode<T, S>* node ) const;
    // �ش� ����� �θ� ��� ���
    CWzBNode<T, S>* GetParent( CWzBNode<T, S>* node ) const;

    // �ش� ��� ����Ÿ ����
    void SetData( CWzBNode<T, S>* node, const T& data );
    // �ش� ��� ����Ÿ ���
    const T& GetData( CWzBNode<T, S>* node ) const;

    // �ش� ��� �񱳰� ���
    const S& GetValue( CWzBNode<T, S>* node ) const;

    // ��ü ��� �� ���
    long GetCount( void ) const;
    // �� Ʈ���ΰ�?
    BOOL IsEmpty( void ) const;

    // ���ĵ� ����Ÿ ����Ʈ ���
    void GetSortedList( T* dstData );
    // Ʈ�� ����ȭ
    void Optimize( void );

private:
    // Ʈ�� ��ȸ�� Ư�� ó�� �Լ� ����
    typedef void (CWzBTree<T, S>::*fnProcess)( const T&, const S&, ProcParam& );

    // �ش� ������ ����(�ڽĵ� ����)
    void ReleaseNodeFrom( CWzBNode<T, S>* node );
    // �ش� ��� ����
    void CopyFrom( CWzBNode<T, S>* node );

    // Ʈ�� ��ȸ
    void Cycle( fnProcess, ProcParam& param );
    void CycleFrom( CWzBNode<T, S>* node, ProcParam& param );

    // ���ĵ� ����Ÿ ���
    void fnGetSortData( const T& data, const S& cmpVal, ProcParam& param );
    void fnGetSortList( const T& data, const S& cmpVal, ProcParam& param );

    // ���ĵ� ����Ÿ �߰�
    void AddB( const T* data, const S* cmpVal, int left, int right );

private:
    // xxx: �����ϰ� �̷� ������ ����� ���� ������
    // ������ �̹� ������ ���� �����ؾ� �ϴ� ��찡 �ֱ� ������
    // ���� �� ����� ���� �� ���� �̷��� ����.
    static T m_dummyData;
    static S m_dummyVal;

private:
    CWzBNode<T, S>* m_head;
    long m_numNodes;

    fnProcess m_fnProcess;
};

template <class T, class S>
T CWzBTree<T, S>::m_dummyData;

template <class T, class S>
S CWzBTree<T, S>::m_dummyVal;

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBTree<T, S>::CWzBTree( void )
: m_head( NULL )
, m_numNodes( 0 )
, m_fnProcess( NULL )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBTree<T, S>::CWzBTree( const CWzBTree<T, S>& rhs )
: m_head( NULL )
, m_numNodes( 0 )
, m_fnProcess( NULL )
{
    CopyFrom( rhs.m_head );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBTree<T, S>::~CWzBTree( void )
{
    RemoveAll();
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::RemoveAll( void )
{
    if( m_head )
    {
        ReleaseNode( m_head );
        m_head = NULL;
    }

    m_numNodes = 0;
}

//------------------------------------------------------------------------------
/**
    ������ ����� �ڽĵ� ���� ����

    - ���Ž� �θ� ����� left �Ǵ� right��ũ�� �����ؾ� �ϱ� ������
      �ݵ�� �� �Լ��� ���ؼ� ���� �ؾ� ��
*/
template <class T, class S>
void CWzBTree<T, S>::ReleaseNode( CWzBNode<T, S>* node )
{
    WzAssert( node );

    if( node )
    {
        CWzBNode<T, S>* parentNode = node->GetParent();
        if( parentNode )
        {
            if( node == parentNode->GetLeft() )
            {
                parentNode->SetLeft( NULL );
            }
            else if( node == parentNode->GetRight() )
            {
                parentNode->SetRight( NULL );
            }
            else
            {
                WZLOG( WZWAR, "CWzBTree::ReleaseNode() - �θ� ������ ��ũ ����!!" );
            }
        }
        else
        {
            m_head = NULL;
        }

        ReleaseNodeFrom( node );
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::ReleaseNodeFrom( CWzBNode<T, S>* node )
{
    WzAssert( node );

    if( node )
    {
        if( node->GetLeft() )
        {
            ReleaseNodeFrom( node->GetLeft() );
        }

        if( node->GetRight() )
        {
            ReleaseNodeFrom( node->GetRight() );
        }

        delete node;
        --m_numNodes;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::CopyFrom( CWzBNode<T, S>* node )
{
    WzAssert( node );

    if( node )
    {
        Add( node->GetData(), node->GetValue() );

        if( node->GetLeft() )
        {
            CopyFrom( node->GetLeft() );
        }

        if( node->GetRight() )
        {
            CopyFrom( node->GetRight() );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBTree<T, S>& CWzBTree<T, S>::operator = ( const CWzBTree<T, S>& rhs )
{
    if( &rhs == this )
    {
        return *this;
    }

    RemoveAll();

    if( rhs.m_head )
    {
        CopyFrom( rhs.m_head );
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::Add( const T& data, const S& cmpVal )
{
    // ��尡 ���� ���, ��忡 ����
    if( !m_head )
    {
        m_head = new CWzBNode<T, S>( data, cmpVal );
        WzAssert( m_head );

        WzAssert( m_numNodes == 0 );
        m_numNodes = 1;

        return m_head;
    }

    CWzBNode<T, S>* curNode = m_head;
    CWzBNode<T, S>* parentNode = m_head;

    // value ��
    // ���� ��� ������ ������ ����, ũ�� ������ ��� ����
    // xxx: ���� ���� ������ ��� ���� (���� ȣȯ�� ����)

    // �������� ȣȯ���� �����ϱ� ���� Ű ���� ���� ��� 
    // ������ ��带 �����ϰ� �ϱ� ������ �� ��� Ű�� �ߺ��Ǳ�
    // ������ Ʈ���� ���� ������.

    while( curNode )
    {
        parentNode = curNode;

        if( cmpVal < curNode->GetValue() )
        {
            curNode = curNode->GetLeft();
        }
        else
        {
            curNode = curNode->GetRight();
        }
    }

    WzAssert( parentNode );

    CWzBNode<T, S>* newNode = new CWzBNode<T, S>( data, cmpVal );
    WzAssert( newNode );

    // �θ� ���̶� ��, ������ ���� �ڽ�����
    if( cmpVal < parentNode->GetValue() )
    {
        parentNode->SetLeft( newNode );
    }
    else
    {
        parentNode->SetRight( newNode );
    }

    ++m_numNodes;

    return newNode;
}

//------------------------------------------------------------------------------
/**
    ������ ����� �ڽĵ��� �� �����ϴ� ���� �ƴϴ�.
    �ܼ��� �ش� ��常 ���� �Ǵ� ���� �����ؼ�, Ʈ���� ������Ų��.
*/
template <class T, class S>
T CWzBTree<T, S>::RemoveNode( CWzBNode<T, S>* remNode )
{
    WzAssert( remNode );

    if( !remNode )
    {
        WZLOG( WZWAR, "CWzBTree::RemoveNode() - ����!! NULL ������ (�����Ⱚ ��ȯ)" );

        return m_dummyData;
    }

    T retData = remNode->GetData();

    // Ʈ�� ���� ������
    // ���� �ڽ��� ���� �����ʿ� �ִ� ������ ��ü

    // ������ ����� ���� �ڽ��� ���
    CWzBNode<T, S>* leftNode = remNode->GetLeft();
    if( leftNode )
    {
        // ������ ����� ������ �ڽ��� ���� ���(���� �ڽĸ� �ִ� ���)
        if( !remNode->GetRight() )
        {
            // ������ ��尡 ��� ����� ���
            if( remNode == m_head )
            {
                // �� ���� ����� �������θ� �ڽĵ��� �ִ� ����.
                // ����� ���� �ڽĳ�带 ���� �����ϸ� ��
                m_head = leftNode;
                m_head->m_parent = NULL;
            }
            else
            {
                CWzBNode<T, S>* parentNode = remNode->GetParent();
                WzAssert( parentNode );

                // ������ ��尡 �θ� ����� ���� �ڽ��� ���
                if( remNode == parentNode->GetLeft() )
                {
                    // ������ ����� ���� �ڽ� ��带 �θ� ����� ���� �ڽ����� ����
                    parentNode->SetLeft( leftNode );
                }
                else if( remNode == parentNode->GetRight() )
                {
                    // ������ ����� ���� �ڽ� ��带 �θ� ����� ������ �ڽ����� ����
                    parentNode->SetRight( leftNode ) ;
                }
                else
                {
                    WZLOG( WZWAR, "CWzBTree::RemoveNode() - �θ� ������ ��ũ ����!!" );
                }
            }

            delete remNode;
            --m_numNodes;

            return retData;
        }

        // ������ ����� ����, ������ �ڽ� �� �ִ� ���

        // ������ ��� ���� �ڽ� ����� ���� �����ʿ� �ִ� ���� ���ϰ�
        // ���� ������ ������ ����� �� ����
        // ���� ���ŵǴ� ���� ������ ��� ���� �ڽ� ����� ���� �����ʿ� �ִ� ��

        // �ϴ� ���� ��带 ������ ��� ���� �ڽ� ���� �����ϰ�
        // �� ����� ���� �����ʿ� �ִ� �ڽ� ��带 ���Ѵ�.
        CWzBNode<T, S>* curNode = leftNode;
        CWzBNode<T, S>* rightLeafNode;

        while( curNode )
        {
            rightLeafNode = curNode;
            curNode = curNode->GetRight();
        }

        // �� ���� ���� ����
        T data = rightLeafNode->m_data;
        S cmpVal = rightLeafNode->m_cmpVal;

        // �� ��带 �����Ѵ�.

        // ������ �� �������, ���� �ڽ� ��尡 ���� �� �ֱ� ������
        // RemoveNode�� ���ؼ� �����ؾ� �Ѵ�.

        RemoveNode( rightLeafNode );

        // ������ ����� ���� ���� ���� ������ ��ü�Ѵ�.
        remNode->m_data = data;
        remNode->m_cmpVal = cmpVal;

        return retData;
    }

    // ���� �ڽ��� ���� ���
    else
    {
        CWzBNode<T, S>* rightNode = remNode->GetRight();

        // ������ �ڽ��� �ִ� ���
        if( rightNode )
        {
            // ������ ��尡 ��� ����� ���
            if( remNode == m_head )
            {
                m_head = rightNode;
                m_head->m_parent = NULL;
            }
            else
            {
                CWzBNode<T, S>* parentNode = remNode->GetParent();
                WzAssert( parentNode );

                // ������ ��尡 �θ� ����� ���� �ڽ��� ���
                if( remNode == parentNode->GetLeft() )
                {
                    parentNode->SetLeft( rightNode );
                }
                else if( remNode == parentNode->GetRight() )
                {
                    parentNode->SetRight( rightNode );
                }
                else
                {
                    WZLOG( WZWAR, "CWzBTree::RemoveNode() - �θ� ������ ��ũ ����!!" );
                }
            }

            delete remNode;
            --m_numNodes;

            return retData;
        }
    }

    // ���� �ڽ� �� ���� ���

    CWzBNode<T, S>* parentNode = remNode->GetParent();
    if( parentNode )
    {
        if( remNode == parentNode->GetLeft() )
        {
            parentNode->SetLeft( NULL );
        }
        else if( remNode == parentNode->GetRight() )
        {
            parentNode->SetRight( NULL );
        }
        else
        {
            WZLOG( WZWAR, "CWzBTree::RemoveNode() - �θ� ������ ��ũ ����!!" );
        }
    }
    else
    {
        m_head = NULL;
    }

    delete remNode;
    --m_numNodes;

    return retData;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::FindHead( void ) const
{
    return m_head;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::FindNode( const S& cmpVal ) const
{
    CWzBNode<T, S>* node = m_head;

    while( node )
    {
        if( node->GetValue() == cmpVal )
        {
            return node;
        }

        if( cmpVal < node->GetValue() )
        {
            node = node->GetLeft();
        }
        else
        {
            node = node->GetRight();
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::GetLeft( CWzBNode<T, S>* node ) const
{
    WzAssert( node );

    return ( node ? node->GetLeft() : NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::GetRight( CWzBNode<T, S>* node ) const
{
    WzAssert( node );

    return ( node ? node->GetRight() : NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzBNode<T, S>* CWzBTree<T, S>::GetParent( CWzBNode<T, S>* node ) const
{
    WzAssert( node );

    return ( node ? node->GetParent() : NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::SetData( CWzBNode<T, S>* node, const T& data )
{
    WzAssert( node );

    if( node )
    {
        node->SetData( data );
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const T& CWzBTree<T, S>::GetData( CWzBNode<T, S>* node ) const
{
    WzAssert( node );

    if( node )
    {
        return node->GetData();
    }

    WZLOG( WZWAR, "CWzBTree::GetData() - ����!! NULL ������ (�����Ⱚ ��ȯ)" );

    return m_dummyData;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const S& CWzBTree<T, S>::GetValue( CWzBNode<T, S>* node ) const
{
    WzAssert( node );

    if( node )
    {
        return node->GetValue();
    }

    WZLOG( WZWAR, "CWzBTree::GetValue() - ����!! NULL ������ (�����Ⱚ ��ȯ)" );

    return m_dummyVal;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
long CWzBTree<T, S>::GetCount( void ) const
{
    return m_numNodes;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
BOOL CWzBTree<T, S>::IsEmpty( void ) const
{
    return ( m_numNodes == 0 );
}

//------------------------------------------------------------------------------
/**
    ���� ��ȸ (���� -> �ڽ� -> ������)
*/
template <class T, class S>
void CWzBTree<T, S>::CycleFrom( CWzBNode<T, S>* node, ProcParam& param )
{
    WzAssert( node );
    WzAssert( m_fnProcess );

    if( node->GetLeft() )
    {
        CycleFrom( node->GetLeft(), param );
    }

    (this->*m_fnProcess)( node->GetData(), node->GetValue(), param );

    if( node->GetRight() )
    {
        CycleFrom( node->GetRight(), param );
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::Cycle( fnProcess fn, ProcParam& param )
{
    WzAssert( fn );

    m_fnProcess = fn;

    if( m_head )
    {
        CycleFrom( m_head, param );
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::fnGetSortData( const T& data, const S& cmpVal, ProcParam& param )
{
    WzAssert( param.data );

    param.data[param.cnt++] = data;
}

//------------------------------------------------------------------------------
/**
    ���ĵ� ����Ÿ�� ��� �Լ�����, �������� GetSortedList���
    �Լ����� ��� ������, ���� �ٲ����� �ʾҴ�.
*/
template <class T, class S>
void CWzBTree<T, S>::GetSortedList( T* dstData )
{
    ProcParam param;
    param.data = dstData;
    param.cnt = 0;

	Cycle( &CWzBTree<T, S>::fnGetSortData, param );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::fnGetSortList( const T& data, const S& cmpVal, ProcParam& param )
{
    WzAssert( param.data );
    WzAssert( param.cmpVal );

    param.data[param.cnt] = data;
    param.cmpVal[param.cnt] = cmpVal;
    ++param.cnt;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::AddB( const T* data, const S* cmpVal, int left, int right )
{
    if( left != right)
    {
        WzAssert( data );
        WzAssert( cmpVal );

        Add( data[(left + right) / 2], cmpVal[(left + right) / 2] );

        AddB( data, cmpVal, left, (left + right) / 2 );
        AddB( data, cmpVal, (left + right) / 2 + 1, right );
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzBTree<T, S>::Optimize( void )
{
    if( m_numNodes <= 0 )
    {
        WZLOG( WZDBG, "CWzBTree::Optimize() - ����Ÿ ����" );

        return;
    }

    int numNodes = m_numNodes;

    ProcParam param;
    memset( &param, 0, sizeof( param ) );

    param.data = new T[numNodes];
    param.cmpVal = new S[numNodes];

    if( !param.data || !param.cmpVal )
    {
        WZLOG( WZWAR, "CWzBTree::Optimize() - �޸� Ȯ�� ����" );

        goto _cleanup;
    }

    Cycle( fnGetSortList, param );

    if( param.cnt != numNodes )
    {
        WZLOG( WZWAR, "CWzBTree::Optimize() - ����!! ����Ÿ �� ����ġ" );

        goto _cleanup;
    }

    RemoveAll();

    AddB( param.data, param.cmpVal, 0, param.cnt );

_cleanup:
    if( param.data )
    {
        delete [] param.data;
    }

    if( param.cmpVal )
    {
        delete [] param.cmpVal;
    }
}

#endif // _PROGRAMCOMMON_WZBTREE_H_