#pragma once
#pragma pack(push,1)

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_DZ.h>

#include <atltime.h>

#define PACKET_DZ_OPEN( c, p )	struct MSG_##p : public MSG_BASE_GUILD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DZ_CLOSE		};

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_SELECT_SYN )
	GUILDGUID		m_GuildGuid;
	BYTE			m_bySelectType;					// select 할 때 왜 하는지 이유.. 1 : 로그인시 정보 요청.....
	CHARGUID		m_LoginCharGuid;				// 로그인시 정보요청일때 요청하는유저 CharGuid
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_SELECT_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_SYN ) 
	TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH + 1];
	CHARGUID	m_MasterCharGuid;
	TCHAR		m_szMasterCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_ACK ) 
	CHARGUID	m_MasterCharGuid;
	GUILDGUID	m_GuildGuid;
	TCHAR		m_szMasterName[MAX_CHARNAME_LENGTH + 1];
	TCHAR		m_szGuildName[MAX_GUILDNAME_LENGTH + 1];
	eGUILD_DUTY	m_MasterDuty;
	BYTE		m_byCreateCorpsNum;
	BYTE		m_byMasterClass;
	LEVELTYPE	m_MasterLevel;
	BYTE		m_byChaoState;
	INT64		m_nGuildPenaltyTime;
	INT64		m_nDestroyRequestTime;
	INT64		m_nMasterChangeTime;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_NAK ) 
	CHARGUID	m_MasterCharGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_SYN ) 
	GUILDGUID	m_GuildGuid;
	DWORD		m_MasterCharGuid;
    DWORD		m_DestroyWaitTime;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_ACK ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_MasterCharGuid;
	INT64		m_dwDestroyTime;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_NAK ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

/*
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_GUILDMARK_SYN ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_GuildMarkBinary[MAX_GUILDMARKSTREAM_SIZE];
PACKET_DZ_CLOSE
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_GUILDMARK_ACK )
	GUILDGUID	m_GuildGuid;
PACKET_DZ_CLOSE
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_GUILDMARK_NAK ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE
*/
// 길드에 UP 헌납
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DONATE_UP_SYN ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	UPTYPE		m_GuildUP;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DONATE_UP_ACK )
	GUILDGUID	m_GuildGuid;
	UPTYPE		m_DonateUP;			// 유저가 기부한 UP
	CHARGUID	m_OperCharGuid;		// 적립한 사람 GUID ... 로그때문에 추가한다.
	UPTYPE		m_TotalUP;			// 기부한후 길드에 남은 총 UP
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DONATE_UP_NAK ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

// UP->GP 변환
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_SYN ) 
	GUILDGUID	m_GuildGuid;
	GPTYPE		m_ConverGP;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_ACK )
	GUILDGUID	m_GuildGuid;
	UPTYPE		m_RemainUP;
	UPTYPE		m_TotalGP;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_NAK ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_SYN ) 
	CHARGUID m_OperCharGuid;
	eGUILD_NOTICE	m_eNoticeType;
	GUILDGUID		m_GuildGuid;
	WORD			m_byNoticeLen;
	TCHAR			m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH+1];		// GetSize()를 하기 위해 + 1
	int GetSize()
	{
		return sizeof(MSG_DZ_GUILD_CHANGE_NOTICE_SYN) - ((MAX_GUILDNOTICE_LENGTH)-m_byNoticeLen)*sizeof(TCHAR);
	}
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_ACK )
	GUILDGUID m_GuildGuid;
	eGUILD_NOTICE	m_eNoticeType;
	CHARGUID m_OperCharGuid;
	WORD	m_byNoticeLen;
	TCHAR m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH+1];		// GetSize()를 하기 위해 + 1
	
	int GetSize()
	{
		return sizeof(MSG_DZ_GUILD_CHANGE_NOTICE_SYN) - ((MAX_GUILDNOTICE_LENGTH)-m_byNoticeLen)*sizeof(TCHAR);
	}
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_NAK ) 
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_JOIN_SYN ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	CHARGUID	m_TargetCharGuid;
	eGUILD_DUTY	m_Duty;
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
	BYTE		m_byCorpsNum;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_JOIN_ACK ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
	eGUILD_DUTY	m_Duty;
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
	BYTE		m_byChaoState;
	BYTE		m_byCorpsNum;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_JOIN_NAK )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_WITHDRAW_SYN ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	BYTE		m_byPenaltyType;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_WITHDRAW_ACK ) 
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	INT64		m_GuildPenaltyTime;
	INT64		m_MemberPenaltyTime;
	BYTE		m_byPenaltyType;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_WITHDRAW_NAK )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	BYTE		m_byErrorCode;
