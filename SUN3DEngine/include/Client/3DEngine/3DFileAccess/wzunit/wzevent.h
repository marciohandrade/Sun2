// wzevent.h

#ifndef _3DFILEACCESS_WZEVENT_H_
#define _3DFILEACCESS_WZEVENT_H_

#include <client/3dengine/common/wzquaternion.h>

// 애니메이션 상태
enum ENUM_ANI_FRAME_STATUS
{
    EAFS_ERROR = -1,    // 오류
    EAFS_NORMAL = 0,
    EAFS_LOOPED,        // 한 바퀴 돌았다.
    EAFS_FINISHED,      // 마지막 프레임에 도달해서 끝났다.
};

#define MAX_EVENT_ELEMENT       (8)
#define MAX_EVENT_ELEMENT_SND   (15)

#define EFF_DESTROY     (0x01)
#define EFF_USEROTATE   (0x02)

// 사운드 이벤트 정보
struct WzSoundEventInfo
{
    bool m_bBlendAni;
    int m_iSound;
};

// 이펙트 이벤트 정보
struct WzEffectEventInfo
{
    bool m_bBlendAni;
    int m_iEffectBoneIndex;
    int m_iEffectType;
    WzID m_wiEffectWzID;
    WzVector m_wvEffectPos;
    WzQuaternion m_wqEffectQuat; 
    WzVector m_vRot;
    WzVector m_wvEffectScale;
    BYTE m_byEffectFlag;
};

// 커스텀 이벤트 정보
struct WzCustomEventInfo
{
    bool m_bBlendAni;
    int m_iaParam3[3];
    WzVector m_wvParam3;
};

// 이벤트 구조체
struct WzEvent
{
    int m_iNumSound;
    WzSoundEventInfo m_Sound[MAX_EVENT_ELEMENT_SND];
    
    int m_iNumEffect;
    WzEffectEventInfo m_Effect[MAX_EVENT_ELEMENT];

    int m_iNumCustom;
    WzCustomEventInfo m_Custom[MAX_EVENT_ELEMENT];

    WzEvent()
    {
        Clear();
    }

    void Clear()
    {
        m_iNumSound = 0;
        m_iNumEffect = 0;
        m_iNumCustom = 0;
    }
};

#endif // _3DFILEACCESS_WZEVENT_H_
