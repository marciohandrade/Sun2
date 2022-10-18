/*!
 *	\file		Application.h
 *	\brief		Application class
 *  \bug		
 *	\todo		�Լ��� doxygen command �ޱ�
 *  \author		���ǥ (lammy@webzen.co.kr)
 *	\version	2008/12/19	���ǥ - doxygen command �ۼ�
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


// �����ս� ������� �����.
typedef struct _VIRTUAL_PACKET
{
	DWORD dwServerIndex;
	BYTE  byData[VIRTUAL_PACKET_BUFFER_SIZE];
} VIRTUAL_PACKET;


/*!
 *  \class	Application class 
 *	\brief	Application class 
 *	\author	���ǥ (lammy@webzen.co.kr)
 */

const int   MAX_PRELOAD_RESOURCE = 24;  // ��ȯ��4 + ĳ����10(Ŭ����*����) + Ż��10

class Application : public CAppLast  
{
	friend VOID NetworkRecv( DWORD dwIndex, BYTE * pData, DWORD dwSize );

public:
	Application( void );
	virtual ~Application( void );
	virtual void Clear( void );

public:
	/*!
		Application �� �����Ѵ�. 	
	 	\param[in]	hInstance		������ hInstance
	 	\param[in]	lpszAppName		app ����
	 	\param[in]	WndProc			������ ���ν���
	 	\param[in]	lpszIconName	������ ����
	 	\param[in]	nCmdShow		Ŀ��Ʈ ���
	 	\return		TRUE			����
		\return		FALSE			����
	 */
	virtual BOOL Create( HINSTANCE hInstance, LPCTSTR lpszAppName, WNDPROC WndProc, LPCTSTR lpszIconName, int nCmdShow );
	/*!
	Application �� �ı��Ѵ�.
	\note		�Ŵ���, ��ü ���� �ı� �Ǵ� ����, ������ ����
	*/
	virtual void Destroy( void );
	/*!
	Application �� ������ ���ν���
	\param[in]	hWnd			������ �ڵ�
	\param[in]	iMessage		������ �޽���
	\param[in]	wParam			�޽��� �Ķ����1
	\param[in]	lParam			�޽��� �Ķ����2
	*/
	virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
	/*!
	HeartBeat ����ڵ���
	\note		������ ������Ʈ �� ���� ����
				������Ʈ �� ������ �ʿ��� ƽ��� �� �����ӿ�ũ, �Ŵ���, ��ü�� ƽ ����
	*/
	virtual int HeartBeat( void );
	/// DiscAccess ��ȯ
	CDiscAccess * GetDiscAccess();
	/// ���� ��� �� ���� ������ ��� ����( ����� ���� - '..\\_bin' )
    void InitRegistry(HINSTANCE hInstance);

	void PreloadResource();
	void UnloadPreloadResource();

	///	���� ���� ���� �ʱ�ȭ
	void InitSound();
	///	���� ���� ���� ����
	void ReleaseSound();

	/*!
	��Ʈ��ũ ��ü ���� �� �ʱ�ȭ
	\note		NetworkRecv			���� ��Ŷ�� �����ϴ� �Լ�
				NetworkDisconnect	Disconnet�� ó�� �Լ�
				NetworkError		��Ʈ�� ���� �α� �Լ�
	*/
	void InitNet();
	/// ���� ���� ����
    BOOL ConnectToAuthServer();
	/// ��Ʈ��ũ ���� ���� �� ��ũ��ũ ��ü ����
    void ReleaseNet();
	/// GAMEOPTION �ε� �� �ʱ�ȭ, QuestManager_Concrete ��MissionManager_Concrete ����
	BOOL LoadGameInfo();
	/// ��ũ��Ʈ �ε� �� AC �ʱ�ȭ
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
	// Performance ���� ����
	void _renderPerfInfo();
	void _renderPerformanceStatus();
	void _renderProfile();
	void _renderProcessProfile();

    static int NewHandler(size_t);
    static void PureCallHandler();
    static void BufferOverflowHandler(int code, void* unused);

protected:
	void SendToChatHeartbeatSyn();	// HeartBeat syn ����

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

    //! ����ȭ�� ��ũ�� ũ��
    POINT				desktop_screen_size_;

	BOOL				m_bRunningGame;
	BOOL				m_bGameInput;

	HANDLE              m_hConsole;

    // ���� ƽ
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

