#include "StdAfx.h"
#include "SlotInsertResultConvert.h"

RC::eITEM_RESULT SlotInsertResultConverter::ConvertToeItemResult( RC::eSLOT_INSERT_RESULT eSlotRT )
{
	RC::eITEM_RESULT eItemRT;

	switch(eSlotRT)
	{
	case RC::RC_INSERT_SLOT_FAILED:
		eItemRT = RC::RC_ITEM_FAILED;
		break;
	case RC::RC_INSERT_SLOT_SUCCESS:
		eItemRT = RC::RC_ITEM_SUCCESS;
		break;
	case RC::RC_INSERT_SLOT_CAN_NOT_EQUIP:
		eItemRT = RC::RC_ITEM_CAN_NOT_EQUIP;
		break;
	case RC::RC_INSERT_SLOT_UNABLE_GUILDWAREHOUSE_POSSESSION_ITEM:
		eItemRT = RC::RC_ITEM_GUILDWAREHOUSE_UNABLE_POSSESSION_ITEM;
		break;
	//Todo:

	default:
		eItemRT = RC::RC_ITEM_FAILED;
		break;
	}

	return eItemRT;
}

RC::eQUICK_RESULT SlotInsertResultConverter::ConvertToeQuickResult( RC::eSLOT_INSERT_RESULT eSlotRT )
{
	RC::eQUICK_RESULT eQuickResult;
	switch(eSlotRT)
	{
	case RC::RC_INSERT_SLOT_FAILED:
		eQuickResult = RC::RC_QUICK_FAILED;
		break;
	case RC::RC_INSERT_SLOT_SUCCESS:
		eQuickResult = RC::RC_QUICK_SUCCESS;
		break;
	//Todo:

	default:
		eQuickResult = RC::RC_QUICK_FAILED;
		break;
	}

	return eQuickResult;
}

RC::eSTYLE_RESULT SlotInsertResultConverter::ConvertToeStyleResult( RC::eSLOT_INSERT_RESULT eSlotRT )
{
	RC::eSTYLE_RESULT eStyleResult;

	switch(eSlotRT)
	{
	case RC::RC_INSERT_SLOT_FAILED:
		eStyleResult = RC::RC_STYLE_FAILED;
		break;
	case RC::RC_INSERT_SLOT_SUCCESS:
		eStyleResult = RC::RC_STYLE_SUCCESS;
		break;
	//Todo:

	default:
		eStyleResult = RC::RC_STYLE_FAILED;
		break;
	}

	return eStyleResult;
}