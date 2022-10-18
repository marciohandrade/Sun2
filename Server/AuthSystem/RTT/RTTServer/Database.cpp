#include "stdafx.h"

#include "Entry.h"
#include "RTTAgent.h"
#include "Database.h"
#include "MonitorProcess.h"
#include "ManagerWops.h"

#define  MASTER_CHECK_TIME			30000	// master 서버에서 동접값 전송을 체크하기 위한 시간 주기
//  에러 메시지 목록
#define  CPU_UPPER_LIMIT			2001		//  CPU 사용률 상한 초과
#define  CPU_LOWER_LIMIT			2002		//  CPU 사용률 하한 미만
#define  MEMORY_UPPER_LIMIT			2003		//  메모리 상한 초과
#define  HDD_LEAST_LIMIT			2005		//  HDD 용량 부족

#define  CPU_NORMAL					2011		//  CPU 장애 복구
#define  MEM_NORMAL					2013		//  메모리 장애 복구
#define  HDD_NORMAL					2015		//  HDD 용량 장애 복구

#define  USERCOUNT_GAPOVER			3001		//  동접수 폭증
#define  USERCOUNT_GAPLOWER			3002		//  동접수 폭감

#define  MASTER_SERVER_ERROR		3003		//  Master 서버 장애 발생
#define  MASTER_SERVER_SUCCESS		3013		//  Master 서버 장애 복구

#define  RTTS_NOT_RESPONSE			4001		//  RTTS 응답 없음
#define  RTTA_NOT_RESPONSE			4002		//  RTTA 응답 없음

#define  RTTA_RESPONSE				4012		//  RTTA 응답 없음

#define  PROCESS_SHUTDOWN			5001		//  Process 중지
#define  PROCESS_STARTUP			5011		//  Process 중지 장애 복구

#define  SUPPLYMONEY_ERROR			6001		//  통화량 장애

#define  PATCH_FTPCONNECT_FAIL		7001		//  패치 중 FTP 연결 실패
#define  PATCH_DOWNLOAD_FAIL		7002		//  패치 중 파일 다운로드 실패
#define  PATCH_FILESEARCH_FAIL		7003		//  패치 중 파일찾기 실패
#define  PATCH_PROCESSRUNNING		7004		//  패치 대상 프로세스가 실행중
#define  PATCH_SUCCESS				7777		//  패치 대상 성공

#define  WEB_CONNECT_FAIL			8001		//  웹 서버 연결 실패
#define  WEB_CONNECT_RESTORE		8002		//  웹 서버 연결 성공
#define  WEB_REQUEST_FAIL			8003		//  HTTP 요청 실패
#define  WEB_REQUEST_RESTORE		8004		//  HTTP 요청 성공

//  에러 문자열 목록
#define  MSG_CPU_UPPER_LIMIT		"(CPU = %d%%)"
#define  MSG_CPU_LOWER_LIMIT		"(CPU = %d%%)"
#define  MSG_MEMORY_UPPER_LIMIT		"(Memory = %d/%dMB (%d%% using))"
#define  MSG_HDD_LEAST_LIMIT		"(HDD = %d/%dMB (%d%% using))"
#define  MSG_USERCOUNT_GAPOVER		"(%d → %d)"
#define  MSG_USERCOUNT_GAPLOWER		"(%d → %d)"
#define  MSG_RTTA_NOT_RESPONSE		""
#define  MSG_RTTA_RESPONSE		    ""
#define  MSG_PROCESS_SHUTDOWN		"(%s)"
#define  MSG_SUPPLYMONEY_ERROR		"(PerGetMoney = %I64d, CurGetMoney = %I64d, Rate of Increase = %d%%)"

#define  MSG_WEB_CONNECT_FAIL		"(커넥션 장애)"
#define	 MSG_WEB_CONNECT_RESTORE	"(커넥션 장애 복구)"
#define  MSG_WEB_REQUEST_FAIL		"(Page Fault 장애)"
#define  MSG_WEB_REQUEST_RESTORE	"(Page Fault 장애 복구)"

#define CONNECT_STRING "DSN=%s;Initial Catalog=%s;User ID=%s;Password=%s;Max Pool Size=100;Min Pool Size=30;Pooling=yes;"


void Database::CallbackErrorDBC( eXtreme::DBC* dbc, HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc )
{
	if(result==E_FAIL)
	{
		dbc->m_lastQuery = "";
		dbc->Close();
	}

	UINT uiQueryLength = dbc->m_lastQuery.length();

	OutputDebug( "[DBC Error]\n" );
	OutputDebug( "Code = %08lx\n", result );
	OutputDebug( "Means = %s(%d)\n", ( char* )msg, dbc->m_lastQuery.length() );
	
	/*
		OutputDeubug에 buffer는 4096이므로 query가 크기를 넘어서는 것을 방지한다.
	*/
	if(uiQueryLength<=4000)
	{
		OutputDebug( "Query = %s\n", ( char* )dbc->m_lastQuery );	
	}
	else
	{
		char szQuery[4096] = {0, };
		strncpy(szQuery, (char*)dbc->m_lastQuery, 4000);
		OutputDebug( "Query = %s\n", ( char* )szQuery );			
	}

	OutputDebug( "Description = %s\n", ( char* )desc );
}

void Database::CallbackErrorRecordSet( eXtreme::RecordSet* recordset,
												HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc )
{
	if(result==E_FAIL)
	{
		recordset->m_lastQuery = "";
	}

	OutputDebug( "[RecordSet Error]\n" );
	OutputDebug( "Code = %08lx\n", result );
	OutputDebug( "Means = %s\n", ( char* )msg );
	OutputDebug( "Query = %s\n", ( char* )recordset->m_lastQuery );
	OutputDebug( "Description = %s\n", ( char* )desc );
}

class TB_ServerInfo : public eXtreme::RecordSet
{
public:
			ServerInfo		record;

public:
			TB_ServerInfo()
			{
				m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( record.nv_ChannelName, "nv_ChannelName" );
				BindVariable( &record.ti_WorldID, "ti_WorldID" );
				BindVariable( &record.ti_ChannelID, "ti_ChannelID" );
				BindVariable( &record.ti_ServerType, "ti_ServerType" );
				BindVariable( &record.ti_ServerID, "ti_ServerID" );
				BindVariable( record.nv_ServerName, "nv_ServerName" );
				BindVariable( record.vc_IP, "vc_IP" );
				BindVariable( record.i_Port, "i_Port" );
				BindVariable( record.vc_InnerIP, "vc_InnerIP" );
				BindVariable( record.i_InnerPort, "i_InnerPort" );
			}
};

class T_Manager : public eXtreme::RecordSet
{
public:
			AdminInfo			record;

public:
			T_Manager()
			{
				m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( record.UserID, "UserID" );
				BindVariable( &record.LV, "LV" );
				BindVariable( &record.IpAddr, "IpAddr" );
			}
};

class TB_WarningCanon : public eXtreme::RecordSet
{
public:
			WarningCanon		record;

public:
			TB_WarningCanon()
			{
				m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( &record.ti_WorldID, "ti_WorldID" );
				BindVariable( &record.ti_ChannelID, "ti_ChannelID" );
				BindVariable( &record.ti_ServerType, "ti_ServerType" );
				BindVariable( &record.ti_ServerID, "ti_ServerID" );
				BindVariable( &record.i_ConnectCountUnderGap, "i_ConnectCountUnderGap" );
				BindVariable( &record.i_ConnectCountOverGap, "i_ConnectCountOverGap" );
				BindVariable( &record.i_CpuUpperLimit, "i_CpuUpperLimit" );
				BindVariable( &record.i_CpuUpperFrequency, "i_CpuUpperFrequency" );
				BindVariable( &record.i_CpuLowerLimit, "i_CpuLowerLimit" );
				BindVariable( &record.i_CpuLowerFrequency, "i_CpuLowerFrequency" );
				BindVariable( &record.i_MemoryUpperLimit, "i_MemoryUpperLimit" );
				BindVariable( &record.i_MemoryUpperFrequency, "i_MemoryUpperFrequency" );
				BindVariable( &record.i_HDDRemainder, "i_HDDRemainder" );
			}
};

class TB_UpdateTime : public eXtreme::RecordSet
{
public:
			UpdateTime			record;

public:
			TB_UpdateTime()
			{
				m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( &record.ti_ConnectCountUpdateTime, "ti_ConnectCountUpdateTime" );
				BindVariable( &record.ti_ResourceUpdateTime, "ti_ResourceUpdateTime" );
				BindVariable( &record.ti_WopsRelationUpdateTime, "ti_WopsRelationUpdateTime" );
				BindVariable( &record.ti_MoneyUpdateTIme, "ti_MoneyUpdateTIme" );
				BindVariable( &record.ti_MoneyGap, "ti_MoneyGap" );
			}
};

class TB_ServerProcess : public eXtreme::RecordSet
{
public:
			ServerProcess		record;

public:
			TB_ServerProcess()
			{
				m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( &record.i_ProcessIndex       , "i_ProcessIndex"        );
				BindVariable( &record.ti_WorldID           , "ti_WorldID"            );
				BindVariable( &record.ti_ChannelID         , "ti_ChannelID"          );
				BindVariable( &record.ti_ServerType        , "ti_ServerType"         );
				BindVariable( &record.ti_ServerID          , "ti_ServerID"           );
				BindVariable( &record.ti_ServerTypeForPatch, "ti_ServerTypeForPatch" );
				BindVariable( &record.vc_ProcessName       , "vc_ProcessName"        );
				BindVariable( &record.vc_AbsolutePath      , "vc_AbsolutePath"       );
			}
};

class TB_WebServerRequestTestURL : public eXtreme::RecordSet
{
public:
			WebServerRequestTestURL	record;
public:
			TB_WebServerRequestTestURL()
			{
                m_callbackError = Database::CallbackErrorRecordSet;

				BindVariable( &record.vc_URL       , "vc_URL"        );
			}
};


Database::Database()
	: m_svrInfoRTTS( NULL ), m_eventKill( INVALID_HANDLE_VALUE ), m_gapConnectCount(0), m_gapResource(0),
	  m_gapWopsRelation(0), m_stdMoneyGap(0), m_flagReconnectDB( true )
{
}

Database::~Database()
{
}

bool Database::Create()
{
	m_eventKill			= ::CreateEvent( 0, 0, 0, 0 );
	m_flagReconnectDB	= !Initialize();

	Start();

	//  DB 연결에 실패하였는지를 반환한다.
	return ( m_flagReconnectDB == false );
}

void Database::Destroy()
{
	//  Thread를 종료시킨다.
	::SetEvent( m_eventKill );
	if( WaitForTerminate( 1000 ) == false )
		Terminate( 0 );

	::CloseHandle( m_eventKill );
	m_eventKill = INVALID_HANDLE_VALUE;


	//  Data를 Release한다.
	Finalize();

	m_flagReconnectDB	= false;
}

bool Database::Initialize()
{
	OutputDebug("[Database::Initialize] Start\n");
	m_flagReconnectDB = true;

	//  ★ Database에 연결한다. 일단은 ODBC의 DSN을 사용해야한다.
	//  IP로 직접 연결은 왜 안되는걸까;;;
	{
		//LPCSTR szConnStr = "Provider=sqloledb;Data Source=10.101.203.201,6279;Network Library=DBMSSOCN;Initial Catalog=WopsRTTSunConnectLogKor;User ID=wops;Password=@wops!1;";

		char	dsn[20]			= {0, };
		char	initialDB[40]	= {0, };
		char	account[20]		= {0, };
		char	password[20]	= {0, };
		
		::GetPrivateProfileString( "DBSetting", "DSN", "WopsAdmin", dsn, 20, SERVERINFO_FILENAME );
		::GetPrivateProfileString( "DBSetting", "InitialDB", "WopsRTTSunConnectLogKor", initialDB, 40, SERVERINFO_FILENAME );
		::GetPrivateProfileString( "DBSetting", "Account", "sa", account, 20, SERVERINFO_FILENAME );
		::GetPrivateProfileString( "DBSetting", "Password", "rmfnqdnpdj", password, 20, SERVERINFO_FILENAME );

		m_dbcWops.m_callbackError = CallbackErrorDBC;
		
		memset(m_lpConnectString, 0x00, 256);
		sprintf(m_lpConnectString, CONNECT_STRING, dsn, initialDB, account, password);
		
		if( m_dbcWops.Open( dsn, initialDB, account, password, 30 ) == false )
		{
			OutputDebug( "Cannot connect to Database(WopsAdmin)\n" );
			return false;
		}

		m_dbcWopsExternal.m_callbackError = CallbackErrorDBC;
		if( m_dbcWopsExternal.Open( dsn, initialDB, account, password ) == false )
		{
			OutputDebug( "Cannot connect to Database(WopsAdmin)\n" );
			return false;
		}

		OutputDebug( "Connected Database(WopsAdmin)\n" );
	}


//	try
//	{
		//  서버 목록
		{
			TB_ServerInfo		record;
			int					count = 0;

			m_dbcWops.Query( &record, "SELECT nv_ChannelName, ti_WorldID, ti_ChannelID, ti_ServerType, ti_ServerID, "
								"nv_ServerName, vc_IP, i_Port, vc_InnerIP, i_InnerPort FROM TB_ServerInfo" );

			while( record.IsEndRecord() == false )
			{
				
				count++;
				InsertServerInfo( record.record );
				InsertResoureTable( record.record.ti_WorldID, record.record.ti_ChannelID, record.record.ti_ServerType, record.record.ti_ServerID, record.record.vc_InnerIP );


				//  프로세스 감시 목록
				{
					TB_ServerProcess	recordProcess;
					char				tmp[127] = {0, };

					sprintf( tmp, "{call USP_GetServerProcess( %d, %d, %d, %d )}",
										record.record.ti_WorldID, record.record.ti_ChannelID,
										record.record.ti_ServerType, record.record.ti_ServerID );

					m_dbcWops.Query( &recordProcess, tmp );


					while( recordProcess.IsEndRecord() == false )
					{
						MonitorProcess::Instance()->AddProcess( recordProcess.record.ti_WorldID, 
																recordProcess.record.ti_ChannelID,
																recordProcess.record.ti_ServerType, 
																recordProcess.record.ti_ServerID,
																record.record.vc_InnerIP,
																recordProcess.record.ti_ServerTypeForPatch,
																recordProcess.record.vc_ProcessName, 
																recordProcess.record.vc_AbsolutePath );

						recordProcess.Next();
					}
				}


				//  경고 기준 테이블
				{
					eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexWarningCanon );
					TB_WarningCanon		recordCanon;
					char				tmp[256] = {0, };

					sprintf( tmp, "{call USP_GetWarningCanonPerServer( %d, %d, %d, %d )}",
										record.record.ti_WorldID, record.record.ti_ChannelID,
										record.record.ti_ServerType, record.record.ti_ServerID );

					m_dbcWops.Query( &recordCanon, tmp );

					if( recordCanon.IsEndRecord() == false )		//  각 서버당 1개의 기준값을 갖는다.
					{
						WarningCanon*	canon = new WarningCanon;

						memcpy( canon, &recordCanon.record, sizeof( WarningCanon ) );
						m_listWarningCanon.push_back( canon );
					}
				}


				//  배틀서버 목록
				if( record.record.ti_ServerType == 3 && record.record.ti_ServerID == 1 )
				{
					eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexBattleZone );

					{	//  1 - 헌팅방
						BattleZoneInfo*		info = new BattleZoneInfo;

						info->ti_worldID	= record.record.ti_WorldID;
						info->ti_channelID	= record.record.ti_ChannelID;
						info->ti_zoneType	= 1;
						info->i_roomCount	= 0;

						m_listBattleZoneInfo.push_back( info );
					}
					{	//  2 - 미션방
						BattleZoneInfo*		info = new BattleZoneInfo;

						info->ti_worldID	= record.record.ti_WorldID;
						info->ti_channelID	= record.record.ti_ChannelID;
						info->ti_zoneType	= 2;
						info->i_roomCount	= 0;

						m_listBattleZoneInfo.push_back( info );
					}
					{	//  3 - PVP존
						BattleZoneInfo*		info = new BattleZoneInfo;

						info->ti_worldID	= record.record.ti_WorldID;
						info->ti_channelID	= record.record.ti_ChannelID;
						info->ti_zoneType	= 3;
						info->i_roomCount	= 0;

						m_listBattleZoneInfo.push_back( info );
					}
					/*
						[2006.04.26] modify by guiun 
						경쟁 헌팅방 추가
					*/
					{	//  4 - 경쟁헌팅존
						BattleZoneInfo*		info = new BattleZoneInfo;

						info->ti_worldID	= record.record.ti_WorldID;
						info->ti_channelID	= record.record.ti_ChannelID;
						info->ti_zoneType	= 4;
						info->i_roomCount	= 0;

						m_listBattleZoneInfo.push_back( info );
					}
				}

				record.Next();
			}

			OutputDebug( "Server list updated(Total %d).\n", count );
		}

		//  관리자 목록
		{
			T_Manager		record;
			int				count = 0;

			m_dbcWops.Query( &record, "SELECT UserID, LV, IpAddr FROM T_Manager" );

			while( record.IsEndRecord() == false )
			{
				InsertAdminInfo( record.record );
				record.Next();
				count++;
			}

			OutputDebug( "Administrator list updated(Total %d).\n\n", count );
		}

		//  Update 주기
		GetUpdateTime();
