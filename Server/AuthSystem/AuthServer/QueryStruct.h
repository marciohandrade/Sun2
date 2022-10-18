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
//    WORD    btAlert;            // 로그인 경고 대상을 식별
//#endif

    WORD    btAuthFailCount;    // 연속오류횟수 저장(리턴값으로 받아야 하나 라이브러리에서 두가지를 동시에 못함..ㅠ.ㅠ) <- 중국은 안씀..;;

#ifdef _AUTH_MATRIX_FAIL_COUNT
    WORD    btShapeAuthFailCount;    // 형상 인증 연속 오류 횟수 저장(현재 안씀 구색 맞추기임..;;)
    WORD    btMatrixAuthFailCount;    // 매트릭스 인증 연속 오류 횟수 저장(현재 안씀 구색 맞추기임..;;)
#endif

    //_CHANNELING_AUTH_FLOW
    static const DWORD DB_AuthToken_Length = 36 + 1; // with null
    typedef wchar_t AuthToken[DB_AuthToken_Length];    //code에서 버퍼 검증
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
	디비에서 필드 타입이 varchar 이기 때문에 char 과 매칭 되느게 맞지만 ado 를 래핑한 라이브러리에서 
	리턴되는 레코드 값을 variant 로 받고 모든 문자열은 wchar_t 형식으로 오기 때문에 어쩔수 없이 wchar_t 타입으로 사용한다.
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
    DWORD   AuthKey;    // 인증 키 추가-성현창
    double  ConnectTime;// 아따 갑갑허다...원래 SYSTEMTIME임
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










