#pragma once

#include <ServerStructInPacket.h>
#include <ServerStruct.h>
#include <PacketStruct.h>

#include <Protocol_DW.h>

#pragma pack(push,1)

#define PACKET_DW_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DW_CLOSE		};

//////////////////////////////////
// 접속 관련 프로토콜
PACKET_DW_OPEN(DW_CONNECTION, DW_CONNECTION_SERVER_INFO_CMD)
	SERVER_KEY		m_ServerKey;
	DWORD			m_dwServerGUID;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN];
	WORD			m_wDBProxyServerPort;
PACKET_DW_CLOSE;

///////////////////////////////////
// 친구 관련 프로토콜

// 친구 목록 요청
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_SYN)
	CHARGUID		m_CharGuid;								//< 친구 목록을 요청하는 캐릭터 guid

    uint16_t GetSize() const { return sizeof(*this); }
PACKET_DW_CLOSE;


// 리팩토링 버전
//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
PACKET_DW_OPEN(DW_COMMUNITY, DW_COMMUNITY_MYLIST_SYN)
	CHARGUID	m_CharGuid;
    uint16_t GetSize() const { return sizeof(*this); }
PACKET_DW_CLOSE;

PACKET_DW_OPEN(DW_COMMUNITY, DW_COMMUNITY_MYLIST_ACK)
	WORD dataNum;
	CHARGUID	 charGuid;
	COMMUNITY_DB_INFO communityInfo[MAX_FRIENDBLOCK_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(communityInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(communityInfo[0]) * dataNum);
    }
PACKET_DW_CLOSE;

PACKET_DW_OPEN(DW_COMMUNITY, DW_COMMUNITY_CONTACT_ME_ACK)
	WORD m_DataNum;
	CHARGUID m_CharGuid;
	CHARGUID m_CotactMEList[MAX_FRIENDBLOCK_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_CotactMEList);
        return static_cast<uint16_t>(kMinSize + sizeof(m_CotactMEList[0]) * m_DataNum);
    }
PACKET_DW_CLOSE;

PACKET_DW_OPEN(DW_COMMUNITY, DW_COMMUNITY_MYLIST_NAK)
	enum {ERR_DEFAULT,};
	BYTE m_byResult;
	CHARGUID m_CharGuid;
PACKET_DW_CLOSE;
//#endif//_NA_0_20080612_WORLDSERVER_REFACTORING

// 구버전
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_ACK)
    // CHANGES: f110104.1L, fixed a data type for counting to solve type overflow.
    CHARGUID m_CharGuid; //< 친구 목록을 받을 캐릭터의 guid
    uint16_t number_of_friends; //< 친구 숫자
    STRUCT_FRIEND_DB_INFO m_sFriendInfo[MAX_FRIENDBLOCK_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sFriendInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sFriendInfo[0]) * number_of_friends);
    }
PACKET_DW_CLOSE;

// 친구 목록 수신 실패
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_NAK)
	enum
	{
		ERR_DEFAULT,
//		NO_FRIEND,		//< 친구 없~다~
	};

	CHARGUID				m_CharGuid;
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// 친구 요청
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_SYN)
	CHARGUID				m_CharGuidFrom;								//< 친구 신청한 쪽의 guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];		//< 친구 신청을 받은 쪽의 캐릭터명
PACKET_DW_CLOSE;

// 친구 요청 성공
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_ACK)
	CHARGUID				m_CharGuidFrom;								//< 친구 신청한 쪽의 guid
	STRUCT_FRIEND_INFO		m_ToFriendInfo;								//< 친구 신청을 받은 쪽의 캐릭터정보
PACKET_DW_CLOSE;

// 친구 요청 실패
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_NAK)
	enum
	{
		ERR_DEFAULT		= 0,		//< 기타 등등
		NO_USER			= 1,		//< 해당 GUID의 유저가 없음
		ALREADY_FRIEND	= 2,		//< 이미 친구 상태
		BLOCKED_USER	,			//< 차단 상태
		
	};

	CHARGUID				m_CharGuidFrom;
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// 친구 삭제 요청
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_SYN)
	CHARGUID		m_CharGuidFrom;	//< 삭제를 요청한 쪽
	CHARGUID		m_CharGuidTo;	//< 삭제되는 쪽
	BYTE			m_Sts;			//< 친구 관계 코드, STRUCT_FRIEND_DB_INFO::eNodeType, _KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_DW_CLOSE;

// 친구 삭제 성공
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_ACK)
	CHARGUID		m_CharGuidFrom;	//< 삭제를 요청한 쪽
	CHARGUID		m_CharGuidTo;	//< 삭제되는 쪽
PACKET_DW_CLOSE;

// 친구 삭제 실패
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_NAK)
	enum
	{
		ERR_DEFAULT,			//< 알 수 없는 이유
		NOT_FRIEND_USER,		//< 친구 아님
	};
	CHARGUID				m_CharGuidFrom;			//< 삭제를 요청한 쪽
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// 차단 요청(친구든 아니든)
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_SYN)
	CHARGUID		m_CharGuidFrom;									//< 차단 요청한 쪽 guid
	TCHAR			m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];			//< 차단 당하는 쪽
PACKET_DW_CLOSE;

// 차단 성공
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_ACK)
	CHARGUID			m_CharGuidFrom;								//< 차단 요청한 쪽 guid
	STRUCT_BLOCK_INFO	m_BlockInfo;
PACKET_DW_CLOSE;

// 차단 실패
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_NAK)
	CHARGUID			m_CharGuidFrom;								//< 차단 요청한 쪽 guid
	BYTE				m_byResult;
PACKET_DW_CLOSE;

// 차단 삭제 요청
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_SYN)
	CHARGUID		m_CharGuidFrom;				//< 차단 삭제 요청한 쪽 guid
	CHARGUID		m_CharGuidTo;				//< 삭제되는 쪽
PACKET_DW_CLOSE;

// 차단 삭제 성공
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_ACK)
	CHARGUID		m_CharGuidFrom;				//< 차단 삭제 요청한 쪽 guid
	CHARGUID		m_CharGuidTo;				//< 삭제되는 쪽
PACKET_DW_CLOSE;

// 차단 삭제 실패
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_NAK)
	enum
	{
		ERR_DEFAULT		=0,		//< 알 수 없는 이유
		NO_USER			=1,		//< 존재하지 않는 캐릭터 이름
		NOT_BLOCKED,			//< 차단된 캐릭터가 아님
	};

	CHARGUID		m_CharGuidFrom;									//< 차단 삭제 요청한 쪽 guid
	BYTE			m_byResult;
PACKET_DW_CLOSE;

//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_INVITATIONRELATION_FREE_CMD )
    CHARGUID		m_CharGuid;									// 추천당한 캐릭의 GUID
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD )
    CHARGUID		m_SendGuid;
    CHARGUID		m_RevGuid;
    WORD            m_Level;
    TCHAR			m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
    TCHAR			m_ptszMemo[MAX_MEMO_LENGTH + 1];
PACKET_DW_CLOSE

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_DETAIL_INFO_SYN )
    CHARGUID		m_dwToCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_DETAIL_INFO_ACK )
    CHARGUID        m_dwToCharGuid;
    TCHAR		    m_ptszToCharName[MAX_CHARNAME_LENGTH+1];
    TCHAR		    m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
    TCHAR		    m_ptszToCharGuildName[MAX_GUILDNAME_LENGTH+1];
    BYTE		    m_byToCharGuildDuty;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_DETAIL_INFO_NAK )
    BYTE		    m_byErrorCode;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_MEMO_UPDATE_SYN )
    CHARGUID	    m_dwToCharGuid;
    TCHAR		    m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_MEMO_UPDATE_ACK )
    CHARGUID	    m_dwToCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_MEMO_UPDATE_NAK )
    BYTE		    m_byErrorCode;
PACKET_DW_CLOSE


//////////////////////////////////
// GM 관련 프로토콜

// 채팅금지
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_SYN )
	TCHAR		m_ptszGMAccountID[ID_MAX_LEN+1];	// 차단 명령한 GM 계정명
	CHARGUID	m_CharGuid;		// 차단 대상 사용자
	DWORD		m_dwBlockTIme;	// 채팅 금지 시간(분단위)
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_ACK )
	CHARGUID		m_CharGuid;
	DWORD			m_dwBlockTime;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

// 채팅허용
PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_SYN )
	TCHAR			m_ptszGMAccountID[ID_MAX_LEN+1];	// 허용 명령한 GM 계정명
	CHARGUID		m_CharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_ACK )
	CHARGUID		m_CharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

// 채팅금지 시간 차감
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_SYN )
	DWORD			m_dwRemainTime;		// 차감되고 남은 시간
PACKET_DW_CLOSE

//PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_ACK )
//PACKET_DW_CLOSE
	
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_REQ )
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_ANS )
	DWORD			m_dwRemainTime;		// 남은 시간(0이면 Block상태 아님)
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE


//////////////////////////////////
// 무단 PK 관련 프로토콜

// 리벤지 리스트에 등록 요청
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< 등록 요청한 쪽의 guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< 등록할 캐릭터명
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< 등록 요청한 쪽의 guid
	STRUCT_REVENGE_INFO		m_sRevengeUserInfo;							//< 등록한 캐릭터 정보
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< 등록 요청한 쪽의 guid
	BYTE					m_byErrorCode;								//< RC::eREVENGE_RESULT 참고
PACKET_DW_CLOSE

// 리벤지 리스트에서 삭제 요청(C->W->D)
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< 삭제 요청한 쪽의 guid
	CHARGUID				m_RevengeGuidTo;							//< 삭제할 캐릭터 Guid
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< 삭제 요청한 쪽의 guid
	CHARGUID				m_RevengeGuidTo;							//< 삭제한 캐릭터 Guid
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< 등록 요청한 쪽의 guid
	BYTE					m_byErrorCode;								//< RC:eREVENGE_RESULT 참고
PACKET_DW_CLOSE

// 리벤지 성공하여 리스트에서 삭제 요청(G->A->W->D)
// -- 이 패킷 구조체들은 기본적으로 DW_CHAO_REMOVE_REVENGE_CHAR...와 완전히 동일하다. 단지 WorldServer의 행동이 다르기 때문에 타입 구분
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN		MSG_DW_CHAO_PK_REVENGE_CHAR_SYN;
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK		MSG_DW_CHAO_PK_REVENGE_CHAR_ACK;
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK		MSG_DW_CHAO_PK_REVENGE_CHAR_NAK;

/*
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< 삭제 요청한 쪽의 guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< 삭제할 캐릭터명
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< 삭제 요청한 쪽의 guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< 삭제한 캐릭터명
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< 등록 요청한 쪽의 guid
	BYTE					m_byErrorCode;								//< RC:eREVENGE_RESULT 참고
PACKET_DW_CLOSE
*/

//////////////////////////////////
// 쪽지 관련 프로토콜
// 쪽지 전송
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_SYN )
	CHARGUID		    m_SendGuid;
	TCHAR			    m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	TCHAR			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    CHARGUID            m_recvGuid;//보상 받을 Guid
    SLOTCODE            m_recvItemCode;//보상 받을 아이템 코드
    ULONGLONG           m_amountCost;//보상 받을 금액
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//시스템 메모 구분자
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_ACK )
	CHARGUID		    m_SendGuid;
	CHARGUID		    m_RecvGuid;
	TCHAR			    m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	STRUCT_SUBJECT_PACKETINFO	m_Subject;
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//환급 받을 아이템 코드
    ULONGLONG           m_amountCost;//환급 받을 금액
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//시스템 메모 구분자
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_NAK )
	CHARGUID		m_SendGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// 쪽지 목록 확인
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_LIST_REQ )
	CHARGUID		m_RecvGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_LIST_ANS )
	CHARGUID		m_RecvGuid;
	BYTE			m_byMemoCnt;				// 쪽지 개수(현재 페이지에서)
	BYTE			m_byMemoPage;				// 쪽지 목록의 현재 페이지(10개 단위로 전송)
	BYTE			m_byMemoPageCnt;			// 쪽지 목록의 총 페이지(현재 쪽지 123개면 7페이지가 됨. m_byMemoPage=m_byMemoPageCnt면 송신 완료)
	STRUCT_SUBJECT_PACKETINFO	m_Memo[MAX_MEMO_PAGENUM];
	WORD		GetSize()
	{
		return ( sizeof(MSG_DW_MEMO_LIST_ANS) - ( sizeof(STRUCT_SUBJECT_PACKETINFO) * (MAX_MEMO_PAGENUM - m_byMemoCnt) ) );
	}
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_LIST_NAK )
	CHARGUID		m_RecvGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// 쪽지 내용 확인
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_REQ )
	CHARGUID		m_RecvGuid;
	MEMOGUID		m_MemoGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_ANS )
	CHARGUID		    m_RecvGuid;
	MEMOGUID		    m_MemoGuid;
	TCHAR			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//환급 받을 아이템 코드
    ULONGLONG           m_amountCost;//환급 받을 금액
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//시스템 메모 구분자
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_NAK )
	CHARGUID		m_RecvGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// 쪽지 삭제
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_DELETE_SYN )
	CHARGUID		m_RecvGuid;
	MEMOGUID		m_MemoGuid;
    STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE m_MemoType; //_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_DELETE_ACK )
	CHARGUID		m_RecvGuid;
	MEMOGUID		m_MemoGuid;
    STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE m_MemoType; //_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_DELETE_NAK )
	CHARGUID		m_RecvGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_STATUS, DW_STATUS_INTERCEPTION_REQ )
	CHARGUID		m_CharGuid;	
