#pragma once
#ifndef SERVERCOMMON_PACKET_STRUCT_AW_H
#define SERVERCOMMON_PACKET_STRUCT_AW_H

//==================================================================================================

#include <Protocol_AW.h>
#include "./PacketStruct_AG.h"

//==================================================================================================
#pragma pack(push,1)
//==================================================================================================

#define PACKET_AW_OPEN(c, p)    struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_AW_CLOSE     };

#define PACKET_AW_INHERIT_OPEN(c, p, b) struct MSG_##p : public b { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_AW_INHERIT_CLOSE     };

//==================================================================================================
// NOTE: f110727.1L, added broadcaster, use 'MSG_ONEtoN_BROADCASTER_INTERNAL'
PACKET_AW_INHERIT_OPEN(AW_CONNECTION, AW_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN,
                       MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN)
PACKET_AW_INHERIT_CLOSE;

typedef VARMSG_EX<VARMSG_T04, MSG_AW_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN> \
    MSG_ONEtoN_BROADCASTER_WORLD_INTERNAL;
//==================================================================================================

// AW_CONNECTION 카테고리
PACKET_AW_OPEN(AW_CONNECTION, AW_AGENTSERVER_INFO_SYN)
    DWORD       dwChannelID;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_SYN)
    DWORD       dwAuthUserID;                       //AuthServer에서 발급한 식별자
    DWORD       dwUserGuid;                         //DB에서 읽어온 GUID
    // set to default (+1) that was added by __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR       ptszAccountID[ID_MAX_LEN + 1];      // 계정 아이디
    char szClientIP[MAX_IP_STRING_LEN + 1];         //클라이언트 아이피
    BYTE        byGMGrade;                          //GM권한
    DWORD       dwPCRoomID;                         // PC방인지 아닌지(0이면 일반 유저)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_ACK)
    bool enabled; // CHANGES: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
PACKET_AW_CLOSE;

// NOTE: f110916.1L, does not use anymore after _NA002676_WORLD_SERVER_RENEWAL 
PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_NAK)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_SET_CHARNAME_SYN)
    DWORD           dwCharGuid;                         //캐릭터 guid
    char            szCharName[MAX_CHARNAME_LENGTH+1];
    GUILDGUID       m_GuildGuid;                        // 소속 길드 Guid(없으면 0)
    LEVELTYPE       m_CharLevel;

    BYTE            m_CharClass;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE            gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

    void Clear()
    {
        const size_t clear_size = static_cast<size_t>((uint8_t*)(this + 1) - (uint8_t*)(&m_dwKey));
        ZeroMemory(&m_dwKey, clear_size);
    };
PACKET_AW_CLOSE;

// unused
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_UNSET_CHARNAME_SYN)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_ENTER_ZONE_SYN)
    DWORD           dwZoneKey;
    BYTE            byZoneType;
    CODETYPE        MapCode;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LEAVE_ZONE_SYN)
    DWORD           dwZoneKey;
    BYTE            byZoneType;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOGOUT)
PACKET_AW_CLOSE;

// 유저 정보 복구 요청
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_RESTORE_SYN)
PACKET_AW_CLOSE;

// 유저 정보 목록
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_RESTORE_CMD)
    DWORD       dwAuthUserID;
    DWORD       dwUserGUID;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR       ptszAccountID[ID_MAX_LEN + 1];      // 계정 아이디
#else
    TCHAR       ptszAccountID[ID_MAX_LEN];          // 계정 아이디
#endif
    char        pszClientIP[IP_MAX_LEN];            //< ip는 char형
    CHARGUID    CharGuid;
    TCHAR       tszCharName[MAX_CHARNAME_LENGTH+1];
    DWORD       dwZoneKey;
    BYTE        byZoneType;
    MAPCODE     map_code; // NOTE: f110929.1L, _NA002676_WORLD_SERVER_RENEWAL
    WORD        wPartyKey;                          // 파티 가입되어 있는 경우 파티 키 전송
    DWORD       dwPCRoomID;                         // 예약값...인데 실제로는 PC방 여부(0이면 일반 유저)
    BYTE        byGMGrade;                          // GM Grade
    GUILDGUID   m_GuildGuid;                            // 소속 길드 Guid(없으면 0)
    LEVELTYPE   m_CharLevel;
    BYTE        m_CharClass;

    void Clear()
    {
        const size_t kClearSize = (char*)(this + 1) - (char*)(&dwAuthUserID);
        ZeroMemory(&dwAuthUserID, kClearSize);
    };
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_AW_CLOSE;

// 유저 레벨업(길드/친구 정보 갱신 위해 필요)
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LEVELUP_CMD)
    CHARGUID    m_CharGuid;     // 레벨업한 놈
    LEVELTYPE   m_Level;        // 현재 레벨
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_SYN)
    eINTERCEPTION_TYPE  m_byInterceptionCode;
    BOOL                m_bFlag;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_ACK)
    eINTERCEPTION_TYPE  m_byInterceptionCode;
    BOOL                m_bFlag;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_NAK)
PACKET_AW_CLOSE;

// __KR_001355_20090608_GROUPINGSYSTEM_GW 
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_ADDEXP_CMD)
    CHARGUID    m_CharGuid;            // 캐릭터 Guid
    EXPTYPE     m_Amount;              // 경험치량
    TCHAR       m_szCharName[MAX_CHARNAME_LENGTH+1];//Low memeber name
PACKET_AW_CLOSE;
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_ADDMONEY_CMD)
    CHARGUID    m_CharGuid;            // 캐릭터 Guid
    MONEY       m_Amount;              // 보너스 하임
    TCHAR       m_szCharName[MAX_CHARNAME_LENGTH+1];//Hight memeber name
PACKET_AW_CLOSE;
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_EXP_ACQUISITION_CMD)
    CHARGUID    m_CharGuid;            // 캐릭터 ID
    BYTE        m_byAcquisitionType;   // 획득 사유 (eHEIM_ACQUISITION_TYPE)
    EXPTYPE     m_Amount;              // 경험치량
PACKET_AW_CLOSE;

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_HEIM_ACQUISITION_NTF)
    CHARGUID m_CharGuid;            // 캐릭터 ID
    BYTE     m_byAcquisitionType;   // 획득 사유 (eHEIM_ACQUISITION_TYPE)
    MONEY    m_Amount;              // 하임 금액
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_HEIM_CONSUMPTION_NTF)
    CHARGUID m_CharGuid;            // 캐릭터 ID
    BYTE     m_byConsumptionType;   // 소비 사유 (eHEIM_CONSUMPTION_TYPE)
    MONEY    m_Amount;              // 하임 금액
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    int      m_LottoAccumRatio;     // 하임 행운 적립율
PACKET_AW_CLOSE;

// __KR_001355_20090608_GROUPINGSYSTEM_GW
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_SET_MEMBERINFO_CMD)
    CHARGUID m_CharGuid;
    eGROUP_STAT m_HighGroupinfo;
    eGROUP_STAT m_LowGroupInfo;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_START_CMD)
    CHARGUID m_CharGuid;
PACKET_AW_CLOSE;

// declared by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { AW_USER_LOTTO_~ }

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_REWARD_SYN)
    CHARGUID m_CharGuid;
    MONEY    m_Prize;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_REWARD_ACK)
    enum eRESULT_CODE {
        RESULT_OK = 0,
        ERROR_CHAR_NOT_ONLINE
    };
    INT32    m_iResult;
    CHARGUID m_CharGuid;
    MONEY    m_Prize;
PACKET_AW_CLOSE;

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
// 하임 로또 복권용지 직접 구입 요청
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_SYN)
    CHARGUID iCharGuid;                 // 캐릭터 ID
    WORD wDrawNum;                      // 회차
    BYTE bAuto;                         // 자동 여부
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
    INT64 i64TicketPrice;               // 복권용지 가격
