//------------------------------------------------------------------------------
//  Char_SkillDialog.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#else

#include "Global.h"
#include "char_skilldialog.h"
#include "interfacemanager.h"
#include "MainDialog.h"
#include "network.h"
#include "Hero.h"
#include "PlayerAttributes.h"
#include "CommonCharacterFormula.h"
#include "HeroActionInput.h"
#include "uiGuildMan/uiGuildMan.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "globalfunc.h"
#include "GuildMarkInfoParser.h"
#include "GameOptionParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "SCItemSlot.h"
#include "OptionListParser.h"
#include "HeroEquipmentContainer.h"
#include "PacketRequestManager.h"

WzID		Char_SkillDialog::m_wzId[] = 
{
	StrToWzID("TI03"),//CHAR_SKILL_VALUE_LEVEL = 0,	//LEVEL
	StrToWzID("TI00"),//CHAR_SKILL_VALUE_NAME,		//CHARACTER NAME 
	StrToWzID("P004"),//CHAR_SKILL_MARK_P,		//이것은 뭔지모르겠지만 아마도 마크같다.
	StrToWzID("TI02"),//CHAR_SKILL_VALUE_CLASS,		//CLASS
	StrToWzID("TI01"),//CHAR_SKILL_VALUE_GUILD_NAME,	//GUILD NAME	
	StrToWzID("B000"),//CHAR_SKILL_STR_B,		//힘 
	StrToWzID("B002"),//CHAR_SKILL_DEX_B,		//민첩
	StrToWzID("B004"),//CHAR_SKILL_VIT_B,		//체력
	StrToWzID("B006"),//CHAR_SKILL_INT_B,		//지능
	StrToWzID("B001"),//CHAR_SKILL_SPR_B,		//지혜

	StrToWzID("TI14"),//CHAR_SKILL_VALUE_STR,		//힘
	StrToWzID("TI04"),//CHAR_SKILL_VALUE_PHYSIC_ATTACK_MIN,//물리공격 min
	StrToWzID("TI16"),//CHAR_SKILL_VALUE_DEX,		//민첩
	StrToWzID("TI12"),//CHAR_SKILL_VALUE_AVOID,		//물리공격회피율
	StrToWzID("TI09"),//CHAR_SKILL_VALUE_PHYSIC_ATTACK_RATE,		//물리공격성공률
	StrToWzID("TI18"),//CHAR_SKILL_VALUE_STAMINA,		//체력
	StrToWzID("TI08"),//CHAR_SKILL_VALUE_PHYSIC_DEFENSE_RATE,		//물리방어력
	StrToWzID("TI13"),//CHAR_SKILL_VALUE_ATTACK_SPEED,		//공격속도
    StrToWzID("TI32"),//CHAR_SKILL_VALUE_MOVE_SPEED,		//이동속도
	StrToWzID("TI20"),//CHAR_SKILL_VALUE_WISDOM,		//지력

    StrToWzID("TI15"),//CHAR_SKILL_VALUE_SPIRIT,		//정신력
    StrToWzID("TI11"),//CHAR_SKILL_VALUE_CRITICAL_RATE,	//크리티컬확률 
    StrToWzID("B999"),//CHAR_SKILL_CLOSE,
    StrToWzID("TI21"),//CHAR_SKILL_STAT,		//BONUS POINT
    StrToWzID("TI05"),//CHAR_SKILL_VALUE_PHYSIC_ATTACK_MAX,	//물리데미지 max

    StrToWzID("I000"),//CHAR_SKILL_VALUE_MAGIC_ATTACK_MIN,	// 추가 스킬 공격력
    StrToWzID("I001"),//CHAR_SKILL_VALUE_PHYSIC_DEFENSE_MAX,		//물리방어력 max

	StrToWzID("TI22"),//CHAR_SKILL_VALUE_ATTR_WATER,	// 수
	StrToWzID("TI23"),//CHAR_SKILL_VALUE_ATTR_WIND,		// 풍
	StrToWzID("TI24"),//CHAR_SKILL_VALUE_ATTR_GROUND,	// 지
	StrToWzID("TI25"),//CHAR_SKILL_VALUE_ATTR_FIRE,		// 화
	StrToWzID("TI26"),//CHAR_SKILL_VALUE_ATTR_DARK,		// 암

    StrToWzID("TI31"),//CHAR_SKILL_VALUE_ATTR_WATER2,	// 수
    StrToWzID("TI30"),//CHAR_SKILL_VALUE_ATTR_WIND2,		// 풍
    StrToWzID("TI29"),//CHAR_SKILL_VALUE_ATTR_GROUND2,	// 지
    StrToWzID("TI28"),//CHAR_SKILL_VALUE_ATTR_FIRE2,		// 화
    StrToWzID("TI27"),//CHAR_SKILL_VALUE_ATTR_DARK2,		// 암

	StrToWzID("P023"),	// CHAR_SKILL_ICON_ATTR_WATER	
	StrToWzID("P025"),	// CHAR_SKILL_ICON_ATTR_WIND
	StrToWzID("P027"),	// CHAR_SKILL_ICON_ATTR_GROUND
	StrToWzID("P029"),	// CHAR_SKILL_ICON_ATTR_FIRE
	StrToWzID("P031"),	// CHAR_SKILL_ICON_ATTR_DARK

	StrToWzID("P022"),	// CHAR_SKILL_ICON_ATTR_WATER2
	StrToWzID("P024"),	// CHAR_SKILL_ICON_ATTR_WIND2
	StrToWzID("P026"),	// CHAR_SKILL_ICON_ATTR_GROUND2
	StrToWzID("P028"),	// CHAR_SKILL_ICON_ATTR_FIRE2
	StrToWzID("P030"),	// CHAR_SKILL_ICON_ATTR_DARK2

    StrToWzID("B003"),//CHAR_SKILL_STATE_INIT_BTN,		//리셋

	StrToWzID("TI33"),	// 
	StrToWzID("ST00"),

    StrToWzID("TI34"),  //CHAR_SKILL_ACCUMULATE_TIME_TEXT,
    StrToWzID("TI35"),  //CHAR_SKILL_ACCUMULATE_POINT_TEXT,
    StrToWzID("BT01"),  //CHAR_SKILL_ACCUMULATEPOINT_REWARD_BUTTON,


    StrToWzID("P000"), //kControl_Picture_P000);
    StrToWzID("P010"), //kControl_Picture_P010);
    StrToWzID("P011"), //kControl_Picture_P011);
    StrToWzID("P012"), //kControl_Picture_P012);
    StrToWzID("P015"), //kControl_Picture_P015);
    StrToWzID("P016"), //kControl_Picture_P016);
    StrToWzID("P013"), //kControl_Picture_P013);
    StrToWzID("P014"), //kControl_Picture_P014);
    StrToWzID("P017"), //kControl_Picture_P017);

    StrToWzID("S006"), //kControl_Text_S006,
    StrToWzID("S007"), //kControl_Text_S007,
    StrToWzID("S008"), //kControl_Text_S008,
    StrToWzID("S009"), //kControl_Text_S009,
    StrToWzID("S010"), //kControl_Text_S010,
    StrToWzID("S011"), //kControl_Text_S011,
    StrToWzID("S012"), //kControl_Text_S012,
    StrToWzID("S013"), //kControl_Text_S013,

    StrToWzID("S001"), //kControl_Text_S001, // 스킬 발동 효과
    StrToWzID("S014"), //kControl_Text_S014,
    StrToWzID("S015"), //kControl_Text_S015,
    StrToWzID("S016"), //kControl_Text_S016,
    StrToWzID("S017"), //kControl_Text_S017,
    StrToWzID("S018"), //kControl_Text_S018,
    StrToWzID("S019"), //kControl_Text_S019,
    StrToWzID("S020"), //kControl_Text_S020,

    StrToWzID("S021"), //kControl_Text_S021,
    StrToWzID("S031"), //kControl_Text_S031,
    StrToWzID("S041"), //kControl_Text_S041,
    StrToWzID("S051"), //kControl_Text_S051,
    StrToWzID("S061"), //kControl_Text_S061,
    StrToWzID("S080"), //kControl_Text_S080,
    StrToWzID("S081"), //kControl_Text_S081,
    StrToWzID("S100"), //kControl_Text_S100,
    StrToWzID("S022"), //kControl_Text_S022,
    StrToWzID("S032"), //kControl_Text_S032,
    StrToWzID("S042"), //kControl_Text_S042,
    StrToWzID("S052"), //kControl_Text_S052,
    StrToWzID("S062"), //kControl_Text_S062,
    StrToWzID("S079"), //kControl_Text_S079,
    StrToWzID("S082"), //kControl_Text_S082,
    StrToWzID("S099"), //kControl_Text_S099,
    StrToWzID("S023"), //kControl_Text_S023,
    StrToWzID("S033"), //kControl_Text_S033,
    StrToWzID("S043"), //kControl_Text_S043,
    StrToWzID("S053"), //kControl_Text_S053,
    StrToWzID("S063"), //kControl_Text_S063,
    StrToWzID("S078"), //kControl_Text_S078,
    StrToWzID("S083"), //kControl_Text_S083,
    StrToWzID("S098"), //kControl_Text_S098,
    StrToWzID("S024"), //kControl_Text_S024,
    StrToWzID("S034"), //kControl_Text_S034,
    StrToWzID("S044"), //kControl_Text_S044,
    StrToWzID("S054"), //kControl_Text_S054,
    StrToWzID("S064"), //kControl_Text_S064,
    StrToWzID("S077"), //kControl_Text_S077,
    StrToWzID("S084"), //kControl_Text_S084,
    StrToWzID("S097"), //kControl_Text_S097,
    StrToWzID("S025"), //kControl_Text_S025,
    StrToWzID("S035"), //kControl_Text_S035,
    StrToWzID("S045"), //kControl_Text_S045,
    StrToWzID("S055"), //kControl_Text_S055,
    StrToWzID("S065"), //kControl_Text_S065,
    StrToWzID("S076"), //kControl_Text_S076,
    StrToWzID("S085"), //kControl_Text_S085,
    StrToWzID("S096"), //kControl_Text_S096,
    StrToWzID("S026"), //kControl_Text_S026,
    StrToWzID("S036"), //kControl_Text_S036,
    StrToWzID("S046"), //kControl_Text_S046,
    StrToWzID("S056"), //kControl_Text_S056,
    StrToWzID("S066"), //kControl_Text_S066,
    StrToWzID("S075"), //kControl_Text_S075,
    StrToWzID("S086"), //kControl_Text_S086,
    StrToWzID("S095"), //kControl_Text_S095,
    StrToWzID("S027"), //kControl_Text_S027,
    StrToWzID("S037"), //kControl_Text_S037,
    StrToWzID("S047"), //kControl_Text_S047,
    StrToWzID("S057"), //kControl_Text_S057,
    StrToWzID("S067"), //kControl_Text_S067,
    StrToWzID("S074"), //kControl_Text_S074,
    StrToWzID("S087"), //kControl_Text_S087,
    StrToWzID("S094"), //kControl_Text_S094,
    StrToWzID("S028"), //kControl_Text_S028,
    StrToWzID("S038"), //kControl_Text_S038,
    StrToWzID("S048"), //kControl_Text_S048,
    StrToWzID("S058"), //kControl_Text_S058,
    StrToWzID("S068"), //kControl_Text_S068,
    StrToWzID("S073"), //kControl_Text_S073,
    StrToWzID("S088"), //kControl_Text_S088,
    StrToWzID("S093"), //kControl_Text_S093,
    StrToWzID("S029"), //kControl_Text_S029,
    StrToWzID("S039"), //kControl_Text_S039,
    StrToWzID("S049"), //kControl_Text_S049,
    StrToWzID("S059"), //kControl_Text_S059,
    StrToWzID("S069"), //kControl_Text_S069,
    StrToWzID("S072"), //kControl_Text_S072,
    StrToWzID("S089"), //kControl_Text_S089,
    StrToWzID("S092"), //kControl_Text_S092,
    StrToWzID("S030"), //kControl_Text_S030,
    StrToWzID("S040"), //kControl_Text_S040,
    StrToWzID("S050"), //kControl_Text_S050,
    StrToWzID("S060"), //kControl_Text_S060,
    StrToWzID("S070"), //kControl_Text_S070,
    StrToWzID("S071"), //kControl_Text_S071,
    StrToWzID("S090"), //kControl_Text_S090,
    StrToWzID("S091"), //kControl_Text_S091,

    StrToWzID("V000"), //kControl_VScroll_V000,

    StrToWzID("B005"), //kControl_Button_B005,
    StrToWzID("B011"), //kControl_Button_B011,
    StrToWzID("B012"), //kControl_Button_B012,
    StrToWzID("B013"), //kControl_Button_B013,
    StrToWzID("B014"), //kControl_Button_B014,
    StrToWzID("B015"), //kControl_Button_B015,
    StrToWzID("P003"), //kControl_Picture_P003, //!< 깜빡이 버튼 이미지

};


