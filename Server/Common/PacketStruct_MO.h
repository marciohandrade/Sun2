#pragma once

#include <Protocol_MO.h>
#include <ServerStruct.h>
#include <PacketStruct.h>

#pragma pack(push,1)

/*
// ������ ������ Ư�� ������ ������ ���ε� ��û
struct MSG_MO_RELOAD_DATA_SYN : public MSG_BASE_INTERNAL
{
	BYTE	byWorldID;
	BYTE	byChannelID;
	BYTE	byServerType;
	BYTE	byServerID;
};
*/

// SERVER_KEY ���� �ٲ�鼭 ����Ʈ ���������� �ӽ÷� ���� - ��������δ� ����ü�� ������ �ʴ� ���� ���ڴ�(����â)
typedef struct tagOLDSERVER_KEY
{
	DWORD dwKey;

	tagOLDSERVER_KEY( const DWORD key = 0 ) { dwKey = key; }
	operator DWORD()		{ return dwKey; }

	DWORD Set( BYTE worldID, BYTE channelID, BYTE serverType, BYTE serverID )
	{
		memcpy( &dwKey, &worldID, sizeof(BYTE) );
		dwKey <<= 8;	memcpy( &dwKey, &channelID, sizeof(BYTE) );
		dwKey <<= 8;	memcpy( &dwKey, &serverType, sizeof(BYTE) );
		dwKey <<= 8;	memcpy( &dwKey, &serverID, sizeof(BYTE) );
		return dwKey;
	}
	BYTE GetWorldID()		{ BYTE id; DWORD temp = dwKey; temp >>= 24; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetChannelID()	{ BYTE id; DWORD temp = dwKey; temp >>= 16; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetServerType()		{ BYTE id; DWORD temp = dwKey; temp >>= 8; memcpy( &id, &temp, sizeof(BYTE) ); return id; }
	BYTE GetServerID()		{ BYTE id; memcpy( &id, &dwKey, sizeof(BYTE) ); return id; }
	const DWORD operator=(const SERVER_KEY &ServerKey)
	{
		this->Set( ServerKey.m_WorldID, ServerKey.m_ChannelID, ServerKey.m_ServerType, ServerKey.m_ServerID );
		return dwKey;
	}
} OLD_SERVER_KEY, *LPOLD_SERVER_KEY;

// OLD_SERVER_KEY ��ü��
#define SWAP_32BIT_DATATYPE(dwOldSK)	\
	(( ((dwOldSK)&0x000000FF) << 24) | (((dwOldSK)&0x0000FF00) << 8) | (((dwOldSK)&0x00FF0000) >> 8) | (((dwOldSK)&0xFF000000) >> 24))



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? �� RTT
//  Description : RTT�� ����ϴ� ���α׷��� RTT�� ������ �̷���� �Ŀ� ������ �޾ƾ��Ѵ�.
class MSG_MO_RTTG_CERTIFY_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_CERTIFY_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_CERTIFY_REQ;

	}
	~MSG_MO_RTTG_CERTIFY_REQ()
	{
	}

	DWORD			dwType;						//  AGENTTYPE_ define�� ����Ѵ�.
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? �� RTT
//  Description : RTT�� ����ϴ� ���α׷��� RTT�� ������ �̷���� �Ŀ� ������ �޾ƾ��Ѵ�.
class MSG_MO_RTTG_CERTIFY_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_CERTIFY_ANS()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_CERTIFY_ANS;
	}
	~MSG_MO_RTTG_CERTIFY_ANS()
	{
	}

	DWORD			dwResult;					//  0 : ���� ����
	DWORD			dwType;						//  SERVERTYPE_ define�� ����Ѵ�.
	DWORD			dwUID;						//  ������ ������ȣ
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS �� RTT �� General
//  Description : ���ӳ� ������ �߼��Ѵ�.
class MSG_MO_RTTG_GAMENOTICE_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMENOTICE_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_GAMENOTICE_REQ;
	}
	~MSG_MO_RTTG_GAMENOTICE_REQ()
	{
	}

	DWORD				dwServerUID;				//  ������ ������ ������ uid.(0=��ü)
	DWORD				dwTickPerLine;				//  1���� ��� �ð�����(ms)
	DWORD				dwNoticeLength;				//  ���� ���ڿ��� ����
	char				szNotice[MAX_NOTICE_LEN+1];	//  ���� ���ڿ�
	BYTE				m_byRed;
	BYTE				m_byGreen;
	BYTE				m_byBlue;

};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS, General �� RTT
//  Description : ���ӳ� ������ ���� ����� ��û�Ѵ�.
class MSG_MO_RTTG_GAMEADMINLIST_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMEADMINLIST_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_GAMEADMINLIST_REQ;
	}
	~MSG_MO_RTTG_GAMEADMINLIST_REQ()
	{
	}
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTT �� GMS, General
//  Description : ������ ���� ����� �����Ѵ�.
class MSG_MO_RTTG_GAMEADMINLIST_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMEADMINLIST_ANS()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_GAMEADMINLIST_ANS;
	}
	~MSG_MO_RTTG_GAMEADMINLIST_ANS()
	{
	}

	DWORD				dwCount;				//  ���� ������ ����
	//  ���� ���� �����͵��� dwCount ���ķ� dwCount��ŭ �ݺ�
	struct sGM_INFO_APPENDED
	{
		char			szAccount[ID_MAX_LEN];
		char			szIP[IP_MAX_LEN];
		DWORD			dwAttr;
	};

	inline sGM_INFO_APPENDED*	GetAppendedDataPtr() { return (sGM_INFO_APPENDED*)((&dwCount)+1); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ������ ��Ʋ���� ������ ��û�Ѵ�.
class MSG_MO_RTTG_BATTLEZONEINFO_REQ : public MSG_BASE_INTERNAL
{
public:
			MSG_MO_RTTG_BATTLEZONEINFO_REQ()
			{
				m_byCategory = MO_OPERATION;
				m_byProtocol = MO_RTTG_BATTLEZONEINFO_REQ;
			}
			~MSG_MO_RTTG_BATTLEZONEINFO_REQ()
			{
			}

	int GetSize()	{ return sizeof(MSG_MO_RTTG_BATTLEZONEINFO_REQ); }
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTTA
//  Description : ������ ��Ʋ���� ������ ������.
struct MSG_MO_RTTG_BATTLEZONEINFO_ANS : public MSG_BASE_INTERNAL
{
	enum { MAX_CHANNEL_NUM = 256, };

	MSG_MO_RTTG_BATTLEZONEINFO_ANS() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_BATTLEZONEINFO_ANS; }

	BYTE					byCount;						// ä�� ����
	STRUCT_BATTLEZONE_INFO	ChannelInfo[MAX_CHANNEL_NUM];	// ä�δ� ��Ʋ�� ����

	int GetSize() { return ( sizeof(MSG_MO_RTTG_BATTLEZONEINFO_ANS) - ( sizeof(STRUCT_BATTLEZONE_INFO) * ( MAX_CHANNEL_NUM - byCount ) ) ); }
};

class MSG_MO_ARMORKIT_FINDHACK_NTF : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_ARMORKIT_FINDHACK_NTF() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_ARMORKIT_FINDHACK_NTF; }
	~MSG_MO_ARMORKIT_FINDHACK_NTF() { }

	DWORD	dwGuid;			// ��ŷ�� ������ Ŭ���̾�Ʈ guid
	TCHAR	szIP[16];		// ��ŷ�� ������ Ŭ���̾�Ʈ IP�ּ�
	int		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
	int		Size;			// Buffer �� ����
	TCHAR	Buf[1024];		// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)

	int GetSize()	{ return sizeof(MSG_MO_ARMORKIT_FINDHACK_NTF); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� Master
//  Description : ������ ��ȭ�� ���� ��û
class MSG_MO_RTTG_USERMONEY_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERMONEY_REQ() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_USERMONEY_REQ; }
	~MSG_MO_RTTG_USERMONEY_REQ() { }

	int GetSize()	{ return sizeof(MSG_MO_RTTG_USERMONEY_REQ); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master �� RTTA
//  Description : ������ ��ȭ�� ���� ����
class MSG_MO_RTTG_USERMONEY_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERMONEY_ANS() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_USERMONEY_ANS; }
	~MSG_MO_RTTG_USERMONEY_ANS() { }

