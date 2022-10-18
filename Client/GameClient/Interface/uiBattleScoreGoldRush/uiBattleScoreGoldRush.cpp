#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include ".\uibattlescoregoldrush.h"
#include "GlobalFunc.h"
#include "AppearanceResourceCode.h"
#include "uiTooltipMan/uiToolTipMan.h"
#include "BattleScene.h"

extern const int c_iListHeight;
extern const int c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAX];
const int gold_rush_score_list_width[uiBattleScoreGoldRush::GOLD_REPORT_LIST_MAX] = 
{
    50,  // 순위
    28,  // 클래스그림
    158, // 캐릭터명
    60,  // 레벨
    140, // 골드 보유량
    175, // 킬/어시스트/데스
    150, // 총 가해 데미지량
    122, // 총 힐량
};

//------------------------------------------------------------------------------
WzID uiBattleScoreGoldRush::wz_id_array_[BTSCORE_MAX] = 
{
    StrToWzID("B999"),	//BTSCORE_BTN_EXIT=0,
    StrToWzID("BT00"),	//BTSCORE_BTN_RANK01,
    StrToWzID("BT60"),	//BTSCORE_BTN_ID01,    
    StrToWzID("BT01"),	//BTSCORE_BTN_LV01,
    StrToWzID("BT02"),	//BTSCORE_BTN_MAXGOLD01,
    StrToWzID("BT07"),	//BTSCORE_BTN_KILL01,
    StrToWzID("BT03"),	//BTSCORE_BTN_GIVEDAMAGE01,
    StrToWzID("BT05"),	//BTSCORE_BTN_HEAL01,    
    StrToWzID("S002"),	//BTSCORE_TXT_TOTALGOLD01,

    StrToWzID("B000"),    // BTSCORE_BTN_RANK02,
    StrToWzID("B001"),    // BTSCORE_BTN_ID02,
    StrToWzID("B002"),    // BTSCORE_BTN_LV02,
    StrToWzID("B003"),    // BTSCORE_BTN_MAXGOLD02,
    StrToWzID("B004"),    // BTSCORE_BTN_KILL02,
    StrToWzID("B005"),    // BTSCORE_BTN_GIVEDAMAGE02,
    StrToWzID("B006"),    // BTSCORE_BTN_HEAL02,
    StrToWzID("S003"),    // BTSCORE_TXT_TOTALGOLD02,    

    /*
    StrToWzID("PI00"),	//BTSCORE_PIC_01,
    StrToWzID("PI02"),	//BTSCORE_PIC_02,
    StrToWzID("ST05"),	//BTSCORE_WINTXT_01,
    StrToWzID("ST03"),	//BTSCORE_WINTXT_02,
    */

    StrToWzID("L000"),	//BTSCORE_LIST01,
    StrToWzID("L001"),	//BTSCORE_LIST02,

    StrToWzID("B007"),  //BTSCORE_BTN_LEAVE_GOLDRUSH,
};

uiBattleScoreGoldRush::uiBattleScoreGoldRush(InterfaceManager* manager_ptr) : 
uiBase(manager_ptr),
is_init_dialog_(FALSE),
is_inited_(false)
{
    handle_texture_ = INVALID_HANDLE_VALUE;
}

uiBattleScoreGoldRush::~uiBattleScoreGoldRush(void)
{
    Release();
}

void uiBattleScoreGoldRush::Init( CDrawBase* draw_base_ptr )
{
    uiBase::Init(draw_base_ptr);

    InitControls();

    battle_score_manager_ptr_ = NULL;

    InitInfo();

    for (int i = 0; i < BTSCORE_LST_MAX; ++i)
    {
        list_ctrls_ptr_[i] = static_cast<CCtrlListWZ*>(this->getControl(BTSCORE_LIST01+i));
        SUN_ASSERT(list_ctrls_ptr_[i]);

        if (list_ctrls_ptr_[i])
            list_ctrls_ptr_[i]->ShowWindowWZ(WZ_SHOW);
    }

    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\common_bg01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_infomation_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    SUN_ASSERT(texture_infomation_);
}

