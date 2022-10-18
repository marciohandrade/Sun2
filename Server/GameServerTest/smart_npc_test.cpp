#include "stdafx.h"
#include <gtest/gtest.h>

#include <NPCInfoParser.h>
#include <ExtraNPCManager.h>
#include <ObjectFactory.h>

TEST(smart_npc_test, /*DISABLED_*/smart_npc_test)
{
    char path[256];
    GetCurrentDirectory(256, path);
    TableData::CreateEnvironment();

    NPCInfoParser* const npc_parser = NPCInfoParser::Instance();
    //npc_parser->Load(SCRIPT_NPCINFO, "data\\npcinfo.txt", false, SEPERATOR_WHITESPACE, NULL, false);
    npc_parser->Load(SCRIPT_NPCEXTRAINFO, "data\\npcextrainfo.txt", false, SEPERATOR_WHITESPACE, NULL, false);
    g_ExtraNPCManager.initialize();

    EXTRA_NPCINFO* const extra_info = npc_parser->GetExtraInfo(64198);
    EXPECT_TRUE(NULL != extra_info);
    CLuaStreamPack* lua_pack = g_ExtraNPCManager.find_lua_pack(extra_info->m_NPCCODE);
    EXPECT_TRUE(NULL != lua_pack);

    EXPECT_EQ(0, g_ExtraNPCManager.execute_script_main( \
        /*CID*/0, /*NID*/0, /*EID*/0, /*STEP*/0, lua_pack));

    EXPECT_EQ(0, g_ExtraNPCManager.execute_script_menu( \
        /*CID*/0, /*NID*/0, /*EID*/2, /*STEP*/0, lua_pack));
    
    g_ExtraNPCManager.release();
    TableData::DestroyEnvironment();
};