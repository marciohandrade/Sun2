#include "SunClientPrecompiledHeader.h"
#include "SummonTargetDialog.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "InterfaceManager.h"
#include "IconImage.h"
#include "StateInfoParser.h"
#include "../Utility/Nstring.h"
#include "Hero.h"
#include "Mouse.h"
#include "DlgPosInfoParser.h"
#include "HeroActionInput.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "uiRadarMan/uiRadarMan.h"
#include "StatusIconManager.h"
#include "QuickContainer.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"

WzID SummonTargetDialog::m_wzId[] = 
{
	StrToWzID("P000"),
	StrToWzID("PI00"),
	StrToWzID("P001"),
	StrToWzID("P002"),
	StrToWzID("P003"),
	StrToWzID("P004"),
	StrToWzID("P005"),
	StrToWzID("P006"),
	StrToWzID("P007"),
	StrToWzID("P008"),
	StrToWzID("P009"),
	StrToWzID("P010"),

	StrToWzID("T000"),
	StrToWzID("PI01"),

	StrToWzID("C000"),
	StrToWzID("C001"),
	StrToWzID("C002"),
	StrToWzID("C003"),
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("C007"),
#else
	StrToWzID("C004"),
#endif
	
	StrToWzID("C005"),
    StrToWzID("S000"),

};

SummonTargetDialog::SummonTargetDialog(InterfaceManager* pUIMan)
:uiBase(pUIMan)
,render_over_texture_(INVALID_HANDLE_VALUE)
{
}


SummonTargetDialog::~SummonTargetDialog(void)
{
}


VOID SummonTargetDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_dwPrevTargetKEY = 0;
	m_dwTargetKEY = 0;

    m_fGaugeSize = (float)getControl(SUMMON_TARGET_PIC_GAUGE)->GetSize().right;


    SetHP( 100 ,1.0f);

	status_icon_container_.reserve(10);
	CCtrlButtonCheckWZ   * pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_ATTACK);
	pCheckControl->SetCheckState(FALSE);

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_FOLLOW);
	pCheckControl->SetCheckState(FALSE);

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_DELEGATE_ATTACK);
	pCheckControl->SetCheckState(FALSE);

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_DELEGATE_DEFENSE);
	pCheckControl->SetCheckState(FALSE);

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_DESTROY);
	pCheckControl->SetCheckState(FALSE);

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_NON_PK_ATTACK);
	pCheckControl->SetCheckState(FALSE);


#ifdef _NA_000000_20130114_RENEWER_UI

	for (int i = 0 ; i < 6 ; ++i)
	{
		render_over_iamge_[i].is_over_mouse = false;

		render_over_iamge_[i].image_source_.left = 471;
		render_over_iamge_[i].image_source_.top = 135 + (i*15);
		render_over_iamge_[i].image_source_.right = 15;
		render_over_iamge_[i].image_source_.bottom = 15;
	}

	render_over_iamge_[0].screen_position = SUMMON_TARGET_CHECK_NON_PK_ATTACK;
	render_over_iamge_[1].screen_position = SUMMON_TARGET_CHECK_ATTACK;
	render_over_iamge_[2].screen_position = SUMMON_TARGET_CHECK_FOLLOW;
	render_over_iamge_[3].screen_position = SUMMON_TARGET_CHECK_DELEGATE_ATTACK;
	render_over_iamge_[4].screen_position = SUMMON_TARGET_CHECK_DELEGATE_DEFENSE;
	render_over_iamge_[5].screen_position = SUMMON_TARGET_CHECK_DESTROY;

	//5번이미지 위치만 좌표가 안맞음
	render_over_iamge_[5].screen_position = SUMMON_TARGET_CHECK_DESTROY;
	render_over_iamge_[5].image_source_.left = 381;
	render_over_iamge_[5].image_source_.top = 60;
	render_over_iamge_[5].image_source_.right = 15;
	render_over_iamge_[5].image_source_.bottom = 15;

	if (render_over_texture_==INVALID_HANDLE_VALUE)
	{
		//너무 긴 라인 줄임
		render_over_texture_ = g_pSunRenderer->x_pManagerTexture->\
			LoadTexture("Data\\Interface\\all_butten4.tga" 
			,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}


#endif
}


