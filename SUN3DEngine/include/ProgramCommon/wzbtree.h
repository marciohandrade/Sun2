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
    // 생성자/소멸자
    CWzBTree( void );
    CWzBTree( const CWzBTree<T, S>& rhs );
    ~CWzBTree( void );

    // operator =
    CWzBTree<T, S>& operator = ( const CWzBTree<T, S>& rhs );

    // 데이타 추가(추가된 노드 반환)
    CWzBNode<T, S>* Add( const T& data, const S& cmpVal );

    // 노드 제거(해당 노드만)
    T RemoveNode( CWzBNode<T, S>* node );
    // 노드 제거(자식들 전부)
    void ReleaseNode( CWzBNode<T, S>* node );

    // 모든 노드 제거
    void RemoveAll( void );

    // 헤드 노드 찾기
    CWzBNode<T, S>* FindHead( void ) const;
    // 비교값 이용 노드 찾기
    CWzBNode<T, S>* FindNode( const S& cmpVal ) const;

    // 해당 노드의 왼쪽 노드 얻기
    CWzBNode<T, S>* GetLeft( CWzBNode<T, S>* node ) const;
    // 해당 노드의 오른쪽 노드 얻기
    CWzBNode<T, S>* GetRight( CWzBNode<T, S>* node ) const;
    // 해당 노드의 부모 노드 얻기
    CWzBNode<T, S>* GetParent( CWzBNode<T, S>* node ) const;

    // 해당 노드 데이타 설정
    void SetData( CWzBNode<T, S>* node, const T& data );
    // 해당 노드 데이타 얻기
    const T& GetData( CWzBNode<T, S>* node ) const;

    // 해당 노드 비교값 얻기
    const S& GetValue( CWzBNode<T, S>* node ) const;

    // 전체 노드 수 얻기
    long GetCount( void ) const;
    // 빈 트리인가?
    BOOL IsEmpty( void ) const;

    // 정렬된 데이타 리스트 얻기
    void GetSortedList( T* dstData );
    // 트리 최적화
    void Optimize( void );

private:
    // 트리 순회중 특정 처리 함수 정의
    typedef void (CWzBTree<T, S>::*fnProcess)( const T&, const S&, ProcParam& );

    // 해당 노드부터 제거(자식들 전부)
    void ReleaseNodeFrom( CWzBNode<T, S>* node );
    // 해당 노드 복사
    void CopyFrom( CWzBNode<T, S>* node );

    // 트리 순회
    void Cycle( fnProcess, ProcParam& param );
    void CycleFrom( CWzBNode<T, S>* node, ProcParam& param );

    // 정렬된 데이타 얻기
    void fnGetSortData( const T& data, const S& cmpVal, ProcParam& param );
    void fnGetSortList( const T& data, const S& cmpVal, ProcParam& param );

    // 정렬된 데이타 추가
    void AddB( const T* data, const S* cmpVal, int left, int right );

private:
    // xxx: 구차하게 이런 변수를 만들고 싶진 않지만
    // 기존에 이미 쓰레기 값을 리턴해야 하는 경우가 있기 때문에
    // 차라리 이 방법이 좋은 것 같아 이렇게 간다.
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
    삭제할 노드의 자식들 전부 제거

    - 제거시 부모 노드의 left 또는 right링크를 수정해야 하기 때문에
      반드시 본 함수를 통해서 제거 해야 함
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
                WZLOG( WZWAR, "CWzBTree::ReleaseNode() - 부모 노드와의 링크 오류!!" );
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
    // 헤드가 없는 경우, 헤드에 세팅
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

    // value 비교
    // 현재 노드 값보다 작으면 왼쪽, 크면 오른쪽 노드 선택
    // xxx: 같은 경우는 오른쪽 노드 선택 (기존 호환성 유지)

    // 기존과의 호환성을 유지하기 위해 키 값이 같은 경우 
    // 오른쪽 노드를 선택하게 하긴 했지만 이 경우 키가 중복되기
    // 때문에 트리가 깨져 버린다.

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

    // 부모 값이랑 비교, 작으면 왼쪽 자식으로
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
    삭제할 노드의 자식들을 다 제거하는 것이 아니다.
    단순히 해당 노드만 삭제 또는 값을 변경해서, 트리를 유지시킨다.
