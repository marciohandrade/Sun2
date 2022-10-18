#pragma once

#include <StructInPacket.h>
#ifdef _SECURE_AUTH
    #include "../MatrixLib/include/main.h"
#endif

//#ifdef  _SHAPE_AUTH	
// #include "SATypeDef.h"
//#endif

#pragma pack(push,1)

struct DB_INFO
{
    TCHAR   m_szInitDB[32];
    TCHAR   m_szIpPort[64];
    TCHAR   m_szID[32];
    TCHAR   m_szPW[32];

    DB_INFO()
    {
        ZeroMemory(this, sizeof(*this));
    }
};

struct DYNAMIC_SERVER_DATA
{
    AGENTID m_GameAgentID;
    SHORT   m_LoginUserCnt;
    BYTE    m_CrowdGrade;
    BYTE    m_ConnectStatus;
};

// CHANGES: f101030.1L, change the specification to accept multi-agent on same ip address
struct sGAME_AGENT_INFO
{
    AGENTID game_agent_id_;
    uint16_t game_agent_port_;
    char game_agent_public_ip_[MAX_IP_NAME_LENGTH + 1];
    char game_agent_private_ip_[MAX_IP_NAME_LENGTH + 1];
};

struct CONTROL_IP_INFO
{
    eIPTYPE m_IPType;
    DWORD   m_StartIP;
    WORD    m_Count;
};

// Japan contents : _JP000000_ONETIMEPASSWORD_AUTHFLOW_
struct OtpStep
{
    enum eOtp_Step {
        eOtp_None,          // no OTP user | no auth processing
        eOtp_WaitOtp,       // wait for 'OTP_PASS_SYN'
        eOtp_CheckingOtp,   // db transaction
        eOtp_Authentication,// OTP auth result - succeed
        eOtp_CantAccept,
    };
    BYTE value;
};

//==================================================================================================

struct AccountInfo 
{
    DWORD   m_LoginFrontId; // AuthServer::NetworkFront의 GUID  //< 이거 NetworkIndex == SessionIndex
    DWORD   m_LoginFrontUserId; // LoginFront에서 발급된 연결된 유저 ID
    ACCOUNTSTATUS_TYPE	m_status;

    BYTE    m_userCode[3];				//  Sts값

    DWORD   m_userGuid;
    TCHAR   m_account[ID_MAX_LEN + 1];
    char    m_MD5[kCryptStringSize];
    BYTE    m_serialKey[32];
    DWORD   m_authKey;
    TCHAR   m_clientIp[IP_MAX_LEN];
    TCHAR   m_localIp[IP_MAX_LEN];

    char    m_glogKey[GLOGKEY_LEN];
    BYTE    m_age;

    DWORD   m_pcBangId;
    CHANGE_PCROOM_STATUS m_pcBangStatus; //CHANGE_PCROOM_STATUS::eStatus, _PCBANG_POLICY_CHANGE_POST_EXPIRED
    DWORD   m_pcBangOnRechargeQueryTime; //정량 만료후 쿼리 주기 설정:_PCBANG_POLICY_CHANGE_POST_EXPIRED2

    DWORD   m_tickConnectTime;  //  접속 시작 시간(로그용)
    DWORD   m_tickBillingTime;  //  빌링차감 시간간격
    SYSTEMTIME m_loginTime;

    // Location Info
    AGENTID m_locationId;   //  접속한 서버의 index번호 //< 이거 AgentID
    DWORD   m_tickAuth;     //  인증완료 시각

#ifdef _BILLING_WAITLOGOUT
    DWORD   m_dwWaitLogoutCnt;  // ANS_PC_LOGOUT 대기한 회수
#endif

#ifdef _SECURE_AUTH
    BYTE    m_btMatrixAuthFailCount;    // 매트릭스 인증이 몇번 실패했는지 저장
    bool    m_bSucceedShape; // 형상 인증(형상인증 미사용시는 계정인증여부로 사용)이 완료 되었는지 식별
    SC_INFO m_secureAuthInfo;   // 보안 인증 정보(몇번 좌표를 물어보았는지)
    matrix_seed m_seedValue;    // 시드값
#endif

//#ifdef _PAAS_ALERT
//    BYTE    m_btAlert;  // 로그인 경고 대상을 식별
//#endif

#ifdef _TIRED_CHECK
    BYTE    m_ucRealNameAuth;   //  실명 인증 사용자 식별 (0 : 비실명 , 1 : 실명)
#endif

    DWORD   m_dwLogSeq;

//#ifdef _SHAPE_AUTH
//BYTE  m_btShapeAuthFailCount; // 형상 인증이 몇번 실패했는지 저장
//bool  m_bSucceedAuth;         // 계정 인증이 완료되었는지 식별
//TCHAR m_SA_Value[SA_VALUE_SIZE];
//#endif

//#ifdef _NEW_BILLING_SYSTEM
//long  m_dwBillingGuid;
//#endif

    BYTE m_AuthUserType; //_CHANNELING_AUTH_FLOW_HANGAME_USER_
    nsPacket::ReAuthInfoBase::ReAuthKeyType m_ReAuthKey; //__NA001334_090420_SERVER_SELECT_FACILITY__
    OtpStep m_OtpStep;

    char    m_GameID[10 + 1];   // for Global authentication

#ifdef __NA_20100201_INTERGRATEDBILLING_
    DWORD   IntergratedBillingGUID;
#endif

    bool    m_IsLoginLogWrite;  //로그인 로그를 남기면 TRUE 로 설정한다. 
                                //이후 해당 변수가 TRUE 일때만 로그아웃 로그를 남긴다.

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    DWORD   gamechu_usn_;
#endif

#ifdef __CN_20100909_SECURITY_TOKEN__
	char	token_base64_key_[TOKEN_BASE64_KEY_MAX + 1];
    bool    is_token_use_;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY betakey_;
#endif
#ifdef __PCBANG_IP_UPDATE
    DWORD   pc_room_ip_value_;
#endif

};


//#ifndef _NEW_BILLING_SYSTEM
struct BILLING_DEDUCTION_INFO
{
    DWORD m_dwGameCode; // 전송한 GameServer Code(SUN = 2)
    DWORD m_dwUserGUID;
    DWORD m_dwPCRoomGUID;
    DWORD m_dwIPAddress;
    DWORD m_dwPlayTime; // Millisecond

    //BILLING_DEDUCTION_INFO()
    //{
    //    ZeroMemory(this, sizeof(*this));
    //}
};
//#endif

// (WAVERIX) (090702) (CHANGES) Auth token usage case가 늘어나고 있으므로, 선별 처리로 변경
enum eAuthToken {
    eAuthToken_None,
    eAuthToken_ReAuthToken,
    eAuthToken_AuthTokenKorea,
    eAuthToken_AuthTokenNorthA,
    eAuthToken_AuthTokenGlobal,
    eAuthToken_AuthTokenGameChu,
    eAuthToken_AuthIngamba,
    eAuthToken_AuthChinaZY,
};

#pragma pack(pop)


