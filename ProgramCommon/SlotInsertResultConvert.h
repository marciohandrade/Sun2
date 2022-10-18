#pragma once

#include "ResultCode.h"

class SlotInsertResultConverter
{
	SlotInsertResultConverter();
	~SlotInsertResultConverter();

public:
	static RC::eITEM_RESULT ConvertToeItemResult( RC::eSLOT_INSERT_RESULT eSlotRT );
	static RC::eQUICK_RESULT ConvertToeQuickResult( RC::eSLOT_INSERT_RESULT eSlotRT );
	static RC::eSTYLE_RESULT ConvertToeStyleResult( RC::eSLOT_INSERT_RESULT eSlotRT );
};