#ifndef _WORLDSERVER_H_
#define _WORLDSERVER_H_

#define DISP_OK		DISPMSG( " [OK]\n" );
#define DISP_FAIL	DISPMSG( " Failed!\n" );

#include <windows.h>
#include <Singleton.h>
#include <map>
#include <const.h>
#include <Timer.h>
#include <ServerStruct.h>

#ifdef _JP_0_20101123_PMS
#include <PMS/PMSManager.h>
#endif

// 맵 로딩 위해 추가
#pragma warning(push)
#pragma warning(disable : 4100 4201)
#include <3DEngine/3DTerrain/3DTerrain.h>
#pragma warning(pop)


class IOCPServer;
class Session;
//class ServerSession;
//class NetworkObject;
#include "./ServerSession.h"

class ScriptParserManager;


#ifdef _JP_0_20101123_PMS
class PMSAnnounceRequest : public IAnnounceRequest
{
    BOOL OnAnnounceReq(LPCSTR lpszMsg);
};
#endif

class WorldServer : public util::Singleton<WorldServer>
{
	enum {	CLIENT_IOHANDLER = 0,
			SERVER_IOHANDLER = 1 };

	friend NetworkObject* CallBackOnAccept( DWORD dwNetworkIndex );
	friend VOID CallBackOnDisconnect( DWORD dwSessionIndex, NetworkObject *pNetworkObject );

public:
	WorldServer();
	~WorldServer();

	VOID					UpdateServerTitle();
	BOOL					Init();
	VOID					Release();
	VOID					Run();
	BOOL					Update();
	VOID					StartListen();
	VOID					StartListen( char *szIP, WORD wPort, char *szInnerIP, WORD wInnerPort );
	VOID					ConnectToServer( NetworkObject *pNetworkObject, char *pszIP, WORD wPort );
	inline VOID				Shutdown() { m_bShutdown = TRUE; }
	inline BOOL				IsOnShutdown() { return m_bShutdown; }
	inline VOID				SetServerKey( SERVER_KEY key ) { m_ServerKey = key; 
#ifdef _JP_0_20101123_PMS
                                                             pms_manager_.SetCategory(key.m_dwKey);
#endif
                                                            }
	inline SERVER_KEY		GetServerKey() { return m_ServerKey; }
	inline DWORD			GetWorldID() { return m_ServerKey.GetWorldID(); }
	inline DWORD			GetChannelID() { return m_ServerKey.GetChannelID(); }
	inline DWORD			GetServerID() { return m_ServerKey.GetServerID(); }
#ifdef _JP_0_20101123_PMS
    inline PMSManager*      GetPMSManager() {return &pms_manager_;}
    VOID                    SetPMSSessionCount();    
    VOID                    SetPMSChannelCount(DWORD channel_count);    
#endif
	VOID					ConnectToDBProxyServer();
	BOOL					ProcessConsole();
	VOID					ReadAndSendNotice();
	BOOL					IsConnectedTo( eSERVER_TYPE eServerType );

	VOID					SetGameDBProxyServerAddr( TCHAR* tszDBProxyIP, WORD wDBProxyPort );
	VOID					ConnectToGameDBProxyServer();
	VOID					SendGameDBProxy( BYTE* pMsg, WORD wSize );
    ServerSession* GetGameDBProxy() const;

	inline eSERVER_TYPE		GetServerType() { return WORLD_SERVER; }

	HINSTANCE				m_hDll3DTerrain;	// DLL Instance
	CLoadHelper *			m_pLoadHelper;
	FUNC_ContactTerrain		m_fnContactTerrain;	// 맵 정보 연결 function

	inline BOOL				CheckShoutInterval( DWORD &dwCheckTime )
	{
		if( m_dwGlobalTick > dwCheckTime + m_dwShoutInterval )
		{
			dwCheckTime = m_dwGlobalTick;
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL				CheckWorldShoutInterval(DWORD &dwWorldCheckTime)
	{
		if(m_dwGlobalTick > dwWorldCheckTime + m_dwShoutInterval)
		{
			dwWorldCheckTime = m_dwGlobalTick;
			return TRUE;
		}

		return FALSE;
	}

	VOID					DisplayServerListInfo();
	inline DWORD			GetGlobalTick() { return m_dwGlobalTick; }

public:
	static VOID				StartWorldServer();
public:
	inline ServerSession*	GetMasterServerSession() { return m_pMasterServer; }

	inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
	inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }

private:
	ePHASE_SHIFT			m_PHASE_SHIFT;

private:
	VOID					UpdateFPS();
	VOID					DisplayServerInfo();
	VOID					MaintainConnection();
	BOOL					CreateSunLog();
	BOOL					CreateGameLog();

private:
#ifdef _JP_0_20101123_PMS
    PMSManager              pms_manager_;
#endif
	ServerSession			*m_pMasterServer;
	ServerSession			*m_pGameDBProxy;

	IOCPServer				*m_pIOCPServer;
	BOOL					m_bShutdown;
	DWORD					m_dwFPS;
	util::Timer				m_ReconnectTimer;
	SERVER_KEY				m_ServerKey;

	util::Timer				m_UserManagerTimer;

	DWORD					m_dwShoutInterval;			// 외치기 간격

	DWORD					m_dwGlobalTick;				// 프레임당 딱 한번 돌리는, 타이머를 쓰지 않는 개체들의 기준 시각

	ScriptParserManager*	m_pWorldScriptParserManager;
};

#endif // _WORLDSERVER_H_