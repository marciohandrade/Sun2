#include "StdAfx.h"
#include ".\listcontrol.h"
#include <stdio.h>

ListControl::ListControl(void)
	:	m_x ( 0 ),
		m_y ( 0 ),
		m_width ( 0 ),
		m_height ( 0 ),
		m_nScrollPos ( 0 ),
		m_nScrollMaxPos ( 0 ),
		m_BGColor ( RGB(10,10,32) ),
		m_FGColor ( RGB(128,128,255) ),
		m_hFont ( NULL ),
		m_hParentWnd ( NULL ),
		m_hScrollWnd ( NULL ),
		m_hDC ( NULL),
		m_pLineList ( NULL ),
		m_pLineNodePool ( NULL )
{
	ZeroMemory( &m_WindowRect, sizeof(RECT) );

	m_hBGPenInlt	= CreatePen( PS_SOLID, 1, FRAMELEFTTOP_INCOLOR );
	m_hBGPenOutlt	= CreatePen( PS_SOLID, 1, FRAMELEFTTOP_OUTCOLOR );
	m_hBGPenInrb	= CreatePen( PS_SOLID, 1, FRAMERIGHTBOTTOM_INCOLOR );
	m_hBGPenOutrb	= CreatePen( PS_SOLID, 1, FRAMERIGHTBOTTOM_OUTCOLOR );
	m_hBGBrush		= CreateSolidBrush( m_BGColor );
}

ListControl::~ListControl(void)
{
	if( m_hBGPenInlt )
		DeleteObject( m_hBGPenInlt );
	if( m_hBGPenOutlt )
		DeleteObject( m_hBGPenOutlt );
	if( m_hBGPenInrb )
		DeleteObject( m_hBGPenInrb );
	if( m_hBGPenOutrb )
		DeleteObject( m_hBGPenOutrb );

	if( m_hBGBrush )
		DeleteObject( m_hBGBrush );

	if( m_hDC )
		ReleaseDC( m_hParentWnd, m_hDC );
}

VOID ListControl::DestroyList()
{
	if( m_pLineList ) 
	{
		ResetContent();
		delete m_pLineList;
		m_pLineList = NULL;
	}

	if( m_pLineNodePool ) 
	{
		m_pLineNodePool->Release();
		delete m_pLineNodePool;
		m_pLineNodePool = NULL;
	}

	if(m_hFont) DeleteObject(m_hFont);
}

VOID ListControl::CreateList( HINSTANCE hInstance, HWND hWnd, LPLOGFONT pLogFont, int x, int y, int width, int height )
{
	m_hParentWnd				= hWnd;
	m_hDC						= GetDC( hWnd );

	OnMove( x, y );
	OnSize( width, height );


	m_hFont						= CreateFontIndirect( pLogFont );
	m_LineDesc.wLineHeight		= (WORD)pLogFont->lfHeight;
	m_LineDesc.wMarginByLine	= 2;
	m_LineDesc.wIndentation		= 2;
	m_LineDesc.wNumLinePerPage  = (WORD)(( m_height - (FRAMEBORDER << 1) ) / ( m_LineDesc.wLineHeight + m_LineDesc.wMarginByLine ));



	m_pLineList					= new util::CLinkedlist<LINENODE *>;
	m_pLineNodePool				= new util::CMemoryPoolFactory<LINENODE>;
	m_pLineNodePool->Initialize( 1000, 500 );

	int scrollX					= 0;
	int scrollY					= 0;
	int scrollWidth				= 0;
	int scrollHeight			= 0;
	GetScrollWindowPos( scrollX, scrollY, scrollWidth, scrollHeight );
	m_hScrollWnd = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, scrollX, scrollY, scrollWidth, scrollHeight, hWnd, NULL, hInstance, NULL);

	UpdateScrollBar();
}

