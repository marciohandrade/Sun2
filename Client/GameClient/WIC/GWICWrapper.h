#pragma once

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include <string>
#include <map>
#include <list>

#include "GDefine.h"

using namespace std;

class GACTMgr;
class GACTFactory;
class IACT;


class GWICWrapper
{
public:
    GWICWrapper();
    ~GWICWrapper(void);


private:
    void    Clear();
    void    ClearRCBF();


private:
    typedef std::map<string, HANDLE> TRCBF;
    typedef TRCBF::iterator          TRCBF_POS;


public:
    BOOL    Initialize();                                // 
    BOOL    TransferMyHWND();                            // Transfer My HWND to WIC Windows In GameChuLauncher.exe

    BOOL    IsInitialize()  { return m_bInitialized; }
  
    LRESULT OnCopyData( WPARAM wParam, LPARAM lParam );  // WM_COPYDATA's Message Handler.
    void    CheckCBEvent();

public:
    BOOL    InvokeChargeCash();                     // Request Charge Cash
    BOOL    InvokeCash();                           // Request Cash's Value 
    BOOL    InvokeBuyItem( const string& sValue );  // Request Buy Item
    BOOL    InvokeGift( const string& sValue );     // Request Give Item to Other.

    BOOL    InvokeClan( const string& sValue );     // Request Join/Leave/Force Leave/
   
    BOOL    InvokeSendMsg( const string& sValue );  // Request Others Type' Msg


public:
    BOOL    Invoke( const string& sID, const string& sCBID, const string& sValue );

    LRESULT SendMsg( IACT* pACT );
    LRESULT SendMsg( const string& sID, HWND hWnd );


private:
    BOOL    CheckFunction( const string& sID );                       // Check Whether or not Kernel Object Exists.
    BOOL    IsProcessing();                                           // Check Whether or not Processing.
    HWND    FindWIC();                                                // Find WIC Windows's HWND In GameChuLaunche.exe     


public:
    HANDLE  CreateCBEvent( const string& sCBID );                     // Create CallBack's Event
    void    DestroyCBEvent( const string& sCBID );                    // Destory CallBack's Event


private:
    string  FindRegisteredCBID( HANDLE hCBEvent );                    // Find and Return CBID
    HANDLE  FindRegisteredCBEvent( const string& sCBID );             // Find and Return CBID

    HANDLE  RegisterCBEvent( const string& sCBID,  HANDLE hEvent );   // Register Kernel Object.
    BOOL    UnRegisterCBEvent( const string& sCBD );                  // UnRegister Kernel Object.

    void    TerminateInvoke(HANDLE hEvent);                           // Terminate Invoke.


public:
    HWND    GetHWIC();                                // Return WIC Windows's HWND
    HWND    GetHGame();                               // Return WIC Windows's HWND

    void    SetHGame(HWND hGame);                     // Set Game Windows' HWND

private:
    HWND                     m_hWIC;                  // In GameChuLaunche.exe
    HWND                     m_hGame;                 // GameClient's HWND.

    std::map<string, HANDLE> m_mapRegisteredCBEvent;  // CallBack's ID and HANDLE.

    GACTMgr*                 m_pACTMgr;
    GACTFactory*             m_pACTFactory;

    BOOL                     m_bInitialized;
};

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
