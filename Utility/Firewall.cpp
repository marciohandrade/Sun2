#include "UtilityCommon.h"
#include "Firewall.h"
#ifdef _USE_FIREWALL_SKIP
#include <windows.h>
#include <strsafe.h>
#include <crtdbg.h>
#include <netfw.h>
#include <stdio.h>
#include <conio.h>


//--------------------------------------------------------------------------------------
// FirewallWrapper()
//      Initialize Com and fetch a firewall profile
//--------------------------------------------------------------------------------------
FirewallWrapper::FirewallWrapper()
    : m_pFwProfile(NULL)
{}




//--------------------------------------------------------------------------------------
// ~FirewallWrapper()
//      Release the firewall profile and uninitialize COM
//--------------------------------------------------------------------------------------
FirewallWrapper::~FirewallWrapper()
{
    // Release the firewall profile.
    if( m_pFwProfile != NULL )
    {
        m_pFwProfile->Release();
    }
}




//--------------------------------------------------------------------------------------
// FirewallWrapper::Create()
//--------------------------------------------------------------------------------------
FirewallWrapper* FirewallWrapper::Create()
{
    FirewallWrapper* pfw = new FirewallWrapper;
    if( pfw )
    {
        HRESULT hr = pfw->InitFirewallProfile();
        if( !pfw->FirewallPresent() )
        {
            hr;
            // printf( "Failed to initialize firewall profile: 0x%08lx\n", hr );
            delete pfw;
            pfw = NULL;
        }
    }
    return pfw;
}




