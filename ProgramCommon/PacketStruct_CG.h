#pragma once
#ifndef __PACKETSTRUCT_CG_H__
#define __PACKETSTRUCT_CG_H__

#include <Define.h>
#include <Struct.h>
#include <Protocol_CG.h>
#include <PacketStruct.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#include <ProgramCommon/WzDataType.h>
#pragma warning(pop)



#include <SafeStringBufferControl.h>
#include "../PartyLib/PartyStruct.h"

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#include "../RankingLib/RankingStruct.h"
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

#if USING_HACKSHIELD

    #ifdef _SERVER
        #define HACKSHIELD_INCLUDE_PATH( v ) <HShield/##v/AntiCpXSvr.h>
    #else
        #define HACKSHIELD_INCLUDE_PATH( v ) <HShield/##v/HShield.h>
    #endif

    #include HACKSHIELD_INCLUDE_PATH( HACKSHIELD_VERSION )

#endif //USING_HACKSHIELD



#pragma pack(push,1)
#pragma warning(push)
#pragma warning(disable : 4201 4307)
//==================================================================================================
// *  Prefix
//      C : Client
//      G : Game Server
//      M : Master Server
//      D : DBP Server
//      W : World Server
//
// * Postfix
//      SYN - Server�� ��û
//      ACK - ��û�� ���� ���
//      NAK - ��û�� ���� �ź�
//      CMD - Server�� ���
//      BRD - Server�� ����ȭ ���� ������ �˸�
//
// * �������� Naming Rule
//      Prefix_ī�װ���_�������ݸ�_Postfix
//      ex) CG_CONNECTION_REGISTER_SYN
//
//==================================================================================================

//--------------------------------------------------------------------------------------------------
///! CG_CONNECTION, // ����
#define PACKET_CG_OPEN(c, p) \
    struct MSG_##p : public MSG_BASE_FORWARD { \
        MSG_##p() { \
            m_byCategory = c; \
            m_byProtocol = p; \
        }

#define PACKET_CG_CLOSE \
    }
//--------------------------------------------------------------------------------------------------
#define PACKET_CG_FWD_OPEN(c, p) \
    struct MSG_##p : public MSG_BASE_FORWARD { \
        MSG_##p() { \
            m_byCategory = c; \
            m_byProtocol = p; \
        }

#define PACKET_CG_FWD_CLOSE \
    }
//--------------------------------------------------------------------------------------------------
#define PACKET_CG_INHERIT_OPEN(c, p, b) \
    struct MSG_##p : public b { \
        MSG_##p() { \
            m_byCategory = c; \
            m_byProtocol = p; \
        }

#define PACKET_CG_INHERIT_CLOSE \
    }

//==================================================================================================
//==================================================================================================
//==================================================================================================

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_HEARTBEAT_SYN);
    PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T2(54), 1);
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_CONNECTION, CG_CONNECTION_BACKTOCHARSELECT_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_CONNECTION, CG_CONNECTION_BACKTOCHARSELECT_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_CONNECTION, CG_CONNECTION_BACKTOCHARSELECT_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_DISCONNECT_CMD)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_SERVER_SHUTDOWN_BRD)
    DWORD m_dwStringCode;
    BYTE  m_byShutdownTime; //< �ð�(m_byShutdownTime�ð��� ���� ����)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_RESTART_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_REENTERSERVER_SYN)
    PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T2(77), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_SYN)
    PACKET_RND_TOTAL_BLOCK_START(GETRNDNO_RNG128_T2(29));
        PACKET_RND_PAD_START(GETRNDNO_RNG04_T3(109));
            // ���������� ������ �������̵�
            DWORD m_dwAuthID;
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(44), 1);
            // �������̵� (ENCODE TARGET)
        #if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || \
            defined(__NA000000_GLOBAL_AUTH_PROCESS__) // || \
            //defined(__KR000000_090128_CHANNELING__) <- ���� �ݿ��ϵ��� ����. �ϰ��ݿ��� ���ؼ� auth�迭 ���� ���� ���� �ʿ�.
            char m_szID[ID_MAX_LEN + 1];
        #else
            char m_szID[16];
        #endif
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(41), 2);

            PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T5(9));
                // Ŭ���̾�Ʈ ���� Major
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byHighVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(8), 1);
                // Ŭ���̾�Ʈ ���� Minor
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byMiddleVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(2), 2);
                // Ŭ���̾�Ʈ ���� Devel
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byLowVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(9), 3);
                // ���� �ڵ�
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byNationCode);
            PACKET_RND_SHRBLK64_END();
            PACKET_RND_PAD_CONT(GETRNDNO_RNG16_T5(141), 3);
            // ������������ ������ �ø��� Ű (ENCODE TARGET)
            BYTE  m_szSerialKey[32];
            DWORD m_dwMOLAProtocolCRC;
        #ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
             char mac_address_[30];
        #endif
        PACKET_RND_PAD_END();
    PACKET_RND_TOTAL_BLOCK_END();

    void Encode();
    void Decode();
    void _Encode_T1();
    void _Encode_T2();
    void _Encode_T3();
    void _Decode_T1();
    void _Decode_T2();
    void _Decode_T3();
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_ACK)
    // CHANGES: f110608.5L
    static const size_t kCharacterPartPaddingSize = sizeof(WORD);
    static const WORD kTailMark = \
        MAKEWORD(CG_CONNECTION_ENTERSERVER_SYN, CG_CONNECTION_ENTERSERVER_NAK);
    // (WAVERIX) (NOTE)
    //   �̰͵� ������ ����, ũ�� ��å�� ��ĳ�� ���ΰ��� ���� �̼������̹Ƿ� �̹��� �н�
    // (WAVERIX) (NOTE) (090719) �� ���� ���� �ִ�. ���� �۾��� ����.
    BOOL m_bPVP; //__NA_000839_20080129_PVP_OPTION
    BYTE m_byCount;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE char_slot_size; // ��� ������ ĳ���� ���� ����
#endif
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    BYTE m_ReNameStatus[MAX_CHARACTER_LIST_NUM]; //ĳ���͸� ���� ���� �÷��� ���̴� ĳ���� ������ ���� ���� ���� �Ҵ��Ѵ�.
#endif
    BYTE m_pBuffer[sizeof(CLIENT_CHARACTER_PART) * MAX_CHARACTER_LIST_NUM +
                   kCharacterPartPaddingSize];
    // <�ӽù��۷� ����� Ÿ��>
    //CLIENT_CHARACTER_PART rpCharacterList[MAX_CHARACTER_LIST_NUM]
    uint16_t ConvertForSend(CLIENT_CHARACTER_PART rpCharacterList[MAX_CHARACTER_LIST_NUM]);
    bool ConvertForRecv(CLIENT_CHARACTER_PART rpCharacterList[MAX_CHARACTER_LIST_NUM]);
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;



PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERVILLAGEREQ_SYN)
    PACKET_RND_TOTAL_BLOCK_START(GETRNDNO_RNG16_T1(129));
        PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T5(35));
            PACKET_RND_SHRBLK64_CONT(BYTE, m_bySelectedCharIndex);
            PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T4(9), 1);
        PACKET_RND_SHRBLK64_END();
    PACKET_RND_TOTAL_BLOCK_END();
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERVILLAGEREQ_ACK)
    PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T5(9));
        PACKET_RND_SHRBLK64_CONT(DWORD, m_dwPlayerKey);
        PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(8), 1);
    PACKET_RND_SHRBLK64_END();
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERVILLAGEREQ_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_SYN)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_ACK)
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    bool enabled;
#else
    char szWorldServerIP[IP_MAX_LEN];
    WORD wWorldServerPort;
#endif
PACKET_CG_CLOSE;

// NOTE: f110916.1L, unused anymore since _NA002676_WORLD_SERVER_RENEWAL
PACKET_CG_OPEN(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_NAK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_NOTIFY_LOGOUT_SYN)
PACKET_CG_CLOSE;

// ���� �ð� ���� �˸�
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_BILLING_ALARM_BRD)
    DWORD dwRestTerm; // ���� �ð� (sec)
PACKET_CG_CLOSE;

// ũ���� ����Ʈ
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_CRASH_REPORT_SYN)
    CRASH_INFO    m_CrashInfo;
    COMPUTER_INFO m_ComputerInfo;
PACKET_CG_CLOSE;

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_SERVER_SELECT_REQUEST_SYN)
    enum eQuery { eQuery_StartTransaction, eQuery_StopTransaction };
    BOOLEAN m_bQueryType; //eQuery
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_SERVER_SELECT_REQUEST_ACK)
    DWORD m_Result; // RC::eROOM_RESULT
    nsPacket::ReAuthTokenStream m_ReAuthToken;
    inline WORD GetSize() const {
        if (m_Result == RC::RC_ROOM_SUCCESS) {
            return sizeof(*this);
        }
        return sizeof(*this) - sizeof(m_ReAuthToken);
    }
PACKET_CG_CLOSE;
#endif

//--------------------------------------------------------
//_NA000000_100609_CS_TIME_SYNCHRONIZATION_
// (f100610.3L) client->server protocol, unused
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TIME_SYNC_SYN)
    util::TimeSyncNode sync_node;
PACKET_CG_CLOSE;

// (f100610.3L) client<-server protocol
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TIME_SYNC_CMD)
    util::TimeSyncNode sync_node;
PACKET_CG_CLOSE;
//--------------------------------------------------------

//--------------------------------------------------------
#ifdef _NA_20100802_SPEED_HACK_DETECTION
// S->C TickCount ����
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_SYN)
    DWORD round;        // ������ ��Ŷ �ĺ��� ���� ��ȣ
    BYTE  index;        // ������ ��Ŷ �ĺ��� �ε��� ��ȣ
PACKET_CG_CLOSE;
// C->S TickCount ���ǿ� ���� ����
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_ACK)
    DWORD round;        // _SYN���� ���� ���� ���� �״�� ��� ����
    BYTE  index;        // _SYN���� ���� �ε��� ���� �״�� ��� ����
    DWORD tick_count;   // ó�� ������ Ŭ���̾�Ʈ TickCount
PACKET_CG_CLOSE;
#endif
//---------------------------------------------------------

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE

PACKET_CG_OPEN(CG_CONNECTION, CG_CHARNAME_CHANGE_SYN)
    SLOTIDX slot_index_;
    char    character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CHARNAME_CHANGE_ACK)
    SLOTIDX slot_index_;
    char    character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CHARNAME_CHANGE_NAK)
    enum ERROR_CODE {SUCCEED,       //����
                     DB_FAIL,       //��� ����
                     DONOT_RENAME,  //�̸��� �����Ҽ� �ִ� ĳ���Ͱ� �ƴ�
                     INDEX_INVALID, //ĳ���� ������ ��ȿ���� �ʴ�.
                     NOT_CONDITION = 10,  //�̸��� �����Ҽ� �ִ� ������ �ȵ� (��� ��, ģ�� ��� ���)
                     SEND_FAIL,     //���� ���� ���� 
                     NOT_FIND_USER, //����ڸ� ã���� ����
                    };
    SLOTIDX     slot_index_;
    ERROR_CODE  result_; 
PACKET_CG_CLOSE;

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

//==================================================================================================

//_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_APPREARANCE_LIST_BRD)
    uint16_t player_key;
#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    uint16_t total_item_score_;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE
    uint8_t number_of_items;
    struct Node {
#ifdef _NA_002935_20110704_ITEM_SCORE
        typedef RENDER_ITEMSTREAMEX ItemStream;
#else
        typedef OPT_ITEMSTREAMEX ItemStream;
#endif
        POSTYPE pos;
        ItemStream render_item;
    } nodes[eEQUIPCONTAINER_MAX];
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(nodes);
        return static_cast<uint16_t>(kMinSize + sizeof(nodes[0]) * number_of_items);
    };
PACKET_CG_CLOSE;

//_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DISAPPREARANCE_LIST_BRD)
    uint16_t player_key;
#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    uint16_t total_item_score_;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE
    uint8_t number_of_items;
    struct Node {
        POSTYPE pos;
    } nodes[eEQUIPCONTAINER_MAX];
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(nodes);
        return static_cast<uint16_t>(kMinSize + sizeof(nodes[0]) * number_of_items);
    };
PACKET_CG_CLOSE;

#if SUN_CODE_DELETE_RESERVED
///! CG_CHARINFO,   // ĳ���� ����
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_APPREARANCE_BRD)
    PLAYERKEY m_PlayerKey;
    POSTYPE   m_EquipPos;
    OPT_ITEMSTREAMEX m_OptItemStream;
#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    DWORD m_TotalItemScore;
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DISAPPREARANCE_BRD)
    PLAYERKEY m_PlayerKey;
    POSTYPE   m_EquipPos;
#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    DWORD m_TotalItemScore;
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE
PACKET_CG_CLOSE;

#endif //SUN_CODE_DELETE_RESERVED

//==================================================================================================

// UP ��ȭ����
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_USERPOINT_CMD)
    UPTYPE m_TotalUP;  // ���� �������� UP
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CREATE_SYN)
    PACKET_RND_TOTAL_BLOCK_START(GETRNDNO_RNG128_T5(99));
        PACKET_RND_PAD_START(GETRNDNO_RNG16_T4(197));
            BYTE m_byClass;
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T1(19), 1);
            char m_pszCharName[MAX_CHARNAME_LENGTH];
            BYTE m_byHeight;
            BYTE m_byFace;
            BYTE m_byHair;
#ifdef _NA_002050_20110216_ADD_GENDER
            BYTE gender;
#endif // _NA_002050_20110216_ADD_GENDER
        PACKET_RND_PAD_END();
    PACKET_RND_TOTAL_BLOCK_END();
PACKET_CG_CLOSE;


PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CREATE_ACK)
    BYTE m_pBuffer[sizeof(CLIENT_CHARACTER_PART)];
    // <�ӽù��۷� ����� Ÿ��>
    //CLIENT_CHARACTER_PART& rCharInfo

    int ConvertForSend(CLIENT_CHARACTER_PART& rCharInfo);
    void ConvertForRecv(CLIENT_CHARACTER_PART& rCharInfo);
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CREATE_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DESTROY_SYN)
    BYTE m_bySelectedCharSlotIndex;
    BYTE m_LastSocialSecurityNumber[MAX_SSN_STREAM_LENGTH];  //< �ֹι�ȣ ���ڸ�
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DESTROY_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DESTROY_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_RECOVER_SYN)
    BYTE m_bySelectedCharSlotIndex;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_RECOVER_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_RECOVER_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

#ifdef __NA_001044_ADD_CUSTOM_UI
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CHARACTERID_CHECK_SYN)
    char m_szCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CHARACTERID_CHECK_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CHARACTERID_CHECK_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;
#endif

PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_CHAR_ITEM_CMD)
    PLAYERINFO_FOR_PACKET   m_CharacterInfo;
    enum { _MAX_BINARY_SIZE = sizeof(RENDER_ITEMSTREAMEX) *
                              (MAX_INVENTORY_SLOT_NUM + MAX_TEMPINVENTORY_SLOT_NUM),
    };
    WORD m_Size;
    BYTE m_BitStreamBuffer[_MAX_BINARY_SIZE];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_BitStreamBuffer);
        return static_cast<uint16_t>(kMinSize + sizeof(m_BitStreamBuffer[0]) * m_Size);
    };

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_SKILL_CMD)
    SKILL_TOTAL_INFO m_SkillInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_SkillInfo);
        return static_cast<uint16_t>(kMinSize + m_SkillInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_QUICK_CMD)
    QUICK_TOTAL_INFO m_QuickInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_QuickInfo);
        return static_cast<uint16_t>(kMinSize + m_QuickInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_STYLE_CMD)
    STYLE_TOTAL_INFO m_StyleInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_StyleInfo);
        return static_cast<uint16_t>(kMinSize + m_StyleInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
PACKET_CG_FWD_OPEN( CG_CHARINFO, CG_CHARINFO_SKILL_COOL_TIME_CMD )
    SkillCoolTime skill_cool_time;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(skill_cool_time);
        return static_cast<uint16_t>(kMinSize + skill_cool_time.GetSize());
    }
PACKET_CG_FWD_CLOSE;
#endif

PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_SELECT_INFO_SYN)
    PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T1(59));
        PACKET_RND_SHRBLK64_CONT(DWORD, m_dwObjectKey);
        PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(8), 1);
    PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_SELECT_INFO_ACK)
    DWORD     m_dwObjectKey;
    PLAYERHP  m_HP;
    PLAYERHP  m_MaxHP;
    LEVELTYPE m_LV;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    PLAYERSD  shield;
    PLAYERSD  max_shield;
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
PACKET_CG_FWD_CLOSE;

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_FATIGUE_CMD)
    WORD m_wPlayTime;
    WORD m_wOfflineTime;
    BYTE m_byType;
PACKET_CG_FWD_CLOSE;
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_ACCUMULATE_POINT_CMD)
    DWORD m_dwReason;         // �޽����� �߻��� ����
    DWORD m_dwAccumulateTime; // ���� �ð� (��:0000000��)
    WORD  m_wAccumulatePoint; // ���� ����Ʈ
    BYTE  m_byRequirePoint;   // �ʿ� ���� ����Ʈ
PACKET_CG_FWD_CLOSE;
#endif

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CHANGE_APPEARANCE_BRD) // ĳ���� �ܸ� ���� ����� �˸�
    PLAYERKEY player_key_; // �÷��̾� Ű
    typedef BYTE ResultType;
    BOOST_STATIC_ASSERT(sizeof(ResultType) ==  sizeof(RC::ItemResultUtil::ValueType));
    BOOST_STATIC_ASSERT(sizeof(ResultType) ==  sizeof(RC::ChangeCharacterAppearanceResultUtil::ValueType));
    ResultType result_; // �ܸ� ���� ���
    CharacterAppearanceInfo character_appearance_; // ����� �ܸ�
    POSTYPE item_position_; // ������ ��ġ
    //
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CG_CLOSE;

// _NA002217_100728_EP2_PERK_SYSTEM_
PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_SKILL_LIST_CMD)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_SKILL_PERK_CMD)
    PerkDataList container_;
    uint16_t GetSize() const {
        const int kMinSize = sizeof(*this) - sizeof(container_);
        return static_cast<uint16_t>(kMinSize + container_.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD)
#ifdef _NA_006372_20130108_BADGE_ITEM
    nsSlot::TOGGLE_ITEMS toggle_items_info_[kToggleItemMax];
#endif //_NA_006372_20130108_BADGE_ITEM
PACKET_CG_FWD_CLOSE;
//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
PACKET_CG_FWD_OPEN(CG_CHARINFO, CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD)
    CHAOS_ZONE_RECORD_INFOS chaos_zone_record_infos;
PACKET_CG_FWD_CLOSE;

//==================================================================================================
// _NA000000_SPEED_HACK_PREVENTION_SUPPORT, client local test
// C->G
PACKET_CG_FWD_OPEN(CG_CONNECTION, CG_CONNECTION_CLIENT_TICK_TEST_SYN)
    DWORD client_send_tick_id;
    DWORD client_send_tick;
PACKET_CG_FWD_CLOSE;
// G->C
PACKET_CG_FWD_OPEN(CG_CONNECTION, CG_CONNECTION_CLIENT_TICK_TEST_ACK)
    DWORD client_send_tick_id;
    DWORD client_send_tick;
    DWORD server_elapsed_tick;
PACKET_CG_FWD_CLOSE;

//==================================================================================================
//_NA000000_SPEED_HACK_PREVENTION_SUPPORT, busy request prevention
// G->C
PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_KBMOVE_SERIAL_CHECK_CMD)
    nsSync::KBMoveSerialNode serial_block;
PACKET_CG_FWD_CLOSE;
// C->G
PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_KBMOVE_SERIAL_CHECK_ACK)
    DWORD serial_guid;
PACKET_CG_FWD_CLOSE;

//==================================================================================================

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_PERK_CONTROL_SYN)
    PerkControl header;
    uint16_t GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_PERK_GROUP_CONTROL_SYN)
    PerkControl header[4];
uint16_t GetSize() const {
    return sizeof(*this);
}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_PERK_GROUP_CONTROL_CMD)
    bool isCanProcessState;
    PerkControl header[4];
    PerkControlData data[4];
uint16_t GetSize() const{
	const size_t kResultDefault = sizeof(isCanProcessState);
    const size_t kMinSize = sizeof(*this) - sizeof(data);
    return static_cast<uint16_t>(kResultDefault + kMinSize + (data[0].GetSize() * 4));
}
PACKET_CG_FWD_CLOSE;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_PERK_CONTROL_CMD)
    PerkControl header;
    PerkControlData data;
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(data);
        return static_cast<uint16_t>(kMinSize + data.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_PERK_STATUS_CMD)
    PerkControl header;
    PerkStatusData data;
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(data);
        return static_cast<uint16_t>(kMinSize + data.GetSize());
    }
PACKET_CG_FWD_CLOSE;

///!------------------------------------------------------------------------------------


PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_TELEPORT_BRD)
    BYTE     m_bForcedCommand;   // Ŭ���û�� ���� ������, ���������� ���� �ڷ���Ʈ����
    DWORD    m_dwObjectKey;      // �����ϴ� ������Ʈ Ű
    WzVector vPos;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_TELEPORT_SYN)
    WzVector vPos;               // �����̵��ϰ��� �ϴ� ��ġ
PACKET_CG_FWD_CLOSE;

//_NA_0_20100722_AUTO_SYSTEM
PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK)
    BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

#ifdef _NA_20100307_BETAKEY_SYSTEM
PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_RECALL_NAK)
    enum { RECALL_CANNOT_BETAKEY = 1, };
    BYTE error_code_;
PACKET_CG_FWD_CLOSE;
#endif

//==================================================================================================
// CG_SYNC
//

//-------------------------------------------------------------------------------------------------
// �÷��̾� �ʵ� ����
//-------------------------------------------------------------------------------------------------

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_ENTER_SYN)
    PACKET_RND_TOTAL_BLOCK_START(GETRNDNO_RNG16_T4(1022));
        PACKET_RND_PAD_START(GETRNDNO_RNG04_T3(109));
            PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T4(459));
                PACKET_RND_SHRBLK64_CONT(DWORD, m_dwCheckSum);
            PACKET_RND_SHRBLK64_END();
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(11), 1);
            DWORD checksum_wmo_;
        PACKET_RND_PAD_END();
    PACKET_RND_TOTAL_BLOCK_END();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_ENTER_ACK)
    float    m_fPos[3];
    SLOTCODE m_TransSkillCode;  // �巡�� ���Ż��¶�� ���Ž�ų�ڵ带 �����ش�.
	//_NA001955_110210_WING_COSTUME
    // CHANGES: f110607.3L, added field to support whether his wings spreading status has continued.
    bool continued_wings_spreading_;
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    CHANGE_PCROOM_STATUS m_PcRoomStatus; // ref. CHANGE_PCROOM_STATUS::eStatus
#endif
    DOMINATION_GUILD_INFO m_GuildInfo; //���� ��� ����
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION    
    sDOMINATION_CONTINENT_LORD_INFO m_ContinentLordInfo;  //��� ���� ����
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    void Encode();
    void Decode();

    void _Encode_01();
    void _Encode_02();
    void _Decode_01();
    void _Decode_02();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_ENTER_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// ���ο� ���Ϳ� ĳ���Ͱ� ������ ���� ������
//-------------------------------------------------------------------------------------------------
// �ڿ� PLAYER_RENDER_INFO,ETC_STATE_TOTAL_INFO,VENDOR_RENDER_INFO �� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = 50, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� PLAYER_VILLAGE_INFO,ETC_STATE_TOTAL_INFO,VENDOR_RENDER_INFO �� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_VILLAGE_ALLPLAYERS_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = 80, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� EQUIP_ITEM_BASEINFO�� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� MONSTER_RENDER_INFO, ETC_STATE_TOTAL_INFO �� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLMONSTERS_CMD)
    enum { _MAX_MONSTER_INFO_SIZE = 80, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� SUMMONED_RENDER_INFO�� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLSUMMONEDS_CMD)
    enum { _MAX_SUMMONED_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// �ڿ� MAPNPC_RENDER_INFO�� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLMAPNPC_CMD)
    enum { _MAX_MAPNPC_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� TOTEMNPC_RENDER_INFO�� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLTOTEMNPC_CMD)
    enum { _MAX_TOTEMNPC_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
// �ڿ� MERCHANT_RENDER_INFO�� m_byCount ��ŭ ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLMERCHANTNPC_CMD)
    enum { _MAX_MERCHANTNPC_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLFIELDITEMS_CMD)
    enum { _MAX_FIELDITEM_INFO_SIZE = 80, };
    BYTE    m_byCount;
    ITEM_RENDER_INFO    m_pFieldItemRenderInfo[_MAX_FIELDITEM_INFO_SIZE];
    //
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pFieldItemRenderInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pFieldItemRenderInfo[0]) * m_byCount);
    };
PACKET_CG_FWD_CLOSE;


//_NA_0_20100208_DOMINATION_AREA_INFO
// ��� ����� ���� ��� ���� ���� ������ ���� ������.
// �������� ���� ���� �߰���.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD)
    enum { _MAX_GUILD_RENDERINFO_SIZE = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE, };
    BYTE m_byCount;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD)
    enum { _MAX_SUN_RANKING_GRADE_INFO_SIZE = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE, };
    BYTE m_byCount;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
//-------------------------------------------------------------------------------------------------
// �÷��̾�, ����, �������� �ʵ忡 ����
//-------------------------------------------------------------------------------------------------
// �ڿ� PLAYER_RENDER_INFO, ETC_STATE_TOTAL_INFO, EQUIP_ITEM_BASEINFO�� ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� PLAYER_VILLAGE_INFO, ETC_STATE_TOTAL_INFO, EQUIP_ITEM_BASEINFO�� ����´�.

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_VILLAGE_PLAYER_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_LEAVE_BRD)
    enum { _MAX_PLAYER_SIZE = 50 };
    BYTE m_byCount;
    WORD m_PlayerKey[_MAX_PLAYER_SIZE];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_PlayerKey);
        return static_cast<uint16_t>(kMinSize + sizeof(m_PlayerKey[0]) * m_byCount);
    }
PACKET_CG_FWD_CLOSE;

// �ڿ� MONSTER_RENDER_INFO�� ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MONSTER_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_SUMMONED_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MAPNPC_ENTER_BRD)
    DWORD   m_dwMapNpcID;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MERCHANT_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// �ڿ� TOTEMNPC_RENDER_INFO�� ����´�.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_TOTEMNPC_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MONSTER_DIR_ENTER_BRD)
    WORD wAngle;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


//  Boss ���� ����
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_BOSS_MONSTER_ENTER_BRD)
    DWORD    m_dwBossCode; // �������� �ڵ�
    WzVector m_vCurPos;    // ���� ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MONSTER_LEAVE_BRD)
    enum { _MAX_MONSTER_SIZE = 50 };
    BYTE  m_byCount;
    DWORD m_dwObjectKey[_MAX_MONSTER_SIZE];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_dwObjectKey);
        return static_cast<uint16_t>(kMinSize + sizeof(m_dwObjectKey[0]) * m_byCount);
    }
PACKET_CG_FWD_CLOSE;


// �ʵ������
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FIELDITEM_ENTER_BRD)
    DWORD            m_dwFromMonsterKey;
    ITEM_RENDER_INFO m_ItemRenderInfo;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FIELDITEM_LEAVE_BRD)
    enum { _MAX_FIELDITEM_SIZE = 50 };
    BYTE  m_byCount;
    DWORD m_dwObjectKey[_MAX_FIELDITEM_SIZE];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_dwObjectKey);
        return static_cast<uint16_t>(kMinSize + sizeof(m_dwObjectKey[0]) * m_byCount);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_OBJECT_DISAPPEAR_DEAD_BRD)
    DWORD m_dwObjectKey;
PACKET_CG_FWD_CLOSE;
//-------------------------------------------------------------------------------------------------
// ĳ���� �̵�
//-------------------------------------------------------------------------------------------------
#ifdef _NA_0_20111114_CHANGE_MOVE_PATH_COUNT
    enum { _MAX_PATHROUTE_SIZE = 512 };
#else
    enum { _MAX_PATHROUTE_SIZE = 64 };
#endif

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MOVE_SYN)
    PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(144), 1);
    WzVector wvCurPos;
    WzVector wvDestPos;
    PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T5(9));
        PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG32_T1(89), 1);
#ifdef _NA_0_20111114_CHANGE_MOVE_PATH_COUNT
        PACKET_RND_SHRBLK64_CONT(DWORD, m_byNumRoute); // ��� ���� Ÿ�� ����
#else
        PACKET_RND_SHRBLK64_CONT(BYTE, m_byNumRoute); // ��� ���� Ÿ�� ����
#endif
    PACKET_RND_SHRBLK64_END();

    TILEINDEX m_iRoute[ _MAX_PATHROUTE_SIZE ];        // ���

    uint16_t GetPacketSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_iRoute);
#ifdef _NA_0_20111114_CHANGE_MOVE_PATH_COUNT
        DWORD routes = m_byNumRoute;
#else
        uint8_t routes = (BYTE)m_byNumRoute;
#endif
        return static_cast<uint16_t>(kMinSize + sizeof(m_iRoute[0]) * routes);
    }

    void Encode();
    void Decode();
    void _Encode_T1();
    void _Encode_T2();
    void _Decode_T1();
    void _Decode_T2();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MOVE_NAK)
    WzVector m_wvCurPos;
    BYTE     m_byErrorCode; // eSYNC_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MOVE_BRD)
    DWORD    m_dwObjectKey;
    BYTE     m_byState;
    WzVector wvCurPos;
    WzVector wvDestPos;
    uint16_t      GetPacketSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MOVE_THRUST_BRD)
    DWORD    m_dwObjectKey;
    BYTE     m_byState;                              // ����(�ȱ�, �ٱ�)
    WzVector wvCurPos;
    WzVector wvDestPos;
    WORD     wTileIndex;
    uint16_t      GetPacketSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_TARGET_MOVE_SYN)
    DWORD     m_dwTargetKey;  
    WzVector  wvCurPos;
    WzVector  wvDestPos;
#ifdef _NA_0_20111114_CHANGE_MOVE_PATH_COUNT
    DWORD     m_byNumRoute;                           // ��� ���� Ÿ�� ����
#else
    BYTE      m_byNumRoute;                           // ��� ���� Ÿ�� ����
#endif
    TILEINDEX m_iRoute[ _MAX_PATHROUTE_SIZE ];        // ���
    //
    uint16_t GetPacketSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_iRoute);
        return static_cast<uint16_t>(kMinSize + sizeof(m_iRoute[0]) * m_byNumRoute);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_TARGET_MOVE_BRD)
    DWORD    m_dwTargetKey;
    WORD     m_wPlayerKey;
    WzVector wvCurPos;
    WzVector wvDestPos;
    uint16_t      GetPacketSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;



// Ű���� �̵�
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_KBMOVE_SYN)
    WzVector  wvCurPos;
    WORD      m_wAngle;
    TILEINDEX m_wTileIndex;
    BYTE      m_byMoveState;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    DWORD issued_serial;
    ulong client_test_result;
    int move_speed;
#endif
    uint16_t GetSize() const { return sizeof(*this); }

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_KBMOVE_BRD)
    WORD      m_wPlayerKey;
    WzVector  wvCurPos;
    TILEINDEX m_wTileIndex;
    WORD      m_wAngle;
    BYTE      m_byMoveState;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;



// ���� �����, ������ Ű���� �̵�
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FAST_KBMOVE_SYN)
    WzVector  m_wvCurPos;
    WORD      m_wAngle;
    TILEINDEX m_wTileIndex;
    BYTE      m_byCmsMoveState;       // eCHAR_MOVE_STATE
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    DWORD issued_serial;
    ulong client_test_result;
    int move_speed;

    void Encode();
    void Decode();
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FAST_KBMOVE_BRD)
    WORD      m_wPlayerKey;
    WzVector  m_wvCurPos;
    WORD      m_wAngle;
    TILEINDEX m_wTileIndex;
    BYTE      m_byCmsMoveState;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_SYN)
    WzVector                    m_wvDestPos;
    int                         m_iDir;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    DWORD issued_serial;
    ulong client_test_result;
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_STOP_SYN)
    WzVector                    m_wvDestPos;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    DWORD issued_serial;
    ulong client_test_result;
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_STOP_NAK)
    DWORD                       m_byErrorCode;          //< eSYNC_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_NAK)
    WzVector                    m_wvCurPos;
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_JUMP_BRD)
    PLAYERKEY                   m_PlayerKey;
    WzVector                    m_wvDestPos;
    int                         m_iDir;
PACKET_CG_FWD_CLOSE;

// NPC ���� ���
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_JUMP_BRD)
    bool     m_bJumpAttack;
    DWORD    m_dwObjectKey;
    WzVector m_wvEndPos;
PACKET_CG_FWD_CLOSE;

// ���� ���� ����
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_JUMPRESULT_BRD)
    DWORD    m_dwObjectKey;
    DWORD    m_dwTargetObjectKey;
    BYTE     m_byAttackType;
    WzVector m_wvCurPos;
    DAMAGETYPE m_wDamage;
    WORD m_wTargetHP;
    BYTE m_byEffect;             // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)
PACKET_CG_FWD_CLOSE;


// �÷��̾� ����
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_STOP_SYN)
    WzVector wvCurPos;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    DWORD issued_stop_serial;
    ulong client_test_result;
#endif
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_STOP_BRD)
    DWORD    dwObjectKey;
    WzVector wvCurPos;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// NPC ����
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_STOP_BRD)
    DWORD    dwObjectKey;
    WzVector wvCurPos;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// �÷��̾� �׼� ���� �˸�
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ACTION_EXPIRED_CMD)
PACKET_CG_FWD_CLOSE;


// �÷��̾� ���� �̵�
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FORCED_WARP_BRD)
    DWORD    dwObjectKey;
    WzVector wvWarpPos;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// ��Ű���� ����
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD)
    DWORD m_dwObjectKey;
    int   m_iRotateAngle;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_COMPOSED_STREAM_CMD)
    WAR_MSG_HDR m_Header[1];    // Stream Header, next pointer is data stream
    uint16_t GetSize() const
    { return static_cast<uint16_t>(sizeof(*this) + m_Header->size); }
PACKET_CG_FWD_CLOSE;

// NOTE: f110315.2L, _NA001955_110210_WING_COSTUME
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_WINGS_CONTROL_SYN)
    WingControl header;
    uint16_t GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_WINGS_CONTROL_CMD)
    WingControl header;
    WingControlData data;
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(data);
        return static_cast<uint16_t>(kMinSize + data.GetSize());
    }
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������
// CG_CRYPTOGRAPHY
//������������������������������������������������������������������������������

// <Cryptography>
// ��Ŷ ��ȣȭ Ű ��ȯ �� �ֱ��� �˰��� ��ȯ
PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_KEY)
    BYTE      m_bySN;         // Sequence Number
    BYTE      m_byCryptInfo;  // ���ڵ��� ��Ŷ ��ȣȭŰ ��� Ǯ�� ���� Ű
    union {
        DWORD m_dwUserKey;
        BYTE  m_byCryptData[200]; // �˰��� ���� (Ű, ���...)
    };
    BYTE&     GetSNKey() {    return m_bySN;  }
    void      SetSNKey(BYTE bySNKey) {    m_bySN = bySNKey;   }
    BYTE&     GetCryptInfo() {    return m_byCryptInfo;   }
    void      SetCryptInfo(BYTE byCryptInfo) {    m_byCryptInfo = byCryptInfo;    }
    BYTE*     GetCryptStream() {  return m_byCryptData;   }
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_SNK_KEY)
    DWORD                       m_dwSNKey;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_SNK_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_ACK)
    DWORD                       m_dwANSKEY;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_NAK)
PACKET_CG_CLOSE;

// ��ȣȭ�� ��Ŷ Ǯ������...
struct MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED {
    BYTE  m_bySN;         // Sequence Number : received
    BYTE  m_byCRC;
    BYTE  m_byStream[1];  // ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

    BYTE& GetSNKey() {    return m_bySN;  }
    void  SetSNKey(BYTE bySNKey) {    m_bySN = bySNKey;   }
    BYTE& GetCRC() {  return m_byCRC; }
    void  SetCRC(BYTE byCRC) {    m_byCRC = byCRC;    }
    BYTE* GetPacketStream() {     return m_byStream;  }
    DWORD GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// ��Ŷ ��ȣȭ �ϱ� ����...
struct MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING {
    BYTE  m_bySN;         // Sequence Number : sending
    BYTE  m_byCRC;
    BYTE  m_byStream[1];  // ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

    BYTE& GetSNKey() {    return m_bySN;  }
    void  SetSNKey(BYTE bySNKey) {    m_bySN = bySNKey;   }
    BYTE& GetCRC() {  return m_byCRC; }
    void  SetCRC(BYTE byCRC) {    m_byCRC = byCRC;    }
    BYTE* GetPacketStream() {     return m_byStream;  }
    DWORD GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

//������������������������������������������������������������������������������
//  CG_ARMORKIT ==> HACKSHIELD
//
// <Waverix><ArmorKit>
//////////////////////////////////
// int���� ���� ����... Ÿ���� ���������� ���ϴ�.
//
// Armorkit ���� ��Ŷ
struct MSG_CG_ARMORKIT_FINDHACK_SYN : public MSG_BASE
{
    MSG_CG_ARMORKIT_FINDHACK_SYN()
    {
        m_byCategory = CG_ARMORKIT;
        m_byProtocol = CG_ARMORKIT_FINDHACK;
    }

    int  HackType;       // Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
    int  Size;           // Buffer �� ����
    char Buf[1024];      // ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)

    int  GetSize() const { return sizeof(*this); }
};

#ifdef __ARMORKIT_ENABLED_CHECK_BY_CMDnACK__

PACKET_CG_OPEN(CG_ARMORKIT, CG_ARMORKIT_SEED_TEST_CMD)
    int     m_iSEED;
PACKET_CG_CLOSE;


PACKET_CG_OPEN(CG_ARMORKIT, CG_ARMORKIT_SEED_TEST_ACK)
    enum { eARMORKIT_AKS_KEYLENGTH = 32, };
    char m_pszGeneratedSeed[eARMORKIT_AKS_KEYLENGTH+1];
PACKET_CG_CLOSE;

#endif //__ARMORKIT_ENABLED_CHECK_BY_CMDnACK__

///! -----------------------------------------------------

#if USING_HACKSHIELD
//__NA000000_070913_HACKSHIELD_INSTALLATION__
PACKET_CG_OPEN(CG_ARMORKIT, CG_HACKSHIELD_SERVER_EX_QUERY_CMD)
    AHNHS_TRANS_BUFFER m_HShieldReqBlock;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ARMORKIT, CG_HACKSHIELD_SERVER_EX_QUERY_ACK)
    AHNHS_TRANS_BUFFER m_HShieldAnsBlock;
PACKET_CG_CLOSE;

#elif USING_XIGNCODE

PACKET_CG_OPEN(CG_ARMORKIT, CG_XIGNCODE_SERVER_QUERY_CMD)
    ulong reserve_tag;
    uint8_t buffer[XIGNCODE_GAME_PACKET_SIZE];
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ARMORKIT, CG_XIGNCODE_SERVER_QUERY_ACK,
                       MSG_CG_XIGNCODE_SERVER_QUERY_CMD)
PACKET_CG_CLOSE;

// dummy code
PACKET_CG_OPEN(CG_ARMORKIT, CG_HACKSHIELD_SERVER_EX_QUERY_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ARMORKIT, CG_HACKSHIELD_SERVER_EX_QUERY_ACK)
PACKET_CG_CLOSE;

#endif //

// USING_GAMEGUARD
PACKET_CG_OPEN(CG_ARMORKIT, CG_GAMEGUARD_SERVER_QUERY_CMD)
    BYTE buffer[GAMEGUARD_CSAUTH_PACKET_SIZE];  // ��������
    UINT32 query_size;     // �������� ������

    void clear()
    {
        ZeroMemory(buffer, sizeof(buffer));
        query_size = 0;
    }
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ARMORKIT, CG_GAMEGUARD_SERVER_QUERY_ACK, MSG_CG_GAMEGUARD_SERVER_QUERY_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ARMORKIT, CG_GAMEGUARD_HACKDATA_CMD)
    BYTE buffer[GAMEGUARD_HACKDATA_PACKET_SIZE];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ARMORKIT, CG_GAMEGUARD_SERVER_QUERY_NAK)
    UINT32 result_code;
PACKET_CG_CLOSE;

//������������������������������������������������������������������������������


//������������������������������������������������������������������������������
// CG_BATTLE
//������������������������������������������������������������������������������


// �÷��̾� ����
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_SYN)
    PACKET_RND_PAD_START(GETRNDNO_RNG04_T4(138));
        DWORD    dwClientSerial;         // Ŭ���̾�Ʈ���� ���� �ø����� ��ε� ĳ��Ʈ
    #ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        uint8_t byAttackType; // ���� Ÿ�� (1Ÿ, 2Ÿ, 3Ÿ), ref:eATTACK_SEQUENCE
        uint8_t byAttackPropensity; // �������� ����
        uint8_t style_presentation; // default(0), client animation presentation
    #else
        BYTE     byAttackPropensity  : 2;    // �������� ����
        BYTE     byAttackType    : 6;    // ���� Ÿ��
    #endif
        DWORD    dwTargetKey;            // ���� ����� ������Ʈ Ű
        WzVector wvCurPos;               // �������� ������ġ
        WzVector wvDestPos;
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        ulong client_test_result;
    #endif
    PACKET_RND_PAD_END();

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_BRD)
    DWORD           dwAttackerKey;          // �������� ������Ʈ Ű
    BYTE            byAttackType;           // ���� Ÿ��
    SLOTCODE        StyleCode;              // ��Ÿ�� �ڵ�
    DWORD           dwClientSerial;         // Ŭ���̾�Ʈ���� ���� �ø����� ��ε� ĳ��Ʈ
    WzVector        wvCurPos;               // �������� ������ġ
    DWORD           dwTargetKey ;           // ���� ����� ������Ʈ Ű
    DAMAGETYPE      wDamage;                // ���� ������
    DWORD           dwTargetHP;             // ���� �� Ÿ���� HP
    BYTE            byEffect;               // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)
    BYTE            byEtherComboCount;          // ���׸� źȯ ���� �� �޺�
    void    Init()
    {
        byEtherComboCount = 0;
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_NAK)
    BYTE            m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// �Ϲ� �������� �������
struct AREA_ATTACK_RESULT_INFO
{
    DWORD           m_dwTargetKey;          // ���� ���ϴ� ������Ʈ Ű
    DAMAGETYPE      m_wDamage;              // ���� ������
    DWORD           m_dwTargetHP;           // ���� �� Ÿ���� HP
    BYTE            m_byEffect;
    BYTE            m_byComboCount;         // ���׸� źȯ ���� �� �޺�
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd;              // ���� �� Ÿ���� SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
};

// �÷��̾� �Ϲ� ��������
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_AREA_ATTACK_BRD)
    DWORD           m_dwClientSerial;       // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD           m_dwAttackerKey;        // �������� ������Ʈ Ű
    DWORD           m_dwPrimaryTargetKey;   // �ְ��� ��� ������Ʈ Ű
    WzVector        m_wvCurPos;             // �������� ���� ��ǥ
    SLOTCODE        m_StyleCode;            // ��Ÿ�� �ڵ�
    BYTE            m_byAttackType;         // ���� Ÿ��
    BYTE            m_byNumberOfTargets;    // ���� ����� ����
    AREA_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * m_byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;


// ��Ű�� �⺻ ����
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_ATTACK_SYN)
    DWORD           dwTargetKey;            // ���� ����� ������Ʈ Ű
    DWORD           dwClientSerial;         // Ŭ���̾�Ʈ���� ���� �ø����� ��ε� ĳ��Ʈ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_ATTACK_BRD)
    DWORD           dwAttackerKey;          // �������� ������Ʈ Ű
    DWORD           dwClientSerial;         // Ŭ���̾�Ʈ���� ���� �ø����� ��ε� ĳ��Ʈ
    DWORD           dwTargetKey ;           // ���� ����� ������Ʈ Ű
    DAMAGETYPE      wDamage;                // ���� ������
    DWORD           dwTargetHP;             // ���� �� Ÿ���� HP
    BYTE            byEffect;               // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)
PACKET_CG_FWD_CLOSE;

// ��Ű�� ���� ������
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_RELOAD_SYN)   //������
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_RELOAD_BRD)   //������
    DWORD           m_dwObjKey;
PACKET_CG_FWD_CLOSE;


// ���� �⺻ ����
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_MONSTER_ATTACK_CMD)
    DWORD           m_dwObjectKey;
    DWORD           m_dwTargetObjectKey;
    BYTE            m_byAttackType;
    WzVector        m_wvCurPos;
    DAMAGETYPE      m_wDamage;
    WORD            m_wTargetHP;
    WORD            m_wTargetReserveHP;
    BYTE            m_byEffect;             // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)
PACKET_CG_FWD_CLOSE;

//// �÷��̾� ������Ʈ ����
//PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_P2O_ATTACK_SYN)
//  DWORD           dwTargetKey;            // ���� ����� ������Ʈ Ű
//PACKET_CG_FWD_CLOSE;
//
//PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_P2O_ATTACK_BRD)
//  DAMAGETYPE      m_wDamage;
//  DWORD           m_dwTargetHP;           // HP
//PACKET_CG_FWD_CLOSE;
//
//PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_P2O_ATTACK_NAK)
//PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_MONSTER_STATUS_RESIST_BRD)
    DWORD           m_dwMonsterObjKey;      // ���������� ������ ���� ObjKey
    BYTE            m_ScriptIndex;          // ��ũ��Ʈ���� �� ��° ���������� ����߳�(0, 1)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_CHARGE_ETHER_BULLET_BRD)
    DWORD           m_dwPlayerKey;
    BYTE            m_BulletIndex;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_REMOVE_ETHER_BULLET_BRD)
    DWORD           m_dwPlayerKey;  
PACKET_CG_FWD_CLOSE;

//_NA_008321_20150601_BATTLE_INFO
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_DETAIL_INFORMATION_CMD)
    DAMAGETYPE damage;
    SLOTCODE skill_code;

    CHAR attacker_name[MAX_CHARNAME_LENGTH+1];
    CHAR target_name[MAX_CHARNAME_LENGTH+1];
PACKET_CG_FWD_CLOSE;
//==================================================================================================
// CG_STYLE
//

// ��Ÿ�� ���� ��ε�
PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_PLAYER_ATTACK_BRD)
    DWORD           dwAttackerKey;              // �������� ������Ʈ Ű
    BYTE            byAttackType;               // ���� Ÿ��
    SLOTCODE        StyleCode;                  // Style Code
    DWORD           dwClientSerial;             // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD           dwPrimaryTargetKey;         // �ְ��� ��� ������Ʈ Ű
    WzVector        wvCurPos;
    uint16_t             GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// ��Ÿ�� ��� ��ε�
struct STYLE_ATTACK_RESULT_INFO
{
    DWORD           dwTargetKey;                // ���� ���ϴ� ������Ʈ Ű
    DAMAGETYPE      wDamage;                    // ���� ������
    DWORD           dwTargetHP;                 // ���� �� Ÿ���� HP
    WzVector        wvCurPos;
    WzVector        wvDestPos;
    BYTE            byEffect;
    BYTE            byEtherComboCount;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd; // ���� �� Ÿ���� SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
};

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_PLAYER_ATTACK_RESULT_BRD)
    DWORD           dwClientSerial;             // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD           dwAttackerKey;              // �������� ������Ʈ Ű
    BYTE            byNumberOfTargets;          // ���� ����� ����

    STYLE_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;


// ��Ÿ�� ����
PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_SYN)
    SLOTCODE                    m_NewStyleCode;     // ������ ��Ÿ�� �ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_BRD)
    DWORD                       m_dwObjectKey;      // ��Ÿ���� �ٲ� ĳ���� ������Ʈ Ű
    SLOTCODE                    m_CurStyleCode;     // ���� ��Ÿ��
    SLOTCODE                    m_NewStyleCode;     // ���� ������ ��Ÿ��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_NAK)
    SLOTCODE                    m_CurStyleCode;
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// ��Ÿ�� ���
PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINK_SYN)
    SLOTCODE                    m_StyleCode;
    POSTYPE                     m_ToPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINK_ACK)
    SLOTCODE                    m_StyleCode;
    POSTYPE                     m_ToPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINK_NAK)
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_UNLINK_SYN)
    POSTYPE                     m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_UNLINK_ACK)
    POSTYPE                     m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_UNLINK_NAK)
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINKMOVE_SYN)
    POSTYPE                     m_fromPos;
    POSTYPE                     m_toPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINKMOVE_ACK)
    POSTYPE                     m_fromPos;
    POSTYPE                     m_toPos;    
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_LINKMOVE_NAK)
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;


//==================================================================================================
// CG_SKILL
//
//-------------------------------------------------------------------------------------------------
// ��ų ����
//-------------------------------------------------------------------------------------------------

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_ACTION_SYN)
    DWORD           m_dwClientSerial;           // Skill �̺�Ʈ�� ���� ���� ��ȣ
    DWORD           m_dwMainTargetKey;          // �ְ��ݴ���� ������Ʈ Ű
    BYTE            m_byAttackPropensity;       // �������� ����
    SLOTCODE        m_SkillCode;                // Skill �ڵ�
    WzVector        m_wvCurPos;                 // Ŭ���̾�Ʈ�� �ִ� �÷��̾��� ���� ��ǥ
    WzVector        m_wvDestPos;                // �÷��̾��� ������ ��ǥ
    WzVector        m_wvMainTargetPos;          // �ְ��ݴ���� ��ġ    

    uint16_t GetSize() const { return sizeof(*this); };

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// ��ų �׼� ����(InstantResult, DelayStart, DelayResult)
//-------------------------------------------------------------------------------------------------

struct SKILL_ACTION_BASE : public MSG_BASE_FORWARD
{
	SLOTCODE		m_SkillCode;						// Skill �ڵ�
	DWORD			m_dwClientSerial;					// ���� �̺�Ʈ�� ���� ���� ��ȣ
	DWORD			m_dwAttackerKey;					// �������� ������Ʈ Ű
	DWORD			m_dwPrimaryTargetKey;				// �ְ��� ��� ������Ʈ Ű
	WzVector		m_wvMainTargetPos;					// �ְ��ݴ���� ��ġ
};

struct MSG_CG_SKILL_ACTION_INSTANT_RESULT_BRD : SKILL_ACTION_BASE
{
	WzVector		m_wvCurPos;							// �������� ���� ��ǥ
	WzVector		m_wvDestPos;						// �������� ������ ��ǥ
	DWORD			m_dwAttackerHP;						// ��ų�� ���� �� �������� HP
	DWORD			m_dwAttackerMP;						// ��ų�� ���� �� �������� MP
	BYTE			m_byNumberOfTargets			: 6;	// ���� ����� ����
	BYTE			m_byNumberOfFieldEffect		: 2;	// �ʵ忡 �ɸ��� ����Ʈ�� ���� ��� �� ����
	
	uint16_t GetSize() { return sizeof(*this); };
};

struct MSG_CG_SKILL_ACTION_DELAY_START_BRD : SKILL_ACTION_BASE
{
	WzVector		m_wvCurPos;							// �������� ���� ��ǥ
	WzVector		m_wvDestPos;						// �������� ������ ��ǥ
	
	uint16_t GetSize() { return sizeof(*this); };
};

struct MSG_CG_SKILL_ACTION_DELAY_RESULT_BRD : SKILL_ACTION_BASE
{
	DWORD			m_dwAttackerHP;						// ��ų�� ���� �� �������� HP
	DWORD			m_dwAttackerMP;						// ��ų�� ���� �� �������� MP
	BYTE			m_byNumberOfTargets			: 6;	// ���� ����� ����
	BYTE			m_byNumberOfFieldEffect		: 2;	// �ʵ忡 �ɸ��� ����Ʈ�� ���� ��� �� ����
	
	uint16_t GetSize() { return sizeof(*this); };
};


// Skill Casting Cancel  Syn
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_SYN )
	SLOTCODE		m_SkillCode;					// Skill �ڵ�
PACKET_CG_FWD_CLOSE;

// Skill Casting Cancel Ack(BRD)
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_BRD )
	DWORD			m_dwObjectKey;					// ��ų ������ ������ƮŰ
	SLOTCODE		m_SkillCode;					// Skill �ڵ�
PACKET_CG_FWD_CLOSE;

// Skill Casting Cancel NAK
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_NAK )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_ACTION_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// ��ź���� ����Ʈ ���
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_EFFECT_BOMB_BRD )
	DWORD				m_dwAttackerKey;
	SLOTCODE			m_SkillCode;
	BYTE				m_byNumberOfTargets;
	EFFECT_BOMB_INFO	m_DamageInfo[MAX_TARGET_NUM];
    //
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_SELF_DESTRUCTION_BRD )
	DWORD				m_dwAttackerKey;
	SLOTCODE			m_SkillCode;
	BYTE				m_byNumberOfTargets;
	DAMAGE_INFO			m_DamageInfo[MAX_TARGET_NUM];
    //
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_TRANSPARENT_BRD )
	DWORD				m_dwObjectKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_SUCTION_BRD )
	DWORD			m_dwAttackerKey;					// ����� �߻���Ų ������ Ű
	SLOTCODE		m_SkillCode;						// �ֱⵥ������ �߻���Ű�� ��ų�ڵ�
    DAMAGETYPE		m_wTargetDamage;					// ��� ������
	DWORD			m_dwTargetHP;						// Ÿ���� ���� HP
	DWORD			m_dwTargetMP;						// Ÿ���� ���� MP
	DWORD			m_dwAttackerHP;						// �������� ���� HP
	DWORD			m_dwAttackerMP;						// �������� ���� MP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_VITAL_SUCTION_BRD )
	DWORD			m_dwAttackerKey;					// ����� �߻���Ų ������ Ű
	DWORD			m_dwTargetKey;						// Ÿ���� Ű.
	DWORD			m_dwHP;								// Ÿ���� ���� HP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_EXECUTE_STATUS_CMD )
	DWORD			m_dwTargetKey;						// Ÿ���� Ű.
	DWORD			m_wStatusCode;						// �����ڵ�.
	WORD			m_iStatusTime;						// ���������ð�.
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// ��ų ��� ����
//-------------------------------------------------------------------------------------------------

struct SKILL_RESULT_BASE
{
	DWORD			m_dwTargetKey;						// ���� ���ϴ� ������Ʈ Ű
	BYTE			m_bySkillEffect			: 5;		// NPC�� ����� ��ų ȿ��
	BYTE			m_byAbilityNum			: 3;		// Ability ����(Ÿ���� ������ �͸� ������)
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BOOL            isDamaged;                          // 1��Ability�� ���ؼ� ���ظ� �Ծ����� ���θ� üũ
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	int GetSize() { return sizeof(SKILL_RESULT_BASE); };
};

struct EFFECT_INFO
{
	DWORD		m_Time;
	WzVector	m_wvEffectPos;
};

struct SKILL_RESULT_EFFECT
{
	WORD			m_wAbilityIndex;					// Ability Index
	BYTE			m_byCount;
	EFFECT_INFO		m_EffectInfo[MAX_EFFECT_COUNT];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_EffectInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_EffectInfo[0]) * m_byCount);
    }
};

struct SKILL_RESULT_CODE
{
	WORD			m_wAbilityIndex;					// Ability Index
	WORD			m_wStatusCode;
	int				m_iStatusTime;						// ���� �����ð�
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    WORD            overlap_status_count;               // ��ø�����ϰ��, ��øȽ��
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    uint16_t GetSize() const { return sizeof(*this); };
};

// eABILITY_DAMAGE
struct SKILL_RESULT_DAMAGE : SKILL_RESULT_CODE
{
    DAMAGETYPE		m_wDamage;							// ���� ������
	DWORD			m_dwTargetHP;						// ���� �� Ÿ���� HP
	BYTE			m_byEffect;							// ������ ����Ʈ(ũ��Ƽ�� ���)
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd; // ���� �� Ÿ���� SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_ABNORMAL_STATUS
struct SKILL_RESULT_STUN : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// ���ݴ���� ���� ��ǥ

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_KNOCKBACK
struct SKILL_RESULT_POSITION : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// ���ݴ���� ���� ��ǥ
	WzVector		m_wvDestPos;						// ���ݴ���� ������ ��ǥ

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_EXHAUST_HP, eABILITY_EXHAUST_MP
struct SKILL_RESULT_EXHAUST : SKILL_RESULT_CODE
{
	DWORD			m_dwTargetHP;						// ��ų ��� �� Ÿ���� ���� HP
	DWORD			m_dwTargetMP;						// ��ų ��� �� Ÿ���� ���� MP

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_FIGHTING_ENERGY_NUM_INCREASE, eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY
struct SKILL_RESULT_FIGHTING_ENERGY : SKILL_RESULT_CODE
{
	SHORT			m_sFightingEnergyCount;				// ���� ���� ����

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_RESURRECTION
struct SKILL_RESULT_RESURRECTION : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// ��Ȱ����� ���� ��ǥ	
	DWORD			m_dwTargetHP;						// ��Ȱ����� HP
	DWORD			m_dwTargetMP;						// ��Ȱ����� MP

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_MONSTER_TRANSFORMATION
struct SKILL_RESULT_MONSTER_TRANSFORMATION : SKILL_RESULT_CODE
{
	DWORD			m_dwHP;
	DWORD			m_dwMaxHP;
	WORD			m_wMoveSpeedRatio;		// �̵��ӵ�(100�� ���� %��ġ)
	WORD			m_wAttSpeedRatio;		// ���ݼӵ�(100�� ���� %��ġ)

    uint16_t GetSize() const  { return sizeof(*this); };
};

struct SKILL_RESULT_SELF_DESTRUCTION : SKILL_RESULT_CODE
{
	DWORD			m_dwHP;
	DWORD			m_dwTargetHP;

    uint16_t GetSize() const  { return sizeof(*this); };
};

//-------------------------------------------------------------------------------------------------
// �ֱ� ������, �ݻ� ������, ��Ÿ
//-------------------------------------------------------------------------------------------------


PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PERIODIC_DAMAGE_BRD )
	DWORD			m_dwAttackerKey;					// �ֱⵥ������ �߻���Ų ������ Ű
	SLOTCODE		m_SkillCode;						// �ֱⵥ������ �߻���Ű�� ��ų�ڵ�
	BYTE			m_byNumberOfTargets;				// ���� ����� ����
	DAMAGE_INFO		m_DamageInfo[MAX_TARGET_NUM];		// ���� ����� ������ ����

	int				GetSize()
	{
		return sizeof(*this) - sizeof(DAMAGE_INFO) * (MAX_TARGET_NUM - m_byNumberOfTargets);
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_REFLECT_DAMAGE_BRD )
	DWORD			m_dwAttackerKey;					// �ֱⵥ������ �߻���Ų ������ Ű
	DWORD			m_dwTargetKey;						// ���� ���ϴ� ������Ʈ Ű
	SLOTCODE		m_SkillCode;						// �ǰ� ��ų�ڵ�
	WORD			m_wAbilityCode;						// �ǰ� �����Ƽ �ڵ�
    DAMAGETYPE      m_wDamage;                          // ���� ������
	DWORD			m_dwTargetHP;						// ���� �� Ÿ���� HP
	DWORD			m_dwTargetResrveHP;					// ������ Ÿ���� ������ HP
	int GetSize() { return sizeof(*this); };
PACKET_CG_FWD_CLOSE;


//-------------------------------------------------------------------------------------------------
// ��ų ����Ʈ ��
//-------------------------------------------------------------------------------------------------
    //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_SELECT_SKILLPOINT_SYN )
        enum { max_skill_count = MAX_SKILL_COUNT};
        uint        skill_count;
        SLOTCODE	skill_code_array[max_skill_count];		// SkillCode, StyleCode
        uint16_t GetSize() const
        {
            const size_t kMinSize = sizeof(*this) - sizeof(skill_code_array);
            return static_cast<uint16_t>(kMinSize + sizeof(skill_code_array[0]) * skill_count);
        }
    PACKET_CG_FWD_CLOSE;

    PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_SELECT_SKILLPOINT_ACK )
        int remain_skill_point;
        SKILL_TOTAL_INFO skill_total_info;                       // ���� ��ϵǴ� ��ų���� ����
        int GetSize()
        {
            return ( sizeof(*this) - (SKILL_TOTAL_INFO::MAX_SLOT_NUM-skill_total_info.m_Count)*sizeof(SKILLSLOT) );
        }
    PACKET_CG_FWD_CLOSE;
    PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_SELECT_SKILLPOINT_NAK )
        uint    error_code;
    PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_REFRESH_CMD )
SKILL_TOTAL_INFO m_SkillInfo;
int GetSize()
{
	return ( sizeof(*this) - (SKILL_TOTAL_INFO::MAX_SLOT_NUM-m_SkillInfo.m_Count)*sizeof(SKILLSLOT) );
}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_POINT_REMAIN_CMD )	
	INT						m_iRemainSkill;
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// �нú� ��ų ����Ʈ ��ε�
//-------------------------------------------------------------------------------------------------
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PASSIVE_SKILL_LIST_CMD )
	DWORD						m_dwObjectKey;					// ��ų ������ ������ƮŰ
	BYTE						m_byCount;
	SLOTCODE					m_SkillCode[MAX_SKILL_SLOT_NUM];
	int GetSize() 
	{
		return (sizeof(*this)-(MAX_SKILL_SLOT_NUM-m_byCount)*sizeof(SLOTCODE));
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PASSIVE_SKILL_ADD_CMD )
	DWORD						m_dwObjectKey;					// ��ų ������
	SLOTCODE					m_OldSkillCode;					// ���� ��ų �ڵ�(ó������ �����ϴ� ���� 0�̴�)
	SLOTCODE					m_NewSkillCode;					// ����� Ȥ�� ������ ��ų �ڵ�
PACKET_CG_FWD_CLOSE;


//������������������������������������������������������������������������������
// CG_CONVERSATION
//������������������������������������������������������������������������������

struct MSG_CG_CONVERSATION_FIELDCHAT_SYN : public MSG_BASE_FORWARD
{
	enum { _MAX_CHATMSG_SIZE = 255 };
	DWORD						m_dwObjectKey;
	BYTE						m_byMsgLength;
	char						m_pszChatMsg[ _MAX_CHATMSG_SIZE+1 ];
	int							GetSize()
	{
								return sizeof(*this) - (_MAX_CHATMSG_SIZE - m_byMsgLength);
	}
};
struct MSG_CG_CONVERSATION_FIELDCHAT_BRD : public MSG_CG_CONVERSATION_FIELDCHAT_SYN	{};

// ������������������������������������������������������������������������������
// ���� �� �׽�Ʈ �Ҷ� ����ϴ� ��Ŷ ����!!!!!
// ������������������������������������������������������������������������������


// �̼� �κ� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_MISSIONLOBBY_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_MISSIONLOBBY_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	MISSION_ROOMINFO	m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(MISSION_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_MISSIONLOBBY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// ���� �κ� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTINGLOBBY_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTINGLOBBY_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	HUNTING_ROOMINFO	m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(HUNTING_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTINGLOBBY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// PVP �κ� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_PVPLOBBY_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_PVPLOBBY_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	PVP_ROOMINFO		m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(PVP_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_PVPLOBBY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// �������� �κ� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTINGLOBBY_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTINGLOBBY_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	CHUNTING_ROOMINFO	m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(CHUNTING_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTINGLOBBY_NAK )
	DWORD				m_byErrorCode;
PACKET_CG_CLOSE;

// ���������� �������� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTING_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTING_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	CHUNTING_ROOMINFO	m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(CHUNTING_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_CHUNTING_NAK )
	DWORD				m_byErrorCode;
PACKET_CG_CLOSE;


// ����� ���� �� ����Ʈ ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTING_SYN )
	BYTE				m_Count;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTING_ACK )
	enum { _MAX_ROOM_NUM = 40, };
	BYTE				m_Count;
	HUNTING_ROOMINFO	m_RoomInfo[_MAX_ROOM_NUM];	
	int GetSize()
	{
		return sizeof(*this) - (_MAX_ROOM_NUM-m_Count)*sizeof(HUNTING_ROOMINFO);
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_HUNTING_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// Ư�� �� ����Ʈ ���ΰ�ħ
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_SYN )
	KEYTYPE				m_RoomKey;								//< ���ΰ�ħ�� ���� Ű
	BYTE				m_byRoomType;							//< eZONETYPE
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_HUNTINGLOBBY_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_MISSIONLOBBY_ACK )
	MISSION_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_PVPLOBBY_ACK )
	PVP_ROOMINFO		m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_CHUNTINGLOBBY_ACK )
	CHUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_HUNTING_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_CHUNTING_ACK )
	CHUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_NONE_ACK )
	KEYTYPE				m_RoomKey;								//< �������� ����. �ı��Ǿ���.
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// �ٷΰ���
// ��й��� �ٷΰ������ �ʴ´�.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_SYN )
	BYTE				m_byRoomType;		//< eZONETYPE �� �� eZONETYPE_HUNTING or eZONETYPE_MISSION�� ���� �ϳ�
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_HUNTINGLOBBY_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK )
	MISSION_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_PVPLOBBY_ACK )
	PVP_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_CHUNTINGLOBBY_ACK )
	CHUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_HUNTING_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;


// ���̵�� �� ã��
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_FIND_ROOM_FROM_CHARID_SYN )
	char				m_pszCharName[MAX_CHARNAME_LENGTH];
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK )
	MISSION_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK )
	PVP_ROOMINFO		m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK )
	CHUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK )
	HUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_ROOM_FROM_CHARID_ACK )
	CHUNTING_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_FIND_ROOM_FROM_CHARID_NAK )
	DWORD				m_dwErrorCode;						//< return 5601 : ���ӵǾ� ���� �ʰų�, ������ ���� ������
PACKET_CG_CLOSE;

// ������ �κ�(����or�̼�orPVP)���� �����
PACKET_CG_OPEN(CG_ZONE, CG_ZONE_LOBBY_CREATE_SYN)
    MAPCODE m_MapCode;
    BYTE    m_byMaxLimitUserNum;
    BYTE    m_byRoomType            : 6;            //< eZONETYPE �� �� eZONETYPE_HUNTING or eZONETYPE_MISSION�� ���� �ϳ�
    BYTE    m_byRoomPublic          : 2;            //< eZONEPUBLIC
    char    m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
    BASE_ROOMINFO   m_RoomInfo;

    char m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];

    int  GetSize() const
    {// m_byRoomPublicüũ���϶�(Ŭ���̾�Ʈ�ŷ��Ҽ�����.)
        return (m_byRoomPublic == eZONEPUBLIC_PUBLIC) ?
                   sizeof(*this) - sizeof(char) * MAX_ROOMPASSWORD_LENGTH :
                   sizeof(MSG_CG_ZONE_LOBBY_CREATE_SYN);
    }

    void Encode();
    void Decode();
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CREATE_ACK )
	KEYTYPE			m_LobbyKey;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN )
	char			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_LOBBY_NAME_CHANGE_ACK )
	char			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_LOBBY_NAME_CHANGE_NAK )
	DWORD			m_dwErrorCode;							//< eROOM_RESULT
PACKET_CG_CLOSE;

// �κ� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_SYN )
	KEYTYPE			m_LobbyKey;
	BYTE			m_byRoomPublic;		//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{// m_byRoomPublicüũ���϶�(Ŭ���̾�Ʈ�ŷ��Ҽ�����.)
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_LOBBY_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_ACK )
	DWORD			m_dwMasterKey;					//< ���� �÷��̾� Ű
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �������� ���ηκ� ����
PACKET_CG_OPEN(CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CREATE_SYN)
    KEYTYPE         m_LinkHuntingKey;                       //< ��ũ��ų �������ù� Ű
    BYTE            m_byRoomPublic;                         //< eZONEPUBLIC
    char            m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
    BASE_ROOMINFO   m_RoomInfo;                             //< ������ �� ����
    char            m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
    int             GetSize() const
    {
        return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN));
    }

    void Encode();
    void Decode();
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CREATE_ACK )
	KEYTYPE					m_LobbyKey;
	SLOTCODE				m_MapCode;
	ADDITIONAL_CHUNTINGINFO m_AdditionalCHuntingInfo;
	BYTE					m_NonBlockNum;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CREATE_NAK )
	DWORD			m_byErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MASTER_CHANGED_BRD )
	DWORD			m_dwMasterPlayerKey;
PACKET_CG_CLOSE;

// ���� ��ư�� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_BRD )
	DWORD			m_PlayerKey;							//< ���� ���� �÷��̾�
PACKET_CG_CLOSE;

// ���� ��ư�� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_BRD )
	DWORD			m_PlayerKey;							//< ������Ҹ� ���� �÷��̾�
PACKET_CG_CLOSE;

// �������� ����� ������ ���� ���� ��û
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_REQUEST_INFO_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_REQUEST_INFO_ACK )
	SLOTCODE			m_MapCode;
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
	BYTE				m_NonBlockNum	:4;
	BYTE				m_Count			:4;
	DWORD				m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM];	//< �ڱ��ڽ� �� ���� + ���� �� ����
	int					GetSize()
	{
		return (sizeof(*this) - sizeof(DWORD)*(MAX_PARTYMEMBER_NUM-m_Count));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK )
    SLOTCODE m_MapCode;
    BYTE     m_NonBlockNum;
    BYTE     m_Count;
    uint16_t m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM]; //< �ڱ��ڽ� �� ���� + ���� �� ����
    // CHANGES: f110608.6L
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_dwReadyPlayerKey);
        return static_cast<uint16_t>(kMinSize + sizeof(m_dwReadyPlayerKey[0]) * m_Count);
    };
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK )
	SLOTCODE			m_MapCode;
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;
	BYTE				m_NonBlockNum	:4;
	BYTE				m_Count			:4;
	PVP_LOBBY_INFO		m_PlayerInfo[MAX_PARTYMEMBER_NUM];			//< �ڱ��ڽ� ���� + ��������
    // CHANGES: f110608.6L
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_PlayerInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_PlayerInfo[0]) * m_Count);
    };
PACKET_CG_CLOSE;
    
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK )
	SLOTCODE				m_MapCode;
	ADDITIONAL_CHUNTINGINFO m_AdditionalCHuntingInfo;
    BYTE     m_NonBlockNum;
    BYTE     m_Count;
    uint16_t m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM]; //< �ڱ��ڽ� �� ���� + ���� �� ����
    // CHANGES: f110608.6L
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_dwReadyPlayerKey);
        return static_cast<uint16_t>(kMinSize + sizeof(m_dwReadyPlayerKey[0]) * m_Count);
    };
PACKET_CG_CLOSE;
    
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_REQUEST_INFO_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �ο��� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NUMBEROFPLAYER_SYN )
	BYTE m_NumberOfPlayers;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NUMBEROFPLAYER_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NUMBEROFPLAYER_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NUMBEROFPLAYER_BRD )
	BYTE m_NumberOfPlayers;
PACKET_CG_CLOSE;


// �ο��� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN )
	BYTE m_NonBlockSlotNum;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NONBLOCKSLOTNUM_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_NONBLOCKSLOTNUM_BRD )
	BYTE m_NonBlockSlotNum;
PACKET_CG_CLOSE;

// �÷��̾� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_SYN )
	DWORD			m_PlayerKey;							//< ������ �÷��̾� Ű
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �÷��̾� �� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_SYN )
	BYTE			m_Team;									//< ������ �� (1or2)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_BRD )
	DWORD			m_dwPlayerKey;							//< ������ �÷��̾� Ű
	BYTE			m_Team;									//< ������ �÷��̾��� ��
PACKET_CG_CLOSE;


// �� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_MAP_SYN )
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_MAP_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_MAP_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_MAP_BRD )
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;


// �κ��� ���� ���� ���� �ٲ�( ���̵�, ���ʽ�)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_SYN )
	ADDITIONAL_ROOMINFO m_AdditionalInfo;			//< ������ ������
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_BRD )
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;			//< ����� ������
PACKET_CG_CLOSE;


// �κ��� PVP ���� ���� �ٲ�( ��, ���)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_SYN )
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;			//< ������ ������
PACKET_CG_CLOSE;

//PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_ACK )
//PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_BRD )
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;
	BYTE				m_Count;
	PVP_LOBBY_INFO		m_PlayerInfo[MAX_PARTYMEMBER_NUM];			//< �ڱ��ڽ� ���� + ��������
    // CHANGES: f110608.6L
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_PlayerInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_PlayerInfo[0]) * m_Count);
    };
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_INFO_BRD )
	// ���� ����� ����
	DWORD			m_dwPlayerKey;
	BYTE			m_Team;
PACKET_CG_CLOSE;

// �κ��� ���� ���� ���� �ٲ�( ���̵�, ���ʽ�)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN )
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;			//< ������ ������
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_NAK )
	DWORD					m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD )
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;			//< ����� ������
PACKET_CG_CLOSE;

// ���� ���� ( START�� ���� )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_CREATE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �̼� ���� ( START�� ���� )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_CREATE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T4(229), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// PVP ���� ( START�� ���� )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_CREATE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �������� ���� ( START�� ���� )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_CREATE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(229), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_CREATE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// �������� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_MIGRATE_SYN )
	KEYTYPE			m_CHuntingKey;							//< ������ ���� Ű
	BYTE			m_byRoomPublic;							//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_CHUNTING_MIGRATE_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_JOIN_ACK )
	DWORD			m_dwMasterObjKey;
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_JOIN_NAK )
	DWORD			m_byErrorCode;
PACKET_CG_CLOSE;

//-------------------------------------------------------------------------------------------------
// ������ ��û
//-------------------------------------------------------------------------------------------------
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTINGLOBBY_JOIN_CMD )
	bool				m_bRequireConfirm;		// ���� ���Ǹ� �޾ƾ� �ϴ°�
	HUNTING_ROOMINFO	m_RoomInfo;
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_RoomInfo.m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTINGLOBBY_JOIN_CMD));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSIONLOBBY_JOIN_CMD )
	bool				m_bRequireConfirm;
	MISSION_ROOMINFO	m_RoomInfo;
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_RoomInfo.m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_MISSIONLOBBY_JOIN_CMD));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVPLOBBY_JOIN_CMD )
	bool				m_bRequireConfirm;
	PVP_ROOMINFO		m_RoomInfo;
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_RoomInfo.m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_PVPLOBBY_JOIN_CMD));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_JOIN_CMD )
	bool				m_bRequireConfirm;
	CHUNTING_ROOMINFO	m_RoomInfo;
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_RoomInfo.m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_CHUNTINGLOBBY_JOIN_CMD));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_CMD )
	bool				m_bRequireConfirm;
	HUNTING_ROOMINFO	m_RoomInfo;
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_RoomInfo.m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_JOIN_CMD));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_JOIN_CMD )
	bool				m_bRequireConfirm;
	MISSION_ROOMINFO	m_RoomInfo;
PACKET_CG_CLOSE;


// ���� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_SYN )
	KEYTYPE			m_HuntingKey;							//< ������ ���� Ű
	BYTE			m_byRoomPublic;							//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_ACK )
	KEYTYPE			m_HuntingKey;							//< ������ ���� Ű
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// �̼� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_JOIN_ACK )
	KEYTYPE			m_MissionKey;							//< ������ �̼� Ű
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// PVP ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_JOIN_ACK )
	KEYTYPE			m_PVPKey;							//< ������ �̼� Ű
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_LEAVE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_LEAVE_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_LEAVE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_LEAVE_SYN )
	enum {
		REASON_INTEND,			// ������ �̼� Ż��
		REASON_EXPULSION,		// �߹�
	};
	BYTE			m_Reason;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_LEAVE_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_LEAVE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_LEAVE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_LEAVE_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_LEAVE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_LEAVE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_LEAVE_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_LEAVE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_LEAVE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_LEAVE_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_LEAVE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;


// ������ ���� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_CREATE_SYN )
	MAPCODE				m_MapCode;			//< ������ �� ����
	BASE_ROOMINFO		m_RoomInfo;			//< ������ �� ����
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;	//< ������ �ΰ� ����
	BYTE				m_byRoomPublic;							//< eZONEPUBLIC
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_RELAY_CREATE_SYN));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_CREATE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_CREATE_BRD )
	KEYTYPE				m_HuntingKey;
	CODETYPE			m_MapCode;			//< ������ �� ����
	BASE_ROOMINFO		m_RoomInfo;			//< ������ �� ����
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;	//< ������ �ΰ� ����
	BYTE				m_byRoomPublic;							//< eZONEPUBLIC
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_RELAY_CREATE_BRD));
	}
PACKET_CG_CLOSE;


// ������ ���� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_SYN )
	KEYTYPE			m_HuntingKey;							//< ������ ���� Ű
	BYTE			m_byRoomPublic;							//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_RELAY_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_ACK )
//	KEYTYPE			m_HuntingKey;							//< ������ ���� Ű
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// ��Ż�� �̿��� �ʵ尣 �̵�.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_SYN )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG32_T3(996));
	PACKET_RND_SHRBLK64_CONT(WORD, m_wIndex);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_ACK )
	WORD			m_wIndex;							// �̿��Ϸ��� ��Ż idx
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// �׾����� �ʵ忡�� ������ �̵�.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T2(44), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_ACK )
	WORD			m_wMapCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// ���÷κ� �ִ������� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD )
	BYTE			m_byMaxLimitUser;
PACKET_CG_CLOSE;

// GM ��ɿ� ���� �ʵ尣 �̵�
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_GMPORTAL_FIELD_MOVE_CMD )
	CODETYPE		m_MapCode;
PACKET_CG_CLOSE;


// ������ �̼� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_BRD )
	KEYTYPE			m_LobbyKey;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_NAK )
	BYTE			m_ErrorCode;
PACKET_CG_CLOSE;


// ������ �̼� ����
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_JOIN_SYN )
	KEYTYPE			m_LobbyKey;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_JOIN_ACK )
	KEYTYPE			m_LobbyKey;
	DWORD			m_dwMasterKey;
PACKET_CG_CLOSE;

// ���� SpecialType�� �ٲ�
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHANGE_SPECIALTYPE_CMD )
	BYTE			m_SpecialType;		// eROOM_SPECIAL_TYPE
PACKET_CG_CLOSE;

// [11/18/2009 namka199] _NA_0_20091118_GAMEROOM_BATTLE_RESULT ���ӹ��� ���� ��� �뺸
PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_PLAYER_BATTLE_RESULT_SYN )
    BYTE    m_OwnerGuild; // 0 : �ڽ��� ��� �׿� ��� ���
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_PLAYER_BATTLE_RESULT_NAK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_INSERT_BATTLE_PLAYER_CMD )
    ZONE_INSERT_BATTLE_PLAYER_INFO m_ZonePlayers;
    int GetSize() 
    {
        return ( sizeof(*this)-(ZONE_INSERT_BATTLE_PLAYER_INFO::MAX_PLAYER-m_ZonePlayers.m_PlayerCount)*sizeof(GAMEROOM_BATTLE_ENTER_INFO) );
    }
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_REMOVE_BATTLE_PLAYER_CMD )
    ZONE_REMOVE_BATTLE_PLAYER_INFO m_ZonePlayers;
    int GetSize()
    {
        return ( sizeof(*this)-(ZONE_REMOVE_BATTLE_PLAYER_INFO::MAX_PLAYER-m_ZonePlayers.m_PlayerCount)*sizeof(GAMEROOM_BATTLE_LEAVE_INFO) );
    }
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_PLAYER_BATTLE_RESULT_ACK )
    ZONE_UPDATE_BATTLE_PLAYER_INFO m_ZonePlayers;
    int GetSize() 
    {
        return ( sizeof(*this)-(ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER-m_ZonePlayers.m_PlayerCount)*sizeof(GAMEROOM_BATTLE_UPDATE_INFO) );
    }
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_CHUNTING_BATTLE_INFO_CMD )
    BYTE    m_HomeTeamWinCount;
    BYTE    m_AwayTeamWinCount;
PACKET_CG_CLOSE;

// �� ��ȣ�� �� ã��
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_FIND_ROOM_FROM_NO_SYN )
	WORD			m_RoomNo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD )
	MAPCODE			m_MapCode;		// Ŭ���� �������� ������ �����Ų��. (ĳ���� ���� ������...)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_ROOM_JOIN_ACK )
	KEYTYPE			m_ktROOMKEY;				//< ������ �� Ű
	MAPCODE			m_MapCode;					//< ������ �� ���ڵ�
	// ���� JOIN�迭�� ���� ��ġ�� ���߱� ���� �۾�
	FIELDCODE		m_fcCODE;					//< ������ �� �ʵ��ڵ� (��Ƽ�� ��ġ�� �ʵ�)
	BYTE			m_byZONETYPE;				//< ������ �� Ÿ��
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN )
#if defined(_CHINA) || defined(_JAPAN)	//	(WAVERIX)(080818)(FAKE-CODE) ���߿� �����ϰ���. �Ф�
	#define __EXTRA_OP	(1)
#else
	#define __EXTRA_OP	(0)
#endif
	BYTE				__dummy1[(PROTOCOL_GENERATOR_ZONE_EX_077)%4 + __EXTRA_OP];
#undef __EXTRA_OP
	POSTYPE				m_ItemPos;	// ����ϰ��� �ϴ� ������... (�������� �� �� �ֱ�� �ϰ�����, ���ʿ��� �����̳� ���� ���ó�� ������)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK )
	enum eRESULT { FAIL = PROTOCOL_GENERATOR_ZONE_EX_089, SUCCESS = PROTOCOL_GENERATOR_ZONE_EX_091 };
	enum eFAIL_RC_CODE { RET_ITEM = PROTOCOL_GENERATOR_ZONE_EX_014, RET_ROOM = PROTOCOL_GENERATOR_ZONE_EX_016 };
	BYTE				m_Result;	// eRESULT
	union {
		struct {
			BYTE		m_byKind;	// eFAIL_RC_CODE { RET_ITEM �̸� eITEM_RESULT, RET_ROOM�̸� eROOM_RESULT }
			DWORD		m_dwErrorCode;
		} RETINFO_FAIL;				//

		struct {
			MAPCODE		m_MapCode;
		} RETINFO_SUCCESS;			//
	};

	inline VOID			SetFailResult( BYTE kind, DWORD errorCode )
	{
		m_Result = FAIL;
		RETINFO_FAIL.m_byKind = kind; RETINFO_FAIL.m_dwErrorCode = errorCode;
	}
	inline VOID			SetSuccessResult( MAPCODE mapCode )
	{
		m_Result = SUCCESS;
		RETINFO_SUCCESS.m_MapCode = mapCode;
	}

	// for Client ���� ���� �Ǵܿ�
	inline BOOL			IsSuccess() { return SUCCESS == m_Result; }
	inline WORD			GetSize()
	{
		const WORD DEF_SIZE = sizeof(MSG_BASE_FORWARD) + sizeof(BYTE);
		return SUCCESS == m_Result ? DEF_SIZE + sizeof(RETINFO_SUCCESS) : DEF_SIZE + sizeof(RETINFO_FAIL);
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_NAK )
PACKET_CG_CLOSE;

//_NA_008069_20150210_ADD_SPA_SYSTEM
PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_ENTER_SYN)
    CODETYPE item_code;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_ENTER_ACK)
    MAPCODE spa_mapcode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_ENTER_NAK)
    BYTE error_code;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_LEAVE_SYN)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_LEAVE_ACK)
    MAPCODE return_map_code;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SPA_LEAVE_NAK)
    BYTE error_code;
PACKET_CG_CLOSE;

//_NA_008334_20150608_SONNENSCHEIN
// SONNENSCHEIN PACKET
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD)
    DWORD remain_time_; //�и�������
    BYTE event_state_;
PACKET_CG_CLOSE;
// ������������������������������������������������������������������������������
// ���� �� �׽�Ʈ �Ҷ� ����ϴ� ��Ŷ ��!!!!
// ������������������������������������������������������������������������������


// CG_ITEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_SYN  )
	SLOTIDX						m_fromIndex;
	SLOTIDX						m_toIndex;
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
    DURATYPE move_quantity_; // �̵� ����, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_ACK  )
	SLOTIDX						m_fromIndex;
	SLOTIDX						m_toIndex;
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
    DURATYPE move_quantity_; // �̵� ����, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_NAK  )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKITEM_SYN  )
	//SLOTIDX					m_OrgSlotIdx;
	POSTYPE						m_OrgPos;
	POSTYPE						m_ToPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKITEM_ACK  )
	//SLOTIDX					m_OrgSlotIdx;
	POSTYPE						m_OrgPos;
	POSTYPE						m_ToPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKITEM_NAK  )
	DWORD						m_dwErrorCode;	
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKSKILL_SYN  )
	SLOTCODE					m_SkillCode;
	POSTYPE						m_ToPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKSKILL_ACK  )
	SLOTCODE					m_SkillCode;
	POSTYPE						m_ToPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_LINKSKILL_NAK  )
	DWORD						m_dwErrorCode;	
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_UNLINK_SYN  )
	POSTYPE						m_atPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_UNLINK_ACK  )
	POSTYPE						m_atPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_UNLINK_NAK  )
	DWORD						m_dwErrorCode;	
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_MOVE_SYN  )
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_MOVE_ACK  )
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_QUICK_MOVE_NAK  )
	DWORD						m_dwErrorCode;	
PACKET_CG_FWD_CLOSE;

// �������� ����� ������ ���� ���� (Ŭ���̾�Ʈ -> ����)
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_QUICK_UPDATE_LINKED_ITEM_CMD)
    typedef WORD MessageSize;
    POSTYPE target_quick_slot_position; // ������ ������ ��ġ
    SLOTIDX new_linked_item_container_type; // SI_INVENTORY, SI_EQUIPMENT
    POSTYPE new_linked_item_position; // ������ ������ ��ġ
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_SYN  )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T4(12));
	PACKET_RND_SHRBLK64_CONT(DWORD, m_dwFieldItemObjectKey);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_PICK_ACK)
    INVENTORY_TOTAL_INFO m_ItemInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_NAK  )
	DWORD						m_dwErrorCode;
	DWORD						m_dwFieldItemObjectKey;			//< ������ ������ ObjectKey
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_BRD  )
	DWORD						m_dwObjKey;
	SLOTCODE					m_ItemCode;
PACKET_CG_FWD_CLOSE;

struct MSG_CG_ITEM_DROP_SYN : public MSG_BASE_FORWARD
{
	SLOTIDX						m_atIndex;
	POSTYPE						m_atPos;
};

struct MSG_CG_ITEM_DROP_ACK : public MSG_CG_ITEM_DROP_SYN
{};

struct MSG_CG_ITEM_DROP_NAK : public MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};

struct MSG_CG_ITEM_COMBINE_SYN : public MSG_BASE_FORWARD
{
	SLOTIDX						m_fromIndex;
	SLOTIDX						m_toIndex;	
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
    DURATYPE combine_quantity_; // ��ĥ ����, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
};

struct MSG_CG_ITEM_COMBINE_ACK : public MSG_CG_ITEM_COMBINE_SYN
{};

struct MSG_CG_ITEM_COMBINE_NAK : public MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DIVIDE_SYN  )
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
	DURATYPE					m_fromNum;
	DURATYPE					m_toNum;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DIVIDE_ACK  )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DIVIDE_NAK  )
	BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_SYN  )
	SLOTIDX						m_atIndex;
	POSTYPE						m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_ACK  )
	SLOTIDX						m_atIndex;
	POSTYPE						m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_NAK  )
	BYTE						m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_BRD  )
	DWORD						m_dwObjKey;
	SLOTCODE					m_ItemCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_CMD  )	
	POSTYPE						m_ItemPos;	
PACKET_CG_FWD_CLOSE;

//������ ���� ��ǰ� Ÿ���� �ִ°�� ��� ���� 
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_SYN  )
SLOTIDX						m_atIndex;
POSTYPE						m_atPos;
WzVector					m_wvMainTargetPos;			// �ְ��� ��ġ	
PACKET_CG_FWD_CLOSE;

//������ ��� ������ �˸���.
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_BRD  )
DWORD						m_dwAttackerKey;
CODETYPE					m_ItemCode;
WzVector					m_wvMainTargetPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_NAK  )
BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//������ ���� Ÿ���� �ִ°��(�ڱ��ڽ��� �ƴѰ��).
PACKET_CG_INHERIT_OPEN( CG_ITEM, CG_ITEM_USE_SKILL_ACTION_SYN, MSG_CG_ITEM_USE_SYN )
WzVector		m_wvMainTargetPos;			// �ְ��� ��ġ	
BYTE			byAttackPropensity;			// �������� ����
CODETYPE target_code_; // ������ ��� ����� �ڵ�, _NA_000587_20100928_DOMINATION_BUFF_ITEM
int GetSize() { return sizeof(*this); };

PACKET_CG_INHERIT_CLOSE;


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
// �Ⱓ ���� ������ ���
// �ش� �������� ����� �Ⱓ���� ����/������(����,����) ��� �κ��丮�� ��ġ�ؾ��Ѵ�.
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_EXTEND_DATE_SYN )
    POSTYPE extend_item_position_;  // �Ⱓ ���� ����
    POSTYPE target_item_position_;  // ����, ����, �Ǽ��������� �Ⱓ ���� ������.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_EXTEND_DATE_ACK )
    POSTYPE			extend_item_position_;  // �Ⱓ ���� ���� ��ġ, �ش���ġ�� �������� 1 ���� ��Ų��.
    POSTYPE			target_item_position_;  // �Ⱓ ����� �������� ��ġ
    ITEMSTREAMEX	target_item_stream_;    // �Ⱓ ����� �������� ���ŵ� ��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_EXTEND_DATE_NAK )
    int error_code_;
PACKET_CG_FWD_CLOSE;
#endif

struct MSG_CG_ITEM_SELL_SYN : public MSG_BASE_FORWARD
{
    union
    {
        BYTE                    fixSIZE[8];
        struct
        {
            CODETYPE            m_NPCCode;
            SLOTIDX             m_atIndex;
            POSTYPE             m_atPos;
            DURATYPE sell_quantity_; // �Ǹ� ����, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
        };
    };

    void Encode();
    void Decode();
};

struct MSG_CG_ITEM_SELL_ACK : public MSG_CG_ITEM_SELL_SYN
{
	MONEY						m_Money;
};

struct MSG_CG_ITEM_SELL_NAK : public MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_BUY_SYN )
	CODETYPE					m_NPCCode;
	DWORD						m_dwShopListID;
	BYTE						m_ShopTabIndex;
	BYTE						m_ShopItemIndex; //< SHOPINFO::_MAX_SELLITEM_NUM�� ���� �ʾƾ� ��
    DURATYPE buy_quantity_; // ���� ����, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_BUY_ACK )
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    NPC_SHOP_TYPE m_ShopType;
#endif
	MONEY						m_Money;
	INVENTORY_TOTAL_INFO		m_TotalInfo;
	int GetSize()
	{
		return (sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_TotalInfo.m_InvenCount-m_TotalInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX));
	}
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_BUY_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;
/* (CHANGES) (f100520.1L) remove an unused section
struct MSG_CG_ITEM_OBTAIN_ACK : public MSG_BASE_FORWARD
{
	POSTYPE						m_ItemPos;
	ITEMSTREAM					m_ItemStream;
};	

struct MSG_CG_ITEM_OBTAIN_NAK : public MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};
*/
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_LOSE_ACK )
	// ������ �κ��丮��, ������ ������
	INVEN_DELETE_TOTAL_INFO		m_ItemInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize +  m_ItemInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
// ���PVP �� �̱״Ͻ����� ��ȯ ����
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_REWARD_ITEM_SYN )
	SLOTCODE					m_ItemCode;
	DWORD						m_Count;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_REWARD_ITEM_ACK )
	INVENTORY_TOTAL_INFO		m_ItemInfo;
	int GetSize()
	{
		return (
			sizeof(MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_ACK) -
			(INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(INVENTORY_TOTAL_INFO::SLOT_TYPE)
			);
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_REWARD_ITEM_NAK )
	BYTE						m_ErrorCode;
PACKET_CG_FWD_CLOSE;
#endif

//---------------------------------------------------------------------------------------------------
//
// ������ ����
//
// STREAM���� ������ ���� �ٲ� �������� ����� ����ȭ �� ������ �ִ�.
// @@@ OPT
//---------------------------------------------------------------------------------------------------


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANT_SYN )
	CODETYPE			m_NPCCode;
	POSTYPE				m_TargetPos;					//< ��þƮ �� ������
	BYTE				m_SuccessRateIndex;				//< ������ ���� Ȯ�� �ε��� �� ( 0 ~ 3 )
#ifdef _NA_003966_20111227_ADD_ENCHANT
    POSTYPE down_guard_item_pos; // ��þƮ ���н� �ٿ� �� �Ҹ� ���� ������ ��ġ
#endif
    void Init()
    {
        m_NPCCode = 0;
        m_TargetPos = INVALID_SLOT_POS;
        m_SuccessRateIndex = 0;
#ifdef _NA_003966_20111227_ADD_ENCHANT
        down_guard_item_pos = INVALID_SLOT_POS;
#endif
    }
    uint16_t GetSize() const
    {
        return static_cast<uint16_t>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANT_SUCCESS_ACK )
	//POSTYPE				m_TargetPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANT_FAILED_ACK )
	BYTE					m_byErrorCode;
//POSTYPE				m_TargetPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANT_NAK )
	BYTE					m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANTUP_SYN )
	POSTYPE				m_TargetPos;					//< ��þƮ �� ������
PACKET_CG_FWD_CLOSE;

// ack�ÿ� Ŭ���̾�Ʈ�� ������ �����ϰ�
// m_TargetItemEnchant�� ���� Ȥ�� ���п� ���� ��þƮ ��� ��
// m_TargetItemEnchant == 0�̸� ��� ������ ����
// m_TargetItemEnchant != 0�ƴϸ� ��� �������� ��þƮ �ٿ�
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANTUP_ACK )
	POSTYPE				m_TargetPos;
	BYTE				m_TargetItemEnchant;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANTUP_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_SOCKET_FILL_SYN )
	POSTYPE				m_SocketItemPos;
	POSTYPE				m_TargetPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_SOCKET_FILL_ACK )
	POSTYPE				m_SocketItemPos; 
	POSTYPE				m_TargetPos;
	ITEMSTREAMEX		m_TargetItemStream;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM,  CG_ITEM_SOCKET_FILL_NAK)
    DWORD m_dwErrorCode;
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    CODETYPE target_item_code; // ��带 ������ ��� ������ �ڵ�
#endif // _NA_000251_20100727_SOCKET_SYSTEM
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_SOCKET_EXTRACT_SYN )
	POSTYPE				m_ChargeIntemPos;
	POSTYPE				m_TargetPos;
	BYTE				m_SocketNum;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK )
	POSTYPE				m_ChargeIntemPos;
	POSTYPE				m_TargetPos; 
	ITEMSTREAMEX		m_TargetItemStream;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_SOCKET_EXTRACT_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_ACCESSORY_CREATE_SYN )
	enum { LOW_RING = 0, HIGH_RING, LOW_NECKLACE, HIGH_NECKLACE, };
	BYTE				m_type;				
	POSTYPE				m_SocketItemPos; 
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_ACCESSORY_CREATE_ACK )
	enum { LOW_RING = 0, HIGH_RING, LOW_NECKLACE, HIGH_NECKLACE, };
	BYTE				m_type;
	POSTYPE					m_SocketItemPos;
	MONEY					m_Money;
	INVENTORY_TOTAL_INFO	m_ItemInfo;
	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_ACCESSORY_CREATE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_SYN )
	CODETYPE				m_NPCCode;
	SLOTCODE				m_ComposeListCode;
	BYTE					m_byMakingNum;				//< ������� ����: default�� 1�̰���?
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_SUCCESS_ACK )
	BYTE					m_byMakingNum;		//���� ��û ����
	BYTE					m_bySuccessNum;		//���� ���� ����
	MONEY					m_Money;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    BYTE                    m_byPreventNum;     //���� ��� �Ҹ� ���� ����
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	INVENTORY_TOTAL_INFO	m_ItemInfo;         // ȹ���� ������(���۰��)

	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_FAILED_ACK )
MONEY					m_Money;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
BYTE                    m_byPreventNum;     //���� ��� �Ҹ� ���� ����
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_CRYSTALIZE_SYN )
    CODETYPE				m_NPCCode;
    SLOTCODE				m_CrystalizeListCode;
    POSTYPE					m_TargetPos;				//< ������ ��� ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_CRYSTALIZE_SUCCESS_ACK )
	MONEY					m_Money;
	INVENTORY_TOTAL_INFO	m_ItemInfo;
	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_CRYSTALIZE_FAILED_ACK )
	MONEY					m_Money;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_CRYSTALIZE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EXTRASTONE_USE_SYN )
	POSTYPE				m_ExtraStonePos;
	POSTYPE				m_TargetPos;				//< extrastone�� ������ ��� ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EXTRASTONE_USE_ACK )
	BYTE				m_NOptionAttrValueIndex;	//< SCItemSlot::SetNOptionAttr(m_NOptionAttrValueIndex) �Լ� ���
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EXTRASTONE_USE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_OBTAIN_DEFAULTITEM_CMD )
//	DWORD				m_ItemCode;
//PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_OBTAIN_ITEM_CMD )
	INVENTORY_TOTAL_INFO	m_tagInventory;
	int				GetSize() { return (sizeof(MSG_BASE_FORWARD)+m_tagInventory.GetSize()); }
PACKET_CG_FWD_CLOSE;

//_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_OBTAIN_ITEM_NAK )
    BYTE                m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_FREE_OWNERSHIP_BRD )
	DWORD				m_dwObjectKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_SYN )
	POSTYPE			m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_ACK )
	INVENTORY_TOTAL_INFO	m_tagInventory;
	int		GetSize() { return (sizeof(MSG_BASE_FORWARD)+m_tagInventory.GetSize()); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXCHANGE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DELETE_SYN )
	POSTYPE				m_AtPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DELETE_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DELETE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_CHANGE_DURA_CMD )
	POSTYPE				m_EquipPos;
	SLOTCODE			m_ItemCode;
	BYTE				m_Dura;
PACKET_CG_FWD_CLOSE;

// ������ ����
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_REPAIR_DURA_SYN )
	CODETYPE			m_NPCCode;
	bool				m_bAllRepair;
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_REPAIR_DURA_ACK )
	bool				m_bAllRepair;
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
	MONEY				m_Money;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_REPAIR_DURA_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_BLOCKC_CMD )
	SLOTIDX				m_SlotIdx;
	POSTYPE				m_SlotPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PK_DROP_ITEM_CMD )
	SLOTIDX				m_atIndex;
	POSTYPE				m_atPos;
PACKET_CG_FWD_CLOSE;

// ������ ���ҷ� ���� ������ �Ҹ�
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DESTROY_DURA_CMD )
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
PACKET_CG_FWD_CLOSE;

// ��ũ�� ������ ���
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PORTAL_SYN )
	POSTYPE				m_CoordPos;
	POSTYPE				m_PortalPos; 
PACKET_CG_FWD_CLOSE;

// ������ �������� ��ȭ
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_CHANGE_ITEM_INFO_CMD )
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
	ITEMSTREAMEX		m_ItemStream;
PACKET_CG_FWD_CLOSE;

// ������ ����
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DELETE_CMD  )
	BYTE				m_byReason;				// ��������(eITEM_DELETE_RESULT)
	SLOTIDX				m_atIndex;
	POSTYPE				m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXTRAINVENTORY_ADD_CMD  )
	BYTE				m_ExtraInventoryTabCount; //< ����ȭ �� ��ü ����
PACKET_CG_FWD_CLOSE;

// ��ǥ �ʱ�ȭ
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_CLEAR_COORD_SYN )
	POSTYPE				m_ClearPos;
	POSTYPE				m_CoordPos;
PACKET_CG_FWD_CLOSE;

//���� �ʱ�ȭ
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_INIT_STAT_SYN )
	POSTYPE				m_ItemSlotPos;	
	WORD				m_wDecreaseValue;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_INIT_STAT_ACK )
	POSTYPE				m_ItemSlotPos;
	WORD				m_wDecreaseValue;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_STAT_ONE_CHANGE_CMD )
	BYTE				m_AttrType;					// eATTR_TYPE
	DWORD				m_dwValue;					// value
	INT					m_iRemainStat;				//<	���� ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_STAT_ALL_CHANGE_CMD )
	SHORT				m_sStrength;
	SHORT				m_sDexterity;
	SHORT				m_sVitality;
	SHORT				m_sInteligence;
	SHORT				m_sSpirit;
	SHORT				m_sSkillStat1;
	SHORT				m_sSkillStat2;	
	INT					m_iRemainStat;				//<	���� ����	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_INIT_STAT_NAK )
	BYTE				m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXTEND_CASH_SLOT_CMD  )
	BYTE				m_ExtraEquipCashSlotCount; //< �߰��� ���� ĳ������ ��ü����
PACKET_CG_FWD_CLOSE;

// �������� ������ ����. Ŭ�󿡼� ó����������. ���� ���� �Ǵ� �ٸ��������λ��...
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_SETITEM_CHANGE_BRD  )  
	PLAYERKEY			m_PlayerKey;
	BOOL				m_bFull;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_HAMMER_OF_ALCHEMIST_SYN )
	POSTYPE				m_ChargeItemPos;
	POSTYPE				m_TargetItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_HAMMER_OF_ALCHEMIST_ACK )
	POSTYPE				m_ChargeItemPos;
	POSTYPE				m_TargetItemPos;	
	ITEMSTREAMEX		m_TargetItemStream;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_HAMMER_OF_ALCHEMIST_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_VALIDATION_OF_REIHALT_SYN )	
	POSTYPE				m_TargetItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_VALIDATION_OF_REIHALT_ACK )	
	POSTYPE				m_TargetItemPos;
	ITEMSTREAMEX		m_TargetItemStream;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_VALIDATION_OF_REIHALT_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_PET_ITEM_SYN )
	POSTYPE				m_PetItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_PET_ITEM_ACK )
	POSTYPE				m_PetItemPos;	//���� �ÿ��� ���
	INT					m_Result;		//eITEM_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_PANDORA_ITEM_NAK )
	INT					m_Result;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_PANDORA_ITEM_SYN )
	POSTYPE				m_InventoryPos;
	POSTYPE				m_PandoraItemPosFrom;
	POSTYPE				m_PandoraItemPosTo;
	BOOL				m_MoveInventory;
PACKET_CG_FWD_CLOSE;			

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN )
	POSTYPE				m_InventoryPos;
PACKET_CG_FWD_CLOSE;		

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_PANDORA_ITEM_ACK )
	BOOL				m_Remove; // �ǵ��� ���ڿ� �������� ����. �����ʿ�
	BOOL				m_MoveInventory; // �ǵ��� -> �κ��丮
	INT					m_Result;
	POSTYPE				m_InventoryPos;
	ITEMSTREAMEX		m_PandoraItemStream;
	INVENTORY_TOTAL_INFO m_InventoryInfo;
#ifdef __NA_1234_LOTTERY_ITEM_RESULT_BUG_FIX
	int GetSize()
	{
        // (WAVERIX) (090721) (BUG-FIX) ��Ŷ ũ�� ���... �̷��� ���ô�...-.-;;; �ܼ��ϰ� ����� ����� ����սô�.
        return sizeof(*this) - sizeof(m_InventoryInfo) + m_InventoryInfo.GetSize();
        //return sizeof(m_Remove) + sizeof(m_MoveInventory) + sizeof(m_Result) + sizeof(m_InventoryPos) + sizeof(m_PandoraItemStream)
        //	+ m_InventoryInfo.GetSize();
	}
#endif
PACKET_CG_FWD_CLOSE;		

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_PANDORA_ITEM_NAK )
	INT					m_Result;
PACKET_CG_FWD_CLOSE;		

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_PANDORA_ITEM_SYN )
	POSTYPE				m_InventoryPos;
PACKET_CG_FWD_CLOSE;

// ���� ���� ACK ������, �ǵ��� ���ھȿ� ������ ������ ������ �־�� �Ѵ�. 
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_PANDORA_ITEM_ACK )
	POSTYPE				m_InventoryPos;
	ITEMSTREAMEX		m_PandoraItemStream;
	BOOL				m_Identity;
	// PANDORA Item Status
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PANDORABOX_CLOSE_SYN )
	POSTYPE				m_InventoryPos;
PACKET_CG_FWD_CLOSE;	

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PANDORABOX_CLOSE_ACK )
PACKET_CG_FWD_CLOSE;	

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PANDORABOX_CLOSE_NAK )
	INT					m_Result;
PACKET_CG_FWD_CLOSE;	

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_REPURCHASE_SYN )
	CODETYPE m_NPCCode;
	POSTYPE m_RepurchasePos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_REPURCHASE_ACK )
	MONEY m_Money;
	INVENTORY_TOTAL_INFO m_TotalInfo;
	int GetSize()
	{
		return (sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_TotalInfo.m_InvenCount-m_TotalInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX));
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_REPURCHASE_NAK )
	BYTE m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EQUIP_ETHER_DEVICE_SYN )
	POSTYPE					m_DeviceItemPos;
	POSTYPE					m_WeaponItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EQUIP_ETHER_DEVICE_ACK )
	INT						m_Result;			//eITEM_RESULT
	POSTYPE					m_DeviceItemPos;
	POSTYPE					m_WeaponItemPos;	//�����ÿ��� ���
PACKET_CG_FWD_CLOSE;

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
// ���� �ٽ� ���� ��û
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RECREATE_SOCKET_SYN)
    typedef WORD MessageSize;

    POSTYPE request_item_pos; // ���� �ٽ� ���� ��û ������ �κ��丮 ��ġ
    POSTYPE target_item_pos; // ������ �ٽ� ������ ��� ������ �κ��丮 ��ġ

    // �ʱ�ȭ�Ѵ�. �޽����� ����ϱ� ���� �����ϰ� ���� ȣ�� ������.
    void Init()
    {
        request_item_pos = INVALID_SLOT_POS;
        target_item_pos = INVALID_SLOT_POS;
    }

    // ��û�� ���������� ũ�⸦ �������� ������� ���� �� �Լ��� �������.
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;
// ���� �ٽ� ���� ����
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RECREATE_SOCKET_ACK)
    typedef WORD MessageSize;
    typedef int ResultCode;
    
    // ó�� ��� 
    // ���� : RC::RC_ITEM_SUCCESS, 
    // ���� : ������ ���� ���� �ڵ�
    ResultCode result_code;

    // �ʱ�ȭ�Ѵ�. �޽����� ����ϱ� ���� �����ϰ� ���� ȣ�� ������.
    void Init()
    {
        result_code = RC::RC_ITEM_FAILED;
    }
    
    // ��û�� ���������� ũ�⸦ �������� ������� ���� �� �Լ��� �������.
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;
#endif // _NA_000251_20100727_SOCKET_SYSTEM

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_SYN)
	SLOTIDX					m_atIndex; // �����̳� �ε���
	POSTYPE					m_atPos; // ü���� ������ �ε���
	POSTYPE					m_ChangeItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_ACK)
	SHORT					m_ItemCount;
	POSTYPE					m_atPos; // ü���� ������ �ε���
	POSTYPE					m_ChangeItemPos; // �ٲ� ������ �ε���
	CODETYPE				m_ItemCodes[MAX_CHANGE_ITEMSTAT_SLOT_NUM];
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_NAK)
	DWORD					m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_SELECT_SYN)
	BOOLEAN					m_bSelect;// ������ ����:TRUE, �׳� ����:FALSE
	DWORD					m_SelectItemCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_SELECT_ACK)
	INVENTORY_TOTAL_INFO	m_InventoryInfo;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_SELECT_NAK)
	DWORD					m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN)
	POSTYPE					m_ItemPos;
	TCHAR					m_PetName[MAX_PET_NAME_LENGTH+1];
PACKET_CG_FWD_CLOSE;

//����, ���� ��� �� ��Ŷ.(������ Result Code�� �ϼ���)
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK)
	INT						m_ResultCode;					//RC::eITEM_RESULT
PACKET_CG_FWD_CLOSE;

//Ŭ�� ������ ������ SYN
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ACTIVE_ETHER_BULLET_SYN)
	BYTE					m_bActive;					// 1�̸� Ȱ��ȭ 0�̸� ��Ȱ��ȭ
	POSTYPE					m_ActiveBulletItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ACTIVE_ETHER_BULLET_ACK)
	INT						m_ResultCode;					//RC::eITEM_RESULT
	BYTE					m_BulletEffectCode;					//
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ETHER_ACTIVE_STATE_BRD)
	DWORD					m_dwPlayerKey;
	BYTE					m_bActive;					// 1�̸� Ȱ��ȭ 0�̸� ��Ȱ��ȭ
	BYTE					m_BulletEffectCode;					//
PACKET_CG_FWD_CLOSE;

//������ Ŭ��� ������ CMD
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_INACTIVE_ETHER_BULLET_CMD)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_IDENTIFY_POSSESSION_SYN)
	BYTE					m_ItemPos;	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_IDENTIFY_POSSESSION_ACK)
	INT						m_ResultCode;					//RC::eITEM_RESULT
	POSTYPE					m_PossessionItemPos;
	ITEMSTREAMEX			m_PossessionItemStream;
PACKET_CG_FWD_CLOSE;

//{__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN)
	POSTYPE					m_FireUpItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK)
	POSTYPE					m_FireUpItemPos;
	INT						m_ResultCode;					//RC::eITEM_RESULT
PACKET_CG_FWD_CLOSE;
//}

// Ÿ��, ������, ��Ÿ �׼� ���� ��û
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_SYN)
    RiderControl header;
    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

// Ÿ��, ������, ��Ÿ �׼� ���� ��û�� ���� ����, ������ ����ȭ
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_BRD, MSG_CG_ITEM_RIDER_CONTROL_SYN)
    RiderControlData data;

    void SetUnknownFailed() {
        header.request = header.eReq_None;
        header.result = header.eRet_Failed;
        //header.result_code <- �߰� ���ǵǸ� �Է��ϱ�� ����.
        data.data_type = data.eDataType_None;
    }

    void Clear();
    WORD GetSize() const {
        return sizeof(*this) - sizeof(data) + data.GetSize();
    }
PACKET_CG_INHERIT_CLOSE;

// Ÿ��, ������, ��Ÿ �׼� ��� ������ �ʿ信 ���� ������ ��� (��: �̼� �����)
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_CMD, MSG_CG_ITEM_RIDER_CONTROL_BRD)
    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_INHERIT_CLOSE;

// Rider�� Parts ����/���� ��û
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_SYN)
    RiderControl header;

    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

// Rider�� Parts ����/���� ��û�� ���� ����, ������ ����ȭ
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_BRD, MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN)
    RiderControlData data;

    void SetUnknownFailed() {
        header.request = header.eReq_None;
        header.result = header.eRet_Failed;
        //header.result_code <- �߰� ���ǵǸ� �Է��ϱ�� ����.
        data.data_type = data.eDataType_None;
    }

    void Clear();
    WORD GetSize() const {
        return sizeof(*this) - sizeof(data) + data.GetSize();
    }
PACKET_CG_INHERIT_CLOSE;

// Rider�� Parts ����/���� ��� ������ �ʿ信 ���� ������ ��� (��: ���� �����, Rider|Parts)
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_CMD, MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD)
    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_INHERIT_CLOSE;

// [10/30/2009 namka199] _NA_0_20091030_RANDOM_ITEM
PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_RANDOM_ITEM_SYN )
    POSTYPE				m_TargetPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_RANDOM_ITEM_ACK )
    INVENTORY_TOTAL_INFO	m_ItemInfo;
    int GetSize()
    {
        return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_RANDOM_ITEM_NAK )
    DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


#ifdef _NA_1419_20100121_ACCUMULATE_POINT

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN )
    BYTE    m_First;    // ����ޱ� ��ư�� �������� TRUE, �׿� FALSE
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK )
    enum { ACCUMULATE_POINT_ITEM_COUNT = 18 };
    SLOTCODE               m_ItemCodeArray[ACCUMULATE_POINT_ITEM_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK )
    DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_ACCUMULATE_POINT_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_ACCUMULATE_POINT_ACK )
    BYTE                    m_byDropItemOrder;  // ��� ������ ���� ����
    WORD                    m_wAccumulatePoint; // ���� ���� ����Ʈ
    INVENTORY_TOTAL_INFO	m_ItemInfo;
    int GetSize()
    {
        return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_USE_ACCUMULATE_POINT_NAK )
    DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

#endif //_NA_1419_20100121_ACCUMULATE_POINT

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN)
    nsSlot::InventoryConfig changed;
PACKET_CG_FWD_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK,
                       MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN)
    RC::eITEM_RESULT result;
PACKET_CG_INHERIT_CLOSE;
//==================================================================================================
//_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ETHERIA_CONTROL_SYN)
    EtheriaControl header;
    //
    void Clear();
    uint16_t GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_ETHERIA_CONTROL_ACK, MSG_CG_ITEM_ETHERIA_CONTROL_SYN)
    EtheriaControlData data;
    //
    void SetUnknownFailed() {
        header.request = header.eReq_None;
        header.result = header.eRet_Failed;
        header.result_code = 0; // should be set considering a result diversity
        data.data_type = data.eDataType_None;
    }
    void Clear();
    uint16_t GetSize() const {
        return sizeof(*this) - sizeof(data) + data.GetSize();
    }
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_ETHERIA_CONTROL_BRD, MSG_CG_ITEM_ETHERIA_CONTROL_ACK)
    // same as the MSG_CG_ITEM_ETHERIA_CONTROL_ACK specification
PACKET_CG_INHERIT_CLOSE;
//==================================================================================================

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN) // ĳ���� �ܸ� ���� ������ ��� ��û
    POSTYPE item_position_; // ������ ��ġ
    CharacterAppearanceInfo character_appearance_; // ������ �ܸ�
    //
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CG_CLOSE;

// ������ �ռ� �Ǵ� ���� ��û
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN) 
    enum 
    { 
        kZardRequest = 1, // ��� �ռ�
        kCubeRequest = 2  // ť�� �ռ�/����
    };
    enum { kMaxMaterialSize = 9 }; // ��� �ִ� ����
    typedef BYTE RequestType;
    typedef CODETYPE ReservedCode;
    typedef BYTE MaterialListSize;
    typedef WORD MessageSize;
    struct MaterialInfo // ��� ����
    {
        POSTYPE inventory_position_; // ����� �κ��丮 ��ġ        
        BYTE selected_number_; // �ռ� �Ǵ� �����ϱ� ���� �κ��丮���� ������ ����
        BYTE matched_index_; // �ռ������� ��ġ�ϴ� ��� �ε���
    }; // MaterialInfo

    RequestType request_type_; // ��û ���� (���/ť��)
    bool is_composition_; // �ռ� ���� (�ռ��� �ƴ϶�� ����)
    // ���� �ڵ�
    // ��� �ռ��� �� ÷������ �κ��丮 ��ġ (�����Ѵٸ� INVALID_POSTYPE_VALUE : Define.h)
    // ť�� �ռ��� �� �ռ��ڵ�
    // ť�� ������ �� ���� ����Ʈ
    ReservedCode reserved_code_;
    POSTYPE using_item_position_; // �������� ����ؼ� ��û�ߴٸ� ������ ��ġ
    MaterialListSize material_list_size_; // �ռ� �Ǵ� ������ ��� ����
    MaterialInfo material_list_[kMaxMaterialSize]; // �ռ� �Ǵ� ������ ��� ���

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            ((kMaxMaterialSize - material_list_size_) * sizeof(MaterialInfo)));
    }
PACKET_CG_CLOSE; // CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN
// ������ �ռ� �Ǵ� ���� ����
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK) 
    typedef int ResultCode;
    typedef WORD MessageSize;
    bool is_composition_; // �ռ� ���� (�ռ��� �ƴ϶�� ����)
    ResultCode result_code_; //��û ó�� ���
    INVENTORY_TOTAL_INFO added_inventory_items_; // �������� �κ��丮�� �߰��� ������ ����
    // ���� ���Ǿ� ������ �κ��丮 ������ ������ MSG_CG_ITEM_LOSE_ACK�� ������.
    // �̹� �����Ǿ� �����Ƿ� �߰� �۾��� �ʿ����.

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM - 
            added_inventory_items_.m_InvenCount - 
            added_inventory_items_.m_TmpInvenCount) * sizeof(ITEMSLOTEX));
    }
PACKET_CG_CLOSE; // CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK
// ������ �ռ� �Ǵ� ���� ������ ��û
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN) 
    typedef MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::RequestType RequestType; 
    typedef WORD MessageSize;
    RequestType request_type; // ��û ����
    bool is_composition; // �ռ� ����

    void Init()
    {
        request_type = MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest;
        is_composition = true;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_CLOSE; // CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN
// ������ �ռ� �Ǵ� ���� ������ ����
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
    enum { kMaxHistoryListSize = ItemCompositionOrDecompositionHistory::kMaxHistories };
    typedef int ResultCode;    
    typedef BYTE HistoryListSize;
    typedef WORD MessageSize;
    ResultCode result_code; // ��û ó�� ���
    HistoryListSize history_list_size; // ���� ����
    ItemCompositionOrDecompositionHistory history_list[kMaxHistoryListSize]; // ����

    void Init()
    {
        result_code = RC::RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES;
        history_list_size = 0;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            ((kMaxHistoryListSize - history_list_size) * 
                sizeof(ItemCompositionOrDecompositionHistory)));
    }
PACKET_CG_CLOSE; // CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK
// ������ �ռ� �Ǵ� ���� ���� ���� ��û
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD)
    enum { kMaxConditionListSize = ItemCompositionOrDecompositionHistory::kMaxHistories };
    typedef BYTE ConditionListSize;
    typedef ItemCompositionOrDecompositionHistory::ModifyCondition ModifyCondition;
    typedef WORD MessageSize;
    ConditionListSize condition_list_size;
    ModifyCondition modify_condition_list[kMaxConditionListSize];

    void Init()
    {
        condition_list_size = 0;
        ::ZeroMemory(modify_condition_list, sizeof(modify_condition_list));
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            ((kMaxConditionListSize - condition_list_size) * 
                sizeof(ModifyCondition)));
    }
PACKET_CG_CLOSE; // CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
// �׼� ��ų, �������� ���� ��Ȯ�� ���� �������� ���ϼ� Ȯ�� ��û
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_IDENTIFY_SOCKET_CMD)
    typedef WORD MessageSize;
    POSTYPE unidentified_item_pos_; // ��Ȯ�� ������ ��ġ
    bool use_identify_item_; // �������� ����� Ȯ���ΰ�?
    POSTYPE identify_item_pos_; // �������� ���� Ȯ�ν� Ȯ�� ������ ��ġ (�����Ѵٸ� INVALID_POSTYPE_VALUE : Define.h)

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_CLOSE; // CG_ITEM_IDENTIFY_SOCKET_CMD

PACKET_CG_OPEN(CG_ITEM, CG_ITEM_IDENTIFY_SOCKET_NAK) 
    typedef int ErrorCode;
    typedef WORD MessageSize;
    ErrorCode error_code_; // RC::eITEM_RESULT

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_CLOSE; // CG_ITEM_IDENTIFY_SOCKET_NAK

// ��� �ռ� ����
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_ZARD_COMPOSE_ACK, MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK)
    // ó�� ��� �ڵ�
    // RC_ITEM_SUCCESS_TO_COMPOSE_ZARD : ��� �ռ� ����
    // RC_ITEM_FAILED_TO_COMPOSE_ZARD : ��� �ռ� ����
    // RC_ITEM_INVALID_VALUE : Ŭ�󿡼� ��û�� ���� �ǹٸ��� ����
    // RC_ITEM_HAVENOTSPACE : �κ��丮�� ������ ���� ���� ������ ����
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_ZARD_COMPOSE_ACK
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_002253_20100811_CUBE_SYSTEM
// ť�� ������ �ռ� �Ǵ� ���� ����
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK, MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK)
    // ó�� ��� �ڵ�
    // RC_ITEM_SUCCESS_TO_COMPOSE_CUBE : ť�� �ռ� ����
    // RC_ITEM_FAILED_TO_COMPOSE_CUBE : ť�� �ռ� ���� (�������� ���� ��� ���⿡ ����)
    // RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE : ť�� ���� ����
    // RC_ITEM_FAILED_TO_DECOMPOSE_CUBE : ť�� ���� ���� (�������� ���� ��� ���⿡ ����)
    // RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE : ī�� ���¶� ��� �Ұ�
    // RC_ITEM_INVALID_VALUE : Ŭ�󿡼� ��û�� ���� �ǹٸ��� ����
    // RC_ITEM_HAVENOTSPACE : �κ��丮�� ������ ���� ���� ������ ����
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK
// ������ ť�� ������ �ռ� ������ ����
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_GET_CUBE_COMPOSITION_HISTORIES_ACK, MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
    // ó�� ��� �ڵ�
    // ���� : RC_ITEM_SUCCESS_TO_GET_COMPOSITION_LIST
    // ���� : RC_ITEM_FAILED_TO_GET_COMPOSITION_LIST �Ǵ� ������ ���� �⺻ ���� �ڵ�
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_GET_CUBE_COMPOSITION_HISTORIES_ACK
#endif // _NA_002253_20100811_CUBE_SYSTEM

// _NA_003740_20111122_RANDOM_ITEM
// ������ �ɷ�ġ ����ȭ
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RANDOMIZE_SYN)
    typedef WORD MessageSize;

    POSTYPE request_item_pos; // ����ȭ ��û ������ �κ��丮 ��ġ
    POSTYPE target_item_pos; // ����ȭ ��� ������ �κ��丮 ��ġ

    void Init()
    {
        request_item_pos = INVALID_SLOT_POS;
        target_item_pos = INVALID_SLOT_POS;
    }

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_OPEN(CG_ITEM, CG_ITEM_RANDOMIZE_ACK)
    typedef int ResultCode;
    typedef WORD MessageSize;

    ResultCode result_code; // RC::eITEM_RESULT ����
    POSTYPE request_item_pos; // ����ȭ ��û ������ �κ��丮 ��ġ
    POSTYPE target_item_pos; // ����ȭ ��� ������ �κ��丮 ��ġ

    void Init()
    {
        result_code = RC::RC_ITEM_SUCCESS;
        request_item_pos = INVALID_SLOT_POS;
        target_item_pos = INVALID_SLOT_POS;
    }

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_CLOSE; // CG_ITEM_IDENTIFY_SOCKET_NAK

//_NA_004035_20120227_COSTUME_ITEM //�ڽ�Ƭ ������ ��Ŷ..
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_COSTUME_ACK )
    typedef WORD MessageSize;

    bool    is_equipped_costume_;
    POSTYPE costume_item_pos_;
    void Init()
    {
        is_equipped_costume_ = false; //�ڽ�Ƭ ������ Ȱ��ȭ ����
        costume_item_pos_ = INVALID_SLOT_POS; //�ڽ�Ƭ ������ �κ��丮 ��ġ
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_COSTUME_BRD )
    typedef WORD MessageSize;    
    
    DWORD       object_key_;
    SLOTCODE    costume_item_code_;
    void Init()
    {
        object_key_ = 0;
        costume_item_code_ = 0;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;

//_NA_008610_20160104_ADD_DECORATECOSTUME
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_DECORATE_COSTUME_ACK )
typedef WORD MessageSize;

bool    is_equipped_costume_;
POSTYPE deco_costume_item_pos_;
void Init()
{
    is_equipped_costume_ = false; //�ڽ�Ƭ ������ Ȱ��ȭ ����
    deco_costume_item_pos_ = INVALID_SLOT_POS; //�ڽ�Ƭ ������ �κ��丮 ��ġ
}
MessageSize GetSize() const
{
    return static_cast<MessageSize>(sizeof(*this));
}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_DECORATE_COSTUME_BRD )
typedef WORD MessageSize;    

DWORD       object_key_;
SLOTCODE    deco_costume_item_code_;
void Init()
{
    object_key_ = 0;
    deco_costume_item_code_ = 0;
}
MessageSize GetSize() const
{
    return static_cast<MessageSize>(sizeof(*this));
}
PACKET_CG_FWD_CLOSE;

//!~ _NA_006372_20130108_BADGE_ITEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_TOGGLE_ACK )
    WORD    toggle_item_type_;
    bool    is_equipped_;
    POSTYPE item_pos_;
PACKET_CG_FWD_CLOSE;
//~! _NA_006372_20130108_BADGE_ITEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_AWAKENING_SYN )
    POSTYPE	equip_item_pos;     // ������ ������
    POSTYPE	material_item_pos;  // ��� ������
    POSTYPE	additive_item_pos;  // ÷���� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_AWAKENING_ACK )
    int result_code;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EVOLUTION_SYN )
    POSTYPE	equip_item_pos;     // ��ȭ�� ������
    POSTYPE	additive_item_pos;  // ÷���� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EVOLUTION_ACK )
    int result_code;
PACKET_CG_FWD_CLOSE;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_SPA_COSTUME_CHANGE_SYN )
    SLOTIDX	slotcontainer_type;
    POSTYPE	item_pos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_SPA_COSTUME_CHANGE_BRD )
    DWORD player_object_key;
    SLOTCODE spa_ticket_code;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_SPA_COSTUME_CHANGE_NAK )
    BYTE error_code;
PACKET_CG_FWD_CLOSE;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

//������������������������������������������������������������������������������������������������
// ������ ����
//������������������������������������������������������������������������������������������������

//������������������������������������������������������������������������������������������������
// �ŷ�
//������������������������������������������������������������������������������������������������
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_SYN )
	DWORD		m_dwPlayerKey;						// ��û�� �÷��̾� Ű
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_ACK )	// �ŷ� ��û�� �¶��Ǿ���
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_CMD )	// ���濡 �ŷ�â ���� ��û
	DWORD		m_dwPlayerKey;						// �ŷ� ��û�� �÷��̾� Ű
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_SYN )
	POSTYPE		m_OrgPos;							// �κ��丮�� ������ ��ġ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_ACK )	
	POSTYPE		m_OrgPos;							// �κ��丮�� ������ ��ġ
	POSTYPE		m_TradePos;							// ����â�� ������ ��ġ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_CMD )
	POSTYPE			m_TradePos;						// ������� �ø� �������� �ŷ�â�� ��ġ
	RENDER_ITEMSTREAMEX	m_ItemStream;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_MONEY_SYN )
	MONEY			m_money;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_MONEY_ACK )
	MONEY			m_money;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_MONEY_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_MONEY_CMD )
	MONEY		m_money;						// ������� �÷����� ���� �׼�
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_SYN )
	POSTYPE		m_TradePos;						// �ŷ�â�� ��ġ�� �ִ� �������� �κ��丮�� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_ACK )
	POSTYPE		m_TradePos;						// �ŷ�â�� ��ġ�� �ִ� �������� �κ��丮�� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_CMD )
	POSTYPE		m_TradePos;						// ������� �ŷ�â�� Ư�� �������� ������
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_SYN )
	MONEY			m_money;					// �ŷ�â���� �� ���� �ݾ�
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_ACK )
	MONEY			m_money;					// �ŷ�â���� �� ���� �ݾ�
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_CMD )
	MONEY			m_money;					// �ŷ�â���� �� ���� �ݾ�
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PROPOSAL_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PROPOSAL_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PROPOSAL_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PROPOSAL_CMD )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_CMD )
PACKET_CG_FWD_CLOSE;

//.//	TRADE POPUP

PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_RESPONSE_SYN )
	BOOL		m_bAccept;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_RESPONSE_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_MODIFY_RESPONSE_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_ACCEPT_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_ACCEPT_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_ACCEPT_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_CANCEL_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_CANCEL_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_CANCEL_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_CANCEL_CMD )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_ACCEPT_CMD )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_ACCEPT_BRD )
	MONEY				m_Money;
	TRADE_TOTAL_INFO	m_TradeInfo;	
	int GetSize()
	{
		return (sizeof(*this) - (TRADE_TOTAL_INFO::MAX_SLOT_NUM - m_TradeInfo.m_InvenCount-m_TradeInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// �ŷ�,���λ���
//������������������������������������������������������������������������������������������������


//@@@ OPT : ����ȭ ���� char m_pszTitle[MAX_VENDOR_TITLE_LENGTH];
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_SYN )
    TCHAR m_pszTitle[MAX_VENDOR_TITLE_LENGTH];   // MAX_VENDOR_TITLE_LENGTH�� ��ü�� �ִ� Ÿ��Ʋ �����ε�...
    ESTABLISHER_VENDOR_TOTAL_INFO m_ItemInfo;
    int GetSize() const
    {
        return (sizeof(*this)-(ESTABLISHER_VENDOR_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_Count)*sizeof(VENDOR_ITEMSLOT));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_BRD )
	DWORD m_dwPlayerKey;		//< �������� �÷��̾� Ű
	char m_pszTitle[MAX_VENDOR_TITLE_LENGTH+1];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszTitle);
        size_t len = strlen(m_pszTitle) + 1;
        assert(len <= _countof(m_pszTitle));
        len = min(len, _countof(m_pszTitle));
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszTitle[0]) * len);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_SYN )
	BYTE m_byEndKind;				//< 0�̸� ����, 1�̸� PAUSE
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_ACK )
	BYTE m_byEndKind;				//< 0�̸� ����, 1�̸� PAUSE
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_BRD )
	DWORD m_dwPlayerKey;		//< ������ ������ �÷��̾��� Ű
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_NAK)
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_SYN )
	POSTYPE m_VendorPos;				//< ���� �ҷ��� �������� ���� ��ġ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_ACK )
	POSTYPE m_VendorPos;				//< ���� �ҷ��� �������� ���� ��ġ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_BRD )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_CANCEL_SYN )
	POSTYPE m_VendorPos;				//< ���� �ҷ��� �������� ���� ��ġ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_CANCEL_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_CANCEL_BRD )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_CANCEL_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_APPLY_SYN )
	VENDOR_ITEMSLOT m_ItemSlot;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_APPLY_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_APPLY_BRD )
	POSTYPE m_VendorPos;
	VENDOR_ITEMSLOTEX m_ItemSlot;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_APPLY_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_INSERT_SYN )
	VENDOR_ITEMSLOT m_ItemSlot;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_INSERT_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_INSERT_BRD )
	POSTYPE m_VendorPos;
	VENDOR_ITEMSLOTEX m_ItemSlot;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_INSERT_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_DELETE_SYN )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_DELETE_ACK )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_DELETE_BRD )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_DELETE_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_SYN )
	POSTYPE m_VendorPos;
	BYTE m_BuyNum;			//< ����� ���� : �Ϲ������δ� �Ĵ� ������ �ִ�ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_VENDOR, CG_VENDOR_BUY_ACK)
	MONEY m_Money;				//< �����ϰ� �� ���� ���� ��
	INVENTORY_TOTAL_INFO m_InventoryTotalInfo;

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_InventoryTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_InventoryTotalInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_CMD )
	POSTYPE m_VendorPos;		//< �ȷ��� ������ ��������
	MONEY m_Money;				//< �����ڰ� �Ǹ��ϰ� ������ ��
	BYTE	m_SellNum;			//< �����ڰ� �� ����
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_BRD )
	POSTYPE m_VendorPos;		//< �ȷ��� ������ ������
	BYTE	m_SellNum;			//< �� ����
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_START_SYN )
	DWORD m_dwPlayerKey;		//< ������ ������ �÷��̾��� Ű
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_START_ACK )
	char m_pszTitle[MAX_VENDOR_TITLE_LENGTH];

    // (BUGFIX) (091116) (WAVERIX) 1st modified - size
    // (TODO) change to block convert structure
    // SlotNum(8) + Money(64) + Lock(1) + RENDER_ITEMSTREAMEX(16 + 8 + 24(v7))
    enum {
        _NODE_BIT_SIZE = 64 + 1 + sizeof(RENDER_ITEMSTREAMEX) * 8,
        _MAX_BIT_SIZE = (8) + _NODE_BIT_SIZE * VENDOR_TOTAL_INFO::MAX_SLOT_NUM,
        _MAX_FIX_AROUND_SIZE = _MAX_BIT_SIZE / 8,
        _MAX_BINARY_SIZE = 2000,
    };
    BOOST_STATIC_ASSERT(_MAX_BINARY_SIZE > _MAX_FIX_AROUND_SIZE);

	WORD	m_Size;
	BYTE	m_BitStreamBuffer[_MAX_BINARY_SIZE];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_BitStreamBuffer);
        return static_cast<uint16_t>(kMinSize + sizeof(m_BitStreamBuffer[0]) * m_Size);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_START_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_END_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_END_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_END_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_TITLE_SYN )
	TCHAR m_pszTitle[MAX_VENDOR_TITLE_LENGTH];
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_TITLE_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_TITLE_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_TITLE_BRD )
	DWORD m_dwPlayerKey;		//< �������� �÷��̾� Ű
	char m_pszTitle[MAX_VENDOR_TITLE_LENGTH+1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszTitle);
        size_t len = strlen(m_pszTitle) + 1;
        assert(len <= _countof(m_pszTitle));
        len = min(len, _countof(m_pszTitle));
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszTitle[0]) * len);
    };
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// ���λ��� ����Ʈ
//������������������������������������������������������������������������������������������������
//���� ����Ʈ ����
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_NAK )
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//���� ����Ʈ Page ��û, 
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_REQUIRE_PAGE_SYN )
BYTE					m_byKind;						//eVENDOR_LIST_PAGE_KIND
BYTE					m_byPage;						//���° ������ �ΰ� - 1���� ����...
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_REQUIRE_PAGE_ACK )
    BYTE m_pBuffer[sizeof(VENDOR_LIST_TOTAL_INFO)];
    //
    uint16_t ConvertforSend(const VENDOR_LIST_TOTAL_INFO& total_info);
    void ConvertForRecv(VENDOR_LIST_TOTAL_INFO& total_info) const;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_REQUIRE_PAGE_NAK )
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//���� ����Ʈ �˻� ��û
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_SEARCH_SYN )
BYTE					m_ebyKind;						//eVENDOR_LIST_PAGE_KIND
char					m_sPattern[MAX_VENDOR_SEARCH_LENGTH];
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
SLOTCODE                m_ItemCode;
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_SEARCH_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_SEARCH_NAK )
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//���� ����Ʈ ����
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_END_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_END_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_END_NAK )
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//!~_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_ENHANCED_SEARCH_SYN )
    TCHAR search_items[MAX_ITEMNAME_LENGTH+1];
    int page;
PACKET_CG_FWD_CLOSE;

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_ENHANCED_SEARCH_ACK )
    BYTE m_byErrorCode;
    ENHANCED_VENDOR_SEARCH_RESULT search_result;

    void clear()
    {
        m_byErrorCode = 0;
        memset(&search_result, 0, sizeof(search_result));
    };
PACKET_CG_FWD_CLOSE;
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
//~!_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

//������������������������������������������������������������������������������������������������
// �ʵ� ������ �ݱ�,������
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_MONEY_SYN )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T1(142));
	PACKET_RND_SHRBLK64_CONT(DWORD, m_dwFieldItemObjectKey);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_MONEY_ACK )
	MONEY						m_Money;
	WORD						m_BonusRatio;					// �߰� �ۼ�Ʈ	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_MONEY_NAK )
	DWORD						m_dwErrorCode;
	DWORD						m_dwFieldItemObjectKey;
PACKET_CG_FWD_CLOSE;


struct MSG_CG_ITEM_DROP_MONEY_SYN : public MSG_BASE_FORWARD
{
	DWORD						m_dwDropMoney;
};
struct MSG_CG_ITEM_DROP_MONEY_NAK : public MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};

//������������������������������������������������������������������������������������������������
// â��
//������������������������������������������������������������������������������������������������


PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_START_SYN )
	CODETYPE					m_NPCCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_START_ACK )
	enum eINVEN_LOCK_MODE { ENABLE = 0, EMPTY_PWD = 0, EXIST_PWD = 1, DISABLED = 2,
	ENABLEnEMPTY = ENABLE|EMPTY_PWD, ENABLEnEXIST = ENABLE|EXIST_PWD, DISABLEDnEXIST = DISABLED|EXIST_PWD, };
	
	BYTE	m_byWarehouseLock;
	MONEY	m_WarehouseMoney;

	enum { _MAX_BINARY_SIZE = sizeof(RENDER_ITEMSTREAMEX)*(WAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM),};
	WORD	m_Size;
	BYTE	m_BitStreamBuffer[_MAX_BINARY_SIZE];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_BitStreamBuffer);
        return static_cast<uint16_t>(kMinSize + sizeof(m_BitStreamBuffer[0]) * m_Size);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_START_NAK )
	DWORD m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_MONEY_SYN )
	enum { PUTMONEY = 0, GETMONEY = 1, };
	BYTE				m_byType;
	MONEY				m_Money;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_MONEY_ACK )
	MONEY				m_InventoryMoney;
	MONEY				m_WarehouseMoney;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_MONEY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_END_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_END_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_END_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//������������������������������������������������������������������������������������������������
// â��, �κ��丮 ��� (�н�����)����
// ���� ��ũ�� : __TASK_WAREHOUSE_N_INVENTORY_LOCK_BY_PWD__
//������������������������������������������������������������������������������������������������

// <TYPE DEFINITIONs>
typedef util::cSTRING_BUFFER<MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1>	sPWD_STREAM;
typedef util::cSTRING_BUFFER<MAX_SSN_STREAM_LENGTH>					sSSN_STREAM;

/*																*/	
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_CHECK_SYN )
	enum eREQ_CODE	// <REQUEST CODE>
	{
		SYNC_FIRST			,	// ù��° ��û (������ �̰ͺ��� ������)
		SYNC_CLEAR			,	// ��� �ʱ�ȭ ��û��
		SYNC_CANCEL			,	// ��� ���� ���� ��ü�� ����� ��,
		SYNC_WITH_PWD		,	// ~_ACK::eRET_CODE::NEED_PWD | INVALID_PWD ���Ž�
		SYNC_WITH_SSN		,	// ~_ACK::eRET_CODE::NEED_SSN | INVALID_SSN ���Ž�
	};
	BYTE					m_ReqCode;
	sSSN_STREAM				m_PWDnSSN;

	// <METHODs>
	static WORD				MINSIZE() { return (WORD)(sizeof(MSG_CG_WAREHOUSE_PWD_CHECK_SYN) - sizeof(sSSN_STREAM)); }

	inline BYTE&			ReqCode()			{ return m_ReqCode; }

	inline VOID				SetFirstSync()		{ ReqCode() = SYNC_FIRST; };
	inline VOID				SetClearSync()		{ ReqCode() = SYNC_CLEAR; };
	inline VOID				SetCancelSync()		{ ReqCode() = SYNC_CANCEL; };

	inline VOID				SetPassword( const TCHAR* ptszPWD )
	{
		ReqCode() = static_cast<uint8_t>(ptszPWD ? SYNC_WITH_PWD : SYNC_CANCEL);
		if( ptszPWD )
			m_PWDnSSN.CopyFrom( ptszPWD );
	};

	inline VOID				SetSSN( const TCHAR* ptsSSN )
	{
		ReqCode() = static_cast<uint8_t>(ptsSSN ? SYNC_WITH_SSN : SYNC_CANCEL);
		if( ptsSSN )
			m_PWDnSSN.CopyFrom( ptsSSN );
	}

	inline TCHAR*			GetPassword()		{ return m_PWDnSSN.GetBUFFER(); };
	inline BYTE*			GetSSN()			{ return (BYTE*)m_PWDnSSN.GetBUFFER(); }

	inline WORD				GetSize()
	{
		switch( ReqCode() )
		{
		case SYNC_FIRST:
		case SYNC_CLEAR:
		case SYNC_CANCEL:
			return MINSIZE();
		case SYNC_WITH_PWD:
		case SYNC_WITH_SSN:
			return (WORD)sizeof(*this);
		}
		return 0;	// 0���� ������, ������ �ش� Ŭ�� ���� ���߰� �����. ������
	}
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN : MSG_CG_WAREHOUSE_PWD_CHECK_SYN
{
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_CHECK_SYN; }
};

/*																*/	
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_CHECK_ACK )
	enum eRET_CODE
	{	// <RESULT_CODE>
		RET_FAIL			,	// �������� ��Ȳ, ������ ���� ���� ���� �ڵ�
		RET_SUCCESS			,	// ��� ���� ������
		RET_SUCCESS_CLEAR	,	// ��� �ʱ�ȭ ������
		RET_CANCELED		,	// ��� ���� ���� ��ü ��ҵ� ���� (~_SYN::SYNC_CANCEL �� ���� ����)
		RET_NEED_PWD		,	// ��� �䱸
		RET_INVALID_PWD		,	// ���ǿ� ���� �ʴ� ����̴�
		RET_NEED_SSN		,	// ��� ���� ������ ���� SSN �䱸
		RET_INVALID_SSN		,	// �߸��� SSN�̴�
	};
	// <FIELD>
	BYTE					m_byRetCode;
	//
	// <METHODs>
	inline BYTE&			ReturnCode() { return m_byRetCode; }
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK : MSG_CG_WAREHOUSE_PWD_CHECK_ACK
{
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_CHECK_ACK; }
};

/*																*/	
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_CHECK_NAK )
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_CHECK_NAK : MSG_CG_WAREHOUSE_PWD_CHECK_NAK
{
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_NAK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_CHECK_NAK; }
};

/*																*/	
struct MSG_CG_WAREHOUSE_PWD_SETTING_SYN : MSG_CG_WAREHOUSE_PWD_CHECK_SYN
{
	MSG_CG_WAREHOUSE_PWD_SETTING_SYN() { m_byCategory = CG_WAREHOUSE; m_byProtocol = CG_WAREHOUSE_PWD_SETTING_SYN; }
};

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN : MSG_CG_WAREHOUSE_PWD_SETTING_SYN
{
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_SETTING_SYN; }
};

/*																*/	
struct MSG_CG_WAREHOUSE_PWD_SETTING_ACK : MSG_CG_WAREHOUSE_PWD_CHECK_ACK
{
	MSG_CG_WAREHOUSE_PWD_SETTING_ACK() { m_byCategory = CG_WAREHOUSE; m_byProtocol = CG_WAREHOUSE_PWD_SETTING_ACK; }
};

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK : MSG_CG_WAREHOUSE_PWD_SETTING_ACK
{
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_SETTING_ACK; }
};

/*																*/	
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_SETTING_NAK )
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK : MSG_CG_WAREHOUSE_PWD_SETTING_NAK
{
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_SETTING_NAK; }
};

/*																*/	
// ��ݻ���->�������, �������->��ݻ��� ��� ��û ��Ŷ ����ü
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_SYN )
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN : MSG_CG_WAREHOUSE_PWD_LOCKING_SYN
{
	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_LOCKING_SYN; }
};

/*																*/	
// ���� ����ְ�, �ӽ� ��� ���������� ���, ��ݻ��·� ��ȯ�Ǵ� ��츸 �ִ�.
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_ACK )
	enum eRET_CODE
	{
		RET_SUCCESS_LOCKED			,	// �������->��ݻ��� ��ȯ
		RET_NEED_PWD_FOR_LOCKING	,	// ���� ����� �������� �ʾƼ� ����� �ϱ� ���� ��� �䱸
		RET_NEED_PWD_FOR_UNLOCKING	,	// ��ݻ��¿��� ����������·� ��ȯ�ϱ� ���� ��� �䱸
	};
	BYTE			m_byRetCode;

	// <METHODs>
	inline BYTE&	ReturnCode() { return m_byRetCode; }
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK : MSG_CG_WAREHOUSE_PWD_LOCKING_ACK
{
	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_LOCKING_ACK; }
};

/*																*/	
// CG_WAREHOUSE_PWD_LOCKING_ACK �̿��� ��Ȳ, �� ��Ȳ�� �ش� RET_CODE�� ����
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_NAK )
	enum eERR_CODE
	{
		IRREGULAR_LOCKING_POLICYMENT	,	// â��,�κ� ��� �÷ο��� �־�� �ȵǴ� ��Ȳ�� �߻�
	};
	BYTE			m_byErrCode;

	// <METHODs>
	inline BYTE&	ErrorCode() { return m_byErrCode; }
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK : MSG_CG_WAREHOUSE_PWD_LOCKING_NAK
{
	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_LOCKING_NAK; }
};


//������������������������������������������������������������������������������������������������
// TRIGGER ���� ��Ŷ
//������������������������������������������������������������������������������������������������

struct MSG_CG_TRIGGER_CLICK_OBJECT_SYN : public MSG_BASE_FORWARD
{
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T2(141));
	PACKET_RND_SHRBLK64_CONT(DWORD, m_dwClickedObjectKey);
	PACKET_RND_SHRBLK64_END();
};

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_CLICK_OBJECT_BRD )
	DWORD						m_dwClickedUserObjectKey;
	DWORD						m_dwClickedObjectKey;
PACKET_CG_FWD_CLOSE;


struct MSG_CG_TRIGGER_ENTER_AREA_SYN : public MSG_BASE_FORWARD
{
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T3(151));
	PACKET_RND_SHRBLK64_CONT(int, m_iAreaID);
	PACKET_RND_SHRBLK64_END();
};


struct MSG_CG_TRIGGER_LEAVE_AREA_SYN : public MSG_BASE_FORWARD
{
	int							m_iAreaID;
};

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DO_ACTION_BRD )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T3(152));
	PACKET_RND_SHRBLK64_CONT(DWORD, m_dwTriggerID);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;

struct MSG_CG_TRIGGER_ACTION_NAK : public  MSG_BASE_FORWARD
{
	DWORD						m_dwErrorCode;
};

struct MSG_CG_TRIGGER_REFLECTDAMAGE_BRD : public MSG_BASE_FORWARD
{
	DWORD						m_dwObjectKey;
	DAMAGETYPE					m_wDamage;
	DWORD						m_dwHP;
};

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_AREADAMAGE_BRD )
	DWORD						m_dwObjectKey;
	DAMAGETYPE					m_wDamage;
	DWORD						m_dwHP;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_TRIGGER, CG_TRIGGER_PORTAL_ACK)
    enum eLeaveType { eLeaveType_Default, eLeaveType_Target, eLeaveType_Count };
    BYTE m_LeaveType; //eLeaveType
    //if(m_LeaveType == eLeaveType_Target) then goto target Village | Field
    CODETYPE m_MapCode;
    CODETYPE m_FieldCode;

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_WINDOW_CMD )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG32_T3(152));
	PACKET_RND_SHRBLK64_CONT(WORD, m_RewardCode);	// (RewardInfoList.txt) ���� ��ũ��Ʈ ��ȣ
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_SYN )
	WORD						m_Num;		//< (RewardInfoList.txt) ���� ��ũ��Ʈ ��ȣ
	//BYTE						m_Num;		//< ����ũ��Ʈ�� ��ȣ
	//BYTE						m_Type;		//< 0 or 1
	enum
	{
		ITEM1_SELECTED = (1<<0),
		ITEM2_SELECTED = (1<<1),
		ITEM3_SELECTED = (1<<2),
		ITEM4_SELECTED = (1<<3),
        ITEM5_SELECTED = (1<<4), //_NA_0_20101130_REWARD_ITEM_COUNT_INCREASE
	};
	BYTE						m_SelectedItem;		//< ������ �����۵�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_ACK )
	MONEY						m_Money;
	EXPTYPE						m_i64Exp;					// ���� ����ġ
	BYTE						m_byFatigueType;			//�Ƿε� Ÿ��.
#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
	bool						m_bGuildBounsExp;			//��庸�ʽ� ����ġ
#endif
    BOOLEAN                     m_bPortalMission;   // Portal���� �������� ����

	INVENTORY_TOTAL_INFO		m_ItemInfo;
	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_InvenCount - m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_NAK )
	DWORD					m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DONE_ACTION_INFO_CMD )
	enum { MAX_DONE_TRIGGER_ID_NUM = 100, };
	BYTE					m_byCount;
	DWORD					m_dwDoneTriggerID[MAX_DONE_TRIGGER_ID_NUM];
	int						GetSize()
	{
		return (sizeof(*this) - (MAX_DONE_TRIGGER_ID_NUM-m_byCount)*sizeof(DWORD));
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_PLAY_EVENT_CMD )
	int						m_nEventIndex;	// �÷����� �̺�Ʈ �ε���
PACKET_CG_FWD_CLOSE;

//__NA000357_070322_TRIGGER_SHOW_THE_FLOOR__
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_SHOW_THE_FLOOR_CMD )
	DWORD					m_dwPlayerKey;		// �ش� �׼��� �����ϴ� �÷��̾�, ��Ƽ�鵵 ���Ź�����, �� ��� �ش� �÷��̾��� �������� ó���Ѵ�.
	BYTE					m_byFloor;			// �� ���ΰ�?
	PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_PLAY_MUSIC_CMD )
	BYTE					m_byTarget;				// Ÿ�� ����(����,��Ƽ)
	INT						m_nMusCode;			// ���� ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DISPLAY_MSGBOX_CMD )
    INT						m_nTextID;			// �ؽ�Ʈ ����
PACKET_CG_FWD_CLOSE;

//_NA_0_20100222_UNIT_TRIGGERS_ADD
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD )
    DWORD					m_dwObjectKey;		// �ؽ�Ʈ ������ ��ġ (���� ��ǳ��)
    DWORD					m_dwMessageCode;	// ����� �޽����ڵ�
PACKET_CG_FWD_CLOSE;

//_NA_0_20100525_ADD_TIMER_ACTION_TRIGGER
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_CONTROL_TIMER_CMD )
	DWORD					    m_dwTime;				// �ð� (ms), ������ �ʱ�ð�, �ı��� ���� �ð�(delay)
	BYTE						m_byControlType;		// ���� eTIMER_CONTROL_TYPE
PACKET_CG_FWD_CLOSE;                                     // (0:������ ����(����), 1:������ ����(����), 2:�ı� )

//_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_CHANGE_RADAR_MAP_CMD )
    WzID sub_map_id;
PACKET_CG_FWD_CLOSE;

//////////////////////////////////////////////////////////////////////////
//__NA000405_070406_LAST_OBJSTATE_APPLICATION__
// �̰� CG_TRIGGER_DONE_ACTION_INFO_CMD ��ſ� �������� ����� �ߴ�.
// ������ �ΰ��� Ʈ���Ű� ���� ���濡�� �ݴ� �Ӽ��� ������ ���� ������ ������ ��� �� �� �ϳ���
// ���������� ������ �� ���� �Ǳ� �����̴�. ������, �̷��� �Ǹ� �ʹ� ���� ��Ŷ�� ������ �� ������ Ŀ����.
// �ϴ��� ����������� ���ܵΰ� �Ѿ��.
//	struct sOBJECT_STATE_INFO_BASE
//	{
//		// TriggerType ���� (������ �ʿ�� �ִ�)
//		//  Client (ProgramCommon/TriggerHeader.h) eTRIGGER_ACTION_TYPE
//		//  Server (GameServer/TriggerCommon.h) eACTION_TYPE
//		WORD					m_wTriggerType;
//		DWORD					m_dwObjectID;
//		INT						m_iValue;
//
//		sOBJECT_STATE_INFO_BASE() : m_wTriggerType(0), m_dwObjectID(0), m_iValue(0) {}
//		~sOBJECT_STATE_INFO_BASE() {}
//
//		inline WORD&			TRIGGER_TYPE() { return m_wTriggerType; }
//		inline DWORD&			OBJECT_ID() { return m_dwObjectID; }
//		inline INT&				VALUE() { return m_iValue; }
//	};
//
//	PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_LAST_OBJECT_STATE_CMD )
//		enum { MAX_LAST_OBJECT_STATE_INFO = 100, };
//		BYTE					m_byCounts;
//		sOBJECT_STATE_INFO_BASE	m_pObjStateInfo[MAX_LAST_OBJECT_STATE_INFO];
//
//		inline WORD				GetSize() { return (WORD)( (sizeof(*this) - sizeof(m_pObjStateInfo)) + (sizeof(sOBJECT_STATE_INFO_BASE) * m_byCounts) ); };
//	PACKET_CG_FWD_CLOSE;
//__NA000405_070406_LAST_OBJSTATE_APPLICATION__
//////////////////////////////////////////////////////////////////////////

//������������������������������������������������������������������������������������������������
// ĳ���� ����, �Ӽ�, ��ġ ���� ����
//������������������������������������������������������������������������������������������������

// AG_STATUS_LEVEL_UP_BRD ���������� �߰�
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_LEVEL_UP_BRD )
	DWORD						m_dwObjectKey;					// ����
	LEVELTYPE					m_Level;						// ���� ����
	WORD						m_wRemainStat;					// ���� ��������Ʈ
	WORD						m_wRemainSkill;					// ���� ��ų����Ʈ
	DWORD						m_dwCurHP;						// ���� HP(�������� HP, MP�� ä���ش�.)
	DWORD						m_dwCurMP;						// ���� MP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_LEVEL_UP_CMD )
	LEVELTYPE					m_Level;						// ���� ����
	WORD						m_wRemainStat;					// ���� ��������Ʈ
	WORD						m_wRemainSkill;					// ���� ��ų����Ʈ
	DWORD						m_dwCurHP;						// ���� HP(�������� HP, MP�� ä���ش�.)
	DWORD						m_dwCurMP;						// ���� MP
    DWORD						m_dwCurSD;						// ���� SD
PACKET_CG_FWD_CLOSE;

// HP ��ȭ
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_HP_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwTargetHP;					// ������
	DWORD						m_dwReserveHP;					// ������ HP
PACKET_CG_FWD_CLOSE;

// MP ��ȭ
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_MP_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwTargetMP;					// ������
PACKET_CG_FWD_CLOSE;

//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//FP��ȭ
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_FP_CMD )
    DWORD                           object_key;
    PLAYERFP						m_dwTargetFP;					// ������
PACKET_CG_FWD_CLOSE;
//FP����Ʈ ������ ���� ��Ŷ
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_FP_EFFECT_BRD )
    DWORD                           object_key;
    PLAYERFP                        current_fp;
PACKET_CG_FWD_CLOSE;

// HP, MP ��ȭ
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RECOVER_ATTR_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwTargetHP;					// ������
	DWORD						m_dwTargetMP;					// ������
PACKET_CG_FWD_CLOSE;

// SD ��ȭ(�ڽ�)
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_SD_CMD )
    DWORD						m_dwSD;					// ������
PACKET_CG_FWD_CLOSE;

// ��Ȱ ��û
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_SYN )
PACKET_CG_FWD_CLOSE;

// ��Ȱ ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_NAK )
	DWORD						m_dwObjectKey;					// ����
	BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// ��Ȱ ��ε�
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwHP;							// ������
	DWORD						m_dwMP;							// ������
	WzVector					m_wvCurPos;						// ��Ȱ ��ġ
	DWORD						m_dwReserveHP;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_TITLE_CHANGED_CMD )
	DWORD						m_PlayerTitle;					//< ePLAYER_TITLE ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_TITLE_CHANGED_BRD )
	DWORD						m_dwPlayerKey;
	DWORD						m_PlayerTitle;					//< ePLAYER_TITLE ����
PACKET_CG_FWD_CLOSE;

// ���
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_DEAD_BRD )
	DWORD						m_dwObjectKey;					// ����
	WzVector					m_wvCurPos;						// ���� ��ġ
	EXPTYPE						m_i64Exp;						// ���� ����ġ
PACKET_CG_FWD_CLOSE;

// ����ġ ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EXP_CMD )
	DWORD						m_dwTargetObjKey;				// Ÿ�� ������Ʈ Ű
	EXPTYPE						m_i64Exp;						// ���� ����ġ
	WORD						m_BonusRatio;					// �߰� �ۼ�Ʈ(0~500)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_GROUPEXP_CMD )
    DWORD						m_dwTargetObjKey;				// Ÿ�� ������Ʈ Ű
    EXPTYPE						m_i64Exp;						// ���� ����ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_GROUPMONEY_BRD )
    CHAR ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusMoney;
PACKET_CG_FWD_CLOSE;

// ��ġ ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ATTR_CHANGE_BRD )
	DWORD						m_dwObjKey;
	BYTE						m_AttrType;						// eATTR_TYPE
	DWORD						m_dwValue;						// value
PACKET_CG_FWD_CLOSE;


// ���� ��
#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION    

// eATTR_STR = 1, ����(��)
// eATTR_DEX = 2, ����(��ø)
// eATTR_VIT = 3, ����(ü��)
// eATTR_INT = 4, ����(����)
// eATTR_SPR = 5, ����(���ŷ�)
// eATTR_BASE_STAT_START_INDEX = eATTR_STR,
// eATTR_BASE_STAT_COUNT = eATTR_SPR + 1,

PACKET_CG_FWD_OPEN(CG_STATUS, CG_STATUS_STAT_APPLY_SYN)
    void Clear()
    {
        // CHANGES: f110305.3L
        ZeroMemory(stat_point, sizeof(stat_point));
    }
    //
    ushort stat_point[eATTR_BASE_STAT_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STATUS, CG_STATUS_STAT_APPLY_ACK)
    void Clear()
    {
        ZeroMemory(stat_point, sizeof(stat_point));
    }
    //
    uint object_key;
    uint remain_stat_point;
    ushort stat_point[eATTR_BASE_STAT_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STATUS, CG_STATUS_STAT_APPLY_NAK)
    uint result_code;
PACKET_CG_FWD_CLOSE;

#endif


// ���� ��
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_STAT_SELECT_SYN )
	BYTE						m_AttrType;						// eATTR_TYPE
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_STAT_SELECT_ACK )
	DWORD						m_dwObjKey;
	BYTE						m_AttrType;						// eATTR_TYPE
	DWORD						m_dwValue;						// value
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_STAT_SELECT_NAK )
	BYTE						m_AttrType;						// eATTR_TYPE
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// ĳ���� ���� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_SYN )
	BYTE						m_byConditionType;				// ĳ������ ���� Ÿ��(eCHAR_CONDITION)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_BRD )
	DWORD						m_dwObjectKey;					// ����
	BYTE						m_byConditionType;				// ĳ������ ���� Ÿ��(eCHAR_CONDITION)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_NAK )
	BYTE						m_byErrorCode;					// eCONDITION_RESULT
PACKET_CG_FWD_CLOSE;

// ���� �߰�
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ADD_BRD )
	DWORD						m_dwTargetKey;					// ���� �߰� ���
	WORD						m_wStatusCode;					// ���� �ڵ�
PACKET_CG_FWD_CLOSE;

// ���� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REMOVE_BRD )
	DWORD						m_dwTargetKey;					// ���� ���� ���
	DWORD						m_dwStatusCode;					// ���� �ڵ�
	int GetSize() { return sizeof(*this); };
PACKET_CG_FWD_CLOSE;

// ĳ���� �̸��
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_SYN )
	BYTE						m_byEmotionType;				// ĳ������ �̸�� Ÿ��(ȯȣ,�Ѽ�,����,����,���� ���)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_BRD )
	DWORD						m_dwObjectKey;					// ����
	BYTE						m_byEmotionType;				// ĳ������ �̸�� Ÿ��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_NAK )
	BYTE						m_byErrorCode;					// eSTATUS_RESULT
PACKET_CG_FWD_CLOSE;

// ���� ���� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_NPC_STATE_CHANGE_CMD )
	DWORD						m_dwObjectKey;					// ����
	BYTE						m_byState;						// NPC�� ���� Ÿ��(eNPC_STATE)
PACKET_CG_FWD_CLOSE;

// ������ �ɼ� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_OPTION_SYN )
	enum 
    { 
        eRENDER_HELMET_OPTION = 0,
        eRENDER_CAPE_OPTION,
        eRENDER_WING_OPTION,
        eRENDER_COSTUME_OPTION, // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    };

	BYTE						m_byRenderOptionType;			// ������ �ɼ� ����
	BYTE						m_byOptionLevel;				// �ɼǴܰ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_OPTION_BRD )
	DWORD						m_dwObjectKey;					// ����
	BYTE						m_byRenderOptionType;			// ������ �ɼ� ����
	BYTE						m_byOptionLevel;				// �ɼǴܰ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_MONSTER_ABILITY )
	float						m_fHPRatio;						// ���� HP ���
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_FATIGUE_CHANGE_CMD )
	WORD						m_wPlayingTime;					// ���� �÷��� �ð�(�д���)
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK )
	BYTE						m_byErrorCode;					// eSTATUS_RESULT ����	
PACKET_CG_FWD_CLOSE;

// �÷��̾� ��������
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ALL_STATEINFO_CMD )
    STATE_TOTAL_INFO  m_StateTotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_StateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_StateTotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

// designed by __NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
// �÷��̾� ETC��������
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ETC_STATEINFO_CMD )
    ETC_STATE_TOTAL_INFO m_stEtcStateTotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_stEtcStateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_stEtcStateTotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

// �巡�� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_START_DRAGON_FORMATION_BRD )
	DWORD						m_dwObjKey;
	SLOTCODE					m_SkillCode;
	WORD						m_wStatusCode;
	int							m_iStatusTime;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_END_DRAGON_FORMATION_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_END_DRAGON_FORMATION_BRD )
	DWORD						m_dwObjKey;
	SLOTCODE					m_SkillCode;
	WORD						m_wStatusCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_END_DRAGON_FORMATION_NAK )
	BYTE						m_byErrorCode;					// eSTATUS_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwHP;							// ������
	DWORD						m_dwMP;							// ������
	WzVector					m_wvCurPos;						// ��Ȱ ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_NAK )
	DWORD						m_dwErrorCode;					//< eSTATUS_RESULT
PACKET_CG_FWD_CLOSE;

//�Ӹ�, �ŷ� ����
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_INTERCEPTION_SYN )
	eINTERCEPTION_TYPE			m_byInterceptionCode;
	BOOL						m_bRequestFlag;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_INTERCEPTION_ACK )
	eINTERCEPTION_TYPE			m_byInterceptionCode;
	BOOL						m_bRequestFlag;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_INTERCEPTION_NAK )
	BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_FREE_INIT_STAT_UNDER_20_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_FREE_INIT_STAT_UNDER_20_ACK )
	BYTE						m_byFreeInitCount;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_FREE_INIT_STAT_UNDER_20_NAK )	
	BYTE						m_ResultCode;							//eSTATUS_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHAGNE_RENDER_EFFECT_SYN )
	enum eSYN_TYPE {
		EFFECT_ADD	= PROTOCOL_GENERATOR_STATUS_201,			// ȿ�� �߰�
		EFFECT_DEL	= PROTOCOL_GENERATOR_STATUS_202,			// ȿ�� ����
	};

	BYTE						m_SynType;
	DWORD						m_SpecialEffect;				// BitStream : eSPECIAL_RENDER_EFFECT

	//	<USAGE>
	//		MSG_CG_STATUS_RENDER_OPTION_SYN msgSYN;
	//		--- ���� �ڵ� ����
	//		// ���� ����
	//		//	msgSYN.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION;
	//		//	msgSYN.m_byOptionLevel = level;
	//		// ���� ����
	//		msgSYN.SetPartialInfo( MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION, level );
	//		--- Ư�� �߰� ���� �Է½�
	//		msgSYN.SetSpecialInfo( eSPECIAL_RENDER_EFFECT_SOLO_BADGE );
	//		SendPacket( &msgSYN, sizeof(msgSYN) );
	inline VOID					SetInfo( eSYN_TYPE cmd, DWORD specialEffect )
	{
		m_SynType = static_cast<uint8_t>(cmd);
		m_SpecialEffect = specialEffect;
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_STATUS, CG_STATUS_CHAGNE_RENDER_EFFECT_BRD, MSG_CG_STATUS_CHAGNE_RENDER_EFFECT_SYN )
	DWORD						m_ObjectKey;					// ����
	inline VOID					SetInfo( eSYN_TYPE cmd, DWORD specialEffect, DWORD target )
	{
		MSG_CG_STATUS_CHAGNE_RENDER_EFFECT_SYN::SetInfo( cmd, specialEffect );
		m_ObjectKey = target;
	}
PACKET_CG_INHERIT_CLOSE;


PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN )
PACKET_CG_FWD_CLOSE;

struct sMSG_RENDER_EFFECT_ITEM_LIST
{
	enum { MAX_EFFECT_ITEMS = 10, };	
	DWORD					m_PlayerKey; // _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET
	eSPECIAL_RENDER_EFFECT	m_BitRender;	// �������� �ƴ� Ư�� ����Ʈ
	BYTE					m_ItemCount;
	SLOTCODE				m_ItemCodes[MAX_EFFECT_ITEMS];

	inline WORD				GetSize()
	{
		return (WORD)( sizeof(*this)-( sizeof(SLOTCODE)*( MAX_EFFECT_ITEMS-m_ItemCount ) ) );
	}
};

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD )
	enum { _MAX_PLAYER_INFO_SIZE = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE, };	
	BYTE					m_Count;
	//	"sMSG_RENDER_EFFECT_ITEM_LIST" �� m_Count��ŭ �߰��ȴ�. �� ���� ���� variable size

	inline WORD				GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_STATUS, CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD, MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD )
PACKET_CG_INHERIT_CLOSE;


PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_DELETE_CHAR_STATE_SYN )
	eCHAR_STATE_TYPE		m_eCharStateType;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_DELETE_CHAR_STATE_ACK )
	BYTE					m_bResult;			// TRUE or FALSE
	eCHAR_STATE_TYPE		m_eCharStateType;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ETHER_ATTACK_SPEED_CMD )
	INT						m_AttackSpeedValue;
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// ��Ƽ���� ��Ŷ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CREATE_CMD )
	WORD						m_PartyKey;
	DWORD						m_MasterKey;
	DWORD						m_TargetKey;
	BYTE						m_DistributionType;		// eITEM_DISTRIBUTION_TYPE
	PARTY_MEMBER_TOTAL_INFO		m_MemberTotalInfo;

	int	GetSize()
	{
		return ( sizeof(MSG_CG_PARTY_CREATE_CMD) - (PARTY_MEMBER_TOTAL_INFO::MAX_SLOT_NUM-m_MemberTotalInfo.m_Count)*sizeof(PARTY_MEMBER_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_CG_FWD_CLOSE;

// ��Ƽ�� �Ѹ� ������m_byHPRatio �ٲ��� ��
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_LEVEL_UP_BRD )
    DWORD				m_dwObjKey;				// �������� ��Ƽ��
    LEVELTYPE			m_wLevel;				// ���� ����
	BYTE				m_byHPRatio;			// ���� HP ����
	BYTE				m_byMPRatio;			// ���� MP ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_HP_CHANGE_BRD )
    struct Node {
        WORD    objectKey;
        BYTE    ratio;
    };
    BYTE    m_NumberOfNodes;
    Node    m_Nodes[MAX_PARTYMEMBER_NUM];
    inline WORD GetSize() const
    {
        const WORD MinSize = sizeof(*this) - sizeof(m_Nodes);
        return MinSize + WORD(sizeof(Node) * m_NumberOfNodes);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_MP_CHANGE_BRD )
    struct Node {
        WORD    objectKey;
        BYTE    ratio;
    };
    BYTE    m_NumberOfNodes;
    Node    m_Nodes[MAX_PARTYMEMBER_NUM];
    inline WORD GetSize() const
    {
        const WORD MinSize = sizeof(*this) - sizeof(m_Nodes);
        return MinSize + WORD(sizeof(Node) * m_NumberOfNodes);
    }
PACKET_CG_FWD_CLOSE;

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SD_CHANGE_BRD )
    struct Node {
        WORD    object_key;
        BYTE    ratio;
    };
    BYTE    number_of_nodes;
    Node    nodes[MAX_PARTYMEMBER_NUM];
    inline WORD GetSize() const
    {
        const WORD MinSize = sizeof(*this) - sizeof(nodes);
        return MinSize + WORD(sizeof(Node) * number_of_nodes);
    }
PACKET_CG_FWD_CLOSE;
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_FP_CHANGE_BRD )
struct Node {
    WORD    object_key;
};
BYTE    number_of_nodes;
Node    nodes[MAX_PARTYMEMBER_NUM];
inline WORD GetSize() const
{
    const WORD MinSize = sizeof(*this) - sizeof(nodes);
    return MinSize + WORD(sizeof(Node) * number_of_nodes);
}
PACKET_CG_FWD_CLOSE;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_STATUS_CHANGE_BRD )
    DWORD				m_dwObjKey;				// ���°� ���� ��Ƽ��
	bool				m_bAdd;					// ���� �߰�/����
    WORD				m_wStateCode;			// ���� �ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_DEAD_BRD )
	DWORD				m_dwObjKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_RESURRECTION_BRD )
	DWORD				m_dwObjKey;
	BYTE				m_byHPRatio;			// ���� HP ����
	BYTE				m_byMPRatio;			// ���� MP ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD )
	DWORD				m_dwObjKey;
	BYTE				m_byChaoState;			// ī������(eCHAO_STATE)
PACKET_CG_FWD_CLOSE;

// ��Ƽ���� ��û
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_SYN )
	char m_TargetCharName[MAX_CHARNAME_LENGTH];// // ��Ƽ������ ��û�ϴ� ���
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_BRD )
	WORD				m_PartyKey;
	_PARTY_MEMBER_INFO	m_NewMemberInfo;
PACKET_CG_FWD_CLOSE;    

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_NAK )
	BYTE				m_byErrorCode;			// ��Ƽ���� ��û ���� ����(ePARTY_RESULT)
	ePLAYER_BEHAVE_STATE	m_ePlayerState;
PACKET_CG_FWD_CLOSE;

// ��Ƽ���� ����
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_SYN )
	char				m_szMasterName[MAX_CHARNAME_LENGTH];// ������ ��û�� ��Ƽ�� ID
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_NAK )
PACKET_CG_FWD_CLOSE;

// ��ƼŻ�� ��û
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_FORCED_EXPULSION_SYN )
	DWORD				m_dwObjKey;				// �����߹��ҷ��� ����� ������Ʈ Ű
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_BRD )
    BYTE                m_byReason;				// eREASON_LEAVE_PARTY
	DWORD				m_dwObjKey;				// Ż���� ��Ƽ���� ������Ʈ Ű
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_NAK )
	BYTE				m_byErrorCode;			// ��ƼŻ�� ���� ����(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// ��Ƽ�� ���� ��û
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_SYN )
	DWORD				m_dwObjKey;				// ������ ��� ������ƮŰ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_BRD )
	DWORD				m_dwObjKey;				// ����� ��Ƽ�� ������ƮŰ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_NAK )
	BYTE				m_byErrorCode;			// ��Ƽ�� ���� ���� ����(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// ��Ƽ��ü ��û
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_BRD )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_NAK )
	BYTE				m_byErrorCode;			// ��Ƽ��ü ���� ����(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// ��Ƽ Ÿ����
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_SYN )
	DWORD				m_dwObjectKey;			// Ÿ������ ���� ���� ������ƮŰ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_BRD )
	DWORD				m_dwObjectKey;			// Ÿ������ ������ ������ƮŰ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// ��Ƽ�� ��ó�� ����(���� ������ CG_MAP_TELEPORT_BRD�� ����)
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_WARP_REQ_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_WARP_REQ_NAK )
	BYTE				m_byErrorCode;			// ePARTY_RESULT
PACKET_CG_FWD_CLOSE;

// ��Ƽ�� ��ȯ(���� ��ȯ�� CG_MAP_TELEPORT_BRD�� ����)
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_REQ_SYN )
	DWORD				m_dwObjKey;				// ��ȯ�� ��Ƽ�� ������ƮŰ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_REQ_NAK )
	BYTE				m_byErrorCode;			// ePARTY_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_REQ_CMD )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_RES_SYN )
	BYTE				m_byResultCode;			// ePARTY_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_RES_NAK )
	BYTE				m_byErrorCode;			// ePARTY_RESULT
PACKET_CG_FWD_CLOSE;

// ��Ƽ �̴ϸ� ������ 
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MINIMAP_POINTING_SYN )
	float				m_fx;
	float				m_fy;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MINIMAP_POINTING_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MINIMAP_POINTING_BRD )
	float				m_fx;
	float				m_fy;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN )
	BYTE						m_byItemDistributionType;						
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_ITEM_DISTRIBUTION_CHANGE_NAK )
	BYTE						m_byErrorCode;					
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_ITEM_DISTRIBUTION_CHANGE_BRD )
	BYTE						m_byItemDistributionType;						
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK )
	enum eANSWER {
		ACCEPT_JOIN			= PROTOCOL_GENERATOR_091,	// ��Ƽ���� ���
		REJECT_JOIN			= PROTOCOL_GENERATOR_054,	// ��Ƽ���� ����
		THINKING_HUMMMM		= PROTOCOL_GENERATOR_034,	// ��Ƽ�� �����
	};
	BYTE						m_byAnswer;
	DWORD						m_dwPlayerKey;
	inline BYTE&				ANSWER()		{ return m_byAnswer; }
	inline DWORD&				PLAYER_KEY()	{ return m_dwPlayerKey; };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD )
	WORD						m_wPartyKey;
	inline WORD&				PARTY_KEY()		{ return m_wPartyKey; }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK )
	enum eANSWER {
		ACCEPT_JOIN			= PROTOCOL_GENERATOR_029,	// ��Ƽ���� OK
		REJECT_JOIN			= PROTOCOL_GENERATOR_147,	// ��Ƽ���� NO
	};
	BYTE						m_byAnswer;
	WORD						m_wPartyKey;
	inline BYTE&				ANSWER()		{ return m_byAnswer; }
	inline WORD&				PARTY_KEY()		{ return m_wPartyKey; }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD )
	sSTRING_BUFFER_CHAR_NAME	m_CharName;
	inline CHAR*				CHAR_NAME()		{ return m_CharName.GetBUFFER(); }
	inline VOID					CopyFrom( const TCHAR* ptszCharName )	// ����ȭ�� ���ڿ�
	{
		m_CharName.CopyFrom( ptszCharName );	// NULL ����
	}
	inline WORD					GetSize()
	{
		return sizeof(*this);
	}
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD )
	sSTRING_BUFFER_CHAR_NAME	m_CharName;
	inline CHAR*				CHAR_NAME()		{ return m_CharName.GetBUFFER(); }
	inline VOID					CopyFrom( TCHAR* ptszCharName )	// ����ȭ�� ���ڿ�
	{
		m_CharName.CopyFrom( ptszCharName );	// NULL ����
	}
	inline WORD					GetSize()
	{
		return sizeof(*this);
	}
PACKET_CG_FWD_CLOSE;

//{__NA001336_090514_PARTY_NAVI_SUPPORT__
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_NAVI_INFO_BRD)
    enum eOption {
        OPTION_NULL             = 0,
        OPTION_PARTY_NAVI       = 1 << 0,
        // 1 << 7�� Max
        MAX_OBJECT_ARRAY_SIZE   = MAX_PARTYMEMBER_NUM,
    };
    struct Node {
        WORD        objectKey;  // Player Object Key
        FIELDCODE   fieldCode;  // FIELDCODE
        short       posX, posY, posZ; // X,Y,Z��ǥ: �Ҽ��� ���� ���� ��ġ
        // NOTE: posZ is added to support _NA_0_20100722_AUTO_SYSTEM
    };

    BYTE m_CodeType;        // eOption
    BYTE m_NumberOfNodes;   // Max = MAX_OBJECT_ARRAY_SIZE
    Node m_Nodes[MAX_OBJECT_ARRAY_SIZE];

    inline WORD GetSize() const
    {
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Nodes));
        return MinSize + WORD(sizeof(Node)) * m_NumberOfNodes;
    }
PACKET_CG_FWD_CLOSE;
//__}

//////////////////////////////////////////////////////////////////////////
//_NA_20110630_BATTLE_ZONE_RENEWAL

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_BATTLE_ZONE_INFO_SYN)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_BATTLE_ZONE_INFO_ACK)
    BYTE all_page_num_;//����Ʈ ��ü ������ ��
    BYTE number_of_list_;//���� ���޵Ǵ� ����Ʈ ��
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_LIST];//����Ʈ ����
    WORD GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_LIST_SYN)
    BYTE request_page_;//��û ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_LIST_ACK)
    BYTE all_page_num_;//����Ʈ ��ü ������ ��
    BYTE number_of_list_;//���� ���޵Ǵ� ����Ʈ ��
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_LIST];//����Ʈ ����
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CREATE_ROOM_SYN)
    BasePartyRoomInfo party_room_info_;//��Ƽ�� �⺻ ���� ����ü
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CREATE_ROOM_ACK)
    RC::ePARTY_RESULT result_code_;//�����
    PartyRoomInfo party_room_info_;//������ �� ���� 
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_JOIN_ROOM_SYN)
    WORD join_party_key_;//Join�Ϸ��� ��Ƽ Ű��
    bool is_private_room_;//��й� ����
    TCHAR password_[MAX_ROOMPASSWORD_LENGTH+1];//��Ƽ�� ��� ��ȣ
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(password_);
        if (is_private_room_ == true)
        {
            return static_cast<uint16_t>(MinSize + sizeof(password_[0]) * _countof(password_));
        }
        else
        {
            return static_cast<uint16_t>(MinSize);
        }
    }
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_JOIN_ROOM_ACK)
    RC::ePARTY_RESULT result_code_;//��� ��
    PartyRoomInfo party_room_info_;//������ ���� ����
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        if (result_code_ == RC::RC_PARTY_SUCCESS)
        {
            return static_cast<uint16_t>(MinSize + sizeof(party_room_info_));
        }
        else
        {
            return static_cast<uint16_t>(MinSize);
        }
    }
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_JOIN_ROOM_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_LEAVE_ROOM_SYN)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_LEAVE_ROOM_ACK)
    RC::ePARTY_RESULT result_value_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_LEAVE_ROOM_BRD)
    DWORD member_key_;//��Ƽ���� ������ Object Key
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    BYTE reason_to_leave_;//eREASON_LEAVE_PARTY
#endif
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_SYN)
    BasePartyRoomInfo change_party_room_info_;//���� ��û �ɼ� ����ü
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_ACK)
    RC::ePARTY_RESULT result_code_;//��� ��
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_BRD)
    BasePartyRoomInfo change_party_room_info_;//���� �Ǵ� �ɼ� ����ü
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN)
    BYTE max_count_;//�����Ϸ��� Max count
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK)
    RC::ePARTY_RESULT result_code_;//��� �ڵ�
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD)
    BYTE max_count_;//����Ǵ� Max count
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_ROOM_SYN)
    BYTE number_of_mission_;//������ �̼��� ����
    MAPCODE map_code_[MAX_MISSION_COUNT];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(map_code_);
        return static_cast<uint16_t>(kMinSize + sizeof(map_code_[0]) * number_of_mission_);
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_ROOM_ACK)
    bool result_value_;//��� ��
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ASK_JOIN_ROOM_BRD)
    MAPCODE map_code_;//���� ������ �̼� �ڵ�
    WORD party_key_;//���� ������ ��Ƽ Ű
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_SYN)
    bool ready_state_;//�غ� ����
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_ACK)
    RC::ePARTY_RESULT result_code_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_BRD)
    DWORD member_key_;//�غ� ���� ������ ��û�� �ɸ��� guid
    bool ready_state_;//�غ� ����
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_PLAYER_MOVE_FIELD_BRD)
    DWORD member_key_;//�� �̵��� �� �ɸ��� guid
    MAPCODE map_code_;//�̵��ѵ� �ش� �ɸ����� ��ġ 
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHECK_ROOM_SYN)
    enum CheckType{
        CheckCreateType,
        CheckStanByType,
    };
    CheckType check_type_;//üũ Ÿ��
    BYTE select_mission_num_;//������ ���ϴ� �̼��� ����
    MAPCODE map_code_[MAX_MISSION_COUNT];//������ ���ϴ� �̼� �ڵ� �迭
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(map_code_);
        return static_cast<uint16_t>(kMinSize + sizeof(map_code_[0]) * select_mission_num_);
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHECK_ROOM_ACK)
    enum CheckType{
        CheckCreateType,
        CheckStanByType,
    };
    CheckType check_type_;//üũ Ÿ��
    WORD find_party_key_;//������ �´� ��Ƽ Ű
    MAPCODE map_code_;//ã�� ���� ���ڵ�
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ALLPLAYERS_CMD)
    enum { MAX_PLAYER_COUNT = 10, };
    BYTE m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD)
    enum { MAX_PLAYER_COUNT = MSG_CG_PARTY_ALLPLAYERS_CMD::MAX_PLAYER_COUNT, };
    BYTE m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD )
    enum { MAX_PLAYER_COUNT = MSG_CG_PARTY_ALLPLAYERS_CMD::MAX_PLAYER_COUNT, };	
    BYTE m_Count;
    inline WORD	GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_FORCE_LEAVE_SYN)
    DWORD member_key_;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_START_SYN)
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������    
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_START_ACK)
    RC::ePARTY_RESULT result_code_;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_SYN)
    BYTE select_team_;//���� 0:�ƹ��� �� ���� ���� ����, 1:ù��° �� 2:�ι�° ��
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_ACK)
    RC::ePARTY_RESULT result_code_;//������ ��� ��
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_BRD)
    DWORD member_key_;//���� ������ ������ object key
    BYTE select_team_;//������ ��
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_REQUEST_MEMBER_INFO_SYN)
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_CHARID_SYN)
    TCHAR find_char_name_[MAX_CHARNAME_LENGTH+1];
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_CHARID_ACK)
    RC::ePARTY_RESULT result_code_;//��� ��
    PartyRoomInfo party_room_info_;//������ ���� ����
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        if (result_code_ == RC::RC_PARTY_SUCCESS)
        {
            return static_cast<uint16_t>(MinSize + sizeof(party_room_info_));
        }
        else
        {
            return static_cast<uint16_t>(MinSize);
        }
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN)
    WORD find_room_key_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK)
RC::ePARTY_RESULT result_code_;//��� ��
PartyRoomInfo party_room_info_;//������ ���� ����
uint16_t GetSize() const
{
    const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
    if (result_code_ == RC::RC_PARTY_SUCCESS)
    {
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_));
    }
    else
    {
        return static_cast<uint16_t>(MinSize);
    }
}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_RESET_READY_STATE_CMD)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_PAUSE_SYN)
    bool set_value_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_PAUSE_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_REQUEST_RENDER_INFO_SYN)
    WORD request_party_key_;
    DWORD request_member_key_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_START_ROOM_COUNT_BRD)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_WAITING_ROOM_SYN)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_WAITING_ROOM_ACK)
    BYTE number_of_list_;//���� ���޵Ǵ� ����Ʈ ��
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_SEARCH_LIST];//����Ʈ ����
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN)
    MAPCODE request_map_code_;//��û �̼� �� �ڵ�
    PartyZoneType party_room_type_;//��û �̼� Ÿ��
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK)
    BYTE number_of_list_;//���� ���޵Ǵ� ����Ʈ ��
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_SEARCH_LIST];//����Ʈ ����
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_INVITE_PARTY_ROOM_REQ)
    WORD invite_party_key_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_INVITE_PARTY_ROOM_ANS)
    bool member_response_;
    WORD join_party_key_;
PACKET_CG_FWD_CLOSE;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN)
    WORD request_party_key_;
    char leave_member_name_[MAX_CHARNAME_LENGTH+1];
PACKET_CG_FWD_CLOSE;    
#endif

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_DICECAST_RESULT_CMD)
    DWORD item_object_key;
    SLOTCODE item_code;
    DWORD winner_key;
    _DICECAST_RESULT result[MAX_PARTYMEMBER_NUM];
PACKET_CG_FWD_CLOSE;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ON_ENTER_GAME_ROOM_BRD)
    bool zone_policy_item_obtain_all_party_member_;
PACKET_CG_FWD_CLOSE;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
//////////////////////////////////////////////////////////////////////////


//������������������������������������������������������������������������������������������������
// GM ��ɾ� ���� ��Ŷ
//������������������������������������������������������������������������������������������������

// ���� ���� �� ����
struct MSG_CG_GM_MONSTER_CREATE_SYN : public MSG_BASE_FORWARD
{
    MONSTERCODE			m_dwMonsterCode;
    BYTE				m_byMonsterCnt;
    float				m_fPos[3];			// �����ϰ� ���� ��ǥ
};

struct MSG_CG_GM_MONSTER_DESTROYALL_SYN : public MSG_BASE_FORWARD
{
};

struct MSG_CG_GM_MONSTER_DESTROYNEAR_SYN : public MSG_BASE_FORWARD
{
};

// ������ ���� ��ȯ
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_OBSERVER_SYN )
    BYTE				m_byObserverOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_OBSERVER_BRD )
	DWORD				m_dwObjectKey;
    BYTE				m_byObserverOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_OBSERVER_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// �������·� ��ȯ
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_SYN )
	BOOL				m_bUndeadOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_ACK )
	BOOL				m_bUndeadOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// ĳ������ ���� ��û
struct MSG_CG_GM_CHARINFO_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwUserID;								// CharName���� �˻��� ���� 0 ���� �����Ѵ�.
    char				m_szCharName[MAX_CHARNAME_LENGTH+1];
};

struct MSG_CG_GM_CHARINFO_ACK : public MSG_BASE_FORWARD
{
	char				m_pszAccountID[ID_MAX_LEN+1];			// ���� ���̵�.
	eCHAR_TYPE			m_eCharType;							// ĳ���� Ŭ����
    WORD				m_wLevel;								// ����
    MONEY				m_Money;								// ���� ��
	KEYTYPE				m_RoomKey;								// ��Ű
	CODETYPE			m_MapCode;								// ���ڵ�
	WzVector			m_vCurPos;								// ���� ��ġ
	char				m_szGuildName[MAX_GUILDNAME_LENGTH+1];	// ����̸�
};

struct MSG_CG_GM_CHARINFO_NAK : public MSG_BASE_FORWARD
{
	DWORD				m_dwErrorCode;		// eGM_RESULT
};

// �Ϲ� ������ ����
struct MSG_CG_GM_CREATE_ITEM_SYN : public MSG_BASE_FORWARD
{
    SLOTCODE			m_ItemCode;				// ������ �ڵ�
    DWORD				m_dwItemCount;			// ������ ����(1 ~ GM_MAX_CREATE_ITEM_NUM)
};


// ��þƮ ������ ����
struct MSG_CG_GM_CREATE_ENCHANT_ITEM_SYN : public MSG_BASE_FORWARD
{
    SLOTCODE			m_ItemCode;						// ������ �ڵ�
    DWORD				m_dwItemCount;					// ������ ����
    BYTE				m_byEnchant;					// ��þƮ���
};


