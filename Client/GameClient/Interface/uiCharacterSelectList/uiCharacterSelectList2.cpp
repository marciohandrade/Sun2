#include "SunClientPrecompiledHeader.h"
#include "uiCharacterSelectList2.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiLoginMan/uiLoginMan.h"
#include "CharacterScene.h"
#include "InterfaceManager.h"
#include "FTextParser.h"
#include "FTextRender.h"
#include "MapInfoParser.h"
#include "GlobalFunc.h"
#include "CharSelectDialog.h"
#include "objectmanager.h"
#include "Player.h"

#ifdef _NA_000000_20130114_RENEWER_UI 
#include "AppearanceResourceCode.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

namespace
{
    const WzID kCharacterNameFont = StrToWzID("st11");
    const WzID kLevelFont = StrToWzID("st11");
    const WzID kCurrentMapNameFont = StrToWzID("st10");

    const DWORD kCharacterNameColor = WzColor_RGBA(255, 255, 255, 255);
    const DWORD kLevelColor = WzColor_RGBA(255, 199, 4, 255);
    const DWORD kCurrentMapNameColor = WzColor_RGBA(160, 160, 160, 255);
    const DWORD kCloseSlotColor = WzColor_RGBA(255, 0, 0, 200);

    const int kListLeftMargin = 10;
};

uiCharacterSelectList2::uiCharacterSelectList2(InterfaceManager* ui_manager):
uiBase(ui_manager)
,save_post_select_(-1)
,save_now_select_(-1)
{
	
}

uiCharacterSelectList2::~uiCharacterSelectList2()
{

}

void uiCharacterSelectList2::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControl();

	int isStap[] = 
	{
		kControl_Max_Step_1,
		kControl_Max_Step_2,
		kControl_Max_Step_3,
		kControl_Max_Step_4,
		kControl_Max_Step_5,
		kControl_Max_Step_6,
		kControl_Max_Step_7,
		kControl_Max_Step_8,
		kControl_Max_Step_9,
		kControl_Max_Step_10
	};

	int isSlotMax[] = 
	{
		KControl_0Slot_Max,
		KControl_1Slot_Max,
		KControl_2Slot_Max,
		KControl_3Slot_Max,
		KControl_4Slot_Max,
		KControl_5Slot_Max,
		KControl_6Slot_Max,
		KControl_7Slot_Max,
		KControl_8Slot_Max,
		KControl_9Slot_Max,
		KControl_10Slot_Max
	};

	//각 컨트롤들을 안에 넣는다
	for (int index = 0 ; index < kMaxListSlot ; ++index)
	{
		int step_value = isStap[index];
		int slotMax = isSlotMax[index];

		for (int i = slotMax+ 1 ; i < step_value ; ++i)
		{
			//slotmax보다 +1더 들어가야 된다.
			DWORD control_id = GetControlID_byPosition(i);
			character_info_[index].save_character_index_[i - (slotMax+1)] = i;
			character_info_[index].charater_Image_[i - (slotMax+1)] = GetControlWZ(control_id);
		}

		slotMax = isSlotMax[index+1];

		for (int i = step_value+ 1 ; i < slotMax ; ++i)
		{
			DWORD control_id = GetControlID_byPosition(i);
			CControlWZ* control = GetControlWZ(control_id);;
			if(i == slotMax-1)
			{
				//이 구간이 마지막 배경컨트롤이 들어간다.
				character_info_[index].Back_Check_ = control;
			}
			else
			{
				character_info_[index].save_Kind_index_[i-(step_value+1)] = i;
				character_info_[index].kind_Image_[i-(step_value+1)] = control;
			}

		}

		character_info_[index].lock_slot_ = false;
		character_info_[index].lock_Image = NULL;
	}

	character_info_[6].lock_slot_ = true;
	character_info_[7].lock_slot_ = true;
	character_info_[8].lock_slot_ = true;
	character_info_[9].lock_slot_ = true;

	DWORD control_id = GetControlID_byPosition(kControl_Picture_P021);
	character_info_[6].lock_Image =  GetControlWZ(control_id);
	control_id = GetControlID_byPosition(kControl_Picture_P022);
	character_info_[7].lock_Image =  GetControlWZ(control_id);
	control_id = GetControlID_byPosition(kControl_Picture_P024);
	character_info_[8].lock_Image =  GetControlWZ(control_id);
	control_id = GetControlID_byPosition(kControl_Picture_P025);
	character_info_[9].lock_Image =  GetControlWZ(control_id);

	for (int index = 0 ; index < kMaxListSlot ; ++index)
	{
		character_info_[index].all_show_character_Image(WZ_HIDE);
		character_info_[index].all_show_Kind_Image(WZ_HIDE);
	}

	character_info_[0].save_back_check_ = kControl_Check_C006;
	character_info_[1].save_back_check_ = kControl_Check_C001;
	character_info_[2].save_back_check_ = kControl_Check_C008;
	character_info_[3].save_back_check_ = kControl_Check_C009;
	character_info_[4].save_back_check_ = kControl_Check_C010;
	character_info_[5].save_back_check_ = kControl_Check_C011;
	character_info_[6].save_back_check_ = kControl_Check_C012;
	character_info_[7].save_back_check_ = kControl_Check_C013;
	character_info_[8].save_back_check_ = kControl_Check_C014;
	character_info_[9].save_back_check_ = kControl_Check_C015;
}

