#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlClientWZ Class
// ��Ʈ�ѵ��� �����ϰ� �����Ѵ�. Ư���� �ϴ� ����� ����.
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlClientWZ : public CControlWZ  
{
public:
	CCtrlClientWZ();
	virtual ~CCtrlClientWZ();	

	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw) ;		
	
	virtual bool			OnDraw(bool bl);	

	//. ����/�ε�
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);
	
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);		

public:	
	DWORD					GetControlCount();			//. ��� ����
};