//	SERVER_KEY	ServerKey;
	BYTE		byWorldID;
	BYTE		byChannelID;
	BYTE		byServerType;
	BYTE		byServerID;

	ULONGLONG	ui64TotalGetMoney;		// ���� �ð� ���� ���ܳ� �� �Ѱ�
	ULONGLONG	ui64TotalLoseMoney;		// ���� �ð� ���� ������ �� �Ѱ�
	ULONGLONG	ui64TotalUserMoney;		// ���� ���� �������� ���� �ִ� ��� �ݾ�

	ULONGLONG	ui64WarehouseAveMoney;		// â�� ��� �ݾ�
	ULONGLONG	ui64WarehousePlusMoney;		// â�� �� ��
	ULONGLONG	ui64WarehouseMinusMoney;	// â���� ���� ��

	USHORT		usJackpotCnt;				// ���� ���� Ƚ��
	ULONGLONG	JackpotMoney;				// ���� �ݾ� �հ�

	ULONGLONG	CirculateMoney;				// ��ŷ� �ݾ� �հ�
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� Master
//  Description : ������ ��Ÿ ��� ���� ��û
class MSG_MO_RTTG_GAMESTATISTICS_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMESTATISTICS_REQ() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_GAMESTATISTICS_REQ; }
	~MSG_MO_RTTG_GAMESTATISTICS_REQ() { }

	int GetSize()	{ return sizeof(MSG_MO_RTTG_GAMESTATISTICS_REQ); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master �� RTTA
//  Description : ������ ��ȭ�� ���� ����
class MSG_MO_RTTG_GAMESTATISTICS_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMESTATISTICS_ANS() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_GAMESTATISTICS_ANS; }
	~MSG_MO_RTTG_GAMESTATISTICS_ANS() { }

