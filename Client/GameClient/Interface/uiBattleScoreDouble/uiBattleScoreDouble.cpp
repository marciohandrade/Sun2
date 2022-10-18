#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else

#include "uibattlescoredouble.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "GlobalFunc.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"

extern const int c_iListHeight;
extern const int c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAX];

//------------------------------------------------------------------------------
WzID uiBattleScoreDouble::m_wzId[BTSCORE_MAX] = 
{
	StrToWzID("B999"),	//BTSCORE_BTN_EXIT=0,
	StrToWzID("BT60"),	//BTSCORE_BTN_ID01,
	StrToWzID("BT63"),	//BTSCORE_BTN_CLASS01,
	StrToWzID("BT00"),	//BTSCORE_BTN_RANK01,
	StrToWzID("BT01"),	//BTSCORE_BTN_LV01,
	StrToWzID("BT02"),	//BTSCORE_BTN_MAXDAMAGE01,
	StrToWzID("BT03"),	//BTSCORE_BTN_GIVEDAMAGE01,
	StrToWzID("BT04"),	//BTSCORE_BTN_GETDAMAGE01,
	StrToWzID("BT07"),	//BTSCORE_BTN_KILL01,
	StrToWzID("BT05"),	//BTSCORE_BTN_HEAL01,
	StrToWzID("BT08"),	//BTSCORE_BTN_DIE01,
	StrToWzID("ST05"),	//BTSCORE_TXT_WIN01,
	
	StrToWzID("BT06"),	//BTSCORE_BTN_DIE02,
	StrToWzID("BT09"),	//BTSCORE_BTN_HEAL02,
	StrToWzID("BT10"),	//BTSCORE_BTN_KILL02,
	StrToWzID("BT11"),	//BTSCORE_BTN_GETDAMAGE02,
	StrToWzID("BT12"),	//BTSCORE_BTN_GIVEDAMAGE02,
	StrToWzID("BT13"),	//BTSCORE_BTN_MAXDAMAGE02,
	StrToWzID("BT14"),	//BTSCORE_BTN_LV02,
	StrToWzID("BT15"),	//BTSCORE_BTN_RANK02,
	StrToWzID("BT16"),	//BTSCORE_BTN_CLASS02,
	StrToWzID("BT17"),	//BTSCORE_BTN_ID02,
	StrToWzID("ST03"),	//BTSCORE_TXT_WIN02,
	
	StrToWzID("PI00"),	//BTSCORE_PIC_01,
	StrToWzID("PI02"),	//BTSCORE_PIC_02,
	StrToWzID("ST05"),	//BTSCORE_WINTXT_01,
	StrToWzID("ST03"),	//BTSCORE_WINTXT_02,

	StrToWzID("L001"),	//BTSCORE_LIST01,
	StrToWzID("LS01"),	//BTSCORE_LIST02,

};


uiBattleScoreDouble::uiBattleScoreDouble(InterfaceManager *pUIMan)
:uiBase(pUIMan),
 m_bInitDlg(FALSE),
 m_bIsInited(false)
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_	= INVALID_HANDLE_VALUE;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreDouble::~uiBattleScoreDouble(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiBattleScoreDouble::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	InitControls();

	m_pBattleScoreMan = NULL;

	InitInfo();

	for(int i=0; i<BTSCORE_LST_MAX; ++i)
	{
		m_pListCtrls[i] = static_cast<CCtrlListWZ *>(this->getControl(BTSCORE_LIST01+i));
		SUN_ASSERT(m_pListCtrls[i]);

		if(m_pListCtrls[i])
			m_pListCtrls[i]->ShowWindowWZ(WZ_SHOW);
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
void uiBattleScoreDouble::InitInfo()
{
	if(m_bIsInited)
	{
        TCHAR result_string[INTERFACE_STRING_LENGTH];
	    TCHAR temp_string[INTERFACE_STRING_LENGTH];
		// 최초는 승수 모두 0
		if (m_pTxtCtrls[eWIN01])
		{   // 2115: Winning: %d
			g_InterfaceManager.GetInterfaceString(2115, temp_string, INTERFACE_STRING_LENGTH);
			Snprintf(result_string, INTERFACE_STRING_LENGTH, temp_string, GetManager()->GetWinPoint(eHomeTeam));
			m_pTxtCtrls[eWIN01]->SetTextWZ(result_string);
		}
		if (m_pTxtCtrls[eWIN02])
		{   // 2115: Winning: %d
			g_InterfaceManager.GetInterfaceString(2115, temp_string, INTERFACE_STRING_LENGTH);
			Snprintf(result_string, INTERFACE_STRING_LENGTH, temp_string, GetManager()->GetWinPoint(eAwayTeam));
			m_pTxtCtrls[eWIN02]->SetTextWZ(result_string);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreDouble::RefreshInfo()
{
	if(!g_pHero)
	{
		return;
	}
    if(m_bIsInited)
	{
        TCHAR result_string[INTERFACE_STRING_LENGTH];
	    TCHAR temp_string[INTERFACE_STRING_LENGTH];
		if(m_pTxtCtrls[eWIN01])
		{	// 2115: Winning: %d
			g_InterfaceManager.GetInterfaceString(2115, temp_string, INTERFACE_STRING_LENGTH);
			Snprintf(result_string, INTERFACE_STRING_LENGTH, temp_string, 
                GetManager()->GetWinPoint(eHomeTeam));
			m_pTxtCtrls[eWIN01]->SetTextWZ(result_string);
		}
		if (m_pTxtCtrls[eWIN02])
		{	// 2115: Winning: %d
			g_InterfaceManager.GetInterfaceString(2115, temp_string, INTERFACE_STRING_LENGTH);
			Snprintf(result_string, INTERFACE_STRING_LENGTH, temp_string, 
                GetManager()->GetWinPoint(eAwayTeam));
			m_pTxtCtrls[eWIN02]->SetTextWZ(result_string);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreDouble::InitControls()
{
	for(int i=0; i<BTSCORE_LST_MAX; ++i)
	{
		m_pListCtrls[i] = static_cast<CCtrlListWZ *>(this->getControl(BTSCORE_LIST01+i));
		SUN_ASSERT(m_pListCtrls[i]);

		if(m_pListCtrls[i])
		{
			m_pListCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	for(i=0; i<uiBattleScoreMan::BTSCROE_LIST_MAX; ++i)
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        if (i == uiBattleScoreMan::BTSCROE_LIST_CLASS)
        {
            // ui변경으로 사라짐
            m_pListTitleBtn01[i] = NULL;
            continue;
        }
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		m_pListTitleBtn01[i] = static_cast<CCtrlButtonWZ *>(this->getControl(BTSCORE_BTN_ID01+i));
		SUN_ASSERT(m_pListTitleBtn01[i]);
	}

	for(i=0; i<uiBattleScoreMan::BTSCROE_LIST_MAX; ++i)
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        if (i == uiBattleScoreMan::BTSCROE_LIST_KILL)
        {
            // ui변경으로 사라짐
            m_pListTitleBtn02[i] = NULL;
            continue;
        }
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		m_pListTitleBtn02[i] = static_cast<CCtrlButtonWZ *>(this->getControl(BTSCORE_BTN_DIE02+i));
		SUN_ASSERT(m_pListTitleBtn02[i]);
	}

	for(i=0; i<uiBattleScoreDouble::eMAX_WINCON; ++i)
	{
		m_pPicCtrls[i] = static_cast<CCtrlPictureWZ *>(this->getControl(BTSCORE_PIC_01+i));
		SUN_ASSERT(m_pPicCtrls[i]);

		if(m_pPicCtrls[i])
		{
			m_pPicCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}

		m_pTxtCtrls[i] = static_cast<CCtrlStaticWZ *>(this->getControl(BTSCORE_WINTXT_01+i));
		SUN_ASSERT(m_pTxtCtrls[i]);

		if(m_pTxtCtrls[i])
		{
			m_pTxtCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
	m_bIsInited = true;
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreDouble::ShowInfo(BOOL eStat)
{
	int i=0;

	for(i=0; i<eMAX_WINCON; ++i)
	{
		m_pPicCtrls[i]->ShowWindowWZ((ENUM_STATEWZ)eStat);
		m_pTxtCtrls[i]->ShowWindowWZ((ENUM_STATEWZ)eStat);
	}
}
//------------------------------------------------------------------------------
/** 다이얼로그 윈도우띄우기
*/
void uiBattleScoreDouble::OnShowWindow( BOOL val )
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Battle_Score;
		msg.wParam = IM_BATTLESCORE_MANAGER::DLG_BT_DOUBLE;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
		//uiBase::MoveCenter();
		m_bShow = true;
		if(GetManager()->GetMode() != uiBattleScoreMan::eBTS_PVP_TEAM)
		{
			ShowInfo(WZ_SHOW);	// PVP가 아니면(헌팅) 보여준다.
		}
		_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
		_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
    }
	else
	{
		m_bShow = false;
        g_KeyQueueManager.DeleteMsg(eDoType_Battle_Score);
		ShowInfo(WZ_HIDE);	// 기본으로 숨기고
	}
}
//------------------------------------------------------------------------------
/**
*/
VOID uiBattleScoreDouble::Release()
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
VOID uiBattleScoreDouble::MessageProc( SI_MESSAGE * pMessage )
{
	bool bSort = uiBattleScoreMan::BTSCROE_LIST_RANK;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case BTSCORE_BTN_EXIT:
				{
					this->ShowInterface(FALSE);
				}
				break;
			case BTSCORE_BTN_ID01:
			case BTSCORE_BTN_ID02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_ID) // 버튼이 같을때
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_ID);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_LV01:
			case BTSCORE_BTN_LV02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_LV)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_LV);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_CLASS01:
			case BTSCORE_BTN_CLASS02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_CLASS)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_CLASS);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_MAXDAMAGE01:
			case BTSCORE_BTN_MAXDAMAGE02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_GIVEDAMAGE01:
			case BTSCORE_BTN_GIVEDAMAGE02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_GETDAMAGE01:
			case BTSCORE_BTN_GETDAMAGE02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_KILL01:
			case BTSCORE_BTN_KILL02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_KILL)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_KILL);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_HEAL01:
			case BTSCORE_BTN_HEAL02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_HEAL)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_HEAL);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			case BTSCORE_BTN_DIE01:
			case BTSCORE_BTN_DIE02:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_DIE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_DIE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam], eHomeTeam);
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eAwayTeam], eAwayTeam);
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreDouble::_clearSelection()
{
	if(GetManager())
	{
		GetManager()->ClearSelection();	
		if(m_pListCtrls[BTSCORE_LST_LIST01])	// 팀 1
			m_pListCtrls[BTSCORE_LST_LIST01]->SetCurSel(-1);
		if(m_pListCtrls[BTSCORE_LST_LIST02])	// 팀 2
			m_pListCtrls[BTSCORE_LST_LIST02]->SetCurSel(-1);
	}
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreMan* uiBattleScoreDouble::GetManager()
{
	if(m_pBattleScoreMan)
		return this->m_pBattleScoreMan;
	else
	{
		this->m_pBattleScoreMan =
        static_cast<uiBattleScoreMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
		SUN_ASSERT (m_pBattleScoreMan);

		return this->m_pBattleScoreMan;
	}
}

void uiBattleScoreDouble::OnRenderSolarDialog()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CCtrlListWZ* list_control[2];
    list_control[eHomeTeam] = static_cast<CCtrlListWZ*>(this->getControl(BTSCORE_LIST01));
    list_control[eAwayTeam] = static_cast<CCtrlListWZ*>(this->getControl(BTSCORE_LIST02));

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    RECT rect;
    float x = 0.0f;
    float y = 0.0f;

    for (int team =  eHomeTeam; team < eTeamCNT; ++team)
    {
        if (list_control[team])
        {
            rect = list_control[team]->GetSizeRect();
        }
        else
        {
            this->GetDialogWZ()->GetClientRect(&rect);
        }
        x = static_cast<float>(rect.left);
        y = static_cast<float>(rect.top);

        for (int i = 0; i < GetManager()->m_BTSMemberArray[team].Size(); ++i)
        {
            eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(GetManager()->m_BTSMemberArray[team][i].byClassCode);
            GenderType::Value gender = static_cast<GenderType::Value>(GetManager()->m_BTSMemberArray[team][i].gender);
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
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

//=====================================================================================
//	1.리스트컨트롤 데이터삽입
//=====================================================================================
void uiBattleScoreDouble::_SetListCtrlData(BTSCORE_LSTS eListNum, int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline)
{
	DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// 기본값

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (nSubIndex == uiBattleScoreMan::BTSCROE_LIST_ID)
    {
        nTextStyle = DT_VCENTER | DT_LEFT;	// 왼쪽 정렬
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    if(!bIsOnline && nSubIndex == eBTSSORT_NAME)
    {   // 오프라인이면 빨강
	    GlobalFunc::SetItem(m_pListCtrls[eListNum], nIndex, nSubIndex,
		    pszText,
		    c_wiListFont,
		    nTextStyle,
		    RGBA(255,255,255,0), 
		    RGBA(255,0,0,255), 
		    RGBA(255,0,0,255));
    }
    else
    {   // 정상이면 흰색
        GlobalFunc::SetItem(m_pListCtrls[eListNum], nIndex, nSubIndex,
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
void uiBattleScoreDouble::_RefreshListCtrlData(nArray<BATTLESCORE_DATA> &ArrayType, E_TEAM eTeamNum)
{
	if(!m_bIsInited)
	{
		return;
	}

	BATTLESCORE_DATA stData;
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	//1.카운팅
	int cnt = ArrayType.Size();
	//0.리스트 목록정보삽입
	m_pListCtrls[eTeamNum]->DeleteItemAll();
	m_pListCtrls[eTeamNum]->SetBeginDrawIndex(0);
    
	if(cnt <= 0)
	{
		return;
	}

	m_pListCtrls[eTeamNum]->SetSellHeight(c_iListHeight);
	m_pListCtrls[eTeamNum]->SetUseReport(true);
	m_pListCtrls[eTeamNum]->SetReportUseCnt(uiBattleScoreMan::BTSCROE_LIST_MAX);
	m_pListCtrls[eTeamNum]->SetHideReportRender(true);
	m_pListCtrls[eTeamNum]->SetFontID(c_wiListFont);

	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_RANK, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_RANK], _T("rank"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_ID, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_ID], _T("name"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_LV, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_LV], _T("lv"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_CLASS, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_CLASS], _T("class"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE], _T("maxdmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE], _T("givedmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE], _T("getdmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_HEAL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_HEAL], _T("heal"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_KILL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_KILL], _T("kill"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls[eTeamNum], uiBattleScoreMan::BTSCROE_LIST_DIE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_DIE], _T("die"));

	//2.데이터삽입
	int iAdd = 0;
	for(int iLine = 0; iLine < cnt; ++ iLine)
	{
		int index = m_pListCtrls[eTeamNum]->InsertItemBack();
		m_pListCtrls[eTeamNum]->SetItemData(index, &(ArrayType[iLine]));
		++iAdd;
		for(int iCnt=0; iCnt<eBTSSORT_MAX; ++iCnt)
		{
			ZeroMemory(szTemp, sizeof(szTemp));
	
			switch(iCnt)
			{
			case eBTSSORT_RANK:	// rank
				Snprintf(szTemp,INTERFACE_STRING_LENGTH-1,_T("%d"), (iLine+1));
				break;
			case eBTSSORT_NAME:
				{
				TCHAR *pName=(TCHAR *)ArrayType[iLine].name.Get();
				Snprintf(szTemp,INTERFACE_STRING_LENGTH-1,_T("%s"),pName);
				}
				break;
			case eBTSSORT_LV:
				Snprintf(szTemp,INTERFACE_STRING_LENGTH-1, "%d", ArrayType[iLine].wLevel);				
				break;
			case eBTSSORT_CLASS://클래스
				{	
					Snprintf(szTemp, 
                        INTERFACE_STRING_LENGTH-1, 
                        _T("%s"), 
                        GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(ArrayType[iLine].byClassCode)));
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                    continue;//아이콘으로 대체
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
				}
				break; 
			case eBTSSORT_MAXDMG:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwMaxDamageToEnemy);
				break;
			case eBTSSORT_GIVEDMG:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwTotalDamageToEnemy);
				break;
			case eBTSSORT_GETDMG:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwTotalDamageByEnemy);
				break;
			case eBTSSORT_HEAL:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].dwTotalHealToParty);
				break;
			case eBTSSORT_KILL:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].wKillCount);
				break;
			case eBTSSORT_DIE:
                GlobalFunc::SetPointText(szTemp, ArrayType[iLine].wDeadCount);
				break;
			}
			_SetListCtrlData((enum BTSCORE_LSTS)(BTSCORE_LST_LIST01+eTeamNum), iLine, iCnt, szTemp, 
                ArrayType[iLine].bConnected);
		}
	}
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
