#include "SunClientPrecompiledHeader.h"
#include "uibattlescoreblood.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"

extern const int c_iListHeight;
extern const int c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAX];

//------------------------------------------------------------------------------
WzID uiBattleScoreBlood::controls_id_[BATTLE_SCORE_MAX] = 
{
	StrToWzID("B999"),	//BATTLE_SCORE_BUTTON_EXIT,
	StrToWzID("L001"),	//BATTLE_SCORE_LIST,
	StrToWzID("T999"),	//BATTLE_SCORE_TXT_TITLE,
	StrToWzID("ST00"),	//BATTLE_SCORE_TXT_DISCRIPT,
	StrToWzID("BT60"),	//BATTLE_SCORE_BUTTON_ID,
	StrToWzID("BT63"),	//BATTLE_SCORE_BUTTON_CLASS,
	StrToWzID("BT00"),	//BATTLE_SCORE_BUTTON_RANK,
	StrToWzID("BT01"),	//BATTLE_SCORE_BUTTON_LV,
	StrToWzID("BT02"),	//BATTLE_SCORE_BUTTON_MAXDAMAGE,
	StrToWzID("BT03"),	//BATTLE_SCORE_BUTTON_GIVEDAMAGE,
	StrToWzID("BT04"),	//BATTLE_SCORE_BUTTON_GETDAMAGE,
	StrToWzID("BT07"),	//BATTLE_SCORE_BUTTON_KILL,
	StrToWzID("BT05"),	//BATTLE_SCORE_BUTTON_HEAL,
	StrToWzID("BT08"),	//BATTLE_SCORE_BUTTON_DIE,
    StrToWzID("CT00"),	//BATTLE_SCORE_CHECK_BUTTON_HOME,
    StrToWzID("CT02"),	//BATTLE_SCORE_CHECK_BUTTON_AWAY,
};


uiBattleScoreBlood::uiBattleScoreBlood(InterfaceManager* manager_ptr)
:uiBase(manager_ptr),
 m_bInitDlg(FALSE),
 m_bIsInited(false),
 home_team_selected_(true)
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_	= INVALID_HANDLE_VALUE;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreBlood::~uiBattleScoreBlood(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBlood::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	InitControls();
	battle_score_manager_ = NULL;
    set_home_team_selected(true);
	if (battle_players_list_control_)
	{
		battle_players_list_control_->ShowWindowWZ(WZ_SHOW);
	}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    assert(texture_information_);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBlood::InitControls()
{
    int index = 0;
	battle_players_list_control_ = static_cast<CCtrlListWZ* >(this->get_control(BATTLE_SCORE_LIST));
	SUN_ASSERT(battle_players_list_control_);
	if (battle_players_list_control_)
	{
		battle_players_list_control_->ShowWindowWZ(WZ_HIDE);
	}
    for (index=0; index<TAB_BUTTON_MAX; ++index)
    {
        team_check_button[index] = 
            static_cast<CCtrlButtonCheckWZ*>(this->get_control(BATTLE_SCORE_CHECK_BUTTON_HOME+index));
		ASSERT(team_check_button[index]);
    }
    if (team_check_button[TAB_BUTTON_HOME_TEAM])
    {
        team_check_button[TAB_BUTTON_HOME_TEAM]->SetCheckState(false);
    }
    if (team_check_button[TAB_BUTTON_AWAY_TEAM])
    {
        team_check_button[TAB_BUTTON_AWAY_TEAM]->SetCheckState(true);
    }
	for (index=0; index<uiBattleScoreMan::BTSCROE_LIST_MAX; ++index)
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        if (index == uiBattleScoreMan::BTSCROE_LIST_CLASS)
        {
            // ui변경으로 사라짐
            list_title_button[index] = NULL;
            continue;
        }
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		list_title_button[index] = 
            static_cast<CCtrlButtonWZ*>(this->get_control(BATTLE_SCORE_BUTTON_ID+index));
		ASSERT(list_title_button[index]);
	}
    m_bIsInited = true;
}
//------------------------------------------------------------------------------
/** 다이얼로그 윈도우띄우기
*/
void uiBattleScoreBlood::OnShowWindow(BOOL val)
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Battle_Score;
		msg.wParam = IM_BATTLESCORE_MANAGER::DLG_BT_BLOOD;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
        //uiBase::MoveCenter();
        if (home_team_selected())
        {
            team_check_button[TAB_BUTTON_HOME_TEAM]->SetCheckState(true);
            team_check_button[TAB_BUTTON_AWAY_TEAM]->SetCheckState(false);
		    refresh_list_control_data(battle_score_manager()->m_BTSMemberArray[eHomeTeam]);
        }
        else
        {
            team_check_button[TAB_BUTTON_HOME_TEAM]->SetCheckState(false);
            team_check_button[TAB_BUTTON_AWAY_TEAM]->SetCheckState(true);
		    refresh_list_control_data(battle_score_manager()->m_BTSMemberArray[eAwayTeam]);
        }
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Battle_Score);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBlood::Release()
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
void uiBattleScoreBlood::MessageProc(SI_MESSAGE* pMessage)
{
	bool order_by_sort = true;
    int sort_by_id = -1;

	switch (pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
		switch (get_control_id_to_position(pMessage->dwCtrlID))
		{
		case BATTLE_SCORE_BUTTON_EXIT:
			{
				this->ShowInterface(FALSE);
			}
			break;
		case BATTLE_SCORE_BUTTON_ID:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_ID) // 버튼이 같을때
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_ID;
			}
			break;
		case BATTLE_SCORE_BUTTON_LV:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_LV)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
                sort_by_id = uiBattleScoreMan::BTSCROE_LIST_LV;
			}
			break;
		case BATTLE_SCORE_BUTTON_CLASS:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_CLASS)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_CLASS;
			}
			break;
		case BATTLE_SCORE_BUTTON_MAXDAMAGE:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE;
			}
			break;
		case BATTLE_SCORE_BUTTON_GIVEDAMAGE:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
                sort_by_id = uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE;				
			}
			break;
		case BATTLE_SCORE_BUTTON_GETDAMAGE:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE;
			}
			break;
		case BATTLE_SCORE_BUTTON_KILL:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_KILL)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_KILL;
			}
			break;
		case BATTLE_SCORE_BUTTON_HEAL:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_HEAL)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_HEAL;
			}
			break;
		case BATTLE_SCORE_BUTTON_DIE:
			{
				if (battle_score_manager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_DIE)
                {
					order_by_sort = !battle_score_manager()->GetOrderBy();	// 반전이 필요
                }
				sort_by_id = uiBattleScoreMan::BTSCROE_LIST_DIE;
			}
			break;
        case BATTLE_SCORE_CHECK_BUTTON_HOME:
            {
                if (home_team_selected() == false)
                {
                    set_home_team_selected(true);
                    if (battle_score_manager() != NULL)
                    {
                        battle_score_manager()->set_toggle_ui_open(true);
                        battle_score_manager()->SendPacketBattleResultSYN();
                    }
                }
                team_check_button[TAB_BUTTON_HOME_TEAM]->SetCheckState(true);
                team_check_button[TAB_BUTTON_AWAY_TEAM]->SetCheckState(false);
            }
            break;
        case BATTLE_SCORE_CHECK_BUTTON_AWAY:
            {
                if (home_team_selected() == true)
                {
                    set_home_team_selected(false);
                    if (battle_score_manager() != NULL)
                    {
                        battle_score_manager()->set_toggle_ui_open(true);
                        battle_score_manager()->SendPacketBattleResultSYN();
                    }
                }
                team_check_button[TAB_BUTTON_HOME_TEAM]->SetCheckState(false);
                team_check_button[TAB_BUTTON_AWAY_TEAM]->SetCheckState(true);
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
		battle_score_manager()->SortbyList();
        E_TEAM team = home_team_selected() ? eHomeTeam : eAwayTeam;
        refresh_list_control_data(battle_score_manager()->m_BTSMemberArray[team]);        
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreBlood::clear_selected()
{
	if (battle_score_manager())
	{
		battle_score_manager()->ClearSelection();	
		if (battle_players_list_control_)
        {
			battle_players_list_control_->SetCurSel(-1);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreMan* uiBattleScoreBlood::battle_score_manager()
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

void uiBattleScoreBlood::OnRenderSolarDialog()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(this->get_control(BATTLE_SCORE_LIST));

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
    x = static_cast<float>(rect.left) + c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_RANK] + 2; //'랭크'버튼 공간만큼 이동
    y = static_cast<float>(rect.top) + 4;

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    BYTE team = eHomeTeam;
    team = home_team_selected() ? eHomeTeam:eAwayTeam;
    for (int i = 0; i < battle_score_manager()->m_BTSMemberArray[team].Size(); ++i)
    {
        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(battle_score_manager()->m_BTSMemberArray[team][i].byClassCode);
        GenderType::Value gender = static_cast<GenderType::Value>(battle_score_manager()->m_BTSMemberArray[team][i].gender);
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

        if (tooltip_manager && IsInRect((x + 14), (y + (i * c_iListHeight)), 14.0f, 14.0f))
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
void uiBattleScoreBlood::set_list_controls_data(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline)
{
	DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// 기본값

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (nSubIndex == uiBattleScoreMan::BTSCROE_LIST_ID)
    {
        nTextStyle = DT_VCENTER | DT_LEFT;	// 왼쪽 정렬
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    if (!bIsOnline && nSubIndex == eBTSSORT_NAME)
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
void uiBattleScoreBlood::refresh_list_control_data(nArray<BATTLESCORE_DATA> &ArrayType)
{
  	if(!m_bIsInited)
	{
		return;
	}

	BATTLESCORE_DATA battler_data;
	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0,};
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
	battle_players_list_control_->SetReportUseCnt(uiBattleScoreMan::BTSCROE_LIST_MAX);
	battle_players_list_control_->SetHideReportRender(true);
	battle_players_list_control_->SetFontID(c_wiListFont);

	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_RANK, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_RANK], _T("rank"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_ID, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_ID], _T("name"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_LV, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_LV], _T("lv"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_CLASS, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_CLASS], _T("class"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE], _T("maxdmg"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE], _T("givedmg"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE], _T("getdmg"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_HEAL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_HEAL], _T("heal"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_KILL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_KILL], _T("kill"));
	GlobalFunc::SetReportButtonInfo(battle_players_list_control_, uiBattleScoreMan::BTSCROE_LIST_DIE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_DIE], _T("die"));

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
		for (sort_index=0; sort_index<eBTSSORT_MAX; ++sort_index)
		{
			ZeroMemory(temp_string, sizeof(temp_string));
			switch(sort_index)
			{
			case eBTSSORT_RANK:	// rank
				Snprintf(temp_string,INTERFACE_STRING_LENGTH-1,_T("%d"), (row_index+1));
				break;
			case eBTSSORT_NAME:
				{
				TCHAR *pName=(TCHAR*)ArrayType[row_index].name.Get();
    			Snprintf(temp_string,INTERFACE_STRING_LENGTH-1,_T("%s"),pName);				
				}
				break;
			case eBTSSORT_LV:
				Snprintf(temp_string,INTERFACE_STRING_LENGTH-1, "%d", ArrayType[row_index].wLevel);				
				break;
			case eBTSSORT_CLASS://클래스
				{	
					Snprintf(temp_string, 
                        INTERFACE_STRING_LENGTH-1, 
                        _T("%s"), 
                        GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(ArrayType[row_index].byClassCode)));
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                    continue;//아이콘으로 대체
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT가
				}
				break;
			case eBTSSORT_MAXDMG:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwMaxDamageToEnemy);
				break;
			case eBTSSORT_GIVEDMG:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalDamageToEnemy);
				break;
			case eBTSSORT_GETDMG:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalDamageByEnemy);
				break;
			case eBTSSORT_HEAL:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].dwTotalHealToParty);
				break;
			case eBTSSORT_KILL:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].wKillCount);
				break;
			case eBTSSORT_DIE:
                GlobalFunc::SetPointText(temp_string, ArrayType[row_index].wDeadCount);
				break;
			}
			set_list_controls_data(row_index, sort_index, temp_string, ArrayType[row_index].bConnected);
		}
	}
}
