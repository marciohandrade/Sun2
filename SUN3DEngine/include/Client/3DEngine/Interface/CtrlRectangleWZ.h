#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlRectangleWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlRectangleWZ :public CControlWZ
{
public://. 기본 함수들.
	CCtrlRectangleWZ(void);
	virtual ~CCtrlRectangleWZ(void);

	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual bool			OnDraw(bool bl);

public://. 외부 접근 가능 함수들.
	virtual void			SetFill(bool blFill){m_blFill = blFill;}
	virtual bool			GetFill(){return m_blFill;}
	virtual void			SetThickness(int nThickness){m_nLineThickness = nThickness;}
	virtual int				GetThickness(){return m_nLineThickness;}
	virtual void			SetLineColor(COLORREF color){m_rgbLineColor = color;}
	virtual void			GetLineColor(COLORREF* pColor){*pColor = m_rgbLineColor;}

protected:
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg){return true;};

protected:
	bool					m_blFill;				//. 안을 채울거냐 말거냐.
	int						m_nLineThickness;		//. 선의 굵기.
	COLORREF				m_rgbLineColor;			//. 색상.	
};
