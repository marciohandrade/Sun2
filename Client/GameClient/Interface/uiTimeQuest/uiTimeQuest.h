#pragma once

#include "uibase.h"


class Quest;
class uiQuestMan;
class uiTimeQuest :	public uiBase
{
	enum eDIALOG_POS
	{
		DLG_QUEST_NAME,
		DLG_REMAINING_TIME,
        DLG_QUEST_NAME_PICTRURE,
        DLG_REMAINING_TIME_PICTRURE,
		DLG_MAX,
	};
	
	enum
	{
		DLG_TXT_SIZE = DLG_REMAINING_TIME - DLG_QUEST_NAME + 1,
	};

public:
	uiTimeQuest(InterfaceManager *pUIMan);
	virtual ~uiTimeQuest(void);

	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );

	void				InitControls();
	uiQuestMan*			GetManager();
	void				SetTimeQuest(Quest * pQuest);

private:
	
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );
	static WzID			m_wzId[DLG_MAX];
	CCtrlStaticWZ*		m_pTxtControl[DLG_MAX];

	//퀘스트타임 변수
	Quest *				m_pTimeQuest;		
	bool				m_bShowLastMin;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiTimeQuest::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DLG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DLG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiTimeQuest::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < DLG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
