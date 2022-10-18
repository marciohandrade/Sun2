#include "SunClientPrecompiledHeader.h"
#include "uibattlescoresingle.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "AppearanceResourceCode.h"
#include "uiToolTipMan/uiToolTipMan.h"

extern const int c_iListHeight;
extern const int c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAX];

//------------------------------------------------------------------------------
WzID uiBattleScoreSingle::m_wzId[BTSCORE_MAX] = 
{
	StrToWzID("B999"),	//BTSCORE_BTN_EXIT,
	StrToWzID("L001"),	//BTSCORE_LIST,
	StrToWzID("T999"),	//BTSCORE_TXT_TITLE,
	StrToWzID("ST00"),	//BTSCORE_TXT_DISCRIPT,
	StrToWzID("BT60"),	//BTSCORE_BTN_ID,
	StrToWzID("BT63"),	//BTSCORE_BTN_CLASS,
	StrToWzID("BT00"),	//BTSCORE_BTN_RANK,
	StrToWzID("BT01"),	//BTSCORE_BTN_LV,
	StrToWzID("BT02"),	//BTSCORE_BTN_MAXDAMAGE,
	StrToWzID("BT03"),	//BTSCORE_BTN_GIVEDAMAGE,
	StrToWzID("BT04"),	//BTSCORE_BTN_GETDAMAGE,
	StrToWzID("BT07"),	//BTSCORE_BTN_KILL,
	StrToWzID("BT05"),	//BTSCORE_BTN_HEAL,e
	StrToWzID("BT08"),	//BTSCORE_BTN_DIE,
	
	StrToWzID("ST01"),	//BTSCORE_TXT_EXP,
	StrToWzID("ST05"),	//BTSCORE_STXT_EXP,
	StrToWzID("ST02"),	//BTSCORE_TXT_HAIM,
	StrToWzID("ST07"),	//BTSCORE_STXT_HAIM,

	StrToWzID("PI02"),	//BTSCORE_PIC_EXP,
	StrToWzID("PI04"),	//BTSCORE_PIC_HAIM,
};


