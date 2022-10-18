#pragma once
#include "AskDialog.h"
#include "InterfaceManager.h"
#include "uiAssistMan/uiAssistMan.h"
#include "FTextRender.h"


class AskSSQObserverDialog :	public AskDialog
{
public:
	AskSSQObserverDialog(InterfaceManager *pUIMan);
	virtual ~AskSSQObserverDialog(void);

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void MessageProc( SI_MESSAGE * pMessage );

protected:
	virtual void OnShowWindow( BOOL val );
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

private:

	FTextRender m_FtextRender;
};

