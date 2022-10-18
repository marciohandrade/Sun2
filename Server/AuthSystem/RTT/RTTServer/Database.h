#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>
#include <map>

#pragma warning(pop)

#include <eXtreme/dbc/dbc.h>
#include <eXtreme/threading/Thread.h>
#include <eXtreme/threading/Mutex.h>

typedef struct tag_ServerInfo
{
	tag_ServerInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	//  해당 시스템에 설치된 RTTAgent가 인증되었는지를 체크한다.
	//  0 - 정상		1 - 비접속/DB기록전		2 - 비접속/DB기록후
	DWORD		certifiedRTTAgent;

	//  해당 서버에서 패치의 유무를 확인한다.
	//  MSG_RTTG_PATCHRESULT_ANS의 에러값 참조
	DWORD		patchStatus;
	bool		bPatchCompleted;	// patch가 끝났는지 확인

	//  MSG_RTTW_LAUNCHPROCESS에 대한 각 서버의 상태.
	//  0 : 작업 진행 중,  1 : 작업 완료,  2 : 에러 발생
	DWORD		launchProcessStatus;


	//  게임 서버의 경우, 동접 수를 기록한다.

	DWORD		userCountField;
	DWORD		userCountBattle;
	DWORD		preUserCountField;
	DWORD		preUserCountBattle;
	DWORD		userCountFieldOfPC;
	DWORD		userCountBattleOfPC;
	DWORD		preUserCountFieldOfPC;
	DWORD		preUserCountBattleOfPC;

	DWORD		HangameuserCountField;			// 국내에서 한게임계정으로 접속한 동접자 정보(해외버전에서는 이 필드를 사용하지 않으며 0으로만 세팅된다)
	DWORD		HangameuserCountBattle;
	DWORD		preHangameUserCountField;
	DWORD		preHangameUserCountBattle;
	DWORD		HangameuserCountFieldOfPC;
	DWORD		HangameuserCountBattleOfPC;
	DWORD		preHangameUserCountFieldOfPC;
	DWORD		preHangameUserCountBattleOfPC;

	DWORD		PManguserCountField;			// 국내에서 피망계정으로 접속한 동접자 정보(해외버전에서는 이 필드를 사용하지 않으며 0으로만 세팅된다)
	DWORD		PManguserCountBattle;
	DWORD		prePMangUserCountField;
	DWORD		prePMangUserCountBattle;
	DWORD		PManguserCountFieldOfPC;
	DWORD		PManguserCountBattleOfPC;
	DWORD		prePMangUserCountFieldOfPC;
	DWORD		prePMangUserCountBattleOfPC;

	DWORD		agentParentHistoryIDX;
	DWORD		userParentHistoryIDX;	//
	bool		bReceiveCurrentUser;	// Master서버로부터 동접수를 주기적으로 받고 있는 체크하고 있는 변수
	//  Database schema
	wchar_t		nv_ChannelName[20];
	short		ti_WorldID;
	short		ti_ChannelID;
	short		ti_ServerType;
	short		ti_ServerID;
	wchar_t		nv_ServerName[20];
	wchar_t		vc_IP[20];
	wchar_t		i_Port[10];
	wchar_t		vc_InnerIP[20];
	wchar_t		i_InnerPort[10];
} ServerInfo;

typedef struct tag_AdminInfo
{
	//  Database schema
#ifndef _110503_CUSTOM_USERID_LENGTH
	wchar_t		UserID[12];
#else
    wchar_t		UserID[ID_MAX_LEN + 1];
#endif

	short		LV;
	wchar_t		IpAddr[15];
} AdminInfo;

typedef struct tag_WarningCanon
{
	//  Database schema
	short		ti_WorldID;
	short		ti_ChannelID;
	short		ti_ServerType;
	short		ti_ServerID;
	DWORD		i_ConnectCountUnderGap;
	DWORD		i_ConnectCountOverGap;
	DWORD		i_CpuUpperLimit;
	DWORD		i_CpuUpperFrequency;
	DWORD		i_CpuLowerLimit;
	DWORD		i_CpuLowerFrequency;
	DWORD		i_MemoryUpperLimit;
	DWORD		i_MemoryUpperFrequency;
	DWORD		i_HDDRemainder;
} WarningCanon;

