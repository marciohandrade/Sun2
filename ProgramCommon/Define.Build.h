#pragma once

//��������������������������������������������������������������������������������������������������������������������������������������������������
// ���� Define ����
//��������������������������������������������������������������������������������������������������������������������������������������������������
// [���̹� ��Ģ]
// 1. ������ ���̹� : NA(�������), KR(�ѱ�), JP(�Ϻ�), US(�̱�), �븸(TW), �߱�(CN), ����(UK)
// 2. �۾� ��û�� ��ȣ : 0(��û������)
// 3. ���� ��¥
// 4. �۾���
// [�ּ� ��Ģ] : �����ڴ� '/'
// 1. �۾� ��û�� ��ȣ
// 2. �۾��ڵ�
// 3. ���� �۾� ���� 
// 4. define�� Ǯ���� ��� �߰����� �۾��� �ؾ� �ϴ� ����

// ex)
// #define __[�����ڵ�]_[�۾���ȣ]_[��¥]_[�۾���]_
// #define __KR_000123_20061020_TOWER_OF_DEVIL		//< [123]	2006. 04. 24	/ ��ο�,���翵 / Į������ ����,���ֹ��� ž �۾� / odbc�� SUNOnline_20455�� �ٲ�� ��
// #define __NA_0_20061020_TOWER_OF_DEVIL			//< [0]		2005. 02. 10	/ ��ο�,���翵 / Į������ ����,���ֹ��� ž �۾� / odbc�� SUNOnline_20455�� �ٲ�� ��

#include "Version.h"


#define __NA_00000_20080410_FOREIGN_DEFINE			// ������ �������� DEFINE ���ΰ���
#ifdef __NA_00000_20080410_FOREIGN_DEFINE
	#include "Foreign.Build.h"
#endif

/*($Delete_[S/C]$)*/#define	__NA_000000_20080515_HACKSHIELD_PATH							// 2008.05.15 / �����, ���ǥ / �ٽ��� ��� �ڵ�ȭ �۾�.
/*($Delete_[S/C]$)*/#define __NA000000_080905_MOLA_VERSION_BUILD_ROUTING__					// 2008.09.05 / ������ / MOLA ���̺귯��, ���� �� ������ ���̺귯�� ��� ���� ����� ��� �߰�
/*($Delete_[S/C]$)*/#define __NA000824_070103_SOLARAUTH_MASTER_NETWORK_MECHANISM_OPTIMIZE__	// 2007.01.03 / ������ / SolarAuth������ Master ������ ��Ʈ��ũ ���� ��Ŀ���� ����ȭ
/*($Delete_[S/C]$)*/#define __NA001098_080711_SERVER_PROCESS_CONTROL_BUG_FIX__				// 2008.07.11 / ������ / [824]����, SolarAuth �� ��Ÿ ���� ��Ʈ�� �̽� ���� ���� ����
/*($Delete_[S/C]$)*/#define _NA_DUPLICATION_CATEGORY_BUG                                    // 
/*($Delete_[S/C]$)*/#define __NA000000_ENABLE_MASTER_SERVER_SCRIPT_LOADING__				// 2008.09.11 / ������ / �����Ϳ����� ��ũ��Ʈ�� ���� �� �ֵ��� ����
// (WARNING) �ϱ� working macro�� �ؿܿ��� �����߻��� ����͸� �ϱ� ���� ������ ������ �� ��.
//#define __NA000000_080709_DBQUERY_FAULT_DETECTION__					// 2008.08.09 / ������ / DBProxy �޸� ���� �߻��� �� �ִ� �κ� ���� (�ӽ� �ڵ嵵 �� ����: ���߿� �����ϰ���)
/*($Delete_[S/C]$)*/#define __NA000630_070817_MOLA_5th_UPDATE				// 2007/08/17 /������,������/���� Ŭ���̾�Ʈ �����۾� (MOLA 5�� ������Ʈ) - LINKED {__NA070212_ENHANCED_SECURE_POLICYMENT__}

/*($Delete_[S/C]$)*/#define __NA_0_20090311_ITEMCOPY_BUG_FOR_LEAVESYN_ERROR					// 2009.03.11 / ���ؼ� / �븸 �����ۺ���, LeaveSyn ó���κп����� ����

//==================================================================================================
//==================================================================================================
//==================================================================================================
// SECURITY MODULE SELECTOR
#if defined(__APPLY_HACKSHIELD)
    // using hackshield module
    #define USING_HACKSHIELD    (1)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (0)
    //
    #if defined(_SERVER) || defined(_SUNGAME_VER)
        #define __NA000000_070913_HACKSHIELD_INSTALLATION__ // 2008.01.23 / lammy / �ٽ��� ����
        #define _AHN_SMART_UPDATE_
    #endif
#elif defined(__APPLY_XIGNCODE)
    //
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (1)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (0)
    //
#elif defined(__APPLY_FROST)
    // using frost module
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (1)
    #define USING_GAMEGUARD     (0)
    //
    #if defined(_SUNGAME_VER) && !defined(_SERVER)
        #define _YMS_USING_FROST_LIBRARY_ // 2010.3.22 / ����� / ���þ��϶��� frost ���
    #endif
#elif defined(__APPLY_GAMEGUARD)
    //
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (1)
        #if defined(_DEV_VER)
            #define NO_GAMEGUARD
        #endif // _DEV_VER
    //
#endif
// Shared value
#define XIGNCODE_GAME_PACKET_SIZE   4000
#define GAMEGUARD_CSAUTH_PACKET_SIZE 4096
#define GAMEGUARD_HACKDATA_PACKET_SIZE 1024
//==================================================================================================
//==================================================================================================
//==================================================================================================

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.6.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1601
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / �ΰ�� / �� �ڽ�Ƭ���� �߰�(�����ڽ�Ƭ)
    #define _NA_008633_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / ȫ���� / ���� �ڽ�Ƭ �߰�
	#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE											// 2016.01.15 / ��ȣ�� / NPC ���� ������ ����
	#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2										// 2016.02.15 / ��ȣ�� / �巡�ﳪ��Ʈ �� ����(�ϵ巡��Ʈ������10����) ��ų ��� �� �̼� �ɷ�ġ ���� �ȵǴ� ���� ����
    #define _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION                        // 2016.02.03 / ȫ���� / ���� �ŷ��� �˸� �ɼ� �߰�
	#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS											// 2016.02.24 / ��ȣ�� / Ÿ�� ��ü�� ���� ��û(��Ÿ���ڵ� �߰�)
    #define _NA_008706_20160229_TEXTLOG_PK                                                        // 2016.02.29 / �ΰ�� / ���ô�� pk�� �ؽ�Ʈ�α�
    #define _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX                                        // 2016.03.08 / ������ / �ڵ��̵����� ��Ż�� �� �� �÷��̾� ��ũ �ȸ´� ���� ����
    #define _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE                                           // 2016.03.15 / ������ / ��Ű�� ���׸������� ����ȹ��� �߰�
    #define _NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL                                        // 2016.03.29 / ������ / ������ �׷��� �������� ��� ����
    #define _NA_000000_20160308_ATTACK_RATE_RISE_BUG                                              // 2016.03.08 / ȫ���� / ���ݼӵ� ��� ���� ����
	#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE								// 2016.03.14 / ��ȣ�� / ���� ������ HP/SD ��ȯ �� ���� �߰� �۾�
    #define _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST                                           // 2016.03.15 / ȫ���� / ��ġ���̵� �뷱�� ���� �۾�.
    #define _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE                                    // 2016.03.21 / ȫ���� / ������Ż����Ʈ ������ �Ϲݰ��� �߻�ü�� �����ϵ��� �۾�.
	#define _NA_008765_20160405_DISCONNECTION_REASON_LOG											// 2016.04.05 / ��ȣ�� / ���ؼ� ������ ��� ������ �α׿� ����� �۾�
	#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / ��ȣ�� / ��ȣ���� �̻����� ����
    #define _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG                                            // 2016.04.18 / ȫ���� / ���潺����� Ǯ���� ���� ����

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.5.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1504
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008334_20150608_SONNENSCHEIN                                  // 2015.06.08 / �ΰ��, ������ / ��Ʈ���� PvE, �ҳٻ���
    #define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                     // 2015.10.01 / �ΰ�� / �����ڱ������ ����, ������ �����˸� ����ʵ��������� �������ϱ�
    #define _NA_008472_20150903_IMPROVED_BALANCE                              // 2015.09.03 / ȫ���� / �뷱�� ����
    #define _NA_008486_20150914_TOTAL_BALANCE                                 // 2015.09.14 / �ΰ�� / �뷱���۾�
    #define _NA_008536_20151023_ELITE4TH_ADD                                  // 2015.10.23 / ȫ���� / ����Ʈ4�� ����/�� �߰� �۾�
    #define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                         // 2015.10.27 / �ΰ�� / ����Ʈ4 �ɼ� �߰� �۾�
    #define _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE               // 2015.10.26 / ȫ���� / Ʈ���� Ÿ�̸� SCENE TYPE�� ���ش�
    #define _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD               // 2015.11.03 / ȫ���� / ����Ʈ �ڵ� ���� ������ ��� �߰�
    #define _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD                 // 2015.10.30 / ȫ���� / ������ ��� ������ �߰��ϴ� �۾�
    #define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / �ΰ�� / ���λ��� �˻� ��, �������� ����
    #define _NA_008571_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / ȫ���� / ���λ��� �˻� ��, �������� ����
    #define _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE                 // 2015.11.25 / ������ / ���콺 Ŭ������ ��ųť���� Ǯ���� ���� ����
    #define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME             // 2015.11.26 / �ΰ�� / �ڽ�Ƭ������ �Ⱓ�������� ����
    #define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                    // 2015.12.02 / �ΰ�� / �̵���ų ���� ��ġ����
    #define _NA_008606_20151222_MODIFY_GUILD_LEVEL                            // 2015.12.22 / �ΰ�� / ����� Ż�� ��, �����ٿ� ���� �ʵ��� ����
    #define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST               // 2016.02.04 / �ΰ�� / �������� ��Ʈ���ȿ���� ���� ������ѵд�

	#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION						  // 2015.10.29 / ��ȣ�� / ���� ��� �̼� ��� ����

    #if defined(_KOREA)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / �ΰ�� / �̼Ӱ��� ����

    #elif defined(_CHINA)
		#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE       // 2015.03.29 / ��ȣ�� / �߱� AC ���� ����Ʈ ������ ���� �ŷ�Ÿ�� �߰�(�ŷ����� + PK��� ����)

    #elif defined(_JAPAN)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / �ΰ�� / �̼Ӱ��� ����
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.5.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1503
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008283_20150511_NOTIFICATIONS_SYSTEM                          // 2015.05.11 / ȫ����, �ӻ�� / �˸� �ý���
    #define _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION                  // 2015.06.17 / ������ / ��õ �ű� ��Ÿ�� ��忡 ���� Ư�� �ִϸ��̼� �۾�
    #define _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL                 // 2015.07.10 / ������ / ������� ui�� �ؽ�Ʈ �� ���� ���� ��ũ��Ʈ���� �о�ͼ� ���
    #define _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY             // 2015.10.15 / ������ / ��Ż�� ��� ��ġ ���� �� ���ѷ��� ���� ����
    #define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                       // 2015.07.03 / �ΰ�� / �ð��� ������� ����� ��, charge sub type 1���� �����ϵ��� ����
    #define _NA_008404_20150712_MINIGAME_SYSTEM                               // 2015.07.12 / �ӻ�� / �̴ϰ��� �ý��� �߰�
    #define _NA_008405_20150716_MINIGAME_BINGO                                // 2015.07.16 / ȫ���� / �̴ϰ��� ����
    #define _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE                // 2015.08.18 / �ӻ�� / ��û��Ҹ�Ÿ�� �߰� 
    #define _NA_008441_20150819_CRITICAL_DAMAGE_CHANGE_FROM_LEVEL_DIFFERENCCE // 2015.08.19 / �ӻ�� / �������̿� ���� ũ��Ƽ�� ������ ���� ��ġ ��ȭ
    #define _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM                  // 2015.08.25 / �ӻ�� / ��� ���� �϶��� ��� ���輳�� ������ ������� ��� �����踦 �����ϵ��� ����
    #define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                        // 2015.09.09 / �ΰ�� / ���Ǹ�������, �˱� Ǯ �������¿����� ��ų��� ����
    #define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT               // 2015.09.09 / �ΰ�� / ���� 300���� ����
    #define _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG                      // 2015.09.22 / ȫ���� / ��Ʋ���� �� �������� ����� ǥ�õ��� �ʴ� ���� ���� �۾�
    

    #if defined(_KOREA)
        #if defined(_SUNGAME_VER)
            #define _NA_008364_20150623_CHANGE_TEXT_PATH                              // 2015.06.23 / ȫ���� / TEXT ��� ����
        #endif // _SUNGAME_VER

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.5.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1502
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////

	#define _NA_008012_20150204_SUN_RANKING_SYSTEM                              // 2015.02.04 / �ӻ�� / Sun Ranking System
	#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM                              // 2015.02.03 / ȫ���� / SUN ��ŷ �ý���
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE                        // 2015.02.03 / ȫ���� / SUN ��ŷ ��� �ý���
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE                       // 2015.02.03 / ȫ���� / SUN ��ŷ �ý��� ����
	#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    #define _NA_008124_20150313_AWAKENING_SYSTEM                                // 2015.03.13 / �ӻ�� / ���� �ý���
    #define _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM                      // 2015.03.11 / ȫ���� / ��� ���� �ý���
    #define _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM                      // 2015.03.18 / ȫ���� / ��� ��ȭ �ý���
    #define _NA_008069_20150210_ADD_SPA_SYSTEM                         // 2015.02.10 / �ΰ��, ������ / ��õ�ý���
    #define _NA_008139_20150318_MODIFY_BUFF_ICON_UI                    // 2015.02.10 / ������ / ����������(Status) UI ����
    #define _NA_008185_20150402_COSTUME_ITEM_FOR_NPC                   // 2015.04.02 / ������ / �ڽ�Ƭ NPC������
    #define _NA_008226_AC_PARENTMAP_LIST_SKIP                          // 2015.04.15 / ������ / AC ������� ����Ʈ ����� ����� �� �����ϱ� ��� �߰�
    #define _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE   // 2015.04.23 / ������ / ����(ĸƾ)���� ũ��Ƽ�� Ÿ�� �ִϸ��̼� ����
    #define _NA_008252_20150427_AURASKILL_PACKET_PENDING               // 2015.04.27 / ������ / ������ų ��Ŷ �ߺ�ó�� ����
    #define _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY  // 2015.04.28 / ������ / �⼮üũ �ý��� ������ ���Ĺ�� ���� (�޷����� �����ϰ�)
    #define _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX            // 2015.05.18 / ������ / ����̵� ����ĵ�� ���� ����
    #define _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE           // 2015.05.19 / ������ / �̵��߿� �ٿ��� �Ȱɸ��� ���� ����(��������ҽð�4�ʿ� ���ؼ� �߻�)
	#define _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST               // 2015.06.01 / ������ / ����Ʈ �Ϸ� ���� �� ���û ��� �߰�
    #define _NA_000000_20150604_BUG_FIND_TEST                          // 2015.06.04 / ������ / ���� ã��� �׽�Ʈ �ڵ� (CTRL+F10 ���� ����Ʈ �׺���̼� ��ġ �ʱ�ȭ)
    #define _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX       // 2015.06.09 / ������ / ȸ�� �̵� �� ������ ���� ����
    #define _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR                  // 2015.06.16 / ������ / ��õ�� �Ӹ� ���� �� �ֵ��� �÷��׷� �����ϵ��� ����
    #define _NA_007863_20141203_VOLUME_LIGHT_SCATTERING                         // 2014.12.03 / ȫ���� / ���� ����Ʈ�� ����� �۵����� �ʴ� ���� ����
    #define _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS          // 2015.03.30 / ȫ���� / �ű� ����Ű ��� ����
    #define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / �ΰ�� / ������AC�� ��� '��������' AC�� ���� �˻��ؼ� ó���ϵ����Ѵ�
    #define _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW             // 2015.03.31 / ȫ���� / SUN ��ŷ �ý��� ������ �����ִ� �۾�
    #define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / �ΰ�� / ������ �̵����� ���� ����
    #define _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE               // 2015.04.22 / ȫ���� / ������ ������ NPC������ �� �� �ֵ��� ����
    #define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / ���Ͱ� �������뿡�� ��ȯ��Ʈ�� ��ã����� �ڷ���Ʈ���ѹ�����
    #define _NA_008256_20150428_MODIFY_CANCLE_STAMP_LOGIC                       // 2015.04.28 / �ӻ�� / ���� ��� ���� ����
    #define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / ��ũ�������Ʈ���� 'Effect'�� ����ϴ� ��ų�� ����ڰ� ������� ���� ��������� �Ѵ�
    #define _NA_008270_20150511_MODIFY_OUT_OF_RANGE                             // 2015.05.11 / �ӻ�� / DominationBattleResult���� OutOfRange ���� ���� ����
    #define _NA_008299_20150519_MAX_DAMAGE_LOG                                  // 2015.05.19 / �ӻ�� / �ִ뵥���� �α� �۾�
    #define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / �ΰ�� / ����ǵ� ȿ���� �������� ���������Ѵ�
    #define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / �ΰ�� / �뷱���۾��� ���� Ŭ�� �������� �����ϱ�
    #define _NA_008333_20150615_BROADCAST_LEFT_RIGHT_TURN                       // 2015.06.15 / �ӻ�� / �¿�ȸ�� ��Ŷ�� ��ε�ĳ��Ʈ �ϵ��� ����
    #define _NA_008333_20150615_FIXED_MAX_LEVEL_EXP                             // 2015.06.15 / �ӻ�� / ���� �޼��� ����ġ ����
    #define _NA_000000_20150714_MOVEMENT_DISTANCE_CHECK                         // 2015.07.14 / ����� / ���ǵ��� üũ. �̵� ���� �Ÿ� üũ
    #define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                            // 2015.09.04 / �ΰ�� / ����̵彺�� ���¿��� BACKSTEP�̵��� ����޸���� ���� �ӵ��� ������
    #define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / �ΰ�� / ĳ�ü��� ������ ����, �������� DB�α� ����ϵ��� �߰�
    #define _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG    // 2015.10.29 / ������ / �ɱ� ��Ŷ SYN�� ACK������ �����̵��� �����̸鼭 ��ġ �ȸ´� ���� ����

#ifdef _DEV_VER
    #define _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO                // 2015.06.04 / ������ / ���߸�忡�� ������ ���ط��� ǥ�����ִ� �۾�
#endif //_DEV_VER

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.5.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1501
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT   // 2015.01.13 / ������ / ���� ��������Ʈ �ִ� ���� ���� ���� (10->15)
    #define _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT     // 2015.01.14 / ������ / �̼Ǻ���â ���� ���,���� ����
    #define _NA_007726_20141015_PROTECTION_SCREENSHOT                 // 2014.10.15 / ������ / ��ũ���� ��ȣȭ(����) ���
    #define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS        // 2014.12.03 / ������ / ����Ʈ�� ���� �� ������Ʈ ���� ���� & ��� ��Ȱ
    #define _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON              // 2015.01.15 / ������ / ���θ޴� ������� ��ư �߰�
    #define _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT       // 2015.02.26 / ������ / �������� �� ���������� ����Ϸ� ��Ŷ�� ��ٸ��� �ʰ� �ٷ� ����
    #define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / �ΰ�� / GM��ɾ�, ����ġ�̺�Ʈ �����ϱ�
    #define _NA_007935_20150109_GM_COMMAND_EXP_EVENT                            // 2015.01.09 / ȫ���� // GM ��ɾ� �߰� ����(GM��ɾ�� ����ġ �̺�Ʈ �����)
    #define _NA_007932_20150106_USER_EXTRA_INFO                                 // 2015.01.06 / �ӻ�� / UserExtraInfo �߰�
    #define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                            // 2015.01.15 / �ΰ�� / ���带 �ռ��ص� ������� �������� �������� ����
    #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC                // 2015.01.27 / �ӻ�� / AgentServer�� packet handler���� ProtocolMatrix�� ���� ��ü�� ������ ����
    #define _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL                  // 2015.01.29 / ȫ���� / ������ ������ ������ ���� �ؽ�Ʈ�� ������ �ʰ� ǥ��
    // #define _NA_008012_20150130_RANKING_SYSTEM                                  // 2015.02.02 / �ӻ�� / ���� ��ŷ �ý��� ������ ����
    #ifdef _NA_008012_20150130_RANKING_SYSTEM
        #define _NA_008016_20150202_OLD_RANKING_SYSTEM                           // 2015.02.02 / ȫ���� / ���� ��ŷ �ý��� ������ ����
    #endif // _NA_008012_20150130_RANKING_SYSTEM
    #define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / �ΰ�� / ����ħ�� �̺�Ʈ�Ŵ����� ����
    #define _NA_008078_20150211_GM_COMMAND_TIME_DATE                            // 2015.02.12 / �ӻ�� / GM��ɾ��߰�, ���� �ð� �� ��¥ ����
    #define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / �ΰ�� / ũ����Ż�緹��,���������� �ð������ ����� �����������ʴ¹���
    #define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / �ΰ�� / �÷��̾�� �ο򿡼� �ѹ濡 �״°�� üũ�ϰ� �α׷� ����Ѵ�
    #define _NA_008150_20150331_MODIFY_EX2_CHARGE_SUB_TYPE_USABLE_ANOTHER_FUNC      // 2015.03.31 / �ӻ�� / eCHARGE_SUB_FIXED_AMOUNT_EX2 Ÿ�� �������� DROP �� LOTTERY�� ���� ���ϴ� ���� ����
    #define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / ���̽��ҵ尰�� ���������� �����ϴ� ��ų�� ����źȯ�� ������ ���� ��꿡 ������ �߻���Ų��
	#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / �ΰ�� / ����Ʈ���װ����۾�, db���ν����� ��Ŷ�� ��ũ����

    #if defined(_KOREA)
        #define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / �ΰ�� / ��Ʈ���ο� �ؽ�Ʈ�α�, �ѹ浥�������� ���ÿ�
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / �ӻ�� / �⼮ �ý��� �߰�

    #elif defined(_CHINA)
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / �ӻ�� / �⼮ �ý��� �߰�

    #elif defined(_JAPAN)
        //#undef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE                   // 2015.10.16 / ������ / �⼮üũ ��ư ui���� �����
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.4.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1404
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// ���� ���� ��ȣ
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007732_20141024_RIDER_MOVESPEED_SYNC     // 2014.10.24 / ������ / �ٸ��÷��̾��� Ż�� �̼� ��ũ ����
    #define _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE // 2014.08.28 / ������ / �ű�Ŭ���� - ��ġ���̵�
    #define _NA_000000_20141222_DOMINATION_RIDER_ACTIVATE // 2014.12.22 / ������ / ���������� Ż �� ���
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007667_20141001_WITCHBLADE_SKILL                // 2014.10.01 / �ΰ�� / ��ġ���̵� ��ų�۾�
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007620_20140915_STATE_RESISTS // 2014.09.15 / ������ / ���� �鿪2
    #define _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN // 2014.09.16 / ȫ���� / �����ο� �ٿ� ó����ĺ���
    #define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / �ΰ�� / ��Ű����ȯ�� �ɷ�ġ ���� ���� ����
    #define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION             // 2014.10.27 / �ΰ�� / ��� �ο��� ��������� �˻縦 �ϴ� ��Ÿ�� �߰�
    #define _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET // 2014.11.12 / �ӻ�� / ��Ŷ ���������� �ڷᱸ�� ����
    #define _NA_007750_20141113_WZDAT_MODIFY_LOG                          // 2014.11.13 / �ӻ�� / WZDAT ���� �α� ����
    #define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / �ΰ�� / ��Ƽ���¿��� ĳ���� ����(HP,MP,SD,FP) ������Ʈ ��� ����
    #define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / �ΰ�� / �굵 �Ҹ�Ÿ�� 11���� �����ֵ����Ѵ�
	#define _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER                       // 2014.12.22 / �ӻ�� / ���� �̵� Ʈ���� �۵������ʴ� ���� ����
    #define _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG // 2015.01.05 / ȫ���� / ��Ż �ؽ�Ʈ�� �̴ϸ� UI ���� ������ ����Ǵ� ���� ����

    #define _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA                          // 2015.06.02 / �ΰ�� / �����۽ø���ѹ��� 40��Ʈ ���� ����Ҽ��ֵ������ش�
    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.4.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1403
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(6)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(0)		// ���� ���� ��ȣ
	#endif
	
	//////////////////////////////////////////////////////////////////////////
    #define _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX // 2014.06.25 / ������, ������ / ACTION_OPERATE_SWITCH_EX Ʈ���� �߰�
    #define _NA_007156_20140703_TOP_OF_TRIAL_LIGHT // 2014.07.03 / ������ / ���ֹ���ž ���ǽ÷�
    #define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // ������,������ / ���͵���
    #define _NA_007477_20140805_RADAR_RAYER_MODIFY // 2014.08.05 // ������ / �̴ϸ��� �� ǥ�� ����(�δ� �ܿ��� ǥ�� ����)
    #define _SCJ_TOGGLE_EDITBOX_CONTROL // 2014.07.22 / ������ / ��� ����Ʈ�ڽ� ��Ʈ���߰�
    #define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / �ΰ�� / ���������� �߰�
    #define _NA_007330_20140620_GUILD_SYSTEM_EXTENSION                      // 2014.06.20 / ȫ����, �ӻ�� / ��� �ý��� Ȯ��
    #ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        #define _NA_004034_20120102_GUILD_INTRODUCTION                          //2012.05.10 / ������,�̵��� / ��� �Ұ� �۾�
    #endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    #define _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP // 2014.07.22 / ������ / �Ⱓ�� ������ ���� �Ǹ�
    #define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / �ΰ�� / ������ᰡ ����� ��� '��æƮ'�� ���õ� ���� ����
    #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / �ӻ�� / ��Ʈ���� ������ �ʱ�ȭ �� ���� ���� ����

    #define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / �ΰ�� / DB���α� �۾�(LogConsole)
    #define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / �ΰ�� / ����ȭ �ٲٱ�
    #ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
        /*($Delete_[x/C]$)*/#define _NA_007450_20140805_CRYSTALIZATION_RENEWAL                   // 2014.08.05 / ȫ���� / ����ȭ ������
    #endif // _NA_007408_20140711_MODIFY_CRYSTALLIZE
    #define _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG // 2014.09.30 / ȫ���� / ĳ�ü� �ڽ�Ƭ �̸����� ��ɿ��� ������ �ָ������� ���Ͽ� ���� ���ñ���� ������ �ʴ´�.
    #define _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM    // 2014.10.14 / �ӻ�� / �Ϲ� ���¿����� PK���ص� ������ ����� �ƴҰ�� �������� ������� �ʴ´�

	#if defined(_KOREA)

    #elif defined(_CHINA)
		#undef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM // #7950 // �߱� ��û���� �ش� ������ ��Ȱ��ȭ
        #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC // 2015.01.27 / �ӻ�� / AgentServer�� packet handler���� ProtocolMatrix�� ���� ��ü�� ������ ����

	#elif defined(_JAPAN)

	#elif defined(_GLOBAL)

	#elif defined(_RUSSIA)

	#endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.4.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1402
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_000000_20140423_SUMMON_BACKGROUND_PICKING             // 2014.04.23 / ������ / ��ȯ�� �ļ� ��ŷ
    #define _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE       // 2014.04.08 / ������ / ������ ��ȯ�� ���� ����
    #define _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION     // 2014.04.08 / ������ / ��ų ��� �� �ڵ����� ��ȯ ���� �ɼ�ȭ
    #define _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL         // 2014.03.24 / ������ / ������ ��ų �뷱�� ������
    #define _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER         // 2014.04.08 / ������ / ������ ���� ������ �߰�  
    #define _NA_007124_20140324_SHADOW_MODIFIED_SKILL_SOULCONTROL_SUMMON_GRADE    //2014.03.24 / �ӻ�� / ������ �ҿ� ��Ʈ�� ���� - ���� �� ĸƾ�� ��ȯ�� �� �ֵ��� ����(Ŭ�� �۾� ������)
    #define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / �ΰ�� / ������ ��ų ������
    #define _NA_000000_20140224_SUMMONER_REFACTORING                // 2014.02.24 / ������ / ��ȯü ������
    #define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.26 /�ӻ�� /(��Ʈ����) domination ��ũ��Ʈ �� �ļ� ����, domination type �߰�, ���� ������ �� ���� �߰�
    #define _NA_007115_20140313_MONDSCHIEN_RENEWAL                 // 2014.03.13 / ȫ���� / ��Ʈ���� ������
    #define _NA_000000_20140331_MAPLOADING_MEMORYPOOL              // 2014.03.31 / ������, ȫ���� / �ʷε��� MemoryPool�� �ٲٴ� �۾�
    #define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / �ΰ�� / �������� ���������� ���������ĺ���
    #define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / �ΰ�� / �ű� ������ ��巯��
    #define _NA_007114_20140313_CHAOSZONE_GOLDRUSH                      // 2014.03.13 / ȫ���� / ��巯��
    /*($Delete_[S/x]$)*/#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / �ΰ�� / ��Ī�ý��� �����丵
    #define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / ������,������ / ������NPC
    #define _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL  //2014.04.02 /�ӻ�� /���忹���� ���� ä�ο����� �����ϵ��� ����
    #define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / �ΰ�� / GM��ɾ� �߰�/����
    #define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / �ΰ�� / ���ºο� Ʈ���Ű� �ʿ�����Ʈ���� �ٿ������� �۵��Ѵ�
    #define _NA_007294_20140613_DRAGONKNIGHT_RENEWAL                    // 2014.06.13 / �ӻ�� / �巡�� ����Ʈ ������
    #define _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW      // 2014.06.13 / ȫ���� / ĳ���� ���� �� ��Ʈ���� ���ֱ��� ���͵��� ���� ��� �� �� �ֵ��� ����

    #define _NA_007743_20141027_ADD_ISVAILD_CHECK                        // 2014.10.27 / �ӻ�� / ��ȣ �ڵ� �߰�
    #define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / �ΰ�� / ��Ŷ������ ���� AC���� ���ѹޱ⹮�� ó��

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY        // 2014.10.27 / �ӻ�� / MiniDumpWithFullMemory�� ����ϵ��� ����


    #elif defined(_JAPAN)
        #define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / �ΰ�� / ��巯�ÿ��� �������Ʈ ���������� DB�α׿� �����
        #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / �ӻ�� / ��Ʈ���� ������ �ʱ�ȭ �� ���� ���� ����
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.4.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1401
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_007019_20140107_PERK_CHANGE_SYSTEM // 2014.01.07 / ȫ���� / Ŀ���� �нú� �׷����� �����ϴ� �۾�
    #define _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX      // 2014.02.10 / ������ / ���� �ٸ� ����Ʈ â�� ������ ���� ����
    #define _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM // 2014.01.20 / ������ / ������ ��Ȱ ��� ���� (������ �˾�â)
    #define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / �ΰ�� / ��� ��, �� �������� 0�̸� ����Ȯ���� ��� �Ҹ��ϴ� �г�Ƽ ����
    #define _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG // 2014.02.21 / ȫ���� / ĳ���� ����ȭ�鿡�� EnterŰ�� ������ ������ ����Ű�� ���� ��� ���õ� ĳ���Ͱ� �ٸ� ĳ���ͷ� �����̵Ǹ�, "�ʹ����� �ٸ��ϴ�" ���� �޽��� ���� Ŭ���̾�Ʈ�� ����Ǵ� ���� ����
    /*($Delete_[x/C]$)*/#define _NA_007080_20140227_DEACTIVATION_APPEARANCE_UPDATE_AND_ATTACKRANGE_UPDATE_BUG // 2014.02.27 / ȫ���� / ��� ��Ȱ��ȭ �Ǿ��� ��(������ 0�̳� ������ �ȸ��� ��)������ ����� ���� �ʴ� ����, ��ġ�����ε��� ���Ÿ� ������ ������ ����
    /*($Delete_[S/x]$)*/#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / �ΰ�� / ������0�̳� ���Ⱥ�ȭ�� ���� ��Ȱ��ȭ ��, ���⸦ ���������� ���·� �����.
    #define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / �ΰ�� / �����������Ʈ�� '�Ϸ�'�ߴµ� ������ �������� ���¿��� �����׷�����Ʈ�� �޴� ���� ����
    #define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / �ΰ�� / ������0�� ������ ��������
    #define _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST // 2014.03.14 / ȫ���� / �κ��丮�� ������ �� �������� ���� ����

    #define _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND // 2014.01.14 / ������ / ī������ ���� Ȱ��ȭ/��Ȱ��ȭ �߰�

    #define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / �ΰ�� / DB�α� ������α��
    #define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / �ΰ�� / ���������� ���� ��� ��, ItemNum�� �κ��丮 �� ������ �ƴ�, ���ռ����� ������ �Ѵ�.

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / �ΰ�� / ģ���ʴ� ������ �������޽�, �������� const.h�� ������
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.3.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1304
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20130812_NEW_CHARACTER_HELLROID  // 2013.08.12 / ����� / �ű�Ŭ���� - ����̵�
    #define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / �ΰ�� / ��ŷ�ý��� ������ �� ���巩ŷ �߰�(���̳�)
    #define _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR // 2013.08.19 / ȫ���� / ��ŷ �ý��� ���� Minor
    #define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL //2013.10.30 / �ΰ�� / ����̵� ��ų���� ũ��Ƽ�� �����Ƽ ���� �߰� �� ����
    #define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / �ΰ�� / ���Ͱ� ��������� ���� '��Ȳ'���°� �ƴ϶� '����'���·� ��ȯ�ϵ��� �Ѵ�.
    #define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / �ΰ�� / thrustState(�˹����)���� �񼱰� ���Ͱ� �ݰ��� ���� �ʴ� ���� ó��
    #define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / �ΰ�� / ������ ������, �޸��Ҵ翡�� ������ �״� ������, ������ƮǮ�� �޸��Ҵ��� �ٲ㺻��.
    #define _NA_007031_20140116_CAN_SELL_CASH_ITEM_IN_STORE             //2014.01.16 / �ΰ�� / �������� ĳ�þ����� �Ǹ� �����ϵ��� ����
    #define _NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY         //2014.01.16 / �ΰ�� / �ǵ�����ڿ� ���� �ִ� �Ⱓ�� ������ ���� ����
    //#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / �ΰ�� / �ؽ�Ʈ�αװ� �Ϸ�������� �ð������� ������ �����ϵ��� ����

    #if defined(_KOREA)
        #define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / �ΰ�� / �㿡 KISS�� ���δ�. ���� �� �����ڼ��� �����Ѵ�.
    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.3.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1303
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM // 2013.07.04 / ȫ���� / ��ŷ �ý��� ����
    #define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM      //2013.07.05 / �ΰ�� / ��ŷ�ý��� ������ �� ���巩ŷ �߰�

    #define _NA_000000_20130828_GUILDMARK_SIZE_MODIFY      // 2013.08.28 / ������ / ��帶ũ ũ�� ����
    #define _NA_006826_20130722_DOMINATION_RENEWAL         // 2013.07.22 / ������, ������ / ���Ǽ��� ������ ����
    #define _NA_007239_20140521_BUG_GUILDRELATION_COUNT    // 2014.05.21 / �ΰ�� / ��������� �ִ�ġ ����
    #if defined(_NA_006826_20130722_DOMINATION_RENEWAL) && !defined(_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL)
        #define _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL // �ӽ÷� �������� �ӽ���Ŷ ���� ���ø���Ʈ�� ���������� ������
    #endif //
    
    #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / ȫ���� / Ʃ�丮�� �ʵ忡���� ��Ʋ���� ���� �� �� �ֵ��� ����
    /*($Delete_[x/C]$)*/#define _NA_006886_20130910_BUG_POINT_ACCUMULATE_IMAGE // 2013.09.10 / ȫ���� / ���� ����Ʈ ���� �ޱ� �� ����Ʈ 2�� ��ᰡ �������� �޾����� ��� �ش� �������� �������� 1�ʸ��� �� Ŭ������ ��� ���������� �ٲ�� ��µǴ� ���� ���� �۾�    
    #define _NA_006928_20131022_HELMET_UI_BUG // 2013.10.22 / ȫ���� / ĳ���Ͱ� ó�� �������� �� ��� ���̱� �Ⱥ��̱� UI�� ���� �ʴ� ���� ����
    #define _NA_006930_20131023_ACCUMULATE_PAUSE_AND_CLOSE_ADD_FUNC // 2013.10.23 / ȫ���� / ��������Ʈ ���� �޴� ���� ESCŰ�� ���� ������Ű�� dialog�� �ݴ� ��� �߰�
    #define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / �ΰ�� / �̼ǳ����� ������ ���� �� �������ϸ�, �̼� �� ����� ��ġ�� ��ġ�ȴ�.

    #if defined(_KOREA)
		#define _NA_000000_20130903_CHANNEL_GAGE_INCREASE      // 2013.09.13 / ������ / ä�� ȥ�� �������� ���ƺ��̰� ����
    #elif defined(_CHINA)
        //#define _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION   // 2013.12.17 / ����� / ���ӿ� ����ȿ��(��������Ʈ, ����Ʈ����Ʈ)�� �׻� OFF�� �ǵ��� ����
        #define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / �ΰ�� / �߱� ��ö��� ���� ���� �αױ��
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.3.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1302
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////
    
 
    #define _NA_006541_RENEWER_ACDIALOG // 2013.04.9 / ���ö / ACâ ������
    #define _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER // 2013.03.22 / ���ö / ������ ����â ������
    #define _NA_006643_20130417_UI_OUT_WINDOW_BREAK // 2013.04.17 / ���ö / UI������ UIâ �̵� ����
    #define _NA_006589_20120415_ENCHANT_PRICE_MODIFY // 2013.04.15 / ������ / ��þƮ ��� ���� ����
    #define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO // 2013.04.15 / ������, ������ / Ÿ��â ������ ���� ǥ�� �߰�
    #define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY // 2013.04.16 / ������ / Ŀ�´�Ƽâ �������� ������ �߸� ǥ�õǴ� ����
    #define _NA_000000_20130417_SELFDISTRUCTIONABILITY_MODIFY // 2013.04.17. / ������ / SelfDistructionAbility ��� ���� ���� ����    
    #define _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP //2013.04.22 / ������, ������ / Ʈ���� �߰� - ���̴���(�̴ϸ�) ����
    #define _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT       //2013.05.31 / ������ / ä�� ���� ��, �����ڵ忡 ���� ä���� ��Ȱ��ȭ ��Ŵ
    #define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / ȫ����,�ΰ�� / ac���� �߰�(���Ǹ� ������)    
    #define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / ȫ����,�ΰ�� / ���忡�� �� óġ ��, �߰� ����
    #define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / ȫ����,�ΰ�� / '�̼ǿϷ�', '����¸�' ����ƮŸ�� �߰�
    #define _NA_000000_20130422_SCALE_FULLMAP                       //2013.04.22 / ���ö / �������� �����ϸ� �۾�.                            
    #define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY // 2013.04.22 / ������ / �̼� Ʈ���� �߰� �� �����۾�
    #define _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE        //2013.04.22 / ���ö / ���ӿɼǸ���Ʈ ��Ʈ���ڵ� ó�� �۾�.
    #define _NA_000000_20130426_DUMPTOSCREENSHOT                    //2013.04.26 / ���ö / �������۽� ��ũ���� ÷��
    #define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY // 2013.04.26 / ������ / ��Ű�� ��ȯ�� ��ų ��ũ ����
    #define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / ������ / �����ǵ� MP�Ҹ� ����
    #define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY // 2013.04.26 / ������ / ������ �ɼ� �߰� ����
    #define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / �ΰ��,ȫ���� / ���忡 '��ý�Ʈ' �߰�
    #define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / �ΰ�� / ���� ������ �켱���� ����� ����
    /*($Delete_[x/C]$)*/#define _NA_006687_20130502_SETTING_FIGHTINGENERGY_INIT_BUG // 2013.05.02 / ȫ���� / ����Ŀ�� �˱������� �ٸ� ĳ���Ϳ����� ���� ���������� ��Ÿ���� ���� ����
    //#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / ������ / ���������� �й� �г�Ƽ ����
    #define _NA_006679_20130423_1302_SKILL_RENEWAL                          // 2013.04.23 / ����� / 1302 ��ų�뷱��
#ifdef _DEV_VER
    #define _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL    // 2013.04.23 / ����� / 1302 ��ų�뷱�� - ���� �ִϸ��̼� ����
#endif 
    #define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / �ΰ�� / ���������� �ɷ����� ���,�������� ���� ��� ����
    #define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER // �̼ǳ� ä�� ��� Ʈ���� �߰�
    #define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / �ΰ�� / �����ƼŸ���߰�(345), ��ų���ݷ�, ���ȿ���� ��������ʴ� HP����
    #define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.06 / ������ / ���� ������ �ʵ� ���� ���� �Ұ�
    #define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.08 / ������ / ���� ������ ���� ���� �Ұ�
    /*($Delete_[x/C]$)*/#define _NA_006708_20130509_BUG_ROOM_STRING_LIMIT // 2013.05.09 / ȫ���� / [��Ʋ��&ī������] ���� ������ �� �������� �ѱ� 15�� ����/���� 30�ڷ����� ���� �۾�
    #define _NA_006710_20130509_CRYSTALWARP_BUG // 2013.05.09 / ������ / �̹� �ı��� ũ����Ż�� �������� ���� ��� ������ ���� ũ����Ż�� �����ϴ� ����
    #define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME // 2013.05.02 / ���ö, ������ / ���� �ɼ� �߰�(�ڽ�Ƭ ���̱� �ɼ�)
    #define _NA_000000_20130521_NOT_SUMMON_SKILL // 2013.05.21 / ���ö / ���ս�ųUI�ڵ� ����
    #define _NA_006728_20130520_SKILL_BALANCE_MODIFY // 2013.05.20 / ������ / ��ų �뷱�� ����( �ݻ���� ��ų ���� )
    #define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / ������ / ��������Ʈ �ʱ�ȭ ������
    #define _NA_000000_20130529_STATUS_EXCEPTIONS_AS_NPC_GRADE // 2013.05.29. / ������ / NPC��޿� ���� �����̻� Ȯ�� ���� ���� �׸�
    #define _NA_006749_20130604_NOT_LIMIT_DARKBREAK // 2013.06.04 / ȫ���� / ��ũ �극��ũ ��ų�� ��� ���� ������ ���� �ʴ� �۾�
    #define _NA_006731_20130521_ENCHANT_ADD_OPTION // 2013.05.21 / ������, ������ / �ֻ��(13~15)��þƮ �ɼ� �߰�
    #define _NA_000000_20130612_RANDOMIZER_CHANGE // 2013.06.12. / ������ / ��æƮ ���� �Լ� ����
    /*($Delete_[x/C]$)*/#define _NA_006772_20130614_BUG_ENERGY_SUCTION_CONTINUE_ANIMATION // 2013.06.14 / ȫ���� / ������ Ŭ���� Ŀ�� �迭 3���� "������ ����" ��ų�� ����ϴ� ĳ���Ͱ� ����� ���¿��� ��ų �ִϸ��̼��� ���ӵǴ� ���� ���� �۾�
    #define _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES // 2013.06.17 / ���ö, ȫ���� / ������� �ʴ� ���ҽ� ���� �۾�
    /*($Delete_[x/C]$)*/#define _NA_006781_20130627_BUG_SHOW_OTHERPLAYERNAME_OF_GAMEOPTION // 2013.06.27 / ȫ���� / �ɼ� ���� �ǿ��� "�ٸ� �÷��̾� �̸�" ����� Off�ϸ� �� ĳ������ �̸��� ��µ��� �ʴ� ���� ���� �۾�
    #define _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2 // 2013.07.10. / ������ / Ȯ��������2 - ȹ��ú��� ���� �Ⱓ ���
    #define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM // 2013.07.03 / ������, ������ / ���� ��ǥ �ý��� �߰�
    #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / ����� / ���¾�����(����) ǥ���ϴ� ����� ����
    #define _NA_000000_20130726_RENEWAL_POINT_REWARD // 2013.07.26 / ���ö / ����â ������

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / ����� / ���¾�����(����) ǥ���ϴ� ����� ����
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / ������ / ��ƼŬ���̾�Ʈ
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / ����� / ��ƼŬ���̾�Ʈ 3�������� ����ǰԲ�
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _RU_006860_20130806_COSTUME_PCBANG_EFFECT  //2013.08.06 / �ΰ�� / �ڽ�Ƭ�� �ǽù� �ɼ� �����Ű��
        //#define _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP // 2013.12.23 / ȫ���� / ������ ����� �� ���尡���ϵ��� �ϴ� �۾�
    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.3.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1301
/////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_006363_20130103_NOT_CALC_ENCHANT_STAT // 2013.01.03 / ������ / ��æƮ�� �����ϴ� ���� �䱸ġ �������� �ʱ�
    #define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / ������ / ���� ������
	#define _NA_000000_20130114_RENEWER_UI // 2013.01.14 / ���ö / ���� UI ������ 
    #if defined(_NA_000000_20130114_RENEWER_UI)
        //#define _NA_000000_20130218_OPTION_TOOLTIP      // 2013.02.18 / ����� / ������� �ɼ�â���� ����ǥ���ϱ�
    #endif 
    #define _NA_000000_20130206_BROADCAST_ITEM_SCORE    // 2013.02.06 / ����� / �ٸ��÷��̾��� ��������� �׳� �����ֵ��� ����
    #define _NA_006413_20130201_PREMIUMSERVICE_ITEM // 2013.02.01 / ������ / PC�� ȿ�� ������
    #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / ������ / ��Ű���� ��ȯ���� ��ȯ �Ͽ� ���͸� ����, ��ȯ���� ĳ���� ��ó�� �̵� �� �� ���� ĳ���͸� �ڷ� �̵���ų ��� ���Ͱ� �ٺ� �Ǵ� ����
    #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / ������ / ���Ǽ������� ����ũ����Ż�� ��ȯ �� ĳ������ �� �Ǳ��� ��ȯ�� ������ �׾ ũ����Ż�� ��ȯ�Ǵ� ����
    #define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG // 2013.03.19 / ������ / �巡�ﳪ��Ʈ�� �������¿��� �뺯�� �ȵǴ� ���� ����
    #define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY // 2013.03.25 / ������ / ���� �߰��۾� #2 <���� ���� ��Ȳ �˸�>
    /*($Delete_[x/C]$)*/#define _NA_006539_20130319_ITEM_COMPOSIT_CERTAIN // 2013.03.19 / ȫ���� /  ���� ������ǥ�ø� 86%~100�� �ſ� �������� 100% �� �� 'Ȯ��'�� �ߵ��� �и� ����
    #define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG // 2013.03.28 / ������ / ��ȯ��(��Ű��)�� ���� �� ��ų��Ÿ�� ���� ���°� ��ȯ������ ������ �ȵǴ� ����
    #define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY // 2013.04.01 / ������ / �̼� �� ���忡�� ��ȯ���� ������� �����͸� �������â�� �ݿ� (������ ���� ������, ��ȯ���� ų��, ����)
    #define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO // 2013.04.01 / ������ / ��Ű�� ��ȯ���� �� ��ų�� ��ȯ��(����)�� ��ų���ݷ� ������ �°� �߰� ������ ������ ��������
    #define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL // 2013.04.09 / ������ / ���� ��ö ����� ��� ���� ������ ��ų�� �����Ǵ� ���� ����.
    #define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME // 2013.04.02 / ������, ������ / GM��ɾ�� �̺�Ʈ ���� ����
    /*($Delete_[x/C]$)*/#define _NA_006639_20130415_DEATH_AND_NOT_RELIEVE_RIDER // 2013.04.15 / ȫ���� / Ż���� ź ���¿��� ��� �� Ż�� ��ȯ�� ���Ⱑ �������� �ʴ� ���� ���� ����
    /*($Delete_[x/C]$)*/#define _NA_006640_20130415_VIEW_INCREASE_HEAL // 2013.04.15 / ȫ���� / ��Ű�� ��ȯ�� "������ ����"�� �� ��ų�� ��ȯ���� ��ų ���ݷ� �߰� ������ ������� �ʴ� ���� ����
    #define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / �ΰ�� / �̵�+������ ��ų�� 1���� �����Ը� ���ظ� ��
    #define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / �ΰ�� / �ַκδ������ Ŀ�ÿ�������� �Բ� ���� �� �ֵ��� ��
    /*($Delete_[x/C]$)*/#define _NA_006647_20130419_TOOLTIP_VIEW_GENDER_COLOR // 2013.04.19 / ȫ���� / ���� ��Ű�� ĳ���Ͱ� "���� ������Ż����Ʈ�� ȣ�ǹ��� �ǻ�"�� ���콺 ������ ��� ���� Ŭ���� ��� �κ��� ������� ��µǴ� ����
	#define _NA_006644_20130419_BUG_QUEST_KILLMONSTER               //2013.04.19 / �ΰ�� / ����ų ����Ʈ �ߺ����� ����
    #define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS // 2013.04.22 / ������ / �뺯�� ���¿��� ũ����Ż�Ӹ��� ������ Ȱ��ȭ�� �����ϰ� �ִ� ��� ������� ���� ����
    #define _NA_006655_20130422_BUG_ACCESSORY_SETOPTION           //2013.04.22 / �ΰ�� / �����+���� ��Ʈ�����ۿɼ� ��ø����

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / ȫ���� / Ʃ�丮�� �ʵ忡���� ��Ʋ���� ���� �� �� �ֵ��� ����
        //#define _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL // 2013.05.27 / ȫ���� / �̺�Ʈ �Ⱓ �� ���ҵ� ������ ���� ���� ����Ʈ ���� ǥ�ø� �ּ�~�ִ� ���� ǥ�÷� ����

    #elif defined(_GLOBAL)
     
    #elif defined(_RUSSIA)

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.2.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1204
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20120920_STARTMOTION_CODE // 2012.09.20 / ����� / ����Ŀ �峪 CE08, ������ CE00 ���� ����ϰ� �ִ� ĳ���� ���۸���� CE00���� ����
	#define _NA_000000_20120911_FULL_MISSION_HELPER // 2012.10.08 / ���ö / �̼� ���� ��� 1204�� ������ �ø�.
    #define _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY // 2012.10.31 / ������ / �巡�ﳪ��Ʈ ���Ǹ������� ��ų�� �ߵ������� ������ ��ũ�극��ũ�� �����ϰ� ����
    #define _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT // 2012.10.31 / ������ / uiEventWindow �� �̺�Ʈ ���� ���� �ƴ� �� ��縦 ����
    #define _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT // 2012.11.13 / ������ / FTEXT ��� �� ���� �߸��� ���� �׸���
    #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / ������ / ���׸�źȯ ��å ����(�⺻���ݷ¸� -> ��ü������)
    #define _NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE // 2012.10.15 / ������ / �ǰݽ� ���̵���� ���� ����
    #define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD // 2012.11.16 / ������ / ť��ý��� ���ؽ� ���󸮽�Ʈ���� ĳ����� ����
    #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / ������ / Ÿ���� ������� ĳ���� �˷����� �ʱ�
    #define _NA_000000_20121210_REFACTORY_GAME_USER // 2012.12.10 / ���ö / �ű����� ������ ���� ���� ���� �۾�.
    #define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2012.12.11 / ������ / ���� ��������Ʈ ���� ��� �ð� ����
    #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / ������ / ��ȯ���� ������ ��ö����� ��� ����,�ǰ�,��ų��� ���� -> �Ұ������� ����
    #define _JP_20110222_INGAME_CHARNAME_CHANGE
 
    #if defined(_KOREA)
        #define _JP_20110222_INGAME_CHARNAME_CHANGE

    #elif defined(_CHINA)
        #define _CN_CHANGE_ICON     // 2013.05.30 / ����� / �߱� �ۺ��� ���濡 ���� ������ ����
        #define _CN_000000_20130605_MATRIX_CARD_REMOVE // 2013.06.05 / ������ / �߱� ��Ʈ���� ī�� ����
        #define _CN_000000_20130605_XIGNCODE_UPDATE // 2013.06.05 / ������ / �߱� �����ڵ� ������Ʈ
        #define _CH_006797_20130703_PK_DROP_RULE_MODIFY // 2013.07.03 / ������ / �߱� �ʵ� PK ��å ����
        #define _CN_000000_20130729_NOT_USE_CUBE // 2013.07.29 / ���ö / ť�� ��� ����.
        #define _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK // // 2013.07.29 / ������ / �߱� ť�� �ռ� or ���� �����丮 ����
        //#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG // 2013.07.31 / ������ / �߱� ��ö��� ���� ���� �αױ��
        #define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / �ΰ�� / �߱� PK���� �����, ������迡���� �������� ����ȴ�.
        #define _NA_000000_20131031_GAMELOG_REDUCE // 2013.10.31 / ������ / ���ӷα� ���̱�
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / ������ / ��ȯ���� ������ ��ö����� ��� ����,�ǰ�,��ų��� ���� -> �Ұ������� ����
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / ������ / ��Ű���� ��ȯ���� ��ȯ �Ͽ� ���͸� ����, ��ȯ���� ĳ���� ��ó�� �̵� �� �� ���� ĳ���͸� �ڷ� �̵���ų ��� ���Ͱ� �ٺ� �Ǵ� ����
    #endif
#endif 

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.2.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1203
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_004157_20120409_QUEST_BANDWITH_4000 // 2012.04.00 / ������ / ����Ʈ �뿪�� Ȯ��
    #define _NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE // 2012.08.07 / ������ / �ǰݽ� ���û��� ���� �Ұ�

    #define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / ������, ������ / �ű� ĳ���� �̽�ƽ �߰�
    #ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / ������ / �ű� ĳ���� Ŭ���̾�Ʈ ����(��ũ��Ʈ�ļ�����)
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / ������ / �ű� Ŭ���� Ÿ�� �߰��� ���� Ÿ��MAX ������ ���� �߻��Ǵ� ���� �ӽ� ����
    #endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC

    #define _NA_000000_20120527_CHAOS_ZONE // 2012.05.27 / ������,������ / ī������ �ý���.
    #ifdef _NA_000000_20120527_CHAOS_ZONE
        #define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND // 2012.06.27 / ������, ������ / ī���� �� ���� �ý���.
        #define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP // 2012.09.10 / ������, ������ / ī���� �� �������� �ý���.
        #define _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION // 2012.09.14 / ������, ������ / ī���� �� ���� �ý��� Ȱ��ȭ
    #endif //_NA_000000_20120527_CHAOS_ZONE

    /*($Delete_[S/x]$)*/ //#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP // 2012.09.04 / ������ / ������ �ٴڹ����� �Ұ���, PK��Ӱ��� Ÿ�� �߰�.
    #define _NA_006057_20120904_PK_DROP_RATIO_MODIFY // 2012.09.04 / ������ / �ڽ��� ���� ���� ���� �÷��̾ �׿��� ��� ���� �÷��̾��� �����۵���� ���� ���Ѽ� 100%->50%

	#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / ����� / �ڽ�Ƭ������ ������ �����ۺ� ��⵿�� ����
    #define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / ������,�̵��� / ���λ��� �˻� �ý���    
    #define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY // 2012.09.26 / ������ / ��Ʋ�� ���忹�� �켱���� ����. (���� -> �ֽż�)
    #define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY // 2012.10.12 / ������ / PVP�뿡�� ��Ƽ����â�� ������ġ�� �߸� ǥ�� �Ǵ� ���� ����
    #define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD // 2012.10.15 / ������ / ��Ʋ�������� �ʵ弭���� �̵��� ����� �ʵ嵵 �����ϵ��� ����
    #define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY //2012.10.17 / ������ / ���� �� ����迭 ��ų ��ũ�� ���� ����
    #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / ������ / ������������ �����ϴ� ���� ���ݺҰ�����(����, �ٿ�..)�� �Ǿ ��ų�� ��� �����Ǵ� ����

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / ������ / ���׸�źȯ ��å ����(�⺻���ݷ¸� -> ��ü������)
        #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / ������ / Ÿ���� ������� ĳ���� �˷����� �ʱ�

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / ����� / ��ƼŬ���̾�Ʈ 3�������� ����ǰԲ�

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / ������ / ��ȯ���� ������ ��ö����� ��� ����,�ǰ�,��ų��� ���� -> �Ұ������� ����
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / ������ / ��Ű���� ��ȯ���� ��ȯ �Ͽ� ���͸� ����, ��ȯ���� ĳ���� ��ó�� �̵� �� �� ���� ĳ���͸� �ڷ� �̵���ų ��� ���Ͱ� �ٺ� �Ǵ� ����
        #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / ������ / ���Ǽ������� ����ũ����Ż�� ��ȯ �� ĳ������ �� �Ǳ��� ��ȯ�� ������ �׾ ũ����Ż�� ��ȯ�Ǵ� ����
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.2.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1202
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / ������,������ / ��� ������
    #ifdef _NA_003923_20120130_GUILD_RENEWAL
        #define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / ������,������ / ���â�� �� ����â�� ���԰��� Ȯ��
        #define _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER // 2012.02.17 / ������ / uiGuildMan ���ۼ�
        #define _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL // 2012.02.22 / ������ / ������ UI ������ 
        #define _NA_004509_20120314_QUESTDROPINDEX10 // 2012.03.12 / ������ / ����Ʈ ��� �ʵ� Ȯ��.
        #define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / ������,������ / ��� ���� �۾�
        //#define _NA_004034_20120102_GUILD_INTRODUCTION //2012.05.10 / ������,�̵��� / ��� �Ұ� �۾�
    #endif //_NA_003923_20120130_GUILD_RENEWAL

    //#define _NA_20120220_JUMP_RENEWAL // 2012.02.20 / ����� / ���� ������
    #define _DH_NPC_GROUP_NAME  // 2012.3.26 / �̵��� / NPC �� ���� Group ���� ǥ��(�Ӹ����� �߰� �̸�)
    #define _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE // 2012.4.02 / �̵��� / ���������� �̹��� �����۾�
    //#define _NA_004157_20120409_QUEST_BANDWITH_INCREASING // 2012.04.00 / ������ / ����Ʈ �뿪�� Ȯ��
	#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                    // 2012.04.18 / ������ / ��Ʋ�� ���ȣ�� Ŀ�´�Ƽ ȭ�鿡 ǥ��.
    #define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE    // 2012.04.18 / ������ / ��Ʋ�� �� ���� �� ��й��� ���忩�� �޼��� ������ ����.
    #define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST      // 2012.04.18 / ������ / ���� ����� ����� ��Ʋ�� �� ����Ʈ�� �����Ѵ�.
    #define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / ������, ����� / 1202 ��ų ������
    #define _NA_0_20120321_SKILLTREE_SECOND_RENEWAL // 2012.03.15 / ������, ����� / 1202 ��ų ������ (��ųƮ��, ��, �׼�)
    #define _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT    // 2012.04.26 / ��ų ������ �޺� ���� �߰�
    #define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / ������, ����� / ��ų ���ݿ� ���׸� źȯ ����
    #define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING // 2012.04.26 / �����, �̵��� / ���� ���� ���� �ɼ� �۾�
    #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / ������ / ��Ű�� ��ȯ���� Ÿ���� ���� �����ϴ� ����, �̹� Ȱ��ȭ�� Ŀ�ǵ� �� ����.

    #define _NA_004756_120504_SHOW_SSQ_PLAYER_NAME              // 2012.05.04 / ������ / Ÿ���� ������� �÷��̾� �̸� ���̱� GM��ɾ� �߰�
    #define _DH_LAST_SELECTED_CHARACTER /// [2012-5-11 by i4u4me] ĳ���� ����ȭ�鿡�� �ֱ� �÷����� ĳ���� ����
    #define _NA_004644_20110506_QUEST_NAVIGATION // 2012.05.16 / �̵��� / ����Ʈ �׺���̼� ���� ���� ��ȭ

    #define _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL   // 2012.06.18 / ������ / �̵� �� ��ų ť�� ���� (Ȱ��ȭ�� ������ ����)
    #define _NA_004859_20120518_FORCE_INPUT_SCREENSHOT_KEY    // 2012.05.18 / ����� / ä���� ��ũ����Ű �Է°����ϵ��� ����
    #define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE    // 2012.05.11 / ������ / ŸŹ�� ��� �߾� �� ���� ���Ƽ ���� ���� �� ���� �� ��� ���� �� PVP����.
    #define _NA_004910_20120601_SUMMON_PVP_RULE                     // 2012.06.01 / ������ / ��Ű�� ��ȯ�� SD(99):HP(1) �� ����.
    /*($Delete_[x/C]$)*/#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / ������, ������ / ������ ���� �ɼ� ���� ��� ����
    #define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST  // 2012.05.11 / ������, ������, ������ / ���� ������ �߰� ��û ����(������巹������ �� ���ݱ�弱��)
    #define _NA_004035_20120227_COSTUME_ITEM                        // 2012.05.23 / ������, ����� / �ڽ�Ƭ ������
    #define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET   //2012.06.14 / ������ / ��ȿ��<����� ���� ����������>
    #define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY            // 2012.06.21 / ������, ������ / ��� ������ ���Ѽ� ���� 99% -> 80%
    #define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE       // 2012.06.22 / ������ / ��Ű�� ��ȯ�� PVP���� ���ΰ� ���� �� ����.
    #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR            // 2011.10.27 / ����� / ���̴��� ä�������� ������Ʈ�� ������ǥ��
	#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / ������ / ������� �Ϸ� ��� �ִ� ���� ��ũ��Ʈ �����ͷ� ����

    #if defined(_KOREA)
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / ������ / ��ƼŬ���̾�Ʈ
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / ����� / ��ƼŬ���̾�Ʈ 3�������� ����ǰԲ�
    #elif defined(_CHINA)
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / ������ / ������� �Ϸ� ��� �ִ� ���� ��ũ��Ʈ �����ͷ� ����
        #define _CN_006014_20120822_GUILD_LOG_DELETE        // 2012.08.22 / ������ / �߱� ���α׿� ������ǥ �������� �α� ����.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#ifndef _NA_COSTUMEITEM_IDLE_ANIMATION	// �۷ι��� 1202, �������� 1203����
			#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / ����� / �ڽ�Ƭ������ ������ �����ۺ� ��⵿�� ����
		#endif

    #elif defined(_RUSSIA)
        #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / ������ / ������������ �����ϴ� ���� ���ݺҰ�����(����, �ٿ�..)�� �Ǿ ��ų�� ��� �����Ǵ� ����
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / ������ / ������� �Ϸ� ��� �ִ� ���� ��ũ��Ʈ �����ͷ� ����
    #endif

#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.2.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1201
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(0)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////



    #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / ����� / ��Ƽ�� �ο����� ���� ���� ����ġ ����.
    #define _NA004034_20120102_POINT_WALLET_SYSTEM // 2012.01.02 / ������,������ / ȭ�� ���� �۾�
    #define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / �����,������ / �巡�� ����Ʈ�� �뺯�� 1,2 ���� ��ų ���� ��� �߰�.
    #define _NA_003649_20111115_BATTLE_ZONE_HARDMODE    // 2011.11.15 / ������ / �̵��� / ��Ʋ��.�ϵ��� �߰�
    #define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / ������, ����� / ���� ������ ����
    #define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / ������, ����� / �ֻ�� ��þƮ
    #define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / ������ / SD ���� ���� ����
    #define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / ������ / ��Ű�� ���ݷ� ���� ����
    #define _SCJ_111202_UI_RENEWAL // 2011.12.02 / ������ / UI ������ �۾� (����Ʈ,ac,ĳ����â)
    #define _SCJ_120104_SORT_QUEST_VIEW_PROGRESS // 2012.01.04 / ������ / ����Ʈ �̸����� �����ϱ�
    #define _NA_20111118_CASHSHOP_UI_RENEWAL // 2011.11.18 / ������ / ĳ�ü� UI ������ (�����۾�x)
    #define _DH_KEY_BINDING // 2011.11.29. �̵���, ������ / ���� ����Ű ���� ����
    #define _NA_20111213_SKILLTREE_VISUAL_RENEWAL2 //2011.12.13 / ����� / ��ų�������� ���� ������
	#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN // 2010.01.06 / ������ / ���� ��ǥ ��ȯ���� �ʵ��� ����
    #define _DH_CHANGE_CHAT_HISTORY // 2012.01.09 �̵��� / ä�� �����丮 Ȱ��ȭ Ű ����
    #define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / ������ / ���Ӽ��� ObjectFactory ����
    //#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO // 2011.06.10 / ������ / ���� ������ ü�� ���� ini���� ����(ServerOptionEX.ini -> GameOption.ini)
    #define _DH_MONEY_COLOR_CHANGE // 2012.02.08 / �̵��� / ���� ������ ���� ����
    #define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / ������ / ObjectKey �뿪�� Ȯ��
    #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / ������ / ������ / ��Ʋ��.��Ƽ����������
    #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
    //#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / ������ / ���� �ɼ� Ʈ����� ó�� ��� �߰�
    #define _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR // 2012.03.14 / �̵��� / ���λ��� �������� �� ���� ����
    #define _DH_SHOW_VENDOR_LIST_POPUP // 2012.03.22 / �̵��� / ���λ��� ����Ʈ���� ��Ŭ������ ĳ���� �˾��޴� ��� ����

    #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT   // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)
    #define _DH_DEBUG_REPORT   // 2012.02.14 / �̵��� / ũ���� ����Ʈ ����
    #define _NA_0_20120321_CASHSHOP_POPUP_EFFECT            // 2012.03.21 / ����� / ĳ�ü� ����Ȯ��â �������úκ� ���̶���Ʈ ǥ��

    #define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE    /// [2012-4-24]  / ������ , ����� / ä�� ��Ŷ�� ����Ʈ �ڵ� �߰�
    #define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    // [2012-4-24]  ����� / �׾����� �ɷ�ġ �ö��� �ʰ� ��

    #define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] �߱� �޸���  
    #define _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER /// [2013-10-22] �����, ������ / ������� �̵��ӵ�, ���ݼӵ��� Ŭ�󿡼� ������� �ʰ� �������� �����ִ� ���� �̿��ϵ��� ����


    #if defined(_KOREA)
        #define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / ����� / ��ȣȭ ��� MD5 -> SHA256 ����.
        #define _NA_20120313_CASHSHOP_DISABLE_ALLITEM   // 2012.03.13 / ����� / ĳ�ü� '��������' ī�װ� ǥ������ �ʵ��� ����
        #define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / ������ / ���ֹ��� ž ���� ����� ����


    #elif defined(_CHINA)
        //#define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] �߱� �޸���  
        #define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / ������ / ���� �ɼ� Ʈ����� ó�� ��� �߰�

    #elif defined(_JAPAN)
        #undef _DH_CHANGE_CHAT_HISTORY // ä�� �����丮 Ȱ��ȭ Ű ���� - �Ϻ������� ���� ����
		#define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / ������ / ���ֹ��� ž ���� ����� ����

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.1.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1103
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(9)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET // 2011.11.10 / ������ / ���뺯�� ���� ������ �ڵ���� �ɼ� ����
    #define _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC // 2011.11.04 / ������ / FTEXT Ÿ�� �߰� - ����Ʈ����/�Ϸ� (NPC�̸� ����/���� �߰�)
    #define _SCJ_ACSYSTEM_OVERLAP_PARENT // 2011.10.05 / ������ / AC���� ����� ��������� ��ġ��
    #define _NA_0_20111013_RADAR_PATHFINDING // 2011.10.13 // ����� / NPC ��ã�� ����߰�
	#define _NA_003033_20110831_DICECAST_ITEM_ROUTING	// 2011.08.31 / ������, ������ / �ֻ��� ȹ�� ��� �߰�
    #define _YMS_TREECONTROL   //! 2011.6.13 / i4u4me /  Ʈ����Ʈ�� / WZENGINEVER >= 400
    #define _NA_003027_20111013_HONOR_SYSTEM								// ���ý��� (������, ������)
    #define _NA_002935_20110704_ITEM_SCORE // 2011.07.04 / ������, ������ / ��� �� ���� ǥ��
    #define /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL// 2011.06.30 / ������, �̵��� / ��Ʋ�� ������

    #ifdef /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL
        #define _DH_BATTLEZONE2_// 2011.06.30 / �̵��� / ��Ʋ�� ������(Ŭ���)
        #define _DH_BATTLEZONE2_LIST_PAGE //2011.06.30 / �̵��� / ��Ʋ�� ������: ��Ʋ�� ����Ʈ ������ ������� ����
        #define _DH_BATTLEZONE2_LOBBY_SLOT//2011.06.30 / �̵��� / ��Ʋ�� ������: �κ񽽷� ���� ����(�������� �ƴϰ�)
        #define _NA_LEAVE_PARTY_AT_USE_ITEM_TO_MISSION//2011.11.24 / ������ / �̼� ������ ���� ��Ƽ Ż�� ���
    #endif//

    /*($Delete_[x/C]$)*/#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT// 2010.01.16 / ������, �̵��� / ������ �溮 ������ �߰�
    #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM// 2011.06.13/ �̵���, ������ / ����������
    #define _NA002635_GZ_DZ_PROTOCOL_UINIFICATION // ������ / Guild ���� ���� �� DG �������ݰ� ���� �۾�
    #define _NA002823_WINGS_EQUIPMENTS_EXTENTION // ������, ������ / ���� �������� Ư��+�� �������� Ȯ��
    //#define _NA000000_SPEED_HACK_PREVENTION_SUPPORT // ������, ����� / SpeedHack�̽� ���� ó�� = { Ŭ���̾�Ʈ �ð� ��ü �˻� + ���� �̵� ��û ���� + �÷��̾� �̵� �Ӽ� ���Ἲ �˻� }
    #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // �����, ������ / ���� ��Ÿ�� ���� ����
    #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // ������, ����� / �̼� ��Ż ���� ��ȭ (Ÿ�̸� �� ��Ż ���� �� ���� Ż�� ��� ����)
    #define _NA_002884_AC_DIALOG_RENEWAL // 2011.09.08 / ������ / AC â ������ �����׸� 10->20�� ���
    #define _SCJ_EXP_EVENT_ENABLE_EFFECT // 2011.09.23 / ������ / ����ġ �̺�Ʈ �˸��� ���
    /*($Delete_[x/C]$)*/#define _NA_003457_20111013_EXCEPT_DOMINATION_REWARD // 2011.10.13 / ������ / ���Ǽ���,Ÿ���ѻ�� ����ȿ�� ����
    #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / �����, ���ӷα� �뷮�� ���� ���Ϻи�.
    #define _NA_003798_SPREAD_WING_KEY // 2011.12.13 / ������ / ���� ���(������) Ű �߰�(Shift+Space)
    #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS// 2011.12.20 / ������, ������ / ��Ʋ�� ������ ������ ���� ���� ���� ó��

    #if defined(_KOREA)
        #define _NA_0_20110810_LOG_COLLECTOR_RESOTRE //������ //�α� �ݷ��� �ڵ� ���� ���
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / �����, �����/ ĳ�ü� �����丵.

        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND // 2011.10.12 // ����� / ������ �Ⱓ���� ����߰�
        #endif
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / Ʃ�丮�� ��ŵ
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)

    #elif defined(_CHINA)
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / �����, �����/ ĳ�ü� �����丵.
        #define _NA_20111118_CASHSHOP_UI_RENEWAL                        // 2011.11.18 / ����� ĳ�ü� UI ������

        #ifndef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      
            #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      // 2011.10.27 / ����� / ���̴��� ä�������� ������Ʈ�� ������ǥ��
        #endif

        #if !defined(_NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT)
            #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / ����� / ��Ƽ�� �ο����� ���� ���� ����ġ ����.
        #endif

    #elif defined(_JAPAN)
        #define _DH_DEBUG_REPORT   // 2012.02.14 / �̵��� / ũ���� ����Ʈ ����
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT               // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)
        #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / ������ / ������ / ��Ʋ��.��Ƽ����������
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                    // 2011.06.13 / �����, �����/ ĳ�ü� �����丵.
        #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS       // 2011.12.20 / ������, ������ / ��Ʋ�� ������ ������ ���� ���� ���� ó��
        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND             // 2011.10.12 // ����� / ������ �Ⱓ���� ����߰�
        #endif
        //#define _YJW_TUTORIAL_SKIP      //! 2011.8.9 / i4u4me / Ʃ�丮�� ��ŵ 

    #elif defined(_GLOBAL)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL

    #elif defined(_RUSSIA)
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.        
        #define _SCJ_MAKE_DOUBLE_QUOTATION_MARKS // 2011.08.22 / ������ / >0    <���ƾ� ����ǥ�޽��� ó��~!!
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / ������ / ��Ű�� ��ȯ���� Ÿ���� ���� �����ϴ� ����, �̹� Ȱ��ȭ�� Ŀ�ǵ� �� ����.
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / �����, �����/ ĳ�ü� �����丵.
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / ������ / ��Ű�� ��ȯ���� Ÿ���� ���� �����ϴ� ����, �̹� Ȱ��ȭ�� Ŀ�ǵ� �� ����.
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.1.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1102
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

	#define _NA_001990_ACSYSTEM_ADD										// 2011.02.14 / ������, ������ / AC ���� ���� �߰�
    #define _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX                      // AC ��ũ��Ʈ ���� ������ ���ְ�, ĳ���Ͱ� �����Ҽ� �ִ� ������ ����
    #define _NA001955_110210_WING_COSTUME_ACCESSORY_EXTENTION // 2010.02.10 / �����, ������ / �Ĵ��Ĵ� ���� ������ ���� �۾�, Ư�Ǽ��縮 �������� Ȯ�����

    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_BLADEFURY_SONICBLADE_EFFECT_RENEWAL    // 2011.03.04 // ����� / �巡�� ��ų 2�� ������ (���̵� ǻ��, �Ҵ� ���̵�, �ű� ����Ʈ ���ҽ��� �����Ͱ� �ʿ���)
    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_WEAPON_THROWING_PROJECTILE_RENEWAL     // 2011.04.08 // ���������� ����Ʈ ������
    #define _NA_0_20110308_LOGSYSTEM_RENEWAL // 2010.03.08 // ����� / �α� �ý��� ������
    #define _NA000000_100427_SSQ_DEFAULT_REWARD_ // ������, ����� / Ÿ���� ��� ������ ���� �⺻ ���� ó�� (��ȣ:108)
    #define _NA001955_110210_WING_COSTUME // 2010.02.10 / �����, ������ / �Ĵ��Ĵ� ���� ������ ���� �۾�
    #if defined(_NA001955_110210_WING_COSTUME)
        /*($Delete_[x/C]$)*/#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / ����Ʈ Ǯ�� ��Ʈ ��ȯ ���Ŀ��� �и�
    #endif 
    #define _NA001956_110210_PERK_SWITCHABLE_REGION // 2010.02.10 / ������, ������ / Ư���� ���� (�� ���� ��ü ����)
    /*($Delete_[S]$)*/#define _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM // 2010.02.10 / ������ / ��� �˻� �˰��� ����
    #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / �����, ����� / ĳ���� �����ӽ� ��ų ��Ÿ�� ���� ��� �߰�.
    #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER // 2011.03.07 / ������, ������ / NPC ���� Ʈ���� �߰�
    
    #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE // 2010.03.11 / ������ / NPC Ʈ���� ���� �̺�Ʈ ��� ó�� �κ�
    #endif
    #define _NA_2463_20100328_OPTIMIZATION_RADAR_MAP_TEXTURE // 2011.04.22 / ������ / �̴ϸ� �ؽ��� ���ҽ� ����ȭ �۾�
    /*($Delete_[S/x]$)*/#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN // 2011.04.06 / ������, ������ / ������ ���� ��� ����
    #define _NA_1937_20100207_NPC_CUSTOMIZING // 2010.03.28 / ������ / NPCĿ���͸���¡ ����
    #define _NA_2088_20100317_MODIFY_QUEST_AND_AC_LIST_ORDER // 2010.03.28 / ������ / ����Ʈ/AC ����Ʈ ���� �� ��� ����
    /*($Delete_[S/x]$)*/#define _NA_002050_20110216_ADD_GENDER // 2011.02.16 / ������, ������ / ĳ���� ���� �߰�
    /*($Delete_[S/x]$)*/#ifdef _NA_002050_20110216_ADD_GENDER
        /*($Delete_[S/x]$)*///#define _USE_MAX_CHAR_LIST_PARSER // ĳ���� �ִ� ��� ���� �ļ� ��� ����
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_CHARACTER_SOUND_INFO // 2011.05.24 / ������ / ĳ���ͻ��彺ũ��Ʈ�ļ� ����(�����ʵ� �߰�)
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_LOAD_CHARACTER_RESOURCE_IN_CHARACTER_SCENE // 2011.06.24 / ������ / ĳ���;����� ĳ���ͻ��� �� ���ҽ��ε����� ���� ���� ���� ����
        /*($Delete_[S/x]$)*/#define _YMS_GENDER_FULLSETITEM //! 2011.5.24 / i4u4me / Ǯ�� ��ũ��Ʈ ���� 
    /*($Delete_[S/x]$)*/#endif

    #define _SCJ_THREE_LEVELUP_PER_EFFECT   // 2011.04.01 / ������ / 3���� ���� ������ ����Ʈ �߰�/����
    #define _SCJ_ADD_TYPE_LAYER_FOR_ITEM    // 2011.04.11 / ������ / ������ Ÿ�Կ� ���� ���̾� �߰�
    #define _SCJ_TEXT_COLOR_RENEWAL         // 2011.03.07 / ������ / ä�� �ؽ�Ʈ �۾��� �׵θ� �Ӽ� �ֱ�
    #define _YMS_AUTOBOT_ITEM_KIND          // 2011.3.23 / i4u4me / �ڵ� ��� ������ �������� �߰� 

    #define _NA_20100322_AGGRO_DISPLAY                                    // 2011.03.24 ��׷� ��ġ ȭ�鿡 ǥ�� (������, �̵���)
    /*($Delete_[x/S]$)*/#define _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT // 2011.04.21 / ������, �̵��� / ��� ���� �����۾�
    #define _GS_GSP_ALLOW_MOVE_TARGETING_								// �Ϲ� ����������ų �̵�.�����ÿ� ����

    #define _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE                        // 2011.03.23 / ������,������ / ������ ȹ�� ��� ����
    
    #define _YMS_WINDOW_MAXSCREEN   //! 2011.4.7 / i4u4me / â��� �ִ�ȭ ��� �߰�
    #define _NA_002455_20110328_AUTH_HEART_BEAT // 2011.03.28 / ������ / �������� Heartbeat ��� �߰�
    #define _NA002635_GP_DP_PROTOCOL_UINIFICATION // ������ / Portal ���� ���� �� DG �������ݰ� ���� �۾�
    #define __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT // 2011.05.09 / ������ / (�߱�) Ʃ�丮�� ���� ���� ��ȭ ���� �۾�

    #define _DH_QUEST_ACCEPT_MESSAGE // 2011.05.19 / �̵��� / ����Ʈ ������ ��ܿ� �޽��� ��� �ϴ� �۾�

    #define _SCJ_SSQ_DIRECT_ENTER  // 2011.05.20 / ������ / Ÿ���ѻ�� ����� ��� �� �ٷ� ��Ż ����
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED // 2011.05.23 / ������ / OptionList.txt�� RatioValue �ʵ带 Suffix�� ���� �۾� (���̻�� ����� ��Ʈ���ڵ带 ����)

    #define _SCJ_CHANGE_SKILL_COOLTIME_COLOR // 2011.06.07 / ������ / ��ų ��Ÿ�� ���� ����

    #define _NA_0_20110329_GAMEOPTION_SETTING                       // 2011.03.29 / �����, �����/ ���� �ɼ��� DB ����

    #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
        #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / ������ / ToTalGuide�� �߰��Ǹ鼭 ������ ����ϴ� Ű ���� â�� ������ 
    #endif
    #define _NA_002950_20110627_SSQ_NOTICE // 2011.06.27 / ������, ������ / Ÿ���� ��� ��� �޽��� ��� ��� ����

    #define _SCJ_ADD_SNS_SHORTCUT                                     // 2011.06.22 / ������ / SNS �ٷΰ��� ��ư �߰�
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO         // 2011.06.22 / ������ / OptionList.txt�� ValueType2 �ʵ尡 �߰��ǰ� ���� 1�� �ɼ��� ������ �̷ο� �ɼ�
    #define _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE // 2011.06.30 / ������ / ��Ż �̵��� ��ȯ�� ������� �ʵ��� ����

    #define _YMS_CHANNEL_LIST_COUNT5    //! 2011.7.12 / i4u4me / ä�θ���Ʈ ���� 4 -> 5��

    #if defined(_KOREA)
        #define _NA_20100307_BETAKEY_SYSTEM                                   //��ŸŰ �ý��� (������, �̵���)
        /*($Delete_[C]$)*/#define REMOVE_SUMMON_FRIEND    // 2011.05.24 / �̵��� / ģ����ȯ ��ư ����
        #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // �����, ������ / ���� ��Ÿ�� ���� ����

    #elif defined(_CHINA)
        #define _YJW_CREATECHARACTER_MOVIE //! 2011.8.3 / i4u4me / ĳ���� ������ ������ ����
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / Ʃ�丮�� ��ŵ 

        #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // ������, ����� / �̼� ��Ż ���� ��ȭ (Ÿ�̸� �� ��Ż ���� �� ���� Ż�� ��� ����)
        #ifndef _NA_0_20110803_DIV_GAMELOG_FILE
            #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / �����, ���ӷα� �뷮�� ���� ���Ϻи�.
        #endif
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
        #define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / �����, �����, ���� Ŭ���̾�Ʈ�� MAC �ּ� �α� �����.
        #define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / �����, ��Ƽ �ο����� ���� ���� ����� ����.
        #define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / �����,����� / ������ 0�� ���⵵ ���� ���� ������� �ı��ǵ��� ��� ����.

    #elif defined(_JAPAN)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_003481_20111026_EXP_PARTYBONUS_CHANGE // ��Ƽ �ο����� ���� ����ġ ������ ���� - arycoat 2011.10.26
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
    #elif defined(_GLOBAL)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / ������, �̵��� / ���� ������ ���� ����� ���̵� �ý���
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / ������ / ToTalGuide�� �߰��Ǹ鼭 ������ ����ϴ� Ű ���� â�� ������ 
        #endif
        /*($Delete_[S/C]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND // 2009.10.21 / ������, �̵��� / ģ�� �ʴ� �ý���
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
    #elif defined(_RUSSIA)
        #define _DH_KEY_BINDING // 2011.11.29. �̵���, ������ / ���� ����Ű ���� ����
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / ������, �̵��� / ���� ������ ���� ����� ���̵� �ý���
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _YMS_TREECONTROL // 2011.07.12. ����� / Ʈ����Ʈ��
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / ������ / ToTalGuide�� �߰��Ǹ鼭 ������ ����ϴ� Ű ���� â�� ������ 
        #endif
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.1.0.1 Episode2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1101
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _GS_ADD_VOLUME_LIGHT                                        // 2010.09.28 / �豤�� / ����Ʈ ����Ʈ �ɼ� �߰�
    /*($Delete_[S/C]$)*/#define _DH_EP2_FULL_MAP                                            // 2010.12.13 / �̵��� / �Ŵ�� ���� + �Ź�Ʋ��
    /*($Delete_[S/C]$)*/#define _DH_IMAGE_TOOLTIP                                           // 2010.11.03 / �̵��� / �̹��� ����
    /*($Delete_[S/C]$)*/#define _NA_000251_20100727_SOCKET_SYSTEM                           // 2010.07.27 / ������, �̵��� / ���� �ý���
    /*($Delete_[S/C]$)*/#define _NA_002253_20100811_CUBE_SYSTEM                             // 2010.08.11 / ������, �̵��� / ť�� �ý���
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_PERK_SYSTEM_                           // 2010.10.19 / ������, ������ / �� �ý���
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_SKILL_SYSTEM_DB_                       // 2010.10.19 / ������ / ��ų �ý��� DB ��Ʈ
    /*($Delete_[x/C]$)*/#define _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_                  // ������ / ��ų �ý��� ���� ���� �� �� �ý��� �ݿ�
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_SKILLTREE_SYSTEM                             // 2010.09.01 / �����, �豤��/ ��ųƮ�� �ý��� �߰�
    /*($Delete_[S/C]$)*/#define _GS_SKLL_TREE_SYSTEM                                        // 2010.09.01 / �豤�� / ��ųƮ���ý���
    /*($Delete_[S/C]$)*/#define _NA_0_20100819_AGGRO_RENEWAL                                // 2010.08.19 / ������ �̵��� / ��׷� �ý��� ������    
    /*($Delete_[S/C]$)*/#define _NA_000816_20101025_AIRSHIP_SYSTEM                          // 2010.10.25 / �豤��, ������ / ����� �ý���
    /*($Delete_[S/C]$)*/#define C_NA_0_20100520_ACSYSTEM_CHANGED                            // 2010.05.20 / ������, ������ / AC �ý����� ���������� �����Ѵ�.
    /*($Delete_[x/C]$)*/#define _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT             // 2010.11.23 / ������ / PortalServer ��ŷ ��� �ð� ����
    /*($Delete_[x/C]$)*/#define _NA_1388_20101207_EP2_INCREASED_MISSION_MAX                 // 2010.12.07 / ������ / �̼� �ִ� ���� ����
    /*($Delete_[S/C]$)*/#define _TEMP_DEFINE_REMOVE_CHARACTER_SCENE_MONSTER                 // 2010.12.13 / ������ / ĳ���ͻ����� ���ͻ��� �ӽ� ��(*�ӽõ�����)
    /*($Delete_[S/C]$)*/#define  _LTJ_CENTIPEDE_MONSTER_BURROW_ANMATION                     // 2101.01.06 / ������ / ���׸��� ���ο�ִϸ��̼� ó��
    /*($Delete_[S/C]$)*/#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                  // 2010.12.14 / ������, ������ / ������ ���� ��� ������ ���� ��� ������ üũ ��� ����
    /*($Delete_[S/C]$)*/#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
    /*($Delete_[x/C]$)*/#define _NA000000_GM_COMMAND_CHANGE_CONCEPT_                        // ������ / GM ��ɾ� �и� �۾� �� ó�� ��� ��ü
    /*($Delete_[S/C]$)*/#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL                     // 2011.01.31 / �����,����� / ��ųƮ�� �ý����� ��ų�� ���� ���� ��� ����.(��ų �⺻ ���� 0��ų -> 1��ų).
    /*($Delete_[S/C]$)*/#define _NA_0_20110215_SKILLTREE_SOUND_RENEWAL                      // 2011.02.15 / ����� / ��ųƮ�� ���� ȿ���� ����
    /*($Delete_[S/C]$)*/#define _DH_TARGET_INFO_RENEWAL                                     // 2011.02.16 / �̵��� / ���� �������� ������EP2 �۾�
    /*($Delete_[S/C]$)*/#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                            // 2011.03.03 / ������, �̵��� / Ÿ�Ժ� NPC ���� ��� �߰�

    /*($Delete_[S/C]$)*/#define _NA_0_20110216_STATUS_STAT_POINT_SIMULATION                 // 2011.02.16 / �����,����� / ���� ����Ʈ �ùķ��̼� �߰�.
    /*($Delete_[S/C]$)*/#if defined(_NA_0_20110216_STATUS_STAT_POINT_SIMULATION)
        /*($Delete_[S/C]$)*/#define _YMS_STAT_SIMULATION_   //! 2011.2.16 / i4u4me / ���� �ùķ��̼� 
    /*($Delete_[S/C]$)*/#endif

    //
    /*($Delete_[S/C]$)*/#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_PARSER // 2010.07.27 / ������ / ���� �ý���(�ļ�)
        /*($Delete_[S/C]$)*/#define _NA002237_100802_EP2_SOCKET_SYSTEM_DB_ // ������ / ���� �ý��� DB ��Ʈ
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_CONVERT // 2010.07.27 / ������ / ���� �ý���(solarSlot)
        /*($Delete_[S/C]$)*/#define _NA_001699_20110104_SOCKET_OPTION_POINT // 2011.01.04 / ������, �̵��� / ���� �ɼ� ����Ʈ ������� ����        
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_RANKUP // 2010.10.13 / �̵��� / ���� �ý���(��ũ�� ����) Ŭ������
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_OLD_SOCKET // 2010.10.16 / �̵��� / ���� �ý���(���� ���� ����) Ŭ������
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_SOCKET_COMPOSITION_ANIMATION// 2011.02.23 / �̵��� / ���� �ִϸ��̼� ����ȭ
    /*($Delete_[S/C]$)*/#endif //
    /*($Delete_[S/C]$)*/#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (2)
    /*($Delete_[S/C]$)*/#else
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (1)
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA001605_EP2_ATTRIBUTES_RENEWAL_ // ������, ����� / �Ӽ� �ý��� ������
    /*($Delete_[S/C]$)*/#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        /*($Delete_[S/C]$)*/#define _YMS_EP2_ATTRIBUTES_RENEWAL_     //! 2011.1.7 / i4u4me / ĳ���� �Ӽ� ������(ui â)
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[S/C]$)*/#define _YMS_EP2_SKILL_PACKET_RENEWAL_ //! 2011.1.10 / i4u4me / ��ų ��Ŷ ����
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_DAMAGETYPE_EXTENTION_ // ������, ����� / DAMAGETYPE Ȯ��
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ // ������, ����� / skill broadcaster �̺�Ʈ ������� ����

    /*($Delete_[S/C]$)*/#define _NA_0_20101130_REWARD_ITEM_COUNT_INCREASE                   // 2010.11.30 / �����, ������ / ���� ���� ����. ���� 4�� -> 5��
    /*($Delete_[S/C]$)*/#define _NA_0_20101206_PARTY_ITEM_DISTRIBUTE_CHANGE                 // 2010.12.06 / �����, �̵��� / ��Ʋ�������� ��Ƽ ������ �й��� ����(�ʵ�� �����ϰ�)

    /*($Delete_[S/C]$)*/#define _NA_0_20110103_EP2_BATTLEZONE_EXP_PARTY_BONUS_ADJUSTED      // 2011.01.03 / ������ / ��Ʋ��(��) ����ġ ��Ƽ ���ʽ� ���� ���� (�߱� ����)
    /*($Delete_[S/C]$)*/#define _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING            // 2011.01.14 / ������, ������ / ����, ������ ������ ������ �� �ִ� �ý��� �޸� ����/���� �α� �߰�
    /*($Delete_[S/C]$)*/#define _NA_20100802_SPEED_HACK_DETECTION                           // 2010.08.02 / ������, ����� / ������ In-Game ���ǵ��� Ž��

    /*($Delete_[S/C]$)*/#define _YMS_DIALOG_ANIMATION_                                      // 2010.12.13 / i4u4me / ���̾�α� �����̰�  DialogPosInfo.txt  �� ���� �����Ǿ�� ��
    /*($Delete_[S/C]$)*/#define _YMS_TEXTURE_ANIMATION_                                     // 2010.12.27 / i4u4me / �ؽ��� �ִϸ��̼�  TextureListInfo.txt, TextureAnimationInfo.txt ���� �����Ǿ�� ��
    /*($Delete_[S/C]$)*/#if defined(_YMS_DIALOG_ANIMATION_) && defined(_YMS_TEXTURE_ANIMATION_)
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_RENEWAL_                                     //! 2010.12.13 / i4u4me / �α��� ȭ�� ������ 
        /*($Delete_[S/C]$)*/#define _YMS_SCREEN_RATIO_                                      //! 2011.1.24 / i4u4me / 16:10 ���� �߰�  (WZENGINEVER >= 280) �̻�
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_CHARACTER_CAMERA_RENEWAL_                    //! 2011.1.25 / i4u4me / ĳ���� ī�޶� ���� ���� 
        /*($Delete_[S/C]$)*/#define _YMS_LOADING_IMAGE_                                     //! 2011.1.27 / i4u4me / �ε� �̹��� �ϳ��� ���� 
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA_0_20110208_SKILLTREE_VISUAL_RENEWAL                     // 2011.02.11 / ����� / ��ų�ý��� ���� ������, �⺻���� 0���� ����

    /*($Delete_[S/C]$)*/#define _DH_DIVIDE_MESSAGE_TEXT                                     // 2011.12.22 / �̵��� / messagetext.txt �и��۾�

    /*($Delete_[S/C]$)*/#define _YMS_SMOOTH_GAUGE_                                          // 2011.2.14 / i4u4me / hp ȸ�� ������ ������ �ε巴��

    /*($Delete_[S/C]$)*/#define _YMS_DEATH_SYNC_                                            //! 2011.2.21 / i4u4me / ���� ��� ����ȭ
    /*($Delete_[S/C]$)*/#define _DH_FATE_REMOVE_MAGIC_ARMOR                                 // 2011.02.23 / �̵��� / ��Ű������ ���� UI���� ���� ���� ��Ʈ�� �� ȿ�� ����

    /*($Delete_[S/C]$)*/#define _NA_002229_20110302_PARTY_DEFUALT_LOOTING_CHANGE            // 2011.03.02 / ������ / ��Ƽ ����Ʈ ������ �й� ��� ���� (���� -> ���ʴ��)
    /*($Delete_[S/C]$)*/#define _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT           // 2011.03.10 / ������ / ��ũŸ�� �̺�Ʈ ���庰 ���� ���� ��� �߰�

    /*($Delete_[S/C]$)*/#define _YMS_SKILL_TOOLTIP_                                         //! 2011.2.28 / i4u4me / ��ų���� ����: �����߰� �ɼ� �ֱ�
    /*($Delete_[S/C]$)*/#define _NA_002128_20110228_MODIFY_ITEM_LEVEL                       // 2011.02.28 / ������, �̵��� / ������ ������ ���� ����
    /*($Delete_[S/C]$)*/#define _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE          // 2011.03.23 / ������,������ / ���� �й� ��� ����

    /*($Delete_[S/C]$)*/#define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / �����, �����/ ������ �� �ӽ� �����丵(ī�װ�)/
    /*($Delete_[x/C]$)*/#define _DH_GM_ITEM_WINDOW  // 2011.03.03 / �̵��� / GM Item Window (�ļ��� ������)
    
    //#if !defined(_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED)
    //  #define _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED  // 2010.03.28 / ������ / OptionList ��ũ��Ʈ�� RatioValue �ʵ� �߰� �۾�(�ɼǰ��� ����Ÿ�Կ��� �ǹ̿� ���� %�� �ٿ� ����� �� �ֵ���)
    //#endif

    /*($Delete_[S/C]$)*/#define _DH_SHOW_ITEM_CREATE_RATE // 2011.03.28 / �̵��� / ������ ����(����) Ȯ�� ǥ�� ���
    /*($Delete_[S/C]$)*/#define _NA_002474_20110330_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2011.03.30 / ������ / �齺�� ��������Ʈ ���� ��� �ð� ����

    /*($Delete_[S/C]$)*/#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                       // 2011.03.18 / �����, / ���� ������ �Ϸ� �Ⱓ ���� �߰�.
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION            // 2011.04.01 / ������ / �ʵ�,��Ʋ�� ������ ��� ����ȭ ���� �۾�
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES    // 2011.04.01 / ������ / ��� Group(4) ��� �߰� (��ø �������� ��ø��Ű�� �ʰ� ������ ���)

    /*($Delete_[S/C]$)*/#define _NA_002471_20110408_EXP_BENEFIT_UP                          // 2011.04.08 / ������ / ����ġ ���ʽ� �ִ밪 ���� ���� (�ִ� ȹ�淮�� �� ����ġ�� 500%���� 1000%��, �� ���� ����)

    /*($Delete_[S/C]$)*/#define _K2P_SHOUT_DEFAULT // 2008.07.25 / ���ǥ / ��ġ�� ����Ʈ�� ���� define �̸� ����

    #if defined(_KOREA)
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                 // 2010.10.11 / �����, / ���� ����Ʈ ���޽� ����/�ִ밹�� �� �����ϴ� �ɼ� �߰�.

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / ������ / Ep2<-Ep1 ��ȯ�� ��� ������ ���� ���� ó�� (�Ϻ� ����)

    #elif defined(_GLOBAL)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER // 2010.06.04 / ������, ������ / ���� ���� ���� ��� ����
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION // 2010.10.11 / �����, / ���� ����Ʈ ���޽� ����/�ִ밹�� �� �����ϴ� �ɼ� �߰�.
        //#define _DH_GLOBAL_VERSION_TEST // 2011.04.21 / �̵��� / �۷ι� ��������Ʈ �׼�/�Ϲ� �и��� �ڵ�
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / ������ / Ep2<-Ep1 ��ȯ�� ��� ������ ���� ���� ó�� (�Ϻ� ����)
    #elif defined(_RUSSIA)
        #define _RU_20110801_SEND_GIFT_FRIEND_CHECK //������/ ģ������ ������ ������ ���� ������ ģ������ �ϰ� ���浵 ���� ģ������ �Ѵ�.
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.0.0.6 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1006
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG //! 2010.10.19 / i4u4me / �ŷ����ϰ�쿡�� �ý��� �޴� ���� �ʴ´� 
    /*($Delete_[S/C]$)*/#define _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM // ������, �豤�� / ��Ű �����۵� �и� �������(���׸��Ƹ�...)
    /*($Delete_[x/C]$)*/#define _DH_QUICK_SLOT_LINK // 2010.11.12 / �̵��� / �����Կ� ��ϵ� ��� ������ ���� ���� ����(�κ�,��Ţ�϶� update, �׿��� ��� unlink)
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_COOLTIME_DRAW // 2010.11.22 / �̵��� / ��Ÿ�� ȿ�� ����
    /*($Delete_[S/C]$)*/#define _NA_0_20110210_INVITE_PARTY_BUG                             // 2011.02.10 / ����� / 2���̼� ��Ƽ �δ� ���߿� ��3�ڿ��� ��Ƽ��û�� �Ұ�� �ٸ� ����� ��Ƽ�� �ξ����� ���� ����.

    #if defined(_KOREA)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / �����, �����/ ������ �� �ӽ� �����丵(ī�װ�)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / �����, ������ / ���λ��� ����/�Ǹ� ���Լ� ����(���� 24��->40��)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / ������ / �Ⱓ Ȱ��ȭ ������ �ŷ� �Ҵ�

    #elif defined(_CHINA)
        #define _HACKSHIELD_CHECK_OPTION_LMP // �ٽǵ� lmp ����
		#define _SCJ_PK_POPUP_DIALOG // pk �޼���
        #define _NA_0_20110305_BUG_READ_BIG_LOGFILE                         // 2011.03.05 / ����� / 2GB�̻��� �α� �б� ���� ����.(���� �ڵ� �б� �Լ��� ����)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / �����, ������ / ���λ��� ����/�Ǹ� ���Լ� ����(���� 24��->40��)
        #ifndef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
            #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                      // 2011.03.08 / �����, ����� / ĳ���� �����ӽ� ��ų ��Ÿ�� ���� ��� �߰�.
        #endif

    #elif defined(_JAPAN)
        #define _JP_0_20101123_PMS                                         // �Ϻ� ���ӿ��� ������ ���� �ý����� PMS ����
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / �����, ������ / ���λ��� ����/�Ǹ� ���Լ� ����(���� 24��->40��)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / ������, ������ / ������ ���� ��� ������ ���� ��� ������ üũ ��� ����
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / ������, ������ / ���� ���� ���� ��� ����
        #define _JP_20110222_INGAME_CHARNAME_CHANGE                        // 2010.2.22  / ������, ������ / �Ϻ��� ���� ������ ���� ĳ���߸� ������ ���ӳ����� �������ش� (������ ĳ���� ������ �ȵȴ�.)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / ������ / �Ⱓ Ȱ��ȭ ������ �ŷ� �Ҵ�    

    #elif defined(_GLOBAL)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / �����, �����/ ������ �� �ӽ� �����丵(ī�װ�)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / �����, ������ / ���λ��� ����/�Ǹ� ���Լ� ����(���� 24��->40��)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / ������ / �Ⱓ Ȱ��ȭ ������ �ŷ� �Ҵ�        
    #elif defined(_RUSSIA)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / �����, ������ / ���λ��� ����/�Ǹ� ���Լ� ����(���� 24��->40��)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / ������, ������ / ������ ���� ��� ������ ���� ��� ������ üũ ��� ����
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / ������, ������ / ���� ���� ���� ��� ����

    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.0.0.5 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1005
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)
    /*($Delete_[S/C]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / ������, ������ / �ý��� �޸� ���� ���� �α� �߰�
    /*($Delete_[x/C]$)*/#define _101011_LTJ_DOMINATION_GUILDMEMBER_EFFECT                   // 2010.10.13 / ������ / ������ �������� ����Ʈ
    /*($Delete_[S/C]$)*/#define _NA_0_20100722_AUTO_SYSTEM                                  // 2010.07.22 / ����� / ���� �ý���
    #define _YMS_AUTOBOT_GAME                                           // 2010.07.01 / ����� / �������� �������� 211 ���� �����Ѵ�.
    /*($Delete_[S/C]$)*/#define _NA_0_20100817_HELLON_LAST_TRIGGER                          // 2010.08.17 / ������, ������ / ��� ���� �̼ǿ� ���ϸ��̼� ���� Ʈ����.
    /*($Delete_[x/C]$)*/#define _GS_ADD_CAMERA_VIEW_POS                                     // 2010.10.28 / �豤�� / ĳ���� ī�޶� �������� �ɼ�
    /*($Delete_[x/C]$)*/#define _DH_DISPLAY_FOR_MISSION_OBJECT                              // 2010.08.02 / �̵��� / �̼� ���� ���� ����Ʈ ǥ�� �۾�
    /*($Delete_[S/C]$)*/#define _NA_000587_20100928_DOMINATION_BUFF_ITEM                    // 2010.09.28 / ������, ����� / ���� ���� ��ȭ ���� ������
    /*($Delete_[S/C]$)*/#define _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL    // 2010.10.01 / ������, �̵��� / ä�� �ʴ� ��� ��ȭ
    /*($Delete_[S/C]$)*/#define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_        // 2010.10.11 / ������ / �߱� �̽�, ������ etherealizable item�� ���� �߰� ��� ó�� (with script control)

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
      #endif

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.0.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1004
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_ADD_RADAR_ICON_100622            // 2010.06.17 / ����� / �̴ϸ� ǥ�� ������Ʈ 
    /*($Delete_[S/C]$)*/#define _YMS_ESCKEY_CLOSE_DIALOG                                // 2010.06.14 / ����� / �Ϻ� UI�� ESC Ű�� ������ �ʴ� ����
    /*($Delete_[S/C]$)*/#define _YMS_SYSTEMINPUTBOX_ONLYNUMBER                          // 2010.05.28 / ����� / �ý��� ��ǲ â���� ���ڸ� ����ġ�� ���� ����(��Ʈ�ѿ� ���ڸ� �Է� ������ �ִ� ��� �߰�)
    /*($Delete_[S/C]$)*/#define _YMS_PET_UI                                             // 2010.05.18 / ����� / �� UI ����. 
    /*($Delete_[S/C]$)*/#define _YMS_GUILD_DETAL_SPEC                                   // 2010.05.11 / ����� / ���� �� ���� ��� 
    /*($Delete_[S/C]$)*/#define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                  // 2010.05.11 / ����� / ��Ż��ġ���� �̼ǿϷ��� ����Ʈ�� �Ϸ� ���ϴ� ����
    /*($Delete_[S/C]$)*/#define _100708_LTJ_SHOW_DATETIME_IN_TIMER                      // 2010.07.26 / ������ / Ÿ�̸�(�Ƿε�)�� ��¥�� ǥ��
    /*($Delete_[S/C]$)*/#define _100705_LTJ_CONTROL_LOD_DISTANCE_PLAYER_COUNT           // 2010.07.26 / ������ / �÷��̾� ī��Ʈ�� ���� LOD ī�޶� �Ÿ� ����
    /*($Delete_[S/C]$)*/#define _100416_LTJ_MODIFY_CHAT_FILTERING                       // 2010.07.26 / ������ / ä�� ��Ģ�� ���͸� ����
    /*($Delete_[S/C]$)*/#define _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER                // 2010.07.26 / ������ / ���¹��� ������ �ؽ������� �۾�
    /*($Delete_[S/C]$)*/#define _GS_ADD_PET_ENCHANT_EFFECT                              // 2010.07.26 / �豤�� / ����þƮȿ��
    /*($Delete_[S/C]$)*/#define _YJ_FADE_FAR_TERRAIN                                    // 2010.07.26 / ������ / ���� ���� �۾�
    /*($Delete_[S/C]$)*/#define _YJ_SHOW_TERRAIN_DRAWCALLS                              // 2010.07.26 / ������ / ���� �޼����� ���� ��ο��� �� ���
    /*($Delete_[S/C]$)*/#define _NA_0_20100705_DOMINATION_EVENT                         // 2010.07.05 / ��ȣ��, ������ / ������ �̺�Ʈ Ʈ����(���͸���).
    /*($Delete_[S/C]$)*/#define _NA_0_20100706_RANDOM_REGEN                             // 2010.07.06 / ��ȣ�� / ���� ���� �籸��.
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG               // 2010.07.06 / ��ȣ�� / ���� ���� ������ ��� �α� �߰�.

    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ // ������, �̵��� / �κ��丮 ���� ��� ����
    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_ // ������ / �κ��丮 �и� �۾�
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_ // ������, �豤�� / ������ Ŀ���͸���¡
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_ // 2010.10.13 / ������, ������ / �������� ����ϴ� ���׸��� ���� ������ ó��
    /*($Delete_[S/C]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_ // ������, ����� / C/S �ð� ����ȭ ó��
    /*($Delete_[S/C]$)*/#define _NA000000_100623_CHECK_WMO_CHECKSUM_ // ������, ������ / �ʵ� ����� WMO ���� �˻� ��ƾ �߰� (engine���� �˻��� �̷��ַ� �߻�...�Ф�)
    /*($Delete_[S/C]$)*/#define _NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_ // ������ / �ű� ĳ���� �����ϴ� ��� ������ Ȯ�� ����

    /*($Delete_[S/C]$)*/#define _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM // 2010.05.27 / ������, �豤�� / ĳ���� �ܸ� ���� �ý���
    /*($Delete_[S/C]$)*/#define _NA_000070_20100609_ENHANCED_ITEM_DIVISION // 2010.06.09 / ������, �̵��� / ������ ���� ��� ����
    /*($Delete_[S/C]$)*/#define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST // 2010.06.22 / ������, �̵��� / ���� ��� ����

    /*($Delete_[S/C]$)*/#define _NA_0_20100325_QUICK_SLOT_CHANGE						// 2010.03.25 / �����,�̵��� / Quick ���� ���� (48���� ����->40���� �������� ����)
    /*($Delete_[S/C]$)*/#define _DH_SHOW_REPAIR_HAIM                                    // 2010.07.23 / �̵��� / ���� ������ ���� 3�ڸ� ������(,) �߰�
    /*($Delete_[S/C]$)*/#define _DH_SHOW_UI_NAME                                        // 2010.07.13 / �̵��� / UI �̸� ǥ�ñ��
    /*($Delete_[S/C]$)*/#define _DH_NORMAL_CHAT_BLOCK                                   // 2010.06.22 / �̵��� / ���� ��� ����
    /*($Delete_[S/C]$)*/#define _DH_ENABLE_DIVIDING_ITEM_WITH_CLICKING_SLOT_TAB         // 2010.06.04 / �̵��� / ������ �й�ÿ� ���� ���� �����ϵ��� ����
    /*($Delete_[S/C]$)*/#define _DH_ENHANCED_UI_PACKAGE_CHATTING                        // 2010.06.17 / �̵��� / UI������ �� ä��â
    /*($Delete_[x/C]$)*/#define _DH_ADD_TYPE_MARK_FOR_ITEM         // 2010.05.06 / �̵��� / �������� ���� ������ �ĺ� �۾�
    /*($Delete_[S/C]$)*/#define _DH_TRIGGER_LOG_MESSAGE                                 // 2010.06.26 / �̵��� / ���� ���� Ŭ���̾�Ʈ���� Ʈ���� �޽��� ��� �۾�
    /*($Delete_[S/C]$)*/#define _DH_DOUBLECLICK_MOVE_WAYPOINT                           // 2010.06.26 / �̵��� / ��������Ʈ ����Ʈ ����Ŭ�����ε� �̵� ��û �����ϰ� ����
    /*($Delete_[S/C]$)*/#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION                // 2010.05.20 / ������ / â�� �̵� �Ұ��� �ɼ� �߰�, ��� â��� ����
    /*($Delete_[S/C]$)*/#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END      // 2010.06.21 / ������ / ���� ������ �̺�Ʈ �߰�

    /*($Delete_[S/C]$)*/#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_             // 2010.05.10 / ������
    /*($Delete_[S/C]$)*/#define _NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG        // 2010.06.16 / ����� / ��Ƽ ������ ��������� ��Ƽ���� �����Կ��� ���ԵǴ� ���� ����.
    /*($Delete_[S/C]$)*/#define _GS_MOD_CASH_SHOP_TOOLTIP_BY_TYPE                       // 2010.09.14 / �豤�� / ĳ�������� ���׸���.�������� ������µǰԲ�����

    /*($Delete_[S/C]$)*/#define _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND                 // 2010.09.13 / ������ / ��� â�� �ʱ�ȭ GM ��ɾ� �߰�

    /*($Delete_[S/C]$)*/#define _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION             // 2010.11.02 / ������ / DBQuery���� ���� ���� ���� �۾�

    #if defined(_KOREA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // ������, �豤�� / ������ Ŀ���͸���¡, ������ ����� �и� ��� ����(������ ���� ������)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                          // 2010.06.04 / ������, ������ / ���� ���� ���� ��� ����
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / ��ȣ��, ����� / ģ��,����,������ ���� �޸�.

    #elif defined(_CHINA)
        #define _YMS_AUTOBOT_GAME                                               // [2010-7-1 by i4u4me] �������� �������� 211 ���� �����Ѵ�.
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // ������, �豤�� / ������ Ŀ���͸���¡, ������ ����� �и� ��� ����(������ ���� ������)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                          // 2009.10.21 / ������, �̵��� / ģ�� �ʴ� �ý���
        #define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_            // 2010.10.11 / ������ / �߱� �̽�, ������ etherealizable item�� ���� �߰� ��� ó�� (with script control)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / �����, / ���� ����Ʈ ���޽� ����/�ִ밹�� �� �����ϴ� �ɼ� �߰�.
        #define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG                             // 2010.10.19 / i4u4me / �ŷ����ϰ�쿡�� �ý��� �޴� ���� �ʴ´� 
        #define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                       // 2010.10.26 / ������ / �׷� ���ʽ� ���� ��� ��,���� ��� ���� ����
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / ��ȣ��, ����� / ģ��,����,������ ���� �޸�.

    #elif defined(_JAPAN)
		/*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_ // ������, �豤�� / ������ Ŀ���͸���¡, ������ ����� �и� ��� ����(������ ���� ������)
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / ��ȣ��, ����� / ģ��,����,������ ���� �޸�.

    #elif defined(_GLOBAL)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // ������, �豤�� / ������ Ŀ���͸���¡, ������ ����� �и� ��� ����(������ ���� ������)

    #elif defined(_RUSSIA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // ������, �豤�� / ������ Ŀ���͸���¡, ������ ����� �и� ��� ����(������ ���� ������)
        #define _RU_110209_RUSSIA_CONTENTS_BLOCK                                // ���þ� ������ �� ������
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / �����, / ���� ����Ʈ ���޽� ����/�ִ밹�� �� �����ϴ� �ɼ� �߰�.
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.0.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1002
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    // unified protocol
    /*($Delete_[S/C]$)*/#define _NA00XXXX_090111_CASH_TYPE_SHARED_                      // ������, �̵��� / ĳ��Ÿ��(PPī��) �߰��� ���� ���� �۾� (�� ���� ����)
    /*($Delete_[x/C]$)*/#define _NA_0_20100208_DOMINATION_AREA_INFO                     // 2010.02.08 / ��ȣ��, ������ / ������ ���� ������ �����ֱ�.
    /*($Delete_[S/C]$)*/#define _NA_0_20100525_ADD_TIMER_ACTION_TRIGGER                 // 2010.05.25 / ��ȣ��, ������ / Ÿ�̸� �ߵ� �׼� Ʈ���� �߰�.
    /*($Delete_[S/C]$)*/#define _NA_0_20100514_NPC_DEAD_STATE_TIME                      // 2010.05.14 / ��ȣ�� / ���� ���� ���� ���� �ð� ���� ���������� ��ũ��Ʈ���� �����ϵ��� ����. 
    /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / ��ȣ�� / ����Ʈ �ݺ� ���� �ǽ� �κ� ����
    /*($Delete_[S/C]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / ��ȣ�� / ����Ʈ ���� üũ �߰�.    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_DURA                      // 2010.04.12 / ��ȣ�� / ������ ������ ����� ������ �϶� ���� ���� (10% -> 5%) 
    /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGER // 2010.02.22 / ��ȣ��, ������, ������ / ���� ���� ��û.
    /*($Delete_[S/C]$)*/#ifdef _NA_0_20100222_UNIT_TRIGGER
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_ADD // 2010.02.22 / ��ȣ��, ������, ������ / ���� ���� ��û. 
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_PATHLIST // 2010.02.22 / ��ȣ��, ������ / ���� AI ����.  
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_REGEN // 2010.02.22 / ��ȣ��, ������, ������ / ���� ���� ����.
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER // 2010.02.22 / ��ȣ�� / �Ʊ��� ���� �߰�.
        /*($Delete_[S/C]$)*/#define __NA_276_20070213_UNIT_TRIGGER // 2010.02.22 / ��ο�,��ȣ�� / ���� ���� �̱��� Ʈ���� ��û / 
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[x/C]$)*/#define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / ��ȣ�� / PVP�� ����Ŀ ������ ��Ŀ���� ��ų�� HP�� �ʰ� ȸ�� �ϴ� ����.
    /*($Delete_[x/C]$)*/#define _100114_LTJ_USE_AMBIENTSOUND_SCRIPT_FOR_AMBIENT_SOUND   // 2010.03.25 / ������ / �����Ʈ���� ��ũ��Ʈ�� ���� ��� ����
    /*($Delete_[x/C]$)*/#define _GS_MOD_PLAYER_LOADING                // 2010.03.25 / �豤�� / Player���ν� �����ε� �� ��Ƽ������ ��ɰ���
    /*($Delete_[S/C]$)*/#define _NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY      // 2010.03.16 / ������, ����� / ��Ʋ�� �κ� ���� ������ �� ����
    /*($Delete_[S/C]$)*/#define _NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2          // ������, ����� / ���� ���� �ʱ�ȭ ���� ���� 40���� 70���� ����
    /*($Delete_[x/C]$)*/#define _DH_COUNT_LOGIN_FRIEND                                  // 2010.03.23 / �̵��� / ģ�� ������ �� ǥ��, �׷� ����
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_EXP_COLOR                                    // 2010.03.22 / �̵��� / �ְ��� �޼��� ����ġ ������ ���� ����
    /*($Delete_[x/C]$)*/#define _NA_0_20091118_GAMEROOM_BATTLE_RESULT                   // 2010.03.24 / �̵���, ����� / ����������� �ý���
    /*($Delete_[x/C]$)*/#define _DH_CUR_SKILL_NEEDSFOR_TOOLTIP                          // 2009.11.17 / �̵��� / �̹� ���� ��ų�� �䱸 ���õ� ����
    /*($Delete_[x/C]$)*/#define _DH_HIDE_INCANT_EFFECT                                  // 2009.11.18 / �̵��� / ī�޶� ���� ���¿��� ��þƮ ����Ʈ �����
    /*($Delete_[x/C]$)*/#define _DH_DBLCLICK_FRIEND_STATUS                              // 2009.11.16 / �̵��� / ģ����� ���� Ŭ������ ������ ����
    /*($Delete_[x/C]$)*/#define _DH_BATTLEZONE_CREATE_CLASS_TOOLTIP                     // 2009.11.17 / �̵��� / ��Ʋ�� ����â���� Ŭ���� ���� ����
    /*($Delete_[x/C]$)*/#define _DH_GUILDINFO_OFFLINE_COLOR                             // 2009.11.11 / �̵��� / ���â���� �������� ���� ȸ�� ���� ����
    /*($Delete_[x/C]$)*/#define _DH_PARTY_UI_CUSTOMIZING                                // 2009.11.16 / �̵��� / ��ƼUI ����� ���
    /*($Delete_[x/C]$)*/#define _DH_VENDER_WINPOS                                       // 2009.11.10 / �̵��� / ���λ��� ��ġ ���� ����
    
    /*($Delete_[x/C]$)*/#define _DH_PARTY_LEADER_CHANGE_CLOSE_OPTWIN                    // 2009.11.12 / �̵��� / ��Ƽ�� ����� �ɼ�â �ݱ�
    /*($Delete_[x/C]$)*/#define _DH_PANDORA_TOOLTIP_ADD_MARGIN                          // 2009.11.25 / �̵��� / ������ ���� ���� �ٰ��� ����
    /*($Delete_[x/C]$)*/#define _DH_MISSIONZONE_NO_ITEMMESSAGE                          // 2009.11.20 / �̵��� / �̼������� ������ �й� �޽��� �����
    /*($Delete_[x/C]$)*/#define _NA_20100106_REPURCHAS_BUGFIX_2496                      //2010.01.06 ������ ��Ʋ�� ���� ���� �籸���Ҷ� �籸�� �ȵǴ� ���� ����

    /*($Delete_[x/C]$)*/#define _YMS_NUMBER_TO_MONEY                                    // 2010.03.25 / ����� / ���� õ������ �޸� �Է�
    /*($Delete_[x/C]$)*/#define _YMS_ESCKEY_COMPOSITE                                   // 2010.03.25 / ����� / ��ȭâ escŰ�� ��� ����
    /*($Delete_[x/C]$)*/#define _YMS_UI_RBUTTONCLICKI_ENCHANT_CRYSTALLIZATION_RANKUP    // 2010.03.25 / ����� / [�ѱ�][0032] ��ȭâ ������ ��� ���� ����
    /*($Delete_[x/C]$)*/#define _YMS_UI_STATE_REMEMBER                                  // 2010.03.25 / ����� / [�ѱ�][0030]UI Ŭ�� ���� ����
    /*($Delete_[x/C]$)*/#define _YMS_AREACONQUEST_MAP_TOOLTIP                           // 2010.03.25 / ����� / [�ѱ�][0025]AC �������� ���� ��� 
    /*($Delete_[x/C]$)*/#define _YMS_GAMEOPTION_LANGUAGETYPE                            // 2010.03.25 / ����� / ������ ���� �߰�
    /*($Delete_[x/C]$)*/#define _YMS_SOUND_CACHE                                        // 2010.03.25 / ����� / ���� ĳ��
    /*($Delete_[x/C]$)*/#define _YMS_SUMMONSKILLCONTAINER_RESET                         // 2010.05.07 / ����� / ��Ű�� ��ȯ������ �� ��ȯ�� ��ųâ�� ���ŵ��� �ʴ� ����
    /*($Delete_[x/C]$)*/#define _YMS_EVENTFRIEND_SCROLLBAR                              // 2010.05.14 / ����� / ģ���ʴ� �޼��� ��ũ�� �ǰ�

    /*($Delete_[x/C]$)*/#define _NA_1419_20100121_ACCUMULATE_POINT                      // 2010.01.21 / �����,������ / �÷��� �ð��� ���� ���� ����Ʈ �߰�
    /*($Delete_[x/C]$)*/#define _100409_LTJ_SHOW_ITEM_UNIT_PRICE_IN_VENDOR              // 2010.04.13 / ������ / ���λ��� ���簡�� �����ֱ�
    /*($Delete_[x/C]$)*/#define _100408_LTJ_SEPARATE_TIMER_INI                          // 2010.04.13 / ������ / Ÿ�̸� ���� ������ PROGRAM.INI ���� TIMER.INI �� �и�

    #if defined(_KOREA)
    
    #elif defined(_CHINA)
        //#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)

    #elif defined(_JAPAN)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / ������, �̵��� / ģ�� �ʴ� �ý���
        #define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST            // 2010.06.22 / ������, �̵��� / ���� ��� ����

    #elif defined(_GLOBAL)
		#define _100708_LTJ_SHOW_DATETIME_IN_TIMER
		/*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)
        /*($Delete_[S/x]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / ������, ������ / �ý��� �޸� ���� ���� �α� �߰�
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / ����� / ��Ż��ġ���� �̼ǿϷ��� ����Ʈ�� �Ϸ� ���ϴ� ����

    #elif defined(_RUSSIA)
        #define _100708_LTJ_SHOW_DATETIME_IN_TIMER
        /*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)
        #define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / ������, ������ / �ý��� �޸� ���� ���� �α� �߰�
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / ����� / ��Ż��ġ���� �̼ǿϷ��� ����Ʈ�� �Ϸ� ���ϴ� ����

    #endif
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 1.0.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 1001
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(2)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

    /*($Delete_[S/C]$)*/#define _NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP                // 2009.11.19 / ������ / NPC ���� �Ǹ� ����� ���� ��� ������ ���� ���� ����
    /*($Delete_[S/C]$)*/#define _NA_0_20091030_RANDOM_ITEM                                  // 2009.10.30 / �����, �̵��� / ���� ������ �߰�.
    /*($Delete_[x/C]$)*/#define _GS_SEPARATE_SPECIAL_TEXTURE                                // 2009.12.04 / �豤�� / ������ؽ��� ��ũ��Ʈ�� �и��ϴ��۾�
    /*($Delete_[x/C]$)*/#define _GS_MOD_NONE_DRAW_ZEROEXP									// 2009/12.07 / �豤�� / EXP 0�϶��¾����
    
	/*($Delete_[x/C]$)*/#define _091229_LTJ_AC_REWARD_HELPMESSAGE_BUGFIX					// 2009.12.29 / ������ / AC 10%���� ���޽� ����ȳ� �����޽��� ��µǵ��� ����
	/*($Delete_[x/C]$)*/#define _100105_LTJ_ADD_SSQ_OBVERVER_ASSIST_DIALOG					// 2010.01.07 / ������ / Ÿ���� ��� �������â�� Assistâ���� ����

	/*($Delete_[x/C]$)*/#define _YJ_MOD_FOG_COLOR_IN_CREATE_CHARACTER						// 2010.01.12 / ������ / ĳ�� ����ȭ�� ������Ǹ鼭 ����� ���� �÷� ���� 
	/*($Delete_[x/C]$)*/#define _YJ_MOD_POS_IN_CHARACTER_SCENE								// 2010.01.12 / ������ / ĳ�� ����ȭ�� ������Ǹ鼭 ����� ĳ�� ������ ����

    /*($Delete_[S/C]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                           // 2010.01.14 / ��ȣ�� / ����Ʈ ����� NPC���� �Ÿ� üũ.
    /*($Delete_[S/C]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                          // 2010.01.06 / ��ȣ�� / SD ��� ���� ���� 

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION
    /*($Delete_[S/C]$)*/#ifdef _NA001385_20090924_DOMINATION
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_GAMELOG_ACTION            // 2010.01.12 / ��ȣ�� / ���� �������� ���� �α�
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_FINDPATH_ATTACK           // 2009.12.15 / ��ȣ�� / ���ݽ� ���� üũ(findpath)
        /*($Delete_[S/C]$)*/#define _0_20100427_DOMINATION_FINDPATH_ATTACK_BUG              // 2010.04.27 / ��ȣ�� / ���� ���� �̵��� �� �Ѿ�� �̵��Ǵ� ���� ����.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SHADOW                    // 2009.12.15 / ��ȣ�� / ������ ���̵� ��ų ����.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NPC_MAXHP                 // 2009.12.09 / ��ȣ�� / monster render info�� maxHp �߰�
        /*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_SYNC                     // 2009.08.17 / ��ȣ�� / ä�� �Ϸ� ����ȭ.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_BASE                      // 2009.09.24 / ��ȣ�� / ����������._Map
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP                     // 2009.09.24 / ��ȣ�� / ����������._stamp
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_ETC                       // 2009.09.24 / ��ȣ�� / ����������._etc(��Ŷ,�ļ�)
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_TRIGGER                   // 2009.09.24 / ��ȣ�� / ����������._trigger
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SKILL                     // 2009.09.24 / ��ȣ�� / ����������._ũ����Ż ���� ��ų
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_EVENT_CONTROL             // 2009.09.24 / ��ȣ�� / ����������._�̺�Ʈ ��Ʈ��
        /*($Delete_[S/C]$)*/#define _0_20100419_DOMINATION_RETURN_TO_ENTRANCE_BUG           // 2009.09.24 / ��ȣ�� / ����������.�Ա��� ���� �̵� ���� �ʴ� ���� ���� ���� ���� ��Ű��.
        /*($Delete_[S/C]$)*/#define __KR_001385_20090907_DOMINATION_PACKET                  // 2009.09.07 / ������ / ���� ������
        /*($Delete_[S/C]$)*/#define _KR_001385_20090907_DOMINATION_PARSER                   // 2009.09.07 / ������ / ���� ������ �ļ� �۾� ����
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_QUERY                    // 2009.10.13 / ������ / ���� ������ DB������ �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GUILDSERVER_BASE         // 2009.10.13 / ������ / ��� ���� �⺻ �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GAMESERVER_BASE          // 2009.10.13 / ������ / ���� ���� �⺻ �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_BASICINFO_SETTING        // 2009.10.13 / ������ / ���� ������ �⺻���� ���� �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091103_DOMINATION_ZONEMOVE_NOTIC           // 2009.11.03 / ������ / �� �̵��ÿ� ���� ������ ��� ���� ǥ�� �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO     // 2009.11.05 / ������ / ���� ������ ���� ������Ʈ ���� �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091202_DOMINATION_RETURN_HEIM              // 2009.12.02 / ������ / ���� ������ / ���� �ǵ����ֱ� ���� �۾�
        /*($Delete_[S/C]$)*/#define _KR_001385_20091211_DOMINATION_GAMELOG_MONEY            // 2009.12.13 / ������ / ���� ������ ���� �α� �۾�
		/*($Delete_[S/C]$)*/#define __001385_LTJ_RENEWAL_NOTICE								// 2009.11.27 / ������ / �������� ������
		/*($Delete_[S/C]$)*/#define __001385_LTJ_ADD_DOMINATIONWAR_TOWER_UI					// 2009.11.27 / ������ / ���������� ���� UI �۾�
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_CAST_LEAVE_DLG								// 2009.11.27 / ������ / CastLeave Dlg ������
		/*($Delete_[S/C]$)*/#define _YJ_MOD_MAPOBJECT_MONSTER								// 2009.11.27 / ������ / ��ġ�� ���� ��� ��� ����	
		/*($Delete_[S/C]$)*/#define _YJ_EDIT_OBJ_COLOR_APPLY								// 2009.11.27 / ������ / �������� ������ �ʿ�����Ʈ/���ǵ�Ʈ�� �÷��� ����
		/*($Delete_[S/C]$)*/#define _YJ_ADD_RADAR_ICON										// 2009.11.27 / ������ / ���������� ���� ���̴� ������ �߰�
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_TEXT_ON_HEAD								// 2009.11.27 / ������ / ĳ���� �Ӹ��� ĳ���� 
		/*($Delete_[S/C]$)*/#define _YJ_ADD_DAMAGE_EFFECT_NPCINFO							// 2009.12.15 / ������ / NPCINFO�� ������ ����Ʈ �ڵ� �߰�
		/*($Delete_[S/C]$)*/#define _YJ_STAMP_GAGE											// 2010.01.08 / ������ / ���� ������ �۾�(Engine Ver 120)
    #endif

    /*($Delete_[x/C]$)*/#define _NA_0_20091104_FIRENDMAP_REFACTORYING                       // 2009.11.04 / ������ / ���� ������ User Ŭ�������� ģ�� �� ���� �κ��� FriendMap Ŭ������ �и� �Ѵ�.

    /*($Delete_[S/C]$)*/#define _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_				// 2009.11.23 / ������, ������ / �������� Ÿ�� �Ӽ� �߰� + �� ���� ���� AI����
    /*($Delete_[x/C]$)*/#define _NA_0_20100312_MODIFY_DELIMITERS_FOR_TEXTSCRIPT             // 2010.03.12 / ������ / (QuestText, Message, HelpLoadingMessage)��ũ��Ʈ�� ���Ǵ� �������� " �� ������� �ʵ��� ����

    /*($Delete_[x/C]$)*/#define _YMS_RANKUP_TOOLTIP                                         // 2010-03-31 / ����� / ��ũ�� ��������(���� ��� ���� ��ġ �����)
    /*($Delete_[x/C]$)*/#define _YMS_CANNOT_EMOTION	                                        // 2010-03-31 / ����� / �Ǿ� ���¿��� ����ǥ�� �ȵǰ�

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NON_PREEMPTIVE_ATTACK_BUG     // 2010.04.09 / ��ȣ�� / ������ ���� ŉ��� ��ȹ�� ������ ���� Ÿ���� �Ǵ� ���� ����.
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_MAPNPC_AI_BUG                 // 2010.04.09 / ��ȣ�� / ���������� ���� AI ����    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP_STYLE_SKILL_BUG         // 2010.04.09 / ��ȣ�� / ���ν� ��Ÿ�� ��ų ������ �߰�
    

    #if defined(_KOREA)
        //���� �� �÷��׸� ���� �Ϸ��� �Ѵٸ� DB ģ�� ���̺��� ģ�� ���� �ڵ尡 5,6�� ������ ��� 1�� �����ؾ� �����ϴ�.
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / ������, �̵��� / ģ�� �ʴ� �ý���
        #define __CN_PET_OPTION_APPLY_FLAG								// 2008.08.24 / ��â�� / �� �ɼ� �÷��׷� ���� ����(��ũ��Ʈ����...)
        //#define _USING_GUILD_DESTROY_LOGGING                            // 2010.03.03 / ������ / ��� �ػ� ���� ������ ���� �α� �߰�
    #elif defined(_CHINA)
        #if defined(__APPLY_HACKSHIELD) && defined(_SUNGAME_VER)
            #define _APPLY_HACKSHIELD_MONITOR                           // 2010.03.17 / ����� / �߱� �ٽ��� ����͸� ����
        #endif
        #define _NA000000_100427_SSQ_DEFAULT_REWARD_                    // ������, ����� / Ÿ���� ��� ������ ���� �⺻ ���� ó��
        #define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / ��ȣ�� / PVP�� ����Ŀ ������ ��Ŀ���� ��ų�� HP�� �ʰ� ȸ�� �ϴ� ����.
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
      #endif
        #ifndef _YMS_SUMMONSKILLCONTAINER_RESET
            #define _YMS_SUMMONSKILLCONTAINER_RESET                     // 2010.05.07 / ����� / ��Ű�� ��ȯ������ �� ��ȯ�� ��ųâ�� ���ŵ��� �ʴ� ����
        #endif
        //#define _YMS_CHINA_TAIWAN_MERGE                                 // 2010.07.15 / ����� / �߱�/�븸 ��ħ 
        #ifndef _NA_20100802_SPEED_HACK_DETECTION
            #define _NA_20100802_SPEED_HACK_DETECTION                   // 2010.08.02 / ������, ����� / ������ In-Game ���ǵ��� Ž��
        #endif
		#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)
        #ifdef _SERVER
            #define _NA_0_20100929_LANGUAGE_OF_FILTER                       // 2010.09.29 / ����� / ���͸� ���� ������ ��� ���� �߰�.
        #endif

    #elif defined(_JAPAN)

    #elif defined(_RUSSIA)

    #endif

