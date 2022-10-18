#pragma once
#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_GZ.h>

#include <CGuid.h> // error C2065: 'GUID_NULL'
#include <atltime.h>

#pragma pack(push,1)

#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	#define PACKET_GZ_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ ZeroMemory(this, sizeof(*this)); m_byCategory = c; m_byProtocol = p; }
#else
	#define PACKET_GZ_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#endif

#define PACKET_GZ_CLOSE		};


PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_INFO_BRD )
	BASE_GUILD_INFO			m_BaseGuildInfo;
	GUILD_MEMBER_TOTAL_INFO	m_GuildMemberInfo;
	int GetSize()
	{
		return ( sizeof(BASE_GUILD_INFO) + m_GuildMemberInfo.GetSize() );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_SELECT_SYN )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_CharGuid;
	BYTE			m_byRequestIdx;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_SELECT_ACK )
	GUILDGUID		m_GuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_SELECT_NAK )
	BYTE			m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_SYN )
	TCHAR			m_tszGuildName[MAX_GUILDNAME_LENGTH + 1];
	CHARGUID		m_MasterCharGuid;
	TCHAR			m_tszMasterCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE			m_byMasterClass;
	LEVELTYPE		m_MasterLV;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_ACK )
	GAME_GUILD_INFO			m_GameGuildInfo;
	BASE_GUILD_MEMBER_INFO	m_BaseMemberInfo;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_NAK )
	BYTE			m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_SYN )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_MasterCharGuid;
    DWORD           m_DestroyWaitTime;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_BRD )
	GUILDGUID		m_GuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_NAK )
	BYTE			m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_JOIN_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	CHARGUID	m_TargetCharGuid;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_JOIN_BRD )
	GUILDGUID			m_GuildGuid;
	GUILD_MEMBER_INFO	m_MemberInfo;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_JOIN_NAK )
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_WITHDRAW_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_WITHDRAW_BRD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	TCHAR		m_szCharName[MAX_CHARNAME_LENGTH + 1];
	INT64		m_MemberPenaltyTime;
	BYTE		m_byPenaltyType;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_WITHDRAW_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

// ������� ���� ����Ż��
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_KICKPLAYER_SYN )
	GUILDGUID	m_GuildGuid;							// �ش� ���
	CHARGUID	m_OperCharGuid;
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// Ż���ų ����
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_KICKPLAYER_NAK )
	GUILDGUID	m_GuildGuid;
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ��û�ߴ� ������ �̸�
	DWORD		m_dwErrorCode;
PACKET_GZ_CLOSE

// ��忡 UP �峳
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DONATE_UP_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	UPTYPE		m_DonateUP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DONATE_UP_BRD )
	GUILDGUID	m_GuildGuid;
	UPTYPE		m_TotalUP;
	UPTYPE		m_DonateUP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DONATE_UP_NAK )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

// UP->GP�� ��ȯ
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	GPTYPE		m_ConvertGP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_BRD )
	GUILDGUID	m_GuildGuid;
	UPTYPE		m_RemainUP;
	GPTYPE		m_TotalGP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_NAK )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RANKUP_BRD )
	GUILDGUID	m_GuildGuid;
	GPTYPE		m_RemainGP;
	BYTE		m_byLevel;
	UPTYPE		m_RemainUP;
PACKET_GZ_CLOSE


#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHUNTING_PVP_RESULT_CMD )
	GUILDGUID			m_WinGuildGuid;
	GUILDGUID			m_LoseGuildGuid;
	GUILDGUID			m_DisGuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHUNTING_PVP_RESULT_BRD )
	GUILDGUID			m_GuildGuid;
	CHUNTING_PVP_INFO	m_PVPInfo;
PACKET_GZ_CLOSE

#endif

//��� �߰� ����
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_INFO_BRD )
	BYTE				m_byRelCnt;
	BYTE				m_byCorpsCnt;

	BASE_GUILD_ADD_INFO	m_AddInfo;

	int GetSize()
	{
		return ( sizeof(MSG_GZ_GUILD_RELATION_INFO_BRD)
			- ((MAX_ALLIENCE_GUILD_NUM + MAX_HOSTILITY_GUILD_NUM + MAX_ONE_SIDE_HOSTILITY_NUM) - m_byRelCnt) * sizeof(GUILD_RELATION));			
	}
