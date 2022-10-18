#pragma once
#ifndef __CHAT_DIALOG_DEF_H__
#define __CHAT_DIALOG_DEF_H__

#define SHOW_DEFAULT_CHAT_MSG_LINE		6
#define SHOW_EXTEND_CHAT_MSG_LINE		14
#define MAX_CHAT_TOKEN_LEN				4
#define MAX_CHAT_REMAIN_MESSAGE			300
#define MAX_CHAT_FILTER_REMAIN_MESSAGE	50

#define MAX_PENDING_WHISPER_SENDER		10

#ifdef _K2P_ADD_WISPER_WINDOW
#define MAX_CHAT_CHECK	6
#else
#define MAX_CHAT_CHECK	5
#endif

#include "StringTableInfo.h"

const int c_iOneLineYPostionToQuickSlot = 50;
const int c_iTwoLineYPostionToQuickSlot = 82;
const int c_iAddYSizeofSystemChat = 45;
const int c_iAddXSizeofChatProperty = 2;
const int c_iAddYSizeofChatPersonalOption = 5;

const DWORD c_dwFilterListTextColor		= WzColor_RGBA (255, 255, 255, 255);
const DWORD c_dwFilterListSelTextColor	= WzColor_RGBA(0,220,0,255);
const DWORD c_dwFilterListBackColor		= WzColor_RGBA (0,0,0,255);
const WzID	c_Font_FilterList			= StrToWzID("mn12");
const int	c_iFilterMenuMax			= 3;
const int	c_iFilterListWidth			= 30;
#ifdef _RUSSIA
const int   c_iLIMITED_EDIT_TEXT        = 96;
#else
const int   c_iLIMITED_EDIT_TEXT        = 64;
#endif //_RUSSIA

enum eLIST_FILTER_MENU
{
	eLFM_NORMAL,
	eLFM_PARTY,
	eLFM_GUILD,
	eLFM_CHANNEL,
	eLFM_SYSTEM,
#ifdef _K2P_ADD_WISPER_WINDOW
	eLFM_WISPER,
#endif
};

enum eLIST_CONTROL_TYPE
{
	LIST_CONTROL_1X,
	LIST_CONTROL_2X,
	LIST_CONTROL_MAX,
};

#endif	//	__CHAT_DIALOG_DEF_H__