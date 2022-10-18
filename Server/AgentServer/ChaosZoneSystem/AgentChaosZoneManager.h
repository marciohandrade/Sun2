#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include <Singleton.h>
class GameServerSession;
class AgentChaosZoneManager : public util::Singleton<AgentChaosZoneManager>
{
public:
    AgentChaosZoneManager()
    {
    }
    ~AgentChaosZoneManager()
    {
    }
    void ParseCGPacket(User* user, MSG_BASE* packet, WORD size);
    void ParseAGPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND