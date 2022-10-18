#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "BattleScene.h"
#include "uiBattleScoreBattleGround.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Map.h"
#include "ChaosSystem/ChaosSystemData.h"

extern const int c_iListHeight;
const int battle_ground_score_list_width[uiBattleScoreBattleGround::REPORT_LIST_MAX] =
{
#ifdef _RUSSIA // ���þ��� dialog�� ���� �����ؼ� ����Ѵ�. 
        50, // ����
        28, // Ŭ�����׸�
        127,// ĳ���͸�                 
        55, // ����         
        150,// ų/����/��ý�Ʈ
        78, // ����ų
        120,// �Ѱ��ص�����
        90, // ������
        95, // ��������
        110,// �������
#else
        50,//REPORT_LIST_RANK = 0,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        28, //REPORT_LIST_CLASS
        192,//    REPORT_LIST_ID,
#else
        220,//    REPORT_LIST_ID,
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        55,//    REPORT_LIST_LV,
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        100,
#else
        67,//    REPORT_LIST_KILL_DIE,
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        58,//    REPORT_LIST_SERIES_KILL,
        120,//    REPORT_LIST_GIVEDAMAGE,
        120,//    REPORT_LIST_GIVEHEAL,
        90,//    REPORT_LIST_POINT_ATTACK,   // ���� ����
        90,//    REPORT_LIST_POINT_DEFFENCE, // ���� ���
#endif // _RUSSIA
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
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);

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

    scroll_control_ = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
    if (scroll_control_)
    {
        scroll_control_->SetScrollPosWZ(0);
        scroll_control_->SetScrollRangeWZ(0,1);
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    assert(texture_information_);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    is_init_ = true;
}
//------------------------------------------------------------------------------
/** ���̾�α� ���������
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
/** �޽��� PROC
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
                    exit_battle_ground();
                }
                break;
            case kControl_Button_B012:
                {
                    this->ShowInterface(FALSE);
                }
                break;
            case kControl_Button_B002:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_ID) // ��ư�� ������
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_ID;
                }
                break;
            case kControl_Button_B001:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_LV)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_LV;
                }
                break;
            case kControl_Button_B004:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_SERIES_KILL)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_SERIES_KILL;
                }
                break;
            case kControl_Button_B005:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE;
                }
                break;
            case kControl_Button_B006:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_HEAL)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_HEAL;
                }
                break;
            case kControl_Button_B003:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_KILL)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_KILL;
                }
                break;
            case kControl_Button_B010:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
                    }
                    sort_by_id = uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK;
                }
                break;
            case kControl_Button_B011:
                {
                    if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE)
                    {
                        order_by_sort = !battle_score_manager()->GetOrderBy();	// ������ �ʿ�
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
                    battle_score_manager()->RefreshBattleGroundList();
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
                    battle_score_manager()->RefreshBattleGroundList();
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
                    battle_score_manager()->RefreshBattleGroundList();
                }
                break;
            }//switch

            if (sort_by_id != -1)
            {
                battle_score_manager()->SetSortBy(sort_by_id);
                battle_score_manager()->SetOrderBy(order_by_sort);
                //battle_score_manager()->SortbyList();
                battle_score_manager()->RefreshBattleGroundList();
            }
        }
        break;
        case RT_MSG_SB_LINETOP:
            {
                if (GetControlPosition_byID(pMessage->dwCtrlID) == kControl_VScroll_V000)
                {
                    OnVScrollUp(pMessage);
                }
            }
            break;
        case RT_MSG_SB_LINEBOTTOM:
            {
                if (GetControlPosition_byID(pMessage->dwCtrlID) == kControl_VScroll_V000)
                {
                    OnVScrollDown(pMessage);
                }
            }
            break;
        case RT_MSG_SB_THUMBTRACK:      //!< ���� �巡�� �ϰ� �ִ�.
        case RT_MSG_SB_THUMBPOSITION:   //!< ���� �巡������ ���콺�� ���Ҵ�.
        case RT_MSG_SB_ENDSCROLL:       //!< ��ũ���� ���� �Ǿ���.
            {
                if (GetControlPosition_byID(pMessage->dwCtrlID) == kControl_VScroll_V000)
                {
                    OnVScrollThumb(pMessage);
                }
            }
            break;
    }//switch

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

    for (int i = 0; (i < list_control->GetDrawRowCount()) && (i < score_member_array.Size()); ++i)
    {
        int list_index = vscroll_control()->GetScrollPosWZ() + i;
        if (list_index >= score_member_array.Size())
        {
            break;
        }
        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(score_member_array[list_index].byClassCode);
        GenderType::Value gender = static_cast<GenderType::Value>(score_member_array[list_index].gender);
        eCHAR_TYPE race_type = ResourceCode::GetImageClassType(class_type, gender);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // �̹��� ��ġ�� ������� �Ǿ����� �ʾ� �ϵ��ڵ�
        if (class_type == eCHAR_WITCHBLADE)
        {
            //����
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x,
                y + (i * c_iListHeight),
                14.0f,
                14.0f,
                730.0f + ((eCHAR_MYSTIC - eCHAR_BERSERKER) * 14),
                297.0f + 28.0f,
                14.0f,
                14.0f,
                NULL,
                0.0f
                );
        }
        else
        {
            //����
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
        }
#else
        //����
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
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if (tooltip_manager && IsInRect(x, (y + (i * c_iListHeight)), 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetRaceString(race_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // �̹��� ��ġ�� ������� �Ǿ����� �ʾ� �ϵ��ڵ�
        // ����
        if ( class_type == eCHAR_WITCHBLADE )
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x + 14,
                y + (i * c_iListHeight),
                14.0f,
                14.0f,
                730.0f + ((eCHAR_HELLROID - eCHAR_BERSERKER) * 14),
                297.0f + 28.0f,
                14.0f,
                14.0f,
                NULL,
                0.0f
                );
        }
        else
        {
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
        }
#else
        //����
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
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if (tooltip_manager && IsInRect((x+14), (y + (i * c_iListHeight)), 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetClassString(class_type));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

//=====================================================================================
//	1.����Ʈ��Ʈ�� �����ͻ���
//=====================================================================================
void uiBattleScoreBattleGround::set_list_controls_data(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline)
{
    if (battle_players_list_control_ == NULL)
    {
        return;
    }
    DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// �⺻��

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (nSubIndex == REPORT_LIST_ID)
    {
        nTextStyle = DT_VCENTER | DT_LEFT;	// ���� ����
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    if (nSubIndex == REPORT_LIST_ID)
    {
        COLORREF team_color = RGBA(255,255,255,255);
        if (bIsOnline == false)
        {
            // ���������̸� ����
            team_color = RGBA(255,0,0,255);
        }
        else if (battle_score_manager()->FindMemberToName(pszText, eHomeTeam) != -1)
        {
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
            team_color = RGBA(0,204,205,255);
#else
            // Ȩ���̸�
            if (g_pHero->GetTeam() == kRed)
            {
                team_color = RGBA(255,102,204,255);
            }
            else
            {
                team_color = RGBA(0,204,205,255);
            }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        }
        else
        {
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
            team_color = RGBA(255,102,204,255);
#else
            // �����̸�
            if (g_pHero->GetTeam() == kRed)
            {
                team_color = RGBA(0,204,205,255);
            }
            else
            {
                team_color = RGBA(255,102,204,255);
            }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        }

        GlobalFunc::SetItem(battle_players_list_control_, nIndex, nSubIndex,
            pszText,
            c_wiListFont,
            nTextStyle,
            RGBA(255,255,255,0),
            team_color,
            team_color);
    }
    else
    {
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
//	2.��Ʈ�� ����
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
    score_member_array = ArrayType; // OnRenderSolarDialog() ���� �ؽ�ó ��� ������ �����ص�

    BATTLESCORE_DATA battler_data;
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    int array_count = ArrayType.Size();

    if (vscroll_control())
    {
        vscroll_control()->SetScrollPosWZ(0);
    }

    //0.����Ʈ �����������
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
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL_DIE_ASSIST, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL_DIE_ASSIST],
        _T("kill_die_assist"));
#else
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL_DIE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_KILL_DIE],
        _T("kill_die"));
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_SERIES_KILL, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_SERIES_KILL],
        _T("series_kill"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEDAMAGE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEDAMAGE],
        _T("givedamage"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEHEAL, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_GIVEHEAL],
        _T("getheal"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_ATTACK, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_ATTACK],
        _T("attack"));
    GlobalFunc::SetReportButtonInfo(battle_players_list_control_,
        BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_DEFFENCE, 
        battle_ground_score_list_width[BATTLE_GROUND_REPORT_LIST::REPORT_LIST_POINT_DEFFENCE],
        _T("deffence"));


    //2.�����ͻ���
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
            case REPORT_LIST_CLASS://Ŭ����
                {	
                    continue; //���������� ��ü
                }
                break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT��
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
            case REPORT_LIST_KILL_DIE_ASSIST:
#else
            case REPORT_LIST_KILL_DIE:
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                {
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                    Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%d / %d / %d",
                        ArrayType[row_index].wKillCount, ArrayType[row_index].wDeadCount, ArrayType[row_index].wAssistCount);
#else
                    Snprintf(temp_string,INTERFACE_STRING_LENGTH-1, "%d / %d",
                        ArrayType[row_index].wKillCount, ArrayType[row_index].wDeadCount);
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                }
                break;
            case REPORT_LIST_SERIES_KILL:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].series_kill);
                break;
            case REPORT_LIST_GIVEDAMAGE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalDamageToEnemy);
                break;
            case REPORT_LIST_GIVEHEAL:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalHealToParty);
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

#if WZENGINEVER >= 400
    int max_line_count = battle_players_list_control_->GetListRowCnt();
    int draw_line_count = battle_players_list_control_->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
#endif //#if WZENGINEVER >= 400
}

void uiBattleScoreBattleGround::InitInfo()
{
    // ���� ������ ��ư ��Ȱ��ȭ
    enable_exit_button(false);
    // ����Ʈ â�� �ڱ� ������
    //

    // 2596 �ҿ���ð�:(%s �� %s �� ���)
    TCHAR message_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(2596, temp_string, INTERFACE_STRING_LENGTH);
    Snprintf(message_string, INTERFACE_STRING_LENGTH-1, temp_string, " 0", " 0");

    CControlWZ* text_control = static_cast<CControlWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (text_control)
    {
        text_control->SetTextWZ(message_string);
    }
}

void uiBattleScoreBattleGround::enable_exit_button(bool is_enable)
{
    CCtrlButtonWZ* exit_button =
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B007));
    if (exit_button)
    {
        exit_button->EnableWindowWZ(is_enable);
    }
}

void uiBattleScoreBattleGround::exit_battle_ground() // ���� ������
{
    BattleScene::BackToTheVillageSyn();
}

void uiBattleScoreBattleGround::OnUpdateSolarDialog()
{
    VScrollDialog::Update();

    DWORD battle_time = ChaosSystem::GetBattleGroundData().GetBattleTime();
    battle_time /= 1000;

    //hour = battle_time / 3600;
    battle_time %= 3600;
    int minute = battle_time / 60;
    battle_time %= 60;
    int second = battle_time % 60;

    TCHAR message_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR minute_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR second_string[INTERFACE_STRING_LENGTH] = {0,};

    // 2596 �ҿ���ð�:(%s �� %s �� ���)
    g_InterfaceManager.GetInterfaceString(2596, temp_string, INTERFACE_STRING_LENGTH);
    Snprintf(minute_string, INTERFACE_STRING_LENGTH-1, "%2d", minute);
    Snprintf(second_string, INTERFACE_STRING_LENGTH-1, "%2d", second);
    Snprintf(message_string, INTERFACE_STRING_LENGTH-1, temp_string, minute_string, second_string);

    CControlWZ* text_control = static_cast<CControlWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (text_control)
    {
        text_control->SetTextWZ(message_string);
    }
}
CCtrlVScrollWZ* uiBattleScoreBattleGround::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
bool uiBattleScoreBattleGround::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattleScoreBattleGround::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattleScoreBattleGround::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
bool uiBattleScoreBattleGround::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
#endif//_NA_0_20091118_GAMEROOM_BATTLE_RESULT