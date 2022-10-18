#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan.h"
#include "uiEquipmentAwakeningDialog.h"

const DWORD IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG = StrToWzID("7121");
TCHAR equipment_wakeup_dialog_resource_path[] = _T("Data\\Interface\\71_21_Itam_Awakening.iwz");

uiEquipmentWakeupMan::uiEquipmentWakeupMan(InterfaceManager* ui_manager) : 
uiBaseMan(ui_manager),
ui_equipment_awakening_dialog_(NULL)
{
}

uiEquipmentWakeupMan::~uiEquipmentWakeupMan(void)
{
}

void uiEquipmentWakeupMan::OnInitialize()
{
	ui_equipment_awakening_dialog_ = CreateUIDialog<uiEquipmentAwakeningDialog>(
		IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG,
		equipment_wakeup_dialog_resource_path,
		this,
		TRUE);
}

SolarDialog* uiEquipmentWakeupMan::ShowEquipmentAwakeningDialog()
{
	if ( ui_equipment_awakening_dialog_ )
	{
		ui_equipment_awakening_dialog_->ShowInterface(TRUE);
	}

	return static_cast<SolarDialog*>(ui_equipment_awakening_dialog_);
}

void uiEquipmentWakeupMan::NetworkProc( MSG_BASE* message )
{
	switch ( message->m_byCategory )
	{
	case CG_ITEM:
		{
			switch ( message->m_byProtocol )
			{
			case CG_ITEM_AWAKENING_ACK:
				{
					Recv_CG_ITEM_AWAKENING_ACK( message );
				}
				break;
			}
		}
		break;
	}
}

void uiEquipmentWakeupMan::Recv_CG_ITEM_AWAKENING_ACK( MSG_BASE* message )
{
	MSG_CG_ITEM_AWAKENING_ACK* recv_packet = static_cast<MSG_CG_ITEM_AWAKENING_ACK*>(message);

	if ( ui_equipment_awakening_dialog_ )
	{
		ui_equipment_awakening_dialog_->ResultCodeProc((RC::eITEM_RESULT)recv_packet->result_code);
		ui_equipment_awakening_dialog_->ResultCodeSoundProc((RC::eITEM_RESULT)recv_packet->result_code);
		ui_equipment_awakening_dialog_->ResetAwakeningDialog();
	}
}

#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
