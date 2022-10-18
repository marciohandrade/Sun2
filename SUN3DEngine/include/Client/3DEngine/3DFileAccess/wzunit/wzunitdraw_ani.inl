// wzunitdraw_ani.inl

#ifndef _3DFILEACCESS_WZUNITDRAW_ANI_INL_
#define _3DFILEACCESS_WZUNITDRAW_ANI_INL_

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetCurrentAnimation( WzID wiAni, 
                                              BOOL bLoop, 
                                              BOOL bRestart, 
                                              int nInterpolationFrame )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetCurrentAnimation() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->SetCurAnimation( wiAni, 
                                        bLoop, 
                                        bRestart, 
                                        nInterpolationFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline WzID CWzUnitDraw::GetCurrentAnimation()
{
    return m_refUnitSkeleton->GetCurAnimationID();
}

//------------------------------------------------------------------------------
/**
*/
inline CWzAniState* CWzUnitDraw::GetCurAniState()
{
    return m_refUnitSkeleton->GetCurAniState();
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzUnitDraw::GetCurrentAnimationInfo()
{
    return m_refUnitSkeleton->GetCurAnimationInfo();
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzUnitDraw::GetPrevAnimationInfo()
{
    return m_refUnitSkeleton->GetPrevAnimationInfo();
}

//------------------------------------------------------------------------------
/**
    xxx: ���� ȣȯ (���� ����)
*/
inline void CWzUnitDraw::ClearNextAnimation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetCurrentBlendAnimation( WzID wiAni, 
                                                   int nBoneIdx, 
                                                   BOOL bLoop, 
                                                   float fSpeedRatio,
                                                   BOOL bDeltaType, 
                                                   float /*fBlendWeight*/ )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetCurrentBlendAnimation() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->SetCurBlendAnimation( wiAni, 
                                             nBoneIdx, 
                                             bLoop,
                                             fSpeedRatio,
                                             bDeltaType );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::UnSetBlendAnimation()
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::UnSetBlendAnimation() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->UnsetBlendAnimation();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::UnLoopBlendAnimation()
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::UnLoopBlendAnimation() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->UnloopBlendAnimation();
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetBlendAniCount()
{
    return m_refUnitSkeleton->GetNumBlendAnimations();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetFrame( float fFrame )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetFrame() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->SetCurFrame( fFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetFrame()
{
    return m_refUnitSkeleton->GetCurFrame();
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnitDraw::GetCurrentFrame()
{
    return GetFrame();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetInterpolationFrame( int nInterpolationFrame )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetInterpolationFrame() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->SetIpolFrame( nInterpolationFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitDraw::SetBlendInterpolationFrame( int nStartFrame, int nEndFrame )
{
    if( m_pRefUnitDraw )
    {
        WZLOG( WZWAR, "CWzUnitDraw::SetBlendInterpolationFrame() - �߸� ȣ���!!" );
        return;
    }

    m_refUnitSkeleton->SetBlendIpolFrame( nStartFrame, nEndFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitDraw::IsInterpolating()
{
    return m_refUnitSkeleton->IsInterpolating();
}

//------------------------------------------------------------------------------
/**
*/
inline WzTransform* CWzUnitDraw::GetTransform()
{
    return m_refUnitSkeleton->GetTransformBuf();
}

//------------------------------------------------------------------------------
/**
*/
inline WzTransform* CWzUnitDraw::GetLastSavedTransform()
{
    return m_refUnitSkeleton->GetLastTransformBuf();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* CWzUnitDraw::GetTransformBuffer( int iSize )
{
    return m_refUnitSkeleton->GetTransformMatrixBuf( iSize );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* CWzUnitDraw::GetInverseAndTransformBuffer( int iSize )
{
    return m_refUnitSkeleton->GetSkinMatrixBuf( iSize );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitDraw::GetTransformBufferSize()
{
    return m_refUnitSkeleton->GetTransformBufSize();
}

#endif // _3DFILEACCESS_WZUNITDRAW_ANI_INL_