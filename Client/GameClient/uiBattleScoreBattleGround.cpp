#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "BattleScene.h"
#include "uiBattleScoreBattleGround.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"

extern const int c_iListHeight;
const int battle_ground_score_list_width[uiBattleScoreBattleGround::REPORT_LIST_MAX] =
{
    50,//REPORT_LIST_RANK = 0,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        28, //REPORT_LIST_CLASS
        192,//    REPORT_LIST_ID,
#else
        220,//    REPORT_LIST_ID,
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        60,//    REPORT_LIST_LV,
        60,//    REPORT_LIST_KILL,
        60,//    REPORT_LIST_DIE,
        120,//    REPORT_LIST_GIVEDAMAGE,
        120,//    REPORT_LIST_GETDAMAGE,
        90,//    REPORT_LIST_POINT_ATTACK,   // 거점 공격
        90,//    REPORT_LIST_POINT_DEFFENCE, // 거점 방어
};

//------------------------------------------------------------------------------
uiBattleScoreBattleGround::uiBattleScoreBattleGround(InterfaceManager* manager_ptr)
:uiBase(manager_ptr),
is_init_(false),
select_tap_(TAB_BUTTON_ALL_TEAM)
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_	= INVALID_HANDLE_VALUE;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreBattleGround::~uiBattleScoreBattleGround(void)
{
    Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBattleGround::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    
    battle_score_manager_ = NULL;
    score_member_array.Clear();

    battle_players_list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (battle_players_list_control_)
    {
        battle_players_list_control_->ShowWindowWZ(WZ_SHOW);
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    assert(texture_information_);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    is_init_ = true;
}
//------------------------------------------------------------------------------
/** 다이얼로그 윈도우띄우기
*/
void uiBattleScoreBattleGround::OnShowWindow(BOOL val)
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Battle_Score;
        msg.wParam = IM_BATTLESCORE_MANAGER::DLG_BT_GROUND;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        
        for (int i = kControl_CheckBT_C000; i < kControl_CheckBT_C003; ++i)
        {
            CCtrlButtonCheckWZ* check_button =
                static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000 + i));
            if (check_button)
            {
                check_button->SetCheckState(false);
            }
        }

        if (select_tap() == TAB_BUTTON_ALL_TEAM)
        {
            CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000));
            if (check_button)
            {
                check_button->SetCheckState(true);
            }
        }
        else if (select_tap() == TAB_BUTTON_HOME_TEAM)
        {
            CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001));
            if (check_button)
            {
                check_button->SetCheckState(true);
            }
        }
        else //if (select_tap() == TAB_BUTTON_AWAY_TEAM)
        {
            CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C003));
            if (check_button)
            {
                check_button->SetCheckState(true);
            }
        }
        
        battle_score_manager()->RefreshBattleGroundList();
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Battle_Score);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBattleGround::Release()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

//-----------------------------------------------------------------------------
/** 메시지 PROC
*/
void uiBattleScoreBattleGround::MessageProc(SI_MESSAGE* pMessage)
{
    bool order_by_sort = true;
    int sort_by_id = -1;

    switch (pMessage->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch (GetControlPosition_byID(pMessage->dwCtrlID))
            {
            case kControl_Button_B007:
                {
                    //exit_battle_ground();
                }
                //break;
            case kControl_Button_B012:
                {
                    this->ShowInterface(FALSE);
                }
                break;
            case kControl_Button_B002:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_ID) // 버튼이 같을때
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_ID;
                }
                break;
            case kControl_Button_B001:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_LV)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_LV;
                }
                break;
            case kControl_Button_B005:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE;
                }
                break;
            case kControl_Button_B006:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE;
                }
                break;
            case kControl_Button_B003:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_KILL)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_KILL;
                }
                break;
            case kControl_Button_B010:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK;
                }
                break;
            case kControl_Button_B011:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE;
                }
                break;
            case kControl_CheckBT_C000:
                {
                    for (int i = kControl_CheckBT_C000; i <= kControl_CheckBT_C003; ++i)
                    {
                        CCtrlButtonCheckWZ* check_button =
                            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
                        if (check_button)
                        {
                            check_button->SetCheckState(false);
                        }
                    }

                    CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(pMessage->dwCtrlID));
                    if (check_button)
                    {
                        check_button->SetCheckState(true);
                    }
                    set_select_tap(TAB_BUTTON_ALL_TEAM);
                }
                break;
            case kControl_CheckBT_C001:
                {
                    for (int i = kControl_CheckBT_C000; i <= kControl_CheckBT_C003; ++i)
                    {
                        CCtrlButtonCheckWZ* check_button =
                            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
                        if (check_button)
                        {
                            check_button->SetCheckState(false);
                        }
                    }

                    CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(pMessage->dwCtrlID));
                    if (check_button)
                    {
                        check_button->SetCheckState(true);
                    }
                    set_select_tap(TAB_BUTTON_HOME_TEAM);
                }
                break;
            case kControl_CheckBT_C003:
                {
                    for (int i = kControl_CheckBT_C000; i <= kControl_CheckBT_C003; ++i)
                    {
                        CCtrlButtonCheckWZ* check_button =
                            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
                        if (check_button)
                        {
                            check_button->SetCheckState(false);
                        }
                    }

                    CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(pMessage->dwCtrlID));
                    if (check_button)
                    {
                        check_button->SetCheckState(true);
                    }

                    set_select_tap(TAB_BUTTON_AWAY_TEAM);
                }
                break;
            }//switch
        }
        break;
    }//switch
    if (sort_by_id != -1)
    {
        battle_score_manager()->SetSortBy(sort_by_id);
        battle_score_manager()->SetOrderBy(order_by_sort);
        //battle_score_manager()->SortbyList();
    }
    battle_score_manager()->RefreshBattleGroundList();
}

