#include "SunClientPrecompiledHeader.h"
#include "ObjectChatData.h"
#include "GlobalData.h"
#include "GameFunc.h"
#include "InterfaceManager.h"


ObjectChatData::ObjectChatData()
{
	_InitializeBuffer();

	m_ChatColor = WzColor_RGBA(152,206,142,255);
}

//------------------------------------------------------------------------------
/**
*/
void ObjectChatData::_InitializeBuffer()
{
	m_iChatDisplayTime = 0;
	m_iChatLineWidth = 0;

	m_ChatArray.clear();
}

//------------------------------------------------------------------------------
/**
*/
void ObjectChatData::ProcessChatTime(DWORD dwTick)
{
	if (m_iChatDisplayTime) 
	{
		m_iChatDisplayTime -= dwTick;

		if (m_iChatDisplayTime < 0)
		{
			m_iChatDisplayTime = 0;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ObjectChatData::AddChatMessage( LPCTSTR szText , DWORD dwTextColor)
{
	if (g_IsBattle)
    {
		return;
    }


	if (dwTextColor == NULL)
	{
		m_ChatColor = WzColor_RGBA(152, 206, 142, 255);
	}
	else
	{
		m_ChatColor = dwTextColor;
		//SetChatColor(dwTextColor);
	}

    if (szText == NULL)
    {
        return;
    }

	if (_tcslen(szText) < 1)
	{
		return;
	}

	if (szText[0] == '/')
	{
		return;
	}

	// 이전 대화가 있다면 날린다.
	_InitializeBuffer();

	const int CHAT_WINDOW_LENGTH = 230;
	const DWORD CHAT_TIME = 5000;

	int iCount = 0;
	int iOldCount = 0;
	int iCurSize = 0;
	int iOldSize = 0;

	m_iChatDisplayTime = CHAT_TIME;

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));		

	int stringlength = _tcslen(szText);

	SIZE TextSize;
	BOOL bDraw = FALSE;
	BOOL bMultiByte = FALSE;
	BOOL bOldMultiByte = FALSE;

	SIZE testsize;

	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(_T("좋은"), 4, &testsize);
	
	while (iCount < stringlength) 
    {
		if (_ismbblead(szText[iCount]))
		{

			TCHAR szBuf[2];

			szBuf[0] = szText[iCount];
			szBuf[1] = szText[iCount + 1];

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szBuf, 2, &TextSize);

			iOldSize = iCurSize;
			iCurSize += TextSize.cx;

			
			iCount += 2;	
			bDraw = TRUE;
			bMultiByte = TRUE;
		}
 		else		
		{
			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)(&szText[iCount]), 1, &TextSize);// 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망

			iOldSize = iCurSize;
			iCurSize += TextSize.cx;

			iCount++;
			bDraw = TRUE;
			bMultiByte = FALSE;
		}

		if (iCurSize >= (CHAT_WINDOW_LENGTH - 16))
		{
			
			_AddMessage(&szText[iOldCount], (iCount - iOldCount));

			iOldCount = iCount;
			bDraw = FALSE;
			iCurSize = 0;
		}
		
	}

	// 마지막줄 넣어주기~
	if (bDraw) 
	{
		_AddMessage(&szText[iOldCount]);	
	}
}

//------------------------------------------------------------------------------
/**
*/
void ObjectChatData::_AddMessage( LPCTSTR szText,int iStrLength)
{
	if( m_ChatArray.size() >= MAX_CHAT_LINE )
	{
		return;
	}

	if( iStrLength == -1 )
	{
		m_ChatArray.push_back( szText );
	}
	else
	{
		TCHAR buffer[1024];
		ZeroMemory( buffer, 1024 );
		StrnCopy( buffer, szText, iStrLength );

		m_ChatArray.push_back( buffer );
	}
}

//------------------------------------------------------------------------------
/**
*/
void ObjectChatData::RenderChatMessage( const WzVector& Position )
{
	if ( m_ChatArray.empty() == true )
	{
		return;
	}

	if (m_iChatDisplayTime <= 0)
	{
		return;
	}

	
	const WzColor BackColor = WzColor_RGBA(0,0,0,100);

	g_pSunRenderer->Set2DMode(TRUE);

	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();

	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

	
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );


	WzVector wzVisualPos = Position;

	POINT pt2;
	float fDepth;

	g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos, &fDepth);
    fDepth *= 0.998f;
	int iTextYInterval = 16;

	m_iChatLineWidth = 0;

	SIZE textsize;
	for ( size_t i = 0; i < m_ChatArray.size(); ++i )	
	{
		const std::string& Text = m_ChatArray[i];
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)Text.c_str(), Text.size(), &textsize);// 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망

		if ( m_iChatLineWidth < textsize.cx )
		{
			m_iChatLineWidth = textsize.cx;
		}
	}

	



	int width = m_iChatLineWidth;
	int height = m_ChatArray.size() * iTextYInterval - 4; // 맨밑에줄에 자간을 없앤다.

	RECT rc;

	rc.left = pt2.x - (width / 2);
	rc.right = rc.left + width;
	rc.top = pt2.y - height;
	rc.bottom = pt2.y;

	g_InterfaceManager.RenderChatRect(rc, BackColor, fDepth);

	RECT textrc;

	textrc.left = rc.left;
	textrc.right = rc.right;
	textrc.top = rc.top;
	textrc.bottom = textrc.top + iTextYInterval;
		
	for ( size_t i = 0; i < m_ChatArray.size(); ++i )	
	{
		RenderBlockedTextWithDepthTest( m_ChatArray[i].c_str(), &textrc, m_ChatColor, 0xffffff, fDepth );

		textrc.top += iTextYInterval;
		textrc.bottom += iTextYInterval;
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );

	g_pSunRenderer->Set2DMode(FALSE);	
}


