#include "SunClientPrecompiledHeader.h"
#include "ObjectData.h"
#include "AppearanceManager.h"
#include <Client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>
#include "OptimizationManager.h"
#include "GlobalData.h"



ObjectData::ObjectData()
{
    m_pPathExplorer = NULL;
    m_ppRenderPart = NULL;
    m_pRenderPartVariation = NULL;

    m_BasePart = 0;
    m_MaxPartNum = 0;

    m_fAngleZ = 0.0f;
    m_fAngleX = 0.0f;

    m_wvVisiblePos.x = 0.0f;
    m_wvVisiblePos.y = 0.0f;
    m_wvVisiblePos.z = 0.0f;

    m_bCustomLight = FALSE;
    m_bShadowDraw = TRUE;
    m_bHideAttachment = FALSE;

    m_renderType = AR_NORMAL;
    m_oldRenderType = AR_NORMAL;
    m_wzColor = 0xFFFFFFFF;

    m_bResourceForceRemove = false;
}

void ObjectData::Initialize()
{
    // 반드시 처음 호출되어야함
    assert( m_pPathExplorer == NULL );	
    assert( m_ppRenderPart == NULL );
    assert( m_pRenderPartVariation == NULL );

	m_pPathExplorer = ( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );

#ifdef _PERF_DEBUG
	m_pPathExplorer->SetSHCStatus(TRUE);
#endif
}

void ObjectData::Release()
{
	ReleaseRender();

	if(m_pPathExplorer)
	{
		//	길찾기 기능 파괴
		Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, m_pPathExplorer );
		m_pPathExplorer = NULL;
	}

	//ReleaseRender();
}

void ObjectData::InitializeRender( size_t MaxPartNum, size_t BasePart,  APPEARANCEINFO* pAppearInfo, size_t iAttachmentCount)
{


	ReleaseRender();

	m_MaxPartNum = MaxPartNum;
	m_BasePart = BasePart;
	//	포인터 초기화
	m_ppRenderPart = new CWzUnitDraw*[m_MaxPartNum];
	m_pRenderPartVariation = new RENDER_PART_OPTION[m_MaxPartNum];
	
	for ( size_t i = 0; i < m_MaxPartNum; ++i)
	{
		m_ppRenderPart[i] = NULL;
		m_pRenderPartVariation[i].Initialize();
	}

	// init Attachment
	for ( size_t i=0;i<iAttachmentCount;++i)
	{
		m_AttachmentArray.push_back( ObjectAttachment() );
	}


	//if (pAppearInfo) 
	//{
	//    pAppearInfo->texture_rate = g_OptimizationManager.GetTextureRate(this);
	//	APPEARANCEMGR->SetAppearance( pAppearInfo, this, TRUE, bMakeBoneOnly);
	//}

	//RefreshAniRefDraw();	
}

void ObjectData::ReleaseRender()
{
	SAFE_DELETE_ARRAY( m_ppRenderPart );
	m_MaxPartNum = 0;

	// Release all attachment
	for( size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		m_AttachmentArray[i].ReleaseAttachment();
	}
	m_AttachmentArray.clear();


	if (m_pRenderPartVariation)
	{
		SAFE_DELETE_ARRAY(m_pRenderPartVariation);
	}
}


////------------------------------------------------------------------------------
///**
//	에니메이션 정보 세팅
//*/
//void ObjectData::RefreshRender( APPEARANCEINFO* pAppearInfo, BOOL bRefreshBone)
//{
//	assert(pAppearInfo);
//
//	WzVector pos = GetRenderPart()->GetPosition();
//	WzVector dir = GetRenderPart()->GetRotation();
//	WzID ani = GetRenderPart()->GetCurrentAnimation();
//	float frame = GetRenderPart()->GetCurrentFrame();
//	//	각 파트 파괴
//	if (bRefreshBone)
//	{
//		if (m_ppRenderPart[m_BasePart]) 
//		{
//			ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[m_BasePart]);
//			m_ppRenderPart[m_BasePart] = NULL;			
//		}
//	}
//	
//    pAppearInfo->texture_rate = g_OptimizationManager.GetTextureRate(this);
//	APPEARANCEMGR->SetAppearance(pAppearInfo,this,bRefreshBone,FALSE);
//	RefreshAniRefDraw();
//
//	if (GetRenderPart()) 
//	{
//		GetRenderPart()->SetPosition(pos,FALSE); // 바운딩 볼륨을 쓸데없이 두번 업뎃하지않는다.
//		GetRenderPart()->SetRotation(dir.x,dir.y,dir.z,TRUE);
//	}
//
//	if (bRefreshBone)//에니메이션중에 에니메이션을 바꿀경우 무한 루핑에 빠진다.
//	{
//		GetRenderPart()->SetCurrentAnimation(ani);
//		GetRenderPart()->SetFrame(frame);
//		m_bAnimated = FALSE;
//	}
//
//	EnableCustomLight(m_bCustomLight);
//	SetShadowDraw(m_bShadowDraw);
//}

