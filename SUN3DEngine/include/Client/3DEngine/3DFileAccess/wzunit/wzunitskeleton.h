// wzunitskeleton.h

#ifndef _3DFILEACCESS_WZUNITSKELETON_H_
#define _3DFILEACCESS_WZUNITSKELETON_H_

#include <programcommon/wzflag.h>
#include "wzanistate.h"
#include "wzanistatecb.h"
#include "wzunit.h"
#include "../wzdbase/wzdtypedef.h"

// 상태 플래그
#define UDSS_ROT_DIRTY          (0x00000001)
#define UDSS_BV_UPDATE          (0x00000002)
#define UDSS_TPARAM_UPDATE      (0x00000004)
#define UDSS_LIGHT_UPDATE       (0x00000008)

class CWzUnitDraw;

//------------------------------------------------------------------------------
/**
    @class CWzUnitSkeleton
*/
class CWzUnitSkeleton : public CWzAniStateCB
{
public:
    // 생성자/소멸자
    CWzUnitSkeleton( CWzUnitDraw* pOwner );
    virtual ~CWzUnitSkeleton();

    // WzUnit이 설정된 경우
    void OnWzUnitSet( BOOL bDynamic );

    // SubType이 변경된 경우
    void OnSubTypeChange();

    // -------- 애니메이션 관련 --------

    // 현재 애니메이션 설정
    void SetCurAnimation( WzID wiAni, 
                          BOOL bLoop,
                          BOOL bRestart,
                          int nIpolFrame );

    // 현재 애니메이션 ID 얻기
    WzID GetCurAnimationID();
    // 현재 애니메이션 상태 얻기
    CWzAniState* GetCurAniState();

    // 현재 애니메이션 정보 얻기
    WzAnimationInfo* GetCurAnimationInfo();
    // 이전 애니메이션 정보 얻기
    WzAnimationInfo* GetPrevAnimationInfo();

    // 블랜딩 애니메이션 설정
    void SetCurBlendAnimation( WzID wiAni, 
                               int nBoneIdx, 
                               BOOL bLoop, 
                               float fSpeedRatio, 
                               BOOL bDeltaType );

    // 블랜딩 애니메이션 제거
    void UnsetBlendAnimation();
    // 블랜딩 애니메이션 반복 안 함
    void UnloopBlendAnimation();

    // 블랜딩 애니메이션 수 얻기
    // xxx: 제거 예정, 이전에는 여러개를 지원했었음
    int GetNumBlendAnimations();

