#pragma once

#include "solardialog.h"

class uiChangeRoomName : public SolarDialog
{
public:
	enum eDIALOG_CHANGE_ROOM_NAME
	{
		eDIALOG_CHANGE_ROOM_NAME_OK_BTN,
		eDIALOG_CHANGE_ROOM_NAME_CANCEL_BTN,
		eDIALOG_CHANGE_ROOM_NAME_EDIT,
		eDIALOG_CHANGE_ROOM_NAME_MAX,
	};

	uiChangeRoomName();
	~uiChangeRoomName();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);
	virtual void		NetworkProc( MSG_BASE * pMsg );

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

	void				OnOKBtnDown( void );

private:
	static WzID		m_wzID[eDIALOG_CHANGE_ROOM_NAME_MAX];

	CCtrlButtonWZ* m_pOkBtn;
	CCtrlButtonWZ* m_pCancelBtn;
	CCtrlDxIMEEditBox*   m_pEdit;

	TCHAR			m_szRoomTitle[MAX_ROOMTITLE_LENGTH];

	BOOL			m_bShow;

	DWORD	m_dwTextColor;
	DWORD	m_dwWideTextColor;
	DWORD	m_dwSelectColor;
	DWORD	m_dwBGColor;
	DWORD	m_dwSetColorSelBarOnText;

public:
	void GetRoomTitle(TCHAR* szMessage);
};

inline void uiChangeRoomName::GetRoomTitle(TCHAR* szMessage)
{
	if('\0' != m_szRoomTitle[0])
		StrnCopy(szMessage, m_szRoomTitle, MAX_ROOMTITLE_LENGTH - 1);
}

inline CControlWZ * uiChangeRoomName::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_CHANGE_ROOM_NAME_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiChangeRoomName::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_CHANGE_ROOM_NAME_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_CHANGE_ROOM_NAME_MAX;
}
