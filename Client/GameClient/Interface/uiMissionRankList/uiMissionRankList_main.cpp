#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiMissionRankList.h"
#include "uiMissionRankMan/uiMissionRankMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "MapInfoParser.h"

#define MAX_SUB_MISSION_COUNT	5

const int c_iDefaultRankLine = 3;
const int c_iDefaultMemberLine = 4;

#ifdef _INTERNATIONAL_UI
	const DWORD c_iListFont = StrToWzID("st10"); 
#else
	const DWORD c_iListFont = StrToWzID("m212"); 
#endif//_INTERNATIONAL_UI

const DWORD c_ListSelBarColor	= RGBA(0,51,102,100);
const DWORD c_ListBKColor		= RGBA(255,255,255,0);
const DWORD c_ListTextColor		= RGBA(255,255,255,255);
const DWORD c_ListSelTextColor	= RGBA(255,255,255,255);

//GS_BUG_FIX:미션순서를 정렬할 함수객체 정의 내림차순으로
inline BOOL MapListSort_Funter::operator()(sMAPINFO *lhs, sMAPINFO* rhs) const
{
	return ((lhs->byMType)<(rhs->byMType));

}
//------------------------------------------------------------------------------
WzID uiMissionRankList::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	// 종료						//DLGPOS_MRL_BTN_EXIT,

		StrToWzID("T002"),	// 결산일					//DLGPOS_MRL_TXT_DATE,
		StrToWzID("T006"),	// 지역	텍스트				//DLGPOS_MRL_TXT_AREA,
		StrToWzID("T008"),	// 임무	텍스트				//DLGPOS_MRL_TXT_MISSION,
		StrToWzID("T200"),	// 페이지 표시				//DLGPOS_MRL_TXT_PAGE,

		StrToWzID("LS00"),	// 명예의 전당 리스트		//DLGPOS_MRL_LST_FAME,
		StrToWzID("L001"),	// 주간순위 리스트			//DLGPOS_MRL_LST_WEEK,
		StrToWzID("L005"),	// 전 주간순위 리스트		//DLGPOS_MRL_LST_PREWEEK,
		StrToWzID("L002"),	// 구성원 정보 리스트		//DLGPOS_MRL_LST_MEMBER,
		StrToWzID("L004"),	// 지역(메인미션) 리스트	//DLGPOS_MRL_LST_AREA,
		StrToWzID("L003"),	// 임무(서브미션)리스트		//DLGPOS_MRL_LST_MISSION,

		StrToWzID("S001"),	// 주간순위 스크롤			//DLGPOS_MRL_VSCR_WEEK,
		StrToWzID("S002"),	// 전 주간순위 스크롤		//DLGPOS_MRL_VSCR_PREWEEK,
		StrToWzID("S003"),	// 구성원 스크롤			//DLGPOS_MRL_VSCR_MEMBER,

		StrToWzID("B001"),	// 지역(메인미션)선택 버튼	//DLGPOS_MRL_BTN_AREA,
		StrToWzID("B002"),	// 임무(서브미션)선택 버튼	//DLGPOS_MRL_BTN_MISSION,
		StrToWzID("B104"),	// 보상받기 버튼			//DLGPOS_MRL_BTN_REWARD,
		StrToWzID("B000"),	// 닫기 버튼				//DLGPOS_MRL_BTN_CLOSE,
		StrToWzID("B105"),	// 페이지 인덱스 감소 버튼(<-)		//DLGPOS_MRL_BTN_PAGE_LEFT,
		StrToWzID("B106"),	// 페이지 인덱스 증가 버튼(->)		//DLGPOS_MRL_BTN_PAGE_RIGHT,

		StrToWzID("T001"),	// DLGPOS_MRL_TXT_TITLE,

		StrToWzID("B009"),	// DLGPOS_MRL_BTN_NAME1,
		StrToWzID("BT01"),	// DLGPOS_MRL_BTN_NAME2,
		StrToWzID("B101"),	// DLGPOS_MRL_BTN_NAME3,

		StrToWzID("B005"),	// DLGPOS_MRL_BTN_POINT1,
		StrToWzID("B003"),	// DLGPOS_MRL_BTN_POINT2,
		StrToWzID("B103"),	// DLGPOS_MRL_BTN_POINT3,

		StrToWzID("CT00"),
		StrToWzID("CT01"),

		StrToWzID("BT00"),
		StrToWzID("BT01"),
		StrToWzID("BT02"),
		StrToWzID("B003"),

		StrToWzID("BT07"),
		StrToWzID("BT06"),
		StrToWzID("BT04"),

		StrToWzID("B008"),	
};

//------------------------------------------------------------------------------
/**
*/
uiMissionRankList::uiMissionRankList(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pMissionRankMan = GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );
    assert (m_pMissionRankMan);

    if (!this->m_pMissionRankMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pMissionRankMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_bShow = false;
	m_bShowMainList = false;
	m_bShowSubList = false;

	m_iCurMainListIndex = -1;
	m_iCurSubListIndex	= -1;
	m_MissionNo			= 0;

	m_iCurEventMainListIndex = -1;
	m_iCurEventSubListIndex	= -1;
}

//------------------------------------------------------------------------------
/**
*/
uiMissionRankList::~uiMissionRankList()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::InitControls()
{
    int i=0;
	for(i=0; i<POS_TXT_SIZE; ++i)
	{
		m_pTxtControl[i] = NULL;
		m_pTxtControl[i] = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_DATE + i));
	}

	for(i=0; i<POS_LST_SIZE; ++i)
	{
		m_pListControl[i] = NULL;
		m_pListControl[i] = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_MRL_LST_FAME + i));
	}

	for(i=0; i<POS_BTN_SIZE; ++i)
	{
		m_pBtnControls[i] = NULL;
		m_pBtnControls[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_BTN_AREA + i));
	}

	for(i=0; i<POS_VSCR_SIZE; ++i)
	{
		m_pScroll[i] = NULL;
		m_pScroll[i] = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_MRL_VSCR_WEEK + i));
	}
	

	if(m_pScroll[MRL_VSCR_PREWEEK])
		m_pScroll[MRL_VSCR_PREWEEK]->ShowWindowWZ( WZ_SHOW );


	m_pTxtTitle = NULL;
	m_pTxtTitle = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_TITLE));

	for(i=0; i<POS_LST_BTN_SIZE; ++i)
	{
		m_pLstBtns[i] = NULL;
		m_pLstBtns[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_BTN_NAME1 + i));
	}

	m_pWeekCheckButton		= static_cast<CCtrlButtonCheckWZ *>( getControl( DLGPOS_MRL_BTN_WEEK ) );
	m_pPreWeekCheckButton	= static_cast<CCtrlButtonCheckWZ *>( getControl( DLGPOS_MRL_BTN_PREWEEK ) );

    this->m_bControlsInitialized = true;

}