BOOL ObjectData::RenderBasicParts( St_RenderCommand* pCommand )
{
	if( m_ppRenderPart[0] == NULL )
		return FALSE;

	WzColor OldColor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetColor(m_wzColor);

	BOOL bResult = TRUE;
	if( g_pSunRenderer->RenderUnit( m_ppRenderPart[0] ) == FALSE )
	{
		////COMMENT:로그
		//WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "RenderBasicParts::RenderOutline - %s(%d)",  __FILE__, __LINE__ );

		bResult = FALSE;
	}

	g_pSunRenderer->SetColor(OldColor);

	return bResult;
}

BOOL ObjectData::RenderParts( St_RenderCommand* pCommand, int PartIndex )
{
    if( m_ppRenderPart[PartIndex]) 
    {
        if( pCommand != NULL )
        {
            // Command의 설정으로 직접 랜더링 할 경우 
            if( g_pSunRenderer->RenderUnit( m_ppRenderPart[PartIndex], pCommand ) == FALSE )
            {
                return FALSE;
            }
        }
        else
        {
            // 나머지 경우에는 파츠의 바리에션을 적용
            if (m_pRenderPartVariation[PartIndex].bSpecularVariation)
            {
                m_ppRenderPart[PartIndex]->SetSpecularIntensity(g_fSpecularVariation);
            }


            if (m_pRenderPartVariation[PartIndex].bCustomDiffuse)
            {
                // 디퓨즈가 적용되어 있는 경우
                WzColor newcolor = WzColor_Multiply(m_wzColor,m_pRenderPartVariation[PartIndex].CustomDiffuseColor);
                g_pSunRenderer->SetColor(newcolor);

                if( g_pSunRenderer->RenderUnit( m_ppRenderPart[PartIndex], pCommand ) == FALSE )
                {
                    g_pSunRenderer->SetColor(m_wzColor);

                    return FALSE;
                }

                g_pSunRenderer->SetColor(m_wzColor);
            }
            else
            {
                if( g_pSunRenderer->RenderUnit( m_ppRenderPart[PartIndex], pCommand ) == FALSE )
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

BOOL ObjectData::RenderParts( St_RenderCommand* pCommand )
{
	// 아름다운 코드가 로그땜시 망가지네..


	WzColor OldColor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetColor(m_wzColor);

	
	BOOL bResult = TRUE;

	if( m_MaxPartNum >= 2) 
	{
		for( size_t i = 1 ; i < m_MaxPartNum ; ++i )
		{
            if( RenderParts( pCommand, i ) == FALSE )
            {
                bResult = FALSE;
            }
		}
	}
	else
	{
		bResult = g_pSunRenderer->RenderUnit( m_ppRenderPart[0], pCommand );
	}

	RenderAttachment();

	g_pSunRenderer->SetColor(OldColor);

	return FALSE;
}


BOOL ObjectData::RenderPartsForCostume()
{
    WzColor OldColor = g_pSunRenderer->GetDiffuseColor();
    g_pSunRenderer->SetColor(m_wzColor);

    BOOL bResult = TRUE;

    if( m_MaxPartNum >= 2) 
    {
        RenderParts( NULL, 1 );
        RenderParts( NULL, 2 );
        RenderParts( NULL, 4 );
        RenderParts( NULL, 8 );
        RenderParts( NULL, 9 );
        RenderParts( NULL, 10 );
        RenderParts( NULL, 11 );
    }
    else
    {
        bResult = g_pSunRenderer->RenderUnit( m_ppRenderPart[0], NULL );
    }

    RenderAttachment();

    g_pSunRenderer->SetColor(OldColor);

    return FALSE;
}


BOOL ObjectData::RenderOutline( const WzColor& Color, float fThickness )
{

	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetColor(Color);

	BOOL  bZWrite = g_pSunRenderer->IsEnableZWrite();
	ENUM_CULL_FACE eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	ENUM_ALPHABLEND eBlend = g_pSunRenderer->GetAlphaBlendMode();


	StartRenderType(AR_INFLATION);
	SetInflationRange(fThickness);	


    //! 2011.5.18 / i4u4me / 캐릭터 선택화면에서 외형선이 안나와서 수치 약간 조절 
	float fDepthBias = 0.01f;   //0.0172f;
	float fSlopeBias = 2.0f;
	g_pSunRenderer->SetDepthBias(&fDepthBias,&fSlopeBias);


	g_pSunRenderer->SetAlphaBlendMode(AB_LIGHTING,FALSE);
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_BACK );
	g_pSunRenderer->EnableTexture(FALSE);

	CWzUnitDraw** pParts = GetRenderParts();
	int nPartCount = GetPartCount();


	BOOL bResult = TRUE;

	if( nPartCount > 1 ) 
	{
		for( int i = 1 ; i < nPartCount ; ++i )
		{
			if ( pParts[i] != NULL )
			{	
#if WZENGINEVER >= 380
                if (g_pSunRenderer->RenderUnitForOutline(pParts[i]) == FALSE)
#else
				if (g_pSunRenderer->RenderUnit(pParts[i]) == FALSE)
#endif //#if WZENGINEVER >= 380
                {
					////COMMENT:로그
					//WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "ObjectData::RenderOutline - %s(%d)",  __FILE__, __LINE__ );

					bResult = FALSE;
				}
			}
		}
	}
	else
	{
		if ( pParts[0] != NULL ) 
		{
#if WZENGINEVER >= 380
            if (g_pSunRenderer->RenderUnitForOutline(pParts[0]) == FALSE)
#else
			if (g_pSunRenderer->RenderUnit( pParts[0] ) == FALSE)
#endif //#if WZENGINEVER >= 380
			{
				////COMMENT:로그
				//WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "ObjectData::RenderOutline - %s(%d)",  __FILE__, __LINE__ );

				bResult = FALSE;
			}
		}

	}

	RenderAttachment();

	float fZero = 0.0f;
	g_pSunRenderer->SetDepthBias(&fZero,&fZero);

	g_pSunRenderer->SetZBufferWrite(bZWrite);

	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );

	g_pSunRenderer->SetColor(oldcolor);

	g_pSunRenderer->SetAlphaBlendMode(eBlend,FALSE);

	EndRenderType();

	g_pSunRenderer->EnableTexture(TRUE);

	return bResult;
}

BOOL ObjectData::RenderOutlineForCostume( const WzColor& Color, float fThickness )
{

    WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
    g_pSunRenderer->SetColor(Color);

    BOOL  bZWrite = g_pSunRenderer->IsEnableZWrite();
    ENUM_CULL_FACE eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
    ENUM_ALPHABLEND eBlend = g_pSunRenderer->GetAlphaBlendMode();


    StartRenderType(AR_INFLATION);
    SetInflationRange(fThickness);	


    //! 2011.5.18 / i4u4me / 캐릭터 선택화면에서 외형선이 안나와서 수치 약간 조절 
    float fDepthBias = 0.01f;   //0.0172f;
    float fSlopeBias = 2.0f;
    g_pSunRenderer->SetDepthBias(&fDepthBias,&fSlopeBias);


    g_pSunRenderer->SetAlphaBlendMode(AB_LIGHTING,FALSE);
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_BACK );
    g_pSunRenderer->EnableTexture(FALSE);

    CWzUnitDraw** pParts = GetRenderParts();
    int nPartCount = GetPartCount();


    BOOL bResult = TRUE;


    if (nPartCount > 1) 
    {
        for (int i = 1; i < nPartCount; ++i)
        {
            //////////////////////////////////////////////////////////////////////////
            // 이부분만 기존 아웃라인과 다름 
            //////////////////////////////////////////////////////////////////////////
            if ((i==3) || (i==5) || (i==6) || (i==7))
            {
                continue;
            }
            //////////////////////////////////////////////////////////////////////////

            if (pParts[i] != NULL)
            {	
#if WZENGINEVER >= 380
                if (g_pSunRenderer->RenderUnitForOutline(pParts[i]) == FALSE)
#else
                if (g_pSunRenderer->RenderUnit(pParts[i]) == FALSE)
#endif //#if WZENGINEVER >= 380
                {
                    ////COMMENT:로그
                    //WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "ObjectData::RenderOutline - %s(%d)",  __FILE__, __LINE__ );

                    bResult = FALSE;
                }
            }
        }
    }
    else
    {
        if (pParts[0] != NULL) 
        {
#if WZENGINEVER >= 380
            if (g_pSunRenderer->RenderUnitForOutline(pParts[0]) == FALSE)
#else
            if (g_pSunRenderer->RenderUnit( pParts[0] ) == FALSE)
#endif //#if WZENGINEVER >= 380
            {
                ////COMMENT:로그
                //WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "ObjectData::RenderOutline - %s(%d)",  __FILE__, __LINE__ );

                bResult = FALSE;
            }
        }
    }

    RenderAttachment();

    float fZero = 0.0f;
    g_pSunRenderer->SetDepthBias(&fZero,&fZero);

    g_pSunRenderer->SetZBufferWrite(bZWrite);

    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );

    g_pSunRenderer->SetColor(oldcolor);

    g_pSunRenderer->SetAlphaBlendMode(eBlend,FALSE);

    EndRenderType();

    g_pSunRenderer->EnableTexture(TRUE);

    return bResult;
}

void ObjectData::RenderAttachment()
{
	if ( m_bHideAttachment == TRUE ) 
		return;

	for( size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		m_AttachmentArray[i].Render();
	}
}

void ObjectData::RefreshAniRefDraw()
{	
	if (m_ppRenderPart[m_BasePart]) 
    {
		//	에니메이션 공유
		for( size_t i = 0 ; i < m_MaxPartNum; ++i )
		{
			if (i != m_BasePart) 
            {
				if ( m_ppRenderPart[i]) 
                {
					m_ppRenderPart[i]->SetAniRefUnitDraw( m_ppRenderPart[m_BasePart] );
				}
			}
		}
	}
}

void ObjectData::SetRefractionRate( float fRefractionRate)
{
    for( size_t i = 0 ; i < m_MaxPartNum ; ++i )
    {
        if (m_ppRenderPart[i])
        {	
            m_ppRenderPart[i]->SetRefractionRate( fRefractionRate);
        }
    }

	for (size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetRefractionRate(fRefractionRate);
		}
	}
}

void ObjectData::SetRefractionAlpha( float fRefractionAlpha)
{
    for( size_t i = 0 ; i < m_MaxPartNum ; ++i )
    {
        if (m_ppRenderPart[i])
        {	
            m_ppRenderPart[i]->SetRefractionAlpha(fRefractionAlpha);
        }
    }

	for (size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetRefractionAlpha(fRefractionAlpha);
		}
	}
}