VOID ListControl::OnSize( int cx, int cy )
{
	m_width				= cx;
	m_height			= cy;

	m_WindowRect.right	= m_WindowRect.left + m_width - ( FRAMEBORDER<<1 ) - SCROLLWIDTH;
	m_WindowRect.bottom = m_WindowRect.top + m_height - ( FRAMEBORDER<<1 );

	if( !m_hScrollWnd ) return ;

	int scrollX			= 0;
	int scrollY			= 0;
	int scrollWidth		= 0;
	int scrollHeight	= 0;
	GetScrollWindowPos( scrollX, scrollY, scrollWidth, scrollHeight );
	MoveWindow( m_hScrollWnd, scrollX, scrollY, scrollWidth, scrollHeight, TRUE );

	m_LineDesc.wNumLinePerPage  = (WORD)(( m_height - (FRAMEBORDER << 1) ) / ( m_LineDesc.wLineHeight + m_LineDesc.wMarginByLine ));

	UpdateScrollRange();
}

VOID ListControl::OnMove( int x, int y )
{
	m_x					= x;
	m_y					= y;

	m_WindowRect.left	= x + FRAMEBORDER;
	m_WindowRect.top	= y + FRAMEBORDER;
	m_WindowRect.right	= m_WindowRect.left + m_width - ( FRAMEBORDER<<1 ) - SCROLLWIDTH;
	m_WindowRect.bottom = m_WindowRect.top + m_height - ( FRAMEBORDER<<1 );
}

VOID ListControl::GetScrollWindowPos( int& x, int& y, int& width, int& height )
{
	x			= m_x + m_width - SCROLLWIDTH;
	y			= m_y + FRAMEBORDER;
	width		= SCROLLWIDTH;
	height		= m_height - ( FRAMEBORDER<<1 );
}

VOID ListControl::UpdateScrollRange()
{
	if( GetLineCount() > m_LineDesc.wNumLinePerPage )
	{
		m_nScrollMaxPos = GetLineCount() - m_LineDesc.wNumLinePerPage;
		SetScrollRange( m_hScrollWnd, SB_CTL, 0, m_nScrollMaxPos, TRUE );
	}
	else
	{
		m_nScrollMaxPos = 1;
		SetScrollRange( m_hScrollWnd, SB_CTL, 0, m_nScrollMaxPos, TRUE );
	}
}

VOID ListControl::UpdateScrollBar()
{
	SetScrollPos( m_hScrollWnd, SB_CTL, m_nScrollPos, TRUE);
}

VOID ListControl::InvalidateUpdate()
{
	// outline frame
	DrawRectangle( m_WindowRect.left-FRAMEBORDER, m_WindowRect.top-FRAMEBORDER, m_WindowRect.right+FRAMEBORDER, m_WindowRect.bottom+FRAMEBORDER, m_hBGPenOutlt, m_hBGPenOutrb );

	// inner bg
	SelectObject( m_hDC, m_hBGBrush );
	Rectangle( m_hDC, m_WindowRect.left-(FRAMEBORDER>>1), m_WindowRect.top-(FRAMEBORDER>>1), m_WindowRect.right+FRAMEBORDER, m_WindowRect.bottom+FRAMEBORDER );

	// inline frame
	DrawRectangle( m_WindowRect.left-(FRAMEBORDER>>1), m_WindowRect.top-(FRAMEBORDER>>1), m_WindowRect.right+(FRAMEBORDER>>1), m_WindowRect.bottom+(FRAMEBORDER>>1), m_hBGPenInlt, m_hBGPenInrb);

	SetBkColor( m_hDC, m_BGColor );

	HFONT oldFont = (HFONT)SelectObject( m_hDC, m_hFont );

	
	char m_tmpEndLine[255];
	LINENODE * pNode = NULL;
	int xPos = 0;
	int numLine = GetLineCount();
	POS pos = m_pLineList->GetIndexPos(m_nScrollPos);
	for( int i = m_nScrollPos, j = 0 ; i < m_nScrollPos + m_LineDesc.wNumLinePerPage && i < numLine ; ++i, ++j )
	{
		pNode = m_pLineList->GetNextPos(pos);

		int lineH = ( m_LineDesc.wLineHeight + m_LineDesc.wMarginByLine );
		int yPos = m_WindowRect.top + lineH * j;

		SetTextColor( m_hDC, pNode->rgb);
	
		xPos = m_WindowRect.left + m_LineDesc.wIndentation;

		sprintf( m_tmpEndLine, "%3d   %s", i, pNode->szLine);
		ExtTextOut( m_hDC, xPos, yPos, ETO_CLIPPED, &m_WindowRect, m_tmpEndLine, (UINT)strlen(m_tmpEndLine), 0);

	}

	SelectObject( m_hDC, oldFont );

}

VOID ListControl::DrawRectangle( int left, int top, int right, int bottom, HPEN PenLeftTop, HPEN PenRightBottom )
{
	SelectObject( m_hDC, PenLeftTop );
	MoveToEx( m_hDC, left, top, NULL );
	LineTo( m_hDC, left, bottom );
	MoveToEx( m_hDC, left, top, NULL );
	LineTo( m_hDC, right, top );

	SelectObject( m_hDC, PenRightBottom );
	MoveToEx( m_hDC, right, top, NULL );
	LineTo( m_hDC, right, bottom );
	MoveToEx( m_hDC, left, bottom, NULL );
	LineTo( m_hDC, right+1, bottom );
}

VOID ListControl::MessageProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	__UNUSED(lParam);

	switch ( message ) 
	{
	case WM_HSCROLL:
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
				--m_nScrollPos;
			break;
		case SB_LINEDOWN:
				++m_nScrollPos;
			break;
		case SB_PAGEUP:
				--m_nScrollPos;
			break;
		case SB_PAGEDOWN:
				++m_nScrollPos;
			break;
		case SB_THUMBPOSITION:
				m_nScrollPos = HIWORD(wParam);
			break;
		case SB_THUMBTRACK:
			//if(!(TL_DISABLESCROLLTUMBTRACK & m_dwListStyle))
			{
				m_nScrollPos = HIWORD(wParam);
			}
			break;
		}

		m_nScrollPos = max(0, min(m_nScrollPos, m_nScrollMaxPos));

		UpdateScrollBar();
		InvalidateUpdate();
		
		break;

	}
	
}


DWORD ListControl::AddString( char * szString, DWORD rgb )
{
	LINENODE* pLine = reinterpret_cast<LINENODE*>(m_pLineNodePool->Alloc());
	strncpy( pLine->szLine, szString, MAX_LINE_LENGTH );
	pLine->rgb = rgb;
	m_pLineList->AddTail( pLine );

	if( GetLineCount() > m_LineDesc.wNumLinePerPage )	m_nScrollPos = GetLineCount() - m_LineDesc.wNumLinePerPage;

	UpdateScrollRange();
	UpdateScrollBar();

	return GetLineCount();
}

BOOL ListControl::DeleteString( char * pszDeleteStringOut )
{
	LINENODE * pDelNode = m_pLineList->DeleteHead();
	
	if ( pDelNode != NULL )
	{
		if( pszDeleteStringOut != NULL )
			strncpy( pszDeleteStringOut, pDelNode->szLine, MAX_LINE_LENGTH );

		m_pLineNodePool->Free( pDelNode );
		pDelNode = NULL;
		return TRUE;
	}
	return FALSE;
}

VOID ListControl::ResetContent()
{
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop ) 
	{
		LINENODE * pDelNode = m_pLineList->DeleteHead();

		if (pDelNode == NULL)
		{
			break;
		}

		m_pLineNodePool->Free( pDelNode );
		pDelNode = NULL;
	}
}

char * ListControl::GetString( DWORD index )
{
	if( index < GetLineCount() )
		return m_pLineList->GetAt(index)->szLine;
	return NULL;
}

DWORD ListControl::GetLineCount()
{
	return m_pLineList->GetCount();
}