//------------------------------------------------------------------------------
void uiMissionRankList::_initRankListSelect()
{
	if(m_pListControl[MRL_LST_FAME])
		m_pListControl[MRL_LST_FAME]->SetCurSel(-1);

	if(m_pListControl[MRL_LST_WEEK])
		m_pListControl[MRL_LST_WEEK]->SetCurSel(-1);

	if(m_pListControl[MRL_LST_PREWEEK])
		m_pListControl[MRL_LST_PREWEEK]->SetCurSel(-1);

	_initMemberList();
}

//------------------------------------------------------------------------------
void uiMissionRankList::_initRankList()
{
	CControlWZ * pRate = getControl( DLGPOS_MRL_STATIC_RATE );
	CControlWZ * pPartyNumber = getControl( DLGPOS_MRL_STATIC_PARTY_NUMBER );
	CControlWZ * pLevel = getControl( DLGPOS_MRL_STATIC_LEVEL );
	CControlWZ * pTime = getControl( DLGPOS_MRL_STATIC_TIME );

	CControlWZ * pFramePartyNumber = getControl( DLGPOS_MRL_BTN_NAME1 );
	CControlWZ * pFrameLevel = getControl( DLGPOS_MRL_STATIC_FLAME_LEVEL );
	CControlWZ * pFrameTime = getControl( DLGPOS_MRL_BTN_POINT1 );

	if(m_pListControl[MRL_LST_FAME])
	{
		m_pListControl[MRL_LST_FAME]->DeleteItemAll();
		m_pListControl[MRL_LST_FAME]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_FAME]->SetUseReport(true);
		m_pListControl[MRL_LST_FAME]->SetReportUseCnt(3);
		m_pListControl[MRL_LST_FAME]->SetHideReportRender(true);
#ifdef _NA_000000_20130114_RENEWER_UI
		m_pListControl[MRL_LST_FAME]->SetStartLineGapTop(4);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_FAME], 0, pFramePartyNumber->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_FAME], 1, pFrameLevel->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_FAME], 2, pFrameTime->GetSize().right, NULL );
	}

	if(m_pListControl[MRL_LST_WEEK])
	{
		m_pListControl[MRL_LST_WEEK]->DeleteItemAll();
		m_pListControl[MRL_LST_WEEK]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_WEEK]->SetUseReport(true);
		m_pListControl[MRL_LST_WEEK]->SetReportUseCnt(4);
		m_pListControl[MRL_LST_WEEK]->SetHideReportRender(true);
#ifdef _NA_000000_20130114_RENEWER_UI
		m_pListControl[MRL_LST_WEEK]->SetStartLineGapTop(4);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_WEEK], 0, pRate->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_WEEK], 1, pPartyNumber->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_WEEK], 2, pLevel->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_WEEK], 3, pTime->GetSize().right, NULL );
	}

	if(m_pListControl[MRL_LST_PREWEEK])
	{
		m_pListControl[MRL_LST_PREWEEK]->DeleteItemAll();
		m_pListControl[MRL_LST_PREWEEK]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_PREWEEK]->SetUseReport(true);
		m_pListControl[MRL_LST_PREWEEK]->SetReportUseCnt(4);
		m_pListControl[MRL_LST_PREWEEK]->SetHideReportRender(true);
#ifdef _NA_000000_20130114_RENEWER_UI
		m_pListControl[MRL_LST_PREWEEK]->SetStartLineGapTop(4);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_PREWEEK], 0, pRate->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_PREWEEK], 1, pPartyNumber->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_PREWEEK], 2, pLevel->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_PREWEEK], 3, pTime->GetSize().right, NULL );
	}
}

//------------------------------------------------------------------------------
void uiMissionRankList::_initMemberList()
{
	if(m_pListControl[MRL_LST_MEMBER])
	{
		m_pListControl[MRL_LST_MEMBER]->DeleteItemAll();
		m_pListControl[MRL_LST_MEMBER]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_MEMBER]->SetUseReport(true);
		m_pListControl[MRL_LST_MEMBER]->SetReportUseCnt(3);
		m_pListControl[MRL_LST_MEMBER]->SetHideReportRender(true);
		m_pListControl[MRL_LST_MEMBER]->SetSellHeight(21);
#ifdef _NA_000000_20130114_RENEWER_UI
		m_pListControl[MRL_LST_MEMBER]->SetStartLineGapTop(4);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		m_pListControl[MRL_LST_MEMBER]->SetCurSel(-1);

		if(m_pScroll[MRL_VSCR_MEMBER])
		{
			m_pScroll[MRL_VSCR_MEMBER]->SetScrollRangeWZ(0,0);	//스크롤 범위
			m_pScroll[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
		}
	}
}

//------------------------------------------------------------------------------
void uiMissionRankList::_initPageSet()
{
	if(m_pListControl[MRL_LST_WEEK])
		m_pListControl[MRL_LST_WEEK]->SetCurSel(-1);

	_initMemberList();
}

