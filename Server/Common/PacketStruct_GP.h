#pragma once
#ifndef SERVERCOMMON_PACKET_STRUCT_GP_H
#define SERVERCOMMON_PACKET_STRUCT_GP_H

#include <ServerStruct.h>
#include <PacketStruct.h>

#include <Protocol_GP.h>
#include <RankingStruct.h>

//==================================================================================================
#pragma pack(push,1)

#define PACKET_GP_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_GP_CLOSE		};

//==================================================================================================
#ifdef _NA_008012_20150130_RANKING_SYSTEM
// NOTE: f110511.1L, DBP->Game, added by _NA002635_GP_DP_PROTOCOL_UINIFICATION.
PACKET_GP_OPEN(GP_RANKING, GP_RANKING_SESSION_CONTROL_CMD)
    uint8_t enabled_service;
PACKET_GP_CLOSE;

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_INFO_REQ_SYN )
PACKET_GP_CLOSE

// ���� ��Ŷ
PACKET_GP_OPEN( GP_RANKING, GP_RANKING_INFO_REQ_ACK )
	BYTE		m_Type;		
	RANKINGNO	m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	BYTE		m_Page;
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD		m_Size;
	BYTE		m_pBUFFER[MAX_BUFFER_SIZE];
	int GetSize() { return sizeof(*this)-(MAX_BUFFER_SIZE-m_Size); }
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_INFO_REQ_NAK )
	BYTE			m_byErrorCode;
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_INFO_REQ_END_NTF )
	int m_SettledYear;
	int m_SettledMonth;
	int m_SettledDay;
	int m_PeriodDay;
	int m_SuspendTime;
PACKET_GP_CLOSE


PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_CLEAR_SYN )
	RANKINGKEY		m_Key;						//< ������ Ű
	_MISSION_RANKING_TOTAL_INFO m_totalInfo;
	int GetSize() { return sizeof(*this)-(sizeof(m_totalInfo)-m_totalInfo.GetSize()); }
PACKET_GP_CLOSE
//PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_CLEAR_ACK )
//	RANKINGKEY		m_Key;						//< ������ Ű
//	RANKINGKEY		m_NewKey;					//< ���� �߱� ���� ��Ƽor��ŷ Ű
//	MISSIONNO		m_MissionNo;
//PACKET_GP_CLOSE
//PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_CLEAR_NAK )
//	RANKINGKEY		m_Key;						//< ������ Ű
//	BYTE			m_byErrorCode;
//PACKET_GP_CLOSE

// ���� ���� �ȳ�
PACKET_GP_OPEN(GP_RANKING, GP_RANKING_MISSION_SUCCESS_ACK)
	RANKINGKEY		m_Key;						//< ��ƼŰ and ��ŷŰ
	RANKINGKEY		m_NewKey;					//< ���� �߱� ���� ��Ƽor��ŷ Ű
	RANKINGNO		m_MissionNo;
	WORD			m_Ranking;					//< ��ŷ
PACKET_GP_CLOSE
PACKET_GP_OPEN(GP_RANKING, GP_RANKING_MISSION_SUCCESS_BRD)
	RANKINGKEY		m_NewKey;					//< ���� �߱� ���� ��Ƽor��ŷ Ű
	_MISSION_RANKING_TOTAL_INFO m_RankingInfo;
PACKET_GP_CLOSE
PACKET_GP_OPEN(GP_RANKING, GP_RANKING_MISSION_FAILED_NAK)
	RANKINGKEY		m_Key;						//< ������ Ű
	RANKINGNO		m_MissionNo;
	DWORD			m_dwErrorCode;
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_REWARD_SYN )
	RANKINGKEY		m_Key;						//< ����ޱ⸦ ���ϴ� �����ַ�ŷ�� Ű(�Ϸù�ȣ)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
	int GetSize() { return (int)(sizeof(*this)-(MAX_CHARNAME_LENGTH-_tcslen(m_tszCharName))*sizeof(TCHAR));  }
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_REWARD_ACK )
	RANKINGKEY		m_Key;						//< ����ޱ⸦ ���ϴ� �����ַ�ŷ�� Ű(�Ϸù�ȣ)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
	int GetSize() { return (int)(sizeof(*this)-(MAX_CHARNAME_LENGTH+1-(_tcslen(m_tszCharName)+1))*sizeof(TCHAR));  }
																						//+1�� NULL���� �Ϸ���...
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_REWARD_BRD )
	RANKINGKEY		m_Key;						//< ����ޱ⸦ ���ϴ� �����ַ�ŷ�� Ű(�Ϸù�ȣ)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
	int GetSize() { return (int)(sizeof(*this)-(MAX_CHARNAME_LENGTH+1-(_tcslen(m_tszCharName)+1))*sizeof(TCHAR));  }
																						//+1�� NULL���� �Ϸ���...
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_MISSION_REWARD_NAK )
	BYTE			m_byErrorCode;
	RANKINGKEY		m_Key;						//< ����ޱ⸦ ���ϴ� �����ַ�ŷ�� Ű(�Ϸù�ȣ)
	RANKINGNO		m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE     ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	TCHAR			m_tszCharName[MAX_CHARNAME_LENGTH+1];
	int GetSize() { return (int)(sizeof(*this)-(MAX_CHARNAME_LENGTH+1-_tcslen(m_tszCharName))*sizeof(TCHAR));  }
PACKET_GP_CLOSE



PACKET_GP_OPEN( GP_RANKING, GP_RANKING_SETTLED_INFO_CMD )
	BYTE		m_Type;		
	RANKINGNO	m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	BYTE		m_Page;
	enum { MAX_BUFFER_SIZE = 4096, };
	WORD m_Size;
	BYTE m_pBUFFER[MAX_BUFFER_SIZE];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pBUFFER);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pBUFFER[0]) * m_Size);
    };
PACKET_GP_CLOSE;

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_SETTLED_INFO_END_NTF )
PACKET_GP_CLOSE


PACKET_GP_OPEN( GP_RANKING, GP_RANKING_CHAGED_STATUS_NTF )
	BYTE			m_ChangedStatus;	//< eRANKING_STATUS ����
PACKET_GP_CLOSE


PACKET_GP_OPEN( GP_RANKING, GP_RANKING_SETTLE_CMD )
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_RANKING_REGISTER_MISSION_POINT_SYN )
	RANKINGKEY					DBKey;
	_MISSION_RECORD_INFO		RecordInfo;
	_MISSION_RANK_MEMBER_INFO   MemberInfo;
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_MISSION_RANKING_SETTLE_DATE_SYN )
	_MISSION_RECORD_INFO		RecordInfo;
	_MISSION_RANK_MEMBER_INFO   MemberInfo;
	DWORD						SettledStartDate;	//< ���� ���۳�¥
	BYTE						Period;				//< �ֱ�
PACKET_GP_CLOSE

PACKET_GP_OPEN( GP_RANKING, GP_MISSION_RANKING_SETTLE_DATE_ACK )
	_MISSION_RECORD_INFO		RecordInfo;
	_MISSION_RANK_MEMBER_INFO   MemberInfo;
	DWORD						SettledStartDate;	//< ���� ���۳�¥
	BYTE						Period;				//< �ֱ�
PACKET_GP_CLOSE
#endif // _NA_008012_20150130_RANKING_SYSTEM

#pragma pack(pop)
//==================================================================================================

#endif //SERVERCOMMON_PACKET_STRUCT_GP_H