PACKET_GZ_CLOSE


PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RELATION_INFO_SYN)
	GUILDGUID m_GuildGuid;
PACKET_GZ_CLOSE

// ��� ����
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_SYN )
	eGUILD_RELATION m_eRelationType;
	GUILDGUID m_RequestGuildGuid;
	GUILDGUID m_ResponseGuildGuid;
	USERGUID  m_RequestUserKey;
	CHARGUID  m_RequestCharGuid;
	TCHAR m_tszRelationGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_BRD )
	eGUILD_RELATION m_eRelationType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelationGuildGuid;
	TCHAR m_tszRequestGuildName[MAX_GUILDNAME_LENGTH+1];
	TCHAR m_tszRelationGuildName[MAX_GUILDNAME_LENGTH+1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 m_RelationApplyTime;
    DWORD64 m_RelationChangeTime;
#endif
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_NAK )
	CHARGUID	 m_OtherGuildMasterUid;
	GUILDGUID m_RelationGuildGuid;
	BYTE	 m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_DELETE_SYN )	// ��� ���� ����
	eGUILD_RELATION	m_eType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	CHARGUID	 m_RequestPlayerIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_DELETE_BRD )
	eGUILD_RELATION	m_eType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_DELETE_NAK )
	eGUILD_RELATION	m_eType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	BYTE m_byErrorCode;
PACKET_GZ_CLOSE

//���� ��û
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_BOTH_RELATION_SYN )
	eGUILD_RELATION m_eRelationType;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelationGuildGuid;
	CHARGUID m_RequestPlayerIndex;
	TCHAR m_tszRelationGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_BOTH_RELATION_ACK)
	eGUILD_RELATION m_eRelationType;
	GUILDGUID m_RequestGuildGuid;
	GUILDGUID m_ResponseGuildGuid;
	CHARGUID	 m_RequestPlayerIndex;
	CHARGUID	 m_ResponsePlayerIndex;
PACKET_GZ_CLOSE

//�ֹ����� ��û
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_SYN )
	CHARGUID  m_MasterCharGuid;
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	TCHAR m_tszRelGuildName[MAX_GUILDNAME_LENGTH + 1]; //������
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_BRD )
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	TCHAR m_tszRelGuildName[MAX_GUILDNAME_LENGTH + 1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
    DWORD64 m_RelationApplyTime;
    DWORD64 m_RelationChangeTime;
#endif
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_NAK )
	GUILDGUID m_GuildGuid;
	GUILDGUID m_RelGuildGuid;
	BYTE	 m_byErrorCode;
PACKET_GZ_CLOSE

// ��� ���� ����
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_CORPS_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_CORPS_BRD )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_CORPS_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_CORPS_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_CORPS_BRD )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_CORPS_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_ACK )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byEmptyCommanderCount;
	BYTE		m_byCorpsNum[MAX_GUILD_CORPS_NUM];
	int GetSize()
	{
		return ( sizeof(MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_ACK) - (MAX_GUILD_CORPS_NUM-m_byEmptyCommanderCount)*sizeof(BYTE) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

// ������ ����
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_BRD )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE


// ���ܰ� �̵�
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_CORPS_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_CORPS_BRD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_TargetCharGuid;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_CORPS_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

// ���� ķ�� â��
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH+1];	
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_BRD )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byCorpsNum;
	TCHAR		m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE
	
