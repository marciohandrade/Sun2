#include "SunClientPrecompiledHeader.h"
#include "testcaseformattext.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "QuestTextInfoParser.h"

//-------------------------------------------------------------------------------------------
/**
*/
TestCaseFormatText::TestCaseFormatText(void)
{
	SetName( _T("TestCaseFormatText") );
}

//-------------------------------------------------------------------------------------------
/**
*/
TestCaseFormatText::~TestCaseFormatText(void)
{
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL TestCaseFormatText::Init()
{
	m_rcRECT.left = 100;
	m_rcRECT.top = 100;
	m_rcRECT.right = 300;
	m_rcRECT.bottom = 300;

	FTextParser Parser;
	FTextSentence Sentence;

	m_TextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
  
	this->m_TextRender.Init(g_pSunRenderer, m_rcRECT);
 
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL TestCaseFormatText::Process( DWORD dwTick )
{
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCaseFormatText::Render( DWORD dwTick )
{  	
	RenderText();

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void TestCaseFormatText::Release()
{
	m_TextRender.ClearSentence();
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseFormatText::RenderText()
{
	g_pSunRenderer->Set2DMode( TRUE, FALSE );

	this->m_rcRECT = m_TextRender.GetRect();
	g_InterfaceManager.RenderRect2(m_rcRECT,WzColor_RGBA(0,0,30,160));
	m_TextRender.Render();
    
	_stprintf( m_szTest, _T("Max Line : %d"), m_TextRender.GetMaxLine());
	
	g_pSunRenderer->x_pManagerTextOut->DrawText(
		m_szTest, 
		50, 
		600, 
		WzColor_RGBA(255,255,255,255));
	g_pSunRenderer->Set2DMode( FALSE, FALSE );
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseFormatText::ProcessKeyInput(DWORD dwTick)
{
	if ( g_Input.GetState(DIK_UP, KS_DOWN) ) 
	{
		int iIndex = m_TextRender.GetBeginLineIndex();
		m_TextRender.SetBeginLineIndex( --iIndex );
	}
	
	if ( g_Input.GetState(DIK_DOWN, KS_DOWN) ) 
	{
		int iIndex = m_TextRender.GetBeginLineIndex();
		m_TextRender.SetBeginLineIndex( ++iIndex );
	}	
}

BOOL 
TestCaseFormatText::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3/*= 0*/, int iParam4/*= 0*/)
{
	SUN_STRING	strSunString;
	BOOL bRet = FALSE;

	switch (wMessage)
	{
	case TESTCASE_FT_APPEND_STRING:
		{
			bRet = TRUE;

			DWORD dwStrID = (DWORD)iParam1;

			QuestTextInfo * pInfo =
				QuestTextInfoParser::Instance()->GetQuestTextInfo(dwStrID);

			if ( !pInfo || !pInfo->m_pszText )
			{
				return FALSE;				
			}

			strSunString = pInfo->m_pszText;
			
			if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
			{
				this->m_TextRender.Append(m_TextSentence);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, _T("[%d] Correct String."), dwStrID);
			}
			else
			{
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, _T("[%d Id String] incorrect String."), dwStrID);
			}
		}
		break;

	case TESTCASE_FT_CLEAR_STRING:
		{
			this->m_TextRender.ClearSentence();
			bRet = TRUE;
		}
		break;

	case TESTCASE_FT_SET_RECT:
		{
			RECT rc;
			rc.left = iParam1;
			rc.top	= iParam2;
			rc.right = iParam3;
			rc.bottom = iParam4;

			this->m_TextRender.SetRect(rc);
		}
		break;

	default:
		break;
	}

	return bRet;
}