// wzanimationinfo.h

#ifndef _3DFILEACCESS_WZANIMATIONINFO_H_
#define _3DFILEACCESS_WZANIMATIONINFO_H_

#include "../typedef.h"
#include "wzunitsubtype.h"
#include <client/3dengine/common/wzbvtype.h>
#include <vector>

// 무조건 애니메이션 정보내 속도만 적용 (유닛 속도 무시)
#define WAIA_ANISPEED_UNCONDITIONAL     (0x01)

// 사운드 정보
struct WzSoundInfo
{
    float m_fSoundFrame;    // 프레임
    BOOL m_bVoice;          // 음성인가?
    std::vector<int> m_VecSoundIndex;
};

// 이펙트 정보
struct WzEffectInfo
{
    float m_fEffectFrame;   // 프레임
    WzID m_wiEffectWzID;    // ID
    int m_nEffectType;      // 타입
    int m_nBoneIndex;       // 본 번호
    WzVector m_wvScale;     // 크기
    WzQuaternion m_wqRot;   // 회전(쿼터니언)
    WzVector m_vRot;        // 회전(오일러)
    WzVector m_wvPos;       // 위치
    BYTE m_byEffectFlag;    // Flag
};

// 일반 정보
struct WzCustomInfo
{
    float m_fCustomFrame;   // 프레임
    int m_naParam[3];       // 파라미터
    WzVector m_wvParam;     // 파라미터
};

//------------------------------------------------------------------------------
/**
    @class WzAnimationInfo

    xxx: 멤버 변수가 public인게 맘에 들진 않지만, 이미 많은 곳에서
    직접 접근하고 있기 때문에 일일이 다 고치기도 귀찮아서 그냥 간다.
    결과적으로 class와 struct의 중간 형태가 되어 버렸다;
*/
class WzAnimationInfo
{
public:
    // 생성자/소멸자
    WzAnimationInfo();
    ~WzAnimationInfo();

    // 사운드 정보 추가(설정)
    WZ3DA_API void AddSound( int iSound, float fFrame, int nSetIndex );

    // 사운드 인덱스 제거
    WZ3DA_API void DelSoundIndex( int iIndex, int nSetIndex );
    // 사운드 인덱스 전부 제거
    WZ3DA_API void ClearSoundIndex( int nSetIndex );
    // 사운드 인덱스 수 얻기
    WZ3DA_API int GetNumSound( int nSetIndex ) const;
    // 사운드 인덱스 얻기
    WZ3DA_API int GetSound( int iIndex, int nSetIndex ) const;

    // 사운드 프레임 설정
    WZ3DA_API void SetSoundFrame( float fFrame, int nSetIndex );
    // 사운드 프레임 얻기
    WZ3DA_API float GetSoundFrame( int nSetIndex ) const;

    // 음성 여부 설정
    WZ3DA_API void SetVoiceChannel( BOOL bVoice, int nSetIndex );
    // 음성 여부 얻기
    WZ3DA_API BOOL IsVoiceChannel( int nSetIndex ) const;

    // 사운드 정보 추가
    WZ3DA_API void AddSoundInfo(const WzSoundInfo* sound_ptr);
    WZ3DA_API int GetNumSoundInfo() const;
    WZ3DA_API WzSoundInfo* GetSoundInfo(int nIndex);


    // 사운드 정보 수 얻기
    WZ3DA_API int GetNumSoundSet() const;
    // 사운드 정보 제거
    WZ3DA_API void DelSoundSetIndex( int nSetIndex );
    // 사운드 정보 전부 제거
    WZ3DA_API void ClearAllSound();

    // 이펙트 정보 추가
    WZ3DA_API void AddEffectInfo( const WzEffectInfo* pEffect );
    // 이펙트 정보 설정
    WZ3DA_API void SetEffectInfo( const WzEffectInfo* pEffect, int nIndex );
    // 이펙트 정보 제거
    WZ3DA_API void DelEffectIndex( int nIndex );
    // 이펙트 정보 수 얻기
    WZ3DA_API int GetNumEffectInfo() const;
    // 이펙트 정보 얻기
    WZ3DA_API WzEffectInfo* GetEffectInfo( int nIndex );

    // 일반 정보 추가
    WZ3DA_API void AddCustomInfo( const WzCustomInfo* pCustom );
    // 일반 정보 설정
    WZ3DA_API void SetCustomInfo( const WzCustomInfo* pCustom, int nIndex );
    // 일반 정보 제거
    WZ3DA_API void DelCustomIndex( int nIndex );
    // 일반 정보 수 얻기
    WZ3DA_API int GetNumCustomInfo() const;
    // 일반 정보 얻기
    WZ3DA_API WzCustomInfo* GetCustomInfo( int nIndex );

    // 바운딩 박스 추가
    WZ3DA_API void AddBV( const BvAABB* paabb );
    // 바운딩 박스 제거
    WZ3DA_API void DeleteAllBV();

    // operator =
    WZ3DA_API WzAnimationInfo& operator = ( const WzAnimationInfo& rhs );

public:
    char m_lpszFileName[MAX_LENGTH_FILENAME];
    WzID m_wiAction;                // 액션(애니메이션) ID
    HANDLE m_hWzdAni;               // 애니메이션 데이타 핸들
    BYTE m_byAttribute;             // 속성

    int m_nMaxFrame;                // 최대 프레임
    float m_fAnimationSpeed;        // 애니메이션 속도
    float m_fInterpolationTime;     // 보간 시간 (단위: 초)

    CWzArray<BvAABB*> m_arAABB;     // 각 프레임마다 AABB를 가짐

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