#endif
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.9.0.5 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 905
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(1)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

	/*($Delete_[x/C]$)*/#define __DH_HAIM_WIN_CLOSE								// 2009.09.11 / �̵��� / â�� �̵��� ������
	/*($Delete_[x/C]$)*/#define _DH_CHARNAME_CHECK_BUG_005306					// 2009/1109 / �̵��� / ĳ���� ������ �̸��� 16����Ʈ ���� ��� �ߺ�üũ�� �״����� ����
	/*($Delete_[x/C]$)*/#define _DH_GUILDHAIM_WIN_CLOSE							// 2009/1019 / �̵��� / ���â�� �־����� �ݴ� �۾�
	/*($Delete_[x/C]$)*/#define _DH_AC_UI_CHANGE								// 2009/1012 / �̵��� / AC UI���� ���� �۾�

	/*($Delete_[x/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL_2		// 2009/0704 / �̵��� / �׷� �α� �ý���-(ĳ�����˾��޴��� �׷���/����), �������� ���� ����ȵǵ��� ����
	/*($Delete_[x/C]$)*/#define _DH_GIFT_CONFIRM								// 2009/1012 / �̵��� / �����ϱ� ��Ȯ�� â �۾�
	/*($Delete_[x/C]$)*/#define _DH_HELLON_TOWER_ENTER							// 2009/0930 / �̵��� / ����� �� ���� �� ����
	/*($Delete_[x/C]$)*/#define _DH_SUNCREDIT									// 2009/0929 / �̵��� / �� ũ���� ���� UI
	/*($Delete_[x/C]$)*/#define __DH_COMMUNITY_MEMO__							// 2009/0821 / �̵��� / ���� �ٰ��� ����

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__  // 2009.09.06 / ������ / SolarSlot �ڵ� ����

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_UPDATE_VER6__       // 2009.09.06 / ������,�豤�� / SolarSlot ItemStream Ver6 update
    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM__                // 2009.09.15 / ������,�豤�� / Riding System
    /*($Delete_[x/C]$)*/#ifdef __NA001390_090915_RIDING_SYSTEM__
    /*($Delete_[x/C]$)*/#define _GS_RIDER_SYSTEM		// �豤�� / Ż�ͽý���
    /*($Delete_[x/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM_ETC__            // 2009.09.20 / ������,�豤�� / ���������� ������ �κ� ��������... ������ �Ǵ� �� �۾��ϱ�� �Ѵ�.
    /*($Delete_[S/C]$)*/#define __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__   // 2009.09.15 / ������,�豤�� / Riding System - Item Part

    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED        //< v6�� ���� �������� �߰��� �����...

	/*($Delete_[x/C]$)*/#define _GS_ELITE_FULLSET_CHANGE_ITEM					// 2009.10.19 / �豤�� / ����ƮǮ��2�� ü����������
    /*($Delete_[x/C]$)*/#define _GS_ADD_SHIELD_EFFECT                           // 2009.11.02 / �豤�� / ��������Ʈ�߰��۾�
    /*($Delete_[x/C]$)*/#define __GS_CHANGE_TARGET_EFFECT_TYPE                  // 2009.11.09 / �豤�� / Ÿ��������ƮŸ�Ժ����Ƿ����ѹ��׼���
    /*($Delete_[x/C]$)*/#define _GS_ADD_RIDER_IDLE_ANI                          // 2009.11.17 / �豤�� / Ż�Ͱ��� IDLE�ִϸ��̼��߰�
    /*($Delete_[x/C]$)*/#define _GS_LOOP_EFFECT_REMAIN                          // 2009.11.05 / �豤�� / �����ִϸ��̼ǿ��� �߻��Ǵ·�������Ʈ ������ ����.�������ϱ�
    /*($Delete_[x/C]$)*/#define _GS_ADD_CANDY_EVENT_WEAPON                      // 2009.11.23 / �豤�� / ĵ������Ʈ����
    /*($Delete_[x/C]$)*/#define _GS_MOD_SHADOW_HIDE_CONDITION                   // 2009.12.15 / �豤�� / ���̵������߰�(��Ƽ���&������������Ժ��̱�)

	/*($Delete_[x/C]$)*/#define __090907_LTJ_GRAPHIC_OPTION_ADD					// 2009.10.19 / ������ / Ÿ�Ϻ��� �׷��ȿɼ��߰�
	/*($Delete_[x/C]$)*/#define __090907_LTJ_GAME_OPTION_ADD					// 2009.10.19 / ������ / �ٸ�ĳ����,����ĳ���� ���̱� ���ӿɼ��߰�
	/*($Delete_[x/C]$)*/#define __090909_LTJ_CHATDIALOG_MOD						// 2009.10.19 / ������ / ä��â ä�ø���Ʈ ��������
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_POPUP_MENU_RCLICK_BUGFIX		// 2009.10.19 / ������ / ä��â ä�ø���Ʈ���� ��Ŭ�������� ä���˾��޴� �ȶߴ� ���׼���
	/*($Delete_[x/C]$)*/#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS				// 2009.08.14 / ������ / Į������ ���� ���밡�� Ŭ���� ���� �߰�
	/*($Delete_[x/C]$)*/#define __LTJ_HIDE_PET_NAME_JP							// 2009.07.28 / ������ / UI �����ɽ� ���� ������ �� �̸� ����
	/*($Delete_[x/C]$)*/#define __090910_LTJ_CHANGEUP_CLASSNAME_JP				// 2009.09.10 / ������ / ü������ ĳ������ ��� ���ο� Ŭ���������� ���
	/*($Delete_[x/C]$)*/#define _LTJ_HERO_INFO_BUGFIX							// 2009.10.30 / ������ / ��Ż�̵� �� Ŭ��/�������� ���� ����ġ ���� �� Ÿ���ѻ�� ����� ���� �ȵǴ� ���� ���� - ������� ����(HERO_INFO)

    /*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION			// 2009.07.09 / ��ȣ��,������ / PK Ÿ�� ���� �ɼ�.

    /*($Delete_[S/C]$)*/#define __NA_0_20090907_GUILD_DESTROY_GM_CMD            // 2009.09.07 / ��ȣ�� / ��� �ٷ� ���� ���� GM ��ɾ�. 
    
    /*($Delete_[S/C]$)*/#define __NA_0_POSSESSION_ITEM_DROP						// 2009.08.27 / ��â��, �ͼ� ������ ��� ����

    /*($Delete_[S/C]$)*/#define __NA_0_20090902_PET_NAME_CARD_LOG               // 2009.09.02 / ������ / �� ����ī�� ������ ��� �α׿� ��� �꿡 ���� �߰� ���� ����

	/*($Delete_[x/C]$)*/#define _YJ_ADD_CONTINENTMAP_POINT						// 2009.10.20 / ������ / �ʵ� �߰��� ���� ������� ���� �߰�

    /*($Delete_[S/x]$)*/#define _KR_001399_20091020_CRYSTALLIZE_EXTEND          // 2009.10.20 / �����,������ / ����ȭ ����. ����Ʈ ������ ����ȭ �߰�, ��æƮ Lv�� ���� ����ȭ ������ ���� ����. data\\ItemCrystalizeList.txt ��æƮ Lv�ʵ� �߰�

    /*($Delete_[x/C]$)*/#define _NA_20100503_MAGIC_SKILL_SAINTAID_CANCLE_STAT_IS_CHANGE // 2010.05.03 / ������ / ������Ż ��ų ����Ʈ ���̵尡 �˽��ϴ� �����̻� ���� ���� 

    #if defined(_KOREA)

    #elif defined(_CHINA)
        //#undef __KR_001355_20090624_GROUPINGSYSTEM_ALL_2    // [2010-3-16 by i4u4me]  �߱������� ����
        
    #elif defined(_JAPAN)
        #define _JP_20100114_CHANGE_ATTRIBUTE_LETECRARFE    // 2010.01.14 / ������ / ������ ����(�ʱ�ȭ ������)�� �нú� �ʱ�ȭ ��ɵ� �߰��Ѵ�.
        #define _NA_20100106_REPURCHAS_BUGFIX_2496          //2010.01.06 ������ ��Ʋ�� ���� ���� �籸���Ҷ� �籸�� �ȵǴ� ���� ����
    #endif