// ���� ķ�� �ػ�
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_BRD )
	GUILDGUID	m_GuildGuid;
	BYTE		m_byCorpsNum;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_INVITE_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	CHARGUID	m_TargetCharGuid;
	PLAYERKEY	m_TargetUserKey;
	LEVELTYPE	m_Level;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_INVITE_ACK )
	PLAYERKEY	m_TargetUserKey;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_INVITE_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_LOGIN_CHAR_SYN )
	GUILDGUID m_GuildGuid;
	CHARGUID	 m_CharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_LOGIN_CHAR_BRD )
	GUILD_INFO				m_GuildInfo;
	GUILD_CORPS_TOTAL_INFO	m_CorpsTotalInfo;
	GUILD_MEMBER_INFO		m_MemberInfo;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_LOGIN_CHAR_NAK )
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_LOGOUT_CHAR_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_LOGOUT_CHAR_BRD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	TCHAR		m_szCharName[MAX_CHARNAME_LENGTH + 1];
PACKET_GZ_CLOSE


PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHAR_LEVELUP_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
	LEVELTYPE	m_Level;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHAR_LEVELUP_BRD )
	GUILDGUID	m_GuildGuid;
	TCHAR		m_szCharName[MAX_CHARNAME_LENGTH + 1];
	LEVELTYPE	m_Level;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_KICKPLAYER_BRD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_MasterCharGuid;
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];		//������� ĳ���� �̸�
	CHARGUID	m_CharGuid;									//������� ĳ���� CharGuid
	INT64		m_GuildPenaltyTime;
	INT64		m_MemberPenaltyTime;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_CharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_ACK )
	GUILD_INFO					m_GuildInfo;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    GUILD_INFO_EXTRA            guild_info_extra_;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GUILD_JOIN_REQUEST_TOTAL_INFO     m_GuildJoinRequestTotalInfo;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	GUILD_CORPS_TOTAL_INFO		m_CropsTotalInfo;
	GUILD_MEMBER_TOTAL_INFO		m_MemberTotalInfo;

	int					GetSize()
	{
		return ( sizeof(MSG_GZ_GUILD_TOTALINFO_REQ_ACK) - ( sizeof(GUILD_MEMBER_TOTAL_INFO) - m_MemberTotalInfo.GetSize() ) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_NAK )
	BYTE				m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_CharGuid;
	UPTYPE				m_UP;
	GPTYPE				m_GP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_BRD )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_CharGuid;
	UPTYPE				m_UP;
	GPTYPE				m_GP;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_NAK )
	BYTE				m_byErrorCode;
PACKET_GZ_CLOSE

// ���� ī�� ���� ����� ��� �˸�
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_CharGuid;
	BYTE				m_byChaoState;			// ī�� ����(eCHAO_STATE)
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_BRD )
	GUILDGUID			m_GuildGuid;
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE				m_byChaoState;			// ī�� ����(eCHAO_STATE)
PACKET_GZ_CLOSE

//���� ����, ������ ��Ŷ�� �ʿ����. �ش� �������� ������ ������ �ش� ���Ӽ������� ����� �ʱ�ȭ ��Ų��.
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_ENTER_SERVER_SYN )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_ENTER_SERVER_ACK )
	GAME_GUILD_INFO			m_GameGuildInfo;
	BASE_GUILD_MEMBER_INFO	m_BaseMemberInfo;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_ENTER_SERVER_NAK )
	BYTE				m_byErrorCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_PRE_LOGIN_CMD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_GZ_CLOSE

/////////// ��� ��å ���� ///////////
//Ÿ�������� �����ϰ� ���� ������ �ִ�.
//���Ӿ��� ������ ������ ���Ӽ����� ������ ���� �ʴ´�.
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_DUTY_SYN )
	GUILDGUID			m_GuildGuid;
	CHARGUID			m_OperCharGuid;
	TCHAR				m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];		
	BYTE				m_byDuty;
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_DUTY_BRD )
	GUILDGUID			m_GuildGuid;
	TCHAR				m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE				m_byDuty;
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_DUTY_NAK )
	BYTE				m_byErrorCode;
PACKET_GZ_CLOSE 


/////////// ��� ���� ���� ��� ��û ///////////
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_SYN )
	GUILDGUID					m_GuildGuid;
	CHARGUID					m_OperCharGuid;
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];	
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_ACK )
	GUILDGUID					m_GuildGuid;
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	BYTE						m_byCount;
	GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_NAK )
	BYTE						m_byErrorCode;
PACKET_GZ_CLOSE 

	/////////// ��� ���� ���� ��û ///////////
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_SYN )
	GUILDGUID					m_GuildGuid;
	CHARGUID					m_OperCharGuid;
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];	
	BYTE						m_byCount;
	GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_BRD )
	GUILDGUID					m_GuildGuid;
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];
	GUILD_RIGHT_USE_INFO		m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_NAK )
	BYTE						m_byErrorCode;
PACKET_GZ_CLOSE 


	/////////// ��� ������ ���� ////////////
PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_MASTER_SYN )
	GUILDGUID					m_GuildGuid;
	CHARGUID					m_OperCharGuid;
	TCHAR						m_szTargetCharName[MAX_CHARNAME_LENGTH + 1];	
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_MASTER_BRD )
	GUILDGUID					m_GuildGuid;
	CHARGUID					m_OldMasterCharGuid;
	TCHAR						m_szOldMasterCharName[MAX_CHARNAME_LENGTH + 1];	
	CHARGUID					m_NewMasterCharGuid;
	TCHAR						m_szNewMasterCharName[MAX_CHARNAME_LENGTH + 1];	
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN( GZ_GUILD, GZ_GUILD_CHANGE_MASTER_NAK )
	BYTE						m_byErrorCode;
PACKET_GZ_CLOSE 

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_DESTROY_WAIT_BRD)
	GUILDGUID	m_GuildGuid;
DWORD64		m_dwDestroyTime;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_SYN)
	CHARGUID	m_CharGuid;
	GUILDGUID	m_GuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_BRD)
	GUILDGUID	m_GuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_NAK)
	CHARGUID	m_CharGuid;
	BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

//////////////��� ��ũ ���//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_SYN)
	SLOTIDX m_SlotIdx;
	POSTYPE m_SlotPos;
	GUILDGUID m_GuildIndex;
	CHARGUID m_RequestPlayerIndex;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_BRD)
	SLOTIDX m_SlotIdx;
	POSTYPE m_SlotPos;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_NAK)
	BYTE m_Result;
PACKET_GZ_CLOSE

//////////////��� ��ũ ����//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_SYN)
	CHARGUID m_RequestPlayerIndex;
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_BRD)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_BackGroundIndex;
	GUILDMARKIDX m_PatternIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_NAK)
	BYTE m_Result;
PACKET_GZ_CLOSE


//////////////��� â�� ����//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_SYN)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_BRD)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
	MONEY m_WarehouseMoney;
	BYTE m_SlotTabNum;
	BOOL m_WarehouseRight;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
	int GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return kMinSize + m_ItemInfo.GetSize();
		//return ( sizeof(*this) - (_GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_Count) * 
		//	sizeof(_GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_ACK)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
	MONEY m_WarehouseMoney;
	BYTE m_SlotTabNum;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
	int GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return kMinSize + m_ItemInfo.GetSize();
		//return ( sizeof(*this) - (_GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_Count) * 
		//	sizeof(_GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_NAK)
	GUILDGUID m_GuildGuid;
	BYTE m_bErrorCode;
PACKET_GZ_CLOSE

//////////////��� â�� �ݱ�//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_CLOSE_SYN)
	GUILDGUID m_GuildGuid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_CLOSE_BRD)
	GUILDGUID m_GuildGuid;
	MONEY m_WarehouseMoney;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
	int GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return kMinSize + m_ItemInfo.GetSize();
		//return ( sizeof(*this) - (_GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM-m_ItemInfo.m_Count)*sizeof(_GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_CLOSE_NAK)
	BYTE m_bErrorCode;
PACKET_GZ_CLOSE

//////////////��� �� �����//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_MONEY_SYN)
	GUILDGUID m_GuildIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_MONEY_BRD)
	GUILDGUID m_GuildIndex;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_MONEY_NAK)
	BYTE m_Result;
PACKET_GZ_CLOSE

