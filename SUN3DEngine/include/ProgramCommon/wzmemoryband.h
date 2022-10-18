// wzmemoryband.h

#ifndef _PROGRAMCOMMON_WZMEMORYBAND_H_
#define _PROGRAMCOMMON_WZMEMORYBAND_H_

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzMemoryBand
*/
template <class T>
class CWzMemoryBand
{
public:
    // 생성자
    CWzMemoryBand( void )
    : m_prev( NULL )
    , m_next( NULL )
    , m_objPool( NULL )
    , m_freeList( NULL )
    , m_size( 0 )
    , m_freeIdx( 0 )
    {
        // empty
    }

    // 소멸자
    ~CWzMemoryBand( void )
    {
        Destroy();
    }

    // 생성
    bool Create( int size )
    {
        WzAssert( size > 0 );
        WzAssert( !m_objPool );
        WzAssert( !m_freeList );

        // 기존 데이타 검사
        if( m_objPool )
        {
            WZLOG( WZWAR, "CWzMemoryBand::Create() - 이미 있음" );
            return false;
        }

        // 객체 풀 생성
        m_objPool = new T[size];
        WzAssert( m_objPool );

        if( m_objPool == NULL)
        {
            WZLOG( WZWAR, "CWzMemoryBand::Create() - objPool 생성 실패" );
            goto _error;
        }

        // free(가용 객체) 리스트 생성
        m_freeList = new T*[size];
        WzAssert( m_freeList );

        if( m_freeList == NULL )
        {
            WZLOG( WZWAR, "CWzMemoryBand::Create() - freeList 생성 실패" );
            goto _error;
        }

        m_size = size;

        // 초기화
        Initialize();

        return true;

    _error:
        Destroy();

        return false;
    }

    // 파괴
    void Destroy( void )
    {
        if( m_freeList )
        {
            delete [] m_freeList;
            m_freeList = NULL;
        }

        if( m_objPool )
        {
            delete [] m_objPool;
            m_objPool = NULL;
        }

        m_size = 0;
        m_freeIdx = 0;

        m_prev = NULL;
        m_next = NULL;
    }

    // 초기화
    void Initialize( void )
    {
        // free리스트를 객체 풀 데이타들의 주소로 초기화
        for( int i = 0; i < m_size; ++i )
        {
            m_freeList[i] = &m_objPool[i];
        }

        m_freeIdx = m_size;
    }

    // 객체 할당
    T* AllocObject( void )
    {
        if( m_freeIdx > 0 )
        {
            return m_freeList[--m_freeIdx];
        }

        return NULL;
    }

    // 객체 해제
    bool FreeObject( T* obj )
    {
        WzAssert( obj );

        if( obj )
        {
            if( m_freeIdx < m_size  )
            {
                m_freeList[m_freeIdx++] = obj;

                return true;
            }
        }

        return false;
    }

    void SetNext( CWzMemoryBand* next )
    {
        m_next = next;
    }

    CWzMemoryBand* GetNext( void )
    {
        return m_next;
    }

    void SetPrev( CWzMemoryBand* prev )
    {
        m_prev = prev;
    }

    CWzMemoryBand* GetPrev( void )
    {
        return m_prev;
    }

#ifdef _DEBUG
    int GetSize() { return m_size; }
    int GetFreeSize() { return m_freeIdx; }
#endif


private:
    CWzMemoryBand* m_prev;
    CWzMemoryBand* m_next;

    T* m_objPool;
    T** m_freeList;

    int m_size;
    int m_freeIdx;
};

#endif // _PROGRAMCOMMON_WZMEMORYBAND_H_
