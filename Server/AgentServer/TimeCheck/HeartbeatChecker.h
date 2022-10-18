#pragma once


class HeartbeatChecker
{
public:
	enum eSTATUS
	{
		CLEAR_FIELD			= 0,
		ENABLED				= 1,
		ARMORKIT_ENABLED	= 2,
		MASK_STATUS			= (CLEAR_FIELD|ENABLED|ARMORKIT_ENABLED),
	};

	enum eWARNING_FIELD
	{
		RESERVED_FIELD			= 0,	// 
		ACCEPTABLE_VIOLATION	= 1,	// Client 컴터 성능이 처리 로직상 발생할 수 있는 오차 보정값
		OVERFLOW_SAMEROUND		= 2,	// 동일 라운드에 복수개의 Heartbeat가 발생한 회수
		ARMORKIT_SIGNALED		= 3,	// ArmorKit으로부터 '나 살아있어'하는 신호를 받은 회수 1이상만 되면 된다.
	};	

public:
	HeartbeatChecker();
	~HeartbeatChecker();

	VOID		Init();

	// TRUE일 경우 측정 시간 보정 및 컨트롤
	VOID		SetEnable( BOOL bSet );
	BOOL		IsEnabled() { return (BOOL)m_dwStatus&ENABLED; }
	VOID		Signal( User* pUser, DWORD dwCurTick );

	DWORD&		LifeTimeOut() { return m_dwLifeTimeout; }
	DWORD&		SignaledCount() { return m_dwSignaledCount; }	

protected:
	VOID		Start();
	VOID		Stop();

private:
	DWORD		GetNextRound( DWORD dwCurTick, DWORD dwTimeout );
	// 받은 Heartbeat주기가 동일 주기 내에 복수개가 들어온 경우 이에 대한 단순 평가 로직
	BOOL		CheckOverflow();
	// 평가 주기 (30round)가 되었을 때, 그 기간동안 들어온 Heartbeat 신호등등을 어떻게 처리할 것인가?
	// 수신된 Heartbeat가 비정상이고, 이것이 허용할 수 있는 수치를 벗어날 경우 Disconnect와 같은 처리를
	// 수행한다.
	// 단, 이것은 Client가 Heartbeat를 보내지 않을 경우에는 처리되지 않으므로, 외부적으로 별도의 평가
	// 주기를 갖는 독립된 로직이 요구된다. PROCESS_HEARTBEAT_VALUATION가 이 역할을 수행하게 될 것이다.
	BOOL		CheckValuationTime( User* pUser );
private:
	DWORD		m_dwStatus;
	DWORD		m_dwLifeTimeout;
	DWORD		m_dwSignaledCount;
	DWORD		m_dwRound;
	DWORD		m_dwTimeout;
	DWORD		m_dwRoundCount;
	DWORD		m_dwAccumulatedCount;
	BYTE		m_pWarningFields[sizeof(DWORD)];
	BOOL		m_bFirstStart;
	//DWORD		m_dwPauseTime;
};


