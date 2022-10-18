#pragma once
#ifndef WORLDSERVER_PRECOMPILED_HEADER_HXX
#define WORLDSERVER_PRECOMPILED_HEADER_HXX

#include <Global.Define.h>
//==================================================================================================

#include <tchar.h>
#include <windows.h>
#include <assert.h>

//==================================================================================================
// SUN project (default)
//
#include <Define.h>
#include <ServerCommonDef.h>
#include <UtilityCommon.h>

#include <Const.h>
#include <ServerConst.h>
#include <ServerBuildVersion.h>
#include <SolarAuth.Common.h>

#include <Struct.h>
#include <ServerStruct.h>

#include <PacketStruct_CG.h>
#include <PacketStruct_CW.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_DW.h>

#include <SunLog.h>
#include <ServerFrame.h>

//==================================================================================================
// Major namespace hierarchy
namespace ns_world
{
    struct IShutdown;
    //
    class WorldUser;
    class WorldUserManager;
    class WorldChannelManager;
    class ServerSessionInWorld;
    class GroupManager;
    class FriendMap;
    class ChattingService;
    class WorldSystemMemo;
    //
    namespace community
    {
        class WorldChannel;
        class WorldChannelManager;

        struct ICommunity;
        class ZoneCommunity;
        class PartyCommunity;
        class ChatRoomCommunity;

        class FieldInfo;
    };
    namespace sessions
    {
    };
}; //end of namespace 'ns_world'
//==================================================================================================
// SUN project (in world server)
#include "CommonStructInWorld.h"
#include "Services/WorldLog.h"

//==================================================================================================
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

namespace ns_world {
;

extern CSunLog* count_log_global;
extern bool MessageCount(eSUN_LOG_LEVEL log_level, const char* meta_format, ...);
#define COUNTLOG ns_world::MessageCount

extern WorldLog* world_log;
#define GAMELOG if (ns_world::world_log) ns_world::world_log

};

#define WORLD_CONTENTS_BLOCK (0)

#endif //WORLDSERVER_PRECOMPILED_HEADER_HXX
