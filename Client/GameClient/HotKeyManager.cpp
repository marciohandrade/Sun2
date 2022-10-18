#include "SunClientPrecompiledHeader.h"
#include "hotkeymanager.h"
#include "SolarDialog.h"
#include "Input.h"
#include "ItemManager.h"
#include "MainDialog.h"
#include "interfacemanager.h"
#include "MouseHandler.h"
#include "hero.h"
#include "GlobalFunc.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "QuickContainer.h"
#include "uiGuildMan/uiGuildMan.h"
#include "HeroActionInput.h"
#include "uiQuestMan/uiQuestMan.h"
#include "BattleScene.h"
#include "RadarFullDialog.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ExtendInventoryDialog.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif//_DH_BATTLEZONE2_
#include "Input/KeyCommands.h"
#include "Interface/UIActionFunction.h"
#include "Input/HotKeyInfoParser.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "uiHotKeyDialog/uiHotKeyDialog.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#endif //_NA_000000_20121210_REFACTORY_GAME_USER

#include "uiOptionKeyBinding.h"
#include "uiMenuMan/uiMenuMan.h"

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
#include "RadarFullDialog.h"

#include "uiInventoryMan/uiInventoryMan.h"
#include "InventoryDialog.h"
#endif
#ifdef _NA_007086_20140318_MONSTERBOOK
  #include "uiMonsterBookMan/uiMonsterBookMan.h"
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMiniGameMan/uiMiniGameMan.h"
#endif // _NA_008405_20150716_MINIGAME_BINGO
HotKeyManager::HotKeyManager()
{
    set_is_character_setting(false);
    set_is_user_setting_mode(false);
}

HotKeyManager::~HotKeyManager()
{
    DialogToggleCommand::DestroyInstance();
    QuickSlotModeCommand::DestroyInstance();
    QuickSlotUseCommand::DestroyInstance();
    SummonControlCommand::DestroyInstance();
    TargetEnemyCommand::DestroyInstance();
    TargetPartyMemberCommand::DestroyInstance();
    ScreenShotCommand::DestroyInstance();
    ChatOpenNSendCommand::DestroyInstance();
    CameraControlCommand::DestroyInstance();
    SystemMenuNCancelCommand::DestroyInstance();
    HeroAutoRunCommand::DestroyInstance();
    HeroMoveCommand::DestroyInstance();
    HeroJumpCommand::DestroyInstance();
    HeroActionSkillCommand::DestroyInstance();
    HeroSpreadWingCommand::DestroyInstance();
}

void HotKeyManager::Initialize()
{
    BuildDefalutKeyBindInfo();

    current_using_key_bind_info() = defalut_key_bind_info();
}

void HotKeyManager::Process()
{
    if (is_user_setting_mode() == false)
    {
        // 통상적 처리
        KEY_BIND_PAIR_MAP& bind_pair_map = current_using_key_bind_info().key_bind_pair_map();
        KEY_BIND_PAIR_MAP::iterator begin_itr = bind_pair_map.begin();
        KEY_BIND_PAIR_MAP::iterator end_itr = bind_pair_map.end();
        HOT_KEY_FUNCTION_INDEX key_function_index;
        KEY_BIND_PAIR bind_pair;
        KeyCommand* key_command = NULL;

        for ( ; begin_itr != end_itr; ++begin_itr)
        {
            key_function_index = begin_itr->first;
            bind_pair = begin_itr->second;
            key_command = GetKeyCommand(key_function_index);
            if (key_command != NULL)
            {
                key_command->OnAction(key_function_index, bind_pair);
            }        
        }
    }
    else
    {
        // 유저키 입력대기 상태
        WORD input_key_code = 0;
        input_key_code = CheckUserKeyInput();
        if (input_key_code != 0)
        {
            EndUserSettingMode(input_key_code);
        }        
    }
}

void HotKeyManager::ProcessHotkeyFunction(HOT_KEY_FUNCTION_INDEX hot_key_function_index)
{
    KEY_BIND_PAIR_MAP& bind_pair_map = current_using_key_bind_info().key_bind_pair_map();
    KEY_BIND_PAIR_MAP::iterator found_itr = bind_pair_map.find(hot_key_function_index);
    if (found_itr != bind_pair_map.end())
    {
        KeyCommand* key_command = GetKeyCommand(hot_key_function_index);
        if (key_command != NULL)
        {
            key_command->OnAction(hot_key_function_index, found_itr->second);
        }
    }
}

void HotKeyManager::StartUserSettingMode()
{
    set_is_user_setting_mode(true);
}

void HotKeyManager::EndUserSettingMode(WORD input_key_code)
{
    set_is_user_setting_mode(false);

    if (input_key_code != 0)
    {
        // 조합된 확장키 상태 체크
        HOT_KEY_CODE hot_key_code;
        if (g_Input.GetState(DIK_LSHIFT, KS_KEY, TRUE) || 
            g_Input.GetState(DIK_LSHIFT, IKS_KEY, TRUE))
        {
            hot_key_code.shift_key_ = 1;
        }

        if (g_Input.GetState(DIK_LALT, KS_KEY, TRUE) || 
            g_Input.GetState(DIK_LALT, IKS_KEY, TRUE))
        {
            hot_key_code.alt_key_ = 1;
        }

        if (g_Input.GetState(DIK_LCONTROL, KS_KEY, TRUE) || 
            g_Input.GetState(DIK_LCONTROL, IKS_KEY, TRUE))
        {
            hot_key_code.ctrl_key_ = 1;
        }
        
        hot_key_code.key_code_ = input_key_code;
        
#ifdef _NA_000000_20130114_RENEWER_UI
        // UI에 키조합 전달
        uiOptionKeyBinding* hot_key_dialog = GET_CAST_DIALOG(uiOptionKeyBinding, IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG);
        if (hot_key_dialog != NULL)
        {
            hot_key_dialog->RecvUserKeySetting(hot_key_code.hot_key_code_);
        }
#else //_NA_000000_20130114_RENEWER_UI
        // UI에 키조합 전달
        uiHotKeyDialog* hot_key_dialog = GET_CAST_DIALOG(uiHotKeyDialog, IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG);
        if (hot_key_dialog != NULL)
        {
            hot_key_dialog->RecvUserKeySetting(hot_key_code.hot_key_code_);
        }
#endif //_NA_000000_20130114_RENEWER_UI
    }
}

WORD HotKeyManager::CheckUserKeyInput()
{
    // 입력 허용키 상태 체크
    BOOL is_input = FALSE;
    HotKeyInfoParser::AllowKeyCodeMap& allow_key_map = HotKeyInfoParser::Instance()->allow_key_code_map();
    HotKeyInfoParser::AllowKeyCodeMap::iterator found_itr = allow_key_map.begin();
    HotKeyInfoParser::AllowKeyCodeMap::iterator end_itr = allow_key_map.end();
    for ( ; found_itr != end_itr; ++found_itr)
    {
        WORD key_code = found_itr->first;
        if (key_code == 0)
        {
            continue;
        }

        if (key_code < MAX_KEY_BUFFER)
        {
            // 키입력
            if (g_Input.GetState(key_code, KS_UP, TRUE) || 
                g_Input.GetState(key_code, IKS_UP, TRUE))
            {
                is_input = TRUE;
            }
        }
        else if (key_code < (MOUSE_BUTTON8 + 1))
        {
            // 마우스 버튼입력
            if (g_Input.GetState(key_code, KS_MOUSE_UP, TRUE) || 
                g_Input.GetState(key_code, IKS_MOUSE_UP, TRUE))
            {
                is_input = TRUE;
            }
        }
        else if (key_code < (MOUSE_WHEEL_BUTTON_DOWN + 1))
        {
            // 마우스 휠입력
            if (g_Input.GetMouseWheel() == (key_code - MOUSE_WHEEL_BUTTON_UP + 1))
            {
                is_input = TRUE;
            }
        }

        if (is_input == TRUE)
        {
            return key_code;
        }
    }

    return 0;
}

KEY_BIND_PAIR* HotKeyManager::GetKeyBindPair(HOT_KEY_FUNCTION_INDEX hot_key_function_index)
{
    KEY_BIND_PAIR_MAP& bind_pair_map = current_using_key_bind_info().key_bind_pair_map();
    KEY_BIND_PAIR_MAP::iterator found_itr = bind_pair_map.find(hot_key_function_index);
    if (found_itr != bind_pair_map.end())
    {
        return &(found_itr->second);
    }

    return NULL;
}

bool HotKeyManager::CheckInputBindKey(KEY_BIND_PAIR& key_bind_pair, 
                                      int key_state, 
                                      BOOL force_input /*= FALSE*/)
{
    int ext_key_state = KS_KEY;

    BYTE shift_key = (g_Input.GetState(DIK_LSHIFT, ext_key_state, force_input) != FALSE) ? 1 : 0;
    BYTE ctrl_key = (g_Input.GetState(DIK_LCONTROL, ext_key_state, force_input) != FALSE) ? 1 : 0;
    BYTE alt_key = (g_Input.GetState(DIK_LALT, ext_key_state, force_input) != FALSE) ? 1 : 0;
    WORD key_code = key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].key_code_;

    if ((key_code != 0) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].shift_key_ == shift_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].ctrl_key_ == ctrl_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].alt_key_ == alt_key))
    {
        if (g_Input.GetState(key_code, key_state, force_input) != FALSE)
        {
            return true;
        }
    }

    key_code = key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].key_code_;
    if ((key_code != 0) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].shift_key_ == shift_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].ctrl_key_ == ctrl_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].alt_key_ == alt_key))
    {
        if (g_Input.GetState(key_code, key_state, force_input) != FALSE)
        {
            return true;
        }
    }

    if (force_input == TRUE)
    {
        ext_key_state = KS_KEY>>4;
        key_state = key_state>>4;

        shift_key = (g_Input.GetState(DIK_LSHIFT, ext_key_state, force_input) != FALSE) ? 1 : 0;
        ctrl_key = (g_Input.GetState(DIK_LCONTROL, ext_key_state, force_input) != FALSE) ? 1 : 0;
        alt_key = (g_Input.GetState(DIK_LALT, ext_key_state, force_input) != FALSE) ? 1 : 0;
        key_code = key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].key_code_;

        if ((key_code != 0) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].shift_key_ == shift_key) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].ctrl_key_ == ctrl_key) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].alt_key_ == alt_key))
        {
            if (g_Input.GetState(key_code, key_state, force_input) != FALSE)
            {
                return true;
            }
        }

        key_code = key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].key_code_;
        if ((key_code != 0) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].shift_key_ == shift_key) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].ctrl_key_ == ctrl_key) && 
            (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kSubHotKey].alt_key_ == alt_key))
        {
            if (g_Input.GetState(key_code, key_state, force_input) != FALSE)
            {
                return true;
            }
        }
    }

    return false;
}

