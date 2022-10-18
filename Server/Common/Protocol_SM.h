
#ifndef __PROTOCOL_SM_H__
#define __PROTOCOL_SM_H__

#include <Protocol.h>

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//
// * Postfix
//		SYN - Server에 요청
//		ACK - 요청에 대한 허락
//		NAK - 요청에 대한 거부
//		CMD - Server의 명령
//		BRD - Server가 동기화 관련 내용을 알림
//		DBR - DBProxy에서만 사용함!
//
// * 프로토콜 Naming Rule
//		Prefix_카테고리명_프로토콜명_Postfix
//		ex) CG_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------


// Solar Auth Server는 보안상의 이유로 마스터가 Passive모드로 동작하도록 유도한다.
// 즉, SM간에 요청하는 주체가 (S)olar auth server가 되는 경우가 많다.
// _CMD 가 되겠군...

// SM에서 각 마스터는 프로토콜 버전이 달라질 수 있다. random number로 하기에는 무리가 있다.
// 기존의 마스터 정보를 감안해야 할 것이다.

enum eSM_CATEGORY
{
	SM_CONNECTION		= 11,
	SM_SERVER_INFO		= 81,
	SM_CRYPTOGRAPHY		= 161,
	SM_DATA_TRANSFER	= 221,
};

enum eSM_CONNECTION_PROTOCOL
{
	SM_CONNECTION_HEARTBEAT			= 92,

	// 첫번째 접속 단계
	SM_CONNECTION_CONNECTION_SYN	= 102,
	SM_CONNECTION_CONNECTION_ACK	= 122,
	SM_CONNECTION_CONNECTION_NAK	= 142,

	SM_CONNECTION_DISCONNECT_CMD	= 162,

	SM_CONNECTION_PHASE_SHIFT_CMD	= 172,
	SM_CONNECTION_BLOCK_CMD			= 182,

	// 서버 시작 명령은 어떻게 할꾜?
	// 특정 서버 중지 명령 필요한교?
};

enum eSM_SERVER_INFO_PROTOCOL
{
	// 두번째 상호 인증 단계 - 서로 간에 아래와 같은 과정을 수행한다.
	SM_SERVER_INFO_DESTINATION_KEY_SYN			= 53,
	SM_SERVER_INFO_DESTINATION_KEY_ACK			= 63,
	SM_SERVER_INFO_DESTINATION_KEY_NAK			= 73,

	SM_SERVER_INFO_QUERY_SYN					= 93,
	SM_SERVER_INFO_QUERY_ACK					= 103,
	SM_SERVER_INFO_QUERY_CMD					= 113,
	SM_SERVER_INFO_QUERY_NAK					= 123,

	// 이건 보류. 시간이 많이 걸릴 듯.
	SM_SERVER_INFO_AUTH_KEY_CHANGE_CMD			= 133,
	SM_SERVER_INFO_AUTH_KEY_CHANGE_ACK			= 143,
	SM_SERVER_INFO_AUTH_KEY_CHANGE_NAK			= 153,
	SM_SERVER_INFO_AUTH_KEY_DATA_CMD			= 163,
	SM_SERVER_INFO_AUTH_KEY_DATA_ACK			= 173,

	SM_SERVER_INFO_AUTHENTICATION_SYN			= 203,
	SM_SERVER_INFO_AUTHENTICATION_ACK			= 213,
	SM_SERVER_INFO_AUTHENTICATION_NAK			= 223,

	//__NA000000_070523_REPORT_TO_SOLARAUTH
	SM_SERVER_INFO_CACHEITEM_REPORT_NTF			= 233,
	SM_SERVER_INFO_CCU_REPORT_NTF				= 234,
};

enum eSM_CRYPTOGRAPHY_PROTOCOL
{
	SM_CRYPTOGRAPHY_KEY						= 34,				// 패킷 암호화 키 교환
	SM_CRYPTOGRAPHY_ACK						= 44,				
	SM_CRYPTOGRAPHY_NAK						= 54,				
	SM_CRYPTOGRAPHY_CMD						= 64,				

	SM_CRYPTOGRAPHY_PACKET_ENCODED			= 254,			// 144 + n	
	SM_CRYPTOGRAPHY_PACKET_ENCODING			= 255,				

	SM_CRYPTOGRAPHY_SNK_KEY					= 84,						
	SM_CRYPTOGRAPHY_SNK_CMD					= 94,						
	SM_CRYPTOGRAPHY_SNK_ACK					= 104,						
	SM_CRYPTOGRAPHY_SNK_NAK					= 114,						
};

enum eSM_DATA_TRANSFER_PROTOCOL
{
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN	= 67,	// M->S
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK	= 77,	// S->M with DATA INFO
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK	= 87,	// S->M
	// 데이터 전송 : 옵셋 전송으로 할까 말까...
	SM_DATA_TRANSFER_SENDDATA_CMD			= 97,	// S->M with Offset
	SM_DATA_TRANSFER_SENDDATA_ACK			= 107,	// M->S with CRC
	SM_DATA_TRANSFER_SENDDATA_NAK			= 117,	// M->S
};



#endif //__PROTOCOL_SM_H__
