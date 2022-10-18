#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
#include "uiCharacterStatusInfo2.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiCharacterStatus/uiCharacterStatusInfo.h"
#include "Hero.h"

const DWORD uiCharacterStatusInfo2::kDialog_ID = StrToWzID("0421");

uiCharacterStatusInfo2::uiCharacterStatusInfo2(InterfaceManager* interface_manager)
{
}

uiCharacterStatusInfo2::~uiCharacterStatusInfo2(void)
{
}

void uiCharacterStatusInfo2::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
	RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
	RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
	RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
	RegisterControlPosition(StrToWzID("C032"), kControl_CheckBT_C032);
	RegisterControlPosition(StrToWzID("TI09"), kControl_TextImage_TI09);
	RegisterControlPosition(StrToWzID("TI12"), kControl_TextImage_TI12);
	RegisterControlPosition(StrToWzID("TI32"), kControl_TextImage_TI32);
	RegisterControlPosition(StrToWzID("TI13"), kControl_TextImage_TI13);
	RegisterControlPosition(StrToWzID("TI11"), kControl_TextImage_TI11);
	RegisterControlPosition(StrToWzID("I003"), kControl_TextImage_I003);
	RegisterControlPosition(StrToWzID("I004"), kControl_TextImage_I004);
	RegisterControlPosition(StrToWzID("I005"), kControl_TextImage_I005);
	RegisterControlPosition(StrToWzID("I001"), kControl_TextImage_I001);
	RegisterControlPosition(StrToWzID("I002"), kControl_TextImage_I002);
}

void uiCharacterStatusInfo2::Release()
{

}

void uiCharacterStatusInfo2::OnShowWindow( BOOL is_show )
{
	if (is_show == TRUE)
	{
		RefreshTab();
		uiCharacterStatusInfo* character_status_info = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);
		if (character_status_info)
		{
			character_status_info->ShowWindowRefreshSimulation();
		}
	}
	else
	{

	}
}

void uiCharacterStatusInfo2::MessageProc( SI_MESSAGE* message )
{
	if (IsLock())
	{
		return;
	}

	if (GetMan() == NULL)
	{
		return;
	}

	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		OnLButtonClick(message);
		break;
	case RT_MSG_LBUTTONDOWN:
		OnLButtonDown(message);
		break;
	case RT_MSG_LBUTTONUP:
		OnLButtonUp(message);
		break;
	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

void uiCharacterStatusInfo2::OnUpdateSolarDialog()
{
	if (g_pHero == NULL)
	{
		return;
	}

	// 능력치 업데이트
	UpdateParameter();
}

void uiCharacterStatusInfo2::OnRenderSolarDialog()
{
	// 툴팁 출력
	if (IsLocked() == false)
	{
		RenderTooltip();
	}
}

bool uiCharacterStatusInfo2::OnLButtonClick( SI_MESSAGE* message )
{
	bool result = true;
	DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
	switch (control_position)
	{
	case kControl_CheckBT_C000:
	case kControl_CheckBT_C003:
	case kControl_CheckBT_C001:
	case kControl_CheckBT_C002:
	case kControl_CheckBT_C032:
		{
			uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
			if (ui_status_manager != NULL)
			{
				TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
				tab_manager.OnClickTab(control_position - kControl_CheckBT_C000);
			}
			RefreshTab();
		}
		break;
	default:
		{
		}
		break;
	}

	return result;
}

bool uiCharacterStatusInfo2::OnLButtonDown( SI_MESSAGE* message )
{
	return true;
}

bool uiCharacterStatusInfo2::OnLButtonUp( SI_MESSAGE* message )
{
	return true;
}

void uiCharacterStatusInfo2::RefreshTab()
{
	CCtrlButtonCheckWZ* check_button = NULL;
	for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C032 + 1); ++position)
	{
		check_button = 
			static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
		if (check_button == NULL)
		{
			continue;
		}
		check_button->SetCheckState((position == kControl_CheckBT_C003) ? (true) : (false));
	}
}

