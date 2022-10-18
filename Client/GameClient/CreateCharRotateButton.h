#pragma once
#include "uiBase.h"

class InterfaceManager;

class CreateCharRotateButton : public uiBase
{
public:
	enum ROTATE_BUTTON_TYPE
	{
		NONE_BUTTON = -1,
		LEFT_BUTTON = 0,
		RIGHT_BUTTON,
		ZOOM_IN_BUTTON,
		ZOOM_OUT_BUTTON,
		MAX_BUTTON
	};

	CreateCharRotateButton(InterfaceManager * pUIman);
	~CreateCharRotateButton();

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnUpdateSolarDialog();

	ROTATE_BUTTON_TYPE GetRotateType() { return m_RotateType; }

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	void			SetZoomButton( BOOL bShow = FALSE );
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

private:
	ROTATE_BUTTON_TYPE	m_RotateType;
	static	WzID	    m_wzId[MAX_BUTTON];
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	CControlWZ*         pZoomInBtn;
	CControlWZ*         pZoomOutBtn;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};