#ifndef __PACKETSTRUCT_AG_H__
#define __PACKETSTRUCT_AG_H__

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_AG.h>
#include <PacketStruct_CG.h>

#pragma pack(push,1)


#define PACKET_AG_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_AG_CLOSE		};

#define PACKET_AG_INHERIT_OPEN( c, p, b )	struct MSG_##p : public b { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_AG_INHERIT_CLOSE		};

// _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
PACKET_AG_OPEN(AG_CONNECTION, AG_CONNECTION_DBCONNECT_UPDATE_CMD)
    int db_connector_index_;
PACKET_AG_CLOSE;

PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_DISCONNECT_CMD )
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    BYTE    m_AbNormalExit; // 0 : NormalExit, else AbNormalExit
PACKET_AG_CLOSE
/*
PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_UNREGISTER_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_UNREGISTER_ACK )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_UNREGISTER_NAK )
	DWORD				m_dwErrorCode;
PACKET_AG_CLOSE
*/

PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_UNREGISTER_CMD )
	BYTE				m_byDisonnectCode;
PACKET_AG_CLOSE

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
PACKET_AG_OPEN(AG_CONNECTION, AG_CONNECTION_SERVER_SELECT_CONTROL_SYN)
    enum eControl {
        eControl_StartTransaction,  // need ack - side effect - can't change behave state
        eControl_StopTransaction,   // notify
    };
    BYTE m_Control;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_CONNECTION, AG_CONNECTION_SERVER_SELECT_CONTROL_ACK)
    DWORD m_Result; // RC::eROOM_RESULT
PACKET_AG_CLOSE
#endif

PACKET_AG_OPEN( AG_CONNECTION, AG_WORLDSERVER_INFO_CMD )
	BYTE				m_bIsConnectedWorld;	// 월드서버와의 접속유무를 알려준다.
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_CONNECTION, AG_CONNECTION_SERVER_INFO_CMD)
    BYTE    pvp_enabled_; // pvp enable mode control by a agent server
    BYTE    channel_id_;  // (CHANGES) (f100424.3L) add channel id to synchronize or debug
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_DISCONNECT_SERVER_CMD )
	eSERVER_TYPE		m_ServerType;
	BYTE				m_byDisonnectCode;
PACKET_AG_CLOSE


PACKET_AG_OPEN(AG_SYNC, AG_SYNC_PLAYER_ENTER_SYN)
    // (f100623.1L) change a data types
    EngineFileChecksum checksum_list_;
    uint8_t is_dummy_client_test_; // to support the agent server dummy client
    uint8_t tail_guard_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_SYNC, AG_SYNC_PLAYER_ENTER_ACK )
	float				m_fPos[3];
	SLOTCODE			m_TransSkillCode;
	// _NA001955_110210_WING_COSTUME
    // CHANGES: f110607.3L, added field to support whether his wings spreading status has continued.
    bool continued_wings_spreading_;
    DOMINATION_GUILD_INFO m_GuildInfo;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION    
    sDOMINATION_CONTINENT_LORD_INFO m_ContinentLordInfo;    //대륙 영주 정보
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_SYNC, AG_SYNC_PLAYER_ENTER_NAK )
	DWORD				m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_SYNC, AG_SYNC_PLAYER_WARP_SYN)
    // (f100623.1L) change a data types
    EngineFileChecksum checksum_list_;
    uint8_t is_dummy_client_test_; // to support the agent server dummy client
    uint8_t tail_guard_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_SYNC, AG_SYNC_PLAYER_WARP_ACK )
	float				m_fPos[3];
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_SYNC, AG_SYNC_PLAYER_WARP_NAK )
	DWORD				m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CONNECTION, AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD )
	CHARGUID			m_CharGuid;
	eABUSE_SUB_TYPE m_eAbuseSubType;
PACKET_AG_CLOSE


//----------------------------------------------------------------------------------
// CHARINFO
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_CHARINFO, AG_CHARINFO_CHAR_CMD )
	PLAYERHP			m_MaxHP;
	PLAYERHP			m_HP;
	MPTYPE		    	m_MaxMP;
	MPTYPE			    m_MP;
	INT					m_dwCHuntingVCount;		// 경쟁헌팅 승리횟수
	INT					m_dwCHuntingTotCount;	// 경쟁헌팅 참여횟수
	BYTE				m_byChaoState;			// 카오상태
    PLAYERSD            m_MaxSD;
    PLAYERSD            m_SD;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP            max_FP;
    PLAYERFP            cur_FP;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHARINFO, AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD )
	WzVector								m_ReturnPos;
	FIELDCODE								m_ReturnFieldCode;
	eZONETYPE								m_ReturnZoneType;
	ZONEKEY									m_ReturnZoneKey;
	WORD									m_PartyKey;
	CHARGUID								m_CharGUID;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_CHARINFO, AG_CHARINFO_PCBANG_BILL_CMD)
    DWORD    m_PcRoomId;    // ReservedValue
PACKET_AG_CLOSE

//!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
PACKET_AG_OPEN(AG_CHARINFO, AG_CHARINFO_PREMIUMSERVICE_BILL_CMD)
    DWORD premiumservice_;
PACKET_AG_CLOSE
//~! _NA_006413_20130201_PREMIUMSERVICE_ITEM

//----------------------------------------------------------------------------------
//
// ZONE
//
//----------------------------------------------------------------------------------
// 로비 생성
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_LOBBY_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< 생성할 방 정보
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_HUNTING_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< 생성할 방 정보
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_MISSION_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
    eZONE_RULESET::eRules m_Rules; //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    //
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< 생성할 방 정보
    PartyZoneType   party_zone_type; //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PVP_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< 생성할 방 정보
	ADDITIONAL_PVPINFO m_AdditionalPVPInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHUNTION_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< 생성할 방 정보
	ADDITIONAL_CHUNTINGINFO m_AdditionalInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PVP_INFO_CMD )
	KEYTYPE				m_RoomKey;
	BYTE				m_Count;
	struct PVP_TEAM_INFO
	{
		DWORD			m_dwPlayerKey;
		BYTE			m_Team;
	} m_Info[MAX_PARTYMEMBER_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Info);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Info[0]) * m_Count);
    }
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_ZONE, AG_ZONE_VILLAGE_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_FIELD_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_FIELD_CREATE_CMD )
    MAPCODE			m_MapCode;
    KEYTYPE			m_RoomKey;
PACKET_AG_CLOSE

//_NA_008069_20150210_ADD_SPA_SYSTEM
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_SPAFIELD_CREATE_CMD)
    MAPCODE map_code;
    KEYTYPE room_key;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_FIELD_INFO_SYN )
    KEYTYPE			m_RoomKey;
    eDOMINATION_EVENT_STATE m_byEventState; 
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_FIELD_INFO_ACK )
    KEYTYPE			m_RoomKey;
    GUILDGUID       m_offenceGuild;
    GUILDGUID       m_defenceGuild;
    eDOMINATION_EVENT_TYPE m_eSetType;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_FIELD_INFO_NAK )
    KEYTYPE			m_RoomKey;
    DWORD		    m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_REQ_KICK_CMD )
    MAPCODE			m_mapCode;
    USERGUID        m_userGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_DESTROY_CMD )
	KEYTYPE		m_RoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_JOIN_CMD )
	//BYTE		m_byRoomType;
	KEYTYPE		m_RoomKey;
	MAPCODE		m_wFieldCode;
	DWORD		m_dwAreaID;
	WzVector	m_vStartPos;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_JOIN_SYN )
	BYTE		m_byRoomType;					//< eZONETYPE
	DWORD		m_dwPlayerKey;
	BYTE		m_bySelectedCharIndex;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR		m_pszUserID[ID_MAX_LEN + 1];
#else
	TCHAR		m_pszUserID[ID_MAX_LEN];
#endif
	char		m_pszUserIP[IP_MAX_LEN];
	DWORD		m_dwCharGUID;
	KEYTYPE		m_RoomKey;
	DWORD		m_dwReservedValue;				// 예약 (0:일반사용자, 나머지값은 PC방 접속자 UID)
	BYTE		m_bySts;						// 유저상태(100이상은 GM등급)
	time_t		m_LoginTime_t;
	MAPCODE		m_wFieldCode;
	DWORD		m_dwAreaID;
	WzVector	m_vStartPos;
	bool		m_bIsFirstJoin;
	BYTE		m_PCRoomSts;

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	BYTE		m_byAge;
	BYTE		m_byRealNameAuth;
