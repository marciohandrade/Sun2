/**
 *	@file		Objcet.cpp
 *	@brief		Object class
 *  @author		Unknown
 *	@version	Unknown
 */

//------------------------------------------------------------------------------
//  Object.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "Object.h"
#include "Character.h"
#include "AppearanceManager.h"
#include "ResourceManager.h"
#include "SoundEffect.h"
#include "Hero.h"
#include "GameFunc.h"

#include "MapResMgr.h"
#include "interfacemanager.h"
#include "GlobalFunc.h"
#include "Camera.h"
#include "OptimizationManager.h"
#include "HeroActionInput.h"
#include "Map.h"
#include "GameFrameWork.h"
#include "SceneBase.h"
#include "EventInfo.h"



#pragma warning ( push )
#pragma warning(disable : 4800)


//------------------------------------------------------------------------------
/**
*/
Object::Object( void ) :
m_bUseDelayLoading(false),
m_bVisible(TRUE),
m_bEnableDisplay(FALSE),
m_pPathExplorer(NULL)
{


	//for (int a= 0; a < MAX_TRAIL; a++) 
 //   {
	//	m_bTrailActivate[a] = FALSE;
	//	m_hTrailEffect[a] = INVALID_HANDLE_VALUE;
	//}

	//InitializeEffect();
}

//------------------------------------------------------------------------------
/**
*/
Object::~Object( void )
{
}


//------------------------------------------------------------------------------
/**
*/
BOOL Object::Create( DWORD dwObjectKey, DWORD dwID )
{
	GetObjectData().Initialize();
	//SetVector(&m_wvVisiblePos,0.0f,0.0f,0.0f);
	m_woiOctIndex = INVALID_WZOCTINDEX;


	SetObjectKey(dwObjectKey);

    m_pPathExplorer = GetObjectData().GetPathExplorer();

	m_bAnimated = FALSE;

	//m_CurrentAnimID = 0;
	
	InitializeState();
	InitializeEffect();

//#ifdef _PERF_DEBUG
//	m_pPathExplorer->SetSHCStatus(TRUE);
//#endif

	return TRUE;
}

//------------------------------------------------------------------------------
void Object::Destroy()
{
	DestroyAllEffect();
	ReleaseRender();
	RemoveObjectToOctree();

	GetObjectData().Release();
}


//------------------------------------------------------------------------------
void Object::InitRender( size_t MaxPartNum, size_t BasePart, APPEARANCEINFO* pAppearInfo, size_t iAttachmentCount, BOOL bMakeBoneOnly )
{
	ReleaseRender();

	GetObjectData().InitializeRender( MaxPartNum, BasePart, pAppearInfo, iAttachmentCount );

	if (pAppearInfo) 
	{
	    pAppearInfo->texture_rate = g_OptimizationManager.GetTextureRate(this);
		APPEARANCEMGR->SetAppearance(pAppearInfo,this,TRUE,bMakeBoneOnly);
	}

	GetObjectData().RefreshAniRefDraw();
}

