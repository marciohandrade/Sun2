#pragma once

#include "uibase.h"


class uiDominationTimer : public uiBase
{
public:
	enum eDIALOG_DOMINATION_TIMER
	{
		eDIALOG_DOMINATION_TIMER_TIME,
		eDIALOG_DOMINATION_TIMER_PLAYER_CNT,
		eDIALOG_DOMINATION_TIMER_MAX,
	};

	uiDominationTimer(InterfaceManager * pUIMan );
	~uiDominationTimer();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

private:

	static WzID		m_wzID[eDIALOG_DOMINATION_TIMER_MAX];

	CCtrlStaticWZ*	   m_pctrlTime;
	CCtrlStaticWZ*	   m_pctrlPlayerCnt;

	BOOL			m_bShow;

	bool			m_bStarted;

	int				m_iPlayerCnt;
	int				m_iMaxPlayerCnt;

	DWORD			m_dwCurrentTime;

#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE event_state_; // 점령전 진행 상태
#endif //_NA_008334_20150608_SONNENSCHEIN

public:
	void			SetTimer(DWORD dwTime, bool bFirst = false);
	void			SetStartTimer(bool bStart);

	void			SetPlayerCnt(int iPlayerCnt = 20);

#ifdef _NA_008334_20150608_SONNENSCHEIN
    void set_event_state(BYTE state);
    BYTE event_state() { return event_state_; }
#endif //_NA_008334_20150608_SONNENSCHEIN
};

inline CControlWZ * uiDominationTimer::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_DOMINATION_TIMER_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiDominationTimer::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_DOMINATION_TIMER_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_DOMINATION_TIMER_MAX;
}

