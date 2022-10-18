#ifndef _uiGuildMemberRightView_h_
#define _uiGuildMemberRightView_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "uiGuildMan/uiGuildMan.h"

class uiGuildMan;

class uiGuildMemberRightView : public uiBase
{

#define MAX_GUILD_RIGHT_VIEW_ITEM	22


	struct sCHECK_STATE
	{
		CCtrlButtonCheckWZ * pCheck;
		CCtrlStaticWZ * pStatic;
	};

	struct sITEM_DATA
	{
		bool	bCheck;
		DWORD	dwStringCode;
	};


	enum eDIALOG_POS
    {
		DLGPOS_CHECK_1,
		DLGPOS_CHECK_2,
		DLGPOS_CHECK_3,
		DLGPOS_CHECK_4,
		DLGPOS_CHECK_5,
		DLGPOS_CHECK_6,
		DLGPOS_CHECK_7,
		DLGPOS_CHECK_8,
		DLGPOS_CHECK_9,
		DLGPOS_CHECK_10,
		DLGPOS_CHECK_11,
		DLGPOS_CHECK_12,
		DLGPOS_CHECK_13,
		DLGPOS_CHECK_14,
		DLGPOS_CHECK_15,
		DLGPOS_CHECK_16,
		DLGPOS_CHECK_17,
		DLGPOS_CHECK_18,
		DLGPOS_CHECK_19,
		DLGPOS_CHECK_20,
		DLGPOS_CHECK_21,
		DLGPOS_CHECK_22,

		DLGPOS_TXT_1,
		DLGPOS_TXT_2,
		DLGPOS_TXT_3,
		DLGPOS_TXT_4,
		DLGPOS_TXT_5,
		DLGPOS_TXT_6,
		DLGPOS_TXT_7,
		DLGPOS_TXT_8,
		DLGPOS_TXT_9,
		DLGPOS_TXT_10,
		DLGPOS_TXT_11,
		DLGPOS_TXT_12,
		DLGPOS_TXT_13,
		DLGPOS_TXT_14,
		DLGPOS_TXT_15,
		DLGPOS_TXT_16,
		DLGPOS_TXT_17,
		DLGPOS_TXT_18,
		DLGPOS_TXT_19,
		DLGPOS_TXT_20,
		DLGPOS_TXT_21,
		DLGPOS_TXT_22,

		DLGPOS_BTN_OK,
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_LEFT,
		DLGPOS_BTN_RIGHT,
		DLGPOS_TXT_PAGE,
		DLGPOS_MAX,
    };

public:
		uiGuildMemberRightView(InterfaceManager * pUIMan );
		~uiGuildMemberRightView();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

		void				RegisteAllItem(stGUILD_MEMBER_INFO * pGuildMember, GUILD_RIGHT_USE_INFO * pRightUseInfo);
		void				RefreshItem();

protected:
		CControlWZ *				getControl(POSTYPE AtPos);
		POSTYPE						getControlIDToPos(WzID wzID);
		uiGuildMan *				GetManager() {return m_pGuildMan;}
		uiGuildMan *				m_pGuildMan;
		GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
private:
		static WzID	m_wzID[DLGPOS_MAX];
		UI_CONTROL_STATIC_PROPERTY(TargetName, StrToWzID("ST01"));

		CCtrlStaticWZ *		m_pTextTargetName;
		CCtrlStaticWZ *		m_pTextPage;
		sCHECK_STATE		m_CheckState[MAX_GUILD_RIGHT_VIEW_ITEM];
		std::vector<sITEM_DATA>	m_vecTotalRight;
		int					m_iCurPage;
};

inline CControlWZ * uiGuildMemberRightView::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildMemberRightView::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildMemberRightView_h_