enum PACKET_STATUS_CHAR_SKILL                       
{ 
	PACKET_STATUS_CHAR_SKILL_NONE			= 0, 
	PACKET_STATUS_CHAR_SKILL_STR			= 1,        
	PACKET_STATUS_CHAR_SKILL_DEX			= (2 << 1), 
	PACKET_STATUS_CHAR_SKILL_VIT			= (2 << 2), 
	PACKET_STATUS_CHAR_SKILL_INT			= (2 << 3), 
	PACKET_STATUS_CHAR_SKILL_SPR			= (2 << 4), 
	PACKET_STATUS_CHAR_SKILL_EXPERTY1		= (2 << 5), 
	PACKET_STATUS_CHAR_SKILL_EXPERTY2		= (2 << 6), 
};


//------------------------------------------------------------------------------
/**
*/
DWORD Char_SkillDialog::GetBit(eATTR_TYPE attrType)
{

	DWORD dwbit=0;

	switch(attrType)
	{
	case eATTR_STR:
		dwbit=PACKET_STATUS_CHAR_SKILL_STR;
		break;

	case eATTR_DEX:
		dwbit=PACKET_STATUS_CHAR_SKILL_DEX;
		break;

	case eATTR_VIT:
		dwbit=PACKET_STATUS_CHAR_SKILL_VIT;
		break;

	case eATTR_INT:
		dwbit=PACKET_STATUS_CHAR_SKILL_INT;
		break;

	case eATTR_SPR:
		dwbit=PACKET_STATUS_CHAR_SKILL_SPR;
		break;

	case eATTR_EXPERTY1:
		dwbit=PACKET_STATUS_CHAR_SKILL_EXPERTY1;
		break;

	case eATTR_EXPERTY2:
		dwbit=PACKET_STATUS_CHAR_SKILL_EXPERTY2;
		break;

	default:
		assert("정의되지 않은 속성");
		break;
	}

	return dwbit;
}


//------------------------------------------------------------------------------
/**
*/
Char_SkillDialog::Char_SkillDialog(InterfaceManager* pUIMan):uiBase(pUIMan)
{
    pressed_button_index_ = CHAR_SKIL_MAX;
    pressed_button_time_ = clock_function::GetTickCount();
    simulation_player_attributes().Clear();
    //simulation_scplayer_;
    reset_simulation_stat_count();
}

//------------------------------------------------------------------------------
/**
*/
Char_SkillDialog::~Char_SkillDialog(void)
{
    simulation_player_attributes().Clear();
}

//------------------------------------------------------------------------------
/**
*/
VOID Char_SkillDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	//MoveWindowWZ(0,110);
	m_dwPacketStatus = 0;
	m_pPictureGuildMark = getControl(CHAR_SKILL_MARK_P);
	assert(m_pPictureGuildMark);

    accumulate_time_text_ = static_cast<CCtrlImageStaticWZ*>(getControl(CHAR_SKILL_ACCUMULATE_TIME_TEXT));
    SUN_ASSERT(accumulate_time_text_);
    accumulate_point_text_ = static_cast<CCtrlImageStaticWZ*>(getControl(CHAR_SKILL_ACCUMULATE_POINT_TEXT));
    SUN_ASSERT(accumulate_point_text_);
    accumulate_reward_button_ = static_cast<CCtrlButtonWZ*>(getControl(CHAR_SKILL_ACCUMULATEPOINT_REWARD_BUTTON));
    SUN_ASSERT(accumulate_reward_button_);
    accumulate_timer_.Clear();

    #ifdef _RU_110209_ACCUMULATE_POINT_BLOCK
    if (accumulate_time_text_ != NULL)
    {
        accumulate_time_text_->ShowWindowWZ(WZ_HIDE);
    }
    if (accumulate_point_text_ != NULL)
    {
        accumulate_point_text_->ShowWindowWZ(WZ_HIDE);
    }
    if (accumulate_reward_button_ != NULL)
    {
        accumulate_reward_button_->ShowWindowWZ(WZ_HIDE);
    }

    char* hide_control_id[3] = {"ST01", "ST02", "PI01"};
    for (int index = 0; index < 3; ++index)
    {
        CControlWZ* control_ptr = GetControlWZ(StrToWzID(hide_control_id[index]));
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }    
    #endif //_RU_110209_ACCUMULATE_POINT_BLOCK


    // 클리어
    for (int i = kControl_Text_S006; i <= kControl_Text_S091; ++i)
    {
        getControl(i)->SetTextWZ("");
    }
    socketoption_vector().clear();
    socket_option_show_begin_index_ = 0;

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(getControl(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        scroll_control->SetScrollRangeWZ(0, 0);
        scroll_control->SetScrollPosWZ(0);
    }

#if (WZENGINEVER >= 320)
    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);
#endif //#if WZENGINEVER >= 320
}

//------------------------------------------------------------------------------
/**
*/
VOID Char_SkillDialog::Release()
{

}

