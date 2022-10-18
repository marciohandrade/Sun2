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
//      SYN - Server에 요청
//      ACK - 요청에 대한 허락
//      NAK - 요청에 대한 거부
//      CMD - Server의 명령
//      BRD - Server가 동기화 관련 내용을 알림
//
// * 프로토콜 Naming Rule
//      Prefix_카테고리명_프로토콜명_Postfix
//      ex) CG_CONNECTION_REGISTER_SYN
//
//==================================================================================================

//--------------------------------------------------------------------------------------------------
///! CG_CONNECTION, // 접속
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
    BYTE  m_byShutdownTime; //< 시간(m_byShutdownTime시간후 서버 종료)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_RESTART_CMD)
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_REENTERSERVER_SYN)
    PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T2(77), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_SYN)
    PACKET_RND_TOTAL_BLOCK_START(GETRNDNO_RNG128_T2(29));
        PACKET_RND_PAD_START(GETRNDNO_RNG04_T3(109));
            // 인증서버가 보내준 인증아이디
            DWORD m_dwAuthID;
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(44), 1);
            // 계정아이디 (ENCODE TARGET)
        #if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || \
            defined(__NA000000_GLOBAL_AUTH_PROCESS__) // || \
            //defined(__KR000000_090128_CHANNELING__) <- 이후 반영하도록 하자. 일괄반영을 위해서 auth계열 버전 정보 설정 필요.
            char m_szID[ID_MAX_LEN + 1];
        #else
            char m_szID[16];
        #endif
            PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(41), 2);

            PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T5(9));
                // 클라이언트 버전 Major
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byHighVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(8), 1);
                // 클라이언트 버전 Minor
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byMiddleVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(2), 2);
                // 클라이언트 버전 Devel
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byLowVerNo);
                PACKET_RND_SHRBLK64_RNDCONT(GETRNDNO_RNG08_T1(9), 3);
                // 국가 코드
                PACKET_RND_SHRBLK64_CONT(BYTE, m_byNationCode);
            PACKET_RND_SHRBLK64_END();
            PACKET_RND_PAD_CONT(GETRNDNO_RNG16_T5(141), 3);
            // 인증서버에서 보내준 시리얼 키 (ENCODE TARGET)
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
    //   이것도 변경할 예정, 크기 정책을 어캐할 것인가를 아직 미설정중이므로 이번엔 패스
    // (WAVERIX) (NOTE) (090719) 참 오래 끌고 있다. 빨리 작업해 보자.
    BOOL m_bPVP; //__NA_000839_20080129_PVP_OPTION
    BYTE m_byCount;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE char_slot_size; // 사용 가능한 캐릭터 슬롯 개수
#endif
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    BYTE m_ReNameStatus[MAX_CHARACTER_LIST_NUM]; //캐릭터명 변경 여부 플레그 값이다 캐릭터 정보와 섞지 말고 따로 할당한다.
#endif
    BYTE m_pBuffer[sizeof(CLIENT_CHARACTER_PART) * MAX_CHARACTER_LIST_NUM +
                   kCharacterPartPaddingSize];
    // <임시버퍼로 사용할 타입>
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

// 빌링 시간 만료 알림
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_BILLING_ALARM_BRD)
    DWORD dwRestTerm; // 남은 시간 (sec)
PACKET_CG_CLOSE;

// 크래쉬 리포트
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
// S->C TickCount 질의
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_SYN)
    DWORD round;        // 서버측 패킷 식별용 라운드 번호
    BYTE  index;        // 서버측 패킷 식별용 인덱스 번호
PACKET_CG_CLOSE;
// C->S TickCount 질의에 대한 응답
PACKET_CG_OPEN(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_ACK)
    DWORD round;        // _SYN에서 받은 라운드 값을 그대로 담아 응답
    BYTE  index;        // _SYN에서 받은 인덱스 값을 그대로 담아 응답
    DWORD tick_count;   // 처리 시점의 클라이언트 TickCount
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
    enum ERROR_CODE {SUCCEED,       //성공
                     DB_FAIL,       //디비 오류
                     DONOT_RENAME,  //이름을 변경할수 있는 캐릭터가 아님
                     INDEX_INVALID, //캐릭터 슬롯이 유효하지 않다.
                     NOT_CONDITION = 10,  //이름을 변경할수 있는 조건이 안됨 (길드 장, 친구 목록 등등)
                     SEND_FAIL,     //디비로 전송 실패 
                     NOT_FIND_USER, //사용자를 찾을수 없음
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
///! CG_CHARINFO,   // 캐릭터 정보
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

// UP 변화했음
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_USERPOINT_CMD)
    UPTYPE m_TotalUP;  // 현재 보유중인 UP
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
    // <임시버퍼로 사용할 타입>
    //CLIENT_CHARACTER_PART& rCharInfo

    int ConvertForSend(CLIENT_CHARACTER_PART& rCharInfo);
    void ConvertForRecv(CLIENT_CHARACTER_PART& rCharInfo);
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CREATE_NAK)
    DWORD m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_DESTROY_SYN)
    BYTE m_bySelectedCharSlotIndex;
    BYTE m_LastSocialSecurityNumber[MAX_SSN_STREAM_LENGTH];  //< 주민번호 뒷자리
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
    DWORD m_dwReason;         // 메시지가 발생한 원인
    DWORD m_dwAccumulateTime; // 적립 시간 (값:0000000초)
    WORD  m_wAccumulatePoint; // 적립 포인트
    BYTE  m_byRequirePoint;   // 필요 적립 포인트
PACKET_CG_FWD_CLOSE;
#endif

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
PACKET_CG_OPEN(CG_CHARINFO, CG_CHARINFO_CHANGE_APPEARANCE_BRD) // 캐릭터 외모 변경 결과를 알림
    PLAYERKEY player_key_; // 플레이어 키
    typedef BYTE ResultType;
    BOOST_STATIC_ASSERT(sizeof(ResultType) ==  sizeof(RC::ItemResultUtil::ValueType));
    BOOST_STATIC_ASSERT(sizeof(ResultType) ==  sizeof(RC::ChangeCharacterAppearanceResultUtil::ValueType));
    ResultType result_; // 외모 변경 결과
    CharacterAppearanceInfo character_appearance_; // 변경된 외모
    POSTYPE item_position_; // 아이템 위치
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
    BYTE     m_bForcedCommand;   // 클라요청에 의한 것인지, 서버에서의 강제 텔레포트인지
    DWORD    m_dwObjectKey;      // 워프하는 오브젝트 키
    WzVector vPos;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_MAP, CG_MAP_TELEPORT_SYN)
    WzVector vPos;               // 순간이동하고자 하는 위치
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
// 플레이어 필드 등장
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
    SLOTCODE m_TransSkillCode;  // 드래곤 변신상태라면 변신스킬코드를 보내준다.
	//_NA001955_110210_WING_COSTUME
    // CHANGES: f110607.3L, added field to support whether his wings spreading status has continued.
    bool continued_wings_spreading_;
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    CHANGE_PCROOM_STATUS m_PcRoomStatus; // ref. CHANGE_PCROOM_STATUS::eStatus
#endif
    DOMINATION_GUILD_INFO m_GuildInfo; //점령 길드 정보
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION    
    sDOMINATION_CONTINENT_LORD_INFO m_ContinentLordInfo;  //대륙 영주 정보
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
// 새로운 섹터에 캐릭터가 등장할 때의 정보들
//-------------------------------------------------------------------------------------------------
// 뒤에 PLAYER_RENDER_INFO,ETC_STATE_TOTAL_INFO,VENDOR_RENDER_INFO 가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = 50, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 PLAYER_VILLAGE_INFO,ETC_STATE_TOTAL_INFO,VENDOR_RENDER_INFO 가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_VILLAGE_ALLPLAYERS_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = 80, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 EQUIP_ITEM_BASEINFO가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD)
    enum { _MAX_PLAYER_INFO_SIZE = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 MONSTER_RENDER_INFO, ETC_STATE_TOTAL_INFO 가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLMONSTERS_CMD)
    enum { _MAX_MONSTER_INFO_SIZE = 80, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 SUMMONED_RENDER_INFO가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLSUMMONEDS_CMD)
    enum { _MAX_SUMMONED_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// 뒤에 MAPNPC_RENDER_INFO가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLMAPNPC_CMD)
    enum { _MAX_MAPNPC_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 TOTEMNPC_RENDER_INFO가 m_byCount 만큼 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ALLTOTEMNPC_CMD)
    enum { _MAX_TOTEMNPC_INFO_SIZE = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE, };
    BYTE    m_byCount;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
// 뒤에 MERCHANT_RENDER_INFO가 m_byCount 만큼 따라온다.
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
// 길드 관계로 인해 길드 랜더 인포 정보를 따로 보낸다.
// 점령지역 상태 정보 추가됨.
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
// 플레이어, 몬스터, 아이템이 필드에 등장
//-------------------------------------------------------------------------------------------------
// 뒤에 PLAYER_RENDER_INFO, ETC_STATE_TOTAL_INFO, EQUIP_ITEM_BASEINFO가 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_PLAYER_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 뒤에 PLAYER_VILLAGE_INFO, ETC_STATE_TOTAL_INFO, EQUIP_ITEM_BASEINFO가 따라온다.

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

// 뒤에 MONSTER_RENDER_INFO가 따라온다.
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

// 뒤에 TOTEMNPC_RENDER_INFO가 따라온다.
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_TOTEMNPC_ENTER_BRD)
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_MONSTER_DIR_ENTER_BRD)
    WORD wAngle;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


//  Boss 몬스터 등장
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_BOSS_MONSTER_ENTER_BRD)
    DWORD    m_dwBossCode; // 보스몬스터 코드
    WzVector m_vCurPos;    // 몬스터 위치
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


// 필드아이템
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
// 캐릭터 이동
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
        PACKET_RND_SHRBLK64_CONT(DWORD, m_byNumRoute); // 경로 상의 타일 개수
#else
        PACKET_RND_SHRBLK64_CONT(BYTE, m_byNumRoute); // 경로 상의 타일 개수
#endif
    PACKET_RND_SHRBLK64_END();

    TILEINDEX m_iRoute[ _MAX_PATHROUTE_SIZE ];        // 경로

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
    BYTE     m_byState;                              // 상태(걷기, 뛰기)
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
    DWORD     m_byNumRoute;                           // 경로 상의 타일 개수
#else
    BYTE      m_byNumRoute;                           // 경로 상의 타일 개수
#endif
    TILEINDEX m_iRoute[ _MAX_PATHROUTE_SIZE ];        // 경로
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



// 키보드 이동
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



// 전방 숏대시, 구르기 키보드 이동
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

// NPC 점프 명령
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_JUMP_BRD)
    bool     m_bJumpAttack;
    DWORD    m_dwObjectKey;
    WzVector m_wvEndPos;
PACKET_CG_FWD_CLOSE;

// 몬스터 점프 공격
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_JUMPRESULT_BRD)
    DWORD    m_dwObjectKey;
    DWORD    m_dwTargetObjectKey;
    BYTE     m_byAttackType;
    WzVector m_wvCurPos;
    DAMAGETYPE m_wDamage;
    WORD m_wTargetHP;
    BYTE m_byEffect;             // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)
PACKET_CG_FWD_CLOSE;


// 플레이어 정지
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

// NPC 정지
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_NPC_STOP_BRD)
    DWORD    dwObjectKey;
    WzVector wvCurPos;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// 플레이어 액션 끝남 알림
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_ACTION_EXPIRED_CMD)
PACKET_CG_FWD_CLOSE;


// 플레이어 강제 이동
PACKET_CG_FWD_OPEN(CG_SYNC, CG_SYNC_FORCED_WARP_BRD)
    DWORD    dwObjectKey;
    WzVector wvWarpPos;
    int GetSize() const { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 럭키몬스터 도망
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_CRYPTOGRAPHY
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// <Cryptography>
// 패킷 암호화 키 교환 및 주기적 알고리즘 변환
PACKET_CG_OPEN(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_KEY)
    BYTE      m_bySN;         // Sequence Number
    BYTE      m_byCryptInfo;  // 인코딩된 패킷 암호화키 블록 풀기 위한 키
    union {
        DWORD m_dwUserKey;
        BYTE  m_byCryptData[200]; // 알고리즘 정보 (키, 등등...)
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

// 암호화된 패킷 풀기위한...
struct MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED {
    BYTE  m_bySN;         // Sequence Number : received
    BYTE  m_byCRC;
    BYTE  m_byStream[1];  // 패킷마다 다른 크기를 가질 것이다.

    BYTE& GetSNKey() {    return m_bySN;  }
    void  SetSNKey(BYTE bySNKey) {    m_bySN = bySNKey;   }
    BYTE& GetCRC() {  return m_byCRC; }
    void  SetCRC(BYTE byCRC) {    m_byCRC = byCRC;    }
    BYTE* GetPacketStream() {     return m_byStream;  }
    DWORD GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// 패킷 암호화 하기 위한...
struct MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING {
    BYTE  m_bySN;         // Sequence Number : sending
    BYTE  m_byCRC;
    BYTE  m_byStream[1];  // 패킷마다 다른 크기를 가질 것이다.

    BYTE& GetSNKey() {    return m_bySN;  }
    void  SetSNKey(BYTE bySNKey) {    m_bySN = bySNKey;   }
    BYTE& GetCRC() {  return m_byCRC; }
    void  SetCRC(BYTE byCRC) {    m_byCRC = byCRC;    }
    BYTE* GetPacketStream() {     return m_byStream;  }
    DWORD GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  CG_ARMORKIT ==> HACKSHIELD
//
// <Waverix><ArmorKit>
//////////////////////////////////
// int값에 대한 문제... 타입이 절대적이지 못하다.
//
// Armorkit 관련 패킷
struct MSG_CG_ARMORKIT_FINDHACK_SYN : public MSG_BASE
{
    MSG_CG_ARMORKIT_FINDHACK_SYN()
    {
        m_byCategory = CG_ARMORKIT;
        m_byProtocol = CG_ARMORKIT_FINDHACK;
    }

    int  HackType;       // 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
    int  Size;           // Buffer 의 길이
    char Buf[1024];      // 데이터 버퍼-1024는 보안파트에서 전달받은 암묵적 최대값(성현창)

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
    BYTE buffer[GAMEGUARD_CSAUTH_PACKET_SIZE];  // 인증쿼리
    UINT32 query_size;     // 인증쿼리 사이즈

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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_BATTLE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


// 플레이어 공격
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_SYN)
    PACKET_RND_PAD_START(GETRNDNO_RNG04_T4(138));
        DWORD    dwClientSerial;         // 클라이언트에서 보낸 시리얼을 브로드 캐스트
    #ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        uint8_t byAttackType; // 공격 타입 (1타, 2타, 3타), ref:eATTACK_SEQUENCE
        uint8_t byAttackPropensity; // 강제공격 여부
        uint8_t style_presentation; // default(0), client animation presentation
    #else
        BYTE     byAttackPropensity  : 2;    // 강제공격 여부
        BYTE     byAttackType    : 6;    // 공격 타입
    #endif
        DWORD    dwTargetKey;            // 공격 대상의 오브젝트 키
        WzVector wvCurPos;               // 공격자의 현재위치
        WzVector wvDestPos;
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        ulong client_test_result;
    #endif
    PACKET_RND_PAD_END();

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_BRD)
    DWORD           dwAttackerKey;          // 공격자의 오브젝트 키
    BYTE            byAttackType;           // 공격 타입
    SLOTCODE        StyleCode;              // 스타일 코드
    DWORD           dwClientSerial;         // 클라이언트에서 보낸 시리얼을 브로드 캐스트
    WzVector        wvCurPos;               // 공격자의 현재위치
    DWORD           dwTargetKey ;           // 공격 대상의 오브젝트 키
    DAMAGETYPE      wDamage;                // 공격 데미지
    DWORD           dwTargetHP;             // 공격 후 타겟의 HP
    BYTE            byEffect;               // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)
    BYTE            byEtherComboCount;          // 에테르 탄환 장착 시 콤보
    void    Init()
    {
        byEtherComboCount = 0;
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_NAK)
    BYTE            m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// 일반 범위공격 결과정보
struct AREA_ATTACK_RESULT_INFO
{
    DWORD           m_dwTargetKey;          // 공격 당하는 오브젝트 키
    DAMAGETYPE      m_wDamage;              // 공격 데미지
    DWORD           m_dwTargetHP;           // 공격 후 타겟의 HP
    BYTE            m_byEffect;
    BYTE            m_byComboCount;         // 에테르 탄환 장착 시 콤보
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd;              // 공격 후 타겟의 SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
};

// 플레이어 일반 범위공격
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_PLAYER_AREA_ATTACK_BRD)
    DWORD           m_dwClientSerial;       // 공격 이벤트에 대한 고유 번호
    DWORD           m_dwAttackerKey;        // 공격자의 오브젝트 키
    DWORD           m_dwPrimaryTargetKey;   // 주공격 대상 오브젝트 키
    WzVector        m_wvCurPos;             // 공격자의 현재 좌표
    SLOTCODE        m_StyleCode;            // 스타일 코드
    BYTE            m_byAttackType;         // 공격 타입
    BYTE            m_byNumberOfTargets;    // 공격 대상의 수량
    AREA_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * m_byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;


// 발키리 기본 공격
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_ATTACK_SYN)
    DWORD           dwTargetKey;            // 공격 대상의 오브젝트 키
    DWORD           dwClientSerial;         // 클라이언트에서 보낸 시리얼을 브로드 캐스트
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_ATTACK_BRD)
    DWORD           dwAttackerKey;          // 공격자의 오브젝트 키
    DWORD           dwClientSerial;         // 클라이언트에서 보낸 시리얼을 브로드 캐스트
    DWORD           dwTargetKey ;           // 공격 대상의 오브젝트 키
    DAMAGETYPE      wDamage;                // 공격 데미지
    DWORD           dwTargetHP;             // 공격 후 타겟의 HP
    BYTE            byEffect;               // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)
PACKET_CG_FWD_CLOSE;

// 발키리 무기 재장전
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_RELOAD_SYN)   //사용안함
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_VKR_RELOAD_BRD)   //사용안함
    DWORD           m_dwObjKey;
PACKET_CG_FWD_CLOSE;


// 몬스터 기본 공격
PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_MONSTER_ATTACK_CMD)
    DWORD           m_dwObjectKey;
    DWORD           m_dwTargetObjectKey;
    BYTE            m_byAttackType;
    WzVector        m_wvCurPos;
    DAMAGETYPE      m_wDamage;
    WORD            m_wTargetHP;
    WORD            m_wTargetReserveHP;
    BYTE            m_byEffect;             // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)
PACKET_CG_FWD_CLOSE;

//// 플레이어 오브젝트 공격
//PACKET_CG_FWD_OPEN(CG_BATTLE, CG_BATTLE_P2O_ATTACK_SYN)
//  DWORD           dwTargetKey;            // 공격 대상의 오브젝트 키
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
    DWORD           m_dwMonsterObjKey;      // 상태저항을 성공한 몬스터 ObjKey
    BYTE            m_ScriptIndex;          // 스크립트에서 몇 번째 상태저항을 사용했나(0, 1)
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

// 스타일 시작 브로드
PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_PLAYER_ATTACK_BRD)
    DWORD           dwAttackerKey;              // 공격자의 오브젝트 키
    BYTE            byAttackType;               // 공격 타입
    SLOTCODE        StyleCode;                  // Style Code
    DWORD           dwClientSerial;             // 공격 이벤트에 대한 고유 번호
    DWORD           dwPrimaryTargetKey;         // 주공격 대상 오브젝트 키
    WzVector        wvCurPos;
    uint16_t             GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

// 스타일 결과 브로드
struct STYLE_ATTACK_RESULT_INFO
{
    DWORD           dwTargetKey;                // 공격 당하는 오브젝트 키
    DAMAGETYPE      wDamage;                    // 공격 데미지
    DWORD           dwTargetHP;                 // 공격 후 타겟의 HP
    WzVector        wvCurPos;
    WzVector        wvDestPos;
    BYTE            byEffect;
    BYTE            byEtherComboCount;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd; // 공격 후 타겟의 SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
};

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_PLAYER_ATTACK_RESULT_BRD)
    DWORD           dwClientSerial;             // 공격 이벤트에 대한 고유 번호
    DWORD           dwAttackerKey;              // 공격자의 오브젝트 키
    BYTE            byNumberOfTargets;          // 공격 대상의 수량

    STYLE_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * byNumberOfTargets);
    }
PACKET_CG_FWD_CLOSE;


// 스타일 지정
PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_SYN)
    SLOTCODE                    m_NewStyleCode;     // 지정할 스타일 코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_BRD)
    DWORD                       m_dwObjectKey;      // 스타일을 바꾼 캐릭터 오브젝트 키
    SLOTCODE                    m_CurStyleCode;     // 현재 스타일
    SLOTCODE                    m_NewStyleCode;     // 새로 지정된 스타일
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_STYLE, CG_STYLE_SELECT_STYLE_NAK)
    SLOTCODE                    m_CurStyleCode;
    BYTE                        m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// 스타일 등록
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
// 스킬 구사
//-------------------------------------------------------------------------------------------------

PACKET_CG_FWD_OPEN(CG_SKILL, CG_SKILL_ACTION_SYN)
    DWORD           m_dwClientSerial;           // Skill 이벤트에 대한 고유 번호
    DWORD           m_dwMainTargetKey;          // 주공격대상의 오브젝트 키
    BYTE            m_byAttackPropensity;       // 강제공격 여부
    SLOTCODE        m_SkillCode;                // Skill 코드
    WzVector        m_wvCurPos;                 // 클라이언트가 주는 플레이어의 현재 좌표
    WzVector        m_wvDestPos;                // 플레이어의 목적지 좌표
    WzVector        m_wvMainTargetPos;          // 주공격대상의 위치    

    uint16_t GetSize() const { return sizeof(*this); };

    void Encode();
    void Decode();
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// 스킬 액션 정보(InstantResult, DelayStart, DelayResult)
//-------------------------------------------------------------------------------------------------

struct SKILL_ACTION_BASE : public MSG_BASE_FORWARD
{
	SLOTCODE		m_SkillCode;						// Skill 코드
	DWORD			m_dwClientSerial;					// 공격 이벤트에 대한 고유 번호
	DWORD			m_dwAttackerKey;					// 공격자의 오브젝트 키
	DWORD			m_dwPrimaryTargetKey;				// 주공격 대상 오브젝트 키
	WzVector		m_wvMainTargetPos;					// 주공격대상의 위치
};

struct MSG_CG_SKILL_ACTION_INSTANT_RESULT_BRD : SKILL_ACTION_BASE
{
	WzVector		m_wvCurPos;							// 공격자의 현재 좌표
	WzVector		m_wvDestPos;						// 공격자의 목적지 좌표
	DWORD			m_dwAttackerHP;						// 스킬을 쓰고난 후 공격자의 HP
	DWORD			m_dwAttackerMP;						// 스킬을 쓰고난 후 공격자의 MP
	BYTE			m_byNumberOfTargets			: 6;	// 공격 대상의 수량
	BYTE			m_byNumberOfFieldEffect		: 2;	// 필드에 걸리는 이펙트가 있을 경우 그 수량
	
	uint16_t GetSize() { return sizeof(*this); };
};

struct MSG_CG_SKILL_ACTION_DELAY_START_BRD : SKILL_ACTION_BASE
{
	WzVector		m_wvCurPos;							// 공격자의 현재 좌표
	WzVector		m_wvDestPos;						// 공격자의 목적지 좌표
	
	uint16_t GetSize() { return sizeof(*this); };
};

struct MSG_CG_SKILL_ACTION_DELAY_RESULT_BRD : SKILL_ACTION_BASE
{
	DWORD			m_dwAttackerHP;						// 스킬을 쓰고난 후 공격자의 HP
	DWORD			m_dwAttackerMP;						// 스킬을 쓰고난 후 공격자의 MP
	BYTE			m_byNumberOfTargets			: 6;	// 공격 대상의 수량
	BYTE			m_byNumberOfFieldEffect		: 2;	// 필드에 걸리는 이펙트가 있을 경우 그 수량
	
	uint16_t GetSize() { return sizeof(*this); };
};


// Skill Casting Cancel  Syn
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_SYN )
	SLOTCODE		m_SkillCode;					// Skill 코드
PACKET_CG_FWD_CLOSE;

// Skill Casting Cancel Ack(BRD)
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_BRD )
	DWORD			m_dwObjectKey;					// 스킬 소유자 오브젝트키
	SLOTCODE		m_SkillCode;					// Skill 코드
PACKET_CG_FWD_CLOSE;

// Skill Casting Cancel NAK
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_CASTING_CANCEL_NAK )
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_ACTION_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// 폭탄투하 이펙트 결과
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
	DWORD			m_dwAttackerKey;					// 흡수를 발생시킨 공격자 키
	SLOTCODE		m_SkillCode;						// 주기데미지를 발생시키는 스킬코드
    DAMAGETYPE		m_wTargetDamage;					// 흡수 데미지
	DWORD			m_dwTargetHP;						// 타겟의 남은 HP
	DWORD			m_dwTargetMP;						// 타겟의 남은 MP
	DWORD			m_dwAttackerHP;						// 공격자의 남은 HP
	DWORD			m_dwAttackerMP;						// 공격자의 남은 MP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_VITAL_SUCTION_BRD )
	DWORD			m_dwAttackerKey;					// 흡수를 발생시킨 공격자 키
	DWORD			m_dwTargetKey;						// 타겟의 키.
	DWORD			m_dwHP;								// 타겟의 남은 HP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_EXECUTE_STATUS_CMD )
	DWORD			m_dwTargetKey;						// 타겟의 키.
	DWORD			m_wStatusCode;						// 상태코드.
	WORD			m_iStatusTime;						// 상태유지시간.
PACKET_CG_FWD_CLOSE;

//-------------------------------------------------------------------------------------------------
// 스킬 결과 정보
//-------------------------------------------------------------------------------------------------

struct SKILL_RESULT_BASE
{
	DWORD			m_dwTargetKey;						// 공격 당하는 오브젝트 키
	BYTE			m_bySkillEffect			: 5;		// NPC에 적용될 스킬 효과
	BYTE			m_byAbilityNum			: 3;		// Ability 개수(타격이 성공한 것만 보낸다)
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BOOL            isDamaged;                          // 1번Ability에 의해서 피해를 입었는지 여부를 체크
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
	int				m_iStatusTime;						// 상태 유지시간
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    WORD            overlap_status_count;               // 중첩버프일경우, 중첩횟수
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    uint16_t GetSize() const { return sizeof(*this); };
};

// eABILITY_DAMAGE
struct SKILL_RESULT_DAMAGE : SKILL_RESULT_CODE
{
    DAMAGETYPE		m_wDamage;							// 공격 데미지
	DWORD			m_dwTargetHP;						// 공격 후 타겟의 HP
	BYTE			m_byEffect;							// 데미지 이펙트(크리티컬 등등)
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD           target_sd; // 공격 후 타켓의 SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_ABNORMAL_STATUS
struct SKILL_RESULT_STUN : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// 공격대상의 현재 좌표

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_KNOCKBACK
struct SKILL_RESULT_POSITION : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// 공격대상의 현재 좌표
	WzVector		m_wvDestPos;						// 공격대상의 목적지 좌표

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_EXHAUST_HP, eABILITY_EXHAUST_MP
struct SKILL_RESULT_EXHAUST : SKILL_RESULT_CODE
{
	DWORD			m_dwTargetHP;						// 스킬 사용 후 타겟의 남은 HP
	DWORD			m_dwTargetMP;						// 스킬 사용 후 타겟의 남은 MP

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_FIGHTING_ENERGY_NUM_INCREASE, eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY
struct SKILL_RESULT_FIGHTING_ENERGY : SKILL_RESULT_CODE
{
	SHORT			m_sFightingEnergyCount;				// 현재 투기 개수

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_RESURRECTION
struct SKILL_RESULT_RESURRECTION : SKILL_RESULT_CODE
{
	WzVector		m_wvCurPos;							// 부활대상의 현재 좌표	
	DWORD			m_dwTargetHP;						// 부활대상의 HP
	DWORD			m_dwTargetMP;						// 부활대상의 MP

    uint16_t GetSize() const  { return sizeof(*this); };
};

// eABILITY_MONSTER_TRANSFORMATION
struct SKILL_RESULT_MONSTER_TRANSFORMATION : SKILL_RESULT_CODE
{
	DWORD			m_dwHP;
	DWORD			m_dwMaxHP;
	WORD			m_wMoveSpeedRatio;		// 이동속도(100을 곱한 %수치)
	WORD			m_wAttSpeedRatio;		// 공격속도(100을 곱한 %수치)

    uint16_t GetSize() const  { return sizeof(*this); };
};

struct SKILL_RESULT_SELF_DESTRUCTION : SKILL_RESULT_CODE
{
	DWORD			m_dwHP;
	DWORD			m_dwTargetHP;

    uint16_t GetSize() const  { return sizeof(*this); };
};

//-------------------------------------------------------------------------------------------------
// 주기 데미지, 반사 데미지, 기타
//-------------------------------------------------------------------------------------------------


PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PERIODIC_DAMAGE_BRD )
	DWORD			m_dwAttackerKey;					// 주기데미지를 발생시킨 공격자 키
	SLOTCODE		m_SkillCode;						// 주기데미지를 발생시키는 스킬코드
	BYTE			m_byNumberOfTargets;				// 공격 대상의 수량
	DAMAGE_INFO		m_DamageInfo[MAX_TARGET_NUM];		// 공격 대상의 데미지 정보

	int				GetSize()
	{
		return sizeof(*this) - sizeof(DAMAGE_INFO) * (MAX_TARGET_NUM - m_byNumberOfTargets);
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_REFLECT_DAMAGE_BRD )
	DWORD			m_dwAttackerKey;					// 주기데미지를 발생시킨 공격자 키
	DWORD			m_dwTargetKey;						// 공격 당하는 오브젝트 키
	SLOTCODE		m_SkillCode;						// 피격 스킬코드
	WORD			m_wAbilityCode;						// 피격 어빌리티 코드
    DAMAGETYPE      m_wDamage;                          // 공격 데미지
	DWORD			m_dwTargetHP;						// 공격 후 타겟의 HP
	DWORD			m_dwTargetResrveHP;					// 공격후 타겟의 적립된 HP
	int GetSize() { return sizeof(*this); };
PACKET_CG_FWD_CLOSE;


//-------------------------------------------------------------------------------------------------
// 스킬 포인트 업
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
        SKILL_TOTAL_INFO skill_total_info;                       // 새로 등록되는 스킬슬롯 정보
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
// 패시브 스킬 리스트 브로드
//-------------------------------------------------------------------------------------------------
PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PASSIVE_SKILL_LIST_CMD )
	DWORD						m_dwObjectKey;					// 스킬 소유자 오브젝트키
	BYTE						m_byCount;
	SLOTCODE					m_SkillCode[MAX_SKILL_SLOT_NUM];
	int GetSize() 
	{
		return (sizeof(*this)-(MAX_SKILL_SLOT_NUM-m_byCount)*sizeof(SLOTCODE));
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SKILL, CG_SKILL_PASSIVE_SKILL_ADD_CMD )
	DWORD						m_dwObjectKey;					// 스킬 소유자
	SLOTCODE					m_OldSkillCode;					// 이전 스킬 코드(처음으로 습득하는 경우는 0이다)
	SLOTCODE					m_NewSkillCode;					// 변경된 혹은 습득한 스킬 코드
PACKET_CG_FWD_CLOSE;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_CONVERSATION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로 방 테스트 할때 사용하는 패킷 시작!!!!!
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


// 미션 로비 방 리스트 요청
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


// 헌팅 로비 방 리스트 요청
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


// PVP 로비 방 리스트 요청
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


// 경쟁헌팅 로비 방 리스트 요청
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

// 참가가능한 경쟁헌팅 방 리스트 요청
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


// 진행된 헌팅 방 리스트 요청
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


// 특정 방 리스트 새로고침
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_SYN )
	KEYTYPE				m_RoomKey;								//< 새로고침할 방의 키
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
	KEYTYPE				m_RoomKey;								//< 방정보가 없다. 파괴되었다.
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LIST_REFRESH_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;


// 바로가기
// 비밀방은 바로가기되지 않는다.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_SHORTCUT_SYN )
	BYTE				m_byRoomType;		//< eZONETYPE 의 값 eZONETYPE_HUNTING or eZONETYPE_MISSION의 값중 하나
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


// 아이디로 방 찾기
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
	DWORD				m_dwErrorCode;						//< return 5601 : 접속되어 있지 않거나, 개설된 방이 없으면
PACKET_CG_CLOSE;

// 방장이 로비(헌팅or미션orPVP)방을 만든다
PACKET_CG_OPEN(CG_ZONE, CG_ZONE_LOBBY_CREATE_SYN)
    MAPCODE m_MapCode;
    BYTE    m_byMaxLimitUserNum;
    BYTE    m_byRoomType            : 6;            //< eZONETYPE 의 값 eZONETYPE_HUNTING or eZONETYPE_MISSION의 값중 하나
    BYTE    m_byRoomPublic          : 2;            //< eZONEPUBLIC
    char    m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
    BASE_ROOMINFO   m_RoomInfo;

    char m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];

    int  GetSize() const
    {// m_byRoomPublic체크를하라(클라이언트신뢰할수없다.)
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

// 로비 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_SYN )
	KEYTYPE			m_LobbyKey;
	BYTE			m_byRoomPublic;		//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{// m_byRoomPublic체크를하라(클라이언트신뢰할수없다.)
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_LOBBY_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_ACK )
	DWORD			m_dwMasterKey;					//< 방장 플레이어 키
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 경쟁헌팅 조인로비 생성
PACKET_CG_OPEN(CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CREATE_SYN)
    KEYTYPE         m_LinkHuntingKey;                       //< 링크시킬 경쟁헌팅방 키
    BYTE            m_byRoomPublic;                         //< eZONEPUBLIC
    char            m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
    BASE_ROOMINFO   m_RoomInfo;                             //< 생성할 방 정보
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

// 레디 버튼을 누름
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_BRD )
	DWORD			m_PlayerKey;							//< 레디를 누른 플레이어
PACKET_CG_CLOSE;

// 레디 버튼을 누름
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_ACK )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_BRD )
	DWORD			m_PlayerKey;							//< 레디취소를 누른 플레이어
PACKET_CG_CLOSE;

// 방참가한 사람이 기존의 방의 정보 요청
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_REQUEST_INFO_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_REQUEST_INFO_ACK )
	SLOTCODE			m_MapCode;
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
	BYTE				m_NonBlockNum	:4;
	BYTE				m_Count			:4;
	DWORD				m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM];	//< 자기자신 불 포함 + 방장 불 포함
	int					GetSize()
	{
		return (sizeof(*this) - sizeof(DWORD)*(MAX_PARTYMEMBER_NUM-m_Count));
	}
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK )
    SLOTCODE m_MapCode;
    BYTE     m_NonBlockNum;
    BYTE     m_Count;
    uint16_t m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM]; //< 자기자신 불 포함 + 방장 불 포함
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
	PVP_LOBBY_INFO		m_PlayerInfo[MAX_PARTYMEMBER_NUM];			//< 자기자신 포함 + 방장포함
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
    uint16_t m_dwReadyPlayerKey[MAX_PARTYMEMBER_NUM]; //< 자기자신 불 포함 + 방장 불 포함
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

// 인원수 변경
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


// 인원수 변경
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

// 플레이어 강퇴
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_SYN )
	DWORD			m_PlayerKey;							//< 강퇴할 플레이어 키
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 플레이어 팀 변경
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_SYN )
	BYTE			m_Team;									//< 변경할 팀 (1or2)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_BRD )
	DWORD			m_dwPlayerKey;							//< 변경한 플레이어 키
	BYTE			m_Team;									//< 변경한 플레이어의 팀
PACKET_CG_CLOSE;


// 맵 변경
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


// 로비의 헌팅 제한 설정 바꿈( 난이도, 보너스)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_SYN )
	ADDITIONAL_ROOMINFO m_AdditionalInfo;			//< 변경할 설정값
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_BRD )
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;			//< 변경된 설정값
PACKET_CG_CLOSE;


// 로비의 PVP 제한 설정 바꿈( 룰, 모드)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_SYN )
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;			//< 변경할 설정값
PACKET_CG_CLOSE;

//PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_ACK )
//PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_BRD )
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;
	BYTE				m_Count;
	PVP_LOBBY_INFO		m_PlayerInfo[MAX_PARTYMEMBER_NUM];			//< 자기자신 포함 + 방장포함
    // CHANGES: f110608.6L
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_PlayerInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_PlayerInfo[0]) * m_Count);
    };
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_PVP_INFO_BRD )
	// 들어온 사람의 정보
	DWORD			m_dwPlayerKey;
	BYTE			m_Team;
PACKET_CG_CLOSE;

// 로비의 헌팅 제한 설정 바꿈( 난이도, 보너스)
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN )
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;			//< 변경할 설정값
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_ACK )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_NAK )
	DWORD					m_dwErrorCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD )
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;			//< 변경된 설정값
PACKET_CG_CLOSE;

// 헌팅 생성 ( START를 누름 )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_CREATE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 미션 생성 ( START를 누름 )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_CREATE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T4(229), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// PVP 생성 ( START를 누름 )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_CREATE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_CREATE_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 경쟁헌팅 생성 ( START를 누름 )
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_CREATE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T3(229), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_CREATE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// 경쟁헌팅 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTING_MIGRATE_SYN )
	KEYTYPE			m_CHuntingKey;							//< 참가할 헌팅 키
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
// 방참가 요청
//-------------------------------------------------------------------------------------------------
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTINGLOBBY_JOIN_CMD )
	bool				m_bRequireConfirm;		// 유저 동의를 받아야 하는가
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


// 헌팅 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_SYN )
	KEYTYPE			m_HuntingKey;							//< 참가할 헌팅 키
	BYTE			m_byRoomPublic;							//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_ACK )
	KEYTYPE			m_HuntingKey;							//< 참가한 헌팅 키
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 미션 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_JOIN_ACK )
	KEYTYPE			m_MissionKey;							//< 참가한 미션 키
	MAPCODE			m_MapCode;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// PVP 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PVP_JOIN_ACK )
	KEYTYPE			m_PVPKey;							//< 참가한 미션 키
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
		REASON_INTEND,			// 자의적 미션 탈퇴
		REASON_EXPULSION,		// 추방
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


// 릴레이 헌팅 생성
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_CREATE_SYN )
	MAPCODE				m_MapCode;			//< 생성할 맵 정보
	BASE_ROOMINFO		m_RoomInfo;			//< 생성할 방 정보
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;	//< 생성할 부가 정보
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
	CODETYPE			m_MapCode;			//< 생성할 맵 정보
	BASE_ROOMINFO		m_RoomInfo;			//< 생성할 방 정보
	ADDITIONAL_ROOMINFO	m_AdditionalInfo;	//< 생성할 부가 정보
	BYTE				m_byRoomPublic;							//< eZONEPUBLIC
	char				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int					GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_RELAY_CREATE_BRD));
	}
PACKET_CG_CLOSE;


// 릴레이 헌팅 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_SYN )
	KEYTYPE			m_HuntingKey;							//< 참가할 헌팅 키
	BYTE			m_byRoomPublic;							//< eZONEPUBLIC
	char			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	int				GetSize()
	{
		return (m_byRoomPublic==eZONEPUBLIC_PUBLIC?sizeof(*this)-sizeof(char)*MAX_ROOMPASSWORD_LENGTH:sizeof(MSG_CG_ZONE_HUNTING_RELAY_JOIN_SYN));
	}
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_ACK )
//	KEYTYPE			m_HuntingKey;							//< 참가한 헌팅 키
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_NAK )
	DWORD			m_dwErrorCode;
PACKET_CG_CLOSE;

// 포탈을 이용한 필드간 이동.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_SYN )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG32_T3(996));
	PACKET_RND_SHRBLK64_CONT(WORD, m_wIndex);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_ACK )
	WORD			m_wIndex;							// 이용하려는 포탈 idx
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// 죽었을때 필드에서 마을로 이동.
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_SYN )
	PACKET_RND_PAD_CONT(GETRNDNO_RNG04_T2(44), 1);
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_ACK )
	WORD			m_wMapCode;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_RETURN_VILLAGE_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_CLOSE;

// 헌팅로비 최대유저수 변경
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD )
	BYTE			m_byMaxLimitUser;
PACKET_CG_CLOSE;

// GM 명령에 의한 필드간 이동
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_GMPORTAL_FIELD_MOVE_CMD )
	CODETYPE		m_MapCode;
PACKET_CG_CLOSE;


// 릴레이 미션 생성
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_SYN )
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_BRD )
	KEYTYPE			m_LobbyKey;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_NAK )
	BYTE			m_ErrorCode;
PACKET_CG_CLOSE;


// 릴레이 미션 참가
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_JOIN_SYN )
	KEYTYPE			m_LobbyKey;
PACKET_CG_CLOSE;
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_MISSION_RELAY_JOIN_ACK )
	KEYTYPE			m_LobbyKey;
	DWORD			m_dwMasterKey;
PACKET_CG_CLOSE;

// 방의 SpecialType이 바뀜
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_CHANGE_SPECIALTYPE_CMD )
	BYTE			m_SpecialType;		// eROOM_SPECIAL_TYPE
PACKET_CG_CLOSE;

// [11/18/2009 namka199] _NA_0_20091118_GAMEROOM_BATTLE_RESULT 게임방의 전투 결과 통보
PACKET_CG_OPEN( CG_ZONE,  CG_ZONE_PLAYER_BATTLE_RESULT_SYN )
    BYTE    m_OwnerGuild; // 0 : 자신의 길드 그외 상대 길드
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

// 방 번호로 방 찾기
PACKET_CG_OPEN( CG_ZONE, CG_ZONE_FIND_ROOM_FROM_NO_SYN )
	WORD			m_RoomNo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD )
	MAPCODE			m_MapCode;		// 클라의 이전마을 설정을 변경시킨다. (캐릭터 시작 마을로...)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_ROOM_JOIN_ACK )
	KEYTYPE			m_ktROOMKEY;				//< 참가한 방 키
	MAPCODE			m_MapCode;					//< 참가한 방 맵코드
	// 이전 JOIN계열과 포맷 위치를 맞추기 위한 작업
	FIELDCODE		m_fcCODE;					//< 참가한 방 필드코드 (파티장 위치한 필드)
	BYTE			m_byZONETYPE;				//< 참가한 방 타입
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN )
#if defined(_CHINA) || defined(_JAPAN)	//	(WAVERIX)(080818)(FAKE-CODE) 나중에 수정하겠음. ㅠㅠ
	#define __EXTRA_OP	(1)
#else
	#define __EXTRA_OP	(0)
#endif
	BYTE				__dummy1[(PROTOCOL_GENERATOR_ZONE_EX_077)%4 + __EXTRA_OP];
#undef __EXTRA_OP
	POSTYPE				m_ItemPos;	// 사용하고자 하는 아이템... (서버에서 할 수 있기야 하겠지만, 불필요한 연산이나 오류 방어처리 피하자)
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE_EX , CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK )
	enum eRESULT { FAIL = PROTOCOL_GENERATOR_ZONE_EX_089, SUCCESS = PROTOCOL_GENERATOR_ZONE_EX_091 };
	enum eFAIL_RC_CODE { RET_ITEM = PROTOCOL_GENERATOR_ZONE_EX_014, RET_ROOM = PROTOCOL_GENERATOR_ZONE_EX_016 };
	BYTE				m_Result;	// eRESULT
	union {
		struct {
			BYTE		m_byKind;	// eFAIL_RC_CODE { RET_ITEM 이면 eITEM_RESULT, RET_ROOM이면 eROOM_RESULT }
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

	// for Client 성공 여부 판단용
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
    DWORD remain_time_; //밀리세컨드
    BYTE event_state_;
PACKET_CG_CLOSE;
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로 방 테스트 할때 사용하는 패킷 끝!!!!
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


// CG_ITEM
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_SYN  )
	SLOTIDX						m_fromIndex;
	SLOTIDX						m_toIndex;
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
    DURATYPE move_quantity_; // 이동 수량, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_MOVE_ACK  )
	SLOTIDX						m_fromIndex;
	SLOTIDX						m_toIndex;
	POSTYPE						m_fromPos;
	POSTYPE						m_toPos;
    DURATYPE move_quantity_; // 이동 수량, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
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

// 퀵슬롯의 연결된 아이템 정보 갱신 (클라이언트 -> 서버)
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_QUICK_UPDATE_LINKED_ITEM_CMD)
    typedef WORD MessageSize;
    POSTYPE target_quick_slot_position; // 갱신할 퀵슬롯 위치
    SLOTIDX new_linked_item_container_type; // SI_INVENTORY, SI_EQUIPMENT
    POSTYPE new_linked_item_position; // 연결할 아이템 위치
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
	DWORD						m_dwFieldItemObjectKey;			//< 실패한 아이템 ObjectKey
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
    DURATYPE combine_quantity_; // 합칠 수량, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
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

//아이템 사용시 모션과 타켓이 있는경우 모션 시작 
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_SYN  )
SLOTIDX						m_atIndex;
POSTYPE						m_atPos;
WzVector					m_wvMainTargetPos;			// 주공격 위치	
PACKET_CG_FWD_CLOSE;

//아이템 모션 시작을 알린다.
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_BRD  )
DWORD						m_dwAttackerKey;
CODETYPE					m_ItemCode;
WzVector					m_wvMainTargetPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_MOTION_NAK  )
BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//아이템 사용시 타켓이 있는경우(자기자신이 아닌경우).
PACKET_CG_INHERIT_OPEN( CG_ITEM, CG_ITEM_USE_SKILL_ACTION_SYN, MSG_CG_ITEM_USE_SYN )
WzVector		m_wvMainTargetPos;			// 주공격 위치	
BYTE			byAttackPropensity;			// 강제공격 여부
CODETYPE target_code_; // 아이템 사용 대상의 코드, _NA_000587_20100928_DOMINATION_BUFF_ITEM
int GetSize() { return sizeof(*this); };

PACKET_CG_INHERIT_CLOSE;


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
// 기간 연장 아이템 사용
// 해당 아이템의 사용은 기간연장 쿠폰/아이템(무기,갑옷) 모두 인벤토리에 위치해야한다.
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_EXTEND_DATE_SYN )
    POSTYPE extend_item_position_;  // 기간 연장 쿠폰
    POSTYPE target_item_position_;  // 무기, 갑옷, 악세서리등의 기간 연장 아이템.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_EXTEND_DATE_ACK )
    POSTYPE			extend_item_position_;  // 기간 연장 쿠폰 위치, 해당위치의 아이템을 1 감소 시킨다.
    POSTYPE			target_item_position_;  // 기간 연장된 아이템의 위치
    ITEMSTREAMEX	target_item_stream_;    // 기간 연장된 아이템의 갱신된 값
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
            DURATYPE sell_quantity_; // 판매 수량, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
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
	BYTE						m_ShopItemIndex; //< SHOPINFO::_MAX_SELLITEM_NUM를 넘지 않아야 함
    DURATYPE buy_quantity_; // 구매 수량, _NA_000070_20100609_ENHANCED_ITEM_DIVISION
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
	// 무조건 인벤토리임, 삭제할 아이템
	INVEN_DELETE_TOTAL_INFO		m_ItemInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize +  m_ItemInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
// 길드PVP 후 이그니스뺏지 교환 보상
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
// 아이템 조합
//
// STREAM으로 보내는 것을 바뀐 정보만을 보내어서 최적화 할 여지가 있다.
// @@@ OPT
//---------------------------------------------------------------------------------------------------


PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_ENCHANT_SYN )
	CODETYPE			m_NPCCode;
	POSTYPE				m_TargetPos;					//< 인첸트 할 아이템
	BYTE				m_SuccessRateIndex;				//< 선택한 성공 확률 인덱스 값 ( 0 ~ 3 )
#ifdef _NA_003966_20111227_ADD_ENCHANT
    POSTYPE down_guard_item_pos; // 인첸트 실패시 다운 및 소멸 방지 아이템 위치
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
	POSTYPE				m_TargetPos;					//< 인첸트 할 아이템
PACKET_CG_FWD_CLOSE;

// ack시에 클라이언트는 보석을 제거하고
// m_TargetItemEnchant는 성공 혹은 실패에 따른 인첸트 결과 값
// m_TargetItemEnchant == 0이면 대상 아이템 삭제
// m_TargetItemEnchant != 0아니면 대상 아이템의 인첸트 다운
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
    CODETYPE target_item_code; // 쟈드를 삽입할 대상 아이템 코드
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
	BYTE					m_byMakingNum;				//< 만들려는 개수: default는 1이겠지?
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_SUCCESS_ACK )
	BYTE					m_byMakingNum;		//조합 요청 갯수
	BYTE					m_bySuccessNum;		//조합 성공 갯수
	MONEY					m_Money;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    BYTE                    m_byPreventNum;     //조합 재료 소멸 방지 갯수
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	INVENTORY_TOTAL_INFO	m_ItemInfo;         // 획득한 아이템(제작결과)

	int GetSize()
	{
		return ( sizeof(*this) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX) );
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_FAILED_ACK )
MONEY					m_Money;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
BYTE                    m_byPreventNum;     //조합 재료 소멸 방지 갯수
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_COMPOSE_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_CRYSTALIZE_SYN )
    CODETYPE				m_NPCCode;
    SLOTCODE				m_CrystalizeListCode;
    POSTYPE					m_TargetPos;				//< 분해할 대상 위치
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
	POSTYPE				m_TargetPos;				//< extrastone을 적용할 대상 위치
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM,  CG_ITEM_EXTRASTONE_USE_ACK )
	BYTE				m_NOptionAttrValueIndex;	//< SCItemSlot::SetNOptionAttr(m_NOptionAttrValueIndex) 함수 사용
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

// 아이템 수리
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

// 내구도 감소로 인한 아이템 소멸
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DESTROY_DURA_CMD )
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
PACKET_CG_FWD_CLOSE;

// 스크롤 아이템 사용
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PORTAL_SYN )
	POSTYPE				m_CoordPos;
	POSTYPE				m_PortalPos; 
PACKET_CG_FWD_CLOSE;

// 아이템 슬롯정보 변화
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_CHANGE_ITEM_INFO_CMD )
	SLOTIDX				m_SlotIndex;
	POSTYPE				m_SlotPos;
	ITEMSTREAMEX		m_ItemStream;
PACKET_CG_FWD_CLOSE;

// 아이템 삭제
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_DELETE_CMD  )
	BYTE				m_byReason;				// 삭제원인(eITEM_DELETE_RESULT)
	SLOTIDX				m_atIndex;
	POSTYPE				m_atPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXTRAINVENTORY_ADD_CMD  )
	BYTE				m_ExtraInventoryTabCount; //< 유료화 탭 전체 개수
PACKET_CG_FWD_CLOSE;

// 좌표 초기화
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_CLEAR_COORD_SYN )
	POSTYPE				m_ClearPos;
	POSTYPE				m_CoordPos;
PACKET_CG_FWD_CLOSE;

//스텟 초기화
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
	INT					m_iRemainStat;				//<	남은 스탯
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_STAT_ALL_CHANGE_CMD )
	SHORT				m_sStrength;
	SHORT				m_sDexterity;
	SHORT				m_sVitality;
	SHORT				m_sInteligence;
	SHORT				m_sSpirit;
	SHORT				m_sSkillStat1;
	SHORT				m_sSkillStat2;	
	INT					m_iRemainStat;				//<	남은 스탯	
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_INIT_STAT_NAK )
	BYTE				m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EXTEND_CASH_SLOT_CMD  )
	BYTE				m_ExtraEquipCashSlotCount; //< 추가된 유료 캐쉬슬롯 전체개수
PACKET_CG_FWD_CLOSE;

// 서버에서 보내지 않음. 클라에서 처리하지않음. 추후 삭제 또는 다른방향으로사용...
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
	POSTYPE				m_PetItemPos;	//성공 시에만 사용
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
	BOOL				m_Remove; // 판도라 상자에 아이템이 없다. 삭제필요
	BOOL				m_MoveInventory; // 판도라 -> 인벤토리
	INT					m_Result;
	POSTYPE				m_InventoryPos;
	ITEMSTREAMEX		m_PandoraItemStream;
	INVENTORY_TOTAL_INFO m_InventoryInfo;
#ifdef __NA_1234_LOTTERY_ITEM_RESULT_BUG_FIX
	int GetSize()
	{
        // (WAVERIX) (090721) (BUG-FIX) 패킷 크기 계산... 이러지 맙시다...-.-;;; 단순하게 만드는 방법을 고민합시다.
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

// 따라서 여기 ACK 보낼때, 판도라 상자안에 보여질 아이템 정보가 있어야 한다. 
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
	POSTYPE					m_WeaponItemPos;	//성공시에만 사용
PACKET_CG_FWD_CLOSE;

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
// 소켓 다시 생성 요청
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RECREATE_SOCKET_SYN)
    typedef WORD MessageSize;

    POSTYPE request_item_pos; // 소켓 다시 생성 요청 아이템 인벤토리 위치
    POSTYPE target_item_pos; // 소켓을 다시 생성할 대상 아이템 인벤토리 위치

    // 초기화한다. 메시지를 사용하기 전에 안전하게 먼저 호출 해주자.
    void Init()
    {
        request_item_pos = INVALID_SLOT_POS;
        target_item_pos = INVALID_SLOT_POS;
    }

    // 요청을 보내기전에 크기를 수동으로 계산하지 말고 이 함수를 사용하자.
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;
// 소켓 다시 생성 응답
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RECREATE_SOCKET_ACK)
    typedef WORD MessageSize;
    typedef int ResultCode;
    
    // 처리 결과 
    // 성공 : RC::RC_ITEM_SUCCESS, 
    // 실패 : 아이템 관련 실패 코드
    ResultCode result_code;

    // 초기화한다. 메시지를 사용하기 전에 안전하게 먼저 호출 해주자.
    void Init()
    {
        result_code = RC::RC_ITEM_FAILED;
    }
    
    // 요청을 보내기전에 크기를 수동으로 계산하지 말고 이 함수를 사용하자.
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_CG_FWD_CLOSE;
#endif // _NA_000251_20100727_SOCKET_SYSTEM

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_SYN)
	SLOTIDX					m_atIndex; // 컨테이너 인덱스
	POSTYPE					m_atPos; // 체인지 아이템 인덱스
	POSTYPE					m_ChangeItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_ACK)
	SHORT					m_ItemCount;
	POSTYPE					m_atPos; // 체인지 아이템 인덱스
	POSTYPE					m_ChangeItemPos; // 바뀔 아이템 인덱스
	CODETYPE				m_ItemCodes[MAX_CHANGE_ITEMSTAT_SLOT_NUM];
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_NAK)
	DWORD					m_ErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_STAT_CHANGE_SELECT_SYN)
	BOOLEAN					m_bSelect;// 아이템 선택:TRUE, 그냥 닫음:FALSE
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

//성공, 실패 모두 이 패킷.(구분은 Result Code로 하세요)
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK)
	INT						m_ResultCode;					//RC::eITEM_RESULT
PACKET_CG_FWD_CLOSE;

//클라가 서버로 보내는 SYN
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ACTIVE_ETHER_BULLET_SYN)
	BYTE					m_bActive;					// 1이면 활성화 0이면 비활성화
	POSTYPE					m_ActiveBulletItemPos;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ACTIVE_ETHER_BULLET_ACK)
	INT						m_ResultCode;					//RC::eITEM_RESULT
	BYTE					m_BulletEffectCode;					//
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_ETHER_ACTIVE_STATE_BRD)
	DWORD					m_dwPlayerKey;
	BYTE					m_bActive;					// 1이면 활성화 0이면 비활성화
	BYTE					m_BulletEffectCode;					//
PACKET_CG_FWD_CLOSE;

//서버가 클라로 보내는 CMD
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

// 타기, 내리기, 기타 액션 제어 요청
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_SYN)
    RiderControl header;
    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

// 타기, 내리기, 기타 액션 제어 요청에 대한 응답, 렌더링 동기화
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_BRD, MSG_CG_ITEM_RIDER_CONTROL_SYN)
    RiderControlData data;

    void SetUnknownFailed() {
        header.request = header.eReq_None;
        header.result = header.eRet_Failed;
        //header.result_code <- 추가 정의되면 입력하기로 하자.
        data.data_type = data.eDataType_None;
    }

    void Clear();
    WORD GetSize() const {
        return sizeof(*this) - sizeof(data) + data.GetSize();
    }
PACKET_CG_INHERIT_CLOSE;

// 타기, 내리기, 기타 액션 제어를 서버의 필요에 따라 제어할 경우 (예: 미션 입장시)
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_CMD, MSG_CG_ITEM_RIDER_CONTROL_BRD)
    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_INHERIT_CLOSE;

// Rider에 Parts 장착/해제 요청
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_SYN)
    RiderControl header;

    void Clear();
    WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_CG_FWD_CLOSE;

// Rider에 Parts 장착/해제 요청에 대한 응답, 렌더링 동기화
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_BRD, MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN)
    RiderControlData data;

    void SetUnknownFailed() {
        header.request = header.eReq_None;
        header.result = header.eRet_Failed;
        //header.result_code <- 추가 정의되면 입력하기로 하자.
        data.data_type = data.eDataType_None;
    }

    void Clear();
    WORD GetSize() const {
        return sizeof(*this) - sizeof(data) + data.GetSize();
    }
PACKET_CG_INHERIT_CLOSE;

// Rider에 Parts 장착/해제 제어를 서버의 필요에 따라 제어할 경우 (예: 기한 만료시, Rider|Parts)
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
    BYTE    m_First;    // 보상받기 버튼을 눌렀을때 TRUE, 그외 FALSE
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
    BYTE                    m_byDropItemOrder;  // 드롭 아이템 정렬 순서
    WORD                    m_wAccumulatePoint; // 남은 적립 포인트
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
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN) // 캐릭터 외모 변경 아이템 사용 요청
    POSTYPE item_position_; // 아이템 위치
    CharacterAppearanceInfo character_appearance_; // 변경할 외모
    //
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CG_CLOSE;

// 아이템 합성 또는 분해 요청
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN) 
    enum 
    { 
        kZardRequest = 1, // 쟈드 합성
        kCubeRequest = 2  // 큐브 합성/분해
    };
    enum { kMaxMaterialSize = 9 }; // 재료 최대 개수
    typedef BYTE RequestType;
    typedef CODETYPE ReservedCode;
    typedef BYTE MaterialListSize;
    typedef WORD MessageSize;
    struct MaterialInfo // 재료 정보
    {
        POSTYPE inventory_position_; // 재료의 인벤토리 위치        
        BYTE selected_number_; // 합성 또는 분해하기 위해 인벤토리에서 선택한 개수
        BYTE matched_index_; // 합성법상의 일치하는 재료 인덱스
    }; // MaterialInfo

    RequestType request_type_; // 요청 종류 (쟈드/큐브)
    bool is_composition_; // 합성 여부 (합성이 아니라면 분해)
    // 예약 코드
    // 쟈드 합성일 때 첨가제의 인벤토리 위치 (사용안한다면 INVALID_POSTYPE_VALUE : Define.h)
    // 큐브 합성일 때 합성코드
    // 큐브 분해일 때 분해 포인트
    ReservedCode reserved_code_;
    POSTYPE using_item_position_; // 아이템을 사용해서 요청했다면 아이템 위치
    MaterialListSize material_list_size_; // 합성 또는 분해할 재료 개수
    MaterialInfo material_list_[kMaxMaterialSize]; // 합성 또는 분해할 재료 목록

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            ((kMaxMaterialSize - material_list_size_) * sizeof(MaterialInfo)));
    }