PACKET_DZ_CLOSE

// 길드장이 길드원 강제탈퇴 요청
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_KICKPLAYER_SYN )
	GUILDGUID	m_GuildGuid;
    CHARGUID    m_OperCharGuid;
    CHARGUID    m_TargetCharGuid;
	BYTE			m_byCorpsNum;	//길드원을 탈퇴시키는 길드원이 속한 군단번호
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_KICKPLAYER_ACK )
	GUILDGUID	m_GuildGuid;
    CHARGUID    m_OperCharGuid;
    CHARGUID    m_TargetCharGuid;
	INT64		m_GuildPenaltyTime;
	INT64		m_MemberPenaltyTime;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_KICKPLAYER_NAK )
	GUILDGUID	m_GuildGuid;
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 탈퇴시킬 캐릭터 이름
	CHARGUID	m_CharGuid;
	DWORD		m_dwErrorCode;
PACKET_DZ_CLOSE

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHUNTING_PVP_RESULT_SYN ) 
	GUILDGUID			m_GuildGuid;
	CHUNTING_PVP_INFO	m_PVPInfo;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHUNTING_PVP_RESULT_ACK ) 
	GUILDGUID			m_GuildGuid;
	CHUNTING_PVP_INFO	m_PVPInfo;
PACKET_DZ_CLOSE
#endif

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_INFO_SYN)	
	GUILDGUID m_GuildGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_INFO_ACK)	
	BYTE	m_byRelCnt;
	BYTE	m_byCorpsCnt;
	BASE_GUILD_ADD_INFO m_AddInfo;

	int GetSize()
	{
		return ( sizeof(MSG_DZ_GUILD_RELATION_INFO_ACK)
			- ((MAX_ALLIENCE_GUILD_NUM + MAX_HOSTILITY_GUILD_NUM + MAX_ONE_SIDE_HOSTILITY_NUM) - m_byRelCnt) * sizeof(GUILD_RELATION));			
	}
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_INFO_NAK)
	GUILDGUID m_GuildGuid;
	BYTE m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_SYN )
	CHARGUID m_RequestUserKey;
	GUILDGUID m_RequestGuildGuid;
	GUILDGUID m_ResponseGuildGuid;
	eGUILD_RELATION m_eRelationType;
	TCHAR m_ResponseGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_ACK )
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelationGuildGuid;
	eGUILD_RELATION	m_eRelationType;
	TCHAR m_tszRELGuildName[MAX_GUILDNAME_LENGTH + 1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 m_RelationApplyTime;
#endif
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_NAK )
	BYTE	 m_byErrorCode;
	CHARGUID m_RequestUserKey;
	CHARGUID m_ResponseUserKey;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_DELETE_SYN )
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	eGUILD_RELATION	m_eType;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_DELETE_ACK )
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	eGUILD_RELATION	m_eType;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_DELETE_NAK )
	BYTE	 m_byErrorCode;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	eGUILD_RELATION	m_eType;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_SYN )
	CHARGUID	 m_MasterCharGuid;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	TCHAR m_tszRelGuildName[MAX_GUILDNAME_LENGTH + 1];
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_ACK )
	eGUILD_RELATION	m_eRelType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 m_RelationApplyTime;
#endif
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_NAK )
	eGUILD_RELATION	m_eRelType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	BYTE	 m_byErrorCode;
PACKET_DZ_CLOSE

//길드 직책 설정
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_DUTY_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_OperGuid;
	CHARGUID			m_TargetGuid;
	eGUILD_DUTY			m_eOldDuty;
	eGUILD_DUTY			m_eNewDuty;
