#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusInfo.h"
#include "uiCharacterStatus/uiCharacterStatusMain.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "CommonCharacterFormula.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "PacketRequestManager.h"
#include "GameOptionParser.h"
#include "Mouse.h"
#ifdef _NA_002935_20110704_ITEM_SCORE
#include "equipmentcontainer.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
#include "uiCharacterStatusInfo2.h"
#endif // _NA_008472_20150903_IMPROVED_BALANCE

const DWORD uiCharacterStatusInfo::kDialog_ID = StrToWzID("0402");
#ifdef _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY
  const int ATTRUBUTE_MAX_PHYSICAL_ATTACK_RATE = 99;
  const int ATTRUBUTE_MAX_PHYSICAL_AVOID_RATE = 80;
#else
  const int ATTRUBUTE_MAX_PHYSICAL_ATTACK_AND_AVOID_RATE = 99;
#endif //_NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY
const int ATTRUBUTE_MAX_PHYSICAL_CRITICAL_RATIO = 100;
const int ATTRUBUTE_MAX_REDUCE_ELEMENT_DAMAGE_RATIO = 80;

uiCharacterStatusInfo::uiCharacterStatusInfo(InterfaceManager* interface_manager):
uiBase(interface_manager)
{

}

uiCharacterStatusInfo::~uiCharacterStatusInfo()
{

}

void uiCharacterStatusInfo::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C032"), kControl_CheckBT_C032);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("ST00"), kControl_Text_ST00);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("TI04"), kControl_TextImage_TI04);
    RegisterControlPosition(StrToWzID("TI33"), kControl_TextImage_TI33);
    RegisterControlPosition(StrToWzID("TI08"), kControl_TextImage_TI08);
    RegisterControlPosition(StrToWzID("TI05"), kControl_TextImage_TI05);
    RegisterControlPosition(StrToWzID("I001"), kControl_TextImage_I001);
    RegisterControlPosition(StrToWzID("TI12"), kControl_TextImage_TI12);
    RegisterControlPosition(StrToWzID("TI32"), kControl_TextImage_TI32);
    RegisterControlPosition(StrToWzID("TI13"), kControl_TextImage_TI13);
    RegisterControlPosition(StrToWzID("TI09"), kControl_TextImage_TI09);
    RegisterControlPosition(StrToWzID("TI11"), kControl_TextImage_TI11);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	RegisterControlPosition(StrToWzID("I003"), kControl_TextImage_I003);
	RegisterControlPosition(StrToWzID("I004"), kControl_TextImage_I004);
	RegisterControlPosition(StrToWzID("I005"), kControl_TextImage_I005);
#else
    RegisterControlPosition(StrToWzID("I000"), kControl_TextImage_I000);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    RegisterControlPosition(StrToWzID("TI14"), kControl_TextImage_TI14);
    RegisterControlPosition(StrToWzID("TI16"), kControl_TextImage_TI16);
    RegisterControlPosition(StrToWzID("TI18"), kControl_TextImage_TI18);
    RegisterControlPosition(StrToWzID("TI20"), kControl_TextImage_TI20);
    RegisterControlPosition(StrToWzID("TI15"), kControl_TextImage_TI15);
    RegisterControlPosition(StrToWzID("TI21"), kControl_TextImage_TI21);
    RegisterControlPosition(StrToWzID("TI25"), kControl_TextImage_TI25);
    RegisterControlPosition(StrToWzID("TI22"), kControl_TextImage_TI22);
    RegisterControlPosition(StrToWzID("TI24"), kControl_TextImage_TI24);
    RegisterControlPosition(StrToWzID("TI23"), kControl_TextImage_TI23);
    RegisterControlPosition(StrToWzID("TI26"), kControl_TextImage_TI26);
    RegisterControlPosition(StrToWzID("TI28"), kControl_TextImage_TI28);
    RegisterControlPosition(StrToWzID("TI31"), kControl_TextImage_TI31);
    RegisterControlPosition(StrToWzID("TI29"), kControl_TextImage_TI29);
    RegisterControlPosition(StrToWzID("TI30"), kControl_TextImage_TI30);
    RegisterControlPosition(StrToWzID("TI27"), kControl_TextImage_TI27);

#ifdef _NA_002935_20110704_ITEM_SCORE
    RegisterControlPosition(StrToWzID("I002"), kControl_TextImage_I002);
#endif //_NA_002935_20110704_ITEM_SCORE

    simulation_info().simulation_player_attributes.Clear();
    simulation_info().pressed_button_index = kControl_Size;
    simulation_info().pressed_button_time = clock_function::GetTickCount();
    simulation_info().ResetStatCount();
}

void uiCharacterStatusInfo::Release()
{
    simulation_info().simulation_player_attributes.Clear();
}

void uiCharacterStatusInfo::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
		if (ui_status_manager != NULL)
		{
			TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
			if (tab_manager.GetPrevTabIndex() != kControl_CheckBT_C003)
			{
				simulation_info().ResetStatCount();
				RefreshSimulationPlayerAttributes();
			}
		}
#else
        simulation_info().ResetStatCount();
        RefreshSimulationPlayerAttributes();