bool HotKeyManager::CompareHotKeyInfo(KEY_BIND_INFO& left_Info, KEY_BIND_INFO& right_Info)
{
    KEY_BIND_PAIR_MAP::iterator left_found_itr = left_Info.key_bind_pair_map().begin();
    KEY_BIND_PAIR_MAP::iterator left_end_itr = left_Info.key_bind_pair_map().end();
    KEY_BIND_PAIR_MAP::iterator right_found_itr = right_Info.key_bind_pair_map().begin();
    KEY_BIND_PAIR_MAP::iterator right_end_itr = right_Info.key_bind_pair_map().end();

    for ( ; left_found_itr != left_end_itr; ++left_found_itr)
    {
        if (left_found_itr->first != right_found_itr->first)
        {
            return false;
        }

        KEY_BIND_PAIR& left_pair = left_found_itr->second;
        KEY_BIND_PAIR& right_pair = right_found_itr->second;

        if (left_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey) != 
            right_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey))
        {
            return false;
        }

        if (left_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey) != 
            right_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey))
        {
            return false;
        }

        ++right_found_itr;
        if (right_found_itr == right_end_itr)
        {
            ++left_found_itr;
            break;
        }
    }

    if ((left_found_itr != left_end_itr) || 
        (right_found_itr != right_end_itr))
    {
        return false;
    }

    return true;
}

bool HotKeyManager::MakeToStream(GameOption& game_option)
{
    WORD* stream = (WORD*)(&game_option.binary_data_[0]);
    int offset = 0;
    // 버전 관리, offset : 0
    *(stream + offset++) = 1;

    // 키값저장
    KEY_BIND_PAIR_MAP::iterator found_itr = temp_key_bind_info().key_bind_pair_map().begin();
    KEY_BIND_PAIR_MAP::iterator end_itr = temp_key_bind_info().key_bind_pair_map().end();
    for ( ; found_itr != end_itr; ++found_itr)
    {        
        *(stream + offset++) = (WORD)found_itr->first;

        KEY_BIND_PAIR& key_bind_pair = found_itr->second;
        *(stream + offset++) = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
        *(stream + offset++) = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);

        if (((offset + 3) * sizeof(WORD)) > MAX_GAMEOPTIONSTREAM_SIZE)
        {
            assert(!"((offset + 3) * sizeof(WORD)) > MAX_GAMEOPTIONSTREAM_SIZE");
            return false;
        }
    }
    
    game_option.binary_data_size_ = (offset * sizeof(WORD));

    return true;
}

bool HotKeyManager::MakeToHotKeyInfo(GameOption& game_option, KEY_BIND_INFO& apply_info)
{
    bool is_success = false;
    if (game_option.binary_data_size_ < sizeof(WORD))
    {
        return is_success;
    }

    WORD* stream = (WORD*)(&game_option.binary_data_[0]);
    int offset = 0;

    // 버전 얻기
    WORD version = *(stream + offset++);

    if (version == 1)
    {
        temp_key_bind_info().key_bind_pair_map().clear();
        temp_key_bind_info().hot_key_code_association_map().clear();

        HOT_KEY_FUNCTION_INDEX function_index;
        KEY_BIND_PAIR key_bind_pair;
        for ( ; ((offset + 3) * sizeof(WORD)) <= game_option.binary_data_size_; )
        {
            function_index = static_cast<HOT_KEY_FUNCTION_INDEX>(*(stream + offset++));
            key_bind_pair.SetHotKeyCode(KEY_BIND_PAIR::kMainHotKey, *(stream + offset++));
            key_bind_pair.SetHotKeyCode(KEY_BIND_PAIR::kSubHotKey, *(stream + offset++));

            temp_key_bind_info().SetKeyBindInfo(function_index, key_bind_pair);
        }

        apply_info = temp_key_bind_info();
        //set_is_character_setting(true);
        is_success = true;
    }

    return is_success;
}