//------------------------------------------------------------------------------
/**
    왼쪽 마우스 버튼 업시 해당 컨트롤을 알아냄 
*/
VOID Char_SkillDialog::MessageProc( SI_MESSAGE * pMessage )
{
	POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

	CControlWZ * pControl = getControl(pos);
	if (!pControl)
	{
		return;
	}

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch(pos)
            {
            case CHAR_SKILL_CLOSE:
                {		
					ShowDialog(FALSE);
                }
                break;

            case kControl_Button_B015:
                {
                    uiSystemMan* system_main = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                    if (system_main != NULL)
                    {
                        TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
                        if ((g_pHero != NULL) && (g_pHero->IsTransForm() == TRUE))
                        {
                            // 5139	변신 상태에서는 이용이 가능하지 않습니다.
                            g_InterfaceManager.GetInterfaceString(5139, message ,INTERFACE_STRING_LENGTH);
                            system_main->Sys_VerifyLock(message);
                        }
                        else
                        {
                            // 2800 변경한 스탯 포인트를 적용하시겠습니까?
                            g_InterfaceManager.GetInterfaceString(2800, message, INTERFACE_STRING_LENGTH);
                            system_main->Sys_ConfirmLock(message, Char_SkillDialog::CallbackSendPacketStatusStatApply);
                        }
                    }
                }
                break;

			case CHAR_SKILL_STATE_INIT_BTN:
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
					    uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					    pSystemMan->Sys_Confirm( g_InterfaceManager.GetInterfaceString( 6291 ), GlobalFunc::SendStateInitSyn);
                    }
				}
				break;
            case CHAR_SKILL_ACCUMULATEPOINT_REWARD_BUTTON:
                {
                    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
                    if (ui_manager)
                    {
                        ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(true);
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_SB_LINETOP:
        OnVScrollUp(pMessage);
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(pMessage);
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(pMessage);
        break;

    case RT_MSG_LBUTTONDOWN:
        {
            switch(pos)
            {
            case CHAR_SKILL_STR_B:
            case CHAR_SKILL_DEX_B:
            case CHAR_SKILL_VIT_B:
            case CHAR_SKILL_INT_B:
            case CHAR_SKILL_SPR_B:
            case kControl_Button_B005:
            case kControl_Button_B011:
            case kControl_Button_B012:
            case kControl_Button_B013:
            case kControl_Button_B014:
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
                    set_pressed_button_time(clock_function::GetTickCount());
                    set_pressed_button_index(static_cast<Char_Skill_Pos>(pos));
                    UpdateSimulationPlayerAttributes(static_cast<Char_Skill_Pos>(pos), 1);
                }
                break;
            }
        }
        break;
    case RT_MSG_LBUTTONUP:
        {
            set_pressed_button_time(clock_function::GetTickCount());
            set_pressed_button_index(CHAR_SKIL_MAX);
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID Char_SkillDialog::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------ 
void Char_SkillDialog::UpdateMouseWheel()
{
    //! 소켓 옵션 리스트들 스크롤 체크
    CControlWZ* control_lt = getControl(kControl_Text_S006);
    CControlWZ* control_rb = getControl(kControl_Text_S020);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(getControl(kControl_VScroll_V000));
    if ((control_lt != NULL) && (control_rb != NULL) && (scroll_control != NULL))
    {
        RECT area;
        area.left = control_lt->GetSizeRect().left;
        area.top = control_lt->GetSizeRect().top;
        area.right = control_rb->GetSizeRect().right;
        area.bottom = control_rb->GetSizeRect().bottom;

        switch (GetMouseWheelStatus(&area))
        {
        case MOUSE_WHEEL_UP:
            {
                OnVScrollUp(scroll_control);
            }
            break;

        case MOUSE_WHEEL_DOWN:
            {
                OnVScrollDown(scroll_control);
            }
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
    캐릭터 정보 갱신 
*/
VOID Char_SkillDialog::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    if (IsOverControl(pressed_button_index()) && Mouse::Instance()->LButtonPressed() == TRUE)
    {
        DWORD cur_tick = clock_function::GetTickCount();
        DWORD gab_tick = cur_tick - pressed_button_time();

        int add_count = ((int)gab_tick - 0) / 1000;
        if (add_count > 0)
        {
            UpdateSimulationPlayerAttributes(pressed_button_index(), add_count);
            //set_pressed_button_time(cur_tick);
        }
    }
    else
    {
        set_pressed_button_index(CHAR_SKIL_MAX);
        set_pressed_button_time(clock_function::GetTickCount());
    }

    #if (WZENGINEVER >= 320)
    CControlWZ* picture_control = getControl(kControl_Picture_P003);
    if (picture_control != NULL)
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

            picture_control->SetTransprency(alpha);
            picture_control->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            alpha_value = 0;
            picture_control->ShowWindowWZ(WZ_HIDE);
        }
        prev_tick = cur_tick;
    }
    #endif //#if (WZENGINEVER >= 320)


	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if(g_pHero)//캐릭터가 생성 되었고 
	{
		PlayerAttributes *pCharInfo=g_pHero->GetPlayerAttribute();			
		BASE_PLAYERINFO*  pPlayInfo=g_pHero->GetCharInfo();

		if(pCharInfo&&pPlayInfo)//서버로 부터 캐릭터 정보를 받았을때 정보를 갱신 한다.
		{
			POSTYPE pos = 0;
			CCtrlImageStaticWZ * pControl = NULL;

			// 기본 스테이터스 
			// name 
			pControl = (CCtrlImageStaticWZ *)getControl(CHAR_SKILL_VALUE_NAME);	
			GlobalFunc::SetCaption( pControl, g_pHero->GetName() );
			
			pControl = (CCtrlImageStaticWZ *)getControl(CHAR_SKILL_VALUE_LEVEL);	
			Sprintf(szMessage, _T("%d"), g_pHero->GetLevel());
			GlobalFunc::SetCaption( pControl, szMessage );

			pControl = (CCtrlImageStaticWZ *)getControl(CHAR_SKILL_VALUE_CLASS);	
			
			g_pHero->GetClassExName(szMessage);

			GlobalFunc::SetCaption( pControl, szMessage );

			pControl = (CCtrlImageStaticWZ *)getControl(CHAR_SKILL_VALUE_GUILD_NAME);	
			szMessage[0] = NULL;

            uiGuildMan* guildMan =
                static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
            assert (guildMan);

            if (guildMan->IsGuildInfo())
            {
				Sprintf (szMessage, _T("%s"), g_pHero->GetGuildName());
            }
            else
            {
				//	없음
				g_InterfaceManager.GetInterfaceString( eST_NONE_TEXT, szMessage, INTERFACE_STRING_LENGTH );
            }
			GlobalFunc::SetCaption( pControl, szMessage );

			CCtrlButtonWZ* pUpButton = NULL;
			for (int nPos = UP_BUTTON_BEGIN; nPos < UP_BUTTON_END+1 ; ++nPos)
			{
				pos = getCtrlIDToPos( m_wzId[nPos] );
				pUpButton = (CCtrlButtonWZ *)getControl(pos);
				if (pUpButton)
				{
					if ( pPlayInfo->m_iRemainStat > 0)
					{
						pUpButton->ShowWindowWZ( WZ_SHOW );
					}
					else
					{
						pUpButton->ShowWindowWZ( WZ_HIDE );
					}					
				}
			}

            UpdateParameter();

            if (g_pHero->GetLevel() <= GameOptionParser::Instance()->GetFreeStatInitLevel())
			{
				FreeStateControlShowWindow( WZ_SHOW );

				pos = getCtrlIDToPos( m_wzId[CHAR_SKILL_STATE_INIT_COUNT_EDIT] ); //초기화 횟수
				pControl = (CCtrlImageStaticWZ *)getControl(pos);	

				if( pControl )
				{
					TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
					Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), pPlayInfo->m_byFreeInitCount );

					pControl->SetTextColor(WzColor_RGBA(255,255,255,255));
					pControl->SetTextMouseOnColor(WzColor_RGBA(255,255,255,255));
					pControl->SetTextMouseDownColor(WzColor_RGBA(255,255,255,255));
			
					pControl->SetTextWZ(szMessage);
				}
			}
			else
			{
				FreeStateControlShowWindow( WZ_HIDE );
			}
            // 접속포인트 - 플레이시간 및 코인 출력 hour, minute;
            uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
            if (ui_manager)
            {                
                // 누적된 플레이타임(초)
                DWORD accumulate_time = ui_manager->accumulate_time() + (accumulate_timer_.GetProcessTime() / 1000);
                DWORD accumulate_time_second = accumulate_time % 60;
                // 누적된 플레이타임(분)
                accumulate_time /= 60;
                Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%02d:%02d:%02d(%dDay)"), 
                         accumulate_time/60, accumulate_time%60, accumulate_time_second, accumulate_time/(60*24));
                accumulate_time_text_->SetTextWZ(szMessage);
                // 누적된 포인트
                Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%d"), ui_manager->accumulate_point());
                accumulate_point_text_->SetTextWZ(szMessage);
                ENUM_DRAWSTATE draw_state = accumulate_reward_button_->GetDrawState();

                if (ui_manager->accumulate_point() >= ui_manager->require_point())
                {
                    if (draw_state == E_DISABLE)
                    {
                        accumulate_reward_button_->EnableWindowWZ(true);
                    }
                }
                else
                {
                    if (draw_state != E_DISABLE)
                    {
                        accumulate_reward_button_->EnableWindowWZ(false);
                    }
                }
            }

        }
	}
}


void	Char_SkillDialog::FreeStateControlShowWindow( ENUM_STATEWZ eState )
{	
	for( int i = CHAR_SKILL_STATE_INIT_BTN; i < CHAR_SKILL_STATE_INIT_TEXT+1 ; ++i )
	{
		POSTYPE pos = getCtrlIDToPos( m_wzId[i] );
		CControlWZ  * pCtrl = getControl(pos);
		if( pCtrl )
			pCtrl->ShowWindowWZ( eState );
	}
}


//------------------------------------------------------------------------------
/**
*/
CControlWZ * Char_SkillDialog::getControl( POSTYPE AtPos )
{
    if (AtPos >= CHAR_SKIL_MAX)
    {
        return NULL;
    }
	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE	Char_SkillDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CHAR_SKIL_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return CHAR_SKIL_MAX;
}

//------------------------------------------------------------------------------
/**
*/
VOID Char_SkillDialog::OnShowWindow(BOOL val)
{
	SolarDialog::OnShowWindow(val);

    if (val)
    {
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_charSkill;
		msg.wParam=InterfaceManager::DIALOG_CHAR_STATUS;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

        //------------------------------------------------------------------------------ 
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(getControl(kControl_VScroll_V000));
        if (scroll_control != NULL)
        {
            scroll_control->SetScrollRangeWZ(0, 0);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->SetChangeSizeThumb(100);
            socketoption_vector().clear();
        }
        //------------------------------------------------------------------------------ 

        reset_simulation_stat_count();
        RefreshSimulationPlayerAttributes();
        CControlWZ* picture_control = getControl(kControl_Picture_P003);
        if (picture_control != NULL)
        {
            picture_control->SetDiscardMessage(true);
            picture_control->ShowWindowWZ(WZ_HIDE);
        }
    }
	else
	{
		//GS_BUG_FIX:스텟올리는중 던전등의 호출로 로딩을 타면  ACK패킷을 받지못할때가있다.
		//			 그러한경우 올리고있던 스텟은 데드락상태과같은 상태가 발생
		m_dwPacketStatus = 0;
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_charSkill);

        reset_simulation_stat_count();
        simulation_player_attributes().Clear();
	}
}


//------------------------------------------------------------------------------
/**
    남아있는 스탯,패킷 중복 날리기 검사 
*/
BOOL Char_SkillDialog::CheckStat(eATTR_TYPE attrType)
{
	if(!g_pHero)
		return FALSE;

	//캐릭터 항상 있나?
	BASE_PLAYERINFO*  pPlayInfo = g_pHero->GetCharInfo();
	//스탯이 없거나 해당 버튼을 눌러 이미 패킷을 보냈을 경우 
	if(pPlayInfo && (pPlayInfo->m_iRemainStat <= 0))
	{
		// 스탯이 부족할때는 한마디 해준다.
		g_HeroInput.PlayNotEnoughPointSound();
		return FALSE;
	}

	DWORD dwbit = GetBit(attrType);
	if( BIT_CHECK(m_dwPacketStatus,dwbit) )
	{
		return FALSE;
	}
	
	return TRUE;
}