//////////////��� â�� DB ����//////////////
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_DBSAVE_SYN)
	GUILDGUID m_GuildGuid;
	MONEY m_WarehouseMoney;
	BYTE m_ChannelID;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
	int GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return kMinSize + m_ItemInfo.GetSize();
		//return ( sizeof(*this) - 
		//	(_GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM - m_ItemInfo.m_Count) 
		//	*  sizeof(_GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE) );
	}
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_WAREHOUSE_LOG_DBSAVE_SYN)
	MONEY m_Money;
	SLOTCODE m_ItemCode;
	WORD m_ItemNum;
	BYTE m_bInOutState;
	GUILDGUID m_GuildGuid;
	TCHAR m_CharName[MAX_CHARNAME_LENGTH+1];
PACKET_GZ_CLOSE



//////////////��� �� ����//////////////
//__NA000000_20081106_GUILD_NAME_CHANGE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_NAME_CHANGE_SYN)
POSTYPE		m_TargetPos;
GUILDGUID	m_GuildGuid;
CHARGUID	m_OperCharGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_NAME_CHANGE_ACK)
POSTYPE		m_TargetPos;
//_NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG
TCHAR		m_tszCurrentGuildName[MAX_GUILDNAME_LENGTH+1]; // ���ӷα׿� ���� ����()
TCHAR		m_tszchangeGuildName[MAX_GUILDNAME_LENGTH+1]; // ���ӷα׿� ���� ����()
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_NAME_CHANGE_BRD)
GUILDGUID	m_GuildGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_NAME_CHANGE_NAK)
POSTYPE		m_TargetPos;
BYTE		m_byErrorCode;
PACKET_GZ_CLOSE

#ifdef _NA_003923_20120130_GUILD_RENEWAL
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ)
    GUILDGUID request_guild_guid_;
    CHARGUID request_char_guid_;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS)
    CHARGUID request_char_guid_;
    BYTE log_category_;//GuildInGameLogCategory
    BYTE log_count_;
    GuildInGameLogData log_data_[MAX_IN_GAME_GUILD_LOG];
    WORD GetSize()const
    {
		assert(log_count_ <= _countof(log_data_) && log_count_ >= 0);
        const size_t min_size = sizeof(*this) - sizeof(log_data_);
        return min_size + (sizeof(log_data_[0]) * log_count_);
    }
PACKET_GZ_CLOSE
#endif

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_INTRODUCTION_SAVE_SYN)
    CHARGUID char_guid;
    GUILDGUID guild_guid;
    GUILD_INTRODUCTIONS guild_introductions_;

    void init()
    {
        char_guid = 0, guild_guid = 0;
        memset(&guild_introductions_, 0, sizeof(guild_introductions_));
    }
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_INTRODUCTION_SAVE_ACK)
    DWORD result_;
CHAR introduction_strings_[MAX_GUILD_INTRODUCTION_LENGTH+1];
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GUILDGUID guild_guid;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
PACKET_GZ_CLOSE
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
/** ��� ���� ��û **/
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_SYN)
    GUILD_JOIN_REQUEST_INFO request_info;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_ACK)
    DWORD result_;
    GUILD_JOIN_REQUEST_INFO request_info;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_REJECT_SYN)
    eGUILD_JOIN_REJECT_TYPE reject_type;
    CHARGUID    char_guid;    //������ ���� ĳ���� GUID
    GUILDGUID   guild_guid;
    CHARGUID    target_guid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_REJECT_ACK)
    void Init()
    {
        memset(&guild_name, 0, sizeof(guild_name));
    }
    TCHAR     guild_name[MAX_GUILDNAME_LENGTH + 1];
    CHARGUID  char_guid;
    GUILDGUID guild_guid;
    CHARGUID  target_guid;
    DWORD result;
    eGUILD_JOIN_REJECT_TYPE reject_type;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RELOAD_SYN)
    CHARGUID char_guid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RELOAD_ACK)
    GUILDGUID guild_guid;
PACKET_GZ_CLOSE

