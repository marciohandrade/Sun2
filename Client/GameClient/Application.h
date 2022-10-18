/*!
 *	\file		Application.h
 *	\brief		Application class
 *  \bug		
 *	\todo		함수에 doxygen command 달기
 *  \author		김경표 (lammy@webzen.co.kr)
 *	\version	2008/12/19	김경표 - doxygen command 작성
 */

#ifndef		_Application_H
#define		_Application_H

#include "PacketStruct_CW.h"

class GameFramework;

#define VIRTUAL_PACKET_BUFFER_SIZE   1024

#include "RankingManager.h"
#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
#include "WIC/GWICWrapper.h"
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
#ifdef _DH_DEBUG_REPORT
#include <dbghelp.h>
#endif//_DH_DEBUG_REPORT


// 퍼포먼스 상관없이 만든다.
typedef struct _VIRTUAL_PACKET
{
	DWORD dwServerIndex;
	BYTE  byData[VIRTUAL_PACKET_BUFFER_SIZE];
} VIRTUAL_PACKET;


/*!
 *  \class	Application class 
 *	\brief	Application class 
 *	\author	김경표 (lammy@webzen.co.kr)
 */

const int   MAX_PRELOAD_RESOURCE = 24;  // 소환수4 + 캐릭터10(클래스*성별) + 탈것10

class Application : public CAppLast  
{
	friend VOID NetworkRecv( DWORD dwIndex, BYTE * pData, DWORD dwSize );

public:
	Application( void );
	virtual ~Application( void );
	virtual void Clear( void );

public:
	/*!
		Application 을 생성한다. 	
	 	\param[in]	hInstance		윈도우 hInstance
	 	\param[in]	lpszAppName		app 네임
	 	\param[in]	WndProc			윈도우 프로시저
	 	\param[in]	lpszIconName	아이콘 네임
	 	\param[in]	nCmdShow		커맨트 쇼우
	 	\return		TRUE			성공
		\return		FALSE			실패
	 */
	virtual BOOL Create( HINSTANCE hInstance, LPCTSTR lpszAppName, WNDPROC WndProc, LPCTSTR lpszIconName, int nCmdShow );
	/*!
	Application 을 파기한다.
	\note		매니저, 객체 등을 파기 또는 해제, 세마포 종료
	*/
	virtual void Destroy( void );
	/*!
	Application 의 윈도우 프로시저
	\param[in]	hWnd			윈도우 핸들
	\param[in]	iMessage		윈도우 메시지
	\param[in]	wParam			메시지 파라미터1
	\param[in]	lParam			메시지 파라미터2
	*/
	virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
	/*!
	HeartBeat 심장박동기
	\note		프레임 업데이트 및 렌더 결정
				업데이트 및 렌더에 필요한 틱계산 후 프레임워크, 매니저, 객체에 틱 전파
	*/
	virtual int HeartBeat( void );
	/// DiscAccess 반환
	CDiscAccess * GetDiscAccess();
	/// 게임 경로 및 게임 데이터 경로 설정( 디버그 전용 - '..\\_bin' )
    void InitRegistry(HINSTANCE hInstance);

	void PreloadResource();
	void UnloadPreloadResource();

	///	웹젠 사운드 엔진 초기화
	void InitSound();
	///	웹젠 사운드 엔진 해제
	void ReleaseSound();

	/*!
	네트워크 객체 생성 및 초기화
	\note		NetworkRecv			받은 패킷을 전달하는 함수
				NetworkDisconnect	Disconnet시 처리 함수
				NetworkError		네트웍 에러 로그 함수
	*/
	void InitNet();
	/// 인증 서버 접속
    BOOL ConnectToAuthServer();
	/// 네트워크 접속 해제 및 네크워크 객체 삭제
    void ReleaseNet();
	/// GAMEOPTION 로딩 및 초기화, QuestManager_Concrete 및MissionManager_Concrete 생성
	BOOL LoadGameInfo();
	/// 스크립트 로드 및 AC 초기화
	void LoadGameInfoForSriptParserMng();
	void UnloadGameInfo();
	void ReleaseManager();

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
#else //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
	BOOL CheckOverlapExecute();
#endif // _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
	
	void AddVirtualPacket(DWORD dwServerIndex,BYTE *pPacket,int iSize);
	void ParseVirtualPacket();
	void InitializeVirtualPacket();

	void ProcessEffectSound();

    ENUM_SCREEN_RATIO screen_ratio();

	BOOL IsWideModeFlag();
    POINT desktop_screen_size() const { return desktop_screen_size_; }
    void set_desktop_screen_size(int width, int height) { desktop_screen_size_.x = width; desktop_screen_size_.y = height; }

	void Set2DMode(BOOL bFlag = TRUE);

	void OnChangeRes(POINT *pPoint);
	BOOL CheckScreenSize();
	
	POINT *GetScreenSize( void ) { return &m_ScreenSize; }
	POINT Get2DModeMapping( void );

	void ProcessDebugInput();
	void UpdateGlobalVariable();

	//
	// WORLD SEREVR RECCONECT
    //
    void OnReconnectToWorldServer();
    void OffReconnectToWorldServer();
    void CheckReconnectToWorldServer();
    void ForceReconnectToWorldServer();

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	RankingManager *        GetRankingManager(){ return &m_RankManager;}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	BOOL IsFocus() { return m_bFocus;}

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
    BOOL InvokeCash();
    BOOL IsInitWIC() { return web_in_client.IsInitialize(); }
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

private:
	// Performance 관련 정보
	void _renderPerfInfo();
	void _renderPerformanceStatus();
	void _renderProfile();
	void _renderProcessProfile();

    static int NewHandler(size_t);
    static void PureCallHandler();
    static void BufferOverflowHandler(int code, void* unused);

protected:
	void SendToChatHeartbeatSyn();	// HeartBeat syn 관련

	void CreateConsole();
	void ProcessConsole();

protected:
	GameFramework*		m_GameFramework;
	CDiscAccess *		m_pDiscAccess;

	DWORD				m_dwHeartBeatTimer;

	MSG_CW_HEARTBEAT	m_CW_HeartbeatMsg;
	vector<VIRTUAL_PACKET *> m_VirtualPacket;
	vector<VIRTUAL_PACKET *> m_RecvVirtualPacket;
	POINT				m_ScreenSize;

    //! 바탕화면 스크린 크기
    POINT				desktop_screen_size_;

	BOOL				m_bRunningGame;
	BOOL				m_bGameInput;

	HANDLE              m_hConsole;

    // 현재 틱
    DWORD               m_dwCurrentTick;

    BOOL                m_bCheckToWorldServerReconnect;
    DWORD               m_dwStartTickToWorldServerReconnect;
    DWORD               m_dwOverTickToWorldServerReconnect;
    int                 m_iTryCountToWroldServerReconnect;

	int                 m_iDebugInfo;

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
#else //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
	HANDLE				m_hSemaphore;
#endif //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

#ifdef GGUO_CRASH_DUMP
#ifdef _DH_DEBUG_REPORT
    static LONG WINAPI		ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo );
    static MINIDUMP_TYPE	s_eDumpType;
    static TCHAR			s_pszDumpFileDir[MAX_PATH*2];
    static TCHAR			s_pszFilePrefix[MAX_PATH];
    static TCHAR			s_pszBuildVersion[MAX_PATH];
    static LPEXCEPTION_POINTERS* g_ExceptionInfo;
#else//_DH_DEBUG_REPORT
    static LONG WINAPI		ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo );
#endif//_DH_DEBUG_REPORT
#endif//GGUO_CRASH_DUMP

    BOOL                m_bFocus;


#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	RankingManager		m_RankManager;
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	TCHAR				m_szProfile[100][100];
	int					m_iProfile[100];

	HANDLE m_hTextureOnHead;

public:
	CWzUnitDraw *l_pPreloadResource[MAX_PRELOAD_RESOURCE]; 
	DWORD l_dwPreloadResource[MAX_PRELOAD_RESOURCE];

	HANDLE GetTextureOnHeadHandle(){return m_hTextureOnHead;}


#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
    GWICWrapper web_in_client;
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
};


extern Application * g_pApplication;


#define GET_MISSION_RANK_MANAGER()	g_pApplication->GetRankingManager()


#endif	//	_Application_H

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
inline BOOL Application::InvokeCash()
{
    if (IsInitWIC() == TRUE)
    {
        return web_in_client.InvokeCash();
    }
    return FALSE;
}
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

