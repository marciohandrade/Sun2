/**
 *	@file		GameDefine.h
 *	@brief		�۾� ������ ��Ƴ��� ��
 *  @author		Unknown
 *	@version	Unknown
 */

////------------------------------------------------------------------------------
///**
//
//���� ���ݿ� ���õ� define�� ��Ƶδ� ��
//
//@author
//	���翵 (y2jinc@webzen.co.kr)
//@since
//
//@remarks
//- 
//*/

#ifndef GAMEDEFINE_H
#define GAMEDEFINE_H

#include "GameDefine_KIKI.h"
#include "GameDefine_KJWS78.h"
#include "GameDefine_JJUMBO.h"
#include "GameDefine_Engine.h"
#include "GameDefine_YJ.h"
#include "GameDefine_K2P.h"
#include "GameDefine_GS.h"
#include "GameDefine_Mjkim.h"
#include "GameDefine_LTJ.h"
#include "GameDefine_DH.h"
#include "GameDefine_YMS.h"
#include "GameDefine_SCJ.h"
#include "GameDefine_HSH.h"

//////////////////////////////////////////////////////////////////////////
// ������ ������� Ŭ�� ���� ������
//////////////////////////////////////////////////////////////////////////

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    //#define _YJ_EXECUTE_IN_MAPTOOL                  // 2010.07.26 / ������ / �̼� �׽�Ʈ ȯ�� ���� �۾�
    #define _NA_006519_20130311_TEXTURE_VIEWER      // 2013.03.11 / ȫ���� / Ŭ���̾�Ʈ �ؽ��� ��� ���� �۾�
#endif //_DEV_VER


//////////////////////////////////////////////////////////////////////////
// ������ ������ 
//#define CUSTOMIZE_HEAD
//#define _JSH_BUGFIX__LEAVE_PARTY_					// 2006.04.20 / �������� ��Ƽ Ż�� ����� ������� �ʴ� ����
//#define _JSH_ADD__ENCRYPT_PROTOCOL_					// 2006.03.15 / �������� ��ȣȭ
//#define _JSH_REFACTORING__WORLD_SERVER_CONNECTION_	// 2006.04.03 / ���弭�� ���� �����丵
//#define _JSH_ADD__GAME_EXIT_STANDING_BY_			// 2006.04.12 / ĳ����/�������� �� ���ӿ��� ���� ��, ��õ��� ����ϴ� ���
//#define _JSH_BUGFIX__LEAVE_FROM_PARTY_				// 2006.04.14 / ��Ƽ�� ���� ���, ��Ƽ ���θ� ���� üũ
//#define _JSH_MODIFY__CHANGE_TRADE_PROTOCOL_			// 2006.04.14 / �÷��̾�� �ŷ� : ������ �������� ����
//#define _ADD_REWARD_SCRIPT							// 2006. 4.  7 / ���翵 / ���� ���� ����
//#define _ADD_FIELD_SCENE				// 2006. 4. 11 / ���翵 / �ʵ�� �߰�.
//#define _ADD_FIELD_IN_DEAD_PROCESS		// 2006. 4. 11 / ���翵 / �ʵ忡�� �׾����� ó��
//#define _ADD_ITEM_EXCHANGE				// 2006. 4. 14 / ���翵 / ������ 1:1 ��ȯ (�������� ������ �ȶ�)
//#define _REMOVE_LOCK_KEYBOARD			// 2006. 4. 18 / ���翵 / Ű���� ��ŷ �÷��� ����
//#define _ADD_ITEM_DROP_DELETE_FOR_NOT_DROP_ITEM	// 2006. 4. 26 / ���翵 / ������ ����Ҽ� ���� ������ ����.
//#define UI_RESOLUTION_VARIATION			// 2006. 2. 15 / ������ / UI �ػ󵵺� ������ ��ȯ
//#define MINIMAP_POS_CHANGE
//#define NEW_ENGINE
//#define BLOCKED_TEXT                    // 2006. 2. 17 / ������ / ĳ���� �̸�� ���� �ﰢ���� ����
//#define PLAYER_HIT_DELTA_ANIMATION		// 2006. 3. 22. ĳ���� ������ �ִϸ��̼��� ��Ÿ �ִϷ� ó�� 
//#define MAPOBJECT_USE_WITH_ACTION       // Map������Ʈ�� ��뵵 Action���� ó��
//#define CRITICAL_HIT_ANIMATION
//#define MONSTER_RADIUS					// ���� �ѷ� ���� ó��
//#define AREA_CONQUEST_NEW_PROJECT
//#define UI_AC_DIALOG