//------------------------------------------------------------------------------
//	에니메이션 정보 세팅
//------------------------------------------------------------------------------
void Object::RefreshRender(APPEARANCEINFO* pAppearInfo,BOOL bRefreshBone)
{
	assert(pAppearInfo);

	CWzUnitDraw** pParts = GetObjectData().GetRenderParts();
	int nBasePartIndex = GetObjectData().GetBasePartIndex();

	CWzUnitDraw* pBasePart = pParts[nBasePartIndex];

	WzVector pos = pBasePart->GetPosition();
	WzVector dir = pBasePart->GetRotation();
	WzID ani = pBasePart->GetCurrentAnimation();

	float frame = pBasePart->GetCurrentFrame();

	bool is_loop = false;

#if (WZENGINEVER >= 540) || defined(USING_MILKY_TRUNK_ENGINE)
	if( CWzAniState* ani_state = pBasePart->GetCurAniState() )
	{
		is_loop = ani_state->GetAniLoop();
	}
#endif

	//	각 파트 파괴
	if (bRefreshBone)
	{
		if (pParts[nBasePartIndex]) 
		{
			ResourceManager::Instance()->FreeResourceUnitDraw( pParts[nBasePartIndex] );
			pParts[nBasePartIndex] = NULL;
            pBasePart = NULL;
		}
	}
	
    pAppearInfo->texture_rate = g_OptimizationManager.GetTextureRate(this);
	APPEARANCEMGR->SetAppearance(pAppearInfo,this,bRefreshBone,FALSE);
    pBasePart = pParts[nBasePartIndex];

	GetObjectData().RefreshAniRefDraw();

	if (pBasePart) 
	{
		pBasePart->SetPosition(pos,FALSE); // 바운딩 볼륨을 쓸데없이 두번 업뎃하지않는다.
		pBasePart->SetRotation(dir.x,dir.y,dir.z,TRUE);
	}

	if (bRefreshBone)//에니메이션중에 에니메이션을 바꿀경우 무한 루핑에 빠진다.
	{
		//m_CurrentAnimID = ani;
		pBasePart->SetCurrentAnimation(ani);
		pBasePart->SetFrame(frame);
#if (WZENGINEVER > 540) || defined(USING_MILKY_TRUNK_ENGINE)
		if( CWzAniState* ani_state = pBasePart->GetCurAniState() )
		{
			ani_state->SetAniLoop(is_loop);
		}
#endif
		m_bAnimated = FALSE;
	}


	GetObjectData().EnableCustomLight( GetObjectData().IsCustomLight() );
	GetObjectData().SetShadowDraw( GetObjectData().IsShadowDraw() );
}

//------------------------------------------------------------------------------
void Object::ReleaseRender()
{
	CWzUnitDraw** pParts = GetParts();
	int nMaxPartNum = GetPartCount();

	if( nMaxPartNum == 0 )
		return;

	//	각 파트 파괴
	for( int i = 0 ; i < nMaxPartNum ; ++i )
	{
		if (pParts[i]) 
		{
			if ( IsKindOfObject(MAP_OBJECT) )
			{
				MAPRESMGR()->RemoveUnitDraw( pParts[i] );
			}
			else
			{
				if ( IsForceRemove() == true )
					ResourceManager::Instance()->FreeResourceUnitDrawVer2( pParts[i], true);
				else
					ResourceManager::Instance()->FreeResourceUnitDraw( pParts[i] );
			}

			pParts[i] = NULL;			
		}
	}

	GetObjectData().ReleaseRender();

}


//------------------------------------------------------------------------------
/**
	본 정보를 얻어 부착물 및 유닛을 랜더 한다. 
*/
BOOL Object::Render()
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

	if ( IsSimpleRenderMode() == TRUE )
	{
		GetObjectData().RenderBasicParts( NULL );
		return TRUE;
	}

    if( IsBodyOnlyRenderMode() == TRUE )
    {
        GetObjectData().RenderPartsForCostume();

        return TRUE;
    }

	GetObjectData().RenderParts( NULL );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Object::Render(St_RenderCommand *pCommand)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

	if ( IsSimpleRenderMode() == TRUE )
	{
		GetObjectData().RenderBasicParts( pCommand );
		return FALSE;
	}

	GetObjectData().RenderParts( pCommand );

	return TRUE;
}
//
//
////------------------------------------------------------------------------------
///**
//	Attachment들을 본에 붙여서 애니메이션 시켜준다
//*/
//void Object::ProcessAttachmentAnimation(DWORD dwTick)
//{
//	CWzUnitDraw* pSkeleton = GetBasePart();
//	if( pSkeleton == NULL ) 
//		return;
//
//
//	std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();
//
//	for( size_t i=0;i!=AttachmentArray.size();++i)
//	{
//		AttachmentArray[i].ProcessAnimation( dwTick );
//	}
//}

