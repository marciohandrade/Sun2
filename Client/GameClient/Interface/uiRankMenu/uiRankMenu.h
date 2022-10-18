#pragma once
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uibase.h"

class uiRankMenu : public uiBase
{
	enum RANK_MENU_UI_POS
	{
		MISSION_RANK_LIST_BUTTON,
		POINT_RANK_LIST_BUTTON,
		EXIT_BUTTON,

		MAX_UI
	};

public:
	uiRankMenu( InterfaceManager *pUIMan );
	~uiRankMenu();

public:
    virtual void	Init(CDrawBase* pDrawBase);
    virtual void	Release();
    virtual void	OnShowWindow( BOOL val );
	virtual void	MessageProc( SI_MESSAGE * pMessage );

private:
	POSTYPE getCtrlIDToPos( WzID wzId )
	{
		for( POSTYPE i = 0 ; i < MAX_UI; ++i )
		{
			if( m_wzId[i] == wzId )
			{
				return i;
			}
		}

		return MAX_UI;
	}

	CControlWZ * getControl( POSTYPE AtPos )
	{
		ASSERT( AtPos < MAX_UI );

	    return GetControlWZ( m_wzId[AtPos] );
	}

private:
	static WzID	m_wzId[MAX_UI];

};
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
