#include "stdafx.h"
#include "PacketControl_AuthFlow.h"
#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include <SunAuth/AuthProtocol_CF.h>
#include <PacketStruct_CG.h>
#ifdef _SERVER
  #include <../Server/Common/SunAuth/AuthProtocol_GA.h>
#endif

#include <ISharemem.h>

#ifdef _SERVER
  #include <HanAuth/HanAuthForSvr.h>
#else
  #include <HanAuth/HanAuthForClient.h>
#endif

//BOOST_STATIC_ASSERT(sizeof(WLAUTH_INFO) == (32 + 260 + sizeof(void*) * 2));
BOOST_STATIC_ASSERT(MSG_CF_AUTH_TOKEN_PASS_SYN::MAX_STREAM_SIZE >=
                    sizeof(WLAUTH_INFO));

#define BUF_ALIGN_008(__size)			(((__size) + ((8) - 1)) & (~((8) - 1)))

//==================================================================================================
//  @name: CSAuthFlow
//  @brief: Client & Server Packet Flow Definition for Authentication
//
namespace nsPacket {

struct CSAuthFlow;

enum CSAuthFlow::eMsgType {
    eMsgType_AuthToken_Pass,
    eMsgType_ReAuthToken_Pass,      //__NA001334_090420_SERVER_SELECT_FACILITY__
    eMsgType_HanAuthToken_Pass,     //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    eMsgType_GlobalAuthToken_Pass,  //__NA000000_GLOBAL_AUTH_PROCESS__
    eMsgType_OneTimePasswordToken_Pass, //_JP000000_ONETIMEPASSWORD_AUTHFLOW_
    eMsgType_GameChuToken_Pass, //_JP_0_20100723_GAMECHU_AUTHFLOW_
	eMsgType_Security_token_pass, //__CN_20100909_SECURITY_TOKEN__
    eMsgType_Ingamba_Auth_Pass,     //_RU_INGAMBA_AUTH_PROCESS
    eMsgType_Count
};

typedef MSG_CF_AUTH_TOKEN_PASS_SYN::Stream  AuthTokenStream;

//__KR000000_090128_CHANNELING__
#pragma warning(push)
#pragma warning(disable:4324)
#pragma pack(push, 1)
struct __declspec(align(8)) AuthToken {
    DWORD type_code;
    char id[WL_ID_LENGTH+1];
    BYTE key[WL_KEY_LENGTH];
};
BOOST_STATIC_ASSERT(MSG_CF_AUTH_TOKEN_PASS_SYN::MAX_STREAM_SIZE >= sizeof(AuthToken));

//__NA000000_GLOBAL_AUTH_PROCESS__
struct __declspec(align(8)) AuthTokenGlobal {
    DWORD type_code;
    char id[ID_MAX_LEN + 1]; // Global Spec. (10) - Game ID
    BYTE key[40 + 1]; // Global Spec. (36)
    char mac_address[17 + 1]; // Global Spec. "FF:FF:FF:FF:FF:FF"
};
BOOST_STATIC_ASSERT(MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::MAX_STREAM_SIZE >= sizeof(AuthTokenGlobal));

#pragma pack(pop)
#pragma warning(pop)
}; //end of namespace


//==================================================================================================
#ifdef __KR000000_090128_CHANNELING__

namespace nsPacket {

WORD
CSAuthFlow::MakeStream(const WLAUTH_INFO& auth_info,
                       MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* INOUT stream)
{
    AuthToken* auth_token = reinterpret_cast<AuthToken*>(stream);
    auth_token->type_code = eMsgType_AuthToken_Pass;
    strncpy(auth_token->id, auth_info.id, _countof(auth_token->id));
    auth_token->id[WL_ID_LENGTH] = '\0';
    CopyMemory(auth_token->key, auth_info.key, sizeof(auth_token->key));

    const WORD msg_size = (WORD)sizeof(MSG_CF_AUTH_TOKEN_PASS_SYN::Stream);
    return msg_size;
}

#if defined(_SERVER)

BOOLEAN
CSAuthFlow::PreVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN* msg, WORD msg_size)
{
    if(sizeof(*msg) != msg_size)
        return false;
    return true;
}

