#include "SunClientPrecompiledHeader.h"
#if USING_GAMEGUARD
#include ".\gameguardapi.h"
#include "GameGuard/NPGameLib.h"
#include "GlobalFunc.h"
#include <windows.h>
#include "resource.h"
TCHAR GameGuardApi::error_message_[MAX_PATH] = {0, };
DWORD GameGuardApi::error_code_ = 0;
BOOL  GameGuardApi::is_timer_type_ = FALSE;

GameGuardApi::GameGuardApi(void) : 
npgame_lib_ptr_(NULL)
{
    ZeroMemory(error_message_, MAX_PATH);
}

GameGuardApi::~GameGuardApi(void)
{
    Release();    
}

BOOL GameGuardApi::Init( TCHAR* construct_str )
{
    npgame_lib_ptr_ = new CNPGameLib(construct_str);
    if (!npgame_lib_ptr_)
        return FALSE;

    DWORD result = npgame_lib_ptr_->Init();
    if (result != NPGAMEMON_SUCCESS)
    {
        // �� �����ڵ� ó������ ���� ����
        ErrorCodeResult(result, true);

        return FALSE;
    }    

    return TRUE;
}

BOOL GameGuardApi::Init( TCHAR* construct_str, TCHAR* module_path )
{
    TCHAR szFullFilePath[MAX_PATH];
    TCHAR szOldFullFilePath[MAX_PATH];    

    GetModuleFileName(NULL, szOldFullFilePath, MAX_PATH);    
    GetModuleFileName(NULL, szFullFilePath, MAX_PATH);    
    
    size_t len = _tcslen(szFullFilePath);
    for (size_t i = len; i > 0; --i)
    {
        if (szFullFilePath[i] == '\\')
            break;

        szFullFilePath[i] = '\0';
    }
    
    _tcscat(szFullFilePath, module_path);
    SetModulePath(szFullFilePath);
    TCHAR current_module_path[MAX_PATH] = {0, };
    GetModuleFileName(NULL, current_module_path, MAX_PATH);
    
    npgame_lib_ptr_ = new CNPGameLib(construct_str, szFullFilePath);
    SetModulePath(szOldFullFilePath);
    GetModuleFileName(NULL, current_module_path, MAX_PATH);
    
    if (!npgame_lib_ptr_)
        return FALSE;

    DWORD result = npgame_lib_ptr_->Init();
    if (result != NPGAMEMON_SUCCESS)
    {
        // �� �����ڵ� ó������ ���� ����
        ErrorCodeResult(result);

        return FALSE;
    }    

    return TRUE;
}

void GameGuardApi::SetClientHandle( HWND client_handle )
{
    if (!npgame_lib_ptr_)
        return;

    npgame_lib_ptr_->SetHwnd(client_handle);

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
    TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
    Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SetClientHandle() ����\n"));
    GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
}

