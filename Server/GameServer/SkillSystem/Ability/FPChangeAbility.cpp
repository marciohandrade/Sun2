#include "stdafx.h"
#include "FPChangeAbility.h"
#include "Player.h"

bool FPChangeAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    if (Ability::Execute(target, pMsg, wMsgSize) == false)
    {
        return false;
    }

    if (target == NULL) {
        return false;
    }

    if (target->IsEqualObjectKind(PLAYER_OBJECT) != true) {
        return false;
    }

    Player* player_target = reinterpret_cast<Player*>(target);

    // 변경될 FP값
    PLAYERFP fp_value = (PLAYERFP)(this->GetBaseAbilityInfo()->m_iParam[1]);
    
    player_target->OnRecoverFP(fp_value);

    return true;
}