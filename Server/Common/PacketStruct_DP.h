
#pragma once
#pragma pack(push,1)

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_DP.h>
#include <RankingStruct.h>



#define PACKET_DP_OPEN( c, p )	struct MSG_##p : public MSG_BASE_GUILD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DP_CLOSE		};

#ifdef _NA_008012_20150130_RANKING_SYSTEM
PACKET_DP_OPEN( DP_RANKING, DP_RANKING_INFO_REQ_SYN )
	DWORD			m_SettledStartDate;		//< 정산 시작날짜
	BYTE			m_Period;				//< 주기
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_INFO_REQ_ACK )
	BYTE			m_Type;		
	RANKINGNO		m_MissionNo;
	BYTE			m_Page;
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD			m_Size;
	BYTE			m_pBUFFER[MAX_BUFFER_SIZE];
    // CHANGES: f110502.3L, logic improvements about 'GetSize'
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pBUFFER);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pBUFFER[0]) * m_Size);
    };
PACKET_DP_CLOSE
	
// 가변 패킷
PACKET_DP_OPEN( DP_RANKING, DP_RANKING_INFO_REQ_END_NTF )
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_INFO_REQ_NAK )
	BYTE			m_byErrorCode;
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_CLEAR_SYN )
	RANKINGKEY m_Key;						//< 기존의 키
	_MISSION_RANKING_TOTAL_INFO m_totalInfo;
    // CHANGES: f110502.3L, logic improvements about 'GetSize'
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_totalInfo);
        return static_cast<uint16_t>(kMinSize + m_totalInfo.GetSize());
    };
PACKET_DP_CLOSE
PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_CLEAR_ACK )
	RANKINGKEY		m_Key;						//< 기존의 키
	RANKINGKEY		m_NewKey;					//< 새로 발급 받은 파티or랭킹 키
	RANKINGNO		m_MissionNo;
PACKET_DP_CLOSE
PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_CLEAR_NAK )
	RANKINGKEY		m_Key;						//< 기존의 키
	RANKINGNO		m_MissionNo;
	BYTE			m_byErrorCode;
PACKET_DP_CLOSE

// CHANGES: f110502.3L, fixed invalid 'GetSize' calcuations = { SYN, ACK, NAK }
PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_REWARD_SYN )
	RANKINGKEY		m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
    uint16_t GetSize() const { return sizeof(*this); };
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_REWARD_ACK )
	RANKINGKEY		m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
    uint16_t GetSize() const { return sizeof(*this); };
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_MISSION_REWARD_NAK )
	BYTE			m_byErrorCode;
	RANKINGKEY		m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
    uint16_t GetSize() const { return sizeof(*this); };
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_SYN )
	RANKINGKEY					DBKey;
	DWORD						TempKey;
	DWORD						SettledStartDate;		//< 정산 시작날짜
	BYTE						Period;					//< 주기
	_MISSION_RANKING_TOTAL_INFO TotalInfo;
    // CHANGES: f110502.3L, logic improvements about 'GetSize'
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(TotalInfo);
        return static_cast<uint16_t>(kMinSize + TotalInfo.GetSize());
    };
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_ACK )
	DWORD			TempKey;						//< 기존의 키
	RANKINGKEY		DBKey;					//< 새로 발급 받은 파티or랭킹 키
	RANKINGNO		MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
PACKET_DP_CLOSE

PACKET_DP_OPEN( DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_NAK )
	RANKINGKEY		m_OldKey;						//< 기존의 키
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	BYTE			m_byErrorCode;
PACKET_DP_CLOSE
#endif // _NA_008012_20150130_RANKING_SYSTEM

#pragma pack(pop)