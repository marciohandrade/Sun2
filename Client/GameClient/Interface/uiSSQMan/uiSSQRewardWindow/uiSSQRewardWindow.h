#pragma once

#include "uiBase.h"

struct SSQ_REWARD_INFO
{
	SSQ_REWARD_INFO() {}
	SSQ_REWARD_INFO( DWORD Point, DWORD Exp, BYTE MonsterKillNum, BYTE PlayerKillNum )
	: m_Point( Point )
	, m_Exp( Exp )
	, m_MonsterKillNum( MonsterKillNum )
	, m_PlayerKillNum( PlayerKillNum )
	{

	}

	DWORD		m_Point;				
	DWORD		m_Exp;				
	BYTE		m_MonsterKillNum;		
	BYTE		m_PlayerKillNum;		
};

enum	REWARD_TYPE
{
	REWARD_WIN,
	REWARD_FAIL,
	REWARD_MAX
};

class uiSSQRewardWindow : public uiBase
{
	enum SSQ_REWARD_WINDOW
	{
		NOTICE_STATIC,
		ACQUIREMENT_POINT_STATIC,
		REWARD_EXP_STATIC,
		MONSTER_KILL_COUNT_STATIC,
		PLAYER_KILL_COUNT_STATIC,
		CONFIRM_BUTTON,
		EXIT_BUTTON,
		MAX_SSQ_REWARD_WINDOW,
	};

public:
	uiSSQRewardWindow( InterfaceManager * pUIMan );
	~uiSSQRewardWindow(void);

	virtual void Init( CDrawBase * pDrawBase );
	virtual void Release();
	virtual void Process( DWORD dwTick );	
	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

	void	SetRewardInfo( const SSQ_REWARD_INFO & Info ) { m_RewardInfo = Info; }
	void	SetType( REWARD_TYPE Type ) { m_Type = Type; }
	void	SetRewardInfoToUI( void );

private:

	CControlWZ * getControl(POSTYPE AtPos)
	{
		assert( AtPos < MAX_SSQ_REWARD_WINDOW );
		return GetControlWZ( m_wzID[AtPos] );
	}

	POSTYPE getControlIDToPos(WzID wzID)
	{
		for( int i = 0; i < MAX_SSQ_REWARD_WINDOW; ++i)
		{
			if( m_wzID[i] == wzID )
				return i;
		}

		return MAX_SSQ_REWARD_WINDOW;
	}

private:
	static WzID		m_wzID[MAX_SSQ_REWARD_WINDOW];
	SSQ_REWARD_INFO	m_RewardInfo;
	REWARD_TYPE			m_Type;
};