//	SERVER_KEY	ServerKey;
	BYTE		byWorldID;
	BYTE		byChannelID;
	BYTE		byServerType;
	BYTE		byServerID;

	ULONGLONG	ui64Statitics[41];	// ��ü ���(������ eSTATISTICS_TYPE ����). USERMONEY_ANS���� ������ ������ ä���� �ʴ´�
};

class MSG_MO_RTTG_ABUSING_NTF : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_ABUSING_NTF() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_ABUSING_NTF; }
	~MSG_MO_RTTG_ABUSING_NTF() {}

	enum { MAX_DESC_LENGTH = 1023, MAX_SUBTYPE_NAME_LEN = 64 };

	DWORD64			m_dw64Time;							// time - GetDateTime_YYMMDDHHMM()�� ���� ����Ѵ�
	OLD_SERVER_KEY	m_ServerKey;
	DWORD			m_dwUserGuid;
	DWORD			m_dwCharGuid;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR			m_pszUserID[ID_MAX_LEN + 1];	// AccountID 
#else
    #ifdef __WAVERIX_ABUSE_EXTENDED_FORMAT__
	    TCHAR			m_pszUserID[(ID_MAX_LEN>>1) + 1];	// AccountID 
    #endif
#endif
	TCHAR			m_pCharName[MAX_CHARNAME_LENGTH + 1];
	TCHAR			m_IP[MAX_IP_STRING_LEN+1];

	BYTE			m_AbuseType;
	BYTE			m_Level;
	TCHAR			m_ptszSubTypeName[MAX_SUBTYPE_NAME_LEN+1];

	WORD			m_wLength;
	TCHAR			m_ptszDesc[MAX_DESC_LENGTH+1];

	WORD	GetSize() { return (WORD)( (sizeof(MSG_MO_RTTG_ABUSING_NTF) - sizeof(m_ptszDesc)) + (sizeof(TCHAR)*m_wLength) ); }
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ������ �������Ḧ ��û�Ѵ�.
class MSG_MO_RTTG_SERVERSHUTDOWN_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_SERVERSHUTDOWN_REQ() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_SERVERSHUTDOWN_REQ; }
	~MSG_MO_RTTG_SERVERSHUTDOWN_REQ() {}

	DWORD			dwKey;				//  RTT ������ ����ϴ� �����ʵ�
