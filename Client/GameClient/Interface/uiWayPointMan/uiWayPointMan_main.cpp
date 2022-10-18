#include "SunClientPrecompiledHeader.h"

#include "uiWayPointMan.h"
#include "uiWayPointWindow/uiWayPointWindow.h"

#include "interfacemanager.h"
#include "MapInfoParser.h"
#include "GlobalFunc.h"
#include "map.h"
#include "ResultCode.h"
using namespace RC;

//------------------------------------------------------------------------------
/**
*/
uiWayPointMan::uiWayPointMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIWayPoint = NULL;

    Clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::Clear()
{

    m_vecCategory.clear();
    m_mapWayPoint.clear();

    ZeroMemory(&m_waypointInfo, sizeof(m_waypointInfo));

    m_bSendPacketToSave = false;
    m_bSendPacketToLoad = false;
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::ShowWayPointWindow(bool bShow)
{
	if(m_pUIWayPoint)
	{
		m_pUIWayPoint->ShowInterface(bShow);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::SetWayPointbyMapCode(MAPCODE code)
{
	if(!g_pMap)
		return;

	
	if(g_pMap->GetCurrentFieldID() == code)
	{
		// 저장된 웨이포인트가 현재필드 id와 같으면, 웨이포인트 인덱스가 없으므로 다른 방법으로 저장
		stWayPointData data;
		ZeroMemory(&data, sizeof(data));

		const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( code );
		if(pMapInfo)
		{
			data.m_dwCategory = pMapInfo->dwDCode;
			data.m_dwNCode = pMapInfo->dwNCode;
			data.m_EnterencePortal.wFieldTo = code;

			_addCategory(data.m_dwCategory);
			m_mapWayPoint.insert(WAYPOINT_PAIR(data.m_dwCategory, data));

		}
	}
	else
	{
		// 서버로 부터 받아온 맵코드를 이용해 웨이포인트 인덱스를 얻어오자
		WORD wIndex = MapInfoParser::Instance()->GetWayPointIndex(g_pMap->GetCurrentFieldID(), code);
		SetWayPoint(wIndex);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::SetWayPoint(WORD wIndex)
{
	// 웨이포인트 세팅
	sMAP_ENTERENCE_PORTAL* pPortal =
            MapInfoParser::Instance()->GetMapPortal( wIndex );

	
	if(pPortal && g_pMap)
	{
		if(g_pMap->GetCurrentFieldID() == pPortal->wFieldFrom)	// 현재필드에서 갈 수있는 웨이포인트만 저장
		{
			stWayPointData data;
			ZeroMemory(&data, sizeof(data));
			CopyMemory(&(data.m_EnterencePortal), pPortal, sizeof(data.m_EnterencePortal));

			const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( data.m_EnterencePortal.wFieldTo );

			if(pMapInfo)
			{
				data.m_dwCategory = pMapInfo->dwDCode;
				data.m_dwNCode = pMapInfo->dwNCode;

				_addCategory(data.m_dwCategory);
				m_mapWayPoint.insert(WAYPOINT_PAIR(data.m_dwCategory, data));

#ifdef _DEBUG
				TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(data.m_dwCategory, szTemp, INTERFACE_STRING_LENGTH);
				g_InterfaceManager.GetInterfaceString(data.m_dwNCode, szTemp2, INTERFACE_STRING_LENGTH);
				Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("[%s] : [%s]"), szTemp, szTemp2);
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, szDebug);
#endif
			}
		}
	}
	
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::_addCategory(DWORD dwCode)
{	
	for(unsigned int i=0;i<m_vecCategory.size(); ++i)
	{
		if(m_vecCategory[i] == dwCode)
		{
			return;
		}
	}

	m_vecCategory.push_back(dwCode);
}

//------------------------------------------------------------------------------
/**
*/
bool uiWayPointMan::_isSaveWayPoint()
{
	if(!g_pMap)
		return false;

	for(int i = 0; i < m_waypointInfo.m_Count; ++i)
	{
		if(m_waypointInfo.m_Slot[i] == g_pMap->GetCurrentFieldID())
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::SEND_CG_WAYPOINT_INFO_LOAD_SYN()
{
	if(!m_bSendPacketToLoad)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("send CG_WAYPOINT_INFO_LOAD_SYN"));
		MSG_CG_WAYPOINT_INFO_LOAD_SYN sync;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

		m_bSendPacketToLoad = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::SEND_CG_WAYPOINT_INFO_SAVE_SYN(DWORD dwTargetID)
{
	if(!dwTargetID)
		return;

	m_dwPendingWayPointID = dwTargetID;

	if(_isSaveWayPoint())
	{
		// 이미 저장된 웨이포인트면 save하지 않고 바로 로드 요청
		SEND_CG_WAYPOINT_INFO_LOAD_SYN();
		return;
	}
	

	if(!m_bSendPacketToSave)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("send CG_WAYPOINT_INFO_SAVE_SYN"));
		MSG_CG_WAYPOINT_INFO_SAVE_SYN sync;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

		m_bSendPacketToSave = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_WAYPOINT:
        {
            switch (pMsg->m_byProtocol)
            {
			case CG_WAYPOINT_INFO_LOAD_ACK:
				{
					GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("CG_WAYPOINT_INFO_LOAD_ACK"));

					m_bSendPacketToLoad = false;
					ZeroMemory(&m_waypointInfo, sizeof(m_waypointInfo));
					m_vecCategory.clear();
					m_mapWayPoint.clear();

					MSG_CG_WAYPOINT_INFO_LOAD_ACK *pRecvPacket = (MSG_CG_WAYPOINT_INFO_LOAD_ACK *)pMsg;
					for(int i = 0; i < pRecvPacket->m_Info.m_Count; ++i)
					{
						m_waypointInfo.m_Count = pRecvPacket->m_Info.m_Count;
						m_waypointInfo.m_Slot[i] = pRecvPacket->m_Info.m_Slot[i];
						SetWayPointbyMapCode(m_waypointInfo.m_Slot[i]);
					}
					
					if(m_pUIWayPoint && m_pUIWayPoint->IsVisible())
					{
						m_pUIWayPoint->RefreshList();
					}
					else
					{
						ShowWayPointWindow(true);
					}
				}
				break;

			case CG_WAYPOINT_INFO_LOAD_NAK:
				{
					MSG_CG_WAYPOINT_INFO_LOAD_NAK *pRecvPacket = (MSG_CG_WAYPOINT_INFO_LOAD_NAK *)pMsg;
					_processErrorCode(pRecvPacket->m_dwErrorCode);

					m_bSendPacketToLoad = false;
					ZeroMemory(&m_waypointInfo, sizeof(m_waypointInfo));
					m_vecCategory.clear();
					m_mapWayPoint.clear();
				}
				break;

			case CG_WAYPOINT_INFO_SAVE_ACK:
				{
					m_bSendPacketToSave = false;

					//6001	웨이포인트가 저장되었습니다.
					g_InterfaceManager.GetInterfaceString(6001, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					SEND_CG_WAYPOINT_INFO_LOAD_SYN();
				}	
				break;

			case CG_WAYPOINT_INFO_SAVE_NAK:
				{
					MSG_CG_WAYPOINT_INFO_SAVE_NAK *pRecvPacket = (MSG_CG_WAYPOINT_INFO_SAVE_NAK *)pMsg;
					_processErrorCode(pRecvPacket->m_dwErrorCode);

					m_bSendPacketToSave = false;

					if(RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE != pRecvPacket->m_dwErrorCode)	// 카오상태라서 이용할 수 없을때는 웨이포인트정보를 요청하지말자
					{
						SEND_CG_WAYPOINT_INFO_LOAD_SYN();
					}
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
void uiWayPointMan::_processErrorCode(DWORD dwErrorCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	switch(dwErrorCode)
	{
	case RC_WP_FAILED:
		{
			if(m_bSendPacketToLoad)
			{
				//웨이포인트 리스트를 가져올 수 없습니다.
				g_InterfaceManager.GetInterfaceString( 6005, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				
			}
			else if(m_bSendPacketToSave)
			{
				//웨이포인트를 저장할 수 없습니다.
				g_InterfaceManager.GetInterfaceString( 6006, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				
			}
			
		}
		break;

	case RC_WP_CANNOT_LOAD_FROM_DB:			//< DB로부터 정보를 가져올 수 없습니다.
		{
			//웨이포인트 리스트를 가져올 수 없습니다
			g_InterfaceManager.GetInterfaceString( 6005, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			
		}
		break;


	case RC_WP_CANNOT_SAVE_FROM_DB:			//< DB로부터 정보를 저장할 수 없습니다.
		{
			//웨이포인트를 저장할 수 없습니다
			g_InterfaceManager.GetInterfaceString( 6006, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			
		}
		break;

	case RC_WP_ALREADY_SAVE_MAPCODE:			//< 이미 저장되어있는 MapCode입니다.
		{
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("RC_WP_ALREADY_SAVE_MAPCODE"));
		}
		break;

	case RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			// 카오 상태에서는 이용이 가능하지 않습니다. 5375
			g_InterfaceManager.GetInterfaceString(eST_CHAO_IS_INAVALIBLE_STATUS , szMessage , INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	default:
		{
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("Waypoint ErrMsg [%d]"), dwErrorCode);
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;
	}

}


//------------------------------------------------------------------------------
/**
*/
void uiWayPointMan::RqstWayPointSave(DWORD dwTargetID)
{
	if(!g_pMap)
		return;

	ZeroMemory(&m_waypointInfo, sizeof(m_waypointInfo));
	m_vecCategory.clear();
	m_mapWayPoint.clear();

	sMAPPORTAL_HASH * pMapPortalHash =  MapInfoParser::Instance()->GetMapPortalHashTable();
	sMAPPORTAL_HASH::const_iterator itr = pMapPortalHash->begin();


	while( itr != pMapPortalHash->end() )
	{
		sMAP_ENTERENCE_PORTAL *pPotal = itr->second;// (*itr);

		if(pPotal && (pPotal->bMoveType == ePORTAL_MOVE_WAYPOINT) && (g_pMap->GetCurrentFieldID() == pPotal->wFieldFrom))
			SetWayPointbyMapCode(pPotal->wFieldTo);
		
		itr++;
	}

	if(m_pUIWayPoint && m_pUIWayPoint->IsVisible())
	{
		m_pUIWayPoint->RefreshList();
	}
	else
	{
		m_dwPendingWayPointID = dwTargetID;
		ShowWayPointWindow(true);
	}
	
}


