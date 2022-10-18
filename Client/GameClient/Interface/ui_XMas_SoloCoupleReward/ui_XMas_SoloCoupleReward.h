#pragma once

#include "uibase.h"

#include "FTextRender.h"
#include "FTextSentence.h"

class ui_XMas_SoloCoupleReward : public uiBase
{
public:
	enum eDIALOG_XMAS_SOLO_COUPLE_REWARD
	{
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_TITLE_TEXT,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAIN_TEXT_LIST,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_SLOT_BTN,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_OK_BTN,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_CANCEL_BTN,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_ITEM_TEXT_LIST,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_EXIT_BTN,
		eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX,
	};

	enum eSOLO_COUPLE_STATE
	{
		eSOLO_COUPLE_STATE_NONE,
		eSOLO_COUPLE_STATE_SOLO,
		eSOLO_COUPLE_STATE_COUPLE,
	};

	ui_XMas_SoloCoupleReward(InterfaceManager * pUIMan );
	~ui_XMas_SoloCoupleReward();
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
	void				LoadItemTextureHandle();

public:
	void				SetSoloCoupleState(eSOLO_COUPLE_STATE eState){m_eSoloCoupleState = eState;}
	eSOLO_COUPLE_STATE  GetSoloCoupleState(){return m_eSoloCoupleState;}

private:
	static WzID		m_wzID[eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX];

	FTextRender		m_FtextRender[4];
	FTextSentence	m_FSentence;
	BOOL m_bTextLoad;

	CCtrlListWZ*    m_ctrlMainList;
	CCtrlListWZ*	m_ctrlItemList;
	CCtrlStaticWZ*  m_ctrlTitleStatic;
	CCtrlButtonWZ*  m_ctrlSlotBtn;

	HANDLE			m_hSlotTexture;

	eSOLO_COUPLE_STATE m_eSoloCoupleState;

	BOOL			m_bShow;
};

inline CControlWZ * ui_XMas_SoloCoupleReward::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_SoloCoupleReward::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX;
}