VOID SummonTargetDialog::Release()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	if (render_over_texture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(render_over_texture_);
		render_over_texture_ = INVALID_HANDLE_VALUE;
	}
#endif
}

VOID SummonTargetDialog::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * SummonTargetDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


POSTYPE SummonTargetDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

VOID SummonTargetDialog::SetHP( float fHPPercent ,float fRadio)
{
	if(100.f < fHPPercent)
	{
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[Warning]SummonTargetDialog::HPPercent = %f", fHPPercent);
#endif 
		fHPPercent = 100.f;
	}

    RECT rect;
    rect = getControl(SUMMON_TARGET_PIC_GAUGE)->GetSizeRect();

    float width2;
    float height = 0;
	float height2 = 0;


	RECT rcGauge2 = getControl(SUMMON_TARGET_PIC_GAUGE)->GetSize();
	height2 = (float)rcGauge2.bottom;
	rcGauge2 = getControl(SUMMON_TARGET_PIC_GAUGE)->GetSize();


	RECT rcDlg;
	GetDialogWZ()->GetClientRect(&rcDlg);

    if (fHPPercent >= 0.0f )
    {
		width2 = (m_fGaugeSize*fRadio/100.f) * fHPPercent;
		float pos=14.0f;
		pos*=fRadio;

		getControl(SUMMON_TARGET_PIC_GAUGE)->MoveWindowWZ( 
			(float)rcGauge2.left, 
			(float)rcGauge2.top, 
			(float)width2, 
			(float)height2 );
	}
}


VOID SummonTargetDialog::SetMP( float fMpPercent ,float fRadio)
{
	assert( fMpPercent <= 100 );


	RECT rect;
	rect = getControl(SUMMON_TARGET_PIC_MP_GAUGE)->GetSizeRect();
	float width2;

	float height = 0;
	float height2 = 0;


	RECT rcGauge2 = getControl(SUMMON_TARGET_PIC_MP_GAUGE)->GetSize();
	height2 = (float)rcGauge2.bottom;
	rcGauge2 = getControl(SUMMON_TARGET_PIC_MP_GAUGE)->GetSize();


	RECT rcDlg;
	GetDialogWZ()->GetClientRect(&rcDlg);

	if (fMpPercent >= 0.0f )
	{
		width2 = (m_fGaugeSize*fRadio/100.f) * fMpPercent;
		float pos=14.0f;
		pos*=fRadio;

		getControl(SUMMON_TARGET_PIC_MP_GAUGE)->MoveWindowWZ( 
			(float)rcGauge2.left, 
			(float)rcGauge2.top, 
			(float)width2, 
			(float)height2 );
	}
}

VOID SummonTargetDialog::SetTargetInfo()
{
	Object * pTargetObject = g_ObjectManager.GetObject(m_dwTargetKEY);

	if (pTargetObject && g_pHero)
	{

		if (m_dwPrevTargetKEY != m_dwTargetKEY)
		{
			m_dwPrevTargetKEY = m_dwTargetKEY;
			g_InterfaceManager.UpdateSummonTargetHeadTexture();
		}
		CCtrlStaticWZ * pStaticControl = (CCtrlStaticWZ *)getControl(SUMMON_TARGET_TEXT_INFO);
		assert(pStaticControl);

        CCtrlStaticWZ * pStaticControl2 = (CCtrlStaticWZ *)getControl(SUMMON_TARGET_TEXT_INFO2);
        assert(pStaticControl2);
        

		CControlWZ * pHPControl = getControl(SUMMON_TARGET_PIC_GAUGE);
		
		if (pTargetObject->IsKindOfObject(MONSTER_OBJECT))
		{
			Character * pChar = static_cast<Character *>(pTargetObject);

			float maxhp =(float)pChar->GetMaxHP();
			float curhp =(float)pChar->GetHP();


			float maxmp = (float)pChar->GetMaxMP();
			float curmp = (float)pChar->GetMP();//몬스터는 mp가 없다.

			int         stringId=0;

			float fRadio=1.0f;

			if (0 == curhp)
			{
				g_pHero->RemoveSummonId(m_dwTargetKEY);
				// 아직 정의된것이없다.
				SolarDialog::ShowWindow(FALSE);
			}

			nString str;
			
			float percent=( (float)curhp / (float)maxhp ) * 100.0f;

			Sprintf(m_szResultInfo, _T("%ld%%"),
				(DWORD)percent);	

			GlobalFunc::SetCaption( pStaticControl, m_szResultInfo );

            percent=( (float)curmp / (float)maxmp ) * 100.0f;
            Sprintf(m_szResultInfo, _T("%ld%%"),
                (DWORD)percent);	
            GlobalFunc::SetCaption(pStaticControl2,m_szResultInfo);

            assert(maxhp > 0);
            if (curhp > maxhp)
                curhp = maxhp;

			assert(maxmp > 0);
			if (curmp > maxmp)
				curmp = maxmp;


           SetHP( (curhp / maxhp ) * 100.0f,1.0f);

		   SetMP( ( curmp / maxmp ) * 100.0f,1.0f);


        }
		else
		{
			SolarDialog::ShowWindow(FALSE);
		}
	}
	else
	{
		SolarDialog::ShowWindow(FALSE);
	}
}

