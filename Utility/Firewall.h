//--------------------------------------------------------------------------------------
// File: Firewall.cpp
//
// Demonstrate the use of the XP/SP2 firewall API
// It requires the latest Microsoft Platform SDK to compile because it uses the Firewall
// API introduced with Windows XP SP2.
//
// Copyright (c) 2003-2004 Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------

#ifndef FIREWALL_H
#define FIREWALL_H



#ifdef _USE_FIREWALL_SKIP

#include <windows.h>
#include <strsafe.h>
#include <crtdbg.h>
#include <netfw.h>
#include <stdio.h>
#include <conio.h>

BOOL OnInstallApplication(IN const wchar_t* szFwProcessImageFileName, IN const wchar_t* szFwFriendlyName);
BOOL OnUninstallApplication(IN const wchar_t* szFwProcessImageFileName);
BOOL CanHostMultiplayer(IN const wchar_t* szFwProcessImageFileName);


class FirewallWrapper 
{
    INetFwProfile*  m_pFwProfile;
    HRESULT         m_hrComInit;

    HRESULT InitFirewallProfile();
    FirewallWrapper();

public:

    static FirewallWrapper* Create();
    ~FirewallWrapper();

    BOOL    FirewallPresent() 
        { return m_pFwProfile != NULL; } 

    BOOL    IsFirewallActive();
    HRESULT AddAuthorizedApp(
        IN const wchar_t* szFwProcessImageFileName,
        IN const wchar_t* szFwFriendlyName
        );
    HRESULT RemoveAuthorizedApp(
        IN const wchar_t* szFwProcessImageFileName
        );
    BOOL IsAppEnabled( 
        IN const wchar_t* szFwProcessImageFileName
        );
    BOOL AreExceptionsAllowed();
};

#endif 
//_USE_FIREWALL_SKIP

#endif 
//FIREWALL_H;