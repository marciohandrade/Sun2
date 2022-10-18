#pragma once

#include <Protocol_MO.h>
#include <ServerStruct.h>
#include <PacketStruct.h>

#pragma pack(push,1)

/*
// 마스터 서버에 특정 서버의 데이터 리로드 요청
struct MSG_MO_RELOAD_DATA_SYN : public MSG_BASE_INTERNAL
{
	BYTE	byWorldID;
	BYTE	byChannelID;
	BYTE	byServerType;
	BYTE	byServerID;
};
*/

// SERVER_KEY 내용 바뀌면서 바이트 뒤집어져서 임시로 수정 - 장기적으로는 구조체로 보내지 않는 편이 좋겠다(성현창)
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

// OLD_SERVER_KEY 대체용
#define SWAP_32BIT_DATATYPE(dwOldSK)	\
	(( ((dwOldSK)&0x000000FF) << 24) | (((dwOldSK)&0x0000FF00) << 8) | (((dwOldSK)&0x00FF0000) >> 8) | (((dwOldSK)&0xFF000000) >> 24))



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? → RTT
//  Description : RTT와 통신하는 프로그램은 RTT와 접속이 이루어진 후에 인증을 받아야한다.
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

	DWORD			dwType;						//  AGENTTYPE_ define을 명시한다.
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : ? → RTT
//  Description : RTT와 통신하는 프로그램은 RTT와 접속이 이루어진 후에 인증을 받아야한다.
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

	DWORD			dwResult;					//  0 : 인증 성공
	DWORD			dwType;						//  SERVERTYPE_ define을 명시한다.
	DWORD			dwUID;						//  인증된 고유번호
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS → RTT → General
//  Description : 게임내 공지를 발송한다.
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

	DWORD				dwServerUID;				//  공지를 전송할 서버의 uid.(0=전체)
	DWORD				dwTickPerLine;				//  1라인 출력 시간간격(ms)
	DWORD				dwNoticeLength;				//  공지 문자열의 길이
	char				szNotice[MAX_NOTICE_LEN+1];	//  공지 문자열
	BYTE				m_byRed;
	BYTE				m_byGreen;
	BYTE				m_byBlue;

};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : GMS, General → RTT
//  Description : 게임내 관리자 계정 목록을 요청한다.
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
//  Redirection : RTT → GMS, General
//  Description : 관리자 계정 목록을 전송한다.
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

	DWORD				dwCount;				//  계정 정보의 개수
	//  이하 다음 데이터들이 dwCount 이후로 dwCount만큼 반복
	struct sGM_INFO_APPENDED
	{
		char			szAccount[ID_MAX_LEN];
		char			szIP[IP_MAX_LEN];
		DWORD			dwAttr;
	};

	inline sGM_INFO_APPENDED*	GetAppendedDataPtr() { return (sGM_INFO_APPENDED*)((&dwCount)+1); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 개설된 배틀존의 정보를 요청한다.
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
//  Redirection : General → RTTA
//  Description : 개설된 배틀존의 정보를 보낸다.
struct MSG_MO_RTTG_BATTLEZONEINFO_ANS : public MSG_BASE_INTERNAL
{
	enum { MAX_CHANNEL_NUM = 256, };

	MSG_MO_RTTG_BATTLEZONEINFO_ANS() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_BATTLEZONEINFO_ANS; }

	BYTE					byCount;						// 채널 개수
	STRUCT_BATTLEZONE_INFO	ChannelInfo[MAX_CHANNEL_NUM];	// 채널당 배틀존 정보

	int GetSize() { return ( sizeof(MSG_MO_RTTG_BATTLEZONEINFO_ANS) - ( sizeof(STRUCT_BATTLEZONE_INFO) * ( MAX_CHANNEL_NUM - byCount ) ) ); }
};

class MSG_MO_ARMORKIT_FINDHACK_NTF : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_ARMORKIT_FINDHACK_NTF() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_ARMORKIT_FINDHACK_NTF; }
	~MSG_MO_ARMORKIT_FINDHACK_NTF() { }

	DWORD	dwGuid;			// 해킹이 감지된 클라이언트 guid
	TCHAR	szIP[16];		// 해킹이 감지된 클라이언트 IP주소
	int		HackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
	int		Size;			// Buffer 의 길이
	TCHAR	Buf[1024];		// 데이터 버퍼-1024는 보안파트에서 전달받은 암묵적 최대값(성현창)

	int GetSize()	{ return sizeof(MSG_MO_ARMORKIT_FINDHACK_NTF); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → Master
//  Description : 서버별 통화량 정보 요청
class MSG_MO_RTTG_USERMONEY_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_USERMONEY_REQ() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_USERMONEY_REQ; }
	~MSG_MO_RTTG_USERMONEY_REQ() { }

	int GetSize()	{ return sizeof(MSG_MO_RTTG_USERMONEY_REQ); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master → RTTA
//  Description : 서버별 통화량 정보 응답
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

	ULONGLONG	ui64TotalGetMoney;		// 일정 시간 동안 생겨난 돈 총계
	ULONGLONG	ui64TotalLoseMoney;		// 일정 시간 동안 없어진 돈 총계
	ULONGLONG	ui64TotalUserMoney;		// 현재 동접 유저들이 갖고 있는 평균 금액

	ULONGLONG	ui64WarehouseAveMoney;		// 창고 평균 금액
	ULONGLONG	ui64WarehousePlusMoney;		// 창고에 들어간 돈
	ULONGLONG	ui64WarehouseMinusMoney;	// 창고에서 나간 돈

	USHORT		usJackpotCnt;				// 잭팟 터진 횟수
	ULONGLONG	JackpotMoney;				// 잭팟 금액 합계

	ULONGLONG	CirculateMoney;				// 상거래 금액 합계
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → Master
//  Description : 서버별 기타 통계 정보 요청
class MSG_MO_RTTG_GAMESTATISTICS_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_GAMESTATISTICS_REQ() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_GAMESTATISTICS_REQ; }
	~MSG_MO_RTTG_GAMESTATISTICS_REQ() { }

	int GetSize()	{ return sizeof(MSG_MO_RTTG_GAMESTATISTICS_REQ); }
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : Master → RTTA
//  Description : 서버별 통화량 정보 응답
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

	ULONGLONG	ui64Statitics[41];	// 전체 통계(순서는 eSTATISTICS_TYPE 참조). USERMONEY_ANS에서 보내는 내용은 채우지 않는다
};

class MSG_MO_RTTG_ABUSING_NTF : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_ABUSING_NTF() { m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_ABUSING_NTF; }
	~MSG_MO_RTTG_ABUSING_NTF() {}

	enum { MAX_DESC_LENGTH = 1023, MAX_SUBTYPE_NAME_LEN = 64 };

	DWORD64			m_dw64Time;							// time - GetDateTime_YYMMDDHHMM()의 값을 사용한다
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
//  Redirection : RTTA → General
//  Description : 서버의 정상종료를 요청한다.
class MSG_MO_RTTG_SERVERSHUTDOWN_REQ : public MSG_BASE_INTERNAL
{
public:
	MSG_MO_RTTG_SERVERSHUTDOWN_REQ() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_SERVERSHUTDOWN_REQ; }
	~MSG_MO_RTTG_SERVERSHUTDOWN_REQ() {}

	DWORD			dwKey;				//  RTT 내에서 사용하는 전용필드
//	SERVER_KEY		m_ServerKey;		// 서버키
	BYTE			m_byWorldID;
	BYTE			m_byChannelID;
	BYTE			m_byServerType;
	BYTE			m_byServerID;
	__int64			i64WopsKey;			// WOPS용