#endif // _NA_008472_20150903_IMPROVED_BALANCE

        CControlWZ* picture_control = GetControlWZ_byPosition(kControl_Picture_P003);
        if (picture_control != NULL)
        {
            picture_control->SetDiscardMessage(true);
            picture_control->ShowWindowWZ(WZ_HIDE);
        }

        RefreshTab();
    }
    else
    {
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
		if (ui_status_manager != NULL)
		{
			TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
			if (tab_manager.GetCurrentTabIndex() != kControl_CheckBT_C003)
			{
				simulation_info().ResetStatCount();
				simulation_info().simulation_player_attributes.Clear();
			}
		}
		else
		{
			simulation_info().ResetStatCount();
			simulation_info().simulation_player_attributes.Clear();
		}
#else
        simulation_info().ResetStatCount();
        simulation_info().simulation_player_attributes.Clear();
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    }
}

void uiCharacterStatusInfo::MessageProc(SI_MESSAGE* message)
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

void uiCharacterStatusInfo::OnUpdateSolarDialog()
{
    if (g_pHero == NULL)
    {
        return;
    }

    // 스탯 시뮬레이션 버튼 누름 처리
    ProcessStatbutton();

    // 능력치 업데이트
    UpdateParameter();

    // 초기화 포인트 업데이트

    if (g_pHero->GetLevel() <= GameOptionParser::Instance()->GetFreeStatInitLevel())
    {
        ShowFreeStatInitControls( WZ_SHOW );

        CCtrlImageStaticWZ* init_point_control = static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_TI33));
        if (init_point_control != NULL)
        {
            TCHAR point_string[INTERFACE_STRING_LENGTH] = {'\0', };
            Snprintf(point_string, INTERFACE_STRING_LENGTH-1, _T("%d"), g_pHero->GetCharInfo()->m_byFreeInitCount);
            init_point_control->SetTextWZ(point_string);
        }
    }
    else
    {
        ShowFreeStatInitControls( WZ_HIDE );
    }

    if (GetDialogWZ()->HaveFocus() == true)
    {
        GlobalFunc::MoveToTopDialog(uiCharacterStatusMain::kDialog_ID);
    }
}

void uiCharacterStatusInfo::OnRenderSolarDialog()
{
    // 툴팁 출력
    if (IsLocked() == false)
    {
        RenderTooltip();
    }
}

void uiCharacterStatusInfo::RefreshTab()
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
        check_button->SetCheckState((position == kControl_CheckBT_C000) ? (true) : (false));
    }
}

void uiCharacterStatusInfo::ProcessStatbutton()
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(simulation_info().pressed_button_index);
    if (IsOverControl(control_ptr) && Mouse::Instance()->LButtonPressed() == TRUE)
    {
        DWORD cur_tick = clock_function::GetTickCount();
        DWORD gab_tick = cur_tick - simulation_info().pressed_button_time;

        int add_count = ((int)gab_tick - 0) / 1000;
        if (add_count > 0)
        {
            UpdateSimulationPlayerAttributes(simulation_info().pressed_button_index, add_count);
        }
    }
    else
    {
        simulation_info().pressed_button_index = kControl_Size;
        simulation_info().pressed_button_time = clock_function::GetTickCount();
    }

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P003);
    if (control_ptr != NULL)
    {
        static DWORD alpha_value = 0;
        static DWORD cur_tick = clock_function::GetTickCount();
        static DWORD prev_tick = clock_function::GetTickCount();
        cur_tick = clock_function::GetTickCount();

        if (IsSimulationStatChanged() == true)
        {
            alpha_value += ((cur_tick - prev_tick) * 2);
            alpha_value  = alpha_value % int(PI * 1000);

            float alpha = abs(sin(alpha_value * 0.001f));

            control_ptr->SetTransprency(alpha);
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            alpha_value = 0;
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        prev_tick = cur_tick;
    }
}

void uiCharacterStatusInfo::UpdateParameter()
{
    PlayerAttributes* player_attributes = &(simulation_info().simulation_player_attributes);
    const BASE_PLAYERINFO* player_info = simulation_info().simulation_player.GetCharacterInfo();

    // 기본 5스텟 종류
    SetParameterCaption(kControl_TextImage_TI14, player_attributes->GetSTR(), player_attributes->GetBaseSTR()); //!< 힘
    SetParameterCaption(kControl_TextImage_TI16, player_attributes->GetDEX(), player_attributes->GetBaseDEX()); //!< 민첩
    SetParameterCaption(kControl_TextImage_TI18, player_attributes->GetVIT(), player_attributes->GetBaseVIT()); //!< 체력
    SetParameterCaption(kControl_TextImage_TI20, player_attributes->GetINT(), player_attributes->GetBaseINT()); //!< 지력
    SetParameterCaption(kControl_TextImage_TI15, player_attributes->GetSPR(), player_attributes->GetBaseSPR()); //!< 정신력

    // 남아있는 스텟포인트
    SetParameterCaption(kControl_TextImage_TI21, player_info->m_iRemainStat);

    // 공격력 및 방어력
    // 데미지
    CCtrlImageStaticWZ *pControl = static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_TI04));
    if (g_pHero->IsRangedNormalAttack())
    {
        SetParameterDamage(pControl, 
            player_attributes->GetBaseRangeMinAttPower(), 
            player_attributes->GetBaseRangeMaxAttPower());
    }
    else 
    {
        SetParameterDamage(pControl, 
            player_attributes->GetBaseMeleeMinAttPower(), 
            player_attributes->GetBaseMeleeMaxAttPower());
    }

    // 물리데미지 옵션
    SetParameterCaption(kControl_TextImage_TI05, 
        player_attributes->GetOptionPhysicalAttPower(), 
        player_attributes->GetBaseOptionPhysicalAttPower());

    //물리방어력
    if (g_pHero->IsRangedNormalAttack())
    {
        SetParameterCaption(kControl_TextImage_TI08, 
            player_attributes->GetBaseRangeDefPower(),// + player_attributes->GetOptionPhysicalDefPower(), 
            player_attributes->GetBaseRangeDefPower());
    }
    else
    {
        SetParameterCaption(kControl_TextImage_TI08, 
            player_attributes->GetBaseMeleeDefPower(),// + player_attributes->GetOptionPhysicalDefPower(), 
            player_attributes->GetBaseMeleeDefPower());
    }

    // 물리방어력 옵션
    SetParameterCaption(kControl_TextImage_I001, 
        player_attributes->GetOptionPhysicalDefPower(), 
        player_attributes->GetBaseOptionPhysicalDefPower());

    // 기타 능력치
    //크리티컬확률
    SetParameterCaptionWithPercent(kControl_TextImage_TI11,
        player_attributes->GetCriticalRatioChange(),
        player_attributes->GetBaseCriticalRatioChange());


#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	// 크리티컬 피격확률
	SetParameterCaptionWithPercentReverseTextColor(kControl_TextImage_I003, 
		player_attributes->GetEnemyCriticalRatioChange(), 
		player_attributes->GetBaseEnemyCriticalRatioChange());

	// 크리티컬 데미지증가
	SetParameterCaption(kControl_TextImage_I004, 
		player_attributes->GetCriticalDamageBonus(), 
		player_attributes->GetBaseCriticalDamageBonus());

	// 크리티컬 데미지증가(%)
	SetParameterCaptionWithPercent(kControl_TextImage_I005, 
		player_attributes->GetRatioCriticalDamageBonus(), 
		player_attributes->GetBaseCriticalDamageBonus());
#else
    //! 추가 스킬 공격력
    SetParameterCaption(kControl_TextImage_I000, 
        player_attributes->GetSkillAttackPower(), 
        player_attributes->GetBaseSkillAttackPower());
#endif // _NA_008472_20150903_IMPROVED_BALANCE

    // 물리공격회피율
    SetParameterCaptionWithPercent(kControl_TextImage_TI12, 
        player_attributes->GetPhysicalAvoidRate(),
        player_attributes->GetBasePhysicalAvoidRate());


    int base_attack_speed = 0;
    int base_attack_rate = 0;
    if (g_pHero->GetWeaponInfo() != NULL)
    {
        base_attack_speed = g_pHero->GetWeaponInfo()->m_wPhyAttSpeed;
        base_attack_rate = g_pHero->GetWeaponInfo()->m_wPhyAttRate;
    }

    // 물리공격성공률
    SetParameterCaptionWithPercent(kControl_TextImage_TI09, 
        player_attributes->GetPhysicalAttackRate(), 
        base_attack_rate + player_attributes->GetBasePhysicalAttackRate());

    //공격 속도
    SetParameterCaption(kControl_TextImage_TI13,
        player_attributes->GetAttSpeedRatio(),  //그냥 비율
        ns_formula::CalcAttackSpeedRatio(g_pHero->GetClass(), 
        g_pHero->GetWeaponKind(), 
        player_attributes->GetDEX(), base_attack_speed, 0));
    //이동 속도
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    SetParameterCaption(kControl_TextImage_TI32, 
        player_attributes->GetMoveSpeedRatio(), 
        ns_formula::CalcMoveSpeedRatio(0, 0, g_pHero->GetLevel()));
#else
    SetParameterCaption(kControl_TextImage_TI32, 
        player_attributes->GetMoveSpeedRatio(), 
        ns_formula::CalcMoveSpeedRatio(player_attributes->GetDEX(), 0, 0));
#endif //_NA_008486_20150914_TOTAL_BALANCE

    // 속성 공격력/저항률
    int over_value = 0;
    // 화 : 속성 공격력
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_FIRE);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_FIRE, eATTR_KIND_BASE);
    SetParameterCaption(kControl_TextImage_TI25, over_value);

    // 수
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WATER);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WATER, eATTR_KIND_BASE);
    SetParameterCaption(kControl_TextImage_TI22, over_value);

    // 지
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_EARTH);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_EARTH, eATTR_KIND_BASE);
    SetParameterCaption(kControl_TextImage_TI24, over_value);

    // 풍
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WIND);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WIND, eATTR_KIND_BASE);
    SetParameterCaption(kControl_TextImage_TI23, over_value);

    // 암
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_DARKNESS);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_DARKNESS, eATTR_KIND_BASE);
    SetParameterCaption(kControl_TextImage_TI26, over_value);


    // 화 : 속성 저항률
    SetParameterCaptionWithPercent(kControl_TextImage_TI28, 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_FIRE), 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_FIRE, eATTR_KIND_BASE));
    // 수
    SetParameterCaptionWithPercent(kControl_TextImage_TI31, 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WATER),
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WATER, eATTR_KIND_BASE));

    // 지
    SetParameterCaptionWithPercent(kControl_TextImage_TI29, 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_EARTH),
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_EARTH, eATTR_KIND_BASE));

    // 풍
    SetParameterCaptionWithPercent(kControl_TextImage_TI30, 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WIND), 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WIND, eATTR_KIND_BASE));
    
    // 암
    SetParameterCaptionWithPercent(kControl_TextImage_TI27, 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_DARKNESS), 
        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_DARKNESS, eATTR_KIND_BASE));