//------------------------------------------------------------------------------
/** 부착되지않았거나 제거되었을경우 FALSE를 리턴한다
*/
BOOL Object::SetAttachment( size_t nIndex, const AttachmentInfo& AttacmentInfo, float fRequestRateResizeTexture )
{
	std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();

	if ( nIndex >= AttachmentArray.size() )
	{
		return FALSE;
	}

	BOOL bPlayer = IsKindOfObject( PLAYER_OBJECT ) == TRUE;

	return AttachmentArray[nIndex].InitializeAttachment( bPlayer, AttacmentInfo, fRequestRateResizeTexture );
}

//------------------------------------------------------------------------------
/**
*/
BOOL Object::DeleteAttachment(size_t nIndex)
{
	std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();

	if ( nIndex >= AttachmentArray.size() )
	{
		return FALSE;
	}

	return AttachmentArray[nIndex].ReleaseAttachment();
}

//------------------------------------------------------------------------------
/**
*/
void  Object::SetAnimation(const char* pAnimation, BOOL bLoop, BOOL bRestart)
{
	SetAnimation(StrToWzID( pAnimation ), bLoop, bRestart);	
}

//------------------------------------------------------------------------------
/**
*/
void  Object::SetAnimation(WzID idAnimation, BOOL bLoop, BOOL bRestart)
{ 
#ifdef _DEBUG
    char str[256] = {NULL, };
    WzIDToString( str, idAnimation );
    if (Stricmp(str, "S418") == 0)
    {
        int a = 0;
    }
#endif //_DEBUG

	if (!GetBasePart()) 
		return;
 
	DestroyAllLoopEffect();

	BOOL bNoBlend = FALSE;

	//BOOL bRestart = TRUE;

	if (bLoop)
	{
		bRestart = FALSE;
	}
	
	WzID CurrentAnimation = GetCurrentAnimation();

	// 임시!
	// 이전애니가 죽는 애니였으면 블렌딩을 하지않는다.
	if (CurrentAnimation == StrToWzID("D001")||
		CurrentAnimation == StrToWzID("D002")||
		CurrentAnimation == StrToWzID("D003"))
	{
		bNoBlend = TRUE;
	}

	if(IsKindOfObject(MAP_OBJECT))
	{
		bNoBlend = TRUE;
	}

	if (CurrentAnimation == 0)
	{
		bNoBlend = TRUE;
	}

	// 똑같은 애니이면서 Loop이면 그냥 아무것도 안한다.
	if (CurrentAnimation == idAnimation && bLoop )
	{
		return;
	}
	
	GetBasePart()->SetCurrentAnimation( idAnimation, bLoop , bRestart);		

	WzAnimationInfo *pInfo = GetBasePart()->GetCurrentAnimationInfo();

	if( pInfo == NULL )
	{
		GetBasePart()->SetCurrentAnimation( idAnimation, bLoop , bRestart);
	}
	else
	{
		// 애니를 찾지 못했을경우 아무 애니메이션이나 플레이 해준다.
		if( pInfo->m_wiAction != idAnimation )
		{
            if (GetBasePart()->GetWzUnit() != NULL)
            {
			    pInfo = GetBasePart()->GetWzUnit()->GetAnimationInfoByIndex(0);
			    // 애니메이션이 완전히 없는 경우는 어쩔수없다.
			    if (pInfo) 
                {
				    GetBasePart()->SetCurrentAnimation( pInfo->m_wiAction, bLoop , bRestart);
			    }
            }
		}
	}	

	if (bNoBlend)
	{
		GetBasePart()->SetInterpolationFrame(0);
	}

	m_bAniProcessed = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void  Object::SetCurrentBlendAnimation(char *pAnimation,
                                       int boneindex,
                                       float fSpeed,
                                       int iBlendFrame,
                                       BOOL bDelta )
{
	SetCurrentBlendAnimation( StrToWzID( pAnimation ), boneindex, fSpeed, iBlendFrame, bDelta);
}

//------------------------------------------------------------------------------
/**
*/
void  Object::SetCurrentBlendAnimation(WzID idAnimation,int boneindex ,float fSpeed,int iBlendFrame,BOOL bDelta)
{
	CWzUnit* pUnit=GetBasePart()->GetWzUnit();

	if(!pUnit)
		return;

	WzAnimationInfo *pAnimationInfo =pUnit->GetAnimationInfo(idAnimation);


	if(!pAnimationInfo)
	{
		return;
	}

	if(pAnimationInfo->m_wiAction!=idAnimation)
	{
		return;
	}

	if (GetBasePart()->GetBlendAniCount() > 0)
	{
		return;
	}

	
	if (GetBasePart()->GetBlendAniCount() > 0)
	{
		GetBasePart()->UnSetBlendAnimation();
	}

	if (idAnimation != 0) 
	{
		if (iBlendFrame == -1)
		{
			GetBasePart()->SetBlendInterpolationFrame(-1, -1);

		}
		else
		{
			GetBasePart()->SetBlendInterpolationFrame(iBlendFrame,iBlendFrame);
		}
	}

    GetBasePart()->SetCurrentBlendAnimation(idAnimation, boneindex, FALSE, fSpeed,bDelta);

	m_bAniProcessed = FALSE; 
}

//------------------------------------------------------------------------------
void  Object::SetCurrentFrame(float fFrame)
{
	if( CWzUnitDraw* pPart = GetBasePart() )
	{
        pPart->SetFrame( fFrame );
	}
}

size_t Object::GetAnimationCount()
{
	if( CWzUnitDraw* pPart = GetBasePart() )
	{
		if( CWzUnit* pUnit = pPart->GetWzUnit() )
		{
			return pUnit->GetCountAnimationInfo();
		}
	}
	return 0;
}

WzID Object::GetCurrentAnimation()
{
	return GetBasePart() ? GetBasePart()->GetCurrentAnimation() : 0;
}

float Object::GetCurrentFrame()
{
	return GetBasePart() ? GetBasePart()->GetFrame() : 0.0f;
}

WzBoundingVolume* Object::GetBoundingVolume()
{
	return GetBasePart() ? GetBasePart()->GetBoundingVolumePtr() : NULL;
}

//------------------------------------------------------------------------------
/**
*/
float Object::GetTotalFrame()
{
	if( WzAnimationInfo* pInfo =  GetBasePart()->GetCurrentAnimationInfo() )
	{
		return (float)(pInfo->m_nMaxFrame);
	}

	return 0.0f;	
}

//------------------------------------------------------------------------------
float Object::ProcessAnimation( DWORD dwTick ,WzEvent *pEvent,BOOL bIgnoreEvent)
{
    // 메인파츠 유효체크
    if (GetBasePart() == NULL)
    {
        return 0.0f;
    }

    static WzEvent ani_event;
	ZeroMemory(&ani_event, sizeof(ani_event));
	
	// 애니메이션 프레임처리 (Event를받아서 이펙트,사운드,커스텀등처리) 
	g_pSunRenderer->ProgressUnitFrame(GetBasePart(), (float)dwTick, &ani_event, &current_ani_progress_rate_);
	ProcessAttachmentAnimation(dwTick);

	EventInfo event_info(ani_event);
	
#ifdef _DEV_VER
	if( event_info.IsGood() == false )
	{
		if ( CWzUnitDraw* pBase = GetBasePart() )
		{
			if( WzAnimationInfo* pAnimationInfo = pBase->GetCurrentAnimationInfo() )
			{
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("event_max: %s"), pAnimationInfo->m_lpszFileName);
			}
		}
	}
#endif // _DEV_VER


    // 사운드이벤트처리
    if ((dwTick != 0) && ((g_pHero != this) || (g_pHero->GetShow() == TRUE)))
    {
        // 주인공 보일때만
        const size_t event_count = event_info.GetSoundEventCount();
        for (size_t i = 0; i != event_count; ++i)
        {
            ProcessSoundEvent(event_info.GetSoundEvent(i), GetVisiblePos());
        }
    }
    
    // 이벤트무시 체크
    if (bIgnoreEvent == FALSE)
    {
        // 이펙트 이벤트
        DWORD object_key = IsKindOfObject(MAP_OBJECT) ? (0) : (GetObjectKey());
        const size_t event_count = event_info.GetEffectEventCount();
		for (size_t i = 0; i != event_count; ++i)
		{
            const EffectEventInfo& info = event_info.GetEffectEvent(i);
            if ((IsAnimated() == TRUE) || 
                (info.IsTrail_LeftToRight() && info.IsDestroy()) || 
                (info.IsTrail_RightToLeft() && info.IsDestroy()) ||
                (info.IsDestroyAll() == true) ||
                (info.IsDestroy() == true))
            {
            }
            else
            {
                AnimateUnit();
            }

			ProcessEffectEvent(event_info.GetEffectEvent(i), 
                                GetBasePart(), 
                                GetVisiblePos(), 
                                GetDirection(), object_key);
		}

        // 커스텀 이벤트
        if (ani_event.m_iNumCustom > 0)
        {
            const size_t event_count = event_info.GetCustomEventCount();
			for (size_t i = 0; i != event_count; ++i)
			{
				ProcessCustomEvent(event_info.GetCustomEvent(i), GetPosition());
			}
        }
    }
	
  	OnWaterEllipse();

	if (pEvent != NULL)
	{
		*pEvent = ani_event;
	}

	m_bAnimated = FALSE;	
	m_bAniProcessed = TRUE;

	return current_ani_progress_rate_;
}