PACKET_DW_CLOSE 

PACKET_DW_OPEN( DW_STATUS, DW_STATUS_INTERCEPTION_ANS )
	CHARGUID		m_CharGuid;
	BOOL			m_bWhisperFlag;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_STATUS, DW_STATUS_INTERCEPTION_NAK )
	CHARGUID		m_CharGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// 길드 로그 확인
PACKET_DW_OPEN(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_REQ)
	GUILDGUID m_GuildGuid;
	CHARGUID		m_ReqCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_ANS)
	GUILDGUID m_GuildGuid;
	CHARGUID	 m_ReqCharGuid;
	BYTE	m_byLogCount;	 // 로그 개수(현재 페이지에서)
	BYTE	m_byCurrentPage; // 전송 되는 현재 페이지
	WORD m_byLogPageCount;	// 로그 목록의 총 페이지(현재 로그 123개면 7페이지가 됨. m_byCurrentPage=m_byLogPageCount면 송신 완료)
	STRUCT_GUILDWAREHOUSE_LOGINFO m_LogList[MAX_GUILDWAREHOUSE_LOGPAGE_NUM];
	WORD GetSize()
	{
		return (sizeof(MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS) - 
			(sizeof(STRUCT_GUILDWAREHOUSE_LOGINFO) 
		  * (MAX_GUILDWAREHOUSE_LOGPAGE_NUM - m_byLogCount)));
	}
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_NAK)
	CHARGUID	 m_ReqCharGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_SELECT_NOTICE_SYN)
	BYTE m_NoticeType;
	GUILDGUID m_GuildGuid;
	CHARGUID	 m_OperCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_SELECT_NOTICE_ACK)
	BYTE m_GuildNoticeType;
	GUILDGUID m_GuildGuid;
	CHARGUID	 m_OperCharGuid;
	TCHAR m_szGuildNotice[MAX_GUILDNOTICE_LENGTH+1];
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_SELECT_NOTICE_NAK)
	DWORD m_dwErrorCode;
PACKET_DW_CLOSE

//지역 점령전 보상/환불 내역 조회 패킷
//_KR_001385_20091202_DOMINATION_RETURN_HEIM
PACKET_DW_OPEN(DW_GUILD, DW_DOMINATION_FINDMONEY_SYN)
    eSYSTEM_MEMO_TYPE m_CostType;
    MAPCODE m_MapCode;
    CHARGUID m_RecvCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_DOMINATION_FINDMONEY_ACK)
    INT32 m_Result;
    MAPCODE m_DBMapCode;
    MAPCODE m_RequestMapCode;
    eSYSTEM_MEMO_TYPE m_CostType;
    eSYSTEM_MEMO_TYPE m_DBCostType;
    CHARGUID m_RecvCharGuid;
    CHARGUID m_DBCharGuid;
    ULONGLONG m_TenderCost;
PACKET_DW_CLOSE

//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
PACKET_DW_OPEN( DW_COMMUNITY, DW_COMMUNITY_INFO_SYN )
	CHARGUID m_RequestCharGuid;	
PACKET_DW_CLOSE 

PACKET_DW_OPEN( DW_COMMUNITY, DW_COMMUNITY_INFO_ANS )
	CHARGUID	 m_RequestCharGuid;
	BYTE m_RevengePoint;
	BOOL m_bWhisperFlag;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_COMMUNITY, DW_COMMUNITY_INFO_NAK )
	CHARGUID	 m_RequestCharGuid;
PACKET_DW_CLOSE
//#endif //_NA_0_20080612_WORLDSERVER_REFACTORING