void uiCharacterSelectList2::RegisterControl()
{
	RegisterControlPosition(StrToWzID("C002"), kControl_Check_C002);
	RegisterControlPosition(StrToWzID("C042"), kControl_Check_C042);
	RegisterControlPosition(StrToWzID("C041"), kControl_Check_C041);
	RegisterControlPosition(StrToWzID("C031"), kControl_Check_C031);
	RegisterControlPosition(StrToWzID("C021"), kControl_Check_C021);
	RegisterControlPosition(StrToWzID("C000"), kControl_Check_C000);
    RegisterControlPosition(StrToWzID("C141"), kControl_Check_C141);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C054"), kControl_Check_C054);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
	RegisterControlPosition(StrToWzID("C101"), kControl_Check_C101);
	RegisterControlPosition(StrToWzID("C091"), kControl_Check_C091);
	RegisterControlPosition(StrToWzID("C063"), kControl_Check_C063);
	RegisterControlPosition(StrToWzID("C062"), kControl_Check_C062);
	RegisterControlPosition(StrToWzID("C061"), kControl_Check_C061);
	RegisterControlPosition(StrToWzID("C007"), kControl_Check_C007);
    RegisterControlPosition(StrToWzID("C151"), kControl_Check_C151);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C124"), kControl_Check_C124);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
	RegisterControlPosition(StrToWzID("C006"), kControl_Check_C006);
	//-1 slot max
	RegisterControlPosition(StrToWzID("C003"), kControl_Check_C003);
	RegisterControlPosition(StrToWzID("C070"), kControl_Check_C070);
	RegisterControlPosition(StrToWzID("C057"), kControl_Check_C057);
	RegisterControlPosition(StrToWzID("C036"), kControl_Check_C036);
	RegisterControlPosition(StrToWzID("C026"), kControl_Check_C026);
	RegisterControlPosition(StrToWzID("C016"), kControl_Check_C016);
    RegisterControlPosition(StrToWzID("C142"), kControl_Check_C142);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C055"), kControl_Check_C055);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
	RegisterControlPosition(StrToWzID("C102"), kControl_Check_C102);
	RegisterControlPosition(StrToWzID("C092"), kControl_Check_C092);
	RegisterControlPosition(StrToWzID("C082"), kControl_Check_C082);
	RegisterControlPosition(StrToWzID("C073"), kControl_Check_C073);
	RegisterControlPosition(StrToWzID("C064"), kControl_Check_C064);
	RegisterControlPosition(StrToWzID("C052"), kControl_Check_C052);
    RegisterControlPosition(StrToWzID("C152"), kControl_Check_C152);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C161"), kControl_Check_C161);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
	RegisterControlPosition(StrToWzID("C001"), kControl_Check_C001);
	//-2 slot max
	RegisterControlPosition(StrToWzID("C004"), kControl_Check_C004);
	RegisterControlPosition(StrToWzID("C071"), kControl_Check_C071);
	RegisterControlPosition(StrToWzID("C058"), kControl_Check_C058);
	RegisterControlPosition(StrToWzID("C037"), kControl_Check_C037);
	RegisterControlPosition(StrToWzID("C027"), kControl_Check_C027);
	RegisterControlPosition(StrToWzID("C017"), kControl_Check_C017);
    RegisterControlPosition(StrToWzID("C143"), kControl_Check_C143);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C056"), kControl_Check_C056);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
	RegisterControlPosition(StrToWzID("C133"), kControl_Check_C133);
	RegisterControlPosition(StrToWzID("C125"), kControl_Check_C125);
	RegisterControlPosition(StrToWzID("C109"), kControl_Check_C109);
	RegisterControlPosition(StrToWzID("C099"), kControl_Check_C099);
	RegisterControlPosition(StrToWzID("C089"), kControl_Check_C089);
	RegisterControlPosition(StrToWzID("C080"), kControl_Check_C080);
    RegisterControlPosition(StrToWzID("C153"), kControl_Check_C153);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C162"), kControl_Check_C162);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
	RegisterControlPosition(StrToWzID("C008"), kControl_Check_C008);
	//-3 slot max
	RegisterControlPosition(StrToWzID("C005"), kControl_Check_C005);
	RegisterControlPosition(StrToWzID("C072"), kControl_Check_C072);
	RegisterControlPosition(StrToWzID("C059"), kControl_Check_C059);
	RegisterControlPosition(StrToWzID("C038"), kControl_Check_C038);
	RegisterControlPosition(StrToWzID("C028"), kControl_Check_C028);
	RegisterControlPosition(StrToWzID("C018"), kControl_Check_C018);
    RegisterControlPosition(StrToWzID("C144"), kControl_Check_C144);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C117"), kControl_Check_C117);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
	RegisterControlPosition(StrToWzID("C134"), kControl_Check_C134);
	RegisterControlPosition(StrToWzID("C126"), kControl_Check_C126);
	RegisterControlPosition(StrToWzID("C110"), kControl_Check_C110);
	RegisterControlPosition(StrToWzID("C100"), kControl_Check_C100);
	RegisterControlPosition(StrToWzID("C090"), kControl_Check_C090);
	RegisterControlPosition(StrToWzID("C081"), kControl_Check_C081);
    RegisterControlPosition(StrToWzID("C154"), kControl_Check_C154);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C163"), kControl_Check_C163);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
	RegisterControlPosition(StrToWzID("C009"), kControl_Check_C009);
	//-4 slot max
	RegisterControlPosition(StrToWzID("C047"), kControl_Check_C047);
	RegisterControlPosition(StrToWzID("C074"), kControl_Check_C074);
	RegisterControlPosition(StrToWzID("C060"), kControl_Check_C060);
	RegisterControlPosition(StrToWzID("C039"), kControl_Check_C039);
	RegisterControlPosition(StrToWzID("C029"), kControl_Check_C029);
	RegisterControlPosition(StrToWzID("C019"), kControl_Check_C019);
    RegisterControlPosition(StrToWzID("C145"), kControl_Check_C145);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C118"), kControl_Check_C118);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
	RegisterControlPosition(StrToWzID("C135"), kControl_Check_C135);
	RegisterControlPosition(StrToWzID("C127"), kControl_Check_C127);
	RegisterControlPosition(StrToWzID("C111"), kControl_Check_C111);
	RegisterControlPosition(StrToWzID("C103"), kControl_Check_C103);
	RegisterControlPosition(StrToWzID("C093"), kControl_Check_C093);
	RegisterControlPosition(StrToWzID("C083"), kControl_Check_C083);
    RegisterControlPosition(StrToWzID("C155"), kControl_Check_C155);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C168"), kControl_Check_C168);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
	RegisterControlPosition(StrToWzID("C010"), kControl_Check_C010);
	//-5 slot max
	RegisterControlPosition(StrToWzID("C048"), kControl_Check_C048);
	RegisterControlPosition(StrToWzID("C075"), kControl_Check_C075);
	RegisterControlPosition(StrToWzID("C065"), kControl_Check_C065);
	RegisterControlPosition(StrToWzID("C040"), kControl_Check_C040);
	RegisterControlPosition(StrToWzID("C030"), kControl_Check_C030);
	RegisterControlPosition(StrToWzID("C020"), kControl_Check_C020);
    RegisterControlPosition(StrToWzID("C146"), kControl_Check_C146);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C119"), kControl_Check_C119);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
	RegisterControlPosition(StrToWzID("C136"), kControl_Check_C136);
	RegisterControlPosition(StrToWzID("C128"), kControl_Check_C128);
	RegisterControlPosition(StrToWzID("C112"), kControl_Check_C112);
	RegisterControlPosition(StrToWzID("C104"), kControl_Check_C104);
	RegisterControlPosition(StrToWzID("C094"), kControl_Check_C094);
	RegisterControlPosition(StrToWzID("C084"), kControl_Check_C084);
    RegisterControlPosition(StrToWzID("C156"), kControl_Check_C156);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C169"), kControl_Check_C169);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
	RegisterControlPosition(StrToWzID("C011"), kControl_Check_C011);
	//-6 slot max
	RegisterControlPosition(StrToWzID("C049"), kControl_Check_C049);
	RegisterControlPosition(StrToWzID("C076"), kControl_Check_C076);
	RegisterControlPosition(StrToWzID("C066"), kControl_Check_C066);
	RegisterControlPosition(StrToWzID("C043"), kControl_Check_C043);
	RegisterControlPosition(StrToWzID("C032"), kControl_Check_C032);
	RegisterControlPosition(StrToWzID("C022"), kControl_Check_C022);
    RegisterControlPosition(StrToWzID("C147"), kControl_Check_C147);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C120"), kControl_Check_C120);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
	RegisterControlPosition(StrToWzID("C137"), kControl_Check_C137);
	RegisterControlPosition(StrToWzID("C129"), kControl_Check_C129);
	RegisterControlPosition(StrToWzID("C113"), kControl_Check_C113);
	RegisterControlPosition(StrToWzID("C105"), kControl_Check_C105);
	RegisterControlPosition(StrToWzID("C095"), kControl_Check_C095);
	RegisterControlPosition(StrToWzID("C085"), kControl_Check_C085);
    RegisterControlPosition(StrToWzID("C157"), kControl_Check_C157);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C166"), kControl_Check_C166);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
	RegisterControlPosition(StrToWzID("C012"), kControl_Check_C012);
	//-7 slot max
	RegisterControlPosition(StrToWzID("C050"), kControl_Check_C050);
	RegisterControlPosition(StrToWzID("C077"), kControl_Check_C077);
	RegisterControlPosition(StrToWzID("C067"), kControl_Check_C067);
	RegisterControlPosition(StrToWzID("C044"), kControl_Check_C044);
	RegisterControlPosition(StrToWzID("C033"), kControl_Check_C033);
	RegisterControlPosition(StrToWzID("C023"), kControl_Check_C023);
    RegisterControlPosition(StrToWzID("C148"), kControl_Check_C148);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C121"), kControl_Check_C121);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
	RegisterControlPosition(StrToWzID("C138"), kControl_Check_C138);
	RegisterControlPosition(StrToWzID("C130"), kControl_Check_C130);
	RegisterControlPosition(StrToWzID("C114"), kControl_Check_C114);
	RegisterControlPosition(StrToWzID("C106"), kControl_Check_C106);
	RegisterControlPosition(StrToWzID("C096"), kControl_Check_C096);
	RegisterControlPosition(StrToWzID("C086"), kControl_Check_C086);
    RegisterControlPosition(StrToWzID("C158"), kControl_Check_C158);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C167"), kControl_Check_C167);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
	RegisterControlPosition(StrToWzID("C013"), kControl_Check_C013);
	//-8 slot max
	RegisterControlPosition(StrToWzID("C051"), kControl_Check_C051);
	RegisterControlPosition(StrToWzID("C078"), kControl_Check_C078);
	RegisterControlPosition(StrToWzID("C068"), kControl_Check_C068);
	RegisterControlPosition(StrToWzID("C045"), kControl_Check_C045);
	RegisterControlPosition(StrToWzID("C034"), kControl_Check_C034);
	RegisterControlPosition(StrToWzID("C024"), kControl_Check_C024);
    RegisterControlPosition(StrToWzID("C149"), kControl_Check_C149);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C122"), kControl_Check_C122);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
	RegisterControlPosition(StrToWzID("C139"), kControl_Check_C139);
	RegisterControlPosition(StrToWzID("C131"), kControl_Check_C131);
	RegisterControlPosition(StrToWzID("C115"), kControl_Check_C115);
	RegisterControlPosition(StrToWzID("C107"), kControl_Check_C107);
	RegisterControlPosition(StrToWzID("C097"), kControl_Check_C097);
	RegisterControlPosition(StrToWzID("C087"), kControl_Check_C087);
    RegisterControlPosition(StrToWzID("C159"), kControl_Check_C159);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C165"), kControl_Check_C165);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
	RegisterControlPosition(StrToWzID("C014"), kControl_Check_C014);
	//-9 slot max
	RegisterControlPosition(StrToWzID("C053"), kControl_Check_C053);
	RegisterControlPosition(StrToWzID("C079"), kControl_Check_C079);
	RegisterControlPosition(StrToWzID("C069"), kControl_Check_C069);
	RegisterControlPosition(StrToWzID("C046"), kControl_Check_C046);
	RegisterControlPosition(StrToWzID("C035"), kControl_Check_C035);
	RegisterControlPosition(StrToWzID("C025"), kControl_Check_C025);
    RegisterControlPosition(StrToWzID("C150"), kControl_Check_C150);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C123"), kControl_Check_C123);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
	RegisterControlPosition(StrToWzID("C140"), kControl_Check_C140);
	RegisterControlPosition(StrToWzID("C132"), kControl_Check_C132);
	RegisterControlPosition(StrToWzID("C116"), kControl_Check_C116);
	RegisterControlPosition(StrToWzID("C108"), kControl_Check_C108);
	RegisterControlPosition(StrToWzID("C098"), kControl_Check_C098);
	RegisterControlPosition(StrToWzID("C088"), kControl_Check_C088);
    RegisterControlPosition(StrToWzID("C160"), kControl_Check_C160);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("C164"), kControl_Check_C164);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
	RegisterControlPosition(StrToWzID("C015"), kControl_Check_C015);
	//-10 slot max

	RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
	RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
	RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
	RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
}