//	}
//	catch( eXtreme::ExceptionDBC& )
//	{
//		return false;
//	}


	Entry::Instance()->ServiceStartUp();

	m_flagReconnectDB = false;

	OutputDebug("[Database::Initialize] End\n");
	return true;
}

void Database::Finalize()
{
	OutputDebug("[Database::Finalize] Start\n");
	Entry::Instance()->ServiceTerminate();

	//  서버 목록 삭제
	{
		ServerInfo*					info = NULL;

		for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listServerInfo.clear();
	}

	//  통화량 목록 삭제
	{
		SupplyMoney*				info = NULL;

		for(ListSupplyMoney::iterator iter = m_listSupplyMoney.begin() ; iter != m_listSupplyMoney.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listSupplyMoney.clear();
	}
	
	// 게임 통게 목록 삭제
	{	
		GameStatistics*				info = NULL;
		for(ListGameStatistics::iterator iter = m_listGameStatistics.begin() ; iter != m_listGameStatistics.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listGameStatistics.clear();		
	}

	//  배틀존 목록 삭제
	{
		eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexBattleZone );
		BattleZoneInfo*					info = NULL;

        for(ListBattleZoneInfo::iterator iter = m_listBattleZoneInfo.begin() ; iter != m_listBattleZoneInfo.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listBattleZoneInfo.clear();
	}

	//  관리자 목록 삭제
	{
		AdminInfo*					info = NULL;

		for(ListAdminInfo::iterator iter = m_listAdminInfo.begin() ; iter != m_listAdminInfo.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listAdminInfo.clear();
	}

	//  경고 기준 테이블 삭제
	{
		eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexWarningCanon );
		WarningCanon*				info = NULL;

		for(ListWarningCanon::iterator iter = m_listWarningCanon.begin() ; iter != m_listWarningCanon.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listWarningCanon.clear();
	}

	//  System Resource 목록 삭제
	{
		eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexSystemResource );

		ResourceInfo*					info = NULL;

		for(ListSystemResource::iterator iter = m_listSystemResource.begin() ; iter != m_listSystemResource.end() ; iter++ )
		{
			info = *iter;

			delete info;
		}

		m_listSystemResource.clear();
	}

	//  Patch Key 목록 삭제
	ClearPatchKeyAll();
	/*
	{
		eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );
		ListPatchKey::iterator		iter;
		PatchKey*					info;

		for( iter = m_listPatchKey.begin() ; iter != m_listPatchKey.end() ; iter++ )
		{
			info = *iter;
			delete info;
		}

		m_listPatchKey.clear();
	}
	*/

	m_dbcWops.Close();
	m_dbcWopsExternal.Close();
	
	m_flagReconnectDB = true;

	OutputDebug("[Database::Finalize] End\n");
}

// 2006. 7. 13 김종호
// void 타입 리턴을 bool로 수정
bool Database::QueryExternal( char* queryStr, ... )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexDBCEx );

	va_list		args;
	char		buffer[1024] = {0, };

	va_start( args, queryStr );
	vsprintf( buffer, queryStr, args );
	va_end( args );

//	m_dbcWopsExternal.Query( 0, buffer );

	if( !m_dbcWopsExternal.Query( 0, buffer ) )
	{
		return false;
	}

	return true;
}

// 2006. 7. 13 김종호
// void 타입 리턴을 bool로 수정
bool Database::QueryExternal2( char* queryStr, ... )
{
	//eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexDBCEx );

	va_list		args;
	char		buffer[2048] = {0, };

	va_start( args, queryStr );
	vsprintf( buffer, queryStr, args );
	va_end( args );
	{
//		eXtreme::DBC dbConn;
//		dbConn.Open(m_lpConnectString);
//		dbConn.m_callbackError = Database::CallbackErrorDBC;
//		dbConn.Query( 0, buffer );

		eXtreme::DBC dbConn;

		if( !dbConn.Open(m_lpConnectString) )
			return false;

		dbConn.m_callbackError = Database::CallbackErrorDBC;

		if( !dbConn.Query( 0, buffer ) )
			return false;
	}

	return true;
}

// 2006. 7. 24 김종호 쿼리함수 추가
int Database::QueryArmorKit(int iHackLogID, int iHackType, int iGuid, char* pIP, int iHookType, char* pDllName,
							int iOrdinal, char* pApiName, DWORD_PTR OriginalAddr, DWORD_PTR CurrentAddr,
							DWORD_PTR DetourAddr, char* pOriginalOpcodes, char* pCurrentOpcodes,
							char* pDetourAddrOpcodes, DWORD_PTR ValidStartAddr, DWORD_PTR ValidEndAddr, int iErrorCode)
{
	eXtreme::DBC		dbConn;
	eXtreme::RecordSet	rec;
	
	if( !dbConn.Open(m_lpConnectString) ) return false;

	dbConn.m_callbackError = Database::CallbackErrorDBC;
	rec.m_callbackError = Database::CallbackErrorRecordSet;

	int iLogID = 0;

	char pOriginalAddr[20]		= {0, };
	char pCurrentAddr[20]		= {0, };
	char pDetourAddr[20]		= {0, };
	char pValidStartAddr[20]	= {0, };
	char pValidEndAddr[20]		= {0, };

	sprintf(pOriginalAddr	, "%d", OriginalAddr);
	sprintf(pCurrentAddr	, "%d", CurrentAddr);
	sprintf(pDetourAddr		, "%d", DetourAddr);
	sprintf(pValidStartAddr	, "%d", ValidStartAddr);
	sprintf(pValidEndAddr	, "%d", ValidEndAddr);


/*-------- char 문자열이 "" 일 경우 DB의 NULL과는 다르므로 ---------------*/
/*-------- 문자열 길이를 체크해서 0바이트면 빈 문자 한개를 넣어준다 ------*/
	if( !pIP || !strlen(pIP) )
		pIP = " ";

	if( !pDllName || !strlen(pDllName) )
		pDllName = " ";

	if( !pApiName || !strlen(pApiName) )
		pApiName = " ";

	if( !pOriginalOpcodes || !strlen(pOriginalOpcodes) )
        pOriginalOpcodes = " ";

	if( !pCurrentOpcodes || !strlen(pCurrentOpcodes) )
		pCurrentOpcodes = " ";

	if( !pDetourAddrOpcodes || !strlen(pDetourAddrOpcodes) )
		pDetourAddrOpcodes = " ";
/*----------------------------------------------------- 여기까지... :-) --*/


	// LogID return 값을 받는다.
	rec.BindParameter(&iLogID, "", adInteger, adParamReturnValue, sizeof(int), "");

	// input parameter bind
	rec.BindParameter(NULL, "@HackLogID"        , adInteger, adParamInput, 4								, _bstr_t(iHackLogID));
	rec.BindParameter(NULL, "@HackType"			, adInteger, adParamInput, 4								, _bstr_t(iHackType));
	rec.BindParameter(NULL, "@Guid"				, adInteger, adParamInput, 4								, _bstr_t(iGuid));
	rec.BindParameter(NULL, "@IP"				, adVarChar, adParamInput, (long)strlen(pIP)				, _bstr_t(pIP));
	rec.BindParameter(NULL, "@HookType"			, adInteger, adParamInput, 4								, _bstr_t(iHookType));
	rec.BindParameter(NULL, "@DllName"			, adVarChar, adParamInput, (long)strlen(pDllName)			, _bstr_t(pDllName));
	rec.BindParameter(NULL, "@Ordinal"			, adInteger, adParamInput, 4								, _bstr_t(iOrdinal));
	rec.BindParameter(NULL, "@ApiName"			, adVarChar, adParamInput, (long)strlen(pApiName)			, _bstr_t(pApiName));
	rec.BindParameter(NULL, "@OriginalAddr"     , adVarChar, adParamInput, (long)strlen(pOriginalAddr)		, _bstr_t(pOriginalAddr));
	rec.BindParameter(NULL, "@CurrentAddr"		, adVarChar, adParamInput, (long)strlen(pCurrentAddr)		, _bstr_t(pCurrentAddr));
	rec.BindParameter(NULL, "@DetourAddr"       , adVarChar, adParamInput, (long)strlen(pDetourAddr)		, _bstr_t(pDetourAddr));
	rec.BindParameter(NULL, "@OriginalOpcodes"  , adVarChar, adParamInput, (long)strlen(pOriginalOpcodes)	, _bstr_t(pOriginalOpcodes));
	rec.BindParameter(NULL, "@CurrentOpcodes"   , adVarChar, adParamInput, (long)strlen(pCurrentOpcodes)	, _bstr_t(pCurrentOpcodes));
	rec.BindParameter(NULL, "@DetourAddrOpcodes", adVarChar, adParamInput, (long)strlen(pDetourAddrOpcodes)	, _bstr_t(pDetourAddrOpcodes));
	rec.BindParameter(NULL, "@ValidStartAddr"   , adVarChar, adParamInput, (long)strlen(pValidStartAddr)	, _bstr_t(pValidStartAddr));
	rec.BindParameter(NULL, "@ValidEndAddr"     , adVarChar, adParamInput, (long)strlen(pValidEndAddr)		, _bstr_t(pValidEndAddr));
	rec.BindParameter(NULL, "@ErrorCode"        , adInteger, adParamInput, 4								, _bstr_t(iErrorCode));

	// call storedProcedure
	dbConn.CallStoredProcedure(&rec, "USP_AddHackLog_HOOK_DETECT");

	//eXtreme::DBC dbConn;

	//if( !dbConn.Open(m_lpConnectString) )
	//	return false;

	//dbConn.m_callbackError = Database::CallbackErrorDBC;

	//if( !dbConn.Query( 0, buffer ) )
	//	return false;

	
	if( dbConn.GetState() == adStateOpen )
	{
        dbConn.Close();
	}

	return iLogID;
}

DWORD Database::GetURLList(short worldID, short channelID, short serverType, short serverID, WebServerRequestTestURL wzURLList[])
{
	TB_WebServerRequestTestURL		record;
	char				tmp[127] = {0, };

    sprintf( tmp, "{call USP_GetWebServerRequestTestURL( %d, %d, %d, %d )}", worldID, channelID, serverType, serverID );

	m_dbcWops.Query( &record, tmp );

	DWORD dwCount = 0;
	while( record.IsEndRecord() == false )
	{
		wcscpy( wzURLList[dwCount++].vc_URL, record.record.vc_URL );
		record.Next();
	}

	return dwCount;
}

DWORD Database::QueryConnectRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError, DWORD dwErrorCode )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexWebRequest );

	char dateTime[128] = {0 };		// 현재 날짜
	char szBuffer[96] = {0, };		// 장애 or 복구 저장시에 추가할 메시지
	DWORD dwRetValue = 0;
	DWORD dwWarning = 0;
	DWORD key = UID( worldID, channelID, serverType, serverID ); // 서버 키를 구한다

	{// 현재 날짜
		SYSTEMTIME	sysTime;
		::GetLocalTime( &sysTime );
		sprintf( dateTime, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	}

	if( bError )
	{
		m_MapWebRequest[key] = WEB_CONNECT_FAIL;		// 맵에 에러정보 저장
        dwWarning = WEB_CONNECT_FAIL;
		sprintf(&szBuffer[0], MSG_WEB_CONNECT_FAIL);
	}
	else
	{
		if( 0 == dwErrorCode )
		{
			m_MapWebRequest.erase(key);						// 맵에서 에러정보 삭제
		}
		else
		{
            m_MapWebRequest[key] = WEB_REQUEST_FAIL;		// 맵에 에러정보 저장
		}
		dwWarning = WEB_CONNECT_RESTORE;
		sprintf(&szBuffer[0], MSG_WEB_CONNECT_RESTORE);
	}

	eXtreme::DBC		dbConn;
	eXtreme::RecordSet	rec;

	// Return Value Bind
	rec.BindParameter(&dwRetValue, "", adInteger, adParamReturnValue, sizeof(int), "");
	// input parameter bind
	rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(worldID   ));
	rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(channelID ));
	rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(serverType));
	rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(serverID  ));
	rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(dwWarning          ));
	rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
	rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
	rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 1		       , _bstr_t(" "          ));
	rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 1	           , _bstr_t(" "              ));
	rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(dwParentHistoryIndex));

	if( !dbConn.Open(m_lpConnectString) )
	{
		OutputDebug("[QueryConnectRequestState] DB Not Connect!\n");
		return 0;
	}

	dbConn.m_callbackError = Database::CallbackErrorDBC;
	rec.m_callbackError = Database::CallbackErrorRecordSet;

	// call storedProcedure
	m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");

	return ( true == bError ? dwRetValue : 0 );
}

DWORD Database::QueryWebPageRequestState( short worldID, short channelID, short serverType, short serverID, DWORD dwParentHistoryIndex, bool bError )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexWebRequest );

	char dateTime[128] = {0 };		// 현재 날짜
	char szBuffer[96] = {0, };		// 장애 or 복구 저장시에 추가할 메시지
	DWORD dwRetValue = 0;
	DWORD dwWarning = 0;
	DWORD key = UID( worldID, channelID, serverType, serverID ); // 서버 키를 구한다

	{// 현재 날짜
		SYSTEMTIME	sysTime;
		::GetLocalTime( &sysTime );
		sprintf( dateTime, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	}

	if( bError )
	{
		m_MapWebRequest[key] = WEB_REQUEST_FAIL;		// 맵에 에러정보 저장
        dwWarning = WEB_REQUEST_FAIL;
		sprintf(&szBuffer[0], MSG_WEB_REQUEST_FAIL);
	}
	else
	{
		m_MapWebRequest.erase(key);						// 맵에서 에러정보 삭제
        dwWarning = WEB_REQUEST_RESTORE;
		sprintf(&szBuffer[0], MSG_WEB_REQUEST_RESTORE);
	}

	eXtreme::DBC		dbConn;
	eXtreme::RecordSet	rec;

	// Return Value Bind
	rec.BindParameter(&dwRetValue, "", adInteger, adParamReturnValue, sizeof(int), "");
	// input parameter bind
	rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(worldID   ));
	rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(channelID ));
	rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(serverType));
	rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(serverID  ));
	rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(dwWarning          ));
	rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
	rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
	rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 1		       , _bstr_t(" "          ));
	rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 1	           , _bstr_t(" "              ));
	rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(dwParentHistoryIndex));

    if( !dbConn.Open(m_lpConnectString) )
	{
		OutputDebug("[QueryWebPageRequestState] DB Not Connect!\n");
		return 0;
	}

	dbConn.m_callbackError = Database::CallbackErrorDBC;
	rec.m_callbackError = Database::CallbackErrorRecordSet;

	// call storedProcedure
	m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");

	return ( true == bError ? dwRetValue : 0 );
}

