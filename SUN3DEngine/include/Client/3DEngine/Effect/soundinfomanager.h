// soundinfomanager.h

#ifndef _EFFECT_SOUNDINFOMANAGER_H_
#define _EFFECT_SOUNDINFOMANAGER_H_

#include "createinfo/soundinfo.h"

#define MAX_NUM_SNDINFO     200

//------------------------------------------------------------------------------
/**
    @class CSoundInfoManager
*/
class CSoundInfoManager
{
public:
    // 생성자
    CSoundInfoManager()
    : m_sndInfoList( NULL )
    , m_freeList( NULL )
    , m_useInfoQ( NULL )
    , m_freeIdx( -1 )
    , m_front( 0 )
    , m_rear( 0 )
    {
        // empty
    }

    // 소멸자
    ~CSoundInfoManager()
    {
        Destroy();
    }

    // 생성
    bool Create()
    {
        // 안전하게 여기서도 호출
        Destroy();

        // 사운드 정보 생성
        m_sndInfoList = new St_SoundInfo[MAX_NUM_SNDINFO];
        WzAssert( m_sndInfoList );

        // 프리 인덱스 리스트 생성
        m_freeList = new St_SoundInfo*[MAX_NUM_SNDINFO];
        WzAssert( m_freeList );

        // 사용중인 정보 큐 생성
        m_useInfoQ = new St_SoundInfo*[MAX_NUM_SNDINFO + 1];
        WzAssert( m_useInfoQ );

        // 초기화
        Initialize();

        return true;
    }

    // 파괴
    void Destroy()
    {
        SAFE_DELETE_ARRAY( m_useInfoQ );
        SAFE_DELETE_ARRAY( m_freeList );
        SAFE_DELETE_ARRAY( m_sndInfoList );
    }

    // 초기화
    void Initialize()
    {
        WzAssert( m_sndInfoList );
        WzAssert( m_freeList );
        WzAssert( m_useInfoQ );

        for( int i = 0; i < MAX_NUM_SNDINFO; ++i )
        {
            m_freeList[i] = &m_sndInfoList[i];
        }

        memset( m_useInfoQ, 0, sizeof( St_SoundInfo* ) * MAX_NUM_SNDINFO );

        m_freeIdx = 0;
        m_front = 0;
        m_rear = 0;
    }

    // 할당
    St_SoundInfo* Alloc()
    {
        WzAssert( m_freeList );

        if( m_freeIdx >= 0 && m_freeIdx < MAX_NUM_SNDINFO )
        {
            return m_freeList[m_freeIdx++];
        }

        return NULL;
    }

    // 해제
    void Free( St_SoundInfo* p )
    {
        WzAssert( m_freeList );

        if( p )
        {
            WzAssert( m_sndInfoList );
            WzAssert( p >= &m_sndInfoList[0] );
            WzAssert( p <= &m_sndInfoList[MAX_NUM_SNDINFO - 1] );

            if( m_freeIdx > 0 && m_freeIdx <= MAX_NUM_SNDINFO )
            {
                m_freeList[--m_freeIdx] = p;
            }
        }
    }

    // 추가
    bool Put( St_SoundInfo* p )
    {
        WzAssert( p );
        WzAssert( m_useInfoQ );

        if( p )
        {
            WzAssert( m_sndInfoList );
            WzAssert( p >= &m_sndInfoList[0] );
            WzAssert( p <= &m_sndInfoList[MAX_NUM_SNDINFO - 1] );

            // 다음 위치를 보고
            int idx = m_front + 1;

            // 최대 인덱스를 넘었으면 0으로
            if( idx > MAX_NUM_SNDINFO )
            {
                idx = 0;
            }

            // rear위치가 아니면 추가
            if( idx != m_rear )
            {
                // front 수정
                m_front = idx;

                // 정보 추가
                m_useInfoQ[m_front] = p;

                return true;
            }
        }

        return false;
    }

    // 꺼내기
    St_SoundInfo* Pop()
    {
        WzAssert( m_useInfoQ );

        // front와 위치가 같으면 리턴
        if( m_rear == m_front )
        {
            return NULL;
        }

        // rear 증가, 최대 인덱스를 넘었으면 0으로
        if( ++m_rear > MAX_NUM_SNDINFO )
        {
            m_rear = 0;
        }

        return m_useInfoQ[m_rear];
    }

    // 비었는가?
    bool IsEmpty() const
    {
        return ( m_front == m_rear );
    }

private:
    St_SoundInfo* m_sndInfoList;
    St_SoundInfo** m_freeList;
    St_SoundInfo** m_useInfoQ;

    int m_freeIdx;
    int m_front;
    int m_rear;
};

#endif // _EFFECT_SOUNDINFOMANAGER_H_