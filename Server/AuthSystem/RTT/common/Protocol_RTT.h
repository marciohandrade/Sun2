#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
// 국가별 기능 디파인

#ifdef _CHINA
#define _110503_CUSTOM_USERID_LENGTH // 국가별 사용자ID(계정이름) 길이 설정 사용
//	1. 중국에서 유저 계정길이를 기존 12에서 50으로 늘려달라는 요청에 의한 추가
// [TO-BE-DELETED] #define  _090223_USER_ID_LENGTH_50_CHINA_ONLY_	// 중국 유저 계정 길이 50자
//	2. 또!! 중국에서 웹 상에서 유저 강제 종료 기능을 RTT에 추가 시켜 달라는 요청에 의한 추가
#define  _090427_HTTP_WEB_DROP_USER				// 중국 웹 상에서 유저 강제 종료 기능 연동
#endif

#ifdef _KOREA
//	썬 채널링 서비스를 통해 들어온 한게임 및 피망 계정 유저의 동접 정보를 마스터 서버로부터 수신한다(현재 국내만 적용)
#define  _090901_CHANNELLING_CONNECT_USER_
#endif

#ifdef _RUSSIA
#define _110503_CUSTOM_USERID_LENGTH // 국가별 사용자ID(계정이름) 길이 설정 사용
#endif

//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef  DESCLEN
	#define  DESC_LEN					30
#endif

#ifdef _110503_CUSTOM_USERID_LENGTH
  #if defined(_CHINA)
    #define ID_MAX_LEN 50
  #elif defined(_RUSSIA)
    #define ID_MAX_LEN 24
  #endif
#endif // _110503_CUSTOM_USERID_LENGTH
#ifndef ID_MAX_LEN
#define ID_MAX_LEN 24
#endif

#ifndef  IP_MAX_LEN
	#define  IP_MAX_LEN					16
#endif


#pragma pack(1)

#define  SIZEOFHDR_SUN					2				//  Sun에서는 size를 2byte 차감한다.

#define  SIZEOFHDR						SIZEOFHDR_SUN

// 웹 감시 URL 프로토콜( 06-12-05, 김종호 )
#define DEFAULT_AGENT_NAME "RTTAgent"		// 감시할 응용 프로그램의 이름
#define MAX_URL				1024			// URL 최대길이
#define MAX_FILENAME		256				// 파일이름 최대길이

//////////////////////////////////////////////////////////////////////////
//  Agent Type
#define  SERVERTYPE_AGENT				0x01			//  RTT Agent
#define  SERVERTYPE_AUTH				0x02			//  AuthServer
#define  SERVERTYPE_MASTER				0x04			//  MasterServer


//////////////////////////////////////////////////////////////////////////
//  Category
#define  CATEGORY_GENERAL				0x00
#define  CATEGORY_WOPS					0x01
#define  CATEGORY_RTT					0x02
#define  CATEGORY_HTTP					0x03


//------------------------------------------------------------------------
//  Message list - WOPS
#define  RTTW_CERTIFY_REQ				( CATEGORY_WOPS | 0x01 << 8 )
#define  RTTW_CERTIFY_ANS				( CATEGORY_WOPS | 0x02 << 8 )
#define  RTTW_DISCERTIFY_NTF			( CATEGORY_WOPS | 0x03 << 8 )
#define  RTTW_SERVERLIST_REQ			( CATEGORY_WOPS | 0x04 << 8 )
#define  RTTW_SERVERLIST_ANS			( CATEGORY_WOPS | 0x05 << 8 )
#define  RTTW_GAMENOTICE_REQ			( CATEGORY_WOPS | 0x06 << 8 )
#define  RTTW_GAMENOTICE_ANS			( CATEGORY_WOPS | 0x07 << 8 )
#define  RTTW_GAMEADMINLIST_REQ			( CATEGORY_WOPS | 0x08 << 8 )
#define  RTTW_GAMEADMINLIST_ANS			( CATEGORY_WOPS | 0x09 << 8 )
#define  RTTW_GAMEADMINADD_NTF			( CATEGORY_WOPS | 0x0A << 8 )
#define  RTTW_GAMEADMINREMOVE_NTF		( CATEGORY_WOPS | 0x0B << 8 )
#define  RTTW_DROPUSER_REQ				( CATEGORY_WOPS | 0x0C << 8 )
#define  RTTW_DROPUSER_ANS				( CATEGORY_WOPS | 0x0D << 8 )
#define  RTTW_USERCOUNT_NTF				( CATEGORY_WOPS | 0x0E << 8 )
#define  RTTW_RTTASTART_REQ				( CATEGORY_WOPS | 0x0F << 8 )
#define  RTTW_RTTASTART_ANS				( CATEGORY_WOPS | 0x10 << 8 )
#define  RTTW_RTTASTOP_REQ				( CATEGORY_WOPS | 0x11 << 8 )
#define  RTTW_RTTASTOP_ANS				( CATEGORY_WOPS | 0x12 << 8 )
#define  RTTW_RTTAPATCH_NTF				( CATEGORY_WOPS | 0x13 << 8 )
#define  RTTW_SYSTEMREBOOT_NTF			( CATEGORY_WOPS | 0x14 << 8 )
#define  RTTW_PROCESSCONTROL_NTF		( CATEGORY_WOPS | 0x15 << 8 )
#define  RTTW_BATTLEZONEINFO_REQ		( CATEGORY_WOPS | 0x16 << 8 )
#define  RTTW_BATTLEZONEINFO_ANS		( CATEGORY_WOPS | 0x17 << 8 )
#define  RTTW_ARMORKIT_FINDHACK_NTF		( CATEGORY_WOPS | 0x18 << 8 )
#define  RTTW_ABUSING_NTF				( CATEGORY_WOPS | 0x19 << 8 )
#define  RTTW_ERRORNOTIFY_NTF			( CATEGORY_WOPS | 0x1F << 8 )
#define  RTTW_CONNECTIONTYPE_NTF		( CATEGORY_WOPS | 0x20 << 8 )
#define  RTTW_USERMONEY_REQ				( CATEGORY_WOPS | 0x21 << 8 )
#define  RTTW_USERMONEY_ANS				( CATEGORY_WOPS | 0x22 << 8 )
#define  RTTW_SERVERSHUTDOWN_REQ		( CATEGORY_WOPS | 0x23 << 8 )
#define  RTTW_SERVERSHUTDOWN_ANS		( CATEGORY_WOPS | 0x24 << 8 )
#define  RTTW_LAUNCHPATCHER_REQ			( CATEGORY_WOPS | 0x25 << 8 )
#define  RTTW_PATCHRESULT_REQ			( CATEGORY_WOPS | 0x26 << 8 )
#define  RTTW_PATCHRESULT_ANS			( CATEGORY_WOPS | 0x27 << 8 )
#define  RTTW_LAUNCHPROCESS_REQ			( CATEGORY_WOPS | 0x28 << 8 )
#define  RTTW_LAUNCHPROCESS_ANS			( CATEGORY_WOPS | 0x29 << 8 )

#define	 RTTW_FUNCRESTRICT_RELOAD_NTF	( CATEGORY_WOPS | 0x30 << 8 )
#define	 RTTW_FUNCRESTRICT_RELOAD_ANS	( CATEGORY_WOPS | 0x31 << 8 )

#define	 RTTW_AUTHIPLIST_RELOAD_REQ		( CATEGORY_WOPS | 0x32 << 8 )
#define	 RTTW_AUTHIPLIST_RELOAD_ANS		( CATEGORY_WOPS | 0x33 << 8 )
#define	 RTTW_GAMEVERSION_UPDATE_REQ	( CATEGORY_WOPS | 0x34 << 8 )
#define	 RTTW_GAMEVERSION_UPDATE_ANS	( CATEGORY_WOPS | 0x35 << 8 )

#define	 RTTW_GAMESTATISTICS_REQ		( CATEGORY_WOPS | 0x36 << 8 )		// 게임 통계 요청
#define  RTTW_GAMESTATISTICS_ANS		( CATEGORY_WOPS | 0x37 << 8 )		// 게임 통계 응답

// 2006. 8. 14 김종호 패턴 추가
#define	 RTTW_PATTERN_RELOAD_REQ		( CATEGORY_WOPS | 0x38 << 8 )		// 보안 패턴 갱신 요청
#define  RTTW_PATTERN_RELOAD_ANS		( CATEGORY_WOPS | 0x39 << 8 )		// 보안 패턴 갱신 응답

// to auth
// Armorkit dispatch command 
#define	 RTTW_ARMORKIT_DISPATCH_NTF		( CATEGORY_WOPS | 0x40 << 8 )		
#define	 RTTW_ARMORKIT_DISPATCH_ANS		( CATEGORY_WOPS | 0x41 << 8 )

#define	 RTTW_CHANGE_GUILDNAME_REQ		( CATEGORY_WOPS | 0x42 << 8 )		
#define	 RTTW_CHANGE_GUILDNAME_ANS		( CATEGORY_WOPS | 0x43 << 8 )
#define	 RTTW_SEND_MESSAGE_REQ			( CATEGORY_WOPS | 0x44 << 8 )		
#define	 RTTW_SEND_MESSAGE_ANS			( CATEGORY_WOPS | 0x45 << 8 )


#define  RTTW_GMSCOMMAND_REQ			( CATEGORY_WOPS | 0xF0 << 8 )
#define  RTTW_GMSCOMMAND_ANS			( CATEGORY_WOPS | 0xF1 << 8 )


//------------------------------------------------------------------------
//  Message list - GENERAL
#define  RTTG_CERTIFY_REQ				( CATEGORY_GENERAL | 0x01 << 8 )
#define  RTTG_CERTIFY_ANS				( CATEGORY_GENERAL | 0x02 << 8 )
#define  RTTG_GAMENOTICE_REQ			( CATEGORY_GENERAL | 0x03 << 8 )
#define  RTTG_GAMENOTICE_ANS			( CATEGORY_GENERAL | 0x04 << 8 )		//  Not using at General
#define  RTTG_GAMEADMINLIST_REQ			( CATEGORY_GENERAL | 0x05 << 8 )
#define  RTTG_GAMEADMINLIST_ANS			( CATEGORY_GENERAL | 0x06 << 8 )
#define  RTTG_GAMEADMINADD_NTF			( CATEGORY_GENERAL | 0x07 << 8 )
#define  RTTG_GAMEADMINREMOVE_NTF		( CATEGORY_GENERAL | 0x08 << 8 )
#define  RTTG_DROPUSER_REQ				( CATEGORY_GENERAL | 0x09 << 8 )
#define  RTTG_DROPUSER_ANS				( CATEGORY_GENERAL | 0x0A << 8 )
#define  RTTG_USERCOUNT_NTF				( CATEGORY_GENERAL | 0x0B << 8 )
#define  RTTG_BATTLEZONEINFO_REQ		( CATEGORY_GENERAL | 0x0C << 8 )
#define  RTTG_BATTLEZONEINFO_ANS		( CATEGORY_GENERAL | 0x0D << 8 )
#define  RTTG_ARMORKIT_FINDHACK_NTF		( CATEGORY_GENERAL | 0x0E << 8 )
#define  RTTG_ABUSING_NTF				( CATEGORY_GENERAL | 0x0F << 8 )
#define  RTTG_USERMONEY_REQ				( CATEGORY_GENERAL | 0x10 << 8 )
#define  RTTG_USERMONEY_ANS				( CATEGORY_GENERAL | 0x11 << 8 )
#define  RTTG_SERVERSHUTDOWN_REQ		( CATEGORY_GENERAL | 0x12 << 8 )
#define  RTTG_SERVERSHUTDOWN_ANS		( CATEGORY_GENERAL | 0x13 << 8 )
#define  RTTG_PATCHRESULT_REQ			( CATEGORY_GENERAL | 0x14 << 8 )
#define  RTTG_PATCHRESULT_ANS			( CATEGORY_GENERAL | 0x15 << 8 )

