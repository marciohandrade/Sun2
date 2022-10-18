#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiQuestListDlg/uiQuestListDlg.h"

#include "GlobalFunc.h"
#include "GameConst_Color.h"

#include "Quest.h"
#include "QuestManager_Concrete.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestListMission/uiQuestListMission.h"
#include "uiLoginMan/uiLoginMan.h"
#include "Hero.h"
#include "ContinentMapDlg.h"
#include "uiRadarMan/uiRadarMan.h"

#ifdef _INTERNATIONAL_UI
	#include "uiToolTipMan/uiToolTipMan.h"
	#include "Mouse.h"
#endif//_INTERNATIONAL_UI

namespace
{
    const DWORD kAllChapterCode = 601; // 전체
};

int __stdcall SimpleXorCryption2( PBYTE in_data, DWORD in_size, PBYTE *out_data,	DWORD *out_size )
{
	PBYTE tmp = (PBYTE) calloc(1, in_size);

	if ( NULL == tmp )
	{
		_ASSERTE( !"calloc fail" );
		return -1;
	}

	for (DWORD i = 0; i < in_size; ++i)
	{
		tmp[i] = in_data[i] ^ 0x33;
	}

	*out_data = tmp;
	*out_size = in_size;

	return 0;
}

WzID uiQuestListDlg::m_wzID[eDIALOG_QUEST_LIST_MAX] = 
{
		StrToWzID("B999"),//eDIALOG_QUEST_LIST_EXIT_BTN
#ifdef _SCJ_111202_UI_RENEWAL
        StrToWzID("C000"),//eDIALOG_QUEST_LIST_MISSION_BTN
#else
		StrToWzID("C001"),//eDIALOG_QUEST_LIST_MISSION_BTN
#endif //_SCJ_111202_UI_RENEWAL
		StrToWzID("C002"),//eDIALOG_QUEST_LIST_QUEST_BTN

		StrToWzID("BT00"),//eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN
#ifdef _SCJ_111202_UI_RENEWAL
#else
		StrToWzID("T201"),//eDIALOG_QUEST_LIST_CHAPTER_STRING
#endif //_SCJ_111202_UI_RENEWAL
		StrToWzID("LS00"),//eDIALOG_QUEST_LIST_CHAPTER_LIST

		StrToWzID("BT01"),//eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN
#ifdef _SCJ_111202_UI_RENEWAL
#else
		StrToWzID("ST01"),//eDIALOG_QUEST_LIST_DP_TYPE_STRING
#endif //_SCJ_111202_UI_RENEWAL
		StrToWzID("LS01"),//eDIALOG_QUEST_LIST_DP_TYPE_LIST

		StrToWzID("B000"),//eDIALOG_QUEST_LIST_QUEST_NAME_BT_START = 9,
		StrToWzID("B001"),//
		StrToWzID("B002"),//
		StrToWzID("B003"),//
		StrToWzID("B004"),//
		StrToWzID("B005"),//
		StrToWzID("B006"),//
		StrToWzID("B007"),//
		StrToWzID("B008"),//
		StrToWzID("B009"),//
		StrToWzID("B010"),//
		StrToWzID("B011"),//
		StrToWzID("B012"),//
		StrToWzID("B013"),//
		StrToWzID("B014"),//
		StrToWzID("B015"),//
		StrToWzID("B016"),//
		StrToWzID("B017"),//eDIALOG_QUEST_LIST_QUEST_NAME_BT_END = 27,

		StrToWzID("PI00"),//eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_START = 28,
		StrToWzID("PI01"),//
		StrToWzID("PI02"),//
		StrToWzID("PI03"),//
		StrToWzID("PI04"),//
		StrToWzID("PI05"),//
		StrToWzID("PI06"),//
		StrToWzID("PI07"),//
		StrToWzID("PI08"),//
		StrToWzID("PI09"),//
		StrToWzID("PI10"),//
		StrToWzID("PI11"),//
		StrToWzID("PI12"),//
		StrToWzID("PI13"),//
		StrToWzID("PI14"),//
#ifdef _SCJ_111202_UI_RENEWAL
		StrToWzID("P038"),//
#else
        StrToWzID("PI15"),//
#endif //_SCJ_111202_UI_RENEWAL
		StrToWzID("PI16"),//
		StrToWzID("PI17"),//

		StrToWzID("PI18"),//
		StrToWzID("PI19"),//
		StrToWzID("PI20"),//
		StrToWzID("PI21"),//
		StrToWzID("PI22"),//
		StrToWzID("PI23"),//
		StrToWzID("PI24"),//
		StrToWzID("PI25"),//
		StrToWzID("PI26"),//
		StrToWzID("PI27"),//
		StrToWzID("PI28"),//
		StrToWzID("PI29"),//
		StrToWzID("PI30"),//
		StrToWzID("PI31"),//
		StrToWzID("PI32"),//
		StrToWzID("PI33"),//
		StrToWzID("PI34"),//
		StrToWzID("PI35"),//

		StrToWzID("PI36"),//
		StrToWzID("PI37"),//
		StrToWzID("PI38"),//
		StrToWzID("PI39"),//
		StrToWzID("PI40"),//
		StrToWzID("PI41"),//
		StrToWzID("PI42"),//
		StrToWzID("PI43"),//
		StrToWzID("PI44"),//
		StrToWzID("PI45"),//
		StrToWzID("PI46"),//
		StrToWzID("PI47"),//
		StrToWzID("PI48"),//
		StrToWzID("PI49"),//
		StrToWzID("PI50"),//
		StrToWzID("PI51"),//
		StrToWzID("PI52"),//
		StrToWzID("PI53"),//

		StrToWzID("PI54"),//
		StrToWzID("PI55"),//
		StrToWzID("PI56"),//
		StrToWzID("PI57"),//
		StrToWzID("PI58"),//
		StrToWzID("PI59"),//
		StrToWzID("PI60"),//
		StrToWzID("PI61"),//
		StrToWzID("PI62"),//
		StrToWzID("PI63"),//
		StrToWzID("PI64"),//
		StrToWzID("PI65"),//
		StrToWzID("PI66"),//
		StrToWzID("PI67"),//
		StrToWzID("PI68"),//
		StrToWzID("PI69"),//
		StrToWzID("PI70"),//
		StrToWzID("PI71"),//eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_END = 98,

		StrToWzID("ST02"),//eDIALOG_QUEST_LIST_QUEST_NUM_STRING = 99,

		StrToWzID("VS00"),//eDIALOG_QUEST_LIST_SCROLL = 100,

		StrToWzID("BT02"),//eDIALOG_QUEST_LIST_NAVI_BTN_START = 101,
		StrToWzID("BT03"),
		StrToWzID("BT04"),
		StrToWzID("BT05"),
		StrToWzID("BT06"),
		StrToWzID("BT07"),
		StrToWzID("BT08"),
		StrToWzID("BT09"),
		StrToWzID("BT10"),
		StrToWzID("BT11"),
		StrToWzID("BT12"),
		StrToWzID("BT13"),
		StrToWzID("BT14"),
		StrToWzID("BT15"),
		StrToWzID("BT16"),
		StrToWzID("BT17"),
		StrToWzID("BT18"),
		StrToWzID("BT19"),//eDIALOG_QUEST_LIST_NAVI_BTN_END   = 118,

		StrToWzID("BT20"),//eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN2 = 119,
		StrToWzID("BT21"),//eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN2    = 120,

#ifdef _SCJ_111202_UI_RENEWAL
        StrToWzID("PI99"),
        StrToWzID("P000"),
        StrToWzID("P003"),
        StrToWzID("P005"),
        StrToWzID("P007"),
        StrToWzID("P010"),
        StrToWzID("P011"),
        StrToWzID("P012"),
        StrToWzID("P013"),
        StrToWzID("P014"),
        StrToWzID("P015"),
        StrToWzID("P016"),
        StrToWzID("P017"),
        StrToWzID("P018"),
        StrToWzID("P019"),
        StrToWzID("PI15"),
        StrToWzID("P021"),
        StrToWzID("P022"),
#endif //_SCJ_111202_UI_RENEWAL
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiQuestListDlg::uiQuestListDlg(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	//m_bShow			  = FALSE;
	m_bInit			  = FALSE;
	for(int i =0; i < MAX_NAVI_CNT; ++i)
	{
		m_dwNaviQCode[i] = 0;
	}
    ZeroMemory(m_szFileName, sizeof(m_szFileName));
    ZeroMemory(active_chapter_combocontrol_position_, sizeof(active_chapter_combocontrol_position_));
    ZeroMemory(active_display_combocontrol_position_, sizeof(active_display_combocontrol_position_));
    last_scrollbar_position_.clear();

    open_quest_code_ = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiQuestListDlg::~uiQuestListDlg()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiQuestMan* uiQuestListDlg::GetManager()
{
	uiQuestMan* puiQuestMan  =
		static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
	assert (puiQuestMan);
	return puiQuestMan;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	//m_bShow			  = FALSE;
	SetDlgState(QUEST_CATEGORY_MISSION);
	m_iCurChapterIdx = 0;
	m_dwChapterCode  = 0;
	m_iOldSelNaviIdx = 0;
    m_eQDpType		 = eQUEST_DISPLAY_TYPE_PRG;

    ZeroMemory(m_szFileName, sizeof(m_szFileName));

    ZeroMemory(active_chapter_combocontrol_position_, sizeof(active_chapter_combocontrol_position_));
    ZeroMemory(active_display_combocontrol_position_, sizeof(active_display_combocontrol_position_));
    last_scrollbar_position_.clear();

	//(서브)퀘스트에 한하여 진행 표시로 변경.
// 	set_active_display_combocontrol_position(QUEST_CATEGORY_QUEST, eQUEST_DISPLAY_TYPE_PRG);
// 	set_active_display_combocontrol_position(QUEST_CATEGORY_MISSION, eQUEST_DISPLAY_TYPE_PRG);

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiQuestListDlg::LoadNaviQCode()
{
	if (NULL == GetManager())
    {
		return;
    }

    if ((IsLoaded() == FALSE) && (LoadInterface() == FALSE))
    {
        return;
    }

	TCHAR szCharInfo[256] = {0,};
	uiLoginMan* loginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if (loginMan && g_pHero)
	{
		Snprintf(szCharInfo, 255, _T("%s-%s"), loginMan->GetID(), g_pHero->GetName());
	}

	TCHAR szCurDirectory[512+1] = {0,};
	GlobalFunc::GetModuleFileNameDirectory( szCurDirectory, 512 );

	Snprintf( m_szFileName, 512,  _T("%s%s"), szCurDirectory, _T("QN.dat") );
	SetFileAttributes( m_szFileName, FILE_ATTRIBUTE_NORMAL );

	if (-1 != m_MemoryINI.LoadFromFile( m_szFileName, SimpleXorCryption2 ))
	{
		TCHAR szIdx[5] = {0,};
		LONG lTemp = 0l;

		GetManager()->clearViewQuestForProgress();

		for(int i = 0; i < MAX_NAVI_CNT; ++i)
		{
			Snprintf(szIdx, 4, _T("%d"), i);
			m_MemoryINI.getInteger( szCharInfo, szIdx, &lTemp );
			m_dwNaviQCode[i] = (DWORD)lTemp;
			GetManager()->AddQuestForProgress( (QCODE)m_dwNaviQCode[i]);

		}
	}
}

void uiQuestListDlg::SaveNaviQCode()
{
	if (FALSE == GENERALPARAM->IsNet())
		return;

	TCHAR szCharInfo[256] = {0,};
	uiLoginMan* loginMan =
		static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if (loginMan && g_pHero)
	{
		Snprintf(szCharInfo, 255, _T("%s-%s"), loginMan->GetID(), g_pHero->GetName());
	}

	TCHAR szIdx[5] = {0,};
	LONG lTemp = 0l;
	for(int i = 0; i < MAX_NAVI_CNT; ++i)
	{
		lTemp = (LONG)m_dwNaviQCode[i];
		Snprintf(szIdx, 4, _T("%d"), i);
		m_MemoryINI.putInteger( szCharInfo, szIdx,  lTemp);
	}
	
	if (-1 == m_MemoryINI.SaveToFile( m_szFileName, SimpleXorCryption2 ))
	{
		assert(!"uiQuestListDlg::m_MemoryINI.SaveToFile");
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::Release()
{
	m_vecMainQuest.clear();
	m_vecSubQuest.clear();
	m_MainQChapterCodes.clear();
	m_SubQChapterCodes.clear();

	m_vecVisCurChapterQuestIdx.clear();
}

//------------------------------------------------------------------------------
void uiQuestListDlg::Reset()
{
    SolarDialog::Reset();

    ZeroMemory(active_chapter_combocontrol_position_, sizeof(active_chapter_combocontrol_position_));
    ZeroMemory(active_display_combocontrol_position_, sizeof(active_display_combocontrol_position_));
    last_scrollbar_position_.clear();

	//(서브)퀘스트에 한하여 진행 표시로 변경.
// 	set_active_display_combocontrol_position(QUEST_CATEGORY_QUEST, eQUEST_DISPLAY_TYPE_PRG);
// 	set_active_display_combocontrol_position(QUEST_CATEGORY_MISSION, eQUEST_DISPLAY_TYPE_PRG);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::OnUpdateSolarDialog()
{
    
	if (!IsVisible())
	{
		ShowDialog(FALSE);
	}

	updateMouseWheel();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::OnRenderSolarDialog()
{
#ifdef _INTERNATIONAL_UI
	renderToolTip();
#endif//_INTERNATIONAL_UI
	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::OnShowWindow(BOOL val)
{
	if ( val )
	{
		bool bCheckState = QUEST_CATEGORY_MISSION == GetDlgState();
		m_pCtrlMissionTabBtn->SetCheckState(bCheckState);
		m_pCtrlQuestTabBtn->SetCheckState(!bCheckState);

		m_pCtrlChapterList->SetCurSel(m_iCurChapterIdx);
        m_eQDpType		 = eQUEST_DISPLAY_TYPE_PRG;
		m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);

		RefreshQuestList();
		SelectChapter(); //ShowWindow시엔 한번 더 호출

        int scroll_max_value = QuestListCount(GetDlgState(), m_dwChapterCode, m_eQDpType) - QUEST_NAME_CHK_BT_CNT;
        m_pCtrlScroll->SetScrollRangeWZ(0, scroll_max_value);
        m_pCtrlScroll->SetScrollPosWZ(last_scrollbar_position(GetDlgState(), m_dwChapterCode, m_eQDpType));

		RePosQuestList();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_Dialog_Quest_List_Dlg;
		msg.DoSomething=GlobalFunc::DoQuestListDlg;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Quest_List_Dlg);
        open_quest_code_ = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::SelectDPType()
{
    if (m_pCtrlScroll == NULL || m_pCtrlDPTypeList == NULL)
    {
        return;
    }

	int iCurSel = m_pCtrlDPTypeList->GetCurSel();
	if (-1 == iCurSel)
	{
        m_pCtrlDPTypeList->SetCurSel(eQUEST_DISPLAY_TYPE_PRG);

		set_active_display_combocontrol_position(QUEST_CATEGORY_MISSION, eQUEST_DISPLAY_TYPE_PRG);
		set_active_display_combocontrol_position(QUEST_CATEGORY_QUEST, eQUEST_DISPLAY_TYPE_PRG);
		iCurSel = m_pCtrlDPTypeList->GetCurSel();
	}

	m_eQDpType = (eQUEST_DISPLAY_TYPE)iCurSel;

	TCHAR szDPType[INTERFACE_STRING_LENGTH + 1] = {0,};
	if (eQUEST_DISPLAY_TYPE_ALL == m_eQDpType)
		g_InterfaceManager.GetInterfaceString(601,szDPType);  //601	    전체
	else if (eQUEST_DISPLAY_TYPE_PRG == m_eQDpType)
		g_InterfaceManager.GetInterfaceString(1664,szDPType); //1664	진행
	else if (eQUEST_DISPLAY_TYPE_END == m_eQDpType)
		g_InterfaceManager.GetInterfaceString(859,szDPType); //859		종료
	else 
	{
		assert(!"uiQuestListDlg::SelectDPType()");
	}

#ifdef _SCJ_111202_UI_RENEWAL
    m_pCtrlDPTypeBtn2->SetTextWZ(szDPType);
#else
	m_pCtrlDPTypeName->SetTextWZ(szDPType);
#endif //_SCJ_111202_UI_RENEWAL
	m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);

    set_active_display_combocontrol_position(GetDlgState(), m_eQDpType);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::SelectChapter()
{
    if ((m_pCtrlChapterList == NULL) || (m_pCtrlScroll == NULL))
    {
        return;
    }

	if (0 == m_pCtrlChapterList->GetListRowCnt())
	{
		SetQuestCntAndChapterCnt();
		if (0 == m_pCtrlChapterList->GetListRowCnt())
			return;
	}

    std::vector<DWORD>* pmapChapterCode = NULL;
	if (QUEST_CATEGORY_MISSION == m_eDlgState)
	{
		pmapChapterCode = &m_MainQChapterCodes;
	}
	else if (QUEST_CATEGORY_QUEST == m_eDlgState)
	{
		pmapChapterCode = &m_SubQChapterCodes;
	}
	else
	{
		assert(!"uiQuestListDlg::SelectChapter()");
		return;
	}

	int iCurSel = m_pCtrlChapterList->GetCurSel();
	if (-1 == iCurSel)
	{
		m_pCtrlChapterList->SetCurSel(0);
		iCurSel = m_pCtrlChapterList->GetCurSel();
	}

	m_iCurChapterIdx = iCurSel;

    std::vector<DWORD>::iterator iter;

	if (true == pmapChapterCode->empty())
		SetQuestCntAndChapterCnt();

    if (true == pmapChapterCode->empty())
    {
#ifdef _SCJ_111202_UI_RENEWAL
        m_pCtrlChapterBtn2->SetTextWZ("");
#else
        m_pCtrlChapterName->SetTextWZ("");
#endif //_SCJ_111202_UI_RENEWAL
    }
	else
	{
        iter = pmapChapterCode->begin();
		int i = 0;
        while(iter != pmapChapterCode->end())
		{
			if (i != m_iCurChapterIdx)
			{
				++i;
                ++iter;
				continue;
			}

            m_dwChapterCode = (*iter);

			TCHAR szChapter[INTERFACE_STRING_LENGTH + 1] = {0,};
            g_InterfaceManager.GetInterfaceString(m_dwChapterCode, szChapter);
#ifdef _SCJ_111202_UI_RENEWAL
            m_pCtrlChapterBtn2->SetTextWZ(szChapter);
#else
			m_pCtrlChapterName->SetTextWZ(szChapter);
#endif //_SCJ_111202_UI_RENEWAL
			break;
		}
	}

    set_active_chapter_combocontrol_position(GetDlgState(), m_iCurChapterIdx);

	m_pCtrlChapterList->ShowWindowWZ(WZ_HIDE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::MessageProc(SI_MESSAGE * pMessage)
{
	POSTYPE pos = getControlIDToPos( pMessage->dwCtrlID );

	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_SB_LINETOP:
	case RT_MSG_SB_PAGETOP:
		{
			if (m_pCtrlScroll)
			{
				int pos = m_pCtrlScroll->GetScrollPosWZ();
				int mmin = 0 , mmax = 0;
				m_pCtrlScroll->GetScrollRangeWZ( &mmin, &mmax );

				if (pos > mmin)// 현재 입력된 라인수 보다 작으면 
				{
					--pos;//한줄 감소 시키고 
					m_pCtrlScroll->SetScrollPosWZ(pos);
				}
                set_last_scrollbar_position(GetDlgState(), 
                                            m_dwChapterCode, 
                                            m_eQDpType, 
                                            m_pCtrlScroll->GetScrollPosWZ());
				RePosQuestList();
				RefreshNaviBtn();
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_PAGEBOTTOM:
		{
			if (m_pCtrlScroll)
			{
				int pos = m_pCtrlScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pCtrlScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if (pos < mmax)// 맥스값 보다 작으면 
				{
					++pos;//한줄 증가 시키고 
					m_pCtrlScroll->SetScrollPosWZ(pos);
				}
                set_last_scrollbar_position(GetDlgState(), 
                                            m_dwChapterCode, 
                                            m_eQDpType, 
                                            m_pCtrlScroll->GetScrollPosWZ());
				RePosQuestList();
				RefreshNaviBtn();
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
            set_last_scrollbar_position(GetDlgState(), 
                                        m_dwChapterCode, 
                                        m_eQDpType, 
                                        m_pCtrlScroll->GetScrollPosWZ());

			RePosQuestList();
			RefreshNaviBtn();
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			switch ( pos )
			{
			case eDIALOG_QUEST_LIST_MISSION_BTN:
				{
					m_pCtrlMissionTabBtn->SetCheckState(true);
					m_pCtrlQuestTabBtn->SetCheckState(false);
					m_pCtrlChapterList->SetCurSel(0);

                    m_pCtrlChapterList->SetCurSel(active_chapter_combocontrol_position(QUEST_CATEGORY_MISSION));
                    m_pCtrlDPTypeList->SetCurSel(active_display_combocontrol_position(QUEST_CATEGORY_MISSION));
					m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);

					SetDlgState(QUEST_CATEGORY_MISSION);
					RefreshQuestList();
				}
				break;

			case eDIALOG_QUEST_LIST_QUEST_BTN:
				{
					m_pCtrlMissionTabBtn->SetCheckState(false);
					m_pCtrlQuestTabBtn->SetCheckState(true);
					m_pCtrlChapterList->SetCurSel(0);
                    m_pCtrlChapterList->SetCurSel(active_chapter_combocontrol_position(QUEST_CATEGORY_QUEST));
                    m_pCtrlDPTypeList->SetCurSel(active_display_combocontrol_position(QUEST_CATEGORY_QUEST));
					m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);

					SetDlgState(QUEST_CATEGORY_QUEST);
					RefreshQuestList();
				}
				break;

			case eDIALOG_QUEST_LIST_CHAPTER_LIST:
				{
					SelectChapter();
                    int scroll_max_value = QuestListCount(GetDlgState(), m_dwChapterCode, m_eQDpType) - QUEST_NAME_CHK_BT_CNT;
                    m_pCtrlScroll->SetScrollRangeWZ(0, scroll_max_value);
                    m_pCtrlScroll->SetScrollPosWZ(last_scrollbar_position(GetDlgState(), m_dwChapterCode, m_eQDpType));
					RePosQuestList();
					RefreshNaviBtn();
				}
				break;

			case eDIALOG_QUEST_LIST_DP_TYPE_LIST:
				{
					SelectDPType();
                    int scroll_max_value = QuestListCount(GetDlgState(), m_dwChapterCode, m_eQDpType) - QUEST_NAME_CHK_BT_CNT;
                    m_pCtrlScroll->SetScrollRangeWZ(0, scroll_max_value);
                    m_pCtrlScroll->SetScrollPosWZ(last_scrollbar_position(GetDlgState(), m_dwChapterCode, m_eQDpType));
					RePosQuestList();
					RefreshNaviBtn();
				}
				break;

			case eDIALOG_QUEST_LIST_EXIT_BTN:
				{
					ShowInterfaceWithSound(FALSE);
				}
				break;

			case eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN:
			case eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN2:
				{
					if (WZ_SHOW == m_pCtrlChapterList->GetState())
						m_pCtrlChapterList->ShowWindowWZ(WZ_HIDE);
					else
						m_pCtrlChapterList->ShowWindowWZ(WZ_SHOW);
				}
				break;

			case eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN:
			case eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN2:
				{
					if (WZ_SHOW == m_pCtrlDPTypeList->GetState())
						m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);
					else
						m_pCtrlDPTypeList->ShowWindowWZ(WZ_SHOW);
				}
				break;

			default:
				{
					if (pos >= eDIALOG_QUEST_LIST_QUEST_NAME_BT_START && pos <= eDIALOG_QUEST_LIST_QUEST_NAME_BT_END)
					{
						int iIdx = pos - eDIALOG_QUEST_LIST_QUEST_NAME_BT_START;

						std::vector<DWORD>* pvecQ = NULL;
						if (QUEST_CATEGORY_MISSION == m_eDlgState)
						{
							pvecQ = &m_vecMainQuest;
						}
						else if (QUEST_CATEGORY_QUEST == m_eDlgState)
						{
							pvecQ = &m_vecSubQuest;
						}
						else
						{
							assert(!"uiQuestListDlg::MessageProc");
							return;
						}

                        OpenQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[iIdx]]);
						/*Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[iIdx]]);
						if (NULL == pQ)
						{
							pQ = g_pQMGR_Temp->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[iIdx]]);
							if (NULL == pQ)
							{
								assert(!"void uiQuestListDlg::MessageProc");
								return;
							}
						}

						uiQuestListMission* pDlg = GET_CAST_DIALOG(uiQuestListMission, IM_QUEST_MANAGER::QUEST_LIST_MISSION);
						if (NULL == pDlg)
							break;

						pDlg->SetCurQuest(pQ);

						//if (FALSE == pDlg->IsVisible())
						{
							pDlg->ShowInterface(TRUE);
						}*/
					}
					else if (pos >= eDIALOG_QUEST_LIST_NAVI_BTN_START && pos <= eDIALOG_QUEST_LIST_NAVI_BTN_END)
					{
						SetNaviBtn(pos);
					}
				}
			}
		}
		break;

	default:
		{
#ifdef _INTERNATIONAL_UI
			SolarDialog::MessageProc( pMessage );
#endif//_INTERNATIONAL_UI
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
void uiQuestListDlg::OpenQuest(QCODE quest_code)
{
    Quest* pQ = g_pQMGR_Real->FindQuest(quest_code);
    if (NULL == pQ)
    {
        pQ = g_pQMGR_Temp->FindQuest(quest_code);
        if (NULL == pQ)
        {
            assert(!"void uiQuestListDlg::MessageProc");
            return;
        }
    }

    uiQuestListMission* pDlg = GET_CAST_DIALOG(uiQuestListMission, IM_QUEST_MANAGER::QUEST_LIST_MISSION);
    if (NULL == pDlg)
    {
        return;
    }

    pDlg->SetCurQuest(pQ);

    if (FALSE == pDlg->IsVisible() || open_quest_code_ != quest_code)
    {
        pDlg->ShowInterface(TRUE);
    }
    else
    {
        pDlg->ShowInterface(FALSE);
    }

    open_quest_code_ = quest_code;
}
//////////////////////////////////////////////////////////////////////////
void uiQuestListDlg::SetNaviQuest(Quest* pQ)
{
	if (NULL == pQ)
	{
		assert(!"uiQuestListDlg::SetNaviBtn::NULL == pQ");
		return;
	}

	if (NULL == GetManager())
		return;

	//같은 퀘스트 코드가 NaviQCodes에 있는지 검사후 있으면 AddQuestForProgress만 하고 리턴
	for(int i = 0; i < MAX_NAVI_CNT; ++i)
	{
		if (pQ->GetQuestCode() == m_dwNaviQCode[i])
		{
			GetManager()->AddQuestForProgress( pQ->GetQuestCode() );
			return;
		}

	}

	int iEmpty = MAX_NAVI_CNT;
	for(int i = 0; i < MAX_NAVI_CNT; ++i)
	{
		if (0 == m_dwNaviQCode[i])
		{
			iEmpty = i;
			break;
		}
		else
		{
			if (NULL == g_pQMGR_Real->FindQuest((QCODE)m_dwNaviQCode[i]))
			{
				iEmpty = i;
				break;
			}
		}
	}
	if (MAX_NAVI_CNT == iEmpty)
	{
        if (m_iOldSelNaviIdx < MAX_NAVI_CNT)
        {
		    GetManager()->DelQuestForProgress( (QCODE)m_dwNaviQCode[m_iOldSelNaviIdx] );
		    m_dwNaviQCode[m_iOldSelNaviIdx] = pQ->GetQuestCode();
		    GetManager()->AddQuestForProgress( pQ->GetQuestCode() );

		    if (MAX_NAVI_CNT-1 <= m_iOldSelNaviIdx)
			    m_iOldSelNaviIdx = 0;
		    else
			    m_iOldSelNaviIdx++;
        }
        else
        {
            SUN_ASSERT_MSG(m_iOldSelNaviIdx < MAX_NAVI_CNT, _T("ERROR!! m_iOldSelNaviIdx >= MAX_NAVI_CNT !!!!!"));
        }
	}
	else
	{
		m_dwNaviQCode[iEmpty] = pQ->GetQuestCode();
		GetManager()->AddQuestForProgress( pQ->GetQuestCode() );
	}

	RefreshQuestList();
}

void uiQuestListDlg::SetNaviBtn(POSTYPE pos)
{
	if (NULL == GetManager())
		return;

	std::vector<DWORD>* pvecQ = NULL;
	if (QUEST_CATEGORY_MISSION == m_eDlgState)
	{
		pvecQ = &m_vecMainQuest;
	}
	else if (QUEST_CATEGORY_QUEST == m_eDlgState)
	{
		pvecQ = &m_vecSubQuest;
	}
	else
	{
		assert(!"uiQuestListDlg::SetNaviBtn");
		return;
	}

	int idx = pos - eDIALOG_QUEST_LIST_NAVI_BTN_START;
	if (idx >= (int)m_vecVisCurChapterQuestIdx.size())
		return;

	Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[idx]]);
	if (NULL == pQ)
		return;

	BOOL bNaviState[MAX_NAVI_CNT] = {FALSE,};
	for(unsigned int i = 0; i < m_vecMainQuest.size(); ++i)
	{
		Quest* pQTemp = g_pQMGR_Real->FindQuest((QCODE)m_vecMainQuest[i]);
		if (NULL == pQTemp)
			continue;

		if (pQTemp->GetQuestCode() == m_dwNaviQCode[0])
			bNaviState[0] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[1])
			bNaviState[1] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[2])
			bNaviState[2] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[3])
			bNaviState[3] = TRUE;
	}

	for(unsigned int i = 0; i < m_vecSubQuest.size(); ++i)
	{
		Quest* pQTemp = g_pQMGR_Real->FindQuest((QCODE)m_vecSubQuest[i]);
		if (NULL == pQTemp)
			continue;

		if (pQTemp->GetQuestCode() == m_dwNaviQCode[0])
			bNaviState[0] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[1])
			bNaviState[1] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[2])
			bNaviState[2] = TRUE;
		if (pQTemp->GetQuestCode() == m_dwNaviQCode[3])
			bNaviState[3] = TRUE;
	}

	if (WZ_HIDE == m_pCtrlQusetChkPicture[idx][0]->GetState() && 
		WZ_HIDE == m_pCtrlQusetChkPicture[idx][1]->GetState() &&
		WZ_HIDE == m_pCtrlQusetChkPicture[idx][2]->GetState() &&
		WZ_HIDE == m_pCtrlQusetChkPicture[idx][3]->GetState())
	{
		BOOL bVal = FALSE;
		for(int i = 0; i < MAX_NAVI_CNT; ++i)
		{
			if (FALSE == bNaviState[i])
			{
				m_pCtrlQusetChkPicture[idx][i]->ShowWindowWZ(WZ_SHOW);
				m_dwNaviQCode[i] = pQ->GetQuestCode();
				m_iOldSelNaviIdx = i;

				GetManager()->AddQuestForProgress( (QCODE)m_dwNaviQCode[i] );


				bVal = TRUE;
				break;
			}
		}

		if (FALSE == bVal)
		{
			for(int i = 0; i < QUEST_NAME_CHK_BT_CNT; ++i)
			{
				if (WZ_SHOW == m_pCtrlQusetChkPicture[i][m_iOldSelNaviIdx]->GetState())
					m_pCtrlQusetChkPicture[i][m_iOldSelNaviIdx]->ShowWindowWZ(WZ_HIDE);
			}

			GetManager()->DelQuestForProgress( (QCODE)m_dwNaviQCode[m_iOldSelNaviIdx] );

			m_pCtrlQusetChkPicture[idx][m_iOldSelNaviIdx]->ShowWindowWZ(WZ_SHOW);
			m_dwNaviQCode[m_iOldSelNaviIdx] = pQ->GetQuestCode();

			GetManager()->AddQuestForProgress( (QCODE)m_dwNaviQCode[m_iOldSelNaviIdx] );


			if (MAX_NAVI_CNT-1 <= m_iOldSelNaviIdx)
				m_iOldSelNaviIdx = 0;
			else
				m_iOldSelNaviIdx++;

			for(int i = 0; i < MAX_NAVI_CNT; ++i)
			{
				if (0 == m_dwNaviQCode[i])
					m_iOldSelNaviIdx = i;
			}
		}
	}
	else
	{
		m_pCtrlQusetChkPicture[idx][0]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[idx][1]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[idx][2]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[idx][3]->ShowWindowWZ(WZ_HIDE);
		for(int i = 0; i < MAX_NAVI_CNT; ++i)
		{
			if (pQ->GetQuestCode() == m_dwNaviQCode[i])
			{
				GetManager()->DelQuestForProgress( (QCODE)m_dwNaviQCode[i] );
				m_dwNaviQCode[i] = 0;
				m_iOldSelNaviIdx = i;
			}
		}
	}

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ui_radar_manager->SetNavigationPoint();
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::RefreshNaviBtn()
{
	for(int i = 0; i < MAX_NAVI_CNT; ++i)
	{
		if (NULL == g_pQMGR_Real->FindQuest((QCODE)m_dwNaviQCode[i]))
			m_dwNaviQCode[i] = 0;
	}


	for(int i = 0; i < QUEST_NAME_CHK_BT_CNT; ++i)
	{
		m_pCtrlQusetChkPicture[i][0]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[i][1]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[i][2]->ShowWindowWZ(WZ_HIDE);
		m_pCtrlQusetChkPicture[i][3]->ShowWindowWZ(WZ_HIDE);
	}

	std::vector<DWORD>* pvecQ = NULL;
	if (QUEST_CATEGORY_MISSION == m_eDlgState)
	{
		pvecQ = &m_vecMainQuest;
	}
	else if (QUEST_CATEGORY_QUEST == m_eDlgState)
	{
		pvecQ = &m_vecSubQuest;
	}
	else
	{
		assert(!"uiQuestListDlg::RefreshNaviBtn()");
		return;
	}

	int iCnt = m_vecVisCurChapterQuestIdx.size();
	if (QUEST_NAME_CHK_BT_CNT < iCnt)
	{
		assert(!"QUEST_NAME_CHK_BT_CNT <= m_vecVisCurChapterQuestIdx.size()");
		return;
	}
	for(int i = 0; i < iCnt; ++i)
	{
		Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[i]]);
		if (NULL == pQ)
			continue;

		for(int j = 0; j < MAX_NAVI_CNT; ++j)
		{
			if (m_dwNaviQCode[j] == pQ->GetQuestCode())
			{
				m_pCtrlQusetChkPicture[i][j]->ShowWindowWZ(WZ_SHOW);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef _INTERNATIONAL_UI
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
VOID uiQuestListDlg::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getControlIDToPos( pMessage->dwCtrlID );

			m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

			if (pos >= eDIALOG_QUEST_LIST_QUEST_NAME_BT_START && pos <= eDIALOG_QUEST_LIST_QUEST_NAME_BT_END)
			{
				int iIdx = pos - eDIALOG_QUEST_LIST_QUEST_NAME_BT_START;
				//step1:툴팁이존재하는 버튼에 들어왔을때만 뿌려준다.
				if (iIdx < QUEST_NAME_CHK_BT_CNT)
				{
					//퀘스트포인터를저장해주자
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_QUEST;
					m_MouseOverObject.iSkillIdx = iIdx;
				}
			}
		}
		break;
	}
}

void uiQuestListDlg::renderToolTip()
{
	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_QUEST == m_MouseOverObject.eMouseOverType  )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			int idx = m_MouseOverObject.iSkillIdx;

			if (pUIMan && idx <QUEST_NAME_CHK_BT_CNT)
			{
				std::vector<DWORD>* pvecQ = NULL;

				//step1:카테고리설정
				switch(m_eDlgState)
				{
				case QUEST_CATEGORY_MISSION:pvecQ = &m_vecMainQuest; break;
				case QUEST_CATEGORY_QUEST:pvecQ = &m_vecSubQuest;break;
				}
				if (!pvecQ) return;
				
				//step2:퀘스트 얻기
				Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[idx]]);
				if (NULL == pQ)
				{
					pQ = g_pQMGR_Temp->FindQuest((QCODE)(*pvecQ)[m_vecVisCurChapterQuestIdx[idx]]);
					if (NULL == pQ) return;
				}
				//step3:퀘스트명랜더링
				TCHAR szQName[INTERFACE_STRING_LENGTH + 1] = {0,};
				g_InterfaceManager.GetInterfaceString(pQ->GetQuestInfo()->GetQNCode(),szQName);

				// (완료) ( 9051 )
				if (QS_COMPLETED == pQ->GetQState())
				{
					TCHAR szSuc[INTERFACE_STRING_LENGTH] = {0,};
					GetUIMan()->GetInterfaceString( 9051, szSuc);                     
					_tcscat(szQName, szSuc );

				}
				pUIMan->InitTooltip();
				pUIMan->RegisterTooltipSimpleText(szQName);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}

			int iMouseX = Mouse::Instance()->GetMouseX();
			int iMouseY = Mouse::Instance()->GetMouseY();

			BOOL bMouseOver = FALSE;

			if (bMouseOver==FALSE)
			{
				RECT rc = m_pCtrlQuestNameBtn[idx]->GetSizeRect();

				if (rc.left <= iMouseX && rc.right >= iMouseX)
				{
					if (rc.top <= iMouseY && rc.bottom >= iMouseY)
					{
						bMouseOver = TRUE;
					}
				}
			}

			if ( !bMouseOver )
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				return;
			}
		}
	}
}
#endif//_INTERNATIONAL_UI
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::RePosQuestList()
{
	if (NULL == m_pCtrlScroll)
		return;

	m_vecVisCurChapterQuestIdx.clear();
	AllHideQuestName();

	int iQCnt = 0;
	int iScrollBarPos = m_pCtrlScroll->GetScrollPosWZ();

	std::vector<DWORD>* pvecQ = NULL;
	if (QUEST_CATEGORY_MISSION == m_eDlgState)
	{
		pvecQ = &m_vecMainQuest;
	}
	else if (QUEST_CATEGORY_QUEST == m_eDlgState)
	{
		pvecQ = &m_vecSubQuest;
	}
	else
	{
		assert(!"uiQuestListDlg::RePosQuestList()");
		return;
	}

	iQCnt = pvecQ->size();

	std::vector<int> vecQIdxTemp;
    STLX_VECTOR<int> temp_vector_progress;
    STLX_VECTOR<int> temp_vector_end;
	if (m_pCtrlScroll)
	{
		int iSameChapterQCode = 0;
		for(int i = 0; i < iQCnt; ++i)
		{
			Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[i]);
			if (NULL == pQ)
			{
				pQ = g_pQMGR_Temp->FindQuest((QCODE)(*pvecQ)[i]);
				if (NULL == pQ)
				{
					assert(!"RePosQuestList()::(NULL == pQ)");
					return;
				}
			}

            if ((pQ->GetQuestInfo()->GetGroupCode() != m_dwChapterCode) && 
                (m_dwChapterCode != kAllChapterCode))
            {
                continue;
            }

            switch (m_eQDpType)
            {
            case eQUEST_DISPLAY_TYPE_ALL:
                {
                    vecQIdxTemp.push_back(i);
                    break;
                }
            case eQUEST_DISPLAY_TYPE_PRG:
            case eQUEST_DISPLAY_TYPE_END:
                {
                    if (g_pQMGR_Real->FindQuest(pQ->GetQuestCode()) != NULL)
                    {
                        temp_vector_progress.push_back(i);
                    }
                    else
                    {
                        temp_vector_end.push_back(i);
                    }
                }
                break;
            }
            iSameChapterQCode++;
        }//for();

        if (eQUEST_DISPLAY_TYPE_PRG == m_eQDpType)
        {
            vecQIdxTemp.insert(vecQIdxTemp.end(),
                temp_vector_progress.begin(), temp_vector_progress.end());
            vecQIdxTemp.insert(vecQIdxTemp.end(),
                temp_vector_end.begin(), temp_vector_end.end());
        }
        else if (eQUEST_DISPLAY_TYPE_END == m_eQDpType)
        {
            vecQIdxTemp.insert(vecQIdxTemp.end(),
                temp_vector_end.begin(), temp_vector_end.end());
            vecQIdxTemp.insert(vecQIdxTemp.end(),
                temp_vector_progress.begin(), temp_vector_progress.end());
        }

        if (QUEST_NAME_CHK_BT_CNT < iSameChapterQCode)
        {
            m_pCtrlScroll->SetScrollRangeWZ(0, iSameChapterQCode - QUEST_NAME_CHK_BT_CNT);
            iScrollBarPos = m_pCtrlScroll->GetScrollPosWZ();
        }
        else
        {
            m_pCtrlScroll->SetScrollRangeWZ(0, 0);
            m_pCtrlScroll->SetScrollPosWZ(0);
            iScrollBarPos = m_pCtrlScroll->GetScrollPosWZ();
        }
    }

	int iMinus = 0;
	for(unsigned int i = iScrollBarPos; i < vecQIdxTemp.size(); ++i)
	{
		int iCnt = m_vecVisCurChapterQuestIdx.size();
		if (QUEST_NAME_CHK_BT_CNT <= iCnt)
        {
			return;
        }

		Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[vecQIdxTemp[i]]);
		if (NULL == pQ)
		{
			pQ = g_pQMGR_Temp->FindQuest((QCODE)(*pvecQ)[vecQIdxTemp[i]]);
			if (NULL == pQ)
			{
				assert(!"RePosQuestList()::(NULL == pQ)");
				return;
			}
		}

		m_vecVisCurChapterQuestIdx.push_back(vecQIdxTemp[i]);

		int FinalPos = i - iScrollBarPos - iMinus;

		TCHAR szQName[INTERFACE_STRING_LENGTH + 1] = {0,};
		g_InterfaceManager.GetInterfaceString(pQ->GetQuestInfo()->GetQNCode(), szQName);

		if (FinalPos >= QUEST_NAME_CHK_BT_CNT)
			continue;

		m_pCtrlQuestNameBtn[FinalPos]->ShowWindowWZ(WZ_SHOW);