//	SERVER_KEY		m_ServerKey;		// ����Ű
	BYTE			m_byWorldID;
	BYTE			m_byChannelID;
	BYTE			m_byServerType;
	BYTE			m_byServerID;
	__int64			i64WopsKey;			// WOPS��
//	DWORD			dwWorldID;			//  0x00 = ��ü World(dwChannelID ����)
//	DWORD			dwChannelID;		//  0x00 = ��ü Channel
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTTA
//  Description : ���� �������� ����� �뺸�Ѵ�.
class MSG_MO_RTTG_SERVERSHUTDOWN_ANS : public MSG_BASE_INTERNAL
{
public:
		MSG_MO_RTTG_SERVERSHUTDOWN_ANS() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_SERVERSHUTDOWN_ANS; }
		~MSG_MO_RTTG_SERVERSHUTDOWN_ANS() {}

		DWORD			dwKey;				//  RTT ������ ����ϴ� �����ʵ�
		DWORD			dwError;			//  0 : �Ϸ�, 1 : �����߻�
		OLD_SERVER_KEY	m_ServerKey;		//  ���� �߻��� ���, Server Key Ȯ��
		__int64			i64WopsKey;			// WOPS��
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA �� General
//  Description : ��ġ ����� �뺸�� ��û�Ѵ�.
class MSG_MO_RTTG_PATCHRESULT_REQ : public MSG_BASE_INTERNAL
{
public:
   MSG_MO_RTTG_PATCHRESULT_REQ() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_PATCHRESULT_REQ; }
   ~MSG_MO_RTTG_PATCHRESULT_REQ() {} 

   DWORD   dwKey;   //  RTT ������ ����ϴ� �����ʵ�
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General �� RTTA
//  Description : ��ġ ����� �뺸�Ѵ�.
class MSG_MO_RTTG_PATCHRESULT_ANS : public MSG_BASE_INTERNAL
{
public:
 static const DWORD  ERROR_PATCHSUCCESS  = 0x00;  //  ��ġ ����
 static  const DWORD  ERROR_PROCESSRUN  = 0x01;  //  �ش� ���μ��� ������  
 static const DWORD  ERROR_FTPCONNECT_FAIL = 0x02;  //  ftp ���� ����
 static const DWORD  ERROR_DOWNLOAD_FAIL  = 0x03;  //  ���� �ޱ� ����
 static const DWORD  ERROR_FILENOTFOUND  = 0x04;  //  ��ġ���� ã�� ����

 

public:
   MSG_MO_RTTG_PATCHRESULT_ANS() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_PATCHRESULT_ANS; }
   ~MSG_MO_RTTG_PATCHRESULT_ANS() {}

   DWORD  dwKey;	//  RTT ������ ����ϴ� �����ʵ�
   DWORD  dwError;	//  0 : ����, 1 �̻� : ������ Ȯ��
   BYTE   byVer1;	//  ���μ����� ����
   BYTE   byVer2;
   BYTE   byVer3;
};

class MSG_MO_RTTG_ALL_SERVER_STATUS_NTF : public MSG_BASE_INTERNAL
{
public:
	enum { MAX_SERVER_COUNT = 100 };
	struct ServerConnectInfo
	{
		OLD_SERVER_KEY	m_ServerKey;
		BYTE			m_bConnected;
	};

