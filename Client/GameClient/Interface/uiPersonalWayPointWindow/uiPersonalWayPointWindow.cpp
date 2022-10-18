#include "SunClientPrecompiledHeader.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "map.h"
#include "MapObject.h"
#include "ObjectManager.h"
#include "uiSystemMan/uiSystemMan.h"
#include <SCItemSlot.h>

#include "CastLeaveFieldDialog.h"  
#include "CooltimeManager.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

namespace
{
	enum
	{
		WayPointItemCoolTimeType = 14,
	};
}


namespace PERSIONERWARP 
{ 
	void SEND_CG_FREE_CHAR_WAYPOINT() 
	{ 
		uiPersonalWayPointWindow* pDlg =  
			GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG); 

		if(pDlg != NULL) 
		{ 
			const std::vector<WAYPOINTSLOT> warpdata = pDlg->GetWaypointdata(); 
 
			if (warpdata.empty()) 
			{ 
				return; 
			} 
 
			MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD sync; 
			sync.m_IdxWayPoint = warpdata[pDlg->GetSeleteData()].m_IdxWayPoint; 
  
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));  
  
			//////////////////////////////////////////////////////////////////////////  
			util::Timer *pTimer = xGetCooltimeManager()->GetItemCooltime(WayPointItemCoolTimeType);  
			if (pTimer)  
			{   
				pTimer->Reset();  
			}  
		}  
		  
	}  
};  


//------------------------------------------------------------------------------
WzID uiPersonalWayPointWindow::m_wzId[DIALOG_WP_MAX] = 
{
        StrToWzID("B001"),	//DLGPOS_WP_BTN_EXIT
		StrToWzID("B002"),	//DLGPOS_WP_BTN_SAVE
		StrToWzID("B003"),	//DLGPOS_WP_BTN_DELETE
		StrToWzID("BT00"),	//DLGPOS_WP_BTN_MOVE

		StrToWzID("L001"),	//DLGPOS_WP_LST
			
		StrToWzID("VS00"),	//DLGPOS_WP_VSCROLL
};