//------------------------------------------------------------------------------
WzAnimationInfo* Object::GetAnimationInfo(WzID animation)
{
	if (GetBasePart() && GetBasePart()->GetWzUnit())
	{
		return GetBasePart()->GetWzUnit()->GetAnimationInfo(animation);
	}

	return NULL;
}

//------------------------------------------------------------------------------
WzID Object::GetTrailEffectCode()
{
	return StrToWzID("arrw");
}
//
////------------------------------------------------------------------------------
//void Object::StartTrail(int TrailIndex, const TRAIL_PARAM& TrailParam)
//{
//	if (!IsAnimated())
//	{
//		AnimateUnit();
//	}
//
//	assert(TrailIndex >= 0 && TrailIndex < MAX_TRAIL);
//	EndTrail(TrailIndex);
//
//	if (g_pEffectManager)
//	{
//		HANDLE hEffect = GlobalFunc::CreateEffectBeUpToOption(
//			GetTrailEffectCode(),
//			GetTrailBonePos(TrailIndex,1),
//			GetObjectKey());
//
//		m_EffectData.StartTrail( TrailIndex, TrailParam, hEffect );
//	}
//
//	UpdateTrail();
//}
//
////------------------------------------------------------------------------------
//void Object::EndTrail(int TrailIndex)
//{
//	m_EffectData.EndTrail( TrailIndex );
//}
//
////------------------------------------------------------------------------------
//void Object::UpdateTrail()
//{
//	for( int i=0; i < m_EffectData.GetTrailCount(); ++i )
//	{
//		if( m_EffectData.IsEnableTrail(i) == false )
//			continue;
//
//		m_EffectData.UpdateTrail( i, GetTrailBonePos(i,1), GetTrailBonePos(i,0) );
//	}
//}

