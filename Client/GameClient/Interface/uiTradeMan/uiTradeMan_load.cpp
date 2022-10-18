//------------------------------------------------------------------------------
//  uiTradeMan_load.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTradeMan.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#else
#include "uiTrade/uiTrade.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "interfacemanager.h"
#include "uiInPutNumTradeUIDialog/uiInPutNumTradeUIDialog.h"
//------------------------------------------------------------------------------
/**
*/
const DWORD IM_TRADE_MANAGER::TRADE						= StrToWzID("0720");
const DWORD IM_TRADE_MANAGER::TRADE_NUMBER_KEYBOARD		= StrToWzID("0108");

BOOL uiTradeMan::_LoadUI( void )
{
	m_pUITrade				= CreateUIDialog<uiTrade>( IM_TRADE_MANAGER::TRADE, "Data\\Interface\\72_Trade.iwz", this );
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
	m_pUINumTradeUIDialog	= CreateUIDialog<uiInPutNumTradeUIDialog>( IM_TRADE_MANAGER::TRADE_NUMBER_KEYBOARD, "Data\\Interface\\1_8_VirtualKeyboard.iwz", this );
#endif

	return TRUE;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------