#ifdef _SCJ_111202_UI_RENEWAL
        quset_check_bg_picture[FinalPos]->ShowWindowWZ(WZ_SHOW);
#endif //_SCJ_111202_UI_RENEWAL

		if (NULL == g_pQMGR_Real->FindQuest(pQ->GetQuestCode())) //보상까지 받은 종료된 퀘스트
		{
#ifdef _INTERNATIONAL_UI
			GlobalFunc::SetCaptionSimple( m_pCtrlQuestNameBtn[FinalPos], szQName,1);
#else
			m_pCtrlQuestNameBtn[FinalPos]->SetTextWZ(szQName);
#endif//_INTERNATIONAL_UI
			m_pCtrlQuestNameBtn[FinalPos]->SetTextColor(RGBA(94, 94, 94, 255));
		}
		else
		{
			if (QS_NORMAL == pQ->GetQState())
			{
#ifdef _INTERNATIONAL_UI
				GlobalFunc::SetCaptionSimple( m_pCtrlQuestNameBtn[FinalPos], szQName,1);
#else
				m_pCtrlQuestNameBtn[FinalPos]->SetTextWZ(szQName);
#endif//_INTERNATIONAL_UI
				m_pCtrlQuestNameBtn[FinalPos]->SetTextColor(RGBA(213, 201, 171, 255));
			}
			else if (QS_COMPLETED == pQ->GetQState())
			{
				TCHAR szSuc[INTERFACE_STRING_LENGTH] = {0,};
				// (완료) ( 9051 )
				GetUIMan()->GetInterfaceString( 9051, szSuc);                     
				_tcscat(szQName, szSuc );
#ifdef _INTERNATIONAL_UI
				GlobalFunc::SetCaptionSimple( m_pCtrlQuestNameBtn[FinalPos], szQName,1);
#else
				m_pCtrlQuestNameBtn[FinalPos]->SetTextWZ(szQName);
#endif//_INTERNATIONAL_UI
				m_pCtrlQuestNameBtn[FinalPos]->SetTextColor(RGBA(213, 201, 171, 255));
			}
		}
	}

	vecQIdxTemp.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::SetQuestCntAndChapterCnt()
{
    //g_pQMGR_Real->QuestNum

	if (m_pCtrlQuestNum)
	{
		std::vector<DWORD>* pvecQ = NULL;
        std::vector<DWORD>* pmapChapterCode = NULL;
        std::vector<DWORD> temp_chapter_code;
		if (QUEST_CATEGORY_MISSION == m_eDlgState)
		{
			pvecQ = &m_vecMainQuest;
			pmapChapterCode = &m_MainQChapterCodes;
		}
		else if (QUEST_CATEGORY_QUEST == m_eDlgState)
		{
			pvecQ = &m_vecSubQuest;
			pmapChapterCode = &m_SubQChapterCodes;
		}
		else
		{
			assert(!"uiQuestListDlg::SetQuestCntAndChapterCnt()");
			return;
		}

		int iProcessQCnt = 0;
		int iQCnt = 0;
        std::vector<DWORD>::iterator iter;

		iQCnt = pvecQ->size();
		for(int i = 0; i < iQCnt; ++i)
		{
			Quest* pQ = g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[i]);
			if (NULL == pQ)
			{
				pQ = g_pQMGR_Temp->FindQuest((QCODE)(*pvecQ)[i]);
				if (NULL == pQ)
				{
					assert(!"uiQuestListDlg::SetQuestCntAndChapterCnt()");
					return;
				}
			}
			DWORD dwGroupCode = pQ->GetQuestInfo()->GetGroupCode();
            iter = std::find(temp_chapter_code.begin(), temp_chapter_code.end(), dwGroupCode);
            if (iter == temp_chapter_code.end())
			{
                temp_chapter_code.push_back(dwGroupCode);
			}

			if ( NULL != g_pQMGR_Real->FindQuest((QCODE)(*pvecQ)[i]) )
				iProcessQCnt++;
		}
        pmapChapterCode->push_back(kAllChapterCode);

		TCHAR szTemp[16] = {0,};
		Snprintf(szTemp, 16-1, _T("%d/%d"), iProcessQCnt, iQCnt);
		m_pCtrlQuestNum->SetTextWZ(szTemp);

		TCHAR szChapterName[INTERFACE_STRING_LENGTH + 1] = {0,};

        // 2012.12.05 / 송찬종 / 챕터가 저장이 안되는 현상 수정
        //m_pCtrlChapterList->DeleteItemAll();를 하면서 CurSel 이 풀려서 생긴 현상
        int current_sell = m_pCtrlChapterList->GetCurSel();

		m_pCtrlChapterList->DeleteItemAll();

        std::sort(temp_chapter_code.begin(), temp_chapter_code.end(), std::greater<DWORD>());
        pmapChapterCode->insert(pmapChapterCode->end(), temp_chapter_code.begin(), temp_chapter_code.end());

        iter = pmapChapterCode->begin();
        while(iter != pmapChapterCode->end())
		{
            g_InterfaceManager.GetInterfaceString((*iter) ,szChapterName);
			int idx = m_pCtrlChapterList->InsertItemBack();
			GlobalFunc::SetItem(m_pCtrlChapterList, idx, 0, szChapterName, StrToWzID("mn12"), 
				DT_CENTER | DT_VCENTER, c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);

            ++iter;
		}
        // 2012.12.05 / 송찬종 / 챕터가 저장이 안되는 현상 수정
        //m_pCtrlChapterList->DeleteItemAll();를 하면서 CurSel 이 풀려서 생긴 현상
        m_pCtrlChapterList->SetCurSel(current_sell);

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiQuestListDlg::RefreshQuestList()
{
	if (NULL == GetManager())
		return;

	m_vecMainQuest.clear();
	m_vecSubQuest.clear();
	m_MainQChapterCodes.clear();
	m_SubQChapterCodes.clear();

	const int FIRST_QUEST = 0;

	for (int i = MAX_QUEST_COMPL_NUM-1; i > FIRST_QUEST; --i)
	{
		Quest* pQuest = g_pQMGR_Real->FindQuest(i);
		if (NULL == pQuest)
		{
			pQuest = g_pQMGR_Temp->FindQuest(i);
			
			if (NULL == pQuest)
			{
				pQuest  = g_pQMGR_Temp->CreateQuest(i);

				if (!pQuest) continue;
			}


			QUEST_CONDITION_REPEAT_INFO* pQCR = (QUEST_CONDITION_REPEAT_INFO*)pQuest->GetQuestInfo()->FindAcceptCondition(eQUEST_CONDITION_REPEAT);
			if (NULL == pQCR)
				continue;

			QUESTPART_COMPL * pComplQ = g_pQMGR_Real->FindComplQuest( i );
			if (NULL == pComplQ || pComplQ->m_Num < pQCR->GetMaxRepeatNum()) 
				continue;
			else
			{
				int a = 0; //종료된 퀘스트
			}
		}
		
		if (QUEST_CATEGORY_MISSION == pQuest->GetQType())
			m_vecMainQuest.push_back(pQuest->GetQuestCode());
		else if (QUEST_CATEGORY_QUEST == pQuest->GetQType())
			m_vecSubQuest.push_back(pQuest->GetQuestCode());
		else
		{
			assert(!"uiQuestListDlg::RefreshQuestList()");
		}
	}	

	if (FALSE ==  m_bInit)
		return;

	if (m_pCtrlScroll)
		m_pCtrlScroll->SetScrollPosWZ(0);

	SelectChapter();
	SelectDPType();
    int scroll_max_value = QuestListCount(GetDlgState(), m_dwChapterCode, m_eQDpType) - QUEST_NAME_CHK_BT_CNT;
    m_pCtrlScroll->SetScrollRangeWZ(0, scroll_max_value);
    m_pCtrlScroll->SetScrollPosWZ(last_scrollbar_position(GetDlgState(), m_dwChapterCode, m_eQDpType));
	RePosQuestList();
	RefreshNaviBtn();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiQuestListDlg::updateMouseWheel()
{
	if (NULL == m_pCtrlScroll)
		return;

	RECT rt;
	this->GetDialogWZ()->GetClientRect(&rt);
	switch(GetMouseWheelStatus(&rt))
	{
		case MOUSE_WHEEL_UP:
			{
				int iPos = m_pCtrlScroll->GetScrollPosWZ();
				int iMin, iMax;
				int iListCnt = 0;
				m_pCtrlScroll->GetScrollRangeWZ(&iMin, &iMax);

				if (0 < iPos)
				{
					m_pCtrlScroll->SetScrollPosWZ(iPos - 1);
                    set_last_scrollbar_position(GetDlgState(), 
                                                m_dwChapterCode, 
                                                m_eQDpType, 
                                                m_pCtrlScroll->GetScrollPosWZ());
					RePosQuestList();
					RefreshNaviBtn();
				}
			}
			break;

		case MOUSE_WHEEL_DOWN:
			{
				int iPos = m_pCtrlScroll->GetScrollPosWZ();
				int iMin, iMax;
				int iListCnt = 0;
				m_pCtrlScroll->GetScrollRangeWZ(&iMin, &iMax);
				if (iMax > iPos)
				{
					m_pCtrlScroll->SetScrollPosWZ(iPos + 1);
                    set_last_scrollbar_position(GetDlgState(), 
                                                m_dwChapterCode,
                                                m_eQDpType, 
                                                m_pCtrlScroll->GetScrollPosWZ());
					RePosQuestList();
					RefreshNaviBtn();
				}
			}
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void	uiQuestListDlg::InitControl()
{
	m_pCtrlMissionTabBtn = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_QUEST_LIST_MISSION_BTN));
	assert(m_pCtrlMissionTabBtn);
	m_pCtrlQuestTabBtn = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_QUEST_LIST_QUEST_BTN));
	assert(m_pCtrlQuestTabBtn);

	m_pCtrlChapterList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_QUEST_LIST_CHAPTER_LIST));
	assert(m_pCtrlChapterList);
	if (m_pCtrlChapterList)
		m_pCtrlChapterList->ShowWindowWZ(WZ_HIDE);
	m_pCtrlChapterBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN));
	assert(m_pCtrlChapterBtn);
	m_pCtrlChapterBtn2 = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN2));
	assert(m_pCtrlChapterBtn2);
