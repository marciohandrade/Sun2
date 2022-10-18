#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiChangeGuildName : public uiBase
{
public:
	enum eDIALOG_CHANGE_GUILD_NAME
	{
		eDIALOG_CHANGE_GUILD_NAME_OK_BTN,
		eDIALOG_CHANGE_GUILD_NAME_CANCEL_BTN,
		eDIALOG_CHANGE_GUILD_NAME_EXIT_BTN,
		eDIALOG_CHANGE_GUILD_NAME_CUR_GUILD_NAME,
		eDIALOG_CHANGE_GUILD_NAME_EDIT,
		eDIALOG_CHANGE_GUILD_NAME_MAX,
	};

	uiChangeGuildName(InterfaceManager * pUIMan );
	~uiChangeGuildName();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

	void				RenderSlotTexture();

public:
	void				SetSlotInfo(SLOTIDX SlotIdx, POSTYPE PosType);
	void				SlotLock(bool bLock);

private:

	static WzID		m_wzID[eDIALOG_CHANGE_GUILD_NAME_MAX];

	CCtrlStaticWZ*	   m_pctrlStatic;
	CCtrlDxIMEEditBox* m_pctrlEdit;

	BOOL			m_bShow;

	SLOTIDX			m_SlotIdx;
	POSTYPE			m_PosType;
};

inline CControlWZ * uiChangeGuildName::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_CHANGE_GUILD_NAME_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiChangeGuildName::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_CHANGE_GUILD_NAME_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_CHANGE_GUILD_NAME_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
