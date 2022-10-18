#include "SunClientPrecompiledHeader.h"

#include "uiWayPointWindow.h"

#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "map.h"
#include "MapObject.h"
#include "ObjectManager.h"
#include "uiSystemMan/uiSystemMan.h"
#ifdef _NA_20100307_BETAKEY_SYSTEM
#include "uiLoginMan/uiLoginMan.h"
#include "BetaKeyImpl.h"
#endif//_NA_20100307_BETAKEY_SYSTEM

const int c_iMaxListLine = 16;
WORD	g_wPendingWayPointIndex = 0;

//------------------------------------------------------------------------------
void _Confirm_MoveWayPoint(bool bYes)
{
	if(bYes)
	{
		uiWayPointMan* pWayPointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
		if(pWayPointMan)
		{
			// 이동요청
			if (g_wPendingWayPointIndex)
            {
				MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
                sync.m_wIndex = g_wPendingWayPointIndex;
                GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));

				pWayPointMan->ShowWayPointWindow(false);
            }
		}
	}
	g_wPendingWayPointIndex = 0;
}

//------------------------------------------------------------------------------
WzID uiWayPointWindow::m_wzId[DIALOG_WP_MAX] = 
{
        StrToWzID("B001"),	//DLGPOS_WP_BTN_EXIT
		StrToWzID("B002"),	//DLGPOS_WP_BTN_OK
		StrToWzID("B003"),	//DLGPOS_WP_BTN_CANCEL
		StrToWzID("L001"),	//DLGPOS_WP_LST
		StrToWzID("VS00"),	//DLGPOS_WP_VSCROLL
		StrToWzID("T002"),	//DLGPOS_AREA_NAME
		StrToWzID("T003"),	//DLGPOS_COST
};

//------------------------------------------------------------------------------
/**
*/
uiWayPointWindow::uiWayPointWindow(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pWayPointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
    assert (m_pWayPointMan);

    if (!this->m_pWayPointMan)
    {
//#ifdef _DEBUG
//        this->GetUIMan()->Sys_VerifyLock("m_pWayPointMan이 없습니다.");
//#endif
        this->ShowInterface(FALSE);
        return;
    }

	this->m_pListControl = NULL;
	this->m_pScroll = NULL;
	ZeroMemory(&this->m_pBtnControls, sizeof(this->m_pBtnControls));

	m_bOpen = false;
	m_vecListData.clear();
	m_dwConfirm_MoveWayPointKey = 0;
}

