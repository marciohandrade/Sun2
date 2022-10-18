#ifndef _uiGuildCorpsChange_h_
#define _uiGuildCorpsChange_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

typedef	BOOL	(*_FnOkBtnProcess)();

class uiGuildCorpsChange : public uiBase
{

#define BUTTON_MAX 8

	struct sCheckState
	{
		CCtrlStaticWZ * pStatic;
		CCtrlButtonCheckWZ * pCheck;
		int				Corps;
	};

	enum eDIALOG_POS
    {
		DLGPOS_BTN1,
		DLGPOS_BTN2,
		DLGPOS_BTN3,
		DLGPOS_BTN4,
		DLGPOS_BTN5,
		DLGPOS_BTN6,
		DLGPOS_BTN7,
		DLGPOS_BTN8,

		DLGPOS_TXT1,
		DLGPOS_TXT2,
		DLGPOS_TXT3,
		DLGPOS_TXT4,
		DLGPOS_TXT5,
		DLGPOS_TXT6,
		DLGPOS_TXT7,
		DLGPOS_TXT8,

		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_OK,
		DLGPOS_BTN_CANCLE,

		DLGPOS_MAX,
    };

public:
		uiGuildCorpsChange(InterfaceManager * pUIMan );
		~uiGuildCorpsChange();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		bool				NET_SEND_CG_GUILD_CHANGE_CORPS_SYN();
		bool				NET_SEND_CG_GUILD_DESTROY_CORPS_SYN();
		void				ShowAndInsertCheckCorps(int index, int corps);
		void				InitAllCheckButton();
		void				SetTargetName(TCHAR * pName){ m_strTargetName = pName;}

		UI_CONTROL_STATIC_PROPERTY(TitleName, StrToWzID("T100"));
		UI_CONTROL_STATIC_PROPERTY(SubTitleName, StrToWzID("T101"));

		void SetOkBtnFnPtr( _FnOkBtnProcess	fn ) { m_FnOkBtnProcess = fn; }

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

private:
		static WzID	m_wzID[DLGPOS_MAX];
		sCheckState m_CheckState[BUTTON_MAX];
		util::_tstring		m_strTargetName;
		bool				m_bShow;
		_FnOkBtnProcess		m_FnOkBtnProcess;
};

inline CControlWZ * uiGuildCorpsChange::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildCorpsChange::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildCorpsChange_h_
