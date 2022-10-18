#pragma once

#include <ServerStructInPacket.h>
#include <ServerStruct.h>
#include <PacketStruct.h>

#include <Protocol_DW.h>

#pragma pack(push,1)

#define PACKET_DW_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DW_CLOSE		};

//////////////////////////////////
// ���� ���� ��������
PACKET_DW_OPEN(DW_CONNECTION, DW_CONNECTION_SERVER_INFO_CMD)
	SERVER_KEY		m_ServerKey;
	DWORD			m_dwServerGUID;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN];
	WORD			m_wDBProxyServerPort;
PACKET_DW_CLOSE;

///////////////////////////////////
// ģ�� ���� ��������

// ģ�� ��� ��û
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_SYN)
	CHARGUID		m_CharGuid;								//< ģ�� ����� ��û�ϴ� ĳ���� guid

    uint16_t GetSize() const { return sizeof(*this); }
PACKET_DW_CLOSE;


// �����丵 ����
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

// ������
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_ACK)
    // CHANGES: f110104.1L, fixed a data type for counting to solve type overflow.
    CHARGUID m_CharGuid; //< ģ�� ����� ���� ĳ������ guid
    uint16_t number_of_friends; //< ģ�� ����
    STRUCT_FRIEND_DB_INFO m_sFriendInfo[MAX_FRIENDBLOCK_STAT_NUM];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sFriendInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_sFriendInfo[0]) * number_of_friends);
    }
PACKET_DW_CLOSE;

// ģ�� ��� ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_GETLIST_NAK)
	enum
	{
		ERR_DEFAULT,
//		NO_FRIEND,		//< ģ�� ��~��~
	};

	CHARGUID				m_CharGuid;
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// ģ�� ��û
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_SYN)
	CHARGUID				m_CharGuidFrom;								//< ģ�� ��û�� ���� guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];		//< ģ�� ��û�� ���� ���� ĳ���͸�
PACKET_DW_CLOSE;

// ģ�� ��û ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_ACK)
	CHARGUID				m_CharGuidFrom;								//< ģ�� ��û�� ���� guid
	STRUCT_FRIEND_INFO		m_ToFriendInfo;								//< ģ�� ��û�� ���� ���� ĳ��������
PACKET_DW_CLOSE;

// ģ�� ��û ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_ADD_NAK)
	enum
	{
		ERR_DEFAULT		= 0,		//< ��Ÿ ���
		NO_USER			= 1,		//< �ش� GUID�� ������ ����
		ALREADY_FRIEND	= 2,		//< �̹� ģ�� ����
		BLOCKED_USER	,			//< ���� ����
		
	};

	CHARGUID				m_CharGuidFrom;
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// ģ�� ���� ��û
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_SYN)
	CHARGUID		m_CharGuidFrom;	//< ������ ��û�� ��
	CHARGUID		m_CharGuidTo;	//< �����Ǵ� ��
	BYTE			m_Sts;			//< ģ�� ���� �ڵ�, STRUCT_FRIEND_DB_INFO::eNodeType, _KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_DW_CLOSE;

// ģ�� ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_ACK)
	CHARGUID		m_CharGuidFrom;	//< ������ ��û�� ��
	CHARGUID		m_CharGuidTo;	//< �����Ǵ� ��
PACKET_DW_CLOSE;

// ģ�� ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_DEL_NAK)
	enum
	{
		ERR_DEFAULT,			//< �� �� ���� ����
		NOT_FRIEND_USER,		//< ģ�� �ƴ�
	};
	CHARGUID				m_CharGuidFrom;			//< ������ ��û�� ��
	BYTE					m_byResult;
PACKET_DW_CLOSE;

// ���� ��û(ģ���� �ƴϵ�)
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_SYN)
	CHARGUID		m_CharGuidFrom;									//< ���� ��û�� �� guid
	TCHAR			m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];			//< ���� ���ϴ� ��
PACKET_DW_CLOSE;

// ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_ACK)
	CHARGUID			m_CharGuidFrom;								//< ���� ��û�� �� guid
	STRUCT_BLOCK_INFO	m_BlockInfo;
PACKET_DW_CLOSE;

// ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_NAK)
	CHARGUID			m_CharGuidFrom;								//< ���� ��û�� �� guid
	BYTE				m_byResult;
PACKET_DW_CLOSE;

// ���� ���� ��û
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_SYN)
	CHARGUID		m_CharGuidFrom;				//< ���� ���� ��û�� �� guid
	CHARGUID		m_CharGuidTo;				//< �����Ǵ� ��
PACKET_DW_CLOSE;

// ���� ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_ACK)
	CHARGUID		m_CharGuidFrom;				//< ���� ���� ��û�� �� guid
	CHARGUID		m_CharGuidTo;				//< �����Ǵ� ��
PACKET_DW_CLOSE;

// ���� ���� ����
PACKET_DW_OPEN(DW_FRIEND, DW_FRIEND_BLOCK_FREE_NAK)
	enum
	{
		ERR_DEFAULT		=0,		//< �� �� ���� ����
		NO_USER			=1,		//< �������� �ʴ� ĳ���� �̸�
		NOT_BLOCKED,			//< ���ܵ� ĳ���Ͱ� �ƴ�
	};

	CHARGUID		m_CharGuidFrom;									//< ���� ���� ��û�� �� guid
	BYTE			m_byResult;
PACKET_DW_CLOSE;

//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_DW_OPEN( DW_FRIEND, DW_FRIEND_INVITATIONRELATION_FREE_CMD )
    CHARGUID		m_CharGuid;									// ��õ���� ĳ���� GUID
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
// GM ���� ��������

// ä�ñ���
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_SYN )
	TCHAR		m_ptszGMAccountID[ID_MAX_LEN+1];	// ���� ����� GM ������
	CHARGUID	m_CharGuid;		// ���� ��� �����
	DWORD		m_dwBlockTIme;	// ä�� ���� �ð�(�д���)
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_ACK )
	CHARGUID		m_CharGuid;
	DWORD			m_dwBlockTime;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

// ä�����
PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_SYN )
	TCHAR			m_ptszGMAccountID[ID_MAX_LEN+1];	// ��� ����� GM ������
	CHARGUID		m_CharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_ACK )
	CHARGUID		m_CharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATALLOW_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

// ä�ñ��� �ð� ����
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_SYN )
	DWORD			m_dwRemainTime;		// �����ǰ� ���� �ð�
PACKET_DW_CLOSE

//PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_ACK )
//PACKET_DW_CLOSE
	
PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCK_UPDATE_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_REQ )
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_ANS )
	DWORD			m_dwRemainTime;		// ���� �ð�(0�̸� Block���� �ƴ�)
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_GM, DW_GM_CHATBLOCKTIME_NAK )
	WORD			m_wErrorCode;
PACKET_DW_CLOSE


//////////////////////////////////
// ���� PK ���� ��������

// ������ ����Ʈ�� ��� ��û
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< ��� ��û�� ���� guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< ����� ĳ���͸�
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< ��� ��û�� ���� guid
	STRUCT_REVENGE_INFO		m_sRevengeUserInfo;							//< ����� ĳ���� ����
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< ��� ��û�� ���� guid
	BYTE					m_byErrorCode;								//< RC::eREVENGE_RESULT ����
PACKET_DW_CLOSE

// ������ ����Ʈ���� ���� ��û(C->W->D)
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< ���� ��û�� ���� guid
	CHARGUID				m_RevengeGuidTo;							//< ������ ĳ���� Guid
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< ���� ��û�� ���� guid
	CHARGUID				m_RevengeGuidTo;							//< ������ ĳ���� Guid
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< ��� ��û�� ���� guid
	BYTE					m_byErrorCode;								//< RC:eREVENGE_RESULT ����
PACKET_DW_CLOSE

// ������ �����Ͽ� ����Ʈ���� ���� ��û(G->A->W->D)
// -- �� ��Ŷ ����ü���� �⺻������ DW_CHAO_REMOVE_REVENGE_CHAR...�� ������ �����ϴ�. ���� WorldServer�� �ൿ�� �ٸ��� ������ Ÿ�� ����
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN		MSG_DW_CHAO_PK_REVENGE_CHAR_SYN;
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK		MSG_DW_CHAO_PK_REVENGE_CHAR_ACK;
typedef MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK		MSG_DW_CHAO_PK_REVENGE_CHAR_NAK;