void uiCharacterSelectList2::Release()
{

}

void uiCharacterSelectList2::MessageProc( SI_MESSAGE * message )
{
	if (IsLock())
		return;

	if (g_CharSelectScene.m_bPickCharLock == TRUE)
	{
		return;
	}

	static bool is_double_click = false;

#ifdef _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG
	if (g_CharSelectScene.IsSendSelectCharPacket())
	{
		is_double_click = false;
		return;
	}
#endif // _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG

	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
			if (is_double_click == true)
			{
				is_double_click = false;
			}
			else
			{
				OnLButtonClick(message);
			}            
		}        
		break;
	case RT_MSG_LBUTTONDCLICK:
		OnLButtonDClick(message);
		is_double_click = true;
		break;
	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

void uiCharacterSelectList2::OnShowWindow(BOOL val)
{
	if (val)
	{
		SetServerName();
		CharacterListUp();
	}
	else
	{

	}
}

void uiCharacterSelectList2::OnRenderSolarDialog()
{

	for (int i = 0 ; i < kMaxListSlot ; ++i)
	{
		if(character_info_[i].lock_Image && character_info_[i].lock_slot_ == true)
		{
			int			iMouseX = Mouse::Instance()->GetMouseX();
			int			iMouseY = Mouse::Instance()->GetMouseY();
			uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			RECT rc = character_info_[i].lock_Image->GetSizeRect();
			if (tooltip_manager)
			{
				if (rc.left <= iMouseX && rc.right >= iMouseX)
				{
					if (rc.top <= iMouseY && rc.bottom >= iMouseY)
					{
						TCHAR ToolTip_Disalbe[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString(5994, ToolTip_Disalbe);
						tooltip_manager->RegisterTooltip(ToolTip_Disalbe);
						tooltip_manager->SetTooltipDialogID(GetDialogKey());
					}
				}
			}
		}
		if(character_info_[i].lock_slot_ == false)
		{
			character_info_[i].character_info_text_.Render();
		}

	}
	

}

void uiCharacterSelectList2::OnUpdateSolarDialog()
{
#ifdef _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG
    if (g_CharSelectScene.IsSendSelectCharPacket())
    {
        return;
    }
#endif // _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG

    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();;
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);

        if ((mouse_wheel_state == MOUSE_WHEEL_UP) || 
            g_Input.GetState(DIK_UP, KS_UP) ||
            g_Input.GetState(DIK_LEFT, KS_UP))
        {
            OnSelectPrevCharacter();
        }
        else if ((mouse_wheel_state == MOUSE_WHEEL_DOWN) || 
                 g_Input.GetState(DIK_DOWN, KS_UP) ||
                 g_Input.GetState(DIK_RIGHT, KS_UP))
        {
            OnSelectNextCharacter();
        }
    }
}

