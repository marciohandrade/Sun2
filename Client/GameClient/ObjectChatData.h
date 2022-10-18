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

	void ProcessChatTime(DWORD dwTick);							// 업데이트
	void RenderChatMessage( const WzVector& Position );			// 랜더
	void SetChatColor(WzColor value) { m_ChatColor = value; }	// 색상 설정


	int GetChatDisplayTime() const { return m_iChatDisplayTime; }
	void SetChatDisplayTime(int value) { m_iChatDisplayTime = value; }

public:
	void AddChatMessage( LPCTSTR szChat, DWORD dwTextColor = NULL);	// 대화 추가

};