#endif  //__CN_OO0510__FATIGUE_MATCHING_UPDATE

	BYTE		m_byApplyFieldCode;				// 특수한 경우 제외하고, FieldCode 적용하지 않는 경우를 변경시키기 위한 코드 0, 1(apply)
	GUILDGUID	m_GuildGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_JOIN_ACK )
	//로딩 중 비정상 종료를 위한 값들...
	WzVector								ReturnPos;
	FIELDCODE								ReturnFieldCode;
	eZONETYPE								ReturnZoneType;
	KEYTYPE									ReturnZoneKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_JOIN_NAK )
	DWORD		m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_LEAVE_CMD )
	KEYTYPE		m_RoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_LEAVE_SYN )
	BYTE		m_byRoomType;								//< eZONETYPE
	KEYTYPE		m_RoomKey;
	BYTE		m_ToRoomType;								//나가서 가려는 방타입.
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_LEAVE_ACK )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_LEAVE_NAK )
	DWORD		m_dwErrorCode;
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_ZONE, AG_ZONE_MASTER_CHANGED_CMD )
	KEYTYPE		m_RoomKey;
	DWORD		m_dwMasterKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_MISSION_LEAVE_CMD )
    enum eLeaveType { eLeaveType_Default, eLeaveType_Target, eLeaveType_Count };
    BYTE        m_LeaveType; //eLeaveType
    MAPCODE     m_ReturnMapCode; //if(m_LeaveType == eLeaveType_Target)
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHUNTING_LEAVE_CMD )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHUNTING_COMPETITION_END_CMD )
	KEYTYPE		m_RoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PORTAL_FIELD_MOVE_SYN )
	WORD		m_wIndex;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PORTAL_FIELD_MOVE_ACK )
	WORD		m_wIndex;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PORTAL_FIELD_MOVE_NAK )
	BYTE		m_byErrorCode;
PACKET_AG_CLOSE


// 필드에서 죽었을대, 마을로 이동 패킷.
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_ACK )
	WORD		m_wMapCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_NAK )
	BYTE		m_byErrorCode;
PACKET_AG_CLOSE

// 방의 SpecialType이 바뀜
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHANGE_SPECIALTYPE_CMD )
	KEYTYPE		m_RoomKey;
	BYTE		m_SpecialType;		// eROOM_SPECIAL_TYPE
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_TUTORIAL_SKIP_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_TUTORIAL_SKIP_ACK )
PACKET_AG_CLOSE


// 방장이 있는 필드,위치등을 요청
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_TO_ROOM_INFO_SYN )
	DWORD			m_dwRequestUserKey;
	WORD			m_wPartyKey;

	inline DWORD&	REQUEST_USER_KEY()		{ return m_dwRequestUserKey; }
	inline WORD&	PARTY_KEY()				{ return m_wPartyKey; }
PACKET_AG_CLOSE

// 방장이 있는 필드,위치등을 요청에 대한 응답 (TRUE, FALSE 포함)
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_TO_ROOM_INFO_ACK )
	enum eRESULT { FAIL = 0, SUCCESS = 1, };
	BYTE				m_byResult;
	DWORD				m_dwRequestUserKey;
    //----- if m_byResult is FAIL, at this point, message size is end. -----
	WORD				m_wPartyKey;
	KEYTYPE				m_ktZONETYPE;

	MAPCODE				m_mcMAPCODE;
	FIELDCODE			m_fcFIELDCODE;
	WzVector			m_wvRndPosOfMaster;

	inline BYTE			RESULT()			{ return m_byResult; }

	inline DWORD&		REQUEST_USER_KEY()	{ return m_dwRequestUserKey; }
	inline WORD&		PARTY_KEY()			{ return m_wPartyKey; }
	inline KEYTYPE&		ZONE_KEY()			{ return m_ktZONETYPE; }

	inline MAPCODE&		MAP_CODE()			{ return m_mcMAPCODE; }
	inline FIELDCODE&	FIELD_CODE()		{ return m_fcFIELDCODE; }
	inline WzVector&	RNDPOSofMASTER()	{ return m_wvRndPosOfMaster; }

    uint16_t GetSize() const {
        if (m_byResult == this->SUCCESS) {
            return sizeof(*this);
        };
        return static_cast<uint16_t>((uint8_t*)&m_wPartyKey - (uint8_t*)this);
    };
PACKET_AG_CLOSE

//AG_ZONE_RETURN_TO_ROOM_SYN			,
//AG_ZONE_RETURN_TO_ROOM_ACK	


PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_JOIN_INFO_SYN, MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN )
	DWORD				m_CodeOfSSQ;
	inline	WORD		GetSize() const { return sizeof(*this); }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_JOIN_INFO_ACK, MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK )
	SLOTCODE			m_ItemCode;	// Addin

	inline VOID			SetSuccessResult( const MAPCODE CodeOfSSQ, const SLOTCODE itemCode )
	{
		m_Result = SUCCESS;
		RETINFO_SUCCESS.m_CodeOfSSQ = CodeOfSSQ;
		m_ItemCode	= itemCode;
	}

	inline WORD			GetSize() const
	{
		const WORD DEF_SIZE = sizeof(MSG_BASE_FORWARD) + sizeof(BYTE);
		return SUCCESS == m_Result
			?	(WORD)( DEF_SIZE + max( sizeof(RETINFO_SUCCESS), sizeof(RETINFO_FAIL) ) + sizeof(m_ItemCode) )
			:	(WORD)( DEF_SIZE + sizeof(RETINFO_FAIL) )
			;
	}
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_JOIN_SYN, MSG_CG_ZONE_EX_SSQ_JOIN_SYN )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_JOIN_ACK, MSG_CG_ZONE_EX_SSQ_JOIN_ACK )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_ROOM_CTRL_CMD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_SSQ_ROOM_INFO_BRD, MSG_CG_STATUS_SSQ_INFO_BRD )
PACKET_AG_INHERIT_CLOSE

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

//G->A
PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_QUEST_PORTAL_REQUEST_SYN,
                       MSG_CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD)
PACKET_AG_INHERIT_CLOSE

//A->G
PACKET_AG_OPEN(AG_ZONE, AG_ZONE_QUEST_PORTAL_CONTROL_CMD)
    enum eCMD { eCMD_CREATE, eCMD_CLOSE, eCMD_COUNT, };
    eCMD            command;    // eCMD
    QuestPortalInfo portalInfo;
PACKET_AG_CLOSE

//A->G
PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_QUEST_PORTAL_ENTER_SYN,
                       MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN)
    MAPCODE     enterableMapCode;
    WORD        questCode;
    WzVector    portalPosision;

    inline WORD GetSize() const
    {   // 세부 옵션이 추가될 것이다.
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_AG_INHERIT_CLOSE

//G->A (SUCCESS|FAIL)
PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_QUEST_PORTAL_ENTER_ACK,
                       MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK)
    BYTE    syncType;  // eSYNC
    KEYTYPE portalKey;
    WORD    mapEnterancePortalIndex;
PACKET_AG_INHERIT_CLOSE
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CHANGE_MONSTER_ABILITY_CMD)
    KEYTYPE roomKey;
    WORD    number_of_players;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_SPA_ENTER_SYN)
    CODETYPE item_code;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_SPA_ENTER_ACK)
    MAPCODE spa_mapcode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_SPA_LEAVE_SYN)
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_SPA_LEAVE_ACK)
    MAPCODE village_map_code;
PACKET_AG_CLOSE
//----------------------------------------------------------------------------------
// Party
//----------------------------------------------------------------------------------
// 파티 생성
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CREATE_CMD )
	WORD		m_wPartykey;
	DWORD		m_MasterKey;
	BYTE		m_byItemDistributionType;
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    ePARTY_TYPE m_PartyType;
PACKET_AG_CLOSE

// 파티 소멸
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_DESTROY_CMD )
	WORD		m_wPartykey;
PACKET_AG_CLOSE

// 파티 가입
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_JOIN_CMD )
	WORD		       m_wPartykey;
	bool		       m_bMaster;
	_PARTY_MEMBER_INFO m_MemberInfo;
