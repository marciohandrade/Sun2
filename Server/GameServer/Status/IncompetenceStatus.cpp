#include "stdafx.h"

#include ".\IncompetenceStatus.h"
#include "AIParamParser.h"

void IncompetenceStatus::Start()
{
	if (GetAttacker() == NULL)
    {
        return;
    }
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    
	if (owner->IsEqualObjectKind(PLAYER_OBJECT) == false)
    {
        owner->ChangeState( STATE_ID_IDLE);
    }
		
	AbilityStatus::Start();
}