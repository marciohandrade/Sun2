#pragma once

#include "uibase.h"
#include "SCItemSlotContainer.h"

typedef struct _REWARD_SLOT
{
	DWORD dwRewardItemCode;
	DWORD dwRewardItemNum;
}
REWARD_SLOT;

class ui_XMas_SantaReward : public uiBase
{
public:
	enum eDIALOG_XMAS_SANTA_REWARD
	{
		eDIALOG_XMAS_SANTA_REWARD_MAIN_TEXT_LIST,
		eDIALOG_XMAS_SANTA_REWARD_SLOT_BTN,
		eDIALOG_XMAS_SANTA_REWARD_OK_BTN,
		eDIALOG_XMAS_SANTA_REWARD_EXIT_BTN,
		eDIALOG_XMAS_SANTA_REWARD_MAX,
	};

	enum eSLOT_STATE
	{
		eSLOT_STATE_NONE,
		eSLOT_STATE_SOUCE,
		eSLOT_STATE_DEST,
	};

	ui_XMas_SantaReward(InterfaceManager * pUIMan );
	~ui_XMas_SantaReward();
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
	void				AddFormatText();
	void				RenderSlotTexture();
	void				UpdateToolTip();
	void				RenderTooltip();

	void				RenderRewardOverlap();

public:
	void				SetSlotState(eSLOT_STATE eState){m_eSlotState = eState;}

	void				SetRewardInfo(INVENTORY_TOTAL_INFO & rTotalInfo);

private:
	static WzID		m_wzID[eDIALOG_XMAS_SANTA_REWARD_MAX];

	CCtrlListWZ*	m_ctrlMainList;
	CCtrlButtonWZ*  m_ctrlSlotBtn;
	CCtrlButtonWZ*  m_ctrlOKBtn;

	BOOL			m_bOnSlot;
	HANDLE			m_hSlotTexture;
	eSLOT_STATE		m_eSlotState;
	BOOL			m_bRenderToolTip;
	REWARD_SLOT     m_SlotInfo;
	BOOL			m_bShow;
};

inline CControlWZ * ui_XMas_SantaReward::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_SANTA_REWARD_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_SantaReward::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_SANTA_REWARD_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_SANTA_REWARD_MAX;
}
