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
	BYTE				m_bIsConnectedWorld;	// ���弭������ ���������� �˷��ش�.
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
    sDOMINATION_CONTINENT_LORD_INFO m_ContinentLordInfo;    //��� ���� ����
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
	INT					m_dwCHuntingVCount;		// �������� �¸�Ƚ��
	INT					m_dwCHuntingTotCount;	// �������� ����Ƚ��
	BYTE				m_byChaoState;			// ī������
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
// �κ� ����
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_LOBBY_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< ������ �� ����
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_HUNTING_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< ������ �� ����
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_MISSION_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
    eZONE_RULESET::eRules m_Rules; //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    //
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< ������ �� ����
    PartyZoneType   party_zone_type; //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_PVP_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< ������ �� ����
	ADDITIONAL_PVPINFO m_AdditionalPVPInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHUNTION_CREATE_CMD )
	MAPCODE			m_MapCode;
	KEYTYPE			m_RoomKey;
	TCHAR			m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	TCHAR			m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO	m_RoomInfo;									//< ������ �� ����
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
	DWORD		m_dwReservedValue;				// ���� (0:�Ϲݻ����, ���������� PC�� ������ UID)
	BYTE		m_bySts;						// ��������(100�̻��� GM���)
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

	BYTE		m_byApplyFieldCode;				// Ư���� ��� �����ϰ�, FieldCode �������� �ʴ� ��츦 �����Ű�� ���� �ڵ� 0, 1(apply)
	GUILDGUID	m_GuildGuid;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_ROOM_JOIN_ACK )
	//�ε� �� ������ ���Ḧ ���� ����...
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
	BYTE		m_ToRoomType;								//������ ������ ��Ÿ��.
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


// �ʵ忡�� �׾�����, ������ �̵� ��Ŷ.
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_ACK )
	WORD		m_wMapCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_VILLAGE_NAK )
	BYTE		m_byErrorCode;
PACKET_AG_CLOSE

// ���� SpecialType�� �ٲ�
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CHANGE_SPECIALTYPE_CMD )
	KEYTYPE		m_RoomKey;
	BYTE		m_SpecialType;		// eROOM_SPECIAL_TYPE
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_TUTORIAL_SKIP_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_TUTORIAL_SKIP_ACK )
PACKET_AG_CLOSE


// ������ �ִ� �ʵ�,��ġ���� ��û
PACKET_AG_OPEN( AG_ZONE, AG_ZONE_RETURN_TO_ROOM_INFO_SYN )
	DWORD			m_dwRequestUserKey;
	WORD			m_wPartyKey;

	inline DWORD&	REQUEST_USER_KEY()		{ return m_dwRequestUserKey; }
	inline WORD&	PARTY_KEY()				{ return m_wPartyKey; }
PACKET_AG_CLOSE

// ������ �ִ� �ʵ�,��ġ���� ��û�� ���� ���� (TRUE, FALSE ����)
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
    {   // ���� �ɼ��� �߰��� ���̴�.
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
// ��Ƽ ����
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CREATE_CMD )
	WORD		m_wPartykey;
	DWORD		m_MasterKey;
	BYTE		m_byItemDistributionType;
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    ePARTY_TYPE m_PartyType;
PACKET_AG_CLOSE

// ��Ƽ �Ҹ�
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_DESTROY_CMD )
	WORD		m_wPartykey;
PACKET_AG_CLOSE

// ��Ƽ ����
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_JOIN_CMD )
	WORD		       m_wPartykey;
	bool		       m_bMaster;
	_PARTY_MEMBER_INFO m_MemberInfo;
PACKET_AG_CLOSE

// ��Ƽ Ż��
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_LEAVE_CMD )
	WORD		m_wPartykey;
	DWORD		m_dwMemberKey;
PACKET_AG_CLOSE

// ��Ƽ�� ����
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CHANGE_MASTER_CMD )
	WORD		m_wPartykey;
	DWORD		m_dwMemberKey;
PACKET_AG_CLOSE