void uiCharacterStatusInfo2::SetControlTextColor( CCtrlImageStaticWZ* control_ptr, int val, bool is_max /*= false*/ )
{
	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE) == false)
	{
		return;
	}

	if ((control_ptr->IsKind(CTRL_KIND_TEXTIMAGE) == true))
	{
		if (is_max == true)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetFontID(StrToWzID("m217"));
			return;
		}
		else
		{
			control_ptr->SetFontID(StrToWzID("st10"));
		}

		if (val > 0)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,255,20,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,255,20,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,255,20,255));
		}
		else if (val < 0)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,20,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,20,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,20,255,255));
		}
		else
		{
			control_ptr->SetTextColor(WzColor_RGBA(255,255,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(255,255,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(255,255,255,255));
		}
	}
}

void uiCharacterStatusInfo2::SetControlReverseTextColor( CCtrlImageStaticWZ* control_ptr, int val, bool is_max /*= false*/ )
{
	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE) == false)
	{
		return;
	}

	if ((control_ptr->IsKind(CTRL_KIND_TEXTIMAGE) == true))
	{
		if (is_max == true)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,255,255,255));
			control_ptr->SetFontID(StrToWzID("m217"));
			return;
		}
		else
		{
			control_ptr->SetFontID(StrToWzID("st10"));
		}

		if (val < 0)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,255,20,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,255,20,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,255,20,255));
		}
		else if (val > 0)
		{
			control_ptr->SetTextColor(WzColor_RGBA(20,20,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(20,20,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(20,20,255,255));
		}
		else
		{
			control_ptr->SetTextColor(WzColor_RGBA(255,255,255,255));
			control_ptr->SetTextMouseOnColor(WzColor_RGBA(255,255,255,255));
			control_ptr->SetTextMouseDownColor(WzColor_RGBA(255,255,255,255));
		}
	}
}

void uiCharacterStatusInfo2::SetParameterCaption( UIControlPos control_position, int val )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr),
			val,
			IsMaxParameterCaption(control_position, val));

		TCHAR text_string[INTERFACE_STRING_LENGTH] = {0, };
		Snprintf(text_string, INTERFACE_STRING_LENGTH-1, _T("%d"), val);
		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(text_string);
	}
}

void uiCharacterStatusInfo2::SetParameterCaption( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr),
			(calc_value - base_value),
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR text_string[INTERFACE_STRING_LENGTH] = {0, };
		Snprintf(text_string, INTERFACE_STRING_LENGTH-1, _T("%d"), calc_value);
		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(text_string);
	}
}

void uiCharacterStatusInfo2::SetParameterDamage( CCtrlImageStaticWZ* control_ptr, int min_value, int max_value )
{
	TCHAR text_string[INTERFACE_STRING_LENGTH] = {0, };

#ifdef _JAPAN
	Snprintf(text_string, INTERFACE_STRING_LENGTH-1, _T("%d-%d"), min_value, max_value);	
#else
	Snprintf(text_string, INTERFACE_STRING_LENGTH-1, _T("%d~%d"), min_value, max_value);
#endif	

	control_ptr->SetTextColor(WzColor_RGBA(255,255,255,255));
	control_ptr->SetTextWZ(text_string);
}

void uiCharacterStatusInfo2::SetParameterCaptionWithPercent( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr),
			(calc_value - base_value),
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
		TCHAR string_messgae[INTERFACE_STRING_LENGTH] = {NULL, };

		//! 158573	%d%%
		g_InterfaceManager.GetInterfaceString(158573, string_format, INTERFACE_STRING_LENGTH);
		Snprintf(string_messgae, INTERFACE_STRING_LENGTH-1, string_format, calc_value);

		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(string_messgae);
	}
}

void uiCharacterStatusInfo2::SetParameterCaptionWithPercentReverseTextColor( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlReverseTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr),
			(calc_value - base_value),
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
		TCHAR string_messgae[INTERFACE_STRING_LENGTH] = {NULL, };

		//! 158573	%d%%
		g_InterfaceManager.GetInterfaceString(158573, string_format, INTERFACE_STRING_LENGTH);
		Snprintf(string_messgae, INTERFACE_STRING_LENGTH-1, string_format, calc_value);

		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(string_messgae);
	}
}

void uiCharacterStatusInfo2::SetParameterCaptionWithSecond( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr), 
			(calc_value - base_value), 
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR string_format[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR string_message[INTERFACE_STRING_LENGTH] = {0, };

		// 2149 : 초
		g_InterfaceManager.GetInterfaceString(2149, string_format, INTERFACE_STRING_LENGTH);
		Snprintf(string_message, INTERFACE_STRING_LENGTH-1, "%d%s", calc_value, string_format);

		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(string_message);
	}
}