	MSG_MO_RTTG_ALL_SERVER_STATUS_NTF() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_ALL_SERVER_STATUS_NTF; }
	~MSG_MO_RTTG_ALL_SERVER_STATUS_NTF() {}

	BYTE				m_Count;
	ServerConnectInfo	m_ServerConnectInfo[MAX_SERVER_COUNT];

	int GetSize()
	{
		return sizeof(MSG_MO_RTTG_ALL_SERVER_STATUS_NTF) - (sizeof(ServerConnectInfo))*(MAX_SERVER_COUNT-m_Count); 
	}
};
 

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : AuthServer �� RTT
//  Description : �� ä���� ���� ���� �����Ѵ�.
class MSG_MO_RTTG_USERCOUNT_NTF : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERCOUNT_NTF()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_USERCOUNT_NTF;
	}
	~MSG_MO_RTTG_USERCOUNT_NTF()
	{
	}

    DWORD   dwWorldID;              // World ��ȣ
    DWORD   dwChannelID;            // Channel ��ȣ
    DWORD   dwVillageCount;         // ������(ĳ���ͼ���+����+�ʵ�)
    DWORD   dwVillagePCRoomCount;	// ������(PC��)
    DWORD   dwBattleZoneCount;      // ������(��Ʋ��)
    DWORD   dwBattleZonePCRoomCount;// ������(PC��)
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    DWORD   numberOfInVillageHangame;       // Village    | Default | Hangame
    DWORD   numberOfInVillageHangamePC;     // Village    | PC      | Hangame
    DWORD   numberOfInBattleZoneHangame;    // BattleZone | Default | Hangame
    DWORD   numberOfInBattleZoneHangamePC;  // BattleZone | PC      | Hangame
#endif
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
  #if !defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
    DWORD   numberOfInVillageNeowiz;       // Village    | Default | Neowiz
    DWORD   numberOfInVillageNeowizPC;     // Village    | PC      | Neowiz
    DWORD   numberOfInBattleZoneNeowiz;    // BattleZone | Default | Neowiz
    DWORD   numberOfInBattleZoneNeowizPC;  // BattleZone | PC      | Neowiz
#endif
};

// ���� ���� ���� ���� ��û
class MSG_MO_RTTG_USERDISCONNECT_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERDISCONNECT_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_USERDISCONNECT_REQ;
	}
	~MSG_MO_RTTG_USERDISCONNECT_REQ()
	{
	}

	DWORD			dwUserGUID;
};

class MSG_MO_RTTG_USERDISCONNECT_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERDISCONNECT_ANS()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_USERDISCONNECT_ANS;
	}
	~MSG_MO_RTTG_USERDISCONNECT_ANS()
	{
	}

	DWORD			dwCharGUID;
};

// ������� ���̺� �ٽ� �о Game�����鿡 Broadcast
class MSG_MO_RTTG_FUNCRESTRICT_RELOAD_CMD : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_FUNCRESTRICT_RELOAD_CMD()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_FUNCRESTRICT_RELOAD_CMD;
	}
	~MSG_MO_RTTG_FUNCRESTRICT_RELOAD_CMD()
	{
	}
};

class MSG_MO_RTTG_WAREHOUSEMONEY_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_WAREHOUSEMONEY_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_WAREHOUSEMONEY_REQ;
	}
	~MSG_MO_RTTG_WAREHOUSEMONEY_REQ()
	{
	}
};

class MSG_MO_RTTG_WAREHOUSEMONEY_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_WAREHOUSEMONEY_ANS()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_WAREHOUSEMONEY_ANS;
	}
	~MSG_MO_RTTG_WAREHOUSEMONEY_ANS()
	{
	}

	BYTE		byWorldID;
	BYTE		byChannelID;
	BYTE		byServerType;
	BYTE		byServerID;

	ULONGLONG	ui64AveMoney;			// â�� ��� �ݾ�
	ULONGLONG	ui64TotalPlusMoney;		// â�� �� ��
	ULONGLONG	ui64TotalMinusMoney;	// â���� ���� ��
};


//{__NA001254_20090109_RTTG_FUNC_ADD

// ���� ���� ��û
class MSG_MO_RTTG_CHANGE_GUILDNAME_REQ : public MSG_BASE_INTERNAL
{
public:

	enum { MAX_DESCRIPTION_LENGTH = 512 };

	MSG_MO_RTTG_CHANGE_GUILDNAME_REQ() 
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_CHANGE_GUILDNAME_REQ;
	}
	~MSG_MO_RTTG_CHANGE_GUILDNAME_REQ()
	{
	}
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    SERVER_KEY GetServerKey() { return SERVER_KEY(m_byWorldID,0,GAME_DBPROXY,1); }
#else
	SERVER_KEY GetServerKey() { return SERVER_KEY(m_byWorldID,0,GUILD_SERVER,1); }
#endif

	DWORD		m_dwWopsKey;									// ��� �ĺ���(ANS�ÿ� �״�� �ٽ� �����ֽø� �˴ϴ�)
//	SERVER_KEY	dwServerGuid;									// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
	BYTE		m_byChannelID;								
	BYTE		m_byServerType;
	BYTE		m_byServerID;

	BYTE		m_byWorldID;

	DWORD		m_dwGuildGuid;								// ��� GUID
	TCHAR		m_szGuildName[MAX_GUILDNAME_LENGTH+1];		// �����ϰ��� �ϴ� ����
	TCHAR		m_szDescription[MAX_DESCRIPTION_LENGTH+1];	// ���� ����
};


// ���� ���� ��û ���
class MSG_MO_RTTG_CHANGE_GUILDNAME_ANS : public MSG_BASE_INTERNAL
{
public:

	enum { MAX_RESULT_DESCRIPTION_LENGTH = 200, CHANGE_SUCCESS = 0, CHANGE_FAIL = 1, };

	MSG_MO_RTTG_CHANGE_GUILDNAME_ANS()
		: m_dwWopsKey(0), m_dwResult(0)
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_CHANGE_GUILDNAME_ANS;

		ZeroMemory(	m_szResultDescription, sizeof(m_szResultDescription) );
	}
	~MSG_MO_RTTG_CHANGE_GUILDNAME_ANS()
	{
	}

	DWORD		m_dwWopsKey;												// ��� �ĺ���
	DWORD		m_dwResult;												// 0 : ���� ����, 1 : ���� ����
	TCHAR		m_szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH+1];	// ó�� ����� ���� Description
};


// �������� ���� ����
class MSG_MO_RTTG_SEND_MESSAGE_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_SEND_MESSAGE_REQ()
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_SEND_MESSAGE_REQ;
	}
	~MSG_MO_RTTG_SEND_MESSAGE_REQ()
	{
	}

	SERVER_KEY GetServerKey() { return SERVER_KEY(m_byWorldID,0,WORLD_SERVER,1); }

	DWORD			m_dwWopsKey;								// ��� �ĺ���
	BYTE			m_byChannelID;								// ���� GUID(����ü�� ������ ID�� ���� ������ �ʵ�� ��� 0)
	BYTE			m_byServerType;
	BYTE			m_byServerID;
	BYTE			m_byWorldID;								

	MMEMO_INFOS		m_MMemoInfo;
};


class MSG_MO_RTTG_SEND_MESSAGE_ANS : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_SEND_MESSAGE_ANS()
		: m_dwWopsKey(0), m_dwCount(0)
	{
		m_byCategory = MO_OPERATION;
		m_byProtocol = MO_RTTG_SEND_MESSAGE_ANS;
	}
	~MSG_MO_RTTG_SEND_MESSAGE_ANS()
	{
	}

	DWORD			m_dwWopsKey;							// ��� �ĺ���
	DWORD			m_dwCount;							// ���� ���� ��� ����
	MMEMO_ERR_RESULT	m_arrResultInfoList[MMEMO_ERR_RESULT::MAX_RESULT_INFO];	// ���� ���� ��� ������
};

//}__NA001254_20090109_RTTG_FUNC_ADD

#pragma pack(pop)
