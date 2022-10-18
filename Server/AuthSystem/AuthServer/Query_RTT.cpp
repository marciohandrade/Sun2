#include "stdafx.h"
#include "Query_RTT.h"
#include "AuthFrame.h"


VOID QueryControlIp::onResult()
{
	QueryObject::onResult();

	AuthFrame::Instance()->ReleaseControlIp();

	int Count = 0;
	while( m_recordSet.IsEndRecord() == false )
	{
		AuthFrame::Instance()->AddControlIp( &m_Record );
		m_recordSet.Next();
		Count++;
	}

	AuthFrame::Instance()->SendControlIp_ToLoginFront();

	AuthFrame::Instance()->GetReadyState().SetReadyState( AUTH_READY_CONTROL_IPLIST, TRUE );

	SUNLOG( eCRITICAL_LOG, "[QueryControlIp::onResult] Control IP List[%d]", Count );
}

VOID QueryGameVersionUpdate::ParseVersion( char* version, BYTE* out )
{
	char*	token;
	int		index = 0;

	token = ::strtok( version, ". " );
	while( token != 0 )
	{
		*( out + index++ ) = atoi( token );
		token = ::strtok( 0, ". " );
	}
}

VOID QueryGameVersionUpdate::onResult()
{
	QueryObject::onResult();

	static char buff[128];
	std::string versionString;

	while( m_recordSet.IsEndRecord() == false )
	{
		TCHAR	ver[20];

		::WideCharToMultiByte( CP_ACP, 0, m_Record.vc_Version, -1, ver, 20, 0, 0 );

		GAME_VERSION TempVersion;
		ParseVersion( ver, ( BYTE* )&TempVersion );

		switch( m_Record.ti_Type )
		{
		case GAME_VERSION_PROTOCOL: 
			AuthFrame::Instance()->SetVerProtocol(TempVersion);	
			versionString = "Protocol"; 
			sprintf( buff, "[%d.%d.%d] ", TempVersion.m_ClassA, TempVersion.m_ClassB, TempVersion.m_ClassC );
			versionString += buff;
			break;
		case GAME_VERSION_LAUNCHER: 
			AuthFrame::Instance()->SetVerLauncher(TempVersion);
			versionString += "Launcher"; 
			sprintf( buff, "[%d.%d.%d] ", TempVersion.m_ClassA, TempVersion.m_ClassB, TempVersion.m_ClassC );
			versionString += buff;
			break;
		case GAME_VERSION_MAJOR: 
			AuthFrame::Instance()->SetVerMajor(TempVersion);
			versionString += "Major"; 
			sprintf( buff, "[%d.%d.%d] ", TempVersion.m_ClassA, TempVersion.m_ClassB, TempVersion.m_ClassC );
			versionString += buff;
			break;
		case GAME_VERSION_MINOR: 
			AuthFrame::Instance()->SetVerMinor(TempVersion);
			versionString += "Minor"; 
			sprintf( buff, "[%d.%d.%d]", TempVersion.m_ClassA, TempVersion.m_ClassB, TempVersion.m_ClassC );
			versionString += buff;
			break;
		}
		m_recordSet.Next();
	}

	AuthFrame::Instance()->SendGameVersion_ToLoginFront();

	AuthFrame::Instance()->GetReadyState().SetReadyState( AUTH_READY_GAMEVERSION, TRUE );

	SUNLOG( eCRITICAL_LOG, "[QueryGameVersionUpdate::onResult] %s", versionString.c_str() );
}








