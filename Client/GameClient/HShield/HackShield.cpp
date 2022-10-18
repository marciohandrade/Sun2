#include "SunClientPrecompiledHeader.h"
#include "HackShield.h"
#include "GlobalFunc.h"
#include "interfaceManager.h"
#include "GameFrameWork.h"
#include "resource.h"
#include "ClientVer.h"

#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
TCHAR HackShield::error_message_[MAX_PATH] = {0, };

//------------------------------------------------------------------------------
HackShield::HackShield():
check_variable_crc_(0)
{
    memset(title_, 0, sizeof(title_));
    memset(error_message_, 0, sizeof(error_message_));
}

HackShield::~HackShield()
{
}

BOOL HackShield::CheckCompatibilityMode()
{
#ifdef _CHECK_COMPATIBILITY_MODE
    /*
        GetVersionEx() 와 VerifyVersionInfo() 을 이용한 호환성여부 체크
        GetVersionEx() 을 이용한 OS버전정보를
        VerifyVersionInfo() 를 통하여 실제 OS버전정보인지 확인
        !> VerifyVersionInfo() 함수가 윈도우2000부터 지원됨.
    */
    OSVERSIONINFOEX os_version_info;
    ZeroMemory(&os_version_info, sizeof(OSVERSIONINFOEX));
    os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((OSVERSIONINFO*)&os_version_info);

    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
    BOOL check_verify = VerifyVersionInfo(&os_version_info, 
                                          VER_MAJORVERSION | VER_MINORVERSION, 
                                          dwlConditionMask);

    return check_verify;
#else
    return TRUE;
#endif //_CHECK_COMPATIBILITY_MODE
}