VOID SummonTargetDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
        MoveDefaultWindowPos();

		SetTargetInfo();

		m_oldIndex=-1;

		m_dwPacketStatus=0;
	}
	else
	{
		_asm nop;
	}
}

VOID SummonTargetDialog::SetTargetKey(DWORD dwTargetKEY)
{
	m_dwTargetKEY = dwTargetKEY;
}

VOID SummonTargetDialog::OnUpdateSolarDialog()
{
	SetTargetInfo();
}

VOID SummonTargetDialog::OnRenderSolarDialog()
{
	if( g_pHero == NULL)
	{
		return;
	}

    int a;
	RECT rc;		
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

#ifdef _NA_000000_20130114_RENEWER_UI
	RenderMouseOver();
#endif

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	for (a = SUMMON_TARGET_CHECK_ATTACK; a <= SUMMON_TARGET_CHECK_NON_PK_ATTACK; a++)
	{
		CControlWZ* pControl = getControl((POSTYPE)a);
		rc = pControl->GetSizeRect();

		if ((rc.left <= iMouseX) && (rc.right >= iMouseX))
		{
			if ((rc.top <= iMouseY) && (rc.bottom >= iMouseY))
			{
				switch (a)
				{
				case SUMMON_TARGET_CHECK_ATTACK:
					{
						//	수동 공격
						g_InterfaceManager.GetInterfaceString(11259, szMessage, INTERFACE_STRING_LENGTH);

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)	
                        {
							pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
																		20 + 2,
																		StrToWzID("m212"),
																		WzColor_RGBA( 253, 183, 0, 255 ),
																		TOOLTIP_BG_WZCOLOR,
																		TP_VCENTER|TP_HCENTER);
							pUIMan->SetTooltipDialogID(GetDialogKey());
                        }
					}
					break;

				case SUMMON_TARGET_CHECK_FOLLOW:
					{	
						//	플레이어추적
							int stringcode = 11297;
							g_InterfaceManager.GetInterfaceString(stringcode, szMessage);

							uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
							if (pUIMan)
                            {
								pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
																	20 + 2,
																	StrToWzID("m212"),
																	WzColor_RGBA( 253, 183, 0, 255 ),
																	TOOLTIP_BG_WZCOLOR,
																	TP_VCENTER|TP_HCENTER);
								pUIMan->SetTooltipDialogID(GetDialogKey());
                            }

					}
					break;

				case SUMMON_TARGET_CHECK_DELEGATE_ATTACK:
					{
						//	공격 모드
						g_InterfaceManager.GetInterfaceString( 11261, szMessage, INTERFACE_STRING_LENGTH );

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)
						{
							pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
																20 + 2,
																StrToWzID("m212"),
																WzColor_RGBA( 253, 183, 0, 255 ),
																TOOLTIP_BG_WZCOLOR,
																TP_VCENTER|TP_HCENTER);
							pUIMan->SetTooltipDialogID(GetDialogKey());
						}
					}
					break;

				case SUMMON_TARGET_CHECK_DELEGATE_DEFENSE:
					{
						//	방어 모드
						g_InterfaceManager.GetInterfaceString( 11262, szMessage, INTERFACE_STRING_LENGTH );

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)						
						{
							pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
																20 + 2,
																StrToWzID("m212"),
																WzColor_RGBA( 253, 183, 0, 255 ),
																TOOLTIP_BG_WZCOLOR,
																TP_VCENTER|TP_HCENTER);
							pUIMan->SetTooltipDialogID(GetDialogKey());

						}


					}
					break;

				case SUMMON_TARGET_CHECK_DESTROY:
					{
						//	소환 해제
						g_InterfaceManager.GetInterfaceString( 11263, szMessage, INTERFACE_STRING_LENGTH );

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)
						{
							pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
																20 + 2,
																StrToWzID("m212"),
																WzColor_RGBA( 253, 183, 0, 255 ),
																TOOLTIP_BG_WZCOLOR,
																TP_VCENTER|TP_HCENTER);
							pUIMan->SetTooltipDialogID(GetDialogKey());
						}
					}
					break;

				case SUMMON_TARGET_CHECK_NON_PK_ATTACK:
					{
						// non pk mode
						g_InterfaceManager.GetInterfaceString(11298, szMessage);

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if (pUIMan)						
                        {
							pUIMan->RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
							                                    20 + 2,
							                                    StrToWzID("m212"),
							                                    WzColor_RGBA(253, 183, 0, 255),
							                                    TOOLTIP_BG_WZCOLOR,
							                                    TP_VCENTER|TP_HCENTER);
							pUIMan->SetTooltipDialogID(GetDialogKey());

                        }
					}
					break;

				}
			}
		}		
	}


}

void SummonTargetDialog::UpdateStatusIcon()
{
    status_icon_container_.clear();

    Character* summon_character = static_cast<Character*>(g_ObjectManager.GetObject(m_dwTargetKEY));
    if ((summon_character == NULL) ||
        (summon_character->IsKindOfObject(CHARACTER_OBJECT) != TRUE))
    {
        return;
    }

    int mouse_x_position = Mouse::Instance()->GetMouseX();
    int mouse_y_position = Mouse::Instance()->GetMouseY();

    // 타겟 의 SKILL_EFFECT 를 순회
    BASE_STATEINFO* state_info = NULL;
    SKILL_EFFECT_IMAGE status_icon;
    DWORD hero_object_key = m_dwTargetKEY;
    int status_Index = 0;
    CControlWZ* buff_control = NULL;


    std::vector<SKILL_EFFECT*>& skill_effect_array = summon_character->GetSkillEffectArray();
    for( size_t i=0;i!=skill_effect_array.size();++i)
    {
        SKILL_EFFECT* skill_effect = skill_effect_array[i];

        state_info = StateInfoParser::Instance()->GetStateInfo(skill_effect->dwStatusID);
        status_icon.iEffectID = skill_effect->iEffectID;
        status_icon.dwStatusID = skill_effect->dwStatusID;
        status_icon.dwOwnerID = hero_object_key;
        status_icon.pSkillEffect = skill_effect;
        status_icon.pStateInfo = state_info;
        status_icon.m_byAlpha = 0xFF;
        // draw_rect 계산
        buff_control = getControl(SUMMON_TARGET_SKILL_EFFECT1 + status_Index);
        if (buff_control != NULL)
        {
            status_icon.rcDrawRect = buff_control->GetSizeRect();

            // 마우스 오버체크
            if ((status_icon.rcDrawRect.left <= mouse_x_position) && 
                (status_icon.rcDrawRect.right >= mouse_x_position) &&
                (status_icon.rcDrawRect.top <= mouse_y_position) && 
                (status_icon.rcDrawRect.bottom >= mouse_y_position))
            {
                // 툴팁 출력
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
                if (tooltip_manager)
                {
                    tooltip_manager->RegisterSkillEffectInfoTooltip(&status_icon, TRUE);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
            status_icon_container_.push_back(status_icon);
        }        

        ++status_Index;
        if (status_Index >= StatusIconManager::MAX_SUMMON_ICON)
        {
            break;
        }
    }

    // status_icon_manager에 삽입
    StatusIconManager* status_icon_manager = g_InterfaceManager.GetStatusIconManager();
    if (status_icon_manager)
    {
        status_icon_manager->RegisterStatusIconInfo(status_icon_container_);
    }
}

BOOL SummonTargetDialog::SendSummonCommandSyn( BYTE CommandType )
{
	if( BIT_CHECK( m_dwPacketStatus, GetBit( CommandType ) ) == FALSE )
	{
		if( g_pHero && g_pHero->GetSummonId() != NONE_ID )
		{
			MSG_CG_SUMMON_COMMAND_SYN	Packet;
			Packet.m_byCategory		= CG_SUMMON;
			Packet.m_byProtocol		= CG_SUMMON_COMMAND_SYN;		
			Packet.m_byCommand		= CommandType;	
			
			BIT_ADD( m_dwPacketStatus, GetBit( CommandType ) );

			if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
			{
				assert(!" 패킷 전송 실패");
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL  SummonTargetDialog::CommandSummonMoster(eSUMMON_TARGET_POS pos)
{
	CControlWZ* pControl = getControl(pos);
	if (!pControl)
	{
		return FALSE;
	}

	switch (pos)
	{

	case SUMMON_TARGET_CHECK_NON_PK_ATTACK:
		{
			CCtrlButtonCheckWZ* pCheckControl = (CCtrlButtonCheckWZ*)pControl;

			SendSummonCommandSyn(eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER);
		}
		break;

	case SUMMON_TARGET_CHECK_ATTACK:
		{
			CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
			if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_ATTACK)==FALSE)
			{
				MSG_CG_SUMMON_COMMAND_SYN	Packet;
				Packet.m_byCategory = CG_SUMMON;
				Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
				Packet.m_byCommand = eSUMMON_COMMAND_ATTACK;	
				Packet.m_dwTargetKey=g_HeroInput.GetCurrentTarget();
				Character* pEnemy=(Character*)g_ObjectManager.GetObject(Packet.m_dwTargetKey);


				Packet.m_byAttackPropensity=g_HeroInput.lsForceAttack();

				if(GameFramework::GetCurrentScene()&&
					GameFramework::GetCurrentScene()->IsEnablePkScene())
				{
				
					if(pEnemy&&pEnemy->IsKindOfObject(MONSTER_OBJECT))
					{
						Monster *pMon=(Monster *)pEnemy;

						if(pMon)
						{
							Player *pPlayer=(Player *)g_ObjectManager.GetObject(pMon->GetParentId());

							if(pPlayer==NULL)
								Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;

						}

					}
				}
				else
					Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;


				if( g_pHero && pEnemy &&
					GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pEnemy, g_HeroInput.lsForceAttack() ) == RELATIONKIND::Enemy &&
					pEnemy->IsDead() == FALSE )
				{
					BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_ATTACK);

					if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
					{
						assert(!"패킷 전송 실패");
					}
				}
				else
				{
					TCHAR szFailMessage[512];
					g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NO_TARGET,szFailMessage,MAX_PATH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);

				}
			}

		}
		return TRUE;
	case SUMMON_TARGET_CHECK_FOLLOW:
		{
			CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
			if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_FOLLOW)==FALSE)
			{
				if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
				{
					MSG_CG_SUMMON_COMMAND_SYN	Packet;
					Packet.m_byCategory = CG_SUMMON;
					Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
					Packet.m_byCommand = eSUMMON_COMMAND_FOLLOW;	
					BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_FOLLOW);

					if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
					{
						assert(!"패킷 전송 실패");
					}
				}
			}

		}
		return TRUE;



	case SUMMON_TARGET_CHECK_DELEGATE_ATTACK:
		{
			CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
			if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK)==FALSE)
			{
				if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
				{
					MSG_CG_SUMMON_COMMAND_SYN	Packet;
					Packet.m_byCategory = CG_SUMMON;
					Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
					Packet.m_byCommand =  eSUMMON_COMMAND_DELEGATE_ATTACK;	
					BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK);

					if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
					{
						assert(!" 패킷 전송 실패");
					}
				}
			}
		}
		return TRUE;


	case SUMMON_TARGET_CHECK_DELEGATE_DEFENSE:
		{
			CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
			if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE)==FALSE)
			{
				if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
				{
					MSG_CG_SUMMON_COMMAND_SYN	Packet;
					Packet.m_byCategory = CG_SUMMON;
					Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
					Packet.m_byCommand = eSUMMON_COMMAND_DELEGATE_DEFENSE;	
					BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE);
					if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
					{
						assert(!" 패킷 전송 실패");
					}
				}
			}

		}
		return TRUE;


	case SUMMON_TARGET_CHECK_DESTROY:
		{
			CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
			if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DESTROY)==FALSE)
			{
				if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
				{
					MSG_CG_SUMMON_COMMAND_SYN	Packet;
					Packet.m_byCategory = CG_SUMMON;
					Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
                    Packet.m_dwTargetKey = 0;   // 타겟키에 오브젝트ID를 넣으면 해당 ID만 역소환
					Packet.m_byCommand = eSUMMON_COMMAND_DESTROY;	
					BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DESTROY);
					if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
					{
						assert(!" 패킷 전송 실패");
					}
				}

			}
		}
		return TRUE;


	}


	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
VOID SummonTargetDialog::MessageProc( SI_MESSAGE * pMessage )
{

	switch( pMessage->eResultMsg ) 
	{

	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}
#ifdef _NA_000000_20130114_RENEWER_UI
			switch (pos)
			{
			case SUMMON_TARGET_CHECK_ATTACK:
			case SUMMON_TARGET_CHECK_FOLLOW:
			case SUMMON_TARGET_CHECK_DELEGATE_ATTACK:
			case SUMMON_TARGET_CHECK_DELEGATE_DEFENSE:
			case SUMMON_TARGET_CHECK_DESTROY:
			case SUMMON_TARGET_CHECK_NON_PK_ATTACK:
				{
					for (int i = 0 ; i < 6 ; ++i)
					{
						if(render_over_iamge_[i].screen_position == pos)
						{
							render_over_iamge_[i].is_over_mouse = true;
						}
						else
						{
							render_over_iamge_[i].is_over_mouse = false;
						}
					}
				}
				break;
			default:
				{
					for (int i = 0 ; i < 6 ; ++i)
					{
						render_over_iamge_[i].is_over_mouse = false;
					}
				}
				break;
			}
#endif


		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			switch (pos)
			{
	
			case SUMMON_TARGET_CHECK_ATTACK:
				{
					CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
					if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_ATTACK)==FALSE)
					{
						MSG_CG_SUMMON_COMMAND_SYN	Packet;
						Packet.m_byCategory = CG_SUMMON;
						Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
						Packet.m_byCommand = eSUMMON_COMMAND_ATTACK;	
						Packet.m_dwTargetKey=g_HeroInput.GetCurrentTarget();

						Character* pEnemy=(Character*)g_ObjectManager.GetObject(Packet.m_dwTargetKey);


						Packet.m_byAttackPropensity=g_HeroInput.lsForceAttack();

						if(GameFramework::GetCurrentScene()&&
							GameFramework::GetCurrentScene()->IsEnablePkScene())
						{

							if(pEnemy&&pEnemy->IsKindOfObject(MONSTER_OBJECT))
							{
								Monster *pMon=(Monster *)pEnemy;

								if(pMon)
								{
									Player *pPlayer=(Player *)g_ObjectManager.GetObject(pMon->GetParentId());

									if(pPlayer==NULL)
										Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;

								}

							}
						}
						else
							Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;

						if(g_pHero && pEnemy &&
							GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pEnemy, g_HeroInput.lsForceAttack() ) == RELATIONKIND::Enemy &&
							pEnemy->IsDead()==FALSE)
						{
							BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_ATTACK);

							if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
							{
								assert(!"패킷 전송 실패");
							}
						}
						else
						{
							TCHAR szFailMessage[512];
							g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NO_TARGET,szFailMessage,MAX_PATH);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);
							
						}
					}
					
					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());


				}
				break;


			case SUMMON_TARGET_CHECK_FOLLOW:
				{
					CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
					if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_FOLLOW)==FALSE)
					{
						if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
						{
							MSG_CG_SUMMON_COMMAND_SYN	Packet;
							Packet.m_byCategory = CG_SUMMON;
							Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
							Packet.m_byCommand = eSUMMON_COMMAND_FOLLOW;	
							BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_FOLLOW);

							if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
							{
								assert(!"패킷 전송 실패");
							}
						}
					}
					
					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());

				}
				break;


			case SUMMON_TARGET_CHECK_DELEGATE_ATTACK:
				{
					CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
					if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK)==FALSE)
					{
						if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
						{
							MSG_CG_SUMMON_COMMAND_SYN	Packet;
							Packet.m_byCategory = CG_SUMMON;
							Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
							Packet.m_byCommand =  eSUMMON_COMMAND_DELEGATE_ATTACK;	
							BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK);

							if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
							{
								assert(!" 패킷 전송 실패");
							}
						}
					}
					
					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());

				}
				break;

			case SUMMON_TARGET_CHECK_NON_PK_ATTACK:
				{
					CCtrlButtonCheckWZ* pCheckControl = (CCtrlButtonCheckWZ*)pControl;
					
					SendSummonCommandSyn(eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER);

					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());
				}
				break;


			case SUMMON_TARGET_CHECK_DELEGATE_DEFENSE:
				{
					CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
					if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE)==FALSE)
					{
						if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
						{
							MSG_CG_SUMMON_COMMAND_SYN	Packet;
							Packet.m_byCategory = CG_SUMMON;
							Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;		
							Packet.m_byCommand = eSUMMON_COMMAND_DELEGATE_DEFENSE;	
							BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE);
							if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
							{
								assert(!" 패킷 전송 실패");
							}
						}
					}
					
					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());
				}
				break;

			case SUMMON_TARGET_CHECK_DESTROY:
				{
					CCtrlButtonCheckWZ  * pCheckControl=(CCtrlButtonCheckWZ *)pControl;
					if(BIT_CHECK(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DESTROY)==FALSE)
					{
						if(g_pHero&&g_pHero->GetSummonId()!=NONE_ID)
						{
							MSG_CG_SUMMON_COMMAND_SYN	Packet;
							Packet.m_byCategory = CG_SUMMON;
							Packet.m_byProtocol = CG_SUMMON_COMMAND_SYN;	
                            Packet.m_dwTargetKey = 0; // 타겟키에 오브젝트ID를 넣으면 해당 ID만 역소환
							Packet.m_byCommand = eSUMMON_COMMAND_DESTROY;	
							BIT_ADD(m_dwPacketStatus,PACKET_STATUS_SUMMONTARGET_DESTROY);
							if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_COMMAND_SYN) ) )
							{
								assert(!" 패킷 전송 실패");
							}
						}
						
					}
					
					pCheckControl->SetCheckState(!pCheckControl->GetCheckState());
				}
				break;

			}
		}
		break;
	};
}


BYTE SummonTargetDialog::ConvertCommandPos(BYTE bCommand)
{
	if (bCommand == eSUMMON_COMMAND_FOLLOW)
	{
		return SUMMON_TARGET_CHECK_FOLLOW;
	}
	else if (bCommand == eSUMMON_COMMAND_ATTACK)
	{
		return SUMMON_TARGET_CHECK_ATTACK;
	}
	else if (bCommand == eSUMMON_COMMAND_DELEGATE_ATTACK)
	{
		return SUMMON_TARGET_CHECK_DELEGATE_ATTACK;
	}
	else if (bCommand == eSUMMON_COMMAND_DELEGATE_DEFENSE)
	{
		return SUMMON_TARGET_CHECK_DELEGATE_DEFENSE;
	}
	else if (bCommand == eSUMMON_COMMAND_DESTROY)
	{
		return SUMMON_TARGET_CHECK_DESTROY;
	}
	else if (bCommand == eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER)
	{
		return SUMMON_TARGET_CHECK_NON_PK_ATTACK;
	}

	return 0;
}

void SummonTargetDialog::ResetCheckButton(BYTE bCommand)
{
	BYTE pos = ConvertCommandPos(bCommand);
	BIT_REMOVE(m_dwPacketStatus, GetBit(pos));

	if (pos != SUMMON_TARGET_CHECK_ATTACK)
	{
		for (int i = SUMMON_TARGET_CHECK_FOLLOW; i <= SUMMON_TARGET_CHECK_NON_PK_ATTACK; ++i)
		{
			CCtrlButtonCheckWZ* pCheckControl = (CCtrlButtonCheckWZ*)getControl(i);
			if (pCheckControl)
            {
                pCheckControl->SetCheckState(FALSE);
            }
		}

		if (m_oldIndex != -1)
		{
			CCtrlButtonCheckWZ* pCheckControl = (CCtrlButtonCheckWZ*)getControl(m_oldIndex);
			if (pCheckControl)
            {
                pCheckControl->SetCheckState(TRUE);	
            }
		}
	}
}

