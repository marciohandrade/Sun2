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
	bool					m_bBackToSelect;		// 게임중 다시 선택화면으로 돌아오는 것인지 여부
	BYTE					m_ChannelID;			// 채널 ID
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
    BYTE m_ReNameStatus[MAX_CHARACTER_LIST_NUM]; //캐릭터명 변경 여부 플레그 값이다 캐릭터 정보와 섞지 말고 따로 할당한다.
#endif
    BYTE m_pBuffer[sizeof(SERVER_CHARACTER_PART) * MAX_CHARACTER_LIST_NUM + sizeof(BYTE)];
    // <임시버퍼로 사용할 타입>
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
    BYTE gender; // 성별
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	WORD				honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool is_equipped_costume; // 코스튬 활성화 여부
    POSTYPE costume_item_postion; // 코스튬 인벤토리 위치
    SLOTCODE costume_item_code; // 코스튬 아이템 번호
#endif  // _NA_004035_20120227_COSTUME_ITEM

PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_CHARINFO, AD_CHARINFO_CREATE_ACK )
    BYTE m_pBuffer[sizeof(SERVER_CHARACTER_PART)];
    // <임시버퍼로 사용할 타입>
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

// _KR_0_20091021_FIRENDINVITATION_EXTEND //프로토콜 선언 = { AD_EVENT_INVITATE~ }

//추천된 친구가 캐릭터를 생성할때 추천한 캐릭터의 정보를 전송한다.
PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATIONR_INSERT_CMD )
    BYTE m_CharGuidCount;
    STRUCT_FRIEND_INVITATE_INFO InvitateInfo[MAX_FRIEND_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(InvitateInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(InvitateInfo[0]) * m_CharGuidCount);
    }
PACKET_AD_CLOSE


// 친구 초대 이벤트에 대한 보상 목록 요청
PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN )
    CHARGUID    CharGUID;
PACKET_AD_CLOSE

PACKET_AD_OPEN( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK )
    CHARGUID    CharGUID;
    BYTE		ErrorCode;  //1: DB 시스템 오류, 2: DB Proxy 서버 접속 불가
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
// END 친구 초대 이벤트에 대한 보상 목록 요청

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