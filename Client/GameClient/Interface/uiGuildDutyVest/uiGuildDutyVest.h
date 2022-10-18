#ifndef _uiGuildDutyVest_h_
#define _uiGuildDutyVest_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"
#include "uiGuildMan/uiGuildMan.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"

#define NUM_CHECK_CONTROL 6
class uiGuildDutyVest : public uiBase
{

	struct sCHECK_STATE
	{
		eGUILD_DUTY	eGuildDuty; // 할당된 값
		CCtrlButtonCheckWZ * pCheck;
		CCtrlStaticWZ * pStatic;
	};

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
		DLGPOS_CHECK_TEXT1,
		DLGPOS_CHECK_TEXT2,
		DLGPOS_CHECK_TEXT3,
		DLGPOS_CHECK_TEXT4,
		DLGPOS_CHECK_TEXT5,
		DLGPOS_CHECK_TEXT6,

		DLGPOS_LIST,
		DLGPOS_MAX,
    };

public:
		uiGuildDutyVest(InterfaceManager * pUIMan );
		~uiGuildDutyVest();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

		uiGuildMan *		GetManager() {return m_pGuildMan;}

		void				SetTargetName(TCHAR * pString){m_strTargetName = pString;}

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

		void				InitControl();
		void				AssignCheckState();
		void				InitCheckState();
		void				SetCheckState(int index );
		bool				NET_SEND_CG_GUILD_CHANGE_DUTY_SYN();

private:
		static WzID	m_wzID[DLGPOS_MAX];
		bool		m_bShow;
		sCHECK_STATE m_CheckState[NUM_CHECK_CONTROL];
		uiGuildMan * m_pGuildMan;
		util::_tstring	m_strTargetName;
		UI_CONTROL_STATIC_PROPERTY(TitleName, StrToWzID("T101"));
		CCtrlListWZ *	m_pList;
};

inline CControlWZ * uiGuildDutyVest::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildDutyVest::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildDutyVest_h_
