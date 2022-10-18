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
        // 각 에러코드 처리법은 예제 참조
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
        // 각 에러코드 처리법은 예제 참조
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
    Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SetClientHandle() 성공\n"));
    GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
}

void GameGuardApi::ErrorCodeResult( DWORD dwResult, bool timer_type /* = false */ )
{
    TCHAR msg[256] = {0, };
    LPCSTR lpszMsg;

    // 나중에 영어로 하자.
    switch (dwResult)
    {
    case NPGAMEMON_ERROR_EXIST:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드가 실행 중 입니다. 잠시 후나 재부팅 후에 다시 실행해보시기 바랍니다.");
#else
            lpszMsg = _T("Game Guard is running now. Please wait or Re-boot your computer.");
#endif // _KOREA
        }
        break;
    case NPGAMEMON_ERROR_GAME_EXIST:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해보시기 바랍니다.");
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
            lpszMsg = _T("게임가드 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치해 보시기 바랍니다.");
#else
            lpszMsg = _T("Game Guard file is missing or modulated. Please install Game Guard setup file.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_CRYPTOAPI:
        {
#if defined(_KOREA)
            lpszMsg = _T("윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해보시기 바랍니다.");
#else
            lpszMsg = _T("Part of system window file is corrupted. Please re install Internet Explore.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_EXECUTE:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드 실행에 실패했습니다. 게임가드 폴더 삭제 후 게임을 재 실행해 보시기 바랍니다.");
#else
            lpszMsg = _T("Fail to run Game Guard. Please delete Game Guard folder and re start.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_ILLEGAL_PRG:
        {
#if defined(_KOREA)
            lpszMsg = _T("불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후 다시 실행해보시기 바랍니다.");
#else
            lpszMsg = _T("illegal program has been detected. Please shut down unnecessary program and start again");
#endif
        }
        break;
    case NPGMUP_ERROR_ABORT:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드 업데이트를 취소하셨습니다. 접속이 계속 되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.");
#else
            lpszMsg = _T("Game Guard update has been cancelled. Please check your internet connection or personal firewall.");
#endif
        }
        break;
    case NPGMUP_ERROR_CONNECT:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업파일을 다시 설치하고 게임을 실행해봅니다.");
#else
            lpszMsg = _T("Either Old Game Guard file or Game Guard reset error found. Please reinstall game guard and run again.");
#endif
        }
        break;
    case NPGMUP_ERROR_PARAM:
        {
#if defined(_KOREA)
            lpszMsg = _T("ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하면 해결 할 수 있습니다.");
#else
            lpszMsg = _T("There is no ini file or corrupted ini file. Reinstalling Game Guard could solve this problem.");
#endif
        }
        break;
    case NPGMUP_ERROR_INIT:
        {
#if defined(_KOREA)
            lpszMsg = _T("npgmup.des 초기화 에러입니다. 게임가드폴더를 삭제 후 다시 게임실행을 해봅니다.");
#else
            lpszMsg = _T("npgmup.des reset error. Please delete Game Guard folder and re start the game");
#endif
        }
        break;
    case NPGMUP_ERROR_DOWNCFG:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 재시도 해보거나, 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.");
#else
            lpszMsg = _T("Unable to connect Game Guard update server. Please restart after while or change personal firewall setting.");
#endif
        }
        break;
    case NPGMUP_ERROR_AUTH:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드 업데이트를 완료하지 못했습니다. 바이러스 백신을 일시 중지 시킨 후 재시도 해보시거나, PC 관리 프로그램을 사용하시면 설정을 조정해 보시기 바랍니다.");
#else
            lpszMsg = _T("Unable to finish Game Guard update. Please pause Anti-virus program and restart. PC management program can adjust its setting as well.");
#endif
        }
        break;
    case NPGAMEMON_ERROR_NPSCAN:
        {
#if defined(_KOREA)
            lpszMsg = _T("바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.");
#else
            lpszMsg = _T("Virus or Hacking tool checking module loading has been failed. There are possible shortage on memory or corrupted with virus");
#endif
        }
        break;
    case NPGG_ERROR_COLLISION:
        {
#if defined(_KOREA)
            lpszMsg = _T("게임가드와 충돌 프로그램이 발견되었습니다.");
#else
            lpszMsg = _T("Found a program that is clashing Game Guard.");
#endif
        }
        break;
    default:
        {
            // 적절한 종료 메시지 출력
#if defined(_KOREA)
            lpszMsg = _T("게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard폴더에 있는 &.erl 파일들을 Game1@inca.co.kr로 첨부하여 메일 보내주시기 바랍니다.");
#else
            lpszMsg = _T("Error occurs while Game Guard is running. Please attach &.erl files from Game Guard folder and send those to Game1@inca.co.k through email.");
#endif
        }
        break;
    }
#if defined(_KOREA)
    Sprintf(msg, _T("게임가드 에러 : %lu"), dwResult);
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

        // 0 - 실패, 1 - 성공
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
            // 게임가드에 통보
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
            // 클라이언트 종료
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
    // 메시지 처리 Callback 함수
    // 게임 종료시에는 false를 return 해주고, 종료하지 않는 경우는 true를 return 합니다.

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

            // 종료 코드
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
            _stprintf(GameGuardApi::error_message_, TEXT("게임가드 초기화 에러 : %lu"), dwArg);
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Guard initializing error"), dwArg);
#endif
            // 종료 코드
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
            _stprintf(GameGuardApi::error_message_, TEXT("스피드핵이 감지되었습니다."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Speed hack has been detected."));
#endif
            // 종료 코드
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
            _stprintf(GameGuardApi::error_message_, TEXT("게임핵이 발견되었습니다."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Hack has been detected."));
#endif
            CONST CHAR* temp_info = GameGuardApi::Instance()->GetInfo();
            if (temp_info)
            {
                // 서버 전송 코드 필요
            }

            // 종료 코드
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
            _stprintf(GameGuardApi::error_message_, TEXT("게임핵이 발견되었습니다."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game Hack has been detected."));
#endif
            CONST CHAR* temp_info = GameGuardApi::Instance()->GetInfo();
            if (temp_info)
            {
                // 서버 전송 코드 필요
            }
            
            // 종료 코드
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
            _stprintf(GameGuardApi::error_message_, TEXT("게임이나 게임가드가 변조되었습니다."));
#else
            _stprintf(GameGuardApi::error_message_, TEXT("Game or Game Guard is corrupted."));
#endif
            // 종료 코드
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

            // 여기는 게임서버로 전송하는 곳
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
            TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("NPGAMEON_CHECK_CSAUTH3 Call\n"));
            GlobalFunc::Write_GameGuard_Log(result_log);
            
            ZeroMemory(result_log, INTERFACE_STRING_LENGTH);
            Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("PCSAuth3Data PacketSize : %d\n"), pCSAuth3->dwPacketSize);
            GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            // 서버로 전송
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
                // 아래 함수는 게임가드에서 제공하는 함수가 아닙니다.                
                MSG_CG_GAMEGUARD_HACKDATA_CMD msg;
                memcpy(&(msg.buffer), pHackInfo, dwHackInfoSize);
                // 서버로 전송
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
    
    return TRUE; // 계속 진행
}
#endif // USING_GAMEGUARD