/*
PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_SYN )
	CHARGUID				m_CharGuidFrom;								//< ���� ��û�� ���� guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< ������ ĳ���͸�
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_ACK )
	CHARGUID				m_CharGuidFrom;								//< ���� ��û�� ���� guid
	TCHAR					m_ptszCharNameTo[MAX_CHARNAME_LENGTH+1];	//< ������ ĳ���͸�
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_NAK )
	CHARGUID				m_CharGuidFrom;								//< ��� ��û�� ���� guid
	BYTE					m_byErrorCode;								//< RC:eREVENGE_RESULT ����
PACKET_DW_CLOSE
*/

//////////////////////////////////
// ���� ���� ��������
// ���� ����
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_SYN )
	CHARGUID		    m_SendGuid;
	TCHAR			    m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	TCHAR			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    CHARGUID            m_recvGuid;//���� ���� Guid
    SLOTCODE            m_recvItemCode;//���� ���� ������ �ڵ�
    ULONGLONG           m_amountCost;//���� ���� �ݾ�
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//�ý��� �޸� ������
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_ACK )
	CHARGUID		    m_SendGuid;
	CHARGUID		    m_RecvGuid;
	TCHAR			    m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	STRUCT_SUBJECT_PACKETINFO	m_Subject;
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//ȯ�� ���� ������ �ڵ�
    ULONGLONG           m_amountCost;//ȯ�� ���� �ݾ�
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//�ý��� �޸� ������
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_SEND_NAK )
	CHARGUID		m_SendGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// ���� ��� Ȯ��
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_LIST_REQ )
	CHARGUID		m_RecvGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_LIST_ANS )
	CHARGUID		m_RecvGuid;
	BYTE			m_byMemoCnt;				// ���� ����(���� ����������)
	BYTE			m_byMemoPage;				// ���� ����� ���� ������(10�� ������ ����)
	BYTE			m_byMemoPageCnt;			// ���� ����� �� ������(���� ���� 123���� 7�������� ��. m_byMemoPage=m_byMemoPageCnt�� �۽� �Ϸ�)
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

// ���� ���� Ȯ��
PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_REQ )
	CHARGUID		m_RecvGuid;
	MEMOGUID		m_MemoGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_ANS )
	CHARGUID		    m_RecvGuid;
	MEMOGUID		    m_MemoGuid;
	TCHAR			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//ȯ�� ���� ������ �ڵ�
    ULONGLONG           m_amountCost;//ȯ�� ���� �ݾ�
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//�ý��� �޸� ������
    // }
PACKET_DW_CLOSE

PACKET_DW_OPEN( DW_MEMO, DW_MEMO_VIEW_NAK )
	CHARGUID		m_RecvGuid;
	BYTE			m_byErrorCode;
PACKET_DW_CLOSE

// ���� ����
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

// ��� �α� Ȯ��
PACKET_DW_OPEN(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_REQ)
	GUILDGUID m_GuildGuid;
	CHARGUID		m_ReqCharGuid;
PACKET_DW_CLOSE

PACKET_DW_OPEN(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_ANS)
	GUILDGUID m_GuildGuid;
	CHARGUID	 m_ReqCharGuid;
	BYTE	m_byLogCount;	 // �α� ����(���� ����������)
	BYTE	m_byCurrentPage; // ���� �Ǵ� ���� ������
	WORD m_byLogPageCount;	// �α� ����� �� ������(���� �α� 123���� 7�������� ��. m_byCurrentPage=m_byLogPageCount�� �۽� �Ϸ�)
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

//���� ������ ����/ȯ�� ���� ��ȸ ��Ŷ
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
// DW_LOTTO : ���� �ζ� ī�װ�
//------------------------------------------------------------------------------

// ���� �ζ� ���� ���� �ε� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_SYN)
    BYTE     bFlag;                         // Ŭ���̾�Ʈ ���� �÷���
    CHARGUID iCharGuid;                     // ��ȸ�� ĳ���� ID
    BYTE     bNumRecent;                    // ���� ȸ�� ���� �ִ� �� ȸ���� ������ ������ ������
    BYTE     bInPreLoading;                 // 0�� �ƴϸ� Ŭ���̾�Ʈ ��û�� �ƴ� �����ε�
