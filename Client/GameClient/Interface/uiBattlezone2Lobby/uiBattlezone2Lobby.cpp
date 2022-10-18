#include "SunClientPrecompiledHeader.h"
#ifdef _DH_BATTLEZONE2_
#include "BattlezoneMissionTreeParser.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "UserSlot.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "SkillInfoParser.h"
#include "map.h"
#include "BattleScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GameFunc.h"
#include "ItemShareSystem.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GameConst_Color.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "uiPartyMan/uiPartyMan.h"
#include "CastDialog.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"

#define WAIT_TIME 5
#define MAX_PLAYER_SLOT 10
//------------------------------------------------------------------------------
void uiBattlezone2Lobby::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
    RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
    RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B034"), kControl_Button_B034);
    RegisterControlPosition(StrToWzID("B035"), kControl_Button_B035);
    RegisterControlPosition(StrToWzID("B036"), kControl_Button_B036);
    RegisterControlPosition(StrToWzID("B037"), kControl_Button_B037);
    RegisterControlPosition(StrToWzID("B038"), kControl_Button_B038);
    RegisterControlPosition(StrToWzID("B039"), kControl_Button_B039);
    RegisterControlPosition(StrToWzID("B040"), kControl_Button_B040);
    RegisterControlPosition(StrToWzID("B041"), kControl_Button_B041);
    RegisterControlPosition(StrToWzID("B042"), kControl_Button_B042);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);//임시 변경 버튼
    RegisterControlPosition(StrToWzID("T007"), kControl_Text_T007);//미션명
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);//방번호
    RegisterControlPosition(StrToWzID("T002"), kControl_Text_T002);
    RegisterControlPosition(StrToWzID("T003"), kControl_Text_T003);
    RegisterControlPosition(StrToWzID("T008"), kControl_Text_T008);
    RegisterControlPosition(StrToWzID("T010"), kControl_Text_T010);
    RegisterControlPosition(StrToWzID("T009"), kControl_Text_T009);
    RegisterControlPosition(StrToWzID("T011"), kControl_Text_T011);
    RegisterControlPosition(StrToWzID("T004"), kControl_Text_T004);
    RegisterControlPosition(StrToWzID("T201"), kControl_Text_T201);//레벨 1    
    RegisterControlPosition(StrToWzID("T202"), kControl_Text_T202);
    RegisterControlPosition(StrToWzID("T203"), kControl_Text_T203);
    RegisterControlPosition(StrToWzID("T204"), kControl_Text_T204);
    RegisterControlPosition(StrToWzID("T205"), kControl_Text_T205);
    RegisterControlPosition(StrToWzID("T206"), kControl_Text_T206);
    RegisterControlPosition(StrToWzID("T207"), kControl_Text_T207);
    RegisterControlPosition(StrToWzID("T208"), kControl_Text_T208);
    RegisterControlPosition(StrToWzID("T209"), kControl_Text_T209);
    RegisterControlPosition(StrToWzID("T210"), kControl_Text_T210);//레벨10
    RegisterControlPosition(StrToWzID("T101"), kControl_Text_T101);//이름1
    RegisterControlPosition(StrToWzID("T102"), kControl_Text_T102);
    RegisterControlPosition(StrToWzID("T103"), kControl_Text_T103);
    RegisterControlPosition(StrToWzID("T104"), kControl_Text_T104);
    RegisterControlPosition(StrToWzID("T105"), kControl_Text_T105);
    RegisterControlPosition(StrToWzID("T106"), kControl_Text_T106);
    RegisterControlPosition(StrToWzID("T107"), kControl_Text_T107);
    RegisterControlPosition(StrToWzID("T108"), kControl_Text_T108);
    RegisterControlPosition(StrToWzID("T109"), kControl_Text_T109);
    RegisterControlPosition(StrToWzID("T110"), kControl_Text_T110);//이름10
    //RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P041"), kControl_Picture_P041);
    RegisterControlPosition(StrToWzID("P042"), kControl_Picture_P042);
    RegisterControlPosition(StrToWzID("P043"), kControl_Picture_P043);
    RegisterControlPosition(StrToWzID("P044"), kControl_Picture_P044);
    RegisterControlPosition(StrToWzID("P045"), kControl_Picture_P045);
    RegisterControlPosition(StrToWzID("P046"), kControl_Picture_P046);
    RegisterControlPosition(StrToWzID("P047"), kControl_Picture_P047);
    RegisterControlPosition(StrToWzID("P048"), kControl_Picture_P048);
    RegisterControlPosition(StrToWzID("P049"), kControl_Picture_P049);

    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032); // 하이라이트 슬롯 이미지1
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
    RegisterControlPosition(StrToWzID("P034"), kControl_Picture_P034);
    RegisterControlPosition(StrToWzID("P035"), kControl_Picture_P035);
    RegisterControlPosition(StrToWzID("P036"), kControl_Picture_P036);
    RegisterControlPosition(StrToWzID("P037"), kControl_Picture_P037);
    RegisterControlPosition(StrToWzID("P038"), kControl_Picture_P038);
    RegisterControlPosition(StrToWzID("P039"), kControl_Picture_P039);
    RegisterControlPosition(StrToWzID("P040"), kControl_Picture_P040);
    RegisterControlPosition(StrToWzID("P050"), kControl_Picture_P050); // 하이라이트 슬롯 이미지10

    RegisterControlPosition(StrToWzID("P051"), kControl_Picture_P051); // 방장 마크1
    RegisterControlPosition(StrToWzID("P052"), kControl_Picture_P052);
    RegisterControlPosition(StrToWzID("P053"), kControl_Picture_P053);
    RegisterControlPosition(StrToWzID("P054"), kControl_Picture_P054);
    RegisterControlPosition(StrToWzID("P055"), kControl_Picture_P055);
    RegisterControlPosition(StrToWzID("P056"), kControl_Picture_P056);
    RegisterControlPosition(StrToWzID("P057"), kControl_Picture_P057);
    RegisterControlPosition(StrToWzID("P058"), kControl_Picture_P058);
    RegisterControlPosition(StrToWzID("P059"), kControl_Picture_P059);
    RegisterControlPosition(StrToWzID("P060"), kControl_Picture_P060); // 방장 마크10
}

uiBattlezone2Lobby::uiBattlezone2Lobby(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
    ui_battlezone2_manager_ = NULL;
    control_ready_start_ = NULL;

    if (GetManager())
    {
        GetManager()->InitializeUserSlotSet();
    }
    is_ready_ = false;
    is_open_slot_count_ = MAX_VIEW_USER_SLOT;
    clicked_slot_index_ = MAX_VIEW_USER_SLOT;
    map_code_ = 0;

    handle_texture_[USE_TEXTURE_READY] = INVALID_HANDLE_VALUE;
}

uiBattlezone2Lobby::~uiBattlezone2Lobby(void)
{    
}

void uiBattlezone2Lobby::Clear()
{
    is_ready_ = false;
    is_open_slot_count_ = MAX_VIEW_USER_SLOT;
    kick_player_key_ = 0;
    clicked_slot_index_ = MAX_VIEW_USER_SLOT;

    if (control_ready_start_)
    {
        control_ready_start_->EnableWindowWZ(true);
        is_ready_button_enable_ = true;
    }
    map_code_ = 0;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    is_disconnected_uesr_kick_ = false;
    ZeroMemory(kick_player_name_,sizeof(kick_player_name_));
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}

void uiBattlezone2Lobby::Init(CDrawBase* draw_base)
{
    RegisterControl();

    //handle_texture_[USE_TEXTURE_READY] = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\63_ready.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    handle_texture_[USE_TEXTURE_READY] = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_[USE_TEXTURE_READY]	= g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_[USE_TEXTURE_READY]);
    assert(texture_information_[USE_TEXTURE_READY]);

    int i = 0;
    for (i = kControl_Button_B001; i < kControl_Button_B033+1; ++i)
    {
        control_user_slots_[i-kControl_Button_B001] = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(i));
        assert(control_user_slots_[i-kControl_Button_B001]);
    }

    for (i = kControl_Button_B002; i < kControl_Button_B042+1; ++i)
    {
        control_ban_[i-kControl_Button_B002] = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(i));
        assert(control_ban_[i-kControl_Button_B002]);
        if (control_ban_[i-kControl_Button_B002])
        {
            control_ban_[i-kControl_Button_B002]->ShowWindowWZ(WZ_HIDE);//강퇴버튼은 유저가 없으면 안보여야한다.
        }
    }

    for (i = kControl_Text_T201; i < kControl_Text_T210+1; ++i)
    {
        control_user_levels_[i-kControl_Text_T201] = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(i));
        assert(control_user_levels_[i-kControl_Text_T201]);
        if (control_user_levels_[i-kControl_Text_T201])
        {
            control_user_levels_[i-kControl_Text_T201]->SetTextWZ("");
        }
    }

    for (i = kControl_Text_T101; i < kControl_Text_T110+1; ++i)
    {
        control_user_names_[i-kControl_Text_T101] = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(i));
        assert(control_user_names_[i-kControl_Text_T101]);
        if (control_user_names_[i-kControl_Text_T101])
        {
            control_user_names_[i-kControl_Text_T101]->SetTextWZ("");
        }
    }

    //활성화 슬롯(내가 있는 슬롯)
    for (i = kControl_Picture_P032; i < kControl_Picture_P050+1; ++i)
    {
        control_highlight_user_[i-kControl_Picture_P032] = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i));
        assert(control_highlight_user_[i-kControl_Picture_P032]);
        if (control_highlight_user_[i-kControl_Picture_P032])
        {
            control_highlight_user_[i-kControl_Picture_P032]->ShowWindowWZ(WZ_HIDE);
        }
    }
    
#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
    if (control_highlight_user_[0])//나는 항상 첫 슬롯에 있다.
    {
        control_highlight_user_[0]->ShowWindowWZ(WZ_SHOW);
    }
#endif//_DH_BATTLEZONE2_LOBBY_SLOT

    //방장 표시 마크
    for (i = kControl_Picture_P051; i < kControl_Picture_P060+1; ++i)
    {
        control_master_mark_[i-kControl_Picture_P051] = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i));
        assert(control_master_mark_[i-kControl_Picture_P051]);
        if (control_master_mark_[i-kControl_Picture_P051])
        {
            control_master_mark_[i-kControl_Picture_P051]->ShowWindowWZ(WZ_HIDE);
        }
    }

	CCtrlButtonCheckWZ* close_slots_= static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(StrToWzID("C000")));
	assert(close_slots_);        
	close_slots_->SetDiscardMessage(true);

    for (i = kControl_CheckBT_C001; i < kControl_CheckBT_C009+1; ++i)//체크버튼은 9개임
    {
        control_close_slots_[i-kControl_CheckBT_C001] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
        assert(control_close_slots_[i-kControl_CheckBT_C001]);        
		control_close_slots_[i-kControl_CheckBT_C001]->SetDiscardMessage(true);
    }

    for (i = kControl_Picture_P031; i < kControl_Picture_P049+1; ++i)
    {
        control_playing_user_[i-kControl_Picture_P031] = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i));
        assert(control_playing_user_[i-kControl_Picture_P031]);
        if (control_playing_user_[i-kControl_Picture_P031])
        {
            control_playing_user_[i-kControl_Picture_P031]->ShowWindowWZ(WZ_HIDE);
        }
    }

    // 드랍방식 드랍다운 컨트롤
    bool init_result = dropdown_item_share().Init(
        GetControlWZ(GetControlID_byPosition(kControl_Button_B007)),
        static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B005))),
        static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000))),
        NULL,
        NULL);

    if (dropdown_item_share().list_control() != NULL)
    {
        dropdown_item_share().DeleteItemAll();
        dropdown_item_share().list_control()->SetStartLineGapTop(8);
        TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
        DWORD distribution_string;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        const int distribution_max = eITEM_DISTRIBUTION_MAX;
#else
        const int distribution_max = eITEM_DISTRIBUTION_MAX_DICE_BEFORE;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
        for (int i = 0; i < distribution_max; ++i)
        {
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            distribution_string = ItemShareSystem::distribution_type_list[i];
#else
            distribution_string = i;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
            g_InterfaceManager.GetInterfaceString(
                ItemShareSystem::distribution_type_string_code[distribution_string],
                message,
                INTERFACE_STRING_LENGTH);

            dropdown_item_share().InsertItem(message, (void*)distribution_string,
#ifdef _INTERNATIONAL_UI
                StrToWzID("st10"),
#else
                StrToWzID("mn12"),
#endif//_INTERNATIONAL_UI
                DT_CENTER | DT_BOTTOM, c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);
        }

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        dropdown_item_share().Select(0);
#else
        dropdown_item_share().Select(1);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING   
    }

    CControlWZ* control = GetControlWZ_byPosition(kControl_Button_B006); //팀바꾸기
    if (control)
    {
        //control->ShowWindowWZ(WZ_HIDE);
        control->EnableWindowWZ(false);
    }

    control_ready_start_ = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B000)); //시작, 준비
    if (control_ready_start_)
    {
        control_ready_start_->SetTextWZ("");
    }
    assert(control_ready_start_);

    is_ready_button_enable_ = true;
    kick_player_key_ = 0;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    is_disconnected_uesr_kick_ = false;
    ZeroMemory(kick_player_name_, sizeof(kick_player_name_));
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    is_party_obtain_room_ = false;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

    MoveDefaultWindowPos();
}

void uiBattlezone2Lobby::SetControlByPlayerMove(BYTE index, bool is_enter)
{
    if (index < 0 && index >= MAX_VIEW_USER_SLOT)
    {
        assert(!"uiBattlezone2Lobby::SetControlByPlayerMove(Wrong slot index)");
        return;
    }

    if (control_ban_[index])
    {
        control_ban_[index]->ShowWindowWZ(is_enter?WZ_SHOW:WZ_HIDE);
    }
    if (control_user_levels_[index])
    {
        if (is_enter == false)
        {
            control_user_levels_[index]->SetTextWZ("");
        }
    }
    if (control_user_names_[index])
    {
        if (is_enter == false)
        {
            control_user_levels_[index]->SetTextWZ("");
        }
    }
}
//------------------------------------------------------------------------------
uiBattlezone2Man* uiBattlezone2Lobby::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}

void uiBattlezone2Lobby::ChangeSetting()
{
    if (GetManager()->IsMaster())
    {
        SettingForMaster();
    }
    else
    {
        SettingForMember();
    }
}

void uiBattlezone2Lobby::SettingForMaster()
{
    int max_member = kControl_Button_B042 - kControl_Button_B002 + 1;
    for (int i = 0; i < max_member; ++i)
    {
        if (control_ban_[i])
        {
            control_ban_[i]->ShowWindowWZ(WZ_SHOW);
        }
    }
    if (control_ban_[0])
    {
        control_ban_[0]->ShowWindowWZ(WZ_HIDE);
    }
    
    if (control_ready_start_)
    {
        control_ready_start_->SetTextID(115);
    }
}

void uiBattlezone2Lobby::SettingForMember()
{
    int max_member = kControl_Button_B042 - kControl_Button_B002 + 1;
    for (int i = 0; i < max_member; ++i)
    {
        if (control_ban_[i])
        {
            control_ban_[i]->ShowWindowWZ(WZ_HIDE);
        }
    }

    if (control_ready_start_)
    {
        if (is_ready_)
        {
            control_ready_start_->SetTextID(302);
        }
        else
        {
            control_ready_start_->SetTextID(110);
        }        
    }
}

void uiBattlezone2Lobby::Release()
{
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
    }
}

void uiBattlezone2Lobby::OnUpdateSolarDialog()
{
    CControlWZ* control = GetControlWZ_byPosition(kControl_Button_B003);
    if (GetManager()->IsMaster())
    {
        if (GetManager()->IsReadyForStart())
        {
            control_ready_start_->SetTextID(115);
        }
        else
        {
            control_ready_start_->SetTextID(70242);
        }
        if (control)
        {
            control->EnableWindowWZ(true);
        }
    }
    else
    {
        if (control)
        {
            control->EnableWindowWZ(false);
        }

        if (control_ready_start_)
		{
	        if (is_ready_)
	        {
	            control_ready_start_->SetTextID(302);
	        }
	        else
	        {
	            control_ready_start_->SetTextID(110);
	        }
		}
    }

    RECT control_rect;
    uiBattlezone2Tree* battlezone2_tree = GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);

    if (battlezone2_tree && battlezone2_tree->IsVisible())
    {
        GetDialogWZ()->GetClientRect(&control_rect);
        battlezone2_tree->MoveWindowWZ(control_rect.right, control_rect.top);
    }

    if (GetDialogWZ()->HaveFocus() == true)
    {
        if (battlezone2_tree && battlezone2_tree->IsVisible())
        {
            GlobalFunc::MoveToTopDialog(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
        }
    }

    if (g_pHero && g_pHero->IsDead())
    {
        if (control_ready_start_ && is_ready_button_enable_)
        {
            control_ready_start_->EnableWindowWZ(false);
            is_ready_button_enable_ = false;
        }
        if (is_ready_)
        {
            SendGameReady();
        }
    }
    else
    {
        if (control_ready_start_ && !(is_ready_button_enable_))
        {
            control_ready_start_->EnableWindowWZ(true);
            is_ready_button_enable_ = true;
        }
    }

    //드랍다운 컨트롤 업데이트
    dropdown_item_share().Update();
}

bool uiBattlezone2Lobby::SendRequestRenderInfo(WORD party_key, DWORD party_member_key)
{
    if (party_member_key == 0 || party_key == 0)
    {
        return false;
    }

    if (GetManager()->SafePKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_RENDER))
    {
        GetManager()->SetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_RENDER);
        //GetManager()->FlushUserSlotSet();

        MSG_CG_PARTY_REQUEST_RENDER_INFO_SYN send_packet;
        send_packet.request_party_key_ = party_key;
        send_packet.request_member_key_ = party_member_key;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        return true;
    }
    return false;
}

bool uiBattlezone2Lobby::SendRequestMember()
{
    if (GetManager()->SafePKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_MEMBER))
    {
        GetManager()->SetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_MEMBER);
        GetManager()->FlushUserSlotSet();

        uiPartyMan* partyMan =
            static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        DWORD master_key = partyMan->GetMasterKey();
        if (ui_battlezone2_manager_ && partyMan)
        {
            ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());
        }
        else
        {
            assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_JOIN_ROOM_ACK): Can't not find master key");
        }