PACKET_AG_CLOSE

// 파티 탈퇴
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_LEAVE_CMD )
	WORD		m_wPartykey;
	DWORD		m_dwMemberKey;
PACKET_AG_CLOSE

// 파티장 변경
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CHANGE_MASTER_CMD )
	WORD		m_wPartykey;
	DWORD		m_dwMemberKey;
PACKET_AG_CLOSE

// 파티 아이템분배 방식 변경
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD )
	WORD		m_wPartykey;
	BYTE		m_byItemDistributionType;
PACKET_AG_CLOSE

//__NA_0_20080416_PARTY_JOIN_PACKET_CHANGE [
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_JOIN_SYN )
    //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
    enum
    {
        ENABLE_REQUEST = 0,
        ENABLE_RESPONSE = 1,
    };
    DWORD       m_dwFlag;
	DWORD		m_dwReqMemberKey;
	DWORD		m_dwRecvMemberKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_JOIN_ACK )
    //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
    DWORD       m_dwFlag;
	DWORD		m_dwReqMemberKey;
	DWORD		m_dwRecvMemberKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_JOIN_NAK )
    //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
    DWORD       m_dwFlag;
    DWORD		m_dwRecvMemberKey;
	DWORD		m_dwReqMemberKey;
	DWORD		m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_LEAVE_BY_GUILDRELATION_SYN )
	WORD		m_wPartykey;
	DWORD		m_dwMemberKey;
	BYTE		m_bReason;
PACKET_AG_CLOSE


//__NA_0_20080416_PARTY_JOIN_PACKET_CHANGE ]

//{__NA001336_090514_PARTY_NAVI_SUPPORT__
PACKET_AG_INHERIT_OPEN(AG_PARTY, AG_PARTY_NAVI_INFO_BRD, MSG_CG_PARTY_NAVI_INFO_BRD)
    inline WORD GetSize() const
    {
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Nodes));
        return MinSize + WORD(sizeof(Node)) * m_NumberOfNodes;
    }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_OPEN(AG_STATUS, AG_STATUS_CHAR_STAT_CHANGE_CMD)
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    enum eEvent { eEvent_None, eEvent_ChangedHP, eEvent_ChangedMP, eEvent_ChangedSD, eEvent_ChangedFP, eEvent_Count };
#else
    enum eEvent { eEvent_None, eEvent_ChangedHP, eEvent_ChangedMP, eEvent_ChangedSD, eEvent_Count };
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    struct SharedInfo { BYTE event; WORD objectKey; };
    union uNode {
        struct HP : SharedInfo { DWORD maxHP, currentHP, reservedHP; } m_HP;
        struct MP : SharedInfo { DWORD maxMP, currentMP; } m_MP;
        struct SD : SharedInfo { DWORD maxSD, currentSD; } m_SD;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        struct FP : SharedInfo { PLAYERFP maxFP, currentFP; } m_FP;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
    BYTE m_NumberOfNodes;
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    BYTE user_count_;       // 이 패킷에 몇명의 유저정보가 들어있는가
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    uNode m_Nodes[MAX_PARTYMEMBER_NUM*(eEvent_Count-1)];       // 한 사람이 HP,MP,SD세 정보를 모두 가지는 경우,

    inline WORD GetSize() const
    {
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Nodes)) + user_count_;
#else
        const WORD MinSize = WORD(sizeof(*this) - sizeof(m_Nodes));
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        
        return MinSize + WORD(sizeof(uNode)) * m_NumberOfNodes;
    }
PACKET_AG_CLOSE
//}__NA001336_090514_PARTY_NAVI_SUPPORT__

//_NA_20110630_BATTLE_ZONE_RENEWAL 
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_CHANGE_OPTION_SYN )
    BasePartyRoomInfo party_room_info_;
    WORD party_key_;
    DWORD master_key_;
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_CHANGE_OPTION_ACK )
    enum eStatus {
        eStatus_PartialFail,// 해당 플레이어 정의되어 있지 않은 다른 이유로 실패
        eStatus_PartialOk,// 해당 플레이어 제약 조건 통과
        eStatus_Transaction,// 해당 플레이어 존 트랜잭션 (기타 상태 위배 Fail)
        eStatus_LevelViolation,// 해당 플레이어 레벨 조건 위반
        eStatus_RequireQuestClear,// 해당 플레이어 선행 Quest 완료 필요
        eStatus_RequireMissionClear,// 해당 플레이어 선행 Mission 완료 필요
    };

    struct MemberState
    {
        WORD object_key_;
        WORD status_;
    };

    RC::ePARTY_RESULT result_code_;
    DWORD master_key_;
    BasePartyRoomInfo party_room_info_;
    BYTE number_of_partymember_;
    MemberState member_state_[MAX_PARTYMEMBER_NUM];

    inline void Clear()
    {
        result_code_ = RC::RC_PARTY_SUCCESS;
        master_key_ = 0;
        number_of_partymember_ = 0;
        ZeroMemory(&member_state_, sizeof(member_state_));
    }

    inline WORD GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(member_state_);
        return static_cast<uint16_t>(MinSize + sizeof(member_state_[0]) * number_of_partymember_);
    }

PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_CREATE_ROOM_SYN )
    DWORD master_key_;
    BasePartyRoomInfo party_room_info_;
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_CREATE_ROOM_ACK )
    RC::ePARTY_RESULT result_code_;
    DWORD master_key_;
    BasePartyRoomInfo party_room_info_;
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_PARTY, AG_PARTY_READY_STATE_SYN )
    bool ready_state_;
    WORD party_key_;
    DWORD member_key_;
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_READY_STATE_ACK )
    DWORD member_key_;
    RC::ePARTY_RESULT result_code_;
    bool ready_state_;
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_PARTY, AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD )
    WORD party_key_;
    DWORD member_key_;
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_PARTY, AG_PARTY_PARTYROOM_ITEM_DELETE_CMD )
    DWORD member_key_;
    SLOTCODE require_item_;
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    PartyZoneType party_zone_type_;
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_RESET_READY_STATE_CMD )
    DWORD master_key_;
    WORD party_key_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_JOIN_ROOM_SYN )
    MAPCODE map_code_;
    eZONETYPE party_room_type_;
    WORD join_party_key_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_JOIN_ROOM_ACK )
    RC::ePARTY_RESULT result_code_;
    WORD join_party_key_;
    DWORD request_user_key_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_PARTY, AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD )
    WORD request_party_key_;
    DWORD request_user_key_;
    DWORD target_user_key_;
PACKET_AG_CLOSE

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
PACKET_AG_OPEN( AG_PARTY, AG_CHAOS_ZONE_PACKET )
    BYTE category_type;
PACKET_AG_CLOSE
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND



//----------------------------------------------------------------------------------
// Status
//----------------------------------------------------------------------------------
// 레벨업
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LEVEL_UP_CMD )
	LEVELTYPE			m_Level;			// 업된 레벨
	WORD				m_wRemainStat;		// 남은 스탯포인트
	WORD				m_wRemainSkill;		// 남은 스킬포인트
	DWORD				m_dwCurHP;			// 현재 HP(레벨업시 HP, MP를 채워준다.)
	DWORD				m_dwMaxMP;			// 최대 MP
	DWORD				m_dwCurMP;			// 현재 MP
    DWORD				m_dwMaxAndCurSD;			// 현재 SD(레벨업시 SD를 Max로 채워준다.)
PACKET_AG_CLOSE

// HP 변화
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_MAXHP_CHANGE_CMD )
	DWORD				m_dwMaxHP;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_HP_CHANGE_CMD )
	DWORD				m_dwCurHP;
	DWORD				m_dwReserveHP;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_MP_CHANGE_CMD )
	DWORD				m_dwCurMP;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_DEAD_CMD )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_RESURRECTION_CMD )
	DWORD				m_dwCurHP;
	DWORD				m_dwCurMP;
    DWORD				m_dwCurSD;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_MAXMP_CHANGE_CMD )
	DWORD				m_dwMaxMP;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_MAXSD_CHANGE_CMD )
DWORD				m_dwMaxSD;
PACKET_AG_CLOSE

