#pragma once
#include "uiBaseMan.h"

class CastLeaveFieldDialog;
class CastDialog;
class uiCastMan :
	public uiBaseMan
{
public:
	uiCastMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();

private:
	CastLeaveFieldDialog *		m_pCastLeaveField;
	CastDialog *				m_pCastDlg;	
};
