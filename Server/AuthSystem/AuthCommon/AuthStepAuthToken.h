#ifndef _AUTHSTEP_AUTH_TOKEN_H_
#define _AUTHSTEP_AUTH_TOKEN_H_

#include "AuthProtocol_FS.h"
#include <PacketControl/PacketControl_AuthFlow.h>
#include "./HanAuth/HanAuthQueryManager.h"

union ReAuthStepAuthTokenFormat; //__NA001334_090420_SERVER_SELECT_FACILITY__
struct ReAuthStepAuthTokenInfo;
//#ifdef _CHANNELING_AUTH_FLOW

class AuthStepAuthToken : public nsPacket::CSAuthFlow
{
public:
    enum eProtocol {
        eProtocol_IdAndPassword,
        eProtocol_AuthTokenKorea,   //_CHANNELING_AUTH_FLOW
        eProtocol_ReAuthToken,      //__NA001334_090420_SERVER_SELECT_FACILITY__
        eProtocol_AuthTokenNorthA,  //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
        eProtocol_AuthTokenGlobal,  //__NA000000_GLOBAL_AUTH_PROCESS__
        eProtocol_AuthTokenGamechu, //_JP_0_20100723_GAMECHU_AUTHFLOW_
        eProtocol_AuthIngamba,      //_RU_INGAMBA_AUTH_PROCESS
        eProtocol_AuthZY,           //_CN_ZY_AUTH_PROCESS
        eProtocol_Unknown,
        eProtocol_Corrupt,
    };

    static const eProtocol AcceptProtocol = eProtocol( (1 << eProtocol_IdAndPassword)
                                            #ifdef _CHANNELING_AUTH_FLOW
                                                     | (1 << eProtocol_AuthTokenKorea)
                                            #endif
                                            #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
                                                     | (1 << eProtocol_ReAuthToken)
                                            #endif
                                            #ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
                                                     | (1 << eProtocol_AuthTokenNorthA)
                                            #endif
                                            #ifdef __NA000000_GLOBAL_AUTH_PROCESS__
                                                     | (1 << eProtocol_AuthTokenGlobal)
                                            #endif
                                            #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                                     | (1 << eProtocol_AuthTokenGamechu)
                                            #endif
                                            #ifdef _RU_INGAMBA_AUTH_PROCESS
                                                     | (1 << eProtocol_AuthIngamba)
                                            #endif
                                            #ifdef _CN_ZY_AUTH_PROCESS
                                                     | (1 << eProtocol_AuthZY)
                                            #endif
                                                     );
    AuthStepAuthToken(DWORD encode_key)
        : m_EncodeKey(encode_key)
    {
    }
    virtual ~AuthStepAuthToken()
    {}

    static const DWORD DB_AuthToken_Length = 36 + 1; // with null
    static const wchar_t AuthTokenStreamTimeout[DB_AuthToken_Length];
    static const wchar_t AuthTokenStreamNull[DB_AuthToken_Length];
    static eProtocol GetProtocolType(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef _CHANNELING_AUTH_FLOW
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* stream,
                                 Token* id_info, Token* key_info) = 0;

    inline const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* GetTokenStream() const {
        return reinterpret_cast<const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream*>(m_AuthTokenStream);
    }
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream,
                                 Token* id_info, Token* key_info) = 0;

    inline const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* GetTokenStream() const {
        return reinterpret_cast<const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream*>(m_AuthTokenStream);
    }
#endif
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    static const char ReAuthTokenStreamMatchingTable[16];
    inline const nsPacket::ReAuthTokenStream* GetReAuthStream() const {
        return reinterpret_cast<const nsPacket::ReAuthTokenStream*>(m_AuthTokenStream);
    }
    static BOOLEAN MakeReAuthToken(DWORD userGuid, WORD index,
                                   ReAuthStepAuthTokenInfo* manage_info);
    static BOOLEAN ExtractReAuthTokenRawIndex(const Token* key_info, WORD* pIndex);
    static BOOLEAN ExtractReAuthToken(const ReAuthStepAuthTokenInfo* manage_info,
                                      const Token* key_info,
                                      DWORD* pUserGuid, WORD* pIndex);
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* stream,
                                 Token* id_info, Token* key_info) = 0;
#endif
#ifdef _RU_INGAMBA_AUTH_PROCESS
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg) = 0;
#endif
protected:
    DWORD m_EncodeKey;
#pragma pack(push, 1)
    WORD __padding__;
    BYTE m_AuthTokenStream[8186];//sizeof(MSG_CF_AUTH_TOKEN_PASS_SYN::Stream)];
#pragma pack(pop)
};

//#endif //_CHANNELING_AUTH_FLOW
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

#pragma warning(push)
#pragma warning(disable : 4201)
#pragma pack(push, 1)
//  "00000000-0000-0000-0000-000000000000"
//  "________-idx -user Code-____________"
union ReAuthStepAuthTokenFormat
{
    struct {
        char        block0[8];
        char        tag0;               //'-'
        ////////////////////////////////////////////
        union {
            char    block1[4];
            char    userCode1[4];       //0xFFFF|0000
        };
        char        tag1;               //'-'
        ////////////////////////////////////////////
        union {
            char    block2[4];
            char    userCode2[4];       //0x0000|FFFF
        };
        char        tag2;               //'-'
        ////////////////////////////////////////////
        union {
            char    block3[4];
            char    reAuthIndex[4];     //0xFFFF
        };
        char        tag3;               //'-'
        ////////////////////////////////////////////
        union {
            char    block4[4];
            char    rawReAuthIndex[4];  //0xFFFF
        };
        char        block5[8];
        ////////////////////////////////////////////
        char        tailMark;           //'\0'
    };

    char raw_stream[AuthStepAuthToken::DB_AuthToken_Length];
};
#pragma pack(pop)
#pragma warning(pop)

struct ReAuthStepAuthTokenInfo
{
    WORD index_converter;
    WORD userKey_converter[2];
    union {
        struct {
            DWORD type_code;
            ReAuthStepAuthTokenFormat format_stream;
        };
        nsPacket::ReAuthTokenStream stored_stream;
    };
};

#endif


#endif //_AUTHSTEP_AUTH_TOKEN_H_