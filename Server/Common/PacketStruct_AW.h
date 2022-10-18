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

// AW_CONNECTION ī�װ�
PACKET_AW_OPEN(AW_CONNECTION, AW_AGENTSERVER_INFO_SYN)
    DWORD       dwChannelID;
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_SYN)
    DWORD       dwAuthUserID;                       //AuthServer���� �߱��� �ĺ���
    DWORD       dwUserGuid;                         //DB���� �о�� GUID
    // set to default (+1) that was added by __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR       ptszAccountID[ID_MAX_LEN + 1];      // ���� ���̵�
    char szClientIP[MAX_IP_STRING_LEN + 1];         //Ŭ���̾�Ʈ ������
    BYTE        byGMGrade;                          //GM����
    DWORD       dwPCRoomID;                         // PC������ �ƴ���(0�̸� �Ϲ� ����)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_ACK)
    bool enabled; // CHANGES: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
PACKET_AW_CLOSE;

// NOTE: f110916.1L, does not use anymore after _NA002676_WORLD_SERVER_RENEWAL 
PACKET_AW_OPEN(AW_CONNECTION, AW_PREPARE_NEW_USER_NAK)
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_SET_CHARNAME_SYN)
    DWORD           dwCharGuid;                         //ĳ���� guid
    char            szCharName[MAX_CHARNAME_LENGTH+1];
    GUILDGUID       m_GuildGuid;                        // �Ҽ� ��� Guid(������ 0)
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

// ���� ���� ���� ��û
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_RESTORE_SYN)
PACKET_AW_CLOSE;

// ���� ���� ���
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_RESTORE_CMD)
    DWORD       dwAuthUserID;
    DWORD       dwUserGUID;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR       ptszAccountID[ID_MAX_LEN + 1];      // ���� ���̵�
#else
    TCHAR       ptszAccountID[ID_MAX_LEN];          // ���� ���̵�
#endif
    char        pszClientIP[IP_MAX_LEN];            //< ip�� char��
    CHARGUID    CharGuid;
    TCHAR       tszCharName[MAX_CHARNAME_LENGTH+1];
    DWORD       dwZoneKey;
    BYTE        byZoneType;
    MAPCODE     map_code; // NOTE: f110929.1L, _NA002676_WORLD_SERVER_RENEWAL
    WORD        wPartyKey;                          // ��Ƽ ���ԵǾ� �ִ� ��� ��Ƽ Ű ����
    DWORD       dwPCRoomID;                         // ���ప...�ε� �����δ� PC�� ����(0�̸� �Ϲ� ����)
    BYTE        byGMGrade;                          // GM Grade
    GUILDGUID   m_GuildGuid;                            // �Ҽ� ��� Guid(������ 0)
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

// ���� ������(���/ģ�� ���� ���� ���� �ʿ�)
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LEVELUP_CMD)
    CHARGUID    m_CharGuid;     // �������� ��
    LEVELTYPE   m_Level;        // ���� ����
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
    CHARGUID    m_CharGuid;            // ĳ���� Guid
    EXPTYPE     m_Amount;              // ����ġ��
    TCHAR       m_szCharName[MAX_CHARNAME_LENGTH+1];//Low memeber name
PACKET_AW_CLOSE;
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_ADDMONEY_CMD)
    CHARGUID    m_CharGuid;            // ĳ���� Guid
    MONEY       m_Amount;              // ���ʽ� ����
    TCHAR       m_szCharName[MAX_CHARNAME_LENGTH+1];//Hight memeber name
PACKET_AW_CLOSE;
PACKET_AW_OPEN(AW_USER_STATUS, AW_STATUS_GROUP_EXP_ACQUISITION_CMD)
    CHARGUID    m_CharGuid;            // ĳ���� ID
    BYTE        m_byAcquisitionType;   // ȹ�� ���� (eHEIM_ACQUISITION_TYPE)
    EXPTYPE     m_Amount;              // ����ġ��
