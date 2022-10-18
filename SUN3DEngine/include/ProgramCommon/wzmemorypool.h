// wzmemorypool.h

#ifndef _PROGRAMCOMMON_WZMEMORYPOOL_H_
#define _PROGRAMCOMMON_WZMEMORYPOOL_H_

#include "wzmemoryband.h"

//------------------------------------------------------------------------------
/**
    @class CWzMemoryPool
*/
template <class T>
class CWzMemoryPool
{
public:
    // ������
    CWzMemoryPool( void )
    : m_head( NULL )
    , m_freeBand( NULL )
    , m_extSize( 0 )
    {
        // empty
    }

    // �Ҹ���
    ~CWzMemoryPool( void )
    {
        Destroy();
    }

    // �ʱ�ȭ
    bool Create( int size, int extSize = 0 )
    {
        WzAssert( size > 0 );
        WzAssert( !m_head );

        if( m_head )
        {
            WZLOG( WZWAR, "CWzMemoryPool::Create() - �̹� ����" );
            return false;
        }

        CWzMemoryBand<T>* newBand = AllocBand( size );
        WzAssert( newBand );

        if( !newBand )
        {
            WZLOG( WZWAR, "CWzMemoryPool::Create() - �� ��� ���� ����" );
            return false;
        }

        SetHeadBand(newBand);
        m_freeBand = newBand;
        m_extSize = ( extSize > 0 ? extSize : size / 2 + 1 );

        return true;
    }

    // ����
    void Destroy( void )
    {
        if( m_head )
        {
            CWzMemoryBand<T>* cur = m_head;
            while( cur )
            {
                CWzMemoryBand<T>* del = cur;

                cur = cur->GetNext();

                delete del;
            }

            m_head = NULL;
            m_freeBand = NULL;
        }
    }

    // ��� (������ ����)
    void Shrink( void )
    {
        if( m_head )
        {
            CWzMemoryBand<T>* next = m_head->GetNext();
            while( next )
            {
                delete m_head;

                m_head = next;
                next = next->GetNext();
            }

            m_head->SetPrev( NULL );
            m_head->SetNext( NULL );

            m_head->Initialize();
            
            m_freeBand = m_head;
        }
    }

    // ��ü �Ҵ�
    T* Alloc( void )
    {
        WzAssert( m_freeBand );

        T* newObj = m_freeBand->AllocObject();
        if( newObj != NULL )
        {
            // ������ freeBand���� �����ߴٸ� ����
            return newObj;
        }

        // ������ freeBand�� ������ ���ٸ� ������� �ٽ� ã�´�
        CWzMemoryBand<T>* band = m_head;
        while( band )
        {
            newObj = band->AllocObject();
            if( newObj != NULL )
            {
                // �� ��带 freeBand�� �����ϰ� ����
                m_freeBand = band;
                return newObj;
            }

            band = band->GetNext();
        }


        // ��� ��忡 ������ ���ٸ� ���ο� ��带 �����Ѵ�
        CWzMemoryBand<T>* newBand = AllocBand( m_extSize );
        WzAssert( newBand );

        newObj = newBand->AllocObject();
        WzAssert(newObj);

        SetHeadBand( newBand );
        m_freeBand = newBand;

        return newObj;
    }

    // ��ü ����
    void Free( T* obj )
    {
        WzAssert( obj );

        if( obj == NULL )
            return;

        WzAssert( m_freeBand );

        // ������ freeBand�� �߰��Ѵ�
        if( m_freeBand->FreeObject( obj ) == false )
        {
            // �� ����� �ٸ���忡 �߰��Ѵ�

            CWzMemoryBand<T>* band = m_head;

            while( band )
            {
                if( band->FreeObject( obj ) == true )
                {
                    return;
                }

                band = band->GetNext();
            }

            WzAssert( FALSE || "CWzMemoryPool::Free() - ��ȯ�� ���� �ߴ�." );
        }
    }

#ifdef _DEBUG
    int GetSize()
    {
        int total_size = 0;

        CWzMemoryBand<T>* band = m_head;

        while( band )
        {
            total_size += band->GetSize();
            band = band->GetNext();
        }

        return total_size;
    }

    int GetFreeSize()
    {
        int total_free_size = 0;

        CWzMemoryBand<T>* band = m_head;

        while( band )
        {
            total_free_size += band->GetFreeSize();
            band = band->GetNext();
        }

        return total_free_size;
    }

#endif

private:
    void SetHeadBand( CWzMemoryBand<T>* band )
    {
        band->SetNext( m_head );

        if( m_head )
        {
            m_head->SetPrev( band );
        }

        m_head = band;
    }

    CWzMemoryBand<T>* AllocBand( int size )
    {
        CWzMemoryBand<T>* newBand = new CWzMemoryBand<T>;
        WzAssert( newBand );

        if( !newBand )
        {
            WZLOG( WZWAR, "CWzMemoryPool::AllocBand() - newBand ���� ����" );
            goto _error;
        }

        if( !newBand->Create( size ) )
        {
            WZLOG( WZWAR, "CWzMemoryPool::AllocBand() - newBand �ʱ�ȭ ����" );
            goto _error;
        }

        return newBand;

    _error:
        if( newBand )
        {
            delete newBand;
        }

        return NULL;
    }

private:
    CWzMemoryBand<T>* m_head;
    CWzMemoryBand<T>* m_freeBand;

    int m_extSize;
};

//////////////////////////////////////////////////////////////////////////
//!~
//#define _USE_ENGINE_MEMORY_POOL_

namespace CMemoryPool
{
#ifdef _USE_ENGINE_MEMORY_POOL_
    template<typename T>
    class SingletonPool
    {
        SingletonPool() {
            pool_.Create(200);
        };

        ~SingletonPool() {
            pool_.Destroy();
        };

    public:
        static SingletonPool<T>* Instance()
        {
            static SingletonPool<T> instance;
            return &instance;
        }

        CWzMemoryPool<T> pool_;
    };

    template<typename T>
    static T* AllocNew()
    {
        return SingletonPool<T>::Instance()->pool_.Alloc();
    }

    template<typename T>
    static void AllocDelete(T* ptr)
    {
        SingletonPool<T>::Instance()->pool_.Free(ptr);
    }
#else //
    template<typename T>
    static T* AllocNew()
    {
        return new T;
    }

    template<typename T>
    static void AllocDelete(T* ptr)
    {
        delete ptr;
    }
#endif //_USE_ENGINE_MEMORY_POOL_
    //~!
}
#endif // _PROGRAMCOMMON_WZMEMORYPOOL_H_