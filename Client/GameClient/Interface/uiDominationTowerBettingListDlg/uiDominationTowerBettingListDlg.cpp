#include "SunClientPrecompiledHeader.h"
#include "uiDominationTowerBettingListDlg.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "interfacemanager.h"
#include "globalfunc.h"
#include "Map.h"
#include "DominationInfoParser.h"

const int kListTextGap = 12;

//------------------------------------------------------------------------------
WzID uiDominationTowerBettingListDlg::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("LS00"),
	StrToWzID("BT00"),
	StrToWzID("BT04"),

	StrToWzID("B999"),

	StrToWzID("ST05"),
	StrToWzID("ST06"),
	StrToWzID("ST00"),
};

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerBettingListDlg::uiDominationTowerBettingListDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	m_pBettingList = NULL;
	m_pBettingButton = NULL;
	m_pCancelButton = NULL;
	m_pCloseButton = NULL;
	for (int i = 0; i < LIST_COL; ++i)
	{
		m_pTextControl[i] = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerBettingListDlg::~uiDominationTowerBettingListDlg()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerBettingListDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pBettingList = static_cast<CCtrlListWZ*>(GetControlWZ(m_wzId[DLGPOS_BETTING_LIST]));
	assert(m_pBettingList);	
	m_pBettingButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_BETTING_BUTTON]));
	assert(m_pBettingButton);
	m_pCancelButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CANCEL_BUTTON]));
	assert(m_pCancelButton);
	m_pCloseButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CLOSE_BUTTON]));
	assert(m_pCloseButton);	

	for (int index = 0; index < LIST_COL; ++index)
	{
		m_pTextControl[index] = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDNAME_TEXT+index]));
		assert(m_pTextControl[index]);	
	}

	// 리스트콘트롤초기화
	if(m_pBettingList)
	{
		m_pBettingList->DeleteItemAll();
		m_pBettingList->SetColorSelBar(0);
		m_pBettingList->SetUseReport(true);
		m_pBettingList->SetReportUseCnt(LIST_COL);
		m_pBettingList->SetHideReportRender(true);

		for (int index = 0; index < LIST_COL; ++index)
		{
			GlobalFunc::SetReportButtonInfo( m_pBettingList, index, m_pTextControl[index]->GetSize().right+((index+1)*kListTextGap), NULL );
		}		
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerBettingListDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		//MoveDefaultWindowPos();

		//키큐메시지삽입
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Domination_BettingList;
		msg.wParam=IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST_DIALOG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		//키큐메시지삭제
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_BettingList);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerBettingListDlg::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			CControlWZ * pControl = GetControlWZ(m_wzId[pos]);
			if (NULL == pControl)
			{
				return;
			}

			// 패킷 펜딩셋 상태면 리턴
			uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
			if (NULL == pUIMan || pUIMan->IsPKPending())
			{
				return;
			}			

			switch(pos)
			{
			case DLGPOS_BETTING_BUTTON:
				{
					// 베팅하기					
					pUIMan->SEND_DOMINATION_BATTING_HEIM_SYN();
				}
				break;
			case DLGPOS_CANCEL_BUTTON: 
			case DLGPOS_CLOSE_BUTTON: 
				{
					// 참여 취소 창닫기 
					ShowInterface(FALSE);					
				}
				break;			
			default:
				{

				}
			}
		}	
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerBettingListDlg::SetBettingListData(DOMINATION_ACTION_INFO* pListInfo, DWORD dwCount)
{
	// 리스트 세팅
	m_pBettingList->DeleteItemAll();
	TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
	MONEY OfferHeim = 0;

	DWORD ListIndex = 0;
#ifdef _INTERNATIONAL_UI
	DWORD TextFont = StrToWzID("st10");
#else
	DWORD TextFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
	DWORD TextColor;

	for (DWORD Index = 0; Index < dwCount; ++Index)
	{
		if (pListInfo[Index].m_GuilGuid)
		{
			ListIndex = m_pBettingList->InsertItemBack();
			if ( 0 == ListIndex )
			{
				TextColor = RGBA(76,240,4,255);
			}
			else
			{
				TextColor = RGBA(200,200,200,255);
			}

            // 입찰 순위
            Snprintf(szResult, INTERFACE_STRING_LENGTH, "%d", ListIndex + 1);
            GlobalFunc::SetItem(m_pBettingList, ListIndex, 0, szResult, 
                TextFont, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), TextColor, TextColor);

            // 길드명
            GlobalFunc::SetItem(m_pBettingList, ListIndex, 1, pListInfo[Index].m_tszGuildName, 
                TextFont, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), TextColor, TextColor);

            // 날짜
            GlobalFunc::SetItem(m_pBettingList, ListIndex, 2, pListInfo[Index].m_tszActionApplyTime, 
                TextFont, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), TextColor, TextColor);
		}
		else
		{
			break;
		}
	}
	
	// 지역점령전 필드별로 설정된 기본입찰금액과 비교하여 입찰가능 금액계산
	uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
	// 파서 변경시 수정 - 필요한 함수 추가
	MAPCODE MapCode = g_pMap->GetCurrentFieldID();
	const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(MapCode);
	OfferHeim++;
	if ( 0 != MapCode && NULL != pDI)
	{
		OfferHeim = max(OfferHeim, (pDI->m_BaseBattingMoney));
		pUIMan->SetMostOfferHeim(OfferHeim);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerBettingListDlg::SetBettingButton(BOOL bCheck)
{
    if (m_pBettingButton)
    {
        if (bCheck)
        {
            m_pBettingButton->EnableWindowWZ(TRUE);
        }
        else
        {
            m_pBettingButton->EnableWindowWZ(FALSE);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
#endif //_NA_006826_20130722_DOMINATION_RENEWAL