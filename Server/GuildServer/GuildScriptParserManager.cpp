#include "StdAfx.h"
#include ".\guildscriptparsermanager.h"
#include "GuildServer.h"
#include <GuildRankInfoParser.h>

#include <GuildRightInfoParser.h>
#include <MaxGuildMemberInfoParser.h>
#include <DominationInfoParser.h>

GuildScriptParserManager::GuildScriptParserManager(void)
{
}

GuildScriptParserManager::~GuildScriptParserManager(void)
{
}

VOID	GuildScriptParserManager::InitParserPool( VOID* pPoolOption )
{

}

BOOL	GuildScriptParserManager::RegisterParser()
{   
    ERROR_PARSER( RegisterScriptData( SCRIPT_GUILD_RANK_INFO, "data\\GuildRankInfo.txt", GuildRankInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_GUILD_RIGHT_INFO, "data\\GuildRightInfo.txt", GuildRightInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAX_GUILD_MEMBER_INFO, "data\\GuildMemberInfo.txt", MaxGuildMemberInfoParser::Instance() ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_DOMINATIONINFO, "data\\DominationInfo.txt", DominationInfoParser::Instance() ) );
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];	
	if( !GetScriptDataNodeList( pNodeTable, SCRIPT_LIST_SIZE ) )	return FALSE;
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( GuildServer::StartServer );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return TRUE;
}

VOID	GuildScriptParserManager::DestroyInstanceEx()
{
	GuildRankInfoParser::DestroyInstance();
	GuildRightInfoParser::DestroyInstance();

	MaxGuildMemberInfoParser::DestroyInstance();
    DominationInfoParser::DestroyInstance();
}

