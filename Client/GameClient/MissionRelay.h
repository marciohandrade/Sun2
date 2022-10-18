#pragma once

#include "uibase.h"

class MissionRelay : public uiBase
{
	enum eDIALOG_POS
	{
		eDIALOG_BTN_OK,
		eDIALOG_BTN_CANCLE,
		eDIALOG_TXT_MAIN,	//다음 진행.
		DLGPOS_MAX,
	};

public:
	MissionRelay(InterfaceManager * pUIMan );
	~MissionRelay();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);
	virtual void		NetworkProc(MSG_BASE * pMsg);
	void				SetForceMember(bool bFlag=FALSE){m_bForcePartyMember=bFlag;}
protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				AddFormatText();


private:
    static WzID		m_wzID[DLGPOS_MAX];
	bool			m_bShow;
	bool			m_bForcePartyMember;	//방장여부관계없이 무조건 팟없

	CCtrlStaticWZ*  m_pStatic;
	CCtrlButtonWZ*  m_pOKBtn;
	CCtrlButtonWZ*  m_pCancelBtn;
	int				m_iDelayTime;
};

inline CControlWZ * MissionRelay::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE MissionRelay::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return DLGPOS_MAX;
}