PACKET_CG_CLOSE; // CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN
// 아이템 합성 또는 분해 응답
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK) 
    typedef int ResultCode;
    typedef WORD MessageSize;
    bool is_composition_; // 합성 여부 (합성이 아니라면 분해)
    ResultCode result_code_; //요청 처리 결과
    INVENTORY_TOTAL_INFO added_inventory_items_; // 보상으로 인벤토리에 추가된 아이템 정보
    // 재료로 사용되어 삭제된 인벤토리 아이템 정보는 MSG_CG_ITEM_LOSE_ACK로 보낸다.
    // 이미 구현되어 있으므로 추가 작업은 필요없다.

    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this) - 
            (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM - 
            added_inventory_items_.m_InvenCount - 
            added_inventory_items_.m_TmpInvenCount) * sizeof(ITEMSLOTEX));
    }
PACKET_CG_CLOSE; // CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK
// 성공한 합성 또는 분해 내역들 요청
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN) 
    typedef MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::RequestType RequestType; 
    typedef WORD MessageSize;
    RequestType request_type; // 요청 종류
    bool is_composition; // 합성 여부

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
// 성공한 합성 또는 분해 내역들 응답
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
    enum { kMaxHistoryListSize = ItemCompositionOrDecompositionHistory::kMaxHistories };
    typedef int ResultCode;    
    typedef BYTE HistoryListSize;
    typedef WORD MessageSize;
    ResultCode result_code; // 요청 처리 결과
    HistoryListSize history_list_size; // 내역 개수
    ItemCompositionOrDecompositionHistory history_list[kMaxHistoryListSize]; // 내역

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
// 아이템 합성 또는 분해 내역 수정 요청
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
// 액션 스킬, 아이템을 통한 미확인 소켓 아이템의 소켓수 확인 요청
PACKET_CG_OPEN(CG_ITEM, CG_ITEM_IDENTIFY_SOCKET_CMD)
    typedef WORD MessageSize;
    POSTYPE unidentified_item_pos_; // 미확인 아이템 위치
    bool use_identify_item_; // 아이템을 사용한 확인인가?
    POSTYPE identify_item_pos_; // 아이템을 통한 확인시 확인 아이템 위치 (사용안한다면 INVALID_POSTYPE_VALUE : Define.h)

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

// 쟈드 합성 응답
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_ZARD_COMPOSE_ACK, MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK)
    // 처리 결과 코드
    // RC_ITEM_SUCCESS_TO_COMPOSE_ZARD : 쟈드 합성 성공
    // RC_ITEM_FAILED_TO_COMPOSE_ZARD : 쟈드 합성 실패
    // RC_ITEM_INVALID_VALUE : 클라에서 요청한 값이 옳바르지 않음
    // RC_ITEM_HAVENOTSPACE : 인벤토리에 보상을 받을 여유 공간이 없음
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_ZARD_COMPOSE_ACK
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_002253_20100811_CUBE_SYSTEM
// 큐브 아이템 합성 또는 분해 응답
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK, MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK)
    // 처리 결과 코드
    // RC_ITEM_SUCCESS_TO_COMPOSE_CUBE : 큐브 합성 성공
    // RC_ITEM_FAILED_TO_COMPOSE_CUBE : 큐브 합성 실패 (예외적인 경우는 모두 여기에 속함)
    // RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE : 큐브 분해 성공
    // RC_ITEM_FAILED_TO_DECOMPOSE_CUBE : 큐브 분해 실패 (예외적인 경우는 모두 여기에 속함)
    // RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE : 카오 상태라서 사용 불가
    // RC_ITEM_INVALID_VALUE : 클라에서 요청한 값이 옳바르지 않음
    // RC_ITEM_HAVENOTSPACE : 인벤토리에 보상을 받을 여유 공간이 없음
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK
// 성공한 큐브 아이템 합성 내역들 응답
PACKET_CG_INHERIT_OPEN(CG_ITEM, CG_ITEM_GET_CUBE_COMPOSITION_HISTORIES_ACK, MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
    // 처리 결과 코드
    // 성공 : RC_ITEM_SUCCESS_TO_GET_COMPOSITION_LIST
    // 실패 : RC_ITEM_FAILED_TO_GET_COMPOSITION_LIST 또는 아이템 관련 기본 오류 코드
PACKET_CG_INHERIT_CLOSE; // CG_ITEM_GET_CUBE_COMPOSITION_HISTORIES_ACK
#endif // _NA_002253_20100811_CUBE_SYSTEM

// _NA_003740_20111122_RANDOM_ITEM
// 아이템 능력치 랜덤화
PACKET_CG_FWD_OPEN(CG_ITEM, CG_ITEM_RANDOMIZE_SYN)
    typedef WORD MessageSize;

    POSTYPE request_item_pos; // 랜덤화 요청 아이템 인벤토리 위치
    POSTYPE target_item_pos; // 랜덤화 대상 아이템 인벤토리 위치

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

    ResultCode result_code; // RC::eITEM_RESULT 참조
    POSTYPE request_item_pos; // 랜덤화 요청 아이템 인벤토리 위치
    POSTYPE target_item_pos; // 랜덤화 대상 아이템 인벤토리 위치

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

//_NA_004035_20120227_COSTUME_ITEM //코스튬 아이템 패킷..
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_USE_COSTUME_ACK )
    typedef WORD MessageSize;

    bool    is_equipped_costume_;
    POSTYPE costume_item_pos_;
    void Init()
    {
        is_equipped_costume_ = false; //코스튬 아이템 활성화 여부
        costume_item_pos_ = INVALID_SLOT_POS; //코스튬 아이템 인벤토리 위치
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
    is_equipped_costume_ = false; //코스튬 아이템 활성화 여부
    deco_costume_item_pos_ = INVALID_SLOT_POS; //코스튬 아이템 인벤토리 위치
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
    POSTYPE	equip_item_pos;     // 각성할 아이템
    POSTYPE	material_item_pos;  // 재료 아이템
    POSTYPE	additive_item_pos;  // 첨가제 아이템
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_AWAKENING_ACK )
    int result_code;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_EVOLUTION_SYN )
    POSTYPE	equip_item_pos;     // 진화할 아이템
    POSTYPE	additive_item_pos;  // 첨가제 아이템
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템 조합
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 거래
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_SYN )
	DWORD		m_dwPlayerKey;						// 요청할 플레이어 키
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_ACK )	// 거래 요청이 승락되었음
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_CMD )	// 상대방에 거래창 오픈 요청
	DWORD		m_dwPlayerKey;						// 거래 요청한 플레이어 키
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_OPEN_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_SYN )
	POSTYPE		m_OrgPos;							// 인벤토리의 아이템 위치
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_ACK )	
	POSTYPE		m_OrgPos;							// 인벤토리의 아이템 위치
	POSTYPE		m_TradePos;							// 장착창의 아이템 위치
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_PUT_CMD )
	POSTYPE			m_TradePos;						// 상대편이 올린 아이템의 거래창의 위치
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
	MONEY		m_money;						// 상대편이 올려놓은 돈의 액수
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_SYN )
	POSTYPE		m_TradePos;						// 거래창의 위치에 있는 아이템을 인벤토리로 가져옴
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_ACK )
	POSTYPE		m_TradePos;						// 거래창의 위치에 있는 아이템을 인벤토리로 가져옴
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_CMD )
	POSTYPE		m_TradePos;						// 상대편이 거래창의 특정 아이템을 삭제함
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_SYN )
	MONEY			m_money;					// 거래창에서 뺄 돈의 금액
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_ACK )
	MONEY			m_money;					// 거래창에서 뺄 돈의 금액
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_TRADE, CG_TRADE_GET_MONEY_CMD )
	MONEY			m_money;					// 거래창에서 뺄 돈의 금액
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 거래,개인상점
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//@@@ OPT : 최적화 여지 char m_pszTitle[MAX_VENDOR_TITLE_LENGTH];
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_SYN )
    TCHAR m_pszTitle[MAX_VENDOR_TITLE_LENGTH];   // MAX_VENDOR_TITLE_LENGTH값 자체가 최대 타이틀 길이인데...
    ESTABLISHER_VENDOR_TOTAL_INFO m_ItemInfo;
    int GetSize() const
    {
        return (sizeof(*this)-(ESTABLISHER_VENDOR_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_Count)*sizeof(VENDOR_ITEMSLOT));
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_START_BRD )
	DWORD m_dwPlayerKey;		//< 개설자의 플레이어 키
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
	BYTE m_byEndKind;				//< 0이면 종료, 1이면 PAUSE
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_ACK )
	BYTE m_byEndKind;				//< 0이면 종료, 1이면 PAUSE
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_BRD )
	DWORD m_dwPlayerKey;		//< 상점을 종료한 플레이어의 키
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_END_NAK)
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_SYN )
	POSTYPE m_VendorPos;				//< 수정 할려는 아이템의 슬롯 위치
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_ACK )
	POSTYPE m_VendorPos;				//< 수정 할려는 아이템의 슬롯 위치
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_BRD )
	POSTYPE m_VendorPos;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_REQ_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_MODIFY_CANCEL_SYN )
	POSTYPE m_VendorPos;				//< 수정 할려는 아이템의 슬롯 위치
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
	BYTE m_BuyNum;			//< 살려는 개수 : 일반적으로는 파는 개수의 최대치
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_VENDOR, CG_VENDOR_BUY_ACK)
	MONEY m_Money;				//< 구입하고 난 뒤의 나의 돈
	INVENTORY_TOTAL_INFO m_InventoryTotalInfo;

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_InventoryTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_InventoryTotalInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_CMD )
	POSTYPE m_VendorPos;		//< 팔려서 없어진 아이템위
	MONEY m_Money;				//< 개설자가 판매하고 난뒤의 돈
	BYTE	m_SellNum;			//< 개설자가 판 개수
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_BRD )
	POSTYPE m_VendorPos;		//< 팔려서 없어진 아이템
	BYTE	m_SellNum;			//< 판 개수
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_BUY_NAK )
	BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_VIEW_START_SYN )
	DWORD m_dwPlayerKey;		//< 상점을 개설한 플레이어의 키
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
	DWORD m_dwPlayerKey;		//< 개설자의 플레이어 키
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 개인상점 리스트
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상점 리스트 시작
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_LIST_START_NAK )
BYTE m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//상점 리스트 Page 요청, 
PACKET_CG_FWD_OPEN( CG_VENDOR, CG_VENDOR_REQUIRE_PAGE_SYN )
BYTE					m_byKind;						//eVENDOR_LIST_PAGE_KIND
BYTE					m_byPage;						//몇번째 페이지 인가 - 1부터 시작...
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

//상점 리스트 검색 요청
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

//상점 리스트 종료
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 필드 아이템 줍기,버리기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_MONEY_SYN )
	PACKET_RND_SHRBLK64_START(GETRNDNO_RNG16_T1(142));
	PACKET_RND_SHRBLK64_CONT(DWORD, m_dwFieldItemObjectKey);
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ITEM, CG_ITEM_PICK_MONEY_ACK )
	MONEY						m_Money;
	WORD						m_BonusRatio;					// 추가 퍼센트	
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 창고
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 창고, 인벤토리 잠금 (패스워드)관련
// 관련 매크로 : __TASK_WAREHOUSE_N_INVENTORY_LOCK_BY_PWD__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// <TYPE DEFINITIONs>
typedef util::cSTRING_BUFFER<MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1>	sPWD_STREAM;
typedef util::cSTRING_BUFFER<MAX_SSN_STREAM_LENGTH>					sSSN_STREAM;

/*																*/	
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_CHECK_SYN )
	enum eREQ_CODE	// <REQUEST CODE>
	{
		SYNC_FIRST			,	// 첫번째 요청 (무조건 이것부터 보낸다)
		SYNC_CLEAR			,	// 비번 초기화 요청시
		SYNC_CANCEL			,	// 비번 설정 과정 자체를 취소할 때,
		SYNC_WITH_PWD		,	// ~_ACK::eRET_CODE::NEED_PWD | INVALID_PWD 수신시
		SYNC_WITH_SSN		,	// ~_ACK::eRET_CODE::NEED_SSN | INVALID_SSN 수신시
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
		return 0;	// 0으로 보내면, 서버는 해당 클라 동작 멈추게 만든다. ㅋㅋㅋ
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
		RET_FAIL			,	// 부적절한 상황, 서버의 문제 등등시 실패 코드
		RET_SUCCESS			,	// 비번 변경 성공시
		RET_SUCCESS_CLEAR	,	// 비번 초기화 성공시
		RET_CANCELED		,	// 비번 설정 과정 자체 취소된 상태 (~_SYN::SYNC_CANCEL 에 의한 응답)
		RET_NEED_PWD		,	// 비번 요구
		RET_INVALID_PWD		,	// 조건에 맞지 않는 비번이다
		RET_NEED_SSN		,	// 비번 변경 인증을 위한 SSN 요구
		RET_INVALID_SSN		,	// 잘못된 SSN이다
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
// 잠금상태->잠금해제, 잠금해제->잠금상태 토글 요청 패킷 구조체
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_SYN )
PACKET_CG_FWD_CLOSE;

/*																*/	
struct MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN : MSG_CG_WAREHOUSE_PWD_LOCKING_SYN
{
	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN() { m_byCategory = CG_ITEM; m_byProtocol = CG_ITEM_INVENTORY_PWD_LOCKING_SYN; }
};

/*																*/	
// 기존 비번있고, 임시 잠금 해제상태일 경우, 잠금상태로 전환되는 경우만 있다.
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_ACK )
	enum eRET_CODE
	{
		RET_SUCCESS_LOCKED			,	// 잠금해제->잠금상태 전환
		RET_NEED_PWD_FOR_LOCKING	,	// 기존 비번이 존재하지 않아서 잠금을 하기 위한 비번 요구
		RET_NEED_PWD_FOR_UNLOCKING	,	// 잠금상태에서 잠금해제상태로 전환하기 위해 비번 요구
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
// CG_WAREHOUSE_PWD_LOCKING_ACK 이외의 상황, 그 상황은 해당 RET_CODE에 존재
PACKET_CG_FWD_OPEN( CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_NAK )
	enum eERR_CODE
	{
		IRREGULAR_LOCKING_POLICYMENT	,	// 창고,인벤 잠금 플로에서 있어서는 안되는 상황이 발생
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// TRIGGER 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
	PACKET_RND_SHRBLK64_CONT(WORD, m_RewardCode);	// (RewardInfoList.txt) 보상 스크립트 번호
	PACKET_RND_SHRBLK64_END();
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_SYN )
	WORD						m_Num;		//< (RewardInfoList.txt) 보상 스크립트 번호
	//BYTE						m_Num;		//< 보상스크립트의 번호
	//BYTE						m_Type;		//< 0 or 1
	enum
	{
		ITEM1_SELECTED = (1<<0),
		ITEM2_SELECTED = (1<<1),
		ITEM3_SELECTED = (1<<2),
		ITEM4_SELECTED = (1<<3),
        ITEM5_SELECTED = (1<<4), //_NA_0_20101130_REWARD_ITEM_COUNT_INCREASE
	};
	BYTE						m_SelectedItem;		//< 선택한 아이템들
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_REWARD_ACK )
	MONEY						m_Money;
	EXPTYPE						m_i64Exp;					// 최종 경험치
	BYTE						m_byFatigueType;			//피로도 타입.
#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
	bool						m_bGuildBounsExp;			//길드보너스 경험치
#endif
    BOOLEAN                     m_bPortalMission;   // Portal통한 입장인지 여부

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
	int						m_nEventIndex;	// 플레이할 이벤트 인덱스
PACKET_CG_FWD_CLOSE;

//__NA000357_070322_TRIGGER_SHOW_THE_FLOOR__
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_SHOW_THE_FLOOR_CMD )
	DWORD					m_dwPlayerKey;		// 해당 액션을 수행하는 플레이어, 파티들도 수신받으며, 이 경우 해당 플레이어의 층정보를 처리한다.
	BYTE					m_byFloor;			// 몇 층인가?
	PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_PLAY_MUSIC_CMD )
	BYTE					m_byTarget;				// 타켓 정보(개인,파티)
	INT						m_nMusCode;			// 뮤직 정보
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DISPLAY_MSGBOX_CMD )
    INT						m_nTextID;			// 텍스트 정보
PACKET_CG_FWD_CLOSE;

//_NA_0_20100222_UNIT_TRIGGERS_ADD
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD )
    DWORD					m_dwObjectKey;		// 텍스트 보여줄 위치 (유닛 말풍선)
    DWORD					m_dwMessageCode;	// 출력할 메시지코드
PACKET_CG_FWD_CLOSE;

//_NA_0_20100525_ADD_TIMER_ACTION_TRIGGER
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_CONTROL_TIMER_CMD )
	DWORD					    m_dwTime;				// 시간 (ms), 생성시 초기시간, 파괴시 유지 시간(delay)
	BYTE						m_byControlType;		// 변동 eTIMER_CONTROL_TYPE
PACKET_CG_FWD_CLOSE;                                     // (0:역방향 생성(감소), 1:정방향 생성(증가), 2:파괴 )

//_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
PACKET_CG_FWD_OPEN( CG_TRIGGER, CG_TRIGGER_CHANGE_RADAR_MAP_CMD )
    WzID sub_map_id;
PACKET_CG_FWD_CLOSE;

//////////////////////////////////////////////////////////////////////////
//__NA000405_070406_LAST_OBJSTATE_APPLICATION__
// 이걸 CG_TRIGGER_DONE_ACTION_INFO_CMD 대신에 보내려고 만들긴 했다.
// 이유는 두개의 트리거가 서로 상대방에서 반대 속성을 가지는 상태 변경을 수행할 경우 둘 중 하나는
// 정상적으로 동작할 수 없게 되기 때문이다. 하지만, 이렇게 되면 너무 많은 패킷을 보내는 등 문제가 커진다.
// 일단은 참고사항으로 남겨두고 넘어가자.
//	struct sOBJECT_STATE_INFO_BASE
//	{
//		// TriggerType 참조 (통일할 필요는 있다)
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터 상태, 속성, 수치 변경 관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// AG_STATUS_LEVEL_UP_BRD 프로토콜을 추가
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_LEVEL_UP_BRD )
	DWORD						m_dwObjectKey;					// 누구
	LEVELTYPE					m_Level;						// 업된 레벨
	WORD						m_wRemainStat;					// 남은 스탯포인트
	WORD						m_wRemainSkill;					// 남은 스킬포인트
	DWORD						m_dwCurHP;						// 현재 HP(레벨업시 HP, MP를 채워준다.)
	DWORD						m_dwCurMP;						// 현재 MP
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_LEVEL_UP_CMD )
	LEVELTYPE					m_Level;						// 업된 레벨
	WORD						m_wRemainStat;					// 남은 스탯포인트
	WORD						m_wRemainSkill;					// 남은 스킬포인트
	DWORD						m_dwCurHP;						// 현재 HP(레벨업시 HP, MP를 채워준다.)
	DWORD						m_dwCurMP;						// 현재 MP
    DWORD						m_dwCurSD;						// 현재 SD
PACKET_CG_FWD_CLOSE;

// HP 변화
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_HP_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwTargetHP;					// 최종값
	DWORD						m_dwReserveHP;					// 적립된 HP
PACKET_CG_FWD_CLOSE;

// MP 변화
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_MP_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwTargetMP;					// 최종값
PACKET_CG_FWD_CLOSE;

//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//FP변화
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_FP_CMD )
    DWORD                           object_key;
    PLAYERFP						m_dwTargetFP;					// 최종값
PACKET_CG_FWD_CLOSE;
//FP이펙트 갱신을 위한 패킷
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_FP_EFFECT_BRD )
    DWORD                           object_key;
    PLAYERFP                        current_fp;
PACKET_CG_FWD_CLOSE;

// HP, MP 변화
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RECOVER_ATTR_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwTargetHP;					// 최종값
	DWORD						m_dwTargetMP;					// 최종값
PACKET_CG_FWD_CLOSE;

// SD 변화(자신)
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_SD_CMD )
    DWORD						m_dwSD;					// 최종값
PACKET_CG_FWD_CLOSE;

// 부활 요청
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_SYN )
PACKET_CG_FWD_CLOSE;

// 부활 실패
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_NAK )
	DWORD						m_dwObjectKey;					// 누구
	BYTE						m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// 부활 브로드
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwHP;							// 최종값
	DWORD						m_dwMP;							// 최종값
	WzVector					m_wvCurPos;						// 부활 위치
	DWORD						m_dwReserveHP;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_TITLE_CHANGED_CMD )
	DWORD						m_PlayerTitle;					//< ePLAYER_TITLE 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_TITLE_CHANGED_BRD )
	DWORD						m_dwPlayerKey;
	DWORD						m_PlayerTitle;					//< ePLAYER_TITLE 참조
PACKET_CG_FWD_CLOSE;

// 사망
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_DEAD_BRD )
	DWORD						m_dwObjectKey;					// 누구
	WzVector					m_wvCurPos;						// 죽은 위치
	EXPTYPE						m_i64Exp;						// 최종 경험치
PACKET_CG_FWD_CLOSE;

// 경험치 변경
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EXP_CMD )
	DWORD						m_dwTargetObjKey;				// 타겟 오브젝트 키
	EXPTYPE						m_i64Exp;						// 최종 경험치
	WORD						m_BonusRatio;					// 추가 퍼센트(0~500)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_GROUPEXP_CMD )
    DWORD						m_dwTargetObjKey;				// 타겟 오브젝트 키
    EXPTYPE						m_i64Exp;						// 최종 경험치
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_GROUPMONEY_BRD )
    CHAR ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusMoney;
PACKET_CG_FWD_CLOSE;

// 수치 변경
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ATTR_CHANGE_BRD )
	DWORD						m_dwObjKey;
	BYTE						m_AttrType;						// eATTR_TYPE
	DWORD						m_dwValue;						// value
PACKET_CG_FWD_CLOSE;


// 스탯 업
#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION    

// eATTR_STR = 1, 스탯(힘)
// eATTR_DEX = 2, 스탯(민첩)
// eATTR_VIT = 3, 스탯(체력)
// eATTR_INT = 4, 스탯(지력)
// eATTR_SPR = 5, 스탯(정신력)
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


// 스탯 업
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

// 캐릭터 상태 변경
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_SYN )
	BYTE						m_byConditionType;				// 캐릭터의 상태 타입(eCHAR_CONDITION)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_BRD )
	DWORD						m_dwObjectKey;					// 누구
	BYTE						m_byConditionType;				// 캐릭터의 상태 타입(eCHAR_CONDITION)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CONDITION_CHANGE_NAK )
	BYTE						m_byErrorCode;					// eCONDITION_RESULT
PACKET_CG_FWD_CLOSE;

// 상태 추가
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ADD_BRD )
	DWORD						m_dwTargetKey;					// 상태 추가 대상
	WORD						m_wStatusCode;					// 상태 코드
PACKET_CG_FWD_CLOSE;

// 상태 해제
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REMOVE_BRD )
	DWORD						m_dwTargetKey;					// 상태 해제 대상
	DWORD						m_dwStatusCode;					// 상태 코드
	int GetSize() { return sizeof(*this); };
PACKET_CG_FWD_CLOSE;

// 캐릭터 이모션
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_SYN )
	BYTE						m_byEmotionType;				// 캐릭터의 이모션 타입(환호,한숨,감사,웃음,울음 등등)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_BRD )
	DWORD						m_dwObjectKey;					// 누구
	BYTE						m_byEmotionType;				// 캐릭터의 이모션 타입
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_EMOTION_NAK )
	BYTE						m_byErrorCode;					// eSTATUS_RESULT
PACKET_CG_FWD_CLOSE;

// 몬스터 상태 변경
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_NPC_STATE_CHANGE_CMD )
	DWORD						m_dwObjectKey;					// 누구
	BYTE						m_byState;						// NPC의 상태 타입(eNPC_STATE)
PACKET_CG_FWD_CLOSE;

// 렌더링 옵션 설정
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_OPTION_SYN )
	enum 
    { 
        eRENDER_HELMET_OPTION = 0,
        eRENDER_CAPE_OPTION,
        eRENDER_WING_OPTION,
        eRENDER_COSTUME_OPTION, // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    };

	BYTE						m_byRenderOptionType;			// 렌더링 옵션 종류
	BYTE						m_byOptionLevel;				// 옵션단계
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RENDER_OPTION_BRD )
	DWORD						m_dwObjectKey;					// 누구
	BYTE						m_byRenderOptionType;			// 렌더링 옵션 종류
	BYTE						m_byOptionLevel;				// 옵션단계
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_CHANGE_MONSTER_ABILITY )
	float						m_fHPRatio;						// 몬스터 HP 배수
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_FATIGUE_CHANGE_CMD )
	WORD						m_wPlayingTime;					// 현재 플레이 시간(분단위)
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK )
	BYTE						m_byErrorCode;					// eSTATUS_RESULT 참조	
PACKET_CG_FWD_CLOSE;

// 플레이어 상태정보
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ALL_STATEINFO_CMD )
    STATE_TOTAL_INFO  m_StateTotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_StateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_StateTotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

// designed by __NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
// 플레이어 ETC상태정보
PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_ETC_STATEINFO_CMD )
    ETC_STATE_TOTAL_INFO m_stEtcStateTotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_stEtcStateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_stEtcStateTotalInfo.GetSize());
    };
PACKET_CG_FWD_CLOSE;

// 드래곤 변신
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
	BYTE						m_byErrorCode;					// eSTATUS_RESULT 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwHP;							// 최종값
	DWORD						m_dwMP;							// 최종값
	WzVector					m_wvCurPos;						// 부활 위치
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_NAK )
	DWORD						m_dwErrorCode;					//< eSTATUS_RESULT
PACKET_CG_FWD_CLOSE;

//귓말, 거래 차단
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
		EFFECT_ADD	= PROTOCOL_GENERATOR_STATUS_201,			// 효과 추가
		EFFECT_DEL	= PROTOCOL_GENERATOR_STATUS_202,			// 효과 제거
	};

	BYTE						m_SynType;
	DWORD						m_SpecialEffect;				// BitStream : eSPECIAL_RENDER_EFFECT

	//	<USAGE>
	//		MSG_CG_STATUS_RENDER_OPTION_SYN msgSYN;
	//		--- 기존 코드 사용시
	//		// 기존 형식
	//		//	msgSYN.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION;
	//		//	msgSYN.m_byOptionLevel = level;
	//		// 변경 형식
	//		msgSYN.SetPartialInfo( MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION, level );
	//		--- 특수 추가 정보 입력시
	//		msgSYN.SetSpecialInfo( eSPECIAL_RENDER_EFFECT_SOLO_BADGE );
	//		SendPacket( &msgSYN, sizeof(msgSYN) );
	inline VOID					SetInfo( eSYN_TYPE cmd, DWORD specialEffect )
	{
		m_SynType = static_cast<uint8_t>(cmd);
		m_SpecialEffect = specialEffect;
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_STATUS, CG_STATUS_CHAGNE_RENDER_EFFECT_BRD, MSG_CG_STATUS_CHAGNE_RENDER_EFFECT_SYN )
	DWORD						m_ObjectKey;					// 누구
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
	eSPECIAL_RENDER_EFFECT	m_BitRender;	// 아이템이 아닌 특수 이펙트
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
	//	"sMSG_RENDER_EFFECT_ITEM_LIST" 가 m_Count만큼 추가된다. 각 노드들 역시 variable size

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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 파티관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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

// 파티원 한명 정보가m_byHPRatio 바꼈을 때
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_LEVEL_UP_BRD )
    DWORD				m_dwObjKey;				// 레벨업한 파티원
    LEVELTYPE			m_wLevel;				// 현재 레벨
	BYTE				m_byHPRatio;			// 현재 HP 비율
	BYTE				m_byMPRatio;			// 현재 MP 비율
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
    DWORD				m_dwObjKey;				// 상태가 변한 파티원
	bool				m_bAdd;					// 상태 추가/해제
    WORD				m_wStateCode;			// 상태 코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_DEAD_BRD )
	DWORD				m_dwObjKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_RESURRECTION_BRD )
	DWORD				m_dwObjKey;
	BYTE				m_byHPRatio;			// 현재 HP 비율
	BYTE				m_byMPRatio;			// 현재 MP 비율
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD )
	DWORD				m_dwObjKey;
	BYTE				m_byChaoState;			// 카오상태(eCHAO_STATE)
PACKET_CG_FWD_CLOSE;

// 파티가입 요청
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_SYN )
	char m_TargetCharName[MAX_CHARNAME_LENGTH];// // 파티가입을 요청하는 상대
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_BRD )
	WORD				m_PartyKey;
	_PARTY_MEMBER_INFO	m_NewMemberInfo;
PACKET_CG_FWD_CLOSE;    

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_NAK )
	BYTE				m_byErrorCode;			// 파티가입 요청 실패 원인(ePARTY_RESULT)
	ePLAYER_BEHAVE_STATE	m_ePlayerState;
PACKET_CG_FWD_CLOSE;

// 파티가입 응답
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_SYN )
	char				m_szMasterName[MAX_CHARNAME_LENGTH];// 가입을 요청한 파티장 ID
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_JOIN_RESPONSE_NAK )
PACKET_CG_FWD_CLOSE;

// 파티탈퇴 요청
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_FORCED_EXPULSION_SYN )
	DWORD				m_dwObjKey;				// 강제추방할려는 대상의 오브젝트 키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_BRD )
    BYTE                m_byReason;				// eREASON_LEAVE_PARTY
	DWORD				m_dwObjKey;				// 탈퇴한 파티원의 오브젝트 키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_LEAVE_NAK )
	BYTE				m_byErrorCode;			// 파티탈퇴 실패 원인(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// 파티장 변경 요청
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_SYN )
	DWORD				m_dwObjKey;				// 변경할 멤버 오브젝트키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_BRD )
	DWORD				m_dwObjKey;				// 변경된 파티장 오브젝트키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_CHANGE_MASTER_NAK )
	BYTE				m_byErrorCode;			// 파티장 변경 실패 원인(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// 파티해체 요청
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_BRD )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_DESTROY_NAK )
	BYTE				m_byErrorCode;			// 파티해체 실패 원인(ePARTY_RESULT)
PACKET_CG_FWD_CLOSE;

// 파티 타겟팅
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_SYN )
	DWORD				m_dwObjectKey;			// 타겟으로 삼을 몬스터 오브젝트키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_BRD )
	DWORD				m_dwObjectKey;			// 타겟으로 지정된 오브젝트키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_SELECT_TARGET_NAK )
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// 파티장 근처로 워프(최종 워프는 CG_MAP_TELEPORT_BRD로 보냄)
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_WARP_REQ_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_WARP_REQ_NAK )
	BYTE				m_byErrorCode;			// ePARTY_RESULT
PACKET_CG_FWD_CLOSE;

// 파티원 소환(최종 소환은 CG_MAP_TELEPORT_BRD로 보냄)
PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_MEMBER_SUMMON_REQ_SYN )
	DWORD				m_dwObjKey;				// 소환할 파티원 오브젝트키
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

// 파티 미니맵 포인팅 
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
		ACCEPT_JOIN			= PROTOCOL_GENERATOR_091,	// 파티참가 허용
		REJECT_JOIN			= PROTOCOL_GENERATOR_054,	// 파티참가 불허
		THINKING_HUMMMM		= PROTOCOL_GENERATOR_034,	// 파티장 고민중
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
		ACCEPT_JOIN			= PROTOCOL_GENERATOR_029,	// 파티참가 OK
		REJECT_JOIN			= PROTOCOL_GENERATOR_147,	// 파티참가 NO
	};
	BYTE						m_byAnswer;
	WORD						m_wPartyKey;
	inline BYTE&				ANSWER()		{ return m_byAnswer; }
	inline WORD&				PARTY_KEY()		{ return m_wPartyKey; }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD )
	sSTRING_BUFFER_CHAR_NAME	m_CharName;
	inline CHAR*				CHAR_NAME()		{ return m_CharName.GetBUFFER(); }
	inline VOID					CopyFrom( const TCHAR* ptszCharName )	// 안정화된 문자열
	{
		m_CharName.CopyFrom( ptszCharName );	// NULL 포함
	}
	inline WORD					GetSize()
	{
		return sizeof(*this);
	}
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_PARTY, CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD )
	sSTRING_BUFFER_CHAR_NAME	m_CharName;
	inline CHAR*				CHAR_NAME()		{ return m_CharName.GetBUFFER(); }
	inline VOID					CopyFrom( TCHAR* ptszCharName )	// 안정화된 문자열
	{
		m_CharName.CopyFrom( ptszCharName );	// NULL 포함
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
        // 1 << 7이 Max
        MAX_OBJECT_ARRAY_SIZE   = MAX_PARTYMEMBER_NUM,
    };
    struct Node {
        WORD        objectKey;  // Player Object Key
        FIELDCODE   fieldCode;  // FIELDCODE
        short       posX, posY, posZ; // X,Y,Z좌표: 소수점 이하 제거 수치
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
    BYTE all_page_num_;//리스트 전체 페이지 수
    BYTE number_of_list_;//현재 전달되는 리스트 수
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_LIST];//리스트 정보
    WORD GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_LIST_SYN)
    BYTE request_page_;//요청 페이지
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_LIST_ACK)
    BYTE all_page_num_;//리스트 전체 페이지 수
    BYTE number_of_list_;//현재 전달되는 리스트 수
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_LIST];//리스트 정보
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CREATE_ROOM_SYN)
    BasePartyRoomInfo party_room_info_;//파티룸 기본 정보 구조체
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CREATE_ROOM_ACK)
    RC::ePARTY_RESULT result_code_;//결과값
    PartyRoomInfo party_room_info_;//생성된 방 정보 
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_JOIN_ROOM_SYN)
    WORD join_party_key_;//Join하려는 파티 키값
    bool is_private_room_;//비밀방 여부
    TCHAR password_[MAX_ROOMPASSWORD_LENGTH+1];//파티룸 비밀 번호
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
    RC::ePARTY_RESULT result_code_;//결과 값
    PartyRoomInfo party_room_info_;//들어가려는 방의 정보
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
    DWORD member_key_;//파티룸을 떠나는 Object Key
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    BYTE reason_to_leave_;//eREASON_LEAVE_PARTY
#endif
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_SYN)
    BasePartyRoomInfo change_party_room_info_;//변경 요청 옵션 구조체
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_ACK)
    RC::ePARTY_RESULT result_code_;//결과 값
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_BRD)
    BasePartyRoomInfo change_party_room_info_;//변경 되는 옵션 구조체
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN)
    BYTE max_count_;//변경하려는 Max count
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK)
    RC::ePARTY_RESULT result_code_;//결과 코드
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD)
    BYTE max_count_;//변경되는 Max count
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_ROOM_SYN)
    BYTE number_of_mission_;//예약한 미션의 갯수
    MAPCODE map_code_[MAX_MISSION_COUNT];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(map_code_);
        return static_cast<uint16_t>(kMinSize + sizeof(map_code_[0]) * number_of_mission_);
    };
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_STANDBY_ROOM_ACK)
    bool result_value_;//결과 값
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ASK_JOIN_ROOM_BRD)
    MAPCODE map_code_;//참가 가능한 미션 코드
    WORD party_key_;//참가 가능한 파티 키
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_SYN)
    bool ready_state_;//준비 상태
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_ACK)
    RC::ePARTY_RESULT result_code_;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_READY_STATE_BRD)
    DWORD member_key_;//준비 상태 변경을 요청한 케릭터 guid
    bool ready_state_;//준비 상태
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_PLAYER_MOVE_FIELD_BRD)
    DWORD member_key_;//맵 이동을 한 케릭터 guid
    MAPCODE map_code_;//이동한뒤 해당 케릭터의 위치 
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_CHECK_ROOM_SYN)
    enum CheckType{
        CheckCreateType,
        CheckStanByType,
    };
    CheckType check_type_;//체크 타입
    BYTE select_mission_num_;//예약을 원하는 미션의 갯수
    MAPCODE map_code_[MAX_MISSION_COUNT];//예약을 원하는 미션 코드 배열
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
    CheckType check_type_;//체크 타입
    WORD find_party_key_;//조건이 맞는 파티 키
    MAPCODE map_code_;//찾은 룸의 맵코드
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
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템    
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_ROOM_START_ACK)
    RC::ePARTY_RESULT result_code_;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_SYN)
    BYTE select_team_;//현재 0:아무런 팀 선택 하지 않음, 1:첫번째 팀 2:두번째 팀
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_ACK)
    RC::ePARTY_RESULT result_code_;//팀선택 결과 값
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_SELECT_TEAM_BRD)
    DWORD member_key_;//팀을 선택한 유저의 object key
    BYTE select_team_;//선택한 팀
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_REQUEST_MEMBER_INFO_SYN)
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_CHARID_SYN)
    TCHAR find_char_name_[MAX_CHARNAME_LENGTH+1];
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_CHARID_ACK)
    RC::ePARTY_RESULT result_code_;//결과 값
    PartyRoomInfo party_room_info_;//들어가려는 방의 정보
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
RC::ePARTY_RESULT result_code_;//결과 값
PartyRoomInfo party_room_info_;//들어가려는 방의 정보
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
    BYTE number_of_list_;//현재 전달되는 리스트 수
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_SEARCH_LIST];//리스트 정보
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(party_room_info_);
        return static_cast<uint16_t>(MinSize + sizeof(party_room_info_[0]) * number_of_list_);
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN)
    MAPCODE request_map_code_;//요청 미션 맵 코드
    PartyZoneType party_room_type_;//요청 미션 타입
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_PARTY, CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK)
    BYTE number_of_list_;//현재 전달되는 리스트 수
    PartyRoomInfo party_room_info_[MAX_PARTYROOM_SEARCH_LIST];//리스트 정보
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// GM 명령어 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 몬스터 생성 및 제거
struct MSG_CG_GM_MONSTER_CREATE_SYN : public MSG_BASE_FORWARD
{
    MONSTERCODE			m_dwMonsterCode;
    BYTE				m_byMonsterCnt;
    float				m_fPos[3];			// 생성하고 싶은 좌표
};

struct MSG_CG_GM_MONSTER_DESTROYALL_SYN : public MSG_BASE_FORWARD
{
};

struct MSG_CG_GM_MONSTER_DESTROYNEAR_SYN : public MSG_BASE_FORWARD
{
};

// 옵져버 모드로 전환
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

// 무적상태로 전환
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_SYN )
	BOOL				m_bUndeadOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_ACK )
	BOOL				m_bUndeadOn;		// On(1) or Off(0)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_UNDEAD_MODE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// 캐릭터의 정보 요청
struct MSG_CG_GM_CHARINFO_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwUserID;								// CharName으로 검색할 때는 0 값을 셋팅한다.
    char				m_szCharName[MAX_CHARNAME_LENGTH+1];
};

struct MSG_CG_GM_CHARINFO_ACK : public MSG_BASE_FORWARD
{
	char				m_pszAccountID[ID_MAX_LEN+1];			// 계정 아이디.
	eCHAR_TYPE			m_eCharType;							// 캐릭터 클래스
    WORD				m_wLevel;								// 레벨
    MONEY				m_Money;								// 가진 돈
	KEYTYPE				m_RoomKey;								// 방키
	CODETYPE			m_MapCode;								// 맵코드
	WzVector			m_vCurPos;								// 현재 위치
	char				m_szGuildName[MAX_GUILDNAME_LENGTH+1];	// 길드이름
};

struct MSG_CG_GM_CHARINFO_NAK : public MSG_BASE_FORWARD
{
	DWORD				m_dwErrorCode;		// eGM_RESULT
};

// 일반 아이템 생성
struct MSG_CG_GM_CREATE_ITEM_SYN : public MSG_BASE_FORWARD
{
    SLOTCODE			m_ItemCode;				// 아이템 코드
    DWORD				m_dwItemCount;			// 아이템 개수(1 ~ GM_MAX_CREATE_ITEM_NUM)
};


// 인첸트 아이템 생성
struct MSG_CG_GM_CREATE_ENCHANT_ITEM_SYN : public MSG_BASE_FORWARD
{
    SLOTCODE			m_ItemCode;						// 아이템 코드
    DWORD				m_dwItemCount;					// 아이템 개수
    BYTE				m_byEnchant;					// 인첸트등급
};


// 일정반경의 필드 아이템을 모두 제거
struct MSG_CG_GM_DESTROY_ITEM_AROUND_SYN : public MSG_BASE_FORWARD
{
};


// 필드에 돈 생성
struct MSG_CG_GM_CREATE_MONEY_SYN : public MSG_BASE_FORWARD
{
    MONEY				m_Money;			// 생성할 액수(1 ~ GM_MAX_CREATE_MONEY)
};

// 레벨 업
struct MSG_CG_GM_LEVEL_UP_SYN : public MSG_BASE_FORWARD
{
    WORD				m_wBonusLevel;		// 상승시킬 레벨수치(현재 레벨 + 상승시킬 레벨수치 = 최종 레벨)
};

// 스텟 업
struct MSG_CG_GM_STAT_UP_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwBonusStat;		// -GM_MAX_STAT_UP_VALUE ~ GM_MAX_STAT_UP_VALUE
};

struct MSG_CG_GM_STAT_UP_ACK : public MSG_BASE_FORWARD
{
    DWORD				m_dwRemainStat;		// 남은 스탯포인트
};

// 스킬포인트 업
struct MSG_CG_GM_SKILLPOINT_UP_SYN : public MSG_BASE_FORWARD
{
    DWORD				m_dwBonusSkill;		// -GM_MAX_SKILLPOINT_UP_VALUE ~ GM_MAX_SKILLPOINT_UP_VALUE
};

struct MSG_CG_GM_SKILLPOINT_UP_ACK : public MSG_BASE_FORWARD
{

    DWORD				m_dwRemainSkill;		// 남은 스킬포인트
};


//GM 부활 SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_SYN )
PACKET_CG_FWD_CLOSE;

//GM 부활 NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_NAK )
DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

//GM 부활 BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RESURRECTION_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwHP;							// 최종값
	DWORD						m_dwMP;							// 최종값
	WzVector					m_wvCurPos;						// 부활 위치
PACKET_CG_FWD_CLOSE;


//GM 회복 SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_SYN )
PACKET_CG_FWD_CLOSE;

//GM 회복 NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

//GM 회복 BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RECOVERY_BRD )
	DWORD						m_dwObjectKey;					// 누구
	DWORD						m_dwHP;							// 최종값
	DWORD						m_dwMP;							// 최종값
PACKET_CG_FWD_CLOSE;
								 

//GM 속도 SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_SYN )
	BYTE						m_byMoveLevel;						// 속도레벨 0~5까지 있음.
PACKET_CG_FWD_CLOSE;

//GM 속도 BRD
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_BRD )
	DWORD						m_dwObjectKey;						// GM의 objectKey
	BYTE						m_byMoveLevel;						// 속도레벨
	FLOAT						m_fSpeedRatio;						// 속도에 따른 증가비율						
PACKET_CG_FWD_CLOSE;

//GM 속도 NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_SET_SPEED_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//GM 입장 SYN
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ENTRANCE_SYN )			
PACKET_CG_FWD_CLOSE;

//GM 입장 NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ENTRANCE_NAK )
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//GM 접속종료 SYN
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


//특정 몬스터 제거(아이템, 경험치 없음)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_DESTROYONE_SYN )
	DWORD						m_dwMonsterKey;
PACKET_CG_FWD_CLOSE;
//NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_DESTROYONE_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;


//특정 몬스터를 정상적으로 죽임(아이템, 경험치 있음)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_KILLONE_SYN )
	DWORD						m_dwMonsterKey;
PACKET_CG_FWD_CLOSE;
//NAK
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_MONSTER_KILLONE_NAK )
	DWORD						m_dwErrorCode;
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN( CG_GM, CG_GM_RELOAD_DATA_CMD )
PACKET_CG_FWD_CLOSE;

// 방정보
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ROOMINFO_SYN )
	KEYTYPE				m_RoomKey;				// 키값이 0이면 현재 소속된 방의 정보를 보여준다.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_ROOMINFO_ACK )
	enum { _MAX_PLAYER_INFO_SIZE = 10 };
	char				m_szMasterName[MAX_CHARNAME_LENGTH+1];	// 방 개설자 이름
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

// 채널정보
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_ACK )
	DWORD				m_dwChannelUserNum;		// 현재 채널 접속자수
	WORD				m_wHuntingRoomCount;	// 헌팅룸 개수(로비포함)
	WORD				m_wMissionRoomCount;	// 미션룸 개수(로비포함)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANNELINFO_NAK )
	DWORD				m_dwErrorCode;			// eGM_RESULT
PACKET_CG_FWD_CLOSE;

// 워프 기능
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_SYN )
	char				m_szCharName[MAX_CHARNAME_LENGTH];	// 이동시킬 대상이름
	bool				m_bWatch;							// 감시켬
	KEYTYPE				m_RoomKey;							// 방번호(마을의 경우 0으로 셋팅)
	CODETYPE			m_MapCode;							// 맵코드(마을이 아닌 경우 0으로 셋팅)
	WzVector			m_vDestPos;							// 이동하고자하는 목적지 좌표
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_WARP_NAK )
	BYTE				m_byErrorCode;						// eGM_RESULT
PACKET_CG_FWD_CLOSE;

// 스트링 명령어(서버쪽에서 파싱해서 처리하는 명령어)
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_STRING_CMD_SYN )
	enum { MAX_STRING_CMD_LENGTH = 100 };
	char				m_szStringCmd[MAX_STRING_CMD_LENGTH+1];	// 스트링 명령어
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
	TCHAR				m_szRoomTitle[MAX_ROOMTITLE_LENGTH];		// 바꿀 룸제목
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GM, CG_GM_CHANGE_ROOMTITLE_NAK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

//가기 GM명령어
PACKET_CG_FWD_OPEN( CG_GM, CG_GM_GO_NAK )
	BYTE				m_byErrorCode;	
PACKET_CG_FWD_CLOSE;

//추적 GM명령어
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 기타 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
	BYTE				m_bIsSummon;		// 1이면 소환 0이면 해제
	BYTE				m_byFullness;
	BYTE				m_Intimacy;
	BYTE				m_bSetPetName;
	TCHAR				m_PetName[MAX_PET_NAME_LENGTH];
	WORD				m_wPetIndex;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_STATUS_CHANGE_BRD )
	DWORD				m_dwObjectKey;
	POSTYPE				m_ItemPos;			// 인벤토리의 펫아이템 위치
	BYTE				m_bIsFullness;		// 1이면 포만감 0이면 친밀도
	BYTE				m_byValue;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_ETC, CG_ETC_PET_RETURN_CMD )
	POSTYPE				m_ItemPos;			// 인벤토리의 펫아이템 위치
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// PVP 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_INFO_CMD )	
	struct {
		BYTE				m_Team	:3;			//< 나의 팀 정보
		BYTE				m_Count :5;			//< 다른 사람의 팀 정보 수
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

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_FIRST_START_CMD )		//< 처음 pvp
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_START_CMD )			//< 자기자신
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_PVP, CG_PVP_MATCH_MODE_START_BRD )			//< 다른 사람
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 경쟁헌팅 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 새로운 파티 등장
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_ENTER_NEW_PARTY_BRD )
PACKET_CG_FWD_CLOSE;

// 경쟁 시작
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_START_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// 승리
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_WIN_COMPETITION_BRD )
	WORD					m_wWinPoint;			// 승리 포인트
	EXPTYPE					m_i64Exp;				// 최종 경험치
	MONEY					m_CurMoney;				// 현재 하임
	WORD					m_PlusPVPPoint;			// 획득 PVP 포인트
	INVENTORY_TOTAL_INFO	m_ItemInfo;				// 획득 아이템
	int GetSize()
	{
		return (sizeof(MSG_CG_CHUNTING_WIN_COMPETITION_BRD) - (INVENTORY_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_InvenCount-m_ItemInfo.m_TmpInvenCount)*sizeof(ITEMSLOTEX));
	}
PACKET_CG_FWD_CLOSE;

// 패배
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_LOSE_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// 무승부
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_TIE_COMPETITION_BRD )
PACKET_CG_FWD_CLOSE;

// 킬포인트 갱신
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_UPDATE_KILLPOINT_BRD )
	WORD					m_wTeamKey;
	WORD					m_wKillPoint;			// 팀의 킬포인트
PACKET_CG_FWD_CLOSE;

// 포션개수 갱신
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_UPDATE_POTIONCOUNT_BRD )
	BYTE					m_byAvailablePotion;	// 이용가능한 포션개수
PACKET_CG_FWD_CLOSE;

// 팀정보
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_TEAM_INFO_CMD )
	WORD					m_wTeamKey;
PACKET_CG_FWD_CLOSE;

// 개인 경쟁인포 요청
PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_POINT_INFO_SYN )
PACKET_CG_FWD_CLOSE;

struct CompetitionPoint
{
	DWORD					m_dwObjKey;
	WORD					m_wKillPoint;			// 내가 획득한 킬포인트
	BYTE					m_byDieNum;				// 죽은 횟수
	BYTE					m_byCombo;				// 콤보(연승횟수)
};

PACKET_CG_FWD_OPEN( CG_CHUNTING, CG_CHUNTING_POINT_INFO_ACK )
	BYTE					m_byCount;
	CompetitionPoint		m_Point[CHUNTING_MAX_MEMBER_NUM*2];
	int						GetSize()
	{
		return ( sizeof(MSG_CG_CHUNTING_POINT_INFO_ACK) - (CHUNTING_MAX_MEMBER_NUM*2 - m_byCount)*sizeof(CompetitionPoint) );
	}
PACKET_CG_FWD_CLOSE;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_SUMMON
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


// 공격정지
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_SYN )
	DWORD				m_dwTargetKey;					// 타겟 키(공격지시일 경우에만 유효함)
	BYTE				m_byAttackPropensity	: 2;	// 강제공격 여부
	BYTE				m_byCommand				: 6;	// 명령타입(eSUMMON_COMMAND)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_ACK )
	BYTE				m_byCommand;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_COMMAND_NAK )
	BYTE				m_byCommand;
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// 스킬구사
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_SKILL_ACTION_SYN )
	DWORD				m_dwSummonedObjKey;			// 소환체 오브젝트키
	SLOTCODE			m_SkillCode;				// Skill 코드
	DWORD				m_dwClientSerial;			// Skill 이벤트에 대한 고유 번호
	WzVector			m_wvCurPos;					// 클라이언트가 주는 소환체의 현재 좌표
	WzVector			m_wvDestPos;				// 소환체의 목적지 좌표
	DWORD				m_dwMainTargetKey;			// 주공격대상의 오브젝트 키
	WzVector			m_wvMainTargetPos;			// 주공격대상의 위치

	BYTE				m_byAttackPropensity;			// 강제공격 여부

	int GetSize() { return sizeof(MSG_CG_SUMMON_SKILL_ACTION_SYN); };
PACKET_CG_FWD_CLOSE;

// 소환체 생성
PACKET_CG_FWD_OPEN( CG_SUMMON, CG_SUMMON_CURRENT_MP_CMD )
	DWORD				m_dwSummonedObjKey;			// 소환체 오브젝트키
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
    WzVector		    m_wvPos;				    // Create  명령에서 생성 위치
    DWORD				m_dwCrystalWarpObjKey;	    // 타겟 키 (명령을 받는 크리스탈오브젝트키, 생성시 == 0)
    BYTE				m_byCommand;			    // 명령타입(eCRYSTALWARP_COMMAND)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_CRYSTALWARP_COMMAND_BRD )
    BYTE				m_byCommand;
    DWORD				m_dwObjKey;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_SUMMON, CG_CRYSTALWARP_COMMAND_NAK )
    BYTE				m_byCommand;
    BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_GUILD 길드 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 길드 생성
// 조건 : 
// 1. 유저 레벨 100 이상
// 2. 길드 창립 자금 XX하임 소요
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_SYN )	
	char	m_tszGuildName[MAX_GUILDNAME_LENGTH];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_ACK )
	GUILDGUID m_GuildGuid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_BRD )
	PLAYERKEY	m_MasterPlayerKey;							//< 길드 창설한 플레이어 키
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CREATE_NAK )
	BYTE	m_byErrorCode;
PACKET_CG_FWD_CLOSE;


// 길드 해체
// 길드장만 가능
// 이권(아이템,돈,각종이득)잃음
// 길드원->일반인으로 등급변경
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_ACK )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_BRD )
	PLAYERKEY	m_MasterPlayerKey;							//< 길드 해산한 플레이어 키
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DESTROY_NAK )
	BYTE	m_byErrorCode;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_FAILED_CMD )		//< 길드 가입을 실패 했습니다.
	BYTE				m_byErrorCode;
PACKET_CG_FWD_CLOSE;



//길드 초대
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_SYN )					
	char m_TargetCharName[MAX_CHARNAME_LENGTH];		//< 초대할 플레이어 캐릭터 이름
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_CMD )	
	CHARGUID m_OperUserKey;										//초대 요청한 캐릭터 인덱스
	char m_OperCharName[MAX_CHARNAME_LENGTH];  //초대 요청한 플레이어 이름
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    char m_OperGuildName[MAX_GUILDNAME_LENGTH + 1];
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_INVITE_NAK )						
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

//길드 초대 응답
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

//길드 가입
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
PACKET_CG_FWD_CLOSE; //길드원들에게 통보

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_JOIN_NAK )
	BYTE			m_byErrorCode;
PACKET_CG_FWD_CLOSE;

// 길드 정보
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



PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_WITHDRAW_SYN )			//< 길드 탈퇴
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

// 길드장이 길드 멤버 강제 탈퇴
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_SYN )
	char				m_ptszKickCharName[MAX_CHARNAME_LENGTH+1];		// 탈퇴시킬 길드원
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_CMD )
PACKET_CG_FWD_CLOSE;

// 길드장에 의해 추방되었음을 알림(탈퇴 당시 해당 멤버가 온라인일 경우에만 전송)
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_BRD )
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];			// 강퇴당한 캐릭터이름
PACKET_CG_FWD_CLOSE;

// 강제 탈퇴 실패(길드장에게만 전송)
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_KICKPLAYER_NAK )
	char				m_ptszKickCharName[MAX_CHARNAME_LENGTH+1];
	DWORD				m_dwErrorCode;					// 에러코드(ResultCode)
PACKET_CG_FWD_CLOSE;

///////////////
// 길드 포인트 관련 패킷들

// 길드에 UP 헌납
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_SYN )
UPTYPE		m_DonateUP;			// 헌납할 UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_ACK )
	UPTYPE		m_RemainUP;			// 헌납하고 남은 UP
	UPTYPE		m_GuildUP;			// 헌납 결과로 늘어난 길드의 총 UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_BRD )
	UPTYPE		m_GuildUP;			// 헌납 결과로 늘어난 길드의 총 UP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_DONATE_UP_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_CLOSE;

// 길드장이 UP->GP로 변환
PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_SYN )
	GPTYPE		m_ConvertGP;		// 변환할 GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_ACK )
	UPTYPE		m_RemainUP;			// 변환하고 남은 UP
	GPTYPE		m_NowGP;			// 변환 결과로 늘어난 총 GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_BRD )
	UPTYPE		m_RemainUP;			// 변환하고 남은 UP
	GPTYPE		m_NowGP;			// 변환 결과로 늘어난 총 GP
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_CONVERT_UP2GP_NAK )
	BYTE		m_byErrorCode;
PACKET_CG_CLOSE;


PACKET_CG_FWD_OPEN( CG_GUILD, CG_GUILD_RANKUP_BRD )
	BYTE		m_byNowLevel;		// 랭크업한 레벨
	GPTYPE		m_RemainGP;			// 남은 GP
	UPTYPE		m_RemainUP;			// 남은 UP
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

// 길드 관계
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

//길드 군단 생성		
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

//길드 군단 해산
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


//비어있는 군단 정보
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

//군단장 변경
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

//군단이동
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

//길드 육성 캠프 창설 
//육성 캠프는 군단장만이 쓸수 있는 기능이다.
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

//길드 육성 캠프 해산
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


// 로그인/아웃/레벨업 
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
	BYTE				m_byChaoState;			// 카오 상태(eCHAO_STATE)		
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 

/////////// 길드 직책 변경 ////////////////
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


///////// 길드 권한 위임 목록 요청 ////////////////
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

///////// 길드 권한 위임 ////////////////
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

/////// 길드장 변경 //////////////////
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
	PLAYERKEY					m_LeaverObjectKey;							//< 길드 탈퇴한 플레이어 키
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE; 


////////////////길드 해산/복구 ////////////////

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

////////////////길드 마크 등록////////////////
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

////////////////길드 마크 삭제////////////////
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

////////////////길드 마크 실패 응답////////////////
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

////////////////길드 창고 오픈////////////////
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

////////////////길드 창고 닫기////////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_SYN)
	BYTE m_NPCCode;
	GUILDGUID m_GuildIndex;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_ACK)
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_CLOSE_NAK)
	DWORD m_bErrorCode;
PACKET_CG_CLOSE;

////////////////길드 창고 돈 입출금///////////////
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

////////////////길드 창고 슬롯 확장///////////////
PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_TAPEX_ACK)
	BYTE m_SlotTabNum;
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_WAREHOUSE_TAPEX_NAK)
BYTE m_bErrorCode;
PACKET_CG_CLOSE;

//{__NA000000_20081106_GUILD_NAME_CHANGE
///////////////길드명 변경 ///////////////////////////

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_SYN)
SLOTIDX						m_atIndex;
POSTYPE						m_atPos;
TCHAR						m_tszGuildName[MAX_GUILDNAME_LENGTH+1];		// 변경할 길드명
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_BRD)
    TCHAR m_tszGuildName[MAX_GUILDNAME_LENGTH + 1]; // 변경된 길드명
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_SECTOR_BRD)
    DWORD m_dwPlayerKey; // 변경 길드원
    TCHAR m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_CLOSE;

PACKET_CG_FWD_OPEN(CG_GUILD, CG_GUILD_NAME_CHANGE_RELATION_BRD)
    BYTE m_RelationType;
    GUILDGUID m_GuildGuid; // 변경 길드
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
///////////////길드소개  ///////////////////////////
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
///////////////길드 가입 요청  ///////////////////////////
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

///////////////길드 시설  ///////////////////////////
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_EVENT 이벤트 인벤토리 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
	POSTYPE				m_ItemPos;								//< 인벤토리로 옮기고자 하는 아이템의 이벤트 인벤토리 위치
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
// 이벤트 아이템 등록 상태 조회 요청
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_SYN )
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// 이벤트 아이템 등록 상태
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_ACK )
	WORD					m_wEventNum;						// 이벤트 개수
	EVENT_STATUS			m_EventStatus[_MAX_EVENT_ROW_NUM];	//

	int GetSize()
	{
		//ASSERT( m_wEventNum < _MAX_EVENT_ROW_NUM );
		return (int)( ( sizeof(MSG_CG_EVENT_ITEM_STATUS_ACK) - sizeof(m_EventStatus) ) + sizeof(EVENT_STATUS)*m_wEventNum );
	}
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// 등록 상태 조회 실패
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_STATUS_NAK )
	CODETYPE			m_dwEventID;
	DWORD				m_dwErrorCode;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// 이벤트 아이템 등록 요청

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_SYN )
	DWORD				m_dwEventID;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// 이벤트 아이템 등록 성공(경품 응모)
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_ACK )
	DWORD				m_dwEventID;
PACKET_CG_FWD_CLOSE;

// <Waverix><Working Packet>
// 이벤트 아이템 등록 실패
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REG_NAK )
	//enum	// errorCode;
	//{
	//	_ERR_DEFAULT,
	//	_ERR_INVALID_EVENT,		// 이벤트 중이 아니다
	//	_ERR_ITEM_NUM_OVERFLOW,	// 이벤트 아이템 등록 제한 개수를 넘었다
	//	_ERR_NOTEXISTITEM,		// 이벤트에 응모할 아이템이 없다
	//	_ERR_CANT_OBTAIN,		// 보상 아이템을 넣을 수가 없다
	//};
	CODETYPE				m_dwEventID;
	DWORD					m_dwErrorCode;
	INVENTORY_TOTAL_INFO	m_TotalInfo;		// 날아간 아이템 다시 보상

	inline WORD				GetSize()
	{ return (WORD)( (sizeof(*this)-sizeof(INVENTORY_TOTAL_INFO)) + m_TotalInfo.GetSize() ); }
PACKET_CG_FWD_CLOSE;

// 이벤트 아이템 보상 받음(W->C) - 아이템 보상
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_ITEM_REWARD_CMD )
	CODETYPE				m_dwEventID;
	MONEY					m_Money;
	INVENTORY_TOTAL_INFO	m_TotalInfo;
	inline WORD				GetSize()
	{ return (WORD)( (sizeof(*this)-sizeof(INVENTORY_TOTAL_INFO)) + m_TotalInfo.GetSize() ); }
PACKET_CG_FWD_CLOSE;


// 이벤트용 누적시간 전달 - 처음 캐릭터 선택할 때 전송
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_CMD )
	int		m_EventUseTime;		// 이벤트 누적 시간
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_QUERY_EXIST_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_QUERY_EXIST_ACK )
	enum eEXIST_RESULT {
		EMPTY		= PROTOCOL_GENERATOR_EVENT_081,
		EXIST		= PROTOCOL_GENERATOR_EVENT_096,
		DISABLED	= PROTOCOL_GENERATOR_EVENT_100,
		// DISABLED - 현재 이벤트 인벤토리 서비스 플로를 위반한 경우
		// 방식은 CG_EVENT_SELECT_SYN 과 동일하게 처리하면 된다.
	};
	BYTE			m_bExist;
PACKET_CG_FWD_CLOSE;



	PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_TIME_PERIOD_SYN )
	PACKET_CG_FWD_CLOSE;				// 서버시간 요청

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
	PACKET_CG_FWD_CLOSE;				// 서버시간 요청 에러


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_REQUEST_ITEM_SYN )
	CODETYPE				m_EventID;
	BYTE					m_RequestItemIndex;	// (0,2) <- EVENT_INFO::m_dwRewardIndex의 인덱스

	inline VOID				SetInfo( const CODETYPE eventID, const BYTE requestItemIndex )
	{ m_EventID = eventID; m_RequestItemIndex = requestItemIndex; }
PACKET_CG_FWD_CLOSE;

struct MSG_CG_EVENT_REQUEST_ITEM_BADGEMAN_SYN : MSG_CG_EVENT_REQUEST_ITEM_SYN
{
	enum eSELECT_TYPE { SOLO_BADGE = 0, COUPLE_BADGE, INVALID_INDEX };	//m_RequestItemIndex에 넣을 값
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
	//	ERR_DEFAULT					= 0,		// 기본 에러,
	//	ERR_INSUFFICIENT_HEIM		,			// 기부하려던 금액을 가지고 있지 못하다.
	//	ERR_DONATION_HEIM_LIMIT		,			// 최소 기부해야 할 금액에서 모자르다.
	//	ERR_CANT_OBTAIN_ITEM		,			// 보상 받을 수 있는데, 아이템을 획득하지 못하다.
	//	SUCCESS						,			// 일반 성공 (보상 조건 만족시 EVENT_REWARD_ITEM으로 전송됨)
	//};
	CODETYPE				m_EventID;
	BYTE					m_ErrorCode;
	MONEY					m_TotalHeims;	// 지금까지 누적된 액수

	inline VOID				SetInfo( const CODETYPE eventID, RC::eEVENT_RESULT errorCode ) { m_EventID=eventID; m_ErrorCode = (BYTE)errorCode; }
	inline VOID				SetTotalHeim( const MONEY totalHeim ) { m_TotalHeims = totalHeim; }
	inline WORD				GetSize()
	{ return (RC::RC_EVENT_SUCCESS == m_ErrorCode) ? sizeof(*this) : sizeof(*this)-sizeof(m_TotalHeims); }
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_BUFF_EVENT_SYN )
	// 특수 타입을 지원하기로 한다. 클라가 EventID를 선택하려면 클라에서 시간을 계산해서 요청해야 한다.
	// 기존에는 안됐겠지만, 지금은 NPC ID가 존재하므로 NPC ID를 보내는 형태로 하자.
	MONSTERCODE				m_NpcCode;

	inline VOID				SetInfo( const MONSTERCODE npcCode ) { m_NpcCode = npcCode; }
	inline WORD				GetSize()	{ return sizeof(*this); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_BUFF_EVENT_ACK )
	// m_ResultCode = RC::eEVENT_RESULT
	//	-	RC::RC_EVENT_SUCCESS			(기존)-기존 상태 부여 패킷이 추가로 갈 수도 있다. 이건 좀더 따지자.
	//	-	RC::RC_EVENT_FAILED				(기존:기타 이유로 실패시)
	//	-	RC::RC_EVENT_CANT_BUFF_GRANTED	(신규:이미 부여되어 있을 경우)
	//	-	RC::RC_EVENT_INVALID_EVENT		(기존)
	//	-	RC::RC_EVENT_UNQUALIFIED		(신규:보상을 받을 수 없다. 자격이 안되서) + 추가 사유 기재 필요

	// m_AddinInfo = eRET_UNQUALIFIED
	enum eRET_UNQUALIFIED
	{
		RUC_EMPTY				,	// 자격 요건 제한에 걸리지 않음.
		RUC_LEVEL_LOWERBOUND	,	// LEVEL 하한 조건
		RUC_LEVEL_UPPERBOUND	,	// LEVEL 상한 조건
		RUC_CLASS_MISMATCH		,	// 캐릭터 클래스 (RESERVED)
		RUC_MORAL				,	// 캐릭터 성향 (RESERVED)
	};

	BYTE						m_ResultCode;
	union {
		// EventID가 존재하는 결과
		struct {
			CODETYPE			m_EventID;		// 활성화된 EventID
			BYTE				m_AddinInfo;	// RESERVED
		} RESULT_SUCCESS;
		struct {
		} RESULT_FAIL;							// 이벤트 중이 아니거나 서버 내부의 이유로 실패시
		struct {
			CODETYPE			m_EventID;		// 활성화된 EventID
		} RESULT_CANT_BUFF_GRANTED;
		struct {
			CODETYPE			m_EventID;		// 활성화된 EventID
			BYTE				m_AddinInfo;	// eRET_UNQUALIFIED 추가 사유 기재
		} RESULT_UNQUALIFIED;
	};

	// 기초값 설정 : 추가 정보는 해당 코드에 맞게 설정할 것.
	// m_ResultCode에 따라 입력하는 인자가 다르다.
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

// 친구 초대 이벤트에 대한 보상 목록 요청
PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN )
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK )
enum {DB_FAIL = 1, DBPROXY_FAIL = 2};
BYTE		byErrorCode;  //1: DB 시스템 오류, 2: DB Proxy 서버 접속 불가
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK )
    BYTE                        m_Count;            //보상 아이템 목록 개수
    InvitateFriendRewardInfo    RewardInfo[MAX_FRIENDINVITATE_REWARD_NUM];
    int GetSize()
    {
        return ( sizeof(MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK) - ((MAX_FRIENDINVITATE_REWARD_NUM - m_Count) * sizeof(InvitateFriendRewardInfo)) );
    }
PACKET_CG_CLOSE;
// END 친구 초대 이벤트에 대한 보상 목록 요청

// 친구 초대 이벤트에 대한 보상 받기 요청
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_SYN )
    CHARGUID    FriendGUID;//친구 GUID
    BYTE        ReWardType;//보상 타입
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
1: DB 시스템 오류
2: DB PROXY 서버 연결 불량
3: 이미 보상 받았음
4: 인벤에 넣을수 없음
*/
PACKET_CG_FWD_CLOSE;
// END 친구 초대 이벤트에 대한 보상 받기 요청

// 친구의 리콜 (C->A->D)
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_SYN )
    CHARGUID    FriendGUID; //소환할 친구의 GUID
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
    BYTE		m_byErrorCode;  //에러코드
    DWORD64     m_LeftTime;     //에러코드가 RECALLTIME_SHORTAGE 경우 소환하기까지 남은 시간 YYYYMMDDHHmmSS 한시간 남았다면 10000
/*
1: DB 시스템 오류
2: DB PROXY 서버 연결 불량
3: 다른 채널에 있음
4: 소환 잔여시간 부족
5: 소환이 불가능한 위치에 있음(내가)
6: 소환이 불가능한 위치에 있음(소환하려는 애가)
7: 이벤트 종료 되었음
8: 소환할 유저 찾을수 없음
9: 친구에게 전송 실패
10: 친구가 거절
*/
PACKET_CG_FWD_CLOSE;

// 친구의 리콜에 응할것인지 파악한다.
PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_SYN )
    CHARGUID    FriendGUID;                         //소환 요청한 캐릭 GUID
    TCHAR		FriendName[MAX_CHARNAME_LENGTH+1];  //소환 요청한 캐릭 이름
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_ACK )
    CHARGUID    FriendGUID;                         //소환 요청한 캐릭 GUID
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK )
    CHARGUID    FriendGUID;                         //소환 요청한 캐릭 GUID
PACKET_CG_FWD_CLOSE;
//

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
//지역 점령전 경매 입찰 아이템을 유저에게 보상
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_DOMINATION_REWARD_CMD)
    MEMOGUID memo_guid_;// 메모Guid
    TCHAR memo_subject_[MAX_MEMO_LENGTH + 1];//쪽지 제목 내용
PACKET_CG_FWD_CLOSE;
//하임 로또 당첨금을 유저에게 보상
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_LOTTO_REWARD_CMD)
    MEMOGUID memo_guid_;// 메모Guid
    INT64 lotto_entry_index_;//로또 인덱스
PACKET_CG_FWD_CLOSE;
//해당 메모에 대한 보상 결과 값을 유저에게 알려준다.
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_ITEM_REWARD_RESULT_CMD)
    MEMOGUID memo_guid_;//메모Guid
    bool result_value_;//보상 결과값 true / false
PACKET_CG_FWD_CLOSE;
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_CG_FWD_OPEN(CG_EVENT, CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ)
    MEMOGUID memo_guid_;
    TCHAR memo_contents_[MAX_MEMO_LENGTH + 1];//게임 로그를 위한 자료로 사용
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
    //시작
    BYTE start_day;     // 시작일 (day)
    DWORD start_time;   // 시작시간 (hhmm)

    //종료
    BYTE end_day;       // 종료일 (day)
    DWORD end_time;     // 종료날짜 (hhmm)

    float exp_ratio;     // 경험치비율 (100%증가 -> 1)

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
//처음 들어온 플레이어에게 진행중인 이벤트의 경험치정보를 전달해준다
PACKET_CG_FWD_OPEN(CG_EVENT, CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD)
    FLOAT      exp_ratio;
PACKET_CG_FWD_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_QUEST 퀘스트 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_STREAM_CMD )
	int GetSize() { return sizeof(*this); }
	//QUEST_COMPL_TOTAL_INFO의 GetSize()만큼
	//QUEST_PROGR_TOTAL_INFO의 GetSize()만큼
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_ACCEPT_SYN )
	WORD		m_QuestCode;
    //{__NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT
	BYTE		m_byIsItemAccept;				// NPC 수락 : 0 ,	아이템 수락 : 1
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
	BYTE						m_SelectedItem;		//< 선택한 아이템들
PACKET_CG_FWD_CLOSE;

// 강제 보상 (GM)
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_CMD )
    EXPTYPE						m_i64Exp;					// 최종 경험치
    MONEY						m_Money;
    SLOTCODE					m_SkillCode;
    BYTE						m_byFatigueType;			//피로도 타입.
    WORD						m_wQuestCode;
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    WORD                        m_wAccumulatePoint;
#endif

    int GetSize() { return sizeof(*this); }
PACKET_CG_FWD_CLOSE;


// 보상처리는 미션의 보상스크립트로 처리할 가능성이 많다
PACKET_CG_FWD_OPEN( CG_QUEST, CG_QUEST_REWARD_ACK )
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    int                         m_BunusExpRatio;            //추가 경험치 비율
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	EXPTYPE						m_i64Exp;					// 최종 경험치
	MONEY						m_Money;
	SLOTCODE					m_SkillCode;
	BYTE						m_byFatigueType;			//피로도 타입.
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
	BYTE		m_Force;		//1: 강제 포기[타임퀘스트 시간 완료], 0: 유저포기 
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
        OPERATION_ALL_INFO  = 1 << 0,   // 전체 정보 보낼 경우 (EnterZone)
        OPERATION_ADD_INFO  = 1 << 1,   // 퀘스트 수락시와 같은 기존 정보 + a

        CODETYPE_NPC        = 1 << 4,   //
        // 1 << 7이 Max
        MAX_OBJECT_ARRAY_SIZE = 100,
    };
    struct Node {
        WORD questCode; // QCODE
        WORD npcCode;
        short posX; // 소수점 이하 제거 수치
        short posY; // 소수점 이하 제거 수치
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
    WzVector	m_wvCurPos;				// 클라이언트가 주는 플레이어의 현재 좌표 (검증용)
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_MISSION 미션 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_STREAM_CMD)
	MISSION_TOTAL_INFO m_TotalInfo;
	int GetSize()
	{
		return (sizeof(MSG_BASE_FORWARD)+m_TotalInfo.GetSize());
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RECORD_ELAPSEDTIME_CMD)
	DWORD			m_MissionElapsedTimePerSecond;				//< 미션 경과된 시간(초단위)
	DWORD			m_MissionBestClearTimePerSecond;			//< 현재주 미션 최고 기록
PACKET_CG_FWD_CLOSE;

//// this packet will be deprecated!!!!!!!
//PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RECORD_CLEARTIME_CMD)
//	DWORD			m_MissionClearTime;			//< 미션 클리어한 시간(초단위)
//PACKET_CG_FWD_CLOSE;

#ifdef _NA_008012_20150130_RANKING_SYSTEM
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_INFO_SYN)
	BYTE			m_Type;			//< eRANKING_TYPE 참조
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RANKINGID       m_RankingID;
#else
    RANKINGNO		m_MissionNo;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	
	BYTE			m_Page;			//< page : 0 ~ 4 ( 총 10개씩 5페이지 )
PACKET_CG_FWD_CLOSE;

// 가변 패킷
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
	BYTE			m_Type;			//< eRANKING_TYPE 참조
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
//	DWORD			m_MissionElapsedTimePerSecond;				//< 타이머가 변경되고 난뒤의 현재 경과된 시간(초단위)
	WORD			m_TotalMonsterKillNum;						//< 누적된 몬스터 킬수
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_CLEAR_RECORD_CMD)
	DWORD			m_MonsterKillNum_General;
	DWORD			m_MonsterKillNum_Leader;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SUCCESS_BRD)
	RANKINGKEY		m_Key;										//< 파티키 and 랭킹키
	DWORD			m_ClearTime;
	WORD			m_Ranking;									//< 랭킹
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
    RANKINGKEY		m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
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
//랭킹 결산일 패킷
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK)
    //미션랭킹 항목이 늘어나면 수치를 바꿔주도록 한다.
    //MAX_REWARDABLE_MISSION_LIST_SIZE는 랭킹미션리스트의 3배로 한다
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
// 랭킹 관련
//------------------------------------------------
PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_GET_MISSION_POINT_BRD)
	DWORD			m_dwPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_MISSION, CG_MISSION_POINT_CANNOT_REGISTER_BRD)
PACKET_CG_FWD_CLOSE;
#endif // _NA_008012_20150130_RANKING_SYSTEM

//----------------------------------------------------------------------------
// <TUTORIAL MISSION 관련>
PACKET_CG_OPEN( CG_MISSION, CG_MISSION_SET_CLEAR_COUNT_CMD )
    MAPCODE m_MapCode;
    WORD    m_ClearCount;
PACKET_CG_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_AREACONQUEST AC 관련 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
BYTE m_byRewardIndex;		//< 보상 단계의 인덱스
#endif
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_AREACONQUEST,CG_AREACONQUEST_REWARD_ACK)
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
WORD                    index_;                     //업적 인덱스
WORD					m_accomplishment_ratio;		//추가된 달성률
WORD					location_code_;
#endif
EXPTYPE					m_i64Exp;					// 최종 경험치
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_AREACONQUEST AC 관련 패킷 끝
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
	DWORD		m_dwErrorCode;									//< ResultCode.h의 eWP_RESULT 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_SYN)
	//현재 맵 번호를 웨이 포인트에 저장한다.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_WAYPOINT_INFO_SAVE_NAK)
	DWORD		m_dwErrorCode;									//< ResultCode.h의 eWP_RESULT 참조
PACKET_CG_FWD_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 개인 웨이 포인트
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
//현재 맵 번호, 좌표를 웨이 포인트에 저장한다.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_SAVE_ACK)
//저장된 index, 맵 번호, 좌표를 전송한다.
WAYPOINTSLOT m_Info;
int GetSize()
{
	return sizeof(MSG_BASE_FORWARD) + sizeof(WAYPOINTSLOT);
}
PACKET_CG_FWD_CLOSE;



PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_DEL_SYN)
//삭제할 index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_DEL_ACK)
//삭제된 index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;


PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_EXE_CMD)
//실행할 index
DWORD		m_IdxWayPoint;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_WAYPOINT,CG_CHAR_WAYPOINT_INFO_NAK)
DWORD		m_dwErrorCode;									//< ResultCode.h의 eWP_RESULT 참조
PACKET_CG_FWD_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 카오 시스템
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_CHANGE_BORADORI_STATE_BRD )
	DWORD		m_dwObjKey;
	bool		m_bBoradori;		// 보라돌이 상태 적용/해제
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_CHANGE_CHAO_STATE_BRD )
	DWORD		m_dwObjKey;
	BYTE		m_byChaoState;		// 카오상태(eCHAO_STATE)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ASK_REVENGE_ADDITION_CMD )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지 리스트에 추가할 대상 Name
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_SYN )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지 리스트에 추가할 대상 Name
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_ACK )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지 리스트에 추가된 대상 이름(상세정보는 World에서 넘겨준다)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_ADD_REVENGE_CHAR_NAK )
	BYTE		m_byErrorCode;		// RC:eREVENGE_RESULT
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_NOTIFIY_CHAO_TIME_CMD )
	DWORD		m_dwLeavedTime;
PACKET_CG_FWD_CLOSE;

// 리벤지 대상으로 워프(추적)
PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_TRACK_REVENGE_CHAR_SYN )
	char		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
	POSTYPE		m_atItemPos;		// 복수의 돌 슬롯위치
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_CHAO, CG_CHAO_TRACK_REVENGE_CHAR_NAK )
	BYTE		m_byErrorCode;		// RC:eREVENGE_RESULT
PACKET_CG_FWD_CLOSE;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 샵 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
//  programcommon\resultcode.h

// 캐시샵 버젼 요청
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_ACK)
    SalesZoneScriptVersion sales_version_;
    BannerZoneScriptVersion banner_version_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_VERSION_NAK)
    int error_code_;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

// 캐시 요청
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_ACK)
    CashInfo cash_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CASH_NAK)
    int error_code_;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;


// 캐시샵 오픈
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_SYN)
    SalesZoneScriptVersion sales_version_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_ACK)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_OPEN_NAK)
    int error_code_;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

// 캐시샵 종료
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_CLOSE_NTF)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// 아이템 보관함 요청
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_SYN)
    int page_;
    int item_count_per_page_;   // MAX_STORAGE_ITEM_PER_PAGE 이하.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_ACK)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(item_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(item_info_[0]) * item_count_);
    }
    int page_;                  // 현재 페이지
    int total_page_;            // 전체 페이지 수
    int total_item_count_;      // 전체 상품 수
    int item_count_;            // 현재 페이지 상품 수
    StorageItemInfoBuy item_info_[BUY_STORAGE_ITEM_PER_PAGE];
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_BUY_STORAGE_LIST_NAK)
    int error_code_;
PACKET_CG_FWD_CLOSE;


// 선물 보관함 요청
PACKET_CG_FWD_OPEN(CG_SHOP, CG_SHOP_GIFT_STORAGE_LIST_SYN)
    int page_;
    int item_count_per_page_;   // MAX_STORAGE_ITEM_PER_PAGE 이하.
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


// 구매하기
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


// 선물하기
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


// 보관함의 아이템 사용
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


// 캐릭터 이름 체크
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

// 샵 UI를 처음 오픈했을 때
// 카테고리 혹은 페이지가 달라졌을 때
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_SYN)
    uchar   partition_index;                          //< 파티션
    uchar   category_index;                           //< 카테고리 번호
    uchar	request_page;					    	  //< 요청 페이지 번호 (0번 부터 시작)
    ushort	item_filter;							  //< 필터
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_ACK)
    enum { MAX_BUFFER_SIZE = 4096, MAX_GUID = 10, };
    uchar   partition_index;                          //< 파티션
    uchar   category_index;                           //< 카테고리 번호
    ushort	item_filter;							  //< 필터
    uchar	answer_page;					    	  //< 응답 페이지 번호 (0번 부터 시작, 요청된 페이지 번호가 전체 페이지수를 넘어설경우 마지막 페이지 번호로 바뀐다.)
    uchar   page_count;                               //< 요청한 아이템의 전체 페이지 수  
    ushort  buffer_size;                              //< 버퍼 사이즈  
    ushort  guid_count;
    uint    shopitem_guid[MAX_GUID];
    uchar   buffer[MAX_BUFFER_SIZE];                  //< 버퍼
    int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-buffer_size); }
PACKET_CG_FWD_CLOSE;
#else //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_LIST_REQ_SYN)
	BYTE		m_byCategoryIndex;								//< 카테코리 번호
	BYTE		m_byPage;										//< 페이지 번호
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
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

// 샵 UI를 처음 오픈했을 때
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_ACK)
    CashData    cash_info_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CASH_REQ_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;


// 아이템 구매를 할때
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_BUY_SYN)
	enum { _MAX_BUY_LIST_NUM = 32, };
	BYTE		m_Count;
	SLOTIDX		m_SlotIndex;
	POSTYPE		m_SlotPos;		// 기간연장 아이템 구입일 경우 연장할 아이템의 인벤토리 슬롯Pos
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
	INT		m_ExtendItemCode;		// 기간연장 아이템에 대한 아이템코드
	JUST_INVENTORY_TOTAL_INFO	m_TotalInfo;
	int GetSize() const
	{
        const size_t NoStreamSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<WORD>(NoStreamSize + m_TotalInfo.GetSize());
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_BUY_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
	DWORD		m_PriceGuid;									//< m_PriceGuid = 0 이면 모든 상품 실패
PACKET_CG_FWD_CLOSE;

// 아이템 구매이력 탭을 처음으로 볼때
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_ACK)
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_BUY_HISTORY_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_SYN)
	INT			m_ItemGuid;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_ACK)
	// ShopItem의 Seriailize()
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ITEM_REQ_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;


//// 아이템 구매를 할때
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
//	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
//	DWORD		m_PriceGuid;									//< m_PriceGuid = 0 이면 모든 상품 실패
//PACKET_CG_FWD_CLOSE;
//



PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_SYN)
	char		m_tszCharName[MAX_CHARNAME_LENGTH+1];				//< 캐릭터 이름 체크
	WORD GetSize()
	{
		return (WORD)(sizeof(*this) - ((MAX_CHARNAME_LENGTH+1)-(int)strlen(m_tszCharName))) ;
	}
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_ACK)
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_CHECK_CHAR_NAME_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_SYN)
	enum { SINGLEITEM=1, PACKAGEITEM=2,};
	enum { MAX_GIFT_MESSAGE_LEN = 512, };
	BYTE	m_ItemType;											//< enum { SINGLEITEM=1, PACKAGEITEM=2,};
	INT  	m_PriceGuid;										//< 판매 가격 고유 번호
    enum {
        PURCHASE_DEFAULT = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_DEFAULT,
        PURCHASE_PPCASH  = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_PPCASH
    };
    BYTE    m_PurchaseType; // ~_PPCARD = { pp card }, ~_DEFAULT = { cash, mileage, heim, etc... }
	char	m_tszCharName[MAX_CHARNAME_LENGTH+1];				//< 캐릭터 이름 체크
	char 	m_tszGiftMessage[MAX_GIFT_MESSAGE_LEN];				//< 선물 메세지
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
	int  price_guid_; //< 판매 가격 고유 번호
    BYTE purchase_type_; // ~_PPCARD = { pp card }, ~_DEFAULT = { cash, mileage, heim, etc... }
    char gift_message_[MAX_GIFT_MESSAGE_LEN]; //< 선물 메세지
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
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

// 선물을 받음 통지 패킷
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_SEND_GIFT_NTF)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_ACTION_RECV_GIFT_SYN)
	enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	enum { MAX_GIFT_MESSAGE_LEN = 512, };
	BYTE	m_ActionType;										//< enum { OBTAIN=1, REJECT=2, DISCARD=3, };
	DWORD	m_GiftGuid;											//< 선물Guid
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
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_SYN)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_ACK)
	DWORD		m_GiftCount;
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN(CG_SHOP,CG_SHOP_IS_RECV_GIFT_NAK)
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
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
	BYTE		m_byErrorCode;									//< ShopDefine.h의 eSHOP_RESULT 참조
PACKET_CG_FWD_CLOSE;




#endif //_NA_0_20110613_GAME_SHOP_RENEWAL



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 인스턴스 던전
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN )
	POSTYPE				m_ItemPos;
PACKET_CG_CLOSE;

PACKET_CG_OPEN( CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_REQUEST_RESULT_NAK )
	BYTE				m_byKind;			//0 이면 eITEM_RESULT 1이면 eROOM_RESULT
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
	// 저탑 정보
	struct {
		eJION_TOP_STATE		m_eJoinState;
		DWORD				m_JoinOverTime;
	};
	// 타락 정보
	nsSSQ::SSQTicketInfo	m_SSQTicketInfo;
PACKET_CG_FWD_CLOSE;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	타락한 사원 프로토콜 정의 <- 세부 정의는 ProgramCommon/SSQ/SSQInfoStruct.hxx에 존재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//─────────────────────────────────────────
//
//
PACKET_CG_OPEN(CG_STATUS, CG_STATUS_SSQ_INFO_BRD) // variable size packet
    WAR_MSG_HDR m_Header[1]; // Stream Header, next pointer is data stream
    uint16_t GetSize() const
    { return static_cast<uint16_t>(sizeof(*this) + m_Header->size); }
PACKET_CG_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_CTRL_CMD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;

//	아마 이건 사용하지 않을 가능성이 크다. C->G 형태이므로
PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_SYN, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;

PACKET_CG_INHERIT_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_BRD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_CG_INHERIT_CLOSE;
//
//─────────────────────────────────────────
//
//
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_SYN)
	POSTYPE				m_ItemPos;	// 사용하는 Item의 슬롯 위치
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_ACK)
	enum eRESULT { FAIL = PROTOCOL_GENERATOR_ZONE_EX_091, SUCCESS = PROTOCOL_GENERATOR_ZONE_EX_054 };
	enum eFAIL_RC_CODE { RET_ITEM = PROTOCOL_GENERATOR_ZONE_EX_044, RET_ROOM = PROTOCOL_GENERATOR_ZONE_EX_116 };
	BYTE				m_Result;	// eRESULT

	struct sRETINFO_FAIL {
		BYTE		m_Kind;		// eFAIL_RC_CODE { RET_ITEM 이면 eITEM_RESULT, RET_ROOM이면 eROOM_RESULT }
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

	// for Client 성공 여부 판단용
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


PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD )	//G->C 마을|필드 입장후 통지 관련 정보 + 환불시 환불 정보
	SSQ_RESERVED_REWARD_INFO		m_RewardInfo;
	struct RefundInfo {
		RC::eITEM_RESULT				Result;		// 환불시 아이템 처리 실패 가능성 있으므로...
		MONEY							ChangedTotalHeim;
		INVENTORY_TOTAL_INFO			TotalInfo;
	}								m_Refund;	// 환불시 사용
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

PACKET_CG_OPEN( CG_ZONE_EX, CG_ZONE_EX_SSQ_WINNER_REWARD_CMD )	//G->C 승자에 대한 인벤토리의 아이템 보상
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// CG_COLLECT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
S_COLLECTKEY	m_ObjKeyCollection;		// 채집물품
WzVector		m_wvCurPos;				// 클라이언트가 주는 플레이어의 현재 좌표 (검증용)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_CANCEL_SYN )
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_SUCCESS_SYN )
PACKET_CG_FWD_CLOSE;
    //.//
// CHANGES: f110425.1L, moved to sync
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTION_BRD )
C_COLLECTKEY	m_dwObjectKey;			// 채집물품.
WzID			m_CollectionID;			// 채집물품ID.
DWORD			m_Collector;			// 채집자.
PACKET_CG_FWD_CLOSE;

// CHANGES: f110425.1L, moved to sync
PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_CANCEL_BRD )
C_COLLECTKEY	m_dwObjectKey;			// 채집물품.
WzID			m_CollectionID;			// 채집물품ID.
DWORD			m_Collector;			// 채집자.
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_COLLECT, CG_COLLECT_ACTIVE_BRD )
C_COLLECTKEY	m_dwObjectKey;			// 채집물품.
WzID			m_CollectionID;			// 채집물품ID.
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
//─────────────────────────────────────────

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

// 포탈 생성 명령 (임시로 생성되는 포탈 형태)
PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD)
    QuestPortalInfo questPortalInfo;
PACKET_CG_CLOSE;

PACKET_CG_OPEN(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD)
    KEYTYPE     portalKey;    // 
PACKET_CG_CLOSE;

// MapEnterancePortal.txt에 의한 고정 포탈형 미션 ||
// QuestPortalCreate에 의해 임시 생성된 포탈 진입 요청
// (Note) 기존 MISSION_CREATE, INST~_CREATE등등을 대체할 수도 있다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	보안강화 관련 처리 패킷 정의 (MOLA 2차, 5차 Update)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/************************************************************************/
/*                       지역 점령전 관련 패킷                          */
/************************************************************************/
//"점령전 참여하기" 버튼 클릭시
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_APPLY_SYN )
    MAPCODE                 m_MapCode;                                      //요청 지역 맵코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_APPLY_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //작업 결과값 코드
    BOOL                    m_CheckBatting;                                 //배팅이 가능한지 불가능한지 체크
    BYTE                    m_DataNum;                                      //경매 리스트 갯수
    DOMINATION_ACTION_INFO  m_ActionList[MAX_DOMINATION_ACTIONLIST];        //경매 게시판 리스트(2009.09.21 현재 Max값 10개)
    uint16_t GetSize() const
    {
        assert(m_DataNum <= _countof(m_ActionList));
        const size_t kMinSize = sizeof(*this) - sizeof(m_ActionList);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ActionList[0]) * m_DataNum);
    }
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"점령전 참여하기" -> "배팅하기" 버튼 클릭시
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BATTING_HEIM_SYN )
    MAPCODE                 m_MapCode;                                      //요청 지역 맵코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BATTING_HEIM_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                    //작업 결과값 코드
    ULONGLONG               m_RemainCost;                                    //현재 남아있는 길드 하임
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    DWORD penalty_time;
#endif //_NA_000000_20130429_DOMINATION_PENALTY
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"배팅하기" 창에서 확인 버튼 눌렀을 경우
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ACCEP_HEIM_SYN )
    MAPCODE                 m_MapCode;                                      //요청 지역 맵코드
    POSTYPE                 m_TokenPos;                                     //배팅 아이템 인벤토리 위치
    BYTE                    betting_item_count_;
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ACCEP_HEIM_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //작업 결과값, 기존 점령전 참여가 있었는지 확인하는 값 추가
    POSTYPE                 m_TokenPos;
    BYTE                    betting_item_count_;
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"점령전관리" 버튼을 눌렀을 경우
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MANAGE_SYN )
    MAPCODE                 m_MapCode;                                      //요청 지역 맵코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MANAGE_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //작업 결과값 코드
    BYTE                    m_TotalPoint;                                   //디펜스 포인트 총갯수
    BYTE                    m_DefenseWall[MAX_DOMINATION_DEFENSEWALL_NUM];  //현제 지역 점령전 방어벽 별 방어력(2009.09.21 현재 Max값 3개)
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"점령전관리"창에서 확인을 눌렀을 경우
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SAVE_GATESTAT_SYN )
    MAPCODE                 m_MapCode;                                      //요청 지역 맵코드
    BYTE                    m_DefenseWall[MAX_DOMINATION_DEFENSEWALL_NUM];  //현제 지역 점령전 방어벽 별 방어력(2009.09.21 현재 Max값 3개)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SAVE_GATESTAT_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //작업 결과값 코드
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//"점령전관리" 방벽 포인트 추가 버튼을 눌렀을 경우
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN )
    MAPCODE                 map_code_;                                      //요청 지역 맵코드
    SLOTCODE                item_code_;                                     //방벽 증가 아이템 코드
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK )
    RC::eDOMINATION_RESULT  result_code_;                                   //작업 결과값 코드
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//점령전 종료후 유저들에게 점령전 종료후 미니맵변경 이벤트 플레그 세팅 알림
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_SET_BRD )
PACKET_CG_FWD_CLOSE;

/************************************************************************/
//미니맵에 길드 표시 자료 요청
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_LIST_SYN )
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MINIMAP_LIST_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;//작업 결과값 코드
    BYTE                    m_NumDominationGuild;//지역을 점령한 길드 숫자
    DOMINATION_GUILD_INFO   m_GuildInfo[MAX_DOMINATION_MAP];//점령 길드 정보

    uint16_t GetSize() const
    {
        assert(m_NumDominationGuild <= _countof(m_GuildInfo));
        return (sizeof(*this) - sizeof(m_GuildInfo)) +           //고정 크기
               (sizeof(m_GuildInfo[0]) * m_NumDominationGuild);  //가변 크기
    }
PACKET_CG_FWD_CLOSE;   

/************************************************************************/
//지역 점령 타워를 클릭했을때
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_NOTICE_SYN )
    MAPCODE                 m_MapCode;                                      //특정 지역 MapCode
PACKET_CG_FWD_CLOSE;
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_NOTICE_ACK )
    RC::eDOMINATION_RESULT  m_ResultCode;                                   //작업 결과값 코드
    BOOL                    m_EnterFlag;                                    //입장버튼 활성화 flag
    BOOL                    m_AuctionFlag;                                   //경매버튼 활성화 flag
    BOOL                    m_ManageFlag;                                   //관리버튼 활성화 flag
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    BOOL                    m_JoinFlag;                                     //점령전 참여 활성화 flag
    DOMINATION_GUILD_INFO   m_GuildInfo;                                    //공격에 선정된 길드 정보
PACKET_CG_FWD_CLOSE;


//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
/************************************************************************/
//참여 멤버 리스트 알림
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
//점령전 멤버로 참여 요청
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_JOIN_SYN )
    MAPCODE                 map_code_;
PACKET_CG_FWD_CLOSE; 
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_JOIN_ACK )
    RC::eDOMINATION_RESULT  result_code_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//점령전 멤버 참여 취소
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_LEAVE_SYN )
    MAPCODE                 map_code_;
    CHARGUID                memeber_guid_;
PACKET_CG_FWD_CLOSE; 
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_MEMBER_LEAVE_ACK )
    RC::eDOMINATION_RESULT  result_code_;
    CHARGUID                memeber_guid_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//점령전 리스트 새로 고침 요청
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_REFRESH_MEMBERLIST_REQ )
    MAPCODE                 map_code_;
PACKET_CG_FWD_CLOSE; 

/************************************************************************/
//지역 점령전 공격길드에 선정 되었을 경우
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_SELECTED_BRD )
    MAPCODE m_DominationFieldCode;                                          //점령전을 실행할 지역 코드
PACKET_CG_FWD_CLOSE;

    //.//
    //_NA001385_20090924_DOMINATION_ETC

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_AREA_INFO_CMD )        // 플레이어 점령지 정보 변경 공지 (자신과 주위에)
    //_NA_0_20100208_DOMINATION_AREA_INFO
    DWORD					    m_dwPlayerKey;						// 변경 길드원 ObjectKey
    DOMINATION_STATE_TOTAL_INFO m_stateTotalInfo;                   // 점령전 상태 정보
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_stateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_stateTotalInfo.GetSize());
    }
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_TIME_SYNC_BRD )        // 점령전 시작시 또는 중간 시간 동기화시 해당 맵 유저에게
    DWORD m_dwRemainedTime;                                         // ms (최대: 3600000)
    BYTE  m_byEventState;                                           // 점령전 진행여부. (eDOMINATION_EVENT_STATE)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_COUNT_NOSES_BRD )      // 점령전 맵에 입.퇴장시 맵 전체 유저에게 카운팅 발송
    BYTE m_byCount;                                                 // 해당 길드 인원수  (현재/최대)
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_KICK_NOTIFY_BRD )      // 점령전 시스템 준비를 위해 타워 입구로 강제 이동 (10초전)
PACKET_CG_FWD_CLOSE;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_TURNOVER_BRD )      // 점령전 턴오버시 클라이언트에게 알림
    GUILDGUID m_winGuid;                                          // 각인 성공 길드
    MAPCODE   m_mapCode;                                          // 점령지역
    char      win_guild_name[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const 
    {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_START_NOTIFY_BRD )      // 점령전 시작 전 알림 (점령전이 시작될 예정입니다. ~~)
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    eDOMINATION_NOTIFY_TIME notify_type;                             // 알림 메시지 타입
    BYTE    m_byTime;                                                // 분
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    eDOMINATION_TYPE m_domination_type;                              // 점령전 종류
#endif
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_CRYSTALWARP_STATE_BRD )  // 수정 워프 소환, 소환 해재, 파괴 알림.
    DWORD   m_dwCrystalWarpKey;                                       // 수정 워프 오브젝트키
    BYTE    m_byState;                                                // 상태
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;
    //.//
    //_NA001385_20090924_DOMINATION_STAMP

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_BEGIN_STAMP_BRD )      // 점령전 각인 시작시 해당 맵 공격길드원에게
    WzID m_nStampID;			                                    // 채집물품ID.
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    char m_stamp_guild_name[MAX_GUILDNAME_LENGTH+1];//각인 길드 이름
#endif
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_CANCLE_STAMP_BRD )     // 점령전 각인 실패시 해당 맵 공격길드원에게 (클라 cancle 패킷에 의한 BRD)
    WzID m_nStampID;			                                    // 채집물품ID.
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_RESULT_BRD )           // 점령전 결과
    GUILDGUID m_winGuid;                                            // 승
    GUILDGUID m_loseGuid;                                           // 패
    MAPCODE   m_mapCode;                                            // 점령지역
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    char win_guild_name[MAX_GUILDNAME_LENGTH+1];
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CG_FWD_CLOSE;

PACKET_CG_FWD_OPEN( CG_BATTLE, CG_DOMINATION_UPDATE_GUILDINFO_BRD )
    WORD domination_map_count_;                                     // 점령지역 수
    WORD domination_join_count_;                                    // 점령전 참여 횟수
    WORD domination_defense_count_;                                 // 점령전 방어 성공 횟수    
    WORD domination_hunting_count_;                                 // 점령전 보스 사냥 횟수
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

/**********************지역 점령전 관련 패킷 END*************************/

#ifdef _NA_0_20100819_AGGRO_RENEWAL
PACKET_CG_FWD_OPEN( CG_BATTLE, CG_AGGRO_VALUES_CMD )	
	BYTE aggro_player_count_;										//몬스터가 어글 포인트를 가지고 있는 플레이어 수
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
    BYTE aggro_player_count_;										//몬스터가 어글 포인트를 가지고 있는 플레이어 수
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
// 랭킹 요청 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SYN)
    eSUN_RANKING_TYPE ranking_type;
    bool is_guild_ranking;
PACKET_CG_CLOSE;

// 랭킹 요청 결과 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_ACK)
    enum { MAX_INFO_COUNT = 50, };
    SunRankingInfoForClient infos[MAX_INFO_COUNT]; //상위 랭커 정보
    int   my_ranking;
    SCORE my_score;
PACKET_CG_CLOSE;

// 랭킹 요청 실패 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_NAK)
    int error_code;
PACKET_CG_CLOSE;

// 동상 정보 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_STATUE_INFO_ACK)
    StatueInfo info;
PACKET_CG_CLOSE;

// 랭킹 정산 정보 요청 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SETTLE_INFO_SYN)
PACKET_CG_CLOSE;

// 랭킹 정산 정보 패킷
PACKET_CG_OPEN(CG_ETC, CG_ETC_SUN_RANKING_SETTLE_INFO_ACK)
    BYTE type;
    BYTE day;
    WORD time;
PACKET_CG_CLOSE;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/************************************************************************/
//전장 시스템
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