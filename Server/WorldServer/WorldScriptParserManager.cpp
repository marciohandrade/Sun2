#include "StdAfx.h"
#include ".\worldscriptparsermanager.h"

#include <SunEventInfoParser.h>
#include <MapInfoParser.h>
#include "ChannelManager.h"
#include "WorldServer.h"
#include <GameOptionParser.h>
#include "ProhibitionWordParser.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

WorldScriptParserManager::WorldScriptParserManager(void)
{
}

WorldScriptParserManager::~WorldScriptParserManager(void)
{
}

VOID	WorldScriptParserManager::InitParserPool( VOID* pPoolOption )
{
	MapInfoParser::Instance()->Init( MAPINFO_PARSER_POOL );
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    NotificationInfoParser::Instance()->Init(0);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
}

BOOL	WorldScriptParserManager::RegisterParser()
{
	ERROR_PARSER( RegisterScriptData( SCRIPT_GAME_OPTION, "data\\GameOption.ini", GameOptionParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt", MapInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SUNEVENTINFO, "data\\SunEventInfo.txt", SunEventInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD, "data\\World.txt", MapInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELD, "data\\Field.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWMO, "data\\fieldwmo.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWUT, "data\\fieldwut.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAPENTERANCEPORTAL, "data\\MapEnterancePortal.txt", MapInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SHOUTRANGE, "data\\ShoutRange.txt", ChannelManager::Instance() ) );
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    ERROR_PARSER( RegisterScriptData( SCRIPT_NOTIFICATION_INFO, "data\\NotificationInfo.txt", NotificationInfoParser::Instance() ) );
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];	
	if( !GetScriptDataNodeList( pNodeTable, SCRIPT_LIST_SIZE ) )	return FALSE;
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( WorldServer::StartWorldServer );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return TRUE;
}

VOID	WorldScriptParserManager::DestroyInstanceEx()
{
    SunEventInfoParser::DestroyInstance();
	GameOptionParser::Instance()->DestroyInstance();
	MapInfoParser::Instance()->DestroyInstance();
    ProhibitionWordParser::Instance()->DestroyInstance();
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    NotificationInfoParser::Instance()->DestroyInstance();
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
}
