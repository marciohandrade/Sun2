#include "stdafx.h"

#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SolarDump.h"

#include "PacketHandler/PacketHandler.h"

#include "Mocks/MockAgentLog.h"

#ifdef _DEBUG
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/AgentServer_d.lib")
#else
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/AgentServer.lib")
#endif

extern "C" {
    int mainCRTStartup();
}

int SunTestEntryPoint()
{
    SharedMemoryPool::CreateInstance();
    return mainCRTStartup();
}

static SolarDump solar_dump;

// �۷ι� �׽�Ʈ ȯ��
class AgentServerEnvironment : public ::testing::Environment
{
public:
    virtual ~AgentServerEnvironment() {}
    virtual void SetUp()
    {
        g_pLOG = new MockAgentLog();
        PacketHandler::RegisterHandler();
    }
    virtual void TearDown()
    {
        PacketHandler::ReleaseHandler();
        SAFE_DELETE(g_pLOG);
    }
};

int main(int argc, char* argv[]) {
    std::cout << "Running AgentServerTest" << std::endl;

    ::testing::InitGoogleMock(&argc, argv);
    // �۷ι� �׽�Ʈ ȯ������ ���޵Ǵ� ��ü���� ���� ������ GoogleTest�� �ñ�
    ::testing::AddGlobalTestEnvironment(new AgentServerEnvironment());
    return RUN_ALL_TESTS();
}
