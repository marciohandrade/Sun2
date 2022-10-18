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
    // ������
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

    // �Ҹ���
    ~CSoundInfoManager()
    {
        Destroy();
    }

    // ����
    bool Create()
    {
        // �����ϰ� ���⼭�� ȣ��
        Destroy();

        // ���� ���� ����
        m_sndInfoList = new St_SoundInfo[MAX_NUM_SNDINFO];
        WzAssert( m_sndInfoList );

        // ���� �ε��� ����Ʈ ����
        m_freeList = new St_SoundInfo*[MAX_NUM_SNDINFO];
        WzAssert( m_freeList );

        // ������� ���� ť ����
        m_useInfoQ = new St_SoundInfo*[MAX_NUM_SNDINFO + 1];
        WzAssert( m_useInfoQ );

        // �ʱ�ȭ
        Initialize();

        return true;
    }

    // �ı�
    void Destroy()
    {
        SAFE_DELETE_ARRAY( m_useInfoQ );
        SAFE_DELETE_ARRAY( m_freeList );
        SAFE_DELETE_ARRAY( m_sndInfoList );
    }

    // �ʱ�ȭ
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

    // �Ҵ�
    St_SoundInfo* Alloc()
    {
        WzAssert( m_freeList );

        if( m_freeIdx >= 0 && m_freeIdx < MAX_NUM_SNDINFO )
        {
            return m_freeList[m_freeIdx++];
        }

        return NULL;
    }

    // ����
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

    // �߰�
    bool Put( St_SoundInfo* p )
    {
        WzAssert( p );
        WzAssert( m_useInfoQ );

        if( p )
        {
            WzAssert( m_sndInfoList );
            WzAssert( p >= &m_sndInfoList[0] );
            WzAssert( p <= &m_sndInfoList[MAX_NUM_SNDINFO - 1] );

            // ���� ��ġ�� ����
            int idx = m_front + 1;

            // �ִ� �ε����� �Ѿ����� 0����
            if( idx > MAX_NUM_SNDINFO )
            {
                idx = 0;
            }

            // rear��ġ�� �ƴϸ� �߰�
            if( idx != m_rear )
            {
                // front ����
                m_front = idx;

                // ���� �߰�
                m_useInfoQ[m_front] = p;

                return true;
            }
        }

        return false;
    }

    // ������
    St_SoundInfo* Pop()
    {
        WzAssert( m_useInfoQ );

        // front�� ��ġ�� ������ ����
        if( m_rear == m_front )
        {
            return NULL;
        }

        // rear ����, �ִ� �ε����� �Ѿ����� 0����
        if( ++m_rear > MAX_NUM_SNDINFO )
        {
            m_rear = 0;
        }

        return m_useInfoQ[m_rear];
    }

    // ����°�?
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