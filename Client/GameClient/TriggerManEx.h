#pragma once

#include "..\\..\\SolarTrigger\\TriggerManager.h"
#include <BaseTrigger.h>
#include <ConditionInfo.h>
#include <ActionInfo.h>


struct sTRIGGER_OBJECT
{
	DWORD m_dwTriggerID;
	DWORD m_dwObjectKey;
	BOOL  m_bLoop;
	BOOL  m_bActive;
};

class TriggerGroupInfo;

class TriggerManEx : public TriggerManager
{
public:
	TriggerManEx(void);
	~TriggerManEx(void);

	VOID		Init( TriggerGroupInfo & IN rInfo);
	VOID		Release();
	VOID		OnMsg( TRIGGER_MSG * pMsg, BOOL bDone = FALSE);

	std::vector<sTRIGGER_OBJECT*> m_vecTrgObj;

protected:	
	virtual BaseTrigger*	AllocTrigger() { return NULL; }
    virtual VOID			FreeTrigger( BaseTrigger* pTrigger ) { SAFE_DELETE(pTrigger); }




};