// ��Ƽ �����ۺй� ��� ����
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
    BYTE user_count_;       // �� ��Ŷ�� ����� ���������� ����ִ°�
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    uNode m_Nodes[MAX_PARTYMEMBER_NUM*(eEvent_Count-1)];       // �� ����� HP,MP,SD�� ������ ��� ������ ���,

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
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_PARTY, AG_PARTY_CAN_CHANGE_OPTION_ACK )
    enum eStatus {
        eStatus_PartialFail,// �ش� �÷��̾� ���ǵǾ� ���� ���� �ٸ� ������ ����
        eStatus_PartialOk,// �ش� �÷��̾� ���� ���� ���
        eStatus_Transaction,// �ش� �÷��̾� �� Ʈ����� (��Ÿ ���� ���� Fail)
        eStatus_LevelViolation,// �ش� �÷��̾� ���� ���� ����
        eStatus_RequireQuestClear,// �ش� �÷��̾� ���� Quest �Ϸ� �ʿ�
        eStatus_RequireMissionClear,// �ش� �÷��̾� ���� Mission �Ϸ� �ʿ�
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
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������
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
// ������
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LEVEL_UP_CMD )
	LEVELTYPE			m_Level;			// ���� ����
	WORD				m_wRemainStat;		// ���� ��������Ʈ
	WORD				m_wRemainSkill;		// ���� ��ų����Ʈ
	DWORD				m_dwCurHP;			// ���� HP(�������� HP, MP�� ä���ش�.)
	DWORD				m_dwMaxMP;			// �ִ� MP
	DWORD				m_dwCurMP;			// ���� MP
    DWORD				m_dwMaxAndCurSD;			// ���� SD(�������� SD�� Max�� ä���ش�.)
PACKET_AG_CLOSE

// HP ��ȭ
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

// ������ ����
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
// ���� �ζ� ���ǿ��� ���� ���� ��û
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_SYN )
    CHARGUID iCharGuid;                 // ĳ���� ID
    WORD wDrawNum;                      // ȸ��
    BYTE bAuto;                         // �ڵ� ����
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
    INT64 i64TicketPrice;               // ���ǿ��� ����
PACKET_AG_CLOSE
// ���� �ζ� ���ǿ��� ���� ���� ����
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_ACK )
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
PACKET_AG_CLOSE
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

//__KR_001355_20090608_GROUPINGSYSTEM_GW 
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_ADDEXP_CMD )
    CHARGUID m_CharGuid;            // ĳ���� Guid
    EXPTYPE  m_Amount;              // ����ġ��
    TCHAR    m_szCharName[MAX_CHARNAME_LENGTH+1];//Low memeber name
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_ADDMONEY_CMD )
    CHARGUID m_CharGuid;            // ĳ���� Guid
    MONEY    m_Amount;              // ���ʽ� ����
    TCHAR    m_szCharName[MAX_CHARNAME_LENGTH+1];//Hight memeber name
PACKET_AG_CLOSE
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_EXP_ACQUISITION_CMD )
    CHARGUID m_CharGuid;            // ĳ���� ID
    BYTE     m_byAcquisitionType;   // ȹ�� ���� (eHEIM_ACQUISITION_TYPE)
    EXPTYPE  m_Amount;              // ����ġ��
PACKET_AG_CLOSE

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_HEIM_ACQUISITION_NTF )
    CHARGUID m_CharGuid;            // ĳ���� ID
    BYTE     m_byAcquisitionType;   // ȹ�� ���� (eHEIM_ACQUISITION_TYPE)
    MONEY    m_Amount;              // ���� �ݾ�
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_STATUS_HEIM_CONSUMPTION_NTF )
    CHARGUID m_CharGuid;            // ĳ���� ID
    BYTE     m_byConsumptionType;   // �Һ� ���� (eHEIM_CONSUMPTION_TYPE)
    MONEY    m_Amount;              // ���� �ݾ�
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    int      m_LottoAccumRatio;     // ���� ��� ������
PACKET_AG_CLOSE

