#include "SunClientPrecompiledHeader.h"

#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "interfacemanager.h"
#include "IDGenerator.h"

const DWORD IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG = StrToWzID("0524");

void uiPersonalWayPointMan::OnInitialize()
{
	m_pUIWayPoint = CreateUIDialog<uiPersonalWayPointWindow>(
						IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG, 
						"Data\\Interface\\52_4_Way_Point.iwz",
						this);
}

