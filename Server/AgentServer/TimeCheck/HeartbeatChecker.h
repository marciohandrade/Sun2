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
		ACCEPTABLE_VIOLATION	= 1,	// Client ���� ������ ó�� ������ �߻��� �� �ִ� ���� ������
		OVERFLOW_SAMEROUND		= 2,	// ���� ���忡 �������� Heartbeat�� �߻��� ȸ��
		ARMORKIT_SIGNALED		= 3,	// ArmorKit���κ��� '�� ����־�'�ϴ� ��ȣ�� ���� ȸ�� 1�̻� �Ǹ� �ȴ�.
	};	

public:
	HeartbeatChecker();
	~HeartbeatChecker();

	VOID		Init();

	// TRUE�� ��� ���� �ð� ���� �� ��Ʈ��
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
	// ���� Heartbeat�ֱⰡ ���� �ֱ� ���� �������� ���� ��� �̿� ���� �ܼ� �� ����
	BOOL		CheckOverflow();
	// �� �ֱ� (30round)�� �Ǿ��� ��, �� �Ⱓ���� ���� Heartbeat ��ȣ����� ��� ó���� ���ΰ�?
	// ���ŵ� Heartbeat�� �������̰�, �̰��� ����� �� �ִ� ��ġ�� ��� ��� Disconnect�� ���� ó����
	// �����Ѵ�.
	// ��, �̰��� Client�� Heartbeat�� ������ ���� ��쿡�� ó������ �����Ƿ�, �ܺ������� ������ ��
	// �ֱ⸦ ���� ������ ������ �䱸�ȴ�. PROCESS_HEARTBEAT_VALUATION�� �� ������ �����ϰ� �� ���̴�.
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


