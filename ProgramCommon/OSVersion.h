////////////////////////////////////////////////////////////////////////////////////////
//
//	< COSVersion >
//
//		Retrieve the Windows OS version.
//
//	* AUTHOR
//		Nam, in hyouk (GreenB)
//
//	* ACKNOWLEDGEMENT
//		Hans Dietrich(hdietrich@gmail.com)
//		I found it by reference to his "CXWinVer" class.
//		Please see his article at www.codeproject.com
//
//	* HISTORY
//		2009-04-11	Initial release.
//
//	* LICENCE
//     This software is released into the public domain. You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
//
//	copyright(c) 2009. GreenB. All right reserved.
//	http://greenmaru.com
//
//
////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GREENB_OSVERSION_
#define _GREENB_OSVERSION_
#pragma once

#include <windows.h>

namespace Greenmaru {

	////////////////////////////////////////////////////////////////////////////////////
	//
	//	COSVersion
	//
	////////////////////////////////////////////////////////////////////////////////////
	class COSVersion
	{
		////////////////////////////////////////////////////////////////////////////////
		//
		//	MEMBER VARS
		//
		////////////////////////////////////////////////////////////////////////////////
	private:
		OSVERSIONINFOEX m_osver;

		////////////////////////////////////////////////////////////////////////////////
		//
		//	CONSTRUCTION
		//
		////////////////////////////////////////////////////////////////////////////////
	public:
		COSVersion( void )
		{
			memset( &m_osver, NULL, sizeof( OSVERSIONINFOEX ) );
			m_osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

			if ( GetVersionEx( reinterpret_cast<LPOSVERSIONINFO>(&m_osver) ) )
			{
				//
				// Is NT4 SP6 or later?
				//
				if ( ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT ) &&  (m_osver.dwMajorVersion >= 5 ) )
				{
					memset( &m_osver, NULL, sizeof( OSVERSIONINFOEX ) );
					m_osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
					GetVersionEx( reinterpret_cast<LPOSVERSIONINFO>(&m_osver) );
				}
			}
		}

		~COSVersion( void )
		{
		}

		////////////////////////////////////////////////////////////////////////////////
		//
		//	PROPERTY
		//
		////////////////////////////////////////////////////////////////////////////////
	public:

		DWORD MajorVersion( void ) const
		{
			return m_osver.dwMajorVersion;
		}

		DWORD MinorVersion( void ) const
		{
			return m_osver.dwMinorVersion;
		}

		DWORD BuildNumber( void ) const
		{
			return ( m_osver.dwBuildNumber & 0xFFFF );
		}

		DWORD ServicePack( void ) const
		{
			int nServicePack = 0;
			for ( int i = 0 ; ( m_osver.szCSDVersion[i] != _T('\0') ) && ( i < (sizeof(m_osver.szCSDVersion) / sizeof(TCHAR) ) ) ; i++ )
			{
				if ( _istdigit( m_osver.szCSDVersion[i] ) )
				{
					nServicePack = _ttoi( &m_osver.szCSDVersion[i] );
					break;
				}
			}
			return nServicePack;
		}

		//
		// Windows 9x
		//
		BOOL IsWindows9x( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && m_osver.dwMajorVersion == 4 )
				return TRUE;
			return FALSE;
		}

		//
		// Windows NT
		//
		BOOL IsWindowsNT4( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 4 && m_osver.dwMinorVersion == 0 )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsNT4SP6( void ) const
		{
			if ( IsWindowsNT4() && ServicePack() == 6 )
				return TRUE;
			return FALSE;
		}

		//
		// Windows XP / 2000 / 2003
		//
		BOOL IsWindowsXp( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 1 )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsXpSP2( void ) const
		{
			if ( IsWindowsXp() && ServicePack() == 2 )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsXpSP3( void ) const
		{
			if ( IsWindowsXp() && ServicePack() == 3 )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsXpHome( void ) const
		{
			if ( IsWindowsXp() && m_osver.wSuiteMask & VER_SUITE_PERSONAL )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsXpPro( void ) const
		{
			if ( IsWindowsXp() && m_osver.wProductType == VER_NT_WORKSTATION && !IsWindowsXpHome() )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsXpOrLater( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion >= 5 )
				return TRUE;
			return FALSE;
		}

		BOOL IsWindows2K( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 0 )
				return TRUE;
			return FALSE;
		}

		BOOL IsWindows2003( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 2 )
				return TRUE;
			return FALSE;
		}

		//
		// Windows Vista / 2008 / 7
		//
		BOOL IsWindowsVista( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 6 && m_osver.dwMinorVersion == 0 && m_osver.wProductType == VER_NT_WORKSTATION )
				return TRUE;
			return FALSE;
		}
		BOOL IsWindowsVistaOrLater( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion >= 6 )
				return TRUE;
			return FALSE;
		}

		BOOL IsWindows2008( void ) const
		{
			if ( m_osver.dwPlatformId == VER_PLATFORM_WIN32_NT && m_osver.dwMajorVersion == 6 && m_osver.dwMinorVersion == 0 && m_osver.wProductType != VER_NT_WORKSTATION )
				return TRUE;
			return FALSE;
		}

		BOOL IsWindows7( void ) const
		{
			return FALSE;
		}

		//
		//	Retrieve the server or workstation
		//
		BOOL IsServerOS( void ) const
		{
			if ( m_osver.wProductType > VER_NT_WORKSTATION )
				return TRUE;
			return FALSE;
		}

		BOOL IsDomainController( void )
		{
			return ( m_osver.wProductType && VER_NT_DOMAIN_CONTROLLER );
		}
	};

};		// namespace Greenmaru

#endif	// _GREENB_OSVERSION_