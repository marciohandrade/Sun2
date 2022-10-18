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

    - T : 데이타 타입, S : 키 타입

    - TableSize만큼 행이 있고, 각 행에 Item들이 linked list로 
      연결되어 있다고 생각하면 됨. 행(index)은 Hash( key )함수를 통해서
      얻을 수 있고, 각 행에 있는 아이템들 수는 행마다 다름

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
    // 생성자/소멸자
    CWzHashTable( void );
    virtual ~CWzHashTable( void );

    // 테이블 생성
    void Create( DWORD tableSize );
    // 테이블 제거
    void Destroy( void );

    // 해당 키로 데이타 추가
    BOOL Insert( const T& val, const S& key );
    // 해당 키 제거(데이타 반환)
    void Remove( const S& key );
    // 모든 데이타 제거
    void RemoveAll( void );

    // 해당 키에 대한 데이타 얻기
    const T& Get( const S& key ) const;
    // 해당 키에 대한 데이타 포인터 얻기
    const T* GetPtr( const S& key ) const;
    // 해당 키카 존재하는가?
    BOOL HasKey( const S& key ) const;
    // 테이블 크기 얻기
    DWORD GetTableSize( void ) const;

    // 처음 위치 얻기
    WzHashTableIterator GetFirstPos( void ) const;
    // 해당 위치의 키와 데이타 얻기(다음 위치로 이동)
    void GetNext( WzHashTableIterator& pos, T& val, S& key ) const;

    // TODO: 제거 예정(사용 금지)
    void Clear( void );

#ifdef _DEBUG
    T GetDataByTableIndex( DWORD index ) const;

    long GetTableMemorySize() const;
#endif


protected:
    // 해시 함수
    virtual DWORD Hash( const S& key ) const;
    // 같은 키인가?
    BOOL IsSameKey( const S& key1, const S& key2 ) const;

private:
    // 복사 생성 금지
    CWzHashTable( const CWzHashTable& );
    // 대입 연산 금지
    CWzHashTable& operator = ( const CWzHashTable& );

protected:
    // xxx: 구차하게 이런 변수를 만들고 싶진 않지만
    // 기존에 이미 쓰레기 값을 리턴해야 하는 경우가 있기 때문에
    // 차라리 이 방법이 좋은 것 같아 이렇게 간다.
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
    TODO: 제거할 것
    - 기존 소스에서 호출하는 부분이 있기 때문에 일단 그냥 놔둠
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
    앞 부분에 추가 (A->B->C new D) => (D->A->B->C)
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
            // xxx : 같은 키가 존재하는 경우, 기존 방식은 엎어 쓰는 방식이나
            // 여타 다른 알고리즘에서는 단순 리턴하는 방식이고 또 그것이 이치상으로
            // 보나 에러 방지 차원에서 보나 맞는 것 같아 기존 방식을 바꾼다.

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