void uiCharacterStatusInfo2::SetParameterCaptionWithMilisecond( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr), 
			(calc_value - base_value), 
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR string_format[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR string_message[INTERFACE_STRING_LENGTH] = {0, };

		// 2149 : 초
		g_InterfaceManager.GetInterfaceString(2149, string_format, INTERFACE_STRING_LENGTH);
		float result_value = static_cast<float>(calc_value) * 0.001f;
		Snprintf(string_message, INTERFACE_STRING_LENGTH-1, "%.1f%s", result_value, string_format);

		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(string_message);
	}
}

bool uiCharacterStatusInfo2::IsMaxParameterCaption( UIControlPos control_position, IN OUT int& calc_value )
{
	return false;
}

void uiCharacterStatusInfo2::UpdateParameter()
{
	uiCharacterStatusInfo* character_status_info = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);

	if (character_status_info == NULL)
	{
		return;
	}

	PlayerAttributes* player_attributes = character_status_info->GetSimulationPlayerAttributes();
	const BASE_PLAYERINFO* player_info  = character_status_info->GetSimulation_BASE_PLAYERINFO();

	// 스킬공격력 증가(%)
	SetParameterCaptionWithPercent(kControl_TextImage_TI09, 
		player_attributes->GetSkillPercentDamage(), 
		player_attributes->GetBaseSkillPercentDamage());

	// 스킬 강화력
	SetParameterCaption(kControl_TextImage_TI12, 
		player_attributes->GetSkillAttackPower(), 
		player_attributes->GetBaseSkillAttackPower());

	// 버프시간 증가
	SetParameterCaptionWithSecond(kControl_TextImage_TI32, 
		player_attributes->GetIncreaseSkillDuration(), 
		player_attributes->GetBaseIncreaseSkillDuration());

	// 피해 디버프시간 감소
	SetParameterCaptionWithSecondReverse(kControl_TextImage_TI13, 
		player_attributes->GetDecreaseSkillDuration(), 
		player_attributes->GetBaseDecreaseSkillDuration());

	// 가해 디버프시간 강화
	SetParameterCaptionWithMilisecond(kControl_TextImage_TI11, 
		player_attributes->GetDebuffDuration(), 
		player_attributes->GetBaseDebuffDuration());

	// 스킬상태무시
	SetParameterCaptionWithPercent(kControl_TextImage_I003, 
		player_attributes->GetResistBadStatusRatio(), 
		player_attributes->GetBaseResistBadStatusRatio());

	// 최대데미지 발생확률
	SetParameterCaptionWithPercent(kControl_TextImage_I004, 
		player_attributes->GetMaxDamageRatio(), 
		player_attributes->GetBaseMaxDamageRatio());

	// 조합 성공률
	SetParameterCaptionWithPercent(kControl_TextImage_I005, 
		player_attributes->GetCompositeIncRatio(), 
		player_attributes->GetBaseCompositeIncRatio());

	// 하임 습득
	SetParameterCaptionWithPercent(kControl_TextImage_I001, 
		player_attributes->GetBonusMoneyRatio(), 
		player_attributes->GetBaseBonusMoneyRatio());

	// 경험치 습득
	SetParameterCaptionWithPercent(kControl_TextImage_I002, 
		player_attributes->GetBonusExpRatio(), 
		player_attributes->GetBaseBonusExpRatio());
}

