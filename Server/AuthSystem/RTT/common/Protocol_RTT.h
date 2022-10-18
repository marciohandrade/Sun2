#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ��� ������

#ifdef _CHINA
#define _110503_CUSTOM_USERID_LENGTH // ������ �����ID(�����̸�) ���� ���� ���
//	1. �߱����� ���� �������̸� ���� 12���� 50���� �÷��޶�� ��û�� ���� �߰�
// [TO-BE-DELETED] #define  _090223_USER_ID_LENGTH_50_CHINA_ONLY_	// �߱� ���� ���� ���� 50��
//	2. ��!! �߱����� �� �󿡼� ���� ���� ���� ����� RTT�� �߰� ���� �޶�� ��û�� ���� �߰�
#define  _090427_HTTP_WEB_DROP_USER				// �߱� �� �󿡼� ���� ���� ���� ��� ����
#endif

#ifdef _KOREA
//	�� ä�θ� ���񽺸� ���� ���� �Ѱ��� �� �Ǹ� ���� ������ ���� ������ ������ �����κ��� �����Ѵ�(���� ������ ����)
#define  _090901_CHANNELLING_CONNECT_USER_
#endif

#ifdef _RUSSIA
#define _110503_CUSTOM_USERID_LENGTH // ������ �����ID(�����̸�) ���� ���� ���
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

#define  SIZEOFHDR_SUN					2				//  Sun������ size�� 2byte �����Ѵ�.

#define  SIZEOFHDR						SIZEOFHDR_SUN

// �� ���� URL ��������( 06-12-05, ����ȣ )
#define DEFAULT_AGENT_NAME "RTTAgent"		// ������ ���� ���α׷��� �̸�
#define MAX_URL				1024			// URL �ִ����
#define MAX_FILENAME		256				// �����̸� �ִ����

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

#define	 RTTW_GAMESTATISTICS_REQ		( CATEGORY_WOPS | 0x36 << 8 )		// ���� ��� ��û
#define  RTTW_GAMESTATISTICS_ANS		( CATEGORY_WOPS | 0x37 << 8 )		// ���� ��� ����

// 2006. 8. 14 ����ȣ ���� �߰�
#define	 RTTW_PATTERN_RELOAD_REQ		( CATEGORY_WOPS | 0x38 << 8 )		// ���� ���� ���� ��û
#define  RTTW_PATTERN_RELOAD_ANS		( CATEGORY_WOPS | 0x39 << 8 )		// ���� ���� ���� ����

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

#define	 RTTG_GAMESTATISTICS_REQ		( CATEGORY_GENERAL | 0x1B << 8 )		// ���� ��� ��û
#define  RTTG_GAMESTATISTICS_ANS		( CATEGORY_GENERAL | 0x1C << 8 )		// ���� ��� ����

#define	 RTTG_AUTHIPLIST_RELOAD_REQ		( CATEGORY_GENERAL | 0x20 << 8 )
#define	 RTTG_AUTHIPLIST_RELOAD_ANS		( CATEGORY_GENERAL | 0x21 << 8 )

#define	 RTTG_GAMEVERSION_UPDATE_REQ	( CATEGORY_GENERAL | 0x22 << 8 )
#define	 RTTG_GAMEVERSION_UPDATE_ANS	( CATEGORY_GENERAL | 0x23 << 8 )

// 2006. 8. 14 ����ȣ ���� �߰�
#define	 RTTG_PATTERN_RELOAD_REQ		( CATEGORY_GENERAL | 0x24 << 8 )		// ���� ���� ���� ��û
#define  RTTG_PATTERN_RELOAD_ANS		( CATEGORY_GENERAL | 0x25 << 8 )		// ���� ���� ���� ����

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
#define  RTTR_WEB_URL_REQ				( CATEGORY_RTT | 0x0A << 8 )	// ���� URL ��û
#define  RTTR_WEB_URL_ANS				( CATEGORY_RTT | 0x0B << 8 )	// ���� URL ����
#define  RTTR_WEB_REQUEST_NTF			( CATEGORY_RTT | 0x0C << 8 )	// ���� URL Request ���
#define  RTTR_HEARTBEAT_NTF				( CATEGORY_RTT | 0xff << 8 )


//------------------------------------------------------------------------
//  Message list - HTTP WEB
#define  RTTH_DROPUSER_REQ				( CATEGORY_HTTP | 0x01 << 8 )		// �߱� �� �󿡼� ���� ���� ���� ��û
#define  RTTH_DROPUSER_ANS				( CATEGORY_HTTP | 0x02 << 8 )		// �߱� �� �󿡼� ���� ���� ���� ��û ���


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
	eZONETYPE_LOBBY = 0,	// �κ�(��Ʋ��)
	eZONETYPE_VILLAGE,		// ����
	eZONETYPE_CHARSELECT,	// ĳ���� ����
	eZONETYPE_MISSION,		// �̼�
	eZONETYPE_HUNTING,		// ����
	eZONETYPE_QUEST,		// ����Ʈ
	eZONETYPE_PVP,			// pvp
	eZONETYPE_EVENT,		// �̺�Ʈ
	eZONETYPE_GUILD,		// ���
	eZONETYPE_SIEGE,		// ������
	eZONETYPE_FIELD,		// �ʵ�(���� ������ ���� ǥ��)
	eZONETYPE_CHUNTING,		// ���� ����
	eZONETYPE_MAX,
};

enum eSTATISTICS_TYPE
{
	// ���� ����Ǵ� ��ȭ��(��ŷ�)
	eSTATISTIC_MONEY_CIRCULATION,
	// ���䵷 : ���ݱ�,����ޱ�,
	eSTATISTIC_MONEY_PLUS,
	// ������� : ������, ���ӼҺ�
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
	// ����ȭ
	eSTATISTIC_CRYSTALIZE_COUNT,

  	// ���հ���
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
	DWORD			dwZoneCount[eZONETYPE_MAX];		// �� ���� ��
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
			WORD		wSize;					//  HEADER ũ�⸦ ������ Payload ������
			union
			{
				WORD			wID;			//  Message ID
				HDRMessageID4	hdrID;
			};
};

class MSG_HEADER8
{
public:
			DWORD		dwSize;					//  HEADER ũ�⸦ ������ Payload ������
			union
			{
				DWORD			dwID;			//  Message ID
				HDRMessageID8	hdrID;
			};
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? �� RTT
//  Description : RTT�� ����ϴ� ���α׷��� RTT�� ������ �̷���� �Ŀ� ������ �޾ƾ��Ѵ�.
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


			DWORD			dwType;						//  AGENTTYPE_ define�� ����Ѵ�.
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


			DWORD			dwType;						//  AGENTTYPE_ define�� ����Ѵ�.
			DWORD			dwVersion;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? �� RTT
//  Description : RTT�� ����ϴ� ���α׷��� RTT�� ������ �̷���� �Ŀ� ������ �޾ƾ��Ѵ�.
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

			DWORD			dwResult;					//  0 : ���� ����,	1 : ���� Ʋ��,	2 : ��ϵ��� ���� Agent
			DWORD			dwType;						//  SERVERTYPE_ define�� ����Ѵ�.
			DWORD			dwUID;						//  ������ ������ȣ
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

			DWORD			dwResult;					//  0 : ���� ����
			DWORD			dwType;						//  SERVERTYPE_ define�� ����Ѵ�.
			DWORD			dwUID;						//  ������ ������ȣ
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A �� RTT_S
//  Description : RTTA�� ����� General Server�� ������ ������ ��, �ش� ������ type�� �����Ѵ�.
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

			DWORD			dwType;						//  SERVERTYPE_ define�� ����Ѵ�.
			DWORD			dwUID;						//  ������ uid���� �ִ´�.
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT
//  Description : ���� ������ ä�� ����� ��û�Ѵ�.
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
//  Redirection : RTT �� GMS
//  Description : ä�� ����� �����Ѵ�.
class MSG_RTTW_SERVERLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_SERVERLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_SERVERLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SERVERLIST_ANS;
				memset(&szList[0], 0, sizeof(wchar_t) * 5120);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_SERVERLIST_ANS()
			{
			}

			DWORD			dwLength;					//  szList�� ���� ���ڿ� ����
			wchar_t			szList[5120];				//  ä�θ�1,GUID1;ä�θ�2,GUID2;...
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� General
//  Description : ���ӳ� ������ �߼��Ѵ�.
#define NOTICE_LENGTH	1025

class MSG_RTTW_GAMENOTICE_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMENOTICE_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GAMENOTICE_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMENOTICE_REQ;
				memset(&szNotice[0], 0, sizeof(wchar_t) * (NOTICE_LENGTH-1) ); // 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GAMENOTICE_REQ()
			{
			}

			DWORD				dwServerUID;				//  ������ ������ ������ uid.(0=��ü)
			DWORD				dwTickPerLine;				//  1���� ��� �ð�����(ms)
			DWORD				dwNoticeLength;				//  ���� ���ڿ��� ����
			wchar_t				szNotice[NOTICE_LENGTH-1];	//  ���� ���ڿ�
			BYTE				btRed;						//  �������ڻ���
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
				memset(&szNotice[0], 0, NOTICE_LENGTH);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_GAMENOTICE_REQ()
			{
			}

			DWORD				dwServerUID;				//  ������ ������ ������ uid.(0=��ü)
			DWORD				dwTickPerLine;				//  1���� ��� �ð�����(ms)
			DWORD				dwNoticeLength;				//  ���� ���ڿ��� ����
			char				szNotice[NOTICE_LENGTH];	//  ���� ���ڿ�
			BYTE				btRed;						//  �������ڻ���
			BYTE				btGreen;					//
			BYTE				btBlue;						//
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT �� GMS
//  Description : ������ ���Ӽ����� ������ �Ϸ�Ǿ������� �����Ѵ�.
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
			//  0 : ����
			//  1 : RTT Agent���� ��� ����
			//  2 : General���� ��� ����
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS, General �� RTT
//  Description : ���ӳ� ������ ���� ����� ��û�Ѵ�.
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
//  Redirection : RTT �� GMS, General
//  Description : ������ ���� ����� �����Ѵ�.
class MSG_RTTW_GAMEADMINLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINLIST_ANS;
				memset(&szList[0], 0, sizeof(wchar_t) * 1024); // 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GAMEADMINLIST_ANS()
			{
			}

			DWORD				dwLength;				//  szList�� ����� ���ڿ��� ����
			wchar_t				szList[1024];			//  ���� ����
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

			DWORD				dwCount;				//  ���� ������ ����
			//  ���� ���� �����͵��� dwCount��ŭ �ݺ�
			//	char				szAccount[ID_MAX_LEN];
			//  char				szIP[IP_MAX_LEN];
			//  DWORD				dwAttr;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� General
//  Description : ������ ������ �߰��Ѵ�.
class MSG_RTTW_GAMEADMINADD_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINADD_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINADD_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINADD_NTF;
				memset(&szAccount[0], 0, sizeof(wchar_t) * ID_MAX_LEN + 1);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
                memset(&szIP[0], 0, sizeof(wchar_t) * IP_MAX_LEN);			// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GAMEADMINADD_NTF()
			{
			}

			wchar_t			szAccount[ID_MAX_LEN + 1];	//  �߰��� ����
			wchar_t			szIP[IP_MAX_LEN];			//  ��� IP
			DWORD			dwAttr;						//  �Ӽ�
};

class MSG_RTTG_GAMEADMINADD_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINADD_NTF()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINADD_NTF ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINADD_NTF;
				memset(&szAccount[0], 0, ID_MAX_LEN	+ 1);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
                memset(&szIP[0], 0, IP_MAX_LEN);			// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_GAMEADMINADD_NTF()
			{
			}

			char			szAccount[ID_MAX_LEN + 1];	//  �߰��� ����
			char			szIP[IP_MAX_LEN];			//  ��� IP
			DWORD			dwAttr;						//  �Ӽ�
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT �� GMS �� General
//  Description : ������ ������ �����Ѵ�.
class MSG_RTTW_GAMEADMINREMOVE_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_GAMEADMINREMOVE_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_GAMEADMINREMOVE_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GAMEADMINREMOVE_NTF;
				memset(&szAccount[0], 0, sizeof(wchar_t) * ID_MAX_LEN + 1);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GAMEADMINREMOVE_NTF()
			{
			}

			wchar_t			szAccount[ID_MAX_LEN + 1];		//  ������ ����
};