//	BYTE				m_byCorpsNum;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_DUTY_ACK )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_OperCharGuid;
	CHARGUID			m_TargetCharGuid;
	eGUILD_DUTY			m_eTargetChangeDuty;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_DUTY_NAK )
	GUILDGUID			m_GuildGuid;
	BYTE				m_byErrorCode;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_SYN )
	GUILDGUID						m_GuildGuid;
	CHARGUID						m_TargetGuid;
	GUILD_RIGHT_USE_INFO			m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_ACK )
	GUILDGUID						m_GuildGuid;
	CHARGUID						m_TargetGuid;
	GUILD_RIGHT_USE_INFO			m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_NAK )
	GUILDGUID			m_GuildGuid;
	BYTE				m_byErrorCode;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_MASTER_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_OperCharGuid;
	CHARGUID			m_TargetCharGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_MASTER_ACK )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_OldMasterCharGuid;
	CHARGUID			m_NewMasterCharGuid;
	INT64				m_nMasterChangeTime;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_MASTER_NAK )
	BYTE				m_byErrorCode;
PACKET_DZ_CLOSE 


//길드 군단
//군단생성
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_CORPS_SYN )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_TargetCharGuid;
	BYTE			m_byCorpsNum;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_CORPS_ACK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID		m_TargetCharGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_CORPS_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

//군단해산
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_CORPS_SYN )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID m_OperCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_CORPS_ACK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID m_OperCharGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_CORPS_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

//군단장 변경
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_SYN )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_TargetCharGuid;
	BYTE			m_byCorpsNum;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_ACK )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_TargetGuid;
	BYTE			m_byCorpsNum;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

//군단 변경
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_CORPS_SYN )
	GUILDGUID		m_GuildGuid;
	BYTE			m_bySourceCorpsNum;
	BYTE			m_byTargetCorpsNum;
	CHARGUID		m_CharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_CORPS_ACK )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_TargetGuid;
	BYTE			m_byCorpsNum;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CHANGE_CORPS_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 


//육성캠프 생성
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_SYN )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID		m_TargetCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_ACK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID		m_TargetCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

//육성캠프 해산
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_SYN )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID m_OperCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_ACK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID m_OperCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_LOGOUT_CHAR_CMD )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_CharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_SELECT_ACK )
	BYTE								m_bySelectType;
	CHARGUID							m_LoginCharGuid;				// 이 멤버는 ACK가 로그인시 정보를 줄때 사용된다.
	GUILD_INFO							m_GuildInfo;
    TCHAR m_szGuildNotice[MAX_GUILDNOTICE_LENGTH + 1]; // CHANGES: f110915.1L
	GUILD_CORPS_TOTAL_INFO				m_CorpsTotalInfo;
	SERVER_GUILD_MEMBER_TOTAL_INFO		m_MemberTotalInfo;
	int GetSize()
	{
		return sizeof(MSG_DZ_GUILD_SELECT_ACK) - ( sizeof(m_MemberTotalInfo) - m_MemberTotalInfo.GetSize() );
	}
PACKET_DZ_CLOSE

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_SELECT_CORPS_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

//GM명령어 길드 포인트 세팅.
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_SYN )
	GUILDGUID		m_GuildGuid;
	UPTYPE			m_UP;
	GPTYPE			m_GP;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_ACK )
	GUILDGUID		m_GuildGuid;
	UPTYPE			m_UP;
	GPTYPE			m_GP;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_NAK )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

////////////////길드 마크 등록////////////////
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_SYN)
	CHARGUID m_RequestPlayerIndex;
	SLOTIDX m_SlotIdx;
	POSTYPE m_SlotPos;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_ACK)
	GUILDGUID m_GuildIndex;
	SLOTIDX m_SlotIdx;
	POSTYPE m_SlotPos;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_NAK)
	BYTE m_Result;
PACKET_DZ_CLOSE 

////////////////길드 마크 삭제////////////////
PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_SYN)
	CHARGUID m_RequestPlayerIndex;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_ACK)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN( DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_NAK)
	BYTE m_Result;
PACKET_DZ_CLOSE 


PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_SYN)
GUILDGUID		m_GuildGuid;
CHARGUID		m_MasterCharGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_ACK)
GUILDGUID		m_GuildGuid;
CHARGUID		m_MasterCharGuid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_NAK)
GUILDGUID		m_GuildGuid;
CHARGUID		m_MasterCharGuid;
BYTE			m_byErrorCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_SYN)
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_CharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_ACK)
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_CharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_NAK)
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_CharGuid;
	BYTE			m_byErrorCode;
PACKET_DZ_CLOSE 

////////길드 창고 오픈///////
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_SYN)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_ACK)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
	BYTE m_SlotTabNum;
	BOOL m_WarehouseRight;
	MONEY m_WarehouseMoney;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    }
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_NAK)
	GUILDGUID m_GuildGuid;
	BYTE m_byErrorCode;
PACKET_DZ_CLOSE 

////////길드 창고 닫기///////
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_CLOSE_SYN)
	GUILDGUID m_GuildGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_CLOSE_ACK)
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_CLOSE_NAK)
PACKET_DZ_CLOSE 

////////길드 창고 입/출금///////
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_MONEY_SYN)
	GUILDGUID m_GuildGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_MONEY_ACK)
	GUILDGUID m_GuildGuid;
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_MONEY_NAK)
	GUILDGUID m_GuildGuid;
	BYTE m_byErrorCode;
PACKET_DZ_CLOSE 

////////길드 창고 변경 DB 저장///////
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_DBSAVE_SYN)
	INT m_iVersion;
	MONEY m_Money;
	GUILDGUID m_GuildGuid;
	BYTE m_ItemWorldType;
	BYTE m_byaWarehouseItem[MAX_GUILDWAREHOUSESTREAM_SIZE];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_DBSAVE_ACK)
	INT m_iVersion;
	MONEY m_Money;
	GUILDGUID m_GuildGuid;
	BYTE m_ItemWorldType;
	BYTE m_byaWarehouseItem[MAX_GUILDWAREHOUSESTREAM_SIZE];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WAREHOUSE_LOG_DBSAVE_SYN)
	MONEY m_Money;
	WORD m_ItemNum;
	BYTE m_bInOutState;
	SLOTCODE m_ItemCode;
	GUILDGUID m_GuildGuid;
	TCHAR m_CharName[MAX_CHARNAME_LENGTH+1];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_NAME_CHANGE_SYN)
POSTYPE		m_TargetPos;
GUILDGUID	m_GuildGuid;
CHARGUID	m_OperCharGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_NAME_CHANGE_ACK)
POSTYPE		m_TargetPos;
GUILDGUID	m_GuildGuid;
CHARGUID	m_OperCharGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_NAME_CHANGE_NAK)
POSTYPE		m_TargetPos;
CHARGUID	m_OperCharGuid;
BYTE		m_byErrorCode;
PACKET_DZ_CLOSE 

/************************************************************************/
/*                      지역 점령전 패킷                                */
/************************************************************************/
/*지역 점령전 기본 정보 요청*/
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_INFO_SYN)
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_INFO_ACK)
    RC::eDOMINATION_RESULT m_RsultCode;//결과값 코드
    WORD m_NumRecords;//데이터 갯수
    DOMINATION_INFO m_DominationInfo[MAX_DOMINATION_MAP];//점령전 기본 정보
    WORD GetSize() const
    {
        assert(m_NumRecords <= _countof(m_DominationInfo));
        return (sizeof(*this) - sizeof(m_DominationInfo)) +
               (sizeof(m_DominationInfo[0]) * m_NumRecords);
    }
PACKET_DZ_CLOSE 

/*지역 점령전 경매 정보 요청*/
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_AUCTION_INFO_SYN)
    MAPCODE                 m_FieldCode;                        //점령전 관련 필드 코드
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_AUCTION_INFO_ACK)
    RC::eDOMINATION_RESULT  m_RsultCode;                        //결과값 코드
    MAPCODE                 m_MapCode;                          //점령전 관련 필드 코드
    BYTE                    m_DataCount;
    DominationAuctionInfo   m_DominationActionDBInfo[MAX_DOMINATION_AUCTION];
    uint16_t GetSize() const
    {
        assert(m_DataCount <= _countof(m_DominationActionDBInfo));
        const size_t kMinSize = sizeof(*this) - sizeof(m_DominationActionDBInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DominationActionDBInfo[0]) * m_DataCount);
    }