//------------------------------------------------------------------------------
WzVector Object::GetTrailBonePos(int index,int start)
{
	WzVector vRet;
	ZeroMemory(&vRet,sizeof(vRet));

	const std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();

	if( AttachmentArray.empty() == false )
	{
		int iAttachIndex = effectdata().GetTrailAttachmentIndex( index );

		iAttachIndex=min( iAttachIndex, (int)AttachmentArray.size()-1);
		iAttachIndex=max( 0, iAttachIndex );

		int startboneindex = 3;
		int endboneindex = 4;	

		if( CWzUnitDraw* pSkeleton = AttachmentArray[iAttachIndex].GetUnitDraw() )
		{		 
            if (pSkeleton->GetWzUnit() != NULL)
            {
			    WzBoneInfo* pBoneInfo = pSkeleton->GetWzUnit()->GetBoneInfo(StrToWzID("EFF1"));
			    if (pBoneInfo)
			    {
				    startboneindex = pBoneInfo->m_iNode;
			    }

			    pBoneInfo = pSkeleton->GetWzUnit()->GetBoneInfo(StrToWzID("EFF2"));
			    if (pBoneInfo)
			    {
				    endboneindex = pBoneInfo->m_iNode;
			    }

			    int boneindex;
			    if (start) 
			    {
				    boneindex = startboneindex;
			    }
			    else
			    {
				    boneindex = endboneindex;
			    }

			    GameFunc::GetBonePos( pSkeleton, boneindex, &vRet );
            }
		}
	}

	return vRet;
}



	
//------------------------------------------------------------------------------
/**
*/
void Object::AnimateUnit()
{
	SUNPROFILE_BEGIN( _T("AnimateUnit") );
	
    CWzUnitDraw* pBasePart = GetBasePart();

	if (pBasePart == NULL)
	{
		SUNPROFILE_END( _T("AnimateUnit") );
		return;
	}

	if (m_bAniProcessed == FALSE)
	{
		ProcessAnimation(0, NULL, TRUE);
	}

	CWzUnitDraw** pParts = GetParts();
	int nPartCount = GetPartCount();

    g_pSunRenderer->AnimateUnit(pBasePart);
    for (int i = 1; i < nPartCount; ++i)
    {
        if ( pParts[i] != NULL)
        {
            g_pSunRenderer->AnimateUnit( pParts[i] );
        }
    }


    //반드시 메인파츠후에 호출되어야됨
	if( pBasePart != NULL )
	{
		std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();
		for( size_t i=0;i!=AttachmentArray.size();++i)
		{
			AttachmentArray[i].Animate( pBasePart, GetVisiblePos(), GetHeightOffset() );
		}
	}
	
	m_bAnimated = TRUE;

	UpdateTrail();
	//UpdateEffectPos();
    UpdateEffectPos(GetBasePart(), GetVisiblePos(), GetDirection());

	SUNPROFILE_END( _T("AnimateUnit") );
}
//
////------------------------------------------------------------------------------
///**
//*/
//void Object::InitializeEffect()
//{
//	m_EffectData.Initialize();
//}
////------------------------------------------------------------------------------
///**
//*/
//void Object::DestroyAllLoopEffect()
//{
//	m_EffectData.ReleaseLoopEffect();
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Object::DestroyObjectEffect(int index,int iBuffer)
//{
//	m_EffectData.ReleaseEffect( index, iBuffer );
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Object::DestroyAllEffect()
//{
//	m_EffectData.ReleaseAllEffect();
//}
////------------------------------------------------------------------------------
///**
//*/
//void Object::MoveRemainEffect(int index)
//{
//	m_EffectData.ReleaseCurEffect(index);
//}