//------------------------------------------------------------------------------
/**
    스탯 변화 패킷을 날림
*/
BOOL Char_SkillDialog::DoStat(eATTR_TYPE attrType)
{
	//네트웍 모드 일때만 동작
	if( GENERALPARAM->IsNet() && 
        CheckStat(attrType)) //현재 스탯이 남아 있나,패킷을 보내고 응답을 받았나 검사
	{
		MSG_CG_STATUS_STAT_SELECT_SYN	SendPacket;
		SendPacket.m_byCategory = CG_STATUS;
		SendPacket.m_byProtocol = CG_STATUS_STAT_SELECT_SYN;
		SendPacket.m_AttrType	= attrType;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
	
		DWORD dwbit=GetBit(attrType);
		BIT_ADD(m_dwPacketStatus,dwbit);
		
		return TRUE;
	}

	return FALSE;
}

VOID Char_SkillDialog::RemoveBit(eATTR_TYPE attrType)
{
	DWORD dwbit=GetBit(attrType);

	BIT_REMOVE(m_dwPacketStatus,dwbit);
}

//------------------------------------------------------------------------------
/**
	GS_BUG_FIX:스피드(이속,공속) 추가수치값을 (아이템+스킬)모두 적용시킴
*/
int	Char_SkillDialog::AddValueForSpeed(PlayerAttributes *pCharInfo,eATTR_TYPE attrType)
{
	int ResultValue  = 0;
		
	for(int i=0;i<2;++i)
	{	
		int iCalc = (*pCharInfo)[attrType][eATTR_KIND_CALC];
		
		float fTotRatio = (*pCharInfo)[attrType][eATTR_KIND_RATIO];	fTotRatio /= 100.f;		 
		
		eATTR_KIND AttrRatioIndex  = (i==0 ? (eATTR_KIND_ITEM_RATIO):(eATTR_KIND_SKILL_RATIO));
		eATTR_KIND AttrIndex	   = (i==0 ? (eATTR_KIND_ITEM):(eATTR_KIND_SKILL));	 

		float fRatio = (*pCharInfo)[attrType][AttrRatioIndex];	
		fRatio /= 100.f;
		int   iValue = (*pCharInfo)[attrType][AttrIndex];
		
		ResultValue+=(iValue+ (int)( ( iCalc / (fTotRatio+1.f) - iValue ) * fRatio ));
	}
	
	return ResultValue;

}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetControlTextColor(CCtrlImageStaticWZ* control, int val)
{
    if (control->IsKind(CTRL_KIND_TEXTIMAGE) == false)
    {
        return;
    }
 
    if (val > 0)
    {
        control->SetTextColor(WzColor_RGBA(20,255,20,255));
        control->SetTextMouseOnColor(WzColor_RGBA(20,255,20,255));
        control->SetTextMouseDownColor(WzColor_RGBA(20,255,20,255));
 
    }
    else if (val < 0)
    {
        control->SetTextColor(WzColor_RGBA(20,20,255,255));
        control->SetTextMouseOnColor(WzColor_RGBA(20,20,255,255));
        control->SetTextMouseDownColor(WzColor_RGBA(20,20,255,255));
    }
    else
    {
        control->SetTextColor(WzColor_RGBA(255,255,255,255));
        control->SetTextMouseOnColor(WzColor_RGBA(255,255,255,255));
        control->SetTextMouseDownColor(WzColor_RGBA(255,255,255,255));
    }
} 
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaption(Char_Skill_Pos pos, int val)
{
    //CControlWZ* control = getControl(getCtrlIDToPos(m_wzId[pos]));
    CControlWZ* control = getControl(pos);

    if (control->IsKind(CTRL_KIND_TEXTIMAGE))
    {
        SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control), val);
 
        TCHAR text_string[INTERFACE_STRING_LENGTH] = {0,};
        Snprintf(text_string, INTERFACE_STRING_LENGTH-1, _T("%d"), val);
        static_cast<CCtrlImageStaticWZ*>(control)->SetTextWZ(text_string);
    }
} 
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaptionWithPercent(Char_Skill_Pos pos, int val)
{
    //CControlWZ* control = getControl(getCtrlIDToPos(m_wzId[pos]));
    CControlWZ* control = getControl(pos);
 
    if (control->IsKind(CTRL_KIND_TEXTIMAGE))
    {
        SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control), val);
 
        TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
        TCHAR string_messgae[INTERFACE_STRING_LENGTH] = {NULL, };

        //! 158573	%d%%
        g_InterfaceManager.GetInterfaceString(158573, string_format, INTERFACE_STRING_LENGTH);
        Snprintf(string_messgae, INTERFACE_STRING_LENGTH-1, string_format, val);
 
        control->SetTextWZ(string_messgae);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaption(Char_Skill_Pos pos, int iCalcValue, int iBaseValue)
{
    //CControlWZ* control = getControl(getCtrlIDToPos(m_wzId[pos]));
    CControlWZ* control = getControl(pos);

    if (control->IsKind(CTRL_KIND_TEXTIMAGE))
    {
        SetParameterCaption(static_cast<CCtrlImageStaticWZ*>(control), iCalcValue, iBaseValue);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaption(CCtrlImageStaticWZ *pControl,int iCalcValue,int iBaseValue)
{
	if (pControl->IsKind(CTRL_KIND_TEXTIMAGE) == false)
	{
		return;
	}

    SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(pControl), (iCalcValue - iBaseValue));

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), iCalcValue);

    pControl->SetTextWZ(szMessage);
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaptionWithPercent(Char_Skill_Pos pos, int calc_value, int base_value)
{
    CControlWZ* control = getControl(getCtrlIDToPos(m_wzId[pos]));

    if (control->IsKind(CTRL_KIND_TEXTIMAGE))
    {
        SetParameterCaptionWithPercent(static_cast<CCtrlImageStaticWZ*>(control), calc_value, base_value);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaptionWithPercent(CCtrlImageStaticWZ* control_ptr, int calc_value, int base_value)
{
    if (control_ptr == NULL)
    {
        return;
    }

    SetControlTextColor(static_cast<CCtrlImageStaticWZ*>(control_ptr), (calc_value - base_value));

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    TCHAR string_messgae[INTERFACE_STRING_LENGTH] = {NULL, };

    //! 158573	%d%%
    g_InterfaceManager.GetInterfaceString(158573, string_format, INTERFACE_STRING_LENGTH);
    Snprintf(string_messgae, INTERFACE_STRING_LENGTH-1, string_format, calc_value);

    control_ptr->SetTextWZ(string_messgae);
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetParameterCaption2(CCtrlImageStaticWZ *pControl,int iCalcValue,int iAdditionalValue)
{
	if (!pControl)
	{
		return;
	}

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), iCalcValue);
 	if (iAdditionalValue > 0)
	{
		pControl->SetTextColor(WzColor_RGBA(20,255,20,255));
		pControl->SetTextMouseOnColor(WzColor_RGBA(20,255,20,255));
		pControl->SetTextMouseDownColor(WzColor_RGBA(20,255,20,255));		
	}
	else if (iAdditionalValue < 0 )
	{
		pControl->SetTextColor(WzColor_RGBA(20,20,255,255));
		pControl->SetTextMouseOnColor(WzColor_RGBA(20,20,255,255));
		pControl->SetTextMouseDownColor(WzColor_RGBA(20,20,255,255));
	}
	else
	{
		pControl->SetTextColor(WzColor_RGBA(255,255,255,255));
		pControl->SetTextMouseOnColor(WzColor_RGBA(255,255,255,255));
		pControl->SetTextMouseDownColor(WzColor_RGBA(255,255,255,255));
	}

	pControl->SetTextWZ(szMessage);
}

void Char_SkillDialog::SetParameterDamage(CCtrlImageStaticWZ *pControl,int iMinValue,int iMaxValue)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

#ifdef _KIKI_TEST_CODE
	if(g_pHero)
	{
		PlayerAttributes *pCharInfo=g_pHero->GetPlayerAttribute();	
		int iValue1 = 0;
		int iValue2 = 0;
		if(pCharInfo)
		{
			if(pCharInfo->GetExperty1())
			{
				iValue1 = pCharInfo->GetExperty1() / 3;
			}

			if(pCharInfo->GetExperty2())
			{
				iValue2 = pCharInfo->GetExperty2() / 3;
			}

			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d~%d (%d) (%d)"), iMinValue, iMaxValue, iValue1, iValue2);
		}
	}
#else

#ifdef _JAPAN
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d-%d"), iMinValue, iMaxValue);	
#else
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d~%d"), iMinValue, iMaxValue);
#endif	
	
	
#endif
	
	pControl->SetTextColor(WzColor_RGBA(255,255,255,255));
	pControl->SetTextWZ(szMessage);
}

