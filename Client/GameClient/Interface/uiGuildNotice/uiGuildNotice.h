#ifndef _uiGuildNotice_h_
#define _uiGuildNotice_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildNotice : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_OK,			
		DLGPOS_BTN_CANCLE,
		DLGPOS_EDIT,
		DLGPOS_TXT_TITLE,
		DLGPOS_BTN_CLOSE,

		DLGPOS_MAX,

	};

public:
		uiGuildNotice(InterfaceManager * pUIMan );
		~uiGuildNotice();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		

		void				SendGuildNoticeChange();
protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

private:
		static WzID	m_wzID[DLGPOS_MAX];
		
		CCtrlDxIMEEditBox * m_pEdit;
		BOOL				m_bShow;
};

inline CControlWZ * uiGuildNotice::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildNotice::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildNotice_h_
