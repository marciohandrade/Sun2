#ifndef __LIST_CONSTROL_H__
#define __LIST_CONSTROL_H__

#pragma once

#include <Linkedlist.h>
#include <MemoryPoolFactory.h>

#define FRAMELEFTTOP_INCOLOR			RGB(64,64,64)
#define FRAMELEFTTOP_OUTCOLOR			RGB(128,128,128)

#define FRAMERIGHTBOTTOM_INCOLOR		RGB(212,208,200)
#define FRAMERIGHTBOTTOM_OUTCOLOR		RGB(255,255,255)

class ListControl
{
public:
	ListControl();
	~ListControl();

	VOID								DestroyList();
	VOID								CreateList( HINSTANCE hInstance, HWND hWnd, LPLOGFONT pLogFont, int x, int y, int width, int height );

	DWORD								AddString( char * szString, DWORD rgb = RGB(200,200,200) );
	BOOL								DeleteString( char * pszString );
	VOID								ResetContent();
	char *								GetString( DWORD index );
	DWORD								GetLineCount();

	VOID								GetScrollWindowPos( int& x, int& y, int& width, int& height );
	VOID								InvalidateUpdate();
	VOID								MessageProc( UINT message, WPARAM wParam, LPARAM lParam );
	VOID								OnSize( int cx, int cy );
	
private:
	VOID								UpdateScrollRange();
	VOID								UpdateScrollBar();
	VOID								OnMove( int x, int y );
	VOID								DrawRectangle( int left, int top, int right, int bottom, HPEN PenLeftTop, HPEN PenRightBottom );
	enum { MAX_LINE_LENGTH = 256, SCROLLWIDTH = 18, FRAMEBORDER = 2, };
	struct LINENODE
	{
		char	szLine[MAX_LINE_LENGTH];
		DWORD	rgb;
	};
	struct LINEDESC
	{
		WORD	wIndentation;
		WORD	wLineHeight;
		WORD	wMarginByLine;
		WORD	wNumLinePerPage;
	};
	int									m_x;
	int									m_y;
	int									m_width;
	int									m_height;
	int									m_nScrollPos;
	int									m_nScrollMaxPos;
	RECT								m_WindowRect;
	LINEDESC							m_LineDesc;
	
	DWORD								m_BGColor;
	DWORD								m_FGColor;
	HFONT								m_hFont;
	HPEN								m_hBGPenInlt;
	HPEN								m_hBGPenInrb;
	HPEN								m_hBGPenOutlt;
	HPEN								m_hBGPenOutrb;
	HBRUSH								m_hBGBrush;

	HWND								m_hParentWnd;
	HWND								m_hScrollWnd;
	HDC									m_hDC;
	util::CLinkedlist<LINENODE *> *			m_pLineList;
	util::CMemoryPoolFactory<LINENODE> *		m_pLineNodePool;
};


#endif // __LIST_CONSTROL_H__