//------------------------------------------------------------------------------
/**
*/
WzVector Object::GetBonePos(int index)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

	WzVector vRet;
	SetVector(&vRet, 0.f, 0.f, 0.f);

	vRet = GetVisiblePos();
	CWzUnitDraw* pSkeleton = GetBasePart();
	GameFunc::GetBonePos(pSkeleton,index,&vRet);
	
	return vRet;
}

//------------------------------------------------------------------------------
/**
*/
WzVector Object::GetSpecialBonePos(WzID id)
{
	int iNode = 0;
    if (GetBasePart()->GetWzUnit() != NULL)
    {
	    if (WzBoneInfo* pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(id))
	    {
		    iNode = pInfo->m_iNode;
	    }
    }

	return GetBonePos(iNode);
}


//------------------------------------------------------------------------------
/**
*/
WzMatrix Object::GetBoneMatrix(int index)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

	WzMatrix matRet;
	MatrixIdentity( &matRet );

	if( CWzUnitDraw* pSkeleton = GetBasePart() )
	{
		WzMatrix *pBoneMatrix = pSkeleton->GetTransformBuffer();	
	
		if(pBoneMatrix && index >= 0 && pSkeleton->GetTransformBufferSize() >  index ) // jjumbo: index가 음수나 범위를 넘어가면 뻗던것 수정.
		{
			matRet = pBoneMatrix[index];
		}
	}
	
	return matRet;
}


//------------------------------------------------------------------------------
/**
*/
void Object::StopBlendAnimation()
{
	if ( CWzUnitDraw* pBasePart = GetBasePart() )
	{
		if( pBasePart->GetBlendAniCount() > 0 )
		{
			pBasePart->UnSetBlendAnimation();
		}
	}
}

//------------------------------------------------------------------------------
/**
	본 정보를 얻어 부착물 및 유닛을 랜더 한다. 
- remark
  - SetColor 메소드를 추가함.
*/
BOOL Object::RenderOutline( const WzColor& color , float fThickness)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

    if( St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo() )
    {
        WzVector vOffset = pCameraInfo->m_wvFrom - (this->GetPosition());
        float fRange = VectorLength(&vOffset);
        if (fRange >= 7.f && fRange < 15.f)
        {
            fThickness = fThickness * 2.f;
        }
        else if (fRange >= 15.f && fRange < 20.f)
        {
            fThickness = fThickness * 3.f;
        }
        else if (fRange >= 20.f && fRange < 30.f)
        {
            fThickness = fThickness * 5.f;
        }
        else if (fRange >= 30.f && fRange < 50.f)
        {
            fThickness = fThickness * 8.f;
        }
        else if (fRange >= 50.f)
        {
            fThickness = fThickness * 15.f;
        }
        else
        {
            fThickness = fThickness * 1.f;
        }
    }

    if (IsBodyOnlyRenderMode() == TRUE)
    {
        return GetObjectData().RenderOutlineForCostume( color, fThickness );
    }
    else
    {
        return GetObjectData().RenderOutline( color, fThickness );
    }
       
}
////------------------------------------------------------------------------------
///**
//*/
//void Object::UpdateEffectPos()
//{
//	m_EffectData.UpdateEffect( GetBasePart(), GetVisiblePos(), GetDirection() );
//}

//------------------------------------------------------------------------------
/**
*/
void Object::RenderInfo()
{
	WzVector ChatPosition = GetVisiblePos();
	ChatPosition.z += GetMeshHeight();
	ChatPosition.z += 0.1f;

	ObjectChatData::RenderChatMessage( ChatPosition );
}

