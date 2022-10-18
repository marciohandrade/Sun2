// wzevent.h

#ifndef _3DFILEACCESS_WZEVENT_H_
#define _3DFILEACCESS_WZEVENT_H_

#include <client/3dengine/common/wzquaternion.h>

// �ִϸ��̼� ����
enum ENUM_ANI_FRAME_STATUS
{
    EAFS_ERROR = -1,    // ����
    EAFS_NORMAL = 0,
    EAFS_LOOPED,        // �� ���� ���Ҵ�.
    EAFS_FINISHED,      // ������ �����ӿ� �����ؼ� ������.
};

#define MAX_EVENT_ELEMENT       (8)
#define MAX_EVENT_ELEMENT_SND   (15)

#define EFF_DESTROY     (0x01)
#define EFF_USEROTATE   (0x02)

// ���� �̺�Ʈ ����
struct WzSoundEventInfo
{
    bool m_bBlendAni;
    int m_iSound;
};

// ����Ʈ �̺�Ʈ ����
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

// Ŀ���� �̺�Ʈ ����
struct WzCustomEventInfo
{
    bool m_bBlendAni;
    int m_iaParam3[3];
    WzVector m_wvParam3;
};

// �̺�Ʈ ����ü
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