void SummonTargetDialog::ResetOhterCheckButton(BYTE bCommand)
{
	BYTE pos = ConvertCommandPos(bCommand);
	
	BIT_REMOVE(m_dwPacketStatus, GetBit(pos));

	if (pos != SUMMON_TARGET_CHECK_ATTACK)
	{
		for (int i = SUMMON_TARGET_CHECK_FOLLOW; i <= SUMMON_TARGET_CHECK_NON_PK_ATTACK; ++i)
		{
			CCtrlButtonCheckWZ* pCheckControl = (CCtrlButtonCheckWZ*) getControl(i);

			if (i == pos)
			{
				pCheckControl->SetCheckState(TRUE);	
				m_oldIndex = i;
			}
			else
			{
				pCheckControl->SetCheckState(FALSE);	
			}
		}
	}
}


DWORD SummonTargetDialog::GetBit(BYTE bPos)
{
	DWORD dwBit = 0;
	switch (bPos)
	{
	case SUMMON_TARGET_CHECK_ATTACK:
		dwBit = PACKET_STATUS_SUMMONTARGET_ATTACK;
		break;

	case SUMMON_TARGET_CHECK_FOLLOW:
		dwBit = PACKET_STATUS_SUMMONTARGET_FOLLOW;
		break;

	case SUMMON_TARGET_CHECK_DELEGATE_ATTACK:
		dwBit = PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK;
		break;

	case SUMMON_TARGET_CHECK_DELEGATE_DEFENSE:
		dwBit = PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE;
		break;

	case SUMMON_TARGET_CHECK_DESTROY:
		dwBit = PACKET_STATUS_SUMMONTARGET_DESTROY;
		break;

	case SUMMON_TARGET_CHECK_NON_PK_ATTACK:
		dwBit = PACKET_STATUS_SUMMONTARGET_NON_PK_ATTACK;
		break;

	}

	return dwBit;
}

void SummonTargetDialog::MoveDefaultWindowPos()
{
    if (g_pHero != NULL)
    {
        QuickContainer* quick_container = g_pHero->GetQuickContainer();

        int quick_size = 1;
        if (quick_container != NULL)
        {
            if (quick_container->double_size() == true)
            {
                quick_size = 2;
            }
            //else // int quick_size = 1
            uiSummonTargetMan* ui_summon_target_man = 
                GET_DIALOG_MANAGER(uiSummonTargetMan, UIMAN_SUMMONTARGET);
            if (ui_summon_target_man != NULL)
            {
                ui_summon_target_man->MoveSummonTargetDialog(quick_size);
            }                        
        }
    }
}
#ifdef _NA_000000_20130114_RENEWER_UI
void SummonTargetDialog::RenderMouseOver()
{
	CCtrlButtonCheckWZ   * pCheckControl = NULL;
	CCtrlButtonCheckWZ   * pCheckControl2 = NULL;

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_NON_PK_ATTACK);
	pCheckControl2 = (CCtrlButtonCheckWZ  *) getControl(SUMMON_TARGET_CHECK_DESTROY);
	//첫 컨트롤 버튼과 마지막 컨트롤 버튼의 길이값을 구한다.
	if(pCheckControl && pCheckControl2)
	{
		isFillRectPos.left = pCheckControl->GetSizeRect().left;
		isFillRectPos.top = pCheckControl->GetSizeRect().top;

		isFillRectPos.right = pCheckControl2->GetSizeRect().right;
		isFillRectPos.bottom = pCheckControl2->GetSizeRect().bottom;
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	if ((isFillRectPos.left <= iMouseX) && (isFillRectPos.right >= iMouseX))
	{
		if ((isFillRectPos.top <= iMouseY) && (isFillRectPos.bottom >= iMouseY))
		{
			for (int i = 0 ; i < 6 ; ++i)
			{
				CCtrlButtonCheckWZ   * pCheckControl = (CCtrlButtonCheckWZ  *) getControl(render_over_iamge_[i].screen_position);
				
				if(pCheckControl && pCheckControl->GetCheckState() == false)
				{
					if(render_over_iamge_[i].is_over_mouse)
					{
						g_pSunRenderer->RenderTexture(render_over_texture_,
							(float)pCheckControl->GetSizeRect().left,
							(float)pCheckControl->GetSizeRect().top,
							15.0f,
							15.0f,
							(float)render_over_iamge_[i].image_source_.left,
							(float)render_over_iamge_[i].image_source_.top,
							15.0f,
							15.0f);
					}
					
				}
				
			}
			
		}
	}
}
#endif