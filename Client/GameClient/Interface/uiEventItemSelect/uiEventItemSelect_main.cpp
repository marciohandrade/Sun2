#include "SunClientPrecompiledHeader.h"

#include "uiEventItemSelect.h"
#include "uiEventMan/uiEventMan.h"
#include "SunEventInfoParser.h"
#include "ItemInfoParser.h"
#include "Hero.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"

static CODETYPE g_dwEventID = 0;

void _Confirm_RequestRegItem(bool bYes)
{
	if(bYes)
	{
		uiEventMan* pEventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
		if(pEventMan)
		{
			pEventMan->RequestEventItemReg(g_dwEventID);
		}
	}
	else
	{
		g_dwEventID = 0;
	}
}

//------------------------------------------------------------------------------
WzID uiEventItemSelect::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	//DLGPOS_BTN_EXIT		// 종료버튼
		StrToWzID("ST00"),	//DLGPOS_TXT_ITEM		// 재료
		StrToWzID("ST01"),	//DLGPOS_TXT_COUNT		// 참가횟수
		StrToWzID("ST02"),	//DLGPOS_TXT_ITEM2		
		StrToWzID("B003"),	//DLGPOS_BTN_OK			// 확인
		StrToWzID("B004"),	//DLGPOS_BTN_CANCEL		// 취소
		StrToWzID("T999"),	//DLGPOS_TXT_TITLE		 // 제목
		StrToWzID("L001"),	//DLGPOS_LIST			//리스트
        StrToWzID("VS00"),  //DLGPOS_VSCROLL        // 스크롤바
};

//------------------------------------------------------------------------------
/**
*/
uiEventItemSelect::uiEventItemSelect(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pEventMan =
        static_cast<uiEventMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_EVENT));
    assert (m_pEventMan);

    if (!this->m_pEventMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pEventMan이 없습니다."));

#endif
		this->ShowInterface(FALSE);
        return;
    }

    m_pListControl = NULL;

	for(int i=0; i<2; ++i)
	{
		m_vecEventList[i].clear();
	}

	for(int a=0; a<POS_TXT_SIZE; ++a)
	{
		m_pTxtControl[a] = NULL;
	}
	
}

//------------------------------------------------------------------------------
/**
*/
uiEventItemSelect::~uiEventItemSelect()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::Release()
{
	for(int i=0; i<2; ++i)
	{
		m_vecEventList[i].clear();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::InitControls()
{
	this->m_pListControl =	static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LIST));
	assert (this->m_pListControl);
	
	for(int i=0; i<POS_TXT_SIZE; ++i)
	{
		this->m_pTxtControl[i] = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_TXT_ITEM+i));
	}

    this->m_bControlsInitialized = true;
    CCtrlVScrollWZ *list_scroll_ = static_cast<CCtrlVScrollWZ *> (this->getControl(DLGPOS_VSCROLL));
	SUN_ASSERT(list_scroll_);
    if (list_scroll_)
    {
        list_scroll_->ShowWindowWZ(WZ_HIDE);
    }
	_initList();
}


//------------------------------------------------------------------------------
void uiEventItemSelect::OnShowWindow( BOOL val )
{
    if (val)
    {
		_initList();
        SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		for(int a=0; a<POS_TXT_SIZE; ++a)
		{
			if(m_pTxtControl[a])
				m_pTxtControl[a]->ShowWindowWZ(WZ_HIDE);
		}
    
		if(GetManager()->GetEventMode() == eEVENT_REGITEM_GETITEM)
		{
			// 재료, 참가횟수 보여준다
			if(m_pTxtControl[TXT_ITEM])
			{
				m_pTxtControl[TXT_ITEM]->ShowWindowWZ(WZ_SHOW);
			}

			if(m_pTxtControl[TXT_COUNT])
			{
				m_pTxtControl[TXT_COUNT]->ShowWindowWZ(WZ_SHOW);
			}
		}
		else if(GetManager()->GetEventMode() == eEVENT_REGITEM_RANDREWARD)	
		{
			// 재료만 보여준다.
			if(m_pTxtControl[TXT_ITEM2])
			{
				m_pTxtControl[TXT_ITEM2]->ShowWindowWZ(WZ_SHOW);
			}
		}

		m_pListControl->SetCurSel(-1);
		UpdateList();
		
		//
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_event_item_select_window;
        msg.wParam = IM_EVENT_MANAGER::EVENT_ITEM_SELECT;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_event_item_select_window);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szName[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_EXIT:
                {
					this->ShowInterface(FALSE);
					GetManager()->ShowEventWindow(TRUE);
                }
                break;

			case DLGPOS_BTN_OK:
				{
					
					int cursel = m_pListControl->GetCurSel();
					if (cursel < 0) 
					{
						const int c_string = 4014;
						GetUIMan()->GetInterfaceString(c_string, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						return;
					}

					int iIndex = 0;
					if(GetManager()->GetEventMode() == eEVENT_REGITEM_GETITEM)
					{
						iIndex = 0;

						if(!m_vecEventList[iIndex].size()) break;
						
						EVENT_INFO_LIST info = m_vecEventList[iIndex][cursel]; 
						
						GetUIMan()->GetInterfaceString( info.m_iNameString[0], szName, INTERFACE_STRING_LENGTH);
						
						GetUIMan()->GetInterfaceString( eST_EVENT_REGITEM_GETITEM_STRING, szTemp, INTERFACE_STRING_LENGTH);
						
						g_dwEventID		= info.m_dwEventID;

						Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName);
						
						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage, _Confirm_RequestRegItem);

					}
					else if(GetManager()->GetEventMode() == eEVENT_REGITEM_RANDREWARD)
					{
						iIndex = 1;

						if(!m_vecEventList[iIndex].size()) break;
						
						EVENT_INFO_LIST info = m_vecEventList[iIndex][cursel]; 
						
						GetUIMan()->GetInterfaceString( info.m_iNameString[0], szName, INTERFACE_STRING_LENGTH);
						
						GetUIMan()->GetInterfaceString( eST_EVENT_REGITEM_RANDREWARD_STRING, szTemp, INTERFACE_STRING_LENGTH);

						g_dwEventID		= info.m_dwEventID;

						Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName);
						
						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage, _Confirm_RequestRegItem);

					}

                }
                break;

			case DLGPOS_BTN_CANCEL:
				{
					this->ShowInterface(FALSE);
					GetManager()->ShowEventWindow(TRUE);
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
BOOL uiEventItemSelect::_isEnable()
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
int uiEventItemSelect::_getEventListIndex()
{
	int iEventMode = GetManager()->GetEventMode();
	int iIndex = -1;
	if(GetManager()->GetEventMode() == eEVENT_REGITEM_GETITEM)
	{
		iIndex = 0;
	}
	else if(GetManager()->GetEventMode() == eEVENT_REGITEM_RANDREWARD)
	{
		iIndex = 1;
	}

	return iIndex;
}

//------------------------------------------------------------------------------
void uiEventItemSelect::_initList()
{
	if (!GetManager())
        return;

	for(int i=0; i<2; ++i)
	{
		m_vecEventList[i].clear();
	}

	
	EVENT_SUB_INFO_ITER it;
	EVENTINFO_MAP_ITER	eventinfo_itr;
	SYSTEMTIME Current_systemTime;
	util::TimeSync::GetLocalTime(&Current_systemTime);
	std::pair<EVENT_SUB_INFO_ITER, EVENT_SUB_INFO_ITER> range;

	// m_vecEventList[0] 세팅
	range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_REGITEM_GETITEM);
	int iIndex = 0;
	for(it=range.first; it!=range.second; ++it)
	{
		EVENT_SUB_INFO info = (*it).second;

		EVENT_INFO_LIST inputInfo;
		ZeroMemory(&inputInfo, sizeof(EVENT_INFO_LIST));
		inputInfo.m_byCountMax = info.m_byCountMax;
		inputInfo.m_byEventMode = info.m_byEventMode;
		inputInfo.m_dwEventID = info.m_dwEventID;
		for(int i=0; i<5; ++i)
		{
			inputInfo.m_dwRegisterItemIndex[i] = info.m_dwRegisterItemIndex[i];
			inputInfo.m_wRegisterItemNum[i] = info.m_wRegisterItemNum[i];

			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( inputInfo.m_dwRegisterItemIndex[i] );

			if(pItemInfo)
			{
				inputInfo.m_iNameString[i] = pItemInfo->m_NCode;
			}
		}
		eventinfo_itr =   SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
		EVENT_INFO Eventinfo = (*eventinfo_itr).second;
		
		if( Eventinfo.CheckEvent( GetManager()->GetCurTime() ) )
			m_vecEventList[0].push_back(inputInfo);

		++iIndex;
	}

	// m_vecEventList[1] 세팅
	range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_REGITEM_RANDREWARD);
	iIndex = 0;
	
	for(it=range.first; it!=range.second; ++it)
	{
		EVENT_SUB_INFO info = (*it).second;

		EVENT_INFO_LIST inputInfo;
		ZeroMemory(&inputInfo, sizeof(EVENT_INFO_LIST));
		inputInfo.m_byCountMax = info.m_byCountMax;
		inputInfo.m_byEventMode = info.m_byEventMode;
		inputInfo.m_dwEventID = info.m_dwEventID;
		for(int i=0; i<5; ++i)
		{
			inputInfo.m_dwRegisterItemIndex[i] = info.m_dwRegisterItemIndex[i];
			inputInfo.m_wRegisterItemNum[i] = info.m_wRegisterItemNum[i];

			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( inputInfo.m_dwRegisterItemIndex[i] );
			if(pItemInfo)
			{
				inputInfo.m_iNameString[i] = pItemInfo->m_NCode;
			}
		}
		eventinfo_itr =   SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
		EVENT_INFO Eventinfo = (*eventinfo_itr).second;
		

		if( Eventinfo.CheckEvent( GetManager()->GetCurTime() ) )
			m_vecEventList[1].push_back(inputInfo);


		++iIndex;
	}
}

