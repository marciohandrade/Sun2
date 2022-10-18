#ifndef _uiGuildConvertGP_h_
#define _uiGuildConvertGP_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiBase.h"
#include "uiGuildMan/uiGuildMan.h"

class uiGuildMan;
class uiGuildConvertGP : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_CONVERT,
		DLGPOS_BTN_CANCLE,
		DLGPOS_BTN_MAXIUM,
		DLGPOS_BTN_DOWN,
		DLGPOS_BTN_UP,
		DLGPOS_TEXT_GP,
		DLGPOS_MAX,
	};


public:
	uiGuildConvertGP(InterfaceManager *pUIMan);
	virtual ~uiGuildConvertGP();

	virtual void			Init( CDrawBase * pDrawBase );
	virtual void			Release();
	virtual void			MessageProc( SI_MESSAGE * pMessage );
	virtual void			NetworkProc( MSG_BASE * pMsg );
	virtual void    		Process(DWORD dwTick);
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	uiGuildMan*             GetManager();
	GPTYPE					GetConverGP() { return m_GPoint;}



protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	


protected:
	void					_OnShowWindow();
	void					_OnMaxUPoint();
	void					_OnIncrementPoint();
	void					_OnDecrementPoint();
	void					_FillAllTextPoint(GPTYPE iGP);


private:
	static WzID				m_wzId[DLGPOS_MAX];
	CCtrlStaticWZ*			m_pStaticGP;
	uiGuildMan*             m_pGuildMan;
	BOOL					m_bControlsInitialized;
	BOOL					m_bShow;


private:
	GPTYPE					m_GPoint;
	GPTYPE					m_MaxGPoint;
};

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ * uiGuildConvertGP::getControl( POSTYPE AtPos )
{
	assert( AtPos < DLGPOS_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiGuildConvertGP::getCtrlIDToPos( WzID wzId )
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
uiGuildConvertGP::GetManager()
{
	return this->m_pGuildMan;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildConvertGP_h_
