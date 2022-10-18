/*!	
 *	\file		TextDisaplyManager.cpp
 *	\brief		시간이 지나면 페이드아웃 텍스트를 관리하는 메니져
 *	\author		김경표
 *	\todo		제네릭하게 바꿀 필요 있음.
 *	\version	2009/02/02  김경표 (lammy@webzen.co.kr) - doxygen 주석 추가
 *	\version	2008/12/12  김경표 (lammy@webzen.co.kr) - first release
 */

#include "SunClientPrecompiledHeader.h"
#include "TextDisplayManager.h"
#include "Application.h"
#include "uiTargetMan/uiTargetMan.h"
#include "DlgPosInfoParser.h"

Text_Member::Text_Member( std::string str )
:m_str( str )
,m_dwDisplayTime( 0 )
,m_dwDisappearTime( 0 )
,m_bProcess( TRUE )
,m_bDisappear( FALSE )
,m_Alpha( 255 )
,m_Color(WzColor_RGBA( 255, 255, 255, 255 ))
{	
}

Text_Member::Text_Member( std::string str, DWORD Color )
:m_str( str )
,m_dwDisplayTime( 0 )
,m_dwDisappearTime( 0 )
,m_bProcess( TRUE )
,m_bDisappear( FALSE )
,m_Alpha( 255 )
,m_Color(Color)
{	
}

BOOL Text_Member::Process( DWORD dwTick )
{
	if( m_bProcess )
		m_dwDisplayTime += dwTick;

	if( m_dwDisplayTime >= 3000 )
	{
		m_dwDisplayTime		= 0;
		m_bProcess			= FALSE;
		m_bDisappear		= TRUE;
	}

	if( m_bDisappear )
	{
		m_dwDisappearTime += dwTick;

		float fRate = 1.0f - (float)m_dwDisappearTime / (float)3000;

		if( fRate <= 0.0f )
		{
			fRate = 0.0f;
			m_bDisappear = FALSE;
		}

		m_Alpha = (BYTE)( m_Alpha * fRate );

		if( m_Alpha == 0 )
			return FALSE;
	}

	return TRUE;
}

TextDisplayManager::TextDisplayManager()
:m_iHeightPosition(DEFAULT_HEIGHT_POS)
,m_FontID(StrToWzID("mn12"))

{
}

TextDisplayManager::TextDisplayManager( int iHeightPosition, WzID FontID )
:m_iHeightPosition(iHeightPosition)
,m_FontID(FontID)
{
}

BOOL TextDisplayManager::Process( DWORD dwTick )
{
	if( m_TextMemberList.empty() )
		return FALSE;

	TEXT_MEMBER_LIST_ITR_TYPE Itr = m_TextMemberList.begin();
	TEXT_MEMBER_LIST_ITR_TYPE end = m_TextMemberList.end();

	for( ; Itr != end; )
	{
		Text_Member * pText = Itr->get();

		if( pText->Process( dwTick ) == FALSE )
		{
			TEXT_MEMBER_LIST_ITR_TYPE Temp = Itr;
			Itr++;
	
			m_TextMemberList.erase( Temp );

		}
		else
		{
			Itr++;
		}
	}

	return TRUE;
}

BOOL TextDisplayManager::Insert( TEXT_MEMBER_SHARED_PTR_TYPE data )
{
	if( m_TextMemberList.size() >= 10 )
	{
		m_TextMemberList.pop_back();
	}

	m_TextMemberList.push_front( data );

	return TRUE;
}

BOOL TextDisplayManager::Render()
{
	if( m_TextMemberList.empty() )
		return FALSE;

	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( m_FontID );

	POINT pt = g_pApplication->Get2DModeMapping();

	TEXT_MEMBER_LIST_ITR_TYPE Itr = m_TextMemberList.begin();
	TEXT_MEMBER_LIST_ITR_TYPE end = m_TextMemberList.end();

	int count = 0;
	for( ; Itr != end; ++Itr )
	{
		Text_Member * pText = Itr->get();

		SIZE size;
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( pText->GetString(), pText->GetStringSize(), &size );

		int x = pt.x / 2;
		x -= size.cx / 2;
		int y = m_iHeightPosition + (size.cy * count);

		RECT rc = { x, y, x + size.cx, y + size.cy };

		g_pSunRenderer->x_pManagerTextOut->DrawText( pText->GetString(),
			&rc,
			WzColor_ChangeAlpha( pText->GetColor(), pText->GetAlpha() ),
			WzColor_RGBA( 255, 255, 255, 0 ),
			TP_HLEFT,
			ETS_OUTLINE,
			WzColor_RGBA( 0, 0, 0, pText->GetAlpha() ) );
		
		count++;
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );

	return TRUE;
}
