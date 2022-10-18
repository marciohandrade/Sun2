#include "SunClientPrecompiledHeader.h"
#include "mapobject.h"
#include "Map.h"
#include "AppearanceManager.h"
#include "Camera.h"
#include "GameFunc.h"
#include "Mouse.h"
#include "interfacemanager.h"
#include "CollectionParser.h"
#include "GlobalFunc.h"
#include "GameConst.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "GuildMarkInfoParser.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemFunction.h"

extern const WzID battle_ground_object_type_id[];
extern const WzID battle_ground_waypoint_animation_id[];
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

// 맵 오브젝트의 파트 식별 인덱스
enum MapPart
{
	MAPPART_BONE = 0,
	MAPPART_COUNT,
};


MapObject::MapObject()
: m_bTriggerObject( FALSE )
, shadow_object_( false )
, m_bTriggerOperating( FALSE )
, m_bEndTriggerAnimation( FALSE )
{

	m_wiCollectionID = INVALID_WZID;
	m_hCollectionEftHdl = INVALID_HANDLE_VALUE;

	SetbCanHit(true);
}

MapObject::~MapObject()
{
}

BOOL MapObject::Create(DWORD dwObjectKey, APPEARANCEINFO * pAppearInfo)
{
	SetObjectType(MAP_OBJECT);


	// 순서 바꾸지 마세요( 이팩트가 모두 모여서 나옵니다. )
	BOOL bRet = Object::Create(dwObjectKey, 0);

	InitRender(MAPPART_COUNT, MAPPART_BONE, pAppearInfo,0,FALSE);	
    
	// m_pPathExplorer가 필요하기때문에 필요.
	SetPosition( pAppearInfo->wzPos );

	//InitializeState();

	// TODO : 이곳에서 기본 스테이트를 실행할수 있게한다.
	//	상태 초기화( IDLE 상태 )
	InitState(STATE::IDLE,g_CurTime);

	SetShow( TRUE );
	m_bTriggerObject	= FALSE;
	m_bTriggerOperating = FALSE;

	m_iAnimationDelay = 0;
	m_dwDelayAnimation = 0;
	m_dwTriggerAnimation = 0;
    m_bTriggerAnimationLoop = FALSE;

	m_iStateChangeDelay = 0;
	m_bDelayStateChange = FALSE;

	m_iTriggerID = 0;

	for (int a = 0; a < 4; a++)
	{
		m_dwCustomAttribute[a] = 0;
	}

	m_fMeshHeight=0.0f;

	if( WzBoundingVolume* pBV = GetBoundingVolume() )
	{
		m_OldBv = *pBV;

		if (pBV->m_eBvType == BVT_AABB)
		{
			m_fMeshHeight = (pBV->m_aabb.m_wvMax.z - pBV->m_aabb.m_wvMin.z);			
		}
	}

	m_OldBv.m_aabb.m_wvMin.z+=1.0f;//처음엔 무조건 업데이트하게 한다.

	if(TRUE == GENERALPARAM->IsNet())
	{
		
	}
	else
	{
		m_wiCollectionID = pAppearInfo->wiUserID;
		if(INVALID_WZID != m_wiCollectionID)
		{
			const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(m_wiCollectionID);
			if(NULL == pCI)
				return FALSE;

			WzVector wvPos;
			float fHeight = 0.f;
			if( WzBoundingVolume* pBV = GetBoundingVolume() )
			{
				switch(pBV->m_eBvType)
				{
				case BVT_AABB:
					fHeight = pBV->m_aabb.m_wvMax.z;
					break;
				case BVT_SPHERE:
					fHeight = pBV->m_BvSphere.m_wvCenter.z + pBV->m_BvSphere.m_fRadius;
					break;
				}
			}

			SetVector(&wvPos, GetPosition().x, GetPosition().y, fHeight + 0.5f);
			m_hCollectionEftHdl = GlobalFunc::CreateEffectBeUpToOption(pCI->m_wzidEffect[CollectionInfo::COLLECTABLE], wvPos, GetObjectKey());
		}
	}

    TCHAR str_id[5] = {0,};
    WzIDToString(str_id, pAppearInfo->MapObjectID);
    if (str_id[0] == 'Z' && str_id[1] == 'Z')
    {
        is_billboard = true;
    }
    else
    {
        is_billboard = false;
    }
	
	//////////////////////////////////////////////////////////////////////////
	StateProcess( g_CurTime, 100 );

	UpdateOctree();

	return TRUE;
}

void MapObject::UpdateOctree()
{
	if( WzBoundingVolume* pBV = GetBoundingVolume() )
	{
		if( m_OldBv.m_aabb.m_wvMax != pBV->m_aabb.m_wvMax || 
			m_OldBv.m_aabb.m_wvMin != pBV->m_aabb.m_wvMin )
		{
			AddObjectToOctree();

			m_OldBv =* pBV;
		}
	}
}



void MapObject::SetCollectionID(WzID wiCollectionID)
{
	if(INVALID_WZID != wiCollectionID)
	{
		if(wiCollectionID == m_wiCollectionID)
			return;

		m_wiCollectionID = wiCollectionID;

		const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(m_wiCollectionID);
		if(NULL == pCI)
			return;

		WzVector wvPos;
		float fHeight = 0.f;
		if( WzBoundingVolume* pBV = GetBoundingVolume() )
		{
			switch(pBV->m_eBvType)
			{
			case BVT_AABB:
				fHeight = pBV->m_aabb.m_wvMax.z;
				break;
			case BVT_SPHERE:
				fHeight = pBV->m_BvSphere.m_wvCenter.z + pBV->m_BvSphere.m_fRadius;
				break;
			}
		}
		SetVector(&wvPos, GetPosition().x, GetPosition().y, fHeight + 0.5f);
		m_hCollectionEftHdl = GlobalFunc::CreateEffectBeUpToOption(pCI->m_wzidEffect[CollectionInfo::COLLECTABLE], wvPos, GetObjectKey());
	}
	else
	{
		RemoveCollectionIDandEffect();
	}
}

void MapObject::RemoveCollectionIDandEffect()
{
	if(INVALID_HANDLE_VALUE != m_hCollectionEftHdl)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCollectionEftHdl);
		m_hCollectionEftHdl = INVALID_HANDLE_VALUE;
	}

	m_wiCollectionID	= INVALID_WZID;
}

float MapObject::GetMeshHeight()
{
	return m_fMeshHeight;
}

void MapObject::Destroy()
{
	DetachLightMap();

	m_vecRealTriggerID.clear();

	Object::Destroy();
}

BOOL MapObject::Process( DWORD dwTick )
{
//#ifdef LAST_RELEASE
//    if( IsEnableDisplay() == TRUE )
//#else
//    if( IsEnableDisplay() == TRUE || IsTriggerObj() == TRUE )
//#endif
	{
		SUNPROCESSPROFILE_BEGIN("MapObject::Process");

		UpdateOctree();

		// 3초후 xxx애니메이션으로 바꾼다 등에 대한 처리
		if(m_dwDelayAnimation )
		{
			m_iAnimationDelay -= dwTick;
			if (m_iAnimationDelay <= 0)
			{
				m_dwTriggerAnimation = m_dwDelayAnimation;
				m_iAnimationDelay = 0;
				m_dwDelayAnimation = 0;
			}
		}

		if (m_iStateChangeDelay > 0)	
		{
			m_iStateChangeDelay -= dwTick;
			if (m_iStateChangeDelay <= 0)
			{
				m_iStateChangeDelay = 0;
				if (m_bDelayStateChange)
				{
					SetShow(TRUE);
				}
				else
				{
					SetShow(FALSE);
				}

			}
		}

        SUNPROCESSPROFILE_BEGIN(_T("ProcessMapObjectState"));
        StateProcess( g_CurTime, dwTick );
        SUNPROCESSPROFILE_END(_T("ProcessMapObjectState"));

		SUNPROCESSPROFILE_END("MapObject::Process");
	}

	return TRUE;
}


//BOOL MapObject::ProcessInterpolation( DWORD dwTick )
//{
//	return TRUE;
//}

BOOL MapObject::RenderOutline( const WzColor& color , float fThickness )
{
	if ( GetShow() == FALSE )
	{
		return TRUE;
	}

	return Object::RenderOutline(color,fThickness);
}

