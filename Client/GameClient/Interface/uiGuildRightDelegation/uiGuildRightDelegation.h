#ifndef _uiGuildRightDelegation_h_
#define _uiGuildRightDelegation_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h" 

class uiGuildRightDelegation : public uiBase
{
	enum eDIALOG_POS
    {
		DLGPOS_BTN_OK,
		DLGPOS_BTN_CANCLE,
		DLGPOS_BTN_EXIT,
		DLGPOS_CHECK1,
		DLGPOS_CHECK2,
		DLGPOS_CHECK3,
		DLGPOS_CHECK4,
		DLGPOS_CHECK5,
		DLGPOS_CHECK6,
		DLGPOS_TEXT1,
		DLGPOS_TEXT2,
		DLGPOS_TEXT3,
		DLGPOS_TEXT4,
		DLGPOS_TEXT5,
		DLGPOS_TEXT6,
		DLGPOS_MAX,
    };

	struct sCHECK_STATE
	{
		eGUILD_RIGHTS	eGuildRight; // 할당된 값
		CCtrlButtonCheckWZ * pCheck;
		CCtrlStaticWZ * pStatic;
	};

public:
		uiGuildRightDelegation(InterfaceManager * pUIMan );
		~uiGuildRightDelegation();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		void				SetTargetName(TCHAR * pString){m_strTargetName = pString;}
		void				AssignCheckState(GUILD_RIGHT_USE_INFO * pRightDelegation);

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);
		void				InitControl();
		uiGuildMan *		GetManager() {return m_pGuildMan;}
		void				NET_SEND_CG_GUILD_RIGHT_DELEGATION_SYN();
private:
		static WzID	m_wzID[DLGPOS_MAX];
		bool		m_bShow;
		sCHECK_STATE m_CheckState[MAX_GUILD_DELEGATE_RIGHT];
		util::_tstring	m_strTargetName;
		uiGuildMan * m_pGuildMan;
		UI_CONTROL_STATIC_PROPERTY(TitlaName,StrToWzID("T101"));
};

inline CControlWZ * uiGuildRightDelegation::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildRightDelegation::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildRightDelegation_h_
