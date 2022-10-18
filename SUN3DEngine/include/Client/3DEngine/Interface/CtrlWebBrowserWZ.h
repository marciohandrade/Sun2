#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlWebBrowserWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlWebBrowserWZ :  public CControlWZ
{
public:
	CCtrlWebBrowserWZ();
	virtual ~CCtrlWebBrowserWZ();

	//. 기본 상속 함수들
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw) ;
	virtual void			OnDestroy();	
	virtual bool			OnDraw(bool bl);
	virtual void			OnDoAction();

	virtual void			OnLoad(HANDLE hHandle);
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual void			ShowWindowWZ(ENUM_STATEWZ eState);

	//. Web 기능 Wrapping 함수들
	virtual void			GoHome(void);	
	virtual void			Navigate(LPCWSTR lpwszURL);	
	virtual void			Refresh(void);	
	virtual void			Stop(void);	
	virtual void			GoBack(void);	
	virtual void			GoForward(void);
	
public:
	//. 한번만 셋팅 하면 되기 때문에 Static으로 사용.
	static void				StaticCreateWebCtrl();
	static void				StaticMoveWebCtrl(long lnx, long lny);
	static void				StaticDestroyWebCtrl();
	static void				StaticShowState(ENUM_STATEWZ eState);

public:
	static VOID*			m_spWebBW;			//. WebBrowser 핸들

protected:
	static HWND				m_hWebDlg;			//. WebDialog 핸들
	SI_FRECT				m_rtfRealRect;		//. 실제 그려지는 크기.
};