/** ��� �ü� **/
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_FACILITY_UPGRADE_SYN)
    FCODE     facility_code;
    CHARGUID  char_guid;
    GUILDGUID guild_guid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_FACILITY_UPGRADE_ACK)
    GUILD_FACILITY_INFO facility_info;
    FCODE     facility_code;
    GUILDGUID guild_guid;
    CHARGUID  char_guid;
    DWORD     result;
    FLEVEL    cur_level;
    FLEVEL    add_level;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_FACILITY_SELECT_SYN)
    GUILDGUID guild_guid;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_FACILITY_SELECT_ACK)
    GUILD_FACILITY_TOTAL_INFO total_info;
    GUILDGUID guild_guid;
PACKET_GZ_CLOSE
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

/*���� ������ �⺻ ������ ���Ӽ����� ������.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_INFO_SYN)
    MAPCODE m_MapCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_INFO_ACK)
    DOMINATION_INFO         m_DominationInfo;
PACKET_GZ_CLOSE

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_CONTINENT_LORD_INFO_ACK)
sDOMINATION_CONTINENT_LORD_INFO m_lordInfo;
PACKET_GZ_CLOSE
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

/*���� ������ ��� ������ ���� ������ ������.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_AUCTION_INFO_SYN)
    MAPCODE m_MapCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_AUCTION_INFO_ACK)         
    MAPCODE                     m_MapCode;
    BYTE                        m_DataCount;
    DominationAuctionInfo       m_ActionInfo[MAX_DOMINATION_AUCTION];
    uint16_t GetSize() const
    {
        assert(m_DataCount <= _countof(m_ActionInfo));
        const size_t kMinSize = sizeof(*this) - sizeof(m_ActionInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ActionInfo[0]) * m_DataCount);
    }
PACKET_GZ_CLOSE

/*���� ������ �⺻ ������ DB�� ������Ʈ ��û�Ѵ�.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_UPDATE_INFO_SYN)
    DOMINATION_INFO             m_DominationInfo;
    eDOMINATION_EVENT_TYPE      m_eSetType;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_UPDATE_INFO_ACK)
    RC::eDOMINATION_RESULT      m_ResultCode;
    eDOMINATION_EVENT_TYPE  m_eSetType;
    DOMINATION_INFO             m_DominationInfo;
PACKET_GZ_CLOSE

/*���� ������ ��ſ� �����Ѵ�.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_ACCEP_HEIM_SYN)
    DominationAuctionInfo       m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    POSTYPE                 m_TokenPos;//���� ������ �κ��丮 ��ġ
    int number_of_betting_item; // ���� ������ ����
#endif
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_ACCEP_HEIM_ACK)
    RC::eDOMINATION_RESULT          m_ResultCode;
    DominationAuctionInfo       m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    POSTYPE                 m_TokenPos;//���� ������ �κ��丮 ��ġ
    int number_of_betting_item; // ���� ������ ����
#endif
PACKET_GZ_CLOSE

/*��Ű� ���� �Ǹ� ��� ��� �����͸� �ʱ�ȭ �Ѵ�.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_INIT_ACTIONINFO_SYN)
    MAPCODE                         m_MapCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_INIT_ACTIONINFO_ACK)
    RC::eDOMINATION_RESULT          m_ResultCode;
    MAPCODE                         m_MapCode;
PACKET_GZ_CLOSE

/*�������� ���� ��忡�� ��� ������ �����ش�.*/
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_RETURNHIME_SYN)
    DOMINATION_INFO                 m_DominationInfo;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_RETURNHIME_ACK)
    MAPCODE                         m_MapCode;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_BATTING_HEIM_SYN)
    GUILDGUID m_guildGuid;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    MAPCODE m_mapCode;
#endif 
PACKET_GZ_CLOSE
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_BATTING_HEIM_ACK)
    RC::eDOMINATION_RESULT m_ResultCode;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    MAPCODE m_mapCode;
#endif
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    DWORD penalty_time;
#endif //_NA_000000_20130429_DOMINATION_PENALTY
PACKET_GZ_CLOSE

