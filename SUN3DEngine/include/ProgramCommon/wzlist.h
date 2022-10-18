// wzlist.h

#ifndef _PROGRAMCOMMON_WZLIST_H_
#define _PROGRAMCOMMON_WZLIST_H_

#include "wztypedef.h"

template <class T> class CWzList;

//------------------------------------------------------------------------------
/**
    @class CWzNode
*/
template <class T>
class CWzNode
{
public:
    // 생성자
    CWzNode( void )
    : m_prev( NULL )
    , m_next( NULL )
    {
        // empty
    }

    // 복사 생성자
    CWzNode( const T& data )
    : m_prev( NULL )
    , m_next( NULL )
    {
        m_data = data;
    }

    // 소멸자
    ~CWzNode( void )
    {
        // empty
    }

    // 데이타 설정
    void SetData( const T& data )
    { 
        m_data = data; 
    }

    // 데이타 얻기
    T& GetData( void )
    { 
        return m_data; 
    }

private:
    // 이전 노드 설정
    void SetPrev( CWzNode<T>* prev )
    { 
        m_prev = prev; 
    }

    // 이전 노드 얻기
    CWzNode<T>* GetPrev( void ) const
    { 
        return m_prev;
    }

    // 다음 노드 설정
    void SetNext( CWzNode<T>* next )
    { 
        m_next = next; 
    }

    // 다음 노드 얻기
    CWzNode<T>* GetNext( void ) const
    { 
        return m_next; 
    }

private:
    friend class CWzList<T>;

    CWzNode<T>* m_prev;
    CWzNode<T>* m_next;

    T m_data;
};

//------------------------------------------------------------------------------
/**
    @class CWzList
*/
template <class T>
class CWzList
{
public:
    // 생성자/소멸자
    CWzList( void );
    virtual ~CWzList( void );

    // 앞에 데이타 추가
    CWzNode<T>* AddHead( const T& data );
    // 뒤에 데이타 추가
    CWzNode<T>* AddTail( const T& data );

    // 해당 노드 전에 데이타 추가
    CWzNode<T>* InsertBefore( CWzNode<T>* node, const T& data );
    // 해당 노드 후에 데이타 추가
    CWzNode<T>* InsertAfter( CWzNode<T>* node, const T& data );

    // 맨 앞 노드 제거
    T RemoveHead( void );
    // 맨 끝 노드 제거
    T RemoveTail( void );
    // 해당 노드 제거
    T RemoveNode( CWzNode<T>* &node );
    // 전체 노드 제거
    void RemoveAll( void );

    // 맨 앞 노드 찾기
    CWzNode<T>* FindHead( void ) const;
    // 맨 끝 노드 찾기
    CWzNode<T>* FindTail( void ) const;
    // 데이타를 통해 노드 찾기
    CWzNode<T>* FindNode( const T& findData ) const;

    // 해당 노드 이전 노드 얻기
    CWzNode<T>* GetPrev( CWzNode<T>* node ) const;
    // 해당 노드 이후 노드 얻기
    CWzNode<T>* GetNext( CWzNode<T>* node ) const;

    // 전체 노드 수 얻기
    int GetCount( void ) const;
    // 비었는가?
    BOOL IsEmpty( void ) const;

    // 해당 노드 데이타 설정
    void SetData( CWzNode<T>* node, const T& data );
    // 해당 노드 데이타 얻기
    T& GetData( CWzNode<T>* node ) const;

protected:
    // xxx: 구차하게 이런 변수를 만들고 싶진 않지만
    // 기존에 이미 쓰레기 값을 리턴해야 하는 경우가 있기 때문에
    // 차라리 이 방법이 좋은 것 같아 이렇게 간다.
    static T m_dummyData;

protected:
    CWzNode<T>* m_head;
    CWzNode<T>* m_tail;

    int m_numNodes;
};

template <class T>
T CWzList<T>::m_dummyData;

