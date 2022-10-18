#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "TutorialMissionInvite.h"
#include "GlobalFunc.h"
#include "FTextRender.h"
#include "FTextSentence.h"
#include "Hero.h"

WzID TutorialMissionInvite::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("BT03") ,  //eDIALOG_BTN_OK
	StrToWzID("BT00") ,  //eDIALOG_BTN_CANCLE
	StrToWzID("ST01") ,  //eDIALOG_TXT_MAIN

};
//------------------------------------------------------------------------------
/**
*/
TutorialMissionInvite::TutorialMissionInvite(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
TutorialMissionInvite::~TutorialMissionInvite()
{

}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
	m_bShow = FALSE;

	m_pStatic = ( CCtrlStaticWZ * ) getControl(eDIALOG_TXT_MAIN);  

	m_pStatic->SetTextID(NULL);

	RECT rt = m_pStatic->GetSizeRect();
	m_FtextRender.Init(g_pSunRenderer , rt);

m_bTextLoad = TRUE;
	
}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::update()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::render()
{
	RECT rt = m_pStatic->GetSizeRect();
	m_FtextRender.SetRect(rt);
	if(g_pHero && m_bTextLoad)
	{
		AddFormatText();
		m_bTextLoad = FALSE;
	}
	m_FtextRender.Render();
}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::showWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();

		m_bTextLoad = TRUE;
		m_bShow     = TRUE;

	}
	else
	{

		m_bTextLoad = FALSE;
		m_bShow     = FALSE;
	}
}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::MessageProc(SI_MESSAGE * pMessage)
{

	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_BTN_OK:
				{
					m_bShow = FALSE;
				}
				break;

			case eDIALOG_BTN_CANCLE:
				{
	  				m_bShow = FALSE;
    			}
				break;
			}
		}
		break;
	}

}
//------------------------------------------------------------------------------
/**
*/
void TutorialMissionInvite::NetworkProc(MSG_BASE * pMsg)
{

}

void TutorialMissionInvite::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rt = m_pStatic->GetSizeRect();
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,} , szResult[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString(1182 , szMessage ,INTERFACE_STRING_LENGTH);
	
	FParser.SetDefaultFont(StrToWzID("mn12") , 0xFFFFFFFF);
	if( g_pHero )
	{
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , szMessage ,g_pHero->GetName());
		ret = FParser.Parser(szResult , m_FSentence );
	}
	else
	{	
		ret = FParser.Parser(szMessage , m_FSentence);
	}

	if( ret )
	{
		m_FtextRender.ClearSentence();
		m_FtextRender.Append(m_FSentence);
		m_FtextRender.SetRect(rt);
	}
}
