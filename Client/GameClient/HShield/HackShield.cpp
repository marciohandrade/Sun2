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
        GetVersionEx() �� VerifyVersionInfo() �� �̿��� ȣȯ������ üũ
        GetVersionEx() �� �̿��� OS����������
        VerifyVersionInfo() �� ���Ͽ� ���� OS������������ Ȯ��
        !> VerifyVersionInfo() �Լ��� ������2000���� ������.
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
    // �� ���������� ���� �ڵ� �� ���̼��� �ڵ� �ο�
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

        if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:�߱�, 3:�븸
        {
            //! �븸
            license = "CBA5ED73840EBC165E95A851";
            gamecode = 7021;
            hackshield_monitorconnect_IP = "127.0.0.1";
        }

    #elif defined( _GLOBAL )
        // �۷ι� �����ڵ�� ���̼���
        char * license = "5822055B34F0FFADD9E72461";
        int	gamecode = 7020;
        char* hackshield_monitorconnect_IP = "127.0.0.1";
    #else
        // ���ο� ���������� ��� ������� ���� �ڵ带 �ӽ÷� ���
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
    AHNHS_EXT_ERRORINFO HsExtError; //HShield.h �� ���ǵ� ����ü 
    ZeroMemory(&HsExtError, sizeof(HsExtError));
    strncpy(HsExtError.szServer, hackshield_monitorconnect_IP, MAX_PATH);
    strncpy(HsExtError.szGameVersion, c_tcBUILD_VERSION, MAX_PATH);
    strncpy(HsExtError.szUserId, "", MAX_PATH); //���̵� Ȯ���Ҽ� �ִ� �������� _AhnHS_SetUserId ȣ��
    //------------------------------------------------------------------------------