    // 프레임 진행
    ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                         WzEvent* pEvent,
                                         float* pfProgressRate );

    // 매 프레임 처리
    void Process( const WzMatrix* pwmParent );

    // 프레임 강제 설정
    void SetCurFrame( float fFrame );
    // 현재 프레임 얻기
    float GetCurFrame();

    // 보간 프레임수 설정
    void SetIpolFrame( int nFrame );
    // 블랜딩 보간 프레임수 설정
    void SetBlendIpolFrame( int nStartFrame, int nEndFrame );

    // 보간 중인가?
    BOOL IsInterpolating();

    // 변환 버퍼 크기 얻기
    int GetTransformBufSize();

    // 변환 버퍼 얻기
    WzTransform* GetTransformBuf();
    // 최근(마지막으로 애니메이션 된) 변환 버퍼 얻기
    WzTransform* GetLastTransformBuf();

    // 변환 행렬 버퍼 얻기
    WzMatrix* GetTransformMatrixBuf( int iSize );
    // 스키닝 행렬 버퍼 얻기
    WzMatrix* GetSkinMatrixBuf( int iSize );

    // 본 위치 얻기
    const WzVector& GetBonePos( int nIndex );

    // 애니메이션 상태 콜백
    virtual void OnAniStateNotice( int iCode, DWORD dwParam );

    // -------- 위치/회전/크기 관련 --------

    // 크기 설정/얻기
    void SetScale( const WzVector& wvScale, BOOL bUpdateBV );
    const WzVector& GetScale() const;
    
    // 회전 설정/얻기
    void SetRotation( const WzVector& wvRotate, BOOL bUpdateBV );
    const WzVector& GetRotation() const;

    // 위치 설정/얻기
    void SetPosition( const WzVector& wvPos, BOOL bUpdateBV );
    const WzVector& GetPosition() const;

    // 회전 행렬 설정/얻기
    void SetRotationByMatrix( const WzMatrix* pwmRotate );
    void GetRotationMatrix( WzMatrix* pwmOutRotate );

    // 변환 flag 갱신
    void UpdateTransform();

    // -------- 바운딩 볼륨 관련 --------

    // 기본(로컬) 바운딩 볼륨 설정
    void SetBaseBV( const WzBoundingVolume& wbv );

    // 월드 바운딩 볼륨 갱신
    void UpdateBV( BOOL bUpdateNow, const WzMatrix* pwmTransform = NULL, int iRefBone= -1 );

    // 월드 바운딩 볼륨 얻기
    const WzBoundingVolume& GetBV() const;
    WzBoundingVolume* GetBVPtr();
    const WzBoundingVolume& GetBaseBV() const;
    WzBoundingVolume* GetBaseBVPtr();

    // 월드 바운딩 구 포인터 얻기
    BvSphere* GetBvSpherePtr();

    // 추가 바운딩 볼륨 초기화
    void InitBVExtra();
    // 추가 바운딩 볼륨 갱신
    void UpdateBVExtra();

    // 추가 바운딩 볼륨 수 얻기
    int GetNumBVExtra();
    // 인덱스로 추가 바운딩 볼륨 얻기
    WzBVExtraInfo* GetBVExtraPtrByIndex( int iIndex );

    // -------- 기타 --------

    // LOD 얻기
    int GetLOD() const;

    // 강제 LOD 설정
    void SetForceLOD( int iLevel );

    // 라이트 얻기
    const int* GetAreaLightPtr() const;

private:
    // 기본 생성자
    CWzUnitSkeleton();

    // 해제
    void Release();

    // 변환 버퍼들 해제
    void FreeTransformBuf();
    // 변환 버퍼들 생성
    void ResizeTransformBuf( int iSize );

    // 변환 버퍼 검사
    void VerifyTransformBuf( int iSize );

    // 기본 바운딩 볼륨 초기화
    void InitBaseBV( ENUM_BVTYPE eBvType );

    // 유효한본 얻기
    int GetSafeBoneIndex( int iBoneIndex ) const;

    // 추가 바운딩 볼륨 제거
    void ReleaseBVExtra();

    // 최근(마지막) 변환 저장
    void SaveLastTransform();

    // 변환 파라미터 초기화/갱신
    void InitTransformParam();
    BOOL UpdateTransformParam( const WzMatrix* pwmParent );

    // 전처리/후처리
    void PreProcess( const WzMatrix* pwmParent );
    void PostProcess();

    // 유닛 LOD 갱신
    void UpdateLOD( const WzMatrix* pwmParent );

    // 부모 유닛 옵션 조절
    void AdjustOwnerOption();

    // 지역 라이트 갱신
    void UpdateAreaLight();

private:
    CWzUnitDraw* m_pOwner;
    CWzUnitSubType* m_pUnitSubType;

    CWzFlag<DWORD> m_stateFlag;
    WzTransformParam m_transformParam;

    CWzAniState m_curState;
    CWzAniState m_blendState;
    WzAnimationInfo* m_pPrevAnimation;

    WzTransform* m_pTransformBuf;
    WzTransform* m_pLastTransformBuf;

    WzMatrix* m_pTransformMatBuf;
    WzMatrix* m_pSkinMatBuf;

    int m_iTransformBufSize;

    WzAniBlendInfo m_aniBlendInfo;
    float m_fUnitAniSpeed;

    WzVector m_wvScale;
    WzVector m_wvRotate;
    WzVector m_wvPos;

    WzMatrix m_wmRotate;

    WzUnitTransformOffset* m_pTransformOffset;

    WzBoundingVolume m_bvBase;
    WzBoundingVolume m_bvWorld;

    BvSphere m_bvSphere;
    CWzArray<WzBVExtraInfo*> m_arBVExtra;

    float m_fDistFromCam;

    int m_iLOD;
    int m_iForceLOD;

    int m_aiAreaLight[2];
};

//------------------------------------------------------------------------------
/**
*/
inline WzID CWzUnitSkeleton::GetCurAnimationID()
{
    const WzAnimationInfo* pInfo = m_curState.GetAnimationInfo();
    if( pInfo )
    {
        return pInfo->m_wiAction;
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline CWzAniState* CWzUnitSkeleton::GetCurAniState()
{
    return &m_curState;
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzUnitSkeleton::GetCurAnimationInfo()
{
    return m_curState.GetAnimationInfo();
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzUnitSkeleton::GetPrevAnimationInfo()
{
    return m_pPrevAnimation;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::UnsetBlendAnimation()
{
    if( m_blendState.HasAnimation() )
    {
        m_blendState.SetAnimationInfo( NULL, false, false, 0, false );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::UnloopBlendAnimation()
{
    m_blendState.SetAniLoop( false );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSkeleton::GetNumBlendAnimations()
{
    // xxx: 기존 호환
    return ( m_blendState.HasAnimation() ? 1 : 0 );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetCurFrame( float fFrame )
{
    m_curState.SetCurFrame( fFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitSkeleton::GetCurFrame()
{
    return m_curState.GetCurFrame();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetIpolFrame( int nFrame )
{
    m_curState.SetStartIpolFrame( nFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetBlendIpolFrame( int nStartFrame, int nEndFrame )
{
    m_aniBlendInfo.m_iStartFrame = nStartFrame;
    m_aniBlendInfo.m_iEndFrame = nEndFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitSkeleton::IsInterpolating()
{
    return m_curState.IsInterpolating();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::VerifyTransformBuf( int iSize )
{
    if( iSize > m_iTransformBufSize )
    {
        ResizeTransformBuf( iSize );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline WzTransform* CWzUnitSkeleton::GetTransformBuf()
{
    return m_pTransformBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SaveLastTransform()
{
    if( m_iTransformBufSize > 0 )
    {
        WzAssert( m_pLastTransformBuf );
        WzAssert( m_pTransformBuf );

        memcpy( (BYTE*)m_pLastTransformBuf, 
                (BYTE*)m_pTransformBuf, 
                sizeof( WzTransform ) * m_iTransformBufSize );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline WzTransform* CWzUnitSkeleton::GetLastTransformBuf()
{
    return m_pLastTransformBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* CWzUnitSkeleton::GetTransformMatrixBuf( int iSize )
{
    VerifyTransformBuf( iSize );

    return m_pTransformMatBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* CWzUnitSkeleton::GetSkinMatrixBuf( int iSize )
{
    VerifyTransformBuf( iSize );

    return m_pSkinMatBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSkeleton::GetTransformBufSize()
{
    return m_iTransformBufSize;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitSkeleton::GetBonePos( int nIndex )
{
    WzAssert( m_pTransformMatBuf );

    if( m_pTransformMatBuf && nIndex >= 0 && nIndex < m_iTransformBufSize )
    {
        static WzVector wvBonePos;

        SetVector( &wvBonePos, m_pTransformMatBuf[nIndex]._41, 
                               m_pTransformMatBuf[nIndex]._42, 
                               m_pTransformMatBuf[nIndex]._43 );

        return wvBonePos;
    }

    return m_wvPos;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetScale( const WzVector& wvScale, BOOL bUpdateBV )
{
    if( m_wvScale != wvScale )
    {
        m_wvScale = wvScale;
        m_stateFlag.Set( UDSS_BV_UPDATE | UDSS_TPARAM_UPDATE );

        if( bUpdateBV )
        {
            UpdateBV( TRUE );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitSkeleton::GetScale() const
{
    return m_wvScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetRotation( const WzVector& wvRotate, BOOL bUpdateBV )
{
    if( m_wvRotate != wvRotate )
    {
        m_wvRotate = wvRotate;
        m_stateFlag.Set( UDSS_ROT_DIRTY | UDSS_BV_UPDATE | UDSS_TPARAM_UPDATE );

        if( bUpdateBV )
        {
            UpdateBV( TRUE );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitSkeleton::GetRotation() const
{
    return m_wvRotate;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetPosition( const WzVector& wvPos, BOOL bUpdateBV )
{
    if( m_wvPos != wvPos )
    {
        m_wvPos = wvPos;
        m_stateFlag.Set( UDSS_BV_UPDATE | UDSS_TPARAM_UPDATE );

        if( bUpdateBV )
        {
            UpdateBV( TRUE );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitSkeleton::GetPosition() const
{
    return m_wvPos;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::UpdateTransform()
{
     m_stateFlag.Set( UDSS_ROT_DIRTY | UDSS_BV_UPDATE | UDSS_TPARAM_UPDATE );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetRotationByMatrix( const WzMatrix* pwmRotate )
{
    if( pwmRotate )
    {
        m_wmRotate = *pwmRotate;

        m_stateFlag.Unset( UDSS_ROT_DIRTY );
        m_stateFlag.Set( UDSS_TPARAM_UPDATE );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::GetRotationMatrix( WzMatrix* pwmOutRotate )
{
    WzAssert( pwmOutRotate );

    if( m_stateFlag.Check( UDSS_ROT_DIRTY ) )
    {
        if( m_pTransformOffset && m_pTransformOffset->m_bRotUse )
        {
            MatrixRotationZYX( &m_wmRotate, &(m_pTransformOffset->m_wvRot + m_wvRotate));
        }
        else
        {
            MatrixRotationZYX( &m_wmRotate, &m_wvRotate );
        }

        m_stateFlag.Unset( UDSS_ROT_DIRTY );
    }

    *pwmOutRotate = m_wmRotate;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetBaseBV( const WzBoundingVolume& wbv )
{
    m_bvBase = wbv;
    m_stateFlag.Set( UDSS_BV_UPDATE );

    UpdateBV( TRUE );
}
//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSkeleton::GetSafeBoneIndex( int iBoneIndex ) const
{
    if( iBoneIndex < 0 || iBoneIndex >= m_iTransformBufSize )
    {
        return 0;
    }

    return iBoneIndex;
}
//------------------------------------------------------------------------------
/**
*/
inline const WzBoundingVolume& CWzUnitSkeleton::GetBV() const
{
    return m_bvWorld; 
}
//------------------------------------------------------------------------------
/**
*/
inline WzBoundingVolume* CWzUnitSkeleton::GetBVPtr()
{
    return &m_bvWorld; 
}
//------------------------------------------------------------------------------
/**
*/
inline const WzBoundingVolume& CWzUnitSkeleton::GetBaseBV() const
{
    return m_bvBase; 
}
//------------------------------------------------------------------------------
inline WzBoundingVolume* CWzUnitSkeleton::GetBaseBVPtr()
{
    return &m_bvBase; 
}
//------------------------------------------------------------------------------
/**
*/
inline BvSphere* CWzUnitSkeleton::GetBvSpherePtr()
{
    return &m_bvSphere; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSkeleton::GetNumBVExtra()
{
    return m_arBVExtra.GetCount(); 
}

//------------------------------------------------------------------------------
/**
*/
inline WzBVExtraInfo* CWzUnitSkeleton::GetBVExtraPtrByIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arBVExtra.GetCount() )
    {
        return m_arBVExtra.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSkeleton::GetLOD() const
{
    return m_iLOD; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSkeleton::SetForceLOD( int iLevel )
{
    m_iForceLOD = iLevel; 
}

//------------------------------------------------------------------------------
/**
*/
inline const int* CWzUnitSkeleton::GetAreaLightPtr() const
{
    return m_aiAreaLight;
}

#endif // _3DFILEACCESS_WZUNITSKELETON_H_