//------------------------------------------------------------------------------ 
bool SortSocketOptionByOptionPoint(const ItemAttrCalculator::SocketOptionCalculator::CalculationResult& lhs, 
                                   const ItemAttrCalculator::SocketOptionCalculator::CalculationResult& lhr)
{
    return (lhs.option_point > lhr.option_point);
}
//------------------------------------------------------------------------------ 
int GetRowSocketOptionByOptionPoint(const eATTR_TYPE attr_type, 
                                       Char_SkillDialog::SocketOptionVector& socketcount_vector)
{
    Char_SkillDialog::SocketOptionVector::iterator begin = socketcount_vector.begin();
    Char_SkillDialog::SocketOptionVector::iterator end = socketcount_vector.end();

    for (int i = 0; begin != end; ++begin, ++i)
    {
        if (begin->attr_option_code == attr_type)
        {
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------ 
int GetColSocketOptionByAttribite(eEQUIPCONTAINER_POS pos)
{
    switch (pos)
    {
        case eEQUIPCONTAINER_WEAPON:    return 0; 
        case eEQUIPCONTAINER_ARMOR:     return 1; 
        case eEQUIPCONTAINER_PROTECTOR: return 2; 
        case eEQUIPCONTAINER_HELMET:    return 3; 
        case eEQUIPCONTAINER_PANTS:     return 4; 
        case eEQUIPCONTAINER_BOOTS:     return 5; 
        case eEQUIPCONTAINER_GLOVE:     return 6; 
        case eEQUIPCONTAINER_BELT:      return 7; 
        case eEQUIPCONTAINER_SHIRTS:    return 8; 
    }

    return -1;
}
//------------------------------------------------------------------------------ 
WzColor GetSocketOptionTextColor(int val)
{
    WzColor text_color = WzColor_RGBA(255,255,255,255);

    if (val > 0)
    {
        text_color = WzColor_RGBA(0,255,0,255);
    }
    else if (val < 0)
    {
        text_color = WzColor_RGBA(0,0,255,255);
    }

    return text_color;
}

//------------------------------------------------------------------------------ 
void Char_SkillDialog::OnRenderSolarDialog()
{
	_renderGuildMark();

    // 클리어
    for (int i = kControl_Text_S006; i <= kControl_Text_S091; ++i)
    {
        getControl(i)->SetTextWZ("");
    }

    int backup_cur_apply_socket_count = socketoption_vector().size();
    socketoption_vector().clear();
    CalcTotalSocketOption(socketoption_vector());

    //------------------------------------------------------------------------------ 
    //! 소켓 옵션이 바뀌었으면 스크롤바 수정.
    if (backup_cur_apply_socket_count != socketoption_vector().size())
    {
        set_socket_option_show_begin_index(0);

        backup_cur_apply_socket_count = socketoption_vector().size();

        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(getControl(kControl_VScroll_V000));
        if (scroll_control != NULL)
        {
            // 스크롤 바 업데이트
            int max_num = max(0, backup_cur_apply_socket_count - kSocketOptionShowRowCount);

            scroll_control->SetScrollRangeWZ(0, max_num);
            scroll_control->SetScrollPosWZ(socket_option_show_begin_index());

            int thumb_size = 100 / (max_num + 1);
            scroll_control->SetChangeSizeThumb(thumb_size);
        }
    }
    //------------------------------------------------------------------------------ 

    //! 내림차순으로 정렬
    std::sort(socketoption_vector().begin(), socketoption_vector().end(), SortSocketOptionByOptionPoint);

    //! 소캣 기능 출력
    UpdateSocketOptionName(socketoption_vector());

    //------------------------------------------------------------------------------ 
    HeroEquipmentContainer* equip_container = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());
    ItemUnitRenderer::RenderUnit * render_unit = NULL;
    ItemUnitRenderer* item_renderer = equip_container->GetItemUnitRender();
    uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    SCItemSlot* item_slot_back_up = NULL;

    item_renderer->GetItemUnitHashTable()->SetFirst();
    while (render_unit = item_renderer->GetItemUnitHashTable()->GetNext())
    {		
        if ((render_unit->m_pIconImage == NULL) || 
            (render_unit->m_pRefSlot == NULL) || 
            (render_unit->m_pRefSlot->GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot* item_slot = (SCItemSlot*)(render_unit->m_pRefSlot);

        int col_index = GetColSocketOptionByAttribite(static_cast<eEQUIPCONTAINER_POS>(item_slot->GetItemInfo()->m_wEqPos));

        if ((col_index >= 0) && (col_index < kSocketOptionShowColCount))
        {
            UpdateSocketOptionValue(item_slot, col_index, socketoption_vector());

            RECT area_rect = getControl(kControl_Picture_P000 + col_index)->GetSizeRect();
            render_unit->m_pIconImage->Render(&area_rect);

            //소켓 툴팁
            if (IsOverControl(static_cast<Char_Skill_Pos>(kControl_Picture_P000 + col_index)))
            {
                item_slot_back_up = item_slot;
            }
        }
   
    }

    // 툴팁 출력
    if (this->IsLocked() == false)
    {
        RenderTooltip_SocketOption(socketoption_vector());

        RenderTooltip();

        if (item_slot_back_up != NULL)
        {
            tooltip_manager->RegisterSlotInfoTooltip(item_slot_back_up);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }


}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::_renderGuildMark()
{
	if( !g_pHero)
		return;

	if( !g_pHero->IsExistGuildMark())
		return;


	uiGuildMan* pGuildMan =
		static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

	HANDLE hGuildMarkTexture = pGuildMan->GetGuildMarkTexture();
	if( hGuildMarkTexture == INVALID_HANDLE_VALUE )
		return;

	BASE_GuildMarkInfo* pMarkInfo = NULL;
	ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();

	RECT rtSize = m_pPictureGuildMark->GetSizeRect();

	rtSize.left += 15;
	rtSize.top += 15;

	float fWidth = (float) rtSize.right - rtSize.left - 15;
	float fHeight = (float) rtSize.bottom - rtSize.top - 15;
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
	for(int i =0; i < 2; ++i)
	{
		if( i == 0)
			pMarkInfo =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_BackGroundIndex);
		else
			pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_PatternIndex);

		if( pMarkInfo )
		{
			g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
				(float)pMarkInfo->iRow * 32.0f , (float) pMarkInfo->iColumn * 32.0f, 32.0f, 32.0f);
		}
	}
	g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);

}
//------------------------------------------------------------------------------
bool Char_SkillDialog::IsOverControl(Char_Skill_Pos pos)
{
    if (pos >= CHAR_SKIL_MAX)
    {
        return false;
    }

    CControlWZ* control_ptr = getControl(pos);
    if (control_ptr == NULL)
    {
        return false;
    }

    RECT rc = control_ptr->GetSizeRect();

    if (rc.left <= Mouse::Instance()->GetMouseX() && rc.right >= Mouse::Instance()->GetMouseX() && 
        rc.top <= Mouse::Instance()->GetMouseY()  && rc.bottom >= Mouse::Instance()->GetMouseY())
    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
void Char_SkillDialog::RenderTooltip_MagicalAttackPower(uiToolTipMan* tooltip_manager, 
                                                        DWORD string_code,
                                                        PlayerAttributes* player_attributes, 
                                                        eATTACK_TYPE attack_type)
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(string_code, msg, INTERFACE_STRING_LENGTH);

    int base_value = player_attributes->GetMagicalAttackPower(attack_type, eATTR_KIND_BASE);
    int calc_value = player_attributes->GetMagicalAttackPower(attack_type, eATTR_KIND_CALC);
    tooltip_manager->RegisterTooltipFormat(msg, base_value, calc_value - base_value);
	tooltip_manager->SetTooltipDialogID(GetDialogKey());
}
//------------------------------------------------------------------------------
void Char_SkillDialog::RenderTooltip_MagicalDefense(uiToolTipMan* tooltip_manager, 
                                                    DWORD string_code,
                                                    PlayerAttributes* player_attributes, 
                                                    eATTACK_TYPE attack_type)
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(string_code, msg, INTERFACE_STRING_LENGTH);

    int base_value = player_attributes->GetReduceElementDamageRatio(attack_type, eATTR_KIND_BASE);
    int calc_value = player_attributes->GetReduceElementDamageRatio(attack_type, eATTR_KIND_CALC);
    tooltip_manager->RegisterTooltipFormat(msg, base_value, calc_value - base_value);
	tooltip_manager->SetTooltipDialogID(GetDialogKey());
}

//------------------------------------------------------------------------------
void Char_SkillDialog::RenderTooltip_StringCode(uiToolTipMan* tooltip_manager, DWORD string_code )
{
	tooltip_manager->RegisterTooltipWithStringCode( string_code );
	tooltip_manager->SetTooltipDialogID(GetDialogKey());
}