PACKET_AW_CLOSE;

// 하임 로또 복권용지 직접 구입 응답
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_ACK)
    enum eRC_LOTTO_TICKET_PURCHASE
    {
        RESULT_OK = 0,
        ERROR_NOT_ENOUGH_MONEY = 1,
        ERROR_CHAR_NOT_ONLINE = 2
    };
    INT32 iResult;                      // 결과 코드
    CHARGUID iCharGuid;                 // 캐릭터 ID
    WORD wDrawNum;                      // 회차
    BYTE bAuto;                         // 자동 여부
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
PACKET_AW_CLOSE;
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

//#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_PCBANG_BILLING_NTF)
    DWORD    m_PcRoomId;    // ReservedValue
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    DWORD   premiumservice_;
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
PACKET_AW_CLOSE;
//#endif

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_DOMINATION_RETURNHIEM_CMD)
    CHARGUID            m_CharGuid;             // 캐릭터 Guid
    eSYSTEM_MEMO_TYPE   m_Type;                 // 타입
    ULONGLONG           m_TenderCost;           // 입찰 하임 | 왕의 증표 갯수
PACKET_AW_CLOSE;

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_SYSTEMMEMO_SEND_CMD)
    DWORD system_memo_list_num_;//시스템 메모 갯수
    BasicSystemMemoInfo system_memo_info_[MAX_SYSTEM_MEMO_LIST];//시스템 메모 정보 리스트

    uint16_t GetSize() const
    {
        assert(system_memo_list_num_ <= _countof(system_memo_info_));
        const size_t kMinSize = sizeof(*this) - sizeof(system_memo_info_);
        return static_cast<uint16_t>(kMinSize + //고정 크기
               (sizeof(system_memo_info_[0]) * system_memo_list_num_)); //가변 크기
    }
PACKET_AW_CLOSE;

// declared by _NA002676_WORLD_SERVER_RENEWAL
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STADING_POSITION_CMD)
    static const DWORD kMaxNumberOfNodes = MSG_AG_PARTY_NAVI_INFO_BRD::MAX_OBJECT_ARRAY_SIZE;
    struct Node {
        DWORD user_guid;
        int16_t pos_x;
        int16_t pos_y;
    };
    uint8_t paddings_0_;
    uint8_t number_of_nodes;
    Node nodes[kMaxNumberOfNodes];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(nodes);
        return static_cast<uint16_t>(kMinSize + sizeof(nodes[0]) * number_of_nodes);
    };
PACKET_AW_CLOSE;
//==========================================================================================

// AW_PARTY_PROTOCOL 카테고리

// 파티 결성
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_MAKE_SYN)
    WORD        wPartyKey;                      // 파티 키
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // 파티장
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 파티 인원
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// 파티 가입
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_JOIN_MEMBER_SYN)
    WORD        wPartyKey;                      // 파티 키
    CHARGUID    m_CharGUID;                     // 가입할 캐릭터 GUID 

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // 파티장
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 파티 인원
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// 파티 탈퇴
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_WITHDRAW_MEMBER_SYN)
    WORD        wPartyKey;                      // 파티 키
    CHARGUID    m_CharGUID;                     // 나갈 캐릭터 GUID  

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // 파티장
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 파티 인원
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// 파티 해산
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_DESTROY_SYN)
    WORD        wPartyKey;                      // 파티 키
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // 파티장
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 파티 인원
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
// 룸키 설정
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_ROOM_KEY_SYN)
    WORD        party_key_;                     // 파티 키
    WORD        room_key_;                      // 룸 키
PACKET_AW_CLOSE;
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// 파티장 변경
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_CHANGE_MASTER_SYN)
    PartyKeyType party_key_; // 파티 키
    CHARGUID new_party_master_id_; // 새로운 파티장
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 파티 인원
    
    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

