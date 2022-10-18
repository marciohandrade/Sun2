#pragma once
#include "uiBaseMan.h"

struct IM_NOTICE_MANAGER
{
	static const DWORD GM_NOTICE_DIALOG;
	static const DWORD GUILD_NOTICE_DIALOG;
};

enum eNoticeType
{
	eNotice_None,
	eGM_Notice,
	eGuild_Notice,
};

struct NOTICE_DATA 
{
	eNoticeType NoticeType;
	std::string NoticeMessage;
	int DisplayTime;

	NOTICE_DATA(eNoticeType eType, const TCHAR* pNoticeMessage, int nDisplayTime)
		:NoticeType(eType),NoticeMessage(pNoticeMessage),DisplayTime(nDisplayTime)
	{
	}
};
typedef std::list<NOTICE_DATA>				LIST_NOTICE;
typedef std::list<NOTICE_DATA>::iterator	LIST_NOTICE_ITR;

class uiNoticeDialog;
class uiGuildNoticeDialog;

class uiNoticeMan : public uiBaseMan
{
public:
	uiNoticeMan(InterfaceManager *pUIMan);

public:
    virtual void OnInitialize();
	virtual void Update();

	// 공지사항 보여주기 - 현재 출력중인 공지가 있다면 리스트에 담는다.(bForce로 바로 출력가능)
	void ShowNoticeMessage(eNoticeType eType, const TCHAR* pNoticeMessage, int DisplayTime, bool bForce = false);
	// 출력중이던 UI가 닫힐때 호출, 리스트에 공지가 담겨있다면 다음 공지 출력
	void ShowNextNotice();

	
private:	

private:
	uiNoticeDialog* m_pNoticeDialog;
	uiGuildNoticeDialog* m_pGuildNoticeDialog;

	eNoticeType m_ShowNoticeType;
	LIST_NOTICE m_NoticeList;
};
/*
	* 각 다이얼로그가 닫힐경우에 대한 코드 변경 필요
*/