PACKET_DW_OPEN( DW_MEMO, DW_MULTIMEMO_SEND_SYN )
MMEMO_INFOS		m_MMemoInfo;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MULTIMEMO_SEND_ACK )
DWORD	m_dwCount;
char	m_SendName[MAX_CHARNAME_LENGTH+1];
MMEMO_RESULT m_arrResultInfoList[MMEMO_ERR_RESULT::MAX_RESULT_INFO];
PACKET_DW_CLOSE

// declared by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { DW_LOTTO~ }
//------------------------------------------------------------------------------
// DW_LOTTO : 하임 로또 카테고리
//------------------------------------------------------------------------------

// 하임 로또 응모 내역 로딩 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_SYN)
    BYTE     bFlag;                         // 클라이언트 전용 플래그
    CHARGUID iCharGuid;                     // 조회할 캐릭터 ID
    BYTE     bNumRecent;                    // 현재 회차 포함 최대 몇 회차의 정보를 가져올 것인지
    BYTE     bInPreLoading;                 // 0이 아니면 클라이언트 요청이 아닌 프리로딩
PACKET_DW_CLOSE

// 하임 로또 응모 내역 로딩 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_ACK)
    BYTE     bFlag;                         // 클라이언트 전용 플래그
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bNumRemainingTickets;          // 남은 복권 용지 개수
    BYTE     bNumUsedTickets;               // 현재 회차에 이미 응모한 복권 용지 개수
    DWORD    dwEffectivePayment;            // 현재 유효 소비 하임 액수
    BYTE     bInPreLoading;                 // 요청에서 지정된 프리로딩 플래그

    static const size_t MAX_ENTRIES = LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS;
    WORD             wNumEntries;           // 총 몇 개의 응모 내역 데이터가 포함되어 있는지
    ServerLottoEntry entries[MAX_ENTRIES];  // 응모 내역

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(entries);
        return static_cast<uint16_t>(kMinSize + sizeof(entries[0]) * wNumEntries);
    }
PACKET_DW_CLOSE

// 하임 로또 정보 로딩 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_SYN)
    BYTE bNumRecent;                        // 현재 회차 포함 최대 몇 회차의 정보를 가져올 것인지
PACKET_DW_CLOSE

// 하임 로또 정보 로딩 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ACK)
    WORD  wCurrentDrawNum;                  // 현재 회차
    INT64 i64TotalAccum;                    // 현재 총 적립액

    static const size_t MAX_RESULTS = LOTTO_NUM_RECENT_DRAWS;
    BYTE              bNumResultRecords;    // 현재 회차 포함 총 몇 회의 로또 결과 데이터가 포함되어 있는지
    ServerLottoResult results[MAX_RESULTS]; // 현재 회차 포함 추첨 결과
PACKET_DW_CLOSE

// 하임 로또 당첨자 목록 로딩 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_WINNER_LIST_SYN)
    WORD  wDrawNum;                         // 회차 번호
    INT32 iPage;                            // 요청할 페이지 번호
PACKET_DW_CLOSE

// 하임 로또 당첨자 목록 로딩 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_WINNER_LIST_ACK)
    WORD  wDrawNum;                         // 회차 번호
    INT32 iPage;                            // 요청된 페이지 번호
    INT32 iMaxPage;                         // 최대 페이지 번호 (페이지의 총 수)
    
    static const size_t MAX_RECORDS = LOTTO_NUM_WINNERS_PER_PAGE;
    INT32             iNumRecords;          // 몇 개의 당첨자 정보가 포함되어 있는지
    ServerLottoWinner winners[MAX_RECORDS]; // 당첨자 목록

    int GetSize() const
    {
        return (sizeof(MSG_DW_LOTTO_WINNER_LIST_ACK) \
            - (sizeof(ServerLottoWinner) * (MAX_RECORDS - iNumRecords)));
    }
PACKET_DW_CLOSE

// 하임 로또 응모 처리 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_SYN)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    WORD     wDrawNum;                      // 응모 회차
    BYTE     bIsAuto;                       // 자동 선택 여부
    BYTE     bStatus;                       // 응모/당첨 상태
    BYTE     bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
PACKET_DW_CLOSE

// 하임 로또 응모 처리 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_ACK)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    WORD     wDrawNum;                      // 응모 회차
    BYTE     bIsAuto;                       // 자동 선택 여부
    BYTE     bStatus;                       // 응모/당첨 상태
    BYTE     bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
    INT32    iResult;                       // 실행 결과 (0:정상, 1:오류)
