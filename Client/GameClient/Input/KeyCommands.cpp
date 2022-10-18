#include "SunClientPrecompiledHeader.h"

#include "KeyCommands.h"
#include "HotKeyInfoParser.h"
#include "InterfaceManager.h"
#include "UIActionFunction.h"
#include "Hero.h"
#include "MouseHandler.h"
#include "HeroActionInput.h"
#include "Map.h"
#include "Camera.h"
#include "BattleScene.h"
#include "ScreenShotSaver.h"
#include "uiChatMan/uiChatMan_def.h"
#include "ChatDialog.h"

//////////////////////////////////////////////////////////////////////////
bool CheckBindKeyInput(KEY_BIND_PAIR& key_bind_pair, int key_state, bool force_input = false)
{
    int ext_key_state = KS_KEY;
    int real_key_state;

    BYTE shift_key = (g_Input.GetState(DIK_LSHIFT, ext_key_state, force_input) != FALSE) ? 1 : 0;
    BYTE ctrl_key = (g_Input.GetState(DIK_LCONTROL, ext_key_state, force_input) != FALSE) ? 1 : 0;
    BYTE alt_key = (g_Input.GetState(DIK_LALT, ext_key_state, force_input) != FALSE) ? 1 : 0;
    WORD key_code = key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].key_code_;

    if ((key_code != 0) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].shift_key_ == shift_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].ctrl_key_ == ctrl_key) && 
        (key_bind_pair.hot_key_code[KEY_BIND_PAIR::kMainHotKey].alt_key_ == alt_key))
    {
        if (key_code >= MAX_KEY_BUFFER)
        {
            real_key_state = key_state<<8;
        }
        else
        {
            real_key_state = key_state;
        }

        if (g_Input.GetState(key_code, real_key_state, force_input) != FALSE)
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
        if (key_code >= MAX_KEY_BUFFER)
        {
            real_key_state = key_state<<8;
        }
        else
        {
            real_key_state = key_state;
        }

        if (g_Input.GetState(key_code, real_key_state, force_input) != FALSE)
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
            if (key_code >= MAX_KEY_BUFFER)
            {
                real_key_state = key_state<<8;
            }
            else
            {
                real_key_state = key_state;
            }

            if (g_Input.GetState(key_code, real_key_state, force_input) != FALSE)
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
            if (key_code >= MAX_KEY_BUFFER)
            {
                real_key_state = key_state<<8;
            }
            else
            {
                real_key_state = key_state;
            }

            if (g_Input.GetState(key_code, real_key_state, force_input) != FALSE)
            {
                return true;
            }
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
// DialogToggleCommand - 다이얼로그 토글
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void DialogToggleCommand::RegisterActionFunction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                                 ActionFunction action_function_ptr)
{
    action_function_container().insert(ActionFunctionMap::value_type(hot_key_function, action_function_ptr));
}

void DialogToggleCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                   KEY_BIND_PAIR key_bind_pair)
{
    ActionFunctionMap::iterator found_itr = action_function_container().find(hot_key_function);
    if (found_itr == action_function_container().end())
    {
        return;
    }

    if (CheckBindKeyInput(key_bind_pair, KS_UP) == false)
    {
        return;
    }

    // 전체맵이 열린 상태에서 다른 UI의 동작을 하지 않도록 함.
    if ((hot_key_function != kModeChange_FullRadar_UI) && 
        (hot_key_function != kOpen_SystemMenu_N_Cancel))
    {
        if ((g_pHero != NULL) && 
            (g_pHero->IsShowFullMap() != FALSE))
        {
            return;
        }
    }

    ActionFunction action_function_ptr = found_itr->second;
    if (action_function_ptr == NULL)
    {
        return;
    }

    action_function_ptr();
}

//////////////////////////////////////////////////////////////////////////
void SystemMenuNCancelCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                        KEY_BIND_PAIR key_bind_pair)
{
    if ((CheckBindKeyInput(key_bind_pair, KS_UP, TRUE) == false) && 
        (CheckBindKeyInput(key_bind_pair, IKS_UP, TRUE) == false))
    {
        return;
    }

    UIActionFunction::ShowSystemMenuNCancel();
}