//------------------------------------------------------------------------------
/**
*/
uiPersonalWayPointWindow::uiPersonalWayPointWindow(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bRecvWayPointInfo(false)
{
    m_pWayPointMan = GET_DIALOG_MANAGER(uiPersonalWayPointMan, UIMAN_PERSONAL_WAYPOINT);
    assert (m_pWayPointMan);

	m_pListControl	= NULL;
	m_pScroll		= NULL;
	ZeroMemory(&m_pBtnControls, sizeof(m_pBtnControls));

	m_bWayPointInfoPending	= false;
	m_bRecvWayPointInfo		= false;
	m_vecListData.clear();
}

//------------------------------------------------------------------------------
/**
*/
uiPersonalWayPointWindow::~uiPersonalWayPointWindow()
{
    Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::InitControls()
{
	m_pListControl = 
		static_cast<CCtrlListWZ *>(getControl(DLGPOS_WP_LST));
	assert(this->m_pListControl);


	m_pScroll = 
		static_cast<CCtrlVScrollWZ *>(getControl(DLGPOS_WP_VSCROLL));
	assert(this->m_pScroll);

	for(int i=0; i<POS_WP_BTN_SIZE; ++i)
	{
		m_pBtnControls[i] =
			static_cast<CCtrlButtonWZ *>(getControl(DLGPOS_WP_BTN_EXIT+i));
		assert(m_pBtnControls[i]);
	}
}


//------------------------------------------------------------------------------
void uiPersonalWayPointWindow::OnShowWindow( BOOL val )
{
	if (val)
    {
        //uiBase::MoveCenter();

		keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_Personal_Waypoint;
        msg.wParam = InterfaceManager::DIALOG_PERSONAL_WAYPOINT;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_iSelectedIndex = -1;

		if(m_pScroll)
		{
			m_pScroll->SetScrollPosWZ(0);
		}

		RefreshList();

	}
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Personal_Waypoint);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiPersonalWayPointWindow::_isEnable()
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
void uiPersonalWayPointWindow::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

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
					m_pWayPointMan->ShowWayPointWindow(false);
                }
                break;

			case DLGPOS_WP_BTN_SAVE:// 웨이포인트 저장
				{
					if ( IsDialogBtnPending() == true )
						break;

					// 10개 미만(9개이하)일 경우 세이브
#ifdef _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
                    if (m_vecListData.size() < MAX_CHAR_WAYPOINT_NUM)
#else
					if (m_vecListData.size() < 10)
#endif //_NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
					{
						const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( (MAPCODE)g_pMap->GetMapID() );
						if ( pMapInfo )
						{
							TCHAR szText[INTERFACE_STRING_LENGTH];
							TCHAR szBuf[INTERFACE_STRING_LENGTH];
							TCHAR szMapName[INTERFACE_STRING_LENGTH];

							// 맵 이름
							g_InterfaceManager.GetInterfaceString( pMapInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH );

							// 1875 %s 좌표를 추가합니다. 계속 하시겠습니까?
							g_InterfaceManager.GetInterfaceString(1875, szBuf, INTERFACE_STRING_LENGTH);					
							Snprintf( szText, INTERFACE_STRING_LENGTH, szBuf, szMapName) ;

							// 5933	현재위치 저장하기
							uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_Confirm(NULL, GlobalFunc::SavePersnalWaypoint, NULL/*szTitle*/, szText);

							SetDialogBtnPending(true);
						}
					}
					else
					{
						// 알림창 1878 최대 10개까지 좌표를 저장할 수 있습니다.
						TCHAR szText[INTERFACE_STRING_LENGTH];

						g_InterfaceManager.GetInterfaceString(1878, szText, INTERFACE_STRING_LENGTH);					

						uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Verify( szText );
					}
				}
				break;
			case DLGPOS_WP_BTN_DELETE:// 웨이포인트 삭제
				{
					if ( IsDialogBtnPending() == true )
						break;

					// 리스트 선택이 안되었으면 무시
					if(m_pListControl)
					{
						m_iSelectedIndex = m_pListControl->GetCurSel();
						if ( (m_iSelectedIndex >= 0) && ((unsigned int)m_iSelectedIndex < m_vecListData.size()) )
						{
							// 알림창 1876	%d, %d, %d 좌표를 삭제합니다. 이후 12시간 동안 좌표 삭제가 불가능합니다. 계속 하시겠습니까?
							TCHAR szBuf[INTERFACE_STRING_LENGTH];
							TCHAR szText[INTERFACE_STRING_LENGTH];

							g_InterfaceManager.GetInterfaceString(1876, szBuf, INTERFACE_STRING_LENGTH);

							WAYPOINTSLOT waypointSlot = m_vecListData[m_iSelectedIndex];
							Snprintf( szText, INTERFACE_STRING_LENGTH, szBuf, 
										waypointSlot.m_X, waypointSlot.m_Y, waypointSlot.m_Z) ;

							uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_Confirm(NULL, GlobalFunc::DeletePersnalWaypoint, NULL/*szTitle*/, szText);
							
							SetDialogBtnPending(true);
						}
					}
				}
				break;
			case DLGPOS_WP_BTN_MOVE:
				{
					MoveWayPoint();
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


//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::MoveWayPoint()
{
    if ( IsDialogBtnPending() == true )
        return;

    if(m_pListControl)
    {
        m_iSelectedIndex = m_pListControl->GetCurSel();
        if((m_iSelectedIndex >= 0) && ((unsigned int)m_iSelectedIndex < m_vecListData.size()))
        {
            // 알림창 1877	%d, %d, %d 좌표로 이동합니다. 계속 하시겠습니까?
            TCHAR szBuf[INTERFACE_STRING_LENGTH];
            TCHAR szText[INTERFACE_STRING_LENGTH];

            g_InterfaceManager.GetInterfaceString(1877, szBuf, INTERFACE_STRING_LENGTH);

            WAYPOINTSLOT waypointSlot = m_vecListData[m_iSelectedIndex];
            Snprintf( szText, INTERFACE_STRING_LENGTH, szBuf, 
                waypointSlot.m_X, waypointSlot.m_Y, waypointSlot.m_Z) ;

            uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if(pSystemMan)
                pSystemMan->Sys_Confirm(NULL, GlobalFunc::ExecutePersnalWaypoint, NULL/*szTitle*/, szText);

            SetDialogBtnPending(true);
        }
    }
}
//----------------------------------------------------------------------------
/**
*/
void
uiPersonalWayPointWindow::DownScroll(int nRange)
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
uiPersonalWayPointWindow::UpScroll(int nRange)
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
uiPersonalWayPointWindow::UpdateMouseWheel()
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
void uiPersonalWayPointWindow::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

	UpdateMouseWheel();

	if(m_pScroll && m_pListControl)
	{
		int iCurScrollPos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다

		int iStartDrawIndex = iCurScrollPos;

		m_pListControl->SetBeginDrawIndex( iStartDrawIndex );

        int iMouseX = Mouse::Instance()->GetMouseX();
        int iMouseY = Mouse::Instance()->GetMouseY();

        RECT rect = m_pListControl->GetSizeRect();

        if ((rect.left <= iMouseX) && (iMouseX <= rect.right) &&
            (rect.top <= iMouseY) && (iMouseY <= rect.bottom))
        {
            // 46330 룬스톤이 활성화 되어있는 기간동안 현재의 좌표를 저장하고, 저장된 좌표로 순간이동을 할 수 있습니다.
            //       \n\n 저장 가능한 최대 좌표수 : %d\n\n 좌표 삭제는 %d분마다 1개씩 제거할 수 있습니다.
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH,
                46330, MAX_CHAR_WAYPOINT_NUM, SCWayPointInfo::eDeleteTime);

            uiToolTipMan* tooltip_man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if (tooltip_man)
            {
                tooltip_man->RegisterTooltipSimpleText(string);
            }
        }
	}

	CheckWaypointEnable();
}

//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN()
{
	//if (!m_bRecvWayPointInfo && !m_bWayPointInfoPending)
	{
		MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN sync;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));

		GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CHAT, _T("개인 웨이포인트 정보 요청 패킷 보냄"));
		m_bWayPointInfoPending = true;
	}	
}
void uiPersonalWayPointWindow::RECV_CG_CHAR_WAYPOINT_INFO_LOAD_ACK(MSG_BASE * pMsg)
{
	MSG_CG_CHAR_WAYPOINT_INFO_LOAD_ACK* pRecv = (MSG_CG_CHAR_WAYPOINT_INFO_LOAD_ACK*)pMsg;

	GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CHAT, _T("개인 웨이포인트 정보 요청 패킷 받음"));

	// 웨이포인트 목록 저장
	CHAR_WAYPOINT_TOTAL_INFO Total_Info;
	memcpy(&Total_Info, &(pRecv->m_Info), sizeof(CHAR_WAYPOINT_TOTAL_INFO));
	
    m_vecListData.resize(Total_Info.m_Count);
	for (int i = 0;i < Total_Info.m_Count;++i)
	{
		m_vecListData[i] = Total_Info.m_Slot[i];
	}

	// WAYPOINTSLOT::m_IdxWayPoint 을 이용한 정렬필요(?)
	//...

	m_bRecvWayPointInfo		= true;
	m_bWayPointInfoPending	= false;
}
void uiPersonalWayPointWindow::SEND_CG_CHAR_WAYPOINT_INFO_SAVE_SYN()
{
	MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN sync;
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
}
void uiPersonalWayPointWindow::RECV_CG_CHAR_WAYPOINT_INFO_SAVE_ACK(MSG_BASE * pMsg)
{
	MSG_CG_CHAR_WAYPOINT_INFO_SAVE_ACK* pRecv = (MSG_CG_CHAR_WAYPOINT_INFO_SAVE_ACK*)pMsg;
	WAYPOINTSLOT waypointSlot;

	memcpy(&waypointSlot, &(pRecv->m_Info), sizeof(WAYPOINTSLOT));

	m_vecListData.push_back(waypointSlot);

	// 저장 후 목록 갱신 필요
	RefreshList();
}
void uiPersonalWayPointWindow::SEND_CG_CHAR_WAYPOINT_INFO_DEL_SYN()
{
	MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN sync;
	sync.m_IdxWayPoint = m_vecListData[m_iSelectedIndex].m_IdxWayPoint;
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
}

void uiPersonalWayPointWindow::RECV_CG_CHAR_WAYPOINT_INFO_DEL_ACK(MSG_BASE * pMsg)
{
	MSG_CG_CHAR_WAYPOINT_INFO_DEL_ACK* pRecv = (MSG_CG_CHAR_WAYPOINT_INFO_DEL_ACK*)pMsg;
	
	DWORD dwIndex = pRecv->m_IdxWayPoint;

	// 해당 웨이포인트 인덱스를 갖는 슬롯 데이터를 찾아 삭제
	WAYPOINT_VEC_ITR itr_End	= m_vecListData.end();
	for ( WAYPOINT_VEC_ITR itr = m_vecListData.begin() ; itr != itr_End; )
	{
		if ( itr->m_IdxWayPoint == dwIndex)
		{
			m_vecListData.erase(itr);
			// 삭제 후 목록 갱신 필요
			RefreshList();
			break;
		}
		++itr;
	}
	RefreshList();
}