typedef struct tag_UpdateTime
{
	//  Database schema
	short		ti_ConnectCountUpdateTime;
	short		ti_ResourceUpdateTime;
	short		ti_WopsRelationUpdateTime;
	short		ti_MoneyUpdateTIme;
	short		ti_MoneyGap;
} UpdateTime;

typedef struct tag_ResourceInfo
{
	//  Database schema
	short		ti_WorldID;
	short		ti_ChannelID;
	short		ti_ServerType;
	short		ti_ServerID;
	char		szServerIP[16];

	DWORD		i_CpuUseResource;
	DWORD		i_CpuMaxResource;
	DWORD		i_MemoryUseResource;
	DWORD		i_MemoryMaxResource;
	DWORD		i_HDDUseResource;
	DWORD		i_HDDMaxResource;


	DWORD		i_CpuUpperFrequency;
	DWORD		i_CpuLowerFrequency;
	DWORD		i_MemoryUpperFrequency;

	DWORD		i_CPU_ParentHistoryIDX;
	DWORD		i_MEM_ParentHistoryIDX;
	DWORD		i_HDD_ParentHistoryIDX;

	//  User definition
	bool		b_HDDAlert;			//  0 : 기록 안함, 1 : 기록 됨

	DWORD		i_WarningKey; // 장애 기록 키	
} ResourceInfo;

typedef struct tag_ServerProcess
{
	//  Database schema
	int			i_ProcessIndex;
	short		ti_WorldID;
	short		ti_ChannelID;
	short		ti_ServerType;
	short		ti_ServerID;
	char		vc_InnerIP[16];
	short		ti_ServerTypeForPatch;
	wchar_t		vc_ProcessName[100];
	wchar_t		vc_AbsolutePath[100];

	//  User definition
	DWORD		dwStatus;			//  0 : inactive, 1 : active
	DWORD		dw_alert;			//  inactive 상태를 shutdown으로 기록해야하는지 여부
									//  0 : 기록안함, 1 : 기록해야 함, 2 : 기록 됨, 3 : 복구 기록 필요
	DWORD		i_ParentHistoryIDX;	
} ServerProcess;


typedef struct tag_BattleZoneInfo
{
	//  User definition
	short		ti_worldID;
	short		ti_channelID;
	short		ti_zoneType;		//  TB_BattleZoneType 참고
	DWORD		i_roomCount;		//  방 개설수
} BattleZoneInfo;


typedef struct tag_SupplyMoney
{
	//  User definition
	short		ti_worldID;
	short		ti_channelID;

	UINT64		prevTotalGetMoney;
	UINT64		prevTotalLoseMoney;
	UINT64		prevTotalUserMoney;

	UINT64		prevWareHouseAveMoney;			
	UINT64		prevWareHouseTotalPlusMoney;		
	UINT64		prevWareHouseTotalMinusMoney;	
	
	USHORT		preJackpotCnt;
	UINT64		preJackpotMoney;

	UINT64		totalGetMoney;			// 일정 시간 동안 생겨난 돈
	UINT64		totalLoseMoney;			// 일정 시간 동안 사라진 돈
	UINT64		totalUserMoney;			// 접속 중인 유저들의 평균 보유 금액
	
	UINT64		wareHouseAveMoney;			// 창고 평균 금액
	UINT64		wareHouseTotalPlusMoney;	// 창고에 들어간 돈
	UINT64		wareHouseTotalMinusMoney;	// 창고에서 나간 돈
	
	USHORT		jackpotCnt;
	UINT64		jackpotMoney;

	UINT64		circulateMoney;
} SupplyMoney;

typedef struct tag_GameStatistics
{
	short		ti_worldID;
	short		ti_channelID;
	UINT64		ui64Statistics[eSTATISTIC_MAX + 1];
} GameStatistics;

typedef struct tag_PatchKey
{
	__int64		bi_wopsKey;			//  WOPS Component에서 부여받은 고유번호
	DWORD		i_uid;				//  WOPS 객체의 uid

	short		ti_WorldID;			//  패치를 시도한 서버 정보
	short		ti_ChannelID;
	short		ti_ServerType;		
	short		ti_ServerID;
	short		ti_ServerTypeForProcess;
} PatchKey;

typedef struct tag_WebServerRequestTestURL
{
	wchar_t		vc_URL[MAX_URL];
} WebServerRequestTestURL;