void Database::SendErrorNotifyNTF(DWORD dwWorldID, DWORD dwChannelID, DWORD dwServerType, DWORD dwServerID, DWORD warningID, char* dateTime, char* msg, ... )
{
//	if( m_svrInfoRTTS == NULL ) return;
//
//#define NOTIFY_WSDL		"http://%s/RTT/SUN/WebService/wsRTTUser/wsConnect.asmx?wsdl"
//
//	char buffer[256];memset(buffer, 0x00, 256);
//	{
//		va_list args;
//		va_start( args, msg );
//		vsprintf( buffer, msg, args );
//		va_end( args );
//	}
//	char wsdl_uri[128];
//	sprintf(wsdl_uri, NOTIFY_WSDL, ManagerWSSession::Instance()->getIP());
//
//	SOAPMessageFactory soap;
//	
//	soap.initalize(wsdl_uri, 80);
//	soap.setSoapAction("WOPS.RTT.WebSerivce/wpTriggerNotify2");
//	
//	soap.addParameter("worldID"			, dwWorldID		);
//	soap.addParameter("channelID"		, dwChannelID	);
//	soap.addParameter("serverType"		, dwServerType	);
//	soap.addParameter("serverID"		, dwServerID	);
//	soap.addParameter("warningDetailID"	, warningID		);
//	soap.addParameter("message"			, buffer		);
//	soap.addParameter("date"           	, dateTime		);
//	soap.addParameter("gameID"         	, ManagerWSSession::Instance()->getGameID());
//	soap.addParameter("serviceID"		, ManagerWSSession::Instance()->getServiceID());	
//
//	ManagerWSSession::Instance()->putMessage(soap.invoke());
//
//	OutputDebug( "[Error Notify to WOPS] %d.%d.%d.%d : warning=%d[%s]%s\n",
//			dwWorldID, 
//			dwChannelID, 
//			dwServerType, 
//			dwServerID,
//			warningID, 
//			buffer, 
//			dateTime);

	va_list						args;
	char						buffer[256] = {0, };
	MSG_RTTW_ERRORNOTIFY_NTF	sendPacket;

	va_start( args, msg );
	vsprintf( buffer, msg, args );
	va_end( args );

	
	sendPacket.dwWorldID			= dwWorldID;
	sendPacket.dwChannelID			= dwChannelID;
	sendPacket.dwServerType			= dwServerType;
	sendPacket.dwServerID			= dwServerID;
	sendPacket.dwWarningDetailID	= warningID;

	::MultiByteToWideChar( CP_ACP, 0, buffer, -1, sendPacket.szMessage, 120 );
	::MultiByteToWideChar( CP_ACP, 0, dateTime, -1, sendPacket.szDateTime, 20 );
	
	if(dwWorldID==0 && dwChannelID==0 && dwServerType==0 && dwServerID==0){
		OutputDebug( "[Error Notify to WOPS] %d.%d.%d.%d : warning=%d\n%s   %s\n",
			sendPacket.dwWorldID, 
			sendPacket.dwChannelID, 
			sendPacket.dwServerType, 
			sendPacket.dwServerID,
			sendPacket.dwWarningDetailID, 
			buffer, 
			dateTime);
	}
	else{	//  연결 유지로 설정된 Wops session으로 보내야 한다.
		Wops* wops = ManagerWops::Instance()->GetSustenance();
		if( wops != 0 ){
			wops->SendMessage( sendPacket );
		}
		else{
			OutputDebug( "Sustenance WOPS connection is not establish.\n" );
		}
	}
}

void Database::InsertServerInfo( ServerInfo& info )
{
	ServerInfo*	newInfo = new ServerInfo;

	memcpy( newInfo, &info, sizeof( ServerInfo ) );

	newInfo->certifiedRTTAgent		= 1;
	newInfo->patchStatus			= MSG_RTTG_PATCHRESULT_ANS::ERROR_NONE;
	newInfo->launchProcessStatus	= 0;

	newInfo->userCountField		= 0;
	newInfo->userCountBattle	= 0;
	newInfo->preUserCountField	= 0;
	newInfo->preUserCountBattle	= 0;

	newInfo->userCountFieldOfPC		= 0;
	newInfo->userCountBattleOfPC	= 0;
	newInfo->preUserCountFieldOfPC	= 0;
	newInfo->preUserCountBattleOfPC	= 0;

	newInfo->HangameuserCountField			= 0;
	newInfo->HangameuserCountBattle			= 0;
	newInfo->preHangameUserCountField		= 0;
	newInfo->preHangameUserCountBattle		= 0;

	newInfo->HangameuserCountFieldOfPC		= 0;
	newInfo->HangameuserCountBattleOfPC		= 0;
	newInfo->preHangameUserCountFieldOfPC	= 0;
	newInfo->preHangameUserCountBattleOfPC	= 0;

	newInfo->PManguserCountField			= 0;
	newInfo->PManguserCountBattle			= 0;
	newInfo->prePMangUserCountField			= 0;
	newInfo->prePMangUserCountBattle		= 0;

	newInfo->PManguserCountFieldOfPC		= 0;
	newInfo->PManguserCountBattleOfPC		= 0;
	newInfo->prePMangUserCountFieldOfPC		= 0;
	newInfo->prePMangUserCountBattleOfPC	= 0;

	newInfo->bReceiveCurrentUser = true;
	newInfo->userParentHistoryIDX = 0;
	newInfo->agentParentHistoryIDX = 0;

	m_listServerInfo.push_back( newInfo );


	//  RTTS는 따로 구분한다.
	if( newInfo->ti_ServerType == (short)::GetPrivateProfileInt( "RTTServer", "RTTS_ServerType", 0, SERVERINFO_FILENAME )
		&& newInfo->ti_ServerID == (short)::GetPrivateProfileInt( "RTTServer", "RTTS_ServerID", 0, SERVERINFO_FILENAME ) ){
		m_svrInfoRTTS = newInfo;
	}
}

void Database::InsertAdminInfo( AdminInfo& info )
{
	AdminInfo*	newInfo = new AdminInfo;

	wcscpy( newInfo->UserID, info.UserID );
	wcscpy( newInfo->IpAddr, info.IpAddr );
	newInfo->LV = info.LV;

	m_listAdminInfo.push_back( newInfo );
}

bool Database::FindServerID( wchar_t* innerIP, short *worldID, short *channelID, short *serverType, short *serverID )
{
	ServerInfo*					info = NULL;

	//  DB와의 연결상태를 체크한다.
	if( m_flagReconnectDB == true )
		return false;


	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;

		if( wcscmp( innerIP, info->vc_InnerIP ) == 0 )
		{
			*worldID	= info->ti_WorldID;
			*channelID	= info->ti_ChannelID;
			*serverType	= info->ti_ServerType;
			*serverID	= info->ti_ServerID;

			OutputDebug( "[Database::FindServerID] IP(%s) w(%d)c(%d)s(%d)i(%d).\n", innerIP, *worldID, *channelID, *serverType, *serverID);
			return true;
		}
	}

	*worldID	= -1;
	*channelID	= -1;
	*serverType	= -1;
	*serverID	= -1;
	OutputDebug( "[Database::FindServerID] IP(%s) w(%d)c(%d)s(%d)i(%d).\n", innerIP, *worldID, *channelID, *serverType, *serverID);
	return false;
}

void Database::MakeServerList( ServerInfo* list[], DWORD bufferSize, DWORD* count )
{
	*count = 0;
	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		if( *count >= bufferSize )
			return;

		list[*count] = *iter;
		( *count )++;
	}
}

void Database::MakeServerListIn( wchar_t* innerIP, ServerInfo* list[], DWORD bufferSize, DWORD* count )
{
	ServerInfo*					info;

	*count = 0;
	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		if( *count >= bufferSize )
			return;

		info = *iter;

		if( wcscmp( innerIP, info->vc_InnerIP ) == 0 )
		{
			list[*count] = info;
			( *count )++;
		}
	}
}

void Database::MakeAdminList( AdminInfo* list[], DWORD bufferSize, DWORD* count )
{
	*count = 0;
	for(ListAdminInfo::iterator iter = m_listAdminInfo.begin() ; iter != m_listAdminInfo.end() ; iter++ )
	{
		if( *count >= bufferSize )
			return;

		list[*count] = *iter;
		( *count )++;
	}
}

void Database::AddAdmin( wchar_t* name, wchar_t* ip, DWORD attr )
{
	AdminInfo*				info = NULL;
	AdminInfo				newInfo = {0, };

	for(ListAdminInfo::iterator iter = m_listAdminInfo.begin() ; iter != m_listAdminInfo.end() ; iter++ )
	{
		info = *iter;

		if( wcscmp( info->UserID, name ) == 0 )
			return;
	}


	//  DB Procedure call
//	try
//	{
		char		mbcs[1024]	= {0, };
		char		UserID[256] = {0, };
		char		IpAddr[256] = {0, };

		::WideCharToMultiByte( CP_ACP, 0, name, -1, UserID, 256, 0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, ip, -1, IpAddr, 256, 0, 0 );

		sprintf( mbcs, "INSERT INTO T_Manager VALUES('%s', %d, '%s', GetDate())", UserID, attr, IpAddr );

		m_dbcWops.Query( 0, mbcs );

//	}
//	catch( eXtreme::ExceptionDBC& )
//	{
//		return;
//	}


	//  Insert info to buffer
	wcscpy( newInfo.UserID, name );
	wcscpy( newInfo.IpAddr, ip );
	newInfo.LV = ( short )attr;

	InsertAdminInfo( newInfo );
}

void Database::RemoveAdmin( wchar_t* name )
{
	AdminInfo*				info = NULL;

	for(ListAdminInfo::iterator iter = m_listAdminInfo.begin() ; iter != m_listAdminInfo.end() ; iter++ )
	{
		info = *iter;

		if( wcscmp( info->UserID, name ) == 0 )
		{
			m_listAdminInfo.erase( iter );
			delete info;

			//  DB Procedure call
//			try
//			{
				char		mbcs[1024]	= {0, };
				char		UserID[256] = {0, };;

				::WideCharToMultiByte( CP_ACP, 0, name, -1, UserID, 256, 0, 0 );

				sprintf( mbcs, "DELETE FROM T_Manager WHERE UserID='%s'", UserID );

				m_dbcWops.Query( 0, mbcs );

//			}
//			catch( eXtreme::ExceptionDBC& )
//			{
//				break;
//			}

			break;
		}
	}
}

void Database::CheckRTTAgentCertify( wchar_t* ip, bool certified )
{
	ServerInfo*					info = NULL;

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;

		if( wcsncmp( info->vc_InnerIP, ip, 20 ) == 0 )
		{
			if( certified == true )
			{
				info->certifiedRTTAgent		= 0;

				/*
				info->preUserCountBattle	= 0;
				info->preUserCountField		= 0;
				info->userCountBattle		= 0;
				info->userCountField		= 0;
				*/
			}
			else
			{
				{
                    eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexWebRequest );
					DWORD key = UID( info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID);
					m_MapWebRequest.erase(key);						// 맵에서 에러정보 삭제
				}

				info->certifiedRTTAgent		= 1;
				/*
				info->preUserCountBattle	= 0;
				info->preUserCountField		= 0;
				info->userCountBattle		= 0;
				info->userCountField		= 0;
				*/
			}
		}
	}
}

void Database::ReloadWarningCanon()
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexWarningCanon );
	TB_WarningCanon						record;
	char								tmp[256] = {0, };

	for(ListWarningCanon::iterator iter = m_listWarningCanon.begin() ; iter != m_listWarningCanon.end() ; iter++ )
	{
		WarningCanon*		info = *iter;

		sprintf( tmp, "{call USP_GetWarningCanonPerServer( %d, %d, %d, %d )}",
							info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID );

		m_dbcWops.Query( &record, tmp );
		if( record.IsEndRecord() == false )
			memcpy( info, &record.record, sizeof( WarningCanon ) );
	}
}

void Database::InsertResoureTable( short worldID, short channelID, short serverType, short serverID, wchar_t* serverIP )
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexSystemResource );
	ResourceInfo*						info = new ResourceInfo;

	memset( info, 0, sizeof( ResourceInfo ) );
	info->ti_WorldID		= worldID;
	info->ti_ChannelID		= channelID;
	info->ti_ServerType		= serverType;
	info->ti_ServerID		= serverID;

	char ip[16] = {0, };
	::WideCharToMultiByte( CP_ACP, 0, serverIP, -1, ip, 16, 0, 0 );

	strncpy(info->szServerIP, ip, 16);
	info->szServerIP[15] = '\0';

	info->b_HDDAlert		= false;

	m_listSystemResource.push_back( info );
}

void Database::UpdateBattleZoneInfo( short worldID, short channelID, short type, DWORD count )
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexBattleZone );
	BattleZoneInfo*						info = NULL;

	for(ListBattleZoneInfo::iterator iter = m_listBattleZoneInfo.begin() ; iter != m_listBattleZoneInfo.end() ; iter++ )
	{
		info = *iter;

		if( info->ti_worldID == worldID && info->ti_channelID == channelID && info->ti_zoneType == type )
		{
			info->i_roomCount	= count;
			break;
		}
	}
}

void Database::UpdateSupplyMoney( short worldID, short channelID, short /*serverType*/, short /*serverID*/, UINT64 get, UINT64 loss, UINT64 user, UINT64 wareHouseAvg, UINT64 wareHousePlus, UINT64 wareHouseMinus, USHORT usJackpotCnt, UINT64 ui64JackpotMoney, UINT64 ui64CirculateMoney)
{
	SupplyMoney*				info = NULL;
	bool						updated = false;

	for(ListSupplyMoney::iterator iter = m_listSupplyMoney.begin() ; iter != m_listSupplyMoney.end() ; iter++ )
	{
		info = *iter;

		if( info->ti_worldID == worldID && info->ti_channelID == channelID )
		{
			info->prevTotalGetMoney		= info->totalGetMoney;
			info->prevTotalLoseMoney	= info->totalLoseMoney;
			info->prevTotalUserMoney	= info->totalUserMoney;

			info->prevWareHouseAveMoney			= info->wareHouseAveMoney;
			info->prevWareHouseTotalPlusMoney	= info->wareHouseTotalPlusMoney;
			info->prevWareHouseTotalMinusMoney	= info->wareHouseTotalMinusMoney;
			
			//info->preJackpotCnt	= info->jackpotCnt;
			//info->preJackpotMoney	= info->jackpotMoney;

			info->totalGetMoney		= get;
			info->totalLoseMoney	= loss;
			info->totalUserMoney	= user;

			info->wareHouseAveMoney			= wareHouseAvg;
			info->wareHouseTotalPlusMoney	= wareHousePlus;
			info->wareHouseTotalMinusMoney	= wareHouseMinus;

			info->jackpotCnt = info->jackpotCnt + usJackpotCnt;
			info->jackpotMoney	+= ui64JackpotMoney;

			info->circulateMoney += ui64CirculateMoney;

			updated = true;
			break;
		}
	}

	//  목록에 해당 world & channel 정보가 존재하지 않는다면
	//  새로 할당하도록 한다.
	if( updated == false )
	{
		info = new SupplyMoney;

		info->ti_worldID			= worldID;
		info->ti_channelID			= channelID;
		info->totalGetMoney			= get;
		info->totalLoseMoney		= loss;
		info->totalUserMoney		= user;
		info->prevTotalGetMoney		= get;
		info->prevTotalLoseMoney	= loss;
		info->prevTotalUserMoney	= user;

		info->wareHouseAveMoney				= wareHouseAvg;
		info->wareHouseTotalPlusMoney		= wareHousePlus;
		info->wareHouseTotalMinusMoney		= wareHouseMinus;
		info->prevWareHouseAveMoney			= wareHouseAvg;
		info->prevWareHouseTotalPlusMoney	= wareHousePlus;
		info->prevWareHouseTotalMinusMoney	= wareHouseMinus;

		info->preJackpotCnt		= usJackpotCnt;
		info->preJackpotMoney	= ui64JackpotMoney;
		info->jackpotCnt		= usJackpotCnt;
		info->jackpotMoney		= ui64JackpotMoney;
		
		info->circulateMoney    = ui64CirculateMoney;

		m_listSupplyMoney.push_back( info );
	}
}

