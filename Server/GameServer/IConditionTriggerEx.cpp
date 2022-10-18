#include "StdAfx.h"

#include ".\iconditiontriggerex.h"

IConditionTriggerEx::IConditionTriggerEx(void)
{
	m_pParent = NULL;
	m_pTriggerMGR = NULL;
}

IConditionTriggerEx::~IConditionTriggerEx(void)
{
}

VOID IConditionTriggerEx::Init( TriggerManager* pMGR, Trigger* pParent )
{
	m_pTriggerMGR = pMGR;
	m_pParent = pParent;
}