// 내구도 감소
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_DECREASE_DURA_CMD )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_CHANGE_STATE_CMD )
	ePLAYER_BEHAVE_STATE	m_eBehaveState;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_PLAYING_TUTORIAL_CMD )
	BOOL				m_bIsPlayingTutorial;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_INTERCEPTION_SYN )
	eINTERCEPTION_TYPE	m_byInterceptionCode;
	BOOL				m_bRequestFlag;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_INTERCEPTION_ACK )
	BYTE				m_byInterceptionCode;
	BOOL				m_bRequestFlag;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_INTERCEPTION_NAK )
	BYTE				m_byInterceptionCode;
	BYTE				m_byErrorCode;
PACKET_AG_CLOSE

// declared by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_REWARD_SYN )
    CHARGUID m_CharGuid;
    MONEY    m_Prize;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_REWARD_ACK)
    enum eRESULT_CODE {
        RESULT_OK = 0,
        ERROR_CHAR_NOT_ONLINE
    };
    INT32    m_iResult;
    CHARGUID m_CharGuid;
    MONEY    m_Prize;
PACKET_AG_CLOSE

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
// 하임 로또 복권용지 직접 구입 요청
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_SYN )
    CHARGUID iCharGuid;                 // 캐릭터 ID
    WORD wDrawNum;                      // 회차
    BYTE bAuto;                         // 자동 여부
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
    INT64 i64TicketPrice;               // 복권용지 가격
PACKET_AG_CLOSE
// 하임 로또 복권용지 직접 구입 응답
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_ACK )
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
PACKET_AG_CLOSE
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

//__KR_001355_20090608_GROUPINGSYSTEM_GW 
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_ADDEXP_CMD )
    CHARGUID m_CharGuid;            // 캐릭터 Guid
    EXPTYPE  m_Amount;              // 경험치량
    TCHAR    m_szCharName[MAX_CHARNAME_LENGTH+1];//Low memeber name
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_ADDMONEY_CMD )
    CHARGUID m_CharGuid;            // 캐릭터 Guid
    MONEY    m_Amount;              // 보너스 하임
    TCHAR    m_szCharName[MAX_CHARNAME_LENGTH+1];//Hight memeber name
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_EXP_ACQUISITION_CMD )
    CHARGUID m_CharGuid;            // 캐릭터 ID
    BYTE     m_byAcquisitionType;   // 획득 사유 (eHEIM_ACQUISITION_TYPE)
    EXPTYPE  m_Amount;              // 경험치량
PACKET_AG_CLOSE

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_HEIM_ACQUISITION_NTF )
    CHARGUID m_CharGuid;            // 캐릭터 ID
    BYTE     m_byAcquisitionType;   // 획득 사유 (eHEIM_ACQUISITION_TYPE)
    MONEY    m_Amount;              // 하임 금액
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_HEIM_CONSUMPTION_NTF )
    CHARGUID m_CharGuid;            // 캐릭터 ID
    BYTE     m_byConsumptionType;   // 소비 사유 (eHEIM_CONSUMPTION_TYPE)
    MONEY    m_Amount;              // 하임 금액
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    int      m_LottoAccumRatio;     // 하임 행운 적립율
PACKET_AG_CLOSE

// __KR_001355_20090608_GROUPINGSYSTEM_GW
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_SET_MEMBERINFO_CMD )
    CHARGUID m_CharGuid;            
    eGROUP_STAT m_HighGroupInfo;
    eGROUP_STAT m_LowGroupInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_USER_DOMINATION_RETURNHIEM_CMD  )
    CHARGUID            m_CharGuid;             // 캐릭터 Guid
    eSYSTEM_MEMO_TYPE   m_Type;                 // 타입
    ULONGLONG           m_TenderCost;           // 입찰가 하임 | 왕의 증표
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// GM 명령어
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_GM, AG_GM_TELEPORT_CMD )
	WzVector			m_vDestPos;			// 이동시킬 위치
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_WHISPER_SET_CMD )
	BOOL				m_bOn;				// FALSE: 귓말 끔 TRUE: 귓말 켬
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_USERCHAT_BLOCK_CMD )
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	CHARGUID			m_CharGuid;			// 해당 사용자 CharGuid
	BOOL				m_bBlock;			// FALSE: 채팅 허용 TRUE: 채팅 금지
	DWORD				m_dwBlockTime;		// 채팅 금지일 경우 제한시간(분 단위)
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_ROOM_INFO_CMD )		//방정보
	DWORD				m_dwRoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_CMD )		//캐릭터 정보
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_SYN )		//캐릭터 정보 요청
	DWORD				m_dwGMUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_ACK )		//캐릭터 정보 요청
	DWORD				m_dwGMUserKey;
	TCHAR				m_pszAccountID[ID_MAX_LEN+1];			// 계정 아이디.
	eCHAR_TYPE			m_eCharType;							// 캐릭터 클래스
	WORD				m_wLevel;								// 레벨
	MONEY				m_Money;								// 가진 돈
	KEYTYPE				m_RoomKey;								// 방키
	CODETYPE			m_MapCode;								// 맵코드
	WzVector			m_vCurPos;								// 현재 위치
	TCHAR				m_szGuildName[MAX_GUILDNAME_LENGTH];	// 길드이름
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_NAK )		//캐릭터 정보 요청 실패
	BYTE				m_byErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHANNEL_INFO_CMD )		//채널정보 요청
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHANGE_ROOMTITLE_CMD )				//채널정보 요청
	KEYTYPE				m_dwRoomKey;							// 방키
	TCHAR				m_szRoomTitle[MAX_ROOMTITLE_LENGTH];	// 바꿀 룸제목
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_CMD )				// 사용자 강제 종료
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];		// 종료시킬 캐릭터 이름
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_SYN )				// 사용자 강제 종료
	DWORD				m_dwGMUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_ACK )				// 사용자 강제 종료
	DWORD				m_dwGMUserKey;
	DWORD				m_dwDisconnectUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_SERVER_SHUTDOWN_CMD )				// 사용자 강제 종료
	DWORD				m_dwStringCode;
	BYTE				m_byShutdownTime;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_GO_CMD )	
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];	// 이동시킬 대상이름(본인이 될 수도 있음)
	KEYTYPE				m_RoomKey;								// 방번호(0이면 필드간 이동임)
	FIELDCODE			m_FieldCode;							// 필드코드
	WzVector			m_vCurPos;								// 현재 좌표
	WzVector			m_vDestPos;								// 이동하고자하는 목적지 좌표

	BYTE				m_PortalType;							// ePORTAL_TYPE
	POSTYPE				m_atItemPos;
    SLOTCODE            m_atItemCode;                           // 해당 포지션의 아이템.

	BOOL IsRevenge()	{ return m_PortalType == ePORTAL_TYPE_REVENGE; }
    BOOL IsFriendInvitatedEvent() { return m_PortalType == ePORTAL_TYPE_FIRENDINVITATED_EVENT; }
	BOOL IsItem()		{ return (m_PortalType == ePORTAL_TYPE_ITEM || m_PortalType == ePORTAL_TYPE_ITEM_EX); }
PACKET_AG_CLOSE

//추적
PACKET_AG_OPEN( AG_GM, AG_GM_TRACKING_CMD )
	WzVector			m_vCurPos;
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];
	
	BYTE				m_PortalType;
	POSTYPE				m_atItemPos;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_TRACKING_REQUEST_INFO_SYN )	
	WzVector			m_vCurPos;
	TCHAR				m_szGMCharName[MAX_CHARNAME_LENGTH+1];

	BYTE				m_PortalType;
	POSTYPE				m_atItemPos;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_NOTICE_CMD)
	eNOTICE_TYPE		m_eType;
	WORD				m_wLen;
	TCHAR				m_tszNoticeMsg[MAX_NOTICE_LEN + 1];

    uint16 GetSize() const
    {   // CHANGES: f101210.2L
        const size_t kMinSize = sizeof(*this) - sizeof(m_tszNoticeMsg);
        return static_cast<uint16_t>(kMinSize + m_wLen * sizeof(m_tszNoticeMsg[0]));
    }
PACKET_AG_CLOSE

// declared by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

