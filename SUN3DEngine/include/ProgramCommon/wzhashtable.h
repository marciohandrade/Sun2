// wzhashtable.h

#ifndef _PROGRAMCOMMON_WZHASHTABLE_H_
#define _PROGRAMCOMMON_WZHASHTABLE_H_

#include "wztypedef.h"

typedef void* WzHashTableIterator;

//------------------------------------------------------------------------------
/**
    @class CWzHashTableItem
*/
template <class T, class S> 
class CWzHashTableItem
{
public:
    CWzHashTableItem( void )
    : m_next( NULL )
    {
        // empty
    }

    CWzHashTableItem* m_next;
    S m_key;
    T m_val;
};

//------------------------------------------------------------------------------
/**
    @class CWzHashTable

    - T : ����Ÿ Ÿ��, S : Ű Ÿ��

    - TableSize��ŭ ���� �ְ�, �� �࿡ Item���� linked list�� 
      ����Ǿ� �ִٰ� �����ϸ� ��. ��(index)�� Hash( key )�Լ��� ���ؼ�
      ���� �� �ְ�, �� �࿡ �ִ� �����۵� ���� �ึ�� �ٸ�

      ex)
        [0] -> item -> item -> ...
        [1] -> item -> item -> item -> ...
            ...
        [TableSize - 1] -> item -> item -> ...
*/
template <class T, class S>
class CWzHashTable
{
public:
    // ������/�Ҹ���
    CWzHashTable( void );
    virtual ~CWzHashTable( void );

    // ���̺� ����
    void Create( DWORD tableSize );
    // ���̺� ����
    void Destroy( void );

    // �ش� Ű�� ����Ÿ �߰�
    BOOL Insert( const T& val, const S& key );
    // �ش� Ű ����(����Ÿ ��ȯ)
    void Remove( const S& key );
    // ��� ����Ÿ ����
    void RemoveAll( void );

    // �ش� Ű�� ���� ����Ÿ ���
    const T& Get( const S& key ) const;
    // �ش� Ű�� ���� ����Ÿ ������ ���
    const T* GetPtr( const S& key ) const;
    // �ش� Űī �����ϴ°�?
    BOOL HasKey( const S& key ) const;
    // ���̺� ũ�� ���
    DWORD GetTableSize( void ) const;

    // ó�� ��ġ ���
    WzHashTableIterator GetFirstPos( void ) const;
    // �ش� ��ġ�� Ű�� ����Ÿ ���(���� ��ġ�� �̵�)
    void GetNext( WzHashTableIterator& pos, T& val, S& key ) const;

    // TODO: ���� ����(��� ����)
    void Clear( void );

#ifdef _DEBUG
    T GetDataByTableIndex( DWORD index ) const;

    long GetTableMemorySize() const;
#endif


protected:
    // �ؽ� �Լ�
    virtual DWORD Hash( const S& key ) const;
    // ���� Ű�ΰ�?
    BOOL IsSameKey( const S& key1, const S& key2 ) const;

private:
    // ���� ���� ����
    CWzHashTable( const CWzHashTable& );
    // ���� ���� ����
    CWzHashTable& operator = ( const CWzHashTable& );

protected:
    // xxx: �����ϰ� �̷� ������ ����� ���� ������
    // ������ �̹� ������ ���� �����ؾ� �ϴ� ��찡 �ֱ� ������
    // ���� �� ����� ���� �� ���� �̷��� ����.
    static T m_dummyData;

protected:
    CWzHashTableItem<T, S>** m_table;

    DWORD m_tableSize;
};

