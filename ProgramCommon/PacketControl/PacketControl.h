#pragma once

//	보안 이슈 + 코드크기 감소
//	-	PACKET_CONTROL_PACKING
#if defined(_SERVER) && !defined(_DEBUG)
    #define PACKET_CONTROL_PACKING  (1)
#elif defined(_DEBUG)
    #define PACKET_CONTROL_PACKING  (isPack)
#else // CLIENT - RELEASE
    #define PACKET_CONTROL_PACKING  (0)
#endif


struct MSG_BASE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
namespace nsPacket
{
    struct PacketInfo
    {
        MSG_BASE*   pMsg;
        WORD        Size;
    };

    class PacketControl
    {
        //_NA_0_20100211_SD_EFFECT_MODIFY
        enum {  
            szPlayerAttack = 41,
            szPlayerAreaAttack = 418,
            szMonsterAttack = 34,
            szStyleAttack = 1167,
            szSkillDamege = 15,
            szPeriodicDamege = 333,
            szReflectDamege = 28,
            szBombDamege = 397,
            szBombInfo = 12,
        };
    public:
        //	Heuristic Pack
        static PacketInfo Pack(MSG_BASE* const pMsg, const WORD wSize);
        //	Heuristic UnPack
        static PacketInfo UnPack(MSG_BASE* const pMsg, const WORD wSize);
    private:
        static PacketInfo MakeStream(const BOOLEAN isPack, MSG_BASE* const pMsg, const WORD wSize);
    private:
        static const DWORD SIZEofSTREAM         = 65536;
        static const DWORD SIZEofCOMPOSESTREAM  = 8192;
    };

    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //
    class Environment
    {
    public:
        static BOOLEAN  Init();
    };
};