uiBattleScoreSingle::uiBattleScoreSingle(InterfaceManager *pUIMan)
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
uiBattleScoreSingle::~uiBattleScoreSingle(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiBattleScoreSingle::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	InitControls();

	m_pBattleScoreMan = NULL;

	InitInfo();

	if(m_pListCtrls)
	{
		m_pListCtrls->ShowWindowWZ(WZ_SHOW);
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
void uiBattleScoreSingle::InitControls()
{
	m_pListCtrls = static_cast<CCtrlListWZ *>(this->getControl(BTSCORE_LIST));
	SUN_ASSERT(m_pListCtrls);

	if(m_pListCtrls)
	{
		m_pListCtrls->ShowWindowWZ(WZ_HIDE);
	}

	for(int i=0; i<uiBattleScoreMan::BTSCROE_LIST_MAX; ++i)
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        if (i == uiBattleScoreMan::BTSCROE_LIST_CLASS)
        {
            // ui변경으로 사라짐
            m_pListTitleBtn[i] = NULL;
            continue;
        }
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		m_pListTitleBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(BTSCORE_BTN_ID+i));
		SUN_ASSERT(m_pListTitleBtn[i]);
	}

	for(int i=0; i<eMAX_PIC; ++i)
	{
		m_pPicCtrls[i] = static_cast<CCtrlPictureWZ *>(this->getControl(BTSCORE_PIC_EXP+i));
		SUN_ASSERT(m_pPicCtrls[i]);

		if(m_pPicCtrls[i])
		{
			m_pPicCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	for(int i=0; i<eMAX_TXT; ++i)
	{
		m_pTxtCtrls[i] = static_cast<CCtrlStaticWZ *>(this->getControl(BTSCORE_TXT_EXP+i));
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
void uiBattleScoreSingle::ShowInfo(BOOL eStat)
{
	int i=0;

	for(i=0; i<eMAX_PIC; ++i)
	{
		m_pPicCtrls[i]->ShowWindowWZ((ENUM_STATEWZ)eStat);
	}
	
	for(i=0; i<eMAX_TXT; ++i)
	{
		m_pTxtCtrls[i]->ShowWindowWZ((ENUM_STATEWZ)eStat);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreSingle::InitInfo()
{
	TCHAR szText[INTERFACE_STRING_LENGTH];
	TCHAR szTemp[INTERFACE_STRING_LENGTH];
	
	if(m_bIsInited)
	{
		m_i64Exp = 0;
		m_i64Money = 0;
		
		// 최초는 exp, 하임 모두 0
		if(m_pTxtCtrls[eTXT_EXP])
		{	// 2114: %d EXP
			g_InterfaceManager.GetInterfaceString( 2114, szTemp, INTERFACE_STRING_LENGTH );
			Snprintf(szText, INTERFACE_STRING_LENGTH, szTemp, "0");
			m_pTxtCtrls[eTXT_EXP]->SetTextWZ(szText);            
		}
		if(m_pTxtCtrls[eTXT_HAIM])
		{	// 2121: %d 하임
			g_InterfaceManager.GetInterfaceString( 2121, szTemp, INTERFACE_STRING_LENGTH );
			Snprintf(szText, INTERFACE_STRING_LENGTH, szTemp, "0");
			m_pTxtCtrls[eTXT_HAIM]->SetTextWZ(szText);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreSingle::RefreshInfo(EXPTYPE add_experience, MONEY add_money)
{
	TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szExp[INTERFACE_STRING_LENGTH] = {0,};
	
	if(m_bIsInited)
	{
		if(!g_pHero)
		{
			return;
		}

        m_i64Exp += add_experience;
	    m_i64Money += add_money;

		if(m_pTxtCtrls[eTXT_EXP])
		{	// 2114: %s EXP
			g_InterfaceManager.GetInterfaceString( 2114, szTemp, INTERFACE_STRING_LENGTH );
            GlobalFunc::SetPointText(szExp, m_i64Exp);
            Snprintf(szText, INTERFACE_STRING_LENGTH, szTemp, szExp);
			m_pTxtCtrls[eTXT_EXP]->SetTextWZ(szText);
		}
		if(m_pTxtCtrls[eTXT_HAIM])
		{	// 2121: %s 하임
            g_InterfaceManager.GetInterfaceString( 2121, szTemp, INTERFACE_STRING_LENGTH );
            GlobalFunc::SetPointText(szMoney, m_i64Money);
            Snprintf(szText, INTERFACE_STRING_LENGTH, szTemp, szMoney);
			m_pTxtCtrls[eTXT_HAIM]->SetTextWZ(szText);
		}
	}
}
//------------------------------------------------------------------------------
/** 다이얼로그 윈도우띄우기
*/
void uiBattleScoreSingle::OnShowWindow( BOOL val )
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Battle_Score;
		msg.wParam = IM_BATTLESCORE_MANAGER::DLG_BT_SINGLE;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
		////uiBase::MoveCenter();
		m_bShow = true;
		if(GetManager()->GetMode() != uiBattleScoreMan::eBTS_PVP_PRIVATE)
		{
			ShowInfo(WZ_SHOW);	// PVP가 아니면(미션, SSQ) 보여준다.
		}
		_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
    }
	else
	{
        g_KeyQueueManager.DeleteMsg(eDoType_Battle_Score);
		m_bShow = false;
		ShowInfo(WZ_HIDE);	// 기본으로 숨기고
	}
}
//------------------------------------------------------------------------------
/**
*/
VOID uiBattleScoreSingle::Release()
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
VOID uiBattleScoreSingle::MessageProc( SI_MESSAGE * pMessage )
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
			case BTSCORE_BTN_ID:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_ID) // 버튼이 같을때
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_ID);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_LV:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_LV)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_LV);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_CLASS:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_CLASS)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_CLASS);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_MAXDAMAGE:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_GIVEDAMAGE:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_GETDAMAGE:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_KILL:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_KILL)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_KILL);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_HEAL:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_HEAL)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_HEAL);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
				}
				break;
			case BTSCORE_BTN_DIE:
				{
					if(GetManager()->GetSortBy() == uiBattleScoreMan::BTSCROE_LIST_DIE)
						bSort = !GetManager()->GetOrderBy();	// 반전이 필요
					else
						bSort = true;	// 무조건 true;
					GetManager()->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_DIE);
					GetManager()->SetOrderBy(bSort);
					GetManager()->SortbyList();
					_RefreshListCtrlData(GetManager()->m_BTSMemberArray[eHomeTeam]);
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
void uiBattleScoreSingle::_clearSelection()
{
	if(GetManager())
	{
		GetManager()->ClearSelection();	
		if(m_pListCtrls)	// 친구와 그룹 리스트 같이 사용
			m_pListCtrls->SetCurSel(-1);
	}
}
//------------------------------------------------------------------------------
/**
*/
uiBattleScoreMan* uiBattleScoreSingle::GetManager()
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

