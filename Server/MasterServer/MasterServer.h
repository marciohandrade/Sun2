#ifndef _MASTERSERVER_H_
#define _MASTERSERVER_H_

#include <windows.h>
#include <Singleton.h>
#include <map>
#include <const.h>
#include <Timer.h>

// ���� ��ȭ�� ����
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

// ���� �������� ���� Ű ����(Key = SERVER_KEY)
typedef vector<SERVER_KEY>			SERVERKEY_VEC;
typedef SERVERKEY_VEC::iterator		SERVERKEY_VEC_ITER;

// ä�� ������ �������� ���� ��� ����(Key = ChannelIndex)
typedef map<BYTE, SERVERKEY_VEC>	CHANNEL_MAP;
typedef CHANNEL_MAP::iterator		CHANNEL_MAP_ITER;
typedef pair<BYTE, SERVERKEY_VEC>	CHANNEL_MAP_PAIR;

// ���� ������ �������� ���� ��� ����(Key = WorldIndex)
typedef map<BYTE, CHANNEL_MAP>			WORLD_MAP;
typedef WORLD_MAP::iterator			WORLD_MAP_ITER;
typedef pair<BYTE, CHANNEL_MAP>		WORLD_MAP_PAIR;

//#define _WHATTHEHELL_AGENTSHUTDOWN_20060524

enum eEXIT_TYPE
{
	eEXIT_NONE,			// ���ϴ� ����
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	eEXIT_AGENTSTART,	// Agent���� ���� ����
	eEXIT_AGENTWAIT,	// ���� ��Ŷ ������ �����
#endif
	eEXIT_GAMESTART,	// Game���� ���� ����
	eEXIT_GAMEWAIT,		// ���� ��Ŷ ������ �����
	eEXIT_DBPROXY,		// GameDBProxy ���� ����
	eEXIT_DBPROXYWAIT,	// ���� ��Ŷ ������ �����
	eEXIT_END,			// ���� ��~
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

	// ���� ���� ����
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
	BOOL					AddExitServer( SERVER_KEY sServerKey );			// ������ ���� �߰�
	VOID					RemoveExitServer( SERVER_KEY serverKey	);		// ������ ���� ����
	BOOL					AddExitDBServer( SERVER_KEY sServerKey );		// ������ ����(DB) �߰�
	VOID					RemoveExitDBServer( SERVER_KEY sServerKey );	// ������ ����(DB) ����
	BOOL					CheckExitServer();								// ���� ��� �޾Ҵ��� Ȯ���ϰ�, ����Ǿ����� TRUE

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

	SERVER_KEY				m_ExitServerKey;		// RTTA���� ���� �������� �������ݿ��� ����Ű�� ���� Ű
	BOOL					m_bExitServer;			// ���� �����������ΰ�?
	DWORD					m_dwExitKey;			// RTT���� �����ִ� ���� Ű. �״�� �����ߴٰ� ������� �Ѵ�
	__int64					m_i64WopsKey;			// RTT���� �����ִ� ���� Ű2. �״�� �����ߴٰ� ������� �Ѵ�
	eEXIT_TYPE				m_eExitType;			// ���� ��Ŷ ���� �����ΰ�?
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524	// ������Ʈ���� ���� �����ϴ� �ڵ�
	SERVERKEY_HASH			*m_pExitAgentTable;		// �������� �������� �޾��� �� Agent ���� �����Ű�� �κ�
#endif
	SERVERKEY_HASH			*m_pExitServerTable;	// ���������� ���� ���-���� �������� �޴� ���� ���ӵ� �������� ����
	SERVERKEY_HASH			*m_pExitDBServerTable;	// ���������� GameDBProxy���� ���-���� �������� �޴� ���� ���ӵ� �������� ����
	util::Timer				m_ExitTimer;			// ���� ���� Ÿ�̸�
	BOOL					m_bExitFailed;			// ���� ����(�� ���¸� ���ķ� ��Ŷ�� RTT�� ������ �ʴ´�)

	WORLD_MAP				m_WorldMap;
	util::Timer				m_MoneyTimer;			// �� Game�������� ��ȭ�� ���� ������ �ֱ�
};

#endif // _MASTERSERVER_H_
