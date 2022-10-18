#include "stdafx.h"
#include "AuthStepAuthToken.h"
#include <RandomNumberGenerator.h>

//#ifdef _CHANNELING_AUTH_FLOW

const wchar_t
AuthStepAuthToken::AuthTokenStreamTimeout[DB_AuthToken_Length] =
{
    L'0', L'0', L'0', L'0',       L'0', L'0', L'0', L'0', L'-',
    L'0', L'0', L'0', L'0', L'-', L'0', L'0', L'0', L'0', L'-',
    L'0', L'0', L'0', L'0', L'-', L'0', L'0', L'0', L'0', 
    L'0', L'0', L'0', L'0',       L'0', L'0', L'0', L'0', L'\0'
};

const wchar_t
AuthStepAuthToken::AuthTokenStreamNull[DB_AuthToken_Length] = { 0, };

AuthStepAuthToken::eProtocol
AuthStepAuthToken::GetProtocolType(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const WORD ProtocolIdPwd_CF = MAKEWORD(CATEGORY_AUTH, CF_AUTH_ID_PASSWORD_CHECK_SYN);
    const WORD ProtocolIdPwd_FS = MAKEWORD(CATEGORY_AUTH, FS_AUTH_ID_PASSWORD_CHECK_SYN);
    BOOST_STATIC_ASSERT(ProtocolIdPwd_CF == MAKEWORD(CATEGORY_AUTH, CF_AUTH_TOKEN_PASS_SYN));
    BOOST_STATIC_ASSERT(ProtocolIdPwd_FS == MAKEWORD(CATEGORY_AUTH, FS_AUTH_TOKEN_PASS_SYN));
    BOOST_STATIC_ASSERT(CF_AUTH_TOKEN_PASS_SYN == CF_AUTH_TOKEN_GLOBAL_PASS_SYN &&
                        CF_AUTH_TOKEN_PASS_SYN == CF_AUTH_TOKEN_NORTHA_PASS_SYN);
    BOOST_STATIC_ASSERT(FS_AUTH_TOKEN_PASS_SYN == FS_AUTH_TOKEN_GLOBAL_PASS_SYN &&
                        FS_AUTH_TOKEN_PASS_SYN == FS_AUTH_TOKEN_NORTHA_PASS_SYN);

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    const WORD ProtocolReAuth_CF = MAKEWORD(CATEGORY_AUTH, CF_AUTH_REAUTH_PASS_SYN);
    const WORD ProtocolReAuth_FS = MAKEWORD(CATEGORY_AUTH, FS_AUTH_REAUTH_PASS_SYN);
#endif

    const WORD protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);

    GLOBAL_DEBUGLOG(__FUNCTION__ " %04X - ProtocolIdPwd_CF\n", ProtocolIdPwd_CF);
    GLOBAL_DEBUGLOG(__FUNCTION__ " %04X - ProtocolIdPwd_FS\n", ProtocolIdPwd_FS);
    GLOBAL_DEBUGLOG(__FUNCTION__ " %04X - requested\n", protocol);

    switch(protocol)
    {
    case ProtocolIdPwd_CF: 
    {
    #if defined(_AUTH_KOREA)
        if(PreVerifier(static_cast<MSG_CF_AUTH_TOKEN_PASS_SYN*>(pMsg), size))
            return eProtocol_AuthTokenKorea;
    #elif defined(_AUTH_GLOBAL)
        if(PreVerifier(static_cast<MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN*>(pMsg), size))
            return eProtocol_AuthTokenGlobal;
    #elif defined(_AUTH_USA)
        if(PreVerifier(static_cast<MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN*>(pMsg), size))
            return eProtocol_AuthTokenNorthA;
    #elif defined(_AUTH_JAPAN)
        if(PreVerifier(static_cast<MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN*>(pMsg), size))
            return eProtocol_AuthTokenGamechu;
    #elif defined(_AUTH_RUSSIA)
        if(PreVerifier(static_cast<MSG_CF_AUTH_PASS_RU_INGAMBA_SYN*>(pMsg), size))
            return eProtocol_AuthIngamba;
    #elif defined(_AUTH_CHINA)
        if(PreVerifier(static_cast<MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN*>(pMsg), size))
            return eProtocol_AuthZY;
    #endif
        if(size == sizeof(MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN))
            return eProtocol_IdAndPassword;

        return eProtocol_Unknown;
    } break;

    case ProtocolIdPwd_FS: {
        // FS packet이며, 이것은 통과시킨다.
    #if defined(_AUTH_KOREA)
        if(size == sizeof(MSG_FS_AUTH_TOKEN_PASS_SYN))
            return eProtocol_AuthTokenKorea;
    #elif defined(_AUTH_GLOBAL)
        if(size == sizeof(MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN))
            return eProtocol_AuthTokenGlobal;
    #elif defined(_AUTH_USA)
        if(size == sizeof(MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN))
            return eProtocol_AuthTokenNorthA;
    #elif defined(_AUTH_JAPAN)
        if(size == sizeof(MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN))
            return eProtocol_AuthTokenGamechu;
    #elif defined(_AUTH_RUSSIA)
        if(size == sizeof(MSG_FS_AUTH_PASS_RU_INGAMBA_SYN))
            return eProtocol_AuthIngamba;
    #elif defined(_AUTH_CHINA)
        if(size == sizeof(MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN))
            return eProtocol_AuthZY;
    #endif
        if(size == sizeof(MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN))
            return eProtocol_IdAndPassword;        

        return eProtocol_Unknown;
    } break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case ProtocolReAuth_CF:
        if(PreVerifier(static_cast<MSG_CF_AUTH_REAUTH_PASS_SYN*>(pMsg), size))
            return eProtocol_ReAuthToken;
        return eProtocol_Unknown;
    case ProtocolReAuth_FS:
        if(size == sizeof(MSG_FS_AUTH_REAUTH_PASS_SYN))
            return eProtocol_ReAuthToken;
        return eProtocol_Unknown;
#endif
    }
    return eProtocol_Unknown;
}

//#endif //_CHANNELING_AUTH_FLOW

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

const char
AuthStepAuthToken::ReAuthTokenStreamMatchingTable[16] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

BOOLEAN
AuthStepAuthToken::MakeReAuthToken(DWORD userGuid, WORD index,
                                   ReAuthStepAuthTokenInfo* manage_info)
{
    typedef RandomNumberGenerator RND;
    const char Tag = '-';

    ZeroMemory(manage_info, sizeof(*manage_info));
    DWORD rndNo = 0;

    char* OUT offset = manage_info->format_stream.raw_stream;

    //  "00000000-0000-0000-0000-000000000000"
    //  "________-idx -user Code-____________"

    rndNo = RND::GetRandomNumber();
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    //rndNo = RND::GetRandomNumber();
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;

    *offset++ = Tag;

    //////////////////////////////////////////////////////////////////////////
    //  user Key
    {
        rndNo = RND::GetRandomNumber();
        manage_info->userKey_converter[0] = WORD(rndNo);
        rndNo ^= userGuid & 0xFFFF;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;

        *offset++ = Tag;

        rndNo = RND::GetRandomNumber();
        manage_info->userKey_converter[1] = WORD(rndNo);
        rndNo ^= userGuid >> 16;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;

        *offset++ = Tag;
    }

    //////////////////////////////////////////////////////////////////////////
    //  index
    {
        rndNo = RND::GetRandomNumber();
        manage_info->index_converter = WORD(rndNo);
        rndNo ^= DWORD(index);

        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
        *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;

        *offset++ = Tag;
    }

    //rndNo = RND::GetRandomNumber();
    rndNo = index;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    rndNo = RND::GetRandomNumber();
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    rndNo = RND::GetRandomNumber();
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;
    *offset++ = ReAuthTokenStreamMatchingTable[char(rndNo & 0xF)]; rndNo >>= 4;

    *offset = '\0';

    return true;
}

inline DWORD StringTokenConverter(DWORD input)
{
    DWORD decode = 0;
    decode |= input & 0xF;  decode <<= 4; input >>= 4;
    decode |= input & 0xF;  decode <<= 4; input >>= 4;
    decode |= input & 0xF;  decode <<= 4; input >>= 4;
    decode |= input & 0xF;
    return decode;
}

BOOLEAN
AuthStepAuthToken::ExtractReAuthTokenRawIndex(const Token* key_info, WORD* pIndex)
{
    const ReAuthStepAuthTokenFormat* format =
        reinterpret_cast<const ReAuthStepAuthTokenFormat*>(key_info->buffer);
    char raw_index[5] = { 0, };
    BOOST_STATIC_ASSERT(sizeof(raw_index) == sizeof(format->rawReAuthIndex) + 1);
    CopyMemory(raw_index, format->rawReAuthIndex, sizeof(format->rawReAuthIndex));

    DWORD index = 0;
    sscanf(raw_index, "%X", &index);
    index = StringTokenConverter(index);

    if(index > USHRT_MAX)
        return false;

    *pIndex = WORD(index);
    return true;
}

BOOLEAN
AuthStepAuthToken::ExtractReAuthToken(const ReAuthStepAuthTokenInfo* manage_info,
                                      const Token* key_info,
                                      DWORD* pUserGuid, WORD* pIndex)
{
    const ReAuthStepAuthTokenFormat* format =
        reinterpret_cast<const ReAuthStepAuthTokenFormat*>(key_info->buffer);
    ReAuthStepAuthTokenFormat extract = *format;
    extract.tag0 = '\0';
    extract.tag1 = '\0';
    extract.tag2 = '\0';
    extract.tag3 = '\0';
    extract.tailMark = '\0';

    DWORD userGuid1 = 0, userGuid2 = 0, index = 0;
    sscanf(extract.reAuthIndex, "%X", &index);
    sscanf(extract.userCode1, "%X", &userGuid1);
    sscanf(extract.userCode2, "%X", &userGuid2);

    userGuid1 = StringTokenConverter(userGuid1);
    userGuid2 = StringTokenConverter(userGuid2);
    userGuid1 ^= manage_info->userKey_converter[0];
    userGuid2 ^= manage_info->userKey_converter[1];

    *pUserGuid = (userGuid2 << 16) | (userGuid1);

    index = StringTokenConverter(index);
    if(index > USHRT_MAX)
        return false;

    index ^= manage_info->index_converter;
    *pIndex = WORD(index);
    return true;
}
#endif