*/
template <class T, class S>
T CWzBTree<T, S>::RemoveNode( CWzBNode<T, S>* remNode )
{
    WzAssert( remNode );

    if( !remNode )
    {
        WZLOG( WZWAR, "CWzBTree::RemoveNode() - 실패!! NULL 포인터 (쓰레기값 반환)" );

        return m_dummyData;
    }

    T retData = remNode->GetData();

    // 트리 구조 재정비
    // 왼쪽 자식의 가장 오른쪽에 있는 값으로 대체

    // 삭제할 노드의 왼쪽 자식을 얻고
    CWzBNode<T, S>* leftNode = remNode->GetLeft();
    if( leftNode )
    {
        // 삭제할 노드의 오른쪽 자식이 없는 경우(왼쪽 자식만 있는 경우)
        if( !remNode->GetRight() )
        {
            // 삭제할 노드가 헤드 노드인 경우
            if( remNode == m_head )
            {
                // 이 경우는 헤드의 왼쪽으로만 자식들이 있는 경우다.
                // 헤드의 왼쪽 자식노드를 헤드로 설정하면 끝
                m_head = leftNode;
                m_head->m_parent = NULL;
            }
            else
            {
                CWzBNode<T, S>* parentNode = remNode->GetParent();
                WzAssert( parentNode );

                // 삭제할 노드가 부모 노드의 왼쪽 자식인 경우
                if( remNode == parentNode->GetLeft() )
                {
                    // 삭제할 노드의 왼쪽 자식 노드를 부모 노드의 왼쪽 자식으로 설정
                    parentNode->SetLeft( leftNode );
                }
                else if( remNode == parentNode->GetRight() )
                {
                    // 삭제할 노드의 왼쪽 자식 노드를 부모 노드의 오른쪽 자식으로 설정
                    parentNode->SetRight( leftNode ) ;
                }
                else
                {
                    WZLOG( WZWAR, "CWzBTree::RemoveNode() - 부모 노드와의 링크 오류!!" );
                }
            }

            delete remNode;
            --m_numNodes;

            return retData;
        }

        // 삭제할 노드의 왼쪽, 오른쪽 자식 다 있는 경우

        // 삭제할 노드 왼쪽 자식 노드의 가장 오른쪽에 있는 값을 구하고
        // 구한 값으로 삭제할 노드의 값 설정
        // 정작 제거되는 넘은 삭제할 노드 왼쪽 자식 노드의 가장 오른쪽에 있는 넘

        // 일단 현재 노드를 삭제할 노드 왼쪽 자식 노드로 설정하고
        // 그 노드의 가장 오른쪽에 있는 자식 노드를 구한다.
        CWzBNode<T, S>* curNode = leftNode;
        CWzBNode<T, S>* rightLeafNode;

        while( curNode )
        {
            rightLeafNode = curNode;
            curNode = curNode->GetRight();
        }

        // 그 값을 구해 놓고
        T data = rightLeafNode->m_data;
        S cmpVal = rightLeafNode->m_cmpVal;

        // 그 노드를 제거한다.

        // 오른쪽 끝 노드지만, 왼쪽 자식 노드가 있을 수 있기 때문에
        // RemoveNode를 통해서 제거해야 한다.

        RemoveNode( rightLeafNode );

        // 삭제할 노드의 값을 구해 놓은 값으로 대체한다.
        remNode->m_data = data;
        remNode->m_cmpVal = cmpVal;

        return retData;
    }

    // 왼쪽 자식이 없는 경우
    else
    {
        CWzBNode<T, S>* rightNode = remNode->GetRight();

        // 오른쪽 자식이 있는 경우
        if( rightNode )
        {
            // 삭제할 노드가 헤드 노드인 경우
            if( remNode == m_head )
            {
                m_head = rightNode;
                m_head->m_parent = NULL;
            }
            else
            {
                CWzBNode<T, S>* parentNode = remNode->GetParent();
                WzAssert( parentNode );

                // 삭제할 노드가 부모 노드의 왼쪽 자식인 경우
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
                    WZLOG( WZWAR, "CWzBTree::RemoveNode() - 부모 노드와의 링크 오류!!" );
                }
            }

            delete remNode;
            --m_numNodes;

            return retData;
        }
    }

    // 양쪽 자식 다 없는 경우

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
            WZLOG( WZWAR, "CWzBTree::RemoveNode() - 부모 노드와의 링크 오류!!" );
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

    WZLOG( WZWAR, "CWzBTree::GetData() - 실패!! NULL 포인터 (쓰레기값 반환)" );

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

    WZLOG( WZWAR, "CWzBTree::GetValue() - 실패!! NULL 포인터 (쓰레기값 반환)" );

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
    중위 순회 (왼쪽 -> 자신 -> 오른쪽)
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
    정렬된 데이타를 얻는 함수지만, 이전부터 GetSortedList라는
    함수명을 썼기 때문에, 굳이 바꾸지는 않았다.
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
        WZLOG( WZDBG, "CWzBTree::Optimize() - 데이타 없음" );

        return;
    }

    int numNodes = m_numNodes;

    ProcParam param;
    memset( &param, 0, sizeof( param ) );

    param.data = new T[numNodes];
    param.cmpVal = new S[numNodes];

    if( !param.data || !param.cmpVal )
    {
        WZLOG( WZWAR, "CWzBTree::Optimize() - 메모리 확보 실패" );

        goto _cleanup;
    }

    Cycle( fnGetSortList, param );

    if( param.cnt != numNodes )
    {
        WZLOG( WZWAR, "CWzBTree::Optimize() - 오류!! 데이타 수 불일치" );

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