PACKET_DW_CLOSE

// ���� �ζ� ���� ���� �ε� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_ACK)
    BYTE     bFlag;                         // Ŭ���̾�Ʈ ���� �÷���
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bNumRemainingTickets;          // ���� ���� ���� ����
    BYTE     bNumUsedTickets;               // ���� ȸ���� �̹� ������ ���� ���� ����
    DWORD    dwEffectivePayment;            // ���� ��ȿ �Һ� ���� �׼�
    BYTE     bInPreLoading;                 // ��û���� ������ �����ε� �÷���

    static const size_t MAX_ENTRIES = LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS;
    WORD             wNumEntries;           // �� �� ���� ���� ���� �����Ͱ� ���ԵǾ� �ִ���
    ServerLottoEntry entries[MAX_ENTRIES];  // ���� ����

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(entries);
        return static_cast<uint16_t>(kMinSize + sizeof(entries[0]) * wNumEntries);
    }
PACKET_DW_CLOSE

// ���� �ζ� ���� �ε� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_SYN)
    BYTE bNumRecent;                        // ���� ȸ�� ���� �ִ� �� ȸ���� ������ ������ ������
PACKET_DW_CLOSE

// ���� �ζ� ���� �ε� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_LOAD_ACK)
    WORD  wCurrentDrawNum;                  // ���� ȸ��
    INT64 i64TotalAccum;                    // ���� �� ������

    static const size_t MAX_RESULTS = LOTTO_NUM_RECENT_DRAWS;
    BYTE              bNumResultRecords;    // ���� ȸ�� ���� �� �� ȸ�� �ζ� ��� �����Ͱ� ���ԵǾ� �ִ���
    ServerLottoResult results[MAX_RESULTS]; // ���� ȸ�� ���� ��÷ ���
PACKET_DW_CLOSE

// ���� �ζ� ��÷�� ��� �ε� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_WINNER_LIST_SYN)
    WORD  wDrawNum;                         // ȸ�� ��ȣ
    INT32 iPage;                            // ��û�� ������ ��ȣ
PACKET_DW_CLOSE

// ���� �ζ� ��÷�� ��� �ε� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_WINNER_LIST_ACK)
    WORD  wDrawNum;                         // ȸ�� ��ȣ
    INT32 iPage;                            // ��û�� ������ ��ȣ
    INT32 iMaxPage;                         // �ִ� ������ ��ȣ (�������� �� ��)
    
    static const size_t MAX_RECORDS = LOTTO_NUM_WINNERS_PER_PAGE;
    INT32             iNumRecords;          // �� ���� ��÷�� ������ ���ԵǾ� �ִ���
    ServerLottoWinner winners[MAX_RECORDS]; // ��÷�� ���

    int GetSize() const
    {
        return (sizeof(MSG_DW_LOTTO_WINNER_LIST_ACK) \
            - (sizeof(ServerLottoWinner) * (MAX_RECORDS - iNumRecords)));
    }
PACKET_DW_CLOSE

// ���� �ζ� ���� ó�� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_SYN)
    CHARGUID iCharGuid;                     // ĳ���� ID
    WORD     wDrawNum;                      // ���� ȸ��
    BYTE     bIsAuto;                       // �ڵ� ���� ����
    BYTE     bStatus;                       // ����/��÷ ����
    BYTE     bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
PACKET_DW_CLOSE

// ���� �ζ� ���� ó�� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_ACK)
    CHARGUID iCharGuid;                     // ĳ���� ID
    WORD     wDrawNum;                      // ���� ȸ��
    BYTE     bIsAuto;                       // �ڵ� ���� ����
    BYTE     bStatus;                       // ����/��÷ ����
    BYTE     bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
    INT32    iResult;                       // ���� ��� (0:����, 1:����)
PACKET_DW_CLOSE

// ���� �ζ� ��÷ �� �̿� ó�� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ROLLOVER_SYN)
    WORD  wCurrentDrawNum;                  // ���� ȸ��
    INT8  iNumWinners;                      // 1�� ��÷�� ��
    INT64 i64Jackpot;                       // 1�� ��÷��
    INT64 i64Prize2;                        // 2�� ��÷��
    INT64 i64Prize3;                        // 3�� ��÷��
    BYTE  bWinningNum[LOTTO_NUM_PICKS];     // ��ȸ ��÷ ��ȣ
PACKET_DW_CLOSE

// ���� �ζ� ��÷ �� �̿� ó�� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ROLLOVER_ACK)
    INT32 iResult;                          // ���� ��� (0:����, 1:����)
PACKET_DW_CLOSE

// ���� �ζ� �� ������ ���� �뺸
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ACCUM_NTF)
    WORD  wCurrentDrawNum;                  // ȸ�� ��ȣ
    INT64 i64TotalAccum;                    // �� ������
PACKET_DW_CLOSE

// ���� �ζ� ���� ���� �� ���� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ADD_TICKET_SYN)
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bNumRemainingTickets;          // ���� ���� ���� ��
    BYTE     bMaxTickets;                   // �ִ�� ���� ������ ���� ���� ��
PACKET_DW_CLOSE

// ���� �ζ� ���� ���� �� ���� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_ADD_TICKET_ACK)
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bNumRemainingTickets;          // ���� ���� ���� ��
    INT32    iResult;                       // ���� ��� (0:����, 1:����)
PACKET_DW_CLOSE

// ���� �ζ� ��÷�� ���� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_REWARD_SYN)
    DWORD    dwUserKey;                     // DB User Key
    INT64    i64Idx;                        // ���� �� ID
    WORD     wDrawNum;
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bStatus;
    INT64    i64Prize;
PACKET_DW_CLOSE

// ���� �ζ� ��÷�� ���� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_REWARD_ACK)
    INT32    iResult;                       // ���� ��� (0:����, 1:����)
    DWORD    dwUserKey;                     // DB User Key
    INT64    i64Idx;                        // ���� �� ID
    WORD     wDrawNum;
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bStatus;
    INT64    i64Prize;
    TCHAR    szCharName[MAX_CHARNAME_LENGTH + 1]; // ĳ���͸�
PACKET_DW_CLOSE

// ���� �ζ� ��÷�ȳ� ���� ��� �ε� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_LIST_SYN)
    CHARGUID iCharGuid;                     // ĳ���� ID
    BYTE     bMaxCount;                     // ������ �ֱ� ������ �ִ� ����
PACKET_DW_CLOSE

// ���� �ζ� ��÷�ȳ� ���� ��� �ε� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_LIST_ACK)
    CHARGUID iCharGuid;
    
    static const size_t MAX_RECORDS = LOTTO_MAX_NOTICE;
    INT               iNumRecords;          // �� ���� ���� �׸��� ���ԵǾ� �ִ��� ( <=LOTTO_MAX_NOTICE )
    LottoNoticeRecord notices[MAX_RECORDS]; // ��÷ �ȳ� ���� ���

    int GetSize() const
    {
        return (sizeof(MSG_DW_LOTTO_NOTICE_LIST_ACK) \
            - (sizeof(LottoNoticeRecord) * (MAX_RECORDS - iNumRecords)));
    }
PACKET_DW_CLOSE

// ���� �ζ� ��÷�ȳ� ���� ���� ��û
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_DEL_SYN)
    CHARGUID iCharGuid;                     // ��û�� ĳ���� ID
    INT64    i64EntryIdx;                   // ������ ������ EntryIdx
PACKET_DW_CLOSE

// ���� �ζ� ��÷�ȳ� ���� ���� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_NOTICE_DEL_ACK)
    INT32    iResult;                       // 0: ����, 2:���ڵ����, 1:��Ÿ����
    CHARGUID iCharGuid1;                    // ��û�� ĳ���� ID
    CHARGUID iCharGuid2;                    // ���� ���� ĳ���� ID
    INT64    i64Prize;                      // ���
PACKET_DW_CLOSE

// ĳ���� ��ȿ �Һ� ���� �׼� ����
PACKET_DW_OPEN(DW_LOTTO, DW_LOTTO_PAYMENT_UPDATE_NTF)
    CHARGUID iCharGuid;                     // ĳ���� ID
    DWORD    dwEffectivePayment;            // ĳ���� ������ ���� ��ȿ �Һ� ���� �׼�
PACKET_DW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷�α� �ý���
//////////////////////////////////////////////////////////////////////////
//�׷� �α� �ý��� : �ɹ� �߰� ���� ��Ŷ ����ü
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_SYN ) // : ��û
	CHARGUID m_fromCharGUID;                 //�׷� �α⸦ ��û�� CharGuid
	CHARGUID m_toCharGUID;                   //�׷� �α� ��û�� ���� CharGuid
    eGROUP_STAT m_GroupSTATFirst;            //���� �ɹ� ���� ǥ��
    eGROUP_STAT m_GroupSTATSecond;           //���� �ɹ� ���� ǥ��
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_NAK ) // : ����
	CHARGUID m_fromCharGUID;                     //�׷� �α⸦ ��û�� CharGuid
    CHARGUID m_toCharGUID;	
    BYTE m_byResult;                         //���� ��� ��
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ADD_ACK ) // : ����
	CHARGUID m_fromCharGUID;                 //�׷� �α⸦ ��û�� CharGuid
    CHARGUID m_toCharGUID;                   //�׷� �α� ��û�� ���� CharGuid
PACKET_DW_CLOSE

//�׷� �α� �ý��� : �ɹ� ���� ���� ��Ŷ ����ü
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_SYN ) // : ��û
	CHARGUID m_fromCharGUID;                 //�׷� �ɹ� ������ ��û�� CharGUID
	CHARGUID m_toCharGUID;                   //�׷� �ɹ����� ������ CharGUID
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_NAK ) // : ����
	CHARGUID m_fromCharGUID;                     //�׷� �ɹ� ������ ��û�� CharGUID
    CHARGUID m_toCharGUID;	
    BYTE m_byResult;                         //���� ��� ��
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_DEL_ACK ) // : ����
	CHARGUID m_fromCharGUID;                 //�׷� �ɹ� ������ ��û�� CharGUID
	CHARGUID m_toCharGUID;                   //�׷� �ɹ����� ������ CharGUID
PACKET_DW_CLOSE

//�׷� �α� �ý��� : �ɹ� ����Ʈ ��û ��Ŷ ����ü
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_LIST_REQ )// : ��û
	CHARGUID m_CharGUID;		             //�׷� �ɹ� ����Ʈ�� ��û�� CharGUID
PACKET_DW_CLOSE
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_LIST_ACK )// : ����
	BYTE m_DataNum;				             //DB�� ����Ǿ��ִ��� �׷� �ɹ��� ����
	CHARGUID m_CharGUID;	                 //�׷� �ɹ� ����Ʈ�� ��û�� CharGUID
	STRUCT_GROUP_MEMBER_INFO m_GroupInfo[MAX_GROUP_MEMBER];
    // (CHANGES) (f100425.3L) (WAVERIX) prevent buffer overrun process
    uint16_t GetSize() const
    {
        assert(m_DataNum <= _countof(m_GroupInfo));
        //     <-      default fixed length       ->   <-        variable length         ->
        return (sizeof(*this) - sizeof(m_GroupInfo)) + (sizeof(m_GroupInfo[0]) * m_DataNum);
    }
PACKET_DW_CLOSE

//�׷� �α� �ý��� : ����ġ ���� ���� ��Ŷ ����ü
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_ACCUMEXP_CMD )// : ��û
    DWORD m_UserKey;                        //����ġ ������ ��û�� CharGUID
    CHARGUID m_HighCharGUID;		        //����ġ ���� ���� CharGUID
    DWORD m_AccumEXP;                       //������ ����ġ
PACKET_DW_CLOSE

//�׷� �α� �ý��� : ����ġ ���� ���� ��Ŷ ����ü
PACKET_DW_OPEN( DW_GROUP, DW_GROUP_INIT_ACCUMEXP_CMD )// : ��û
    CHARGUID m_CharGUID;		        //����ġ ���� ���� CharGUID
PACKET_DW_CLOSE

//�׷� �α� �ý��� : �׷� �ɹ� ���� ����
PACKET_DW_OPEN(DW_GROUP, DW_GROUP_SET_MEMBERINFO_CMD)// : ��û
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