BOOL HackShield::HS_Init()
{
	GetWindowText(g_hWndMain, title_, sizeof(title_));

	int nRet = 0;
	TCHAR szFullFilePath[MAX_PATH];
	TCHAR szFullFileDLLPath[MAX_PATH];
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	GetModuleFileName(NULL, szFullFilePath, MAX_PATH);

	size_t len = _tcslen(szFullFilePath);
	for (size_t i = len; i > 0; --i)
	{
		if (szFullFilePath[i] == '\\')
			break;

		szFullFilePath[i] = '\0';
	}

	lstrcpy(szFullFileDLLPath, szFullFilePath);


//------------------------------------------------------------------------------
#ifdef	_SUNGAME_VER			// sungame.exe
    // 각 국가버전별 게임 코드 및 라이센스 코드 부여
    #if defined( _KOREA )
        char * license = "132370F56EA3A25145D48FB1";
        int	gamecode = 7012;
        char* hackshield_monitorconnect_IP = "127.0.0.1";
    #elif defined( _JAPAN )
        char * license = "26628E6F50BCC589D6B4192C";
        int	gamecode = 7018;
        char* hackshield_monitorconnect_IP = "127.0.0.1";
    #elif defined( _CHINA )
        char * license = "5B1EADF1FB22BB0B5587862E";
        int	gamecode = 7019;
        char* hackshield_monitorconnect_IP = "114.80.196.21";

        if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:중국, 3:대만
        {
            //! 대만
            license = "CBA5ED73840EBC165E95A851";
            gamecode = 7021;
            hackshield_monitorconnect_IP = "127.0.0.1";
        }

    #elif defined( _GLOBAL )
        // 글로벌 게임코드와 라이센스
        char * license = "5822055B34F0FFADD9E72461";
        int	gamecode = 7020;
        char* hackshield_monitorconnect_IP = "127.0.0.1";
    #else
        // 새로운 국가버전의 경우 기존대로 국내 코드를 임시로 사용
        char * license = "132370F56EA3A25145D48FB1";
        int	gamecode = 7012;
        char* hackshield_monitorconnect_IP = "127.0.0.1";
    #endif

#elif	defined( _GAMECLIENT_VER )
    char * license = "9E5D4B3960E56F6C176BFE56";	// gameclient.exe
    int	gamecode = 7014;
    char* hackshield_monitorconnect_IP = "127.0.0.1";
#elif	defined( _SUNGAME_DUMP_VER )
    char * license = "948B1F9C4EC58A1D87ACC7ED";	// sungame_dump.exe
    int	gamecode = 7015;
    char* hackshield_monitorconnect_IP = "127.0.0.1";
#else
    char * license = "9902A239DBEFEB010C31A26F";	// sungametest.exe	
    int	gamecode = 7013;
    char* hackshield_monitorconnect_IP = "127.0.0.1";
#endif

    //------------------------------------------------------------------------------
    AHNHS_EXT_ERRORINFO HsExtError; //HShield.h 에 정의된 구조체 
    ZeroMemory(&HsExtError, sizeof(HsExtError));
    strncpy(HsExtError.szServer, hackshield_monitorconnect_IP, MAX_PATH);
    strncpy(HsExtError.szGameVersion, c_tcBUILD_VERSION, MAX_PATH);
    strncpy(HsExtError.szUserId, "", MAX_PATH); //아이디를 확인할수 있는 시점에서 _AhnHS_SetUserId 호출
    //------------------------------------------------------------------------------

#ifdef	_AHN_SMART_UPDATE_
	lstrcat(szFullFilePath + i, _T("\\HShield"));
	
    #ifdef _APPLY_HACKSHIELD_MONITOR
        if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:중국, 3:대만
        {
            //! 대만
            nRet = _AhnHS_HSUpdate(szFullFilePath, 600000);
        }
        else
        {
            // Ex 함수를 사용하실때는 반드시 HSUpSetEnv.exe 설정 툴로 env 파일에 
            // 게임 코드를 입력하셔야 합니다.
            nRet = _AhnHS_HSUpdateEx(szFullFilePath,    // 핵쉴드 폴더 경로 
                                    1000 * 600,         // 업데이트 전체 타임 아웃 
                                    gamecode,           // 게임 코드 
                                    AHNHSUPDATE_CHKOPT_HOSTFILE | AHNHSUPDATE_CHKOPT_GAMECODE, 
                                    HsExtError, 
                                    1000* 20); // 서버 연결 타임아웃 
        }
    #else
        nRet = _AhnHS_HSUpdate(szFullFilePath, 600000);
    #endif

	if (nRet != HACKSHIELD_ERROR_SUCCESS)
	{
		switch (nRet)
		{
		case HSERROR_ENVFILE_NOTREAD:
		case HSERROR_ENVFILE_NOTWRITE:
		case HSERROR_NETWORK_CONNECT_FAIL:
		case HSERROR_LIB_NOTEDIT_REG:
		case HSERROR_NOTFINDFILE:
		case HSERROR_PROTECT_LISTLOAD_FAIL:
		case HSERROR_HSUPDATE_TIMEOUT:
		default:
    #ifdef	_KOREA
			wsprintf(szMessage, "업데이트 기능에 문제가 발생하였습니다.(%x)", nRet);
    #else
			wsprintf(szMessage, "An error has been occurred on the update functions.(%x)", nRet);
    #endif
			break;
		}

		MessageBox(NULL, szMessage, title_, MB_OK);
		return FALSE;
	}
#endif //_AHN_SMART_UPDATE_

	lstrcat(szFullFileDLLPath + i, _T("\\HShield\\EhSvc.dll"));

#ifdef _APPLY_HACKSHIELD_MONITOR
    if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:중국, 3:대만
    {
        //! 대만
    }
    else
    {
        //! 중국
        nRet = _AhnHS_StartMonitor(HsExtError, szFullFileDLLPath); 
        if (nRet != ERROR_SUCCESS) 
        { 
        #ifdef	_KOREA
            wsprintf(szMessage, "해킹방지 모니터링 기능에 문제가 발생하였습니다.(%x)", nRet);
        #else
            wsprintf(szMessage, "An error has been occurred on the anti-hacking monitor functions.(%x)", nRet);
        #endif
            MessageBox(NULL, szMessage, title_, MB_OK);

            return FALSE;
        }
    }
#endif //_APPLY_HACKSHIELD_MONITOR

    DWORD dwOption = AHNHS_CHKOPT_ALL;
    DWORD dwSensingRatio = AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL;
#ifdef _CHINA
    // 대만 호환성모드 이슈로 인한 시피드핵 민감도 설정
    dwSensingRatio = AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST;

    if (PROGRAMPARAM->nation_type() != kLanguageTaiwan)       //!< 2:중국, 3:대만
    {
        // 중국 - 핵쉴드 업데이트 체크 기능
        dwOption |= AHNHS_CHKOPT_UPDATED_FILE_CHECK;
    }
#endif

#ifdef _HACKSHIELD_CHECK_OPTION_LMP
    // LMP 옵션 추가
    dwOption |= AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION;
#endif //_HACKSHIELD_CHECK_OPTION_LMP

	nRet = _AhnHS_Initialize(szFullFileDLLPath, 
							 HS_CallbackProc,
							 gamecode,
							 license,
							 dwOption,
							 dwSensingRatio);

	if (nRet != HS_ERR_OK)
	{
		switch (nRet)
		{
		case HS_ERR_UNKNOWN:                          // 알 수 없는 오류가 발생했습니다.
		case HS_ERR_INVALID_PARAM:                    // 올바르지 않은 인자입니다.
		case HS_ERR_NOT_INITIALIZED:                  // 핵쉴드 모듈이 초기화되지 않은 상태입니다.
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:       // 현재 프로세스가 호환성 모드로 실행되었습니다.
		case HS_ERR_ALREADY_UNINITIALIZED:			  // Uninitialize가 수행된 이후에는 게임을 재실행한후 Initialize해야 합니다.
		case HS_ERR_INVALID_LICENSE:                  // 올바르지 않은 라이센스 키입니다.
		case HS_ERR_INVALID_FILES:                    // 잘못된 파일 설치되었습니다. 프로그램을 재설치하시기 바랍니다.
		case HS_ERR_INIT_DRV_FAILED:                  
		case HS_ERR_ANOTHER_SERVICE_RUNNING:          // 다른 게임이나 프로세스에서 핵쉴드를 이미 사용하고 있습니다.
		case HS_ERR_ALREADY_INITIALIZED:              // 이미 핵쉴드 모듈이 초기화되어 있습니다.
		case HS_ERR_DEBUGGER_DETECT:                  // 컴퓨터에서 디버거 실행이 감지되었습니다. 디버거의 실행을 중지시킨 뒤에 다시 실행시켜주시기바랍니다.
		case HS_ERR_EXECUTABLE_FILE_CRACKED:		  // 실행 파일의 코드가 크랙 되었습니다.
		case HS_ERR_NEED_ADMIN_RIGHTS:				  // ADMIN 권한이 필요합니다.
		case HS_ERR_START_ENGINE_FAILED:              // 해킹 프로그램 감지 엔진을 시작할 수 없습니다.
		case HS_ERR_ALREADY_SERVICE_RUNNING:          // 이미 핵쉴드 서비스가 실행 중입니다.
		case HS_ERR_DRV_FILE_CREATE_FAILED:           // 핵쉴드 드라이버 파일을 생성할 수 없습니다.
		case HS_ERR_REG_DRV_FILE_FAILED:              // 핵쉴드 드라이버를 등록할 수 없습니다.
		case HS_ERR_START_DRV_FAILED:                 // 핵쉴드 드라이버를 시작할 수 없습니다.
		case HS_ERR_SERVICE_NOT_RUNNING:              // 핵쉴드 서비스가 실행되고 있지 않은 상태입니다.
		case HS_ERR_SERVICE_STILL_RUNNING:            // 핵쉴드 서비스가 아직 실행중인 상태입니다.
		case HS_ERR_NEED_UPDATE:                      // 핵쉴드 모듈의 업데이트가 필요합니다.
		case HS_ERR_API_IS_HOOKED:					  // 해당 API가 후킹되어 있는 상태입니다.
		default:
#ifdef	_KOREA
			wsprintf(szMessage, "해킹방지 기능에 문제가 발생하였습니다.(%x)", nRet);
#else
			wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)", nRet);
#endif
			//g_InterfaceManager.GetInterfaceString( 6031, szMessage );
			//lstrcat( szMessage, "(%x)" );
			break;
		}

        MessageBox(NULL, szMessage, title_, MB_OK);
		return FALSE;
	}

	::SetUnhandledExceptionFilter(Game_UnhandledExceptionHandler);

	return CryptManager::InitInfo(NULL);
}

BOOL HackShield::HS_StartService()
{
	int nRet = 0;
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	nRet = _AhnHS_StartService();

	if (nRet != HS_ERR_OK)
	{
		switch (nRet)
		{
		case HS_ERR_UNKNOWN:                          // 알 수 없는 오류가 발생했습니다.
		case HS_ERR_INVALID_PARAM:                    // 올바르지 않은 인자입니다.
		case HS_ERR_NOT_INITIALIZED:                  // 핵쉴드 모듈이 초기화되지 않은 상태입니다.
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:       // 현재 프로세스가 호환성 모드로 실행되었습니다.
		case HS_ERR_ALREADY_UNINITIALIZED:			  // Uninitialize가 수행된 이후에는 게임을 재실행한후 Initialize해야 합니다.
		case HS_ERR_INVALID_LICENSE:                  // 올바르지 않은 라이센스 키입니다.
		case HS_ERR_INVALID_FILES:                    // 잘못된 파일 설치되었습니다. 프로그램을 재설치하시기 바랍니다.
		case HS_ERR_INIT_DRV_FAILED:                  
		case HS_ERR_ANOTHER_SERVICE_RUNNING:          // 다른 게임이나 프로세스에서 핵쉴드를 이미 사용하고 있습니다.
		case HS_ERR_ALREADY_INITIALIZED:              // 이미 핵쉴드 모듈이 초기화되어 있습니다.
		case HS_ERR_DEBUGGER_DETECT:                  // 컴퓨터에서 디버거 실행이 감지되었습니다. 디버거의 실행을 중지시킨 뒤에 다시 실행시켜주시기바랍니다.
		case HS_ERR_EXECUTABLE_FILE_CRACKED:		  // 실행 파일의 코드가 크랙 되었습니다.
		case HS_ERR_NEED_ADMIN_RIGHTS:				  // ADMIN 권한이 필요합니다.
		case HS_ERR_START_ENGINE_FAILED:              // 해킹 프로그램 감지 엔진을 시작할 수 없습니다.
		case HS_ERR_ALREADY_SERVICE_RUNNING:          // 이미 핵쉴드 서비스가 실행 중입니다.
		case HS_ERR_DRV_FILE_CREATE_FAILED:           // 핵쉴드 드라이버 파일을 생성할 수 없습니다.
		case HS_ERR_REG_DRV_FILE_FAILED:              // 핵쉴드 드라이버를 등록할 수 없습니다.
		case HS_ERR_START_DRV_FAILED:                 // 핵쉴드 드라이버를 시작할 수 없습니다.
		case HS_ERR_SERVICE_NOT_RUNNING:              // 핵쉴드 서비스가 실행되고 있지 않은 상태입니다.
		case HS_ERR_SERVICE_STILL_RUNNING:            // 핵쉴드 서비스가 아직 실행중인 상태입니다.
		case HS_ERR_NEED_UPDATE:                      // 핵쉴드 모듈의 업데이트가 필요합니다.
		case HS_ERR_API_IS_HOOKED:					  // 해당 API가 후킹되어 있는 상태입니다.
        case HS_ERR_LMP_START:                        // LMP 기능을 시작할 수 없습니다.
		default:
#ifdef	_KOREA
			wsprintf(szMessage, "해킹방지 기능에 문제가 발생하였습니다.(%x)", nRet);
#else
			wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)", nRet);
#endif
			break;
		}

		MessageBox(NULL, szMessage, title_, MB_OK);
		return FALSE;
	}

