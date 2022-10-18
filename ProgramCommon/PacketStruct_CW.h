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
//		SYN - Server에 요청
//		ACK - 요청에 대한 허락
//		NAK - 요청에 대한 거부
//		CMD - Server의 명령
//		BRD - Server가 동기화 관련 내용을 알림
//
// * 프로토콜 Naming Rule
//		Prefix_카테고리명_프로토콜명_Postfix
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
// CW_CONNECTION 카테고리
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

// 접속하면 가장 먼저 주는 패킷
PACKET_CW_OPEN( CW_CONNECTION, CW_ENTER_READY_BRD )
PACKET_CW_CLOSE

//-------------------------------------------------------------------------------------------------
// CW_CHAT 카테고리
//-------------------------------------------------------------------------------------------------

// 귓속말 요청
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

// 귓속말 성공
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

// 귓속말 실패
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_NAK )
	enum
	{
		USER_NOT_FOUND,
		USER_WHISPER_OFF,
	};

	BYTE		byReason;
	
	int GetSize() { return sizeof(MSG_CW_WHISPER_NAK); }
PACKET_CW_CLOSE

// 공지 요청
PACKET_CW_OPEN( CW_CHAT, CW_NOTICE_SYN )
	BYTE		byLen;
	char		szMsg[MAX_NOTICE_LEN+1];

	int GetSize() { return ( sizeof(MSG_CW_NOTICE_SYN) - ( sizeof(char) * ( MAX_NOTICE_LEN - byLen ) ) ); }
PACKET_CW_CLOSE

// 공지 브로드캐스트
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

// 귓속말 차단 요청
struct MSG_CW_WHISPER_SET_SYN : public MSG_BASE
{
	MSG_CW_WHISPER_SET_SYN() { m_byCategory = CW_CHAT; m_byProtocol = CW_WHISPER_SET_SYN; }

	BOOL	bWhisper;		// TRUE면 켬, FALSE면 끔

	int GetSize() { return sizeof(MSG_CW_WHISPER_SET_SYN); }
};

// 귓속말 차단 체크 결과
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_SET_ACK )
	BOOL	bWhisper;		// 사용자가 보낸 값을 그대로 돌려준다

	int GetSize()		{ return sizeof(MSG_CW_WHISPER_SET_ACK); }
PACKET_CW_CLOSE

// 귓속말 실패
PACKET_CW_OPEN( CW_CHAT, CW_WHISPER_SET_NAK )
	int GetSize()		{ return sizeof(MSG_CW_WHISPER_SET_NAK); }
PACKET_CW_CLOSE

// 일반 채팅 요청
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

// 일반 채팅 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_NORMAL_NAK )
//	enum
//	{
//		_ERR_DEFAULT = 0,
//		_ERR_CHATBLOCKED,	// 채팅금지 상태
//		_ERR_NOTSYNC,		// 싱크가 안 맞은 상태(VIEWPORT_CHARSTATE 재전송 요청
//	};
	WORD						m_wErrorCode;	// RC::eCHAT_RESULT 참조
PACKET_CW_CLOSE

// 일반 채팅 브로드캐스트
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

//외치기 요청
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

// 외치기 브로드캐스트
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

// 외치기 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_SHOUT_NAK )
//	enum
//	{
//		_ERR_DEFAULT = 0,
//		_ERR_CHATBLOCKED,	// 채팅금지 상태
//		_ERR_ALREADYSHOUT,	// 외치기 시간 간격 이전에 다시 시도
//	};
	WORD						m_wErrorCode;		// RC::eCHAT_RESULT 참조
PACKET_CW_CLOSE

// 파티 채팅 요청
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

// 파티 채팅 브로드캐스트
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

// 음성 채팅 요청
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_SYN )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// 상대 캐릭명
	char						m_ptszAccountID[ID_MAX_LEN+1];				// 본인 계정 ID
PACKET_CW_CLOSE

// 음성 채팅 요청됐음
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// 상대 캐릭명
	char						m_ptszAccountID[ID_MAX_LEN+1];				// 상대 계정 ID
PACKET_CW_CLOSE

// 음성 채팅 요청 오류
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REQUEST_NAK )
	enum
	{
		_ERR_DEFAULT,		// 알 수 없는 이유
		_ERR_BLOCKVOICE,	// 상대가 음성채팅 금지 상태
		_ERR_NO_CHARNAME,	// 상대 캐릭터가 없거나 접속하지 않았음
	};

	WORD						wErrorCode;
PACKET_CW_CLOSE

// 음성 채팅 수락
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_SYN )
	char						m_ptszAccountID[ID_MAX_LEN+1];				// 본인 계정 ID
PACKET_CW_CLOSE

// 음성 채팅 수락됐음
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// 상대 캐릭명
	char						m_ptszAccountID[ID_MAX_LEN+1];				// 상대 계정 ID
PACKET_CW_CLOSE

// 음성 채팅 수락 오류
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_ACCEPT_NAK )
	enum
	{
		_ERR_DEFAULT,		// 알 수 없는 이유
		_ERR_NO_CHARNAME,	// 상대 캐릭터가 없거나 접속하지 않았음
	};

	WORD						wErrorCode;
PACKET_CW_CLOSE

