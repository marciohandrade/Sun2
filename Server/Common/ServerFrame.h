#pragma once
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/// ServerFrame class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 8. 22
	@remark
		- 서버 프레임 워크의 최상위 클래스
	@note
		- 하위 클래스에서 구현 해야 한다.
	@history 
		- 2006.10.30~	: (Waverix) 자잘한 기능 및 버그 수정
		- 2007.11.05	: (Waverix) 코드 정리 및 Append Assertion Log Handler on Release Mode
*/
//=============================================================================================================================

#include <ServerStruct.h>
#include <IOCPServer.h>
#include <Timer.h>
#include <hash_map>
#include <algorithm>
#include "ReadyState.h"

#ifdef _JP_0_20101123_PMS
#include "pms/PMSManager.h"
#endif

//==================================================================================================
//
class ServerSession;
class NetworkObject;
struct MSG_BASE;
struct MSG_BASE_INTERNAL;

enum eSERVER_MODE				
{
	eSERVER_DEV			= 1,		// 개발용으로 서버를 작동
	eSERVER_INTRAOFFICE,			// 사내용으로 서버를 작동
	eSERVER_LIVE,					// 실제 서비스용으로 서버를 작동
	eSERVER_TESTLIVE,
	eSERVER_UPPERBOUND,
};

enum eIOHANDLER
{
	SERVER_IOHANDLER = 0x01,
	CLIENT_IOHANDLER = 0x02,
};

#define __READ_CONFIG_NUMBER( outValue, szSectionName, keyName, defValue, ini )			\
	outValue = GetPrivateProfileInt( szSectionName, keyName,  defValue, ini)
#define __READ_CONFIG_STRING( outString, szSectionName, keyName, defValue, ini )		\
	GetPrivateProfileString( szSectionName, keyName, defValue, outString, 256, ini)
#define __READ_CONFIG_STRING_IP( outString, szSectionName, keyName, defValue, ini )		\
	GetPrivateProfileString( szSectionName, keyName, defValue, outString, 16, ini)

#define P_SUCCESS			DISPMSG("SUCCESS!\n")
#define P_FAILED			DISPMSG("FAILED!\n")

//==================================================================================================
namespace util {
;

struct ServerTimeInfoPerFrame : public util::TimeInfoPerFrame
{
    void Init();
    void Update();
    //
    ulong aging_counter_; // increasing 1 per second
    // (CHANGES) (f100415.1L) (WAVERIX) fix up delta tick error related to degree of accuracy
    LARGE_INTEGER time_counter_for_start_;
    LARGE_INTEGER time_counter_for_latest_;
    LARGE_INTEGER frequency_; // TODO(WAVERIX) change access authorization to private
};

}; //end of namespace

//==================================================================================================
//
class ServerFrame
{
protected:	typedef STLX_HASH_MAP<DWORD, ServerSession *>	SERVER_MAP;

public:		ServerFrame(VOID);
	virtual ~ServerFrame(VOID);

public:		BOOL				DoInit(
									const CHAR* Name,
									fnCallBackCreateAcceptedObject		FnAllocAcceptServer,
									fnCallBackDestroyAcceptedObject		FnFreeAcceptServer,
									fnCallBackDestroyConnectedObject	FnFreeConnectServer,
									fnCallBackCreateAcceptedObject		FnAllocAcceptClient,
									fnCallBackDestroyAcceptedObject		FnFreeAcceptClient,
									fnCallBackDestroyConnectedObject	FnFreeConnectClient
									);
			VOID				DoRelease();
			VOID				DoProcess();
			BOOL				DoUpdate();

	__inline VOID				Shutdown()				{ m_bShutdown = TRUE; }
	__inline BOOL				IsServerInfoFromFile()	{ return m_bGetServerInfoFromFile; }
	virtual eSERVER_TYPE		GetServerType() = 0;
			VOID				UpdateAppTitle();

	__inline ServerSession*		FindSession( DWORD idx )	
	{ 
		SERVER_MAP::iterator it = m_SessionMap.find( idx );
		return (it != m_SessionMap.end()) ? it->second : NULL ;
	}

	// 좋은 방법은 아니나, 이것에 관련해 접근하는 방법이 각 Session*로 시작하는 for_each외에 없는 관계로...-_-;
	// 아님 아래 주석과 같은 시작,끝 만을 얻어서 사용할까?
	__inline SERVER_MAP&		GetSessionList() { return m_SessionMap; }

	// connection이 이루어졌을 때
	__inline VOID				AddSession( DWORD idx, ServerSession* pSession )	{ std::pair<SERVER_MAP::iterator,bool> rt = m_SessionMap.insert( SERVER_MAP::value_type(idx, pSession) ); ASSERT(rt.second); 
#ifdef _JP_0_20101123_PMS
                                SetTotalNumberOfConnections();
#endif
                                }
	// connection이 끊어졌을 때
	__inline VOID				RemoveSession( DWORD idx )				{ m_SessionMap.erase(idx); 
#ifdef _JP_0_20101123_PMS
                                SetTotalNumberOfConnections();
#endif
                                }
			VOID				AddConnectionlessConnector( ServerSession* pSession );
	__inline VOID				RemoveConnectionlessConnector( DWORD idx )	{ m_ConnectionlessMap.erase(idx); }