void GameGuardApi::ErrorCodeResult( DWORD dwResult, bool timer_type /* = false */ )
{
    TCHAR msg[256] = {0, };
    LPCSTR lpszMsg;

    // ���߿� ����� ����.
    switch (dwResult)
    {
    case NPGAMEMON_ERROR_EXIST:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��尡 ���� �� �Դϴ�. ��� �ĳ� ����� �Ŀ� �ٽ� �����غ��ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Game Guard is running now. Please wait or Re-boot your computer.");
#endif // _KOREA
        }
        break;
    case NPGAMEMON_ERROR_GAME_EXIST:
        {
#if defined(_KOREA)
            lpszMsg = _T("������ �ߺ� ����Ǿ��ų� ���Ӱ��尡 �̹� ���� �� �Դϴ�. ���� ���� �� �ٽ� �����غ��ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Either Game is already running or Game Guard is running. Please close the game and start again.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
    case NPGAMEMON_ERROR_NFOUND_GG:
    case NPGAMEMON_ERROR_AUTH_INI:
    case NPGAMEMON_ERROR_NFOUND_INI:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�� ���ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Game Guard file is missing or modulated. Please install Game Guard setup file.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_CRYPTOAPI:
        {
#if defined(_KOREA)
            lpszMsg = _T("�������� �Ϻ� �ý��� ������ �ջ�Ǿ����ϴ�. ���ͳ� �ͽ��÷η�(IE)�� �ٽ� ��ġ�غ��ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Part of system window file is corrupted. Please re install Internet Explore.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_EXECUTE:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ���࿡ �����߽��ϴ�. ���Ӱ��� ���� ���� �� ������ �� ������ ���ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Fail to run Game Guard. Please delete Game Guard folder and re start.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_ILLEGAL_PRG:
        {
#if defined(_KOREA)
            lpszMsg = _T("�ҹ� ���α׷��� �߰ߵǾ����ϴ�. ���ʿ��� ���α׷��� ������ �� �ٽ� �����غ��ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("illegal program has been detected. Please shut down unnecessary program and start again");
#endif
        }
        break;
    case NPGMUP_ERROR_ABORT:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ������Ʈ�� ����ϼ̽��ϴ�. ������ ��� ���� ���� ��� ���ͳ� �� ���� ��ȭ�� ������ ������ ���ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Game Guard update has been cancelled. Please check your internet connection or personal firewall.");
#endif
        }
        break;
    case NPGMUP_ERROR_CONNECT:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� �ʱ�ȭ ���� �Ǵ� �������� ���Ӱ��� �����Դϴ�. ���Ӱ��� �¾������� �ٽ� ��ġ�ϰ� ������ �����غ��ϴ�.");
#else
            lpszMsg = _T("Either Old Game Guard file or Game Guard reset error found. Please reinstall game guard and run again.");
#endif
        }
        break;
    case NPGMUP_ERROR_PARAM:
        {
#if defined(_KOREA)
            lpszMsg = _T("ini ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�ϸ� �ذ� �� �� �ֽ��ϴ�.");
#else
            lpszMsg = _T("There is no ini file or corrupted ini file. Reinstalling Game Guard could solve this problem.");
#endif
        }
        break;
    case NPGMUP_ERROR_INIT:
        {
#if defined(_KOREA)
            lpszMsg = _T("npgmup.des �ʱ�ȭ �����Դϴ�. ���Ӱ��������� ���� �� �ٽ� ���ӽ����� �غ��ϴ�.");
#else
            lpszMsg = _T("npgmup.des reset error. Please delete Game Guard folder and re start the game");
#endif
        }
        break;
    case NPGMUP_ERROR_DOWNCFG:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� ��õ� �غ��ų�, ���� ��ȭ���� �ִٸ� ������ ������ ���ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Unable to connect Game Guard update server. Please restart after while or change personal firewall setting.");
#endif
        }
        break;
    case NPGMUP_ERROR_AUTH:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ������Ʈ�� �Ϸ����� ���߽��ϴ�. ���̷��� ����� �Ͻ� ���� ��Ų �� ��õ� �غ��ðų�, PC ���� ���α׷��� ����Ͻø� ������ ������ ���ñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Unable to finish Game Guard update. Please pause Anti-virus program and restart. PC management program can adjust its setting as well.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_NPSCAN:
        {
#if defined(_KOREA)
            lpszMsg = _T("���̷��� �� ��ŷ�� �˻� ��� �ε��� ���� �߽��ϴ�. �޸� �����̰ų� ���̷����� ���� ������ �� �ֽ��ϴ�.");
#else
            lpszMsg = _T("Virus or Hacking tool checking module loading has been failed. There are possible shortage on memory or corrupted with virus");
#endif
        }
        break;
    case NPGG_ERROR_COLLISION:
        {
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ���� �浹 ���α׷��� �߰ߵǾ����ϴ�.");
#else
            lpszMsg = _T("Found a program that is clashing Game Guard.");
#endif
        }
        break;
    default:
        {
            // ������ ���� �޽��� ���
#if defined(_KOREA)
            lpszMsg = _T("���Ӱ��� ���� �� ������ �߻��Ͽ����ϴ�. ���� ���� ���� GameGuard������ �ִ� &.erl ���ϵ��� Game1@inca.co.kr�� ÷���Ͽ� ���� �����ֽñ� �ٶ��ϴ�.");
#else
            lpszMsg = _T("Error occurs while Game Guard is running. Please attach &.erl files from Game Guard folder and send those to Game1@inca.co.k through email.");
#endif
        }
        break;
    }
#if defined(_KOREA)
    Sprintf(msg, _T("���Ӱ��� ���� : %lu"), dwResult);
#else
    Sprintf(msg, _T("GameGuard Error : %lu"), dwResult);
#endif
    if (true == timer_type)
    {
        _tcscpy(error_message_, lpszMsg);
        error_code_ = dwResult;
        is_timer_type_ = TRUE;
        GameGuardMessageBox(lpszMsg);
    }
    else
    {        
        error_code_ = dwResult;
        GameGuardMessageBox(lpszMsg);
        is_timer_type_ = FALSE;
    }
}

DWORD GameGuardApi::Check()
{
    if (npgame_lib_ptr_)
    {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
        GlobalFunc::Write_GameGuard_Log(_T("GameGuardApi::Check() Call\n"));
#endif // _NA_000000_20140519_GAMEGUARD_LOG
        return npgame_lib_ptr_->Check();
    }

    return 0;
}

void GameGuardApi::Release()
{
    SAFE_DELETE(npgame_lib_ptr_);
}

BOOL GameGuardApi::SendUserId( TCHAR* user_id_string )
{
    BOOL result = FALSE;
    if (npgame_lib_ptr_)
    {
        TCHAR user_id[ID_MAX_LEN] = {0, };
        int len = lstrlen(user_id_string);
        _tcsncpy(user_id, user_id_string, ID_MAX_LEN - 1);
        if (len >= 0 && len < ID_MAX_LEN)
        {
            user_id[len + 1] = _T('\0');
        }
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
        TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
        Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SendUserId() Call : %s\n"), user_id);
        GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

        // 0 - ����, 1 - ����
        result = npgame_lib_ptr_->Send(user_id);
        if (result == 0)
        {
            ErrorCodeResult(0, true);
        }
        return result;
    }

    return result;
}

LPCSTR GameGuardApi::GetInfo()
{
    if (npgame_lib_ptr_)
    {
        return npgame_lib_ptr_->GetInfo();
    }

    return NULL;
}

LRESULT CALLBACK GameGuardApi::MessageBoxProc( HWND dlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR title[256] = {0, };
            if (error_code_ != 0)
            {
                _stprintf(title, _T("nProtect Gameuard : %u"), error_code_);
            }
            else
            {
                _stprintf(title, _T("nProtect GameGuard"));
            }

            SetWindowText(dlg, title);
            SetDlgItemText(dlg, IDC_MESSAGE_TEXT, error_message_);
            SetDlgItemText(dlg, IDOK, "OK");

            if (is_timer_type_ == TRUE)
            {
                SetTimer(dlg, 1, 8000, (TIMERPROC) NULL);
            }            
        }
        return TRUE;

    case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(dlg, LOWORD(wParam));
                GlobalFunc::LogOutGame();
                return TRUE;
            }
        }
        break;

    case WM_TIMER:
        {
            switch (wParam)
            {
            case 1:
                {
                    EndDialog(dlg, 0);
                    GlobalFunc::LogOutGame();
                }
                break;
            }
        }
        break;
    }
    return FALSE;
}