//------------------------------------------------------------------------------
void uiMissionRankList::_prepareDisplay()
{
	m_bShow = true;
	m_bShowMainList = false;
	m_bShowSubList = false;
	
	_setTitle();
	SetAreaList();

	_initRankListSelect();

	// 이번주 주간 순위
	if(m_pScroll[MRL_VSCR_WEEK])
	{
		m_pScroll[MRL_VSCR_WEEK]->SetScrollPosWZ(0);
	}

	// 지난주 주간 순위
	if(m_pScroll[MRL_VSCR_PREWEEK])
	{
		m_pScroll[MRL_VSCR_PREWEEK]->SetScrollPosWZ(0);
	}

	// 구성원 정보
	if(m_pScroll[MRL_VSCR_MEMBER])
	{
		m_pScroll[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
	}

	GetManager()->ClearRankData();

	UpdateMissionSelectList();
	UpdateMissionText(true);
	_initRankList();
}

//------------------------------------------------------------------------------
void uiMissionRankList::OnShowWindow( BOOL val )
{
    if (val)
    {
        //uiBase::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		_prepareDisplay();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_mission_rank_list;
		if (GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK)
		{
			msg.wParam = IM_MISSION_RANK_MANAGER::MissionRankList;
		}
		else if (GetManager()->GetRankingSystemType() == eRANKING_TYPE_POINT)
		{
			msg.wParam = IM_MISSION_RANK_MANAGER::PointRankList;
		}		
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

		if( GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK )
		{
			if( m_pWeekCheckButton )
				m_pWeekCheckButton->SetCheckState( TRUE );

			if( m_pPreWeekCheckButton )
				m_pPreWeekCheckButton->SetCheckState( FALSE );

			m_pListControl[MRL_LST_WEEK]->ShowWindowWZ( WZ_SHOW );
			m_pListControl[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_HIDE );
		}

	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_rank_list);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiMissionRankList::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_MRL_BTN_EXIT:
			case DLGPOS_MRL_BTN_CLOSE:
                {
					m_bShow = false;
                }
                break;

			case DLGPOS_MRL_BTN_PAGE_LEFT:
				{
					if(GetManager()->DecPage() == true)
					{
						_initRankListSelect();
						UpdatePageText();
					}
					
				}
				break;

			case DLGPOS_MRL_BTN_PAGE_RIGHT:
				{
					if(GetManager()->IncPage() == true)
					{
						_initRankListSelect();
						UpdatePageText();
					}
				}
				break;

			case DLGPOS_MRL_BTN_AREA:
				{
					m_bShowMainList = !m_bShowMainList;

					if(m_bShowMainList)
						m_bShowSubList = false;

					UpdateMissionSelectList();
				}
				break;

			case DLGPOS_MRL_BTN_MISSION:
				{
					m_bShowSubList = !m_bShowSubList;

					if(m_bShowSubList)
						m_bShowMainList = false;

					UpdateMissionSelectList();
				}
				break;

			case DLGPOS_MRL_BTN_REWARD:
				{
					if(GetManager()->IsEnableReward())
					{
						GetManager()->ShowRankGamble();
					}
				}
				break;

			case DLGPOS_MRL_LST_AREA:
				{
					if(m_pListControl[MRL_LST_AREA] && (m_pListControl[MRL_LST_AREA]->GetCurSel() >= 0))
					{
						if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) // 미션랭킹
						{
							m_iCurMainListIndex = m_pListControl[MRL_LST_AREA]->GetCurSel();
						}
						else	// 포인트 랭킹
						{
							m_iCurEventMainListIndex = m_pListControl[MRL_LST_AREA]->GetCurSel();
						}

						m_bShowMainList = false;
						m_bShowSubList = false;
						
						SetMissionList();
						UpdateMissionSelectList();
						UpdateMissionText(false);

						_initRankListSelect();
					}
					else
					{
						m_bShowMainList = false;
						m_bShowSubList = false;

						UpdateMissionSelectList();
					}
				}
				break;

			case DLGPOS_MRL_LST_MISSION:
				{
					if(m_pListControl[MRL_LST_MISSION] && (m_pListControl[MRL_LST_MISSION]->GetCurSel() >= 0))
					{
						if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) // 미션랭킹
						{
							m_iCurSubListIndex = m_pListControl[MRL_LST_MISSION]->GetCurSel();
						}
						else	// 포인트 랭킹
						{
							m_iCurEventSubListIndex = m_pListControl[MRL_LST_MISSION]->GetCurSel();
						}
						m_bShowMainList = false;
						m_bShowSubList = false;

						UpdateMissionSelectList();
						UpdateMissionText(false);

						_initRankListSelect();
					}
					else
					{
						m_bShowMainList = false;
						m_bShowSubList = false;

						UpdateMissionSelectList();
					}
				}
				break;

			case DLGPOS_MRL_LST_FAME:
				{
					m_pListControl[MRL_LST_WEEK]->SetCurSel(-1);
					m_pListControl[MRL_LST_PREWEEK]->SetCurSel(-1);
					
					if( GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK )
                    {
						m_pListControl[MRL_LST_MEMBER]->SetCurSel(-1);
                    }

					if(GetManager()->GetRankingSystemType() != eRANKING_TYPE_TIME_ATTACK) //미션랭킹
					{
						break;
					}

					int iIndex = m_pListControl[MRL_LST_FAME]->GetCurSel();

					if(iIndex >= 0)
                    {
						GetManager()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(m_MissionNo, iIndex, eRANKING_FAME);
                    }
				}
				break;

			case DLGPOS_MRL_LST_WEEK:
				{
					m_pListControl[MRL_LST_FAME]->SetCurSel(-1);
					m_pListControl[MRL_LST_PREWEEK]->SetCurSel(-1);

					if( GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK )
                    {
						m_pListControl[MRL_LST_MEMBER]->SetCurSel(-1);
                    }

					if(GetManager()->GetRankingSystemType() != eRANKING_TYPE_TIME_ATTACK) //미션랭킹
					{
						break;
					}

					int iIndex = m_pListControl[MRL_LST_WEEK]->GetCurSel();
					
					if(iIndex >= 0)
                    {
						GetManager()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(m_MissionNo, iIndex, eRANKING_TODAYWEEK);
                    }
				}
				break;

			case DLGPOS_MRL_LST_PREWEEK:
				{
					m_pListControl[MRL_LST_FAME]->SetCurSel(-1);
					m_pListControl[MRL_LST_WEEK]->SetCurSel(-1);

					if( GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK )
                    {
						m_pListControl[MRL_LST_MEMBER]->SetCurSel(-1);
                    }

					if(GetManager()->GetRankingSystemType() != eRANKING_TYPE_TIME_ATTACK) //미션랭킹
					{
						break;
					}

					int iIndex = m_pListControl[MRL_LST_PREWEEK]->GetCurSel();
					
					if(iIndex >= 0)
                    {
						GetManager()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(m_MissionNo, iIndex, eRANKING_LASTWEEK);
                    }
				}
				break;

			case DLGPOS_MRL_LST_MEMBER:
				{
				}
				break;

			case DLGPOS_MRL_BTN_WEEK:
				{
					m_pWeekCheckButton->SetCheckState( TRUE );
					m_pPreWeekCheckButton->SetCheckState( FALSE );

					m_pListControl[MRL_LST_WEEK]->ShowWindowWZ( WZ_SHOW );
					m_pListControl[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_HIDE );

				}
				break;

			case DLGPOS_MRL_BTN_PREWEEK:
				{
					m_pWeekCheckButton->SetCheckState( FALSE );
					m_pPreWeekCheckButton->SetCheckState( TRUE );

					m_pListControl[MRL_LST_WEEK]->ShowWindowWZ( WZ_HIDE );
					m_pListControl[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_SHOW );
				}
				break;

            }
        }
        break;

	case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case DLGPOS_MRL_VSCR_WEEK:
				{
					if(m_pScroll[MRL_VSCR_WEEK])
					{
						int pos = m_pScroll[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
						int mmin = 0 , mmax = 0;
						m_pScroll[MRL_VSCR_WEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

						if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
						{
							pos--;//한줄 감소 시키고 
							m_pScroll[MRL_VSCR_WEEK]->SetScrollPosWZ(pos);
						}
					}
				}
				break;

			case DLGPOS_MRL_VSCR_MEMBER:
				{
					if(m_pScroll[MRL_VSCR_MEMBER])
					{
						int pos = m_pScroll[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
						int mmin = 0 , mmax = 0;
						m_pScroll[MRL_VSCR_MEMBER]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

						if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
						{
							pos--;//한줄 감소 시키고 
							m_pScroll[MRL_VSCR_MEMBER]->SetScrollPosWZ(pos);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case DLGPOS_MRL_VSCR_WEEK:
				{
					if(m_pScroll[MRL_VSCR_WEEK])
					{
						int pos = m_pScroll[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
						int mmin = 0 , mmax = 0;
						m_pScroll[MRL_VSCR_WEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

						if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
						{
							pos++;//한줄 증가 시키고 
							m_pScroll[MRL_VSCR_WEEK]->SetScrollPosWZ(pos);
						}
					}
				}
				break;

			case DLGPOS_MRL_VSCR_PREWEEK:
				{
					if(m_pScroll[MRL_VSCR_PREWEEK])
					{
						int pos = m_pScroll[MRL_VSCR_PREWEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
						int mmin = 0 , mmax = 0;
						m_pScroll[MRL_VSCR_PREWEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

						if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
						{
							pos++;//한줄 증가 시키고 
							m_pScroll[MRL_VSCR_PREWEEK]->SetScrollPosWZ(pos);
						}
					}
				}
				break;

			case DLGPOS_MRL_VSCR_MEMBER:
				{
					if(m_pScroll[MRL_VSCR_MEMBER])
					{
						int pos = m_pScroll[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
						int mmin = 0 , mmax = 0;
						m_pScroll[MRL_VSCR_MEMBER]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

						if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
						{
							pos++;//한줄 증가 시키고 
							m_pScroll[MRL_VSCR_MEMBER]->SetScrollPosWZ(pos);
						}
					}
				}
				break;

			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		ShowInterface(FALSE);
		return;
	}

    if (!_isEnable())
        return;

	int iCurScrollPos = 0;
	int iStartDrawIndex = 0;

	if( GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK )
	{
		if( m_pWeekCheckButton->GetCheckState() )
		{
			if( m_pListControl[MRL_LST_WEEK] && m_pScroll[MRL_VSCR_WEEK] )
			{
				iCurScrollPos = m_pScroll[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
				iStartDrawIndex = iCurScrollPos;
				_UpdateMouseWheel(MRL_LST_WEEK);
				m_pListControl[MRL_LST_WEEK]->SetBeginDrawIndex( iStartDrawIndex );
			}

		}
		else
		{
			if(m_pListControl[MRL_LST_PREWEEK] && m_pScroll[MRL_VSCR_WEEK])
			{
				iCurScrollPos = m_pScroll[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
				iStartDrawIndex = iCurScrollPos;
				_UpdateMouseWheel(MRL_LST_PREWEEK);
				m_pListControl[MRL_LST_PREWEEK]->SetBeginDrawIndex( iStartDrawIndex );
			}
		}
	}
	else
	{
		if(m_pListControl[MRL_LST_WEEK] && m_pScroll[MRL_VSCR_WEEK])
		{
			iCurScrollPos = m_pScroll[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
			iStartDrawIndex = iCurScrollPos;
			_UpdateMouseWheel(MRL_LST_WEEK);
			m_pListControl[MRL_LST_WEEK]->SetBeginDrawIndex( iStartDrawIndex );
		}

		// 지난주 주간 순위
		if(m_pListControl[MRL_LST_PREWEEK] && m_pScroll[MRL_VSCR_PREWEEK])
		{
			iCurScrollPos = m_pScroll[MRL_VSCR_PREWEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
			iStartDrawIndex = iCurScrollPos;
			_UpdateMouseWheel(MRL_LST_PREWEEK);
			m_pListControl[MRL_LST_PREWEEK]->SetBeginDrawIndex( iStartDrawIndex );
		}
	}

	// 구성원 정보
	if(m_pListControl[MRL_LST_MEMBER] && m_pScroll[MRL_VSCR_MEMBER])
	{
		iCurScrollPos = m_pScroll[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
		iStartDrawIndex = iCurScrollPos;
		_UpdateMouseWheel(MRL_LST_MEMBER);
		m_pListControl[MRL_LST_MEMBER]->SetBeginDrawIndex( iStartDrawIndex );
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::SetAreaList()
{
	if (!GetManager())
        return;

	if(m_pListControl[MRL_LST_AREA])
	{
		m_pListControl[MRL_LST_AREA]->DeleteItemAll();
		m_pListControl[MRL_LST_AREA]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_AREA]->SetUseReport(false);

		int index = -1; 
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

		if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) //미션랭킹
		{
			MISSION_GROUP_ITR itr = GetManager()->m_MapMissionGroup.begin();
			for ( ; itr !=  GetManager()->m_MapMissionGroup.end(); ++itr)
			{
				MapGroup * pMapGroup = (MapGroup *)itr->second;		

				if ( !pMapGroup )
				{
					continue;
				}

				sMAPINFO * pMapInfo = GlobalFunc::GetMainMapFromGroup( (MAPCODE)itr->first );
				if (pMapInfo)
				{	
					index = m_pListControl[MRL_LST_AREA]->InsertItemBack();
					ZeroMemory(szTemp, sizeof(szTemp));

					g_InterfaceManager.GetInterfaceString( 
						pMapInfo->dwANCode, 
						szTemp, 
						INTERFACE_STRING_LENGTH );

					GlobalFunc::SetItem(
						m_pListControl[MRL_LST_AREA], 
						index, 0, szTemp, 
						c_iListFont,
#ifdef _INTERNATIONAL_UI
						DT_BOTTOM | DT_CENTER, 
#else
						DT_VCENTER | DT_CENTER, 
#endif//_INTERNATIONAL_UI
						c_ListBKColor, 
						c_ListTextColor, 
						c_ListSelTextColor);


					m_pListControl[MRL_LST_AREA]->SetItemData(index, (VOID*)(pMapGroup));
				}
			}
		}
		else if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_POINT) //포인트랭킹
		{
			std::vector<stEventMapInfo>	vecCategory = GetManager()->GetEventMapCategory();
			for(unsigned int i=0; i<vecCategory.size(); ++i)
			{
				index = m_pListControl[MRL_LST_AREA]->InsertItemBack();
				ZeroMemory(szTemp, sizeof(szTemp));

				g_InterfaceManager.GetInterfaceString( 
					vecCategory[i].dwCategory, 
					szTemp, 
					INTERFACE_STRING_LENGTH );

				GlobalFunc::SetItem(
					m_pListControl[MRL_LST_AREA], 
					index, 0, szTemp, 
					c_iListFont,
					DT_VCENTER | DT_CENTER, 
					c_ListBKColor, 
					c_ListTextColor, 
					c_ListSelTextColor);


				m_pListControl[MRL_LST_AREA]->SetItemData(index, NULL);				
			}
		}

		m_iCurMainListIndex = 0;
		m_iCurEventMainListIndex = 0;


		SetMissionList();
	}
}

//------------------------------------------------------------------------------
/** 마우스 휠처리
*/
void uiMissionRankList::_UpdateMouseWheel(eMRL_LST eListType)
{
	RECT rc = m_pListControl[eListType]->GetSizeRect();

	eMRL_VSCR eScrollType=MRL_VSCR_WEEK;

	switch(eListType)
	{
	case MRL_LST_PREWEEK:eScrollType=MRL_VSCR_PREWEEK;	break;
	case MRL_LST_MEMBER: eScrollType=MRL_VSCR_MEMBER;	break;
	}
	
	switch (GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP:  _DecreaseScroll(eScrollType);	break;
		
	case MOUSE_WHEEL_DOWN:_IncreaseScroll(eScrollType);	break;
	}
}
//------------------------------------------------------------------------------
/** 
*/
void uiMissionRankList::_IncreaseScroll(eMRL_VSCR eScrollType)
{
	if(m_pScroll[eScrollType])
	{
		int pos = m_pScroll[eScrollType]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
		int mmin = 0 , mmax = 0;
		m_pScroll[eScrollType]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

		if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
		{
			pos++;//한줄 증가 시키고 
			m_pScroll[eScrollType]->SetScrollPosWZ(pos);
		}
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::_DecreaseScroll(eMRL_VSCR eScrollType)
{
	if(m_pScroll[eScrollType])
	{
		int pos = m_pScroll[eScrollType]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
		int mmin = 0 , mmax = 0;
		m_pScroll[eScrollType]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

		if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
		{
			pos--;//한줄 감소 시키고 
			m_pScroll[eScrollType]->SetScrollPosWZ(pos);
		}
	}
	
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::SetMissionList()
{
	if (!GetManager())
        return;

	// 선택된 지역에 해당하는 미션 리스트를 출력한다. 메인/서브 모두

	if(m_pListControl[MRL_LST_MISSION])
	{
		m_pListControl[MRL_LST_MISSION]->DeleteItemAll();
		m_pListControl[MRL_LST_MISSION]->SetColorSelBar(c_ListSelBarColor);
		m_pListControl[MRL_LST_MISSION]->SetUseReport(false);

		int index = -1; 

		if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) //미션랭킹
		{
			if(m_iCurMainListIndex < 0)
				return;

			MapGroup * pMapGroup = (MapGroup *)m_pListControl[MRL_LST_AREA]->GetItemData(m_iCurMainListIndex);


			if (pMapGroup)
			{
				sMAPINFO_HASH & mapHash = pMapGroup->GetMapInfoHash();
				sMAPINFO_HASH::const_iterator itr = mapHash.begin();
				sMAPINFO * pMapInfo = NULL;

				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

				std::vector< sMAPINFO* > vMapGroupSort;
			
				while ( itr != mapHash.end())
				{
					pMapInfo =  itr->second;
					itr++;

					//GS_BUG_FIX: first 벡터에 미션정보 삽입
					if(pMapInfo)
						vMapGroupSort.push_back(pMapInfo);
				}
			
				static MapListSort_Funter sFunter;
				//GS_BUG_FIX: second 함수객체를 이용해서 Mtype내림차순으로 정렬
				if(!vMapGroupSort.empty())
					sort(vMapGroupSort.begin(),vMapGroupSort.end(),sFunter);
		
				for(unsigned int i=0;i<vMapGroupSort.size();++i)
				{
					//GS_BUG_FIX: third 정렬된정보에따라 삽입
					const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(vMapGroupSort[i]->MapCode);
					 
					if(mapInfo)	
					{
						if(GetManager()->IsMissionRanking(mapInfo->MapCode, eRANKING_TYPE_TIME_ATTACK))
						{
							index = m_pListControl[MRL_LST_MISSION]->InsertItemBack();
							ZeroMemory(szTemp, sizeof(szTemp));
							g_InterfaceManager.GetInterfaceString( 
								mapInfo->dwNCode, 
								szTemp, 
								INTERFACE_STRING_LENGTH );

							GlobalFunc::SetItem(
								m_pListControl[MRL_LST_MISSION], 
								index, 0, szTemp, 
								c_iListFont,
#ifdef _INTERNATIONAL_UI
								DT_BOTTOM | DT_CENTER, 
#else
								DT_VCENTER | DT_CENTER, 
#endif//_INTERNATIONAL_UI
								c_ListBKColor, 
								c_ListTextColor, 
								c_ListSelTextColor);

							m_pListControl[MRL_LST_MISSION]->SetItemData(index,(void*)mapInfo);
						}
					}
				}
			}
		}
		else if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_POINT) //포인트랭킹
		{
			if(m_iCurEventMainListIndex < 0)
				return;

			// 이벤트 맵도 랭킹정보를 볼 수 있어야 함
			std::vector<stEventMapInfo>	vecCategory = GetManager()->GetEventMapCategory();
			for(unsigned int i=0; i<vecCategory.size(); ++i)
			{
				if(m_iCurEventMainListIndex == vecCategory[i].iGroup)
				{
					TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					EVENTMAP_MULTIMAP	mapEventMap = GetManager()->GetEventMap();
					EVENTMAP_ITER it;
					std::pair<EVENTMAP_ITER, EVENTMAP_ITER> range;
					range = mapEventMap.equal_range(vecCategory[i].dwCategory);
					for(it=range.first; it!=range.second; ++it)
					{
						sMAPINFO* pMap = (*it).second;
						if(pMap)
						{
							// MissionRankingLVArea.txt 스크립트에 없으면 미션랭킹 대상이 아니다
							if(GetManager()->IsMissionRanking(pMap->MapCode, eRANKING_TYPE_POINT))
							{
								index = m_pListControl[MRL_LST_MISSION]->InsertItemBack();
								ZeroMemory(szTemp, sizeof(szTemp));
								g_InterfaceManager.GetInterfaceString( 
									pMap->dwNCode, 
									szTemp, 
									INTERFACE_STRING_LENGTH );

								GlobalFunc::SetItem(
									m_pListControl[MRL_LST_MISSION], 
									index, 0, szTemp, 
									c_iListFont,
#ifdef _INTERNATIONAL_UI
									DT_BOTTOM | DT_CENTER, 
#else
									DT_VCENTER | DT_CENTER, 
#endif//_INTERNATIONAL_U
									c_ListBKColor, 
									c_ListTextColor, 
									c_ListSelTextColor);


								m_pListControl[MRL_LST_MISSION]->SetItemData(index, (VOID*)pMap);
							}
						}
					}
					break;
				}
			}
		}

		m_iCurSubListIndex = 0;
		m_iCurEventSubListIndex = 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdateMissionText(bool bPrepare)
{
	if (!_isEnable())
        return;

	int iCurSubIndex = 0;

	if(m_pTxtControl[MRL_TXT_AREA])
	{
		if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) //미션랭킹
		{
			iCurSubIndex = m_iCurSubListIndex;

			MapGroup * pMapGroup = (MapGroup *)m_pListControl[MRL_LST_AREA]->GetItemData(m_iCurMainListIndex);

			if (pMapGroup)
			{
				sMAPINFO * pMapInfo = NULL;
				size_t	MapGroupCnt = pMapGroup->GetMapInfoHash().size();
				
				if ( 0 == MapGroupCnt )
				{
					return;
				}

				for ( size_t i = 0; i < MAX_SUB_MISSION_COUNT; ++i )
				{
					pMapInfo = pMapGroup->GetMapInfoIndex(i);
					if ( pMapInfo )
						break;
				}

				if (!pMapInfo)
				{
					// 메인미션 확인.		
					pMapInfo = pMapGroup->GetMapInfoIndex(0);
				}

				if ( pMapInfo )
				{
					TCHAR szText[INTERFACE_STRING_LENGTH] ={0,};

					g_InterfaceManager.GetInterfaceString(
						pMapInfo->dwANCode, 
						szText, 
						INTERFACE_STRING_LENGTH);

					GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_AREA], szText);
				}	
			}
		}
		else if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_POINT) //포인트랭킹
		{
			// 이벤트 맵도 랭킹정보를 볼 수 있어야 함
			iCurSubIndex = m_iCurEventSubListIndex;

			GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_AREA], _T("") );

			std::vector<stEventMapInfo>		vecEventMapCategory = GetManager()->GetEventMapCategory();
			
			for(unsigned int i=0; i < vecEventMapCategory.size(); ++i)
			{
				if(vecEventMapCategory[i].iGroup == m_iCurEventMainListIndex)
				{
					DWORD dwAKCode = 0;
					dwAKCode = vecEventMapCategory[i].dwCategory;

					TCHAR szText[INTERFACE_STRING_LENGTH] ={0,};

					g_InterfaceManager.GetInterfaceString(
						dwAKCode, 
						szText, 
						INTERFACE_STRING_LENGTH);

					GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_AREA], szText);

					break;
				}
			}
		}
		
	}

	if(m_pTxtControl[MRL_TXT_MISSION])
	{
		const sMAPINFO * pMapInfo = (sMAPINFO *)m_pListControl[MRL_LST_MISSION]->GetItemData(iCurSubIndex);
		
		if ( pMapInfo )
		{
			m_MissionNo = pMapInfo->MapCode;	
			TCHAR szText[INTERFACE_STRING_LENGTH] ={0,};
			g_InterfaceManager.GetInterfaceString(
				pMapInfo->dwNCode, 
				szText, 
				INTERFACE_STRING_LENGTH);

			GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_MISSION], szText);

			GetManager()->UpdateReceiveData(m_MissionNo, 0);
		}	
		else
		{
			GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_MISSION], _T("") );
		}

	}

	UpdatePageText();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdateRankList()
{
	if (!_isEnable())
        return;

	_initRankList();

	bool bPointRanking = false;
	if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_POINT)				// 포인트 랭킹
	{
		bPointRanking = true;
	}
	else if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK)	// 미션랭킹
	{	
	}
	else
	{
		return;
	}

	// 명예의 전당
	if(m_pListControl[MRL_LST_FAME])
	{
		int index = 0;
		int iCnt = 0;

		if(GetManager()->m_FameData.iRank > 0)	// 데이터가 있으면 리스트에 추가
		{
			index = m_pListControl[MRL_LST_FAME]->InsertItemBack();
			m_pListControl[MRL_LST_FAME]->SetItemData(index, &(GetManager()->m_FameData));

			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

			DWORD dwStyle = 0;
			for(iCnt=0; iCnt<eFAME_LST_MAX; iCnt++)
			{
				ZeroMemory(szTemp, sizeof(szTemp));
				if(eFAME_LST_PARTY_NUM == iCnt)	// 파티번호 or 캐릭명 
				{
					if(bPointRanking)
					{
						//Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_FameData.strName.c_str());
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_FameData.strName);
					}
					else
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_FameData.iPartyNum);
					}

					dwStyle = DT_VCENTER | DT_CENTER;

				}
				else if(eFAME_LST_LEVEL == iCnt)	// 레벨
				{
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), GetManager()->m_FameData.iLevel);
					dwStyle = DT_VCENTER | DT_CENTER;
				}
				else if(eFAME_LST_TIME == iCnt)	// 진행시간 or 포인트
				{
					if(bPointRanking)
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_FameData.dwTime);
					}
					else
					{
						util::_tstring strTime;
						GetManager()->GetTimeString(GetManager()->m_FameData.dwTime, strTime);
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
					}

					dwStyle = DT_VCENTER | DT_CENTER;
				}

				GlobalFunc::SetItem(m_pListControl[MRL_LST_FAME], index, iCnt,
								szTemp,
								StrToWzID("m212"),
								dwStyle,
								c_ListBKColor, 
								c_ListTextColor, 
								c_ListSelTextColor);
				
			}
		}
	}

	// 이번주 주간 순위
	if(m_pListControl[MRL_LST_WEEK])
	{
		int index = 0;
		int iCnt = 0;

		DWORD dwStyle = 0;

		for(int i=0; i<MAX_WEEK_RANK; ++i)
		{
			if(GetManager()->m_CurWeekRank[i].iRank > 0)	// 데이터가 있으면 리스트에 추가
			{
				index = m_pListControl[MRL_LST_WEEK]->InsertItemBack();
				m_pListControl[MRL_LST_WEEK]->SetItemData(index, &(GetManager()->m_CurWeekRank[i]));
				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

				for(iCnt=0; iCnt<eWEEK_LST_MAX; iCnt++)
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					if(eWEEK_LST_RANK == iCnt)				// 순위
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-4d"), GetManager()->m_CurWeekRank[i].iRank);
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_PARTY_NUM == iCnt)	// 파티번호 or 캐릭명 
					{
						if(bPointRanking)
						{
							//Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_CurWeekRank[i].strName.c_str());
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_CurWeekRank[i].strName);
						}
						else
						{
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_CurWeekRank[i].iPartyNum);
						}
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_LEVEL == iCnt)		// 레벨
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), GetManager()->m_CurWeekRank[i].iLevel);
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_TIME == iCnt)		// 진행시간 or 포인트
					{
						if(bPointRanking)
						{
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_CurWeekRank[i].dwTime);
						}
						else
						{
							util::_tstring strTime;
							GetManager()->GetTimeString(GetManager()->m_CurWeekRank[i].dwTime, strTime);
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
						}
						dwStyle = DT_VCENTER | DT_CENTER;
					}

					GlobalFunc::SetItem(m_pListControl[MRL_LST_WEEK], index, iCnt,
									szTemp,
									StrToWzID("m212"),
									dwStyle,
									c_ListBKColor, 
									c_ListTextColor, 
									c_ListSelTextColor);
				}
			}
		}


		int iPage = 0;

		if(index)
		{
			if(index >= c_iDefaultRankLine)
				iPage = (index+1)-c_iDefaultRankLine;
		}

		if(m_pScroll[MRL_VSCR_WEEK])
        {
			m_pScroll[MRL_VSCR_WEEK]->SetScrollRangeWZ(0,iPage);	//스크롤 범위
        }
	}

	// 지난주 주간 순위
	if(m_pListControl[MRL_LST_PREWEEK])
	{
		int index = 0;
		int iCnt = 0;

		DWORD dwStyle = 0;

		for(int i=0; i<MAX_PREWEEK_RANK; ++i)
		{
			if(GetManager()->m_PreWeekRank[i].iRank > 0)	// 데이터가 있으면 리스트에 추가
			{
				index = m_pListControl[MRL_LST_PREWEEK]->InsertItemBack();
				m_pListControl[MRL_LST_PREWEEK]->SetItemData(index, &(GetManager()->m_PreWeekRank[i]));
				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

				for(iCnt=0; iCnt<eWEEK_LST_MAX; iCnt++)
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					if(eWEEK_LST_RANK == iCnt)				// 순위
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-4d"), GetManager()->m_PreWeekRank[i].iRank);
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_PARTY_NUM == iCnt)	// 파티번호 or 캐릭명 
					{
						if(bPointRanking)
						{
							//Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_PreWeekRank[i].strName.c_str());
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), GetManager()->m_PreWeekRank[i].strName);
						}
						else
						{
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_PreWeekRank[i].iPartyNum);
						}
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_LEVEL == iCnt)		// 레벨
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), GetManager()->m_PreWeekRank[i].iLevel);
						dwStyle = DT_VCENTER | DT_CENTER;
					}
					else if(eWEEK_LST_TIME == iCnt)		// 진행시간 or 포인트
					{
						if(bPointRanking)
						{
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), GetManager()->m_PreWeekRank[i].dwTime);
						}
						else
						{
							util::_tstring strTime;
							GetManager()->GetTimeString(GetManager()->m_PreWeekRank[i].dwTime, strTime);
							Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
						}
						dwStyle = DT_VCENTER | DT_CENTER;
					}

					GlobalFunc::SetItem(m_pListControl[MRL_LST_PREWEEK], index, iCnt,
									szTemp,
									StrToWzID("m212"),
									dwStyle,
									c_ListBKColor, 
									c_ListTextColor, 
									c_ListSelTextColor);

				}
			}
		}

		int iPage = 0;

		if(index)
		{
			if(index >= c_iDefaultRankLine)
            {
				iPage = (index+1)-c_iDefaultRankLine;
            }
		}


		if(m_pScroll[MRL_VSCR_PREWEEK])
        {
			m_pScroll[MRL_VSCR_PREWEEK]->SetScrollRangeWZ(0,iPage);	//스크롤 범위
        }
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdateMissionSelectList()
{
	if (!_isEnable())
        return;

	if(m_pListControl[MRL_LST_AREA])
	{
		m_pListControl[MRL_LST_AREA]->SetCurSel(-1);
		if(m_bShowMainList)
		{
			m_pListControl[MRL_LST_AREA]->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			m_pListControl[MRL_LST_AREA]->ShowWindowWZ(WZ_HIDE);
		}
	}

	if(m_pListControl[MRL_LST_MISSION])
	{
		m_pListControl[MRL_LST_MISSION]->SetCurSel(-1);
		if(m_bShowSubList)
		{
			m_pListControl[MRL_LST_MISSION]->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			m_pListControl[MRL_LST_MISSION]->ShowWindowWZ(WZ_HIDE);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdatePageText()
{
	if (!_isEnable())
        return;

	if(m_pTxtControl[MRL_TXT_PAGE])
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		int iPage1 = (GetManager()->m_CurRankPage * MAX_WEEK_RANK) + 1;
		int iPage2 = (GetManager()->m_CurRankPage * MAX_WEEK_RANK) + MAX_WEEK_RANK;
#ifdef _JAPAN
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d - %d"), iPage1, iPage2);
#else
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d ~ %d"), iPage1, iPage2);
#endif

		GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_PAGE], szMessage);
	}

	
}



