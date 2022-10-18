#include "stdafx.h"

#include <iostream>

#include <gtest/gtest.h>

#include "SolarDump.h"

#pragma comment(lib, "GameServer_d.lib")
#pragma comment(lib, "ProgramCommon_sd.lib")

extern "C" {
    int mainCRTStartup();
}

int SunTestEntryPoint()
{
    SharedMemoryPool::CreateInstance();
    return mainCRTStartup();
}

static SolarDump solar_dump;

int main(int argc, char* argv[]) {
    std::cout << "Running GameServerTest" << std::endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
