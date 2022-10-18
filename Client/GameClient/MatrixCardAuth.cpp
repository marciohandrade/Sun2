#include "SunClientPrecompiledHeader.h"

#ifdef __CN_269_MATRIX_CARD_AUTH

#include "MatrixCardAuth.h"
#include "GlobalFunc.h"
#include "uiPasswordMan/uiPassWordMan.h"
#include "uipassworddlg/uiPasswordDlg.h"
#include "interfacemanager.h"

#include "FTextParser.h"
#include "FTextSentence.h"

WzID MatrixCardAuth::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("ST03"),
	StrToWzID("ST06"),
	StrToWzID("ST07"),
	StrToWzID("ST02"),
	StrToWzID("ST01"),
	StrToWzID("ST08"),
	StrToWzID("BT14"),
	StrToWzID("CT00"),
	StrToWzID("CT01"),
	StrToWzID("CT02"),
};
//------------------------------------------------------------------------------
/**
*/
MatrixCardAuth::MatrixCardAuth(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
MatrixCardAuth::~MatrixCardAuth()
{

}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	for( int i = 0 ; i < MAX_TXT_CONTROL ; ++i)
	{
		m_pStatic[i] =  (CCtrlStaticWZ* )getControl( i);
	}

	GlobalFunc::SetCaption(m_pStatic[DLGPOS_TXT_SUB_TITLE] , _T("") );
	GlobalFunc::SetCaption(m_pStatic[DLGPOS_TXT_SYSTEM] , _T(""));
	GlobalFunc::SetCaption(m_pStatic[DLGPOS_TXT_MAIN] , _T(""));

	m_FtextRender.Init( g_pSunRenderer , m_pStatic[DLGPOS_TXT_MAIN]->GetSizeRect());	

	for ( int i = 0; i < MAX_CHECK_CONTROL ; ++i)
	{
		m_pCheck[i] = (CCtrlButtonCheckWZ *) getControl(i +START_CHECK_CONTROL);
	}

	m_pButton = (CCtrlButtonWZ *) getControl(DLGPOS_BTN_EXIT);

#ifndef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
	for( int i = 0; i< NUM_INPUT_MATRIX_CODE ; ++i)
	{
		m_InputData[i].bInput = FALSE;
		m_InputData[i].Value = 0;
	}
#endif
}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::OnUpdateSolarDialog()
{
#ifndef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
	if( IsFinishInput() )
	{
		this->NET_SEND_C2S_ANS_SCAUTH();
	}
#endif
}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::OnRenderSolarDialog()
{
	m_FtextRender.SetRect(m_pStatic[DLGPOS_TXT_MAIN]->GetSizeRect());
	m_FtextRender.Render();
}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		InitFormatText();
	}
	else
	{
		uiPasswordMan * pPasswordMan = (uiPasswordMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD);
		if( pPasswordMan )
		{
			pPasswordMan->m_pUIPasswordDlg->ShowInterface(FALSE);
		}

	}
}
//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_CHK_INPUT1:
			case DLGPOS_CHK_INPUT2:
			case DLGPOS_CHK_INPUT3:
				{
					//유저 입력으로 체크를 컨트롤 하지 못하게 한다.
					for ( int i = 0; i < MAX_CHECK_CONTROL; ++i )
					{
#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
						m_pCheck[i]->SetCheckState( FALSE);
#else
						m_pCheck[i]->SetCheckState( m_InputData[i].bInput);
#endif
					}
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
void MatrixCardAuth::NetworkProc(MSG_BASE * pMsg)
{

}

//------------------------------------------------------------------------------
/**
*/
void MatrixCardAuth::InitializeMatrixCode(SC_INFO * pSCInfo)
{
	for( int i = 0; i <NUM_INPUT_MATRIX_CODE; ++i)
	{
		// 메트릭스 코드 포멧에 따라 출력 해준다.
		// A~ I , 1~ 9
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
		int row = pSCInfo->wMatrix[i] % 9;
		int column = pSCInfo->wMatrix[i] /9;
		int alphabetA = 65;
		int num1	= 49;
		szText[0] = alphabetA + row;
		szText[1] = num1 + column;

		GlobalFunc::SetCaption(m_pStatic[i] , szText);
		m_pCheck[i]->SetCheckState(FALSE);
	}

#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
	SetNoticeMessage();
#else
	m_InputData[i].bInput = FALSE;
	m_InputData[i].Value = 0;
	SetNoticeMessage(DLGPOS_TXT_INPUT1);
#endif //__CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT 


	uiPasswordMan * pPasswordMan = (uiPasswordMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD);
	if( pPasswordMan )
	{
#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
		pPasswordMan->PasswordDlg( 1231 ,  GlobalFunc::MatrixCodeSubmit, 1 , 9 , TRUE);
#else
		pPasswordMan->PasswordDlg( 1231 ,  GlobalFunc::MatrixCodeSubmit, 1 , 3 , TRUE);
#endif
		RECT rt;
		this->GetDialogWZ()->GetClientRect(&rt);
		pPasswordMan->m_pUIPasswordDlg->MoveWindowWZ( rt.right , rt.top );
	}

}

//------------------------------------------------------------------------------
/**
*/
#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
void MatrixCardAuth::SEND_AUTH_MATRIX_CARD_ACK(int iMatrixCode)
{
	MSG_CF_AUTH_MATRIX_CARD_ACK SendPacket;
	memcpy(SendPacket.m_SecureInfo.byData , &iMatrixCode , sizeof( iMatrixCode) );
	GlobalFunc::SendPacket(CI_AUTHSERVERIDX , &SendPacket , sizeof( SendPacket) );
	ShowInterface(FALSE);
}

void MatrixCardAuth::SetNoticeMessage()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,} , szResult[INTERFACE_STRING_LENGTH] ={0,};
	// 1251	[%s][%s][%s]를 순서대로 연속 입력하세요
	g_InterfaceManager.GetInterfaceString(1251, szMessage ,INTERFACE_STRING_LENGTH);
	char szCodeName1[INTERFACE_STRING_LENGTH] ={0,} , szCodeName2[INTERFACE_STRING_LENGTH] ={0,} , szCodeName3[INTERFACE_STRING_LENGTH] ={0,};
	m_pStatic[DLGPOS_TXT_INPUT1]->GetTextWZA(szCodeName1, INTERFACE_STRING_LENGTH);
	m_pStatic[DLGPOS_TXT_INPUT2]->GetTextWZA(szCodeName2, INTERFACE_STRING_LENGTH);
	m_pStatic[DLGPOS_TXT_INPUT3]->GetTextWZA(szCodeName3, INTERFACE_STRING_LENGTH);
	sprintf( szResult, szMessage, szCodeName1 , szCodeName2, szCodeName3 );
	GlobalFunc::SetCaption(m_pStatic[DLGPOS_TXT_SYSTEM] , szResult);
	
}
#else // __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
void MatrixCardAuth::SEND_AUTH_MATRIX_CARD_ACK()
{
	SC_INFO SCInfo;
	for ( int i = 0; i < NUM_INPUT_MATRIX_CODE; ++i)
	{
		SCInfo.m_wMatrix[i] = m_InputData[i].Value;

		// 입력값 초기화
		m_InputData[i].bInput = FALSE;
	}

	MSG_CF_AUTH_MATRIX_CARD_ACK SendPacket;
	memcpy(SendPacket.m_SecureInfo.m_byData , &SCInfo , sizeof( SCInfo) );
	
	GlobalFunc::SendPacket(CI_AUTHSERVERIDX , &SendPacket , sizeof( SendPacket) );
	ShowInterface(FALSE);
}

void MatrixCardAuth::SetNoticeMessage( eDIALOG_POS eDlgPos)
{
	if ( eDlgPos >= DLGPOS_TXT_INPUT1 && eDlgPos <= DLGPOS_TXT_INPUT3)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,} , szResult[INTERFACE_STRING_LENGTH] ={0,};
		// 1251	[%s] 칸의 코드를 입력해주세요.
		g_InterfaceManager.GetInterfaceString(1251, szMessage ,INTERFACE_STRING_LENGTH);
		char szCodeName[INTERFACE_STRING_LENGTH] ={0,};
		m_pStatic[eDlgPos]->GetTextWZA(szCodeName);
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1, szMessage , szCodeName);
		GlobalFunc::SetCaption(m_pStatic[DLGPOS_TXT_SYSTEM] , szResult);
	}
}
void MatrixCardAuth::SetMatrixCardCode(int val)
{
	if( !m_InputData[0].bInput)
	{
		m_InputData[0].bInput = TRUE;
		m_InputData[0].Value = val;
		m_pCheck[0]->SetCheckState(TRUE);
		SetNoticeMessage(DLGPOS_TXT_INPUT2);

	}
	else if ( !m_InputData[1].bInput)
	{
		m_InputData[1].bInput = TRUE;
		m_InputData[1].Value = val;
		m_pCheck[1]->SetCheckState(TRUE);
		SetNoticeMessage(DLGPOS_TXT_INPUT3);
	}
	else if ( !m_InputData[2].bInput )
	{
		m_InputData[2].bInput = TRUE;
		m_InputData[2].Value = val;
		m_pCheck[2]->SetCheckState(TRUE);
	}
}

BOOL MatrixCardAuth::IsFinishInput()
{
	return ( m_InputData[0].bInput && m_InputData[1].bInput && m_InputData[2].bInput);
}
#endif //__CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT

void MatrixCardAuth::InitFormatText()
{
	FTextParser		FTParser;
	FTextSentence	FTSentence;
	FTParser.SetDefaultFont(StrToWzID("mn12") , 0xFFFFFFFF);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	g_InterfaceManager.GetInterfaceString( 1232 , szMessage ,INTERFACE_STRING_LENGTH);
	BOOL ret = FTParser.Parser(szMessage , FTSentence);
	if( ret )
	{
		m_FtextRender.ClearSentence();
		m_FtextRender.Append(FTSentence);
		RECT rt = m_pStatic[DLGPOS_TXT_MAIN]->GetSizeRect();
		m_FtextRender.SetRect( rt );
	}

}


#endif // __CN_269_MATRIX_CARD_AUTH
