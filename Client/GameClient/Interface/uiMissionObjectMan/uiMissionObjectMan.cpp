#include "SunClientPrecompiledHeader.h"


#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "InterfaceManager.h"
#include "uiMissionObject/uiMissionObject.h"

const char  dialog_name_mission_object[] = ("Data\\Interface\\50_4_mission_info.iwz");
const DWORD compare_id_mission_object = StrToWzID("0504");

uiMissionObjectMan::uiMissionObjectMan(InterfaceManager* ui_manager) : uiBaseMan(ui_manager)
{
    ui_mission_object_ = NULL;
}

void uiMissionObjectMan::OnInitialize()
{
	//�̼� ���� ����̶� ����� ���ļ� ���� ��û ����. 
	//������ü�� ���� �ʴ´�.
	//ui_mission_object_ = CreateUIDialog<uiMissionObject>(IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT, dialog_name_mission_object, this, TRUE);
}
