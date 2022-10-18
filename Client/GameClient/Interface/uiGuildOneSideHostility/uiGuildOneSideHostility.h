#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildOneSideHostility : public uiBase
{
public:
	enum eDIALOG_GUILD_ONE_SIDE_HOSTILITY
	{
		eDIALOG_GUILD_ALLIANCE_TAB,
		eDIALOG_GUILD_HOSTILITY_TAB,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB,

		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_RELATION_BTN_FOR_SORT,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_STATE_BTN_FOR_SORT,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_EXIT_BTN,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_SCROLL,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_APPLY_BTN,
		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAIN_LIST,

		eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAX,
	};

	uiGuildOneSideHostility(InterfaceManager * pUIMan );
	~uiGuildOneSideHostility();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

	uiGuildMan*         GetManager();

	void				UpdateToolTip();
	void				RenderTooltip();

private:
	static WzID		m_wzID[eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAX];

	uiGuildMan*             m_pGuildMan;

	CCtrlButtonCheckWZ*  m_pAlianceTab;
	CCtrlButtonCheckWZ*  m_pHostilityTab;
	CCtrlButtonCheckWZ*  m_pOneSideHostilityTab;

	CCtrlButtonWZ*  m_pOKBtn;
	CCtrlButtonWZ*  m_pExitBtn;

	CCtrlButtonWZ*  m_pRelationBtnForSort;
	CCtrlButtonWZ*  m_pStateBtnForSort;

	CCtrlListWZ*	m_pMainList;

	CCtrlVScrollWZ*	m_pScroll;

	BOOL			m_bShow;

	eGUILD_RELATION	m_eRenderToolTip;

	int				m_iPreRelationCnt;
};

inline CControlWZ * uiGuildOneSideHostility::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildOneSideHostility::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_GUILD_ONE_SIDE_HOSTILITY_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildOneSideHostility::GetManager()
{
	return (uiGuildMan*)GetMan();
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
