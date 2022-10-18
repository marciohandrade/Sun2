#include "StdAfx.h"
#include "HeartbeatChecker.h"
#include "HeartbeatManager.h"

HeartbeatChecker::HeartbeatChecker()
{
}

HeartbeatChecker::~HeartbeatChecker()
{
}

VOID		HeartbeatChecker::Init()
{
	m_dwStatus				= this->CLEAR_FIELD;
	m_dwSignaledCount		= 0;
	m_dwLifeTimeout			= ULONG_MAX;
	m_dwRound				= 0;
	m_dwTimeout				= 0;
	m_dwRoundCount			= 0;
	m_dwAccumulatedCount	= 0;
	
	m_bFirstStart			= TRUE;
//	m_dwPauseTime			= 0;

	memset( m_pWarningFields, 0, sizeof(m_pWarningFields) );
}

// TRUE�� ��� ���� �ð� ���� �� ��Ʈ��
VOID		HeartbeatChecker::SetEnable( BOOL bSet )
{
	// �۵� ���� �����...
	if( !bSet )
	{
		//���� �۵����̾��ٸ�
		if( this->ENABLED & m_dwStatus )
		{
			Stop();
		}
	}

	// �˻� ����/�簳 ���
	else
	{
		//�̹� �������̸�
		if( this->ENABLED & m_dwStatus )
			return;
        
		Start();
	}	
}

VOID HeartbeatChecker::Start()
{
	//���� �����̸�
	if( m_bFirstStart )
	{
		DWORD dwCurTick = GetTickCount();
		DWORD dwHeartbeatTick = PROCESS_HEARTBEAT_VALUATION::GetHeartbeatTick();
		m_dwLifeTimeout
			= dwCurTick + (dwHeartbeatTick * PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->dwMaxRound);
		m_dwTimeout = dwCurTick + dwHeartbeatTick;		

		m_bFirstStart = FALSE;
	}
	//Pause �� Resume�̸�
	else
	{
		//ASSERT(m_dwPauseTime);
		DWORD dwNowTickCount = GetTickCount();
		//if( dwNowTickCount < m_dwPauseTime )
		//{
		//	ASSERT(FALSE);
		//	return;
		//}		
		//Pause �� �ð���ŭ �߰�.		
		//DWORD dwDiff = dwNowTickCount - m_dwPauseTime;
		
		//MessageOut( eCRITICAL_LOG, "[RESUME]: Now: %u , TO: %u ", dwNowTickCount, m_dwTimeout );
		if( dwNowTickCount > m_dwTimeout )
		{
			m_dwLifeTimeout += ( dwNowTickCount - m_dwTimeout );
			m_dwTimeout		= dwNowTickCount;
		}
	}

	m_dwStatus |= this->ENABLED;
}

VOID HeartbeatChecker::Stop()
{
	m_dwStatus &= ~this->ENABLED;
//	m_dwPauseTime = GetTickCount();

	//MessageOut( eCRITICAL_LOG, "[PAUSE]: %u",GetTickCount() );
}

//  NR : (Next Round) = n%MR
//  CR : (Current Round)
//  CT : (Current Tick)
//  MR : (Max Round) = n % info.MaxRound
//  DR : (Diff Round)
//  RC : (Round Count)
//  TC : (Total Count)

DWORD		HeartbeatChecker::GetNextRound( DWORD dwCurTick, DWORD dwTimeout )
{
	if( dwCurTick >= dwTimeout )
	{
		DWORD dwDiff = dwCurTick - dwTimeout;
		DWORD dwPeriod = PROCESS_HEARTBEAT_VALUATION::GetHeartbeatTick();
		DWORD dwNR = (DWORD)(dwDiff / dwPeriod) + m_dwRound + 1;
		return dwNR;
	}

	// ���� Round
	return m_dwRound;
}

BOOL		HeartbeatChecker::CheckOverflow()
{
	// Heartbeat ��ȣ�� �޾��� ��, �ش� ���忡 ���� �˻��̸�, ���� ���� ���� ������
	// ��ȣ�� ������ ���� �˻��ϴ� ��ƾ
	DWORD dwOverflowRC = m_pWarningFields[this->OVERFLOW_SAMEROUND];
	dwOverflowRC = (dwOverflowRC+1) > 0xFF ? 0xFF : dwOverflowRC+1;
	m_pWarningFields[this->OVERFLOW_SAMEROUND] = (BYTE)dwOverflowRC;

	return TRUE;
}

BOOL		HeartbeatChecker::CheckValuationTime( User* pUser )
{
	DWORD dwMR = PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->dwMaxRound;
	DWORD rdwTC = m_dwAccumulatedCount;
	RC::eCONNECTION_RESULT eRet = RC::RC_DISCONNECT_UNKNOWN;

	BOOL bValuation = TRUE;

	//---------------------------------------------------------------------------
	// <1> �� �⵿ֱ�� ���� Heartbeat�� �� ���� �� (�ִ�ġ ��)
	//   '2' ���� hard-coding �ϴ� ���� �����Ͱ� �䱸�ȴ�.
	//   dwMR���� ū ���̸� �ϴ� SpeedHack�� �̿��� ��� �÷��̰� �� ���̴�.
	//if( rdwTC > dwMR + 2 )
	if( rdwTC > dwMR + ((dwMR>>2) + 4) )
	{
		bValuation = FALSE;
		eRet = RC::RC_DISCONNECT_SPEED_HACK;
	}

	//---------------------------------------------------------------------------
	// <2> �� �⵿ֱ�� ���� Heartbeat�� �� ���� �� (�ּ�ġ ��)
	//   0 �� ��츦 �����ϰ�, (�̰� ��� ����� ���� �� ����)
	//   '.6f'�� hard-coding �ϴ� �� �� �����Ͱ� �䱸�ȴ�.
	if( bValuation && rdwTC == 0 )
	{
		bValuation = FALSE;
		eRet = RC::RC_DISCONNECT_CRACKED_CLIENT;
	}

	if( bValuation )
	{
		DWORD dwMR_MIN = (DWORD)((dwMR >> 2) + 1);
		if( !(rdwTC > dwMR_MIN) )
		{
			bValuation = FALSE;
			eRet = RC::RC_DISCONNECT_IRREGULAR_CLIENT_CYCLE_TICK;
		}
	}

	//---------------------------------------------------------------------------
	// <3> �� �⵿ֱ�� ArmorKit Heartbeat�� �����Ǿ��� �� ArmorKit Ȱ�� ���� ��
	if( bValuation
		&& (m_dwStatus&this->ARMORKIT_ENABLED)
		&& !m_pWarningFields[this->ARMORKIT_SIGNALED] )
	{
		bValuation = FALSE;
		eRet = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
	}

	if( !bValuation && pUser )
	{
		m_dwLifeTimeout = ULONG_MAX;
		// �� ��ƾ�� Ŭ���̾�Ʈ ���� ���°� �ذ�Ǹ� �����ϱ�� �Ѵ�.
		pUser->SetDisconnectCode( eRet );
		pUser->DisconnectUser( TRUE );
	}

	return bValuation;
}

VOID		HeartbeatChecker::Signal( User* pUser, DWORD dwCurTick )
{	
	//������ �ڵ� Resume
	if( !(this->ENABLED & m_dwStatus) )
		Start();

	// �۵� ���� üũ ��ƾ ������ ��
	// �ϴ��� �۵����� �ƴϴ��� �޾Ƶ��̴� ��å�� ������.
	++m_dwSignaledCount;

	DWORD dwCR = m_dwRound;
	DWORD dwNR = GetNextRound( dwCurTick, m_dwTimeout );
	DWORD dwMR = PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->dwMaxRound;

	DWORD dwDR = dwNR - dwCR;

	DWORD& rdwRC = m_dwRoundCount;
	DWORD& rdwTC = m_dwAccumulatedCount;

	//MessageOut( eCRITICAL_LOG,
	////_tprintf(
	//	_T("CT\t%8u\tTO\t%8u\tDT\t%d\tCR\t%d\tNR\t%d\tMR\t%d\tDR\t%d\tRC\t%d\tTC\t%d\n"),
	//	dwCurTick, m_dwTimeout, m_dwTimeout-dwCurTick, dwCR, dwNR, dwMR, dwDR, rdwRC, rdwTC );
	//MessageOut( eCRITICAL_LOG,"---------------------------------------------------------------------");

	if( dwDR == 0 )
	{
		// ���� Round �� ���� Count
		++rdwRC;
		++rdwTC;

		// <NEEDED> ���� �� ����
		if( rdwRC > 1 )
			CheckOverflow();

		return;
	}
	else
	{
		if( dwNR < dwMR )
		{
			dwCR = dwNR%dwMR;
			rdwRC = 1;
			++rdwTC;
		}
		else
		{
			// <NEEDED> Valuation Time
			CheckValuationTime( pUser );

			dwCR = dwNR%dwMR;
			rdwRC = 1;
			rdwTC = 1;
			m_pWarningFields[this->OVERFLOW_SAMEROUND] = 0;
			m_pWarningFields[this->ARMORKIT_SIGNALED] = 0;
		}

		m_dwRound = dwCR;
		sSTRICT_HEARTBEAT_UPPERBOUND_RECORD* pRECORD = PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->pRECORD;
		pRECORD = pRECORD + dwCR;
		//m_dwTimeout = dwCurTick + pRECORD->m_dwFlagTick;
		m_dwTimeout = m_dwTimeout + (dwDR*pRECORD->m_dwFlagTick);
//		_tprintf( "Fix Value %d %d %d\n", dwDR, pRECORD->m_dwFlagTick, (dwDR*pRECORD->m_dwFlagTick) );
	}
}