// 음성 채팅 거절
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REJECT_SYN )
	enum
	{
		_REASON_DEFAULT,			// 거절
		_REASON_VOICE_CHAT,			// 1:1 음성채팅중..
		_REASON_PARTY_VOICE_CHAT,	// 파티 음성채팅중
		_REASON_NOT_USE,			// 음성채팅 사용 안함
		_REASON_NOT_ALLOW,			// 허용된 상대가 아니다
		_REASON_BUSY,				// 요청/수락 대기중이라 거절
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// 음성 채팅 거절됐음
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_REJECT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];

	enum
	{
		_REASON_DEFAULT,			// 거절
		_REASON_VOICE_CHAT,			// 1:1 음성채팅중..
		_REASON_PARTY_VOICE_CHAT,	// 파티 음성채팅중
		_REASON_NOT_USE,			// 음성채팅 사용 안함
		_REASON_NOT_ALLOW,			// 허용된 상대가 아니다
		_REASON_BUSY,				// 요청/수락 대기중이라 거절
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// 음성 채팅 종료
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_SYN )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 상대
	
	enum
	{
		_REASON_DEFAULT,			// 음성채팅 종료
		_REASON_PARTY_VOICE_CHAT,	// 파티 참가로 인한 종료
		_REASON_LOGOUT,				// 로그아웃
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// 음성 채팅 종료되었음
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 상대

	enum
	{
		_REASON_DEFAULT,			// 음성채팅 종료
		_REASON_PARTY_VOICE_CHAT,	// 파티 참가로 인한 종료
		_REASON_LOGOUT,				// 로그아웃
	};

	WORD						wReasonCode;
PACKET_CW_CLOSE

// 음성 채팅 종료 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_NAK )
	enum
	{
		_ERR_DEFAULT,		// 알 수 없는 이유
		_ERR_NOTCHAT,		// 음성 채팅 중 아니었음
		_ERR_NO_CHARNAME,	// 상대 캐릭터가 없거나 접속하지 않았음
	};
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 상대
	WORD						wErrorCode;
PACKET_CW_CLOSE


// 음성 채팅 마이크 ON/OFF  => 파티원에게 상태를 전달해야 합니다.
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_SYN )
	BOOL						bOn;	
PACKET_CW_CLOSE

// 음성 채팅 ON/OFF 브로드
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_BRD )
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 상대
	BOOL						bOn;	
PACKET_CW_CLOSE

// 음성 채팅 ON/OFF 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_NAK )
	enum
	{
		_ERR_DEFAULT,		// 알 수 없는 이유
	};
	char						m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 상대
	WORD						wErrorCode;
PACKET_CW_CLOSE

// 파티 가입했을 때 기존 파티원들 음성 채팅 정보 요청
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_REQ )
PACKET_CW_CLOSE

// 파티 가입했을 때 기존 파티원들 음성 채팅 정보 전송
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_ANS )
	enum { _MAX_PARTYMEMBER_NUM = 9, };	// 총 파티원 10명이므로 최대 기존 파티원은 9명

	BYTE					m_byMemberNum;
	STRUCT_VOICE_STATUS		m_sMemberStatus[_MAX_PARTYMEMBER_NUM];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PARTY_MIC_STATUS_ANS) - ( sizeof(STRUCT_VOICE_STATUS) * ( _MAX_PARTYMEMBER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// 파티 가입했을 때 기존 파티원들 음성 채팅 정보 전송 실패
// 이 경우 파티가 없거나 파티 정보를 아직 받지 못한 상태임
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_NAK )
PACKET_CW_CLOSE

// 다른 캐릭터 음성 채팅 on/off 상태 확인 요청(C->W)
// CW_CHAT_PARTY_MIC_STATUS_NAK 받았을 때 사용
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_REQ )
	enum { _MAX_PLAYER_NUM = 9, };

	BYTE				m_byMemberNum;
	char				m_ptszMemberStatus[_MAX_PLAYER_NUM][MAX_CHARNAME_LENGTH+1];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ) - ( (sizeof(char) * (MAX_CHARNAME_LENGTH+1)) * ( _MAX_PLAYER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// 다른 캐릭터 음성 채팅 on/off 상태 확인 성공(C->W)
// 이 때 실패한다면 그냥 MemberNum을 0으로 보낸다
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_ANS )
	enum { _MAX_PLAYER_NUM = 9, };

	BYTE					m_byMemberNum;
	STRUCT_VOICE_STATUS		m_sMemberStatus[_MAX_PLAYER_NUM];

	int	GetSize()
	{
		return ( sizeof(MSG_CW_CHAT_PLAYER_MIC_STATUS_ANS) - ( sizeof(STRUCT_VOICE_STATUS) * ( _MAX_PLAYER_NUM - m_byMemberNum ) ) );
	}
PACKET_CW_CLOSE

// 채팅 금지 통보
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_BLOCKED_CMD )
	DWORD					m_dwRemainTime;		// 채팅 금지 남은 시간(분 단위)
PACKET_CW_CLOSE

// 채널 채팅 관련
// 채널 채팅 참여
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN )
    BYTE m_byRoomNameLength; // 방제 길이, without null terminated character
    char m_ptszRoomName[MAX_ROOMTITLE_LENGTH + 1]; // 방제

    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_CW_CLOSE

// 채널 채팅 참여 성공
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_ACK )
	BOOL	m_bCreated;						// 생성된 것이면 TRUE, 참여한 것이면 FALSE
	BYTE	m_byUserNum;					// 현재 대화방에 참여중인 유저 숫자
PACKET_CW_CLOSE

// 채널 채팅 참여 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT 참조
PACKET_CW_CLOSE

// 채널 채팅 참여 알림
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_JOIN_BRD )
	BYTE	m_byChannelID;								// 들어온 사용자의 채널 인덱스
	BYTE	m_byUserNum;					// 현재 대화방에 참여중인 유저 숫자
	char	m_ptszJoinMember[MAX_CHARNAME_LENGTH+1];	// 들어온 사용자
PACKET_CW_CLOSE

// 채널 채팅 내용 보냄
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN)
    BYTE m_byChatLength; // 채팅 길이, _tcslen(m_ptszChat)
    char m_ptszChat[MAX_CHAT_LEN + 1]; // 채팅 내용

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChat) + sizeof(m_ptszChat[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChat[0]) * m_byChatLength);
    };
PACKET_CW_CLOSE;

// 채널 채팅 내용 브로드
PACKET_CW_OPEN(CW_CHAT, CW_CHAT_CHANNEL_SEND_BRD)
    //enum { _MAX_CHATMSG_SIZE = 255 };
    BYTE m_byChannelID;                          // 보낸 놈의 채널 인덱스
    char m_ptszCharName[MAX_CHARNAME_LENGTH + 1];  // 보낸 놈
    BYTE m_byChatLength;                         // 채팅 길이, _tcslen(m_ptszChat)
    char m_ptszChat[MAX_CHAT_LEN + 1];             // 채팅 내용

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_ptszChat) + sizeof(m_ptszChat[0]);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ptszChat[0]) * m_byChatLength);
    };
