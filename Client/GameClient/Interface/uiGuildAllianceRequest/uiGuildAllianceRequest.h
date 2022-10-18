#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildAllianceRequest : public uiBase
{
	enum eDIALOG_POS
	{
		eDIALOG_TXT_TITLE,
		eDIALOG_BTN_OK,
		eDIALOG_BTN_CANCLE,
		eDIALOG_LIST_MAIN,	//다음 진행.
		DLGPOS_MAX,
	};

public:
	uiGuildAllianceRequest(InterfaceManager * pUIMan );
	~uiGuildAllianceRequest();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

	void				SetGuildName(TCHAR* szGuildName);
	void				SetGuildRelationType(eGUILD_RELATION eGuildRelation);
	void				SetCharGUID(CHARGUID iCharGUID);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				AddFormatText();

private:
	static WzID		m_wzID[DLGPOS_MAX];
	bool			m_bShow;
	CCtrlStaticWZ*	m_pTitleStatic;
	CCtrlListWZ*    m_pList;

	CCtrlImageStaticWZ*  m_pOKBtn;
	CCtrlImageStaticWZ*  m_pCancelBtn;
	
	int				m_iDelayTime;

	TCHAR			m_szGuildName[MAX_GUILDNAME_LENGTH + 1];
	eGUILD_RELATION m_eGuildRelation;
	CHARGUID		m_iCharGUID;
};

inline CControlWZ * uiGuildAllianceRequest::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildAllianceRequest::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
