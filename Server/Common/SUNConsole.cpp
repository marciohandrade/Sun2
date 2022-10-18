#include "stdafx.h"
#include ".\sunconsole.h"

BOOL SUNConsole::Init( eCONSOLE_TYPE eType )
{
	m_eType = eType;
	switch( eType )
	{
	case eCOMPLEX_CONSOLE:
		{
			CoInitialize(NULL);

			// 솔라콘솔 콘솔창 초기화
			HRESULT hr;	
			hr = CoCreateInstance( CLSID_SOLAR_CONSOLE, NULL, CLSCTX_INPROC_SERVER, IID_SOLARConsole, (void**)&m_pIConsole );	
			if( FAILED( hr ) ) 
			{
				return FALSE;
			}

			LOGFONT logFont;
			logFont.lfHeight			= 17; 
			logFont.lfWidth				= 0; 
			logFont.lfEscapement		= 0; 
			logFont.lfOrientation		= 0; 
			logFont.lfWeight			= FW_BOLD; 
			logFont.lfItalic			= 0; 
			logFont.lfUnderline			= 0; 
			logFont.lfStrikeOut			= 0; 
			logFont.lfCharSet			= HANGUL_CHARSET; 
			logFont.lfOutPrecision		= 0; 
			logFont.lfClipPrecision		= 0; 
			logFont.lfQuality			= 0; 
			logFont.lfPitchAndFamily	= 0; 
			strcpy(logFont.lfFaceName, "굴림"); 

			HWND hWnd;

			SOLAR_CONSOLE_DESC	desc;
			desc.pszConsoleTitle					= "Game Server - SUN Online Server Console";
			desc.pszLogFileName						= "GameServer";
			desc.dwConsoleStyle						= SCS_ENABLE_WRITETOFILE;
			desc.dwRefreshRateDelayTime				= 1000;
			desc.dwNoMoreRefreshDelayTime			= 16000;
			desc.dwBufferingLineNum					= 2000;
			desc.wConsoleWidthSize					= 800;
			desc.wConsoleHeightSize					= 400;
			desc.pLogFont							= &logFont;
			desc.fnCommand							= NULL;
			/*
			SOLAR_MENU_INFO pMenu[3];
			pMenu[0].pszMenuName					= "abc1";
			pMenu[0].cbMenuCommand					= Menu1;
			pMenu[1].pszMenuName					= "abc2";
			pMenu[1].cbMenuCommand					= Menu2;
			pMenu[2].pszMenuName					= "abc3";
			pMenu[2].cbMenuCommand					= Menu3;
			*/
			desc.wMenuNum							= 0;
			desc.pMemnuInfo							= NULL;

			return m_pIConsole->CreateConsole( desc, &hWnd );
			
		}
		break;
	case eSIMPLE_CONSOLE:
		{
			CONSOLE_DESC cdesc;
			ZeroMemory( &cdesc, sizeof(cdesc) );
			cdesc.bUsingConsoleInput	= FALSE;
			cdesc.bUsingRedirect		= TRUE;
			m_pSimpleConsole = new SolarSimpleConsole;
			m_pSimpleConsole->Init( &cdesc );
		}
		break;
	}

	return TRUE;
}


BOOL SUNConsole::Update()
{
	return m_pIConsole->MessageUpdate();
}

VOID SUNConsole::Release()
{
	switch( m_eType )
	{
	case eCOMPLEX_CONSOLE:
		{
			if( m_pIConsole )
			{
				m_pIConsole->DestroyConsole();
				m_pIConsole->Release();
				m_pIConsole = NULL;
				CoUninitialize();
			}
		}
		break;
	case eSIMPLE_CONSOLE:
		{
			if( m_pSimpleConsole ) 
			{
				m_pSimpleConsole->Release();
				delete m_pSimpleConsole;
				m_pSimpleConsole = NULL;
			}
		}
		break;
	}

}