//------------------------------------------------------------------------------
//  NPC.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "NPC.h"
#include "NPCInfoParser.h"
#include "AppearanceManager.h"
#include "Monster.h"
#include "SolarDialog.h"
#include "hero.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "HeroActionInput.h"
#include "NPCVoiceInfoParser.h"
#include "SoundEffect.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorList/uiVendorList.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiitemcompositeMan/uiitemcompositeMan.h"
#include "uiEventMan/uiEventMan.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
#include "NpcImageInfo.h"
#include "EquipmentContainer.h"
#include "AppearanceResourceCode.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningMan.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionMan.h"
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

int NPC::m_sVoiceIndex = 0;

#define	NPC_HP_GAUGE_BAR_WIDTH 119	
#define	NPC_HP_GAUGE_BAR_HEIGHT 5
#define	NPC_HP_LINE_WIDTH 125
#define	NPC_HP_LINE_HEIGHT 11

//------------------------------------------------------------------------------
/**
*/
NPC::NPC()
{
	SetObjectType(NPC_OBJECT);
}

//------------------------------------------------------------------------------
/**
*/
NPC::~NPC()
{
    DestroyCharacterVariation();
}

//------------------------------------------------------------------------------
/**
*/
BOOL NPC::Create( DWORD dwObjectKey, DWORD dwID) 
{
	SetObjectKey( dwObjectKey );

	BASE_NPCINFO *pNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( dwID );

	if (!pNPCInfo)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("NPC::Create() Invalid NPCCode %d"),dwID);
		return FALSE;
	}

	//-----------------------------------------------------------------------	
	// 몬스터 정보 연결
	//-----------------------------------------------------------------------
    SetMonsterInfo( pNPCInfo );
    BOOL rt = Character::Create(dwObjectKey, dwID);
    assert(rt);

    APPEARANCEINFO appearInfo;
    ZeroMemory(&appearInfo,sizeof(appearInfo));

    if (pNPCInfo->m_wNpcImageIndex != 0)
    {
        BASE_NPCImageInfo* pNpcImageInfo = NPCImageInfoParser::Instance()->GetNpcImageInfo(pNPCInfo->m_wNpcImageIndex);
        if (pNpcImageInfo != NULL)
        {
            appearInfo.ObjectType = APPEARANCEINFO::PLAYER;
            ClassCodeEx class_code;
            class_code.class_type = pNpcImageInfo->bClassCode;
            class_code.gender = ResourceCode::GetBaseGenderOfClass(static_cast<eCHAR_TYPE>(class_code.class_type));
            appearInfo.Job = class_code.class_code;

            CreateCharacterVariation(&appearInfo, TRUE);

            EquipmentContainer* equipment_container = new EquipmentContainer();
            equipment_container->SetOwnerKey(GetObjectKey());
            SetEquipmentContainer(equipment_container);

            // 외형세팅
            SetPartVariation(PLAYER_VARIATION_HAIR, pNpcImageInfo->hairImageID);
            SetPartVariation(PLAYER_VARIATION_FACE, pNpcImageInfo->faceImageID);
            BOOL is_hide_helmet = (pNpcImageInfo->showhelmet == 1) ? false : true;
            HideHelmet(is_hide_helmet, FALSE);

            // NPC커스터마이징을 사용하는 경우 스케일 적용
            float npc_scale = static_cast<float>(m_NPCInfo.m_wSize) / 100.0f;
            SetScale(npc_scale);
        }            
    }
    else
    {
        appearInfo.ObjectType = APPEARANCEINFO::MONSTER;
        appearInfo.pMonsterInfo = GetMonsterInfo();
        InitRender(MP_COUNT,MP_BONE,&appearInfo,0,FALSE);
    }

	m_iDelaySwingTick = 0;
	m_pOpenDialogByNPC = NULL;

	//	상태 초기화( IDLE 상태 )
	InitState(STATE::NPC_IDLE_STATE,g_CurTime);

	TCHAR	szString[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString(
	    pNPCInfo->m_NCode, 
	    szString, 
	    MAX_MONSTERNAME_LENGTH);

#ifdef _DEV_VER
    //#ifdef _SCJ_SHOW_NPC_ID
    if (CGeneralGameParam::Instance()->IsShowNpcId())
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
        StrnCopy(buffer_string, szString, INTERFACE_STRING_LENGTH-1); 
        Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
            buffer_string, pNPCInfo->m_MonsterCode);
    }
    //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

	StrCopy( pNPCInfo->m_pszName, szString );


	SetName( pNPCInfo->m_pszName );
	m_iVoiceKind = eNPCVOICEKIND_NONE;

	InitMoveInfo();
	ClearQuestInfo();

    InitMeshHeight();


#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    npc_costume_item_ = 0;
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC

    return TRUE;
}




//------------------------------------------------------------------------------
/**
*/
void NPC::Destroy( void )
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container != NULL)
    {
        delete equipment_container;
        SetEquipmentContainer(NULL);
    }

	Character::Destroy();
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentAttackAnimation()
{
	WzID animationID = StrToWzID("A001");	
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentIdleAnimation()
{
    char idle_animation_id[5] = {'N', '0', '0', '1', '\0'};

    if ( IsCharacterDataCreated() == true )
    {
        idle_animation_id[1] = GetWeaponAnimationCode();
    }

	return StrToWzID(idle_animation_id);
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentSwingAnimation()
{
    char idle_animation_id[5] = {'N', '0', '0', '2', '\0'};

    if( IsCharacterDataCreated() == true )
    {
        idle_animation_id[1] = GetWeaponAnimationCode();
    }

    return StrToWzID(idle_animation_id);
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentMoveAnimation()
{
	WzID animationID = 0;

	if (GetMoveState() == CMS_WALK)
	{
		animationID = StrToWzID("N006");	
	}
	else
	{
		animationID = StrToWzID("N007");
	}

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentDeathAnimation()
{
	WzID animationID = 0;
	animationID = StrToWzID("D001");	
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical)
{
	WzID animationID = 0;
	animationID = StrToWzID("H001");	
	return animationID;
}

BASE_NPCImageInfo* NPC::GetNpcImageInfo()
{
    BASE_NPCImageInfo* npc_image_info = NULL;
    if (m_NPCInfo.m_wNpcImageIndex)
    {
        npc_image_info = NPCImageInfoParser::Instance()->GetNpcImageInfo(m_NPCInfo.m_wNpcImageIndex);
    }
    return npc_image_info;
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentResponseAnimation()
{
    if( IsCharacterDataCreated() == true )
    {
        BASE_NPCImageInfo* npc_image_info = GetNpcImageInfo();
        if (npc_image_info != NULL)
        {
            return npc_image_info->talkAni;
        }
    }
	return StrToWzID("N003");
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentQuestAcceptAnimation()
{
    if( IsCharacterDataCreated() == true )
    {
        BASE_NPCImageInfo* npc_image_info = GetNpcImageInfo();
        if (npc_image_info != NULL)
        {
            return npc_image_info->questAcceptAni;
        }
    }

    return StrToWzID("N003");
}

//------------------------------------------------------------------------------
/**
*/
WzID NPC::GetCurrentQuestCompleteAnimation()
{
    if( IsCharacterDataCreated() == true )
    {
        BASE_NPCImageInfo* npc_image_info = GetNpcImageInfo();
        if (npc_image_info != NULL)
        {
            return npc_image_info->questCompleteAni;
        }
    }
    return StrToWzID("N003");
}

//------------------------------------------------------------------------------
/**
*/
void NPC::SetMonsterInfo(BASE_NPCINFO *pInfo)
{
	m_NPCInfo = *pInfo;  
}

//------------------------------------------------------------------------------
/**
*/
BASE_NPCINFO * NPC::GetMonsterInfo()
{
	return &m_NPCInfo;
}


//------------------------------------------------------------------------------
/**
*/
void NPC::SetName( const tstring& Name )
{
	StrCopy(m_NPCInfo.m_pszName, Name.c_str() );

    Character::SetName( Name );
}

//------------------------------------------------------------------------------
void NPC::RestoreObjectName()
{
    g_InterfaceManager.GetInterfaceString(m_NPCInfo.m_NCode, m_NPCInfo.m_pszName, MAX_MONSTERNAME_LENGTH);

#ifdef _DEV_VER
    //#ifdef _SCJ_SHOW_NPC_ID
    if (CGeneralGameParam::Instance()->IsShowNpcId())
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
        StrnCopy(buffer_string, m_NPCInfo.m_pszName, INTERFACE_STRING_LENGTH-1); 
        Snprintf(m_NPCInfo.m_pszName, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
            buffer_string, m_NPCInfo.m_MonsterCode);
    }
    //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

    SetName(m_NPCInfo.m_pszName);
}


//------------------------------------------------------------------------------
/** Process
*/
BOOL NPC::Process( DWORD dwTick )
{
	// 거리에 따라 열려있는 NPC 창을 닫아준다.
	if (this->m_pOpenDialogByNPC && 
		NULL != g_pHero )
	{
		WzVector vDiff = GetPosition() - g_pHero->GetPosition();
		float distance = VectorLength( &vDiff );
		if ( distance > DISTANCE_NPC_MEET * 0.9f)
		{
			closeOpenedDialog();
		}
	}

	ProcessNPCMove(dwTick);
	ProcessNPCVoice(dwTick);

	return Character::Process(dwTick);
}


//------------------------------------------------------------------------------
/**
*/
BOOL NPC::Render()
{
	if ( GetShow() == FALSE  ) 
		return FALSE;	

	return Character::Render();
}


//------------------------------------------------------------------------------
/**
*/
void NPC::SetExtraInfo( EXTRA_NPCINFO *	pExtraInfo)
{
	m_ExtraInfo = *pExtraInfo;

	SetMoveType(m_ExtraInfo.m_iMoveType);

	switch (m_ExtraInfo.m_iMoveType)
	{
	case eNPCMOVETYPE_PATROL:
		{
			WzVector vTemp;

			vTemp.x = m_ExtraInfo.m_fNPCPos[0];
			vTemp.y = m_ExtraInfo.m_fNPCPos[1];
			vTemp.z = m_ExtraInfo.m_fNPCPos[2];

			SetRouteSize(2);
			SetRoutePos(0,vTemp);

			vTemp.x = m_ExtraInfo.m_fMovePos[0];
			vTemp.y = m_ExtraInfo.m_fMovePos[1];
			vTemp.z = m_ExtraInfo.m_fMovePos[2];

			SetRoutePos(1,vTemp);
			break;

		}

		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
EXTRA_NPCINFO * NPC::GetExtraInfo() 
{
	return &m_ExtraInfo;
}

// 스텟 관련
void NPC::SetExp(EXPTYPE exp)
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
}

//------------------------------------------------------------------------------
/**
*/
void NPC::SetLevel(LEVELTYPE LV)
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
}

//------------------------------------------------------------------------------
/**
*/
LEVELTYPE NPC::GetLevel()
{
	return m_NPCInfo.m_Level;
}

//------------------------------------------------------------------------------
/**
*/
EXPTYPE NPC::GetExp()
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
DWORD NPC::GetMaxHP()
{
	return this->m_NPCInfo.m_dwMaxHP;
}

//------------------------------------------------------------------------------
/**
*/
DWORD NPC::GetMaxMP()
{
	return this->m_NPCInfo.m_dwMaxMP;
}

//------------------------------------------------------------------------------
/**
*/

EXPTYPE NPC::GetNextExp()
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
	return 0;
}

//------------------------------------------------------------------------------
/**
	NPC가 오른한 다이얼로그를 셋해둔다. 
*/
VOID NPC::SetOpenDialog(SolarDialog * pDlg)
{
	assert(pDlg);

	if ((m_pOpenDialogByNPC != NULL) && 
		(m_pOpenDialogByNPC != pDlg) )
	{
		closeOpenedDialog();
	}

	this->m_pOpenDialogByNPC = pDlg;
}


void NPC::SetDelaySwingTick(DWORD dwTick)
{
	m_iDelaySwingTick = CalDelaySwingTick(dwTick);
}

int NPC::ProcessDelaySwingTick(DWORD dwTick)
{
	// 움직이는놈은 Idle애니 안한다.
	if (m_iMoveType)
	{
		return 1;
	}

	return m_iDelaySwingTick -= dwTick;
}

DWORD NPC::CalDelaySwingTick(DWORD dwTick)
{
//	GlobalFunc::InitRandom();
	return NPC_SWING_DEFAULT_DELAY_TERM + GlobalFunc::GetRandom(NPC_SWING_RANDOM_DELAY_TERM);	// 30초 + Random(30초)
}

void NPC::RenderInfo()
{
	const WzColor TextColor = WzColor_RGBA(4,183,150,255);
	const float fShowInfoDistance = 80.0f * 80.0f;

	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
	WzVector vDistance = GetPosition() - pCameraInfo->m_wvFrom;

	BOOL bShow = FALSE;
	if (GAMEOPTION->GetShowNPCName())
	{
		bShow = TRUE;	

		if (VectorLength2(&vDistance) >= fShowInfoDistance)
		{
			bShow = FALSE;

		}
	}
	else
	{
		if (g_HeroInput.GetPickedObjectKey() == GetObjectKey())
		{
			bShow = TRUE;
			if (VectorLength2(&vDistance) >= fShowInfoDistance)
			{
				bShow = FALSE;
			}
		}
	}

#ifdef _DH_NPC_GROUP_NAME
    if (bShow &&g_HeroInput.IsShowPartyMemberOnly()==FALSE && GetNpcInfo()->group_name != 0)
    {
        TCHAR temp_string[256];
        g_InterfaceManager.GetInterfaceString(GetNpcInfo()->group_name, temp_string, 256);
        RenderTextOnHeadGroup(temp_string, TextColor, -16);
    }
#endif//_DH_NPC_GROUP_NAME    
	if (bShow&&g_HeroInput.IsShowPartyMemberOnly()==FALSE) 
    {
		// 머리에 이름그리기
		TCHAR szText[256];			
		Sprintf(szText,_T("%s"),GetName());
		RenderTextOnHead(szText,TextColor,0);
	}

    // NPCExtraInfo 에 따라 출력
    if (GetExtraInfo()->m_eNPCTYPE == eNPC_TYPE_PROTECTS)
    {
        RenderHPGuage();
    }    

	Object::RenderInfo();
}

void NPC::RenderHPGuage( void )
{
    WzVector wzVisualPos = GetVisiblePos();
    wzVisualPos.z += GetMeshHeight();
    wzVisualPos.z += (HEIGHT_GAP * 2);

    float fDepth;
    POINT pt2;
    g_pSunRenderer->Set2DMode( TRUE );
    g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos, &fDepth );		

    RECT rc;
    rc.left = pt2.x - ( NPC_HP_LINE_WIDTH / 2);
    rc.top = pt2.y - 8;//12;//+ 20 - 22;
    rc.right = rc.left + NPC_HP_LINE_WIDTH;
    rc.bottom = rc.top + 17;

    DWORD HPersent = 0;
    if( GetMaxHP() )
    {
        HPersent = ( GetHP() * 100 ) / GetMaxHP();
    }
    else
    {
        assert(!"GetMaxHP() == 0");
    }

    HPersent=min(100,HPersent);
    DWORD HPWidth = ( NPC_HP_GAUGE_BAR_WIDTH * HPersent ) / 100;

    ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
    BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
    BOOL bTest = g_pSunRenderer->IsEnableZTest();
    g_pSunRenderer->SetZBufferTest( TRUE );
    g_pSunRenderer->SetZBufferWrite(TRUE);

    g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hMonsterGauageTexture, (float)rc.left, (float)rc.top, NPC_HP_LINE_WIDTH, NPC_HP_LINE_HEIGHT,
        0.0f, 2.0f, NPC_HP_LINE_WIDTH, NPC_HP_LINE_HEIGHT, 0, fDepth );

    g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hMonsterGauageTexture, (float)rc.left + 3, (float)rc.top + 3, (float)HPWidth, NPC_HP_GAUGE_BAR_HEIGHT,
        3.0f, 20.0f, NPC_HP_GAUGE_BAR_WIDTH, NPC_HP_GAUGE_BAR_HEIGHT, 0, fDepth );

    g_pSunRenderer->SetZBufferTest( bTest );
    g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
    g_pSunRenderer->SetZBufferWrite( bZWrite );	
    g_pSunRenderer->Set2DMode( FALSE );
}

void NPC::InitMeshHeight()
{
    if (GetBasePart())
    {
        WzBoundingVolume* pBV = GetBoundingVolume();

        if (pBV != NULL)
        {
            if (pBV->m_eBvType == BVT_AABB)
            {
                float fHeight = pBV->m_aabb.m_wvMax.z - GetVisiblePos().z;
                SetMeshHeight(fHeight + 0.1f);
            }
        }
    }
}

void NPC::ProcessNPCMove(DWORD dwTick)
{
	const int MOVE_DELAY = 3000;
	const int MOVE_RANDOM = 2000;

	switch (m_iMoveType)
	{
	case eNPCMOVETYPE_PATROL:
		{
			if (GetCurState() != STATE::MOVE)
			{
				m_iMoveWaitDelay -= dwTick;

				if (m_iMoveWaitDelay < 0) 
				{
					m_iCurDest++;
					if (m_iCurDest >= m_iNumRoute)
					{
						m_iCurDest = 0;
					}

					PLAYER_ACTION act;

					ZeroMemory(&act,sizeof(act));

					act.ActionID = ACTION_MOVE;
					act.MOVE.bThrust = FALSE;
					act.MOVE.byState = CMS_WALK;
					act.MOVE.vCurPos.wvPos = GetPosition();
					act.MOVE.vCurPos.wTile = GetPathExplorer()->GetTile();
					act.MOVE.vDestPos.wvPos = GetNextRoutePos();
					act.MOVE.vDestPos.wTile = -1;

					PutAction(&act);

					m_iMoveWaitDelay = MOVE_DELAY + rand() % MOVE_RANDOM;
				}				
			}
		}
		break;

	case eNPCMOVETYPE_RANDOMMOVE:
		{
			if (GetCurState() != STATE::MOVE)
			{
				m_iMoveWaitDelay -= dwTick;

				if (m_iMoveWaitDelay < 0) 
				{
					
					PLAYER_ACTION act;

					ZeroMemory(&act,sizeof(act));

					act.ActionID = ACTION_MOVE;
					act.MOVE.bThrust = FALSE;
					act.MOVE.byState = CMS_WALK;
					act.MOVE.vCurPos.wvPos = GetPosition();
					act.MOVE.vCurPos.wTile = GetPathExplorer()->GetTile();
					act.MOVE.vDestPos.wvPos = GetNextRoutePos();
					act.MOVE.vDestPos.wTile = -1;

					PutAction(&act);

					m_iMoveWaitDelay = MOVE_DELAY + rand() % MOVE_RANDOM;
				}				
			}
		}
		break;
	}	
}

BOOL NPC::ProcessNPCVoice(DWORD dwTick)
{
	m_iWaitSoundDelay -= dwTick;

	if (m_iWaitSoundDelay < 0)
	{
		m_iWaitSoundDelay = 0;
	}

	if( GAMEOPTION->GetNPCVoice() == FALSE )
	{
		return FALSE;
	}

	if (!g_pHero)
	{
		return FALSE;
	}

	WzVector vDistance;

	vDistance = GetPosition() - g_pHero->GetPosition();

	if (m_iVoiceHandle != -1)
	{
		if ( VectorLength2(&vDistance) >= 15.0f * 15.0f )
		{
			StopVoice();			
		}
	}

	if (m_iWaitSoundDelay)
	{
		return FALSE;
	}
		
	BASE_NPCVoiceInfo *pInfo = NPCVoiceInfoParser::Instance()->GetNPCVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (!pInfo)
	{
		return FALSE;
	}

	const float fSightRange = 9.0f;
	const float fDecayRange = 15.0f;
	const int WAIT_SOUND_DELAY = 45000; 

	// 시야 안에들어왔으면!
	if ( VectorLength2(&vDistance) <= fSightRange * fSightRange )
	{
		if (m_iWaitSoundDelay == 0) 
        {
			// 60% 확률로 대사
			if (rand() % 10 < 6) {
				PlayWaitSound();
			}

			m_iWaitSoundDelay = WAIT_SOUND_DELAY; 		
		}
	}
	
	return FALSE;
}

void NPC::PlayLeaveSound()
{
	if( GAMEOPTION->GetNPCVoice() == FALSE )
	{
		return;
	}

	BASE_NPCVoiceInfo *pInfo = NPCVoiceInfoParser::Instance()->GetNPCVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_NPC_VOICE_SAMPLE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);

		if (pInfo->m_Leave[iVoiceIndex].dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(pInfo->m_Leave[iVoiceIndex].dwTextID,szText,512);
			AddChatMessage(szText);
		}

		m_iVoiceKind = eNPCVOICEKIND_LEAVE;

		StopVoice();

		m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Leave[iVoiceIndex].dwVoiceID,NULL);

		return;
	};
}

