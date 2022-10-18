#include "SunClientPrecompiledHeader.h"

#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "InterfaceManager.h"
#include "MissionRelay.h"

const char  c_strDlgFname_Mission_Relay[]           = ("Data\\Interface\\50_2_mission_popup_retry.iwz");
const DWORD c_dwCompareID_Mission_Relay             = StrToWzID("0502");

uiMissionRelayMan::uiMissionRelayMan( InterfaceManager * pUIMAN)
: uiBaseMan(pUIMAN)
{

}

void uiMissionRelayMan::OnInitialize()
{
	m_pUIMissionRelay =  CreateUIDialog<MissionRelay>(
		c_dwCompareID_Mission_Relay, 
		c_strDlgFname_Mission_Relay, 
		this);
}