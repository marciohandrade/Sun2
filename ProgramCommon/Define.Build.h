#pragma once

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢鱔 Define 掘羲
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// [啻檜墅 敘罌]
// 1. 措陛滌 啻檜墅 : NA(婦啗橈擠), KR(и措), JP(橾獄), US(嘐措), 渠虜(TW), 醞措(CN), 艙措(UK)
// 2. 濛機 蹂羶憮 廓�� : 0(蹂羶憮橈擠)
// 3. 衛濛 陳瞼
// 4. 濛機貲
// [輿戮 敘罌] : 掘碟濠朝 '/'
// 1. 濛機 蹂羶憮 廓��
// 2. 濛機濠菟
// 3. 鼻撮и 濛機 頂辨 
// 4. define擊 ヴ歷擊 唳辦 蹺陛瞳檣 濛機擊 п撿 ж朝 頂辨

// ex)
// #define __[措陛囀萄]_[濛機廓�β_[陳瞼]_[濛機貲]_
// #define __KR_000123_20061020_TOWER_OF_DEVIL		//< [123]	2006. 04. 24	/ 梯團遵,嶸營艙 / 蠔幗蝶曖 羲蝗,盪輿嫡擎 鱉 濛機 / odbc蒂 SUNOnline_20455煎 夥脯撿 л
// #define __NA_0_20061020_TOWER_OF_DEVIL			//< [0]		2005. 02. 10	/ 梯團遵,嶸營艙 / 蠔幗蝶曖 羲蝗,盪輿嫡擎 鱉 濛機 / odbc蒂 SUNOnline_20455煎 夥脯撿 л

#include "Version.h"


#define __NA_00000_20080410_FOREIGN_DEFINE			// 措陛滌 絮董瞳檣 DEFINE 評煎婦葬
#ifdef __NA_00000_20080410_FOREIGN_DEFINE
	#include "Foreign.Build.h"
#endif

/*($Delete_[S/C]$)*/#define	__NA_000000_20080515_HACKSHIELD_PATH							// 2008.05.15 / 褻營衝, 梯唳ル / р蔓萄 唳煎 濠翕�� 濛機.
/*($Delete_[S/C]$)*/#define __NA000000_080905_MOLA_VERSION_BUILD_ROUTING__					// 2008.09.05 / 檜擎だ / MOLA 塭檜粽楝葬, 措陛 塽 幗瞪滌 塭檜粽楝葬 唳煎 撲薑 塭辦た 晦棟 蹺陛
/*($Delete_[S/C]$)*/#define __NA000824_070103_SOLARAUTH_MASTER_NETWORK_MECHANISM_OPTIMIZE__	// 2007.01.03 / 檜擎だ / SolarAuth憮幗諦 Master 憮幗除 啻お錶觼 瞪歎 詭醴棲闈 譆瞳��
/*($Delete_[S/C]$)*/#define __NA001098_080711_SERVER_PROCESS_CONTROL_BUG_FIX__				// 2008.07.11 / 檜擎だ / [824]翱翕, SolarAuth 塽 晦顫 憮幗 鐘お煤 嘐蝶 僥薯 熱薑 婦溼
/*($Delete_[S/C]$)*/#define _NA_DUPLICATION_CATEGORY_BUG                                    // 
/*($Delete_[S/C]$)*/#define __NA000000_ENABLE_MASTER_SERVER_SCRIPT_LOADING__				// 2008.09.11 / 檜擎だ / 葆蝶攪縑憮紫 蝶觼董お蒂 檗擊 熱 氈紫煙 熱薑
// (WARNING) ж晦 working macro朝 п諼縑憮 僥薯嫦儅衛 賅棲攪葭 ж晦 嬪и 匙戲煎 雖辦雖 蜓 匙.
//#define __NA000000_080709_DBQUERY_FAULT_DETECTION__					// 2008.08.09 / 檜擎だ / DBProxy 詭賅葬 援熱 嫦儅й 熱 氈朝 睡碟 熱薑 (歜衛 囀萄紫 覬 氈擠: 釭醞縑 薑葬ж啊擠)
/*($Delete_[S/C]$)*/#define __NA000630_070817_MOLA_5th_UPDATE				// 2007/08/17 /檜擎だ,梯雖諫/陛鼻 贗塭檜樹お 寞雖濛機 (MOLA 5離 機等檜お) - LINKED {__NA070212_ENHANCED_SECURE_POLICYMENT__}

/*($Delete_[S/C]$)*/#define __NA_0_20090311_ITEMCOPY_BUG_FOR_LEAVESYN_ERROR					// 2009.03.11 / 檜遽戮 / 渠虜 嬴檜蠱犒餌, LeaveSyn 籀葬睡碟縑憮曖 螃盟

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
        #define __NA000000_070913_HACKSHIELD_INSTALLATION__ // 2008.01.23 / lammy / р蔓萄 婦溼
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
        #define _YMS_USING_FROST_LIBRARY_ // 2010.3.22 / 嶸貲熱 / 楝衛嬴橾陽朝 frost 餌辨
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.6.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1601
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / 團唳蝓 / 億 囀蝶え蝸煜 蹺陛(爾褻囀蝶え)
    #define _NA_008633_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / �姨瘉� / 等囀 囀蝶え 蹺陛
	#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE											// 2016.01.15 / 辦�ㄟ� / NPC 鼻薄 啗骯奢衝 滲唳
	#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2										// 2016.02.15 / 辦�ㄟ� / 萄楚堆釭檜お 辨 滲褐(懍萄楚堆お楠蝶イ10溯漣) 蝶鑒 餌辨 衛 檜樓 棟溘纂 瞳辨 寰腎朝 幗斜 熱薑
    #define _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION                        // 2016.02.03 / �姨瘉� / ж歜 剪楚榆 憲葡 褫暮 蹺陛
	#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS											// 2016.02.24 / 辦�ㄟ� / 顫問 偃羹熱 滲唳 蹂羶(蝶顫橾囀萄 蹺陛)
    #define _NA_008706_20160229_TEXTLOG_PK                                                        // 2016.02.29 / 團唳蝓 / 馬衛渠鼻 pk衛 臢蝶お煎斜
    #define _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX                                        // 2016.03.08 / 歎雙謙 / 濠翕檜翕戲煎 ん驍煎 陞 陽 Ы溯檜橫 諒觼 寰蜃朝 僥薯 熱薑
    #define _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE                                           // 2016.03.15 / 歎雙謙 / 嫦酈葬 縑纔腦錚ア紫 蝶む暫鼠粽 蹺陛
    #define _NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL                                        // 2016.03.29 / 歎雙謙 / 薄滄瞪 斜楚а жщ褻瞰 晦棟 犒掘
    #define _NA_000000_20160308_ATTACK_RATE_RISE_BUG                                              // 2016.03.08 / �姨瘉� / 奢問樓紫 鼻蝓 幗斜 熱薑
	#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE								// 2016.03.14 / 辦�ㄟ� / 陛п 等嘐雖 HP/SD 瞪�� 衛 褻勒 蹺陛 濛機
    #define _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST                                           // 2016.03.15 / �姨瘉� / 嬪纂綰溯檜萄 寨楛諒 褻薑 濛機.
    #define _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE                                    // 2016.03.21 / �姨瘉� / 縣葬詮驍葬蝶お 雖な檜 橾奩奢問 嫦餌羹蒂 儅撩ж紫煙 濛機.
	#define _NA_008765_20160405_DISCONNECTION_REASON_LOG											// 2016.04.05 / 辦�ㄟ� / 鐘喀暮 莒啣擊 唳辦 檜嶸蒂 煎斜縑 陴晦朝 濛機
	#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / 辦�ㄟ� / 爾�˙鬌� 檜鼻⑷鼻 熱薑
    #define _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG                                            // 2016.04.18 / �姨瘉� / 蛤も蝶賅暮檜 ヴ葬朝 幗斜 熱薑

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.5.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1504
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008334_20150608_SONNENSCHEIN                                  // 2015.06.08 / 團唳蝓, 歎雙謙 / 跨お凜檣 PvE, 模喧凜檣
    #define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                     // 2015.10.01 / 團唳蝓 / 奢問濠望萄薑爾 薯剪, 薄滄瞪 餌瞪憲葡 賅萇в萄嶸盪縑啪 爾頂啪ж晦
    #define _NA_008472_20150903_IMPROVED_BALANCE                              // 2015.09.03 / �姨瘉� / 寨楛蝶 偃摹
    #define _NA_008486_20150914_TOTAL_BALANCE                                 // 2015.09.14 / 團唳蝓 / 寨楛蝶濛機
    #define _NA_008536_20151023_ELITE4TH_ADD                                  // 2015.10.23 / �姨瘉� / 縣葬お4離 鼠晦/寞橫掘 蹺陛 濛機
    #define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                         // 2015.10.27 / 團唳蝓 / 縣葬お4 褫暮 蹺陛 濛機
    #define _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE               // 2015.10.26 / �姨瘉� / お葬剪 顫檜該 SCENE TYPE擊 橈撻棻
    #define _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD               // 2015.11.03 / �姨瘉� / 蠡蝶お 濠翕 熱塊 む檜雖 晦棟 蹺陛
    #define _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD                 // 2015.10.30 / �姨瘉� / 陝撩縑 營猿 偃熱蒂 蹺陛ж朝 濛機
    #define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / 團唳蝓 / 偃檣鼻薄 匐儀 衛, 葆擊薯и 薯剪
    #define _NA_008571_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / �姨瘉� / 偃檣鼻薄 匐儀 衛, 葆擊薯и 薯剪
    #define _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE                 // 2015.11.25 / 歎雙謙 / 葆辦蝶 贗葛戲煎 蝶鑒聽濘檜 ヴ葬朝 褻勒 滲唳
    #define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME             // 2015.11.26 / 團唳蝓 / 囀蝶え嬴檜蠱 晦除撲薑婦溼 熱薑
    #define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                    // 2015.12.02 / 團唳蝓 / 檜翕蝶鑒 煎霜 嬪纂滲區
    #define _NA_008606_20151222_MODIFY_GUILD_LEVEL                            // 2015.12.22 / 團唳蝓 / 望萄詹幗 驍黴 衛, 溯漣棻遴 腎雖 彊紫煙 熱薑
    #define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST               // 2016.02.04 / 團唳蝓 / 瞳辨醞檣 撮お濰綠�膩�蒂 評煎 盪濰衛麵菩棻

	#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION						  // 2015.10.29 / 辦�ㄟ� / 橾橾 望萄 嘐暮 晦棟 掘⑷

    #if defined(_KOREA)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / 團唳蝓 / 檜樓奢衝 滲唳

    #elif defined(_CHINA)
		#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE       // 2015.03.29 / 辦�ㄟ� / 醞措 AC 爾鼻 縣葬お 嬴檜蠱 瞪辨 剪楚顫殮 蹺陛(剪楚薯и + PK萄奧 陛棟)

    #elif defined(_JAPAN)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / 團唳蝓 / 檜樓奢衝 滲唳
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.5.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1503
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008283_20150511_NOTIFICATIONS_SYSTEM                          // 2015.05.11 / �姨瘉�, 歜鼻熱 / 憲葡 衛蝶蠱
    #define _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION                  // 2015.06.17 / 歎雙謙 / 螞繭 褐敘 蝶顫橾 賅萄縑 評艇 か熱 擁棲詭檜暮 濛機
    #define _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL                 // 2015.07.10 / 歎雙謙 / 熱薑晦菅 ui曖 臢蝶お 醞 溯漣 高擊 蝶觼董お縑憮 檗橫諦憮 轎溘
    #define _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY             // 2015.10.15 / 歎雙謙 / ん驍貲 轎溘 嬪纂 褻薑 醞 鼠и瑞Щ 僥薯 熱薑
    #define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                       // 2015.07.03 / 團唳蝓 / 衛除薯 ゆ嬴檜蠱 萄奧腆 衛, charge sub type 1廓紫 陛棟ж紫煙 熱薑
    #define _NA_008404_20150712_MINIGAME_SYSTEM                               // 2015.07.12 / 歜鼻熱 / 嘐棲啪歜 衛蝶蠱 蹺陛
    #define _NA_008405_20150716_MINIGAME_BINGO                                // 2015.07.16 / �姨瘉� / 嘐棲啪歜 綵堅
    #define _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE                // 2015.08.18 / 歜鼻熱 / 闊衛餌辨模資顫殮 蹺陛 
    #define _NA_008441_20150819_CRITICAL_DAMAGE_CHANGE_FROM_LEVEL_DIFFERENCCE // 2015.08.19 / 歜鼻熱 / 溯漣離檜縑 評艇 觼葬じ鏽 等嘐雖 隸馬 熱纂 滲��
    #define _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM                  // 2015.08.25 / 歜鼻熱 / 望萄 溯漣 ж塊衛 望萄 婦啗撲薑 掏и檜 橈擊唳辦 賅萇 望萄婦啗蒂 餉薯ж紫煙 熱薑
    #define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                        // 2015.09.09 / 團唳蝓 / 蝶Я葩葬離雖, 匐晦 ヴ 醱瞪鼻鷓縑憮紫 蝶鑒餌辨 陛棟
    #define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT               // 2015.09.09 / 團唳蝓 / 奢樓 300薯и п薯
    #define _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG                      // 2015.09.22 / �姨瘉� / 寡ぎ襄曖 塊 嬴檜夔檜 薯渠煎 ル衛腎雖 彊朝 ⑷鼻 熱薑 濛機
    

    #if defined(_KOREA)
        #if defined(_SUNGAME_VER)
            #define _NA_008364_20150623_CHANGE_TEXT_PATH                              // 2015.06.23 / �姨瘉� / TEXT 唳煎 滲唳
        #endif // _SUNGAME_VER

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.5.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1502
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////

	#define _NA_008012_20150204_SUN_RANKING_SYSTEM                              // 2015.02.04 / 歜鼻熱 / Sun Ranking System
	#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM                              // 2015.02.03 / �姨瘉� / SUN 楨韁 衛蝶蠱
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE                        // 2015.02.03 / �姨瘉� / SUN 楨韁 蛔晝 衛蝶蠱
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE                       // 2015.02.03 / �姨瘉� / SUN 楨韁 衛蝶蠱 翕鼻
	#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    #define _NA_008124_20150313_AWAKENING_SYSTEM                                // 2015.03.13 / 歜鼻熱 / 陝撩 衛蝶蠱
    #define _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM                      // 2015.03.11 / �姨瘉� / 濰綠 陝撩 衛蝶蠱
    #define _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM                      // 2015.03.18 / �姨瘉� / 濰綠 霞�� 衛蝶蠱
    #define _NA_008069_20150210_ADD_SPA_SYSTEM                         // 2015.02.10 / 團唳蝓, 歎雙謙 / 螞繭衛蝶蠱
    #define _NA_008139_20150318_MODIFY_BUFF_ICON_UI                    // 2015.02.10 / 歎雙謙 / 幗Щ嬴檜夔(Status) UI 熱薑
    #define _NA_008185_20150402_COSTUME_ITEM_FOR_NPC                   // 2015.04.02 / 歎雙謙 / 囀蝶え NPC殮�鰼�
    #define _NA_008226_AC_PARENTMAP_LIST_SKIP                          // 2015.04.15 / 歎雙謙 / AC 摹ч婁薯 葬蝶お 跡煙檜 望橫韓 陽 儅楞ж晦 晦棟 蹺陛
    #define _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE   // 2015.04.23 / 歎雙謙 / 爾蝶(譫ず)跨蝶攪 觼葬じ鏽 顫問 擁棲詭檜暮 鼠衛
    #define _NA_008252_20150427_AURASKILL_PACKET_PENDING               // 2015.04.27 / 歎雙謙 / 螃螃塭蝶鑒 ぬ韃 醞犒籀葬 僥薯
    #define _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY  // 2015.04.28 / 歎雙謙 / 轎戮羹觼 衛蝶蠱 援瞳⑽ 薑溺寞衝 熱薑 (殖溘⑽婁 翕橾ж啪)
    #define _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX            // 2015.05.18 / 歎雙謙 / ⑦煎檜萄 蝶б警蝸 幗斜 熱薑
    #define _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE           // 2015.05.19 / 歎雙謙 / 檜翕醞縑 棻遴檜 寰勘葬朝 ⑷鼻 熱薑(蛤幗Щ馬模衛除4蟾縑 曖п憮 嫦儅)
	#define _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST               // 2015.06.01 / 歎雙謙 / 蠡蝶お 諫猿 褒ぬ 衛 營蹂羶 晦棟 蹺陛
    #define _NA_000000_20150604_BUG_FIND_TEST                          // 2015.06.04 / 歎雙謙 / 幗斜 瓊晦辨 纔蝶お 囀萄 (CTRL+F10 戲煎 蠡蝶お 啻綠啪檜暮 嬪纂 蟾晦��)
    #define _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX       // 2015.06.09 / 歎雙謙 / �蛻� 檜翕 衛 遺霜歜 幗斜 熱薑
    #define _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR                  // 2015.06.16 / 歎雙謙 / 螞繭褡 該葬 爾橾 熱 氈紫煙 Ы楚斜煎 撲薑ж紫煙 偃摹
    #define _NA_007863_20141203_VOLUME_LIGHT_SCATTERING                         // 2014.12.03 / �姨瘉� / 獐睞 塭檜お陛 薯渠煎 濛翕ж雖 彊湍 ⑷鼻 熱薑
    #define _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS          // 2015.03.30 / �姨瘉� / 褐敘 欽蹴酈 蛔煙 偃摹
    #define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / 團唳蝓 / 溯漣機AC檜 唳辦 '檜瞪溯漣' AC紫 偽檜 匐儀п憮 籀葬ж紫煙и棻
    #define _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW             // 2015.03.31 / �姨瘉� / SUN 楨韁 衛蝶蠱 薑骯橾 爾罹輿朝 濛機
    #define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / 團唳蝓 / 嬴檜蠱 檜翕婦溼 僥薯 熱薑
    #define _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE               // 2015.04.22 / �姨瘉� / 薄滄瞪 壁縑憮 NPC鼻薄擊 翮 熱 氈紫煙 熱薑
    #define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / 跨蝶攪陛 寰瞪雖渠縑憮 敝�窈褗捂� 跤瓊擊唳辦 蘿溯んお衛麵幗萼棻
    #define _NA_008256_20150428_MODIFY_CANCLE_STAMP_LOGIC                       // 2015.04.28 / 歜鼻熱 / 陝檣 鏃模 煎霜 熱薑
    #define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / 棻觼螃粽塭檜お偽擎 'Effect'蒂 餌辨ж朝 蝶鑒檜 餌辨濠陛 避擊唳辦 偽檜 餌塭雖紫煙 и棻
    #define _NA_008270_20150511_MODIFY_OUT_OF_RANGE                             // 2015.05.11 / 歜鼻熱 / DominationBattleResult縑憮 OutOfRange 釭朝 ⑷鼻 熱薑
    #define _NA_008299_20150519_MAX_DAMAGE_LOG                                  // 2015.05.19 / 歜鼻熱 / 譆渠等嘐雖 煎斜 濛機
    #define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / 團唳蝓 / 孺萄褒萄 �膩�陛 鬼薯橫鷗 詳�鰽絲珌挬�
    #define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / 團唳蝓 / 寨楛蝶濛機擊 嬪п 贗塭縑 瞪癱薑爾 瞪殖ж晦
    #define _NA_008333_20150615_BROADCAST_LEFT_RIGHT_TURN                       // 2015.06.15 / 歜鼻熱 / 謝辦�蛻� ぬ韃紫 粽煎萄議蝶お ж紫煙 熱薑
    #define _NA_008333_20150615_FIXED_MAX_LEVEL_EXP                             // 2015.06.15 / 歜鼻熱 / 虜溝 殖撩衛 唳я纂 堅薑
    #define _NA_000000_20150714_MOVEMENT_DISTANCE_CHECK                         // 2015.07.14 / 陴晦瑰 / 蝶Я萄р 羹觼. 檜翕 陛棟 剪葬 羹觼
    #define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                            // 2015.09.04 / 團唳蝓 / ⑦煎檜萄蝶б 鼻鷓縑憮 BACKSTEP檜翕擎 薑賊殖葬晦諦 偽擎 樓紫蒂 陛霞棻
    #define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / 團唳蝓 / 議衛憧曖 嬴檜蠱 掘衙, 摹僭婦溼 DB煎斜 晦煙ж紫煙 蹺陛
    #define _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG    // 2015.10.29 / 歎雙謙 / 導晦 ぬ韃 SYN諦 ACK餌檜曖 裁溯檜翕寰 遺霜檜賊憮 嬪纂 寰蜃朝 ⑷鼻 熱薑

#ifdef _DEV_VER
    #define _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO                // 2015.06.04 / 歎雙謙 / 偃嫦賅萄縑憮 等嘐雖 陛п榆擊 ル衛п輿朝 濛機
#endif //_DEV_VER

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.5.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1501
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT   // 2015.01.13 / 歎雙謙 / 偃檣 錚檜ん檣お 譆渠 盪濰 偎熱 隸陛 (10->15)
    #define _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT     // 2015.01.14 / 歎雙謙 / 嘐暮爾鼻璽 蝸煜 轎溘,罐そ 熱薑
    #define _NA_007726_20141015_PROTECTION_SCREENSHOT                 // 2014.10.15 / 歎雙謙 / 蝶觼萼憐 懍����(憶蹴) 晦棟
    #define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS        // 2014.12.03 / 歎雙謙 / 塭檜お裘 網萄 衛 螃粽薛お 婦溼 薯諼 & 晦棟 睡��
    #define _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON              // 2015.01.15 / 歎雙謙 / 詭檣詭景 渠睛雖紫 幗が 蹺陛
    #define _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT       // 2015.02.26 / 歎雙謙 / 啪歜謙猿 衛 薑爾翮塋褒 盪濰諫猿 ぬ韃擊 晦棻葬雖 彊堅 夥煎 謙猿
    #define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / 團唳蝓 / GM貲滄橫, 唳я纂檜漸お 薯橫ж晦
    #define _NA_007935_20150109_GM_COMMAND_EXP_EVENT                            // 2015.01.09 / �姨瘉� // GM 貲滄橫 蹺陛 偃嫦(GM貲滄橫煎 唳я纂 檜漸お 虜菟晦)
    #define _NA_007932_20150106_USER_EXTRA_INFO                                 // 2015.01.06 / 歜鼻熱 / UserExtraInfo 蹺陛
    #define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                            // 2015.01.15 / 團唳蝓 / 橫雯牆萄蒂 м撩п紫 唸婁僭擎 楠渾戲煎 釭螃紫煙 熱薑
    #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC                // 2015.01.27 / 歜鼻熱 / AgentServer曖 packet handler縑憮 ProtocolMatrix蒂 薑瞳 偌羹煎 噙紫煙 熱薑
    #define _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL                  // 2015.01.29 / �姨瘉� / 嬴檜蠱 憧縑憮 蝸煜曖 撲貲 臢蝶お蒂 還檜雖 彊堅 ル衛
    // #define _NA_008012_20150130_RANKING_SYSTEM                                  // 2015.02.02 / 歜鼻熱 / 晦襄 楨韁 衛蝶蠱 蛤だ檣 撲薑
    #ifdef _NA_008012_20150130_RANKING_SYSTEM
        #define _NA_008016_20150202_OLD_RANKING_SYSTEM                           // 2015.02.02 / �姨瘉� / 晦襄 楨韁 衛蝶蠱 蛤だ檣 撲薑
    #endif // _NA_008012_20150130_RANKING_SYSTEM
    #define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / 團唳蝓 / 跨蝶攪藹奢 檜漸お衙棲盪煎 薯橫
    #define _NA_008078_20150211_GM_COMMAND_TIME_DATE                            // 2015.02.12 / 歜鼻熱 / GM貲滄橫蹺陛, 憮幗 衛除 塽 陳瞼 滲唳
    #define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / 團唳蝓 / 觼葬蝶驍ゃ溯蝶,察雖嬴檜蠱 衛除虜猿衛 薯渠煎 п薯п輿雖彊朝僥薯
    #define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / 團唳蝓 / Ы溯檜橫郭葬 諄遺縑憮 и寞縑 避朝唳辦 羹觼ж堅 煎斜煎 晦煙и棻
    #define _NA_008150_20150331_MODIFY_EX2_CHARGE_SUB_TYPE_USABLE_ANOTHER_FUNC      // 2015.03.31 / 歜鼻熱 / eCHARGE_SUB_FIXED_AMOUNT_EX2 顫殮 嬴檜蠱擊 DROP 塽 LOTTERY煎 橢雖 跤ж朝 ⑷鼻 熱薑
    #define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / 夥檜蝶模萄偽擎 雖樓瞳戲煎 翕濛ж朝 蝶鑒檜 葆撿驕�素� 隸筒睦 隸陛 啗骯縑 僥薯蒂 嫦儅衛鑑棻
	#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / 團唳蝓 / 蠡蝶お幗斜婦溼濛機, dbЩ煎衛盪諦 ぬ韃除 諒觼褻瞰

    #if defined(_KOREA)
        #define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / 團唳蝓 / 跨お凜檣辨 臢蝶お煎斜, и寞等嘐雖僥薯 馬衛辨
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / 歜鼻熱 / 轎戮 衛蝶蠱 蹺陛

    #elif defined(_CHINA)
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / 歜鼻熱 / 轎戮 衛蝶蠱 蹺陛

    #elif defined(_JAPAN)
        //#undef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE                   // 2015.10.16 / 歎雙謙 / 轎戮羹觼 幗が ui縑憮 獗晦晦
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.4.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1404
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// ж嬪 幗瞪 廓��
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007732_20141024_RIDER_MOVESPEED_SYNC     // 2014.10.24 / 歎雙謙 / 棻艇Ы溯檜橫曖 驍匙 檜樓 諒觼 葆辭
    #define _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE // 2014.08.28 / 歎雙謙 / 褐敘贗楚蝶 - 嬪纂綰溯檜萄
    #define _NA_000000_20141222_DOMINATION_RIDER_ACTIVATE // 2014.12.22 / 歎雙謙 / 薄滄瞪縑憮 驍 匙 ъ辨
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007667_20141001_WITCHBLADE_SKILL                // 2014.10.01 / 團唳蝓 / 嬪纂綰溯檜萄 蝶鑒濛機
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007620_20140915_STATE_RESISTS // 2014.09.15 / 薑⑷撩 / 鼻鷓 賊羲2
    #define _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN // 2014.09.16 / �姨瘉� / 縑橫綰煎辦 棻遴 籀葬寞衝滲唳
    #define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / 團唳蝓 / 嫦酈葬模�紡� 棟溘纂 啗骯衝 螃盟 熱薑
    #define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION             // 2014.10.27 / 團唳蝓 / 賅萇 檣錳曖 殮濰嬴檜蠱 匐餌蒂 ж朝 襄顫殮 蹺陛
    #define _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET // 2014.11.12 / 歜鼻熱 / ぬ韃 綠薑鼻瞳檣 濠猿掘褻 熱薑
    #define _NA_007750_20141113_WZDAT_MODIFY_LOG                          // 2014.11.13 / 歜鼻熱 / WZDAT 婦溼 煎斜 熱薑
    #define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / 團唳蝓 / だじ鼻鷓縑憮 議葛攪 鼻鷓(HP,MP,SD,FP) 機等檜お 寞衝 滲唳
    #define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / 團唳蝓 / ゆ紫 模賅顫殮 11廓擊 噩熱氈紫煙и棻
	#define _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER                       // 2014.12.22 / 歜鼻熱 / 跨蝶攪 檜翕 お葬剪 濛翕ж雖彊朝 ⑷鼻 熱薑
    #define _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG // 2015.01.05 / �姨瘉� / ん驍 臢蝶お陛 嘐棲裘 UI 嬪縑 啜藥螳 轎溥腎朝 ⑷鼻 熱薑

    #define _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA                          // 2015.06.02 / 團唳蝓 / 嬴檜蠱衛葬橡剩幗蒂 40綠お 梱雖 餌辨й熱氈紫煙п遽棻
    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.4.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1403
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(6)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(0)		// ж嬪 幗瞪 廓��
	#endif
	
	//////////////////////////////////////////////////////////////////////////
    #define _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX // 2014.06.25 / 薑⑷撩, 歎雙謙 / ACTION_OPERATE_SWITCH_EX お葬剪 蹺陛
    #define _NA_007156_20140703_TOP_OF_TRIAL_LIGHT // 2014.07.03 / 薑⑷撩 / 盪輿嫡擎鱉 維曖衛溼
    #define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // 薑⑷撩,歎雙謙 / 跨蝶攪紫馬
    #define _NA_007477_20140805_RADAR_RAYER_MODIFY // 2014.08.05 // 歎雙謙 / 嘐棲裘曖 類 ル衛 滲唳(檣湍 諼縑紫 ル衛 陛棟)
    #define _SCJ_TOGGLE_EDITBOX_CONTROL // 2014.07.22 / 歎雙謙 / 饜旋 縑蛤お夢蝶 鐘お煤蹺陛
    #define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / 團唳蝓 / 紫瞪橾橾蠡 蹺陛
    #define _NA_007330_20140620_GUILD_SYSTEM_EXTENSION                      // 2014.06.20 / �姨瘉�, 歜鼻熱 / 望萄 衛蝶蠱 �挫�
    #ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        #define _NA_004034_20120102_GUILD_INTRODUCTION                          //2012.05.10 / 薑⑷撩,檜紫⑽ / 望萄 模偃 濛機
    #endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    #define _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP // 2014.07.22 / 薑⑷撩 / 晦除薯 嬴檜蠱 鼻薄 っ衙
    #define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / 團唳蝓 / 營濛營猿陛 濰綠橾 唳辦 '檣疆お'縑 婦溼脹 褻勒 滲唳
    #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 歜鼻熱 / 跨お凜檣 奢撩瞪 蟾晦�� 塽 爾鼻 雖晝 熱薑

    #define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / 團唳蝓 / DB望萄煎斜 濛機(LogConsole)
    #define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / 團唳蝓 / 唸薑�� 夥紱晦
    #ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
        /*($Delete_[x/C]$)*/#define _NA_007450_20140805_CRYSTALIZATION_RENEWAL                   // 2014.08.05 / �姨瘉� / 唸薑�� 葬景橡
    #endif // _NA_007408_20140711_MODIFY_CRYSTALLIZE
    #define _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG // 2014.09.30 / �姨瘉� / 議衛憧 囀蝶え 嘐葬爾晦 晦棟縑憮 撩滌曖 擁衙л戲煎 檣ж罹 撩滌 摹鷗晦棟擊 爾檜雖 彊朝棻.
    #define _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM    // 2014.10.14 / 歜鼻熱 / 橾奩 鼻鷓縑憮朝 PK渡п紫 葬漸雖 渠鼻檜 嬴棍唳辦 嬴檜蠱擊 萄奧ж雖 彊朝棻

	#if defined(_KOREA)

    #elif defined(_CHINA)
		#undef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM // #7950 // 醞措 蹂羶戲煎 п渡 蛤だ檣 綠�側瘓�
        #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC // 2015.01.27 / 歜鼻熱 / AgentServer曖 packet handler縑憮 ProtocolMatrix蒂 薑瞳 偌羹煎 噙紫煙 熱薑

	#elif defined(_JAPAN)

	#elif defined(_GLOBAL)

	#elif defined(_RUSSIA)

	#endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.4.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1402
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_000000_20140423_SUMMON_BACKGROUND_PICKING             // 2014.04.23 / 歎雙謙 / 模�紡� �躁� а韁
    #define _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE       // 2014.04.08 / 歎雙謙 / 憎紫辦 模�紡� 諼⑽ 滲唳
    #define _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION     // 2014.04.08 / 歎雙謙 / 蝶鑒 餌辨 �� 濠翕奢問 瞪�� 罹睡 褫暮��
    #define _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL         // 2014.03.24 / 歎雙謙 / 憎紫辦 蝶鑒 寨楛蝶 葬景橡
    #define _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER         // 2014.04.08 / 歎雙謙 / 憎紫辦 瞪辨 葆徹霞 蹺陛  
    #define _NA_007124_20140324_SHADOW_MODIFIED_SKILL_SOULCONTROL_SUMMON_GRADE    //2014.03.24 / 歜鼻熱 / 憎紫辦 模選 鐘お煤 熱薑 - 爾蝶 塽 譫ず紫 模�納� 熱 氈紫煙 滲唳(贗塭 濛機 歎雙謙)
    #define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / 團唳蝓 / 膠紫辦 蝶鑒 葬景橡
    #define _NA_000000_20140224_SUMMONER_REFACTORING                // 2014.02.24 / 薑⑷撩 / 模�紐� 葬景橡
    #define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.26 /歜鼻熱 /(跨お凜檣) domination 蝶觼董お 塽 だ憮 熱薑, domination type 蹺陛, 陝檣 撩奢衛 欐 螃幗 蹺陛
    #define _NA_007115_20140313_MONDSCHIEN_RENEWAL                 // 2014.03.13 / �姨瘉� / 跨お凜檣 葬景橡
    #define _NA_000000_20140331_MAPLOADING_MEMORYPOOL              // 2014.03.31 / 薑⑷撩, �姨瘉� / 裘煎註擊 MemoryPool煎 夥紱朝 濛機
    #define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / 團唳蝓 / 膠紫辦曖 縑傘雖戮暮 等嘐雖奢衝滲唳
    #define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / 團唳蝓 / 褐敘 瞪濰賅萄 埤萄楝衛
    #define _NA_007114_20140313_CHAOSZONE_GOLDRUSH                      // 2014.03.13 / �姨瘉� / 埤萄楝蓮
    /*($Delete_[S/x]$)*/#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / 團唳蝓 / 衙蘆衛蝶蠱 葬ね饜葭
    #define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / 薑⑷撩,歎雙謙 / 雖棟⑽NPC
    #define _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL  //2014.04.02 /歜鼻熱 /瞪濰蕨擒擎 瞪濰 瓣割縑憮虜 陛棟ж紫煙 熱薑
    #define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / 團唳蝓 / GM貲滄橫 蹺陛/熱薑
    #define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / 團唳蝓 / 鼻鷓睡罹 お葬剪陛 裘螃粽薛おи纔 稱艘擊陽虜 濛翕и棻
    #define _NA_007294_20140613_DRAGONKNIGHT_RENEWAL                    // 2014.06.13 / 歜鼻熱 / 萄楚堆 釭檜お 葬景橡
    #define _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW      // 2014.06.13 / �姨瘉� / 議葛攪 蕾樓 衛 跨お凜檣 撩輿望萄諦 翕裊菟擊 偽檜 嗥錶 還 熱 氈紫煙 偃摹

    #define _NA_007743_20141027_ADD_ISVAILD_CHECK                        // 2014.10.27 / 歜鼻熱 / 爾�� 囀萄 蹺陛
    #define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / 團唳蝓 / ぬ韃滲褻縑 曖и AC爾鼻 鼠и嫡晦僥薯 籀葬

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY        // 2014.10.27 / 歜鼻熱 / MiniDumpWithFullMemory煎 晦煙ж紫煙 熱薑


    #elif defined(_JAPAN)
        #define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / 團唳蝓 / 埤萄楝衛縑憮 埤萄ん檣お 滲翕餌о擊 DB煎斜縑 陴曹棻
        #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 歜鼻熱 / 跨お凜檣 奢撩瞪 蟾晦�� 塽 爾鼻 雖晝 熱薑
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.4.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1401
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_007019_20140107_PERK_CHANGE_SYSTEM // 2014.01.07 / �姨瘉� / 醴蝶籤 ぬ衛粽 斜瑜戲煎 滲唳ж朝 濛機
    #define _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX      // 2014.02.10 / 歎雙謙 / 瞪⑵ 棻艇 蠡蝶お 璽檜 翮葬朝 ⑷鼻 熱薑
    #define _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM // 2014.01.20 / 歎雙謙 / 薄滄瞪 睡�� 寞衝 滲唳 (避戲賊 で機璽)
    #define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / 團唳蝓 / 餌蜂 衛, 寞橫掘 頂掘紫陛 0檜賊 雪擎�捕�煎 濰綠陛 模資ж朝 ぬ割じ 薯剪
    #define _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG // 2014.02.21 / �姨瘉� / 議葛攪 摹鷗�飛橦□� Enter酈蒂 援腦堅 緒腦啪 寞щ酈蒂 援蒂 唳辦 摹鷗脹 議葛攪陛 棻艇 議葛攪煎 蕾樓檜腎賊, "裘幗瞪檜 棻落棲棻" 縑楝 詭衛雖 檜�� 贗塭檜樹お陛 謙猿腎朝 ⑷鼻 熱薑
    /*($Delete_[x/C]$)*/#define _NA_007080_20140227_DEACTIVATION_APPEARANCE_UPDATE_AND_ATTACKRANGE_UPDATE_BUG // 2014.02.27 / �姨瘉� / 濰綠陛 綠�側瘓� 腎歷擊 陽(頂掘紫 0檜釭 蝶邐檜 寰蜃擊 陽)偵褐檜 薯渠煎 腎雖 彊朝 幗斜, べ纂鼻鷓檣等紫 錳剪葬 奢問檜 陛棟и 幗斜
    /*($Delete_[S/x]$)*/#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / 團唳蝓 / 頂掘紫0檜釭 蝶囌滲�風� 鼠晦 綠�側瘓� 衛, 鼠晦蒂 濰雜п薯и 鼻鷓煎 虜萇棻.
    #define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / 團唳蝓 / 望萄橾橾蠡蝶お蒂 '諫猿'ц朝等 爾鼻擊 嫡雖彊擎 鼻鷓縑憮 偽擎斜瑜蠡蝶お蒂 嫡朝 僥薯 熱薑
    #define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / 團唳蝓 / 頂掘紫0檣 嬴檜蠱 濰雜陛棟
    #define _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST // 2014.03.14 / �姨瘉� / 檣漸饜葬蒂 翮歷擊 陽 替溥雖朝 ⑷鼻 熱薑

    #define _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND // 2014.01.14 / 歎雙謙 / 蘋螃蝶襄 殮濰 �側瘓�/綠�側瘓� 蹺陛

    #define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / 團唳蝓 / DB煎斜 望萄囀檣晦睡
    #define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / 團唳蝓 / 嬴檜蠱褻м 撩奢 晦煙 衛, ItemNum擎 檣漸饜葬 識 偃熱陛 嬴棋, 褻м撩奢и 偃熱煎 и棻.

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / 團唳蝓 / 耀掘蟾渠 溯漣滌 爾鼻雖晝衛, 溯漣高擊 const.h煎 貍魚棻
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.3.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1304
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20130812_NEW_CHARACTER_HELLROID  // 2013.08.12 / 梯褐鷓 / 褐敘贗楚蝶 - ⑦煎檜萄
    #define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / 團唳蝓 / 楨韁衛蝶蠱 葬景橡 塽 瞪濰楨韁 蹺陛(葆檜傘)
    #define _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR // 2013.08.19 / �姨瘉� / 楨韁 衛蝶蠱 偃ら Minor
    #define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL //2013.10.30 / 團唳蝓 / ⑦煎檜萄 蝶鑒嬪и 觼葬じ鏽 橫網葬じ 鼻鷓 蹺陛 塽 熱薑
    #define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / 團唳蝓 / 跨蝶攪陛 寰瞪雖渠煎 菟橫陛賊 '寞��'鼻鷓陛 嬴棲塭 '葬欐'鼻鷓煎 瞪�納炵絲� и棻.
    #define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / 團唳蝓 / thrustState(傚寥鼻鷓)檜�� 綠摹奢 跨蝶攪陛 奩問擊 ж雖 彊朝 僥薯 籀葬
    #define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / 團唳蝓 / 嬴檜蠱 儅撩衛, 詭賅葬й渡縑憮 憮幗陛 避朝 僥薯煎, 螃粽薛おヴ婁 詭賅葬й渡擊 夥脯獄棻.
    #define _NA_007031_20140116_CAN_SELL_CASH_ITEM_IN_STORE             //2014.01.16 / 團唳蝓 / 鼻薄縑憮 議衛嬴檜蠱 っ衙 陛棟ж紫煙 熱薑
    #define _NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY         //2014.01.16 / 團唳蝓 / っ紫塭鼻濠縑 菟橫陞熱 氈朝 晦除薯 嬴檜蠱 謙盟 熱薑
    //#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / 團唳蝓 / 臢蝶お煎斜陛 ж瑞欽嬪縑憮 衛除欽嬪煎 だ橾擊 儅撩ж紫煙 滲唳

    #if defined(_KOREA)
        #define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / 團唳蝓 / 賬縑 KISS蒂 稱檣棻. 憮幗 識 翕蕾濠熱蒂 瞪歎и棻.
    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.3.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1303
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM // 2013.07.04 / �姨瘉� / 楨韁 衛蝶蠱 偃ら
    #define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM      //2013.07.05 / 團唳蝓 / 楨韁衛蝶蠱 葬景橡 塽 瞪濰楨韁 蹺陛

    #define _NA_000000_20130828_GUILDMARK_SIZE_MODIFY      // 2013.08.28 / 歎雙謙 / 望萄葆觼 觼晦 滲唳
    #define _NA_006826_20130722_DOMINATION_RENEWAL         // 2013.07.22 / 梯霞��, 歎雙謙 / Я曖撩瞪 薄滄瞪 偃ら
    #define _NA_007239_20140521_BUG_GUILDRELATION_COUNT    // 2014.05.21 / 團唳蝓 / 望萄婦啗婦溼 譆渠纂 熱薑
    #if defined(_NA_006826_20130722_DOMINATION_RENEWAL) && !defined(_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL)
        #define _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL // 歜衛煎 撩僥婦葬 歜衛ぬ韃 虜菟橫憮 漆た葬蝶お縑 撩僥偃熱蒂 爾罹邀
    #endif //
    
    #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / �姨瘉� / ぅ饜葬橡 в萄縑憮紫 寡ぎ襄縑 霤陛 й 熱 氈紫煙 偃摹
    /*($Delete_[x/C]$)*/#define _NA_006886_20130910_BUG_POINT_ACCUMULATE_IMAGE // 2013.09.10 / �姨瘉� / 蕾樓 ん檣お 爾鼻 嫡晦 醞 縣葬お 2離 營猿陛 爾鼻戲煎 嫡嬴雖朝 唳辦 п渡 嬴檜蠱曖 嬴檜夔檜 1蟾葆棻 陝 贗楚蝶曖 營猿 嬴檜蠱戲煎 夥船橫 轎溘腎朝 ⑷鼻 熱薑 濛機    
    #define _NA_006928_20131022_HELMET_UI_BUG // 2013.10.22 / �姨瘉� / 議葛攪陛 籀擠 蕾樓ц擊 陽 ⑦訾 爾檜晦 寰爾檜晦 UI陛 蜃雖 彊朝 ⑷鼻 熱薑
    #define _NA_006930_20131023_ACCUMULATE_PAUSE_AND_CLOSE_ADD_FUNC // 2013.10.23 / �姨瘉� / 蕾樓ん檣お 爾鼻 嫡朝 紫醞 ESC酈蒂 鱔п 薑雖衛酈堅 dialog蒂 殘朝 晦棟 蹺陛
    #define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / 團唳蝓 / 嘐暮頂縑憮 綠薑鼻 謙猿 �� 營蕾樓ж賊, 嘐暮 頂 謙猿脹 嬪纂煎 寡纂脹棻.

    #if defined(_KOREA)
		#define _NA_000000_20130903_CHANNEL_GAGE_INCREASE      // 2013.09.13 / 歎雙謙 / 瓣割 �母� 啪檜雖蒂 號嬴爾檜啪 隸陛
    #elif defined(_CHINA)
        //#define _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION   // 2013.12.17 / 梯褐鷓 / 啪歜褫 惜錳�膩�(獐睞塭檜お, ん蝶お檜めお)陛 о鼻 OFF煎 腎紫煙 熱薑
        #define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / 團唳蝓 / 醞措 鬼繹賅萄 幗斜 婦溼 煎斜晦煙
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.3.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1302
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////
    
 
    #define _NA_006541_RENEWER_ACDIALOG // 2013.04.9 / 梯團繹 / AC璽 葬景橡
    #define _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER // 2013.03.22 / 梯團繹 / 嬴檜蠱 褻м璽 葬景橡
    #define _NA_006643_20130417_UI_OUT_WINDOW_BREAK // 2013.04.17 / 梯團繹 / UI偃摹塽 UI璽 檜翕 偃摹
    #define _NA_006589_20120415_ENCHANT_PRICE_MODIFY // 2013.04.15 / 梯霞�� / 檣臘お 綠辨 奢衝 熱薑
    #define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO // 2013.04.15 / 梯霞��, 歎雙謙 / 顫啃璽 薑爾縑 蔓萄 ル衛 蹺陛
    #define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY // 2013.04.16 / 梯霞�� / 醴嗽棲じ璽 鼻撮薑爾縑 謙褶檜 澀跤 ル衛腎朝 幗斜
    #define _NA_000000_20130417_SELFDISTRUCTIONABILITY_MODIFY // 2013.04.17. / 薑⑷撩 / SelfDistructionAbility 餌辨 薯и 褻勒 滲唳    
    #define _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP //2013.04.22 / 梯霞��, 歎雙謙 / お葬剪 蹺陛 - 溯檜渦裘(嘐棲裘) 滲唳
    #define _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT       //2013.05.31 / 歎雙謙 / 瓣餵 褒ぬ 衛, 縑楝囀萄縑 評塭 瓣餵擊 綠�側瘓� 衛霽
    #define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / �姨瘉�,團唳蝓 / ac婁薯 蹺陛(濠Я腦 問瞪雖)    
    #define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / �姨瘉�,團唳蝓 / 瞪濰縑憮 瞳 籀纂 衛, 蹺陛 菜薄
    #define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / �姨瘉�,團唳蝓 / '嘐暮諫猿', '瞪濰蝓葬' 蠡蝶お顫殮 蹺陛
    #define _NA_000000_20130422_SCALE_FULLMAP                       //2013.04.22 / 梯團繹 / 雖羲雖紫 蝶馨橾葭 濛機.                            
    #define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY // 2013.04.22 / 梯霞�� / 嘐暮 お葬剪 蹺陛 塽 熱薑濛機
    #define _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE        //2013.04.22 / 梯團繹 / 啪歜褫暮葬蝶お 蝶お葭囀萄 籀葬 濛機.
    #define _NA_000000_20130426_DUMPTOSCREENSHOT                    //2013.04.26 / 梯團繹 / 渾Щ瞪歎衛 蝶觼萼憐 繩睡
    #define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY // 2013.04.26 / 梯霞�� / 嫦酈葬 模�紡� 蝶鑒 諒觼 幗斜
    #define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / 薑⑷撩 / 衙霜褒萄 MP模賅榆 滲唳
    #define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY // 2013.04.26 / 梯霞�� / 嬴檜蠱 褫暮 蹺陛 滲唳
    #define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / 團唳蝓,�姨瘉� / 瞪濰縑 '橫衛蝶お' 蹺陛
    #define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / 團唳蝓 / 鼻鷓 幗Щ曖 辦摹牖嬪 啗骯寞衝 滲唳
    /*($Delete_[x/C]$)*/#define _NA_006687_20130502_SETTING_FIGHTINGENERGY_INIT_BUG // 2013.05.02 / �姨瘉� / 幗憮醴曖 匐晦醱瞪檜 棻艇 議葛攪縑憮紫 幗Щ 嬴檜夔戲煎 釭顫釭朝 ⑷鼻 熱薑
    //#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / 薑⑷撩 / 雖羲薄滄瞪 ぬ寡 ぬ割じ 瞳辨
    #define _NA_006679_20130423_1302_SKILL_RENEWAL                          // 2013.04.23 / 梯褐鷓 / 1302 蝶鑒寨楛諒
#ifdef _DEV_VER
    #define _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL    // 2013.04.23 / 梯褐鷓 / 1302 蝶鑒寨楛諒 - 嗥辦晦 擁棲詭檜暮 滲唳
#endif 
    #define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / 團唳蝓 / 鼻嬪幗Щ陛 勘溥氈擊 唳辦,ж嬪幗Щ 僭擒 餌辨 寞雖
    #define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER // 嘐暮頂 瓣餵 晦棟 お葬剪 蹺陛
    #define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / 團唳蝓 / 橫網葬じ顫殮蹺陛(345), 蝶鑒奢問溘, 寞橫�膩�縑 艙щ嫡雖彊朝 HP隸馬
    #define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.06 / 梯霞�� / 瞪濰 蕨擒醞 в萄 跨蝶攪 奢問 碳陛
    #define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.08 / 梯霞�� / 瞪濰 蕨擒醞 憮幗 摹鷗 碳陛
    /*($Delete_[x/C]$)*/#define _NA_006708_20130509_BUG_ROOM_STRING_LIMIT // 2013.05.09 / �姨瘉� / [寡ぎ襄&蘋螃蝶襄] 寞擊 儅撩й 陽 寞薯跡擊 и旋 15濠 艙僥/璋濠 30濠煎薯и 熱薑 濛機
    #define _NA_006710_20130509_CRYSTALWARP_BUG // 2013.05.09 / 梯霞�� / 檜嘐 だ惚脹 觼葬蝶驍擊 霤褻ж雖 跤й 唳辦 鬼薯煎 棻擠 觼葬蝶驍擊 霤褻ж朝 幗斜
    #define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME // 2013.05.02 / 梯團繹, 梯霞�� / 啪歜 褫暮 蹺陛(囀蝶え 爾檜晦 褫暮)
    #define _NA_000000_20130521_NOT_SUMMON_SKILL // 2013.05.21 / 梯團繹 / 憮試蝶鑒UI囀萄 薑葬
    #define _NA_006728_20130520_SKILL_BALANCE_MODIFY // 2013.05.20 / 梯霞�� / 蝶鑒 寨楛蝶 熱薑( 奩餌婦溼 蝶鑒 熱薑 )
    #define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / 薑⑷撩 / 橾橾蠡蝶お 蟾晦�� 嬴檜蠱
    #define _NA_000000_20130529_STATUS_EXCEPTIONS_AS_NPC_GRADE // 2013.05.29. / 薑⑷撩 / NPC蛔晝縑 評艇 鼻鷓檜鼻 �捕� 瞳辨 蕨諼 о跡
    #define _NA_006749_20130604_NOT_LIMIT_DARKBREAK // 2013.06.04 / �姨瘉� / 棻觼 粽溯檜觼 蝶鑒曖 蛔晝 跨蝶攪 薯и擊 ж雖 彊朝 濛機
    #define _NA_006731_20130521_ENCHANT_ADD_OPTION // 2013.05.21 / 梯霞��, 歎雙謙 / 譆鼻晝(13~15)檣臘お 褫暮 蹺陛
    #define _NA_000000_20130612_RANDOMIZER_CHANGE // 2013.06.12. / 薑⑷撩 / 檣疆お 楠渾 л熱 滲唳
    /*($Delete_[x/C]$)*/#define _NA_006772_20130614_BUG_ENERGY_SUCTION_CONTINUE_ANIMATION // 2013.06.14 / �姨瘉� / 憎紫辦 贗楚蝶 醴蝶 啗翮 3ч曖 "縑傘雖 戮暮" 蝶鑒擊 餌辨ж棻 議葛攪陛 餌蜂и 鼻鷓縑憮 蝶鑒 擁棲詭檜暮檜 雖樓腎朝 ⑷鼻 熱薑 濛機
    #define _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES // 2013.06.17 / 梯團繹, �姨瘉� / 餌辨ж雖 彊朝 葬模蝶 薑葬 濛機
    /*($Delete_[x/C]$)*/#define _NA_006781_20130627_BUG_SHOW_OTHERPLAYERNAME_OF_GAMEOPTION // 2013.06.27 / �姨瘉� / 褫暮 啪歜 龔縑憮 "棻艇 Ы溯檜橫 檜葷" 晦棟擊 Offж賊 頂 議葛攪曖 檜葷紫 轎溘腎雖 彊朝 ⑷鼻 熱薑 濛機
    #define _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2 // 2013.07.10. / 薑⑷撩 / �挫斲冗袬�2 - �僱瞏繳恓� 橾薑 晦除 餌辨
    #define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM // 2013.07.03 / 梯霞��, 歎雙謙 / 瞪濰 癱ル 衛蝶蠱 蹺陛
    #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / 梯褐鷓 / 鼻鷓嬴檜夔(幗Щ) ル衛ж朝 寞衝擊 滲唳
    #define _NA_000000_20130726_RENEWAL_POINT_REWARD // 2013.07.26 / 梯團繹 / 爾鼻璽 葬景橡

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / 梯褐鷓 / 鼻鷓嬴檜夔(幗Щ) ル衛ж朝 寞衝擊 滲唳
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / 薑⑷撩 / 詩じ贗塭檜樹お
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 梯褐鷓 / 詩じ贗塭檜樹お 3偃梱雖虜 褒ч腎啪莠
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _RU_006860_20130806_COSTUME_PCBANG_EFFECT  //2013.08.06 / 團唳蝓 / 囀蝶え縑 Я衛寞 褫暮 瞳辨衛酈晦
        //#define _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP // 2013.12.23 / �姨瘉� / 窒窒葆 換餌塋 裘 殮濰陛棟ж紫煙 ж朝 濛機
    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.3.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1301
/////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_006363_20130103_NOT_CALC_ENCHANT_STAT // 2013.01.03 / 薑⑷撩 / 檣疆お衛 隸陛ж朝 蝶邐 蹂掘纂 瞳辨ж雖 彊晦
    #define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / 薑⑷撩 / 漯雖 嬴檜蠱
	#define _NA_000000_20130114_RENEWER_UI // 2013.01.14 / 梯團繹 / 啪歜 UI 葬景橡 
    #if defined(_NA_000000_20130114_RENEWER_UI)
        //#define _NA_000000_20130218_OPTION_TOOLTIP      // 2013.02.18 / 梯褐鷓 / 葬景橡脹 褫暮璽縑憮 罐そル衛ж晦
    #endif 
    #define _NA_000000_20130206_BROADCAST_ITEM_SCORE    // 2013.02.06 / 梯褐鷓 / 棻艇Ы溯檜橫曖 濰綠ゎ薄擊 斜傖 爾頂輿紫煙 滲唳
    #define _NA_006413_20130201_PREMIUMSERVICE_ITEM // 2013.02.01 / 薑⑷撩 / PC寞 �膩� 嬴檜蠱
    #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 梯霞�� / 嫦酈葬曖 模�紡鷏� 模�� ж罹 跨蝶攪蒂 奢問, 模�紡鰤� 議葛攪 斬籀煎 檜翕 й 陽 梱雖 議葛攪蒂 菴煎 檜翕衛鑒 唳辦 跨蝶攪陛 夥爾 腎朝 ⑷鼻
    #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / 梯霞�� / Я曖撩瞪縑憮 錶Щ觼葬蝶驍擊 模�� �� 議蝶た檜 棻 腎晦瞪 模�納� 嶸盪陛 避橫紫 觼葬蝶驍檜 模�秘Ж� 幗斜
    #define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG // 2013.03.19 / 梯霞�� / 萄楚堆釭檜お煎 鼠瞳鼻鷓縑憮 辨滲褐 寰腎朝 幗斜 熱薑
    #define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY // 2013.03.25 / 梯霞�� / 瞪濰 蹺陛濛機 #2 <瞪濰 瞪癱 鼻�� 憲葡>
    /*($Delete_[x/C]$)*/#define _NA_006539_20130319_ITEM_COMPOSIT_CERTAIN // 2013.03.19 / �姨瘉� /  薯濛 撩奢睦ル衛蒂 86%~100蒂 衙辦 堪擠縑憮 100% 橾 陽 '�捐�'煎 嗑紫煙 碟葬 熱薑
    #define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG // 2013.03.28 / 梯霞�� / 模�粉�(嫦酈葬)曖 鼻鷓 醞 蝶鑒籐顫歜 隸馬 鼻鷓陛 模�紡鶹※� 瞳辨檜 寰腎朝 幗斜
    #define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY // 2013.04.01 / 梯霞�� / 嘐暮 塽 瞪濰縑憮 模�紡鶺� 瞪癱唸婁 等檜攪蒂 瞪癱唸婁璽縑 奩艙 (瞳縑啪 陛и 等嘐雖, 模�紡鶺� 鑒熱, ��榆)
    #define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO // 2013.04.01 / 梯霞�� / 嫦酈葬 模�紡鶺� �� 蝶鑒檜 模�粉�(輿檣)曖 蝶鑒奢問溘 綠徽縑 蜃啪 蹺陛 ��榆檜 啗骯腎雖 彊擎幗斜
    #define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL // 2013.04.09 / 梯霞�� / 頂陛 鬼繹 賅萄橾 唳辦 瞳檜 釭縑啪 蝶鑒檜 衛瞪腎朝 ⑷鼻 熱薑.
    #define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME // 2013.04.02 / 梯霞��, 歎雙謙 / GM貲滄橫煎 檜漸お 瞪濰 衛濛
    /*($Delete_[x/C]$)*/#define _NA_006639_20130415_DEATH_AND_NOT_RELIEVE_RIDER // 2013.04.15 / �姨瘉� / 驍匙擊 驕 鼻鷓縑憮 餌蜂 衛 驍匙 模�秣� 陳晦陛 п薯腎雖 彊朝 ⑷鼻 幗斜 熱薑
    /*($Delete_[x/C]$)*/#define _NA_006640_20130415_VIEW_INCREASE_HEAL // 2013.04.15 / �姨瘉� / 嫦酈葬 模�紡� "儅貲曖 薑滄"曖 �� 蝶鑒縑 模�粉蝩� 蝶鑒 奢問溘 蹺陛 ��榆檜 瞳辨腎雖 彊朝 ⑷鼻 熱薑
    #define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / 團唳蝓 / 檜翕+婦鱔⑽ 蝶鑒檜 1貲曖 瞳縑啪虜 Яп蒂 邀
    #define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / 團唳蝓 / 樂煎睡渠察雖諦 醴Ы膩濰察雖蒂 л眷 陛韓 熱 氈紫煙 л
    /*($Delete_[x/C]$)*/#define _NA_006647_20130419_TOOLTIP_VIEW_GENDER_COLOR // 2013.04.19 / �姨瘉� / 罹撩 嫦酈葬 議葛攪陛 "罹撩 縣葬詮驍葬蝶お辨 ��Я鼠棧 曖鼻"縑 葆辦蝶 螃幗衛 餌辨 陛棟 贗楚蝶 轎溘 睡碟檜 �羃鶺虞� 轎溘腎朝 ⑷鼻
	#define _NA_006644_20130419_BUG_QUEST_KILLMONSTER               //2013.04.19 / 團唳蝓 / 跨蝶攪鑒 蠡蝶お 醞犒熱ч 陛棟
    #define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS // 2013.04.22 / 梯霞�� / 辨滲褐 鼻鷓縑憮 觼葬蝶驍は葬蝶 嬴檜蠱 �側瘓亳� 雜辨ж堅 氈朝 濰綠陛 瞳辨腎雖 彊擎 幗斜
    #define _NA_006655_20130422_BUG_ACCESSORY_SETOPTION           //2013.04.22 / 團唳蝓 / 跡勘檜+奩雖 撮お嬴檜蠱褫暮 醞繪僥薯

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / �姨瘉� / ぅ饜葬橡 в萄縑憮紫 寡ぎ襄縑 霤陛 й 熱 氈紫煙 偃摹
        //#define _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL // 2013.05.27 / �姨瘉� / 檜漸お 晦除 醞 縣模萄 葆擊煎 陛朝 錚檜 ん檣お 溯漣 ル衛蒂 譆模~譆渠 溯漣 ル衛煎 偃摹

    #elif defined(_GLOBAL)
     
    #elif defined(_RUSSIA)

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.2.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1204
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20120920_STARTMOTION_CODE // 2012.09.20 / 梯褐鷓 / 幗憮醴 萄釭 CE08, 釭該雖 CE00 戲煎 餌辨ж堅 氈湍 議葛攪 衛濛賅暮擊 CE00戲煎 鱔橾
	#define _NA_000000_20120911_FULL_MISSION_HELPER // 2012.10.08 / 梯團繹 / 嘐暮 薑爾 晦棟 1204煎 蛤だ檣 螢葡.
    #define _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY // 2012.10.31 / 歎雙謙 / 萄楚堆釭檜お 蝶Я葩蝸楝斜 蝶鑒曖 嫦翕褻勒擊 憎紫辦 棻觼粽溯檜觼諦 翕橾ж啪 滲唳
    #define _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT // 2012.10.31 / 歎雙謙 / uiEventWindow 縑 檜漸お 醞橾 陽諦 嬴棍 陽 渠餌蒂 釭揹
    #define _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT // 2012.11.13 / 歎雙謙 / FTEXT 轎溘 衛 壽曖 澀葬朝 艙羲 斜葬晦
    #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / 梯霞�� / 縑纔腦驕�� 薑疇 滲唳(晦獄奢問溘虜 -> 瞪羹等嘐雖)
    #define _NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE // 2012.10.15 / 薑⑷撩 / Я問衛 ж檜萄鼻鷓 п薯 薯剪
    #define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD // 2012.11.16 / 梯霞�� / 聽粽衛蝶蠱 碟п衛 爾鼻葬蝶お縑憮 議衛牆萄 薯剪
    #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / 薑⑷撩 / 顫塊и 餌錳縑憮 議葛貲 憲溥輿雖 彊晦
    #define _NA_000000_20121210_REFACTORY_GAME_USER // 2012.12.10 / 梯團繹 / 褐敘嶸盪 寰雜擊 嬪и 偃摹 餌о 濛機.
    #define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2012.12.11 / 梯霞�� / 偃檣 錚檜ん檣お 餉薯 渠晦 衛除 熱薑
    #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 梯霞�� / 模�紡鶺� 輿檣檜 鬼繹賅萄橾 唳辦 奢問,Я問,蝶鑒餌辨 陛棟 -> 碳陛棟戲煎 熱薑
    #define _JP_20110222_INGAME_CHARNAME_CHANGE
 
    #if defined(_KOREA)
        #define _JP_20110222_INGAME_CHARNAME_CHANGE

    #elif defined(_CHINA)
        #define _CN_CHANGE_ICON     // 2013.05.30 / 梯褐鷓 / 醞措 ぷ綰葬敷 滲唳縑 蜃醮 嬴檜夔 滲唳
        #define _CN_000000_20130605_MATRIX_CARD_REMOVE // 2013.06.05 / 梯霞�� / 醞措 詭お葛蝶 蘋萄 薯剪
        #define _CN_000000_20130605_XIGNCODE_UPDATE // 2013.06.05 / 梯霞�� / 醞措 諄檣囀萄 機等檜お
        #define _CH_006797_20130703_PK_DROP_RULE_MODIFY // 2013.07.03 / 梯霞�� / 醞措 в萄 PK 薑疇 滲唳
        #define _CN_000000_20130729_NOT_USE_CUBE // 2013.07.29 / 梯團繹 / 聽粽 餌辨 薯и.
        #define _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK // // 2013.07.29 / 梯霞�� / 醞措 聽粽 м撩 or 碟п �鷜瘍銝� 薯剪
        //#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG // 2013.07.31 / 梯霞�� / 醞措 鬼繹賅萄 幗斜 婦溼 煎斜晦煙
        #define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / 團唳蝓 / 醞措 PK縑憮 餌蜂衛, 瞳渠婦啗縑憮紫 嬴檜蠱檜 萄奧脹棻.
        #define _NA_000000_20131031_GAMELOG_REDUCE // 2013.10.31 / 薑⑷撩 / 啪歜煎斜 還檜晦
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 梯霞�� / 模�紡鶺� 輿檣檜 鬼繹賅萄橾 唳辦 奢問,Я問,蝶鑒餌辨 陛棟 -> 碳陛棟戲煎 熱薑
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 梯霞�� / 嫦酈葬曖 模�紡鷏� 模�� ж罹 跨蝶攪蒂 奢問, 模�紡鰤� 議葛攪 斬籀煎 檜翕 й 陽 梱雖 議葛攪蒂 菴煎 檜翕衛鑒 唳辦 跨蝶攪陛 夥爾 腎朝 ⑷鼻
    #endif
#endif 

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.2.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1203
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_004157_20120409_QUEST_BANDWITH_4000 // 2012.04.00 / 薑⑷撩 / 蠡蝶お 渠羲ァ �挫�
    #define _NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE // 2012.08.07 / 薑⑷撩 / Я問衛 楝衛鼻鷓 п薯 碳陛

    #define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / 梯⑷錠, 檜鷓遽 / 褐敘 議葛攪 嘐蝶す 蹺陛
    #ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / 檜鷓遽 / 褐敘 議葛攪 贗塭檜樹お 熱薑(蝶觼董おだ憮婦溼)
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / 檜鷓遽 / 褐敘 贗楚蝶 顫殮 蹺陛縑 評艇 顫殮MAX 隸陛煎 檣и 嫦儅腎朝 螃盟 歜衛 熱薑
    #endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC

    #define _NA_000000_20120527_CHAOS_ZONE // 2012.05.27 / 歎雙謙,梯霞�� / 蘋螃蝶襄 衛蝶蠱.
    #ifdef _NA_000000_20120527_CHAOS_ZONE
        #define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND // 2012.06.27 / 歎雙謙, 梯霞�� / 蘋螃蝶 襄 瞪濰 衛蝶蠱.
        #define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP // 2012.09.10 / 歎雙謙, 梯霞�� / 蘋螃蝶 襄 濠嶸瞪癱 衛蝶蠱.
        #define _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION // 2012.09.14 / 歎雙謙, 梯霞�� / 蘋螃蝶 襄 瞪濰 衛蝶蠱 �側瘓�
    #endif //_NA_000000_20120527_CHAOS_ZONE

    /*($Delete_[S/x]$)*/ //#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP // 2012.09.04 / 梯霞�� / 嬴檜蠱 夥款幗葬晦 碳陛棟, PK萄照陛棟 顫殮 蹺陛.
    #define _NA_006057_20120904_PK_DROP_RATIO_MODIFY // 2012.09.04 / 梯霞�� / 濠褐曖 溯漣 爾棻 堪擎 Ы溯檜橫蒂 避艘擊 唳辦 避擎 Ы溯檜橫曖 嬴檜蠱萄奧睦 熱薑 鼻и摹 100%->50%

	#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / 梯褐鷓 / 囀蝶え嬴檜蠱 濰雜衛 嬴檜蠱滌 渠晦翕濛 瞳辨
    #define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / 薑⑷撩,檜紫⑽ / 偃檣鼻薄 匐儀 衛蝶蠱    
    #define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY // 2012.09.26 / 梯霞�� / 寡ぎ襄 殮濰蕨擒 辦摹牖嬪 滲唳. (譆蟾 -> 譆褐牖)
    #define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY // 2012.10.12 / 梯霞�� / PVP瑛縑憮 だじ薑爾璽曖 ⑷營嬪纂陛 澀跤 ル衛 腎朝 幗斜 熱薑
    #define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD // 2012.10.15 / 梯霞�� / 寡ぎ憮幗縑憮 в萄憮幗煎 檜翕衛 綠奢薑 в萄紫 陛棟ж紫煙 熱薑
    #define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY //2012.10.17 / 梯霞�� / 給霞 塽 婦鱔啗翮 蝶鑒 諒觼塽 っ薑 熱薑
    #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / 梯霞�� / 縑傘雖戮暮擊 衛瞪ж朝 紫醞 奢問碳陛鼻鷓(蝶欐, 棻遴..)陛 腎橫紫 蝶鑒檜 啗樓 衛瞪腎朝 幗斜

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / 梯霞�� / 縑纔腦驕�� 薑疇 滲唳(晦獄奢問溘虜 -> 瞪羹等嘐雖)
        #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / 薑⑷撩 / 顫塊и 餌錳縑憮 議葛貲 憲溥輿雖 彊晦

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 梯褐鷓 / 詩じ贗塭檜樹お 3偃梱雖虜 褒ч腎啪莠

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 梯霞�� / 模�紡鶺� 輿檣檜 鬼繹賅萄橾 唳辦 奢問,Я問,蝶鑒餌辨 陛棟 -> 碳陛棟戲煎 熱薑
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 梯霞�� / 嫦酈葬曖 模�紡鷏� 模�� ж罹 跨蝶攪蒂 奢問, 模�紡鰤� 議葛攪 斬籀煎 檜翕 й 陽 梱雖 議葛攪蒂 菴煎 檜翕衛鑒 唳辦 跨蝶攪陛 夥爾 腎朝 ⑷鼻
        #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / 梯霞�� / Я曖撩瞪縑憮 錶Щ觼葬蝶驍擊 模�� �� 議蝶た檜 棻 腎晦瞪 模�納� 嶸盪陛 避橫紫 觼葬蝶驍檜 模�秘Ж� 幗斜
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.2.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1202
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / 薑⑷撩,檜鷓遽 / 望萄 葬景橡
    #ifdef _NA_003923_20120130_GUILD_RENEWAL
        #define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / 薑⑷撩,檜鷓遽 / 望萄璽堅 塽 偃檣璽堅 蝸煜偃熱 �挫�
        #define _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER // 2012.02.17 / 檜鷓遽 / uiGuildMan 營濛撩
        #define _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL // 2012.02.22 / 檜鷓遽 / 望萄婦溼 UI 葬景橡 
        #define _NA_004509_20120314_QUESTDROPINDEX10 // 2012.03.12 / 薑⑷撩 / 蠡蝶お 萄照 в萄 �挫�.
        #define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / 薑⑷撩,檜鷓遽 / 望萄 囀檣 濛機
        //#define _NA_004034_20120102_GUILD_INTRODUCTION //2012.05.10 / 薑⑷撩,檜紫⑽ / 望萄 模偃 濛機
    #endif //_NA_003923_20120130_GUILD_RENEWAL

    //#define _NA_20120220_JUMP_RENEWAL // 2012.02.20 / 梯褐鷓 / 薄Щ 葬景橡
    #define _DH_NPC_GROUP_NAME  // 2012.3.26 / 檜紫⑽ / NPC 塽 跨蝶攪 Group 啻歜 ル衛(該葬嬪縑 蹺陛 檜葷)
    #define _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE // 2012.4.02 / 檜紫⑽ / 薑爾翮塋褒 檜嘐雖 偃摹濛機
    //#define _NA_004157_20120409_QUEST_BANDWITH_INCREASING // 2012.04.00 / 薑⑷撩 / 蠡蝶お 渠羲ァ �挫�
	#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                    // 2012.04.18 / 梯霞�� / 寡ぎ襄 寞廓�ㄧ� 醴嗽棲じ �飛橦� ル衛.
    #define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE    // 2012.04.18 / 梯霞�� / 寡ぎ襄 寞 蕨擒 衛 綠塵寞擎 殮濰罹睡 詭撮雖 爾頂雖 彊擠.
    #define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST      // 2012.04.18 / 梯霞�� / 蕾樓 謙猿衛 蕨擒脹 寡ぎ襄 寞 葬衛お蒂 餉薯и棻.
    #define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / 梯⑷錠, 梯褐鷓 / 1202 蝶鑒 葬景橡
    #define _NA_0_20120321_SKILLTREE_SECOND_RENEWAL // 2012.03.15 / 梯⑷錠, 梯褐鷓 / 1202 蝶鑒 葬景橡 (蝶鑒お葬, へ, 擋暮)
    #define _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT    // 2012.04.26 / 蝶鑒 嬴檜夔 巍爾 翱轎 蹺陛
    #define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / 梯⑷錠, 梯褐鷓 / 蝶鑒 奢問縑 縑纔腦 驕�� 瞳辨
    #define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING // 2012.04.26 / 嶸營錳, 檜紫⑽ / 陳偃 爾歜 罹睡 褫暮 濛機
    #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 梯霞�� / 嫦酈葬 模�紡鰤� 顫啃擊 熱翕 奢問ж朝 翕寰, 檜嘐 �側瘓音� 醴裔萄 嘐 犒謙.

    #define _NA_004756_120504_SHOW_SSQ_PLAYER_NAME              // 2012.05.04 / 歎雙謙 / 顫塊и 餌錳縑憮 Ы溯檜橫 檜葷 爾檜晦 GM貲滄橫 蹺陛
    #define _DH_LAST_SELECTED_CHARACTER /// [2012-5-11 by i4u4me] 議葛攪 摹鷗�飛橦□� 譆斬 Ы溯檜и 議葛攪 摹鷗
    #define _NA_004644_20110506_QUEST_NAVIGATION // 2012.05.16 / 檜紫⑽ / 蠡蝶お 啻綠啪檜暮 儀鼻 薑爾 鬼��

    #define _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL   // 2012.06.18 / 歎雙謙 / 檜翕 衛 蝶鑒 聽濘 п薯 (�側瘓音� 葆徹霞 薯諼)
    #define _NA_004859_20120518_FORCE_INPUT_SCREENSHOT_KEY    // 2012.05.18 / 梯褐鷓 / 瓣た醞 蝶觼萼憐酈 殮溘陛棟ж紫煙 熱薑
    #define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE    // 2012.05.11 / 梯霞�� / 顫饕и 餌錳 醞懈 絳 掘羲 む割じ 雖羲 彰嬪 營 撲薑 塽 唸啗 雖羲 頂 PVP陛棟.
    #define _NA_004910_20120601_SUMMON_PVP_RULE                     // 2012.06.01 / 梯霞�� / 嫦酈葬 模�紡� SD(99):HP(1) 瑙 瞳辨.
    /*($Delete_[x/C]$)*/#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / 梯⑷錠, 檜鷓遽 / 薄滄瞪 爾鼻 褫暮 瞳辨 寞衝 滲唳
    #define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST  // 2012.05.11 / 團薑艙, 梯⑷錠, 檜鷓遽 / 雖羲 薄滄瞪 蹺陛 蹂羶 餌о(殮雞望萄溯漣薯и 塽 奢問望萄摹薑)
    #define _NA_004035_20120227_COSTUME_ITEM                        // 2012.05.23 / 梯霞��, 梯褐鷓 / 囀蝶え 嬴檜蠱
    #define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET   //2012.06.14 / 梯霞�� / へ�膩�<渠鼻曖 寞橫溘 馬模徽瞳辨>
    #define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY            // 2012.06.21 / 梯霞��, 歎雙謙 / 寞橫 撩奢睦 鼻и摹 熱薑 99% -> 80%
    #define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE       // 2012.06.22 / 梯霞�� / 嫦酈葬 模�紡� PVP瑙擊 輿檣婁 偽擎 瑙 瞳辨.
    #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR            // 2011.10.27 / 梯褐鷓 / 溯檜渦縑 瓣餵陛棟и 螃粽薛お曖 嬴檜夔ル衛
	#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 梯霞�� / 望萄囀檣 ж瑞 晦睡 譆渠 偎熱 蝶觼董お 等檜攪煎 瞳辨

    #if defined(_KOREA)
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / 薑⑷撩 / 詩じ贗塭檜樹お
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 梯褐鷓 / 詩じ贗塭檜樹お 3偃梱雖虜 褒ч腎啪莠
    #elif defined(_CHINA)
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 梯霞�� / 望萄囀檣 ж瑞 晦睡 譆渠 偎熱 蝶觼董お 等檜攪煎 瞳辨
        #define _CN_006014_20120822_GUILD_LOG_DELETE        // 2012.08.22 / 梯霞�� / 醞措 望萄煎斜縑 諾曖隸ル 殮雞偎熱 煎斜 餉薯.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#ifndef _NA_COSTUMEITEM_IDLE_ANIMATION	// 旋煎弊虜 1202, 釭該雖朝 1203睡攪
			#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / 梯褐鷓 / 囀蝶え嬴檜蠱 濰雜衛 嬴檜蠱滌 渠晦翕濛 瞳辨
		#endif

    #elif defined(_RUSSIA)
        #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / 梯霞�� / 縑傘雖戮暮擊 衛瞪ж朝 紫醞 奢問碳陛鼻鷓(蝶欐, 棻遴..)陛 腎橫紫 蝶鑒檜 啗樓 衛瞪腎朝 幗斜
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 梯霞�� / 望萄囀檣 ж瑞 晦睡 譆渠 偎熱 蝶觼董お 等檜攪煎 瞳辨
    #endif

#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.2.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1201
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(0)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////



    #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 陴晦瑰 / だじ錳 檣錳熱縑 評艇 ж歜 陛醞纂 滲唳.
    #define _NA004034_20120102_POINT_WALLET_SYSTEM // 2012.01.02 / 薑⑷撩,檜鷓遽 / �倖� 雖骨 濛機
    #define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / 陴晦瑰,檜鷓遽 / 萄楚堆 釭檜お諦 辨滲褐 1,2 離曖 蝶鑒 奢嶸 晦棟 蹺陛.
    #define _NA_003649_20111115_BATTLE_ZONE_HARDMODE    // 2011.11.15 / 薑⑷撩 / 檜紫⑽ / 寡ぎ襄.ж萄賅萄 蹺陛
    #define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / 梯⑷錠, 梯褐鷓 / 楠渾 嬴檜蠱 儅撩
    #define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / 梯⑷錠, 梯褐鷓 / 譆鼻晝 檣臘お
    #define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / 梯⑷錠 / SD 婦溼 奢衝 滲唳
    #define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / 梯⑷錠 / 嫦酈葬 奢問溘 奢衝 滲唳
    #define _SCJ_111202_UI_RENEWAL // 2011.12.02 / 歎雙謙 / UI 葬景橡 濛機 (蠡蝶お,ac,議葛攪璽)
    #define _SCJ_120104_SORT_QUEST_VIEW_PROGRESS // 2012.01.04 / 歎雙謙 / 蠡蝶お 嘐葬爾晦 薑溺ж晦
    #define _NA_20111118_CASHSHOP_UI_RENEWAL // 2011.11.18 / 歎雙謙 / 議衛憧 UI 葬景橡 (縛霞濛機x)
    #define _DH_KEY_BINDING // 2011.11.29. 檜紫⑽, 檜鷓遽 / 嶸盪 欽蹴酈 撮た 雖錳
    #define _NA_20111213_SKILLTREE_VISUAL_RENEWAL2 //2011.12.13 / 梯褐鷓 / 蝶鑒瑰檣п薯 翱轎 葬景橡
	#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN // 2010.01.06 / 團薑艙 / 諾曖 隸ル 奩�秘Щ� 彊紫煙 熱薑
    #define _DH_CHANGE_CHAT_HISTORY // 2012.01.09 檜紫⑽ / 瓣た �鷜瘍銝� �側瘓� 酈 滲唳
    #define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / 梯⑷錠 / 啪歜憮幗 ObjectFactory 偃摹
    //#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO // 2011.06.10 / 團薑艙 / 雖羲 薄滄瞪 羹割 薑爾 iniだ橾 滲唳(ServerOptionEX.ini -> GameOption.ini)
    #define _DH_MONEY_COLOR_CHANGE // 2012.02.08 / 檜紫⑽ / ж歜 欽嬪滌 儀鼻 熱薑
    #define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / 梯⑷錠 / ObjectKey 渠羲ァ �挫�
    #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / 薑⑷撩 / 歎雙謙 / 寡ぎ襄.だじ錳瞪錳蝗菜
    #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
    //#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 梯⑷錠 / 啪歜 褫暮 お楠燮暮 籀葬 晦棟 蹺陛
    #define _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR // 2012.03.14 / 檜紫⑽ / 偃檣鼻薄 偃滌陛問 塽 儀鼻 瞳辨
    #define _DH_SHOW_VENDOR_LIST_POPUP // 2012.03.22 / 檜紫⑽ / 偃檣鼻薄 葬蝶お縑憮 辦贗葛戲煎 議葛攪 で機詭景 晦棟 瞳辨

    #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT   // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)
    #define _DH_DEBUG_REPORT   // 2012.02.14 / 檜紫⑽ / 觼楚蓮 葬んお 熱餵
    #define _NA_0_20120321_CASHSHOP_POPUP_EFFECT            // 2012.03.21 / 梯褐鷓 / 議衛憧 掘衙�挫恞� 熱榆摹鷗睡碟 ж檜塭檜お ル衛

    #define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE    /// [2012-4-24]  / 歎雙謙 , 嶸營錳 / 瓣餵 ぬ韃縑 蠡蝶お 囀萄 蹺陛
    #define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    // [2012-4-24]  嶸營錳 / 避歷擊陽 棟溘纂 螢塭陛雖 彊啪 л

    #define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] 醞措 詭賅葬р  
    #define _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER /// [2013-10-22] 梯褐鷓, 薑⑷撩 / �鷑貔怹� 檜翕樓紫, 奢問樓紫蒂 贗塭縑憮 啗骯ж雖 彊堅 憮幗縑憮 爾頂輿朝 高擊 檜辨ж紫煙 熱薑


    #if defined(_KOREA)
        #define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / 陴晦瑰 / 懍���� 賅菊 MD5 -> SHA256 滲唳.
        #define _NA_20120313_CASHSHOP_DISABLE_ALLITEM   // 2012.03.13 / 梯褐鷓 / 議衛憧 '賅萇嬴檜蠱' 蘋纔堅葬 ル衛ж雖 彊紫煙 滲唳
        #define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / 薑⑷撩 / 盪輿嫡擎 鱉 ж歜 萄奧榆 滲唳


    #elif defined(_CHINA)
        //#define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] 醞措 詭賅葬р  
        #define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 梯⑷錠 / 啪歜 褫暮 お楠燮暮 籀葬 晦棟 蹺陛

    #elif defined(_JAPAN)
        #undef _DH_CHANGE_CHAT_HISTORY // 瓣た �鷜瘍銝� �側瘓� 酈 滲唳 - 橾獄縑憮朝 瞳辨 寰л
		#define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / 薑⑷撩 / 盪輿嫡擎 鱉 ж歜 萄奧榆 滲唳

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.1.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1103
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(9)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET // 2011.11.10 / 歎雙謙 / 頂辨滲唳 氈擊 陽縑虜 濠翕餌傖 褫暮 盪濰
    #define _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC // 2011.11.04 / 歎雙謙 / FTEXT 顫殮 蹺陛 - 蠡蝶お衛濛/諫猿 (NPC檜葷 儀鼻/罐そ 蹺陛)
    #define _SCJ_ACSYSTEM_OVERLAP_PARENT // 2011.10.05 / 歎雙謙 / AC婁薯 跡煙擊 摹ч婁薯煎 僎纂晦
    #define _NA_0_20111013_RADAR_PATHFINDING // 2011.10.13 // 梯褐鷓 / NPC 望瓊晦 晦棟蹺陛
	#define _NA_003033_20110831_DICECAST_ITEM_ROUTING	// 2011.08.31 / 薑⑷撩, 歎雙謙 / 輿餌嬪 �僱� 寞衝 蹺陛
    #define _YMS_TREECONTROL   //! 2011.6.13 / i4u4me /  お葬夔お煤 / WZENGINEVER >= 400
    #define _NA_003027_20111013_HONOR_SYSTEM								// 貲蕨衛蝶蠱 (薑⑷撩, 檜鷓遽)
    #define _NA_002935_20110704_ITEM_SCORE // 2011.07.04 / 梯⑷錠, 歎雙謙 / 濰綠 ゎ陛 薄熱 ル衛
    #define /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL// 2011.06.30 / 團薑艙, 檜紫⑽ / 寡ぎ襄 葬景橡

    #ifdef /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL
        #define _DH_BATTLEZONE2_// 2011.06.30 / 檜紫⑽ / 寡ぎ襄 葬景橡(贗塭辨)
        #define _DH_BATTLEZONE2_LIST_PAGE //2011.06.30 / 檜紫⑽ / 寡ぎ襄 葬景橡: 寡ぎ襄 葬蝶お む檜雖 寞衝戲煎 滲唳
        #define _DH_BATTLEZONE2_LOBBY_SLOT//2011.06.30 / 檜紫⑽ / 寡ぎ襄 葬景橡: 煎綠蝸煜 牖憮 滲唳(寞濰譆蟾 嬴棲啪)
        #define _NA_LEAVE_PARTY_AT_USE_ITEM_TO_MISSION//2011.11.24 / 團薑艙 / 嘐暮 嬴檜蠱 餌辨衛 だじ 驍黴 晦棟
    #endif//

    /*($Delete_[x/C]$)*/#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT// 2010.01.16 / 團薑艙, 檜紫⑽ / 薄滄瞪 寞漁 等檜攪 蹺陛
    #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM// 2011.06.13/ 檜紫⑽, 梯⑷錠 / 薑爾翮塋褒
    #define _NA002635_GZ_DZ_PROTOCOL_UINIFICATION // 檜擎だ / Guild 憮幗 薯剪 塽 DG Щ煎饜屬婁 鱔м 濛機
    #define _NA002823_WINGS_EQUIPMENTS_EXTENTION // 檜擎だ, 歎雙謙 / 陳偃 嬴檜蠱擊 か學+寞橫掘 偃喃戲煎 �挫�
    //#define _NA000000_SPEED_HACK_PREVENTION_SUPPORT // 檜擎だ, 嶸貲熱 / SpeedHack檜蓬 寞雖 籀葬 = { 贗塭檜樹お 衛除 濠羹 匐餌 + 緒艇 檜翕 蹂羶 寞雖 + Ы溯檜橫 檜翕 樓撩 鼠唸撩 匐餌 }
    #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // 嶸貲熱, 檜擎だ / 奢問 蝶顫橾 翱轎 偃摹
    #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // 檜擎だ, 嶸貲熱 / 嘐暮 ん驍 翱轎 鬼�� (顫檜該 塽 ん驍 翱唸 �� 濠霞 驍黴 晦棟 雖錳)
    #define _NA_002884_AC_DIALOG_RENEWAL // 2011.09.08 / 檜鷓遽 / AC 璽 葬景橡 婁薯о跡 10->20偃 轎溘
    #define _SCJ_EXP_EVENT_ENABLE_EFFECT // 2011.09.23 / 歎雙謙 / 唳я纂 檜漸お 憲葬嘐 晦棟
    /*($Delete_[x/C]$)*/#define _NA_003457_20111013_EXCEPT_DOMINATION_REWARD // 2011.10.13 / 薑⑷撩 / Я曖撩瞪,顫塊и餌錳 檣濰�膩� 薯剪
    #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 陴晦瑰, 啪歜煎斜 辨榆縑 評艇 だ橾碟葬.
    #define _NA_003798_SPREAD_WING_KEY // 2011.12.13 / 檜鷓遽 / 陳偃 よ晦(綠ч賅萄) 酈 蹺陛(Shift+Space)
    #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS// 2011.12.20 / 團薑艙, 歎雙謙 / 寡ぎ襄 葬景橡 綠薑鼻 蕾樓 謙猿 嶸盪 籀葬

    #if defined(_KOREA)
        #define _NA_0_20110810_LOG_COLLECTOR_RESOTRE //僥鼻⑷ //煎斜 屬滓攪 濠翕 犒掘 晦棟
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / 陴晦瑰, 嶸貲熱/ 議衛憧 葬ね饜葭.

        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND // 2011.10.12 // 梯褐鷓 / 嬴檜蠱 晦除翱濰 晦棟蹺陛
        #endif
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / ぅ饜葬橡 蝶霾
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)

    #elif defined(_CHINA)
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / 陴晦瑰, 嶸貲熱/ 議衛憧 葬ね饜葭.
        #define _NA_20111118_CASHSHOP_UI_RENEWAL                        // 2011.11.18 / 梯褐鷓 議衛憧 UI 葬景橡

        #ifndef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      
            #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      // 2011.10.27 / 梯褐鷓 / 溯檜渦縑 瓣餵陛棟и 螃粽薛お曖 嬴檜夔ル衛
        #endif

        #if !defined(_NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT)
            #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 陴晦瑰 / だじ錳 檣錳熱縑 評艇 ж歜 陛醞纂 滲唳.
        #endif

    #elif defined(_JAPAN)
        #define _DH_DEBUG_REPORT   // 2012.02.14 / 檜紫⑽ / 觼楚蓮 葬んお 熱餵
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT               // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)
        #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / 薑⑷撩 / 歎雙謙 / 寡ぎ襄.だじ錳瞪錳蝗菜
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                    // 2011.06.13 / 陴晦瑰, 嶸貲熱/ 議衛憧 葬ね饜葭.
        #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS       // 2011.12.20 / 團薑艙, 歎雙謙 / 寡ぎ襄 葬景橡 綠薑鼻 蕾樓 謙猿 嶸盪 籀葬
        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND             // 2011.10.12 // 梯褐鷓 / 嬴檜蠱 晦除翱濰 晦棟蹺陛
        #endif
        //#define _YJW_TUTORIAL_SKIP      //! 2011.8.9 / i4u4me / ぅ饜葬橡 蝶霾 

    #elif defined(_GLOBAL)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL

    #elif defined(_RUSSIA)
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.        
        #define _SCJ_MAKE_DOUBLE_QUOTATION_MARKS // 2011.08.22 / 歎雙謙 / >0    <畦嬴憾 評褥ル詭衛雖 籀葬~!!
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 梯霞�� / 嫦酈葬 模�紡鰤� 顫啃擊 熱翕 奢問ж朝 翕寰, 檜嘐 �側瘓音� 醴裔萄 嘐 犒謙.
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / 陴晦瑰, 嶸貲熱/ 議衛憧 葬ね饜葭.
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 梯霞�� / 嫦酈葬 模�紡鰤� 顫啃擊 熱翕 奢問ж朝 翕寰, 檜嘐 �側瘓音� 醴裔萄 嘐 犒謙.
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.1.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1102
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

	#define _NA_001990_ACSYSTEM_ADD										// 2011.02.14 / 薑⑷撩, 歎雙謙 / AC 婁薯 謙盟 蹺陛
    #define _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX                      // AC 蝶觼董お 偃熱 薯и擎 橈擁堅, 議葛攪陛 熱чй熱 氈朝 偃熱虜 薯и
    #define _NA001955_110210_WING_COSTUME_ACCESSORY_EXTENTION // 2010.02.10 / 嶸貲熱, 檜擎だ / だ款だ款 濰雜 嬴檜蠱 婦溼 濛機, か學撮餌葬 偃熱薯и �挫撠�溼

    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_BLADEFURY_SONICBLADE_EFFECT_RENEWAL    // 2011.03.04 // 梯褐鷓 / 萄楚堆 蝶鑒 2謙 葬景橡 (綰溯檜萄 У葬, 模棣 綰溯檜萄, 褐敘 檜めお 葬模蝶諦 等檜攪陛 в蹂л)
    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_WEAPON_THROWING_PROJECTILE_RENEWAL     // 2011.04.08 // 錚ア噙煎濘 檜めお 葬景橡
    #define _NA_0_20110308_LOGSYSTEM_RENEWAL // 2010.03.08 // 梯褐鷓 / 煎斜 衛蝶蠱 葬景橡
    #define _NA000000_100427_SSQ_DEFAULT_REWARD_ // 檜擎だ, 嶸貲熱 / 顫塊и 餌錳 霤陛濠 瞪錳 晦獄 爾鼻 籀葬 (廓��:108)
    #define _NA001955_110210_WING_COSTUME // 2010.02.10 / 嶸貲熱, 檜擎だ / だ款だ款 濰雜 嬴檜蠱 婦溼 濛機
    #if defined(_NA001955_110210_WING_COSTUME)
        /*($Delete_[x/C]$)*/#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / 縣葬お ヴ撢 だお 掖�� ⑽衝縑憮 碟葬
    #endif 
    #define _NA001956_110210_PERK_SWITCHABLE_REGION // 2010.02.10 / 檜擎だ, 歎雙謙 / か掏曖 釭鼠 (へ 濠嶸 掖羹 雖錳)
    /*($Delete_[S]$)*/#define _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM // 2010.02.10 / 檜擎だ / 渠鼻 匐儀 憲堅葬闈 偃摹
    #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / 陴晦瑰, 嶸貲熱 / 議葛攪 綠蕾樓衛 蝶鑒 籐顫歜 嶸雖 晦棟 蹺陛.
    #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER // 2011.03.07 / 梯⑷錠, 檜鷓遽 / NPC 薯橫 お葬剪 蹺陛
    
    #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE // 2010.03.11 / 檜擎だ / NPC お葬剪 艙羲 檜漸お 晦奩 籀葬 睡碟
    #endif
    #define _NA_2463_20100328_OPTIMIZATION_RADAR_MAP_TEXTURE // 2011.04.22 / 檜鷓遽 / 嘐棲裘 臢蝶藥 葬模蝶 譆瞳�� 濛機
    /*($Delete_[S/x]$)*/#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN // 2011.04.06 / 團薑艙, 檜鷓遽 / 薄滄瞪 霤罹 寞衝 滲唳
    #define _NA_1937_20100207_NPC_CUSTOMIZING // 2010.03.28 / 檜鷓遽 / NPC醴蝶攪葆檜癒 雖錳
    #define _NA_2088_20100317_MODIFY_QUEST_AND_AC_LIST_ORDER // 2010.03.28 / 檜鷓遽 / 蠡蝶お/AC 葬蝶お 薑溺 塽 轎溘 滲唳
    /*($Delete_[S/x]$)*/#define _NA_002050_20110216_ADD_GENDER // 2011.02.16 / 梯⑷錠, 檜鷓遽 / 議葛攪 撩滌 蹺陛
    /*($Delete_[S/x]$)*/#ifdef _NA_002050_20110216_ADD_GENDER
        /*($Delete_[S/x]$)*///#define _USE_MAX_CHAR_LIST_PARSER // 議葛攪 譆渠 跡煙 撲薑 だ憮 餌辨 罹睡
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_CHARACTER_SOUND_INFO // 2011.05.24 / 檜鷓遽 / 議葛攪餌遴萄蝶觼董おだ憮 滲唳(撩滌в萄 蹺陛)
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_LOAD_CHARACTER_RESOURCE_IN_CHARACTER_SCENE // 2011.06.24 / 檜鷓遽 / 議葛攪壁縑憮 議葛攪儅撩 衛 葬模蝶煎註戲煎 檣и 雖翱 ⑷鼻 偃摹
        /*($Delete_[S/x]$)*/#define _YMS_GENDER_FULLSETITEM //! 2011.5.24 / i4u4me / ヴ撢 蝶觼董お 熱薑 
    /*($Delete_[S/x]$)*/#endif

    #define _SCJ_THREE_LEVELUP_PER_EFFECT   // 2011.04.01 / 歎雙謙 / 3溯漣 葆棻 溯漣機 檜めお 蹺陛/滲唳
    #define _SCJ_ADD_TYPE_LAYER_FOR_ITEM    // 2011.04.11 / 歎雙謙 / 嬴檜蠱 顫殮縑 評艇 溯檜橫 蹺陛
    #define _SCJ_TEXT_COLOR_RENEWAL         // 2011.03.07 / 歎雙謙 / 瓣た 臢蝶お 旋噢縑 纔舒葬 樓撩 輿晦
    #define _YMS_AUTOBOT_ITEM_KIND          // 2011.3.23 / i4u4me / 濠翕 餌傖 嬴檜蠱 謙盟滌煎 蹺陛 

    #define _NA_20100322_AGGRO_DISPLAY                                    // 2011.03.24 橫斜煎 熱纂 �飛橦� ル衛 (僥鼻⑷, 檜紫⑽)
    /*($Delete_[x/S]$)*/#define _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT // 2011.04.21 / 檜擎だ, 檜紫⑽ / 濰綠 慇晦 偃摹濛機
    #define _GS_GSP_ALLOW_MOVE_TARGETING_								// 磁嘐 雖羲雖薑蝶鑒 檜翕.薄Щ衛縑 嶸雖

    #define _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE                        // 2011.03.23 / 薑⑷撩,歎雙謙 / 嬴檜蠱 �僱� 寞衝 滲唳
    
    #define _YMS_WINDOW_MAXSCREEN   //! 2011.4.7 / i4u4me / 璽賅萄 譆渠�� 晦棟 蹺陛
    #define _NA_002455_20110328_AUTH_HEART_BEAT // 2011.03.28 / 梯⑷錠 / 檣隸憮幗 Heartbeat 晦棟 蹺陛
    #define _NA002635_GP_DP_PROTOCOL_UINIFICATION // 檜擎だ / Portal 憮幗 薯剪 塽 DG Щ煎饜屬婁 鱔м 濛機
    #define __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT // 2011.05.09 / 檜擎だ / (醞措) ぅ饜葬橡 路葡 ⑷鼻 諫�� 雖錳 濛機

    #define _DH_QUEST_ACCEPT_MESSAGE // 2011.05.19 / 檜紫⑽ / 蠡蝶お 熱塊衛 鼻欽縑 詭衛雖 轎溘 ж朝 濛機

    #define _SCJ_SSQ_DIRECT_ENTER  // 2011.05.20 / 歎雙謙 / 顫塊и餌錳 殮濰掏 餌辨 衛 夥煎 ん驍 翮晦
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED // 2011.05.23 / 歎雙謙 / OptionList.txt曖 RatioValue в萄蒂 Suffix煎 滲唳 濛機 (蕾嘐餌煎 餌辨й 蝶お葭囀萄蒂 嫡擠)

    #define _SCJ_CHANGE_SKILL_COOLTIME_COLOR // 2011.06.07 / 歎雙謙 / 蝶鑒 籐顫歜 儀鼻 滲唳

    #define _NA_0_20110329_GAMEOPTION_SETTING                       // 2011.03.29 / 陴晦瑰, 嶸貲熱/ 啪歜 褫暮曖 DB 盪濰

    #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
        #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 歎雙謙 / ToTalGuide陛 蹺陛腎賊憮 晦襄縑 餌辨ж湍 酈 紫遺蜓 璽擊 餌辨寰л 
    #endif
    #define _NA_002950_20110627_SSQ_NOTICE // 2011.06.27 / 梯⑷錠, 歎雙謙 / 顫塊и 餌錳 辦蝓 詭衛雖 轎溘 寞衝 滲唳

    #define _SCJ_ADD_SNS_SHORTCUT                                     // 2011.06.22 / 歎雙謙 / SNS 夥煎陛晦 幗が 蹺陛
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO         // 2011.06.22 / 歎雙謙 / OptionList.txt曖 ValueType2 в萄陛 蹺陛腎堅 高檜 1檣 褫暮擎 擠熱陛 檜煎遴 褫暮
    #define _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE // 2011.06.30 / 梯⑷錠 / ん驍 檜翕衛 模�笆� 餌塭雖雖 彊紫煙 滲唳

    #define _YMS_CHANNEL_LIST_COUNT5    //! 2011.7.12 / i4u4me / 瓣割葬蝶お 偎熱 4 -> 5偃

    #if defined(_KOREA)
        #define _NA_20100307_BETAKEY_SYSTEM                                   //漆顫酈 衛蝶蠱 (僥鼻⑷, 檜紫⑽)
        /*($Delete_[C]$)*/#define REMOVE_SUMMON_FRIEND    // 2011.05.24 / 檜紫⑽ / 耀掘模�� 幗が 薯剪
        #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // 嶸貲熱, 檜擎だ / 奢問 蝶顫橾 翱轎 偃摹

    #elif defined(_CHINA)
        #define _YJW_CREATECHARACTER_MOVIE //! 2011.8.3 / i4u4me / 議葛攪 儅撩�� 翕艙鼻 爾晦
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / ぅ饜葬橡 蝶霾 

        #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // 檜擎だ, 嶸貲熱 / 嘐暮 ん驍 翱轎 鬼�� (顫檜該 塽 ん驍 翱唸 �� 濠霞 驍黴 晦棟 雖錳)
        #ifndef _NA_0_20110803_DIV_GAMELOG_FILE
            #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 陴晦瑰, 啪歜煎斜 辨榆縑 評艇 だ橾碟葬.
        #endif
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
        #define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / 嶸貲熱, 陴晦瑰, 蕾樓 贗塭檜樹お曖 MAC 輿模 煎斜 陴晦晦.
        #define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / 陴晦瑰, だじ 檣錳熱縑 曖и ж歜 萄奧徽 褻薑.
        #define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / 陴晦瑰,梯褐鷓 / 頂掘紫 0檣 鼠晦紫 寞橫掘諦 偽擎 寞衝戲煎 だ惚腎紫煙 晦棟 熱薑.

    #elif defined(_JAPAN)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_003481_20111026_EXP_PARTYBONUS_CHANGE // だじ 檣錳熱縑 評艇 唳я纂 隸陛徽 滲唳 - arycoat 2011.10.26
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
    #elif defined(_GLOBAL)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 梯⑷錠, 檜紫⑽ / 薑爾 翮塋褒 鱔м 餌辨濠 陛檜萄 衛蝶蠱
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 歎雙謙 / ToTalGuide陛 蹺陛腎賊憮 晦襄縑 餌辨ж湍 酈 紫遺蜓 璽擊 餌辨寰л 
        #endif
        /*($Delete_[S/C]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND // 2009.10.21 / 僥鼻⑷, 檜紫⑽ / 耀掘 蟾渠 衛蝶蠱
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
    #elif defined(_RUSSIA)
        #define _DH_KEY_BINDING // 2011.11.29. 檜紫⑽, 檜鷓遽 / 嶸盪 欽蹴酈 撮た 雖錳
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 梯⑷錠, 檜紫⑽ / 薑爾 翮塋褒 鱔м 餌辨濠 陛檜萄 衛蝶蠱
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _YMS_TREECONTROL // 2011.07.12. 嶸營錳 / お葬鐘お煤
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 歎雙謙 / ToTalGuide陛 蹺陛腎賊憮 晦襄縑 餌辨ж湍 酈 紫遺蜓 璽擊 餌辨寰л 
        #endif
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.1.0.1 Episode2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1101
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _GS_ADD_VOLUME_LIGHT                                        // 2010.09.28 / 梯惜熱 / ん蝶お 檜めお 褫暮 蹺陛
    /*($Delete_[S/C]$)*/#define _DH_EP2_FULL_MAP                                            // 2010.12.13 / 檜紫⑽ / 褐渠睛 雖紫 + 褐寡ぎ襄
    /*($Delete_[S/C]$)*/#define _DH_IMAGE_TOOLTIP                                           // 2010.11.03 / 檜紫⑽ / 檜嘐雖 罐そ
    /*($Delete_[S/C]$)*/#define _NA_000251_20100727_SOCKET_SYSTEM                           // 2010.07.27 / 梯⑷錠, 檜紫⑽ / 模鰍 衛蝶蠱
    /*($Delete_[S/C]$)*/#define _NA_002253_20100811_CUBE_SYSTEM                             // 2010.08.11 / 梯⑷錠, 檜紫⑽ / 聽粽 衛蝶蠱
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_PERK_SYSTEM_                           // 2010.10.19 / 檜擎だ, 檜鷓遽 / へ 衛蝶蠱
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_SKILL_SYSTEM_DB_                       // 2010.10.19 / 檜擎だ / 蝶鑒 衛蝶蠱 DB だお
    /*($Delete_[x/C]$)*/#define _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_                  // 檜擎だ / 蝶鑒 衛蝶蠱 幗瞪 婦葬 塽 へ 衛蝶蠱 奩艙
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_SKILLTREE_SYSTEM                             // 2010.09.01 / 陴晦瑰, 梯惜熱/ 蝶鑒お葬 衛蝶蠱 蹺陛
    /*($Delete_[S/C]$)*/#define _GS_SKLL_TREE_SYSTEM                                        // 2010.09.01 / 梯惜熱 / 蝶鑒お葬衛蝶蠱
    /*($Delete_[S/C]$)*/#define _NA_0_20100819_AGGRO_RENEWAL                                // 2010.08.19 / 僥鼻⑷ 檜紫⑽ / 橫斜煎 衛蝶蠱 葬景橡    
    /*($Delete_[S/C]$)*/#define _NA_000816_20101025_AIRSHIP_SYSTEM                          // 2010.10.25 / 梯惜熱, 梯⑷錠 / 綠奢薑 衛蝶蠱
    /*($Delete_[S/C]$)*/#define C_NA_0_20100520_ACSYSTEM_CHANGED                            // 2010.05.20 / 僥鼻⑷, 檜鷓遽 / AC 衛蝶蠱擊 機瞳欽嬪煎 滲唳и棻.
    /*($Delete_[x/C]$)*/#define _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT             // 2010.11.23 / 鬼營遽 / PortalServer 楨韁 唸骯 衛除 滲唳
    /*($Delete_[x/C]$)*/#define _NA_1388_20101207_EP2_INCREASED_MISSION_MAX                 // 2010.12.07 / 鬼營遽 / 嘐暮 譆渠 偃熱 鼻щ
    /*($Delete_[S/C]$)*/#define _TEMP_DEFINE_REMOVE_CHARACTER_SCENE_MONSTER                 // 2010.12.13 / 檜鷓遽 / 議葛攪儅撩衛 跨蝶攪儅撩 歜衛 綰楣(*歜衛蛤だ檣)
    /*($Delete_[S/C]$)*/#define  _LTJ_CENTIPEDE_MONSTER_BURROW_ANMATION                     // 2101.01.06 / 檜鷓遽 / 雖啻跨蝶攪 幗煎辦擁棲衙檜暮 籀葬
    /*($Delete_[S/C]$)*/#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                  // 2010.12.14 / 團薑艙, 檜鷓遽 / 薄滄瞪 婦葬 寞衝 偃摹縑 評艇 唳衙 嬴檜蠱 羹觼 寞衝 滲唳
    /*($Delete_[S/C]$)*/#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
    /*($Delete_[x/C]$)*/#define _NA000000_GM_COMMAND_CHANGE_CONCEPT_                        // 檜擎だ / GM 貲滄橫 碟葬 濛機 塽 籀葬 寞徹 掖羹
    /*($Delete_[S/C]$)*/#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL                     // 2011.01.31 / 陴晦瑰,梯褐鷓 / 蝶鑒お葬 衛蝶蠱曖 蝶鑒曖 譆蟾 蝗菜 寞衝 滲唳.(蝶鑒 晦獄 溯漣 0蝶鑒 -> 1蝶鑒).
    /*($Delete_[S/C]$)*/#define _NA_0_20110215_SKILLTREE_SOUND_RENEWAL                      // 2011.02.15 / 梯褐鷓 / 蝶鑒お葬 翱轎 �膩�擠 瞳辨
    /*($Delete_[S/C]$)*/#define _DH_TARGET_INFO_RENEWAL                                     // 2011.02.16 / 檜紫⑽ / 跨蝶攪 薑爾爾晦 葬景橡EP2 濛機
    /*($Delete_[S/C]$)*/#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                            // 2011.03.03 / 團薑艙, 檜紫⑽ / 顫殮滌 NPC 鼻薄 晦棟 蹺陛

    /*($Delete_[S/C]$)*/#define _NA_0_20110216_STATUS_STAT_POINT_SIMULATION                 // 2011.02.16 / 陴晦瑰,嶸貲熱 / 蝶邐 ん檣お 衛嘆溯檜暮 蹺陛.
    /*($Delete_[S/C]$)*/#if defined(_NA_0_20110216_STATUS_STAT_POINT_SIMULATION)
        /*($Delete_[S/C]$)*/#define _YMS_STAT_SIMULATION_   //! 2011.2.16 / i4u4me / 蝶邐 衛嘆溯檜暮 
    /*($Delete_[S/C]$)*/#endif

    //
    /*($Delete_[S/C]$)*/#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_PARSER // 2010.07.27 / 梯⑷錠 / 模鰍 衛蝶蠱(だ憮)
        /*($Delete_[S/C]$)*/#define _NA002237_100802_EP2_SOCKET_SYSTEM_DB_ // 檜擎だ / 模鰍 衛蝶蠱 DB だお
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_CONVERT // 2010.07.27 / 梯⑷錠 / 模鰍 衛蝶蠱(solarSlot)
        /*($Delete_[S/C]$)*/#define _NA_001699_20110104_SOCKET_OPTION_POINT // 2011.01.04 / 梯⑷錠, 檜紫⑽ / 模鰍 褫暮 ん檣お 寞衝戲煎 滲唳        
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_RANKUP // 2010.10.13 / 檜紫⑽ / 模鰍 衛蝶蠱(楨觼機 薯剪) 贗塭瞪辨
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_OLD_SOCKET // 2010.10.16 / 檜紫⑽ / 模鰍 衛蝶蠱(晦襄 模鰍 薯剪) 贗塭瞪辨
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_SOCKET_COMPOSITION_ANIMATION// 2011.02.23 / 檜紫⑽ / 模鰍 擁棲詭檜暮 除模��
    /*($Delete_[S/C]$)*/#endif //
    /*($Delete_[S/C]$)*/#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (2)
    /*($Delete_[S/C]$)*/#else
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (1)
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA001605_EP2_ATTRIBUTES_RENEWAL_ // 檜擎だ, 嶸貲熱 / 樓撩 衛蝶蠱 葬景橡
    /*($Delete_[S/C]$)*/#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        /*($Delete_[S/C]$)*/#define _YMS_EP2_ATTRIBUTES_RENEWAL_     //! 2011.1.7 / i4u4me / 議葛攪 樓撩 葬景橡(ui 璽)
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[S/C]$)*/#define _YMS_EP2_SKILL_PACKET_RENEWAL_ //! 2011.1.10 / i4u4me / 蝶鑒 ぬ韃 熱薑
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_DAMAGETYPE_EXTENTION_ // 檜擎だ, 嶸貲熱 / DAMAGETYPE �挫�
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ // 檜擎だ, 嶸貲熱 / skill broadcaster 檜漸お 寞衝戲煎 滲唳

    /*($Delete_[S/C]$)*/#define _NA_0_20101130_REWARD_ITEM_COUNT_INCREASE                   // 2010.11.30 / 陴晦瑰, 歎雙謙 / 爾鼻 蝸煜 隸陛. 摹鷗 4偃 -> 5偃
    /*($Delete_[S/C]$)*/#define _NA_0_20101206_PARTY_ITEM_DISTRIBUTE_CHANGE                 // 2010.12.06 / 陴晦瑰, 檜紫⑽ / 寡ぎ憮幗縑憮 だじ 嬴檜蠱 碟寡寞衝 滲唳(в萄諦 翕橾ж啪)

    /*($Delete_[S/C]$)*/#define _NA_0_20110103_EP2_BATTLEZONE_EXP_PARTY_BONUS_ADJUSTED      // 2011.01.03 / 鬼營遽 / 寡ぎ襄(寞) 唳я纂 だじ 爾傘蝶 綠徽 褻薑 (醞措 薯諼)
    /*($Delete_[S/C]$)*/#define _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING            // 2011.01.14 / 鬼營遽, 檜鷓遽 / ж歜, 嬴檜蠱 爾鼻擊 んлй 熱 氈朝 衛蝶蠱 詭賅 熱滄/餉薯 煎斜 蹺陛
    /*($Delete_[S/C]$)*/#define _NA_20100802_SPEED_HACK_DETECTION                           // 2010.08.02 / 鬼營遽, 嶸貲熱 / 偃摹脹 In-Game 蝶Я萄р 鬚雖

    /*($Delete_[S/C]$)*/#define _YMS_DIALOG_ANIMATION_                                      // 2010.12.13 / i4u4me / 棻檜橡煎斜 遺霜檜啪  DialogPosInfo.txt  諦 偽檜 寡ん腎橫撿 л
    /*($Delete_[S/C]$)*/#define _YMS_TEXTURE_ANIMATION_                                     // 2010.12.27 / i4u4me / 臢蝶藥 擁棲詭檜暮  TextureListInfo.txt, TextureAnimationInfo.txt 偽檜 寡ん腎橫撿 л
    /*($Delete_[S/C]$)*/#if defined(_YMS_DIALOG_ANIMATION_) && defined(_YMS_TEXTURE_ANIMATION_)
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_RENEWAL_                                     //! 2010.12.13 / i4u4me / 煎斜檣 �飛� 葬景橡 
        /*($Delete_[S/C]$)*/#define _YMS_SCREEN_RATIO_                                      //! 2011.1.24 / i4u4me / 16:10 綠徽 蹺陛  (WZENGINEVER >= 280) 檜鼻
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_CHARACTER_CAMERA_RENEWAL_                    //! 2011.1.25 / i4u4me / 議葛攪 蘋詭塭 撲薑 滲唳 
        /*($Delete_[S/C]$)*/#define _YMS_LOADING_IMAGE_                                     //! 2011.1.27 / i4u4me / 煎註 檜嘐雖 ж釭煎 熱薑 
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA_0_20110208_SKILLTREE_VISUAL_RENEWAL                     // 2011.02.11 / 梯褐鷓 / 蝶鑒衛蝶蠱 翱轎 葬景橡, 晦獄溯漣 0戲煎 熱薑

    /*($Delete_[S/C]$)*/#define _DH_DIVIDE_MESSAGE_TEXT                                     // 2011.12.22 / 檜紫⑽ / messagetext.txt 碟葬濛機

    /*($Delete_[S/C]$)*/#define _YMS_SMOOTH_GAUGE_                                          // 2011.2.14 / i4u4me / hp �蛹� 啪檜雖 遺霜歜 睡萄毀啪

    /*($Delete_[S/C]$)*/#define _YMS_DEATH_SYNC_                                            //! 2011.2.21 / i4u4me / 奢問 唸婁 翕晦��
    /*($Delete_[S/C]$)*/#define _DH_FATE_REMOVE_MAGIC_ARMOR                                 // 2011.02.23 / 檜紫⑽ / 楣酈嬴檜蠱 檣隸 UI縑憮 葆徹 寞橫溘 鐘お煤 塽 �膩� 薯剪

    /*($Delete_[S/C]$)*/#define _NA_002229_20110302_PARTY_DEFUALT_LOOTING_CHANGE            // 2011.03.02 / 鬼營遽 / だじ 蛤ィお 嬴檜蠱 碟寡 寞衝 滲唳 (濠嶸 -> 離滔渠煎)
    /*($Delete_[S/C]$)*/#define _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT           // 2011.03.10 / 鬼營遽 / Я觼顫歜 檜漸お 錯萄滌 掘碟 瞳辨 晦棟 蹺陛

    /*($Delete_[S/C]$)*/#define _YMS_SKILL_TOOLTIP_                                         //! 2011.2.28 / i4u4me / 蝶鑒罐そ 熱薑: 撲貲縑蹺陛 褫暮 厥晦
    /*($Delete_[S/C]$)*/#define _NA_002128_20110228_MODIFY_ITEM_LEVEL                       // 2011.02.28 / 梯⑷錠, 檜紫⑽ / 嬴檜蠱 溯漣高 撲薑 滲唳
    /*($Delete_[S/C]$)*/#define _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE          // 2011.03.23 / 薑⑷撩,歎雙謙 / ж歜 碟寡 寞衝 滲唳

    /*($Delete_[S/C]$)*/#define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 陴晦瑰, 嶸貲熱/ 嬴檜蠱 憧 歜衛 葬ね饜葭(蘋纔堅葬)/
    /*($Delete_[x/C]$)*/#define _DH_GM_ITEM_WINDOW  // 2011.03.03 / 檜紫⑽ / GM Item Window (だ憮薹 蛤だ檣)
    
    //#if !defined(_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED)
    //  #define _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED  // 2010.03.28 / 檜鷓遽 / OptionList 蝶觼董お縑 RatioValue в萄 蹺陛 濛機(褫暮高檜 薑熱顫殮縑紫 曖嘐縑 評塭 %蒂 稱罹 轎溘й 熱 氈紫煙)
    //#endif

    /*($Delete_[S/C]$)*/#define _DH_SHOW_ITEM_CREATE_RATE // 2011.03.28 / 檜紫⑽ / 嬴檜蠱 儅撩(褻м) �捕� ル衛 晦棟
    /*($Delete_[S/C]$)*/#define _NA_002474_20110330_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2011.03.30 / 梯⑷錠 / 琿蝶驛 錚檜ん檣お 餉薯 渠晦 衛除 熱薑

    /*($Delete_[S/C]$)*/#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                       // 2011.03.18 / 陴晦瑰, / 蝗菜 嬴檜蠱 諫猿 晦除 撲薑 蹺陛.
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION            // 2011.04.01 / 檜擎だ / в萄,寡ぎ襄 嬴檜蠱 萄奧 離滌�� 婦溼 濛機
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES    // 2011.04.01 / 檜擎だ / 萄奧 Group(4) 賅萄 蹺陛 (醞繪 嬴檜蠱擊 醞繪衛酈雖 彊堅 偃熱煎 萄奧)

    /*($Delete_[S/C]$)*/#define _NA_002471_20110408_EXP_BENEFIT_UP                          // 2011.04.08 / 鬼營遽 / 唳я纂 爾傘蝶 譆渠高 薯и 鼻щ (譆渠 �僱瘛挫� 錳 唳я纂曖 500%縑憮 1000%煎, 瞪 措陛 奢鱔)

    /*($Delete_[S/C]$)*/#define _K2P_SHOUT_DEFAULT // 2008.07.25 / 梯唳ル / 諼纂晦 蛤ィお煎 瞳辨 define 檜葷 滲唳

    #if defined(_KOREA)
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                 // 2010.10.11 / 陴晦瑰, / 瞳董 ん檣お 雖晝衛 溯漣/譆渠偎熱 蒂 薯иж朝 褫暮 蹺陛.

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / 檜擎だ / Ep2<-Ep1 滲�紗� 濰綠 嬴檜蠱 婦溼 爾鼻 籀葬 (橾獄 幗瞪)

    #elif defined(_GLOBAL)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER // 2010.06.04 / 團薑艙, 檜鷓遽 / 雖羲 薄滄 婦葬 寞衝 偃摹
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION // 2010.10.11 / 陴晦瑰, / 瞳董 ん檣お 雖晝衛 溯漣/譆渠偎熱 蒂 薯иж朝 褫暮 蹺陛.
        //#define _DH_GLOBAL_VERSION_TEST // 2011.04.21 / 檜紫⑽ / 旋煎弊 憮幗葬蝶お 纔撥/橾奩 碟葬辨 囀萄
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / 檜擎だ / Ep2<-Ep1 滲�紗� 濰綠 嬴檜蠱 婦溼 爾鼻 籀葬 (橾獄 幗瞪)
    #elif defined(_RUSSIA)
        #define _RU_20110801_SEND_GIFT_FRIEND_CHECK //僥鼻⑷/ 耀掘縑啪 摹僭擊 爾鳥陽 頂陛 鼻渠寞曖 耀掘罹撿 ж堅 鼻渠寞紫 釭曖 耀掘罹撿 и棻.
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.0.0.6 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1006
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG //! 2010.10.19 / i4u4me / 剪楚醞橾唳辦縑朝 衛蝶蠱 詭景 翮雖 彊朝棻 
    /*($Delete_[S/C]$)*/#define _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM // 檜擎だ, 梯惜熱 / 楝酈 嬴檜蠱紫 碟葬 陛棟饜煙(縑纔葬嬴虜...)
    /*($Delete_[x/C]$)*/#define _DH_QUICK_SLOT_LINK // 2010.11.12 / 檜紫⑽ / 襬蝸煜縑 蛔煙脹 濰綠 嬴檜蠱 婦溼 煎霜 滲唳(檣漸,檜籠橾陽 update, 斜諼曖 唳辦 unlink)
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_COOLTIME_DRAW // 2010.11.22 / 檜紫⑽ / 籐顫歜 �膩� 鱔橾
    /*($Delete_[S/C]$)*/#define _NA_0_20110210_INVITE_PARTY_BUG                             // 2011.02.10 / 陴晦瑰 / 2貲檜憮 だじ 裕朝 紫醞縑 薯3濠縑啪 だじ褐羶擊 й唳辦 棻艇 餌塋婁 だじ陛 裕橫雖朝 幗斜 熱薑.

    #if defined(_KOREA)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 陴晦瑰, 嶸貲熱/ 嬴檜蠱 憧 歜衛 葬ね饜葭(蘋纔堅葬)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 陴晦瑰, 歎雙謙 / 偃檣鼻薄 掘衙/っ衙 蝸煜熱 隸陛(晦襄 24偃->40偃)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 團薑艙 / 晦除 �側瘓� 嬴檜蠱 剪楚 碳棟

    #elif defined(_CHINA)
        #define _HACKSHIELD_CHECK_OPTION_LMP // р褒萄 lmp 瞳辨
		#define _SCJ_PK_POPUP_DIALOG // pk 詭撮雖
        #define _NA_0_20110305_BUG_READ_BIG_LOGFILE                         // 2011.03.05 / 陴晦瑰 / 2GB檜鼻曖 煎斜 檗晦 褒ぬ 幗斜.(だ橾 с菟 檗晦 л熱煎 滲唳)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 陴晦瑰, 歎雙謙 / 偃檣鼻薄 掘衙/っ衙 蝸煜熱 隸陛(晦襄 24偃->40偃)
        #ifndef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
            #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                      // 2011.03.08 / 陴晦瑰, 嶸貲熱 / 議葛攪 綠蕾樓衛 蝶鑒 籐顫歜 嶸雖 晦棟 蹺陛.
        #endif

    #elif defined(_JAPAN)
        #define _JP_0_20101123_PMS                                         // 橾獄 啪歜螞曖 憮幗該 婦葬 衛蝶蠱檣 PMS 瞳辨
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 陴晦瑰, 歎雙謙 / 偃檣鼻薄 掘衙/っ衙 蝸煜熱 隸陛(晦襄 24偃->40偃)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / 團薑艙, 檜鷓遽 / 薄滄瞪 婦葬 寞衝 偃摹縑 評艇 唳衙 嬴檜蠱 羹觼 寞衝 滲唳
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / 團薑艙, 檜鷓遽 / 雖羲 薄滄 婦葬 寞衝 偃摹
        #define _JP_20110222_INGAME_CHARNAME_CHANGE                        // 2010.2.22  / 僥鼻⑷, 檜鷓遽 / 橾獄縑 憮幗 鱔м擊 嬪п 議葛鑣貲 滲唳擊 啪歜頂縑憮 雖錳п遽棻 (壎戲煎 議葛貲 滲唳檜 寰脹棻.)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 團薑艙 / 晦除 �側瘓� 嬴檜蠱 剪楚 碳棟    

    #elif defined(_GLOBAL)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 陴晦瑰, 嶸貲熱/ 嬴檜蠱 憧 歜衛 葬ね饜葭(蘋纔堅葬)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 陴晦瑰, 歎雙謙 / 偃檣鼻薄 掘衙/っ衙 蝸煜熱 隸陛(晦襄 24偃->40偃)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 團薑艙 / 晦除 �側瘓� 嬴檜蠱 剪楚 碳棟        
    #elif defined(_RUSSIA)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 陴晦瑰, 歎雙謙 / 偃檣鼻薄 掘衙/っ衙 蝸煜熱 隸陛(晦襄 24偃->40偃)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / 團薑艙, 檜鷓遽 / 薄滄瞪 婦葬 寞衝 偃摹縑 評艇 唳衙 嬴檜蠱 羹觼 寞衝 滲唳
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / 團薑艙, 檜鷓遽 / 雖羲 薄滄 婦葬 寞衝 偃摹

    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.0.0.5 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1005
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)
    /*($Delete_[S/C]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 婦溼 啪歜 煎斜 蹺陛
    /*($Delete_[x/C]$)*/#define _101011_LTJ_DOMINATION_GUILDMEMBER_EFFECT                   // 2010.10.13 / 檜鷓遽 / 薄滄瞪 望萄錳爾鼻 檜めお
    /*($Delete_[S/C]$)*/#define _NA_0_20100722_AUTO_SYSTEM                                  // 2010.07.22 / 陴晦瑰 / 螃饜 衛蝶蠱
    #define _YMS_AUTOBOT_GAME                                           // 2010.07.01 / 嶸貲熱 / 螃饜罐擎 縛霞幗瞪 211 睡攪 雖錳и棻.
    /*($Delete_[S/C]$)*/#define _NA_0_20100817_HELLON_LAST_TRIGGER                          // 2010.08.17 / 梯⑷錠, 檜鷓遽 / ⑦煩 詭檣 嘐暮辨 縑棲詭檜暮 薯橫 お葬剪.
    /*($Delete_[x/C]$)*/#define _GS_ADD_CAMERA_VIEW_POS                                     // 2010.10.28 / 梯惜熱 / 議葛攪 蘋詭塭 衛薄褻瞰 褫暮
    /*($Delete_[x/C]$)*/#define _DH_DISPLAY_FOR_MISSION_OBJECT                              // 2010.08.02 / 檜紫⑽ / 嘐暮 熱ч 跡瞳 葬蝶お ル衛 濛機
    /*($Delete_[S/C]$)*/#define _NA_000587_20100928_DOMINATION_BUFF_ITEM                    // 2010.09.28 / 梯⑷錠, 嶸貲熱 / Я曖 撩瞪 鬼�� 幗Щ 嬴檜蠱
    /*($Delete_[S/C]$)*/#define _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL    // 2010.10.01 / 梯⑷錠, 檜紫⑽ / 瓣割 蟾渠 晦棟 鬼��
    /*($Delete_[S/C]$)*/#define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_        // 2010.10.11 / 檜擎だ / 醞措 檜蓬, 歜曖曖 etherealizable item縑 渠и 蹺陛 寞橫 籀葬 (with script control)

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
      #endif

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.0.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1004
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_ADD_RADAR_ICON_100622            // 2010.06.17 / 嶸貲熱 / 嘐棲裘 ル衛 機等檜お 
    /*($Delete_[S/C]$)*/#define _YMS_ESCKEY_CLOSE_DIALOG                                // 2010.06.14 / 嶸貲熱 / 橾睡 UI陛 ESC 酈煎 殘�鷙� 彊朝 ⑷鼻
    /*($Delete_[S/C]$)*/#define _YMS_SYSTEMINPUTBOX_ONLYNUMBER                          // 2010.05.28 / 嶸貲熱 / 衛蝶蠱 檣Е 璽縑憮 璋濠蒂 跤堅纂朝 ⑷鼻 熱薑(夔お煤縑 璋濠虜 殮溘 嫡擊熱 氈朝 晦棟 蹺陛)
    /*($Delete_[S/C]$)*/#define _YMS_PET_UI                                             // 2010.05.18 / 嶸貲熱 / ゆ UI 滲褐. 
    /*($Delete_[S/C]$)*/#define _YMS_GUILD_DETAL_SPEC                                   // 2010.05.11 / 嶸貲熱 / 望萄錳 鼻撮 薑爾 轎溘 
    /*($Delete_[S/C]$)*/#define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                  // 2010.05.11 / 嶸貲熱 / ん驍嬪纂縑憮 嘐暮諫猿�� 蠡蝶お蒂 諫猿 跤ж朝 ⑷鼻
    /*($Delete_[S/C]$)*/#define _100708_LTJ_SHOW_DATETIME_IN_TIMER                      // 2010.07.26 / 檜鷓遽 / 顫檜該(Я煎紫)縑 陳瞼紫 ル衛
    /*($Delete_[S/C]$)*/#define _100705_LTJ_CONTROL_LOD_DISTANCE_PLAYER_COUNT           // 2010.07.26 / 檜鷓遽 / Ы溯檜橫 蘋遴お縑 評塭 LOD 蘋詭塭 剪葬 褻瞰
    /*($Delete_[S/C]$)*/#define _100416_LTJ_MODIFY_CHAT_FILTERING                       // 2010.07.26 / 檜鷓遽 / 瓣た 旎罌橫 в攪葭 滲唳
    /*($Delete_[S/C]$)*/#define _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER                // 2010.07.26 / 檜鷓遽 / 鼻鷓幗Щ 嬴檜夔 臢蝶藥鱔м 濛機
    /*($Delete_[S/C]$)*/#define _GS_ADD_PET_ENCHANT_EFFECT                              // 2010.07.26 / 梯惜熱 / ゆ檣臘お�膩�
    /*($Delete_[S/C]$)*/#define _YJ_FADE_FAR_TERRAIN                                    // 2010.07.26 / 彌艙輿 / 錳唳 偃摹 濛機
    /*($Delete_[S/C]$)*/#define _YJ_SHOW_TERRAIN_DRAWCALLS                              // 2010.07.26 / 彌艙輿 / 偃嫦 詭撮雖煎 雖⑽ 萄煎辦屬 熱 轎溘
    /*($Delete_[S/C]$)*/#define _NA_0_20100705_DOMINATION_EVENT                         // 2010.07.05 / 檜�ˋ�, 彌艙輿 / 薄滄瞪 檜漸お お葬剪(跨蝶攪葬薇).
    /*($Delete_[S/C]$)*/#define _NA_0_20100706_RANDOM_REGEN                             // 2010.07.06 / 檜�ˋ� / 楠渾 葬薇 營掘⑷.
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG               // 2010.07.06 / 檜�ˋ� / 望萄貲 滲唳 嬴檜蠱 餌辨 煎斜 蹺陛.

    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ // 檜擎だ, 檜紫⑽ / 檣漸饜葬 蝸煜 晦棟 偃摹
    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_ // 檜擎だ / 檣漸饜葬 碟葬 濛機
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_ // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_ // 2010.10.13 / 檜擎だ, 檜鷓遽 / 褻м營猿煎 餌辨ж朝 縑纔葬嬴 婦溼 嬴檜蠱 籀葬
    /*($Delete_[S/C]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_ // 檜擎だ, 嶸貲熱 / C/S 衛除 翕晦�� 籀葬
    /*($Delete_[S/C]$)*/#define _NA000000_100623_CHECK_WMO_CHECKSUM_ // 檜擎だ, 彌艙輿 / в萄 殮濰衛 WMO 幗瞪 匐餌 瑞ず 蹺陛 (engine幗瞪 匐餌塭朝 檜溯敔楝 嫦儅...壬壬)
    /*($Delete_[S/C]$)*/#define _NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_ // 檜擎だ / 褐敘 議葛縑 雖晝ж朝 濰綠 嬴檜蠱 �挫� 雖錳

    /*($Delete_[S/C]$)*/#define _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM // 2010.05.27 / 梯⑷錠, 梯惜熱 / 議葛攪 諼賅 滲唳 衛蝶蠱
    /*($Delete_[S/C]$)*/#define _NA_000070_20100609_ENHANCED_ITEM_DIVISION // 2010.06.09 / 梯⑷錠, 檜紫⑽ / 嬴檜蠱 碟й 寞衝 偃摹
    /*($Delete_[S/C]$)*/#define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST // 2010.06.22 / 梯⑷錠, 檜紫⑽ / 離欽 跡煙 偃摹

    /*($Delete_[S/C]$)*/#define _NA_0_20100325_QUICK_SLOT_CHANGE						// 2010.03.25 / 陴晦瑰,檜紫⑽ / Quick 蝸煜 滲唳 (48偃曖 蝸煜->40偃曖 蝸煜戲煎 滲唳)
    /*($Delete_[S/C]$)*/#define _DH_SHOW_REPAIR_HAIM                                    // 2010.07.23 / 檜紫⑽ / 熱葬 孺紫辦 ж歜 3濠葬 掘碟薄(,) 蹺陛
    /*($Delete_[S/C]$)*/#define _DH_SHOW_UI_NAME                                        // 2010.07.13 / 檜紫⑽ / UI 檜葷 ル衛晦棟
    /*($Delete_[S/C]$)*/#define _DH_NORMAL_CHAT_BLOCK                                   // 2010.06.22 / 檜紫⑽ / 離欽 跡煙 偃摹
    /*($Delete_[S/C]$)*/#define _DH_ENABLE_DIVIDING_ITEM_WITH_CLICKING_SLOT_TAB         // 2010.06.04 / 檜紫⑽ / 嬴檜蠱 碟寡衛縑 蝸煜 滲唳 陛棟ж紫煙 滲唳
    /*($Delete_[S/C]$)*/#define _DH_ENHANCED_UI_PACKAGE_CHATTING                        // 2010.06.17 / 檜紫⑽ / UI葬景橡 塽 瓣た璽
    /*($Delete_[x/C]$)*/#define _DH_ADD_TYPE_MARK_FOR_ITEM         // 2010.05.06 / 檜紫⑽ / 嬴檜夔擊 鱔и 嬴檜蠱 衝滌 濛機
    /*($Delete_[S/C]$)*/#define _DH_TRIGGER_LOG_MESSAGE                                 // 2010.06.26 / 檜紫⑽ / 偃嫦 啪歜 贗塭檜樹お縑憮 お葬剪 詭衛雖 轎溘 濛機
    /*($Delete_[S/C]$)*/#define _DH_DOUBLECLICK_MOVE_WAYPOINT                           // 2010.06.26 / 檜紫⑽ / 錚檜ん檣お 葬蝶お 渦綰贗葛戲煎紫 檜翕 蹂羶 陛棟ж啪 滲唳
    /*($Delete_[S/C]$)*/#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION                // 2010.05.20 / 團薑艙 / 璽堅 檜翕 碳陛棟 褫暮 蹺陛, 望萄 璽堅朝 陛棟
    /*($Delete_[S/C]$)*/#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END      // 2010.06.21 / 團薑艙 / 雖羲 薄滄瞪 檜漸お 蹺陛

    /*($Delete_[S/C]$)*/#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_             // 2010.05.10 / 檜擎だ
    /*($Delete_[S/C]$)*/#define _NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG        // 2010.06.16 / 陴晦瑰 / だじ 熱塊衛 瞳渠婦啗曖 だじ錳檜 襄營л縑紫 陛殮腎朝 幗斜 熱薑.
    /*($Delete_[S/C]$)*/#define _GS_MOD_CASH_SHOP_TOOLTIP_BY_TYPE                       // 2010.09.14 / 梯惜熱 / 議蓮憧縑憮 縑纔葬嬴.蔗嬴檜蠱 罐そ轎溘腎啪莠熱薑

    /*($Delete_[S/C]$)*/#define _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND                 // 2010.09.13 / 鬼營遽 / 望萄 璽堅 蟾晦�� GM 貲滄橫 蹺陛

    /*($Delete_[S/C]$)*/#define _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION             // 2010.11.02 / 檜擎だ / DBQuery羲瞪 ⑷鼻 虞晦 嬪и 濛機

    #if defined(_KOREA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒, 嬴檜蠱 餌辨и 碟葬 晦棟 雖錳(措陛滌 摹鷗 鐘蘸隴)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                          // 2010.06.04 / 團薑艙, 檜鷓遽 / 雖羲 薄滄 婦葬 寞衝 偃摹
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 檜�ˋ�, 嶸貲熱 / 耀掘,望萄錳,葬漸雖 奢嶸 詭賅.

    #elif defined(_CHINA)
        #define _YMS_AUTOBOT_GAME                                               // [2010-7-1 by i4u4me] 螃饜罐擎 縛霞幗瞪 211 睡攪 雖錳и棻.
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒, 嬴檜蠱 餌辨и 碟葬 晦棟 雖錳(措陛滌 摹鷗 鐘蘸隴)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                          // 2009.10.21 / 僥鼻⑷, 檜紫⑽ / 耀掘 蟾渠 衛蝶蠱
        #define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_            // 2010.10.11 / 檜擎だ / 醞措 檜蓬, 歜曖曖 etherealizable item縑 渠и 蹺陛 寞橫 籀葬 (with script control)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / 陴晦瑰, / 瞳董 ん檣お 雖晝衛 溯漣/譆渠偎熱 蒂 薯иж朝 褫暮 蹺陛.
        #define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG                             // 2010.10.19 / i4u4me / 剪楚醞橾唳辦縑朝 衛蝶蠱 詭景 翮雖 彊朝棻 
        #define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                       // 2010.10.26 / 團薑艙 / 斜瑜 爾傘蝶 瞳辨 寞徹 鼻,ж嬪 詹幗 瞳辨 滲唳
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 檜�ˋ�, 嶸貲熱 / 耀掘,望萄錳,葬漸雖 奢嶸 詭賅.

    #elif defined(_JAPAN)
		/*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_ // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒, 嬴檜蠱 餌辨и 碟葬 晦棟 雖錳(措陛滌 摹鷗 鐘蘸隴)
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 檜�ˋ�, 嶸貲熱 / 耀掘,望萄錳,葬漸雖 奢嶸 詭賅.

    #elif defined(_GLOBAL)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒, 嬴檜蠱 餌辨и 碟葬 晦棟 雖錳(措陛滌 摹鷗 鐘蘸隴)

    #elif defined(_RUSSIA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 檜擎だ, 梯惜熱 / 嬴檜蠱 醴蝶攪葆檜癒, 嬴檜蠱 餌辨и 碟葬 晦棟 雖錳(措陛滌 摹鷗 鐘蘸隴)
        #define _RU_110209_RUSSIA_CONTENTS_BLOCK                                // 楝衛嬴 鐘蘸隴 綰楣 蛤だ檣
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / 陴晦瑰, / 瞳董 ん檣お 雖晝衛 溯漣/譆渠偎熱 蒂 薯иж朝 褫暮 蹺陛.
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.0.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1002
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    // unified protocol
    /*($Delete_[S/C]$)*/#define _NA00XXXX_090111_CASH_TYPE_SHARED_                      // 檜擎だ, 檜紫⑽ / 議衛顫殮(PP蘋萄) 蹺陛縑 評艇 掘碟 濛機 (瞪 措陛 奢鱔)
    /*($Delete_[x/C]$)*/#define _NA_0_20100208_DOMINATION_AREA_INFO                     // 2010.02.08 / 檜�ˋ�, 檜鷓遽 / 薄滄瞪 檣濰 輿嬪縑 爾罹輿晦.
    /*($Delete_[S/C]$)*/#define _NA_0_20100525_ADD_TIMER_ACTION_TRIGGER                 // 2010.05.25 / 檜�ˋ�, 檜鷓遽 / 顫檜該 嫦翕 擋暮 お葬剪 蹺陛.
    /*($Delete_[S/C]$)*/#define _NA_0_20100514_NPC_DEAD_STATE_TIME                      // 2010.05.14 / 檜�ˋ� / 跨蝶攪 避擎 鼻鷓 雖樓 衛除 高擊 堅薑高縑憮 蝶觼董お縑憮 薯橫ж紫煙 滲唳. 
    /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / 檜�ˋ� / 蠡蝶お 奩犒 幗斜 曖褕 睡碟 熱薑
    /*($Delete_[S/C]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / 檜�ˋ� / 蠡蝶お 偎熱 羹觼 蹺陛.    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_DURA                      // 2010.04.12 / 檜�ˋ� / 薄滄瞪 頂縑憮 餌蜂衛 頂掘紫 ж塊 綠徽 滲唳 (10% -> 5%) 
    /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGER // 2010.02.22 / 檜�ˋ�, 彌艙輿, 檜鷓遽 / 裘罐 偃摹 蹂羶.
    /*($Delete_[S/C]$)*/#ifdef _NA_0_20100222_UNIT_TRIGGER
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_ADD // 2010.02.22 / 檜�ˋ�, 彌艙輿, 檜鷓遽 / 裘罐 偃摹 蹂羶. 
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_PATHLIST // 2010.02.22 / 檜�ˋ�, 檜鷓遽 / 跨蝶攪 AI 偃摹.  
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_REGEN // 2010.02.22 / 檜�ˋ�, 彌艙輿, 檜鷓遽 / 葬薇 婦溼 滲唳.
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER // 2010.02.22 / 檜�ˋ� / 嬴捱⑽ 跨蝶攪 蹺陛.
        /*($Delete_[S/C]$)*/#define __NA_276_20070213_UNIT_TRIGGER // 2010.02.22 / 梯團遵,檜�ˋ� / 嶸棉 婦溼 嘐掘⑷ お葬剪 蹂羶 / 
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[x/C]$)*/#define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / 檜�ˋ� / PVP衛 幗憮醴 等嘐雖 葬醴幗葬 蝶鑒檜 HP蒂 蟾婁 �蛹� ж朝 ⑷鼻.
    /*($Delete_[x/C]$)*/#define _100114_LTJ_USE_AMBIENTSOUND_SCRIPT_FOR_AMBIENT_SOUND   // 2010.03.25 / 檜鷓遽 / 縞綠樹お餌遴萄 蝶觼董お蒂 鱔и 營儅 薯橫
    /*($Delete_[x/C]$)*/#define _GS_MOD_PLAYER_LOADING                // 2010.03.25 / 梯惜熱 / Player葬煎蝶 雖翱煎註 塽 詩じ噙溯萄 晦棟偃摹
    /*($Delete_[S/C]$)*/#define _NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY      // 2010.03.16 / 梯⑷錠, 嶸貲熱 / 寡ぎ襄 煎綠 殮濰 陛棟и 寞 爾晦
    /*($Delete_[S/C]$)*/#define _NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2          // 鬼營遽, 嶸貲熱 / 鼠猿 蝶邐 蟾晦�� 薯и 溯漣 40縑憮 70戲煎 鼻щ
    /*($Delete_[x/C]$)*/#define _DH_COUNT_LOGIN_FRIEND                                  // 2010.03.23 / 檜紫⑽ / 耀掘 蕾樓濠 熱 ル衛, 斜瑜 んл
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_EXP_COLOR                                    // 2010.03.22 / 檜紫⑽ / 譆堅溯漣 殖撩衛 唳я纂 啪檜雖 儀鼻 滲唳
    /*($Delete_[x/C]$)*/#define _NA_0_20091118_GAMEROOM_BATTLE_RESULT                   // 2010.03.24 / 檜紫⑽, 陴晦瑰 / 瞪癱唸婁爾晦 衛蝶蠱
    /*($Delete_[x/C]$)*/#define _DH_CUR_SKILL_NEEDSFOR_TOOLTIP                          // 2009.11.17 / 檜紫⑽ / 檜嘐 鎰擎 蝶鑒紫 蹂掘 熨溼紫 爾晦
    /*($Delete_[x/C]$)*/#define _DH_HIDE_INCANT_EFFECT                                  // 2009.11.18 / 檜紫⑽ / 蘋詭塭 邀檣 鼻鷓縑憮 檣臘お 檜めお 獗晦晦
    /*($Delete_[x/C]$)*/#define _DH_DBLCLICK_FRIEND_STATUS                              // 2009.11.16 / 檜紫⑽ / 耀掘跡煙 渦綰 贗葛戲煎 鼻撮薑爾 爾晦
    /*($Delete_[x/C]$)*/#define _DH_BATTLEZONE_CREATE_CLASS_TOOLTIP                     // 2009.11.17 / 檜紫⑽ / 寡ぎ襄 儅撩璽縑憮 贗楚蝶 罐そ 爾晦
    /*($Delete_[x/C]$)*/#define _DH_GUILDINFO_OFFLINE_COLOR                             // 2009.11.11 / 檜紫⑽ / 望萄璽縑憮 螃Щ塭檣 嶸盪 �蜓� 儀鼻 瞳辨
    /*($Delete_[x/C]$)*/#define _DH_PARTY_UI_CUSTOMIZING                                // 2009.11.16 / 檜紫⑽ / だじUI 獗晦晦 晦棟
    /*($Delete_[x/C]$)*/#define _DH_VENDER_WINPOS                                       // 2009.11.10 / 檜紫⑽ / 偃檣鼻薄 嬪纂 滲唳 嶸雖
    
    /*($Delete_[x/C]$)*/#define _DH_PARTY_LEADER_CHANGE_CLOSE_OPTWIN                    // 2009.11.12 / 檜紫⑽ / だじ濰 滲唳衛 褫暮璽 殘晦
    /*($Delete_[x/C]$)*/#define _DH_PANDORA_TOOLTIP_ADD_MARGIN                          // 2009.11.25 / 檜紫⑽ / 餌薯曖 摹僭 罐そ 還除問 熱薑
    /*($Delete_[x/C]$)*/#define _DH_MISSIONZONE_NO_ITEMMESSAGE                          // 2009.11.20 / 檜紫⑽ / 嘐暮襄縑憮 嬴檜蠱 碟寡 詭衛雖 嘐轎溘
    /*($Delete_[x/C]$)*/#define _NA_20100106_REPURCHAS_BUGFIX_2496                      //2010.01.06 僥鼻⑷ 寡ぎ襄 菟橫鬲棻 螞�� 營掘衙й陽 營掘衙 寰腎朝 幗斜 熱薑

    /*($Delete_[x/C]$)*/#define _YMS_NUMBER_TO_MONEY                                    // 2010.03.25 / 嶸貲熱 / ж歜 繭欽嬪縑 巍葆 殮溘
    /*($Delete_[x/C]$)*/#define _YMS_ESCKEY_COMPOSITE                                   // 2010.03.25 / 嶸貲熱 / 鬼�倥� esc酈煎 鏃模 陛棟
    /*($Delete_[x/C]$)*/#define _YMS_UI_RBUTTONCLICKI_ENCHANT_CRYSTALLIZATION_RANKUP    // 2010.03.25 / 嶸貲熱 / [и措][0032] 鬼�倥� 嬴檜蠱 蛔煙 よ曖 薯奢
    /*($Delete_[x/C]$)*/#define _YMS_UI_STATE_REMEMBER                                  // 2010.03.25 / 嶸貲熱 / [и措][0030]UI 贗葛 薑爾 盪濰
    /*($Delete_[x/C]$)*/#define _YMS_AREACONQUEST_MAP_TOOLTIP                           // 2010.03.25 / 嶸貲熱 / [и措][0025]AC 爾僭雖紫 罐そ 轎溘 
    /*($Delete_[x/C]$)*/#define _YMS_GAMEOPTION_LANGUAGETYPE                            // 2010.03.25 / 嶸貲熱 / 措陛滌 滲熱 蹺陛
    /*($Delete_[x/C]$)*/#define _YMS_SOUND_CACHE                                        // 2010.03.25 / 嶸貲熱 / 餌遴萄 議諒
    /*($Delete_[x/C]$)*/#define _YMS_SUMMONSKILLCONTAINER_RESET                         // 2010.05.07 / 嶸貲熱 / 嫦酈葬 模�紡鷅秣� 衛 模�紡� 蝶鑒璽檜 偵褐腎雖 彊朝 幗斜
    /*($Delete_[x/C]$)*/#define _YMS_EVENTFRIEND_SCROLLBAR                              // 2010.05.14 / 嶸貲熱 / 耀掘蟾渠 詭撮雖 蝶觼煤 腎啪

    /*($Delete_[x/C]$)*/#define _NA_1419_20100121_ACCUMULATE_POINT                      // 2010.01.21 / 陴晦瑰,檜鷓遽 / Ы溯檜 衛除縑 評艇 瞳董 ん檣お 蹺陛
    /*($Delete_[x/C]$)*/#define _100409_LTJ_SHOW_ITEM_UNIT_PRICE_IN_VENDOR              // 2010.04.13 / 檜鷓遽 / 偃檣鼻薄 偃渡陛問 爾罹輿晦
    /*($Delete_[x/C]$)*/#define _100408_LTJ_SEPARATE_TIMER_INI                          // 2010.04.13 / 檜鷓遽 / 顫檜該 婦溼 撲薑擊 PROGRAM.INI 縑憮 TIMER.INI 煎 碟葬

    #if defined(_KOREA)
    
    #elif defined(_CHINA)
        //#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)

    #elif defined(_JAPAN)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / 僥鼻⑷, 檜紫⑽ / 耀掘 蟾渠 衛蝶蠱
        #define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST            // 2010.06.22 / 梯⑷錠, 檜紫⑽ / 離欽 跡煙 偃摹

    #elif defined(_GLOBAL)
		#define _100708_LTJ_SHOW_DATETIME_IN_TIMER
		/*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)
        /*($Delete_[S/x]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 婦溼 啪歜 煎斜 蹺陛
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / 嶸貲熱 / ん驍嬪纂縑憮 嘐暮諫猿�� 蠡蝶お蒂 諫猿 跤ж朝 ⑷鼻

    #elif defined(_RUSSIA)
        #define _100708_LTJ_SHOW_DATETIME_IN_TIMER
        /*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)
        #define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 婦溼 啪歜 煎斜 蹺陛
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / 嶸貲熱 / ん驍嬪纂縑憮 嘐暮諫猿�� 蠡蝶お蒂 諫猿 跤ж朝 ⑷鼻

    #endif
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 1.0.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 1001
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(2)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

    /*($Delete_[S/C]$)*/#define _NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP                // 2009.11.19 / 鬼營遽 / NPC 鼻檣 っ衙 渠旎曖 ж歜 ч遴 瞳董徽 偃滌 褻瞰 雖錳
    /*($Delete_[S/C]$)*/#define _NA_0_20091030_RANDOM_ITEM                                  // 2009.10.30 / 陴晦瑰, 檜紫⑽ / 楠渾 嬴檜蠱 蹺陛.
    /*($Delete_[x/C]$)*/#define _GS_SEPARATE_SPECIAL_TEXTURE                                // 2009.12.04 / 梯惜熱 / 蝶む暫臢蝶藥 蝶觼董お煎 碟葬ж朝濛機
    /*($Delete_[x/C]$)*/#define _GS_MOD_NONE_DRAW_ZEROEXP									// 2009/12.07 / 梯惜熱 / EXP 0橾陽朝寰鎰晦
    
	/*($Delete_[x/C]$)*/#define _091229_LTJ_AC_REWARD_HELPMESSAGE_BUGFIX					// 2009.12.29 / 檜鷓遽 / AC 10%欽嬪 紫殖衛 爾鼻寰頂 ⑦Щ詭衛雖 轎溘腎紫煙 熱薑
	/*($Delete_[x/C]$)*/#define _100105_LTJ_ADD_SSQ_OBVERVER_ASSIST_DIALOG					// 2010.01.07 / 檜鷓遽 / 顫塊и 餌錳 婦瞪賅萄璽擊 Assist璽戲煎 掘⑷

	/*($Delete_[x/C]$)*/#define _YJ_MOD_FOG_COLOR_IN_CREATE_CHARACTER						// 2010.01.12 / 彌艙輿 / 議葛 儅撩�飛� 葬景橡腎賊憮 滲唳脹 ん斜 鏽楝 熱薑 
	/*($Delete_[x/C]$)*/#define _YJ_MOD_POS_IN_CHARACTER_SCENE								// 2010.01.12 / 彌艙輿 / 議葛 摹鷗�飛� 葬景橡腎賊憮 滲唳脹 議葛 ん雖暮 熱薑

    /*($Delete_[S/C]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                           // 2010.01.14 / 檜�ˋ� / 蠡蝶お 爾鼻衛 NPC諦曖 剪葬 羹觼.
    /*($Delete_[S/C]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                          // 2010.01.06 / 檜�ˋ� / SD 啗骯 奢衝 滲唳 

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION
    /*($Delete_[S/C]$)*/#ifdef _NA001385_20090924_DOMINATION
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_GAMELOG_ACTION            // 2010.01.12 / 檜�ˋ� / 雖羲 薄滄瞪辨 啪歜 煎斜
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_FINDPATH_ATTACK           // 2009.12.15 / 檜�ˋ� / 奢問衛 陛棟 羹觼(findpath)
        /*($Delete_[S/C]$)*/#define _0_20100427_DOMINATION_FINDPATH_ATTACK_BUG              // 2010.04.27 / 檜�ˋ� / 熱薑 錶Щ 檜翕衛 漁 剩橫煎 檜翕腎朝 幗斜 熱薑.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SHADOW                    // 2009.12.15 / 檜�ˋ� / 膠紫辦 ж檜萄 蝶鑒 滲唳.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NPC_MAXHP                 // 2009.12.09 / 檜�ˋ� / monster render info縑 maxHp 蹺陛
        /*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_SYNC                     // 2009.08.17 / 檜�ˋ� / 瓣餵 諫猿 翕晦��.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_BASE                      // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._Map
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP                     // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._stamp
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_ETC                       // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._etc(ぬ韃,だ憮)
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_TRIGGER                   // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._trigger
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SKILL                     // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._觼葬蝶驍 錶Щ 蝶鑒
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_EVENT_CONTROL             // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪._檜漸お 鐘お煤
        /*($Delete_[S/C]$)*/#define _0_20100419_DOMINATION_RETURN_TO_ENTRANCE_BUG           // 2009.09.24 / 檜�ˋ� / 雖羲薄滄瞪.殮掘煎 鬼薯 檜翕 腎雖 彊朝 嶸盪 鬼薯 蕾樓 謙猿 衛酈晦.
        /*($Delete_[S/C]$)*/#define __KR_001385_20090907_DOMINATION_PACKET                  // 2009.09.07 / 團薑艙 / 雖羲 薄滄瞪
        /*($Delete_[S/C]$)*/#define _KR_001385_20090907_DOMINATION_PARSER                   // 2009.09.07 / 團薑艙 / 雖羲 薄滄瞪 だ憮 濛機 薑曖
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_QUERY                    // 2009.10.13 / 團薑艙 / 雖羲 薄滄瞪 DB蘭葬僥 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GUILDSERVER_BASE         // 2009.10.13 / 團薑艙 / 望萄 憮幗 晦獄 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GAMESERVER_BASE          // 2009.10.13 / 團薑艙 / 啪歜 憮幗 晦獄 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_BASICINFO_SETTING        // 2009.10.13 / 團薑艙 / 雖羲 薄滄瞪 晦獄薑爾 撮た 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091103_DOMINATION_ZONEMOVE_NOTIC           // 2009.11.03 / 團薑艙 / 襄 檜翕衛縑 薄滄 雖羲曖 望萄 薑爾 ル衛 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO     // 2009.11.05 / 團薑艙 / 雖羲 薄滄瞪 薑爾 機等檜お 婦溼 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091202_DOMINATION_RETURN_HEIM              // 2009.12.02 / 團薑艙 / 雖羲 薄滄瞪 / ж歜 腎給溥輿晦 婦溼 濛機
        /*($Delete_[S/C]$)*/#define _KR_001385_20091211_DOMINATION_GAMELOG_MONEY            // 2009.12.13 / 團薑艙 / 雖羲 薄滄瞪 啪歜 煎斜 濛機
		/*($Delete_[S/C]$)*/#define __001385_LTJ_RENEWAL_NOTICE								// 2009.11.27 / 檜鷓遽 / 奢雖餌о 葬景橡
		/*($Delete_[S/C]$)*/#define __001385_LTJ_ADD_DOMINATIONWAR_TOWER_UI					// 2009.11.27 / 檜鷓遽 / 雖羲薄滄瞪 婦溼 UI 濛機
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_CAST_LEAVE_DLG								// 2009.11.27 / 彌艙輿 / CastLeave Dlg 葬景橡
		/*($Delete_[S/C]$)*/#define _YJ_MOD_MAPOBJECT_MONSTER								// 2009.11.27 / 彌艙輿 / 撲纂⑽ 跨蝶攪 餌辨 寞衝 熱薑	
		/*($Delete_[S/C]$)*/#define _YJ_EDIT_OBJ_COLOR_APPLY								// 2009.11.27 / 彌艙輿 / 裘罐縑憮 熱薑и 裘螃粽薛お/蝶Я萄お葬 鏽楝高 瞳辨
		/*($Delete_[S/C]$)*/#define _YJ_ADD_RADAR_ICON										// 2009.11.27 / 彌艙輿 / 雖羲薄滄瞪 婦溼 溯檜渦 嬴檜夔 蹺陛
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_TEXT_ON_HEAD								// 2009.11.27 / 彌艙輿 / 議葛攪 該葬嬪 議葛攪 
		/*($Delete_[S/C]$)*/#define _YJ_ADD_DAMAGE_EFFECT_NPCINFO							// 2009.12.15 / 彌艙輿 / NPCINFO縑 等嘐雖 檜めお 囀萄 蹺陛
		/*($Delete_[S/C]$)*/#define _YJ_STAMP_GAGE											// 2010.01.08 / 彌艙輿 / 陝檣 啪檜雖 濛機(Engine Ver 120)
    #endif

    /*($Delete_[x/C]$)*/#define _NA_0_20091104_FIRENDMAP_REFACTORYING                       // 2009.11.04 / 僥鼻⑷ / 錯萄 憮幗曖 User 贗楚蝶縑憮 耀掘 裘 婦葬 睡碟擊 FriendMap 贗楚蝶煎 碟葬 и棻.

    /*($Delete_[S/C]$)*/#define _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_				// 2009.11.23 / 檜擎だ, 檜鷓遽 / 寰瞪雖渠 顫橾 樓撩 蹺陛 + 縑 評艇 跨蝶攪 AI滲唳
    /*($Delete_[x/C]$)*/#define _NA_0_20100312_MODIFY_DELIMITERS_FOR_TEXTSCRIPT             // 2010.03.12 / 檜鷓遽 / (QuestText, Message, HelpLoadingMessage)蝶觼董お縑 餌辨腎朝 掘碟濠醞 " 蒂 餌辨ж雖 彊紫煙 滲唳

    /*($Delete_[x/C]$)*/#define _YMS_RANKUP_TOOLTIP                                         // 2010-03-31 / 嶸貲熱 / 楨觼機 罐そ熱薑(棻擠 蛔晝 薯и 熱纂 嘐轎溘)
    /*($Delete_[x/C]$)*/#define _YMS_CANNOT_EMOTION	                                        // 2010-03-31 / 嶸貲熱 / Я橫 鼻鷓縑憮 馬薑ル⑷ 寰腎啪

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NON_PREEMPTIVE_ATTACK_BUG     // 2010.04.09 / 檜�ˋ� / 薄滄瞪 檣濰 ��菜衛 嘐�僱� 嶸盪縑 曖п 顫啃た 腎朝 幗斜 熱薑.
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_MAPNPC_AI_BUG                 // 2010.04.09 / 檜�ˋ� / 雖羲薄滄瞪 撩僥 AI 滲唳    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP_STYLE_SKILL_BUG         // 2010.04.09 / 檜�ˋ� / 陝檣衛 蝶顫橾 蝶鑒 嘐瞳辨 蹺陛
    

    #if defined(_KOREA)
        //虜橾 檜 Ы溯斜蒂 薯剪 ж溥堅 и棻賊 DB 耀掘 纔檜綰縑憮 耀婁 婦啗 囀萄陛 5,6檣 高菟擊 賅舒 1煎 滲唳п撿 寰瞪ж棻.
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / 僥鼻⑷, 檜紫⑽ / 耀掘 蟾渠 衛蝶蠱
        #define __CN_PET_OPTION_APPLY_FLAG								// 2008.08.24 / 梯璽⑷ / ゆ 褫暮 Ы溯斜煎 摹鷗 瞳辨(蝶觼董お縑憮...)
        //#define _USING_GUILD_DESTROY_LOGGING                            // 2010.03.03 / 梯⑷錠 / 望萄 п骯 僥薯 蹺瞳擊 嬪и 煎斜 蹺陛
    #elif defined(_CHINA)
        #if defined(__APPLY_HACKSHIELD) && defined(_SUNGAME_VER)
            #define _APPLY_HACKSHIELD_MONITOR                           // 2010.03.17 / 嶸貲熱 / 醞措 р蔓萄 賅棲攪葭 瞳辨
        #endif
        #define _NA000000_100427_SSQ_DEFAULT_REWARD_                    // 檜擎だ, 嶸貲熱 / 顫塊и 餌錳 霤陛濠 瞪錳 晦獄 爾鼻 籀葬
        #define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / 檜�ˋ� / PVP衛 幗憮醴 等嘐雖 葬醴幗葬 蝶鑒檜 HP蒂 蟾婁 �蛹� ж朝 ⑷鼻.
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
      #endif
        #ifndef _YMS_SUMMONSKILLCONTAINER_RESET
            #define _YMS_SUMMONSKILLCONTAINER_RESET                     // 2010.05.07 / 嶸貲熱 / 嫦酈葬 模�紡鷅秣� 衛 模�紡� 蝶鑒璽檜 偵褐腎雖 彊朝 幗斜
        #endif
        //#define _YMS_CHINA_TAIWAN_MERGE                                 // 2010.07.15 / 嶸貲熱 / 醞措/渠虜 м藹 
        #ifndef _NA_20100802_SPEED_HACK_DETECTION
            #define _NA_20100802_SPEED_HACK_DETECTION                   // 2010.08.02 / 鬼營遽, 嶸貲熱 / 偃摹脹 In-Game 蝶Я萄р 鬚雖
        #endif
		#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)
        #ifdef _SERVER
            #define _NA_0_20100929_LANGUAGE_OF_FILTER                       // 2010.09.29 / 陴晦瑰 / в攪蒂 嬪и 措陛滌 樹橫 撲薑 蹺陛.
        #endif

    #elif defined(_JAPAN)

    #elif defined(_RUSSIA)

    #endif

#endif
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.9.0.5 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 905
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(1)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

	/*($Delete_[x/C]$)*/#define __DH_HAIM_WIN_CLOSE								// 2009.09.11 / 檜紫⑽ / 璽堅 檜翕衛 殘�鰼�
	/*($Delete_[x/C]$)*/#define _DH_CHARNAME_CHECK_BUG_005306					// 2009/1109 / 檜紫⑽ / 議葛攪 儅撩衛 檜葷檜 16夥檜お 剩擊 唳辦 醞犒羹觼衛 避朝⑷鼻 熱薑
	/*($Delete_[x/C]$)*/#define _DH_GUILDHAIM_WIN_CLOSE							// 2009/1019 / 檜紫⑽ / 望萄璽堅 詩橫雖賊 殘朝 濛機
	/*($Delete_[x/C]$)*/#define _DH_AC_UI_CHANGE								// 2009/1012 / 檜紫⑽ / AC UI滲唳 婦溼 濛機

	/*($Delete_[x/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL_2		// 2009/0704 / 檜紫⑽ / 斜瑜 裕晦 衛蝶蠱-(議葛攪で機詭景煎 斜瑜蛔煙/п營), 螃Щ塭檣 в攪 瞳辨寰腎紫煙 熱薑
	/*($Delete_[x/C]$)*/#define _DH_GIFT_CONFIRM								// 2009/1012 / 檜紫⑽ / 摹僭ж晦 營�挫� 璽 濛機
	/*($Delete_[x/C]$)*/#define _DH_HELLON_TOWER_ENTER							// 2009/0930 / 檜紫⑽ / ⑦煩曖 撩 殮濰 塽 罐そ
	/*($Delete_[x/C]$)*/#define _DH_SUNCREDIT									// 2009/0929 / 檜紫⑽ / 賬 觼楚覃 艙鼻 UI
	/*($Delete_[x/C]$)*/#define __DH_COMMUNITY_MEMO__							// 2009/0821 / 檜紫⑽ / 薹雖 還除問 滲唳

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__  // 2009.09.06 / 檜擎だ / SolarSlot 囀萄 薑葬

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_UPDATE_VER6__       // 2009.09.06 / 檜擎だ,梯惜熱 / SolarSlot ItemStream Ver6 update
    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM__                // 2009.09.15 / 檜擎だ,梯惜熱 / Riding System
    /*($Delete_[x/C]$)*/#ifdef __NA001390_090915_RIDING_SYSTEM__
    /*($Delete_[x/C]$)*/#define _GS_RIDER_SYSTEM		// 梯惜熱 / 驍匙衛蝶蠱
    /*($Delete_[x/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM_ETC__            // 2009.09.20 / 檜擎だ,梯惜熱 / 堅韓瞳戲煎 犒濩и 睡碟 熱薑囀濠... 罹溘檜 腎朝 и 濛機ж晦煎 и棻.
    /*($Delete_[S/C]$)*/#define __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__   // 2009.09.15 / 檜擎だ,梯惜熱 / Riding System - Item Part

    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED        //< v6陛 滌紫 跡瞳戲煎 蹺陛脹 婦啗煎...

	/*($Delete_[x/C]$)*/#define _GS_ELITE_FULLSET_CHANGE_ITEM					// 2009.10.19 / 梯惜熱 / 縣葬おヴ撢2離 羹檣雖嬴檜蠱
    /*($Delete_[x/C]$)*/#define _GS_ADD_SHIELD_EFFECT                           // 2009.11.02 / 梯惜熱 / 蔓萄檜めお蹺陛濛機
    /*($Delete_[x/C]$)*/#define __GS_CHANGE_TARGET_EFFECT_TYPE                  // 2009.11.09 / 梯惜熱 / 顫啃た檜めお顫殮滲唳曖煎檣и幗斜熱薑
    /*($Delete_[x/C]$)*/#define _GS_ADD_RIDER_IDLE_ANI                          // 2009.11.17 / 梯惜熱 / 驍匙婦溼 IDLE擁棲詭檜暮蹺陛
    /*($Delete_[x/C]$)*/#define _GS_LOOP_EFFECT_REMAIN                          // 2009.11.05 / 梯惜熱 / 瑞Щ擁棲詭檜暮縑憮 嫦儅腎朝瑞Щ檜めお 雖樓瞳 薯剪.儅撩寰ж晦
    /*($Delete_[x/C]$)*/#define _GS_ADD_CANDY_EVENT_WEAPON                      // 2009.11.23 / 梯惜熱 / 警蛤檜めお錚ア
    /*($Delete_[x/C]$)*/#define _GS_MOD_SHADOW_HIDE_CONDITION                   // 2009.12.15 / 梯惜熱 / ж檜萄褻剪蹺陛(だじ詹幗&薄滄瞪望錳縑啪爾檜晦)

	/*($Delete_[x/C]$)*/#define __090907_LTJ_GRAPHIC_OPTION_ADD					// 2009.10.19 / 檜鷓遽 / 顫橾綰溶註 斜楚а褫暮蹺陛
	/*($Delete_[x/C]$)*/#define __090907_LTJ_GAME_OPTION_ADD					// 2009.10.19 / 檜鷓遽 / 棻艇議葛攪,鼻薄議葛攪 爾檜晦 啪歜褫暮蹺陛
	/*($Delete_[x/C]$)*/#define __090909_LTJ_CHATDIALOG_MOD						// 2009.10.19 / 檜鷓遽 / 瓣た璽 瓣た葬蝶お 煎霜熱薑
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_POPUP_MENU_RCLICK_BUGFIX		// 2009.10.19 / 檜鷓遽 / 瓣た璽 瓣た葬蝶お縑憮 辦贗葛虜戲煎 瓣たで機詭景 寰嗑朝 幗斜熱薑
	/*($Delete_[x/C]$)*/#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS				// 2009.08.14 / 檜鷓遽 / 蠔幗蝶曖 奩雖 雜辨陛棟 贗楚蝶 罐そ 蹺陛
	/*($Delete_[x/C]$)*/#define __LTJ_HIDE_PET_NAME_JP							// 2009.07.28 / 檜鷓遽 / UI 獗梯晦棟衛 ゆ曖 啪檜雖 塽 檜葷 獗梯
	/*($Delete_[x/C]$)*/#define __090910_LTJ_CHANGEUP_CLASSNAME_JP				// 2009.09.10 / 檜鷓遽 / 羹檣雖機 議葛攪曖 唳辦 億煎遴 贗楚蝶貲戲煎 轎溘
	/*($Delete_[x/C]$)*/#define _LTJ_HERO_INFO_BUGFIX							// 2009.10.30 / 檜鷓遽 / ん驍檜翕 衛 贗塭/憮幗除曖 薑爾 碳橾纂 ⑷鼻 塽 顫塊и餌錳 殮濰猿 離馬 寰腎朝 ⑽鼻 熱薑 - �鷑貔怹� 薑爾(HERO_INFO)

    /*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION			// 2009.07.09 / 檜�ˋ�,檜鷓遽 / PK 顫橾 鼠衛 褫暮.

    /*($Delete_[S/C]$)*/#define __NA_0_20090907_GUILD_DESTROY_GM_CMD            // 2009.09.07 / 檜�ˋ� / 望萄 夥煎 餉薯 陛棟 GM 貲滄橫. 
    
    /*($Delete_[S/C]$)*/#define __NA_0_POSSESSION_ITEM_DROP						// 2009.08.27 / 梯璽⑷, 敝樓 嬴檜蠱 萄奧 陛棟

    /*($Delete_[S/C]$)*/#define __NA_0_20090902_PET_NAME_CARD_LOG               // 2009.09.02 / 鬼營遽 / ゆ 啻歜蘋萄 嬴檜蠱 餌辨 煎斜縑 渠鼻 ゆ縑 渠и 蹺陛 薑爾 んл

	/*($Delete_[x/C]$)*/#define _YJ_ADD_CONTINENTMAP_POINT						// 2009.10.20 / 彌艙輿 / в萄 蹺陛縑 評艇 渠睛雖紫 雖羲 蹺陛

    /*($Delete_[S/x]$)*/#define _KR_001399_20091020_CRYSTALLIZE_EXTEND          // 2009.10.20 / 陴晦瑰,檜鷓遽 / 唸薑�� 偃摹. 縣葬お 嬴檜蠱 唸薑�� 蹺陛, 檣疆お Lv縑 評艇 唸薑�� 嬴檜蠱 偎熱 撲薑. data\\ItemCrystalizeList.txt 檣疆お Lvв萄 蹺陛

    /*($Delete_[x/C]$)*/#define _NA_20100503_MAGIC_SKILL_SAINTAID_CANCLE_STAT_IS_CHANGE // 2010.05.03 / 團薑艙 / 縣葬詮驍 蝶鑒 撮檣お 縑檜萄陛 騰蝸ж朝 鼻鷓檜鼻 艙羲 滲唳 

    #if defined(_KOREA)

    #elif defined(_CHINA)
        //#undef __KR_001355_20090624_GROUPINGSYSTEM_ALL_2    // [2010-3-16 by i4u4me]  醞措幗瞪擎 薯剪
        
    #elif defined(_JAPAN)
        #define _JP_20100114_CHANGE_ATTRIBUTE_LETECRARFE    // 2010.01.14 / 僥鼻⑷ / 溯纔曖 僭煽(蟾晦�� 嬴檜蠱)縑 ぬ衛粽 蟾晦�� 晦棟紫 蹺陛и棻.
        #define _NA_20100106_REPURCHAS_BUGFIX_2496          //2010.01.06 僥鼻⑷ 寡ぎ襄 菟橫鬲棻 螞�� 營掘衙й陽 營掘衙 寰腎朝 幗斜 熱薑
    #endif
#endif
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.9.0.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 904
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// ж嬪 幗瞪 廓��
    #endif
    //////////////////////////////////////////////////////////////////////////

	/*($Delete_[x/C]$)*/#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / 彌艙輿 / 檜めお 葬蝶お 碟葬 濛機
	/*($Delete_[x/C]$)*/#define _GS_MOD_ITEMNAME_DRAW_BY_BV									// 2009.07.30 / 梯惜熱 / 嬴檜蠱啻歜 Я韁獐睞嬪縑鎰晦
	/*($Delete_[x/C]$)*/#define _GS_BUG_FIX_PARTY_POPUP										// 2009.07.31 / 梯惜熱 / 顫啃璽 で機璽 幗斜 熱薑	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_BAR_DRAW									// 2009.07.30 / 梯惜熱 / Hero啪檜雖(hp.mp.sd) 斜萵陽 還罹憮 斜葬朝寞衝縑憮 贗董п憮斜葬朝寞衝戲煎滲唳	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_IMG_NUMBER								// 2009.08.05 / 梯惜熱 / Hero啪檜雖(hp.mp.sd) 熱纂 text陛 嬴棋 檜嘐雖煎 斜葬晦
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_FORWARD_TOOLTIP								// 2009.08.18 / 梯惜熱 / npc虜陬擊陽 嗑朝璽 蠡蝶お貲 ...籀葬
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_HELP_TOOLTIP								// 2009.08.19 / 梯惜熱 / 嘐暮嬴檜蠱撲貲璽 臢蝶お撲貲...籀葬

    /*($Delete_[S/C]$)*/#define __NA000000_090613_TRIGGER_LOGIC_REARRANGE__                 // 2009.06.13 / 檜擎だ / お葬剪 艙羲 婦溼 偃摹 濛機
    /*($Delete_[S/C]$)*/#define __NAM00000_090621_TRIGGER_LOGIC_REARRANGE2__                // 2009.06.21 / 檜擎だ / お葬剪 艙羲 婦溼 籀葬 滲唳:跨蝶攪 模�� 籀葬 熱薑
    /*($Delete_[S/C]$)*/#define __NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__              // 2009.06.22 / 檜擎だ / 綠薑鼻 蕾樓 謙猿縑 評艇 跨蝶攪 棟溘衛 褻薑 衛薄 偃摹

    /*($Delete_[S/C]$)*/#define __NA000000_090712_SCRIPT_PARSING_SYSTEM_MODIFICATION        // 2009.07.12 / 檜擎だ / 蝶觼董お 煎註 偃摹
    /*($Delete_[S/C]$)*/#define __NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2       // 2009.07.18 / 檜擎だ / 蝶觼董お 煎註 偃摹 (だ諒 憲堅葬闈 掖羹)

    /*($Delete_[S/C]$)*/#define __NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__       // 2009.07.16 / 檜擎だ / G->C 煎曖 ぬ韃 籀葬鼻 偃摹 濛機 + 諒觼 婦溼 匐熱
    /*($Delete_[S/C]$)*/#define __NA000000_090717_SHADOW_SKILL_PROBLEM_MODIFICATION__       // 2009.07.17 / 檜擎だ / 膠紫辦 蝶鑒菟曖 檜溯敔楝 匐隸 塽 熱薑
	/*($Delete_[x/C]$)*/#define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / 梯璽⑷,檜紫⑽ / AC 翮潸 夥煎 蛔煙
	/*($Delete_[S/C]$)*/#define __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK						// 2009.06.01 / 梯璽⑷, 僭擒 嬴檜蠱 溯漣薯и 羹觼 蹺陛
	/*($Delete_[S/C]$)*/#define __BUG_34356_ATTR_RECOVERY_HP_BUG							// 2009.08.07 / 梯璽⑷, HP�蛹� 幗斜
	
	/*($Delete_[x/C]$)*/#define __LTJ_RADAR_MAP_NAME_BUGFIX									// 2009.07.17 / 檜鷓遽 / 嘐棲裘 雖羲貲 臢蝶お 澀葬朝 ⑷鼻 熱薑 - (...) 還歜ル衛, 罐そ 轎溘
	/*($Delete_[x/C]$)*/#define __LTJ_UI_DIALOG_TEXTURE_LOAD_N_RELEASE_MOD					// 2009.07.17 / 檜鷓遽 / 碳в蹂и UI 臢蝶籀 п薯 晦棟 蹺陛
	
    /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / 檜�ˋ�,檜鷓遽 / 蹂掘 跨蝶攪 KILL 熱ч 顫殮 蹺陛.
	/*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / 檜�ˋ�,檜鷓遽 / 鬚儀 蠡蝶お
    /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 檜�ˋ�,檜鷓遽 / 蠡蝶お GM貲滄橫 蹺陛

    /*($Delete_[x/C]$)*/#define __NA_001244_20090417_ATTACK_RESIST					        // 2009.04.17 / 檜�ˋ� / 霜機滌 奢問 頂撩

    /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE                 // 2009.06.18 / 檜�ˋ� / WOUNDSTATUS 紫お 等嘐雖 蝶囌顫殮 2煎 鬼薯 腎橫 氈朝 睡碟 熱薑.
    /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                        // 2009.06.18 / 檜�ˋ� / и措 PK瑙 滲唳.
    /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK               // 2009.07.06 / 檜�ˋ� / AC 爾僭 鼻濠 瓣餵衛 陛莠 替釵ル 餌塭雖雖 彊朝 幗斜.
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG                 // 2009.07.15 / 檜�ˋ� / 嬴檜蠱 營掘衙 啪歜煎斜
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG             // 2009.07.15 / 檜�ˋ� / 望萄 楨觼機衛 掏и 羹觼 ж雖 彊朝 幗斜.
    /*($Delete_[S/C]$)*/#define __NA_0_20090819_GUILDMARK_REGISTER_LOG                      // 2009.08.19 / 檜�ˋ� / 望萄 檣濰 嬴檜蠱 餌辨 煎斜.
    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / 檜�ˋ� / Itemstream曖 餌辨ж雖 彊朝 睡碟婁 敝樓 婦溼 睡碟 蟾晦��

    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM							// 2009.07.16 / 檜�ˋ�,梯惜熱 / 褒萄 衛蝶蠱.
    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE  // 2009.07.16 / 檜�ˋ�,梯惜熱 / SD鼻鷓 翕晦��.
    /*($Delete_[x/C]$)*/#define _GS_MOD_PET_STX_POS											// 2009.07.16 / 梯惜熱 / ゆ嬪纂 蝶觼董お��
	/*($Delete_[x/C]$)*/#define _GS_GSP_PORTAL_NAME_OFFSET									// 2009.07.21 / 梯惜熱 / 嘐棲裘&ヴ裘 ん驍貲臢蝶お 啜纂朝 幗斜熱薑	

    /*($Delete_[S/C]$)*/#define __NA_0_20090818_SYSTEM_MEMO_SUPPORT                         // 2009.08.18 / 鬼營遽 / 衛蝶蠱 薹雖 雖錳
    /*($Delete_[S/C]$)*/#define __NA_0_20090820_SSQ_FEE_REFUND_LOG                          // 2009.08.20 / 鬼營遽 / 顫塊и餌錳 殮濰 褒ぬ煎 檣и 殮濰猿 �笑瓟� ж歜 �僱� 煎斜 蹺陛

    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION           // 2009.06.27 / 鬼營遽 / 啪歜 頂 ж歜 模綠 薑爾 顫 憮幗 鱔雖 晦棟
    /*($Delete_[S/C]$)*/#define __NA_001359_20090619_HEIM_LOTTO_SYSTEM                      // 2009.06.19 / 鬼營遽 / ж歜 煎傳 衛蝶蠱
    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION           // 2009.06.27 / 鬼營遽 / 啪歜 頂 ж歜 �僱� 薑爾 顫 憮幗 鱔雖 晦棟
    /*($Delete_[S/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL                     //2009.06.08 / 檜紫⑽, 團薑艙 / 斜瑜ё 衛蝶蠱 賅萇 define
    /*($Delete_[S/C]$)*/#ifdef __KR_001355_20090624_GROUPINGSYSTEM_ALL                  
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_CW                  //2009.06.08 / 團薑艙 / 斜瑜ё 衛蝶蠱 [ Client	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_GW                  //2009.06.08 / 團薑艙 / 斜瑜ё 衛蝶蠱 [ Game	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DW                  //2009.06.08 / 團薑艙 / 斜瑜ё 衛蝶蠱 [ DBP		<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DATA                //2009.06.08 / 團薑艙 / 斜瑜ё 衛蝶蠱 [ DATA 婦溼 ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_PARSER		        //2009.07.07 / 團薑艙 / 斜瑜ё 衛蝶蠱 [ PARSER ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
        /*($Delete_[S/C]$)*/#define __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG              //斜瑜 衛蝶蠱 詭撮雖歜擊 憲葬朝 濛機 define
        /*($Delete_[S/C]$)*/#define _DH_GROUPSYSTEM_CHANGE_PACKET                           //斜瑜 ж歜ぬ韃 CW->CG煎 滲唳
    /*($Delete_[S/C]$)*/#endif //__KR_001355_20090624_GROUPINGSYSTEM_ALL

    /*($Delete_[S/C]$)*/#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS               //2010.04.15 / 團薑艙 / ん檣お 楨韁 唸骯 褻勒 熱薑 Mission Key => RegisterDate
    /*($Delete_[S/C]$)*/#define _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG              // 2010.04.23 / 團薑艙 / 斜瑜 衛蝶蠱 啪歜 煎斜 濛機 

    /*($Delete_[S/C]$)*/#define _NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION       // 2010.05.18 / 鬼營遽 / 輿橫霞 霤褻 濰雜 嬪纂諦 偽擎 謙盟曖 濰雜 嬴檜蠱 (奩雖, か熱學撮餌葬 蛔) 匐儀 晦棟

    #if defined(_KOREA)
        #define __NA000000_090911_NEOWIZ_USER_CCU_REPORT__ // 2009.09.11 / 檜擎だ / 啻螃嬪鍔 瓣割葭縑 曖и 嶸盪 掘碟 蹺陛 濛機
  
    #elif defined(_CHINA)
		#define __DH_HAIM_WIN_CLOSE										// 2009.09.11 / 檜紫⑽ / 璽堅 檜翕衛 殘�鰼�
        /*($Delete_[S/x]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                       // 2010.01.14 / 檜�ˋ� / 蠡蝶お 爾鼻衛 NPC諦曖 剪葬 羹觼.
        /*($Delete_[S/x]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                      // 2010.01.06 / 檜�ˋ� / SD 啗骯 奢衝 滲唳 
        #define _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE               // 2010.01.15 / 鬼營遽 / [醞措] ж歜 ч遴 犒掏辨雖 霜蕾 掘殮 晦棟
        //#define _DH_NOAPPLY_GROUPINGSYSTEM_                             // 2010.01.19 / 檜紫⑽ / 斜瑜 嘐瞳辨        
        #define _NA_0_SETITEM_FULL_OPTION_COUNT_CHANGE                  // 2010.02.01 / 檜�ˋ� / 撢お 嬴檜蠱 ヴ 褫暮 啃熱 滲唳 5 -> 6.
        #define _DH_CHANGE_SHADOW_SKILL_DARK_BREAK                      // 2010.02.02 / 檜紫⑽ / 棻觼粽溯檜觼 蝶鑒 滲唳
        /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / 檜�ˋ� / 蠡蝶お 奩犒 幗斜 曖褕 睡碟 熱薑
        /*($Delete_[S/x]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / 檜�ˋ� / 蠡蝶お 偎熱 羹觼 蹺陛.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
        
    #elif defined(_RUSSIA)        

    #endif
#endif
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.9.0.3 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 903
/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(8)		// ж嬪 幗瞪 廓��
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_UNUSEABLE_MAPIMG__			// 2009.06.03 // 檜紫⑽ // 噙雖 彊朝 嘐棲裘 в攪
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_REMOVE_UI__				// 2009.05.29 // 檜紫⑽ // 餌辨ж雖 彊朝 UI 薯剪
	/*($Delete_[x/C]$)*/#define _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	// 2009.05.22 // 檜紫⑽ // 濰綠 UI 薑溺 晦棟(Tooltip)

	/*($Delete_[x/C]$)*/#define _GS_GSP_PK_BLOCK_ESODE								// 2009.07.30 // 梯惜熱 // 縑模萄葆擊縑憮朝  PK 虞晦
	/*($Delete_[x/C]$)*/#define _GS_GSP_SCROLL_PROFILE								// 2009.07.30 // 梯惜熱 // 議葛儅撩璽 Щ煎в 蝶觼煤籀葬

	/*($Delete_[x/C]$)*/#define __KR_0_20090519_EASY_GET_LUCKYMONSTER_				//2009.05.19 / 團薑艙 / 楣酈 跨蝶攪 模資蘋遴た擊 HP 98%睡攪 衛濛

	/*($Delete_[S/C]$)*/#define __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM			// 2009.05.11 / 檜�ˋ�, 檜紫⑽ / 蠡蝶お 渠羲ァ 1500 -> 3000 鼻щ 
	/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / 檜�ˋ� / 醞措 GM 馨葛攪 耀掘 蹺陛 跤л.	(措陛 奢鱔)
	/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / 檜�ˋ� / 醞措 膠紫辦 殮濰 碳陛 裘		(措陛 奢鱔)
	/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / 檜�ˋ� / 醞措 望萄 璽堅縑 爾婦 碳陛棟 顫殮蹺陛.(措陛 奢鱔)
	/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / 檜�ˋ�, 梯惜熱 / 醞措 瓣餵諫猿�� 爾鼻 ぬ韃 滲唳.(措陛 奢鱔)
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP             // 2009.07.14 / 檜�ˋ� / 濛瞪雖紫諦 琿蝶驛擎 GM裘婁 縑模萄 葆擊煎 檜翕й熱 橈戲貊, 謝ル 盪濰й熱 橈棻.
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / 檜�ˋ� / 望萄 葆觼 蝶觼董お 撲薑 溯漣煎 滲唳檜 寰腎朝 幗斜.
    /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / 檜�ˋ�, 彌艙輿 / 望萄 楠渦 檣ん 等檜攪 蹺陛.
    /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / 檜�ˋ�, 彌艙輿 / 望萄 陛殮衛 輿嬪縑 憲葡 ぬ韃 滲唳 (望萄 蛔晝, 婦啗).

	/*($Delete_[S/C]$)*/#define __NA_1323_CHAO_TIME_DECREASE_ITEM						// 2009.04.16 / 梯璽⑷, 梯惜熱 / 蘋螃衛除 馬模 嬴檜蠱
	/*($Delete_[S/C]$)*/#define __NA_1324_IGTYPE_IMPROVEMENT							// 2009.04.29 / 梯璽⑷ / IGType 偃摹
	/*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 梯璽⑷ / DROP婦溼 GM 貲滄橫 蹺陛
	/*($Delete_[S/C]$)*/#define __NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP				// 2009.05.19 / 梯璽⑷ / 敝樓 嬴檜蠱 楨觼機 熱薑
	/*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT					// 2009.05.26 / 梯璽⑷, 檜鷓遽 / 蠡蝶お 熱塊 嬴檜蠱 偃摹
	/*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / 梯璽⑷, 蠡蝶お 蹂掘 嬴檜蠱 餉薯 3偃梱雖虜 腎朝 幗斜
	/*($Delete_[S/C]$)*/#define __NA_0_CASH_ITEM_RENEWAL_SUPERMASTER					// 2009.07.02 / 梯璽⑷, 檜鷓遽 / 議衛嬴檜蠱 葬景橡 1.貲雖�眥�曖 雖�硅�

	/*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 檜擎だ / 議衛嬴檜蠱 葬景橡#2 避擎 �� 餌塭雖雖 彊朝 幗Щ

    /*($Delete_[S/C]$)*/#define __NA0000_090416_RETURN_STATE_MOB_CONTROL__                  // 2009.04.16 / 檜擎だ / 跨蝶攪 犒敝縑 渠и 鼻鷓 薯擒 籀葬 滲唳
    /*($Delete_[S/C]$)*/#define __NA000000_090506_EVENT_TYPE_MODIFICATION__                 // 2009.05.06 / 檜擎だ / 橾奩 嶸盪 Ы溯檜顫歜 憮綠蝶 雖錳

	/*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                      // 2009.05.14 / 檜擎だ,檜鷓遽 / だじ錳 衝滌 偃摹

	/*($Delete_[x/C]$)*/#define __001333_LTJ_QUEST_LIST_UI_REFORM					// 2009.05.22 / 檜鷓遽 / 蠡蝶お璽 偃摹
	/*($Delete_[x/C]$)*/#define __001331_LTJ_AC_TREASUREBOX_TOOLTIP					// 2009.05.22 / 檜鷓遽 / AC 璽 偃摹
	/*($Delete_[x/C]$)*/#define __001341_LTJ_POSSESSION_ITEM_MODIFY					// 2009.05.22 / 檜鷓遽 / 敝樓 嬴檜蠱 偃摹 - 敝樓檣隸 嬴檜夔 轎溘
	/*($Delete_[x/C]$)*/#define __001345_LTJ_QUEST_REPEAT_DISPLAY					// 2009.05.22 / 檜鷓遽 / 奩犒 捨蝶お 離蛔 ル衛

	/*($Delete_[S/C]$)*/#define __NA_001304_20090521_GUILD_LOG_FIX                  // 2009.05.21 / 鬼營遽 / 望萄 煎斜 晦棟 營濛撩
    /*($Delete_[x/C]$)*/#define __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT          // 2009.08.12 / 鬼營遽 / 夥檜傘葬 啪歜 煎梵 偃摹
    /*($Delete_[x/C]$)*/#define __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY           // 2009.08.13 / 鬼營遽 / 夥檜傘葬 啪歜 煎斜 煎註衛 螃盟 犒掘

	/*($Delete_[x/C]$)*/#define _YJ_MOD_GUILD_MARK_CREATE_GRADE						//2009.07.08 / 彌艙輿 / 望萄葆觼蒂 殖熱氈朝 望萄 溯漣 滲唳(3Lv -> 1Lv)
	/*($Delete_[x/C]$)*/#define _YJ_ADD_GUILD_MARK_TEX								//2009.07.08 / 彌艙輿 / 望萄葆觼 僥曄 蹺陛

    /*($Delete_[x/C]$)*/#define _NA_0_20091020_COMPACT_COMPOSITION_LOG_         // 2009.10.20 / 鬼營遽 / 褻м 煎斜 蹺陛 薑爾縑憮 營猿 嬴檜蠱 跡煙 澀葡 寞雖蒂 嬪и 蹴擒 ん裝 餌辨

    #if defined(_KOREA)
		#define __001347_LTJ_REMOVE_TEMP_INVENTORY				// 2009.05.22 / 檜鷓遽 / 爾褻 璽堅 薯剪 濛機

	#elif defined(_CHINA)
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 檜�ˋ�,檜鷓遽 / 蠡蝶お GM貲滄橫 蹺陛
        #define __NA_0_POSSESSION_ITEM_DROP								    // 2009.08.27 / 梯璽⑷, 敝樓 嬴檜蠱 萄奧 陛棟
        /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / 檜�ˋ� / Itemstream曖 餌辨ж雖 彊朝 睡碟婁 敝樓 婦溼 睡碟 蟾晦��
        #define _NA_0_20091020_COMPACT_COMPOSITION_LOG_
		#define _DH_TOOLTIP_LIMIT											// 2009.10.29 / 檜紫⑽ / 薯濛衛 褻м 陛棟 嬴檜蠱 罐そ縑 嗑朝 嬴檜蠱 偎熱 薯и

	#elif defined(_JAPAN)
        /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / 檜�ˋ�,檜鷓遽 / 蹂掘 跨蝶攪 KILL 熱ч 顫殮 蹺陛.
        /*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / 檜�ˋ�,檜鷓遽 / 鬚儀 蠡蝶お
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 檜�ˋ�,檜鷓遽 / 蠡蝶お GM貲滄橫 蹺陛
        #define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / 梯璽⑷,檜紫⑽ / AC 翮潸 夥煎 蛔煙
		#define _DH_AC_UI_CHANGE											// 2009/1012 / 檜紫⑽ / AC UI滲唳 婦溼 濛機
		#define _DH_HELLON_TOWER_ENTER										// 2009/0930 / 檜紫⑽ / ⑦煩曖 撩 殮濰 塽 罐そ
		#define __GS_CHANGE_TARGET_EFFECT_TYPE				                // 2009.11.09 / 梯惜熱 / 顫啃た檜めお顫殮滲唳曖煎檣и幗斜熱薑
		#define _GS_ADD_CANDY_EVENT_WEAPON					                // 2009.11.23 / 梯惜熱 / 警蛤檜めお錚ア
		#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / 彌艙輿 / 檜めお 葬蝶お 碟葬 濛機

    #elif defined(_GLOBAL)
        #define _DH_TOOLTIP_LIMIT											// 2009.10.29 / 檜紫⑽ / 薯濛衛 褻м 陛棟 嬴檜蠱 罐そ縑 嗑朝 嬴檜蠱 偎熱 薯и
		#undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN

    #elif defined(_RUSSIA)
        #undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	  
	#endif
#endif
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.9.0.2 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 902
/////////////////////       Protocol Version       //////////////////////
#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(6)		// ж嬪 幗瞪 廓��
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __001251_MATERIAL_ITEM_COMPOSE_LIST					// 2009.03.05 // 檜鷓遽 // 褻м陛棟 跡煙 ル衛
	/*($Delete_[x/C]$)*/#define __001268_CHANGEUP_SKILL_TOOLTIP						// 2009.03.05 // 檜鷓遽 // 羹檣雖機 蝶鑒 薯擒褻勒 罐そ 蹺陛
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_SKILL_DIALOG_TAB						// 2009.03.11 // 檜鷓遽 // 蝶鑒棻檜橡煎斜 譆斬 餌辨龔 嶸雖
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_CHAR_DIALOG_UPBUTTON_HIDE				// 2009.03.11 // 檜鷓遽 // 議葛攪蝶鑒棻檜橡煎斜 ん檣お 機 幗が 獗梯
	/*($Delete_[x/C]$)*/#define __001314_LTJ_ADD_CLOCK_OPTION_TIRED_SYSTEM			// 2009.03.12 // 檜鷓遽 // Я煎紫 衛蝶蠱曖 衛啗 晦棟 碟葬
	/*($Delete_[x/C]$)*/#define __001285_LTJ_MOD_CREATE_CHAR_DESCRIPTION			// 2009.03.13 // 檜鷓遽 // 議葛攪 儅撩 撲薑 臢蝶お 熱薑
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_DROP_ITEM_NOTIFY_TIMELINE				// 2009.03.18 // 檜鷓遽 // 萄奧嬴檜蠱 蝗菜 憲葡衛蝶蠱縑 濠翕 蝗菜 嬴檜蠱蹺陛
	/*($Delete_[x/C]$)*/#define __LTJ_DISPLAY_FULL_INVENTORY_MESSAGE				// 2009.03.20 // 檜鷓遽 // 嬴檜蠱 蝗菜 �� 檣漸饜葬縑 罹嶸 奢除檜 橈擊 衛 詭衛雖 轎溘
	/*($Delete_[x/C]$)*/#define __LTJ_DRAW_NEW_SERVER_ICON_SERVER_LIST				// 2009.04.01 // 檜鷓遽	// 憮幗摹鷗璽縑憮 褐憮幗 NEW 幗が 斜葬晦
	/*($Delete_[x/C]$)*/#define __001326_LTJ_LEVEL_COLOR_MODIFY_2ND					// 2009.04.06 // 檜鷓遽 // 瞳薑溯漣 ル衛 蛔晝 滲唳 7欽啗->5欽啗煎 滲唳
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_SCROLL_BUGFIX						// 2009.04.06 // 檜鷓遽	// 瓣た璽 蝶觼煤夥 譆鼻嬪煎 螢塭陛朝 ⑷鼻 熱薑
	/*($Delete_[x/C]$)*/#define __LTJ_EVENT_TIMER_MODE_MODIFY						// 2009.04.21 // 檜鷓遽	// 檜漸お 顫檜該 賅萄蹺陛

	/*($Delete_[x/C]$)*/#define _YJ_RENEWAL_QUEST_UI_001265							// 09.02.05 //彌艙輿 //蠡蝶お璽 葬景橡
		/*($Delete_[x/C]$)*/#ifdef _YJ_RENEWAL_QUEST_UI_001265
			/*($Delete_[x/C]$)*/#define __DH_004145_20090710_ITEMQUESTBUG			// 2009.07.17 //檜紫⑽ //幗斜 熱薑: 嬴檜蠱 蠡蝶お 棻賅擎�� 餉薯衛, 諫猿煎 ル衛
			/*($Delete_[x/C]$)*/#define _KIKI_REMOVE_QUEST_INCOMPLETE_PACKET_LOGIC	// 2009.11.09 //檜雖翱 // CG_QUEST_INCOMPLETE_SYN蒂 ��轎ж雖 彊堅, 贗塭檜樹お縑憮 濠羹瞳戲煎 蠡蝶お 嘐諫猿 鼻鷓煎 偵褐ж紫煙 熱薑
			/*($Delete_[x/C]$)*/#define _KIKI_CHECK_SEND_QUEST_COMPLETE_PACKET		// 2009.11.09 //檜雖翱 // CG_QUEST_COMPLETE_SYN 陛 醞犒 瞪歎腎雖 彊紫煙 羹觼 �� 爾魚棻.
		/*($Delete_[x/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _YJ_CONTINENT_MAP_001277							//09.02.05 //彌艙輿 //渠睛 雖紫 蹺陛
	/*($Delete_[S/C]$)*/#define _YJ_NAVI_SYSTEM_001277								//09.02.05 //彌艙輿 //啻綠 衛蝶蠱 + 檜擎だ(贗塭 跨蝶攪 啻綠 雖錳 籀葬 in ServerCode)
    /*($Delete_[S/C]$)*/#ifdef _YJ_NAVI_SYSTEM_001277
        /*($Delete_[S/C]$)*/#define __NA001277_090205_NAVI_SYSTEM_SUPPORT__         //09.03.xx //檜擎だ,彌艙輿/啻綠 衛蝶蠱 憮幗薹 雖錳 濛機 (蛤だ檣 薯剪衛 �扒� 寞雖辨戲煎 蹺陛)
    /*($Delete_[S/C]$)*/#endif

	
	/*($Delete_[x/C]$)*/#define _YJ_TOOLTIP_ON_TEXT_001277							//09.03.09 //彌艙輿 //蠡蝶お 婦溼 罐そ 濛機 + 檜擎だ(贗塭 詭衛雖 掘碟 雖錳 籀葬 in ServerCode)
	/*($Delete_[x/C]$)*/#define _YJ_REUSE_SAME_MAP_RADAR_TEXTURE					//09.03.12 //彌艙輿 //
	/*($Delete_[x/C]$)*/#define _YJ_COLLECTION_SYSTEM_001283						//09.03.04 //彌艙輿 //瓣餵 衛蝶蠱
	/*($Delete_[x/C]$)*/#define _YJ_SHOW_QUEST_EVEN_IF_UNDER_MIN_LEV				//09.04.17 //彌艙輿 //[001328]棻擠 蠡蝶お 嘐葬 爾晦 濛機

	/*($Delete_[S/C]$)*/#define __NA_0_CAN_RESURRECTION_ON_VILLAGE					// 2009.03.25 / 梯璽⑷,梯惜熱/ 葆擊縑憮 睡�� 陛棟
	/*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_ETHER_BULLET_BY_GM_CMD				// 2009.01.07 / 梯璽⑷, 縑纔腦 憮幗 遘朝 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_1241_AUTO_PICKUP_PET_OPTION					// 2009.01.14 / 梯璽⑷, 梯惜熱 / 嬴檜蠱 濠翕�僱磈炴� 褫暮
	/*($Delete_[S/C]$)*/#define __NA_1247_POSSESSION_ITEM							// 2009.01.17 / 梯璽⑷, 檜鷓遽 / 敝樓 嬴檜蠱
	/*($Delete_[S/C]$)*/#define __NA_1269_CHANGE_STYLE_OPTION_BREAKER_OF_FUZ		// 2009.02.05 / 梯璽⑷, �瓾轃� 粽溯檜醴 3顫縑虜 褫暮 瞳辨 

	/*($Delete_[S/C]$)*/#define __NA_1288_AC_RENEWAL_FOR_TIME_LINE					// 2009.02.19 / 梯璽⑷, 檜鷓遽 / [顫歜塭檣]AC 葬景橡
		/*($Delete_[S/C]$)*/#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
			/*($Delete_[S/C]$)*/#define _LTJ_AC_REMOVE_CORRUPT_TEMPLE_ADD_MAIN_QUEST	// 2009.03.23 / 檜鷓遽, AC 葬景橡 - 嘐暮 顫塊и餌錳 薯剪, 陝雖羲 詭檣蠡蝶お 贗葬橫 �蝦� 蹺陛
		/*($Delete_[S/C]$)*/#endif
	/*($Delete_[S/C]$)*/#define __NA_1248_SSQ_ADD_LOG								// 2009.01.20 / 梯璽⑷, 顫塊и 餌錳 煎斜
	/*($Delete_[S/C]$)*/#define __NA_1307_LOTTERY_SCRIPT_IMPROVEMENT				// 2009.03.02 / 梯璽⑷, 犒掏 蝶觼董お 偃摹
	/*($Delete_[S/C]$)*/#define __NA_0_ADD_GM_CMD_ABOUT_AC							// 2009.03.05 / 梯璽⑷, AC婦溼 GM貲滄橫 蹺陛
	/*($Delete_[S/C]$)*/#define __NA_1313_AC_OVER_MODIFY							// 2009.03.10 / 梯璽⑷, AC 殖撩睦 螃幗Ы煎 熱薑
	/*($Delete_[S/C]$)*/#define __NA_0_TEMP_INVENTORY_DELETE						// 2009.03.21 / 梯璽⑷, 檜鷓遽
	/*($Delete_[S/C]$)*/#define __NA_0_RECALL_ALLIANCE_CAN_USE_ALONE				// 2009.03.25 / 梯璽⑷, �母� 罹褐曖 擎�� 餌辨 陛棟

	/*($Delete_[S/C]$)*/#define __NA_0_AC_FOR_REAL_TIME_REFRESH						// 2009.02.14 / 梯璽⑷, 檜鷓遽/ AC 褒衛除 偵褐擊 嬪и 濛機

	/*($Delete_[x/C]$)*/#define _DH_KOR_000000_20090522_CHARACTER_SCENE_NAME_OVERFLOW_REPAIR	// 2009.05.25 / 檜紫⑽ / 議葛攪 摹鷗縑憮 酈贖 議葛攪 檜葷 鼻衛 ル衛, NO.001346

	/*($Delete_[S/C]$)*/#define __NA_0_20080414_GUILD_WAREHOUSE_LOG_BUG							// 2009.04.14 / 檜熱鷗 / 望萄 璽堅 煎斜 澀跤 陴晦湍 幗斜.

	/*($Delete_[x/C]$)*/#define __NA003373_20090402_NOT_FIND_ENGLISHNAME_BUG					// 2009.04.02 / 檜熱鷗 / 望萄 蟾渠衛 艙僥 渠模僥濠蒂 掘碟ж朝 ⑷鼻.
	/*($Delete_[S/C]$)*/#define __NA002150_20090402_GUILD_ALLIENCE_NOT_CHK_DESTROY_WAIT_BUG		// 2009.04.02 / 檜熱鷗 / 望萄 п骯 渠晦醞檣 望萄縑啪 翕裊 褐羶檜 陛棟и ⑷鼻.
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTACCEPT_GM_CMD								// 2009.04.07 / 檜熱鷗 / 蠡蝶お 鬼薯 熱塊 gm 貲滄橫
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY							// 2009.04.07 / 檜熱鷗 / 蠡蝶お 蝶觼董お 睡罹 嬴檜蠱 о跡 3в萄 瞳辨 寰腎朝 幗斜.
	/*($Delete_[S/C]$)*/#define __NA_0_20090408_GUILD_RELATION_CHECKRIGHT_BUG					// 2009.04.08 / 檜熱鷗 / 望萄 翕裊衛 掏и檜 橈擠 釭螃朝 幗斜.
	/*($Delete_[S/C]$)*/#define __NA_0_20090324_RANKINGSYSTEMCTL								// 2009.03.24 / 檜熱鷗 / 寡ぎ 憮幗 避朝 幗斜 歜衛 熱薑
	/*($Delete_[S/C]$)*/#define __NA001254_20090109_RTTG_FUNC_ADD								// 2009.01.09 / 檜熱鷗 / 鄴蝶 貲滄 蹺陛
	/*($Delete_[S/C]$)*/#define __NA000000_20090320_DELAY_ITEM_DROP_TIME						// 2009.03.20 / 檜熱鷗 / 跨蝶攪 嬴檜蠱 雖翱 萄奧 
	/*($Delete_[S/C]$)*/#define __NA001140_20080128_SOUND_TRIGGER_ADD							// 2009.01.28 / 檜熱鷗, 彌艙輿 / 餌遴萄 營儅 お葬剪 蹺陛
	/*($Delete_[S/C]$)*/#define __NA001282_20090206_MODIFY_QUEST_TIME							// 2009.02.06 / 檜熱鷗, 彌艙輿 / [顫歜塭檣]蠡蝶お 睡罹 衛除 薯и 偃摹
	/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECT										// 2009.02.11 / 檜熱鷗 / [顫歜塭檣]瓣餵 衛蝶蠱
		/*($Delete_[S/C]$)*/#ifdef __NA001283_20090221_COLLECT
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PACKET			
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PARSER
			/*($Delete_[S/C]$)*/#define __NA001283_20090223_COLLECTION_CLASS
			/*($Delete_[S/C]$)*/#define __NA001283_20090225_COLLECTION_USE
			/*($Delete_[S/C]$)*/#define __NA001283_20090227_COLLECTION_LIST
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_GM
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_MAPOBJECTID
		/*($Delete_[S/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _GS_MOD_PET_GAUGE_POS_1242							// 2009.03.103 梯惜熱 / ゆ 鼻鷓 UI 嬪纂 堅薑
	/*($Delete_[x/C]$)*/#define __GS_ADD_REMOVE_TUTORIAL_MSG_1315					// 2009.03.13  梯惜熱 / ぅ饜葬橡 詭衛雖薯剪
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ACTIVE_FOR_CLASS					// 2009.03.18  梯惜熱 / 蠡蝶お爾鼻璽縑憮 濠褐贗楚蝶蜃朝嬴檜蠱摹鷗�側瘓� 
	/*($Delete_[x/C]$)*/#define __GS_ADD_PANDORA_BOX_NAME							// 2009.03.18  梯惜熱 / っ紫塭鼻濠 啻歜 嬴檜蠱婁翕橾ж啪 滲唳 
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ITEM_LIST_COLOR						// 2009.03.19  梯惜熱 / 蠡蝶お爾鼻璽嬴檜蠱 爾鼻衛諦 歜鼠衛 儀棻腦啪滲唳
	/*($Delete_[x/C]$)*/#define __GS_ADD_SCREEN_OPTION_SCROLL						// 2009.03.22  梯惜熱 / 斜楚а璽п鼻紫 撲薑婦溼巍爾夢蝶 葆辦蝶�棌瘍弧捙葥�
	/*($Delete_[x/C]$)*/#define __GS_MOD_WAYPOINT_LEVEL_CHECK						// 2009.03.25  梯惜熱 / 錚檜ん檣お 溯漣ル衛塽 薯и羹觼
	/*($Delete_[x/C]$)*/#define __CN_000097_GM_HIDE_TAGETING_BLOCK					// 2009.05.13  梯惜熱 / GM議葛癱貲鼻鷓衛 顫啃た碳陛

    /*($Delete_[S/C]$)*/#define __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__         // 2009.02.09 / 檜擎だ,梯惜熱 / [顫歜塭檣] 蠡蝶お 塽 в萄縑憮曖 嘐暮 + 寡ぎ襄 薯и 撲薑
    /*($Delete_[S/C]$)*/#define __NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__   // 2009.02.26 / 檜擎だ,梯惜熱 / ん驍擊 鱔и 葆擊|в萄 犒敝衛 雖薑脹 裘戲煎 犒敝 雖錳
    /*($Delete_[S/C]$)*/#define __NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__              // 2009.05.19 / 檜擎だ,梯惜熱 / 縑模萄 葆擊 (в萄顫殮縑憮 葆擊 顫殮戲煎 滲唳)


    /*($Delete_[x/C]$)*/#define __NA_4710_20090819_REPAIREMONEY_FIX                     // 2009.08.24 / 團薑艙 / 熱葬 旎擋曖 贗塭檜樹お諦 憮幗除曖 模熱薄 啗骯離檜縑曖и 離檜幗斜
    /*($Delete_[x/C]$)*/#define __NA_34726_20090421_ARMDRAGON_TRANSFORM_BUG_FIX //2009.04.21 / 團薑艙 / 懍萄溯堆 お溶蝶イ 滲褐�� 啪歜 謙猿衛 議蓮 僭擒 餌塭雖朝 幗斜 熱薑


    /*($Delete_[S/C]$)*/#define __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__  // 2009.03.10 / 檜擎だ / Object縑 渠и 薯剪衛 營敝 掘褻 僥薯 п唸 濛機
    /*($Delete_[S/C]$)*/#define __NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__   // 2009.03.16 / 檜擎だ / [顫歜塭檣] 寡ぎ襄 薯и 撲薑 + 煎綠縑憮紫 п渡 薯擒褻勒 勘葬紫煙...
    /*($Delete_[S/C]$)*/#define __NA001319_SAME_CLASS_LIMIT_REMOVE__                        // 2009.03.16 / 檜擎だ / 翕橾 贗楚蝶 薯擒(5貲) 薯剪
    /*($Delete_[S/C]$)*/#define __NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__          // 2009.03.20 / 檜擎だ / 0902 熱薑 蹂羶餌о菟 (蠡蝶お 婦溼)
    /*($Delete_[S/C]$)*/#define __NA000000_090327_MONSTER_HELP_FILTER__                     // 2009.03.27 / 檜擎だ / Grade 9,5曖 唳辦朝 Help縑 擬港ж雖 彊紫煙 籀葬
    /*($Delete_[S/C]$)*/#define __NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__           // 2009.04.07 / 檜擎だ / ん驍 嘐暮 熱ч衛 '貲雖�眥�曖 雖�硅�'婁 偽擎 寡ぎ襄縑 撲薑腎朝 樓撩 瞳辨腎紫煙...
    /*($Delete_[S/C]$)*/#define __NA_1317_20090316_SKILL_DAMAGE_FORMULA_MODIFY		// 2009.03.16 / 檜遽戮 / 蝶鑒等嘐雖 奢衝 熱薑
	/*($Delete_[S/C]$)*/#define __NA_1320_20090316_STATUS_RATIO_FORMULA_MODIFY		// 2009.03.16 / 檜遽戮 / 檜鼻鼻鷓 �捕� 奢衝 熱薑
	//#define __NA_0_20090317_ATTACKPOWER_FORMULA_MODIFY			// 2009.03.17 / 檜遽戮 / 嫦酈葬 奢問溘 奢衝 熱薑 (錳犒 籀葬)
	/*($Delete_[S/C]$)*/#define __NA_0_20090318_AGGRO_ABILITY_MODIFY				// 2009.03.18 / 檜遽戮 / 橫斜煎 橫網葬じ 熱薑
    

  //------------------------------------------------------------------------------------------------
  // (BUG-FIX)
  /*($Delete_[S/C]$)*/#define __BUG_35162_COMPOSE_BUG_BY_FULL_IVEN					// 2009.06.29 / 梯璽⑷, 檣漸饜葬陛 陛菜雙 鼻鷓縑憮 薯濛褻м 衛 幗斜

	#ifdef _KOREA
        #define __KR000000_090128_CHANNELING__                              // 2009.01.28 / 檜擎だ,檜鷓遽 / 瓣割葭 婦溼 滲唳 濛機
	#elif defined(_JAPAN) 
		#define __LTJ_JP_TEMP_INVEN_MOD	
		#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS						// 2009.08.14 / 檜鷓遽 / 蠔幗蝶曖 奩雖 雜辨陛棟 贗楚蝶 罐そ 蹺陛
		#define __LTJ_DISABLE_HUNTINGZONE_BUTTON_JP						// 2009.07.30 / 檜鷓遽 / 寡ぎ襄 煎綠 ьた婦溼 UI 綠�側瘓�
		#define __LTJ_HIDE_PET_NAME_JP									// 2009.07.28 / 檜鷓遽 / UI 獗梯晦棟衛 ゆ曖 啪檜雖 塽 檜葷 獗梯
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 梯璽⑷ / DROP婦溼 GM 貲滄橫 蹺陛		
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / 檜�ˋ� / 望萄 楨觼機衛 掏и 羹觼 ж雖 彊朝 幗斜.(橾獄幗斜)
        /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK           // 2009.07.06 / 檜�ˋ� / AC 爾僭 鼻濠 瓣餵衛 陛莠 替釵ル 餌塭雖雖 彊朝 幗斜.

		/*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION					// 2009.07.09 / 檜�ˋ�,檜鷓遽 / PK 顫橾 鼠衛 褫暮.

        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 檜擎だ / 議衛嬴檜蠱 葬景橡#2 避擎 �� 餌塭雖雖 彊朝 幗Щ

	#elif defined(_CHINA)	
		/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / 檜熱鷗 / 醞措 GM 馨葛攪 耀掘 蹺陛 跤л.
		/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / 檜熱鷗 / 醞措 膠紫辦 殮濰 碳陛 裘
		/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / 檜熱鷗 / 醞措 望萄 璽堅縑 爾婦 碳陛棟 顫殮蹺陛.
		/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / 檜熱鷗, 梯惜熱 / 醞措 瓣餵諫猿�� 爾鼻 ぬ韃 滲唳.
		#define __CN_000005_20090522_WAREHOUSE_ISACCEPTABLE_X			// 2009.05.22 / 檜熱鷗 / 醞措 璽堅 綠廓縑 憲だ漯 x 餌辨.
        #define __CN_0_20090617_PRE_CHAO_USE_FLAG	                    // 2009.06.16 / 檜熱鷗 / 醞措 憮幗縑憮朝 Щ葬蘋螃陛 鼻薄檜辨 陛棟.
        /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE             // 2009.06.18 / 檜熱鷗 / WOUNDSTATUS 紫お 等嘐雖 蝶囌顫殮 2煎 鬼薯 腎橫 氈朝 睡碟 熱薑.
	    /*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / 梯璽⑷, 蠡蝶お 蹂掘 嬴檜蠱 餉薯 3偃梱雖虜 腎朝 幗斜
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG             // 2009.07.15 / 檜�ˋ� / 嬴檜蠱 營掘衙 啪歜煎斜

        /*($Delete_[x/C]$)*/#define __CN_0_20090706_CHINA_VISTA_CHARNAME                 	// 2009.07.06 / 檜�ˋ� / 醞措 綠蝶顫 纔蝶お

#elif defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / 檜�ˋ� / 望萄 楨觼機衛 掏и 羹觼 ж雖 彊朝 幗斜.
        /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / 檜�ˋ� / 望萄 葆觼 蝶觼董お 撲薑 溯漣煎 滲唳檜 寰腎朝 幗斜.
        /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / 檜�ˋ�, 彌艙輿 / 望萄 楠渦 檣ん 等檜攪 蹺陛.
        /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / 檜�ˋ� / 望萄 陛殮衛 輿嬪縑 憲葡 ぬ韃 滲唳 (望萄 蛔晝, 婦啗).
		#define _YJ_MOD_GUILD_MARK_CREATE_GRADE							// 2009.07.08 / 彌艙輿 / 望萄葆觼蒂 殖熱氈朝 望萄 溯漣
        #define _US_ACCOUNT_LENTH_UNCHECKED_                            // 2009.08.28 / 團薑艙 / 啗薑 望檜 羹觼ж雖 彊擠(磁嘐 幗瞪 薯и)
        #define _US_SHOP_GIFT_ERRORCONTROL_ADD                          // 2009.11.11 / 團薑艙,梯惜熱 / 磁嘐 摹僭ж晦 蕨諼, 縑楝 籀葬 蹺陛 
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 梯璽⑷ / DROP婦溼 GM 貲滄橫 蹺陛  / 2009.09.16 /團薑艙 磁嘐 纔蝶お辨 蹺陛
        /*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                  // 2009.05.14 / 檜擎だ,檜鷓遽 / だじ錳 衝滌 偃摹 : 2009.11.10 羹溘 啪檜雖 螃幗 幗斜 熱薑 蛤だ檣
        /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                    // 2009.06.18 / 檜�ˋ� / и措 PK瑙 滲唳. : 2009.12.02 / 團薑艙 / pk瑙 滲唳 磁嘐 奩艙
/*  */#if defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 檜擎だ / 議衛嬴檜蠱 葬景橡#2 避擎 �� 餌塭雖雖 彊朝 幗Щ
/*  */#endif        
    #endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.9.0.1 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 901
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(5)		// ж嬪 幗瞪 廓��
	#endif	
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define _GS_MOD_NANUM_FONT_1239								// 2009.02.19 //梯惜熱 // 釭揹アお瞳辨
	/*($Delete_[x/C]$)*/#define __GS_MOD_HIDE_PET_S0032								// 2009.01.22 //梯惜熱 // 膠紫辦ж檜萄衛ゆ紫ж檜萄+模�站騣瘍芶萼棐繳秣�
	/*($Delete_[x/C]$)*/#define	_GS_ADD_HIDE_ANI_0029								// 2009.01.06 //梯惜熱 // 膠紫辦ж檜萄蝶鑒餌辨衛 賅暮蹺陛

	/*($Delete_[S/C]$)*/#define __NA_S00015_20080828_NEW_HOPE_SHADOW				// 2008.08.28 / 褻營衝, 梯惜熱, 彌艙輿 / 憎紫辦 ( 憎紫辦 婦溼 賅萇 濛機擎 檜 蛤だ檣擊 僮擊 匙. )
	/*($Delete_[S/C]$)*/#define __S00013_SHADOW_BASE_WORK							// 2008.08.11 / 譆薑煙 / 膠紫辦 漆檜蝶濛機
	/*($Delete_[S/C]$)*/#define __NA_001196_20081110_RECOVERY_ADD_OPTION			// 2008.11.10 / 褻營衝 / 葬醴幗葬 蝶鑒 熱薑.( 葬醴幗葬 疆蝶 )
	/*($Delete_[S/C]$)*/#define __NA_001206_20081112_DUPEQUIP_GROUP					// 2008.11.12 / 褻營衝 / 濰雜醞犒斜瑜 в萄 蹺陛.
	/*($Delete_[S/C]$)*/#define __NA_0S0023_20081114_SPAWN_MONSTER_QUEST			// 2008.11.14 / 褻營衝, 彌艙輿 / NPC ==> 跨蝶攪 滲褐 蠡蝶お
	/*($Delete_[S/C]$)*/#define __NA_S00028_SHADOW_ADD_WORK							// 2008.12.17 / 褻營衝 / 憎紫辦 ж檜萄, 模選鐘お煤, ぬ衛粽, 衛羹婦溼 蹺陛 掘⑷
	/*($Delete_[S/C]$)*/#define __NA_000000_20090211_SUMMON_HP_BUG_FIX				// 2009.02.11 / 褻營衝, 梯惜熱 / 憎紫辦 模�紐� HP 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_001270_20090209_GUILD_PENALTY_FIX				// 2009.02.09 / 褻營衝 / 望萄 ぬ割じ 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20090910_WOPS2610						// 2009.02.10 / 褻營衝 / ④腦詭衛闈曖 撩憮 餌辨衛 模鰍褫暮檜 餌塭雖朝 ⑷鼻 幗斜 熱薑.


	/*($Delete_[S/C]$)*/#define __NA_1165_ETHER_DEVICE_SAVE_DB						// 2008.10.26 / 梯璽⑷, 梯惜熱 / 縑纔腦 寞轎濰纂 薑爾 DB盪濰
	/*($Delete_[S/C]$)*/#define __NA_1131_PET_NAME_SETTING							// 2008.11.18 / 梯璽⑷, 梯惜熱 / ゆ 檜葷 撲薑, 檜賅じ夔
	/*($Delete_[S/C]$)*/#define __NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT			// 2008.09.23 / 梯璽⑷ / 嶸猿 檣漸曖 蝸煜紫 襬蝸煜 蛔煙ж紫煙...
	/*($Delete_[S/C]$)*/#define __NA_1234_LOTTERY_ITEM_RESULT_BUG_FIX				// 2008.12.09 / 梯璽⑷, 梯惜熱 / 犒掏 嬴檜蠱 蝗菜 詭撮雖
	/*($Delete_[S/C]$)*/#define __NA_1168_ADD_CONTENTS_ADD_LOG						// 2008.11.06 / 梯璽⑷ / 煎斜 蹺陛 濛機
	/*($Delete_[S/C]$)*/#define __NA_1233_2009_NEW_YEARS_DAY_EVENT					// 2008.12.30 / 梯璽⑷, 梯惜熱 / 2009喇 掘薑 檜漸お
	/*($Delete_[S/C]$)*/#define __NA_1260_CHANGE_LEVEL_FOR_STAT_INIT				// 2009.01.12 / 梯璽⑷, 檜鷓遽 / 鼠猿 蝶囌 蟾晦�� 溯漣 滲唳
	/*($Delete_[S/C]$)*/#define __NA_01240_GM_CMD_ADD_SERVER_TIME					// 2009.01.09 / 梯璽⑷, 檜鷓遽 / GM貲滄橫 蹺陛, 憮幗衛除, 譆渠 等嘐雖 蛔蛔	
	/*($Delete_[S/C]$)*/#define __NA_0_SHADOW_HIDE_BUG_FIX							// 2009.02.20 / 梯璽⑷, 梯惜熱 /膠紫辦 ж檜萄 幗斜 熱薑

	/*($Delete_[S/C]$)*/#define _YJ_SHADOW_S00013
	/*($Delete_[S/C]$)*/#ifdef _YJ_SHADOW_S00013
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_SKILL_S00013
		/*($Delete_[S/C]$)*/#define _GS_ADD_WEAPON_TYPE
		/*($Delete_[S/C]$)*/#define _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212 // 2008.11.25 / 彌艙輿 / 蝶鑒 螃饜 奢問(蝶鑒餌辨�� 濠翕奢問 й雖 寰й雖)婦溼 蝶觼董お в萄 蹺陛 
		/*($Delete_[S/C]$)*/#define _K2P_ADD_CREATE_SHADOW_S00018					// 2008.11.21 / 梯唳ル / 億紫辦 儅撩 婦溼
		/*($Delete_[S/C]$)*/#define _K2P_NEW_SELECT_MAP_							// 2008.11.21 / 梯唳ル / 億煎遴 寡唳 婦溼 撮た
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_CRITICAL_DAMAGE_EFFECT					//09.01.05 //彌艙輿 //膠紫辦 議葛橾 唳辦 棻艇 觼葬じ鏽 檜めお 轎溘
	/*($Delete_[S/C]$)*/#endif //_YJ_SHADOW_S00013

	/*($Delete_[x/C]$)*/#define _YJ_MOD_DEATH_ANI									//08.12.09 //彌艙輿 //錚ア顫殮縑 評塭 避朝 賅暮 殖塭雖啪 熱薑
	/*($Delete_[x/C]$)*/#define _YJ_MOD_DAMAGE_EFFECT_POS_001188					//08.12.03 //彌艙輿 //等嘐雖 檜めお 嬪纂 熱薑(顫啃 夥遴註獐腺曖 醞除高縑憮 陛蝙獄戲煎)
	/*($Delete_[x/C]$)*/#define _YJ_SHADOW_QUEST_S00023								//08.12.03 //彌艙輿 //膠紫辦 蠡蝶お
	/*($Delete_[C]$)*/#define _YJ_CHANGE_GUILD_NAME_001201						    //08.11.18 //彌艙輿 //望萄貲 滲唳 晦棟 蹺陛

    //! 2011.5.3 / i4u4me / 議葛ぜ(褻�儱﹉擬�)縑憮 顫問衛 裁溯檜 輿雖 彊啪 蹂羶
    //! 婦溼 濛機 undef л;
	//#define _YJ_MOD_BLOW_MOTION_001188						//08.12.03 //彌艙輿 //顫問馬 偃摹

	/*($Delete_[C]$)*/#define _YJ_RETIRE_EXECUTED_TRIGGER_OBJECT_001014			    //08.12.16 //彌艙輿 //お葬剪 濛翕�� 葆辦蝶 醴憮 熱薑
	/*($Delete_[C]$)*/#define _YJ_SPAWN_MONSTER_WITH_EFFECT_001250				    //08.12.22 //彌艙輿 //跨蝶攪模�紗� 檜めお 轎溘
	/*($Delete_[C]$)*/#define _YJ_PRELOADING_MONSTER								//09.01.05 //彌艙輿 //お葬剪 跨蝶攪 蝶ア衛 寰幗幔檜啪 熱薑
	/*($Delete_[C]$)*/#define _YJ_RENEWAL_KNOCKBACK_MOTION_001256					//09.01.07 //彌艙輿 //傚寥 賅暮 葬景橡
	/*($Delete_[C]$)*/#define _YJ_ETC_4_PERFORMANCE								    //09.05.22 //彌艙輿 //贗塭檜樹お 撩棟 偃摹
	/*($Delete_[C]$)*/#define _YJ_TEST_FREE_ALL_CHCHED_DATA_WHEN_MAP_LOADING		//09.05.22 //彌艙輿 //嘐暮,в萄 霞殮衛 跨蝶攪 摹煎註
	/*($Delete_[C]$)*/#define _KIKI_MODIFY_RESOURCE_MANAGER						    // 2009.06.09	/ 檜雖翱 /	ResourceManager 婦溼 煎霜 熱薑 - 餉薯渠晦葬蝶お跡煙擊 2碟縑 и廓噶 羹觼 (30碟檜鼻 霤褻腎雖 彊擎 嶸棉 薯剪)

	/*($Delete_[C]$)*/#define	_K2P_SORT_TEXT_UI_001210							// 2008.11.21 / 梯唳ル / 臢蝶お 薑溺 UI 熱薑 濛機
	/*($Delete_[C]$)*/#define _K2P_CLOSE_WINDOW_AFTER_RETIRVE_001198				// 2008.11.21 / 梯唳ル / 啪歜 頂 晦棟 偃摹[1] / 睡�偷� 濠翕戲煎 で機璽 殘晦
	/*($Delete_[C]$)*/#define _K2P_WATER_WAVE_001184								// 2008.12.17 / 梯唳ル / 熱賊 濤鼻�膩� 蹺陛
	/*($Delete_[C]$)*/#define _K2P_AFTER_JUMP_WATER_EFFECT_001182					// 2008.12.17 / 梯唳ル / 僭樓 雜雖 檜めお 熱薑

	/*($Delete_[C]$)*/#define _K2P_ITEM_DISPLAY_OPTION_001232					    // 2008.12.17 / 梯唳ル / 萄奧 塽 蝗菜 嬴檜蠱 檜葷 ル晦
	/*($Delete_[C]$)*/#ifdef _K2P_ITEM_DISPLAY_OPTION_001232
		/*($Delete_[C]$)*/#define __001232_ITEM_DISPLAY_OPTION__REFORM			    // 2008.12.13 / 檜鷓遽 / 萄奧 塽 蝗菜 嬴檜蠱 檜葷 ル晦 縑 渠и 偃摹 濛機
	/*($Delete_[C]$)*/#endif
		
	/*($Delete_[C]$)*/#define _K2P_MONSTER_IDLE_ANI_001246						    // 2009.01.12 / 梯唳ル / 議葛攪 儅撩璽 蓮夥腦隴 擁棲囀萄 滲唳

	/*($Delete_[x/C]$)*/#define __001193__20081107_LEVEL_COLOR_MODIFY__				// 2008.11.21 / 檜鷓遽 / 瞳薑 溯漣 ル衛 滲唳

	/*($Delete_[C]$)*/#define  __001199__20081117_SYSTEM_REFORM					        // 2008.11.21 / 檜鷓遽 / 啪歜 頂 晦棟 偃摹[2]
	/*($Delete_[C]$)*/#ifdef __001199__20081117_SYSTEM_REFORM
		/*($Delete_[C]$)*/#define __001199_CHAT_WHEEL_SCROLL							// 瓣た璽 �� 蝶觼煤 瞳辨
		/*($Delete_[C]$)*/#define __001199_GUILD_DUTY_COLOR							    // 望萄 雖嬪 睡罹  摹鷗 捱欽 撚 鏽楝 滲唳
		/*($Delete_[C]$)*/#define __001199_ITEM_DROP_MOUSEOVER_ALT					    // ALT 饜旋衛 葆辦蝶 а韁腎朝 嬴檜蠱貲 儀 滲唳
		/*($Delete_[C]$)*/#define __001199_DELETE_INVITE_BLOCK_REVENGE				    // 離欽, 葬漸雖 璽縑憮 だじ蟾渠 幗が 餉薯
		/*($Delete_[C]$)*/#define __001199_BATTLEZONE_LOBBY_CANCEL_BUTTON				// 寡ぎ襄 煎綠縑憮 Ready 幗が 贗葛衛 Cancel煎 滲唳
	/*($Delete_[C]$)*/#endif
	
    /*($Delete_[C]$)*/#define __001225__CHECK_INVENTORY_LOCK_USE_ITEM				    // 2008.12.01 / 檜鷓遽 / 檣漸饜葬 濡旎鼻鷓 橫箔癒 縑諼籀葬

	/*($Delete_[C]$)*/#define __001240_GM_COMMAND_INVENTORY_ITEM_LISTUP			        // 2008.01.07 / 檜鷓遽 / GM貲滄橫 - 檣漸饜葬 塽 璽堅 嬴檜蠱 跡煙 だ橾轎溘
	
	/*($Delete_[S/C]$)*/#define __NA001284_20090205_MODIFY_SUMMONED_ATTR				// 2009.02.05 / 檜熱鷗 / 憮試 模�紐� 奢衝 熱薑
	/*($Delete_[S/C]$)*/#define _NA_001231_20081216_ADD_OPTION_KIND						// 2008.12.31 / 檜熱鷗 / PVP 偃摹擊 嬪и 褫暮 蹺陛
	/*($Delete_[x/C]$)*/#define __NA003322_20090217_NOT_FIND_ENGLISHNAME_BUG			// 2009.02.17 / 檜熱鷗 / 艙僥 馨葛攪 檜葷擊 瓊雖 跤ж朝 幗斜.
	/*($Delete_[S/C]$)*/#define __NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION	// 2009.02.02 / 檜熱鷗 / 憧 嬴檜蠱 醞犒 瞳辨 幗斜.
	/*($Delete_[S/C]$)*/#define __NA000000_20081106_GUILD_NAME_CHANGE					// 2008.11.06 / 檜熱鷗, 彌艙輿 / 望萄貲 滲唳	(濛機蹂羶憮[1201])
	/*($Delete_[S/C]$)*/#define __NA_003197_20090114_CHARACTER_POS_SYNC_BUG_FIX			// 2009.01.14 / 檜熱鷗 / 蝶鑒 餌辨 褒わ衛 馨葛攪 嬪纂 翕晦��
	/*($Delete_[S/C]$)*/#define __NA_001253_20090107_MAX_CHARACTER_MODIFY				// 2009.01.07 / 檜熱鷗 / 馨葛攪 儅撩 MAX 纂 滲唳

	/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT						// 2008.10.24 / 檜熱鷗, 檜鷓遽 / 偃檣 錚檜 ん檣お
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

	/*($Delete_[S/C]$)*/#define __NA_0_20080930_AUTH_REFACTORING					// 2008.09.30 / 檜遽戮 / 檣隸 葬ね饜葭 醞縑 в蹂и 啪歜薹 模蝶 滲唳
	/*($Delete_[S/C]$)*/#define __NA_0_20081029_PARTY_REFACTORING					// 2008.10.29 / 檜遽戮, 梯唳ル / だじ 塭檜粽楝葬 AgentServer 瞳辨 塽 葬ね饜葭
	/*($Delete_[S/C]$)*/#define __NA_0_20081124_PARTY_REFACTORING_GAMESERVER		// 2008.11.24 / 檜遽戮 / だじ 塭檜粽楝葬 GameServer 瞳辨

    /*($Delete_[S/C]$)*/#define __NA001230_SSQ_UPDATE_ADD_EXP_FIELD_SUPPORT__		// 2008.12.04 / 檜擎だ / 顫塊и 餌錳 蹺陛 唳я纂 爾鼻 滌紫 蹺陛 籀葬
    /*($Delete_[S/C]$)*/#define __NA000000_090224_PARTY_RETURN_PROBLEM_CORRECTION__ // 2009.02.24 / 檜擎だ / 寞濰 煎註衛 犒敝 寰腎朝 僥薯 熱薑
    /*($Delete_[x/C]$)*/#define __NA_0_20081222_BUGFIX_INITPOS_OF_MAPOBJ_EFFECT       // 2008.12.23 / 梯團褻 / 裘 螃粽薛お 檜めお 蟾晦 嬪纂 螃盟 熱薑

	#ifdef _KOREA
		
	#elif defined(_CHINA) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / 梯唳ル / 億紫辦 儅撩 婦溼
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_JAPAN) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / 梯唳ル / 億紫辦 儅撩 婦溼
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_GLOBAL)
		#ifdef  __CN_CREATE_BLOCK_SHADOW								// 2009.07.23 / 梯惜熱 / 億紫辦 儅撩 虞晦
			#undef _K2P_ADD_CREATE_SHADOW_S00018						// 2008.11.21 / 梯唳ル / 億紫辦 儅撩 婦溼
		#endif//__CN_CREATE_BLOCK_SHADOW
	#endif

#endif

#ifdef __NA_0_20080930_AUTH_REFACTORING
    #include "./SunAuth/AuthDefine.Build.h"
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.1.8 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 818
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(4)		// ж嬪 幗瞪 廓��
	#endif	
//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA001226_INSTANCE_DUNGEON_EXTENTION__				// 2008.12.01 / 檜擎だ, 梯惜熱 / 檣蝶欐蝶 湍蟒 檜漸お 裘 寞衝 蹺陛
	/*($Delete_[S/C]$)*/#define __NA_000000_20081202_ADD_EXP_ACTION_2				// 2008.12.02 / 褻營衝 / 唳я纂 輿晦 ==> だじ衛 п渡 裘縑 氈朝 賅萇 嶸盪縑啪 唳я纂 輿晦煎 滲唳.
	/*($Delete_[S/C]$)*/#define __NA_0_20081209_ONLEVELUP_BUG_FIX					// 2008.12.09 / 檜遽戮/ 唳я纂蒂 號檜 嫡嬴紫 и疏廓縑 溯漣機檜 腎雖 彊堅 ж釭噶 腎朝 僥薯 熱薑
	#ifdef _JAPAN
		#define __090512_LTJ_ETHER_CHARGE_JP_BUGFIX				// 2009.05.12 / 檜鷓遽 / 縑纔腦 濰瞪鼻鷓縑憮 憮幗摹鷗 檜釭 в萄縑憮 餌蜂 �� 葆擊煎 敝�納� 陽 濰瞪鼻鷓 檜鼻 ⑷鼻 熱薑		
	#endif
#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.1.7 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 817
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(3)		// ж嬪 幗瞪 廓��
	#endif	
	//////////////////////////////////////////////////////////////////////////

	
	/*($Delete_[C]$)*/#define _K2P_NEW_RANKING_UI													// 2008.09.30 / 梯唳ル / 楨韁 嶸嬴檜 葬景橡
	/*($Delete_[C]$)*/#define _GS_MOD_PET_BASEOPTION_EXTEND_1146									// 2008.08.13 / 梯惜熱 / ゆ晦獄褫暮�挫暩躝�
	/*($Delete_[C]$)*/#define	_GS_BUG_FIX_PVP_VIEWPORT											// 2008.12.21 //梯惜熱 // 憮幗箔んお滲唳戲煎檣и 鼠瞳鼻鷓嶸雖幗斜

	/*($Delete_[S/C]$)*/#define	__NA_000839_20080129_PVP_OPTION										// 2008.01.29 / 褻營衝, 梯雖諫 / PVP憮幗 褫暮 晦棟 蹺陛.	  ==> 2 1,2廓擊 п諼縑憮 螢葡.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20071120_FATIGUE_AC_RANKING								// 2007.11.20 / 褻營衝, 梯惜熱 / AC爾鼻 塽 嘐暮楨韁 爾鼻 Я煎紫 瞳辨. ==> 1
	/*($Delete_[S/C]$)*/#define __NA_000000_20070704_FATIGUE_REWARD									// 2007. 07. 04 / 褻營衝, 梯唳ル / Я煎紫衛 爾鼻婦溼 
	/*($Delete_[S/C]$)*/#define __NA_000902_20080226_MESSAGE_RESTRICTION							// 2008.03.10 / 褻營衝, lammy / だじ錳 檜諼縑 詭衛雖 薯и

	/*($Delete_[S/C]$)*/#define __NA001145_20080909_AIOLT_TRIGGERS_ADD								// 2008.09.08 / 檜熱鷗, 彌艙輿 / 嬴檜螢お 詭檣嘐暮辨 お葬剪 蹺陛 
	/*($Delete_[S/C]$)*/#define __NA001173_20080924_SUMMON_NONPVP_MODE_ADD							// 2008.09.24 / 檜熱鷗, 梯惜熱 / 模�紡� NONPVP賅萄 蹺陛
    /*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM											// 2008.10.28 / 梯璽⑷, 梯惜熱 / 縣葬お 1.5離

	/*($Delete_[S/C]$)*/#define __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH			// 2008.12.10 //檜熱鷗 // GM蝶觼董お蹺陛
	/*($Delete_[S/C]$)*/#define	__NA002410_20081006_TROUBLED_PARTS_RULE_ADD							// 2008.10.06 / 檜熱鷗 / pvp 瑙 蹺陛 (碟爵雖羲縑憮 望萄 翕裊婦啗 羹觼)
	/*($Delete_[S/C]$)*/#define	__NA_001142_20080806_TRIGGER_ADD									// 2008. 8. 6 / 褻營衝, 彌艙輿 / 褐敘 お葬剪 蹺陛.
	/*($Delete_[S/C]$)*/#define __NA001139_20080922_IGNORE_RESERVEHP_STATE_ADD						// 2008.09.22 / 檜熱鷗 / 儅貲曖 釭鼠辨 HP瞳董鼠衛 鼻鷓 蹺陛
	/*($Delete_[S/C]$)*/#define __NA002494_20081006_CHANGEUP_SAINTAID_CURE_BUG_FIX					// 2008.10.06 / 檜熱鷗 / 撮檣お縑檜萄 蝶鑒 餌辨衛 鼻鷓 薯剪 寰腎湍 幗斜.   
	/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM											// 2008.09.13 / 梯璽⑷,梯惜熱 / 楣酈 縣葬お 嬴檜蠱 掘⑷ ( 縣葬お1.5離 )
	/*($Delete_[S/C]$)*/#define __BUG_2542_PET_INTIMACY_BUG											// 2008.10.14 / 梯璽⑷ / 耀塵紫 幗斜 熱薑	
	/*($Delete_[S/C]$)*/#define __BUG_2536_RECOVER_HP_BUG											// 2008.10.16 / 梯璽⑷ / HP�蛹� 幗斜
	/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG								// 2008.10.21 / 梯璽⑷ / 犒熱曖 給 餌辨衛 溯漣 薯и縑 勘葬雖 彊朝 幗斜
	/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG							// 2008.10.21 / 梯璽⑷ / 盪鱉 だじ錳 裘 煎註衛 嫦儅ж朝 幗斜
	/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG										// 2008.10.22 / 梯璽⑷ / 盪鱉擊 葆擊檜 嬴棋夠縑憮 殮濰 衛 幗斜
	/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG											// 2008.10.30 / 梯璽⑷ / 蝶Я葩蝸楝斜 餌辨衛 蛤夥檣縞綰源 �膩� 瞳辨腎朝 幗斜
	/*($Delete_[S/C]$)*/#define __CN_1298_BASESTATE_INFO_AND_ENCHANT_ADD							// 2009.02.22 / 梯璽⑷, basestate_info, enchant 薑爾 蹺陛
	/*($Delete_[S/C]$)*/#define _NA_0_20081015_EXP_LEVEL_RATIO										// 2008.10.15 / 檜遽戮 / 唳я纂奢衝 熱薑. 溯漣綠徽高 褻瞰.
	/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY						// 2008.10.24 / 檜遽戮 / 葆徹等嘐雖 婦溼 熱薑. 鼻渠寞 葆徹樓撩盪о 馬模 褫暮 奢衝 熱薑!
	/*($Delete_[S/C]$)*/#define _NA_0_20081124_ADDEXP_SENDPACKET_BUG_FIX							// 2008.11.24 / 檜遽戮 / 唳я纂 僭擒 ぬ韃 寰爾頂朝 僥薯 熱薑
	/*($Delete_[S/C]$)*/#define __NA000000_080918_GAMEFIELD_LIST_MANAGE_SYNCHRONIZATION_FIX__		// 2008.09.17 / 檜擎だ / GameField::Enter, LeaveField 葬蝶お 婦葬 翕晦�� 熱薑
    /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED       // 2008.07.04 / 梯團褻 / 萄楚堆 滲褐 鼻鷓縑憮 濰雜腎橫 氈朝 晦除薯 嬴檜蠱 晦除 虜猿衛 檜鼻 ⑷鼻 熱薑

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	/*($Delete_[S/C]$)*/#define __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__					// 2008.07.07 / 檜擎だ, 梯唳ル / [顫塊и 餌錳] 衛蝶蠱 掘⑷ - (檜瞪 衙觼煎菟擎 褻旎噶 檜匙戲煎 檜瞪衛酈紫煙 ж啊擠)
	#ifdef __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__
		/*($Delete_[S/C]$)*/#define __NA000000_080204_TASK_SSQ_REBUILDING_FOR_BASE_FRAME__			// 2008.02.04 / 檜擎だ / (Zone) 顫塊и 餌錳 濛機擊 嬪и 憮幗 晦蟾 Щ溯歜 掘蹴 濛機 (晦襄 掘褻 偃摹)
		/*($Delete_[S/C]$)*/#define __NA000000_080210_TASK_SSQ_REBUILDING_FOR_BASE_FRAME_PARTY__	// 2008.02.10 / 檜擎だ / (Party) 顫塊и 餌錳 濛機擊 嬪и 憮幗 晦蟾 Щ溯歜 掘蹴 濛機 (晦襄 掘褻 偃摹)
		/*($Delete_[S/C]$)*/#define __NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__			// 2008.02.04 / 檜擎だ, 梯雖諫 / [顫塊и 餌錳] 衛蝶蠱 掘⑷
		/*($Delete_[S/C]$)*/#define __NA001144_080828_HIGH_RESOLUTION_SECTOR_LOAD_BALANCE_POS_SYNC__// 2008.08.28 / 檜擎だ / Sync(position)婦溼 Packet Load Balancing
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SSQ_REWARD_PROCESS__						// 2008.09.04 / 檜擎だ / 爾鼻,犒掘 蛔蛔縑 婦溼и 籀葬 (渠睡碟擎 棻艇 衙觼煎縑 勘葬啊雖虜...濛機 撮睡 掘碟 跡瞳)
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST__					// 2008.09.07 / 檜擎だ / п渡 陳瞼 檜�� 濛機辨
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_STATUS__			// 2008.09.14 / 檜擎だ / 鼻鷓 薯橫 婦溼 濛機辨
		/*($Delete_[S/C]$)*/#define __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__				// 2008.09.28 / 檜擎だ / 晦顫 撮睡 濛機
		/*($Delete_[S/C]$)*/#define __NA001166_080922_TASK_SSQ_HUMANOID_AI_01__						// 2008.09.22 / 檜擎だ / Humanoid AI 掘⑷ 1離
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_COLLECT__			// 2008.09.07 / 檜擎だ / 衙觼煎 鱔м辨
		/*($Delete_[S/C]$)*/#define __NA000000_081024_PARTY_MANAGEMENT_EASIABLITY				// 2008.10.24 / 檜擎だ / だじ 婦溼 僥薯薄 嫦儅 僥薯(婦葬鼻) п唸
		/*($Delete_[S/C]$)*/#define __NA001205_081104_MONSTER_SIEGE_EVENT_IMPL__				// 2008.11.04 / 檜擎だ / 跨蝶攪 熱撩 檜漸お
		/*($Delete_[S/C]$)*/#define __NA001094_081104_LOGIC_CHANGES_FOR_0817VER__				// 2008.11.04 / 檜擎だ / 0817 幗瞪縑憮曖 僥薯薄 傳朝 蹂羶縑 渠и 煎霜 滲唳 濛機
		/*($Delete_[S/C]$)*/#define __NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__		// 2008.11.10 / 檜擎だ / 塭檜粽縑憮 か薑 嶸盪縑 ии 纔蝶お 賅萄 蹺陛(撲薑 煎註 僥薯煎 歜衛 ж萄囀註 高 蹺陛)
		/*($Delete_[S/C]$)*/#define __NA000000_081124_DEAD_AND_LEAVE_PACKET_SYNCHRONIZATION__	// 2008.11.24 / 檜擎だ, 梯唳ル / 0.8.1.7 檜�� ぬ韃 翕晦�� 牖憮 羲瞪 僥薯陛 嫦儅й 熱 氈戲貊, 斜醞 DEAD, LEAVE曖 羲瞪 僥薯 熱薑
        /*($Delete_[S/C]$)*/#define __NA000000_081223_LOGIC_CHANGES_FOR_0817VER_SYNC__  // 2008.12.23 / 檜擎だ / 0817 幗瞪 檜�� 翕晦�� 婦溼 僥薯薄菟 匐隸 塽 熱薑

		/*($Delete_[S/C]$)*/#define __NA001187_081015_SSQ_OBSERVER_MODE__							// 2008.10.15 / 檜擎だ, 梯唳ル / 顫塊и 餌錳 婦瞪賅萄 偃嫦
		/*($Delete_[S/C]$)*/#define __NA001189_081015_SSQ_TICKET_INFO_CTRL__						// 2008.10.15 / 檜擎だ, 梯唳ル / 顫塊и 餌錳 殮濰掏 殮濰衛除 ル衛
		/*($Delete_[S/C]$)*/#define __NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__				// 2008.09.22 / 檜擎だ, 梯唳ル / 跨蝶攪 濠ァ 籀葬 婦溼
		/*($Delete_[S/C]$)*/#define __NA001144_080806_HIGH_RESOLUTION_SECTOR__						// 2008.08.06 / 檜擎だ, 梯唳ル / Sector Rebuilding + High Resolution Sector蒂 鱔и 贗塭檜樹お 楷 馬模 濛機
		/*($Delete_[S/C]$)*/#define __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__		// 2008.02.15 / 檜擎だ, 梯唳ル / 顫塊и 餌錳 濛機擊 嬪и C/S 奢鱔 賅菊 濛機
        /*($Delete_[S/C]$)*/#define __NA000000_090223_VARIABLE_SIZE_GRID_SYNC_CORRECTION__      // 2009.02.23 / 檜擎だ,彌艙輿 / 陛滲觼晦 箔籀葬煎 檣п 翕晦�倍� 僥薯陛 氈朝 睡碟 熱薑
	#endif
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

	
	

//	#define _NA_0_20080612_WORLDSERVER_REFACTORING		 // 喻⑷繹/葬ね饜葭 錯萄憮幗 闡だ橾

	#ifdef _KOREA
		#define			_NA_20081121_ABUMAN_NIJOT_EVENT					// 2008. 11. 21 / 褻營衝, 梯惜熱 / 嘐暮諫猿衛 嘐錳霞ч錳檜 賅舒 偽擎 望萄錳檜賊 唳я纂 渦輿朝 檜漸お.				
	#elif defined(_CHINA) 
		/*($Delete_[S/C]$)*/#define			__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH	// 2008.12.10 //檜熱鷗 // GM蝶觼董お蹺陛
		/*($Delete_[S/C]$)*/#undef			_NA_0_20081015_EXP_LEVEL_RATIO								// (醞措薯諼) 2008.10.15 / 檜遽戮 / 唳я纂奢衝 熱薑. 溯漣綠徽高 褻瞰.
		#define			__CN_0_SSQ_CAHO_EXCEPTION									// 2008.12.22 / 梯璽⑷ / 顫塊и餌錳 蘋螃紫 殮濰
		#define			_GS_ADD_PWD_X_BTN_1245										// 2008.12.22 / 梯惜熱 / 醞措模僥濠x綠廓蹺陛				
		/*($Delete_[S/C]$)*/#define			__NA_000000_20081202_ADD_EXP_ACTION_2						// 2008.12.02 / 褻營衝 / 唳я纂 輿晦 ==> だじ衛 п渡 裘縑 氈朝 賅萇 嶸盪縑啪 唳я纂 輿晦煎 滲唳.
		#define			__CN_0_20090421_EXP_BENEFIT_UP								// 2009.04.21 / 檜熱鷗  // 醞措 爾傘蝶 唳я纂 寡徽 鼻蝓 400% -> 700% (chd
	#endif

#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.1.6 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 816
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(2)		// ж嬪 幗瞪 廓��
	#endif	
	//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA_963_LOG_LIB_VERSION_REFACTORING					// 2008.04.29 / 梯璽⑷ / 煎斜 塭檜粽楝葬 偃摹
	/*($Delete_[S/C]$)*/#define __NA_0_ITEM_STREAM_STRUCT_SEPARATION					// 2008.05.30 / 梯璽⑷ / 嬴檜蠱 蝶お葡 掘褻羹 だ橾 碟葬
	/*($Delete_[S/C]$)*/#define __NA_1127_DELETE_BUFF_ICON								// 2008.07.21 / 梯璽⑷ / 幗Щ 嬴檜夔 餉薯
    /*($Delete_[S/C]$)*/#define __BUG_FIX_CAN_NOT_GET_POINT_WHEN_DEAD					// 2008.09.04 / 梯璽⑷ / 議葛攪 餌蜂衛縑 楨韁ん檣お 橢雖 跤ж紫煙 熱薑
	

	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__			// 2008.06.17 / 檜擎だ / 撩棟 碟戮 唸婁縑 晦奩и 煽跡 睡碟 囀萄 偃摹 濛機 (4th macro)
	/*($Delete_[S/C]$)*/#define __NA001086_080616_BUFF_EVENT_UI_AND_MULTI_SUPPORT_FUNCTION__		// 2008.06.16 / 檜擎だ, 彌艙輿 / 幗Щ 檜漸お UI 偃摹 塽 犒熱 雖錳 陛棟 掘褻煎 滲唳
	/*($Delete_[S/C]$)*/#define __NA000000_080707_PLAYER_ENTER_NAK_RESULT_CONTROL__					// 2008.07.07 / 檜擎だ, 彌艙輿 / Ы溯檜橫 殮濰 褒ぬ衛 縑楝 掘碟 籀葬
	/*($Delete_[S/C]$)*/#define __NA000000_080903_REGENTIME_CALC_PROBLEM_FIX__						// 2008.09.03 / 檜擎だ / 裘罐 殮溘 熱纂諦 婦溼и 葬薇 衛除 啗骯 顫殮 螃幗Ы煎 僥薯 熱薑
	/*($Delete_[S/C]$)*/#define __NA_001128_20080717_ADD_GM_CMD							// 2008.07.17 / 檜熱鷗 / ж歜 蟾晦��, 馨葛攪 餌蜂, 蝶鑒 籐顫歜 蟾晦��, 蝶鑒溯漣 蟾晦��
	/*($Delete_[S/C]$)*/#define __NA_000921_20080618_TROUBLED_PARTS_REFACTORING			// 2008.06.18 / 檜熱鷗 / 碟爵 雖羲 掘褻滲唳
	/*($Delete_[S/C]$)*/#define __NA000000_080818_LEVEL_DIFFERENCE_NO_MASTER_BUG		// 2008.08.18 / 檜熱鷗 / だじ濰檜 橈朝 だじ衛 檣蝶湍瞪 殮濰薯и縑 勘葬雖 彊朝 幗斜. 
	/*($Delete_[S/C]$)*/#define	__NA_002260_20080821_BORADORY_BUG_FIX					// 2008.08.21 / 檜熱鷗 / 蘋螃鼻鷓陛 腎橫紫 晦襄 爾塭給檜 衛除翕寰擎 爾塭給檜陛 嶸雖腎朝 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define	__NA_0_20080905_BORADORY_BUG_FIX						// 2008.09.05 / 檜熱鷗 / 碟爵雖羲縑憮 爾塭給檜陛 腎朝 ⑷鼻.			
	/*($Delete_[S/C]$)*/#define __NA001117_SUMMON_MONSTER_ITEM							// 2008.07.10 / 譆薑煙         / 跨蝶攪模�� 議衛嬴檜蠱 褐敘蹺陛
	/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM							// 2008.07.17 / 譆薑煙, 彌艙輿 / 嬴檜蠱 蝶邐 羹檣雖 衛蝶蠱

    /*($Delete_[S/C]$)*/#define __NA_1119_CHANGE_ETHER_EXPEND_STYLE						// 2008.08.12 / 梯璽⑷, 梯惜熱 / 縑纔腦 驕�� 模綠 寞衝 欽嫦縑憮 翱嫦煎 滲��
    /*($Delete_[S/C]$)*/#define __NA_1339_ENCHANT_RENEWAL_EX							// 2009.05.12 / 梯璽⑷, 檜鷓遽 / 檣臘お 褒ぬ衛 だ惚 - 熱鷗噢 濛機檜�� 蹺陛 濛機
	/*($Delete_[S/C]$)*/#define __NA_001013_20080428_ENCHANT_RENEWAL					// 2008.04.28 / 檜熱鷗, 梯團褻 / 檣疆お 衛蝶蠱 葬景橡 
	/*($Delete_[S/C]$)*/#define __NA_001092_20080717_ADD_SKILL_ITEM						// 2008.07.17 / 檜熱鷗, 瞪爾�� / ァ避, 粒陛瑞, �飯�, ァ 嬴檜蠱

	#define _YJ_PK_OPTION_001120								//08.08.05 //彌艙輿 //PK 鬼薯奢問 褫暮 籀葬
	/*($Delete_[C]$)*/#define _YJ_STAT_CHANGE_SYSTEM_001124						        //08.07.28 //彌艙輿 //蝶囌 羹檣雖 衛蝶蠱
	/*($Delete_[C]$)*/#define _YJ_ADD_DOF_RANGE_001101							        //08.07.09 //彌艙輿 //縛霞 瞳辨 в蹂
	/*($Delete_[C]$)*/#define _YJ_TEXT_SHADOW_EFFECT_001905						//08.07.01 //彌艙輿 //臢蝶お 擠艙�膩� 蹺陛
	/*($Delete_[C]$)*/#define _YJ_ADD_BUFF_EVENT_UI_001086						//08.06,30 //彌艙輿 //幗Щ檜漸お ui 蹺陛

	/*($Delete_[S/C]$)*/#define __NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN	//            / 譆薑煙, 梯惜熱 / 議葛攪 摹鷗璽縑憮 ゆ 檜嘐雖 轎溘
	/*($Delete_[S/C]$)*/#define __SUMMONED_SPEED_CHANGE									// 2008.09.01 / 譆薑煙, 梯惜熱 / 憮跨 跨蝶攪 蝶Я萄 滲唳

	/*($Delete_[S/C]$)*/#define	__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	// 2007.11.23 / 褻營衝 / 億煎遴 蝶觼董お だ諒 衛蝶蠱.
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[C]$)*/#define _GS_MOD_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[S/C]$)*/#define _YJ_NEW_SCRIPT_PARSING_SYSTEM
		/*($Delete_[S/C]$)*/#define _KIKI_20080711_NEW_SCRIPT

		/*($Delete_[S/C]$)*/#define __NEW_SCRIPT_SYSTEM_BY_AEORAGIE_PART // 2008.07.14 / 譆薑煙 / 褐敘蝶鑒だ憮
		/*($Delete_[S/C]$)*/#define __NA_000000_20080714_NEW_SCRIPT_PARSING_SYSTEM_APPLY    // 
		/*($Delete_[C]$)*/#define __NA_0_20080716_PARSER_RENEWAL_BY_MJKIM
	/*($Delete_[S/C]$)*/#endif //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	

	/*($Delete_[S/C]$)*/#define __NA_001093_20080623_SERVER_DISCONNECT_			    // 2008. 06. 23 / 褻營衝 / 憮幗除曖 翱唸檜 莒橫 螳擊 陽 籀葬.
	/*($Delete_[S/C]$)*/#define	__000000_20080811_BUG_FIX_VIEW_JOIN_TOP_TIME		// 2008. 8. 11 / 褻營衝, 梯璽⑷ / '盪輿嫡擎 鱉曖 橾奩 翮潸縑 衛除 ル衛' ==> null 羹觼 幗斜 熱薑.

	/*($Delete_[C]$)*/#define _GS_ADD_TARGETING_EFFECT_1125							// 2008.08.13 / 梯惜熱 / 跨蝶攪 顫啃た檜めお

	/*($Delete_[C]$)*/#define _GS_ADD_SINGLEMODE_MONSTER_1016							// 2008.08.13 / 梯惜熱 / 諒旋賅萄衛 跨蝶攪檜翕塽蝶鑒爾晦

	/*($Delete_[S/C]$)*/#define __NA_0_20080717_DBUNIT_ADD_SERVERTYPE						// 2008.07.17 / 檜遽戮 / DBUnit縑 m_ServerType 詹幗滲熱 蹺陛
	/*($Delete_[S/C]$)*/#define __NA_0_20080825_MODIFY_OPTION_DEFENCE_POWER_CALC			// 2008.08.25 / 檜遽戮 / 褫暮 奢問溘, 寞橫溘 綠徽 瞳辨 奢衝 滲唳


    #define __NA_001062_20080623_STRENGTH_PENALTY					// 2008.06.23 / 檜�ˋ�, 梯惜熱 / 頂掘紫 馬模縑 評艇 ぬ割じ 睡罹. 

	//措陛幗瞪滌
	#ifdef _CHINA
		//#undef __NA_001062_20080623_STRENGTH_PENALTY				//醞措擎 頂掘紫 薯剪
	#elif defined(_JAPAN)
		#undef _YJ_PK_OPTION_001120									//橾獄擎 PK鬼薯褫暮 薯剪
	#endif

#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.1.5 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 815
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(1)		// ж嬪 幗瞪 廓��
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define __NA_001044_ADD_CUSTOM_UI								// 2008.05.28 / lammy / 馨葛攪 儅撩衛 嘐葬 殮橫爾晦 晦棟 蹺陛	

	/*($Delete_[C]$)*/#define _K2P_CHARACTER_ROTATE_000986							// 2008.05.28 / lammy / 馨葛攪 儅撩衛 �蛻� 晦棟 蹺陛
	/*($Delete_[C]$)*/#define _K2P_ADD_INVITE_BUTTON_001018							// 2008.05.28 / lammy / 蟾渠 幗が 蹺陛
    /*($Delete_[C]$)*/#define _GS_MOD_CHATMSG_MAINTAIN_1058							//[1058] 2008.05.30 / GS / 裘檜翕衛 瓣た頂辨 嶸雖 
    /*($Delete_[C]$)*/#define __NA_001003_PC_MONSTER									// 2008.05.07 / kjws78 / pc⑽ 跨蝶攪 蹺陛
    /*($Delete_[C]$)*/#define __NA_0_20080507_CHANGUP_PC_SKLLL_ADD					// 2008.05.07	/ 瞪爾�� / 羹檣雖機縑 評艇 PC 蝶鑒 蹺陛
    /*($Delete_[C]$)*/#define __NA_0_2008_06_03_PLAYER_INVALID_EQUIPMENT_CHECK		// 2008.06.04	/ 瞪爾�� / 濠褐婁 棻艇 贗溯蝶曖 濰綠蒂 雜辨衛 晦獄 寞橫掘煎 撢たл
    /*($Delete_[C]$)*/#define __NA_1061_20080609_SHOW_MY_ID_RENDERINFO							//08.06.09   / 瞪爾�� / 頂 嬴檜蛤 爾檜晦 晦棟蹺陛
    /*($Delete_[C]$)*/#define _YJ_MOD_QUEST_NPC_NAME_COLOR_1938									// 08.06.18 / 彌艙輿 / 蠡蝶お NPC 啻歜 鏽楝 滲唳
    
    


    /*($Delete_[S/C]$)*/#define __NA_000962_ADD_GUILD_ERROR								// 2008.05.28 / 喻⑷繹, 梯唳ル / 偽擎 望萄 檜葷戲煎 儅撩 縑楝高 蹺陛 
	/*($Delete_[S/C]$)*/#define	__NA_001012_20080506_RESERVE_HP_SYSTEM					// 2008.05.06 / 褻營衝, 梯團褻 / 晦獄 HP衛蝶蠱擊 瞳董�風� 滲唳ж罹 曹濰馬 塽 僭擒曖 �蕞趧瑰� 堪檣棻.
    /*($Delete_[S/C]$)*/#define _BUG_FIX_LOTTERY_SYSTEM									// 2008.09.11 / 褻營衝, 梯惜熱 / 犒掏 衛蝶蠱 犒掏 醞犒腎朝 幗斜 熱薑.
    /*($Delete_[S/C]$)*/#define __NA_000946_20080415_LOTTERY_SYSTEM						// 2008.05.28 / 譆薑煙, 梯惜熱         / 犒掏衛蝶蠱					[NO:0946]
    /*($Delete_[S/C]$)*/#define __NA_001068_ACTION_DEAD_PLAYER							// 2008.06.19 / 譆薑煙, 梯唳ル         / 餌蜂и Ы溯檜橫 贗葛陛棟	[NO:1068] 
    /*($Delete_[S/C]$)*/#define __NA_000994_CHANGUP_NEW_SKILL							// 2008.05.28 / 譆薑煙, 瞪爾��         / 褐敘蝶鑒					[NO:0994]

    /*($Delete_[S/C]$)*/#ifdef __NA_000994_CHANGUP_NEW_SKILL                                // 
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS							// 鼠瞳, 葆徹塽 僭葬 等嘐雖 賊羲, 奢問陛棟
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS_BUGFIX					// 鼻鷓檜鼻縑 勘葬湍 幗斜 熱薑
        /*($Delete_[S/C]$)*/#define CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS					// 鬼��, 蝶鑒 等嘐雖 300%鼻蝓
        /*($Delete_[S/C]$)*/#define CHANGUP_SUMMONMASTERY_SKILL								// 憮試 葆蝶攪葬
        /*($Delete_[S/C]$)*/#define CHANGUP_IMPOSSIBLE_ATTCAK_STATUS						// 奢問碳陛, 奢問й 熱 橈棻.
        /*($Delete_[S/C]$)*/#define CHANGUP_PURGE_SKILL										// 渠鼻擊 楠渾и 雖羲戲煎 蹺寞и棻.
        /*($Delete_[S/C]$)*/#define CHANGEUP_SAINTAID										// 鼻鷓檜鼻 п薯
        /*($Delete_[S/C]$)*/#define CHANGUP_RECKLESS_STATUS									// ァ輿, 模�紡鶺� 等嘐雖 渠ァ 鼻蝓
    /*($Delete_[S/C]$)*/#endif


	/*($Delete_[S/C]$)*/#define	__NA_000000_20080522_DROPRATIO_BUGFIX					// 2008.05.22 / 褻營衝 / 嬴檜蠱 Drop 曖 濠猿⑽檜 WORD⑽檜罹憮 儅晦朝 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define	__NA_001042_NEW_CHARACTER_ITEM							// 2008.08.26 / 褻營衝 / 褐敘 議葛攪 儅撩衛 嗽猿煎 議衛嬴檜蠱擊 雖晝.


    // (WARNING) ж晦 working macro朝 嶸辨и 寞衝擊 薯奢й 熱 氈戲嘎煎 薑葬й 蕨薑. 雖辦雖 蜓 匙.
	/*($Delete_[S/C]$)*/#define	__NA_001070_20080610_DBPROXY_LOG						// 2008.06.09 / 褻營衝 / DBProxy煎斜 薑葬


    // 雖辦雖 蜓 匙////////////////////////////////
    #define _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// 2008.05.28 / lammy / 馨葛攪 摹鷗璽縑 憮幗 檜翕幗が 蹺陛
    #define __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT  // (醞措 薯諼 鐘蘸隴) 2008.04.28 / 梯璽⑷ / 葬嘐じ萄 嬴檜蠱 熱葬 �蝦� 熱薑 (鼠薯и戲煎)
    /////////////////////////////////////////////


	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080616_HP_ZONEMOVE						// 2008.06.16 / 褻營衝 / 襄 檜翕衛 HP 幗斜( HP�挫� 嬴檜蠱擊 陛雖堅 氈橫紫 檜翕�� 瞳辨寰腎朝 幗斜 )
	/*($Delete_[S/C]$)*/#define	__NA_001081_20080612_TUTORIAL_CHANGE					// 2008.06.12 / 褻營衝, 梯惜熱 / ぅ饜葬橡 殮濰寞衝 滲唳.
	/*($Delete_[S/C]$)*/#define __NA_VENDOR_BUGFIX_										// 2008.09.23 / 褻營衝 / 掖�� 割 ん檣お 霤褻 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define _BUG_FIX_20081007_WOPS2463_JUMP_DAMAGE					// 2008.10.07 / 褻營衝 /薄Щ ц擊陽 等嘐雖 還雖 彊朝剪籀歲 爾檜朝 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_001039_20080509_GM_COMMAND_MODIFY					// 2008.05.09 / 檜熱鷗, 梯惜熱 / GM 貲滄橫 熱薑, 蹺陛	
	/*($Delete_[S/C]$)*/#define __NA_0_20080509_GM_COMMAND_MODIFY_DEV					// 2008.05.09 / 檜熱鷗 / 偃嫦 纔蝶お辨 GM貲滄橫 盪濰, 褒ч, gm蝶觼董お褒ч
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_HAMMER_OF_ALCHEMIST_MODIFY			// 2008.05.17 / 檜熱鷗 / 貲奢曖 蜂纂 晦棟 滲唳
	/*($Delete_[S/C]$)*/#define __NA_1921_20080610_BUG_FIX_HAMMER_OF_ALCHEMIST_MODIFY	// 2008.06.10 / 檜熱鷗 / 貲奢曖 蜂纂 熱葬衛 ⑷營 頂掘紫 == 譆渠 頂掘紫 橾衛 熱葬 寰腎朝 幗斜 
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_WORLD_SHOUT_CONSUME_MODIFY			// 2008.05.17 / 檜熱鷗 / п寞捱曖 ん堅僥 晦棟 滲唳
	/*($Delete_[S/C]$)*/#define __NA_0_20080603_REMOVE_QUESTSCRIPT_ASSERT				// 2007.06.03 / 檜熱鷗 / 蠡蝶お だ憮縑憮 load л熱曖 assert, Validating л熱煎 貍頂晦
	/*($Delete_[S/C]$)*/#define __NA_001892_20080618_WORLD_SHOUT_CONSUME_MODIFY_BUGFIX  // 2008.06.18 / 檜熱鷗 / 錯萄 諼纂晦 pc寞縑憮 寰腎朝 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY			// 2008.06.10 / 檜熱鷗, 梯雖諫 / 雖羲滌(碟爵雖羲蛔) 奢問撩щ(橾奩.碟爵.PK蛔) 羹觼 滲唳
	/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / 檜熱鷗 / CHECK_ATTACK_PROPENSITY_MODYFY 濛機衛 緒霞 割羹觼 蹺陛.
	/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX						// 2008.06.26 / 檜熱鷗 / 唳啗雖羲縑憮 幗Щ蝶鑒 衛瞪衛 爾塭給檜 腎湍 幗斜.
	/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX					// 2008.06.30 / 檜熱鷗 / Щ葬蘋螃 嶸盪曖 蠡蝶お 熱塊 陛棟ж啪(蝶觼董お薯橫).[蝶觼董お 滲唳衛 翕衛 蹺陛 腎橫撿л]
	/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY						// 2008.07.01 / 檜熱鷗 / 蘋螃嶸盪 濠翕戲煎 鬼薯奢問腎雖 彊朝 幗斜.
	/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK						// 2008.07.24 / 檜熱鷗 / 顫橾羹觼衛 詹幗л熱頂 蝶鷓す滲熱 餌辨 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_001074_ETHER_EMISSION_DEVICE_EXPAND				//            / 譆薑煙         / 縑纔腦 嬴檜蠱 褫暮
	
	/*($Delete_[S/C]$)*/#define __NA_1047_EXP_POTION												// 2008.06.02 / 梯璽⑷ / 唳я纂 僭擒
	/*($Delete_[S/C]$)*/#define __NA_1075_FIELD_BATTLE_SERVER_LOG_REGULATION						// 2008.06.09 / 梯璽⑷ / в萄 寡ぎ 憮幗 煎斜 薑葬
	/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__							// 2008.04.02 / 檜擎だ, 梯雖諫 / Skill婦溼 ぬ韃 籀葬 籀葬 偃摹擊 嬪и 晦蟾濛機 Version01
	/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__								// 2008.04.21 / 檜擎だ, 梯雖諫 / 橾奩 憶蹴 憲堅葬闈 瞳辨擊 嬪и ぬ韃 碟戮 塽 瞳辨
	/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__								// 2008.06.19 / 檜擎だ / PacketControl 3rd Server<->Server (か薑 ぬ韃縑 渠п憮虜 濛機)
	/*($Delete_[S/C]$)*/#define __NA000000_080228_TARGET_FIND_CORRUPT_FLOW_FIX__					// 2008.02.28 / 檜擎だ / GameField::SearchTarget 煎霜, 歜衛 偌羹曖 ん檣攪蒂 檜辨и 嬪я繭虜 囀萄 熱薑 - 棻艇 翱婦脹 僥薯陛 嫦儅ж朝 雖曖 罹睡 だ學 в蹂
	/*($Delete_[S/C]$)*/#define __NA000000_GRID_REBUIDING_FOR_READABLE_CODE__						// 2008.04.24 / 檜擎だ / Player_Grid 陛絮撩 堪檜晦 濛機
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_NPC_KILL_OPTIMIZER__		// 2008.02.19 / 檜擎だ / [844,896] NPC Kill Event籀葬衛 虞渠и 瑞Щ 薯剪辨, 檜漸お陛 й渡脹 跨蝶攪虜 籀葬й 熱 氈朝 掘褻 _掘 100欽 蠡蝶お 晦棟縑 瞳辨腎橫 氈湍 頂辨 檜瞪 塽 蹺陛 濛機
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_INSTANCE_ZONE__			// 2008.02.19 / 檜擎だ / [844,896] InstanceDungeon 碟й 籀葬 _掘 100欽 蠡蝶お 晦棟縑 瞳辨腎橫 氈湍 頂辨 檜瞪 塽 滲唳 濛機
	/*($Delete_[S/C]$)*/#define __NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__					// 2008.05.27 / 檜擎だ, 梯雖諫 / Ы溯檜橫 擁棲詭檜暮 籀葬衛 嬪纂翕晦�� 蜃雖 彊朝 僥薯 п唸擊 嬪и 濛機
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO03__			// 2008.06.XX / 檜擎だ / 撩棟 碟戮 唸婁縑 晦奩и 煽跡 睡碟 囀萄 偃摹 濛機
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_SOLARHASH_PROBLEM_CORRECTION_V01__				// 2008.06.19 / 檜擎だ / SolarHashtable 鼠唸撩 匐餌(DEBUG) & 婦溼 僥薯 熱薑
	/*($Delete_[S/C]$)*/#define __NA000000_080626_TUTORIAL_MISSION_LEAVE_CASE_CORRECTION__			// 2008.06.26 / 檜擎だ / Agent縑 襄營ж朝 ぅ饜葬橡 鼻鷓陛 嘐餌辨(樹薯睡攪...?)腎賊憮 窒檜朝 僥薯 熱薑
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL__			// 2008.06.09 / 檜擎だ / 橫箔癒 嶸盪 寞雖疇_20080609 : 薑鼻,綠薑鼻 瞳檣 鼻�窒� 渠и 鼻撮 碟戮 塽 в蹂и 唳辦 熱薑 塽 偃摹
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__		// 2008.07.01 / 檜擎だ / HackShield 婦溼 蛤臢暮 褐煆紫 堪檜晦 濛機 (寰薑�� 腎賊 檜瞪 衙觼煎諦 м艦 匙)
	/*($Delete_[S/C]$)*/#define __NA000000_081218_GAMEFIELD_LEAVE_FIELD_USAGE_CORRECTION__			// 2008.12.18 / 檜擎だ / GameField::LeaveField衛 褒 偌羹 п薯 寰ж朝 僥薯 熱薑(詭賅葬 援熱)
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_REFACTORING						// 2008.02.19 / 檜遽戮 / 陛鼻 贗塭檜樹お 寞雖疇. 橫箔癒 嶸盪 籀葬 葬ね饜葭
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_POWERFUL						// 2008.02.19 / 檜遽戮 / 陛鼻 贗塭檜樹お 寞雖疇. 橫箔癒 嶸盪 籀葬 蹺陛餌о
	/*($Delete_[S/C]$)*/#define __NA_0_20080430_SKILL_REFACTORING									// 2008.04.30 / 檜遽戮 / 蝶鑒 衙棲盪 葬ね饜葭
	/*($Delete_[S/C]$)*/#define __NA_891_20080602_CHANGE_EXP_RATIO2									// 2008.05.02 / 檜遽戮 / 橾獄 唳я纂 寡徽 褻薑 奢衝擊 и措縑紫 瞳辨
	/*($Delete_[S/C]$)*/#define __NA_28934_20080609_CANBE_ATTACKED_CHECKMISS_BUG_FIX				// 2008.06.09 / 檜遽戮 / 蝶鑒 檜めお 籀葬縑憮 CanbeAttacked() 羹觼 蹺陛. 鬼繹鼻鷓翕寰 等嘐雖 菟橫螃朝 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT					// 2008.06.13 / 檜遽戮 / 箔んお 撩棟偃摹
	/*($Delete_[S/C]$)*/#define __NA_0_20080616_AGENT_CHARSELECT_INIT_BUG_FIX						// 2008.06.16 / 檜遽戮 / 議葛攪 摹鷗�飛橉虞� 給嬴螢 陽 蟾晦�� 寰脹剪 熱薑. RemoveUserForCharGuid 煎斜 寰陴啪 熱薑.
	/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX						// 2008.06.17 / 檜遽戮 / 贗楚蝶滌 蝶顫た ん檣お 晦棟 薯剪. 蕨瞪縑 晦�僱Ц�雖虜 ⑷營 噙檜雖 彊擠.
	/*($Delete_[S/C]$)*/#define __NA_0_20080619_GET_VALID_SPECIAL_AREA								// 2008.06.19 / 檜遽戮 / WzSpecialArea 掘й 陽 檜翕陛棟и 嶸�褲� 顫橾擊 摹鷗ж朝 л熱 蹺陛
	/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD								// 2008.06.09 / 喻⑷繹,彌艙輿 /望萄婦溼 葬瞰お 囀萄 蹺陛
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER					// 2008.5.30 / 喻⑷繹 / 錯萄 憮幗 陛鼻 贗塭檜樹お 掘滌ぬ韃
	/*($Delete_[S/C]$)*/#define		__NA_BUG_181415_20080820_GUILD_BUG_FIX							// 2008.8.20 / 褻營衝 / 葆蝶攪陛 望萄錳擊 蕨綠煽戲煎 滲唳й 熱 氈朝 幗斜 熱薑.

	/*($Delete_[S/C]$)*/#define __NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__					// 2008.03.24 / 檜擎だ, 瞪爾�� / 100溯漣 羹檣雖機
	/*($Delete_[S/C]$)*/#define __NA001048_080514_APPEND_ABILITY_KNOCKBACK2__						// 2008.05.14 / 檜擎だ, 瞪爾�� / 傚寥2 橫網葬じ 蹺陛 (擎だ:Ability婦溼 Flow 匐隸 塽 螃盟 匐轎紫 售й 蕨薑)
	/*($Delete_[S/C]$)*/#define __NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__		// 2008.05.26 / 檜擎だ, 彌艙輿 / [檜漸お]2輿喇 檜漸お 檜めお 轎溘 濛機
	/*($Delete_[S/C]$)*/#define __NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__		// 2008.06.18 / 檜擎だ, 梯惜熱 / Cash Shop 曖 摹僭ж晦 晦棟 碳陛棟 ⑽鷓 雖錳
	/*($Delete_[S/C]$)*/#define __NA_001053_20080522_ATTR_RATIO_TYPE								// 2008.05.22 / 檜遽戮, 梯團褻 / 綠徽顫殮(5) 褫暮 薑葬
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_GUILD_WAREHOUSE_LIMIT_ITEM_LOG						// 2008.5.30 / 喻⑷繹, 彌艙輿 / 望萄 璽堅 煎斜 橾奩 嬴檜蠱婁 葬嘐じ萄 嬴檜蠱 掘滌


	#ifdef _CHINA
		#define _GS_ADD_QUEST_SCROLL_1143										// 08.09.06 / 梯惜熱 / 醞措NPC蠡蝶お璽 蝶觼煤夥蹺陛
        //-------------------------------------------------------
        #undef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT   // 2008.04.28 / 梯璽⑷ / 葬嘐じ萄 嬴檜蠱 熱葬 �蝦� 熱薑 (鼠薯и戲煎)
        //-------------------------------------------------------
		/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG							// 2008.10.21 / 梯璽⑷ / 犒熱曖 給 餌辨衛 溯漣 薯и縑 勘葬雖 彊朝 幗斜
		/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG						// 2008.10.21 / 梯璽⑷ / 盪鱉 だじ錳 裘 煎註衛 嫦儅ж朝 幗斜
		/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG									// 2008.10.22 / 梯璽⑷ / 盪鱉擊 葆擊檜 嬴棋夠縑憮 殮濰 衛 幗斜		
		/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM										// 2008.09.13 / 梯璽⑷,梯惜熱 / 楣酈 縣葬お 嬴檜蠱 掘⑷ ( 縣葬お1.5離 )		

		/*($Delete_[S/C]$)*/#define	__NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT						// 2008.09.23 / 梯璽⑷ / 嶸猿 檣漸曖 蝸煜紫 襬蝸煜 蛔煙ж紫煙...
		#define __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP							// 2008.09.23 / 梯璽⑷,梯惜熱 / 100溯漣 羹檣雖機 й陽 蝶囌 蟾晦��		
		#define _GS_ADD_PWD_X_BTN_1143                                          // 08.08.13 / 梯惜熱 / 醞措璽堅綠廓縑x幗が蹺陛

		/*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM										// 2008.10.28 / 梯璽⑷, 梯惜熱 / 縣葬お 1.5離
		/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG										// 2008.10.30 / 梯璽⑷ / 蝶Я葩蝸楝斜 餌辨衛 蛤夥檣縞綰源 �膩� 瞳辨腎朝 幗斜
        // (NOTE) (100130) 橾瞪縑 晦�嘀� 僥曖衛 賅萇 蝶觼董お陛 鱔橾脹 鼻鷓艘棻. (高虜 ぎ葬堅...) 鱔橾衛酈紫煙 ж濠.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / 梯璽⑷,梯惜熱 / ゆ 褫暮 Ы溯斜煎 摹鷗 瞳辨(蝶觼董お縑憮...)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / 譆薑煙, 彌艙輿 / 嬴檜蠱 蝶邐 羹檣雖 衛蝶蠱
		//!罹晦梱雖 塭檜粽		
		/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY					// 2008.10.24 / 檜遽戮 / 葆徹等嘐雖 婦溼 熱薑. 鼻渠寞 葆徹樓撩盪о 馬模 褫暮 奢衝 熱薑!

	#elif defined(_KOREA) 

	#elif defined(_JAPAN)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / 譆薑煙, 彌艙輿 / 嬴檜蠱 蝶邐 羹檣雖 衛蝶蠱
		#define _K2P_ADD_WISPER_WINDOW
		#define _K2P_USE_CHATWINDOW_USING_VENDORLIST
		#define _K2P_HERO_POSITION
		/*($Delete_[C]$*/#define _K2P_CHECK_USE_ITEM_BY_POPUP
        // (NOTE) (100130) 橾瞪縑 晦�嘀� 僥曖衛 賅萇 蝶觼董お陛 鱔橾脹 鼻鷓艘棻. (高虜 ぎ葬堅...) 鱔橾衛酈紫煙 ж濠.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / 梯璽⑷ / ゆ 褫暮 Ы溯斜煎 摹鷗 瞳辨(蝶觼董お縑憮...)
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW								// 2008.03.28 / 檜遽戮 / 寞葬蝶お 褻勒羹觼橈檜 賅舒 爾罹輿晦. 橾欽 措頂虜 瞳辨.
		//#define __NETWORK_FIX_20081119													
	#endif

#endif
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.1.4 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 814

	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 鼻嬪 幗瞪 廓��
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(2)		// 醞除 幗瞪 廓��
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(7)		// ж嬪 幗瞪 廓��
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[C]$)*/#define _GS_MOD_PERSONAL_VENDER_00877								// 2008.03.24 / 梯惜熱 / 偃檣鼻薄啪衛っ 蝶觼煤夥薯剪
    /*($Delete_[C]$)*/#define _GS_MOD_ITEM_NAMMING_COLOR_000882							// 2008.03.24 / 梯惜熱 / 嬴檜蠱啻檜墅儀 & 葬嘐じ萄 萄奧衛儀熱薑
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_SCROLL2											// 2008.04.02 / 梯惜熱 / 楨韁&嘐暮婦葬濠,蠡蝶お璽,褻м璽 蝶觼煤熱薑
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_EVENTRECT_928									// 2008.04.10 / 梯惜熱 / 嘐暮 help,衛蝶蠱瓣た璽 Я韁 艙羲熱薑
    /*($Delete_[C]$)*/#define _GS_BUGFIX_GUILD_NOTICE_1470								// 2008.04.23 / 梯惜熱 / 檣攪む檜蝶縛霞 縑蛤お鐘お煤 還薯и蹺陛
    /*($Delete_[C]$)*/#define __NA_000966_20080312_ADD_PARAM_FOR_ADDITIONAL_SOUND			// 2008.03.24 / 梯團褻 / 嬴檜蠱 餌遴萄 蹺陛縑 評艇 蝶觼董お 滲唳 塽 だ塭嘐攪 蹺陛
    /*($Delete_[C]$)*/#define __NA_0_20080318_PROCESS_AFTER_RENDERING						// 2008.03.24 / 梯團褻 / 溶渦葭 �� 籀葬 л熱 蹺陛
    /*($Delete_[C]$)*/#define __KR_000978_20080321_CHANGE_HUNTING_JOIN_LEVEL_OFFSET		// 2008.03.24 / 梯團褻 / 唳爵 ьた 殮濰 溯漣 薯и 滲唳
    /*($Delete_[C]$)*/#define __NA_0_20080311_BUGFIX_TARGET_INFO_FOR_TREE_OF_LIFE			// 2008.03.11 / 梯團褻 / 儅貲, 薑褐曖 釭鼠 顫啃 薑爾 幗斜(%d煎 ル衛) 熱薑

    /*($Delete_[x/C]$)*/#define __NA_676_PK_SYSTEM_IMPROVEMENT								// 2008.01.29 / 梯璽⑷, 梯雖諫 / PK 瑙 偃摹

    /*($Delete_[S/C]$)*/#define __MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM						// 2007.12.29 / 瞪爾��,梯璽⑷ / 嬴檜蠱 唸薑�� 譆渠 偎熱 熱薑.    
    /*($Delete_[S/C]$)*/#define __NA_000968_ETHER_EMISSION_DEVICE							// 2008.03.14 / 梯璽⑷, 瞪爾�� / 縑纔腦 寞轎濰纂
    /*($Delete_[S/C]$)*/#define __NA_780_PREVENTION_PARTY_RETURN_ABUSE						// 2008.02.05 / 梯璽⑷, 梯惜熱 / 營蕾樓 晦棟 學辨 寞雖
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SYSTEM											// 2008.02.14 / 梯璽⑷, 梯惜熱 / ゆ 衛蝶蠱 - 嫦溶顫檣等檜 辦選ж棻    
    /*($Delete_[S/C]$)*/#define __NA_1006_POINT_RANKING_REGISTER_LV							// 2008.04.17 / 梯璽⑷, 梯惜熱 / ん檣お 楨韁 蛔煙 溯漣 蹺陛	

    /*($Delete_[S/C]$)*/#define __NA_0_CHARGE_ITEM_SELL_CHECK_REMOVE						// 2008.03.11 / 梯璽⑷ / 嶸猿 嬴檜蠱 つ晦 晦棟 羹觼 薯剪( 蝶觼董お縑憮 薯橫и港棲棻. )
    /*($Delete_[S/C]$)*/#define __NA_825_HOLDDROP2											// 2008.01.21 / 梯璽⑷ / 謙樓曖 堅葬2
    /*($Delete_[S/C]$)*/#define	__NA_0_BUG_FIX_POINT_RANKING_PERIOD							// 2008.01.25 / 梯璽⑷ / ん檣お 楨韁 輿晦 婦溼 幗斜
    /*($Delete_[S/C]$)*/#define __NA_790_MODIFY_BASE_PLAYERINFO_VERSION						// 2007.11.30 / 梯璽⑷ / Base_PlayerInfo 煎斜 婦溼 幗瞪 熱薑
    /*($Delete_[S/C]$)*/#define __NA_977_PREVENTION_PARTY_RETURN_ABUSE						// 2008.03.26 / 梯璽⑷ / 營蕾樓 晦棟 學辨 寞雖 蹺陛 濛機
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SAVE_TO_DB										// 2008.02.21 / 梯璽⑷ / ゆ 模�� 衛 DB縑 盪濰
    /*($Delete_[S/C]$)*/#define __NA_0_PET_FEED												// 2008.03.06 / 梯璽⑷ / ゆ 詳檜
    /*($Delete_[S/C]$)*/#define __NA_0_PET_TEST												// 2008.03.06 / 梯璽⑷ / 纔蝶お 囀萄 褻輿歜椒 濛機 �� 餉薯
    /*($Delete_[S/C]$)*/#define __NA_821_STAT_BUG_RECOVERY									// 2007.12.20 / 梯璽⑷ / 蝶邐 幗斜 犒掘
    /*($Delete_[S/C]$)*/#define __NA_989_ABOLITION_LIMIT_LV_IN_CURSE_TOWER					// 2008.03.31 / 梯璽⑷ / 盪輿曖鱉 溯漣 薯и 繹わ
    /*($Delete_[S/C]$)*/#define __NA_1443_BUG_FIX_HOLDDROP									// 2008.04.07 / 梯璽⑷ / 嬴檜蠱 橈橫紫 謙樓堅葬 模賅腎朝 幗斜 熱薑
    /*($Delete_[S/C]$)*/#define __NA_0_SOCKET_OPTION_BUG_RECOVERY							// 2008.05.08 / 梯璽⑷ / 模鰍 褫暮 幗斜 犒掘
    /*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_SKILL_ALL_STAT_UP							// 2008.05.15 / 梯璽⑷ / 蝶鑒煎 螢蝶邐 褫暮 瞳辨衛 遘朝僥薯 天天;
    /*($Delete_[S/C]$)*/#define __NA_1017_LUCKY_ITEM_IDENTIFY_LOG							// 2008.04.23 / 梯璽⑷ / 楣酈 嬴檜蠱 檣隸 衛 煎斜 陴梯
    /*($Delete_[S/C]$)*/#define	__NA_0_PET_TIMER_PERIOD_CHANGE								// 2008.05.26 / 梯璽⑷ / ゆ 顫檜該 輿晦 滲唳
    /*($Delete_[S/C]$)*/#define __NA_1090_NO_TRADE_WHILE_PET_SUMMONING						// 2008.06.18 / 梯璽⑷ / 模�� 醞 剪楚 塽 偃檣 鼻薄 っ衙 旎雖


	/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_SYSTEM  //2離 望萄衛蝶蠱
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_2ND_GUILD_SYSTEM                                 // 
		/*($Delete_[S/C]$)*/#define __NA_000596_20070821_GUILD_CLASS_SYSTEM					// [596] 2007.08.21 / 褻營衝 , 瞪爾�� / 望萄 啗晝 衛蝶蠱
		/*($Delete_[S/C]$)*/#define __NA_000597_20070821_GUILD_CORPS_SYSTEM					// [596] 2007.08.21 / 褻營衝 , 瞪爾�� / 望萄 捱欽 衛蝶蠱
		/*($Delete_[S/C]$)*/#define __NA_000614_GUILD_RANK_REFACTORING						// [614] 2007.08.09 / 褻營衝 , 望萄 楨觼機 衛蝶蠱 葬ね饜葭( 醞措 望萄 楨觼機 瞳辨嬪и 濛機 )
		/*($Delete_[S/C]$)*/#define __NA_000593_20071226_GUILD_RELATION_SYSTEM				// [593] 2007.12.26 / 喻⑷繹 , 彌艙輿 / 望萄 婦啗 衛蝶蠱
		/*($Delete_[S/C]$)*/#define __NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY	 		// [593] 2007.12.31 / 喻⑷繹 , 彌艙輿 / 望萄 驍黴,薯貲,п骯縑 曖и ぬ割じ
		/*($Delete_[S/C]$)*/#define __NA_000569_20080117_GUILD_MARK							// [593] 2008.01.17 / 喻⑷繹 , 瞪爾�� / 望萄 葆觼
		/*($Delete_[S/C]$)*/#define __NA_0_20070829_GUILD_ADD_INFO							// [000] 2007.12.16 / 喻⑷繹 , 彌艙輿 / 望萄 婦啗 蹺陛 薑爾
		/*($Delete_[S/C]$)*/#define __NA000584_20080201_GUILD_WAREHOUSE						// [000] 2008.02.01 / 喻⑷繹 , 彌艙輿 / 望萄 璽堅 <2離 望萄 衛蝶蠱縑 餵橫厥橫撿 й匙>
		/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_RENDERINFO_PACKET					// [584] 2008.02.01 / 喻⑷繹 , 望萄 溶渦 檣ん ぬ韃縑 薑爾 蹺陛 / 喻⑷繹/
		/*($Delete_[S/C]$)*/#define __NA_000880_20080204_GUILD_NOTICE						// [880] 2008.02.01 / 喻⑷繹 , 望萄 奢雖 望檜蒂 150 -> 768煎 棺葡, 望萄 憮幗縑憮 婦葬腎湍 奢雖蒂 錯萄憮幗煎 衡梯
		/*($Delete_[S/C]$)*/#define __NA_0_20080406_GUILD_MARK_MODIFY						// 2008.04.11 / 喻⑷繹 / 望萄 葆觼/婦啗 ぬ韃 瞪歎 熱薑		
		/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080613_GUILD_CONVERT_GP					// 2008.06.13 / 褻營衝 / WOPS_1963 / 望萄 幗斜 熱薑( 嬴鼠釭 GP滲�素� 腎朝 幗斜 熱薑. )
		/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080618_GUILD_RIGHT_VIEW					// 2008.06.18 / 褻營衝 / WOPS_1893 / 望萄 掏и 爾晦 幗斜 熱薑.
		/*($Delete_[S/C]$)*/#define _BUGFIX_WOPS2627_20081020_GUILD_NOTICE					// 2008.10.20 / 褻營衝 / WOPS_2627 / 望萄 奢雖 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#endif //__NA_000000_2ND_GUILD_SYSTEM

	/*($Delete_[S/C]$)*/#define	__NA_000000_20080709_DBP_MEMERY_LEAK_BUGFIX					// 2008.07.09 / 褻營衝 / DBP 詭賅葬 葛 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define	__NA_001108_20080704_GM_ADD									// 2008.07.04 / 褻營衝, 梯唳ル / GM貲滄橫 橾獄 MOVE, CALL 腎紫煙 熱薑.
	/*($Delete_[S/C]$)*/#define	__NA_000950_20080311_HEIM_DISTRIBUTE_SYSTEM					// 2008.03.11 / 褻營衝 / ж歜擊 碟寡衛蝶蠱縑 瞳辨ж晦.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20080108_WORLDSERVER_HANDLER_MACRO				// 2008.01.08 / 褻營衝 / 錯萄憮幗 ぬ韃 с菟楝 衙觼煎 瞳辨.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080310_WORLD_FRIEND_OVERFLOW					// 2008.03.10 / 褻營衝 / 錯萄憮幗 耀掘婦溼 晦棟 螃幗Ы煎 幗斜 熱薑.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_WOPS1373_20080424_DRAGON_TRANSFORM_STATUP_BUG	// 2008.04.24 / 褻營衝 / 萄楚堆 滲褐鼻鷓縑憮 蝶邐擊 螢萵唳辦 濰綠 嬴檜蠱 瞳辨 碳陛⑷鼻 熱薑.
	/*($Delete_[S/C]$)*/#define __NA000920_20080306_NPCSHOP_ITEM_SELL_HISTORY				// [920] 2008.03.06 /瞪爾��,喻⑷繹/ NPCShop 嬴檜蠱 っ衙頂羲 �鷜瘍銝�.
	/*($Delete_[S/C]$)*/#define __NA_0_080131_LF_BUG_MODIFY                                 // [000] 2008.01.10 / 喻⑷繹 / 薹雖 爾鳥衛 偃ч僥濠陛 籀葬腎雖 彊朝 幗斜 熱薑.	
	/*($Delete_[S/C]$)*/#define __NA_000921_20080227_TROUBLED_PARTS							// 2008.02.27 / 檜熱鷗, 梯雖諫 / 碟爵 雖羲 蹺陛 & Я嬴衝滌 瑙
	/*($Delete_[S/C]$)*/#define __NA_0_20080326_ARRANGMENT_LOG_02							// 2008.03.26 / 檜熱鷗 / 2離 煎斜 薑葬
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080326_RELEASE_PARTY_KEY						// 2008.03.26 / 檜熱鷗 / だじ 謙猿衛 嶸盪曖 だじ酈朝 陴嬴 氈湍 幗斜( в萄 憮幗 )
	/*($Delete_[S/C]$)*/#define __NA_0_20080417_ADD_GUILD_PK								// 2008.04.17 / 檜熱鷗, 梯雖諫 / 望萄 婦啗縑 評艇  PVP蹺陛(橾寞瞳渠)
	/*($Delete_[S/C]$)*/#define __NA_0_20080418_ADD_TROUBLED_KILL_LOG						// 2008.04.18 / 檜熱鷗 / 碟爵雖羲 嶸盪 避歜煎斜 蹺陛
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080421_MODIFY_CHAO_BORADORI_LOGIC				// 2008.04.21 / 檜熱鷗 / 偽擎 だじ錳郭葬 鬼薯奢問衛 奢問擎 腎雖彊堅 爾塭給檜朝 腎湍 幗斜
	/*($Delete_[S/C]$)*/#define __NA_00000_080107_ADD_SERVEROPTIONEX_CHAO_CONDITION_		// 2008.01.07 / 譆薑煙 / 馨葛攪曖 鼻鷓(蘋螃,橾奩)縑 評塭憮 檣蝶湍瞪塽 寡ぎ襄 檜辨陛棟 罹睡蒂 撲薑ж朝 褫暮 蹺陛
	/*($Delete_[S/C]$)*/#define __20080414_SLOTSTREAM_FOR_PACKET_BUGFIX						// 2008.04.14 / 譆薑煙 / SCSlot 婦溼 Packet Stream 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA000773_071108_ITEM_FUNCTION_EX_REVISION__				// 2008.03.12 / 檜擎だ / __NA000773_071108_ITEM_FUNCTION_EX__縑 渠и 陪絮 囀萄 偃摹 + [957] 晦棟 んл
	/*($Delete_[S/C]$)*/#define __NA001033_ARRANGEMENT_AGENT_LOG__							// 2008.04.30 / 檜擎だ / AgentServer Log 薑葬
	/*($Delete_[S/C]$)*/#define __NA_0_20080227_BACKTO_CHARLISTREQ_SYN_MODIFY3				// 2008.02.27 / 檜遽戮 / 渠虜 剪楚 嬴檜蠱 犒餌 僥薯
	/*($Delete_[S/C]$)*/#define _NA000000_20080227_MOB_AI_TRACKING_PROBLEM_CORRECTION__		// 2008.02.27 / 檜遽戮 / 跨蝶攪 AI Tracking 鼻鷓陛 鼠熱�� 奩犒腆 �捕� 還檜晦 濛機( 唸措, 縛霞僥薯諦 唸睡腎橫 鼠и Trackingж朝 鼻鷓朝 葆雙陛雖雖虜...)
	/*($Delete_[S/C]$)*/#define __NA_0_20080411_ALLPLAYERS_EQUIPINFO_BUG_FIX				// 2008.04.11 / 檜遽戮 / 葆擊縑憮 議葛攪菟檜 濰綠蒂 慇堅 氈朝 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_997_20080415_SKILL_DAMAGE_FORMULA_MODIFY				// 2008.04.15 / 檜遽戮 / 蝶鑒等嘐雖 啗骯奢衝 滲唳. 熨溼紫 щ鼻.
	/*($Delete_[S/C]$)*/#define __NA_0_20080415_DBPROXY_USER_SELECT_QUERY_INIT				// 2008.04.15 / 檜遽戮 / Query_User_Select 幗ぷ蒂 衙廓 蟾晦�倆炵絲� 熱薑. 檜瞪 議葛曖 渦嘐高檜 盪濰腎朝 匙擊 虞擠.
	/*($Delete_[S/C]$)*/#define __NA_0_20080425_WORLD_SERVER_ITERATOR_BUG_FIX				// 2008.04.25 / 檜遽戮 / end()衛 蕨諼籀葬 澀跤и剪 熱薑
	/*($Delete_[S/C]$)*/#define __NA_0_20080506_DBSERIAL_DUPLICATE_BUGFIX					// 2008.05.06 / 檜遽戮 / 嬴檜蠱 醞犒衛葬橡 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_20080801_SHOP_GAMESERVER_DOWN_BUGFIX					// 2008.08.01 / 檜遽戮 / 憧 嬴檜蠱 葬蝶お蒂 嫡棻陛 啪歜憮幗陛 啗樓 棻遴腎朝 僥薯 嫦儅
	/*($Delete_[S/C]$)*/#define __NA_000890_20080130_SEND_MEMO_BLOCK_CHECK					// 喻⑷繹 / 濠褐擊 離欽и 渠鼻и纔 薹雖 爾鳥衛 離欽 罹睡 羹觼
	/*($Delete_[S/C]$)*/#define __NA000000_080327_TRADESELL_WAREHOUSE_BUGFIX				// 璽堅煎 檜翕腎賊 寰腎朝 嬴檜蠱檜 璽堅煎 檜翕腎朝 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080404_FATIGUE_VILLAGE_APPLY_OTTA_			// Я煎紫 葆擊縑憮 瞳辨腎雖 彊朝 幗斜 熱薑.(醞措縑憮 僥薯脾.)
	/*($Delete_[S/C]$)*/#define __NA_0_20080416_PARTY_JOIN_PACKET_CHANGE					// 喻⑷繹 / だじ 蟾渠衛 瞳渠 婦啗縑 氈朝 望萄錳菟郭葬 だじ 碳陛.
	/*($Delete_[S/C]$)*/#define __NA_0_20090122_PARTY_JOIN_PACKET_CHANGE_BUG				// 2009.01.22 / 梯璽⑷, ⑷繹噢 だじ 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA_000998_20080416_INVITE_BY_CHARNAME						// 2008.04.24 / 梯唳ル, 喻⑷繹 / 嬴鼠襄縑憮釭 だじ, 望萄 蟾渠 陛棟ж啪 滲唳
    /*($Delete_[S/C]$)*/#define __NA_0_20080415_DISP_MY_ITEMINFO_AT_TOOLTIP_IF_SHIFT_DOWN	// 2008.04.21 / 梯團褻/ 鼠晦/寞橫 嬴檜蠱 罐そ ル衛衛 Ctrl酈蒂 援艇 唳辦, 濠褐檜 濰雜脹 嬴檜蠱 罐そ 翕衛 ル衛
	/*($Delete_[S/C]$)*/#define __NA_1633_20080423_BUGFIX_DONT_CREATE_CHAR_IF_CHARNUM_FULL  // [1633] 2008.04.24 / 梯團褻 / 議葛攪 5偃陛 賅舒 儅撩腎橫 氈朝 鼻鷓縑憮紫 議葛攪 儅撩�飛橉虞� 檜翕ж朝 幗斜 熱薑
    /*($Delete_[S/C]$)*/#define __NA_001030_20080428_CENTER_ALIGN_RANK_OPTION				// 2008.04.30 / 梯團褻 / 罐そ縑憮 楨觼 褫暮 陛遴等 薑溺
    /*($Delete_[S/C]$)*/#define __NA_001688_20080429_BUGFIX_HIDE_LOCKBTN_IF_CURSTATE_IS_UNLOCK	// 2008.04.30 / 梯團褻 / 璽堅 濡旎 撲薑衛 N幗が 獗晦晦	
    /*($Delete_[S/C]$)*/#define _YJ_MOD_TOOLTIP_ROW_001023 //'\n' 厥戲賊 濠翕偃ч腎啪 熱薑.
    /*($Delete_[S/C]$)*/#define __NA_0_20080331_SHOW_MSG_FOR_DISP_QUEST_PROGRESS			// 2008.04.02 / 梯團褻 / "Shift酈蒂 援腦堅 蠡蝶お 葬蝶お蒂 贗葛ж賊 蠡蝶お 霞ч鼻�窕� ル衛腌棲棻" 詭衛雖 ル衛
	/*($Delete_[S/C]$)*/#define __NA000000_080416_WAREINVEN_LOCK_BUG_FIX__					// 2008.04.16 / 檜擎だ / 綠廓 撲薑衛 輿團廓�� 殮溘 欽啗縑憮 鏃模п紫 綠廓 撲薑腎朝 幗斜 熱薑
	/*($Delete_[S/C]$)*/#define __NA001051_080515_EVENT_EFFECT_RENDER_BY_SCRIPT_1stPROCESS__	// 2008.05.15 / 檜擎だ, 彌艙輿 / [檜漸お]2輿喇 檜漸お 檜めお 轎溘 濛機 - 歜衛 (晦襄 幗瞪 ����)

    // (WAVERIX) (090719) (NOTE) ж晦 衙觼煎朝 雖辦雖 蜓匙... 橾欽 噩 虜ж賊 瞳辨 蕨薑
	//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE					// 喻⑷繹 / GameGuild曖 等檜攪蒂 SolarHash縑憮 stdext::hash煎 滲唳
	/*($Delete_[S/C]$)*/#define _NA_001029_20080428_PARTY_LEAVE_BY_GUILD_RELATION	// 200804.28 /喻⑷繹/ だじ 裕擊 衛 だじ 蟾渠й Ы溯檜橫諦 だじ頂 詹幗醞縑 瞳渠婦啗檣 望萄陛 氈朝雖 羹觼


	#if defined(__CN) || defined(_CHINA) 
		#define _GS_ADD_BIGEVENT_MSG_1122							// 2008.07.11 / 梯惜熱 / 醞措螢葡а檜漸お 詭衛雖轎溘
        #define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE				// 20080717 / 喻⑷繹 / 望萄ぬ割じ 蹺陛濛機 (醞措虜)

		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 檜遽戮 / 寞葬蝶お 褻勒羹觼橈檜 賅舒 爾罹輿晦. 橾欽 措頂虜 瞳辨.
		/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD				// 2008.06.09 / 喻⑷繹,彌艙輿 /望萄婦溼 葬瞰お 囀萄 蹺陛
		/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER	// 2008.5.30 / 喻⑷繹 / 錯萄 憮幗 陛鼻 贗塭檜樹お 掘滌ぬ韃
		/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT	// 2008.06.13 / 檜遽戮 / 箔んお 撩棟偃摹
		/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__			// 2008.04.02 / 檜擎だ, 梯雖諫 / Skill婦溼 ぬ韃 籀葬 籀葬 偃摹擊 嬪и 晦蟾濛機 Version01
		/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__				// 2008.04.21 / 檜擎だ, 梯雖諫 / 橾奩 憶蹴 憲堅葬闈 瞳辨擊 嬪и ぬ韃 碟戮 塽 瞳辨
		/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__				// 2008.06.19 / 檜擎だ / PacketControl 3rd Server<->Server (か薑 ぬ韃縑 渠п憮虜 濛機)
		/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX		// 2008.06.17 / 檜遽戮 / 贗楚蝶滌 蝶顫た ん檣お 晦棟 薯剪. 蕨瞪縑 晦�僱Ц�雖虜 ⑷營 噙檜雖 彊擠.
		/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY		// 2008.06.10 / 檜熱鷗, 梯雖諫 / 雖羲滌(碟爵雖羲蛔) 奢問撩щ(橾奩.碟爵.PK蛔) 羹觼 滲唳
		/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / 檜熱鷗 / CHECK_ATTACK_PROPENSITY_MODYFY 濛機衛 緒霞 割羹觼 蹺陛.
		/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX					// 2008.06.26 / 檜熱鷗 / 唳啗雖羲縑憮 幗Щ蝶鑒 衛瞪衛 爾塭給檜 腎湍 幗斜.
		/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX				// 2008.06.30 / 檜熱鷗 / Щ葬蘋螃 嶸盪曖 蠡蝶お 熱塊 陛棟ж啪(蝶觼董お薯橫).[蝶觼董お 滲唳衛 翕衛 蹺陛 腎橫撿л]
		/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY					// 2008.07.01 / 檜熱鷗 / 蘋螃嶸盪 濠翕戲煎 鬼薯奢問腎雖 彊朝 幗斜.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK					// 2008.07.24 / 檜熱鷗 / 顫橾羹觼衛 詹幗л熱頂 蝶鷓す滲熱 餌辨 幗斜 熱薑.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_CHINA_PK_RULE						// 2008.07.24 / 檜熱鷗 / 醞措 PK瑙 滲唳
        /*($Delete_[S/C]$)*/#define __NA_2001_20080617_BUGFIX_CLEAR_PARTY_WHEN_RECONNECT// 2008.06.18 / 梯團褻 / 營 蕾樓衛 だじ錳 薑爾 薯剪
        /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED // 2008.07.04 / 梯團褻 / 萄楚堆 滲褐 鼻鷓縑憮 濰雜腎橫 氈朝 晦除薯 嬴檜蠱 晦除 虜猿衛 檜鼻 ⑷鼻 熱薑
        /*($Delete_[S/C]$)*/#define __NA_0_20080707_CLEAR_HERO_INFO_WHEN_SELECT_CHAR	// 20080707 / 梯團褻 / 檜瞪 議葛攪 望萄 葆觼陛 薯剪腎雖 彊朝 幗斜 熱薑

	#elif defined(__JP) || defined(_JAPAN) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / 檜擎だ / 酈爾萄 檜翕 р 寞雖 塽 翕晦�� 薯橫 瑞ず 橾睡 薯剪

	#elif defined(__KR) || defined(_KOREA) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / 檜擎だ / 酈爾萄 檜翕 р 寞雖 塽 翕晦�� 薯橫 瑞ず 橾睡 薯剪
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 檜遽戮 / 寞葬蝶お 褻勒羹觼橈檜 賅舒 爾罹輿晦. 橾欽 措頂虜 瞳辨.
	#endif

//==================================================================================================
// (0.8.1.4) (Delete reserved working macros)
//
//==================================================================================================


#endif


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0.8.0.8 網萄 婦溼 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#if NATION_BUILD_VERSION >= 808 

	/*($Delete_[x/C]$)*/#define __ARMORKIT_ENABLED_CHECK_BY_CMDnACK__			// 2007/01/11 Armorkit 濛翕罹睡 羹觼ж晦 嬪и ぬ韃薑曖
	/*($Delete_[S/C]$)*/#define __NA070212_ENHANCED_SECURE_POLICYMENT__			// 2007/02/12 爾寰 鬼�� 寞寰 婦溼 憮幗/贗塭 籀葬 瑞ず

#endif //NATION_BUILD_VERSION >= 808 


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢鱔 偃嫦濠滌 Define
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  氬湯 奢除  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//#define __NA_0_20061109_EXP_EVENT_								// 2006.11.09 / 檜遽戮 / Ы溯檜衛除縑 評艇 唳я纂 隸陛 檜漸お
//#define __NA_0_20070329_CHUNTING_FORMULA_BUG						// 2007.03.29 / 檜遽戮 / 唳爵ьた 爾鼻唳я纂 掘⑷ 幗斜 熱薑
//#define __NA_0_20080618_STATE_INFO_SIZE_DECREASE					// 2008.06.18 / 檜遽戮 / STATE_INFO 餌檜鍔 蹴模. PLAYER_RENDER_INFO曖 餌檜鍔蒂 還歜.

//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


// 2006. 9. 13 / 嶸營艙 / NPC諦 渠�亳� NPC 陛 給嬴爾剪釭 爾雖 彊紫煙 ж堅,嘐棲裘縑 ル衛 罹睡縑評艇 ル衛.(NPCExtraInfo.txt だ憮 熱薑蛔.)
//#define __CN_269_MATRIX_CARD_AUTH

//收收收收收收收收收收收收收收收收收收收  璽⑷ 奢除  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//蛤幗斜 辨
//#define __NA_0_20070521_QUEST_CONDITION_RESULT_CHANGE	// 2007.05.21 / 梯璽⑷ / 蠡蝶お 鐘蛤暮 唸婁 Type擊 BOOL縑憮 enum戲煎 滲唳(蛤幗斜辨)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//晦�嘍� 雖翱 濛機
//#define WORK_RANKUP_ETC_ITEM							// 2006.09.19 / 梯璽⑷ / RankUp縑憮 偽雖 彊擎 嬴檜蠱戲煎紫 �捕�擊 螢萵 熱 氈朝 衛蝶蠱 - 爾�紋� 霤堅ж3 
//#define __NA_0_20080421_REBIRTH_NO_PARTY							// 2008.04.21 / 梯璽⑷, 梯唳ル / だじ 嬴棲塭紫 睡�� 陛棟ж啪...
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// 棻擠網萄
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//濛機
//#define __NA_1217_EXTEND_MISSION_TYPE_COUNT						// 2009.02.06 / 梯璽⑷, 嘐暮 顫殮 偃熱 �挫�
///*($Delete_[S/x]$)*/#define __NA_001358_ENCHANT_UNIFICATION							// 2009.06.22 / 梯璽⑷, 檣臘お 橾錳��

//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  JSCHO 奢除  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//#define __WAVERIX_2006122x_SERVER_TYPE_CHANGE_POSTWORK__// 2006. 12. 2x / 檜擎だ / 憮幗 顫殮 翱掘偃嫦薹婁 蜃蹺晦 嬪и 濛機 (衙觼煎貲擎 嬴霜 敘罌擊 評腦雖朝 跤ц擠)
// 檜 衙觼煎 瞳辨衛 掘幗瞪戲煎 翕濛(⑷營 瞳辨醞) 檜 輿戮擊 薯剪ж賊, 億煎遴 憮幗顫殮戲煎 瞪��;.

//#define	__NA_PACKETFLOW_TEST	
//#define		__TRIGGER_TEST_LOG
// ==> 嬪曖 望萄 楨觼機擎 п諼幗瞪縑朝 瞳辨腎橫 氈棻.
//#define		__NA_000000_CHANGE_TABLE_DATA					// 2008.09.02 / 褻營衝 / 纔檜綰等檜攪蒂 翕瞳戲煎 滲唳ж濠.
//#define		__NA_001192_20081110_TICKET_LOG					// 2008.11.10 / 褻營衝 / じ鰍 煎斜.


//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//==================================================================================================
//====================================  WAVERIX 奢除 衛濛 ==========================================
//--------------------------------------------------------------------------------------------------
// debugging tools
//#define __NA000000_080317_ABNORMAL_EXIT_REDIRECT__                // 2008.03.17 / 檜擎だ / CRT Runtime Error Message Box 渠褐 dump陴紫煙 ж朝 濛機 (僥薯 �挫怹� в蹂и 唳辦縑虜 餌辨й 匙)
//#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_USE_SHARED_POOL_  
//--------------------------------------------------------------------------------------------------
// NOTE: applied section, this working macros help reference search.
// a independent facility related to the item customizing content.
// this content can enables various outputs
// when a customizing item is pointed using an etherealizer item
// Keyword: _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
// Keyword: __NA002610_110428_REMOVE_INFINITY_USING_LUCKY // 2011.04.28 / 檜擎だ / 楝酈 衛除 (鼠и) 嫦晝 薯剪
//--------------------------------------------------------------------------------------------------
// reserved contents for next version
//--------------------------------------------------------------------------------------------------
// specific contents for foreign version
//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_ // 檜擎だ, 檜紫⑽ / 議衛顫殮(PP蘋萄) 蹺陛縑 評艇 掘碟 濛機
//#define __NA000000_070523_REPORT_TO_SOLARAUTH
//#define _NA000000_110218_RANK_REPORTER_IN_DBUNIT // 2011.02.18 / 檜擎だ / Ep2<-Ep1 滲�紗� 薯剪腎朝 楨觼 薑爾 蹺轎晦
//--------------------------------------------------------------------------------------------------
// pending list
//#define __NA000000_070928_PACKET_UNICAST_UPDATE__                 // 2007.09.28 / 檜擎だ / VarMsg2煎 籀葬腎湍 睡碟擊 覬渦 蔣啪 籀葬й 熱 氈紫煙 掘鼻
//#define __NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__  // 2008.01.22 / 檜擎だ / VALENTINE 檜漸お蒂 翱翕 議衛婦溼 濛機
//#define __NA001243_090112_SKILL_ANIMATION_SYNC_CORRECTION__         // 2009.01.12 / 檜擎だ,梯惜熱 / 蝶鑒縑棲詭檜暮 翕晦�� 掘褻 滲唳 濛機
//#define _NA00XXXX_091123_SETUP_CASH_ITEM_AVAILABLE_PERIOD_ // 2009.XX.XX / 檜擎だ, ? / 議衛嬴檜蠱 嶸�膨滶� 撲薑
//#define _NA_0_20100211_SD_EFFECT_MODIFY          // 2010.02.11 / 檜�ˋ� / 2顫 瞳辨 蝶鑒 衛瞪衛 等嘐雖 滲唳.
//#define _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_    // 檜擎だ / 嬴檜蠱 蝶お葡 幗瞪 8 濛機
//#define _NA000000_VALUE_TYPE_SIMPLIFICATION // 2010.02.16 / 檜擎だ / 熱纂 殮溘 顫殮 撲薑高 (1,2)煎 除模�� 濛機
//--------------------------------------------------------------------------------------------------
// working contents
//#define _NA000000_CW_PROTOCOL_UNIFICATION // 檜擎だ / WorldServer<->Client Щ煎饜屬 CG啗翮婁 鱔м, to change inter-communication
//#define _NA002676_WORLD_SERVER_RENEWAL // 檜擎だ / WorldServer 營掘蹴
//--------------------------------------------------------------------------------------------------
//=====================================  WAVERIX 奢除 部 ===========================================
//==================================================================================================

//收收收收收收收收收收收收收收收收收收收 GS 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _GS_MOD_ITEMDURA_PENALTY_1062
//收收收收收收收收收收收收收收收收收收收 END       收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收 AEORAGIE 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define __NA001126_NEW_SKILL_SCREEN_EFFECT // 2008.07.16 / 譆薑煙, 梯惜熱 / 蝶鑒 餌辨 衛, �蝯曏� �膩� 蹺陛
//收收收收收收收收收收收收收收收收收收收 END       收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收 SHAKETAKE 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE
//#define __NA_001169_20080930_GUILD_LOG_SYSTEM

//#define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE
//#define SHAKETAKE_DATA_CHECK
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//收收收收收收收收收收收收收收收收收收收     撿瑞 DEFINE      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define __NA_000571_20070702_WORLD_UNIT_PARTY_2ND			// 2007.07.02 / �票瑰� / 錯萄欽嬪 だじ 2離 濛機
//#define __NA_000313_20070223_REALTIME_CHAT_BLOCK			// 2007.02.23 / �票瑰� / 褒衛除 瓣た 綰楣

//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  gamelulu(團薑艙)  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define __[措陛囀萄]_[濛機廓�β_[陳瞼]_[濛機貲]_
//#define Debug_Domination                                          // 雖羲 薄滄瞪 煎斜 婦葬 define       
//#define _NA_20091014_ZEROPOINT_RANKING_UNACCEPT                   // 2009.12.14 / 團薑艙 / ん檣お 楨韁 唸骯衛縑 0ん檣お朝 蛔煙ж雖 彊朝棻.
//#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS             // 2010.04.15 / 團薑艙 / ん檣お 楨韁 唸骯 褻勒 熱薑 Mission Key => RegisterDate
//#define _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC                     // 2010.04.20 / 團薑艙, 檜鷓遽 / 衛蝶蠱 薹雖 煎霜 熱薑
//#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION
//#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END        // 2010.06.21 / 團薑艙 / 雖羲 薄滄瞪 檜漸お 蹺陛
//#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 鱔м 濛機(雖羲薄滄瞪)
//#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE             // 2010.09.01 / 團薑艙, 檜鷓遽 / 衛蝶蠱 詭賅 婦溼 啪歜 煎斜 蹺陛
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / 團薑艙 / 斜瑜 爾傘蝶 瞳辨 寞徹 鼻,ж嬪 詹幗 瞳辨 滲唳
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / 團薑艙 / 雖羲 薄滄瞪 檜漸お 犒掘
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / 團薑艙 / 檜漸お 歜衛 顫檜該
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / 團薑艙 / 晦除 �側瘓� 嬴檜蠱 剪楚 碳棟
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / 團薑艙 / 斜瑜 爾傘蝶 瞳辨 寞徹 鼻,ж嬪 詹幗 瞳辨 滲唳
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / 團薑艙 / 晦除 �側瘓� 嬴檜蠱 剪楚 碳棟
//#define _NA_20100604_BETTER_DOMINATION_MANAGER                    // 2010.06.04 / 團薑艙, 檜鷓遽 / 雖羲 薄滄 婦葬 寞衝 偃摹
//#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                  // 2010.04.20 / 團薑艙, 檜鷓遽 / 雖羲 薄滄瞪 殮雞 寞衝 滲唳
//#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                // 2010.12.14 / 團薑艙, 檜鷓遽 / 薄滄瞪 婦葬 寞衝 偃摹縑 評艇 唳衙 嬴檜蠱 羹觼 寞衝 滲唳
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / 團薑艙 / 雖羲 薄滄瞪 檜漸お 犒掘
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / 團薑艙 / 檜漸お 歜衛 顫檜該
//#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT       // 2010.01.16 / 團薑艙, 檜紫⑽ / 薄滄瞪 寞漁 等檜攪 蹺陛
//#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                          // 2011.03.03 / 團薑艙, 檜紫⑽ / 顫殮滌 NPC 鼻薄 晦棟 蹺陛
//#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN              // 2011.04.06 / 團薑艙, 檜鷓遽 / 薄滄瞪 霤罹 寞衝 滲唳
//#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO                 // 2011.06.10 / 團薑艙 / 雖羲 薄滄瞪 羹割 薑爾 iniだ橾 滲唳(ServerOptionEX.ini -> GameOption.ini)
//#define _NA_20110630_BATTLE_ZONE_RENEWAL                          // 2011.06.30 / 團薑艙, 檜紫⑽ / 寡ぎ襄 葬景橡
//#define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS             // 2011.12.20 / 團薑艙, 歎雙謙 / 寡ぎ襄 葬景橡 綠薑鼻 蕾樓 謙猿 嶸盪 籀葬
//#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN               // 2010.01.06 / 團薑艙 / 諾曖 隸ル 奩�秘Щ� 彊紫煙 熱薑
//#define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST          // 2012.05.11 / 團薑艙 / 雖羲 薄滄瞪 蹺陛 蹂羶 餌о
//  
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  JAYKANG  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _NA_20101011_HEIM_LOTTO_CASH_TICKET                             // 2010.10.11 / 鬼營遽 / 議衛 嬴檜蠱戲煎 ж歜ч遴 擬賅 辨雖 �僱� 晦棟 : _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE諦 л眷 �側瘓音� 熱 橈擠
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//收收收收收收收收收收收收收收收收收收收  NAMKA  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _CN_0_20091013_RANKUP_EXTEND                          // 2009.10.13 / 陴晦瑰 / 楨觼機 偃摹. 睡瞳曖 給, 蟾晦�倣� 給 蹺陛 塽 楨觼機衛 稱朝 樓撩 褫暮 薯剪
//#define _NA_0_20100325_QUICK_SLOT_CHANGE                      // 2010.03.25 / 陴晦瑰,檜紫⑽ / Quick 蝸煜 滲唳 (48偃曖 蝸煜->40偃曖 蝸煜戲煎 滲唳)
//#define _NA_0_20091208_VENDOR_ITEM_SEARCH                     // 2010.01.20 / 檜紫⑽, 陴晦瑰 / 鼻薄匐儀 衛蝶蠱
//#define _NA_0_20100416_RELOAD_MAPOBJECT                       // 2010.04.16 / 陴晦瑰, 彌艙輿 / 裘螃粽薛お だ橾 葬煎萄
//#define _NA_0_20100519_GMCOMMAND_MOVEFIELD                    // 2010.05.19 / 陴晦瑰 / GM 貲滄橫 蹺陛. 翕橾и 啪歜Zone曖 か薑 в萄煎 牖除 檜翕
//#define _NA_0_20100519_GMCOMMAND_CREATEMISSION                // 2010.05.19 / 陴晦瑰 / GM 貲滄橫 蹺陛. 翕橾и 啪歜Zone曖 か薑 в萄煎 牖除 檜翕
//#define _NA_0_20100528_ACCUMULATE_POINT_EVENT                 // 2010.05.28 / 陴晦瑰,檜鷓遽 / 瞳董 ん檣お 爾鼻 瑙滇衛 か薑 [蹂橾/衛除] 鼻晝 嬴檜蠱 �僱� 檜漸お 蹺陛
//#define _NA_0_20100901_SKILLTREE_SYSTEM                       // 2010.09.01 / 陴晦瑰, / 蝶鑒お葬 衛蝶蠱 蹺陛
//#define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION          // 2010.10.11 / 陴晦瑰, / 瞳董 ん檣お 雖晝衛 溯漣/譆渠偎熱 蒂 薯иж朝 褫暮 蹺陛.
//#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL               // 2011.01.31 / 陴晦瑰,梯褐鷓 / 蝶鑒お葬 衛蝶蠱曖 蝶鑒曖 譆蟾 蝗菜 寞衝 滲唳.(蝶鑒 晦獄 溯漣 0蝶鑒 -> 1蝶鑒).
//#define _NA_0_20110305_BUG_READ_BIG_LOGFILE                   // 2011.03.05 / 陴晦瑰 / 2GB檜鼻曖 煎斜 檗晦 褒ぬ 幗斜.(だ橾 с菟 檗晦 л熱煎 滲唳)
//#define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / 陴晦瑰, 嶸貲熱 / 議葛攪 綠蕾樓衛 蝶鑒 籐顫歜 嶸雖 晦棟 蹺陛.
//#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                 // 2011.03.18 / 陴晦瑰, / 蝗菜 嬴檜蠱 諫猿 晦除 撲薑 蹺陛.
//#define _NA_0_20110329_GAMEOPTION_SETTING                     // 2011.03.29 / 陴晦瑰, 嶸貲熱/ 啪歜 褫暮曖 DB 盪濰
//#define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / 陴晦瑰, 嶸貲熱/ 議衛憧 葬ね饜葭.
//#define _NA_0_20110803_PLAYER_MOVEMENT_SYNC                     // 2011.08.03 / 陴晦瑰, Ы溯檜橫 檜翕 翕晦��.
//#define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 陴晦瑰, 啪歜煎斜 辨榆縑 評艇 だ橾碟葬.
//#define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 陴晦瑰, 煎傳 嬴檜蠱 餌辨陛棟и Ы溯檜橫 溯漣 薯и.
//#define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / 嶸貲熱, 陴晦瑰, 蕾樓 贗塭檜樹お曖 MAC 輿模 煎斜 陴晦晦.
//#define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / 陴晦瑰, / だじ 檣錳熱縑 曖и ж歜 萄奧徽 褻薑.
//#define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 陴晦瑰,梯褐鷓 / 葆辦蝶 檜翕衛 檜翕 唳煎 偎熱 滲唳 (晦襄 64 -> 512)
//#define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / 陴晦瑰,梯褐鷓 / 頂掘紫 0檣 鼠晦紫 寞橫掘諦 偽擎 寞衝戲煎 だ惚腎紫煙 晦棟 熱薑.
//#define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 陴晦瑰 / だじ錳 檣錳熱縑 評艇 ж歜 陛醞纂 滲唳.
//#define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / 陴晦瑰,檜鷓遽 / 萄楚堆 釭檜お諦 辨滲褐 1,2 離曖 蝶鑒 奢嶸 晦棟 蹺陛.
//#define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / 陴晦瑰 / 懍���� 賅菊 MD5 -> SHA256 滲唳.
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//收收收收收收收收收收收收收收收收收收收  YJ  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  僥鼻⑷  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _NA_20100307_BETAKEY_SYSTEM                                   //漆顫酈 衛蝶蠱 (僥鼻⑷, 檜紫⑽)
//#define _NA_20100322_AGGRO_DISPLAY                                    //橫斜煎 熱纂 �飛橦� ル衛 (僥鼻⑷, 檜紫⑽)
//#define C_NA_0_20100520_ACSYSTEM_CHANGED                            //2010.05.20 僥鼻⑷ 檜鷓遽 AC 衛蝶蠱擊 機瞳欽嬪煎 滲唳и棻.
//#define _JP_0_20101123_PMS                                          //橾獄 啪歜螞曖 憮幗該 婦葬 衛蝶蠱檣 PMS 瞳辨
//#define _JP_20110222_INGAME_CHARNAME_CHANGE                         //橾獄縑 憮幗 鱔м擊 嬪п 議葛鑣貲 滲唳擊 啪歜頂縑憮 雖錳п遽棻 (壎戲煎 議葛貲 滲唳檜 寰脹棻.)
//#define _RU_20110801_SEND_GIFT_FRIEND_CHECK //耀掘縑啪 摹僭擊 爾鳥陽 頂陛 鼻渠寞曖 耀掘罹撿 ж堅 鼻渠寞紫 釭曖 耀掘罹撿 и棻.
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//--------------------------------------------------------------------------------------------------
// hukim(梯⑷錠) 衛濛
//
//#define _NA_000068_20100419_PARTY_FINDING_SYSTEM // 2010.04.19 / 梯⑷錠, 檜紫⑽ / だじ 瓊晦 衛蝶蠱
//#define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 梯⑷錠, 檜紫⑽ / 薑爾 翮塋褒 鱔м 餌辨濠 陛檜萄 衛蝶蠱
//#define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / 梯⑷錠, 梯褐鷓 / 楠渾 嬴檜蠱 儅撩
//#define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / 梯⑷錠, 梯褐鷓 / 譆鼻晝 檣臘お
//#define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / 梯⑷錠 / SD 婦溼 奢衝 滲唳
//#define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / 梯⑷錠 / 嫦酈葬 奢問溘 奢衝 滲唳
//#define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / 梯⑷錠 / 啪歜憮幗 ObjectFactory 偃摹
//#define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / 梯⑷錠 / ObjectKey 渠羲ァ �挫�
//#define _NA_004035_20120227_COSTUME_ITEM // 2012.02.27 / 梯⑷錠, 梯褐鷓 / 囀蝶え 嬴檜蠱
//#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 梯⑷錠 / 啪歜 褫暮 お楠燮暮 籀葬 晦棟 蹺陛
//#define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / 梯⑷錠, 梯褐鷓 / 1202 蝶鑒 葬景橡
//#define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / 梯⑷錠, 梯褐鷓 / 蝶鑒 奢問縑 縑纔腦 驕�� 瞳辨
//#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / 梯⑷錠, 檜鷓遽 / 薄滄瞪 爾鼻 褫暮 瞳辨 寞衝 滲唳
//#define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / 梯⑷錠, 檜鷓遽 / 褐敘 議葛攪 嘐蝶す 蹺陛
//#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / 檜鷓遽 / 褐敘 議葛攪 贗塭檜樹お 熱薑(蝶觼董おだ憮婦溼)
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / 檜鷓遽 / 褐敘 贗楚蝶 顫殮 蹺陛縑 評艇 顫殮MAX 隸陛煎 檣и 嫦儅腎朝 螃盟 歜衛 熱薑
//#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
//
// hukim(梯⑷錠) 部

//收收收收收收收收收收收收收收收收收收收  chief85(梯霞��)  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// chief85(梯霞��) 衛濛
//#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                                   // 2012.04.12 / 梯霞�� / 寡ぎ襄 寞廓�ㄧ� 醴嗽棲じ �飛橦� ル衛.
//#define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE                   // 2012.04.13 / 梯霞�� / 寡ぎ襄 寞 蕨擒 衛 綠塵寞擎 殮濰罹睡 詭撮雖 爾頂雖 彊擠.
//#define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST                     // 2012.04.16 / 梯霞�� / 蕾樓 謙猿衛 蕨擒脹 寡ぎ襄 寞 葬衛お蒂 餉薯и棻.
//#define _NA_004791_20120430_1202_SUMMON_STATE                                     // 2012.04.30 / 梯霞�� / 嫦酈葬 模�紡鰤� 顫啃擊 熱翕 奢問ж朝 翕寰, 檜嘐 �側瘓音� 醴裔萄 嘐 犒謙.
//#define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE                      // 2012.05.11 / 梯霞�� / 顫饕и 餌錳 醞懈 絳 掘羲 む割じ 雖羲 彰嬪 營 撲薑 塽 唸啗 雖羲 頂 PVP陛棟.
//#define _NA_004035_20120227_COSTUME_ITEM                                          // 2012.05.23 / 梯霞��, 梯褐鷓 / 囀蝶え 嬴檜蠱
//#define _NA_004910_20120601_SUMMON_PVP_RULE                                       // 2012.06.01 / 梯霞�� / 嫦酈葬 模�紡� SD(99):HP(1) 瑙 瞳辨.
//#define _NA_000000_20120607_INVENTORY_SORT                                        // 2012.06.07 / 梯霞�� / 檣漸饜楝 薑溺.
//#define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET             // 2012.06.14 / 梯霞�� / へ�膩�<渠鼻曖 寞橫溘 馬模徽瞳辨>
//#define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY                        // 2012.06.21 / 梯霞��, 歎雙謙 / 寞橫 撩奢睦 鼻и摹 熱薑 99% -> 80%
//#define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE                   // 2012.06.22 / 梯霞�� / 嫦酈葬 模�紡� PVP瑙擊 輿檣婁 偽擎 瑙 瞳辨.
//#define _NA_005028_20120627_MODIFY_IMPOSSIBLE_CAST_HIDE_SKILL_IN_SSQ              // 2012.06.27 / 梯霞�� / 顫塊и 餌錳縑憮 憎紫辦 ж檜萄 蝶鑒 餌辨碳陛棟.
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                              // 2012.06.27 / 歎雙謙, 梯霞�� / 蘋螃蝶 襄 瞪濰 衛蝶蠱.
//#define _CN_006014_20120822_GUILD_LOG_DELETE                                      // 2012.08.22 / 梯霞�� / 醞措 望萄煎斜縑 諾曖隸ル 殮雞偎熱 煎斜 餉薯.
//#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA             // 2012.08.23 / 梯霞�� / 望萄囀檣 ж瑞 晦睡 譆渠 偎熱 蝶觼董お 等檜攪煎 瞳辨
//#define _NA_006040_20120830_RESIS_BAD_STATUS_RATIO                                // 2012.08.23 / 梯霞�� / 僭擒 塽 幗Щ 蝶鑒 餌辨衛 蝶鑒鼠衛�捕�檜 瞳辨腎朝 幗斜
//#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP                                // 2012.09.04 / 梯霞�� / 嬴檜蠱 夥款幗葬晦 碳陛棟, PK萄照陛棟 顫殮 蹺陛.
//#define _NA_006057_20120904_PK_DROP_RATIO_MODIFY                                  // 2012.09.04 / 梯霞�� / 濠褐曖 溯漣 爾棻 堪擎 Ы溯檜橫蒂 避艘擊 唳辦 避擎 Ы溯檜橫曖 嬴檜蠱萄奧睦 熱薑 鼻и摹 100%->50%
//#define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP                                   // 2012.09.10 / 歎雙謙, 梯霞�� / 蘋螃蝶 襄 濠嶸瞪癱 衛蝶蠱.
//#define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY                              // 2012.09.25 / 梯霞�� / 縑纔腦驕�� 薑疇 滲唳(晦獄奢問溘虜 -> 瞪羹等嘐雖)
//#define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY                   // 2012.09.26 / 梯霞�� / 寡ぎ襄 殮濰蕨擒 辦摹牖嬪 滲唳. (譆蟾 -> 譆褐牖)
//#define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY   // 2012.10.12 / 梯霞�� / PVP瑛縑憮 だじ薑爾璽曖 ⑷營嬪纂陛 澀跤 ル衛 腎朝 幗斜 熱薑
//#define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD                    // 2012.10.15 / 梯霞�� / 寡ぎ憮幗縑憮 в萄憮幗煎 檜翕衛 綠奢薑 в萄紫 陛棟ж紫煙 熱薑
//#define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY                 // 2012.10.17 / 梯霞�� / 給霞 塽 婦鱔啗翮 蝶鑒 諒觼塽 っ薑 熱薑
//#define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION                      // 2012.11.01 / 梯霞�� / 縑傘雖戮暮擊 衛瞪ж朝 紫醞 奢問碳陛鼻鷓(蝶欐, 棻遴..)陛 腎橫紫 蝶鑒檜 啗樓 衛瞪腎朝 幗斜
//#define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD               // 2012.11.16 / 梯霞�� / 聽粽衛蝶蠱 碟п衛 爾鼻葬蝶お縑憮 議衛牆萄 薯剪
//#define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME                // 2012.12.11 / 梯霞�� / 偃檣 錚檜ん檣お 餉薯 渠晦 衛除 熱薑
//#define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY          // 2012.12.28 / 梯霞�� / 模�紡鶺� 輿檣檜 鬼繹賅萄橾 唳辦 奢問,Я問,蝶鑒餌辨 陛棟 -> 碳陛棟戲煎 熱薑
//#define _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER                              // 2013.01.03 / 梯霞�� / 憮幗檜漸お お葬剪 蹺陛
//#define _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM                      // 2013.01.04 / 梯霞�� / team check condition お葬剪 蹺陛
//#define _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD                       // 2013.01.04 / 梯霞�� / [偃檣/瞪羹]縑啪 [HP/MP/SD]蒂 [熱纂]虜躑 [隸陛/馬模]蒂 瞳辨 action お葬剪 蹺陛
//#define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG                                 // 2013.02.25 / 梯霞�� / 嫦酈葬曖 模�紡鷏� 模�� ж罹 跨蝶攪蒂 奢問, 模�紡鰤� 議葛攪 斬籀煎 檜翕 й 陽 梱雖 議葛攪蒂 菴煎 檜翕衛鑒 唳辦 跨蝶攪陛 夥爾 腎朝 ⑷鼻
//#define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD                                  // 2013.03.13 / 梯霞�� / Я曖撩瞪縑憮 錶Щ觼葬蝶驍擊 模�� �� 議蝶た檜 棻 腎晦瞪 模�納� 嶸盪陛 避橫紫 觼葬蝶驍檜 模�秘Ж� 幗斜
//#define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG                                    // 2013.03.19 / 梯霞�� / 萄楚堆釭檜お煎 鼠瞳鼻鷓縑憮 辨滲褐 寰腎朝 幗斜 熱薑
//#define _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM                              // 2013.03.20 / 梯霞�� / 瞪濰 蹺陛濛機 #1 <瞪濰 衛除 薯橫 衛蝶蠱 蹺陛>
//#define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY                              // 2013.03.25 / 梯霞�� / 瞪濰 蹺陛濛機 #2 <瞪濰 瞪癱 鼻�� 憲葡>
//#define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG                           // 2013.03.28 / 梯霞�� / 模�粉�(嫦酈葬)曖 鼻鷓 醞 蝶鑒籐顫歜 隸馬 鼻鷓陛 模�紡鶹※� 瞳辨檜 寰腎朝 幗斜
//#define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY                          // 2013.04.01 / 梯霞�� / 嘐暮 塽 瞪濰縑憮 模�紡鶺� 瞪癱唸婁 等檜攪蒂 瞪癱唸婁璽縑 奩艙 (瞳縑啪 陛и 等嘐雖, 模�紡鶺� 鑒熱, ��榆)
//#define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO       // 2013.04.01 / 梯霞�� / 嫦酈葬 模�紡鶺� �� 蝶鑒檜 模�粉�(輿檣)曖 蝶鑒奢問溘 綠徽縑 蜃啪 蹺陛 ��榆檜 啗骯腎雖 彊擎幗斜
//#define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL                  // 2013.04.09 / 梯霞�� / 頂陛 鬼繹 賅萄橾 唳辦 瞳檜 釭縑啪 蝶鑒檜 衛瞪腎朝 ⑷鼻 熱薑.
//#define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME                           // 2013.04.02 / 梯霞��, 歎雙謙 / GM貲滄橫煎 檜漸お 瞪濰 衛濛
//#define _NA_006589_20120415_ENCHANT_PRICE_MODIFY                                  // 2013.04.15 / 梯霞�� / 檣臘お 綠辨 奢衝 熱薑
//#define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO                        // 2013.04.15 / 梯霞��, 歎雙謙 / 顫啃璽 薑爾縑 蔓萄 ル衛 蹺陛
//#define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY                             // 2013.04.16 / 梯霞�� / 醴嗽棲じ璽 鼻撮薑爾縑 謙褶檜 澀跤 ル衛腎朝 幗斜
//#define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS                      // 2013.04.22 / 梯霞�� / 辨滲褐 鼻鷓縑憮 觼葬蝶驍は葬蝶 嬴檜蠱 �側瘓亳� 雜辨ж堅 氈朝 濰綠陛 瞳辨腎雖 彊擎 幗斜
//#define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY                        // 2013.04.22 / 梯霞�� / 嘐暮 お葬剪 蹺陛 塽 熱薑濛機
//#define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY                          // 2013.04.26 / 梯霞�� / 嫦酈葬 模�紡� 蝶鑒 諒觼 幗斜
//#define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY                            // 2013.04.26 / 梯霞�� / 嬴檜蠱 褫暮 蹺陛 滲唳
//#define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME                            // 2013.05.02 / 梯團繹, 梯霞�� / 啪歜 褫暮 蹺陛(囀蝶え 爾檜晦 褫暮)
//#define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION          // 2013.05.06 / 梯霞�� / 瞪濰 蕨擒醞 в萄 跨蝶攪 奢問 碳陛
//#define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION   // 2013.05.08 / 梯霞�� / 瞪濰 蕨擒醞 憮幗 摹鷗 碳陛
//#define _NA_006710_20130509_CRYSTALWARP_BUG                                       // 2013.05.09 / 梯霞�� / 檜嘐 だ惚脹 觼葬蝶驍擊 霤褻ж雖 跤й 唳辦 鬼薯煎 棻擠 觼葬蝶驍擊 霤褻ж朝 幗斜
//#define _NA_006728_20130520_SKILL_BALANCE_MODIFY                                  // 2013.05.20 / 梯霞�� / 蝶鑒 寨楛蝶 熱薑( 奩餌婦溼 蝶鑒 熱薑 )
//#define _NA_006731_20130521_ENCHANT_ADD_OPTION                                    // 2013.05.21 / 梯霞��, 歎雙謙 / 譆鼻晝(13~15)檣臘お 褫暮 蹺陛
//#define _CH_006797_20130703_PK_DROP_RULE_MODIFY                                   // 2013.07.03 / 梯霞�� / 醞措 в萄 PK 薑疇 滲唳
//#define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM                             // 2013.07.03 / 梯霞��, 歎雙謙 / 瞪濰 癱ル 衛蝶蠱 蹺陛
//#define _NA_006826_20130722_DOMINATION_RENEWAL                                    // 2013.07.22 / 梯霞��, 歎雙謙 / Я曖撩瞪 薄滄瞪 偃ら
//#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG                               // 2013.07.31 / 梯霞�� / 醞措 鬼繹賅萄 幗斜 婦溼 煎斜晦煙
//  
// chief85(梯霞��) 部
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  團唳蝓  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 團唳蝓 衛濛
//#define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / 團唳蝓 / ac婁薯 蹺陛(濠Я腦 問瞪雖)
//#define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / 團唳蝓 / 瞪濰縑憮 瞳 籀纂 衛, 蹺陛 菜薄
//#define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / 團唳蝓 / 檜翕+婦鱔⑽ 蝶鑒檜 1貲曖 瞳縑啪虜 Яп蒂 邀
//#define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / 團唳蝓 / 樂煎睡渠察雖諦 醴Ы膩濰察雖蒂 л眷 陛韓 熱 氈紫煙 л
//#define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / 團唳蝓 / '嘐暮諫猿', '瞪濰蝓葬' 蠡蝶お顫殮 蹺陛
//#define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / 團唳蝓 / 瞪濰縑 '橫衛蝶お' 蹺陛
//#define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / 團唳蝓 / 鼻鷓 幗Щ曖 辦摹牖嬪 啗骯寞衝 滲唳
//#define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / 團唳蝓 / 鼻嬪幗Щ陛 勘溥氈擊 唳辦,ж嬪幗Щ 僭擒 餌辨 寞雖
//#define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / 團唳蝓 / 橫網葬じ顫殮蹺陛(345), 蝶鑒奢問溘, 寞橫�膩�縑 艙щ嫡雖彊朝 HP隸馬
//#define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM               //2013.07.05 / 團唳蝓 / 楨韁衛蝶蠱 葬景橡 塽 瞪濰楨韁 蹺陛
//#define _RU_006860_20130806_COSTUME_PCBANG_EFFECT                //2013.08.06 / 團唳蝓 / 囀蝶え縑 Я衛寞 褫暮 瞳辨衛酈晦
//#define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / 團唳蝓 / 楨韁衛蝶蠱 葬景橡 塽 瞪濰楨韁 蹺陛(葆檜傘)
//#define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / 團唳蝓 / 醞措 PK縑憮 餌蜂衛, 瞳渠婦啗縑憮紫 嬴檜蠱檜 萄奧脹棻.
//#define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL           //2013.10.30 / 團唳蝓 / ⑦煎檜萄 蝶鑒嬪и 觼葬じ鏽 橫網葬じ 鼻鷓 蹺陛 塽 熱薑
//#define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / 團唳蝓 / 嘐暮頂縑憮 綠薑鼻 謙猿 �� 營蕾樓ж賊, 嘐暮 頂 謙猿脹 嬪纂煎 寡纂脹棻.
//#define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / 團唳蝓 / 賬縑 KISS蒂 稱檣棻. 憮幗 識 翕蕾濠熱蒂 瞪歎и棻.
//#define _NA_000000_20140103_CHANGE_DASH_AND_PIERCE_SKILL_MOVE     // 2014.01.03 / 團唳蝓 / 給霞婦鱔蝶鑒曖 檜翕籀葬蒂 DamageAbility陛 嬴棋, QuickStep橫網葬じ煎 渠羹и棻.
//#define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / 團唳蝓 / 跨蝶攪陛 寰瞪雖渠煎 菟橫陛賊 '寞��'鼻鷓陛 嬴棲塭 '葬欐'鼻鷓煎 瞪�納炵絲� и棻.
//#define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / 團唳蝓 / thrustState(傚寥鼻鷓)檜�� 綠摹奢 跨蝶攪陛 奩問擊 ж雖 彊朝 僥薯 籀葬
//#define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / 團唳蝓 / 嬴檜蠱 儅撩衛, 詭賅葬й渡縑憮 憮幗陛 避朝 僥薯煎, 螃粽薛おヴ婁 詭賅葬й渡擊 夥脯獄棻.
//#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / 團唳蝓 / 臢蝶お煎斜陛 ж瑞欽嬪縑憮 衛除欽嬪煎 だ橾擊 儅撩ж紫煙 滲唳
//#define _NA_007053_20140210_BUG_SUMMONED_MONSTER_ATTACKSPEED      // 2014.02.10 / 團唳蝓 / 模�紡鶺� 奢問樓紫陛 說塭雖朝 幗斜 熱薑
//#define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / 團唳蝓 / 餌蜂 衛, 寞橫掘 頂掘紫陛 0檜賊 雪擎�捕�煎 濰綠陛 模資ж朝 ぬ割じ 薯剪
//#define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / 團唳蝓 / 醞措 鬼繹賅萄 幗斜 婦溼 煎斜晦煙
//#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / 團唳蝓 / 頂掘紫0檜釭 蝶囌滲�風� 鼠晦 綠�側瘓� 衛, 鼠晦蒂 濰雜п薯и 鼻鷓煎 虜萇棻.
//#define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / 團唳蝓 / 望萄橾橾蠡蝶お蒂 '諫猿'ц朝等 爾鼻擊 嫡雖彊擎 鼻鷓縑憮 偽擎斜瑜蠡蝶お蒂 嫡朝 僥薯 熱薑
//#define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / 團唳蝓 / 褐敘 瞪濰賅萄 埤萄楝衛
//#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / 團唳蝓 / 衙蘆衛蝶蠱 葬ね饜葭
//#define _NA_007112_20140312_MODIFY_PROCESSRECEIVEDPACKET            //2014.03.12 / 團唳蝓 / ProcessReceivedPacketл熱縑憮 ぬ韃籀葬 褒ぬ衛 false葬欐腎朝 睡碟 薯剪
//#define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / 團唳蝓 / 膠紫辦 蝶鑒 葬景橡
//#define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / 團唳蝓 / 頂掘紫0檣 嬴檜蠱 濰雜陛棟
//#define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / 團唳蝓 / 膠紫辦曖 縑傘雖戮暮 等嘐雖奢衝滲唳
//#define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / 團唳蝓 / GM貲滄橫 蹺陛/熱薑
//#define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / 團唳蝓 / 耀掘蟾渠 溯漣滌 爾鼻雖晝衛, 溯漣高擊 const.h煎 貍魚棻
//#define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / 團唳蝓 / DB煎斜 望萄囀檣晦睡
//#define _NA_007239_20140521_BUG_GUILDRELATION_COUNT                          // 2014.05.21 / 團唳蝓 / 望萄婦啗婦溼 譆渠纂 熱薑
//#define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / 團唳蝓 / 嬴檜蠱褻м 撩奢 晦煙 衛, ItemNum擎 檣漸饜葬 識 偃熱陛 嬴棋, 褻м撩奢и 偃熱煎 и棻.
//#define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / 團唳蝓 / 嫦酈葬模�紡� 棟溘纂 啗骯衝 螃盟 熱薑
//#define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / 團唳蝓 / 鼻鷓睡罹 お葬剪陛 裘螃粽薛おи纔 稱艘擊陽虜 濛翕и棻
//#define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / 團唳蝓 / 議衛憧曖 嬴檜蠱 掘衙, 摹僭婦溼 DB煎斜 晦煙ж紫煙 蹺陛
//#define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / 團唳蝓 / 埤萄楝衛縑憮 埤萄ん檣お 滲翕餌о擊 DB煎斜縑 陴曹棻
//#define _DEV_007390_20140703_AUTH_EXCEPTION_VERSIONCHECK               // 2014.07.03 / 團唳蝓 / 偃嫦,餌頂憮幗縑憮 Щ煎饜屬 幗瞪羹觼 鼠褻勒 鱔婁衛鑑棻.
//#define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / 團唳蝓 / 紫瞪橾橾蠡 蹺陛
//#define _NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT                       // 2014.07.10 / 團唳蝓 / AC 熱ч陛棟�蝦� 棺葬晦
//#define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / 團唳蝓 / 營濛營猿陛 濰綠橾 唳辦 '檣疆お'縑 婦溼脹 褻勒 滲唳
//#define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / 團唳蝓 / 唸薑�� 夥紱晦
//#define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / 團唳蝓 / DB望萄煎斜 濛機
//#define _NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN                       // 2014.09.16 / 團唳蝓 / 檜鼻鼻鷓 橫ぷ棻遴 蹺陛
//#define _NA_007667_20141001_WITCHBLADE_SKILL                            // 2014.10.01 / 團唳蝓 / 嬪纂綰溯檜萄 蝶鑒濛機
//#define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION                   // 2014.10.27 / 團唳蝓 / 賅萇 檣錳曖 殮濰嬴檜蠱 匐餌蒂 ж朝 襄顫殮 蹺陛
//#define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / 團唳蝓 / ぬ韃滲褻縑 曖и AC爾鼻 鼠и嫡晦僥薯 籀葬
//#define _NA_007827_20141117_ADD_ACCESSORY_TYPE                              // 2014.11.14 / 團唳蝓 / 學撮餌葬 顫殮 蹺陛
//#define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / 團唳蝓 / だじ鼻鷓縑憮 議葛攪 鼻鷓(HP,MP,SD,FP) 機等檜お 寞衝 滲唳
//#define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / 團唳蝓 / ゆ紫 模賅顫殮 11廓擊 噩熱氈紫煙и棻
//#define _NA_007909_20141218_SUNLOG_QUESTLIST                                  // 2014.12.18 / 團唳蝓 / 臢蝶お煎斜辨, 蠡蝶お 爾鼻嫡晦 褒ぬж賊 濠褐檜 嫡擎 蠡蝶お葬蝶お 囀萄 賅舒轎溘
//#define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / 團唳蝓 / GM貲滄橫, 唳я纂檜漸お 薯橫ж晦
//#define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                                // 2015.01.15 / 團唳蝓 / 橫雯牆萄蒂 м撩п紫 唸婁僭擎 楠渾戲煎 釭螃紫煙 熱薑
//#define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / 團唳蝓 / 跨蝶攪藹奢 檜漸お衙棲盪煎 薯橫
//#define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / 團唳蝓 / 觼葬蝶驍ゃ溯蝶,察雖嬴檜蠱 衛除虜猿衛 薯渠煎 п薯п輿雖彊朝僥薯
//#define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / 團唳蝓 / Ы溯檜橫郭葬 諄遺縑憮 и寞縑 避朝唳辦 羹觼ж堅 煎斜煎 晦煙и棻
//#define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / 團唳蝓 / 跨お凜檣辨 臢蝶お煎斜, и寞等嘐雖僥薯 馬衛辨
//#define _NA_008069_20150210_ADD_SPA_SYSTEM                                       // 2015.02.10 / 團唳蝓 / 螞繭衛蝶蠱 蹺陛
//#define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / 團唳蝓 / 溯漣機AC檜 唳辦 '檜瞪溯漣' AC紫 偽檜 匐儀п憮 籀葬ж紫煙и棻
//#define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / 團唳蝓 / 嬴檜蠱 檜翕婦溼 僥薯 熱薑
//#define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / 夥檜蝶模萄偽擎 雖樓瞳戲煎 翕濛ж朝 蝶鑒檜 葆撿驕�素� 隸筒睦 隸陛 啗骯縑 僥薯蒂 嫦儅衛鑑棻
//#define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / 跨蝶攪陛 寰瞪雖渠縑憮 敝�窈褗捂� 跤瓊擊唳辦 蘿溯んお衛麵幗萼棻
//#define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / 棻觼螃粽塭檜お偽擎 'Effect'蒂 餌辨ж朝 蝶鑒檜 餌辨濠陛 避擊唳辦 偽檜 餌塭雖紫煙 и棻
//#define _NA_000000_20150507_TIME_DISPLAY                                              // 2015.05.07 / 團唳蝓 / agentserver, 蝶む檜蝶夥 援腦賊 衛除薑爾 嗥辦晦
//#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / 團唳蝓 / 蠡蝶お幗斜婦溼濛機, dbЩ煎衛盪諦 ぬ韃除 諒觼褻瞰
//#define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / 團唳蝓 / 寨楛蝶濛機擊 嬪п 贗塭縑 瞪癱薑爾 瞪殖ж晦
//#define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / 團唳蝓 / 孺萄褒萄 �膩�陛 鬼薯橫鷗 詳�鰽絲珌挬�
//#define _NA_008334_20150608_SONNENSCHEIN                                                  // 2015.06.08 / 團唳蝓 / 跨お凜檣 PvE, 模喧凜檣
//#define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                                       // 2015.07.03 / 團唳蝓 / 衛除薯 ゆ嬴檜蠱 萄奧腆 衛, charge sub type 1廓紫 陛棟ж紫煙 熱薑
//#define _NA_008456_20150819_ETHER_BULLET_PVP_CHECK                                        // 2015.08.19 / 團唳蝓 / 縑纔腦驕�素� PvP羹觼蒂ж罹 PvP衛 熱榆檜 還雖 彊紫煙 и棻
//#define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                          // 2015.09.04 / 團唳蝓 / ⑦煎檜萄蝶б 鼻鷓縑憮 BACKSTEP檜翕擎 薑賊殖葬晦諦 偽擎 樓紫蒂 陛霞棻
//#define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                                        // 2015.09.09 / 團唳蝓 / 蝶Я葩葬離雖, 匐晦 ヴ 醱瞪鼻鷓縑憮紫 蝶鑒餌辨 陛棟
//#define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT                               // 2015.09.09 / 團唳蝓 / 奢樓 300薯и п薯
//#define _NA_008483_20150910_REMOVE_DAYLIGHT_TIME                                          // 2015.09.10 / 團唳蝓 / 措頂縑憮 橾惜 瞰擒 衛除 0戲煎 撮たп幗萼棻
//#define _NA_008486_20150914_TOTAL_BALANCE                                                 // 2015.09.14 / 團唳蝓 / 寨楛蝶濛機
//#define _NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER                                      // 2015.09.23 / 團唳蝓 / 啪歜憮幗縑憮 晦煙ж朝 煎斜嬴醒 煎斜 蹺陛
//#define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                                       // 2015.10.01 / 團唳蝓 / 奢問濠望萄薑爾 薯剪, 薄滄瞪 餌瞪憲葡 賅萇в萄嶸盪縑啪 爾頂啪ж晦
//#define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                                           // 2015.10.27 / 團唳蝓 / 縣葬お4 褫暮 蹺陛 濛機
//#define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                                           // 2015.11.05 / 團唳蝓 / 檜樓奢衝 滲唳
//#define _NA_000000_20151109_BUG_SSQ_HANDLER_NULL                                              // 2015.11.09 / 團唳蝓 / ssq handler 婦溼 幗斜
//#define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                                            // 2015.11.16 / 團唳蝓 / 偃檣鼻薄 匐儀 衛, 葆擊薯и 薯剪
//#define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME                                 // 2015.11.26 / 團唳蝓 / 囀蝶え嬴檜蠱 晦除撲薑婦溼 熱薑
//#define _NA_008606_20151222_MODIFY_GUILD_LEVEL                                              // 2015.12.22 / 團唳蝓 / 望萄詹幗 驍黴 衛, 溯漣棻遴 腎雖 彊紫煙 熱薑
//#define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / 團唳蝓 / 億 囀蝶え蝸煜 蹺陛(爾褻囀蝶え)
//#define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                                      // 2015.12.02 / 團唳蝓 / 檜翕蝶鑒 煎霜 嬪纂滲區
//#define _NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION                                    // 2016.01.29 / 團唳蝓 / 撮お嬴檜蠱 褫暮 瞳辨褻勒 滲唳
//#define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST                                       // 2016.02.04 / 團唳蝓 / 瞳辨醞檣 撮お濰綠�膩�蒂 評煎 盪濰衛麵菩棻
//#define _NA_008706_20160229_TEXTLOG_PK                                                      // 2016.02.29 / 團唳蝓 / 馬衛渠鼻 pk衛 臢蝶お煎斜
// 團唳蝓 部
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  辦�ㄟ�  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 辦�ㄟ� 衛濛
//#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION												// 2015.10.29 / 辦�ㄟ� / 橾橾 望萄 嘐暮 晦棟 掘⑷
//#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE												// 2016.01.15 / 辦�ㄟ� / NPC 鼻薄 啗骯 奢衝 滲唳
//#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2											// 2016.02.15 / 辦�ㄟ� / 萄楚堆釭檜お 辨 滲褐(懍萄楚堆お楠蝶イ10溯漣) 蝶鑒 餌辨 衛 檜樓 棟溘纂 瞳辨 寰腎朝 幗斜 熱薑
//#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS												// 2016.02.24 / 辦�ㄟ� / 顫問 偃羹熱 滲唳 蹂羶(蝶顫橾囀萄 蹺陛)
//#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE									// 2016.03.14 / 辦�ㄟ� / 陛п 等嘐雖 HP/SD 瞪�� 衛 褻勒 蹺陛 濛機
//#define _NA_008765_20160405_DISCONNECTION_REASON_LOG												// 2016.04.05 / 辦�ㄟ� / 鐘喀暮 莒啣擊 唳辦 檜嶸蒂 煎斜縑 陴晦朝 濛機
//#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE									// 2015.03.29 / 辦�ㄟ� / 醞措 AC 爾鼻 縣葬お 嬴檜蠱 瞪辨 剪楚顫殮 蹺陛(剪楚薯и + PK萄奧 陛棟)
//#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / 辦�ㄟ� / 爾�˙鬌� 檜鼻⑷鼻 熱薑
// 辦�ㄟ� 部
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收  歜鼻熱  收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 歜鼻熱 衛濛
//#define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.13 /歜鼻熱 /(跨お凜檣) domination 蝶觼董お 塽 だ憮 熱薑, domination type 蹺陛, 陝檣 撩奢衛 欐 螃幗 蹺陛, Map npc 螃粽薛お 羹溘�蛹�
//#define _NA_000000_20140402_MODIFY_PERKMANAGER_POSSIBLE_TO_USE_WHILE_BATTLE_GROUND_RESERVATION //2014.04.02 /歜鼻熱 /瞪濰蕨擒 醞縑紫 醴蝶籤 ぬ衛粽 蝶鑒 滲唳檜 陛棟ж紫煙 熱薑.
//#define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 歜鼻熱 / 跨お凜檣 奢撩瞪 蟾晦�� 塽 爾鼻 雖晝 熱薑
// 歜鼻熱 部
//收收收收收收收收收收收收收收收收收收收     部      收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//--------------------------------------------------------------------------------------------------
//! 2011.3.4 / i4u4me / 嶸貲熱 衛濛
//#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / 縣葬お2 ヴ撢 だお 掖�� ⑽衝縑憮 碟葬 

//#define _NA_000000_20120410_SPEEDHACK
//#define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
//#define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    //!< 避歷擊陽 棟溘纂 螢塭陛雖 彊啪 л
//#define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING                   //!< 陳偃 爾歜 罹睡
//#define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER           //!< 嶸營錳 嘐暮頂 瓣餵 晦棟 お葬剪 蹺陛
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                  //!< 瞪濰

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// arycoat :
//#define _HARDMODE_TRIGGER_PRINT_ // ж萄賅萄 蝶觼董お 濛撩霤堅辨 お葬剪 碟戮 だ橾 儅撩晦. - arycoat.2011.12.06
//#define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / 薑⑷撩,檜鷓遽 / 望萄 囀檣 濛機
//#define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / 薑⑷撩,檜鷓遽 / 望萄 葬景橡
//#define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / 薑⑷撩,檜鷓遽 / 望萄璽堅 塽 偃檣璽堅 蝸煜偃熱 �挫�
//#define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / 薑⑷撩,檜紫⑽ / 偃檣鼻薄 匐儀 衛蝶蠱
//#define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / 薑⑷撩 / 漯雖 嬴檜蠱
//#define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / 薑⑷撩 / 衙霜褒萄 MP模賅榆 滲唳
//#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / 薑⑷撩 / 雖羲薄滄瞪 ぬ寡 ぬ割じ 瞳辨
//#define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / 薑⑷撩 / 橾橾蠡蝶お 蟾晦�� 嬴檜蠱
//#define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / 薑⑷撩,歎雙謙 / 雖棟⑽NPC
//#define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // 薑⑷撩,歎雙謙 / 跨蝶攪紫馬



//收收收收收收收收收收收收收 LAST SECTION(Shared) - Dependency Chain 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SAMPLE>
//		#if defined( A_EX ) && !defined( A )
//			#error "Impossible Build!! - Reason: Violation Dependency Chain"
//		#endif
//
//	<EXPRESSION>
//		{ ╯(A_EX) | (A') ↗ (A_EX) }   <--   A_EX 晦棟擎 A晦棟縑 謙樓瞳檜棻.
//		if (0) then IMPOSSIBLE,
//			(A') = (A) + (A_EX)
//			(A) = (A') - (A_EX)
//			(0) = (A_EX) - (A')
//		
//	<USEFUL SECTION>
//		1) A晦棟曖 �挫�
//		2) A晦棟擊 餌辨и 蹺陛 晦棟
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
	#if !defined(__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__)	// 晦顫 蹺陛й 匙 瓊嬴憮 蹺陛й 匙.
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