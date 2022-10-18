#include "stdafx.h"

#include <iostream>

#include <gtest/gtest.h>

#include "SolarDump.h"

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
    std::cout << "Running PortalServerTest" << std::endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
