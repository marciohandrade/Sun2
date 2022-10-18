#ifndef _uiGuildRankup_h_
#define _uiGuildRankup_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiBase.h"
#include "FTextRender.h"
#include "FTextSentence.h"

class uiGuildMan;
class uiGuildRankup : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_RANKUP,
		DLGPOS_BTN_EXIT,
		DLGPOS_LIST , 
		DLGPOS_TEXT_TITLE,
		DLGPOS_MAX,
	};

public:

	enum eRANKUP_DLG_STATE
	{
		eRANKUP_DLG_STATE_RANKUP,
		eRANKUP_DLG_STATE_GUILD_WAREHOUSE_CONFIRM,
	};

	uiGuildRankup(InterfaceManager *pUIMan);
	virtual ~uiGuildRankup();
	virtual void			Init( CDrawBase * pDrawBase );
	virtual void			Release();
	virtual void			MessageProc( SI_MESSAGE * pMessage );
	virtual void			NetworkProc( MSG_BASE * pMsg );
	virtual void    		Process(DWORD dwTick);
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();
	uiGuildMan*             GetManager();

	void					SetDlgState(eRANKUP_DLG_STATE eDlgState){m_eDlgState = eDlgState;}

protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	void					AddFormatText();

private:
	uiGuildMan *			m_pGuildMan;
	static WzID				m_wzId[DLGPOS_MAX];
	BOOL					m_bControlsInitialized;
	CCtrlListWZ	*			m_pList;
	BOOL					m_bShow;				

	eRANKUP_DLG_STATE		m_eDlgState;

	FTextRender				m_FtextRender;
	FTextSentence			m_FSentence;
};

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ * uiGuildRankup::getControl( POSTYPE AtPos )
{
	assert( AtPos < DLGPOS_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiGuildRankup::getCtrlIDToPos( WzID wzId )
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
uiGuildRankup::GetManager()
{
	return this->m_pGuildMan;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildRankup_h_
