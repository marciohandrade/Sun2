// managerunitlod.h

#ifndef _3DRENDERER_MANAGERUNITLOD_H_
#define _3DRENDERER_MANAGERUNITLOD_H_

#include "managerbase.h"
#include <programcommon/wzflag.h>

// 유닛 LOD 옵션
#define ULO_ENABLE_UNITLOD          (0x00000001)    // 유닛 LOD 적용
#define ULO_USE_UNITSUBTYPE_LOD     (0x00000002)    // 유닛의 서브 타입 LOD 적용
#define ULO_ENABLE_LODSKINNING      (0x00000004)    // 거리에 따라 스키닝 적용
#define ULO_ENABLE_LODNORMALMAP     (0x00000008)    // 거리에 따라 노말맵 적용
#define ULO_ENABLE_LODUNITFADE      (0x00000010)    // 거리에 따라 페이딩 적용
#define ULO_AFFECTED_BY_UNITSIZE    (0x00000020)    // 거리 계산시 유닛 크기 반영

//------------------------------------------------------------------------------
/**
    @class CManagerUnitLOD

    * LOD 거리 설정
    - 유닛이 카메라와 가까워질 때와 멀어질 때 거리값 다르게 적용(진동 방지)
    - 레벨이 작을 수록(카메라와 가까울 수록) 고 퀄리티

                 0레벨        1레벨      2레벨      3레벨
                         [1]         [3]        [5]
                 --->     |           |          |
        카메라 ---------|-|---------|-|--------|-|---- 유닛
                 <---   |           |          |
                       [0]         [2]        [4]

        * 유닛과 카메라가 멀어질 때는 위 거리(1, 3, 5)보다 멀어져야 레벨이 바뀌고
          반대로 가까워질 때는 아래 거리(0, 2, 4)보다 가까워져야 레벨이 바뀜
*/
class CManagerUnitLOD : public CManagerBase
{
public:
    // 생성자/소멸자
    CManagerUnitLOD();
    virtual ~CManagerUnitLOD();

    // 파괴
    virtual void Destroy();

    // xxx: 기존 호환 (제거 예정)
    WZRND_API void UpdateUnitLOD( CWzUnitDraw* pUnitDraw );

    // 거리 설정/얻기
    WZRND_API void SetDistanceOfLOD( float* apfDist, int iNumDist );
    float* GetDistanceOfLOD();
    // 거리 수 얻기
    int GetNumberOfDistance() const;

    // 거리에 따른 스키닝 적용 여부 관련
    void AutoInvalidSkinning( BOOL bUse, float* apfDist = NULL );
    float* GetSkinningDistance();
    BOOL IsSkinningLOD() const;

    // 거리에 따른 노말맵 적용 여부 관련
    void AutoInvalidNormalMap( BOOL bUse, float* apfDist = NULL );
    float* GetNormalMapDistance();
    BOOL IsNormalMapLOD() const;

    // 거리에 따른 페이드 효과 적용 여부 관련
    void AutoUnitFade( BOOL bUse, float* apfDist = NULL );
    float* GetFadeDistance();
    BOOL IsEnableUnitFade() const;

    // 유닛 LOD 활성 여부 설정/얻기
    void EnableUnitLOD( BOOL bEnable );
    BOOL IsUnitLOD() const;

    // 유닛의 서브 타입 LOD 적용 여부 설정/얻기
    void EnableUnitSubTypeLOD( BOOL bEnable );
    BOOL IsUnitSubTypeLOD() const;

    // 유닛 크기에 따른 거리 계산 여부 설정/얻기
    void EnableAffectedByUnitSize( BOOL bEnable );
    BOOL IsAffectedByUnitSize() const;

    // 기본 유닛 크기 설정/얻기 (유닛 크기에 따른 거리 계산시 사용)
    void SetDefaultUnitSize( float fSize );
    float GetDefaultUnitSize() const;

private:
    // 초기화
    void Initialize();
    // 해제
    void Release();

protected:
    CWzFlag<DWORD> m_opt;

    float* m_apfLODDist;
    int m_iNumLODDist;

    float m_fSkinningDist[2];
    float m_fNormalMapDist[2];
    float m_fFadeDist[2];

    float m_fDefUnitSize;
};

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetDistanceOfLOD()
{
    return m_apfLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline int CManagerUnitLOD::GetNumberOfDistance() const
{
    return m_iNumLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoInvalidSkinning( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODSKINNING );

        if( apfDist )
        {
            memcpy( m_fSkinningDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODSKINNING );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetSkinningDistance()
{
    return m_fSkinningDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsSkinningLOD() const
{
    return m_opt.Check( ULO_ENABLE_LODSKINNING ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoInvalidNormalMap( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODNORMALMAP );

        if( apfDist )
        {
            memcpy( m_fNormalMapDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODNORMALMAP );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetNormalMapDistance()
{
    return m_fNormalMapDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsNormalMapLOD() const
{
    return m_opt.Check( ULO_ENABLE_LODNORMALMAP ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoUnitFade( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODUNITFADE );

        if( apfDist )
        {
            memcpy( m_fFadeDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODUNITFADE );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetFadeDistance()
{
    return m_fFadeDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsEnableUnitFade() const
{
    return m_opt.Check( ULO_ENABLE_LODUNITFADE );; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableUnitLOD( BOOL bEnable )
{
    m_opt.SetB( ULO_ENABLE_UNITLOD, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsUnitLOD() const
{
    return m_opt.Check( ULO_ENABLE_UNITLOD );
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableUnitSubTypeLOD( BOOL bEnable )
{
    m_opt.SetB( ULO_USE_UNITSUBTYPE_LOD, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsUnitSubTypeLOD() const
{
    return m_opt.Check( ULO_USE_UNITSUBTYPE_LOD ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableAffectedByUnitSize( BOOL bEnable )
{
    m_opt.SetB( ULO_AFFECTED_BY_UNITSIZE, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsAffectedByUnitSize() const
{
    return m_opt.Check( ULO_AFFECTED_BY_UNITSIZE ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::SetDefaultUnitSize( float fSize )
{
    WzAssert( fSize > 0.0f );

    m_fDefUnitSize = max( fSize, 0.00001f ); 
}

//------------------------------------------------------------------------------
/**
*/
inline float CManagerUnitLOD::GetDefaultUnitSize() const
{
    return m_fDefUnitSize; 
}

#endif // _3DRENDERER_MANAGERUNITLOD_H_