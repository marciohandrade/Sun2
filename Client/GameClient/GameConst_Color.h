#ifndef _GameConst_Color_h_
#define _GameConst_Color_h_

/**
 *	@file		GameConst_Color.h
 *	@brief		기본 컬러값은 모아놓은 header
 *  @author		Unknown
 *	@version	Unknown
 */

#include "GlobalFunc.h"
// PLAYER 관련 
const WzColor c_TextColor		        = WzColor_RGBA( 226, 213, 152, 255 );
const WzColor c_GMTextColor				= WzColor_RGBA( 255, 0, 255, 255 );
const WzColor c_DeadTextColor			= WzColor_RGBA( 100, 100, 100, 255 );
const WzColor c_NoIdleTextColor			= WzColor_RGBA( 121, 209, 156, 255 );
const WzColor c_EnemyTextColor			= WzColor_RGBA( 125, 0, 0, 255 );
const WzColor c_TimeAttackTextColor		= WzColor_RGBA( 165, 74, 255, 255 );
const WzColor c_FreeChaoColor			= WzColor_RGBA( 255, 128,0, 255 );
const WzColor c_ChaoColor				= WzColor_RGBA( 255, 0, 0, 255 );
const WzColor c_BoraDoriColor			= WzColor_RGBA( 128, 0, 255, 255 );
const WzColor c_VendorColor				= WzColor_RGBA( 157,202,212,255);
const WzColor c_VirtualIDColor			= WzColor_RGBA( 153, 153, 153,255);


// CHARACTOR 관련
const WzColor c_ExpColor              = WzColor_RGBA(255,40,255,255);
const WzColor c_PlayerHPDamageColor   = WzColor_RGBA(255,0,0,255);
const WzColor c_HeroHPDamageColor     = WzColor_RGBA(255,0,0,255);
const WzColor c_PlayerHPHealColor     = WzColor_RGBA(40,255,40,255);
const WzColor c_MonsterHPDamageColor  = WzColor_RGBA(255,255,0,255); // 타플레이어가 몬스터를 떄렸을때
const WzColor c_Hero2MonsterHPDamageColor = WzColor_RGBA(255,255,0,255); // 주인공이 몬스터를 때렸을때
const WzColor c_MonsterHPHealColor    = WzColor_RGBA(40,255,40,255);
const WzColor c_MPDamageColor         = WzColor_RGBA(100,100,20,255);
const WzColor c_MPHealColor           = WzColor_RGBA(255,255,40,255);

enum eDMGPOP_TARGET_TYPE
{
	eDMGPOP_TARGET_PLAYER = 0,
	eDMGPOP_TARGET_MONSTER,
	eDMGPOP_TARGET_MAX,
};

enum eDMGPOP_COLOR_TYPE
{
	eDMGPOP_COLOR_GENERAL = 0,
	eDMGPOP_COLOR_SKILL,
	eDMGPOP_COLOR_CRITICAL,
	eDMGPOP_COLOR_HEAL,
	eDMGPOP_COLOR_EXP,
	eDMGPOP_COLOR_BLOCK,
	eDMGPOP_COLOR_MISS,
	eDMGPOP_COLOR_MAX
};

const WzColor c_PopupColorInfo[eDMGPOP_TARGET_MAX][eDMGPOP_COLOR_MAX] =
{
	{ // 플레이어
		WzColor_RGBA(0xF6,0x02,0x02,0xFF),	// eDMGPOP_COLOR_GENERAL = 0,
		WzColor_RGBA(0xFE,0xFC,0x40,0xFF),	// eDMGPOP_COLOR_SKILL,
		WzColor_RGBA(0xF6,0x02,0x02,0xFF),	// eDMGPOP_COLOR_CRITICAL,
		WzColor_RGBA(40,255,40,255),		// eDMGPOP_COLOR_HEAL,
		WzColor_RGBA(0xFF,0xFF,0xFF,0xFF),	// eDMGPOP_COLOR_EXP,
		WzColor_RGBA(0x4E,0x3D,0xD2,0xFF),	// eDMGPOP_COLOR_BLOCK, 
		WzColor_RGBA(0x4E,0x3D,0xD2,0xFF),	// eDMGPOP_COLOR_MISS,
	},	

	{	// 몬스터
		WzColor_RGBA(0xF8,0xF8,0xF6,0xFF),	// eDMGPOP_COLOR_GENERAL = 0,
		WzColor_RGBA(0xFE,0xFC,0x40,0xFF),	// eDMGPOP_COLOR_SKILL,
		WzColor_RGBA(0xF6,0x02,0x02,0xFF),	// eDMGPOP_COLOR_CRITICAL,
		WzColor_RGBA(40,255,40,255),		// eDMGPOP_COLOR_HEAL,
		WzColor_RGBA(0xFF,0xFF,0xFF,0xFF),	// eDMGPOP_COLOR_EXP,
		WzColor_RGBA(0xF8,0xF8,0xF6,0xFF),	// eDMGPOP_COLOR_BLOCK,
		WzColor_RGBA(0xF8,0xF8,0xF6,0xFF),	// eDMGPOP_COLOR_MISS,
	},	
};

//채팅 관련.
const WzColor c_Chat_PartyColor			= WzColor_RGBA(82,214,82,255);
const WzColor c_Chat_GuildColor			= WzColor_RGBA(202,170,255,255);
const WzColor c_Chat_ChannelColor		= WzColor_RGBA(255,173,74,255);
const WzColor c_Chat_SystemColor		= WzColor_RGBA(211,211,108,255);
const WzColor c_WorldShoutColor			= WzColor_RGBA(237,252,109,255);
const WzColor c_SuperLobbyMasterColor	= WzColor_RGBA(129 ,235 ,216,255);
const WzColor c_Chat_DefaultColor		= WzColor_RGBA(255,255,255,255);
const WzColor c_Chat_WhisperColor		= WzColor_RGBA(231,101,156,255);
const WzColor c_Chat_TradeColor			= WzColor_RGBA(214,165,96,255);
const WzColor c_Chat_GMColor			= WzColor_RGBA(85,172,220,255);
const WzColor c_Chat_GMWhisperColor		= WzColor_RGBA(191,148,255,255);
const WzColor c_Chat_HelpColor			= WzColor_RGBA(231,127,100,255);

const WzColor c_Chat_ShoutColor			= WzColor_RGBA(85,172,220,255);

const WzColor c_Chat_NoticeColor		= WzColor_RGBA(129,254,82,255);


//아이템 관련
const WzColor c_LuckyItemColor = WzColor_RGBA(0,255,240,255);

// Etc
const WzColor c_PCBangSuperRoomColor = WzColor_RGBA(0 , 176, 240,255);
const WzColor c_ITEM1_SuperRoomColor = WzColor_RGBA( 183 , 27, 23, 255);
const WzColor c_NormalRoomColor		 = WzColor_RGBA( 180 , 180, 180, 255); 

//인터페이스 관련.
const DWORD c_InterfaceBKColor	= RGBA(0,0,0,0);
const DWORD c_InterfaceSelColor	= RGBA(255,255,255,255);
const DWORD c_InterfaceTextColor  = RGBA(180,180,180,255);
const DWORD c_InterfaceCaretColor  = RGBA(200,200,200,255);


#endif // _GameConst_Color_h_