bool uiCharacterSelectList2::OnLButtonClick( SI_MESSAGE* message )
{
	bool result = true;
	DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
	switch (control_position)
	{
	case kControl_Check_C006:SetSelectedCharacterListIndex(0);break;
	case kControl_Check_C001:SetSelectedCharacterListIndex(1);break;
	case kControl_Check_C008:SetSelectedCharacterListIndex(2);break;
	case kControl_Check_C009:SetSelectedCharacterListIndex(3);break;
	case kControl_Check_C010:SetSelectedCharacterListIndex(4);break;
	case kControl_Check_C011:SetSelectedCharacterListIndex(5);break;
	case kControl_Check_C012:SetSelectedCharacterListIndex(6);break;
	case kControl_Check_C013:SetSelectedCharacterListIndex(7);break;
	case kControl_Check_C014:SetSelectedCharacterListIndex(8);break;
	case kControl_Check_C015:SetSelectedCharacterListIndex(9);break;
		break;
	default:
		{
			//누군가 클릭이 되었다 하지만 이는 누구인가
			//눌러진 객체중 체크 버튼 확인
			for (int i = 0 ; i < kMaxListSlot ; ++i)
			{
				if(character_info_[i].isRangeCharacter(control_position))
				{
					SetSelectedCharacterListIndex(i); 
					break;
				}
				if(character_info_[i].isRangeKind(control_position))
				{
					SetSelectedCharacterListIndex(i); 
					break;
				}
			}

			//활성화된 체크 버튼 비활성화 되지 않도록 
		}
		break;
	}
	return result;
}

