#include "uibase.h"
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_

#define MAX_VIEW_LIST_ROW_COUNT	9

struct CLIENT_EXCHANG_ITEM_INFO
{
	int itemCode;
	int itemNum;
};

class uiGuildEventDialog : public uiBase
{
	enum eDIALOG_POS
    {
		DLGPOS_BTN_OK,
		DLGPOS_BTN_EXIT,
		DLGPOS_LIST,
		DLGPOS_VSCROLL,
		DLGPOS_TEXT_LIST,
		
		DLGPOS_TEXT_BUTTON0,
		DLGPOS_TEXT_BUTTON1,
		

		DLGPOS_MAX,
    };

public:
		uiGuildEventDialog(InterfaceManager * pUIMan );
		~uiGuildEventDialog();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		void				NET_SEND_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN();

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);
		
		void				RegisterItemList();
private:
		static WzID	m_wzID[DLGPOS_MAX];
		CCtrlButtonWZ *		m_pButton;
		CCtrlListWZ *		m_pList;
		CCtrlVScrollWZ *	m_pVScroll;
		CLIENT_EXCHANG_ITEM_INFO	m_ClientExchangInfo;
		int					m_iCurScrollValue;
		int					m_iMaxScrollValue;
		BOOL				m_bShow;
};

inline CControlWZ * uiGuildEventDialog::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildEventDialog::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}
#endif