#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
#else
        if (master_key != g_pHero->GetObjectKey())
        {
            UserSlot EnterUser;
            EnterUser.SetPlayerKey(master_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            EnterUser.SetPlayerName(partyMan()->GetPartyLeader().Get().m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            ui_battlezone2_manager_->AddPlayer(EnterUser);
        }
#endif//_DH_BATTLEZONE2_LOBBY_SLOT
        UserSlot EnterUser;
        EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        EnterUser.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        ui_battlezone2_manager_->AddPlayer(EnterUser);

        UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(EnterUser.GetPlayerKey());
        if (user_slot != NULL)
        {
            if( user_slot->GetBattleZonePlayerKey() == 0 )
            {
                DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
                Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
                if (player != NULL)
                {
                    player->SetShow(FALSE);
                    user_slot->SetBattleZonePlayerKey(battlezone_object_key);
                }
            }
        }

        MSG_CG_PARTY_REQUEST_MEMBER_INFO_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        return true;
    }
    return false;
}

void uiBattlezone2Lobby::OnRenderSolarDialog()
{
    // 그대로 두고 아니면 함수로 빼자.
    float fMeshHeight	= 1.f;
    float fMeshWidth	= 1.f;
    RECT rcSize;			// 보여줄 위치 정보
    RECT rcViewport;		// view port rect

    // 현재 BoundingBox를 구할수없어 캐릭터의 키를 구할수없다.
    // 캐릭터의 키를 하드코딩한다.
    if(!GetManager())
    {
        return;
    }

    uiPartyMan* party_man =
        static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    TCHAR level_string[32] = {0,};
    uicPartyMemberInfoPlus* party_member = NULL;

    if (party_man && party_man->GetPartyNum() != ui_battlezone2_manager_->GetUserSlotNum())
    {
        ResetLobby();
    }

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    // 모두분배 아이콘 출력
    if (is_party_obtain_room_ == true)
    {
        CControlWZ *mission_name_control = GetControlWZ_byPosition(kControl_Text_T007); //미션명
        if (mission_name_control)
        {
            RECT rect = mission_name_control->GetSizeRect();
            const int party_obtain_icon_size = 16;

            rect.right = rect.left + party_obtain_icon_size;
            rect.bottom = rect.top + party_obtain_icon_size;

            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PARTY_OBTAIN_ROOM,
                &rect);
        }
    }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    int is_master_slot_number = 0;
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    for (int i = 0; i < MAX_PLAYER_SLOT; ++i)
    {
        UserSlot* user_slot = ui_battlezone2_manager_->GetUserSlotByIndex(i);
        if(!user_slot)
        {
            FASSERT(0);
            continue;
        }

        party_member = NULL;//party_man이 NULL일 경우를 대비
        if (party_man)
        {
            party_member = party_man->GetPartyMemberInfo(user_slot->GetPlayerKey());
        }

        Player* player = g_ObjectManager.GetPlayer(user_slot->GetBattleZonePlayerKey());

        // TODO 일단 파티에 종속적이게 파티장 마크를 계속 갱신하자, 추후에 슬롯 전체를 파티로 넘기면, 파티장 변경시에만 적용하도록 하자.(11.11.02)
        if (party_man && user_slot && control_master_mark_[i])
        {
            ENUM_STATEWZ is_state = (party_man->GetMasterKey() == user_slot->GetPlayerKey())?WZ_SHOW:WZ_HIDE;
            control_master_mark_[i]->ShowWindowWZ(is_state);
            if (is_state == WZ_SHOW)
            {
                is_master_slot_number = i;
            }
        }
#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
#else
        if (g_pHero && user_slot && control_highlight_user_[i])//내가 있는 슬롯만 on
        {
            ENUM_STATEWZ is_state = (g_pHero->GetObjectKey() == user_slot->GetPlayerKey())?WZ_SHOW:WZ_HIDE;
            control_highlight_user_[i]->ShowWindowWZ(is_state);
        }
#endif//_DH_BATTLEZONE2_LOBBY_SLOT


        bool is_in_battlezone = false;
        if (party_member)
        {
            const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->FindData((MAPCODE)party_member->map_code());
            if (find_info != NULL)
            {
                is_in_battlezone = true;
            }
        }

        if (control_playing_user_)
        {
            if (user_slot && user_slot->GetPlayerKey() != 0 &&
                ((player == NULL)
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                || (party_member && party_member->IsConnected() == false) //비정상 접속 종료면
                || (is_in_battlezone == true) // 미션 안이면
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                ))
            {
                if (party_man && party_member)
                {
                    control_user_names_[i]->SetTextWZ(party_member->Get().m_szCharName);
                    sprintf(level_string, "%d", party_member->Get().m_wLevel);
                    control_user_levels_[i]->SetTextWZ(level_string);
                }

                if (user_slot->GetPlayerKey() == g_pHero->GetObjectKey())
                {
                    UserSlot* find_user_slot = ui_battlezone2_manager_->FindPlayer(user_slot->GetPlayerKey());
                    if (find_user_slot != NULL)
                    {
                        if( find_user_slot->GetBattleZonePlayerKey() == 0 )
                        {
                            DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
                            Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
                            if (player != NULL)
                            {
                                player->SetShow(FALSE);
                                find_user_slot->SetBattleZonePlayerKey(battlezone_object_key);
                            }
                        }
                    }
                }
                else
                {
                    control_playing_user_[i]->ShowWindowWZ(WZ_SHOW);
                    if (party_member && g_pMap &&
                        ((g_pMap->IsField() || g_pMap->IsVill())
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        && (party_member->IsConnected() == true)// 접속중인 유저면
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        ))
                    {
                        if (user_slot && (is_in_battlezone == false)) // 미션이 아닐때
                        {
                            //랜더정보 요청
                            SendRequestRenderInfo(party_man->GetPartyKey(), user_slot->GetPlayerKey());
                        }
                    }
                }
            }
            else
            {
                control_playing_user_[i]->ShowWindowWZ(WZ_HIDE);
                control_user_names_[i]->SetTextWZ("");
                control_user_levels_[i]->SetTextWZ("");
            }
        }

        if (player == NULL)
        {
            if (user_slot->GetBattleZonePlayerKey())//키가 널이 아니면
            {
                if (SendRequestMember())
                {
                    return;
                }
            }

            if (i > 0 && control_close_slots_[i-1])
            {
                if (user_slot->GetSlotState() == UserSlot::BLOCK)
                {
                    control_close_slots_[i-1]->SetCheckState(true);
                }
                else
                {
                    control_close_slots_[i-1]->SetCheckState(false);
                }

                if (GetManager()->IsMaster() && control_ban_[i])
                {
                    if (user_slot->GetSlotState() == UserSlot::BLOCK)
                    {
                        control_ban_[i]->ShowWindowWZ(WZ_HIDE);
                    }
                    else
                    {
                        control_ban_[i]->ShowWindowWZ(WZ_SHOW);
                    }
                }
            }
            continue;  
        }

        FPOINT org;
        
        // Calc ViewPortWithRect
        if (!control_user_slots_[i])
        {
            assert(!"uiBattlezone2Lobby:check need to 61_4_BattleZone_ready_new.iwz");
            continue;
        }

        rcSize = control_user_slots_[i]->GetSizeRect();
        rcViewport = g_InterfaceManager.CalcuateViewPortWithRect(rcSize);

        if (user_slot && user_slot->GetTeam() == 0)
        {
            player->SetShow(TRUE);
            // 팀이 없다.
            control_user_names_[i]->SetTextWZ(player->GetName());
            sprintf(level_string, "%d", player->GetLevel());
            control_user_levels_[i]->SetTextWZ(level_string);
            player->SetShow(FALSE);
        }
        else
        {
            WzColor wzColor = 0xFFFF6FFF;	//	붉게

            int team = 0;
            if (user_slot)
            {
                team = user_slot->GetTeam();
            }
            switch (team)
            {
            case 1:
                {
                    wzColor = WzColor_RGBA(0, 50, 121, 100);	//	청
                }
                break;
            case 2:
                {
                    wzColor = WzColor_RGBA(137, 9, 0, 100);	//	붉
                }
                break;
            case 3:
                {
                    wzColor = 0xFF8F8F8F;	//	노랑
                }
                break;
            default:
                break;
            }

            g_InterfaceManager.RenderRect(rcSize, wzColor);
        }

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        // 비접속중이거나 미션 안이면 캐릭터를 그리지 않음
        if (party_member &&
            ((party_member->IsConnected() == false) ||
            (is_in_battlezone == true)))
        {
            continue;
        }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        
        player->SetShow(TRUE);
        g_InterfaceManager.RenderPlayerOnRect(player, control_user_slots_[i]->GetSizeRect(), PI, org, 1);
        player->SetShow(FALSE);

        //switch (user_slot->GetSlotState())

        if (party_member && party_member->is_ready())
        {
            //// 여기다가 Ready Texture를 가운데에 찍는다.
            if (handle_texture_[USE_TEXTURE_READY] != INVALID_HANDLE_VALUE) 
            {
                float x = (float)(rcSize.left + (rcSize.right - rcSize.left)/2 - 48.5f);
                float y = (float)(rcSize.top+ (rcSize.bottom - rcSize.top)/2 - 14.0f);

                g_pSunRenderer->RenderTexture( 
                    handle_texture_[USE_TEXTURE_READY],
                    x,
                    y,
                    97.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                    28.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                    894.0f,//x,
                    206.0f,//y,
                    97.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                    28.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                    NULL,
                    0.0f
                    );
            }
        }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        float class_x = (float)(rcSize.left + (rcSize.right - rcSize.left)/2 - 30.0f);
        float class_y = (float)(rcSize.bottom);

        eCHAR_TYPE class_type    = player->GetClass();
        GenderType::Value gender = player->GetGender();
        eCHAR_TYPE race_type = ResourceCode::GetImageClassType(class_type ,gender);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // 이미지 위치가 순서대로 되어있지 않아 하드코딩
        if ( class_type == eCHAR_WITCHBLADE)
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_[USE_TEXTURE_READY],
                class_x,
                class_y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                730.0f + ((eCHAR_MYSTIC-1) * 14),//x,
                297.0f + 28.0f,//y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                NULL,
                0.0f
                );
        }
        else
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_[USE_TEXTURE_READY],
                class_x,
                class_y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                730.0f + ((race_type-1) * 14),//x,
                297.0f,//y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                NULL,
                0.0f
                );
        }
#else
        g_pSunRenderer->RenderTexture( 
            handle_texture_[USE_TEXTURE_READY],
            class_x,
            class_y,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
            730.0f + ((race_type-1) * 14),//x,
            297.0f,//y,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
            NULL,
            0.0f
            );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if (tooltip_manager && IsInRect(class_x, class_y, 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetRaceString(race_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );

        }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if ( class_type == eCHAR_WITCHBLADE )
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_[USE_TEXTURE_READY],
                class_x + 14.0f,
                class_y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                730.0f + ((eCHAR_HELLROID-1) * 14),//x,
                297.0f + 28.0f,//y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                NULL,
                0.0f
                );
        }
        else
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_[USE_TEXTURE_READY],
                class_x + 14.0f,
                class_y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                730.0f + ((class_type-1) * 14),//x,
                297.0f + 14.0f,//y,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                NULL,
                0.0f
                );
        }
#else
        g_pSunRenderer->RenderTexture( 
            handle_texture_[USE_TEXTURE_READY],
            class_x + 14.0f,
            class_y,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
            730.0f + ((class_type-1) * 14),//x,
            297.0f + 14.0f,//y,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
            14.0f,//(float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
            NULL,
            0.0f
            );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if (tooltip_manager && IsInRect(class_x + 14.0f, class_y, 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetClassString(class_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
/*
        switch (1)
        {
        case UserSlot::FILL:
            {
            }
            break;

        case UserSlot::READY:
            {
                //// 여기다가 Ready Texture를 가운데에 찍는다.
                if (handle_texture_[USE_TEXTURE_READY] != INVALID_HANDLE_VALUE) 
                {
                    float x = (float)((rcSize.left+rcSize.right) - texture_information_[USE_TEXTURE_READY]->m_nImageWidth)/2;
                    float y = (float)(rcSize.top + rcSize.bottom + texture_information_[USE_TEXTURE_READY]->m_nImageHeight) /2.f;

                    g_pSunRenderer->RenderTexture( 
                        handle_texture_[USE_TEXTURE_READY],
                        x,
                        y,
                        (float)texture_information_[USE_TEXTURE_READY]->m_nImageWidth,
                        (float)texture_information_[USE_TEXTURE_READY]->m_nImageHeight,
                        0.0f,
                        0.0f,
                        0.0f,
                        0.0f,
                        NULL,
                        0.0f
                        );
                }
            }
            break;

        case UserSlot::OPENED:
        case UserSlot::BLOCK:
            break;

        default:
            {	
                assert(0);
            }
            break;
        }*/
    }

    CControlWZ* ready_control = control_master_mark_[is_master_slot_number];
    if (ready_control) 
    {
        RECT rect = ready_control->GetSizeRect();
        SI_TFORMAT* image_format = ready_control->GetCtrlImageFormat();
        if (ready_control != INVALID_HANDLE_VALUE)
        {
            g_pSunRenderer->RenderTexture( 
                image_format->m_sImageState[E_BASE].m_pVoid,
                (float)rect.left,
                (float)rect.top,
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fWidth, 
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fHeight,
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fTopX,
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fTopY, 
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fWidth, 
                (float)image_format->m_sImageState[E_BASE].m_sFRect.fHeight);
        }
    }
}

void uiBattlezone2Lobby::MouseOver_MsgProc(SI_MESSAGE* message)
{

}

void uiBattlezone2Lobby::OnShowWindow(BOOL is_show)
{
    RECT control_rect;
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (is_show)
    {
        if (this->IsVisible() && battlezone2_list && battlezone2_list->GetDialogWZ())
        {
            battlezone2_list->GetDialogWZ()->GetClientRect(&control_rect);
            this->MoveWindowWZ(control_rect.left, control_rect.top);
        }
        ChangeSetting();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Battlezone2Lobby_Dialog;
        msg.wParam = IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        ResetLobby();
        GetManager()->UnsetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_RENDER);//창을 열면 일단 풀어주자.
    }
    else
    {
        if (!this->IsVisible() && battlezone2_list && battlezone2_list->GetDialogWZ())
        {
            this->GetDialogWZ()->GetClientRect(&control_rect);
            battlezone2_list->MoveWindowWZ(control_rect.left, control_rect.top);
        }

        if (GetManager())
        {
            GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_TREE, false);
            GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_RESERVATION, false);
        }
        g_KeyQueueManager.DeleteMsg(eDoType_Battlezone2Lobby_Dialog);
    }
}

void uiBattlezone2Lobby::Process(DWORD tick_count)
{
    if (wait_timer_.Check() != TIMER_STAT_NONE)
    {
        if (wait_timer_.Check() == TIMER_STAT_FINISH)
        {
            WaitGague(false);
        }
        else
        {
            CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
            if (cast_dialog)
            {
                cast_dialog->SetGage((wait_timer_.SpentTime()/50.0f));
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiBattlezone2Lobby::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    /*if (GetManager() == NULL)
        return;*/

    if (GetManager()->IsMaster())
    {
        bool is_change = false;
        if (dropdown_item_share().MessageProc(message, OUT is_change))
        {
            if (is_change)
            {
                BYTE select_type = static_cast<BYTE>(dropdown_item_share().CurrentUserData());

                MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN message;
                message.m_byItemDistributionType = select_type;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &message, sizeof(message));
            }
            return;
        }
    }
    else
    {
        if (message->eResultMsg == RT_MSG_LBUTTONCLICK)
        {
            switch (GetControlPosition_byID(message->dwCtrlID))
            {
            case kControl_Button_B005:
            case kControl_List_L000:
            case kControl_Button_B007:
                //파티장만 변경 할 수 있다
                if (GetManager()->IsMaster() == FALSE)
                {
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
                    //5334	파티장이 아닙니다.
                    g_InterfaceManager.GetInterfaceString(5334, szMessage,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                return;
            default:
                break;
            }
        }
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiBattlezone2Lobby::SendGameStart()
{
    if (GetManager()->IsReadyForStart())//시작 가능한지 체크, 모두 레디인가?
    {
        //GetManager()->StartBattle();
        MSG_CG_PARTY_ROOM_START_SYN send_packet;

        PartyRoomInfo* party_room_info = GetManager()->current_room_info();
        PartyZoneType type = PartyOfNone;
        if (party_room_info)
        {
            type = party_room_info->base_party_room_info.party_room_type_;
        }
        send_packet.require_item_ = 0;
        uiBattlezone2Tree* battlezone2_tree = GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
        const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->FindData(static_cast<MAPCODE>(map_code_), type);
        if (battlezone2_tree && find_info)        
        {
            send_packet.require_item_ = static_cast<SLOTCODE>(battlezone2_tree->GetRequireItem(find_info));
        }
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
    else
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(70243, message, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.ShowMessageBox(message, 10000, NULL, 0);
        return false;
    }
    return true;
}

bool uiBattlezone2Lobby::SendGameReady()
{
    //is_ready_ = !is_ready_;
    uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

    if (partyMan && g_pHero)
    {
        uicPartyMemberInfoPlus* party_member = partyMan->GetPartyMemberInfo(g_pHero->GetObjectKey());
        if (party_member)
        {
            is_ready_ = !party_member->is_ready();
        }            
        MSG_CG_PARTY_READY_STATE_SYN send_packet;
        send_packet.ready_state_ = is_ready_;//내 현재 상태를 기준으로 변경해서 보내줘야한다.
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

        return true;
    }
    return false;    
}

bool uiBattlezone2Lobby::SendGameExit()
{
    MSG_CG_PARTY_LEAVE_ROOM_SYN send_packet;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

    return true;
}

void uiBattlezone2Lobby::SetShareType(BYTE share_type)
{
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    const int share_type_max = eITEM_DISTRIBUTION_MAX;
#else
    const int share_type_max = eITEM_DISTRIBUTION_MAX_DICE_BEFORE;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    if (share_type < share_type_max)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
        g_InterfaceManager.GetInterfaceString(
            ItemShareSystem::distribution_type_string_code[share_type],
            message,
            INTERFACE_STRING_LENGTH);

        //CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S002));
        CControlWZ* text_control = dropdown_item_share().text_control();
        if (text_control != NULL)
        {
            text_control->SetTextWZ(message);
        }
    }
}

bool uiBattlezone2Lobby::SendChangeTeam()
{
    if (GetManager()->SafePKBool(uiBattlezone2Man::PACKET_STATUS_SEND_CHANGE_TEAM))
    {
        GetManager()->SetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_CHANGE_TEAM);
        uiPartyMan* partyMan =
            static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        /*DWORD master_key = partyMan->GetMasterKey();
        if (ui_battlezone2_manager_ && partyMan)
        {
            ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());
        }
        else
        {
            assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_JOIN_ROOM_ACK): Can't not find master key");
        }

        if (master_key != g_pHero->GetObjectKey())
        {
            UserSlot EnterUser;
            EnterUser.SetPlayerKey(master_key);
            ui_battlezone2_manager_->AddPlayer(EnterUser);
        }
        UserSlot EnterUser;
        EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
        ui_battlezone2_manager_->AddPlayer(EnterUser);*/

        UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(g_pHero->GetObjectKey());
        if (user_slot != NULL)
        {
            MSG_CG_PARTY_SELECT_TEAM_SYN send_packet;
            send_packet.select_team_ = static_cast<BYTE>(user_slot->GetTeam()==1)?2:1;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool uiBattlezone2Lobby::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004: //닫기
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B000://게임시작/레디
        {
            if (GetManager()->IsMaster())//파티장이면
            {
                if (GetManager()->IsReadyForStart() == true)
                {
                    if (GetManager()->StartBattle() == true)
                    {
                        SendGameStart();
                    }
                }
                else
                {
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                    g_InterfaceManager.GetInterfaceString(70243, message, INTERFACE_STRING_LENGTH);
                    g_InterfaceManager.ShowMessageBox(message, 10000, NULL, 0);
                }
            }
            else
            {
                SendGameReady();
            }
            
        }
        break;
    case kControl_Button_B010://방나가기
        {
            SendGameExit();
        }
        break;
    case kControl_Button_B006://팀바꾸기
        {
            SendChangeTeam();
        }
        break;
    case kControl_Button_B001:
        { // 방장은 강퇴 안됨
        }
        break;
    case kControl_Button_B025:
    case kControl_Button_B026:
    case kControl_Button_B027:
    case kControl_Button_B028:
    case kControl_Button_B029:
    case kControl_Button_B030:
    case kControl_Button_B031:
    case kControl_Button_B032:
    case kControl_Button_B033:
        {
            BlockPlayer(GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B001);
        }
        break;
    case kControl_Button_B002:
        { // 방장은 강퇴 안됨
        }
        break;
    case kControl_Button_B034:
    case kControl_Button_B035:
    case kControl_Button_B036:
    case kControl_Button_B037:
    case kControl_Button_B038:
    case kControl_Button_B039:
    case kControl_Button_B040:
    case kControl_Button_B041:
    case kControl_Button_B042:
        {
            int index = GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B002;
            UserSlot* user_slot = GetManager()->GetUserSlotByIndex(index);
            bool is_party = false;

            if (user_slot)
            {
                Player* player = g_ObjectManager.GetPlayer(user_slot->GetBattleZonePlayerKey());
                uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                if (partyMan->IsPartyMember(user_slot->GetPlayerKey()))
                {
                    is_party = true;
                }

                if (player || is_party)
                {
                    //  플레이어가 있으면 강퇴
                    BanPlayerByIndex(GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B002);
                }
                else
                {
                    BlockPlayer(GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B002);
                }
            }
        }
        break;
    case kControl_Button_B003:
        {
            uiBattlezone2List* battlezone_list_dialog = 
                GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
            if (battlezone_list_dialog)
            {
                battlezone_list_dialog->ShowBattlezoneTree(uiBattlezone2Tree::kModificationMode);
            }
        }
        break;
    /*case kControl_Text_S002:
        {
        }
        break;*/
//     case kControl_CheckBT_C001:
//     case kControl_CheckBT_C002:
//     case kControl_CheckBT_C004:
//     case kControl_CheckBT_C003:
//     case kControl_CheckBT_C005:
//     case kControl_CheckBT_C006:
//     case kControl_CheckBT_C007:
//     case kControl_CheckBT_C008:
//     case kControl_CheckBT_C009:
//         {
//             if (GetManager()->IsMaster())
//             {
//                 BlockPlayer(GetControlPosition_byID(message->dwCtrlID) - kControl_CheckBT_C001);
//             }
//             else
//             {
//                 CCtrlButtonCheckWZ* control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(message->dwCtrlID));
//                 if (control)
//                 {
//                     if (control->GetCheckState())
//                     {
//                         control->SetCheckState(false);
//                     }
//                     else
//                     {
//                         control->SetCheckState(true);
//                     }
//                 }
//             }
//         }
//         break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiBattlezone2Lobby::SetBaseInfo(PartyRoomInfo* mission_infomation)
{
    TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
    CCtrlStaticWZ* control = NULL;

    if (mission_infomation == NULL)
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("uiBattlezone2Lobby::SetBaseInfo(base_mission_infomation is null)"));
#endif
        return;
    }

    control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T001));//방번호
    if (control)
    {
        sprintf(buffer_string, "%d", (mission_infomation->room_key_ - BASE_ROOM_KEY));
        control->SetTextWZ(buffer_string);
    }

    map_code_ = mission_infomation->base_party_room_info.map_code_;
    const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->FindData(mission_infomation->base_party_room_info.map_code_);
    if (find_info)
    {
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T007));//미션명
        if (control)
        {
            control->SetTextID(find_info->mission_string_code);
        }

        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T009));//레벨제한
        if (control)
        {
            const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)find_info->map_code);
            if(map_info != NULL)
            {
                sprintf(buffer_string, "%d-%d", map_info->minLV, map_info->maxLV);
                control->SetTextWZ(buffer_string);
            }
            else
            {
                sprintf(buffer_string, "All");
                control->SetTextWZ(buffer_string);
            }
        }

        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T011));//권장레벨
        if (control)
        {
            sprintf(buffer_string, "%d-%d", find_info->min_level, find_info->max_level);
            control->SetTextWZ(buffer_string);
        }

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        if (find_info->item_obtain_ == kObtainType_Enable)
        {
            is_party_obtain_room_ = true;
        }
        else
        {
            is_party_obtain_room_ = false;
        }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    }
    control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T003));//비밀번호
    if (control)
    {
        sprintf(buffer_string, "%s", mission_infomation->base_party_room_info.party_room_password_);
        control->SetTextWZ(buffer_string);
    }
    control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T002));//방이름
    if (control)
    {
        control->SetTextWZ(mission_infomation->base_party_room_info.party_room_title_);
    }

    GetManager()->SetNonBlockSlot(mission_infomation->max_user_num_);
    
    CControlWZ* control_button = GetControlWZ_byPosition(kControl_Button_B006); //팀바꾸기
    if (mission_infomation->base_party_room_info.party_room_type_ == PartyOfPvPByTeam)
    {
        if (control_button)
        {
            //control_button->ShowWindowWZ(WZ_SHOW);
            control_button->EnableWindowWZ(true);
        }
    }
    else
    {
        if (control_button)
        {
            //control_button->ShowWindowWZ(WZ_HIDE);
            control_button->EnableWindowWZ(false);
        }
    }
    //base_mission_infomation->special_type_;
}

void uiBattlezone2Lobby::ChangeMaster(DWORD master_key)
{
    if (ui_battlezone2_manager_ == NULL)
    {
        return;
    }
    uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

    DWORD old_master_key = 0;
    if (partyMan)
    {
        old_master_key = partyMan->GetMasterKey();
    }
    else
    {
        assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_ALLPLAYERS_CMD): Can't not find master key");
        return;
    }

    ui_battlezone2_manager_->SetLobbyMasterPlayerKey(master_key);
#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
#else
    ui_battlezone2_manager_->SwapUserSlot(old_master_key, master_key);
#endif//_DH_BATTLEZONE2_LOBBY_SLOT
    //test 임시 코드
    //GetManager()->SetCancleAllReady();
    //is_ready_ = false;
    //test 임시 코드

    ChangeSetting();
}

void uiBattlezone2Lobby::WaitGague(bool is_show)
{
    CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
    if (is_show)
    {
        wait_timer_.On(WAIT_TIME);
        if (cast_dialog)
        {
            g_InterfaceManager.HideAllWindows();
            g_InterfaceManager.ShowGameInterface(TRUE);

            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            //5116	잠시만 기다려 주십시오.
            g_InterfaceManager.GetInterfaceString(5116, message);
            cast_dialog->SetText(message);
            cast_dialog->ShowWindow(TRUE);
        }
        if (ui_battlezone2_manager_)
        {
            ui_battlezone2_manager_->set_is_wait(true);
        }
        g_InterfaceManager.LockDlgsAll();
    }
    else
    {
        wait_timer_.Off();
        if (cast_dialog)
        {
            cast_dialog->ShowWindow(FALSE);
            cast_dialog->SetText("");
        }
        /*if (ui_battlezone2_manager_)
        {
            ui_battlezone2_manager_->set_is_wait(false);
        }
        g_InterfaceManager.UnLockDlgs();*/
    }
}

void uiBattlezone2Lobby::Unlocks()
{
    if (ui_battlezone2_manager_)
    {
        ui_battlezone2_manager_->set_is_wait(false);
    }
    g_InterfaceManager.UnLockDlgs();
}

