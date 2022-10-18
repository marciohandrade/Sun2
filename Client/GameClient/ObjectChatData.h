#pragma once


#define MAX_CHAT_LINE       4
#define MAX_CHAT_LENGTH     128

class ObjectChatData
{
private:
	int					        m_iChatDisplayTime;
	int                         m_iChatLineWidth;
	std::vector<std::string>    m_ChatArray;
	WzColor                     m_ChatColor;

	void _InitializeBuffer();
	void _AddMessage(LPCTSTR szChat, int iStrLength = -1);

protected:
	ObjectChatData();

	void ProcessChatTime(DWORD dwTick);							// ������Ʈ
	void RenderChatMessage( const WzVector& Position );			// ����
	void SetChatColor(WzColor value) { m_ChatColor = value; }	// ���� ����


	int GetChatDisplayTime() const { return m_iChatDisplayTime; }
	void SetChatDisplayTime(int value) { m_iChatDisplayTime = value; }

public:
	void AddChatMessage( LPCTSTR szChat, DWORD dwTextColor = NULL);	// ��ȭ �߰�

};