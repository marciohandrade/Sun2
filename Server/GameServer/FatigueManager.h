#pragma once

class Player;
struct _FATIGUE_TYPE;

class FatigueManager
{
public:
	FatigueManager(void);
	~FatigueManager(void);

	enum eFATIGUE_INIT_TYPE
	{
		FATIGUE_INIT_MIDNIGHT = 1,
		FATIGUE_INIT_OFFLINE = 2,
	};

private:
	Player *		m_pPlayer;
	_FATIGUE_TYPE*	m_pCurFatigueType;		// 현재 플레이 시간에 적용되는 FatigueType;

	util::Timer		m_PlayingTimer;			// 플레이어의 플레이 시간을 누적하는 카운터.			
	int				m_nSecPlayTime;			// 현재 유저의 누적 플레이 시간.( 단위 : 초 )
	int				m_nSecRemainMsgTime;	// 메세지를 보낼시간까지 남은 시간.( 단위 : 초 )
	BOOL			m_bApplyFatigue;		// 개인별로 피도로를 적용시키는지 여부.

	__PROPERTY( WORD, CurDay );				// 오늘 날자.
	__PROPERTY( BOOL, Init );
	__PROPERTY( DWORD, CurTickCount );
	__PROPERTY( DWORD, PreTickCount );

public:
	VOID		Init( Player * pPlayer );
	VOID		Update();
	BYTE		GetItemDropRatio();
	BYTE		GetExpRatio();
	BOOL		GetItemPickUp();
	BYTE		GetShareItemPickupRatio();
	BYTE		GetShareHeimPickupRatio();
    //implemented by __NA_000000_20070704_FATIGUE_REWARD
	BOOL		GetItemReward();
	BYTE		GetHeimRewardRatio();
	BYTE		GetExpRewardRatio();

	BOOL			IsApplyFatigue();
	VOID			SetApplyFatigue( BOOL bApplyFatigue ){ m_bApplyFatigue = bApplyFatigue; }
	_FATIGUE_TYPE*	GetCurFatigueType();

	int			GetPlayTime(){ return m_nSecPlayTime; }
private:
	BOOL		CheckMaxPlayTime( SYSTEMTIME& sysTime );
	BOOL		SetDBPlayTime( INT nMinPlayTime );
	VOID		SetPlayTime( int nSecPlayTime );	
	int			GetMinutePlayTime(){ return (int)(m_nSecPlayTime / 60); }	//분단위로 플레이 타임을 구한다.
	VOID		AddPlayTime( int nSecPlayTime );
	void		SendTimeMessage();
	BYTE		GetCurFatigueTypeNum();
	BOOL		CanApplyFatigue();
	VOID		InitTickCount();
	DWORD		GetDiffTickCount(){ return ( GetCurTickCount() - GetPreTickCount() ); }
	BOOL		IsMsgTimeExpired();
	BOOL		ChangeFatigueType();
	BOOL		CheckAuth();
};

