#include "stdafx.h"
#include "./IConditionTrigger.h"

bool IConditionTrigger::OnMsg(TRIGGER_MSG* trigger_msg)
{ 
    if (satisfied_) {
        return false;
    };
    onMsg(trigger_msg);

    if (satisfied_)
    {
        // m_bSatisfied값이 변경되었다.
        return true;
    }

    return false;
}