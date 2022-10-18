#include "uiBase.h"

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM

class uiVendorMan;
class uiVendorBuyPopup : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_OK,
		DLGPOS_BTN_CANCLE,
		DLGPOS_BTN_LEFT,
		DLGPOS_BTN_RIGHT,
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_MAX,
		DLGPOS_EDIT_INPUT,
		DLGPOS_MAX,
	};

public:
		uiVendorBuyPopup(InterfaceManager * pUIMan );
		~uiVendorBuyPopup();
		virtual void			Init(CDrawBase* pDrawBase);
		virtual void			Release();
		virtual void			Process(DWORD dwTick);
		virtual void			OnShowWindow(BOOL val );
		virtual void			MessageProc(SI_MESSAGE * pMessage );
		virtual void			NetworkProc(MSG_BASE * pMsg );


public:
	void					SetMaxNum( int val ) { m_iMaxNum = val; }   // 외부에서 셋팅해 주어야 함.
	int						GetMaxNum() { return m_iMaxNum ; }
	void					SetCurSlotPos( POSTYPE pos ) { m_curSlotPos = pos; } // 외부에서 셋팅해 주어야함.

	
protected:
		CControlWZ *			getControl(POSTYPE AtPos);
		POSTYPE					getControlIDToPos(WzID wzID);	
		uiVendorMan *			GetManager();
		BOOL					IsDigit(const TCHAR * pszMessage);

private:
	static WzID						m_WzId[DLGPOS_MAX];
	uiVendorMan *				m_pVendorMan;
	CCtrlDxIMEEditBox *			m_pEdit;
	int							m_iNum;		
	int							m_iMaxNum;
	POSTYPE						m_curSlotPos;
};



inline CControlWZ * uiVendorBuyPopup::getControl(POSTYPE AtPos)
{
	assert( AtPos < DLGPOS_MAX);
	return GetControlWZ(m_WzId[AtPos]);
}

inline POSTYPE uiVendorBuyPopup::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if( m_WzId[i] == wzID)
			return i;
	}
	return DLGPOS_MAX;
}

inline uiVendorMan * uiVendorBuyPopup::GetManager()
{
	return m_pVendorMan;
}

	
#endif