void NPC::PlayMeetSound()
{
	if( GAMEOPTION->GetNPCVoice() == FALSE )
	{
		return;
	}

	BASE_NPCVoiceInfo *pInfo = NPCVoiceInfoParser::Instance()->GetNPCVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (m_iVoiceKind == eNPCVOICEKIND_MEET)
	{
		return;
	}

	if (pInfo)
	{
		//int iVoiceIndex = rand() % MAX_NPC_VOICE_SAMPLE;
		int iVoiceIndex = NPC::GetNpcVoiceIndex();

		TCHAR szText[512];
		ZeroMemory(szText,512);

		if (pInfo->m_Meet[iVoiceIndex].dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(pInfo->m_Meet[iVoiceIndex].dwTextID,szText,512);
			AddChatMessage(szText);
		}

		m_iVoiceKind = eNPCVOICEKIND_MEET;

		StopVoice();
		
		m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Meet[iVoiceIndex].dwVoiceID,NULL);		

		return;
	};
}

void NPC::PlayWaitSound()
{
	if( GAMEOPTION->GetNPCVoice() == FALSE )
	{
		return;
	}

	BASE_NPCVoiceInfo *pInfo = NPCVoiceInfoParser::Instance()->GetNPCVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (m_iVoiceKind == eNPCVOICEKIND_WAIT)
	{
		return;
	}

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_NPC_VOICE_SAMPLE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);

		if (pInfo->m_Wait[iVoiceIndex].dwTextID)
		{
			g_InterfaceManager.GetInterfaceString(pInfo->m_Wait[iVoiceIndex].dwTextID,szText,512);
			AddChatMessage(szText);
		}

		m_iVoiceKind = eNPCVOICEKIND_WAIT;

		StopVoice();
		
		m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Wait[iVoiceIndex].dwVoiceID,NULL);

		return;
	};
}

