#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlPictureWZ Class
// - 그림만 그려주고 메세지는 처리 안 해준다.
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlPictureWZ : public CControlWZ  
{
    float                   m_CurrentBlendRate;
    DWORD                   m_BlendStartedTime;

    bool                    m_bEnableAutoBlend;
    


    bool                    m_bEnableLButtonEvent;

    bool                    m_bEnableInnerClip;
    float                   m_ClipRateWidth;
    float                   m_ClipRateHeight;

public:
	CCtrlPictureWZ();
	virtual ~CCtrlPictureWZ();
	virtual bool			OnDraw(bool bl);
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg); 

    virtual void            EnableLButtonEvent( bool enable );
    virtual void            EnableInnerClipDraw( bool enable );
    virtual void            EnableBlend( bool enable );

protected:
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

public:
    virtual float GetClipRateWidth() const { return m_ClipRateWidth; }
    virtual void SetClipRateWidth(float value) { m_ClipRateWidth = value; }

    virtual float GetClipRateHeight() const { return m_ClipRateHeight; }
    virtual void SetClipRateHeight(float value) { m_ClipRateHeight = value; }

    
    bool OnDraw_InnerClip();
    void UpdateBlend();
};