#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlClientWZ Class
// 컨트롤들을 관리하고 제어한다. 특별히 하는 기능은 없음.
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlClientWZ : public CControlWZ  
{
public:
	CCtrlClientWZ();
	virtual ~CCtrlClientWZ();	

	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw) ;		
	
	virtual bool			OnDraw(bool bl);	

	//. 저장/로드
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);
	
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);		

public:	
	DWORD					GetControlCount();			//. 사용 안함
};