#ifdef _SCJ_111202_UI_RENEWAL
    if (m_pCtrlChapterBtn2 != NULL)
    {
        m_pCtrlChapterBtn2->SetTextWZ("");
    }
#else
    m_pCtrlChapterName = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_QUEST_LIST_CHAPTER_STRING));
    assert(m_pCtrlChapterName);
    if (m_pCtrlChapterName)
        m_pCtrlChapterName->SetTextWZ("");
#endif //_SCJ_111202_UI_RENEWAL

	m_pCtrlDPTypeList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_QUEST_LIST_DP_TYPE_LIST));
	assert(m_pCtrlDPTypeList);
	if (m_pCtrlDPTypeList)
	{
		TCHAR szDPType[INTERFACE_STRING_LENGTH + 1] = {0,};
		//601	전체
		g_InterfaceManager.GetInterfaceString(601,szDPType);
		int idx = m_pCtrlDPTypeList->InsertItemBack();
		GlobalFunc::SetItem(m_pCtrlDPTypeList, idx, 0, szDPType, StrToWzID("mn12"), 
			DT_CENTER | DT_VCENTER, c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);

		//1664	진행
		g_InterfaceManager.GetInterfaceString(1664,szDPType);
		idx = m_pCtrlDPTypeList->InsertItemBack();
		GlobalFunc::SetItem(m_pCtrlDPTypeList, idx, 0, szDPType, StrToWzID("mn12"), 
			DT_CENTER | DT_VCENTER, c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);

		//859	종료
		g_InterfaceManager.GetInterfaceString(859,szDPType);
		idx = m_pCtrlDPTypeList->InsertItemBack();
		GlobalFunc::SetItem(m_pCtrlDPTypeList, idx, 0, szDPType, StrToWzID("mn12"), 
			DT_CENTER | DT_VCENTER, c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);

		m_pCtrlDPTypeList->ShowWindowWZ(WZ_HIDE);
	}
	m_pCtrlDPTypeBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN));
	assert(m_pCtrlDPTypeBtn);
	m_pCtrlDPTypeBtn2 = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN2));
	assert(m_pCtrlDPTypeBtn2);