#endif
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.9.0.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 904
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// ���� ���� ��ȣ
    #endif
    //////////////////////////////////////////////////////////////////////////

	/*($Delete_[x/C]$)*/#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / ������ / ����Ʈ ����Ʈ �и� �۾�
	/*($Delete_[x/C]$)*/#define _GS_MOD_ITEMNAME_DRAW_BY_BV									// 2009.07.30 / �豤�� / �����۳��� ��ŷ�����������
	/*($Delete_[x/C]$)*/#define _GS_BUG_FIX_PARTY_POPUP										// 2009.07.31 / �豤�� / Ÿ��â �˾�â ���� ����	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_BAR_DRAW									// 2009.07.30 / �豤�� / Hero������(hp.mp.sd) �׸��� �ٿ��� �׸��¹�Ŀ��� Ŭ���ؼ��׸��¹�����κ���	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_IMG_NUMBER								// 2009.08.05 / �豤�� / Hero������(hp.mp.sd) ��ġ text�� �ƴ� �̹����� �׸���
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_FORWARD_TOOLTIP								// 2009.08.18 / �豤�� / npc�������� �ߴ�â ����Ʈ�� ...ó��
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_HELP_TOOLTIP								// 2009.08.19 / �豤�� / �̼Ǿ����ۼ���â �ؽ�Ʈ����...ó��

    /*($Delete_[S/C]$)*/#define __NA000000_090613_TRIGGER_LOGIC_REARRANGE__                 // 2009.06.13 / ������ / Ʈ���� ���� ���� ���� �۾�
    /*($Delete_[S/C]$)*/#define __NAM00000_090621_TRIGGER_LOGIC_REARRANGE2__                // 2009.06.21 / ������ / Ʈ���� ���� ���� ó�� ����:���� ��ȯ ó�� ����
    /*($Delete_[S/C]$)*/#define __NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__              // 2009.06.22 / ������ / ������ ���� ���ῡ ���� ���� �ɷ½� ���� ���� ����

    /*($Delete_[S/C]$)*/#define __NA000000_090712_SCRIPT_PARSING_SYSTEM_MODIFICATION        // 2009.07.12 / ������ / ��ũ��Ʈ �ε� ����
    /*($Delete_[S/C]$)*/#define __NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2       // 2009.07.18 / ������ / ��ũ��Ʈ �ε� ���� (�Ľ� �˰��� ��ü)

    /*($Delete_[S/C]$)*/#define __NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__       // 2009.07.16 / ������ / G->C ���� ��Ŷ ó���� ���� �۾� + ��ũ ���� �˼�
    /*($Delete_[S/C]$)*/#define __NA000000_090717_SHADOW_SKILL_PROBLEM_MODIFICATION__       // 2009.07.17 / ������ / ������ ��ų���� �̷��ַ� ���� �� ����
	/*($Delete_[x/C]$)*/#define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / ��â��,�̵��� / AC ���� �ٷ� ���
	/*($Delete_[S/C]$)*/#define __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK						// 2009.06.01 / ��â��, ���� ������ �������� üũ �߰�
	/*($Delete_[S/C]$)*/#define __BUG_34356_ATTR_RECOVERY_HP_BUG							// 2009.08.07 / ��â��, HPȸ�� ����
	
	/*($Delete_[x/C]$)*/#define __LTJ_RADAR_MAP_NAME_BUGFIX									// 2009.07.17 / ������ / �̴ϸ� ������ �ؽ�Ʈ �߸��� ���� ���� - (...) ����ǥ��, ���� ���
	/*($Delete_[x/C]$)*/#define __LTJ_UI_DIALOG_TEXTURE_LOAD_N_RELEASE_MOD					// 2009.07.17 / ������ / ���ʿ��� UI �ؽ�ó ���� ��� �߰�
	
    /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / ��ȣ��,������ / �䱸 ���� KILL ���� Ÿ�� �߰�.
	/*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / ��ȣ��,������ / Ž�� ����Ʈ
    /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / ��ȣ��,������ / ����Ʈ GM��ɾ� �߰�

    /*($Delete_[x/C]$)*/#define __NA_001244_20090417_ATTACK_RESIST					        // 2009.04.17 / ��ȣ�� / ������ ���� ����

    /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE                 // 2009.06.18 / ��ȣ�� / WOUNDSTATUS ��Ʈ ������ ����Ÿ�� 2�� ���� �Ǿ� �ִ� �κ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                        // 2009.06.18 / ��ȣ�� / �ѱ� PK�� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK               // 2009.07.06 / ��ȣ�� / AC ���� ���� ä���� ���� ����ǥ ������� �ʴ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG                 // 2009.07.15 / ��ȣ�� / ������ �籸�� ���ӷα�
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG             // 2009.07.15 / ��ȣ�� / ��� ��ũ���� ���� üũ ���� �ʴ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090819_GUILDMARK_REGISTER_LOG                      // 2009.08.19 / ��ȣ�� / ��� ���� ������ ��� �α�.
    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / ��ȣ�� / Itemstream�� ������� �ʴ� �κа� �ͼ� ���� �κ� �ʱ�ȭ

    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM							// 2009.07.16 / ��ȣ��,�豤�� / �ǵ� �ý���.
    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE  // 2009.07.16 / ��ȣ��,�豤�� / SD���� ����ȭ.
    /*($Delete_[x/C]$)*/#define _GS_MOD_PET_STX_POS											// 2009.07.16 / �豤�� / ����ġ ��ũ��Ʈȭ
	/*($Delete_[x/C]$)*/#define _GS_GSP_PORTAL_NAME_OFFSET									// 2009.07.21 / �豤�� / �̴ϸ�&Ǯ�� ��Ż���ؽ�Ʈ ��ġ�� ���׼���	

    /*($Delete_[S/C]$)*/#define __NA_0_20090818_SYSTEM_MEMO_SUPPORT                         // 2009.08.18 / ������ / �ý��� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_0_20090820_SSQ_FEE_REFUND_LOG                          // 2009.08.20 / ������ / Ÿ���ѻ�� ���� ���з� ���� ����� ȯ�ҽ� ���� ȹ�� �α� �߰�

    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION           // 2009.06.27 / ������ / ���� �� ���� �Һ� ���� Ÿ ���� ���� ���
    /*($Delete_[S/C]$)*/#define __NA_001359_20090619_HEIM_LOTTO_SYSTEM                      // 2009.06.19 / ������ / ���� �ζ� �ý���
    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION           // 2009.06.27 / ������ / ���� �� ���� ȹ�� ���� Ÿ ���� ���� ���
    /*($Delete_[S/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL                     //2009.06.08 / �̵���, ������ / �׷��� �ý��� ��� define
    /*($Delete_[S/C]$)*/#ifdef __KR_001355_20090624_GROUPINGSYSTEM_ALL                  
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_CW                  //2009.06.08 / ������ / �׷��� �ý��� [ Client	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_GW                  //2009.06.08 / ������ / �׷��� �ý��� [ Game	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DW                  //2009.06.08 / ������ / �׷��� �ý��� [ DBP		<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DATA                //2009.06.08 / ������ / �׷��� �ý��� [ DATA ���� ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_PARSER		        //2009.07.07 / ������ / �׷��� �ý��� [ PARSER ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
        /*($Delete_[S/C]$)*/#define __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG              //�׷� �ý��� �޼������� �˸��� �۾� define
        /*($Delete_[S/C]$)*/#define _DH_GROUPSYSTEM_CHANGE_PACKET                           //�׷� ������Ŷ CW->CG�� ����
    /*($Delete_[S/C]$)*/#endif //__KR_001355_20090624_GROUPINGSYSTEM_ALL

    /*($Delete_[S/C]$)*/#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS               //2010.04.15 / ������ / ����Ʈ ��ŷ ��� ���� ���� Mission Key => RegisterDate
    /*($Delete_[S/C]$)*/#define _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG              // 2010.04.23 / ������ / �׷� �ý��� ���� �α� �۾� 

    /*($Delete_[S/C]$)*/#define _NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION       // 2010.05.18 / ������ / �־��� ���� ���� ��ġ�� ���� ������ ���� ������ (����, Ư���Ǽ��縮 ��) �˻� ���

    #if defined(_KOREA)
        #define __NA000000_090911_NEOWIZ_USER_CCU_REPORT__ // 2009.09.11 / ������ / �׿����� ä�θ��� ���� ���� ���� �߰� �۾�
  
    #elif defined(_CHINA)
		#define __DH_HAIM_WIN_CLOSE										// 2009.09.11 / �̵��� / â�� �̵��� ������
        /*($Delete_[S/x]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                       // 2010.01.14 / ��ȣ�� / ����Ʈ ����� NPC���� �Ÿ� üũ.
        /*($Delete_[S/x]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                      // 2010.01.06 / ��ȣ�� / SD ��� ���� ���� 
        #define _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE               // 2010.01.15 / ������ / [�߱�] ���� ��� ���ǿ��� ���� ���� ���
        //#define _DH_NOAPPLY_GROUPINGSYSTEM_                             // 2010.01.19 / �̵��� / �׷� ������        
        #define _NA_0_SETITEM_FULL_OPTION_COUNT_CHANGE                  // 2010.02.01 / ��ȣ�� / ��Ʈ ������ Ǯ �ɼ� �ټ� ���� 5 -> 6.
        #define _DH_CHANGE_SHADOW_SKILL_DARK_BREAK                      // 2010.02.02 / �̵��� / ��ũ�극��ũ ��ų ����
        /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / ��ȣ�� / ����Ʈ �ݺ� ���� �ǽ� �κ� ����
        /*($Delete_[S/x]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / ��ȣ�� / ����Ʈ ���� üũ �߰�.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
        
    #elif defined(_RUSSIA)        

    #endif
#endif
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.9.0.3 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 903
/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(8)		// ���� ���� ��ȣ
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_UNUSEABLE_MAPIMG__			// 2009.06.03 // �̵��� // ���� �ʴ� �̴ϸ� ����
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_REMOVE_UI__				// 2009.05.29 // �̵��� // ������� �ʴ� UI ����
	/*($Delete_[x/C]$)*/#define _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	// 2009.05.22 // �̵��� // ��� UI ���� ���(Tooltip)

	/*($Delete_[x/C]$)*/#define _GS_GSP_PK_BLOCK_ESODE								// 2009.07.30 // �豤�� // ���ҵ帶��������  PK ����
	/*($Delete_[x/C]$)*/#define _GS_GSP_SCROLL_PROFILE								// 2009.07.30 // �豤�� // ĳ������â ������ ��ũ��ó��

	/*($Delete_[x/C]$)*/#define __KR_0_20090519_EASY_GET_LUCKYMONSTER_				//2009.05.19 / ������ / ��Ű ���� �Ҹ�ī������ HP 98%���� ����

	/*($Delete_[S/C]$)*/#define __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM			// 2009.05.11 / ��ȣ��, �̵��� / ����Ʈ �뿪�� 1500 -> 3000 ���� 
	/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / ��ȣ�� / �߱� GM �ɸ��� ģ�� �߰� ����.	(���� ����)
	/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / ��ȣ�� / �߱� ������ ���� �Ұ� ��		(���� ����)
	/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / ��ȣ�� / �߱� ��� â�� ���� �Ұ��� Ÿ���߰�.(���� ����)
	/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / ��ȣ��, �豤�� / �߱� ä���Ϸ��� ���� ��Ŷ ����.(���� ����)
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP             // 2009.07.14 / ��ȣ�� / ���������� �齺���� GM�ʰ� ���ҵ� ������ �̵��Ҽ� ������, ��ǥ �����Ҽ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / ��ȣ�� / ��� ��ũ ��ũ��Ʈ ���� ������ ������ �ȵǴ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / ��ȣ��, ������ / ��� ���� ���� ������ �߰�.
    /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / ��ȣ��, ������ / ��� ���Խ� ������ �˸� ��Ŷ ���� (��� ���, ����).

	/*($Delete_[S/C]$)*/#define __NA_1323_CHAO_TIME_DECREASE_ITEM						// 2009.04.16 / ��â��, �豤�� / ī���ð� ���� ������
	/*($Delete_[S/C]$)*/#define __NA_1324_IGTYPE_IMPROVEMENT							// 2009.04.29 / ��â�� / IGType ����
	/*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / ��â�� / DROP���� GM ��ɾ� �߰�
	/*($Delete_[S/C]$)*/#define __NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP				// 2009.05.19 / ��â�� / �ͼ� ������ ��ũ�� ����
	/*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT					// 2009.05.26 / ��â��, ������ / ����Ʈ ���� ������ ����
	/*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / ��â��, ����Ʈ �䱸 ������ ���� 3�������� �Ǵ� ����
	/*($Delete_[S/C]$)*/#define __NA_0_CASH_ITEM_RENEWAL_SUPERMASTER					// 2009.07.02 / ��â��, ������ / ĳ�þ����� ������ 1.�����ְ��� ���ֺ�

	/*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / ������ / ĳ�þ����� ������#2 ���� �� ������� �ʴ� ����

    /*($Delete_[S/C]$)*/#define __NA0000_090416_RETURN_STATE_MOB_CONTROL__                  // 2009.04.16 / ������ / ���� ���Ϳ� ���� ���� ���� ó�� ����
    /*($Delete_[S/C]$)*/#define __NA000000_090506_EVENT_TYPE_MODIFICATION__                 // 2009.05.06 / ������ / �Ϲ� ���� �÷���Ÿ�� ���� ����

	/*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                      // 2009.05.14 / ������,������ / ��Ƽ�� �ĺ� ����

	/*($Delete_[x/C]$)*/#define __001333_LTJ_QUEST_LIST_UI_REFORM					// 2009.05.22 / ������ / ����Ʈâ ����
	/*($Delete_[x/C]$)*/#define __001331_LTJ_AC_TREASUREBOX_TOOLTIP					// 2009.05.22 / ������ / AC â ����
	/*($Delete_[x/C]$)*/#define __001341_LTJ_POSSESSION_ITEM_MODIFY					// 2009.05.22 / ������ / �ͼ� ������ ���� - �ͼ����� ������ ���
	/*($Delete_[x/C]$)*/#define __001345_LTJ_QUEST_REPEAT_DISPLAY					// 2009.05.22 / ������ / �ݺ� �˽�Ʈ ���� ǥ��

	/*($Delete_[S/C]$)*/#define __NA_001304_20090521_GUILD_LOG_FIX                  // 2009.05.21 / ������ / ��� �α� ��� ���ۼ�
    /*($Delete_[x/C]$)*/#define __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT          // 2009.08.12 / ������ / ���̳ʸ� ���� �α� ����
    /*($Delete_[x/C]$)*/#define __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY           // 2009.08.13 / ������ / ���̳ʸ� ���� �α� �ε��� ���� ����

	/*($Delete_[x/C]$)*/#define _YJ_MOD_GUILD_MARK_CREATE_GRADE						//2009.07.08 / ������ / ��帶ũ�� �޼��ִ� ��� ���� ����(3Lv -> 1Lv)
	/*($Delete_[x/C]$)*/#define _YJ_ADD_GUILD_MARK_TEX								//2009.07.08 / ������ / ��帶ũ ���� �߰�

    /*($Delete_[x/C]$)*/#define _NA_0_20091020_COMPACT_COMPOSITION_LOG_         // 2009.10.20 / ������ / ���� �α� �߰� �������� ��� ������ ��� �߸� ������ ���� ��� ���� ���

    #if defined(_KOREA)
		#define __001347_LTJ_REMOVE_TEMP_INVENTORY				// 2009.05.22 / ������ / ���� â�� ���� �۾�

	#elif defined(_CHINA)
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / ��ȣ��,������ / ����Ʈ GM��ɾ� �߰�
        #define __NA_0_POSSESSION_ITEM_DROP								    // 2009.08.27 / ��â��, �ͼ� ������ ��� ����
        /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / ��ȣ�� / Itemstream�� ������� �ʴ� �κа� �ͼ� ���� �κ� �ʱ�ȭ
        #define _NA_0_20091020_COMPACT_COMPOSITION_LOG_
		#define _DH_TOOLTIP_LIMIT											// 2009.10.29 / �̵��� / ���۽� ���� ���� ������ ������ �ߴ� ������ ���� ����

	#elif defined(_JAPAN)
        /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / ��ȣ��,������ / �䱸 ���� KILL ���� Ÿ�� �߰�.
        /*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / ��ȣ��,������ / Ž�� ����Ʈ
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / ��ȣ��,������ / ����Ʈ GM��ɾ� �߰�
        #define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / ��â��,�̵��� / AC ���� �ٷ� ���
		#define _DH_AC_UI_CHANGE											// 2009/1012 / �̵��� / AC UI���� ���� �۾�
		#define _DH_HELLON_TOWER_ENTER										// 2009/0930 / �̵��� / ����� �� ���� �� ����
		#define __GS_CHANGE_TARGET_EFFECT_TYPE				                // 2009.11.09 / �豤�� / Ÿ��������ƮŸ�Ժ����Ƿ����ѹ��׼���
		#define _GS_ADD_CANDY_EVENT_WEAPON					                // 2009.11.23 / �豤�� / ĵ������Ʈ����
		#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / ������ / ����Ʈ ����Ʈ �и� �۾�

    #elif defined(_GLOBAL)
        #define _DH_TOOLTIP_LIMIT											// 2009.10.29 / �̵��� / ���۽� ���� ���� ������ ������ �ߴ� ������ ���� ����
		#undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN

    #elif defined(_RUSSIA)
        #undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	  
	#endif
#endif
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.9.0.2 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 902
/////////////////////       Protocol Version       //////////////////////
#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(6)		// ���� ���� ��ȣ
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __001251_MATERIAL_ITEM_COMPOSE_LIST					// 2009.03.05 // ������ // ���հ��� ��� ǥ��
	/*($Delete_[x/C]$)*/#define __001268_CHANGEUP_SKILL_TOOLTIP						// 2009.03.05 // ������ // ü������ ��ų �������� ���� �߰�
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_SKILL_DIALOG_TAB						// 2009.03.11 // ������ // ��ų���̾�α� �ֱ� ����� ����
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_CHAR_DIALOG_UPBUTTON_HIDE				// 2009.03.11 // ������ // ĳ���ͽ�ų���̾�α� ����Ʈ �� ��ư ����
	/*($Delete_[x/C]$)*/#define __001314_LTJ_ADD_CLOCK_OPTION_TIRED_SYSTEM			// 2009.03.12 // ������ // �Ƿε� �ý����� �ð� ��� �и�
	/*($Delete_[x/C]$)*/#define __001285_LTJ_MOD_CREATE_CHAR_DESCRIPTION			// 2009.03.13 // ������ // ĳ���� ���� ���� �ؽ�Ʈ ����
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_DROP_ITEM_NOTIFY_TIMELINE				// 2009.03.18 // ������ // ��������� ���� �˸��ý��ۿ� �ڵ� ���� �������߰�
	/*($Delete_[x/C]$)*/#define __LTJ_DISPLAY_FULL_INVENTORY_MESSAGE				// 2009.03.20 // ������ // ������ ���� �� �κ��丮�� ���� ������ ���� �� �޽��� ���
	/*($Delete_[x/C]$)*/#define __LTJ_DRAW_NEW_SERVER_ICON_SERVER_LIST				// 2009.04.01 // ������	// ��������â���� �ż��� NEW ��ư �׸���
	/*($Delete_[x/C]$)*/#define __001326_LTJ_LEVEL_COLOR_MODIFY_2ND					// 2009.04.06 // ������ // �������� ǥ�� ��� ���� 7�ܰ�->5�ܰ�� ����
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_SCROLL_BUGFIX						// 2009.04.06 // ������	// ä��â ��ũ�ѹ� �ֻ����� �ö󰡴� ���� ����
	/*($Delete_[x/C]$)*/#define __LTJ_EVENT_TIMER_MODE_MODIFY						// 2009.04.21 // ������	// �̺�Ʈ Ÿ�̸� ����߰�

	/*($Delete_[x/C]$)*/#define _YJ_RENEWAL_QUEST_UI_001265							// 09.02.05 //������ //����Ʈâ ������
		/*($Delete_[x/C]$)*/#ifdef _YJ_RENEWAL_QUEST_UI_001265
			/*($Delete_[x/C]$)*/#define __DH_004145_20090710_ITEMQUESTBUG			// 2009.07.17 //�̵��� //���� ����: ������ ����Ʈ �ٸ����� ������, �Ϸ�� ǥ��
			/*($Delete_[x/C]$)*/#define _KIKI_REMOVE_QUEST_INCOMPLETE_PACKET_LOGIC	// 2009.11.09 //������ // CG_QUEST_INCOMPLETE_SYN�� ȣ������ �ʰ�, Ŭ���̾�Ʈ���� ��ü������ ����Ʈ �̿Ϸ� ���·� �����ϵ��� ����
			/*($Delete_[x/C]$)*/#define _KIKI_CHECK_SEND_QUEST_COMPLETE_PACKET		// 2009.11.09 //������ // CG_QUEST_COMPLETE_SYN �� �ߺ� ���۵��� �ʵ��� üũ �� ������.
		/*($Delete_[x/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _YJ_CONTINENT_MAP_001277							//09.02.05 //������ //��� ���� �߰�
	/*($Delete_[S/C]$)*/#define _YJ_NAVI_SYSTEM_001277								//09.02.05 //������ //�׺� �ý��� + ������(Ŭ�� ���� �׺� ���� ó�� in ServerCode)
    /*($Delete_[S/C]$)*/#ifdef _YJ_NAVI_SYSTEM_001277
        /*($Delete_[S/C]$)*/#define __NA001277_090205_NAVI_SYSTEM_SUPPORT__         //09.03.xx //������,������/�׺� �ý��� ������ ���� �۾� (������ ���Ž� ȥ�� ���������� �߰�)
    /*($Delete_[S/C]$)*/#endif

	
	/*($Delete_[x/C]$)*/#define _YJ_TOOLTIP_ON_TEXT_001277							//09.03.09 //������ //����Ʈ ���� ���� �۾� + ������(Ŭ�� �޽��� ���� ���� ó�� in ServerCode)
	/*($Delete_[x/C]$)*/#define _YJ_REUSE_SAME_MAP_RADAR_TEXTURE					//09.03.12 //������ //
	/*($Delete_[x/C]$)*/#define _YJ_COLLECTION_SYSTEM_001283						//09.03.04 //������ //ä�� �ý���
	/*($Delete_[x/C]$)*/#define _YJ_SHOW_QUEST_EVEN_IF_UNDER_MIN_LEV				//09.04.17 //������ //[001328]���� ����Ʈ �̸� ���� �۾�

	/*($Delete_[S/C]$)*/#define __NA_0_CAN_RESURRECTION_ON_VILLAGE					// 2009.03.25 / ��â��,�豤��/ �������� ��Ȱ ����
	/*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_ETHER_BULLET_BY_GM_CMD				// 2009.01.07 / ��â��, ���׸� ���� ���� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_1241_AUTO_PICKUP_PET_OPTION					// 2009.01.14 / ��â��, �豤�� / ������ �ڵ�ȹ���ϴ� �ɼ�
	/*($Delete_[S/C]$)*/#define __NA_1247_POSSESSION_ITEM							// 2009.01.17 / ��â��, ������ / �ͼ� ������
	/*($Delete_[S/C]$)*/#define __NA_1269_CHANGE_STYLE_OPTION_BREAKER_OF_FUZ		// 2009.02.05 / ��â��, ������ �극��Ŀ 3Ÿ���� �ɼ� ���� 

	/*($Delete_[S/C]$)*/#define __NA_1288_AC_RENEWAL_FOR_TIME_LINE					// 2009.02.19 / ��â��, ������ / [Ÿ�Ӷ���]AC ������
		/*($Delete_[S/C]$)*/#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
			/*($Delete_[S/C]$)*/#define _LTJ_AC_REMOVE_CORRUPT_TEMPLE_ADD_MAIN_QUEST	// 2009.03.23 / ������, AC ������ - �̼� Ÿ���ѻ�� ����, ������ ��������Ʈ Ŭ���� Ƚ�� �߰�
		/*($Delete_[S/C]$)*/#endif
	/*($Delete_[S/C]$)*/#define __NA_1248_SSQ_ADD_LOG								// 2009.01.20 / ��â��, Ÿ���� ��� �α�
	/*($Delete_[S/C]$)*/#define __NA_1307_LOTTERY_SCRIPT_IMPROVEMENT				// 2009.03.02 / ��â��, ���� ��ũ��Ʈ ����
	/*($Delete_[S/C]$)*/#define __NA_0_ADD_GM_CMD_ABOUT_AC							// 2009.03.05 / ��â��, AC���� GM��ɾ� �߰�
	/*($Delete_[S/C]$)*/#define __NA_1313_AC_OVER_MODIFY							// 2009.03.10 / ��â��, AC �޼��� �����÷� ����
	/*($Delete_[S/C]$)*/#define __NA_0_TEMP_INVENTORY_DELETE						// 2009.03.21 / ��â��, ������
	/*($Delete_[S/C]$)*/#define __NA_0_RECALL_ALLIANCE_CAN_USE_ALONE				// 2009.03.25 / ��â��, ȥ�� ������ ���� ��� ����

	/*($Delete_[S/C]$)*/#define __NA_0_AC_FOR_REAL_TIME_REFRESH						// 2009.02.14 / ��â��, ������/ AC �ǽð� ������ ���� �۾�

	/*($Delete_[x/C]$)*/#define _DH_KOR_000000_20090522_CHARACTER_SCENE_NAME_OVERFLOW_REPAIR	// 2009.05.25 / �̵��� / ĳ���� ���ÿ��� Űū ĳ���� �̸� ��� ǥ��, NO.001346

	/*($Delete_[S/C]$)*/#define __NA_0_20080414_GUILD_WAREHOUSE_LOG_BUG							// 2009.04.14 / �̼��� / ��� â�� �α� �߸� ����� ����.

	/*($Delete_[x/C]$)*/#define __NA003373_20090402_NOT_FIND_ENGLISHNAME_BUG					// 2009.04.02 / �̼��� / ��� �ʴ�� ���� ��ҹ��ڸ� �����ϴ� ����.
	/*($Delete_[S/C]$)*/#define __NA002150_20090402_GUILD_ALLIENCE_NOT_CHK_DESTROY_WAIT_BUG		// 2009.04.02 / �̼��� / ��� �ػ� ������� ��忡�� ���� ��û�� ������ ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTACCEPT_GM_CMD								// 2009.04.07 / �̼��� / ����Ʈ ���� ���� gm ��ɾ�
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY							// 2009.04.07 / �̼��� / ����Ʈ ��ũ��Ʈ �ο� ������ �׸� 3�ʵ� ���� �ȵǴ� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20090408_GUILD_RELATION_CHECKRIGHT_BUG					// 2009.04.08 / �̼��� / ��� ���ͽ� ������ ���� ������ ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20090324_RANKINGSYSTEMCTL								// 2009.03.24 / �̼��� / ��Ʋ ���� �״� ���� �ӽ� ����
	/*($Delete_[S/C]$)*/#define __NA001254_20090109_RTTG_FUNC_ADD								// 2009.01.09 / �̼��� / �� ��� �߰�
	/*($Delete_[S/C]$)*/#define __NA000000_20090320_DELAY_ITEM_DROP_TIME						// 2009.03.20 / �̼��� / ���� ������ ���� ��� 
	/*($Delete_[S/C]$)*/#define __NA001140_20080128_SOUND_TRIGGER_ADD							// 2009.01.28 / �̼���, ������ / ���� ��� Ʈ���� �߰�
	/*($Delete_[S/C]$)*/#define __NA001282_20090206_MODIFY_QUEST_TIME							// 2009.02.06 / �̼���, ������ / [Ÿ�Ӷ���]����Ʈ �ο� �ð� ���� ����
	/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECT										// 2009.02.11 / �̼��� / [Ÿ�Ӷ���]ä�� �ý���
		/*($Delete_[S/C]$)*/#ifdef __NA001283_20090221_COLLECT
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PACKET			
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PARSER
			/*($Delete_[S/C]$)*/#define __NA001283_20090223_COLLECTION_CLASS
			/*($Delete_[S/C]$)*/#define __NA001283_20090225_COLLECTION_USE
			/*($Delete_[S/C]$)*/#define __NA001283_20090227_COLLECTION_LIST
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_GM
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_MAPOBJECTID
		/*($Delete_[S/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _GS_MOD_PET_GAUGE_POS_1242							// 2009.03.103 �豤�� / �� ���� UI ��ġ ����
	/*($Delete_[x/C]$)*/#define __GS_ADD_REMOVE_TUTORIAL_MSG_1315					// 2009.03.13  �豤�� / Ʃ�丮�� �޽�������
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ACTIVE_FOR_CLASS					// 2009.03.18  �豤�� / ����Ʈ����â���� �ڽ�Ŭ�����´¾����ۼ���Ȱ��ȭ 
	/*($Delete_[x/C]$)*/#define __GS_ADD_PANDORA_BOX_NAME							// 2009.03.18  �豤�� / �ǵ������ ���� �����۰������ϰ� ���� 
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ITEM_LIST_COLOR						// 2009.03.19  �豤�� / ����Ʈ����â������ ����ÿ� �ӹ��� ���ٸ��Ժ���
	/*($Delete_[x/C]$)*/#define __GS_ADD_SCREEN_OPTION_SCROLL						// 2009.03.22  �豤�� / �׷���â�ػ� ���������޺��ڽ� ���콺�ٽ�ũ���߰�
	/*($Delete_[x/C]$)*/#define __GS_MOD_WAYPOINT_LEVEL_CHECK						// 2009.03.25  �豤�� / ��������Ʈ ����ǥ�ù� ����üũ
	/*($Delete_[x/C]$)*/#define __CN_000097_GM_HIDE_TAGETING_BLOCK					// 2009.05.13  �豤�� / GMĳ��������½� Ÿ���úҰ�

    /*($Delete_[S/C]$)*/#define __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__         // 2009.02.09 / ������,�豤�� / [Ÿ�Ӷ���] ����Ʈ �� �ʵ忡���� �̼� + ��Ʋ�� ���� ����
    /*($Delete_[S/C]$)*/#define __NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__   // 2009.02.26 / ������,�豤�� / ��Ż�� ���� ����|�ʵ� ���ͽ� ������ ������ ���� ����
    /*($Delete_[S/C]$)*/#define __NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__              // 2009.05.19 / ������,�豤�� / ���ҵ� ���� (�ʵ�Ÿ�Կ��� ���� Ÿ������ ����)


    /*($Delete_[x/C]$)*/#define __NA_4710_20090819_REPAIREMONEY_FIX                     // 2009.08.24 / ������ / ���� �ݾ��� Ŭ���̾�Ʈ�� �������� �Ҽ��� ������̿����� ���̹���
    /*($Delete_[x/C]$)*/#define __NA_34726_20090421_ARMDRAGON_TRANSFORM_BUG_FIX //2009.04.21 / ������ / �ϵ巹�� Ʈ������ ������ ���� ����� ĳ�� ���� ������� ���� ����


    /*($Delete_[S/C]$)*/#define __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__  // 2009.03.10 / ������ / Object�� ���� ���Ž� ��� ���� ���� �ذ� �۾�
    /*($Delete_[S/C]$)*/#define __NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__   // 2009.03.16 / ������ / [Ÿ�Ӷ���] ��Ʋ�� ���� ���� + �κ񿡼��� �ش� �������� �ɸ�����...
    /*($Delete_[S/C]$)*/#define __NA001319_SAME_CLASS_LIMIT_REMOVE__                        // 2009.03.16 / ������ / ���� Ŭ���� ����(5��) ����
    /*($Delete_[S/C]$)*/#define __NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__          // 2009.03.20 / ������ / 0902 ���� ��û���׵� (����Ʈ ����)
    /*($Delete_[S/C]$)*/#define __NA000000_090327_MONSTER_HELP_FILTER__                     // 2009.03.27 / ������ / Grade 9,5�� ���� Help�� �������� �ʵ��� ó��
    /*($Delete_[S/C]$)*/#define __NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__           // 2009.04.07 / ������ / ��Ż �̼� ����� '�����ְ��� ���ֺ�'�� ���� ��Ʋ���� �����Ǵ� �Ӽ� ����ǵ���...
    /*($Delete_[S/C]$)*/#define __NA_1317_20090316_SKILL_DAMAGE_FORMULA_MODIFY		// 2009.03.16 / ���ؼ� / ��ų������ ���� ����
	/*($Delete_[S/C]$)*/#define __NA_1320_20090316_STATUS_RATIO_FORMULA_MODIFY		// 2009.03.16 / ���ؼ� / �̻���� Ȯ�� ���� ����
	//#define __NA_0_20090317_ATTACKPOWER_FORMULA_MODIFY			// 2009.03.17 / ���ؼ� / ��Ű�� ���ݷ� ���� ���� (���� ó��)
	/*($Delete_[S/C]$)*/#define __NA_0_20090318_AGGRO_ABILITY_MODIFY				// 2009.03.18 / ���ؼ� / ��׷� �����Ƽ ����
    

  //------------------------------------------------------------------------------------------------
  // (BUG-FIX)
  /*($Delete_[S/C]$)*/#define __BUG_35162_COMPOSE_BUG_BY_FULL_IVEN					// 2009.06.29 / ��â��, �κ��丮�� ������ ���¿��� �������� �� ����

	#ifdef _KOREA
        #define __KR000000_090128_CHANNELING__                              // 2009.01.28 / ������,������ / ä�θ� ���� ���� �۾�
	#elif defined(_JAPAN) 
		#define __LTJ_JP_TEMP_INVEN_MOD	
		#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS						// 2009.08.14 / ������ / Į������ ���� ���밡�� Ŭ���� ���� �߰�
		#define __LTJ_DISABLE_HUNTINGZONE_BUTTON_JP						// 2009.07.30 / ������ / ��Ʋ�� �κ� ���ð��� UI ��Ȱ��ȭ
		#define __LTJ_HIDE_PET_NAME_JP									// 2009.07.28 / ������ / UI �����ɽ� ���� ������ �� �̸� ����
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / ��â�� / DROP���� GM ��ɾ� �߰�		
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / ��ȣ�� / ��� ��ũ���� ���� üũ ���� �ʴ� ����.(�Ϻ�����)
        /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK           // 2009.07.06 / ��ȣ�� / AC ���� ���� ä���� ���� ����ǥ ������� �ʴ� ����.

		/*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION					// 2009.07.09 / ��ȣ��,������ / PK Ÿ�� ���� �ɼ�.

        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / ������ / ĳ�þ����� ������#2 ���� �� ������� �ʴ� ����

	#elif defined(_CHINA)	
		/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / �̼��� / �߱� GM �ɸ��� ģ�� �߰� ����.
		/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / �̼��� / �߱� ������ ���� �Ұ� ��
		/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / �̼��� / �߱� ��� â�� ���� �Ұ��� Ÿ���߰�.
		/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / �̼���, �豤�� / �߱� ä���Ϸ��� ���� ��Ŷ ����.
		#define __CN_000005_20090522_WAREHOUSE_ISACCEPTABLE_X			// 2009.05.22 / �̼��� / �߱� â�� ����� ���ĺ� x ���.
        #define __CN_0_20090617_PRE_CHAO_USE_FLAG	                    // 2009.06.16 / �̼��� / �߱� ���������� ����ī���� �����̿� ����.
        /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE             // 2009.06.18 / �̼��� / WOUNDSTATUS ��Ʈ ������ ����Ÿ�� 2�� ���� �Ǿ� �ִ� �κ� ����.
	    /*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / ��â��, ����Ʈ �䱸 ������ ���� 3�������� �Ǵ� ����
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG             // 2009.07.15 / ��ȣ�� / ������ �籸�� ���ӷα�

        /*($Delete_[x/C]$)*/#define __CN_0_20090706_CHINA_VISTA_CHARNAME                 	// 2009.07.06 / ��ȣ�� / �߱� ��Ÿ �׽�Ʈ

#elif defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / ��ȣ�� / ��� ��ũ���� ���� üũ ���� �ʴ� ����.
        /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / ��ȣ�� / ��� ��ũ ��ũ��Ʈ ���� ������ ������ �ȵǴ� ����.
        /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / ��ȣ��, ������ / ��� ���� ���� ������ �߰�.
        /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / ��ȣ�� / ��� ���Խ� ������ �˸� ��Ŷ ���� (��� ���, ����).
		#define _YJ_MOD_GUILD_MARK_CREATE_GRADE							// 2009.07.08 / ������ / ��帶ũ�� �޼��ִ� ��� ����
        #define _US_ACCOUNT_LENTH_UNCHECKED_                            // 2009.08.28 / ������ / ���� ���� üũ���� ����(�Ϲ� ���� ����)
        #define _US_SHOP_GIFT_ERRORCONTROL_ADD                          // 2009.11.11 / ������,�豤�� / �Ϲ� �����ϱ� ����, ���� ó�� �߰� 
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / ��â�� / DROP���� GM ��ɾ� �߰�  / 2009.09.16 /������ �Ϲ� �׽�Ʈ�� �߰�
        /*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                  // 2009.05.14 / ������,������ / ��Ƽ�� �ĺ� ���� : 2009.11.10 ü�� ������ ���� ���� ���� ������
        /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                    // 2009.06.18 / ��ȣ�� / �ѱ� PK�� ����. : 2009.12.02 / ������ / pk�� ���� �Ϲ� �ݿ�
/*  */#if defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / ������ / ĳ�þ����� ������#2 ���� �� ������� �ʴ� ����
/*  */#endif        
    #endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.9.0.1 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 901
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(5)		// ���� ���� ��ȣ
	#endif	
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define _GS_MOD_NANUM_FONT_1239								// 2009.02.19 //�豤�� // ������Ʈ����
	/*($Delete_[x/C]$)*/#define __GS_MOD_HIDE_PET_S0032								// 2009.01.22 //�豤�� // ���������̵���굵���̵�+��ȯ����Ÿ���ú���
	/*($Delete_[x/C]$)*/#define	_GS_ADD_HIDE_ANI_0029								// 2009.01.06 //�豤�� // ���������̵彺ų���� ����߰�

	/*($Delete_[S/C]$)*/#define __NA_S00015_20080828_NEW_HOPE_SHADOW				// 2008.08.28 / �����, �豤��, ������ / ������ ( ������ ���� ��� �۾��� �� �������� ���� ��. )
	/*($Delete_[S/C]$)*/#define __S00013_SHADOW_BASE_WORK							// 2008.08.11 / ������ / ������ ���̽��۾�
	/*($Delete_[S/C]$)*/#define __NA_001196_20081110_RECOVERY_ADD_OPTION			// 2008.11.10 / ����� / ��Ŀ���� ��ų ����.( ��Ŀ���� æ�� )
	/*($Delete_[S/C]$)*/#define __NA_001206_20081112_DUPEQUIP_GROUP					// 2008.11.12 / ����� / �����ߺ��׷� �ʵ� �߰�.
	/*($Delete_[S/C]$)*/#define __NA_0S0023_20081114_SPAWN_MONSTER_QUEST			// 2008.11.14 / �����, ������ / NPC ==> ���� ���� ����Ʈ
	/*($Delete_[S/C]$)*/#define __NA_S00028_SHADOW_ADD_WORK							// 2008.12.17 / ����� / ������ ���̵�, �ҿ���Ʈ��, �нú�, ��ü���� �߰� ����
	/*($Delete_[S/C]$)*/#define __NA_000000_20090211_SUMMON_HP_BUG_FIX				// 2009.02.11 / �����, �豤�� / ������ ��ȯü HP ���� ����.
	/*($Delete_[S/C]$)*/#define __NA_001270_20090209_GUILD_PENALTY_FIX				// 2009.02.09 / ����� / ��� �г�Ƽ ����.
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20090910_WOPS2610						// 2009.02.10 / ����� / �츣�޽����� ���� ���� ���Ͽɼ��� ������� ���� ���� ����.


	/*($Delete_[S/C]$)*/#define __NA_1165_ETHER_DEVICE_SAVE_DB						// 2008.10.26 / ��â��, �豤�� / ���׸� ������ġ ���� DB����
	/*($Delete_[S/C]$)*/#define __NA_1131_PET_NAME_SETTING							// 2008.11.18 / ��â��, �豤�� / �� �̸� ����, �̸�Ƽ��
	/*($Delete_[S/C]$)*/#define __NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT			// 2008.09.23 / ��â�� / ���� �κ��� ���Ե� ������ ����ϵ���...
	/*($Delete_[S/C]$)*/#define __NA_1234_LOTTERY_ITEM_RESULT_BUG_FIX				// 2008.12.09 / ��â��, �豤�� / ���� ������ ���� �޼���
	/*($Delete_[S/C]$)*/#define __NA_1168_ADD_CONTENTS_ADD_LOG						// 2008.11.06 / ��â�� / �α� �߰� �۾�
	/*($Delete_[S/C]$)*/#define __NA_1233_2009_NEW_YEARS_DAY_EVENT					// 2008.12.30 / ��â��, �豤�� / 2009�� ���� �̺�Ʈ
	/*($Delete_[S/C]$)*/#define __NA_1260_CHANGE_LEVEL_FOR_STAT_INIT				// 2009.01.12 / ��â��, ������ / ���� ���� �ʱ�ȭ ���� ����
	/*($Delete_[S/C]$)*/#define __NA_01240_GM_CMD_ADD_SERVER_TIME					// 2009.01.09 / ��â��, ������ / GM��ɾ� �߰�, �����ð�, �ִ� ������ ���	
	/*($Delete_[S/C]$)*/#define __NA_0_SHADOW_HIDE_BUG_FIX							// 2009.02.20 / ��â��, �豤�� /������ ���̵� ���� ����

	/*($Delete_[S/C]$)*/#define _YJ_SHADOW_S00013
	/*($Delete_[S/C]$)*/#ifdef _YJ_SHADOW_S00013
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_SKILL_S00013
		/*($Delete_[S/C]$)*/#define _GS_ADD_WEAPON_TYPE
		/*($Delete_[S/C]$)*/#define _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212 // 2008.11.25 / ������ / ��ų ���� ����(��ų����� �ڵ����� ���� ������)���� ��ũ��Ʈ �ʵ� �߰� 
		/*($Delete_[S/C]$)*/#define _K2P_ADD_CREATE_SHADOW_S00018					// 2008.11.21 / ���ǥ / ������ ���� ����
		/*($Delete_[S/C]$)*/#define _K2P_NEW_SELECT_MAP_							// 2008.11.21 / ���ǥ / ���ο� ��� ���� ����
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_CRITICAL_DAMAGE_EFFECT					//09.01.05 //������ //������ ĳ���� ��� �ٸ� ũ��Ƽ�� ����Ʈ ���
	/*($Delete_[S/C]$)*/#endif //_YJ_SHADOW_S00013

	/*($Delete_[x/C]$)*/#define _YJ_MOD_DEATH_ANI									//08.12.09 //������ //����Ÿ�Կ� ���� �״� ��� �޶����� ����
	/*($Delete_[x/C]$)*/#define _YJ_MOD_DAMAGE_EFFECT_POS_001188					//08.12.03 //������ //������ ����Ʈ ��ġ ����(Ÿ�� �ٿ�������� �߰������� ����������)
	/*($Delete_[x/C]$)*/#define _YJ_SHADOW_QUEST_S00023								//08.12.03 //������ //������ ����Ʈ
	/*($Delete_[C]$)*/#define _YJ_CHANGE_GUILD_NAME_001201						    //08.11.18 //������ //���� ���� ��� �߰�

    //! 2011.5.3 / i4u4me / ĳ����(���﷡����)���� Ÿ�ݽ� ������ ���� �ʰ� ��û
    //! ���� �۾� undef ��;
	//#define _YJ_MOD_BLOW_MOTION_001188						//08.12.03 //������ //Ÿ�ݰ� ����

	/*($Delete_[C]$)*/#define _YJ_RETIRE_EXECUTED_TRIGGER_OBJECT_001014			    //08.12.16 //������ //Ʈ���� �۵��� ���콺 Ŀ�� ����
	/*($Delete_[C]$)*/#define _YJ_SPAWN_MONSTER_WITH_EFFECT_001250				    //08.12.22 //������ //���ͼ�ȯ�� ����Ʈ ���
	/*($Delete_[C]$)*/#define _YJ_PRELOADING_MONSTER								//09.01.05 //������ //Ʈ���� ���� ������ �ȹ����̰� ����
	/*($Delete_[C]$)*/#define _YJ_RENEWAL_KNOCKBACK_MOTION_001256					//09.01.07 //������ //�˹� ��� ������
	/*($Delete_[C]$)*/#define _YJ_ETC_4_PERFORMANCE								    //09.05.22 //������ //Ŭ���̾�Ʈ ���� ����
	/*($Delete_[C]$)*/#define _YJ_TEST_FREE_ALL_CHCHED_DATA_WHEN_MAP_LOADING		//09.05.22 //������ //�̼�,�ʵ� ���Խ� ���� ���ε�
	/*($Delete_[C]$)*/#define _KIKI_MODIFY_RESOURCE_MANAGER						    // 2009.06.09	/ ������ /	ResourceManager ���� ���� ���� - ������⸮��Ʈ����� 2�п� �ѹ��� üũ (30���̻� �������� ���� ���� ����)

	/*($Delete_[C]$)*/#define	_K2P_SORT_TEXT_UI_001210							// 2008.11.21 / ���ǥ / �ؽ�Ʈ ���� UI ���� �۾�
	/*($Delete_[C]$)*/#define _K2P_CLOSE_WINDOW_AFTER_RETIRVE_001198				// 2008.11.21 / ���ǥ / ���� �� ��� ����[1] / ��Ȱ�� �ڵ����� �˾�â �ݱ�
	/*($Delete_[C]$)*/#define _K2P_WATER_WAVE_001184								// 2008.12.17 / ���ǥ / ���� �ܻ�ȿ�� �߰�
	/*($Delete_[C]$)*/#define _K2P_AFTER_JUMP_WATER_EFFECT_001182					// 2008.12.17 / ���ǥ / ���� ���� ����Ʈ ����

	/*($Delete_[C]$)*/#define _K2P_ITEM_DISPLAY_OPTION_001232					    // 2008.12.17 / ���ǥ / ��� �� ���� ������ �̸� ǥ��
	/*($Delete_[C]$)*/#ifdef _K2P_ITEM_DISPLAY_OPTION_001232
		/*($Delete_[C]$)*/#define __001232_ITEM_DISPLAY_OPTION__REFORM			    // 2008.12.13 / ������ / ��� �� ���� ������ �̸� ǥ�� �� ���� ���� �۾�
	/*($Delete_[C]$)*/#endif
		
	/*($Delete_[C]$)*/#define _K2P_MONSTER_IDLE_ANI_001246						    // 2009.01.12 / ���ǥ / ĳ���� ����â ���ٸ��� �ִ��ڵ� ����

	/*($Delete_[x/C]$)*/#define __001193__20081107_LEVEL_COLOR_MODIFY__				// 2008.11.21 / ������ / ���� ���� ǥ�� ����

	/*($Delete_[C]$)*/#define  __001199__20081117_SYSTEM_REFORM					        // 2008.11.21 / ������ / ���� �� ��� ����[2]
	/*($Delete_[C]$)*/#ifdef __001199__20081117_SYSTEM_REFORM
		/*($Delete_[C]$)*/#define __001199_CHAT_WHEEL_SCROLL							// ä��â �� ��ũ�� ����
		/*($Delete_[C]$)*/#define __001199_GUILD_DUTY_COLOR							    // ��� ���� �ο�  ���� ���� �� �÷� ����
		/*($Delete_[C]$)*/#define __001199_ITEM_DROP_MOUSEOVER_ALT					    // ALT ��۽� ���콺 ��ŷ�Ǵ� �����۸� �� ����
		/*($Delete_[C]$)*/#define __001199_DELETE_INVITE_BLOCK_REVENGE				    // ����, ������ â���� ��Ƽ�ʴ� ��ư ����
		/*($Delete_[C]$)*/#define __001199_BATTLEZONE_LOBBY_CANCEL_BUTTON				// ��Ʋ�� �κ񿡼� Ready ��ư Ŭ���� Cancel�� ����
	/*($Delete_[C]$)*/#endif
	
    /*($Delete_[C]$)*/#define __001225__CHECK_INVENTORY_LOCK_USE_ITEM				    // 2008.12.01 / ������ / �κ��丮 ��ݻ��� ���¡ ����ó��

	/*($Delete_[C]$)*/#define __001240_GM_COMMAND_INVENTORY_ITEM_LISTUP			        // 2008.01.07 / ������ / GM��ɾ� - �κ��丮 �� â�� ������ ��� �������
	
	/*($Delete_[S/C]$)*/#define __NA001284_20090205_MODIFY_SUMMONED_ATTR				// 2009.02.05 / �̼��� / ���� ��ȯü ���� ����
	/*($Delete_[S/C]$)*/#define _NA_001231_20081216_ADD_OPTION_KIND						// 2008.12.31 / �̼��� / PVP ������ ���� �ɼ� �߰�
	/*($Delete_[x/C]$)*/#define __NA003322_20090217_NOT_FIND_ENGLISHNAME_BUG			// 2009.02.17 / �̼��� / ���� �ɸ��� �̸��� ã�� ���ϴ� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION	// 2009.02.02 / �̼��� / �� ������ �ߺ� ���� ����.
	/*($Delete_[S/C]$)*/#define __NA000000_20081106_GUILD_NAME_CHANGE					// 2008.11.06 / �̼���, ������ / ���� ����	(�۾���û��[1201])
	/*($Delete_[S/C]$)*/#define __NA_003197_20090114_CHARACTER_POS_SYNC_BUG_FIX			// 2009.01.14 / �̼��� / ��ų ��� ����� �ɸ��� ��ġ ����ȭ
	/*($Delete_[S/C]$)*/#define __NA_001253_20090107_MAX_CHARACTER_MODIFY				// 2009.01.07 / �̼��� / �ɸ��� ���� MAX ġ ����

	/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT						// 2008.10.24 / �̼���, ������ / ���� ���� ����Ʈ
	/*($Delete_[S/C]$)*/#ifdef __NA001164_20081024_CHAR_WAY_POINT		
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_QUERY
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_FUNC
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_PACKET
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_FUNC
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_ITEM
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_GM_CMD
        /*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_PORTAL
	/*($Delete_[S/C]$)*/#endif

	/*($Delete_[S/C]$)*/#define __NA_0_20080930_AUTH_REFACTORING					// 2008.09.30 / ���ؼ� / ���� �����丵 �߿� �ʿ��� ������ �ҽ� ����
	/*($Delete_[S/C]$)*/#define __NA_0_20081029_PARTY_REFACTORING					// 2008.10.29 / ���ؼ�, ���ǥ / ��Ƽ ���̺귯�� AgentServer ���� �� �����丵
	/*($Delete_[S/C]$)*/#define __NA_0_20081124_PARTY_REFACTORING_GAMESERVER		// 2008.11.24 / ���ؼ� / ��Ƽ ���̺귯�� GameServer ����

    /*($Delete_[S/C]$)*/#define __NA001230_SSQ_UPDATE_ADD_EXP_FIELD_SUPPORT__		// 2008.12.04 / ������ / Ÿ���� ��� �߰� ����ġ ���� ���� �߰� ó��
    /*($Delete_[S/C]$)*/#define __NA000000_090224_PARTY_RETURN_PROBLEM_CORRECTION__ // 2009.02.24 / ������ / ���� �ε��� ���� �ȵǴ� ���� ����
    /*($Delete_[x/C]$)*/#define __NA_0_20081222_BUGFIX_INITPOS_OF_MAPOBJ_EFFECT       // 2008.12.23 / ����� / �� ������Ʈ ����Ʈ �ʱ� ��ġ ���� ����

	#ifdef _KOREA
		
	#elif defined(_CHINA) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / ���ǥ / ������ ���� ����
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_JAPAN) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / ���ǥ / ������ ���� ����
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_GLOBAL)
		#ifdef  __CN_CREATE_BLOCK_SHADOW								// 2009.07.23 / �豤�� / ������ ���� ����
			#undef _K2P_ADD_CREATE_SHADOW_S00018						// 2008.11.21 / ���ǥ / ������ ���� ����
		#endif//__CN_CREATE_BLOCK_SHADOW
	#endif

