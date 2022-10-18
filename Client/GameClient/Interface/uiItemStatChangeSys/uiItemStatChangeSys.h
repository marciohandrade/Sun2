#pragma once

#include "uibase.h"

class uiItemStatChangeSys : public uiBase
{
public:
	enum eDIALOG_ITEM_STAT_CHANGE_SYS
	{
		eDIALOG_ITEM_STAT_CHANGE_SYS_OK_BTN,
		eDIALOG_ITEM_STAT_CHANGE_SYS_CANCEL_BTN,
		eDIALOG_ITEM_STAT_CHANGE_SYS_EXIT_BTN,
		eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_1,
		eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_2,
		eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_3,
		eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_4,
		eDIALOG_ITEM_STAT_CHANGE_MAX,
	};

	uiItemStatChangeSys(InterfaceManager * pUIMan );
	~uiItemStatChangeSys();
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

	void				RenderSlotTexture();

public:
	void				SetSlotInfo(int iCnt, DWORD* SlotInfo, POSTYPE atItemPos, POSTYPE ChangeItemPos);

private:
	
	DWORD			m_dwSelItemCode;
	POSTYPE			m_ItemPosInInven; //스탯 변경 시켜주는 아이템
	POSTYPE			m_ChangedPosItem;//변경될 아이템
	DWORD			m_dwItemCode[MAX_CHANGE_ITEMSTAT_SLOT_NUM];
	HANDLE			m_hTexHandle;
	static WzID		m_wzID[eDIALOG_ITEM_STAT_CHANGE_MAX];

	CCtrlButtonCheckWZ* m_pSlotChkBtn[MAX_CHANGE_ITEMSTAT_SLOT_NUM];

	BOOL			m_bShow;

	BOOL			m_bSendPacket;
};

inline CControlWZ * uiItemStatChangeSys::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_ITEM_STAT_CHANGE_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiItemStatChangeSys::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_ITEM_STAT_CHANGE_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_ITEM_STAT_CHANGE_MAX;
}