void GameGuardApi::GameGuardMessageBox( LPCTSTR message )
{
    is_timer_type_ = TRUE;
    _tcscpy(error_message_, message);
    DialogBox(g_hInstance, (LPCTSTR)IDD_MESSAGE_BOX, NULL, (DLGPROC)MessageBoxProc);
    ZeroMemory(error_message_, MAX_PATH);
}

void GameGuardApi::LastMessagePrint()
{
    if (_tcscmp(error_message_, _T("")) != 0)
    {
        GameGuardMessageBox(error_message_);

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
        TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
        Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::LastMessagePrint() Call\n"));
        GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
    }
}

void GameGuardApi::SendToGameGuardServer( PBYTE pbPacket, DWORD dwPacketSize, DWORD dwServerNumber )
{
    if (npgame_lib_ptr_)
    {
        npgame_lib_ptr_->Auth3(pbPacket, dwPacketSize, dwServerNumber);

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
        TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
        Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SendToGameGuardServer() Call\n"));
        GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
    }
}

bool GameGuardApi::Recv_CG_GAMEGUARD_SERVER_QUERY_CMD_Packet( MSG_BASE* msg )
{
    if ((msg->m_byCategory == CG_ARMORKIT) && (msg->m_byProtocol == CG_GAMEGUARD_SERVER_QUERY_CMD))
    {
        MSG_CG_GAMEGUARD_SERVER_QUERY_CMD* cmd = static_cast<MSG_CG_GAMEGUARD_SERVER_QUERY_CMD*>(msg);

        if (npgame_lib_ptr_ && cmd)
        {
            // ���Ӱ��忡 �뺸
            npgame_lib_ptr_->Auth3(cmd->buffer, cmd->query_size, 0);

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::Recv_CG_GAMEGUARD_SERVER_QUERY_CMD_Packet() Call\n"));
            GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

            return true;
        }
    }

    return false;
}

bool GameGuardApi::Recv_CG_GAMEGUARD_SERVER_QUERY_NAK_Packet( MSG_BASE* msg )
{
    if ((msg->m_byCategory == CG_ARMORKIT) && (msg->m_byProtocol == CG_GAMEGUARD_SERVER_QUERY_NAK))
    {
        MSG_CG_GAMEGUARD_SERVER_QUERY_NAK* packet = static_cast<MSG_CG_GAMEGUARD_SERVER_QUERY_NAK*>(msg);

        if (npgame_lib_ptr_ && packet)
        {
            // Ŭ���̾�Ʈ ����
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::Recv_CG_GAMEGUARD_SERVER_QUERY_NAK_Packet() Call\n"));
            GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            ErrorCodeResult(packet->result_code, true);            

            return true;
        }
    }

    return false;
}
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
    TCHAR szHackMsg[256] = {0, };
    // �޽��� ó�� Callback �Լ�
    // ���� ����ÿ��� false�� return ���ְ�, �������� �ʴ� ���� true�� return �մϴ�.

    GameGuardApi::Instance()->error_code_ = dwMsg;

    switch (dwMsg)
    {
    case NPGAMEMON_COMM_ERROR:
    case NPGAMEMON_COMM_CLOSE:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback() NPGAMEON_COMM_ERROR || NPGAMEON_COMM_CLOSE Call\n"));
            GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

            _stprintf(GameGuardApi::error_message_, TEXT("GameGuard Error : %lu"), dwArg);

            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;        
    case NPGAMEMON_INIT_ERROR:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback::NPGAMEON_INIT_ERROR Call\n"));
            GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#if defined(_KOREA)
            _stprintf(GameGuardApi::error_message_, TEXT("���Ӱ��� �ʱ�ȭ ���� : %lu"), dwArg);
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Guard initializing error"), dwArg);
#endif
            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;
    case NPGAMEMON_SPEEDHACK:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback::NPGAMEON_SPEEDHACK Call\n"));
            GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#if defined(_KOREA)
            _stprintf(GameGuardApi::error_message_, TEXT("���ǵ����� �����Ǿ����ϴ�."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Speed hack has been detected."));
#endif
            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;
    case NPGAMEMON_GAMEHACK_KILLED:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback::NPGAMEON_GAMEHACK_KILLED Call\n"));
            GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#if defined(_KOREA)
            _stprintf(GameGuardApi::error_message_, TEXT("�������� �߰ߵǾ����ϴ�."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Hack has been detected."));
#endif
            CONST CHAR* temp_info = GameGuardApi::Instance()->GetInfo();
            if (temp_info)
            {
                // ���� ���� �ڵ� �ʿ�
            }

            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;
    case NPGAMEMON_GAMEHACK_DETECT:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback::NPGAMEON_GAMEHACK_DETECT Call\n"));
            GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#if defined(_KOREA)
            _stprintf(GameGuardApi::error_message_, TEXT("�������� �߰ߵǾ����ϴ�."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Hack has been detected."));
#endif
            CONST CHAR* temp_info = GameGuardApi::Instance()->GetInfo();
            if (temp_info)
            {
                // ���� ���� �ڵ� �ʿ�
            }
            
            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;
    case NPGAMEMON_GAMEHACK_DOUBT:
        {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("NPGameMonCallback::NPGAMEON_DOUBT Call\n"));
            GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#if defined(_KOREA)
            _stprintf(GameGuardApi::error_message_, TEXT("�����̳� ���Ӱ��尡 �����Ǿ����ϴ�."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game or Game Guard is corrupted."));
#endif
            // ���� �ڵ�
            GlobalFunc::LogOutGame(true);
            return FALSE;
        }
        break;
    case NPGAMEMON_CHECK_CSAUTH3:
        {
            PCSAuth3Data pCSAuth3 = (PCSAuth3Data)dwArg;

            MSG_CG_GAMEGUARD_SERVER_QUERY_ACK msg;
            memcpy(&(msg.buffer), pCSAuth3->bPacket, pCSAuth3->dwPacketSize);
            msg.query_size = pCSAuth3->dwPacketSize;

            // ����� ���Ӽ����� �����ϴ� ��
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("NPGAMEON_CHECK_CSAUTH3 Call\n"));
            GlobalFunc::Write_GameGuard_Log(result_log);
            
            ZeroMemory(result_log, INTERFACE_STRING_LENGTH);
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("PCSAuth3Data PacketSize : %d\n"), pCSAuth3->dwPacketSize);
            GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            // ������ ����
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));
            return TRUE;
        }
        break;
    case NPGAMEMON_GAMEHACK_REPORT:
        {
            DWORD dwHackInfoSize = 0;
            LPBYTE pHackInfo = NULL;
            pHackInfo = GetHackInfoFromGameMon(&dwHackInfoSize);            
            if (pHackInfo && dwHackInfoSize > 0)
            {
                // �Ʒ� �Լ��� ���Ӱ��忡�� �����ϴ� �Լ��� �ƴմϴ�.                
                MSG_CG_GAMEGUARD_HACKDATA_CMD msg;
                memcpy(&(msg.buffer), pHackInfo, dwHackInfoSize);
                // ������ ����
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
                TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
                Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("NPGAMEON_GAMEHACK_REPORT Call\n"));
                GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            }
        }

        return TRUE;
    }
    
    return TRUE; // ��� ����
}
#endif // USING_GAMEGUARD