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
    DWORD   m_LoginFrontId; // AuthServer::NetworkFront�� GUID  //< �̰� NetworkIndex == SessionIndex
    DWORD   m_LoginFrontUserId; // LoginFront���� �߱޵� ����� ���� ID
    ACCOUNTSTATUS_TYPE	m_status;

    BYTE    m_userCode[3];				//  Sts��

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
    DWORD   m_pcBangOnRechargeQueryTime; //���� ������ ���� �ֱ� ����:_PCBANG_POLICY_CHANGE_POST_EXPIRED2

    DWORD   m_tickConnectTime;  //  ���� ���� �ð�(�α׿�)
    DWORD   m_tickBillingTime;  //  �������� �ð�����
    SYSTEMTIME m_loginTime;

    // Location Info
    AGENTID m_locationId;   //  ������ ������ index��ȣ //< �̰� AgentID
    DWORD   m_tickAuth;     //  �����Ϸ� �ð�

#ifdef _BILLING_WAITLOGOUT
    DWORD   m_dwWaitLogoutCnt;  // ANS_PC_LOGOUT ����� ȸ��
#endif

#ifdef _SECURE_AUTH
    BYTE    m_btMatrixAuthFailCount;    // ��Ʈ���� ������ ��� �����ߴ��� ����
    bool    m_bSucceedShape; // ���� ����(�������� �̻��ô� �����������η� ���)�� �Ϸ� �Ǿ����� �ĺ�
    SC_INFO m_secureAuthInfo;   // ���� ���� ����(��� ��ǥ�� ����Ҵ���)
    matrix_seed m_seedValue;    // �õ尪
#endif

//#ifdef _PAAS_ALERT
//    BYTE    m_btAlert;  // �α��� ��� ����� �ĺ�
//#endif

#ifdef _TIRED_CHECK
    BYTE    m_ucRealNameAuth;   //  �Ǹ� ���� ����� �ĺ� (0 : ��Ǹ� , 1 : �Ǹ�)
#endif

    DWORD   m_dwLogSeq;

//#ifdef _SHAPE_AUTH
//BYTE  m_btShapeAuthFailCount; // ���� ������ ��� �����ߴ��� ����
//bool  m_bSucceedAuth;         // ���� ������ �Ϸ�Ǿ����� �ĺ�
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

    bool    m_IsLoginLogWrite;  //�α��� �α׸� ����� TRUE �� �����Ѵ�. 
                                //���� �ش� ������ TRUE �϶��� �α׾ƿ� �α׸� �����.

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
    DWORD m_dwGameCode; // ������ GameServer Code(SUN = 2)
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

// (WAVERIX) (090702) (CHANGES) Auth token usage case�� �þ�� �����Ƿ�, ���� ó���� ����
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


