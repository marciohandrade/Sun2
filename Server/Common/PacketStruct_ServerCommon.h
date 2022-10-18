#ifndef _PACKETSTRUCT_SERVERCOMMON_H_
#define _PACKETSTRUCT_SERVERCOMMON_H_

#include <PacketStruct.h>
#include "./Protocol_ServerCommon.h"
#include "./ServerStructInPacket.h"

#pragma pack(push,1)



struct MSG_HEARTBEAT : public MSG_BASE
{
};

struct MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ : public MSG_BASE
{
};

struct MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS : public MSG_BASE
{
	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS()
	{
		m_byCategory = SERVERCOMMON;
		m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;
	};
};

struct MSG_SERVERCOMMON_SECURE_LOG_NTF : public MSG_BASE
{
	DWORD					m_dwUserGUID;
	DWORD					m_dwCharGUID;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR					m_pUserID[ID_MAX_LEN + 1];
#else
	TCHAR					m_pUserID[(ID_MAX_LEN>>1) + 1];			// AccountID (ID_~ 24�̸�, ���� 12��� �Ѵ�. 13���� ����)
#endif
	TCHAR					m_pCharName[MAX_CHARNAME_LENGTH+1];
	DWORD					m_IP;

	BYTE					m_AbuseType;
	BYTE					m_AbuseSubType;

	WORD					m_wLength;
	CHAR					m_pStream[1];	// ���� ��Ŷ�� ������
};


#define PACKET_COMMON_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_COMMON_CLOSE		};


#define PACKET_COMMON_INHERIT_OPEN( c, p, b )	\
    struct MSG_##p : public b { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }

#define PACKET_COMMON_INHERIT_CLOSE		};


//================================================================================
// <SERVERCOMMON>
//

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )
	WORD	m_wDataType;	// ��ũ��Ʈ ��ȣ
PACKET_COMMON_CLOSE

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
PACKET_COMMON_CLOSE

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
	// Error Code
PACKET_COMMON_CLOSE

//-------------------------------------------------------------------------------------------------------
//
//
PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
	// <TYPE><DEFINITIONs>
	enum
	{
		TRANS_CMD_NOTIFY = 0,
		TRANS_CMD_SEND_HEADER = 1,
		TRANS_CMD_SEND_DATA = 2,
		TRANS_CMD_SEND_CLOSURE = 3,
		TRANS_CMD_UPPERBOUND
	};

	struct SEND_INFO_FILE_INFO
	{
		WORD	m_wDataType;	// ��ũ��Ʈ ��ȣ, �ļ� ���� �����ؼ�...
		DWORD	m_dwBufferSize;	// ������ ������ ��Ŷ�� �� ũ��
		DWORD	m_dwCRC;		// ������ �������� CRC��
	};

	struct SEND_INFO_FILE_CLOSURE
	{
		WORD	m_wDataType;
		DWORD	m_dwCRC;
	};

	struct SEND_INFO_TOKEN_INFO_HEADER
	{
		WORD	m_wDataType;	// ��ũ��Ʈ ��ȣ, �ļ� ���� �����ؼ�...
		WORD	m_wTokenNo;		// �� ��Ͽ� ���Ե� ��ū ��ȣ
		WORD	m_wBufferSize;	// ��ū�� ���Ե� ���� ������ ����
	};

	struct SEND_INFO_TOKEN_INFO
	{
		enum { MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE = 4000 };
		SEND_INFO_TOKEN_INFO_HEADER		m_Header;
		BYTE	m_pSTREAM[MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE];

		WORD	GetSize()
		{
			return (WORD)( (sizeof(*this)-sizeof(m_pSTREAM)) + (sizeof(BYTE)*m_Header.m_wBufferSize) );
		}
	};

	// <DATA>
	WORD	m_wControlCmd;
	union
	{
		SEND_INFO_FILE_INFO		m_FILE_INFO;
		SEND_INFO_FILE_CLOSURE	m_FILE_CLOSURE;
		SEND_INFO_TOKEN_INFO	m_TOKEN_INFO;	//�� ���� ���� ũ��
	};

	WORD	GetSize()
	{
		switch( m_wControlCmd )
		{
		case TRANS_CMD_NOTIFY:
			return (WORD)( sizeof(*this)-sizeof(m_TOKEN_INFO) );
		case TRANS_CMD_SEND_HEADER:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + sizeof(m_FILE_INFO) );
		case TRANS_CMD_SEND_CLOSURE:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + sizeof(m_FILE_CLOSURE) );
		case TRANS_CMD_SEND_DATA:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + m_TOKEN_INFO.GetSize() );
		}
		return 0;
	}
PACKET_COMMON_CLOSE
//
//-------------------------------------------------------------------------------------------------------

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_ACK )
	WORD	m_wDataType;	// ��ũ��Ʈ ��ȣ
PACKET_COMMON_CLOSE

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_NAK )
	WORD	m_wDataType;	// ��ũ��Ʈ ��ȣ
PACKET_COMMON_CLOSE

PACKET_COMMON_OPEN( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF )
PACKET_COMMON_CLOSE

//
//================================================================================

//Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
PACKET_COMMON_OPEN(SERVERCOMMON, SERVERCOMMON_QUERY_CHARINFO_SYN)
    BYTE action_type_;		//< enum eCHARNAME_QUERY_ACTION_TYPE
    BOOLEAN result_;
    QueryUserIdInfo user_info_;

    inline void Clear() {
        action_type_ = 0;
        result_ = 0;
        ZeroMemory(&user_info_, sizeof(user_info_));
    }
    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_COMMON_CLOSE

PACKET_COMMON_INHERIT_OPEN(SERVERCOMMON, SERVERCOMMON_QUERY_CHARINFO_ACK,
                           MSG_SERVERCOMMON_QUERY_CHARINFO_SYN)
    inline WORD GetSize() const {
        return sizeof(*this);
    }
PACKET_COMMON_INHERIT_CLOSE

#pragma pack(pop)

#endif // _PACKETSTRUCT_SERVERCOMMON_H_