void NPC::InitMoveInfo()
{
	m_iMoveType = 0;
	m_iNumRoute = 0;
	m_iCurDest = 0;
	m_iMoveWaitDelay = 0;
	ZeroMemory(m_vRoute,MAX_NPC_ROUTE * sizeof(WzVector));
}

static WzVector GetAngleVector(float fAngle)
{
	WzVector vDirection;
	vDirection.x = sin(fAngle);
	vDirection.y = -cos(fAngle);
	vDirection.z = 0.0f;
	return vDirection;
}

WzVector NPC::GetNextRoutePos()
{
	switch(m_iMoveType)
	{
	case eNPCMOVETYPE_PATROL:
		return m_vRoute[m_iCurDest];

	case eNPCMOVETYPE_RANDOMMOVE:
		{
			DWORD dwAngle = rand() % 628;
#ifdef _DEV_VER
			if(m_ExtraInfo.m_fRange <= 0)
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("NPCExtraInfo.txt->RandumRange == 0"));
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Confirm(szMessage, NULL);
			}
#endif	//	_DEV_VER

			DWORD dwArea = rand() % ( (int)(m_ExtraInfo.m_fRange * 1000.0f));
			float fAngle = (float)dwAngle / 100.0f;
			float fDistance = (float)dwArea / 1000.0f;

			WzVector vDirection = GetAngleVector(fAngle);
			WzVector vTemp;

			vTemp.x = m_ExtraInfo.m_fNPCPos[0];
			vTemp.y = m_ExtraInfo.m_fNPCPos[1];
			vTemp.z = m_ExtraInfo.m_fNPCPos[2];

			WzVector vTargetPos =  vTemp + (vDirection * fDistance);

			return vTargetPos;	
		}

	default:
		return GetPosition();
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void NPC::closeOpenedDialog()
{
	WzID DialogID = g_InterfaceManager.GetDialogWzID( InterfaceManager::DIALOG_SELECT_COMPOSITE_MENU );
	CDialogWZ * pDialog = m_pOpenDialogByNPC->GetDialogWZ();
	if( pDialog == NULL)
		return;

	if(eNPC_TYPE_GUILD == this->GetExtraInfo()->m_eNPCTYPE)
	{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GuildSystem::Function::CloseGuildWareHouse();
        GuildSystem::Function::CloseAllGuildDialog();
#else
        uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
        if(pGuildMan)
        {
            if(pGuildMan->IsGuildRankUpWindow())
            {
                pGuildMan->ShowGuildRankUpWindow(FALSE);
            }

            if(pGuildMan->IsGuildWareHouseWindow())
            {
                pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
            }

            pGuildMan->CloseAllOpenedGuildWindow();
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER		
	}


	if (DialogID == m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() )
	{
		// 조합 메뉴 이면 
		// 기타 랭크 메뉴에서 열수있는 다이얼로그를 닫는다.
		g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_ENCHANT, FALSE );
        if ( g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ITEM_COMPOSITE ) )
		{
			g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ITEM_COMPOSITE )->ShowDialog(FALSE);
		}

		GET_DIALOG( IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE )->ShowInterface( FALSE );
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		GET_DIALOG( IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG )->ShowInterface( FALSE );
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
		GET_DIALOG( IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG )->ShowInterface( FALSE );
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	}

	DialogID = g_InterfaceManager.GetDialogWzID( InterfaceManager::DIALOG_EVENT_WINDOW );
	if(DialogID == m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID())
	{
		GET_DIALOG( IM_EVENT_MANAGER::EVENT_ITEM_SELECT )->ShowInterface( FALSE );
	}

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	if((m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() == IM_MISSION_RANK_MANAGER::MissionRankList )
		|| (m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() == IM_MISSION_RANK_MANAGER::RankGamble))
	{
		if (GET_DIALOG( IM_MISSION_RANK_MANAGER::MissionRankList )->IsVisibleWindow() )
		{
			GET_DIALOG( IM_MISSION_RANK_MANAGER::MissionRankList )->ShowInterface(FALSE);
		}

		if ( GET_DIALOG( IM_MISSION_RANK_MANAGER::RankGamble )->IsVisibleWindow() )
		{
			GET_DIALOG( IM_MISSION_RANK_MANAGER::RankGamble )->ShowInterface(FALSE);
		}
	}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	if((m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() == g_InterfaceManager.GetDialogWzID(InterfaceManager::DIALOG_CASH_ITEM_SHOP_NPC_POPUP)))
	{
		if ( g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASH_ITEM_SHOP_NPC_POPUP)->IsVisibleWindow() )
		{
			g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASH_ITEM_SHOP_NPC_POPUP)->ShowDialog(FALSE);
		}
	}

	DialogID = IM_BANK_MANAGER::BANK_DIALOG;
	if ( DialogID == m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() )
	{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
        uiBankDialog* pBankDialog = (uiBankDialog*)m_pOpenDialogByNPC;
#else
        BankDialog* pBankDialog = (BankDialog*)m_pOpenDialogByNPC;
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM	
		pBankDialog->SendWareHouseEnd();
		m_pOpenDialogByNPC = NULL;
	}
	else if ( 
		m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() == g_InterfaceManager.GetDialogWzID(InterfaceManager::DIALOG_VENDOR_LIST) )
	{
		uiVendorList * pVendorList = GET_CAST_DIALOG ( uiVendorList, IM_VENDOR_MANAGER::VENDOR_LIST );
		if( pVendorList && pVendorList->IsVisibleWindow() )
		{
			pVendorList->NET_SEND_CG_VENDOR_LIST_END_SYN();			
		}	

		m_pOpenDialogByNPC = NULL;
	}
	else
	{
		WzID DialogID = g_InterfaceManager.GetDialogWzID( InterfaceManager::DIALOG_BATTLEZONE_LIST);
		if ( DialogID == m_pOpenDialogByNPC->GetDialogWZ()->GetDlgID() )
		{
			m_pOpenDialogByNPC = NULL;
		}
		else
		{
            if (m_pOpenDialogByNPC->IsVisible() == TRUE)
            {
                m_pOpenDialogByNPC->ShowWindow(FALSE);
            }
			m_pOpenDialogByNPC = NULL;
		}
	}	
}


