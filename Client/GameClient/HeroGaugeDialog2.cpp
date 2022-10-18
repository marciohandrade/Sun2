

#include "SunClientPrecompiledHeader.h"
#include "HeroGaugeDialog2.h"
#include "Hero.h"
#include "CommonCharacterFormula.h"
#include "InterfaceManager.h"
#include "Mouse.h"
#include "HeroActionInput.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"

#include "StateInfoParser.h"
#include "GameOptionParser.h"
#include "StatusIconManager.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif
#ifdef _DH_BATTLEZONE2_
  #include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif //_DH_BATTLEZONE2_

#include "uiPerkDialog/uiPerkDialog.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SCPerkSlot.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"
#include "HeroGaugeDialog2Control.h"
#include "uiPetGauge/uiPetGauge.h"
#include "uiPetMan/uiPetMan.h"

static const int HIT_EFFECT_TICK_PER_FRAME = 40;
static const int HIT_EFFECT_FRAME_COUNT  = 3;
static const float HIT_EFFECT_SIZE = 16;

const float MAX_RATIO	= 0.9f;

#define	TOOLTIP_STRING_NOTE_CODE	5798	
#define BLINK_RANGE_TICK	500

#pragma warning ( push )
#pragma warning(disable : 4800)

#ifdef _NA_000000_20130114_RENEWER_UI

//-si-
// 11_HP.iwz 의 컨트롤 설정
WzID HeroGaugeDialog2::m_wzId[HGAUGE_MAX] = 
{ 
	StrToWzID("S000"),//eControl_Static_SD_S000,
	StrToWzID("S001"),//eControl_Static_HP_S001,
	StrToWzID("S002"),//eControl_Static_MP_S002,
	StrToWzID("S003"),//eControl_Static_None_S003

	StrToWzID("P024"),//eControl_Picture_SD_P024,
	StrToWzID("P025"),//eControl_Picture_HP_P025,
	StrToWzID("P026"),//eControl_Picture_MP_P026,
	StrToWzID("P027"),//eControl_Picture_SP_P027,

	StrToWzID("B001"),//eControl_Button_PasSlot_B001,
	StrToWzID("B002"),//eControl_Button_PasSlot_B002,
	StrToWzID("B003"),//eControl_Button_PasSlot_B003,
	StrToWzID("B004"),//eControl_Button_PasSlot_B004,
	StrToWzID("B000"),//eControl_Button_PasSlot_B000,

	StrToWzID("P004"),//eControl_Picture_Buff_P004,
	StrToWzID("P005"),//eControl_Picture_Buff_P005,
	StrToWzID("P006"),//eControl_Picture_Buff_P006,
	StrToWzID("P007"),//eControl_Picture_Buff_P007,
	StrToWzID("P008"),//eControl_Picture_Buff_P008,
	StrToWzID("P009"),//eControl_Picture_Buff_P009,
	StrToWzID("P010"),//eControl_Picture_Buff_P010,
	StrToWzID("P011"),//eControl_Picture_Buff_P011,
	StrToWzID("P012"),//eControl_Picture_Buff_P012,
	StrToWzID("P013"),//eControl_Picture_Buff_P013,
	StrToWzID("P014"),//eControl_Picture_Buff_P014,
	StrToWzID("P015"),//eControl_Picture_Buff_P015,
	StrToWzID("P016"),//eControl_Picture_Buff_P016,
	StrToWzID("P017"),//eControl_Picture_Buff_P017,
	StrToWzID("P018"),//eControl_Picture_Buff_P018,
	StrToWzID("P019"),//eControl_Picture_Buff_P019,
	StrToWzID("P020"),//eControl_Picture_Buff_P020,
	StrToWzID("P021"),//eControl_Picture_Buff_P021,
	StrToWzID("P022"),//eControl_Picture_Buff_P022,
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
	StrToWzID("P023"),//eControl_Picture_Buff_P023,
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI

	StrToWzID("P031"),//eControl_Picture_Buff_P004,
	StrToWzID("P032"),//eControl_Picture_Buff_P005,
	StrToWzID("P033"),//eControl_Picture_Buff_P006,
	StrToWzID("P034"),//eControl_Picture_Buff_P007,
	StrToWzID("P035"),//eControl_Picture_Buff_P008,
	StrToWzID("P036"),//eControl_Picture_Buff_P009,
	StrToWzID("P037"),//eControl_Picture_Buff_P010,
	StrToWzID("P038"),//eControl_Picture_Buff_P011,
	StrToWzID("P039"),//eControl_Picture_Buff_P012,
	StrToWzID("P040"),//eControl_Picture_Buff_P013,
	StrToWzID("P041"),//eControl_Picture_Buff_P014,
	StrToWzID("P042"),//eControl_Picture_Buff_P015,
	StrToWzID("P043"),//eControl_Picture_Buff_P016,
	StrToWzID("P044"),//eControl_Picture_Buff_P017,
	StrToWzID("P045"),//eControl_Picture_Buff_P018,
	StrToWzID("P046"),//eControl_Picture_Buff_P019,
	StrToWzID("P047"),//eControl_Picture_Buff_P020,
	StrToWzID("P048"),//eControl_Picture_Buff_P021,
	StrToWzID("P049"),//eControl_Picture_Buff_P022,
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
	StrToWzID("P050"),//eControl_Picture_Buff_P023,
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI

	StrToWzID("PI13"),//eControl_Picture_SDGauge_PI13,
	StrToWzID("P000"),//eControl_Picture_HPGauge_P000,
	StrToWzID("PI02"),//eControl_Picture_MPSPGauge_PI02,
	StrToWzID("P001"),//eControl_Picture_MPGauge_P001,

	StrToWzID("PI01"),//eControl_Picture_HPRecGauge_PI01,
	StrToWzID("P001"),//eControl_Picture_MPSPRecGauge_P001,

	StrToWzID("PI03"),//eControl_Picture_HeadPI03
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    StrToWzID("B006"),//eControl_Picture_PerkStyleA
    StrToWzID("B005"),//eControl_Picture_PerkStyleB
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
};

