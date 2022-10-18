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
    // xxx: ���� ȣȯ, ���� ������ ������ ���� ���� �� ����
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
    // xxx: ī�޶���� �Ÿ��� ���� ��Ű�� ���ΰ�
    // �����Ǳ� ������ ����(���̽�) ������ ���� ��ȯ�Ѵ�.

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
    // xxx: ī�޶���� �Ÿ��� ���� �븻�� ���� ���ΰ�
    // �����Ǳ� ������ ����(���̽�) ������ ���� ��ȯ�Ѵ�.

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
    // xxx: ���� ȣȯ, ���� ������ ������ ���� ���� �� ����
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
    // xxx: ���� ȣȯ, ���� ������ ������ ���� ���� �� ����
    if( m_pRefUnitDraw )
    {
        return m_pRefUnitDraw->IsCustomLight();
    }

    return m_renderOption.Check( UDR_ENABLE_CUSTOM_LIGHT ); 
}

#endif // _3DFILEACCESS_WZUNITDRAW_RENDEROPT_INL_