void uiBattlezone2Lobby::NetworkProc(MSG_BASE* message_ptr)
{
    //배틀존 패킷만 처리한다
    if (message_ptr->m_byCategory != CG_PARTY)
    {
        return;
    }

    switch (message_ptr->m_byProtocol)
    {
    case CG_PARTY_START_ROOM_COUNT_BRD:
        {
            WaitGague(true);
        }
        break;
    case CG_PARTY_READY_STATE_BRD:      
        {
            MSG_CG_PARTY_READY_STATE_BRD* recv_packet = (MSG_CG_PARTY_READY_STATE_BRD*)message_ptr;				
            if (recv_packet)
            {

                GetManager()->SetReadyUserSlot(recv_packet->member_key_, recv_packet->ready_state_);
            }
            else
            {
                assert("CG_PARTY_READY_STATE_BRD: packet is null");
            }
        }
        break;
    case CG_PARTY_ALLPLAYERS_CMD:
        {
            MSG_CG_PARTY_ALLPLAYERS_CMD* recv_packet = (MSG_CG_PARTY_ALLPLAYERS_CMD*)message_ptr;

            BYTE* pPacket = ( (BYTE*)(recv_packet) + sizeof(MSG_CG_PARTY_ALLPLAYERS_CMD) );

            PLAYER_RENDER_INFO_EX* pRenderInfo = NULL;

            ETC_STATE_TOTAL_INFO* pShield = NULL;

           //is_ready_ = false;

            int max_count = 0;
            if (recv_packet)
            {
                max_count = recv_packet->m_byCount;
            }
            uiPartyMan* partyMan =
                static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

            for (int i = 0; i < max_count; i++)
            {
                pRenderInfo = (PLAYER_RENDER_INFO_EX *)pPacket; 
                PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
                int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);
                pShield = (ETC_STATE_TOTAL_INFO *)( (BYTE*)(pPacket) + nSize );
                //pVendorInfo = (VENDOR_RENDER_INFO*)( (BYTE*)(pShield) + pShield->GetSize() );

                if (ui_battlezone2_manager_)
                {
                    uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                    DWORD master_key = partyMan->GetMasterKey();
                    if (ui_battlezone2_manager_ && partyMan)
                    {
                        ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());
                    }
                    else
                    {
                        assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_ALLPLAYERS_CMD): Can't not find master key");
                        return;
                    }

                    UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(output.player_key);
                    if (user_slot == NULL)
                    {
                        UserSlot EnterUser;
                        EnterUser.SetPlayerKey(output.player_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        EnterUser.SetPlayerName(output.char_name);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        ui_battlezone2_manager_->AddPlayer(EnterUser);
                        user_slot = ui_battlezone2_manager_->FindPlayer(output.player_key);
                    }

                    if ((user_slot != NULL) && (user_slot->GetBattleZonePlayerKey() <= 0))
                    {
                        Player* player = GameFunc::MakeBattleZonePlayer(output, pShield, NULL, NULL, NULL);
                        if (player != NULL)
                        {
                            player->SetShow(FALSE);
                            user_slot->SetBattleZonePlayerKey(player->GetObjectKey());
                        }
                        
                        if (partyMan)
                        {
                            uicPartyMemberInfoPlus* party_member = partyMan->GetPartyMemberInfo(output.player_key);
                            if (party_member)
                            {
                                if (party_member->is_ready())
                                {
                                    user_slot->SetSlotState(UserSlot::READY);
                                }
                            }
                        }
                    }
                }
                pPacket += (nSize + pShield->GetSize());
                //pPacket += GetPlayerSize(pPacket,nSize);
                GetManager()->UnsetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_MEMBER);
            }
        }
        break;
    case CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD:
        {
            MSG_CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD* recv_packet = (MSG_CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD*)message_ptr;

            // 첫번째 장착정보의 위치를 셋팅한다.
            EQUIP_ITEM_BASEINFO *pEquipInfo = (EQUIP_ITEM_BASEINFO*)( (BYTE*)recv_packet + sizeof(MSG_CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD) );

            int max_count = 0;
            if (recv_packet)
            {
                max_count = recv_packet->m_byCount;
            }
            for (int i = 0; i < max_count; i++)
            {
                // 아이템 슬롯의 위치를 셋팅한다.
                EQUIP_ITEM_BASEINFO::data_type* pItemSlot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)pEquipInfo + sizeof(EQUIP_ITEM_BASEINFO));

                UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(pEquipInfo->m_dwObjectKey);
                if (user_slot != NULL)
                {
                    Player* player = (Player*)g_ObjectManager.GetObject(user_slot->GetBattleZonePlayerKey());
                    if (player)
                    {
                        player->SetEquipItemBaseInfo(pEquipInfo->m_Count, pItemSlot);
                        player->RefreshAppearRender();
                    }
                }

                // 다음 장착정보의 위치를 셋팅한다.
                pEquipInfo = (EQUIP_ITEM_BASEINFO*)((BYTE*)pItemSlot + pEquipInfo->m_Count * sizeof(EQUIP_ITEM_BASEINFO::data_type));
            }
        }
        break;
    case CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD:
        {
            MSG_CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD* recv_packet = (MSG_CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD*)message_ptr;

            int iPlayerCnt = 0;
            if (recv_packet)
            {
                iPlayerCnt = recv_packet->m_Count;
            }
            sMSG_RENDER_EFFECT_ITEM_LIST* pOffsetOfList = (sMSG_RENDER_EFFECT_ITEM_LIST*)( ((BYTE*)recv_packet) + recv_packet->GetSize() );
            for(int i = 0; i < iPlayerCnt; i++)
            {
                sMSG_RENDER_EFFECT_ITEM_LIST* pRender_effect_List = pOffsetOfList;

                UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(pRender_effect_List->m_PlayerKey);
                if (user_slot != NULL)
                {
                    if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(user_slot->GetBattleZonePlayerKey()) )
                    {
                        int iItemCnt = pRender_effect_List->m_ItemCount;
                        pPlayer->RefreshRenderEffectItem(pPlayer, &pRender_effect_List->m_ItemCodes[0], iItemCnt);
                    }
                }
                pOffsetOfList = (sMSG_RENDER_EFFECT_ITEM_LIST*)( ((BYTE*)pOffsetOfList) + pOffsetOfList->GetSize() );
            }
        }
        break;
    case CG_PARTY_JOIN_ROOM_BRD:
        {
            MSG_CG_PARTY_JOIN_ROOM_BRD* recv_packet = (MSG_CG_PARTY_JOIN_ROOM_BRD*)message_ptr;

            GetManager()->UnsetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_REQUEST_RENDER);

            if (GetManager() && GetManager()->battlezone_room_state() == uiBattlezone2Man::ROOM_STATE_LIST)
            {
                ShowInterface(TRUE);
                GetManager()->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
                GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LIST, false);
                GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_RESERVATION, false);
                GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_TREE, false);
            }

            PLAYER_RENDER_INFO_EX* pRenderInfo = (PLAYER_RENDER_INFO_EX*)( (BYTE*)(recv_packet) + sizeof(MSG_CG_PARTY_JOIN_ROOM_BRD) );
            PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
            int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

            //쉴드정보가 이중간에 들어온다
            ETC_STATE_TOTAL_INFO *pShield = (ETC_STATE_TOTAL_INFO*)((BYTE*)(pRenderInfo) + nSize);

            //이큅정보
            EQUIP_ITEM_BASEINFO* pEquipInfo = (EQUIP_ITEM_BASEINFO*)((BYTE*)(pShield) + pShield->GetSize());

            //이펙트 정보
            sMSG_RENDER_EFFECT_ITEM_LIST* effect_item_info = (sMSG_RENDER_EFFECT_ITEM_LIST*)((BYTE*)(pEquipInfo) + pEquipInfo->GetSize());

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            // 들어오는 사람과 같은 이름이 존재하면 삭제
            ui_battlezone2_manager_->DeleteDisConnectedUser(output.char_name);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

            // 버그수정 : 중복+엇갈린 패킷으로 파티를 떠난 유저가 슬롯에 삽입되는 현상 수정
            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            uicPartyMemberInfoPlus* party_member = NULL;
            if (party_manager)
            {
                party_member = party_manager->GetPartyMemberInfo(output.player_key);
                if (party_member == NULL)
                {
                    return;
                }
            }

            UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(output.player_key);
            if (user_slot == NULL)
            {
                UserSlot EnterUser;
                EnterUser.SetPlayerKey(output.player_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                EnterUser.SetPlayerName(output.char_name);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                ui_battlezone2_manager_->AddPlayer(EnterUser);
                user_slot = ui_battlezone2_manager_->FindPlayer(output.player_key);
            }
            if (user_slot != NULL)
            {
                if (user_slot->GetBattleZonePlayerKey() <= 0)
                {
                    Player* player = GameFunc::MakeBattleZonePlayer(output, pShield, NULL, pEquipInfo, effect_item_info);
                    if (player != NULL)
                    {
                        player->SetShow(FALSE);
                        player->SetAnimation("N002", TRUE);
                        user_slot->SetBattleZonePlayerKey(player->GetObjectKey());
                    }
                }

                if (party_member->is_ready())
                {
                    user_slot->SetSlotState(UserSlot::READY);
                }
            }
        }
        break;
    case CG_PARTY_LEAVE_ROOM_ACK:
        {
            MSG_CG_PARTY_LEAVE_ROOM_ACK* recv_packet = (MSG_CG_PARTY_LEAVE_ROOM_ACK*)message_ptr;
            is_ready_ = false;

            if (recv_packet && recv_packet->result_value_ != RC::RC_PARTY_SUCCESS)
            {
#ifdef _DEV_VER 
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "CG_PARTY_LEAVE_ROOM_ACK:result code(%d)", recv_packet->result_value_);
#endif //_DEV_VER
                if (g_pHero && !g_pHero->IsParty())//실패지만 파티가 아니면 나가자
                {
                    LeaveHero();
                }
                return;
            }
            //BattleScene::SetLobbyFlag(FALSE);
            LeaveHero();
        }
        break;
    case CG_PARTY_ROOM_START_ACK:
        {
            MSG_CG_PARTY_ROOM_START_ACK* recv_packet = (MSG_CG_PARTY_ROOM_START_ACK*)message_ptr;
            if (recv_packet && recv_packet->result_code_ != RC::RC_PARTY_SUCCESS)
            {
                uiPartyMan* party_manager =
                    static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                if (party_manager)
                {
                    party_manager->Reason(recv_packet->result_code_);
                }
                ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
                ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_ICON, true);
                ui_battlezone2_manager_->set_is_wait(false);
                if (g_InterfaceManager.IsLockDlg())
                {
                    g_InterfaceManager.UnLockDlgs();
                }
                if (GetManager() && GetManager()->IsMaster())
                {
                    SendGameReady();//방장인데 게임 시작 실패시 레디를 푼다.
                }
#ifdef _DEV_VER 
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "CG_PARTY_ROOM_START_ACK:result code(%d)", recv_packet->result_code_);
#endif //_DEV_VER
            }
        }
        break;
    case CG_PARTY_LEAVE_ROOM_BRD:
        {
            MSG_CG_PARTY_LEAVE_ROOM_BRD* recv_packet = (MSG_CG_PARTY_LEAVE_ROOM_BRD*)message_ptr;

            if (ui_battlezone2_manager_)
            {
                DWORD master_key = 0;
                if (recv_packet && ui_battlezone2_manager_ && recv_packet->member_key_ == ui_battlezone2_manager_->master_player_key())
                {
                    /*uiPartyMan* partyMan =
                        static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));*/

                    //ChangeMaster(recv_packet->member_key_);

                    //master_key = partyMan->GetMasterKey();
                    //ui_battlezone2_manager_->SetLobbyMasterPlayerKey(recv_packet->member_key_);

                    //방장이 나가면 방 초기화
                    PartyRoomInfo* party_room_info = GetManager()->current_room_info();
                    if (party_room_info)
                    {
                        party_room_info->base_party_room_info.map_code_ = 0;
                        SetBattleRoom(party_room_info);
                    }
                }
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                if (recv_packet->reason_to_leave_ != eREASON_LEAVE_PARTY_ABNORMAL)
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                {
                    // 비정상 종료가 아니면 유저슬롯에서 제거
                    ui_battlezone2_manager_->LeavePlayer(recv_packet->member_key_);
                }

                if (kick_player_key() == recv_packet->member_key_)
                {
                    GetManager()->UnsetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_BAN);
                }

                if (g_pHero && g_pHero->GetObjectKey() == recv_packet->member_key_)
                {
                    LeaveHero();
                    return;
                }
            }
        }
        break;
    case CG_PARTY_CHANGE_ROOM_OPTION_BRD:
        {
            MSG_CG_PARTY_CHANGE_ROOM_OPTION_BRD* recv_packet = (MSG_CG_PARTY_CHANGE_ROOM_OPTION_BRD*)message_ptr;

            PartyRoomInfo* party_room_info = GetManager()->current_room_info();
            if (party_room_info && recv_packet)
            {
                party_room_info->base_party_room_info = recv_packet->change_party_room_info_;
                SetBattleRoom(party_room_info);
                if (recv_packet->change_party_room_info_.map_code_ == 0)
                {
                    GetManager()->SetCancleAllReady();
                }
                is_ready_ = false;
            }
            //GetManager()->SetCancleAllReady();
        }
        break;
    case CG_PARTY_RESET_READY_STATE_CMD:
        {
            MSG_CG_PARTY_RESET_READY_STATE_CMD* recv_packet = (MSG_CG_PARTY_RESET_READY_STATE_CMD*)message_ptr;
            GetManager()->SetCancleAllReady();
            is_ready_ = false;
        }
        break;
    case CG_PARTY_CREATE_ROOM_ACK:
        {
            MSG_CG_PARTY_CREATE_ROOM_ACK* recv_packet = (MSG_CG_PARTY_CREATE_ROOM_ACK*)message_ptr;
            
            if (recv_packet == NULL)
            {
                return;
            }
            if (recv_packet->result_code_ != RC::RC_PARTY_SUCCESS)
            {
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};

                uiPartyMan* party_manager =
                    static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                if (party_manager)
                {
                    party_manager->Reason(recv_packet->result_code_);
                    uiBattlezone2Tree* battlezone2_tree = GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
                    if (battlezone2_tree)
                    {
                        battlezone2_tree->set_last_create_room(0);
                    }
                    break;
                }
#ifdef _DEV_VER 
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "MSG_CG_PARTY_CREATE_ROOM_ACK:result code(%d)", recv_packet->result_code_);
#endif //_DEV_VER
                return;
            }

            if (ui_battlezone2_manager_)
            {
                ui_battlezone2_manager_->SetLobbyMasterPlayerKey(g_pHero->GetObjectKey());

                UserSlot EnterUser;
                EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                EnterUser.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                ui_battlezone2_manager_->AddPlayer(EnterUser);
                is_ready_ = false;

                UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(EnterUser.GetPlayerKey());
                if (user_slot != NULL)
                {
                    if( user_slot->GetBattleZonePlayerKey() == 0 )
                    {
                        DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
                        Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
                        if (player != NULL)
                        {
                            player->SetShow(FALSE);
                            user_slot->SetBattleZonePlayerKey(battlezone_object_key);
                        }
                    }
                }
            }

            ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
            if (chat_dialog)
            {
                chat_dialog->OnPartyChat();
            }
            ShowInterface(TRUE);
            SetBattleRoom(&recv_packet->party_room_info_);

            is_ready_ = false;
        }
        break;
    case CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK:
        {
            MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK* recv_message = (MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK*)message_ptr;
            GetManager()->SetBlockUserSlot();
        }
        break;
    case CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD:
        {
            //if(BattleScene::IsLobby())
            {
                // 마스터가 아닐때만 의미가 있다.
                // 최고 인원수에 따라 뒤에서부터 블럭해나간다.
                if (GetManager()->IsMaster())
                {
                    return;
                }
                MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD* recv_message = (MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD*)message_ptr;
                GetManager()->SetNonBlockSlot(recv_message->max_count_);
                GetManager()->set_current_room_max_user_num(recv_message->max_count_);

            }
        }
        break;
    case CG_PARTY_PLAYER_MOVE_FIELD_BRD:
        {
            MSG_CG_PARTY_PLAYER_MOVE_FIELD_BRD* recv_message = (MSG_CG_PARTY_PLAYER_MOVE_FIELD_BRD*)message_ptr;
            uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            if (partyMan)
            {
                uicPartyMemberInfoPlus* member_info = partyMan->GetPartyMemberInfo(recv_message->member_key_);
                if (member_info)
                {
                    member_info->set_map_code(recv_message->map_code_);
                    partyMan->CheckOutputZoneType();
                }
            }
        }
        break;
    case CG_PARTY_SELECT_TEAM_ACK:
        {
            MSG_CG_PARTY_SELECT_TEAM_ACK* recv_message = (MSG_CG_PARTY_SELECT_TEAM_ACK*)message_ptr;
            GetManager()->UnsetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_CHANGE_TEAM);
            if (recv_message && recv_message->result_code_ == RC::RC_PARTY_SUCCESS)
            {
                int a = 0;
            }
            else
            {
                //팀변경 실패
            }
        }
        break;
    case CG_PARTY_SELECT_TEAM_BRD:
        {
            MSG_CG_PARTY_SELECT_TEAM_BRD* recv_message = (MSG_CG_PARTY_SELECT_TEAM_BRD*)message_ptr;
            //팀변경
            SetTeam(recv_message->member_key_, recv_message->select_team_);
        }
        break;
    }   
}