void MapObject::RenderGuildMark()
{
	if(FALSE == IsDominationObject())
		return;
	if(NULL == g_pMap)
		return;
	if(NULL == g_pHero)
		return;
	uiDominationTowerMan* pDTMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
	if(NULL == pDTMan)
		return;

	DOMINATION_GUILD_INFO* pDGI = pDTMan->GetDominationGuildInfo();
	if(NULL == pDGI)
		return;
	WzVector wvPos1, wvPos2;

	WzColor wcOld = g_pSunRenderer->GetDiffuseColor();
	ENUM_ALPHABLEND oldMode = g_pSunRenderer->GetAlphaBlendMode();
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	BOOL bZTest  = g_pSunRenderer->IsEnableZTest();
	ENUM_CULL_FACE eCullFaceMode = g_pSunRenderer->GetCullFaceMode();

	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor( GetColor() );
	g_pSunRenderer->SetZBufferWrite(TRUE);	
	g_pSunRenderer->SetZBufferTest(TRUE);
	
	BASE_GuildMarkInfo * pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pDGI->m_BackGroundIndex);
	if(pMarkInfo)
	{
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK0")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK1")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK2")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK3")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK4")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK5")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
	}

	pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pDGI->m_PatternIndex);
	if(pMarkInfo)
	{
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK0")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK1")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK2")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK3")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
		wvPos1 = GetBonePos(GetBoneIndex(StrToWzID("MRK4")));
		wvPos2 = GetBonePos(GetBoneIndex(StrToWzID("MRK5")));
		g_InterfaceManager.RenderGuildMark4MapObject(&wvPos1, &wvPos2, pMarkInfo->iRow, pMarkInfo->iColumn);
	}

	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetAlphaBlendMode( oldMode );
	g_pSunRenderer->SetColor(wcOld);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bZTest );
}

BOOL MapObject::Render( void )
{

#ifdef _PERF_DEBUG
	if (!IsTriggerObj())
	{
		return FALSE;
	}
#endif

	if (GetShow() == FALSE) 
		return FALSE;	
	if (GetBasePart() == NULL) 
		return FALSE;

	if (g_pMap)
	{
		g_pMap->GetWorldBase()->SetGouraudForTerrainObject(TRUE);
	}

    if (is_billboard == true)
    {
        static WzVector wvRotWave;
        wvRotWave.x=0.0;
        wvRotWave.y=0.0;
        wvRotWave.z=Math_GetAngleFromVector(&g_pSunRenderer->GetCameraInfo()->m_wvDir);
        GetBasePart()->SetRotation( wvRotWave.x,wvRotWave.y,wvRotWave.z ,FALSE);
    }

    BOOL z_write = g_pSunRenderer->IsEnableZWrite();

    if (shadow_object())
    {
        g_pSunRenderer->SetZBufferWrite(FALSE);
    }

	BOOL bRet = Object::Render();
    
    g_pSunRenderer->SetZBufferWrite(z_write);
    
	if (g_pMap)
	{
		g_pMap->GetWorldBase()->SetGouraudForTerrainObject(FALSE);
	}

	RenderGuildMark();

	return bRet;
}

BOOL MapObject::SetPosition( const WzVector& wvPos, int iTileIndex )
{
	g_pSunTerrain->PathExp_Reset( m_pPathExplorer, wvPos, -1);

    if (GetBasePart())
    {
	    GetBasePart()->SetPosition(wvPos);
    }

	// 이전 코드가 이랬음.. 이게 맞는건지 틀린건지 구분이 안됨
	if( g_pSunTerrain && g_pMap )
	{
		AddObjectToOctree();
	}

	SetVisiblePos( m_pPathExplorer->GetVisiblePos() );

	return TRUE;
}

WzID MapObject::GetCurrentIdleAnimation()
{
	return StrToWzID("D000");
}


WzID MapObject::GetCurrentTriggerAnimation()
{
	return m_dwTriggerAnimation;
}

void MapObject::SetTriggerAnimation(WzID animID,int iDelay,BOOL bDone, BOOL bLoop)
{
	m_bEndTriggerAnimation = bDone;

	if (!iDelay) 
    {
		m_dwTriggerAnimation = animID;
        m_bTriggerAnimationLoop = bLoop;
		m_iAnimationDelay = 0;
		m_dwDelayAnimation = 0;
	}
	else
	{
        m_bTriggerAnimationLoop = bLoop;
		m_iAnimationDelay = iDelay;
		m_dwDelayAnimation = animID;
    }	
}

