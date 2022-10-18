//------------------------------------------------------------------------------------
// Module Name			: IAF_Export(.h)
// First Modified Date	: 2008/03/05
// Author				: NHN USA Game Technical Team
//						  Kevin,Byoung-Tae,Ahn(kevin.ahn@nhnusainc.com)
// Description			: 
//------------------------------------------------------------------------------------
#pragma once

#ifdef _IAF_EXPORT
    #define IAF_DLL_API	extern "C" __declspec(dllexport)
#else//_IAF_EXPORT
    #define IAF_DLL_API	extern "C" __declspec(dllimport)

	//#ifdef _DEBUG
	//    #pragma comment(lib, _T("../../lib/IAFD.lib"))
	//#else//_DEBUG
	//    #pragma comment(lib, _T("../../lib/IAF.lib"))
	//#endif//_DEBUG
#endif//_IAF_EXPORT

//------------------------------------------------------------------------------ 
namespace IAF
{
	enum _enOption
	{
		FILTER_CHAT = 0, 
		FILTER_CHARACTER ,
	};

	/* return value
	-2 : Input string is greater than OutBuffer
	-1 : Unexpected Error
	 0 : Success , Abuse Word not found
	 1 : Success , Abuse Word found
	*/
	IAF_DLL_API	
	int __stdcall Filtering(	const char* strInput , 
								char* strOutput , 
								unsigned int nOutBufSize , 
								_enOption option = FILTER_CHAT );

	/* return value
	 0 : Success
	-1 : Error(Can't find DB file)
	-2 : Error(Inappropriate DB file - checksum invalid)
	-3 : Error(Inappropriate DB file - can't decrypt db file)
	-4 : Error(Unexpected Error)
	*/
	IAF_DLL_API
	int __stdcall LoadAbuseDB( void ); 

	// Test is made for In-office QA Test.
	// Do not use this in game.
	IAF_DLL_API
	int __stdcall Test(	  const char* strInput , 
						  char* strOutput , 
						  unsigned int nOutBufSize , 
						  char* strResult,
						  _enOption option = FILTER_CHAT );
};