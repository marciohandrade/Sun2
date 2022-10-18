// wzunitskeleton.h

#ifndef _3DFILEACCESS_WZUNITSKELETON_H_
#define _3DFILEACCESS_WZUNITSKELETON_H_

#include <programcommon/wzflag.h>
#include "wzanistate.h"
#include "wzanistatecb.h"
#include "wzunit.h"
#include "../wzdbase/wzdtypedef.h"

// ���� �÷���
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
    // ������/�Ҹ���
    CWzUnitSkeleton( CWzUnitDraw* pOwner );
    virtual ~CWzUnitSkeleton();

    // WzUnit�� ������ ���
    void OnWzUnitSet( BOOL bDynamic );

    // SubType�� ����� ���
    void OnSubTypeChange();

    // -------- �ִϸ��̼� ���� --------

    // ���� �ִϸ��̼� ����
    void SetCurAnimation( WzID wiAni, 
                          BOOL bLoop,
                          BOOL bRestart,
                          int nIpolFrame );

    // ���� �ִϸ��̼� ID ���
    WzID GetCurAnimationID();
    // ���� �ִϸ��̼� ���� ���
    CWzAniState* GetCurAniState();

    // ���� �ִϸ��̼� ���� ���
    WzAnimationInfo* GetCurAnimationInfo();
    // ���� �ִϸ��̼� ���� ���
    WzAnimationInfo* GetPrevAnimationInfo();

    // ���� �ִϸ��̼� ����
    void SetCurBlendAnimation( WzID wiAni, 
                               int nBoneIdx, 
                               BOOL bLoop, 
                               float fSpeedRatio, 
                               BOOL bDeltaType );

    // ���� �ִϸ��̼� ����
    void UnsetBlendAnimation();
    // ���� �ִϸ��̼� �ݺ� �� ��
    void UnloopBlendAnimation();

    // ���� �ִϸ��̼� �� ���
    // xxx: ���� ����, �������� �������� �����߾���
    int GetNumBlendAnimations();

    // ������ ����
    ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                         WzEvent* pEvent,
                                         float* pfProgressRate );

    // �� ������ ó��
    void Process( const WzMatrix* pwmParent );

    // ������ ���� ����
    void SetCurFrame( float fFrame );
    // ���� ������ ���
    float GetCurFrame();

    // ���� �����Ӽ� ����
    void SetIpolFrame( int nFrame );
    // ���� ���� �����Ӽ� ����
    void SetBlendIpolFrame( int nStartFrame, int nEndFrame );

    // ���� ���ΰ�?
    BOOL IsInterpolating();

    // ��ȯ ���� ũ�� ���
    int GetTransformBufSize();

    // ��ȯ ���� ���
    WzTransform* GetTransformBuf();
    // �ֱ�(���������� �ִϸ��̼� ��) ��ȯ ���� ���
    WzTransform* GetLastTransformBuf();

    // ��ȯ ��� ���� ���
    WzMatrix* GetTransformMatrixBuf( int iSize );
    // ��Ű�� ��� ���� ���
    WzMatrix* GetSkinMatrixBuf( int iSize );

    // �� ��ġ ���
    const WzVector& GetBonePos( int nIndex );

    // �ִϸ��̼� ���� �ݹ�
    virtual void OnAniStateNotice( int iCode, DWORD dwParam );

    // -------- ��ġ/ȸ��/ũ�� ���� --------

    // ũ�� ����/���
    void SetScale( const WzVector& wvScale, BOOL bUpdateBV );
    const WzVector& GetScale() const;
    
    // ȸ�� ����/���
    void SetRotation( const WzVector& wvRotate, BOOL bUpdateBV );
    const WzVector& GetRotation() const;

    // ��ġ ����/���
    void SetPosition( const WzVector& wvPos, BOOL bUpdateBV );
    const WzVector& GetPosition() const;

    // ȸ�� ��� ����/���
    void SetRotationByMatrix( const WzMatrix* pwmRotate );
    void GetRotationMatrix( WzMatrix* pwmOutRotate );

    // ��ȯ flag ����
    void UpdateTransform();

    // -------- �ٿ�� ���� ���� --------

    // �⺻(����) �ٿ�� ���� ����
    void SetBaseBV( const WzBoundingVolume& wbv );

    // ���� �ٿ�� ���� ����
    void UpdateBV( BOOL bUpdateNow, const WzMatrix* pwmTransform = NULL, int iRefBone= -1 );

    // ���� �ٿ�� ���� ���
    const WzBoundingVolume& GetBV() const;
    WzBoundingVolume* GetBVPtr();
    const WzBoundingVolume& GetBaseBV() const;
    WzBoundingVolume* GetBaseBVPtr();

    // ���� �ٿ�� �� ������ ���
    BvSphere* GetBvSpherePtr();

    // �߰� �ٿ�� ���� �ʱ�ȭ
    void InitBVExtra();
    // �߰� �ٿ�� ���� ����
    void UpdateBVExtra();

    // �߰� �ٿ�� ���� �� ���
    int GetNumBVExtra();
    // �ε����� �߰� �ٿ�� ���� ���
    WzBVExtraInfo* GetBVExtraPtrByIndex( int iIndex );

    // -------- ��Ÿ --------

    // LOD ���
    int GetLOD() const;

    // ���� LOD ����
    void SetForceLOD( int iLevel );

    // ����Ʈ ���
    const int* GetAreaLightPtr() const;

private:
    // �⺻ ������
    CWzUnitSkeleton();

    // ����
    void Release();

    // ��ȯ ���۵� ����
    void FreeTransformBuf();
    // ��ȯ ���۵� ����
    void ResizeTransformBuf( int iSize );

    // ��ȯ ���� �˻�
    void VerifyTransformBuf( int iSize );

    // �⺻ �ٿ�� ���� �ʱ�ȭ
    void InitBaseBV( ENUM_BVTYPE eBvType );

    // ��ȿ�Ѻ� ���
    int GetSafeBoneIndex( int iBoneIndex ) const;

    // �߰� �ٿ�� ���� ����
    void ReleaseBVExtra();

    // �ֱ�(������) ��ȯ ����
    void SaveLastTransform();

    // ��ȯ �Ķ���� �ʱ�ȭ/����
    void InitTransformParam();
    BOOL UpdateTransformParam( const WzMatrix* pwmParent );

    // ��ó��/��ó��
    void PreProcess( const WzMatrix* pwmParent );
    void PostProcess();

    // ���� LOD ����
    void UpdateLOD( const WzMatrix* pwmParent );

    // �θ� ���� �ɼ� ����
    void AdjustOwnerOption();

    // ���� ����Ʈ ����
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
    // xxx: ���� ȣȯ
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