PACKET_AG_OPEN( AG_GM, AG_GM_LOTTO_NUM_CMD )
    DWORD dwUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_LOTTO_SETTLE_CMD )
    DWORD dwUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_LOTTO_FUND_CMD )
    DWORD user_key;
    INT64 amount_delta;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_DOMINATION_INIT_CMD )
    enum {time = 0, observer = 1};
    BYTE                   m_flag;                  // 0:시간 이벤트, 1:gm observer
    BOOL                   m_bMode; 
    sDOMINATION_EVENT_INFO m_Info;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_GM, AG_GM_GUILD_RANKING_REFRESH)
PACKET_AG_CLOSE

// _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
PACKET_AG_OPEN(AG_GM, AG_GM_BATTLE_GROUND_CMD)
    int cmd_mode;
    ChaosZoneTeam team_key;
    CHARGUID char_guid;
    KEYTYPE zone_key;
    char gm_cmd_str[100+1];
#ifdef _NA_007191_20140428_MODIFY_GM_COMMAND
    MAPCODE map_code;
#endif //_NA_007191_20140428_MODIFY_GM_COMMAND
PACKET_AG_CLOSE

//_NA_008334_20150608_SONNENSCHEIN
PACKET_AG_OPEN(AG_GM, AG_GM_SONNENSCHEIN_CMD)
    char gm_cmd[16];
PACKET_AG_CLOSE

// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
PACKET_AG_OPEN(AG_GM, AG_GM_ONEDAYSGUILDMISSION_CMD)
	char gm_cmd[16];
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// GUILD 관련 프로토콜
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_GUILD, AG_GUILD_CREATE_ACK )
	GUILDGUID			m_GuildGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GUILD, AG_GUILD_DESTROY_ACK )
PACKET_AG_CLOSE


PACKET_AG_OPEN( AG_GUILD, AG_GUILD_JOIN_SUCCESS_CMD )
	GUILDGUID			m_GuildGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GUILD, AG_GUILD_WITHDRAW_ACK )
PACKET_AG_CLOSE

// 길드원 강제탈퇴
PACKET_AG_OPEN( AG_GUILD, AG_GUILD_KICKPLAYER_ACK )
	TCHAR				m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	CHARGUID			m_CharGuid;	// 탈퇴시'킨' CharGuid
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// AG_CHUNTING 관련 프로토콜
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_ENTER_TEAM_CMD )
	KEYTYPE				m_RoomKey;
	BYTE				m_byReadyPlayerNum;		// 입장하려는 플레이어 수
	GUILDGUID			m_GuildGuid;
	DWORD				m_UserKeyList[CHUNTING_MAX_MEMBER_NUM];

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_UserKeyList);
        return static_cast<uint16_t>(kMinSize + sizeof(m_UserKeyList[0]) * m_byReadyPlayerNum);
    };
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_WIN_CMD )
	DWORD				m_WinCount;				// 경쟁헌팅 총 승리횟수
	DWORD				m_TotCount;				// 경쟁헌팅 총 전적
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_START_COMPETITION_CMD )
	DWORD				m_TotCount;				// 경쟁헌팅 총 전적
PACKET_AG_CLOSE


//----------------------------------------------------------------------------------
// AG_MISSION 관련 프로토콜
//----------------------------------------------------------------------------------
PACKET_AG_OPEN(AG_MISSION, AG_MISSION_STREAM_CMD)
    MISSION_TOTAL_INFO m_TotalInfo;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    }
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_MISSION, AG_MISSION_CLEAR_CMD )
	BYTE m_MissionNo;
PACKET_AG_CLOSE


//----------------------------------------------------------------------------------
// AG_CHAO 관련 프로토콜
//----------------------------------------------------------------------------------

// 리벤지(나쁜친구) 리스트 추가 요청
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_SYN )
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지 리스트에 추가할 대상 Name
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_ACK )
	DWORD				m_dwRevengeCharGuid;
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_NAK )
	DWORD				m_dwCharGuid;			// 요청하는 본인 CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT 참고
PACKET_AG_CLOSE


// 리벤지 정보 요청
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_ACK )
	BYTE				m_byRevengePoint;							// 남은 Revenge Point
	BYTE				m_byRevengeStatNum;							// 등록된 캐릭터 수
	STRUCT_REVENGE_INFO	m_sRevengeUserInfo[MAX_REVENGE_STAT_NUM];	// 등록된 캐릭터 목록

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sRevengeUserInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sRevengeUserInfo[0]) * m_byRevengeStatNum);
    };
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_NAK )
	DWORD				m_dwCharGuid;			// 요청하는 본인 CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT 참고
PACKET_AG_CLOSE

// 리벤지 정보 삭제 - 유저가 선택(A->G)
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_CMD )
	DWORD				m_dwRevengeCharGuid;	// 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AG_CLOSE

// 리벤지 성공 - 해당 정보 삭제 요청(G->A)
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_SYN )
	DWORD				m_dwRevengeCharGuid;	// 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_ACK )
	DWORD				m_dwRevengeCharGuid;	// 삭제할 리벤지(나쁜친구) CharGuid
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_NAK )
	DWORD				m_dwCharGuid;			// 요청하는 본인 CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT 참고
PACKET_AG_CLOSE

// 카오상태 변경
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_CHANGE_CHAO_STATE_CMD )
	DWORD				m_dwObjKey;
	BYTE				m_byChaoState;			// 카오상태(eCHAO_STATE)
PACKET_AG_CLOSE

// 리벤지 대상 추적 성공
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_TRACK_REVENGE_CHAR_ACK )
	POSTYPE				m_atItemPos;			// 복수의 돌 위치
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// AG_ITEM 관련 프로토콜
//----------------------------------------------------------------------------------

// 아이템을 이용한 좌표이동 성공
PACKET_AG_OPEN( AG_ITEM, AG_ITEM_PORTAL_ACK )
	POSTYPE				m_PortalPos;
    SLOTCODE            m_ItemCode;
PACKET_AG_CLOSE

// 스페셜 아이템 소유 변경
PACKET_AG_OPEN( AG_ITEM, AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD )
	BYTE				m_Apply;
	WORD				m_ItemType;
	WORD				m_Count;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_ITEM, AG_ITEM_UNLOCK_CMD)
    // (f100528.2L) prevent a locked state after an instance dungeon moving transaction failed
    enum ePostAction {
        eAction_None = 0,
        eAction_MoveFailed, // cancel an operation if the player plan on leave
        eAction_Counts
    };
    SLOTIDX m_ItemIndex;    
    POSTYPE m_ItemPos;
    uint8_t post_action_;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ITEM, AG_ITEM_DECREASE_UNLOCK_CMD )
	SLOTIDX				m_ItemIndex;	
	POSTYPE				m_ItemPos;
PACKET_AG_CLOSE

// 
PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_SYN)
	CHARGUID			m_CharGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_ACK)
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_NAK)
	BYTE				m_byErrorCode;
PACKET_AG_CLOSE

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// G->A 하임행운 캐시 응모권 사용 요청
PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN)
    CHARGUID char_guid;     // 캐릭터 ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AG_CLOSE

// A->G 하임행운 캐시 응모권 사용 응답
PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK)
    WORD     result_code;   // 결과 코드 : eITEM_RESULT
    CHARGUID char_guid;     // 캐릭터 ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AG_CLOSE
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

//----------------------------------------------------------------------------------
// AG_INSTANCE 관련 프로토콜
//----------------------------------------------------------------------------------

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN )
	POSTYPE				m_ItemPos;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK )
	MAPCODE				m_MapCode;
	BOOL				m_UseCharge;	// 캐시 사용여부 필드였지만, 재정의한다. 시간 제약 없는 경우의 플랙
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK )
	enum eRet { eErr_ItemRC, eErr_RoomRC };
	BYTE				m_ErrorKind;
	DWORD				m_dwErrorCode;
PACKET_AG_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_TOP_OF_TRIAL_SYN, MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_TOP_OF_TRIAL_ACK, MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_TOP_OF_TRIAL_NAK, MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_INSTANCE_CREATE_CMD)
	MAPCODE				m_MapCode;
	KEYTYPE				m_RoomKey;
	BYTE				m_TypeOfIndun;
	BASE_ROOMINFO		m_RoomInfo;									//< 생성할 방 정보
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    PartyZoneType       party_zone_info_;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneType       chaos_zone_type_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
PACKET_AG_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN, MSG_CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN )
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN( AG_ZONE, AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK, MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK )
PACKET_AG_INHERIT_CLOSE