void Database::UpdateGameStatistics( short worldID, short channelID, short /*serverType*/, short /*serverID*/, UINT64* ui64Statistics)
{
	GameStatistics*				info = NULL;
	bool						updated = false;

	for(ListGameStatistics::iterator iter = m_listGameStatistics.begin() ; iter != m_listGameStatistics.end() ; iter++ )
	{
		info = *iter;

		if( info->ti_worldID == worldID && info->ti_channelID == channelID )
		{
			// 통계 값 업데이트
			for(int i=0;i<eSTATISTIC_MAX + 1;i++){
				info->ui64Statistics[i] += ui64Statistics[i];
			}

			updated = true;
			break;
		}
	}

	//  목록에 해당 world & channel 정보가 존재하지 않는다면
	//  새로 할당하도록 한다.
	if( updated == false )
	{
		info = new GameStatistics;
		info->ti_worldID			= worldID;
		info->ti_channelID			= channelID;
		
		for(int i=0;i<eSTATISTIC_MAX + 1;i++){
			info->ui64Statistics[i] = ui64Statistics[i];
		}
		m_listGameStatistics.push_back( info );
	}

	//OutputDebug("[Database::UpdateGameStatistics](%d,%d)\n", worldID, channelID);
	//OutputDebug("[Database::UpdateGameStatistics](%d,%d,%d,%d)\n", worldID, channelID, serverType, serverID);
}

void Database::UpdateSystemResource( ResourceInfo& info )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexSystemResource );

	ResourceInfo*					systemInfo = NULL;

	for(ListSystemResource::iterator iter = m_listSystemResource.begin() ; iter != m_listSystemResource.end() ; iter++ )
	{
		systemInfo = *iter;

		if( systemInfo->ti_WorldID == info.ti_WorldID && systemInfo->ti_ChannelID == info.ti_ChannelID &&
			systemInfo->ti_ServerType == info.ti_ServerType && systemInfo->ti_ServerID == info.ti_ServerID )
		{
			systemInfo->i_CpuMaxResource	= info.i_CpuMaxResource;
			systemInfo->i_CpuUseResource	= info.i_CpuUseResource;
			systemInfo->i_MemoryMaxResource	= info.i_MemoryMaxResource;
			systemInfo->i_MemoryUseResource	= info.i_MemoryUseResource;
			systemInfo->i_HDDMaxResource	= info.i_HDDMaxResource;
			systemInfo->i_HDDUseResource	= info.i_HDDUseResource;
			
			break;
		}
	}
}

void Database::UpdateCurrentUserCount( DWORD worldID, DWORD channelID, DWORD field, DWORD fieldOfPCRoom , DWORD battle, DWORD battleOfPCRoom, DWORD Hangamefield, DWORD HangamefieldOfPCRoom, DWORD Hangamebattle, DWORD HangamebattleOfPCRoom, DWORD PMangfield, DWORD PMangfieldOfPCRoom, DWORD PMangbattle, DWORD PMangbattleOfPCRoom)
{
	ServerInfo*					serverInfo = NULL;
	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		serverInfo = *iter;
		if( serverInfo->ti_WorldID == (short)worldID && serverInfo->ti_ChannelID == (short)channelID && serverInfo->ti_ServerID == 1)
		{
			if( serverInfo->ti_ServerType == 2 )			//  Field
			{
				serverInfo->userCountField				= field;
				serverInfo->userCountFieldOfPC			= fieldOfPCRoom;
				serverInfo->HangameuserCountField		= Hangamefield;
				serverInfo->HangameuserCountFieldOfPC	= HangamefieldOfPCRoom;
				serverInfo->PManguserCountField			= PMangfield;
				serverInfo->PManguserCountFieldOfPC		= PMangfieldOfPCRoom;
			}
			else if( serverInfo->ti_ServerType == 3 )		//  Battle
			{
				serverInfo->userCountBattle				= battle;
				serverInfo->userCountBattleOfPC			= battleOfPCRoom;
				serverInfo->HangameuserCountBattle		= Hangamebattle;
				serverInfo->HangameuserCountBattleOfPC	= HangamebattleOfPCRoom;
				serverInfo->PManguserCountBattle		= PMangbattle;
				serverInfo->PManguserCountBattleOfPC	= PMangbattleOfPCRoom;
			}
			else
			{
				continue;
			}

			// 동접 값을 받고 있음.
			serverInfo->bReceiveCurrentUser = true;
			
			/*
			OutputDebug("[Database::UpdateCurrentUserCount]w(%d)c(%d)st(%d)si(%d) field(%5d/%5d, %5d/%5d, %5d/%5d) battle(%5d/%5d, %5d/%5d, %5d/%5d)\n"
				,serverInfo->ti_WorldID
				,serverInfo->ti_ChannelID
				,serverInfo->ti_ServerType
				,serverInfo->ti_ServerID
				,field
				,fieldOfPCRoom
				,Hangamefield
				,HangamefieldOfPCRoom
				,PMangfield
				,PMangfieldOfPCRoom
				,battle
				,battleOfPCRoom
				,Hangamebattle
				,HangamebattleOfPCRoom
				,PMangbattle
				,PMangbattleOfPCRoom
				);
			//*/
		}
	}
}

void Database::CheckSystemResource( ResourceInfo& info, int* warning, char* dateTime )
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexWarningCanon );
	WarningCanon*				warningDef = NULL;
	
	//  요청된 리소스의 장애 기준값을 찾는다.
	*warning	= 0;
	for(ListWarningCanon::iterator iter = m_listWarningCanon.begin() ; iter != m_listWarningCanon.end() ; iter++ )
	{
		warningDef = *iter;

		if( warningDef->ti_WorldID == info.ti_WorldID && warningDef->ti_ChannelID == info.ti_ChannelID &&
			warningDef->ti_ServerType == info.ti_ServerType && warningDef->ti_ServerID == info.ti_ServerID )
			break;

		warningDef = NULL;
	}


	if( NULL == warningDef )		//  기준값이 존재하지 않는다.
		return;


	if( info.i_CpuMaxResource == 0 && info.i_CpuUseResource == 0 &&
		info.i_HDDMaxResource == 0 && info.i_HDDUseResource == 0 &&
		info.i_MemoryMaxResource == 0 && info.i_MemoryUseResource == 0 )
		return;

/*
	2006.04.10
	modify by guiun	

	리소스 장애처리 변경
	장애는 한번 로그를 남긴고 정상 상태로 복구가 되면 다시 로그를 남긴다.
	parentHistorIDX를 같이 남겨야 한다.	
*/
	//  CPU 상한값
	UINT cpuWarning = 0;
	UINT memWarning = 0;
	UINT hddWarning = 0;

    char szBuffer[96] = {0, };
	if( info.i_CpuUseResource >= warningDef->i_CpuUpperLimit ){
		info.i_CpuUpperFrequency++;
		if(info.i_CpuUpperFrequency>=warningDef->i_CpuUpperFrequency){	// 같은 경우에만 처리를 한다.
			cpuWarning = CPU_UPPER_LIMIT;
		}
	}
	else{	
		if(info.i_CpuUpperFrequency>=warningDef->i_CpuUpperFrequency){ // 정상 상태로 돌아온 경우			
			cpuWarning = CPU_NORMAL;			
		}
		info.i_CpuUpperFrequency = 0;
	}

	if((cpuWarning==CPU_UPPER_LIMIT && info.i_CpuUpperFrequency==warningDef->i_CpuUpperFrequency) || cpuWarning==CPU_NORMAL){
		sprintf(szBuffer, MSG_CPU_UPPER_LIMIT, info.i_CpuUseResource);
		eXtreme::RecordSet rec;
		// return parameter bind
		
		if(cpuWarning==CPU_UPPER_LIMIT){	// 장애 보고 경우에만 return 값을 받는다.
			rec.BindParameter(&info.i_CPU_ParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
		}
		// input parameter bind
		rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(info.ti_WorldID   ));
		rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ChannelID ));
		rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerType));
		rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerID  ));
		rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(cpuWarning        ));
		rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
		rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
		rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(info.i_CPU_ParentHistoryIDX));
		// call storedProcedure
		m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");
		if(cpuWarning==CPU_NORMAL){	// 장애 복구 보고인 경우에는 ParentHistory를 0 으로 설정한다.
			info.i_CPU_ParentHistoryIDX = 0;
		}
		else{
			SendErrorNotifyNTF(	info.ti_WorldID,
								info.ti_ChannelID,
								info.ti_ServerType,
								info.ti_ServerID,
								cpuWarning, 
								dateTime, 
								MSG_CPU_UPPER_LIMIT, 
								info.i_CpuUseResource );
		}
	}

	
	//  메모리 상한값
	DWORD	percentageMem = (info.i_MemoryMaxResource > 0 ) ? info.i_MemoryUseResource * 100 / info.i_MemoryMaxResource : 0;
	DWORD	percentageHDD = (info.i_HDDMaxResource    > 0 ) ? info.i_HDDUseResource    * 100 / info.i_HDDMaxResource    : 0;
	
	memset(szBuffer, 0x00, 96);
	if(percentageMem >= warningDef->i_MemoryUpperLimit){
		info.i_MemoryUpperFrequency++;
		if(info.i_MemoryUpperFrequency >= warningDef->i_MemoryUpperFrequency){
			memWarning					= MEMORY_UPPER_LIMIT;
		}
	}
	else{
		if(info.i_MemoryUpperFrequency>=warningDef->i_MemoryUpperFrequency){ // 정상 상태로 돌아온 경우			
			memWarning = MEM_NORMAL;
		}
		info.i_MemoryUpperFrequency	= 0;
	}

	if((memWarning==MEMORY_UPPER_LIMIT && info.i_MemoryUpperFrequency==warningDef->i_MemoryUpperFrequency) || memWarning==MEM_NORMAL){
		sprintf(szBuffer, MSG_MEMORY_UPPER_LIMIT, info.i_MemoryUseResource, info.i_MemoryMaxResource, percentageMem);
		eXtreme::RecordSet rec;
		// return parameter bind
		
		if(memWarning==MEMORY_UPPER_LIMIT){	// 장애 보고 경우에만 return 값을 받는다.
			rec.BindParameter(&info.i_MEM_ParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
		}
		// input parameter bind
		rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(info.ti_WorldID   ));
		rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ChannelID ));
		rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerType));
		rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerID  ));
		rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(memWarning        ));
		rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
		rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
		rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(info.i_MEM_ParentHistoryIDX));
		// call storedProcedure
		m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");
		
		if(memWarning==MEM_NORMAL){	// 장애 복구 보고인 경우에는 ParentHistory를 0 으로 설정한다.
			info.i_MEM_ParentHistoryIDX = 0;
		}
		else{
            SendErrorNotifyNTF(	info.ti_WorldID,
								info.ti_ChannelID,
								info.ti_ServerType,
								info.ti_ServerID,
								memWarning, 
								dateTime, 
								MSG_MEMORY_UPPER_LIMIT, 
								info.i_MemoryUseResource, 
								info.i_MemoryMaxResource, 
								percentageMem);
		}
	}

	//  HDD 용량
	memset(szBuffer, 0x00, 96);
	/*
	if(info.b_HDDAlert==false){
		if(info.i_HDDMaxResource > 0 && info.i_HDDMaxResource - info.i_HDDUseResource <= warningDef->i_HDDRemainder){	// 장애가 발생한 경우
			hddWarning = HDD_LEAST_LIMIT;
			info.b_HDDAlert = true;
		}
	}
	else{	// 정상 상태가 된 경우
		if((info.i_HDDMaxResource > 0 && info.i_HDDMaxResource - info.i_HDDUseResource <= warningDef->i_HDDRemainder )==false){
			hddWarning = HDD_NORMAL;			
			info.b_HDDAlert = false;
		}
	}
	*/
	// 장애인 경우
	if(info.i_HDDMaxResource > 0 && info.i_HDDMaxResource - info.i_HDDUseResource <= warningDef->i_HDDRemainder){	// 장애가 발생한 경우
		hddWarning = HDD_LEAST_LIMIT;		
	}
	
	// 장애 복구 알림이 필요한 상태 임을 확인한다.
	if(info.b_HDDAlert==true){	// 정상 상태가 된 경우
		if((info.i_HDDMaxResource > 0 && info.i_HDDMaxResource - info.i_HDDUseResource <= warningDef->i_HDDRemainder )==false){
			hddWarning = HDD_NORMAL;						
		}
	}

	if((hddWarning==HDD_LEAST_LIMIT && info.b_HDDAlert==false) || (hddWarning==HDD_NORMAL && info.b_HDDAlert==true)){
		sprintf(szBuffer, MSG_HDD_LEAST_LIMIT, info.i_HDDUseResource, info.i_HDDMaxResource, percentageHDD);
		eXtreme::RecordSet rec;
		// return parameter bind
		
		if(hddWarning==HDD_LEAST_LIMIT){	// 장애 보고 경우에만 return 값을 받는다.
			rec.BindParameter(&info.i_HDD_ParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
		}
		// input parameter bind
		rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(info.ti_WorldID   ));
		rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ChannelID ));
		rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerType));
		rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerID  ));
		rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(hddWarning        ));
		rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
		rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
		rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 1               , _bstr_t(" "               ));
		rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(info.i_HDD_ParentHistoryIDX));
		// call storedProcedure
		m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");
		
		if(hddWarning==HDD_NORMAL){	// 장애 복구 보고인 경우에는 ParentHistory를 0 으로 설정한다.
			info.i_HDD_ParentHistoryIDX = 0;
			hddWarning = 0;
		}
		else{
			SendErrorNotifyNTF(	info.ti_WorldID,
								info.ti_ChannelID,
								info.ti_ServerType,
								info.ti_ServerID,
								hddWarning, 
								dateTime, 
								MSG_HDD_LEAST_LIMIT, 
								info.i_HDDUseResource, 
								info.i_HDDMaxResource, 
								percentageHDD);			
		}
		info.b_HDDAlert = !info.b_HDDAlert;
	}

	// 장애인 경우 리턴 장애 코드를 해당 장애로 설정 해야 한다.
	// 임의적인 우선순위로 장애 코드를 리턴 한다.
	if(cpuWarning==CPU_UPPER_LIMIT){
		*warning = cpuWarning;
		
	}
	else if(memWarning==MEMORY_UPPER_LIMIT){
		*warning = memWarning;		
	}
	else if(hddWarning==HDD_LEAST_LIMIT){
		*warning = hddWarning;
	}
	else{
		*warning = 0;
	}
}

