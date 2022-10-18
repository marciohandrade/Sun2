//=======================================================================================================================
/// ChatMsg - 채팅관련 type 정의
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since
			2005. 1. 23
	@remarks
			- 채팅 필터링
			- 		
*/

#pragma once

#define CHAT_MESSAGE_DELAY		(SEC_PER_TICK * 30)	// 메시지 딜레이 타임.  
#define CHAT_HIDE_DELAY			SEC_PER_TICK  * 8					// Hide Delay


enum eCHAT_TYPE
{
	CHAT_TYPE_GENERAL,
	CHAT_TYPE_WHISPER,
	CHAT_TYPE_PARTY,
	CHAT_TYPE_GUILD,
	CHAT_TYPE_TRADE,
	CHAT_TYPE_SYSTEM,
	CHAT_TYPE_GM,
	CHAT_TYPE_GM_WHISPER,
	CHAT_TYPE_HELP,
	CHAT_TYPE_SHOUT,
	CHAT_TYPE_CHANNEL,
	CHAT_TYPE_WORLD_SHOUT,
	CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT,
    CHAT_TYPE_GM_NOTICE,
    // _100416_LTJ_MODIFY_CHAT_FILTERING
    CHAT_TYPE_SLANG_FILTERING,
    CHAT_TYPE_SLANG_FILTERING_NOTICE,

	CHAT_TYPE_MAX
};


enum eCHAT_FILTER_TYPE
{
	CHAT_FILTER_TYPE_GUILD		= 0x01,
	CHAT_FILTER_TYPE_GENERAL	= 0x02,
	CHAT_FILTER_TYPE_PARTY		= 0x08,
	CHAT_FILTER_TYPE_CHANNEL	= 0x10,
	CHAT_FILTER_TYPE_SHOUT		= 0x04,
	CHAT_FILTER_TYPE_SYSTEM		= 0x20,
	CHAT_FILTER_TYPE_WHISPER	= 0x40,
	CHAT_FILTER_TYPE_MAX	 = 3,
};

struct ChatMsg
{
	eCHAT_TYPE		m_eType;
	TCHAR 			m_pszMessage[MSG_CG_CONVERSATION_FIELDCHAT_SYN::_MAX_CHATMSG_SIZE+1];
	TCHAR *			m_pszCharName;
	TCHAR			m_pszCharNameRef[MAX_CLIENT_CHARNAME_LENGTH];	// 임시적으로 가지고 있는 Sender 명 바뀔수도 있다.
	TCHAR			m_pszRecvName[MAX_CLIENT_CHARNAME_LENGTH];		
	DWORD			m_dwSenderKey;
	DWORD			m_dwDelayAppearTick;		// 보여줄시간.
	DWORD			m_dwAccTick;

	DWORD			m_dwRefCount;				// 참조 카운트

	ChatMsg()
	{
		m_pszMessage[0]		= 0;		
		m_pszCharName		= NULL;
		m_pszCharNameRef[0] = 0;
		m_pszRecvName[0]	= 0;
		m_dwSenderKey		= 0;
		m_dwDelayAppearTick = 0;
		m_dwAccTick			= 0;
		m_dwRefCount		= 0;
	}

	~ChatMsg()
	{
	}
};

struct SenderName
{
	TCHAR		m_szName[MAX_CLIENT_CHARNAME_LENGTH];
	SenderName()
	{
		memset(m_szName , 0 , sizeof(m_szName) );
	}

	~SenderName()
	{

	}
};