//------------------------------------------------------------------------------
/**
*/
uiWayPointWindow::~uiWayPointWindow()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::InitControls()
{
	this->m_pListControl = 
		static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_WP_LST));
	assert(this->m_pListControl);


	this->m_pScroll = static_cast<CCtrlVScrollWZ*>(this->getControl(DLGPOS_WP_VSCROLL));
	assert(this->m_pScroll);

	for(int i=0; i<POS_WP_BTN_SIZE; ++i)
	{
		this->m_pBtnControls[i] =
			static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_WP_BTN_EXIT+i));
		assert(this->m_pBtnControls[i]);
	}

	m_pListControl->SetHideReportRender( TRUE );

	CControlWZ  * pCtrl1 = 	getControl( DLGPOS_AREA_NAME );
	CControlWZ  * pCtrl2 = 	getControl( DLGPOS_COST );

	GlobalFunc::SetReportButtonInfo( m_pListControl, 0, pCtrl1->GetSize().right, NULL );
	GlobalFunc::SetReportButtonInfo( m_pListControl, 1, pCtrl2->GetSize().right, NULL );

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiWayPointWindow::OnShowWindow( BOOL val )
{
	if (val)
    {
        ////uiBase::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_waypoint;
        msg.wParam = InterfaceManager::DIALOG_WAYPOINT;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_bOpen = true;
		m_iSelectedIndex = -1;
		g_wPendingWayPointIndex = 0;
		m_dwConfirm_MoveWayPointKey = 0;

		if(m_pScroll)
		{
			m_pScroll->SetScrollPosWZ(0);
		}
		RefreshList();
	}
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_waypoint);

		if(0 != m_dwConfirm_MoveWayPointKey)
		{
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
			{
				pSystemMan->Destroy_Sys_Confirm(m_dwConfirm_MoveWayPointKey);
                m_dwConfirm_MoveWayPointKey = 0;
                g_wPendingWayPointIndex = 0;
			}
		}
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiWayPointWindow::_isEnable()
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
void uiWayPointWindow::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONDCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_WP_LST:
                {
                    if (m_pListControl != NULL)
                    {
                        m_iSelectedIndex = m_pListControl->GetCurSel();
                        MoveWayPoint();
                    }
                }
                break;
            }
        }
        break;
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_WP_BTN_EXIT:
                {
					m_bOpen = false;
                }
                break;

			case DLGPOS_WP_BTN_OK:
				{
					if(m_pListControl)
					{
						m_iSelectedIndex = m_pListControl->GetCurSel();
						MoveWayPoint();
					}
				}
				break;

			case DLGPOS_WP_BTN_CANCEL:
				{
					m_bOpen = false;
				}
				break;

            }
        }
        break;


	case RT_MSG_SB_LINETOP:
	case RT_MSG_SB_PAGETOP:
		{
			//.윗쪽 화살표를 눌렀다.
			if(m_pScroll)
			{
				UpScroll(1);
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_PAGEBOTTOM:
		{
			//.아랫쪽 화살표를 눌렀다
			if(m_pScroll)
			{
				DownScroll(1);
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
//----------------------------------------------------------------------------
/**
*/
void
uiWayPointWindow::DownScroll(int nRange)
{
	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));

		this->m_pScroll->SetScrollPosWZ(pos);
	}
}
//----------------------------------------------------------------------------
/**
*/
void
uiWayPointWindow::UpScroll(int nRange)
{
	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));

		this->m_pScroll->SetScrollPosWZ(pos);
	}

}
//----------------------------------------------------------------------------
/**
*/
void
uiWayPointWindow::UpdateMouseWheel()
{
	if(!m_pListControl) return;

	RECT rc=m_pListControl->GetSizeRect();
	
	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP: UpScroll(1);break;

	case MOUSE_WHEEL_DOWN:DownScroll(1);break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

	UpdateMouseWheel();

	// 웨이포인트 오브젝트와 멀어지면 창 닫아준다
	if (GetManager()->GetPendingWayPointID())
	{
		MapObject * pObject = (MapObject *)g_ObjectManager.GetObject(GetManager()->GetPendingWayPointID());
		if(pObject && g_pHero)
		{
			WzVector vDiff = pObject->GetPosition() - g_pHero->GetVisiblePos();
			float distance = VectorLength2( &vDiff );
			if ( distance > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
			{
				m_bOpen = false;
				GetManager()->ClearPendingWayPointID();
			}
		}
	}

	if(!m_bOpen)
	{
		this->ShowInterface(FALSE);
		GetManager()->ClearPacketStatus();
	}

	if(m_pScroll && m_pListControl)
	{
		int iCurScrollPos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다

		int iStartDrawIndex = iCurScrollPos;

		m_pListControl->SetBeginDrawIndex( iStartDrawIndex );
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::RefreshList()
{
	if(m_pListControl && GetManager())
	{
		int iCurSel = -1;

		m_pListControl->DeleteItemAll();
		m_vecListData.clear();

		stWayPointData data;

		std::vector<DWORD>		vecCategory = GetManager()->GetCategory();
		WAYPOINT_MULTIMAP		mapWayPoint = GetManager()->GetWayPoint();

		if(vecCategory.size() && mapWayPoint.size() && g_pMap)
		{
			int iListSize = vecCategory.size() + mapWayPoint.size() -1;
			int iPage = 0;

			if(iListSize > 0)
			{
				iPage = (iListSize/(c_iMaxListLine))*c_iMaxListLine;
			}
			
			if(iPage<0)
				iPage=0;
			
			if(m_pScroll)
				m_pScroll->SetScrollRangeWZ(0,iPage);	//스크롤 범위


			int idx = 0;
			TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

			DWORD c_dwCategoryTextColor = RGBA(0, 128, 128, 255);
			DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
			DWORD c_dwSelTextColor = RGBA(0,220,0,255);
			DWORD c_dwBackColor = RGBA(0,0,0,0);


			TCHAR szLevel[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szLevelInfo[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(1729, szLevel, INTERFACE_STRING_LENGTH);

#ifdef _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL
            TCHAR szExtendLevel[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(2445, szExtendLevel, INTERFACE_STRING_LENGTH);
#endif // _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL

			WAYPOINT_ITER it;
			std::pair<WAYPOINT_ITER, WAYPOINT_ITER> range;

			for(unsigned int i=0; i<vecCategory.size(); ++i)
			{
				// 카테고리 add
				g_InterfaceManager.GetInterfaceString(vecCategory[i], szTemp, INTERFACE_STRING_LENGTH);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%-s"), szTemp);

				idx = m_pListControl->InsertItemBack();
				ZeroMemory(&data, sizeof(stWayPointData));
				m_vecListData.push_back(data);

				GlobalFunc::SetItemText(m_pListControl, idx, 0, szMessage);
#ifdef _INTERNATIONAL_UI				
				m_pListControl->SetItemFontID(idx, 0, StrToWzID("st10"));
#else
				m_pListControl->SetItemFontID(idx, 0, StrToWzID("m212"));	
#endif//_INTERNATIONAL_UI				

				m_pListControl->SetItemTextColor( idx, 0, c_dwCategoryTextColor );
				m_pListControl->SetItemBKColor( idx, 0, c_dwBackColor );
				m_pListControl->SetItemSelTextColor( idx, 0, c_dwCategoryTextColor );

				// 해당 카테고리에 속하는 웨이포인트 리스트 add
				range = mapWayPoint.equal_range(vecCategory[i]);

				for(it=range.first; it!=range.second; ++it)
				{
					ZeroMemory(&data, sizeof(stWayPointData));
					data = (*it).second;

					//지역명 
					g_InterfaceManager.GetInterfaceString(data.m_dwNCode, szName, INTERFACE_STRING_LENGTH);
					
					if(data.m_EnterencePortal.wFieldTo == g_pMap->GetCurrentFieldID())
					{
						//현재위치라면 <현재위치>로 표시
						g_InterfaceManager.GetInterfaceString(1334, szMoney, INTERFACE_STRING_LENGTH);
						iCurSel = idx+1;
					}
					else
					{
						//이동수수료 
						g_InterfaceManager.GetInterfaceString(999, szTemp, INTERFACE_STRING_LENGTH);

						TCHAR szMoneyTemp[INTERFACE_STRING_LENGTH] = {0,};
						TCHAR szMoneyTempFinal[INTERFACE_STRING_LENGTH] = {0,};
						NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
#ifdef _NA_20100307_BETAKEY_SYSTEM
                        DWORD portal_price = data.m_EnterencePortal.dwHeim;
                        uiLoginMan* login_manager = static_cast<uiLoginMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
                        if (login_manager)
                        {
                            if (login_manager->CanAbilityBetaKey(data.m_EnterencePortal.nBeytaKey))
                            {
                                portal_price = BetaKeyImpl::GetPortalMoney(login_manager->GetBetaKeyInfo(), &(data.m_EnterencePortal));
                            }
                        }
                        Sprintf(szMoneyTemp, _T("%I64u"), (MONEY)portal_price);
#else
						Sprintf(szMoneyTemp, _T("%I64u"), (MONEY)data.m_EnterencePortal.dwHeim);
#endif//_NA_20100307_BETAKEY_SYSTEM
						::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoneyTemp, &nFmt, szMoneyTempFinal, sizeof(szMoneyTempFinal));
						Snprintf(szMoney, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szMoneyTempFinal, szTemp );
					}

					

					idx = m_pListControl->InsertItemBack();

#ifdef _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL
                    const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( data.m_EnterencePortal.wFieldTo );
                    if (pMapInfo)
                    {
                        if (pMapInfo->MapCode == 20206)
                        {
                            Snprintf(szLevelInfo, INTERFACE_STRING_LENGTH-1, szExtendLevel, data.m_EnterencePortal.byMin_Level, data.m_EnterencePortal.byMax_Level);
                        }
                        else
                        {
                            Snprintf(szLevelInfo, INTERFACE_STRING_LENGTH-1, szLevel, data.m_EnterencePortal.byMin_Level);
                        }
                    }                    
#else
					Snprintf(szLevelInfo, INTERFACE_STRING_LENGTH-1, szLevel, data.m_EnterencePortal.byMin_Level);
#endif // _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL
					strcat(szName, szLevelInfo);
					DWORD fontID = StrToWzID("st10");

					GlobalFunc::SetItem( m_pListControl , idx , 0 , szName, fontID, DT_RIGHT,
						c_dwBackColor, c_dwTextColor, c_dwSelTextColor );
					GlobalFunc::SetItem( m_pListControl , idx , 1 , szMoney, fontID, DT_RIGHT,
						c_dwBackColor, c_dwTextColor, c_dwSelTextColor );

					m_vecListData.push_back(data);
					
				}
			}
		}

		m_pListControl->SetCurSel(iCurSel);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::NetworkProc( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointWindow::MoveWayPoint()
{
    if ((g_pHero == NULL) ||
        (g_pMap == NULL))
    {
        return;
    }

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    if ((m_iSelectedIndex >= 0) && ((unsigned int)m_iSelectedIndex < m_vecListData.size()))
	{
		stWayPointData data = m_vecListData[m_iSelectedIndex];
		if (data.m_dwNCode)
		{
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
			if (data.m_EnterencePortal.wFieldTo == g_pMap->GetCurrentFieldID())
			{
				return;
			}

			// 레벨제한조건이 최우선
			if (g_pHero->GetLevel() < data.m_EnterencePortal.byMin_Level)
			{
				TCHAR szMapName[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szFinal[INTERFACE_STRING_LENGTH] = {0,};

				//1728	%s 웨이포인트는 %d레벨이상부터 가능합니다.
				g_InterfaceManager.GetInterfaceString(1728, szMessage, INTERFACE_STRING_LENGTH);
				g_InterfaceManager.GetInterfaceString(data.m_dwNCode, szMapName, INTERFACE_STRING_LENGTH);
			
				//최종메시지
				Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage,szMapName,(int)data.m_EnterencePortal.byMin_Level);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
                {
					pSystemMan->Sys_Verify(szFinal);
                }                
			}
            else if (g_pHero->GetMoney() >= data.m_EnterencePortal.dwHeim)
			{
				g_wPendingWayPointIndex = data.m_EnterencePortal.wIndex;
				
				//6002	%s 웨이포인트로 이동합니다. 이동 수수료 %d하임이 필요합니다.
				g_InterfaceManager.GetInterfaceString(6002, szTemp, INTERFACE_STRING_LENGTH);

				TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(data.m_dwNCode, szName, INTERFACE_STRING_LENGTH);

				TCHAR szMoneyTemp[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szMoneyTempFinal[INTERFACE_STRING_LENGTH] = {0,};
				NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
				Sprintf(szMoneyTemp, _T("%I64u"), (MONEY)data.m_EnterencePortal.dwHeim);
				::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoneyTemp, &nFmt, szMoneyTempFinal, sizeof(szMoneyTempFinal));
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName, szMoneyTempFinal );

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
				{
                    if (m_dwConfirm_MoveWayPointKey)
                    {
                        pSystemMan->Destroy_Sys_Confirm(m_dwConfirm_MoveWayPointKey);
                        m_dwConfirm_MoveWayPointKey = 0;
                    }
					m_dwConfirm_MoveWayPointKey = pSystemMan->Sys_Confirm(szMessage, _Confirm_MoveWayPoint);
				}
			}
			else
			{
				// 돈 부족
				//6003	해당 웨이포인트로 이동하기 위해서는 %d하임이 필요합니다.
				g_InterfaceManager.GetInterfaceString(6003, szTemp, INTERFACE_STRING_LENGTH);
				TCHAR szMoneyTemp[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szMoneyTempFinal[INTERFACE_STRING_LENGTH] = {0,};
				NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
				Sprintf(szMoneyTemp, _T("%I64u"), (MONEY)data.m_EnterencePortal.dwHeim);
				::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoneyTemp, &nFmt, szMoneyTempFinal, sizeof(szMoneyTempFinal));
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szMoneyTempFinal );
				uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if (pSystemMan)
                {
					pSystemMan->Sys_Verify(szMessage);
                }
			}
		}
		else
		{
			//6004	이동할 웨이포인트를 선택하세요
			g_InterfaceManager.GetInterfaceString(6004, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
	}
	else
	{
		//6004	이동할 웨이포인트를 선택하세요
		g_InterfaceManager.GetInterfaceString(6004, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
}
//------------------------------------------------------------------------------
stWayPointData* uiWayPointWindow::GetWayPointData(MAPID dest_mapid, FIELDID dest_fieldid)
{
    std::vector<stWayPointData>::iterator begin = m_vecListData.begin();
    std::vector<stWayPointData>::iterator end = m_vecListData.end();

    for (; begin != end; ++ begin)
    {
        if ((dest_mapid == begin->m_EnterencePortal.wWorldTo) && 
            (dest_fieldid == begin->m_EnterencePortal.wFieldTo))
        {
            return &(*begin);
        }
    }
    return NULL;
}