PACKET_AW_CLOSE;

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_HEIM_ACQUISITION_NTF)
    CHARGUID m_CharGuid;            // ĳ���� ID
    BYTE     m_byAcquisitionType;   // ȹ�� ���� (eHEIM_ACQUISITION_TYPE)
    MONEY    m_Amount;              // ���� �ݾ�
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_STATUS_HEIM_CONSUMPTION_NTF)
    CHARGUID m_CharGuid;            // ĳ���� ID
    BYTE     m_byConsumptionType;   // �Һ� ���� (eHEIM_CONSUMPTION_TYPE)
    MONEY    m_Amount;              // ���� �ݾ�
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    int      m_LottoAccumRatio;     // ���� ��� ������
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
// ���� �ζ� ���ǿ��� ���� ���� ��û
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_SYN)
    CHARGUID iCharGuid;                 // ĳ���� ID
    WORD wDrawNum;                      // ȸ��
    BYTE bAuto;                         // �ڵ� ����
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
    INT64 i64TicketPrice;               // ���ǿ��� ����
PACKET_AW_CLOSE;

// ���� �ζ� ���ǿ��� ���� ���� ����
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_ACK)
    enum eRC_LOTTO_TICKET_PURCHASE
    {
        RESULT_OK = 0,
        ERROR_NOT_ENOUGH_MONEY = 1,
        ERROR_CHAR_NOT_ONLINE = 2
    };
    INT32 iResult;                      // ��� �ڵ�
    CHARGUID iCharGuid;                 // ĳ���� ID
    WORD wDrawNum;                      // ȸ��
    BYTE bAuto;                         // �ڵ� ����
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
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
    CHARGUID            m_CharGuid;             // ĳ���� Guid
    eSYSTEM_MEMO_TYPE   m_Type;                 // Ÿ��
    ULONGLONG           m_TenderCost;           // ���� ���� | ���� ��ǥ ����
PACKET_AW_CLOSE;

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_AW_OPEN(AW_USER_STATUS, AW_USER_SYSTEMMEMO_SEND_CMD)
    DWORD system_memo_list_num_;//�ý��� �޸� ����
    BasicSystemMemoInfo system_memo_info_[MAX_SYSTEM_MEMO_LIST];//�ý��� �޸� ���� ����Ʈ

    uint16_t GetSize() const
    {
        assert(system_memo_list_num_ <= _countof(system_memo_info_));
        const size_t kMinSize = sizeof(*this) - sizeof(system_memo_info_);
        return static_cast<uint16_t>(kMinSize + //���� ũ��
               (sizeof(system_memo_info_[0]) * system_memo_list_num_)); //���� ũ��
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

// AW_PARTY_PROTOCOL ī�װ�

// ��Ƽ �Ἲ
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_MAKE_SYN)
    WORD        wPartyKey;                      // ��Ƽ Ű
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // ��Ƽ��
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ��Ƽ �ο�
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// ��Ƽ ����
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_JOIN_MEMBER_SYN)
    WORD        wPartyKey;                      // ��Ƽ Ű
    CHARGUID    m_CharGUID;                     // ������ ĳ���� GUID 

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // ��Ƽ��
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ��Ƽ �ο�
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// ��Ƽ Ż��
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_WITHDRAW_MEMBER_SYN)
    WORD        wPartyKey;                      // ��Ƽ Ű
    CHARGUID    m_CharGUID;                     // ���� ĳ���� GUID  

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // ��Ƽ��
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ��Ƽ �ο�
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

// ��Ƽ �ػ�
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_DESTROY_SYN)
    WORD        wPartyKey;                      // ��Ƽ Ű
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    CHARGUID party_master_id_; // ��Ƽ��
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ��Ƽ �ο�
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
PACKET_AW_CLOSE;

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
// ��Ű ����
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_ROOM_KEY_SYN)
    WORD        party_key_;                     // ��Ƽ Ű
    WORD        room_key_;                      // �� Ű
PACKET_AW_CLOSE;
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ�� ����
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_CHANGE_MASTER_SYN)
    PartyKeyType party_key_; // ��Ƽ Ű
    CHARGUID new_party_master_id_; // ���ο� ��Ƽ��
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ��Ƽ �ο�
    
    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

// ��Ƽ ������ ���� ��Ƽ ���� ��û
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_FINDING_JOIN_SYN)
    PartyFindingKeyType join_wanted_party_finding_key_; // ���� ��û�� ��Ƽ ���� Ű   
    CHARGUID join_wanted_party_master_id_; // ���� ��û�� ��Ƽ�� ��Ƽ��
    CHARGUID join_wanted_user_id_; // ���� ��û�� ����
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_member_; // �ִ� ���� �ο�
    
    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

// ��Ƽ ������ ���� ��Ƽ ���� ��û�� ���� ó�� ��� ����
PACKET_AW_OPEN(AW_PARTY, AW_PARTY_FINDING_JOIN_RESULT_ACK)
    CHARGUID join_wanted_user_id_; // ���� ��û�� ����
    RC::PartyFindingResultUtil::ValueType join_result_; // ���� ��û ���
    PartyFindingKeyType join_wanted_party_finding_key_; // ���� ��û�� ��Ƽ ���� Ű
    PartyKeyType join_wanted_party_key_; // ���� ��û�� ��Ƽ Ű
    PartyFindingNumOfMemberConst::ValueType num_of_joined_people_; // ���� ��û�� ��Ƽ�� ������ �ο� ��

    inline WORD GetSize() const 
    { 
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AW_CLOSE;

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

// AW_GM_PROTOCOL ī�װ�
PACKET_AW_OPEN(AW_GM, AW_GM_WHISPER_SET_CMD)
    BOOL                m_bOn;              // FALSE: �Ӹ� �� TRUE: �Ӹ� ��
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_GM, AW_GM_USERCHAT_BLOCK_CMD)
    CHARGUID    m_CharGuid;         // �ش� ����� CharGuid
    BOOL        m_bBlock;           // FALSE: ä�� ��� TRUE: ä�� ����
    DWORD       m_dwBlockTime;      // ä�� ������ ��� ���ѽð�(�� ����)
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
// AW_FRIENDINVITATE �������� ����
//----------------------------------------------------------------------------------
//W->A �ʴ��� ĳ���� ��� (����)
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
// AW_FRIENDINVITATE �������� ��
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// AW_CHAO ���� ��������
//----------------------------------------------------------------------------------

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_SYN)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
PACKET_AW_CLOSE;


// ������ ���� ����(����)
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_ACK)
    DWORD               m_dwCharGuid;                               // ��û�� ĳ������ CharGuid
    BYTE                m_byRevengePoint;                           // ���� Revenge Point
    BYTE                m_byRevengeStatNum;
    STRUCT_REVENGE_INFO m_sRevengeUserInfo[MAX_REVENGE_STAT_NUM];    // ��ϵ� ĳ���� ���

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sRevengeUserInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sRevengeUserInfo[0]) * m_byRevengeStatNum);
    };
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_NAK)
    DWORD               m_dwCharGuid;           // ��û�� ĳ������ CharGuid
    BYTE                m_byErrorCode;          // ��û ������ ����(0: �� �� ���� ���� 1: ����� WorldServer�� ����Ǿ� ���� ����)
PACKET_AW_CLOSE;

// Ŭ���̾�Ʈ�� ������ ����Ʈ ���� ��û����(W->A)
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_CMD)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    DWORD               m_dwRevengeCharGuid;    // ������ ������(����ģ��) CharGuid
PACKET_AW_CLOSE;

// A->�� PK ���������� ������ ����Ʈ ���� ��û
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_SYN)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    DWORD               m_dwRevengeCharGuid;    // ������ ������(����ģ��) CharGuid
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_ACK)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    DWORD               m_dwRevengeCharGuid;    // ������ ������(����ģ��) CharGuid
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_NAK)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    BYTE                m_byErrorCode;          // eREVENGE_RESULT ���� 
PACKET_AW_CLOSE;

// ������ ��� �߰� ��û
PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_SYN)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    TCHAR               m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];   // ������ ����Ʈ�� �߰��� ��� Name
                                                                    // �α׾ƿ� ���̶� �߰��� �Ǿ�� �ϹǷ� m_dwRevengeCharGuid ��� �̸��� ����Ѵ�.
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_ACK)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    DWORD               m_dwRevengeCharGuid;    // �߰��� ������(����ģ��) CharGuid
    TCHAR               m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];   // ������ ����Ʈ�� �߰��� ��� Name
PACKET_AW_CLOSE;