// 파티 모집에 속한 파티 참가 요청
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_FINDING_JOIN_SYN)
    PartyFindingKeyType join_wanted_party_finding_key_; // 참가 신청한 파티 모집 키   
    CHARGUID join_wanted_party_master_id_; // 참가 신청한 파티의 파티장
    CHARGUID join_wanted_user_id_; // 참가 신청한 유저
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_member_; // 최대 모집 인원
    
    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

// 파티 모집에 속한 파티 참가 요청에 대한 처리 결과 응답
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_FINDING_JOIN_RESULT_ACK)
    CHARGUID join_wanted_user_id_; // 참가 신청한 유저
    RC::PartyFindingResultUtil::ValueType join_result_; // 참가 신청 결과
    PartyFindingKeyType join_wanted_party_finding_key_; // 참가 신청한 파티 모집 키
    PartyKeyType join_wanted_party_key_; // 참가 신청한 파티 키
    PartyFindingNumOfMemberConst::ValueType num_of_joined_people_; // 참가 신청한 파티에 참가한 인원 수

    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

// AW_GM_PROTOCOL 카테고리
PACKET_AW_OPEN(AW_GM, AW_GM_WHISPER_SET_CMD)
    BOOL                m_bOn;              // FALSE: 귓말 끔 TRUE: 귓말 켬
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_GM, AW_GM_USERCHAT_BLOCK_CMD)
    CHARGUID    m_CharGuid;         // 해당 사용자 CharGuid
    BOOL        m_bBlock;           // FALSE: 채팅 허용 TRUE: 채팅 금지
    DWORD       m_dwBlockTime;      // 채팅 금지일 경우 제한시간(분 단위)
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_GM, AW_GM_NOTICE_CMD)
    eNOTICE_TYPE    m_eType;
    WORD            m_wLen;
    TCHAR           m_tszNoticeMsg[MAX_NOTICE_LEN + 1];

    uint16 GetSize() const
    {   // CHANGES: f101210.2L
        const size_t kMinSize = sizeof(*this) - sizeof(m_tszNoticeMsg);
        return static_cast<uint16_t>(kMinSize + m_wLen * sizeof(m_tszNoticeMsg[0]));
    }
PACKET_AW_CLOSE;

// declared by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { AW_GM_LOTTO_~ }
PACKET_AW_OPEN(AW_GM, AW_GM_LOTTO_NUM_CMD)
    DWORD dwUserKey;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_GM, AW_GM_LOTTO_SETTLE_CMD)
    DWORD dwUserKey;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_GM, AW_GM_LOTTO_FUND_CMD)
    DWORD user_key;
    INT64 amount_delta;
PACKET_AW_CLOSE;

//----------------------------------------------------------------------------------
// AW_FRIENDINVITATE 프로토콜 시작
//----------------------------------------------------------------------------------
//W->A 초대한 캐릭터 목록 (가변)
PACKET_AW_OPEN(AW_FRIENDINVITATE, AW_FRIENDINVITATE_INVITATELIST_CMD) 
    INVITATEFRIENDLIST_INFO m_InvitateFriendListInfo;   

    WORD GetSize() const
    {   // (CHANGES) (f100624.2L)
        const WORD kMinSize = sizeof(*this) - sizeof(m_InvitateFriendListInfo);
        return static_cast<WORD>(kMinSize + m_InvitateFriendListInfo.GetSize());
        //return (sizeof(MSG_AW_FRIENDINVITATE_INVITATELIST_CMD) - ((MAX_FRIEND_STAT_NUM - m_InvitateFriendListInfo.m_byListCount) * sizeof(DWORD)));
    }
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_FRIENDINVITATE, AW_FRIEND_INVITATIONR_INSERT_CMD) 
    STRUCT_FRIEND_INVITATE_INFO InvitateInfo;