void uiPersonalWayPointWindow::SEND_CG_CHAR_WAYPOINT_INFO_EXE_CMD()
{
#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
	SCItemSlot* pItemSlot = GetManager()->GetActiveWaypointItem();
	if(pItemSlot == NULL)
	{
		//룬스톤이 없거나 활성화 된 것이 없음.
		TCHAR	isNotRuneStone[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(158643, isNotRuneStone);
		GlobalFunc::OutputMsg_SysMsg(isNotRuneStone);
		return;
	}
#endif //_NA_000000_20121210_REFACTORY_GAME_USER

	keyMsg msg; 
	ZeroMemory(&msg,sizeof(msg)); 
	msg.dwType		= eDoType_PersonalWarp; 
	msg.DoSomething	= GlobalFunc::DoLeaveFieldCancel; 
	msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING; 
	msg.wParam		= eDoType_PersonalWarp; 
	g_KeyQueueManager.PushBack(msg); 
 
	// 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
	StartLeaveFieldCastTimer( eDoType_PersonalWarp, DEALY_SPAWN_VILLAGE );   
}

void uiPersonalWayPointWindow::RefreshList()
{
	// 처음 창을 열어 웨이포인트 정보를 얻는다.
	if (m_bRecvWayPointInfo == false)
		return;

	if (m_pListControl && GetManager())
	{
		m_pListControl->DeleteItemAll();

		int mDataSize = m_vecListData.size();
		for (int i = 0;i < mDataSize;++i)
		{
			WAYPOINTSLOT& waypointSlot = m_vecListData[i];
			const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( waypointSlot.m_MapCode );

			if (pMapInfo)
			{
				DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
				DWORD c_dwSelTextColor = RGBA(0,220,0,255);
				DWORD c_dwBackColor = RGBA(0,0,0,0);

				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szMapName[INTERFACE_STRING_LENGTH] = {0,};

				g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH);

				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, "%s (%d, %d, %d)" , 
					szMapName, waypointSlot.m_X, waypointSlot.m_Y, waypointSlot.m_Z);

				int nIndex = m_pListControl->InsertItemBack();
				GlobalFunc::SetItem(m_pListControl , nIndex , 0 , szMessage, StrToWzID("m212"),   DT_LEFT ,
					c_dwBackColor, c_dwTextColor, c_dwSelTextColor);				
			}
		}
		m_pListControl->SetCurSel(-1);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPersonalWayPointWindow::NetworkProc( MSG_BASE * pMsg )
{
}

void uiPersonalWayPointWindow::CheckWaypointEnable()
{
	m_pBtnControls[DLGPOS_WP_BTN_SAVE]->EnableWindowWZ(false);
	m_pBtnControls[DLGPOS_WP_BTN_DELETE]->EnableWindowWZ(false);
	m_pBtnControls[DLGPOS_WP_BTN_MOVE]->EnableWindowWZ(false);

	SCItemSlot* pItemSlot = GetManager()->GetActiveWaypointItem();
	if ( pItemSlot )
	{
		// 배틀존이 아니면 추가, 이동 버튼 활성화
		if ( !GlobalFunc::IsBattleZone() )
		{
			m_pBtnControls[DLGPOS_WP_BTN_SAVE]->EnableWindowWZ(true);
			m_pBtnControls[DLGPOS_WP_BTN_MOVE]->EnableWindowWZ(true);
		}
		// 삭제 가능상태면 삭제 버튼 활성화
        if ( pItemSlot->CanWayPointDelete() )
        {
			m_pBtnControls[DLGPOS_WP_BTN_DELETE]->EnableWindowWZ(true);
        }

		//쿨타임이 남아있다면 이동 버튼 비활성화
		util::Timer *pTimer = xGetCooltimeManager()->GetItemCooltime(WayPointItemCoolTimeType);
		if (pTimer && !pTimer->IsExpired(FALSE))
			m_pBtnControls[DLGPOS_WP_BTN_MOVE]->EnableWindowWZ(false);
	}
	
}

bool uiPersonalWayPointWindow::IsWayPointItemPending()
{
	return m_bWayPointItemPending;
}
void uiPersonalWayPointWindow::SetWayPointItemPending(bool bFlag)
{
	m_bWayPointItemPending = bFlag;
}

bool uiPersonalWayPointWindow::IsDialogBtnPending()
{
	return m_bDialgBtnPending;
}
void uiPersonalWayPointWindow::SetDialogBtnPending(bool bFlag)
{
	 m_bDialgBtnPending = bFlag;
}

void uiPersonalWayPointWindow::InitWayPoint()
{
	m_vecListData.clear();
	m_bRecvWayPointInfo		= false;
	m_bWayPointInfoPending	= false;
	m_bWayPointItemPending	= false;
	m_iSelectedIndex		= -1;
}