void Database::CheckCurrentUserCount(ServerInfo& info, int* warning, char* dateTime, bool bCheckAlive )
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexWarningCanon );

	DWORD dwCurrentCount = 0;
	DWORD dwPrevCount = 0;
	
	*warning = 0;
	/*
		[2006.04.18] modify by guiun
	*/
	// 동접 값 변동에 대한 확인은 우선은 PC방과 개인 동접에 합을 가지고 처리를 한다.
	if( info.ti_ServerType == 2 )
	{
		// 필드 동접
		dwCurrentCount	= info.userCountField		+ info.userCountFieldOfPC		+ info.HangameuserCountField		+ info.HangameuserCountFieldOfPC		+ info.PManguserCountField		+ info.PManguserCountFieldOfPC;
		dwPrevCount		= info.preUserCountField	+ info.preUserCountFieldOfPC	+ info.preHangameUserCountField		+ info.preHangameUserCountFieldOfPC		+ info.prePMangUserCountField	+ info.prePMangUserCountFieldOfPC;
	}
	else if( info.ti_ServerType == 3 )
	{
		// 배틀 동접
		dwCurrentCount	= info.userCountBattle		+ info.userCountBattleOfPC		+ info.HangameuserCountBattle		+ info.HangameuserCountBattleOfPC		+ info.PManguserCountBattle		+ info.PManguserCountBattleOfPC;
		dwPrevCount		= info.preUserCountBattle	+ info.preUserCountBattleOfPC	+ info.preHangameUserCountBattle	+ info.preHangameUserCountBattleOfPC	+ info.prePMangUserCountBattle  + info.prePMangUserCountBattleOfPC;
	}
	else		//이 경우는 존재 하지 않는다.
	{
		return;
	}

	//  prevCount == 0 이면 서비스를 시작한 순간이므로, 이 경우 오류로그를 판단하지 않는다.
	if(dwPrevCount==0 && info.userParentHistoryIDX==0)
	{
		// OutputDebug("[Database::CheckCurrentUserCount] Service initialize, dwPrevCount==0, return\n");
		return;
	}
	
	if( info.ti_ServerType == 2 )
	{
		/*
		OutputDebug("[Database::CheckCurrentUserCount]w(%d)c(%d)st(%d)si(%d) field_(%5d/%5d, %5d/%5d, %5d/%5d)  pre_field_(%5d/%5d, %5d/%5d, %5d/%5d) (%7d/%7d)\n"
						, info.ti_WorldID
						, info.ti_ChannelID
						, info.ti_ServerType
						, info.ti_ServerID
						, info.userCountField
						, info.userCountFieldOfPC
						, info.HangameuserCountField
						, info.HangameuserCountFieldOfPC
						, info.PManguserCountField
						, info.PManguserCountFieldOfPC
						, info.preUserCountField
						, info.preUserCountFieldOfPC
						, info.preHangameUserCountField
						, info.preHangameUserCountFieldOfPC
						, info.prePMangUserCountField
						, info.prePMangUserCountFieldOfPC
						, dwCurrentCount
						, dwPrevCount);
		//*/
	}
	else if( info.ti_ServerType == 3 )
	{
		/*
		OutputDebug("[Database::CheckCurrentUserCount]w(%d)c(%d)st(%d)si(%d) battle(%5d/%5d, %5d/%5d, %5d/%5d)  pre_battle(%5d/%5d, %5d/%5d, %5d/%5d) (%7d/%7d)\n"
						, info.ti_WorldID
						, info.ti_ChannelID
						, info.ti_ServerType
						, info.ti_ServerID
						, info.userCountBattle
						, info.userCountBattleOfPC
						, info.HangameuserCountBattle
						, info.HangameuserCountBattleOfPC
						, info.PManguserCountField
						, info.PManguserCountFieldOfPC
						, info.preUserCountBattle
						, info.preUserCountBattleOfPC
						, info.preHangameUserCountBattle
						, info.preHangameUserCountBattleOfPC
						, info.prePMangUserCountField
						, info.prePMangUserCountFieldOfPC
						, dwCurrentCount
						, dwPrevCount);
		//*/
	}

	///////////////////////////////////////////////
	//  요청된 리소스의 장애 기준값을 찾는다.
	WarningCanon*				warningDef = NULL;
	for(ListWarningCanon::iterator iter = m_listWarningCanon.begin() ; iter != m_listWarningCanon.end() ; iter++ )
	{
		warningDef = *iter;
		if( warningDef->ti_WorldID == info.ti_WorldID ) break;
		warningDef = NULL;
	}

	if( NULL == warningDef)
	{
		return;	//  기준값이 존재하지 않는다.
	}

	char szMsgFormat[32] = {0, };	
	DWORD dwWarning = 0;
	
	// 마스터 동접값 체크와 동접 체크가 주기가 다르므로 만약 에러시에는 다음과 같이 인위적으로 마스터 장애를 체크 하게 한다.
	if(bCheckAlive==false && info.userParentHistoryIDX>0)
	{
		bCheckAlive = true;
	}
	/*
		[2006.04.18] add by guiun
		Master 서버에서 동접 값을 전송 하고 있는지 확인을 한다.
	*/
	DWORD dwParentHistoryIDX = 0;
	if(bCheckAlive==true)
	{ 
		if(info.bReceiveCurrentUser==false)	// 동접 값을 받지 못하고 있는 경우
		{
			if(info.userParentHistoryIDX==0)	// userParentHistoryIDX가 0인 경우는 아직 해당 에러가 발생되지 않았다는 것이다.
			{
				dwWarning = MASTER_SERVER_ERROR;
				strncpy(szMsgFormat, " ", 1);

				// 동접 값을 받고 있지 못하므로 값을 초기화 한다.
				info.userCountBattle		= 0;
				info.userCountField			= 0;
				info.userCountBattleOfPC	= 0;
				info.userCountFieldOfPC		= 0;

				info.preUserCountBattle		= 0;
				info.preUserCountField		= 0;
				info.preUserCountBattleOfPC	= 0;
				info.preUserCountFieldOfPC	= 0;

				info.HangameuserCountBattle			= 0;
				info.HangameuserCountField			= 0;
				info.HangameuserCountBattleOfPC		= 0;
				info.HangameuserCountFieldOfPC		= 0;

				info.preHangameUserCountBattle		= 0;
				info.preHangameUserCountField		= 0;
				info.preHangameUserCountBattleOfPC	= 0;
				info.preHangameUserCountFieldOfPC	= 0;

				info.PManguserCountBattle			= 0;
				info.PManguserCountField			= 0;
				info.PManguserCountBattleOfPC		= 0;
				info.PManguserCountFieldOfPC		= 0;

				info.prePMangUserCountBattle		= 0;
				info.prePMangUserCountField			= 0;
				info.prePMangUserCountBattleOfPC	= 0;
				info.prePMangUserCountFieldOfPC		= 0;

				OutputDebug("[Database::CheckCurrentUserCount]w(%d)c(%d)st(%d)si(%d) 동접 초기화\n", info.ti_WorldID, info.ti_ChannelID, info.ti_ServerType, info.ti_ServerID);
			}
		}
		else
		{
			if(info.userParentHistoryIDX!=0)	// 장애에서 복구 상태가 된 경우이다.
			{
				dwWarning = MASTER_SERVER_SUCCESS;
				strncpy(szMsgFormat, " ", 1);

				dwParentHistoryIDX = info.userParentHistoryIDX;
				info.userParentHistoryIDX = 0;

			}
			info.bReceiveCurrentUser = false; // UpdateCurrentUserCount에서 동접 값을 받으면 이 변수를 true로 바꾸어 줄것이다.

		}			
	}

	if(dwWarning==0)	// 동접 값 체크를 한 경우에 아무런 이상이 없는 경우 폭증/폭감에 대한 확인을 한다.
	{
		/*
		2006.04.15
		modify by guiun
		*/		
		if(dwPrevCount>dwCurrentCount)			// 폭감 체크
		{
			if((dwPrevCount - dwCurrentCount ) > (warningDef->i_ConnectCountUnderGap))
			{
				dwWarning = USERCOUNT_GAPLOWER;
				sprintf(szMsgFormat, MSG_USERCOUNT_GAPLOWER, dwPrevCount, dwCurrentCount);
			}
		}
		else if(dwPrevCount<dwCurrentCount)	// 폭증 체크
		{
			if((dwCurrentCount - dwPrevCount) > (warningDef->i_ConnectCountOverGap))
			{
				dwWarning = USERCOUNT_GAPOVER;
				sprintf(szMsgFormat, MSG_USERCOUNT_GAPOVER, dwPrevCount, dwCurrentCount);
			}
		}
	}

	if(dwWarning!=0)	// 장애가 발생하였으므로 장애 로그를 남긴다.
	{
		eXtreme::RecordSet rec;
		rec.m_callbackError  = Database::CallbackErrorRecordSet;
		if(dwWarning==MASTER_SERVER_ERROR)	// return parameter bind MasterServer 장애 발생시에만 HistoryIDX를 받는다.
		{
			rec.BindParameter(&info.userParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
		}
		// input parameter bind
		rec.BindParameter(NULL, "@WorldID"          , adTinyInt, adParamInput, 1                   , _bstr_t(info.ti_WorldID   ));
		rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1                   , _bstr_t(info.ti_ChannelID ));
		rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1                   , _bstr_t(info.ti_ServerType));
		rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1                   , _bstr_t(info.ti_ServerID  ));
		rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4                   , _bstr_t(dwWarning         ));
		rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szMsgFormat) , _bstr_t(szMsgFormat       ));
		rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime)    , _bstr_t(dateTime          ));
		rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4                   , _bstr_t(dwParentHistoryIDX));
		// call storedProcedure
		m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForConnectCount");

		SendErrorNotifyNTF(	info.ti_WorldID,
							info.ti_ChannelID,
							info.ti_ServerType,
							info.ti_ServerID,
							dwWarning, 
							dateTime, 
							szMsgFormat);

	}
	else	// 장애 상태가 계속 되고 있으면 장애 코드만 리턴을 해준다.
	{
		if(bCheckAlive==true && info.userParentHistoryIDX>0)
		{
			dwWarning = MASTER_SERVER_ERROR;
		}
	}

	if(dwWarning!=MASTER_SERVER_SUCCESS)
	{
		*warning = dwWarning;
	}

}

void Database::CheckProcess( ServerProcess& info, int* warning, char* dateTime )
{
	char	filename[20] = {0, };
	char	path[100] = {0, };
	//DWORD	dwAlert = 0;
	*warning = 0;
	
	::WideCharToMultiByte( CP_ACP, 0, info.vc_AbsolutePath, -1, path, 100, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, info.vc_ProcessName, -1, filename, 100, 0, 0 );


	/*
		2006.04.15
		modify by guiun
		복구 기록시 parentHistoryIdx를 입력 하기 위하여 장애 기록시 historyIdx를 프로시져 결과로 받는다.
	*/
	*warning = 0;
	if(info.dw_alert==1){	// 장애 기록
		*warning = PROCESS_SHUTDOWN;
		info.dw_alert = 2;
	}
	else if(info.dw_alert==3){	// 복구 기록
		*warning = PROCESS_STARTUP;	
	}

	if(*warning!=0){
		char szBuffer[96] = {0, };

		sprintf(szBuffer, MSG_PROCESS_SHUTDOWN, filename);
		eXtreme::RecordSet rec;
		rec.m_callbackError = Database::CallbackErrorRecordSet;
		// return parameter bind
		//if(*warning==PROCESS_SHUTDOWN){	// 장애 보고 경우에만 return 값을 받는다.
		//}
		rec.BindParameter(&info.i_ParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
		// input parameter bind
		rec.BindParameter(NULL, "@WorldID"        ,   adTinyInt, adParamInput, 1               , _bstr_t(info.ti_WorldID   ));
		rec.BindParameter(NULL, "@ChannelID"        , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ChannelID ));
		rec.BindParameter(NULL, "@ServerType"       , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerType));
		rec.BindParameter(NULL, "@ServerID"         , adTinyInt, adParamInput, 1               , _bstr_t(info.ti_ServerID  ));
		rec.BindParameter(NULL, "@HitchDetailID"    , adInteger, adParamInput, 4               , _bstr_t(*warning          ));
		rec.BindParameter(NULL, "@nv_Message"       , adVarChar, adParamInput, (long)strlen(szBuffer), _bstr_t(szBuffer          ));
		rec.BindParameter(NULL, "@dt_RegisterDate"  , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime          ));
		rec.BindParameter(NULL, "@ProcessName"      , adVarChar, adParamInput, 100              , _bstr_t(filename          ));
		rec.BindParameter(NULL, "@AbsolutePath"     , adVarChar, adParamInput, 100             , _bstr_t(path              ));
		rec.BindParameter(NULL, "@ParentHistoryIDX" , adInteger, adParamInput, 4               , _bstr_t(info.i_ParentHistoryIDX));
		// call storedProcedure
		m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForResource");
		SendErrorNotifyNTF(	info.ti_WorldID, 
							info.ti_ChannelID,
							info.ti_ServerType,
							info.ti_ServerID,
							*warning, 
							dateTime, 
							MSG_PROCESS_SHUTDOWN, 
							filename );

		if(*warning==PROCESS_STARTUP){	// 장애 복구 보고인 경우에는 ParentHistory를 0 으로 설정한다.
			info.i_ParentHistoryIDX = 0;
			info.dw_alert = 0;
		}
		//  DB에 기록되었음을 체크한다.
		MonitorProcess::Instance()->SetProcessAlert( info.ti_WorldID, info.ti_ChannelID, info.ti_ServerType, info.ti_ServerID, info.vc_InnerIP,
					info.vc_ProcessName, info.vc_AbsolutePath, info.dw_alert, info.i_ParentHistoryIDX);

		//MonitorProcess::Instance()->SetProcessAlert( info.ti_WorldID, info.ti_ChannelID, info.ti_ServerType, info.ti_ServerID,,
		//			info.vc_ProcessName, info.vc_AbsolutePath, info.dw_alert, info.i_ParentHistoryIDX);
	}	
}

