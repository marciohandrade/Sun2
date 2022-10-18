#include "stdafx.h"
#include "HeartbeatManager.h"
#include <UserSessions/UserManager.h>

//
sHEARTBEAT_VALUATION_INFO	PROCESS_HEARTBEAT_VALUATION::ms_VALUATION_INFO = { 0, };
PROCESS_HEARTBEAT_VALUATION::sRECORD_INFO	PROCESS_HEARTBEAT_VALUATION::ms_RECORD_INFO = { 0, };
//

BOOL	PROCESS_HEARTBEAT_VALUATION::SetInformation( const sHEARTBEAT_VALUATION_INFO* pVALUATION_INFO )
{
	sHEARTBEAT_VALUATION_INFO* pINFO = (sHEARTBEAT_VALUATION_INFO*)pVALUATION_INFO;

	ASSERT( pINFO && pINFO->m_dwValuationPeriod && pINFO->m_dwHeartbeatTick );

	if( !(pINFO && pINFO->m_dwValuationPeriod && pINFO->m_dwHeartbeatTick ) )
		return FALSE;

	//
	m_dwCountPerPeriod = 1;

	// 모든 유저들을 대상으로 한 Heartbeat 무결성 검사주기... 말은 좋지만 실상 Heartbeat 오는지 등의 여부검사
	DWORD dwCheckPeriod = pINFO->m_dwCheckUserValuationPeriod;
	ms_VALUATION_INFO.m_dwCheckUserValuationPeriod = dwCheckPeriod < 1000 ? 1000 : dwCheckPeriod;

	// 각 유저별 Heartbeat 패킷 수신시 수행되는 평가로직에 사용되는 인자설정
	ms_VALUATION_INFO.m_dwValuationPeriod = pINFO->m_dwValuationPeriod;
	ms_VALUATION_INFO.m_dwHeartbeatTick = pINFO->m_dwHeartbeatTick;
	ms_VALUATION_INFO.m_bUseArmorKitHeartbeat = pINFO->m_bUseArmorKitHeartbeat;
	//

	return TRUE;
}

//sSTRICT_HEARTBEAT_UPPERBOUND_RECORD s_pRECORDs[50];
BOOL	PROCESS_HEARTBEAT_VALUATION::DoInit()
{
	m_dwStatus = this->FIRST;
	m_dwAssuranceTime[this->CHECK_FIXER] = 0;
	m_dwAssuranceTime[this->ADD_FIXER] = 0;

	DWORD& dwMaxRound = ms_RECORD_INFO.dwMaxRound;
	dwMaxRound = (DWORD)(ms_VALUATION_INFO.m_dwValuationPeriod / ms_VALUATION_INFO.m_dwHeartbeatTick);
	dwMaxRound += (DWORD)((ms_VALUATION_INFO.m_dwValuationPeriod % ms_VALUATION_INFO.m_dwHeartbeatTick) ? 1 : 0);

	ms_RECORD_INFO.dwMaxRound = dwMaxRound;
	ms_RECORD_INFO.pRECORD = new sSTRICT_HEARTBEAT_UPPERBOUND_RECORD[ dwMaxRound ];
	memset( ms_RECORD_INFO.pRECORD, 0, sizeof(sSTRICT_HEARTBEAT_UPPERBOUND_RECORD)*dwMaxRound );

	//ms_RECORD_INFO.pRECORD = s_pRECORDs;
	//memset( ms_RECORD_INFO.pRECORD, 0, sizeof(s_pRECORDs) );

	sSTRICT_HEARTBEAT_UPPERBOUND_RECORD* pRECORD = ms_RECORD_INFO.pRECORD;
	//DWORD dwSumTick = ms_VALUATION_INFO.m_dwHeartbeatTick;

	for( DWORD i=0 ; i<dwMaxRound ; ++i, ++pRECORD )
	{
		//pRECORD->m_dwFlagTick = dwSumTick;
		//pRECORD->m_dwTotalCounts = i+1;
		//dwSumTick += ms_VALUATION_INFO.m_dwHeartbeatTick;
		pRECORD->m_dwFlagTick = ms_VALUATION_INFO.m_dwHeartbeatTick;
		pRECORD->m_dwTotalCounts = i+1;
	}

	DWORD dwPeriod = ms_VALUATION_INFO.m_dwCheckUserValuationPeriod;
	DWORD dwStartTick = GetTickCount();
	m_dwTimeout = dwStartTick + dwPeriod;


	return TRUE;
}

