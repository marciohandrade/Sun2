#include "SunClientPrecompiledHeader.h"
#include "LogManager.h"
#ifdef _DEV_VER
#if (WZENGINEVER >= 480)

#include <programcommon/wzlogsystem.h>
#include <programcommon/WzConsoleLogHandler.h>
#include <Client/3dengine/effect/dllaccess.h>
#include <Client/3dengine/3dterrain/dllaccess.h>
#include <Client/3dengine/3dfileaccess/dllaccess.h>
#include <Client/3dengine/3drenderer/dllaccess.h>


namespace LogManager
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    // 출력창에 표시하는 로그핸들러
    class ClientLogHandler : public WzLog::WzLogHandler
    {
        BOOL m_LevelFilter[WZNUM_LOGLVL];
        BOOL m_IsActiveState;

    public:
        ClientLogHandler()
        {
            memset( m_LevelFilter, 0, sizeof(BOOL) * WZNUM_LOGLVL );
            m_LevelFilter[WZERR] = TRUE;
            m_IsActiveState = FALSE;
        }
        void SetFilter( WZLOG_TYPE LogLevel, BOOL bEnable ) { m_LevelFilter[LogLevel] = bEnable; }  // override
        BOOL GetFilter( WZLOG_TYPE LogLevel ) { return m_LevelFilter[LogLevel]; } // override

        void SetActiveState( BOOL bState ) { m_IsActiveState = bState; }
    private:

        // override
        BOOL OnInitialize() { return TRUE; }
        void OnRelease() {}
        void WriteLog( WZLOG_TYPE level, const char* message )
        {
            if( m_IsActiveState == FALSE )
                return;

            if( m_LevelFilter[level] == FALSE )
                return;

            static const char lv[][WZNUM_LOGLVL] = { "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "RECV", "SEND", "ASSERT"  };    
            static char msg[2048];

            _snprintf( msg, sizeof( msg ) - 1, "[%s] %s\r\n", lv[level], message );

            OutputDebugString( msg );
        }
    };

    ClientLogHandler client_log_handler;

    ////////////////////////////////////////////////////////////////////////////////////////////

    void Initialize()
    {
        // 로그 설정을 초기화 GENERALPARAM이 로드된 후 호출되어야함을 주의

        InitializeWzLogSystem();


        if( const char* setting_text = GENERALPARAM->GetConsoleSetting() )
        {
            // console handler

            client_log_handler.Initialize();
            client_log_handler.SetFilter( WZERR, strstr( setting_text, "ERROR" ) != NULL );
            client_log_handler.SetFilter( WZINF, strstr( setting_text, "INFOMATION" ) != NULL );
            client_log_handler.SetFilter( WZWAR, strstr( setting_text, "WARNING" ) != NULL );
            client_log_handler.SetFilter( WZDBG, strstr( setting_text, "DEBUG" ) != NULL );
            client_log_handler.SetFilter( WZTRA, strstr( setting_text, "TRACE" ) != NULL );
            client_log_handler.SetFilter( WZPKT1, strstr( setting_text, "PKT_RECV" ) != NULL );
            client_log_handler.SetFilter( WZPKT2, strstr( setting_text, "PKT_SEND" ) != NULL );
            client_log_handler.SetFilter( WZPKT3, strstr( setting_text, "ETC" ) != NULL );
            AddWzLogHandler( &client_log_handler );

            Func_ContactEffect( CE_SET_LOGHANDLER, &client_log_handler );
            Func_ContactWzd( CW_SET_LOGHANDLER, &client_log_handler );
            Func_ContactTerrain( CT_SET_LOGHANDLER, &client_log_handler );
            Func_ContactRenderer( CR_SET_LOGHANDLER, &client_log_handler );

            if( GENERALPARAM->IsShowConsole() == TRUE )
            {
                client_log_handler.SetActiveState(TRUE);
            }
        }
    }

    void Release()
    {
        client_log_handler.Release();
        ReleaseWzLogSystem();
    }


    void ToggleConsoleView()
    {
        //console_log_handler.ToggleConsole();
    }


    void ProcessCommand( LPCTSTR command )
    {
        if( command == NULL )
            return;

        if( _tcslen( command ) == 0 )
            return;

        static const TCHAR lv[][WZNUM_LOGLVL] = { _T("E"), _T("W"), _T("I"), _T("D"), _T("T"), _T("RECV"), _T("SEND"), _T("ETC")  };    

        // 필터 켜기/끄기
        for( size_t i=0;i!=WzLogType::WZNUM_LOGLVL;++i)
        {
            WzLogType::WZLOG_TYPE Type = (WzLogType::WZLOG_TYPE)i;
            if( Strcmp( lv[i], command ) == 0 )
            {
                client_log_handler.SetFilter( Type, client_log_handler.GetFilter( Type ) == FALSE );
            }
        }

        // 기본 설정
        if( Strcmp( _T("기본"), command ) == 0 || Strcmp( _T("basic"), command ) == 0 )
        {
            client_log_handler.SetFilter( WzLogType::WZERR, TRUE );
            client_log_handler.SetFilter( WzLogType::WZWAR, FALSE );
            client_log_handler.SetFilter( WzLogType::WZINF, FALSE );
            client_log_handler.SetFilter( WzLogType::WZDBG, FALSE );
            client_log_handler.SetFilter( WzLogType::WZTRA, FALSE );
            client_log_handler.SetFilter( WzLogType::WZPKT1, TRUE );
            client_log_handler.SetFilter( WzLogType::WZPKT2, TRUE );
            client_log_handler.SetFilter( WzLogType::WZPKT3, FALSE );
            client_log_handler.SetActiveState( TRUE );
        }

        // 현재 설정을 표시
        OutputDebugString( "Console LogFilterState ------------------------------------\n" );
        for( size_t i=0;i!=WzLogType::WZNUM_LOGLVL;++i)
        {
            WzLogType::WZLOG_TYPE Type = (WzLogType::WZLOG_TYPE)i;
            char temp[1024];
            wsprintf( temp, "console log Filter[%s] : %s\n", lv[i], client_log_handler.GetFilter(Type) == TRUE ? "Enalbed" : "Disabled" );
            OutputDebugString( temp );
        }
        OutputDebugString( "-----------------------------------------------------------\n" );
    }

}

#endif // (WZENGINEVER >= 480)
#endif // _DEV_VER