//--------------------------------------------------------------------------------------
// InitFirewallProfile()
//      Access the firewall and policy to retrieve the firewall profile.  
//          Initialize COM  
//          Retrieve the firewall profile
//--------------------------------------------------------------------------------------
HRESULT FirewallWrapper::InitFirewallProfile()
{
    HRESULT         hr = S_OK;
    INetFwMgr*      pFwMgr = NULL;
    INetFwPolicy*   pFwPolicy = NULL;

    m_pFwProfile = NULL;

    // Create an instance of the firewall settings manager.
    hr = CoCreateInstance(
            __uuidof(NetFwMgr),
            NULL,
            CLSCTX_INPROC_SERVER,
            __uuidof(INetFwMgr),
            (void**)&pFwMgr
            );
    _ASSERT( hr != CO_E_NOTINITIALIZED && "COM has not been initialized" );
    if( FAILED(hr) )
    {
        // printf( "CoCreateInstance failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Retrieve the local firewall policy.
    hr = pFwMgr->get_LocalPolicy( &pFwPolicy );
    if( FAILED(hr) )
    {
        // printf( "get_LocalPolicy failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Retrieve the firewall profile currently in effect.
    hr = pFwPolicy->get_CurrentProfile( &m_pFwProfile );
    if( FAILED(hr) )
    {
        // printf( "get_CurrentProfile failed: 0x%08lx\n", hr );
        goto cleanup;
    }

cleanup:

    // Release the local firewall policy.
    if( pFwPolicy != NULL )
    {
        pFwPolicy->Release();
    }

    // Release the firewall settings manager.
    if( pFwMgr != NULL )
    {
        pFwMgr->Release();
    }

    return hr;
}




//--------------------------------------------------------------------------------------
// IsFirewallActive()
//      Test if the firewall is on, return as a boolean  
//--------------------------------------------------------------------------------------
BOOL FirewallWrapper::IsFirewallActive()
{
    HRESULT         hr = S_OK;
    VARIANT_BOOL    vbFwEnabled;

    // Get the current state of the firewall.
    // This requires the latest Microsoft Platform SDK to compile because 
    // it uses the Firewall API introduced with Windows XP SP2.
    hr = m_pFwProfile->get_FirewallEnabled( &vbFwEnabled );  // upgrade Platform SDK if this fails to compile
    if( FAILED(hr) || vbFwEnabled == VARIANT_FALSE )
    {
        return FALSE;
    }

    return TRUE;
}




//--------------------------------------------------------------------------------------
// FirewallWrapper::IsAppEnabled()
//      Determine if the specified application is enabled. Return the 
//      result in pbFWAppEnabled. 
//--------------------------------------------------------------------------------------
BOOL FirewallWrapper::IsAppEnabled(
            IN const wchar_t* szFwProcessImageFileName
            )
{
    HRESULT             hr = S_OK;
    BSTR                fwBstrProcessImageFileName = NULL;
    VARIANT_BOOL        vbFwEnabled;
    INetFwAuthorizedApplication*    pFwApp = NULL;
    INetFwAuthorizedApplications*   pFwApps = NULL;
    BOOL bFwAppEnabled = FALSE;
    
    _ASSERT( szFwProcessImageFileName != NULL );

    // Retrieve the collection of authorized applications.
    hr = m_pFwProfile->get_AuthorizedApplications( &pFwApps );
    if( FAILED(hr) )
    {
        // printf( "get_AuthorizedApplications failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Allocate a BSTR for the process image file name.
    fwBstrProcessImageFileName = SysAllocString( szFwProcessImageFileName );
    if( SysStringLen( fwBstrProcessImageFileName ) == 0 )
    {
        hr = E_OUTOFMEMORY;
        // printf( "SysAllocString failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Attempt to retrieve the authorized application.
    hr = pFwApps->Item( fwBstrProcessImageFileName, &pFwApp );
    if( SUCCEEDED(hr ) )
    {
        // Find out if the authorized application is enabled.
        hr = pFwApp->get_Enabled( &vbFwEnabled );
        if( FAILED(hr) )
        {
            // printf( "get_Enabled failed: 0x%08lx\n", hr );
            goto cleanup;
        }

        if( vbFwEnabled != VARIANT_FALSE )
        {
            // The authorized application is enabled.
            bFwAppEnabled = TRUE;

            // printf(
            //     "Authorized application %lS is enabled in the firewall.\n",
            //     szFwProcessImageFileName
            //     );
        }
        else
        {
            // printf(
            //     "Authorized application %lS is disabled in the firewall.\n",
            //     szFwProcessImageFileName
            //     );
        }
    }
    else
    {
        // The authorized application was not in the collection.
        hr = S_OK;

        // printf(
        //     "Authorized application %lS is disabled in the firewall.\n",
        //     szFwProcessImageFileName
        //     );
    }

cleanup:

    // Free the BSTR.
    SysFreeString( fwBstrProcessImageFileName );

    // Release the authorized application instance.
    if( pFwApp != NULL )
    {
        pFwApp->Release();
    }

    // Release the authorized application collection.
    if( pFwApps != NULL )
    {
        pFwApps->Release();
    }

    return bFwAppEnabled;
}


//--------------------------------------------------------------------------------------
// ExceptionsAllowed()
//      Find out if the system is in no-exceptions mode   
//--------------------------------------------------------------------------------------
BOOL FirewallWrapper::AreExceptionsAllowed()
{
    VARIANT_BOOL        vbNotAllowed = VARIANT_FALSE;
    HRESULT             hr = S_OK;

    hr = m_pFwProfile->get_ExceptionsNotAllowed( &vbNotAllowed );
    if( SUCCEEDED(hr) && vbNotAllowed != VARIANT_FALSE )
    {
        return FALSE;
    }
    
    return TRUE; 
}


//--------------------------------------------------------------------------------------
// AddAuthorizedApp()
//      Add an application to the exception list (aka AuthorizedApplication list)  
//--------------------------------------------------------------------------------------
HRESULT FirewallWrapper::AddAuthorizedApp(
            IN const wchar_t* szFwProcessImageFileName,
            IN const wchar_t* szFwFriendlyName
            )
{
    HRESULT             hr = S_OK;
    BSTR                fwBstrName = NULL;
    BSTR                fwBstrProcessImageFileName = NULL;
    INetFwAuthorizedApplication*    pFwApp = NULL;
    INetFwAuthorizedApplications*   pFwApps = NULL;

    _ASSERT( szFwProcessImageFileName != NULL );
    _ASSERT( szFwFriendlyName != NULL );

    // Retrieve the authorized application collection.
    hr = m_pFwProfile->get_AuthorizedApplications( &pFwApps );
    if( FAILED(hr) )
    {
        // printf( "get_AuthorizedApplications failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Create an instance of an authorized application.
    hr = CoCreateInstance(
            __uuidof(NetFwAuthorizedApplication),
            NULL,
            CLSCTX_INPROC_SERVER,
            __uuidof(INetFwAuthorizedApplication),
            (void**)&pFwApp
            );
    if( FAILED(hr) )
    {
        // printf( "CoCreateInstance failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Allocate a BSTR for the process image file name.
    fwBstrProcessImageFileName = SysAllocString( szFwProcessImageFileName );
    if( SysStringLen( fwBstrProcessImageFileName ) == 0 )
    {
        hr = E_OUTOFMEMORY;
        // printf( "SysAllocString failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Set the process image file name.
    hr = pFwApp->put_ProcessImageFileName( fwBstrProcessImageFileName );
    if( FAILED(hr) )
    {
        // printf( "put_ProcessImageFileName failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Allocate a BSTR for the application friendly name.
    fwBstrName = SysAllocString( szFwFriendlyName );
    if( SysStringLen( fwBstrName ) == 0 )
    {
        hr = E_OUTOFMEMORY;
        // printf( "SysAllocString failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Set the application friendly name.
    hr = pFwApp->put_Name( fwBstrName );
    if( FAILED(hr) )
    {
        // printf( "put_Name failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Add the application to the collection.
    hr = pFwApps->Add( pFwApp );
    if( FAILED(hr) )
    {
        // printf( "Add failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // printf(
    //     "Authorized application %lS is now enabled in the firewall.\n",
    //     szFwProcessImageFileName
    //     );

cleanup:

    // Free the BSTRs.
    SysFreeString( fwBstrName );
    SysFreeString( fwBstrProcessImageFileName );

    // Release the authorized application instance.
    if( pFwApp != NULL )
    {
        pFwApp->Release();
    }

    // Release the authorized application collection.
    if( pFwApps != NULL )
    {
        pFwApps->Release();
    }

    return hr;
}


//--------------------------------------------------------------------------------------
// RemoveAuthorizedApp()
//      Remove an application from the exception list (aka AuthorizedApplication list)  
//--------------------------------------------------------------------------------------
HRESULT FirewallWrapper::RemoveAuthorizedApp(
            IN const wchar_t* szFwProcessImageFileName
            )
{
    HRESULT             hr = S_OK;
    BSTR                fwBstrProcessImageFileName = NULL;
    INetFwAuthorizedApplications*   pFwApps = NULL;

    _ASSERT( szFwProcessImageFileName != NULL );

    // Retrieve the authorized application collection.
    hr = m_pFwProfile->get_AuthorizedApplications( &pFwApps );
    if( FAILED(hr) )
    {
        // printf( "get_AuthorizedApplications failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // Allocate a BSTR for the process image file name.
    fwBstrProcessImageFileName = SysAllocString( szFwProcessImageFileName );
    if( SysStringLen( fwBstrProcessImageFileName ) == 0 )
    {
        hr = E_OUTOFMEMORY;
        // printf( "SysAllocString failed: 0x%08lx\n", hr );
        goto cleanup;
    }


    // Remove the application from the collection.
    hr = pFwApps->Remove( fwBstrProcessImageFileName );
    if( FAILED(hr) )
    {
        // printf( "Remove failed: 0x%08lx\n", hr );
        goto cleanup;
    }

    // printf(
    //     "Authorized application %lS is now removed from the firewall.\n",
    //     szFwProcessImageFileName
    //     );

cleanup:

    // Free the BSTRs.
    SysFreeString( fwBstrProcessImageFileName );

    // Release the authorized application collection.
    if( pFwApps != NULL )
    {
        pFwApps->Release();
    }

    return hr;
}


//--------------------------------------------------------------------------------------
// OnInstallApplication() 
// Add the application to the exception (aka authorized) list.
// Returns true if we've been added.
//--------------------------------------------------------------------------------------
BOOL OnInstallApplication(
            IN const wchar_t* szFwProcessImageFileName,
            IN const wchar_t* szFwFriendlyName
            )
{
    FirewallWrapper* pfw = FirewallWrapper::Create();
    if( !pfw )
        return FALSE; 

    HRESULT hr = pfw->AddAuthorizedApp(            
        szFwProcessImageFileName,
        szFwFriendlyName
        );
    return SUCCEEDED(hr);
}


//--------------------------------------------------------------------------------------
// OnUninstallApplication()
//      Remove the application from the exception list.
//--------------------------------------------------------------------------------------
BOOL OnUninstallApplication(
            IN const wchar_t* szFwProcessImageFileName
            )
{
    FirewallWrapper* pfw = FirewallWrapper::Create();
    if( !pfw )
        return FALSE; 
    
    HRESULT hr = pfw->RemoveAuthorizedApp( 
        szFwProcessImageFileName
        );
    return SUCCEEDED(hr);
}


//--------------------------------------------------------------------------------------
// CanHostMultiplayer() 
//      Check that the firewall is properly configured for the game. 
//      Returns FALSE if the firewall is configured to block us from hosting. 
//--------------------------------------------------------------------------------------
BOOL CanHostMultiplayer(
            IN const wchar_t* szFwProcessImageFileName
            )
{
    FirewallWrapper* pfw = FirewallWrapper::Create();
    if( !pfw )
        return TRUE; 
    
    if( !pfw->IsAppEnabled(szFwProcessImageFileName) )
    {
        OutputDebugString( "Application is not enabled in the firewall. You will not be able host games or join a peer-to-peer game. Depending on the game, you may be able to join.\n" );
        return FALSE;
    }

    if( !pfw->AreExceptionsAllowed() )
    {
        OutputDebugString( "Firewall is on with no exceptions. You will not be able host games or join a peer-to-peer game. Depending on the game, you may be able to join.\n" );
        return FALSE;
    }

    return TRUE; 
}

#endif
// _USE_FIREWALL_SKIP
