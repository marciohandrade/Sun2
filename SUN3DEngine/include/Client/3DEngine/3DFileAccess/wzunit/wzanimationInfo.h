// wzanimationinfo.h

#ifndef _3DFILEACCESS_WZANIMATIONINFO_H_
#define _3DFILEACCESS_WZANIMATIONINFO_H_

#include "../typedef.h"
#include "wzunitsubtype.h"
#include <client/3dengine/common/wzbvtype.h>
#include <vector>

// ������ �ִϸ��̼� ������ �ӵ��� ���� (���� �ӵ� ����)
#define WAIA_ANISPEED_UNCONDITIONAL     (0x01)

// ���� ����
struct WzSoundInfo
{
    float m_fSoundFrame;    // ������
    BOOL m_bVoice;          // �����ΰ�?
    std::vector<int> m_VecSoundIndex;
};

// ����Ʈ ����
struct WzEffectInfo
{
    float m_fEffectFrame;   // ������
    WzID m_wiEffectWzID;    // ID
    int m_nEffectType;      // Ÿ��
    int m_nBoneIndex;       // �� ��ȣ
    WzVector m_wvScale;     // ũ��
    WzQuaternion m_wqRot;   // ȸ��(���ʹϾ�)
    WzVector m_vRot;        // ȸ��(���Ϸ�)
    WzVector m_wvPos;       // ��ġ
    BYTE m_byEffectFlag;    // Flag
};

// �Ϲ� ����
struct WzCustomInfo
{
    float m_fCustomFrame;   // ������
    int m_naParam[3];       // �Ķ����
    WzVector m_wvParam;     // �Ķ����
};

//------------------------------------------------------------------------------
/**
    @class WzAnimationInfo

    xxx: ��� ������ public�ΰ� ���� ���� ������, �̹� ���� ������
    ���� �����ϰ� �ֱ� ������ ������ �� ��ġ�⵵ �����Ƽ� �׳� ����.
    ��������� class�� struct�� �߰� ���°� �Ǿ� ���ȴ�;
*/
class WzAnimationInfo
{
public:
    // ������/�Ҹ���
    WzAnimationInfo();
    ~WzAnimationInfo();

    // ���� ���� �߰�(����)
    WZ3DA_API void AddSound( int iSound, float fFrame, int nSetIndex );

    // ���� �ε��� ����
    WZ3DA_API void DelSoundIndex( int iIndex, int nSetIndex );
    // ���� �ε��� ���� ����
    WZ3DA_API void ClearSoundIndex( int nSetIndex );
    // ���� �ε��� �� ���
    WZ3DA_API int GetNumSound( int nSetIndex ) const;
    // ���� �ε��� ���
    WZ3DA_API int GetSound( int iIndex, int nSetIndex ) const;

    // ���� ������ ����
    WZ3DA_API void SetSoundFrame( float fFrame, int nSetIndex );
    // ���� ������ ���
    WZ3DA_API float GetSoundFrame( int nSetIndex ) const;

    // ���� ���� ����
    WZ3DA_API void SetVoiceChannel( BOOL bVoice, int nSetIndex );
    // ���� ���� ���
    WZ3DA_API BOOL IsVoiceChannel( int nSetIndex ) const;

    // ���� ���� �߰�
    WZ3DA_API void AddSoundInfo(const WzSoundInfo* sound_ptr);
    WZ3DA_API int GetNumSoundInfo() const;
    WZ3DA_API WzSoundInfo* GetSoundInfo(int nIndex);


    // ���� ���� �� ���
    WZ3DA_API int GetNumSoundSet() const;
    // ���� ���� ����
    WZ3DA_API void DelSoundSetIndex( int nSetIndex );
    // ���� ���� ���� ����
    WZ3DA_API void ClearAllSound();

    // ����Ʈ ���� �߰�
    WZ3DA_API void AddEffectInfo( const WzEffectInfo* pEffect );
    // ����Ʈ ���� ����
    WZ3DA_API void SetEffectInfo( const WzEffectInfo* pEffect, int nIndex );
    // ����Ʈ ���� ����
    WZ3DA_API void DelEffectIndex( int nIndex );
    // ����Ʈ ���� �� ���
    WZ3DA_API int GetNumEffectInfo() const;
    // ����Ʈ ���� ���
    WZ3DA_API WzEffectInfo* GetEffectInfo( int nIndex );

    // �Ϲ� ���� �߰�
    WZ3DA_API void AddCustomInfo( const WzCustomInfo* pCustom );
    // �Ϲ� ���� ����
    WZ3DA_API void SetCustomInfo( const WzCustomInfo* pCustom, int nIndex );
    // �Ϲ� ���� ����
    WZ3DA_API void DelCustomIndex( int nIndex );
    // �Ϲ� ���� �� ���
    WZ3DA_API int GetNumCustomInfo() const;
    // �Ϲ� ���� ���
    WZ3DA_API WzCustomInfo* GetCustomInfo( int nIndex );

    // �ٿ�� �ڽ� �߰�
    WZ3DA_API void AddBV( const BvAABB* paabb );
    // �ٿ�� �ڽ� ����
    WZ3DA_API void DeleteAllBV();

    // operator =
    WZ3DA_API WzAnimationInfo& operator = ( const WzAnimationInfo& rhs );

public:
    char m_lpszFileName[MAX_LENGTH_FILENAME];
    WzID m_wiAction;                // �׼�(�ִϸ��̼�) ID
    HANDLE m_hWzdAni;               // �ִϸ��̼� ����Ÿ �ڵ�
    BYTE m_byAttribute;             // �Ӽ�

    int m_nMaxFrame;                // �ִ� ������
    float m_fAnimationSpeed;        // �ִϸ��̼� �ӵ�
    float m_fInterpolationTime;     // ���� �ð� (����: ��)

    CWzArray<BvAABB*> m_arAABB;     // �� �����Ӹ��� AABB�� ����

private:
    typedef std::vector<WzSoundInfo> VECTOR_SOUNDSET;
    typedef std::vector<WzEffectInfo> VECTOR_EFFECTSET;
    typedef std::vector<WzCustomInfo> VECTOR_CUSTOMSET;

    VECTOR_SOUNDSET m_VecSound;
    VECTOR_EFFECTSET m_VecEffect;
    VECTOR_CUSTOMSET m_VecCustom;
};

//------------------------------------------------------------------------------
/**
*/
inline int WzAnimationInfo::GetNumSoundSet() const
{
    return (int)m_VecSound.size();
}

//------------------------------------------------------------------------------
/**
*/
inline int WzAnimationInfo::GetNumEffectInfo() const
{
    return (int)m_VecEffect.size();
}

//------------------------------------------------------------------------------
/**
*/
inline int WzAnimationInfo::GetNumCustomInfo() const
{
    return (int)m_VecCustom.size();
}

#endif // _3DFILEACCESS_WZANIMATIONINFO_H_