//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdateMemberList()
{
	if (!_isEnable())
        return;

	// 구성원 정보
	if(m_pListControl[MRL_LST_MEMBER])
	{
		int index = 0;
		int iCnt = 0;

		_initMemberList();

		CControlWZ * pGuildLevel = getControl( DLGPOS_MRL_STATIC_GUILD_LEVEL );
		CControlWZ * pGuild = getControl( DLGPOS_MRL_STATIC_GUILD );
		CControlWZ * pName = getControl( DLGPOS_MRL_STATIC_NAME );

		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_MEMBER], 0, pGuildLevel->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_MEMBER], 1, pGuild->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pListControl[MRL_LST_MEMBER], 2, pName->GetSize().right, NULL );

		for(int i=0; i<MAX_RANK_MEMBER; ++i)
		{
			if(GetManager()->m_MemberData[i].iLevel > 0)
			{
				index = m_pListControl[MRL_LST_MEMBER]->InsertItemBack();
				m_pListControl[MRL_LST_MEMBER]->SetItemData(index, &(GetManager()->m_MemberData[i]));
				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

				for(iCnt=0; iCnt<eMEMBER_LST_MAX; iCnt++)
				{
					ZeroMemory(szTemp, sizeof(szTemp));

					if(eMEMBER_LST_LEVEL == iCnt)			// 레벨
					{
						Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%d"), GetManager()->m_MemberData[i].iLevel);
					}
					else if(eMEMBER_LST_GUILD == iCnt)		// 길드
					{
						//Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strGuild.c_str());
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strGuild);
					}
					else if(eMEMBER_LST_NAME == iCnt)		// 이름
					{
						//Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strName.c_str());
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strName);
					}

					GlobalFunc::SetItem(m_pListControl[MRL_LST_MEMBER], index, iCnt,
									szTemp,
									StrToWzID("m212"),
									DT_VCENTER | DT_CENTER,
									c_ListBKColor, 
									c_ListTextColor, 
									c_ListSelTextColor);

				}
			}
		}

		m_pListControl[MRL_LST_MEMBER]->SetCurSel(-1);

		int iPage = 0;

		if(index)
		{
			if(index >=c_iDefaultMemberLine)
            {
				iPage = (index+1)-c_iDefaultMemberLine;
            }
		}
		

		if(m_pScroll[MRL_VSCR_MEMBER])
		{
			m_pScroll[MRL_VSCR_MEMBER]->SetScrollRangeWZ(0,iPage);	//스크롤 범위
			m_pScroll[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList::UpdateFinishDay()
{
	if (!_isEnable())
        return;

	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	//9952	결산일: %s
	g_InterfaceManager.GetInterfaceString( 9952, szTemp, INTERFACE_STRING_LENGTH );
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_strFinishDay.c_str());
	GlobalFunc::SetCaption( m_pTxtControl[MRL_TXT_DATE], szMessage );
}


//------------------------------------------------------------------------------
/**
*/

void uiMissionRankList::_setTitle()
{
	if(GetManager())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

		if(GetManager()->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) // 미션랭킹
		{
			// 미션랭킹
			g_InterfaceManager.GetInterfaceString( 5492, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::SetCaption( m_pTxtTitle, szMessage );
			
			//파티번호
			g_InterfaceManager.GetInterfaceString( 9956, szMessage, INTERFACE_STRING_LENGTH );
			int i;
			for(i=0; i <= MRL_LST_BTN_NAME3; ++i)
			{
				if(m_pLstBtns[i])
				{
					GlobalFunc::SetCaption( m_pLstBtns[i], szMessage );
				}
			}

			//진행시간
			g_InterfaceManager.GetInterfaceString( 9957, szMessage, INTERFACE_STRING_LENGTH );
			for(i=MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i)
			{
				if(m_pLstBtns[i])
				{
					GlobalFunc::SetCaption( m_pLstBtns[i], szMessage );
				}
			}

		}
		else	// 포인트 랭킹
		{
			// 포인트 랭킹
			g_InterfaceManager.GetInterfaceString( 1465, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::SetCaption( m_pTxtTitle, szMessage );
			
			//캐릭터명
			g_InterfaceManager.GetInterfaceString( 9702, szMessage, INTERFACE_STRING_LENGTH );
			int i;
			for(i=0; i <= MRL_LST_BTN_NAME3; ++i)
			{
				if(m_pLstBtns[i])
				{
					GlobalFunc::SetCaption( m_pLstBtns[i], szMessage );
				}
			}

			//포인트점수
			g_InterfaceManager.GetInterfaceString( 1461, szMessage, INTERFACE_STRING_LENGTH );
			for(i=MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i)
			{
				if(m_pLstBtns[i])
				{
					GlobalFunc::SetCaption( m_pLstBtns[i], szMessage );
				}
			}
		}

	}
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