//------------------------------------------------------------------------------
/**
    생성자에서 뭔가를 생성한다는 것이 좋아 보이지는 않지만
    기존에 이미 그렇게 써 왔기 때문에 그냥 간다.
*/
template <class T>
CWzList<T>::CWzList( void )
: m_numNodes( 0 )
{
    m_head = new CWzNode<T>;
    WzAssert( m_head );

    m_tail = new CWzNode<T>;
    WzAssert( m_tail );

    m_head->SetNext( m_tail );
    m_tail->SetPrev( m_head );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzList<T>::~CWzList( void )
{
    RemoveAll();

    if( m_tail )
    {
        delete m_tail;
        m_tail = NULL;
    }

    if( m_head )
    {
        delete m_head;
        m_head = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::AddHead( const T& data )
{
    WzAssert( m_head );

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::AddHead() - 새 노드 생성 실패" );

        return NULL;
    }

    newNode->SetData( data );

    // 새로운 노드와 이전 첫 노드간 링크 설정
    newNode->SetNext( m_head->GetNext() );
    (m_head->GetNext())->SetPrev( newNode );

    // 새로운 노드와 head간 링크 설정
    newNode->SetPrev( m_head );
    m_head->SetNext( newNode );

    ++m_numNodes;

    return newNode;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::AddTail( const T& data )
{
    WzAssert( m_tail );

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::AddTail() - 새 노드 생성 실패" );

        return NULL;
    }

    newNode->SetData( data ) ;

    // 새로운 노드와 이전 마지막 노드간 링크 설정
    newNode->SetPrev( m_tail->GetPrev() );
    (m_tail->GetPrev())->SetNext( newNode );

    // 새로운 노드와 tail간 링크 설정
    newNode->SetNext( m_tail );
    m_tail->SetPrev( newNode );

    ++m_numNodes;

    return newNode;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::InsertBefore( CWzNode<T>* node, const T& data )
{
    WzAssert( node );

    if( !node )
    {
        WZLOG( WZWAR, "CWzList::InsertBefore() - NULL 노드" );

        return NULL;
    }

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::InsertBefore() - 새 노드 생성 실패" );

        return NULL;
    }

    newNode->SetData( data );

    // 새로운 노드와 주어진 노드 이전 노드간 링크 설정
    newNode->SetPrev( node->GetPrev() );
    (node->GetPrev())->SetNext( newNode );

    // 새로운 노드와 주어진 노드간 링크 설정
    newNode->SetNext( node );
    node->SetPrev( newNode );

    ++m_numNodes;

    return newNode;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::InsertAfter( CWzNode<T>* node, const T& data )
{
    WzAssert( node );

    if( !node )
    {
        WZLOG( WZWAR, "CWzList::InsertAfter() - NULL 노드" );

        return NULL;
    }

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::InsertAfter() - 새 노드 생성 실패" );

        return NULL;
    }

    newNode->SetData( data );

    // 새로운 노드와 주어진 노드 다음 노드간 링크 설정
    newNode->SetNext( node->GetNext() );
    (node->GetNext())->SetPrev( newNode );

    // 새로운 노드와 주어진 노드간 링크 설정
    newNode->SetPrev( node );
    node->SetNext( newNode );

    ++m_numNodes;

    return newNode;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
T CWzList<T>::RemoveHead( void )
{
    WzAssert( m_head );
    WzAssert( m_tail );

    // 첫 노드를 구하고
    CWzNode<T>* headNode = m_head->GetNext();
    WzAssert( headNode );
    
    // 리스트가 비어 있는 경우
    WzAssert( headNode != m_tail );
    if( headNode == m_tail )
    {
        WZLOG( WZWAR, "CWzList::RemoveHead() - 리스트가 비어 있음(쓰레기값 반환)" );

        return m_dummyData;
    }

    // 노드값 저장
    T data = headNode->GetData();

    // head와 첫 노드 다음 노드간 링크 설정
    (headNode->GetNext())->SetPrev( m_head );
    m_head->SetNext( headNode->GetNext() );
    
    // 노드 제거
    delete headNode;

    --m_numNodes;

    return data;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
T CWzList<T>::RemoveTail( void )
{
    WzAssert( m_head );
    WzAssert( m_tail );

    // 마지막 노드를 구하고
    CWzNode<T>* tailNode = m_tail->GetPrev();
    WzAssert( tailNode );
    
    // 리스트가 비어 있는 경우
    WzAssert( tailNode != m_head );
    if( tailNode == m_head )
    {
        WZLOG( WZWAR, "CWzList::RemoveTail() - 리스트가 비어 있음(쓰레기값 반환)" );

        return m_dummyData;
    }

    // 노드값 저장
    T data = tailNode->GetData();

    // tail과 마지막 노드 이전 노드간 링크 설정
    (tailNode->GetPrev())->SetNext( m_tail );
    m_tail->SetPrev( tailNode->GetPrev() );
    
    // 노드 제거
    delete tailNode;

    --m_numNodes;

    return data;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
T CWzList<T>::RemoveNode( CWzNode<T>* &node )
{
    WzAssert( node );

    if( !node )
    {
        WZLOG( WZWAR, "CWzList::RemoveNode() - NULL 노드(쓰레기값 반환)" );

        return m_dummyData;
    }

    T data = node->GetData();

    CWzNode<T>* nextNode = node->GetNext();
    WzAssert( nextNode );

    // node의 앞 뒤 노드 링크 설정
    (node->GetPrev())->SetNext( nextNode );
    nextNode->SetPrev( node->GetPrev() );

    // node 제거
    delete node;

    // node를 다음 노드로 설정 후 반환
    node = ( nextNode != m_tail ? nextNode : NULL );

    --m_numNodes;

    return data;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzList<T>::RemoveAll( void )
{
    WzAssert( m_head );
    WzAssert( m_tail );

    // 전체 노드 삭제
    CWzNode<T>* curNode = m_head->GetNext();
    while( curNode != m_tail )
    {
        CWzNode<T>* delNode = curNode;

        curNode = curNode->GetNext();

        delete delNode;
    }

    // head와 tail 연결
    m_head->SetNext( m_tail );
    m_tail->SetPrev( m_head );

    m_numNodes = 0;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::FindHead( void ) const
{
    WzAssert( m_head );
    WzAssert( m_tail );

    CWzNode<T>* headNode = m_head->GetNext();

    return ( headNode != m_tail ? headNode : NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::FindTail( void ) const
{
    WzAssert( m_head );
    WzAssert( m_tail );

    CWzNode<T>* tailNode = m_tail->GetPrev();

    return ( tailNode != m_head ? tailNode : NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::FindNode( const T& findData ) const
{
    WzAssert( m_head );
    WzAssert( m_tail );

    CWzNode<T>* curNode = m_head->GetNext();
    while( curNode != m_tail )
    {
        if( curNode->GetData() == findData )
        {
            return curNode;
        }

        curNode = curNode->GetNext();
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::GetPrev( CWzNode<T>* node ) const
{
    WzAssert( node );
    WzAssert( m_head );

    if( node && node->GetPrev() != m_head )
    {
        return node->GetPrev();
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
CWzNode<T>* CWzList<T>::GetNext( CWzNode<T>* node ) const
{
    WzAssert( node );
    WzAssert( m_tail );

    if( node && node->GetNext() != m_tail )
    {
        return node->GetNext();
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
int CWzList<T>::GetCount( void ) const
{ 
    return m_numNodes;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
BOOL CWzList<T>::IsEmpty( void ) const
{ 
    WzAssert( m_head );
    WzAssert( m_tail );

    return ( m_head->GetNext() == m_tail );
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
void CWzList<T>::SetData( CWzNode<T>* node, const T& data )
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
template <class T>
T& CWzList<T>::GetData( CWzNode<T>* node ) const
{
    WzAssert( node );
    WzAssert( m_head );
    WzAssert( m_tail );

    if( node && node != m_head && node != m_tail )
    {
        return node->GetData();
    }

    WZLOG( WZWAR, "CWzList::GetData() - 잘못된 노드(쓰레기값 반환)" );

    return m_dummyData;
}

#endif // _PROGRAMCOMMON_WZLIST_H_