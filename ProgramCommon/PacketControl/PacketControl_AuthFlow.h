#ifndef _PACKETCONTROL_AUTH_FLOW_H_
#define _PACKETCONTROL_AUTH_FLOW_H_

#include <SunAuth/AuthProtocol_CF.h>

//==============================================================================
//  @brief: ISharemem.h
//
struct _WLAUTH_INFO;
typedef _WLAUTH_INFO  WLAUTH_INFO, *PWLAUTH_INFO;
//
//
//==============================================================================
//  @brief: PacketStruct_CF.h
//
struct MSG_CF_AUTH_TOKEN_PASS_SYN;
struct MSG_FC_AUTH_TOKEN_PASS_ACK;

struct MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN;
struct MSG_FC_AUTH_TOKEN_GLOBAL_PASS_ACK;

struct MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN;
struct MSG_FC_AUTH_TOKEN_NORTHA_PASS_ACK;

//_JP000000_ONETIMEPASSWORD_AUTHFLOW_
struct MSG_CF_AUTH_OTP_PASS_SYN;
struct MSG_CF_AUTH_OTP_PASS_ACK;
//==============================================================================
//  @brief: PacketStruct_GA.h
//
struct MSG_AU_AUTH_SERVER_SELECT_ACK;
//==============================================================================
//  @brief: PacketStruct_CG.h
//
struct MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK;
//
//
//==============================================================================
//  @name: namespace 'nsPacket'
//
namespace nsPacket {

//==============================================================================
//  @name: CSAuthFlow
//  @brief: Client & Server Packet Flow Definition for Authentication
//
struct CSAuthFlow
{
    enum eMsgType;
#if defined(_SERVER)
    struct Token {
        BYTE*   buffer;
        size_t  length;
    };
#endif

#ifdef __KR000000_090128_CHANNELING__
    //--------------------------------------------------------------------------
    //  @brief: in Client, MSG_CF_AUTH_TOKEN_PASS_SYN를 보내는 시점
    //  @returns:
    //      if(msg_size == 0) then FALSE;
    //      else { 패킷 구성 완료, 관련 처리 및 전송 }
    //  @code:
    static WORD MakeStream(const WLAUTH_INFO& auth_info,
                           MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* INOUT stream);

  #if defined(_SERVER)
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN* msg, WORD msg_size);
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* stream,
                                 Token* id_info, Token* key_info);
  #endif
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    //--------------------------------------------------------------------------
    //  @brief: in Client, MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN를 보내는 시점
    //  @returns:
    //      if(msg_size == 0) then FALSE;
    //      else { 패킷 구성 완료, 관련 처리 및 전송 }
    //  @code:
    //                              // user ID
    static WORD MakeStream(const char* game_id, const char* auth_key, const char* mac_address,
                           MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* INOUT stream);

  #if defined(_SERVER)
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN* msg, WORD msg_size);
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream,
                                 Token* id_info, Token* key_info);
    static BOOLEAN ExtractAddinInfo(Token* INOUT extracted_key_info, //extracted 1st
                                    Token* mac_address_info);        //extracted 2nd
  #endif
#endif
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    //--------------------------------------------------------------------------
    //  @brief: in Client, MSG_CF_AUTH_TOKEN_PASS_SYN를 보내는 시점
    //  @returns:
    //      if(msg_size == 0) then FALSE;
    //      else { 패킷 구성 완료, 관련 처리 및 전송 }
    //  @code:
    static WORD MakeStream(const char* userID, const ReAuthTokenStream& reauth_info,
                           MSG_CF_AUTH_REAUTH_PASS_SYN* INOUT stream);
  #if defined(_SERVER)
    static WORD MakeStream(const char* userID, const ReAuthTokenStream& reauth_info,
                           MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK* INOUT stream);
    static WORD MakeStream(const char* nullID, const ReAuthTokenStream& reauth_info,
                           MSG_AU_AUTH_SERVER_SELECT_ACK* INOUT stream);
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_REAUTH_PASS_SYN* msg, WORD msg_size);
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_REAUTH_PASS_SYN* stream,
                                 Token* id_info, Token* key_info);
  #endif
    static WORD MakeReAuthStream(const char* userID, const ReAuthTokenStream& reauth_info,
                                 ReAuthTokenStream* stream);
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    static WORD MakeStream(const char* memberID, const char* authString,
                           MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* INOUT stream);
  #if defined(_SERVER)
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* msg, WORD msg_size);
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* stream,
                                 Token* id_info, Token* key_info);
  #endif
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
    static WORD MakeStream(const IngambaAuthInfo* auth_info,
                           MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* INOUT auth_msg);
  #if defined(_SERVER)
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg, WORD msg_size);
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg);
  #endif
#endif

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
  #ifdef _SERVER
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_OTP_PASS_SYN* msg, WORD msg_size);
    static BOOLEAN PostVerifier(const MSG_CF_AUTH_OTP_PASS_SYN* msg, OneTimePassword* extracted);
  #endif
    static WORD MakeOtpStream(const char* user_id, const OneTimePassword& otp_info,
                              MSG_CF_AUTH_OTP_PASS_SYN* INOUT stream);
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
#ifdef _SERVER
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg, WORD msg_size);
    static BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg);
#endif
    static WORD MakeStream(wchar_t first_parameter[GameChuInfoStream::MAX_FIRST_PARAMETER],
        wchar_t second_parameter[GameChuInfoStream::MAX_SECOND_PARAMETER],
        MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN& OUT msg);
#endif    

#ifdef __CN_20100909_SECURITY_TOKEN__
#ifdef _SERVER
	static BOOLEAN PreVerifier(const MSG_CF_AUTH_SECURITY_TOKEN_SYN* msg, WORD msg_size);
	static BOOLEAN PostVerifier(const MSG_CF_AUTH_SECURITY_TOKEN_SYN* msg);
#endif
	static WORD MakeOtpStream(const char* security_token_code_,
		MSG_CF_AUTH_SECURITY_TOKEN_SYN& OUT msg);
#endif    
#ifdef _CN_ZY_AUTH_PROCESS
#ifdef _SERVER
    static BOOLEAN PreVerifier(const MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN* msg, WORD msg_size);
#endif
#endif //_CN_ZY_AUTH_PROCESS
}; //end of namespace


//==============================================================================
//  @brief: Sample Codes
//
#define __KR000000_090128_CHANNELING__SAMPLE__ (0)


#if __KR000000_090128_CHANNELING__SAMPLE__
{
    WLAUTH_INFO auth_info; //<-- 값이 이미 설정되어 있다.
    MSG_CF_AUTH_TOKEN_PASS_SYN msg;
    WORD msg_size = nsPacket::CSAuthFlow::MakeStream(auth_info, msg);
    if(msg_size) {
        Crypt::PacketEncode((BYTE*)&msg->info_stream,
                            msg_size,
                            (BYTE*)&msg->info_stream,
                            encode_key /*AuthServer에서 받아둔 키*/);

        msg_size += (WORD)sizeof(MSG_BASE);
        GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &msg, msg_size);
        //SendPacket ~~
    }
    else {
        // error handling
    }
}
#endif

}; // end of namespace nsPacket
//
//
//==============================================================================

//#endif //__KR000000_090128_CHANNELING__


#endif //_PACKETCONTROL_AUTH_FLOW_H_

