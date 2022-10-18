// wzunitdraw.inl

#ifndef _3DFILEACCESS_WZUNITDRAW_INL_
#define _3DFILEACCESS_WZUNITDRAW_INL_

//------------------------------------------------------------------------------
/**
*/
inline CWzUnit* CWzUnitDraw::GetWzUnit() const
{
    return m_pUnit; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetSubType() const
{
    return m_iSubType; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetSubTypeNum()
{
    if( m_pUnit )
    {
        return m_pUnit->GetNumSubType();
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::CheckCreateParam( DWORD dwFlag )
{
    return m_createFlag.Check( dwFlag );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::CheckStateFlag( DWORD dwFlag )
{
    return m_stateFlag.Check( dwFlag );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetScale( const WzVector& wvScale, BOOL bUpdateBV )
{
    if( m_pRefUnitDraw )
    {
        // xxx: 실제로 이런 경우가 너무 많아서 주석 처리 했음
        // WZLOG( WZWAR, "CWzUnitDraw::SetScale() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetScale( wvScale, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetScale( float x, float y, float z, BOOL bUpdateBV )
{
    WzVector wvScale = { x, y, z };

    SetScale( wvScale, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetScale( float fScale, BOOL bUpdateBV )
{
    WzVector wvScale = { fScale, fScale, fScale };

    SetScale( wvScale, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitDraw::GetScale() const
{
    return m_refUnitSkeleton->GetScale();
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::GetScale( WzVector* pwvScale ) const
{
    WzAssert( pwvScale );

    *pwvScale = GetScale();
    
    return ( pwvScale->x != 1.0f || 
             pwvScale->y != 1.0f || 
             pwvScale->z != 1.0f ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRotation( const WzVector& wvRotate, BOOL bUpdateBV )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetRotation() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetRotation( wvRotate, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRotation( float x, float y, float z, BOOL bUpdateBV )
{
    WzVector wvRot = { x, y, z };

    SetRotation( wvRot, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitDraw::GetRotation() const
{
    return m_refUnitSkeleton->GetRotation(); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetPosition( const WzVector& wvPos, BOOL bUpdateBV )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetPosition() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetPosition( wvPos, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetPosition( float x, float y, float z, BOOL bUpdateBV )
{
    WzVector wvPos = { x, y, z };

    SetPosition( wvPos, bUpdateBV );
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitDraw::GetPosition() const
{
    return m_refUnitSkeleton->GetPosition(); 
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzUnitDraw::GetBonePos( int nIndex )
{
    return m_refUnitSkeleton->GetBonePos( nIndex );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRotationByMatrix( const WzMatrix* pwmRotate )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetRotationByMatrix() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetRotationByMatrix( pwmRotate );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::GetRotationMatrix( WzMatrix* pwmOutRotate )
{
    m_refUnitSkeleton->GetRotationMatrix( pwmOutRotate );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::UpdateTransform()
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::UpdateTransform() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->UpdateTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetBoundingVolume( const WzBoundingVolume& wbv )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetBoundingVolume() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetBaseBV( wbv );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::UpdateBoundingVolumeWithMatrix( WzMatrix* pwmTransform )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::UpdateBoundingVolumeWithMatrix() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->UpdateBV( TRUE, pwmTransform );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::ResetBVExtra()
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::UpdateBoundingVolumeExtra() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->InitBVExtra();
    m_refUnitSkeleton->UpdateBVExtra();
}

//------------------------------------------------------------------------------
/**
*/
inline const WzBoundingVolume& CWzUnitDraw::GetBoundingVolume() const
{
    return m_refUnitSkeleton->GetBV();
}
//------------------------------------------------------------------------------
/**
*/
inline WzBoundingVolume* CWzUnitDraw::GetBoundingVolumePtr()
{
    return m_refUnitSkeleton->GetBVPtr();
}
//------------------------------------------------------------------------------
/**
*/
inline const WzBoundingVolume& CWzUnitDraw::GetBaseBoundingVolume() const
{
    return m_refUnitSkeleton->GetBaseBV();
}
//------------------------------------------------------------------------------
/**
*/
inline WzBoundingVolume* CWzUnitDraw::GetBaseBoundingVolumePtr()
{
    return m_refUnitSkeleton->GetBaseBVPtr();
}

//------------------------------------------------------------------------------
/**
*/
inline BvSphere* CWzUnitDraw::GetBoundingSpherePtr()
{
    return m_refUnitSkeleton->GetBvSpherePtr();
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetCountBoundingVolumeExtra()
{
    return m_refUnitSkeleton->GetNumBVExtra();
}

//------------------------------------------------------------------------------
/**
*/
inline WzBVExtraInfo* CWzUnitDraw::GetBoundingVolumeExtraPtrByIndex( int iIndex )
{
    return m_refUnitSkeleton->GetBVExtraPtrByIndex( iIndex );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetLODLevel() const
{
    return m_refUnitSkeleton->GetLOD();
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetWzdLOD() const
{
    return m_iWzdLOD;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetForcedLOD( int iLevel )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetForcedLOD() - 잘못 호출됨!!" );
        return;
    }

    m_refUnitSkeleton->SetForceLOD( iLevel );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsUnitFullLoaded() const
{
    WzAssert( m_pUnit );

    if( m_pUnit )
    {
        return ( m_pUnit->GetLoadState() == eLOAD_FULL );
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline CWzSilhouette* CWzUnitDraw::GetSilhouette()
{
    return m_pwsSilhouette; 
}

//------------------------------------------------------------------------------
/**
*/
inline CWzShadowVolume* CWzUnitDraw::GetShadowVolume()
{
    return m_pwsvShadowVolume; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetLightMap( HANDLE hLightMap )
{
    m_hLightMap = hLightMap; 
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CWzUnitDraw::GetLightMap() const
{
    return m_hLightMap; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsLightMap() const
{
    return ( m_hLightMap != INVALID_HANDLE_VALUE );
}

//------------------------------------------------------------------------------
/**
*/
inline WzUnitEffectInfo* CWzUnitDraw::GetUnitEffectInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arEffectInfo.GetCount() )
    {
        return m_arEffectInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetNumUnitEffectInfo()
{
    return m_arEffectInfo.GetCount(); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::CreateUnitEffect( BOOL bForce )
{
    // 강제 또는 아직 생성되지 않은 경우만
    if( bForce || !m_stateFlag.Check( UDS_EFFECT_CREATED ) )
    {
        m_stateFlag.Set( UDS_RECREATE_EFFECT );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::OnUnitEffectCreate()
{
    m_stateFlag.Unset( UDS_RECREATE_EFFECT );
    m_stateFlag.Set( UDS_EFFECT_CREATED );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::OnUnitEffectDestroy()
{
    // xxx: 이펙트가 제거됐다고 해서 꼭 재생성해야 하는 것은 아니다.

    m_stateFlag.Unset( UDS_EFFECT_CREATED );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRefractionRate( float fRefractionRate )
{
    m_fRefractionRate = fRefractionRate; 
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetRefractionRate() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->GetRefractionRate();
    }

    return m_fRefractionRate; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRefractionAlpha( float fRefractionAlpha )
{
    m_fRefractionAlpha = fRefractionAlpha;
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetRefractionAlpha() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->GetRefractionAlpha();
    }

    return m_fRefractionAlpha; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetInflateFactor( float fInflateFactor )
{
    m_fInflateFactor = fInflateFactor; 
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetInflateFactor() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->GetInflateFactor();
    }

    return m_fInflateFactor; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetFadeFactor( float fFadeFactor )
{
    m_fFadeFactor = fFadeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetFadeFactor() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->GetFadeFactor();
    }

    return m_fFadeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetSpecularIntensity( float fIntensity )
{
    m_fSpecularIntensity = fIntensity; 
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetSpecularIntensity() const
{
    return m_fSpecularIntensity; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetExternalTexture( int iMeshGroup, HANDLE hTexture, int index )
{
    WzAssert( index >= 0 && index < WZU_NUM_EXTERNAL_TEXTURE );

    if( index >= 0 && index < WZU_NUM_EXTERNAL_TEXTURE )
    {
        m_ExtTextureInfo[index].m_iMeshGroup = iMeshGroup;
        m_ExtTextureInfo[index].m_hTexture = hTexture;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const St_ExternalTextureInfo* CWzUnitDraw::GetExternalTexture( int index ) const
{
    if( index >= 0 && index < WZU_NUM_EXTERNAL_TEXTURE )
    {
        return &m_ExtTextureInfo[index]; 
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableUpdateLight( BOOL bEnable )
{
    // 사용 안 함
}

//------------------------------------------------------------------------------
/**
*/
inline const int* CWzUnitDraw::GetAreaLightPtr() const
{
    return m_refUnitSkeleton->GetAreaLightPtr();
}

#endif // _3DFILEACCESS_WZUNITDRAW_INL_