//#define MOD_NETWORK_HEARTBEAT						// 2006. 12. 09 / ����ȯ + ������ �۾�
//#define _JBH_ADD_AGENT_ENCRYPT_PROTOCOL
//#define _ADD_CHARGE_ITEM_REPAIR_ASK					// 2006. 12. 18 / ���翵 / ���� ������ ���� �ǰ� ����� �޽��� �߰�.
//#define _JBH_ADD_OPTION_CAMERA_BASE_MOVE			// 2006.2.23    / ����ȯ / ī�޶� ���� �̵�. 
//#define _JBH_ADD_OPTION_CAMERA_BASE_MOVE_CALL_FUNCTION
//#define _JBH_MOD_ARMOR_KIT				// 2006.4.20	/ ����ȯ / �Ƹ�Ŷ 4.11���� ����
//#define _124_KIKI_CASH_ITEM_SHOP					// 2006. 11. 21 / ������ / ���� �����۸�
//
//#define _ADD_QUEST_COMMON_FORWARD		// 2006. 3. 30 / ���翵 / ����Ʈ �ӹ� ���� ��� ǥ�� �� �Ϲ� NPC ��ȭ�� �̿�Ǵ� UI
//#define _ADD_QUEST_REWARD				// 2006. 3. 30 / ���翵 / ����Ʈ �ӹ� ����, ����, ������ �޴� UI
//#define _ADD_QUEST_LIST_CONTENT			// 2006. 3. 30 / ���翵 / ������ ��û�� ���� ����Ʈ�� ������ ǥ��.
//#define _ADD_QUEST_LIST_MISSION			// 2006. 3. 30 / ���翵 / ������ ��û�� ���� ����Ʈ�� �ӹ��� ǥ��.
//#define _ADD_QUEST_MISSION_LIST			// 2006. 3. 30 / ���翵 / �̼����� ǥ��
//#define _ADD_QUEST_TIME_CHECK			// 2006. 4. 19 / ���翵 / ����Ʈ Ÿ�� üũ
//#define _ADD_QUEST_AREA_CHECK			// 2006. 4. 19 / ���翵 / ����Ʈ ���� üũ
//#define _ADD_QUEST_UNION				// 2006. 4. 19 / ���翵 / ��������Ʈ
//#define _ADD_QUEST_NPC_MARK_IN_RADAR	// 2006. 4. 25 / ���翵 / ����Ʈ�� ���� NPC �� ���̴� ǥ�� ����.
//#define _ADD_QUEST_CANCEL_BUTTON		// 2006. 5. 19 / ���翵 / ����Ʈ ����Ʈ ���� ��ư �߰�.
//
//#define _ADD_QUEST						// 2006. 3. 28 / ���翵 / Quest 
//#define _ADD_OPEN_FORWARD_NPC_MEET		// 2006. 3. 30 / ���翵 / NPC ������ �� ����Ʈ ������ â ����.
//#define _ADD_QUEST_AUTO_ACCEPT			// 2006. 5. 19 / ���翵 / ����Ʈ �ڵ� ����
//#define _ADD_QUEST_AUTO_ACCEPT_REQUEST  // 2006. 8.  3 / ���翵 / ����Ʈ �ڵ� ���� ��û(NPC ����)
//#define _ADD_QUEST_AUTO_REWARD_REQUEST  // 2006. 8.  3 / ���翵 / ����Ʈ �ڵ� �ϷẸ�� ��û(NPC ����)
//#define _ADD_QUEST_CHAO_QUEST			// 2006. 10.30 / ���翵 / ����Ʈ ī���� ī���ð� ���� ����Ʈ �߰�.
//#define _ADD_QUEST_VIEW_PROGRESS_QUEST	// 2007.  1.15 / ���翵 / ����Ʈ ������� ǥ��.

//#define _ADD_CHARGE_ITEM_SAVE_POSITION_SCROLL	// 2006. 11. 20 / ���翵 / ����ȭ ������ ��ġ ���� ��Ż ��ũ�� �۾�.
//#define _ADD_CHARGE_ITEM_EXTEND_INVENTORY		// 2006. 11. 28 / ���翵 / �κ��丮 Ȯ�� �۾�.
//#define PCBANG_ITEM_TOOTIP_PRESENT					// 2006. 12.  9 / ����ȯ / PC�� ������ �ɼ� ���.
//#define _ADD_CHARGE_ITEM
//#define AUTOTARGET_OPTION					// 2006.2.21	/ ����ȯ / �� ���ݽ� ����Ÿ�� �ɼ�ȭ. 
//#define FIXTARGET_OPTION					// 2006.2.21	/ ����ȯ / ��Ű ���������� Ÿ���� �����̼� ���� �ʰ� ����Ÿ��. �ɼ�ȭ.
//#define FOG_RATE_CHANGE						// 2006.2.15	/ ����ȯ /  ���� weight �� ����.    
//#define _JBH_ADD_OPTION_SCROLL_ROTATION_SPEED // 2006. 2.23	/ ����ȯ / ī�޶� ȸ�� �ӵ� ��ũ�ѹ� �ɼ�.
//#define _JBH_ADD_OPTION_SCROLL_ROTATION_SPEED_CALL_FUNCTION
//#define _JBH_ADD_OPTION_SCROLL_MOUSE_SPEED	// 2006. 2.24	/ ����ȯ / ���콺�̵� ����������ũ�ѹ� �ɼ�
//#define _JBH_ADD_OPTION_FORWARD_TARGETING	// 2006. 2.24	/ ����ȯ / ���� Ÿ���� �ɼ�.
//#define _JBH_ADD_OPTION_FORWARD_TARGETING_CALL_FUNCTION
//#define _JBH_MOD_CAMERA_DISTANCE			// 2006.3.16	/ ī�޶�� �ɸ��� �⺻ �Ÿ� ����.

//#define _JBH_MOD_MOUSE_ACTION_CHANGE		// 2006.3.16	/ ���콺 �¿� ��ư ����.	
//#define _JBH_MOD_OPTION_DIALOG				// 2006.3.30	/ �ɼ� ���� ���̾� �α� ����.
//#define _JBH_MOD_CHARACTERSCENE_NOT_MOUSE_CHANGE // 4.13	/ ����ȯ / ĳ���;������� ���콺 �¿캯�� ���� �ʰ� ����.
//#define	_JBH_ADD_YOUNGMAN_PROTECT_MESSAGE	// 2006.4.10	/ ����ȯ / �ε��� û�ҳ� ��ȣ �޼��� ���.
//#define _JBH_MOD_CAMERA_INTERPORATION		// 2006.4.15	/ ����ȯ / ���콺 ȸ���� ī�޶� �����Ǿ� Hero�ڷ� ������ ����.
//#define	_JBH_ADD_NPC_SCALE					// 2006.4.21	/ ����ȯ / NPC �����ϰ� ��ũ��Ʈ���� �о�ͼ� ó��.
//#define _JBH_ADD_FIELD_AREA_INFO_MESSAGE  // 2006.4.22	/ ����ȯ / �ʵ� ����� ���� �̸� ���� �߰�.
//#define _JBH_SPAWN_MONSTER_DIRECTION		// 2006.4.26	/ ����ȯ / ������ Ư�� ���� ���� ����.
//#define _JBH_ADD_TIRED_SYSTEM_DIRECTING		// 2006.3. 9	/ ����ȯ / �Ƿε� �ý��� ����

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ���� �׼� ���� ������
//////////////////////////////////////////////////////////////////////////
//#ifdef LAST_TEST_RELEASE
//	#define _SHOW_HEIM_IN_TESTLIVE_CASH_ITEM_SHOP		// 2006. 12. 14 / ������ / �׼����� ��������۸��� ���Ӹ� ǥ���Ұ�	(==> ���� �׼����� ����ϴ� �������ε�, 0813 ���� ������)
//#endif


//////////////////////////////////////////////////////////////////////////

#if defined ( _DEV_VER )
	// �ּ� ó�� ���� �����ּ���!!
	// �ּ�ó�� ���ص� ��� �ܼ��� �����ʽ��ϴ�.
	// ��� �ܼ� ������ Ŭ���̾�Ʈ ��Ű 
	// '�׽�Ʈ ���̽� ����'�� Ȯ���Ͻñ� �ٶ��ϴ�.
	#define _USE_LUA_SCRIPT		
#endif

//_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
// ��� Ȱ��ȭ
//#define _USE_LUA_SCRIPT

//////////////////////////////////////////////////////////////////////////
//
//			�Ѱ��� ä�θ� �۾�
//
//////////////////////////////////////////////////////////////////////////
#if defined(__KR000000_090128_CHANNELING__)
	// 03/26 - ä�θ� ����
	#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__				// ���ο� �������� ��� <<- ä�θ��� �켱 ���� �� ��ü ���� ����
		// ���� ���� ������ ���
	#else
		#define _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING				// ���� ���� �� ������ ����
	#endif	
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020				// undefine ĳ���ͼ��� â �������� ��ư
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// ���� ����Ʈâ�� �ݱ��ư ����( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING �����ο��� �и�(09.04.28) )
	#define _MOD_IDENTITY_NUM__FOR_CHANNELING						// �ֹι�ȣ �Է����� 6�ڸ��� ����( 7->6 )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// �α��� ��(��������ȭ��)���� �����ϱ� ��ư ����

	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // ä�θ� ���� ���� ���� ������
		#define _APPLY_KOREA_CHANNELING                             // ���� ���α��� �� ����Ÿ�� ����, �������μ��� �����丵
	#endif	
#endif

//////////////////////////////////////////////////////////////////////////
//
//			�۷ι� ä�θ� �۾�
//
//////////////////////////////////////////////////////////////////////////
#if defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// undefine ĳ���ͼ��� â �������� ��ư
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// ���� ����Ʈâ�� �ݱ��ư ����( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING �����ο��� �и�(09.04.28) )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// �α��� ��(��������ȭ��)���� �����ϱ� ��ư ����

	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // ä�θ� ���� ���� ���� ������
		#define _APPLY_KOREA_CHANNELING					            // ���α��� �� ����Ÿ�� ����, �������μ��� �����丵
		#define _APPLY_GLOBAL_CHANNELING							// �۷ι� ä�θ� ���� �÷ο� �۾� -  ���� ä�θ� �۾��� ����Ͽ� �߰� �۾�
	#endif	
#endif


//////////////////////////////////////////////////////////////////////////
//
//			�Ϲ�(NHN_USA)ä�θ����� ACM�ʱ�ȭ�� ������ �۾�
//
//////////////////////////////////////////////////////////////////////////
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// undefine ĳ���ͼ��� â �������� ��ư
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// ���� ����Ʈâ�� �ݱ��ư ����( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING �����ο��� �и�(09.04.28) )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// �α��� ��(��������ȭ��)���� �����ϱ� ��ư ����
	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // ä�θ� ���� ���� ���� ������
        #define _NHN_USA_CHANNELING						
	#endif	
#endif//__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__(�Ϲ�ä�θ�)

//////////////////////////////////////////////////////////////////////////
//
//			�Ϻ� - ������ ä�θ����� �۾�
//
//////////////////////////////////////////////////////////////////////////
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    #undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020                    // undefine ĳ���ͼ��� â �������� ��ư
    #define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// ���� ����Ʈâ�� �ݱ��ư ����( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING �����ο��� �и�(09.04.28) )
    #define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// �α��� ��(��������ȭ��)���� �����ϱ� ��ư ����
    #define _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_                // ���߸��׽�Ʈ�� ���� 2���� ������� ����(������ �����̾����� ���߸����� �ȿ��Ͽ� ������)
    #define _APPLY_JAPAN_GAMECHU_CHANNELING                         // �Ϻ� ������ ä�θ� �۾� ������    
#endif //_JP_0_20100723_GAMECHU_AUTHFLOW_(�Ϻ�������ä�θ�)

//////////////////////////////////////////////////////////////////////////
//
//			���þ� - �ΰ��� ä�θ����� �۾�
//
//////////////////////////////////////////////////////////////////////////
#ifdef _RU_INGAMBA_AUTH_PROCESS
    #undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020                    // undefine ĳ���ͼ��� â �������� ��ư
    #define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// ���� ����Ʈâ�� �ݱ��ư ����( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING �����ο��� �и�(09.04.28) )
    #define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// �α��� ��(��������ȭ��)���� �����ϱ� ��ư ����
    #define _APPLY_RUSSIA_INGAMBA_CHANNELING                        // ���þ� �ΰ��� ä�θ� �۾� ������    
#endif //_RU_INGAMBA_AUTH_PROCESS(���þ��ΰ���ä�θ�)

//////////////////////////////////////////////////////////////////////////
//
// Ŭ���̾�Ʈ ���� ������
//
//////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
	#if !defined( _CREATE_DUMP_ )	// Full Dump
		#define GGUO_CRASH_DUMP     // Mini Dump             
	#endif
#endif


#define AK_FOR_SUN									//	�Ƹ�Ŷ 2.0 ���� �������� ������ �ʿ�

//////////////////////////////////////////////////////////////////////////
// ��� ����
//////////////////////////////////////////////////////////////////////////
/*($Delete$)*/#define __MOD_GUILDMEMBER_REFERENCE				//08.01.15 GuildMain ���� GuildMeber Reference �߸��ϴ� �κ� ����.


//////////////////////////////////////////////////////////////////////////
//
// ���翵 �۾� ������
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �۾� �Ϸ� 
//#define _ZONE_TEST_LOBBY_IN_SINGLE	// �̱� �����󿡼� �׽�Ʈ�� ���� ó��
//#define _ADD_CHARGE_ITEM_RANDOM_OPTION			// 2006. 11. 27 / ���翵 / ���� �ɼ� �߰� ������ �۾�.
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// ����ȯ �۾� ������
//
//////////////////////////////////////////////////////////////////////////

///////////////   [ ��      �� ] ///////////////////////////////////////////////////////////////// 

//#define CAMERA_SMOOTH_DELETE						// 2006.2.21	/ ����ȯ / ī�޶� �������� �̵� ����.
//#define	_JBH_MOD_MOUSE_MBUTTON_IN_BATTLESCENE	//   4.17	/ ����ȯ / ���콺 M��ư Ŭ���� ī�޶� �������� ���ϵ��� ����.

//////////////    [     ETC     ] ////////////////////////////////////////				
#ifndef _CHINA // �߱��� ���� �ڵ��ݱ� ����
	#define _ADD_CHARGE_ITEM_AUTO_GET_HAIM				// 2006. 11. 27 / ����ȯ / ���� �ڵ��ݱ�
#endif


#ifdef _CHINA
	#define _JBH_ADD_ITEM_OVER_PLAYING_TIME_MESSAGE // 2006.3. 7	/ ����ȯ / [�߱���ȣȹ���] �ߵ����� �ý��ۿ����� ������ȹ�� ���� �޼���.							
#endif






//////////////////////////////////////////////////////////////////////////
//
// ������ �۾� ������
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ��Ÿ
//////////////////////////////////////////////////////////////////////////

// �׽�Ʈ �� (�������� ���� ����)////////////////////////////////////////////////////////////////
//#define _JSH_ADD__PLAY_MOVIE_BEFORE_MISSION_		// 2006.02.23 / �̼ǽ����� ������ ��� ( ����� �׽�Ʈ �ܰ� )
//#define VIBRATE_CHARACTER             // 2006. 2. 17 / ������ / �ǰݽ� ĳ���� ����
//#define HAIR_COLOR_VARIATION          // 2006 .2 .17 / ������ / �Ӹ�ī�� ���� ����~
//#define MAPOBJECT_INSTANCING


//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	//#	define _CHECK_MEMORY_OVERRUN					// 2006. 12. 27 / ���翵 / �޸� ������ �ǽɳ��� �κп� �ش� �Լ��� ����.
#endif

#ifdef _CHECK_MEMORY_OVERRUN
	#define CRT_CHECK_INTERRUPT()	if ( !_CrtCheckMemory() ) _asm {int 3};		// �ǽɳ��� �κп� �ش� �Լ��� �յڷ� �ִ´�.
#else
	#define CRT_CHECK_INTERRUPT()	
#endif



//////////////////////////////////////////////////////////////////////////
// �׽�Ʈ ���� �÷���
#define INTERFACE_DIALOG_TEST			FALSE		// Wz Dialog ������ TRUE �̸� Dialog�ϳ��� Render �Ѵ�.
#define ITEM_COMPOSITE_APPLY			TRUE		// ������ ���� 

//////////////////////////////////////////////////////////////////////////
// ��������
#define ATTACK_STAND_TIME				5000


//////////////////////////////////////////////////////////////////////////
//����
#define TUTO_FIELD 20206
#define TUTO_MISSION 31

#define MAX_ITEM_INFO					1000
#define MAX_NPC_INFO					1000
#define MAX_SKILL_INFO					1000
#define MAX_SKILL_INVENTORY_STORE_INFO  1000
#define MAX_STYLE_INFO					1000
#define MAX_MAP_INFO					100
#define MAX_SHOP_INFO					1000
#define MAX_ITEM_COMPOSITE_INFO			100
#define MAX_ITEM_CRYSTALIZE_INFO		50
#define MAX_ITEM_COMPOSITE_NPC_INFO		50
#define MAX_SOCKET_DESC_INFO			50
#define MAX_GRAPHIC_INFO				10000
#define MAX_DIALOG						100
#define	MAX_BGM							100
#define MAX_STYLE_QUICK_REGISTER		100
#define MAX_ITEM_SOUND_INFO				50
#define MAX_NPC_PORTAL_INFO				50
#define MAX_ENCHANT_INFO				100
#define MAX_SKILL_TEXT_INFO				1000
#define MAX_MISSION_MESSAGE_LENGTH		2048
#define MAX_MESSAGE_LENGTH				1024
#define MAX_HELP_INFO					100
#define MAX_QUEST_TEXT_INFO				50000
#define MAX_MISSION_REWARD_INFO			100
#define MAX_ATTACH_ITEM_INFO            1000
#define MAX_EXPVALUE_INFO				100
#define GROUP_ID_NONE					0
#define MAX_MISSION_TEXT_INFO			500

#ifdef	_JAPAN
#define MAX_SSN_COUNT					8
#else
	#ifdef _MOD_IDENTITY_NUM__FOR_CHANNELING
		#define MAX_SSN_COUNT					6
	#else
		#define MAX_SSN_COUNT					7
	#endif
#endif

#define MAX_ICON_IMAGE					50000

#define DISTANCE_NPC_MEET_EXT			0.70f	// NPC�� ���� �� ���� ���, DISTANCE_NPC_MEET �� 70% �Ÿ����� �ٰ����� ���� ��
#define DISTANCE_TAB_TARGET_MONSTER		30.0f	// TAB���� �� �� �ִ� ���� �Ÿ� 30 ����
#define DISTANCE_TRADING                6.0f    // 6 �����̳��������� �ŷ��� �����ϴ�.
#define DISTANCE_FOLLOW_PLAYER			3.0f	// �÷��̾� ĳ���͸� ����ٴ� ���� �Ÿ�

#define NONE_ID							0xFFFFFFFF

// �ð� ����
#define SEC_PER_TICK					1000					// 1�ʴ� ƽ 
#define MIN_PER_TICK					SEC_PER_TICK * 60		// 1�д� ƽ 
#define HOUR_PER_TICK					MIN_PER_TICK * 60		// 1�ð��� ƽ 
#define DAY_PER_TICK					HOUR_PER_TICK * 24		// 1�ϴ� ƽ
#define NO_COOLTIME						//��Ÿ�� üũ ����
#define DELAY_LEAVE_FIELD_TICK			10*SEC_PER_TICK			// 10��
#define DEALY_SPAWN_VILLAGE             5*SEC_PER_TICK			// 10��
// PVP ����
#define MAX_TEAM						2

// font ���� define

// ���� ����
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
   #define MAX_SLOTINFO_LINE				70
#else
   #define MAX_SLOTINFO_LINE				60
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
#define MAX_SLOTINFO_LENGTH				256

#define MAX_TEMP_VARIABLE				20

// ����Ʈ ����
#define MAX_QUEST_AREA_ID				1000

#define HEIGHT_GAP                      0.3f
// �÷� ����


#define DEFAULT_FONT_WZCOLOR			WzColor_RGBA(165,165,165,255)
#define TOOLTIP_FONT_WZCOLOR			DEFAULT_FONT_WZCOLOR
#define TOOLTIP_BG_WZCOLOR				WzColor_RGBA(0,0,0,200)
#define TOOLTIP_SIDE_LINE_WZCOLOR		WzColor_RGBA(119,119,119,255)		// �ܰ���

#define TOOLTIP_OPTION_NAME_WZCOLOR		DEFAULT_FONT_WZCOLOR

#define REVENGE_STONE	7243

#define SSQ_PORTAL		61009
//------------------------------------------------------------------------------
/**
    ���Ӽ�������� �ִ��� ���� (used in GameServerListParser)
*/
const int c_iMAX_GAMESERVERLIST_PARSER  = 50;

using namespace util;

class Quest;

// typedefs 
typedef DWORD							RESOURCECODE;       ///< �׷��� ���ҽ�
typedef WORD							KEYSETCODE;			///< DInput CODE 
typedef util::_tstring					SUN_STRING;			///< 
typedef std::basic_string<TCHAR>::size_type	STR_INDEX;			/// 
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
typedef	WORD QCODE;
typedef std::list<QCODE>				QUEST_LIST;
typedef std::list<QCODE>::iterator		QUEST_LIST_ITER;
#else
typedef std::list<Quest *>				QUEST_LIST;
typedef std::list<Quest*>::iterator		QUEST_LIST_ITER;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

//---------------------------------------------------------------------------------------
//		���̳��� ������ ���̵� �뿪��
//---------------------------------------------------------------------------------------
#define DYNAMIC_WINDOW_ID_MIN 1500
#define DYNAMIC_WINDOW_ID_MAX 1600

#define RANDOM_ITEM_CODE			  65535


#define WzIdToConst(l1,l2,l3,l4) ((l1) * 0x01000000 + (l2) * 0x00010000 + (l3) * 0x00000100 + (l4) * 0x00000001)

#define EFFECT_HIGHT  4000
#define EFFECT_MIDDLE 500
#define EFFECT_LOW    50

#endif 
// GAMEDEFINE_H