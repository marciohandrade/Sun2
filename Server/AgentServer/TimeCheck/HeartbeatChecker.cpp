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

// TRUE일 경우 측정 시간 보정 및 컨트롤
VOID		HeartbeatChecker::SetEnable( BOOL bSet )
{
	// 작동 중지 명령중...
	if( !bSet )
	{
		//현재 작동중이었다면
		if( this->ENABLED & m_dwStatus )
		{
			Stop();
		}
	}

	// 검사 시작/재개 명령
	else
	{
		//이미 시작중이면
		if( this->ENABLED & m_dwStatus )
			return;
        
		Start();
	}	
}

VOID HeartbeatChecker::Start()
{
	//최초 시작이면
	if( m_bFirstStart )
	{
		DWORD dwCurTick = GetTickCount();
		DWORD dwHeartbeatTick = PROCESS_HEARTBEAT_VALUATION::GetHeartbeatTick();
		m_dwLifeTimeout
			= dwCurTick + (dwHeartbeatTick * PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->dwMaxRound);
		m_dwTimeout = dwCurTick + dwHeartbeatTick;		

		m_bFirstStart = FALSE;
	}
	//Pause 후 Resume이면
	else
	{
		//ASSERT(m_dwPauseTime);
		DWORD dwNowTickCount = GetTickCount();
		//if( dwNowTickCount < m_dwPauseTime )
		//{
		//	ASSERT(FALSE);
		//	return;
		//}		
		//Pause 된 시간만큼 추가.		
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

	// 동일 Round
	return m_dwRound;
}

BOOL		HeartbeatChecker::CheckOverflow()
{
	// Heartbeat 신호를 받았을 때, 해당 라운드에 대한 검사이며, 동일 라운드 내에 복수개
	// 신호가 들어왔을 때만 검사하는 루틴
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
	// <1> 평가 주기동안 들어온 Heartbeat의 총 개수 평가 (최대치 평가)
	//   '2' 값은 hard-coding 일단 좀더 데이터가 요구된다.
	//   dwMR보다 큰 값이면 일단 SpeedHack을 이용한 고속 플레이가 될 것이다.
	//if( rdwTC > dwMR + 2 )
	if( rdwTC > dwMR + ((dwMR>>2) + 4) )
	{
		bValuation = FALSE;
		eRet = RC::RC_DISCONNECT_SPEED_HACK;
	}

	//---------------------------------------------------------------------------
	// <2> 평가 주기동안 들어온 Heartbeat의 총 개수 평가 (최소치 평가)
	//   0 인 경우를 제외하고, (이건 사실 여기로 들어올 리 없다)
	//   '.6f'는 hard-coding 일단 좀 더 데이터가 요구된다.
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
	// <3> 평가 주기동안 ArmorKit Heartbeat가 설정되었을 시 ArmorKit 활성 여부 평가
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
		// 이 루틴은 클라이언트 정지 상태가 해결되면 제공하기로 한다.
		pUser->SetDisconnectCode( eRet );
		pUser->DisconnectUser( TRUE );
	}

	return bValuation;
}

VOID		HeartbeatChecker::Signal( User* pUser, DWORD dwCurTick )
{	
	//중지면 자동 Resume
	if( !(this->ENABLED & m_dwStatus) )
		Start();

	// 작동 여부 체크 루틴 삽입할 것
	// 일단은 작동중이 아니더라도 받아들이는 정책을 취하자.
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
		// 동일 Round 내 복수 Count
		++rdwRC;
		++rdwTC;

		// <NEEDED> 단일 평가 로직
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