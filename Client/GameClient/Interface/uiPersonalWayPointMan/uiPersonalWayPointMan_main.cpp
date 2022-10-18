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
		// ��� ���� ���� üũ
		if ( !m_pUIWayPoint->IsRecvWaypointInfo() )
		{
			if ( m_pUIWayPoint->IsWaypointInfoPending() == false )
			{
				m_pUIWayPoint->SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();
				GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("���� ��������Ʈ ���� ��û ��Ŷ ����"));
			}
			else
			{
				GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("���� ��������Ʈ ���� ��û �����"));
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
				if ( rItemSlot.IsProgressTime() )// Ȱ��ȭ �Ǿ� �ִ°�?
					return &rItemSlot; // Ȱ��ȭ�� �齺���� �ִ�
			}
		}			
	}
	return NULL; // Ȱ��ȭ�� �齺���� ����
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
	case RC::RC_WP_CANNOT_LOAD_FROM_DB:				//< DB�κ��� ������ ������ �� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_LOAD_FROM_DB"));
		break;

	case RC::RC_WP_CANNOT_SAVE_FROM_DB:				//< DB�κ��� ������ ������ �� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_SAVE_FROM_DB"));
		break;

	case RC::RC_WP_ALREADY_SAVE_MAPCODE:				//< �̹� ����Ǿ��ִ� MapCode�Դϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_ALREADY_SAVE_MAPCODE"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE:	//< ī�����¿����� �̿��� �� ���� ����̴�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE"));
		break;

	case RC::RC_WP_CANNOT_DELETE_FROM_DB:			//< DB�κ��� ������ ������ �� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_DELETE_FROM_DB"));
		break;

	case RC::RC_WP_CANNOT_FIND_WP_IDX:				//< ��������Ʈ �ε����� ã���� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_FIND_WP_IDX"));
		break;

	case RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM:		//< ��������Ʈ �������� ã���� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM"));
		break;

	case RC::RC_WP_UNABLE_DELETE_FOR_DELTIME:		//< �����Ұ��ð��� ���� �־� ��������� ����Ҽ� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_DELETE_FOR_DELTIME"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT:	//< ���� ������ ������ �Ѿ���.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE:		//< ����, �ʵ�, �ν����������� ��� �����ϴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE"));
		break;

	case RC::RC_WP_NOT_YET_LOAD_FROM_DB:				//< DB�� ���� ������ LOAD �Ǿ� ���� �ʴ�.
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_NOT_YET_LOAD_FROM_DB"));
		break;

	case RC::RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE:		//< ��������Ʈ�� �����Ҽ� ���� ��ġ�� �ֽ��ϴ�.   
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("RC::RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE"));
		break;

	case RC::RC_WP_PORTAL_FAIL:						//< ��������Ʈ�� �̵��� �����߽��ϴ�.
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