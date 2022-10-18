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
        // m_bSatisfied���� ����Ǿ���.
        return true;
    }

    return false;
}