#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlRectangleWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlRectangleWZ :public CControlWZ
{
public://. �⺻ �Լ���.
	CCtrlRectangleWZ(void);
	virtual ~CCtrlRectangleWZ(void);

	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual bool			OnDraw(bool bl);

public://. �ܺ� ���� ���� �Լ���.
	virtual void			SetFill(bool blFill){m_blFill = blFill;}
	virtual bool			GetFill(){return m_blFill;}
	virtual void			SetThickness(int nThickness){m_nLineThickness = nThickness;}
	virtual int				GetThickness(){return m_nLineThickness;}
	virtual void			SetLineColor(COLORREF color){m_rgbLineColor = color;}
	virtual void			GetLineColor(COLORREF* pColor){*pColor = m_rgbLineColor;}

protected:
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg){return true;};

protected:
	bool					m_blFill;				//. ���� ä��ų� ���ų�.
	int						m_nLineThickness;		//. ���� ����.
	COLORREF				m_rgbLineColor;			//. ����.	
};