#ifdef _CHECK_COMPATIBILITY_MODE
    // 호환성 모드의 경우 클라이언트 종료
    if (CheckCompatibilityMode() == FALSE)
    {
        MessageBox(NULL, "Can not use Compatibility Mode", NULL, MB_OK);
        return FALSE;
    }
#endif //_CHECK_COMPATIBILITY_MODE

	return TRUE;
}

BOOL HackShield::HS_StopService()
{
	int nRet = 0;

	nRet = _AhnHS_StopService();

	if (nRet != HS_ERR_OK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL HackShield::HS_UnInit()
{
	int nRet = 0;

	nRet = _AhnHS_Uninitialize();

	if (nRet != HS_ERR_OK)
	{
		return FALSE;
	}

	return TRUE;

}

int	__stdcall HackShield::HS_CallbackProc(long lCode, long lParamSize, void* pParam)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	switch (lCode)
	{
	case AHNHS_ENGINE_DETECT_GAME_HACK:           			// 게임 해킹툴의 실행이 발견되었습니다.
#ifdef	_KOREA
		wsprintf(szMessage, "다음의 프로그램과 게임이 함께 실행될수 없습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "The game can not be executed with the following program.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		
		HSMessageBox(szMessage);
		break;

	case AHNHS_ACTAPC_DETECT_AUTOMACRO:						// 오토메크로 행위가 감지되었습니다.	
#ifdef	_KOREA
		wsprintf(szMessage, "매크로 기능으로 의심되는 동작이 감지되었습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An operation whitch seems to be Auto-macro has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:						// 오토마우스 행위가 감지되었습니다.	
	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:					// 일부 API가 이미 후킹되어 있는 상태입니다. (그러나 실제로는 이를 차단하고 있기 때문에 후킹프로그램은 동작하지 않습니다.)
		break;

	case AHNHS_ACTAPC_DETECT_SPEEDHACK:						// 스피드핵류의 프로그램에 의해 시스템 시간이 변경되었습니다.
#ifdef	_KOREA
		wsprintf(szMessage, "스피드핵으로 의심되는 동작이 감지되었습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An operation whitch seems to be Speed-hack has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_KDTRACE:							// 디버거 트래이싱이 발생했다.(커널 디버거 활성화, 이후 브레이크 포인터 처리)
	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:					// 설치된 디버거 트래이싱이 변경되었다.
#ifdef	_KOREA
		wsprintf(szMessage, "게임에 대하여 디버깅 시도가 감지되었습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "A debugging attempt to the game has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;


	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:					// 해킹 차단 드라이버가 로드되지 않았습니다.
	case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:						// 메시지 후킹이 시도되었으며 이를 차단하지 못했습니다.
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:					// 보호 API에 대한 후킹 행위가 감지되었습니다.
	case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:       			// 핵쉴드 관련모듈이 변경되었습니다.
#ifdef	_KOREA
		wsprintf(szMessage, "해킹 방어 기능에 이상이 발생하였습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO:				// 스피드핵 감지 옵션이 'GAME'이 경우 이 콜백으로 최근 5초동안의 시간정보가 전달됩니다.
	case AHNHS_ENGINE_DETECT_WINDOWED_HACK:					// 창모드 해킹툴
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:		// 코드 패치 시도가 감지되었습니다.
		break;
	case AHNHS_ACTAPC_DETECT_ENGINEFAILED:	    			// 엔진기능이 정상적이지 않습니다.
	case AHNHS_ACTAPC_DETECT_CODEMISMATCH:					// 코드가 일치하지 않습니다. 
    case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:           // 보호 모듈에 대해서 메모리 변조가 감지되었습니다.
    case AHNHS_ACTAPC_DETECT_LMP_FAILED:                    // 핵쉴드 로컬 메모리 보호 기능이 정상적이지 않습니다.
	case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
	case AHNHS_ACTAPC_DETECT_ANTIFREESERVER:
#ifdef	_KOREA
		wsprintf(szMessage, "해킹으로 인해 핵쉴드 파일에 이상이 생겼습니다.(%x)\n[%s]", lCode, (LPTSTR)pParam); 
#else
		wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)\n[%s]", lCode, (LPTSTR)pParam); 
#endif
		HSMessageBox(szMessage);
		break;
	}

	return 1;
}

LRESULT CALLBACK HackShield::MessageBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            SetWindowText(hDlg, "Error");
			SetDlgItemText(hDlg, IDC_MESSAGE_TEXT, error_message_);

            SetTimer(hDlg, 1, 3000, (TIMERPROC) NULL);
		}
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog(hDlg, LOWORD(wParam));
            GlobalFunc::LogOutGame();
            return TRUE;
        }
        break;

    case WM_TIMER:
        {
            switch(wParam)
            {
            case 1:
                {
					EndDialog(hDlg, 0);
					GlobalFunc::LogOutGame();
                }
                break;
            }
        }
        break;
    }
    return FALSE;

}

void HackShield::HSMessageBox(LPCTSTR szMessage)
{
	if (FALSE == GlobalFunc::SysMessageBox(szMessage, TRUE))
	{
		_tcscpy(error_message_, szMessage);
		DialogBox(g_hInstance, (LPCTSTR)IDD_MESSAGE_BOX, NULL, (DLGPROC)MessageBoxProc);
	}
}

LONG WINAPI HackShield::Game_UnhandledExceptionHandler(struct _EXCEPTION_POINTERS * lpTopLevelExcptionFilter)
{
    HackShield::Instance()->HS_StopService();
    HackShield::Instance()->HS_UnInit();
    HackShield::DestroyInstance();

	return EXCEPTION_CONTINUE_SEARCH;
}
#endif