void HotKeyManager::SaveHotKeyInfoToServer(bool is_check_character)
{
    // 스트림으로 변경
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN send_packet;    

    if (is_character_setting() == true)
    {
        if (is_check_character == true)
        {
            // 캐릭터 -> 캐릭터 : 캐릭터 업데이트
            // 디폴트와 같은지 체크하여 다르면 저장함
            if (CompareHotKeyInfo(temp_key_bind_info(), current_using_key_bind_info()) == false)
            {
                send_packet.game_option_.option_type_ = GameOption::kKeyCustomizingPerChar;
                if (MakeToStream(send_packet.game_option_) == true)
                {
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
                }
            }
        }
        else
        {
            // 캐릭터 -> 계정 : 계정 저장, 캐릭터 삭제
            if (CompareHotKeyInfo(temp_key_bind_info(), account_key_bind_info()) == false)
            {
                send_packet.game_option_.option_type_ = GameOption::kKeyCustomizing;
                if (MakeToStream(send_packet.game_option_) == true)
                {
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
                }
            }

            send_packet.game_option_.option_type_ = GameOption::kKeyCustomizingPerChar;
            send_packet.game_option_.binary_data_size_ = 0;
            send_packet.game_option_.binary_data_[0] = 0;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
    }
    else
    {
        if (is_check_character == true)
        {
            // 계정 -> 캐릭터 : 캐릭터 저장
            send_packet.game_option_.option_type_ = GameOption::kKeyCustomizingPerChar;
            if (MakeToStream(send_packet.game_option_) == true)
            {
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
        else
        {
            // 계정/캐릭터 저장 여부에 따른 분기
            // 계정 -> 계정 : 계정 업데이트
            // 디폴트와 같은지 체크하여 다르면 저장함
            if (CompareHotKeyInfo(temp_key_bind_info(), account_key_bind_info()) == false)
            {
                send_packet.game_option_.option_type_ = GameOption::kKeyCustomizing;
                if (MakeToStream(send_packet.game_option_) == true)
                {
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
                }
            }
        }
    }
    set_is_character_setting(is_check_character);
}

void HotKeyManager::LoadHotKeyInfoFromServer()
{
    current_using_key_bind_info() = defalut_key_bind_info();

    MSG_CG_ETC_GAMEOPTION_LOAD_SYN send_packet;
    send_packet.option_type_ = GameOption::kKeyCustomizing;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

    set_is_character_setting(false);
}

void HotKeyManager::LoadHotKeyInfoFromServerByCharacter()
{
    MSG_CG_ETC_GAMEOPTION_LOAD_SYN send_packet;
    send_packet.option_type_ = GameOption::kKeyCustomizingPerChar;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}

void HotKeyManager::RecvHotKeyInfo(GameOption& game_option)
{
    if (game_option.option_type_ == GameOption::kKeyCustomizing)
    {
        // 계정 세팅
        // 스트림을 풀어서 current에 저장
        if (MakeToHotKeyInfo(game_option, account_key_bind_info()))
        {
#ifdef _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
			InValidDefaultShortCutCheck(account_key_bind_info(), GameOption::kKeyCustomizing); // 새로 추가된 키가 있는지 검사
#endif // _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
            current_using_key_bind_info() = account_key_bind_info();
        }
        
        // 캐릭터 세팅 요청
        LoadHotKeyInfoFromServerByCharacter();
    }
    else if (game_option.option_type_ == GameOption::kKeyCustomizingPerChar)
    {
        // 캐릭터 세팅
        // 스트림을 풀어서 current에 저장
        if (MakeToHotKeyInfo(game_option, current_using_key_bind_info()) == true)
        {
#ifdef _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
			InValidDefaultShortCutCheck(current_using_key_bind_info(), GameOption::kKeyCustomizingPerChar);
#endif // _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
            set_is_character_setting(true);
        }
    }
#ifdef _NA_000000_20130114_RENEWER_UI
	uiMenuMan* MainMenuMan = static_cast<uiMenuMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));
	if(MainMenuMan)
	{
		MainMenuMan->KeyModifyChange();
	}
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    RadarFullDialog*  pfullRadar = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
    if (pfullRadar)
    {
        pfullRadar->CheckModifyOption();
    }

    InventoryDialog*  pInventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if (pInventory)
    {
        pInventory->CheckModifyOption();
    }
#endif

#endif

}

void HotKeyManager::RecLoadNak(GameOption::Type option_type)
{
    if (option_type == GameOption::kKeyCustomizing)
    {
        LoadHotKeyInfoFromServerByCharacter();
    }
}

void HotKeyManager::BuildDefalutKeyBindInfo()
{
    HotKeyInfoParser::Instance()->GetDefalutKeyInfo(defalut_key_bind_info());

    BuildKeyCommands();
}

void HotKeyManager::BuildKeyCommands()
{
    // 각 KeyCommand 인스턴스 컨테이너 삽입
    // 캐릭터 이동 관련
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kMove_Forward, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kMove_Backward, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kMove_LeftSide, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kMove_RightSide, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kTurn_Left, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kTurn_Right, HeroMoveCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kJump, HeroJumpCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kAutoRun, HeroAutoRunCommand::Instance()));    

    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSitStand, HeroActionSkillCommand::Instance()));    
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kPickUp_Item, HeroActionSkillCommand::Instance()));

    key_command_container().insert(KEY_COMMAND_MAP::value_type(kWingSpread, HeroSpreadWingCommand::Instance()));

    // 대화 입력/보내기
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kScreenShot, ScreenShotCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kScreenShotMode, ScreenShotCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kHide_InterfaceAll, ScreenShotCommand::Instance()));

    // 타겟 선택
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_TargetEnemy, TargetEnemyCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kNext_TargetEnemy, TargetEnemyCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kPrev_TargetEnemy, TargetEnemyCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_Hero, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember1, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember2, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember3, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember4, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember5, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember6, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember7, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember8, TargetPartyMemberCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_PartyMember9, TargetPartyMemberCommand::Instance()));
    
    // 퀵슬롯 확장 토글
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kExpand_QuickSlotBar, QuickSlotModeCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kExchange_QuickSlotBar, QuickSlotModeCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot1, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot2, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot3, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot4, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot5, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot6, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot7, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot8, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot9, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot10, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot11, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot12, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot13, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot14, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot15, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot16, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot17, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot18, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot19, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot20, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot21, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot22, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot23, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot24, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot25, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot26, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot27, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot28, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot29, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot30, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot31, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot32, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot33, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot34, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot35, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot36, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot37, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot38, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot39, QuickSlotUseCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_QuickSlot40, QuickSlotUseCommand::Instance()));

    // UI 토글
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_CharacterStatus_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Inventory_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kModeChange_ExtendInventory_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Skill_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Quest_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Guild_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Community_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_AreaConquest_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_PetStatus_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_SystemMenu_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_ContinentMap_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_BattleScore_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kModeChange_Radar_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kModeChange_FullRadar_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_CashShop_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_GuideInfomation_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_MissionObject_UI, DialogToggleCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_BattleZone_UI, DialogToggleCommand::Instance()));
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_ChaosZone_UI, DialogToggleCommand::Instance()));
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_personal_Warp_UI, DialogToggleCommand::Instance()));
#endif //_NA_000000_20121210_REFACTORY_GAME_USER
#ifdef _NA_007086_20140318_MONSTERBOOK
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_MonsterBook_UI, DialogToggleCommand::Instance()));
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_Attendance_UI, DialogToggleCommand::Instance()));
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_SUN_Ranking_UI, DialogToggleCommand::Instance()));
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_MINI_Game_UI, DialogToggleCommand::Instance()));
#endif // _NA_008405_20150716_MINIGAME_BINGO

    // 취소/시스템메뉴
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOpen_SystemMenu_N_Cancel, SystemMenuNCancelCommand::Instance()));

    // 소환수 조종
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kSelect_MySummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_SummonSkill1, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_SummonSkill2, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_SummonSkill3, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kUse_SummonSkill4, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOrderAttack_SummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOrderFollow_SummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kAttackMode_SummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kDefenceMode_SummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kRelease_SummonMonster, SummonControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kOrderNonPKAttack_SummonMonster, SummonControlCommand::Instance()));

    // 카메라조종
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kZoom_In, CameraControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kZoom_Out, CameraControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kRotate_Left_Camera, CameraControlCommand::Instance()));
    key_command_container().insert(KEY_COMMAND_MAP::value_type(kRotate_Right_Camera, CameraControlCommand::Instance()));

    // 각 KeyCommand 에 대한 추가적인 처리
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_CharacterStatus_UI, UIActionFunction::ToggleCharacterStatusUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Inventory_UI, UIActionFunction::ToggleInventoryUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kModeChange_ExtendInventory_UI, UIActionFunction::ToggleExtendInventoryUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Skill_UI, UIActionFunction::ToggleSkillUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Quest_UI, UIActionFunction::ToggleQuestUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Guild_UI, UIActionFunction::ToggleGuildUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Community_UI, UIActionFunction::ToggleCommunityUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_AreaConquest_UI, UIActionFunction::ToggleACUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_PetStatus_UI, UIActionFunction::TogglePetUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_SystemMenu_UI, UIActionFunction::ToggleSystemMenuUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_ContinentMap_UI, UIActionFunction::ToggleContinentMap);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_BattleScore_UI, UIActionFunction::ToggleBattleScoreUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kModeChange_Radar_UI, BattleScene::ShowRadirDialog);
    DialogToggleCommand::Instance()->RegisterActionFunction(kModeChange_FullRadar_UI, UIActionFunction::ToggleFullRadarMode);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_CashShop_UI, UIActionFunction::ToggleCashShopUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_GuideInfomation_UI, UIActionFunction::ToggleGuideInfomationUI);
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_MissionObject_UI, UIActionFunction::ToggleMissionObjectUI);
#ifdef _DH_BATTLEZONE2_
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_BattleZone_UI, uiBattlezone2Man::ToggleBattlezoneDialog);
#endif //_DH_BATTLEZONE2_    
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_ChaosZone_UI, uiChaosSystemMan::HotKeyToggleChaoszoneDialog);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
	DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_personal_Warp_UI, uiPersonalWayPointMan::HotKeyTogglePersonalwarpDialog);
