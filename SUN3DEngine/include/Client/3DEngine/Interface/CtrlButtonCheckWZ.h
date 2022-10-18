#pragma once

#include "CtrlButtonWZ.h"


//--------------------------------------------------------------
// CCtrlButtonCheckWZ Class 
//--------------------------------------------------------------
class CCtrlButtonCheckWZ : public CCtrlButtonWZ  
{
public:
    CCtrlButtonCheckWZ();
    virtual ~CCtrlButtonCheckWZ();

    ENUM_DRAWSTATE GetDrawState(); // override

    virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);
    virtual bool			OnDraw(bool bl);
    virtual void			OnInitialize();

public:	
    virtual void			SetCheckState(bool blCheck);		//. Check ���� 
    virtual bool			GetCheckState();

protected:
    virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

protected:
    bool					m_blChecking;						 //. üũ ��ư �̱� ������ ���� üŷ ���̴�?
};