//------------------------------------------------------------------------------
/**
*/
float Object::GetMeshHeight()
{
	return 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Object::Process(DWORD dwTick)
{
	ProcessChatTime( dwTick );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Object::SetShow(BOOL bFlag)
{
	m_bVisible = bFlag;

    // 유닛 이펙트 보이기/감추기
    SetSubTypeEffectShow( bFlag );
}

//------------------------------------------------------------------------------
/**
*/
float Object::GetCurrentProgressRate()
{
	float fTotalFrame = GetTotalFrame();
	
	if (fTotalFrame >= 1.0f) {		
		return (GetBasePart()->GetCurrentFrame() * 100.0f) / float(fTotalFrame-1);
	}
	else
	{
		return 0.0f;
	}

}

//------------------------------------------------------------------------------
/**
옥트리에 (다시) 추가한다
*/
void Object::AddObjectToOctree()
{
	RemoveObjectToOctree();

	WzBoundingVolume* pBV = GetBoundingVolume();

	m_woiOctIndex = g_pMap->GetWorldBase()->x_pOctree->AddObject( DTO_UNIT, GetObjectKey(), pBV );
}

void Object::RemoveObjectToOctree()
{
	if (m_woiOctIndex != INVALID_WZOCTINDEX)
	{
        if (g_pMap->GetWorldBase() != NULL)
        {
            g_pMap->GetWorldBase()->x_pOctree->RemoveObject(DTO_UNIT,GetObjectKey(),m_woiOctIndex);
        }
		m_woiOctIndex = INVALID_WZOCTINDEX;
	}
}


DWORD Object::GetCurrentTileAtt( BOOL bForce )
{
	if( CPathExplorer* pPathExplorer = GetObjectData().GetPathExplorer() )
	{
		if( g_pMap != NULL )
		{
			int iTile = m_pPathExplorer->GetTile();
			if( iTile >= 0 )
				return g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute( iTile );
		}
	}

	return 0;
}


//------------------------------------------------------------------------------
void Object::ProcessAttachmentAnimation(DWORD dwTick)
{
	CWzUnitDraw* pSkeleton = GetBasePart();
	if( pSkeleton == NULL ) 
		return;

	std::vector<ObjectAttachment>& AttachmentArray = GetObjectData().GetAttachmentArray();

	for( size_t i=0;i!=AttachmentArray.size();++i)
	{
		AttachmentArray[i].ProcessAnimation( dwTick );
	}
}
//
////------------------------------------------------------------------------------
//// 사운드 이벤트 처리
//void Object::ProcessSoundEvent(const SoundEventInfo& SoundEventInfo )
//{
//    // 주인공 보일때만
//    if ((g_pHero == this) &&
//        (g_pHero->GetShow() == FALSE))
//    {
//        return;
//    }
//
//	g_SoundEffect.Play( SoundEventInfo.GetSoundIndex(), &GetVisiblePos() );
//}
//
//
////------------------------------------------------------------------------------
//// 이펙트 이벤트 처리
//void Object::ProcessEffectEvent(const EffectEventInfo& EffectEventInfo)
//{
//	if( EffectEventInfo.IsTrail_LeftToRight() == true )
//	{
//		if( EffectEventInfo.IsDestroy() )
//		{
//			EndTrail(1);
//		}
//		else
//		{
//			TRAIL_PARAM param;
//			param.iAttachmentIndex = 1;
//			param.iTailNum = 40;
//			StartTrail(1, param);
//		}
//	}
//	else if( EffectEventInfo.IsTrail_RightToLeft() == true )
//	{
//		if( EffectEventInfo.IsDestroy() )
//		{
//			EndTrail(0);
//		}
//		else
//		{
//			TRAIL_PARAM param;
//			param.iAttachmentIndex = 0;
//			param.iTailNum = 40;
//			StartTrail(0, param);
//		}
//	}
//	else if( EffectEventInfo.IsDestroyAll()  == true )
//	{
//		// 모든 이펙트 삭제
//		DestroyAllEffect();
//	}
//	else if( EffectEventInfo.IsDestroy() == true )
//	{
//		// 루프가 아닌경우 바로죽이지않고 자연스레 죽이기위해 옮겨놓기
//		MoveRemainEffect( EffectEventInfo.GetEventIndex() );
//
//		// 해당 이펙트 삭제
//		DestroyObjectEffect( EffectEventInfo.GetEventIndex(), CUR_EFFECT);
//	}
//	else
//	{
//        if (!IsAnimated())
//        {
//            AnimateUnit();
//        }
//
//		m_EffectData.BeginEffect( GetBasePart(), GetVisiblePos(), GetDirection(), EffectEventInfo, IsKindOfObject(MAP_OBJECT) ? (0) : (GetObjectKey()) );
//	}
//}
//
////------------------------------------------------------------------------------
//// 커스텀 이벤트 처리
//void Object::ProcessCustomEvent( const CustomEventInfo& Info )
//{
//	if( Info.IsFootStep() == true )
//	{
//		OnFootStep();
//	}
//	else if( Info.IsWaterWave() == true )
//	{
//		OnWaterWave();
//	}
//	else if( Info.IsCameraWave() == true )
//	{
//		if ( g_pHero != NULL )
//		{
//			WzVector distance = g_pHero->GetPosition() - GetPosition();
//			if ( VectorLength(&distance) > 15.f )
//			{
//				return;
//			}
//
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_ALL);
//			g_Camera.StartVibration( Info.GetCameraWaveDuration(), Info.GetCameraWavePower() );
//		}
//	}
//}

#pragma warning ( pop )
