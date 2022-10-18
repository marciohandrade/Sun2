#include "SunClientPrecompiledHeader.h"

#include "iconditiontriggerex.h"

ConditionTriggerEx::ConditionTriggerEx(void)
{
	m_pParent = NULL;
	m_pTriggerMGR = NULL;
	m_bAlwaysAction = TRUE;
}

ConditionTriggerEx::~ConditionTriggerEx(void)
{
}

VOID ConditionTriggerEx::Init( TriggerManager* pMGR, Trigger* pParent )
{
	m_pTriggerMGR = pMGR;
	m_pParent = pParent;
}

BOOL ConditionTriggerEx::OnMsg( TRIGGER_MSG * pMsg )
{ 
	onMsg( pMsg );
	return TRUE;
}