			BOOL				SendToSession( DWORD idx, MSG_BASE_INTERNAL* pMsg, WORD wSize );
			VOID				SendToSessionType( eSERVER_TYPE type, MSG_BASE_INTERNAL* pMsg, WORD wSize, DWORD dwExceptIndex=0 );
			BOOL				SendToSession( DWORD idx, MSG_BASE* pMsg, WORD wSize );
			VOID				SendToSessionType( eSERVER_TYPE type, MSG_BASE* pMsg, WORD wSize, DWORD dwExceptIndex=0 );

	virtual VOID				SendServerType( ServerSession* pSession );
	virtual ServerSession*		AllocServerSession( eSERVER_TYPE ) = 0;
	virtual VOID				FreeServerSession( ServerSession* ) = 0;

			BOOL				IsListening( DWORD dwKey );
			VOID				Listen( DWORD dwKey );
			VOID				SuspendListenThread( DWORD dwKey );
			VOID				ResumeListenThread( DWORD dwKey );

			ReadyState &		GetReadyState()		{ return m_ReadyState; }
			ServerSession*		InitConnectSession( eSERVER_TYPE type, CHAR* pszIP, WORD wPort );

	__inline SERVER_KEY			GetKey() { return m_Key; }
	__inline DWORD				GetGuid() { return m_Guid; }
	__inline eSERVER_MODE		GetMode() { return (eSERVER_MODE)m_Mode;	}

#ifdef _JP_0_20101123_PMS
    __inline PMSManager*        GetPMSManager() {return &pms_manager_;}
             VOID               SetTotalNumberOfConnections();
             VOID               DisplayPMSInfo();
#endif

	__inline VOID				SetGuid( DWORD guid ) { m_Guid = guid; }
    __inline VOID   	        SetKey( SERVER_KEY key ) { m_Key = key; 
#ifdef _JP_0_20101123_PMS
                                pms_manager_.SetCategory(key.m_dwKey);
#endif
                                }
	__inline VOID				SetListenIP_s( const CHAR* ip ) { strncpy( m_pszListenIP_s, ip, 16 ); m_pszListenIP_s[15] = '\0'; }
	__inline VOID				SetListenIP_c( const CHAR* ip ) { strncpy( m_pszListenIP_c, ip, 16 ); m_pszListenIP_c[15] = '\0'; }
	__inline VOID				SetListenPort_s( const WORD port ) { m_wListenPort_s = port; }
	__inline VOID				SetListenPort_c( const WORD port ) { m_wListenPort_c = port; }
    __inline const WORD			GetListenPort_s()	{ return m_wListenPort_s; }
    __inline const WORD			GetListenPort_c()	{ return m_wListenPort_c; }

	inline VOID					StartNetworkBlock4Closing() { m_bNetworkBlock4Closing = TRUE; }
    const util::ServerTimeInfoPerFrame& GetTimeInfoPerFrame() const;
			template<typename _OPERATOR>
			VOID				for_each( _OPERATOR& op );
protected:
			VOID				FrameCheck();
			VOID				DisplayAppInfo( BOOL bLog = FALSE );
			VOID				DisplayServerList();
			VOID				MaintainConnection();
			BOOL				ProcessConsole();
			VOID				Listen( DWORD dwKey, const CHAR* ip, WORD port );
			VOID				Connect( INT Num );
			VOID				Connect( ServerSession* pNetObject );
			VOID				RunUnitTest();

	virtual BOOL				Init() = 0;
	virtual BOOL				Update( DWORD tick ) = 0;
	virtual VOID				Release() = 0;
	virtual BOOL				KeyEvent(CHAR ch) { ch; return TRUE; }

	__inline const CHAR*		GetDescFileName()	{ return m_pszDescFileName; }
	__inline const CHAR*		GetListenIP_s()		{ return m_pszListenIP_s; }
	__inline const CHAR*		GetListenIP_c()		{ return m_pszListenIP_c; }
	__inline IOCPServer*		GetIOCPServer()		{ return m_pIOCPServer; }

private:
			BOOL				_Update();

private:	DWORD				m_Guid;
			SERVER_KEY			m_Key;
			INT					m_Mode;
			INT					m_HandlerMask;
			BOOL				m_bShutdown;
			BOOL				m_bNetworkBlock4Closing;
protected:	util::Timer			m_ReconnectTimer;
private:	DWORD				m_dwFPS;
			IOCPServer*			m_pIOCPServer;
			BOOL				m_bGetServerInfoFromFile;

			CHAR				m_pszListenIP_s[16];
			WORD				m_wListenPort_s;
			CHAR				m_pszListenIP_c[16];
			WORD				m_wListenPort_c;
			CHAR				m_pszDescFileName[64];

			// Connection이 연결된 세션을 가진 리스트
			SERVER_MAP			m_SessionMap;								//< connection이 유지된 세션의 리스트
			SERVER_MAP			m_ConnectionlessMap;
			ReadyState			m_ReadyState;
#ifdef _JP_0_20101123_PMS
            PMSManager          pms_manager_;
#endif
public: //temporary
    const LARGE_INTEGER& m_freq; // TODO(WAVERIX) change access authorization to private
private:
    // CHANGES: f110805.1L, globally server time information accessability per frame.
    util::ServerTimeInfoPerFrame time_info_per_frame_;
    //
    __DISABLE_COPY(ServerFrame);
};

template<typename _OPERATOR>
VOID		ServerFrame::for_each( _OPERATOR& op )
{
	std::for_each( m_SessionMap.begin(), m_SessionMap.end(), 
		compose1( op, select2nd<SERVER_MAP::value_type>() ) );
}

inline const util::ServerTimeInfoPerFrame& ServerFrame::GetTimeInfoPerFrame() const {
    return time_info_per_frame_;
};