void ObjectData::SetInflationRange(float fRange)
{
	 for( size_t i = 0 ; i < m_MaxPartNum ; ++i )
    {
        if (m_ppRenderPart[i])
        {	
			m_ppRenderPart[i]->SetInflateFactor(fRange);
        }
    }

	for (size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetInflateFactor(fRange);
		}
	}
}

void ObjectData::StartRenderType( ENUM_ALTERNATIVE_RENDER eRenderType )
{
    m_oldRenderType = m_renderType;

    //유닛에 설정을 해준다.
    for( size_t i = 0; i < m_MaxPartNum ; ++i )
    {
        if (m_ppRenderPart[i])
        {	
            m_ppRenderPart[i]->SetRenderType(eRenderType);
        }
    }

	for (size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetRenderType(eRenderType);
		}
	}

    m_renderType = eRenderType;
}

void ObjectData::EndRenderType()
{
    for( size_t i = 0 ; i < m_MaxPartNum ; ++i )
    {
        if (m_ppRenderPart[i])
        {	
            m_ppRenderPart[i]->SetRenderType(m_oldRenderType);
        }
    }

	for (size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetRenderType(m_oldRenderType);
		}
	}

	m_renderType = m_oldRenderType;
}


void ObjectData::SetRenderPartOption( size_t index, const RENDER_PART_OPTION& Option)
{
	if (index < 0 || index >= m_MaxPartNum)
	{
		return;
	}

	m_pRenderPartVariation[index] = Option;

}


