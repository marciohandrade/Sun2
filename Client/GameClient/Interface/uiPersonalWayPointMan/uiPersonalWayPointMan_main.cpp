#include "SunClientPrecompiledHeader.h"
#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "interfacemanager.h"
#include "MapInfoParser.h"
#include "GlobalFunc.h"
#include "map.h"
#include "ResultCode.h"
#include "ItemManager.h"
#include "SCSlotContainer.h"
#include "SCItemSlot.h"

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
#include "CastLeaveFieldDialog.h"
#endif // _NA_000000_20121210_REFACTORY_GAME_USER

using namespace RC;

//------------------------------------------------------------------------------
/**
*/
uiPersonalWayPointMan::uiPersonalWayPointMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan), m_pUIWayPoint(NULL)
{
    m_pUIWayPoint = NULL;
}

//------------------------------------------------------------------------------
/**
*/

void uiPersonalWayPointMan::InitWayPoint()
{
	if (m_pUIWayPoint)
	{
		m_pUIWayPoint->InitWayPoint();
	}	
}

void uiPersonalWayPointMan::SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN()
{
	if(m_pUIWayPoint)
	{
		// 목록 수신 여부 체크
		if ( !m_pUIWayPoint->IsRecvWaypointInfo() )
		{
			if ( m_pUIWayPoint->IsWaypointInfoPending() == false )
			{
				m_pUIWayPoint->SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();
				GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("개인 웨이포인트 정보 요청 패킷 보냄"));
			}
			else
			{
				GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("개인 웨이포인트 정보 요청 대기중"));
			}
		}			
	}
}

void uiPersonalWayPointMan::ShowWayPointWindow(bool bShow)
{
	if(m_pUIWayPoint)
	{
		m_pUIWayPoint->ShowInterface(bShow);
	}
}

SCItemSlot* uiPersonalWayPointMan::GetActiveWaypointItem()
{
	SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (!pAtContainer)
    {
        return NULL;
    }

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
			continue;

		SCSlot& rSlot = pAtContainer->GetSlot(i);
		if ( rSlot.GetSlotType() == ST_ITEM )
		{
			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
			if ( rItemSlot.GetItemInfo()->m_wType == eITEMTYPE_WAYPOINT )
			{
				if ( rItemSlot.IsProgressTime() )// 활성화 되어 있는가?
					return &rItemSlot; // 활성화된 룬스톤이 있다
			}
		}			
	}
	return NULL; // 활성화된 룬스톤이 없다
}

void uiPersonalWayPointMan::NetworkProc( MSG_BASE * pMsg )
{
	switch (pMsg->m_byCategory)
	{
	case CG_WAYPOINT:
		{
		    switch (pMsg->m_byProtocol)
		    {
			case CG_CHAR_WAYPOINT_INFO_LOAD_ACK:
				{
					m_pUIWayPoint->RECV_CG_CHAR_WAYPOINT_INFO_LOAD_ACK(pMsg);
				}
			break;
			case CG_CHAR_WAYPOINT_INFO_SAVE_ACK:
				{
					m_pUIWayPoint->RECV_CG_CHAR_WAYPOINT_INFO_SAVE_ACK(pMsg);
				}
				break;
			case CG_CHAR_WAYPOINT_INFO_DEL_ACK:
				{
					m_pUIWayPoint->RECV_CG_CHAR_WAYPOINT_INFO_DEL_ACK(pMsg);
				}
				break;
			case CG_CHAR_WAYPOINT_INFO_NAK:
				{
					MSG_CG_CHAR_WAYPOINT_INFO_NAK* pRecv = (MSG_CG_CHAR_WAYPOINT_INFO_NAK*)pMsg;
					//DWORD dwErrorCode = pRecv->m_dwErrorCode;
					m_pUIWayPoint->UnSetWaypointInfoPending();
					_processErrorCode( pRecv->m_dwErrorCode );
				}
				break;
			}
		}
	}
}
void uiPersonalWayPointMan::_processErrorCode(DWORD dwErrorCode)
{
	switch (dwErrorCode)
	{
	case RC::RC_WP_CANNOT_LOAD_FROM_DB:				//< DB로부터 정보를 가져올 수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_LOAD_FROM_DB"));
		break;

	case RC::RC_WP_CANNOT_SAVE_FROM_DB:				//< DB로부터 정보를 저장할 수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_SAVE_FROM_DB"));
		break;

	case RC::RC_WP_ALREADY_SAVE_MAPCODE:				//< 이미 저장되어있는 MapCode입니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_ALREADY_SAVE_MAPCODE"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE:	//< 카오상태에서는 이용할 수 없는 기능이다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE"));
		break;

	case RC::RC_WP_CANNOT_DELETE_FROM_DB:			//< DB로부터 정보를 삭제할 수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_DELETE_FROM_DB"));
		break;

	case RC::RC_WP_CANNOT_FIND_WP_IDX:				//< 웨이포인트 인덱스를 찾을수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_FIND_WP_IDX"));
		break;

	case RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM:		//< 웨이포인트 아이템을 찾을수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM"));
		break;

	case RC::RC_WP_UNABLE_DELETE_FOR_DELTIME:		//< 삭제불가시간이 남아 있어 삭제기능을 사용할수 없습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_DELETE_FOR_DELTIME"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT:	//< 저장 가능한 갯수를 넘었다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE:		//< 마을, 필드, 인스던전에서만 사용 가능하다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE"));
		break;

	case RC::RC_WP_NOT_YET_LOAD_FROM_DB:				//< DB로 부터 정보가 LOAD 되어 있지 않다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_NOT_YET_LOAD_FROM_DB"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE:		//< 웨이포인트로 저장할수 없는 위치에 있습니다.   
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE"));
		break;

	case RC::RC_WP_PORTAL_FAIL:						//< 웨이포인트로 이동에 실폐했습니다.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_PORTAL_FAIL"));
		break;

	}
}

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
void uiPersonalWayPointMan::HotKeyTogglePersonalwarpDialog()
{

    if ((g_pMap) &&
        (g_pMap->IsVill() == FALSE) && 
        (g_pMap->IsField() == FALSE))
    {
        return;
    }

    uiPersonalWayPointWindow* pDlg = 
        GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);

    if (pDlg != NULL)
    {
		CastLeaveFieldDialog* cast_dialog = 
			(CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);

		if ( cast_dialog->GetDoType() == eDoType_PersonalWarp )
		{
			return;
		}

		pDlg->ShowInterface(pDlg->IsVisible() == TRUE ? false : true);
    }
}
#endif//#ifdef _NA_000000_20121210_REFACTORY_GAME_USER