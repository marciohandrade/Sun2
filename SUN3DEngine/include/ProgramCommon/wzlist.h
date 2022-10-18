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
    // ������
    CWzNode( void )
    : m_prev( NULL )
    , m_next( NULL )
    {
        // empty
    }

    // ���� ������
    CWzNode( const T& data )
    : m_prev( NULL )
    , m_next( NULL )
    {
        m_data = data;
    }

    // �Ҹ���
    ~CWzNode( void )
    {
        // empty
    }

    // ����Ÿ ����
    void SetData( const T& data )
    { 
        m_data = data; 
    }

    // ����Ÿ ���
    T& GetData( void )
    { 
        return m_data; 
    }

private:
    // ���� ��� ����
    void SetPrev( CWzNode<T>* prev )
    { 
        m_prev = prev; 
    }

    // ���� ��� ���
    CWzNode<T>* GetPrev( void ) const
    { 
        return m_prev;
    }

    // ���� ��� ����
    void SetNext( CWzNode<T>* next )
    { 
        m_next = next; 
    }

    // ���� ��� ���
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
    // ������/�Ҹ���
    CWzList( void );
    virtual ~CWzList( void );

    // �տ� ����Ÿ �߰�
    CWzNode<T>* AddHead( const T& data );
    // �ڿ� ����Ÿ �߰�
    CWzNode<T>* AddTail( const T& data );

    // �ش� ��� ���� ����Ÿ �߰�
    CWzNode<T>* InsertBefore( CWzNode<T>* node, const T& data );
    // �ش� ��� �Ŀ� ����Ÿ �߰�
    CWzNode<T>* InsertAfter( CWzNode<T>* node, const T& data );

    // �� �� ��� ����
    T RemoveHead( void );
    // �� �� ��� ����
    T RemoveTail( void );
    // �ش� ��� ����
    T RemoveNode( CWzNode<T>* &node );
    // ��ü ��� ����
    void RemoveAll( void );

    // �� �� ��� ã��
    CWzNode<T>* FindHead( void ) const;
    // �� �� ��� ã��
    CWzNode<T>* FindTail( void ) const;
    // ����Ÿ�� ���� ��� ã��
    CWzNode<T>* FindNode( const T& findData ) const;

    // �ش� ��� ���� ��� ���
    CWzNode<T>* GetPrev( CWzNode<T>* node ) const;
    // �ش� ��� ���� ��� ���
    CWzNode<T>* GetNext( CWzNode<T>* node ) const;

    // ��ü ��� �� ���
    int GetCount( void ) const;
    // ����°�?
    BOOL IsEmpty( void ) const;

    // �ش� ��� ����Ÿ ����
    void SetData( CWzNode<T>* node, const T& data );
    // �ش� ��� ����Ÿ ���
    T& GetData( CWzNode<T>* node ) const;

protected:
    // xxx: �����ϰ� �̷� ������ ����� ���� ������
    // ������ �̹� ������ ���� �����ؾ� �ϴ� ��찡 �ֱ� ������
    // ���� �� ����� ���� �� ���� �̷��� ����.
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
    �����ڿ��� ������ �����Ѵٴ� ���� ���� �������� ������
    ������ �̹� �׷��� �� �Ա� ������ �׳� ����.
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
        WZLOG( WZWAR, "CWzList::AddHead() - �� ��� ���� ����" );

        return NULL;
    }

    newNode->SetData( data );

    // ���ο� ���� ���� ù ��尣 ��ũ ����
    newNode->SetNext( m_head->GetNext() );
    (m_head->GetNext())->SetPrev( newNode );

    // ���ο� ���� head�� ��ũ ����
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
        WZLOG( WZWAR, "CWzList::AddTail() - �� ��� ���� ����" );

        return NULL;
    }

    newNode->SetData( data ) ;

    // ���ο� ���� ���� ������ ��尣 ��ũ ����
    newNode->SetPrev( m_tail->GetPrev() );
    (m_tail->GetPrev())->SetNext( newNode );

    // ���ο� ���� tail�� ��ũ ����
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
        WZLOG( WZWAR, "CWzList::InsertBefore() - NULL ���" );

        return NULL;
    }

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::InsertBefore() - �� ��� ���� ����" );

        return NULL;
    }

    newNode->SetData( data );

    // ���ο� ���� �־��� ��� ���� ��尣 ��ũ ����
    newNode->SetPrev( node->GetPrev() );
    (node->GetPrev())->SetNext( newNode );

    // ���ο� ���� �־��� ��尣 ��ũ ����
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
        WZLOG( WZWAR, "CWzList::InsertAfter() - NULL ���" );

        return NULL;
    }

    CWzNode<T>* newNode = new CWzNode<T>;
    WzAssert( newNode );

    if( !newNode )
    {
        WZLOG( WZWAR, "CWzList::InsertAfter() - �� ��� ���� ����" );

        return NULL;
    }

    newNode->SetData( data );

    // ���ο� ���� �־��� ��� ���� ��尣 ��ũ ����
    newNode->SetNext( node->GetNext() );
    (node->GetNext())->SetPrev( newNode );

    // ���ο� ���� �־��� ��尣 ��ũ ����
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

    // ù ��带 ���ϰ�
    CWzNode<T>* headNode = m_head->GetNext();
    WzAssert( headNode );
    
    // ����Ʈ�� ��� �ִ� ���
    WzAssert( headNode != m_tail );
    if( headNode == m_tail )
    {
        WZLOG( WZWAR, "CWzList::RemoveHead() - ����Ʈ�� ��� ����(�����Ⱚ ��ȯ)" );

        return m_dummyData;
    }

    // ��尪 ����
    T data = headNode->GetData();

    // head�� ù ��� ���� ��尣 ��ũ ����
    (headNode->GetNext())->SetPrev( m_head );
    m_head->SetNext( headNode->GetNext() );
    
    // ��� ����
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

    // ������ ��带 ���ϰ�
    CWzNode<T>* tailNode = m_tail->GetPrev();
    WzAssert( tailNode );
    
    // ����Ʈ�� ��� �ִ� ���
    WzAssert( tailNode != m_head );
    if( tailNode == m_head )
    {
        WZLOG( WZWAR, "CWzList::RemoveTail() - ����Ʈ�� ��� ����(�����Ⱚ ��ȯ)" );

        return m_dummyData;
    }

    // ��尪 ����
    T data = tailNode->GetData();

    // tail�� ������ ��� ���� ��尣 ��ũ ����
    (tailNode->GetPrev())->SetNext( m_tail );
    m_tail->SetPrev( tailNode->GetPrev() );
    
    // ��� ����
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
        WZLOG( WZWAR, "CWzList::RemoveNode() - NULL ���(�����Ⱚ ��ȯ)" );

        return m_dummyData;
    }

    T data = node->GetData();

    CWzNode<T>* nextNode = node->GetNext();
    WzAssert( nextNode );

    // node�� �� �� ��� ��ũ ����
    (node->GetPrev())->SetNext( nextNode );
    nextNode->SetPrev( node->GetPrev() );

    // node ����
    delete node;

    // node�� ���� ���� ���� �� ��ȯ
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

    // ��ü ��� ����
    CWzNode<T>* curNode = m_head->GetNext();
    while( curNode != m_tail )
    {
        CWzNode<T>* delNode = curNode;

        curNode = curNode->GetNext();

        delete delNode;
    }

    // head�� tail ����
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

    WZLOG( WZWAR, "CWzList::GetData() - �߸��� ���(�����Ⱚ ��ȯ)" );

    return m_dummyData;
}

#endif // _PROGRAMCOMMON_WZLIST_H_