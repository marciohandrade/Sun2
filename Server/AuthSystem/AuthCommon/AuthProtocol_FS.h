#pragma once

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol.h>
#include <SunAuth/AuthProtocol_CF.h>

#pragma pack(push,1)


// ID, PW 체크
//#ifdef __KR000000_090128_CHANNELING__

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_ID_PASSWORD_CHECK_SYN,
                               MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_INHERITED_CLOSE

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, FS_AUTH_TOKEN_PASS_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
    MSG_CF_AUTH_TOKEN_PASS_SYN::Stream  info_stream;
PACKET_INTERNAL_CLOSE

//PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, SF_AUTH_TOKEN_PASS_ACK,
//                               MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK)
//PACKET_INTERNAL_INHERITED_CLOSE

//#else
//    PACKET_INTERNAL_OPEN(CATEGORY_AUTH, FS_AUTH_ID_PASSWORD_CHECK_SYN)
//        DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
//        DWORD   m_dwAuthUserID;             // 인증 서버에서 발급한 아이디
//        char    m_szID[ID_MAX_LEN + 1];
//        char    m_szPasswd[PASSWD_MAX_LEN];
//        char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
//        char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
//        PACKET_INTERNAL_CLOSE
//#endif

PACKET_INTERNAL_OPEN(CATEGORY_AUTH, SF_AUTH_ID_PASSWORD_CHECK_ACK)
    DWORD   m_dwFrontUserID;
    BYTE    m_byResult;     // AUTHRESULT_TYPE 참조
    BYTE    m_byFailCount;  // 연속 인증오류 회수( byResult값이 인증 실패일경우 참조, 그 외엔 사용하지 않는다 )
    char    m_szInfo[MAX_AUTHRESULT_MESSAGE_LEN];   // 에러 정보 문자열
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    char	m_szID[ID_MAX_LEN + 1];
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	bool	m_Is_Token_Use;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY m_betakey_;
#endif
PACKET_INTERNAL_CLOSE


#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_REAUTH_PASS_SYN, MSG_CF_AUTH_REAUTH_PASS_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_INHERITED_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, SF_AUTH_REAUTH_PASS_ACK,
                               MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
#endif

//#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_TOKEN_NORTHA_PASS_SYN,
                               MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_INHERITED_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, SF_AUTH_TOKEN_NORTHA_PASS_ACK,
                               MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
//#endif

//#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_TOKEN_GLOBAL_PASS_SYN,
                               MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_INHERITED_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, SF_AUTH_TOKEN_GLOBAL_PASS_ACK,
                               MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
//#endif

//#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_TOKEN_JP_GAMECHU_SYN,
                               MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN)
    DWORD   m_dwFrontUserID;            // LoginFront에서 발급한 식별자
    char    m_szClientIP[IP_MAX_LEN];   // 클라이언트 아이피
    char    m_szLocalIP[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_CLOSE
//#endif

//_RU_INGAMBA_AUTH_PROCESS
//dev : FS SYB, SF ACK 구조체 정의
#ifdef _RU_INGAMBA_AUTH_PROCESS 
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, FS_AUTH_PASS_RU_INGAMBA_SYN,
                               MSG_CF_AUTH_PASS_RU_INGAMBA_SYN)
    DWORD front_user_id_;             // LoginFront에서 발급한 식별자
    char client_ip_[IP_MAX_LEN];   // 클라이언트 아이피
    char local_ip_[IP_MAX_LEN];    // 클라이언트가 전송한 IP
PACKET_INTERNAL_INHERITED_CLOSE

PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, SF_AUTH_PASS_RU_INGAMBA_ACK,
                               MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK)
PACKET_INTERNAL_INHERITED_CLOSE
#endif

#ifdef _CN_ZY_AUTH_PROCESS
PACKET_INTERNAL_INHERITED_OPEN( \
    CATEGORY_AUTH, FS_AUTH_ZY_PASSWORD_CHECK_SYN, MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN)
PACKET_INTERNAL_INHERITED_CLOSE
#endif //_CN_ZY_AUTH_PROCESS

//#ifdef _SHAPE_AUTH
//// ID, PW 체크 - 형상 인증
//struct MSG_SF_AUTH_SHAPE_SYN : public MSG_BASE
//{
//	DWORD				m_dwFrontUserID;
//	BYTE				m_btResult;								// 결과값(0: 형상 인증 실패하여 재요청 1: 형상 인증이 처음)
//	BYTE				m_btAuthFailCount;						// 연속 오류 횟수(계정, 형상 인증 오류를 포함한 횟수임)
//	BYTE				m_btDataSts;							// 0 : 전송시작, 1 : 데이터전송, 2 : 전송완료(Sts가 0, 2 인경우 데이터는 보내지 않는다)
//	DWORD				m_dwShapeDataSize;						// Sts가 0 일때 총 데이터의 크기가 얼마나 되는지 알려준다(Sts가 1일 경우는 사용된 버퍼의 크기만큼만 넣어준다)
//	BYTE				m_szShapeData[SHAPE_DATA_MAX_LEN];
//
//	MSG_SF_AUTH_SHAPE_SYN()
//	{
//		m_byCategory = CATEGORY_AUTH;
//		m_byProtocol = SF_AUTH_SHAPE_SYN;
//		m_dwFrontUserID = m_dwShapeDataSize = 0;
//		m_btResult = m_btAuthFailCount = m_btDataSts = 0;
//		memset( m_szShapeData, 0, SHAPE_DATA_MAX_LEN * sizeof(BYTE) );
//	}
//};
//
//struct MSG_FS_AUTH_SHAPE_ACK : public MSG_BASE
//{
//	DWORD				m_dwFrontUserID;
//	TCHAR				m_SA_Value[SA_VALUE_SIZE];
//
//	MSG_FS_AUTH_SHAPE_ACK()
//	{
//		m_byCategory = CATEGORY_AUTH;
//		m_byProtocol = FS_AUTH_SHAPE_ACK;
//		m_dwFrontUserID = 0;
//		memset( m_SA_Value, 0, SA_VALUE_SIZE * sizeof(TCHAR) );
//	}
//};
//
//#endif	// _SHAPE_AUTH

#ifdef _SECURE_AUTH
// ID, PW 체크 - 매트릭스 카드 인증
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_MATRIX_CARD_SYN )
	DWORD				m_dwFrontUserID;						//  LoginFront에서 발급한 식별자
	BYTE				m_byResult;								//  결과값(0: 보안 인증 실패하여 재요청 1: 보안 인증이 처음)
	SECURE_INFO			m_SecureInfo;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FS_AUTH_MATRIX_CARD_ACK )
	DWORD				m_dwFrontUserID;						//  LoginFront에서 발급한 식별자
	SECURE_INFO			m_SecureInfo;
PACKET_INTERNAL_CLOSE

#endif	// _SECURE_AUTH

//==================================================================================================
//_JP000000_ONETIMEPASSWORD_AUTHFLOW_

// S -> S -> C (show OTP windows, ref: packet)
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_OTP_PASS_CMD, MSG_CF_AUTH_OTP_PASS_CMD)
    DWORD front_user_id;
PACKET_INTERNAL_INHERITED_CLOSE

// C -> S -> S (retry with OTP)
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_OTP_PASS_SYN, MSG_CF_AUTH_OTP_PASS_SYN)
    DWORD front_user_id;
PACKET_INTERNAL_INHERITED_CLOSE

// S -> S -> C (if success then next, if fail then retry 'show OTP window')
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_OTP_PASS_ACK, MSG_CF_AUTH_OTP_PASS_ACK)
    DWORD front_user_id;
PACKET_INTERNAL_INHERITED_CLOSE

//==================================================================================================

//==================================================================================================
//__CN_20100909_SECURITY_TOKEN__

// S -> S -> C (show OTP windows, ref: packet)
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_SECURITY_TOKEN_SYN, MSG_CF_AUTH_SECURITY_TOKEN_SYN)
	DWORD front_user_id;
PACKET_INTERNAL_INHERITED_CLOSE

// S -> S -> C (if success then next, if fail then retry 'show OTP window')
PACKET_INTERNAL_INHERITED_OPEN(CATEGORY_AUTH, AU_AUTH_SECURITY_TOKEN_ACK, MSG_CF_AUTH_SECURITY_TOKEN_ACK)
DWORD front_user_id;
PACKET_INTERNAL_INHERITED_CLOSE

//==================================================================================================

// 서버 선택
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FS_AUTH_SERVER_SELECT_SYN )
	DWORD				m_dwFrontUserID;
	BYTE				m_GroupID;
	BYTE				m_ChannelID;
	BOOL				m_IsOfficeIP;							//  사내IP인지 아닌지(GM 체크 위해 필요)
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_SERVER_SELECT_ACK )
	DWORD				m_dwFrontUserID;
	DWORD				m_dwAuthUserID;
	char				m_szSvrIP[IP_MAX_LEN];
	DWORD				m_dwSvrPort;
	BYTE				m_serialKey[32];
	BYTE				m_byResult;								//  성공 (0) 서버다운(1) 중복 (2)
	char				m_szGLogKey[GLOGKEY_LEN];				//  GLog 인증키
PACKET_INTERNAL_CLOSE

// 서버 목록
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_SERVER_LIST_NTF )
	BYTE				m_Count;
	DYNAMIC_SERVER_DATA	m_ServerDataList[MAX_SVR];
	int GetSize()
	{
		return sizeof(*this) - (MAX_SVR-m_Count)*sizeof(DYNAMIC_SERVER_DATA);
	}
PACKET_INTERNAL_CLOSE

// Client Disconnect 통지 받음 From LoginFront
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, FS_AUTH_CLIENT_DISCONNECT_CMD )
	DWORD				m_dwFrontUserID;
PACKET_INTERNAL_CLOSE

// Client Disconnect 명령 내림 To LoginFront
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_CLIENT_DISCONNECT_CMD )
	DWORD				m_dwFrontUserID;
PACKET_INTERNAL_CLOSE

// NTF To LoginFront
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_GAME_VERSION_NTF )
	GAME_VERSION		m_verProtocol;
	GAME_VERSION		m_verLauncher;
	GAME_VERSION		m_verMajor;
	GAME_VERSION		m_verMinor;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SF_AUTH_CONTROL_IPLIST_NTF )
	enum { _MAX_IPDATA_NUM = 500 };
	BYTE				m_Flag;									//  0 : 최초 전송, 1 : 이어받기, 2 : 종료
	WORD				m_Count;
	CONTROL_IP_INFO		m_IPDataList[_MAX_IPDATA_NUM];
	int GetSize()
	{
		return sizeof(*this) - (_MAX_IPDATA_NUM-m_Count)*sizeof(CONTROL_IP_INFO);
	}
PACKET_INTERNAL_CLOSE



#pragma pack(pop)





