HeroGaugeDialog2::HeroGaugeDialog2():
	hp_prev_percent_(100.0f)
	,mouseover_status_icon_index_(0)
	,hp_gauge_size_(0.0f)
	,mp_gauge_size_(0.0f)
	,sd_gauge_size_(0.0f)
	,sd_using_character_(false)
	,sp_gauge_size_(0.0f)
	,hp_post_gauge_(100.0f)
	,mp_post_gauge_(100.0f)
	,sp_post_gauge_(100.0f)
	,sd_post_gauge_(100.0f)

{
	m_hTextureHeadMask = INVALID_HANDLE_VALUE;
	m_hTextureGaugeEffect = INVALID_HANDLE_VALUE;

	ui_perk_dialog_ = NULL;
}

HeroGaugeDialog2::~HeroGaugeDialog2()
{

}

VOID HeroGaugeDialog2::Init( CDrawBase * /*pDrawBase*/ )
{
	hero_hp_bar_ = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_HPGauge_P000));
	hero_SD_bar_ = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_SDGauge_PI13));
	hero_SP_bar_ = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_SPGauge_PI02));
	hero_hp_rev_bar_ = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_HPRecGauge_PI01));
	hero_MP_bar_ = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_MPGauge_P001));

	if(hero_hp_bar_)
	{
		hp_gauge_size_ = (float)hero_hp_bar_->GetSize().right;
	}	

	if(hero_MP_bar_)
	{
		mp_gauge_size_ = (float)hero_MP_bar_->GetSize().right;
	}

	if(hero_SD_bar_)
	{
		sd_gauge_size_ = (float)hero_SD_bar_->GetSize().right;
	}

	if(hero_hp_rev_bar_)
	{
		hp_rev_size_ = (float)hero_hp_rev_bar_->GetSize().right;
	}

	if(hero_SP_bar_)
	{
		sp_gauge_size_ = (float)hero_SP_bar_->GetSize().right;
	}

	hp_current_max_ = getControl(eControl_Static_HP_S001);
	sd_current_max_ = getControl(eControl_Static_SD_S000);
	mp_current_max_ = getControl(eControl_Static_MP_S002);

	SetHeroHP( 75.f );
	SetHeroMP( 75.f );
	SetHeroSD( 75.f );

	CCtrlPictureWZ* HeadUI = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_HeadPI03));

	if(HeadUI)
	{
		HeadUI->ShowWindowWZ(WZ_HIDE);
	}

	if (m_hTextureHeadMask == INVALID_HANDLE_VALUE)
	{
		m_hTextureHeadMask = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\HP_AP00.tga"
			,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

	if(m_hTextureGaugeEffect == INVALID_HANDLE_VALUE)
	{
		m_hTextureGaugeEffect = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg02.tga"
			,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

	for (int i = 0 ; i < 3 ; ++i)
	{
		Effect_Rect_Pos_[3][i].left = 976 + (16 * i);
		Effect_Rect_Pos_[3][i].top = 0;

		Effect_Rect_Pos_[0][i].left = 976 + (16 * i);
		Effect_Rect_Pos_[0][i].top = 16;

		Effect_Rect_Pos_[1][i].left = 976 + (16 * i);
		Effect_Rect_Pos_[1][i].top = 16 * 2;

		Effect_Rect_Pos_[2][i].left = 976 + (16 * i);
		Effect_Rect_Pos_[2][i].top = 16 * 3;
	}
	
	CControlWZ* is_hide = getControl(eControl_Static_None_S003);

	if(is_hide)
	{
		is_hide->ShowWindowWZ(WZ_HIDE);
	}

	for (int index = 0 ; index < 4 ; ++index)
	{
		Hit_Effect_Delay_[index] = 0;
	}

	is_mouse_over_in_perk_slot_ = false;
	mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;
}

VOID HeroGaugeDialog2::MessageProc( SI_MESSAGE * pMessage )
{
	

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONDOWN:
		{
			HeroGaugeDialog2Control* parent = (HeroGaugeDialog2Control*)ui_parent_dialog_;
			//UI간 통신을 위한 SendMessage처리. 
			//MessageMap을 구현하면 실질적으로 사용할수 있음.
			//지금은 야메
			SendMesaageUI(parent,WM_CLICK_EVENT,0,0);
		}
		break;
	case RT_MSG_LBUTTONCLICK:
		{
			HeroGaugeDialog2Control* parent = (HeroGaugeDialog2Control*)ui_parent_dialog_;
			//UI간 통신을 위한 SendMessage처리. 
			//MessageMap을 구현하면 실질적으로 사용할수 있음.
			//지금은 야메
			SendMesaageUI(parent,WM_CLICK_EVENT,0,0);
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch(pos)
			{
			
			case eControl_Button_PasSlot_B001:
			case eControl_Button_PasSlot_B002:
			case eControl_Button_PasSlot_B003:
			case eControl_Button_PasSlot_B004:
			case eControl_Button_PasSlot_B000:
				{
					uiBase* pUIBase = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);

					if (pUIBase)
					{
						BOOL bShow = pUIBase->IsVisible();
						pUIBase->ShowWindow(!bShow);

						uiBase* pAction = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_ACTION_DLG);
						uiBase* pskill = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_TREE_DLG);
						
						if(pAction && pAction->IsVisible())
						{
							pAction->ShowWindow(false);
						}

						if(pskill && pskill->IsVisible())
						{
							pskill->ShowWindow(false);
						}
					}

				}
				break;
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
            case eControl_Button_PerkStyleA:
            case eControl_Button_PerkStyleB:
                {
                    g_HeroInput.OnSkill(35023);
                }
                break;            
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
			}

			uiPetGauge* pet_dialog_ = GET_CAST_DIALOG(uiPetGauge, IM_PET_MANAGER::PET_GAUGE_DLG);

			if(pet_dialog_)
			{
				pet_dialog_->ClickMessage();
			}
		}
		break;
	case RT_MSG_MOUSEOVER:
		{

		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}


}

VOID HeroGaugeDialog2::NetworkProc( MSG_BASE * pMsg )
{

}

VOID HeroGaugeDialog2::OnRenderSolarDialog()
{
	RenderCharacterHeader();
	RenderPerkSlot();
	RenderBarEffect();
	RenderTooltip();

	HeroGaugeDialog2Control* GaugeControl = (HeroGaugeDialog2Control*)ui_parent_dialog_;
	if(GaugeControl)
	{
		GaugeControl->Render();
	}

	if( hp_current_max_ )
	{
		hp_current_max_->OnDraw(true);
	}
	if( sd_current_max_ )
	{
		sd_current_max_->OnDraw(true);
	}
	if( mp_current_max_ )
	{
		mp_current_max_->OnDraw(true);
	}


}

VOID HeroGaugeDialog2::OnUpdateSolarDialog()
{
	if(ui_perk_dialog_ == NULL)
	{
		ui_perk_dialog_ = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
	}

	UpdateHeroState();
	UpdateHitEffect();

	if (mouseover_status_icon_index_ >= 0 && mouseover_status_icon_index_ < 30)
	{   
		SKILL_EFFECT_IMAGE* status_icon = &(status_icon_container_[mouseover_status_icon_index_]);
		if ((status_icon != NULL) &&
			((g_Input.GetState(MOUSE_RBUTTON, IKS_MOUSE_DOWN) == TRUE) ||
			(g_Input.GetState(MOUSE_RBUTTON, KS_MOUSE_DOWN) == TRUE)))
		{
			if(status_icon->pStateInfo->m_bDelType == TRUE)
			{
				MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
				send_packet.m_eCharStateType = 
					static_cast<eCHAR_STATE_TYPE>(status_icon->pStateInfo->m_wStateID);
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
//
//#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
//				// 디펜스 모션을 사용해서 자신에게 피격스턴 상태가 부여되고 이 상태를 삭제하면 모션을 스킵하는 하드코딩 ㅋ
//				static int state_code = 121;    // 피격스턴의 상태코드
//				if( status_icon->pStateInfo->m_wStateID == state_code )
//				{
//					if( StateBase* current_state = g_pHero->GetCurStateInstance() )
//					{
//						// Skip DefenceMotionState 
//						current_state->ProcessExtraCommand();
//					}
//				}
//#endif
			}
			else
			{
#ifdef _DEV_VER //_NA_008196_20150407_GM_REMOVE_STATUS
                static DWORD last_rightclick_tick = GetTickCount();
                if (last_rightclick_tick + 1000 > GetTickCount())
                {
                    // 5016 쉴드
                    if (status_icon->pStateInfo->m_wStateID != 5016) 
                    {
                        MSG_CG_GM_STRING_CMD_SYN msg;
                        ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                        Snprintf(msg.m_szStringCmd,
                            MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH,
                            "상태제거 %d",
                            status_icon->pStateInfo->m_wStateID);
                        GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
                        return;
                    }
                }
                last_rightclick_tick = GetTickCount();
#endif //_NA_008196_20150407_GM_REMOVE_STATUS

				TCHAR notice_message[INTERFACE_STRING_LENGTH + 1] = {0,};
				TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0,};
				TCHAR status_name[INTERFACE_STRING_LENGTH + 1] = {0,};
				//6332 %s 상태 아이콘을 삭제 할 수 없습니다.
				g_InterfaceManager.GetInterfaceString(6332, temp_string);
				g_InterfaceManager.GetInterfaceString(status_icon->pStateInfo->m_dwNamecode, status_name);
				Snprintf(notice_message, INTERFACE_STRING_LENGTH, temp_string, status_name);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
			}
		} 
	}

	int mouse_x_position = Mouse::Instance()->GetMouseX();
	int mouse_y_position = Mouse::Instance()->GetMouseY();

	int i = 0;
	for (i = eControl_Button_PasSlot_B001 ; i <= eControl_Button_PasSlot_B000+1 ; ++i )
	{
		if( i == eControl_Button_PasSlot_B000+1)
		{
			is_mouse_over_in_perk_slot_ = false;
			mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;
			break;
		}

		CControlWZ* control_pos = getControl(i);

		if(control_pos)
		{
			RECT draw_rect = control_pos->GetSizeRect();

			if ((draw_rect.left <= mouse_x_position) && 
				(draw_rect.right >= mouse_x_position) &&
				(draw_rect.top <= mouse_y_position) && 
				(draw_rect.bottom >= mouse_y_position))
			{
				mouse_over_perk_slot_ = i;
				if (mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE)
				{
					is_mouse_over_in_perk_slot_ = true;
				}
				break;
			}
		}
	}

	
}


void HeroGaugeDialog2::Process( DWORD dwTick )
{

}


CControlWZ * HeroGaugeDialog2::getControl( POSTYPE AtPos )
{
	assert( AtPos < HGAUGE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE HeroGaugeDialog2::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < HGAUGE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return HGAUGE_MAX;
}

void HeroGaugeDialog2::MakeHitEffect()
{
	//not use
}

void HeroGaugeDialog2::set_hp_prev_percent( float val )
{
	hp_prev_percent_ = val; 
	hp_prev_percent_ = min(100.0f, hp_prev_percent_);
	hp_prev_percent_ = max(0.0f, hp_prev_percent_);
	
}

void HeroGaugeDialog2::hp_prev_percent_increase( float val )
{
	hp_prev_percent_ += val;
	hp_prev_percent_ = min(100.0f, hp_prev_percent_);
	hp_prev_percent_ = max(0.0f, hp_prev_percent_);
}

void HeroGaugeDialog2::UpdateStatusIcon()
{
	status_icon_container_.clear();
	if (g_pHero == NULL)
	{
		return;
	}

	int mouse_x_position = Mouse::Instance()->GetMouseX();
	int mouse_y_position = Mouse::Instance()->GetMouseY();
	mouseover_status_icon_index_ = 255;

	// Hero 의 SKILL_EFFECT 를 순회
	BASE_STATEINFO* state_info = NULL;
	SKILL_EFFECT_IMAGE status_icon;
	DWORD hero_object_key = g_pHero->GetObjectKey();
	int status_Index = 0;

	CControlWZ* buff_control = NULL;

	std::vector<SKILL_EFFECT*>& array = g_pHero->GetSkillEffectArray();
	for( size_t i=0;i!=array.size();++i)
	{
		SKILL_EFFECT* skill_effect = array[i];

		state_info = StateInfoParser::Instance()->GetStateInfo(skill_effect->dwStatusID);
		status_icon.iEffectID = skill_effect->iEffectID;
		status_icon.dwStatusID = skill_effect->dwStatusID;
		status_icon.dwOwnerID = hero_object_key;
		status_icon.pSkillEffect = skill_effect;
		status_icon.pStateInfo = state_info;

		if(status_Index < 20)
		{
			status_icon.m_byAlpha = 0xFF;
		}
		else
		{
			status_icon.m_byAlpha = StatusIconManager::ALPHA_VALUE_MOUSE_OVER_ICON;
		}
		// draw_rect 계산

		buff_control = getControl(eControl_Picture_Buff_P004 + status_Index);
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

				// 마우스 오버된 아이콘은 반투명
				status_icon.m_byAlpha = StatusIconManager::ALPHA_VALUE_MOUSE_OVER_ICON;
				mouseover_status_icon_index_ = status_Index;
			}
			status_icon_container_.push_back(status_icon);	
		}

		++status_Index;
		if (status_Index >= StatusIconManager::MAX_HERO_ICON)
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

VOID HeroGaugeDialog2::SetHeroHP( float fHPPercent )
{
	if( !hero_hp_bar_ )
	{
		return;
	}

	fHPPercent = max( 0.0f, min( 100.0f, fHPPercent ) );

	RECT rect;
	rect = hero_hp_bar_->GetSizeRect();

	float width, height;
	height = (float)hero_hp_bar_->GetSize().bottom;

	Hit_Effect_Hight_[0] = (float)rect.top - 3;//3픽셀식 위로

	if (hp_gauge_size_ >= 0.0f )
	{
		width = (hp_gauge_size_/100.f) * fHPPercent;

		hero_hp_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			hero_hp_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			hero_hp_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		hero_hp_bar_->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );

		Hit_Effect_width_[0] = rect.left + width - 8;
	}
	else
	{
		//m_fHitEffectPosX = (float)rect.left;
	}

	if(hp_post_gauge_ < width) //체력이 담
	{
		hp_post_gauge_ = width;
		return;
	}

	if(hp_post_gauge_ > width) //체력이 담
	{
		use_effect_texture_[0] = true;
		hp_post_gauge_ = width;
	}
}

VOID HeroGaugeDialog2::SetHeroMP( float fMPPercent )
{
	if( !hero_MP_bar_ )
	{
		return;
	}

	if (fMPPercent >= 100.0f)
	{
		fMPPercent = 100.0f;
	}

	if (fMPPercent < 0.0f)
	{
		fMPPercent = 0.0f;

	}

	RECT rect;
	rect = hero_MP_bar_->GetSizeRect();
	float width;
	float height = (float)hero_MP_bar_->GetSize().bottom;

	//중간좌표를 구한뒤 거기에 이미지 사이즈 만큼
	Hit_Effect_Hight_[1] = (float)rect.top - 3;//3픽셀식 위로

	if (mp_gauge_size_ >= 0.0f )
	{
		width = (mp_gauge_size_/100.f) * fMPPercent;

		hero_MP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			hero_MP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			hero_MP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		hero_MP_bar_->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );

		Hit_Effect_width_[1] = rect.left + width - 8;
	}

	if(mp_post_gauge_ < width) //체력이 담
	{
		mp_post_gauge_ = width;
		return;
	}

	if(mp_post_gauge_ > width) //체력이 담
	{
		use_effect_texture_[1] = true;
		mp_post_gauge_ = width;
	}
}


VOID HeroGaugeDialog2::SetHeroSP( float fSPPercent )
{
	if( !hero_SP_bar_ )
	{
		return;
	}

	if (fSPPercent >= 100.0f)
	{
		fSPPercent = 100.0f;
	}

	if (fSPPercent < 0.0f)
	{
		fSPPercent = 0.0f;

	}

	RECT rect;
	rect = hero_SP_bar_->GetSizeRect();
	float width;
	float height = (float)hero_SP_bar_->GetSize().bottom;

	Hit_Effect_Hight_[2] = (float)rect.top - 3;//3픽셀식 위로

	if (mp_gauge_size_ >= 0.0f )
	{
		width = (sp_gauge_size_/100.f) * fSPPercent;

		hero_SP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			hero_SP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			hero_SP_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		hero_SP_bar_->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
		Hit_Effect_width_[2] = rect.left + width - 8;
	}

	if(sp_post_gauge_ < width) //체력이 담
	{
		sp_post_gauge_ = width;
		return;
	}

	if(sp_post_gauge_ > width) //체력이 담
	{
		use_effect_texture_[2] = true;
		sp_post_gauge_ = width;
	}
}


VOID HeroGaugeDialog2::SetHeroSD( float fHPPercent )
{
	if( !hero_SD_bar_ )
	{
		return;
	}

	if (fHPPercent >= 100.0f)
	{
		fHPPercent = 100.0f;
	}

	if (fHPPercent < 0.0f)
	{
		fHPPercent = 0.0f;

	}

	RECT rect;
	rect = hero_SD_bar_->GetSizeRect();
	float width ;
	float height = (float)hero_SD_bar_->GetSize().bottom;

	Hit_Effect_Hight_[3] = (float)rect.top - 2;//2픽셀식 위로

	if (sd_gauge_size_ >= 0.0f )
	{
		width = (sd_gauge_size_/100.f) * fHPPercent;
		hero_SD_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			hero_SD_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			hero_SD_bar_->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		hero_SD_bar_->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
		Hit_Effect_width_[3] = rect.left + width - 8;
	}

	if(sd_post_gauge_ < width) //체력이 담
	{
		sd_post_gauge_ = width;
		return;
	}

	if(sd_post_gauge_ > width) //체력이 담
	{
		use_effect_texture_[3] = true;
		sd_post_gauge_ = width;
	}
}


void HeroGaugeDialog2::SetHeroReserveHP( float val )
{
	val = max( 0.0f, min( 100.0f, val ) );


	RECT hpRect, rcvRect;
	float width, height;

	hpRect = hero_hp_bar_->GetSizeRect();
	rcvRect = hero_hp_rev_bar_->GetSize();

	width = ( hp_gauge_size_ * ( val / 100.0f ) );
	height = (float)rcvRect.bottom;

	if( rcvRect.left + width > hpRect.left + hp_gauge_size_ )
	{
		width = ( hpRect.left + hp_gauge_size_ ) - rcvRect.left;
	}

	hero_hp_rev_bar_->MoveWindowWZ( (float)rcvRect.left, (float)rcvRect.top, width, height );
}


VOID HeroGaugeDialog2::UpdateHeroState()
{
	CCtrlPictureWZ * pCtrlStatic_mp =  (CCtrlPictureWZ * )getControl(eControl_Picture_MP_P026);
	CCtrlPictureWZ * pCtrlStatic_sp =  (CCtrlPictureWZ * )getControl(eControl_Picture_SP_P027);

	if (pCtrlStatic_sp && pCtrlStatic_mp)
	{
		// sp/mp 를 정하자.
		if (eCHAR_BERSERKER == g_pHero->GetClass())
		{
			pCtrlStatic_sp->ShowWindowWZ(WZ_SHOW);
			pCtrlStatic_mp->ShowWindowWZ(WZ_HIDE);

			hero_MP_bar_->ShowWindowWZ(WZ_HIDE);
			hero_SP_bar_->ShowWindowWZ(WZ_SHOW);

			sd_using_character_ = true;
		}
		else
		{
			pCtrlStatic_sp->ShowWindowWZ(WZ_HIDE);
			pCtrlStatic_mp->ShowWindowWZ(WZ_SHOW);

			hero_MP_bar_->ShowWindowWZ(WZ_SHOW);
			hero_SP_bar_->ShowWindowWZ(WZ_HIDE);

			sd_using_character_ = false;
		}
	}

	DWORD maxhp = max(g_pHero->GetMaxHP(),1);
	DWORD curhp = g_pHero->GetHP();

	//safecode:현재hp가 맥스hp보다 클수는없다
	if(curhp>=maxhp)
		curhp=maxhp;

	DWORD maxmp = max(g_pHero->GetMaxMP(),1);
	DWORD curmp = g_pHero->GetMP();

	if(sd_using_character_)
	{
		SetHeroSP( ((float)curmp / (float)maxmp) * 100.0f );
	}
	else
	{
		SetHeroMP( ((float)curmp / (float)maxmp) * 100.0f );
	}
	
	DWORD cursd	= g_pHero->GetSD();
	DWORD curmaxsd = g_pHero->GetMaxSD();
	SetHeroSD( ( (float)cursd /  (float)curmaxsd ) * 100.0f);

	DWORD reserveHP = g_pHero->GetReserveHP();

	if (reserveHP > maxhp)
	{
		reserveHP = maxhp;
	}

	DWORD dest_hp = curhp + reserveHP;
	if (dest_hp > maxhp)
	{
		dest_hp = maxhp;
	}

	float dest_hp_percent = (float)dest_hp / (float)maxhp * 100.0f;
	{
		static DWORD cur_time = timeGetTime();
		static DWORD prev_time = timeGetTime();
		cur_time = timeGetTime();
		DWORD gab = cur_time - prev_time;

		float vel_per_second = CGraphicParam::Instance()->hero_gauge_velocity(maxhp, g_pHero->GetSitState() == TRUE);
		float vel_inc = vel_per_second * gab / 1000.0f;

		hp_prev_percent_increase(vel_inc); 
		if (hp_prev_percent() > dest_hp_percent)
		{
			set_hp_prev_percent(dest_hp_percent);
		}
		prev_time = cur_time;
	}

	SetHeroHP(hp_prev_percent());
	SetHeroReserveHP(dest_hp_percent);


	//Update String
	TCHAR stateString[128];

	if( hp_current_max_ )
	{
		memset(stateString,0,sizeof(TCHAR)*128);
		sprintf(stateString,"%d/%d",g_pHero->GetHP(),max(g_pHero->GetMaxHP(),1));
		hp_current_max_->SetTextWZ( const_cast<LPTSTR>(stateString) );
	}
	if( sd_current_max_ )
	{
		memset(stateString,0,sizeof(TCHAR)*128);
		sprintf(stateString,"%d/%d",g_pHero->GetSD(),max(g_pHero->GetMaxSD(),1));
		sd_current_max_->SetTextWZ( const_cast<LPTSTR>(stateString) );
	}
	if( mp_current_max_ )
	{
		memset(stateString,0,sizeof(TCHAR)*128);
		sprintf(stateString,"%d/%d",g_pHero->GetMP(),max(g_pHero->GetMaxMP(),1));
		mp_current_max_->SetTextWZ( const_cast<LPTSTR>(stateString) );
	}
}

void HeroGaugeDialog2::UpdateHitEffect()
{
	for (int idx = 0 ; idx < 4 ; ++idx)
	{
		if(use_effect_texture_[idx] && Hit_Effect_Delay_[idx] == 0.0f)
		{
			Hit_Effect_Delay_[idx] = 12.0f;
			use_effect_texture_[idx] = false;
		}
	}
}

void HeroGaugeDialog2::RenderText()
{
	
}

void HeroGaugeDialog2::RenderCharacterHeader()
{
	CCtrlPictureWZ* HeadUI = static_cast<CCtrlPictureWZ*>(getControl(eControl_Picture_HeadPI03));
	if (HeadUI) 
	{
		RECT rc = HeadUI->GetSizeRect();

		HANDLE hTexture = g_InterfaceManager.GetMyHeadTexture();

		if (hTexture != INVALID_HANDLE_VALUE)
		{
			g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA);

			g_pSunRenderer->x_pManagerTexture->SetColorArg1(0,CManagerTexture::ETARG_TEXTURE);				
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0,CManagerTexture::ETARG_TEXTURE);
			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);

			g_pSunRenderer->x_pManagerTexture->SetColorArg1(1,CManagerTexture::ETARG_CURRENT);				
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1,CManagerTexture::ETARG_CURRENT);
			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1,CManagerTexture::ETOP_SELECTARG1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1,CManagerTexture::ETOP_MODULATE);

			g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1,CManagerTexture::ETARG_TEXTURE);

			g_pSunRenderer->RenderTextureBlending(
				hTexture,
				m_hTextureHeadMask,
				(float)rc.left -9,
				(float)rc.top ,
				(float)128.0f, 
				(float)125.0f);

			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0);
			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(0);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(1);				

			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0);
			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1);
		}

	}
}

void HeroGaugeDialog2::Release()
{
	if(m_hTextureHeadMask != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTextureHeadMask);
		m_hTextureHeadMask = INVALID_HANDLE_VALUE;
	}

	if(m_hTextureGaugeEffect != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTextureGaugeEffect);
		m_hTextureGaugeEffect = INVALID_HANDLE_VALUE;
	}
}

void HeroGaugeDialog2::RenderBarEffect()
{
	static DWORD m_LastTickCount = 0;
	static DWORD m_LastTick2 = 0;
	static int   Ani_tick_Count = 0;
	static bool  Reverse_animation = false;

	DWORD tick_count = clock_function::GetTickCount();

	if (m_LastTickCount == 0)
	{
		m_LastTickCount = tick_count;
	}

	DWORD current_tick_count = (tick_count - m_LastTickCount);

	if (current_tick_count > 60)
	{
		Ani_tick_Count += Reverse_animation ? -1 : 1;
		m_LastTickCount = tick_count;
	}

	if(Ani_tick_Count > 2)
	{
		Reverse_animation = true;
		Ani_tick_Count = 2;
	}

	if(Ani_tick_Count < 0)
	{
		Reverse_animation = false;
		Ani_tick_Count = 0;
	}

	static float perTickTime[4] = {0.0f,};

	for (int idx = 0 ; idx < 4 ; ++idx)
	{
		int isMovePosx = 3;
		int isMoveposy = (idx == 3) ? 1 : 0;

		//SP & MP에 따른 렌더 제외
		if(eCHAR_BERSERKER == g_pHero->GetClass())
		{
			if(idx == 1) 
			{
				continue;
			}
		}
		else
		{
			if(idx == 2) 
			{
				continue;
			}
		}

		perTickTime[idx] += (float)(current_tick_count/1000.0f);
		if(Hit_Effect_Delay_[idx] > 0)
		{
			int ani_tick = (current_tick_count / 30) - 1;
			if(ani_tick > 2) ani_tick = 0;

			g_pSunRenderer->RenderTexture(m_hTextureGaugeEffect,
				(float)Hit_Effect_width_[idx] - isMovePosx,
				(float)Hit_Effect_Hight_[idx] - isMoveposy,
				16.0f,
				16.0f,
				(float)Effect_Rect_Pos_[idx][Ani_tick_Count].left,
				(float)Effect_Rect_Pos_[idx][Ani_tick_Count].top,
				16.0f,
				16.0f);

			Hit_Effect_Delay_[idx] -= perTickTime[idx];
		}

		if(Hit_Effect_Delay_[idx] <= 0)
		{
			g_pSunRenderer->RenderTexture(m_hTextureGaugeEffect,
				(float)Hit_Effect_width_[idx] - isMovePosx,
				(float)Hit_Effect_Hight_[idx] - isMoveposy,
				16.0f,
				16.0f,
				(float)Effect_Rect_Pos_[idx][1].left,
				(float)Effect_Rect_Pos_[idx][1].top,
				16.0f,
				16.0f);

			Hit_Effect_Delay_[idx] = 0.0f;
			perTickTime[idx] = 0.0f;
		}
	}
}

void HeroGaugeDialog2::RenderPerkSlot()
{
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    // 현재 active_group_struct를 구해온다.
    const PerkGroupStruct& active_perk_group_struct = ui_perk_dialog_->get_active_group_struct();

    ItemUnitRenderer* item_unit_renderer = ui_perk_dialog_->GetItemUnitRender();
    if (item_unit_renderer != NULL)
    {
        ItemUnitRenderer::RenderUnit* render_unit = NULL;
        CControlWZ* slot_control = NULL;
        RECT slot_control_rect;
        for (int slot_pos = 0; slot_pos < PerkDataList::kNumberOfActiveSlots; ++slot_pos)
        {
            slot_control = getControl(eControl_Button_PasSlot_B001 + slot_pos);
            if (slot_control == NULL)
            {
                continue;
            }

            POSTYPE active_slot_pos = INVALID_POSTYPE_VALUE;
            if (slot_pos != PerkDataList::kNumberOfActiveSlots - 1)
            {
                active_slot_pos = active_perk_group_struct.from_pos[slot_pos];
                if (active_slot_pos == INVALID_POSTYPE_VALUE)
                {
                    continue;
                }
            }
            else
            {
                active_slot_pos = PerkDataList::kActiveSlotPositions[slot_pos];
                if (ui_perk_dialog_->ValidPos(active_slot_pos) == false)
                {
                    continue;
                }
            }            

            SCPerkSlot& active_slot = 
                static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
            if ( slot_pos == PerkDataList::kNumberOfActiveSlots - 1)
            {
                if (active_slot.GetLinkSlot() == NULL)
                {
                    continue;
                }
            }

            render_unit = item_unit_renderer->GetItemUnit(active_slot.GetPerkID().group_);
            if (render_unit == NULL)
            {
                continue;
            }

            slot_control_rect = slot_control->GetSizeRect();
            item_unit_renderer->RenderPerkSlot(&slot_control_rect, render_unit);
        }
    }

    if (is_mouse_over_in_perk_slot_ && 
        (mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE))
    {
        CControlWZ* control_ptr = getControl(mouse_over_perk_slot_);
        if (control_ptr != NULL)
        {
            uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

            if (tooltip_manager != NULL)
            {
                POSTYPE active_slot_pos = 
                    PerkDataList::kActiveSlotPositions[mouse_over_perk_slot_ - eControl_Button_PasSlot_B001];
                SCPerkSlot& perk_slot = 
                    static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
                tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, active_slot_pos);            
                tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }
        } 
    }
#else
	ItemUnitRenderer* item_unit_renderer = ui_perk_dialog_->GetItemUnitRender();
	if (item_unit_renderer != NULL)
	{
		ItemUnitRenderer::RenderUnit* render_unit = NULL;
		CControlWZ* slot_control = NULL;
		RECT slot_control_rect;
		for (int slot_pos = 0; slot_pos < PerkDataList::kNumberOfActiveSlots; ++slot_pos)
		{
			slot_control = getControl(eControl_Button_PasSlot_B001 + slot_pos);
			if (slot_control == NULL)
			{
				continue;
			}

			POSTYPE active_slot_pos = PerkDataList::kActiveSlotPositions[slot_pos];
			if (ui_perk_dialog_->ValidPos(active_slot_pos) == false)
			{
				continue;
			}

			SCPerkSlot& active_slot = 
				static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
			if (active_slot.GetLinkSlot() == NULL)
			{
				continue;
			}

			render_unit = item_unit_renderer->GetItemUnit(active_slot.GetPerkID().group_);
			if (render_unit == NULL)
			{
				continue;
			}

			slot_control_rect = slot_control->GetSizeRect();
			item_unit_renderer->RenderPerkSlot(&slot_control_rect, render_unit);
		}
	}

	if (is_mouse_over_in_perk_slot_ && 
		(mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE))
	{
		CControlWZ* control_ptr = getControl(mouse_over_perk_slot_);
		if (control_ptr != NULL)
		{
				uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

				if (tooltip_manager != NULL)
				{
					POSTYPE active_slot_pos = 
						PerkDataList::kActiveSlotPositions[mouse_over_perk_slot_ - eControl_Button_PasSlot_B001];
					SCPerkSlot& perk_slot = 
						static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
					tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, active_slot_pos);            
					tooltip_manager->SetTooltipDialogID(GetDialogKey());

				}
		} 
	}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
}



