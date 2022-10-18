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

    * Ŭ���̾�Ʈ ���� ����

       O     - ��ü(���̽�)�� CWzUnitDraw��, �� ��Ʈ(�Ӹ�, �� ��)�� CWzUnitDraw��
     --+--   - ���̽��� ȭ�鿡 �׷����� ������, �ִϸ��̼ǵ� �������� ó��
       |     - ��Ʈ�� ���̽����� ��κ� ���� ���, �ַ� �������� ��
      / |    - ���� �ʱ� ������ ĳ���Ͱ� ��Ʈ�� �̷��� �Ŷ� ������ ���ߴ� �� ����
     /  |    - �̷� ������ CWzUnitSkeleton(���ٱ�)�� ������� ���̽��� ���ٱ���
               �����ϰ� ��, ��Ʈ�� ���ٱ� �����Ͱ� NULL��
             - CWzUnitSkeleton���� ���̽��� �� �� ��κ� ó��
*/
class CWzUnitDraw
{
public:
    // ������/�Ҹ���
    CWzUnitDraw( DWORD dwParam = 0 );
    virtual ~CWzUnitDraw();

    // �ı�
    virtual void Destroy();

    // WzUnit ����/���
    WZ3DA_API void SetWzUnit( CWzUnit* pUnit, BOOL bDynamic = TRUE );
    CWzUnit* GetWzUnit() const;

    // ���� Ÿ�� ����/���
    WZ3DA_API void SetSubType( int iSubType );
    int GetSubType() const;

    // xxx: ���� ȣȯ (���� ����)
    int GetSubTypeNum();

    // ���� UnitDraw ����/���
    // xxx: �ִϸ��̼Ǹ� �����ϴ� ���� �ƴ�
    WZ3DA_API void SetAniRefUnitDraw( CWzUnitDraw* pRefUnitDraw );

    // -------- �ִϸ��̼� ���� --------

    // ���� �ִϸ��̼� ����
    WZ3DA_API void SetCurrentAnimation( WzID wiAni, 
                                        BOOL bLoop = TRUE, 
                                        BOOL bRestart = FALSE, 
                                        int nInterpolationFrame = -1 );

    // ���� �ִϸ��̼� ID ���
    WzID GetCurrentAnimation();
    // ���� �ִϸ��̼� ���� ���
    CWzAniState* GetCurAniState();

    // ���� �ִϸ��̼� ���� ���
    WzAnimationInfo* GetCurrentAnimationInfo();
    // ���� �ִϸ��̼� ���� ���
    WzAnimationInfo* GetPrevAnimationInfo();

    // xxx: ���� ȣȯ (���� ����)
    void ClearNextAnimation();

    // ���� �ִϸ��̼� ����
    // xxx: ���� ȣȯ (fBlendWeight�� ��� ����)
    WZ3DA_API void SetCurrentBlendAnimation( WzID wiAni, 
                                             int nBoneIdx, 
                                             BOOL bLoop = FALSE, 
                                             float fSpeedRatio = 1.0f,
                                             BOOL bDeltaType = FALSE, 
                                             float fBlendWeight = 1.0f );

    // ���� �ִϸ��̼� ����
    WZ3DA_API void UnSetBlendAnimation();
    // ���� �ִϸ��̼� �ݺ� �� ��
    void UnLoopBlendAnimation();

    // xxx: ���� ȣȯ (���� ����)
    int GetBlendAniCount();