#ifdef _NA_002935_20110704_ITEM_SCORE
    // 장비평점
    int item_score = g_pHero->GetItemScore();
    SetParameterCaption(kControl_TextImage_I002, item_score, item_score);
#endif //_NA_002935_20110704_ITEM_SCORE
}

void uiCharacterStatusInfo::ShowFreeStatInitControls(ENUM_STATEWZ show_state)
{
    for (DWORD control_position = kControl_TextImage_TI33; control_position < kControl_Text_ST00 + 1; ++control_position)
    {
        CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(show_state);
        }
    }
}

eATTR_TYPE uiCharacterStatusInfo::GetAttrTypeFromButtonPos(UIControlPos control_position)
{
    switch (control_position)
    {
    case kControl_Button_B000:
    case kControl_Button_B005:
        return eATTR_STR;
    case kControl_Button_B002:
    case kControl_Button_B011:
        return eATTR_DEX;
    case kControl_Button_B004:
    case kControl_Button_B012:
        return eATTR_VIT;
    case kControl_Button_B006:
    case kControl_Button_B013:
        return eATTR_INT;
    case kControl_Button_B001:
    case kControl_Button_B014:
        return eATTR_SPR;
    }

    return eATTR_TYPE_INVALID;
}

void uiCharacterStatusInfo::UpdateSimulationPlayerAttributes(UIControlPos control_position, int add_value)
{
    eATTR_TYPE attr_type = GetAttrTypeFromButtonPos(control_position);
    if (attr_type == eATTR_TYPE_INVALID)
    {
        return;
    }

    int org_value = g_pHero->GetPlayerAttribute()->GetAttrValue(attr_type);
    int cur_value = simulation_info().simulation_player_attributes.GetAttrValue(attr_type);
    int base_value = simulation_info().simulation_player_attributes.GetAttrValue(attr_type, eATTR_KIND_BASE);

    switch (control_position)
    {
        // [-] 버튼을 누름
    case kControl_Button_B005:
    case kControl_Button_B011:
    case kControl_Button_B012:
    case kControl_Button_B013:
    case kControl_Button_B014:
        {
            add_value = -add_value;
        }
        break;
    }

    if (add_value < 0)
    {
        if ((cur_value <= org_value) || (simulation_info().simulation_stat_count[attr_type] <= 0))
        {
            return;
        }
        if (cur_value + add_value < org_value)
        {
            //! 기본값 보다는 크거나 같게 한다.
            add_value = org_value - cur_value;
        }
    }
    else if (add_value > 0)
    {
        //! 스텟 포인트 최대가 1000
        const int kMaxStatValue = 1000;

        if ((cur_value >= kMaxStatValue) || (simulation_info().simulation_stat_count[attr_type] >= kMaxStatValue))
        {
            return;
        }
        if ((cur_value + add_value) > kMaxStatValue)
        {
            add_value = kMaxStatValue - cur_value;
        }
        if (add_value > simulation_info().simulation_player.GetCharacterInfo()->m_iRemainStat)
        {
            add_value = simulation_info().simulation_player.GetCharacterInfo()->m_iRemainStat;
        }
    }

    if (add_value == 0)
    {
        return;
    }

    simulation_info().simulation_player_attributes.SetAttrValue(attr_type, eATTR_KIND_BASE, base_value + add_value);
    simulation_info().simulation_player_attributes.Update();

    int apply_value = simulation_info().simulation_player_attributes.GetAttrValue(attr_type);

    simulation_info().simulation_player.GetCharacterInfo()->m_iRemainStat -= add_value;    //(add_value - (next_value - apply_value));

    //! 변경 스텟 카운트를 저장한다.
    simulation_info().simulation_stat_count[attr_type] += add_value;

    GetControlWZ_byPosition(kControl_Button_B015)->EnableWindowWZ(IsSimulationStatChanged());
}

bool uiCharacterStatusInfo::IsSimulationStatChanged()
{
    return (g_pHero->GetCharInfo()->m_iRemainStat != simulation_info().simulation_player.GetCharacterInfo()->m_iRemainStat);
}