void NPC::SetRoutePos(int index,WzVector vPos)
{
	if (index < 0 || index >= MAX_NPC_ROUTE)
	{
		assert(!"잘못된 Route Index");
		return;
	}

	m_vRoute[index] = vPos;
}

void NPC::SetMoveType(int iType)
{
	m_iMoveType = iType;
}

void NPC::SetRouteSize(int iSize)
{
	m_iNumRoute = iSize;
}


void NPC::InitNpcImageInfo(Character* character_ptr, BASE_NPCINFO* npc_info)
{
    if (character_ptr == NULL )
    {
        return;
    }

    EquipmentContainer* equipment_container = character_ptr->GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    if (npc_info == NULL )
    {
        return;
    }

    if (npc_info->m_wNpcImageIndex == 0)
    {
        return;
    }


    BASE_NPCImageInfo* pNpcImageInfo = NPCImageInfoParser::Instance()->GetNpcImageInfo(npc_info->m_wNpcImageIndex);
    if (pNpcImageInfo == NULL)
    {
        return;
    }

    // BASE_NPCImageInfo 정보에 따른 장비세팅
    SCItemSlot ItemSlot;
    BYTE item_enchant = 0;
    item_enchant = pNpcImageInfo->Enchant;

    if (pNpcImageInfo->armorCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->armorCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR,ItemSlot);
    }
    if (pNpcImageInfo->bootsCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->bootsCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS,ItemSlot);
    }
    if (pNpcImageInfo->helmetCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->helmetCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_HELMET,ItemSlot);
    }
    if (pNpcImageInfo->gloveCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->gloveCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE,ItemSlot);
    }
    if (pNpcImageInfo->pantsCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->pantsCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_PANTS,ItemSlot);
    }
    if (pNpcImageInfo->weaponCode)
    {
        ItemSlot.SetCode(pNpcImageInfo->weaponCode);
        ItemSlot.SetEnchant(item_enchant);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1,ItemSlot);
    }
    if (pNpcImageInfo->AccCode1)
    {
        ItemSlot.SetCode(pNpcImageInfo->AccCode1);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_SACCESSORY1,ItemSlot);
    }
    if (pNpcImageInfo->AccCode2)
    {
        ItemSlot.SetCode(pNpcImageInfo->AccCode2);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_SACCESSORY2,ItemSlot);
    }
    if (pNpcImageInfo->AccCode3)
    {
        ItemSlot.SetCode(pNpcImageInfo->AccCode3);
        equipment_container->JustInsertSlot(EquipmentContainer::EQUIP_SACCESSORY3,ItemSlot);
    }

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    if (pNpcImageInfo->npc_costume_item)
    {
        character_ptr->SetCostumeItemCode(pNpcImageInfo->npc_costume_item);
    }
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC

    character_ptr->RefreshAppearRender();
}

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
void NPC::SetCostumeItemCode( SLOTCODE item_code )
{
    npc_costume_item_ = item_code;
}

SLOTCODE NPC::GetCostumeItemCode()
{
    return npc_costume_item_;
}
#endif//_NA_008185_20150402_COSTUME_ITEM_FOR_NPC

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
void NPC::SetDecorateCostumeItemCode( SLOTCODE item_code )
{
	npc_decorate_costume_item_ = item_code;
}

SLOTCODE NPC::GetDecorateCostumeItemCode()
{
	return npc_decorate_costume_item_;
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME