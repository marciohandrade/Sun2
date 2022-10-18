#ifndef __PACKETSTRUCT_AD_H__
#define __PACKETSTRUCT_AD_H__

#include <Protocol_AD.h>
#include <PacketStruct.h>
#include <ServerStruct.h>
#include <ArchiveOfStream.h>


#pragma pack(push,1)




#define PACKET_AD_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_AD_CLOSE		};

// _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
PACKET_AD_OPEN(AD_CONNECTION, AD_CONNECTION_DBCONNECT_UPDATE_CMD)
    int db_connector_index_;
PACKET_AD_CLOSE;

// NOTE: f110622.2L
PACKET_AD_OPEN(AD_CONNECTION, AD_CONNECTION_HE_IS_HERE_CMD)
    uint8_t channel_id_;
PACKET_AD_CLOSE;

PACKET_AD_OPEN( AD_CONNECTION, AD_CONNECTION_DISCONNECT_CMD )
	GUILDGUID				m_GuildGuid;
	CHARGUID				m_CharGuid;
	BYTE					m_ChannelID;
PACKET_AD_CLOSE


PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_SYN )
	USERGUID				m_UserGuid;
	bool					m_bIsPCRoom;
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	time_t					m_LoginTime;
	BOOL					m_bFatigueUser;
#endif
	bool					m_bBackToSelect;		// ������ �ٽ� ����ȭ������ ���ƿ��� ������ ����
	BYTE					m_ChannelID;			// ä�� ID
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN              gamechu_usn_;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM    
    BETAKEY                 betakey_;
#endif
PACKET_AD_CLOSE


PACKET_AD_OPEN(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_ACK)
    BYTE m_byCount;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE char_slot_size;
#endif
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    BYTE m_ReNameStatus[MAX_CHARACTER_LIST_NUM]; //ĳ���͸� ���� ���� �÷��� ���̴� ĳ���� ������ ���� ���� ���� �Ҵ��Ѵ�.
#endif
    BYTE m_pBuffer[sizeof(SERVER_CHARACTER_PART) * MAX_CHARACTER_LIST_NUM + sizeof(BYTE)];
    // <�ӽù��۷� ����� Ÿ��>
    //SERVER_CHARACTER_PART char_part_list[MAX_CHARACTER_LIST_NUM];
    // returns: used buffer size
    uint16_t ConvertForSend(
        SERVER_CHARACTER_PART char_part_list[MAX_CHARACTER_LIST_NUM]);
    void ConvertForRecv(
        SERVER_CHARACTER_PART char_part_list[MAX_CHARACTER_LIST_NUM]);
PACKET_AD_CLOSE;


PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_NAK )
	DWORD					m_byErrorCode;
PACKET_AD_CLOSE


PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CREATE_SYN )
	DWORD				m_UserGuid;
	TCHAR				m_tszUserID[ID_MAX_LEN+1];
	BYTE				m_byClass;
	TCHAR				m_tszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE				m_byHeight;
	BYTE				m_byFace;
	BYTE				m_byHair;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // ����
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	WORD				honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool is_equipped_costume; // �ڽ�Ƭ Ȱ��ȭ ����
    POSTYPE costume_item_postion; // �ڽ�Ƭ �κ��丮 ��ġ
    SLOTCODE costume_item_code; // �ڽ�Ƭ ������ ��ȣ
#endif  // _NA_004035_20120227_COSTUME_ITEM

PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CREATE_ACK )
    BYTE m_pBuffer[sizeof(SERVER_CHARACTER_PART)];
    // <�ӽù��۷� ����� Ÿ��>
    //SERVER_CHARACTER_PART& rCharPart
    uint16_t ConvertForSend(SERVER_CHARACTER_PART& server_char_part);
    void ConvertForRecv(SERVER_CHARACTER_PART& server_char_part);
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CREATE_NAK )
	DWORD				m_dwErrorCode;
PACKET_AD_CLOSE


PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_DESTROY_SYN )
	CHARGUID			m_CharGuid;
PACKET_AD_CLOSE
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_DESTROY_ACK )
	CHARGUID			m_CharGuid;
PACKET_AD_CLOSE
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_DESTROY_NAK )
	DWORD				m_dwErrorCode;
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_RECOVER_SYN )
	CHARGUID			m_CharGuid;
PACKET_AD_CLOSE
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_RECOVER_ACK )
	CHARGUID			m_CharGuid;
PACKET_AD_CLOSE
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_RECOVER_NAK )
	DWORD				m_dwErrorCode;
PACKET_AD_CLOSE

#ifdef __NA_001044_ADD_CUSTOM_UI
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_SYN )
	CHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_ACK )
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_NAK )
	DWORD				m_dwErrorCode;
PACKET_AD_CLOSE
#endif

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_ADDIN_NTF )
	sSSN_ENCODED_STREAM		m_EncodedSSN;
PACKET_AD_CLOSE

//#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
PACKET_AD_OPEN(AD_CHARINFO, AD_CHARINFO_PCBANG_BILLING_NTF)
    DWORD    m_PcRoomId;    // ReservedValue
PACKET_AD_CLOSE
//#endif

// _KR_0_20091021_FIRENDINVITATION_EXTEND //�������� ���� = { AD_EVENT_INVITATE~ }

//��õ�� ģ���� ĳ���͸� �����Ҷ� ��õ�� ĳ������ ������ �����Ѵ�.
PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATIONR_INSERT_CMD )
    BYTE m_CharGuidCount;
    STRUCT_FRIEND_INVITATE_INFO InvitateInfo[MAX_FRIEND_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(InvitateInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(InvitateInfo[0]) * m_CharGuidCount);
    }
PACKET_AD_CLOSE


// ģ�� �ʴ� �̺�Ʈ�� ���� ���� ��� ��û
PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN )
    CHARGUID    CharGUID;
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK )
    CHARGUID    CharGUID;
    BYTE		ErrorCode;  //1: DB �ý��� ����, 2: DB Proxy ���� ���� �Ұ�
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK )
    CHARGUID                    CharGUID;
    WORD                        Count;
    InvitateFriendRewardInfo    RewardInfo[MAX_FRIENDINVITATE_REWARD_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(RewardInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(RewardInfo[0]) * Count);
    }
PACKET_AD_CLOSE
// END ģ�� �ʴ� �̺�Ʈ�� ���� ���� ��� ��û

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
PACKET_AD_OPEN( AD_CHARINFO, AD_CHARNAME_CHANGE_SYN )
    SLOTIDX slot_index_;
    CHARGUID    character_guid_;
    char    character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARNAME_CHANGE_ACK )
    SLOTIDX slot_index_;
    char    character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARNAME_CHANGE_NAK )
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
PACKET_AD_CLOSE
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE



#ifdef _NA_0_20110329_GAMEOPTION_SETTING
PACKET_AD_OPEN(AD_ETC, AD_ETC_GAMEOPTION_SELECT_SYN)
    USERGUID    user_guid_;
    CHARGUID    character_guid_;
    uint        option_type_;
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GAMEOPTION_SELECT_ACK)
    USERGUID    user_guid_;
    CHARGUID    character_guid_;
    GameOption  game_option_;
    //
    uint16_t GetSize() const
    {   // f110609.8L, fixed the invalid size calcuation spot.
        const size_t kMinSize = sizeof(*this) - sizeof(game_option_);
        return static_cast<uint16_t>(kMinSize + game_option_.GetSize());
    };
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GAMEOPTION_SELECT_NAK)
    uint error_code_;
    uint option_type_;
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_CMD)
    USERGUID    user_guid_;
    CHARGUID    character_guid_;
    GameOption  game_option_;
    //
    uint16_t GetSize() const
    {   // f110609.8L, fixed the invalid size calcuation spot.
        const size_t kMinSize = sizeof(*this) - sizeof(game_option_);
        return static_cast<uint16_t>(kMinSize + game_option_.GetSize());
    };
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_ACK)
    USERGUID user_guid;
    CHARGUID char_guid;
    uint option_type;
    bool is_success;

    void Clear()
    {
        user_guid = 0;
        char_guid = 0;
        option_type = 0;
        is_success = false;
    }
    uint16_t GetSize() const
    {
        return static_cast<uint16_t>(sizeof(*this));
    }
PACKET_AD_CLOSE
#endif //_NA_0_20110329_GAMEOPTION_SETTING

//!~ _NA004034_20120102_GUILD_POINT_SYSTEM
PACKET_AD_OPEN(AD_ETC, AD_ETC_GUILD_RANKINGINFO_SYN)
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GUILD_RANKINGINFO_ACK)
    INT guild_ranking_info_count_;
    GUILD_RANKING_INFO guild_ranking_info_[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GUILDINFO_SYN)
    GUILDGUID   guid_;
PACKET_AD_CLOSE

PACKET_AD_OPEN(AD_ETC, AD_ETC_GUILDINFO_ACK)
    GUILDGUID guid_;
    GAME_GUILD_INFO game_guild_info_;
    GUILD_INFO_EXTRA guild_info_extra_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eGUILD_JOIN_REQUEST request_state_;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void init()
    {
        guid_ = 0;
        memset(&game_guild_info_, 0, sizeof(game_guild_info_));
        memset(&guild_info_extra_, 0, sizeof(guild_info_extra_));
    }
PACKET_AD_CLOSE
//~! _NA004034_20120102_GUILD_POINT_SYSTEM

#pragma pack(pop)

#endif // __PACKETSTRUCT_AD_H__