void uiCharacterStatusInfo::RefreshStatButton()
{
    ENUM_STATEWZ show_state = WZ_HIDE;
    if (g_pHero->GetCharInfo()->m_iRemainStat > 0)
    {
        show_state = WZ_SHOW;
    }

    GetControlWZ_byPosition(kControl_Button_B000)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B005)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B002)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B011)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B004)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B012)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B006)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B013)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B001)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B014)->ShowWindowWZ(show_state);
    GetControlWZ_byPosition(kControl_Button_B015)->ShowWindowWZ(show_state);
    if (show_state == WZ_SHOW)
    {
        GetControlWZ_byPosition(kControl_Button_B015)->EnableWindowWZ(IsSimulationStatChanged());
    }
}

void uiCharacterStatusInfo::SEND_STATUS_STAT_APPLY_SYN()
{
    if (PacketRequestManager::Instance()->is_sent(CG_STATUS, CG_STATUS_STAT_APPLY_SYN))
    {
        return;
    }
    if (IsSimulationStatChanged() == false)
    {
        return;
    }

    MSG_CG_STATUS_STAT_APPLY_SYN syn;
    syn.stat_point[eATTR_STR] = simulation_info().simulation_stat_count[eATTR_STR];
    syn.stat_point[eATTR_DEX] = simulation_info().simulation_stat_count[eATTR_DEX];
    syn.stat_point[eATTR_VIT] = simulation_info().simulation_stat_count[eATTR_VIT];
    syn.stat_point[eATTR_INT] = simulation_info().simulation_stat_count[eATTR_INT];
    syn.stat_point[eATTR_SPR] = simulation_info().simulation_stat_count[eATTR_SPR];

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &syn, sizeof(syn));


    PacketRequestManager::Instance()->PushPacket((new MSG_CG_STATUS_STAT_APPLY_SYN),
        (new MSG_CG_STATUS_STAT_APPLY_ACK), 
        (new MSG_CG_STATUS_STAT_APPLY_NAK),
        sizeof(MSG_CG_STATUS_STAT_APPLY_SYN), 
        PACKET_WAITING_MILLISECOND, 
        false);
}

void uiCharacterStatusInfo::RefreshSimulationPlayerAttributes()
{
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	uiCharacterStatusInfo2* character_status_info2 = GET_CAST_DIALOG(uiCharacterStatusInfo2, uiCharacterStatusInfo2::kDialog_ID);
	if (IsVisible() == FALSE && 
		character_status_info2 && 
		character_status_info2->IsVisible() == FALSE)
	{
		return;
	}
#else
    if (IsVisible() == FALSE)
    {
        return;
    }
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    if (g_pHero->GetCharInfo() == NULL)
    {
        return;
    }

    simulation_info().simulation_player.SetCharInfo(g_pHero->GetCharInfo());
    simulation_info().simulation_player.SetWeaponKind(g_pHero->GetWeaponKind());

    simulation_info().simulation_player_attributes.Clear();
    g_pHero->GetPlayerAttribute()->Clone(&(simulation_info().simulation_player_attributes));
    simulation_info().simulation_player_attributes.SetOwner(&simulation_info().simulation_player);

    int cur_str_stat_count = simulation_info().simulation_player_attributes.GetAttrValue(eATTR_STR, eATTR_KIND_BASE);
    int cur_dex_stat_count = simulation_info().simulation_player_attributes.GetAttrValue(eATTR_DEX, eATTR_KIND_BASE);
    int cur_vit_stat_count = simulation_info().simulation_player_attributes.GetAttrValue(eATTR_VIT, eATTR_KIND_BASE);
    int cur_int_stat_count = simulation_info().simulation_player_attributes.GetAttrValue(eATTR_INT, eATTR_KIND_BASE);
    int cur_spr_stat_count = simulation_info().simulation_player_attributes.GetAttrValue(eATTR_SPR, eATTR_KIND_BASE);

    simulation_info().simulation_player_attributes.SetAttrValue(eATTR_STR, eATTR_KIND_BASE, cur_str_stat_count + simulation_info().simulation_stat_count[eATTR_STR]);
    simulation_info().simulation_player_attributes.SetAttrValue(eATTR_DEX, eATTR_KIND_BASE, cur_dex_stat_count + simulation_info().simulation_stat_count[eATTR_DEX]);
    simulation_info().simulation_player_attributes.SetAttrValue(eATTR_VIT, eATTR_KIND_BASE, cur_vit_stat_count + simulation_info().simulation_stat_count[eATTR_VIT]);
    simulation_info().simulation_player_attributes.SetAttrValue(eATTR_INT, eATTR_KIND_BASE, cur_int_stat_count + simulation_info().simulation_stat_count[eATTR_INT]);
    simulation_info().simulation_player_attributes.SetAttrValue(eATTR_SPR, eATTR_KIND_BASE, cur_spr_stat_count + simulation_info().simulation_stat_count[eATTR_SPR]);

    simulation_info().simulation_player_attributes.Update();

    simulation_info().simulation_player.GetCharacterInfo()->m_iRemainStat -= (simulation_info().simulation_stat_count[eATTR_STR] + 
        simulation_info().simulation_stat_count[eATTR_DEX] + 
        simulation_info().simulation_stat_count[eATTR_VIT] + 
        simulation_info().simulation_stat_count[eATTR_INT] + 
        simulation_info().simulation_stat_count[eATTR_SPR]);

    RefreshStatButton();
}

void uiCharacterStatusInfo::ResetSimulationStatCount()
{
    simulation_info().ResetStatCount();
}

void uiCharacterStatusInfo::CallbackSendPacketStatusStatApply(bool is_yes)
{
    if (is_yes)
    {
        uiCharacterStatusInfo* status_info_dialog = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);
        if ((status_info_dialog != NULL) && (status_info_dialog->IsVisible() == TRUE))
        {
            status_info_dialog->SEND_STATUS_STAT_APPLY_SYN();
        }
    }
}

void uiCharacterStatusInfo::SetParameterCaption(UIControlPos control_position, int val)
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

void uiCharacterStatusInfo::SetParameterCaption(UIControlPos control_position, int calc_value, int base_value)
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

void uiCharacterStatusInfo::SetParameterDamage(CCtrlImageStaticWZ* control_ptr,int min_value,int max_value)
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

void uiCharacterStatusInfo::SetParameterCaptionWithPercent(UIControlPos control_position, int calc_value, int base_value)
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

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
void uiCharacterStatusInfo::SetParameterCaptionWithPercentReverseTextColor( UIControlPos control_position, int calc_value, int base_value )
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
#endif // _NA_008472_20150903_IMPROVED_BALANCE

void uiCharacterStatusInfo::SetControlTextColor(CCtrlImageStaticWZ* control_ptr, int val, bool is_max /* =false */)
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

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
void uiCharacterStatusInfo::SetControlReverseTextColor( CCtrlImageStaticWZ* control_ptr, int val, bool is_max /*= false*/ )
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
#endif // _NA_008472_20150903_IMPROVED_BALANCE

void uiCharacterStatusInfo::RenderTooltip()
{
    PlayerAttributes* player_attributes = &(simulation_info().simulation_player_attributes);
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    if ((tooltip_manager == NULL) || (player_attributes == NULL))
    {
        return;
    }
    // 속성 공격력
    if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI25)))        RenderTooltip_StringCode(tooltip_manager, 11490);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI22)))   RenderTooltip_StringCode(tooltip_manager, 11487);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI24)))   RenderTooltip_StringCode(tooltip_manager, 11489);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI23)))   RenderTooltip_StringCode(tooltip_manager, 11488);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI26)))   RenderTooltip_StringCode(tooltip_manager, 11491);
    // 속성 저항률
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI28)))   RenderTooltip_StringCode(tooltip_manager, 11484);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI31)))   RenderTooltip_StringCode(tooltip_manager, 11481);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI29)))   RenderTooltip_StringCode(tooltip_manager, 11483);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI30)))   RenderTooltip_StringCode(tooltip_manager, 11482);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI27)))   RenderTooltip_StringCode(tooltip_manager, 11485);
    // 기본 능력치
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI14)))   RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseSTR(), player_attributes->GetSTR(), true, 11560);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI16)))   RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseDEX(), player_attributes->GetDEX(), true, 11561);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI18)))   RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseVIT(), player_attributes->GetVIT(), true, 11562);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI20)))   RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseINT(), player_attributes->GetINT(), true, 11563);
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI15)))   RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseSPR(), player_attributes->GetSPR(), true, 11564);

    // 기타 능력치
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI09)))
    {
        // 물리공격성공률
        int base_attack_rate = 0;
        if (g_pHero->GetWeaponInfo() != NULL)
        {
            base_attack_rate = g_pHero->GetWeaponInfo()->m_wPhyAttRate;
        }

        RenderTooltip_BaseAndCalc(tooltip_manager, 
            base_attack_rate + player_attributes->GetBasePhysicalAttackRate(),
            player_attributes->GetPhysicalAttackRate());
    }
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI12)))
    {
        // 물리공격회피율
        RenderTooltip_BaseAndCalc(tooltip_manager, 
            player_attributes->GetBasePhysicalAvoidRate(),
            player_attributes->GetPhysicalAvoidRate());
    }
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI32)))
    {
        // 이동 속도
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        RenderTooltip_BaseAndCalc(tooltip_manager, 
            ns_formula::CalcMoveSpeedRatio(0, 0, g_pHero->GetLevel()), 
            player_attributes->GetMoveSpeedRatio());
#else
        RenderTooltip_BaseAndCalc(tooltip_manager, 
            ns_formula::CalcMoveSpeedRatio(player_attributes->GetDEX(), 0, 0), 
            player_attributes->GetMoveSpeedRatio());
#endif //_NA_008486_20150914_TOTAL_BALANCE
    }
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI13)))
    {
        // 공격 속도
        int base_attack_speed = 0;
        if (g_pHero->GetWeaponInfo() != NULL)
        {
            base_attack_speed = g_pHero->GetWeaponInfo()->m_wPhyAttSpeed;
        }

        RenderTooltip_BaseAndCalc(tooltip_manager, 
            ns_formula::CalcAttackSpeedRatio(g_pHero->GetClass(), 
            g_pHero->GetWeaponKind(), 
            player_attributes->GetDEX(), base_attack_speed, 0), 
            player_attributes->GetAttSpeedRatio());
    }
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_TI11)))
    {
        // 크리티컬확률
        RenderTooltip_BaseAndCalc(tooltip_manager, 
            player_attributes->GetBaseCriticalRatioChange(), 
            player_attributes->GetCriticalRatioChange());
    }
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I003)))
	{
		// 크리티컬 피격확률
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseEnemyCriticalRatioChange(), 
			player_attributes->GetEnemyCriticalRatioChange());
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I004)))
	{
		// 크리티컬 데미지증가
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseCriticalDamageBonus(), 
			player_attributes->GetCriticalDamageBonus());			
	}
	else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I005)))
	{
		// 크리티컬 데미지증가(%)
		RenderTooltip_BaseAndCalc(tooltip_manager, 
			player_attributes->GetBaseCriticalDamageBonus(), 
			player_attributes->GetRatioCriticalDamageBonus());
	}