void uiBattlezone2Lobby::ResetLobby()
{
    uiPartyMan* partyMan =
        static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    DWORD master_key = partyMan->GetMasterKey();

    if (ui_battlezone2_manager_ && partyMan && g_pHero)
    {  
        if (partyMan->GetPartyNum() != ui_battlezone2_manager_->GetUserSlotNum())
        {
            int a = 0;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());

#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
    UserSlot EnterUser1;
    EnterUser1.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    EnterUser1.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    ui_battlezone2_manager_->AddPlayer(EnterUser1);
#else
    UserSlot EnterUser, EnterUser1;

    if (master_key == g_pHero->GetObjectKey())
    {
        EnterUser1.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        EnterUser1.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        ui_battlezone2_manager_->AddPlayer(EnterUser1);
    }
    else
    {
        EnterUser.SetPlayerKey(master_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        EnterUser.SetPlayerName(partyMan->GetPartyLeader().Get().m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        ui_battlezone2_manager_->AddPlayer(EnterUser);

        EnterUser1.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        EnterUser1.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        ui_battlezone2_manager_->AddPlayer(EnterUser1);
    }
#endif//

    UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(EnterUser1.GetPlayerKey());
    if (user_slot != NULL)
    {
        if( user_slot->GetBattleZonePlayerKey() == 0 )
        {
            DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
            Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
            if (player != NULL)
            {
                user_slot->SetBattleZonePlayerKey(battlezone_object_key);
            }
        }
    }

    // 파티장, 나를 제외한 나머지 플레이어
    if (ui_battlezone2_manager_ && partyMan)
    {
        for (int i = 0; i < partyMan->GetPartyNum(); ++i)
        {
            const uicPartyMemberInfoPlus member_info = partyMan->GetParty(i);
            const _PARTY_MEMBER_INFO party_member_info = member_info.Get();

            DWORD member_key = party_member_info.m_dwMemberKey;
#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
			if (member_key != 0 && member_key != g_pHero->GetObjectKey())
#else
            if (member_key != 0 && member_key != g_pHero->GetObjectKey() && member_key != master_key)
#endif//_DH_BATTLEZONE2_LOBBY_SLOT
            {
                UserSlot EnterUser;
                EnterUser.SetPlayerKey(member_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                EnterUser.SetPlayerName(party_member_info.m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                ui_battlezone2_manager_->AddPlayer(EnterUser);
            }
        }
    }
}

void uiBattlezone2Lobby::SetTeam(DWORD member_key, BYTE team)
{
    //팀변경
    UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(member_key);
    if (user_slot)
    {
        user_slot->SetTeam(team);
    }
}

void uiBattlezone2Lobby::SetBattleRoom(PartyRoomInfo* party_room_info)
{
    if (GetManager())
    {
        GetManager()->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
        GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LIST, false);
        GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_TREE, false);
        GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_RESERVATION, false);
    }

    GetManager()->set_current_room_info(party_room_info);
    SetBaseInfo(party_room_info);
}


void uiBattlezone2Lobby::Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN()
{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    if (is_disconnected_uesr_kick_ == false)
    {
        if (GetManager()->SafePKBool(uiBattlezone2Man::PACKET_STATUS_SEND_BAN))
        {
            GetManager()->SetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_BAN);

            MSG_CG_PARTY_ROOM_FORCE_LEAVE_SYN send_packet;
            send_packet.member_key_ = kick_player_key();
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
    }
    else
    {
        if (GetManager()->SafePKBool(
            uiBattlezone2Man::PACKET_STATUS_SEND_DISCONNECTED_USER_BAN))
        {
            GetManager()->SetPKBool(
                uiBattlezone2Man::PACKET_STATUS_SEND_DISCONNECTED_USER_BAN);

            // 비정상 종료자 강퇴 요청
            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager)
            {
                MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN send_packet;
                send_packet.request_party_key_ = party_manager->GetPartyKey();
                StrnCopy(send_packet.leave_member_name_,
                    kick_player_name_,
                    MAX_CHARNAME_LENGTH);
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
    }

#else
    if (GetManager()->SafePKBool(uiBattlezone2Man::PACKET_STATUS_SEND_BAN))
    {
        GetManager()->SetPKBool(uiBattlezone2Man::PACKET_STATUS_SEND_BAN);
        MSG_CG_PARTY_ROOM_FORCE_LEAVE_SYN send_packet;
        send_packet.member_key_ = kick_player_key();

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}

void ConfirmKickMember(bool is_ok)
{
    if (is_ok)
    {
        uiBattlezone2Lobby* battlezone_lobby_dialog = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
        if (battlezone_lobby_dialog)
        {
            battlezone_lobby_dialog->Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN();
        }
    }
}

bool uiBattlezone2Lobby::BanPlayerByIndex(int index)
{
    assert(index >= 1 && index < MAX_VIEW_USER_SLOT-1);		

    if (GetManager()->IsMaster())
    {
        clicked_slot_index_ = index;
        // 방장만 내보내기를 한다.
        UserSlot* user_slot = GetManager()->GetUserSlotByIndex(index);

        if (!user_slot)
        {
            assert(0);
            return FALSE;
        }

        switch (user_slot->GetSlotState())
        {
        case UserSlot::READY:
        case UserSlot::FILL:
            {
                TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
                TCHAR temp_string[INTERFACE_STRING_LENGTH] ={0,};

                uiPartyMan* partyMan =
                    static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                uiSystemMan* system_manager =
                    static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                if (partyMan && system_manager)
                {
                    uicPartyMemberInfoPlus *party_info =
                        partyMan->GetPartyMemberInfo(user_slot->GetPlayerKey());

                    if (party_info)
                    {
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        if (party_info->IsConnected() == true)
                        {
                            kick_player_key_ = party_info->Get().m_dwMemberKey;
                            ZeroMemory(kick_player_name_, sizeof(kick_player_name_));
                            is_disconnected_uesr_kick_ = false;
                        }
                        else
                        {
                            StrnCopy(kick_player_name_,
                                party_info->Get().m_szCharName,
                                MAX_CHARNAME_LENGTH);
                            kick_player_key_ = 0;
                            is_disconnected_uesr_kick_ = true;
                        }
#else
                        kick_player_key_ = party_info->Get().m_dwMemberKey;
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

                        // %s님을 파티에서 탈퇴시키겠습니까?
                        g_InterfaceManager.GetInterfaceString(5215, temp_string, INTERFACE_STRING_LENGTH);

                        Snprintf(message,
                            INTERFACE_STRING_LENGTH-1,
                            temp_string,
                            party_info->Get().m_szCharName);
                        system_manager->Sys_ConfirmLock(message, ConfirmKickMember);
                    }
                }
            }
            break;

        default:
            {
                // 열려있거나 블럭된 슬롯이다.
                return FALSE;
            }
            break;
        }
    }
    else
    {
        assert(!"방장아니면 됐거든");
        return FALSE;
    }

    return TRUE;
}

bool uiBattlezone2Lobby::BlockPlayer(int index)
{
    if(!GetManager())
    {
        return false;
    }

    //assert(index >= 1 && index < MAX_VIEW_USER_SLOT);		

    // 방장만 블럭을 할수있당
    if(ui_battlezone2_manager_ && !ui_battlezone2_manager_->IsMaster())
    {
        return false;
    }
    clicked_slot_index_ = index;

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    //int current_user_number = ui_battlezone2_manager_->GetNumUser();
    //ui_battlezone2_manager_->m_iPendingPlayerIndex = index;

    UserSlot* user_slot = ui_battlezone2_manager_->GetUserSlotByIndex(index);
    if (!user_slot)
    {
        assert(0);
        return false;
    }

    // 최대 유저를 변경해준다. 최소 인원은 현재 셋못하게 한상태이다.
    switch (user_slot->GetSlotState())
    {
    case UserSlot::BLOCK:
        {
            // 블럭된 유저는 오픈해줘야지
            // 최대 유저 제한에 위배되지 않느냐?
            int opened_slot = ui_battlezone2_manager_->GetNonBlockUserSlotNum();		
            /*if (opened_slot >= ui_battlezone2_manager_->GetBattleRoomEntry().GetCurLimitMaxUser()) 
            {
                // 인원수 제한에 위배 됩니다.
                const int	c_iNotChangeMinNumber = 70245;
                g_InterfaceManager.GetInterfaceString(c_iNotChangeMinNumber, message, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);
                g_InterfaceManager.ShowMessageBox(message, 10000);
                return FALSE;
            }*/

            ++opened_slot;
            assert(opened_slot <= MAX_VIEW_USER_SLOT);	
            ui_battlezone2_manager_->ModifyPlayerNumber(opened_slot);
        }
        break;

    case UserSlot::OPENED:
        {
            // 오픈된 유저는 블럭해줘야지
            int blocked_number = ui_battlezone2_manager_->GetBlockUserSlotNum();	
            int can_block_number = ui_battlezone2_manager_->GetMaxUserSlot() - blocked_number;
            //int max_limit_user = ui_battlezone2_manager_->GetBattleRoomEntry().GetCurLimitMinUser();

            // TODO : 일단 Min User가 없으니까 1로하자.
            // Min User 보다 작게 블럭할수 없다.
            // 최소 유저 제한에 위배되지 않느냐?
            /*if (can_block_number <= max_limit_user) 
            {
                // 인원수 제한에 위배 됩니다.
                const int c_iNotChangeMinNumber = 70244;
                g_InterfaceManager.GetInterfaceString(c_iNotChangeMinNumber, message, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);
                return FALSE;
            }*/

            int change_number = ui_battlezone2_manager_->GetMaxUserSlot() - (blocked_number + 1);	
            ui_battlezone2_manager_->ModifyPlayerNumber(change_number);
        }
        break;

    case UserSlot::FILL:
    case UserSlot::READY:
        {
            // 무시.
        }
        break;

    default:
        assert(0);
        return false;
        break;
    }

    return true;
}

void uiBattlezone2Lobby::LeaveHero()
{
    ShowInterface(FALSE);//방나가기
    if (GetManager() && GetManager()->battlezone_room_state() == uiBattlezone2Man::ROOM_STATE_MISSION)
    {
        //GetManager()->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
    }
    else
    {
		GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LIST, true);
    }

    if(ui_battlezone2_manager_ && g_pHero)
    {
        ui_battlezone2_manager_->LeavePlayer(g_pHero->GetObjectKey());
    }

    if (GetManager())
    {
		GetManager()->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LIST);
        GetManager()->FlushUserSlotSet();
    }

    ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (chat_dialog)
    {
        chat_dialog->OnNoramlChat();
    }
}
#endif //_DH_BATTLEZONE2_