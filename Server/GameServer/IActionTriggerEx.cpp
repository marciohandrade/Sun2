#include "StdAfx.h"

#include ".\iactiontriggerex.h"
#include "TriggerManagerEx.h"
#include "Trigger.h"

IActionTriggerEx::IActionTriggerEx(void)
{
	m_pParent = NULL;
	m_pTriggerMGR = NULL;
}

IActionTriggerEx::~IActionTriggerEx(void)
{
}

VOID IActionTriggerEx::Init( TriggerManager* pMGR, Trigger* pParent )
{
	m_pTriggerMGR = pMGR;
	m_pParent = pParent;

	IActionTrigger::Init();

}