#ifdef _SCJ_111202_UI_RENEWAL
    if (m_pCtrlDPTypeBtn2 != NULL)
    {
        //601	전체
        TCHAR szDPType[INTERFACE_STRING_LENGTH + 1] = {0,};
        g_InterfaceManager.GetInterfaceString(601, szDPType);
        m_pCtrlDPTypeBtn2->SetTextWZ(szDPType);
    }
#else
	m_pCtrlDPTypeName = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_QUEST_LIST_DP_TYPE_STRING));
	assert(m_pCtrlDPTypeName);
	if (m_pCtrlDPTypeName)
	{
		//601	전체
		TCHAR szDPType[INTERFACE_STRING_LENGTH + 1] = {0,};
		g_InterfaceManager.GetInterfaceString(601, szDPType);
		m_pCtrlDPTypeName->SetTextWZ(szDPType);
	}
#endif //_SCJ_111202_UI_RENEWAL

	for(int i = 0; i < QUEST_NAME_CHK_BT_CNT; ++i)
	{
		m_pCtrlQuestNameBtn[i] = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_QUEST_NAME_BT_START + i));
		assert(m_pCtrlQuestNameBtn[i]);
		if (m_pCtrlQuestNameBtn[i])
		{
			m_pCtrlQuestNameBtn[i]->SetTextOffsetY(-1l);
			m_pCtrlQuestNameBtn[i]->ShowWindowWZ(WZ_HIDE);
#ifdef _SCJ_LONG_TEXT_REDUCING
			m_pCtrlQuestNameBtn[i]->SetTextReducing(true);
#endif //_SCJ_LONG_TEXT_REDUCING
		}

		m_pCtrlQuestNaviBtn[i] = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_QUEST_LIST_NAVI_BTN_START + i));
		assert(m_pCtrlQuestNaviBtn[i]);
	}

	for(int i = 0; i < QUEST_CHK_PICTURE_KIND; ++i)
	{
		for(int j = 0; j < QUEST_NAME_CHK_BT_CNT; ++j)
		{
			m_pCtrlQusetChkPicture[j][i] = 
				static_cast<CCtrlPictureWZ*>(getControl(eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_START + (QUEST_NAME_CHK_BT_CNT * i) + j));
			assert(m_pCtrlQusetChkPicture[j][i]);
			if (m_pCtrlQusetChkPicture[j][i])
				m_pCtrlQusetChkPicture[j][i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	m_pCtrlQuestNum = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_QUEST_LIST_QUEST_NUM_STRING));
	assert(m_pCtrlQuestNum);
	if (m_pCtrlQuestNum)
		m_pCtrlQuestNum->SetTextWZ("0/0");
	
	m_pCtrlScroll = static_cast<CCtrlVScrollWZ*>(getControl(eDIALOG_QUEST_LIST_SCROLL));
	assert(m_pCtrlScroll);
	if (m_pCtrlScroll)
	{
		m_pCtrlScroll->SetScrollRangeWZ(0, 0);
		m_pCtrlScroll->SetScrollPosWZ(0);

        m_pCtrlScroll->SetScrollPosWZ(last_scrollbar_position(GetDlgState(), m_dwChapterCode, m_eQDpType));
	}

#ifdef _SCJ_111202_UI_RENEWAL
    for(int i = 0; i < QUEST_NAME_CHK_BT_CNT; ++i)
    {
        quset_check_bg_picture[i] = static_cast<CCtrlPictureWZ*>(getControl(eDIALOG_QUEST_LIST_QUEST_CHK_BG_PICTURE_START + i));
        assert(quset_check_bg_picture[i]);
        if (quset_check_bg_picture[i])
        {
            quset_check_bg_picture[i]->ShowWindowWZ(WZ_HIDE);
        }
    }
#endif //_SCJ_111202_UI_RENEWAL

	m_bInit = TRUE;
}
//------------------------------------------------------------------------------
void uiQuestListDlg::AllHideQuestName()
{
    for(int i = 0; i < QUEST_NAME_CHK_BT_CNT; ++i)
    {
        m_pCtrlQuestNameBtn[i]->ShowWindowWZ(WZ_HIDE);
#ifdef _SCJ_111202_UI_RENEWAL
        quset_check_bg_picture[i]->ShowWindowWZ(WZ_HIDE);
#endif //_SCJ_111202_UI_RENEWAL
    }
}
//------------------------------------------------------------------------------
DWORD uiQuestListDlg::GetNaviQCodes(BYTE btNaviIdx)
{
    if (btNaviIdx < MAX_NAVI_CNT)
    {
        return m_dwNaviQCode[btNaviIdx];
    }
    assert(false);
    return 0;
}