#endif //_NA_000000_20121210_REFACTORY_GAME_USER
#ifdef _NA_007086_20140318_MONSTERBOOK
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_MonsterBook_UI, uiMonsterBookMan::HotKeyToggleMonsterBookDialog);
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_Attendance_UI, uiAttendanceCheckMan::HotKeyToggleAttendanceCheckDialog);
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_SUN_Ranking_UI, uiSUNRankingMan::HotKeyToggleSUNRankingDialog);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	DialogToggleCommand::Instance()->RegisterActionFunction(kOpen_MINI_Game_UI, uiMiniGameMan::HotKeyToggleMiniGameBingoDialog);
#endif // _NA_008405_20150716_MINIGAME_BINGO
}

KeyCommand* HotKeyManager::GetKeyCommand(HOT_KEY_FUNCTION_INDEX hot_key_function_index)
{
    STLX_MAP<HOT_KEY_FUNCTION_INDEX, KeyCommand*>::iterator found_itr = 
        key_command_container().find(hot_key_function_index);
    if (found_itr != key_command_container().end())
    {
        return found_itr->second;
    }

    return NULL;
}

#ifdef _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
void HotKeyManager::InValidDefaultShortCutCheck(KEY_BIND_INFO& dest_key_bind, GameOption::Type option_type)
{
	// 현재 단축키와 기본 단축키가 다를 때...
	if ( CompareHotKeyInfo(dest_key_bind, defalut_key_bind_info()) == false )
	{
		KEY_BIND_PAIR_MAP::iterator found_itr = defalut_key_bind_info().key_bind_pair_map().begin();
		KEY_BIND_PAIR_MAP::iterator end_itr = defalut_key_bind_info().key_bind_pair_map().end();
		KEY_BIND_PAIR_MAP::iterator temp_itr;
		bool is_change = false;
		for ( ; found_itr != end_itr; ++found_itr)
		{
			temp_itr = dest_key_bind.key_bind_pair_map().find(found_itr->first);
			if ( temp_itr == dest_key_bind.key_bind_pair_map().end() )
			{
				HOT_KEY_FUNCTION_INDEX index = found_itr->first;

				KEY_BIND_PAIR& key_bind_pair = found_itr->second;
				WORD main_hot_key = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
				WORD sub_hot_key = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);

				bool is_exist_key = false;
				KEY_BIND_PAIR_MAP::iterator temp_find_itr = dest_key_bind.key_bind_pair_map().begin();
				for ( ; temp_find_itr != dest_key_bind.key_bind_pair_map().end(); ++temp_find_itr )
				{
					is_exist_key = false;
					// 이미 같은 키가 존재하는지 검사.
					WORD temp_main_hot_key = temp_find_itr->second.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
					WORD temp_sub_hot_key = temp_find_itr->second.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);
					if ( main_hot_key == temp_main_hot_key && sub_hot_key == temp_sub_hot_key )
					{
						is_exist_key = true;
						break;
					}
				}

				// 이미 키가 존재하면 0으로 셋팅.
				if ( is_exist_key == true )
				{
					KEY_BIND_PAIR temp_key_bind_pair;
					temp_key_bind_pair.SetHotKeyCode(KEY_BIND_PAIR::kMainHotKey, 0);
					temp_key_bind_pair.SetHotKeyCode(KEY_BIND_PAIR::kSubHotKey, 0);
					dest_key_bind.SetKeyBindInfo(index, temp_key_bind_pair);
				}
				else
				{
					dest_key_bind.SetKeyBindInfo(index, key_bind_pair);
				}

				is_change = true;
			}
		}

		if ( is_change == true )
		{
			temp_key_bind_info() = dest_key_bind;

			// 스트림으로 변경
			MSG_CG_ETC_GAMEOPTION_SAVE_SYN send_packet;
			send_packet.game_option_.option_type_ = option_type;
			if (MakeToStream(send_packet.game_option_) == true)
			{
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
			}
		}
	}
}
#endif // _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS

//////////////////////////////////////////////////////////////////////////
//HOT_CODE HotKeyManager::m_HotCode[eHOTKEY_MAX] = 
//{ 
//    /* { shift, ctrl, alt, normal_key, excute } */
//    { false, true, false, DIK_ESCAPE, OPERATION_TOGGLE_DIALOG_FUNCTION },	//eHOTKEY_SYSTEMDLG,
//    { false, false, false, DIK_I, OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP },			//eHOTKEY_INVENTORYDLG,
//    { false, false, false, DIK_K, OPERATION_TOGGLE_DIALOG_FUNCTION },	//eHOTKEY_SKILLDLG,
//    { false, false, false, DIK_O, OPERATION_TOGGLE_DIALOG_FUNCTION },			//eHOTKEY_MISSIONOBJECTDLG,	
//    { false, false, false, DIK_C, OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP },			//eHOTKEY_CHARACTERSKILLDLG,	
//    { false, false, false, DIK_F, OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP },			//eHOTKEY_FRIENDDLG,	
//    { false, false, false, DIK_J, OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP },			//eHOTKEY_ACDLG,
//    { false, false, false, DIK_Z, OPERATION_TOGGLE_DIALOG_FUNCTION },			//eHOTKEY_BATTLE_RESULT_DLG,
//    { false, false, false, DIK_G, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_GUILDDLG
//    { false, false, false, DIK_P, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_PETDLG
//    { false, false, false, DIK_L, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_QUESTDLG
//    { false, false, false, DIK_N, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_RADARDLG
//    { false, false, false, DIK_M, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_FULLMAPDLG
//    { false, false, false, DIK_B, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_WORLDMAPDLG
//    { false, false, false, DIK_F12, OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP }, //eHOTKEY_OPTIONDLG
//    { false, false, false, DIK_U, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_EXTEND_INVENTORY_DLG
//    { false, false, false, DIK_RETURN, OPERATION_TOGGLE_DIALOG_FUNCTION }, //eHOTKEY_CHATTING_DLG
//
//    { false, false, false, DIK_1, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_1
//    { false, false, false, DIK_2, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_2
//    { false, false, false, DIK_3, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_3
//    { false, false, false, DIK_4, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_4
//    { false, false, false, DIK_5, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_5
//    { false, false, false, DIK_6, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_6
//    { false, false, false, DIK_7, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_7
//    { false, false, false, DIK_8, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_8
//    { false, false, false, DIK_9, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_9
//    { false, false, false, DIK_0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_10
//    { false, false, false, DIK_1, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_11
//    { false, false, true, DIK_2, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_12
//    { false, false, true, DIK_3, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_13
//    { false, false, true, DIK_4, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_14
//    { false, false, true, DIK_5, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_15
//    { false, false, true, DIK_6, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_16
//    { false, false, true, DIK_7, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_17
//    { false, false, true, DIK_8, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_18
//    { false, false, true, DIK_9, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_19
//    { false, false, true, DIK_0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_20
//
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_21
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_22
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_23
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_24
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_25
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_26
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_27
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_28
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_29
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_30
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_31
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_32
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_33
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_34
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_35
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_36
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_37
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_38
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_39
//    { false, false, false, 0, OPERATION_QUICK_SLOT }, //eHOTKEY_QUICK_40
//
//    { false, false, false, DIK_F1, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_SELF,
//    { false, false, false, DIK_F2, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_1,
//    { false, false, false, DIK_F3, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_2,
//    { false, false, false, DIK_F4, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_3,
//    { false, false, false, DIK_F5, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_4,
//    { false, false, false, DIK_F6, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_5,
//    { false, false, false, DIK_F7, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_6,
//    { false, false, false, DIK_F8, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_7,
//    { false, false, false, DIK_F9, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_8,
//    { false, false, false, DIK_F10, OPERATION_SELECT_PARTY }, //eHOTKEY_SELECT_PARTY_9,
//
//#ifdef _JAPAN
//    { false, false, false, DIK_MINUS, OPERATION_SYSTEM }, //eHOTKEY_SYSTEM_QUICK_CHANGE, 퀵슬롯 변경
//    { false, false, false, DIK_CIRCUMFLEX, OPERATION_SYSTEM },//eHOTKEY_SYSTEM_QUICK_EXTEND, 퀵슬롯 확장,축소
//#else
//    { false, false, false, DIK_GRAVE, OPERATION_SYSTEM }, //eHOTKEY_SYSTEM_QUICK_CHANGE, 퀵슬롯 변경
//    { false, false, false, DIK_MINUS, OPERATION_SYSTEM },//eHOTKEY_SYSTEM_QUICK_EXTEND, 퀵슬롯 확장,축소
//#endif//_JAPAN
//};
//
//HotKeyManager::HotKeyManager()
//{
//}
//
//HotKeyManager::~HotKeyManager()
//{
//}
//
//VOID HotKeyManager::HotKeyUpdate()
//{
//    for( int code = 0 ; code < eHOTKEY_MAX ; ++code )
//    {
//        {
//            bool is_shift_down = ((g_Input.GetState(DIK_LSHIFT, KS_KEY) != FALSE) || (g_Input.GetState(DIK_RSHIFT, KS_KEY)!= FALSE));
//            bool is_alt_down = (g_Input.GetState(DIK_LALT, KS_KEY) != FALSE);
//            bool is_control_down = (g_Input.GetState(DIK_LCONTROL, KS_KEY) != FALSE);
//
//            if(!(is_shift_down ^ m_HotCode[code].shift_key) && // shift가 활성화 일때 shift 키 인경우 true
//                !(is_alt_down ^ m_HotCode[code].alt_key) && // alt키가 활성화 일때 lalt인 경우 true
//                !(is_control_down ^ m_HotCode[code].ctrl_key) && // alt키가 활성화 일때 lalt인 경우 true
//                g_Input.GetState(m_HotCode[code].iCmdKey, KS_DOWN))
//            {
//                //	단독키 사용( 조합키 없이 단독으로 )
//                ExcuteHotKey( (eHOTKEYCODE)code, m_HotCode[code].iOpeType );
//            }
//        }
//
//    }
//}
//
//VOID HotKeyManager::Initialize()
//{
//    int i = 0;
//    for (i = 0; i < eHOTKEY_DLG_MAX; ++i)
//    {
//        datas[i] = 0;
//    }
//}
//
//VOID HotKeyManager::RegisterHotKey( eHOTKEYCODE code, SolarDialog * pDlg )
//{
//#ifdef MAP_EDIT_MODE
//    // no operation
//#else
//    datas[code] = static_cast<void*>(pDlg);
//#endif
//}
//
//VOID HotKeyManager::UnregisteredHotKey(eHOTKEYCODE code)
//{
//    //assert( m_pDlg[ code ] );
//    datas[code] = NULL;
//}
//
//VOID HotKeyManager::ShowDialogFunction(eHOTKEYCODE code)
//{
//    switch (code)
//    {
//    case eHOTKEY_BATTLE_RESULT_DLG:
//        {
//#ifdef _NA_0_20091118_GAMEROOM_BATTLE_RESULT
//            uiBattleScoreMan* battle_score_manager_ptr = 
//                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
//            if (battle_score_manager_ptr && battle_score_manager_ptr->GetMode() != uiBattleScoreMan::eBTS_NONE_MODE &&
//                battle_score_manager_ptr->GetMode() != uiBattleScoreMan::eBTS_MODE_MAX)
//            {
//                battle_score_manager_ptr->set_toggle_ui_open(false);
//                battle_score_manager_ptr->ToggleBattleScoreWindow();
//            }
//#endif //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
//        }
//        break;
//    case eHOTKEY_MISSIONOBJECTDLG:
//        {
//#if defined(_NA_0_20091118_GAMEROOM_BATTLE_RESULT) 
//            uiBattleScoreMan* battle_score_manager_ptr = 
//                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
//            if (battle_score_manager_ptr && battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_MISSION)
//            {
//                if( NULL == datas[ code ] ) return;
//                SolarDialog* dialog = static_cast<SolarDialog*>(datas[code]);
//                BOOL val = dialog->IsVisibleWindow();
//                if( g_pHero && g_pHero->IsShowFullMap() == FALSE )
//                {
//                    dialog->ShowDialog( val ^= TRUE );
//                }
//            }
//#endif//_NA_0_20091118_GAMEROOM_BATTLE_RESULT 
//        }
//        break;
//    case eHOTKEY_SKILLDLG:
//        {
//            //	전체멥 출력할 때는 다이얼로그 자체를 처리 안한다.
//            if( g_pHero && g_pHero->IsShowFullMap() == FALSE )
//            {
//                uiBase* pUIBase = g_InterfaceManager.GetUISkillCurrentDialog();
//
//                if (pUIBase)
//                {
//                    BOOL visible = pUIBase->IsVisible();
//                    pUIBase->ShowInterfaceWithSound(visible);
//                }
//            }
//        }
//        break;
//    case eHOTKEY_SYSTEMDLG:
//        {
//            GlobalFunc::DoSystemDialog(0,0);
//        }
//        break;
//    case eHOTKEY_GUILDDLG:
//        {
//            // 길드 메뉴
//            uiGuildMan* guildMan =
//                static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
//
//            if (guildMan)
//            {
//                if(guildMan->IsShowWindow())
//                {
//                    guildMan->CloseWin();
//                }
//                else
//                {
//                    guildMan->SetShowGuildMain();
//                    guildMan->SEND_CG_GUILD_TOTALINFO_REQ_SYN();
//                }
//            }
//        }
//        break;
//    case eHOTKEY_PETDLG:
//        {
//            if (g_pHero->IsPetReturn())
//            {
//                g_HeroInput.ToggleShowPetInfo();
//            }
//        }
//        break;
//    case eHOTKEY_QUESTDLG:
//        {
//            uiQuestMan* pQuestMan = 
//                (uiQuestMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
//
//            if (pQuestMan)
//            {
//                pQuestMan->OpenQuestList();
//            }	
//        }
//        break;
//    case eHOTKEY_RADARDLG:
//        {
//            BattleScene::ShowRadirDialog();
//        }
//        break;
//    case eHOTKEY_FULLMAPDLG:
//        {
//            if (GlobalFunc::IsSSQScene() == FALSE)
//            {
//                RadarFullDialog* pRadarFullDlg = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
//                if (pRadarFullDlg)
//                {
//                    pRadarFullDlg->IncreaseRadarState();
//                    if (eFULL_RADAR_STATE_MAX != pRadarFullDlg->GetRadarState())
//                    {
//                        pRadarFullDlg->ShowDialog(TRUE);
//                    }
//                    else
//                    {
//                        pRadarFullDlg->ShowDialog(FALSE);
//                    }
//                }
//            }
//        }
//        break;
//    case eHOTKEY_WORLDMAPDLG:
//        {
//            uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
//            if (ui_radar_manager != NULL)
//            {
//                ui_radar_manager->ToggleShowContinentMap();
//            }
//        }
//        break;
//    case eHOTKEY_EXTEND_INVENTORY_DLG:
//        {
//            ExtendInventoryDialog* extend_inventory_dialog = 
//                GET_CAST_DIALOG(ExtendInventoryDialog, IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG);
//            if (extend_inventory_dialog != NULL)
//            {
//                extend_inventory_dialog->ShowExtendInventory();
//            }
//        }
//        break;
//    case eHOTKEY_CHATTING_DLG:
//        {
//            if (g_Input.GetState(DIK_RETURN, KS_DOWN) || g_Input.GetState(DIK_NUMPADENTER, KS_DOWN))
//            {
//                ChatDialog* pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
//                if (pDlg && !pDlg->IsAlwaysOnTop())
//                {			  
//                    if (!pDlg->m_preShow && !pDlg->IsVisibleWindow())			
//                    {
//                        pDlg->ShowDialog(TRUE);
//                    }
//                    else
//                    {
//                        pDlg->m_preShow=FALSE;
//                    }
//                }
//            }
//        }
//        break;
//    }
//}
//
//VOID HotKeyManager::SystemFunction(eHOTKEYCODE code)
//{
//    switch (code)
//    {
//    case eHOTKEY_SYSTEM_QUICK_CHANGE:
//        {
//            g_HeroInput.PressQuickSlotKey();
//        }
//        break;
//
//    case eHOTKEY_SYSTEM_QUICK_EXTEND:
//        {
//            g_HeroInput.ToggleChangeSlotSize();//사이즈변경
//        }
//        break;
//    }
//}
//
//VOID HotKeyManager::ExcuteHotKey( eHOTKEYCODE code, eOPE_TYPE opeType )
//{
//    switch( opeType )
//    {
//    case OPERATION_TOGGLE_DIALOG_WITHOUT_FULLMAP:
//        {
//            if( NULL == datas[ code ] ) return;
//
//            SolarDialog* dialog = static_cast<SolarDialog*>(datas[code]);
//            BOOL val = dialog->IsVisibleWindow();
//
//            // 단축키와 호환하는 각 다이얼로그 버튼 On/Off 설정
//            /*case eHOTKEY_CHARACTERSKILLDLG:
//            case eHOTKEY_INVENTORYDLG:
//            case eHOTKEY_ACDLG:
//            case eHOTKEY_OPTIONDLG:*/
//            if( g_pHero && g_pHero->IsShowFullMap() == FALSE )
//            {
//                dialog->ShowDialog( val ^= TRUE );
//            }
//        }
//        break;
//    case OPERATION_TOGGLE_DIALOG_FUNCTION:
//        {
//            ShowDialogFunction(code);
//        }
//        break;
//
//    case OPERATION_QUICK_SLOT:
//        {
//            const int QUICK_MAX = 40;
//            int quickslot_tab_number = 0;
//            QuickContainer* quick_container = NULL;
//            if (g_pHero == NULL)
//            {
//                return;
//            }
//            quick_container = g_pHero->GetQuickContainer();
//            if (quick_container && quick_container->double_size())
//            {
//                quickslot_tab_number = quick_container->GetCurTabNum();
//            }
//
//            int index = code-eHOTKEY_QUICK_1;
//            index += quickslot_tab_number;
//            index %= QUICK_MAX;
//            MouseHandler::Instance()->ItemUseTransaction(SI_QUICK, index);
//        }
//        break;
//
//    case OPERATION_SELECT_PARTY:
//        {
//            int index = code-eHOTKEY_SELECT_PARTY_1;// eHOTKEY_SELECT_PARTY_SELF = -1
//            BattleScene::ProcessKeyInput_PartyMember(index);
//        }
//        break;
//
//    case OPERATION_SYSTEM:
//        {
//            SystemFunction(code);
//        }
//        break;
//    }
//}
//