void Database::CheckSupplyMoney( SupplyMoney& info, int* warning, char* dateTime )
{
	char	queryStr[256] = {0, };
	char	message[256] = {0, };

	*warning = 0;

	//  장애 판단
	//  얻은 돈의 증감 % = a, 평균보유금액의 증감 % = b
	//  abs( a - b ) > m_stdMoneyGap 이면 장애처리.
	/*
	UINT64 percentageGetMoney  = (info.prevTotalGetMoney!=0) 
							? ( info.totalGetMoney - info.prevTotalGetMoney ) * 100 / info.prevTotalGetMoney
							: 0;
	*/
	/*
	int percentageUserMoney = (info.prevTotalUserMoney!=0) 
							? (int)((info.totalUserMoney - info.prevTotalUserMoney) * 100 / info.prevTotalUserMoney)
							: 0;
	int percentageGetMoney  = (info.prevTotalGetMoney!=0) 
							? (int)((__int64)(info.totalGetMoney - info.prevTotalGetMoney) * 100 / info.prevTotalGetMoney)
							: 0;
	int percentageLoseMoney = (info.prevTotalLoseMoney!=0) 
							? (int)((info.totalLoseMoney - info.prevTotalLoseMoney) * 100 / info.prevTotalLoseMoney)
							: 0;
	*/
	/*
	int percentageGetMoney  = 0;
	
	__int64 temp1 = (info.totalGetMoney - info.prevTotalGetMoney);
	__int64 tmep2 = temp1 * 100;
	__int64 temp3 = ( tmep2 / (__int64)info.prevTotalGetMoney);
	percentageGetMoney = (int)temp3;
	*/
	int percentageGetMoney  = (info.prevTotalGetMoney!=0) 
							? (int)((__int64)(info.totalGetMoney - info.prevTotalGetMoney) * 100 / (__int64)info.prevTotalGetMoney)
							: 0;

	// 얻은 통화 장애 발생 조건
	if(abs(percentageGetMoney) >= (int)m_stdMoneyGap){
		//  장애처리
		sprintf( message, MSG_SUPPLYMONEY_ERROR, info.prevTotalGetMoney, info.totalGetMoney, percentageGetMoney);

		*warning = SUPPLYMONEY_ERROR;
		sprintf( queryStr, "{call USP_AddWarningHistoryForMoney(%d, %d, %d, '%s', '%s')}",
					info.ti_worldID, info.ti_channelID, SUPPLYMONEY_ERROR, message, dateTime );
		m_dbcWops.Query( 0, queryStr );

		SendErrorNotifyNTF(	info.ti_worldID, 
							info.ti_channelID,
							0,
							0,
							*warning, 
							dateTime, 
							message  );
	}
	/*
	if(info.prevTotalGetMoney!=0){
		if(info.totalGetMoney > info.prevTotalGetMoney){
			percentageGetMoney = (info.totalGetMoney - info.prevTotalGetMoney) * 100 / info.prevTotalGetMoney;
		}
		else{
			percentageGetMoney = (info.prevTotalGetMoney - info.totalGetMoney) * 100 / info.prevTotalGetMoney;
		}
	}
	
	if(info.prevTotalUserMoney!=0){
		if(info.totalUserMoney > info.prevTotalUserMoney){
			percentageUserMoney = (info.totalUserMoney - info.prevTotalUserMoney) * 100 / info.prevTotalUserMoney;
		}
		else{
			percentageUserMoney = (info.prevTotalUserMoney - info.totalUserMoney) * 100 / info.prevTotalUserMoney;
		}
	}
	

	if( _abs64( ( percentageGetMoney - percentageUserMoney ) ) >= ( int )m_stdMoneyGap )
	{
		//  장애처리
		sprintf( message, MSG_SUPPLYMONEY_ERROR, percentageGetMoney, percentageUserMoney );

		*warning = SUPPLYMONEY_ERROR;
		sprintf( queryStr, "{call USP_AddWarningHistoryForMoney(%d, %d, %d, '%s', '%s')}",
					info.ti_worldID, info.ti_channelID, SUPPLYMONEY_ERROR, message, dateTime );
		m_dbcWops.Query( 0, queryStr );

		SendErrorNotifyNTF(	info.ti_worldID, 
							info.ti_channelID,
							0,
							0,
							*warning, 
							dateTime, 
							MSG_SUPPLYMONEY_ERROR, 
							percentageGetMoney, 
							percentageUserMoney );
	}
	*/
}

void Database::ClearResourceInfo()
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexSystemResource );
	ResourceInfo*					resourceInfo = NULL;


	for(ListSystemResource::iterator iter = m_listSystemResource.begin() ; iter != m_listSystemResource.end() ; iter++ )
	{
		resourceInfo	= *iter;

		resourceInfo->i_CpuMaxResource		= 0;
		resourceInfo->i_CpuUseResource		= 0;
		resourceInfo->i_MemoryMaxResource	= 0;
		resourceInfo->i_HDDUseResource		= 0;
		resourceInfo->i_HDDMaxResource		= 0;
		resourceInfo->i_HDDUseResource		= 0;
	}
}

void Database::SP_CURRENT_RESOURCE_INSERT( char* dateTime )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexSystemResource );
	ResourceInfo*					resourceInfo = NULL;
	int								warningCode = 0;
	char							temp[256] = {0, };
	std::string						queryStr;

	queryStr = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );
	queryStr += std::string( "Delete From TB_CurrentTimeResourceLog;" );

	for(ListSystemResource::iterator iter = m_listSystemResource.begin() ; iter != m_listSystemResource.end() ; iter++ )
	{
//		bool hasError = false;

		int hasError = 0;		// 0 : 에러없음, 1 : Request 에러, 2 : 프로세스 에러, 3 : 리소스 에러

		resourceInfo = *iter;

		// Request 에러가 있다면 우선순위상 가장 높으므로 이를 기록한다
		{
            eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexWebRequest );
			DWORD key = UID( resourceInfo->ti_WorldID, resourceInfo->ti_ChannelID, resourceInfo->ti_ServerType, resourceInfo->ti_ServerID ); // 서버 키를 구한다
			MapWebRequest::iterator iter = m_MapWebRequest.find(key);

			if( iter != m_MapWebRequest.end() )
			{
                hasError = 1;		// Request 에러                
				resourceInfo->i_WarningKey = iter->second;
			}
		}

		{
			//  Process 정보
			ServerProcess	processList[20] = {0, };
			int				size = 0;

			MonitorProcess::Instance()->GetList( resourceInfo->ti_WorldID, resourceInfo->ti_ChannelID,
				resourceInfo->ti_ServerType, resourceInfo->ti_ServerID, resourceInfo->szServerIP, processList, 20, &size );


			for(int i = 0 ; i < size ; i++ )
			{
				// 접속 되어 있는 RTTAgent에 대해서만 프로세스 감시를 한다.
				//if(ManagerClient::Instance()->FindClient( resourceInfo->ti_WorldID, 
				//	                                      resourceInfo->ti_ChannelID,
				//										  resourceInfo->ti_ServerType, 
				//										  resourceInfo->ti_ServerID ) != NULL)

				// 접속 되어 있는 RTTAgent에 대해서만 프로세스 감시를 한다.
				if(ManagerClient::Instance()->FindClient( resourceInfo->szServerIP ) != NULL)
				{
					//  이 함수에서는 프로세스의 상태를 계속 기록한다.
					CheckProcess( processList[i], &warningCode, dateTime );

					if( processList[i].dwStatus == 0 )
					{
						if( 0 == hasError )		// Request 에러가 없으면 프로세스 에러를 기록
						{
							resourceInfo->i_WarningKey = PROCESS_SHUTDOWN;
							hasError = 2;		// 프로세스 에러
						}

						//resourceInfo->i_WarningKey = PROCESS_SHUTDOWN;
						//	hasError = true;

						//  Process 에러는 1개만 보고되어야 하므로 1개 기록후 루프를 종료한다.
						break;
					}
					/*
					else if( processList[i].dwStatus == 1 )
					{
						//  프로세스가 살아있는 상태는 기록하지 않는다.
					}
					*/
				}
			}
		}

		// 리소스의 상태를 기록한다
		CheckSystemResource( *resourceInfo, &warningCode, dateTime );

		//  실시간 에러보고는 ServerKey당 1개만 남아야 하기때문에,
		//  Process에러가 보고되면 우선순위상 리소스 에러는 보고하지 않는다.

		if( 0 == hasError )
		{
			if(warningCode==CPU_UPPER_LIMIT || warningCode==MEMORY_UPPER_LIMIT || warningCode==HDD_LEAST_LIMIT)
			{
				resourceInfo->i_WarningKey = warningCode; // 장애인 경우
				hasError = 3;		// 리소스 에러
			}
		}

		if( 0 == hasError )
		{
			resourceInfo->i_WarningKey = 0;
		}