//------------------------------------------------------------------------------
int uiQuestListDlg::last_scrollbar_position(const eQUEST_CATEGORY quest_category,
                                            const DWORD qeust_chapter_groupcode, 
                                            const eQUEST_DISPLAY_TYPE quest_display_type)
{
    std::vector<ScrollbarPosition>::iterator begin = last_scrollbar_position_.begin();
    std::vector<ScrollbarPosition>::iterator end = last_scrollbar_position_.end();

    for (; begin != end; ++begin)
    {
        if ((begin->quest_category() == quest_category) &&
            (begin->qeust_chapter_groupcode() == qeust_chapter_groupcode) &&
            (begin->quest_display_type() == quest_display_type))
        {
            return begin->scrollbar_position();
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
void uiQuestListDlg::set_last_scrollbar_position(const eQUEST_CATEGORY quest_category, 
                                                 const DWORD qeust_chapter_groupcode, 
                                                 const eQUEST_DISPLAY_TYPE quest_display_type, 
                                                 const int scrollbar_position)
{
    std::vector<ScrollbarPosition>::iterator begin = last_scrollbar_position_.begin();
    std::vector<ScrollbarPosition>::iterator end = last_scrollbar_position_.end();

    for (; begin != end; ++begin)
    {
        if ((begin->quest_category() == quest_category) &&
            (begin->qeust_chapter_groupcode() == qeust_chapter_groupcode) &&
            (begin->quest_display_type() == quest_display_type))
        {
            begin->set_scrollbar_position(scrollbar_position);
            return;
        }
    }

    last_scrollbar_position_.push_back(ScrollbarPosition(quest_category, 
                                                        qeust_chapter_groupcode, 
                                                        quest_display_type,
                                                        scrollbar_position));
}
//------------------------------------------------------------------------------
int uiQuestListDlg::QuestListCount(const eQUEST_CATEGORY quest_category, 
                                     const DWORD quest_chapter_groupCode, 
                                     const eQUEST_DISPLAY_TYPE quest_display_type)
{
    std::vector<DWORD>* quest_vector_ptr = NULL;
    if (QUEST_CATEGORY_MISSION == quest_category)
    {
        quest_vector_ptr = &m_vecMainQuest;
    }
    else if (QUEST_CATEGORY_QUEST == quest_category)
    {
        quest_vector_ptr = &m_vecSubQuest;
    }
    else
    {
        return 0;
    }

    int filter_quest_count = 0;
    int quest_count = quest_vector_ptr->size();

    for(int i = 0; i < quest_count; ++i)
    {
        QCODE quest_code = static_cast<QCODE>((*quest_vector_ptr)[i]);
        Quest* quest = g_pQMGR_Real->FindQuest(quest_code);
        if (NULL == quest)
        {
            quest = g_pQMGR_Temp->FindQuest(quest_code);
            if (NULL == quest)
            {
                break;
            }
        }

        if (eQUEST_DISPLAY_TYPE_PRG == quest_display_type)
        {
            if (g_pQMGR_Real->FindQuest(quest->GetQuestCode()) == NULL) 
            {
                continue;
            }
        }
        else if (eQUEST_DISPLAY_TYPE_END == quest_display_type)
        {
            if (g_pQMGR_Real->FindQuest(quest->GetQuestCode()) != NULL)
            {
                continue;
            }
        }

        if (quest->GetQuestInfo()->GetGroupCode() == quest_chapter_groupCode)
        {
            ++filter_quest_count;
        }
    }

    return filter_quest_count;
}
//------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