class RTTAgent;
class Database : public eXtreme::Thread
{
	Singleton( Database );

public:
	static	void		CallbackErrorDBC( eXtreme::DBC* dbc,
												HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc );
	static	void		CallbackErrorRecordSet( eXtreme::RecordSet* recordset,
												HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc );


			bool		Create();
			void		Destroy();

			//  Methods for Server list
			bool		FindServerID( wchar_t* innerIP, short *worldID, short *channelID, short *serverType, short *serverID );
			void		MakeServerList( ServerInfo* list[], DWORD bufferSize, DWORD* count );
			void		MakeServerListIn( wchar_t* innerIP, ServerInfo* list[], DWORD bufferSize, DWORD* count );
			void		MakeAdminList( AdminInfo* list[], DWORD bufferSize, DWORD* count );


			//  Methods for administrator
			void		AddAdmin( wchar_t* name, wchar_t* ip, DWORD attr );
			void		RemoveAdmin( wchar_t* name );


			//  Methods for update info to database
			void		UpdateSystemResource( ResourceInfo& info );

			// 맨 뒤의 한게임, 피망이 붙은 기본값이 지정된 8개의 파라미터는 한게임, 피망 계정을 통한 동접이므로 국내버전에서만 사용되며 해외버전에서는 0으로 세팅된다
			// Define을 통해 Function을 2개로 분리하는것보다 이게 나을것 같다는 판단에서 나온 Output임. 맘에 안들면 바꾸삼~!
			void		UpdateCurrentUserCount( DWORD worldID, DWORD channelID, DWORD field, DWORD fieldOfPCRoom , DWORD battle, DWORD battleOfPCRoom, DWORD Hangamefield = 0, DWORD HangamefieldOfPCRoom = 0 , DWORD Hangamebattle = 0, DWORD HangamebattleOfPCRoom = 0, DWORD PMangfield = 0, DWORD PMangfieldOfPCRoom = 0 , DWORD PMangbattle = 0, DWORD PMangbattleOfPCRoom = 0);

			//  Methods for etc...
			void		CheckRTTAgentCertify( wchar_t* ip, bool certified );
			void		UpdateBattleZoneInfo( short worldID, short channelID, short type, DWORD count );
			void		UpdateSupplyMoney( short worldID, short channelID, short serverType, short serverID, UINT64 get, UINT64 loss, UINT64 user, UINT64 wareHouseAvg, UINT64 wareHousePlus, UINT64 wareHouseMinus, USHORT usJackpotCnt, UINT64 ui64JackpotMoney, UINT64 ui64CirculateMoney);
			void		UpdateGameStatistics( short worldID, short channelID, short serverType, short serverID, UINT64* ui64Statistics);

			// 2006. 7. 13 김종호
			// void 타입 리턴을 bool로 수정( QueryExternal(), QueryExternal2() )
			bool		QueryExternal( char* queryStr, ... );
			bool		QueryExternal2( char* queryStr, ... );

			// 2006. 7. 24 김종호 쿼리함수 추가
			int			QueryArmorKit(int iHackLogID, int iHackType, int iGuid, char* pIP, int iHookType, char* pDllName,
										int iOrdinal, char* pApiName, DWORD_PTR OriginalAddr, DWORD_PTR CurrentAddr,
										DWORD_PTR DetourAddr,char* pOriginalOpcodes, char* pCurrentOpcodes,
										char* pDetourAddrOpcodes, DWORD_PTR ValidStartAddr, DWORD_PTR ValidEndAddr, int iErrorCode = 0);

			DWORD		GetURLList(short worldID, short channelID, short serverType, short serverID, WebServerRequestTestURL wzURLList[]);
			DWORD		QueryConnectRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError = true, DWORD dwErrorCode = 0 );	// true == 에러등록, false == 복구등록
			DWORD		QueryWebPageRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError = true );							// true == 에러등록, false == 복구등록


			void		ClearLaunchProcessStatus( short worldID, short channelID, short serverType, short serverID );
			bool		CheckLaunchProcessStatus( short worldID, short channelID, short serverType, short serverID, DWORD result, DWORD* error );


			DWORD		MakePatchKey( __int64 wopsKey, DWORD uid, short worldID, short channelID , short serverType, short serverID, short serverTypeForProcess );
			PatchKey*	GetPatchKey( DWORD patchKey );
			DWORD		SetPatchResult( wchar_t* ip, __int64 wopsKey, DWORD dwServerTypeForProcess, DWORD result, BYTE byIndex , const char* lpVersion);
			bool		IsPatchCompleted(short worldID, short channelID, short serverType, short serverID );
			void		ClearPatchKeyAll();
			void		BroadcastForPatch( char* buffer, int size );