// �����ݰ��� �ʵ� �������� ��� ����
struct MSG_CG_GM_DESTROY_ITEM_AROUND_SYN : public MSG_BASE_FORWARD
{
};


// �ʵ忡 �� ����
struct MSG_CG_GM_CREATE_MONEY_SYN : public MSG_BASE_FORWARD
{
    MONEY				m_Money;			// ������ �׼�(1 ~ GM_MAX_CREATE_MONEY)
};

// ���� ��
struct MSG_CG_GM_LEVEL_UP_SYN : public MSG_BASE_FORWARD
{
    WORD				m_wBonusLevel;		// ��½�ų ������ġ(���� ���� + ��½�ų ������ġ = ���� ����)
};

// ���� ��
struct MSG_CG_GM_STAT_UP_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwBonusStat;		// -GM_MAX_STAT_UP_VALUE ~ GM_MAX_STAT_UP_VALUE
};

struct MSG_CG_GM_STAT_UP_ACK : public MSG_BASE_FORWARD
{
    DWORD				m_dwRemainStat;		// ���� ��������Ʈ
};

// ��ų����Ʈ ��
struct MSG_CG_GM_SKILLPOINT_UP_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwBonusSkill;		// -GM_MAX_SKILLPOINT_UP_VALUE ~ GM_MAX_SKILLPOINT_UP_VALUE
};

struct MSG_CG_GM_SKILLPOINT_UP_ACK : public MSG_BASE_FORWARD
{

    DWORD				m_dwRemainSkill;		// ���� ��ų����Ʈ
};


//GM ��Ȱ SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_SYN )
PACKET_CG_FWD_CLOSE;

//GM ��Ȱ NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_NAK )
DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

//GM ��Ȱ BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwHP;							// ������
	DWORD						m_dwMP;							// ������
	WzVector					m_wvCurPos;						// ��Ȱ ��ġ
PACKET_CG_FWD_CLOSE;


//GM ȸ�� SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_SYN )
PACKET_CG_FWD_CLOSE;

//GM ȸ�� NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

//GM ȸ�� BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_BRD )
	DWORD						m_dwObjectKey;					// ����
	DWORD						m_dwHP;							// ������
	DWORD						m_dwMP;							// ������
PACKET_CG_FWD_CLOSE;
								 

//GM �ӵ� SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_SYN )
	BYTE						m_byMoveLevel;						// �ӵ����� 0~5���� ����.
PACKET_CG_FWD_CLOSE;

//GM �ӵ� BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_BRD )
	DWORD						m_dwObjectKey;						// GM�� objectKey
	BYTE						m_byMoveLevel;						// �ӵ�����
	FLOAT						m_fSpeedRatio;						// �ӵ��� ���� ��������						
PACKET_CG_FWD_CLOSE;

//GM �ӵ� NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//GM ���� SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ENTRANCE_SYN )			
PACKET_CG_FWD_CLOSE;

//GM ���� NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ENTRANCE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//GM �������� SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_FORCE_DISCONNECT_SYN )	
	DWORD						m_dwUserKey;
PACKET_CG_FWD_CLOSE;
//ACK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_FORCE_DISCONNECT_ACK )	
	DWORD						m_dwUserKey;
PACKET_CG_FWD_CLOSE;
//NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_FORCE_DISCONNECT_NAK )			
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//Ư�� ���� ����(������, ����ġ ����)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_DESTROYONE_SYN )
	DWORD						m_dwMonsterKey;
PACKET_CG_FWD_CLOSE;
//NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_DESTROYONE_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//Ư�� ���͸� ���������� ����(������, ����ġ ����)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_KILLONE_SYN )
	DWORD						m_dwMonsterKey;
PACKET_CG_FWD_CLOSE;
//NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_KILLONE_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RELOAD_DATA_CMD )
PACKET_CG_FWD_CLOSE;

// ������
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ROOMINFO_SYN )
	KEYTYPE				m_RoomKey;				// Ű���� 0�̸� ���� �Ҽӵ� ���� ������ �����ش�.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ROOMINFO_ACK )
	enum { _MAX_PLAYER_INFO_SIZE = 10 };
	char				m_szMasterName[MAX_CHARNAME_LENGTH+1];	// �� ������ �̸�
	BYTE				m_byCount;
	GM_CHARINFO			m_pPlayerInfo[_MAX_PLAYER_INFO_SIZE];
	int					GetSize()
	{
		return sizeof(*this) - (sizeof(GM_CHARINFO)*(_MAX_PLAYER_INFO_SIZE - m_byCount)); 
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ROOMINFO_NAK )
	DWORD				m_dwErrorCode;			// eGM_RESULT
PACKET_CG_FWD_CLOSE;

// ä������
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_ACK )
	DWORD				m_dwChannelUserNum;		// ���� ä�� �����ڼ�
	WORD				m_wHuntingRoomCount;	// ���÷� ����(�κ�����)
	WORD				m_wMissionRoomCount;	// �̼Ƿ� ����(�κ�����)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_NAK )
	DWORD				m_dwErrorCode;			// eGM_RESULT
PACKET_CG_FWD_CLOSE;

// ���� ���
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_SYN )
	char				m_szCharName[MAX_CHARNAME_LENGTH];	// �̵���ų ����̸�
	bool				m_bWatch;							// ������
	KEYTYPE				m_RoomKey;							// ���ȣ(������ ��� 0���� ����)
	CODETYPE			m_MapCode;							// ���ڵ�(������ �ƴ� ��� 0���� ����)
	WzVector			m_vDestPos;							// �̵��ϰ����ϴ� ������ ��ǥ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_NAK )
	BYTE				m_byErrorCode;						// eGM_RESULT
PACKET_CG_FWD_CLOSE;

// ��Ʈ�� ��ɾ�(�����ʿ��� �Ľ��ؼ� ó���ϴ� ��ɾ�)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_STRING_CMD_SYN )
	enum { MAX_STRING_CMD_LENGTH = 100 };
	char				m_szStringCmd[MAX_STRING_CMD_LENGTH+1];	// ��Ʈ�� ��ɾ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_STRING_CMD_ACK )
	enum { MAX_STRING_CMD_LENGTH = 100 };
	char				m_szCommand[MAX_STRING_CMD_LENGTH+1];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_STRING_CMD_NAK )
	BYTE				m_byErrorCode;								// eGM_RESULT
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ACCOUNT_MODE_ACK )
	BOOL				m_bTurnOn;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANGE_ROOMTITLE_ACK )
	KEYTYPE				m_RoomKey;
	TCHAR				m_szRoomTitle[MAX_ROOMTITLE_LENGTH];		// �ٲ� ������
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANGE_ROOMTITLE_NAK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

//���� GM��ɾ�
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_GO_NAK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

//���� GM��ɾ�
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_TRACKING_NAK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_VERSION_ACK )
	BYTE				m_byErrorCode;	
	BYTE				m_byVersionType;
	BYTE				m_byScriptCode;
	TCHAR				m_szVersion[MAX_VERSION_SIZE];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_PICKUP_ACK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SERVER_TIME_CMD )
	DWORD64				m_CurTime;
PACKET_CG_FWD_CLOSE;
//������������������������������������������������������������������������������������������������
// ��Ÿ ��Ŷ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_DEBUGINFO_CMD )
	enum { _MAX_CHATMSG_SIZE = 255 };
	DWORD						m_dwObjectKey;
	BYTE						m_byMsgLength;
	char						m_pszChatMsg[ _MAX_CHATMSG_SIZE+1 ];
    uint16_t GetSize() {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszChatMsg);
        // NOTE: the usage case is not standard.
        // this null-terminated process is based on received message process case
        int len = min(m_byMsgLength, _countof(m_pszChatMsg) - 1);
        m_pszChatMsg[len] = '\0';
        m_byMsgLength = static_cast<BYTE>(len + 1);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszChatMsg[0]) * m_byMsgLength);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_SUMMON_RETURN_BRD )
	DWORD				m_dwObjectKey;
	BYTE				m_bIsSummon;		// 1�̸� ��ȯ 0�̸� ����
	BYTE				m_byFullness;
	BYTE				m_Intimacy;
	BYTE				m_bSetPetName;
	TCHAR				m_PetName[MAX_PET_NAME_LENGTH];
	WORD				m_wPetIndex;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_STATUS_CHANGE_BRD )
	DWORD				m_dwObjectKey;
	POSTYPE				m_ItemPos;			// �κ��丮�� ������� ��ġ
	BYTE				m_bIsFullness;		// 1�̸� ������ 0�̸� ģ�е�
	BYTE				m_byValue;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_RETURN_CMD )
	POSTYPE				m_ItemPos;			// �κ��丮�� ������� ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_SET_NAME_BRD)
	DWORD				m_dwObjectKey;
	TCHAR				m_PetName[MAX_PET_NAME_LENGTH];
PACKET_CG_FWD_CLOSE;



#ifdef _NA_0_20110329_GAMEOPTION_SETTING
PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_LOAD_SYN)
    GameOption::Type option_type_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_LOAD_ACK)
    GameOption game_option_;
    //
    uint16_t GetSize() const
    {   // f110609.8L, fixed the invalid size calcuation spot.
        const size_t kMinSize = sizeof(*this) - sizeof(game_option_);
        return static_cast<uint16_t>(kMinSize + game_option_.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_LOAD_NAK)
    int error_code_;
    uint option_type_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_SAVE_SYN)
    GameOption game_option_;
    //
    uint16_t GetSize() const
    {   // f110609.8L, fixed the invalid size calcuation spot.
        const size_t kMinSize = sizeof(*this) - sizeof(game_option_);
        return static_cast<uint16_t>(kMinSize + game_option_.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_SAVE_ACK)
    GameOption::Type option_type_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GAMEOPTION_SAVE_NAK)
    int error_code_;
    GameOption::Type option_type_;
PACKET_CG_FWD_CLOSE;
#endif //_NA_0_20110329_GAMEOPTION_SETTING

//------------------------------------------------------------------------------
//_NA_000000_20120410_SPEEDHACK
PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_MEMORY_HACK_CMD)
PACKET_CG_FWD_CLOSE;

//!~ _NA004034_20120102_GUILD_POINT_SYSTEM
PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GUILD_RANKINGINFO_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GUILD_RANKINGINFO_ACK)
    INT guild_ranking_info_count_;
    GUILD_RANKING_INFO guild_ranking_info_[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];

    void init()
    {
        guild_ranking_info_count_ = 0;
        memset(guild_ranking_info_, 0, sizeof(guild_ranking_info_));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GUILDINFO_SYN)
    GUILDGUID guid_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GUILDINFO_ACK)
    GUILDGUID guid_;
    GAME_GUILD_INFO game_guild_info_;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    GUILD_INFO_EXTRA            guild_info_extra_;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eGUILD_JOIN_REQUEST request_state_;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void init()
    {
        guid_ = 0;
        memset(&game_guild_info_, 0, sizeof(game_guild_info_));
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        memset(&guild_info_extra_, 0, sizeof(guild_info_extra_));
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
    }
PACKET_CG_FWD_CLOSE;
//~! _NA004034_20120102_GUILD_POINT_SYSTEM

//������������������������������������������������������������������������������������������������
// PVP ���� ��Ŷ
//������������������������������������������������������������������������������������������������


PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_INFO_CMD )	
	struct {
		BYTE				m_Team	:3;			//< ���� �� ����
		BYTE				m_Count :5;			//< �ٸ� ����� �� ���� ��
	};
	struct PVP_TEAM_INFO
	{
		DWORD				m_dwPlayerKey;
		BYTE				m_Team;
	} m_Info[MAX_PARTYMEMBER_NUM];
	int GetSize()
	{
		return sizeof(*this) - sizeof(PVP_TEAM_INFO)*(MAX_PARTYMEMBER_NUM-m_Count);
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_INFO_BRD )
	DWORD				m_dwPlayerKey;
	BYTE				m_Team;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_PRESS_KEY_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_PRESS_KEY_ACK )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_PRESS_KEY_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCHLESS_MODE_START_CMD )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_FIRST_START_CMD )		//< ó�� pvp
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_START_CMD )			//< �ڱ��ڽ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_START_BRD )			//< �ٸ� ���
	DWORD				m_dwPlayerKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_SCORE_CMD )
	DWORD				m_Score;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_RESULT_BRD )
	DWORD				m_KillerKey;
	DWORD				m_DierKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_END_BRD )
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// �������� ���� ��Ŷ
//������������������������������������������������������������������������������������������������

// ���ο� ��Ƽ ����
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_ENTER_NEW_PARTY_BRD )
PACKET_CG_FWD_CLOSE;

// ���� ����
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_START_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// �¸�
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_WIN_COMPETITION_BRD )
	WORD					m_wWinPoint;			// �¸� ����Ʈ
	EXPTYPE					m_i64Exp;				// ���� ����ġ
	MONEY					m_CurMoney;				// ���� ����
	WORD					m_PlusPVPPoint;			// ȹ�� PVP ����Ʈ
	INVENTORY_TOTAL_INFO	m_ItemInfo;				// ȹ�� ������
	int GetSize()
	{
		return (sizeof(MSG_CG_CHUNTING_WIN_COMPETITION_BRD) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX));
	}
PACKET_CG_FWD_CLOSE;

// �й�
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_LOSE_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// ���º�
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_TIE_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// ų����Ʈ ����
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_UPDATE_KILLPOINT_BRD )
	WORD					m_wTeamKey;
	WORD					m_wKillPoint;			// ���� ų����Ʈ
PACKET_CG_FWD_CLOSE;

// ���ǰ��� ����
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_UPDATE_POTIONCOUNT_BRD )
	BYTE					m_byAvailablePotion;	// �̿밡���� ���ǰ���
PACKET_CG_FWD_CLOSE;

// ������
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_TEAM_INFO_CMD )
	WORD					m_wTeamKey;
PACKET_CG_FWD_CLOSE;

// ���� �������� ��û
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_POINT_INFO_SYN )
PACKET_CG_FWD_CLOSE;

struct CompetitionPoint
{
	DWORD					m_dwObjKey;
	WORD					m_wKillPoint;			// ���� ȹ���� ų����Ʈ
	BYTE					m_byDieNum;				// ���� Ƚ��
	BYTE					m_byCombo;				// �޺�(����Ƚ��)
};

PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_POINT_INFO_ACK )
	BYTE					m_byCount;
	CompetitionPoint		m_Point[CHUNTING_MAX_MEMBER_NUM*2];
	int						GetSize()
	{
		return ( sizeof(MSG_CG_CHUNTING_POINT_INFO_ACK) - (CHUNTING_MAX_MEMBER_NUM*2 - m_byCount)*sizeof(CompetitionPoint) );
	}
PACKET_CG_FWD_CLOSE;


//������������������������������������������������������������������������������������������������
// CG_SUMMON
//������������������������������������������������������������������������������������������������


// ��������
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_SYN )
	DWORD				m_dwTargetKey;					// Ÿ�� Ű(���������� ��쿡�� ��ȿ��)
	BYTE				m_byAttackPropensity	: 2;	// �������� ����
	BYTE				m_byCommand				: 6;	// ���Ÿ��(eSUMMON_COMMAND)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_ACK )
	BYTE				m_byCommand;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_NAK )
	BYTE				m_byCommand;
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// ��ų����
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_SKILL_ACTION_SYN )
	DWORD				m_dwSummonedObjKey;			// ��ȯü ������ƮŰ
	SLOTCODE			m_SkillCode;				// Skill �ڵ�
	DWORD				m_dwClientSerial;			// Skill �̺�Ʈ�� ���� ���� ��ȣ
	WzVector			m_wvCurPos;					// Ŭ���̾�Ʈ�� �ִ� ��ȯü�� ���� ��ǥ
	WzVector			m_wvDestPos;				// ��ȯü�� ������ ��ǥ
	DWORD				m_dwMainTargetKey;			// �ְ��ݴ���� ������Ʈ Ű
	WzVector			m_wvMainTargetPos;			// �ְ��ݴ���� ��ġ

	BYTE				m_byAttackPropensity;			// �������� ����

	int GetSize() { return sizeof(MSG_CG_SUMMON_SKILL_ACTION_SYN); };
PACKET_CG_FWD_CLOSE;

// ��ȯü ����
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_CURRENT_MP_CMD )
	DWORD				m_dwSummonedObjKey;			// ��ȯü ������ƮŰ
	DWORD				m_dwMaxMP;
	DWORD				m_dwMP;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_CURRENT_HP_CMD )
	DWORD				m_dwSummonedObjKey;
	DWORD				m_dwMaxHP;
	DWORD				m_dwHP;
	DWORD				m_dwSpeedRatio;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_CURRENT_HP_BRD )
	DWORD				m_dwSummonedObjKey;
	DWORD				m_dwMaxHP;
	DWORD				m_dwHP;
	DWORD				m_dwSpeedRatio;
PACKET_CG_FWD_CLOSE;

// _NA001385_20090924_DOMINATION_SKILL
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_CRYSTALWARP_COMMAND_SYN )
    WzVector		    m_wvPos;				    // Create  ��ɿ��� ���� ��ġ
    DWORD				m_dwCrystalWarpObjKey;	    // Ÿ�� Ű (����� �޴� ũ����Ż������ƮŰ, ������ == 0)
    BYTE				m_byCommand;			    // ���Ÿ��(eCRYSTALWARP_COMMAND)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_CRYSTALWARP_COMMAND_BRD )
    BYTE				m_byCommand;
    DWORD				m_dwObjKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_CRYSTALWARP_COMMAND_NAK )
    BYTE				m_byCommand;
    BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// CG_GUILD ��� ���� ��Ŷ
//������������������������������������������������������������������������������������������������

// ��� ����
// ���� : 
// 1. ���� ���� 100 �̻�
// 2. ��� â�� �ڱ� XX���� �ҿ�
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_SYN )	
	char	m_tszGuildName[MAX_GUILDNAME_LENGTH];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_ACK )
	GUILDGUID m_GuildGuid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_BRD )
	PLAYERKEY	m_MasterPlayerKey;							//< ��� â���� �÷��̾� Ű
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_NAK )
	BYTE	m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// ��� ��ü
// ����常 ����
// �̱�(������,��,�����̵�)����
// ����->�Ϲ������� ��޺���
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_BRD )
	PLAYERKEY	m_MasterPlayerKey;							//< ��� �ػ��� �÷��̾� Ű
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_NAK )
	BYTE	m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_FAILED_CMD )		//< ��� ������ ���� �߽��ϴ�.
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;



//��� �ʴ�
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_SYN )					
	char m_TargetCharName[MAX_CHARNAME_LENGTH];		//< �ʴ��� �÷��̾� ĳ���� �̸�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_CMD )	
	CHARGUID m_OperUserKey;										//�ʴ� ��û�� ĳ���� �ε���
	char m_OperCharName[MAX_CHARNAME_LENGTH];  //�ʴ� ��û�� �÷��̾� �̸�
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    char m_OperGuildName[MAX_GUILDNAME_LENGTH + 1];
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_NAK )						
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//��� �ʴ� ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_ANSWER_SYN )	
	CHARGUID m_OperUserKey;
	BOOL			m_bInveite;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_ANSWER_CMD )				
	PLAYERKEY		m_TargetObjKey;
	BOOL			m_bInveite;
	char m_TargetCharName[MAX_CHARNAME_LENGTH];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_ANSWER_NAK )			
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//��� ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_SYN )
	PLAYERKEY m_TargetObjKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_CMD )
	DWORD m_GuildGuid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_BRD )
	GUILD_MEMBER_INFO	m_MemberInfo;
PACKET_CG_FWD_CLOSE; //�����鿡�� �뺸

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// ��� ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_TOTALINFO_REQ_SYN )
PACKET_CG_FWD_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_TOTALINFO_REQ_ACK )
	GUILD_INFO					m_GuildInfo;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    GUILD_INFO_EXTRA            guild_info_extra_;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GUILD_JOIN_REQUEST_TOTAL_INFO     m_GuildJoinRequestTotalInfo;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	GUILD_CORPS_TOTAL_INFO		m_CorpsTotalInfo;
	GUILD_MEMBER_TOTAL_INFO		m_MemberTotalInfo;

	int		GetSize()
	{
		return ( sizeof(MSG_CG_GUILD_TOTALINFO_REQ_ACK) - ( sizeof(GUILD_MEMBER_TOTAL_INFO) - m_MemberTotalInfo.GetSize() ) );
	}
PACKET_CG_FWD_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_TOTALINFO_REQ_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_GM_GUILD_POINT_BRD )
	GUILDGUID					m_GuildGuid;
	UPTYPE						m_UP;
	GPTYPE						m_GP;
PACKET_CG_FWD_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_GM_GUILD_POINT_NAK )
	BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE; 



PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_WITHDRAW_SYN )			//< ��� Ż��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_WITHDRAW_ACK )
	BYTE	m_byPenaltyType;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_WITHDRAW_BRD )
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH];
	BYTE				m_byPenaltyType;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_WITHDRAW_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// ������� ��� ��� ���� Ż��
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_SYN )
	char				m_ptszKickCharName[MAX_CHARNAME_LENGTH+1];		// Ż���ų ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_CMD )
PACKET_CG_FWD_CLOSE;

// ����忡 ���� �߹�Ǿ����� �˸�(Ż�� ��� �ش� ����� �¶����� ��쿡�� ����)
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_BRD )
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];			// ������� ĳ�����̸�
PACKET_CG_FWD_CLOSE;

// ���� Ż�� ����(����忡�Ը� ����)
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_NAK )
	char				m_ptszKickCharName[MAX_CHARNAME_LENGTH+1];
	DWORD				m_dwErrorCode;					// �����ڵ�(ResultCode)
PACKET_CG_FWD_CLOSE;

///////////////
// ��� ����Ʈ ���� ��Ŷ��

// ��忡 UP �峳
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_SYN )
UPTYPE		m_DonateUP;			// �峳�� UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_ACK )
	UPTYPE		m_RemainUP;			// �峳�ϰ� ���� UP
	UPTYPE		m_GuildUP;			// �峳 ����� �þ ����� �� UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_BRD )
	UPTYPE		m_GuildUP;			// �峳 ����� �þ ����� �� UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_CLOSE;

// ������� UP->GP�� ��ȯ
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_SYN )
	GPTYPE		m_ConvertGP;		// ��ȯ�� GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_ACK )
	UPTYPE		m_RemainUP;			// ��ȯ�ϰ� ���� UP
	GPTYPE		m_NowGP;			// ��ȯ ����� �þ �� GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_BRD )
	UPTYPE		m_RemainUP;			// ��ȯ�ϰ� ���� UP
	GPTYPE		m_NowGP;			// ��ȯ ����� �þ �� GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RANKUP_BRD )
	BYTE		m_byNowLevel;		// ��ũ���� ����
	GPTYPE		m_RemainGP;			// ���� GP
	UPTYPE		m_RemainUP;			// ���� UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_ADD_INFO_SYN	)
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_ADD_INFO_ACK )
	BYTE				m_byRelCnt;
	BYTE				m_byCorpsCnt;
	BASE_GUILD_ADD_INFO	m_AddInfo;
    uint16_t GetSize() const {
        // TODO: f110506.6L, calculates accurate size
        return static_cast<uint16_t>( sizeof(MSG_CG_GUILD_ADD_INFO_ACK)
            - ((MAX_ALLIENCE_GUILD_NUM + MAX_HOSTILITY_GUILD_NUM + MAX_ONE_SIDE_HOSTILITY_NUM) - m_byRelCnt) * sizeof(GUILD_RELATION));			
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_ADD_INFO_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_CLOSE;

// ��� ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_SYN )
	eGUILD_RELATION m_eType;
	CHAR m_szRelationGuildName[MAX_GUILDNAME_LENGTH + 1];
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_NAK )
	BYTE	 m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_REQUEST_CMD )
	eGUILD_RELATION	m_eType;
	CHARGUID m_RequestUserKey;
	CHAR m_RequestGuildName[MAX_GUILDNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_ANSWER_SYN )
	eGUILD_ANSWER	m_eAnswerType;
	eGUILD_RELATION	m_eRelationType;
	CHARGUID m_RequestUserKey;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_REJECT_ACK )
	char		m_szRelGuildName[MAX_GUILDNAME_LENGTH + 1];
    eGUILD_RELATION	m_eRelationType;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_DELETE_SYN )
	eGUILD_RELATION	m_eType;
	GUILDGUID m_eDeleteGuildGuid;
	CHAR m_szGuildName[MAX_GUILDNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_DELETE_NAK )
	BYTE m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_BOTH_AGAINST_SYN )
	GUILDGUID m_BothAgainstGuildGuid;
	CHAR m_szRelationGuildName[MAX_GUILDNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_BRD )
	DWORD m_dwPlayerKey;
	GUILD_RELATION m_stRELGuild;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_DELETE_BRD )
	DWORD m_dwPlayerKey;
	GUILDGUID m_RelGuildGuid;
	eGUILD_RELATION	m_eRelType;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_BOTH_AGAINST_BRD )
	DWORD m_dwPlayerKey;
	GUILDGUID	m_BothGuildGuid;
	eGUILD_RELATION	m_eDELType;
	eGUILD_RELATION	m_eMOVType;
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 m_RelationApplyTime;
    DWORD64 m_RelationChangeTime;
#endif
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RELATION_MOVE_BRD)
	DWORD m_dwPlayerKey;
	GUILDGUID m_GuildGuid;
	eGUILD_RELATION	m_eDELType;
	eGUILD_RELATION	m_eMOVType;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

//��� ���� ����		
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_CORPS_SYN )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
PACKET_CG_CLOSE; 

//PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_CORPS_ACK )
//	BYTE		m_byCorpsNum;
//PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_CORPS_BRD )
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_CORPS_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

//��� ���� �ػ�
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_CORPS_SYN )
	BYTE		m_byCorpsNum;
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_CORPS_BRD )
	BYTE		m_byCorpsNum;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_CORPS_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 


//����ִ� ���� ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_EMPTY_COMMANDER_INFO_SYN )
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_EMPTY_COMMANDER_INFO_ACK )
	BYTE		m_byEmptyCommanderCount;
	BYTE		m_byCorpsNum[MAX_GUILD_CORPS_NUM];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_byCorpsNum);
        return static_cast<uint16_t>(kMinSize + sizeof(m_byCorpsNum[0]) * m_byEmptyCommanderCount);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_EMPTY_COMMANDER_INFO_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

//������ ����
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_COMMANDER_SYN )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
	BYTE		m_byCorpsNum;
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_COMMANDER_BRD )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
	BYTE		m_byCorpsNum;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_COMMANDER_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

//�����̵�
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_CORPS_SYN )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
	BYTE		m_byCorpsNum;
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_CORPS_BRD )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
	BYTE		m_byCorpsNum;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_CORPS_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

//��� ���� ķ�� â�� 
//���� ķ���� �����常�� ���� �ִ� ����̴�.
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_REAR_CAMP_SYN )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_REAR_CAMP_ACK )
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_REAR_CAMP_BRD )
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_REAR_CAMP_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

//��� ���� ķ�� �ػ�
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_REAR_CAMP_SYN )
	BYTE	m_byCorpsNum;
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_REAR_CAMP_ACK )
	BYTE	m_byCorpsNum;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_REAR_CAMP_NAK )
	BYTE	m_byErrorCode;
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_REAR_CAMP_BRD )
	BYTE	m_byCorpsNum;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 


// �α���/�ƿ�/������ 
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_LOGIN_CHAR_BRD )
	TCHAR			m_szCharName[MAX_CHARNAME_LENGTH];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_LOGIN_CHAR_CMD )
	GUILD_INFO					m_GuildInfo;
	GUILD_CORPS_TOTAL_INFO		m_CorpsTotalInfo;
	GUILD_MEMBER_INFO			m_MemberInfo;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_LOGOUT_CHAR_BRD )
	TCHAR			m_szCharName[MAX_CHARNAME_LENGTH];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHAR_LEVELUP_BRD )
	TCHAR			m_szCharName[MAX_CHARNAME_LENGTH];
	LEVELTYPE		m_Level;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_CHAOSTATE_BRD )
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH];
	BYTE				m_byChaoState;			// ī�� ����(eCHAO_STATE)		
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

/////////// ��� ��å ���� ////////////////
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_DUTY_SYN )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byDuty;
	BYTE		m_byCorpsNum;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_DUTY_ACK )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byDuty;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_DUTY_BRD )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byDuty;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CHANGE_DUTY_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;


///////// ��� ���� ���� ��� ��û ////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_LIST_SYN )
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_LIST_ACK )
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_LIST_NAK )
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

///////// ��� ���� ���� ////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_SYN)
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_ACK)
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_CMD)
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RIGHT_DELEGATION_NAK)
	BYTE		m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

/////// ����� ���� //////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_CHANGE_MASTER_SYN)
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_CLOSE; 

//PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_CHANGE_MASTER_ACK)
//	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
//PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_CHANGE_MASTER_BRD)
	TCHAR						m_szOldMasterName[MAX_CHARNAME_LENGTH + 1];
	TCHAR						m_szNewMasterName[MAX_CHARNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_CHANGE_MASTER_NAK)
	BYTE						m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_LEAVE_BRD)
	PLAYERKEY					m_LeaverObjectKey;							//< ��� Ż���� �÷��̾� Ű
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 


////////////////��� �ػ�/���� ////////////////

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_DESTROY_WAIT_ACK)
	GUILDGUID	m_GuildGuid;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RECOVERY_GUILD_SYN)
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RECOVERY_GUILD_ACK)
	GUILDGUID	m_GuildGuid;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_RECOVERY_GUILD_NAK)
	BYTE	m_byErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_DESTROY_WAIT_BRD)
    DWORD m_dwPlayerKey;
    GUILDGUID m_RelGuildGuid;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

////////////////��� ��ũ ���////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_SYN)
	SLOTIDX m_SlotIndex;
	POSTYPE m_SlotPos;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_ACK)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_BRD)
	DWORD m_dwPlayerKey;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD)
	DWORD m_dwPlayerKey;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

////////////////��� ��ũ ����////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_SYN)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_ACK)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_BRD)
	DWORD m_dwPlayerKey;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD)
	DWORD m_dwPlayerKey;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

////////////////��� ��ũ ���� ����////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_REGISTER_NAK)
	BYTE m_bErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_GUILDMARK_DELETE_NAK)
	BYTE m_bErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

////////////////��� â�� ����////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_OPEN_SYN)
	BYTE m_NPCCode;
	GUILDGUID m_GuildIndex;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_OPEN_ACK)

	enum { _MAX_BINARY_SIZE = 
	      sizeof(RENDER_ITEMSTREAMEX) * (GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM),
	};

	WORD	m_Size;
	MONEY	m_WarehouseMoney;
	BYTE	m_SlotTabNum;
	BYTE	m_BitStreamBuffer[_MAX_BINARY_SIZE];
	int GetSize()
	{
		return ( sizeof(*this) - (sizeof(m_BitStreamBuffer)-m_Size) );
	}	
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_OPEN_NAK)
	DWORD m_bErrorCode;
PACKET_CG_CLOSE;

////////////////��� â�� �ݱ�////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_SYN)
	BYTE m_NPCCode;
	GUILDGUID m_GuildIndex;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_ACK)
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_NAK)
	DWORD m_bErrorCode;
PACKET_CG_CLOSE;

////////////////��� â�� �� �����///////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_MONEY_SYN)
	enum { PUTMONEY = 0, GETMONEY = 1, };
	BYTE	 m_byType;
	MONEY	m_Money;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_MONEY_ACK)
	MONEY	m_InventoryMoney;
	MONEY	m_WarehouseMoney;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_MONEY_NAK)
	DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

////////////////��� â�� ���� Ȯ��///////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_TAPEX_ACK)
	BYTE m_SlotTabNum;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_TAPEX_NAK)
BYTE m_bErrorCode;
PACKET_CG_CLOSE;

//{__NA000000_20081106_GUILD_NAME_CHANGE
///////////////���� ���� ///////////////////////////

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_SYN)
SLOTIDX						m_atIndex;
POSTYPE						m_atPos;
TCHAR						m_tszGuildName[MAX_GUILDNAME_LENGTH+1];		// ������ ����
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_BRD)
    TCHAR m_tszGuildName[MAX_GUILDNAME_LENGTH + 1]; // ����� ����
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_SECTOR_BRD)
    DWORD m_dwPlayerKey; // ���� ����
    TCHAR m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_RELATION_BRD)
    BYTE m_RelationType;
    GUILDGUID m_GuildGuid; // ���� ���
    TCHAR m_tszGuildName[MAX_GUILDNAME_LENGTH + 1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_NAK)
    DWORD m_bErrorCode;
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;
//}__NA000000_20081106_GUILD_NAME_CHANGE

#ifdef _NA_003923_20120130_GUILD_RENEWAL
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ)
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS)
    BYTE log_category_;//GuildInGameLogCategory
    BYTE log_count_;
    GuildInGameLogData log_data_[MAX_IN_GAME_GUILD_LOG];
    WORD GetSize() const
    {
        const size_t min_size = sizeof(*this) - sizeof(log_data_);
        return static_cast<WORD>(min_size + (sizeof(GuildInGameLogData) * log_count_));
    }
PACKET_CG_CLOSE;
#endif

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
///////////////���Ұ�  ///////////////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_INTRODUCTION_SAVE_SYN)
    GUILD_INTRODUCTIONS guild_introductions_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_INTRODUCTION_SAVE_ACK)
    DWORD result_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_INTRODUCTION_CHANGE_BRD)
    CHAR introduction_strings_[MAX_GUILD_INTRODUCTION_LENGTH+1];
PACKET_CG_FWD_CLOSE;
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
///////////////��� ���� ��û  ///////////////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_SYN)
    GUILDGUID               guild_guid;
    eGUILD_JOIN_REQUEST     request_type;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_ACK)
    GUILDGUID   guild_guid;
    DWORD       result_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_BRD)
    GUILD_JOIN_REQUEST_INFO request_info;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_APPROVE_SYN)
    CHARGUID    char_guid;
    TCHAR       char_name[MAX_CHARNAME_LENGTH + 1];
    BYTE        char_class;
    LEVELTYPE   char_level;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_REJECT_SYN)
    CHARGUID char_guid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_REJECT_ACK)
    void Init()
    {
        memset(&guild_name, 0, sizeof(guild_name));
    }
    TCHAR     guild_name[MAX_GUILDNAME_LENGTH + 1];
    GUILDGUID guild_guid;
    DWORD     result_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_JOIN_REQUEST_REJECT_BRD)
    CHARGUID char_guid;
PACKET_CG_FWD_CLOSE;

///////////////��� �ü�  ///////////////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_UPGRADE_SYN)
    FCODE facility_code;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_UPGRADE_ACK)
    FCODE facility_code;
    DWORD result;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_UPGRADE_BRD)
    GUILD_FACILITY_INFO facility_info;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_ACTIVE_SYN)
    FCODE facility_code;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_ACTIVE_ACK)
    FCODE facility_code;
    DWORD result;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_FACILITY_TOTALINFO_ACK)
    GUILD_FACILITY_TOTAL_INFO guild_facility_total_info;
PACKET_CG_FWD_CLOSE;

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_SONNENSCHEIN_UPDATE_CMD)
    DWORD m_dwPlayerKey;
    BYTE sonnenschein_phase;

    uint16_t GetSize() const {return static_cast<uint16_t>(sizeof(*this));}
PACKET_CG_FWD_CLOSE;
#endif //_NA_008334_20150608_SONNENSCHEIN
//������������������������������������������������������������������������������������������������
// CG_EVENT �̺�Ʈ �κ��丮 ���� ��Ŷ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_SELECT_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_SELECT_ACK )
    EVENT_ITEM_TOTAL_INFO m_TotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_SELECT_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_MOVE_ITEM_TO_INVEN_SYN )
	POSTYPE				m_ItemPos;								//< �κ��丮�� �ű���� �ϴ� �������� �̺�Ʈ �κ��丮 ��ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_MOVE_ITEM_TO_INVEN_ACK )
    JUST_INVENTORY_TOTAL_INFO m_TotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_MOVE_ITEM_TO_INVEN_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// �̺�Ʈ ������ ��� ���� ��ȸ ��û
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_SYN )
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// �̺�Ʈ ������ ��� ����
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_ACK )
	WORD					m_wEventNum;						// �̺�Ʈ ����
	EVENT_STATUS			m_EventStatus[_MAX_EVENT_ROW_NUM];	//

	int GetSize()
	{
		//ASSERT( m_wEventNum < _MAX_EVENT_ROW_NUM );
		return (int)( ( sizeof(MSG_CG_EVENT_ITEM_STATUS_ACK) - sizeof(m_EventStatus) ) + sizeof(EVENT_STATUS)*m_wEventNum );
	}
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// ��� ���� ��ȸ ����
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_NAK )
	CODETYPE			m_dwEventID;
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// �̺�Ʈ ������ ��� ��û

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_SYN )
	DWORD				m_dwEventID;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// �̺�Ʈ ������ ��� ����(��ǰ ����)
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_ACK )
	DWORD				m_dwEventID;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// �̺�Ʈ ������ ��� ����
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_NAK )
	//enum	// errorCode;
	//{
	//	_ERR_DEFAULT,
	//	_ERR_INVALID_EVENT,		// �̺�Ʈ ���� �ƴϴ�
	//	_ERR_ITEM_NUM_OVERFLOW,	// �̺�Ʈ ������ ��� ���� ������ �Ѿ���
	//	_ERR_NOTEXISTITEM,		// �̺�Ʈ�� ������ �������� ����
	//	_ERR_CANT_OBTAIN,		// ���� �������� ���� ���� ����
	//};
	CODETYPE				m_dwEventID;
	DWORD					m_dwErrorCode;
	INVENTORY_TOTAL_INFO	m_TotalInfo;		// ���ư� ������ �ٽ� ����

	inline WORD				GetSize()
	{ return (WORD)( (sizeof(*this)-sizeof(INVENTORY_TOTAL_INFO)) + m_TotalInfo.GetSize() ); }
PACKET_CG_FWD_CLOSE;

// �̺�Ʈ ������ ���� ����(W->C) - ������ ����
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REWARD_CMD )
	CODETYPE				m_dwEventID;
	MONEY					m_Money;
	INVENTORY_TOTAL_INFO	m_TotalInfo;
	inline WORD				GetSize()
	{ return (WORD)( (sizeof(*this)-sizeof(INVENTORY_TOTAL_INFO)) + m_TotalInfo.GetSize() ); }
PACKET_CG_FWD_CLOSE;


// �̺�Ʈ�� �����ð� ���� - ó�� ĳ���� ������ �� ����
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_CMD )
	int		m_EventUseTime;		// �̺�Ʈ ���� �ð�
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_QUERY_EXIST_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_QUERY_EXIST_ACK )
	enum eEXIST_RESULT {
		EMPTY		= PROTOCOL_GENERATOR_EVENT_081,
		EXIST		= PROTOCOL_GENERATOR_EVENT_096,
		DISABLED	= PROTOCOL_GENERATOR_EVENT_100,
		// DISABLED - ���� �̺�Ʈ �κ��丮 ���� �÷θ� ������ ���
		// ����� CG_EVENT_SELECT_SYN �� �����ϰ� ó���ϸ� �ȴ�.
	};
	BYTE			m_bExist;
PACKET_CG_FWD_CLOSE;



	PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_PERIOD_SYN )
	PACKET_CG_FWD_CLOSE;				// �����ð� ��û

	PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_PERIOD_ACK )
		time_t time;

		explicit MSG_CG_EVENT_TIME_PERIOD_ACK(const time_t& t)
			: time(t)
		{
			m_byCategory = CG_EVENT; 
			m_byProtocol = CG_EVENT_TIME_PERIOD_ACK;
		}
	PACKET_CG_FWD_CLOSE;

	PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_PERIOD_NAK )
		BYTE				m_byErrorCode;
	PACKET_CG_FWD_CLOSE;				// �����ð� ��û ����


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_REQUEST_ITEM_SYN )
	CODETYPE				m_EventID;
	BYTE					m_RequestItemIndex;	// (0,2) <- EVENT_INFO::m_dwRewardIndex�� �ε���

	inline VOID				SetInfo( const CODETYPE eventID, const BYTE requestItemIndex )
	{ m_EventID = eventID; m_RequestItemIndex = requestItemIndex; }
PACKET_CG_FWD_CLOSE;

struct MSG_CG_EVENT_REQUEST_ITEM_BADGEMAN_SYN : MSG_CG_EVENT_REQUEST_ITEM_SYN
{
	enum eSELECT_TYPE { SOLO_BADGE = 0, COUPLE_BADGE, INVALID_INDEX };	//m_RequestItemIndex�� ���� ��
};

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_REQUEST_ITEM_NAK )
	CODETYPE				m_EventID;
	DWORD					m_ErrorCode;

	inline VOID				SetInfo( const CODETYPE eventID, DWORD errorCode ) { m_EventID=eventID; m_ErrorCode = errorCode; }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_DONATION_HEIM_SYN )
	CODETYPE				m_EventID;
	MONEY					m_DonatedHeims;

	inline VOID				SetInfo( const CODETYPE eventID, const MONEY donatedHeims )
	{ m_EventID = eventID; m_DonatedHeims = donatedHeims; }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_DONATION_HEIM_ACK )
	//enum eRESULT {
	//	ERR_DEFAULT					= 0,		// �⺻ ����,
	//	ERR_INSUFFICIENT_HEIM		,			// ����Ϸ��� �ݾ��� ������ ���� ���ϴ�.
	//	ERR_DONATION_HEIM_LIMIT		,			// �ּ� ����ؾ� �� �ݾ׿��� ���ڸ���.
	//	ERR_CANT_OBTAIN_ITEM		,			// ���� ���� �� �ִµ�, �������� ȹ������ ���ϴ�.
	//	SUCCESS						,			// �Ϲ� ���� (���� ���� ������ EVENT_REWARD_ITEM���� ���۵�)
	//};
	CODETYPE				m_EventID;
	BYTE					m_ErrorCode;
	MONEY					m_TotalHeims;	// ���ݱ��� ������ �׼�

	inline VOID				SetInfo( const CODETYPE eventID, RC::eEVENT_RESULT errorCode ) { m_EventID=eventID; m_ErrorCode = (BYTE)errorCode; }
	inline VOID				SetTotalHeim( const MONEY totalHeim ) { m_TotalHeims = totalHeim; }
	inline WORD				GetSize()
	{ return (RC::RC_EVENT_SUCCESS == m_ErrorCode) ? sizeof(*this) : sizeof(*this)-sizeof(m_TotalHeims); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_BUFF_EVENT_SYN )
	// Ư�� Ÿ���� �����ϱ�� �Ѵ�. Ŭ�� EventID�� �����Ϸ��� Ŭ�󿡼� �ð��� ����ؼ� ��û�ؾ� �Ѵ�.
	// �������� �ȵư�����, ������ NPC ID�� �����ϹǷ� NPC ID�� ������ ���·� ����.
	MONSTERCODE				m_NpcCode;

	inline VOID				SetInfo( const MONSTERCODE npcCode ) { m_NpcCode = npcCode; }
	inline WORD				GetSize()	{ return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_BUFF_EVENT_ACK )
	// m_ResultCode = RC::eEVENT_RESULT
	//	-	RC::RC_EVENT_SUCCESS			(����)-���� ���� �ο� ��Ŷ�� �߰��� �� ���� �ִ�. �̰� ���� ������.
	//	-	RC::RC_EVENT_FAILED				(����:��Ÿ ������ ���н�)
	//	-	RC::RC_EVENT_CANT_BUFF_GRANTED	(�ű�:�̹� �ο��Ǿ� ���� ���)
	//	-	RC::RC_EVENT_INVALID_EVENT		(����)
	//	-	RC::RC_EVENT_UNQUALIFIED		(�ű�:������ ���� �� ����. �ڰ��� �ȵǼ�) + �߰� ���� ���� �ʿ�

	// m_AddinInfo = eRET_UNQUALIFIED
	enum eRET_UNQUALIFIED
	{
		RUC_EMPTY				,	// �ڰ� ��� ���ѿ� �ɸ��� ����.
		RUC_LEVEL_LOWERBOUND	,	// LEVEL ���� ����
		RUC_LEVEL_UPPERBOUND	,	// LEVEL ���� ����
		RUC_CLASS_MISMATCH		,	// ĳ���� Ŭ���� (RESERVED)
		RUC_MORAL				,	// ĳ���� ���� (RESERVED)
	};

	BYTE						m_ResultCode;
	union {
		// EventID�� �����ϴ� ���
		struct {
			CODETYPE			m_EventID;		// Ȱ��ȭ�� EventID
			BYTE				m_AddinInfo;	// RESERVED
		} RESULT_SUCCESS;
		struct {
		} RESULT_FAIL;							// �̺�Ʈ ���� �ƴϰų� ���� ������ ������ ���н�
		struct {
			CODETYPE			m_EventID;		// Ȱ��ȭ�� EventID
		} RESULT_CANT_BUFF_GRANTED;
		struct {
			CODETYPE			m_EventID;		// Ȱ��ȭ�� EventID
			BYTE				m_AddinInfo;	// eRET_UNQUALIFIED �߰� ���� ����
		} RESULT_UNQUALIFIED;
	};

	// ���ʰ� ���� : �߰� ������ �ش� �ڵ忡 �°� ������ ��.
	// m_ResultCode�� ���� �Է��ϴ� ���ڰ� �ٸ���.
	inline VOID				SetInfo_SUCCESS( const CODETYPE eventID, const BYTE reserved = 0 )
	{ m_ResultCode = RC::RC_EVENT_SUCCESS; RESULT_SUCCESS.m_EventID = eventID; RESULT_SUCCESS.m_AddinInfo = reserved; }
	inline VOID				SetInfo_FAIL()
	{ m_ResultCode = RC::RC_EVENT_FAILED; }
	inline VOID				SetInfo_INVALID()
	{ m_ResultCode = RC::RC_EVENT_INVALID_EVENT; }
	inline VOID				SetInfo_CANT_BUFF_GRANTED( const CODETYPE eventID )
	{ m_ResultCode = RC::RC_EVENT_CANT_BUFF_GRANTED; RESULT_CANT_BUFF_GRANTED.m_EventID = eventID; }
	inline VOID				SetInfo_UNQUALIFIED( const CODETYPE eventID, const eRET_UNQUALIFIED reason )
	{ m_ResultCode = RC::RC_EVENT_UNQUALIFIED; RESULT_UNQUALIFIED.m_EventID = eventID; RESULT_UNQUALIFIED.m_AddinInfo = (BYTE)reason; }

	inline WORD				GetSize()
	{
		const WORD MIN_SIZE = sizeof(MSG_BASE_FORWARD)+sizeof(BYTE);
		const WORD FAIL_SIZE = MIN_SIZE+sizeof(RESULT_FAIL);
		switch( m_ResultCode )
		{
		case RC::RC_EVENT_SUCCESS:				return MIN_SIZE+sizeof(RESULT_SUCCESS);
		case RC::RC_EVENT_FAILED:
		case RC::RC_EVENT_INVALID_EVENT:		return FAIL_SIZE;
		case RC::RC_EVENT_CANT_BUFF_GRANTED:	return MIN_SIZE+sizeof(RESULT_CANT_BUFF_GRANTED);
		case RC::RC_EVENT_UNQUALIFIED:			return MIN_SIZE+sizeof(RESULT_UNQUALIFIED);
		}
	#ifdef _DEBUG
		__debugbreak();
	#endif
		SetInfo_FAIL();
		return FAIL_SIZE;
	}
PACKET_CG_FWD_CLOSE;

// CG_EVENT_INVITATE~ protocols are used by _KR_0_20091021_FIRENDINVITATION_EXTEND

// ģ�� �ʴ� �̺�Ʈ�� ���� ���� ��� ��û
PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK )
enum {DB_FAIL = 1, DBPROXY_FAIL = 2};
BYTE		byErrorCode;  //1: DB �ý��� ����, 2: DB Proxy ���� ���� �Ұ�
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK )
    BYTE                        m_Count;            //���� ������ ��� ����
    InvitateFriendRewardInfo    RewardInfo[MAX_FRIENDINVITATE_REWARD_NUM];
    int GetSize()
    {
        return ( sizeof(MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK) - ((MAX_FRIENDINVITATE_REWARD_NUM - m_Count) * sizeof(InvitateFriendRewardInfo)) );
    }
PACKET_CG_CLOSE;
// END ģ�� �ʴ� �̺�Ʈ�� ���� ���� ��� ��û

// ģ�� �ʴ� �̺�Ʈ�� ���� ���� �ޱ� ��û
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_SYN )
    CHARGUID    FriendGUID;//ģ�� GUID
    BYTE        ReWardType;//���� Ÿ��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_ACK )
    INVENTORY_TOTAL_INFO		m_TotalInfo;
    int GetSize()
    {
        return (sizeof(MSG_CG_EVENT_INVITATEFRIEND_REWARD_ACK) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_TotalInfo.m_InvenCount-m_TotalInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX));
    }   
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_NAK )
    enum {DB_FAIL = 1, DBPROXY_FAIL = 2, RECEV_REWARDED = 3, NOTOBTAIN_INVEN = 4};
    BYTE		m_byErrorCode;
/*
1: DB �ý��� ����
2: DB PROXY ���� ���� �ҷ�
3: �̹� ���� �޾���
4: �κ��� ������ ����
*/
PACKET_CG_FWD_CLOSE;
// END ģ�� �ʴ� �̺�Ʈ�� ���� ���� �ޱ� ��û

// ģ���� ���� (C->A->D)
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_SYN )
    CHARGUID    FriendGUID; //��ȯ�� ģ���� GUID
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_NAK )
    enum {
        SUCCEEDED = 0,
        DB_FAIL,	// EVENT START
        DBPROXY_FAIL		,	// EVENT END
        OTHER_CHNNEL,
        RECALLTIME_SHORTAGE,
        OTHERZONE_ME,
        OTHERZONE_FRIEND,
        TERMINATE_EVENT,
        NOTFOUND_USER,
        SENDFAIL,
        FRIEND_REJECT,
        OTHERREQUESTEXIST,
    };
    BYTE		m_byErrorCode;  //�����ڵ�
    DWORD64     m_LeftTime;     //�����ڵ尡 RECALLTIME_SHORTAGE ��� ��ȯ�ϱ���� ���� �ð� YYYYMMDDHHmmSS �ѽð� ���Ҵٸ� 10000
/*
1: DB �ý��� ����
2: DB PROXY ���� ���� �ҷ�
3: �ٸ� ä�ο� ����
4: ��ȯ �ܿ��ð� ����
5: ��ȯ�� �Ұ����� ��ġ�� ����(����)
6: ��ȯ�� �Ұ����� ��ġ�� ����(��ȯ�Ϸ��� �ְ�)
7: �̺�Ʈ ���� �Ǿ���
8: ��ȯ�� ���� ã���� ����
9: ģ������ ���� ����
10: ģ���� ����
*/
PACKET_CG_FWD_CLOSE;

// ģ���� ���ݿ� ���Ұ����� �ľ��Ѵ�.
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_SYN )
    CHARGUID    FriendGUID;                         //��ȯ ��û�� ĳ�� GUID
    TCHAR		FriendName[MAX_CHARNAME_LENGTH+1];  //��ȯ ��û�� ĳ�� �̸�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_ACK )
    CHARGUID    FriendGUID;                         //��ȯ ��û�� ĳ�� GUID
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK )
    CHARGUID    FriendGUID;                         //��ȯ ��û�� ĳ�� GUID
PACKET_CG_FWD_CLOSE;
//

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
//���� ������ ��� ���� �������� �������� ����
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_DOMINATION_REWARD_CMD)
    MEMOGUID memo_guid_;// �޸�Guid
    TCHAR memo_subject_[MAX_MEMO_LENGTH + 1];//���� ���� ����
PACKET_CG_FWD_CLOSE;
//���� �ζ� ��÷���� �������� ����
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_LOTTO_REWARD_CMD)
    MEMOGUID memo_guid_;// �޸�Guid
    INT64 lotto_entry_index_;//�ζ� �ε���
PACKET_CG_FWD_CLOSE;
//�ش� �޸� ���� ���� ��� ���� �������� �˷��ش�.
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_ITEM_REWARD_RESULT_CMD)
    MEMOGUID memo_guid_;//�޸�Guid
    bool result_value_;//���� ����� true / false
PACKET_CG_FWD_CLOSE;
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ)
    MEMOGUID memo_guid_;
    TCHAR memo_contents_[MAX_MEMO_LENGTH + 1];//���� �α׸� ���� �ڷ�� ���
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_SYSTEMMEMO_ITEM_REWARD_ANS)
    MEMOGUID memo_guid_;
    RC::eITEM_RESULT result_value_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD )
	CODETYPE				m_EventID;
	enum eTIME_CONTROL {
		TC_START	,	// EVENT START
		TC_END		,	// EVENT END
	};
	BYTE					m_TimeControl;

	inline VOID				SetInfo( const CODETYPE eventID, const eTIME_CONTROL timeCtrl )
	{ m_EventID = eventID; m_TimeControl = (BYTE)timeCtrl; }
PACKET_CG_FWD_CLOSE;

//_NA_007919_20141211_ADD_GMCOMMAND_EVENT
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_PEAKTIME_NOTIFY_BRD)
    ePEAKTIME_EVENT_NOTIFY notify_type;
    //����
    BYTE start_day;     // ������ (day)
    DWORD start_time;   // ���۽ð� (hhmm)

    //����
    BYTE end_day;       // ������ (day)
    DWORD end_time;     // ���ᳯ¥ (hhmm)

    float exp_ratio;     // ����ġ���� (100%���� -> 1)

    MSG_CG_EVENT_PEAKTIME_NOTIFY_BRD(ePEAKTIME_EVENT_NOTIFY n_type, BYTE s_day, BYTE s_time, BYTE e_day, BYTE e_time, WORD ratio)
    : notify_type(n_type), start_day(s_day), start_time(s_time), end_day(e_day), end_time(e_time), exp_ratio(ratio) {}
PACKET_CG_FWD_CLOSE;

//_NA_007919_20141211_ADD_GMCOMMAND_EVENT
PACKET_CG_FWD_OPEN(CG_EVENT, CG_GMEVENT_PEAKTIME_EVENTLIST_CMD)
    static const int event_data_max_count = 10;

    struct PeakTimeEventData
    {
        __time64_t start_date;
        __time64_t end_date;
        FLOAT      exp_ratio;
    };

    PeakTimeEventData event_data[event_data_max_count];
    int data_count;
PACKET_CG_FWD_CLOSE;

//_NA_007919_20141211_ADD_GMCOMMAND_EVENT
//ó�� ���� �÷��̾�� �������� �̺�Ʈ�� ����ġ������ �������ش�
PACKET_CG_FWD_OPEN(CG_EVENT, CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD)
    FLOAT      exp_ratio;
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// CG_QUEST ����Ʈ ���� ��Ŷ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_STREAM_CMD )
	int GetSize() { return sizeof(*this); }
	//QUEST_COMPL_TOTAL_INFO�� GetSize()��ŭ
	//QUEST_PROGR_TOTAL_INFO�� GetSize()��ŭ
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ACCEPT_SYN )
	WORD		m_QuestCode;
    //{__NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT
	BYTE		m_byIsItemAccept;				// NPC ���� : 0 ,	������ ���� : 1
	POSTYPE		m_QuestItemPos;
    //}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ACCEPT_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ACCEPT_NAK )
	BYTE		m_byErrorCode;
    //{__NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT
	BYTE		m_byIsItemResult;				// Quest result : 0 ,	ItemResult : 1
    //}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ACCEPT_CMD )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_COMPLETE_SYN )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_COMPLETE_ACK )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_COMPLETE_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_INCOMPLETE_SYN )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_INCOMPLETE_ACK )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_INCOMPLETE_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_CHANGEofCLASS_CMD )
	PROPERTY<BYTE>		m_ChangeOfClassCode;	// eCHAREX_TYPE
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_SYN )
	WORD		m_QuestCode;
	enum
	{
		ITEM1_SELECTED = (1<<0),
		ITEM2_SELECTED = (1<<1),
		ITEM3_SELECTED = (1<<2),
		ITEM4_SELECTED = (1<<3),
        ITEM5_SELECTED = (1<<4), //_NA_0_20101130_REWARD_ITEM_COUNT_INCREASE
	};
	BYTE						m_SelectedItem;		//< ������ �����۵�
PACKET_CG_FWD_CLOSE;

// ���� ���� (GM)
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_CMD )
    EXPTYPE						m_i64Exp;					// ���� ����ġ
    MONEY						m_Money;
    SLOTCODE					m_SkillCode;
    BYTE						m_byFatigueType;			//�Ƿε� Ÿ��.
    WORD						m_wQuestCode;
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    WORD                        m_wAccumulatePoint;
#endif

    int GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// ����ó���� �̼��� ����ũ��Ʈ�� ó���� ���ɼ��� ����
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_ACK )
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    int                         m_BunusExpRatio;            //�߰� ����ġ ����
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	EXPTYPE						m_i64Exp;					// ���� ����ġ
	MONEY						m_Money;
	SLOTCODE					m_SkillCode;
	BYTE						m_byFatigueType;			//�Ƿε� Ÿ��.
	WORD						m_wQuestCode;
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    WORD                        m_wAccumulatePoint;
#endif

	INVENTORY_TOTAL_INFO		m_ItemInfo;

	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_InvenCount - m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_FAILED_SYN )
	WORD		m_QuestCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_FAILED_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_FAILED_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ABANDON_SYN )
	WORD		m_QuestCode;
	BYTE		m_Force;		//1: ���� ����[Ÿ������Ʈ �ð� �Ϸ�], 0: �������� 
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ABANDON_ACK )
	BYTE		m_Force;		
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ABANDON_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_MONSTER_KILL_CMD )
	WORD		m_QuestCode;
	WORD		m_MonsterCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_MONSTER_KILL_GRADE_CMD )
    WORD		m_QuestCode;
    BYTE		m_MonsterGrade;
    WORD		m_MonsterLevel;
PACKET_CG_FWD_CLOSE;

//{__NA001277_090205_NAVI_SYSTEM_SUPPORT__
PACKET_CG_FWD_OPEN(CG_QUEST, CG_QUEST_OBJECT_NAVI_POS_CMD)
    enum eOption {
        OPTION_NULL         = 0,
        OPERATION_ALL_INFO  = 1 << 0,   // ��ü ���� ���� ��� (EnterZone)
        OPERATION_ADD_INFO  = 1 << 1,   // ����Ʈ �����ÿ� ���� ���� ���� + a

        CODETYPE_NPC        = 1 << 4,   //
        // 1 << 7�� Max
        MAX_OBJECT_ARRAY_SIZE = 100,
    };
    struct Node {
        WORD questCode; // QCODE
        WORD npcCode;
        short posX; // �Ҽ��� ���� ���� ��ġ
        short posY; // �Ҽ��� ���� ���� ��ġ
    };

    BYTE m_CodeType;    // eOption
    BYTE m_NumberOfNodes;
    Node m_Nodes[MAX_OBJECT_ARRAY_SIZE];

    inline WORD GetSize() const
    {
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Nodes));
        return MinSize + WORD(sizeof(Node)) * m_NumberOfNodes;
    }
PACKET_CG_FWD_CLOSE;
//}

PACKET_CG_FWD_OPEN(CG_QUEST, CG_QUEST_ITEM_OBTAIN_CMD)
    WORD m_QuestCode;
    JUST_INVENTORY_TOTAL_INFO m_TotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_QUEST, CG_QUEST_ITEM_DELETE_CMD)
    WORD m_QuestCode;
    INVEN_DELETE_TOTAL_INFO m_TotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_EVENT_AREA_SYN )
	WORD		m_QuestCode;
	DWORD		m_AreaID;
    WzVector	m_wvCurPos;				// Ŭ���̾�Ʈ�� �ִ� �÷��̾��� ���� ��ǥ (������)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_EVENT_AREA_ACK )
    DWORD		m_AreaID;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_EVENT_AREA_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_DAILYQUEST_PACKET )
BYTE packet_type;
PACKET_CG_FWD_CLOSE;
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_MISSION_CLEAR_CMD )
    WORD quest_code;
    MAPCODE map_code;
PACKET_CG_FWD_CLOSE;

#endif //_NA_006607_20130402_ADD_QUESTTYPE
//������������������������������������������������������������������������������������������������
// CG_MISSION �̼� ���� ��Ŷ
//������������������������������������������������������������������������������������������������
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_STREAM_CMD)
	MISSION_TOTAL_INFO m_TotalInfo;
	int GetSize()
	{
		return (sizeof(MSG_BASE_FORWARD)+m_TotalInfo.GetSize());
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RECORD_ELAPSEDTIME_CMD)
	DWORD			m_MissionElapsedTimePerSecond;				//< �̼� ����� �ð�(�ʴ���)
	DWORD			m_MissionBestClearTimePerSecond;			//< ������ �̼� �ְ� ���
PACKET_CG_FWD_CLOSE;

//// this packet will be deprecated!!!!!!!
//PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RECORD_CLEARTIME_CMD)
//	DWORD			m_MissionClearTime;			//< �̼� Ŭ������ �ð�(�ʴ���)
//PACKET_CG_FWD_CLOSE;

#ifdef _NA_008012_20150130_RANKING_SYSTEM
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_INFO_SYN)
	BYTE			m_Type;			//< eRANKING_TYPE ����
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RANKINGID       m_RankingID;
#else
    RANKINGNO		m_MissionNo;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	
	BYTE			m_Page;			//< page : 0 ~ 4 ( �� 10���� 5������ )
PACKET_CG_FWD_CLOSE;

// ���� ��Ŷ
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_INFO_ACK)
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    WORD myranking;
#else
	DWORD m_SettledDate;			//< 20050923
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pBUFFER);
        return static_cast<uint16_t>(kMinSize + m_Size);
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_INFO_NAK)
	DWORD			m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_MEMBER_INFO_SYN)
	BYTE			m_Type;			//< eRANKING_TYPE ����
	RANKINGNO		m_MissionNo;
	RANKINGKEY		m_Key;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_MEMBER_INFO_ACK)
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pBUFFER);
        return static_cast<uint16_t>(kMinSize + m_Size);
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_MEMBER_INFO_NAK)
	DWORD			m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_TIMER_CHANGED_BRD)
//	DWORD			m_MissionElapsedTimePerSecond;				//< Ÿ�̸Ӱ� ����ǰ� ������ ���� ����� �ð�(�ʴ���)
	WORD			m_TotalMonsterKillNum;						//< ������ ���� ų��
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_CLEAR_RECORD_CMD)
	DWORD			m_MonsterKillNum_General;
	DWORD			m_MonsterKillNum_Leader;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SUCCESS_BRD)
	RANKINGKEY		m_Key;										//< ��ƼŰ and ��ŷŰ
	DWORD			m_ClearTime;
	WORD			m_Ranking;									//< ��ŷ
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_FAILED_BRD)
	DWORD			m_ClearTime;
	DWORD			m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_REWARD_SYN)
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RANKINGID       ranking_id;
#else
    RANKINGNO		m_MissionNo;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RANKINGKEY		m_Key;						//< ����ޱ⸦ ���ϴ� �����ַ�ŷ�� Ű(�Ϸù�ȣ)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_REWARD_ACK)
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RANKINGID       ranking_id;
    BYTE            reward_ranking;
    MONEY           rewarded_money;
    WORD            hime_bonus_ratio;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	INVENTORY_TOTAL_INFO	m_ItemInfo;
	int GetSize()
	{
		return ( sizeof(*this)-
			(INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)
			*sizeof(INVENTORY_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_REWARD_NAK)
	DWORD			m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
//��ŷ ����� ��Ŷ
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK)
    //�̼Ƿ�ŷ �׸��� �þ�� ��ġ�� �ٲ��ֵ��� �Ѵ�.
    //MAX_REWARDABLE_MISSION_LIST_SIZE�� ��ŷ�̼Ǹ���Ʈ�� 3��� �Ѵ�
    enum { MAX_MISSION_RANKING_LIST_NUMBER = 8,
           MAX_REWARDABLE_MISSION_LIST_SIZE = 24, };
    
    REWARDABLE_MISSION_LIST reward_mission_list[MAX_REWARDABLE_MISSION_LIST_SIZE];
    DWORD           settled_date;			//< 20050923
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SETTLEDDATE_INFO_NAK)
    DWORD			error_code;
PACKET_CG_FWD_CLOSE;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
//------------------------------------------------
// ��ŷ ����
//------------------------------------------------
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_GET_MISSION_POINT_BRD)
	DWORD			m_dwPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_POINT_CANNOT_REGISTER_BRD)
PACKET_CG_FWD_CLOSE;
#endif // _NA_008012_20150130_RANKING_SYSTEM

//----------------------------------------------------------------------------
// <TUTORIAL MISSION ����>
PACKET_CG_OPEN( CG_MISSION, CG_MISSION_SET_CLEAR_COUNT_CMD )
    MAPCODE m_MapCode;
    WORD    m_ClearCount;
PACKET_CG_CLOSE;

//������������������������������������������������������������������������������������������������
// CG_AREACONQUEST AC ���� ��Ŷ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_STREAM_CMD)
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    Character_Achievements achievement_info_;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(achievement_info_);
        return static_cast<uint16_t>(kMinSize + achievement_info_.GetSize());
    };
#else
	BYTE m_ACStream[MAX_ACSTREAM_SIZE];
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_VALUE_CHANGE_CMD)
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
WORD	index_number_;
WORD	location_code;
BYTE	object_index_;
WORD	object_value_;
#else
BYTE m_byAreaIndex;
BYTE m_byCondition;
DWORD m_dwValue;
int GetSize()
{
    return sizeof(MSG_BASE_FORWARD)+sizeof(m_byAreaIndex)+sizeof(m_byCondition)+sizeof(m_dwValue);
}
#endif
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_REWARD_SYN)
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
WORD	index_number_;
WORD	location_code_;
#else
BYTE m_byAreaIndex;
BYTE m_byRewardIndex;		//< ���� �ܰ��� �ε���
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_REWARD_ACK)
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
WORD                    index_;                     //���� �ε���
WORD					m_accomplishment_ratio;		//�߰��� �޼���
WORD					location_code_;
#endif
EXPTYPE					m_i64Exp;					// ���� ����ġ
MONEY					m_Money;
SLOTCODE				m_SkillCode;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
WORD                    honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
INVENTORY_TOTAL_INFO	m_ItemInfo;
int GetSize()
{
    return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_InvenCount - m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_REWARD_NAK)
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// CG_AREACONQUEST AC ���� ��Ŷ ��
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_LOAD_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_LOAD_ACK)
    WAYPOINT_TOTAL_INFO m_Info;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Info);
        return static_cast<uint16_t>(kMinSize + m_Info.GetSize());
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_LOAD_NAK)
	DWORD		m_dwErrorCode;									//< ResultCode.h�� eWP_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_SYN)
	//���� �� ��ȣ�� ���� ����Ʈ�� �����Ѵ�.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_NAK)
	DWORD		m_dwErrorCode;									//< ResultCode.h�� eWP_RESULT ����
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// ���� ���� ����Ʈ
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_LOAD_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_LOAD_ACK)
    CHAR_WAYPOINT_TOTAL_INFO m_Info;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Info);
        return static_cast<uint16_t>(kMinSize + m_Info.GetSize());
    };
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_SAVE_SYN)
//���� �� ��ȣ, ��ǥ�� ���� ����Ʈ�� �����Ѵ�.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_SAVE_ACK)
//����� index, �� ��ȣ, ��ǥ�� �����Ѵ�.
WAYPOINTSLOT m_Info;
int GetSize()
{
	return sizeof(MSG_BASE_FORWARD) + sizeof(WAYPOINTSLOT);
}
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_DEL_SYN)
//������ index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_DEL_ACK)
//������ index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_EXE_CMD)
//������ index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_NAK)
DWORD		m_dwErrorCode;									//< ResultCode.h�� eWP_RESULT ����
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
// ī�� �ý���
//������������������������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_CHANGE_BORADORI_STATE_BRD )
	DWORD		m_dwObjKey;
	bool		m_bBoradori;		// ������ ���� ����/����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_CHANGE_CHAO_STATE_BRD )
	DWORD		m_dwObjKey;
	BYTE		m_byChaoState;		// ī������(eCHAO_STATE)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ASK_REVENGE_ADDITION_CMD )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// ������ ����Ʈ�� �߰��� ��� Name
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_SYN )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// ������ ����Ʈ�� �߰��� ��� Name
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_ACK )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// ������ ����Ʈ�� �߰��� ��� �̸�(�������� World���� �Ѱ��ش�)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_NAK )
	BYTE		m_byErrorCode;		// RC:eREVENGE_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_NOTIFIY_CHAO_TIME_CMD )
	DWORD		m_dwLeavedTime;
PACKET_CG_FWD_CLOSE;

// ������ ������� ����(����)
PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_TRACK_REVENGE_CHAR_SYN )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
	POSTYPE		m_atItemPos;		// ������ �� ������ġ
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_TRACK_REVENGE_CHAR_NAK )
	BYTE		m_byErrorCode;		// RC:eREVENGE_RESULT
PACKET_CG_FWD_CLOSE;


//������������������������������������������������������������������������������������������������
// �� ��Ŷ
//������������������������������������������������������������������������������������������������

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
//  programcommon\resultcode.h

// ĳ�ü� ���� ��û
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_ACK)
    SalesZoneScriptVersion sales_version_;
    BannerZoneScriptVersion banner_version_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_NAK)
    int error_code_;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

// ĳ�� ��û
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_ACK)
    CashInfo cash_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_NAK)
    int error_code_;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;


// ĳ�ü� ����
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_SYN)
    SalesZoneScriptVersion sales_version_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_NAK)
    int error_code_;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

// ĳ�ü� ����
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CLOSE_NTF)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// ������ ������ ��û
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_SYN)
    int page_;
    int item_count_per_page_;   // MAX_STORAGE_ITEM_PER_PAGE ����.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_ACK)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    int page_;                  // ���� ������
    int total_page_;            // ��ü ������ ��
    int total_item_count_;      // ��ü ��ǰ ��
    int item_count_;            // ���� ������ ��ǰ ��
    StorageItemInfoBuy item_info_[BUY_STORAGE_ITEM_PER_PAGE];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// ���� ������ ��û
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_GIFT_STORAGE_LIST_SYN)
    int page_;
    int item_count_per_page_;   // MAX_STORAGE_ITEM_PER_PAGE ����.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_GIFT_STORAGE_LIST_ACK)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    int page_;
    int total_page_;
    int total_item_count_;
    int item_count_;
    StorageItemInfoGift item_info_[GIFT_STORAGE_ITEM_PER_PAGE];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_GIFT_STORAGE_LIST_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// �����ϱ�
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_ITEM_SYN)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    uint item_count_;
    ShopItemInfo item_info_[SHOP_MAX_ITEM_BUY_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_ITEM_ACK)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    uint item_count_;
    ShopItemInfoEx item_info_[SHOP_MAX_ITEM_BUY_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_ITEM_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// �����ϱ�
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_SEND_GIFT_SYN)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    char recv_character_name_[MAX_CHARNAME_LENGTH+1];
    char gift_message_[GIFT_MESSAGE_LENGTH];
    uint item_count_;
    ShopItemInfo item_info_[SHOP_MAX_ITEM_GIFT_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_SEND_GIFT_ACK)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    char recv_character_name_[MAX_CHARNAME_LENGTH+1];
    uint item_count_;
    ShopItemInfoEx item_info_[SHOP_MAX_ITEM_GIFT_COUNT];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_SEND_GIFT_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_SEND_GIFT_NTF)
PACKET_CG_FWD_CLOSE;


// �������� ������ ���
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_USE_STORAGE_ITEM_SYN)
    UseStorageItem use_item_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_USE_STORAGE_ITEM_ACK)
    UseStorageItem use_item_;
    INVENTORY_TOTAL_INFO inventory_info_;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(inventory_info_);
        return static_cast<uint16_t>(kMinSize + inventory_info_.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_USE_STORAGE_ITEM_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// ĳ���� �̸� üũ
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CHAR_NAME_CHECK_SYN)
    char character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CHAR_NAME_CHECK_ACK)
    char character_name_[MAX_CHARNAME_LENGTH + 1];
    char guild_name_[MAX_GUILDNAME_LENGTH + 1];
    int character_level_;
    int character_class_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CHAR_NAME_CHECK_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_EXIST_GIFT_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_EXIST_GIFT_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_EXIST_GIFT_NAK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_EXIST_GIFT_NTF)
PACKET_CG_FWD_CLOSE;

#else //_NA_0_20110613_GAME_SHOP_RENEWAL

// �� UI�� ó�� �������� ��
// ī�װ� Ȥ�� �������� �޶����� ��
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_SYN)
    uchar   partition_index;                          //< ��Ƽ��
    uchar   category_index;                           //< ī�װ� ��ȣ
    uchar	request_page;					    	  //< ��û ������ ��ȣ (0�� ���� ����)
    ushort	item_filter;							  //< ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_ACK)
    enum { MAX_BUFFER_SIZE = 4096, MAX_GUID = 10, };
    uchar   partition_index;                          //< ��Ƽ��
    uchar   category_index;                           //< ī�װ� ��ȣ
    ushort	item_filter;							  //< ����
    uchar	answer_page;					    	  //< ���� ������ ��ȣ (0�� ���� ����, ��û�� ������ ��ȣ�� ��ü ���������� �Ѿ��� ������ ������ ��ȣ�� �ٲ��.)
    uchar   page_count;                               //< ��û�� �������� ��ü ������ ��  
    ushort  buffer_size;                              //< ���� ������  
    ushort  guid_count;
    uint    shopitem_guid[MAX_GUID];
    uchar   buffer[MAX_BUFFER_SIZE];                  //< ����
    int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-buffer_size); }
PACKET_CG_FWD_CLOSE;
#else //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_SYN)
	BYTE		m_byCategoryIndex;								//< ī���ڸ� ��ȣ
	BYTE		m_byPage;										//< ������ ��ȣ
	BYTE		m_byCategorySeperate;							//< eSHOP_CATEGORY_SEPERATE
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_ACK)
	BYTE m_TotalPageNum;
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

// �� UI�� ó�� �������� ��
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_ACK)
    CashData    cash_info_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;


// ������ ���Ÿ� �Ҷ�
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_BUY_SYN)
	enum { _MAX_BUY_LIST_NUM = 32, };
	BYTE		m_Count;
	SLOTIDX		m_SlotIndex;
	POSTYPE		m_SlotPos;		// �Ⱓ���� ������ ������ ��� ������ �������� �κ��丮 ����Pos
    enum { PURCHASE_DEFAULT = 0, PURCHASE_PPCASH };
    BYTE        m_PurchaseType; // ~_PPCARD = { pp card }, ~_DEFAULT = { cash, mileage, heim, etc... }
	INT			m_PriceGuid[_MAX_BUY_LIST_NUM];

	int GetSize() const
	{
    #if 1 //replace calculation
        const size_t ZeroCountSize = sizeof(*this) - sizeof(m_PriceGuid);
        assert(_MAX_BUY_LIST_NUM >= m_Count);
        return static_cast<int>(ZeroCountSize + sizeof(m_PriceGuid[0]) * m_Count);
    #else
		return (sizeof(*this)-(_MAX_BUY_LIST_NUM-m_Count)*sizeof(INT));
    #endif
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_BUY_ACK)
	MONEY	m_UserMoney;
    CashData m_CashData;
	INT		m_ExtendItemCode;		// �Ⱓ���� �����ۿ� ���� �������ڵ�
	JUST_INVENTORY_TOTAL_INFO	m_TotalInfo;
	int GetSize() const
	{
        const size_t NoStreamSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<WORD>(NoStreamSize + m_TotalInfo.GetSize());
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_BUY_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
	DWORD		m_PriceGuid;									//< m_PriceGuid = 0 �̸� ��� ��ǰ ����
PACKET_CG_FWD_CLOSE;

// ������ �����̷� ���� ó������ ����
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_ACK)
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_SYN)
	INT			m_ItemGuid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_ACK)
	// ShopItem�� Seriailize()
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;


//// ������ ���Ÿ� �Ҷ�
//PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_PACKAGE_BUY_SYN)
//	enum { _MAX_BUY_LIST_NUM = 32, };
//	BYTE m_Count;
//	INT	m_PriceGuid[_MAX_BUY_LIST_NUM];
//	int GetSize()
//	{
//		return (sizeof(*this)-(_MAX_BUY_LIST_NUM-m_Count)*sizeof(INT));
//	}
//PACKET_CG_FWD_CLOSE;
//
//PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_PACKAGE_BUY_ACK)
//	MONEY	m_UserMoney;
//	CASH	m_UserCash;
//	JUST_INVENTORY_TOTAL_INFO	m_TotalInfo;
//	int GetSize()
//	{
//		return sizeof(MSG_BASE_FORWARD)+
//			sizeof(m_UserMoney)+
//			sizeof(m_UserCash)+
//			m_TotalInfo.GetSize();
//	}
//PACKET_CG_FWD_CLOSE;
//
//PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_PACKAGE_BUY_NAK)
//	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
//	DWORD		m_PriceGuid;									//< m_PriceGuid = 0 �̸� ��� ��ǰ ����
//PACKET_CG_FWD_CLOSE;
//



PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_SYN)
	char		m_tszCharName[MAX_CHARNAME_LENGTH+1];				//< ĳ���� �̸� üũ
	WORD GetSize()
	{
		return (WORD)(sizeof(*this) - ((MAX_CHARNAME_LENGTH+1)-(int)strlen(m_tszCharName))) ;
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_ACK)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_SYN)
	enum { SINGLEITEM=1, PACKAGEITEM=2,};
	enum { MAX_GIFT_MESSAGE_LEN = 512, };
	BYTE	m_ItemType;											//< enum { SINGLEITEM=1, PACKAGEITEM=2,};
	INT  	m_PriceGuid;										//< �Ǹ� ���� ���� ��ȣ
    enum {
        PURCHASE_DEFAULT = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_DEFAULT,
        PURCHASE_PPCASH  = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_PPCASH
    };
    BYTE    m_PurchaseType; // ~_PPCARD = { pp card }, ~_DEFAULT = { cash, mileage, heim, etc... }
	char	m_tszCharName[MAX_CHARNAME_LENGTH+1];				//< ĳ���� �̸� üũ
	char 	m_tszGiftMessage[MAX_GIFT_MESSAGE_LEN];				//< ���� �޼���
PACKET_CG_FWD_CLOSE;

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_SEND_GIFT_RELAY_SYN)
    enum {
        SINGLEITEM = MSG_CG_SHOP_SEND_GIFT_SYN::SINGLEITEM,
        PACKAGEITEM = MSG_CG_SHOP_SEND_GIFT_SYN::PACKAGEITEM,
        MAX_GIFT_MESSAGE_LEN = MSG_CG_SHOP_SEND_GIFT_SYN::MAX_GIFT_MESSAGE_LEN,
        PURCHASE_DEFAULT = MSG_CG_SHOP_SEND_GIFT_SYN::PURCHASE_DEFAULT,
        PURCHASE_PPCASH  = MSG_CG_SHOP_SEND_GIFT_SYN::PURCHASE_PPCASH,
    };
    BYTE item_type_;  //< enum { SINGLEITEM=1, PACKAGEITEM=2,};
	int  price_guid_; //< �Ǹ� ���� ���� ��ȣ
    BYTE purchase_type_; // ~_PPCARD = { pp card }, ~_DEFAULT = { cash, mileage, heim, etc... }
    char gift_message_[MAX_GIFT_MESSAGE_LEN]; //< ���� �޼���
    //
    QueryUserIdInfo user_info_;

    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;
#endif

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_ACK)
	MONEY	m_UserMoney;
    CashData m_CashData;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

// ������ ���� ���� ��Ŷ
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_NTF)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ACTION_RECV_GIFT_SYN)
	enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	enum { MAX_GIFT_MESSAGE_LEN = 512, };
	BYTE	m_ActionType;										//< enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	DWORD	m_GiftGuid;											//< ����Guid
	//char	m_tszThanksMessage[MAX_GIFT_MESSAGE_LEN+1];
	//int GetSize()
	//{
	//	switch(m_ActionType)
	//	{
	//	case OBTAIN:
	//		return sizeof(MSG_BASE_FORWARD)+(MAX_GIFT_MESSAGE_LEN-strlen(m_tszThanksMessage));
	//	}
	//	return sizeof(MSG_BASE_FORWARD)+sizeof(m_ActionType)+sizeof(m_GiftGuid);
	//}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ACTION_RECV_GIFT_ACK)
	enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	BYTE	m_ActionType;										//< enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	JUST_INVENTORY_TOTAL_INFO	m_TotalInfo;
	int GetSize()
	{
		switch(m_ActionType)
		{
		case OBTAIN:
			return sizeof(MSG_BASE_FORWARD)+
				m_TotalInfo.GetSize();
		}
		return sizeof(MSG_BASE_FORWARD)+sizeof(m_ActionType);
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ACTION_RECV_GIFT_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_ACK)
	DWORD		m_GiftCount;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_RECV_GIFT_LIST_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_RECV_GIFT_LIST_ACK)
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_RECV_GIFT_LIST_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h�� eSHOP_RESULT ����
PACKET_CG_FWD_CLOSE;




#endif //_NA_0_20110613_GAME_SHOP_RENEWAL



//������������������������������������������������������������������������������������������������
// �ν��Ͻ� ����
//������������������������������������������������������������������������������������������������

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN )
	POSTYPE				m_ItemPos;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_REQUEST_RESULT_NAK )
	BYTE				m_byKind;			//0 �̸� eITEM_RESULT 1�̸� eROOM_RESULT
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_JOIN_ACK )
	MAPCODE				m_MapCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_JOIN_NAK )
PACKET_CG_CLOSE;

// for compatible
PACKET_CG_INHERIT_OPEN( CG_ZONE, CG_ZONE_CREATE_TOP_OF_TRIAL_SYN, MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN )
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE, CG_ZONE_CREATE_TOP_OF_TRIAL_NAK, MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_RESULT_NAK )
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE, CG_ZONE_JOIN_TOP_OF_TRIAL_ACK, MSG_CG_ZONE_INSTANCE_DUNGEON_JOIN_ACK )
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE, CG_ZONE_JOIN_TOP_OF_TRIAL_NAK, MSG_CG_ZONE_INSTANCE_DUNGEON_JOIN_NAK )
PACKET_CG_INHERIT_CLOSE;


PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_LEAVE_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK )
	DWORD			m_dwErrorCode;			//< eITEM_RESULT
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD)
	DWORD			m_NoticeCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GET_BATTLE_OPEN_TIME_SYN)	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ETC, CG_ETC_GET_BATTLE_OPEN_TIME_ACK)	
	// ��ž ����
	struct {
		eJION_TOP_STATE		m_eJoinState;
		DWORD				m_JoinOverTime;
	};
	// Ÿ�� ����
	nsSSQ::SSQTicketInfo	m_SSQTicketInfo;
PACKET_CG_FWD_CLOSE;

//������������������������������������������������������������������������������������������������
//	Ÿ���� ��� �������� ���� <- ���� ���Ǵ� ProgramCommon/SSQ/SSQInfoStruct.hxx�� ����
//������������������������������������������������������������������������������������������������

//����������������������������������������������������������������������������������
//
//
PACKET_CG_OPEN(CG_STATUS, CG_STATUS_SSQ_INFO_BRD) // variable size packet
    WAR_MSG_HDR m_Header[1]; // Stream Header, next pointer is data stream
    uint16_t GetSize() const
    { return static_cast<uint16_t>(sizeof(*this) + m_Header->size); }
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_CTRL_CMD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;

//	�Ƹ� �̰� ������� ���� ���ɼ��� ũ��. C->G �����̹Ƿ�
PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_SYN, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_BRD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;
//
//����������������������������������������������������������������������������������
//
//
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_SYN)
	POSTYPE				m_ItemPos;	// ����ϴ� Item�� ���� ��ġ
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_ACK)
	enum eRESULT { FAIL = PROTOCOL_GENERATOR_ZONE_EX_091, SUCCESS = PROTOCOL_GENERATOR_ZONE_EX_054 };
	enum eFAIL_RC_CODE { RET_ITEM = PROTOCOL_GENERATOR_ZONE_EX_044, RET_ROOM = PROTOCOL_GENERATOR_ZONE_EX_116 };
	BYTE				m_Result;	// eRESULT

	struct sRETINFO_FAIL {
		BYTE		m_Kind;		// eFAIL_RC_CODE { RET_ITEM �̸� eITEM_RESULT, RET_ROOM�̸� eROOM_RESULT }
		DWORD		m_ErrorCode;
	};
	struct sRETINFO_SUCCESS {
		MAPCODE		m_CodeOfSSQ;
	};
	union {
		sRETINFO_FAIL		RETINFO_FAIL;		//
		sRETINFO_SUCCESS	RETINFO_SUCCESS;	//
	};

	inline VOID			SetFailResult( const BYTE kind, const DWORD errorCode )
	{
		m_Result = FAIL;
		sRETINFO_FAIL& ref = RETINFO_FAIL;
		ref.m_Kind = kind;
		ref.m_ErrorCode = errorCode;
	}
	inline VOID			SetSuccessResult( const MAPCODE CodeOfSSQ )
	{
		m_Result = SUCCESS;
		RETINFO_SUCCESS.m_CodeOfSSQ = CodeOfSSQ;
	}

	// for Client ���� ���� �Ǵܿ�
	inline BOOL			IsSuccess() { return SUCCESS == m_Result; }
	inline WORD			GetSize()
	{
		const WORD DEF_SIZE = sizeof(MSG_BASE_FORWARD) + sizeof(BYTE);
		return SUCCESS == m_Result ? DEF_SIZE + sizeof(RETINFO_SUCCESS) : DEF_SIZE + sizeof(RETINFO_FAIL);
	}
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_SYN, MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN )
	DWORD				m_CodeOfSSQ;
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_ACK, MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK )
PACKET_CG_INHERIT_CLOSE;


PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD )	//G->C ����|�ʵ� ������ ���� ���� ���� + ȯ�ҽ� ȯ�� ����
	SSQ_RESERVED_REWARD_INFO		m_RewardInfo;
	struct RefundInfo {
		RC::eITEM_RESULT				Result;		// ȯ�ҽ� ������ ó�� ���� ���ɼ� �����Ƿ�...
		MONEY							ChangedTotalHeim;
		INVENTORY_TOTAL_INFO			TotalInfo;
	}								m_Refund;	// ȯ�ҽ� ���
	inline	BOOLEAN					IsWinnerReward() const	{ return eEVENT_MULTIUSE_SSQ_WINNER_REWARD == m_RewardInfo.m_SharedInfo.EventID; }
	inline	BOOLEAN					IsQuestFail() const		{ return eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO == m_RewardInfo.m_SharedInfo.EventID; }
	inline	BOOLEAN					IsRefund() const		{ return eEVENT_MULTIUSE_SSQ_REFUND == m_RewardInfo.m_SharedInfo.EventID; }
	inline	WORD					GetSize() /*const*/
	{
		const SIZE_T min_size = sizeof(*this)-sizeof(m_Refund);
		return !IsRefund()
			?	(WORD)( min_size )
			:	(WORD)( min_size
					+ ( sizeof(m_Refund)-sizeof(m_Refund.TotalInfo) )
					+ m_Refund.TotalInfo.GetSize() )
			;
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_WINNER_REWARD_CMD )	//G->C ���ڿ� ���� �κ��丮�� ������ ����
	RC::eITEM_RESULT				m_Result;
	INVENTORY_TOTAL_INFO			m_TotalInfo;
	inline	WORD					GetSize() /*const*/
	{
		const SIZE_T min_size = (sizeof(*this)-sizeof(m_TotalInfo));
		return (WORD)( min_size + m_TotalInfo.GetSize() );
	}
PACKET_CG_CLOSE; 

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_OBSERVER_SYN )
	BYTE							Request;	//nsSSQ::Observer::Info::eRequest
	union {
		struct {
			BYTE				PressedKey;	//nsSSQ::Observer::Info::eKeyCmd
		}							ObserverCmd;
	};
PACKET_CG_CLOSE;

//{__NA001283_20090221_COLLECTION_PACKET
//������������������������������������������������������������������������������
// CG_COLLECT
//������������������������������������������������������������������������������

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ALLCOLLECTIVE_SYN )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ALLCOLLECTIVE_CMD )
enum { _MAX_FIELDCOLLECT_INFO_SIZE = 50 };

BYTE					m_byCount;
COLLECT_RENDER_INFO		m_CollectiveInfo[_MAX_FIELDCOLLECT_INFO_SIZE];

int	GetSize() {	return (sizeof(*this)-(_MAX_FIELDCOLLECT_INFO_SIZE-m_byCount)*sizeof(COLLECT_RENDER_INFO)); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_LEAVE_SECTOR_CMD )
enum { _MAX_FIELDCOLLECT_INFO_SIZE = 50 };

BYTE			m_byCount;
C_COLLECTKEY	m_ObjKeyCollection[_MAX_FIELDCOLLECT_INFO_SIZE];

int	GetSize() {	return (sizeof(*this)-(_MAX_FIELDCOLLECT_INFO_SIZE-m_byCount)*sizeof(C_COLLECTKEY)); }
PACKET_CG_FWD_CLOSE;

	//.//

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTION_SYN )
S_COLLECTKEY	m_ObjKeyCollection;		// ä����ǰ
WzVector		m_wvCurPos;				// Ŭ���̾�Ʈ�� �ִ� �÷��̾��� ���� ��ǥ (������)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_CANCEL_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_SUCCESS_SYN )
PACKET_CG_FWD_CLOSE;
    //.//
// CHANGES: f110425.1L, moved to sync
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTION_BRD )
C_COLLECTKEY	m_dwObjectKey;			// ä����ǰ.
WzID			m_CollectionID;			// ä����ǰID.
DWORD			m_Collector;			// ä����.
PACKET_CG_FWD_CLOSE;

// CHANGES: f110425.1L, moved to sync
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_CANCEL_BRD )
C_COLLECTKEY	m_dwObjectKey;			// ä����ǰ.
WzID			m_CollectionID;			// ä����ǰID.
DWORD			m_Collector;			// ä����.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTIVE_BRD )
C_COLLECTKEY	m_dwObjectKey;			// ä����ǰ.
WzID			m_CollectionID;			// ä����ǰID.
PACKET_CG_FWD_CLOSE;

	//.//

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTION_NAK )
DWORD		m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

	//.//
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_OBTAIN_CMD )
#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
    WORD quest_code_;
#endif
    INVENTORY_TOTAL_INFO	m_tagInventory;
    int GetSize() 
    { 
#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
        return (sizeof(MSG_BASE_FORWARD) + sizeof(quest_code_) + m_tagInventory.GetSize()); 
#else
        return (sizeof(MSG_BASE_FORWARD)+m_tagInventory.GetSize()); 
#endif
    }
PACKET_CG_FWD_CLOSE;

#ifdef _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_DEACTIVE_BRD )
enum { _MAX_FIELDCOLLECT_INFO_SIZE = 10 };

BYTE			m_byCount;
C_COLLECTKEY	m_ObjKeyCollection[_MAX_FIELDCOLLECT_INFO_SIZE];

int	GetSize() {	return (sizeof(*this)-(_MAX_FIELDCOLLECT_INFO_SIZE-m_byCount)*sizeof(C_COLLECTKEY)); }
PACKET_CG_FWD_CLOSE;
#endif _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
//}__NA001283_20090221_COLLECTION_PACKET
//
//����������������������������������������������������������������������������������

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

// ��Ż ���� ��� (�ӽ÷� �����Ǵ� ��Ż ����)
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD)
    QuestPortalInfo questPortalInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD)
    KEYTYPE     portalKey;    // 
PACKET_CG_CLOSE;

// MapEnterancePortal.txt�� ���� ���� ��Ż�� �̼� ||
// QuestPortalCreate�� ���� �ӽ� ������ ��Ż ���� ��û
// (Note) ���� MISSION_CREATE, INST~_CREATE����� ��ü�� ���� �ִ�.
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN)
    enum eSYNC { eSYNC_TEMP_PORTAL, eSYNC_FIXED_PORTAL, eSYNC_COUNT };
    BYTE    syncType;  // eSYNC
    KEYTYPE portalKey;
    WORD    mapEnterancePortalIndex;

    inline WORD GetSize() const
    {
        return WORD(sizeof(*this));
    }
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK)
    enum eRESULT_TYPE { eRESULT_ROOM, eRESULT_ITEM, eRESULT_COUNT };
    BYTE errorType; //eRESULT_TYPE
    WORD errorCode; //::eROOM_RESULT | ::eITEM_RESULT
PACKET_CG_CLOSE;

//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

//_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION = {
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD)
    enum eOpenState {
        eOpenState_None = 0,
        eOpenState_Created, // create fixed portal
        eOpenState_Countdown,
        eOpenState_Closed,
        //
        eOpenState_Counts
    };
    KEYTYPE portal_key;
    uint8_t open_state;
    ulong portal_lifetime;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN)
    KEYTYPE portal_key;
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_NAK,
                        MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN)
    RC::eROOM_RESULT error_code; // < result != RC::RC_ROOM_SUCCESS
PACKET_CG_INHERIT_CLOSE;
//}_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_ROOM_MEMBER_STATUS_BRD)
    enum eRESULT_TYPE { RET_FAIL, RET_SUCCESS };
    struct MemberInfo
    {
        WORD    objectKey;
        WORD    roomResult;  //RC::eROOM_RESULT
    };

    BYTE        m_Result;   //eRESULT_TYPE
    BYTE        m_NumberOfMembers;
    MemberInfo  m_Members[max(MAX_FIELD_PARTYMEMBER_NUM, MAX_PARTYMEMBER_NUM)];
    inline void Clear()
    {
        m_Result = RET_FAIL;
        m_NumberOfMembers = 0;
        ZeroMemory(m_Members, sizeof(m_Members));
    }

    inline WORD GetSize() const
    {
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Members));
        if(m_Result == RET_SUCCESS)
            return MinSize;
        return MinSize + WORD(sizeof(MemberInfo) * m_NumberOfMembers);
    }
PACKET_CG_CLOSE;

//������������������������������������������������������������������������������������������������
//	���Ȱ�ȭ ���� ó�� ��Ŷ ���� (MOLA 2��, 5�� Update)
//������������������������������������������������������������������������������������������������

//	__NA000630_070817_MOLA_5th_UPDATE

//	eCG_CONNECTION_PROTOCOL
//		CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD		= PROTOCOL_GENERATOR_026,
//		CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK		= PROTOCOL_GENERATOR_027,
//		CG_SECURE_QUERY_BUILTIN_CHK_CMD			= PROTOCOL_GENERATOR_028,
//		CG_SECURE_QUERY_BUILTIN_CHK_ACK			= PROTOCOL_GENERATOR_029,
//	eCG_CHARINFO_PROTOCOL
//		CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD		= PROTOCOL_GENERATOR_026,
//		CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK		= PROTOCOL_GENERATOR_027,
//	eCG_MAP_PROTOCOL
//		CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN	= PROTOCOL_GENERATOR_003,
//	eCG_BATTLE_PROTOCOL
//		CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD		= PROTOCOL_GENERATOR_016,
//		CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK		= PROTOCOL_GENERATOR_017,
//	eCG_STYLE_PROTOCOL
//		CG_SECURE_QUERY_RESOURCE_CHK_T4_CMD		= PROTOCOL_GENERATOR_013,
//		CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK		= PROTOCOL_GENERATOR_014,
//		CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN	= PROTOCOL_GENERATOR_015,

PACKET_CG_OPEN(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_CMD)
	BYTE	m_dwBuiltinCMD[8/* +a */];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_ACK)
	BYTE	m_dwBuiltinACK[8/* +b */];
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_MAP, CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN)
PACKET_CG_CLOSE;


//	__NA070212_ENHANCED_SECURE_POLICYMENT__
//	CG_SYNC
//		CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD	= PROTOCOL_GENERATOR_061,
//		CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK	= PROTOCOL_GENERATOR_062,
//#ifdef __NA000630_070817_MOLA_5th_UPDATE
//		CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD	= PROTOCOL_GENERATOR_065,
//		CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK	= PROTOCOL_GENERATOR_066,
//#else __NA000630_070817_MOLA_5th_UPDATE
//	CG_SKILL
//		CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD	= PROTOCOL_GENERATOR_024,
//		CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK	= PROTOCOL_GENERATOR_025,
//#endif //__NA000630_070817_MOLA_5th_UPDATE
//	CG_ITEM
//		CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD	= PROTOCOL_GENERATOR_137,
//		CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK	= PROTOCOL_GENERATOR_138,
//	CG_TRADE
//		CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_CMD	= PROTOCOL_GENERATOR_059,
//		CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK	= PROTOCOL_GENERATOR_060,
//	CG_VENDOR
//		CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_CMD	= PROTOCOL_GENERATOR_077,
//		CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK	= PROTOCOL_GENERATOR_078,
//	CG_WAREHOUSE
//		CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_CMD	= PROTOCOL_GENERATOR_016,
//		CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK	= PROTOCOL_GENERATOR_017,
//	CG_PARTY
//		CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_CMD	= PROTOCOL_GENERATOR_054,
//		CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK	= PROTOCOL_GENERATOR_055,
//	CG_STATUS
//		CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_KEY	= PROTOCOL_GENERATOR_046,
//		CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK	= PROTOCOL_GENERATOR_047,
//		CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_CMD	= PROTOCOL_GENERATOR_048,
//////////////////////////////////////////////////////////////////////////

PACKET_CG_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK)
PACKET_CG_CLOSE;

//////////////////////////////////////////////////////////////////////////
PACKET_CG_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_KEY)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_CMD)
PACKET_CG_CLOSE;
// <END>
//������������������������������������������������������������������������������������������������

/************************************************************************/
/*                       ���� ������ ���� ��Ŷ                          */
/************************************************************************/
//"������ �����ϱ�" ��ư Ŭ����
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_APPLY_SYN )
    MAPCODE                 m_MapCode;                                      //��û ���� ���ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_APPLY_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //�۾� ����� �ڵ�
    BOOL                    m_CheckBatting;                                 //������ �������� �Ұ������� üũ
    BYTE                    m_DataNum;                                      //��� ����Ʈ ����
    DOMINATION_ACTION_INFO  m_ActionList[MAX_DOMINATION_ACTIONLIST];        //��� �Խ��� ����Ʈ(2009.09.21 ���� Max�� 10��)
    uint16_t GetSize() const
    {
        assert(m_DataNum <= _countof(m_ActionList));
        const size_t kMinSize = sizeof(*this) - sizeof(m_ActionList);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ActionList[0]) * m_DataNum);
    }
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"������ �����ϱ�" -> "�����ϱ�" ��ư Ŭ����
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BATTING_HEIM_SYN )
    MAPCODE                 m_MapCode;                                      //��û ���� ���ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BATTING_HEIM_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                    //�۾� ����� �ڵ�
    ULONGLONG               m_RemainCost;                                    //���� �����ִ� ��� ����
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    DWORD penalty_time;
#endif //_NA_000000_20130429_DOMINATION_PENALTY
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"�����ϱ�" â���� Ȯ�� ��ư ������ ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ACCEP_HEIM_SYN )
    MAPCODE                 m_MapCode;                                      //��û ���� ���ڵ�
    POSTYPE                 m_TokenPos;                                     //���� ������ �κ��丮 ��ġ
    BYTE                    betting_item_count_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ACCEP_HEIM_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //�۾� �����, ���� ������ ������ �־����� Ȯ���ϴ� �� �߰�
    POSTYPE                 m_TokenPos;
    BYTE                    betting_item_count_;
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"����������" ��ư�� ������ ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MANAGE_SYN )
    MAPCODE                 m_MapCode;                                      //��û ���� ���ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MANAGE_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //�۾� ����� �ڵ�
    BYTE                    m_TotalPoint;                                   //���潺 ����Ʈ �Ѱ���
    BYTE                    m_DefenseWall[MAX_DOMINATION_DEFENSEWALL_NUM];  //���� ���� ������ �� �� ����(2009.09.21 ���� Max�� 3��)
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"����������"â���� Ȯ���� ������ ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SAVE_GATESTAT_SYN )
    MAPCODE                 m_MapCode;                                      //��û ���� ���ڵ�
    BYTE                    m_DefenseWall[MAX_DOMINATION_DEFENSEWALL_NUM];  //���� ���� ������ �� �� ����(2009.09.21 ���� Max�� 3��)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SAVE_GATESTAT_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //�۾� ����� �ڵ�
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"����������" �溮 ����Ʈ �߰� ��ư�� ������ ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN )
    MAPCODE                 map_code_;                                      //��û ���� ���ڵ�
    SLOTCODE                item_code_;                                     //�溮 ���� ������ �ڵ�
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK )
    RC::eDOMINATION_RESULT  result_code_;                                   //�۾� ����� �ڵ�
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//������ ������ �����鿡�� ������ ������ �̴ϸʺ��� �̺�Ʈ �÷��� ���� �˸�
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_SET_BRD )
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//�̴ϸʿ� ��� ǥ�� �ڷ� ��û
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_LIST_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_LIST_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;//�۾� ����� �ڵ�
    BYTE                    m_NumDominationGuild;//������ ������ ��� ����
    DOMINATION_GUILD_INFO   m_GuildInfo[MAX_DOMINATION_MAP];//���� ��� ����

    uint16_t GetSize() const
    {
        assert(m_NumDominationGuild <= _countof(m_GuildInfo));
        return (sizeof(*this) - sizeof(m_GuildInfo)) +           //���� ũ��
               (sizeof(m_GuildInfo[0]) * m_NumDominationGuild);  //���� ũ��
    }
PACKET_CG_FWD_CLOSE;   

/************************************************************************/
//���� ���� Ÿ���� Ŭ��������
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_NOTICE_SYN )
    MAPCODE                 m_MapCode;                                      //Ư�� ���� MapCode
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_NOTICE_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //�۾� ����� �ڵ�
    BOOL                    m_EnterFlag;                                    //�����ư Ȱ��ȭ flag
    BOOL                    m_AuctionFlag;                                   //��Ź�ư Ȱ��ȭ flag
    BOOL                    m_ManageFlag;                                   //������ư Ȱ��ȭ flag
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    BOOL                    m_JoinFlag;                                     //������ ���� Ȱ��ȭ flag
    DOMINATION_GUILD_INFO   m_GuildInfo;                                    //���ݿ� ������ ��� ����
PACKET_CG_FWD_CLOSE;


//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
/************************************************************************/
//���� ��� ����Ʈ �˸�
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBERLIST_NTF )
    bool                    is_delete_member_;
    MAPCODE                 map_code_;
    BYTE                    data_count_;
    DominationMemberInfo    member_list_[MAX_DOMINATION_MEMBER*2];
    uint16_t GetSize() const
    {
        assert(data_count_ <= _countof(member_list_));
        const size_t kMinSize = sizeof(*this) - sizeof(member_list_);
        return static_cast<uint16_t>(kMinSize + sizeof(member_list_[0]) * data_count_);
    }
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//������ ����� ���� ��û
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_JOIN_SYN )
    MAPCODE                 map_code_;
PACKET_CG_FWD_CLOSE; 
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_JOIN_ACK )
    RC::eDOMINATION_RESULT  result_code_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//������ ��� ���� ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_LEAVE_SYN )
    MAPCODE                 map_code_;
    CHARGUID                memeber_guid_;
PACKET_CG_FWD_CLOSE; 
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_LEAVE_ACK )
    RC::eDOMINATION_RESULT  result_code_;
    CHARGUID                memeber_guid_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//������ ����Ʈ ���� ��ħ ��û
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_REFRESH_MEMBERLIST_REQ )
    MAPCODE                 map_code_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//���� ������ ���ݱ�忡 ���� �Ǿ��� ���
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SELECTED_BRD )
    MAPCODE m_DominationFieldCode;                                          //�������� ������ ���� �ڵ�
PACKET_CG_FWD_CLOSE;

    //.//
    //_NA001385_20090924_DOMINATION_ETC

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_AREA_INFO_CMD )        // �÷��̾� ������ ���� ���� ���� (�ڽŰ� ������)
    //_NA_0_20100208_DOMINATION_AREA_INFO
    DWORD					    m_dwPlayerKey;						// ���� ���� ObjectKey
    DOMINATION_STATE_TOTAL_INFO m_stateTotalInfo;                   // ������ ���� ����
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_stateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_stateTotalInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_TIME_SYNC_BRD )        // ������ ���۽� �Ǵ� �߰� �ð� ����ȭ�� �ش� �� ��������
    DWORD m_dwRemainedTime;                                         // ms (�ִ�: 3600000)
    BYTE  m_byEventState;                                           // ������ ���࿩��. (eDOMINATION_EVENT_STATE)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_COUNT_NOSES_BRD )      // ������ �ʿ� ��.����� �� ��ü �������� ī���� �߼�
    BYTE m_byCount;                                                 // �ش� ��� �ο���  (����/�ִ�)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_KICK_NOTIFY_BRD )      // ������ �ý��� �غ� ���� Ÿ�� �Ա��� ���� �̵� (10����)
PACKET_CG_FWD_CLOSE;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_TURNOVER_BRD )      // ������ �Ͽ����� Ŭ���̾�Ʈ���� �˸�
    GUILDGUID m_winGuid;                                          // ���� ���� ���
    MAPCODE   m_mapCode;                                          // ��������
    char      win_guild_name[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const 
    {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_START_NOTIFY_BRD )      // ������ ���� �� �˸� (�������� ���۵� �����Դϴ�. ~~)
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    eDOMINATION_NOTIFY_TIME notify_type;                             // �˸� �޽��� Ÿ��
    BYTE    m_byTime;                                                // ��
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    eDOMINATION_TYPE m_domination_type;                              // ������ ����
#endif
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_CRYSTALWARP_STATE_BRD )  // ���� ���� ��ȯ, ��ȯ ����, �ı� �˸�.
    DWORD   m_dwCrystalWarpKey;                                       // ���� ���� ������ƮŰ
    BYTE    m_byState;                                                // ����
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;
    //.//
    //_NA001385_20090924_DOMINATION_STAMP

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BEGIN_STAMP_BRD )      // ������ ���� ���۽� �ش� �� ���ݱ�������
    WzID m_nStampID;			                                    // ä����ǰID.
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    char m_stamp_guild_name[MAX_GUILDNAME_LENGTH+1];//���� ��� �̸�
#endif
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_CANCLE_STAMP_BRD )     // ������ ���� ���н� �ش� �� ���ݱ������� (Ŭ�� cancle ��Ŷ�� ���� BRD)
    WzID m_nStampID;			                                    // ä����ǰID.
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_RESULT_BRD )           // ������ ���
    GUILDGUID m_winGuid;                                            // ��
    GUILDGUID m_loseGuid;                                           // ��
    MAPCODE   m_mapCode;                                            // ��������
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    char win_guild_name[MAX_GUILDNAME_LENGTH+1];
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_UPDATE_GUILDINFO_BRD )
    WORD domination_map_count_;                                     // �������� ��
    WORD domination_join_count_;                                    // ������ ���� Ƚ��
    WORD domination_defense_count_;                                 // ������ ��� ���� Ƚ��    
    WORD domination_hunting_count_;                                 // ������ ���� ��� Ƚ��
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD )
    GUILDGUID allience_list[MAX_ALLIENCE_GUILD_NUM];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

/**********************���� ������ ���� ��Ŷ END*************************/

#ifdef _NA_0_20100819_AGGRO_RENEWAL
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_AGGRO_VALUES_CMD )	
	BYTE aggro_player_count_;										//���Ͱ� ��� ����Ʈ�� ������ �ִ� �÷��̾� ��
	AggroValueInfo aggor_info_[MAX_TARGET_NUM];	
	int GetSize() const
	{
		return ( sizeof(MSG_CG_AGGRO_VALUES_CMD) - 
			((MAX_TARGET_NUM - aggro_player_count_) * sizeof(AggroValueInfo)) );
	}	
PACKET_CG_FWD_CLOSE;
#endif //_NA_0_20100819_AGGRO_RENEWAL

#ifdef _NA_20100322_AGGRO_DISPLAY
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_ARRGO_DISPLAY_CMD )	
    enum {MAX_AGGRO_DISPLAY = 5,};
    BYTE aggro_player_count_;										//���Ͱ� ��� ����Ʈ�� ������ �ִ� �÷��̾� ��
    AggroValueInfo aggor_info_[MAX_AGGRO_DISPLAY];	    
    int GetSize() const
    {   
        return ( sizeof(MSG_CG_ARRGO_DISPLAY_CMD) - 
            ((MAX_AGGRO_DISPLAY - aggro_player_count_) * sizeof(AggroValueInfo)) );
    }	
PACKET_CG_FWD_CLOSE;
#endif //_NA_20100322_AGGRO_DISPLAY

#ifdef _NA_20100322_AGGRO_DISPLAY
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_MONSTER_SELECT_INFO_SYN )	
    DWORD monster_key;
PACKET_CG_FWD_CLOSE;
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
PACKET_CG_FWD_OPEN( CG_CHARINFO, CG_HONORSYSTEM_PACKET )
    BYTE packet_type;
PACKET_CG_FWD_CLOSE;
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_POINTWALLET_PACKET)
    BYTE packet_type;
PACKET_CG_CLOSE;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARNAME_CHANGE_REQUEST_ACK)
    BYTE result;
PACKET_CG_CLOSE;

//!~ _NA_000000_20140120_SMART_NPC_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_SMARTNPC_PACKET)
    BYTE packet_type;
    DWORD CID;
    DWORD NID;
    DWORD EID;
    DWORD STEP;
PACKET_CG_CLOSE;
//~! _NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_ATTENDANCE_INFO_PACKET)
    AttendanceInfo attendance_infos;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET)
PACKET_CG_CLOSE;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET)
    MiniGameBingoInfos minigame_bingo_infos;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_MINIGAME_NAK)
    int result_code;
PACKET_CG_CLOSE;
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_SUN_RANKING_GRADE_PACKET)
    eSUN_RANKING_TYPE ranking_type;
    eSUN_RANKING_GRADE_TYPE grade;
PACKET_CG_CLOSE;
// ��ŷ ��û ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SYN)
    eSUN_RANKING_TYPE ranking_type;
    bool is_guild_ranking;
PACKET_CG_CLOSE;

// ��ŷ ��û ��� ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_ACK)
    enum { MAX_INFO_COUNT = 50, };
    SunRankingInfoForClient infos[MAX_INFO_COUNT]; //���� ��Ŀ ����
    int   my_ranking;
    SCORE my_score;
PACKET_CG_CLOSE;

// ��ŷ ��û ���� ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_NAK)
    int error_code;
PACKET_CG_CLOSE;

// ���� ���� ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_STATUE_INFO_ACK)
    StatueInfo info;
PACKET_CG_CLOSE;

// ��ŷ ���� ���� ��û ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SETTLE_INFO_SYN)
PACKET_CG_CLOSE;

// ��ŷ ���� ���� ��Ŷ
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SETTLE_INFO_ACK)
    BYTE type;
    BYTE day;
    WORD time;
PACKET_CG_CLOSE;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/************************************************************************/
//���� �ý���
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
PACKET_CG_OPEN(CG_PARTY, CG_CHAOS_ZONE_PACKET)
    BYTE category_type;
PACKET_CG_CLOSE;
#endif  // #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//!~ _NA_007086_20140318_MONSTERBOOK
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_MONSTERBOOK_PACKET)
    BYTE packet_type;
PACKET_CG_CLOSE;
//~! _NA_007086_20140318_MONSTERBOOK

#pragma warning(pop)

#pragma pack(pop)

#endif // __PACKETSTRUCT_CG_H__