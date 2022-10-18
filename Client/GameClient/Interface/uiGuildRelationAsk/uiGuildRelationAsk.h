#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildRelationAsk : public uiBase
{
public:
	enum eDIALOG_GUILD_RELATION_ASK
	{
		eDIALOG_GUILD_RELATION_ASK_TITLE,
		eDIALOG_GUILD_RELATION_ASK_EXIT_BTN,
		eDIALOG_GUILD_RELATION_ASK_OK_BTN,
		eDIALOG_GUILD_RELATION_ASK_CANCEL_BTN,

		eDIALOG_GUILD_RELATION_ASK_TEXT,
		eDIALOG_GUILD_RELATION_ASK_EDIT,

		eDIALOG_GUILD_RELATION_ASK_MAX,
	};

	uiGuildRelationAsk(InterfaceManager * pUIMan );
	~uiGuildRelationAsk();
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

	uiGuildMan*         GetManager();
	void				AddMainTextInfo();

private:
	static WzID		m_wzID[eDIALOG_GUILD_RELATION_ASK_MAX];

	uiGuildMan*             m_pGuildMan;

	CCtrlStaticWZ*  m_pTitle;

	CCtrlButtonWZ*  m_pOKBtn;
	CCtrlButtonWZ*  m_pCancelBtn;
	CCtrlButtonWZ*  m_pExitBtn;

	CCtrlDxIMEEditBox* m_pMainEdit;
	CCtrlStaticWZ*     m_pMainText;

	BOOL			m_bShow;

	BYTE			m_btRelation; //0 == Default, 1== 悼竿, 2 == 利措, 3 == 老规利 利措
	BOOL			m_bRelationState;  //TRUE == 脚没, FALSE == 秦力

public:
	void			SetbtRelation(BYTE btVal){m_btRelation = btVal;} //0 == Default, 1== 悼竿, 2 == 利措, 3 == 老规利 利措
	void			SetbRelationState(BOOL bVal){m_bRelationState = bVal;} //TRUE == 脚没, FALSE == 秦力
};

inline CControlWZ * uiGuildRelationAsk::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_GUILD_RELATION_ASK_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildRelationAsk::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_GUILD_RELATION_ASK_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_GUILD_RELATION_ASK_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildRelationAsk::GetManager()
{
	return (uiGuildMan*)GetMan();
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
