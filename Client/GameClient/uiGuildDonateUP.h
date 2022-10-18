#ifndef _uiGuildDonateUP_h_
#define _uiGuildDonateUP_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiBase.h"

class uiGuildMan;
class uiGuildDonateUP : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_DONATE,
		DLGPOS_BTN_CANCLE,
		DLGPOS_EDIT_UPOINT,
		DLGPOS_MAX,
	};

public:
	uiGuildDonateUP(InterfaceManager * pUIMan);
	virtual ~uiGuildDonateUP();
	virtual void			Init( CDrawBase * pDrawBase );
	virtual void			Release();
	virtual void			MessageProc( SI_MESSAGE * pMessage );
	virtual void			NetworkProc( MSG_BASE * pMsg );
	virtual void    		Process(DWORD dwTick);
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	uiGuildMan*             GetManager();

protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );


public:
	UPTYPE					GetDonateUP() { return m_donateUP; }
	void					ResetDonateUP(){m_donateUP = 0;}

protected:
	BOOL					IsDigit(const TCHAR *);
	void					UPProcess();

private:
	static WzID				m_wzId[DLGPOS_MAX];
	uiGuildMan *			m_pGuildMan;
	BOOL					m_bControlInitialized;
	CCtrlDxIMEEditBox *		m_pEdit;
	UPTYPE					m_donateUP;
	BOOL					m_bShow;

	TCHAR			m_szMoneyOld[INTERFACE_STRING_LENGTH + 1];
	TCHAR			m_szMoneyNew[INTERFACE_STRING_LENGTH + 1];
};


//------------------------------------------------------------------------------
/**
*/
inline CControlWZ * uiGuildDonateUP::getControl( POSTYPE AtPos )
{
	assert( AtPos < DLGPOS_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiGuildDonateUP::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DLGPOS_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DLGPOS_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildDonateUP::GetManager()
{
	return this->m_pGuildMan;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildDonateUP_h_