//_NA_20110630_BATTLE_ZONE_RENEWAL
PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN)
    WORD party_key_;
    MAPCODE map_code_;
    PartyZoneType party_room_type_;
    SLOTCODE require_item_;//파티룸 생성에 필요한 아이템
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK)
    enum eResult {
        eResult_NoMemberFail,// 플레이어 검사 자체를 못한 상황임.
        eResult_InvalidMap,// 요청된 맵이 없거나 입장가능 타입이 아니다.
        eResult_CheckedMembers,// 해당 플레이어들의 검사까지 수행했다.
        eResult_FailCheckTerms,//필요 아이템이 룸 생성 조건에 맞지 않다.
        eResult_RequireQuestClear,//선행 퀘스트를 진행해야 한다.
        eResult_RequireMissionClear,//선행 미션을 진행해야 한다.
    };
    enum eStatus {
        eStatus_PartialFail,// 해당 플레이어 정의되어 있지 않은 다른 이유로 실패
        eStatus_PartialOk,// 해당 플레이어 제약 조건 통과
        eStatus_Transaction,// 해당 플레이어 존 트랜잭션 (기타 상태 위배 Fail)
        eStatus_LevelViolation,// 해당 플레이어 레벨 조건 위반
        eStatus_RequireQuestClear,// 해당 플레이어 선행 Quest 완료 필요
        eStatus_RequireMissionClear,// 해당 플레이어 선행 Mission 완료 필요
    };
    struct MemberInfo
    {
        WORD object_key_;
        WORD status_;
    };

    BYTE result_value_;
    MAPCODE map_code_;
    WORD party_key_;
    BYTE number_of_partymember_;
    bool is_cash_item_;
    SLOTCODE require_item_;
    MemberInfo member_info_[MAX_PARTYMEMBER_NUM];

    inline void Clear()
    {
        result_value_ = eResult_NoMemberFail;
        map_code_ = 0;
        party_key_ = 0;
        number_of_partymember_ = 0;
        is_cash_item_ = 0;
        ZeroMemory(&member_info_, sizeof(member_info_));
    }

    inline WORD GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(member_info_);
        return static_cast<uint16_t>(MinSize + sizeof(member_info_[0]) * number_of_partymember_);
    }
PACKET_AG_INHERIT_CLOSE


//{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__

PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_SYN)
    enum eCheckMethod {
        eCheck_None     ,
        eCheck_Party    = eCheck_None,  // 파티 기준 파티원들의 제약조건 검사
        eCheck_Members  ,               // 첨부되는 플레이어들의 제약 조건 검사
    };
    enum eResult {
        eResult_NoMemberFail        ,   // 플레이어 검사 자체를 못한 상황임.
        eResult_InvalidMap          ,   // 요청된 맵이 없거나 입장가능 타입이 아니다.
        eResult_CheckedMembers      ,   // 해당 플레이어들의 검사까지 수행했다.
    };
    enum eStatus {
        eStatus_PartialFail         ,   // 해당 플레이어 정의되어 있지 않은 다른 이유로 실패
        eStatus_PartialOk           ,   // 해당 플레이어 제약 조건 통과
        eStatus_Transaction         ,   // 해당 플레이어 존 트랜잭션 (기타 상태 위배 Fail)
        eStatus_LevelViolation      ,   // 해당 플레이어 레벨 조건 위반
        eStatus_RequireQuestClear   ,   // 해당 플레이어 선행 Quest 완료 필요
        eStatus_RequireMissionClear ,   // 해당 플레이어 선행 Mission 완료 필요
    };
    struct MemberInfo
    {
        WORD    objectKey;
        WORD    status;     // eResult
    };
    struct CheckArgParty
    {
        // SYN->ACK passing arg: GameServer는 해당 파티대상으로 MemberResult를 구축한다.
        WORD    partyKey;
    };
    struct CheckArgMembers : public CheckArgParty
    {
        // {partyKey, roomKey} Member들의 조사대상. 해당 (파티원/존) 목록에서 검사 수행.
        WORD    roomKey;    // SYN->ACK passing arg
        // 반드시 해당 배열(members)를 초기화 하고 설정할 것. 0이면 end
        MemberInfo  members[max(MAX_FIELD_PARTYMEMBER_NUM, MAX_PARTYMEMBER_NUM)];
    };

    //////////////////////////////////////////////////////////////////////////
    BYTE    m_CheckMethod;  // eCheckMethod 검사 방법
    BYTE    m_Result;       // eResult
    MAPCODE m_MapCode;      // Enter MapCode
    union
    {
        CheckArgParty   m_CheckArgParty;
        CheckArgMembers m_CheckArgMembers;
    };

    inline void Clear()
    {
        m_CheckMethod = eCheck_None;
        m_Result = eResult_NoMemberFail;
        m_MapCode = 0;
        const size_t argSize = max(sizeof(m_CheckArgParty), sizeof(m_CheckArgMembers));
        ZeroMemory(&m_CheckArgMembers, argSize);
    }

    inline WORD GetSize() const
    {
        const size_t MinSize = sizeof(*this) - max(sizeof(m_CheckArgParty), sizeof(m_CheckArgMembers));
        if(m_CheckMethod == eCheck_Members)
        {
            return WORD(sizeof(*this));
        }
        if(m_CheckMethod == eCheck_Party)
        {
            //gamelulu's note : 기존 Party관련 부분 처리가 없었다. 엄밀히 Member부분 체크에 있어서도
            //정확한 가변 패킷은 아닌 상태지만 좀더 살펴볼 필요가 있어서 기존 로직과 동일하게 처리
            return WORD(sizeof(*this));
        }
        return WORD(MinSize);   // Assert Violation...
    }
PACKET_AG_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_ACK, MSG_AG_ZONE_CAN_CREATE_ROOM_SYN)
    // Ack도 SYN내용과 동일하게 처리한다. 개별 조건으로 보내므로 m_CheckMethod == eCheck_Members;
    inline WORD GetSize() const { return MSG_AG_ZONE_CAN_CREATE_ROOM_SYN::GetSize(); }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_LOBBY_CREATE_SYN, MSG_CG_ZONE_LOBBY_CREATE_SYN)
    inline WORD GetSize() const { return WORD(sizeof(*this)); }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_LOBBY_CREATE_ACK, MSG_AG_ZONE_LOBBY_CREATE_SYN)
    RC::eROOM_RESULT    m_1stCheckResultInGameServer;
    inline WORD GetSize() const { return WORD(sizeof(*this)); }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_LOBBY_JOIN_SYN, MSG_CG_ZONE_LOBBY_JOIN_SYN)
    MAPCODE             m_MapCode;  // by Agent Setting
    inline WORD GetSize() const { return WORD(sizeof(*this)); }
PACKET_AG_INHERIT_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_LOBBY_JOIN_ACK, MSG_AG_ZONE_LOBBY_JOIN_SYN)
    RC::eROOM_RESULT    m_1stCheckResultInGameServer;
    inline WORD GetSize() const { return WORD(sizeof(*this)); }
PACKET_AG_INHERIT_CLOSE

//}__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__

PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN)
    enum { kMaxRoomCount = 40 };
    BYTE current_room_count_;
    MISSION_ROOMINFO room_info_list_[kMaxRoomCount];
    int GetSize() const
    {
        return sizeof(*this) - (kMaxRoomCount - current_room_count_) * sizeof(MISSION_ROOMINFO);
    }
PACKET_AG_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK, MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN)
PACKET_AG_INHERIT_CLOSE

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_DOMINATION_NOTIFIER_BRD )
    GUILDGUID win_guild_guid;                                            // 승
    GUILDGUID lose_guild_guid;                                           // 패
    MAPCODE   map_code;                                            // 점령지역
    char win_guild_name[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_AG_CLOSE
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

PACKET_AG_INHERIT_OPEN( AG_EVENT, AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN, MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD )
PACKET_AG_INHERIT_CLOSE

//W->A 초대한 캐릭터 목록 (가변)
//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD)
	INVITATEFRIENDLIST_INFO m_InvitateFriendListInfo;	

	WORD GetSize() const
    {   // (CHANGES) (f100624.2L)
        const WORD kMinSize = sizeof(*this) - sizeof(m_InvitateFriendListInfo);
        return static_cast<WORD>(kMinSize + m_InvitateFriendListInfo.GetSize());
		//return ( sizeof(MSG_AW_EVENT_FRIENDINVITATE_INVITATELIST_CMD) - ( (MAX_FRIEND_STAT_NUM - m_InvitateFriendListInfo.m_byListCount) * sizeof(DWORD) ) );
	}
PACKET_AG_INHERIT_CLOSE

//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_RECALL_ACK) //에인전트에서 게임 서버로 리콜 결과를 전송
    CHARGUID    CalleeGUID;
PACKET_AG_INHERIT_CLOSE

//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_RECALL_NAK) //에인전트에서 게임 서버로 리콜 결과를 전송
    BYTE    ErrorCode;
PACKET_AG_INHERIT_CLOSE

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_SYSTEMMEMO_SEND_CMD) 
    DWORD system_memo_list_num_;//시스템 메모 갯수
    BasicSystemMemoInfo system_memo_info_[MAX_SYSTEM_MEMO_LIST];//시스템 메모 정보 리스트

    DWORD GetSize() const
    {
        assert(system_memo_list_num_ <= _countof(system_memo_info_));
        return (sizeof(*this) - sizeof(system_memo_info_)) +        //고정 크기
               (sizeof(system_memo_info_[0]) * system_memo_list_num_);//가변 크기
    }
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    void Clear()
    {
        system_memo_list_num_ = 0;
        memset(system_memo_info_, 0, sizeof(BasicSystemMemoInfo) * _countof(system_memo_info_));
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
PACKET_AG_INHERIT_CLOSE

#ifdef _NA_003027_20111013_HONOR_SYSTEM
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_TODAYQUEST_INFO_BRD )
WORD count_;
QUEST_TODAY_QUEST_INFO today_quest_info[MAX_TODAYQUEST_COUNT];
PACKET_AG_CLOSE
#endif //_NA_003027_20111013_HONOR_SYSTEM

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//	<PACKET BROADCAST FUNCTIONs>
//
//
//
#include <VarPacket.h>


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	전송시 사용하는 패킷 형태 (고정 패킷 입력시)
//
template<typename _TYPE>
struct MSG_PACKET_BROADCASTER_ONEtoN : MSG_BASE_FORWARD
{
	MSG_PACKET_BROADCASTER_ONEtoN() { Init(); }

//////////////////////////////////////////////////////////////////////////
// <FIELDs>
	enum { MAX_USER_COUNT = 255 };
	BYTE		m_byUserCount;
	WORD		msgBODY_SZ;
	_TYPE		msgBODY;

	DWORD		m_dwUserKeys[MAX_USER_COUNT];

//////////////////////////////////////////////////////////////////////////
// <METHODs>
	inline VOID			Init()
	{
		m_byCategory = AG_CONNECTION;
		m_byProtocol = AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN;
		m_byUserCount = 0;
		msgBODY_SZ = (WORD)sizeof(msgBODY);
	}

	inline DWORD		AddUser( DWORD dwUserKey )
	{
		if( MAX_USER_COUNT == m_byUserCount )
			return 0;

		m_dwUserKeys[m_byUserCount++] = dwUserKey;
		return (DWORD)m_byUserCount;
	}

	inline WORD			GetSize()
	{
		const WORD SZofBASE_ZEROUSER = sizeof(*this) - sizeof(m_dwUserKeys);
		return SZofBASE_ZEROUSER + (WORD)(sizeof(DWORD)*m_byUserCount);
	}

	//inline WORD			GetRecvSize()
	//{
	//	//															type size	
	//	const WORD cwSize = sizeof(MSG_BASE_FORWARD) + sizeof(msgBODY_SZ) + sizeof(m_byUserCount);
	//	WORD SZofSENDINGMSG = cwSize + (WORD)(msgBODY_SZ + sizeof(DWORD)*m_byUserCount);
	//	return SZofSENDINGMSG;
	//}
};

//==================================================================================================
//==================================================================================================
//==================================================================================================
// CHANGES: f110601.1L, remove the broadcast upper boundary limitation using 1toN broadcaster
PACKET_AG_OPEN(AG_CONNECTION, AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN)
    enum { MAX_USER_COUNT = 5000, };
    struct MsgBodyInfo
    {
        MSG_BASE_FORWARD* msg_send;
        uint16_t msg_send_size;
        uint16_t number_of_users;
        // NOTE: f080704.x, waverix, changes user_key to object_key
        //  changed by __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__
        uint16_t* user_object_keys;
    };
    //
    uint16_t number_of_users;
    uint16_t msg_body_size;
    // msg_body stream[msg_body_size]
    // - alignment
    // user key stream[number_of_users]
    //
    void Init(MSG_BASE_FORWARD* msg, uint16_t msg_size);
    void InitByArray(uint8_t** msg_array, uint16_t* msg_size_array, ulong number_of_arrays);
    // Init, InitByArray 이후, AddUser호출 이전에 추가시키는 방식이다.
    void AppendMsg(uint8_t* msg, uint16_t msg_size);
    MSG_BASE_FORWARD* GetBrdPacketPtr();
    uint16_t* GetUsersStartPtr();
    uint16_t AddUser(uint16_t* const user_begin, DWORD object_key);
    uint16_t GetSendingSize() const;

    // <패킷 수신시 사용하는 METHOD>
    // <returns>
    //  size of variable msg - <WARNING> 이 값을 실제 수신된 값과 비교해서 일치하지 않으면 처리해선 안된다.
    //  MsgBodyInfo.msg_send = BROADCASTING MSG
    //  MsgBodyInfo.msg_send_size = BROADCASTING MSG SIZE
    uint16_t GetPacketInfo(MsgBodyInfo* result_info) const;
PACKET_AG_CLOSE;

//==================================================================================================

inline void MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::Init(
    MSG_BASE_FORWARD* msg, uint16_t msg_size)
{
    m_byCategory = AG_CONNECTION;
    m_byProtocol = AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN;
    number_of_users = 0;
    msg_body_size = msg_size;

    CopyMemory(GetBrdPacketPtr(), msg, msg_size);
}

inline void MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::InitByArray(
    uint8_t** msg_array, uint16_t* msg_size_array, ulong number_of_arrays)
{
    m_byCategory = AG_CONNECTION;
    m_byProtocol = AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN;
    number_of_users = 0;
    msg_body_size = 0;

    uint8_t* offset = reinterpret_cast<uint8_t*>(GetBrdPacketPtr());
    for (ulong index = 0; index < number_of_arrays; ++index, ++msg_array, ++msg_size_array)
    {
        uint16_t msg_size = *msg_size_array;
        msg_body_size += msg_size;
        CopyMemory(offset, *msg_array, msg_size);
        offset += msg_size;
    };
}

inline void MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::AppendMsg(
    uint8_t* msg, uint16_t msg_size)
{
    uint8_t* writing_pos = reinterpret_cast<uint8_t*>((uint16_t*)(&msg_body_size) + 1);
    ;        writing_pos += msg_body_size;
    CopyMemory(writing_pos, msg, msg_size);
    msg_body_size += msg_size;
}

inline MSG_BASE_FORWARD* MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::GetBrdPacketPtr()
{
    MSG_BASE_FORWARD* offset = reinterpret_cast<MSG_BASE_FORWARD*>((uint16_t*)(&msg_body_size) + 1);
    return offset;
}

inline uint16_t* MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::GetUsersStartPtr()
{
    uint16_t* user_begin = reinterpret_cast<uint16_t*>(
        reinterpret_cast<uint8_t*>(GetBrdPacketPtr()) + msg_body_size);
    return user_begin;
}

inline uint16_t MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::AddUser(
    uint16_t* const user_begin, DWORD object_key)
{
    if (number_of_users == MAX_USER_COUNT) {
        return 0;
    };
    user_begin[number_of_users++] = static_cast<uint16_t>(object_key);
    return number_of_users;
}

inline uint16_t MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::GetSendingSize() const
{
    const size_t kMinSize = sizeof(*this);
    size_t total_size = kMinSize + msg_body_size + (sizeof(uint16_t) * number_of_users);
    return static_cast<uint16_t>(total_size);
}