//////////////////////////////////////////////////////////////////////////
void QuickSlotModeCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                    KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_UP) == false)
    {
        return;
    }

    if (hot_key_function == kExpand_QuickSlotBar)
    {
        UIActionFunction::ToggleQuickSlotExtend();
    }
    else if (hot_key_function == kExchange_QuickSlotBar)
    {
        UIActionFunction::ExchangeQuickSlot();
    }
}

//////////////////////////////////////////////////////////////////////////
void QuickSlotUseCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                   KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    POSTYPE quick_slot_position = static_cast<POSTYPE>(hot_key_function - kUse_QuickSlot1);
    MouseHandler::Instance()->ItemUseTransaction(SI_QUICK, quick_slot_position);
}

//////////////////////////////////////////////////////////////////////////
void SummonControlCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                    KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    switch (hot_key_function)
    {
    case kSelect_MySummonMonster:
        {
            if (g_pHero != NULL)
            {
                DWORD summon_id = g_pHero->GetSummonId();
                if (summon_id != NONE_ID)
                {
                    g_HeroInput.SetCurrentTarget(summon_id);
                }
            }            
        }
        break;
    case kUse_SummonSkill1:
    case kUse_SummonSkill2:
    case kUse_SummonSkill3:
    case kUse_SummonSkill4:
    case kUse_SummonSkill5:
    case kUse_SummonSkill6:
    case kUse_SummonSkill7:
        {
            UIActionFunction::UseSummonSkill(hot_key_function - kUse_SummonSkill1);
        }
        break;
    case kOrderAttack_SummonMonster:
    case kOrderFollow_SummonMonster:
    case kAttackMode_SummonMonster:
    case kDefenceMode_SummonMonster:
    case kRelease_SummonMonster:
    case kOrderNonPKAttack_SummonMonster:
        {
            UIActionFunction::CommandSummon(hot_key_function - kOrderAttack_SummonMonster);
        }
        break;
    }
}

void TargetEnemyCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                  KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    if ((g_pMap != NULL) && 
        (g_pMap->IsSnowManEventMap() == FALSE))
    {
        SEARCH_TARGET_TYPE search_type = SEARCH_TARGET_MAX;
        if (hot_key_function == kSelect_TargetEnemy)
        {
            search_type = SEARCH_TARGET_NEAREST;
        }
        else if (hot_key_function == kNext_TargetEnemy)
        {
            search_type = SEARCH_TARGET_NEXT;
        }
        else if (hot_key_function == kPrev_TargetEnemy)
        {
            search_type = SEARCH_TARGET_PREVIOUS;
        }

        if (search_type != SEARCH_TARGET_MAX)
        {
            g_HeroInput.OnSearchTarget(search_type);
        }
    }
}

void TargetPartyMemberCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                        KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    int party_member_index = -1;
    switch (hot_key_function)
    {
    case kSelect_Hero:
        {
            party_member_index = -1;
        }
        break;
    case kSelect_PartyMember1:
    case kSelect_PartyMember2:
    case kSelect_PartyMember3:
    case kSelect_PartyMember4:
    case kSelect_PartyMember5:
    case kSelect_PartyMember6:
    case kSelect_PartyMember7:
    case kSelect_PartyMember8:
    case kSelect_PartyMember9:
        {
            party_member_index = hot_key_function - kSelect_PartyMember1;
        }
        break;
    }
    BattleScene::ProcessKeyInput_PartyMember(party_member_index);
}

void ScreenShotCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                 KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    switch (hot_key_function)
    {
    case kScreenShot:
        {
            if (ScreenShotSaver::Instance()->IsPrepared() == false)
            {
                ScreenShotSaver::Instance()->Prepare();
            }
        }
        break;
    case kScreenShotMode:
        {
            if (GENERALPARAM->GetScreenShotMode() != 0)
            {
                GENERALPARAM->SetScreenShotMode(0);
            }
            else
            {
                GENERALPARAM->SetScreenShotMode(1);
            }

            BOOL screenshot_mode = !g_pHero->IsScreenShotMode();
            if (screenshot_mode != FALSE)
            {
                BattleScene::SetScreenShotState(eSCREEN_SHOT_NOT_UI_AND_HERO);
                g_InterfaceManager.ShowInterface(FALSE);
            }
            else
            {
                BattleScene::SetScreenShotState(eSCREEN_SHOT_FULL);
                g_InterfaceManager.ShowInterface(TRUE );
            }
            g_pHero->SetScreenShotMode(screenshot_mode);
        }
        break;
    case kHide_InterfaceAll:
        {
            BOOL interface_show_flag = g_InterfaceManager.GetShowFlag();
            interface_show_flag = !interface_show_flag;
            g_InterfaceManager.ShowInterface(interface_show_flag);
        }
        break;
    }
}

void ChatOpenNSendCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                    KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if ((chat_dialog != NULL) && 
        (chat_dialog->IsAlwaysOnTop() == FALSE))
    {			  
        if ((chat_dialog->m_preShow == FALSE) && 
            (chat_dialog->IsVisibleWindow() == FALSE))
        {
            chat_dialog->ShowDialog(TRUE);
        }
        else
        {
            chat_dialog->m_preShow=FALSE;
        }
    }
}

void CameraControlCommand::OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, 
                                    KEY_BIND_PAIR key_bind_pair)
{
    if (CheckBindKeyInput(key_bind_pair, KS_KEY) == false)
    {
        return;
    }

    const int zoom_speed = 120;
    const int rotate_speed = 30;

    switch (hot_key_function)
    {
    case kZoom_In:
        {
            g_Camera.MoveFrontBack(zoom_speed);
        }
        break;
    case kZoom_Out:
        {
            g_Camera.MoveFrontBack(-zoom_speed);
        }
        break;
    case kRotate_Left_Camera:
        {
            g_Camera.Rotate(-rotate_speed, 0, Camera::CAMERA_ROTATE_RMOUSE);
        }
        break;
    case kRotate_Right_Camera:
        {
            g_Camera.Rotate(rotate_speed, 0, Camera::CAMERA_ROTATE_RMOUSE);
        }
        break;
    }
}

void HeroAutoRunCommand::OnAction( HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair )
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == false)
    {
        return;
    }

    BOOL is_auto_run = g_HeroInput.GetAutoRun();
    g_HeroInput.SetAutoRun(!is_auto_run);
    g_HeroInput.SetFollowState(FALSE);
}

void HeroMoveCommand::OnAction( HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair )
{
    bool key_pressed = false;
    if (CheckBindKeyInput(key_bind_pair, KS_KEY) == TRUE)
    {
        key_pressed = true;
    }

    switch (hot_key_function)
    {
    case kMove_Forward:
        g_HeroInput.set_forward_key_pressed(key_pressed);
        break;
    case kMove_Backward:
        g_HeroInput.set_backward_key_pressed(key_pressed);
        break;
    case kMove_LeftSide:
        g_HeroInput.set_left_step_key_pressed(key_pressed);
        break;
    case kMove_RightSide:
        g_HeroInput.set_right_step_key_pressed(key_pressed);
        break;
    case kTurn_Left:
        g_HeroInput.set_left_turn_key_pressed(key_pressed);
        break;
    case kTurn_Right:
        g_HeroInput.set_right_turn_key_pressed(key_pressed);
        break;
    }
}

void HeroJumpCommand::OnAction( HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair )
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == TRUE)
    {
        g_HeroInput.set_jump_key_pressed(true);
    }
}

void HeroActionSkillCommand::OnAction( HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair )
{
    SLOTCODE action_code = HotKeyInfoParser::Instance()->GetUserActionCode(hot_key_function);
    if (action_code == 0)
    {
        return;
    }

    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == TRUE)
    {
        g_HeroInput.ProcessUserAction(action_code);
    }
}

void HeroSpreadWingCommand::OnAction( HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair )
{
    if (CheckBindKeyInput(key_bind_pair, KS_DOWN) == TRUE)
    {
        g_HeroInput.set_spread_wing_key_pressed(true);
    }
    else
    {
        g_HeroInput.set_spread_wing_key_pressed(false);
    }
}