#endif

#ifdef __NA_0_20080930_AUTH_REFACTORING
    #include "./SunAuth/AuthDefine.Build.h"
#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.1.8 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 818
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(4)		// ���� ���� ��ȣ
	#endif	
//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA001226_INSTANCE_DUNGEON_EXTENTION__				// 2008.12.01 / ������, �豤�� / �ν��Ͻ� ���� �̺�Ʈ �� ��� �߰�
	/*($Delete_[S/C]$)*/#define __NA_000000_20081202_ADD_EXP_ACTION_2				// 2008.12.02 / ����� / ����ġ �ֱ� ==> ��Ƽ�� �ش� �ʿ� �ִ� ��� �������� ����ġ �ֱ�� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20081209_ONLEVELUP_BUG_FIX					// 2008.12.09 / ���ؼ�/ ����ġ�� ���� �޾Ƶ� �Ѳ����� �������� ���� �ʰ� �ϳ��� �Ǵ� ���� ����
	#ifdef _JAPAN
		#define __090512_LTJ_ETHER_CHARGE_JP_BUGFIX				// 2009.05.12 / ������ / ���׸� �������¿��� �������� �̳� �ʵ忡�� ��� �� ������ ��ȯ�� �� �������� �̻� ���� ����		
	#endif
#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.1.7 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 817
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	//////////////////////////////////////////////////////////////////////////

	
	/*($Delete_[C]$)*/#define _K2P_NEW_RANKING_UI													// 2008.09.30 / ���ǥ / ��ŷ ������ ������
	/*($Delete_[C]$)*/#define _GS_MOD_PET_BASEOPTION_EXTEND_1146									// 2008.08.13 / �豤�� / ��⺻�ɼ�Ȯ������
	/*($Delete_[C]$)*/#define	_GS_BUG_FIX_PVP_VIEWPORT											// 2008.12.21 //�豤�� // ��������Ʈ������������ ����������������

	/*($Delete_[S/C]$)*/#define	__NA_000839_20080129_PVP_OPTION										// 2008.01.29 / �����, ������ / PVP���� �ɼ� ��� �߰�.	  ==> 2 1,2���� �ؿܿ��� �ø�.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20071120_FATIGUE_AC_RANKING								// 2007.11.20 / �����, �豤�� / AC���� �� �̼Ƿ�ŷ ���� �Ƿε� ����. ==> 1
	/*($Delete_[S/C]$)*/#define __NA_000000_20070704_FATIGUE_REWARD									// 2007. 07. 04 / �����, ���ǥ / �Ƿε��� ������� 
	/*($Delete_[S/C]$)*/#define __NA_000902_20080226_MESSAGE_RESTRICTION							// 2008.03.10 / �����, lammy / ��Ƽ�� �̿ܿ� �޽��� ����

	/*($Delete_[S/C]$)*/#define __NA001145_20080909_AIOLT_TRIGGERS_ADD								// 2008.09.08 / �̼���, ������ / ���̿�Ʈ ���ι̼ǿ� Ʈ���� �߰� 
	/*($Delete_[S/C]$)*/#define __NA001173_20080924_SUMMON_NONPVP_MODE_ADD							// 2008.09.24 / �̼���, �豤�� / ��ȯ�� NONPVP��� �߰�
    /*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM											// 2008.10.28 / ��â��, �豤�� / ����Ʈ 1.5��

	/*($Delete_[S/C]$)*/#define __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH			// 2008.12.10 //�̼��� // GM��ũ��Ʈ�߰�
	/*($Delete_[S/C]$)*/#define	__NA002410_20081006_TROUBLED_PARTS_RULE_ADD							// 2008.10.06 / �̼��� / pvp �� �߰� (������������ ��� ���Ͱ��� üũ)
	/*($Delete_[S/C]$)*/#define	__NA_001142_20080806_TRIGGER_ADD									// 2008. 8. 6 / �����, ������ / �ű� Ʈ���� �߰�.
	/*($Delete_[S/C]$)*/#define __NA001139_20080922_IGNORE_RESERVEHP_STATE_ADD						// 2008.09.22 / �̼��� / ������ ������ HP�������� ���� �߰�
	/*($Delete_[S/C]$)*/#define __NA002494_20081006_CHANGEUP_SAINTAID_CURE_BUG_FIX					// 2008.10.06 / �̼��� / ����Ʈ���̵� ��ų ���� ���� ���� �ȵǴ� ����.   
	/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM											// 2008.09.13 / ��â��,�豤�� / ��Ű ����Ʈ ������ ���� ( ����Ʈ1.5�� )
	/*($Delete_[S/C]$)*/#define __BUG_2542_PET_INTIMACY_BUG											// 2008.10.14 / ��â�� / ģ�е� ���� ����	
	/*($Delete_[S/C]$)*/#define __BUG_2536_RECOVER_HP_BUG											// 2008.10.16 / ��â�� / HPȸ�� ����
	/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG								// 2008.10.21 / ��â�� / ������ �� ���� ���� ���ѿ� �ɸ��� �ʴ� ����
	/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG							// 2008.10.21 / ��â�� / ��ž ��Ƽ�� �� �ε��� �߻��ϴ� ����
	/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG										// 2008.10.22 / ��â�� / ��ž�� ������ �ƴѰ����� ���� �� ����
	/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG											// 2008.10.30 / ��â�� / ���Ǹ������� ���� ����ο��� ȿ�� ����Ǵ� ����
	/*($Delete_[S/C]$)*/#define __CN_1298_BASESTATE_INFO_AND_ENCHANT_ADD							// 2009.02.22 / ��â��, basestate_info, enchant ���� �߰�
	/*($Delete_[S/C]$)*/#define _NA_0_20081015_EXP_LEVEL_RATIO										// 2008.10.15 / ���ؼ� / ����ġ���� ����. ���������� ����.
	/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY						// 2008.10.24 / ���ؼ� / ���������� ���� ����. ���� �����Ӽ����� ���� �ɼ� ���� ����!
	/*($Delete_[S/C]$)*/#define _NA_0_20081124_ADDEXP_SENDPACKET_BUG_FIX							// 2008.11.24 / ���ؼ� / ����ġ ���� ��Ŷ �Ⱥ����� ���� ����
	/*($Delete_[S/C]$)*/#define __NA000000_080918_GAMEFIELD_LIST_MANAGE_SYNCHRONIZATION_FIX__		// 2008.09.17 / ������ / GameField::Enter, LeaveField ����Ʈ ���� ����ȭ ����
    /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED       // 2008.07.04 / ����� / �巡�� ���� ���¿��� �����Ǿ� �ִ� �Ⱓ�� ������ �Ⱓ ����� �̻� ���� ����

	//����������������������������������������������������������������������������������
	/*($Delete_[S/C]$)*/#define __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__					// 2008.07.07 / ������, ���ǥ / [Ÿ���� ���] �ý��� ���� - (���� ��ũ�ε��� ���ݾ� �̰����� ������Ű���� �ϰ���)
	#ifdef __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__
		/*($Delete_[S/C]$)*/#define __NA000000_080204_TASK_SSQ_REBUILDING_FOR_BASE_FRAME__			// 2008.02.04 / ������ / (Zone) Ÿ���� ��� �۾��� ���� ���� ���� ������ ���� �۾� (���� ���� ����)
		/*($Delete_[S/C]$)*/#define __NA000000_080210_TASK_SSQ_REBUILDING_FOR_BASE_FRAME_PARTY__	// 2008.02.10 / ������ / (Party) Ÿ���� ��� �۾��� ���� ���� ���� ������ ���� �۾� (���� ���� ����)
		/*($Delete_[S/C]$)*/#define __NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__			// 2008.02.04 / ������, ������ / [Ÿ���� ���] �ý��� ����
		/*($Delete_[S/C]$)*/#define __NA001144_080828_HIGH_RESOLUTION_SECTOR_LOAD_BALANCE_POS_SYNC__// 2008.08.28 / ������ / Sync(position)���� Packet Load Balancing
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SSQ_REWARD_PROCESS__						// 2008.09.04 / ������ / ����,���� �� ������ ó�� (��κ��� �ٸ� ��ũ�ο� �ɸ�������...�۾� ���� ���� ����)
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST__					// 2008.09.07 / ������ / �ش� ��¥ ���� �۾���
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_STATUS__			// 2008.09.14 / ������ / ���� ���� ���� �۾���
		/*($Delete_[S/C]$)*/#define __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__				// 2008.09.28 / ������ / ��Ÿ ���� �۾�
		/*($Delete_[S/C]$)*/#define __NA001166_080922_TASK_SSQ_HUMANOID_AI_01__						// 2008.09.22 / ������ / Humanoid AI ���� 1��
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_COLLECT__			// 2008.09.07 / ������ / ��ũ�� ���տ�
		/*($Delete_[S/C]$)*/#define __NA000000_081024_PARTY_MANAGEMENT_EASIABLITY				// 2008.10.24 / ������ / ��Ƽ ���� ������ �߻� ����(������) �ذ�
		/*($Delete_[S/C]$)*/#define __NA001205_081104_MONSTER_SIEGE_EVENT_IMPL__				// 2008.11.04 / ������ / ���� ���� �̺�Ʈ
		/*($Delete_[S/C]$)*/#define __NA001094_081104_LOGIC_CHANGES_FOR_0817VER__				// 2008.11.04 / ������ / 0817 ���������� ������ �Ǵ� ��û�� ���� ���� ���� �۾�
		/*($Delete_[S/C]$)*/#define __NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__		// 2008.11.10 / ������ / ���̺꿡�� Ư�� ������ ���� �׽�Ʈ ��� �߰�(���� �ε� ������ �ӽ� �ϵ��ڵ� �� �߰�)
		/*($Delete_[S/C]$)*/#define __NA000000_081124_DEAD_AND_LEAVE_PACKET_SYNCHRONIZATION__	// 2008.11.24 / ������, ���ǥ / 0.8.1.7 ���� ��Ŷ ����ȭ ���� ���� ������ �߻��� �� ������, ���� DEAD, LEAVE�� ���� ���� ����
        /*($Delete_[S/C]$)*/#define __NA000000_081223_LOGIC_CHANGES_FOR_0817VER_SYNC__  // 2008.12.23 / ������ / 0817 ���� ���� ����ȭ ���� �������� ���� �� ����

		/*($Delete_[S/C]$)*/#define __NA001187_081015_SSQ_OBSERVER_MODE__							// 2008.10.15 / ������, ���ǥ / Ÿ���� ��� ������� ����
		/*($Delete_[S/C]$)*/#define __NA001189_081015_SSQ_TICKET_INFO_CTRL__						// 2008.10.15 / ������, ���ǥ / Ÿ���� ��� ����� ����ð� ǥ��
		/*($Delete_[S/C]$)*/#define __NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__				// 2008.09.22 / ������, ���ǥ / ���� ���� ó�� ����
		/*($Delete_[S/C]$)*/#define __NA001144_080806_HIGH_RESOLUTION_SECTOR__						// 2008.08.06 / ������, ���ǥ / Sector Rebuilding + High Resolution Sector�� ���� Ŭ���̾�Ʈ �� ���� �۾�
		/*($Delete_[S/C]$)*/#define __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__		// 2008.02.15 / ������, ���ǥ / Ÿ���� ��� �۾��� ���� C/S ���� ��� �۾�
        /*($Delete_[S/C]$)*/#define __NA000000_090223_VARIABLE_SIZE_GRID_SYNC_CORRECTION__      // 2009.02.23 / ������,������ / ����ũ�� ��ó���� ���� ����ȭ�� ������ �ִ� �κ� ����
	#endif
	//����������������������������������������������������������������������������������

	
	

//	#define _NA_0_20080612_WORLDSERVER_REFACTORING		 // ����ö/�����丵 ���弭�� ������

	#ifdef _KOREA
		#define			_NA_20081121_ABUMAN_NIJOT_EVENT					// 2008. 11. 21 / �����, �豤�� / �̼ǿϷ�� �̿�������� ��� ���� �����̸� ����ġ ���ִ� �̺�Ʈ.				
	#elif defined(_CHINA) 
		/*($Delete_[S/C]$)*/#define			__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH	// 2008.12.10 //�̼��� // GM��ũ��Ʈ�߰�
		/*($Delete_[S/C]$)*/#undef			_NA_0_20081015_EXP_LEVEL_RATIO								// (�߱�����) 2008.10.15 / ���ؼ� / ����ġ���� ����. ���������� ����.
		#define			__CN_0_SSQ_CAHO_EXCEPTION									// 2008.12.22 / ��â�� / Ÿ���ѻ�� ī���� ����
		#define			_GS_ADD_PWD_X_BTN_1245										// 2008.12.22 / �豤�� / �߱��ҹ���x����߰�				
		/*($Delete_[S/C]$)*/#define			__NA_000000_20081202_ADD_EXP_ACTION_2						// 2008.12.02 / ����� / ����ġ �ֱ� ==> ��Ƽ�� �ش� �ʿ� �ִ� ��� �������� ����ġ �ֱ�� ����.
		#define			__CN_0_20090421_EXP_BENEFIT_UP								// 2009.04.21 / �̼���  // �߱� ���ʽ� ����ġ ���� ��� 400% -> 700% (chd
	#endif

#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.1.6 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 816
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(2)		// ���� ���� ��ȣ
	#endif	
	//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA_963_LOG_LIB_VERSION_REFACTORING					// 2008.04.29 / ��â�� / �α� ���̺귯�� ����
	/*($Delete_[S/C]$)*/#define __NA_0_ITEM_STREAM_STRUCT_SEPARATION					// 2008.05.30 / ��â�� / ������ ��Ʈ�� ����ü ���� �и�
	/*($Delete_[S/C]$)*/#define __NA_1127_DELETE_BUFF_ICON								// 2008.07.21 / ��â�� / ���� ������ ����
    /*($Delete_[S/C]$)*/#define __BUG_FIX_CAN_NOT_GET_POINT_WHEN_DEAD					// 2008.09.04 / ��â�� / ĳ���� ����ÿ� ��ŷ����Ʈ ���� ���ϵ��� ����
	

	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__			// 2008.06.17 / ������ / ���� �м� ����� ����� ���� �κ� �ڵ� ���� �۾� (4th macro)
	/*($Delete_[S/C]$)*/#define __NA001086_080616_BUFF_EVENT_UI_AND_MULTI_SUPPORT_FUNCTION__		// 2008.06.16 / ������, ������ / ���� �̺�Ʈ UI ���� �� ���� ���� ���� ������ ����
	/*($Delete_[S/C]$)*/#define __NA000000_080707_PLAYER_ENTER_NAK_RESULT_CONTROL__					// 2008.07.07 / ������, ������ / �÷��̾� ���� ���н� ���� ���� ó��
	/*($Delete_[S/C]$)*/#define __NA000000_080903_REGENTIME_CALC_PROBLEM_FIX__						// 2008.09.03 / ������ / ���� �Է� ��ġ�� ������ ���� �ð� ��� Ÿ�� �����÷� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_001128_20080717_ADD_GM_CMD							// 2008.07.17 / �̼��� / ���� �ʱ�ȭ, �ɸ��� ���, ��ų ��Ÿ�� �ʱ�ȭ, ��ų���� �ʱ�ȭ
	/*($Delete_[S/C]$)*/#define __NA_000921_20080618_TROUBLED_PARTS_REFACTORING			// 2008.06.18 / �̼��� / ���� ���� ��������
	/*($Delete_[S/C]$)*/#define __NA000000_080818_LEVEL_DIFFERENCE_NO_MASTER_BUG		// 2008.08.18 / �̼��� / ��Ƽ���� ���� ��Ƽ�� �ν����� �������ѿ� �ɸ��� �ʴ� ����. 
	/*($Delete_[S/C]$)*/#define	__NA_002260_20080821_BORADORY_BUG_FIX					// 2008.08.21 / �̼��� / ī�����°� �Ǿ ���� ������ �ð������� �����̰� �����Ǵ� ���� ����.
	/*($Delete_[S/C]$)*/#define	__NA_0_20080905_BORADORY_BUG_FIX						// 2008.09.05 / �̼��� / ������������ �����̰� �Ǵ� ����.			
	/*($Delete_[S/C]$)*/#define __NA001117_SUMMON_MONSTER_ITEM							// 2008.07.10 / ������         / ���ͼ�ȯ ĳ�þ����� �ű��߰�
	/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM							// 2008.07.17 / ������, ������ / ������ ���� ü���� �ý���

    /*($Delete_[S/C]$)*/#define __NA_1119_CHANGE_ETHER_EXPEND_STYLE						// 2008.08.12 / ��â��, �豤�� / ���׸� źȯ �Һ� ��� �ܹ߿��� ���߷� ��ȯ
    /*($Delete_[S/C]$)*/#define __NA_1339_ENCHANT_RENEWAL_EX							// 2009.05.12 / ��â��, ������ / ��þƮ ���н� �ı� - ���þ� �۾����� �߰� �۾�
	/*($Delete_[S/C]$)*/#define __NA_001013_20080428_ENCHANT_RENEWAL					// 2008.04.28 / �̼���, ����� / ��æƮ �ý��� ������ 
	/*($Delete_[S/C]$)*/#define __NA_001092_20080717_ADD_SKILL_ITEM						// 2008.07.17 / �̼���, ����ȯ / ����, �ɰ���, ȶ��, �� ������

	#define _YJ_PK_OPTION_001120								//08.08.05 //������ //PK �������� �ɼ� ó��
	/*($Delete_[C]$)*/#define _YJ_STAT_CHANGE_SYSTEM_001124						        //08.07.28 //������ //���� ü���� �ý���
	/*($Delete_[C]$)*/#define _YJ_ADD_DOF_RANGE_001101							        //08.07.09 //������ //���� ���� �ʿ�
	/*($Delete_[C]$)*/#define _YJ_TEXT_SHADOW_EFFECT_001905						//08.07.01 //������ //�ؽ�Ʈ ����ȿ�� �߰�
	/*($Delete_[C]$)*/#define _YJ_ADD_BUFF_EVENT_UI_001086						//08.06,30 //������ //�����̺�Ʈ ui �߰�

	/*($Delete_[S/C]$)*/#define __NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN	//            / ������, �豤�� / ĳ���� ����â���� �� �̹��� ���
	/*($Delete_[S/C]$)*/#define __SUMMONED_SPEED_CHANGE									// 2008.09.01 / ������, �豤�� / ���� ���� ���ǵ� ����

	/*($Delete_[S/C]$)*/#define	__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	// 2007.11.23 / ����� / ���ο� ��ũ��Ʈ �Ľ� �ý���.
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[C]$)*/#define _GS_MOD_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[S/C]$)*/#define _YJ_NEW_SCRIPT_PARSING_SYSTEM
		/*($Delete_[S/C]$)*/#define _KIKI_20080711_NEW_SCRIPT

		/*($Delete_[S/C]$)*/#define __NEW_SCRIPT_SYSTEM_BY_AEORAGIE_PART // 2008.07.14 / ������ / �űԽ�ų�ļ�
		/*($Delete_[S/C]$)*/#define __NA_000000_20080714_NEW_SCRIPT_PARSING_SYSTEM_APPLY    // 
		/*($Delete_[C]$)*/#define __NA_0_20080716_PARSER_RENEWAL_BY_MJKIM
	/*($Delete_[S/C]$)*/#endif //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	

	/*($Delete_[S/C]$)*/#define __NA_001093_20080623_SERVER_DISCONNECT_			    // 2008. 06. 23 / ����� / �������� ������ ���� ���� �� ó��.
	/*($Delete_[S/C]$)*/#define	__000000_20080811_BUG_FIX_VIEW_JOIN_TOP_TIME		// 2008. 8. 11 / �����, ��â�� / '���ֹ��� ž�� �Ϲ� ���迡 �ð� ǥ��' ==> null üũ ���� ����.

	/*($Delete_[C]$)*/#define _GS_ADD_TARGETING_EFFECT_1125							// 2008.08.13 / �豤�� / ���� Ÿ��������Ʈ

	/*($Delete_[C]$)*/#define _GS_ADD_SINGLEMODE_MONSTER_1016							// 2008.08.13 / �豤�� / �̱۸��� �����̵��׽�ų����

	/*($Delete_[S/C]$)*/#define __NA_0_20080717_DBUNIT_ADD_SERVERTYPE						// 2008.07.17 / ���ؼ� / DBUnit�� m_ServerType ������� �߰�
	/*($Delete_[S/C]$)*/#define __NA_0_20080825_MODIFY_OPTION_DEFENCE_POWER_CALC			// 2008.08.25 / ���ؼ� / �ɼ� ���ݷ�, ���� ���� ���� ���� ����


    #define __NA_001062_20080623_STRENGTH_PENALTY					// 2008.06.23 / ��ȣ��, �豤�� / ������ ���ҿ� ���� �г�Ƽ �ο�. 

	//����������
	#ifdef _CHINA
		//#undef __NA_001062_20080623_STRENGTH_PENALTY				//�߱��� ������ ����
	#elif defined(_JAPAN)
		#undef _YJ_PK_OPTION_001120									//�Ϻ��� PK�����ɼ� ����
	#endif

#endif

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.1.5 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 815
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(1)		// ���� ���� ��ȣ
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define __NA_001044_ADD_CUSTOM_UI								// 2008.05.28 / lammy / �ɸ��� ������ �̸� �Ծ�� ��� �߰�	

	/*($Delete_[C]$)*/#define _K2P_CHARACTER_ROTATE_000986							// 2008.05.28 / lammy / �ɸ��� ������ ȸ�� ��� �߰�
	/*($Delete_[C]$)*/#define _K2P_ADD_INVITE_BUTTON_001018							// 2008.05.28 / lammy / �ʴ� ��ư �߰�
    /*($Delete_[C]$)*/#define _GS_MOD_CHATMSG_MAINTAIN_1058							//[1058] 2008.05.30 / GS / ���̵��� ä�ó��� ���� 
    /*($Delete_[C]$)*/#define __NA_001003_PC_MONSTER									// 2008.05.07 / kjws78 / pc�� ���� �߰�
    /*($Delete_[C]$)*/#define __NA_0_20080507_CHANGUP_PC_SKLLL_ADD					// 2008.05.07	/ ����ȯ / ü�������� ���� PC ��ų �߰�
    /*($Delete_[C]$)*/#define __NA_0_2008_06_03_PLAYER_INVALID_EQUIPMENT_CHECK		// 2008.06.04	/ ����ȯ / �ڽŰ� �ٸ� Ŭ������ ��� ����� �⺻ ���� ������
    /*($Delete_[C]$)*/#define __NA_1061_20080609_SHOW_MY_ID_RENDERINFO							//08.06.09   / ����ȯ / �� ���̵� ���̱� ����߰�
    /*($Delete_[C]$)*/#define _YJ_MOD_QUEST_NPC_NAME_COLOR_1938									// 08.06.18 / ������ / ����Ʈ NPC ���� �÷� ����
    
    


    /*($Delete_[S/C]$)*/#define __NA_000962_ADD_GUILD_ERROR								// 2008.05.28 / ����ö, ���ǥ / ���� ��� �̸����� ���� ������ �߰� 
	/*($Delete_[S/C]$)*/#define	__NA_001012_20080506_RESERVE_HP_SYSTEM					// 2008.05.06 / �����, ����� / �⺻ HP�ý����� ����ȭ�� �����Ͽ� ���尨 �� ������ ȿ�뼺�� ���δ�.
    /*($Delete_[S/C]$)*/#define _BUG_FIX_LOTTERY_SYSTEM									// 2008.09.11 / �����, �豤�� / ���� �ý��� ���� �ߺ��Ǵ� ���� ����.
    /*($Delete_[S/C]$)*/#define __NA_000946_20080415_LOTTERY_SYSTEM						// 2008.05.28 / ������, �豤��         / ���ǽý���					[NO:0946]
    /*($Delete_[S/C]$)*/#define __NA_001068_ACTION_DEAD_PLAYER							// 2008.06.19 / ������, ���ǥ         / ����� �÷��̾� Ŭ������	[NO:1068] 
    /*($Delete_[S/C]$)*/#define __NA_000994_CHANGUP_NEW_SKILL							// 2008.05.28 / ������, ����ȯ         / �űԽ�ų					[NO:0994]

    /*($Delete_[S/C]$)*/#ifdef __NA_000994_CHANGUP_NEW_SKILL                                // 
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS							// ����, ������ ���� ������ �鿪, ���ݰ���
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS_BUGFIX					// �����̻� �ɸ��� ���� ����
        /*($Delete_[S/C]$)*/#define CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS					// ��ȭ, ��ų ������ 300%���
        /*($Delete_[S/C]$)*/#define CHANGUP_SUMMONMASTERY_SKILL								// ���� �����͸�
        /*($Delete_[S/C]$)*/#define CHANGUP_IMPOSSIBLE_ATTCAK_STATUS						// ���ݺҰ�, ������ �� ����.
        /*($Delete_[S/C]$)*/#define CHANGUP_PURGE_SKILL										// ����� ������ �������� �߹��Ѵ�.
        /*($Delete_[S/C]$)*/#define CHANGEUP_SAINTAID										// �����̻� ����
        /*($Delete_[S/C]$)*/#define CHANGUP_RECKLESS_STATUS									// ����, ��ȯ���� ������ ���� ���
    /*($Delete_[S/C]$)*/#endif


	/*($Delete_[S/C]$)*/#define	__NA_000000_20080522_DROPRATIO_BUGFIX					// 2008.05.22 / ����� / ������ Drop �� �ڷ����� WORD���̿��� ����� ���� ����.
	/*($Delete_[S/C]$)*/#define	__NA_001042_NEW_CHARACTER_ITEM							// 2008.08.26 / ����� / �ű� ĳ���� ������ �·�� ĳ�þ������� ����.


    // (WARNING) �ϱ� working macro�� ������ ����� ������ �� �����Ƿ� ������ ����. ������ �� ��.
	/*($Delete_[S/C]$)*/#define	__NA_001070_20080610_DBPROXY_LOG						// 2008.06.09 / ����� / DBProxy�α� ����


    // ������ �� ��////////////////////////////////
    #define _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// 2008.05.28 / lammy / �ɸ��� ����â�� ���� �̵���ư �߰�
    #define __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT  // (�߱� ���� ������) 2008.04.28 / ��â�� / ����Ƽ�� ������ ���� Ƚ�� ���� (����������)
    /////////////////////////////////////////////


	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080616_HP_ZONEMOVE						// 2008.06.16 / ����� / �� �̵��� HP ����( HPȮ�� �������� ������ �־ �̵��� ����ȵǴ� ���� )
	/*($Delete_[S/C]$)*/#define	__NA_001081_20080612_TUTORIAL_CHANGE					// 2008.06.12 / �����, �豤�� / Ʃ�丮�� ������ ����.
	/*($Delete_[S/C]$)*/#define __NA_VENDOR_BUGFIX_										// 2008.09.23 / ����� / ��ȯ �� ����Ʈ ���� ���� ����.
	/*($Delete_[S/C]$)*/#define _BUG_FIX_20081007_WOPS2463_JUMP_DAMAGE					// 2008.10.07 / ����� /���� ������ ������ ���� �ʴ°�ó�� ���̴� ���� ����.
	/*($Delete_[S/C]$)*/#define __NA_001039_20080509_GM_COMMAND_MODIFY					// 2008.05.09 / �̼���, �豤�� / GM ��ɾ� ����, �߰�	
	/*($Delete_[S/C]$)*/#define __NA_0_20080509_GM_COMMAND_MODIFY_DEV					// 2008.05.09 / �̼��� / ���� �׽�Ʈ�� GM��ɾ� ����, ����, gm��ũ��Ʈ����
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_HAMMER_OF_ALCHEMIST_MODIFY			// 2008.05.17 / �̼��� / ����� ��ġ ��� ����
	/*($Delete_[S/C]$)*/#define __NA_1921_20080610_BUG_FIX_HAMMER_OF_ALCHEMIST_MODIFY	// 2008.06.10 / �̼��� / ����� ��ġ ������ ���� ������ == �ִ� ������ �Ͻ� ���� �ȵǴ� ���� 
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_WORLD_SHOUT_CONSUME_MODIFY			// 2008.05.17 / �̼��� / �ع決�� ���� ��� ����
	/*($Delete_[S/C]$)*/#define __NA_0_20080603_REMOVE_QUESTSCRIPT_ASSERT				// 2007.06.03 / �̼��� / ����Ʈ �ļ����� load �Լ��� assert, Validating �Լ��� ������
	/*($Delete_[S/C]$)*/#define __NA_001892_20080618_WORLD_SHOUT_CONSUME_MODIFY_BUGFIX  // 2008.06.18 / �̼��� / ���� ��ġ�� pc�濡�� �ȵǴ� ���� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY			// 2008.06.10 / �̼���, ������ / ������(����������) ���ݼ���(�Ϲ�.����.PK��) üũ ����
	/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / �̼��� / CHECK_ATTACK_PROPENSITY_MODYFY �۾��� ���� ��üũ �߰�.
	/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX						// 2008.06.26 / �̼��� / ����������� ������ų ������ ������ �Ǵ� ����.
	/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX					// 2008.06.30 / �̼��� / ����ī�� ������ ����Ʈ ���� �����ϰ�(��ũ��Ʈ����).[��ũ��Ʈ ����� ���� �߰� �Ǿ����]
	/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY						// 2008.07.01 / �̼��� / ī������ �ڵ����� �������ݵ��� �ʴ� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK						// 2008.07.24 / �̼��� / Ÿ��üũ�� ����Լ��� ����ƽ���� ��� ���� ����.
	/*($Delete_[S/C]$)*/#define __NA_001074_ETHER_EMISSION_DEVICE_EXPAND				//            / ������         / ���׸� ������ �ɼ�
	
	/*($Delete_[S/C]$)*/#define __NA_1047_EXP_POTION												// 2008.06.02 / ��â�� / ����ġ ����
	/*($Delete_[S/C]$)*/#define __NA_1075_FIELD_BATTLE_SERVER_LOG_REGULATION						// 2008.06.09 / ��â�� / �ʵ� ��Ʋ ���� �α� ����
	/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__							// 2008.04.02 / ������, ������ / Skill���� ��Ŷ ó�� ó�� ������ ���� �����۾� Version01
	/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__								// 2008.04.21 / ������, ������ / �Ϲ� ���� �˰��� ������ ���� ��Ŷ �м� �� ����
	/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__								// 2008.06.19 / ������ / PacketControl 3rd Server<->Server (Ư�� ��Ŷ�� ���ؼ��� �۾�)
	/*($Delete_[S/C]$)*/#define __NA000000_080228_TARGET_FIND_CORRUPT_FLOW_FIX__					// 2008.02.28 / ������ / GameField::SearchTarget ����, �ӽ� ��ü�� �����͸� �̿��� ����õ�� �ڵ� ���� - �ٸ� ������ ������ �߻��ϴ� ���� ���� �ľ� �ʿ�
	/*($Delete_[S/C]$)*/#define __NA000000_GRID_REBUIDING_FOR_READABLE_CODE__						// 2008.04.24 / ������ / Player_Grid ������ ���̱� �۾�
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_NPC_KILL_OPTIMIZER__		// 2008.02.19 / ������ / [844,896] NPC Kill Eventó���� ������ ���� ���ſ�, �̺�Ʈ�� �Ҵ�� ���͸� ó���� �� �ִ� ���� _�� 100�� ����Ʈ ��ɿ� ����Ǿ� �ִ� ���� ���� �� �߰� �۾�
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_INSTANCE_ZONE__			// 2008.02.19 / ������ / [844,896] InstanceDungeon ���� ó�� _�� 100�� ����Ʈ ��ɿ� ����Ǿ� �ִ� ���� ���� �� ���� �۾�
	/*($Delete_[S/C]$)*/#define __NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__					// 2008.05.27 / ������, ������ / �÷��̾� �ִϸ��̼� ó���� ��ġ����ȭ ���� �ʴ� ���� �ذ��� ���� �۾�
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO03__			// 2008.06.XX / ������ / ���� �м� ����� ����� ���� �κ� �ڵ� ���� �۾�
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_SOLARHASH_PROBLEM_CORRECTION_V01__				// 2008.06.19 / ������ / SolarHashtable ���Ἲ �˻�(DEBUG) & ���� ���� ����
	/*($Delete_[S/C]$)*/#define __NA000000_080626_TUTORIAL_MISSION_LEAVE_CASE_CORRECTION__			// 2008.06.26 / ������ / Agent�� �����ϴ� Ʃ�丮�� ���°� �̻��(��������...?)�Ǹ鼭 ���̴� ���� ����
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL__			// 2008.06.09 / ������ / ���¡ ���� ����å_20080609 : ����,������ ���� ��Ȳ�� ���� �� �м� �� �ʿ��� ��� ���� �� ����
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__		// 2008.07.01 / ������ / HackShield ���� ���ؼ� �ŷڵ� ���̱� �۾� (����ȭ �Ǹ� ���� ��ũ�ο� ��ĥ ��)
	/*($Delete_[S/C]$)*/#define __NA000000_081218_GAMEFIELD_LEAVE_FIELD_USAGE_CORRECTION__			// 2008.12.18 / ������ / GameField::LeaveField�� �� ��ü ���� ���ϴ� ���� ����(�޸� ����)
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_REFACTORING						// 2008.02.19 / ���ؼ� / ���� Ŭ���̾�Ʈ ����å. ���¡ ���� ó�� �����丵
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_POWERFUL						// 2008.02.19 / ���ؼ� / ���� Ŭ���̾�Ʈ ����å. ���¡ ���� ó�� �߰�����
	/*($Delete_[S/C]$)*/#define __NA_0_20080430_SKILL_REFACTORING									// 2008.04.30 / ���ؼ� / ��ų �Ŵ��� �����丵
	/*($Delete_[S/C]$)*/#define __NA_891_20080602_CHANGE_EXP_RATIO2									// 2008.05.02 / ���ؼ� / �Ϻ� ����ġ ���� ���� ������ �ѱ����� ����
	/*($Delete_[S/C]$)*/#define __NA_28934_20080609_CANBE_ATTACKED_CHECKMISS_BUG_FIX				// 2008.06.09 / ���ؼ� / ��ų ����Ʈ ó������ CanbeAttacked() üũ �߰�. ��ö���µ��� ������ ������ ���� ����
	/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT					// 2008.06.13 / ���ؼ� / ����Ʈ ���ɰ���
	/*($Delete_[S/C]$)*/#define __NA_0_20080616_AGENT_CHARSELECT_INIT_BUG_FIX						// 2008.06.16 / ���ؼ� / ĳ���� ����ȭ������ ���ƿ� �� �ʱ�ȭ �ȵȰ� ����. RemoveUserForCharGuid �α� �ȳ��� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX						// 2008.06.17 / ���ؼ� / Ŭ������ ��Ÿ�� ����Ʈ ��� ����. ������ ��ȹ�Ǿ����� ���� ������ ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20080619_GET_VALID_SPECIAL_AREA								// 2008.06.19 / ���ؼ� / WzSpecialArea ���� �� �̵������� ��ȿ�� Ÿ���� �����ϴ� �Լ� �߰�
	/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD								// 2008.06.09 / ����ö,������ /������ ����Ʈ �ڵ� �߰�
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER					// 2008.5.30 / ����ö / ���� ���� ���� Ŭ���̾�Ʈ ������Ŷ
	/*($Delete_[S/C]$)*/#define		__NA_BUG_181415_20080820_GUILD_BUG_FIX							// 2008.8.20 / ����� / �����Ͱ� ������ �������� ������ �� �ִ� ���� ����.

	/*($Delete_[S/C]$)*/#define __NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__					// 2008.03.24 / ������, ����ȯ / 100���� ü������
	/*($Delete_[S/C]$)*/#define __NA001048_080514_APPEND_ABILITY_KNOCKBACK2__						// 2008.05.14 / ������, ����ȯ / �˹�2 �����Ƽ �߰� (����:Ability���� Flow ���� �� ���� ���⵵ ���� ����)
	/*($Delete_[S/C]$)*/#define __NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__		// 2008.05.26 / ������, ������ / [�̺�Ʈ]2�ֳ� �̺�Ʈ ����Ʈ ��� �۾�
	/*($Delete_[S/C]$)*/#define __NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__		// 2008.06.18 / ������, �豤�� / Cash Shop �� �����ϱ� ��� �Ұ��� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_001053_20080522_ATTR_RATIO_TYPE								// 2008.05.22 / ���ؼ�, ����� / ����Ÿ��(5) �ɼ� ����
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_GUILD_WAREHOUSE_LIMIT_ITEM_LOG						// 2008.5.30 / ����ö, ������ / ��� â�� �α� �Ϲ� �����۰� ����Ƽ�� ������ ����


	#ifdef _CHINA
		#define _GS_ADD_QUEST_SCROLL_1143										// 08.09.06 / �豤�� / �߱�NPC����Ʈâ ��ũ�ѹ��߰�
        //-------------------------------------------------------
        #undef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT   // 2008.04.28 / ��â�� / ����Ƽ�� ������ ���� Ƚ�� ���� (����������)
        //-------------------------------------------------------
		/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG							// 2008.10.21 / ��â�� / ������ �� ���� ���� ���ѿ� �ɸ��� �ʴ� ����
		/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG						// 2008.10.21 / ��â�� / ��ž ��Ƽ�� �� �ε��� �߻��ϴ� ����
		/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG									// 2008.10.22 / ��â�� / ��ž�� ������ �ƴѰ����� ���� �� ����		
		/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM										// 2008.09.13 / ��â��,�豤�� / ��Ű ����Ʈ ������ ���� ( ����Ʈ1.5�� )		

		/*($Delete_[S/C]$)*/#define	__NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT						// 2008.09.23 / ��â�� / ���� �κ��� ���Ե� ������ ����ϵ���...
		#define __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP							// 2008.09.23 / ��â��,�豤�� / 100���� ü������ �Ҷ� ���� �ʱ�ȭ		
		#define _GS_ADD_PWD_X_BTN_1143                                          // 08.08.13 / �豤�� / �߱�â������x��ư�߰�

		/*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM										// 2008.10.28 / ��â��, �豤�� / ����Ʈ 1.5��
		/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG										// 2008.10.30 / ��â�� / ���Ǹ������� ���� ����ο��� ȿ�� ����Ǵ� ����
        // (NOTE) (100130) ������ ��ȹ�� ���ǽ� ��� ��ũ��Ʈ�� ���ϵ� ���¿���. (���� Ʋ����...) ���Ͻ�Ű���� ����.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / ��â��,�豤�� / �� �ɼ� �÷��׷� ���� ����(��ũ��Ʈ����...)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / ������, ������ / ������ ���� ü���� �ý���
		//!������� ���̺�		
		/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY					// 2008.10.24 / ���ؼ� / ���������� ���� ����. ���� �����Ӽ����� ���� �ɼ� ���� ����!

	#elif defined(_KOREA) 

	#elif defined(_JAPAN)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / ������, ������ / ������ ���� ü���� �ý���
		#define _K2P_ADD_WISPER_WINDOW
		#define _K2P_USE_CHATWINDOW_USING_VENDORLIST
		#define _K2P_HERO_POSITION
		/*($Delete_[C]$*/#define _K2P_CHECK_USE_ITEM_BY_POPUP
        // (NOTE) (100130) ������ ��ȹ�� ���ǽ� ��� ��ũ��Ʈ�� ���ϵ� ���¿���. (���� Ʋ����...) ���Ͻ�Ű���� ����.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / ��â�� / �� �ɼ� �÷��׷� ���� ����(��ũ��Ʈ����...)
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW								// 2008.03.28 / ���ؼ� / �渮��Ʈ ����üũ���� ��� �����ֱ�. �ϴ� ������ ����.
		//#define __NETWORK_FIX_20081119													
	#endif

#endif
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.1.4 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 814

	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// ���� ���� ��ȣ
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(2)		// �߰� ���� ��ȣ
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(7)		// ���� ���� ��ȣ
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[C]$)*/#define _GS_MOD_PERSONAL_VENDER_00877								// 2008.03.24 / �豤�� / ���λ����Խ��� ��ũ�ѹ�����
    /*($Delete_[C]$)*/#define _GS_MOD_ITEM_NAMMING_COLOR_000882							// 2008.03.24 / �豤�� / �����۳��ֻ̹� & ����Ƽ�� ����û�����
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_SCROLL2											// 2008.04.02 / �豤�� / ��ŷ&�̼ǰ�����,����Ʈâ,����â ��ũ�Ѽ���
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_EVENTRECT_928									// 2008.04.10 / �豤�� / �̼� help,�ý���ä��â ��ŷ ��������
    /*($Delete_[C]$)*/#define _GS_BUGFIX_GUILD_NOTICE_1470								// 2008.04.23 / �豤�� / �������̽����� ����Ʈ��Ʈ�� �������߰�
    /*($Delete_[C]$)*/#define __NA_000966_20080312_ADD_PARAM_FOR_ADDITIONAL_SOUND			// 2008.03.24 / ����� / ������ ���� �߰��� ���� ��ũ��Ʈ ���� �� �Ķ���� �߰�
    /*($Delete_[C]$)*/#define __NA_0_20080318_PROCESS_AFTER_RENDERING						// 2008.03.24 / ����� / ������ �� ó�� �Լ� �߰�
    /*($Delete_[C]$)*/#define __KR_000978_20080321_CHANGE_HUNTING_JOIN_LEVEL_OFFSET		// 2008.03.24 / ����� / ���� ���� ���� ���� ���� ����
    /*($Delete_[C]$)*/#define __NA_0_20080311_BUGFIX_TARGET_INFO_FOR_TREE_OF_LIFE			// 2008.03.11 / ����� / ����, ������ ���� Ÿ�� ���� ����(%d�� ǥ��) ����

    /*($Delete_[x/C]$)*/#define __NA_676_PK_SYSTEM_IMPROVEMENT								// 2008.01.29 / ��â��, ������ / PK �� ����

    /*($Delete_[S/C]$)*/#define __MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM						// 2007.12.29 / ����ȯ,��â�� / ������ ����ȭ �ִ� ���� ����.    
    /*($Delete_[S/C]$)*/#define __NA_000968_ETHER_EMISSION_DEVICE							// 2008.03.14 / ��â��, ����ȯ / ���׸� ������ġ
    /*($Delete_[S/C]$)*/#define __NA_780_PREVENTION_PARTY_RETURN_ABUSE						// 2008.02.05 / ��â��, �豤�� / ������ ��� �ǿ� ����
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SYSTEM											// 2008.02.14 / ��â��, �豤�� / �� �ý��� - �߷�Ÿ�ε��� ����ϴ�    
    /*($Delete_[S/C]$)*/#define __NA_1006_POINT_RANKING_REGISTER_LV							// 2008.04.17 / ��â��, �豤�� / ����Ʈ ��ŷ ��� ���� �߰�	

    /*($Delete_[S/C]$)*/#define __NA_0_CHARGE_ITEM_SELL_CHECK_REMOVE						// 2008.03.11 / ��â�� / ���� ������ �ȱ� ��� üũ ����( ��ũ��Ʈ���� �����Ѵ�ϴ�. )
    /*($Delete_[S/C]$)*/#define __NA_825_HOLDDROP2											// 2008.01.21 / ��â�� / ������ ��2
    /*($Delete_[S/C]$)*/#define	__NA_0_BUG_FIX_POINT_RANKING_PERIOD							// 2008.01.25 / ��â�� / ����Ʈ ��ŷ �ֱ� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_790_MODIFY_BASE_PLAYERINFO_VERSION						// 2007.11.30 / ��â�� / Base_PlayerInfo �α� ���� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_977_PREVENTION_PARTY_RETURN_ABUSE						// 2008.03.26 / ��â�� / ������ ��� �ǿ� ���� �߰� �۾�
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SAVE_TO_DB										// 2008.02.21 / ��â�� / �� ��ȯ �� DB�� ����
    /*($Delete_[S/C]$)*/#define __NA_0_PET_FEED												// 2008.03.06 / ��â�� / �� ����
    /*($Delete_[S/C]$)*/#define __NA_0_PET_TEST												// 2008.03.06 / ��â�� / �׽�Ʈ �ڵ� �����Ӵ� �۾� �� ����
    /*($Delete_[S/C]$)*/#define __NA_821_STAT_BUG_RECOVERY									// 2007.12.20 / ��â�� / ���� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_989_ABOLITION_LIMIT_LV_IN_CURSE_TOWER					// 2008.03.31 / ��â�� / ������ž ���� ���� ö��
    /*($Delete_[S/C]$)*/#define __NA_1443_BUG_FIX_HOLDDROP									// 2008.04.07 / ��â�� / ������ ��� ���Ӱ� �Ҹ�Ǵ� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_0_SOCKET_OPTION_BUG_RECOVERY							// 2008.05.08 / ��â�� / ���� �ɼ� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_SKILL_ALL_STAT_UP							// 2008.05.15 / ��â�� / ��ų�� �ý��� �ɼ� ����� ���¹��� �Ѥ�;
    /*($Delete_[S/C]$)*/#define __NA_1017_LUCKY_ITEM_IDENTIFY_LOG							// 2008.04.23 / ��â�� / ��Ű ������ ���� �� �α� ����
    /*($Delete_[S/C]$)*/#define	__NA_0_PET_TIMER_PERIOD_CHANGE								// 2008.05.26 / ��â�� / �� Ÿ�̸� �ֱ� ����
    /*($Delete_[S/C]$)*/#define __NA_1090_NO_TRADE_WHILE_PET_SUMMONING						// 2008.06.18 / ��â�� / ��ȯ �� �ŷ� �� ���� ���� �Ǹ� ����


	/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_SYSTEM  //2�� ���ý���
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_2ND_GUILD_SYSTEM                                 // 
		/*($Delete_[S/C]$)*/#define __NA_000596_20070821_GUILD_CLASS_SYSTEM					// [596] 2007.08.21 / ����� , ����ȯ / ��� ��� �ý���
		/*($Delete_[S/C]$)*/#define __NA_000597_20070821_GUILD_CORPS_SYSTEM					// [596] 2007.08.21 / ����� , ����ȯ / ��� ���� �ý���
		/*($Delete_[S/C]$)*/#define __NA_000614_GUILD_RANK_REFACTORING						// [614] 2007.08.09 / ����� , ��� ��ũ�� �ý��� �����丵( �߱� ��� ��ũ�� �������� �۾� )
		/*($Delete_[S/C]$)*/#define __NA_000593_20071226_GUILD_RELATION_SYSTEM				// [593] 2007.12.26 / ����ö , ������ / ��� ���� �ý���
		/*($Delete_[S/C]$)*/#define __NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY	 		// [593] 2007.12.31 / ����ö , ������ / ��� Ż��,����,�ػ꿡 ���� �г�Ƽ
		/*($Delete_[S/C]$)*/#define __NA_000569_20080117_GUILD_MARK							// [593] 2008.01.17 / ����ö , ����ȯ / ��� ��ũ
		/*($Delete_[S/C]$)*/#define __NA_0_20070829_GUILD_ADD_INFO							// [000] 2007.12.16 / ����ö , ������ / ��� ���� �߰� ����
		/*($Delete_[S/C]$)*/#define __NA000584_20080201_GUILD_WAREHOUSE						// [000] 2008.02.01 / ����ö , ������ / ��� â�� <2�� ��� �ý��ۿ� ����־�� �Ұ�>
		/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_RENDERINFO_PACKET					// [584] 2008.02.01 / ����ö , ��� ���� ���� ��Ŷ�� ���� �߰� / ����ö/
		/*($Delete_[S/C]$)*/#define __NA_000880_20080204_GUILD_NOTICE						// [880] 2008.02.01 / ����ö , ��� ���� ���̸� 150 -> 768�� �ø�, ��� �������� �����Ǵ� ������ ���弭���� �ű�
		/*($Delete_[S/C]$)*/#define __NA_0_20080406_GUILD_MARK_MODIFY						// 2008.04.11 / ����ö / ��� ��ũ/���� ��Ŷ ���� ����		
		/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080613_GUILD_CONVERT_GP					// 2008.06.13 / ����� / WOPS_1963 / ��� ���� ����( �ƹ��� GP��ȯ�� �Ǵ� ���� ����. )
		/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080618_GUILD_RIGHT_VIEW					// 2008.06.18 / ����� / WOPS_1893 / ��� ���� ���� ���� ����.
		/*($Delete_[S/C]$)*/#define _BUGFIX_WOPS2627_20081020_GUILD_NOTICE					// 2008.10.20 / ����� / WOPS_2627 / ��� ���� ���� ����.
	/*($Delete_[S/C]$)*/#endif //__NA_000000_2ND_GUILD_SYSTEM

	/*($Delete_[S/C]$)*/#define	__NA_000000_20080709_DBP_MEMERY_LEAK_BUGFIX					// 2008.07.09 / ����� / DBP �޸� �� ���� ����.
	/*($Delete_[S/C]$)*/#define	__NA_001108_20080704_GM_ADD									// 2008.07.04 / �����, ���ǥ / GM��ɾ� �Ϻ� MOVE, CALL �ǵ��� ����.
	/*($Delete_[S/C]$)*/#define	__NA_000950_20080311_HEIM_DISTRIBUTE_SYSTEM					// 2008.03.11 / ����� / ������ �й�ý��ۿ� �����ϱ�.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20080108_WORLDSERVER_HANDLER_MACRO				// 2008.01.08 / ����� / ���弭�� ��Ŷ �ڵ鷯 ��ũ�� ����.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080310_WORLD_FRIEND_OVERFLOW					// 2008.03.10 / ����� / ���弭�� ģ������ ��� �����÷� ���� ����.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_WOPS1373_20080424_DRAGON_TRANSFORM_STATUP_BUG	// 2008.04.24 / ����� / �巡�� ���Ż��¿��� ������ �ø���� ��� ������ ���� �Ұ����� ����.
	/*($Delete_[S/C]$)*/#define __NA000920_20080306_NPCSHOP_ITEM_SELL_HISTORY				// [920] 2008.03.06 /����ȯ,����ö/ NPCShop ������ �Ǹų��� �����丮.
	/*($Delete_[S/C]$)*/#define __NA_0_080131_LF_BUG_MODIFY                                 // [000] 2008.01.10 / ����ö / ���� ������ ���๮�ڰ� ó������ �ʴ� ���� ����.	
	/*($Delete_[S/C]$)*/#define __NA_000921_20080227_TROUBLED_PARTS							// 2008.02.27 / �̼���, ������ / ���� ���� �߰� & �Ǿƽĺ� ��
	/*($Delete_[S/C]$)*/#define __NA_0_20080326_ARRANGMENT_LOG_02							// 2008.03.26 / �̼��� / 2�� �α� ����
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080326_RELEASE_PARTY_KEY						// 2008.03.26 / �̼��� / ��Ƽ ����� ������ ��ƼŰ�� ���� �ִ� ����( �ʵ� ���� )
	/*($Delete_[S/C]$)*/#define __NA_0_20080417_ADD_GUILD_PK								// 2008.04.17 / �̼���, ������ / ��� ���迡 ����  PVP�߰�(�Ϲ�����)
	/*($Delete_[S/C]$)*/#define __NA_0_20080418_ADD_TROUBLED_KILL_LOG						// 2008.04.18 / �̼��� / �������� ���� ���ӷα� �߰�
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080421_MODIFY_CHAO_BORADORI_LOGIC				// 2008.04.21 / �̼��� / ���� ��Ƽ������ �������ݽ� ������ �����ʰ� �����̴� �Ǵ� ����
	/*($Delete_[S/C]$)*/#define __NA_00000_080107_ADD_SERVEROPTIONEX_CHAO_CONDITION_		// 2008.01.07 / ������ / �ɸ����� ����(ī��,�Ϲ�)�� ���� �ν������� ��Ʋ�� �̿밡�� ���θ� �����ϴ� �ɼ� �߰�
	/*($Delete_[S/C]$)*/#define __20080414_SLOTSTREAM_FOR_PACKET_BUGFIX						// 2008.04.14 / ������ / SCSlot ���� Packet Stream ���� ����
	/*($Delete_[S/C]$)*/#define __NA000773_071108_ITEM_FUNCTION_EX_REVISION__				// 2008.03.12 / ������ / __NA000773_071108_ITEM_FUNCTION_EX__�� ���� ���� �ڵ� ���� + [957] ��� ����
	/*($Delete_[S/C]$)*/#define __NA001033_ARRANGEMENT_AGENT_LOG__							// 2008.04.30 / ������ / AgentServer Log ����
	/*($Delete_[S/C]$)*/#define __NA_0_20080227_BACKTO_CHARLISTREQ_SYN_MODIFY3				// 2008.02.27 / ���ؼ� / �븸 �ŷ� ������ ���� ����
	/*($Delete_[S/C]$)*/#define _NA000000_20080227_MOB_AI_TRACKING_PROBLEM_CORRECTION__		// 2008.02.27 / ���ؼ� / ���� AI Tracking ���°� ������ �ݺ��� Ȯ�� ���̱� �۾�( �ᱹ, ���������� ��εǾ� ���� Tracking�ϴ� ���´� ������������...)
	/*($Delete_[S/C]$)*/#define __NA_0_20080411_ALLPLAYERS_EQUIPINFO_BUG_FIX				// 2008.04.11 / ���ؼ� / �������� ĳ���͵��� ��� ���� �ִ� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_997_20080415_SKILL_DAMAGE_FORMULA_MODIFY				// 2008.04.15 / ���ؼ� / ��ų������ ������ ����. ���õ� ���.
	/*($Delete_[S/C]$)*/#define __NA_0_20080415_DBPROXY_USER_SELECT_QUERY_INIT				// 2008.04.15 / ���ؼ� / Query_User_Select ���۸� �Ź� �ʱ�ȭ�ϵ��� ����. ���� ĳ���� ���̰��� ����Ǵ� ���� ����.
	/*($Delete_[S/C]$)*/#define __NA_0_20080425_WORLD_SERVER_ITERATOR_BUG_FIX				// 2008.04.25 / ���ؼ� / end()�� ����ó�� �߸��Ѱ� ����
	/*($Delete_[S/C]$)*/#define __NA_0_20080506_DBSERIAL_DUPLICATE_BUGFIX					// 2008.05.06 / ���ؼ� / ������ �ߺ��ø��� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_20080801_SHOP_GAMESERVER_DOWN_BUGFIX					// 2008.08.01 / ���ؼ� / �� ������ ����Ʈ�� �޴ٰ� ���Ӽ����� ��� �ٿ�Ǵ� ���� �߻�
	/*($Delete_[S/C]$)*/#define __NA_000890_20080130_SEND_MEMO_BLOCK_CHECK					// ����ö / �ڽ��� ������ ������� ���� ������ ���� ���� üũ
	/*($Delete_[S/C]$)*/#define __NA000000_080327_TRADESELL_WAREHOUSE_BUGFIX				// â��� �̵��Ǹ� �ȵǴ� �������� â��� �̵��Ǵ� ���� ����
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080404_FATIGUE_VILLAGE_APPLY_OTTA_			// �Ƿε� �������� ������� �ʴ� ���� ����.(�߱����� ������.)
	/*($Delete_[S/C]$)*/#define __NA_0_20080416_PARTY_JOIN_PACKET_CHANGE					// ����ö / ��Ƽ �ʴ�� ���� ���迡 �ִ� �����鳢�� ��Ƽ �Ұ�.
	/*($Delete_[S/C]$)*/#define __NA_0_20090122_PARTY_JOIN_PACKET_CHANGE_BUG				// 2009.01.22 / ��â��, ��ö�� ��Ƽ ���� ����
	/*($Delete_[S/C]$)*/#define __NA_000998_20080416_INVITE_BY_CHARNAME						// 2008.04.24 / ���ǥ, ����ö / �ƹ��������� ��Ƽ, ��� �ʴ� �����ϰ� ����
    /*($Delete_[S/C]$)*/#define __NA_0_20080415_DISP_MY_ITEMINFO_AT_TOOLTIP_IF_SHIFT_DOWN	// 2008.04.21 / �����/ ����/��� ������ ���� ǥ�ý� CtrlŰ�� ���� ���, �ڽ��� ������ ������ ���� ���� ǥ��
	/*($Delete_[S/C]$)*/#define __NA_1633_20080423_BUGFIX_DONT_CREATE_CHAR_IF_CHARNUM_FULL  // [1633] 2008.04.24 / ����� / ĳ���� 5���� ��� �����Ǿ� �ִ� ���¿����� ĳ���� ����ȭ������ �̵��ϴ� ���� ����
    /*($Delete_[S/C]$)*/#define __NA_001030_20080428_CENTER_ALIGN_RANK_OPTION				// 2008.04.30 / ����� / �������� ��ũ �ɼ� ��� ����
    /*($Delete_[S/C]$)*/#define __NA_001688_20080429_BUGFIX_HIDE_LOCKBTN_IF_CURSTATE_IS_UNLOCK	// 2008.04.30 / ����� / â�� ��� ������ N��ư �����	
    /*($Delete_[S/C]$)*/#define _YJ_MOD_TOOLTIP_ROW_001023 //'\n' ������ �ڵ�����ǰ� ����.
    /*($Delete_[S/C]$)*/#define __NA_0_20080331_SHOW_MSG_FOR_DISP_QUEST_PROGRESS			// 2008.04.02 / ����� / "ShiftŰ�� ������ ����Ʈ ����Ʈ�� Ŭ���ϸ� ����Ʈ �����Ȳ�� ǥ�õ˴ϴ�" �޽��� ǥ��
	/*($Delete_[S/C]$)*/#define __NA000000_080416_WAREINVEN_LOCK_BUG_FIX__					// 2008.04.16 / ������ / ��� ������ �ֹι�ȣ �Է� �ܰ迡�� ����ص� ��� �����Ǵ� ���� ����
	/*($Delete_[S/C]$)*/#define __NA001051_080515_EVENT_EFFECT_RENDER_BY_SCRIPT_1stPROCESS__	// 2008.05.15 / ������, ������ / [�̺�Ʈ]2�ֳ� �̺�Ʈ ����Ʈ ��� �۾� - �ӽ� (���� ���� ȣȯ)

    // (WAVERIX) (090719) (NOTE) �ϱ� ��ũ�δ� ������ ����... �ϴ� �� ���ϸ� ���� ����
	//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE					// ����ö / GameGuild�� �����͸� SolarHash���� stdext::hash�� ����
	/*($Delete_[S/C]$)*/#define _NA_001029_20080428_PARTY_LEAVE_BY_GUILD_RELATION	// 200804.28 /����ö/ ��Ƽ ���� �� ��Ƽ �ʴ��� �÷��̾�� ��Ƽ�� ����߿� ��������� ��尡 �ִ��� üũ


	#if defined(__CN) || defined(_CHINA) 
		#define _GS_ADD_BIGEVENT_MSG_1122							// 2008.07.11 / �豤�� / �߱��ø����̺�Ʈ �޽������
        #define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE				// 20080717 / ����ö / ����г�Ƽ �߰��۾� (�߱���)

		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / ���ؼ� / �渮��Ʈ ����üũ���� ��� �����ֱ�. �ϴ� ������ ����.
		/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD				// 2008.06.09 / ����ö,������ /������ ����Ʈ �ڵ� �߰�
		/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER	// 2008.5.30 / ����ö / ���� ���� ���� Ŭ���̾�Ʈ ������Ŷ
		/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT	// 2008.06.13 / ���ؼ� / ����Ʈ ���ɰ���
		/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__			// 2008.04.02 / ������, ������ / Skill���� ��Ŷ ó�� ó�� ������ ���� �����۾� Version01
		/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__				// 2008.04.21 / ������, ������ / �Ϲ� ���� �˰��� ������ ���� ��Ŷ �м� �� ����
		/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__				// 2008.06.19 / ������ / PacketControl 3rd Server<->Server (Ư�� ��Ŷ�� ���ؼ��� �۾�)
		/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX		// 2008.06.17 / ���ؼ� / Ŭ������ ��Ÿ�� ����Ʈ ��� ����. ������ ��ȹ�Ǿ����� ���� ������ ����.
		/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY		// 2008.06.10 / �̼���, ������ / ������(����������) ���ݼ���(�Ϲ�.����.PK��) üũ ����
		/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / �̼��� / CHECK_ATTACK_PROPENSITY_MODYFY �۾��� ���� ��üũ �߰�.
		/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX					// 2008.06.26 / �̼��� / ����������� ������ų ������ ������ �Ǵ� ����.
		/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX				// 2008.06.30 / �̼��� / ����ī�� ������ ����Ʈ ���� �����ϰ�(��ũ��Ʈ����).[��ũ��Ʈ ����� ���� �߰� �Ǿ����]
		/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY					// 2008.07.01 / �̼��� / ī������ �ڵ����� �������ݵ��� �ʴ� ����.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK					// 2008.07.24 / �̼��� / Ÿ��üũ�� ����Լ��� ����ƽ���� ��� ���� ����.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_CHINA_PK_RULE						// 2008.07.24 / �̼��� / �߱� PK�� ����
        /*($Delete_[S/C]$)*/#define __NA_2001_20080617_BUGFIX_CLEAR_PARTY_WHEN_RECONNECT// 2008.06.18 / ����� / �� ���ӽ� ��Ƽ�� ���� ����
        /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED // 2008.07.04 / ����� / �巡�� ���� ���¿��� �����Ǿ� �ִ� �Ⱓ�� ������ �Ⱓ ����� �̻� ���� ����
        /*($Delete_[S/C]$)*/#define __NA_0_20080707_CLEAR_HERO_INFO_WHEN_SELECT_CHAR	// 20080707 / ����� / ���� ĳ���� ��� ��ũ�� ���ŵ��� �ʴ� ���� ����

	#elif defined(__JP) || defined(_JAPAN) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / ������ / Ű���� �̵� �� ���� �� ����ȭ ���� ��ƾ �Ϻ� ����

	#elif defined(__KR) || defined(_KOREA) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / ������ / Ű���� �̵� �� ���� �� ����ȭ ���� ��ƾ �Ϻ� ����
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / ���ؼ� / �渮��Ʈ ����üũ���� ��� �����ֱ�. �ϴ� ������ ����.
	#endif

//==================================================================================================
// (0.8.1.4) (Delete reserved working macros)
//
//==================================================================================================


#endif


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// 0.8.0.8 ���� ���� Define
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if NATION_BUILD_VERSION >= 808 

	/*($Delete_[x/C]$)*/#define __ARMORKIT_ENABLED_CHECK_BY_CMDnACK__			// 2007/01/11 Armorkit �۵����� üũ�ϱ� ���� ��Ŷ����
	/*($Delete_[S/C]$)*/#define __NA070212_ENHANCED_SECURE_POLICYMENT__			// 2007/02/12 ���� ��ȭ ��� ���� ����/Ŭ�� ó�� ��ƾ

#endif //NATION_BUILD_VERSION >= 808 


//��������������������������������������������������������������������������������������������������������������������������������������������������
// ���� �����ں� Define
//��������������������������������������������������������������������������������������������������������������������������������������������������

//��������������������������������������  ��� ����  ������������������������������������������������������������������������������������������������������������

//#define __NA_0_20061109_EXP_EVENT_								// 2006.11.09 / ���ؼ� / �÷��̽ð��� ���� ����ġ ���� �̺�Ʈ
//#define __NA_0_20070329_CHUNTING_FORMULA_BUG						// 2007.03.29 / ���ؼ� / �������� �������ġ ���� ���� ����
//#define __NA_0_20080618_STATE_INFO_SIZE_DECREASE					// 2008.06.18 / ���ؼ� / STATE_INFO ������ ���. PLAYER_RENDER_INFO�� ����� ����.

//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������


// 2006. 9. 13 / ���翵 / NPC�� ��ȭ�� NPC �� ���ƺ��ų� ���� �ʵ��� �ϰ�,�̴ϸʿ� ǥ�� ���ο����� ǥ��.(NPCExtraInfo.txt �ļ� ������.)
//#define __CN_269_MATRIX_CARD_AUTH

//��������������������������������������  â�� ����  ������������������������������������������������������������������������������������������������������������
//����� ��
//#define __NA_0_20070521_QUEST_CONDITION_RESULT_CHANGE	// 2007.05.21 / ��â�� / ����Ʈ ����� ��� Type�� BOOL���� enum���� ����(����׿�)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//��ȹ�� ���� �۾�
//#define WORK_RANKUP_ETC_ITEM							// 2006.09.19 / ��â�� / RankUp���� ���� ���� ���������ε� Ȯ���� �ø� �� �ִ� �ý��� - ��ȯ�� ������3 
//#define __NA_0_20080421_REBIRTH_NO_PARTY							// 2008.04.21 / ��â��, ���ǥ / ��Ƽ �ƴ϶� ��Ȱ �����ϰ�...
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//�۾�
//#define __NA_1217_EXTEND_MISSION_TYPE_COUNT						// 2009.02.06 / ��â��, �̼� Ÿ�� ���� Ȯ��
///*($Delete_[S/x]$)*/#define __NA_001358_ENCHANT_UNIFICATION							// 2009.06.22 / ��â��, ��þƮ �Ͽ�ȭ

//��������������������������������������     ��      ����������������������������������������������������������������������������������������������������������������

//��������������������������������������  JSCHO ����  ������������������������������������������������������������������������������������������������������������

//#define __WAVERIX_2006122x_SERVER_TYPE_CHANGE_POSTWORK__// 2006. 12. 2x / ������ / ���� Ÿ�� ���������ʰ� ���߱� ���� �۾� (��ũ�θ��� ���� ��Ģ�� �������� ������)
// �� ��ũ�� ����� ���������� ����(���� ������) �� �ּ��� �����ϸ�, ���ο� ����Ÿ������ ��ȯ;.

//#define	__NA_PACKETFLOW_TEST	
//#define		__TRIGGER_TEST_LOG
// ==> ���� ��� ��ũ���� �ؿܹ������� ����Ǿ� �ִ�.
//#define		__NA_000000_CHANGE_TABLE_DATA					// 2008.09.02 / ����� / ���̺����͸� �������� ��������.
//#define		__NA_001192_20081110_TICKET_LOG					// 2008.11.10 / ����� / Ƽ�� �α�.


//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//==================================================================================================
//====================================  WAVERIX ���� ���� ==========================================
//--------------------------------------------------------------------------------------------------
// debugging tools
//#define __NA000000_080317_ABNORMAL_EXIT_REDIRECT__                // 2008.03.17 / ������ / CRT Runtime Error Message Box ��� dump������ �ϴ� �۾� (���� Ȯ���� �ʿ��� ��쿡�� ����� ��)
//#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_USE_SHARED_POOL_  
//--------------------------------------------------------------------------------------------------
// NOTE: applied section, this working macros help reference search.
// a independent facility related to the item customizing content.
// this content can enables various outputs
// when a customizing item is pointed using an etherealizer item
// Keyword: _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
// Keyword: __NA002610_110428_REMOVE_INFINITY_USING_LUCKY // 2011.04.28 / ������ / ��Ű �ð� (����) �߱� ����
//--------------------------------------------------------------------------------------------------
// reserved contents for next version
//--------------------------------------------------------------------------------------------------
// specific contents for foreign version
//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_ // ������, �̵��� / ĳ��Ÿ��(PPī��) �߰��� ���� ���� �۾�
//#define __NA000000_070523_REPORT_TO_SOLARAUTH
//#define _NA000000_110218_RANK_REPORTER_IN_DBUNIT // 2011.02.18 / ������ / Ep2<-Ep1 ��ȯ�� ���ŵǴ� ��ũ ���� �����
//--------------------------------------------------------------------------------------------------
// pending list
//#define __NA000000_070928_PACKET_UNICAST_UPDATE__                 // 2007.09.28 / ������ / VarMsg2�� ó���Ǵ� �κ��� ���� ���� ó���� �� �ֵ��� ����
//#define __NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__  // 2008.01.22 / ������ / VALENTINE �̺�Ʈ�� ���� ĳ�ð��� �۾�
//#define __NA001243_090112_SKILL_ANIMATION_SYNC_CORRECTION__         // 2009.01.12 / ������,�豤�� / ��ų���ϸ��̼� ����ȭ ���� ���� �۾�
//#define _NA00XXXX_091123_SETUP_CASH_ITEM_AVAILABLE_PERIOD_ // 2009.XX.XX / ������, ? / ĳ�þ����� ��ȿ�Ⱓ ����
//#define _NA_0_20100211_SD_EFFECT_MODIFY          // 2010.02.11 / ��ȣ�� / 2Ÿ ���� ��ų ������ ������ ����.
//#define _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_    // ������ / ������ ��Ʈ�� ���� 8 �۾�
//#define _NA000000_VALUE_TYPE_SIMPLIFICATION // 2010.02.16 / ������ / ��ġ �Է� Ÿ�� ������ (1,2)�� ����ȭ �۾�
//--------------------------------------------------------------------------------------------------
// working contents
//#define _NA000000_CW_PROTOCOL_UNIFICATION // ������ / WorldServer<->Client �������� CG�迭�� ����, to change inter-communication
//#define _NA002676_WORLD_SERVER_RENEWAL // ������ / WorldServer �籸��
//--------------------------------------------------------------------------------------------------
//=====================================  WAVERIX ���� �� ===========================================
//==================================================================================================

//�������������������������������������� GS ����������������������������������������������������������������������������������������������������������������
//#define _GS_MOD_ITEMDURA_PENALTY_1062
//�������������������������������������� END       ����������������������������������������������������������������������������������������������������������������

//�������������������������������������� AEORAGIE ����������������������������������������������������������������������������������������������������������������
//#define __NA001126_NEW_SKILL_SCREEN_EFFECT // 2008.07.16 / ������, �豤�� / ��ų ��� ��, ��鸲 ȿ�� �߰�
//�������������������������������������� END       ����������������������������������������������������������������������������������������������������������������

//�������������������������������������� SHAKETAKE ����������������������������������������������������������������������������������������������������������������
//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE
//#define __NA_001169_20080930_GUILD_LOG_SYSTEM

//#define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE
//#define SHAKETAKE_DATA_CHECK
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������


//��������������������������������������     �߷� DEFINE      ������������������������������������������������������������������������������������������������������
//#define __NA_000571_20070702_WORLD_UNIT_PARTY_2ND			// 2007.07.02 / Ȳ���� / ������� ��Ƽ 2�� �۾�
//#define __NA_000313_20070223_REALTIME_CHAT_BLOCK			// 2007.02.23 / Ȳ���� / �ǽð� ä�� ��

//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  gamelulu(������)  ������������������������������������������������������������������������������������������������������
//#define __[�����ڵ�]_[�۾���ȣ]_[��¥]_[�۾���]_
//#define Debug_Domination                                          // ���� ������ �α� ���� define       
//#define _NA_20091014_ZEROPOINT_RANKING_UNACCEPT                   // 2009.12.14 / ������ / ����Ʈ ��ŷ ���ÿ� 0����Ʈ�� ������� �ʴ´�.
//#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS             // 2010.04.15 / ������ / ����Ʈ ��ŷ ��� ���� ���� Mission Key => RegisterDate
//#define _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC                     // 2010.04.20 / ������, ������ / �ý��� ���� ���� ����
//#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION
//#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END        // 2010.06.21 / ������ / ���� ������ �̺�Ʈ �߰�
//#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / ������, ������ / �ý��� �޸� ���� �۾�(����������)
//#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE             // 2010.09.01 / ������, ������ / �ý��� �޸� ���� ���� �α� �߰�
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / ������ / �׷� ���ʽ� ���� ��� ��,���� ��� ���� ����
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / ������ / ���� ������ �̺�Ʈ ����
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / ������ / �̺�Ʈ �ӽ� Ÿ�̸�
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / ������ / �Ⱓ Ȱ��ȭ ������ �ŷ� �Ҵ�
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / ������ / �׷� ���ʽ� ���� ��� ��,���� ��� ���� ����
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / ������ / �Ⱓ Ȱ��ȭ ������ �ŷ� �Ҵ�
//#define _NA_20100604_BETTER_DOMINATION_MANAGER                    // 2010.06.04 / ������, ������ / ���� ���� ���� ��� ����
//#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                  // 2010.04.20 / ������, ������ / ���� ������ ���� ��� ����
//#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                // 2010.12.14 / ������, ������ / ������ ���� ��� ������ ���� ��� ������ üũ ��� ����
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / ������ / ���� ������ �̺�Ʈ ����
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / ������ / �̺�Ʈ �ӽ� Ÿ�̸�
//#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT       // 2010.01.16 / ������, �̵��� / ������ �溮 ������ �߰�
//#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                          // 2011.03.03 / ������, �̵��� / Ÿ�Ժ� NPC ���� ��� �߰�
//#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN              // 2011.04.06 / ������, ������ / ������ ���� ��� ����
//#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO                 // 2011.06.10 / ������ / ���� ������ ü�� ���� ini���� ����(ServerOptionEX.ini -> GameOption.ini)
//#define _NA_20110630_BATTLE_ZONE_RENEWAL                          // 2011.06.30 / ������, �̵��� / ��Ʋ�� ������
//#define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS             // 2011.12.20 / ������, ������ / ��Ʋ�� ������ ������ ���� ���� ���� ó��
//#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN               // 2010.01.06 / ������ / ���� ��ǥ ��ȯ���� �ʵ��� ����
//#define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST          // 2012.05.11 / ������ / ���� ������ �߰� ��û ����
//  
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  JAYKANG  ��������������������������������������������������������������������������������������������������������������
//#define _NA_20101011_HEIM_LOTTO_CASH_TICKET                             // 2010.10.11 / ������ / ĳ�� ���������� ������� ���� ���� ȹ�� ��� : _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE�� �Բ� Ȱ��ȭ�� �� ����
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������


//��������������������������������������  NAMKA  ��������������������������������������������������������������������������������������������������������������
//#define _CN_0_20091013_RANKUP_EXTEND                          // 2009.10.13 / ����� / ��ũ�� ����. ������ ��, �ʱ�ȭ�� �� �߰� �� ��ũ���� �ٴ� �Ӽ� �ɼ� ����
//#define _NA_0_20100325_QUICK_SLOT_CHANGE                      // 2010.03.25 / �����,�̵��� / Quick ���� ���� (48���� ����->40���� �������� ����)
//#define _NA_0_20091208_VENDOR_ITEM_SEARCH                     // 2010.01.20 / �̵���, ����� / �����˻� �ý���
//#define _NA_0_20100416_RELOAD_MAPOBJECT                       // 2010.04.16 / �����, ������ / �ʿ�����Ʈ ���� ���ε�
//#define _NA_0_20100519_GMCOMMAND_MOVEFIELD                    // 2010.05.19 / ����� / GM ��ɾ� �߰�. ������ ����Zone�� Ư�� �ʵ�� ���� �̵�
//#define _NA_0_20100519_GMCOMMAND_CREATEMISSION                // 2010.05.19 / ����� / GM ��ɾ� �߰�. ������ ����Zone�� Ư�� �ʵ�� ���� �̵�
//#define _NA_0_20100528_ACCUMULATE_POINT_EVENT                 // 2010.05.28 / �����,������ / ���� ����Ʈ ���� �귿�� Ư�� [����/�ð�] ��� ������ ȹ�� �̺�Ʈ �߰�
//#define _NA_0_20100901_SKILLTREE_SYSTEM                       // 2010.09.01 / �����, / ��ųƮ�� �ý��� �߰�
//#define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION          // 2010.10.11 / �����, / ���� ����Ʈ ���޽� ����/�ִ밹�� �� �����ϴ� �ɼ� �߰�.
//#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL               // 2011.01.31 / �����,����� / ��ųƮ�� �ý����� ��ų�� ���� ���� ��� ����.(��ų �⺻ ���� 0��ų -> 1��ų).
//#define _NA_0_20110305_BUG_READ_BIG_LOGFILE                   // 2011.03.05 / ����� / 2GB�̻��� �α� �б� ���� ����.(���� �ڵ� �б� �Լ��� ����)
//#define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / �����, ����� / ĳ���� �����ӽ� ��ų ��Ÿ�� ���� ��� �߰�.
//#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                 // 2011.03.18 / �����, / ���� ������ �Ϸ� �Ⱓ ���� �߰�.
//#define _NA_0_20110329_GAMEOPTION_SETTING                     // 2011.03.29 / �����, �����/ ���� �ɼ��� DB ����
//#define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / �����, �����/ ĳ�ü� �����丵.
//#define _NA_0_20110803_PLAYER_MOVEMENT_SYNC                     // 2011.08.03 / �����, �÷��̾� �̵� ����ȭ.
//#define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / �����, ���ӷα� �뷮�� ���� ���Ϻи�.
//#define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / �����, �ζ� ������ ��밡���� �÷��̾� ���� ����.
//#define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / �����, �����, ���� Ŭ���̾�Ʈ�� MAC �ּ� �α� �����.
//#define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / �����, / ��Ƽ �ο����� ���� ���� ����� ����.
//#define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / �����,����� / ���콺 �̵��� �̵� ��� ���� ���� (���� 64 -> 512)
//#define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / �����,����� / ������ 0�� ���⵵ ���� ���� ������� �ı��ǵ��� ��� ����.
//#define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / ����� / ��Ƽ�� �ο����� ���� ���� ����ġ ����.
//#define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / �����,������ / �巡�� ����Ʈ�� �뺯�� 1,2 ���� ��ų ���� ��� �߰�.
//#define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / ����� / ��ȣȭ ��� MD5 -> SHA256 ����.
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������


//��������������������������������������  YJ  ������������������������������������������������������������������������������������������������������

//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  ������  ��������������������������������������������������������������������������������������������������������������
//#define _NA_20100307_BETAKEY_SYSTEM                                   //��ŸŰ �ý��� (������, �̵���)
//#define _NA_20100322_AGGRO_DISPLAY                                    //��׷� ��ġ ȭ�鿡 ǥ�� (������, �̵���)
//#define C_NA_0_20100520_ACSYSTEM_CHANGED                            //2010.05.20 ������ ������ AC �ý����� ���������� �����Ѵ�.
//#define _JP_0_20101123_PMS                                          //�Ϻ� ���ӿ��� ������ ���� �ý����� PMS ����
//#define _JP_20110222_INGAME_CHARNAME_CHANGE                         //�Ϻ��� ���� ������ ���� ĳ���߸� ������ ���ӳ����� �������ش� (������ ĳ���� ������ �ȵȴ�.)
//#define _RU_20110801_SEND_GIFT_FRIEND_CHECK //ģ������ ������ ������ ���� ������ ģ������ �ϰ� ���浵 ���� ģ������ �Ѵ�.
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//--------------------------------------------------------------------------------------------------
// hukim(������) ����
//
//#define _NA_000068_20100419_PARTY_FINDING_SYSTEM // 2010.04.19 / ������, �̵��� / ��Ƽ ã�� �ý���
//#define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / ������, �̵��� / ���� ������ ���� ����� ���̵� �ý���
//#define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / ������, ����� / ���� ������ ����
//#define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / ������, ����� / �ֻ�� ��þƮ
//#define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / ������ / SD ���� ���� ����
//#define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / ������ / ��Ű�� ���ݷ� ���� ����
//#define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / ������ / ���Ӽ��� ObjectFactory ����
//#define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / ������ / ObjectKey �뿪�� Ȯ��
//#define _NA_004035_20120227_COSTUME_ITEM // 2012.02.27 / ������, ����� / �ڽ�Ƭ ������
//#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / ������ / ���� �ɼ� Ʈ����� ó�� ��� �߰�
//#define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / ������, ����� / 1202 ��ų ������
//#define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / ������, ����� / ��ų ���ݿ� ���׸� źȯ ����
//#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / ������, ������ / ������ ���� �ɼ� ���� ��� ����
//#define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / ������, ������ / �ű� ĳ���� �̽�ƽ �߰�
//#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / ������ / �ű� ĳ���� Ŭ���̾�Ʈ ����(��ũ��Ʈ�ļ�����)
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / ������ / �ű� Ŭ���� Ÿ�� �߰��� ���� Ÿ��MAX ������ ���� �߻��Ǵ� ���� �ӽ� ����
//#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
//
// hukim(������) ��

//��������������������������������������  chief85(������)  ������������������������������������������������������������������������������������������������������
// chief85(������) ����
//#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                                   // 2012.04.12 / ������ / ��Ʋ�� ���ȣ�� Ŀ�´�Ƽ ȭ�鿡 ǥ��.
//#define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE                   // 2012.04.13 / ������ / ��Ʋ�� �� ���� �� ��й��� ���忩�� �޼��� ������ ����.
//#define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST                     // 2012.04.16 / ������ / ���� ����� ����� ��Ʋ�� �� ����Ʈ�� �����Ѵ�.
//#define _NA_004791_20120430_1202_SUMMON_STATE                                     // 2012.04.30 / ������ / ��Ű�� ��ȯ���� Ÿ���� ���� �����ϴ� ����, �̹� Ȱ��ȭ�� Ŀ�ǵ� �� ����.
//#define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE                      // 2012.05.11 / ������ / ŸŹ�� ��� �߾� �� ���� ���Ƽ ���� ���� �� ���� �� ��� ���� �� PVP����.
//#define _NA_004035_20120227_COSTUME_ITEM                                          // 2012.05.23 / ������, ����� / �ڽ�Ƭ ������
//#define _NA_004910_20120601_SUMMON_PVP_RULE                                       // 2012.06.01 / ������ / ��Ű�� ��ȯ�� SD(99):HP(1) �� ����.
//#define _NA_000000_20120607_INVENTORY_SORT                                        // 2012.06.07 / ������ / �κ��䷯ ����.
//#define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET             // 2012.06.14 / ������ / ��ȿ��<����� ���� ����������>
//#define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY                        // 2012.06.21 / ������, ������ / ��� ������ ���Ѽ� ���� 99% -> 80%
//#define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE                   // 2012.06.22 / ������ / ��Ű�� ��ȯ�� PVP���� ���ΰ� ���� �� ����.
//#define _NA_005028_20120627_MODIFY_IMPOSSIBLE_CAST_HIDE_SKILL_IN_SSQ              // 2012.06.27 / ������ / Ÿ���� ������� ������ ���̵� ��ų ���Ұ���.
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                              // 2012.06.27 / ������, ������ / ī���� �� ���� �ý���.
//#define _CN_006014_20120822_GUILD_LOG_DELETE                                      // 2012.08.22 / ������ / �߱� ���α׿� ������ǥ �������� �α� ����.
//#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA             // 2012.08.23 / ������ / ������� �Ϸ� ��� �ִ� ���� ��ũ��Ʈ �����ͷ� ����
//#define _NA_006040_20120830_RESIS_BAD_STATUS_RATIO                                // 2012.08.23 / ������ / ���� �� ���� ��ų ���� ��ų����Ȯ���� ����Ǵ� ����
//#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP                                // 2012.09.04 / ������ / ������ �ٴڹ����� �Ұ���, PK��Ӱ��� Ÿ�� �߰�.
//#define _NA_006057_20120904_PK_DROP_RATIO_MODIFY                                  // 2012.09.04 / ������ / �ڽ��� ���� ���� ���� �÷��̾ �׿��� ��� ���� �÷��̾��� �����۵���� ���� ���Ѽ� 100%->50%
//#define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP                                   // 2012.09.10 / ������, ������ / ī���� �� �������� �ý���.
//#define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY                              // 2012.09.25 / ������ / ���׸�źȯ ��å ����(�⺻���ݷ¸� -> ��ü������)
//#define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY                   // 2012.09.26 / ������ / ��Ʋ�� ���忹�� �켱���� ����. (���� -> �ֽż�)
//#define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY   // 2012.10.12 / ������ / PVP�뿡�� ��Ƽ����â�� ������ġ�� �߸� ǥ�� �Ǵ� ���� ����
//#define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD                    // 2012.10.15 / ������ / ��Ʋ�������� �ʵ弭���� �̵��� ����� �ʵ嵵 �����ϵ��� ����
//#define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY                 // 2012.10.17 / ������ / ���� �� ����迭 ��ų ��ũ�� ���� ����
//#define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION                      // 2012.11.01 / ������ / ������������ �����ϴ� ���� ���ݺҰ�����(����, �ٿ�..)�� �Ǿ ��ų�� ��� �����Ǵ� ����
//#define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD               // 2012.11.16 / ������ / ť��ý��� ���ؽ� ���󸮽�Ʈ���� ĳ����� ����
//#define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME                // 2012.12.11 / ������ / ���� ��������Ʈ ���� ��� �ð� ����
//#define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY          // 2012.12.28 / ������ / ��ȯ���� ������ ��ö����� ��� ����,�ǰ�,��ų��� ���� -> �Ұ������� ����
//#define _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER                              // 2013.01.03 / ������ / �����̺�Ʈ Ʈ���� �߰�
//#define _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM                      // 2013.01.04 / ������ / team check condition Ʈ���� �߰�
//#define _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD                       // 2013.01.04 / ������ / [����/��ü]���� [HP/MP/SD]�� [��ġ]��ŭ [����/����]�� ���� action Ʈ���� �߰�
//#define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG                                 // 2013.02.25 / ������ / ��Ű���� ��ȯ���� ��ȯ �Ͽ� ���͸� ����, ��ȯ���� ĳ���� ��ó�� �̵� �� �� ���� ĳ���͸� �ڷ� �̵���ų ��� ���Ͱ� �ٺ� �Ǵ� ����
//#define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD                                  // 2013.03.13 / ������ / ���Ǽ������� ����ũ����Ż�� ��ȯ �� ĳ������ �� �Ǳ��� ��ȯ�� ������ �׾ ũ����Ż�� ��ȯ�Ǵ� ����
//#define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG                                    // 2013.03.19 / ������ / �巡�ﳪ��Ʈ�� �������¿��� �뺯�� �ȵǴ� ���� ����
//#define _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM                              // 2013.03.20 / ������ / ���� �߰��۾� #1 <���� �ð� ���� �ý��� �߰�>
//#define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY                              // 2013.03.25 / ������ / ���� �߰��۾� #2 <���� ���� ��Ȳ �˸�>
//#define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG                           // 2013.03.28 / ������ / ��ȯ��(��Ű��)�� ���� �� ��ų��Ÿ�� ���� ���°� ��ȯ������ ������ �ȵǴ� ����
//#define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY                          // 2013.04.01 / ������ / �̼� �� ���忡�� ��ȯ���� ������� �����͸� �������â�� �ݿ� (������ ���� ������, ��ȯ���� ų��, ����)
//#define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO       // 2013.04.01 / ������ / ��Ű�� ��ȯ���� �� ��ų�� ��ȯ��(����)�� ��ų���ݷ� ������ �°� �߰� ������ ������ ��������
//#define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL                  // 2013.04.09 / ������ / ���� ��ö ����� ��� ���� ������ ��ų�� �����Ǵ� ���� ����.
//#define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME                           // 2013.04.02 / ������, ������ / GM��ɾ�� �̺�Ʈ ���� ����
//#define _NA_006589_20120415_ENCHANT_PRICE_MODIFY                                  // 2013.04.15 / ������ / ��þƮ ��� ���� ����
//#define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO                        // 2013.04.15 / ������, ������ / Ÿ��â ������ ���� ǥ�� �߰�
//#define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY                             // 2013.04.16 / ������ / Ŀ�´�Ƽâ �������� ������ �߸� ǥ�õǴ� ����
//#define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS                      // 2013.04.22 / ������ / �뺯�� ���¿��� ũ����Ż�Ӹ��� ������ Ȱ��ȭ�� �����ϰ� �ִ� ��� ������� ���� ����
//#define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY                        // 2013.04.22 / ������ / �̼� Ʈ���� �߰� �� �����۾�
//#define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY                          // 2013.04.26 / ������ / ��Ű�� ��ȯ�� ��ų ��ũ ����
//#define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY                            // 2013.04.26 / ������ / ������ �ɼ� �߰� ����
//#define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME                            // 2013.05.02 / ���ö, ������ / ���� �ɼ� �߰�(�ڽ�Ƭ ���̱� �ɼ�)
//#define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION          // 2013.05.06 / ������ / ���� ������ �ʵ� ���� ���� �Ұ�
//#define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION   // 2013.05.08 / ������ / ���� ������ ���� ���� �Ұ�
//#define _NA_006710_20130509_CRYSTALWARP_BUG                                       // 2013.05.09 / ������ / �̹� �ı��� ũ����Ż�� �������� ���� ��� ������ ���� ũ����Ż�� �����ϴ� ����
//#define _NA_006728_20130520_SKILL_BALANCE_MODIFY                                  // 2013.05.20 / ������ / ��ų �뷱�� ����( �ݻ���� ��ų ���� )
//#define _NA_006731_20130521_ENCHANT_ADD_OPTION                                    // 2013.05.21 / ������, ������ / �ֻ��(13~15)��þƮ �ɼ� �߰�
//#define _CH_006797_20130703_PK_DROP_RULE_MODIFY                                   // 2013.07.03 / ������ / �߱� �ʵ� PK ��å ����
//#define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM                             // 2013.07.03 / ������, ������ / ���� ��ǥ �ý��� �߰�
//#define _NA_006826_20130722_DOMINATION_RENEWAL                                    // 2013.07.22 / ������, ������ / ���Ǽ��� ������ ����
//#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG                               // 2013.07.31 / ������ / �߱� ��ö��� ���� ���� �αױ��
//  
// chief85(������) ��
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  �ΰ��  ������������������������������������������������������������������������������������������������������
// �ΰ�� ����
//#define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / �ΰ�� / ac���� �߰�(���Ǹ� ������)
//#define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / �ΰ�� / ���忡�� �� óġ ��, �߰� ����
//#define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / �ΰ�� / �̵�+������ ��ų�� 1���� �����Ը� ���ظ� ��
//#define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / �ΰ�� / �ַκδ������ Ŀ�ÿ�������� �Բ� ���� �� �ֵ��� ��
//#define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / �ΰ�� / '�̼ǿϷ�', '����¸�' ����ƮŸ�� �߰�
//#define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / �ΰ�� / ���忡 '��ý�Ʈ' �߰�
//#define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / �ΰ�� / ���� ������ �켱���� ����� ����
//#define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / �ΰ�� / ���������� �ɷ����� ���,�������� ���� ��� ����
//#define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / �ΰ�� / �����ƼŸ���߰�(345), ��ų���ݷ�, ���ȿ���� ��������ʴ� HP����
//#define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM               //2013.07.05 / �ΰ�� / ��ŷ�ý��� ������ �� ���巩ŷ �߰�
//#define _RU_006860_20130806_COSTUME_PCBANG_EFFECT                //2013.08.06 / �ΰ�� / �ڽ�Ƭ�� �ǽù� �ɼ� �����Ű��
//#define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / �ΰ�� / ��ŷ�ý��� ������ �� ���巩ŷ �߰�(���̳�)
//#define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / �ΰ�� / �߱� PK���� �����, ������迡���� �������� ����ȴ�.
//#define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL           //2013.10.30 / �ΰ�� / ����̵� ��ų���� ũ��Ƽ�� �����Ƽ ���� �߰� �� ����
//#define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / �ΰ�� / �̼ǳ����� ������ ���� �� �������ϸ�, �̼� �� ����� ��ġ�� ��ġ�ȴ�.
//#define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / �ΰ�� / �㿡 KISS�� ���δ�. ���� �� �����ڼ��� �����Ѵ�.
//#define _NA_000000_20140103_CHANGE_DASH_AND_PIERCE_SKILL_MOVE     // 2014.01.03 / �ΰ�� / �������뽺ų�� �̵�ó���� DamageAbility�� �ƴ�, QuickStep�����Ƽ�� ��ü�Ѵ�.
//#define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / �ΰ�� / ���Ͱ� ��������� ���� '��Ȳ'���°� �ƴ϶� '����'���·� ��ȯ�ϵ��� �Ѵ�.
//#define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / �ΰ�� / thrustState(�˹����)���� �񼱰� ���Ͱ� �ݰ��� ���� �ʴ� ���� ó��
//#define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / �ΰ�� / ������ ������, �޸��Ҵ翡�� ������ �״� ������, ������ƮǮ�� �޸��Ҵ��� �ٲ㺻��.
//#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / �ΰ�� / �ؽ�Ʈ�αװ� �Ϸ�������� �ð������� ������ �����ϵ��� ����
//#define _NA_007053_20140210_BUG_SUMMONED_MONSTER_ATTACKSPEED      // 2014.02.10 / �ΰ�� / ��ȯ���� ���ݼӵ��� �������� ���� ����
//#define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / �ΰ�� / ��� ��, �� �������� 0�̸� ����Ȯ���� ��� �Ҹ��ϴ� �г�Ƽ ����
//#define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / �ΰ�� / �߱� ��ö��� ���� ���� �αױ��
//#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / �ΰ�� / ������0�̳� ���Ⱥ�ȭ�� ���� ��Ȱ��ȭ ��, ���⸦ ���������� ���·� �����.
//#define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / �ΰ�� / �����������Ʈ�� '�Ϸ�'�ߴµ� ������ �������� ���¿��� �����׷�����Ʈ�� �޴� ���� ����
//#define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / �ΰ�� / �ű� ������ ��巯��
//#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / �ΰ�� / ��Ī�ý��� �����丵
//#define _NA_007112_20140312_MODIFY_PROCESSRECEIVEDPACKET            //2014.03.12 / �ΰ�� / ProcessReceivedPacket�Լ����� ��Ŷó�� ���н� false���ϵǴ� �κ� ����
//#define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / �ΰ�� / ������ ��ų ������
//#define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / �ΰ�� / ������0�� ������ ��������
//#define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / �ΰ�� / �������� ���������� ���������ĺ���
//#define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / �ΰ�� / GM��ɾ� �߰�/����
//#define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / �ΰ�� / ģ���ʴ� ������ �������޽�, �������� const.h�� ������
//#define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / �ΰ�� / DB�α� ������α��
//#define _NA_007239_20140521_BUG_GUILDRELATION_COUNT                          // 2014.05.21 / �ΰ�� / ��������� �ִ�ġ ����
//#define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / �ΰ�� / ���������� ���� ��� ��, ItemNum�� �κ��丮 �� ������ �ƴ�, ���ռ����� ������ �Ѵ�.
//#define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / �ΰ�� / ��Ű����ȯ�� �ɷ�ġ ���� ���� ����
//#define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / �ΰ�� / ���ºο� Ʈ���Ű� �ʿ�����Ʈ���� �ٿ������� �۵��Ѵ�
//#define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / �ΰ�� / ĳ�ü��� ������ ����, �������� DB�α� ����ϵ��� �߰�
//#define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / �ΰ�� / ��巯�ÿ��� �������Ʈ ���������� DB�α׿� �����
//#define _DEV_007390_20140703_AUTH_EXCEPTION_VERSIONCHECK               // 2014.07.03 / �ΰ�� / ����,�系�������� �������� ����üũ ������ �����Ų��.
//#define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / �ΰ�� / ���������� �߰�
//#define _NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT                       // 2014.07.10 / �ΰ�� / AC ���డ��Ƚ�� �ø���
//#define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / �ΰ�� / ������ᰡ ����� ��� '��æƮ'�� ���õ� ���� ����
//#define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / �ΰ�� / ����ȭ �ٲٱ�
//#define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / �ΰ�� / DB���α� �۾�
//#define _NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN                       // 2014.09.16 / �ΰ�� / �̻���� ���۴ٿ� �߰�
//#define _NA_007667_20141001_WITCHBLADE_SKILL                            // 2014.10.01 / �ΰ�� / ��ġ���̵� ��ų�۾�
//#define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION                   // 2014.10.27 / �ΰ�� / ��� �ο��� ��������� �˻縦 �ϴ� ��Ÿ�� �߰�
//#define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / �ΰ�� / ��Ŷ������ ���� AC���� ���ѹޱ⹮�� ó��
//#define _NA_007827_20141117_ADD_ACCESSORY_TYPE                              // 2014.11.14 / �ΰ�� / �Ǽ��縮 Ÿ�� �߰�
//#define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / �ΰ�� / ��Ƽ���¿��� ĳ���� ����(HP,MP,SD,FP) ������Ʈ ��� ����
//#define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / �ΰ�� / �굵 �Ҹ�Ÿ�� 11���� �����ֵ����Ѵ�
//#define _NA_007909_20141218_SUNLOG_QUESTLIST                                  // 2014.12.18 / �ΰ�� / �ؽ�Ʈ�α׿�, ����Ʈ ����ޱ� �����ϸ� �ڽ��� ���� ����Ʈ����Ʈ �ڵ� ������
//#define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / �ΰ�� / GM��ɾ�, ����ġ�̺�Ʈ �����ϱ�
//#define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                                // 2015.01.15 / �ΰ�� / ���带 �ռ��ص� ������� �������� �������� ����
//#define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / �ΰ�� / ����ħ�� �̺�Ʈ�Ŵ����� ����
//#define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / �ΰ�� / ũ����Ż�緹��,���������� �ð������ ����� �����������ʴ¹���
//#define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / �ΰ�� / �÷��̾�� �ο򿡼� �ѹ濡 �״°�� üũ�ϰ� �α׷� ����Ѵ�
//#define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / �ΰ�� / ��Ʈ���ο� �ؽ�Ʈ�α�, �ѹ浥�������� ���ÿ�
//#define _NA_008069_20150210_ADD_SPA_SYSTEM                                       // 2015.02.10 / �ΰ�� / ��õ�ý��� �߰�
//#define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / �ΰ�� / ������AC�� ��� '��������' AC�� ���� �˻��ؼ� ó���ϵ����Ѵ�
//#define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / �ΰ�� / ������ �̵����� ���� ����
//#define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / ���̽��ҵ尰�� ���������� �����ϴ� ��ų�� ����źȯ�� ������ ���� ��꿡 ������ �߻���Ų��
//#define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / ���Ͱ� �������뿡�� ��ȯ��Ʈ�� ��ã����� �ڷ���Ʈ���ѹ�����
//#define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / ��ũ�������Ʈ���� 'Effect'�� ����ϴ� ��ų�� ����ڰ� ������� ���� ��������� �Ѵ�
//#define _NA_000000_20150507_TIME_DISPLAY                                              // 2015.05.07 / �ΰ�� / agentserver, �����̽��� ������ �ð����� ����
//#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / �ΰ�� / ����Ʈ���װ����۾�, db���ν����� ��Ŷ�� ��ũ����
//#define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / �ΰ�� / �뷱���۾��� ���� Ŭ�� �������� �����ϱ�
//#define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / �ΰ�� / ����ǵ� ȿ���� �������� ���������Ѵ�
//#define _NA_008334_20150608_SONNENSCHEIN                                                  // 2015.06.08 / �ΰ�� / ��Ʈ���� PvE, �ҳٻ���
//#define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                                       // 2015.07.03 / �ΰ�� / �ð��� ������� ����� ��, charge sub type 1���� �����ϵ��� ����
//#define _NA_008456_20150819_ETHER_BULLET_PVP_CHECK                                        // 2015.08.19 / �ΰ�� / ���׸�źȯ�� PvPüũ���Ͽ� PvP�� ������ ���� �ʵ��� �Ѵ�
//#define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                          // 2015.09.04 / �ΰ�� / ����̵彺�� ���¿��� BACKSTEP�̵��� ����޸���� ���� �ӵ��� ������
//#define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                                        // 2015.09.09 / �ΰ�� / ���Ǹ�������, �˱� Ǯ �������¿����� ��ų��� ����
//#define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT                               // 2015.09.09 / �ΰ�� / ���� 300���� ����
//#define _NA_008483_20150910_REMOVE_DAYLIGHT_TIME                                          // 2015.09.10 / �ΰ�� / �������� �ϱ� ���� �ð� 0���� �����ع�����
//#define _NA_008486_20150914_TOTAL_BALANCE                                                 // 2015.09.14 / �ΰ�� / �뷱���۾�
//#define _NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER                                      // 2015.09.23 / �ΰ�� / ���Ӽ������� ����ϴ� �α׾ƿ� �α� �߰�
//#define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                                       // 2015.10.01 / �ΰ�� / �����ڱ������ ����, ������ �����˸� ����ʵ��������� �������ϱ�
//#define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                                           // 2015.10.27 / �ΰ�� / ����Ʈ4 �ɼ� �߰� �۾�
//#define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                                           // 2015.11.05 / �ΰ�� / �̼Ӱ��� ����
//#define _NA_000000_20151109_BUG_SSQ_HANDLER_NULL                                              // 2015.11.09 / �ΰ�� / ssq handler ���� ����
//#define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                                            // 2015.11.16 / �ΰ�� / ���λ��� �˻� ��, �������� ����
//#define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME                                 // 2015.11.26 / �ΰ�� / �ڽ�Ƭ������ �Ⱓ�������� ����
//#define _NA_008606_20151222_MODIFY_GUILD_LEVEL                                              // 2015.12.22 / �ΰ�� / ����� Ż�� ��, �����ٿ� ���� �ʵ��� ����
//#define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / �ΰ�� / �� �ڽ�Ƭ���� �߰�(�����ڽ�Ƭ)
//#define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                                      // 2015.12.02 / �ΰ�� / �̵���ų ���� ��ġ����
//#define _NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION                                    // 2016.01.29 / �ΰ�� / ��Ʈ������ �ɼ� �������� ����
//#define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST                                       // 2016.02.04 / �ΰ�� / �������� ��Ʈ���ȿ���� ���� ������ѵд�
//#define _NA_008706_20160229_TEXTLOG_PK                                                      // 2016.02.29 / �ΰ�� / ���ô�� pk�� �ؽ�Ʈ�α�
// �ΰ�� ��
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  ��ȣ��  ������������������������������������������������������������������������������������������������������
// ��ȣ�� ����
//#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION												// 2015.10.29 / ��ȣ�� / ���� ��� �̼� ��� ����
//#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE												// 2016.01.15 / ��ȣ�� / NPC ���� ��� ���� ����
//#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2											// 2016.02.15 / ��ȣ�� / �巡�ﳪ��Ʈ �� ����(�ϵ巡��Ʈ������10����) ��ų ��� �� �̼� �ɷ�ġ ���� �ȵǴ� ���� ����
//#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS												// 2016.02.24 / ��ȣ�� / Ÿ�� ��ü�� ���� ��û(��Ÿ���ڵ� �߰�)
//#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE									// 2016.03.14 / ��ȣ�� / ���� ������ HP/SD ��ȯ �� ���� �߰� �۾�
//#define _NA_008765_20160405_DISCONNECTION_REASON_LOG												// 2016.04.05 / ��ȣ�� / ���ؼ� ������ ��� ������ �α׿� ����� �۾�
//#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE									// 2015.03.29 / ��ȣ�� / �߱� AC ���� ����Ʈ ������ ���� �ŷ�Ÿ�� �߰�(�ŷ����� + PK��� ����)
//#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / ��ȣ�� / ��ȣ���� �̻����� ����
// ��ȣ�� ��
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//��������������������������������������  �ӻ��  ������������������������������������������������������������������������������������������������������
// �ӻ�� ����
//#define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.13 /�ӻ�� /(��Ʈ����) domination ��ũ��Ʈ �� �ļ� ����, domination type �߰�, ���� ������ �� ���� �߰�, Map npc ������Ʈ ü��ȸ��
//#define _NA_000000_20140402_MODIFY_PERKMANAGER_POSSIBLE_TO_USE_WHILE_BATTLE_GROUND_RESERVATION //2014.04.02 /�ӻ�� /���忹�� �߿��� Ŀ���� �нú� ��ų ������ �����ϵ��� ����.
//#define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / �ӻ�� / ��Ʈ���� ������ �ʱ�ȭ �� ���� ���� ����
// �ӻ�� ��
//��������������������������������������     ��      ������������������������������������������������������������������������������������������������������������

//--------------------------------------------------------------------------------------------------
//! 2011.3.4 / i4u4me / ����� ����
//#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / ����Ʈ2 Ǯ�� ��Ʈ ��ȯ ���Ŀ��� �и� 

//#define _NA_000000_20120410_SPEEDHACK
//#define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
//#define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    //!< �׾����� �ɷ�ġ �ö��� �ʰ� ��
//#define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING                   //!< ���� ���� ����
//#define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER           //!< ����� �̼ǳ� ä�� ��� Ʈ���� �߰�
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                  //!< ����

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// arycoat :
//#define _HARDMODE_TRIGGER_PRINT_ // �ϵ��� ��ũ��Ʈ �ۼ������ Ʈ���� �м� ���� ������. - arycoat.2011.12.06
//#define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / ������,������ / ��� ���� �۾�
//#define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / ������,������ / ��� ������
//#define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / ������,������ / ���â�� �� ����â�� ���԰��� Ȯ��
//#define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / ������,�̵��� / ���λ��� �˻� �ý���
//#define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / ������ / ���� ������
//#define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / ������ / �����ǵ� MP�Ҹ� ����
//#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / ������ / ���������� �й� �г�Ƽ ����
//#define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / ������ / ��������Ʈ �ʱ�ȭ ������
//#define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / ������,������ / ������NPC
//#define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // ������,������ / ���͵���



//�������������������������� LAST SECTION(Shared) - Dependency Chain ����������������������������������������������������������������������������������������������
//	<SAMPLE>
//		#if defined( A_EX ) && !defined( A )
//			#error "Impossible Build!! - Reason: Violation Dependency Chain"
//		#endif
//
//	<EXPRESSION>
//		{ ��(A_EX) | (A') �� (A_EX) }   <--   A_EX ����� A��ɿ� �������̴�.
//		if (0) then IMPOSSIBLE,
//			(A') = (A) + (A_EX)
//			(A) = (A') - (A_EX)
//			(0) = (A_EX) - (A')
//		
//	<USEFUL SECTION>
//		1) A����� Ȯ��
//		2) A����� ����� �߰� ���
//

#if defined(_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL) && !defined(_NA_0_20100901_SKILLTREE_SYSTEM)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_004530_20120315_1202_SKILL_RENEWAL) && !defined(_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100528_ACCUMULATE_POINT_EVENT) && !defined(_NA_1419_20100121_ACCUMULATE_POINT)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_KR_0_20091021_FIRENDINVITATION_EXTEND) && !defined(_NA_0_20091104_FIRENDMAP_REFACTORYING)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_YJ_SHADOW_QUEST_S00023) && !defined(__NA_0S0023_20081114_SPAWN_MONSTER_QUEST)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif


#if defined(__NA_000597_20070821_GUILD_CORPS_SYSTEM) && !defined(__NA_000596_20070821_GUILD_CLASS_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_PET_SAVE_TO_DB) && !defined(__NA_0_PET_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_PET_FEED) && !defined(__NA_0_PET_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined (__NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined( __NA_001027_20080423_RANDOM_REWARD_EVENT_ )
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif


#if defined(__NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__)
	#if !(defined(__NA844896_080219_TASK_SSQ_AND_100LV_QUEST_INSTANCE_ZONE__) &&		\
		  defined(__NA844896_080219_TASK_SSQ_AND_100LV_QUEST_NPC_KILL_OPTIMIZER__))
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#if defined(__NA001021_080421_PACKET_COMPRESS_V02__) && !defined(__NA000995_080402_SKILL_PROC_UPDATE_V01__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__NA000000_GRID_REBUIDING_FOR_READABLE_CODE__) && !defined(__NA001021_080421_PACKET_COMPRESS_V02__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__)
	#if !defined(__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__)	// ��Ÿ �߰��� �� ã�Ƽ� �߰��� ��.
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif
#if defined(__NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__) && !defined(__NA000000_GRID_REBUIDING_FOR_READABLE_CODE__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__)
	#if !(	defined(__NA_0_20080219_DETECT_ABUSING_USER_REFACTORING)				\
			&& defined(__NA_0_20080219_DETECT_ABUSING_USER_POWERFUL)				\
			&& defined(__NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL__)	\
		)
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#if defined(__NA_1119_CHANGE_ETHER_EXPEND_STYLE) && !defined(__NA_000968_ETHER_EMISSION_DEVICE)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_1247_POSSESSION_ITEM) && !defined(__NA_1131_PET_NAME_SETTING)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__) && \
    !defined(__NA_0S0023_20081114_SPAWN_MONSTER_QUEST)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_20081029_PARTY_REFACTORING)
    #if !defined(__NA_0_20081124_PARTY_REFACTORING_GAMESERVER)
      #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(__NA000000_090224_PARTY_RETURN_PROBLEM_CORRECTION__) && \
   !defined(__NA_0_20081029_PARTY_REFACTORING)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__CN_1159__LUCKY_ELITE_ITEM) && !defined(__CN_1194__ELITE_1_5_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_ADD_GM_CMD_ABOUT_AC) && !defined(__NA_1288_AC_RENEWAL_FOR_TIME_LINE)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001283_20090221_COLLECT) && !defined(__NA_1288_AC_RENEWAL_FOR_TIME_LINE)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__) && \
    !defined(__NA001226_INSTANCE_DUNGEON_EXTENTION__)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__) && \
    !defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP) && !defined(__NA_1247_POSSESSION_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__) && \
    !defined(__NA_1233_2009_NEW_YEARS_DAY_EVENT)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING) && !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM) \
    && ( !defined(__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION) || !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__KR_001355_20090624_GROUPINGSYSTEM_ALL) \
    && ( !defined(__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION) || !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL) && !defined(__NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2) && \
    !defined(__NA000000_090712_SCRIPT_PARSING_SYSTEM_MODIFICATION)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_20090813_BINARY_LOG_ERROR_RECOVERY) \
    && !defined(__NA_0_20090812_BINARY_LOGGING_IMPROVEMENT)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_POSSESSION_ITEM_DROP) && !defined(__NA_1247_POSSESSION_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__) && \
   !(defined(__NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__) && defined(_NA_20090925_ITEMSTREAM_INIT_RESERVED))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__) && \
   !(defined(__NA001390_090915_RIDING_SYSTEM__))// && defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP) && !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) && !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100211_SD_EFFECT_MODIFY) && !defined(__NA_001290_20090525_SHIELD_SYSTEM)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_DH_GROUPSYSTEM_CHANGE_PACKET) && !defined(__KR_001355_20090624_GROUPINGSYSTEM_ALL)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_) && \
    !defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_) && \
    !(defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__) && \
      defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

// NOTE: cross dependency '_NA002217_100728_EP2_SKILL_SYSTEM_DB_'
//                                         and
//                          'C_NA_0_20100520_ACSYSTEM_CHANGED'
#if defined(_NA002217_100728_EP2_SKILL_SYSTEM_DB_) && \
    !(defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_) && \
      defined(C_NA_0_20100520_ACSYSTEM_CHANGED))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(C_NA_0_20100520_ACSYSTEM_CHANGED) && \
    !defined(_NA002217_100728_EP2_SKILL_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA002823_WINGS_EQUIPMENTS_EXTENTION) && \
    !defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000424_GLOBAL_CASH_ITEM_RENEWAL_) && \
    !(defined(_NA000424_GLOBAL_CASH_ITEM_RENEWAL_) && defined(__NA000000_SHOP_USERID_50_PASSING_PROC__))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET) && \
    ( !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM) || defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_) || \
    defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_) || \
    defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_) || \
    defined(_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM)
    //_NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_ (default enabled)
  #if !defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
#endif

#if defined(_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM)
  #if !defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_) || \
      !defined(_NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
#endif

#if defined(_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY)
    #if !defined(_NA_20100604_BETTER_DOMINATION_MANAGER) || \
        !defined(_CH_20100420_CHANGE_DOMINATION_APPLY_WAY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA001605_EP2_DAMAGETYPE_EXTENTION_) && !defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#ifdef _NA_001990_ACSYSTEM_ADD
	#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED) && !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
    #if !defined(_NA_001990_ACSYSTEM_ADD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM) && \
   !defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20100322_AGGRO_DISPLAY) && !defined(_NA_0_20100819_AGGRO_RENEWAL)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES) && \
   !defined(_NA_0_20110318_ITEM_EXPIRATION_PERIOD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN)
    #if !defined(_NA001385_20090924_DOMINATION) || \
        !defined(_NA_20100604_BETTER_DOMINATION_MANAGER) || \
        !defined(_CH_20100420_CHANGE_DOMINATION_APPLY_WAY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED) && \
    defined(_NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_002935_20110704_ITEM_SCORE)
    #if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT) || \
        !defined(_NA_001990_ACSYSTEM_ADD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_SCJ_ACSYSTEM_OVERLAP_PARENT) && !defined(_NA_001990_ACSYSTEM_ADD)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET)
    #if !defined(_YMS_AUTOBOT_ITEM_KIND) || \
        !defined(_NA_0_20110329_GAMEOPTION_SETTING)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA_003740_20111122_RANDOM_ITEM) && \
    !defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif