#pragma once

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol.h>
#include "./../PacketStruct_ServerCommon.h"

#pragma pack(push,1)


// 서버 선택 - GameAgent 검증 및 미리 알림
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN)
	DWORD			m_dwAuthUserID;
	DWORD			m_dwUserGuid;
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char			m_szID[ID_MAX_LEN + 1];
#else
	char			m_szID[ID_MAX_LEN];
#endif
    char			m_szClientIP[IP_MAX_LEN];
	CHANGE_PCROOM_STATUS m_BillingType;                // CHANGE_PCROOM_STATUS::eStatus reuse by _PCBANG_POLICY_CHANGE_POST_EXPIRED
	char			m_szBillingInfo[INFO_MAX_LEN];
	DWORD			m_dwReservedValue;				// PC방 UID(0 = 일반사용자, 1 ~ PC방)
    BYTE            m_AuthUserType;                 //eAUTH_USER_TYPE(0:일반, 1:한게임유저)
	BYTE			m_serialKey[32];
    BYTE			m_MD5Key[kCryptStringSize];
	BYTE			m_userCode[3];
#ifdef	_TIRED_CHECK
	BYTE			m_byAge;						// 사용자 연령 (게임 서버에서 피로도 측정으로 사용)
	BYTE			m_byRealNameAuth;				// 실명 인증 사용자 식별 (0 : 비실명 , 1 : 실명)
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN      gamechu_usn_;                    //게임츠 GUID
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY         betakey_;
#endif
PACKET_INTERNAL_CLOSE 

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK)
	DWORD			m_dwAuthUserID;
	BYTE			m_byResult;
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char			m_szID[ID_MAX_LEN + 1];
#else
	char			m_szID[ID_MAX_LEN];
#endif
PACKET_INTERNAL_CLOSE

// PC LOGIN, LOGOUT CMD From GameAgent
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_PC_LOGIN_CMD)
	DWORD			m_dwAuthUserID;
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char			m_szAccountID[ID_MAX_LEN + 1];
#else
	char			m_szAccountID[ID_MAX_LEN];
#endif
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_PC_LOGOUT_CMD)
	DWORD			m_dwAuthUserID;
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char			m_szAccountID[ID_MAX_LEN + 1];
#else
	char			m_szAccountID[ID_MAX_LEN];
#endif
	char			m_szPCName[16];
	WORD			m_wPCLevel;
	BYTE			m_byPCClass;
	BYTE			m_byLogoutType;
PACKET_INTERNAL_CLOSE

// PC LOGOUT ASK To GameAgent
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AG_AUTH_ASK_PC_LOGOUT_SYN)
	DWORD			m_dwAuthUserID;	
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char			m_szID[ID_MAX_LEN + 1];
#else
	char			m_szID[ID_MAX_LEN];
#endif
	DWORD			m_dwUserGUID;					// SUN -> GUID, The others -> Reserved
	RTT_DROP_REASON	m_dwLogoutCode;	
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_ASK_PC_LOGOUT_ACK)
    DWORD       m_dwAuthUserID; 
    BYTE        m_byResult;
#ifdef _NA000000_091005_EXTENDED_LOG_
    char        m_CharName[MAX_CHARNAME_LENGTH + 1];
    BYTE        m_CharClass;
    LEVELTYPE   m_CharLevel;
#endif
PACKET_INTERNAL_CLOSE


// 빌링 마감을 GmaeAgent에 알림
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AG_AUTH_BILLING_ALARM_SYN)
	DWORD			m_dwAuthUserID;
	DWORD			m_dwRestTerm;					// 남은 시간 (sec)
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_BILLING_ALARM_ACK)
	DWORD			m_dwAuthUserID;	
	BYTE			m_byResult;						// OK(1)
PACKET_INTERNAL_CLOSE

//==================================================================================================
//_PCBANG_POLICY_CHANGE_POST_EXPIRED
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_BILLING_CHANGE_CONTROL_SYN)
    AGENTID game_agent_id_; // AuthServer <-> AuthAgent
    CHANGE_PCROOM_STATUS status_;   //CHANGE_PCROOM_STATUS::eStatus
    DWORD   pc_room_id_;    // AgentServer는 status_와 조합해서 reserved value설정을 변경한다.
    DWORD   user_key_;
    DWORD   auth_user_key_;
    // Status         -  eStatus_None | eStatus_PcRoom | eStatus_PcRoomExpired
    // pc_room_id     -        0              !0                   !0
    // ResValue in agent       0          pc_room_id                0
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_BILLING_CHANGE_CONTROL_ACK)
    enum eResult {
        eResult_Success,
        eResult_Fail,   // Unknown fail
        eResult_CantFindUser,
    };

    AGENTID game_agent_id_; // AuthServer <-> AuthAgent
    DWORD   user_key_;
    eResult result_;
PACKET_INTERNAL_CLOSE
//_PCBANG_POLICY_CHANGE_POST_EXPIRED
//==================================================================================================

// PC Name과 UserCount NTF From GameAgent
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_PC_NAME_NTF)
	DWORD       m_dwAuthUserID;
#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
    char        m_szAccountID[ID_MAX_LEN + 1];
#else
	char        m_szAccountID[ID_MAX_LEN];
#endif
	char        m_szPCName[PC_MAX_LEN];
#ifdef _NA000000_091005_EXTENDED_LOG_
    BYTE        m_CharClass;
    LEVELTYPE   m_CharLevel;
#endif
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, GA_AUTH_USER_COUNT_NTF)
	WORD			m_wUserCount;
PACKET_INTERNAL_CLOSE


#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_SERVER_SELECT_SYN)
    enum eRequest {
        eRequest_QueryStart,//AU_AUTH_SERVER_SELECT_ACK
        eRequest_Cancel,    //notify
        eRequest_Count,
    };
    AGENTID m_GameAgentID;
    DWORD m_AuthUserID;    
    eRequest m_Request;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_SERVER_SELECT_ACK)
    enum eResult {
        eResult_Success,
        eResult_DefaultFail,
        // etc. error result
        //eResult_~~
        eResult_Count,
    };
    AGENTID m_GameAgentID;
    DWORD   m_AuthUserID;
    BYTE    m_Result;

    nsPacket::ReAuthTokenStream m_ReAuthToken;
    inline WORD GetSize() const {
        if(m_Result == eResult_Success)
            return sizeof(*this);
        return sizeof(*this) - sizeof(m_ReAuthToken);
    }
PACKET_INTERNAL_CLOSE

#endif

//#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_COMMON_QUERY_USER_ID_SYN)
    AGENTID game_agent_id_;
    BYTE action_type_;		//< enum eCHARNAME_QUERY_ACTION_TYPE (assert(eCHARNAME_ACTION_DEFAULT))
    BOOLEAN result_;
    QueryUserIdInfo user_info_;

    inline void Clear() {
        game_agent_id_ = 0;
        action_type_ = 0;
        result_ = 0;
        ZeroMemory(&user_info_, sizeof(user_info_));
    };
    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_COMMON_QUERY_USER_ID_ACK,
                               MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN)
    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_INTERNAL_INHERITED_CLOSE
//#endif //__NA000000_SHOP_USERID_50_PASSING_PROC__

// AgentServer -> AuthAgent
// NOTE: f101030.1L, change the specification to accept multi-agent on same ip address
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN)
    uint16_t listen_port_;
    //
    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_INTERNAL_CLOSE;

// _NA_002455_20110328_AUTH_HEART_BEAT
// AgentServer -> AuthAgent Heartbeat 전송
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_HEART_BEAT_SYN)
    inline WORD GetSize() const 
    {
        return sizeof(*this);
    }
PACKET_INTERNAL_CLOSE;

// _NA_002455_20110328_AUTH_HEART_BEAT
// AuthAgent -> AgentServer Heartbeat 응답
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, AU_AUTH_HEART_BEAT_ACK)
    inline WORD GetSize() const 
    {
        return sizeof(*this);
    }
PACKET_INTERNAL_CLOSE;

#pragma pack(pop)
















