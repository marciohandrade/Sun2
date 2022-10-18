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
/**  �ֻ���
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
	// ���� ���� ����
	//-----------------------------------------------------------------------
	m_Pet.m_NPCInfo = *pNPCInfo;

    BOOL rt = Character::Create(dwObjectKey, dwID);
	assert(rt);

	//	���� �ʱ�ȭ( IDLE ���� )
	InitState(STATE::IDLE,g_CurTime);

    InitMeshHeight();

	return TRUE;
}


//------------------------------------------------------------------------------
/**  Process ������
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
	��Ҹ�� ȣ�� ����Ʈ����
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
/**	�귣������(������.�̸�)
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

	
	//step2:���̸���������
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
	����Ʈ��ġ����
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
    �� ����Ÿ�� ȿ��
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
    
    // �������� ����Ÿ�� ȿ���� �����ϴ���
    if ((subtype < unit_ptr->GetNumSubType()) &&
        (subtype > 0))
    {
        unit_draw_ptr->SetSubType(subtype);
    }

}
//------------------------------------------------------------------------------
/**
	��Ӽ�����(ģ�е�.������)
*/	
void Pet::UpdatePetAttributes(BOOL bHero)
{
	//ģ�е� üũ
	if(m_Pet.bMaxIntimacyFlag)
	{
		if(m_Pet.byIntimacy!=100)
		{
			//ģ�е�MAX�ɼ�����
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
	
	//������üũ
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
			//���ʹ���»����ϰ��� �ѹ� �ٷ� ����ְԲ��ð������
			m_Pet.dwLowFullnessTime = m_Pet.dwLowFullnessOldTime = 0;
			m_Pet.bLowFullnessFlag = TRUE;
		}
	}


}
//------------------------------------------------------------------------------
/**
	��ġŸ�԰� ���(������ġ,������ġ)
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
	
	//�����°��̰ų� bFlyOnHead�ϰ�� �ٷ����ΰ���ġ��
	if( m_Pet.bFlyOnHead || g_pSunTerrain->x_pPathFinder->GetAllowTileToStand( vWarpDest) < 0 )
	{
		vWarpDest.x = vTargetPos.x;
		vWarpDest.y = vTargetPos.y;
	}
	return vWarpDest;
}
//------------------------------------------------------------------------------
/**
	ĳ���;����� ���ġ�Լ�
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
	���� �ʱ�ȭ�Ѵ�(����Ű���ʱ�ȭ,�� ������ġ����)
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

	//�⺻���� ����Ӽ���(NPC���� ����������)
	if(m_Pet.pet_script_ptr_)
	{
		SetName(g_InterfaceManager.GetInterfaceString(m_Pet.pet_script_ptr_->m_dwNameCode));

        //�������������� 
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
    �� ���� �ε�
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
	���� ������ġ����(����������ġ����:�ϵ��ڵ�)
*/
void Pet::UpdateMeshHeight()
{
	Player* master_ptr = GetMasterObject();

	if (master_ptr == NULL || 
        m_Pet.pet_script_ptr_->m_byPpossition == 0)
	{
		return;
	}

	//step2:��⺻��ġ����(0.���ٴ� 1.�Ӹ� 2:�ڰ���)
    float offset_pos = 0.0f;
	switch(m_Pet.pet_script_ptr_->m_byPpossition)
	{
	case 1://ĳ���͸Ӹ��ٷ��� ���ִ��ֵ�
		{
            offset_pos = HEIGHT_GAP;
            // ĳ���;� ����ó��
            if (GameFramework::GetCurScene() == SCENE_TYPE_CHARSELECT)
            {
                offset_pos = (-HEIGHT_GAP);
            }
		}
		break;
	case 2://ĳ���͵ڿ� ���ִ��ֵ�
		{
            offset_pos = -0.9f;
		}
		break;
	}
    SetHeight(master_ptr->GetMeshHeight() + offset_pos);
}
//------------------------------------------------------------------------------
/** ������Ʈ ����: �ܹ�����Ʈ����
*/
void Pet::CreateInstanceEffect(ePET_EFFECT eType)
{
	//�ʹ� ���������� ��������Ʈ������ �������ؼ�
	//������Ʈ�� �Ҹ��� �����ϰԲ�
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
	//��������ƮȰ��ȭ���¿��� �ܹ�����Ʈ�� ���ð�� ��ġ���ʰ��ϱ�����
	//����Ʈ ����� 4���Ŀ� �ٽö��Բ�(��������:�����������Ʈ�� 5�д��ۿ�����)
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
/** ������Ʈ ����:��ġ����
*/
HANDLE Pet::CreateInstanceEffect(ePET_EFFECT eType, WzVector vPos)
{
	WzID wzID=GetPetEffectCode(eType);

	return GlobalFunc::CreateEffectBeUpToOption(wzID,vPos,this->GetObjectKey());
}
//------------------------------------------------------------------------------
/**
	����� �Դ¾׼�
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
	���� ����Ʈ�� Ȱ��ȭ�Ѵ�
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
	���� �׼�(�Ÿ��� �ʹ��ָ� �����ο��� �����Ѵ�)
*/
void Pet::WrapAction()
{
	if(!GetMasterObject()) return;

	this->SetPosition( GetBasePostion(), -1);

	CreateInstanceEffect(E_WARP,GetPosition());
}
//------------------------------------------------------------------------------
/** 
	����󰡱�
*/
void Pet::FollowAction()
{
	Player* pMaster = GetMasterObject(); 	

	if(!pMaster) return;
	
	//�̵��Ҷ��� ���ʱ�ȭ �ϰڴ�
	if(m_Pet.bEffect)
	{
		m_Pet.dwPetBoringCnt = 0;
		DestroyEffect();
	}

	if (GetCurState() == STATE::MOVE)
	{
		//�����̵����϶���
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
		//�̵����°� �ƴϸ� �̵�
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
/** �⺻�׼� ����
*/
void Pet::BaseAction(DWORD dwTick)
{
	if(GetCurState()==STATE::MOVE)
	{
		//���󰡱� ���¿��� ���ΰ� �����Ÿ��������� �����.
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
	 �� ���̵��Ű��
*/
void Pet::SetStatePetHide(BYTE byHide)
{
	m_Pet.byHideState =  byHide;	

    switch(m_Pet.byHideState)
    {
    case HIDE_SKILL_INFO::NONE_HIDE:
        {
            SetColor(WzColor_RGBA( 255,255,255,255));
            //�����̵����
           if(!GetShow())
           {
                SetFadeIn(1000);
           }
           SetShow(TRUE);
        }
        break;
    case HIDE_SKILL_INFO::HALF_HIDE:
        {
            //�������̵��϶�
            SetShow(TRUE);
        }
        break;
    case HIDE_SKILL_INFO::ALL_HIDE:
        {
            //�Ⱥ��̱�
            SetShow(FALSE);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/** 
	�����̵�����
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
	�� ���ɱ�(������ ���̻� �������ʴ´� ���ڸ����� �ִϸ�)
*/
void Pet::SetPetLock(BOOL bLock)	
{
	m_Pet.bCharacterScene = TRUE;
	m_Pet.bPetLock		 = bLock;
	m_Pet.ePetName		 = NAME_UNDRAW;
}
//------------------------------------------------------------------------------
/** 
	���ΰ� �����;��
*/
Player* Pet::GetMasterObject()
{
	Player* pMaster = static_cast<Player*>(g_ObjectManager.GetObject(m_Pet.dwMasterObjKey));
	return pMaster; 
}
//------------------------------------------------------------------------------
/**
	���ΰ��� ��ġüũ(�Ÿ��� ����)
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
	//������¿����� IDLE����
	if(m_Pet.bPetLock)
	{
		if(this->GetNextState() == STATE::NONE)
		{
			this->SetNextState(STATE::IDLE,g_CurTime);
		}
		return;
	}

	float fDistance = GetDistanceMaster(this->GetPosition());
		
	//step1:�ʹ�����üũ(����üũ)
	if(fDistance >= GetWarpDistance())
	{
		WrapAction();		
	}//step1:���󰡱�üũ
	else if(fDistance >= GetFollowDistance() )
	{
		FollowAction();		
	}
	else
	{	
		//�⺻�׼�ó��
		BaseAction(dwTick);		
	}
}
//------------------------------------------------------------------------------
/** �� �ٿ���������
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
    
	//�ö��������� �ٿ���������� üũ�Ѵ�.
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
/** �� ������Ÿ��(��������Ʈ �������϶�,ģ�е��϶���)
*/
void Pet::ProcessEmoticon(DWORD dwTick)
{	
	//ģ�е� 100%�϶� 2~3�и��� ����Ʈ ���
	if(m_Pet.bMaxIntimacyFlag)
	{
		if( (m_Pet.dwMaxIntimacyOldTime+=dwTick) >= m_Pet.dwMaxIntimacyTime )
		{
			m_Pet.dwMaxIntimacyOldTime =  0;
			m_Pet.dwMaxIntimacyTime = 120000+ rand() % 60000; //2~3�а������� ����
			CreateInstanceEffect(E_MAX_INTIMACY);
		}
	}

	//������ 10%�����϶� 2~3�и��� ����Ʈ ���	
	if(m_Pet.bLowFullnessFlag)
	{	
		if( (m_Pet.dwLowFullnessOldTime+=dwTick) >= m_Pet.dwLowFullnessTime )
		{
			m_Pet.dwLowFullnessOldTime =  0;
			m_Pet.dwLowFullnessTime = 120000+ rand() % 60000; //2~3�а������� ����
			CreateInstanceEffect(E_MIN_FULLNESS);
		}
	}

	//5���̻����(��������Ʈ)
	if(!m_Pet.bEffect && !m_Pet.bCharacterScene)
	{
		if( (m_Pet.dwPetBoringCnt+=dwTick) >=300000)
		{
			AddMotionEffect(E_BORING,1.5f);
		}
	}
}
//------------------------------------------------------------------------------
/** ����Ӽ���
*/
void Pet::RefreshPetName()
{
	//���ΰ����� ��� �������� �����Ѵ�.
	if(m_Pet.bHeroPet)
	{
		SCItemSlot & rInvenItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer( SI_INVENTORY ))->GetSlot(g_pHero->GetPetItemPos()); 
		
		//�������� ���� ��찡 �ֳ�?
		//������ ���⼭ ����. ��ũ��Ʈ �����ϼ��� ������.
		//��ȣ�ڵ� �ϳ������� �־��ִ°� �����ҵ�.
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

				//�̸��ʱ�ȭüũ
				if(_tcslen(szPetName)==0)
				{
					m_Pet.ePetName  = NAME_UNDRAW;
				}
			}
		}
		
	}
	else
	{
		//�÷��̾ ������� �ִٸ� �״�� ����
		Player *pPlayer = GetMasterObject();
		
		if(!pPlayer) return;
		
		//�̸��� �����Ѵٸ����
		if(pPlayer->IsSetPetName())
		{
			SetName(pPlayer->GetPetName());

			//�̸��ʱ�ȭüũ
			if(_tcslen(pPlayer->GetPetName())==0)
			{
				m_Pet.ePetName  = NAME_UNDRAW;
			}
		}
	}
}
//------------------------------------------------------------------------------
/** ����� ����
*/
void Pet::RenderPetName()
{
	//����Ӳ���ɼ����̶�� ��������
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

	//1�̸� ������ 0�̸� ģ�е�
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
	//1.�����������
	RECT rc;
	rc.left = pt2.x - ( (int)INFO_WIDTH / 2);
	rc.top = pt2.y - 44;
	rc.right = rc.left + (int)INFO_WIDTH;
	rc.bottom = rc.top + 27;

	//2.���������
	float fFullness = (((float)GAUGE_WIDTH)/C_PET_MAX_GAUGE)*byFullness;
	float fIntimacy = (((float)GAUGE_WIDTH)/C_PET_MAX_GAUGE)*byIntimacy;

	BOOL bTest   = g_pSunRenderer->IsEnableZTest();
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	g_pSunRenderer->SetZBufferTest( TRUE );
	g_pSunRenderer->SetZBufferWrite(TRUE);

	//3.���ǳڱ׸���
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left, (float)rc.top, INFO_WIDTH, INFO_HEIGHT,
		0.0f, 0.0f, INFO_WIDTH, INFO_HEIGHT, 0, fDepth );

	//4.ģ�е�
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left+5, (float)rc.top+5, fFullness, GAUGE_HEIGHT,
		5.0f, 28.0f, fFullness, GAUGE_HEIGHT, 0, fDepth );

	//5.������
	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hPetInfoTexure, (float)rc.left+5, (float)rc.top+14, fIntimacy, GAUGE_HEIGHT,
		5.0f, 37.0f, fIntimacy, GAUGE_HEIGHT, 0, fDepth );

	St_Camera *pCam = g_pSunRenderer->GetCameraInfo();
	float fNewTest = pCam->m_fNear + (pCam->m_fFar - pCam->m_fNear) * fDepth;
	g_pSunRenderer->x_pManagerTextOut->BeginSpriteTextWithDepth(fDepth);
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("st07") );

	TCHAR szText[128];
	SIZE textsize;

	//������ 
	Sprintf(szText,_T("%d/%d"),min(byFullness,C_PET_MAX_GAUGE),C_PET_MAX_GAUGE);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szText,lstrlen(szText),&textsize);
	g_pSunRenderer->x_pManagerTextOut->DrawText( szText, pt2.x - (textsize.cx/2), rc.top+5, WzColor_RGBA(255, 255, 255,255));

	//ģ�е� 
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
���� ����Ʈ ������ �������ش�.(�������� STATUS���� ��Ŷ�� �������� ȣ��)
������� POS�� �ٲ𶧳� �ʱ�ȭ�ɶ� ȣ��ȴ�.
*/
void Pet::RefreshEffect()
{	
	//1�̸� ������ 0�̸� ģ�е�
	BYTE byFullness = m_Pet.byFullness;
	BYTE byIntimacy = m_Pet.byIntimacy;

	//����Ʈ ������
	if(m_Pet.bEffect==FALSE && m_Pet.PetEffectParam.hEffect == INVALID_HANDLE_VALUE)
	{	
		//�켱������ ģ�е�
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

	//����Ʈ�� �����Ҷ�
	if(m_Pet.bEffect && m_Pet.PetEffectParam.hEffect != INVALID_HANDLE_VALUE)
	{
		if(m_Pet.PetEffectParam.eEffectType==E_MIN_FULLNESS)				
		{	
			//�������� 10�̾ƴҶ��� ����Ʈ����
			if(byFullness>=10)
			{
				DestroyEffect();
			}
		}
		else if(m_Pet.PetEffectParam.eEffectType==E_MAX_INTIMACY)
		{
			//ģ�е��� max�� �ƴҶ� ����Ʈ����
			if(byIntimacy<C_PET_MAX_GAUGE)
			{
				DestroyEffect();
			}
		}
	}
}
//------------------------------------------------------------------------------
/** �ش� ����Ʈ�� �������ش�.
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
/** ������Ʈ ��ġ ����
*/
void  Pet::UpdatePetEffectPos()
{
	//����Ʈ ������ �׸�������
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
		
	//��������Ʈ 
	CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_Pet.PetEffectParam.hEffect);

	if(pBody)
	{
		pBody->SetShow(!m_Pet.byHideState);
		pBody->SetResultMatrix(wvPos,vRot,vScale);
	}
 
	//�ν��Ͻ�����Ʈ(�̵��ϸ鼭 ����Ʈ�� ���ֹ߻��ϰ� ����Ʈ ������Ÿ���̳ʹ��� ����)
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