PACKET_CW_CLOSE;

// 채널 채팅 내용 보내기 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_SEND_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT 참조
PACKET_CW_CLOSE

// 채널 채팅 초대
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN )
	char	m_ptszInviteName[MAX_CHARNAME_LENGTH+1];	// 초대할 사람
PACKET_CW_CLOSE

// 초대 성공
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_ACK )
PACKET_CW_CLOSE

// 초대 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK )
	BYTE		m_byErrorCode;
    // added by _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL = {
    // 클라->서버 : 초대 신청자 이름
    // 서버->클라 : 초대 대상자 이름
    TCHAR invite_target_name_[MAX_CHARNAME_LENGTH+1];
    // }
PACKET_CW_CLOSE

// 초대 받았음
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INVITE_CMD )
	char	m_ptszInviteName[MAX_CHARNAME_LENGTH+1];	// 나를 초대한 사람
	BYTE	m_byRoomNameLength;							// 방제 길이
	char	m_ptszRoomName[MAX_ROOMTITLE_LENGTH+1];
    uint16_t GetSize() const
    {
        return sizeof(*this);
    };
PACKET_CW_CLOSE

// 채널 채팅방 퇴장 요청
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN )
PACKET_CW_CLOSE

// 퇴장 성공
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_ACK )
PACKET_CW_CLOSE

// 퇴장 실패
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_NAK )
	BYTE	m_byErrorCode;					// RC::eCHAT_RESULT 참조
PACKET_CW_CLOSE

// 퇴장사실 알림
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_OUT_BRD )
	BYTE	m_byUserNum;							// 현재 대화방에 참여중인 유저 숫자
	char	m_ptszOutName[MAX_CHARNAME_LENGTH+1];	// 나간 사람 이름
PACKET_CW_CLOSE

// 채널채팅방 정보 요청(인원, 이름)
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ )
PACKET_CW_CLOSE

// 채널채팅방 정보
PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_ANS )
	BYTE	m_byUserNum;	// 현재 참여중인 유저 숫자
	char	m_ptszCharName[MAX_CHATZONE_USER][MAX_CHARNAME_LENGTH+1];	// 현재 참여중인 인원 정보

	WORD GetSize() { return ( sizeof(MSG_CW_CHAT_CHANNEL_INFO_ANS) - ( MAX_CHATZONE_USER - m_byUserNum ) ); }
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_CHAT, CW_CHAT_CHANNEL_INFO_NAK )
	BYTE	m_byErrorCode;
PACKET_CW_CLOSE

////////////////////////////////////
// GM 명령어

// GM 공지 명령
PACKET_CW_OPEN(CW_GM, CW_GM_NOTICE_SYN)
    enum { _MAX_CHATMSG_SIZE = MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE };
    enum
    {
        NOTICE_WORLD = 0,   // 자신이 속한 월드 전체 공지
        NOTICE_CHANNEL, // 자신이 속한 채널 전체 공지
        NOTICE_ZONE,    // 자기가 들어와 있는 지역에만 공지
    };

	BYTE		byNoticeType;					// 공지 종류
	DWORD		dwNoticeLength;					//  공지 문자열의 길이
	char		szNotice[_MAX_CHATMSG_SIZE+1];	//  공지 문자열

	int		GetSize() { return sizeof(MSG_CW_GM_NOTICE_SYN); }	// 가변 패킷 아님
	//WORD		GetSize() { return sizeof(MSG_CW_GM_NOTICE_SYN) - ( MAX_NOTICE_LEN - byNoticeLen ); }
};

// GM 공지 실패
PACKET_CW_OPEN( CW_GM, CW_GM_NOTICE_NAK )
	enum
	{
		ERR_NOTICE_DEFAULT,
	};

	DWORD		dwResultCode;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_STRING_CMD_SYN )
	enum { MAX_STRING_CMD_LENGTH = 100 };
	char				m_szStringCmd[MAX_STRING_CMD_LENGTH];	// 스트링 명령어
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_STRING_CMD_NAK )
	BYTE				m_byErrorCode;								// eGM_RESULT
PACKET_CW_CLOSE

// GM 귓말 켬/끔 결과
PACKET_CW_OPEN( CW_GM, CW_GM_WHISPER_SET_ACK )
	BOOL				m_bOn;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_WHISPER_SET_NAK )
PACKET_CW_CLOSE

// 유저 채팅금지/허용 결과
PACKET_CW_OPEN( CW_GM, CW_GM_USERCHAT_BLOCK_ACK )
	CHARGUID			m_CharGuid;
	BOOL				m_bBlock;
	DWORD				m_dwBlockTime;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_GM, CW_GM_USERCHAT_BLOCK_NAK )
	enum
	{
		_ERR_DEFAULT = 0,
		_ERR_NOTCONNUSER,	// 접속중이 아닌 사용자 - WOPS를 통해 처리해야 한다
		_ERR_BLOCKED,		// 이미 채팅금지된 사용자
		_ERR_NOTBLOCK,		// 이미 채팅허용된 사용자
	};
	WORD				m_wErrorCode;
PACKET_CW_CLOSE

//////////////////////////////////
// 뷰포트 정보 패킷

struct MSG_CW_VIEWPORT_CHARSTATE : public MSG_BASE_FORWARD
{
	DWORD			dwSectorIndex;		// 현재 위치한 섹터 인덱스
	DWORD			dwFieldCode;		// 현재 위치한 맵의 필드 코드

	int GetSize()		{ return sizeof(MSG_CW_VIEWPORT_CHARSTATE); }
};

//////////////////////////////////
// Armorkit 관련 패킷

struct MSG_CW_ARMORKIT_FINDHACK_SYN : public MSG_BASE
{
	MSG_CW_ARMORKIT_FINDHACK_SYN()	{ m_byCategory = CW_ARMORKIT; m_byProtocol = CW_ARMORKIT_FINDHACK; }

	int		HackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
	int		Size;			// Buffer 의 길이
	char	Buf[1024];		// 데이터 버퍼-1024는 보안파트에서 전달받은 암묵적 최대값(성현창)

	int GetSize()		{ return sizeof(MSG_CW_ARMORKIT_FINDHACK_SYN); }
};

//////////////////////////////////
//////////////////////////////////
// 친구 관련 패킷

struct STRUCT_FRIEND_STAT	// 친구 상태 구조체
{
	char			ptszFriendName[MAX_CHARNAME_LENGTH+1];		// 친구 이름
	BOOL			bOnline;									// 온라인이면 TRUE, 오프라인이면 FALSE
};

// 처음 접속할 때 친구 목록 요청
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_NEWLIST_SYN )
	int GetSize()		{ return sizeof(MSG_CW_FRIEND_NEWLIST_SYN); }
PACKET_CW_CLOSE

// 처음 접속할 때 친구 목록 수신(가변)
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_FRIENDLIST_ACK)
	BYTE				byFriendStatNum;						// 친구 숫자
	STRUCT_FRIEND_INFO	sFriendStat[MAX_FRIENDBLOCK_STAT_NUM];	// 친구들의 상태(차단도 포함)

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sFriendStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sFriendStat[0]) * byFriendStatNum);
    };
PACKET_CW_CLOSE;

// 처음 접속할 때 차단 목록 수신(가변)
PACKET_CW_OPEN(CW_FRIEND, CW_FRIEND_BLOCKLIST_ACK)
	BYTE				byBlockStatNum;						// 차단된 캐릭터 숫자
	STRUCT_BLOCK_INFO	sBlockStat[MAX_BLOCK_STAT_NUM];		// 캐릭터들의 정보

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sBlockStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sBlockStat[0]) * byBlockStatNum);
    };
PACKET_CW_CLOSE;

// 처음 접속할 때 친구 목록 요청 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_NEWLIST_NAK )
	enum
	{
		ERR_DEFAULT,
		NO_FRIEND_DATA,		// 데이터 없다
	};

	DWORD				dwResult;
PACKET_CW_CLOSE

// 접속 중에 친구 로그온
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
// 친구 추가
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_SYN )
	char				ptszCharNameTo[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_ACK )
	STRUCT_FRIEND_INFO	sFriendStat;	// 해당 친구의 정보
PACKET_CW_CLOSE

// 친구 요청 불가 상태
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_ADD_NAK )

	enum
	{
		ERR_DEFAULT,	// 기타 등등
		NO_USER,		// 해당하는 유저가 없음
		ALREADY_FRIEND,	// 이미 친구 상태
		FRIEND_IS_ME,	// 나를 친구 요청했음
#ifdef _FRIEND_OVERFLOW
		FRIEND_OVERFLOW,	//친구 숫자 제한 초과(90명)
#endif
	};

	BYTE				m_byResult;
PACKET_CW_CLOSE

// 친구 삭제 요청
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_SYN )
	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 친구 삭제 성공
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_ACK )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID deleteBuddyIndex;
	//#endif
	char				ptszDeletedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 친구 삭제 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_DEL_NAK )
	enum
	{
		ERR_DEFAULT,			// 알 수 없는 이유
		NOT_FRIEND_USER,		// 친구 아님
	};

	BYTE				m_byResult;
PACKET_CW_CLOSE

// 차단 요청
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_SYN )
	char				ptszBlockName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 차단 성공
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_ACK )
	char				ptszBlockedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 차단 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_NAK )
	enum
	{
		ERR_DEFAULT,		// 알 수 없는 이유
		NO_USER,			// 존재하지 않는 캐릭터 이름
		BLOCKED_ME,			// 자신을 차단하려고 시도
		ALREADY_BLOCKED,	// 이미 블록된 상태
#ifdef _FRIEND_OVERFLOW
		BLOCK_OVERFLOW,		// 차단 최대 개수 초과
#endif
	};

	DWORD				m_byResult;
PACKET_CW_CLOSE

// 차단 삭제 요청
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN )
	char				ptszBlockName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 차단 삭제 성공
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_ACK )
	//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	//	CHARGUID blockFreeCharGuid;
	//	CHARGUID FreeCharOnlineState;
	//#endif
	char				ptszBlockedName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 차단 삭제 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_BLOCK_FREE_NAK )
	enum
	{
		ERR_DEFAULT,		// 알 수 없는 이유
		NO_USER,			// 존재하지 않는 캐릭터 이름
		NOT_BLOCKED,		// 차단된 캐릭터가 아님
	};

	DWORD				m_byResult;
PACKET_CW_CLOSE

// 친구 채팅
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

// 친구 채팅 수신
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

// 친구 채팅 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHAT_NAK )
	enum
	{
		ERR_DEFAULT,	// 알 수 없는 이유
		NOT_FRIEND,		// 친구 아님
		OFFLINE_FRIEND,	// 친구가 오프라인 상태
		BLOCKED_FRIEND,	// 당신은 차단된 친구입니다.-0-/
		BLOCK_CHAT,		// 친구가 채팅 거부 상태
	};

	char				ptszFriendNameTo[MAX_CHARNAME_LENGTH+1];	// 누구한테 보내는 채팅이었냐?!
	DWORD				m_byResult;
PACKET_CW_CLOSE

// 친구 채팅 거부 체크(토글 방식)
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN )
	BOOL				m_bBlock;	// 0: 허용 1: 거부
PACKET_CW_CLOSE

// 친구 채팅 거부 토글 성공
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_ACK )
	BOOL		m_bNowBlock;		// 결과적으로 거부 됐으면 TRUE, 허용 됐으면 FALSE
PACKET_CW_CLOSE

// 친구 채팅 거부 토글 실패
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_CHATBLOCK_NAK )
	enum { ERR_DEFAULT = 0, };

	DWORD		dwResult;		// 원인
PACKET_CW_CLOSE

//친구 추천 목록중 일반친구로 상태가 변경된 리스트, _KR_0_20091021_FIRENDINVITATION_EXTEND
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
// 친구 채팅 초대(3인 이상의 경우)
struct MSG_CW_FRIEND_CHAT_INVITE_SYN : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_SYN() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_SYN; }

	char				ptszFriendNameTo[MAX_CHARNAME_LENGTH+1];
};

// 친구 초대 통보
struct MSG_CW_FRIEND_CHAT_INVITE_BRD : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_BRD() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_BRD; }

	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
};

// 친구 초대 실패
struct MSG_CW_FRIEND_CHAT_INVITE_NAK : public MSG_BASE
{
	MSG_CW_FRIEND_CHAT_INVITE_NAK() { m_byCategory = CW_FRIEND; m_byProtocol = CW_FRIEND_CHAT_INVITE_NAK; }
	enum
	{
		ERR_DEFAULT,	// 알 수 없는 이유
		NOT_FRIEND,		// 친구 아님
		OFFLINE_FRIEND,	// 친구가 오프라인 상태
		BLOCKED_FRIEND,	// 넌 차단당했다
	};

	char				ptszFriendName[MAX_CHARNAME_LENGTH+1];
};
*/

// 친구 레벨업 알림
PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_LEVELUP_BRD )
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	LEVELTYPE	m_Level;
    void Clear() {
        const size_t kClearSize = static_cast<size_t>((char*)(this + 1) - m_ptszCharName);
        ZeroMemory(m_ptszCharName, kClearSize);
    };
PACKET_CW_CLOSE

// 친구 상세정보 관련
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
	BYTE		m_byZoneType;							// 현재 위치(마을/필드/로비...), eZONETYPE 참고
	MAPCODE		m_MapCode;								// 현재 맵 코드
	DWORD		m_dwZoneKey;							// 현재 지역 인덱스(배틀존 번호로 사용 가능)
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	// 소속된 길드명(없으면 0)
	BYTE		m_byDuty;								// 길드가 있을 경우 길드에서의 지위. 
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    TCHAR		m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
#endif
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_FRIEND, CW_FRIEND_STATUS_NAK )
	BYTE		m_byErrorCode;							// 에러코드(ResultCode 참고)
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
	BYTE		m_byErrorCode;							// 에러코드(ResultCode 참고)
PACKET_CW_CLOSE



// 친구와 게임 같이하기 - '같이하기' 버튼 눌렀을 때 최신 정보를 얻어오기 위해 사용한다
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
// 길드 관련 프로토콜

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
    char ptszCharName[MAX_CHARNAME_LENGTH + 1]; // 보낸 사람
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
		ERR_NOT_GUILD,	// 그런 길드 없다(서버와 클라의 GuildGuid 다름)
		ERR_NOT_MEMBER,	// 길드 멤버 아니다
		ERR_WAIT_INFO,	// 정보가 없으므로 길드서버에 요청 들어갔음(잠시 후 다시 시도하십시오)
	};

	BYTE		byResult;
PACKET_CW_CLOSE

// 길드 공지
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
// 패킷 암호화 키 교환 및 주기적 알고리즘 변환
PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_KEY )
	BYTE						m_bySN;			// Sequence Number
	BYTE						m_byCryptInfo;	// 인코딩된 패킷 암호화키 블록 풀기 위한 키
	union {
		DWORD					m_dwUserKey;
		BYTE					m_byCryptData[200];	// 알고리즘 정보 (키, 등등...)
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

// 암호화된 패킷 풀기위한...
struct MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED {
	BYTE						m_bySN;			// Sequence Number : received
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// 패킷마다 다른 크기를 가질 것이다.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// 패킷 암호화 하기 위한...
struct MSG_CW_CRYPTOGRAPHY_PACKET_ENCODING {
	BYTE						m_bySN;			// Sequence Number : sending
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// 패킷마다 다른 크기를 가질 것이다.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

// 암호화 내용 사용해라!
PACKET_CW_OPEN( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_CMD )
PACKET_CW_CLOSE

// 다른 유저의 UserID 요청(웹 접근 때문)
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
		_ERR_NOTCONNUSER,			// 접속 중이 아닌 사용자 정보 요청
	};

	WORD						m_wErrorCode;
PACKET_CW_CLOSE

//////////////////////////////////
//////////////////////////////////
// 리벤지(나쁜친구) 관련 패킷

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_ADD_CMD )
	BYTE						m_byRevengePoint;	// Remain Revenge Point
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    STRUCT_REVENGE_INFO         m_sRevengeStat;
#else
    STRUCT_REVENGE_PACKET_INFO	m_sRevengeStat;
#endif
PACKET_CW_CLOSE

// 리벤지 리스트에서 제거
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
	char			m_ptszCharName[MAX_CHARNAME_LENGTH+1];	// 사용하지 않음 - 삭제 예정
	BYTE			m_byErrorCode;	// RC::eREVENGE_RESULT 참고
PACKET_CW_CLOSE

// 처음 접속할 때 리벤지 리스트 수신(가변) - 친구 리스트 요청했을 때 함께 보냄
PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LIST_CMD )
	BYTE						byRevengePoint;						// 리벤지 포인트
    // CHANGES: f111004.2L, changes default type, this is changed by _NA002676_WORLD_SERVER_RENEWAL
    uint8_t byRevengeStatNum; // 복수 대상 캐릭터 숫자, 
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO 
    STRUCT_REVENGE_INFO         sRevengeStat[MAX_REVENGE_STAT_NUM];
#else
	STRUCT_REVENGE_PACKET_INFO	sRevengeStat[MAX_REVENGE_STAT_NUM];	// 캐릭터들의 정보(임시로 친구 개수와 동일하게)
#endif
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sRevengeStat);
        return static_cast<uint16_t>(kMinSize + sizeof(sRevengeStat[0]) * byRevengeStatNum);
    };
PACKET_CW_CLOSE

// 리벤지 대상 로그인
PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LOGIN_BRD )
	char				m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		byChannelID;							// 접속중인 채널
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_REVENGE, CW_REVENGE_LOGOUT_BRD )
	char				ptszCharName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

// 리벤지 대상  레벨업 알림
PACKET_CW_INHERIT_OPEN(CW_REVENGE, CW_REVENGE_LEVELUP_BRD, MSG_CW_FRIEND_LEVELUP_BRD)
    void Clear() {
        // NOTE: first configurable data field = 'MSG_CW_FRIEND_LEVELUP_BRD::m_ptszCharName'
        const size_t kClearSize = static_cast<size_t>((char*)(this + 1) - m_ptszCharName);
        ZeroMemory(m_ptszCharName, kClearSize);
    };
PACKET_CW_INHERIT_CLOSE;

//////////////////////////////////
//////////////////////////////////
// 쪽지 관련 패킷

// 쪽지 전송
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_SYN )
	char				m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
	char				m_ptszMemo[MAX_MEMO_LENGTH + 1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_ACK )
	char				m_ptszRecvName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_SEND_NAK )
	BYTE				m_byErrorCode;							// RC::eMEMO_RESULT 참조
PACKET_CW_CLOSE

// 쪽지 목록
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_REQ )
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_ANS )
	BYTE			m_byMemoCnt;								// 쪽지 개수(현재 페이지에서)
	BYTE			m_byMemoPage;								// 쪽지 목록의 현재 페이지(20개 단위로 전송)
	BYTE			m_byMemoPageCnt;							// 쪽지 목록의 총 페이지(현재 쪽지 123개면 7페이지가 됨. m_byMemoPage=m_byMemoPageCnt면 송신 완료)
	STRUCT_SUBJECT_PACKETINFO	m_Subject[MAX_MEMO_PAGENUM];	// 쪽지 요약과 읽었음 표시만 전송
	WORD	GetSize()
	{
		return ( sizeof(MSG_CW_MEMO_LIST_ANS) - ( sizeof(STRUCT_SUBJECT_PACKETINFO) * (MAX_MEMO_PAGENUM - m_byMemoCnt) ) );
	}
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_LIST_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE

// 쪽지 내용 확인
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_REQ )
	MEMOGUID		m_MemoGuid;
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_ANS )
	MEMOGUID		    m_MemoGuid;
	char			    m_ptszMemo[MAX_MEMO_LENGTH + 1];
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    SLOTCODE            m_recvItemCode;//환급 받을 아이템 코드
    ULONGLONG           m_amountCost;//환급 받을 금액
    eSYSTEM_MEMO_TYPE   m_typeSystemMemo;//시스템 메모 구분자
    // }
PACKET_CW_CLOSE

PACKET_CW_OPEN( CW_MEMO, CW_MEMO_VIEW_NAK )
	BYTE			m_byErrorCode;
PACKET_CW_CLOSE

// 쪽지 삭제
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

// 새 쪽지 알림
PACKET_CW_OPEN( CW_MEMO, CW_MEMO_NEWMEMO_BRD )
	STRUCT_SUBJECT_PACKETINFO	m_Subject;	// 보낸 사람 - 클릭하여 확인하면 바로 CW_MEMO_VIEW_REQ 전송하면 됨
PACKET_CW_CLOSE

//길드 관계
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

// 길드 창고 로그
PACKET_CW_OPEN(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ )
PACKET_CW_CLOSE

PACKET_CW_OPEN(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_ANS )
	BYTE	m_byLogCount;	 // 로그 개수(현재 페이지에서)
	BYTE	m_byCurrentPage; // 전송 되는 현재 페이지
	BYTE	m_byLogPageCount;	// 로그 목록의 총 페이지(현재 로그 123개면 7페이지가 됨. m_byCurrentPage=m_byLogPageCount면 송신 완료)
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
//그룹 시스템 통보 메세지
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LOGIN_BRD)                        //접속 중에 그룹맴버 로그온 통보
    BYTE byChannel;
	char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LOGOUT_BRD)                       //접속 중에 그룹맴버 로그아웃 통보
	char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ACCUMEXP_BRD)                     //로그인시 하위 멤버가 쌓아 놓은 경험치를 받았음을 통보
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD accumEXP;
    eGROUP_STAT checkDATE;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_BONUSMONEY_BRD)                   //그룹 보너스 하임 통보
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusMoney;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_BONUSEXP_BRD)                     //그룹 보너스 경험치 통보
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    DWORD bonusEXP;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_LEVELUP_BRD)                      //하위 멤버 레벨업
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
    BYTE charLevel;
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_BRD)                          //멤버 삭제 통보
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_RESTOR_CMD)                       //멤버 정보 재요청
char ptszMemeberName[MAX_CHARNAME_LENGTH+1];
PACKET_CW_CLOSE



//////////////////////////////////////////////////////////////////////////
//그룹 멤버 추가
//요청
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //요청받는 케릭터 이름
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_ACK)
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    DWORD charGUID;		                                            //맴버들의 GUID
#endif
    STRUCT_GROUP_MEMBER_PACKET_INFO m_MemberInfo;                   //요청 받은 맴버 정보 구조체
    eGROUP_STAT m_GroupInfo;
PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_NAK)
    enum
    {
        NONE = 0, // none or not set
        //
        ERR_DEFAULT,                                                // 기타 등등
        NO_USER,                                                    // 해당하는 유저가 없음
        ALREADY_GROUP,                                              // 이미 그룹 상태
        NOT_ADD_ME,                                                 // 스스로를 친구 요청했음
        NOT_MATCHING_LEVEL,                                         // 레벨이 적당하지 않다.
        GROUP_OVERFLOW,                                             // 그룹 제한 초과(6명)
        IGNOR_ADD_MEMBER,                                           // 상대방이 맴버 수락 요청 거부
        RES_TIME_OUT,                                               // 요청 시간 초과
        ON_TOGGLE_ADDBLOCK,                                         // 맴버 추가 거부 토글 켜있음
        ALREADY_GROUP_REQ,                                          // 다른 맴버로부터 그룹 요청중이다.
        BEHAVE_VENDOR_ESTABLISHER,                                  // 상점 개설중
        //
        MAX_COUNTS // upperbound
    };
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //삭제 요청하는 케릭터 이름, f110811.5L, unused?
    BYTE m_byResult;                                                //결과 값  
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹 멤버 추가 응답
//요청
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //자신을 초대한 케릭터 이름
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //자신을 초대한 케릭터 이름
PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK)
    enum
    {
        ERR_DEFAULT,                                                //알수 없는 오류
        RES_TIME_OUT,                                               //수락 타이밍이 지났다.
        IGNOR_ADD_MEMBER,                                           //거절을 했다.
    };
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //자신을 초대한 케릭터 이름
    BYTE m_byResult;                                                //실패 결과 값
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹 멤버 삭제 
//요청
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_SYN)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //삭제할 케릭터 이름
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_ACK)
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //삭제된 케릭터 이름
PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_DEL_NAK)
	enum
	{
		ERR_DEFAULT,    //알 수 없는 이유
        DB_ERROR,       //DB에서 데이터 처리 에러
		NOT_MEMBER_USER,                                            //그룹 맴버가 아님
	};                                              
    char ptszMemeberName[MAX_CHARNAME_LENGTH+1];                    //삭제된 케릭터 이름
   	BYTE m_byResult;                                                //결과값           
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹 멤버 리스트 요청
//요청
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_REQ)
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_ANS)
    BYTE byGroupMemberNum;                                          //맴버 수
    static const size_t MAX_RECORDS = MAX_GROUP_MEMBER;
    STRUCT_GROUP_MEMBER_INFO sGroupMemberInfo[MAX_RECORDS];    //맴버 정보

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(sGroupMemberInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(sGroupMemberInfo[0]) * byGroupMemberNum);
    }

PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_GROUPLIST_NAK)
    enum
    {
      ERR_DEFAULT,                                                  //알수없는 에러
      NO_MEMBER_DATA,                                               //데이터 없다
      GROUP_OVERFLOW,                                               //그룹원이 더 많다
    };
    DWORD dwResult;                                                 //결과 값           
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹 멤버 상세정보 요청
//요청
PACKET_CW_OPEN(CW_GROUP,CW_GROUP_STATUS_REQ)
    char m_ptszCharName[MAX_CHARNAME_LENGTH+1];                     //상세정보의 케릭터 이름
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_STATUS_ANS)
    char m_ptszCharName[MAX_CHARNAME_LENGTH+1];                     //케릭터 이름
    BYTE m_byChannel;                                               //케릭터 위치 체널
    BYTE m_byZoneType;							                    //현재 위치(마을/필드/로비...), eZONETYPE 참고
    MAPCODE m_MapCode;								                //현재 맵 코드
    DWORD m_dwZoneKey;							                    //현재 지역 인덱스(배틀존 번호로 사용 가능)
    char m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	                //소속된 길드명(없으면 0)
    BYTE m_byDuty;								                    //길드가 있을 경우 길드에서의 지위. 
PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN(CW_GROUP, CW_GROUP_STATUS_NAK)
    enum
    {
        ERR_DEFAULT,                                                //알수없는 에러
        NO_MEMBER_DATA,//데이터 없다
        NOT_MEMBER,
    };
    DWORD dwResult;                                                 //결과 값       
PACKET_CW_CLOSE
//////////////////////////////////////////////////////////////////////////
//그룹 초대 거부 토글
//요청
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_SYN )
BOOL m_bBlock;	                                                    // 0: 허용 1: 거부
PACKET_CW_CLOSE
//성공
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_ACK )
BOOL m_bNowBlock;		                                            //거부 됐으면 TRUE, 허용 됐으면 FALSE
PACKET_CW_CLOSE
//실패
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_ADDBLOCK_NAK )
PACKET_CW_CLOSE

//////////////////////////////////////////////////////////////////////////
//그룹 맴버 수가 현재 정상이 아닐 경우 해당 유저에게 알려준다.
//요청
PACKET_CW_OPEN( CW_GROUP, CW_GROUP_REWARD_FAIL_CMD )
PACKET_CW_CLOSE

//------------------------------------------------------------------------------
// CW_LOTTO : 하임 로또 카테고리
//------------------------------------------------------------------------------

// 하임 로또 응모 시작 공지
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_START_BRD)
    WORD wDrawNum;                      // 회차
PACKET_CW_CLOSE

// (내) 하임 로또 정보 요청
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_SYN)
    BYTE bFlag;                         // 클라이언트를 위한 구분 플래그: 그대로 응답으로 전달
PACKET_CW_CLOSE

// (내) 하임 로또 정보 응답
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_ACK)
    BYTE bFlag;                         // 요청으로 전달된 클라이언트용 플래그
    BYTE bStatus;                       // 현재 로또 활성화 상태 - 0:진행중이 아님(추첨 또는 보상 지급중), 1: 진행중
    CHARGUID iCharGuid;                 // 캐릭터 ID
    WORD wCurrentDrawNum;               // 현재 회차
    INT64 i64TotalAccum;                // 현재 총 적립액
    BYTE bNumTickets;                   // 현재 소지하고 있는 복권 용지 수
    BYTE bNumResultRecords;             // 과거 몇 회의 로또 결과 데이터가 포함되어 있는지
    LottoResultRecord results[LOTTO_NUM_RECENT_DRAWS - 1]; // 현재 회차 제외한 과거 추첨 결과
    BYTE bNumEntryRecords;              // 몇 개의 응모 데이터가 포함되어 있는지 (현재/과거 모두 포함)
    LottoEntryRecord entries[LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS]; // 응모 내역

    int GetSize() const
    {
        return (sizeof(MSG_CW_LOTTO_INFO_ACK) 
            - (sizeof(LottoEntryRecord) 
            * ((LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS) - bNumEntryRecords)));
    }
PACKET_CW_CLOSE

// (내) 하임 로또 정보 오류 응답
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_INFO_NAK)
    enum Reason {
        NOT_READY       // 하임 로또가 아직 초기화되지 않아 사용할 수 없음
      , UNKNOWN
    };
    CHARGUID iCharGuid;                 // 캐릭터 ID
    BYTE bReason;                       // 실패 사유
PACKET_CW_CLOSE

// 하임 로또 용지 획득 알림
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_TICKET_NTF)
    CHARGUID iCharGuid;                 // 캐릭터 ID
    BYTE bNumTickets;                   // 결과적으로 소지하고 있는 복권 용지 수
PACKET_CW_CLOSE

// 하임 로또 응모 요청
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_SYN)
    WORD wDrawNum;                      // 회차
    BYTE bAuto;                         // 자동 여부
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
PACKET_CW_CLOSE

// 하임 로또 응모 정상 응답
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_ACK)
    CHARGUID iCharGuid;                 // 캐릭터 ID
    WORD wDrawNum;                      // 회차
    BYTE bNumTickets;                   // 소모 후 결과적으로 소지하고 있는 복권 용지 수
    BYTE bAuto;                         // 자동 여부
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    INT64 i64TicketPrice;               // 복권용지 직접 구입시 차감된 가격
#endif
PACKET_CW_CLOSE

// 하임 로또 응모 오류 응답
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ENTRY_NAK)
    enum Reason {
        NOT_READY           // 하임 로또가 진행 중이 아님
      , OUT_OF_TICKET       // 하임 로또 용지 없음 (_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE 사용시에는 구매 제한 초과)
      , NOT_ENOUGH_MONEY    // 복권용지 구입을 위한 잔액 부족 (_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE)
      , UNKNOWN
    };
    CHARGUID iCharGuid;                 // 캐릭터 ID
    BYTE bReason;                       // 실패 사유
PACKET_CW_CLOSE

// 하임 로또 적립액 갱신 공지
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_ACCUM_BRD)
    WORD wDrawNum;                      // 회차
    INT64 i64TotalAccum;                // 현재 총 적립액
PACKET_CW_CLOSE

// 하임 로또 추첨 카운트다운 공지
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_COUNTDOWN_BRD)
    WORD wDrawNum;                      // 회차
    WORD wSeconds;                      // 추첨까지 남은 초 수
PACKET_CW_CLOSE

// 하임 로또 추첨/종료 공지
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_FINISH_BRD)
    WORD wDrawNum;                      // 회차
    BYTE bWinningNum[LOTTO_NUM_PICKS];  // 당첨 번호
PACKET_CW_CLOSE

// 당첨 안내 쪽지 도착 알림
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_ALM)
PACKET_CW_CLOSE

// 당첨 안내 쪽지 통보
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_NTF)
    LottoNoticeRecord notice;
PACKET_CW_CLOSE

// 당첨 안내 쪽지 목록 요청
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN)
PACKET_CW_CLOSE

// 당첨 안내 쪽지 목록 응답
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_LIST_ACK)
    BYTE bNumNoticeRecords;
    LottoNoticeRecord notices[LOTTO_MAX_NOTICE]; // 당첨 안내 쪽지 목록

    int GetSize() const
    {
        return (sizeof(MSG_CW_LOTTO_NOTICE_LIST_ACK) \
            - (sizeof(LottoNoticeRecord) * (LOTTO_MAX_NOTICE - bNumNoticeRecords)));
    }
PACKET_CW_CLOSE

// 당첨 안내 쪽지 읽음 확인
PACKET_CW_OPEN(CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM)
    INT64 i64Index;
PACKET_CW_CLOSE

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
// 알림 메세지
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_BRD)
    NotificationInfo info;
PACKET_CW_CLOSE

// 지난 알림 메세지 요청
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_PREV_SYN)
PACKET_CW_CLOSE

// 지난 알림 메세지 응답
PACKET_CW_OPEN(CW_NOTIFICATION, CW_NOTIFICATION_PREV_ACK)
    int              count;
    NotificationInfo infos[MAX_NOTIFICATION_COUNT];
PACKET_CW_CLOSE
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// 파티 모집 등록/갱신 요청 
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_REGISTER_SYN)
    PartyFindingKeyType update_wanted_party_finding_key_; // 갱신을 원하는 파티 모집 키 (등록 일때는 0)
    TCHAR party_finding_title_[PartyFindingTitleConst::kBufferLength]; // 모집 내용
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_people_; // 모집 인원
    PartyFindingGoalConst::ValueType party_goal_; // 파티 목적

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 목록 요청
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_GET_LIST_SYN)
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 목록 응답
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_GET_LIST_ACK)
    RC::PartyFindingResultUtil::ValueType get_list_result_; // 요청에 대한 처리 결과
    PartyFindingListConst::ValueType party_finding_list_size_; // 모집 목록 개수    
    PartyFindingInfo party_finding_list_[PartyFindingListConst::kMaxLength]; // 모집 목록
    
    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(MSG_CW_PARTY_FINDING_GET_LIST_ACK) - 
                                (sizeof(PartyFindingInfo) * 
                                (PartyFindingListConst::kMaxLength - party_finding_list_size_)));
    }
PACKET_CW_CLOSE

// 파티 모집 참가 신청 요청
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_REQUEST_SYN)
    PartyFindingKeyType join_wanted_party_finding_key_; // 참가 하려는 파티 모집 키

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 참가 신청에 대한 응답 요청
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_RESPONSE_CMD)
    PartyFindingKeyType join_wanted_party_finding_key_; // 참가 신청한 파티 모집 키
    CHARGUID join_wanted_user_id_; // 참가 신청한 유저
    TCHAR join_wanted_user_name_[MAX_CHARNAME_LENGTH+1]; // 참가 신청한 유저 이름
    BYTE join_wanted_user_class_; // 참가 신청한 유저 클래스
    LEVELTYPE join_wanted_user_level_; // 참가 신청한 유저 레벨

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 참가 신청에 대한 응답
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK)
    RC::PartyFindingResultUtil::ValueType join_response_result_; // 응답 결과
    PartyFindingKeyType join_wanted_party_finding_key_; // 참가 신청한 파티 모집 키
    CHARGUID join_wanted_user_id_; // 참가 신청한 유저

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 취소 요청
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_CANCEL_SYN)
    PartyFindingKeyType cancel_wanted_party_finding_key_; // 취소 하려는 파티 모집 키

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

// 파티 모집 등록/수정/참가/취소 요청에 대한 응답
PACKET_CW_OPEN(CW_PARTY, CW_PARTY_FINDING_RESULT_ACK)
    PartyFindingResponseTypeConst::ValueType response_type_; // 응답 형식
    RC::PartyFindingResultUtil::ValueType result_; // 요청에 대한 처리 결과
    PartyFindingKeyType party_finding_key_; // 처리를 요청한 파티 모집 키

    WORD GetSize() const
    {
        return static_cast<WORD>(sizeof(*this));
    }
PACKET_CW_CLOSE

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)
