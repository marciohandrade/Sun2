#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiEventMan class
//------------------------------------------------------------------------------

#include "uiEventMan_def.h"
#include "uiBaseMan.h"

class uiEventWindow;
class uiEventItemSelect;
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
class uiGuildEventDialog;
#endif

class ui_XMas_Confirm;		
class ui_XMas_SantaDlg;	
class ui_XMas_DonationConfirm;
class ui_XMas_DonationReward;
class ui_XMas_SoloCoupleReward;
class ui_XMas_SantaReward;	

class uiEventFriend;

class uiBuffEvent;

class uiEventMan : public uiBaseMan
{
public:
    uiEventMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void            OnInitialize();


private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ShowEventWindow(BOOL bShow);
	void					ShowEventItemSelectWindow(BOOL bShow, int iEventMode);
    
	int						GetEventMode() { return m_iEventMode; }
	
	BOOL					RequestEventStatus();
	BOOL					RequestEventItemReg(CODETYPE dwEventID);

	void					SetWait(BOOL bWait)		{ m_bWait = bWait;	}
	BOOL					IsWait()				{ return m_bWait;	}

	BOOL					IsValidEventTime(SYSTEMTIME * pCurTime , 
												SYSTEMTIME * pEventStartTime ,
												SYSTEMTIME * pEventEndTime);

	bool					IsEnableEvent(int iEventMode);


	BOOL					SendEventTimePeriod( void );
	BOOL					SendEventFriend( void );
	void					ShowEventFriendWin(BOOL bShow);
	BOOL					IsShowEventFriendWin();
	void					InsertList(InvitateFriendRewardInfo *pRewardInfo);
    void                    InitInvitateFriendList();
	const time_t			GetCurTime( void ) const { return m_CurTime; }

private:
	void					_setEnableEvent();

	void					ShowErrorMessage(DWORD dwErrorCode);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
    
    //--------------------------------------------------------------------------
    // member vars
    //
public:
	

private:
	uiEventWindow			*m_pUIEventWindow;
    uiEventItemSelect		*m_pUIEventItemSelect;
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	uiGuildEventDialog		*m_pUIGuildEventWindow;
#endif //__NA_000000_20070528_TAIWAN_GUILD_PVP

	ui_XMas_Confirm*			m_pUI_XMas_Confirm;
	ui_XMas_SantaDlg*			m_pUI_XMas_SantaDlg;
	ui_XMas_DonationConfirm*	m_pUI_XMas_DonationConfirm;
	ui_XMas_DonationReward*		m_pUI_XMas_DonationReward;
	ui_XMas_SoloCoupleReward*	m_pUI_XMas_SoloCoupleReward;
	ui_XMas_SantaReward*		m_pUI_XMas_SantaReward;

	uiBuffEvent*				m_pUIBuffEvent;

	uiEventFriend*				m_pUIFriendEvent;

	int						m_iEventMode;
	BOOL					m_bWait;

	int						m_iEvente_Regitem_GetItem;
	int						m_iEvente_Regitem_RandReward;

	time_t					m_CurTime;
};