//------------------------------------------------------------------------------
void Char_SkillDialog::RenderTooltip_BaseAndCalc(uiToolTipMan* tooltip_manager, 
                                                 int base_value, 
                                                 int calc_value, 
                                                 bool is_fource,
                                                 DWORD string_code)
{
    if ((base_value != calc_value) || (is_fource == true))
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        if (string_code > 0)
        {
            g_InterfaceManager.GetInterfaceString(string_code, msg, INTERFACE_STRING_LENGTH);
        }

        if (calc_value >= base_value)
        {
            _tcscat(msg, _T("%d + %d"));
        }
        else
        {
            _tcscat(msg, _T("%d - %d"));
        }
        tooltip_manager->RegisterTooltipFormat(msg, base_value, abs(calc_value - base_value));
		tooltip_manager->SetTooltipDialogID(GetDialogKey());
    }
}
//------------------------------------------------------------------------------
void Char_SkillDialog::RenderTooltip()
{
    PlayerAttributes* player_attributes = &(simulation_player_attributes());



    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if ((tooltip_manager == NULL) || (player_attributes == NULL))
    {
        return;
    }

    if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WATER))         RenderTooltip_StringCode(11487);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WIND))     RenderTooltip_StringCode(11488);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_GROUND))   RenderTooltip_StringCode(11489);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_FIRE))     RenderTooltip_StringCode(11490);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_DARK))     RenderTooltip_StringCode(11491);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WATER2))   RenderTooltip_StringCode(11481);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WIND2))    RenderTooltip_StringCode(11482);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_GROUND2))  RenderTooltip_StringCode(11483);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_FIRE2))    RenderTooltip_StringCode(11484);
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_DARK2))    RenderTooltip_StringCode(11485);
    //if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WATER))         RenderTooltip_MagicalAttackPower(tooltip_manager, 11487, player_attributes, eATTACK_TYPE_WATER);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WIND))     RenderTooltip_MagicalAttackPower(tooltip_manager, 11488 , player_attributes, eATTACK_TYPE_WIND);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_GROUND))   RenderTooltip_MagicalAttackPower(tooltip_manager, 11489, player_attributes, eATTACK_TYPE_EARTH);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_FIRE))     RenderTooltip_MagicalAttackPower(tooltip_manager, 11490 , player_attributes, eATTACK_TYPE_FIRE);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_DARK))     RenderTooltip_MagicalAttackPower(tooltip_manager, 11491 , player_attributes, eATTACK_TYPE_DARKNESS);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WATER2))   RenderTooltip_MagicalDefense(tooltip_manager, 11481, player_attributes, eATTACK_TYPE_WATER);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_WIND2))    RenderTooltip_MagicalDefense(tooltip_manager, 11482 , player_attributes, eATTACK_TYPE_WIND);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_GROUND2))  RenderTooltip_MagicalDefense(tooltip_manager, 11483, player_attributes, eATTACK_TYPE_EARTH);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_FIRE2))    RenderTooltip_MagicalDefense(tooltip_manager, 11484 , player_attributes, eATTACK_TYPE_FIRE);
    //else if (IsOverControl(CHAR_SKILL_VALUE_ATTR_DARK2))    RenderTooltip_MagicalDefense(tooltip_manager, 11485 , player_attributes, eATTACK_TYPE_DARKNESS);
    else if (IsOverControl(CHAR_SKILL_VALUE_STR))           RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseSTR(), player_attributes->GetSTR(), true, 11560);
    else if (IsOverControl(CHAR_SKILL_VALUE_DEX))           RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseDEX(), player_attributes->GetDEX(), true, 11561);
    else if (IsOverControl(CHAR_SKILL_VALUE_STAMINA))       RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseVIT(), player_attributes->GetVIT(), true, 11562);
    else if (IsOverControl(CHAR_SKILL_VALUE_WISDOM))        RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseINT(), player_attributes->GetINT(), true, 11563);
    else if (IsOverControl(CHAR_SKILL_VALUE_SPIRIT))        RenderTooltip_BaseAndCalc(tooltip_manager, player_attributes->GetBaseSPR(), player_attributes->GetSPR(), true, 11564);
    //else if (IsOverControl(CHAR_SKILL_VALUE_PHYSIC_ATTACK_MAX))
    //{
    //    //! 추가 공격력
    //    RenderTooltip_BaseAndCalc(tooltip_manager, 
    //                            player_attributes->GetBaseOptionPhysicalAttPower(), 
    //                            player_attributes->GetOptionPhysicalAttPower());
    //}
    //else if (IsOverControl(CHAR_SKILL_VALUE_PHYSIC_DEFENSE_RATE))
    //{
    //    int base_value = 0;
    //    int over_value = 0;

    //    //물리방어력
    //    if (g_pHero->IsRangedNormalAttack())
    //    {
    //        base_value = player_attributes->GetBaseRangeDefPower();
    //        over_value = player_attributes->GetOptionPhysicalDefPower();
    //    }
    //    else
    //    {
    //        base_value = player_attributes->GetBaseMeleeDefPower();
    //        over_value = player_attributes->GetOptionPhysicalDefPower();
    //    }
    //    if (over_value != 0)
    //    {
    //        tooltip_manager->RegisterTooltipFormat(_T("%d + %d"), base_value, over_value);
    //    }
    //}
    //else if (IsOverControl(CHAR_SKILL_VALUE_PHYSIC_DEFENSE_MAX))
    //{
    //    // 물리방어력 옵션
    //    RenderTooltip_BaseAndCalc(tooltip_manager, 
    //        player_attributes->GetBaseOptionPhysicalDefPower(), 
    //        player_attributes->GetOptionPhysicalDefPower());
    //}
    else if (IsOverControl(CHAR_SKILL_VALUE_PHYSIC_ATTACK_RATE))
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
    else if (IsOverControl(CHAR_SKILL_VALUE_AVOID))
    {
        // 물리공격회피율
        RenderTooltip_BaseAndCalc(tooltip_manager, 
                                player_attributes->GetBasePhysicalAvoidRate(),
                                player_attributes->GetPhysicalAvoidRate());
    }
    else if (IsOverControl(CHAR_SKILL_VALUE_MOVE_SPEED))
    {
        // 이동 속도
        RenderTooltip_BaseAndCalc(tooltip_manager, 
                                ns_formula::CalcMoveSpeedRatio(player_attributes->GetDEX(), 0, 0), 
                                player_attributes->GetMoveSpeedRatio());
    }
    else if (IsOverControl(CHAR_SKILL_VALUE_ATTACK_SPEED))
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
    else if (IsOverControl(CHAR_SKILL_VALUE_CRITICAL_RATE))
    {
        // 크리티컬확률
        RenderTooltip_BaseAndCalc(tooltip_manager, 
                                player_attributes->GetBasePhysicalCriticalRatio(), 
                                player_attributes->GetPhysicalCriticalRatio());
    }
    else if (IsOverControl(CHAR_SKILL_VALUE_MAGIC_ATTACK_MIN))
    {
        // 추가 스킬 공격력
        RenderTooltip_BaseAndCalc(tooltip_manager, 
                                player_attributes->GetBaseSkillAttackPower(), 
                                player_attributes->GetSkillAttackPower());
    }
    else if (IsOverControl(CHAR_SKILL_ACCUMULATEPOINT_REWARD_BUTTON))
    {
        //! 접속포인트 보상버튼 툴팁
        uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
        if (ui_manager)
        {
            TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(6145, temp_string);
            Snprintf(tooltip_string, INTERFACE_STRING_LENGTH, temp_string, ui_manager->require_point());

            tooltip_manager->InitTooltip();
            tooltip_manager->RegisterTooltipLineChange(tooltip_string, _tcslen(tooltip_string));
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }
}

//------------------------------------------------------------------------------ 
void    Char_SkillDialog::UpdateParameter()
{
    PlayerAttributes* player_attributes = &(simulation_player_attributes());
    BASE_PLAYERINFO* player_info = simulation_scplayer().GetCharInfo();

    // 남아있는 스텟포인트
    SetParameterCaption(CHAR_SKILL_STAT, player_info->m_iRemainStat);

    // 기본 스탯 5종류
    SetParameterCaption(CHAR_SKILL_VALUE_STR, player_attributes->GetSTR(), player_attributes->GetBaseSTR());    //!< 힘
    SetParameterCaption(CHAR_SKILL_VALUE_DEX, player_attributes->GetDEX(), player_attributes->GetBaseDEX());    //!< 민첩
    SetParameterCaption(CHAR_SKILL_VALUE_STAMINA, player_attributes->GetVIT(), player_attributes->GetBaseVIT()); //!< 체력
    SetParameterCaption(CHAR_SKILL_VALUE_WISDOM, player_attributes->GetINT(), player_attributes->GetBaseINT()); //!< 지력
    SetParameterCaption(CHAR_SKILL_VALUE_SPIRIT, player_attributes->GetSPR(), player_attributes->GetBaseSPR()); //!< 정신력

    // 데미지
    {
        POSTYPE pos = getCtrlIDToPos( m_wzId[CHAR_SKILL_VALUE_PHYSIC_ATTACK_MIN] ); // 물리데미지 min ~ max
        CCtrlImageStaticWZ *pControl = (CCtrlImageStaticWZ *)getControl(pos);	

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
    }

    // 물리데미지 옵션
    SetParameterCaption(CHAR_SKILL_VALUE_PHYSIC_ATTACK_MAX, 
                        player_attributes->GetOptionPhysicalAttPower(), 
                        player_attributes->GetBaseOptionPhysicalAttPower());

    //물리방어력
    if (g_pHero->IsRangedNormalAttack())
    {
        SetParameterCaption(CHAR_SKILL_VALUE_PHYSIC_DEFENSE_RATE, 
                            player_attributes->GetBaseRangeDefPower(),// + player_attributes->GetOptionPhysicalDefPower(), 
                            player_attributes->GetBaseRangeDefPower());
    }
    else
    {
        SetParameterCaption(CHAR_SKILL_VALUE_PHYSIC_DEFENSE_RATE, 
                        player_attributes->GetBaseMeleeDefPower(),// + player_attributes->GetOptionPhysicalDefPower(), 
                        player_attributes->GetBaseMeleeDefPower());
    }

    // 물리방어력 옵션
    SetParameterCaption(CHAR_SKILL_VALUE_PHYSIC_DEFENSE_MAX, 
                        player_attributes->GetOptionPhysicalDefPower(), 
                        player_attributes->GetBaseOptionPhysicalDefPower());

    //크리티컬확률
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_CRITICAL_RATE,
                        player_attributes->GetPhysicalCriticalRatio(),
                        player_attributes->GetBasePhysicalCriticalRatio());


    //! 추가 스킬 공격력
    SetParameterCaption(CHAR_SKILL_VALUE_MAGIC_ATTACK_MIN, 
                        player_attributes->GetSkillAttackPower(), 
                        player_attributes->GetBaseSkillAttackPower());

    // 물리공격회피율
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_AVOID, 
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
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_PHYSIC_ATTACK_RATE, 
                        player_attributes->GetPhysicalAttackRate(), 
                        base_attack_rate + player_attributes->GetBasePhysicalAttackRate());

    //공격 속도
    SetParameterCaption(CHAR_SKILL_VALUE_ATTACK_SPEED,
                        player_attributes->GetAttSpeedRatio(),  //그냥 비율
                        ns_formula::CalcAttackSpeedRatio(g_pHero->GetClass(), 
                                                        g_pHero->GetWeaponKind(), 
                                                        player_attributes->GetDEX(), base_attack_speed, 0));
    //이동 속도
    SetParameterCaption(CHAR_SKILL_VALUE_MOVE_SPEED, 
                        player_attributes->GetMoveSpeedRatio(), 
                        ns_formula::CalcMoveSpeedRatio(player_attributes->GetDEX(), 0, 0));

    //수 ;공격력
    int over_value = 0;
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WATER);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WATER, eATTR_KIND_BASE);
    SetParameterCaption(CHAR_SKILL_VALUE_ATTR_WATER, over_value);

    //풍
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WIND);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_WIND, eATTR_KIND_BASE);
    SetParameterCaption(CHAR_SKILL_VALUE_ATTR_WIND, over_value);

    //지
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_EARTH);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_EARTH, eATTR_KIND_BASE);
    SetParameterCaption(CHAR_SKILL_VALUE_ATTR_GROUND, over_value);

    //화
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_FIRE);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_FIRE, eATTR_KIND_BASE);
    SetParameterCaption(CHAR_SKILL_VALUE_ATTR_FIRE, over_value);

    //암
    over_value = player_attributes->GetMagicalAttackPower(eATTACK_TYPE_DARKNESS);// - player_attributes->GetMagicalAttackPower(eATTACK_TYPE_DARKNESS, eATTR_KIND_BASE);
    SetParameterCaption(CHAR_SKILL_VALUE_ATTR_DARK, over_value);

    //수 ;방어력
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_ATTR_WATER2, 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WATER),
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WATER, eATTR_KIND_BASE));

    //풍
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_ATTR_WIND2, 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WIND), 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_WIND, eATTR_KIND_BASE));

    //지
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_ATTR_GROUND2, 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_EARTH),
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_EARTH, eATTR_KIND_BASE));
    //화
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_ATTR_FIRE2, 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_FIRE), 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_FIRE, eATTR_KIND_BASE));
    //암
    SetParameterCaptionWithPercent(CHAR_SKILL_VALUE_ATTR_DARK2, 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_DARKNESS), 
                        player_attributes->GetReduceElementDamageRatio(eATTACK_TYPE_DARKNESS, eATTR_KIND_BASE));

}

