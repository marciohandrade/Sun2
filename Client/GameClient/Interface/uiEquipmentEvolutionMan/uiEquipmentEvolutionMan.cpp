#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan.h"
#include "uiEquipmentEvolutionDialog.h"

const DWORD IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG = StrToWzID("7122");
TCHAR equipment_evolution_dialog_resource_path[] = _T("Data\\Interface\\71_22_Itam_Evolution.iwz");

uiEquipmentEvolutionMan::uiEquipmentEvolutionMan(InterfaceManager* ui_manager) : 
uiBaseMan(ui_manager),
ui_equipment_evolution_dialog_(NULL)
{
}

uiEquipmentEvolutionMan::~uiEquipmentEvolutionMan(void)
{
}

void uiEquipmentEvolutionMan::OnInitialize()
{
	ui_equipment_evolution_dialog_ = CreateUIDialog<uiEquipmentEvolutionDialog>(
		IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG,
		equipment_evolution_dialog_resource_path,
		this,
		TRUE);
}

SolarDialog* uiEquipmentEvolutionMan::ShowEquipmentEvolutionDialog()
{
	if ( ui_equipment_evolution_dialog_ )
	{
		ui_equipment_evolution_dialog_->ShowInterface(TRUE);
	}

	return static_cast<SolarDialog*>(ui_equipment_evolution_dialog_);
}

void uiEquipmentEvolutionMan::NetworkProc( MSG_BASE* message )
{
	switch ( message->m_byCategory )
	{
	case CG_ITEM:
		{
			switch ( message->m_byProtocol )
			{
			case CG_ITEM_EVOLUTION_ACK:
				{
					Recv_CG_ITEM_EVOLUTION_ACK( message );
				}
				break;
			}
		}
		break;
	}
}

void uiEquipmentEvolutionMan::Recv_CG_ITEM_EVOLUTION_ACK( MSG_BASE* message )
{
	MSG_CG_ITEM_EVOLUTION_ACK* recv_packet = static_cast<MSG_CG_ITEM_EVOLUTION_ACK*>(message);

	if ( ui_equipment_evolution_dialog_ )
	{
		ui_equipment_evolution_dialog_->ResultCodeProc((RC::eITEM_RESULT)recv_packet->result_code);
		ui_equipment_evolution_dialog_->ResultCodeSoundProc((RC::eITEM_RESULT)recv_packet->result_code);
		ui_equipment_evolution_dialog_->ResetEvolutionDialog();
	}
}

#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
