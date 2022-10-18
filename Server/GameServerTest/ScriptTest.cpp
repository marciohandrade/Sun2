#include "stdafx.h"
#include <gtest/gtest.h>

#include "../Server/GameServer/FieldServer.h"
#include "../GameServer/GameServerEx.h"
#include "../Server/Common/ServerOptionParser.h"
#include "../GameServer/GameScriptParserManager.h"

class ScriptTest : public ::testing::Test
{

};

TEST(ScriptTest, DISABLED_ScriptLoadTest)
{
    g_pGameServer = new FieldServer;

    GameScriptParserManager script_parser_manager;
    script_parser_manager.RegisterParser();
    SERVER_POOL_OPTION& stServerPoolOptionInfo = ServerOptionParser::Instance()->GetPoolInfo();
    script_parser_manager.InitParserPool( (VOID*)&stServerPoolOptionInfo );
    script_parser_manager.Load();
}