inline uint16_t MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::GetPacketInfo(
    MsgBodyInfo* result_info) const
{
    const size_t kMinSize = sizeof(*this);
    ZeroMemory(result_info, sizeof(*result_info));

    uint8_t* offset = reinterpret_cast<uint8_t*>((uint16_t*)(&msg_body_size) + 1);
    result_info->msg_send = reinterpret_cast<MSG_BASE_FORWARD*>(offset);
    result_info->msg_send_size = msg_body_size;

    offset += msg_body_size;
    result_info->number_of_users = number_of_users;
    result_info->user_object_keys = reinterpret_cast<uint16_t*>(offset);

    size_t total_msg_size = kMinSize + msg_body_size;
    ;      total_msg_size += (sizeof(uint16_t) * number_of_users);
    return static_cast<uint16_t>(total_msg_size);
}
//==================================================================================================
#if SUN_CODE_BACKUP
//==================================================================================================

//	송/수신시 사용하는 패킷 형태 (가변 패킷)
//	송신시 패킷은 특정 버퍼를 기준으로 사용해야 한다.
PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN )
//////////////////////////////////////////////////////////////////////////
// <FIELDs>
	enum { MAX_USER_COUNT = 255 };
	struct MSG_INFO
	{
		MSG_BASE_FORWARD*	pMSG_BODY;
		WORD				wSIZE;
		WORD				wUSER_COUNT;
		DWORD*				pUSERs;	//	(WAVERIX)(080704) - UserKey가 아니라 ObjectKey로 변경 <-__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__ 적용시
	};

	BYTE		m_byUserCount;
	WORD		msgBODY_SZ;
	// ~~
	// ~~

	//	<패킷 제작시 사용하는 METHODs>
	inline VOID					Init( MSG_BASE_FORWARD* pMsg, WORD wBrdPacketSize )
	{
		m_byCategory	= AG_CONNECTION;
		m_byProtocol	= AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN;
		m_byUserCount	= 0;
		msgBODY_SZ		= wBrdPacketSize;

		CopyMemory( GetBrdPacketPtr(), pMsg, msgBODY_SZ );
	}

	inline VOID					InitByArray( BYTE** ppMsg, WORD* pwBrdPacketSize, DWORD dwMsgArrayCnt )
	{
		m_byCategory	= AG_CONNECTION;
		m_byProtocol	= AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN;
		m_byUserCount	= 0;
		msgBODY_SZ		= 0;

		BYTE* pOffset	= (BYTE*)GetBrdPacketPtr();
		for( DWORD i=0 ; i<dwMsgArrayCnt ; ++i )
		{
			WORD wSize = pwBrdPacketSize[i];
			msgBODY_SZ += wSize;
			CopyMemory( pOffset, ppMsg[i], wSize );
			pOffset += wSize;
		}
	}

	//	Init, InitByArray 이후, AddUser호출 이전에 추가시키는 방식이다.
	inline VOID					AppendMsg( BYTE* const pMsg, const WORD msg_size )
	{
		BYTE* pIt	= (BYTE*)(((WORD*)&msgBODY_SZ)+1);
		pIt			+= msgBODY_SZ;
		CopyMemory( pIt, pMsg, msg_size );
		msgBODY_SZ	+= msg_size;
	}

	inline MSG_BASE_FORWARD*	GetBrdPacketPtr()
	{
		MSG_BASE_FORWARD* pOFFSET = (MSG_BASE_FORWARD*)(((WORD*)&msgBODY_SZ)+1);
		return pOFFSET;
	}

	inline DWORD*				GetUsersStartPtr()
	{
		DWORD* pStartPtr = (DWORD*) ( ((BYTE*)GetBrdPacketPtr()) + msgBODY_SZ );
		return pStartPtr;
	}

	inline DWORD				AddUser( DWORD* pUserStartPtr, DWORD dwUserKey )
	{
		if( MAX_USER_COUNT == m_byUserCount )
			return 0;

		pUserStartPtr[m_byUserCount++] = dwUserKey;
		return (DWORD)m_byUserCount;
	}

	inline WORD					GetSendingSize() const
	{
		const WORD cwMIN_SZ = sizeof(MSG_BASE_FORWARD) + sizeof(msgBODY_SZ) + sizeof(m_byUserCount);
		WORD wSIZE = cwMIN_SZ + msgBODY_SZ + (WORD)(sizeof(DWORD)*m_byUserCount);
		return wSIZE;
	}

	//	<패킷 수신시 사용하는 METHOD>
	//	<returns>
	//		size of variable msg - <WARNING> 이 값을 실제 수신된 값과 비교해서 일치하지 않으면 처리해선 안된다.
	//		MSG_INFO.pMSG_BODY = BROADCASTING MSG
	//		MSG_INFO.wSIZE = BROADCASTING MSG SIZE
	inline WORD			GetPacketInfo( MSG_INFO& rINFO )
	{
		//
		const WORD cwMIN_SZ = sizeof(MSG_BASE_FORWARD) + sizeof(msgBODY_SZ) + sizeof(m_byUserCount);

		BYTE* pOFFSET = (BYTE*)(((WORD*)&msgBODY_SZ)+1);
		rINFO.pMSG_BODY = (MSG_BASE_FORWARD*)pOFFSET;	//최소한 보내고자 했던 메시지 정보는 확인 가능
		rINFO.wSIZE = msgBODY_SZ;

		WORD wSIZE = cwMIN_SZ + msgBODY_SZ;
		pOFFSET += msgBODY_SZ;
		rINFO.wUSER_COUNT = (WORD)m_byUserCount;
		rINFO.pUSERs = (DWORD*)pOFFSET;

		wSIZE = wSIZE + (WORD)(sizeof(DWORD)*m_byUserCount) ;
		return wSIZE;
	}
PACKET_AG_CLOSE;

#endif //!SUN_CODE_RENEWAL

//==================================================================================================
//==================================================================================================
//==================================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TYPE DEFINITIONs>
//
//	<MSG_ONEtoN_BROADCASTER><EXAMPLE>
//		MSG_CG_CONNECTION_HEARTBEAT_SYN msgSYN;
//		MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msgBRD = PACKET_ONEtoN_BROADCASTER::InstanceOfBase( FALSE );
//		msgBRD.Init( &msgSYN, sizeof(msgSYN) );
//		DWORD* pStartPtr = msgBRD.GetUsersStartPtr();
//------(이 부분의 유저 입력은 필요에 따라 수정해야 한다)
//		for( DWORD i=0 ; i<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::MAX_USER_COUNT ; ++i )
//			msgBRD.AddUser( pStartPtr, i+1 );
//------(선택)
//		PACKET_ONEtoN_BROADCASTER::AddOffset( msgBRD.GetSendingSize() );
//------(위와 같은 경우 둘의 크기는 반드시 같다. 보내는 크기 역시 둘 중에 하나 사용이 가능하다)
//		ASSERT( PACKET_ONEtoN_BROADCASTER::GetTotalSize() == msgBRD.GetSendingSize() );
//
//------(Render정보와 같은 한명에게 여러 정보가 가변으로 집산되는 경우 다음과 같은 처리가 가능하다)
//		// _1_에 디폴트 값은 TRUE, 내부적으로 옵셋이 자동계산된다.
//		PLAYER_RENDER_INFO& rRENDER = PACKET_ONEtoN_BROADCASTER::InstanceOf<PLAYER_RENDER_INFO>( _1_ );
//------(따라서 위의 Assert와 다른 반대의 결과가 취해진다)
//		ASSERT( PACKET_ONEtoN_BROADCASTER::GetTotalSize() != msgBRD.GetSendingSize() );
//

typedef VARMSG_EX<VARMSG_T01, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	MSG_ONEtoN_BROADCASTER;
typedef MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN							MSG_AG_CONNECTION_PACKET_UNICASTER;
typedef VARMSG_EX<VARMSG_T02, MSG_AG_CONNECTION_PACKET_UNICASTER>				MSG_COMPOSE_UNICASTER;
typedef VARMSG_EX<VARMSG_T03, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	MSG_ONEtoN_BROADCASTER_INTERNAL;
//typedef VARMSG_EX<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	PACKET_NtoONE_UNICASTER;

#pragma pack(pop)




#endif // __PACKETSTRUCT_AG_H__
