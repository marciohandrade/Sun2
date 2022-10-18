//------------------------------------------------------------------------------
//  Clone.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Clone.h"
#include "Character.h"
#include "AppearanceManager.h"
#include "ResourceManager.h"
#include "ObjectManager.h"


//------------------------------------------------------------------------------
/**
*/
Clone::Clone()
{
	m_AlphaMode = AB_LIGHTING;
	m_bMove = FALSE;	
	m_bDisappear = FALSE;
	m_bAppear = FALSE;
	m_dwAppearCount = 0;
	m_dwAppearTime = 0;
	m_bScaleAnim = FALSE;
	m_fCurScale = 1.0f;
    m_bRefraction =FALSE;
	m_IsAni       = FALSE;

    m_dwRefractionCount=0;
    m_dwRefractionTime=0;
    m_byRefractionAlpha=0;

	m_fVibrateForce = 0.0f;

	m_dwAttachObject = 0;
	m_bRestraintAniTick=FALSE;
//	m_bPermanant = FALSE;
}


//------------------------------------------------------------------------------
/**
*/
Clone::~Clone()
{
}

//------------------------------------------------------------------------------
/**
	에니메이션 틱이 특정 수치를 안넘기게 할것인가
	실시간 로딩시 틱이 너무 크게 들어올경우 에니메이션이 생략되는 현상 때문에 
*/
void Clone::SetRestraintAniTick(BOOL bFlag)
{
	m_bRestraintAniTick=bFlag;
}

