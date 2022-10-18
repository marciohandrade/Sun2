#pragma once

#include "uibase.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

//TODO: 사용안하는걸로 합의ㅠ_ㅠ(추후 삭제 예정)
#include "uiGuildMain/uiGuildMain.h"

class uiGuildWareHouseRight : public uiBase
{
public:
	enum eDIALOG_GUILD_WAREHOUSE_RIGHT
	{
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN0,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN1,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN2,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN3,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN4,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN5,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN6,

		eDIALOG_GUILD_WAREHOUSE_RIGHT_EXIT_BTN,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_OK_BTN,
		eDIALOG_GUILD_WAREHOUSE_RIGHT_CANCEL_BTN,

		eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX,
	};

	enum
	{
		eMAX_RIGHT_CHECK_BTN_NUM = eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN6 + 1
	};

	enum
	{
		MAX_GUILD_DUTY_NUM = eGUILD_DUTY_MAX - eGUILD_DUTY_MASTER
	};

	uiGuildWareHouseRight(InterfaceManager * pUIMan );
	~uiGuildWareHouseRight();
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
	void				SetCheckBtn();

	uiGuildMan*			GetManager(){return m_pGuildMan;}

public:
	void				SetCurCheckState(BYTE btCheck){m_btCurCheckState = btCheck;}
	BYTE				GetCurCheckState(){return m_btCurCheckState;}

private:
	uiGuildMan*		m_pGuildMan;
	static WzID		m_wzID[eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX];

	BOOL			m_bShow;

	CCtrlButtonCheckWZ* m_pctrlDutyCheckBtn[eMAX_RIGHT_CHECK_BTN_NUM];
	BYTE				m_btCurCheckState;
};

inline CControlWZ * uiGuildWareHouseRight::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildWareHouseRight::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