bool uiCharacterSelectList2::OnLButtonDClick( SI_MESSAGE* message )
{
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    BYTE list_slot_size = character_slot_list_.size();

    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    if (list_slot_size <= control_position &&
        make_and_makeable_slot > control_position)
    {
        CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
        if (charater_select_dialog)
        {
            charater_select_dialog->OnCreateCharacter();
        }
        return false;
    }

    if (g_CharSelectScene.GetCurSelectCharacterInfo() &&
        (g_CharSelectScene.GetCurSelectCharacterInfo()->CharacterInfo.m_DelChk != 0))
    {
        CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
        if (charater_select_dialog)
        {
            charater_select_dialog->OnDeleteCharacter();
        }
        return false;
    }

    switch (control_position)
    {
    case kControl_Check_C006:CheckModifyDoubleClick(0); break;
    case kControl_Check_C001:CheckModifyDoubleClick(1); break;
    case kControl_Check_C008:CheckModifyDoubleClick(2); break;
    case kControl_Check_C009:CheckModifyDoubleClick(3); break;
    case kControl_Check_C010:CheckModifyDoubleClick(4); break;
    case kControl_Check_C011:CheckModifyDoubleClick(5); break;
    case kControl_Check_C012:CheckModifyDoubleClick(6); break;
    case kControl_Check_C013:CheckModifyDoubleClick(7); break;
    case kControl_Check_C014:CheckModifyDoubleClick(8); break;
    case kControl_Check_C015:CheckModifyDoubleClick(9); break;
    default:
		{	
			//누군가 클릭이 되었다 하지만 이는 누구인가
			//눌러진 객체중 체크 버튼 확인
			for (int i = 0 ; i < kMaxListSlot ; ++i)
			{
				if(character_info_[i].isRangeCharacter(control_position))
				{
					CheckModifyDoubleClick(i); 
					break;
				}
				if(character_info_[i].isRangeKind(control_position))
				{
					CheckModifyDoubleClick(i); 
					break;
				}
			}
		}
        break;
    }

    return result;
}

void uiCharacterSelectList2::SetServerName()
{
    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top == NULL)
    {
        return;
    }

    uiLoginMan* login_manager = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
    if (login_manager == NULL)
    {
        return;
    }

    AuthSetverData* server_data = login_manager->GetCurrentServerData();
    if (server_data == NULL)
    {
        return;
    }