#define	 RTTG_FUNCRESTRICT_RELOAD_NTF	( CATEGORY_GENERAL | 0x18 << 8 )
#define	 RTTG_FUNCRESTRICT_RELOAD_ANS	( CATEGORY_GENERAL | 0x19 << 8 )

#define	 RTTG_GAMESTATISTICS_REQ		( CATEGORY_GENERAL | 0x1B << 8 )		// 게임 통계 요청
#define  RTTG_GAMESTATISTICS_ANS		( CATEGORY_GENERAL | 0x1C << 8 )		// 게임 통계 응답

#define	 RTTG_AUTHIPLIST_RELOAD_REQ		( CATEGORY_GENERAL | 0x20 << 8 )
#define	 RTTG_AUTHIPLIST_RELOAD_ANS		( CATEGORY_GENERAL | 0x21 << 8 )

#define	 RTTG_GAMEVERSION_UPDATE_REQ	( CATEGORY_GENERAL | 0x22 << 8 )
#define	 RTTG_GAMEVERSION_UPDATE_ANS	( CATEGORY_GENERAL | 0x23 << 8 )

// 2006. 8. 14 김종호 패턴 추가
#define	 RTTG_PATTERN_RELOAD_REQ		( CATEGORY_GENERAL | 0x24 << 8 )		// 보안 패턴 갱신 요청
#define  RTTG_PATTERN_RELOAD_ANS		( CATEGORY_GENERAL | 0x25 << 8 )		// 보안 패턴 갱신 응답

#define	 RTTG_ARMORKIT_DISPATCH_NTF		( CATEGORY_GENERAL | 0x26 << 8 )		
#define	 RTTG_ARMORKIT_DISPATCH_ANS		( CATEGORY_GENERAL | 0x27 << 8 )

#define	 RTTG_CHANGE_GUILDNAME_REQ		( CATEGORY_GENERAL | 0x28 << 8 )		
#define	 RTTG_CHANGE_GUILDNAME_ANS		( CATEGORY_GENERAL | 0x29 << 8 )
#define	 RTTG_SEND_MESSAGE_REQ			( CATEGORY_GENERAL | 0x2A << 8 )		
#define	 RTTG_SEND_MESSAGE_ANS			( CATEGORY_GENERAL | 0x2B << 8 )

#define  RTTG_GMSCOMMAND_REQ			( CATEGORY_GENERAL | 0xF0 << 8 )
#define  RTTG_GMSCOMMAND_ANS			( CATEGORY_GENERAL | 0xF1 << 8 )


//------------------------------------------------------------------------
//  Message list - RTT( Server, Agent, Service )
#define  RTTR_RESOURCE_NTF				( CATEGORY_RTT | 0x01 << 8 )
#define  RTTR_RTTASTART_NTF				( CATEGORY_RTT | 0x02 << 8 )
#define  RTTR_RTTASTOP_NTF				( CATEGORY_RTT | 0x03 << 8 )
#define  RTTR_RTTAPATCH_NTF				( CATEGORY_RTT | 0x04 << 8 )
#define  RTTR_PROCESSLIST_REQ			( CATEGORY_RTT | 0x06 << 8 )
#define  RTTR_PROCESSLIST_ANS			( CATEGORY_RTT | 0x07 << 8 )
#define  RTTR_PROCESSMONITOR_NTF		( CATEGORY_RTT | 0x08 << 8 )
#define  RTTR_PATCHLAUNCHERINFO_NTF		( CATEGORY_RTT | 0x09 << 8 )
#define  RTTR_WEB_URL_REQ				( CATEGORY_RTT | 0x0A << 8 )	// 감시 URL 요청
#define  RTTR_WEB_URL_ANS				( CATEGORY_RTT | 0x0B << 8 )	// 감시 URL 전송
#define  RTTR_WEB_REQUEST_NTF			( CATEGORY_RTT | 0x0C << 8 )	// 감시 URL Request 결과
#define  RTTR_HEARTBEAT_NTF				( CATEGORY_RTT | 0xff << 8 )


//------------------------------------------------------------------------
//  Message list - HTTP WEB
#define  RTTH_DROPUSER_REQ				( CATEGORY_HTTP | 0x01 << 8 )		// 중국 웹 상에서 유저 강제 종료 요청
#define  RTTH_DROPUSER_ANS				( CATEGORY_HTTP | 0x02 << 8 )		// 중국 웹 상에서 유저 강제 종료 요청 결과


//////////////////////////////////////////////////////////////////////////
//  Definition

typedef struct tagSERVER_KEY
{
	DWORD dwKey;

	tagSERVER_KEY( const DWORD key = 0 ) { dwKey = key; }

	operator DWORD()  { return dwKey; }

	DWORD Set( BYTE worldID, BYTE channelID, BYTE serverType, BYTE serverID )
	{
		memcpy( &dwKey, &worldID, sizeof(BYTE) );
		dwKey <<= 8; memcpy( &dwKey, &channelID, sizeof(BYTE) );
		dwKey <<= 8; memcpy( &dwKey, &serverType, sizeof(BYTE) );
		dwKey <<= 8; memcpy( &dwKey, &serverID, sizeof(BYTE) );
		return dwKey;
	}

	BYTE GetWorldID()  { BYTE id; DWORD temp = dwKey; temp >>= 24; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetChannelID() { BYTE id; DWORD temp = dwKey; temp >>= 16; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetServerType()  { BYTE id; DWORD temp = dwKey; temp >>= 8; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetServerID()  { BYTE id; memcpy( &id, &dwKey, sizeof(BYTE) ); return id; }
} SERVER_KEY, *LPSERVER_KEY;

enum eZONETYPE
{
	eZONETYPE_LOBBY = 0,	// 로비(배틀존)
	eZONETYPE_VILLAGE,		// 마을
	eZONETYPE_CHARSELECT,	// 캐릭터 선택
	eZONETYPE_MISSION,		// 미션
	eZONETYPE_HUNTING,		// 헌팅
	eZONETYPE_QUEST,		// 퀘스트
	eZONETYPE_PVP,			// pvp
	eZONETYPE_EVENT,		// 이벤트
	eZONETYPE_GUILD,		// 길드
	eZONETYPE_SIEGE,		// 공성전
	eZONETYPE_FIELD,		// 필드(마을 동접과 같이 표시)
	eZONETYPE_CHUNTING,		// 경쟁 헌팅
	eZONETYPE_MAX,
};

enum eSTATISTICS_TYPE
{
	// 현재 유통되는 통화량(상거래)
	eSTATISTIC_MONEY_CIRCULATION,
	// 생긴돈 : 돈줍기,보상받기,
	eSTATISTIC_MONEY_PLUS,
	// 사라진돈 : 돈버림, 하임소비
	eSTATISTIC_MONEY_MINUS,

  	eSTATISTIC_MONEY_WAREHOUSE_PLUS,
	eSTATISTIC_MONEY_WAREHOUSE_MINUS,

	eSTATISTIC_JACKPOT_COUNT,
	eSTATISTIC_JACKPOT_MONEY,

	eSTATISTIC_FEELMISTONE_CREATE,
	eSTATISTIC_FEELMISTONE_DESTROY,
	eSTATISTIC_OVERMISTONE_CREATE,
	eSTATISTIC_OVERMISTONE_DESTROY,
	eSTATISTIC_STRENGTHMISTONE_CREATE,
	eSTATISTIC_STRENGTHMISTONE_DESTROY,
	eSTATISTIC_MEGAMISTONE_CREATE,
	eSTATISTIC_MEGAMISTONE_DESTROY,

  	eSTATISTIC_ENCHANT_COUNT,
	eSTATISTIC_RANKUP_COUNT,

	//eSTATISTIC_COMPOSE_COUNT,
	// 결정화
	eSTATISTIC_CRYSTALIZE_COUNT,

  	// 조합관련
	eSTATISTIC_COMPOSE_RING_LOW,
	eSTATISTIC_COMPOSE_RING_HIGH,
	eSTATISTIC_COMPOSE_NECKLACE_LOW,
	eSTATISTIC_COMPOSE_NECKLACE_HIGH,
	eSTATISTIC_COMPOSE_SOUL_SHEILD_LOW,
	eSTATISTIC_COMPOSE_SOUL_SHEILD_HIGH,
	eSTATISTIC_COMPOSE_SOUL_ARMLET_LOW,
	eSTATISTIC_COMPOSE_SOUL_ARMLET_HIGH,
	eSTATISTIC_COMPOSE_SOUL_WING_LOW,
	eSTATISTIC_COMPOSE_SOUL_WING_HIGH,
	eSTATISTIC_COMPOSE_SOUL_EYE_LOW,
	eSTATISTIC_COMPOSE_SOUL_EYE_HIGH,

  	eSTATISTIC_COMPOSE_EXTRASTONE,
	eSTATISTIC_COMPOSE_FEELMISTONE,
	eSTATISTIC_COMPOSE_OVERMISTONE,
	eSTATISTIC_COMPOSE_STRENGTHMISTONE,
	eSTATISTIC_COMPOSE_MEGAMISTONE,
	eSTATISTIC_COMPOSE_GIGAMISTONE,

	eSTATISTIC_COMPOSE_PROTECTION_CRYSTAL,
	eSTATISTIC_COMPOSE_SHINING_CRYSTAL,
	eSTATISTIC_COMPOSE_UNIQUE_CRYSTAL,
	eSTATISTIC_COMPOSE_GARNET_CRYSTAL,
	eSTATISTIC_COMPOSE_NOBLESS_CRYSTAL,

	eSTATISTIC_MAX,
};

typedef struct tag_Statistic
{
	tag_Statistic()
	{
uiEnumStatistic[eSTATISTIC_MONEY_CIRCULATION         ] =  0;
uiEnumStatistic[eSTATISTIC_MONEY_PLUS                ] =  0;
uiEnumStatistic[eSTATISTIC_MONEY_MINUS               ] =  0;
uiEnumStatistic[eSTATISTIC_MONEY_WAREHOUSE_PLUS      ] =  0;
uiEnumStatistic[eSTATISTIC_MONEY_WAREHOUSE_MINUS     ] =  0;
uiEnumStatistic[eSTATISTIC_JACKPOT_COUNT             ] =  0;
uiEnumStatistic[eSTATISTIC_JACKPOT_MONEY             ] =  0;
uiEnumStatistic[eSTATISTIC_FEELMISTONE_CREATE        ] =  1;
uiEnumStatistic[eSTATISTIC_FEELMISTONE_DESTROY       ] =  2;
uiEnumStatistic[eSTATISTIC_OVERMISTONE_CREATE        ] =  4;
uiEnumStatistic[eSTATISTIC_OVERMISTONE_DESTROY       ] =  5;
uiEnumStatistic[eSTATISTIC_STRENGTHMISTONE_CREATE    ] =  7;
uiEnumStatistic[eSTATISTIC_STRENGTHMISTONE_DESTROY   ] =  8;
uiEnumStatistic[eSTATISTIC_MEGAMISTONE_CREATE        ] = 10;
uiEnumStatistic[eSTATISTIC_MEGAMISTONE_DESTROY       ] = 11;
uiEnumStatistic[eSTATISTIC_ENCHANT_COUNT             ] = 32;
uiEnumStatistic[eSTATISTIC_RANKUP_COUNT              ] = 33;
uiEnumStatistic[eSTATISTIC_CRYSTALIZE_COUNT          ] = 34;
uiEnumStatistic[eSTATISTIC_COMPOSE_RING_LOW          ] = 15;
uiEnumStatistic[eSTATISTIC_COMPOSE_RING_HIGH         ] = 16;
uiEnumStatistic[eSTATISTIC_COMPOSE_NECKLACE_LOW      ] = 17;
uiEnumStatistic[eSTATISTIC_COMPOSE_NECKLACE_HIGH     ] = 18;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_SHEILD_LOW   ] = 19;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_SHEILD_HIGH  ] = 20;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_ARMLET_LOW   ] = 21;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_ARMLET_HIGH  ] = 22;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_WING_LOW     ] = 23;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_WING_HIGH    ] = 24;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_EYE_LOW      ] = 25;
uiEnumStatistic[eSTATISTIC_COMPOSE_SOUL_EYE_HIGH     ] = 26;
uiEnumStatistic[eSTATISTIC_COMPOSE_EXTRASTONE        ] = 13;
uiEnumStatistic[eSTATISTIC_COMPOSE_FEELMISTONE       ] =  3;
uiEnumStatistic[eSTATISTIC_COMPOSE_OVERMISTONE       ] =  6;
uiEnumStatistic[eSTATISTIC_COMPOSE_STRENGTHMISTONE   ] =  9;
uiEnumStatistic[eSTATISTIC_COMPOSE_MEGAMISTONE       ] = 12;
uiEnumStatistic[eSTATISTIC_COMPOSE_GIGAMISTONE       ] = 14;
uiEnumStatistic[eSTATISTIC_COMPOSE_PROTECTION_CRYSTAL] = 27;
uiEnumStatistic[eSTATISTIC_COMPOSE_SHINING_CRYSTAL   ] = 28;
uiEnumStatistic[eSTATISTIC_COMPOSE_UNIQUE_CRYSTAL    ] = 29;
uiEnumStatistic[eSTATISTIC_COMPOSE_GARNET_CRYSTAL    ] = 30;
uiEnumStatistic[eSTATISTIC_COMPOSE_NOBLESS_CRYSTAL   ] = 31;
uiEnumStatistic[eSTATISTIC_MAX                       ] =  0;
	}

	UINT uiEnumStatistic[eSTATISTIC_MAX + 1];
}Statistic;

typedef struct tag_BATTLEZONE_INFO
{
	BYTE			byWorldID;
	BYTE			byChannelID;
	DWORD			dwZoneCount[eZONETYPE_MAX];		// 존 개설 수
} BATTLEZONE_INFO;

namespace Protocol_RTT{
class HDRMessageID4
{
public:
			BYTE		byCategory;
			BYTE		byMessageID;
};

class HDRMessageID8
{
public:
			WORD		wCategory;
			WORD		wMessageID;
};

class MSG_HEADER4
{
public:
			WORD		wSize;					//  HEADER 크기를 제외한 Payload 사이즈
			union
			{
				WORD			wID;			//  Message ID
				HDRMessageID4	hdrID;
			};
};

class MSG_HEADER8
{
public:
			DWORD		dwSize;					//  HEADER 크기를 제외한 Payload 사이즈
			union
			{
				DWORD			dwID;			//  Message ID
				HDRMessageID8	hdrID;
			};
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? → RTT
//  Description : RTT와 통신하는 프로그램은 RTT와 접속이 이루어진 후에 인증을 받아야한다.
class MSG_RTTW_CERTIFY_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_CERTIFY_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_CERTIFY_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_CERTIFY_REQ;
			}
			~MSG_RTTW_CERTIFY_REQ()
			{
			}


			DWORD			dwType;						//  AGENTTYPE_ define을 명시한다.
			DWORD			dwVersion;
};

class MSG_RTTG_CERTIFY_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_CERTIFY_REQ()
			{
				wSize	= sizeof( MSG_RTTG_CERTIFY_REQ ) - SIZEOFHDR;
				wID		= RTTG_CERTIFY_REQ;
			}
			~MSG_RTTG_CERTIFY_REQ()
			{
			}


			DWORD			dwType;						//  AGENTTYPE_ define을 명시한다.
			DWORD			dwVersion;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? → RTT
//  Description : RTT와 통신하는 프로그램은 RTT와 접속이 이루어진 후에 인증을 받아야한다.
class MSG_RTTW_CERTIFY_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_CERTIFY_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_CERTIFY_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_CERTIFY_ANS;
			}
			~MSG_RTTW_CERTIFY_ANS()
			{
			}

			DWORD			dwResult;					//  0 : 인증 성공,	1 : 버젼 틀림,	2 : 등록되지 않은 Agent
			DWORD			dwType;						//  SERVERTYPE_ define을 명시한다.
			DWORD			dwUID;						//  인증된 고유번호
};

class MSG_RTTG_CERTIFY_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_CERTIFY_ANS()
			{
				wSize	= sizeof( MSG_RTTG_CERTIFY_ANS ) - SIZEOFHDR;
				wID	= RTTG_CERTIFY_ANS;
			}
			~MSG_RTTG_CERTIFY_ANS()
			{
			}

			DWORD			dwResult;					//  0 : 인증 성공
			DWORD			dwType;						//  SERVERTYPE_ define을 명시한다.
			DWORD			dwUID;						//  인증된 고유번호
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : RTTA에 연결된 General Server가 연결이 끊겼을 때, 해당 서버의 type을 전송한다.
class MSG_RTTW_DISCERTIFY_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_DISCERTIFY_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_DISCERTIFY_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_DISCERTIFY_NTF;
			}
			~MSG_RTTW_DISCERTIFY_NTF()
			{
			}

			DWORD			dwType;						//  SERVERTYPE_ define을 명시한다.
			DWORD			dwUID;						//  인증된 uid값을 넣는다.
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT
//  Description : 게임 서버의 채널 목록을 요청한다.
class MSG_RTTW_SERVERLIST_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_SERVERLIST_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_SERVERLIST_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SERVERLIST_REQ;
			}
			~MSG_RTTW_SERVERLIST_REQ()
			{
			}
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → GMS
//  Description : 채널 목록을 전송한다.
class MSG_RTTW_SERVERLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_SERVERLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_SERVERLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SERVERLIST_ANS;
				memset(&szList[0], 0, sizeof(wchar_t) * 5120);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_SERVERLIST_ANS()
			{
			}

			DWORD			dwLength;					//  szList의 실제 문자열 길이
			wchar_t			szList[5120];				//  채널명1,GUID1;채널명2,GUID2;...
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → General
//  Description : 게임내 공지를 발송한다.
#define NOTICE_LENGTH	1025

class MSG_RTTW_GAMENOTICE_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMENOTICE_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GAMENOTICE_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMENOTICE_REQ;
				memset(&szNotice[0], 0, sizeof(wchar_t) * (NOTICE_LENGTH-1) ); // 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GAMENOTICE_REQ()
			{
			}

			DWORD				dwServerUID;				//  공지를 전송할 서버의 uid.(0=전체)
			DWORD				dwTickPerLine;				//  1라인 출력 시간간격(ms)
			DWORD				dwNoticeLength;				//  공지 문자열의 길이
			wchar_t				szNotice[NOTICE_LENGTH-1];	//  공지 문자열
			BYTE				btRed;						//  공지문자색상
			BYTE				btGreen;					//
			BYTE				btBlue;						//
};

class MSG_RTTG_GAMENOTICE_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMENOTICE_REQ()
			{
				wSize	= sizeof( MSG_RTTG_GAMENOTICE_REQ ) - SIZEOFHDR;
				wID		= RTTG_GAMENOTICE_REQ;
				memset(&szNotice[0], 0, NOTICE_LENGTH);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_GAMENOTICE_REQ()
			{
			}

			DWORD				dwServerUID;				//  공지를 전송할 서버의 uid.(0=전체)
			DWORD				dwTickPerLine;				//  1라인 출력 시간간격(ms)
			DWORD				dwNoticeLength;				//  공지 문자열의 길이
			char				szNotice[NOTICE_LENGTH];	//  공지 문자열
			BYTE				btRed;						//  공지문자색상
			BYTE				btGreen;					//
			BYTE				btBlue;						//
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → GMS
//  Description : 공지가 게임서버로 전송이 완료되었는지를 통지한다.
class MSG_RTTW_GAMENOTICE_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMENOTICE_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GAMENOTICE_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMENOTICE_ANS;
			}
			~MSG_RTTW_GAMENOTICE_ANS()
			{
			}

			DWORD				dwError;
			//  Error values
			//  0 : 성공
			//  1 : RTT Agent와의 통신 두절
			//  2 : General와의 통신 두절
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS, General → RTT
//  Description : 게임내 관리자 계정 목록을 요청한다.
class MSG_RTTW_GAMEADMINLIST_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINLIST_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINLIST_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINLIST_REQ;
			}
			~MSG_RTTW_GAMEADMINLIST_REQ()
			{
			}
};

class MSG_RTTG_GAMEADMINLIST_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINLIST_REQ()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINLIST_REQ ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINLIST_REQ;
			}
			~MSG_RTTG_GAMEADMINLIST_REQ()
			{
			}
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → GMS, General
//  Description : 관리자 계정 목록을 전송한다.
class MSG_RTTW_GAMEADMINLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINLIST_ANS;
				memset(&szList[0], 0, sizeof(wchar_t) * 1024); // 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GAMEADMINLIST_ANS()
			{
			}

			DWORD				dwLength;				//  szList에 저장된 문자열의 길이
			wchar_t				szList[1024];			//  계정 정보
};

class MSG_RTTG_GAMEADMINLIST_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINLIST_ANS()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINLIST_ANS ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINLIST_ANS;
			}
			~MSG_RTTG_GAMEADMINLIST_ANS()
			{
			}

			DWORD				dwCount;				//  계정 정보의 개수
			//  이하 다음 데이터들이 dwCount만큼 반복
			//	char				szAccount[ID_MAX_LEN];
			//  char				szIP[IP_MAX_LEN];
			//  DWORD				dwAttr;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → General
//  Description : 관리자 계정을 추가한다.
class MSG_RTTW_GAMEADMINADD_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINADD_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINADD_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINADD_NTF;
				memset(&szAccount[0], 0, sizeof(wchar_t) * ID_MAX_LEN + 1);	// 07-01-03 김종호(버퍼 초기화)
                memset(&szIP[0], 0, sizeof(wchar_t) * IP_MAX_LEN);			// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GAMEADMINADD_NTF()
			{
			}

			wchar_t			szAccount[ID_MAX_LEN + 1];	//  추가할 계정
			wchar_t			szIP[IP_MAX_LEN];			//  허용 IP
			DWORD			dwAttr;						//  속성
};

class MSG_RTTG_GAMEADMINADD_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINADD_NTF()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINADD_NTF ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINADD_NTF;
				memset(&szAccount[0], 0, ID_MAX_LEN	+ 1);	// 07-01-03 김종호(버퍼 초기화)
                memset(&szIP[0], 0, IP_MAX_LEN);			// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_GAMEADMINADD_NTF()
			{
			}

			char			szAccount[ID_MAX_LEN + 1];	//  추가할 계정
			char			szIP[IP_MAX_LEN];			//  허용 IP
			DWORD			dwAttr;						//  속성
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → GMS → General
//  Description : 관리자 계정을 삭제한다.
class MSG_RTTW_GAMEADMINREMOVE_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINREMOVE_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINREMOVE_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINREMOVE_NTF;
				memset(&szAccount[0], 0, sizeof(wchar_t) * ID_MAX_LEN + 1);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GAMEADMINREMOVE_NTF()
			{
			}

			wchar_t			szAccount[ID_MAX_LEN + 1];		//  삭제할 계정
};

class MSG_RTTG_GAMEADMINREMOVE_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINREMOVE_NTF()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINREMOVE_NTF ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINREMOVE_NTF;
				memset(&szAccount[0], 0, ID_MAX_LEN + 1);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_GAMEADMINREMOVE_NTF()
			{
			}

			char			szAccount[ID_MAX_LEN + 1];		//  삭제할 계정
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → General
//  Description : 임의의 메시지를 지정된 목적지로 전송한다.
class MSG_RTTW_GMSCOMMAND_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GMSCOMMAND_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GMSCOMMAND_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GMSCOMMAND_REQ;
				memset(&szMessage[0], 0, sizeof(wchar_t) * 1024);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GMSCOMMAND_REQ()
			{
			}

			DWORD				dwUID;				//	목적 Application의 고유 번호
			DWORD				dwSize;				//  szMessage의 문자열 길이
			wchar_t				szMessage[1024];	//  메지시 버퍼
};

class MSG_RTTG_GMSCOMMAND_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_GMSCOMMAND_REQ()
			{
				wSize	= sizeof( MSG_RTTG_GMSCOMMAND_REQ ) - SIZEOFHDR;
				wID		= RTTG_GMSCOMMAND_REQ;
			}
			~MSG_RTTG_GMSCOMMAND_REQ()
			{
			}

			DWORD				dwUID;				//	목적 Application의 고유 번호
			DWORD				dwSize;				//  szMessage의 문자열 길이
			BYTE				szMessage[1024];	//  메지시 버퍼
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTT → GMS
//  Description : GMS 명령의 처리 결과를 GMS로 통지한다.
class MSG_RTTW_GMSCOMMAND_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GMSCOMMAND_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GMSCOMMAND_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GMSCOMMAND_ANS;
				memset(&szMessage[0], 0, sizeof(wchar_t) * 1024);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_GMSCOMMAND_ANS()
			{
			}

			DWORD				dwSize;				//  szMessage의 문자열 길이
			wchar_t				szMessage[1024];	//  메지시 버퍼
};

class MSG_RTTG_GMSCOMMAND_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_GMSCOMMAND_ANS()
			{
				wSize	= sizeof( MSG_RTTG_GMSCOMMAND_ANS ) - SIZEOFHDR;
				wID		= RTTG_GMSCOMMAND_ANS;
			}
			~MSG_RTTG_GMSCOMMAND_ANS()
			{
			}

			DWORD				dwSize;				//  szMessage의 문자열 길이
			BYTE				szMessage[1024];	//  메지시 버퍼
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTT
//  Description : 각 채널의 동접 수를 통지한다.
class MSG_RTTW_USERCOUNT_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_USERCOUNT_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_USERCOUNT_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_USERCOUNT_NTF;
			}
			~MSG_RTTW_USERCOUNT_NTF()
			{
			}

			DWORD			dwWorldID;
			DWORD			dwChannelID;
			DWORD			dwFieldUserCount;
			DWORD			dwFieldPCRoomCount;
			DWORD			dwBattleUserCount;
			DWORD			dwBattleZonePCRoomCount;

#ifdef	_090901_CHANNELLING_CONNECT_USER_
            DWORD			dwHangameFieldUserCount;
			DWORD			dwHangameFieldPCRoomCount;
			DWORD			dwHangameBattleUserCount;
			DWORD			dwHangameBattleZonePCRoomCount;
			DWORD			dwPMangFieldUserCount;
			DWORD			dwPMangFieldPCRoomCount;
			DWORD			dwPMangBattleUserCount;
			DWORD			dwPMangBattleZonePCRoomCount;
#endif
};


class MSG_RTTG_USERCOUNT_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_USERCOUNT_NTF()
			{
				wSize	= sizeof( MSG_RTTG_USERCOUNT_NTF ) - SIZEOFHDR;
				wID		= RTTG_USERCOUNT_NTF;
			}
			~MSG_RTTG_USERCOUNT_NTF()
			{
			}

			DWORD			dwWorldID;
			DWORD			dwChannelID;
			DWORD			dwFieldUserCount;
			DWORD			dwFieldPCRoomCount;
			DWORD			dwBattleUserCount;
			DWORD			dwBattleZonePCRoomCount;

#ifdef	_090901_CHANNELLING_CONNECT_USER_
            DWORD			dwHangameFieldUserCount;
			DWORD			dwHangameFieldPCRoomCount;
			DWORD			dwHangameBattleUserCount;
			DWORD			dwHangameBattleZonePCRoomCount;
			DWORD			dwPMangFieldUserCount;
			DWORD			dwPMangFieldPCRoomCount;
			DWORD			dwPMangBattleUserCount;
			DWORD			dwPMangBattleZonePCRoomCount;
#endif
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 개설된 배틀존의 정보를 요청한다.
class MSG_RTTG_BATTLEZONEINFO_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_BATTLEZONEINFO_REQ()
			{
				wSize	= sizeof( MSG_RTTG_BATTLEZONEINFO_REQ ) - SIZEOFHDR;
				wID		= RTTG_BATTLEZONEINFO_REQ;
			}
			~MSG_RTTG_BATTLEZONEINFO_REQ()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTTA
//  Description : 개설된 배틀존의 정보를 보낸다.
class MSG_RTTG_BATTLEZONEINFO_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_BATTLEZONEINFO_ANS()
			{
				wSize	= sizeof( MSG_RTTG_BATTLEZONEINFO_ANS ) - SIZEOFHDR;
				wID		= RTTG_BATTLEZONEINFO_ANS;
			}
			~MSG_RTTG_BATTLEZONEINFO_ANS()
			{
			}

			BYTE					byCount;
			//  이하 BATTLEZONE_INFO로 byCount만큼 정보 받음
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTT
//  Description : 클라이언트의 ArmorKit에서 발견한 해킹감지
class MSG_RTTG_ARMORKIT_FINDHACK_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_ARMORKIT_FINDHACK_NTF()
			{
				wSize	= sizeof( MSG_RTTG_ARMORKIT_FINDHACK_NTF ) - SIZEOFHDR;
				wID		= RTTG_ARMORKIT_FINDHACK_NTF;
				memset(&szIP[0], 0, 16);		// 07-01-03 김종호(버퍼 초기화)
				memset(&szBuf[0], 0, 1024);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_ARMORKIT_FINDHACK_NTF()
			{
			}

			DWORD		dwGuid;			// 해킹이 감지된 클라이언트 guid
			char		szIP[16];		// 해킹이 감지된 클라이언트 IP주소
			DWORD		dwHackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
			DWORD		dwBufSize;		// Buffer 의 길이
			char		szBuf[1024];	// 데이터 버퍼-1024는 보안파트에서 전달받은 암묵적 최대값(성현창)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTT
//  Description : 게임 서버에서 체크한 어뷰징 정보
class MSG_RTTG_ABUSING_NTF : public MSG_HEADER4
{
public:

			enum { MAX_DESC_LENGTH = 1023, MAX_CHARNAME_LENGTH = 16, MAX_IP_LENGTH = 15, MAX_ID_LEN = 12, 
				MAX_SUBTYPE_NAME_LEN = 64, };


			MSG_RTTG_ABUSING_NTF()
			{
				wSize	= sizeof( MSG_RTTG_ABUSING_NTF ) - SIZEOFHDR;
				wID		= RTTG_ABUSING_NTF;

#ifndef  _110503_CUSTOM_USERID_LENGTH
				memset(&m_ptszUserID[0], 0, sizeof(char) * (MAX_ID_LEN + 1) );				// 07-06-11 김종호(버퍼 초기화)
#else
				memset(&m_ptszUserID[0], 0, sizeof(char) * (ID_MAX_LEN + 1) );
#endif
				memset(&m_ptszCharName[0], 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH + 1) );	// 07-01-31 김종호(버퍼 초기화)
				memset(&m_ptszIP[0], 0, sizeof(TCHAR) * (MAX_IP_LENGTH + 1) );				// 07-01-03 김종호(버퍼 초기화)
				memset(&m_ptszDesc[0], 0, sizeof(TCHAR) * (MAX_DESC_LENGTH + 1) );			// 07-01-03 김종호(버퍼 초기화)
				ZeroMemory(m_ptszSubTypeName, sizeof(m_ptszSubTypeName));	// 08-04-03 박동권. 똑같이하면왠지 베낀거같아서 약간 다르게 쓴...;(사실, 변수명이 길어서 라인이 너무 길어지기때문에 짧게하려고 바꿨는데... 괜시리 주석을 길게쓰다보니까 주석이 아주 그냥 하늘끝까지 닿겠네... 어느덧 200글자 돌파?! ㅋ
			}
			~MSG_RTTG_ABUSING_NTF()
			{
			}

 			DWORD64			m_dwTime;								// time - GetDateTime_YYMMDDHHMM()의 값을 사용한다
			SERVER_KEY		m_ServerKey;							// server
			DWORD			m_dwUserGuid;							// user guid
			DWORD			m_dwCharGuid;							// char guid

#ifndef  _110503_CUSTOM_USERID_LENGTH
			char			m_ptszUserID[MAX_ID_LEN + 1];					// user id
#else
			char			m_ptszUserID[ID_MAX_LEN + 1];					// user id
#endif
			TCHAR			m_ptszCharName[MAX_CHARNAME_LENGTH + 1];		// char name
			TCHAR			m_ptszIP[MAX_IP_LENGTH + 1];					// user ip
			BYTE			m_AbuseType;									// type
			BYTE			m_Level;										// level
			TCHAR			m_ptszSubTypeName[MAX_SUBTYPE_NAME_LEN + 1];	// typename(64)
			WORD			m_wLength;										// desc length
			TCHAR			m_ptszDesc[MAX_DESC_LENGTH + 1];				// desc
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → General
//  Description : 통화량 정보를 요청한다.
class MSG_RTTG_USERMONEY_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_USERMONEY_REQ()
			{
				wSize	= sizeof( MSG_RTTG_USERMONEY_REQ ) - SIZEOFHDR;
				wID		= RTTG_USERMONEY_REQ;
			}
			~MSG_RTTG_USERMONEY_REQ()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTT
//  Description : 통화량을 전송한다.
class MSG_RTTG_USERMONEY_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_USERMONEY_ANS()
			{
				wSize	= sizeof( MSG_RTTG_USERMONEY_ANS ) - SIZEOFHDR;
				wID		= RTTG_USERMONEY_ANS;
			}
			~MSG_RTTG_USERMONEY_ANS()
			{
			}

			BYTE	byWorldID;
			BYTE	byChannelID;
			BYTE	byServerType;
			BYTE	byServerID;

			// 개인 보유액 관련
			UINT64	ui64TotalGetMoney;		// 일정 시간 동안 생겨난 돈 총계
			UINT64	ui64TotalLoseMoney;		// 일정 시간 동안 없어진 돈 총계
			UINT64	ui64TotalUserMoney;		// 현재 동접 유저들이 갖고 있는 평균 금액

			// 창고 보유액 관련
			UINT64	ui64AveMoney;			// 창고 평균 금액
			UINT64	ui64TotalPlusMoney;		// 창고에 들어간 돈
			UINT64	ui64TotalMinusMoney;	// 창고에서 나간 돈

			USHORT	usJackpotCnt;			// 잭팟 터진 횟수
			UINT64	ui64JackpotMoney;		// 잭팟 금액 합계

			UINT64	ui64CirculateMoney;		// 상거래 금액 합계
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 서버의 정상종료를 요청한다.
/*
	[2006.04.20] modify by guiun
	dwServerType, dwServerID 추가
*/
class MSG_RTTG_SERVERSHUTDOWN_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_SERVERSHUTDOWN_REQ()
			{
				wSize	= sizeof( MSG_RTTG_SERVERSHUTDOWN_REQ ) - SIZEOFHDR;
				wID		= RTTG_SERVERSHUTDOWN_REQ;
			}
			~MSG_RTTG_SERVERSHUTDOWN_REQ()
			{
			}

			DWORD			dwKey;			//  RTT 내에서 사용하는 전용필드
			BYTE			dwWorldID;		//  0x00 = 전체 World(dwChannelID 무시)
			BYTE			dwChannelID;	//  0x00 = 전체 Channel
			BYTE			dwServerType;	//  
			BYTE			dwServerID;		// 
			UINT64			i64WopsKey;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 마스터 서버에게 서버 정보 Reload 명령
/*
*/
class MSG_RTTG_FUNCRESTRICT_RELOAD_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_FUNCRESTRICT_RELOAD_NTF()
			{
				wSize	= sizeof( MSG_RTTG_FUNCRESTRICT_RELOAD_NTF ) - SIZEOFHDR;
				wID		= RTTG_FUNCRESTRICT_RELOAD_NTF;
			}
			~MSG_RTTG_FUNCRESTRICT_RELOAD_NTF()
			{
			}
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 마스터 서버에게 서버 정보 Reload 명령
/*
*/
class MSG_RTTG_AUTHIPLIST_RELOAD_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_AUTHIPLIST_RELOAD_REQ()
			{
				wSize	= sizeof( MSG_RTTG_AUTHIPLIST_RELOAD_REQ ) - SIZEOFHDR;
				wID		= RTTG_AUTHIPLIST_RELOAD_REQ;
			}
			~MSG_RTTG_AUTHIPLIST_RELOAD_REQ()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTG_AUTHIPLIST_RELOAD_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_AUTHIPLIST_RELOAD_ANS()
			{
				wSize	= sizeof( MSG_RTTG_AUTHIPLIST_RELOAD_ANS ) - SIZEOFHDR;
				wID		= RTTG_AUTHIPLIST_RELOAD_ANS;
			}
			~MSG_RTTG_AUTHIPLIST_RELOAD_ANS()
			{
			}

			DWORD dwWopsKey;
			DWORD dwResult;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 마스터 서버에게 서버 정보 Reload 명령
/*
*/
class MSG_RTTG_ARMORKIT_DISPATCH_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_ARMORKIT_DISPATCH_NTF()
			{
				wSize	= sizeof( MSG_RTTG_ARMORKIT_DISPATCH_NTF ) - SIZEOFHDR;
				wID		= RTTG_ARMORKIT_DISPATCH_NTF;
			}
			~MSG_RTTG_ARMORKIT_DISPATCH_NTF()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTG_ARMORKIT_DISPATCH_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_ARMORKIT_DISPATCH_ANS()
			{
				wSize	= sizeof( MSG_RTTG_ARMORKIT_DISPATCH_ANS ) - SIZEOFHDR;
				wID		= RTTG_ARMORKIT_DISPATCH_ANS;
			}
			~MSG_RTTG_ARMORKIT_DISPATCH_ANS()
			{
			}

			DWORD dwWopsKey;
			DWORD dwResult;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 인증 서버에게 패턴 Reload 명령

class MSG_RTTG_PATTERN_RELOAD_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_PATTERN_RELOAD_REQ()
			{
				wSize	= sizeof( MSG_RTTG_PATTERN_RELOAD_REQ ) - SIZEOFHDR;
				wID		= RTTG_PATTERN_RELOAD_REQ;
			}
			~MSG_RTTG_PATTERN_RELOAD_REQ()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTG_PATTERN_RELOAD_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_PATTERN_RELOAD_ANS()
			{
				wSize	= sizeof( MSG_RTTG_PATTERN_RELOAD_ANS ) - SIZEOFHDR;
				wID		= RTTG_PATTERN_RELOAD_ANS;
			}
			~MSG_RTTG_PATTERN_RELOAD_ANS()
			{
			}

			DWORD dwWopsKey;
			DWORD dwResult;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 인증 서버에게 GameClinet, Laucher version Reload 명령
class MSG_RTTG_GAMEVERSION_UPDATE_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEVERSION_UPDATE_REQ()
			{
				wSize	= sizeof( MSG_RTTG_GAMEVERSION_UPDATE_REQ ) - 2;
				wID		= RTTG_GAMEVERSION_UPDATE_REQ;
			}

			DWORD 		dwWopsKey;
};

