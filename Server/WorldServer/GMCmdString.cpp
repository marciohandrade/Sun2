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
	if( _access(GMSTRING_FILE, 0) != -1 )  //������ �����ϸ�..
	{
	//�ָ�ɾ�
		GetPrivateProfileString( "WORLD", "WORLD_NOTICE", "����_����", m_szWorldNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
		GetPrivateProfileString( "WORLD", "CHANNEL_NOTICE", "ä��_����", m_szChannelNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
		GetPrivateProfileString( "WORLD", "ZONE_NOTICE", "����_����", m_szZoneNotice, MAX_GMCMD_STRING, GMSTRING_FILE );
	}
	else
		return false;

	return true;
}