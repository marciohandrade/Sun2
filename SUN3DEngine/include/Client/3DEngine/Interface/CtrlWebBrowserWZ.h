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

	//. �⺻ ��� �Լ���
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw) ;
	virtual void			OnDestroy();	
	virtual bool			OnDraw(bool bl);
	virtual void			OnDoAction();

	virtual void			OnLoad(HANDLE hHandle);
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual void			ShowWindowWZ(ENUM_STATEWZ eState);

	//. Web ��� Wrapping �Լ���
	virtual void			GoHome(void);	
	virtual void			Navigate(LPCWSTR lpwszURL);	
	virtual void			Refresh(void);	
	virtual void			Stop(void);	
	virtual void			GoBack(void);	
	virtual void			GoForward(void);
	
public:
	//. �ѹ��� ���� �ϸ� �Ǳ� ������ Static���� ���.
	static void				StaticCreateWebCtrl();
	static void				StaticMoveWebCtrl(long lnx, long lny);
	static void				StaticDestroyWebCtrl();
	static void				StaticShowState(ENUM_STATEWZ eState);

public:
	static VOID*			m_spWebBW;			//. WebBrowser �ڵ�

protected:
	static HWND				m_hWebDlg;			//. WebDialog �ڵ�
	SI_FRECT				m_rtfRealRect;		//. ���� �׷����� ũ��.
};
