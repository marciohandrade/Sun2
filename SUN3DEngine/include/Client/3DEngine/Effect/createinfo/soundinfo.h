// soundinfo.h

#ifndef _EFFECT_SOUNDINFO_H_
#define _EFFECT_SOUNDINFO_H_

#include "../typedef.h"

// ���� ����
struct St_SoundState
{
    bool m_blState;         // ���� ����
    DWORD m_dwTick;         // ���� �ð�
};

//------------------------------------------------------------------------------
/**
    @struct St_SoundInfo
*/
struct St_SoundInfo
{
    St_SoundState m_sStartState;    // ���� ����
    St_SoundState m_sStopState;     // ���� ����
    
    bool m_blLoop;                  // ���� ����
    short m_nSoundID;               // ���� ID
    bool m_bOneSound;               // ���� ������� ��ȸ�� ��� ����

    ENUM_SOUND_EVENT m_eSMassage;   // ���� ���� �޼���
    WzVector m_wvPos;               // ���� ��ġ

    int m_nResultHandle;            // ���� �ڵ�
    bool m_bPlayed;                 // ���� ��� �߳�?

    // ������
    St_SoundInfo()
    {
        memset( this, 0, sizeof( St_SoundInfo ) );
    }

    // ���� ������
    St_SoundInfo( const St_SoundInfo& rhs )
    {
        if( &rhs != this )
        {
            memcpy( this, &rhs, sizeof( St_SoundInfo ) );
        }
    }

    // operator =
    St_SoundInfo& operator = ( const St_SoundInfo& rhs )
    {
        if( &rhs != this )
        {
            memcpy( this, &rhs, sizeof( St_SoundInfo ) );
        }

        return *this;
    }
};

#endif // _EFFECT_SOUNDINFO_H_