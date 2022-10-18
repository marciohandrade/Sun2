#ifndef _uiGuildMarkRegisteConfirm_h_
#define _uiGuildMarkRegisteConfirm_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildMan;

class uiGuildMarkRegisteConfirm : public uiBase
{
	enum eDIALOG_POS
    {
		DLG_BTN_OK,
		DLG_BTN_CANCLE,
		DLG_BTN_EXIT,
		DLG_BTN_TARGET_MARK,
		DLGPOS_MAX,
    };

public:
		uiGuildMarkRegisteConfirm(InterfaceManager * pUIMan );
		~uiGuildMarkRegisteConfirm();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

		void				SetMarkIndex(int iBackGround, int iPattern);

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);
		uiGuildMan *		GetManager() {return m_pGuildMan;}

private:
		static WzID	m_wzID[DLGPOS_MAX];
		int					m_indexMark[2];
		CCtrlButtonWZ *		m_pMarkPictureButton;
		uiGuildMan *		m_pGuildMan;
};

inline CControlWZ * uiGuildMarkRegisteConfirm::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildMarkRegisteConfirm::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildMarkRegisteConfirm_h_
