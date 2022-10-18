#pragma once

//=================================================================================================
// MasterServer 정보 관리
//	- MasterServer는 Server로서의 관리체계, 이것은 MasterServer가 하는 역할에 대한 세부 기능들을
//  모아둔다.
//=================================================================================================
#include <BitSetOperation.h>

//
//
class ServerSessionEx;

typedef std::pair<ServerSessionEx*, BOOL>	EXIT_SERVER_PAIR;
typedef std::map<ServerSessionEx*, BOOL>		EXIT_SERVER_MAP;
typedef EXIT_SERVER_MAP::iterator		EXIT_SERVER_MAP_IT;

struct eLOGIC_VIOLATION_FIELD_INFO
{
	static const DWORD RESERVED1					= 0;
	static const DWORD FORCED_DISCONNECT_CONTROL	= 1;
	//
	static const DWORD MAX_INDEX					= 31;
};

typedef util::BitSetOperation<eLOGIC_VIOLATION_FIELD_INFO::MAX_INDEX>	LOGIC_VIOLATION_CONTOL_FLAGS;
//
//


//enum eEXIT_TYPE
//{
//	eEXIT_NONE,			// 안하는 상태
//#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
//	eEXIT_AGENTSTART,	// Agent서버 종료 시작
//	eEXIT_AGENTWAIT,	// 종료 패킷 보내고 대기중
//#endif
//	eEXIT_GAMESTART,	// Game서버 종료 시작
//	eEXIT_GAMEWAIT,		// 종료 패킷 보내고 대기중
//	eEXIT_DBPROXY,		// GameDBProxy 종료 시작
//	eEXIT_DBPROXYWAIT,	// 종료 패킷 보내고 대기중
//	eEXIT_END,			// 빨래 끝~
//};

class PROCESS_TIMER_PROCESS;


class MasterServerInfo
{
public:
	MasterServerInfo() {}
	~MasterServerInfo() {}

	VOID	Init( MasterServerEx* pMasterServer )
	{
		m_pMasterServer = pMasterServer;
		m_bTransactionExitProcess = FALSE;
		m_wkExitProcess = 0;
		//m_wkTimerProcess = 0;
		//m_pMoneyTimer = NULL;
		m_dwMoneyTimer = 0;
		m_dwMoneyTimerInterval = 0;
	}

	// 서버 종료 관련
	BOOL		IsTransactionExitProcess() { return m_bTransactionExitProcess; }
	WORK_KEY	GetWorkKey4ExitProcess() { return m_wkExitProcess; }
	BOOL		MakeServerExitList(
							SERVER_KEY IN skExitKey,
							EXIT_SERVER_MAP*& OUT pAgentServerList,
							EXIT_SERVER_MAP*& OUT pDBPServerList,
							EXIT_SERVER_MAP*& OUT pServerList );
	BOOL		RegisterServerExitProcess(
							EXIT_SERVER_MAP*& OUT pAgentServerList,
							EXIT_SERVER_MAP*& OUT pDBPServerList,
							EXIT_SERVER_MAP*& OUT pServerList );

	VOID		ExitProcessTransaction( BOOL bEnable ) { m_bTransactionExitProcess = bEnable; }

	VOID		StartTimerProcess( DWORD dwInterval );
	//util::ITimer*	GetMoneyTimer() { return m_pMoneyTimer; }

	VOID		Update();

public:
	// <INTERFACEs>

	BOOL	SendMX_FUNCRESTRICT_RELOAD_REQ( MasterServerEx* pMasterServer );
	BOOL	SendGM_OPERATION_FUNCRESTRICT_CMD( MasterServerEx* pMasterServer );

	BOOL	SendGM_OPERATION_STATISTICS_REQ( MasterServerEx* pMasterServer );

private:
	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }

private:
	MasterServerEx*			m_pMasterServer;
	BOOL					m_bTransactionExitProcess;
	WORK_KEY				m_wkExitProcess;
	//WORK_KEY				m_wkTimerProcess;
	//util::ITimer*			m_pMoneyTimer;
	DWORD					m_dwMoneyTimer;
	DWORD					m_dwMoneyTimerInterval;

	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};

