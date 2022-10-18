#pragma once

struct sHEARTBEAT_VALUATION_INFO
{
	DWORD	m_dwCheckUserValuationPeriod;
	DWORD	m_dwValuationPeriod;
	DWORD	m_dwHeartbeatTick;
	// m_dwValuationPeriod/m_dwHeartbeatTick ==> ���� ������ ������ ���� Heartbeat ����
	BOOL	m_bUseArmorKitHeartbeat;
};

struct sSTRICT_HEARTBEAT_UPPERBOUND_RECORD
{
	DWORD	m_dwFlagTick;
	DWORD	m_dwTotalCounts;
};

// ������ �ֱ⸶��
class PROCESS_HEARTBEAT_VALUATION : public IWorkNode
{
public:
	enum eSTATUS { FIRST, PROCESSING, LAST };
	enum eASSURANCE { CHECK_FIXER=0, ADD_FIXER, MAX };
	struct sRECORD_INFO
	{
		DWORD									dwMaxRound;
		sSTRICT_HEARTBEAT_UPPERBOUND_RECORD*	pRECORD;
	};

	PROCESS_HEARTBEAT_VALUATION() {}
	~PROCESS_HEARTBEAT_VALUATION() {}

	BOOL	SetInformation( const sHEARTBEAT_VALUATION_INFO* pVALUATION_INFO );

	static DWORD			GetHeartbeatTick() { return ms_VALUATION_INFO.m_dwHeartbeatTick; }
	static sRECORD_INFO*	GetRecordInfo() { return &ms_RECORD_INFO; }
public:
	// <INTERFACE><IMPLEMENTATION>
	virtual BOOL	DoInit();
	virtual BOOL	DoProcess();
	virtual BOOL	IsComplete();
	virtual BOOL	DoRelease();

	virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

private:
	DWORD								m_dwStatus;
	// �� �ֱ� üũ��
	DWORD								m_dwTimeout;
	// ������ �������� ������ �߻������� �ð� �ս� ������
	DWORD								m_dwAssuranceTime[MAX];
	// �� �ֱ�� ������ ����
	DWORD								m_dwCountPerPeriod;
	static sHEARTBEAT_VALUATION_INFO	ms_VALUATION_INFO;
	static sRECORD_INFO					ms_RECORD_INFO;
};

