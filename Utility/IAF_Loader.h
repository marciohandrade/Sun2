//------------------------------------------------------------------------------------
// Module Name			: IAF_Loader(.h)
// First Modified Date	: 2008/03/05
// Author				: NHN USA Game Technical Team
//						  Kevin,Byoung-Tae,Ahn(kevin.ahn@nhnusainc.com)
// Description			: 
//------------------------------------------------------------------------------------
#pragma once
#ifndef _WINDOWS_
#include <windows.h>
#endif//_WINDOWS_

namespace IAF
{
	enum _enOption
	{
		FILTER_CHAT = 0, 
		FILTER_CHARACTER ,
	};

	//Declare DLL function pointer 
	typedef int (__stdcall *IAFFilterDL )( const char* , char* , unsigned int , _enOption );
	typedef int (__stdcall *IAFLoadAbuseDBDL)();
	typedef int (__stdcall *IAFTest)( const char* , char* , unsigned int , char* , _enOption );
	//static 
	static HMODULE			g_hIAF			= NULL;
	static IAFFilterDL		g_pfIAFFilter	= NULL;
	static IAFLoadAbuseDBDL g_pfIAFLoadAbuseDB	= NULL;
	static IAFTest			g_pfIAFTest		= NULL;

	bool Load( void )
	{
#ifdef _DEBUG
		g_hIAF = LoadLibrary(_T("IAFD.dll"));
#else//_DEBUG
		g_hIAF = LoadLibrary(_T("IAF.dll"));
#endif//_DEBUG

		g_pfIAFFilter		= (IAFFilterDL)::GetProcAddress( g_hIAF , _T("_Filtering@16") );
		g_pfIAFLoadAbuseDB	= (IAFLoadAbuseDBDL)::GetProcAddress( g_hIAF , _T("_LoadAbuseDB@0") );
		g_pfIAFTest			= (IAFTest)::GetProcAddress( g_hIAF , _T("_Test@20") );

		return ( g_pfIAFFilter != NULL && g_pfIAFLoadAbuseDB != NULL && g_pfIAFTest != NULL );
	}

	/* return value
	-2 : Input string is greater than OutBuffer
	-1 : Unexpected Error
	 0 : Success , Abuse Word not found
	 1 : Success , Abuse Word found
	*/
	int wrpFiltering(	const char* strInput, 
						char* strOutput, 
						unsigned int nOutBufSize , 
						_enOption option )
	{
		int nRet = -1;
		if ( g_pfIAFFilter )
		{
			nRet = g_pfIAFFilter( strInput , strOutput , nOutBufSize , option );
		}
		return nRet;
	}

	/* return value
	 0 : Success
	-1 : Error(Can't find DB file)
	-2 : Error(Inappropriate DB file - checksum invalid)
	-3 : Error(Inappropriate DB file - can't decrypt db file)
	-4 : Error(Unexpected Error)
	*/
	int wrpLoadAbuseDB( void )
	{
		int nRet = -4;
		if ( g_pfIAFLoadAbuseDB )
		{
			nRet = g_pfIAFLoadAbuseDB();
		}
		return nRet;
	}
	// wrpTest is made for In-office QA Test.
	// Do not use this in game.
	int wrpTest( const char* strInput, 
				   char* strOutput, 
				   unsigned int nOutBufSize ,
				   char* strResult,
				   _enOption option )
	{
		int nRet = -1;
		if ( g_pfIAFTest )
		{
			nRet = g_pfIAFTest( strInput , strOutput , nOutBufSize , strResult , option );
		}
		return nRet;
	}
	void Unload( void )
	{
		if ( g_hIAF != NULL ) ::FreeLibrary( g_hIAF );
	}
};