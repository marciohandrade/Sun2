#include "WorldServerPch.hxx"
#include "./WorldScriptParserManager.h"

#include <SunEventInfoParser.h>
#include <MapInfoParser.h>
#include "WorldServer.h"
#include <GameOptionParser.h>

#include "Channel/WorldChannelManager.h"
#include "Functions/GMCmdManager.h"

//==================================================================================================

ns_world::WorldScriptParserManager::WorldScriptParserManager()
{
}

ns_world::WorldScriptParserManager::~WorldScriptParserManager()
{
}

void ns_world::WorldScriptParserManager::InitParserPool(void* pool_option)
{
    MapInfoParser::Instance()->Init(0);
}

BOOL ns_world::WorldScriptParserManager::RegisterParser()
{
    ERROR_PARSER(RegisterScriptData(SCRIPT_GAME_OPTION, "data\\GameOption.ini",
                                    GameOptionParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt",
                                    MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_SUNEVENTINFO, "data\\SunEventInfo.txt",
                                    SunEventInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WORLD, "data\\World.txt",
                                    MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELD, "data\\Field.txt",
                                    MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELDWMO, "data\\fieldwmo.txt",
                                    MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELDWUT, "data\\fieldwut.txt",
                                    MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));
    ERROR_PARSER(RegisterScriptData(SCRIPT_MAPENTERANCEPORTAL, "data\\MapEnterancePortal.txt",
                                    MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_SHOUTRANGE, "data\\ShoutRange.txt",
                                    ns_world::WorldChannelManager::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_GMRIGHTS, "data\\GMRights.txt",
                                    ns_world::GMCmdManager::Instance()));
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    sSCRIPT_DATA_INFO_NODE script_data_list[SCRIPT_LIST_SIZE];
    if (!GetScriptDataNodeList(script_data_list, _countof(script_data_list))) {
        return false;
    };
    ScriptDataManager::RegisterDataList(script_data_list);
    ScriptDataManager::RegisterCallbackAllDataLoaded(&WorldServer::StartServer);
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

    return true;
}

void ns_world::WorldScriptParserManager::DestroyInstanceEx()
{
    SunEventInfoParser::DestroyInstance();
    GameOptionParser::DestroyInstance();
}
