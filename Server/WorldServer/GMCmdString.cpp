#include "stdAfx.h"
#include ".\gmcmdstring.h"
#include <io.h>

GMCmdString::GMCmdString(void)
{
	ZeroMemory( m_szWorldNotice, MAX_GMCMD_STRING );
	ZeroMemory( m_szChannelNotice, MAX_GMCMD_STRING );
	ZeroMemory( m_szZoneNotice, MAX_GMCMD_STRING );
}

GMCmdString::~GMCmdString(void)
{
}


bool	GMCmdString::Init()
{	
	if( _access(GMSTRING_FILE, 0) != -1 )  //파일이 존재하면..
	{
	//주명령어
		GetPrivateProfileString( "WORLD", "WORLD_NOTICE", "월드_공지", m_szWorldNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
		GetPrivateProfileString( "WORLD", "CHANNEL_NOTICE", "채널_공지", m_szChannelNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
		GetPrivateProfileString( "WORLD", "ZONE_NOTICE", "지역_공지", m_szZoneNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
	}
	else
		return false;

	return true;
}