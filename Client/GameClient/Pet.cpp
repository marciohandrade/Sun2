//------------------------------------------------------------------------------
//  Pet.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "Pet.h"
#include "globalfunc.h"
#include "soundeffect.h"
#include "AppearanceManager.h"
#include "Map.h"
#include "Hero.h"
#include "Monster.h"
#include "HeroActionInput.h"
#include "SCItemSlot.h"
#include "GameFramework.h"
#include "ObjectManager.h"
#include "InterfaceManager.h"
#include "ItemManager.h"
#include "GameOptionParser.h"
#include "PetInfoParser.h"
#include "NpcInfoParser.h"
#include "Interface/uiPetMan/uiPetMan.h"
#include "Interface/uiPetGauge/uiPetGauge.h"
#include <SCSlotContainer.h>


//------------------------------------------------------------------------------
/** 
*/
Pet::Pet( void )
    :pet_dialog_(NULL)
{
	memset(&m_Pet,0,sizeof(m_Pet));
	m_Pet.hInstanceEffect=INVALID_HANDLE_VALUE;
	m_Pet.PetEffectParam.hEffect=INVALID_HANDLE_VALUE;
	SetObjectType(PET_OBJECT);
	
}
//------------------------------------------------------------------------------
/** 
*/
Pet::~Pet( void )
{
    pet_dialog_ = NULL;
}
//------------------------------------------------------------------------------
/**  팻생성
*/
BOOL Pet::Create( DWORD dwObjectKey, DWORD dwID )
{
	SetObjectKey( dwObjectKey );

	BASE_NPCINFO *pNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( dwID );

	if (!pNPCInfo)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("Pet::Create() Invalid NPCCode %d"),dwID);
		return FALSE;
	}
	//-----------------------------------------------------------------------	
	// 몬스터 정보 연결
	//-----------------------------------------------------------------------
	m_Pet.m_NPCInfo = *pNPCInfo;

    BOOL rt = Character::Create(dwObjectKey, dwID);
	assert(rt);

	//	상태 초기화( IDLE 상태 )
	InitState(STATE::IDLE,g_CurTime);

    InitMeshHeight();

	return TRUE;
}


