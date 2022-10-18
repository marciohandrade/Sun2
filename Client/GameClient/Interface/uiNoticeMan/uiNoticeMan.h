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

	// �������� �����ֱ� - ���� ������� ������ �ִٸ� ����Ʈ�� ��´�.(bForce�� �ٷ� ��°���)
	void ShowNoticeMessage(eNoticeType eType, const TCHAR* pNoticeMessage, int DisplayTime, bool bForce = false);
	// ������̴� UI�� ������ ȣ��, ����Ʈ�� ������ ����ִٸ� ���� ���� ���
	void ShowNextNotice();

	
private:	

private:
	uiNoticeDialog* m_pNoticeDialog;
	uiGuildNoticeDialog* m_pGuildNoticeDialog;

	eNoticeType m_ShowNoticeType;
	LIST_NOTICE m_NoticeList;
};
/*
	* �� ���̾�αװ� ������쿡 ���� �ڵ� ���� �ʿ�
*/