void uiBattleScoreGoldRush::OnShowWindow( BOOL val )
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_Battle_Score;
        msg.wParam = IM_BATTLESCORE_MANAGER::DLG_BT_GOLDRUSH;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        is_show_ = true;        
    }
    else
    {
        is_show_ = false;
        g_KeyQueueManager.DeleteMsg(eDoType_Battle_Score);        
    }
}

void uiBattleScoreGoldRush::InitInfo()
{
    // 전장 나가기 버튼 비활성화
    enable_exit_button(false);

    if (is_inited_)
    {        
        TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        // 최초는 승수 모두 0
        if (total_gold_point_[eHomeTeam])
        {
            // 2795 팀골드 : %s            
            Snprintf(temp_string, INTERFACE_STRING_LENGTH, "%d", 0);
            g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2795, temp_string);
            total_gold_point_[eHomeTeam]->SetTextWZ(result_string);
        }
        if (total_gold_point_[eAwayTeam])
        {
            // 2795 팀골드 : %s            
            Snprintf(temp_string, INTERFACE_STRING_LENGTH, "%d", 0);
            g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2795, temp_string);
            total_gold_point_[eRedTeam]->SetTextWZ(result_string);
        }        
    }
}

void uiBattleScoreGoldRush::RefreshInfo()
{
    if (!g_pHero)
    {
        return;
    }
    if (is_inited_)
    {
        TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        // 최초는 승수 모두 0
        if (total_gold_point_[eHomeTeam])
        {
            // 2795 팀골드 : %s
            GlobalFunc::SetPointText(temp_string, GetManager()->get_team_gold_point(eHomeTeam));
            g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2795, temp_string);
            total_gold_point_[eHomeTeam]->SetTextWZ(result_string);
        }
        if (total_gold_point_[eAwayTeam])
        {
            // 2795 팀골드 : %s
            GlobalFunc::SetPointText(temp_string, GetManager()->get_team_gold_point(eAwayTeam));
            g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2795, temp_string);
            total_gold_point_[eRedTeam]->SetTextWZ(result_string);
        }
    }
}

void uiBattleScoreGoldRush::InitControls()
{
    for (int i = 0; i < BTSCORE_LST_MAX; ++i)
    {
        list_ctrls_ptr_[i] = static_cast<CCtrlListWZ*>(this->getControl(BTSCORE_LIST01+i));
        SUN_ASSERT(list_ctrls_ptr_[i]);

        if (list_ctrls_ptr_[i])
        {
            list_ctrls_ptr_[i]->ShowWindowWZ(WZ_HIDE);
        }
    }    

    total_gold_point_[eHomeTeam] = static_cast<CCtrlStaticWZ*>(this->getControl(BTSCORE_TXT_TOTALGOLD01));
    assert(total_gold_point_[eHomeTeam]);
    if (total_gold_point_[eHomeTeam])
    {
        total_gold_point_[eHomeTeam]->SetStyle(DT_LEFT);
        total_gold_point_[eHomeTeam]->SetTextColor(RGBA(22, 155, 250, 255));
    }
    total_gold_point_[eAwayTeam] = static_cast<CCtrlStaticWZ*>(this->getControl(BTSCORE_TXT_TOTALGOLD02));
    if (total_gold_point_[eAwayTeam])
    {
        total_gold_point_[eAwayTeam]->SetStyle(DT_LEFT);
        total_gold_point_[eAwayTeam]->SetTextColor(RGBA(255, 0, 0, 255));
    }
    assert(total_gold_point_[eAwayTeam]);
    
    is_inited_ = true;
}

VOID uiBattleScoreGoldRush::Release()
{
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
    }
}

VOID uiBattleScoreGoldRush::MessageProc( SI_MESSAGE* message_ptr )
{
    switch (message_ptr->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch (getCtrlIDToPos(message_ptr->dwCtrlID))
            {
            case BTSCORE_BTN_LEAVE_GOLDRUSH:
                {
                    exit_gold_rush();
                }
                break;
            case BTSCORE_BTN_EXIT:
                {
                    this->ShowInterface(FALSE);
                }
                break;
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message_ptr);
        }
    }
}

void uiBattleScoreGoldRush::_clearSelection()
{
    if(GetManager())
    {
        GetManager()->ClearSelection();	
        if(list_ctrls_ptr_[BTSCORE_LST_LIST01])	// 팀 1
            list_ctrls_ptr_[BTSCORE_LST_LIST01]->SetCurSel(-1);
        if(list_ctrls_ptr_[BTSCORE_LST_LIST02])	// 팀 2
            list_ctrls_ptr_[BTSCORE_LST_LIST02]->SetCurSel(-1);
    }
}

uiBattleScoreMan* uiBattleScoreGoldRush::GetManager()
{
    if(battle_score_manager_ptr_)
        return this->battle_score_manager_ptr_;
    else
    {
        this->battle_score_manager_ptr_ =
            static_cast<uiBattleScoreMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
        SUN_ASSERT (battle_score_manager_ptr_);

        return this->battle_score_manager_ptr_;
    }
}

void uiBattleScoreGoldRush::OnRenderSolarDialog()
{    
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    RECT rect;
    float x = 0.0f;
    float y = 0.0f;

    for (int team =  eHomeTeam; team < eTeamCNT; ++team)
    {
        if (list_ctrls_ptr_[team])
        {
            rect = list_ctrls_ptr_[team]->GetSizeRect();
        }
        else
        {
            this->GetDialogWZ()->GetClientRect(&rect);
        }
        x = static_cast<float>(rect.left) + gold_rush_score_list_width[uiBattleScoreGoldRush::GOLD_REPORT_LIST_RANK];
        y = static_cast<float>(rect.top) + 4; /* 4 pixel fix */
        //y = static_cast<float>(rect.top);

        for (int i = 0; i < GetManager()->m_BTSMemberArray[team].Size(); ++i)
        {
            eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(GetManager()->m_BTSMemberArray[team][i].byClassCode);
            GenderType::Value gender = static_cast<GenderType::Value>(GetManager()->m_BTSMemberArray[team][i].gender);
            eCHAR_TYPE race_type = ResourceCode::GetImageClassType(class_type, gender);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            // 이미지 위치가 순서대로 되어있지 않아 하드코딩
            //종족
            if ( class_type == eCHAR_WITCHBLADE )
            {                
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
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            if (tooltip_manager && IsInRect(x, (y + (i * c_iListHeight)), 14.0f, 14.0f))
            {
                tooltip_manager->RegisterTooltip(GlobalFunc::GetRaceString(race_type));
                tooltip_manager->SetTooltipDialogID( GetDialogKey() );
            }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            // 이미지 위치가 순서대로 되어있지 않아 하드코딩
            // 직업
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
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            if (tooltip_manager && IsInRect((x+14), (y + (i * c_iListHeight)), 14.0f, 14.0f))
            {
                tooltip_manager->RegisterTooltip(GlobalFunc::GetClassString(class_type));
                tooltip_manager->SetTooltipDialogID( GetDialogKey() );
            }
        }
    }
}

void uiBattleScoreGoldRush::_RefreshListCtrlData( nArray<BATTLESCORE_DATA>& ArrayType, E_TEAM eTeamNum )
{
    if(!is_inited_)
    {
        return;
    }

    BATTLESCORE_DATA stData;
    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

    //1.카운팅
    int cnt = ArrayType.Size();
    //0.리스트 목록정보삽입
    list_ctrls_ptr_[eTeamNum]->DeleteItemAll();
    list_ctrls_ptr_[eTeamNum]->SetBeginDrawIndex(0);

    if(cnt <= 0)
    {
        return;
    }

    list_ctrls_ptr_[eTeamNum]->SetSellHeight(c_iListHeight);
    list_ctrls_ptr_[eTeamNum]->SetUseReport(true);
    list_ctrls_ptr_[eTeamNum]->SetReportUseCnt(GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_MAX);
    list_ctrls_ptr_[eTeamNum]->SetHideReportRender(true);
    list_ctrls_ptr_[eTeamNum]->SetFontID(c_wiListFont);    

    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_RANK,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_RANK],
        _T("rank"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_CLASS,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_CLASS],
        _T("class"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_ID,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_ID],
        _T("id"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_LV,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_LV],
        _T("lv"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GOLD_POINT,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GOLD_POINT],
        _T("gold_point"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_KILL_DIE_ASSIST,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_KILL_DIE_ASSIST],
        _T("kill_die_assist"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GIVEDAMAGE,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GIVEDAMAGE],
        _T("givedamage"));
    GlobalFunc::SetReportButtonInfo(list_ctrls_ptr_[eTeamNum],
        GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GIVEHEAL,
        gold_rush_score_list_width[GOLD_RUSH_REPORT_LIST::GOLD_REPORT_LIST_GIVEHEAL],
        _T("giveheal"));

    //2.데이터삽입
    int iAdd = 0;
    for(int iLine = 0; iLine < cnt; ++iLine)
    {
        int index = list_ctrls_ptr_[eTeamNum]->InsertItemBack();
        list_ctrls_ptr_[eTeamNum]->SetItemData(index, &(ArrayType[iLine]));
        ++iAdd;
        for(int iCnt=0; iCnt<eBTSSORT_MAX; ++iCnt)
        {
            ZeroMemory(szTemp, sizeof(szTemp));

            switch (iCnt)
            {
            case GOLD_REPORT_LIST_RANK:
                {
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%d"), (iLine+1));
                }
                break;
            case GOLD_REPORT_LIST_ID:
                {
                    TCHAR* name_string = (TCHAR*)(ArrayType[iLine].name.Get());
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), name_string);
                }
                break;
            case GOLD_REPORT_LIST_LV:
                {
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%d"), ArrayType[iLine].wLevel);

                }
                break;
            case GOLD_REPORT_LIST_CLASS:
                {
                    // 아이콘으로 대체
                    continue;
                }
                break;
            case GOLD_REPORT_LIST_KILL_DIE_ASSIST:
                {
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, "%d / %d / %d",
                        ArrayType[iLine].wKillCount, ArrayType[iLine].wDeadCount, ArrayType[iLine].wAssistCount);
                }
                break;
            case GOLD_REPORT_LIST_GIVEDAMAGE:
                {
                    GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwTotalDamageToEnemy);
                }
                break;
            case GOLD_REPORT_LIST_GIVEHEAL:
                {
                    GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwTotalHealToParty);
                }
                break;
            case GOLD_REPORT_LIST_GOLD_POINT:
                {
                    GlobalFunc::SetPointText(szTemp, ArrayType[iLine].gold_point_);
                }
                break;
            }
            _SetListCtrlData((enum BTSCORE_LSTS)(BTSCORE_LST_LIST01+eTeamNum), iLine, iCnt, szTemp, 
                ArrayType[iLine].bConnected);
        }
    }
    RefreshInfo();
}

void uiBattleScoreGoldRush::_SetListCtrlData( BTSCORE_LSTS eListNum, int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline )
{
    DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// 기본값

    if (nSubIndex == uiBattleScoreMan::BTSCROE_LIST_ID)
    {
        nTextStyle = DT_VCENTER | DT_LEFT;	// 왼쪽 정렬
    }

    if (nSubIndex == GOLD_REPORT_LIST_ID)
    {
        COLORREF team_color = RGBA(255, 255, 255, 255);
        if (bIsOnline == false)
        {
            // 오프라인이면 빨강색
            team_color = RGBA(255,0,0,255);
        }
        else if (GetManager()->FindMemberToName(pszText, eHomeTeam) != -1)
        {
            team_color = RGBA(0,204,205,255);
        }
        else
        {
            team_color = RGBA(255,102,204,255);
        }

        GlobalFunc::SetItem(list_ctrls_ptr_[eListNum], nIndex, nSubIndex,
            pszText,
            c_wiListFont,
            nTextStyle,
            RGBA(255,255,255,0),
            team_color,
            team_color);
    }
    else
    {
        GlobalFunc::SetItem(list_ctrls_ptr_[eListNum], nIndex, nSubIndex,
            pszText,
            c_wiListFont,
            nTextStyle,
            RGBA(255,255,255,0),
            RGBA(255,255,255,255),
            RGBA(255,255,255,255));
    }
}

void uiBattleScoreGoldRush::enable_exit_button( const bool is_enable )
{
    CCtrlButtonWZ* exit_button_ptr = 
        static_cast<CCtrlButtonWZ*>(this->getControl(BTSCORE_BTN_LEAVE_GOLDRUSH));
    if (exit_button_ptr)
    {
        exit_button_ptr->EnableWindowWZ(is_enable);
    }
}

void uiBattleScoreGoldRush::exit_gold_rush()
{
    BattleScene::BackToTheVillageSyn();
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH