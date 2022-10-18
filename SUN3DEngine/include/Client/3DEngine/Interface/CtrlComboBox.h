#pragma once

#include "ControlWZ.h"

//--------------------------------------------------------------
// CCtrlComboBoxWZ Class
//--------------------------------------------------------------
class CCtrlComboBoxWZ : public CControlWZ  
{
public:
    CCtrlComboBoxWZ();
    virtual ~CCtrlComboBoxWZ();

    virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);	
    virtual bool			OnDraw(bool bl);

    //. 저장/일기 관련 함수들
    virtual void			OnLoad(HANDLE hHandle);
    virtual void			OnSave(HANDLE hHandle);

};