class CheckValuationHeartbeat
{
	DWORD m_dwCurTick;
	DWORD m_dwValuationPeriod;
public:
	CheckValuationHeartbeat( DWORD dwCurTick, DWORD dwValuationPeriod )
		: m_dwCurTick(dwCurTick), m_dwValuationPeriod(dwValuationPeriod) {}
	~CheckValuationHeartbeat(){}

	void operator() ( UserSession* pUser )
	{
		User* pCheckUser = (User*)pUser;
		HeartbeatChecker& rChecker = pCheckUser->GetHeartbeatChecker();
		DWORD& dwLifeTimeout = rChecker.LifeTimeOut();

		if( m_dwCurTick < dwLifeTimeout )
			return;

		DWORD& dwSignaledCount = rChecker.SignaledCount();
		if( dwSignaledCount )
		{
			// 최소한 하나라도 Heartbeat 패킷을 수신받았다.
			// 수신 받은 패킷 수는 여기서는 평가대상이 아니다.
			dwLifeTimeout += m_dwValuationPeriod;
			dwSignaledCount = 0;
		}
		else
		{
			// 다음 두 줄은 임시 루틴이다.
			dwLifeTimeout += m_dwValuationPeriod;
			dwSignaledCount = 0;

			//한번도 수신받지 않는 경우다. 이 경우라면 끊어도 무방

			// 이 루틴은 클라이언트 정지 상태가 해결되면 제공하기로 한다.
			pCheckUser->SetDisconnectCode( RC::RC_DISCONNECT_IRREGULAR_CLIENT_CYCLE_TICK );
			pCheckUser->DisconnectUser( TRUE );

			SECURELOG( SECURELOG_USERINFO(pCheckUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_HEARTBEAT_DISCONNECT), 
				_T("[VALUATION ROUTINE] Disconnect by Heartbeat")	);
			return;
		}

		// ArmorKit Heartbeat가 구현되었을 경우에는 여기에 구현하기로 한다.
	}
};

BOOL	PROCESS_HEARTBEAT_VALUATION::DoProcess()
{
    // CHANGES: f101229.2L, skip time statistics checker because LMP issues.
    return true;
    //
#if SUN_CODE_BACKUP
	DWORD dwCurTick = GetTickCount();
	if( m_dwTimeout > dwCurTick )
		return TRUE;

	// 가장 마지막에 업데이트 하도록 하자. 처리과정에서 시간이 바뀌는 경우... -_-;;;
	// 이 루틴은 그렇게까지 할 필요 없나?
	DWORD dwPeriod = ms_VALUATION_INFO.m_dwCheckUserValuationPeriod;
	m_dwTimeout = dwCurTick + dwPeriod;

	// 처음을 제외한 주기마다 모든 플레이중인 유저들을 대상으로 검사하는 루틴이 요구된다.
	// 측정 시점:
	//   해당 유저의 클라이언트에 ArmorKit이 동작중인가?
	//   해당 유저의 Heartbeat는 수신되고 있는가?
	//   기타 등등
	DWORD dwHeartbeatTick = PROCESS_HEARTBEAT_VALUATION::GetHeartbeatTick();
	DWORD dwValuationPeriod = dwHeartbeatTick * (PROCESS_HEARTBEAT_VALUATION::GetRecordInfo()->dwMaxRound);

	CheckValuationHeartbeat checker(dwCurTick, dwValuationPeriod);
	UserManager::Instance()->Foreach( checker );

	return TRUE;
#endif //SUN_CODE_BACKUP
}

BOOL	PROCESS_HEARTBEAT_VALUATION::IsComplete()
{
	return (BOOL)(this->LAST == m_dwStatus);
}

BOOL	PROCESS_HEARTBEAT_VALUATION::DoRelease()
{
	SAFE_DELETE(ms_RECORD_INFO.pRECORD);
	return TRUE;
}

VOID	PROCESS_HEARTBEAT_VALUATION::OnMsg( sSIG_MSG* pSIG_MSG )
{
    __UNUSED(pSIG_MSG);
}
