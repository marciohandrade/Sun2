#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookView.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "NPC.h"
#include "Monster.h"
#include "PlayerMonster.h"
#include "Mouse.h"
#include "MonsterBookParser.h"
#include "NPCInfoParser.h"
#include "NpcImageInfo.h"
#include "GameFunc.h"

uiMonsterBookView::uiMonsterBookView(InterfaceManager* ui_manager)
{
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);    // lv %s
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);    // 이름
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);    // hp
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);    // mp
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);    // 공격력
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);    // 방어력
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);    // 공격속도
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);    // 스킬공격력
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);    // 확인된몬스터 %s/%s

    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
   
    is_active_monster_ = FALSE;
}
//------------------------------------------------------------------------------
uiMonsterBookView::~uiMonsterBookView()
{
}
//------------------------------------------------------------------------------
void uiMonsterBookView::Init(CDrawBase* draw_base)
{
    uiMonsterBookBase::Init(draw_base);

    screen_info().reset();
}
//------------------------------------------------------------------------------
void uiMonsterBookView::Release()
{
    uiMonsterBookBase::Release();

    Object* object = g_ObjectManager.GetObject(MONSTERBOOK_PREVIEW_CHARACTER_KEY);
    if (object != NULL)
    {
        g_ObjectManager.Delete(MONSTERBOOK_PREVIEW_CHARACTER_KEY);
    }
    
}
//------------------------------------------------------------------------------
void uiMonsterBookView::NetworkProc(struct MSG_BASE *message)
{
    uiMonsterBookBase::NetworkProc(message);
}
//------------------------------------------------------------------------------
void uiMonsterBookView::Process(DWORD tick)
{
    if ((GetControlWZ_byPosition(kControl_Button_B000)->GetState() == WZ_SHOW) && 
        (IsOverControl(GetControlWZ_byPosition(kControl_Button_B000)) == true) && 
        (Mouse::Instance()->LButtonPressed() == TRUE))
    {
        TurnRight(tick * 0.002f);
    }
    else if ((GetControlWZ_byPosition(kControl_Button_B001)->GetState() == WZ_SHOW) && 
        (IsOverControl(GetControlWZ_byPosition(kControl_Button_B001)) == true) &&
        (Mouse::Instance()->LButtonPressed() == TRUE))
    {
        TurnLeft(tick * 0.002f);
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookView::OnRenderSolarDialog()
{
    uiMonsterBookBase::OnRenderSolarDialog();

    if (is_active_monster_ == TRUE)
    {
        Monster* monster_object = static_cast<Monster*>(g_ObjectManager.GetObject(MONSTERBOOK_PREVIEW_CHARACTER_KEY));
        if (monster_object != NULL)
        {
            monster_object->SetShow(TRUE);

            CControlWZ* control = GetControlWZ_byPosition(kControl_Picture_P016);
            if (control)
            {
                g_InterfaceManager.RenderMonsterOnRect(
                    monster_object,
                    control->GetSizeRect(),
                    screen_info().rot,
                    screen_info().org,
                    screen_info().zoom);
            }
            monster_object->SetShow(FALSE);
        }

        GetControlWZ_byPosition(kControl_Picture_P013)->OnDraw(true);
        GetControlWZ_byPosition(kControl_Button_B000)->OnDraw(true);
        GetControlWZ_byPosition(kControl_Button_B001)->OnDraw(true);
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookView::OnUpdateSolarDialog()
{
    uiMonsterBookBase::OnUpdateSolarDialog();

    //------------------------------------------------------------------------------ 
    if ((IsOverControl(GetControlWZ_byPosition(kControl_Picture_P016)) == true) &&
        (g_InterfaceManager.IsLockDlg() == FALSE))
    {
        RECT area = GetControlWZ_byPosition(kControl_Picture_P016)->GetSizeRect();
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
        case MOUSE_WHEEL_UP:    screen_info().zoom -= (0.1f * screen_info().zoom); break;
        case MOUSE_WHEEL_DOWN:  screen_info().zoom += (0.1f * screen_info().zoom); break;
        } 
        if (Mouse::Instance()->LButtonPressed() == TRUE)
        {
            screen_info().org.x -= ((Mouse::Instance()->GetMouseX() - screen_info().mouse_pos.x) * 0.005f);
            screen_info().org.y += ((Mouse::Instance()->GetMouseY() - screen_info().mouse_pos.y) * 0.005f);
        }
        screen_info().check_valid_value();
    }
    screen_info().mouse_pos.x = Mouse::Instance()->GetMouseX();
    screen_info().mouse_pos.y = Mouse::Instance()->GetMouseY();
}
//------------------------------------------------------------------------------
void uiMonsterBookView::OnShowWindow(BOOL is_show)
{
    uiMonsterBookBase::OnShowWindow(is_show);

    if (is_show)
    {
        set_tap_control(MonsterBookTap_View);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_MonsterBook_View;
        msg.wParam = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW;
        msg.DoSomething = GlobalFunc::DoMonsterBookDialog;
        g_KeyQueueManager.PushBack(msg);

        Monster* monster = static_cast<Monster*>(g_ObjectManager.GetObject(MONSTERBOOK_PREVIEW_CHARACTER_KEY));
        if (monster == NULL)
        {
            CreateMonster(current_window_state_.select_monster_);
        }

    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_MonsterBook_View);
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookView::MessageProc(SI_MESSAGE* message)
{
    uiMonsterBookBase::MessageProc(message);

    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

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


//------------------------------------------------------------------------------
bool uiMonsterBookView::OnLButtonClick(SI_MESSAGE* message)
{
    if (GetMan() == NULL)
    {
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
void uiMonsterBookView::SetMonsterInfo(DWORD monster_code)
{
    current_window_state_.select_monster_ = monster_code;
    select_monster_code_ = monster_code;

    BASE_NPCINFO* monster_info = NPCInfoParser::Instance()->GetNPCInfo(monster_code);

    if (monster_info == NULL)
    {
        return;
    }

    // 탭에 맞는 몬스터정보 세팅
    const MonsterBookInfo* book_info = MonsterBookParser::Instance()->FindMonsterBookInfo(monster_code);
    if (book_info)
    {
        is_active_monster_ = book_info->active_;
    }
    if (is_active_monster_ == TRUE)
    {
        // 미리보기 몬스터 생성
        Monster* monster = static_cast<Monster*>(g_ObjectManager.GetObject(MONSTERBOOK_PREVIEW_CHARACTER_KEY));
        if (monster)
        {
            g_ObjectManager.Delete(MONSTERBOOK_PREVIEW_CHARACTER_KEY);
            monster = NULL;
        }
        CreateMonster(monster_code);
        screen_info().reset();

        // 배경 이미지
        CControlWZ* control = GetControlWZ_byPosition(kControl_Picture_P010);
        if (control)
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
        control = GetControlWZ_byPosition(kControl_Picture_P013);
        if (control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ_byPosition(kControl_Button_B000);
        if (control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ_byPosition(kControl_Button_B001);
        if (control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
    }
    else
    {
        // 배경 이미지
        CControlWZ* control = GetControlWZ_byPosition(kControl_Picture_P010);
        if (control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ_byPosition(kControl_Picture_P013);
        if (control)
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
        control = GetControlWZ_byPosition(kControl_Button_B000);
        if (control)
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
        control = GetControlWZ_byPosition(kControl_Button_B001);
        if (control)
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
    }
    
    // 몬스터 이름
    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR grade_string[INTERFACE_STRING_LENGTH] = {0,};

    BOOL bGrade = TRUE;
    switch (monster_info->m_byGrade)
    {
    case eNPC_GENERAL: //11501	일반
    case eNPC_MAPOBJECT_NPC:
        g_InterfaceManager.GetInterfaceString(11501, grade_string);
        break;
    case eNPC_ELITE: //11503	정예
        g_InterfaceManager.GetInterfaceString(11503, grade_string);
        break;
    case eNPC_LEADER: //11502	리더
        g_InterfaceManager.GetInterfaceString(11502, grade_string);
        break;
    case eNPC_MIDDLEBOSS: //11504	캡틴
        g_InterfaceManager.GetInterfaceString(11504, grade_string);
        break;
    case eNPC_BOSS: //11505	보스
        g_InterfaceManager.GetInterfaceString(11505, grade_string);
        break;
    case eNPC_LUCKY_MONSTER: //11500	럭키
        g_InterfaceManager.GetInterfaceString(11500, grade_string);
        break;
    case eNPC_MERCHANT_NPC:
        bGrade = FALSE;
        break;
    default:
        break;
    }

    // 몬스터 레벨 (등급)
    if (bGrade == TRUE)
    {
        Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d %s",
            monster_info->m_DisplayLevel, 
            grade_string);
    }
    else
    {
        Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d",
            monster_info->m_DisplayLevel);
    }

    // 3240 LV %s
    g_InterfaceManager.GetInterfaceString(3240, temp_string);
    Snprintf(result_string, INTERFACE_STRING_LENGTH-1, temp_string, temp_string2);

    CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S007));
    if (control)
    {
        control->SetTextWZ(result_string);
    }

    // 몬스터이름
    control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S018));
    if (control)
    {
        g_InterfaceManager.GetInterfaceString(monster_info->m_NCode, result_string);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, result_string, INTERFACE_STRING_LENGTH-1); 
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, monster_info->m_MonsterCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
        control->SetTextWZ(result_string);
    }
    

    // 기타 수치 설정
    if (is_active_monster_ == TRUE)
    {
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S013));
        if (control)
        {
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%d", monster_info->m_dwMaxHP);
            control->SetTextWZ(result_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S014));
        if (control)
        {
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%d", monster_info->m_dwMaxMP);
            control->SetTextWZ(result_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S015));
        if (control)
        {
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%d - %d",
                monster_info->m_dwMinAttackPower, monster_info->m_dwMaxAttackPower);
            control->SetTextWZ(result_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S016));
        if (control)
        {
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%d", monster_info->m_dwPhyDef);
            control->SetTextWZ(result_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S017));
        if (control)
        {
            float attack_speed = (monster_info->m_wAttSpeed + monster_info->m_wAttSpeed2) / 2.0f / 1000 / 1.5f; // 1.5는 CalcAttackSpeedRatio에서 발췌
            // 2990 초당 %s회
            g_InterfaceManager.GetInterfaceString(2990, temp_string);
            Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%.1f", attack_speed);
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, temp_string, temp_string2);
            control->SetTextWZ(result_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S006));
        if (control)
        {
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%d", monster_info->skill_power);
            control->SetTextWZ(result_string);
        } 
    }
    else
    {
        // 미확인은 ????로 표시
        TCHAR unknown_string[INTERFACE_STRING_LENGTH] = {0,};
        // 2989 ????
        g_InterfaceManager.GetInterfaceString(2989, unknown_string);

        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S013));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S014));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S015));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S016));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S017));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
        control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S006));
        if (control)
        {
            control->SetTextWZ(unknown_string);
        }
    }
}
void uiMonsterBookView::CreateMonster(DWORD monster_code)
{
    Monster* monster = static_cast<Monster*>(g_ObjectManager.GetObject(MONSTERBOOK_PREVIEW_CHARACTER_KEY));
    if (monster)
    {
        return;
    }

    BASE_NPCINFO* monster_info = NPCInfoParser::Instance()->GetNPCInfo(monster_code);
    if (monster_info == NULL)
    {
        return;
    }

    WzVector m_CameraFrom, m_CameraTo, m_Position;
    SetVector(&m_CameraFrom, 0.0, 0.0, 0.0);
    SetVector(&m_CameraTo, 0.0, 100.0, 0.0);
    SetVector(&m_Position, 0.0, 0.0, -100.0);

    if (monster_info->m_wNpcImageIndex)
    {
        // 플레이어형 몬스터
        BASE_NPCImageInfo* pNpcImageInfo =
            NPCImageInfoParser::Instance()->GetNpcImageInfo(monster_info->m_wNpcImageIndex);
        if (pNpcImageInfo)
        {
            PlayerMonster* pPlayerMonster = (PlayerMonster*)g_ObjectManager.Add(
                MONSTERBOOK_PREVIEW_CHARACTER_KEY, SSQMONSTER_OBJECT, pNpcImageInfo->bClassCode, FALSE);

            if (pPlayerMonster)
            {
                // 외형세팅
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_HAIR, pNpcImageInfo->hairImageID);
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_FACE, pNpcImageInfo->faceImageID);
                BOOL is_hide_helmet = (pNpcImageInfo->showhelmet == 1) ? false : true;
                pPlayerMonster->HideHelmet(is_hide_helmet, FALSE);
                NPC::InitNpcImageInfo(pPlayerMonster, monster_info);

                pPlayerMonster->RefreshEquipData();
                GameFunc::SetPlayerMonsterAttackStyle(pPlayerMonster);

                // 저탑 보스 등으로 등장하는 플레이어형몬스터의 경우 MONSTER_OBJECT 세팅
                pPlayerMonster->SetMonsterInfo(monster_info);

                pPlayerMonster->SetDirection( m_CameraFrom, m_CameraTo,TRUE);
                pPlayerMonster->SetPosition(m_Position);
            }
        }
    }
    else 
    {
        monster = static_cast<Monster*>(g_ObjectManager.Add(MONSTERBOOK_PREVIEW_CHARACTER_KEY, MONSTER_OBJECT, monster_code));
        monster->SetDirection(m_CameraFrom, m_CameraTo,TRUE);
        monster->SetPosition(m_Position);
    }
}
//------------------------------------------------------------------------------ 
void uiMonsterBookView::TurnLeft(float radin)
{
    screen_info().rot += radin;
    screen_info().check_valid_value();
}
//------------------------------------------------------------------------------ 
void uiMonsterBookView::TurnRight(float radin)
{
    screen_info().rot -= radin;
    screen_info().check_valid_value();
}
//------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------ 

#endif //_NA_007086_20140318_MONSTERBOOK