/*
		//if(hasError==false){
		//	if(warningCode==CPU_UPPER_LIMIT || warningCode==MEMORY_UPPER_LIMIT || warningCode==HDD_LEAST_LIMIT){
		//		resourceInfo->i_WarningKey = warningCode; // 장애인 경우
		//		hasError=true;
		//	}
		//}

		//if(hasError==false) resourceInfo->i_WarningKey = 0;
*/
		sprintf( temp, "EXEC USP_CURRENT_RESOURCE_INSERT '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d;", 
			    dateTime,
				resourceInfo->ti_WorldID, 
				resourceInfo->ti_ChannelID, 
				resourceInfo->ti_ServerType, 
				resourceInfo->ti_ServerID,
				resourceInfo->i_CpuUseResource, 
				resourceInfo->i_CpuMaxResource,
				resourceInfo->i_MemoryUseResource, 
				resourceInfo->i_MemoryMaxResource,
				resourceInfo->i_HDDUseResource, 
				resourceInfo->i_HDDMaxResource, 
				resourceInfo->i_WarningKey );
		queryStr += std::string( temp );
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );

	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_USER_INSERT( char* dateTime, bool bCheckAlive )
{
	ServerInfo*					serverInfo = NULL;
	char						temp[256] = {0, };
	std::string					queryStr;
	int							warningCode = 0;


	queryStr  = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );
	queryStr += std::string( "Delete From TB_CurrentTimeConnectLog;" );
	queryStr += std::string( "Declare @CurrentTime DateTime;" );
	queryStr += std::string( "Set @CurrentTime = getdate();" );

	DWORD dwConnectCountOfPersonal			= 0;	// 기존 썬 유저 개인 동접
	DWORD dwConnectCountOfPC				= 0;	// 기존 썬 유저 PC방 동접
	DWORD dwHangameConnectCountOfPersonal	= 0;	// 한게임계정 개인 동접
	DWORD dwHangameConnectCountOfPC			= 0;	// 한게임계정 PC방 동접
	DWORD dwPMangConnectCountOfPersonal		= 0;	// 피망계정 개인 동접
	DWORD dwPMangConnectCountOfPC			= 0;	// 피망계정 PC방 동접

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		serverInfo = *iter;
		
		// Field 및 Battle 서버인 경우만 동접 값을 처리한다.
		// ServerID가 1인 서버만 동접 값을 받고 있다.
		if(serverInfo->ti_ServerType==2 || (serverInfo->ti_ServerType==3 && serverInfo->ti_ServerID==1))
		{
			// 동접 값에 대한 확인을 한다.
			CheckCurrentUserCount(*serverInfo, &warningCode, dateTime, bCheckAlive);

			if(serverInfo->ti_ServerType==2)
			{
                // 필드 동접
				dwConnectCountOfPersonal		= serverInfo->userCountField;				// 기존 썬 유저 개인 동접
				dwConnectCountOfPC				= serverInfo->userCountFieldOfPC;			// 기존 썬 유저 PC방 동접 
				dwHangameConnectCountOfPersonal	= serverInfo->HangameuserCountField;		// 한게임계정 개인 동접
				dwHangameConnectCountOfPC		= serverInfo->HangameuserCountFieldOfPC;	// 한게임계정 PC방 동접
				dwPMangConnectCountOfPersonal	= serverInfo->PManguserCountField;			// 피망계정 개인 동접
				dwPMangConnectCountOfPC			= serverInfo->PManguserCountFieldOfPC;		// 피망계정 PC방 동접
			}
			else
			{
				// 배틀존 동접
				dwConnectCountOfPersonal		= serverInfo->userCountBattle;				// 기존 썬 유저 개인 동접
				dwConnectCountOfPC				= serverInfo->userCountBattleOfPC;			// 기존 썬 유저 PC방 동접 
				dwHangameConnectCountOfPersonal	= serverInfo->HangameuserCountBattle;		// 한게임계정 개인 동접
				dwHangameConnectCountOfPC		= serverInfo->HangameuserCountBattleOfPC;	// 한게임계정 PC방 동접
				dwPMangConnectCountOfPersonal	= serverInfo->PManguserCountBattle;			// 피망계정 개인 동접
				dwPMangConnectCountOfPC			= serverInfo->PManguserCountBattleOfPC;		// 피망계정 PC방 동접
			}
			memset(temp, 0x00, 256);

			// 한게임, 피망 계정으로 접속한 유저의 동접정보 추가 Insert
#ifdef	_090901_CHANNELLING_CONNECT_USER_
			sprintf( temp, "EXEC USP_CURRENT_USER_INSERT '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d;", 
				            dateTime,						
							serverInfo->ti_WorldID, 
							serverInfo->ti_ChannelID, 
							serverInfo->ti_ServerType, 
							serverInfo->ti_ServerID,
							dwConnectCountOfPersonal,			// 기존 썬 유저 개인 동접
							dwConnectCountOfPC,					// 기존 썬 유저 PC방 동접
							dwHangameConnectCountOfPersonal,	// 한게임계정 개인 동접
							dwHangameConnectCountOfPC,			// 한게임계정 PC방 동접
							dwPMangConnectCountOfPersonal,		// 피망계정 개인 동접
                            dwPMangConnectCountOfPC,			// 피망계정 PC방 동접
							warningCode );

#else		// 기존 유저의 동접정보 Insert
            sprintf( temp, "EXEC USP_CURRENT_USER_INSERT '%s', %d, %d, %d, %d, %d, %d, %d;", 
				            dateTime,						
							serverInfo->ti_WorldID, 
							serverInfo->ti_ChannelID, 
							serverInfo->ti_ServerType, 
							serverInfo->ti_ServerID,
							dwConnectCountOfPersonal,			// 기존 썬 유저 개인 동접
							dwConnectCountOfPC,					// 기존 썬 유저 PC방 동접
							warningCode );
#endif

			queryStr += std::string( temp );

			serverInfo->preUserCountField		= serverInfo->userCountField;
			serverInfo->preUserCountFieldOfPC	= serverInfo->userCountFieldOfPC;

			serverInfo->preUserCountBattle		= serverInfo->userCountBattle;
			serverInfo->preUserCountBattleOfPC	= serverInfo->userCountBattleOfPC;

			serverInfo->preHangameUserCountField		= serverInfo->HangameuserCountField;
			serverInfo->preHangameUserCountFieldOfPC	= serverInfo->HangameuserCountFieldOfPC;

			serverInfo->preHangameUserCountBattle		= serverInfo->HangameuserCountBattle;
			serverInfo->preHangameUserCountBattleOfPC	= serverInfo->HangameuserCountBattleOfPC;

			serverInfo->prePMangUserCountField		= serverInfo->PManguserCountField;
			serverInfo->prePMangUserCountFieldOfPC	= serverInfo->PManguserCountFieldOfPC;

			serverInfo->prePMangUserCountBattle		= serverInfo->PManguserCountBattle;
			serverInfo->prePMangUserCountBattleOfPC	= serverInfo->PManguserCountBattleOfPC;

			/*
			OutputDebug("[Database::SP_CURRENT_USER_INSERT]w(%d)c(%d)st(%d)si(%d) (%d/%d, %d/%d, %d/%d) field info(%d/%d, %d/%d, %d/%d), battleinfo(%d/%d, %d/%d, %d/%d)\n"
						, serverInfo->ti_WorldID
						, serverInfo->ti_ChannelID
						, serverInfo->ti_ServerType
						, serverInfo->ti_ServerID
						, dwConnectCountOfPersonal
						, dwConnectCountOfPC
						, dwHangameConnectCountOfPersonal
						, dwHangameConnectCountOfPC
						, dwPMangConnectCountOfPersonal
						, dwPMangConnectCountOfPC
						, serverInfo->userCountField
						, serverInfo->userCountFieldOfPC
						, serverInfo->HangameuserCountField
						, serverInfo->HangameuserCountFieldOfPC
						, serverInfo->PManguserCountField
						, serverInfo->PManguserCountFieldOfPC
						, serverInfo->userCountBattle
						, serverInfo->userCountBattleOfPC
						, serverInfo->HangameuserCountBattle
                        , serverInfo->HangameuserCountBattleOfPC
						, serverInfo->PManguserCountBattle
                        , serverInfo->PManguserCountBattleOfPC
						);
			//*/
		}
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );

	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_BattleZone_INSERT( char* dateTime )
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexBattleZone );
	BattleZoneInfo*						info = NULL;
	char								temp[256] = {0, };
	std::string							queryStr;


	queryStr = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );
	queryStr += std::string( "Delete From TB_CurrentTimeBattleZoneLog;" );

	for(ListBattleZoneInfo::iterator iter = m_listBattleZoneInfo.begin() ; iter != m_listBattleZoneInfo.end() ; iter++ )
	{
		info = *iter;

		sprintf( temp, "EXEC USP_CURRENT_BattleZone_INSERT '%s', %d, %d, %d, %d;",
			dateTime, info->ti_worldID, info->ti_channelID, info->ti_zoneType, info->i_roomCount );

		queryStr += std::string( temp );
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );

	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_Money_INSERT( char* dateTime )
{
	SupplyMoney*				info = NULL;
	int							warningCode = 0;
	char						temp[256] = {0, };
	char						jackpotQuery[256] = {0, };
	std::string					queryStr;

	if( m_listSupplyMoney.size() == 0 )
		return;

	queryStr = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );
	//queryStr += std::string( "Delete From TB_CurrentTimeMoneyLog;" );

	for(ListSupplyMoney::iterator iter = m_listSupplyMoney.begin() ; iter != m_listSupplyMoney.end() ; iter++ )
	{
		info = *iter;


		CheckSupplyMoney( *info, &warningCode, dateTime );

		sprintf( temp, "EXEC USP_CURRENT_Money_INSERT '%s', %d, %d, %I64d, %I64d, %I64d, %I64d, %I64d, %I64d, %d, %I64d;",
			dateTime, 
			info->ti_worldID, 
			info->ti_channelID,
			info->totalGetMoney, 
			info->totalLoseMoney, 
			info->totalUserMoney, 
			info->wareHouseTotalPlusMoney,
			info->wareHouseTotalMinusMoney,
			info->wareHouseAveMoney,
			warningCode,
			info->circulateMoney);
		queryStr += std::string( temp );
		// 상거래 금액 초기화
		info->circulateMoney = 0;
		
		/// jackpot 관련 처리 ///////////////////////
		//USHORT usJackpotCnt		= info->jackpotCnt;
		//UINT64 ui64JackpotMoney	= info->jackpotMoney;

		if(info->jackpotCnt>0){
			memset(jackpotQuery, 0x00, 256);
			sprintf(jackpotQuery, "EXEC USP_AddJackPotLog '%s', %d, %d, %I64d, %d;",
				dateTime,
				info->ti_worldID,
				info->ti_channelID,
				info->jackpotMoney,
				info->jackpotCnt);

			info->preJackpotCnt		= info->jackpotCnt;
			info->preJackpotMoney	= info->jackpotMoney;

			info->jackpotCnt	= 0;
			info->jackpotMoney	= 0;

			queryStr += std::string( jackpotQuery );
		}
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );

	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_GameStatistics_INSERT( char* dateTime)
{
	GameStatistics*				info = NULL;
	char						temp[256] = {0, };
	std::string					queryStr;

	if( m_listGameStatistics.empty() == true ) return;

	queryStr = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );

	for(ListGameStatistics::iterator iter = m_listGameStatistics.begin() ; iter != m_listGameStatistics.end() ; iter++ )
	{
		info = *iter;
		
		for(int i=0;i<eSTATISTIC_MAX + 1;i++){
			if(m_StatisticEnumCode.uiEnumStatistic[i]!=0 && info->ui64Statistics[i]>0){
				memset(temp, 0x00, 256);
				sprintf( temp, "EXEC USP_Day_Statistics_ItemAction_INSERT '%s', %d, %d, %d, %I64d;\r\n",
				dateTime, 
				m_StatisticEnumCode.uiEnumStatistic[i],
				info->ti_worldID, 
				info->ti_channelID,
				info->ui64Statistics[i]);
				
				queryStr += std::string( temp );
				// 초기화
				info->ui64Statistics[i] = 0;
			}
		}		
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );
	
	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_RTTAgent_INSERT( char* dateTime )
{
	ServerInfo*					serverInfo = NULL;
	char						temp[256] = {0, };
	std::string					queryStr;

	//m_dbcWops.Query( 0, "SET XACT_ABORT On;Begin Tran;EXEC USP_CURRENT_USER_INSERT '2006-07-14 17:07:35', 1, 1, 2, 1, 100, 110, 0;IF @@Error <> 0 RollBack Tran Else Commit Tran;SET XACT_ABORT Off;" );

	queryStr  = std::string( "SET XACT_ABORT On;" );
	queryStr += std::string( "Begin Tran;" );
	queryStr += std::string( "Delete From TB_CurrentTimeRTTAgentHitchLog;" );

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ ){
		serverInfo = *iter;
		//char szFormat[12];memset(szFormat, 0x00, 12);
		//  장애 검사
		bool bChecker = false;
		DWORD dwParentHistoryIDX = 0;
		DWORD dwWarning = 0;
		if(serverInfo->certifiedRTTAgent==1){	// 장애가 발생한 경우
			bChecker = true;
			dwWarning = RTTA_NOT_RESPONSE;
			//strcpy(szFormat, MSG_RTTA_NOT_RESPONSE);			
			//  DB에 기록되었음을 표시한다.
			serverInfo->certifiedRTTAgent = 2;
		}
		else if(serverInfo->certifiedRTTAgent==0 && serverInfo->agentParentHistoryIDX!=0){	// 장애가 복구된경우
			bChecker = true;
			dwWarning = RTTA_RESPONSE;
			//strcpy(szFormat, MSG_RTTA_RESPONSE);
			dwParentHistoryIDX = serverInfo->agentParentHistoryIDX;
			serverInfo->agentParentHistoryIDX = 0;
		}

		if(bChecker==true){	// 장애 및 복구 로그를 남긴다.
			eXtreme::RecordSet rec;

			// return parameter bind
			if(dwWarning==RTTA_NOT_RESPONSE){
				rec.BindParameter(&serverInfo->agentParentHistoryIDX, "", adInteger, adParamReturnValue, sizeof(int), "");
			}
			// input parameter bind
			rec.BindParameter(NULL, "@WorldID"         , adTinyInt, adParamInput, 1               , _bstr_t(serverInfo->ti_WorldID   ));
			rec.BindParameter(NULL, "@ChannelID"       , adTinyInt, adParamInput, 1               , _bstr_t(serverInfo->ti_ChannelID ));
			rec.BindParameter(NULL, "@ServerType"      , adTinyInt, adParamInput, 1               , _bstr_t(serverInfo->ti_ServerType));
			rec.BindParameter(NULL, "@ServerID"        , adTinyInt, adParamInput, 1               , _bstr_t(serverInfo->ti_ServerID  ));
			rec.BindParameter(NULL, "@HitchDetailID"   , adInteger, adParamInput, 4               , _bstr_t(dwWarning                ));
			rec.BindParameter(NULL, "@Message"         , adVarChar, adParamInput, 1               , _bstr_t(" "                      ));
			rec.BindParameter(NULL, "@RegisterDate"    , adVarChar, adParamInput, (long)strlen(dateTime), _bstr_t(dateTime                 ));
			rec.BindParameter(NULL, "@ParentHistoryIDX", adInteger, adParamInput, 4               , _bstr_t(dwParentHistoryIDX       ));
			// call storedProcedure
			m_dbcWops.CallStoredProcedure(&rec, "USP_AddWarningHistoryForWopsRelation");
			SendErrorNotifyNTF(	serverInfo->ti_WorldID, 
								serverInfo->ti_ChannelID, 
								serverInfo->ti_ServerType, 
								serverInfo->ti_ServerID, 
								dwWarning, 
								dateTime, 
								MSG_RTTA_NOT_RESPONSE );
		}		
		
		{//  현재 상태 기록
			sprintf( temp, "EXEC USP_CURRENT_RTTAgent_INSERT '%s', %d, %d, %d, %d, %d;", 
				dateTime,
				serverInfo->ti_WorldID, 
				serverInfo->ti_ChannelID, 
				serverInfo->ti_ServerType, 
				serverInfo->ti_ServerID,
				( serverInfo->certifiedRTTAgent == 0 ? 0 : RTTA_NOT_RESPONSE ) );
			queryStr += std::string( temp );
		}
	}

	queryStr += std::string( "IF @@Error <> 0 RollBack Tran Else Commit Tran;" );
	queryStr += std::string( "SET XACT_ABORT Off;" );

	m_dbcWops.Query( 0, queryStr.data() );
}

void Database::SP_CURRENT_RTTS_UPDATE( char* dateTime )
{
	char	mbcs[256] = {0, };

	if( m_svrInfoRTTS == 0 ) return;

	sprintf( mbcs, "{call USP_CURRENT_RTTS_UPDATE('%s', %d, %d, %d, %d)}", 
		            dateTime, 
					m_svrInfoRTTS->ti_WorldID, 
					m_svrInfoRTTS->ti_ChannelID, 
					m_svrInfoRTTS->ti_ServerType, 
					m_svrInfoRTTS->ti_ServerID );

	m_dbcWops.Query( 0, mbcs );
}

void Database::GetUpdateTime()
{
	TB_UpdateTime	record;

	m_dbcWops.Query( &record, "SELECT ti_ConnectCountUpdateTime, ti_ResourceUpdateTime, ti_WopsRelationUpdateTime, "
								"ti_MoneyUpdateTime, ti_MoneyGap FROM TB_UpdateTime" );
	if( record.IsEndRecord() == false )
	{
		m_gapConnectCount	= record.record.ti_ConnectCountUpdateTime * 1000;
		m_gapResource		= record.record.ti_ResourceUpdateTime * 1000;
		m_gapWopsRelation	= record.record.ti_WopsRelationUpdateTime * 1000;
		m_stdMoneyGap		= record.record.ti_MoneyGap;		//  이건 % 기준이다.

		RTTAgent::m_updateUserMoneyTick			= record.record.ti_MoneyUpdateTIme * 1000;
		RTTAgent::m_updateBattleZoneInfoTick	= record.record.ti_MoneyUpdateTIme * 1000;
	}
	else
	{
		m_gapConnectCount	= 0;
		m_gapResource		= 0;
		m_gapWopsRelation	= 0;
	}
}

/*
	[2006.04.20] modify by guiun
	서버 탐색 방법 변경
*/
void Database::ClearLaunchProcessStatus( short worldID, short channelID, short serverType, short serverID )
{
	ServerInfo*					info = NULL;

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;
		if(worldID==0 || worldID==info->ti_WorldID){						// 전체 world 또는 특정 world
			if(channelID==CHANNEL_ALL || channelID==info->ti_ChannelID){	// 전체 channel 또는 특정 channel
				if(serverType==0 || serverType==info->ti_ServerType){		// 전체 servertype 또는 특정 servertype
					if(serverID==0 || serverID==info->ti_ServerID){			// 전체 serverid 또는 특정 serverid
						info->launchProcessStatus = 0;
					}
				}
			}
		}	
		/*
		//  해당 서버만 검색한다.
		if( worldID == 0 ||
			( worldID != 0 && channelID == 0 && info->ti_WorldID == worldID ) ||
			( worldID != 0 && channelID != 0 && info->ti_WorldID == worldID && info->ti_ChannelID == channelID ) )
		{
			info->launchProcessStatus = 0;
		}
		*/
	}
}

/*
	[2006.04.20] modify by guiun
	서버 탐색 방법 변경
*/
bool Database::CheckLaunchProcessStatus( short worldID, 
										 short channelID,
										 short serverType, 
										 short serverID, 
										 DWORD result, DWORD* error )
{
	ServerInfo*					info = NULL;
	bool						ret = true;

	*error = 0;

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;

		if(worldID==0 || worldID==info->ti_WorldID){						// 전체 world 또는 특정 world
			if(channelID==CHANNEL_ALL || channelID==info->ti_ChannelID){	// 전체 channel 또는 특정 channel
				if(serverType==0 || serverType==info->ti_ServerType){		// 전체 servertype 또는 특정 servertype
					if(serverID==0 || serverID==info->ti_ServerID){			// 전체 serverid 또는 특정 serverid
						
						char ipAddress[IP_MAX_LEN] = {0, };
						::WideCharToMultiByte( CP_ACP, 0, info->vc_InnerIP, -1, ipAddress, IP_MAX_LEN, 0, 0 );
						if(ManagerClient::Instance()->FindClient(ipAddress)==NULL){	// 접속된 Agent가 아니다.
							continue;
						}

						if( result == 0 ){
							info->launchProcessStatus = 2;		//  에러 발생
						}
						else if( result == 1 ){
							info->launchProcessStatus = 1;		//  작업 완료
						}

						//  에러가 보고된 기록을 찾는다.
						if( info->launchProcessStatus == 2 ){
							*error = 1;
						}

						//  아직 결과가 보고되지 않았다면 false를 반환한다.
						if( info->launchProcessStatus == 0 ){
							ret = false;
						}
					}
				}
			}
		}	

		/*
		//  해당 서버만 검색한다.
		if( checkWorldID == 0 ||
			( checkWorldID != 0 && checkChannelID == 0 && info->ti_WorldID == checkWorldID ) ||
			( checkWorldID != 0 && checkChannelID != 0 && info->ti_WorldID == checkWorldID && info->ti_ChannelID == checkChannelID ) )
		{
			if( info->ti_WorldID == worldID && info->ti_ChannelID == channelID &&
				info->ti_ServerType == serverType && info->ti_ServerID == serverID )
			{
				if( result == 0 )
					info->launchProcessStatus = 2;		//  에러 발생
				else if( result == 1 )
					info->launchProcessStatus = 1;		//  작업 완료
			}

			//  에러가 보고된 기록을 찾는다.
			if( info->launchProcessStatus == 2 )
				*error = 1;

			//  아직 결과가 보고되지 않았다면 false를 반환한다.
			if( info->launchProcessStatus == 0 )
				ret = false;
		}
		*/
	}

	return ret;
}

