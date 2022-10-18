#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildRankup.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "FTextRender.h"
#include "FTextSentence.h"

WzID uiGuildRankup::m_wzId[DLGPOS_MAX] = 
{
	StrToWzID("B001") ,
	StrToWzID("B002") ,
	StrToWzID("L001") ,
	StrToWzID("T001"), //DLGPOS_TEXT_TITLE,
};


//------------------------------------------------------------------------------
/**
*/
uiGuildRankup::uiGuildRankup(InterfaceManager *pUIMan) :
uiBase(pUIMan) ,
m_bControlsInitialized(FALSE) 
{
	this->m_pGuildMan =
		static_cast<uiGuildMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD));
	assert (m_pGuildMan);

	SetDlgState(eRANKUP_DLG_STATE_RANKUP);
}

//------------------------------------------------------------------------------
/**
*/
uiGuildRankup::~uiGuildRankup()
{
	this->Release();
}


//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_pList = static_cast<CCtrlListWZ * > ( getControl(DLGPOS_LIST) );
	assert(m_pList);

	m_pList->SetUnuseSelBar(FALSE);
	m_pList->SetUseReport(FALSE);

	m_bControlsInitialized = TRUE;
	m_bShow = FALSE;

	RECT rtMain  = m_pList->GetSizeRect();
	m_FtextRender.Init(g_pSunRenderer , rtMain);

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::Release()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = { 0,};
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_BTN_RANKUP:
				{					
					if(eRANKUP_DLG_STATE_RANKUP == m_eDlgState)
					{
						GetManager()->SEND_CG_GUILD_RANKUP_SYN();
					}
					else if(eRANKUP_DLG_STATE_GUILD_WAREHOUSE_CONFIRM == m_eDlgState)
					{
						GetManager()->SEND_CG_GUILD_WAREHOUSE_OPEN_SYN();
					}

					m_bShow =FALSE;
					
				}
				break;

			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::Process(DWORD dwTick)
{


}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::OnShowWindow( BOOL val )
{
	if( val )
	{
		AddFormatText();

		assert( m_bControlsInitialized );
		if( ! m_bControlsInitialized)
		{
			ShowInterface(FALSE);
			return;
		}
		m_bShow = TRUE;

	}
	else
	{
		m_bShow = FALSE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		this->ShowInterface(FALSE);
		return;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::OnRenderSolarDialog()
{
	RECT rt = m_pList->GetSizeRect();
	rt.right -= 15;
	m_FtextRender.SetRect(rt);
	m_FtextRender.Render();

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRankup::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rtMain		   = m_pList->GetSizeRect();

	TCHAR szMain[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTitle[INTERFACE_STRING_LENGTH +1] = {0,};
	TCHAR szOK[INTERFACE_STRING_LENGTH + 1] = {0,};

	if(eRANKUP_DLG_STATE_GUILD_WAREHOUSE_CONFIRM == m_eDlgState)
	{
		//6194	1. ��� â���� ������ ���� ������ �����ϳ�, ���� ������ �ο� ���� ������ �����մϴ�.
		//<return=1/>2. â�� �̿��� 1ĭ �� 300���� �̸�, ���� �� �ڵ����� ������ ����˴ϴ�.
		//<return=1/>3. �ⳳ ������ ���� 1000������ Ȯ���� �� �ֽ��ϴ�.
		//<return=1/>4. �Ʒ� ��� â�� �̿� ��ư�� Ŭ���� �� â�� �̿��� �ֽñ� �ٶ��ϴ�.
		g_InterfaceManager.GetInterfaceString(6194, szMain);
		//1223	��� â�� �̿� ���
		g_InterfaceManager.GetInterfaceString(1223, szTitle);
		//1224	��� â�� �̿�
		g_InterfaceManager.GetInterfaceString(1224, szOK);
#ifdef _INTERNATIONAL_UI
		FParser.SetDefaultFont(StrToWzID("st10") , RGBA(235,224,200,255));
#else
		FParser.SetDefaultFont(StrToWzID("mn12") , RGBA(235,224,200,255));
#endif//_INTERNATIONAL_UI
	}
	else if(eRANKUP_DLG_STATE_RANKUP == m_eDlgState)
	{
		//1091	2����: 500�� ����, GP����Ʈ 10 <RETURN=1/>3����: 2000�� ����, GP����Ʈ 20 <RETURN=1/>3����: 2000�� ����, GP����Ʈ 20 
		g_InterfaceManager.GetInterfaceString(1091, szMain);
		//1053 ��� ������
		g_InterfaceManager.GetInterfaceString(1053, szTitle);
		//1090 ��� ��ũ��
		g_InterfaceManager.GetInterfaceString(1090, szOK);
#ifdef _INTERNATIONAL_UI
		FParser.SetDefaultFont(StrToWzID("st10") , RGBA(235,224,200,255));
#else
		FParser.SetDefaultFont(StrToWzID("mn12") , RGBA(235,224,200,255));
#endif//_INTERNATIONAL_UI
	}

	CCtrlStaticWZ* pctrlTitle = static_cast<CCtrlStaticWZ*>(getControl(DLGPOS_TEXT_TITLE));
	if(pctrlTitle)
		pctrlTitle->SetTextWZ(szTitle);
	CCtrlButtonWZ* pctrlOK = static_cast<CCtrlButtonWZ*>(getControl(DLGPOS_BTN_RANKUP));
	if(pctrlOK)
		pctrlOK->SetTextWZ(szOK);

	ret = FParser.Parser(szMain , m_FSentence );

	if( ret )
	{
		m_FtextRender.ClearSentence();
		m_FtextRender.Append(m_FSentence);
		m_FtextRender.SetRect(rtMain);
#ifdef _INTERNATIONAL_UI
		m_FtextRender.SetGap(FTextRender::DEFAULT_CHARACTER_GAP,2);
#else
		m_FtextRender.SetGap(FTextRender::DEFAULT_CHARACTER_GAP,10);
#endif//_INTERNATIONAL_UI
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