//------------------------------------------------------------------------------
/**
*/
void Clone::CopyAttachment(Object *pObject)
{
	size_t PartsCount = pObject->GetPartCount();	
	CWzUnitDraw **pSrcParts = pObject->GetParts();
	int AttachmentCount = 0;

	InitRender(PartsCount,0,NULL,pObject->GetAttachmentCount(),FALSE);

    CWzUnitDraw** pDestParts = GetParts();

	if (pSrcParts[BP_BONE] && pSrcParts[BP_BONE]->GetWzUnit())
	{
		RESOURCECODE code = pSrcParts[BP_BONE]->GetWzUnit()->GetType();
		
		if(pObject->IsKindOfObject(PLAYER_OBJECT) == FALSE)
		{
			pDestParts[BP_BONE] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
		}
		else
		{
			pDestParts[BP_BONE] = ResourceManager::Instance()->AllocResourceUnitDraw(code, -1.0f, false);

		}

		pDestParts[BP_BONE]->EnableNormalMap(FALSE);
		pDestParts[BP_BONE]->EnableSkinning(FALSE);

		pDestParts[BP_BONE]->SetPosition( pSrcParts[BP_BONE]->GetPosition(), FALSE); // 쓸데없이 바운딩 볼륨을 두번 업뎃하지않는다.
		pDestParts[BP_BONE]->SetRotation( pSrcParts[BP_BONE]->GetRotation() );
	}
	
	RefreshAniRefDraw();

	// 유효한 첫번째 Attachment만 복사한다.
	for ( size_t i=0;i!=pObject->GetAttachmentCount();++i)
	{
		const AttachmentInfo& Info = pObject->GetAttachmentInfo(i);
		if( Info.GetResourceID() == 0 )
			continue;

		SetAttachment( i, Info );
	}

	//int iAttachmentNum = pObject->GetAttachmentCount();
	//for (int a = 0; a < iAttachmentNum; a++)
	//{
	//	OBJECT_ATTACHMENT *pInfo = pObject->GetAttachmentInfo(a);
	//	if ( !pInfo )
	//	{
	//		continue;
	//	}

	//	if (pInfo->codeResource != 0) 
	//	{
	//		ATTACHMENT_INFO info;
	//		info.codeResource = pInfo->codeResource;
	//		info.iBoneIndex = pInfo->iBoneIndex;
	//		info.iSubType = 0;
	//		info.offsetMatrix = pInfo->offsetMatrix;
 //           info.bUseOffsetMatrix = pInfo->bUseOffsetMatrix;
 //       
	//		SetAttachment(a,&info);
	//		break;
	//	}
	//}

	WzID AnimID = pObject->GetCurrentAnimation();
	float fFrame = pObject->GetCurrentFrame();

	SetAnimation(AnimID);
	SetCurrentFrame(fFrame);

	m_vCurPos = GetBasePart()->GetPosition();

	m_bMove = FALSE;

	SetShadowDraw(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void Clone::CopyAppearanceExceptAni(Object *pObject)
{
	size_t PartsCount = pObject->GetPartCount();	
	CWzUnitDraw** pSrcParts = pObject->GetParts();
	int AttachmentCount = 0;

	InitRender(PartsCount,0,NULL,pObject->GetAttachmentCount(),FALSE);

    CWzUnitDraw** pDestParts = GetParts();
	for ( size_t i = 0; i < PartsCount; i++)
	{
		if (pSrcParts[i] && pSrcParts[i]->GetWzUnit())
		{
			CWzUnitDraw *pDraw = pSrcParts[i];
			RESOURCECODE code = pSrcParts[i]->GetWzUnit()->GetType();
			
			if(pObject->IsKindOfObject(PLAYER_OBJECT) == FALSE)
			{
				pDestParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
			}
			else
			{
				pDestParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code, -1.0f, false);
			}

			pDestParts[i]->EnableNormalMap(FALSE);
			pDestParts[i]->EnableSkinning(FALSE);

			pDestParts[i]->SetPosition(pSrcParts[i]->GetPosition(),FALSE); // 쓸데없이 바운딩 볼륨을 두번 업뎃하지않는다.
			pDestParts[i]->SetRotation(pSrcParts[i]->GetRotation());

			WzVector vScale;
			pSrcParts[i]->GetScale(&vScale);
			pDestParts[i]->SetScale(vScale);

		}
	}
	RefreshAniRefDraw();

	// 유효한 첫번째 Attachment만 복사한다.
	for ( size_t i=0;i!=pObject->GetAttachmentCount();++i)
	{
		const AttachmentInfo& Info = pObject->GetAttachmentInfo(i);
		if( Info.GetResourceID() == 0 )
			continue;

		SetAttachment( i, Info );
	}

	WzID AnimID = pObject->GetCurrentCamAnimation();
	float fFrame = pObject->GetCurrentFrame();

	SetAnimation(AnimID);
	SetCurrentFrame(0.0f);

	m_vCurPos = GetBasePart()->GetPosition();

	m_bMove = FALSE;

	SetShadowDraw(FALSE);

}

//------------------------------------------------------------------------------
/**
*/
void Clone::CopyAppearance(Object *pObject)
{
	int PartsCount = pObject->GetPartCount();	
	CWzUnitDraw** pSrcParts = pObject->GetParts();

	InitRender(PartsCount,0,NULL,pObject->GetAttachmentCount(),FALSE);

    CWzUnitDraw** pDestParts = GetParts();

	for (int i = 0; i < PartsCount; i++)
	{
		if (pSrcParts[i] && pSrcParts[i]->GetWzUnit())
		{
			RESOURCECODE code = pSrcParts[i]->GetWzUnit()->GetType();
			
			if(pObject->IsKindOfObject(PLAYER_OBJECT) == FALSE)
			{
				pDestParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
			}
			else
			{
				pDestParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code, -1.0f, false);
			}

			pDestParts[i]->EnableNormalMap(FALSE);
			pDestParts[i]->EnableSkinning(FALSE);

			pDestParts[i]->SetPosition(pSrcParts[i]->GetPosition(), FALSE); // 쓸데없이 바운딩 볼륨을 두번 업뎃하지않는다.
			pDestParts[i]->SetRotation(pSrcParts[i]->GetRotation());

			WzVector vScale;
			pSrcParts[i]->GetScale(&vScale);
			pDestParts[i]->SetScale(vScale);

		}
	}
	RefreshAniRefDraw();

	// 유효한 첫번째 Attachment만 복사한다.
	for ( size_t i=0;i!=pObject->GetAttachmentCount();++i)
	{
		const AttachmentInfo& Info = pObject->GetAttachmentInfo(i);
		if( Info.GetResourceID() == 0 )
			continue;

		SetAttachment( i, Info );
	}

	for (size_t i = 0; i < pObject->GetAttachmentCount(); i++)
	{
		AttachmentInfo Info = pObject->GetAttachmentInfo(i);

		if( Info.GetResourceID() != 0 )
		{
			SetAttachment( i, Info );
			break;
		}
	}	

	WzID AnimID = pObject->GetCurrentAnimation();
	float fFrame = pObject->GetCurrentFrame();

	SetAnimation(AnimID);
	SetCurrentFrame(fFrame);

	m_vCurPos = GetBasePart()->GetPosition();

	m_bMove = FALSE;

	SetShadowDraw(FALSE);
	SetColor(pObject->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
void Clone::Destroy()
{
	Object::Destroy();
}


//------------------------------------------------------------------------------
/**
*/
BOOL Clone::Render()
{
	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( m_AlphaMode, FALSE );

	if(m_bScaleAnim)
		Object::SetScale(m_fCurScale);
	
	Object::SetColor(m_Color);
    
	Object::Render();

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode, FALSE );

	return TRUE;
}

BOOL Clone::Render(St_RenderCommand *pCommand)
{
	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( m_AlphaMode, FALSE );

	if(m_bScaleAnim)
		Object::SetScale(m_fCurScale);

	Object::SetColor(m_Color);
    
    if (pCommand)
	{
		Object::Render(pCommand);
	}

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode, FALSE );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Clone::SetColor(WzColor color)
{
	m_Color = color;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Clone::Process(DWORD dwTick)
{
    if(m_IsAni)
	{
		if(m_bRestraintAniTick)
			dwTick=min(30,dwTick);

		ProcessAnimation(dwTick);
	}
	else
        ProcessAnimation(0, NULL, TRUE);


	// 붙어다닐 Object자체가 소멸했으면 클론도 소멸

	Object *pAttachObject = NULL;
	if (m_dwAttachObject)
	{
		pAttachObject = g_ObjectManager.GetObject(m_dwAttachObject);
		if (!pAttachObject)
		{
			return TRUE;
		}

	}


	if (m_bMove)
	{
		m_vCurPos = m_vCurPos + (m_vDir * (m_fMoveSpeed * dwTick));

		m_fMoved += ( m_fMoveSpeed * dwTick);

		if (m_fMoved >= m_fDistance)
		{
			m_fMoved = m_fDistance;
			m_vCurPos = m_vEnd;
			m_bMove = FALSE;
		}
	

	}
	
	
	const float fRate = -0.95f;
	const float fEpsilon = 0.00005f;

	if (m_fVibrateForce != 0.0f)
	{		
		// 이동이 끝났으면 반대방향으로 다시이동	
		if (!m_bMove ) 
		{
			m_fVibrateForce *= fRate;

			if (fabs(m_fVibrateForce) <= fEpsilon)
			{
				m_fVibrateForce = 0.0f;
				m_bMove = FALSE;
			}
			else 
			{
				m_vStart = m_vCurPos;
				m_vCurPos = m_vCurPos;
				if (pAttachObject) 
				{
					m_vEnd = m_vVibrateDirection * (m_fVibrateForce);
				}
				else
				{
					m_vEnd = m_vVibStartPos + m_vVibrateDirection * (m_fVibrateForce);
				}
				m_bMove = TRUE;
				m_fMoved = 0.0f;
				WzVector vDir;
				vDir = m_vEnd - m_vStart;
				m_fDistance = VectorLength(&vDir);
				VectorNormalize(&m_vDir,&vDir);
			}
		}

	}


    WzVector vRenderPos;

	// AttachObject가 있을경우에는 Move를 Offset처럼 사용한다.
	if (pAttachObject)
	{

		vRenderPos = pAttachObject->GetVisiblePos();

		if (m_bMove)
		{			
			vRenderPos += m_vCurPos;

		}

	}
	else 
	{
		vRenderPos = m_vCurPos;
	}


	GetBasePart()->SetPosition(vRenderPos);

	if (m_bDisappear)
	{
		m_dwAppearCount += dwTick;

		float fRate = 1.0f - (float)m_dwAppearCount / (float)m_dwAppearTime;
		if (fRate <= 0.0f)
		{
			fRate = 0.0f;
			m_bDisappear = FALSE;
		}

		WzColor color = m_Color;
		BYTE Alpha = (BYTE)(m_byAppearAlpha * fRate);
		m_Color = WzColor_RGBA(Red_WzColor(m_Color),Green_WzColor(m_Color),Blue_WzColor(m_Color),Alpha);
	}

  	if (m_bScaleAnim)
	{
		m_dwScaleCount += dwTick;

		float fRate = (float)m_dwScaleCount / (float)m_dwScaleTime;
		if (fRate >= 1.0f)
		{
			fRate = 1.0f;
			m_bScaleAnim = FALSE;
			m_fCurScale = m_fDestScale;
		}
		else 
		{
			m_fCurScale = m_fStartScale * (1.0f - fRate) + m_fDestScale * fRate;
		}


	}

	if (m_bAppear)
	{
		m_dwAppearCount += dwTick;
		float fRate = (float)m_dwAppearCount / (float)m_dwAppearTime;
		if (fRate >= 1.0f)
		{
			fRate = 1.0f;
			m_bAppear = FALSE;
		}

		WzColor color = m_Color;
		BYTE Alpha = BYTE( FLOAT(m_byAppearStartAlpha  * (1.0 - fRate) + (BYTE)(m_byAppearAlpha * fRate) ) );
		m_Color = WzColor_RGBA(Red_WzColor(m_Color),Green_WzColor(m_Color),Blue_WzColor(m_Color),Alpha);
	}


	//if (m_bPermanant)
	//{
	//	return FALSE;
	//}

	m_dwCurDelay += dwTick;

	if( m_dwCurDelay >= m_dwLifeTime )
	{
		return TRUE;
	}

	return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL Clone::SetPosition( const WzVector& wvPos, int iTileIndex )
{
	GetBasePart()->SetPosition(wvPos);
	return TRUE;	
}


//------------------------------------------------------------------------------
/**
*/
BOOL Clone::SetRotation(float fAngle)
{
	if (GetBasePart())
	{
		GetBasePart()->SetRotation(0.0f,0.0f,fAngle);
	}
	
	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL  Clone::Move(WzVector vCurPos,WzVector vDestPos)
{
	m_vStart = vCurPos;
	m_vCurPos = vCurPos;
	m_vEnd = vDestPos;
	m_bMove = TRUE;
	WzVector vDir;
	vDir = m_vEnd - m_vStart;
	m_fDistance = VectorLength(&vDir);
	VectorNormalize(&m_vDir,&vDir);
	
	m_fMoved = 0.0f;

	Process(0);



	return TRUE;	
}


//------------------------------------------------------------------------------
/**
*/
void Clone::SetAlphaBlend(ENUM_ALPHABLEND alpha)
{
	m_AlphaMode = alpha;
}


//------------------------------------------------------------------------------
/**
*/
void Clone::Init()
{
	m_dwCurDelay = 0;
	m_dwLifeTime = 0;
}


//------------------------------------------------------------------------------
/**
*/
void Clone::ResetCurDelay()
{
	m_dwCurDelay = 0;
}


//------------------------------------------------------------------------------
/**
*/
void Clone::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}


//------------------------------------------------------------------------------
/**
*/
void Clone::SetAppear(DWORD dwTime,BYTE byAppearStartAlpha)
{
	m_dwAppearTime = dwTime;
	m_dwAppearCount = 0;
	m_bAppear = TRUE;
	m_bDisappear = FALSE;
	m_byAppearAlpha = Alpha_WzColor(m_Color);
	m_byAppearStartAlpha = byAppearStartAlpha;
}

//------------------------------------------------------------------------------
/**
*/
void Clone::SetDisappear(DWORD dwTime)
{
	m_dwAppearTime = dwTime;
	m_dwAppearCount = 0;
	m_bAppear = FALSE;
	m_bDisappear = TRUE;
	m_byAppearAlpha = Alpha_WzColor(m_Color);
}

//------------------------------------------------------------------------------
/**
*/
void Clone::SetRefraction(DWORD dwTime)
{
    m_dwAppearTime = dwTime;
    m_dwAppearCount = 0;
    m_bAppear = FALSE;
    m_bDisappear = TRUE;
    m_byAppearAlpha = Alpha_WzColor(m_Color);

    m_bRefraction= TRUE;
	m_dwRefractionCount = 0;
	m_dwRefractionTime= dwTime;
	m_byRefractionAlpha= Alpha_WzColor(m_Color);

}


void Clone::SetScale(float fScale)
{
	m_fCurScale = fScale;
}

void Clone::SetScaleAnimation(DWORD dwTime,float fDest)
{
	m_fDestScale = fDest;
	m_fStartScale = m_fCurScale;
	m_bScaleAnim = TRUE;
	m_dwScaleCount = 0;
	m_dwScaleTime = dwTime;
}

void Clone::Vibrate(WzVector vDirection,float fForce)
{
	m_vVibrateDirection = vDirection;
	m_fVibrateForce = fForce;

	if ( m_dwAttachObject) 
	{
		ZeroMemory(&m_vVibStartPos,sizeof(m_vVibStartPos));
	}
	else 
	{
		m_vVibStartPos = m_vCurPos;
	}

	m_vStart = m_vVibStartPos;
	m_vCurPos = m_vVibStartPos;


	m_vEnd = m_vVibStartPos + m_vVibrateDirection * (m_fVibrateForce);

	m_bMove = TRUE;
	m_fMoved = 0.0f;
	WzVector vDir;
	vDir = m_vEnd - m_vStart;
	m_fDistance = VectorLength(&vDir);
	VectorNormalize(&m_vDir,&vDir);
}

void Clone::AttachToObject(DWORD dwObjectKey)
{
	m_dwAttachObject = dwObjectKey;
}

//void Clone::SetPermanant(BOOL bFlag)
//{
//	m_bPermanant = bFlag;
//}

void Clone::SetCurPos(WzVector vPos)
{
	m_vCurPos = vPos;
}