//_NA_20100604_BETTER_DOMINATION_MANAGER
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_UPDATE_GUILDINFO_SYN)
    GUILDGUID guild_guid_;                      //��û ��� guid
    WORD domination_join_count_;                //������ ���� ī��Ʈ
    WORD domination_defense_count_;             //������ ��� ���� ī��Ʈ
    WORD domination_hunting_count_;             //������ ���� ��� ī��Ʈ
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_UPDATE_GUILDINFO_ACK)
	enum { DB_RESULT_FAIL, DB_RESULT_SUCCESS };
    bool db_result_;                            //DB ó�� ��� ��
    GUILDGUID guild_guid_;                      //��û ��� guid
    WORD domination_join_count_;                //������ ���� ���� ī��Ʈ
    WORD domination_defense_count_;             //������ ��� ���� ���� ī��Ʈ
    WORD domination_hunting_count_;             //������ ���� ��� ���� ī��Ʈ
PACKET_GZ_CLOSE

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN)
    DOMINATION_INFO domination_info_;
    SLOTCODE item_code_;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK)
    DOMINATION_INFO domination_info_;
    SLOTCODE item_code;
    bool db_result_;
PACKET_GZ_CLOSE

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBER_JOIN_SYN)
    MAPCODE map_code_;
    DominationMemberInfo member_info_;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBER_JOIN_ACK)
    bool db_result_;
    MAPCODE map_code_;
    DominationMemberInfo member_info_;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBER_LEAVE_SYN)
    MAPCODE map_code_;
    CHARGUID member_guid_; // ��û ��� ����� guid
    CHARGUID requester_guid; // ��û ��� ��û�� guid
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBER_LEAVE_ACK)
    bool db_result_;
    MAPCODE map_code_;
    CHARGUID member_guid_; // ��û ��� ����� guid
    CHARGUID requester_guid; // ��û ��� ��û�� guid
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_REQ)
    MAPCODE map_code_;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_ANS)
    MAPCODE                     map_code_;
    BYTE                        data_count_;
    DominationMemberInfo        member_list_[MAX_DOMINATION_MEMBER*2];
    uint16_t GetSize() const
    {
        assert(data_count_ <= _countof(member_list_));
        const size_t kMinSize = sizeof(*this) - sizeof(member_list_);
        return static_cast<uint16_t>(kMinSize + sizeof(member_list_[0]) * data_count_);
    }
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_INIT_CMD)
PACKET_GZ_CLOSE

// G->Z GM ��ɿ� ���� ��� â�� ���� �ݱ�
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD)
    DWORD gm_player_key;                        // ������ - GM �÷��̾� Ű
    TCHAR guild_name[MAX_GUILDNAME_LENGTH + 1]; // ��� ����
PACKET_GZ_CLOSE

// Z->G GM ��ɿ� ���� ��� â�� ���� �ݱ� ����
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD)
    DWORD gm_player_key;                        // ������ - GM �÷��̾� Ű
    GUILDGUID guild_guid;                       // ��� ��� ID
PACKET_GZ_CLOSE

//_NA004034_20120102_GUILD_POINT_SYSTEM
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILDPOINTSYSTEM)
    BYTE packet_type;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_RENAME_CHAR_CMD)
    GUILDGUID	guildguid;
    CHARGUID	charguid;
    TCHAR       character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_GZ_CLOSE

//!~ _NA_000000_20130429_DOMINATION_PENALTY
PACKET_GZ_OPEN(GZ_GUILD, GZ_DOMINATION_PENALTY_CMD)
    GUILDGUID guild_guid;
    MAPCODE map_code;
    WORD member_count;
    CTime penalty_time;
PACKET_GZ_CLOSE
//~! _NA_000000_20130429_DOMINATION_PENALTY

//_NA_008334_20150608_SONNENSCHEIN
PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD)
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_UPDATE_SYN)
    GUILDGUID   guild_guid;
    BYTE        sonnenschein_phase;
PACKET_GZ_CLOSE

PACKET_GZ_OPEN(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_UPDATE_ACK)
    GUILDGUID   guild_guid;
    BYTE        sonnenschein_phase;
PACKET_GZ_CLOSE

#pragma pack(pop)