//-------------------------------------------------------------------------------------------
/**
*/
void MapObject::PlayTriggerAnimation()
{
	SetAnimation(m_dwTriggerAnimation, m_bTriggerAnimationLoop);

	if (m_bEndTriggerAnimation)
	{
		float fTotalFrame = GetTotalFrame();
		SetCurrentFrame(fTotalFrame);
		// 이벤트를 무시해서 플레이
		ProcessAnimation(0,NULL,TRUE);
		m_bEndTriggerAnimation = FALSE;		
	}	
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsEndTriggerAnimation()
{
	return m_bEndTriggerAnimation;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject_Render( CDrawBase *pDrawBase,WzVector vHeroPos,CWzUnitDraw *pUnitDraw,St_Camera * pCameraInfo,WzColor TerrainColor)
{	
	if (!pUnitDraw) return FALSE;
	const float fVisibleRate = 0.08f;
	const float fAlphaRate = 0.15f;	
	
	WzVector vOffset = pCameraInfo->m_wvFrom - pUnitDraw->GetPosition();
	float fRange = VectorLength(&vOffset);
	float fVolume = 0.0f;
	float fBase = fRange * sin(pCameraInfo->m_fFov / 2.0f) * 2.0f;

	WzBoundingVolume Volume = pUnitDraw->GetBoundingVolume();
	switch(Volume.m_eBvType)
	{
	case BVT_AABB:
		{
			float fX = Volume.m_aabb.m_wvMax.x - Volume.m_aabb.m_wvMin.x;
			float fY = Volume.m_aabb.m_wvMax.y - Volume.m_aabb.m_wvMin.y;
			float fZ = Volume.m_aabb.m_wvMax.z - Volume.m_aabb.m_wvMin.z;
			fVolume = max(max(fX,fY),fZ);
		}
		break;
	case BVT_SPHERE:
		fVolume = Volume.m_BvSphere.m_fRadius * 2.0f;
		break;
	}

	WzColor oldcolor = pDrawBase->GetDiffuseColor();
	pDrawBase->SetColor(TerrainColor);

	pDrawBase->RenderUnit(pUnitDraw);

	pDrawBase->SetColor(oldcolor);

	return TRUE;
}

BOOL MapObject_GrassRender( CDrawBase *pDrawBase,WzVector vHeroPos,CWzUnitDraw *pUnitDraw,St_Camera * pCameraInfo,WzColor TerrainColor)
{
	if (!pUnitDraw) return FALSE;

	const float fVisibleGrassRange = g_Camera.GetFarClip() - 80.0f; // 풀이 보이는거리
	const float fAlphaGrassRange = g_Camera.GetFarClip() - 40.0f; // 풀이 알파처리되서 보이는 거리
	const float fVisibleRate = 0.01f;

	WzVector vOffset = pCameraInfo->m_wvFrom - pUnitDraw->GetPosition();
	WzVector wvRotOld;
	wvRotOld = pUnitDraw->GetRotation();
	float fRange = VectorLength(&vOffset);
	if (fRange <= fVisibleGrassRange) 
    {
		pDrawBase->EnableAlphaSort( FALSE );	// 속도 향상 기법!
		pDrawBase->SetColor( TerrainColor );
		pDrawBase->AnimateUnit( pUnitDraw, 0 );
		pDrawBase->RenderUnit( pUnitDraw, 0 );
		pDrawBase->EnableAlphaSort( TRUE );
	}
	else if (fRange <= fAlphaGrassRange)
	{
		ENUM_ALPHABLEND eAlphablendMode = pDrawBase->GetAlphaBlendMode();
		pDrawBase->SetAlphaBlendMode( AB_ALPHA, FALSE );

		float fRate = 1.0f - (fRange - fVisibleGrassRange) / (fAlphaGrassRange - fVisibleGrassRange); // 거리에따라 알파값을 조정해준다.
		pDrawBase->EnableAlphaSort( FALSE );	// 속도 향상 기법!
		WzColor color;
		color = WzColor_RGBA(Red_WzColor(TerrainColor),
			Green_WzColor(TerrainColor),
			Blue_WzColor(TerrainColor),
			(BYTE)(255.0f * fRate));
		pDrawBase->SetColor( color );
		pDrawBase->AnimateUnit( pUnitDraw, 0 );
		pDrawBase->RenderUnit( pUnitDraw, 0 );
		pDrawBase->EnableAlphaSort( TRUE );

		pDrawBase->SetAlphaBlendMode( eAlphablendMode, FALSE );
	}

	// rotate 복구
	pUnitDraw->SetRotation( wvRotOld );
	pDrawBase->SetColor(255,255,255);

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
void			
MapObject::SetTriggerObj(BOOL bFlag)
{
	this->m_bTriggerObject = bFlag;
}


//----------------------------------------------------------------------------
/**
*/
void MapObject::SetTriggerOperating(BOOL bFlag)
{
	this->m_bTriggerOperating = bFlag;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsTriggerOperating()
{
	return this->m_bTriggerOperating;
}

//----------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsTriggerObj()
{
	return this->m_bTriggerObject;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsPickable()
{
	if (!GetShow())
	{
		return FALSE;
	}

#ifdef MAP_EDIT_MODE
	if(IsTerrain())
		return TRUE;
#endif 

	if(0 != m_wiCollectionID)
		return TRUE;

	if (TRUE == IsDominationObject())
	{
		return TRUE;
	}	

    if (IsAirShipObject())
    {
        return TRUE;
    }

    if (IsBattleGroundResurrectionObject())
    {
        return TRUE;
    }

	if(TRUE == IsWayPointObj())
		return TRUE;

	return CheckTriggerActive();
}

BOOL MapObject::CheckTriggerActive()
{
	if(IsTriggerObj())
	{
		TriggerManEx* pTriggerMan = g_pMap->GetTriggerMan();
		if(NULL == pTriggerMan)
			return FALSE;

		int iCnt = pTriggerMan->m_vecTrgObj.size();
		for(int i = 0; i < iCnt; i++)
		{
			int kCnt = m_vecRealTriggerID.size();
			for(int k = 0; k < kCnt; k++)
			{
				sTRIGGER_OBJECT* pTO = pTriggerMan->m_vecTrgObj[i];
				if(m_vecRealTriggerID[k] == pTO->m_dwTriggerID && m_iTriggerID == pTO->m_dwObjectKey)
				{
					if(TRUE == pTO->m_bActive)
						return TRUE;
				}
			}
		}

		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

//-------------------------------------------------------------------------------------------
BOOL MapObject::IsSettingNPCObject()
{
	// 서버에선 이값으로 NPC 정보를 알지만 클라이언트에선
	// 이놈이 설치형 NPC 인지만 알면 되기때문에 이렇게 처리함. comment by j0
	return (m_dwCustomAttribute[eMAPOBJECT_MONSTER]);
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsWayPointObj()
{
	
	return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_WAYPOINT);
}
//-------------------------------------------------------------------------------------------
/**
*/
bool MapObject::IsAirShipObject()
{
    return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_AIRSHIP);
}
//-------------------------------------------------------------------------------------------
/**
*/
//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool MapObject::IsBattleGroundStrongPointObject()
{
    return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_BATTLE_GROUND_STRONGPOINT);
}
//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool MapObject::IsBattleGroundResurrectionObject()
{
    return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_BATTLE_GROUND_RESURRECTION);
}
//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsTransparencyObj()
{

	return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_TRANSPARENCY);
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsDominationObject()
{
	return (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_DOMINATION_OBJECT);
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL MapObject::IsTerrain()
{

	return  (m_dwCustomAttribute[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_TERRAIN);
}

//-------------------------------------------------------------------------------------------
/**
	두번째 파라미터는 알파사용 이다 
*/
BOOL MapObject::IsAlpha()
{
	return  (m_dwCustomAttribute[eMAPOBJECT_USE_ALPHA] == 0);	
}

float MapObject::GetRotationAngle()
{
	float fRotate = 0.f;
	if (GetBasePart() )
	{
		fRotate = Math_GetAngleFromVector( &GetBasePart()->GetRotation() );
	}

	return fRotate;
}

//-------------------------------------------------------------------------------------------
/**
*/
WzVector MapObject::GetRotation()
{
	return GetBasePart()->GetRotation();
}

//-------------------------------------------------------------------------------------------
/**
*/
void MapObject::SetDelayStateChange(BOOL bShow,int iDelay)
{
	if (iDelay <= 0)
	{
		SetShow(bShow);
	}
	else
	{
		m_bDelayStateChange = bShow;
		m_iStateChangeDelay = iDelay;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void MapObject::SetCustomAttribute(int index,DWORD dwCustom)
{
	if (index >= 0 && index < MAPOBJECT_CUSTOM_ATTRIBUTE)
	{
		m_dwCustomAttribute[index] = dwCustom;
	}

}

//-------------------------------------------------------------------------------------------
/**
*/
DWORD MapObject::GetCustomAttribute(int index)
{
	if (index >= 0 && index < MAPOBJECT_CUSTOM_ATTRIBUTE)
	{
		return m_dwCustomAttribute[index];
	}

	return 0;
}

//-------------------------------------------------------------------------------------------
/**
*/
int MapObject::GetTriggerID()
{
	return m_iTriggerID;

}

//-------------------------------------------------------------------------------------------
/**
*/
void MapObject::SetID(int iID)
{
	m_iID=iID;
}

//-------------------------------------------------------------------------------------------
/**
*/
int MapObject::GetID()
{
	return m_iID;
}

//-------------------------------------------------------------------------------------------
/**
*/
void  MapObject::SetTriggerID(int iID) //트리거 오브젝트 ID
{
	m_iTriggerID = iID;
}	

int MapObject::GetBoneIndex(DWORD wiID)
{
	int boneindex = -1;

	if (GetBasePart()) 
	{
		if (GetBasePart()->GetWzUnit()) 
		{
			WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(wiID);

			if (pInfo) 
			{
				boneindex = pInfo->m_iNode;
			}
		}
	}

	return boneindex;
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
void MapObject::ChangeBattleGroundObjectAppearance(battle_ground_object_type object_type) // 거점 오브젝트 외형 변경
{
    if (object_type >= battle_ground_object_max)
    {
#ifdef _DEV_VER
        assert(!"object_type <= battle_ground_object_max,  type miss");
#endif //_DEV_VER
        return;
    }

    // 외형 변경
    APPEARANCEINFO appearInfo;
    memset(&appearInfo, 0, sizeof(appearInfo));
    appearInfo.ObjectType = APPEARANCEINFO::MAPOBJECT;
    appearInfo.MapObjectID = battle_ground_object_type_id[object_type];
    appearInfo.wzPos	= GetPosition();
    appearInfo.wzRot	= GetRotation();
    appearInfo.wzScale	= GetScale();
    appearInfo.wzColor  = GetColor();
    appearInfo.wzBV = *GetBoundingVolume();
    appearInfo.dwAttr = appearInfo.dwAttr;

    InitRender(MAPPART_COUNT, MAPPART_BONE, &appearInfo,0,FALSE);	
    SetPosition( appearInfo.wzPos );
}

void MapObject::ChangeBattleGroundObjectAnimation(battle_ground_waypoint_animation animation_type, bool is_visible_check) // 거점 오브젝트 애니메이션 변경
{
    if (animation_type >= battle_ground_waypoint_animation_max)
    {
#ifdef _DEV_VER
        assert(!"animation_type >= battle_ground_waypoint_animation_max,  type miss");
#endif //_DEV_VER
        return;
    }

    bool is_loop = true;

    if (is_visible_check == true)
    {
        // 터지는 이펙트는 화면상에 보일때에만 보여지게 한다.  화면 밖일땐 해당 색상의 기본모드로 변경해줌
        St_Camera *pCameraInfo = g_pSunRenderer->GetCameraInfo();
        BOOL bVisible = FALSE;
        WzMatrix mtViewTransform;
        g_pSunRenderer->GetViewMatrix(&mtViewTransform);

        WzBoundingVolume *pBV = GetBoundingVolume();

        if (Outside_Frustum_Bv(&pCameraInfo->m_wfFrustum,pBV) != NTCT_OUTSIDE)
        {
            if (!g_pMap->IsOccluded(&mtViewTransform,pBV))
            {
                bVisible = TRUE;
            }
        }

        if (bVisible == FALSE)
        {
            // 각각 터지는 애니를 기본애니로 변경
            switch (animation_type)
            {
            case battle_ground_blue_waypoint_bluefield_blueconquerd:
                animation_type = battle_ground_blue_waypoint_bluefield;
                break;
            case battle_ground_red_waypoint_redfield_redconquered:
                animation_type = battle_ground_red_waypoint_redfield;
                break;
            case battle_ground_normal_waypoint_bluefield_blueconquered:
                animation_type = battle_ground_normal_waypoint_bluefield;
                break;
            case battle_ground_normal_waypoint_redfield_redconquered:
                animation_type = battle_ground_normal_waypoint_redfield;
                break;
            case battle_ground_flag_change_animation:
                {
                    return;
                }
            }
        }
        else
        {
            switch (animation_type)
            {
            case battle_ground_blue_waypoint_bluefield_blueconquerd:
            case battle_ground_red_waypoint_redfield_redconquered:
            case battle_ground_normal_waypoint_bluefield_blueconquered:
            case battle_ground_normal_waypoint_redfield_redconquered:
            case battle_ground_flag_change_animation:
                is_loop = false;
                break;
            }
        }
    }

    SetAnimation(battle_ground_waypoint_animation_id[animation_type], is_loop);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//////////////////////////////////////////////////////////////////////////