    // ������ ����
    WZ3DA_API ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                                   WzEvent* pEvent = NULL,
                                                   float* pfProgressRate = NULL );

    // �� ������ ó�� (xxx: ����ü �ȿ� �ִ� �ѵ鸸)
    WZ3DA_API void Process( const WzMatrix* pwmParent );

    // ���� ������ ����/���
    void SetFrame( float fFrame );
    float GetFrame();

    // xxx: ���� ȣȯ (���� ����)
    float GetCurrentFrame();

    // ���� ������ ����
    WZ3DA_API void SetInterpolationFrame( int nInterpolationFrame );
    // ���� ���� ������ ����
    WZ3DA_API void SetBlendInterpolationFrame( int nStartFrame, int nEndFrame = -1 );

    // ���� ���ΰ�?
    BOOL IsInterpolating();

    // ���� ��ȯ ���
    WzTransform* GetTransform();
    // ���������� ����� ��ȯ ��� (���� �ִϸ��̼ǰ� ������)
    WzTransform* GetLastSavedTransform();

    // ��ȯ ��� ���� ���
    WZ3DA_API WzMatrix* GetTransformBuffer( int iSize = -1 );
    // ��Ű�� ��� ���� ���
    WZ3DA_API WzMatrix* GetInverseAndTransformBuffer( int iSize = -1 );

    // ��ȯ ���� ũ�� ���
    int GetTransformBufferSize();

    // -------- ��ġ/ȸ��/ũ�� ���� --------

    // ũ�� ����/���
    WZ3DA_API void SetScale( const WzVector& wvScale, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetScale( float x, float y, float z, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetScale( float fScale, BOOL bUpdateBV = TRUE );
    const WzVector& GetScale() const;
    BOOL GetScale( WzVector* pwvScale ) const;
    
    // ȸ�� ����/���
    WZ3DA_API void SetRotation( const WzVector& wvRotate, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetRotation( float x, float y, float z, BOOL bUpdateBV = TRUE );
    const WzVector& GetRotation() const;

    // ��ġ ����/���
    WZ3DA_API void SetPosition( const WzVector& wvPos, BOOL bUpdateBV = TRUE );
    WZ3DA_API void SetPosition( float x, float y, float z, BOOL bUpdateBV = TRUE );
    const WzVector& GetPosition() const;

    // �� ��ġ ���
    const WzVector& GetBonePos( int nIndex );

    // ȸ�� ��� ����/���
    void SetRotationByMatrix( const WzMatrix* pwmRotate );
    void GetRotationMatrix( WzMatrix* pwmOutRotate );

    // �ܺο��� ��ȯ flag ������ ������Ʈ(������ ���)
    void UpdateTransform();

    // -------- �ٿ�� ���� ���� --------

    // �⺻(����) �ٿ�� ���� ����
    WZ3DA_API void SetBoundingVolume( const WzBoundingVolume& wbv );

    // ���� �ٿ�� ���� ���� (��ȯ ��� ����)
    WZ3DA_API void UpdateBoundingVolumeWithMatrix( WzMatrix* pwmTransform );

    // ���� �ٿ�� ���� ���
    const WzBoundingVolume& GetBoundingVolume() const;
    WzBoundingVolume* GetBoundingVolumePtr();
    const WzBoundingVolume& GetBaseBoundingVolume() const;
    WzBoundingVolume* GetBaseBoundingVolumePtr();

    // ���� �ٿ�� �� ������ ���
    BvSphere* GetBoundingSpherePtr();

    // �߰� �ٿ�� ���� �ʱ�ȭ
    WZ3DA_API void ResetBVExtra();

    // �߰� �ٿ�� ���� �� ���
    int GetCountBoundingVolumeExtra();
    // �ε����� �߰� �ٿ�� ���� ���
    WzBVExtraInfo* GetBoundingVolumeExtraPtrByIndex( int iIndex );

    // -------- LOD ���� --------

    // LOD ���
    int GetLODLevel() const;
    // ����Ÿ LOD ���
    int GetWzdLOD() const;

    // ���� LOD ����
    void SetForcedLOD( int iLevel = -1 );

    // LOD�� ��� �޽� �ε� ����
    BOOL IsUnitFullLoaded() const;

    // -------- �׸��� ���� --------

    // �Ƿ翧 ����
    WZ3DA_API void UpdateSilhouette( CWzdBase* pWzd, 
                                     const WzVector& wvRay,
                                     const WzVector* pwvTransformedVertex = NULL );
    // �Ƿ翧 ���
    CWzSilhouette* GetSilhouette();

    // �׸��� ���� ����
    WZ3DA_API void UpdateShadowVolume( CWzdBase* pWzd, 
                                       const WzVector& wvRay, 
                                       const WzVector* pwvTransformedVertex = NULL, 
                                       DWORD dwOption = 0 );
    // �׸��� ���� ���
    CWzShadowVolume* GetShadowVolume();

    // �׸��� �ְ�/�ޱ� ���� �ʱ�ȭ
    WZ3DA_API void InitShadowCastRecvInfo();

    // �׸��� �ֱ� ����/��� (-1�̸� ��� Cast)
    WZ3DA_API void EnableShadowCast( BOOL bEnable, int nRenderInfoIndex = -1 );
    WZ3DA_API BOOL IsEnableShadowCast( int nRenderInfoIndex );

    // �׸��� �ޱ� ����/��� (-1�̸� ��� Receive)
    WZ3DA_API void EnableShadowReceive( BOOL bEnable, int nRenderInfoIndex = -1 );
    WZ3DA_API BOOL IsEnableShadowReceive( int nRenderInfoIndex );

    // ����Ʈ�� ����/���
    void SetLightMap( HANDLE hLightMap );
    HANDLE GetLightMap() const;

    // ����Ʈ���� �ִ°�?
    BOOL IsLightMap() const;

    // -------- ����Ʈ ���� --------

    // ����Ʈ ���� �߰�
    WZ3DA_API void AddUnitEffectInfo( const WzUnitEffectInfo* pInfo );
    // �ε����� ����Ʈ ���� ���
    WzUnitEffectInfo* GetUnitEffectInfo( int iIndex );

    // ����Ʈ ���� �� ���
    int GetNumUnitEffectInfo();

    // ����Ʈ ���� ����
    WZ3DA_API void RemoveUnitEffectInfo( int iIndex );
    WZ3DA_API void ClearAllUnitEffectInfo();

    // ����Ʈ ũ�� ����
    WZ3DA_API void SetEffectScale( HANDLE hEffect, const WzVector& wvScale );
    // ����Ʈ ȸ�� ����
    WZ3DA_API void SetEffectRotation( HANDLE hEffect, const WzQuaternion& wqRot );
    // ����Ʈ ��ġ ����
    WZ3DA_API void SetEffectPosition( HANDLE hEffect, const WzVector& wvPos );

    // ����Ʈ ȸ�� ���� ���� ����
    WZ3DA_API void SetEffectApplyRotation( HANDLE hEffect, BOOL bRotation );

    // ����Ʈ �����ض�
    void CreateUnitEffect( BOOL bForce = FALSE );

    // ����Ʈ ����/���� ����
    void OnUnitEffectCreate();
    void OnUnitEffectDestroy();

    // -------- ������ ��� ���� --------

    // ������ ����/���
    void SetRefractionRate( float fRefractionRate );
    float GetRefractionRate() const;

    // ���� ���� ����/���
    void SetRefractionAlpha( float fRefractionAlpha );
    float GetRefractionAlpha() const;

    // ��â�� ����/���
    void SetInflateFactor( float fInflateFactor );
    float GetInflateFactor() const;

    // Fade ��� ����/���
    void SetFadeFactor( float fFadeFactor );
    float GetFadeFactor() const;

    // ����ŧ�� ���� ����/���
    void SetSpecularIntensity( float fIntensity = 1.0f );
    float GetSpecularIntensity() const;

    // �ܺ� �ؽ�ó ����/���
    // �ؽ�ó �ڵ鰪�� INVALID_HANDLE_VALUE�� ��� Disable
    // iMeshGroup�� -1�� ��� ��ü ����
    void SetExternalTexture( int iMeshGroup, HANDLE hTexture, int index = 0 );
    const St_ExternalTextureInfo* GetExternalTexture( int index = 0 ) const;

    // xxx: ��� �� ��(���� ȣȯ)
    void EnableUpdateLight( BOOL bEnable );

    // ���� ����Ʈ ���
    const int* GetAreaLightPtr() const;

    // -------- ������ �ɼ� ���� --------

    // ������ Ÿ�� ����/���
    void SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType = AR_NORMAL );
    ENUM_ALTERNATIVE_RENDER GetRenderType() const;

    // ��Ű�� Ȱ�� ���� ����/��� (���̽��� ���� ����)
    void EnableSkinning( BOOL bEnable );
    BOOL IsSkinning() const;

    // �븻�� ���� ���� ����/��� (���̽��� ���� ����)
    void EnableNormalMap( BOOL bEnable );
    BOOL IsNormalMap() const;

    // �븻�� ���� Ȱ�� ���� ����/���
    void EnableNormalMapRefraction( BOOL bEnable );
    BOOL IsNormalMapRefraction() const;

    // ���� Ÿ�� ���� ���� Ȱ�� ���� ����/���
    void EnableAlphaBlendOfSubType( BOOL bEnable );
    BOOL IsAlphaBlendOfSubType() const;

    // xxx: ���?
    void EnableCullFaceModeIn( BOOL bEnable );
    BOOL IsCullFaceModeIn() const;

    // �¾� �׸��� Ȱ�� ���� ����/���
    void EnableShadowOnSunLight( BOOL bEnable );
    BOOL IsShadowOnSunLight() const;

    // ���� Ÿ���� Į�� Ȱ�� ���� ����/���
    void EnableColorOfSubType( BOOL bEnable );
    BOOL IsColorOfSubType() const;

    // �ܺ� �ؽ�ó Ȱ�� ���� ����/���
    void EnableExternalTexture( BOOL bEnable );
    BOOL IsExternalTexture() const;

    // ������ ���� ������ Ȱ�� ���� ����/���
    void EnableLastAlphaRender( BOOL bEnable );
    BOOL IsLastAlphaRender() const;

    // Ŀ���� ����Ʈ Ȱ�� ���� ����/���
    void EnableCustomLight( BOOL bEnable );
    BOOL IsCustomLight() const;

    // ��Ÿ
    BOOL CheckCreateParam( DWORD dwFlag );
    BOOL CheckStateFlag( DWORD dwFlag );

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void Release();

    // ����Ÿ LOD ����
    void UpdateWzdLOD();

    // ����Ʈ ó��
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

    float m_fRefractionRate;    // �ܰ� �� 0 ~ 1���� ��
    float m_fRefractionAlpha;   // �ܰ� ���ĵ� ( 0 - Diffuse 100%, 1 - �ܰ� 100% )
    float m_fInflateFactor;     // ��â ��

    float m_fSpecularIntensity;

    St_ExternalTextureInfo m_ExtTextureInfo[WZU_NUM_EXTERNAL_TEXTURE];

    ENUM_ALTERNATIVE_RENDER m_eRenderType;
    CWzFlag<DWORD> m_renderOption;

    CWzFlag<DWORD> m_createFlag;
    CWzFlag<DWORD> m_stateFlag;

    float m_fFadeFactor;
    int m_iWzdLOD;
};

// �ζ��� �Լ��� �߰�
#include "wzunitdraw.inl"
#include "wzunitdraw_ani.inl"
#include "wzunitdraw_renderopt.inl"

#endif // _3DFILEACCESS_WZUNITDRAW_H_