//------------------------------------------------------------------------------
/**
*/
uiBattleScoreMan* uiBattleScoreBattleGround::battle_score_manager()
{
    if (battle_score_manager_)
    {
        return this->battle_score_manager_;
    }
    else
    {
        this->battle_score_manager_ = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
        ASSERT(battle_score_manager_);
        return this->battle_score_manager_;
    }
}

void uiBattleScoreBattleGround::OnRenderSolarDialog()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));

    RECT rect;
    float x = 0.0f;
    float y = 0.0f;

    if (list_control)
    {
        rect = list_control->GetSizeRect();
    }
    else
    {
        this->GetDialogWZ()->GetClientRect(&rect);
    }
    x = static_cast<float>(rect.left) + battle_ground_score_list_width[uiBattleScoreBattleGround::REPORT_LIST_RANK];
    y = static_cast<float>(rect.top) + 4; /* 4 pixel fix */

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    BYTE team = eHomeTeam;

    for (int i = 0; i < score_member_array.Size(); ++i)
    {
        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(score_member_array[i].byClassCode);
        GenderType::Value gender = static_cast<GenderType::Value>(score_member_array[i].gender);
        eCHAR_TYPE race_type = ResourceCode::GetImageClassType(class_type, gender);

        //종족
        g_pSunRenderer->RenderTexture( 
            handle_texture_,
            x,
            y + (i * c_iListHeight),
            14.0f,
            14.0f,
            730.0f + ((race_type - eCHAR_BERSERKER) * 14),
            297.0f,
            14.0f,
            14.0f,
            NULL,
            0.0f
            );

        if (tooltip_manager && IsInRect(x, (y + (i * c_iListHeight)), 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetRaceString(race_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }

        //직업
        g_pSunRenderer->RenderTexture( 
            handle_texture_,
            x + 14,
            y + (i * c_iListHeight),
            14.0f,
            14.0f,
            730.0f + ((class_type - eCHAR_BERSERKER) * 14),
            297.0f + 14.0f,
            14.0f,
            14.0f,
            NULL,
            0.0f
            );

        if (tooltip_manager && IsInRect((x+14), (y + (i * c_iListHeight)), 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetClassString(class_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

//=====================================================================================
//	1.리스트컨트롤 데이터삽입
//=====================================================================================
void uiBattleScoreBattleGround::set_list_controls_data(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline)
{
    if (battle_players_list_control_ == NULL)
    {
        return;
    }
    DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// 기본값

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (nSubIndex == REPORT_LIST_ID)
    {
        nTextStyle = DT_VCENTER | DT_LEFT;	// 왼쪽 정렬
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    if (!bIsOnline && nSubIndex == REPORT_LIST_ID)
    {   // 오프라인이면 빨강
        GlobalFunc::SetItem(battle_players_list_control_, nIndex, nSubIndex,
            pszText,
            c_wiListFont,
            nTextStyle,
            RGBA(255,255,255,0),
            RGBA(255,0,0,255), 
            RGBA(255,0,0,255));
    }
    else
    {   // 정상이면 흰색
        GlobalFunc::SetItem(battle_players_list_control_, nIndex, nSubIndex,
            pszText,
            c_wiListFont,
            nTextStyle,
            RGBA(255,255,255,0),
            RGBA(255,255,255,255),
            RGBA(255,255,255,255));
    }
}
//=====================================================================================
//	2.컨트롤 갱신
//=====================================================================================
void uiBattleScoreBattleGround::refresh_list_control_data(nArray<BATTLESCORE_DATA> &ArrayType)
{
    if(!is_init_)
    {
        return;
    }
    if (battle_players_list_control_ == NULL)
    {
        return;
    }
    score_member_array = ArrayType; // OnRenderSolarDialog() 에서 텍스처 출력 용으로 보관해둠

    BATTLESCORE_DATA battler_data;
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    int array_count = ArrayType.Size();

    //0.리스트 목록정보삽입
    battle_players_list_control_->DeleteItemAll();
    battle_players_list_control_->SetBeginDrawIndex(0);

    if (array_count <= 0)
    {
        return;
    }

    battle_players_list_control_->SetSellHeight(c_iListHeight);
    battle_players_list_control_->SetUseReport(true);
    battle_players_list_control_->SetReportUseCnt(BATTLE_GROUND_REPORT_LIST::REPORT_LIST_MAX);
    battle_players_list_control_->SetHideReportRender(true);
    battle_players_list_control_->SetFontID(c_wiListFont);

    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_RANK, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_RANK],
        _T("rank"));
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_CLASS, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_CLASS],
        _T("class"));
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_ID, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_ID],
        _T("id"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_LV, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_LV],
        _T("lv"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL],
        _T("kill"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_DIE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_DIE],
        _T("die"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEDAMAGE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEDAMAGE],
        _T("givedamage"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GETDAMAGE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GETDAMAGE],
        _T("getdamage"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_ATTACK, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_ATTACK],
        _T("attack"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_DEFFENCE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_DEFFENCE],
        _T("deffence"));


    //2.데이터삽입
    int iAdd = 0;
    int row_index = 0;
    int sort_index = 0;
    int class_code = 0;
    int index = 0;
    for (int row_index = 0; row_index < array_count; ++row_index)
    {
        index = battle_players_list_control_->InsertItemBack();
        battle_players_list_control_->SetItemData(index, &(ArrayType[row_index]));
        ++iAdd;
        for (sort_index=0; sort_index < REPORT_LIST_MAX; ++sort_index)
        {
            ZeroMemory(temp_string, sizeof(temp_string));
            switch(sort_index)
            {
            case REPORT_LIST_RANK:	// rank
                Snprintf(temp_string,INTERFACE_STRING_LENGTH-1,_T("%d"), (row_index+1));
                break;
            case REPORT_LIST_ID:
                {
                    TCHAR *pName=(TCHAR*)ArrayType[row_index].name.Get();
                    Snprintf(temp_string,INTERFACE_STRING_LENGTH-1,_T("%s"),pName);				
                }
                break;
            case REPORT_LIST_LV:
                Snprintf(temp_string,INTERFACE_STRING_LENGTH-1, "%d", ArrayType[row_index].wLevel);				
                break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            case REPORT_LIST_CLASS://클래스
                {	
                    continue; //아이콘으로 대체
                }
                break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT가
            case REPORT_LIST_KILL:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].wKillCount);
                break;
            case REPORT_LIST_DIE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].wDeadCount);
                break;
            case REPORT_LIST_GIVEDAMAGE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalDamageToEnemy);
                break;
            case REPORT_LIST_GETDAMAGE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalDamageByEnemy);
                break;
            case REPORT_LIST_POINT_ATTACK:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].strongpoint_attack);
                break;
            case REPORT_LIST_POINT_DEFFENCE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].strongpoint_defense);
                break;
            }
            set_list_controls_data(row_index, sort_index, temp_string, ArrayType[row_index].bConnected);
        }
    }
}

void uiBattleScoreBattleGround::InitInfo()
{
    // 전장 나가기 버튼 비활성화
    show_exit_button(false);
}

void uiBattleScoreBattleGround::show_exit_button(bool is_show)
{
    CCtrlButtonWZ* exit_button =
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B007));
    if (exit_button)
    {
        exit_button->ShowWindowWZ(is_show ? WZ_SHOW : WZ_HIDE);
    }
}

void uiBattleScoreBattleGround::exit_battle_ground() // 전장 떠나기
{
    BattleScene::BackToTheVillageSyn();
}
#endif//_NA_0_20091118_GAMEROOM_BATTLE_RESULT