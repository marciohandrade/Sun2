#pragma once

#if USING_GAMEGUARD

#include "../../../include/GameGuard/NPGameLib.h"

#pragma comment( lib, "../../lib/"VC_DIR_REDIRECT"/GameGuard/NPGameLib_101_DE_MT.lib")

class GameGuardApi : public Singleton<GameGuardApi>
{
public:
    BOOL Init(TCHAR* construct_str);
    BOOL Init(TCHAR* construct_str, TCHAR* module_path);
    void Release();    
    void SetClientHandle(HWND client_handle);
    bool Recv_CG_GAMEGUARD_SERVER_QUERY_CMD_Packet(MSG_BASE* msg);
    bool Recv_CG_GAMEGUARD_SERVER_QUERY_NAK_Packet(MSG_BASE* msg);
public:
    DWORD Check();
    BOOL SendUserId(TCHAR* user_id_string);
    void SendToGameGuardServer(PBYTE pbPacket, DWORD dwPacketSize, DWORD dwServerNumber);
    LPCSTR GetInfo();
    void LastMessagePrint();    
public:
    void ErrorCodeResult(DWORD dwResult, bool timer_type = false);

public:    
    static void GameGuardMessageBox( LPCTSTR message );
    static LRESULT CALLBACK MessageBoxProc(HWND dlg, UINT message, WPARAM wParam, LPARAM lParam);
public:
    GameGuardApi(void);
    ~GameGuardApi(void);
private:
    CNPGameLib* npgame_lib_ptr_;    
public:
    static TCHAR error_message_[MAX_PATH];
    static DWORD error_code_;
    static BOOL  is_timer_type_;
};
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg);
#endif // USING_GAMEGUARD