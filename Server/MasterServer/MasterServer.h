#ifndef _MASTERSERVER_H_
#define _MASTERSERVER_H_

#include <windows.h>
#include <Singleton.h>
#include <map>
#include <const.h>
#include <Timer.h>

// 서버 통화량 위해
#include <ServerStruct.h>
#include <SolarHashTable.h>
#include <vector>

using namespace std;

class IOCPServer;
class Session;
class GameDBProxySession;
class AccountDBProxySession;
class OpServerSession;
class RemoteServerSession;
class NetworkObject;
struct SERVER_MONEY_INFO;
class ServerSession;

class SolarAuthServerSession;

typedef util::SolarHashTable<SERVER_KEY*>			SERVERKEY_HASH;
typedef util::SolarHashTable<SERVER_KEY*>::iterator	SERVERKEY_HASH_ITR;

// 실제 접속중인 서버 키 보관(Key = SERVER_KEY)
typedef vector<SERVER_KEY>			SERVERKEY_VEC;
typedef SERVERKEY_VEC::iterator		SERVERKEY_VEC_ITER;

// 채널 단위로 접속중인 서버 목록 보관(Key = ChannelIndex)
typedef map<BYTE, SERVERKEY_VEC>	CHANNEL_MAP;
typedef CHANNEL_MAP::iterator		CHANNEL_MAP_ITER;
typedef pair<BYTE, SERVERKEY_VEC>	CHANNEL_MAP_PAIR;

// 월드 단위로 접속중인 서버 목록 보관(Key = WorldIndex)
typedef map<BYTE, CHANNEL_MAP>			WORLD_MAP;
typedef WORLD_MAP::iterator			WORLD_MAP_ITER;
typedef pair<BYTE, CHANNEL_MAP>		WORLD_MAP_PAIR;

//#define _WHATTHEHELL_AGENTSHUTDOWN_20060524

enum eEXIT_TYPE
{
	eEXIT_NONE,			// 안하는 상태
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	eEXIT_AGENTSTART,	// Agent서버 종료 시작
	eEXIT_AGENTWAIT,	// 종료 패킷 보내고 대기중
#endif
	eEXIT_GAMESTART,	// Game서버 종료 시작
	eEXIT_GAMEWAIT,		// 종료 패킷 보내고 대기중
	eEXIT_DBPROXY,		// GameDBProxy 종료 시작
	eEXIT_DBPROXYWAIT,	// 종료 패킷 보내고 대기중
	eEXIT_END,			// 빨래 끝~
};


class MasterServer : public Singleton<MasterServer>
{
	enum
	{
		IOHANDLER_KEY = 0,
		INSIDE_IOHANDLER = IOHANDLER_KEY,
		OUTSIDE_IOHANDLER = 1,
		MAX_SIZE_IOHANDLER = 2
	};

	friend NetworkObject* CallBackOnAccept( DWORD dwNetworkIndex );
	friend VOID CallBackOnDisconnect( DWORD dwSessionIndex, NetworkObject *pNetworkObject );

public:
	MasterServer();
	~MasterServer();

	BOOL					Init();
	VOID					Release();
	VOID					Run();
	BOOL					Update();
	VOID					StartListen();
	VOID					ConnectToServer( NetworkObject *pNetworkObject, char *pszIP, WORD wPort );
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	VOID					ConnectToServerOutside( NetworkObject *pNetworkObject, char *pszIP, WORD wPort );
	static VOID				StartDataService();
	inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
	inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	inline VOID				Shutdown() { m_bShutdown = TRUE; }
	inline BOOL				IsOnShutdown() { return m_bShutdown; }
	BOOL					IsConnectedTo( eSERVER_TYPE eServerType );

//	VOID					SetMoney( SERVER_KEY serverkey, ULONGLONG &ui64GetMoney, ULONGLONG &ui64LoseMoney, ULONGLONG &ui64UserMoney );
//	BOOL					GetMoney( SERVER_KEY serverkey, ULONGLONG &ui64GetMoney, ULONGLONG &ui64LoseMoney, ULONGLONG &ui64UserMoney );
//	VOID					RemoveMoney( SERVER_KEY serverkey );
//	DWORD					GetMoneyInfoNum();
//	VOID					SetMoneyInfoFirst();
//	SERVER_MONEY_INFO*		GetMoneyInfoNext();

	inline eSERVER_TYPE		GetServerType() { return MASTER_SERVER; }

	// 서버 종료 관련
	inline BOOL				IsExitServer() { return m_bExitServer; }
	inline VOID				SetExitServer( BOOL bExit = TRUE ) { m_bExitServer = bExit; m_bExitFailed = FALSE; }
	inline VOID				SetExitKey( DWORD dwKey, __int64 i64WopsKey ) { m_dwExitKey = dwKey; m_i64WopsKey = i64WopsKey; }
	inline DWORD			GetExitKey() { return m_dwExitKey; }
	inline __int64			GetExit64Key() { return m_i64WopsKey; }
	inline eEXIT_TYPE		GetExitType() { return m_eExitType; }
	inline VOID				SetExitType( eEXIT_TYPE eExit ) { m_eExitType = eExit; }
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	BOOL					AddExitAgentServer( SERVER_KEY sServerKey );
	VOID					RemoveExitAgentServer( SERVER_KEY serverKey );
#endif
	BOOL					AddExitServer( SERVER_KEY sServerKey );			// 종료할 서버 추가
	VOID					RemoveExitServer( SERVER_KEY serverKey	);		// 종료할 서버 삭제
	BOOL					AddExitDBServer( SERVER_KEY sServerKey );		// 종료할 서버(DB) 추가
	VOID					RemoveExitDBServer( SERVER_KEY sServerKey );	// 종료할 서버(DB) 삭제
	BOOL					CheckExitServer();								// 종료 명령 받았는지 확인하고, 종료되었으면 TRUE

	inline ServerSession*	GetOpServer() { return (ServerSession*)m_pOpServerSession; }
	inline ServerSession*	GetAccountDBProxyServer() { return (ServerSession*)m_pAccountDBProxySession; }

	VOID					AddServerMap( SERVER_KEY &serverKey );
	VOID					RemoveServerMap( SERVER_KEY &serverKey );
	VOID					CheckServerMoney();
	VOID					CheckStatistics();
	VOID					CheckServerWarehouseMoney();
	VOID					SendMoneyReq();

	BOOL					SendRestrictReq();
	VOID					SendRestrictAns();

private:
	BOOL					ProcessConsole();
	VOID					UpdateFPS();
	VOID					DisplayServerInfo();
	VOID					MaintainConnection();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
private:
	SolarAuthServerSession*	m_pSolarAuthServerSession;
	ePHASE_SHIFT			m_PHASE_SHIFT;
	//GLOBAL_SERVER_INFO...
#endif

	IOCPServer				*m_pIOCPServer;
	AccountDBProxySession	*m_pAccountDBProxySession;
	OpServerSession			*m_pOpServerSession;
	BOOL					m_bShutdown;
	DWORD					m_dwFPS;
	util::Timer				m_ReconnectTimer;
	util::SolarHashTable<SERVER_MONEY_INFO *>	*m_pServerMoneyInfoTable;

	SERVER_KEY				m_ExitServerKey;		// RTTA에서 받은 정상종료 프로토콜에서 가리키는 서버 키
	BOOL					m_bExitServer;			// 서버 정상종료중인가?
	DWORD					m_dwExitKey;			// RTT에서 보내주는 종료 키. 그대로 보관했다가 돌려줘야 한다
	__int64					m_i64WopsKey;			// RTT에서 보내주는 종료 키2. 그대로 보관했다가 돌려줘야 한다
	eEXIT_TYPE				m_eExitType;			// 종료 패킷 보낸 상태인가?
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524	// 에이전트부터 먼저 종료하는 코드
	SERVERKEY_HASH			*m_pExitAgentTable;		// 정상종료 프로토콜 받았을 때 Agent 먼저 종료시키는 부분
#endif
	SERVERKEY_HASH			*m_pExitServerTable;	// 정상종료할 서버 목록-종료 프로토콜 받는 순간 접속된 서버들을 저장
	SERVERKEY_HASH			*m_pExitDBServerTable;	// 정상종료할 GameDBProxy서버 목록-종료 프로토콜 받는 순간 접속된 서버들을 저장
	util::Timer				m_ExitTimer;			// 종료 대기용 타이머
	BOOL					m_bExitFailed;			// 종료 실패(이 상태면 이후로 패킷을 RTT로 보내지 않는다)

	WORLD_MAP				m_WorldMap;
	util::Timer				m_MoneyTimer;			// 각 Game서버에서 통화량 정보 얻어오는 주기
};

#endif // _MASTERSERVER_H_
