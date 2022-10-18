#pragma once

#include <BaseTrigger.h>
#include <ConditionInfo.h>
#include <ActionInfo.h>
#include <IConditionTrigger.h>
#include <IActionTrigger.h>

class Trigger : public BaseTrigger
{
public:
	Trigger();
	~Trigger();

	VOID _Action(BOOL bDone = FALSE);
	VOID OnMsg( TRIGGER_MSG * pMsg, BOOL bDone = FALSE );

protected:

	virtual VOID			_createCondition( ConditionInfo* pTrigger );
	virtual VOID			_createAction( ActionInfo* pTrigger );
	virtual VOID			_destroyCondition( IConditionTrigger * pTrigger );
	virtual VOID			_destroyAction( IActionTrigger * pTrigger );
	virtual VOID			_OnAction();
};