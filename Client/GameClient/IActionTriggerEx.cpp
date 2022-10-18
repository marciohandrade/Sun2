#include "SunClientPrecompiledHeader.h"

#include "iactiontriggerex.h"
#include <TriggerManager.h>
#include "Trigger.h"

ActionTriggerEx::ActionTriggerEx(void)
{
	m_pParent = NULL;
	m_pTriggerMGR = NULL;
	m_bDone       = FALSE;
}

ActionTriggerEx::~ActionTriggerEx(void)
{
}

VOID ActionTriggerEx::Init( TriggerManager* pMGR, Trigger* pParent )
{
	m_pTriggerMGR = pMGR;
	m_pParent = pParent;

	IActionTrigger::Init();

}