void HeroGaugeDialog2::RenderTooltip()
{
	int			x = Mouse::Instance()->GetMouseX();
	int			y = Mouse::Instance()->GetMouseY();
	RECT postion;
	if(hero_hp_bar_)
	{
		postion = hero_hp_bar_->GetSizeRect();
		postion.right = postion.left + (int)hp_gauge_size_;

		if( x > postion.left && x < postion.right && 
			y > postion.top &&  y < postion.bottom )
		{
			uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
			if (tooltip_manager)
			{
				TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
				g_InterfaceManager.GetInterfaceString(11195, tooltip_message);

				tooltip_manager->RegisterTooltip(tooltip_message);
			}
		}
	}	

	if(hero_SD_bar_)
	{
		postion = hero_SD_bar_->GetSizeRect();
		postion.right = postion.left + (int)sd_gauge_size_;
		if( x > postion.left && x < postion.right && 
			y > postion.top &&  y < postion.bottom )
		{
			uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
			if (tooltip_manager)
			{
				TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
				g_InterfaceManager.GetInterfaceString(11194, tooltip_message);

				tooltip_manager->RegisterTooltip(tooltip_message);
			}
		}
	}

	if(eCHAR_BERSERKER == g_pHero->GetClass())
	{
		if(hero_SP_bar_)
		{
			postion = hero_SP_bar_->GetSizeRect();
			postion.right = postion.left + (int)sp_gauge_size_;

			if( x > postion.left && x < postion.right && 
				y > postion.top &&  y < postion.bottom )
			{
				uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
				if (tooltip_manager)
				{
					TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
					g_InterfaceManager.GetInterfaceString(11197, tooltip_message);

					tooltip_manager->RegisterTooltip(tooltip_message);
				}
			}
		}
	}
	else
	{
		if(hero_MP_bar_)
		{
			postion = hero_MP_bar_->GetSizeRect();
			postion.right = postion.left + (int)mp_gauge_size_;

			if( x > postion.left && x < postion.right && 
				y > postion.top &&  y < postion.bottom )
			{
				uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
				if (tooltip_manager)
				{
					TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
					g_InterfaceManager.GetInterfaceString(11196, tooltip_message);

					tooltip_manager->RegisterTooltip(tooltip_message);
				}
			}
		}
	}
	
}

VOID HeroGaugeDialog2::OnShowWindow( BOOL val )
{
	if(val)
	{
#if WZENGINEVER >= 590
		CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
		pDialogManager->MoveTail( GetDialogKey() );
		SendMesaageUI(ui_parent_dialog_,WM_CLICK_EVENT,0,0);
#endif // WZENGINEVER >= 590


		for (int i = eControl_Static_SD_S000 ; i <= eControl_Picture_SP_P027 ; ++i)
		{
			CControlWZ* base = getControl(i);
			if(base)
			{
				base->SetDiscardMessage(true);
			}
		}

		for (int i = eControl_Picture_Buff_P004 ; i <= eControl_Picture_HeadPI03 ; ++i)
		{
			CControlWZ* base = getControl(i);
			if(base)
			{
				base->SetDiscardMessage(true);
			}
		}

	}
	else
	{

	}
}

void HeroGaugeDialog2::MoveWindowForce(float addx, float addy)
{
	float x = this->GetDialogWZ()->GetDlgX();
	float y = this->GetDialogWZ()->GetDlgY();

	x += addx;
	y += addy;

	this->GetDialogWZ()->MoveWindowWZ(x,y);
}

void HeroGaugeDialog2::MoveDefaultWindowPos()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	uiBase::MoveDefaultWindowPos();
	MoveWindowForce(0.0f,-6.0f);
#endif
}

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void HeroGaugeDialog2::set_perk_style_change( const int& style )
{
    switch ( style )
    {
    case eStylePageA:
        {
            CCtrlButtonWZ* picture_control = static_cast<CCtrlButtonWZ*>(getControl(eControl_Button_PerkStyleA));
            if ( picture_control )
            {
                picture_control->ShowWindowWZ(WZ_SHOW);
            }
            picture_control = static_cast<CCtrlButtonWZ*>(getControl(eControl_Button_PerkStyleB));
            if ( picture_control )
            {
                picture_control->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
    case eStylePageB:
        {
            CCtrlButtonWZ* picture_control = static_cast<CCtrlButtonWZ*>(getControl(eControl_Button_PerkStyleB));
            if ( picture_control )
            {
                picture_control->ShowWindowWZ(WZ_SHOW);
            }
            picture_control = static_cast<CCtrlButtonWZ*>(getControl(eControl_Button_PerkStyleA));
            if ( picture_control )
            {
                picture_control->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
    }
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#endif

#pragma warning ( pop )
