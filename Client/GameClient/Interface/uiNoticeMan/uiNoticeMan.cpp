#include "SunClientPrecompiledHeader.h"
#include "interfacemanager.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiNoticeDialog/uiNoticeDialog.h"
#include "uiGuildNoticeDialog/uiGuildNoticeDialog.h"
#include "GlobalFunc.h"

const DWORD IM_NOTICE_MANAGER::GM_NOTICE_DIALOG		= StrToWzID("9720");
const DWORD IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG	= StrToWzID("9726");

uiNoticeMan::uiNoticeMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pNoticeDialog = NULL;
    m_pGuildNoticeDialog = NULL;
    m_ShowNoticeType = eNotice_None;
}


//------------------------------------------------------------------------------
/**
*/
void uiNoticeMan::Update()
{
	//if (m_pGuildNoticeDialog 
	//	&& m_pGuildNoticeDialog->IsShowNotice() 
	//	&& FALSE == m_pGuildNoticeDialog->IsVisible())
	//{
	//	m_pGuildNoticeDialog->ShowInterface(TRUE);
	//}
	if (eNotice_None != m_ShowNoticeType 
		&& FALSE == m_pNoticeDialog->IsVisible()
		&& FALSE == m_pGuildNoticeDialog->IsVisible())
	{
		ShowNextNotice();
	}
	// 닫힘 키처리
	static bool LButtonDown = false;
	if (g_Input.GetState( DIK_LCONTROL, KS_KEY ))
	{
		if (g_Input.GetState( DIK_X, KS_DOWN ) && false == LButtonDown)
		{
			if (m_pNoticeDialog->IsVisible())
			{
				m_pNoticeDialog->ShowInterface(FALSE);
				ShowNextNotice();
			}
			else if (m_pGuildNoticeDialog->IsVisible())
			{
				m_pGuildNoticeDialog->ShowInterface(FALSE);
				ShowNextNotice();
			}
			LButtonDown = true;
		}
		else if (LButtonDown && g_Input.GetState( DIK_X, KS_UP ))
		{
			LButtonDown = false;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeMan::OnInitialize()
{
	m_pNoticeDialog = CreateUIDialog<uiNoticeDialog>(IM_NOTICE_MANAGER::GM_NOTICE_DIALOG, "Data\\Interface\\97_20_gm_notice.iwz", this);
	assert(m_pNoticeDialog);
	m_pGuildNoticeDialog = CreateUIDialog<uiGuildNoticeDialog>(IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG, "Data\\Interface\\97_26_guild_notice.iwz", this);
	assert(m_pGuildNoticeDialog);

	//if(m_pNoticeDialog && m_pGuildNoticeDialog)
	//{
	//	return TRUE;
	//}
	//return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
// 공지사항 보여주기 - 현재 출력중인 공지가 있다면 리스트에 담는다.(bForce로 바로 출력가능)
void uiNoticeMan::ShowNoticeMessage(eNoticeType eType, const TCHAR* pNoticeMessage, int DisplayTime, bool bForce)
{
	if (bForce)
	{
		m_NoticeList.push_front(NOTICE_DATA(eType, pNoticeMessage, DisplayTime));

		m_pNoticeDialog->ShowInterface(FALSE);
		m_pGuildNoticeDialog->ShowInterface(FALSE);

		ShowNextNotice();
	}
	else
	{
		m_NoticeList.push_back(NOTICE_DATA(eType, pNoticeMessage, DisplayTime));
		if (eNotice_None == m_ShowNoticeType)
		{
			ShowNextNotice();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
// 출력중이던 UI가 닫힐때 호출, 리스트에 공지가 담겨있다면 다음 공지 출력
void uiNoticeMan::ShowNextNotice()
{
	// 리스트에 공지가 담겨있지 않다면 리턴
	if (m_NoticeList.empty())
	{
		m_ShowNoticeType = eNotice_None;
		return;
	}

	// 공지가 있다면 출력
	LIST_NOTICE_ITR itr = m_NoticeList.begin();
	m_ShowNoticeType = itr->NoticeType;
	switch(m_ShowNoticeType)
	{
	case eGM_Notice:
		{
			m_pNoticeDialog->ShowNoticeMessage(itr->NoticeMessage.c_str(), itr->DisplayTime);
		}
		break;
	case eGuild_Notice:
		{
			m_pGuildNoticeDialog->ShowNoticeMessage(itr->NoticeMessage.c_str(), itr->DisplayTime);
		}
		break;
	default:
		{}
	}	
	m_NoticeList.pop_front();	
}