// __KR_001355_20090608_GROUPINGSYSTEM_GW
PACKET_AG_OPEN( AG_STATUS, AG_STATUS_GROUP_SET_MEMBERINFO_CMD )
    CHARGUID m_CharGuid;            
    eGROUP_STAT m_HighGroupInfo;
    eGROUP_STAT m_LowGroupInfo;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_STATUS, AG_USER_DOMINATION_RETURNHIEM_CMD  )
    CHARGUID            m_CharGuid;             // ĳ���� Guid
    eSYSTEM_MEMO_TYPE   m_Type;                 // Ÿ��
    ULONGLONG           m_TenderCost;           // ������ ���� | ���� ��ǥ
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// GM ��ɾ�
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_GM, AG_GM_TELEPORT_CMD )
	WzVector			m_vDestPos;			// �̵���ų ��ġ
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_WHISPER_SET_CMD )
	BOOL				m_bOn;				// FALSE: �Ӹ� �� TRUE: �Ӹ� ��
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_USERCHAT_BLOCK_CMD )
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	CHARGUID			m_CharGuid;			// �ش� ����� CharGuid
	BOOL				m_bBlock;			// FALSE: ä�� ��� TRUE: ä�� ����
	DWORD				m_dwBlockTime;		// ä�� ������ ��� ���ѽð�(�� ����)
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_ROOM_INFO_CMD )		//������
	DWORD				m_dwRoomKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_CMD )		//ĳ���� ����
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_SYN )		//ĳ���� ���� ��û
	DWORD				m_dwGMUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_ACK )		//ĳ���� ���� ��û
	DWORD				m_dwGMUserKey;
	TCHAR				m_pszAccountID[ID_MAX_LEN+1];			// ���� ���̵�.
	eCHAR_TYPE			m_eCharType;							// ĳ���� Ŭ����
	WORD				m_wLevel;								// ����
	MONEY				m_Money;								// ���� ��
	KEYTYPE				m_RoomKey;								// ��Ű
	CODETYPE			m_MapCode;								// ���ڵ�
	WzVector			m_vCurPos;								// ���� ��ġ
	TCHAR				m_szGuildName[MAX_GUILDNAME_LENGTH];	// ����̸�
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHAR_INFO_NAK )		//ĳ���� ���� ��û ����
	BYTE				m_byErrorCode;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHANNEL_INFO_CMD )		//ä������ ��û
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_CHANGE_ROOMTITLE_CMD )				//ä������ ��û
	KEYTYPE				m_dwRoomKey;							// ��Ű
	TCHAR				m_szRoomTitle[MAX_ROOMTITLE_LENGTH];	// �ٲ� ������
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_CMD )				// ����� ���� ����
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];		// �����ų ĳ���� �̸�
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_SYN )				// ����� ���� ����
	DWORD				m_dwGMUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_FORCE_DISCONNECT_ACK )				// ����� ���� ����
	DWORD				m_dwGMUserKey;
	DWORD				m_dwDisconnectUserKey;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_SERVER_SHUTDOWN_CMD )				// ����� ���� ����
	DWORD				m_dwStringCode;
	BYTE				m_byShutdownTime;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_GM, AG_GM_GO_CMD )	
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];	// �̵���ų ����̸�(������ �� ���� ����)
	KEYTYPE				m_RoomKey;								// ���ȣ(0�̸� �ʵ尣 �̵���)
	FIELDCODE			m_FieldCode;							// �ʵ��ڵ�
	WzVector			m_vCurPos;								// ���� ��ǥ
	WzVector			m_vDestPos;								// �̵��ϰ����ϴ� ������ ��ǥ

	BYTE				m_PortalType;							// ePORTAL_TYPE
	POSTYPE				m_atItemPos;
    SLOTCODE            m_atItemCode;                           // �ش� �������� ������.

	BOOL IsRevenge()	{ return m_PortalType == ePORTAL_TYPE_REVENGE; }
    BOOL IsFriendInvitatedEvent() { return m_PortalType == ePORTAL_TYPE_FIRENDINVITATED_EVENT; }
	BOOL IsItem()		{ return (m_PortalType == ePORTAL_TYPE_ITEM || m_PortalType == ePORTAL_TYPE_ITEM_EX); }
PACKET_AG_CLOSE

//����
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
    BYTE                   m_flag;                  // 0:�ð� �̺�Ʈ, 1:gm observer
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
// GUILD ���� ��������
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

// ���� ����Ż��
PACKET_AG_OPEN( AG_GUILD, AG_GUILD_KICKPLAYER_ACK )
	TCHAR				m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	CHARGUID			m_CharGuid;	// Ż���'Ų' CharGuid
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// AG_CHUNTING ���� ��������
//----------------------------------------------------------------------------------
PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_ENTER_TEAM_CMD )
	KEYTYPE				m_RoomKey;
	BYTE				m_byReadyPlayerNum;		// �����Ϸ��� �÷��̾� ��
	GUILDGUID			m_GuildGuid;
	DWORD				m_UserKeyList[CHUNTING_MAX_MEMBER_NUM];

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_UserKeyList);
        return static_cast<uint16_t>(kMinSize + sizeof(m_UserKeyList[0]) * m_byReadyPlayerNum);
    };
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_WIN_CMD )
	DWORD				m_WinCount;				// �������� �� �¸�Ƚ��
	DWORD				m_TotCount;				// �������� �� ����
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHUNTING, AG_CHUNTING_START_COMPETITION_CMD )
	DWORD				m_TotCount;				// �������� �� ����
PACKET_AG_CLOSE


//----------------------------------------------------------------------------------
// AG_MISSION ���� ��������
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
// AG_CHAO ���� ��������
//----------------------------------------------------------------------------------

// ������(����ģ��) ����Ʈ �߰� ��û
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_SYN )
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// ������ ����Ʈ�� �߰��� ��� Name
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_ACK )
	DWORD				m_dwRevengeCharGuid;
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_NAK )
	DWORD				m_dwCharGuid;			// ��û�ϴ� ���� CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT ����
PACKET_AG_CLOSE


// ������ ���� ��û
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_SYN )
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_ACK )
	BYTE				m_byRevengePoint;							// ���� Revenge Point
	BYTE				m_byRevengeStatNum;							// ��ϵ� ĳ���� ��
	STRUCT_REVENGE_INFO	m_sRevengeUserInfo[MAX_REVENGE_STAT_NUM];	// ��ϵ� ĳ���� ���

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sRevengeUserInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sRevengeUserInfo[0]) * m_byRevengeStatNum);
    };
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_NAK )
	DWORD				m_dwCharGuid;			// ��û�ϴ� ���� CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT ����
PACKET_AG_CLOSE

// ������ ���� ���� - ������ ����(A->G)
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_CMD )
	DWORD				m_dwRevengeCharGuid;	// ������ ������(����ģ��) CharGuid
PACKET_AG_CLOSE

// ������ ���� - �ش� ���� ���� ��û(G->A)
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_SYN )
	DWORD				m_dwRevengeCharGuid;	// ������ ������(����ģ��) CharGuid
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_ACK )
	DWORD				m_dwRevengeCharGuid;	// ������ ������(����ģ��) CharGuid
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_NAK )
	DWORD				m_dwCharGuid;			// ��û�ϴ� ���� CharGuid
	BYTE				m_byErrorCode;			// eREVENGE_RESULT ����
PACKET_AG_CLOSE

// ī������ ����
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_CHANGE_CHAO_STATE_CMD )
	DWORD				m_dwObjKey;
	BYTE				m_byChaoState;			// ī������(eCHAO_STATE)
PACKET_AG_CLOSE

// ������ ��� ���� ����
PACKET_AG_OPEN( AG_CHAO, AG_CHAO_TRACK_REVENGE_CHAR_ACK )
	POSTYPE				m_atItemPos;			// ������ �� ��ġ
PACKET_AG_CLOSE

//----------------------------------------------------------------------------------
// AG_ITEM ���� ��������
//----------------------------------------------------------------------------------

// �������� �̿��� ��ǥ�̵� ����
PACKET_AG_OPEN( AG_ITEM, AG_ITEM_PORTAL_ACK )
	POSTYPE				m_PortalPos;
    SLOTCODE            m_ItemCode;
PACKET_AG_CLOSE

// ����� ������ ���� ����
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
// G->A ������� ĳ�� ����� ��� ��û
PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN)
    CHARGUID char_guid;     // ĳ���� ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AG_CLOSE