void ObjectData::SetShadowDraw(BOOL bDraw)
{
	m_bShadowDraw = bDraw;
	
	for( size_t a = 0 ; a < m_MaxPartNum; ++a )
    {        
		if ( m_ppRenderPart[a]) {
			m_ppRenderPart[a]->EnableShadowCast(bDraw);
			m_ppRenderPart[a]->EnableShadowReceive(bDraw);                
        }
        
    }

	for ( size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if ( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->EnableShadowCast( bDraw );
			pDraw->EnableShadowReceive( bDraw );
		}
	}
}


void ObjectData::EnableCustomLight(BOOL bFlag)
{	

	m_bCustomLight = bFlag;

	for( size_t a = 0 ; a < m_MaxPartNum; ++a )
	{
		if ( m_ppRenderPart[a]) 
		{
			m_ppRenderPart[a]->EnableCustomLight(bFlag);
		}

	}

	for ( size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if ( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->EnableCustomLight( bFlag );
			pDraw->EnableShadowReceive( bFlag );
		}
	}
}

void ObjectData::DetachLightmap()
{
	// DetachLightmap 은 MapObject인 경우에만 유효, Lightmap을 Attach하는건 Map::AttachWlu에서... -_-

	if(m_MaxPartNum == 0)
		return;

	//	각 파트 라이트맵 제거
	for( size_t i = 0 ; i < m_MaxPartNum ; ++i )
	{
		if (m_ppRenderPart[i]) 
		{
			g_pSunRenderer->x_pManagerUnitLightMap->DetachLightMap(m_ppRenderPart[i]);										  
		}
	}
}

void ObjectData::SetSubTypeEffectShow(BOOL bShow)
{
	//딸린 이펙트 들을 감춘다.

	for( size_t i=0;i!=m_AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = m_AttachmentArray[i].GetUnitDraw() )
		{
			g_pSunRenderer->ShowSubTypeEffect( pDraw ,bShow);				
		}
	}

	if(m_ppRenderPart)
	{
		if (m_MaxPartNum >= 2) 
		{
			for( size_t i = 1 ; i < m_MaxPartNum ; ++i )
			{
				if (m_ppRenderPart[i]) 
				{	
					g_pSunRenderer->ShowSubTypeEffect( m_ppRenderPart[i] ,bShow);
				}
			}
		}
		else if(m_ppRenderPart[0])
		{
			g_pSunRenderer->ShowSubTypeEffect( m_ppRenderPart[0] ,bShow);
		}
	}
}

void ObjectData::SetColor(WzColor wzColor)
{
	if(Alpha_WzColor(m_wzColor)==0.0f&&Alpha_WzColor(wzColor)>0.0f)
	{
		SetSubTypeEffectShow(TRUE);
	}
	else if(Alpha_WzColor(m_wzColor)>0.0f&&Alpha_WzColor(wzColor)==0.0f)
	{
		SetSubTypeEffectShow(FALSE);
	}

	m_wzColor = wzColor;
}

void ObjectData::SetColor(int r,int g,int b,int a)
{

	if(Alpha_WzColor(m_wzColor)==0.0f&&a>0)
	{
		SetSubTypeEffectShow(TRUE);
	}
	else if(Alpha_WzColor(m_wzColor)>0.0f&&a==0)
	{
		SetSubTypeEffectShow(FALSE);
	}


	m_wzColor = WzColor_RGBA(r,g,b,a);
}

BOOL ObjectData::IsCompletelyLoaded()
{
	for (size_t i = 0; i < m_MaxPartNum; i++)
	{
		if (m_ppRenderPart[i])
		{
			if (g_pSunRenderer->CheckUnitLoaded(m_ppRenderPart[i]) == FALSE)
			{
				return FALSE;

			}			
		}
	}
	return TRUE;
}

BOOL ObjectData::IsCompletelyFullLoaded()
{
    for (size_t i = 0; i < m_MaxPartNum; i++)
    {
        if (m_ppRenderPart[i])
        {
            CWzUnit* pUnit = m_ppRenderPart[i]->GetWzUnit();
            if ((pUnit == NULL) || 
                (pUnit->GetLoadState() != eLOAD_FULL))
            {
                return FALSE;
            }			
        }
    }
    return TRUE;
}


WzBoundingVolume* ObjectData::GetBoundingVolume()
{
	if( CWzUnitDraw* pBasePart = GetBasePart() )
	{
		return pBasePart->GetBoundingVolumePtr();
	}
	return NULL;
}

size_t ObjectData::GetNumPickingBoundingVolume()
{
	if (CWzUnitDraw* pBasePart = GetBasePart())
	{
        if (pBasePart->GetWzUnit() != NULL)
        {
		    return (size_t)pBasePart->GetWzUnit()->GetCountBVExtraInfo();
        }
	}

	return 0;
}

WzBoundingVolume* ObjectData::GetPickingBoundingVolume( size_t index)
{
	if (CWzUnitDraw* pBasePart = GetBasePart())
	{
        if (pBasePart->GetWzUnit() != NULL)
        {
		    if ( index >= (size_t)pBasePart->GetWzUnit()->GetCountBVExtraInfo())
		    {
			    return NULL;
		    }
		    else
		    {
			    if( WzBVExtraInfo *pInfo = pBasePart->GetBoundingVolumeExtraPtrByIndex(index) )
			    {
				    return &(pInfo->m_BV);
			    }
		    }
        }
	}

	return NULL;
}


void ObjectData::SetAttachmentAnimation(size_t nIndex, WzID animID,BOOL bLoop)
{
	if( nIndex >= m_AttachmentArray.size() )
		return;

	m_AttachmentArray[nIndex].SetAnimation( animID, bLoop );
}

void ObjectData::ApplyInterpolateAccesory()
{
	if( CWzUnitDraw* pBasePart = GetBasePart() )
	{
		for( size_t i=0;i!=m_AttachmentArray.size();++i)
		{
			m_AttachmentArray[i].ApplyInterpolateAccesory( pBasePart );
		}
	}
}

BOOL ObjectData::ProcessInterpolationAttachment( DWORD dwTick )
{
	SUNPROCESSPROFILE_BEGIN(_T("ProcessInterpolationAttachment"));

	if( CWzUnitDraw* pBasePart = GetBasePart() )
	{
		for( size_t i=0;i!=m_AttachmentArray.size();++i)
		{
			m_AttachmentArray[i].ProcessInterpolation( pBasePart, dwTick );
		}
	}

	SUNPROCESSPROFILE_END(_T("ProcessInterpolationAttachment"));

	return TRUE;
}



//------------------------------------------------------------------------------
void ObjectData::SetScale(float fScale)
{
	CWzUnitDraw** pParts = GetRenderParts();
	for( int i=0;i!= GetPartCount(); ++i )
	{
		if( pParts[i] != NULL )
		{
			pParts[i]->SetScale( fScale, fScale, fScale );
		}
	}

	const std::vector<ObjectAttachment>& AttachmentArray = GetAttachmentArray();
	for( size_t i=0;i!=AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = AttachmentArray[i].GetUnitDraw() )
		{
            float attachment_scale = AttachmentArray[i].GetAttachmentInfo().GetScale();
            pDraw->SetScale(attachment_scale);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void  ObjectData::GetScale( WzVector* pScale )
{
	if ( CWzUnitDraw* pDraw = GetBasePart() )
	{
		pDraw->GetScale(pScale);
	}
}

//------------------------------------------------------------------------------
/**
*/
WzVector ObjectData::GetScale()
{
	WzVector vScale;
	SetVector(&vScale,1.0f,1.0f,1.0f);

	if ( CWzUnitDraw* pDraw = GetBasePart() )
	{
		pDraw->GetScale(&vScale);
	}

	return vScale;
}


//------------------------------------------------------------------------------
/**
*/
void ObjectData::SetScale(const WzVector& vScale)
{
	CWzUnitDraw** pParts = GetRenderParts();
	for( int i=0;i!= GetPartCount(); ++i )
	{
		if( pParts[i] != NULL )
		{
			pParts[i]->SetScale( vScale.x, vScale.y, vScale.z );
		}
	}

	const std::vector<ObjectAttachment>& AttachmentArray = GetAttachmentArray();
	for( size_t i=0;i!=AttachmentArray.size();++i)
	{
		if( CWzUnitDraw* pDraw = AttachmentArray[i].GetUnitDraw() )
		{
			pDraw->SetScale( vScale.x, vScale.y, vScale.z );
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
BOOL ObjectData::SetPosition( const WzVector& wvPos ,int iTileIndex)
{
    if( g_pSunTerrain == NULL )
        return FALSE;

    return g_pSunTerrain->PathExp_Reset( m_pPathExplorer, wvPos ,iTileIndex);
}

//------------------------------------------------------------------------------
BOOL ObjectData::SetPosition( const HONGPOS& pPos)
{
	return SetPosition( pPos.wvPos, pPos.wTile);
}
//------------------------------------------------------------------------------
void ObjectData::SetDirection( const WzVector& TargetPosition, BOOL bVisibleAngle /*= FALSE*/ )
{
	if( m_pPathExplorer == NULL )
		return;

	SetDirection( TargetPosition, m_pPathExplorer->GetPos(), bVisibleAngle );
}

//------------------------------------------------------------------------------
void ObjectData::SetDirection( const WzVector& TargetPosition, const WzVector& SrcPosition, BOOL bVisibleAngle /*= FALSE*/ )
{
	WzVector vDirection = TargetPosition - SrcPosition;
	vDirection.z = 0.0f;

	if( VectorLength2( &vDirection ) == 0.0f )
	{
		//똑같은 위치는 무시 한다.
		return;
	}

	m_fAngleZ = Math_GetAngleFromVector( &vDirection );
	SetAngle( m_fAngleZ, bVisibleAngle);
}

//------------------------------------------------------------------------------
WzVector ObjectData::GetDirection()
{
	WzVector vDirection;
	vDirection.x = sin(m_fAngleZ);
	vDirection.y = -cos(m_fAngleZ);
	vDirection.z = 0.0f;

	return vDirection;
}

//------------------------------------------------------------------------------
WzVector ObjectData::GetPosition( void )
{
	assert(m_pPathExplorer);
	
	if(m_pPathExplorer==NULL)
		return m_wvVisiblePos;

	return m_pPathExplorer->GetPos();
}


//------------------------------------------------------------------------------
WzVector ObjectData::GetVisiblePos( void )
{
#ifdef _PERF_DEBUG
	return GetPosition();
#else
	return m_wvVisiblePos;
#endif
}

//------------------------------------------------------------------------------
void  ObjectData::SetAngle(float fAngle,BOOL bSetVisibleAngle)
{
	_ClampAngle(&fAngle);
	m_fAngleZ = fAngle;
	if (bSetVisibleAngle) 
    {
		if (GetBasePart()) 
        {
			GetBasePart()->SetRotation( m_fAngleX,0.0f, m_fAngleZ );
		}
	}
}

//------------------------------------------------------------------------------
void ObjectData::SetRandomAngle()
{
	float fAngle = (rand() % 628) / 100.0f;
	SetAngle(fAngle,TRUE); 
}

//------------------------------------------------------------------------------
void ObjectData::SetAngle(float fAngleX,float fAngleY,float fAngleZ)
{
	_ClampAngle(&fAngleZ);
	m_fAngleZ = fAngleZ;
	GetBasePart()->SetRotation( fAngleX, fAngleY, fAngleZ );
}

//------------------------------------------------------------------------------
void ObjectData::Rotate( float fAngle, BOOL bSetVisibleAngle)
{
	m_fAngleZ += fAngle;

	// 0~360도 사이로 유지
	_ClampAngle( &m_fAngleZ );
}

//------------------------------------------------------------------------------
void ObjectData::_ClampAngle(float* pAngle)
{
	float fNewAngle = *pAngle;
	fNewAngle = fmod(fNewAngle,WZ_PI * 2.0f);

	if (fNewAngle < 0.0f)
	{
		fNewAngle += WZ_PI * 2.0f;
	}

	*pAngle = fNewAngle;
}



