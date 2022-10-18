#include "SunClientPrecompiledHeader.h"

#include "uiWayPointMan.h"
#include "uiWayPointWindow/uiWayPointWindow.h"
#include "interfacemanager.h"

const DWORD IM_WAYPOINT_MANAGER::WAYPOINT = StrToWzID("0521");
BOOL uiWayPointMan::_LoadUI()
{
	m_pUIWayPoint = CreateUIDialog<uiWayPointWindow>(
						IM_WAYPOINT_MANAGER::WAYPOINT, 
						"Data\\Interface\\52_1_Way_Point.iwz",
						this);
	assert(m_pUIWayPoint);

	return TRUE;
}
