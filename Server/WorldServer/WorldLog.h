#pragma once
//=======================================================================================================================
/// WorldLog
/**
	@history
			- 2006.5.15 : [taiyo] 버그 수정 및 위험한 문자열 처리 수정
*/
//=======================================================================================================================
#include <BaseLog.h>
#include <ChatData.h>
#include <MoneyData.h>
#include <ActionData.h>

class User;
class WorldGuild;

class WorldLog : public BaseLog
{
private:
	ChatData	m_ChatData;
    MoneyData   m_MoneyData;
    ActionData  m_ActionData;
	BOOL		m_bLogAble[CHAT_CHANNEL - CHAT_NORMAL + 1];
    TCHAR		m_pTmpBuffer[MAX_ADDINFO_SIZE+1];

public:
	WorldLog(void);
	~WorldLog(void);

	inline BOOL		IsLogAble( eLOG_CHAT eChatType )	{ return m_bLogAble[eChatType - CHAT_NORMAL]; }
	VOID			SetLogAble();

	//////////////////////////////////////////////////////////////////////////
	// 채팅 로그
	// 1. 일반 채팅
	void WriteNormalChat( User* pUser, int nChannelID, TCHAR* pszChatMsg, DWORD MaxSize );
	// 2. 외치기
	void WriteShoutChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize );
	// 3. 파티 채팅
	void WritePartyChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize );
	// 4. 길드 채팅
	void WriteGuildChat( User* pUser, int nChannelID, WorldGuild* pGuild, TCHAR* szChatMsg, DWORD MaxSize );
	// 5. 귓속말
	void WriteWhisperChat( User* pUser, int nChannelID, User* pTargetUser, TCHAR* szChatMsg, DWORD MaxSize );
	// 6. 친구 채팅(1:1 채팅)
	void WriteFriendChat( User* pUser, int nChannelID, User* pFriend, TCHAR* szChatMsg, DWORD MaxSize );
	// 7. 채널 채팅(채팅방 생성)
	void WriteChannelChat(User* pUser, int nChannelID, TCHAR* ptszChatMsg, DWORD dwMaxSize);

    //////////////////////////////////////////////////////////////////////////
    // 하임 로그
    // 1. 하임 로또 정산 로그(로그타입, 케릭터 고유값, 하임로또 회차, 당첨등수, 당첨금액)
    void LogSettleHeimLotto(int log_type, User* user, CHARGUID char_guid, WORD draw_num, BYTE winner_grade, MONEY amount_cost);
    //////////////////////////////////////////////////////////////////////////
    // 액션 로그
    // implemented by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    void LogAction(int log_type, User* user, const TCHAR* char_name, const TCHAR* additional_info = NULL);
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    //////////////////////////////////////////////////////////////////////////
    // 알림메세지 로그
    void LogNotification(NotificationInfo& notification);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

};