/*
	[2006.04.20] modify by guiun
	서버 패치 정보에 ServerType, ServerID 추가
*/
DWORD Database::MakePatchKey( __int64 wopsKey, DWORD uid, short worldID, short channelID , short serverType, short serverID, short serverTypeForProcess )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );
	PatchKey*	key = new PatchKey;

	key->bi_wopsKey					= wopsKey;
	key->i_uid						= uid;
	key->ti_WorldID					= worldID;
	key->ti_ChannelID				= channelID;
	key->ti_ServerType				= serverType;
	key->ti_ServerID				= serverID;
	key->ti_ServerTypeForProcess	= serverTypeForProcess;

	m_listPatchKey.push_back( key );

	//  해당 서버정보에서 패치 기록을 초기화한다.
	
	{
		ServerInfo*					info = NULL;
		
		for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
		{
			bool bCmp = false;
			info = *iter;
			if(worldID==0 || worldID==info->ti_WorldID)						// 전체 world 또는 특정 world
			{
				if(channelID==CHANNEL_ALL || channelID==info->ti_ChannelID)	// 전체 channel 또는 특정 channel
				{
					if(serverType==0 || serverType==info->ti_ServerType)	// 전체 servertype 또는 특정 servertype
					{
						if(serverID==0 || serverID==info->ti_ServerID)		// 전체 serverid 또는 특정 serverid
						{
							bCmp = true;							
						}
					}
				}
			}

			if( bCmp == true )
			{
				/*
					2006.09.19	modify by guiun
					패치시 해당 서버의 패치 대상 서버가 존재 하는지 확인을 한다.

					1. 해당 서버에 RTTAgent가 실행중인지 확인을 한다.
					2. 해당 서버의 패치 대상 서버가 존재 하는지 확인을 한다.
				*/

				// 해당 서버에 RTTAgent가 실행중인지 확인을 한다.
				RTTAgent* pAgent = ManagerClient::Instance()->FindClient(info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID);
				if( NULL != pAgent )
				{
					
					bool bPatch = false;
					if ( serverTypeForProcess != 0 )	// 해당 서버의 패치 대상 서버가 존재 하는지 확인을 한다.
					{
						//  Process 정보
						ServerProcess	processList[20];
						int				size = 0;
						
						char ip[16] = {0, };
						::WideCharToMultiByte( CP_ACP, 0, info->vc_InnerIP, -1, ip, 16, 0, 0 );
						ip[15] = '\0';

						MonitorProcess::Instance()->GetList( info->ti_WorldID, 
															 info->ti_ChannelID,
															 info->ti_ServerType, 
															 info->ti_ServerID, 
															 ip,
															 processList, 
															  20, 
															 &size );

						for ( int i  = 0 ; i < size ; i++ )
						{
							if( processList[i].ti_ServerTypeForPatch == serverTypeForProcess )
							{
								bPatch = true;
								break;
							}
						}
					}
					else	// 패치 대상 서버가 전체이므로 패치 대상 서버 등록 여부를 확인 하지 않고 패치를 진행한다.
					{
						bPatch = true;
					}
					
					if( bPatch )
					{
						info->patchStatus = MSG_RTTG_PATCHRESULT_ANS::ERROR_NONE;
						info->bPatchCompleted = false;
					}
					else	// 패치 대상 검색 실패
					{
						OutputDebug( "[Database::MakePatchKey] Not Found from MonitorProcess %d, %d, %d, %d, %S\n",
								info->ti_WorldID,
								info->ti_ChannelID,
								info->ti_ServerType,
								info->ti_ServerID,
								info->i_InnerPort );
					}
				}
				else	// RTTAgent가 실행중이 아니므로 실패
				{
					OutputDebug( "[Database::MakePatchKey] Not Found from ManagerClient %d, %d, %d, %d, %S\n",
								info->ti_WorldID,
								info->ti_ChannelID,
								info->ti_ServerType,
								info->ti_ServerID,
								info->i_InnerPort );
				}
			}
		}
	}
	return ( DWORD )( u_int64 )key;
}

/*
	[2006.04.25] modify by guiun
	list에서 patchkey를 꺼내고 list에서 해당 patchkey를 삭제 
*/
PatchKey* Database::GetPatchKey( DWORD patchKey )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );
	PatchKey*					key = NULL;

	for(ListPatchKey::iterator iter = m_listPatchKey.begin() ; iter != m_listPatchKey.end() ; iter++ )
	{
		key = *iter;
		if( patchKey == (DWORD)( u_int64 )key ){
			return key;
		}
	}

	return 0;
}

DWORD Database::SetPatchResult( wchar_t* ip, __int64 wopsKey, DWORD dwServerTypeForProcess, DWORD result, BYTE byIndex , const char* lpVersion)
{
	ServerInfo*					info = NULL;

	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;

		if( wcscmp( info->vc_InnerIP, ip ) == 0)
		{
			if(info->patchStatus==MSG_RTTG_PATCHRESULT_ANS::ERROR_NONE){	// 처음 error code를 받은 경우
				info->patchStatus = result;
			}
			else{
				// 기존 에러코드가 존재 하고 결과로 받은 에러코드가 성공이 아닌경우
				// 최종 에러코드가 기록이 된다.
				if(result!=MSG_RTTG_PATCHRESULT_ANS::ERROR_PATCHSUCCESS){	
					info->patchStatus = result;
				}
			}

			if(byIndex==0) info->bPatchCompleted = true;
            
			break;
		}
	}

	//  패치 결과를 DB에 Update
	char keyStr[64] = {0, };
	sprintf( keyStr, "%I64d", wopsKey );

//	try
//	{

	// 2006. 7. 13 김종호
	// QueryExternal()함수의 리턴값을 저장(예외 발생시 false)
	bool bException = false;

		switch( result )
		{
		case  MSG_RTTG_PATCHRESULT_ANS::ERROR_PROCESSRUN:
			bException = QueryExternal( "{call USP_AddPatchLog('%s', %d, %d, %d, %d, %d, %d, '%s')}", keyStr,
				info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, dwServerTypeForProcess,PATCH_PROCESSRUNNING, lpVersion );
			break;

		case  MSG_RTTG_PATCHRESULT_ANS::ERROR_FTPCONNECT_FAIL:
			bException = QueryExternal( "{call USP_AddPatchLog('%s', %d, %d, %d, %d, %d, %d, '%s')}", keyStr,
				info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, dwServerTypeForProcess, PATCH_FTPCONNECT_FAIL, lpVersion );
			break;

		case  MSG_RTTG_PATCHRESULT_ANS::ERROR_DOWNLOAD_FAIL:
			bException = QueryExternal( "{call USP_AddPatchLog('%s', %d, %d, %d, %d, %d, %d, '%s')}", keyStr,
				info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, dwServerTypeForProcess, PATCH_DOWNLOAD_FAIL, lpVersion );
			break;

		case  MSG_RTTG_PATCHRESULT_ANS::ERROR_FILENOTFOUND:
			bException = QueryExternal( "{call USP_AddPatchLog('%s', %d, %d, %d, %d, %d, %d, '%s')}", keyStr,
				info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, dwServerTypeForProcess, PATCH_FILESEARCH_FAIL, lpVersion );
			break;

		case  MSG_RTTG_PATCHRESULT_ANS::ERROR_PATCHSUCCESS:
			bException = QueryExternal( "{call USP_AddPatchLog('%s', %d, %d, %d, %d, %d, %d, '%s')}", keyStr,
				info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, dwServerTypeForProcess, PATCH_SUCCESS, lpVersion );
			break;
		
		case MSG_RTTG_PATCHRESULT_ANS::ERROR_INVALID_SERVERTYPE:
			/*
				특정 타입에 대하여 전체 서버에게 보내는 경우 RTTS가 각 서버가 가지고 있는 타입을 알수가 없다.
				따라서 이 에러코드는 패치 클라이언트가 특정 타입의 서버를 가지고 있지 않는 경우가 생기므로
				패치 클라이언트에서 에러로 전송되지만 
				아무런 처리를 하지 않는다.
            */ 
			break;
		default:
			OutputDebug( "Not defined result code(=%d).\n", result );
			break;
		}
//	}
//	catch( eXtreme::ExceptionDBC& )
//	{
//		return 0;
//	}

	/*
	if( !bException )
	{
		return 0;	// 예외발생시...
	}
	*/
	return info->patchStatus;
}

bool Database::IsPatchCompleted(short /*worldID*/, short /*channelID*/, short /*serverType*/, short /*serverID*/ )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );

	ServerInfo*					info = NULL;
	
	bool bRetValue = true;
	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;
		if( info->bPatchCompleted == false )
		{
			bRetValue =  false;
		}		
	}

	return bRetValue;

	//ListServerInfo::iterator	iter;
	//ServerInfo*					info;
	//
	//char szAddress[IP_MAX_LEN];
	//for( iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	//{
	//	info = *iter;	
	//	memset(szAddress, 0x00, IP_MAX_LEN);
	//	::WideCharToMultiByte( CP_ACP, 0, info->vc_InnerIP, -1, szAddress, IP_MAX_LEN, 0, 0 );
	//	
	//	if(ManagerClient::Instance()->FindClient(szAddress)!=NULL){
	//		if(worldID==0 || worldID==info->ti_WorldID){					// 전체 world 또는 특정 world
	//			if(channelID==CHANNEL_ALL || channelID==info->ti_ChannelID){			// 전체 channel 또는 특정 channel
	//				if(serverType==0 || serverType==info->ti_ServerType){	// 전체 servertype 또는 특정 servertype
	//					if(serverID==0 || serverID==info->ti_ServerID){		// 전체 serverid 또는 특정 serverid
	//						if(info->bPatchCompleted==false){
	//							return false;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	/*
	//	//  해당 서버만 검색한다.
	//	if( worldID == 0 ||
	//		( worldID != 0 && channelID == 0 && info->ti_WorldID == worldID ) ||
	//		( worldID != 0 && channelID != 0 && info->ti_WorldID == worldID && info->ti_ChannelID == channelID ) )
	//	{
	//		//  패치가 시작되지 않았거나 진행중인 경우
	//		if( info->patchStatus == MSG_RTTG_PATCHRESULT_ANS::ERROR_NONE )
	//			return false;
	//	}
	//	*/
	//}
	//return true;
}

void Database::BroadcastForPatch( char* buffer, int size )
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );

	ServerInfo*					info = NULL;
	
	for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
	{
		info = *iter;
		if( info->bPatchCompleted == false && info->patchStatus == MSG_RTTG_PATCHRESULT_ANS::ERROR_NONE)
		{
			RTTAgent* pAgent = ManagerClient::Instance()->FindClient(info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID);
			if( pAgent != NULL )
			{
				pAgent->SendPacket( buffer, size);
				OutputDebug( "[Database::BroadcastForPatch] %d, %d, %d, %d, %S\n",
								info->ti_WorldID,
								info->ti_ChannelID,
								info->ti_ServerType,
								info->ti_ServerID,
								info->i_InnerPort );
			}
			else
			{
				OutputDebug( "[Database::BroadcastForPatch] Not Found from ManagerClient %d, %d, %d, %d, %S\n",
								info->ti_WorldID,
								info->ti_ChannelID,
								info->ti_ServerType,
								info->ti_ServerID,
								info->i_InnerPort );
			}
		}		
	}
}

void Database::ClearPatchKeyAll()
{
	eXtreme::Guard	< eXtreme::Mutex >		lock( m_mutexPatchKey );
	
	{	// 서버 정보에 있는 패치 정보를 초기화 한다.
		ServerInfo*					info = NULL;
		
		for(ListServerInfo::iterator iter = m_listServerInfo.begin() ; iter != m_listServerInfo.end() ; iter++ )
		{
			info = *iter;
			info->bPatchCompleted = true;
		}
	}

	{	// 기존에 존재하는 패치 키를 삭제 한다.
		PatchKey*					info = NULL;

		for(ListPatchKey::iterator iter = m_listPatchKey.begin() ; iter != m_listPatchKey.end() ; iter++ )
		{
			info = *iter;
			delete info;
		}

		m_listPatchKey.clear();
	}
}

void Database::Run()
{
	char		dateTime[127] = {0, };
	char		tmp[8] = {0, };

	//  현재 Tick으로 초기화
	DWORD preTick_Resource	 = ::GetTickCount();
	DWORD preTick_UserCount	 = ::GetTickCount();
	DWORD preTick_UpdateTime	 = ::GetTickCount();
	DWORD preTick_RTTAgent 	 = ::GetTickCount();
	DWORD preTick_RTTServer    = ::GetTickCount();
	DWORD preTick_MasterServer = ::GetTickCount();

	DWORD dwTickGameStatistics = ::GetTickCount();


	//  DB 사용 유무를 설정
	::GetPrivateProfileString( "DBSetting", "Enable_UserCount", "err", tmp, 8, SERVERINFO_FILENAME );
	bool enableUserCount =  ( ::strncmp( tmp, "true", 8 ) == 0 );

	::GetPrivateProfileString( "DBSetting", "Enable_SystemResource", "err", tmp, 8, SERVERINFO_FILENAME );
	bool enableSystemResource =  ( ::strncmp( tmp, "true", 8 ) == 0 );

	::GetPrivateProfileString( "DBSetting", "Enable_UpateTime", "err", tmp, 8, SERVERINFO_FILENAME );
	bool enableUpdateTime =  ( ::strncmp( tmp, "true", 8 ) == 0 );

	::GetPrivateProfileString( "DBSetting", "Enable_RTTAgent", "err", tmp, 8, SERVERINFO_FILENAME );
	bool enableRTTAgent =  ( ::strncmp( tmp, "true", 8 ) == 0 );
	
	//DWORD dwCurrentTick = 0;
	while( 1 ){
		if( ::WaitForSingleObject( m_eventKill, 1000 ) == WAIT_OBJECT_0 ) break;
		
		//dwCurrentTick = ::GetTickCount();

//		if( m_flagReconnectDB == true )
//		{
//			m_flagReconnectDB = ( Initialize() == true ? false : true );
//			continue;
//		}

		// 2006. 7. 14 김종호
		// 위 소스를 아래와 같이 수정
		if( m_dbcWops.GetState() == adStateClosed )
		{
			Instance()->Finalize();
			if(false==Initialize())
			{
				OutputDebug( "[Database::Run] DB 연결 실패.\n" );
				Sleep(5000);
				continue;
			}
		}

		{// 현재 날짜
			SYSTEMTIME	sysTime;
			::GetLocalTime( &sysTime );
			sprintf( dateTime, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
		}

		//  동시접속자 수 갱신
		if( enableUserCount == true && ::GetTickCount() - preTick_UserCount > m_gapConnectCount )
		{
			/*
				[2006.04.17] Addy by guiun
			*/
			bool bCheckAlive = false;	// 동접 값 입력시 이 값으 true이면 주기적으로 동접값을 받고 있는 확인한다.
			// Master 서버 동접 값이 주기적으로 전송 되고 있는지 확인 한다.
			if(::GetTickCount() - preTick_MasterServer > MASTER_CHECK_TIME)
			{
				bCheckAlive = true;
			}// End Master Server 동접 값 확인

//*	1. 개발쪽에 패치때는 DB업데이트를 하지 않으므로 1~5까지를 주석처리 후 빌드한다
			SP_CURRENT_USER_INSERT( dateTime, bCheckAlive );
			SP_CURRENT_BattleZone_INSERT( dateTime );
			SP_CURRENT_Money_INSERT( dateTime );
//*/
			if(bCheckAlive)
			{
				preTick_MasterServer = ::GetTickCount();
			}
			preTick_UserCount = ::GetTickCount();
		}

		//  System Resource 갱신
		if( enableSystemResource == true && ::GetTickCount() - preTick_Resource > m_gapResource )
		{
//* 2.
			SP_CURRENT_RESOURCE_INSERT( dateTime );
//*/
			//ClearResourceInfo();
			preTick_Resource = ::GetTickCount();
		}

		//  RTTAgent 갱신
		if( enableRTTAgent == true && ::GetTickCount() - preTick_RTTAgent > m_gapWopsRelation )
		{
			//SP_CURRENT_RTTS_UPDATE( dateTime );
//* 3.
			SP_CURRENT_RTTAgent_INSERT( dateTime );
//*/
			preTick_RTTAgent = ::GetTickCount();
		}
		
		/*
			[2006.04.13] Modify by guiun
			RTTS 갱신은 5sec로 고정한다.
		*/
		if(::GetTickCount() - preTick_RTTServer > 5000 )
		{
//* 4.
			SP_CURRENT_RTTS_UPDATE( dateTime );
//*/
			preTick_RTTServer = ::GetTickCount();
		}

		/*  [2006.06.29] Add by guiun
			게임 통계 추가
		*/
		if(::GetTickCount() - dwTickGameStatistics > 60000)
		{
//* 5. 
			SP_CURRENT_GameStatistics_INSERT( dateTime );
//*/
			dwTickGameStatistics = ::GetTickCount();
		}

		//  Update 시간 갱신
		if( enableUpdateTime == true && ::GetTickCount() - preTick_UpdateTime > 30000 )
		{
			GetUpdateTime();
			ReloadWarningCanon();
			preTick_UpdateTime = ::GetTickCount();
		}
	}
}