PACKET_DZ_CLOSE 

/*지역 점령전 기본 정보 업데이트 요청*/
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_UPDATE_INFO_SYN)
    eDOMINATION_EVENT_TYPE  m_eSetType;
    DOMINATION_INFO m_DominationInfo;                               //점령전 기본 정보 
PACKET_DZ_CLOSE 

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_UPDATE_INFO_ACK)
    RC::eDOMINATION_RESULT  m_RsultCode;                            //결과값 코드
    eDOMINATION_EVENT_TYPE  m_eSetType;
    DOMINATION_INFO m_DominationInfo;                               //정상 응답일 경우 서버에서 셋팅해야 할 정보
PACKET_DZ_CLOSE 

/*지역 점령전 경매 정보 업데이트 요청*/
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_ACCEP_HEIM_SYN)
    DominationAuctionInfo          m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    POSTYPE                 m_TokenPos;//배팅 아이템 인벤토리 위치
    int number_of_betting_item; // 배팅 아이템 개수
#endif
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_ACCEP_HEIM_ACK)
    RC::eDOMINATION_RESULT          m_ResultCode;
    DominationAuctionInfo          m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    POSTYPE                 m_TokenPos;//배팅 아이템 인벤토리 위치
    int number_of_betting_item; // 배팅 아이템 개수
#endif
PACKET_DZ_CLOSE

/*지역 점령전 경매 정보 초기화 요청*/
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_INIT_AUCTIONINFO_SYN)
MAPCODE m_MapCode;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_INIT_AUCTIONINFO_ACK)
    RC::eDOMINATION_RESULT          m_ResultCode;
    MAPCODE                         m_MapCode;
PACKET_DZ_CLOSE

//_NA_20100604_BETTER_DOMINATION_MANAGER
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_UPDATE_GUILDINFO_SYN)
    GUILDGUID guild_guid_;                  //요청 길드 guid
    WORD domination_join_count_;            //점령전 참여 카운트
    WORD domination_defense_count_;         //점령전 방어 성공 카운트
    WORD domination_hunting_count_;         //점령전 보스 사냥 카운트
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_UPDATE_GUILDINFO_ACK)
    enum { DB_RESULT_FAIL, DB_RESULT_SUCCESS };
    bool db_result_;                        //DB 처리 결과 값
    GUILDGUID guild_guid_;                  //요청 길드 guid
    WORD domination_join_count_;            //점령전 참여 증가 카운트
    WORD domination_defense_count_;         //점령전 방어 성공 증가 카운트
    WORD domination_hunting_count_;         //점령전 보스 사냥 증가 카운트
PACKET_DZ_CLOSE

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN)
    DOMINATION_INFO domination_info_;
    SLOTCODE item_code_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK)
    bool result_value_;
    DOMINATION_INFO domination_info_;
    SLOTCODE item_code_;
PACKET_DZ_CLOSE

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBER_JOIN_SYN)
    MAPCODE map_code_;
    DominationMemberInfo member_info_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBER_JOIN_ACK)
    bool db_result_;
    MAPCODE map_code_;
    DominationMemberInfo member_info_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBER_LEAVE_SYN)
    MAPCODE map_code_;
    CHARGUID member_guid_;
    CHARGUID requester_guid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBER_LEAVE_ACK)
    bool db_result_;
    MAPCODE map_code_;
    CHARGUID member_guid_;
    CHARGUID requester_guid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_REQ)
    MAPCODE map_code_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_ANS)
    MAPCODE map_code_;
    BYTE data_count_;
    DominationMemberInfo member_list_[MAX_DOMINATION_MEMBER * 2];
    uint16_t GetSize() const
    {
        assert(data_count_ <= _countof(member_list_));
        const size_t kMinSize = sizeof(*this) - sizeof(member_list_);
        return static_cast<uint16_t>(kMinSize + sizeof(member_list_[0]) * data_count_);
    }
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_INIT_SYN)
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_INIT_ACK)
    bool db_result_;
PACKET_DZ_CLOSE

//_NA004034_20120102_GUILD_POINT_SYSTEM
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILDPOINT_SYSTEM)
    BYTE packet_type;
PACKET_DZ_CLOSE

/************************************************************************/
/*                      지역 점령전 패킷 END                            */
/************************************************************************/

// Z->D GM 명령에 의한 길드 창고 강제 닫기
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD)
    DWORD gm_player_key;    // 조작자 - GM 플레이어 키
    GUILDGUID guild_guid;   // 대상 길드 GUID
PACKET_DZ_CLOSE

#ifdef _NA_003923_20120130_GUILD_RENEWAL
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WRITE_IN_GAME_LOG_SYN)
    GUILDGUID guild_guid_;
    BYTE log_category_;
    GuildInGameLogData log_data_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_WRITE_IN_GAME_LOG_ACK)
    GUILDGUID guild_guid_;
    GuildInGameLogData log_data_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_IN_GAME_LOG_LIST_REQ)
    GUILDGUID guild_guid_;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_IN_GAME_LOG_LIST_ANS)
    GUILDGUID guild_guid_;
    WORD log_count_;
    GuildInGameLogData log_data_[MAX_IN_GAME_GUILD_LOG * (GuildMaxLog)];
    WORD GetSize()
    {
        assert(log_count_ <= _countof(log_data_) && log_count_ >= 0);
        const size_t min_size = sizeof(*this) - sizeof(log_data_);
        return static_cast<WORD>(min_size + (sizeof(log_data_[0]) * log_count_));
    }
PACKET_DZ_CLOSE
#endif

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_INTRODUCTION_SAVE_SYN)
    CHARGUID char_guid;
    GUILDGUID guild_guid;
    GUILD_INTRODUCTIONS guild_introductions_;

    void init()
    {
        char_guid = 0, guild_guid = 0;
        memset(&guild_introductions_, 0, sizeof(guild_introductions_));
    }
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_INTRODUCTION_LOAD_SYN)
    GUILDGUID guild_guid;
PACKET_DZ_CLOSE
#endif

PACKET_DZ_OPEN(DZ_GUILD, DZ_DOMINATION_PENALTY_CMD)
    GUILDGUID guild_guid;
    CTime penalty_time;
PACKET_DZ_CLOSE


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION

PACKET_DZ_OPEN(DZ_GUILD, DZ_CONTINENT_LORD_INFO_SYN)
    GUILDGUID lord_guild_guid;
    int       contienet_number;
    MAPCODE   map_code;
    TCHAR     lord_guild_name[MAX_GUILDNAME_LENGTH + 1];
    bool      send_game_server;
PACKET_DZ_CLOSE

#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_INSERT_SYN)
    GUILD_JOIN_REQUEST_INFO request_info;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_SELECT_SYN)
    GUILDGUID   guild_guid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_DELETE_SYN)
    CHARGUID    target_guid;
    CHARGUID    char_guid;
    GUILDGUID   guild_guid;
    eGUILD_JOIN_REJECT_TYPE reject_type;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_FACILITY_UPDATE_SYN)
    CHARGUID    char_guid;
    GUILDGUID guild_guid;
    FCODE     facility_code;
    FLEVEL    current_level;
    FLEVEL    add_level;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_FACILITY_SELECT_SYN)
    GUILDGUID guild_guid;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_FACILITY_INIT_SYN)
    GUILDGUID guild_guid;
PACKET_DZ_CLOSE
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//_NA_008334_20150608_SONNENSCHEIN
PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN)
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_INITIALIZE_ACK)
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_UPDATE_SYN)
    GUILDGUID   guild_guid;
    BYTE        sonnenschein_phase;
PACKET_DZ_CLOSE

PACKET_DZ_OPEN(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_UPDATE_ACK)
    GUILDGUID   guild_guid;
    BYTE        sonnenschein_phase;
PACKET_DZ_CLOSE
#pragma pack(pop)


















