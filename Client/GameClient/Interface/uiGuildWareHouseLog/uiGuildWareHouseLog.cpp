#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildWareHouseLog/uiGuildWareHouseLog.h"
#include "InterfaceManager.h"
#include "KeyQueueManager.h"
#include "GlobalFunc.h"
#include "GameFunc.h"
#include "ItemInfoParser.h"
#include "LimitedItemInfoParser.h"

#ifdef _INTERNATIONAL_DATE_TOOLTIP
#include "uiToolTipMan/uiToolTipMan.h"
#endif//_INTERNATIONAL_DATE_TOOLTIP

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiGuildWareHouseLog::m_wzID[eDIALOG_GUILD_WAREHOUSE_LOG_MAX] = 
{
		StrToWzID("B100"), //eDIALOG_GUILD_WAREHOUSE_LOG_EXIT_BTN,
		StrToWzID("B101"), //eDIALOG_GUILD_WAREHOUSE_LOG_OK_BTN,
		StrToWzID("L100"), //eDIALOG_GUILD_WAREHOUSE_LOG_MAIN_LIST,
		StrToWzID("T101"), //eDIALOG_GUILD_WAREHOUSE_LOG_ID_TEXT,   
		StrToWzID("T102"), //eDIALOG_GUILD_WAREHOUSE_LOG_USED_TEXT, 
		StrToWzID("T103"), //eDIALOG_GUILD_WAREHOUSE_LOG_TIME_TEXT, 
		StrToWzID("ST00"), //eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TEXT,
		StrToWzID("BT00"), //eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_RIGHT_BTN,
		StrToWzID("BT01"), //eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_LEFT_BTN,
		StrToWzID("CT00"), //eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_KEEP_CHECK_BTN,
		StrToWzID("CT01"), //eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TAKE_CHECK_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildWareHouseLog::uiGuildWareHouseLog(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_iCurInPage	  = 1;
	m_iCurOutPage     = 1;
	m_btCurState	  = eSTATE_IN;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildWareHouseLog::~uiGuildWareHouseLog()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::Release()
{
	m_listLogListIn.clear();
	m_listLogListOut.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;

		m_iCurInPage	  = 1;
		m_iCurOutPage     = 1;
		RefreshCurTab(eSTATE_IN);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_WareHouse_Log;
		msg.wParam = InterfaceManager::DIALOG_GUILD_WAREHOUSE_LOG;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse_Log);
		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_GUILD_WAREHOUSE_LOG_OK_BTN:
			case eDIALOG_GUILD_WAREHOUSE_LOG_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_MAIN_LIST:
				{
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_ID_TEXT:
				{
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_USED_TEXT:
				{
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_TIME_TEXT:
				{
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TEXT:
				{
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_RIGHT_BTN:
				{
					if(eSTATE_IN == m_btCurState)
					{
						int iCnt1 = m_iCurInPage * MAX_GUILDWAREHOUSE_LOGPAGE_NUM;
						int iCnt2 = m_listLogListIn.size();
						if(iCnt1 < iCnt2) 
							m_iCurInPage++;

						RefreshCurTab(eSTATE_IN);
					}
					else if(eSTATE_OUT == m_btCurState)
					{
						int iCnt1 = m_iCurOutPage * MAX_GUILDWAREHOUSE_LOGPAGE_NUM;
						int iCnt2 = m_listLogListOut.size();
						if(iCnt1 < iCnt2) 
							m_iCurOutPage++;

						RefreshCurTab(eSTATE_OUT);
					}
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_LEFT_BTN:
				{
					if(eSTATE_IN == m_btCurState)
					{
						if(1 >= m_iCurInPage)
							m_iCurInPage = 1;
						else
							m_iCurInPage--;

						RefreshCurTab(eSTATE_IN);
					}
					else if(eSTATE_OUT == m_btCurState)
					{
						if(1 >= m_iCurOutPage)
							m_iCurOutPage = 1;
						else
							m_iCurOutPage--;

						RefreshCurTab(eSTATE_OUT);
					}
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_KEEP_CHECK_BTN:
				{
					RefreshCurTab(eSTATE_IN);
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TAKE_CHECK_BTN:
				{
					RefreshCurTab(eSTATE_OUT);
				}
				break;
			}
		}
		break;

	default:
		{
		}
		break;

	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::RefreshCurTab(BYTE btState)
{
#ifdef _INTERNATIONAL_UI
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
#else
	TCHAR szTime[32 + 1] = {0,};
#endif//_INTERNATIONAL_UI

	TCHAR szMoney[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szMoneyFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szMoneyRealFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

	m_btCurState = btState;
	m_pctrlList->DeleteItemAll();
	if(eSTATE_IN != m_btCurState && eSTATE_OUT != m_btCurState)
	{
		assert(!"SetCurTab 확인해");
		return;
	}

	std::list<STRUCT_GUILDWAREHOUSE_LOGINFO>::iterator iterBegin;
	std::list<STRUCT_GUILDWAREHOUSE_LOGINFO>::iterator iterEnd;
	int iPageMaxCnt = 0;

	if(eSTATE_IN == m_btCurState)
	{
		m_pctrlInCheckBtn->SetCheckState(TRUE);
		m_pctrlOutCheckBtn->SetCheckState(FALSE);

		iterBegin    = m_listLogListIn.begin();
		iterEnd      = m_listLogListIn.end();
		iPageMaxCnt = m_iCurInPage * MAX_GUILDWAREHOUSE_LOGPAGE_NUM;
		TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
		m_pctrlPageText->SetTextWZ(_itoa(m_iCurInPage, szTemp, 10));
	}
	else
	{
		m_pctrlInCheckBtn->SetCheckState(FALSE);
		m_pctrlOutCheckBtn->SetCheckState(TRUE);

		iterBegin    = m_listLogListOut.begin();
		iterEnd		 = m_listLogListOut.end();
		iPageMaxCnt = m_iCurOutPage * MAX_GUILDWAREHOUSE_LOGPAGE_NUM;
		TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
		m_pctrlPageText->SetTextWZ(_itoa(m_iCurOutPage, szTemp, 10));
	}

	int iCurCnt = 0;
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if(iPageMaxCnt > iCurCnt && (iPageMaxCnt - MAX_GUILDWAREHOUSE_LOGPAGE_NUM) <= iCurCnt)
		{
			STRUCT_GUILDWAREHOUSE_LOGINFO sLogInfo = (*iterBegin);
#ifdef _INTERNATIONAL_UI
			DWORD dwListFont = StrToWzID("st10");
#else
			DWORD dwListFont = StrToWzID("mn12");
#endif//
			int index = m_pctrlList->InsertItemBack();
			GlobalFunc::SetItem(m_pctrlList, index, eLISTCNT_ID, sLogInfo.m_UsedCharName, dwListFont, 
				DT_CENTER | DT_VCENTER,
				RGBA(0,0,0,0), 
				RGBA(255,255,255,255), 
				RGBA(235,192,38,255));

			if(0 != sLogInfo.m_ItemCode) //아이템
			{
				BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(sLogInfo.m_ItemCode);
				if(pItemInfo)
				{
					TCHAR szItemNameFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szItemName[INTERFACE_STRING_LENGTH + 1] = {0,};

                    GameFunc::GetItemName(NULL, pItemInfo, szItemName, INTERFACE_STRING_LENGTH + 1);

					if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
					{
						if(0 == sLogInfo.m_LimiteCode)
						{
							Snprintf(szItemNameFinal, INTERFACE_STRING_LENGTH, _T("%s"), szItemName);
						}
						else
						{
							//11283	리미티드
							TCHAR szLimited[INTERFACE_STRING_LENGTH + 1] = {0,};
							g_InterfaceManager.GetInterfaceString(11283, szLimited);
							Snprintf(szItemNameFinal, INTERFACE_STRING_LENGTH, _T("%s %s"), szLimited, szItemName);
						}
					}
					else
					{
						g_InterfaceManager.GetInterfaceString(pItemInfo->m_NCode, szItemName);
						if(0 == sLogInfo.m_ItemNum)
							Snprintf(szItemNameFinal, INTERFACE_STRING_LENGTH, _T("%s"), szItemName);
						else
							Snprintf(szItemNameFinal, INTERFACE_STRING_LENGTH, _T("%s X %d"), szItemName, sLogInfo.m_ItemNum);
					}

					GlobalFunc::SetItem(m_pctrlList, index, eLISTCNT_USED, szItemNameFinal, dwListFont, 
						DT_CENTER | DT_VCENTER,
						RGBA (0,0,0,0), 
						RGBA(235,192,38,255), 
						RGBA(235,192,38,255));
				}
			}
			else //하임
			{
				Snprintf(szMoney, INTERFACE_STRING_LENGTH, _T("%I64u"), sLogInfo.m_Money);
				NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
				::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));

				//999	하임
				TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
				g_InterfaceManager.GetInterfaceString(999, szTemp);
				Snprintf(szMoneyRealFinal, INTERFACE_STRING_LENGTH, _T("%s%s"), szMoneyFinal, szTemp);
				GlobalFunc::SetItem(m_pctrlList, index, eLISTCNT_USED, szMoneyRealFinal, dwListFont, 
					DT_CENTER | DT_VCENTER,
					RGBA(0,0,0,0), 
					RGBA(235,192,38,255), 
					RGBA(235,192,38,255));
			}

			TCHAR szTemp[12 + 1] = {0,};
			_i64toa(sLogInfo.m_UsedTime, szTemp, 10); 
#ifdef _INTERNATIONAL_DATE_TOOLTIP
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			TCHAR szMonth[INTERFACE_STRING_LENGTH] = {0,};
			if(pUIMan)
			{
				szMonth[0]=szTemp[4];
				szMonth[1]=szTemp[5];
				szMonth[2]='\0';
				//월을 영문스트링으로 교체
                int nStringIdx = pUIMan->GetStringMonth(atoi(szMonth));
                g_InterfaceManager.GetInterfaceString(nStringIdx, szMonth);
            }
    #ifdef _RUSSIA
            // 러시아 july.02.2009 (12:12).
            Snprintf(szTime,INTERFACE_STRING_LENGTH, _T("%s.%c%c.%c%c%c%c (%c%c:%c%c)."),szMonth	//월(july)
                ,szTemp[6], szTemp[7]		//1일
                , szTemp[0], szTemp[1], szTemp[2], szTemp[3] //2009
                , szTemp[8], szTemp[9], szTemp[10], szTemp[11]);//(00:00)
    #else
            // 글로벌 july/02 (12:12), 2009	
            Snprintf(szTime,INTERFACE_STRING_LENGTH, _T("%s %c%c (%c%c:%c%c), %c%c%c%c"),szMonth	//월(july)
                ,szTemp[6], szTemp[7]		//1일
                , szTemp[8], szTemp[9], szTemp[10], szTemp[11]//(00:00)
                , szTemp[0], szTemp[1], szTemp[2], szTemp[3]);//2009
    #endif //_RUSSIA

#else
			Snprintf(szTime, 32, _T("%c%c%c%c/%c%c/%c%c %c%c:%c%c"), szTemp[0], szTemp[1], szTemp[2], szTemp[3]
																	, szTemp[4], szTemp[5], szTemp[6], szTemp[7]
																	, szTemp[8], szTemp[9], szTemp[10], szTemp[11]);
#endif//_INTERNATIONAL_DATE_TOOLTIP
			GlobalFunc::SetItem(m_pctrlList, index, eLISTCNT_TIME, szTime, StrToWzID("st10"), 
				DT_CENTER | DT_VCENTER,
				RGBA(0,0,0,0), 
				RGBA(255,255,255,255), 
				RGBA(235,192,38,255));
		}

		iCurCnt++;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::ClearLogList()
{
	m_listLogListIn.clear();
	m_listLogListOut.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseLog::SetLogList(STRUCT_GUILDWAREHOUSE_LOGINFO sLogInfo)
{
	if(  (MONEY_IN == sLogInfo.m_LogType) || (ITEM_IN == sLogInfo.m_LogType) )
	{
		if( 100 <= m_listLogListIn.size()){
			m_listLogListIn.pop_front();
			m_listLogListIn.push_back(sLogInfo);
		}
		else if( 100 >  m_listLogListIn.size() && 0 <=  m_listLogListIn.size())
		{
			m_listLogListIn.push_back(sLogInfo);
		}
	}
	else if( (MONEY_OUT == sLogInfo.m_LogType) || (ITEM_OUT == sLogInfo.m_LogType))
	{

		if( 100 <= m_listLogListOut.size() ){
			m_listLogListOut.pop_front();
			m_listLogListOut.push_back(sLogInfo);
		}
		else if( 100 >  m_listLogListOut.size() && 0 <= m_listLogListOut.size() )
		{
			m_listLogListOut.push_back(sLogInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void	uiGuildWareHouseLog::InitControl()
{
	m_pctrlRightBtn = (CCtrlButtonWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_RIGHT_BTN);
	assert(m_pctrlRightBtn);
	m_pctrlLeftBtn = (CCtrlButtonWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_LEFT_BTN);
	assert(m_pctrlLeftBtn);

	m_pctrlInCheckBtn  = (CCtrlButtonCheckWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_KEEP_CHECK_BTN);
	assert(m_pctrlInCheckBtn);
	m_pctrlOutCheckBtn = (CCtrlButtonCheckWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TAKE_CHECK_BTN);
	assert(m_pctrlOutCheckBtn);

	m_pctrlList = (CCtrlListWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_MAIN_LIST);

	CCtrlStaticWZ * pIDStaitc = (CCtrlStaticWZ*)getControl( eDIALOG_GUILD_WAREHOUSE_LOG_ID_TEXT );
	CCtrlStaticWZ * pUsedStaitc = (CCtrlStaticWZ*)getControl( eDIALOG_GUILD_WAREHOUSE_LOG_USED_TEXT );
	CCtrlStaticWZ * pTimeStaitc = (CCtrlStaticWZ*)getControl( eDIALOG_GUILD_WAREHOUSE_LOG_TIME_TEXT );


	assert(m_pctrlList);
	if(m_pctrlList)
	{
		m_pctrlList->SetUnuseSelBar(TRUE);
		m_pctrlList->SetUseReport(TRUE);
		m_pctrlList->SetColorSelBar(RGBA(0,0,0,125));
		m_pctrlList->SetSellHeight(18);
		m_pctrlList->SetReportUseCnt(3);
		m_pctrlList->SetHideReportRender(TRUE);
#ifdef _INTERNATIONAL_UI
		GlobalFunc::SetReportButtonInfo( m_pctrlList, 0, pIDStaitc->GetSize().right+5, NULL );
		GlobalFunc::SetReportButtonInfo( m_pctrlList, 1, pUsedStaitc->GetSize().right+10, NULL );
#else
		GlobalFunc::SetReportButtonInfo( m_pctrlList, 0, pIDStaitc->GetSize().right, NULL );
		GlobalFunc::SetReportButtonInfo( m_pctrlList, 1, pUsedStaitc->GetSize().right, NULL );
#endif//_INTERNATIONAL_UI(하드코딩:디자이너요청)

		GlobalFunc::SetReportButtonInfo( m_pctrlList, 2, pTimeStaitc->GetSize().right, NULL );
	}

	m_pctrlPageText = (CCtrlStaticWZ*)getControl(eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TEXT);
	assert(m_pctrlPageText);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
