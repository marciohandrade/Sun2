#pragma once

#include "uibase.h"

#include "FTextRender.h"
#include "FTextSentence.h"

class ui_XMas_DonationReward : public uiBase
{
public:
	enum eDIALOG_XMAS_DONATION_REWARD
	{
		eDIALOG_XMAS_DONATION_REWARD_MAIN_TEXT_LIST,
		eDIALOG_XMAS_DONATION_REWARD_SLOT_BTN,
		eDIALOG_XMAS_DONATION_REWARD_OK_BTN,
		eDIALOG_XMAS_DONATION_REWARD_ITEM_TEXT_LIST,
		eDIALOG_XMAS_DONATION_REWARD_EXIT_BTN,
		eDIALOG_XMAS_DONATION_REWARD_MAX,
	};

	ui_XMas_DonationReward(InterfaceManager * pUIMan );
	~ui_XMas_DonationReward();
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
	void				ShowNpcEffect();

public:
	void				SetItemCode(SLOTCODE wSlotCode){m_ItemCode = wSlotCode;}

private:
	static WzID		m_wzID[eDIALOG_XMAS_DONATION_REWARD_MAX];

	FTextRender		m_FtextRender[3];
	FTextSentence	m_FSentence;
	BOOL m_bTextLoad;

	CCtrlListWZ*    m_ctrlMainList;
	CCtrlListWZ*	m_ctrlItemList;
	CCtrlButtonWZ*  m_ctrlSlotBtn;

	HANDLE			m_hSlotTexture;
	SLOTCODE		m_ItemCode;

	BOOL			m_bShow;
};

inline CControlWZ * ui_XMas_DonationReward::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_DONATION_REWARD_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_DonationReward::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_DONATION_REWARD_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_DONATION_REWARD_MAX;
}