PACKET_DW_CLOSE

// 하임 로또 추첨 및 이월 처리 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ROLLOVER_SYN)
    WORD  wCurrentDrawNum;                  // 현재 회차
    INT8  iNumWinners;                      // 1등 당첨자 수
    INT64 i64Jackpot;                       // 1등 당첨금
    INT64 i64Prize2;                        // 2등 당첨금
    INT64 i64Prize3;                        // 3등 당첨금
    BYTE  bWinningNum[LOTTO_NUM_PICKS];     // 차회 당첨 번호
PACKET_DW_CLOSE

// 하임 로또 추첨 및 이월 처리 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ROLLOVER_ACK)
    INT32 iResult;                          // 실행 결과 (0:정상, 1:오류)
PACKET_DW_CLOSE

// 하임 로또 총 적립액 갱신 통보
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ACCUM_NTF)
    WORD  wCurrentDrawNum;                  // 회차 번호
    INT64 i64TotalAccum;                    // 총 적립액
PACKET_DW_CLOSE

// 하임 로또 복권 용지 수 증가 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ADD_TICKET_SYN)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bNumRemainingTickets;          // 남은 복권 용지 수
    BYTE     bMaxTickets;                   // 최대로 보유 가능한 복권 용지 수
PACKET_DW_CLOSE

// 하임 로또 복권 용지 수 증가 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ADD_TICKET_ACK)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bNumRemainingTickets;          // 남은 복권 용지 수
    INT32    iResult;                       // 실행 결과 (0:정상, 1:오류)
PACKET_DW_CLOSE

// 하임 로또 당첨금 지급 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_REWARD_SYN)
    DWORD    dwUserKey;                     // DB User Key
    INT64    i64Idx;                        // 응모 건 ID
    WORD     wDrawNum;
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bStatus;
    INT64    i64Prize;
PACKET_DW_CLOSE

// 하임 로또 당첨금 지급 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_REWARD_ACK)
    INT32    iResult;                       // 실행 결과 (0:정상, 1:오류)
    DWORD    dwUserKey;                     // DB User Key
    INT64    i64Idx;                        // 응모 건 ID
    WORD     wDrawNum;
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bStatus;
    INT64    i64Prize;
    TCHAR    szCharName[MAX_CHARNAME_LENGTH + 1]; // 캐릭터명
PACKET_DW_CLOSE

// 하임 로또 당첨안내 쪽지 목록 로딩 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_LIST_SYN)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    BYTE     bMaxCount;                     // 가져올 최근 쪽지의 최대 개수
PACKET_DW_CLOSE

// 하임 로또 당첨안내 쪽지 목록 로딩 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_LIST_ACK)
    CHARGUID iCharGuid;
    
    static const size_t MAX_RECORDS = LOTTO_MAX_NOTICE;
    INT               iNumRecords;          // 몇 개의 쪽지 항목이 포함되어 있는지 ( <=LOTTO_MAX_NOTICE )
    LottoNoticeRecord notices[MAX_RECORDS]; // 당첨 안내 쪽지 목록

    int GetSize() const
    {
        return (sizeof(MSG_DW_LOTTO_NOTICE_LIST_ACK) \
            - (sizeof(LottoNoticeRecord) * (MAX_RECORDS - iNumRecords)));
    }
PACKET_DW_CLOSE

// 하임 로또 당첨안내 쪽지 삭제 요청
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_DEL_SYN)
    CHARGUID iCharGuid;                     // 요청자 캐릭터 ID
    INT64    i64EntryIdx;                   // 삭제할 쪽지의 EntryIdx
PACKET_DW_CLOSE

// 하임 로또 당첨안내 쪽지 삭제 응답
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_DEL_ACK)
    INT32    iResult;                       // 0: 성공, 2:레코드없음, 1:기타오류
    CHARGUID iCharGuid1;                    // 요청자 캐릭터 ID
    CHARGUID iCharGuid2;                    // 응모 건의 캐릭터 ID
    INT64    i64Prize;                      // 상금
PACKET_DW_CLOSE

// 캐릭터 유효 소비 하임 액수 저장
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_PAYMENT_UPDATE_NTF)
    CHARGUID iCharGuid;                     // 캐릭터 ID
    DWORD    dwEffectivePayment;            // 캐릭터 세션의 최종 유효 소비 하임 액수
