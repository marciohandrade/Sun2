// wzunitdraw_renderopt.inl

#ifndef _3DFILEACCESS_WZUNITDRAW_RENDEROPT_INL_
#define _3DFILEACCESS_WZUNITDRAW_RENDEROPT_INL_

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType )
{
    m_eRenderType = eRenderType;
}

//------------------------------------------------------------------------------
/**
*/
inline ENUM_ALTERNATIVE_RENDER CWzUnitDraw::GetRenderType() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->GetRenderType();
    }

    return m_eRenderType;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableSkinning( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_SKINNING, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsSkinning() const
{
    // xxx: 카메라와의 거리에 따라 스키닝 여부가
    // 결정되기 때문에 참조(베이스) 유닛의 값을 반환한다.

    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->IsSkinning();
    }

    return m_renderOption.Check( UDR_ENABLE_SKINNING ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableNormalMap( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_NORMALMAP, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsNormalMap() const
{
    // xxx: 카메라와의 거리에 따라 노말맵 적용 여부가
    // 결정되기 때문에 참조(베이스) 유닛의 값을 반환한다.

    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->IsNormalMap();
    }

    return m_renderOption.Check( UDR_ENABLE_NORMALMAP ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableNormalMapRefraction( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_NORMALMAP_REFRACTION, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsNormalMapRefraction() const
{
    return m_renderOption.Check( UDR_ENABLE_NORMALMAP_REFRACTION ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableAlphaBlendOfSubType( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_SUBTYPE_ALPHABLEND, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsAlphaBlendOfSubType() const
{
    return m_renderOption.Check( UDR_ENABLE_SUBTYPE_ALPHABLEND ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableCullFaceModeIn( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_CULLFACEMODE_IN, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsCullFaceModeIn() const
{
    return m_renderOption.Check( UDR_ENABLE_CULLFACEMODE_IN ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableShadowOnSunLight( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_SHADOW_ON_SUNLIGHT, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsShadowOnSunLight() const
{
    return m_renderOption.Check( UDR_ENABLE_SHADOW_ON_SUNLIGHT ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableColorOfSubType( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_SUBTYPE_COLOR, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsColorOfSubType() const
{
    return m_renderOption.Check( UDR_ENABLE_SUBTYPE_COLOR ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableExternalTexture( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_EXTERNAL_TEXTURE, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsExternalTexture() const
{
    return m_renderOption.Check( UDR_ENABLE_EXTERNAL_TEXTURE ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableLastAlphaRender( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_LASTALPHA_RENDER, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsLastAlphaRender() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->IsLastAlphaRender();
    }

    return m_renderOption.Check( UDR_ENABLE_LASTALPHA_RENDER ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::EnableCustomLight( BOOL bEnable )
{
    m_renderOption.SetB( UDR_ENABLE_CUSTOM_LIGHT, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsCustomLight() const
{
    // xxx: 기존 호환, 참조 유닛이 있으면 참조 유닛 값 리턴
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->IsCustomLight();
    }

    return m_renderOption.Check( UDR_ENABLE_CUSTOM_LIGHT ); 
}

#endif // _3DFILEACCESS_WZUNITDRAW_RENDEROPT_INL_
