#include "stdafx.h"
#include "Object.h"
#include "GameField.h"

//==================================================================================================

void Object::OnEnterField(GameField& IN game_field, const WzVector& IN pos, int angle)
{
    SetField(&game_field);
    SetPos(&pos);

    SECTOR_IDX sector_index = game_field.GetSectorIndex(&pos);

    object_node_->sector_index_current = sector_index;
}

void Object::OnLeaveField()
{
    SetField(NULL);

    object_node_->sector_index_current = INVALID_SECTOR_INDEX;
    object_node_->sector_index_previous = INVALID_SECTOR_INDEX;
}