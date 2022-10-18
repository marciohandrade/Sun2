#pragma once

#include "uiBase.h"
class uiHelmetShow : public uiBase
{
public:
	enum
	{
		eControl_Check_Show_C000,
		eControl_Picture_show_P010,
		eControl_Max,
	};
public:
	uiHelmetShow();
	uiHelmetShow(InterfaceManager* pUIMan);
	~uiHelmetShow();

public:
	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			CheckMessageMouse(bool UpMessage);
	virtual VOID			OnShowWindow(BOOL val);

	void					MoveWindowToEquip(int x, int y);
	void					Render();

public:
    void                    set_check_show_helmet_state(const bool& is_check);
	

private:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	static WzID				m_wzId[eControl_Max];
};