#ifdef	_AHN_SMART_UPDATE_
	lstrcat(szFullFilePath + i, _T("\\HShield"));
	
    #ifdef _APPLY_HACKSHIELD_MONITOR
        if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:�߱�, 3:�븸
        {
            //! �븸
            nRet = _AhnHS_HSUpdate(szFullFilePath, 600000);
        }
        else
        {
            // Ex �Լ��� ����ϽǶ��� �ݵ�� HSUpSetEnv.exe ���� ���� env ���Ͽ� 
            // ���� �ڵ带 �Է��ϼž� �մϴ�.
            nRet = _AhnHS_HSUpdateEx(szFullFilePath,    // �ٽ��� ���� ��� 
                                    1000 * 600,         // ������Ʈ ��ü Ÿ�� �ƿ� 
                                    gamecode,           // ���� �ڵ� 
                                    AHNHSUPDATE_CHKOPT_HOSTFILE | AHNHSUPDATE_CHKOPT_GAMECODE, 
                                    HsExtError, 
                                    1000* 20); // ���� ���� Ÿ�Ӿƿ� 
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
			wsprintf(szMessage, "������Ʈ ��ɿ� ������ �߻��Ͽ����ϴ�.(%x)", nRet);
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
    if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:�߱�, 3:�븸
    {
        //! �븸
    }
    else
    {
        //! �߱�
        nRet = _AhnHS_StartMonitor(HsExtError, szFullFileDLLPath); 
        if (nRet != ERROR_SUCCESS) 
        { 
        #ifdef	_KOREA
            wsprintf(szMessage, "��ŷ���� ����͸� ��ɿ� ������ �߻��Ͽ����ϴ�.(%x)", nRet);
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
    // �븸 ȣȯ����� �̽��� ���� ���ǵ��� �ΰ��� ����
    dwSensingRatio = AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST;

    if (PROGRAMPARAM->nation_type() != kLanguageTaiwan)       //!< 2:�߱�, 3:�븸
    {
        // �߱� - �ٽ��� ������Ʈ üũ ���
        dwOption |= AHNHS_CHKOPT_UPDATED_FILE_CHECK;
    }
#endif

#ifdef _HACKSHIELD_CHECK_OPTION_LMP
    // LMP �ɼ� �߰�
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
		case HS_ERR_UNKNOWN:                          // �� �� ���� ������ �߻��߽��ϴ�.
		case HS_ERR_INVALID_PARAM:                    // �ùٸ��� ���� �����Դϴ�.
		case HS_ERR_NOT_INITIALIZED:                  // �ٽ��� ����� �ʱ�ȭ���� ���� �����Դϴ�.
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:       // ���� ���μ����� ȣȯ�� ���� ����Ǿ����ϴ�.
		case HS_ERR_ALREADY_UNINITIALIZED:			  // Uninitialize�� ����� ���Ŀ��� ������ ��������� Initialize�ؾ� �մϴ�.
		case HS_ERR_INVALID_LICENSE:                  // �ùٸ��� ���� ���̼��� Ű�Դϴ�.
		case HS_ERR_INVALID_FILES:                    // �߸��� ���� ��ġ�Ǿ����ϴ�. ���α׷��� �缳ġ�Ͻñ� �ٶ��ϴ�.
		case HS_ERR_INIT_DRV_FAILED:                  
		case HS_ERR_ANOTHER_SERVICE_RUNNING:          // �ٸ� �����̳� ���μ������� �ٽ��带 �̹� ����ϰ� �ֽ��ϴ�.
		case HS_ERR_ALREADY_INITIALIZED:              // �̹� �ٽ��� ����� �ʱ�ȭ�Ǿ� �ֽ��ϴ�.
		case HS_ERR_DEBUGGER_DETECT:                  // ��ǻ�Ϳ��� ����� ������ �����Ǿ����ϴ�. ������� ������ ������Ų �ڿ� �ٽ� ��������ֽñ�ٶ��ϴ�.
		case HS_ERR_EXECUTABLE_FILE_CRACKED:		  // ���� ������ �ڵ尡 ũ�� �Ǿ����ϴ�.
		case HS_ERR_NEED_ADMIN_RIGHTS:				  // ADMIN ������ �ʿ��մϴ�.
		case HS_ERR_START_ENGINE_FAILED:              // ��ŷ ���α׷� ���� ������ ������ �� �����ϴ�.
		case HS_ERR_ALREADY_SERVICE_RUNNING:          // �̹� �ٽ��� ���񽺰� ���� ���Դϴ�.
		case HS_ERR_DRV_FILE_CREATE_FAILED:           // �ٽ��� ����̹� ������ ������ �� �����ϴ�.
		case HS_ERR_REG_DRV_FILE_FAILED:              // �ٽ��� ����̹��� ����� �� �����ϴ�.
		case HS_ERR_START_DRV_FAILED:                 // �ٽ��� ����̹��� ������ �� �����ϴ�.
		case HS_ERR_SERVICE_NOT_RUNNING:              // �ٽ��� ���񽺰� ����ǰ� ���� ���� �����Դϴ�.
		case HS_ERR_SERVICE_STILL_RUNNING:            // �ٽ��� ���񽺰� ���� �������� �����Դϴ�.
		case HS_ERR_NEED_UPDATE:                      // �ٽ��� ����� ������Ʈ�� �ʿ��մϴ�.
		case HS_ERR_API_IS_HOOKED:					  // �ش� API�� ��ŷ�Ǿ� �ִ� �����Դϴ�.
		default:
#ifdef	_KOREA
			wsprintf(szMessage, "��ŷ���� ��ɿ� ������ �߻��Ͽ����ϴ�.(%x)", nRet);
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
		case HS_ERR_UNKNOWN:                          // �� �� ���� ������ �߻��߽��ϴ�.
		case HS_ERR_INVALID_PARAM:                    // �ùٸ��� ���� �����Դϴ�.
		case HS_ERR_NOT_INITIALIZED:                  // �ٽ��� ����� �ʱ�ȭ���� ���� �����Դϴ�.
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:       // ���� ���μ����� ȣȯ�� ���� ����Ǿ����ϴ�.
		case HS_ERR_ALREADY_UNINITIALIZED:			  // Uninitialize�� ����� ���Ŀ��� ������ ��������� Initialize�ؾ� �մϴ�.
		case HS_ERR_INVALID_LICENSE:                  // �ùٸ��� ���� ���̼��� Ű�Դϴ�.
		case HS_ERR_INVALID_FILES:                    // �߸��� ���� ��ġ�Ǿ����ϴ�. ���α׷��� �缳ġ�Ͻñ� �ٶ��ϴ�.
		case HS_ERR_INIT_DRV_FAILED:                  
		case HS_ERR_ANOTHER_SERVICE_RUNNING:          // �ٸ� �����̳� ���μ������� �ٽ��带 �̹� ����ϰ� �ֽ��ϴ�.
		case HS_ERR_ALREADY_INITIALIZED:              // �̹� �ٽ��� ����� �ʱ�ȭ�Ǿ� �ֽ��ϴ�.
		case HS_ERR_DEBUGGER_DETECT:                  // ��ǻ�Ϳ��� ����� ������ �����Ǿ����ϴ�. ������� ������ ������Ų �ڿ� �ٽ� ��������ֽñ�ٶ��ϴ�.
		case HS_ERR_EXECUTABLE_FILE_CRACKED:		  // ���� ������ �ڵ尡 ũ�� �Ǿ����ϴ�.
		case HS_ERR_NEED_ADMIN_RIGHTS:				  // ADMIN ������ �ʿ��մϴ�.
		case HS_ERR_START_ENGINE_FAILED:              // ��ŷ ���α׷� ���� ������ ������ �� �����ϴ�.
		case HS_ERR_ALREADY_SERVICE_RUNNING:          // �̹� �ٽ��� ���񽺰� ���� ���Դϴ�.
		case HS_ERR_DRV_FILE_CREATE_FAILED:           // �ٽ��� ����̹� ������ ������ �� �����ϴ�.
		case HS_ERR_REG_DRV_FILE_FAILED:              // �ٽ��� ����̹��� ����� �� �����ϴ�.
		case HS_ERR_START_DRV_FAILED:                 // �ٽ��� ����̹��� ������ �� �����ϴ�.
		case HS_ERR_SERVICE_NOT_RUNNING:              // �ٽ��� ���񽺰� ����ǰ� ���� ���� �����Դϴ�.
		case HS_ERR_SERVICE_STILL_RUNNING:            // �ٽ��� ���񽺰� ���� �������� �����Դϴ�.
		case HS_ERR_NEED_UPDATE:                      // �ٽ��� ����� ������Ʈ�� �ʿ��մϴ�.
		case HS_ERR_API_IS_HOOKED:					  // �ش� API�� ��ŷ�Ǿ� �ִ� �����Դϴ�.
        case HS_ERR_LMP_START:                        // LMP ����� ������ �� �����ϴ�.
		default:
#ifdef	_KOREA
			wsprintf(szMessage, "��ŷ���� ��ɿ� ������ �߻��Ͽ����ϴ�.(%x)", nRet);
#else
			wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)", nRet);
#endif
			break;
		}

		MessageBox(NULL, szMessage, title_, MB_OK);
		return FALSE;
	}

#ifdef _CHECK_COMPATIBILITY_MODE
    // ȣȯ�� ����� ��� Ŭ���̾�Ʈ ����
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
	case AHNHS_ENGINE_DETECT_GAME_HACK:           			// ���� ��ŷ���� ������ �߰ߵǾ����ϴ�.
#ifdef	_KOREA
		wsprintf(szMessage, "������ ���α׷��� ������ �Բ� ����ɼ� �����ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "The game can not be executed with the following program.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		
		HSMessageBox(szMessage);
		break;

	case AHNHS_ACTAPC_DETECT_AUTOMACRO:						// �����ũ�� ������ �����Ǿ����ϴ�.	
#ifdef	_KOREA
		wsprintf(szMessage, "��ũ�� ������� �ǽɵǴ� ������ �����Ǿ����ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An operation whitch seems to be Auto-macro has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:						// ���丶�콺 ������ �����Ǿ����ϴ�.	
	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:					// �Ϻ� API�� �̹� ��ŷ�Ǿ� �ִ� �����Դϴ�. (�׷��� �����δ� �̸� �����ϰ� �ֱ� ������ ��ŷ���α׷��� �������� �ʽ��ϴ�.)
		break;

	case AHNHS_ACTAPC_DETECT_SPEEDHACK:						// ���ǵ��ٷ��� ���α׷��� ���� �ý��� �ð��� ����Ǿ����ϴ�.
#ifdef	_KOREA
		wsprintf(szMessage, "���ǵ������� �ǽɵǴ� ������ �����Ǿ����ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An operation whitch seems to be Speed-hack has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_KDTRACE:							// ����� Ʈ���̽��� �߻��ߴ�.(Ŀ�� ����� Ȱ��ȭ, ���� �극��ũ ������ ó��)
	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:					// ��ġ�� ����� Ʈ���̽��� ����Ǿ���.
#ifdef	_KOREA
		wsprintf(szMessage, "���ӿ� ���Ͽ� ����� �õ��� �����Ǿ����ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "A debugging attempt to the game has been detected.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;


	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:					// ��ŷ ���� ����̹��� �ε���� �ʾҽ��ϴ�.
	case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:						// �޽��� ��ŷ�� �õ��Ǿ����� �̸� �������� ���߽��ϴ�.
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:					// ��ȣ API�� ���� ��ŷ ������ �����Ǿ����ϴ�.
	case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:       			// �ٽ��� ���ø���� ����Ǿ����ϴ�.
#ifdef	_KOREA
		wsprintf(szMessage, "��ŷ ��� ��ɿ� �̻��� �߻��Ͽ����ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#else
		wsprintf(szMessage, "An error has been occurred on the anti-hacking functions.(%x)\n[%s]", lCode, (LPTSTR)pParam);
#endif
		HSMessageBox(szMessage);        
		break;

	case AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO:				// ���ǵ��� ���� �ɼ��� 'GAME'�� ��� �� �ݹ����� �ֱ� 5�ʵ����� �ð������� ���޵˴ϴ�.
	case AHNHS_ENGINE_DETECT_WINDOWED_HACK:					// â��� ��ŷ��
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:		// �ڵ� ��ġ �õ��� �����Ǿ����ϴ�.
		break;
	case AHNHS_ACTAPC_DETECT_ENGINEFAILED:	    			// ��������� ���������� �ʽ��ϴ�.
	case AHNHS_ACTAPC_DETECT_CODEMISMATCH:					// �ڵ尡 ��ġ���� �ʽ��ϴ�. 
    case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:           // ��ȣ ��⿡ ���ؼ� �޸� ������ �����Ǿ����ϴ�.
    case AHNHS_ACTAPC_DETECT_LMP_FAILED:                    // �ٽ��� ���� �޸� ��ȣ ����� ���������� �ʽ��ϴ�.
	case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
	case AHNHS_ACTAPC_DETECT_ANTIFREESERVER:
#ifdef	_KOREA
		wsprintf(szMessage, "��ŷ���� ���� �ٽ��� ���Ͽ� �̻��� ������ϴ�.(%x)\n[%s]", lCode, (LPTSTR)pParam); 
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