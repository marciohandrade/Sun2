#include "SunClientPrecompiledHeader.h"

#ifdef _DH_DISPLAY_FOR_MISSION_OBJECT

#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "InterfaceManager.h"
#include "uiMissionObject.h"

const char  dialog_name_mission_object[] = ("Data\\Interface\\50_4_mission_info.iwz");
const DWORD compare_id_mission_object = StrToWzID("0504");

uiMissionObjectMan::uiMissionObjectMan(InterfaceManager* ui_manager) : uiBaseMan(ui_manager)
{

}


uiMissionObjectMan::~uiMissionObjectMan(void)
{
	this->Release();
}


void  uiMissionObjectMan::Init(CDrawBase* pDrawBase)
{
	uiBaseMan::Init(pDrawBase);

	LoadUI(UI_MISSION_OBJECT);
}

void uiMissionObjectMan::Release()
{
}


void uiMissionObjectMan::LoadUI(UINT ui_id)
{
	ui_mission_object_ = CreateUIDialog<uiMissionObject>(IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT, dialog_name_mission_object, this, TRUE);
}
#endif//_DH_DISPLAY_FOR_MISSION_OBJECT