PACKET_AW_OPEN(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_NAK)
    DWORD               m_dwCharGuid;           // ��û�ϴ� ���� CharGuid
    BYTE                m_byErrorCode;          // eREVENGE_RESULT ���� 
PACKET_AW_CLOSE;

// ����� ������ ���� ����
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

// ����� ������ ��û(W -> A)
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_SYN)
    DWORD                   m_dwUserGuid;
PACKET_AW_CLOSE;

// ����� ������ Ŭ���� ���(A->W)
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
// A->W ������� ĳ�� ����� ��� ��û
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN)
    CHARGUID char_guid;     // ĳ���� ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AW_CLOSE;

// W->A ������� ĳ�� ����� ��� ����
PACKET_AW_OPEN(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK)
    WORD     result_code;   // ��� �ڵ� : eITEM_RESULT
    CHARGUID char_guid;     // ĳ���� ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AW_CLOSE;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

#ifndef __SSQ_MSG_HDR__
#define __SSQ_MSG_HDR__
// <Prototype of WAR_MSG_HDR>
//  ��ü �޽����� ���̰� �������. (variable stream)
struct WAR_MSG_HDR
{
    typedef ulong MSG_SIZE_T; // expanded by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    MSG_SIZE_T size;
};

//  <Raw Header of Each Message>
//  -   �� �޽����� Ÿ�Ե��� ����ȴ�.
//  -   1Byte, ������ raw stream�� WAR_TAG_V�� �̰��� ���� ����ϴ� ���� ����� �Ѵ�.
//      (�̰� common routine������ offset���� ���Ǽ��� �����̴�.)
//      ��� �޽������� ���������� WAR_TAG1_<id, type>�� ����ؼ� �����ǰ� �Ǿ� �ִ�.
//  <����>
//  -   Server/Client ���� pointer������ �����ϸ鼭 ���׸� ������ �� �ִ�.
//      �ϳ��� �������� serializing input/output�� ������������ �ϴ� ���� ��ǥ��.
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
    CHARGUID m_CharGuid;            // ��û�ϴ� ���� CharGuid
    eABUSE_SUB_TYPE m_eAbuseSubType;
PACKET_AW_CLOSE;

// USERCOUNT_SYN�� ���� ���� ��ü���� �����ϱ�� �����ϰ�, USERMONEY_SYN�� GM���� �Ű���
/*
// AW_USER_OPERATION ī�װ�
struct MSG_AW_USERCOUNT_SYN : public MSG_BASE_FORWARD
{
    MSG_AW_USERCOUNT_SYN() { m_byCategory = AW_OPERATION; m_byProtocol = AW_USERCOUNT_SYN; }
    DWORD           dwVillageUserCount;         // ������ ����
    DWORD           dwHuntingLobbyZoneCount;    // Hunting�κ�� ����
    DWORD           dwHuntingLobbyUserCount;    // Hunting�κ�� �� ����
    DWORD           dwMissionLobbyZoneCount;    // Mission�κ�� ����
    DWORD           dwMissionLobbyUserCount;    // Mission�κ�� �� ����
    DWORD           dwPVPLobbyZoneCount;        // PVP�κ�� ����
    DWORD           dwPVPLobbyUserCount;        // PVP�κ�� �� ����
    DWORD           dwHuntingZoneCount;         // ���ù� ����
    DWORD           dwHuntingUserCount;         // ���ù� �� ����
    DWORD           dwMissionZoneCount;         // �̼ǹ� ����
    DWORD           dwMissionUserCount;         // �̼ǹ� �� ����
    DWORD           dwPVPZoneCount;             // PVP�� ����
    DWORD           dwPVPUserCount;             // PVP�� �� ����
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

    DWORD           dwTotalGetMoney;            // ���� �ð� ���� ���ܳ� �� �Ѱ�
    DWORD           dwTotalLoseMoney;           // ���� �ð� ���� ������ �� �Ѱ�
    DWORD           dwTotalUserMoney;           // ���� ���� �������� ���� �ִ� ��� �ݾ�
};
*/

//==================================================================================================
#pragma pack(pop)
//==================================================================================================


#endif //SERVERCOMMON_PACKET_STRUCT_AW_H