// A->G ������� ĳ�� ����� ��� ����
PACKET_AG_OPEN(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK)
    WORD     result_code;   // ��� �ڵ� : eITEM_RESULT
    CHARGUID char_guid;     // ĳ���� ID
    SLOTIDX  index;
    POSTYPE  pos;
PACKET_AG_CLOSE
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

//----------------------------------------------------------------------------------
// AG_INSTANCE ���� ��������
//----------------------------------------------------------------------------------

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN )
	POSTYPE				m_ItemPos;
PACKET_AG_CLOSE

PACKET_AG_OPEN( AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK )
	MAPCODE				m_MapCode;
	BOOL				m_UseCharge;	// ĳ�� ��뿩�� �ʵ忴����, �������Ѵ�. �ð� ���� ���� ����� �÷�
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
	BASE_ROOMINFO		m_RoomInfo;									//< ������ �� ����
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
    SLOTCODE require_item_;//��Ƽ�� ������ �ʿ��� ������
PACKET_AG_CLOSE

PACKET_AG_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK)
    enum eResult {
        eResult_NoMemberFail,// �÷��̾� �˻� ��ü�� ���� ��Ȳ��.
        eResult_InvalidMap,// ��û�� ���� ���ų� ���尡�� Ÿ���� �ƴϴ�.
        eResult_CheckedMembers,// �ش� �÷��̾���� �˻���� �����ߴ�.
        eResult_FailCheckTerms,//�ʿ� �������� �� ���� ���ǿ� ���� �ʴ�.
        eResult_RequireQuestClear,//���� ����Ʈ�� �����ؾ� �Ѵ�.
        eResult_RequireMissionClear,//���� �̼��� �����ؾ� �Ѵ�.
    };
    enum eStatus {
        eStatus_PartialFail,// �ش� �÷��̾� ���ǵǾ� ���� ���� �ٸ� ������ ����
        eStatus_PartialOk,// �ش� �÷��̾� ���� ���� ���
        eStatus_Transaction,// �ش� �÷��̾� �� Ʈ����� (��Ÿ ���� ���� Fail)
        eStatus_LevelViolation,// �ش� �÷��̾� ���� ���� ����
        eStatus_RequireQuestClear,// �ش� �÷��̾� ���� Quest �Ϸ� �ʿ�
        eStatus_RequireMissionClear,// �ش� �÷��̾� ���� Mission �Ϸ� �ʿ�
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
        eCheck_Party    = eCheck_None,  // ��Ƽ ���� ��Ƽ������ �������� �˻�
        eCheck_Members  ,               // ÷�εǴ� �÷��̾���� ���� ���� �˻�
    };
    enum eResult {
        eResult_NoMemberFail        ,   // �÷��̾� �˻� ��ü�� ���� ��Ȳ��.
        eResult_InvalidMap          ,   // ��û�� ���� ���ų� ���尡�� Ÿ���� �ƴϴ�.
        eResult_CheckedMembers      ,   // �ش� �÷��̾���� �˻���� �����ߴ�.
    };
    enum eStatus {
        eStatus_PartialFail         ,   // �ش� �÷��̾� ���ǵǾ� ���� ���� �ٸ� ������ ����
        eStatus_PartialOk           ,   // �ش� �÷��̾� ���� ���� ���
        eStatus_Transaction         ,   // �ش� �÷��̾� �� Ʈ����� (��Ÿ ���� ���� Fail)
        eStatus_LevelViolation      ,   // �ش� �÷��̾� ���� ���� ����
        eStatus_RequireQuestClear   ,   // �ش� �÷��̾� ���� Quest �Ϸ� �ʿ�
        eStatus_RequireMissionClear ,   // �ش� �÷��̾� ���� Mission �Ϸ� �ʿ�
    };
    struct MemberInfo
    {
        WORD    objectKey;
        WORD    status;     // eResult
    };
    struct CheckArgParty
    {
        // SYN->ACK passing arg: GameServer�� �ش� ��Ƽ������� MemberResult�� �����Ѵ�.
        WORD    partyKey;
    };
    struct CheckArgMembers : public CheckArgParty
    {
        // {partyKey, roomKey} Member���� ������. �ش� (��Ƽ��/��) ��Ͽ��� �˻� ����.
        WORD    roomKey;    // SYN->ACK passing arg
        // �ݵ�� �ش� �迭(members)�� �ʱ�ȭ �ϰ� ������ ��. 0�̸� end
        MemberInfo  members[max(MAX_FIELD_PARTYMEMBER_NUM, MAX_PARTYMEMBER_NUM)];
    };

    //////////////////////////////////////////////////////////////////////////
    BYTE    m_CheckMethod;  // eCheckMethod �˻� ���
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
            //gamelulu's note : ���� Party���� �κ� ó���� ������. ������ Member�κ� üũ�� �־��
            //��Ȯ�� ���� ��Ŷ�� �ƴ� �������� ���� ���캼 �ʿ䰡 �־ ���� ������ �����ϰ� ó��
            return WORD(sizeof(*this));
        }
        return WORD(MinSize);   // Assert Violation...
    }
PACKET_AG_CLOSE

PACKET_AG_INHERIT_OPEN(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_ACK, MSG_AG_ZONE_CAN_CREATE_ROOM_SYN)
    // Ack�� SYN����� �����ϰ� ó���Ѵ�. ���� �������� �����Ƿ� m_CheckMethod == eCheck_Members;
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
    GUILDGUID win_guild_guid;                                            // ��
    GUILDGUID lose_guild_guid;                                           // ��
    MAPCODE   map_code;                                            // ��������
    char win_guild_name[MAX_GUILDNAME_LENGTH+1];
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_AG_CLOSE
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

PACKET_AG_INHERIT_OPEN( AG_EVENT, AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN, MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD )
PACKET_AG_INHERIT_CLOSE

//W->A �ʴ��� ĳ���� ��� (����)
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
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_RECALL_ACK) //������Ʈ���� ���� ������ ���� ����� ����
    CHARGUID    CalleeGUID;
PACKET_AG_INHERIT_CLOSE

//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_RECALL_NAK) //������Ʈ���� ���� ������ ���� ����� ����
    BYTE    ErrorCode;
PACKET_AG_INHERIT_CLOSE

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
PACKET_AG_OPEN( AG_EVENT, AG_EVENT_SYSTEMMEMO_SEND_CMD) 
    DWORD system_memo_list_num_;//�ý��� �޸� ����
    BasicSystemMemoInfo system_memo_info_[MAX_SYSTEM_MEMO_LIST];//�ý��� �޸� ���� ����Ʈ

    DWORD GetSize() const
    {
        assert(system_memo_list_num_ <= _countof(system_memo_info_));
        return (sizeof(*this) - sizeof(system_memo_info_)) +        //���� ũ��
               (sizeof(system_memo_info_[0]) * system_memo_list_num_);//���� ũ��
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

//������������������������������������������������������������������������������������������������
//
//	<PACKET BROADCAST FUNCTIONs>
//
//
//
#include <VarPacket.h>


//������������������������������������������������������������������������������������������������
//	���۽� ����ϴ� ��Ŷ ���� (���� ��Ŷ �Է½�)
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
    // Init, InitByArray ����, AddUserȣ�� ������ �߰���Ű�� ����̴�.
    void AppendMsg(uint8_t* msg, uint16_t msg_size);
    MSG_BASE_FORWARD* GetBrdPacketPtr();
    uint16_t* GetUsersStartPtr();
    uint16_t AddUser(uint16_t* const user_begin, DWORD object_key);
    uint16_t GetSendingSize() const;

    // <��Ŷ ���Ž� ����ϴ� METHOD>
    // <returns>
    //  size of variable msg - <WARNING> �� ���� ���� ���ŵ� ���� ���ؼ� ��ġ���� ������ ó���ؼ� �ȵȴ�.
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

//	��/���Ž� ����ϴ� ��Ŷ ���� (���� ��Ŷ)
//	�۽Ž� ��Ŷ�� Ư�� ���۸� �������� ����ؾ� �Ѵ�.
PACKET_AG_OPEN( AG_CONNECTION, AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN )
//////////////////////////////////////////////////////////////////////////
// <FIELDs>
	enum { MAX_USER_COUNT = 255 };
	struct MSG_INFO
	{
		MSG_BASE_FORWARD*	pMSG_BODY;
		WORD				wSIZE;
		WORD				wUSER_COUNT;
		DWORD*				pUSERs;	//	(WAVERIX)(080704) - UserKey�� �ƴ϶� ObjectKey�� ���� <-__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__ �����
	};

	BYTE		m_byUserCount;
	WORD		msgBODY_SZ;
	// ~~
	// ~~

	//	<��Ŷ ���۽� ����ϴ� METHODs>
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

	//	Init, InitByArray ����, AddUserȣ�� ������ �߰���Ű�� ����̴�.
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

	//	<��Ŷ ���Ž� ����ϴ� METHOD>
	//	<returns>
	//		size of variable msg - <WARNING> �� ���� ���� ���ŵ� ���� ���ؼ� ��ġ���� ������ ó���ؼ� �ȵȴ�.
	//		MSG_INFO.pMSG_BODY = BROADCASTING MSG
	//		MSG_INFO.wSIZE = BROADCASTING MSG SIZE
	inline WORD			GetPacketInfo( MSG_INFO& rINFO )
	{
		//
		const WORD cwMIN_SZ = sizeof(MSG_BASE_FORWARD) + sizeof(msgBODY_SZ) + sizeof(m_byUserCount);

		BYTE* pOFFSET = (BYTE*)(((WORD*)&msgBODY_SZ)+1);
		rINFO.pMSG_BODY = (MSG_BASE_FORWARD*)pOFFSET;	//�ּ��� �������� �ߴ� �޽��� ������ Ȯ�� ����
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

//������������������������������������������������������������������������������������������������
//	<TYPE DEFINITIONs>
//
//	<MSG_ONEtoN_BROADCASTER><EXAMPLE>
//		MSG_CG_CONNECTION_HEARTBEAT_SYN msgSYN;
//		MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msgBRD = PACKET_ONEtoN_BROADCASTER::InstanceOfBase( FALSE );
//		msgBRD.Init( &msgSYN, sizeof(msgSYN) );
//		DWORD* pStartPtr = msgBRD.GetUsersStartPtr();
//------(�� �κ��� ���� �Է��� �ʿ信 ���� �����ؾ� �Ѵ�)
//		for( DWORD i=0 ; i<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::MAX_USER_COUNT ; ++i )
//			msgBRD.AddUser( pStartPtr, i+1 );
//------(����)
//		PACKET_ONEtoN_BROADCASTER::AddOffset( msgBRD.GetSendingSize() );
//------(���� ���� ��� ���� ũ��� �ݵ�� ����. ������ ũ�� ���� �� �߿� �ϳ� ����� �����ϴ�)
//		ASSERT( PACKET_ONEtoN_BROADCASTER::GetTotalSize() == msgBRD.GetSendingSize() );
//
//------(Render������ ���� �Ѹ��� ���� ������ �������� ����Ǵ� ��� ������ ���� ó���� �����ϴ�)
//		// _1_�� ����Ʈ ���� TRUE, ���������� �ɼ��� �ڵ����ȴ�.
//		PLAYER_RENDER_INFO& rRENDER = PACKET_ONEtoN_BROADCASTER::InstanceOf<PLAYER_RENDER_INFO>( _1_ );
//------(���� ���� Assert�� �ٸ� �ݴ��� ����� ��������)
//		ASSERT( PACKET_ONEtoN_BROADCASTER::GetTotalSize() != msgBRD.GetSendingSize() );
//

typedef VARMSG_EX<VARMSG_T01, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	MSG_ONEtoN_BROADCASTER;
typedef MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN							MSG_AG_CONNECTION_PACKET_UNICASTER;
typedef VARMSG_EX<VARMSG_T02, MSG_AG_CONNECTION_PACKET_UNICASTER>				MSG_COMPOSE_UNICASTER;
typedef VARMSG_EX<VARMSG_T03, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	MSG_ONEtoN_BROADCASTER_INTERNAL;
//typedef VARMSG_EX<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>	PACKET_NtoONE_UNICASTER;

#pragma pack(pop)




#endif // __PACKETSTRUCT_AG_H__
