#pragma once

#include <Struct.h>
#include <PacketStruct.h>
#include <StructInPacket.h>
#include <Protocol_CW.h>

#pragma pack(push,1)

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//		W : World Server
//
// * Postfix
//		SYN - Server�� ��û
//		ACK - ��û�� ���� ���
//		NAK - ��û�� ���� �ź�
//		CMD - Server�� ���
//		BRD - Server�� ����ȭ ���� ������ �˸�
//
// * �������� Naming Rule
//		Prefix_ī�װ���_�������ݸ�_Postfix
//		ex) CW_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------

#define PACKET_CW_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_CW_CLOSE		};

#define PACKET_CW_FWD_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_CW_FWD_CLOSE		};

#define PACKET_CW_INHERIT_OPEN(c, p, b) \
    struct MSG_##p : public b { \
        MSG_##p() { \
            m_byCategory = c; \
            m_byProtocol = p; \
        }

#define PACKET_CW_INHERIT_CLOSE \
    }


//-------------------------------------------------------------------------------------------------
// CW_CONNECTION ī�װ�
//-------------------------------------------------------------------------------------------------
struct MSG_CW_HEARTBEAT : public MSG_BASE
{
	MSG_CW_HEARTBEAT()	{ m_byCategory = CW_CONNECTION; m_byProtocol = CW_HEARTBEAT; }
	int GetSize()		{ return sizeof(MSG_CW_HEARTBEAT); }
};

struct MSG_CW_ENTER_SERVER_SYN : public MSG_BASE
{
	DWORD				dwAuthUserID;
};

// �����ϸ� ���� ���� �ִ� ��Ŷ
PACKET_CW_OPEN( CW_CONNECTION, CW_ENTER_READY_BRD )
PACKET_CW_CLOSE

//-------------------------------------------------------------------------------------------------
// CW_CHAT ī�װ�
//-------------------------------------------------------------------------------------------------

// �ӼӸ� ��û
PACKET_CW_OPEN(CW_CHAT, CW_WHISPER_SYN)
    char szCharNameTo[MAX_CHARNAME_LENGTH + 1];
    BYTE byMsgLen; // _tcslen(szWhisperMsg)
    char szWhisperMsg[MAX_CHAT_LEN + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(szWhisperMsg) + sizeof(szWhisperMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(szWhisperMsg[0]) * byMsgLen);
    };
PACKET_CW_CLOSE;

// �ӼӸ� ����
PACKET_CW_OPEN(CW_CHAT, CW_WHISPER_ACK)
    BYTE byMsgLen; // _tcslen(szWhisperMsg)
    char szCharNameFrom[MAX_CHARNAME_LENGTH + 1];
    char szWhisperMsg[MAX_CHAT_LEN + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(szWhisperMsg) + sizeof(szWhisperMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(szWhisperMsg[0]) * byMsgLen);
    };
PACKET_CW_CLOSE;

// �ӼӸ� ����
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_NAK )
	enum
	{
		USER_NOT_FOUND,
		USER_WHISPER_OFF,
	};

	BYTE		byReason;
	
	int GetSize() { return sizeof(MSG_CW_WHISPER_NAK); }
PACKET_CW_CLOSE

// ���� ��û
PACKET_CW_OPEN( CW_CHAT, CW_NOTICE_SYN )
	BYTE		byLen;
	char		szMsg[MAX_NOTICE_LEN+1];

	int GetSize() { return ( sizeof(MSG_CW_NOTICE_SYN) - ( sizeof(char) * ( MAX_NOTICE_LEN - byLen ) ) ); }
PACKET_CW_CLOSE

// ���� ��ε�ĳ��Ʈ
PACKET_CW_OPEN( CW_CHAT, CW_NOTICE_BRD )
	BYTE		m_byRed;
	BYTE		m_byGreen;
	BYTE		m_byBlue;
	WORD		wLen;
	char		szMsg[MAX_NOTICE_LEN+1];
    // CHANGES: f101210.2L, fixed invalid calculation, this message need consider null-terminated
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(szMsg);
        return static_cast<uint16_t>(kMinSize + wLen * sizeof(szMsg[0]));
    };
PACKET_CW_CLOSE

// �ӼӸ� ���� ��û
struct MSG_CW_WHISPER_SET_SYN : public MSG_BASE
{
	MSG_CW_WHISPER_SET_SYN() { m_byCategory = CW_CHAT; m_byProtocol = CW_WHISPER_SET_SYN; }

	BOOL	bWhisper;		// TRUE�� ��, FALSE�� ��

	int GetSize() { return sizeof(MSG_CW_WHISPER_SET_SYN); }
};

// �ӼӸ� ���� üũ ���
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_SET_ACK )
	BOOL	bWhisper;		// ����ڰ� ���� ���� �״�� �����ش�

	int GetSize()		{ return sizeof(MSG_CW_WHISPER_SET_ACK); }
PACKET_CW_CLOSE

// �ӼӸ� ����
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_SET_NAK )
	int GetSize()		{ return sizeof(MSG_CW_WHISPER_SET_NAK); }
PACKET_CW_CLOSE

// �Ϲ� ä�� ��û
PACKET_CW_FWD_OPEN(CW_CHAT, CW_CHAT_NORMAL_SYN)
    enum { _MAX_CHATMSG_SIZE = 255 };
    BYTE m_byMsgLength; // _tcslen(m_pszChatMsg)
    char m_pszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszChatMsg) + sizeof(m_pszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszChatMsg[0]) * m_byMsgLength);
    };
PACKET_CW_FWD_CLOSE

// �Ϲ� ä�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_NORMAL_NAK )
//	enum
//	{
//		_ERR_DEFAULT = 0,
//		_ERR_CHATBLOCKED,	// ä�ñ��� ����
//		_ERR_NOTSYNC,		// ��ũ�� �� ���� ����(VIEWPORT_CHARSTATE ������ ��û
//	};
	WORD						m_wErrorCode;	// RC::eCHAT_RESULT ����
PACKET_CW_CLOSE

// �Ϲ� ä�� ��ε�ĳ��Ʈ
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_NORMAL_BRD)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };

	char						m_ptszCharName[ MAX_CHARNAME_LENGTH+1 ];
	BYTE						m_byMsgLength;
	char						m_ptszChatMsg[ _MAX_CHATMSG_SIZE+1 ];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChatMsg) + sizeof(m_ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChatMsg[0]) * m_byMsgLength);
    }
PACKET_CW_CLOSE

//��ġ�� ��û
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_SHOUT_SYN)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    enum { GENERAL_SHOUT = 1, WORLD_SHOUT, };

    BYTE m_byMessageType;
    BYTE m_byMsgLength; // _tcslen(m_pszChatMsg)
    char m_pszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszChatMsg) + sizeof(m_pszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszChatMsg[0]) * m_byMsgLength);
    };
PACKET_CW_CLOSE;

// ��ġ�� ��ε�ĳ��Ʈ
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_SHOUT_BRD)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    enum { GENERAL_SHOUT = 1, DECREE_SHOUT, SUPER_MASTER_SHOUT, };

    BYTE m_byMessageType;

    char m_szCharName[MAX_CHARNAME_LENGTH + 1];
    BYTE m_byMsgLength; //_tcslen(m_pszChatMsg)
    char m_pszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_pszChatMsg) + sizeof(m_pszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_pszChatMsg[0]) * m_byMsgLength);
    };
PACKET_CW_CLOSE;

// ��ġ�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_SHOUT_NAK )
//	enum
//	{
//		_ERR_DEFAULT = 0,
//		_ERR_CHATBLOCKED,	// ä�ñ��� ����
//		_ERR_ALREADYSHOUT,	// ��ġ�� �ð� ���� ������ �ٽ� �õ�
//	};
	WORD						m_wErrorCode;		// RC::eCHAT_RESULT ����
PACKET_CW_CLOSE

// ��Ƽ ä�� ��û
PACKET_CW_FWD_OPEN( CW_CHAT, CW_CHAT_PARTY_SYN )
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };

    BYTE m_byMsgLength; // _tcslen(m_ptszChatMsg)
    char m_ptszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChatMsg) + sizeof(m_ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChatMsg[0]) * m_byMsgLength);
    };
PACKET_CW_FWD_CLOSE;

// ��Ƽ ä�� ��ε�ĳ��Ʈ
PACKET_CW_FWD_OPEN(CW_CHAT, CW_CHAT_PARTY_BRD)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };

    char m_ptszCharName[MAX_CHARNAME_LENGTH + 1];
    BYTE m_byMsgLength; // _tcslen(m_ptszChatMsg)
    char m_ptszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChatMsg) + sizeof(m_ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChatMsg[0]) * m_byMsgLength);
    };
};

// ���� ä�� ��û
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_SYN )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// ��� ĳ����
	char						m_ptszAccountID[ID_MAX_LEN+1];				// ���� ���� ID
PACKET_CW_CLOSE

// ���� ä�� ��û����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// ��� ĳ����
	char						m_ptszAccountID[ID_MAX_LEN+1];				// ��� ���� ID
PACKET_CW_CLOSE

// ���� ä�� ��û ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_NAK )
	enum
	{
		_ERR_DEFAULT,		// �� �� ���� ����
		_ERR_BLOCKVOICE,	// ��밡 ����ä�� ���� ����
		_ERR_NO_CHARNAME,	// ��� ĳ���Ͱ� ���ų� �������� �ʾ���
	};

	WORD						wErrorCode;
PACKET_CW_CLOSE

// ���� ä�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_SYN )
	char						m_ptszAccountID[ID_MAX_LEN+1];				// ���� ���� ID
PACKET_CW_CLOSE

// ���� ä�� ��������
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// ��� ĳ����
	char						m_ptszAccountID[ID_MAX_LEN+1];				// ��� ���� ID
PACKET_CW_CLOSE

// ���� ä�� ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_NAK )
	enum
	{
		_ERR_DEFAULT,		// �� �� ���� ����
		_ERR_NO_CHARNAME,	// ��� ĳ���Ͱ� ���ų� �������� �ʾ���
	};

	WORD						wErrorCode;
PACKET_CW_CLOSE

// ���� ä�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REJECT_SYN )
	enum
	{
		_REASON_DEFAULT,			// ����
		_REASON_VOICE_CHAT,			// 1:1 ����ä����..
		_REASON_PARTY_VOICE_CHAT,	// ��Ƽ ����ä����
		_REASON_NOT_USE,			// ����ä�� ��� ����
		_REASON_NOT_ALLOW,			// ���� ��밡 �ƴϴ�
		_REASON_BUSY,				// ��û/���� ������̶� ����
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// ���� ä�� ��������
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REJECT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];

	enum
	{
		_REASON_DEFAULT,			// ����
		_REASON_VOICE_CHAT,			// 1:1 ����ä����..
		_REASON_PARTY_VOICE_CHAT,	// ��Ƽ ����ä����
		_REASON_NOT_USE,			// ����ä�� ��� ����
		_REASON_NOT_ALLOW,			// ���� ��밡 �ƴϴ�
		_REASON_BUSY,				// ��û/���� ������̶� ����
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// ���� ä�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_SYN )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ���
	
	enum
	{
		_REASON_DEFAULT,			// ����ä�� ����
		_REASON_PARTY_VOICE_CHAT,	// ��Ƽ ������ ���� ����
		_REASON_LOGOUT,				// �α׾ƿ�
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// ���� ä�� ����Ǿ���
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ���

	enum
	{
		_REASON_DEFAULT,			// ����ä�� ����
		_REASON_PARTY_VOICE_CHAT,	// ��Ƽ ������ ���� ����
		_REASON_LOGOUT,				// �α׾ƿ�
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// ���� ä�� ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_NAK )
	enum
	{
		_ERR_DEFAULT,		// �� �� ���� ����
		_ERR_NOTCHAT,		// ���� ä�� �� �ƴϾ���
		_ERR_NO_CHARNAME,	// ��� ĳ���Ͱ� ���ų� �������� �ʾ���
	};
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ���
	WORD						wErrorCode;
PACKET_CW_CLOSE


// ���� ä�� ����ũ ON/OFF  => ��Ƽ������ ���¸� �����ؾ� �մϴ�.
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_SYN )
	BOOL						bOn;	
PACKET_CW_CLOSE

// ���� ä�� ON/OFF ��ε�
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ���
	BOOL						bOn;	
PACKET_CW_CLOSE

// ���� ä�� ON/OFF ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_NAK )
	enum
	{
		_ERR_DEFAULT,		// �� �� ���� ����
	};
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ���
	WORD						wErrorCode;
PACKET_CW_CLOSE

// ��Ƽ �������� �� ���� ��Ƽ���� ���� ä�� ���� ��û
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_REQ )
PACKET_CW_CLOSE

// ��Ƽ �������� �� ���� ��Ƽ���� ���� ä�� ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_ANS )
	enum { _MAX_PARTYMEMBER_NUM = 9, };	// �� ��Ƽ�� 10���̹Ƿ� �ִ� ���� ��Ƽ���� 9��

	BYTE					m_byMemberNum;
	STRUCT_VOICE_STATUS		m_sMemberStatus[_MAX_PARTYMEMBER_NUM];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PARTY_MIC_STATUS_ANS) - ( sizeof(STRUCT_VOICE_STATUS) * ( _MAX_PARTYMEMBER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// ��Ƽ �������� �� ���� ��Ƽ���� ���� ä�� ���� ���� ����
// �� ��� ��Ƽ�� ���ų� ��Ƽ ������ ���� ���� ���� ������
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_NAK )
PACKET_CW_CLOSE

// �ٸ� ĳ���� ���� ä�� on/off ���� Ȯ�� ��û(C->W)
// CW_CHAT_PARTY_MIC_STATUS_NAK �޾��� �� ���
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_REQ )
	enum { _MAX_PLAYER_NUM = 9, };

	BYTE				m_byMemberNum;
	char				m_ptszMemberStatus[_MAX_PLAYER_NUM][MAX_CHARNAME_LENGTH+1];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ) - ( (sizeof(char) * (MAX_CHARNAME_LENGTH+1)) * ( _MAX_PLAYER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// �ٸ� ĳ���� ���� ä�� on/off ���� Ȯ�� ����(C->W)
// �� �� �����Ѵٸ� �׳� MemberNum�� 0���� ������
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_ANS )
	enum { _MAX_PLAYER_NUM = 9, };

	BYTE					m_byMemberNum;
	STRUCT_VOICE_STATUS		m_sMemberStatus[_MAX_PLAYER_NUM];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PLAYER_MIC_STATUS_ANS) - ( sizeof(STRUCT_VOICE_STATUS) * ( _MAX_PLAYER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// ä�� ���� �뺸
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_BLOCKED_CMD )
	DWORD					m_dwRemainTime;		// ä�� ���� ���� �ð�(�� ����)
PACKET_CW_CLOSE

// ä�� ä�� ����
// ä�� ä�� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN )
    BYTE m_byRoomNameLength; // ���� ����, without null terminated character
    char m_ptszRoomName[MAX_ROOMTITLE_LENGTH + 1]; // ����

    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CW_CLOSE

// ä�� ä�� ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_ACK )
	BOOL	m_bCreated;						// ������ ���̸� TRUE, ������ ���̸� FALSE
	BYTE	m_byUserNum;					// ���� ��ȭ�濡 �������� ���� ����
PACKET_CW_CLOSE

// ä�� ä�� ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT ����
PACKET_CW_CLOSE

// ä�� ä�� ���� �˸�
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_BRD )
	BYTE	m_byChannelID;								// ���� ������� ä�� �ε���
	BYTE	m_byUserNum;					// ���� ��ȭ�濡 �������� ���� ����
	char	m_ptszJoinMember[MAX_CHARNAME_LENGTH+1];	// ���� �����
PACKET_CW_CLOSE

// ä�� ä�� ���� ����
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN)
    BYTE m_byChatLength; // ä�� ����, _tcslen(m_ptszChat)
    char m_ptszChat[MAX_CHAT_LEN + 1]; // ä�� ����

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChat) + sizeof(m_ptszChat[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChat[0]) * m_byChatLength);
    };
PACKET_CW_CLOSE;

// ä�� ä�� ���� ��ε�
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_CHANNEL_SEND_BRD)
    //enum { _MAX_CHATMSG_SIZE = 255 };
    BYTE m_byChannelID;                          // ���� ���� ä�� �ε���
    char m_ptszCharName[MAX_CHARNAME_LENGTH + 1];  // ���� ��
    BYTE m_byChatLength;                         // ä�� ����, _tcslen(m_ptszChat)
    char m_ptszChat[MAX_CHAT_LEN + 1];             // ä�� ����

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChat) + sizeof(m_ptszChat[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChat[0]) * m_byChatLength);
    };
PACKET_CW_CLOSE;

// ä�� ä�� ���� ������ ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_SEND_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT ����
PACKET_CW_CLOSE

// ä�� ä�� �ʴ�
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN )
	char	m_ptszInviteName[MAX_CHARNAME_LENGTH+1];	// �ʴ��� ���
PACKET_CW_CLOSE

// �ʴ� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_ACK )
PACKET_CW_CLOSE

// �ʴ� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK )
	BYTE		m_byErrorCode;
    // added by _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL = {
    // Ŭ��->���� : �ʴ� ��û�� �̸�
    // ����->Ŭ�� : �ʴ� ����� �̸�
    TCHAR invite_target_name_[MAX_CHARNAME_LENGTH+1];
    // }
PACKET_CW_CLOSE

// �ʴ� �޾���
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_CMD )
	char	m_ptszInviteName[MAX_CHARNAME_LENGTH+1];	// ���� �ʴ��� ���
	BYTE	m_byRoomNameLength;							// ���� ����
	char	m_ptszRoomName[MAX_ROOMTITLE_LENGTH+1];
    uint16_t GetSize() const
    {
        return sizeof(*this);
    };
PACKET_CW_CLOSE

// ä�� ä�ù� ���� ��û
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN )
PACKET_CW_CLOSE

// ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_ACK )
PACKET_CW_CLOSE

// ���� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT ����
PACKET_CW_CLOSE

// ������ �˸�
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_BRD )
	BYTE	m_byUserNum;							// ���� ��ȭ�濡 �������� ���� ����
	char	m_ptszOutName[MAX_CHARNAME_LENGTH+1];	// ���� ��� �̸�
PACKET_CW_CLOSE

// ä��ä�ù� ���� ��û(�ο�, �̸�)
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ )
PACKET_CW_CLOSE

// ä��ä�ù� ����
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_ANS )
	BYTE	m_byUserNum;	// ���� �������� ���� ����
	char	m_ptszCharName[MAX_CHATZONE_USER][MAX_CHARNAME_LENGTH+1];	// ���� �������� �ο� ����

	WORD GetSize() { return ( sizeof(MSG_CW_CHAT_CHANNEL_INFO_ANS) - ( MAX_CHATZONE_USER - m_byUserNum ) ); }
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_NAK )
	BYTE	m_byErrorCode;
PACKET_CW_CLOSE

////////////////////////////////////
// GM ��ɾ�

// GM ���� ���
PACKET_CW_OPEN(CW_GM, CW_GM_NOTICE_SYN)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    enum
    {
        NOTICE_WORLD = 0,   // �ڽ��� ���� ���� ��ü ����
        NOTICE_CHANNEL, // �ڽ��� ���� ä�� ��ü ����
        NOTICE_ZONE,    // �ڱⰡ ���� �ִ� �������� ����
    };

	BYTE		byNoticeType;					// ���� ����
	DWORD		dwNoticeLength;					//  ���� ���ڿ��� ����
	char		szNotice[_MAX_CHATMSG_SIZE+1];	//  ���� ���ڿ�

	int		GetSize() { return sizeof(MSG_CW_GM_NOTICE_SYN); }	// ���� ��Ŷ �ƴ�
	//WORD		GetSize() { return sizeof(MSG_CW_GM_NOTICE_SYN) - ( MAX_NOTICE_LEN - byNoticeLen ); }
};

// GM ���� ����
PACKET_CW_OPEN( CW_GM, CW_GM_NOTICE_NAK )
	enum
	{
		ERR_NOTICE_DEFAULT,
	};

	DWORD		dwResultCode;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_STRING_CMD_SYN )
	enum { MAX_STRING_CMD_LENGTH = 100 };
	char				m_szStringCmd[MAX_STRING_CMD_LENGTH];	// ��Ʈ�� ��ɾ�
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_STRING_CMD_NAK )
	BYTE				m_byErrorCode;								// eGM_RESULT
PACKET_CW_CLOSE

// GM �Ӹ� ��/�� ���
PACKET_CW_OPEN( CW_GM, CW_GM_WHISPER_SET_ACK )
	BOOL				m_bOn;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_WHISPER_SET_NAK )
PACKET_CW_CLOSE

// ���� ä�ñ���/��� ���
PACKET_CW_OPEN( CW_GM, CW_GM_USERCHAT_BLOCK_ACK )
	CHARGUID			m_CharGuid;
	BOOL				m_bBlock;
	DWORD				m_dwBlockTime;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_USERCHAT_BLOCK_NAK )
	enum
	{
		_ERR_DEFAULT = 0,
		_ERR_NOTCONNUSER,	// �������� �ƴ� ����� - WOPS�� ���� ó���ؾ� �Ѵ�
		_ERR_BLOCKED,		// �̹� ä�ñ����� �����
		_ERR_NOTBLOCK,		// �̹� ä������ �����
	};
	WORD				m_wErrorCode;
PACKET_CW_CLOSE

//////////////////////////////////
// ����Ʈ ���� ��Ŷ

struct MSG_CW_VIEWPORT_CHARSTATE : public MSG_BASE_FORWARD
{
	DWORD			dwSectorIndex;		// ���� ��ġ�� ���� �ε���
	DWORD			dwFieldCode;		// ���� ��ġ�� ���� �ʵ� �ڵ�

	int GetSize()		{ return sizeof(MSG_CW_VIEWPORT_CHARSTATE); }
};

//////////////////////////////////
// Armorkit ���� ��Ŷ

struct MSG_CW_ARMORKIT_FINDHACK_SYN : public MSG_BASE
{
	MSG_CW_ARMORKIT_FINDHACK_SYN()	{ m_byCategory = CW_ARMORKIT; m_byProtocol = CW_ARMORKIT_FINDHACK; }

	int		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
	int		Size;			// Buffer �� ����
	char	Buf[1024];		// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)

	int GetSize()		{ return sizeof(MSG_CW_ARMORKIT_FINDHACK_SYN); }
};

//////////////////////////////////
//////////////////////////////////
// ģ�� ���� ��Ŷ

struct STRUCT_FRIEND_STAT	// ģ�� ���� ����ü
{
	char			ptszFriendName[MAX_CHARNAME_LENGTH+1];		// ģ�� �̸�
	BOOL			bOnline;									// �¶����̸� TRUE, ���������̸� FALSE
};

// ó�� ������ �� ģ�� ��� ��û
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_NEWLIST_SYN )
	int GetSize()		{ return sizeof(MSG_CW_FRIEND_NEWLIST_SYN); }
PACKET_CW_CLOSE

// ó�� ������ �� ģ�� ��� ����(����)
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_FRIENDLIST_ACK)
	BYTE				byFriendStatNum;						// ģ�� ����
	STRUCT_FRIEND_INFO	sFriendStat[MAX_FRIENDBLOCK_STAT_NUM];	// ģ������ ����(���ܵ� ����)

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sFriendStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sFriendStat[0]) * byFriendStatNum);
    };
PACKET_CW_CLOSE;

// ó�� ������ �� ���� ��� ����(����)
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_BLOCKLIST_ACK)
	BYTE				byBlockStatNum;						// ���ܵ� ĳ���� ����
	STRUCT_BLOCK_INFO	sBlockStat[MAX_BLOCK_STAT_NUM];		// ĳ���͵��� ����

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sBlockStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sBlockStat[0]) * byBlockStatNum);
    };
PACKET_CW_CLOSE;

// ó�� ������ �� ģ�� ��� ��û ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_NEWLIST_NAK )
	enum
	{
		ERR_DEFAULT,
		NO_FRIEND_DATA,		// ������ ����
	};

	DWORD				dwResult;
PACKET_CW_CLOSE

// ���� �߿� ģ�� �α׿�
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_LOGIN_BRD )
//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
//	CHARGUID loginCharGuid;
//#endif
	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
	BYTE				byChannelID;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_LOGOUT_BRD )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID logoutCharGuid;
	//#endif
	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

/*
struct MSG_CW_FRIEND_CHGSTATE_BRD : public MSG_BASE
{
	MSG_CW_FRIEND_CHGSTATE_BRD() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHGSTATE_BRD; }

	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
	LEVELTYPE			Level;
	BYTE				byClass;
};
*/
// ģ�� �߰�
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_SYN )
	char				ptszCharNameTo[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_ACK )
	STRUCT_FRIEND_INFO	sFriendStat;	// �ش� ģ���� ����
PACKET_CW_CLOSE

// ģ�� ��û �Ұ� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_NAK )

	enum
	{
		ERR_DEFAULT,	// ��Ÿ ���
		NO_USER,		// �ش��ϴ� ������ ����
		ALREADY_FRIEND,	// �̹� ģ�� ����
		FRIEND_IS_ME,	// ���� ģ�� ��û����
#ifdef _FRIEND_OVERFLOW
		FRIEND_OVERFLOW,	//ģ�� ���� ���� �ʰ�(90��)
#endif
	};

	BYTE				m_byResult;
PACKET_CW_CLOSE

// ģ�� ���� ��û
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_SYN )
	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ģ�� ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_ACK )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID deleteBuddyIndex;
	//#endif
	char				ptszDeletedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ģ�� ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_NAK )
	enum
	{
		ERR_DEFAULT,			// �� �� ���� ����
		NOT_FRIEND_USER,		// ģ�� �ƴ�
	};

	BYTE				m_byResult;
PACKET_CW_CLOSE

// ���� ��û
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_SYN )
	char				ptszBlockName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_ACK )
	char				ptszBlockedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_NAK )
	enum
	{
		ERR_DEFAULT,		// �� �� ���� ����
		NO_USER,			// �������� �ʴ� ĳ���� �̸�
		BLOCKED_ME,			// �ڽ��� �����Ϸ��� �õ�
		ALREADY_BLOCKED,	// �̹� ��ϵ� ����
#ifdef _FRIEND_OVERFLOW
		BLOCK_OVERFLOW,		// ���� �ִ� ���� �ʰ�
#endif
	};

	DWORD				m_byResult;
PACKET_CW_CLOSE

// ���� ���� ��û
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN )
	char				ptszBlockName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ���� ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_ACK )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID blockFreeCharGuid;
	//	CHARGUID FreeCharOnlineState;
	//#endif
	char				ptszBlockedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ���� ���� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_NAK )
	enum
	{
		ERR_DEFAULT,		// �� �� ���� ����
		NO_USER,			// �������� �ʴ� ĳ���� �̸�
		NOT_BLOCKED,		// ���ܵ� ĳ���Ͱ� �ƴ�
	};

	DWORD				m_byResult;
PACKET_CW_CLOSE

// ģ�� ä��
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_CHAT_SYN)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };

    char ptszFriendNameTo[MAX_CHARNAME_LENGTH + 1];
    BYTE byMsgLength; // _tcslen(ptszChatMsg)
    char ptszChatMsg[_MAX_CHATMSG_SIZE + 1];
    // NOTE: f110907.2L, the client send message with total size.
    // NOTE: f110915.1L, added
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(ptszChatMsg) + sizeof(ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(ptszChatMsg[0]) * byMsgLength);
    };
PACKET_CW_CLOSE;

// ģ�� ä�� ����
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_CHAT_BRD)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    char ptszFriendNameFrom[MAX_CHARNAME_LENGTH + 1];
    BYTE byMsgLength; // _tcslen(ptszChatMsg)
    char ptszChatMsg[_MAX_CHATMSG_SIZE + 1];

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(ptszChatMsg) + sizeof(ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(ptszChatMsg[0]) * byMsgLength);
    };
PACKET_CW_CLOSE;

// ģ�� ä�� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHAT_NAK )
	enum
	{
		ERR_DEFAULT,	// �� �� ���� ����
		NOT_FRIEND,		// ģ�� �ƴ�
		OFFLINE_FRIEND,	// ģ���� �������� ����
		BLOCKED_FRIEND,	// ����� ���ܵ� ģ���Դϴ�.-0-/
		BLOCK_CHAT,		// ģ���� ä�� �ź� ����
	};

	char				ptszFriendNameTo[MAX_CHARNAME_LENGTH+1];	// �������� ������ ä���̾���?!
	DWORD				m_byResult;
PACKET_CW_CLOSE

// ģ�� ä�� �ź� üũ(��� ���)
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN )
	BOOL				m_bBlock;	// 0: ��� 1: �ź�
PACKET_CW_CLOSE

// ģ�� ä�� �ź� ��� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_ACK )
	BOOL		m_bNowBlock;		// ��������� �ź� ������ TRUE, ��� ������ FALSE
PACKET_CW_CLOSE

// ģ�� ä�� �ź� ��� ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_NAK )
	enum { ERR_DEFAULT = 0, };

	DWORD		dwResult;		// ����
PACKET_CW_CLOSE

//ģ�� ��õ ����� �Ϲ�ģ���� ���°� ����� ����Ʈ, _KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_CW_OPEN( CW_FRIEND, CW_FRIENDINVITATE_INVITATELIST_CMD )
    INVITATEFRIENDLIST_INFO m_InvitateFriendInfo;
	WORD GetSize() const
    {   // (CHANGES) (f100624.2L)
        const WORD kMinSize = sizeof(*this) - sizeof(m_InvitateFriendInfo);
        return static_cast<WORD>(kMinSize + m_InvitateFriendInfo.GetSize());
		//return ( sizeof(MSG_CW_FRIENDINVITATE_INVITATELIST_CMD) - ( (MAX_REVENGE_STAT_NUM - m_InvitateFriendInfo.m_byListCount) * sizeof(DWORD) ) );
	}
PACKET_CW_CLOSE
/*
// ģ�� ä�� �ʴ�(3�� �̻��� ���)
struct MSG_CW_FRIEND_CHAT_INVITE_SYN : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_SYN() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_SYN; }

	char				ptszFriendNameTo[MAX_CHARNAME_LENGTH+1];
};

// ģ�� �ʴ� �뺸
struct MSG_CW_FRIEND_CHAT_INVITE_BRD : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_BRD() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_BRD; }

	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
};

// ģ�� �ʴ� ����
struct MSG_CW_FRIEND_CHAT_INVITE_NAK : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_NAK() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_NAK; }
	enum
	{
		ERR_DEFAULT,	// �� �� ���� ����
		NOT_FRIEND,		// ģ�� �ƴ�
		OFFLINE_FRIEND,	// ģ���� �������� ����
		BLOCKED_FRIEND,	// �� ���ܴ��ߴ�
	};

	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
};
*/

// ģ�� ������ �˸�
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_LEVELUP_BRD )
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	LEVELTYPE	m_Level;
    void Clear() {
        const size_t kClearSize = static_cast<size_t>((char*)(this + 1) - m_ptszCharName);
        ZeroMemory(m_ptszCharName, kClearSize);
    };
PACKET_CW_CLOSE

// ģ�� ������ ����
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_REQ )
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    CHARGUID    m_dwToCharGuid;                       
#else
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
#endif
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_ANS )
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byChannel;
	BYTE		m_byZoneType;							// ���� ��ġ(����/�ʵ�/�κ�...), eZONETYPE ����
	MAPCODE		m_MapCode;								// ���� �� �ڵ�
	DWORD		m_dwZoneKey;							// ���� ���� �ε���(��Ʋ�� ��ȣ�� ��� ����)
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	// �Ҽӵ� ����(������ 0)
	BYTE		m_byDuty;								// ��尡 ���� ��� ��忡���� ����. 
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    TCHAR		m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
#endif
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_NAK )
	BYTE		m_byErrorCode;							// �����ڵ�(ResultCode ����)
PACKET_CW_CLOSE

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_MEMO_UPDATE_SYN )
    CHARGUID    m_dwToCharGuid;
    TCHAR		m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_MEMO_UPDATE_ACK )
    CHARGUID    m_dwToCharGuid;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_MEMO_UPDATE_NAK )
	BYTE		m_byErrorCode;							// �����ڵ�(ResultCode ����)
PACKET_CW_CLOSE



// ģ���� ���� �����ϱ� - '�����ϱ�' ��ư ������ �� �ֽ� ������ ������ ���� ����Ѵ�
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_JOIN_SYN )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID m_dwToCharGuid;
	//#endif
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_JOIN_ACK )
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byZoneType;
	DWORD		m_dwZoneKey;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_JOIN_NAK )
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byZoneType;
	DWORD		m_dwZoneKey;
	BYTE		m_byErrorCode;
PACKET_CW_CLOSE


//////////////////////////////
// ��� ���� ��������

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_CHAT_SYN)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };

    GUILDGUID   m_GuildGuid;
    BYTE        m_byMsgLen; // _tcslen(m_ptszCharMsg)
    char        m_ptszCharMsg[_MAX_CHATMSG_SIZE + 1];
    // WARNING: f110826.1L, not null terminated string
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszCharMsg) + sizeof(m_ptszCharMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszCharMsg[0]) * m_byMsgLen);
    };
PACKET_CW_CLOSE;

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_CHAT_BRD)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    //
    char ptszCharName[MAX_CHARNAME_LENGTH + 1]; // ���� ���
    BYTE byMsgLen; // _tcslen(m_ptszCharMsg)
    char ptszChatMsg[_MAX_CHATMSG_SIZE + 1];
    // WARNING: f110826.1L, m_byMsgLen include the null character (strlen(ptszChatMsg) + 1)
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(ptszChatMsg) + sizeof(ptszChatMsg[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(ptszChatMsg[0]) * byMsgLen);
    };
PACKET_CW_CLOSE;

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_CHAT_NAK )
	enum
	{
		ERR_DEFAULT,
		ERR_NOT_GUILD,	// �׷� ��� ����(������ Ŭ���� GuildGuid �ٸ�)
		ERR_NOT_MEMBER,	// ��� ��� �ƴϴ�
		ERR_WAIT_INFO,	// ������ �����Ƿ� ��弭���� ��û ����(��� �� �ٽ� �õ��Ͻʽÿ�)
	};

	BYTE		byResult;
PACKET_CW_CLOSE

// ��� ����
PACKET_CW_OPEN( CW_GUILD, CW_GUILD_SELECT_NOTICE_SYN )
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
	BYTE m_NoticeType;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_SELECT_NOTICE_NAK )
	DWORD m_dwErrorCode;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_SELECT_NOTICE_ACK )
	BYTE m_NoticeType;
	WORD m_wNoticeLen;
	char	 m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1];
    uint16_t GetSize() const
    {   //                                                              with null char size
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszNotice) + sizeof(m_ptszNotice[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszNotice[0]) * m_wNoticeLen);
    };
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_CHANGE_NOTICE_SYN )
	WORD		m_byNoticeLen;
	char		m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH+1];		
    uint16_t GetSize() const
    {   //                                                                  with null char size
        const size_t kMinSize = sizeof(*this) - sizeof(m_tszGuildNotice) + sizeof(m_tszGuildNotice[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_tszGuildNotice[0]) * m_byNoticeLen);
    };
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_CHANGE_NOTICE_BRD )
	WORD			m_wNoticeLen; // _tcslen(m_ptszNotice)
	char			m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1];
    uint16_t GetSize() const
    {   //                                                              with null char size
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszNotice) + sizeof(m_ptszNotice[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszNotice[0]) * m_wNoticeLen);
    };
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_CHANGE_NOTICE_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE


PACKET_CW_OPEN( CW_GUILD, CW_GUILD_NOTICE_CMD )
	WORD			m_wNoticeLen; // _tcslen(m_ptszNotice)
	char			m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszNotice) + sizeof(m_ptszNotice[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszNotice[0]) * m_wNoticeLen);
    };
PACKET_CW_CLOSE

// <Cryptography>
// ��Ŷ ��ȣȭ Ű ��ȯ �� �ֱ��� �˰��� ��ȯ
PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_KEY )
	BYTE						m_bySN;			// Sequence Number
	BYTE						m_byCryptInfo;	// ���ڵ��� ��Ŷ ��ȣȭŰ ��� Ǯ�� ���� Ű
	union {
		DWORD					m_dwUserKey;
		BYTE					m_byCryptData[200];	// �˰��� ���� (Ű, ���...)
	};
	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCryptInfo() {	return m_byCryptInfo;	}
	VOID						SetCryptInfo( BYTE byCryptInfo ) {	m_byCryptInfo = byCryptInfo;	}
	BYTE*						GetCryptStream() {	return m_byCryptData;	}
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_ACK )
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_NAK )
PACKET_CW_CLOSE

// ��ȣȭ�� ��Ŷ Ǯ������...
struct MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED {
	BYTE						m_bySN;			// Sequence Number : received
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// ��Ŷ ��ȣȭ �ϱ� ����...
struct MSG_CW_CRYPTOGRAPHY_PACKET_ENCODING {
	BYTE						m_bySN;			// Sequence Number : sending
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

// ��ȣȭ ���� ����ض�!
PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_CMD )
PACKET_CW_CLOSE

// �ٸ� ������ UserID ��û(�� ���� ����)
PACKET_CW_OPEN( CW_COMMUNITY, CW_COMM_USERID_REQ )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_COMMUNITY, CW_COMM_USERID_ACK )
	char						m_ptszAccountID[ID_MAX_LEN+1];
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_COMMUNITY, CW_COMM_USERID_NAK )
	enum
	{
		_ERR_DEFAULT,
		_ERR_NOTCONNUSER,			// ���� ���� �ƴ� ����� ���� ��û
	};

	WORD						m_wErrorCode;
PACKET_CW_CLOSE

//////////////////////////////////
//////////////////////////////////
// ������(����ģ��) ���� ��Ŷ

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_ADD_CMD )
	BYTE						m_byRevengePoint;	// Remain Revenge Point
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    STRUCT_REVENGE_INFO         m_sRevengeStat;
#else
    STRUCT_REVENGE_PACKET_INFO	m_sRevengeStat;
#endif
PACKET_CW_CLOSE

// ������ ����Ʈ���� ����
PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_DEL_SYN )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID m_RevengeGuid;
	//#endif
	char			m_ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_DEL_ACK )
	char			m_ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_DEL_NAK )
	char			m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// ������� ���� - ���� ����
	BYTE			m_byErrorCode;	// RC::eREVENGE_RESULT ����
PACKET_CW_CLOSE

// ó�� ������ �� ������ ����Ʈ ����(����) - ģ�� ����Ʈ ��û���� �� �Բ� ����
PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LIST_CMD )
	BYTE						byRevengePoint;						// ������ ����Ʈ
    // CHANGES: f111004.2L, changes default type, this is changed by _NA002676_WORLD_SERVER_RENEWAL
    uint8_t byRevengeStatNum; // ���� ��� ĳ���� ����, 
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO 
    STRUCT_REVENGE_INFO         sRevengeStat[MAX_REVENGE_STAT_NUM];
#else
	STRUCT_REVENGE_PACKET_INFO	sRevengeStat[MAX_REVENGE_STAT_NUM];	// ĳ���͵��� ����(�ӽ÷� ģ�� ������ �����ϰ�)
#endif
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sRevengeStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sRevengeStat[0]) * byRevengeStatNum);
    };
PACKET_CW_CLOSE

// ������ ��� �α���
PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LOGIN_BRD )
	char				m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		byChannelID;							// �������� ä��
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LOGOUT_BRD )
	char				ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// ������ ���  ������ �˸�
PACKET_CW_INHERIT_OPEN(CW_REVENGE, CW_REVENGE_LEVELUP_BRD, MSG_CW_FRIEND_LEVELUP_BRD)
    void Clear() {
        // NOTE: first configurable data field = 'MSG_CW_FRIEND_LEVELUP_BRD::m_ptszCharName'
        const size_t kClearSize = static_cast<size_t>((char*)(this + 1) - m_ptszCharName);
        ZeroMemory(m_ptszCharName, kClearSize);
    };
PACKET_CW_INHERIT_CLOSE;

//////////////////////////////////
//////////////////////////////////
// ���� ���� ��Ŷ

// ���� ����
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_SYN )
	char				m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	char				m_ptszMemo[MAX_MEMO_LENGTH + 1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_ACK )
	char				m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_NAK )
	BYTE				m_byErrorCode;							// RC::eMEMO_RESULT ����
PACKET_CW_CLOSE

// ���� ���
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_REQ )
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_ANS )
	BYTE			m_byMemoCnt;								// ���� ����(���� ����������)
	BYTE			m_byMemoPage;								// ���� ����� ���� ������(20�� ������ ����)
	BYTE			m_byMemoPageCnt;							// ���� ����� �� ������(���� ���� 123���� 7�������� ��. m_byMemoPage=m_byMemoPageCnt�� �۽� �Ϸ�)
	STRUCT_SUBJECT_PACKETINFO	m_Subject[MAX_MEMO_PAGENUM];	// ���� ���� �о��� ǥ�ø� ����
	WORD	GetSize()
	{
		return ( sizeof(MSG_CW_MEMO_LIST_ANS) - ( sizeof(STRUCT_SUBJECT_PACKETINFO) * (MAX_MEMO_PAGENUM - m_byMemoCnt) ) );
	}
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE

// ���� ���� Ȯ��
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_REQ )
	MEMOGUID		m_MemoGuid;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_ANS )
	MEMOGUID		    m_MemoGuid;
	char			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//ȯ�� ���� ������ �ڵ�
    ULONGLONG           m_amountCost;//ȯ�� ���� �ݾ�
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//�ý��� �޸� ������
    // }
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE

// ���� ����
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_DELETE_SYN )
	MEMOGUID		m_MemoGuid;
    STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE m_MemoType; //_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_DELETE_ACK )
	MEMOGUID		m_MemoGuid;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_DELETE_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE

// �� ���� �˸�
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_NEWMEMO_BRD )
	STRUCT_SUBJECT_PACKETINFO	m_Subject;	// ���� ��� - Ŭ���Ͽ� Ȯ���ϸ� �ٷ� CW_MEMO_VIEW_REQ �����ϸ� ��
PACKET_CW_CLOSE

//��� ����
PACKET_CW_OPEN( CW_GUILD, CW_GUILD_RELATION_BRD )
	GUILD_RELATION m_stRELGuild;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_RELATION_DELETE_BRD )
	eGUILD_RELATION	m_eRelType;
	GUILDGUID m_RelGuildGuid;
	CHAR m_szGuildName[MAX_GUILDNAME_LENGTH + 1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_RELATION_BOTH_AGAINST_BRD )
	GUILDGUID	m_BothGuildGuid;
	eGUILD_RELATION	m_eDELType;
	eGUILD_RELATION	m_eMOVType;
	CHAR m_tszBothGuildName[MAX_GUILDNAME_LENGTH + 1];	
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GUILD, CW_GUILD_RELATION_MOVE_BRD)
	GUILDGUID m_GuildGuid;
	eGUILD_RELATION	m_eDELType;
	eGUILD_RELATION	m_eMOVType;
	CHAR m_szGuildName[MAX_GUILDNAME_LENGTH + 1];
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_RECOVERY_GUILD_BRD)
	GUILDGUID m_GuildIndex;
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_GUILDMARK_REGISTER_BRD)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_GUILDMARK_DELETE_BRD)
	GUILDGUID m_GuildIndex;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_CW_CLOSE

// ��� â�� �α�
PACKET_CW_OPEN(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ )
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_ANS )
	BYTE	m_byLogCount;	 // �α� ����(���� ����������)
	BYTE	m_byCurrentPage; // ���� �Ǵ� ���� ������
	BYTE	m_byLogPageCount;	// �α� ����� �� ������(���� �α� 123���� 7�������� ��. m_byCurrentPage=m_byLogPageCount�� �۽� �Ϸ�)
	STRUCT_GUILDWAREHOUSE_LOGINFO m_LogList[MAX_GUILDWAREHOUSE_LOGPAGE_NUM];
	WORD GetSize()
	{
		return (sizeof(MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS) - 
			(sizeof(STRUCT_GUILDWAREHOUSE_LOGINFO) 
			* (MAX_GUILDWAREHOUSE_LOGPAGE_NUM - m_byLogCount)));
	}
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_NAK )
	BYTE	 m_byErrorCode;
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_DOMINATION_RETURN_HEIM_CMD)
    TCHAR       m_ptszMemo[MAX_MEMO_LENGTH + 1];
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//////////////////////////GROUPING SYSTEM/////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�׷� �ý��� �뺸 �޼���
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LOGIN_BRD)                        //���� �߿� �׷�ɹ� �α׿� �뺸
    BYTE byChannel;
	char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LOGOUT_BRD)                       //���� �߿� �׷�ɹ� �α׾ƿ� �뺸
	char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ACCUMEXP_BRD)                     //�α��ν� ���� ����� �׾� ���� ����ġ�� �޾����� �뺸
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD accumEXP;
    eGROUP_STAT checkDATE;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_BONUSMONEY_BRD)                   //�׷� ���ʽ� ���� �뺸
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusMoney;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_BONUSEXP_BRD)                     //�׷� ���ʽ� ����ġ �뺸
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusEXP;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LEVELUP_BRD)                      //���� ��� ������
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    BYTE charLevel;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_BRD)                          //��� ���� �뺸
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_RESTOR_CMD)                       //��� ���� ���û
char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE



//////////////////////////////////////////////////////////////////////////
//�׷� ��� �߰�
//��û
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //��û�޴� �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_ACK)
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    DWORD charGUID;		                                            //�ɹ����� GUID
#endif
    STRUCT_GROUP_MEMBER_PACKET_INFO m_MemberInfo;                   //��û ���� �ɹ� ���� ����ü
    eGROUP_STAT m_GroupInfo;
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_NAK)
    enum
    {
        NONE = 0, // none or not set
        //
        ERR_DEFAULT,                                                // ��Ÿ ���
        NO_USER,                                                    // �ش��ϴ� ������ ����
        ALREADY_GROUP,                                              // �̹� �׷� ����
        NOT_ADD_ME,                                                 // �����θ� ģ�� ��û����
        NOT_MATCHING_LEVEL,                                         // ������ �������� �ʴ�.
        GROUP_OVERFLOW,                                             // �׷� ���� �ʰ�(6��)
        IGNOR_ADD_MEMBER,                                           // ������ �ɹ� ���� ��û �ź�
        RES_TIME_OUT,                                               // ��û �ð� �ʰ�
        ON_TOGGLE_ADDBLOCK,                                         // �ɹ� �߰� �ź� ��� ������
        ALREADY_GROUP_REQ,                                          // �ٸ� �ɹ��κ��� �׷� ��û���̴�.
        BEHAVE_VENDOR_ESTABLISHER,                                  // ���� ������
        //
        MAX_COUNTS // upperbound
    };
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //���� ��û�ϴ� �ɸ��� �̸�, f110811.5L, unused?
    BYTE m_byResult;                                                //��� ��  
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷� ��� �߰� ����
//��û
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //�ڽ��� �ʴ��� �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //�ڽ��� �ʴ��� �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK)
    enum
    {
        ERR_DEFAULT,                                                //�˼� ���� ����
        RES_TIME_OUT,                                               //���� Ÿ�̹��� ������.
        IGNOR_ADD_MEMBER,                                           //������ �ߴ�.
    };
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //�ڽ��� �ʴ��� �ɸ��� �̸�
    BYTE m_byResult;                                                //���� ��� ��
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷� ��� ���� 
//��û
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //������ �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_ACK)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //������ �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_NAK)
	enum
	{
		ERR_DEFAULT,    //�� �� ���� ����
        DB_ERROR,       //DB���� ������ ó�� ����
		NOT_MEMBER_USER,                                            //�׷� �ɹ��� �ƴ�
	};                                              
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //������ �ɸ��� �̸�
   	BYTE m_byResult;                                                //�����           
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷� ��� ����Ʈ ��û
//��û
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_REQ)
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_ANS)
    BYTE byGroupMemberNum;                                          //�ɹ� ��
    static const size_t MAX_RECORDS = MAX_GROUP_MEMBER;
    STRUCT_GROUP_MEMBER_INFO sGroupMemberInfo[MAX_RECORDS];    //�ɹ� ����

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sGroupMemberInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(sGroupMemberInfo[0]) * byGroupMemberNum);
    }

PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_NAK)
    enum
    {
      ERR_DEFAULT,                                                  //�˼����� ����
      NO_MEMBER_DATA,                                               //������ ����
      GROUP_OVERFLOW,                                               //�׷���� �� ����
    };
    DWORD dwResult;                                                 //��� ��           
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷� ��� ������ ��û
//��û
PACKET_CW_OPEN(CW_GROUP,CW_GROUP_STATUS_REQ)
    char m_ptszCharName[MAX_CHARNAME_LENGTH+1];                     //�������� �ɸ��� �̸�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_STATUS_ANS)
    char m_ptszCharName[MAX_CHARNAME_LENGTH+1];                     //�ɸ��� �̸�
    BYTE m_byChannel;                                               //�ɸ��� ��ġ ü��
    BYTE m_byZoneType;							                    //���� ��ġ(����/�ʵ�/�κ�...), eZONETYPE ����
    MAPCODE m_MapCode;								                //���� �� �ڵ�
    DWORD m_dwZoneKey;							                    //���� ���� �ε���(��Ʋ�� ��ȣ�� ��� ����)
    char m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	                //�Ҽӵ� ����(������ 0)
    BYTE m_byDuty;								                    //��尡 ���� ��� ��忡���� ����. 
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_STATUS_NAK)
    enum
    {
        ERR_DEFAULT,                                                //�˼����� ����
        NO_MEMBER_DATA,//������ ����
        NOT_MEMBER,
    };
    DWORD dwResult;                                                 //��� ��       
PACKET_CW_CLOSE
//////////////////////////////////////////////////////////////////////////
//�׷� �ʴ� �ź� ���
//��û
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_SYN )
BOOL m_bBlock;	                                                    // 0: ��� 1: �ź�
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_ACK )
BOOL m_bNowBlock;		                                            //�ź� ������ TRUE, ��� ������ FALSE
PACKET_CW_CLOSE
//����
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_NAK )
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//�׷� �ɹ� ���� ���� ������ �ƴ� ��� �ش� �������� �˷��ش�.
//��û
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_REWARD_FAIL_CMD )
PACKET_CW_CLOSE

//------------------------------------------------------------------------------
// CW_LOTTO : ���� �ζ� ī�װ�
//------------------------------------------------------------------------------

// ���� �ζ� ���� ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_START_BRD)
    WORD wDrawNum;                      // ȸ��
PACKET_CW_CLOSE

// (��) ���� �ζ� ���� ��û
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_SYN)
    BYTE bFlag;                         // Ŭ���̾�Ʈ�� ���� ���� �÷���: �״�� �������� ����
PACKET_CW_CLOSE

// (��) ���� �ζ� ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_ACK)
    BYTE bFlag;                         // ��û���� ���޵� Ŭ���̾�Ʈ�� �÷���
    BYTE bStatus;                       // ���� �ζ� Ȱ��ȭ ���� - 0:�������� �ƴ�(��÷ �Ǵ� ���� ������), 1: ������
    CHARGUID iCharGuid;                 // ĳ���� ID
    WORD wCurrentDrawNum;               // ���� ȸ��
    INT64 i64TotalAccum;                // ���� �� ������
    BYTE bNumTickets;                   // ���� �����ϰ� �ִ� ���� ���� ��
    BYTE bNumResultRecords;             // ���� �� ȸ�� �ζ� ��� �����Ͱ� ���ԵǾ� �ִ���
    LottoResultRecord results[LOTTO_NUM_RECENT_DRAWS - 1]; // ���� ȸ�� ������ ���� ��÷ ���
    BYTE bNumEntryRecords;              // �� ���� ���� �����Ͱ� ���ԵǾ� �ִ��� (����/���� ��� ����)
    LottoEntryRecord entries[LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS]; // ���� ����

    int GetSize() const
    {
        return (sizeof(MSG_CW_LOTTO_INFO_ACK) 
            - (sizeof(LottoEntryRecord) 
            * ((LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS) - bNumEntryRecords)));
    }
PACKET_CW_CLOSE

// (��) ���� �ζ� ���� ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_NAK)
    enum Reason {
        NOT_READY       // ���� �ζǰ� ���� �ʱ�ȭ���� �ʾ� ����� �� ����
      , UNKNOWN
    };
    CHARGUID iCharGuid;                 // ĳ���� ID
    BYTE bReason;                       // ���� ����
PACKET_CW_CLOSE

// ���� �ζ� ���� ȹ�� �˸�
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_TICKET_NTF)
    CHARGUID iCharGuid;                 // ĳ���� ID
    BYTE bNumTickets;                   // ��������� �����ϰ� �ִ� ���� ���� ��
PACKET_CW_CLOSE

// ���� �ζ� ���� ��û
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_SYN)
    WORD wDrawNum;                      // ȸ��
    BYTE bAuto;                         // �ڵ� ����
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
PACKET_CW_CLOSE

// ���� �ζ� ���� ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_ACK)
    CHARGUID iCharGuid;                 // ĳ���� ID
    WORD wDrawNum;                      // ȸ��
    BYTE bNumTickets;                   // �Ҹ� �� ��������� �����ϰ� �ִ� ���� ���� ��
    BYTE bAuto;                         // �ڵ� ����
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    INT64 i64TicketPrice;               // ���ǿ��� ���� ���Խ� ������ ����
#endif
PACKET_CW_CLOSE

// ���� �ζ� ���� ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_NAK)
    enum Reason {
        NOT_READY           // ���� �ζǰ� ���� ���� �ƴ�
      , OUT_OF_TICKET       // ���� �ζ� ���� ���� (_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE ���ÿ��� ���� ���� �ʰ�)
      , NOT_ENOUGH_MONEY    // ���ǿ��� ������ ���� �ܾ� ���� (_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE)
      , UNKNOWN
    };
    CHARGUID iCharGuid;                 // ĳ���� ID
    BYTE bReason;                       // ���� ����
PACKET_CW_CLOSE

// ���� �ζ� ������ ���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ACCUM_BRD)
    WORD wDrawNum;                      // ȸ��
    INT64 i64TotalAccum;                // ���� �� ������
PACKET_CW_CLOSE

// ���� �ζ� ��÷ ī��Ʈ�ٿ� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_COUNTDOWN_BRD)
    WORD wDrawNum;                      // ȸ��
    WORD wSeconds;                      // ��÷���� ���� �� ��
PACKET_CW_CLOSE

// ���� �ζ� ��÷/���� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_FINISH_BRD)
    WORD wDrawNum;                      // ȸ��
    BYTE bWinningNum[LOTTO_NUM_PICKS];  // ��÷ ��ȣ
PACKET_CW_CLOSE

// ��÷ �ȳ� ���� ���� �˸�
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_ALM)
PACKET_CW_CLOSE

// ��÷ �ȳ� ���� �뺸
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_NTF)
    LottoNoticeRecord notice;
PACKET_CW_CLOSE

// ��÷ �ȳ� ���� ��� ��û
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN)
PACKET_CW_CLOSE

// ��÷ �ȳ� ���� ��� ����
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_LIST_ACK)
    BYTE bNumNoticeRecords;
    LottoNoticeRecord notices[LOTTO_MAX_NOTICE]; // ��÷ �ȳ� ���� ���

    int GetSize() const
    {
        return (sizeof(MSG_CW_LOTTO_NOTICE_LIST_ACK) \
            - (sizeof(LottoNoticeRecord) * (LOTTO_MAX_NOTICE - bNumNoticeRecords)));
    }
PACKET_CW_CLOSE

// ��÷ �ȳ� ���� ���� Ȯ��
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM)
    INT64 i64Index;
PACKET_CW_CLOSE

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
// �˸� �޼���
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_BRD)
    NotificationInfo info;
PACKET_CW_CLOSE

// ���� �˸� �޼��� ��û
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_PREV_SYN)
PACKET_CW_CLOSE

// ���� �˸� �޼��� ����
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_PREV_ACK)
    int              count;
    NotificationInfo infos[MAX_NOTIFICATION_COUNT];
PACKET_CW_CLOSE
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ ���� ���/���� ��û 
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_REGISTER_SYN)
    PartyFindingKeyType update_wanted_party_finding_key_; // ������ ���ϴ� ��Ƽ ���� Ű (��� �϶��� 0)
    TCHAR party_finding_title_[PartyFindingTitleConst::kBufferLength]; // ���� ����
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_people_; // ���� �ο�
    PartyFindingGoalConst::ValueType party_goal_; // ��Ƽ ����

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ��� ��û
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_GET_LIST_SYN)
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ��� ����
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_GET_LIST_ACK)
    RC::PartyFindingResultUtil::ValueType get_list_result_; // ��û�� ���� ó�� ���
    PartyFindingListConst::ValueType party_finding_list_size_; // ���� ��� ����    
    PartyFindingInfo party_finding_list_[PartyFindingListConst::kMaxLength]; // ���� ���
    
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(MSG_CW_PARTY_FINDING_GET_LIST_ACK) - 
                                (sizeof(PartyFindingInfo) * 
                                (PartyFindingListConst::kMaxLength - party_finding_list_size_)));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ���� ��û ��û
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_REQUEST_SYN)
    PartyFindingKeyType join_wanted_party_finding_key_; // ���� �Ϸ��� ��Ƽ ���� Ű

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ���� ��û�� ���� ���� ��û
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_RESPONSE_CMD)
    PartyFindingKeyType join_wanted_party_finding_key_; // ���� ��û�� ��Ƽ ���� Ű
    CHARGUID join_wanted_user_id_; // ���� ��û�� ����
    TCHAR join_wanted_user_name_[MAX_CHARNAME_LENGTH+1]; // ���� ��û�� ���� �̸�
    BYTE join_wanted_user_class_; // ���� ��û�� ���� Ŭ����
    LEVELTYPE join_wanted_user_level_; // ���� ��û�� ���� ����

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ���� ��û�� ���� ����
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK)
    RC::PartyFindingResultUtil::ValueType join_response_result_; // ���� ���
    PartyFindingKeyType join_wanted_party_finding_key_; // ���� ��û�� ��Ƽ ���� Ű
    CHARGUID join_wanted_user_id_; // ���� ��û�� ����

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ��� ��û
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_CANCEL_SYN)
    PartyFindingKeyType cancel_wanted_party_finding_key_; // ��� �Ϸ��� ��Ƽ ���� Ű

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// ��Ƽ ���� ���/����/����/��� ��û�� ���� ����
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_RESULT_ACK)
    PartyFindingResponseTypeConst::ValueType response_type_; // ���� ����
    RC::PartyFindingResultUtil::ValueType result_; // ��û�� ���� ó�� ���
    PartyFindingKeyType party_finding_key_; // ó���� ��û�� ��Ƽ ���� Ű

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)
