#ifndef  __WBANetwork_H
#define  __WBANetwork_H

#include <crtdbg.h>

#pragma		comment(lib, "ws2_32.lib")
#pragma		comment(lib, "netapi32")
#pragma		comment(lib, "mpr.lib")

#ifdef  _CRT_DEBUG
	//  Detection memory leak
	#define  _new_dbg_	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define  _new_dbg_	new
#endif

#if !defined( _MT )
	#error You must select multi-threaded libraries if you want to use WBANetwork Network Library.
#endif


namespace WBANetwork
{
	//  Error Handling
	typedef	void	( *LPFN_ErrorHandler )( DWORD lastError, char* desc );
			void	CallbackErrorHandler( DWORD lastError, char* desc );

			bool	Initialize( char* mutexName = 0, LPFN_ErrorHandler handler = 0 );
			DWORD	StringToHash( char* target );
};

// WBANetwork error define
#define EXT_ERROR_BASE					WSABASEERR + 10000
#define EXT_ERROR_ZERO_BYTE_RECEIVE		EXT_ERROR_BASE + 1

#endif
