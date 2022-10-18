#include "SunClientPrecompiledHeader.h"
#include "createcharleftdialog.h"
#include "CharInfoParser.h"
#include "CharacterScene.h" 
#include "HeroHeightInfoParser.h"
#include "Mouse.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "GameConst_Color.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"

const int MAX_CHAR_DESCRIPTION_LINE = 9;

CreateCharLeftDialog::CreateCharLeftDialog(InterfaceManager * pUIman)
	:uiBase(pUIman)
{
}

CreateCharLeftDialog::~CreateCharLeftDialog(void)
{
}



WzID CreateCharLeftDialog::m_wzId[CCL_MAX] = {
	StrToWzID("E000"),	
	StrToWzID("VS00"),	
	StrToWzID("L000"),	
	StrToWzID("T012"),	
	StrToWzID("T013"),	
	StrToWzID("T014"),	
	StrToWzID("T015"),	
	StrToWzID("T016"),	

	StrToWzID("B000"),	
	StrToWzID("B001"),	
	StrToWzID("B002"),	
	StrToWzID("B003"),	
	StrToWzID("B004"),	
	StrToWzID("B005"),	

	StrToWzID("T021"),	
	StrToWzID("T022"),	
	StrToWzID("T023"),	
	

};



VOID CreateCharLeftDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	m_TextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

	RECT rc = {0, };
	m_TextRender.Init(g_pSunRenderer, rc);

}


VOID CreateCharLeftDialog::Release()
{
	m_TextSentence.Release();
	m_TextRender.ClearSentence();
}

void
CreateCharLeftDialog::OnRenderSolarDialog()
{
	m_TextRender.Render();
}


VOID CreateCharLeftDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{

			
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch (pos)
			{
			case CCL_HAIR_FOWARD:
				{
					g_CharSelectScene.OnClickChangeHair(TRUE);
				}
				break;
			case CCL_HAIR_BACKWARD:
				{
					g_CharSelectScene.OnClickChangeHair(FALSE);
				}
				break;
			case CCL_FACE_FOWARD:
				{
					g_CharSelectScene.OnClickChangeFace(TRUE);
				}
				break;
			case CCL_FACE_BACKWARD:
				{
					g_CharSelectScene.OnClickChangeFace(FALSE);
				}
				break;
			case CCL_HEIGHT_FOWARD:
				{
					g_CharSelectScene.OnClickChangeHeight(TRUE);
				}
				break;
			case CCL_HEIGHT_BACKWARD:
				{
					g_CharSelectScene.OnClickChangeHeight(FALSE);
				}
				break;
			
			
			
			}
		}
		break;

	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			__asm nop;;
		}
		break;
	
	case RT_MSG_SB_LINETOP:
		{
			//.윗쪽 화살표를 눌렀다.
			//
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

			if(pScroll)
			{
				int index = pScroll->GetScrollPosWZ();

				--index;
				index = max(0, index);
				m_TextRender.SetBeginLineIndex( index );
				pScroll->SetScrollPosWZ( index );

			}		
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

			//.아랫쪽 화살표를 눌렀다
			if (pScroll)
			{
				int index = pScroll->GetScrollPosWZ();

				++index;
				index = min((int)m_TextRender.GetMaxLine(), index);
				m_TextRender.SetBeginLineIndex( index );
				pScroll->SetScrollPosWZ( index );
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:
		{
			//.썸을 드래그 하고 있다.
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

			if ( pScroll )
			{
				int index = pScroll->GetScrollPosWZ();
				m_TextRender.SetBeginLineIndex( index );
				pScroll->SetScrollPosWZ( index );
			}
		}
		break;
	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

			//.썸을 드래그한후 마우스를 놓았다.
			if ( pScroll )
			{
				int index = pScroll->GetScrollPosWZ();
				m_TextRender.SetBeginLineIndex( index );
				pScroll->SetScrollPosWZ( index );
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


VOID CreateCharLeftDialog::OnUpdateSolarDialog()
{
	UpdateVariationText();

	CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( CCL_LIST );

	CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );


	if (pLCtrl && pScroll) 
	{
		m_VscrInfo.ScrollRange.iMax=m_TextRender.GetMaxLine();

		pScroll->SetScrollRangeWZ(0, m_VscrInfo.ScrollRange.iMax);
	}

}


//------------------------------------------------------------------------------
/**
*/
BYTE
CreateCharLeftDialog::GetMouseWheelStatus(const RECT* rc /* =NULL */)
{
	if (!rc)
		return g_Input.GetMouseWheel();

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	if (rc->left <= iMouseX && rc->right >= iMouseX)
	{
		if (rc->top <= iMouseY && rc->bottom >= iMouseY)
		{
			return g_Input.GetMouseWheel();
		}
	}

	return 0;
}


//-------------------------------------------------------------------------------------------
/**
*/
void					
CreateCharLeftDialog::updateMouseWheel()
{
	CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( CCL_LIST );

	CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

	
	if (pLCtrl&&pScroll) 
	{
		m_VscrInfo.ScrollRange.iMax=m_TextRender.GetMaxLine();

		pScroll->SetScrollRangeWZ(0, m_VscrInfo.ScrollRange.iMax);

		RECT rc;
		// QuestList
		rc = pLCtrl->GetSizeRect();
		switch (GetMouseWheelStatus(&rc))
		{
		default:
			break;
		case MOUSE_WHEEL_UP:
			{
				//.윗쪽 화살표를 눌렀다.
				int index = pScroll->GetScrollPosWZ();

				--index;
				index = max(0, index);
				m_VscrInfo.iStartDrawIndex = m_VscrInfo.iCurPos = index;
				pScroll->SetScrollPosWZ( index );
				pLCtrl->SetBeginDrawIndex( index );
			}
			break;

		case MOUSE_WHEEL_DOWN:
			{
				//.아랫쪽 화살표를 눌렀다
				int index = pScroll->GetScrollPosWZ();

				++index;
				index = min(pLCtrl->GetListRowCnt(), index);
				m_VscrInfo.iStartDrawIndex = m_VscrInfo.iCurPos = index;			
				pScroll->SetScrollPosWZ( index );
				pLCtrl->SetBeginDrawIndex( index );
			}
			break;
		}

		pScroll->SetScrollRangeWZ(0, m_TextRender.GetMaxLine());

		rc = pLCtrl->GetSizeRect();
		switch (GetMouseWheelStatus(&rc))
		{
		default:
			break;
		case MOUSE_WHEEL_UP:
			{
				//.윗쪽 화살표를 눌렀다.
				int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
				{
					pos--;      // 한줄 감소 시키고 
					pScroll->SetScrollPosWZ(pos);
					m_TextRender.SetBeginLineIndex(pos);

				}
			}
			break;

		case MOUSE_WHEEL_DOWN:
			{
				//.아랫쪽 화살표를 눌렀다
				int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
				{
					pos++;      // 한줄 증가 시키고 
					pScroll->SetScrollPosWZ(pos);
					m_TextRender.SetBeginLineIndex(pos);
				}
			}
			break;
		}
	}
}

CControlWZ * CreateCharLeftDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < CCL_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE	CreateCharLeftDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CCL_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CCL_MAX;

}

//------------------------------------------------------------------------------
/**
*/
VOID CreateCharLeftDialog::OnShowWindow( BOOL val )
{
	if(val)
	{
		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CCL_SCROLL );

		if(pScroll)
		{
			m_TextRender.SetBeginLineIndex( 0);
			pScroll->SetScrollPosWZ( 0);

		}	
	}
}

void CreateCharLeftDialog::SetStat(BYTE byClass)
{
	INIT_CHARINFO *pInfo = CharInfoParser::Instance()->GetInitCharInfo(byClass);

	TCHAR	szText[INTERFACE_STRING_LENGTH];
	TCHAR	szString[INTERFACE_STRING_LENGTH];
	SUN_STRING	strSunString;

	if (pInfo)
	{
		
		CCtrlStaticWZ *pText = NULL;
		pText = (CCtrlStaticWZ *)getControl(CCL_CHAR_STR);
		Sprintf(szString,_T("%d"),pInfo->m_sStrength);
		pText->SetTextWZ(szString);

		pText = (CCtrlStaticWZ *)getControl(CCL_CHAR_DEX);
		Sprintf(szString,_T("%d"),pInfo->m_sDexterity);
		pText->SetTextWZ(szString);

		pText = (CCtrlStaticWZ *)getControl(CCL_CHAR_CON);
		Sprintf(szString,_T("%d"),pInfo->m_sVitality);
		pText->SetTextWZ(szString);

		pText = (CCtrlStaticWZ *)getControl(CCL_CHAR_INT);
		Sprintf(szString,_T("%d"),pInfo->m_sInteligence);
		pText->SetTextWZ(szString);

		pText = (CCtrlStaticWZ *)getControl(CCL_CHAR_SPR);
		Sprintf(szString,_T("%d"),pInfo->m_sSpirit);
		pText->SetTextWZ(szString);


		CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( CCL_LIST );
		if (pLCtrl) {
			pLCtrl->DeleteItemAll();
			pLCtrl->SetBeginDrawIndex(0);
			pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
			pLCtrl->SetFontID(StrToWzID("st10"));
			
			m_TextRender.SetRect( pLCtrl->GetSizeRect() );


			switch(byClass)
			{
			case eCHAR_BERSERKER:
				this->m_TextRender.ClearSentence();

				g_InterfaceManager.GetInterfaceString(10025, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10026, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10027, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10028, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}
				break;

			case eCHAR_DRAGON:
				this->m_TextRender.ClearSentence();

				g_InterfaceManager.GetInterfaceString(10030, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10031, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10032, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10033, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				break;

			case eCHAR_VALKYRIE:
				this->m_TextRender.ClearSentence();

				g_InterfaceManager.GetInterfaceString(10040, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10041, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10042, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10043, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}
				break;

			case eCHAR_MAGICIAN:
				this->m_TextRender.ClearSentence();

				g_InterfaceManager.GetInterfaceString(10045, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10046, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				g_InterfaceManager.GetInterfaceString(10047, szText, INTERFACE_STRING_LENGTH);
				strSunString = szText;
				if ( this->m_TextParser.Parser(strSunString, m_TextSentence) )
				{
					this->m_TextRender.Append(m_TextSentence);
				}

				break;

			case eCHAR_SHADOW:
				break;
			}
		}
		
		
		pLCtrl->SetUnuseSelBar(TRUE);

	}
	
}

void CreateCharLeftDialog::GetCharName(TCHAR *szName)
{
	CCtrlDxEditBox * pE = (CCtrlDxEditBox*) getControl(CCL_CHAR_NAME);
	if (pE)
	{
		pE->GetTextWZ(szName,256);
	}

}


static void InsertItemText(CCtrlListWZ *pList,TCHAR *szText,int iLength)
{
	TCHAR szTemp[512];
	int index = pList->InsertItemBack();

	if (iLength == -1)
	{
		iLength = _tcslen(szText);
	}

	ZeroMemory(szTemp,sizeof(szTemp));
	StrnCopy(szTemp,szText,iLength);

	if (szTemp[0] == _T(' '))
	{
		if (_tcslen(szTemp) >= 1)
		{
			pList->SetItemTextWZ(index,&szTemp[1]);
		}

	}
	else
	{
		pList->SetItemTextWZ(index,szTemp);

	}

	pList->SetItemBKColor(index, 0, RGBA(0,0,0,0));
	pList->SetItemFontID( index, 0, StrToWzID("st10"));
	pList->SetItemTextColor(index,0,RGBA(152,208,142,255));


}

void CreateCharLeftDialog::PutItemText(CCtrlListWZ *pList,TCHAR *szText,int stringlength)
{

	int iControlWidth = 277;
	
	int iCount = 0;
	int iOldCount = 0;
	int iCurSize = 0;

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("st10") );	

	SIZE TextSize;
	BOOL bDraw = FALSE;
	BOOL bMultiByte = FALSE;
	BOOL bOldMultiByte = FALSE;
	
	char szBuf[INTERFACE_STRING_LENGTH];
	while(iCount < stringlength)
	{
		if( _ismbblead(szText[iCount]))
		{
			szBuf[0] = szText[iCount];
			szBuf[1] = szText[iCount + 1];

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPointA(szBuf,2,&TextSize);

			iCurSize += TextSize.cx;

			
			iCount += 2;	
			bDraw = TRUE;
			bMultiByte = TRUE;
		}
 		else		
		{
			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(&(szText[iCount]),1,&TextSize);

			iCurSize += TextSize.cx;

			iCount++;
			bDraw = TRUE;
			bMultiByte = FALSE;
		}

		if (iCurSize >= (iControlWidth - 16) )
		{
			
			InsertItemText(pList,&szText[iOldCount],(iCount - iOldCount));
				
			iOldCount = iCount;
			bDraw = FALSE;
			iCurSize = 0;
		}

		
	}

	// 마지막줄 넣어주기~
	if (bDraw) {
		InsertItemText(pList,&szText[iOldCount],-1);
		
		
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

void CreateCharLeftDialog::InitText()
{
	CCtrlDxEditBox * pE = (CCtrlDxEditBox*) getControl(CCL_CHAR_NAME);
	if (pE) 
	{
		pE->ClearText();
		pE->SetLimitText( MAX_CHARNAME_LENGTH );
		pE->SetColorCaret(c_InterfaceCaretColor);
	}
	
}

void CreateCharLeftDialog::UpdateVariationText()
{

	CCtrlStaticWZ *pText = NULL;
	pText = (CCtrlStaticWZ *)getControl(CCL_TEXT_HAIR);

	TCHAR szString[INTERFACE_STRING_LENGTH];
	TCHAR szText[128];

	int iVariation;

	if (pText)
	{

		iVariation = g_CharSelectScene.GetCurrentHairVariation();

		//	Type %d
		const int	c_iType = 70426;
		g_InterfaceManager.GetInterfaceString( c_iType, szString, INTERFACE_STRING_LENGTH );
		Sprintf(szText,szString,iVariation);
		GlobalFunc::SetCaption( pText, szText );

	}

	
	pText = (CCtrlStaticWZ *)getControl(CCL_TEXT_FACE);

	if (pText)
	{
		iVariation = g_CharSelectScene.GetCurrentFaceVariation();

		//	Type %d
		const int	c_iType = 70426;
		g_InterfaceManager.GetInterfaceString( c_iType, szString, INTERFACE_STRING_LENGTH );
		Sprintf(szText,szString,iVariation);
		GlobalFunc::SetCaption( pText, szText );

	}


	pText = (CCtrlStaticWZ *)getControl(CCL_TEXT_HEIGHT);

	if (pText)
	{

		iVariation = g_CharSelectScene.GetCurrentHeightVariation();

		DWORD dwClass = g_CharSelectScene.GetSelectedClass();

		float fHeight = 0.0f;

		BASE_HeroHeightInfo *pInfo = HeroHeightInfoParser::Instance()->GetHeroHeightInfo(dwClass);

		if (pInfo)
		{
			fHeight = pInfo->m_fBase * pInfo->m_fHeight[iVariation];
		}

		Sprintf(szText,_T("%.2fm"),fHeight);
		GlobalFunc::SetCaption( pText, szText );
	}



}