#ifdef _DH_LAST_SELECTED_CHARACTER
    uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
    if (select_manager)
    {
        select_manager->SetLastSelectServer(static_cast<WORD>(server_data->m_GroupID), static_cast<WORD>(server_data->m_ChannelID));
    }
#endif//_DH_LAST_SELECTED_CHARACTER

    TCHAR* server_name = const_cast<TCHAR*>(server_data->m_GroupName.c_str());
    login_back_top->SetSceneTitleString(server_name);
}

void uiCharacterSelectList2::CharacterListUp()
{

	for (int index = 0 ; index < kMaxListSlot ; ++index)
	{
		character_info_[index].all_show_character_Image(WZ_HIDE);
		character_info_[index].all_show_Kind_Image(WZ_HIDE);
	}

    character_slot_list_.clear();
    selected_list_index_ = 0;

    for (int slot_index = 0; slot_index < MAX_CHARACTER_LIST_NUM; ++slot_index)
    {
        CharListInfo* character_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(slot_index);
        if (character_info == NULL)
        {
            continue;
        }
        character_slot_list_.push_back(slot_index);
    }

#ifdef _DH_LAST_SELECTED_CHARACTER //캐릭터 삭제시 및 갱신시 선택 부분이 다시 처리 되게 된다.
    uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
    if (select_manager && select_manager->last_selected_header().last_select_version > 0 && 
        select_manager->last_selected_header().last_select_version <= LAST_SELECT_VERSION)
    {
        int selected_slot_index = select_manager->FindLastSelectedSlotIndex(select_manager->last_selected_header().last_select_server);
        selected_list_index_ = GetListIndexFromSlotIndex(selected_slot_index);
    }
#endif//_DH_LAST_SELECTED_CHARACTER

    RefreshSelectList();
    SetSelectedCharacterListIndex(selected_list_index_);
}

void uiCharacterSelectList2::RefreshSelectList()
{
    int list_size = static_cast<int>(character_slot_list_.size());
    assert(list_size <= kMaxListSlot);

    ShowListSlotByCount(list_size); // 빈슬롯 사용 못하게

    for (int slot_index = 0; slot_index < kMaxListSlot; ++slot_index)
    {
        if (slot_index >= static_cast<int>(character_slot_list_.size()))
        {
            SetCharacterSlot(slot_index);
        }
        else
        {
            CharListInfo* character_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(character_slot_list_[slot_index]);
            if (character_info == NULL)
            {
                continue;
            }

            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR delete_string[INTERFACE_STRING_LENGTH] = "%d:%d";

            g_InterfaceManager.GetInterfaceString(70560, buffer_string);

            Player* player = static_cast<Player*>(g_ObjectManager.GetPlayer(character_info->dwObjectKey));

            if (character_info->CharacterInfo.m_DelChk != 0 && player)
            {
                int day = 0 , hour = 0 , minute = 0;
                int RemainTotalTime = player->GetCharDelRemainTime();
                int remainder = RemainTotalTime;
                // 1일 보다 큰가
                if( RemainTotalTime >= 86400)
                {
                    day = RemainTotalTime / 86400;
                    remainder = RemainTotalTime % 86400;
                }
                // 1시간 보다 큰가
                if( RemainTotalTime >= 3600 )
                {
                    hour = remainder / 3600;
                    remainder = RemainTotalTime % 3600;
                }
                // 분 단위 인가
                minute = remainder / 60;
                sprintf(delete_string, buffer_string, day, (hour+1));
            }

            CLIENT_CHARACTER_PART* part_info = &(character_info->CharacterInfo);
            SetCharacterSlotInfo(slot_index, 
                part_info->m_tszCharName, 
                part_info->m_DelChk,
                part_info->m_LV, 
                part_info->m_byClass, 
                part_info->m_iRegion,
                delete_string,
				part_info->gender);
        }
    }
}

void uiCharacterSelectList2::ShowListSlotByCount(int slot_count)
{
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    for (int slot_index = 0; slot_index < kMaxListSlot; ++slot_index)
    {
		
        if (slot_index >= make_and_makeable_slot)
        {
			character_info_[slot_index].lock_slot_ = true;
			character_info_[slot_index].lock_Image->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
			character_info_[slot_index].lock_slot_ = false;
			if (character_info_[slot_index].lock_Image != NULL)
			{
				character_info_[slot_index].lock_Image->ShowWindowWZ(WZ_HIDE);
			}
        }
       character_info_[slot_index].character_info_text_.ClearSentence();
    }

    CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
    if (charater_select_dialog)
    {
        charater_select_dialog->EnableButton((slot_count < make_and_makeable_slot), CharSelectDialog::CharSelect_Create);
    }
}