protected:
	virtual	void		Run();

private:
			bool		Initialize();
			void		Finalize();

			void		SendErrorNotifyNTF(DWORD dwWorldID, DWORD dwChannelID, DWORD dwServerType, DWORD dwServerID, DWORD warningID, char* dateTime, char* msg, ... );
			

			void		InsertServerInfo( ServerInfo& info );
			void		InsertResoureTable( short worldID, short channelID, short serverType, short serverID, wchar_t* serverIP );
			void		InsertAdminInfo( AdminInfo& info );

			void		ClearResourceInfo();
			void		GetUpdateTime();
			void		CheckSystemResource( ResourceInfo& info, int* warning, char* dateTime  );
			void		CheckCurrentUserCount( ServerInfo& info, int* warning, char* dateTime, bool bCheckAlive  );
			//void		CheckCurrentUserCount( ServerInfo info, int* warning, char* dateTime, bool bCheckAlive  );
			void		CheckProcess( ServerProcess& info, int* warning, char* dateTime );
			void		CheckSupplyMoney( SupplyMoney& info, int* warning, char* dateTime );

			void		SP_CURRENT_RESOURCE_INSERT( char* dateTime );
			void		SP_CURRENT_USER_INSERT( char* dateTime, bool bCheckAlive );
			void		SP_CURRENT_RTTS_UPDATE( char* dateTime );
			void		SP_CURRENT_RTTAgent_INSERT( char* dateTime );
			void		SP_CURRENT_BattleZone_INSERT( char* dateTime );
			void		SP_CURRENT_Money_INSERT( char* dateTime );
			void		SP_CURRENT_GameStatistics_INSERT( char* dateTime );
						
			void		ReloadWarningCanon();


	typedef	std::list	< ServerInfo* >			ListServerInfo;
	typedef std::list	< ResourceInfo* >		ListSystemResource;
	typedef	std::list	< AdminInfo* >			ListAdminInfo;
	typedef	std::list	< WarningCanon* >		ListWarningCanon;
	typedef	std::list	< BattleZoneInfo* >		ListBattleZoneInfo;
	typedef	std::list	< SupplyMoney* >		ListSupplyMoney;
	typedef std::list	< std::string >			ListPostedQuery;
	typedef std::list	< PatchKey* >			ListPatchKey;
	typedef std::list	< GameStatistics* >		ListGameStatistics;
	typedef std::map	< DWORD, DWORD >		MapWebRequest;


			ListServerInfo						m_listServerInfo;
			ListAdminInfo						m_listAdminInfo;
			ListWarningCanon					m_listWarningCanon;
			ListSystemResource					m_listSystemResource;
			ListBattleZoneInfo					m_listBattleZoneInfo;
			ListSupplyMoney						m_listSupplyMoney;
			ListPatchKey						m_listPatchKey;
			ListGameStatistics					m_listGameStatistics;
			MapWebRequest						m_MapWebRequest;

			Statistic							m_StatisticEnumCode;

			eXtreme::Mutex						m_mutexSystemResource;
			eXtreme::Mutex						m_mutexBattleZone;
			eXtreme::Mutex						m_mutexPostedQuery;
			eXtreme::Mutex						m_mutexPatchKey;
			eXtreme::Mutex						m_mutexDBCEx;
			eXtreme::Mutex						m_mutexWarningCanon;
			eXtreme::Mutex						m_mutexWebRequest;

			eXtreme::DBC						m_dbcWops, m_dbcWopsExternal;
			ServerInfo*							m_svrInfoRTTS;
			HANDLE								m_eventKill;

			DWORD								m_gapConnectCount;
			DWORD								m_gapResource;
			DWORD								m_gapWopsRelation;
			DWORD								m_stdMoneyGap;

			bool								m_flagReconnectDB;

			char m_lpConnectString[256];
			//char m_lpMatchDBConnectString[256];
};