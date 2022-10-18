#pragma once
//=======================================================================================================================
/// WorldLog
/**
	@history
			- 2006.5.15 : [taiyo] ���� ���� �� ������ ���ڿ� ó�� ����
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
	// ä�� �α�
	// 1. �Ϲ� ä��
	void WriteNormalChat( User* pUser, int nChannelID, TCHAR* pszChatMsg, DWORD MaxSize );
	// 2. ��ġ��
	void WriteShoutChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize );
	// 3. ��Ƽ ä��
	void WritePartyChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize );
	// 4. ��� ä��
	void WriteGuildChat( User* pUser, int nChannelID, WorldGuild* pGuild, TCHAR* szChatMsg, DWORD MaxSize );
	// 5. �ӼӸ�
	void WriteWhisperChat( User* pUser, int nChannelID, User* pTargetUser, TCHAR* szChatMsg, DWORD MaxSize );
	// 6. ģ�� ä��(1:1 ä��)
	void WriteFriendChat( User* pUser, int nChannelID, User* pFriend, TCHAR* szChatMsg, DWORD MaxSize );
	// 7. ä�� ä��(ä�ù� ����)
	void WriteChannelChat(User* pUser, int nChannelID, TCHAR* ptszChatMsg, DWORD dwMaxSize);

    //////////////////////////////////////////////////////////////////////////
    // ���� �α�
    // 1. ���� �ζ� ���� �α�(�α�Ÿ��, �ɸ��� ������, ���ӷζ� ȸ��, ��÷���, ��÷�ݾ�)
    void LogSettleHeimLotto(int log_type, User* user, CHARGUID char_guid, WORD draw_num, BYTE winner_grade, MONEY amount_cost);
    //////////////////////////////////////////////////////////////////////////
    // �׼� �α�
    // implemented by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    void LogAction(int log_type, User* user, const TCHAR* char_name, const TCHAR* additional_info = NULL);
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    //////////////////////////////////////////////////////////////////////////
    // �˸��޼��� �α�
    void LogNotification(NotificationInfo& notification);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

};