#else
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I000)))
    {
        // 추가 스킬 공격력
        RenderTooltip_BaseAndCalc(tooltip_manager, 
            player_attributes->GetBaseSkillAttackPower(), 
            player_attributes->GetSkillAttackPower());
    }
#endif // _NA_008472_20150903_IMPROVED_BALANCE
#ifdef _NA_002935_20110704_ITEM_SCORE
    else if (IsOverControl(GetControlWZ_byPosition(kControl_TextImage_I002)))
    {
        // 11478 : 장비 평점
		RenderTooltip_StringCode( tooltip_manager, 11478);
    }
#endif //_NA_002935_20110704_ITEM_SCORE
    else if (IsOverControl(GetControlWZ_byPosition(kControl_Button_B003)))
    {
        if (GetControlWZ_byPosition(kControl_Button_B003)->GetState() == WZ_SHOW)
        {
            TCHAR limit_level[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR limit_count[INTERFACE_STRING_LENGTH] = {0,};

            _itot(GameOptionParser::Instance()->GetFreeStatInitLevel(), limit_level, 10);
            _itot(g_pHero->GetCharInfo()->m_byFreeInitCount, limit_count, 10);

            // 6297	%s레벨까지 무료로 스텟과 스킬을 초기화 할 수 있습니다.\n현재 남은 횟수 : %s회
            TCHAR tooltip_text[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceStringFormat(tooltip_text,
                INTERFACE_STRING_LENGTH,
                6297,
                limit_level,
                limit_count);
            tooltip_manager->RegisterTooltipSimpleText(tooltip_text);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }

}

void uiCharacterStatusInfo::RenderTooltip_StringCode(uiToolTipMan* tooltip_manager, int StringCode )
{
	tooltip_manager->RegisterTooltipWithStringCode( StringCode );
	tooltip_manager->SetTooltipDialogID(GetDialogKey());
}

void uiCharacterStatusInfo::RenderTooltip_BaseAndCalc(uiToolTipMan* tooltip_manager, 
                                                      int base_value, 
                                                      int calc_value, 
                                                      bool is_force /*= true*/, 
                                                      DWORD string_code /*= 0*/)
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

bool uiCharacterStatusInfo::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	case kControl_CheckBT_C003:
#endif // _NA_008472_20150903_IMPROVED_BALANCE
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
    case kControl_Button_B015:
        {
            // 포인트 적용 버튼
            uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (system_manager != NULL)
            {
                TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
                if ((g_pHero != NULL) && (g_pHero->IsTransForm() == TRUE))
                {
                    // 5139	변신 상태에서는 이용이 가능하지 않습니다.
                    g_InterfaceManager.GetInterfaceString(5139, message ,INTERFACE_STRING_LENGTH);
                    system_manager->Sys_VerifyLock(message);
                }
                else
                {
                    // 2800 변경한 스탯 포인트를 적용하시겠습니까?
                    g_InterfaceManager.GetInterfaceString(2800, message, INTERFACE_STRING_LENGTH);
                    system_manager->Sys_ConfirmLock(message, uiCharacterStatusInfo::CallbackSendPacketStatusStatApply);
                }
            }
        }
        break;
    case kControl_Button_B003:
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
            if ((g_pHero != NULL) && (g_pHero->IsTransForm() == TRUE))
            {
                uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                if (system_manager != NULL)
                {
                    // 5139	변신 상태에서는 이용이 가능하지 않습니다.
                    g_InterfaceManager.GetInterfaceString(5139, message ,INTERFACE_STRING_LENGTH);
                    system_manager->Sys_VerifyLock(message);
                }
            }
            else
            {
                TCHAR limit_level[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR limit_count[INTERFACE_STRING_LENGTH] = {0,};
                _itot(GameOptionParser::Instance()->GetFreeStatInitLevel(), limit_level, 10);
                _itot(g_pHero->GetCharInfo()->m_byFreeInitCount, limit_count, 10);

                // 6291 %s 레벨까지 모든 스텟을 5회 초기화할 수 있습니다. 단, %s 레벨을 초과하면 사용할 수 없습니다.\n 남은 횟수 : %s 회
                TCHAR tooltip_text[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceStringFormat(tooltip_text,
                    INTERFACE_STRING_LENGTH,
                    6291,
                    limit_level,
                    limit_level,
                    limit_count);

                // 초기화 버튼
                uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                system_manager->Sys_Confirm(tooltip_text, GlobalFunc::SendStateInitSyn);
            }
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

bool uiCharacterStatusInfo::OnLButtonDown(SI_MESSAGE* message)
{
    bool result = true;
    UIControlPos control_position = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_position)
    {
    case kControl_Button_B000:
    case kControl_Button_B005:
    case kControl_Button_B002:
    case kControl_Button_B011:
    case kControl_Button_B004:
    case kControl_Button_B012:
    case kControl_Button_B006:
    case kControl_Button_B013:
    case kControl_Button_B001:
    case kControl_Button_B014:
        {
            if ((g_pHero != NULL) && (g_pHero->IsTransForm() == TRUE))
            {
                uiSystemMan* system_main = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                if (system_main != NULL)
                {
                    TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
                    // 5139	변신 상태에서는 이용이 가능하지 않습니다.
                    g_InterfaceManager.GetInterfaceString(5139, message ,INTERFACE_STRING_LENGTH);
                    system_main->Sys_VerifyLock(message);
                }
            }
            else
            {
                simulation_info().pressed_button_time = clock_function::GetTickCount();
                simulation_info().pressed_button_index = control_position;
                UpdateSimulationPlayerAttributes(control_position, 1);
            }
        }
        break;
    default:
        {
        }
    }
    return result;
}

bool uiCharacterStatusInfo::OnLButtonUp( SI_MESSAGE* message )
{
    bool result = true;

    simulation_info().pressed_button_time = clock_function::GetTickCount();
    simulation_info().pressed_button_index = kControl_Size;
    
    return result;
}

bool uiCharacterStatusInfo::IsMaxParameterCaption(UIControlPos control_position, IN OUT int& calc_value)
{
    PlayerAttributes* player_attributes = &(simulation_info().simulation_player_attributes);

    int attribute_max_value = 0;
    bool check_enable = false;
    //최대값 조사
    switch(control_position)
    {
    case kControl_TextImage_TI14: //힘
        attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_STR);
        check_enable = true;
        break;
    case kControl_TextImage_TI16: //민첩
        attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_DEX);
        check_enable = true;
        break;
    case kControl_TextImage_TI18: //체력
        attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_VIT);
        check_enable = true;
        break;
    case kControl_TextImage_TI20: //지력
        attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_INT);
        check_enable = true;
        break;
    case kControl_TextImage_TI15: //정신력
        attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_SPR);
        check_enable = true;
        break;
    //case kControl_TextImage_TI32: //이동속도
    //    attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_MOVE_SPEED);
    //    check_enable = true;
    //    break;
    //case kControl_TextImage_TI13: //공격속도
    //    attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_ATTACK_SPEED);
    //    check_enable = true;
    //    break;
    //case kControl_TextImage_I000: //스킬강화력
    //    attribute_max_value = player_attributes->GetAttrMaxBoundValue(eATTR_ADD_SKILL_ATTACK_POWER);
    //    check_enable = true;
    //    break;

    // 2011. 08. 22 / 송찬종 
    // MAX 값은 변할 수 있는 값이므로 데이터로써 관리해야 하지만, 임시로 직접 값을 입력함
    // 기획자와 협의하고 수정 필요함