//------------------------------------------------------------------------------
/**  Process 재정의
*/
BOOL Pet::Process( DWORD dwTick )
{
    if (GetBasePart())
    {
        UpdateMeshHeight();

        ProcessPetUnit(dwTick);

        ProcessEmoticon(dwTick);
        
        Character::Process(dwTick);
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/**
	펫소멸시 호출 이펙트해제
*/
void Pet::Destroy( void )
{
    if (pet_dialog())
    {
        pet_dialog()->ShowInterface(FALSE);
        pet_dialog_ = NULL;
    }

	DestroyEffect();
	Character::Destroy();
}
//------------------------------------------------------------------------------
/**	펫랜더정보(게이지.이름)
*/
void Pet::RenderInfo()
{
    if (GetBasePart() == NULL)
    {
        return;
    }

	if ( FALSE == g_InterfaceManager.GetShowFlag())
	{
		return;
	}

    if ((m_Pet.bHeroPet == TRUE) && (pet_dialog() != NULL))
    {
        WzVector wzVisualPos = GetPetBoundingVolume();
        wzVisualPos.z += 0.45f;

        pet_dialog()->set_fullness((int)m_Pet.byFullness, C_PET_MAX_GAUGE);
        pet_dialog()->set_intimacy((int)m_Pet.byIntimacy, C_PET_MAX_GAUGE);
        pet_dialog()->render(wzVisualPos);
    }

	
	//step2:펫이름랜더여부
	if(m_Pet.ePetName==NAME_UNDRAW || m_Pet.bPetNameHide)
	{
		return;
	}

	if( m_Pet.byHideState!=HIDE_SKILL_INFO::ALL_HIDE)
	{
		RenderPetName();
	}
}
//------------------------------------------------------------------------------
/**
	이펙트위치갱신
*/
void Pet::AnimateUnit()
{
    if (GetBasePart() == NULL)
    {
        return;
    }

	Object::AnimateUnit();

	UpdatePetEffectPos();
}
//------------------------------------------------------------------------------
/**
    펫 서브타입 효과
*/	
void Pet::SetSubType(BYTE subtype)
{
    CWzUnitDraw* unit_draw_ptr = GetBasePart();

    if (!unit_draw_ptr)
    {
        return;
    }

    CWzUnit* unit_ptr = unit_draw_ptr->GetWzUnit();

    if (!unit_ptr)
    {
        return;
    }
    
    // 펫유닛이 서브타입 효과가 존재하는지
    if ((subtype < unit_ptr->GetNumSubType()) &&
        (subtype > 0))
    {
        unit_draw_ptr->SetSubType(subtype);
    }

}
//------------------------------------------------------------------------------
/**
	펫속성갱신(친밀도.포만감)
*/	
void Pet::UpdatePetAttributes(BOOL bHero)
{
	//친밀도 체크
	if(m_Pet.bMaxIntimacyFlag)
	{
		if(m_Pet.byIntimacy!=100)
		{
			//친밀도MAX옵션해제
			m_Pet.bMaxIntimacyFlag  = FALSE;
			CreateInstanceEffect(E_DECREASE_INTIMACY);
		}
	}
	else
	{
		if(m_Pet.byIntimacy==100)
		{	
			m_Pet.dwMaxIntimacyOldTime = m_Pet.dwMaxIntimacyTime=0;
			m_Pet.bMaxIntimacyFlag = TRUE;
		}
	}
	
	//포만감체크
	if(m_Pet.bLowFullnessFlag)
	{
		if(m_Pet.byFullness>10)
		{
			m_Pet.bLowFullnessFlag = FALSE;
		}
	}
	else
	{
		if(m_Pet.byFullness<=10)
		{
			//최초배고픈상태일경우는 한번 바로 띄워주게끔시간맞춘다
			m_Pet.dwLowFullnessTime = m_Pet.dwLowFullnessOldTime = 0;
			m_Pet.bLowFullnessFlag = TRUE;
		}
	}


}
//------------------------------------------------------------------------------
/**
	위치타입값 얻기(워프위치,시작위치)
*/
WzVector Pet::GetBasePostion()
{
	Player *pPlayer = GetMasterObject();

	if(!pPlayer)
	{
		return c_ZeroVector;
	}
		
	WzVector vTargetPos = pPlayer->GetPosition();
	WzVector vWarpDest  = vTargetPos+(pPlayer->GetDirection()* -(GetStopPositon()));
	
	//못가는곳이거나 bFlyOnHead일경우 바로주인공위치로
	if( m_Pet.bFlyOnHead || g_pSunTerrain->x_pPathFinder->GetAllowTileToStand( vWarpDest) < 0 )
	{
		vWarpDest.x = vTargetPos.x;
		vWarpDest.y = vTargetPos.y;
	}
	return vWarpDest;
}
//------------------------------------------------------------------------------
/**
	캐릭터씬에서 펫배치함수
*/
void Pet::SetModifyPosition(float fAngle,float fDistance)
{
	if(!GetMasterObject())
	{
		return;
	}

	WzVector vDirection;
	vDirection.x = cos(fAngle);
	vDirection.y = sin(fAngle);
	vDirection.z = 0.0f;
	SetPosition( (GetMasterObject()->GetPosition()+vDirection*fDistance),-1);
}
//------------------------------------------------------------------------------
/**
	펫을 초기화한다(주인키값초기화,및 시작위치설정)
*/
void Pet::InitPetMonster(DWORD dwMaserKey,WORD wPetIndex)
{
	m_Pet.dwMasterObjKey =  dwMaserKey;
	m_Pet.dwIndex		 =  wPetIndex;

    Player* master_ptr = GetMasterObject();

    if (master_ptr == NULL)
    {
        return;
    }

	if(g_pHero == master_ptr)
	{
		m_Pet.bHeroPet = TRUE;
	}

    if (m_Pet.bHeroPet == TRUE)
    {
        pet_dialog_ = GET_CAST_DIALOG(uiPetGauge, IM_PET_MANAGER::PET_GAUGE_DLG);
        pet_dialog()->ShowInterface(TRUE);
    }
    else
    {
        pet_dialog_ = NULL;
    }

     m_Pet.pet_script_ptr_ = PetInfoParser::Instance()->GetPetInfo(m_Pet.dwIndex);

	//기본적인 펫네임셋팅(NPC명보다 펫인포참조)
	if(m_Pet.pet_script_ptr_)
	{
		SetName(g_InterfaceManager.GetInterfaceString(m_Pet.pet_script_ptr_->m_dwNameCode));

        //고정게이지인지 
		m_Pet.byMoveGaugeUI = m_Pet.pet_script_ptr_->m_byPdisplayMove;
        if (m_Pet.pet_script_ptr_->m_byPpossition == 1)
        {
            m_Pet.bFlyOnHead = TRUE;
        }

        this->SetPosition( this->GetBasePostion(), -1);
        this->SetDirection(master_ptr->GetPosition());
        SetStatePetHide(master_ptr->GetHideSkill());
	}

    RefreshApperPet(master_ptr);    
}
//------------------------------------------------------------------------------
/**
    펫 외형 로드
*/
bool Pet::RefreshApperPet(Player* master_ptr)
{
    if ((master_ptr != NULL) && (master_ptr->IsUseDelayLoading() == true))
    {
        return false;
    }

    if (GetBasePart())
    {
        return false;
    }

    APPEARANCEINFO pet_appearance;
    ZeroMemory(&pet_appearance,sizeof(pet_appearance));
    pet_appearance.ObjectType = APPEARANCEINFO::PET;
    pet_appearance.pMonsterInfo = &m_Pet.m_NPCInfo;
    InitRender(MP_COUNT,MP_BONE,&pet_appearance,0,FALSE);

    if (m_Pet.pet_script_ptr_)
    {
        SetSubType(m_Pet.pet_script_ptr_->effect_subtype);
    }
    this->SetFadeIn(1500);

    return true;
}
//------------------------------------------------------------------------------
/**
	펫의 시작위치셋팅(공중유닛위치보정:하드코딩)
*/
void Pet::UpdateMeshHeight()
{
	Player* master_ptr = GetMasterObject();

	if (master_ptr == NULL || 
        m_Pet.pet_script_ptr_->m_byPpossition == 0)
	{
		return;
	}

	//step2:펫기본위치설정(0.땅바닥 1.머리 2:뒤공중)
    float offset_pos = 0.0f;
	switch(m_Pet.pet_script_ptr_->m_byPpossition)
	{
	case 1://캐릭터머리바로위 떠있는팻들
		{
            offset_pos = HEIGHT_GAP;
            // 캐릭터씬 예외처리
            if (GameFramework::GetCurScene() == SCENE_TYPE_CHARSELECT)
            {
                offset_pos = (-HEIGHT_GAP);
            }
		}
		break;
	case 2://캐릭터뒤에 떠있는팻들
		{
            offset_pos = -0.9f;
		}
		break;
	}
    SetHeight(master_ptr->GetMeshHeight() + offset_pos);
}
//------------------------------------------------------------------------------
/** 펫이펙트 생성: 단발이펙트생성
*/
void Pet::CreateInstanceEffect(ePET_EFFECT eType)
{
	//너무 연속적으로 같은이펙트생성을 방지위해서
	//한이펙트가 소멸후 생성하게끔
	if(m_Pet.hInstanceEffect != INVALID_HANDLE_VALUE)
	{
		if(m_Pet.eOldEffectType == E_AUTO_PICK_ITEM)
		{
			return;
		}
		else
		{
			DestroyEffect();
		}
	}
	//루프이펙트활성화상태에서 단발이펙트가 들어올경우 겹치지않게하기위해
	//이펙트 지우고 4초후에 다시띄우게끔(전제조건:현재루프이펙트는 5분대기밖에없다)
	if(m_Pet.bEffect)
	{
		DestroyEffect();
		m_Pet.dwPetBoringCnt = 296000;
	}

	WzVector vPos = GetPetBoundingVolume();
	vPos.z +=HEIGHT_GAP;
	vPos+=(GetDirection()*0.2f);
	m_Pet.hInstanceEffect  = CreateInstanceEffect(eType, vPos);
	m_Pet.eOldEffectType	  = eType;	

}
//------------------------------------------------------------------------------
/** 펫이펙트 생성:위치보정
*/
HANDLE Pet::CreateInstanceEffect(ePET_EFFECT eType, WzVector vPos)
{
	WzID wzID=GetPetEffectCode(eType);

	return GlobalFunc::CreateEffectBeUpToOption(wzID,vPos,this->GetObjectKey());
}
//------------------------------------------------------------------------------
/**
	펫먹이 먹는액션
*/
void Pet::PetEatAction()
{
	if(this->SetNextState( STATE::PET_EAT_ACTION_STATE,g_CurTime ) )
	{
		CreateInstanceEffect(E_EAT);
	}
}
//------------------------------------------------------------------------------
/** 
	펫의 이펙트를 활성화한다
*/
void Pet::AddMotionEffect(ePET_EFFECT eType,float fScaleSize)
{
	DestroyEffect();

	WzID wzID = GetPetEffectCode(eType);

	PET_EFFECT_PARAM param;
	ZeroMemory(&param,sizeof(param));
	
	param.eEffectType = (ePET_EFFECT)eType;

	WzVector vPos=GetPetBoundingVolume();
	param.hEffect=GlobalFunc::CreateEffectBeUpToOption(wzID,vPos,GetObjectKey());
	
	if(!param.hEffect) return;

	memcpy(&m_Pet.PetEffectParam,&param,sizeof(PET_EFFECT_PARAM));
	
	this->UpdatePetEffectPos();
	
	m_Pet.bEffect =  TRUE;
}
//------------------------------------------------------------------------------
/** 
	워프 액션(거리가 너무멀면 펫주인에게 워프한다)
*/
void Pet::WrapAction()
{
	if(!GetMasterObject()) return;

	this->SetPosition( GetBasePostion(), -1);

	CreateInstanceEffect(E_WARP,GetPosition());
}
//------------------------------------------------------------------------------
/** 
	펫따라가기
*/
void Pet::FollowAction()
{
	Player* pMaster = GetMasterObject(); 	

	if(!pMaster) return;
	
	//이동할때만 값초기화 하겠다
	if(m_Pet.bEffect)
	{
		m_Pet.dwPetBoringCnt = 0;
		DestroyEffect();
	}

	if (GetCurState() == STATE::MOVE)
	{
		//주인이동중일때만
		if(pMaster->IsMove())
		{
			if(GetDistanceMaster(m_Pet.vOldPosition) >= GetFollowDistance())
			{
				m_Pet.vOldPosition = pMaster->GetPosition();
				Move(GetPosition(),pMaster->GetPosition(),CMS_RUN,FALSE);
			}
		}
	}
	else
	{
		//이동상태가 아니면 이동
		if(this->SetNextState( STATE::MOVE,g_CurTime ))
		{
			m_Pet.dwIdlePetOldTime = 0;
			m_Pet.vOldPosition = pMaster->GetPosition();
			if(!Move(GetPosition(),pMaster->GetPosition(),CMS_RUN,FALSE))
			{
				WrapAction();
			}
		}
	}
}
//------------------------------------------------------------------------------
/** 기본액션 수행
*/
void Pet::BaseAction(DWORD dwTick)
{
	if(GetCurState()==STATE::MOVE)
	{
		//따라가기 상태에서 주인과 적정거리에있을때 멈춘다.
		if(GetDistanceMaster(this->GetPosition()) <= GetStopPositon())
		{
			this->SetNextState(STATE::PET_IDLE,g_CurTime);
		}
	}
	else if(GetCurState()==STATE::IDLE)
	{
		if( (m_Pet.dwIdlePetTime+=dwTick) >= m_Pet.dwIdlePetOldTime )
		{
			m_Pet.dwIdlePetTime =  0;
			this->SetNextState(STATE::PET_IDLE,g_CurTime);	
			m_Pet.dwIdlePetOldTime = 8000 + rand() % 8000;
		}
	}
}
//------------------------------------------------------------------------------
/** 
*/
BOOL Pet::Render()
{
    if ( GetShow() == FALSE  ) 
		return FALSE;	

    return Character::Render();
}
//------------------------------------------------------------------------------
/** 
	 펫 하이드시키기
*/
void Pet::SetStatePetHide(BYTE byHide)
{
	m_Pet.byHideState =  byHide;	

    switch(m_Pet.byHideState)
    {
    case HIDE_SKILL_INFO::NONE_HIDE:
        {
            SetColor(WzColor_RGBA( 255,255,255,255));
            //비하이드상태
           if(!GetShow())
           {
                SetFadeIn(1000);
           }
           SetShow(TRUE);
        }
        break;
    case HIDE_SKILL_INFO::HALF_HIDE:
        {
            //반절하이드일때
            SetShow(TRUE);
        }
        break;
    case HIDE_SKILL_INFO::ALL_HIDE:
        {
            //안보이기
            SetShow(FALSE);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/** 
	펫하이드적용
*/
void Pet::StartExtraEffect()
{
    if(m_Pet.byHideState==HIDE_SKILL_INFO::HALF_HIDE)
    {
        SetColor(WzColor_RGBA( 255,255,255,128));
    }
}
//------------------------------------------------------------------------------
/** 
	펫 락걸기(주인을 더이상 따라가지않는다 제자리에서 애니만)
*/
void Pet::SetPetLock(BOOL bLock)	
{
	m_Pet.bCharacterScene = TRUE;
	m_Pet.bPetLock		 = bLock;
	m_Pet.ePetName		 = NAME_UNDRAW;
}
//------------------------------------------------------------------------------
/** 
	주인공 포인터얻기
*/
Player* Pet::GetMasterObject()
{
	Player* pMaster = static_cast<Player*>(g_ObjectManager.GetObject(m_Pet.dwMasterObjKey));
	return pMaster; 
}
//------------------------------------------------------------------------------
/**
	주인과의 위치체크(거리차 리턴)
*/
float Pet::GetDistanceMaster(WzVector vPos)
{
	Player* pMaster = GetMasterObject();

	if(!pMaster) return 0.0f;

	return VectorLength(&(vPos- pMaster->GetPosition()));
}
//------------------------------------------------------------------------------
/**
*/
void Pet::ProcessPetUnit(DWORD dwTick)
{
	//펫락상태에서는 IDLE만함
	if(m_Pet.bPetLock)
	{
		if(this->GetNextState() == STATE::NONE)
		{
			this->SetNextState(STATE::IDLE,g_CurTime);
		}
		return;
	}

	float fDistance = GetDistanceMaster(this->GetPosition());
		
	//step1:너무먼지체크(워프체크)
	if(fDistance >= GetWarpDistance())
	{
		WrapAction();		
	}//step1:따라가기체크
	else if(fDistance >= GetFollowDistance() )
	{
		FollowAction();		
	}
	else
	{	
		//기본액션처리
		BaseAction(dwTick);		
	}
}
//------------------------------------------------------------------------------
/** 펫 바운딩볼륨얻기
*/
WzVector Pet::GetPetBoundingVolume()
{
	WzVector visible_position = GetVisiblePos();

	BOOL fixed_volume = (!m_Pet.byMoveGaugeUI || m_Pet.bFlyOnHead) ? (TRUE):(FALSE);

    WzBoundingVolume* bounding_volume_ptr = GetPickingBoundingVolume(0);

    if (!bounding_volume_ptr)
    {
        return visible_position;
    }
    
	//플라이유닛은 바운딩볼륨으로 체크한다.
	if (fixed_volume)
	{
        m_fMeshHeight = (bounding_volume_ptr->m_aabb.m_wvMax.z - bounding_volume_ptr->m_aabb.m_wvMin.z) + 0.1f;

		visible_position.z += GetMeshHeight();
	}
	else
	{
		visible_position.z = bounding_volume_ptr->m_aabb.m_wvMax.z;
	}

	return visible_position;
}
//------------------------------------------------------------------------------
/** 펫 딜레이타임(랜덤이펙트 포만감하락,친밀도하락등)
*/
void Pet::ProcessEmoticon(DWORD dwTick)
{	
	//친밀도 100%일때 2~3분마다 이펙트 출력
	if(m_Pet.bMaxIntimacyFlag)
	{
		if( (m_Pet.dwMaxIntimacyOldTime+=dwTick) >= m_Pet.dwMaxIntimacyTime )
		{
			m_Pet.dwMaxIntimacyOldTime =  0;
			m_Pet.dwMaxIntimacyTime = 120000+ rand() % 60000; //2~3분간격으로 랜덤
			CreateInstanceEffect(E_MAX_INTIMACY);
		}
	}

	//포만감 10%이하일때 2~3분마다 이펙트 출력	
	if(m_Pet.bLowFullnessFlag)
	{	
		if( (m_Pet.dwLowFullnessOldTime+=dwTick) >= m_Pet.dwLowFullnessTime )
		{
			m_Pet.dwLowFullnessOldTime =  0;
			m_Pet.dwLowFullnessTime = 120000+ rand() % 60000; //2~3분간격으로 랜덤
			CreateInstanceEffect(E_MIN_FULLNESS);
		}
	}

	//5분이상대기시(루프이펙트)
	if(!m_Pet.bEffect && !m_Pet.bCharacterScene)
	{
		if( (m_Pet.dwPetBoringCnt+=dwTick) >=300000)
		{
			AddMotionEffect(E_BORING,1.5f);
		}
	}
}
//------------------------------------------------------------------------------
/** 펫네임셋팅
*/
void Pet::RefreshPetName()
{
	//주인공펫을 경우 아이템을 참조한다.
	if(m_Pet.bHeroPet)
	{
		SCItemSlot & rInvenItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer( SI_INVENTORY ))->GetSlot(g_pHero->GetPetItemPos()); 
		
		//팻정보가 없을 경우가 있나?
		//하지만 여기서 죽음. 스크립트 문제일수도 있지만.
		//보호코드 하나정도는 있어주는게 안전할듯.
		SCPetInfo* pPetInfo_ = rInvenItemSlot.GetPetInfo();

		if(pPetInfo_)
		{
			if(pPetInfo_->IsSetPetName())
			{
				m_Pet.ePetName = NAME_DRAW;
			}

			if( m_Pet.ePetName == NAME_DRAW)
			{
				TCHAR	szPetName[MAX_PET_NAME_LENGTH+1];

				ZeroMemory(szPetName, sizeof(szPetName));

				StrnCopy(szPetName, pPetInfo_->GetPetName(), MAX_PET_NAME_LENGTH);

				SetName(szPetName);

				//이름초기화체크
				if(_tcslen(szPetName)==0)
				{
					m_Pet.ePetName  = NAME_UNDRAW;
				}
			}
		}
		
	}
	else
	{
		//플레이어도 펫네임이 있다면 그대로 셋팅
		Player *pPlayer = GetMasterObject();
		
		if(!pPlayer) return;
		
		//이름이 존재한다면셋팅
		if(pPlayer->IsSetPetName())
		{
			SetName(pPlayer->GetPetName());

			//이름초기화체크
			if(_tcslen(pPlayer->GetPetName())==0)
			{
				m_Pet.ePetName  = NAME_UNDRAW;
			}
		}
	}
}
//------------------------------------------------------------------------------
/** 펫네임 랜더
*/
void Pet::RenderPetName()
{
	//펫네임끄기옵션중이라면 랜더안함
	if(!( GAMEOPTION->GetShowPetName())) return ;

	TCHAR *szString = const_cast<TCHAR*>(GetName());

	float fDepth;

	WzColor color = WzColor_RGBA(217,149,148,255);
	
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
    g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	WzVector wzVisualPos = GetPetBoundingVolume();
	wzVisualPos.z += HEIGHT_GAP;

	POINT pt2;
	g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos ,&fDepth);

	RECT rc;
	SIZE textsize;

	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szString,lstrlen(szString),&textsize);

	rc.left = pt2.x - (textsize.cx / 2);
	rc.top	= pt2.y;
	rc.right = rc.left + textsize.cx;
	rc.bottom = rc.top + 50;

	WzColor BackColor = WzColor_RGBA(0,0,0,100);		

	if( IsShowName() == TRUE )
		RenderBlockedText(szString,&rc,color,BackColor);
	else
		RenderBlockedTextWithDepthTest(szString,&rc,color,BackColor,fDepth);		

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
	g_pSunRenderer->SetColor(oldcolor);
}
//------------------------------------------------------------------------------
/**
*/
void Pet::RenderPetGauge()
{
	static const float INFO_WIDTH   = 136.f;		
	static const float INFO_HEIGHT  = 27.f;
	
	static const float GAUGE_WIDTH  = 126.f;			
	static const float GAUGE_HEIGHT = 8.f;

	//1이면 포만감 0이면 친밀도
	BYTE byFullness = m_Pet.byFullness;
	BYTE byIntimacy = m_Pet.byIntimacy;
	
	if(byFullness>C_PET_MAX_GAUGE)
	{
		byFullness = 100;
	}

	if(byIntimacy>C_PET_MAX_GAUGE)
	{
		byIntimacy = 100;
	}

	WzVector wzVisualPos = GetPetBoundingVolume();

	wzVisualPos.z += 0.45f;

	float fDepth;
	POINT pt2;
		
	g_pSunRenderer->Set2DMode(TRUE);
	g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos, &fDepth );
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	//1.랜더영역잡기
	RECT rc;
	rc.left = pt2.x - ( (int)INFO_WIDTH / 2);
	rc.top = pt2.y - 44;
	rc.right = rc.left + (int)INFO_WIDTH;
	rc.bottom = rc.top + 27;

	//2.게이지계산
	float fFullness = (((float)GAUGE_WIDTH)/C_PET_MAX_GAUGE)*byFullness;
	float fIntimacy = (((float)GAUGE_WIDTH)/C_PET_MAX_GAUGE)*byIntimacy;

	BOOL bTest   = g_pSunRenderer->IsEnableZTest();
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	g_pSunRenderer->SetZBufferTest( TRUE );
	g_pSunRenderer->SetZBufferWrite(TRUE);

	//3.백판넬그리기
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left, (float)rc.top, INFO_WIDTH, INFO_HEIGHT,
		0.0f, 0.0f, INFO_WIDTH, INFO_HEIGHT, 0, fDepth );

	//4.친밀도
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left+5, (float)rc.top+5, fFullness, GAUGE_HEIGHT,
		5.0f, 28.0f, fFullness, GAUGE_HEIGHT, 0, fDepth );

	//5.포만감
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left+5, (float)rc.top+14, fIntimacy, GAUGE_HEIGHT,
		5.0f, 37.0f, fIntimacy, GAUGE_HEIGHT, 0, fDepth );

	St_Camera *pCam = g_pSunRenderer->GetCameraInfo();
	float fNewTest = pCam->m_fNear + (pCam->m_fFar - pCam->m_fNear) * fDepth;
	g_pSunRenderer->x_pManagerTextOut->BeginSpriteTextWithDepth(fDepth);
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("st07") );

	TCHAR szText[128];
	SIZE textsize;

	//포만감 
	Sprintf(szText,_T("%d/%d"),min(byFullness,C_PET_MAX_GAUGE),C_PET_MAX_GAUGE);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szText,lstrlen(szText),&textsize);
	g_pSunRenderer->x_pManagerTextOut->DrawText( szText, pt2.x - (textsize.cx/2), rc.top+5, WzColor_RGBA(255, 255, 255,255));

	//친밀도 
	Sprintf(szText,_T("%d/%d"),min(byIntimacy,C_PET_MAX_GAUGE),C_PET_MAX_GAUGE);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szText,lstrlen(szText),&textsize);
	g_pSunRenderer->x_pManagerTextOut->DrawText( szText, pt2.x - (textsize.cx/2), rc.top+14, WzColor_RGBA(255, 255, 255,255));
	g_pSunRenderer->x_pManagerTextOut->EndSpriteTextWithDepth();
	
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bTest );
	g_pSunRenderer->Set2DMode( FALSE );

}
//------------------------------------------------------------------------------
/** 
펫의 이펙트 정보를 갱신해준다.(서버에서 STATUS변경 패킷이 왔을때만 호출)
펫아이템 POS가 바뀔때나 초기화될때 호출된다.
*/
void Pet::RefreshEffect()
{	
	//1이면 포만감 0이면 친밀도
	BYTE byFullness = m_Pet.byFullness;
	BYTE byIntimacy = m_Pet.byIntimacy;

	//이펙트 없을때
	if(m_Pet.bEffect==FALSE && m_Pet.PetEffectParam.hEffect == INVALID_HANDLE_VALUE)
	{	
		//우선순위는 친밀도
		if(byIntimacy==C_PET_MAX_GAUGE)
		{
			g_SoundEffect.PlayUI(UI_SOUND_PET_INTIMACY);
			AddMotionEffect(E_MAX_INTIMACY);
		}
		else if(byFullness<10)
		{
			g_SoundEffect.PlayUI(UI_SOUND_PET_FULLNESS);
			AddMotionEffect(E_MIN_FULLNESS);
		}
		return;
	}

	//이펙트가 존재할때
	if(m_Pet.bEffect && m_Pet.PetEffectParam.hEffect != INVALID_HANDLE_VALUE)
	{
		if(m_Pet.PetEffectParam.eEffectType==E_MIN_FULLNESS)				
		{	
			//포만감이 10이아닐때는 이펙트제거
			if(byFullness>=10)
			{
				DestroyEffect();
			}
		}
		else if(m_Pet.PetEffectParam.eEffectType==E_MAX_INTIMACY)
		{
			//친밀도가 max가 아닐때 이펙트제거
			if(byIntimacy<C_PET_MAX_GAUGE)
			{
				DestroyEffect();
			}
		}
	}
}
//------------------------------------------------------------------------------
/** 해당 이펙트를 제거해준다.
*/
void Pet::DestroyEffect()
{
	if(g_pEffectManager && m_Pet.PetEffectParam.hEffect!= INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_Pet.PetEffectParam.hEffect);
		m_Pet.PetEffectParam.hEffect=INVALID_HANDLE_VALUE;
		m_Pet.bEffect	= FALSE;
	}

	if(g_pEffectManager  && m_Pet.hInstanceEffect!= INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_Pet.hInstanceEffect);
		m_Pet.hInstanceEffect = INVALID_HANDLE_VALUE;
		m_Pet.bPetNameHide = FALSE;
	}
}
//------------------------------------------------------------------------------
/** 펫이펙트 위치 보정
*/
void  Pet::UpdatePetEffectPos()
{
	//이펙트 없을땐 그리지말자
	if(m_Pet.hInstanceEffect==INVALID_HANDLE_VALUE &&  m_Pet.PetEffectParam.hEffect==INVALID_HANDLE_VALUE)
	{
		m_Pet.bPetNameHide = FALSE;
		return;
	}
	
	//Scale
	WzVector vScale;
     SetVector(&vScale,1.f,1.f,1.f);
	//Rot
	WzVector vRot = c_ZeroVector;
	//Pos
	WzVector wvPos = GetPetBoundingVolume();
	wvPos+=(GetDirection()*0.2f);
	wvPos.z += (HEIGHT_GAP);
		
	//루프이펙트 
	CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_Pet.PetEffectParam.hEffect);

	if(pBody)
	{
		pBody->SetShow(!m_Pet.byHideState);
		pBody->SetResultMatrix(wvPos,vRot,vScale);
	}
 
	//인스턴스이펙트(이동하면서 이펙트가 자주발생하고 이펙트 라이프타임이너무길어서 보정)
	CEffectBody* pBodyInstance = g_pEffectManager->GetEffectPtrHandle(m_Pet.hInstanceEffect);
	if(pBodyInstance)
	{
		pBodyInstance->SetShow(!m_Pet.byHideState);
		pBodyInstance->SetResultMatrix(wvPos,vRot,vScale);
	}
	else
	{
		m_Pet.hInstanceEffect = INVALID_HANDLE_VALUE;
	}

	m_Pet.bPetNameHide = ((pBody || pBodyInstance) ? (TRUE):(FALSE));
}