BOOLEAN
CSAuthFlow::PostVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* stream,
                         Token* id_info, Token* key_info)
{
    const AuthToken* auth_token = reinterpret_cast<const AuthToken*>(stream);
    if(auth_token->type_code != eMsgType_AuthToken_Pass)
        return false;
    if(auth_token->id[WL_ID_LENGTH] != '\0')
        return false;

    if(id_info) {
        id_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(auth_token->id));
        id_info->length = sizeof(auth_token->id);
    }
    if(key_info) {
        key_info->buffer = reinterpret_cast<BYTE*>(const_cast<BYTE*>(auth_token->key));
        key_info->length = sizeof(auth_token->key);
    }

    return true;
}
#endif //_SERVER

}; // end of namespace nsPacket
#endif //__KR000000_090128_CHANNELING__
//==================================================================================================
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__

namespace nsPacket {

;

////__NA000000_GLOBAL_AUTH_PROCESS__
//struct __declspec(align(8)) AuthTokenGlobal {
//    DWORD type_code;
//    char id[ID_MAX_LEN + 1]; // Global Spec. (10) - Game ID
//    BYTE key[40 + 1]; // Global Spec. (36)
//    char mac_address[17 + 1]; // Global Spec. "FF:FF:FF:FF:FF:FF"
//};

WORD
CSAuthFlow::MakeStream(const char* game_id, const char* auth_key, const char* mac_address,
                       MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* INOUT stream)
{
    AuthTokenGlobal* auth_token = reinterpret_cast<AuthTokenGlobal*>(stream);
    auth_token->type_code = eMsgType_GlobalAuthToken_Pass;

    strncpy(auth_token->id, game_id, _countof(auth_token->id));
    auth_token->id[_countof(auth_token->id) - 1] = '\0';

    CopyMemory(auth_token->key, auth_key, sizeof(auth_token->key));
    auth_token->key[_countof(auth_token->key) - 1] = 0xFF; // for coninuous string

    strncpy(auth_token->mac_address, mac_address, _countof(auth_token->mac_address));
    auth_token->mac_address[_countof(auth_token->mac_address) - 1] = '\0';

    const WORD msg_size = (WORD)sizeof(MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream);
    return msg_size;
}

#if defined(_SERVER)

BOOLEAN
CSAuthFlow::PreVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN* msg, WORD msg_size)
{
    if(sizeof(*msg) != msg_size)
        return false;
    return true;
}

BOOLEAN
CSAuthFlow::PostVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream,
                         Token* id_info, Token* key_info)
{
    const AuthTokenGlobal* auth_token = reinterpret_cast<const AuthTokenGlobal*>(stream);
    if(auth_token->type_code != eMsgType_GlobalAuthToken_Pass)
        return false;
    if(auth_token->id[_countof(auth_token->id) - 1] != '\0')
        return false;
    if(auth_token->key[_countof(auth_token->key) - 1] != 0xFF) // for coninuous string
        return false;
    if(auth_token->mac_address[_countof(auth_token->mac_address) - 1] != '\0')
        return false;

    if(id_info) {
        id_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(auth_token->id));
        id_info->length = sizeof(auth_token->id);
    }
    if(key_info) {
        key_info->buffer = reinterpret_cast<BYTE*>(const_cast<BYTE*>(auth_token->key));
        key_info->length = sizeof(auth_token->key) + sizeof(auth_token->mac_address);
    }

    return true;
}

BOOLEAN
CSAuthFlow::ExtractAddinInfo(Token* INOUT extracted_key_info,   //extracted 1st
                             Token* mac_address_info)           //extracted 2nd
{
    //assert(extracted_key_info != 0);
    const AuthTokenGlobal* const auth_token_null = (AuthTokenGlobal*)(0);
    BYTE* mac_address = extracted_key_info->buffer + sizeof(auth_token_null->key);
    extracted_key_info->buffer[_countof(auth_token_null->key) - 1] = '\0';
    if(mac_address_info) {
        mac_address_info->buffer = mac_address;
        mac_address_info->length = sizeof(auth_token_null->mac_address);
    }
    return true;
}

#endif //_SERVER

}; // end of namespace nsPacket
#endif //__NA000000_GLOBAL_AUTH_PROCESS__
//
//==================================================================================================
//

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
namespace nsPacket {

#pragma pack(push, 1)
struct ReAuthTokenStreamInternal
{
    enum {
        MAX_AUTHKEY_STREAM_SIZE = nsPacket::ReAuthTokenStream::MAX_AUTHKEY_STREAM_SIZE,
        MAX_ID_STREAM_SIZE      = nsPacket::ReAuthTokenStream::MAX_ID_STREAM_SIZE,
        MAX_STREAM_SIZE         = nsPacket::ReAuthTokenStream::MAX_STREAM_SIZE,
    };
    DWORD   type_code;
    char    reauth_keystream[MAX_AUTHKEY_STREAM_SIZE];
    char    id_stream[MAX_ID_STREAM_SIZE];
};
#pragma pack(pop)

BOOST_STATIC_ASSERT(sizeof(ReAuthTokenStreamInternal) == sizeof(ReAuthTokenStream));
BOOST_STATIC_ASSERT(sizeof(ReAuthTokenStream) <= sizeof(MSG_CF_AUTH_TOKEN_PASS_SYN::Stream));

WORD
CSAuthFlow::MakeStream(const char* userID, const ReAuthTokenStream& reauth_info,
                       MSG_CF_AUTH_REAUTH_PASS_SYN* INOUT stream)
{
    WORD msg_size = MakeReAuthStream(userID, reauth_info, &stream->info_stream);
    __UNUSED(msg_size);
    return sizeof(*stream); // not used 'msg_size'
}

WORD
CSAuthFlow::MakeReAuthStream(const char* userID, const ReAuthTokenStream& reauth_info,
                             ReAuthTokenStream* stream)
{
    const ReAuthTokenStreamInternal* source =
        reinterpret_cast<const ReAuthTokenStreamInternal*>(&reauth_info);
    ReAuthTokenStreamInternal* dest = reinterpret_cast<ReAuthTokenStreamInternal*>(stream);

    dest->type_code = eMsgType_ReAuthToken_Pass;

    strncpy(dest->reauth_keystream, source->reauth_keystream, _countof(dest->reauth_keystream));
    dest->reauth_keystream[_countof(dest->reauth_keystream) - 1] = '\0';

    if(userID == 0)
        ZeroMemory(dest->id_stream, sizeof(dest->id_stream));
    else
    {
        strncpy(dest->id_stream, userID, _countof(dest->id_stream));
        dest->id_stream[_countof(dest->id_stream) - 1] = '\0';
    }

    WORD msg_size = sizeof(*stream);
    return msg_size;
}

#if defined(_SERVER)

WORD
CSAuthFlow::MakeStream(const char* userID, const ReAuthTokenStream& reauth_info,
                       MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK* INOUT stream)
{
    MakeReAuthStream(userID, reauth_info, &stream->m_ReAuthToken);
    return sizeof(*stream); // not used 'msg_size'
}

WORD
CSAuthFlow::MakeStream(const char* nullID, const ReAuthTokenStream& reauth_info,
                       MSG_AU_AUTH_SERVER_SELECT_ACK* INOUT stream)
{
    MakeReAuthStream(nullID, reauth_info, &stream->m_ReAuthToken);
    return sizeof(*stream); // not used 'msg_size'
}

BOOLEAN
CSAuthFlow::PreVerifier(const MSG_CF_AUTH_REAUTH_PASS_SYN* msg, WORD msg_size)
{
    if(sizeof(*msg) != msg_size)
        return false;
    return true;
}

BOOLEAN
CSAuthFlow::PostVerifier(const MSG_CF_AUTH_REAUTH_PASS_SYN* stream,
                         Token* id_info, Token* key_info)
{
    const ReAuthTokenStreamInternal* source =
        reinterpret_cast<const ReAuthTokenStreamInternal*>(&stream->info_stream);
    if(source->type_code != eMsgType_ReAuthToken_Pass)
        return false;
    if(source->reauth_keystream[_countof(source->reauth_keystream) - 1] != '\0')
        return false;
    if(source->id_stream[_countof(source->id_stream) - 1] != '\0')
        return false;

    if(id_info) {
        id_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(source->id_stream));
        id_info->length = sizeof(source->id_stream);
    }
    if(key_info) {
        key_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(source->reauth_keystream));
        key_info->length = sizeof(source->reauth_keystream);
    }

    return true;
}
#endif

}; //end of namespace
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
//
//
//==================================================================================================

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

namespace nsPacket {

#pragma pack(push, 1)
struct HanAuthTokenStreamInternal
{
    enum {
        MAX_AUTHKEY_STREAM_SIZE = nsPacket::HanAuthTokenStream::MAX_AUTHKEY_STREAM_SIZE,
        MAX_MEMBER_ID           = nsPacket::HanAuthTokenStream::MAX_MEMBER_ID,
        MAX_AUTHSTRING          = nsPacket::HanAuthTokenStream::MAX_AUTHSTRING,
    };
    DWORD   type_code;
    char    id_stream[MAX_MEMBER_ID + 1];
    char    auth_stream[MAX_AUTHSTRING + 1];
};
#pragma pack(pop)

BOOST_STATIC_ASSERT(sizeof(nsPacket::HanAuthTokenStream) == sizeof(HanAuthTokenStreamInternal));
#ifdef _SERVER
// (WAVERIX) (090701) (NOTE) server/client header 파일의 SIZE_AUTHSTRING는 서로 틀리다. GameString
//과 혼용된 듯 한데, 일단 서버 기준으로 설정하고 문제 발생시 클라이언트도 검증하기로 한다.
BOOST_STATIC_ASSERT(HanAuthTokenStreamInternal::MAX_MEMBER_ID == (50) &&
                    HanAuthTokenStreamInternal::MAX_MEMBER_ID == LIMIT_MEMBERID);
BOOST_STATIC_ASSERT(HanAuthTokenStreamInternal::MAX_AUTHSTRING == (4 << 10) &&
                    HanAuthTokenStreamInternal::MAX_AUTHSTRING == LIMIT_AUTHSTRING);
#endif

WORD
CSAuthFlow::MakeStream(const char* memberID, const char* authString,
                       MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* INOUT stream)
{
    HanAuthTokenStreamInternal* auth_token =
        reinterpret_cast<HanAuthTokenStreamInternal*>(&stream->info_stream);
    auth_token->type_code = eMsgType_HanAuthToken_Pass;
    strncpy(auth_token->id_stream, memberID, _countof(auth_token->id_stream));
    auth_token->id_stream[_countof(auth_token->id_stream) - 1] = '\0';
    strncpy(auth_token->auth_stream, authString, _countof(auth_token->auth_stream));
    auth_token->auth_stream[_countof(auth_token->auth_stream) - 1] = '\0';

    const WORD msg_size = (WORD)sizeof(*auth_token);
    return msg_size;
}

#if defined(_SERVER)

BOOLEAN
CSAuthFlow::PreVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* msg, WORD msg_size)
{
    if(sizeof(*msg) != msg_size)
        return false;
    return true;
}

BOOLEAN
CSAuthFlow::PostVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* stream,
                         Token* id_info, Token* key_info)
{
    const HanAuthTokenStreamInternal* auth_token =
        reinterpret_cast<const HanAuthTokenStreamInternal*>(&stream->info_stream);

    assert(auth_token->type_code == eMsgType_HanAuthToken_Pass);
    assert(auth_token->id_stream[_countof(auth_token->id_stream) - 1] == '\0');
    assert(auth_token->auth_stream[_countof(auth_token->auth_stream) - 1] == '\0');

    if(auth_token->type_code != eMsgType_HanAuthToken_Pass)
        return false;
    if(auth_token->id_stream[_countof(auth_token->id_stream) - 1] != '\0')
        return false;
    if(auth_token->auth_stream[_countof(auth_token->auth_stream) - 1] != '\0')
        return false;

    if(id_info) {
        id_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(auth_token->id_stream));
        id_info->length = sizeof(auth_token->id_stream);
    }
    if(key_info) {
        key_info->buffer = reinterpret_cast<BYTE*>(const_cast<char*>(auth_token->auth_stream));
        key_info->length = sizeof(auth_token->auth_stream);
    }

    return true;
}
#endif //_SERVER

}; // end of namespace nsPacket
#endif //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__


#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_

namespace nsPacket {
;

#pragma pack(push, 1)
struct OneTimePasswordStreamInternal
{
    enum {
        MAX_OTP_LENGTH      = OneTimePasswordStream::MAX_OTP_LENGTH,
        MAX_OTP_STREAM_SIZE = OneTimePasswordStream::MAX_OTP_STREAM_SIZE,
    };
    DWORD   type_code;
    OneTimePassword otp_stream;
    BYTE    __pad00[MAX_OTP_STREAM_SIZE - sizeof(OneTimePassword)];
};
#pragma pack(pop)

BOOST_STATIC_ASSERT(sizeof(OneTimePasswordStream) == sizeof(OneTimePasswordStreamInternal));

WORD
CSAuthFlow::MakeOtpStream(const char* user_id, const OneTimePassword& otp_info,
                          MSG_CF_AUTH_OTP_PASS_SYN* INOUT stream)
{
    OneTimePasswordStreamInternal* otp_token = \
        reinterpret_cast<OneTimePasswordStreamInternal*>(&stream->info_stream);
    otp_token->type_code = eMsgType_OneTimePasswordToken_Pass;
    OneTimePassword* password = &otp_token->otp_stream;
    *password = otp_info;
    password->value[_countof(password->value) - 1] = '\0';

    const WORD msg_size = sizeof(*stream);
    return msg_size;
}

#ifdef _SERVER
BOOLEAN
CSAuthFlow::PreVerifier(const MSG_CF_AUTH_OTP_PASS_SYN* msg, WORD msg_size)
{
    if (sizeof(*msg) != msg_size) {
        return false;
    }

    const OneTimePasswordStreamInternal* otp_token = \
        reinterpret_cast<const OneTimePasswordStreamInternal*>(msg->info_stream);

    if (otp_token->type_code != eMsgType_OneTimePasswordToken_Pass) {
        return false;
    }

    const OneTimePassword* password = &otp_token->otp_stream;
    if (password->value[_countof(password->value) - 1] != '\0') {
        return false;
    }

    return true;        
}

// s2s inter check routine
BOOLEAN
CSAuthFlow::PostVerifier(const MSG_CF_AUTH_OTP_PASS_SYN* msg, OneTimePassword* extracted)
{
    if (PreVerifier(msg, sizeof(*msg)) == false) {
        return false;
    }

    const OneTimePasswordStreamInternal* otp_token = \
        reinterpret_cast<const OneTimePasswordStreamInternal*>(msg->info_stream);
    *extracted = otp_token->otp_stream;

    return true;
}
#endif

}; //end of namespace

#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_


#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_

namespace nsPacket {
    ;

    WORD
        CSAuthFlow::MakeStream(wchar_t first_parameter[GameChuInfoStream::MAX_FIRST_PARAMETER],
                                  wchar_t second_parameter[GameChuInfoStream::MAX_SECOND_PARAMETER],
                                  MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN& OUT msg)
    {
        msg.gamechu_info_.type_code = eMsgType_GameChuToken_Pass;

        wmemcpy(msg.gamechu_info_.first_parameter, first_parameter, 
                GameChuInfoStream::MAX_FIRST_PARAMETER);
        msg.gamechu_info_.first_parameter[_countof(msg.gamechu_info_.first_parameter) - 1] = L'\0';
        wmemcpy(msg.gamechu_info_.second_parameter, second_parameter, 
                GameChuInfoStream::MAX_SECOND_PARAMETER);
        msg.gamechu_info_.second_parameter[_countof(msg.gamechu_info_.second_parameter) - 1] = L'\0';
       
        return sizeof(msg);
    }

#ifdef _SERVER
    BOOLEAN
        CSAuthFlow::PreVerifier(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg, WORD msg_size)
    {
        if (sizeof(*msg) != msg_size) {
            return false;
        }

        if (msg->gamechu_info_.type_code != eMsgType_GameChuToken_Pass) {
            return false;
        }

        return true;        
    }

    BOOLEAN
        CSAuthFlow::PostVerifier(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg)
    {
        if (PreVerifier(msg, sizeof(*msg)) == false) {
            return false;
        }

        return true;
    }
#endif

}; //end of namespace

#endif //_JP_0_20100723_GAMECHU_AUTHFLOW_


#ifdef __CN_20100909_SECURITY_TOKEN__

namespace nsPacket {
	;

	WORD
		CSAuthFlow::MakeOtpStream(const char* security_token_code_,
		                          MSG_CF_AUTH_SECURITY_TOKEN_SYN& OUT msg)
	{
		msg.security_token_info_.type_code_ = eMsgType_Security_token_pass;
		::_tcsncpy(msg.security_token_info_.security_token_code_, 
				   security_token_code_, 
				   _countof(msg.security_token_info_.security_token_code_));
		msg.security_token_info_.security_token_code_[_countof(msg.security_token_info_.security_token_code_)-1] = '\0';
		
		return sizeof(msg);
	}

#ifdef _SERVER
	BOOLEAN
		CSAuthFlow::PreVerifier(const MSG_CF_AUTH_SECURITY_TOKEN_SYN* msg, WORD msg_size)
	{
		if (sizeof(*msg) != msg_size) {
			return false;
		}

		if (msg->security_token_info_.type_code_ != eMsgType_Security_token_pass) {
			return false;
		}

		return true;          
	}

	// s2s inter check routine
	BOOLEAN
		CSAuthFlow::PostVerifier(const MSG_CF_AUTH_SECURITY_TOKEN_SYN* msg)
	{
		if (PreVerifier(msg, sizeof(*msg)) == false) {
			return false;
		}

		return true;
	}
#endif

}; //end of namespace

#endif //__CN_20100909_SECURITY_TOKEN__

#ifdef _RU_INGAMBA_AUTH_PROCESS
namespace nsPacket {
;
WORD CSAuthFlow::MakeStream(const IngambaAuthInfo* auth_stream , 
                            MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* INOUT syn_msg)
{
    syn_msg->ingamba_auth_info_ = *auth_stream;
    syn_msg->ingamba_auth_info_.type_code_ = eMsgType_Ingamba_Auth_Pass;

    const WORD msg_size = (WORD)sizeof(*syn_msg);
    return msg_size;
}

#if defined(_SERVER)

BOOLEAN CSAuthFlow::PreVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg, WORD msg_size)
{
    if (sizeof(*auth_msg) != msg_size) {
        return false;
    }
    return true;
}

BOOLEAN CSAuthFlow::PostVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg)
{
    if (PreVerifier(auth_msg, sizeof(*auth_msg)) == false)
    {
        return false;
    }
    return true;
}
#endif //_SERVER

}; // end of namespace nsPacket
#endif//_RU_INGAMBA_AUTH_PROCESS

#ifdef _CN_ZY_AUTH_PROCESS
namespace nsPacket {
#ifdef _SERVER
    BOOLEAN CSAuthFlow::PreVerifier(const MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN* msg, WORD size)
    {
        if(sizeof(*msg) != size) {
            return false;
        }
        return true;
    }
#endif
};
#endif //_CN_ZY_AUTH_PROCESS