//	DWORD			dwWorldID;			//  0x00 = 전체 World(dwChannelID 무시)
//	DWORD			dwChannelID;		//  0x00 = 전체 Channel
};



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTTA
//  Description : 서버 정상종료 결과를 통보한다.
class MSG_MO_RTTG_SERVERSHUTDOWN_ANS : public MSG_BASE_INTERNAL
{
public:
		MSG_MO_RTTG_SERVERSHUTDOWN_ANS() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_SERVERSHUTDOWN_ANS; }
		~MSG_MO_RTTG_SERVERSHUTDOWN_ANS() {}

		DWORD			dwKey;				//  RTT 내에서 사용하는 전용필드
		DWORD			dwError;			//  0 : 완료, 1 : 에러발생
		OLD_SERVER_KEY	m_ServerKey;		//  에러 발생의 경우, Server Key 확인
		__int64			i64WopsKey;			// WOPS용
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : RTTA → General
//  Description : 패치 결과의 통보를 요청한다.
class MSG_MO_RTTG_PATCHRESULT_REQ : public MSG_BASE_INTERNAL
{
public:
   MSG_MO_RTTG_PATCHRESULT_REQ() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_PATCHRESULT_REQ; }
   ~MSG_MO_RTTG_PATCHRESULT_REQ() {} 

   DWORD   dwKey;   //  RTT 내에서 사용하는 전용필드
};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//  Redirection : General → RTTA
//  Description : 패치 결과를 통보한다.
class MSG_MO_RTTG_PATCHRESULT_ANS : public MSG_BASE_INTERNAL
{
public:
 static const DWORD  ERROR_PATCHSUCCESS  = 0x00;  //  패치 성공
 static  const DWORD  ERROR_PROCESSRUN  = 0x01;  //  해당 프로세스 실행중  
 static const DWORD  ERROR_FTPCONNECT_FAIL = 0x02;  //  ftp 연결 오류
 static const DWORD  ERROR_DOWNLOAD_FAIL  = 0x03;  //  파일 받기 오류
 static const DWORD  ERROR_FILENOTFOUND  = 0x04;  //  패치파일 찾기 오류

 

public:
   MSG_MO_RTTG_PATCHRESULT_ANS() {	m_byCategory = MO_OPERATION; m_byProtocol = MO_RTTG_PATCHRESULT_ANS; }
   ~MSG_MO_RTTG_PATCHRESULT_ANS() {}

   DWORD  dwKey;	//  RTT 내에서 사용하는 전용필드
   DWORD  dwError;	//  0 : 성공, 1 이상 : 에러값 확인
   BYTE   byVer1;	//  프로세스의 버젼
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
//  Redirection : AuthServer → RTT
//  Description : 각 채널의 동접 수를 통지한다.
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

    DWORD   dwWorldID;              // World 번호
    DWORD   dwChannelID;            // Channel 번호
    DWORD   dwVillageCount;         // 동접수(캐릭터선택+마을+필드)
    DWORD   dwVillagePCRoomCount;	// 동접수(PC방)
    DWORD   dwBattleZoneCount;      // 동접수(배틀존)
    DWORD   dwBattleZonePCRoomCount;// 동접수(PC방)
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

// 유저 접속 강제 종료 요청
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

// 기능제한 테이블 다시 읽어서 Game서버들에 Broadcast
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

	ULONGLONG	ui64AveMoney;			// 창고 평균 금액
	ULONGLONG	ui64TotalPlusMoney;		// 창고에 들어간 돈
	ULONGLONG	ui64TotalMinusMoney;	// 창고에서 나간 돈
};


//{__NA001254_20090109_RTTG_FUNC_ADD

// 길드명 변경 요청
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

	DWORD		m_dwWopsKey;									// 명령 식별자(ANS시에 그대로 다시 보내주시면 됩니다)
//	SERVER_KEY	dwServerGuid;									// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
	BYTE		m_byChannelID;								
	BYTE		m_byServerType;
	BYTE		m_byServerID;

	BYTE		m_byWorldID;

	DWORD		m_dwGuildGuid;								// 길드 GUID
	TCHAR		m_szGuildName[MAX_GUILDNAME_LENGTH+1];		// 변경하고자 하는 길드명
	TCHAR		m_szDescription[MAX_DESCRIPTION_LENGTH+1];	// 변경 사유
};


// 길드명 변경 요청 결과
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

	DWORD		m_dwWopsKey;												// 명령 식별자
	DWORD		m_dwResult;												// 0 : 변경 성공, 1 : 변경 실패
	TCHAR		m_szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH+1];	// 처리 결과에 대한 Description
};


// 유저에게 쪽지 전송
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

	DWORD			m_dwWopsKey;								// 명령 식별자
	BYTE			m_byChannelID;								// 서버 GUID(구조체에 월드의 ID만 적고 나머지 필드는 모두 0)
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

	DWORD			m_dwWopsKey;							// 명령 식별자
	DWORD			m_dwCount;							// 쪽지 전송 결과 갯수
	MMEMO_ERR_RESULT	m_arrResultInfoList[MMEMO_ERR_RESULT::MAX_RESULT_INFO];	// 쪽지 전송 결과 정보들
};

//}__NA001254_20090109_RTTG_FUNC_ADD

#pragma pack(pop)