void uiCharacterStatusInfo2::RenderTooltip()
{
	uiCharacterStatusInfo* character_status_info = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);

	if (character_status_info == NULL)
	{
		return;
	}

	PlayerAttributes* player_attributes = character_status_info->GetSimulationPlayerAttributes();
	uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

	if ((tooltip_manager == NULL) || (player_attributes == NULL))
	{
		return;
	}

	if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI09)))      // 스킬공격력 증가(%)
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseSkillPercentDamage(), 
			player_attributes->GetSkillPercentDamage());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI12)))  // 스킬 강화력
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseSkillAttackPower(), 
			player_attributes->GetSkillAttackPower());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI32)))  // 버프시간 증가
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseIncreaseSkillDuration(), 
			player_attributes->GetIncreaseSkillDuration());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI13)))  // 피해 디버프시간 감소
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseDecreaseSkillDuration(), 
			player_attributes->GetDecreaseSkillDuration());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI11)))  // 가해 디버프시간 강화
	{
		RenderTooltip_BaseAndCalcMilisecond(tooltip_manager, 
			player_attributes->GetBaseDebuffDuration(), 
			player_attributes->GetDebuffDuration());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I003)))  // 스킬상태무시
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseResistBadStatusRatio(), 
			player_attributes->GetResistBadStatusRatio());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I004)))  // 최대데미지 발생확률
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseMaxDamageRatio(), 
			player_attributes->GetMaxDamageRatio());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I005)))  // 조합 성공률
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseCompositeIncRatio(), 
			player_attributes->GetCompositeIncRatio());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I001)))  // 하임 습득
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseBonusMoneyRatio(), 
			player_attributes->GetBonusMoneyRatio());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I002)))  // 경험치 습득
	{
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseBonusExpRatio(), 
			player_attributes->GetBonusExpRatio());
	}
}

void uiCharacterStatusInfo2::RenderTooltip_StringCode( uiToolTipMan* tooltip_manager, int StringCode )
{
	tooltip_manager->RegisterTooltipWithStringCode( StringCode );
	tooltip_manager->SetTooltipDialogID(GetDialogKey());
}

void uiCharacterStatusInfo2::RenderTooltip_BaseAndCalc( uiToolTipMan* tooltip_manager, int base_value, int calc_value, bool is_force /*= true*/, DWORD string_code /*= 0*/ )
{
	if ((base_value != calc_value) || 
		(is_force == true))
	{
		TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {NULL, };
		if (string_code > 0)
		{
			g_InterfaceManager.GetInterfaceString(string_code, tooltip_string, INTERFACE_STRING_LENGTH);
		}

		if (calc_value >= base_value)
		{
			_tcscat(tooltip_string, _T("%d + %d"));
		}
		else
		{
			_tcscat(tooltip_string, _T("%d - %d"));
		}
		tooltip_manager->RegisterTooltipFormat(tooltip_string, base_value, abs(calc_value - base_value));
		tooltip_manager->SetTooltipDialogID(GetDialogKey());
	}
}

void uiCharacterStatusInfo2::RenderTooltip_BaseAndCalcMilisecond( uiToolTipMan* tooltip_manager, int base_value, int calc_value, bool is_force /*= true*/, DWORD string_code /*= 0*/ )
{
	if ((base_value != calc_value) || 
		(is_force == true))
	{
		TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {NULL, };
		if (string_code > 0)
		{
			g_InterfaceManager.GetInterfaceString(string_code, tooltip_string, INTERFACE_STRING_LENGTH);
		}

		if (calc_value >= base_value)
		{
			_tcscat(tooltip_string, _T("%.1f + %.1f"));
		}
		else
		{
			_tcscat(tooltip_string, _T("%.1f - %.1f"));
		}
		float result_calc = static_cast<float>(calc_value) * 0.001f;
		float result_base = static_cast<float>(base_value) * 0.001f;
		tooltip_manager->RegisterTooltipFormat(tooltip_string, result_base, fabs(result_calc - result_base));
		tooltip_manager->SetTooltipDialogID(GetDialogKey());
	}
}

void uiCharacterStatusInfo2::SetParameterCaptionWithSecondReverse( UIControlPos control_position, int calc_value, int base_value )
{
	CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);

	if (control_ptr->IsKind(CTRL_KIND_TEXTIMAGE))
	{
		SetControlReverseTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr),
			(calc_value - base_value),
			IsMaxParameterCaption(control_position, calc_value));

		TCHAR string_format[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR string_message[INTERFACE_STRING_LENGTH] = {0, };

		// 2149 : 초
		g_InterfaceManager.GetInterfaceString(2149, string_format, INTERFACE_STRING_LENGTH);
		Snprintf(string_message, INTERFACE_STRING_LENGTH-1, "%d%s", calc_value, string_format);

		static_cast<CCtrlImageStaticWZ*>(control_ptr)->SetTextWZ(string_message);
	}
}

#endif // _NA_008472_20150903_IMPROVED_BALANCE