template <class T, class S>
T CWzHashTable<T, S>::m_dummyData;

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzHashTable<T, S>::CWzHashTable( void )
: m_table( NULL )
, m_tableSize( 0 )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
CWzHashTable<T, S>::~CWzHashTable( void )
{
    Destroy();
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzHashTable<T, S>::Create( DWORD tableSize )
{
    WzAssert( tableSize > 0 );
    WzAssert( !m_table );

    Destroy();

    m_tableSize = tableSize;

    m_table = new CWzHashTableItem<T, S>*[m_tableSize];
    WzAssert( m_table );

    memset( m_table, 0, sizeof( CWzHashTableItem<T, S>* ) * m_tableSize );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzHashTable<T, S>::Destroy( void )
{
    if( m_table )
    {
        RemoveAll();

        delete [] m_table;
        m_table = NULL;
    }

    m_tableSize = 0;
}

//------------------------------------------------------------------------------
/**
    TODO: ������ ��
    - ���� �ҽ����� ȣ���ϴ� �κ��� �ֱ� ������ �ϴ� �׳� ����
*/
template <class T, class S>
void CWzHashTable<T, S>::Clear( void )
{
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzHashTable<T, S>::RemoveAll( void )
{
    if( m_table )
    {
        for( DWORD i = 0; i < m_tableSize; ++i ) 
        {
            while( m_table[i] )
            {
                CWzHashTableItem<T, S>* delItem = m_table[i];

                m_table[i] = m_table[i]->m_next;

                delete delItem;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
DWORD CWzHashTable<T, S>::GetTableSize( void ) const
{
    return m_tableSize; 
}

#ifdef _DEBUG
template <class T, class S>
T CWzHashTable<T, S>::GetDataByTableIndex( DWORD index ) const
{
    CWzHashTableItem<T, S>* item = m_table[index];
    if( item != NULL )
        item->m_val;
    return NULL;
}


//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
long CWzHashTable<T, S>::GetTableMemorySize( void ) const
{
    return sizeof(CWzHashTable<T, S>) * m_tableSize; 
}
#endif

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
BOOL CWzHashTable<T, S>::IsSameKey( const S& key1, const S& key2 ) const
{
    return ( key1 == key2 );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
DWORD CWzHashTable<T, S>::Hash( const S& key ) const
{
    WzAssert( m_tableSize > 0 );

    if( m_tableSize > 0 )
    {
        DWORD calc = 0;

        const BYTE* check = (const BYTE*)&key;
        for( int i = 0; i < sizeof( S ); ++i, ++check)
        {
            calc = (calc * 131) + (*check);
        }

        return ( calc % m_tableSize );
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
    �� �κп� �߰� (A->B->C new D) => (D->A->B->C)
*/
template <class T, class S>
BOOL CWzHashTable<T, S>::Insert( const T& val, const S& key )
{
    WzAssert( m_table );

    DWORD index = Hash( key );

    CWzHashTableItem<T, S>* item = m_table[index];
    while( item )
    {
        if( IsSameKey( key, item->m_key ) )
        {
            // xxx : ���� Ű�� �����ϴ� ���, ���� ����� ���� ���� ����̳�
            // ��Ÿ �ٸ� �˰��򿡼��� �ܼ� �����ϴ� ����̰� �� �װ��� ��ġ������
            // ���� ���� ���� �������� ���� �´� �� ���� ���� ����� �ٲ۴�.

            return FALSE;
        }

        item = item->m_next;
    }

    item = new CWzHashTableItem<T, S>;
    WzAssert( item );

    item->m_key = key;
    item->m_val = val;
    item->m_next = m_table[index];

    m_table[index] = item;

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzHashTable<T, S>::Remove( const S& key )
{
    WzAssert( m_table );

    DWORD index = Hash( key );

    CWzHashTableItem<T, S>* item = m_table[index];
    if( item )
    {
        if( IsSameKey( key, item->m_key ) )
        {
            m_table[index] = item->m_next;

            delete item;

            return;
        }
        else
        {
            CWzHashTableItem<T, S>* prev = item;
            CWzHashTableItem<T, S>* cur = item->m_next;

            while( cur )
            {
                if( IsSameKey( key, cur->m_key ) )
                {
                    prev->m_next = cur->m_next;

                    delete cur;

                    return;
                }

                prev = cur;
                cur = cur->m_next;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const T& CWzHashTable<T, S>::Get( const S& key ) const
{
    WzAssert( m_table );

    DWORD index = Hash( key );

    CWzHashTableItem<T, S>* item = m_table[index];
    while( item )
    {
        if( IsSameKey( key, item->m_key ) )
        {
            return item->m_val;
        }

        item = item->m_next;
    }

    return m_dummyData;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
const T* CWzHashTable<T, S>::GetPtr( const S& key ) const
{
    WzAssert( m_table );

    DWORD index = Hash( key );

    CWzHashTableItem<T, S>* item = m_table[index];
    while( item )
    {
        if( IsSameKey( key, item->m_key ) )
        {
            return &( item->m_val );
        }

        item = item->m_next;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
BOOL CWzHashTable<T, S>::HasKey( const S& key ) const
{
    return ( GetPtr( key ) != NULL );
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
WzHashTableIterator CWzHashTable<T, S>::GetFirstPos( void ) const
{
    if( m_table )
    {
        for( DWORD i = 0; i < m_tableSize; ++i ) 
        {
            if( m_table[i] )
            {
                return m_table[i];
            }
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template <class T, class S>
void CWzHashTable<T, S>::GetNext( WzHashTableIterator& pos, T& val, S& key ) const
{
    CWzHashTableItem<T, S>* item = (CWzHashTableItem<T, S>*)pos;
    WzAssert( item );

    pos = NULL;

    if( item )
    {
        key = item->m_key;
        val = item->m_val;

        if( item->m_next )
        {
            pos = item->m_next;
            return;
        }

        WzAssert( m_table );

        DWORD i = ( Hash( item->m_key ) + 1 );
        for( ; i < m_tableSize; ++i ) 
        {
            if( m_table[i] ) 
            {
                pos = m_table[i];
                return;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    @class CKey_HashTableStr
*/
class CKey_HashTableStr
{
public:
    CKey_HashTableStr( void )
    : m_szKey( NULL )
    {
        // empty
    }

    CKey_HashTableStr( const char* szKey )
    : m_szKey( NULL )
    {
        SetStr( szKey ); 
    }

    CKey_HashTableStr( const CKey_HashTableStr& rhs )
    : m_szKey( NULL )
    {
        *this = rhs; 
    }

    ~CKey_HashTableStr( void )
    {
        Delete(); 
    }

    void Delete( void )
    {
        if( m_szKey )
        {
            delete [] m_szKey; 
            m_szKey = NULL;
        }
    }

    void SetStr( const char* szKey )
    {
        WzAssert( szKey );

        Delete();

        int len = (int)strlen( szKey );
        WzAssert( len > 0 );

        if( len > 0 )
        {
            m_szKey = new char[len + 1];
            WzAssert( m_szKey );

            strcpy( m_szKey, szKey );
        }
    }

    CKey_HashTableStr& operator = ( const CKey_HashTableStr& rhs )
    {
        SetStr( rhs.m_szKey );

        return *this;
    }

    BOOL operator == ( const CKey_HashTableStr& rhs ) const
    {
        return ( m_szKey && rhs.m_szKey && !strcmp( m_szKey, rhs.m_szKey ) ); 
    }

public:
    char* m_szKey;
};

//------------------------------------------------------------------------------
/**
    @class CWzHashTableStr
*/
template <class T>
class CWzHashTableStr : public CWzHashTable<T, CKey_HashTableStr>  
{
public:
    const T& GetC( const char* key ) const;

protected:
    virtual DWORD HashC( const char* key ) const;
    virtual DWORD Hash( const CKey_HashTableStr& key ) const;
};

//------------------------------------------------------------------------------
/**
*/
template <class T>
const T& CWzHashTableStr<T>::GetC( const char* key ) const
{
    WzAssert( key );
    WzAssert( m_table );

    if( key )
    {
        DWORD index = HashC( key );

        CWzHashTableItem<T, CKey_HashTableStr>* item = m_table[index];
        while( item )
        {
            if( IsSameKey( key, item->m_key ) )
            {
                return item->m_val;
            }

            item = item->m_next;
        }
    }

    return m_dummyData;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
DWORD CWzHashTableStr<T>::HashC( const char* key ) const
{
    WzAssert( key );
    WzAssert( m_tableSize > 0 );

    if( m_tableSize > 0 )
    {
        DWORD calc = 0;

        const BYTE* check = (const BYTE*)key;
        for( ; *check; ++check )
        {
            calc = (calc * 131) + (*check);
        }

        return ( calc % m_tableSize );
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
DWORD CWzHashTableStr<T>::Hash( const CKey_HashTableStr& key ) const
{
    WzAssert( key.m_szKey );
    WzAssert( m_tableSize > 0 );

    if( m_tableSize > 0 )
    {
        DWORD calc = 0;

        const BYTE* check = (const BYTE*)key.m_szKey;
        for( ; *check; ++check )
        {
            calc = (calc * 131) + (*check);
        }

        return ( calc % m_tableSize );
    }

    return 0;
}

#endif // _PROGRAMCOMMON_WZHASHTABLE_H_