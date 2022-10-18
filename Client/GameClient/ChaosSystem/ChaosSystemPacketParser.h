#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

namespace ChaosSystem
{
    namespace ParserPacket
    {
        void ParsePacketChaosSystem(MSG_BASE* recv_packet);
        void ParsePacketChaosSystemForWorld(MSG_BASE* recv_packet);

        void ParsePacketChaosSystemZone(MSG_BASE* recv_packet); // zone ����
        void ParsePacketChaosSystemBattleGround(MSG_BASE* recv_packet); // ����
        void ParsePacketChaosSystemFreePVP(MSG_BASE* recv_packet); // ������������
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void ParsePacketChaosSystemGoldRush(MSG_BASE* recv_packet); // ��� ����
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void ParsePacketChaosSystemSonnenSchein(MSG_BASE* recv_packet); // �ҳٻ���
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void ParsePacketChaosSystemOneDaysGuildMission(MSG_BASE* recv_packet); // ������Ϲ̼�
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND