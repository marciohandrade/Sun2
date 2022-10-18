#pragma once

#pragma pack(push,1)

#include <Define.h>

struct DB_LOGIN_USER
{
    int     UserGuid;
    char    PassWD[kCryptStringSize];
    WORD    Sts[3];
    char    CharPassWD[kCryptStringSize];
    WORD    Age;
    int     IpGuid;
#ifdef _SECURE_AUTH
    BYTE    pData[128];
#endif

//#ifdef _PAAS_ALERT
//    WORD    btAlert;            // �α��� ��� ����� �ĺ�
//#endif

    WORD    btAuthFailCount;    // ���ӿ���Ƚ�� ����(���ϰ����� �޾ƾ� �ϳ� ���̺귯������ �ΰ����� ���ÿ� ����..��.��) <- �߱��� �Ⱦ�..;;

#ifdef _AUTH_MATRIX_FAIL_COUNT
    WORD    btShapeAuthFailCount;    // ���� ���� ���� ���� Ƚ�� ����(���� �Ⱦ� ���� ���߱���..;;)
    WORD    btMatrixAuthFailCount;    // ��Ʈ���� ���� ���� ���� Ƚ�� ����(���� �Ⱦ� ���� ���߱���..;;)
#endif

    //_CHANNELING_AUTH_FLOW
    static const DWORD DB_AuthToken_Length = 36 + 1; // with null
    typedef wchar_t AuthToken[DB_AuthToken_Length];    //code���� ���� ����
    AuthToken  auth_token_result;
    //_CHANNELING_AUTH_FLOW_HANGAME_USER_
    WORD    userType;   //ref: eAUTH_USER_TYPE
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    wchar_t user_account[ID_MAX_LEN + 1];
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	bool	is_token_use_;
	wchar_t	token_base64_key_[TOKEN_BASE64_KEY_MAX+1];
	/*
	��񿡼� �ʵ� Ÿ���� varchar �̱� ������ char �� ��Ī �Ǵ��� ������ ado �� ������ ���̺귯������ 
	���ϵǴ� ���ڵ� ���� variant �� �ް� ��� ���ڿ��� wchar_t �������� ���� ������ ��¿�� ���� wchar_t Ÿ������ ����Ѵ�.
	*/
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    wchar_t betakey_[MAX_BETA_KEY+1];
#endif
    //
};

struct DB_RECOVER_LOCATION
{
    DWORD   UserGuid;
    wchar_t UserID[ID_MAX_LEN + 1];
    DWORD   ServerGuid;
    short   ConnectSts;
    wchar_t IpPublicAddr[MAX_IP_NAME_LENGTH];
    wchar_t IpLocalAddr[MAX_IP_NAME_LENGTH];
    wchar_t AuthUserGuid[8];
    DWORD   CompGuid;
    DWORD   AuthKey;    // ���� Ű �߰�-����â
    double  ConnectTime;// �Ƶ� �������...���� SYSTEMTIME��
};

struct DB_CONTROL_IP_INFO
{
    WORD    ti_Type;
    wchar_t vc_IPAddress[50];
    __int64 i_IPAddress;
    DWORD   i_IPAddressCount;
};

struct DB_GAME_VERSION
{
    WORD    ti_Type;
    wchar_t vc_Version[20];
};



#pragma pack(pop)