class MSG_RTTG_GAMEVERSION_UPDATE_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEVERSION_UPDATE_ANS()
			{
				wSize	= sizeof( MSG_RTTG_GAMEVERSION_UPDATE_ANS ) - 2;
				wID		= RTTG_GAMEVERSION_UPDATE_ANS;
			}

			DWORD		dwResult;
			DWORD 		dwWopsKey;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTTA
//  Description : 서버 정상종료 결과를 통보한다.
class MSG_RTTG_SERVERSHUTDOWN_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_SERVERSHUTDOWN_ANS()
			{
				wSize	= sizeof( MSG_RTTG_SERVERSHUTDOWN_ANS ) - SIZEOFHDR;
				wID		= RTTG_SERVERSHUTDOWN_ANS;
			}
			~MSG_RTTG_SERVERSHUTDOWN_ANS()
			{
			}

			DWORD			dwKey;				//  RTT 내에서 사용하는 전용필드
			DWORD			dwError;			//  0 : 완료, 1 : 에러발생
			SERVER_KEY		m_ServerKey;		//  에러 발생의 경우, Server Key 확인
			__int64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 패치 결과의 통보를 요청한다.
class MSG_RTTG_PATCHRESULT_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_PATCHRESULT_REQ()
			{
				wSize	= sizeof( MSG_RTTG_PATCHRESULT_REQ ) - SIZEOFHDR;
				wID		= RTTG_PATCHRESULT_REQ;
			}
			~MSG_RTTG_PATCHRESULT_REQ()
			{
			}

			DWORD			dwKey;			//  RTT 내에서 사용하는 전용필드
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTTA
//  Description : 패치 결과를 통보한다.
class MSG_RTTG_PATCHRESULT_ANS : public MSG_HEADER4
{
public:
	static	const DWORD		ERROR_PATCHSUCCESS			= 0x00;		//  패치 성공
	static  const DWORD		ERROR_PROCESSRUN			= 0x01;		//  해당 프로세스 실행중		
	static	const DWORD		ERROR_FTPCONNECT_FAIL		= 0x02;		//  ftp 연결 오류
	static	const DWORD		ERROR_DOWNLOAD_FAIL			= 0x03;		//  파일 받기 오류
	static	const DWORD		ERROR_FILENOTFOUND			= 0x04;		//  패치파일 찾기 오류
	static	const DWORD		ERROR_INVALID_SERVERTYPE	= 0x05;		//  잘못된 서버 타입

	//  RTT 전용 코드
	static	const DWORD		ERROR_NONE				= 0xFC;		//  nothing
	static	const DWORD		ERROR_NOTEXISTLAUNCHER	= 0xFD;		//  런쳐를 실행할 수 없음
	static	const DWORD		ERROR_PROGRESS			= 0xFE;		//  패치 진행중
	static	const DWORD		ERROR_COMPLETE			= 0xFF;		//  패치 완료

public:
			MSG_RTTG_PATCHRESULT_ANS()
			{
				wSize	= sizeof( MSG_RTTG_PATCHRESULT_ANS ) - SIZEOFHDR;
				wID		= RTTG_PATCHRESULT_ANS;
			}
			~MSG_RTTG_PATCHRESULT_ANS()
			{
			}

			DWORD			dwKey;			//  RTT 내에서 사용하는 전용필드
			DWORD			dwError;		//  0 : 성공, 1 이상 : 에러값 확인
			BYTE		    byIndex;
			BYTE		    byServerType;
			BYTE			byVer1;			//  프로세스의 버젼
			BYTE			byVer2;
			BYTE			byVer3;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT
//  Description : RTT_A 서비스를 시작시킬 것을 요청한다.
class MSG_RTTW_RTTASTART_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTART_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTART_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTART_REQ;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_RTTASTART_REQ()
			{
			}


			DWORD			dwFlag;						//  0 : 전체 RTTA를 대상으로 적용
														//  1 : szIPAddress에 해당하는 RTTA만 적용
			wchar_t			szIPAddress[IP_MAX_LEN];
};

class MSG_RTTW_RTTASTART_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTART_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTART_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTART_ANS;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_RTTASTART_ANS()
			{
			}


			DWORD			dwResult;					//  0 : 성공,		1 : 실패		2 : RTTA검색 실패
			wchar_t			szIPAddress[IP_MAX_LEN];	//  목적지 주소
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT
//  Description : RTT_A 서비스를 종료시킬 것을 요청한다.
class MSG_RTTW_RTTASTOP_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTOP_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTOP_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTOP_REQ;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_RTTASTOP_REQ()
			{
			}


			DWORD			dwFlag;						//  0 : 전체 RTTA를 대상으로 적용
														//  1 : szIPAddress에 해당하는 RTTA만 적용
			wchar_t			szIPAddress[IP_MAX_LEN];
};


class MSG_RTTW_RTTASTOP_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTOP_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTOP_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTOP_ANS;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_RTTASTOP_ANS()
			{
			}


			DWORD			dwResult;					//  0 : 성공,		1 : 실패
			wchar_t			szIPAddress[IP_MAX_LEN];	//  목적지 주소
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_Service
//  Description : RTT_A의 패치를 명령한다.
class MSG_RTTW_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTAPATCH_NTF;
				memset(&szFtpAddress[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpUser[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpPassword[0]	, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpGetFilePath[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpGetFileName[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_RTTAPATCH_NTF()
			{
			}


			DWORD			dwWorldID;				//  0 : 전체
			DWORD			dwChannelID;			//  0 : 전체

			wchar_t			szFtpAddress[20];
			DWORD			dwFtpPort;
			wchar_t			szFtpUser[20];
			wchar_t			szFtpPassword[20];
			wchar_t			szFtpGetFilePath[40];
			wchar_t			szFtpGetFileName[40];			
};

/*
class MSG_RTTW_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTAPATCH_NTF;
			}
			~MSG_RTTW_RTTAPATCH_NTF()
			{
			}


			DWORD			dwWorldID;				//  0 : 전체
			DWORD			dwChannelID;			//  0 : 전체

			wchar_t			szIp[20];
			wchar_t			szPath[40];
			wchar_t			szAccount[20];
			wchar_t			szPassword[40];
};
*/


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT
//  Description : 시스템의 재부팅을 요청한다.
class MSG_RTTW_SYSTEMREBOOT_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_SYSTEMREBOOT_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_SYSTEMREBOOT_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SYSTEMREBOOT_NTF;
				memset(&szIPAddress[0]	, 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_SYSTEMREBOOT_NTF()
			{
			}

			wchar_t			szIPAddress[IP_MAX_LEN];	//  목적지 주소
			BOOL			bReboot;					// 재부팅여부(FALSE면 꺼버림.) 08-01-28 박동권 by 김종호
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT_S → RTT_A
//  Description : 프로세스 제어 명령을 보낸다
class MSG_RTTW_PROCESSCONTROL_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_PROCESSCONTROL_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_PROCESSCONTROL_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_PROCESSCONTROL_NTF;
				memset(&szIPAddress[0]		, 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szBinaryName[0]		, 0, sizeof(wchar_t) * 100);		// 07-01-03 김종호(버퍼 초기화)
				memset(&szAbsolutePath[0]	, 0, sizeof(wchar_t) * 100);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_PROCESSCONTROL_NTF()
			{
			}


			DWORD		dwCommand;					//  0 : Process Kill, 1 : Process Run
			wchar_t		szIPAddress[IP_MAX_LEN];	//  목적지 IP주소
			wchar_t		szBinaryName[100];			//  바이너리
			wchar_t		szAbsolutePath[100];		//  절대 경로
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → Auth
//  Description : 지정된 계정의 사용자 접속을 강제로 종료시킨다.
class MSG_RTTW_DROPUSER_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_DROPUSER_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_DROPUSER_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_DROPUSER_REQ;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_DROPUSER_REQ()
			{
			}

			wchar_t			szUserID[ID_MAX_LEN + 1];			//  강제종료시킬 사용자의 계정(ID)
};

/**
	2006.11.28
	modify by guiun
	AuthServer로 전송되는 사용자 DROP 패킷에 DROP 이유를 담을 수 있는 DropCode를 추가한다.
 */
enum eDROP_CODE
{
	eDRC_WOPS_GM		= 100,
	eDRC_BILLING_EXPIRE = 200,
	eDRC_WEB			= 300,
	eDRC_MAX
};

class MSG_RTTG_DROPUSER_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_DROPUSER_REQ()
			{
				wSize	= sizeof( MSG_RTTG_DROPUSER_REQ ) - SIZEOFHDR;
				wID		= RTTG_DROPUSER_REQ;
				memset(&szUserID[0]	, 0, ID_MAX_LEN + 1);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_DROPUSER_REQ()
			{
			}

			char			szUserID[ID_MAX_LEN + 1];			//  강제종료시킬 사용자의 계정(ID)
			DWORD			dwDropCode;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Auth → RTT → GMS
//  Description : 사용자 접속 종료처리 후, 결과를 통지한다.
class MSG_RTTW_DROPUSER_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_DROPUSER_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_DROPUSER_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_DROPUSER_ANS()
			{
			}

			DWORD			dwError;							//  0 : 성공		1 : 접속하지 않은 계정
																//  2 : 서버간 통신 오류	3 : 이미 작업이 진행중인 계정
			wchar_t			szUserID[ID_MAX_LEN + 1];			//  요청된 사용자의 계정(ID)
};

class MSG_RTTG_DROPUSER_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_DROPUSER_ANS()
			{
				wSize	= sizeof( MSG_RTTG_DROPUSER_ANS ) - SIZEOFHDR;
				wID		= RTTG_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, ID_MAX_LEN + 1);		// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTG_DROPUSER_ANS()
			{
			}

#ifdef  _090427_HTTP_WEB_DROP_USER
			DWORD			dwDropCode;							//	REQ에 보낸 드롭 코드를 리턴한 값(일단 중국 웹 상에서 강제 종료 기능에만 추가된다)
#endif
			DWORD			dwError;							//  0 : 성공		1 : 접속하지 않은 계정
																//  2 : 서버간 통신 오류
			char			szUserID[ID_MAX_LEN + 1];			//  요청된 사용자의 계정(ID)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : 시스템의 Resource 상황을 보고한다.
class MSG_RTTR_RESOURCE_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RESOURCE_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RESOURCE_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RESOURCE_NTF;
			}
			~MSG_RTTR_RESOURCE_NTF()
			{
			}

			DWORD			dwCPU;						//  CPU %
			DWORD			dwMemoryUse;				//  Memory 사용량
			DWORD			dwMemoryMax;				//  Memory 최대값
			DWORD			dwHDDUse;					//  HDD 사용량
			DWORD			dwHDDMax;					//  HDD 최대값
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTS → RTTA
//  Description : 개설된 배틀존의 정보를 요청한다.
class MSG_RTTW_BATTLEZONEINFO_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_BATTLEZONEINFO_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_BATTLEZONEINFO_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_BATTLEZONEINFO_REQ;
			}
			~MSG_RTTW_BATTLEZONEINFO_REQ()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → RTTS
//  Description : 개설된 배틀존의 정보를 보낸다.
class MSG_RTTW_BATTLEZONEINFO_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_BATTLEZONEINFO_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_BATTLEZONEINFO_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_BATTLEZONEINFO_ANS;
			}
			~MSG_RTTW_BATTLEZONEINFO_ANS()
			{
			}


			BATTLEZONE_INFO			info;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → RTTS
//  Description : ArmorKit에서 발생된 정보를 보낸다.
class MSG_RTTW_ARMORKIT_FINDHACK_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_ARMORKIT_FINDHACK_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_ARMORKIT_FINDHACK_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ARMORKIT_FINDHACK_NTF;
				memset(&szIP[0]	, 0, sizeof(wchar_t) * 16);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szBuf[0], 0, 1024);					// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_ARMORKIT_FINDHACK_NTF()
			{
			}


			DWORD		dwGuid;			// 해킹이 감지된 클라이언트 guid
			wchar_t		szIP[16];		// 해킹이 감지된 클라이언트 IP주소
			DWORD		dwHackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
			DWORD		dwBufSize;		// Buffer 의 길이
			char		szBuf[1024];	// 데이터 버퍼-1024는 보안파트에서 전달받은 암묵적 최대값(성현창)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → RTTS
//  Description : 어뷰징 정보
class MSG_RTTW_ABUSING_NTF : public MSG_HEADER8
{
public:

	enum { MAX_DESC_LENGTH = 1023, MAX_CHARNAME_LENGTH = 16, MAX_IP_LENGTH = 15, MAX_ID_LEN = 12, 
			MAX_SUBTYPE_NAME_LEN = 64, };

	MSG_RTTW_ABUSING_NTF()
	{
		dwSize	= sizeof( MSG_RTTW_ABUSING_NTF ) - sizeof( MSG_HEADER8 );
		dwID	= RTTW_ABUSING_NTF;

#ifndef  _110503_CUSTOM_USERID_LENGTH
		memset(&m_ptszUserID[0], 0, sizeof(char) * (MAX_ID_LEN + 1) );				// 07-06-11 김종호(버퍼 초기화)
#else
		memset(&m_ptszUserID[0], 0, sizeof(char) * (ID_MAX_LEN + 1) );
#endif

		memset(&m_ptszCharName[0], 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH + 1) );	// 07-02-01 김종호(버퍼 초기화)
		memset(&m_ptszIP[0]			, 0, sizeof(TCHAR) * (MAX_IP_LENGTH + 1)   );		// 07-01-03 김종호(버퍼 초기화)
		memset(&m_ptszDesc[0]		, 0, sizeof(TCHAR) * (MAX_DESC_LENGTH + 1) );		// 07-01-03 김종호(버퍼 초기화)
		ZeroMemory(m_ptszSubTypeName, sizeof(m_ptszSubTypeName));						// 08-04-03 박동권(노코멘트 ㄱ-)
	}
	~MSG_RTTW_ABUSING_NTF()
	{
	}

	DWORD64			m_dwTime;										// time - GetDateTime_YYMMDDHHMM()의 값을 사용한다
	SERVER_KEY		m_ServerKey;									// server
	DWORD			m_dwUserGuid;									// user guid
	DWORD			m_dwCharGuid;									// char guid

#ifndef  _110503_CUSTOM_USERID_LENGTH
	char			m_ptszUserID[MAX_ID_LEN + 1];					// user id
#else
	char			m_ptszUserID[ID_MAX_LEN + 1];					// user id
#endif

	TCHAR			m_ptszCharName[MAX_CHARNAME_LENGTH + 1];		// char name
	TCHAR			m_ptszIP[MAX_IP_LENGTH + 1];					// user ip
	
   	BYTE			m_AbuseType;									// type
	BYTE			m_Level;										// level
	TCHAR			m_ptszSubTypeName[MAX_SUBTYPE_NAME_LEN + 1];	// typename(64)

	WORD			m_wLength;										// desc length
	TCHAR			m_ptszDesc[MAX_DESC_LENGTH + 1];				// desc
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTS → WOPS
//  Description : 장애 발생에 대한 통지
class MSG_RTTW_ERRORNOTIFY_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_ERRORNOTIFY_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_ERRORNOTIFY_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ERRORNOTIFY_NTF;
				memset(&szMessage[0]	, 0, sizeof(wchar_t) * 120 );	// 07-01-03 김종호(버퍼 초기화)
				memset(&szDateTime[0]	, 0, sizeof(wchar_t) * 20 );	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTW_ERRORNOTIFY_NTF()
			{
			}


			DWORD			dwWorldID;
			DWORD			dwChannelID;
			DWORD			dwServerType;
			DWORD			dwServerID;

			DWORD			dwWarningDetailID;
			wchar_t			szMessage[120];
			wchar_t			szDateTime[20];
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTTS
//  Description : WOPS의 연결 형식을 정의한다.
class MSG_RTTW_CONNECTIONTYPE_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_CONNECTIONTYPE_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_CONNECTIONTYPE_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_CONNECTIONTYPE_NTF;
			}
			~MSG_RTTW_CONNECTIONTYPE_NTF()
			{
			}

			DWORD			dwType;
			//  0 : 지속적인 연결	- RTTS가 WOPS로 먼저 패킷을 보낼 수 있다.
			//  1 : 단발적인 연결	- WOPS의 요청이 있을 경우만 패킷을 보낼 수 있다.
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → RTTS
//  Description : 통화량 전송을 요청한다.
class MSG_RTTW_USERMONEY_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_USERMONEY_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_USERMONEY_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_USERMONEY_REQ;
			}
			~MSG_RTTW_USERMONEY_REQ()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → RTTS
//  Description : 통화량을 보낸다.
class MSG_RTTW_USERMONEY_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_USERMONEY_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_USERMONEY_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_USERMONEY_ANS;
			}
			~MSG_RTTW_USERMONEY_ANS()
			{
			}
			BYTE	byWorldID;
			BYTE	byChannelID;
			BYTE	byServerType;
			BYTE	byServerID;

			// 개인 보유액 관련
			UINT64	ui64TotalGetMoney;		// 일정 시간 동안 생겨난 돈 총계
			UINT64	ui64TotalLoseMoney;		// 일정 시간 동안 없어진 돈 총계
			UINT64	ui64TotalUserMoney;		// 현재 동접 유저들이 갖고 있는 평균 금액

			// 창고 보유액 관련
			UINT64	ui64AveMoney;			// 창고 평균 금액
			UINT64	ui64TotalPlusMoney;		// 창고에 들어간 돈
			UINT64	ui64TotalMinusMoney;	// 창고에서 나간 돈

			USHORT	usJackpotCnt;			// 잭팟 터진 횟수
			UINT64	ui64JackpotMoney;		// 잭팟 금액 합계

			UINT64	ui64CirculateMoney;		// 상거래 금액 합계
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 서버의 정상종료를 요청한다.

/*
	[2006.04.20] modify by guiun
	dwServerType, dwServerID 추가
*/
class MSG_RTTW_SERVERSHUTDOWN_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_SERVERSHUTDOWN_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_SERVERSHUTDOWN_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SERVERSHUTDOWN_REQ;
			}
			~MSG_RTTW_SERVERSHUTDOWN_REQ()
			{
			}

			DWORD			dwKey;			//  RTT 내에서 사용하는 전용필드
			DWORD			dwWorldID;		//  0x00 = 전체 World(dwChannelID 무시)
			DWORD			dwChannelID;	//  0x00 = 전체 Channel
			DWORD			dwServerType;	//  
			DWORD			dwServerID;		//  
			UINT64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → WOPS
//  Description : 서버 정상종료 처리 결과를 통지한다.
class MSG_RTTW_SERVERSHUTDOWN_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_SERVERSHUTDOWN_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_SERVERSHUTDOWN_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SERVERSHUTDOWN_ANS;
			}
			~MSG_RTTW_SERVERSHUTDOWN_ANS()
			{
			}

			DWORD			dwKey;				//  RTT 내에서 사용하는 전용필드
			DWORD			dwError;			//  0 : 완료, 1 : 에러발생
			SERVER_KEY		m_ServerKey;		//  에러 발생의 경우, Server Key 확인
			__int64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 패치 런쳐의 실행을 명령한다.
class MSG_RTTW_LAUNCHPATCHER_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_LAUNCHPATCHER_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_LAUNCHPATCHER_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_LAUNCHPATCHER_REQ;
			}
			~MSG_RTTW_LAUNCHPATCHER_REQ()
			{
			}

			DWORD			dwWorldID;		//  0x00 = 전체 World(dwChannelID 무시)
			DWORD			dwChannelID;	//  0x00 = 전체 Channel
			DWORD			dwServerType;	//  
			DWORD			dwServerID;		// 
			DWORD			dwServerTypeForProcess;
			__int64			i64WopsKey;		//  WOPS와 DB로 연동할 key값
			DWORD			dwKey;			//  RTT 내에서 사용하는 전용필드
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 기능제한 테이블 다시 읽어서 Game서버들에 Broadcast
class MSG_RTTW_FUNCRESTRICT_RELOAD_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_FUNCRESTRICT_RELOAD_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_FUNCRESTRICT_RELOAD_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_FUNCRESTRICT_RELOAD_NTF;
			}
			~MSG_RTTW_FUNCRESTRICT_RELOAD_NTF()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTW_FUNCRESTRICT_RELOAD_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_FUNCRESTRICT_RELOAD_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_FUNCRESTRICT_RELOAD_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_FUNCRESTRICT_RELOAD_ANS;
			}
			~MSG_RTTW_FUNCRESTRICT_RELOAD_ANS()
			{
			}
			DWORD dwResult;
			DWORD dwWopsKey;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : ARMORKIT DISPATCH
class MSG_RTTW_ARMORKIT_DISPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_ARMORKIT_DISPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_ARMORKIT_DISPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ARMORKIT_DISPATCH_NTF;
			}
			~MSG_RTTW_ARMORKIT_DISPATCH_NTF()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTW_ARMORKIT_DISPATCH_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_ARMORKIT_DISPATCH_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_ARMORKIT_DISPATCH_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ARMORKIT_DISPATCH_ANS;
			}
			~MSG_RTTW_ARMORKIT_DISPATCH_ANS()
			{
			}
			DWORD dwResult;
			DWORD dwWopsKey;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → Master
//  Description : 길드명 변경 요청.
class MSG_RTTW_CHANGE_GUILDNAME_REQ : public MSG_HEADER8
{
public:

	enum { MAX_GUILD_NAME_LENGTH = 16, MAX_DESCRIPTION_LENGTH = 256, };

			MSG_RTTW_CHANGE_GUILDNAME_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_CHANGE_GUILDNAME_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_CHANGE_GUILDNAME_REQ;
				memset( wszGuildName, 0, sizeof(wszGuildName) );
				memset( wszDescription, 0, sizeof(wszDescription) );
			}
			~MSG_RTTW_CHANGE_GUILDNAME_REQ()
			{
			}

			SERVER_KEY	dwServerGuid;								// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
			DWORD		dwGuildGuid;								// 길드 GUID
			wchar_t		wszGuildName[MAX_GUILD_NAME_LENGTH + 1];	// 변경하고자 하는 길드명
			wchar_t		wszDescription[MAX_DESCRIPTION_LENGTH + 1];	// 변경 사유
};

class MSG_RTTG_CHANGE_GUILDNAME_REQ : public MSG_HEADER4
{
public:

	enum { MAX_GUILD_NAME_LENGTH = 16, MAX_DESCRIPTION_LENGTH = 512, };

			MSG_RTTG_CHANGE_GUILDNAME_REQ()
			{
				wSize	= sizeof( MSG_RTTG_CHANGE_GUILDNAME_REQ ) - SIZEOFHDR;
				wID	= RTTG_CHANGE_GUILDNAME_REQ;
				memset( szGuildName, 0, sizeof(szGuildName) );
				memset( szDescription, 0, sizeof(szDescription) );
			}
			~MSG_RTTG_CHANGE_GUILDNAME_REQ()
			{
			}

			DWORD		dwWopsKey;									// 명령 식별자
            SERVER_KEY	dwServerGuid;								// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
			DWORD		dwGuildGuid;								// 길드 GUID
			char		szGuildName[MAX_GUILD_NAME_LENGTH + 1];		// 변경하고자 하는 길드명
			char		szDescription[MAX_DESCRIPTION_LENGTH + 1];	// 변경 사유
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master → RTT → GMS
//  Description : 길드명 변경 요청 결과
class MSG_RTTW_CHANGE_GUILDNAME_ANS : public MSG_HEADER8
{
public:

	enum { MAX_RESULT_DESCRIPTION_LENGTH = 200, };

			MSG_RTTW_CHANGE_GUILDNAME_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_CHANGE_GUILDNAME_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_CHANGE_GUILDNAME_ANS;
				memset( wszResultDescription, 0, sizeof(wszResultDescription) );
			}
			~MSG_RTTW_CHANGE_GUILDNAME_ANS()
			{
			}

			DWORD		dwResult;													// 0 : 변경 성공, 1 : 변경 실패, 2 : 서버간 통신오류(RTT까지의 통신오류)
			wchar_t		wszResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// 처리 결과에 대한 Description
};

class MSG_RTTG_CHANGE_GUILDNAME_ANS : public MSG_HEADER4
{
public:

	enum { MAX_RESULT_DESCRIPTION_LENGTH = 200, };

			MSG_RTTG_CHANGE_GUILDNAME_ANS()
			{
				wSize	= sizeof( MSG_RTTG_CHANGE_GUILDNAME_ANS ) - SIZEOFHDR;
				wID	= RTTG_CHANGE_GUILDNAME_ANS;
				memset( szResultDescription, 0, sizeof(szResultDescription) );
			}
			~MSG_RTTG_CHANGE_GUILDNAME_ANS()
			{
			}

			DWORD		dwWopsKey;												// 명령 식별자
			DWORD		dwResult;												// 0 : 변경 성공, 1 : 변경 실패
			char		szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// 처리 결과에 대한 Description
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → Master
//  Description : 유저에게 쪽지를 전송 요청.
class MSG_RTTW_SEND_MESSAGE_REQ : public MSG_HEADER8
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_MESSAGE_LENGTH = 100, MAX_USER_INFO = 10, };

	typedef struct tag_USER_INFO
	{
		wchar_t	wszRecvCharName[MAX_CHAR_NAME_LENGTH + 1];	// 쪽지를 받을 캐릭터명
	} USER_INFO, *LPUSER_INFO;

public:

			MSG_RTTW_SEND_MESSAGE_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_SEND_MESSAGE_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SEND_MESSAGE_REQ;
				memset( arrUserInfoList, 0, sizeof(arrUserInfoList) );
				memset( wszSendCharName, 0, sizeof(wszSendCharName) );
				memset( wszMessage, 0, sizeof(wszMessage) );
			}
			~MSG_RTTW_SEND_MESSAGE_REQ()
			{
			}

			SERVER_KEY		dwServerGuid;								// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
			DWORD			dwCount;									// 쪽지 정보 갯수
            USER_INFO		arrUserInfoList[MAX_USER_INFO];				// 쪽지 정보들
			wchar_t			wszSendCharName[MAX_CHAR_NAME_LENGTH + 1];	// 쪽지를 보낸 캐릭터명
			wchar_t			wszMessage[MAX_MESSAGE_LENGTH + 1];			// 쪽지 내용
};

class MSG_RTTG_SEND_MESSAGE_REQ : public MSG_HEADER4
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_MESSAGE_LENGTH = 200, MAX_USER_INFO = 10, };

	typedef struct tag_USER_INFO
	{
		char	szRecvCharName[MAX_CHAR_NAME_LENGTH + 1];	// 쪽지를 받을 캐릭터명
	} USER_INFO, *LPUSER_INFO;

public:

			MSG_RTTG_SEND_MESSAGE_REQ()
			{
				wSize	= sizeof( MSG_RTTG_SEND_MESSAGE_REQ ) - SIZEOFHDR;
				wID	= RTTG_SEND_MESSAGE_REQ;
				memset( arrUserInfoList, 0, sizeof(arrUserInfoList) );
				memset( szSendCharName, 0, sizeof(szSendCharName) );
				memset( szMessage, 0, sizeof(szMessage) );
			}
			~MSG_RTTG_SEND_MESSAGE_REQ()
			{
			}

			DWORD			dwWopsKey;									// 명령 식별자
            SERVER_KEY		dwServerGuid;								// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
			DWORD			dwCount;									// 쪽지 정보 갯수
            USER_INFO		arrUserInfoList[MAX_USER_INFO];				// 쪽지 정보들
			char			szSendCharName[MAX_CHAR_NAME_LENGTH + 1];	// 쪽지를 보낸 캐릭터명
			char			szMessage[MAX_MESSAGE_LENGTH + 1];			// 쪽지 내용
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master → RTT → GMS
//  Description : 유저에게 쪽지 전송 요청 결과
class MSG_RTTW_SEND_MESSAGE_ANS : public MSG_HEADER8
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_RESULT_INFO = 10, MAX_RESULT_DESCRIPTION_LENGTH = 200, };

	typedef struct tag_RESULT_INFO
	{
		DWORD	dwResult;													// 0 : 전송 성공, 1 : 전송 실패, 2 : 서버간 통신오류(RTT까지의 통신오류)
		wchar_t	wszRecvCharName[MAX_CHAR_NAME_LENGTH + 1];					// 쪽지를 받은 캐릭터명
		wchar_t	wszResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// 쪽지 전송 결과 Description
	} RESULT_INFO, *LPRESULT_INFO;

public:
			MSG_RTTW_SEND_MESSAGE_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_SEND_MESSAGE_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SEND_MESSAGE_ANS;
				memset( arrResultInfoList, 0, sizeof(arrResultInfoList) );
			}
			~MSG_RTTW_SEND_MESSAGE_ANS()
			{
			}

			DWORD			dwCount;							// 쪽지 정보 갯수
			RESULT_INFO		arrResultInfoList[MAX_RESULT_INFO];	// 쪽지 정보들
};

class MSG_RTTG_SEND_MESSAGE_ANS : public MSG_HEADER4
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_RESULT_INFO = 10, MAX_RESULT_DESCRIPTION_LENGTH = 200, };

	typedef struct tag_RESULT_INFO
	{
		DWORD	dwResult;												// 0 : 전송 성공, 1 : 전송 실패
		char	szRecvCharName[MAX_CHAR_NAME_LENGTH + 1];				// 메시지를 받은 캐릭터명
		char	szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// 쪽지 전송 결과 Description
	} RESULT_INFO, *LPRESULT_INFO;

public:

			MSG_RTTG_SEND_MESSAGE_ANS()
			{
				wSize	= sizeof( MSG_RTTG_SEND_MESSAGE_ANS ) - SIZEOFHDR;
				wID	= RTTG_SEND_MESSAGE_ANS;
				memset( arrResultInfoList, 0, sizeof(arrResultInfoList) );
			}
			~MSG_RTTG_SEND_MESSAGE_ANS()
			{
			}

			DWORD			dwWopsKey;							// 명령 식별자
			DWORD			dwCount;							// 쪽지 전송 결과 갯수
			RESULT_INFO		arrResultInfoList[MAX_RESULT_INFO];	// 쪽지 전송 결과 정보들
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 인증 서버에게 접속 필터 IP 테이블 Reload 명령
class MSG_RTTW_AUTHIPLIST_RELOAD_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_AUTHIPLIST_RELOAD_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_AUTHIPLIST_RELOAD_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_AUTHIPLIST_RELOAD_REQ;
			}
			~MSG_RTTW_AUTHIPLIST_RELOAD_REQ()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTW_AUTHIPLIST_RELOAD_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_AUTHIPLIST_RELOAD_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_AUTHIPLIST_RELOAD_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_AUTHIPLIST_RELOAD_ANS;
			}
			~MSG_RTTW_AUTHIPLIST_RELOAD_ANS()
			{
			}

			DWORD dwWopsKey;
			DWORD dwResult;// 1 성공, 0 실패
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 인증 서버에게 GameClinet, Laucher version Reload 명령
class MSG_RTTW_GAMEVERSION_UPDATE_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEVERSION_UPDATE_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEVERSION_UPDATE_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEVERSION_UPDATE_REQ;
			}
			~MSG_RTTW_GAMEVERSION_UPDATE_REQ()
			{
			}
			
			DWORD	 		dwWopsKey;
};

class MSG_RTTW_GAMEVERSION_UPDATE_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEVERSION_UPDATE_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEVERSION_UPDATE_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEVERSION_UPDATE_ANS;
			}
			~MSG_RTTW_GAMEVERSION_UPDATE_ANS()
			{
			}
			
			DWORD		dwResult;
			DWORD 		dwWopsKey;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTS → WOPS
//  Description : 패치 결과를 통지한다.
class MSG_RTTW_PATCHRESULT_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_PATCHRESULT_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_PATCHRESULT_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_PATCHRESULT_ANS;
			}
			~MSG_RTTW_PATCHRESULT_ANS()
			{
			}

			DWORD			dwKey;				//  RTT 내에서 사용하는 전용필드
			DWORD			dwError;			//  0 : 성공, 1 이상 : MSG_RTTG_PATCHRESULT_ANS참조
			BYTE			byIndex;			//  여러 프로세스를 실행 시킬 경우 몇번째 실행된 프로세스인지 구분(MAX->0)
			BYTE			byServerType;
			BYTE			byVer1;				//  프로세스의 버젼
			BYTE			byVer2;
			BYTE			byVer3;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 지정된 머신의 감시 대상 프로세스를 모두 시작시킨다.
class MSG_RTTW_LAUNCHPROCESS_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_LAUNCHPROCESS_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_LAUNCHPROCESS_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_LAUNCHPROCESS_REQ;
			}
			~MSG_RTTW_LAUNCHPROCESS_REQ()
			{
			}


			DWORD			dwWorldID;			//  1 이상의 값만 허용한다.
			DWORD			dwChannelID;		//  0 : 전체 채널, 1 이상 : 지정된 채널
			DWORD			dwServerType;
			DWORD			dwServerID;
			DWORD			dwServerTypeForProcess;
			__int64			i64WopsKey;

			//  이하 필드는 RTT 내에서만 전용으로 사용한다.
			DWORD			dwWopsUID;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT → WOPS
//  Description : 지정된 머신의 감시 대상 프로세스를 모두 시작시킨다.
class MSG_RTTW_LAUNCHPROCESS_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_LAUNCHPROCESS_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_LAUNCHPROCESS_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_LAUNCHPROCESS_ANS;
			}
			~MSG_RTTW_LAUNCHPROCESS_ANS()
			{
			}


			DWORD			dwError;			//  0 : 성공		1 : 프로세스 실행 에러(RTTA 로그 확인)

			//  이하 필드는 RTT 내에서만 전용으로 사용한다.
			DWORD			dwServerKey;
			DWORD			dwWopsUID;
			__int64			i64WopsKey;			
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_Service
//  Description : RTT_A 서비스를 시작시킬 것을 요청한다.
class MSG_RTTR_RTTASTART_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RTTASTART_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RTTASTART_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RTTASTART_NTF;
			}
			~MSG_RTTR_RTTASTART_NTF()
			{
			}
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_Service
//  Description : RTT_A 서비스를 종료시킬 것을 요청한다.
class MSG_RTTR_RTTASTOP_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RTTASTOP_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RTTASTOP_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RTTASTOP_NTF;
			}
			~MSG_RTTR_RTTASTOP_NTF()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_Service
//  Description : RTT_A의 패치를 명령한다.
class MSG_RTTR_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RTTAPATCH_NTF;
				memset(&szFtpAddress[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpUser[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpPassword[0]	, 0, sizeof(wchar_t) * 20);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpGetFilePath[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 김종호(버퍼 초기화)
				memset(&szFtpGetFileName[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTR_RTTAPATCH_NTF()
			{
			}

			DWORD			dwWorldID;				//  0 : 전체
			DWORD			dwChannelID;			//  0 : 전체

			wchar_t			szFtpAddress[20];
			DWORD			dwFtpPort;
			wchar_t			szFtpUser[20];
			wchar_t			szFtpPassword[20];
			wchar_t			szFtpGetFilePath[40];
			wchar_t			szFtpGetFileName[40];			
};

/*
class MSG_RTTR_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RTTAPATCH_NTF;
			}
			~MSG_RTTR_RTTAPATCH_NTF()
			{
			}


			wchar_t			szIp[20];
			wchar_t			szPath[40];
			wchar_t			szAccount[20];
			wchar_t			szPassword[40];
};
*/
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : 감시 프로세스 목록을 요청한다.
class MSG_RTTR_PROCESSLIST_REQ : public MSG_HEADER8
{
public:
			MSG_RTTR_PROCESSLIST_REQ()
			{
				dwSize	= sizeof( MSG_RTTR_PROCESSLIST_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PROCESSLIST_REQ;
			}
			~MSG_RTTR_PROCESSLIST_REQ()
			{
			}
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_A
//  Description : 감시 프로세스 목록을 전송한다.
class MSG_RTTR_PROCESSLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTR_PROCESSLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTR_PROCESSLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PROCESSLIST_ANS;

				for( int i = 0 ; i < 10 ; ++i )
				{
					memset(&szBinaryName[i][0]		, 0, sizeof(wchar_t) * 100);	// 07-01-03 김종호(버퍼 초기화)
					memset(&szAbsolutePath[i][0]	, 0, sizeof(wchar_t) * 100);	// 07-01-03 김종호(버퍼 초기화)
				}
			}
			~MSG_RTTR_PROCESSLIST_ANS()
			{
			}


			DWORD		dwFlag;						//  0 : 초기화 후 수신.   1 : 수신 후 추가
			int			dwCount;					//  목록 개수
			wchar_t		szBinaryName[10][100];		//  바이너리
			wchar_t		szAbsolutePath[10][100];	//  절대 경로
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : 프로세스의 상태를 전송한다.
class MSG_RTTR_PROCESSMONITOR_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_PROCESSMONITOR_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_PROCESSMONITOR_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PROCESSMONITOR_NTF;

				for( int i = 0 ; i < 20 ; ++i )
				{
					memset(&szBinaryName[i][0]		, 0, sizeof(wchar_t) * 100);	// 07-01-03 김종호(버퍼 초기화)
					memset(&szAbsolutePath[i][0]	, 0, sizeof(wchar_t) * 100);	// 07-01-03 김종호(버퍼 초기화)
				}
				memset(&dwStatus[0], 0, sizeof(DWORD) * 20);
			}
			~MSG_RTTR_PROCESSMONITOR_NTF()
			{
			}


			int			dwCount;					//  목록 개수
			wchar_t		szBinaryName[20][100];		//  바이너리
			wchar_t		szAbsolutePath[20][100];	//  절대 경로
			DWORD		dwStatus[20];				//  0 : Inactive, 1 : Active
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_A
//  Description : 패치 런쳐의 정보를 전송한다.
class MSG_RTTR_PATCHLAUNCHERINFO_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_PATCHLAUNCHERINFO_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_PATCHLAUNCHERINFO_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PATCHLAUNCHERINFO_NTF;
                memset(&szAbsolutePath[0]	, 0, sizeof(wchar_t) * 100);// 07-01-03 김종호(버퍼 초기화)
				memset(&szBinaryName[0]		, 0, sizeof(wchar_t) * 60);	// 07-01-03 김종호(버퍼 초기화)
			}
			~MSG_RTTR_PATCHLAUNCHERINFO_NTF()
			{
			}

			wchar_t		szAbsolutePath[100];	//  절대경로
			wchar_t		szBinaryName[60];		//  실행파일명
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : 감시 URL을 요청한다.(06-12-5, 김종호)
class MSG_RTTR_WEB_URL_REQ : public MSG_HEADER8
{
public:
			MSG_RTTR_WEB_URL_REQ()
			{
				dwSize	= sizeof( MSG_RTTR_WEB_URL_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_WEB_URL_REQ;
			}
			~MSG_RTTR_WEB_URL_REQ()
			{
			}
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S → RTT_A
//  Description : 감시 URL을 전송한다.(06-12-5, 김종호)
class MSG_RTTR_WEB_URL_ANS : public MSG_HEADER8
{
public:
			MSG_RTTR_WEB_URL_ANS() : bExist(false), dwURLIndex(0)
			{
				dwSize	= sizeof( MSG_RTTR_WEB_URL_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_WEB_URL_ANS;
				memset(&szURL[0], 0, sizeof(wchar_t) * MAX_URL);
			}
			~MSG_RTTR_WEB_URL_ANS()
			{
			}

			bool		bExist;				// 감시할 URL이 있는지에 대한 플래그
			DWORD		dwURLIndex;			// URL 고유 인덱스
			wchar_t		szURL[MAX_URL];		// URL
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A → RTT_S
//  Description : Request 결과를 전송한다.(06-12-5, 김종호)
class MSG_RTTR_WEB_REQUEST_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_WEB_REQUEST_NTF() : dwURLIndex(0), dwStatusCode( HTTP_STATUS_OK )		// 성공으로 초기화 한다.
			{
				dwSize	= sizeof( MSG_RTTR_WEB_REQUEST_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_WEB_REQUEST_NTF;
			}
			~MSG_RTTR_WEB_REQUEST_NTF()
			{
			}

			DWORD		dwURLIndex;			// 감시 URL 고유 인덱스
			DWORD		dwStatusCode;		// Request 상태코드
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTServer → RTTAgent
//  Description : 게임 통게 요청 
class MSG_RTTW_GAMESTATISTICS_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMESTATISTICS_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GAMESTATISTICS_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMESTATISTICS_REQ;
			}
			~MSG_RTTW_GAMESTATISTICS_REQ()
			{
			}
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTAgent → RTTServer 
//  Description : 게임 통게 응답
class MSG_RTTW_GAMESTATISTICS_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMESTATISTICS_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GAMESTATISTICS_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMESTATISTICS_ANS;
			}
			~MSG_RTTW_GAMESTATISTICS_ANS()
			{
			}
			
			DWORD		dwWorldID;
			DWORD		dwChannelID;
			DWORD		dwServerType;
			DWORD		dwServerID;
			UINT64		ui64Statistics[eSTATISTIC_MAX + 1];	// 전체 통계(순서는 eSTATISTICS_TYPE 참조). USERMONEY_ANS에서 보내는 내용은 채우지 않는다
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTAgent → RTTServer 
//  Description : Heart Beat
class MSG_RTTR_HEARTBEAT_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_HEARTBEAT_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_HEARTBEAT_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_HEARTBEAT_NTF;
			}
			~MSG_RTTR_HEARTBEAT_NTF()
			{
			}
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTAgent → Master
//  Description : 게임 통게 요청 
class MSG_RTTG_GAMESTATISTICS_REQ : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMESTATISTICS_REQ()
			{
				wSize	= sizeof( MSG_RTTG_GAMESTATISTICS_REQ ) - SIZEOFHDR;
				wID	= RTTG_GAMESTATISTICS_REQ;
			}
			~MSG_RTTG_GAMESTATISTICS_REQ()
			{
			}
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master → RTTAgent
//  Description : 게임 통게 응답
class MSG_RTTG_GAMESTATISTICS_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMESTATISTICS_ANS()
			{
				wSize	= sizeof( MSG_RTTG_GAMESTATISTICS_ANS ) - SIZEOFHDR;
				wID	= RTTG_GAMESTATISTICS_ANS;
			}
			~MSG_RTTG_GAMESTATISTICS_ANS()
			{
			}
			
			BYTE		byWorldID;
			BYTE		byChannelID;
			BYTE		byServerType;
			BYTE		byServerID;
			UINT64		ui64Statistics[eSTATISTIC_MAX + 1];	// 전체 통계(순서는 eSTATISTICS_TYPE 참조). USERMONEY_ANS에서 보내는 내용은 채우지 않는다
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS → RTT
//  Description : 인증 서버에게 패턴 Reload 명령
class MSG_RTTW_PATTERN_RELOAD_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_PATTERN_RELOAD_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_PATTERN_RELOAD_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_PATTERN_RELOAD_REQ;
			}
			~MSG_RTTW_PATTERN_RELOAD_REQ()
			{
			}

			DWORD dwWopsKey;
};

class MSG_RTTW_PATTERN_RELOAD_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_PATTERN_RELOAD_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_PATTERN_RELOAD_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_PATTERN_RELOAD_ANS;
			}
			~MSG_RTTW_PATTERN_RELOAD_ANS()
			{
			}

			DWORD dwWopsKey;
			DWORD dwResult;// 1 성공, 0 실패
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WEB → RTT → Auth
//  Description : 중국 웹 요청에 의해 지정된 계정의 사용자 접속을 강제로 종료시킨다
class MSG_RTTH_DROPUSER_REQ : public MSG_HEADER8
{
public:
			MSG_RTTH_DROPUSER_REQ()
			{
				dwSize	= sizeof( MSG_RTTH_DROPUSER_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTH_DROPUSER_REQ;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// 버퍼 초기화
			}
			~MSG_RTTH_DROPUSER_REQ()
			{
			}

			wchar_t			szUserID[ID_MAX_LEN + 1];			//  강제종료시킬 사용자의 계정(ID)
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Auth → RTT → WEB
//  Description : 중국 웹 요청에 의해 지정된 사용자의 접속을 종료처리 후, 결과를 통지한다.
class MSG_RTTH_DROPUSER_ANS : public MSG_HEADER8
{
public:
			MSG_RTTH_DROPUSER_ANS()
			{
				dwSize	= sizeof( MSG_RTTH_DROPUSER_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTH_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// (버퍼 초기화)
			}
			~MSG_RTTH_DROPUSER_ANS()
			{
			}

			DWORD			dwError;							//  0 : 성공		1 : 접속하지 않은 계정
																//  2 : 서버간 통신 오류	3 : 이미 작업이 진행중인 계정
			wchar_t			szUserID[ID_MAX_LEN + 1];			//  요청된 사용자의 계정(ID)
};




};


using namespace Protocol_RTT;
#pragma pack()