//------------------------------------------------------------------------------ 
void    Char_SkillDialog::CalcTotalSocketOption(OUT Char_SkillDialog::SocketOptionVector& socketcount_vector)
{
    HeroEquipmentContainer* equip_container = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());

    ItemUnitRenderer::RenderUnit * render_unit = NULL;
    ItemUnitRenderer* item_renderer = equip_container->GetItemUnitRender();

    item_renderer->GetItemUnitHashTable()->SetFirst();
    while (render_unit = item_renderer->GetItemUnitHashTable()->GetNext())
    {		
        if ((render_unit->m_pIconImage == NULL) || 
            (render_unit->m_pRefSlot == NULL) || 
            (render_unit->m_pRefSlot->GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot* item_slot = (SCItemSlot*)(render_unit->m_pRefSlot);
        if (item_slot->IsExpiredTime() || item_slot->IsExpiredWaitTime())
        {
            continue;
        }

        ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap socket_option_calc_result; 
        if (ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(*item_slot, socket_option_calc_result) == false)
        {
            continue;
        }

        ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = g_pHero->GetEquipmentContainer()->GetEquipmentManageInfo();
        ns_formula::EquipSlotsManageInfo::Node* it = equip_slot_manage_info->slots;
        if (!equip_slot_manage_info->slots[item_slot->GetPos()].activated) // 아이템이 활성화인 경우만 적용
        {
            continue;
        }
        
        FOREACH_CONTAINER(ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap::value_type& socket_option_calc_pair, 
                            socket_option_calc_result, 
                            ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap)
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult& socket_option_calc = socket_option_calc_pair.second;

            //------------------------------------------------------------------------------ 
            Char_SkillDialog::SocketOptionVector::iterator begin = socketcount_vector.begin();
            Char_SkillDialog::SocketOptionVector::iterator end = socketcount_vector.end();
            bool is_find = false;
            for (; begin != end; ++begin)
            {
                if (begin->attr_option_code == socket_option_calc.attr_option_code)
                {
                    begin->option_point += socket_option_calc.option_point;
                    is_find = true;
                    break;
                }
            }
            if (is_find == false)
            {
                socketcount_vector.push_back(socket_option_calc);
            }
        }
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::RenderTooltip_SocketOption(IN Char_SkillDialog::SocketOptionVector& socketcount_vector)
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    for (int i = 0; i < kSocketOptionShowRowCount; ++i)
    {
        if (i + socket_option_show_begin_index() >= (int)socketcount_vector.size())
        {
            return;
        }
        
        if (IsOverControl(static_cast<Char_Skill_Pos>(kControl_Text_S006 + i)))
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult* result = &(socketcount_vector[i + socket_option_show_begin_index()]);

            tooltip_manager->RegisterTooltipSocketOption(result->item_option_code);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
        }
    }

}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetSocketOptionValue(int col, int row, int val, WzColor text_color)
{
    int pos = kControl_Text_S021 + (col * kSocketOptionShowRowCount) + row;
    if (col < 0 || col >= kSocketOptionShowColCount || 
        row < 0 || row >= kSocketOptionShowRowCount || 
        pos < kControl_Text_S021 || pos > kControl_Text_S091)
    {
        return;
    }

    SetSocketOptionValue(static_cast<Char_Skill_Pos>(pos), val, text_color);
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetSocketOptionValue(Char_Skill_Pos pos, int val, WzColor text_color)
{
    CControlWZ* control_ptr = getControl(pos);

    if (control_ptr != NULL)
    {
        static TCHAR str[MAX_PATH] = {NULL, };
        Sprintf(str, _T("%d"), val);

        if (control_ptr->IsKind(CTRL_KIND_TEXT) == true)
        {
            (static_cast<CCtrlStaticWZ*>(control_ptr))->SetTextColor(text_color);
        }

        control_ptr->SetTextWZA(str);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SetSocketOptionValue(Char_Skill_Pos pos, TCHAR* text_string, WzColor text_color)
{
    CControlWZ* control_ptr = getControl(pos);

    if (control_ptr != NULL)
    {
        if (control_ptr->IsKind(CTRL_KIND_TEXT) == true)
        {
            (static_cast<CCtrlStaticWZ*>(control_ptr))->SetTextColor(text_color);
        }

        control_ptr->SetTextWZA(text_string);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::UpdateSocketOptionValue(SCItemSlot* item_slot, 
                                          int col, 
                                          Char_SkillDialog::SocketOptionVector& socketcount_vector)
{
    ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap socket_option_calc_result; 
    if (ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(*item_slot, socket_option_calc_result) == true)
    {
        FOREACH_CONTAINER(ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap::value_type& socket_option_calc_pair, 
            socket_option_calc_result, 
            ItemAttrCalculator::SocketOptionCalculator::CalculationResultMap)
        {
            ItemAttrCalculator::SocketOptionCalculator::CalculationResult& socket_option_calc = socket_option_calc_pair.second;

            int row = GetRowSocketOptionByOptionPoint(socket_option_calc.attr_option_code, 
                                                        socketcount_vector);

            if ((row < 0) ||
                (row < socket_option_show_begin_index()) ||
                (row - socket_option_show_begin_index() >= kSocketOptionShowRowCount) ||
                (row >= (int)socketcount_vector.size()))
            {
                continue;
            }

            ZardOptionInfo* zard_option_info = SocketOptionParser::Instance()->FindZardOption(socket_option_calc.item_option_code);
            if (zard_option_info == NULL)
            {
                continue;
            }

            WzColor text_color = WzColor_RGBA(255,255,255,255);

            const ZardOptionInfo::OptionValueInfo* option_value_info = zard_option_info->Find(socketcount_vector[row].option_point);
            if (item_slot->IsExpiredTime() || item_slot->IsExpiredWaitTime())
            {
                text_color = WzColor_RGBA(110,110,110,255);
            }
            else
            {
                if (option_value_info != NULL)
                {
                    if (option_value_info->point > 0)
                    {
                        text_color = WzColor_RGBA(0,255,0,255);
                    }
                    else if (option_value_info->point < 0)
                    {
                        text_color = WzColor_RGBA(0,0,255,255);
                    }
                }
            }

            SetSocketOptionValue(col, 
                                row - socket_option_show_begin_index(), 
                                socket_option_calc.option_point, 
                                text_color);
        }
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::UpdateSocketOptionName(Char_SkillDialog::SocketOptionVector& socketcount_vector)
{
    if (socket_option_show_begin_index() >= (int)socketcount_vector.size())
    {
        return;
    }

    //! 스킬명
    Char_SkillDialog::SocketOptionVector::iterator begin = socketcount_vector.begin() + socket_option_show_begin_index();
    Char_SkillDialog::SocketOptionVector::iterator end = socketcount_vector.end();
    int index = 0;
    for (; begin != end; ++begin)
    {
        BASE_OptionList* socket_option_desc = OptionListParser::Instance()->GetOptionList(begin->item_option_code);
        if (socket_option_desc != NULL)
        {
            WzColor text_color = WzColor_RGBA(255,255,255,255);

            TCHAR str_name[INTERFACE_STRING_LENGTH] = {NULL, };
            TCHAR str_msg[INTERFACE_STRING_LENGTH] = {NULL, };
            TCHAR str_format[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(socket_option_desc->mdwNameCode, str_name);
            getControl(kControl_Text_S006 + index)->SetTextWZ(str_name);

            ZardOptionInfo* zard_option_info = SocketOptionParser::Instance()->FindZardOption(begin->item_option_code);
            if (zard_option_info != NULL)
            {
                const ZardOptionInfo::OptionValueInfo* option_value_info = zard_option_info->Find(begin->option_point);
                if (option_value_info != NULL)
                {
                    text_color = GetSocketOptionTextColor(option_value_info->value);

                    //! 발동
                    if (option_value_info->value > 0)
                    {
                        _tcscpy(str_format, _T("%s +%d"));
                    }
                    else
                    {
                        _tcscpy(str_format, _T("%s %d"));
                    }
                    //if ((option_value_info->type == VALUE_TYPE_PERCENT_PER_CUR) || (option_value_info->type == VALUE_TYPE_PERCENT_PER_MAX))

                    Sprintf(str_msg, str_format, str_name, option_value_info->value);

                    //옵션명 접미사
                    GlobalFunc::AddOptionSuffix(option_value_info->type, socket_option_desc, str_msg);
                }
                else
                {
                    //! 미발동
                    g_InterfaceManager.GetInterfaceString(3444, str_msg);
                }

                SetSocketOptionValue(static_cast<Char_Skill_Pos>(kControl_Text_S001 + index), str_msg, text_color);
            }

            SetSocketOptionValue(static_cast<Char_Skill_Pos>(kControl_Text_S030 + index), begin->option_point, text_color);
        }

        ++index;

        if (kControl_Text_S006 + index > kControl_Text_S013)
        {
            break;
        }
    }
}
//------------------------------------------------------------------------------ 
bool Char_SkillDialog::OnVScrollThumb(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return true;
    }

    int pos2 = scroll_control->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        scroll_control->SetScrollPosWZ(pos);
    }

    POSTYPE control_index = getCtrlIDToPos(message->dwCtrlID);
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }

    return true;
}
//------------------------------------------------------------------------------ 
void     Char_SkillDialog::OnVScrollUp(CCtrlVScrollWZ* scroll_control)
{
    if (scroll_control == NULL)
    {
        return;
    }

    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    POSTYPE control_index = getCtrlIDToPos(scroll_control->GetCtrlID());
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }
}
//------------------------------------------------------------------------------ 
void     Char_SkillDialog::OnVScrollDown(CCtrlVScrollWZ* scroll_control)
{
    if (scroll_control == NULL)
    {
        return;
    }

    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax); 

    if (pos < mmax)
    {
        ++pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    POSTYPE control_index = getCtrlIDToPos(scroll_control->GetCtrlID());
    if (control_index == kControl_VScroll_V000)
    {
        set_socket_option_show_begin_index(pos);
    }
}
//------------------------------------------------------------------------------ 
bool Char_SkillDialog::OnVScrollUp(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));

    OnVScrollUp(scroll_control);

    return true;
}
//------------------------------------------------------------------------------ 
bool Char_SkillDialog::OnVScrollDown(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));

    OnVScrollDown(scroll_control);

    return true;
}
//------------------------------------------------------------------------------ 
eATTR_TYPE Char_SkillDialog::GetAttrTypeFromButtonPos(Char_Skill_Pos pos)
{
    switch (pos)
    {
    case CHAR_SKILL_STR_B: return eATTR_STR; 
    case CHAR_SKILL_DEX_B: return eATTR_DEX; 
    case CHAR_SKILL_VIT_B: return eATTR_VIT; 
    case CHAR_SKILL_INT_B: return eATTR_INT; 
    case CHAR_SKILL_SPR_B: return eATTR_SPR; 
    case kControl_Button_B005: return eATTR_STR; 
    case kControl_Button_B011: return eATTR_DEX; 
    case kControl_Button_B012: return eATTR_VIT; 
    case kControl_Button_B013: return eATTR_INT; 
    case kControl_Button_B014: return eATTR_SPR; 
    }

    return eATTR_TYPE_INVALID;
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::UpdateSimulationPlayerAttributes(Char_Skill_Pos pos, int add_value)
{
    eATTR_TYPE attr_type = GetAttrTypeFromButtonPos(pos);
    if (attr_type == eATTR_TYPE_INVALID)
    {
        return;
    }

    int org_value = g_pHero->GetPlayerAttribute()->GetAttrValue(attr_type);
    int cur_value = simulation_player_attributes().GetAttrValue(attr_type); 
    int base_value = simulation_player_attributes().GetAttrValue(attr_type, eATTR_KIND_BASE);

    if (pos >= kControl_Button_B005)
    {
        add_value = -add_value;
    }

    if (add_value < 0)
    {
        if ((cur_value <= org_value) || (simulation_stat_count(attr_type) <= 0))
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

        if ((cur_value >= kMaxStatValue) || (simulation_stat_count(attr_type) >= kMaxStatValue))
        {
            return;
        }
        if ((cur_value + add_value) > kMaxStatValue)
        {
            add_value = kMaxStatValue - cur_value;
        }
        if (add_value > simulation_scplayer().GetCharInfo()->m_iRemainStat)
        {
            add_value = simulation_scplayer().GetCharInfo()->m_iRemainStat;
        }
    }

    if (add_value == 0)
    {
        return;
    }

    simulation_player_attributes().SetAttrValue(attr_type, eATTR_KIND_BASE, base_value + add_value);
    simulation_player_attributes().Update();

    int apply_value = simulation_player_attributes().GetAttrValue(attr_type);

    simulation_scplayer().GetCharInfo()->m_iRemainStat -= add_value;    //(add_value - (next_value - apply_value));

    //! 변경 스텟 카운트를 저장한다.
    add_simulation_stat_count(attr_type, add_value);

    getControl(kControl_Button_B015)->EnableWindowWZ(IsSimulationStatChanged());
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::SEND_STATUS_STAT_APPLY_SYN()
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
    syn.stat_point[eATTR_STR] = simulation_stat_count(eATTR_STR);
    syn.stat_point[eATTR_DEX] = simulation_stat_count(eATTR_DEX);
    syn.stat_point[eATTR_VIT] = simulation_stat_count(eATTR_VIT);
    syn.stat_point[eATTR_INT] = simulation_stat_count(eATTR_INT);
    syn.stat_point[eATTR_SPR] = simulation_stat_count(eATTR_SPR);

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &syn, sizeof(syn));


    PacketRequestManager::Instance()->PushPacket((new MSG_CG_STATUS_STAT_APPLY_SYN),
                                                (new MSG_CG_STATUS_STAT_APPLY_ACK), 
                                                (new MSG_CG_STATUS_STAT_APPLY_NAK),
                                                sizeof(MSG_CG_STATUS_STAT_APPLY_SYN), 
                                                PACKET_WAITING_MILLISECOND, 
                                                false);
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::RefreshSimulationPlayerAttributes()
{
    if (IsVisible() == FALSE)
    {
        return;
    }
    if (g_pHero->GetCharInfo() == NULL)
    {
        return;
    }

    static BASE_PLAYERINFO player_info;

    player_info = *(g_pHero->GetCharInfo());
    simulation_scplayer_.SetCharInfo(&player_info);
    simulation_scplayer_.SetState(0);
    simulation_scplayer_.SetWeaponKind(g_pHero->GetWeaponKind());

    simulation_player_attributes().Clear();
    g_pHero->GetPlayerAttribute()->Clone(&(simulation_player_attributes()));
    simulation_player_attributes().SetOwner(&simulation_scplayer_);

    int cur_str_stat_count = simulation_player_attributes().GetAttrValue(eATTR_STR, eATTR_KIND_BASE);
    int cur_dex_stat_count = simulation_player_attributes().GetAttrValue(eATTR_DEX, eATTR_KIND_BASE);
    int cur_vit_stat_count = simulation_player_attributes().GetAttrValue(eATTR_VIT, eATTR_KIND_BASE);
    int cur_int_stat_count = simulation_player_attributes().GetAttrValue(eATTR_INT, eATTR_KIND_BASE);
    int cur_spr_stat_count = simulation_player_attributes().GetAttrValue(eATTR_SPR, eATTR_KIND_BASE);

    simulation_player_attributes().SetAttrValue(eATTR_STR, eATTR_KIND_BASE, cur_str_stat_count + simulation_stat_count(eATTR_STR));
    simulation_player_attributes().SetAttrValue(eATTR_DEX, eATTR_KIND_BASE, cur_dex_stat_count + simulation_stat_count(eATTR_DEX));
    simulation_player_attributes().SetAttrValue(eATTR_VIT, eATTR_KIND_BASE, cur_vit_stat_count + simulation_stat_count(eATTR_VIT));
    simulation_player_attributes().SetAttrValue(eATTR_INT, eATTR_KIND_BASE, cur_int_stat_count + simulation_stat_count(eATTR_INT));
    simulation_player_attributes().SetAttrValue(eATTR_SPR, eATTR_KIND_BASE, cur_spr_stat_count + simulation_stat_count(eATTR_SPR));

    simulation_player_attributes().Update();

    simulation_scplayer().GetCharInfo()->m_iRemainStat -= (simulation_stat_count(eATTR_STR) + 
                                                            simulation_stat_count(eATTR_DEX) + 
                                                            simulation_stat_count(eATTR_VIT) + 
                                                            simulation_stat_count(eATTR_INT) + 
                                                            simulation_stat_count(eATTR_SPR));

    RefreshStatButton();
}
//------------------------------------------------------------------------------ 
bool Char_SkillDialog::IsSimulationStatChanged()
{
    return (g_pHero->GetCharInfo()->m_iRemainStat != simulation_scplayer().GetCharInfo()->m_iRemainStat);
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::RefreshStatButton()
{
    if (g_pHero->GetCharInfo()->m_iRemainStat > 0)
    {
        getControl(CHAR_SKILL_STR_B)->ShowWindowWZ(WZ_SHOW);
        getControl(CHAR_SKILL_DEX_B)->ShowWindowWZ(WZ_SHOW);
        getControl(CHAR_SKILL_VIT_B)->ShowWindowWZ(WZ_SHOW);
        getControl(CHAR_SKILL_INT_B)->ShowWindowWZ(WZ_SHOW);
        getControl(CHAR_SKILL_SPR_B)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B005)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B011)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B012)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B013)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B014)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B015)->ShowWindowWZ(WZ_SHOW);
        getControl(kControl_Button_B015)->EnableWindowWZ(IsSimulationStatChanged());
    }
    else
    {
        getControl(CHAR_SKILL_STR_B)->ShowWindowWZ(WZ_HIDE);
        getControl(CHAR_SKILL_DEX_B)->ShowWindowWZ(WZ_HIDE);
        getControl(CHAR_SKILL_VIT_B)->ShowWindowWZ(WZ_HIDE);
        getControl(CHAR_SKILL_INT_B)->ShowWindowWZ(WZ_HIDE);
        getControl(CHAR_SKILL_SPR_B)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B005)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B011)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B012)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B013)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B014)->ShowWindowWZ(WZ_HIDE);
        getControl(kControl_Button_B015)->ShowWindowWZ(WZ_HIDE);
    }
}
//------------------------------------------------------------------------------ 
void Char_SkillDialog::CallbackSendPacketStatusStatApply(bool is_yes)
{
    if (is_yes)
    {
        Char_SkillDialog* status_dialog = GET_CAST_DIALOG(Char_SkillDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG);
        if ((status_dialog != NULL) && (status_dialog->IsVisible() == TRUE))
        {
            status_dialog->SEND_STATUS_STAT_APPLY_SYN();
        }
    }
}
#endif //_NA_003027_20111013_HONOR_SYSTEM