PACKET_AW_CLOSE;
//----------------------------------------------------------------------------------
// AW_FRIENDINVITATE 프로토콜 끝
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// AW_CHAO 관련 프로토콜
//----------------------------------------------------------------------------------

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_SYN)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
PACKET_AW_CLOSE;


// 리벤지 정보 전달(가변)
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_ACK)
    DWORD               m_dwCharGuid;                               // 요청한 캐릭터의 CharGuid
    BYTE                m_byRevengePoint;                           // 남은 Revenge Point
    BYTE                m_byRevengeStatNum;
    STRUCT_REVENGE_INFO m_sRevengeUserInfo[MAX_REVENGE_STAT_NUM];    // 등록된 캐릭터 목록

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sRevengeUserInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sRevengeUserInfo[0]) * m_byRevengeStatNum);
    };
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_NAK)
    DWORD               m_dwCharGuid;           // 요청한 캐릭터의 CharGuid
    BYTE                m_byErrorCode;          // 요청 실패한 이유(0: 알 수 없는 에러 1: 대상이 WorldServer에 연결되어 있지 않음)
PACKET_AW_CLOSE;

// 클라이언트가 리벤지 리스트 삭제 요청했음(W->A)
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_CMD)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    DWORD               m_dwRevengeCharGuid;    // 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AW_CLOSE;

// A->로 PK 성공했으니 리벤지 리스트 삭제 요청
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_SYN)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    DWORD               m_dwRevengeCharGuid;    // 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_ACK)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    DWORD               m_dwRevengeCharGuid;    // 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_NAK)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    BYTE                m_byErrorCode;          // eREVENGE_RESULT 참고 
PACKET_AW_CLOSE;

// 리벤지 대상 추가 요청
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_SYN)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    TCHAR               m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];   // 리벤지 리스트에 추가할 대상 Name
                                                                    // 로그아웃 중이라도 추가가 되어야 하므로 m_dwRevengeCharGuid 대신 이름을 사용한다.
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_ACK)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    DWORD               m_dwRevengeCharGuid;    // 추가할 리벤지(나쁜친구) CharGuid
    TCHAR               m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];   // 리벤지 리스트에 추가한 대상 Name
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_NAK)
    DWORD               m_dwCharGuid;           // 요청하는 본인 CharGuid
    BYTE                m_byErrorCode;          // eREVENGE_RESULT 참고 
PACKET_AW_CLOSE;

// 스페셜 아이템 소유 변경
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD)
    enum { MAX_SPECIAL_ITEM_COUNT = 30 };
    DWORD                   m_dwCharGuid;
    BYTE                    m_Count;
    SPECIAL_ITEM_OWN_INFO   m_ItemInfo[MAX_SPECIAL_ITEM_COUNT];

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ItemInfo[0]) * m_Count);
    };
PACKET_AW_CLOSE;

// 스페셜 아이템 요청(W -> A)
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_SYN)
    DWORD                   m_dwUserGuid;
PACKET_AW_CLOSE;

// 스페셜 아이템 클리어 명령(A->W)
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_SPECIAL_ITEM_CLEAR_CMD)
    DWORD                   m_dwCharGuid;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_SYN)
    DWORD                   m_dwCharGuid;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_ACK)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_NAK)
    BYTE                    m_byErrorCode;
PACKET_AW_CLOSE;

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// A->W 하임행운 캐시 응모권 사용 요청
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN)
    CHARGUID char_guid;     // 캐릭터 ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AW_CLOSE;

// W->A 하임행운 캐시 응모권 사용 응답
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK)
    WORD     result_code;   // 결과 코드 : eITEM_RESULT
    CHARGUID char_guid;     // 캐릭터 ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AW_CLOSE;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

#ifndef __SSQ_MSG_HDR__
#define __SSQ_MSG_HDR__
// <Prototype of WAR_MSG_HDR>
//  전체 메시지의 길이가 담겨진다. (variable stream)
struct WAR_MSG_HDR
{
    typedef ulong MSG_SIZE_T; // expanded by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    MSG_SIZE_T size;
};

//  <Raw Header of Each Message>
//  -   각 메시지의 타입들이 기재된다.
//  -   1Byte, 하지만 raw stream인 WAR_TAG_V의 이것을 직접 사용하는 경우는 없어야 한다.
//      (이것 common routine에서의 offset연산 편의성이 목적이다.)
//      모든 메시지들은 실질적으로 WAR_TAG1_<id, type>을 상속해서 구현되게 되어 있다.
//  <목적>
//  -   Server/Client 각각 pointer연산을 수행하면서 버그를 유발할 수 있다.
//      하나의 로직으로 serializing input/output이 가능해지도록 하는 것이 목표다.
typedef struct WAR_TAG_V
{
    BYTE                _TAG;
} *WAR_TAG_P;
#endif

PACKET_AW_OPEN(AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD)
    WAR_MSG_HDR m_Header[1]; // Stream Header, next pointer is data stream
    uint16_t GetSize() const
    { return static_cast<uint16_t>(sizeof(*this) + m_Header->size); }
PACKET_AW_CLOSE;


PACKET_AW_OPEN(AW_CONNECTION, AW_ABUSING_USER_VERIFY_QUERY_CMD)
    CHARGUID m_CharGuid;            // 요청하는 본인 CharGuid
    eABUSE_SUB_TYPE m_eAbuseSubType;
PACKET_AW_CLOSE;

// USERCOUNT_SYN은 월드 서버 자체에서 집계하기로 수정하고, USERMONEY_SYN은 GM으로 옮겼음
/*
// AW_USER_OPERATION 카테고리
struct MSG_AW_USERCOUNT_SYN : public MSG_BASE_FORWARD
{
    MSG_AW_USERCOUNT_SYN() { m_byCategory = AW_OPERATION; m_byProtocol = AW_USERCOUNT_SYN; }
    DWORD           dwVillageUserCount;         // 마을존 개수
    DWORD           dwHuntingLobbyZoneCount;    // Hunting로비방 개수
    DWORD           dwHuntingLobbyUserCount;    // Hunting로비방 총 동접
    DWORD           dwMissionLobbyZoneCount;    // Mission로비방 개수
    DWORD           dwMissionLobbyUserCount;    // Mission로비방 총 동접
    DWORD           dwPVPLobbyZoneCount;        // PVP로비방 개수
    DWORD           dwPVPLobbyUserCount;        // PVP로비방 총 동접
    DWORD           dwHuntingZoneCount;         // 헌팅방 개수
    DWORD           dwHuntingUserCount;         // 헌팅방 총 동접
    DWORD           dwMissionZoneCount;         // 미션방 개수
    DWORD           dwMissionUserCount;         // 미션방 총 동접
    DWORD           dwPVPZoneCount;             // PVP존 개수
    DWORD           dwPVPUserCount;             // PVP존 총 동접
    DWORD           GetTotalUserNum()
    {
        return      dwVillageUserCount
                    +dwHuntingLobbyUserCount
                    +dwMissionLobbyUserCount
                    +dwPVPLobbyUserCount
                    +dwHuntingUserCount
                    +dwMissionUserCount
                    +dwPVPUserCount;
    }
};

struct MSG_AW_USERMONEY_SYN : public MSG_BASE_FORWARD
{
    MSG_AW_USERMONEY_SYN() { m_byCategory = AW_OPERATION; m_byProtocol = AW_USERMONEY_SYN; }

    DWORD           dwTotalGetMoney;            // 일정 시간 동안 생겨난 돈 총계
    DWORD           dwTotalLoseMoney;           // 일정 시간 동안 없어진 돈 총계
    DWORD           dwTotalUserMoney;           // 현재 동접 유저들이 갖고 있는 평균 금액
};
*/

//==================================================================================================
#pragma pack(pop)
//==================================================================================================


#endif //SERVERCOMMON_PACKET_STRUCT_AW_H