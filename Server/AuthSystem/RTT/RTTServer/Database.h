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

	//  �ش� �ý��ۿ� ��ġ�� RTTAgent�� �����Ǿ������� üũ�Ѵ�.
	//  0 - ����		1 - ������/DB�����		2 - ������/DB�����
	DWORD		certifiedRTTAgent;

	//  �ش� �������� ��ġ�� ������ Ȯ���Ѵ�.
	//  MSG_RTTG_PATCHRESULT_ANS�� ������ ����
	DWORD		patchStatus;
	bool		bPatchCompleted;	// patch�� �������� Ȯ��

	//  MSG_RTTW_LAUNCHPROCESS�� ���� �� ������ ����.
	//  0 : �۾� ���� ��,  1 : �۾� �Ϸ�,  2 : ���� �߻�
	DWORD		launchProcessStatus;


	//  ���� ������ ���, ���� ���� ����Ѵ�.

	DWORD		userCountField;
	DWORD		userCountBattle;
	DWORD		preUserCountField;
	DWORD		preUserCountBattle;
	DWORD		userCountFieldOfPC;
	DWORD		userCountBattleOfPC;
	DWORD		preUserCountFieldOfPC;
	DWORD		preUserCountBattleOfPC;

	DWORD		HangameuserCountField;			// �������� �Ѱ��Ӱ������� ������ ������ ����(�ؿܹ��������� �� �ʵ带 ������� ������ 0���θ� ���õȴ�)
	DWORD		HangameuserCountBattle;
	DWORD		preHangameUserCountField;
	DWORD		preHangameUserCountBattle;
	DWORD		HangameuserCountFieldOfPC;
	DWORD		HangameuserCountBattleOfPC;
	DWORD		preHangameUserCountFieldOfPC;
	DWORD		preHangameUserCountBattleOfPC;

	DWORD		PManguserCountField;			// �������� �Ǹ��������� ������ ������ ����(�ؿܹ��������� �� �ʵ带 ������� ������ 0���θ� ���õȴ�)
	DWORD		PManguserCountBattle;
	DWORD		prePMangUserCountField;
	DWORD		prePMangUserCountBattle;
	DWORD		PManguserCountFieldOfPC;
	DWORD		PManguserCountBattleOfPC;
	DWORD		prePMangUserCountFieldOfPC;
	DWORD		prePMangUserCountBattleOfPC;

	DWORD		agentParentHistoryIDX;
	DWORD		userParentHistoryIDX;	//
	bool		bReceiveCurrentUser;	// Master�����κ��� �������� �ֱ������� �ް� �ִ� üũ�ϰ� �ִ� ����
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
	bool		b_HDDAlert;			//  0 : ��� ����, 1 : ��� ��

	DWORD		i_WarningKey; // ��� ��� Ű	
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
	DWORD		dw_alert;			//  inactive ���¸� shutdown���� ����ؾ��ϴ��� ����
									//  0 : ��Ͼ���, 1 : ����ؾ� ��, 2 : ��� ��, 3 : ���� ��� �ʿ�
	DWORD		i_ParentHistoryIDX;	
} ServerProcess;


typedef struct tag_BattleZoneInfo
{
	//  User definition
	short		ti_worldID;
	short		ti_channelID;
	short		ti_zoneType;		//  TB_BattleZoneType ����
	DWORD		i_roomCount;		//  �� ������
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

	UINT64		totalGetMoney;			// ���� �ð� ���� ���ܳ� ��
	UINT64		totalLoseMoney;			// ���� �ð� ���� ����� ��
	UINT64		totalUserMoney;			// ���� ���� �������� ��� ���� �ݾ�
	
	UINT64		wareHouseAveMoney;			// â�� ��� �ݾ�
	UINT64		wareHouseTotalPlusMoney;	// â�� �� ��
	UINT64		wareHouseTotalMinusMoney;	// â���� ���� ��
	
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
	__int64		bi_wopsKey;			//  WOPS Component���� �ο����� ������ȣ
	DWORD		i_uid;				//  WOPS ��ü�� uid

	short		ti_WorldID;			//  ��ġ�� �õ��� ���� ����
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

			// �� ���� �Ѱ���, �Ǹ��� ���� �⺻���� ������ 8���� �Ķ���ʹ� �Ѱ���, �Ǹ� ������ ���� �����̹Ƿ� �������������� ���Ǹ� �ؿܹ��������� 0���� ���õȴ�
			// Define�� ���� Function�� 2���� �и��ϴ°ͺ��� �̰� ������ ���ٴ� �Ǵܿ��� ���� Output��. ���� �ȵ�� �ٲٻ�~!
			void		UpdateCurrentUserCount( DWORD worldID, DWORD channelID, DWORD field, DWORD fieldOfPCRoom , DWORD battle, DWORD battleOfPCRoom, DWORD Hangamefield = 0, DWORD HangamefieldOfPCRoom = 0 , DWORD Hangamebattle = 0, DWORD HangamebattleOfPCRoom = 0, DWORD PMangfield = 0, DWORD PMangfieldOfPCRoom = 0 , DWORD PMangbattle = 0, DWORD PMangbattleOfPCRoom = 0);

			//  Methods for etc...
			void		CheckRTTAgentCertify( wchar_t* ip, bool certified );
			void		UpdateBattleZoneInfo( short worldID, short channelID, short type, DWORD count );
			void		UpdateSupplyMoney( short worldID, short channelID, short serverType, short serverID, UINT64 get, UINT64 loss, UINT64 user, UINT64 wareHouseAvg, UINT64 wareHousePlus, UINT64 wareHouseMinus, USHORT usJackpotCnt, UINT64 ui64JackpotMoney, UINT64 ui64CirculateMoney);
			void		UpdateGameStatistics( short worldID, short channelID, short serverType, short serverID, UINT64* ui64Statistics);

			// 2006. 7. 13 ����ȣ
			// void Ÿ�� ������ bool�� ����( QueryExternal(), QueryExternal2() )
			bool		QueryExternal( char* queryStr, ... );
			bool		QueryExternal2( char* queryStr, ... );

			// 2006. 7. 24 ����ȣ �����Լ� �߰�
			int			QueryArmorKit(int iHackLogID, int iHackType, int iGuid, char* pIP, int iHookType, char* pDllName,
										int iOrdinal, char* pApiName, DWORD_PTR OriginalAddr, DWORD_PTR CurrentAddr,
										DWORD_PTR DetourAddr,char* pOriginalOpcodes, char* pCurrentOpcodes,
										char* pDetourAddrOpcodes, DWORD_PTR ValidStartAddr, DWORD_PTR ValidEndAddr, int iErrorCode = 0);

			DWORD		GetURLList(short worldID, short channelID, short serverType, short serverID, WebServerRequestTestURL wzURLList[]);
			DWORD		QueryConnectRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError = true, DWORD dwErrorCode = 0 );	// true == �������, false == �������
			DWORD		QueryWebPageRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError = true );							// true == �������, false == �������


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