PACKET_DW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹맺기 시스템
//////////////////////////////////////////////////////////////////////////
//그룹 맺기 시스템 : 맴버 추가 관련 패킷 구조체
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_SYN ) // : 요청
	CHARGUID m_fromCharGUID;                 //그룹 맺기를 요청한 CharGuid
	CHARGUID m_toCharGUID;                   //그룹 맺기 요청을 받은 CharGuid
    eGROUP_STAT m_GroupSTATFirst;            //상위 맴버 상태 표시
    eGROUP_STAT m_GroupSTATSecond;           //하위 맴버 상태 표시
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_NAK ) // : 실패
	CHARGUID m_fromCharGUID;                     //그룹 맺기를 요청한 CharGuid
    CHARGUID m_toCharGUID;	
    BYTE m_byResult;                         //실패 결과 값
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_ACK ) // : 성공
	CHARGUID m_fromCharGUID;                 //그룹 맺기를 요청한 CharGuid
    CHARGUID m_toCharGUID;                   //그룹 맺기 요청을 받은 CharGuid
PACKET_DW_CLOSE

//그룹 맺기 시스템 : 맴버 삭제 관련 패킷 구조체
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_SYN ) // : 요청
	CHARGUID m_fromCharGUID;                 //그룹 맴버 삭제를 요청한 CharGUID
	CHARGUID m_toCharGUID;                   //그룹 맴버에서 삭제될 CharGUID
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_NAK ) // : 실패
	CHARGUID m_fromCharGUID;                     //그룹 맴버 삭제를 요청한 CharGUID
    CHARGUID m_toCharGUID;	
    BYTE m_byResult;                         //실패 결과 값
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_ACK ) // : 성공
	CHARGUID m_fromCharGUID;                 //그룹 맴버 삭제를 요청한 CharGUID
	CHARGUID m_toCharGUID;                   //그룹 맴버에서 삭제될 CharGUID
PACKET_DW_CLOSE

//그룹 맺기 시스템 : 맴버 리스트 요청 패킷 구조체
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_LIST_REQ )// : 요청
	CHARGUID m_CharGUID;		             //그룹 맴버 리스트를 요청한 CharGUID
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_LIST_ACK )// : 성공
	BYTE m_DataNum;				             //DB에 저장되어있느느 그룹 맴버의 개수
	CHARGUID m_CharGUID;	                 //그룹 맴버 리스트를 요청한 CharGUID
	STRUCT_GROUP_MEMBER_INFO m_GroupInfo[MAX_GROUP_MEMBER];
    // (CHANGES) (f100425.3L) (WAVERIX) prevent buffer overrun process
    uint16_t GetSize() const
    {
        assert(m_DataNum <= _countof(m_GroupInfo));
        //     <-      default fixed length       ->   <-        variable length         ->
        return (sizeof(*this) - sizeof(m_GroupInfo)) + (sizeof(m_GroupInfo[0]) * m_DataNum);
    }
PACKET_DW_CLOSE

//그룹 맺기 시스템 : 경험치 축적 관련 패킷 구조체
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ACCUMEXP_CMD )// : 요청
    DWORD m_UserKey;                        //경험치 축적을 요청한 CharGUID
    CHARGUID m_HighCharGUID;		        //경험치 축적 받을 CharGUID
    DWORD m_AccumEXP;                       //축적할 경험치
PACKET_DW_CLOSE

//그룹 맺기 시스템 : 경험치 축적 관련 패킷 구조체
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_INIT_ACCUMEXP_CMD )// : 요청
    CHARGUID m_CharGUID;		        //경험치 축적 받을 CharGUID
PACKET_DW_CLOSE

//그룹 맺기 시스템 : 그룹 맴버 정보 세팅
PACKET_DW_OPEN(DW_GROUP, DW_GROUP_SET_MEMBERINFO_CMD)// : 요청
    DWORD m_UserKey;
    eGROUP_STAT m_HighGroupinfo;
    eGROUP_STAT m_LowGroupInfo;
PACKET_DW_CLOSE

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
PACKET_DW_OPEN(DW_NOTIFICATION, DW_NOTIFICATION_ITEM_SYN)
    NotificationInfo info;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_NOTIFICATION, DW_NOTIFICATION_CLEAR_SYN)
PACKET_DW_CLOSE
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#pragma pack(pop)
