#ifndef __AUTH_PROTOCOL_CF_H__
#define __AUTH_PROTOCOL_CF_H__

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol.h>
#include <SunAuth/AuthStruct.h>

#pragma pack(push,1)


//--------------------------------------------------------------------------------------------------
// 접속후 LoginFront에서 최초로 클라이언트로 보냄
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_READY_CMD )
	char				m_szServerInfo[INFO_MAX_LEN];
	DWORD				m_dwEncKey;
PACKET_INTERNAL_CLOSE

//--------------------------------------------------------------------------------------------------
// 패치버전 요청
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_REQ_PATCH_VER_SYN )
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_REQ_PATCH_VER_ACK )
	GAME_VERSION		m_verLauncher;
	GAME_VERSION		m_verMajor;
	GAME_VERSION		m_verMinor;
PACKET_INTERNAL_CLOSE


//--------------------------------------------------------------------------------------------------
// 클라이언트가 접속 요청(버전확인)
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_VERIFY_SYN )
	BYTE				m_byHighVersion;
	BYTE				m_byMiddleVersion;
	BYTE				m_byLowVersion;			
	char				m_szLocalIP[IP_MAX_LEN];
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_VERIFY_ACK )
	BYTE				m_byResult;
PACKET_INTERNAL_CLOSE

//--------------------------------------------------------------------------------------------------
// ID, PW 체크 - START!
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_ID_PASSWORD_CHECK_SYN )
	DWORD				m_dwAuthUserID;				// 인증 서버에서 발급한 아이디
#ifdef _ACCOUNT_ID_LENGTH_EXTENTION_
    char				m_szID[ID_MAX_LEN + 1];
#else
	char				m_szID[ID_MAX_LEN];
#endif
	char				m_szPasswd[PASSWD_MAX_LEN];
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_ID_PASSWORD_CHECK_ACK )
	BYTE				m_byResult;					// 결과, OK(0) 그 외 에러 코드표 참조
	BYTE				m_byFailCount;				// 연속 인증오류 회수( byResult값이 인증 실패일경우 참조, 그 외엔 사용하지 않는다 )
	char				m_szInfo[INFO_MAX_LEN];
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    char				m_szID[ID_MAX_LEN + 1];
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	bool				m_Is_Token_Use;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY             m_betakey_;
#endif
PACKET_INTERNAL_CLOSE

//--------------------------------------------------------------------------------------------------
//#ifdef __KR000000_090128_CHANNELING__
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_TOKEN_PASS_SYN )
    enum { MAX_STREAM_SIZE = 320, };
    struct Stream {
        DWORD   type_code;
        BYTE    stream[MAX_STREAM_SIZE];
    };
    Stream              info_stream;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN( CATEGORY_AUTH, FC_AUTH_TOKEN_PASS_ACK, MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK )
PACKET_INTERNAL_INHERITED_CLOSE
//#endif //__KR000000_090128_CHANNELING__

//--------------------------------------------------------------------------------------------------
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_REAUTH_PASS_SYN)
    enum { MAX_STREAM_SIZE = nsPacket::ReAuthTokenStream::MAX_STREAM_SIZE, };
    nsPacket::ReAuthTokenStream   info_stream;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FC_AUTH_REAUTH_PASS_ACK, MSG_FC_AUTH_TOKEN_PASS_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
#endif

//--------------------------------------------------------------------------------------------------
//#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_TOKEN_NORTHA_PASS_SYN)
    enum { MAX_STREAM_SIZE = nsPacket::HanAuthTokenStream::MAX_AUTHKEY_STREAM_SIZE, };
    nsPacket::HanAuthTokenStream    info_stream;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FC_AUTH_TOKEN_NORTHA_PASS_ACK,
                               MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
//#endif

//--------------------------------------------------------------------------------------------------
//#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_TOKEN_GLOBAL_PASS_SYN)
    //ref) 동일 프로토콜에 CF_AUTH_TOKEN_PASS_SYN과의 구별에 크기도 중요요소
    // for 8byte align
    enum { MAX_STREAM_SIZE = 128, };
    struct Stream {
        DWORD   type_code;
        BYTE    stream[MAX_STREAM_SIZE];
    };
    Stream      info_stream;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FC_AUTH_TOKEN_GLOBAL_PASS_ACK,
                               MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
//#endif //__NA000000_GLOBAL_AUTH_PROCESS__

//#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_TOKEN_JP_GAMECHU_SYN)
    GameChuInfoStream gamechu_info_;
PACKET_INTERNAL_CLOSE
//#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_PASS_RU_INGAMBA_SYN)
    IngambaAuthInfo ingamba_auth_info_;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FC_AUTH_PASS_RU_INGAMBA_ACK,
                               MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
#endif

#ifdef _CN_ZY_AUTH_PROCESS
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, CF_AUTH_ZY_PASSWORD_CHECK_SYN, MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN)
PACKET_INTERNAL_INHERITED_CLOSE
#endif //_CN_ZY_AUTH_PROCESS

//--------------------------------------------------------------------------------------------------
//#ifdef _SHAPE_AUTH
//// ID, PW 체크 - 형상 인증
//struct MSG_FC_AUTH_SHAPE_SYN : public MSG_BASE
//{
//	BYTE				m_btResult;
//	BYTE				m_btAuthFailCount;
//	BYTE				m_btDataSts;
//	DWORD				m_dwShapeDataSize;
//	BYTE				m_szShapeData[SHAPE_DATA_MAX_LEN];
//
//	MSG_FC_AUTH_SHAPE_SYN()
//	{
//		m_byCategory = CATEGORY_AUTH;
//		m_byProtocol = FC_AUTH_SHAPE_SYN;
//		m_dwShapeDataSize = 0;
//		m_btResult = m_btAuthFailCount = m_btDataSts = 0;
//		memset( m_szShapeData, 0, SHAPE_DATA_MAX_LEN * sizeof(BYTE) );
//	}
//};
//
//PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_SHAPE_ACK )
//	TCHAR				m_SA_Value[SA_VALUE_SIZE];
//PACKET_INTERNAL_CLOSE
//
//#endif	// _SHAPE_AUTH

//==================================================================================================

#ifdef _SECURE_AUTH

// ID, PW 체크 - 매트릭스 카드 인증
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_MATRIX_CARD_SYN )
    BYTE    m_byResult;             //  결과값(0: 보안 인증 실패하여 재요청 1: 보안 인증이 처음)
    SECURE_INFO m_SecureInfo;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_MATRIX_CARD_ACK )
    SECURE_INFO m_SecureInfo;
PACKET_INTERNAL_CLOSE

#endif // _SECURE_AUTH

//==================================================================================================
//_JP000000_ONETIMEPASSWORD_AUTHFLOW_

// S -> S -> C (show OTP windows, ref: packet)
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_OTP_PASS_CMD)
    enum eCommand {
        eCommand_None,
        eCommand_AuthSuccess_HasNotOneTimePassword, // 1차 인증 성공, OTP 미사용 유저
        eCommand_Require_OneTimePassword, // OTP 사용 유저, 2차 인증(OTP) 정보 요구
        eCommand_AuthSuccess_NoOtpEvent,  // 1차 인증 성공, OTP 미 서비스 서버이며, 관련 OTP 이벤트 통과
    };
    BYTE command;
PACKET_INTERNAL_CLOSE

// C -> S -> S (retry with OTP)
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_OTP_PASS_SYN)
    char info_stream[98];
PACKET_INTERNAL_CLOSE

// S -> S -> C (if success then next, if fail then retry 'show OTP window')
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_OTP_PASS_ACK)
    enum eResult {
        eResult_Fail,   // Irregular fail
        eResult_AuthSuccess, // OTP 사용 유저, 2차 인증(OTP) 성공
        eResult_OtpExpired, // OTP 사용 유저, OTP 유효기간 만료(3분 오버) with re-issued OTP
        eResult_Retry,  // not matched OTP, retry to 'CU_AUTH_OTP_PASS_SYN'
    };
    BYTE result;
PACKET_INTERNAL_CLOSE

//// no specification, reserved slot
//PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CU_AUTH_OTP_PASS_RESERVED_SLOT)
//PACKET_INTERNAL_CLOSE

//==================================================================================================

//==================================================================================================
//__CN_20100909_SECURITY_TOKEN__

// C -> S -> S
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_SECURITY_TOKEN_SYN)
	SecurityTokenCode security_token_info_;
PACKET_INTERNAL_CLOSE

// S -> S -> C
PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CF_AUTH_SECURITY_TOKEN_ACK)
	enum eResult {
		eResult_Fail,
		eResult_Success,		
	};
	BYTE result_;
PACKET_INTERNAL_CLOSE

//// no specification, reserved slot
//PACKET_INTERNAL_OPEN(CATEGORY_AUTH, CU_AUTH_OTP_PASS_RESERVED_SLOT)
//PACKET_INTERNAL_CLOSE

//==================================================================================================

// 서버 목록 요청
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_SERVER_LIST_SYN )

PACKET_INTERNAL_CLOSE

// 서버 목록 응답(월드)
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_SERVER_LIST_WORLD_ACK )
	BYTE				m_Count;
	GROUP_DATA			m_GroupDataList[MAX_GROUP];
	int GetSize()
	{
		return sizeof(*this) - (MAX_GROUP-m_Count)*sizeof(GROUP_DATA);
	}
PACKET_INTERNAL_CLOSE

// 서버 목록 응답(채널)
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_SERVER_LIST_CHANNEL_ACK )
	BYTE				m_Count;
	SERVER_DATA			m_ServerDataList[MAX_SVR];
	int GetSize()
	{
		return sizeof(*this) - (MAX_SVR-m_Count)*sizeof(SERVER_DATA);
	}
PACKET_INTERNAL_CLOSE

// 서버 선택
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, CF_AUTH_SERVER_SELECT_SYN )
	BYTE				m_GroupID;
	BYTE				m_ChannelID;
PACKET_INTERNAL_CLOSE

// 선택한 서버의 상세 정보
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FC_AUTH_SERVER_SELECT_ACK )
	DWORD				m_dwAuthUserID;
	BYTE				m_serialKey[32];
	char				m_szSvrIP[IP_MAX_LEN];
	DWORD				m_dwSvrPort;
	BYTE				m_byResult;					// 성공(0) 서버다운(1)
	char				m_szGLogKey[GLOGKEY_LEN];
PACKET_INTERNAL_CLOSE

#pragma pack(pop)

#endif //__AUTH_PROTOCOL_CF_H__

