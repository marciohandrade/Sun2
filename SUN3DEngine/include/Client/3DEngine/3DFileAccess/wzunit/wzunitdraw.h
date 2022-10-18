// wzunitdraw.h

#ifndef _3DFILEACCESS_WZUNITDRAW_H_
#define _3DFILEACCESS_WZUNITDRAW_H_

#include <programcommon/wzflag.h>
#include <programcommon/wzref.h>
#include <client/3dengine/common/wzmatrix.h>
#include "wzunit.h"
#include "wzunitskeleton.h"

class CWzSilhouette;
class CWzShadowVolume;

//------------------------------------------------------------------------------
/**
    @class CWzUnitDraw

    * 클라이언트 유닛 구성

       O     - 전체(베이스)도 CWzUnitDraw고, 각 파트(머리, 팔 등)도 CWzUnitDraw임
     --+--   - 베이스는 화면에 그려지진 않지만, 애니메이션등 여러가지 처리
       |     - 파트는 베이스에서 대부분 값을 얻고, 주로 렌더링만 함
      / |    - 엔진 초기 구현시 캐릭터가 파트로 이뤄질 거란 생각을 못했던 것 같음
     /  |    - 이런 이유로 CWzUnitSkeleton(뼈다구)을 만들었고 베이스가 뼈다구를
               포함하게 함, 파트는 뼈다구 포인터가 NULL임
             - CWzUnitSkeleton에서 베이스가 할 일 대부분 처리
*/
class CWzUnitDraw
{
public:
    // 생성자/소멸자
    CWzUnitDraw( DWORD dwParam = 0 );
    virtual ~CWzUnitDraw();

    // 파괴
    virtual void Destroy();

    // WzUnit 설정/얻기
    WZ3DA_API void SetWzUnit( CWzUnit* pUnit, BOOL bDynamic = TRUE );
    CWzUnit* GetWzUnit() const;

    // 서브 타입 설정/얻기
    WZ3DA_API void SetSubType( int iSubType );
    int GetSubType() const;

    // xxx: 기존 호환 (제거 예정)
    int GetSubTypeNum();

    // 참조 UnitDraw 설정/얻기
    // xxx: 애니메이션만 참조하는 것은 아님
    WZ3DA_API void SetAniRefUnitDraw( CWzUnitDraw* pRefUnitDraw );

    // -------- 애니메이션 관련 --------

    // 현재 애니메이션 설정
    WZ3DA_API void SetCurrentAnimation( WzID wiAni, 
                                        BOOL bLoop = TRUE, 
                                        BOOL bRestart = FALSE, 
                                        int nInterpolationFrame = -1 );

    // 현재 애니메이션 ID 얻기
    WzID GetCurrentAnimation();
    // 현재 애니메이션 상태 얻기
    CWzAniState* GetCurAniState();

    // 현재 애니메이션 정보 얻기
    WzAnimationInfo* GetCurrentAnimationInfo();
    // 이전 애니메이션 정보 얻기
    WzAnimationInfo* GetPrevAnimationInfo();

    // xxx: 기존 호환 (제거 예정)
    void ClearNextAnimation();

    // 블랜딩 애니메이션 설정
    // xxx: 기존 호환 (fBlendWeight는 사용 안함)
    WZ3DA_API void SetCurrentBlendAnimation( WzID wiAni, 
                                             int nBoneIdx, 
                                             BOOL bLoop = FALSE, 
                                             float fSpeedRatio = 1.0f,
                                             BOOL bDeltaType = FALSE, 
                                             float fBlendWeight = 1.0f );

    // 블랜딩 애니메이션 제거
    WZ3DA_API void UnSetBlendAnimation();
    // 블랜딩 애니메이션 반복 안 함
    void UnLoopBlendAnimation();

    // xxx: 기존 호환 (제거 예정)
    int GetBlendAniCount();

    // 프레임 진행
    WZ3DA_API ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                                   WzEvent* pEvent = NULL,
                                                   float* pfProgressRate = NULL );

    // 매 프레임 처리 (xxx: 절두체 안에 있는 넘들만)
    WZ3DA_API void Process( const WzMatrix* pwmParent );

    // 현재 프레임 설정/얻기
    void SetFrame( float fFrame );
    float GetFrame();

    // xxx: 기존 호환 (제거 예정)
    float GetCurrentFrame();

    // 보간 프레임 설정
    WZ3DA_API void SetInterpolationFrame( int nInterpolationFrame );
    // 블랜딩 보간 프레임 설정
    WZ3DA_API void SetBlendInterpolationFrame( int nStartFrame, int nEndFrame = -1 );

    // 보간 중인가?
    BOOL IsInterpolating();

    // 현재 변환 얻기
    WzTransform* GetTransform();
    // 마지막으로 저장된 변환 얻기 (다음 애니메이션과 보간용)
    WzTransform* GetLastSavedTransform();

    // 변환 행렬 버퍼 얻기
    WZ3DA_API WzMatrix* GetTransformBuffer( int iSize = -1 );
    // 스키닝 행렬 버퍼 얻기
    WZ3DA_API WzMatrix* GetInverseAndTransformBuffer( int iSize = -1 );

    // 변환 버퍼 크기 얻기
    int GetTransformBufferSize();

    // -------- 위치/회전/크기 관련 --------

    // 크기 설정/얻기
    WZ3DA_API void SetScale( const WzVector& wvScale, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetScale( float x, float y, float z, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetScale( float fScale, BOOL bUpdateBV = TRUE );
    const WzVector& GetScale() const;
    BOOL GetScale( WzVector* pwvScale ) const;
    
    // 회전 설정/얻기
    WZ3DA_API void SetRotation( const WzVector& wvRotate, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetRotation( float x, float y, float z, BOOL bUpdateBV = TRUE );
    const WzVector& GetRotation() const;

    // 위치 설정/얻기
    WZ3DA_API void SetPosition( const WzVector& wvPos, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetPosition( float x, float y, float z, BOOL bUpdateBV = TRUE );
    const WzVector& GetPosition() const;

    // 본 위치 얻기
    const WzVector& GetBonePos( int nIndex );

    // 회전 행렬 설정/얻기
    void SetRotationByMatrix( const WzMatrix* pwmRotate );
    void GetRotationMatrix( WzMatrix* pwmOutRotate );

    // 외부에서 변환 flag 강제로 업데이트(툴에서 사용)
    void UpdateTransform();

    // -------- 바운딩 볼륨 관련 --------

    // 기본(로컬) 바운딩 볼륨 설정
    WZ3DA_API void SetBoundingVolume( const WzBoundingVolume& wbv );

    // 월드 바운딩 볼륨 갱신 (변환 행렬 적용)
    WZ3DA_API void UpdateBoundingVolumeWithMatrix( WzMatrix* pwmTransform );

    // 월드 바운딩 볼륨 얻기
    const WzBoundingVolume& GetBoundingVolume() const;
    WzBoundingVolume* GetBoundingVolumePtr();
    const WzBoundingVolume& GetBaseBoundingVolume() const;
    WzBoundingVolume* GetBaseBoundingVolumePtr();

    // 월드 바운딩 구 포인터 얻기
    BvSphere* GetBoundingSpherePtr();

    // 추가 바운딩 볼륨 초기화
    WZ3DA_API void ResetBVExtra();

    // 추가 바운딩 볼륨 수 얻기
    int GetCountBoundingVolumeExtra();
    // 인덱스로 추가 바운딩 볼륨 얻기
    WzBVExtraInfo* GetBoundingVolumeExtraPtrByIndex( int iIndex );

    // -------- LOD 관련 --------

    // LOD 얻기
    int GetLODLevel() const;
    // 데이타 LOD 얻기
    int GetWzdLOD() const;

    // 강제 LOD 설정
    void SetForcedLOD( int iLevel = -1 );

    // LOD별 모든 메시 로딩 여부
    BOOL IsUnitFullLoaded() const;

    // -------- 그림자 관련 --------

    // 실루엣 갱신
    WZ3DA_API void UpdateSilhouette( CWzdBase* pWzd, 
                                     const WzVector& wvRay,
                                     const WzVector* pwvTransformedVertex = NULL );
    // 실루엣 얻기
    CWzSilhouette* GetSilhouette();

    // 그림자 볼륨 갱신
    WZ3DA_API void UpdateShadowVolume( CWzdBase* pWzd, 
                                       const WzVector& wvRay, 
                                       const WzVector* pwvTransformedVertex = NULL, 
                                       DWORD dwOption = 0 );
    // 그림자 볼륨 얻기
    CWzShadowVolume* GetShadowVolume();

    // 그림자 주고/받기 정보 초기화
    WZ3DA_API void InitShadowCastRecvInfo();

    // 그림자 주기 설정/얻기 (-1이면 모두 Cast)
    WZ3DA_API void EnableShadowCast( BOOL bEnable, int nRenderInfoIndex = -1 );
    WZ3DA_API BOOL IsEnableShadowCast( int nRenderInfoIndex );

    // 그림자 받기 설정/얻기 (-1이면 모두 Receive)
    WZ3DA_API void EnableShadowReceive( BOOL bEnable, int nRenderInfoIndex = -1 );
    WZ3DA_API BOOL IsEnableShadowReceive( int nRenderInfoIndex );

    // 라이트맵 설정/얻기
    void SetLightMap( HANDLE hLightMap );
    HANDLE GetLightMap() const;

    // 라이트맵이 있는가?
    BOOL IsLightMap() const;

    // -------- 이펙트 관련 --------

    // 이펙트 정보 추가
    WZ3DA_API void AddUnitEffectInfo( const WzUnitEffectInfo* pInfo );
    // 인덱스로 이펙트 정보 얻기
    WzUnitEffectInfo* GetUnitEffectInfo( int iIndex );

    // 이펙트 정보 수 얻기
    int GetNumUnitEffectInfo();

    // 이펙트 정보 제거
    WZ3DA_API void RemoveUnitEffectInfo( int iIndex );
    WZ3DA_API void ClearAllUnitEffectInfo();

    // 이펙트 크기 설정
    WZ3DA_API void SetEffectScale( HANDLE hEffect, const WzVector& wvScale );
    // 이펙트 회전 설정
    WZ3DA_API void SetEffectRotation( HANDLE hEffect, const WzQuaternion& wqRot );
    // 이펙트 위치 설정
    WZ3DA_API void SetEffectPosition( HANDLE hEffect, const WzVector& wvPos );

    // 이펙트 회전 적용 여부 설정
    WZ3DA_API void SetEffectApplyRotation( HANDLE hEffect, BOOL bRotation );

    // 이펙트 생성해라
    void CreateUnitEffect( BOOL bForce = FALSE );

    // 이펙트 생성/제거 됐음
    void OnUnitEffectCreate();
    void OnUnitEffectDestroy();

    // -------- 렌더링 요소 관련 --------

    // 굴절율 설정/얻기
    void SetRefractionRate( float fRefractionRate );
    float GetRefractionRate() const;

    // 굴절 알파 설정/얻기
    void SetRefractionAlpha( float fRefractionAlpha );
    float GetRefractionAlpha() const;

    // 팽창율 설정/얻기
    void SetInflateFactor( float fInflateFactor );
    float GetInflateFactor() const;

    // Fade 요소 설정/얻기
    void SetFadeFactor( float fFadeFactor );
    float GetFadeFactor() const;

    // 스피큘러 강도 설정/얻기
    void SetSpecularIntensity( float fIntensity = 1.0f );
    float GetSpecularIntensity() const;

    // 외부 텍스처 설정/얻기
    // 텍스처 핸들값이 INVALID_HANDLE_VALUE일 경우 Disable
    // iMeshGroup이 -1일 경우 전체 적용
    void SetExternalTexture( int iMeshGroup, HANDLE hTexture, int index = 0 );
    const St_ExternalTextureInfo* GetExternalTexture( int index = 0 ) const;

    // xxx: 사용 안 함(기존 호환)
    void EnableUpdateLight( BOOL bEnable );

    // 지역 라이트 얻기
    const int* GetAreaLightPtr() const;

    // -------- 렌더링 옵션 관련 --------

    // 렌더링 타입 설정/얻기
    void SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType = AR_NORMAL );
    ENUM_ALTERNATIVE_RENDER GetRenderType() const;

    // 스키닝 활성 여부 설정/얻기 (베이스만 설정 가능)
    void EnableSkinning( BOOL bEnable );
    BOOL IsSkinning() const;

    // 노말맵 적용 여부 설정/얻기 (베이스만 설정 가능)
    void EnableNormalMap( BOOL bEnable );
    BOOL IsNormalMap() const;

    // 노말맵 굴절 활성 여부 설정/얻기
    void EnableNormalMapRefraction( BOOL bEnable );
    BOOL IsNormalMapRefraction() const;

    // 서브 타입 알파 블랜딩 활성 여부 설정/얻기
    void EnableAlphaBlendOfSubType( BOOL bEnable );
    BOOL IsAlphaBlendOfSubType() const;

    // xxx: 모냐?
    void EnableCullFaceModeIn( BOOL bEnable );
    BOOL IsCullFaceModeIn() const;

    // 태양 그림자 활성 여부 설정/얻기
    void EnableShadowOnSunLight( BOOL bEnable );
    BOOL IsShadowOnSunLight() const;

    // 서브 타입의 칼라 활성 여부 설정/얻기
    void EnableColorOfSubType( BOOL bEnable );
    BOOL IsColorOfSubType() const;

    // 외부 텍스처 활성 여부 설정/얻기
    void EnableExternalTexture( BOOL bEnable );
    BOOL IsExternalTexture() const;

    // 마지막 알파 렌더링 활성 여부 설정/얻기
    void EnableLastAlphaRender( BOOL bEnable );
    BOOL IsLastAlphaRender() const;

    // 커스텀 라이트 활성 여부 설정/얻기
    void EnableCustomLight( BOOL bEnable );
    BOOL IsCustomLight() const;

    // 기타
    BOOL CheckCreateParam( DWORD dwFlag );
    BOOL CheckStateFlag( DWORD dwFlag );

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void Release();

    // 데이타 LOD 갱신
    void UpdateWzdLOD();

    // 이펙트 처리
    void ProcessEffect();

protected:
    CWzUnit* m_pUnit;
    int m_iSubType;

    CWzUnitDraw* m_pRefUnitDraw;
    CWzRef<CWzUnitSkeleton> m_refUnitSkeleton;

    CWzSilhouette* m_pwsSilhouette;
    CWzShadowVolume* m_pwsvShadowVolume;

    BOOL* m_pbShadowCast;
    BOOL* m_pbShadowReceive;

    HANDLE m_hLightMap;

    CWzArray<WzUnitEffectInfo*> m_arEffectInfo;

    float m_fRefractionRate;    // 외곡 율 0 ~ 1사이 값
    float m_fRefractionAlpha;   // 외곡 알파도 ( 0 - Diffuse 100%, 1 - 외곡 100% )
    float m_fInflateFactor;     // 팽창 값

    float m_fSpecularIntensity;

    St_ExternalTextureInfo m_ExtTextureInfo[WZU_NUM_EXTERNAL_TEXTURE];

    ENUM_ALTERNATIVE_RENDER m_eRenderType;
    CWzFlag<DWORD> m_renderOption;

    CWzFlag<DWORD> m_createFlag;
    CWzFlag<DWORD> m_stateFlag;

    float m_fFadeFactor;
    int m_iWzdLOD;
};

// 인라인 함수들 추가
#include "wzunitdraw.inl"
#include "wzunitdraw_ani.inl"
#include "wzunitdraw_renderopt.inl"

#endif // _3DFILEACCESS_WZUNITDRAW_H_