void uiBattleScoreSingle::OnRenderSolarDialog()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(this->getControl(BTSCORE_LIST));

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
    x = static_cast<float>(rect.left) + 2;
    y = static_cast<float>(rect.top);

    if (m_pListTitleBtn[0])//BTSCORE_BTN_ID
    {
        x = static_cast<float>(m_pListTitleBtn[0]->GetSizeRect().left) + 2;
    }

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    for (int i = 0; i < GetManager()->m_BTSMemberArray[eHomeTeam].Size(); ++i)
    {
        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(GetManager()->m_BTSMemberArray[eHomeTeam][i].byClassCode);
        GenderType::Value gender = static_cast<GenderType::Value>(GetManager()->m_BTSMemberArray[eHomeTeam][i].gender);
        eCHAR_TYPE race_type = ResourceCode::GetImageClassType(class_type, gender);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        //종족
        if ( class_type == eCHAR_WITCHBLADE )
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x,
                y + (i * c_iListHeight) + 4,
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
                y + (i * c_iListHeight) + 4,
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
            y + (i * c_iListHeight) + 4,
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
        //직업
        if ( class_type == eCHAR_WITCHBLADE )
        {
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x + 14,
                y + (i * c_iListHeight) + 4,
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
                y + (i * c_iListHeight) + 4,
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
            y + (i * c_iListHeight) + 4,
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
//	1.리스트컨트롤 데이터삽입
//=====================================================================================
void uiBattleScoreSingle::_SetListCtrlData(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline)
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
	    GlobalFunc::SetItem(m_pListCtrls, nIndex, nSubIndex,
		    pszText,
		    c_wiListFont,
		    nTextStyle,
		    RGBA(255,255,255,0), 
		    RGBA(255,0,0,255), 
		    RGBA(255,0,0,255));
    }
    else
    {   // 정상이면 흰색
        GlobalFunc::SetItem(m_pListCtrls, nIndex, nSubIndex,
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
void uiBattleScoreSingle::_RefreshListCtrlData(nArray<BATTLESCORE_DATA> &ArrayType)
{
	if(!m_bIsInited)
	{
		return;
	}

	BATTLESCORE_DATA stData;
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    util::_tstring number_string;

	//1.카운팅
	int cnt = ArrayType.Size();

	//0.리스트 목록정보삽입
	m_pListCtrls->DeleteItemAll();
	m_pListCtrls->SetBeginDrawIndex(0);

    if (cnt <= 0)
	{
		return;
	}

	m_pListCtrls->SetSellHeight(c_iListHeight);
	m_pListCtrls->SetUseReport(true);
	m_pListCtrls->SetReportUseCnt(uiBattleScoreMan::BTSCROE_LIST_MAX);
	m_pListCtrls->SetHideReportRender(true);
	m_pListCtrls->SetFontID(c_wiListFont);

	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_RANK, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_RANK], _T("rank"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_ID, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_ID], _T("name"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_LV, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_LV], _T("lv"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_CLASS, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_CLASS], _T("class"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAXDAMAGE], _T("maxdmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GIVEDAMAGE], _T("givedmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_GETDAMAGE], _T("getdmg"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_HEAL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_HEAL], _T("heal"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_KILL, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_KILL], _T("kill"));
	GlobalFunc::SetReportButtonInfo( m_pListCtrls, uiBattleScoreMan::BTSCROE_LIST_DIE, 
		c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_DIE], _T("die"));

	//2.데이터삽입
	int iAdd = 0;
	for(int iLine = 0; iLine < cnt; ++ iLine)
	{
		int index = m_pListCtrls->InsertItemBack();
		m_pListCtrls->SetItemData(index, &(ArrayType[iLine]));
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
			_SetListCtrlData(iLine, iCnt, szTemp, ArrayType[iLine].bConnected);
		}
	}
}
