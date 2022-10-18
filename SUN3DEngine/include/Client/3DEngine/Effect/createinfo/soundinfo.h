// soundinfo.h

#ifndef _EFFECT_SOUNDINFO_H_
#define _EFFECT_SOUNDINFO_H_

#include "../typedef.h"

// 사운드 상태
struct St_SoundState
{
    bool m_blState;         // 적용 여부
    DWORD m_dwTick;         // 적용 시간
};

//------------------------------------------------------------------------------
/**
    @struct St_SoundInfo
*/
struct St_SoundInfo
{
    St_SoundState m_sStartState;    // 시작 정보
    St_SoundState m_sStopState;     // 멈춤 정보
    
    bool m_blLoop;                  // 루프 여부
    short m_nSoundID;               // 사운드 ID
    bool m_bOneSound;               // 루핑 관계없이 일회만 출력 여부

    ENUM_SOUND_EVENT m_eSMassage;   // 사운드 상태 메세지
    WzVector m_wvPos;               // 사운드 위치

    int m_nResultHandle;            // 사운드 핸들
    bool m_bPlayed;                 // 사운드 재생 했나?

    // 생성자
    St_SoundInfo()
    {
        memset( this, 0, sizeof( St_SoundInfo ) );
    }

    // 복사 생성자
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