class MSG_RTTG_GAMEADMINREMOVE_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_GAMEADMINREMOVE_NTF()
			{
				wSize	= sizeof( MSG_RTTG_GAMEADMINREMOVE_NTF ) - SIZEOFHDR;
				wID		= RTTG_GAMEADMINREMOVE_NTF;
				memset(&szAccount[0], 0, ID_MAX_LEN + 1);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_GAMEADMINREMOVE_NTF()
			{
			}

			char			szAccount[ID_MAX_LEN + 1];		//  ������ ����
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� General
//  Description : ������ �޽����� ������ �������� �����Ѵ�.
class MSG_RTTW_GMSCOMMAND_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_GMSCOMMAND_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_GMSCOMMAND_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GMSCOMMAND_REQ;
				memset(&szMessage[0], 0, sizeof(wchar_t) * 1024);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GMSCOMMAND_REQ()
			{
			}

			DWORD				dwUID;				//	���� Application�� ���� ��ȣ
			DWORD				dwSize;				//  szMessage�� ���ڿ� ����
			wchar_t				szMessage[1024];	//  ������ ����
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

			DWORD				dwUID;				//	���� Application�� ���� ��ȣ
			DWORD				dwSize;				//  szMessage�� ���ڿ� ����
			BYTE				szMessage[1024];	//  ������ ����
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTT �� GMS
//  Description : GMS ����� ó�� ����� GMS�� �����Ѵ�.
class MSG_RTTW_GMSCOMMAND_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_GMSCOMMAND_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_GMSCOMMAND_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_GMSCOMMAND_ANS;
				memset(&szMessage[0], 0, sizeof(wchar_t) * 1024);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_GMSCOMMAND_ANS()
			{
			}

			DWORD				dwSize;				//  szMessage�� ���ڿ� ����
			wchar_t				szMessage[1024];	//  ������ ����
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

			DWORD				dwSize;				//  szMessage�� ���ڿ� ����
			BYTE				szMessage[1024];	//  ������ ����
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTT
//  Description : �� ä���� ���� ���� �����Ѵ�.
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
//  Redirection : RTTA �� General
//  Description : ������ ��Ʋ���� ������ ��û�Ѵ�.
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
//  Redirection : General �� RTTA
//  Description : ������ ��Ʋ���� ������ ������.
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
			//  ���� BATTLEZONE_INFO�� byCount��ŭ ���� ����
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTT
//  Description : Ŭ���̾�Ʈ�� ArmorKit���� �߰��� ��ŷ����
class MSG_RTTG_ARMORKIT_FINDHACK_NTF : public MSG_HEADER4
{
public:
			MSG_RTTG_ARMORKIT_FINDHACK_NTF()
			{
				wSize	= sizeof( MSG_RTTG_ARMORKIT_FINDHACK_NTF ) - SIZEOFHDR;
				wID		= RTTG_ARMORKIT_FINDHACK_NTF;
				memset(&szIP[0], 0, 16);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szBuf[0], 0, 1024);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_ARMORKIT_FINDHACK_NTF()
			{
			}

			DWORD		dwGuid;			// ��ŷ�� ������ Ŭ���̾�Ʈ guid
			char		szIP[16];		// ��ŷ�� ������ Ŭ���̾�Ʈ IP�ּ�
			DWORD		dwHackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
			DWORD		dwBufSize;		// Buffer �� ����
			char		szBuf[1024];	// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTT
//  Description : ���� �������� üũ�� ���¡ ����
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
				memset(&m_ptszUserID[0], 0, sizeof(char) * (MAX_ID_LEN + 1) );				// 07-06-11 ����ȣ(���� �ʱ�ȭ)
#else
				memset(&m_ptszUserID[0], 0, sizeof(char) * (ID_MAX_LEN + 1) );
#endif
				memset(&m_ptszCharName[0], 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH + 1) );	// 07-01-31 ����ȣ(���� �ʱ�ȭ)
				memset(&m_ptszIP[0], 0, sizeof(TCHAR) * (MAX_IP_LENGTH + 1) );				// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&m_ptszDesc[0], 0, sizeof(TCHAR) * (MAX_DESC_LENGTH + 1) );			// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				ZeroMemory(m_ptszSubTypeName, sizeof(m_ptszSubTypeName));	// 08-04-03 �ڵ���. �Ȱ����ϸ���� �����Ű��Ƽ� �ణ �ٸ��� ��...;(���, �������� �� ������ �ʹ� ������⶧���� ª���Ϸ��� �ٲ�µ�... ���ø� �ּ��� ��Ծ��ٺ��ϱ� �ּ��� ���� �׳� �ϴó����� ��ڳ�... ����� 200���� ����?! ��
			}
			~MSG_RTTG_ABUSING_NTF()
			{
			}

 			DWORD64			m_dwTime;								// time - GetDateTime_YYMMDDHHMM()�� ���� ����Ѵ�
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
//  Redirection : RTT �� General
//  Description : ��ȭ�� ������ ��û�Ѵ�.
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
//  Redirection : General �� RTT
//  Description : ��ȭ���� �����Ѵ�.
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

			// ���� ������ ����
			UINT64	ui64TotalGetMoney;		// ���� �ð� ���� ���ܳ� �� �Ѱ�
			UINT64	ui64TotalLoseMoney;		// ���� �ð� ���� ������ �� �Ѱ�
			UINT64	ui64TotalUserMoney;		// ���� ���� �������� ���� �ִ� ��� �ݾ�

			// â�� ������ ����
			UINT64	ui64AveMoney;			// â�� ��� �ݾ�
			UINT64	ui64TotalPlusMoney;		// â�� �� ��
			UINT64	ui64TotalMinusMoney;	// â���� ���� ��

			USHORT	usJackpotCnt;			// ���� ���� Ƚ��
			UINT64	ui64JackpotMoney;		// ���� �ݾ� �հ�

			UINT64	ui64CirculateMoney;		// ��ŷ� �ݾ� �հ�
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ������ �������Ḧ ��û�Ѵ�.
/*
	[2006.04.20] modify by guiun
	dwServerType, dwServerID �߰�
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

			DWORD			dwKey;			//  RTT ������ ����ϴ� �����ʵ�
			BYTE			dwWorldID;		//  0x00 = ��ü World(dwChannelID ����)
			BYTE			dwChannelID;	//  0x00 = ��ü Channel
			BYTE			dwServerType;	//  
			BYTE			dwServerID;		// 
			UINT64			i64WopsKey;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ������ �������� ���� ���� Reload ���
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
//  Redirection : RTTA �� General
//  Description : ������ �������� ���� ���� Reload ���
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
//  Redirection : RTTA �� General
//  Description : ������ �������� ���� ���� Reload ���
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
//  Redirection : RTTA �� General
//  Description : ���� �������� ���� Reload ���

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
//  Redirection : RTTA �� General
//  Description : ���� �������� GameClinet, Laucher version Reload ���
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
//  Redirection : General �� RTTA
//  Description : ���� �������� ����� �뺸�Ѵ�.
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

			DWORD			dwKey;				//  RTT ������ ����ϴ� �����ʵ�
			DWORD			dwError;			//  0 : �Ϸ�, 1 : �����߻�
			SERVER_KEY		m_ServerKey;		//  ���� �߻��� ���, Server Key Ȯ��
			__int64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ��ġ ����� �뺸�� ��û�Ѵ�.
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

			DWORD			dwKey;			//  RTT ������ ����ϴ� �����ʵ�
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTTA
//  Description : ��ġ ����� �뺸�Ѵ�.
class MSG_RTTG_PATCHRESULT_ANS : public MSG_HEADER4
{
public:
	static	const DWORD		ERROR_PATCHSUCCESS			= 0x00;		//  ��ġ ����
	static  const DWORD		ERROR_PROCESSRUN			= 0x01;		//  �ش� ���μ��� ������		
	static	const DWORD		ERROR_FTPCONNECT_FAIL		= 0x02;		//  ftp ���� ����
	static	const DWORD		ERROR_DOWNLOAD_FAIL			= 0x03;		//  ���� �ޱ� ����
	static	const DWORD		ERROR_FILENOTFOUND			= 0x04;		//  ��ġ���� ã�� ����
	static	const DWORD		ERROR_INVALID_SERVERTYPE	= 0x05;		//  �߸��� ���� Ÿ��

	//  RTT ���� �ڵ�
	static	const DWORD		ERROR_NONE				= 0xFC;		//  nothing
	static	const DWORD		ERROR_NOTEXISTLAUNCHER	= 0xFD;		//  ���ĸ� ������ �� ����
	static	const DWORD		ERROR_PROGRESS			= 0xFE;		//  ��ġ ������
	static	const DWORD		ERROR_COMPLETE			= 0xFF;		//  ��ġ �Ϸ�

public:
			MSG_RTTG_PATCHRESULT_ANS()
			{
				wSize	= sizeof( MSG_RTTG_PATCHRESULT_ANS ) - SIZEOFHDR;
				wID		= RTTG_PATCHRESULT_ANS;
			}
			~MSG_RTTG_PATCHRESULT_ANS()
			{
			}

			DWORD			dwKey;			//  RTT ������ ����ϴ� �����ʵ�
			DWORD			dwError;		//  0 : ����, 1 �̻� : ������ Ȯ��
			BYTE		    byIndex;
			BYTE		    byServerType;
			BYTE			byVer1;			//  ���μ����� ����
			BYTE			byVer2;
			BYTE			byVer3;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT
//  Description : RTT_A ���񽺸� ���۽�ų ���� ��û�Ѵ�.
class MSG_RTTW_RTTASTART_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTART_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTART_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTART_REQ;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_RTTASTART_REQ()
			{
			}


			DWORD			dwFlag;						//  0 : ��ü RTTA�� ������� ����
														//  1 : szIPAddress�� �ش��ϴ� RTTA�� ����
			wchar_t			szIPAddress[IP_MAX_LEN];
};

class MSG_RTTW_RTTASTART_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTART_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTART_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTART_ANS;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_RTTASTART_ANS()
			{
			}


			DWORD			dwResult;					//  0 : ����,		1 : ����		2 : RTTA�˻� ����
			wchar_t			szIPAddress[IP_MAX_LEN];	//  ������ �ּ�
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT
//  Description : RTT_A ���񽺸� �����ų ���� ��û�Ѵ�.
class MSG_RTTW_RTTASTOP_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTOP_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTOP_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTOP_REQ;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_RTTASTOP_REQ()
			{
			}


			DWORD			dwFlag;						//  0 : ��ü RTTA�� ������� ����
														//  1 : szIPAddress�� �ش��ϴ� RTTA�� ����
			wchar_t			szIPAddress[IP_MAX_LEN];
};


class MSG_RTTW_RTTASTOP_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTASTOP_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_RTTASTOP_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTASTOP_ANS;
				memset(&szIPAddress[0], 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_RTTASTOP_ANS()
			{
			}


			DWORD			dwResult;					//  0 : ����,		1 : ����
			wchar_t			szIPAddress[IP_MAX_LEN];	//  ������ �ּ�
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S �� RTT_Service
//  Description : RTT_A�� ��ġ�� ����Ѵ�.
class MSG_RTTW_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_RTTAPATCH_NTF;
				memset(&szFtpAddress[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpUser[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpPassword[0]	, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpGetFilePath[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpGetFileName[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_RTTAPATCH_NTF()
			{
			}


			DWORD			dwWorldID;				//  0 : ��ü
			DWORD			dwChannelID;			//  0 : ��ü

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


			DWORD			dwWorldID;				//  0 : ��ü
			DWORD			dwChannelID;			//  0 : ��ü

			wchar_t			szIp[20];
			wchar_t			szPath[40];
			wchar_t			szAccount[20];
			wchar_t			szPassword[40];
};
*/


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT
//  Description : �ý����� ������� ��û�Ѵ�.
class MSG_RTTW_SYSTEMREBOOT_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_SYSTEMREBOOT_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_SYSTEMREBOOT_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_SYSTEMREBOOT_NTF;
				memset(&szIPAddress[0]	, 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_SYSTEMREBOOT_NTF()
			{
			}

			wchar_t			szIPAddress[IP_MAX_LEN];	//  ������ �ּ�
			BOOL			bReboot;					// ����ÿ���(FALSE�� ������.) 08-01-28 �ڵ��� by ����ȣ
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT_S �� RTT_A
//  Description : ���μ��� ���� ����� ������
class MSG_RTTW_PROCESSCONTROL_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_PROCESSCONTROL_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_PROCESSCONTROL_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_PROCESSCONTROL_NTF;
				memset(&szIPAddress[0]		, 0, sizeof(wchar_t) * IP_MAX_LEN);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szBinaryName[0]		, 0, sizeof(wchar_t) * 100);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szAbsolutePath[0]	, 0, sizeof(wchar_t) * 100);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_PROCESSCONTROL_NTF()
			{
			}


			DWORD		dwCommand;					//  0 : Process Kill, 1 : Process Run
			wchar_t		szIPAddress[IP_MAX_LEN];	//  ������ IP�ּ�
			wchar_t		szBinaryName[100];			//  ���̳ʸ�
			wchar_t		szAbsolutePath[100];		//  ���� ���
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� Auth
//  Description : ������ ������ ����� ������ ������ �����Ų��.
class MSG_RTTW_DROPUSER_REQ : public MSG_HEADER8
{
public:
			MSG_RTTW_DROPUSER_REQ()
			{
				dwSize	= sizeof( MSG_RTTW_DROPUSER_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_DROPUSER_REQ;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_DROPUSER_REQ()
			{
			}

			wchar_t			szUserID[ID_MAX_LEN + 1];			//  ���������ų ������� ����(ID)
};

/**
	2006.11.28
	modify by guiun
	AuthServer�� ���۵Ǵ� ����� DROP ��Ŷ�� DROP ������ ���� �� �ִ� DropCode�� �߰��Ѵ�.
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
				memset(&szUserID[0]	, 0, ID_MAX_LEN + 1);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_DROPUSER_REQ()
			{
			}

			char			szUserID[ID_MAX_LEN + 1];			//  ���������ų ������� ����(ID)
			DWORD			dwDropCode;
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Auth �� RTT �� GMS
//  Description : ����� ���� ����ó�� ��, ����� �����Ѵ�.
class MSG_RTTW_DROPUSER_ANS : public MSG_HEADER8
{
public:
			MSG_RTTW_DROPUSER_ANS()
			{
				dwSize	= sizeof( MSG_RTTW_DROPUSER_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_DROPUSER_ANS()
			{
			}

			DWORD			dwError;							//  0 : ����		1 : �������� ���� ����
																//  2 : ������ ��� ����	3 : �̹� �۾��� �������� ����
			wchar_t			szUserID[ID_MAX_LEN + 1];			//  ��û�� ������� ����(ID)
};

class MSG_RTTG_DROPUSER_ANS : public MSG_HEADER4
{
public:
			MSG_RTTG_DROPUSER_ANS()
			{
				wSize	= sizeof( MSG_RTTG_DROPUSER_ANS ) - SIZEOFHDR;
				wID		= RTTG_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, ID_MAX_LEN + 1);		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTG_DROPUSER_ANS()
			{
			}

#ifdef  _090427_HTTP_WEB_DROP_USER
			DWORD			dwDropCode;							//	REQ�� ���� ��� �ڵ带 ������ ��(�ϴ� �߱� �� �󿡼� ���� ���� ��ɿ��� �߰��ȴ�)
#endif
			DWORD			dwError;							//  0 : ����		1 : �������� ���� ����
																//  2 : ������ ��� ����
			char			szUserID[ID_MAX_LEN + 1];			//  ��û�� ������� ����(ID)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A �� RTT_S
//  Description : �ý����� Resource ��Ȳ�� �����Ѵ�.
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
			DWORD			dwMemoryUse;				//  Memory ��뷮
			DWORD			dwMemoryMax;				//  Memory �ִ밪
			DWORD			dwHDDUse;					//  HDD ��뷮
			DWORD			dwHDDMax;					//  HDD �ִ밪
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTS �� RTTA
//  Description : ������ ��Ʋ���� ������ ��û�Ѵ�.
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
//  Redirection : RTTA �� RTTS
//  Description : ������ ��Ʋ���� ������ ������.
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
//  Redirection : RTTA �� RTTS
//  Description : ArmorKit���� �߻��� ������ ������.
class MSG_RTTW_ARMORKIT_FINDHACK_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_ARMORKIT_FINDHACK_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_ARMORKIT_FINDHACK_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ARMORKIT_FINDHACK_NTF;
				memset(&szIP[0]	, 0, sizeof(wchar_t) * 16);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szBuf[0], 0, 1024);					// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTW_ARMORKIT_FINDHACK_NTF()
			{
			}


			DWORD		dwGuid;			// ��ŷ�� ������ Ŭ���̾�Ʈ guid
			wchar_t		szIP[16];		// ��ŷ�� ������ Ŭ���̾�Ʈ IP�ּ�
			DWORD		dwHackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
			DWORD		dwBufSize;		// Buffer �� ����
			char		szBuf[1024];	// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� RTTS
//  Description : ���¡ ����
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
		memset(&m_ptszUserID[0], 0, sizeof(char) * (MAX_ID_LEN + 1) );				// 07-06-11 ����ȣ(���� �ʱ�ȭ)
#else
		memset(&m_ptszUserID[0], 0, sizeof(char) * (ID_MAX_LEN + 1) );
#endif

		memset(&m_ptszCharName[0], 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH + 1) );	// 07-02-01 ����ȣ(���� �ʱ�ȭ)
		memset(&m_ptszIP[0]			, 0, sizeof(TCHAR) * (MAX_IP_LENGTH + 1)   );		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
		memset(&m_ptszDesc[0]		, 0, sizeof(TCHAR) * (MAX_DESC_LENGTH + 1) );		// 07-01-03 ����ȣ(���� �ʱ�ȭ)
		ZeroMemory(m_ptszSubTypeName, sizeof(m_ptszSubTypeName));						// 08-04-03 �ڵ���(���ڸ�Ʈ ��-)
	}
	~MSG_RTTW_ABUSING_NTF()
	{
	}

	DWORD64			m_dwTime;										// time - GetDateTime_YYMMDDHHMM()�� ���� ����Ѵ�
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
//  Redirection : RTTS �� WOPS
//  Description : ��� �߻��� ���� ����
class MSG_RTTW_ERRORNOTIFY_NTF : public MSG_HEADER8
{
public:
			MSG_RTTW_ERRORNOTIFY_NTF()
			{
				dwSize	= sizeof( MSG_RTTW_ERRORNOTIFY_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTW_ERRORNOTIFY_NTF;
				memset(&szMessage[0]	, 0, sizeof(wchar_t) * 120 );	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szDateTime[0]	, 0, sizeof(wchar_t) * 20 );	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
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
//  Redirection : WOPS �� RTTS
//  Description : WOPS�� ���� ������ �����Ѵ�.
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
			//  0 : �������� ����	- RTTS�� WOPS�� ���� ��Ŷ�� ���� �� �ִ�.
			//  1 : �ܹ����� ����	- WOPS�� ��û�� ���� ��츸 ��Ŷ�� ���� �� �ִ�.
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� RTTS
//  Description : ��ȭ�� ������ ��û�Ѵ�.
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
//  Redirection : RTTA �� RTTS
//  Description : ��ȭ���� ������.
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

			// ���� ������ ����
			UINT64	ui64TotalGetMoney;		// ���� �ð� ���� ���ܳ� �� �Ѱ�
			UINT64	ui64TotalLoseMoney;		// ���� �ð� ���� ������ �� �Ѱ�
			UINT64	ui64TotalUserMoney;		// ���� ���� �������� ���� �ִ� ��� �ݾ�

			// â�� ������ ����
			UINT64	ui64AveMoney;			// â�� ��� �ݾ�
			UINT64	ui64TotalPlusMoney;		// â�� �� ��
			UINT64	ui64TotalMinusMoney;	// â���� ���� ��

			USHORT	usJackpotCnt;			// ���� ���� Ƚ��
			UINT64	ui64JackpotMoney;		// ���� �ݾ� �հ�

			UINT64	ui64CirculateMoney;		// ��ŷ� �ݾ� �հ�
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ������ �������Ḧ ��û�Ѵ�.

/*
	[2006.04.20] modify by guiun
	dwServerType, dwServerID �߰�
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

			DWORD			dwKey;			//  RTT ������ ����ϴ� �����ʵ�
			DWORD			dwWorldID;		//  0x00 = ��ü World(dwChannelID ����)
			DWORD			dwChannelID;	//  0x00 = ��ü Channel
			DWORD			dwServerType;	//  
			DWORD			dwServerID;		//  
			UINT64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT �� WOPS
//  Description : ���� �������� ó�� ����� �����Ѵ�.
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

			DWORD			dwKey;				//  RTT ������ ����ϴ� �����ʵ�
			DWORD			dwError;			//  0 : �Ϸ�, 1 : �����߻�
			SERVER_KEY		m_ServerKey;		//  ���� �߻��� ���, Server Key Ȯ��
			__int64			i64WopsKey;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ��ġ ������ ������ ����Ѵ�.
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

			DWORD			dwWorldID;		//  0x00 = ��ü World(dwChannelID ����)
			DWORD			dwChannelID;	//  0x00 = ��ü Channel
			DWORD			dwServerType;	//  
			DWORD			dwServerID;		// 
			DWORD			dwServerTypeForProcess;
			__int64			i64WopsKey;		//  WOPS�� DB�� ������ key��
			DWORD			dwKey;			//  RTT ������ ����ϴ� �����ʵ�
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ������� ���̺� �ٽ� �о Game�����鿡 Broadcast
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
//  Redirection : WOPS �� RTT
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
//  Redirection : GMS �� RTT �� Master
//  Description : ���� ���� ��û.
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

			SERVER_KEY	dwServerGuid;								// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
			DWORD		dwGuildGuid;								// ��� GUID
			wchar_t		wszGuildName[MAX_GUILD_NAME_LENGTH + 1];	// �����ϰ��� �ϴ� ����
			wchar_t		wszDescription[MAX_DESCRIPTION_LENGTH + 1];	// ���� ����
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

			DWORD		dwWopsKey;									// ��� �ĺ���
            SERVER_KEY	dwServerGuid;								// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
			DWORD		dwGuildGuid;								// ��� GUID
			char		szGuildName[MAX_GUILD_NAME_LENGTH + 1];		// �����ϰ��� �ϴ� ����
			char		szDescription[MAX_DESCRIPTION_LENGTH + 1];	// ���� ����
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master �� RTT �� GMS
//  Description : ���� ���� ��û ���
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

			DWORD		dwResult;													// 0 : ���� ����, 1 : ���� ����, 2 : ������ ��ſ���(RTT������ ��ſ���)
			wchar_t		wszResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// ó�� ����� ���� Description
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

			DWORD		dwWopsKey;												// ��� �ĺ���
			DWORD		dwResult;												// 0 : ���� ����, 1 : ���� ����
			char		szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// ó�� ����� ���� Description
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� Master
//  Description : �������� ������ ���� ��û.
class MSG_RTTW_SEND_MESSAGE_REQ : public MSG_HEADER8
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_MESSAGE_LENGTH = 100, MAX_USER_INFO = 10, };

	typedef struct tag_USER_INFO
	{
		wchar_t	wszRecvCharName[MAX_CHAR_NAME_LENGTH + 1];	// ������ ���� ĳ���͸�
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

			SERVER_KEY		dwServerGuid;								// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
			DWORD			dwCount;									// ���� ���� ����
            USER_INFO		arrUserInfoList[MAX_USER_INFO];				// ���� ������
			wchar_t			wszSendCharName[MAX_CHAR_NAME_LENGTH + 1];	// ������ ���� ĳ���͸�
			wchar_t			wszMessage[MAX_MESSAGE_LENGTH + 1];			// ���� ����
};

class MSG_RTTG_SEND_MESSAGE_REQ : public MSG_HEADER4
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_MESSAGE_LENGTH = 200, MAX_USER_INFO = 10, };

	typedef struct tag_USER_INFO
	{
		char	szRecvCharName[MAX_CHAR_NAME_LENGTH + 1];	// ������ ���� ĳ���͸�
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

			DWORD			dwWopsKey;									// ��� �ĺ���
            SERVER_KEY		dwServerGuid;								// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
			DWORD			dwCount;									// ���� ���� ����
            USER_INFO		arrUserInfoList[MAX_USER_INFO];				// ���� ������
			char			szSendCharName[MAX_CHAR_NAME_LENGTH + 1];	// ������ ���� ĳ���͸�
			char			szMessage[MAX_MESSAGE_LENGTH + 1];			// ���� ����
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master �� RTT �� GMS
//  Description : �������� ���� ���� ��û ���
class MSG_RTTW_SEND_MESSAGE_ANS : public MSG_HEADER8
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_RESULT_INFO = 10, MAX_RESULT_DESCRIPTION_LENGTH = 200, };

	typedef struct tag_RESULT_INFO
	{
		DWORD	dwResult;													// 0 : ���� ����, 1 : ���� ����, 2 : ������ ��ſ���(RTT������ ��ſ���)
		wchar_t	wszRecvCharName[MAX_CHAR_NAME_LENGTH + 1];					// ������ ���� ĳ���͸�
		wchar_t	wszResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// ���� ���� ��� Description
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

			DWORD			dwCount;							// ���� ���� ����
			RESULT_INFO		arrResultInfoList[MAX_RESULT_INFO];	// ���� ������
};

class MSG_RTTG_SEND_MESSAGE_ANS : public MSG_HEADER4
{
public:

	enum{ MAX_CHAR_NAME_LENGTH = 16, MAX_RESULT_INFO = 10, MAX_RESULT_DESCRIPTION_LENGTH = 200, };

	typedef struct tag_RESULT_INFO
	{
		DWORD	dwResult;												// 0 : ���� ����, 1 : ���� ����
		char	szRecvCharName[MAX_CHAR_NAME_LENGTH + 1];				// �޽����� ���� ĳ���͸�
		char	szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];	// ���� ���� ��� Description
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

			DWORD			dwWopsKey;							// ��� �ĺ���
			DWORD			dwCount;							// ���� ���� ��� ����
			RESULT_INFO		arrResultInfoList[MAX_RESULT_INFO];	// ���� ���� ��� ������
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ���� �������� ���� ���� IP ���̺� Reload ���
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
			DWORD dwResult;// 1 ����, 0 ����
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ���� �������� GameClinet, Laucher version Reload ���
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
//  Redirection : RTTS �� WOPS
//  Description : ��ġ ����� �����Ѵ�.
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

			DWORD			dwKey;				//  RTT ������ ����ϴ� �����ʵ�
			DWORD			dwError;			//  0 : ����, 1 �̻� : MSG_RTTG_PATCHRESULT_ANS����
			BYTE			byIndex;			//  ���� ���μ����� ���� ��ų ��� ���° ����� ���μ������� ����(MAX->0)
			BYTE			byServerType;
			BYTE			byVer1;				//  ���μ����� ����
			BYTE			byVer2;
			BYTE			byVer3;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ������ �ӽ��� ���� ��� ���μ����� ��� ���۽�Ų��.
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


			DWORD			dwWorldID;			//  1 �̻��� ���� ����Ѵ�.
			DWORD			dwChannelID;		//  0 : ��ü ä��, 1 �̻� : ������ ä��
			DWORD			dwServerType;
			DWORD			dwServerID;
			DWORD			dwServerTypeForProcess;
			__int64			i64WopsKey;

			//  ���� �ʵ�� RTT �������� �������� ����Ѵ�.
			DWORD			dwWopsUID;
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT �� WOPS
//  Description : ������ �ӽ��� ���� ��� ���μ����� ��� ���۽�Ų��.
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


			DWORD			dwError;			//  0 : ����		1 : ���μ��� ���� ����(RTTA �α� Ȯ��)

			//  ���� �ʵ�� RTT �������� �������� ����Ѵ�.
			DWORD			dwServerKey;
			DWORD			dwWopsUID;
			__int64			i64WopsKey;			
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S �� RTT_Service
//  Description : RTT_A ���񽺸� ���۽�ų ���� ��û�Ѵ�.
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
//  Redirection : RTT_S �� RTT_Service
//  Description : RTT_A ���񽺸� �����ų ���� ��û�Ѵ�.
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
//  Redirection : RTT_S �� RTT_Service
//  Description : RTT_A�� ��ġ�� ����Ѵ�.
class MSG_RTTR_RTTAPATCH_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_RTTAPATCH_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_RTTAPATCH_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_RTTAPATCH_NTF;
				memset(&szFtpAddress[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpUser[0]		, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpPassword[0]	, 0, sizeof(wchar_t) * 20);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpGetFilePath[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szFtpGetFileName[0]	, 0, sizeof(wchar_t) * 40);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTR_RTTAPATCH_NTF()
			{
			}

			DWORD			dwWorldID;				//  0 : ��ü
			DWORD			dwChannelID;			//  0 : ��ü

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
//  Redirection : RTT_A �� RTT_S
//  Description : ���� ���μ��� ����� ��û�Ѵ�.
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
//  Redirection : RTT_S �� RTT_A
//  Description : ���� ���μ��� ����� �����Ѵ�.
class MSG_RTTR_PROCESSLIST_ANS : public MSG_HEADER8
{
public:
			MSG_RTTR_PROCESSLIST_ANS()
			{
				dwSize	= sizeof( MSG_RTTR_PROCESSLIST_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PROCESSLIST_ANS;

				for( int i = 0 ; i < 10 ; ++i )
				{
					memset(&szBinaryName[i][0]		, 0, sizeof(wchar_t) * 100);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
					memset(&szAbsolutePath[i][0]	, 0, sizeof(wchar_t) * 100);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				}
			}
			~MSG_RTTR_PROCESSLIST_ANS()
			{
			}


			DWORD		dwFlag;						//  0 : �ʱ�ȭ �� ����.   1 : ���� �� �߰�
			int			dwCount;					//  ��� ����
			wchar_t		szBinaryName[10][100];		//  ���̳ʸ�
			wchar_t		szAbsolutePath[10][100];	//  ���� ���
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A �� RTT_S
//  Description : ���μ����� ���¸� �����Ѵ�.
class MSG_RTTR_PROCESSMONITOR_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_PROCESSMONITOR_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_PROCESSMONITOR_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PROCESSMONITOR_NTF;

				for( int i = 0 ; i < 20 ; ++i )
				{
					memset(&szBinaryName[i][0]		, 0, sizeof(wchar_t) * 100);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
					memset(&szAbsolutePath[i][0]	, 0, sizeof(wchar_t) * 100);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				}
				memset(&dwStatus[0], 0, sizeof(DWORD) * 20);
			}
			~MSG_RTTR_PROCESSMONITOR_NTF()
			{
			}


			int			dwCount;					//  ��� ����
			wchar_t		szBinaryName[20][100];		//  ���̳ʸ�
			wchar_t		szAbsolutePath[20][100];	//  ���� ���
			DWORD		dwStatus[20];				//  0 : Inactive, 1 : Active
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_S �� RTT_A
//  Description : ��ġ ������ ������ �����Ѵ�.
class MSG_RTTR_PATCHLAUNCHERINFO_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_PATCHLAUNCHERINFO_NTF()
			{
				dwSize	= sizeof( MSG_RTTR_PATCHLAUNCHERINFO_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_PATCHLAUNCHERINFO_NTF;
                memset(&szAbsolutePath[0]	, 0, sizeof(wchar_t) * 100);// 07-01-03 ����ȣ(���� �ʱ�ȭ)
				memset(&szBinaryName[0]		, 0, sizeof(wchar_t) * 60);	// 07-01-03 ����ȣ(���� �ʱ�ȭ)
			}
			~MSG_RTTR_PATCHLAUNCHERINFO_NTF()
			{
			}

			wchar_t		szAbsolutePath[100];	//  ������
			wchar_t		szBinaryName[60];		//  �������ϸ�
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A �� RTT_S
//  Description : ���� URL�� ��û�Ѵ�.(06-12-5, ����ȣ)
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
//  Redirection : RTT_S �� RTT_A
//  Description : ���� URL�� �����Ѵ�.(06-12-5, ����ȣ)
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

			bool		bExist;				// ������ URL�� �ִ����� ���� �÷���
			DWORD		dwURLIndex;			// URL ���� �ε���
			wchar_t		szURL[MAX_URL];		// URL
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT_A �� RTT_S
//  Description : Request ����� �����Ѵ�.(06-12-5, ����ȣ)
class MSG_RTTR_WEB_REQUEST_NTF : public MSG_HEADER8
{
public:
			MSG_RTTR_WEB_REQUEST_NTF() : dwURLIndex(0), dwStatusCode( HTTP_STATUS_OK )		// �������� �ʱ�ȭ �Ѵ�.
			{
				dwSize	= sizeof( MSG_RTTR_WEB_REQUEST_NTF ) - sizeof( MSG_HEADER8 );
				dwID	= RTTR_WEB_REQUEST_NTF;
			}
			~MSG_RTTR_WEB_REQUEST_NTF()
			{
			}

			DWORD		dwURLIndex;			// ���� URL ���� �ε���
			DWORD		dwStatusCode;		// Request �����ڵ�
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTServer �� RTTAgent
//  Description : ���� ��� ��û 
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
//  Redirection : RTTAgent �� RTTServer 
//  Description : ���� ��� ����
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
			UINT64		ui64Statistics[eSTATISTIC_MAX + 1];	// ��ü ���(������ eSTATISTICS_TYPE ����). USERMONEY_ANS���� ������ ������ ä���� �ʴ´�
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTAgent �� RTTServer 
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
//  Redirection : RTTAgent �� Master
//  Description : ���� ��� ��û 
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
//  Redirection : Master �� RTTAgent
//  Description : ���� ��� ����
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
			UINT64		ui64Statistics[eSTATISTIC_MAX + 1];	// ��ü ���(������ eSTATISTICS_TYPE ����). USERMONEY_ANS���� ������ ������ ä���� �ʴ´�
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WOPS �� RTT
//  Description : ���� �������� ���� Reload ���
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
			DWORD dwResult;// 1 ����, 0 ����
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : WEB �� RTT �� Auth
//  Description : �߱� �� ��û�� ���� ������ ������ ����� ������ ������ �����Ų��
class MSG_RTTH_DROPUSER_REQ : public MSG_HEADER8
{
public:
			MSG_RTTH_DROPUSER_REQ()
			{
				dwSize	= sizeof( MSG_RTTH_DROPUSER_REQ ) - sizeof( MSG_HEADER8 );
				dwID	= RTTH_DROPUSER_REQ;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// ���� �ʱ�ȭ
			}
			~MSG_RTTH_DROPUSER_REQ()
			{
			}

			wchar_t			szUserID[ID_MAX_LEN + 1];			//  ���������ų ������� ����(ID)
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Auth �� RTT �� WEB
//  Description : �߱� �� ��û�� ���� ������ ������� ������ ����ó�� ��, ����� �����Ѵ�.
class MSG_RTTH_DROPUSER_ANS : public MSG_HEADER8
{
public:
			MSG_RTTH_DROPUSER_ANS()
			{
				dwSize	= sizeof( MSG_RTTH_DROPUSER_ANS ) - sizeof( MSG_HEADER8 );
				dwID	= RTTH_DROPUSER_ANS;
				memset(&szUserID[0]	, 0, sizeof(wchar_t) * ID_MAX_LEN + 1);		// (���� �ʱ�ȭ)
			}
			~MSG_RTTH_DROPUSER_ANS()
			{
			}

			DWORD			dwError;							//  0 : ����		1 : �������� ���� ����
																//  2 : ������ ��� ����	3 : �̹� �۾��� �������� ����
			wchar_t			szUserID[ID_MAX_LEN + 1];			//  ��û�� ������� ����(ID)
};




};


using namespace Protocol_RTT;
#pragma pack()