void uiCharacterSelectList2::SetCharacterSlot(int list_index)
{
    if (list_index < 0 || list_index >= kMaxListSlot)
    {
        return;
    }

	RECT Base_rect = character_info_[list_index].Back_Check_->GetSizeRect();
	int Left_out = (int)character_info_[list_index].charater_Image_[0]->GetSizeWidth();
	int Right_out = (int)character_info_[list_index].kind_Image_[0]->GetSizeWidth();

	Base_rect.left += Left_out;
	Base_rect.right -= Right_out;

    TCHAR character_info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR class_string[INTERFACE_STRING_LENGTH] = {'\0', };
    FTextParser parser;
    FTextSentence sentence;
    RECT control_rect = Base_rect;

    character_info_[list_index].character_info_text_.Init(GetDrawBase(), control_rect);
    character_info_[list_index].character_info_text_.SetVCenterAlign(true);
    character_info_[list_index].character_info_text_.SetHCenterAlign(true);
    character_info_[list_index].character_info_text_.SetMargin(kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_TOP + 1,
        kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_BOTTOM);

    // 캐릭터 이름
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    if (list_index < make_and_makeable_slot)
    {
        g_InterfaceManager.GetInterfaceString(5993, character_info_string); // 5993: 캐릭터를 생성 하실 수 있습니다.
        parser.SetDefaultFont(kCharacterNameFont, kLevelColor);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(5994, character_info_string); // 5994: 캐릭터를 생성 하실 수 없습니다.
        parser.SetDefaultFont(kCharacterNameFont, kCloseSlotColor);
    }
    
     parser.Parser(character_info_string, sentence);
     character_info_[list_index].character_info_text_.Append(sentence);
     character_info_[list_index].character_info_text_.convertLineInfo();
}

void uiCharacterSelectList2::SetCharacterSlotInfo(int list_index, 
                                                 TCHAR* character_name, 
                                                 BYTE delete_check,
                                                 int level, 
                                                 BYTE class_type, 
                                                 MAPCODE current_mapcode,
                                                 TCHAR* day_for_delete,
												 BYTE Gender)
{

	if (character_name == NULL)
	{
		return;
	}

	if (list_index < 0 || list_index >= kMaxListSlot)
	{
		return;
	}

	int kind_index = 0;
	int class_index = 0;

	onShowCheckUICharacterandKind(class_type,Gender,class_index,kind_index);
	character_info_[list_index].show_character_Image(WZ_SHOW,class_index);
	character_info_[list_index].show_Kind_Image(WZ_SHOW,kind_index);


    TCHAR character_info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR class_string[INTERFACE_STRING_LENGTH] = {'\0', };
    FTextParser parser;
    FTextSentence sentence;

	RECT Base_rect = character_info_[list_index].Back_Check_->GetSizeRect();
	int Left_out = (int)character_info_[list_index].charater_Image_[0]->GetSizeWidth();
	int Right_out = (int)character_info_[list_index].kind_Image_[0]->GetSizeWidth();

	Base_rect.left += Left_out;
	Base_rect.right -= Right_out;

    character_info_[list_index].character_info_text_.Init(GetDrawBase(), Base_rect);
    character_info_[list_index].character_info_text_.SetVCenterAlign(false);
    character_info_[list_index].character_info_text_.SetHCenterAlign(false);
    character_info_[list_index].character_info_text_.SetMargin(kListLeftMargin, 
		5, 
		kListLeftMargin, 
		0);

	int baselineGap = character_info_[list_index].character_info_text_.getGapLine();
	int basecharGap = character_info_[list_index].character_info_text_.getGapChar();

	character_info_[list_index].character_info_text_.SetGap(basecharGap,-1);

    // 캐릭터 이름
    Snprintf(character_info_string, INTERFACE_STRING_LENGTH - 1, character_name);

    if (delete_check > 0)
    {
        parser.SetDefaultFont(kCharacterNameFont, kCloseSlotColor); // 삭제 대기중이면 빨간색
    }
    else
    {
        parser.SetDefaultFont(kCharacterNameFont, kCharacterNameColor);
    }    
    parser.Parser(character_info_string, sentence);
    character_info_[list_index].character_info_text_.Append(sentence);

    // 캐릭터 레벨 & 클래스
    int class_name_code = GlobalFunc::GetClassNameCode(class_type);
    g_InterfaceManager.GetInterfaceString(class_name_code, class_string);
    Snprintf(character_info_string, INTERFACE_STRING_LENGTH - 1, "%d %s", level, class_string);
    parser.SetDefaultFont(kLevelFont, kLevelColor);
    parser.Parser(character_info_string, sentence);
    character_info_[list_index].character_info_text_.Append(sentence);

    // 현재지역
    const sMAPINFO* current_map_info = MapInfoParser::Instance()->FindMapInfo(current_mapcode);
    if (delete_check > 0)
    { // 삭제 대기중이면 삭제 대기 메시지
        g_InterfaceManager.GetInterfaceString(5038, class_string);
        Snprintf(character_info_string, INTERFACE_STRING_LENGTH, "%s [%s]", class_string, day_for_delete);
        parser.SetDefaultFont(kCurrentMapNameFont, kCurrentMapNameColor);
        parser.Parser(character_info_string, sentence);
        character_info_[list_index].character_info_text_.Append(sentence);
    }
    else
    {
        if (current_map_info != NULL)
        {
            g_InterfaceManager.GetInterfaceString(current_map_info->dwNCode, character_info_string);
            parser.SetDefaultFont(kCurrentMapNameFont, kCurrentMapNameColor);
            parser.Parser(character_info_string, sentence);
            character_info_[list_index].character_info_text_.Append(sentence);
        }
    }

    character_info_[list_index].character_info_text_.convertLineInfo();
}

void uiCharacterSelectList2::SetSelectedCharacterSlotIndex(int slot_index)
{
    int list_size = static_cast<int>(character_slot_list_.size());
    int list_index = -1;
    
    for (int index = 0; index < list_size; ++index)
    {
        if (character_slot_list_[index] == slot_index)
        {
            list_index = index;
            break;
        }
    }

    SetSelectedCharacterListIndex(list_index);
}

void uiCharacterSelectList2::SetSelectedCharacterListIndex(int list_index)
{
	onCheckUIList(list_index);

    int list_size = static_cast<int>(character_slot_list_.size());
    if (list_size < 1)
    {
        list_index = -1;
    }

    selected_list_index_ = list_index;
	

    g_CharSelectScene.ShowSelectCharacter(GetSelectedCharacterSlotIndex());
}

void uiCharacterSelectList2::OnSelectPrevCharacter()
{
    --selected_list_index_;
    selected_list_index_ = max(selected_list_index_, 0);
    
    SetSelectedCharacterListIndex(selected_list_index_);
}

void uiCharacterSelectList2::OnSelectNextCharacter()
{
    ++selected_list_index_;
    selected_list_index_ = min(selected_list_index_, kMaxListSlot);
    selected_list_index_ = min(selected_list_index_, static_cast<int>(character_slot_list_.size() - 1)) ;
    if (selected_list_index_ == -1)
    {
        selected_list_index_ = 0;
    }

    SetSelectedCharacterListIndex(selected_list_index_);
}

int uiCharacterSelectList2::GetSelectedCharacterSlotIndex()
{
    int list_size = static_cast<int>(character_slot_list_.size());
    if ((list_size < 1) || 
        (list_size <= selected_list_index_))
    {
        return -1;
    }
    int index = character_slot_list_[selected_list_index_];
    return index;
}
//------------------------------------------------------------------------------
int uiCharacterSelectList2::GetListIndexFromSlotIndex(int slot_index)
{
    std::vector<int>::iterator begin = character_slot_list_.begin(); 
    std::vector<int>::iterator end = character_slot_list_.end();
    for (int index = 0; begin != end; ++begin, ++index)
    {
        if ((*begin) == slot_index)
        {
            return index;
        }
    }
    return 0;
}

//넘어오는 캐릭터에 따라 인덱스가 틀려야됨.
void uiCharacterSelectList2::onShowCheckUICharacterandKind( byte classidx,
														   byte gender, 
														   int& charIndex, 
														   int& KindIndex )
{
	switch(classidx)
	{
	case eCHAR_BERSERKER:
			KindIndex = 5;
		break;
	case eCHAR_DRAGON:
			KindIndex = 4;
		break;
	case eCHAR_SHADOW:
			KindIndex = 3;
		break;
	case eCHAR_ELEMENTALIST:
			KindIndex = 2;
		break;
	case eCHAR_MYSTIC:
			KindIndex = 1;
		break;
	case eCHAR_VALKYRIE:
			KindIndex = 0;
		break;
    case eCHAR_HELLROID:
            KindIndex = 6;
        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
            KindIndex = 7;
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}

	eCHAR_TYPE race_type = ResourceCode::GetImageClassType((eCHAR_TYPE)classidx, (GenderType::Value)gender);

	switch(race_type)
	{
	case eCHAR_BERSERKER:
		charIndex = 5;
		break;
	case eCHAR_DRAGON:
		charIndex = 4;
		break;
	case eCHAR_SHADOW:
		charIndex = 3;
		break;
	case eCHAR_ELEMENTALIST:
		charIndex = 0;
		break;
	case eCHAR_MYSTIC:
		charIndex = 1;
		break;
	case eCHAR_VALKYRIE:
		charIndex = 2;
		break;
    case eCHAR_HELLROID:
        charIndex = 6;
        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        charIndex = 7;
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}
}

void uiCharacterSelectList2::onCheckUIList( int NowClickList )
{
	save_now_select_ = NowClickList;

	if(save_post_select_ != -1)
	{
		character_info_[save_post_select_].show_control_check(false);
	}
	if(save_now_select_ != -1)
	{
		character_info_[save_now_select_].show_control_check(true);
	}

	save_post_select_ = save_now_select_;
}

void uiCharacterSelectList2::CheckModifyDoubleClick( DWORD PosCheck )
{
	SetSelectedCharacterListIndex(PosCheck);

	g_Input.Reset();
	TCHAR notice_message[INTERFACE_STRING_LENGTH] = {'\0', };
	WzColor	notice_color = WzColor_RGBA(255, 80, 80, 255);

	if( g_CharSelectScene.SendCharSelectPacket() == FALSE )
	{
		CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
		if (charater_select_dialog)
		{
			charater_select_dialog->OnCreateCharacter();
		}
	}
}

#endif