#ifdef _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY
    case kControl_TextImage_TI12: // 방어 성공률
        attribute_max_value = ATTRUBUTE_MAX_PHYSICAL_AVOID_RATE;
        check_enable = true;
        break;
    case kControl_TextImage_TI09: // 공격 성공률
        attribute_max_value = ATTRUBUTE_MAX_PHYSICAL_ATTACK_RATE;
        check_enable = true;
        break;
#else
    case kControl_TextImage_TI12: // 방어 성공률
    case kControl_TextImage_TI09: // 공격 성공률
        attribute_max_value = ATTRUBUTE_MAX_PHYSICAL_ATTACK_AND_AVOID_RATE;
        check_enable = true;
        break;
#endif //_NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY
    case kControl_TextImage_TI11: // 크리티컬 확률
        attribute_max_value = ATTRUBUTE_MAX_PHYSICAL_CRITICAL_RATIO;
        check_enable = true;
        break;
    case kControl_TextImage_TI28: // 화 저항
    case kControl_TextImage_TI31: // 수 저항
    case kControl_TextImage_TI29: // 지 저항
    case kControl_TextImage_TI30: // 풍 저항
    case kControl_TextImage_TI27: // 암 저항
        attribute_max_value = ATTRUBUTE_MAX_REDUCE_ELEMENT_DAMAGE_RATIO;
        check_enable = true;
        break;
    //원소 저항율은 무제한
#ifdef _NA_002935_20110704_ITEM_SCORE
    case kControl_TextImage_I002: // 장비평점
        attribute_max_value = kItemScoreMax;
        check_enable = true;
#endif //_NA_002935_20110704_ITEM_SCORE
    }

    if ((check_enable == true) && 
        (attribute_max_value <= calc_value))
    {
        calc_value = attribute_max_value;
        return true;
    }

    return false;
}

#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
void uiCharacterStatusInfo::ShowWindowRefreshSimulation()
{
	RefreshSimulationPlayerAttributes();
}
#endif // _NA_008472_20150903_IMPROVED_BALANCE