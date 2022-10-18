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
	_FATIGUE_TYPE*	m_pCurFatigueType;		// ���� �÷��� �ð��� ����Ǵ� FatigueType;

	util::Timer		m_PlayingTimer;			// �÷��̾��� �÷��� �ð��� �����ϴ� ī����.			
	int				m_nSecPlayTime;			// ���� ������ ���� �÷��� �ð�.( ���� : �� )
	int				m_nSecRemainMsgTime;	// �޼����� �����ð����� ���� �ð�.( ���� : �� )
	BOOL			m_bApplyFatigue;		// ���κ��� �ǵ��θ� �����Ű���� ����.

	__PROPERTY( WORD, CurDay );				// ���� ����.
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
	int			GetMinutePlayTime(){ return (int)(m_nSecPlayTime / 60); }	//�д����� �÷��� Ÿ���� ���Ѵ�.
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

