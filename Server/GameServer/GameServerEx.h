//=============================================================================================================================
/// GameServerEx class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 9. 4
	@remark
		- 프레임워크를 상속받아 구현하는 것으로 코드 변경
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================

#pragma once

#include <ServerFrame.h>

#include <MemoryPoolFactory.h>

#include <TempServerSession.h>

#include "AgentServerSession.h"
#include "GameDBProxySession.h"
#include "GuildServerSession.h"
#include "MasterServerSession.h"
#include "PortalServerSession.h"
#include "ShopServerSession.h"
//#include "GameRankingManager.h"


#include <3DEngine/3DTerrain/3DTerrain.h>


class CFileParser;

class GameServerEx : public ServerFrame
{
public:
	static NetworkObject *	DoAllocServerSession();
	static VOID DoFreeServerSession( NetworkObject * pNetObject );
	static VOID DoFreeConnectSession( NetworkObject * pNetObject ){__UNUSED(pNetObject);}

	static NetworkObject *	DoAllocClient_NotUsed(){ return NULL; }
	static VOID DoFreeClient_NotUsed( NetworkObject * pNetObject ){__UNUSED(pNetObject);}
	static VOID DoFreeConnect_NotUsed( NetworkObject * pNetObject ){__UNUSED(pNetObject);}

public:
	GameServerEx();
	virtual ~GameServerEx();

	virtual eSERVER_TYPE	GetServerType() = 0;

	BOOL			SendToServer( eSERVER_TYPE type, MSG_BASE * pMsg, WORD wSize );
	BOOL			IsConnected( eSERVER_TYPE type );
	BOOL			ConnectTo( eSERVER_TYPE type );
	void			SetServerAddr( eSERVER_TYPE type, char * pszIP, WORD wPort );

public:		inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
			inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }
private:	ePHASE_SHIFT			m_PHASE_SHIFT;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
public:		static VOID				ReadyToStartServer();
			static VOID				GameServerSyncWithMasterServer();
			inline ServerSession*	GetMasterServerSession() { return m_pSession[MASTER_SERVER]; }
#endif //

public:
	__inline void	SetConnectToWorldServer( BOOL v ) { m_bConnectWorldServer = v; }
	__inline BOOL	IsConnectToWorldServer() 	{ return m_bConnectWorldServer; }
	ServerSession * GetSession( eSERVER_TYPE type );

protected:
	virtual BOOL	Init();
	virtual BOOL	Update( DWORD tick );
	virtual void	Release();
public:
	VOID			Finalize();
protected:
	virtual ServerSession * AllocServerSession( eSERVER_TYPE );
	virtual void FreeServerSession( ServerSession * );

	BOOL ParserInit();
	void ParserRelease();

	VOID LogicInit();

private:
	util::CMemoryPoolFactory<AgentServerSession> m_AgentServerSessionPool;
	util::CMemoryPoolFactory<GameDBProxySession> m_DBProxyServerSessionPool;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	util::CMemoryPoolFactory<GuildServerSession> m_GuildServerSessionPool;
#endif
	util::CMemoryPoolFactory<MasterServerSession> m_MasterServerSessionPool;
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	util::CMemoryPoolFactory<PortalServerSession> m_PortalServerSessionPool;
#endif
	util::CMemoryPoolFactory<TempServerSession> m_TempServerSessionPool;

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	util::CMemoryPoolFactory<ShopServerSession> m_ShopServerSessionPool;
#endif

	ServerSession *			m_pSession[MAX_SERVER];

public:
	VOID DisplayServerInfo( BOOL bLog = FALSE );
	VOID ReloadAttributes();
	VOID ReloadData();

	BOOL	IsPVPMode()			{ return m_bPVPMode; }
	VOID	SetPVPMode( BOOL bPVPMode ) { m_bPVPMode = bPVPMode; }
    int GetDBConnectorIndex() const { return db_connector_index_; };
    void ChangeDBConnectorIndex(int db_connector_index) { db_connector_index_ = db_connector_index; }

    BOOL NotifyEXPAcquisition( CHARGUID charGuid, eEXP_ACQUISITION_TYPE type, EXPTYPE amount );
    BOOL NotifyHeimAcquisition(CHARGUID charGuid, eHEIM_ACQUISITION_TYPE type, MONEY amount);
    BOOL NotifyHeimConsumption(CHARGUID charGuid, eHEIM_CONSUMPTION_TYPE type, MONEY amount, int lottoAccumRatio = -1);
public:
	FUNC_ContactTerrain		m_fnContactTerrain;

	CLoadHelper *			GetLoadHelper() { return m_pLoadHelper; }

//Log
protected:
	BOOL	GetServerTypeStr( TCHAR* pszServerType, int nSize );
	BOOL	CreateSunLog();
	BOOL	CheckServerMode( eSERVER_MODE ServerMode, eSERVER_TYPE ServerType, BYTE byServerPoolType );
    bool LoadGameShopConfiguration(); //_NA_0_20110613_GAME_SHOP_RENEWAL

private:
	// custom 
	BOOL					m_bConnectWorldServer;
    int db_connector_index_; //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
	HINSTANCE				m_hDll3DTerrain;			// 3D엔진팀의 지형엔진
	CLoadHelper *			m_pLoadHelper;
	//HINSTANCE				m_hDllNumerical;			// 3D엔진팀의 수치엔진
	util::Timer				m_ServerInfoLogTimer;		// 서버정보의 로그를 남기는 타이머.
	BOOL					m_bPVPMode;
    
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    public:
        CSunLog* battle_ground_log_;
        bool MessageBattleGroundOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
        CSunLog* mondschein_log_;
        BOOL MessageMondscheinOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
#ifdef _NA_008334_20150608_SONNENSCHEIN
        CSunLog* sonnenschein_log_;
        BOOL MessageSonnenScheinOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
        CSunLog* max_damage_log;
        BOOL MessageMaxDamageOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
#ifdef _NA_008706_20160229_TEXTLOG_PK
        CSunLog* battle_monitor_log_;   // 특정 유저들의 전투 감시 목적 텍스트로그
        BOOL MessageBattleMonitorOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#endif //_NA_008706_20160229_TEXTLOG_PK
};

extern GameServerEx * g_pGameServer;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#define SUNBATTLEGROUNDLOG if(g_pGameServer) g_pGameServer->MessageBattleGroundOut
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
#define SUNMONDSCHIENLOG if(g_pGameServer) g_pGameServer->MessageMondscheinOut
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
#ifdef _NA_008334_20150608_SONNENSCHEIN
#define SUNSONNENSCHEINLOG if(g_pGameServer) g_pGameServer->MessageSonnenScheinOut
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
#define SUNMAXDAMAGELOG if(g_pGameServer) g_pGameServer->MessageMaxDamageOut
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
#ifdef _NA_008706_20160229_TEXTLOG_PK
#define SUNBATTLEMONITORLOG if(g_pGameServer) g_pGameServer->MessageBattleMonitorOut
#endif //_NA_008706_20160229_TEXTLOG_PK