//------------------------------------------------------------------------------
void uiEventItemSelect::UpdateList()
{
    if (!GetManager())
        return;

	_refreshList();

	// 서버에 요청
	GetManager()->RequestEventStatus();
}

//------------------------------------------------------------------------------
void uiEventItemSelect::_refreshList()
{
    if (!GetManager())
        return;

	if(NULL == m_pListControl)
		return;

	DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
	DWORD c_dwSelTextColor = RGBA(0,220,0,255);
	DWORD c_dwBackColor = RGBA (0,0,0,0);


	TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp3[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	
    int current_sell_index = m_pListControl->GetCurSel();
	m_pListControl->DeleteItemAll();
	m_pListControl->SetBeginDrawIndex(0);

	int iIndex = _getEventListIndex();
	if(iIndex < 0) return;

	int idx = 0;

	for(int i=0; i!=m_vecEventList[iIndex].size(); ++i)
	{
		EVENT_INFO_LIST info = m_vecEventList[iIndex][i];
		if(GetManager()->GetEventMode() == eEVENT_REGITEM_GETITEM)	// %d/%d 형식
		{
			GetUIMan()->GetInterfaceString(info.m_iNameString[0] ,szTemp ,INTERFACE_STRING_LENGTH);
            Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%-21s  %-2ld / %-2ld ") ,szTemp ,info.m_iCurRegItemNum[0], info.m_byCountMax);
		}
		else if(GetManager()->GetEventMode() == eEVENT_REGITEM_RANDREWARD)	// %d 형식
		{
			GetUIMan()->GetInterfaceString(info.m_iNameString[0] ,szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("      %-27s") ,szTemp);
		}
		
		idx = m_pListControl->InsertItemBack();
		
		GlobalFunc::SetItemText( m_pListControl, idx ,szMessage);
		
		m_pListControl->SetItemFontID( idx,  0, StrToWzID("m212"));

		m_pListControl->SetItemTextColor(
				idx, 
				0, 
				c_dwTextColor
				);

		m_pListControl->SetItemBKColor(
				idx, 
				0, 
				c_dwBackColor
				);

		m_pListControl->SetItemSelTextColor(
				idx, 
				0, 
				c_dwSelTextColor
				);
	}
    if (current_sell_index > static_cast<int>(m_vecEventList[iIndex].size()))
    {
        current_sell_index = m_vecEventList[iIndex].size();	
    }
    m_pListControl->SetCurSel(current_sell_index);

}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::SuccessItemReg(CODETYPE dwEventID)
{	
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};

	EVENT_INFO *pInfo = SunEventInfoParser::Instance()->FindEventInfo(dwEventID);

	if(!pInfo)
	{
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1,  _T("이벤트 인덱스가 유효하지 않습니다."));
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return;
	}

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( pInfo->m_dwRegisterItemIndex[0] );
	if(pItemInfo)
	{
		// %s 아이템을 등록했습니다.
		GetUIMan()->GetInterfaceString( eST_EVENT_REGITEM_STRING, szTemp, INTERFACE_STRING_LENGTH);
		g_InterfaceManager.GetItemName( pItemInfo->m_Code, pItemInfo->m_pszName );
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pItemInfo->m_pszName);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	

		int iIndex = 0;
		if(pInfo->m_byEventMode == eEVENT_REGITEM_GETITEM)
		{
			iIndex = 0;
		}
		else if(pInfo->m_byEventMode == eEVENT_REGITEM_RANDREWARD)
		{
			iIndex = 1;
		}
		else
		{
			// 유효한 이벤트 모드가 아니다..
			return;
		}

		int iRegNum = 0;
		int iMaxNum = 0;

		std::vector<EVENT_INFO_LIST>::iterator it;
		for(it = m_vecEventList[iIndex].begin(); it != m_vecEventList[iIndex].end(); ++it)
		{
			if((*it).m_dwRegisterItemIndex[0] == pInfo->m_dwRegisterItemIndex[0]) 
			{
				iRegNum = (*it).m_iCurRegItemNum[0];
				iMaxNum = (*it).m_byCountMax;
				
				iRegNum++;

				if(iRegNum)
				{
					int iSub = iMaxNum - iRegNum;
					if(iSub > 0)
					{
						//4019	"%s" 아이템 %d개를
						//4020	더 모으시면 이벤트에 응모 하실 수 있습니다.
						GetUIMan()->GetInterfaceString( eST_EVENT_RANDREWARD_MSG1, szTemp, INTERFACE_STRING_LENGTH);
						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1,  szTemp, pItemInfo->m_pszName, iSub);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

						GetUIMan()->GetInterfaceString( eST_EVENT_RANDREWARD_MSG2, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					}
					else if(iSub == 0)
					{
						//4016	"%s" %d개를
						//4017	모두 모아 오프라인 이벤트에 자동 응모 되셨습니다.
						//4018	이벤트 종료 후, 공식 홈페이지를 통해 담첨자 확인을 하실 수 있습니다.
						GetUIMan()->GetInterfaceString( eST_EVENT_OFFLINE_MSG1, szTemp, INTERFACE_STRING_LENGTH);
						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pItemInfo->m_pszName, iMaxNum);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

						GetUIMan()->GetInterfaceString( eST_EVENT_OFFLINE_MSG2, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

						GetUIMan()->GetInterfaceString( eST_EVENT_OFFLINE_MSG3, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
				}
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::SetEventItemStatusList(MSG_BASE * pMsg)
{
	MSG_CG_EVENT_ITEM_STATUS_ACK *pRecvPacket = (MSG_CG_EVENT_ITEM_STATUS_ACK *)pMsg; 

 	for(int i=0; i < pRecvPacket->m_wEventNum; ++i)
	{
		EVENT_STATUS status = pRecvPacket->m_EventStatus[i];

		//임시: 특정 이벤트(uiEventItemSelect를 사용하지 않는.)는 스킵.
		DWORD dwEventNpcType[c_dw_Special_Event_Num] = {c_dw_Donation_NPC_Type, c_dw_SoloCouple_NPC_Type, c_dw_Santa_NPC_Type, c_dw_Buff_Event_NPC_Type };

		BOOL bVal = FALSE;
		for(int i = 0; i < c_dw_Special_Event_Num; ++i)
		{
			EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(dwEventNpcType[i]);
			if(NULL != pEventInfoVec)
			{
				int iCnt = pEventInfoVec->size();
				for(int j = 0; j < iCnt; ++j)
				{
					if((*pEventInfoVec)[j])
					{
						if(status.m_dwEventID == (*pEventInfoVec)[j]->m_dwEventID)
						{
							bVal = TRUE;
							break;
						}
					}
				}
			}
		}

		if(TRUE == bVal)
		{
			continue;
		}

		EVENT_INFO *pInfo = SunEventInfoParser::Instance()->FindEventInfo(status.m_dwEventID);

		if(pInfo)
		{
			int iIndex = 0;
			if(pInfo->m_byEventMode == eEVENT_REGITEM_GETITEM)
			{
				iIndex = 0;
			}
			else if(pInfo->m_byEventMode == eEVENT_REGITEM_RANDREWARD)
			{
				iIndex = 1;
			}
			else
			{
				// 유효한 이벤트 모드가 아니다..
				continue;
			}
			std::vector<EVENT_INFO_LIST>::iterator it;
			for(it = m_vecEventList[iIndex].begin(); it != m_vecEventList[iIndex].end(); ++it)
			{
				if((*it).m_dwRegisterItemIndex[0] == pInfo->m_dwRegisterItemIndex[0]) 
				{
					(*it).m_iCurRegItemNum[0] = status.m_wRegisterCnt;
					break;
				}
			}
		}
	}
	
	_refreshList();

}

//------------------------------------------------------------------------------
/**
*/
void uiEventItemSelect::NetworkProc( MSG_BASE * pMsg )
{
}
