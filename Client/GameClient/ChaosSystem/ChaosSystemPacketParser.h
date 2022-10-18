#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

namespace ChaosSystem
{
    namespace ParserPacket
    {
        void ParsePacketChaosSystem(MSG_BASE* recv_packet);
        void ParsePacketChaosSystemForWorld(MSG_BASE* recv_packet);

        void ParsePacketChaosSystemZone(MSG_BASE* recv_packet); // zone 관련
        void ParsePacketChaosSystemBattleGround(MSG_BASE* recv_packet); // 전장
        void ParsePacketChaosSystemFreePVP(MSG_BASE* recv_packet); // 자유전투지역
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void ParsePacketChaosSystemGoldRush(MSG_BASE* recv_packet); // 골드 러쉬
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void ParsePacketChaosSystemSonnenSchein(MSG_BASE* recv_packet); // 소넨샤인
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void ParsePacketChaosSystemOneDaysGuildMission(MSG_BASE* recv_packet); // 길드일일미션
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND