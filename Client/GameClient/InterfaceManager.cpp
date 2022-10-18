#include "SunClientPrecompiledHeader.h"
#include "IconImage.h"
#include "interfacemanager.h"
#include "SolarDialog.h"
#include "ChatDialog.h"
#include "InventoryDialog.h"
#include "HeroEquipmentContainer.h"
#include "RadarInfoParser.h"
#include "RadarDialog.h"
#include "HotKeyManager.h"
#include "QuickContainer.h"
#include "CharacterDialog.h"
#include "MainDialog.h"
#include "MainMenuOptionDlg.h"
#include "HeroGaugeDialog.h"
#include "Camera.h"
#include "MonsterGauge.h"
#include "LoadScene.h"


#include "CharSelectDialog.h"
#include "CharSelectFontDialog.h"
#include "InputNumDialog.h"
#include "ShopDialog.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "Map.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "Char_SkillDialog.h"
#include "SystemDialog.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Application.h"
#include "RewardDialog.h"
#include "StateInfoParser.h"
#include "AskDialog.h"
#include "TargetDialog.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "ItemInfoParser.h"
#include "SelectCompositeMenu.h"
#include "SkillInfoParser.h"
#include "SkillInventoryStoreParser.h"
#include "Mouse.h"
#include "SCItemSlot.h"
#include "chatmenu.h"
#include "RadarPlusDialog.h"
#include "ItemFormula.h"
#include "SocketOptionParser.h"
#include "SocketOptionDescParser.h"
#include "StyleContainer.h"
#include "GameFunc.h"
#include "BattleScene.h"
#include "Hero.h"
#include "QuickSlot.h"
#include "CastDialog.h"
#include "CastLeaveFieldDialog.h"
#include "StyleSlot.h"
#include "StyleInfoParser.h"
#include "RadarFullDialog.h"
#include "BattleLobby.h"
#include "MouseHandler.h"
#include "OptionMenuDialog.h"
#include "OptionDialog.h"
#include "OptionGameDialog.h"
#include "OptionKeyHelpDialog.h"
#include "ResourceManager.h"
#include "HeroActionInput.h"
#include "CharacterScene.h"
#include "CreateCharBottomDialog.h"
#include "CreateCharLeftDialog.h"
#include "CreateCharSelect.h"
#include "PlayerAttributes.h"
#include "MouseCursor.h"
#include "ItemManager.h"
#include "HeroEquipmentContainer.h"
#include "Clone.h"
#include "CloneManager.h"
#include "DlgPosInfoParser.h"
#include "SkillQueueDialog.h"
#include "EventInventoryDialog.h"
#include "NPCPortalDialog.h"
#ifdef __NA_001358_ENCHANT_UNIFICATION
#else
#include "EnchantDialog2.h"
#endif //__NA_001358_ENCHANT_UNIFICATION

//
// IG: Interface Groups includes
//
#include "uiSkillTree/uiSkillTree.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "uiSkillAction/uiNewSkillAction.h"

#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiPartyOption/uiPartyOption.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiTrade/uiTrade.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "uiLoginBack/uiLoginBack.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginNotice/uiLoginNotice.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiLogin15Year/uiLogin15Year.h"
#include "uiSystemVerify/uiSystemVerify.h"
#include "uiSystemConfirm/uiSystemConfirm.h"
#include "uiSystemIdNumberConfirm/uiSystemIdNumberConfirm.h"
#include "uiSystemInputMessageConfirm/uiSystemInputMessageConfirm.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildMain/uiGuildMain.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiGuildHostility/uiGuildHostility.h"
#include "uiGuildOneSideHostility/uiGuildOneSideHostility.h"
#include "uiGuildRelationAsk/uiGuildRelationAsk.h"
#include "uiGuildAllianceRequest/uiGuildAllianceRequest.h"
#include "uiLoginMan/uiLoginMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/uiGuildMan.h"
    #include "GuildSystem/GuildSystemData.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiCharPopupMan/uiCharPopupMan.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemETCCompositeDlg.h"
#include "tiredsystemdialog.h"
#include "helpmessageparser.h"
#include "HeroActionInput.h"
#include "SummonSkillContainer.h"
#include "OptionSoundDialog.h"
#include "uiEventMan/uiEventMan.h"

#ifdef _KIKI_UI_WEB_DIALOG
	#include "uiWebDialogMan/uiWebDialogMan.h"
	#include "uiWebDialog/uiWebDialog.h"
#endif

#include "uiItemDiv/uiItemDiv.h"
#include "uiItemDivMan/uiItemDivMan.h"
#include "EnduranceDialog.h"
#include "SystemChatDialog.h"
#include "uiWayPointMan/uiWayPointMan.h"
#include "uiPcbangPalyTimeMan/uiPcbangPlayTimeMan.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiSocketRemoveMan.h"

#ifdef __ADD_GAME_HELP_SYSTEM
	#include "uiGameHelpSystemMan.h"
#endif

#include "uiFateItemSystemMan/uiFateItemSystemMan.h"

#pragma warning ( push )
#pragma warning ( disable : 4244)
 
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "uiQuickContainerMan/uiQuickContainerMan.h"
#include "uiChatMan/uiChatMan.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "uiAssistMan/uiAssistMan.h"
#include "uiMenuMan/uiMenuMan.h"
#include "uiHeroGaugeMan/uiHeroGaugeMan.h"

#include "uiBankMan/uiBankMan.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "uiTargetMan/uiTargetMan.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "uiRewardMan/uiRewardMan.h"
#include "uiEnchantMan/uiEnchantMan.h"
#include "uiCompositeMan/uiCompositeMan.h"
#include "uiSummonSkillMan/uiSummonSkillMan.h"
#include "uiCastMan/uiCastMan.h"
#include "uiBattleLobbyMan/uiBattleLobbyMan.h"
#include "uiSkillQueueMan/uiSkillQueueMan.h"
#include "uiNPCPortalMan/uiNPCPortalMan.h"
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiEnduranceMan/uiEnduranceMan.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiShopMan/uiShopMan.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiPasswordMan/uiPassWordMan.h"


#ifdef __CN_269_MATRIX_CARD_AUTH
	#include "uiMatrixCard/uiMatrixCardMan.h"
#endif //__CN_269_MATRIX_CARD_AUTH

#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "ui_XMas_Confirm/ui_XMas_Confirm.h"
#include "ui_XMas_SantaDlg/ui_XMas_SantaDlg.h"
#include "ui_XMas_DonationConfirm/ui_XMas_DonationConfirm.h"
#include "ui_XMas_DonationReward/ui_XMas_DonationReward.h"
#include "ui_XMas_SoloCoupleReward/ui_XMas_SoloCoupleReward.h"
#include "ui_XMas_SantaReward/ui_XMas_SantaReward.h"
#include "ItemShareSystem.h"
#include "uiGuildWareHouseRight/uiGuildWareHouseRight.h"
#include "uiGuildWareHouseLog/uiGuildWareHouseLog.h"
#include "uiChangeGuildName/uiChangeGuildName.h"

#ifdef __NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN_2
	#include "Pet.h"
#endif//__NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN_2

#ifdef _YJ_FISHING_SYSTEM_000933
	#include "uiFishingMan/uiFishingMan.h"
	#include "uiFishingShop/uiFishingShop.h"
	#include "uiFishingTrade/uiFishingTrade.h"
	#include "uiFishingAction/uiFishingAction.h"
#endif //_YJ_FISHING_SYSTEM_000933

#include "uiItemStatChangeSysMan/uiItemStatChangeSysMan.h"
#include "uiSSQMan/uiSSQManager.h"
#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#include "uiPetMan/uiPetMan.h"
#include "Rider.h"

#include "uiLottoMan/uiLottoMan.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationTowerAreaDlg/uiDominationTowerAreaDlg.h"
#include "ConstantDialogListParser.h"

#include "DamageMesh.h"

#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "StatusIconManager.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiHelpMissionTimer/uiHelpMissionTimer.h"
#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"
#include "Item.h"
#include "PacketRequestManager.h"

#include "ItemCooltimeManager.h"
#include "uiPetGauge/uiPetGauge.h"
#include "uiDominationZoneMarkDlg/uiDominationZoneMarkDlg.h"
#include "uiMissionObjectMan/uiMissionObjectMan.h"
#ifdef _YMS_AUTOBOT_GAME
    #include "Interface/uiAutobot/uiAutobotPlay.h"
    #include "Interface/uiAutobot/uiAutobotMan.h"
#endif //_YMS_AUTOBOT_GAME

    #include "uiSocketMan/uiSocketMan.h"
#include "SoundEffect.h"

#ifdef _DEV_VER
    #include "uiGMWindowMan/uiGMWindowMan.h"
#endif//_DEV_VER// DH_GM_WINDOW
#include "NPCInfoParser.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "Interface/uiConvenienceStore/uiConvenienceStoreMan.h"
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiGuideSystemMan/uiGuideSystemMan.h"

#include "uiMenuMan/uiTutorialSkip.h"

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "Map.h"
#endif//_DH_BATTLEZONE2_
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
  #include "DiceCastManager.h"
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING


#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#include "uiQuestList/uiQuestList.h"
#endif//_NA_004644_20110506_QUEST_NAVIGATION

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiTextureViewerMan/uiTextureViewerMan.h"
#endif
#endif

#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiFPGaugeMan/uiFPGaugeMan.h"
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningMan.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionMan.h"
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "uiNotificationsSystemMan/uiNotificationsSystemMan.h"
#include "uiNotificationsSystemDialog/uiNotificationsSystemDialog.h"
#include "uiNotificationsSystemListDialog/uiNotificationsSystemListDialog.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMiniGameMan/uiMiniGameMan.h"
#endif // _NA_008405_20150716_MINIGAME_BINGO

#include "uiOptionMan.h"


//------------------------------------------------------------------------------
/**
*/
WzID InterfaceManager::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("0010"),   //DIALOG_LOGIN,
    StrToWzID("0020"),   //DIALOG_SERVER_SELECTION,
	StrToWzID("0410"),   //DIALOG_EQUIP,
	StrToWzID("0411"),	 //DIALOG_INVENTORY,
	StrToWzID("0412"),	 //DIALOG_EVENT_INVENTORY
	StrToWzID("0200"),	 //DIALOG_CHAT,
	StrToWzID("0201"),	 //DIALOG_CHATMENU
	StrToWzID("0230"),	 //DIALOG_QUICK,
	StrToWzID("0260"),	 //DIALOG_RADAR,
	StrToWzID("0261"),	 //DIALOG_RADAR_PLUS,
	StrToWzID("0460"),	 //DIALOG_RADAR_FULL,
    StrToWzID("0210"),	 //DIALOG_HEROGAUGE,
	//StrToWzID("0211"),   //DIALOG_HEROGAUGECONTROL
	StrToWzID("0240"),	 //DIALOG_MAIN,
	StrToWzID("0241"),	 //DIALOG_MAIN_OPTION,
	
    // from uiBase
    StrToWzID("0010"),  //DIALOG_LOGIN_BACK,
    StrToWzID("0011"),  //DIALOG_LOGIN_ACCOUNT,
    StrToWzID("0012"),  //DIALOG_LOGIN_NOTICE,
    StrToWzID("0020"),  //DIALOG_LOGIN_SERVER_LIST,
    StrToWzID("0022"),  //DIALOG_LOGIN_CHANNEL_LIST,
	StrToWzID("0013"),	//DIALOG_LOGIN_15YEAR
    StrToWzID("0720"),  //DIALOG_TRADE,
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
	StrToWzID("0108"),  //DIALOG_TRADE_NUMBER_KEYBOARD,
#endif

#ifndef _NA_000000_20130114_RENEWER_UI
	StrToWzID("0453"),  //DIALOG_OPTION_MENU,
	StrToWzID("0450"),  //DIALOG_OPTION_SYSTEM,
	StrToWzID("0451"),  //DIALOG_OPTION_GAME,
	StrToWzID("0454"),  //DIALOG_OPTION_KEYHELP,
	StrToWzID("0455"), 	//DIALOG_OPTION_SOUND,
#else
    StrToWzID("0451"),  //DIALOG_OPTION,
#endif
    StrToWzID("0456"),  //DIALOG_OPTION_KEYBINDING,

	StrToWzID("0480"),  //DIALOG_VENDOR_SELL,
    StrToWzID("0481"),  //DIALOG_VENDOR_BUY,
    StrToWzID("0880"),  //DIALOG_VENDOR_POPUP,
    StrToWzID("0483"),  //DIALOG_VENDOR_TITLE_POPUP,
    StrToWzID("0610"),  //DIALOG_BATTLEZONE_LIST,
    StrToWzID("0620"),  //DIALOG_BATTLEZONE_CREATE,
    StrToWzID("0622"),  //DIALOG_BATTLEZONE_PASSWORD,
    StrToWzID("0650"),  //DIALOG_BATTLEZONE_RELAY,
    StrToWzID("0632"),  //DIALOG_BATTLEZONE_READY_MISSION_CFG,

    StrToWzID("0631"),  //DIALOG_BATTLEZONE_READY_HUNTINGCFG,
    StrToWzID("0633"),  //DIALOG_BATTLEZONE_READY_CHUNTINGCFG,

	StrToWzID("0670"),	//DIALOG_BATTLEZONE_CHUNTING_WIN
    StrToWzID("0671"),	//DIALOG_BATTLEZONE_CHUNTING_SCORE_BOARD
	StrToWzID("0672"),	//DIALOG_BATTLEZONE_CHUNTING_DOING

    StrToWzID("0220"),  //DIALOG_PARTY_LEADER
    StrToWzID("0221"),  //DIALOG_PARTY_MEMBER1
    StrToWzID("0222"),  //DIALOG_PARTY_MEMBER2
    StrToWzID("0223"),  //DIALOG_PARTY_MEMBER3
    StrToWzID("0224"),  //DIALOG_PARTY_MEMBER4
    StrToWzID("0225"),  //DIALOG_PARTY_MEMBER5
    StrToWzID("0226"),  //DIALOG_PARTY_MEMBER6
    StrToWzID("0227"),  //DIALOG_PARTY_MEMBER7
    StrToWzID("0228"),  //DIALOG_PARTY_MEMBER8
    StrToWzID("0229"),  //DIALOG_PARTY_MEMBER9
    StrToWzID("1220"),  //DIALOG_PARTY_OPTION
    StrToWzID("0730"),  //DIALOG_GUILD_CREATE
    StrToWzID("0733"),  //DIALOG_GUILD_WINDOW
    StrToWzID("0731"),  //DIALOG_GUILD_MENU
    StrToWzID("0732"),  //DIALOG_GUILD_MAIN
    StrToWzID("0870"),  //DIALOG_GUILD_INPUT_MESSAGE
    StrToWzID("0821"),  //DIALOG_SYSTEM_VERIFY
    StrToWzID("0820"),  //DIALOG_SYSTEM_CONFIRM
	StrToWzID("0890"),  //DIALOG_SYSTEM_IDNUMBER_CONFIRM
	StrToWzID("0870"),  //DIALOG_SYSTEM_INPUTMSG_CONFIRM,

	StrToWzID("D102"),	 //DIALOG_SEARCH_ID,
	StrToWzID("D103"),	 //DIALOG_INPUT_PW,
	StrToWzID("0041"),	 //DIALOG_CHARSELECT,
	StrToWzID("0040"),	 //DIALOG_FONT_CHARSELECT
	StrToWzID("0440"),	 //DIALOG_SHOP
	StrToWzID("0840"),	 //DIALOG_INPUT_NUM,

	StrToWzID("0600"),	 //DIALOG_BANK,   
	StrToWzID("0400"),	 //DIALOG_CHAR_STATUS,
    StrToWzID("0800"),   //DIALOG_SYSTEM,
    StrToWzID("D301"),   //DIALOG_HELP,
	StrToWzID("9999"),	 //DIALOG_MISSION_SELECTION
	StrToWzID("0030"),	 //DIALOG_CREATE_CHARACTER
	StrToWzID("0820"),	 //DIALOG_ASK				
	StrToWzID("0810"),	 //DIALOG_WARNING
	StrToWzID("0270"),	 //DIALOG_TARGET
	StrToWzID("0300"),	 //DIALOG_SUMMON_TARGET

//	StrToWzID("0800"),	 //DIALOG_TOOLTIP
	StrToWzID("0830"),	 //DIALOG_REWARD
	StrToWzID("0710"),	 //DIALOG_ENCHANT
	StrToWzID("4419"),	 //DIALOG_RANKDOWN

	StrToWzID("0712"),	 //DIALOG_SELECT_COMPOSITE_MENU
	StrToWzID("0290"),	 //DIALOG_STYLE_CONTAINER
	StrToWzID("0301"),	 //DIALOG_SUMMON_SKILL_CONTAINER
	StrToWzID("0281"),   //DIALOG_DELAY_CASTING
	StrToWzID("0280"),   //DIALOG_CAST 
	StrToWzID("0630"),	 //DIALOG_BATTLE_LOBBY
	StrToWzID("0032"),	 //DIALOG_CREATECHARACTER_LEFT
	StrToWzID("0033"),	 //DIALOG_CREATECHARACTER_BOTTOM
	StrToWzID("0031"),	 //DIALOG_CREATECHARACTER_SELECT
	StrToWzID("0034"),	 //DIALOG_CREATECHARACTER_ROTATE

	StrToWzID("0291"),	 //DIALOG_MAGAZINE
	StrToWzID("0640"),	 //DIALOG_PVP_SCORE
	StrToWzID("0425"),	 //DIALOG_SKILL_QUEUE
	
	StrToWzID("0520"),	 //DIALOG_NPC_PORTAL
	StrToWzID("0634"),	//DIALOG_BATTLE_CONFIG,

	StrToWzID("0271"),	//DIALOG_CHAR_POPUP_MENU,

	StrToWzID("0490"),	//DIALOG_HELP_MENU,
	StrToWzID("0459"),	//DIALOG_HELP_WINDOW,
	StrToWzID("0430"),		//DIALOG_HELP_ANNOUNCE_WINDOW,

	StrToWzID("0716"),	//DIALOG_ITEM_COMPOSITE,

	StrToWzID("0717"),	//DIALOG_ITEM_ETC_COMPOSE,

	StrToWzID("0212"),	//DIALOG_TIREDSYSTEM

	StrToWzID("0202"),	//DIALOG_VOICE_CHAT,

	StrToWzID("0921"),	//DIALOG_AREA_CONQUEST,
	
	StrToWzID("0831"),	//DIALOG_QUEST_COMMON_FORWARD,
	StrToWzID("0832"),	//DIALOG_QUEST_REWARD,
	StrToWzID("0833"),	//DIALOG_QUEST_LIST_MISSION,
	StrToWzID("0834"),	//DIALOG_QUEST_LIST_CONTENT,
	StrToWzID("0835"),	//DIALOG_QUEST_MISSION_LIST,
	StrToWzID("0836"),	//DIALOG_QUEST_TIME_INFO,
	StrToWzID("0837"),	//DIALOG_QUEST_AUTO_ACEEPT_ALRAM,	


	StrToWzID("0990"),	//DIALOG_EVENT_WINDOW,
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	StrToWzID("7317"),
#endif
	StrToWzID("0991"),	//DIALOG_EVENT_ITEM_SELECT,

	StrToWzID("0994"), //DIALOG_EVENT_X_MAS_CONFIRM,
	StrToWzID("0995"), //DIALOG_EVENT_X_MAS_SANTADLG,
	StrToWzID("0996"), //DIALOG_EVENT_X_MAS_DONATION_CONFIRM,
	StrToWzID("0997"), //DIALOG_EVENT_X_MAS_DONATION_REWARD,
	StrToWzID("0998"), //DIALOG_EVENT_X_MAS_SOLOCOUPLE_REWARD,
	StrToWzID("0999"), //DIALOG_EVENT_X_MAS_SANTA_REWARD,

#ifdef _KIKI_UI_WEB_DIALOG
	StrToWzID("0980"),	//DIALOG_WEB_DIALOG,
#endif

    StrToWzID("0721"),  //DIALOG_ITEM_DIV,

	StrToWzID("0231"),
	StrToWzID("0718"),	//DIALOG_ITEM_TOTAL_COMPOSITE,

	StrToWzID("0203"),
	StrToWzID("0521"),	//DIALOG_WAYPOINT,

	StrToWzID("0992"),

	StrToWzID("0900"),	//DIALOG_MISSION_RANK_TIMER,
	StrToWzID("0901"),	//DIALOG_MISSION_RANK_ENDING,
	StrToWzID("0903"),	//DIALOG_MISSION_RANK_LIST,
	StrToWzID("0902"),	//DIALOG_MISSION_RANK_GAMBLE,

	StrToWzID("0735"),	// DIALOG_GUILD_POINT_EXCHANGE
	StrToWzID("0734"), // DIALOG_GUILD_DONATE_UP


	StrToWzID("0736"),

	StrToWzID("0737"),

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	StrToWzID("0486"),
#endif

	StrToWzID("0501"),	//DIALOG_HELP_MISSION,

	StrToWzID("0485"),  //DIALOG_VENDOR_LIST

	StrToWzID("0662"),	// DIALOG_COMMUNITY_DLG
	StrToWzID("0665"),	//DIALOG_COMMUNITY_INFO,
	StrToWzID("0667"),	//DIALOG_COMMUNITY_READ_MAIL,
	StrToWzID("0666"),	//DIALOG_COMMUNITY_WRITE_MAIL,
	StrToWzID("0487"),  //DIALOG_VENDOR_SEARCH
	StrToWzID("0204"),		// DIALOG_CHAT_PROPERTY

	StrToWzID("0442"),		// DIALOG_CASH_ITEM_SHOP,
	StrToWzID("0444"),		// DIALOG_CASH_ITEM_SHOP_BUY,
	StrToWzID("0447"),		// DIALOG_CASH_ITEM_SHOP_BUY_CHECK,
	StrToWzID("0448"),		// DIALOG_CASH_ITEM_SHOP_BUY_COMPLETE,
	StrToWzID("4431"),		// DIALOG_CASH_ITEM_SHOP_CART,
	StrToWzID("0441"),		// DIALOG_CASH_ITEM_SHOP_NPC_POPUP,
	StrToWzID("4432"),		// DIALOG_CASH_ITEM_SHOP_PRESENT_DLG,
	StrToWzID("0445"),		// DIALOG_CASH_ITEM_SHOP_PRESENT_CHECK,
	StrToWzID("0446"),		// DIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE,
	StrToWzID("4412"),		// DIALOG_CASH_ITEM_SHOP_PACKAGE_DLG,
	StrToWzID("4413"),		// DIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK,
	StrToWzID("4414"),		// DIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE,

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	StrToWzID("4415"),		// DIALOG_CASH_ITEM_SHOP_MILEAGE_DLG,
	StrToWzID("4416"),		// DIALOG_CASH_ITEM_SHOP_MILEAGE_BUY,
	StrToWzID("4417"),		// DIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK,
	StrToWzID("4418"),		// DIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE,
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	StrToWzID("4420"),		// DIALOG_CASH_ITEM_SHOP_EXTENSION_BUY,
	StrToWzID("4421"),		// DIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE,
	StrToWzID("0993"),      // DIALOG_SOCKET_REMOVE

	StrToWzID("0432"),

	StrToWzID("0016"),		// DIALOG_PASSWORD,

#ifdef __CN_269_MATRIX_CARD_AUTH
	StrToWzID("0107"),      // DIALOG_MATRIX_CARD_AUTH
#endif //__CN_269_MATRIX_CARD_AUTH

	StrToWzID("0502"), //DIALOG_MISSION_RELAY
	StrToWzID("0635"), //DIALOG_CHANGE_ROOM_NAME //BattleLobby
	StrToWzID("0206"),
	StrToWzID("2210"), //DIALOG_PARTY_ITEM_SHARE_SYSTEM,
	StrToWzID("7323"), //DIALOG_GUILD_DUTY_VEST,
	StrToWzID("7322"), //DIALOG_GUILD_RIGHT_DELEGATION,
	StrToWzID("7327"), //DIALOG_GUILD_MEMBER_RIGHT_VIEW
	StrToWzID("7330"),	//DIALOG_GUILD_CORPS_CHANGE
	StrToWzID("7329"),  //DIALOG_GUILD_MARK_COMBINATION
	StrToWzID("7328"),	//DIALOG_GUILD_MARK_REGISTE_CONFIRM

	StrToWzID("7324"), //DIALOG_GUILD_ALLIANCE,
	StrToWzID("7325"), //DIALOG_GUILD_HOSTILITY,
	StrToWzID("7326"), //DIALOG_GUILD_ONE_SIDE_HOSTILITY,
	StrToWzID("7311"), //DIALOG_GUILD_RELATION_ASK,
	StrToWzID("0820"), //DIALOG_GUILD_ALLIANCE_REQUEST

	StrToWzID("0741"),	//DIALOG_FATE_ITEM_SYSTEM

	StrToWzID("0738"),	//DIALOG_GUILD_WAREHOUSE,
	StrToWzID("0739"),	//DIALOG_GUILD_WAREHOUSE_RIGHT,
	StrToWzID("7310"),	//DIALOG_GUILD_WAREHOUSE_LOG,

#ifdef _YJ_FISHING_SYSTEM_000933
	StrToWzID("0751"),  //DIALOG_FISHING_SHOP,
	StrToWzID("0752"),  //DIALOG_FISHING_TRADE,
	StrToWzID("0753"),  //DIALOG_FISHING_ACTION,
#endif //_YJ_FISHING_SYSTEM_000933

	StrToWzID("0524"),

    StrToWzID("4451"),

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
	StrToWzID("0613"),   //DIALOG_TEXTURE_VIEWER,  
#endif
#endif
};

const int MOVE_POINTER_RESOURCE_ID = SCRIPT_RESOURCE_ID_OFFSET + 120000;
const int AREA_POINTER_RESOURCE_ID = SCRIPT_RESOURCE_ID_OFFSET + 120001;
const int TARGET_POINTER_RESOURCE_ID = SCRIPT_RESOURCE_ID_OFFSET + 120002;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
const int DICE_RESOURCE_ID = SCRIPT_RESOURCE_ID_OFFSET + 120005;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING



//------------------------------------------------------------------------------ 
// RenderToRect을 사용할때의 라이트방향
WzVector LOCAL_SUNLIGHT_DIRECTION;
//------------------------------------------------------------------------------ 




void RenderUnitDrawOutline(CWzUnitDraw *pDraw,WzColor color , float fThickness)
{
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetColor(color);
	
	BOOL                bZWrite;
	bZWrite = g_pSunRenderer->IsEnableZWrite();
	ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	ENUM_ALPHABLEND eBlend = g_pSunRenderer->GetAlphaBlendMode();

	pDraw->SetRenderType(AR_INFLATION);
	pDraw->SetInflateFactor(fThickness);	

	g_pSunRenderer->SetAlphaBlendMode(AB_LIGHTING,FALSE);
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_BACK );
	g_pSunRenderer->EnableTexture(TRUE);

	g_pSunRenderer->RenderUnit( pDraw );

	g_pSunRenderer->SetZBufferWrite(bZWrite);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->SetAlphaBlendMode(eBlend,FALSE);

	pDraw->SetRenderType(AR_NORMAL);
	g_pSunRenderer->EnableTexture(TRUE);
}

//------------------------------------------------------------------------------
//! static function
//------------------------------------------------------------------------------ 
BOOL InterfaceManager::backup_old2dmode;
BOOL InterfaceManager::backup_oldrealsize;
BOOL InterfaceManager::backup_zwrite;
BOOL InterfaceManager::backup_ztest;
BOOL InterfaceManager::backup_fog;
DWORD           InterfaceManager::backup_alpharef;
WzColor         InterfaceManager::backup_oldcolor;
ENUM_CMP_FUNC   InterfaceManager::backup_alphafunc;
ENUM_CMP_FUNC	InterfaceManager::backup_zfunc;
ENUM_ALPHABLEND	InterfaceManager::backup_alphablendmode;
ENUM_CULL_FACE  InterfaceManager::backup_cullfacemode;
ENUM_MESH_DRAW  InterfaceManager::backup_drawmode;
//------------------------------------------------------------------------------ 
void InterfaceManager::Begin2DMode(RECT* viewport, 
                                   DWORD clear_flags /*= CR_DEPTH*/, 
                                   WzColor* clear_color /*= NULL*/, 
                                   float* clear_z /*= NULL*/, 
                                   DWORD* clear_stencil /*= NULL*/)
{
    backup_drawmode = g_pSunRenderer->GetMeshDrawMode();
    backup_oldcolor = g_pSunRenderer->GetDiffuseColor();
    backup_cullfacemode = g_pSunRenderer->GetCullFaceMode();	
    backup_zfunc = g_pSunRenderer->GetZFunc();
    backup_alphablendmode = g_pSunRenderer->GetAlphaBlendMode();
    backup_zwrite = g_pSunRenderer->IsEnableZWrite();
    backup_ztest = g_pSunRenderer->IsEnableZTest();
    backup_fog = g_pSunRenderer->IsFogEnabled();
    g_pSunRenderer->GetAlphaTestMode(&backup_alpharef, &backup_alphafunc);
    g_pSunRenderer->Check2DMode(&backup_old2dmode, &backup_oldrealsize);

    g_pSunRenderer->Set2DMode(FALSE);
    g_pSunRenderer->SetZBufferWrite(TRUE);
    g_pSunRenderer->SetZBufferTest(TRUE);
    g_pSunRenderer->EnableFog(FALSE); 
    g_pSunRenderer->SetViewport(viewport);

    g_pSunRenderer->ClearRender(clear_flags, clear_color, clear_z, clear_stencil, viewport);  

    WzMatrix world_matrix;
    MatrixIdentity(&world_matrix);
    g_pSunRenderer->SetWorldMatrix(&world_matrix);

    WzVector cam_from, cam_to, cam_up;
    SetVector(&cam_from, 0.0, 10.0f, 0.0);
    SetVector(&cam_to, 0.0, 0.0, 0.0);
    SetVector(&cam_up, 0.0, 0.0, 1.0);
    g_pSunRenderer->SetCameraMatrix(&cam_from, &cam_to, &cam_up);
}
//------------------------------------------------------------------------------ 
void InterfaceManager::End2DMode()
{
    g_Camera.UpdateCamera();
    g_pSunRenderer->SetViewport(NULL);
    g_Camera.RestoreProjection();
    g_pSunRenderer->Set2DMode(backup_old2dmode ,backup_oldrealsize);			
    g_pSunRenderer->EnableFog(backup_fog);
    g_pSunRenderer->SetZBufferWrite(backup_zwrite);
    g_pSunRenderer->SetZBufferTest(backup_ztest);
    g_pSunRenderer->SetZFunc(backup_zfunc);
    g_pSunRenderer->SetAlphaBlendMode(backup_alphablendmode);
    g_pSunRenderer->SetAlphaTestMode(backup_alpharef, backup_alphafunc);
    g_pSunRenderer->SetCullFaceMode(backup_cullfacemode );
    g_pSunRenderer->SetMeshDrawMode(backup_drawmode );
    g_pSunRenderer->SetColor(backup_oldcolor);
    g_pSunRenderer->EnableAlphaTest(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
InterfaceManager::InterfaceManager()
: m_pDialogManager( NULL )
,m_pDialogHashTable( NULL )
,m_pNotLoadDlgTable( NULL )
,pet_etheria_icon_(NULL)
,etheria_icon_(NULL)
,warning_check_item_(INVALID_HANDLE_VALUE)
,m_DamageMgr( NULL )
,m_bValid ( TRUE )
{
    // 라이트 각도를 설정
    LOCAL_SUNLIGHT_DIRECTION.x = -0.58192080f;
    LOCAL_SUNLIGHT_DIRECTION.y = 0.41834027f;
    LOCAL_SUNLIGHT_DIRECTION.z = 0.69739485f;



	m_bRender = TRUE;
	m_bLockDlg = FALSE;
	m_pMessageMesh = NULL;

	m_hTextureTargetPointer = INVALID_HANDLE_VALUE;
	m_hTextureAreaPointer = INVALID_HANDLE_VALUE;
    m_hTextureAreaPointerForMystic = INVALID_HANDLE_VALUE;
#ifdef _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
    m_hTextureAreaPointerForShadow = INVALID_HANDLE_VALUE;
#endif //_NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    texture_area_pointer_for_witchblade_ = INVALID_HANDLE_VALUE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	m_hTextureActivate = INVALID_HANDLE_VALUE;
	m_hTextureChat = INVALID_HANDLE_VALUE;
	m_pInfoSlot = NULL;

    m_hTextureActivateAutobot = INVALID_HANDLE_VALUE;

	m_bMovePointerFlag = FALSE;
	m_pMovePointerUnitDraw = NULL;
	m_pMovePointerUnit = NULL;

	m_bAreaPointerFlag = FALSE;
	m_pAreaPointerUnitDraw = NULL;
	m_pAreaPointerUnit = NULL;

	m_bShowItemName = TRUE;

	m_dwInterfaceTick = 0;

	m_hTextureMyHead = INVALID_HANDLE_VALUE;
	m_hTextureTargetHead = INVALID_HANDLE_VALUE;
	m_hTextureSummonTargetHead = INVALID_HANDLE_VALUE;
	
	m_bUpdateMyHeadTexture = FALSE;
	m_bUpdateTargetHeadTexture = FALSE;
	m_bUpdateSummonTargetHeadTexture = FALSE;

	m_hPetInfoTexure  = INVALID_HANDLE_VALUE;
	m_pPetIconLockImg = NULL;	

	m_hNumberTexture = INVALID_HANDLE_VALUE;	
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
    item_score_texture = INVALID_HANDLE_VALUE;
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
    battle_icon_texure_ = INVALID_HANDLE_VALUE;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    button_activate_texture_handle_ = INVALID_HANDLE_VALUE;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	m_pIdentifyPossessionImg = NULL;

#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	for(int i = 0; i< eITEM_TYPEMAX; ++i)
	{
		m_pIdentifyDamegeImg[i] = NULL;
	}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

    for(int i = MARK_TYPE_START; i < MARK_TYPE_MAX; ++i)
    {
        item_mark_ptr[i] = NULL;
    }

	for (int a = 0; a < MAX_PARTYMEMBER_NUM; a++)
	{
		m_bUpdatePartyHeadTexture[a] = FALSE;
		m_hTexturePartyHead[a] = INVALID_HANDLE_VALUE;
        m_hTexturePartyDice[a] = INVALID_HANDLE_VALUE;
	}

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    common_bg01_texture_ = INVALID_HANDLE_VALUE;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

	m_pDynamicDialogKeyGenerator=NULL;
	m_bLoadFontAndString = FALSE;
	m_bShowGameInterface = FALSE;

	memset(m_vUserInterfaceMans,NULL,sizeof(m_vUserInterfaceMans));
    status_icon_manager_ = NULL;
}


//------------------------------------------------------------------------------
/**
*/
InterfaceManager::~InterfaceManager()
{
	
}


//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::Init( HWND hWnd, CDrawBase * pDrawBase)
{
	m_pDraw = pDrawBase;
	m_pDialogHashTable = new SolarHashTable<SolarDialog *>;
	m_pDialogHashTable->Initialize(500);

	m_pNotLoadDlgTable = new SolarHashTable<SolarDialog *>;
	m_pNotLoadDlgTable->Initialize(500);


    status_icon_manager_ = new StatusIconManager;
    status_icon_manager_->Initialize(this, pDrawBase);
    

	m_bHaveFocusAtEdit = FALSE;
	
	char szCurDirectory[512];
	ZeroMemory(szCurDirectory,sizeof(TCHAR));
	GetCurrentDirectoryA(512, szCurDirectory); 
	sprintf( szCurDirectory, "%s\\%s", szCurDirectory, "Data\\Interface\\");
	g_pSunRenderer->x_pManagerTextOut->SetFontFolderPath(szCurDirectory);


	m_hTextureMyHead = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 192,192, 4,  TLS_AS_EXTERNAL_RENDERTARGET  | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	m_hTextureTargetHead = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 64, 64, 4,  TLS_AS_EXTERNAL_RENDERTARGET   | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD) ;

	m_hTextureSummonTargetHead = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 64, 64, 4,  TLS_AS_EXTERNAL_RENDERTARGET   | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	for (int a = 0; a < MAX_PARTYMEMBER_NUM; a++)
	{
		m_hTexturePartyHead[a] = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 64, 64, 4,  TLS_AS_EXTERNAL_RENDERTARGET   | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        m_hTexturePartyDice[a] = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 32, 32, 4,  TLS_AS_EXTERNAL_RENDERTARGET   | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

	m_bUpdateMyHeadTexture = FALSE;
	m_bUpdateTargetHeadTexture = FALSE;
	m_bUpdateSummonTargetHeadTexture = FALSE;

	m_hTestRenderTarget = m_pDraw->x_pManagerTexture->GenerateEmptyTexture( 256, 256, 4,  TLS_AS_EXTERNAL_RENDERTARGET  | TLS_LINEAR | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

    //
    // IG: Interface Groups Initialize
    //
    if (!this->InitInterfaceManagerGroups())
        return FALSE;
		
#ifdef _KIKI_WEB_AUTH	
	
#ifdef _KIKI_UI_WEB_DIALOG

	uiBase * pDlg = GET_DIALOG( IM_WEB_DIALOG_MANAGER::WEB_DIALOG );
	//uiWebDialog* pDlg = (uiWebDialog *)g_InterfaceManager.GetUserInterface(UI_WEB_DIALOG);

	if (pDlg)
    {
        //pDlg->GetInterface();
		pDlg->ShowInterface( TRUE, TRUE );
		//pDlg->GetDialogWZ()->MoveWindowWZ(-1000, -1000);
    }
#endif

#endif

	m_hMonsterGauageTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\Dlg28.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);


    item_mark_ptr[MARK_TYPE_QUEST]       = ICONMGR()->Add(25110);
    item_mark_ptr[MARK_TYPE_QUEST_START] = ICONMGR()->Add(25111);
    item_mark_ptr[MARK_TYPE_DEVINE]      = ICONMGR()->Add(25112);
    item_mark_ptr[MARK_TYPE_CASH_ITEM]   = ICONMGR()->Add(25113);
    item_mark_ptr[MARK_TYPE_AC]          = ICONMGR()->Add(25298);
    item_mark_ptr[MARK_TYPE_ELITE]       = ICONMGR()->Add(25299);
    item_mark_ptr[MARK_TYPE_RARE]        = ICONMGR()->Add(25300);
    item_mark_ptr[MARK_TYPE_UNIQUE]      = ICONMGR()->Add(25301);

	m_hPetInfoTexure  = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\Pet_window.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	m_pPetIconLockImg = ICONMGR()->Add(74628);

    set_etheria_icon(ICONMGR()->Add(34136));
    set_pet_etheria_icon(ICONMGR()->Add(34137));
    
    set_warning_check_item(g_pSunRenderer->x_pManagerTexture->LoadTexture(
                                                        "Data\\Interface\\etc\\exclamation_mark.DDS", 
                                                        TLS_ORIGINAL_SIZE |
                                                        TLS_NO_MIPMAP | 
                                                        TLS_NO_MULTITHREAD));

	m_hNumberTexture  = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\all_text.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
    item_score_texture = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\all_buttenline_new1.tga",
        TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
    battle_icon_texure_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\all_split01.tga",
        TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	m_pIdentifyPossessionImg = ICONMGR()->Add(34048);

#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
    // Texture가 없는데 왜 부르고 있는지 봐야 한다. 일단 정리한다.
	m_pIdentifyDamegeImg[eITEM_WEAPON] = ICONMGR()->Add(34078);
	m_pIdentifyDamegeImg[eITEM_ARMOR] = ICONMGR()->Add(34079);
	m_pIdentifyDamegeImg[eITEM_ACCESSORY] = ICONMGR()->Add(34080);
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

    m_DamageMgr = new DamageMesh;
    m_DamageMgr->InitDamage(10000);
    
    HotKeyManager::Instance()->Initialize();
    
	// 타겟 포인터.	
	m_hTextureTargetPointer = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\cursor\\CLC2.tga",TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
	
	// 범위 포인터 	
	m_hTextureAreaPointer = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Effect\\areamagic.tga",TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    m_hTextureAreaPointerForMystic = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Effect\\areamagic_mio.tga", TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
#ifdef _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
    m_hTextureAreaPointerForShadow = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Effect\\areamagic_shadow.tga", TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
#endif //_NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    texture_area_pointer_for_witchblade_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Effect\\AreaMagic_WitchBlade.dds", TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	// 스킬 활성
	m_hTextureActivate	= g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\activate.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    m_hTextureLinkSkill = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\link.dds",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif

#ifdef _YMS_AUTOBOT_GAME
    m_hTextureActivateAutobot = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\auto_green.tga",
                                                                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif //_YMS_AUTOBOT_GAME
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    button_activate_texture_handle_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\activate.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	// 채팅
	m_hTextureChat = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\chat.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	m_hTextureBuffBG = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\Alpha_Out.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( MOVE_POINTER_RESOURCE_ID, "Data\\Interface\\cursor\\point.wzu" );

	m_pMovePointerUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(MOVE_POINTER_RESOURCE_ID);
    m_pMovePointerUnitDraw = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );
	m_pMovePointerUnitDraw->SetWzUnit( m_pMovePointerUnit,FALSE );
	m_pMovePointerUnitDraw->SetCurrentAnimation(StrToWzID("ID01"),TRUE);
	m_pMovePointerUnitDraw->SetScale(3.0f,3.0f,3.0f);

	g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( AREA_POINTER_RESOURCE_ID, "Data\\Interface\\cursor\\point.wzu" );

	g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( TARGET_POINTER_RESOURCE_ID, "Data\\Interface\\cursor\\Select_arrow.wzu" );

	m_pTargetPointerUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(TARGET_POINTER_RESOURCE_ID);
    m_pTargetPointerUnitDraw = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );
	m_pTargetPointerUnitDraw->SetWzUnit( m_pTargetPointerUnit,FALSE );
	m_pTargetPointerUnitDraw->SetCurrentAnimation(StrToWzID("ID01"),TRUE);
	m_pTargetPointerUnitDraw->SetScale(2.5f,2.5f,2.5f);

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING

    dice_cast_manager_ = new DiceCastManager(this);

    g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit(
        DICE_RESOURCE_ID, "Data\\Interface\\dice\\dice.wzu"/*, TRUE, ULS_NO_MULTITHREAD*/);
    dice_unit_			= g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(DICE_RESOURCE_ID);
    dice_unit_draw_		= (CWzUnitDraw *)Func_ContactWzd(CW_NEW_WZUNITDRAW, 0);

    dice_unit_draw_->SetWzUnit(dice_unit_,FALSE);
    WzVector dice_position;
    SetVector( &dice_position, 0.0f, 15.0f, 0.0f );
    dice_unit_draw_->SetPosition(dice_position);
    for (int i = 0; i < MAX_PARTYMEMBER_NUM; ++i)
    {
        dice_angle_x_[i] = 0.0f;
        dice_angle_z_[i] = 0.0f;
    }
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    common_bg01_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\common_bg01.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

    m_pMessageMesh = New_SimpleMesh(2);

    GameMessage::Init();

    // 2009.03.18
    // 엔진에서의 폰트 제한(32개)을 초과 생성 - 해당 폰트들은 클라이언트에서 사용하지 않음, 주석처리 - 이태준
    //BOOL bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("NEWF"), 18, TRUE,"Garamond", DEFAULT_CHARSET,0,NULL);
    //bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("HEA1"), 32, TRUE,"HY헤드라인M", DEFAULT_CHARSET,0,NULL);
    //bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("HEA1"), 48, TRUE,"HY헤드라인M", DEFAULT_CHARSET,0,NULL);
    //bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("HEA3"), 10, TRUE,"HY헤드라인M", DEFAULT_CHARSET,0,NULL);
    //bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("SFDL"), 12, FALSE,"SF들국화", DEFAULT_CHARSET,ANTIALIASED_QUALITY,DEFAULT_PITCH | FF_DONTCARE,NULL);
    //bFontRet = g_pSunRenderer->x_pManagerTextOut->AddExtendedFont( StrToWzID("0000"), 8, TRUE,"HY헤드라인M", DEFAULT_CHARSET,0,NULL);


	m_bLoadFontAndString = TRUE;

#ifdef _DEV_VER
    LoadItemNameCode();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH   
#ifdef _DH_GM_NPC_WINDOW
    LoadNpcNameCode();
#endif//_DH_GM_NPC_WINDOW

    grid_effect_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\etc\\Help_Focus.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    InitBlinkGrid();

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        gold_rush_mark_map_.clear();
        int gold_mark_index = 287;
        for (int i = eGOLDGRADE1; i < eGOLDGRADE_MAX; ++i)
        {
            gold_rush_mark_map_.insert(make_pair(i, gold_mark_index + i));
        }
    }    
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	{
		std::vector<std::map<WORD, WORD> >::iterator iter;
		for ( iter = sun_ranking_grade_mark_vector_.begin(); iter != sun_ranking_grade_mark_vector_.end(); ++iter)
		{
			(*iter).clear();
		}
		sun_ranking_grade_mark_vector_.clear();
		int sun_ranking_grade_mark_index = 294;
		for ( int i = eSUN_RANKING_TYPE_START; i < eSUN_RANKING_TYPE_END; ++i )
		{
			std::map<WORD, WORD> ranking_mark_map;
			for ( int j = eSUN_RANKING_GRADE_START; j <= eSUN_RANKING_GRADE_4; ++j )
			{
				ranking_mark_map.insert(make_pair(j, sun_ranking_grade_mark_index + (i * 5) + j));
			}
			sun_ranking_grade_mark_vector_.push_back(ranking_mark_map);
		}
	}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
VOID InterfaceManager::Release()
{	
	//
    // IG: Release Interface Groups
    //
    this->ReleaseInterfaceManagerGroups();

    //
    // Release others
    //
	m_bValid = FALSE;
	if( m_pDialogHashTable )
	{
        DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
        DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
        SolarDialog* dialog_ptr = NULL;

        while (dialog_current_iterator != dialog_end_iterator)
        {
            dialog_ptr = (*dialog_current_iterator);
            if ((dialog_ptr != NULL))
            {
                if (dialog_ptr->GetDialogWZ()->GetDlgID() == StrToWzID("0830"))
                {
                    _asm nop;
                }

                dialog_ptr->Release();
                delete dialog_ptr;
                dialog_ptr = NULL;
            }
            ++dialog_current_iterator;
        }

		m_pDialogHashTable->RemoveAll();
		delete m_pDialogHashTable;
		m_pDialogHashTable = NULL;
	}

	if( m_pNotLoadDlgTable )
	{
		m_pNotLoadDlgTable->RemoveAll();
		delete m_pNotLoadDlgTable;
 		m_pNotLoadDlgTable = NULL;
	}
	
	if( m_pDialogManager )
	{
        assert( g_pContactDialog_FnPtr );
		g_pContactDialog_FnPtr( WZ_DELETE_DLG_MANAGER, m_pDialogManager );
		m_pDialogManager = NULL;
	}


	if( m_hMonsterGauageTexture != INVALID_HANDLE_VALUE )
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hMonsterGauageTexture );
		}
		m_hMonsterGauageTexture = INVALID_HANDLE_VALUE;
	}

    if (grid_effect_texture_ != INVALID_HANDLE_VALUE)
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture(grid_effect_texture_);
        }
        grid_effect_texture_ = INVALID_HANDLE_VALUE;
    }

    for(int i = MARK_TYPE_START; i < MARK_TYPE_MAX; ++i)
    {
        if (item_mark_ptr[i] != NULL)
        {
            ICONMGR()->RemoveIcon(item_mark_ptr[i]);
        }
    }

	if( m_hPetInfoTexure != INVALID_HANDLE_VALUE )
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hPetInfoTexure );
		}
		m_hPetInfoTexure = INVALID_HANDLE_VALUE;
	}

	if(m_pPetIconLockImg)
	{
		ICONMGR()->RemoveIcon(m_pPetIconLockImg);
	}

    if (pet_etheria_icon())
    {
        ICONMGR()->RemoveIcon(pet_etheria_icon());
    }
    
    if (etheria_icon())
    {
        ICONMGR()->RemoveIcon(etheria_icon());
    }

    if( get_warning_check_item() != INVALID_HANDLE_VALUE )
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( get_warning_check_item() );
        }
        set_warning_check_item(INVALID_HANDLE_VALUE);
    }


	if( m_hNumberTexture != INVALID_HANDLE_VALUE )
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hNumberTexture );
		}
		m_hNumberTexture = INVALID_HANDLE_VALUE;
	}

#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
    if (item_score_texture != INVALID_HANDLE_VALUE)
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture(item_score_texture);
        }
        item_score_texture = INVALID_HANDLE_VALUE;
    }
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
    if (battle_icon_texure_ != INVALID_HANDLE_VALUE)
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture(battle_icon_texure_);
        }
        battle_icon_texure_ = INVALID_HANDLE_VALUE;
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (button_activate_texture_handle_ != INVALID_HANDLE_VALUE)
    {
        if (m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture(button_activate_texture_handle_);
        }
        button_activate_texture_handle_ = INVALID_HANDLE_VALUE;
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    if(m_DamageMgr)
    {
        delete m_DamageMgr;
    }
	
	if ( m_pMovePointerUnitDraw)
	{
		Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pMovePointerUnitDraw );
		m_pMovePointerUnitDraw = NULL;
	}
	if ( m_pMovePointerUnit )
	{
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(MOVE_POINTER_RESOURCE_ID);
		m_pMovePointerUnit = NULL;

	}

	if ( m_pAreaPointerUnitDraw)
	{
		Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pAreaPointerUnitDraw );
		m_pAreaPointerUnitDraw = NULL;
	}
	if ( m_pAreaPointerUnit )
	{
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(AREA_POINTER_RESOURCE_ID);
		m_pAreaPointerUnit = NULL;

	}

	if ( m_pTargetPointerUnitDraw)
	{
		Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pTargetPointerUnitDraw );
		m_pTargetPointerUnitDraw = NULL;
	}
	if ( m_pTargetPointerUnit )
	{
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(TARGET_POINTER_RESOURCE_ID);
		m_pTargetPointerUnit = NULL;

	}
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    if (dice_cast_manager_)
    {
        delete dice_cast_manager_;
    }
    if (dice_unit_draw_)
    {
        Func_ContactWzd(CW_DELETE_WZUNITDRAW, dice_unit_draw_);
        dice_unit_draw_ = NULL;
    }
    if (dice_unit_)
    {
        g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(DICE_RESOURCE_ID);
        dice_unit_ = NULL;
    }
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

	if ( m_hTextureTargetPointer != INVALID_HANDLE_VALUE) 
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureTargetPointer );
		}
		m_hTextureTargetPointer = INVALID_HANDLE_VALUE;
	}

	if ( m_hTextureAreaPointer != INVALID_HANDLE_VALUE) 
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureAreaPointer );
		}
		m_hTextureAreaPointer = INVALID_HANDLE_VALUE;
	}

    if ( m_hTextureAreaPointerForMystic != INVALID_HANDLE_VALUE) 
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureAreaPointerForMystic );
        }
        m_hTextureAreaPointerForMystic = INVALID_HANDLE_VALUE;
    }

#ifdef _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
    if ( m_hTextureAreaPointerForShadow != INVALID_HANDLE_VALUE) 
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureAreaPointerForShadow );
        }
        m_hTextureAreaPointerForShadow = INVALID_HANDLE_VALUE;
    }
#endif //_NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if ( texture_area_pointer_for_witchblade_ != INVALID_HANDLE_VALUE )
    {
        if (m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( texture_area_pointer_for_witchblade_ );
        }
        texture_area_pointer_for_witchblade_ = INVALID_HANDLE_VALUE;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    if (m_hTextureLinkSkill != INVALID_HANDLE_VALUE)
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureLinkSkill );
        }
        m_hTextureLinkSkill = INVALID_HANDLE_VALUE;
    }
#endif

	if (m_hTextureActivate != INVALID_HANDLE_VALUE)
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureActivate );
		}
		m_hTextureActivate = INVALID_HANDLE_VALUE;

	}

    if (m_hTextureActivateAutobot != INVALID_HANDLE_VALUE)
    {
        if (m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture(m_hTextureActivateAutobot);
        }
        m_hTextureActivateAutobot = INVALID_HANDLE_VALUE;
    }
    
	if (m_hTextureChat != INVALID_HANDLE_VALUE)
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureChat );
		}
		m_hTextureChat = INVALID_HANDLE_VALUE;

	}

	if (m_hTextureBuffBG != INVALID_HANDLE_VALUE)
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureBuffBG );
		}
		m_hTextureBuffBG = INVALID_HANDLE_VALUE;

	}
	
	if ( m_pMessageMesh )
	{
		Delete_SimpleMesh(m_pMessageMesh);
		m_pMessageMesh = NULL;
	}

	if (m_hTextureMyHead != INVALID_HANDLE_VALUE) 
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureMyHead );
		}
		m_hTextureMyHead = INVALID_HANDLE_VALUE;
	}
	if (m_hTextureTargetHead != INVALID_HANDLE_VALUE)
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureTargetHead );
		}
		m_hTextureTargetHead = INVALID_HANDLE_VALUE;

	}

	if (m_hTextureSummonTargetHead != INVALID_HANDLE_VALUE)
	{
		if(m_pDraw && m_pDraw->x_pManagerTexture)
		{
			m_pDraw->x_pManagerTexture->DestroyTexture( m_hTextureSummonTargetHead );
		}
		m_hTextureSummonTargetHead = INVALID_HANDLE_VALUE;

	}

	for (int a = 0; a < MAX_PARTYMEMBER_NUM; a++)
	{
        if (m_pDraw && m_pDraw->x_pManagerTexture)
        {
		    if (m_hTexturePartyHead[a] != INVALID_HANDLE_VALUE)
		    {
				m_pDraw->x_pManagerTexture->DestroyTexture( m_hTexturePartyHead[a] );
                m_hTexturePartyHead[a] = INVALID_HANDLE_VALUE;
			}
            if (m_hTexturePartyDice[a] != INVALID_HANDLE_VALUE)
            {
                m_pDraw->x_pManagerTexture->DestroyTexture( m_hTexturePartyDice[a] );
                m_hTexturePartyDice[a] = INVALID_HANDLE_VALUE;
            }
		}

	}

    if (status_icon_manager_ != NULL)
    {
        delete status_icon_manager_;
        status_icon_manager_ = NULL;
    }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    gold_rush_mark_map_.clear();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    if (common_bg01_texture_ != INVALID_HANDLE_VALUE)
    {
        if(m_pDraw && m_pDraw->x_pManagerTexture)
        {
            m_pDraw->x_pManagerTexture->DestroyTexture( common_bg01_texture_ );
        }
        common_bg01_texture_ = INVALID_HANDLE_VALUE;
    }
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
}

//------------------------------------------------------------------------------
/**
*/
VOID InterfaceManager::Render()
{
    if (!m_pDraw)
        return;

	m_pDraw->SetViewport(NULL);

#ifdef _YMS_SCREEN_RATIO_
#else
	if (g_pApplication->IsWideModeFlag())
	{
		g_Camera.SetAspectRatio(SR_16_9);
		
	}
	else
	{
		g_Camera.SetAspectRatio(SR_4_3);
	}
#endif //_YMS_SCREEN_RATIO_

	if( GetShowFlag() == TRUE )
	{
		g_pApplication->Set2DMode(TRUE);

		ENUM_ALPHABLEND eAlphablendMode = m_pDraw->GetAlphaBlendMode();
		BOOL bAlphaTest = m_pDraw->IsEnableAlphaTest();
		m_pDraw->SetAlphaBlendMode( AB_ALPHA );
		DWORD dwRef;
		ENUM_CMP_FUNC func;
		m_pDraw->GetAlphaTestMode( &dwRef,&func);
		m_pDraw->EnableAlphaTest(TRUE);

        if (status_icon_manager_)
        {
            status_icon_manager_->RegisterStatusIconInfo();
            status_icon_manager_->RenderStatusIcon();
        }

		BOOL RenderTooltip = TRUE;

		if (m_pDialogManager)
		{
			int iDlgCnt = m_pDialogManager->GetDlgCnt();

			SolarDialog * pTargetDlg;

			for (int i = 0; i < iDlgCnt; i++)
			{
				DWORD dwID = m_pDialogManager->GetDialogIDToIndex(i);
				
				m_pDraw->SetAlphaBlendMode( AB_ALPHA );
				m_pDraw->EnableAlphaTest(TRUE);
				m_pDraw->SetAlphaTestMode(1,CF_GREATEREQUAL);

                pTargetDlg = m_pDialogHashTable->GetData(dwID);
#ifdef _DEBUG
                if( pTargetDlg != NULL )
                {
                    if( pTargetDlg->GetDialogWZ()->IsState(WZ_HIDE) == false )
                    {
                        static char s_idstring[] = "0402";
                        WzID temp_id = StrToWzID(s_idstring);
                        if( pTargetDlg->GetDialogWZ()->GetDlgID() == temp_id )
                        {
                            temp_id = temp_id;
                        }
                    }
                }
#endif // _DEBUG

				m_pDialogManager->OnDrawDialog(dwID);


				if (pTargetDlg)
				{
					//m_pDraw->SetAlphaBlendMode( AB_ALPHA ,TRUE);
					//m_pDraw->EnableAlphaTest(TRUE , TRUE);
					//m_pDraw->SetAlphaTestMode(1,CF_GREATEREQUAL , TRUE);
					//m_pDraw->EnableSpecular(FALSE);
					pTargetDlg->Render();
				}
				m_pDraw->SetAlphaBlendMode( AB_ALPHA );
			}
			
			if(GAMEOPTION->GetSkillQueueing())
				g_SkillQueue.RenderNode();


#if WZENGINEVER >= 580
			uiToolTipMan* pTooltipMan = static_cast<uiToolTipMan*>( m_vUserInterfaceMans[UIMAN_TOOLTIP] );

			int MouseX = Mouse::Instance()->GetMouseX();
			int MouseY = Mouse::Instance()->GetMouseY();

			DWORD DIalogID = m_pDialogManager->PickDialog( MouseX, MouseY );
			if( DIalogID != 0 && pTooltipMan->GetTooltipDialogID() != 0 && pTooltipMan->GetTooltipDialogID() != DIalogID )
			{
                SolarDialog* pPickedDialog = m_pDialogHashTable->GetData(DIalogID);

				pTooltipMan->InitTooltip();
				pTooltipMan->SetTooltipDialogID(0);
				RenderTooltip = FALSE;
			}
#endif
		}

		m_pDraw->EnableAlphaTest(bAlphaTest);
		m_pDraw->SetAlphaBlendMode( eAlphablendMode );
		m_pDraw->Set2DMode( FALSE );

		
        GameMessage::RenderMessage(m_pMessageMesh);


		if( RenderTooltip == TRUE )
		{
			uiToolTipMan* ttm = static_cast<uiToolTipMan*>( m_vUserInterfaceMans[UIMAN_TOOLTIP] );
			assert( ttm );

			uiToolTipMan* ttm2 = static_cast<uiToolTipMan*>( m_vUserInterfaceMans[UIMAN_TOOLTIP2] );
			assert( ttm2 );

			ttm->Render();

			const RECT& baseRect = ttm->GetTooltipRect();
			ttm2->RenderEx( baseRect );

			ttm->SetTooltipDialogID(0);
		}
	}

	g_Camera.UpdateCamera();
	
}

//------------------------------------------------------------------------------
/**
*/
VOID InterfaceManager::Update()
{
    if (!m_pDialogHashTable)
    {
        return;
    }

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
	{
        dialog_ptr = (*dialog_current_iterator);
        if ((dialog_ptr != NULL))
        {
            dialog_ptr->Update();

		    if (dialog_ptr->GetDestroyWindow())
		    {
                ++dialog_current_iterator;
			    DeleteInterface(dialog_ptr->GetDialogWZ()->GetDlgID());			 
                continue;
		    }
        }
        ++dialog_current_iterator;
	}

	m_pDialogManager->OnDoAction();

    // ui manager updates
    for (int i = 0; i < UIMAN_MAX; ++i)
    {
        uiBaseMan* baseMan = this->GetUserInterfaceManager(i);

        if (baseMan)
        {
            baseMan->Update();
        }
    }
}


BOOL InterfaceManager::IsIntersectRect(POINT& rPoint)
{
	if (!m_pDialogHashTable)
    {
        return FALSE;
    }

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;
    BOOL return_value = FALSE;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if ((dialog_ptr != NULL) &&
            (dialog_ptr->IsVisibleWindow() == TRUE) &&
            (dialog_ptr->IsLocked() != TRUE))
        {
            return_value = dialog_ptr->InterSectRect(rPoint);
            if (return_value == TRUE) 
            {
                return return_value;
            }
        }
        ++dialog_current_iterator;
    }
    
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::InterfaceWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, SI_MESSAGE *pSiMessage )
{
	if( !m_pDialogManager || !IsValid() ) return TRUE;

    BOOL bRet = TRUE;

	BOOL bOld2dMode,bOldRealSize;
	g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
	g_pApplication->Set2DMode();
	
    bRet = m_pDialogManager->WindowProc( hWnd, iMessage, wParam, lParam, pSiMessage );


    if (pSiMessage->eResultMsg != RT_NONE && ( pSiMessage->dwDlgID ))
    {
		SolarDialog* dlg = m_pDialogHashTable->GetData( pSiMessage->dwDlgID );
		if (dlg)
		{
			if (dlg->IsLocked() == false)
			{
                //------------------------------------------------------------------------------
                //! 사운드 처리
                if ((pSiMessage->eResultMsg == RT_MSG_LBUTTONCLICK) ||
                    (pSiMessage->eResultMsg == RT_MSG_LBUTTONDCLICK) ||
                    (pSiMessage->eResultMsg == RT_MSG_RBUTTONCLICK) ||
                    (pSiMessage->eResultMsg == RT_MSG_RBUTTONDCLICK))
                {
                    CControlWZ* controlwz = dlg->GetControlWZ(pSiMessage->dwCtrlID);

                    if (controlwz != NULL)
                    {
                        int sound_index = controlwz->GetSoundIndex(pSiMessage->eResultMsg);
                        if (sound_index > 0)
                        {
                            g_SoundEffect.PlayUI(sound_index);
                        }
                    }
                }
                //------------------------------------------------------------------------------ 

				dlg->MessageProc(pSiMessage);
			}
		}
    }

	g_pSunRenderer->Set2DMode( bOld2dMode ,bOldRealSize);

	return bRet;
}



//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::IsShowDialog( eDIALOG_ID id ) const
{
	SolarDialog * pDlg = m_pDialogHashTable->GetData( m_wzId[id] );

	if (pDlg)
		return pDlg->IsVisibleWindow();
	else
		return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::IsHaveFocus( eDIALOG_ID id ) const
{
	SolarDialog * pDlg = m_pDialogHashTable->GetData( m_wzId[id] );
	if ( pDlg )
	{
		return pDlg->HaveFocus();
	}
	else
	{
		return FALSE;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::IsHaveFocus( WzID id ) const
{
	SolarDialog *  pDlg = m_pDialogHashTable->GetData(id);
	if ( pDlg )
	{
		return pDlg->HaveFocus();
	}
	else
	{
		return FALSE;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::IsShowDialog( WzID id ) const
{
	SolarDialog *  pDlg = m_pDialogHashTable->GetData(id);

	if (!pDlg)
	{
		return FALSE;
	}

	return (pDlg->IsVisibleWindow());
}

DWORD InterfaceManager::NewInterface( SolarDialog * pSolarDialog, char * pszFileName, BOOL bShow )
{
	DWORD dwNewId = g_IDGenerator.GenerateID();

	DA_FILE hf;

	if( GENERALPARAM->IsUsePackFile() )
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_RESOURCE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);
	else
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_NOT_USE_PACKFILE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);

	if (INVALID_DA_FILE	== hf )
	{
		MessageBoxA(NULL,pszFileName,"없어요",MB_OK);

		assert( !"인터페이스 파일이 없음" );

		return 0;
	}
	DWORD dwID = m_pDialogManager->OnLoad( hf );

	g_pApplication->GetDiscAccess()->CloseFile( hf );

	CDialogWZ	*pwzDlg = m_pDialogManager->GetDialogWZ( dwID );
	
	pwzDlg->SetDlgID(dwNewId);

	pSolarDialog->SetDialogWZ( pwzDlg );
	pSolarDialog->Init( m_pDraw );
	pSolarDialog->ShowWindow( bShow );
	m_pDialogHashTable->Add( pSolarDialog, dwNewId );
	pSolarDialog->SetDynamicWindow(TRUE);

	return dwNewId;

}
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
BOOL InterfaceManager::InsertInterface(SolarDialog * solar_dialog, CDialogWZ* dialog, BOOL is_show)
{
    // 생성된 ui를 연결
    if (solar_dialog == NULL || dialog == NULL)
    {
        return FALSE;
    }

    DWORD dialog_key = dialog->GetDlgID();

    solar_dialog->SetDialogWZ(dialog);
    solar_dialog->Init(m_pDraw);
    solar_dialog->ShowWindow(is_show);

    if (m_pDialogHashTable->Add(solar_dialog, dialog_key) == TRUE)
    {
        solar_dialog->SetLoaded(TRUE);
        solar_dialog->MoveDefaultWindowPos();
    }

    return TRUE;
}

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
BOOL InterfaceManager::LoadInterfaceChangeID( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwChangeID, BOOL bShow )
{
	DA_FILE hf;

	if( GENERALPARAM->IsUsePackFile() )
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_RESOURCE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);
	else
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_NOT_USE_PACKFILE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);

	if (INVALID_DA_FILE	== hf )
	{
		MessageBoxA(NULL,pszFileName,"없어요",MB_OK);

		assert( !"인터페이스 파일이 없음" );

		return 0;
	}

	DWORD dwID = m_pDialogManager->OnLoad( hf, &dwChangeID);

	g_pApplication->GetDiscAccess()->CloseFile( hf );

	CDialogWZ	*pwzDlg = m_pDialogManager->GetDialogWZ( dwID );
	
	pSolarDialog->SetDialogWZ( pwzDlg );
	pSolarDialog->Init( m_pDraw );
	pSolarDialog->ShowWindow( bShow );
	if( m_pDialogHashTable->Add( pSolarDialog, dwID ) == TRUE )
	{
		pSolarDialog->SetLoaded( TRUE );
	}

	return TRUE;
}
//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::LoadInterface( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwCompareID, BOOL bShow, BOOL bCompare )
{
	if( !_loadDialog( pszFileName, dwCompareID, bCompare ) )
	{		
		assert( !"잘못된 ID" );
		return FALSE;
	}

	CDialogWZ	*pwzDlg = m_pDialogManager->GetDialogWZ( dwCompareID );

	pSolarDialog->SetDialogWZ( pwzDlg );
	pSolarDialog->Init( m_pDraw );
	pSolarDialog->ShowWindow( bShow );

	if( m_pDialogHashTable->Add( pSolarDialog, dwCompareID ) == TRUE )
	{
		pSolarDialog->SetLoaded( TRUE );
        pSolarDialog->MoveDefaultWindowPos();
		m_pNotLoadDlgTable->Remove( dwCompareID );		// 로드 되면 지운다.	
	}

	return TRUE;
}


BOOL InterfaceManager::CloneInterface( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwNewKey )
{
	if( !_loadDialog( pszFileName, dwNewKey, FALSE ) )
	{		
		assert( !"잘못된 ID" );
		return FALSE;
	}

	CDialogWZ	*pwzDlg = m_pDialogManager->GetDialogWZ( dwNewKey );

	pSolarDialog->SetDialogWZ( pwzDlg );
	pSolarDialog->Init( m_pDraw );
	pSolarDialog->ShowWindow( FALSE );

	if( m_pDialogHashTable->Add( pSolarDialog, dwNewKey ) == TRUE )
	{
		pSolarDialog->SetLoaded(TRUE);
        pSolarDialog->MoveDefaultWindowPos();
		m_pNotLoadDlgTable->Remove( dwNewKey );		// 로드 되면 지운다.	
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::_loadDialog( char * pszFileName, DWORD dwCompareID, BOOL bCompare )
{
	DA_FILE hf;
	if( GENERALPARAM->IsUsePackFile() )
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_RESOURCE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);
	else
		hf = g_pApplication->GetDiscAccess()->OpenFile(ePFN_NOT_USE_PACKFILE, pszFileName, DA_READ, SM_READ, OD_OPEN_EXISTING);

	if (INVALID_DA_FILE	== hf )
	{
		TCHAR szTemp[MAX_PATH];
		_snprintf( szTemp, MAX_PATH, "cannot find %s file", pszFileName );
		MessageBoxA( NULL, szTemp, "file access error", MB_OK);
		assert( !szTemp );
		g_pApplication->GetDiscAccess()->CloseFile( hf );
		return FALSE;
	}

	DWORD dwID = -1;

	if( bCompare == TRUE )
	{
		dwID = m_pDialogManager->OnLoad( hf );
		if( dwCompareID != dwID )
		{
			g_pApplication->GetDiscAccess()->CloseFile( hf );
			return FALSE;
		}
	}
	else
	{
		dwID = m_pDialogManager->OnLoad( hf, &dwCompareID );
	}
	 
	g_pApplication->GetDiscAccess()->CloseFile( hf );
	
	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::DeleteInterface( DWORD dwCompareID )
{
	DeAttachFirstDialog(dwCompareID);

	SolarDialog * pDlg = m_pDialogHashTable->GetData( dwCompareID );

	if(pDlg)
	{
        pDlg->ShowWindow(FALSE);
		if(pDlg->IsDynamicWindow())
		{
			g_IDGenerator.DestroyID(dwCompareID);
		}		

		pDlg->Release();
		m_pDialogHashTable->Remove( dwCompareID );
		delete pDlg;
		pDlg = NULL;		
	}

	if( FALSE == m_pDialogManager->RemoveDialogWZ( dwCompareID ) )
	{
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
SolarDialog * 
InterfaceManager::GetDialog( eDIALOG_ID eDlgID )
{
	if (!m_pDialogHashTable)
	{
		return NULL;
	}

	return m_pDialogHashTable->GetData( m_wzId[eDlgID] );	
}


//----------------------------------------------------------------------------
/**
*/
SolarDialog * InterfaceManager::GetDialog( WzID DlgWzID )
{
	if (!m_pDialogHashTable)
	{
		return NULL;
	}

	SolarDialog * pDlg = m_pDialogHashTable->GetData( DlgWzID );	

	if( pDlg == NULL )	// 로드가 안되어 있으면 등록되어 있는 해쉬에서 얻어온다
	{
		if( m_pNotLoadDlgTable == NULL )
			return NULL;

		uiBase * pUI = (uiBase*)m_pNotLoadDlgTable->GetData( DlgWzID );
		return pUI;
	}

	return pDlg;
}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RemoveDialog( WzID DlgWzID )
{
    m_pDialogHashTable->Remove( DlgWzID );
	m_pNotLoadDlgTable->Remove( DlgWzID );
}

//------------------------------------------------------------------------------
/**
*/
WzID InterfaceManager::GetDialogWzID( eDIALOG_ID eDlgID )
{
	assert( eDlgID < DIALOG_MAX );

	if ( eDlgID >= DIALOG_MAX )
	{
		return (WzID)0;
	}

	return m_wzId[eDlgID];
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InterfaceManager::AttachFirstDialog(eDIALOG_ID eDlgID)
{
	WzID wzChatID = GetDialogWzID(eDlgID);
	GetDialogManager()->FirstDialogID( wzChatID );
}


//----------------------------------------------------------------------------
/**
*/
VOID			
InterfaceManager::AttachFirstDialog(WzID DlgWzID)
{
	GetDialogManager()->FirstDialogID( DlgWzID );
}


//----------------------------------------------------------------------------
/**	 현재는 다이얼로그 하나 단위로 밖에 안되지만 앞으로는 여러개의 다이얼로그를 
	최상위 다이얼로그로 올릴수 있도록 eDlgID를 받아놓는다.
*/
//----------------------------------------------------------------------------
/**
*/
VOID 
InterfaceManager::DeAttachFirstDialog(eDIALOG_ID eDlgID)
{
	WzID DlgWzID = GetDialogWzID(eDlgID);
	GetDialogManager()->DeAttachFirstDialogID(DlgWzID);
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID								
InterfaceManager::DeAttachFirstDialog(WzID DlgWzID)
{
	GetDialogManager()->DeAttachFirstDialogID(DlgWzID);
}


//----------------------------------------------------------------------------
/**
*/
VOID
InterfaceManager::MoveWindow( WzID DlgWzID, int x, int y )
{
	SolarDialog * pDlg = GetDialog(DlgWzID);
	if (pDlg)
	{
		pDlg->MoveWindowWZ(x, y);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID								
InterfaceManager::MoveWindow( eDIALOG_ID eDlgID, int x, int y )
{
	SolarDialog * pDlg = GetDialog( eDlgID );
	if (pDlg)
	{
		pDlg->MoveWindowWZ(x, y);
	}
}

//------------------------------------------------------------------------------ 
void InterfaceManager::MoveDefaultWindowPos()
{
    if (!m_pDialogHashTable)
    {
        return;
    }

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr != NULL)
        {
            dialog_ptr->MoveDefaultWindowPos();
        }
        ++dialog_current_iterator;
    }
}

//----------------------------------------------------------------------------
/**
*/
VOID 
InterfaceManager::ShowGameInterface( BOOL bFlag )
{
	m_bShowGameInterface = bFlag;
	// 레이더 Update
	if (bFlag)
	{
        uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
        if (ui_radar_manager != NULL)
        {
            ui_radar_manager->ChangeRadarMap();
        }
	}


	uiQuestMan* pQuestMan = (uiQuestMan*)GetUserInterfaceManager(UIMAN_QUEST);
	if (pQuestMan)
	{
		pQuestMan->SetCheckTime(bFlag);

		// 자동 수락 퀘스트 표시를 위한 업데이트.
		pQuestMan->UpdateQuestAlram(bFlag);
		pQuestMan->ShowTimeQuest(bFlag);
	}	


    this->ShowUIPartys();
    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager != NULL)
    {
        ui_party_manager->SetOpenPartyJoinWindow(FALSE);
    }

	if (GlobalFunc::IsSSQScene() == FALSE)
    {
		ShowWindow(IM_RADAR_MANAGER::RADAR_DIALOG, bFlag);
    }

	ShowWindow(InterfaceManager::DIALOG_HEROGAUGE, bFlag);
	ShowWindow(IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLGCONTROL, bFlag);
	ShowWindow(InterfaceManager::DIALOG_QUICK, bFlag);
	
	//GS_BUG_FIX:존이동시 스탯창은 닫는다
    uiCharacterStatusMan::ShowCharacterStatusDialog(false);
	if(!bFlag)
	{
		UnLockDlgs();
	}

	
    if (TIMERPARAM->GetShowTiredSystem())
	{
		GET_DIALOG(IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM)->ShowWindow(bFlag);
	}


	ShowWindow(InterfaceManager::DIALOG_ENDURANCE,bFlag);

    int nEventTimerMode = TIMERPARAM->GetShowEventTimer();
	BOOL bPCBangMode = (g_pHero && g_pHero->GetPCBangMode());
	if ( nEventTimerMode == 1 && bPCBangMode				// PC방
		|| nEventTimerMode == 2 && !bPCBangMode				// 일반
		|| nEventTimerMode == 3 )							// PC방+일반
	{
		ShowWindow(InterfaceManager::DIALOG_PCBANG_EVENT_TIME , bFlag);
	}

	ChatDialog * pChatDialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog * ) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
	if( pChatDialog && pChatDialog->GetShowSystemChat() )
    {
        ShowWindow(IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG, bFlag);
        uiBattleZoneList* pZoneListDlg=(uiBattleZoneList*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST);
        if(pZoneListDlg)
        {
            if(pZoneListDlg->IsVisible())
            {
                pZoneListDlg->AttachChatDialog();
            }
        }
	}
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	uiNotificationsSystemDialog* notifications_system_dialog = static_cast<uiNotificationsSystemDialog*>(GET_DIALOG(IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_DIALOG));
	if ( notifications_system_dialog )
	{
		notifications_system_dialog->ShowInterface(bFlag);
	}
	uiNotificationsSystemListDialog* notifications_system_list_dialog = static_cast<uiNotificationsSystemListDialog*>(GET_DIALOG(IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_LIST));
	if ( notifications_system_list_dialog )
	{
		notifications_system_list_dialog->ShowInterface(bFlag);
		notifications_system_list_dialog->ShowInterface(FALSE);
		notifications_system_list_dialog->ClearList();
		MSG_CW_NOTIFICATION_PREV_SYN msg;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX,&msg,sizeof(msg));
	}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	if( g_pHero )
	{

		switch (g_pHero->GetClass())
		{
		case eCHAR_BERSERKER:
		case eCHAR_DRAGON:
			{
                ShowWindow(InterfaceManager::DIALOG_STYLE_CONTAINER, bFlag);
			}
			break;

		case eCHAR_VALKYRIE:
			{
				if(g_pHero->GetSummonId()!=NONE_ID)
				{
					SummonTargetDialog * pSummonTargetDlg = GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);

					if (pSummonTargetDlg)
					{
						pSummonTargetDlg->ShowWindow(TRUE);
					}
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
					SummonSkillContainer * pSummonSkillContainer = (SummonSkillContainer * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SUMMON_SKILL_CONTAINER);

					if(pSummonSkillContainer)
					{
						pSummonSkillContainer->ShowWindow(TRUE);
					}
#endif
				}
			}
			break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE:
            {
                // 위치블레이드 일 경우에는 띄워 줄 수 있도록 하자.
                ShowWindow(IM_FP_GAUGE_MANAGER::FP_GAUGE_DIALOG, bFlag);
            }
            break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

		case eCHAR_ELEMENTALIST:
		case eCHAR_SHADOW:
		default:
			break;
		}
	}  

	if(!bFlag)
	{
        DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
        DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
        SolarDialog* dialog_ptr = NULL;

        while (dialog_current_iterator != dialog_end_iterator)
        {
            dialog_ptr = (*dialog_current_iterator);
            if ((dialog_ptr != NULL) &&
                (dialog_ptr->IsDynamicWindow() == TRUE))
            {
                ++dialog_current_iterator;
                DeleteInterface(dialog_ptr->GetDialogWZ()->GetDlgID());			 
                continue;
            }
            ++dialog_current_iterator;
        }
	}

	ShowWindow( InterfaceManager::DIALOG_MAIN, bFlag );
	ShowWindow( IM_MENU_MANAGER::MAIN_DLG_2, bFlag );

    uiHelpMissionTimer* timer_dialog = 
        GET_CAST_DIALOG(uiHelpMissionTimer, IM_HELP_MANAGER::HELP_MISSION_TIMER_DLG);
    if ((timer_dialog != NULL) &&
        (timer_dialog->IsActiveTimer() == true))
    {
        timer_dialog->ShowInterface(bFlag);
    }

//------------------------------------------------------------------------------ 
#ifdef _YMS_AUTOBOT_GAME
    uiAutobotPlay* play = GET_CAST_DIALOG(uiAutobotPlay, uiAutobotPlay::kDialog_ID);
    if (play != NULL)
    {
        //! 해당 아이템을 장착 하고 있는지 검사
        SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_EQUIPMENT, 
                                                                            eITEMTYPE_AUTO_HUNTING);
        if (item_slot != NULL)
        {
            play->ShowInterface(bFlag);
        }
        else
        {
            play->ShowInterface(FALSE);
        }
    }
#endif //_YMS_AUTOBOT_GAME
//------------------------------------------------------------------------------ 

    if (g_pHero->IsPetReturn())
    {
        uiPetGauge* pet_dialog = GET_CAST_DIALOG(uiPetGauge, IM_PET_MANAGER::PET_GAUGE_DLG);
        if (pet_dialog)
        {
            pet_dialog->ShowInterface(bFlag);
        }
    }

    uiDominationZoneMarkDlg* domination_zonemark_dialog = GET_CAST_DIALOG(uiDominationZoneMarkDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG);
    if (domination_zonemark_dialog)
    {
        domination_zonemark_dialog->ShowInterface(bFlag);
    }
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    uiBase* perk_slot = GET_DIALOG(IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG);
    if (perk_slot != NULL)
    {
        perk_slot->ShowInterface(bFlag);
    }
#endif

#ifdef _YJW_TUTORIAL_SKIP
    if ((g_pMap != NULL) && (g_pMap->IsEsodeMap() == TRUE) && (g_pMap->GetMapID() == TUTO_FIELD))
    {
        uiTutorialSkip* tutorial_skip = GET_CAST_DIALOG(uiTutorialSkip, uiTutorialSkip::kDialog_ID);
        if (tutorial_skip != NULL)
        {
            tutorial_skip->ShowInterface(TRUE);
        }
    }
#endif //_YJW_TUTORIAL_SKIP

#ifdef _DH_BATTLEZONE2_
    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (ui_battlezone2_manager_)
    {
#ifdef _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
        if (g_pMap->IsVill() || g_pMap->IsField() || g_pMap->IsTutorialField())
#else
        if (g_pMap->IsVill() || g_pMap->IsField())
#endif // _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
        {
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_ICON, (bFlag==TRUE));
        }
        else
        {
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_ICON, FALSE);
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LOBBY, FALSE);
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_RESERVATION, FALSE);
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LIST, FALSE);
            ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_TREE, FALSE);
        }
    }
#endif//_DH_BATTLEZONE2_

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    uiQuestList* quest_list = GET_CAST_DIALOG(uiQuestList, IM_QUEST_MANAGER::QUEST_LIST);
    if (quest_list != NULL)
    {
        quest_list->ShowInterface(bFlag);
    }
#endif//_NA_004644_20110506_QUEST_NAVIGATION
}

//------------------------------------------------------------------------------
/**
*/
VOID InterfaceManager::ShowDialogInterface( eDIALOG_ID id )
{
	assert( id < DIALOG_MAX );

	if(id == DIALOG_CHAT)
	{
		uiBattleZoneList * battleZoneList = GET_CAST_DIALOG( uiBattleZoneList, IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST );
		//uiBattleZoneList* battleZoneList =
  //      static_cast<uiBattleZoneList *>(this->GetUserInterface(UI_BATTLEZONE_LIST));
		if(battleZoneList && battleZoneList->IsVisibleWindow())
		{
			GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG)->ShowWindow(FALSE);//g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHAT, FALSE);

			// 배틀존 리스트 창에서의 채팅창 처리
			battleZoneList->AttachChatDialog();
			return;
		}

		BattleLobby * pBattleLobby = 
				(BattleLobby *)GetDialog(DIALOG_BATTLE_LOBBY);
		if(pBattleLobby && pBattleLobby->IsVisibleWindow())
		{
			GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG)->ShowWindow(FALSE);//g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHAT, FALSE);

			// 배틀로비 창에서의 채팅창 처리
			pBattleLobby->AttachChatDialog();
			return;
		}
	}
	

	ShowWindow(id, TRUE);
	return;	
}


//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::ShowWindow( WzID wzDlgID, BOOL val )
{
	SolarDialog *  pDlg = m_pDialogHashTable->GetData(wzDlgID);

    return ShowWindow(pDlg, val);

	//if (pDlg)
	//{
	//	if ( pDlg->IsVisibleWindow() != val )
	//	{
	//		pDlg->ShowWindow( val );
	//	}
	//	else
	//	{
	//	   if(!m_bLockDlg)
	//	   {
	//	 	  this->GetDialogManager()->RequestFocus(pDlg->GetDialogWZ());
	//	   }
	//	}
	//}
	//else
	//{
	//	return FALSE;
	//}

	//return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::ShowWindow( eDIALOG_ID id, BOOL val )
{
    assert( id < DIALOG_MAX );

    SolarDialog * pDlg = m_pDialogHashTable->GetData( m_wzId[id] );

    return ShowWindow(pDlg, val);

    //if (pDlg)
    //{
    //    if ( pDlg->IsVisibleWindow() != val )
    //    {
    //        pDlg->ShowWindow( val );
    //    }
    //    else
    //    {
    //        if(!m_bLockDlg)
    //        {
    //            if (val) 
    //            {
    //                //! 2010.8.9 / i4u4me / 보일때만 포커스를 넣어 준다.
    //                //! 보이지 않는 다이얼로그에 포커스를 계속 넣으면 인터페이스 버튼 클릭이 일어 나지 않는다.
    //                this->GetDialogManager()->RequestFocus(pDlg->GetDialogWZ());
    //            }
    //        }
    //    }
    //}
    //else
    //{
    //    return FALSE;
    //}

    //return TRUE;
}
//------------------------------------------------------------------------------ 
BOOL InterfaceManager::ShowWindow(SolarDialog* dialog, BOOL val)
{
    if (dialog == NULL)
    {
        return FALSE;
    }

    if (dialog->IsVisibleWindow() != val)
    {
        dialog->ShowWindow(val);
    }
    else
    {
        if((m_bLockDlg == FALSE) && (val == TRUE))
        {
            //! 2010.8.9 / i4u4me / 보일때만 포커스를 넣어 준다.
            //! 보이지 않는 다이얼로그에 포커스를 계속 넣으면 인터페이스 버튼 클릭이 일어 나지 않는다.
            GetDialogManager()->RequestFocus(dialog->GetDialogWZ());
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/**
*/
VOID InterfaceManager::NetworkProc(  eDIALOG_ID id, MSG_BASE * pMsg )
{
	SolarDialog * pDlg = m_pDialogHashTable->GetData( m_wzId[id] );
	if( pDlg )
	{
		pDlg->NetworkProc( pMsg );
	}
}

VOID InterfaceManager::NetworkProc( WzID id, MSG_BASE * pMsg )
{
	SolarDialog * pDlg = m_pDialogHashTable->GetData( id );
	if( pDlg )
	{
		pDlg->NetworkProc( pMsg );
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL InterfaceManager::ShowDialog( eDIALOG_ID id, BOOL val )
{
	assert( id < DIALOG_MAX );	

	SolarDialog * pDlg = m_pDialogHashTable->GetData( m_wzId[id] );
	if (pDlg)
	{
		if ( pDlg->IsVisibleWindow() != val )
		{
			pDlg->ShowDialog( val );
		}
		else
		{
		   if(!m_bLockDlg)
		   {
			   this->GetDialogManager()->RequestFocus(pDlg->GetDialogWZ());
		   }
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL InterfaceManager::ShowDialog( WzID wzDlgID, BOOL val )
{
	SolarDialog * pDlg = m_pDialogHashTable->GetData( wzDlgID );
	if (pDlg)
	{
		if ( pDlg->IsVisibleWindow() != val )
		{
			pDlg->ShowDialog( val );
		}
		else
		{
			if(!m_bLockDlg)
			{
				this->GetDialogManager()->RequestFocus(pDlg->GetDialogWZ());
			}
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;	
}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::AddDamageList(DAMAGE_POPUP &popup)
{
	if (GENERALPARAM->GetScreenShotMode() || (GAMEOPTION->GetDamageOutput() == eODO_NONE))
	{
		return;		
	}

    if(m_DamageMgr)
    {
        m_DamageMgr->AddDamageList(popup);
    }
}


//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::ResetDamageList()
{
    if(m_DamageMgr)
    {
        m_DamageMgr->ResetDamageList();
    }
}

static int MakeDigitArray(int value,int *digitarray)
{
	int iDigitcount = 0;

	// 0이어도 한번은 쓰도록해주자
	while (value >= 0)
	{
		DWORD number = value % 10;
		digitarray[iDigitcount] = number;
		value /= 10;
		iDigitcount++;
		if (value == 0) break;
		assert(iDigitcount < 30);
	}

	// 뒤집자~

	int startindex = 0;
	int backindex = iDigitcount - 1;

	while(startindex < backindex)
	{
		digitarray[startindex] ^= digitarray[backindex];
		digitarray[backindex] ^= digitarray[startindex];
		digitarray[startindex] ^= digitarray[backindex];

		startindex++;
		backindex--;
	}
	return iDigitcount;
}

//------------------------------------------------------------------------------
/** 숫자이미지 srcX srcY srcW srcH를 얻어오기
*/
void InterfaceManager::GetNumberImgInfo( eIMAGE_NUMBER_TYPE nIndex, RECT *rcOut, eIMG_TYPE eType)
{
    //현재는 숫자인덱스가 0 하나뿐임 혹 추가시에 정보를 여기서 분기해주거나 멤버변수로저장

    if( nIndex == eIMAGE_NUMBER_LARGE_YELLOW )
    {
        rcOut->left		= 0;//srcx
        rcOut->top		= 9;//srcy
        rcOut->right	= 9;//srcw
        rcOut->bottom	= 13;//srch
    }
    else
    {
        rcOut->left		= 0;//srcx
        rcOut->top		= 0;//srcy
        rcOut->right	= 7;//srcw
        rcOut->bottom	= 9;//srch

        switch( nIndex )
        {
        case eIMAGE_NUMBER_DEFAULT_YELLOW:  rcOut->top = 0; break;
        case eIMAGE_NUMBER_WHITE:       rcOut->top = 128; break;
        case eIMAGE_NUMBER_RED:         rcOut->top = 137; break;
        case eIMAGE_NUMBER_YELLOW:      rcOut->top = 146; break;
        case eIMAGE_NUMBER_GREEN:       rcOut->top = 155; break;
        case eIMAGE_NUMBER_BLUE:        rcOut->top = 164; break;
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		case eIMAGE_NUMBER_ORANGE:      rcOut->top = 184; break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        }

        switch( eType )
        {
        case eIMG_SLASH: 
            rcOut->left  = 70; 
            break;
        case eIMG_DOT:
            rcOut->left  = 90; 
            rcOut->right = 4; 
            break;
        case eIMG_PERCENT:
            rcOut->left  = 78;
            rcOut->right = 11;
            break;
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		case eIMG_STAR:
			rcOut->left  = 95;
			rcOut->right = 9;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        }
    }
}
//------------------------------------------------------------------------------
/** 기호 그리기
*/
POINT InterfaceManager::RenderDrawSign( POINT &rPos, eIMG_TYPE eType, eIMAGE_NUMBER_TYPE nIndex )
{
	//step1:해당 이미지인덱스 정보얻기
	POINT RenderPos;
	RECT rcImg;
	GetNumberImgInfo( nIndex, &rcImg, eType);
	RenderPos.x = rPos.x;
	RenderPos.y = rPos.y;
	g_pSunRenderer->RenderTexture(m_hNumberTexture,
									RenderPos.x,RenderPos.y,	//DEST  x.y(찍을위치)
									rcImg.right,rcImg.bottom,	//DEST  w.h	
									rcImg.left,	rcImg.top,		//SRC	x.y
									rcImg.right,rcImg.bottom);	//SRC	w.h
	//이미지의 끝점을 리턴한다.
	RenderPos.x+=rcImg.right;
	return RenderPos;
}
//------------------------------------------------------------------------------
/** 숫자이미지 그리기(rc는 GetSizeRect를 기준으로함)
*/
POINT InterfaceManager::RenderDrawNumber (RECT &rc, int nNumber, DWORD dwStyle, eIMAGE_NUMBER_TYPE nIndex)
{
	//step1:해당 이미지인덱스 정보얻기
	RECT rcImg;
	GetNumberImgInfo(nIndex, &rcImg, eIMG_NUMBER );
	
	//step2:숫자를 배열로 만들어담아낸다(popmesh에쓰는 함수)
	int digitarray[30];
	int nCnt = MakeDigitArray(nNumber,digitarray);
   
	//기본은 좌측.
	int nXoffset =0;
	int nYoffset =0;
	//========================================
	//	X축정렬체크
	//========================================
	if(dwStyle & DT_CENTER)
	{
		nXoffset = (rc.right-rc.left)/2-(rcImg.right/2);
	}
	else if(dwStyle & DT_RIGHT)
	{
		nXoffset = (rc.right-rc.left) - (nCnt*rcImg.right);
	}
	//========================================
	//	Y축정렬체크
	//========================================
	if(dwStyle & DT_VCENTER)
	{
		nYoffset = (rc.bottom-rc.top)/2-rcImg.right/2;

	}
	else if(dwStyle & DT_BOTTOM)
	{
		nYoffset = (rc.bottom-rc.top) - rcImg.bottom;
	}

	//step5:배열에 담긴갯수(자릿수)만큼돌면서 랜더링한다.
    POINT RenderPos ={0,0};
	for(int i=0;i<nCnt;++i)
	{
		//찍힐위치= (시작위치+ 옵셋값(정렬) + 이미지자릿수*숫자사이gap)
		RenderPos.x	=	rc.left + nXoffset+(i*rcImg.right);
		RenderPos.y =	rc.top	+ nYoffset;

		g_pSunRenderer->RenderTexture(m_hNumberTexture,
										RenderPos.x,RenderPos.y,				//DEST  x.y(찍을위치)
										rcImg.right,rcImg.bottom,				//DEST  w.h	
										rcImg.left+digitarray[i]*rcImg.right,	//SRC	x
										rcImg.top,								//SRC	y
										rcImg.right,rcImg.bottom);				//SRC	w.h
	}
	//이미지의 끝점을 리턴한다.
	RenderPos.x+=rcImg.right;
	return RenderPos;
}

//------------------------------------------------------------------------------
/** 숫자이미지 그리기(아이템 평점 출력용)
*/
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
void InterfaceManager::RenderDrawItemScore(RECT start_rect,int score)
{
    if (score > kItemScoreMax)
    {
        //ui가 숫자 5칸까지여서 최대치를 99999로 설정함
        score = kItemScoreMax;
    }

    int digit_array[30];
    int count = MakeDigitArray(score, digit_array);

    int number_size_x = 6;
    int number_size_y = 11;

    for(int i = 0; i < count; ++i)
    {
        int source_x = 333 + ((number_size_x + 1) * digit_array[i]);
        int source_y = 145;

        float desc_rect_left = start_rect.left + ((number_size_x + 1) * (5 - count + i));

        g_pSunRenderer->RenderTexture(
            item_score_texture,
            desc_rect_left,
            start_rect.top,
            number_size_x,
            number_size_y,
            source_x,
            source_y,
            number_size_x,
            number_size_y);
    }
    // source_x = 숫자 위치 + ((가로 사이즈 + 간격 1픽셀) * n번째 숫자)

}
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderBattleRoomIcon(eBATTLE_ICON_TYPE icon_type,
                          RECT* dest_rect)
{
    this->RenderBattleRoomIcon(icon_type,
        dest_rect->left,
        dest_rect->top,
        dest_rect->right - dest_rect->left,
        dest_rect->bottom - dest_rect->top);
}
//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderBattleRoomIcon(eBATTLE_ICON_TYPE icon_type,
                                            float dest_x, float dest_y,
                                            float dest_width, float dest_height)
{
    if (icon_type >= eBATTLE_ICON_MAX)
    {
        return;
    }

    float source_x = 0.0f;
    float source_y = 0.0f;
    const float sorce_size = 38;

    switch (icon_type)
    {
    case eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM:
        {
            // pc방 슈퍼방장 아이콘
            source_x = 474;
            source_y = 76;
        }
        break;
    case eBATTLE_ICON_ITEM_SUPERMASTER_ROOM:
        {
            // 아이템 슈퍼방장 아이콘
            source_x = 474;
            source_y = 114;
        }
        break;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    case eBATTLE_ICON_GUILD_PVP_ROOM:
        {
            // 2012.2.6 현재 미사용
            return;
        }
        break;
#endif
#ifdef _DH_BATTLEZONE2_
    case eBATTLE_ICON_LOCKED_ROOM:
        {
            // 잠김 방 아이콘
            source_x = 474;
            source_y = 38;
        }
        break;
#endif//_DH_BATTLEZONE2_
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    case eBATTLE_ICON_PARTY_OBTAIN_ROOM:
        {
            // 아이템 모두분배 아이콘
            source_x = 474;
            source_y = 0;
        }
        break;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    default:
        {
            return;
        }
    }//switch

    g_pSunRenderer->RenderTexture(
        battle_icon_texure_,
        dest_x,
        dest_y,
        dest_width,
        dest_height,
        source_x,
        source_y,
        sorce_size,
        sorce_size);

}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderDamageList(DWORD dwTick)
{	
    if(m_DamageMgr)
    {
        m_DamageMgr->RenderDamageList(dwTick);
        
    }
}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::SetMovePointer(BOOL bFlag,WzVector *vPos)
{
	if (bFlag)
	{
		m_bMovePointerFlag = TRUE;
		m_vMovePointerPos = *vPos;
	}
	else
	{
		m_bMovePointerFlag = FALSE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderMovePointer(DWORD dwTick)
{
	if (GENERALPARAM->GetScreenShotMode()) 
	{
		return;
	}

	if (GetShowFlag() == FALSE)
	{
		return;
	}

	if (m_bMovePointerFlag) 
	{
		if (m_pMovePointerUnitDraw) 
		{
			m_pMovePointerUnitDraw->SetPosition(m_vMovePointerPos);
			WzEvent event;	
			
			g_pSunRenderer->ProgressUnitFrame(m_pMovePointerUnitDraw,(float)dwTick,&event);
			g_pSunRenderer->AnimateUnit(m_pMovePointerUnitDraw);
				
			// 외각선 굵기 
			// 오브젝트와 카메라의 거리에 따라 조절할 필요가 있다!
			const WzColor OutlineColor = WzColor_RGBA(235,192,38,100);
			// 외각선 굵기 
			// 오브젝트와 카메라의 거리에 따라 조절할 필요가 있다!
			float OutlineWidth = 0.0350f; 

			RenderUnitDrawOutline(m_pMovePointerUnitDraw,OutlineColor,OutlineWidth);

			g_pSunRenderer->RenderUnit(m_pMovePointerUnitDraw);

		}
	}

}

HANDLE InterfaceManager::GetAreaTargetTextureHandle( eCHAR_TYPE CharacterType )
{
    if (CharacterType == eCHAR_MYSTIC)
    {
        return m_hTextureAreaPointerForMystic;
    }
#ifdef _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
    else if (CharacterType == eCHAR_SHADOW)
    {
        return m_hTextureAreaPointerForShadow;
    }
#endif //_NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if (CharacterType == eCHAR_WITCHBLADE)
    {
        return texture_area_pointer_for_witchblade_;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    return m_hTextureAreaPointer;
}

void InterfaceManager::SetAreaTargetPointerTexture( HANDLE AreaTargetTexture )
{
    if( m_pAreaPointer != NULL )
    {
        m_pAreaPointer->m_hTexDiffuse = AreaTargetTexture;
    }
}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::SetAreaPointer(BOOL bShow, WzVector* vPos, float fSize, WzColor color )
{
	m_AreaColor=color;

	if( vPos == NULL )
	{
		bShow = FALSE;
	}

	if( bShow)
	{
		if (m_vAreaPointerPos.x == vPos->x && m_vAreaPointerPos.y == vPos->y)
		{
			return;
		}

		m_bAreaPointerFlag = TRUE;
		m_vAreaPointerPos = *vPos;

		if(fSize > 0.0f)
		{
			WzRay ray;
			ray.m_wvOrigin = *vPos;
			ray.m_wvOrigin.z += 1.0f;
			ray.m_wvDirection.x = 0.0f;
			ray.m_wvDirection.y = 0.0f;
			ray.m_wvDirection.z = -1.0f;

			float		fT;
			int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT );
			if( iTerrainMeshFocussing < 0 )
			{
				m_vAreaPointerPos = *vPos;
			}
			else
			{
				m_vAreaPointerPos = ray.m_wvOrigin + ray.m_wvDirection * fT;
			}		

			if (g_pMap->IsLoaded())
			{			
				if (m_pAreaPointer)
				{
					g_pMap->GetWorldBase()->DestroyDecal(m_pAreaPointer);
					m_pAreaPointer = NULL;

				}

				WzVector wvNormal = { 0.0f, 0.0f, 1.0f };
				WzVector wvUp = { 0.0f, 1.0f, 0.0f };
				g_pMap->GetWorldBase()->CreateDecal(&m_pAreaPointer,m_vAreaPointerPos,wvNormal,wvUp,fSize,3.0f);
			}
		}
	}
	else
	{
		m_bAreaPointerFlag = FALSE;
		m_vAreaPointerPos.x = 0.0f;	

		if (g_pMap->IsLoaded())
		{			
			if (m_pAreaPointer)
			{
				g_pMap->GetWorldBase()->DestroyDecal(m_pAreaPointer);
				m_pAreaPointer = NULL;
			}
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderAreaPointer(DWORD dwTick)
{

	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_LIGHTING );

	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
	

	g_pSunRenderer->SetColor(m_AreaColor);

	if (m_bAreaPointerFlag)
	{
		if (m_pAreaPointerUnitDraw)
		{
			m_pAreaPointerUnitDraw->SetPosition(m_vAreaPointerPos);
			WzEvent event;	

			g_pSunRenderer->ProgressUnitFrame(m_pAreaPointerUnitDraw,(float)dwTick,&event);
			g_pSunRenderer->AnimateUnit(m_pAreaPointerUnitDraw);
			if (GetShowFlag() == TRUE)
			{
				g_pSunRenderer->RenderUnit(m_pAreaPointerUnitDraw);

				
			}
		}

		if (m_pAreaPointer)
		{
			if (GetShowFlag() == TRUE)
			{
				g_pSunRenderer->RenderSimpleMesh(m_pAreaPointer);
			}
		}
	}

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->SetColor(oldcolor);

}

//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderTargetPointer(DWORD dwTick)
{
	if (g_pMap->IsSnowManEventMap()) 
    {
        return;
    }

	if (GENERALPARAM->GetScreenShotMode()) 
	{
		return;
	}

	if (GetShowFlag() == FALSE)
	{
		return;
	}
			
	if (g_HeroInput.GetCurrentTarget())
	{
		Object* pObject = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
		if (pObject)
		{
			ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
            g_pSunRenderer->SetAlphaBlendMode(AB_LIGHTING);

			WzColor PointerColor = WzColor_RGBA(235, 192, 38, 255);
			WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
			BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
			BOOL bZTest = g_pSunRenderer->IsEnableZTest();

            g_pSunRenderer->SetZBufferWrite( TRUE );	
			g_pSunRenderer->SetZBufferTest( TRUE);
			WzVector vPos = pObject->GetVisiblePos();
			if (pObject->IsKindOfObject(CHARACTER_OBJECT))
			{
				vPos.z += ((Character*)pObject)->GetMeshHeight();
				vPos.z += 0.5f;

				Character* pChr = (Character*)pObject;

				switch( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChr, g_HeroInput.lsForceAttack() ) )
				{
				case RELATIONKIND::Enemy:
					PointerColor = WzColor_RGBA(255, 80, 25, 255);
					break;
				case RELATIONKIND::Friend:
					PointerColor = WzColor_RGBA(120, 180, 20, 255);
					break;
				default:
					PointerColor = WzColor_RGBA(0, 171, 137, 255);
					break;
				}
			}

			m_pTargetPointerUnitDraw->SetPosition(vPos);
			m_pTargetPointerUnitDraw->EnableCullFaceModeIn(TRUE);

			WzEvent event;	
			g_pSunRenderer->ProgressUnitFrame(m_pTargetPointerUnitDraw, (float)dwTick, &event);
			g_pSunRenderer->AnimateUnit(m_pTargetPointerUnitDraw);
			g_pSunRenderer->SetColor(PointerColor);

			St_RenderCommand Command;
			InitRenderCommand(&Command);
			Command.m_iApplySunLight = 0;

			g_pSunRenderer->RenderUnit(m_pTargetPointerUnitDraw, &Command);

			WzColor OutlineColor = PointerColor;
			float OutlineWidth = 0.0350f; 
           
			RenderUnitDrawOutline(m_pTargetPointerUnitDraw, OutlineColor, OutlineWidth);

            g_pSunRenderer->SetColor(oldcolor);
			g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
			g_pSunRenderer->SetZBufferWrite(bZWrite);	
			g_pSunRenderer->SetZBufferTest(bZTest);
		}
	}
 
}

//------------------------------------------------------------------------------
/** 타겟팅 이펙트
	
*/
void InterfaceManager::RenderTargetEffect()
{	
	if (g_pMap->IsSnowManEventMap()) 
    {
        return;
    }

	if (g_pMap->GetTargetingEffect() == INVALID_HANDLE_VALUE) 
    {
        return;
    }
		
	CEffectBody* pEffectBody = g_pEffectManager->GetEffectPtrHandle(g_pMap->GetTargetingEffect()); 
	
	if (!pEffectBody) 
    {
        return;
    }
			
	static DWORD dwOldTargetKey = 0;

	pEffectBody->SetShow(FALSE);
	
	//스크린샷모드에서는 랜더안함
	if (GENERALPARAM->GetScreenShotMode() || (GetShowFlag() == FALSE) ) 
	{
		dwOldTargetKey = 0;
		return;
	}

	//타겟팅이있을때만 업데이트한다.
	if (g_HeroInput.GetCurrentTarget())
	{
		if( Object* pObject = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget()) )
		{
			if ( pObject->IsKindOfObject(PLAYER_OBJECT) || pObject->IsKindOfObject(MONSTER_OBJECT) )
			{
				Character* pCharacter = static_cast<Character*>(pObject);

				//자유이동(점프)일때는 랜더안함
				if (pCharacter->IsFreeMove())
				{
					return;           
				}

				if(pCharacter->IsKindOfObject(MONSTER_OBJECT))
				{
					Monster* pMon = (Monster*)pCharacter;
					if (pMon->IsShadowSummon()) 
					{
						return;
					}
				}

				static WzMatrix wmOldScale;

				//타겟팅이 바뀌었을때만 변경
				if (g_HeroInput.GetCurrentTarget() != dwOldTargetKey)
				{
					dwOldTargetKey = g_HeroInput.GetCurrentTarget();
									
					//피킹볼륨
					WzBoundingVolume *pVolume  = pCharacter->GetPickingBoundingVolume(0);	
						
					//피킹볼륨이 없다면 바운딩볼륨으로
					if(!pVolume)
						pVolume  = pCharacter->GetBoundingVolume();
									
					WzVector vScale;
					SetVector(&vScale,0.7f,0.7f,0.7f);
									
					//볼륨박스대각길이를 기준으로함
					if(pVolume)
					{
						WzVector vLength = pVolume->m_aabb.m_wvMax-pVolume->m_aabb.m_wvMin;
						vLength.z = 0.f;
						float fBoxLength = VectorLength(&vLength)*0.3;

						if(fBoxLength >= 1.f)
							vScale*=fBoxLength;
					}
					MatrixIdentity( &wmOldScale );
					MatrixScaling( &wmOldScale, vScale.x,vScale.y,vScale.z);
					
				}

				//위치값은 지속적으로 갱신한다.	
				WzVector wvWorldPos;
				WzMatrix wmPosMat,mat;
				MatrixIdentity( &mat );														
				VectorTransformCoord( &wvWorldPos, &(pCharacter->GetVisiblePos()), &mat );		
				MatrixTranslation( &wmPosMat, wvWorldPos.x, wvWorldPos.y, wvWorldPos.z );
				pEffectBody->SetShow(TRUE);
				pEffectBody->SetResultMatrix( &wmPosMat,NULL,&wmOldScale);

				return;
			}
		}
	}

	dwOldTargetKey = 0;
}

#ifdef MAP_EDIT_MODE
//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::RenderTerrainTargetPointer(DWORD dwTick)
{
	if (GENERALPARAM->GetScreenShotMode()) 
	{
		return;
	}

	if(GetShowFlag() == FALSE)
	{
		return;
	}

	if (g_HeroInput.GetEditMapObjKey())
	{
		Object *pObject = g_ObjectManager.GetObject(g_HeroInput.GetEditMapObjKey());
		if (pObject)
		{
			if (pObject->IsKindOfObject(MAP_OBJECT))
			{
				MapObject *pMapObj= (MapObject *)pObject;
				WzVector vPos = pObject->GetVisiblePos();
				if(pMapObj->IsTerrain())
				{
					ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
					g_pSunRenderer->SetAlphaBlendMode( AB_LIGHTING);

					WzColor PointerColor = WzColor_RGBA(235,192,38,60);
					WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

					ENUM_CULL_FACE eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
					g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );	
					BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
					BOOL bZTest = g_pSunRenderer->IsEnableZTest();

					g_pSunRenderer->SetZBufferWrite( FALSE );	
					g_pSunRenderer->SetZBufferTest( TRUE);

					float MeshHeight=0.0f;
					WzVector vScale;
					pMapObj->GetScale(&vScale);

					vPos.z += pMapObj->GetMeshHeight() * vScale.z;
					PointerColor = WzColor_RGBA(255,80,25,150);
					m_pTargetPointerUnitDraw->SetScale(10.5f,10.5f,10.5f);
					m_pTargetPointerUnitDraw->SetPosition(vPos);
					m_pTargetPointerUnitDraw->EnableCullFaceModeIn(TRUE);

					WzEvent event;	
					g_pSunRenderer->ProgressUnitFrame(m_pTargetPointerUnitDraw,(float)dwTick,&event);
					g_pSunRenderer->AnimateUnit(m_pTargetPointerUnitDraw);
					g_pSunRenderer->SetColor(PointerColor);

					St_RenderCommand Command;
					InitRenderCommand(&Command);
					Command.m_iApplySunLight = 0;

					g_pSunRenderer->RenderUnit(m_pTargetPointerUnitDraw,&Command);

					WzColor OutlineColor = PointerColor;
					// 외각선 굵기 
					// 오브젝트와 카메라의 거리에 따라 조절할 필요가 있다!
					float OutlineWidth = 0.0350f; 

					RenderUnitDrawOutline(m_pTargetPointerUnitDraw,OutlineColor,OutlineWidth);

					g_pSunRenderer->SetColor(oldcolor);
					g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
					g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
					g_pSunRenderer->SetZBufferWrite( bZWrite );	
					g_pSunRenderer->SetZBufferTest( bZTest);

				}
			}
		}
	}

	m_pTargetPointerUnitDraw->SetScale(2.5f,2.5f,2.5f);
}

#endif 
//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::SaveAppearWindows()
{
    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr)
        {
            if ((dialog_ptr != NULL) &&
                (dialog_ptr->IsVisibleWindow()))
            {
                m_vecShowWindow.push_back(dialog_ptr->GetDialogWZ()->GetDlgID());
            }
        }
        ++dialog_current_iterator;
    }
}



//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::LoadAppearWindows()
{
	vector<DWORD>::iterator dialogkey_current_iterator = m_vecShowWindow.begin();
    vector<DWORD>::iterator dialogkey_end_iterator = m_vecShowWindow.end();
    DWORD dialogkey = 0;
    SolarDialog* dialog_ptr = NULL;

    while (dialogkey_current_iterator != dialogkey_end_iterator)
    {
        dialogkey = *(dialogkey_current_iterator);
        dialog_ptr = m_pDialogHashTable->GetData(dialogkey);
        if (dialog_ptr != NULL)
        {
            dialog_ptr->ShowWindow(TRUE);
        }
        ++dialogkey_current_iterator;
    }

	m_vecShowWindow.clear();
}


//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::HideAllWindows()
{
    g_InterfaceManager.UnLockDlgs();

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr)
        {
            if (dialog_ptr->IsDynamicWindow() == TRUE)
            {
                ++dialog_current_iterator;
                DeleteInterface(dialog_ptr->GetDialogWZ()->GetDlgID());	 
                continue;
            }
            else
            {
                dialog_ptr->ShowWindow(FALSE);
            }
        }
        ++dialog_current_iterator;
    }  
}




//----------------------------------------------------------------------------
/** eROOM_RESULT 참조
*/
BOOL								
InterfaceManager::GetStringZoneError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen )
{
	if ( !pszOutMessage )
		return FALSE;

	BOOL bRet = TRUE;

	int iStringIndex = -1;

	switch (dwErrorCode)
	{
	case RC_ROOM_SUCCESS:
		{
			iStringIndex = eST_RC_ROOM_SUCCESS;
		}
		break;

	case RC_ROOM_FAILED:
		{
			iStringIndex = eST_RC_ROOM_FAILED;
		}
		break;

	case RC_ROOM_NOTEXISTLOBBY:
		{
			iStringIndex = eST_RC_ROOM_NOTEXISTLOBBY;
		}
		break;

	case RC_ROOM_NOTEXISTROOM:
		{
			iStringIndex = eST_RC_ROOM_NOTEXISTROOM;
		}
		break;

	case RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			iStringIndex = eST_CHAO_IS_INAVALIBLE_STATUS;
		}
		break;

	case RC_ROOM_ISNOTMATERPLAYER:
		{
			iStringIndex = eST_RC_ROOM_ISNOTMATERPLAYER;
		}
		break;

	case RC_ROOM_BEFOREENTERZONE:
		{
			iStringIndex = eST_RC_ROOM_BEFOREENTERZONE;
		}
		break;

	case RC_ROOM_INVALIDROOMTYPE:
		{
			iStringIndex = eST_RC_ROOM_INVALIDROOMTYPE;

            MSG_CG_QUEST_COMPLETE_SYN syn;
            if ((g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_VILLAGE) || 
				(g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_FIELD) || 
				(PacketRequestManager::Instance()->is_sent(syn.m_byCategory, syn.m_byProtocol) == true))
            {
                //! 패킷 순서가 
                //!     -> MSG_CG_QUEST_COMPLETE_SYN 
                //!     -> MSG_CG_QUEST_COMPLETE_ACK 
                //!     -> MSG_CG_ZONE_MISSION_LEAVE_SYN
                //!     -> MSG_CG_ZONE_MISSION_LEAVE_ACK
                //! 어야 하는데 MSG_CG_QUEST_COMPLETE_ACK 받기전에 MSG_CG_ZONE_MISSION_LEAVE_SYN 를 보냈을때 에러 발생
                return FALSE;
            }
		
		}
		break;

	case RC_ROOM_INVALIDPREVLOBBY:
		{
			iStringIndex = eST_RC_ROOM_INVALIDPREVLOBBY;
		}
		break;

	case RC_ROOM_INVALID_ROOM_TYPE:
		{
			iStringIndex = eST_RC_ROOM_INVALID_ROOM_TYPE;
		}
		break;

	case RC_ROOM_ALREADY_EXIST_PLAYER:
		{
			iStringIndex = eST_RC_ROOM_ALREADY_EXIST_PLAYER;
		}
		break;

	case RC_ROOM_ALREADY_DOING_TRANSACTION:
		{
			iStringIndex = eST_RC_ROOM_ALREADY_DOING_TRANSACTION;
			return FALSE;
		}
		break;

	case RC_ROOM_CANNOT_JOIN_ROOMTYPE:
		{
			iStringIndex = eST_RC_ROOM_CANNOT_JOIN_ROOMTYPE;
		}
		break;

	case RC_ROOM_NOTEQUAL_PASSWORD:
		{
			iStringIndex = eST_RC_ROOM_NOTEQUAL_PASSWORD;
		}
		break;

	case RC_ROOM_FULL_ROOM:
		{
			iStringIndex = eST_RC_ROOM_FULL_ROOM;
		}
		break;

	case RC_ROOM_INVALID_MAPCODE:			
		{
			iStringIndex = eST_RC_ROOM_INVALID_MAPCODE;
		}
		break;

	case RC_ROOM_INVALID_LIMITLEVEL:			
		{
			iStringIndex = eST_RC_ROOM_INVALID_LIMITLEVEL;
		}
		break;

	case RC_ROOM_INVALID_LIMITCLASS:			
		{
			iStringIndex = eST_RC_ROOM_INVALID_LIMITCLASS;
		}
		break;

	case RC_ROOM_INVALID_LIMITCLASS_FOR_ME:	
		{
			iStringIndex = eST_RC_ROOM_INVALID_LIMITCLASS_FOR_ME;
		}
		break;

	case RC_ROOM_INVALID_LIMITLEVEL_FOR_ME:	
		{
			iStringIndex = eST_RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
		}
		break;

	case RC_ROOM_INVALID_LIMIT_NUMBEROFPLAYER:
		{
			iStringIndex = eST_RC_ROOM_INVALID_LIMIT_NUMBEROFPLAYER;
		}
		break;

	case RC_ROOM_INVALID_BONUS_VALUE:		
		{
			iStringIndex = eST_RC_ROOM_INVALID_BONUS_VALUE;
		}
		break;

	case RC_ROOM_INVALID_FIELD_VALUE:		
		{
			iStringIndex = eST_RC_ROOM_INVALID_FIELD_VALUE;
		}
		break;

	case RC_ROOM_INVALID_DIFFICULTY_VALUE:	
		{
			iStringIndex = eST_RC_ROOM_INVALID_DIFFICULTY_VALUE;
		}
		break;

	case RC_ROOM_INVALID_LASTROOMTYPE:		
		{
			iStringIndex = eST_RC_ROOM_INVALID_LASTROOMTYPE;
		}
		break;

	case RC_ROOM_NOTEXISTVILLAGE:			
		{
			iStringIndex = eST_RC_ROOM_NOTEXISTVILLAGE;
		}
		break;

	case RC_ROOM_ALREADY_READY_STATE:		
		{
			iStringIndex = eST_RC_ROOM_ALREADY_READY_STATE;
		}
		break;

	case RC_ROOM_ALREADY_NOT_READY_STATE:	
		{
			iStringIndex = eST_RC_ROOM_ALREADY_NOT_READY_STATE;
		}
		break;

	case RC_ROOM_IS_NOT_ALL_READY:			
		{
			iStringIndex = eST_RC_ROOM_IS_NOT_ALL_READY;
		}
		break;

	case RC_ROOM_INSUFFICIENT_USER_NUM:		
		{
			iStringIndex = eST_RC_ROOM_INSUFFICIENT_USER_NUM;
		}
		break;

	case RC_ROOM_NOT_CMD_FOR_MASTER:			
		{
			iStringIndex = eST_RC_ROOM_NOT_CMD_FOR_MASTER;
		}
		break;

	case RC_ROOM_NOT_CMD_FOR_MEMBER:			
		{
			iStringIndex = eST_RC_ROOM_NOT_CMD_FOR_MEMBER;
		}
		break;

	case RC_ROOM_NOT_EXIST_MEMBER:			
		{
			iStringIndex = eST_RC_ROOM_NOT_EXIST_MEMBER;
		}
		break;

	case RC_ROOM_NOT_EXIST_ROOM:				
		{
			iStringIndex = eST_RC_ROOM_NOT_EXIST_ROOM;
		}
		break;

	case RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT:
		{
#ifdef _YJW_TUTORIAL_SKIP
            if ((g_pMap->IsEsodeMap() == TRUE) && (g_pMap->GetMapID() == TUTO_FIELD))
            {
                //70551	튜토리얼 미션 '신목의 숲' 입장인원이 초과되어 입장 할 수 없습니다.\n잠시 후 다시 시도해 주십시요.\n\n[Tip]\n화면 하단의 Skip 버튼을 이용하여 다음 지역으로 이동 할 수 있습니다.
                iStringIndex = 70551;
            }
            else
            {
                iStringIndex = eSR_RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT;
            }
#else
			iStringIndex = eSR_RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT;
#endif //#ifdef _YJW_TUTORIAL_SKIP

		}
		break;

	case RC_ROOM_INVALID_PLAYER_STATE:			// 잘못된 플레이어의 상태이다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PLAYER_STATE;
		}
		break;

	case RC_ROOM_NEED_CHAR_LEVEL_10_FOR_PVP:		// PVP 10레벨이상이 필요하다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PVP_LIMIT_LEVEL;	
		}
		break;

	case RC_ROOM_NEED_EVEN_NUMBER:				// PVP 인원 제한이 짝수여야 합니다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PVP_MEMBER_NUM;	
		}
		break;

	case RC_ROOM_NOTEXISTHUNTINGROOM:			// 헌팅방이 존재하지 않는다.
		{
			iStringIndex = eSR_RC_ROOM_NOT_EXIST_HUNTING_ZONE;	
		}
		break;

	case RC_ROOM_INVALID_TEAM:					// 잘못된 팀 번호입니다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PVP_TEAM_INFO;	
		}
		break;

	case RC_ROOM_INVALID_RULE_VALUE:				// 잘못된 룰 값입니다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PVP_RULE_INFO;	
		}
		break;

	case RC_ROOM_INVALID_MODE_VALUE:				// 잘못된 모드 값입니다.
		{
			iStringIndex = eSR_RC_ROOM_INVALID_PVP_MODE_INFO;	
		}
		break;

	case RC_ROOM_INSUFFICIENT_MONEY:
		{
			iStringIndex = eST_NOT_ENOUGH_MONEY;	// 하임이 부족합니다.
		}
		break;

	case RC_ROOM_PREVIOUS_MISSION_CLEAR:
		{
			iStringIndex = eSR_RC_ROOM_MUST_CLEAR_MISSION;
		}
		break;

	case RC_ROOM_LIMITMAP_FOR_ME:
		{
			//5331	참가자 제한설정으로 인해 입장 할 수 없는 맵입니다.
			iStringIndex = 5331;
		}
		break;

	case RC_ROOM_SUCCESS_FOR_GENERAL_JOIN:
		{
			//5332	방에 참가할 수 있습니다.	
			iStringIndex = 5332;
		}
		break;

	case RC_ROOM_SUCCESS_FOR_RECOMMEND_JOIN:
		{
			//5333	추천 레벨방이 존재합니다.
			iStringIndex = 5333;
		}
		break;

	case RC_ROOM_ISNOT_MASTER_OF_PARTY:
		{
			//5334	파티장이 아닙니다.
			iStringIndex = 5334;
		}
		break;

	case RC_ROOM_DOING_COMPETITION:
		{
			//5335	경쟁헌팅이 진행 중이라 대기실을 만들거나 입장 할 수 없습니다.
			iStringIndex = 5335;
		}
		break;

	case RC_ROOM_CANNOT_CHANGE_OPTION_ROOMTYPE:
		{
			//5336	옵션을 변경 할 수 없는 방 타입입니다.
			iStringIndex = 5336;
		}
		break;

	case RC_ROOM_INVALID_CHUNTING_LINK_KEY:
		{
			//5337	링크시킬 경쟁헌팅방이 없습니다.
			iStringIndex = 5337;
		}
		break;

	case RC_ROOM_CAN_JOIN_CHUNTING_THROUGH_LOBBY:
		{
			//5338	로비를 통해서만 경쟁헌팅방에 입장할 수 있습니다.
			iStringIndex = 5338;
		}
		break;

	case RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY:
		{
			//5339	파티원 수가 진행 중인 파티원 수 보다 많아 입장 할 수 없습니다.
			iStringIndex = 5339;
		}
		break;

	case RC_ROOM_NEED_ITEM:
		{
			//5340	필요한 아이템을 소지하고 있지 않습니다.
			iStringIndex = 5340;
		}
		break;

	case RC_ROOM_CHUNTING_PLAYTIMES_LIMIT:
		{
			//5341	하루 동안 진행할 수 있는 경쟁헌팅 참여 횟수가 초과했습니다.
			iStringIndex = 5341;
		}
		break;

	case RC_ROOM_ALREADY_EXIST_LINKED_LOBBY:
		{
			//5342	이미 경쟁헌팅에 링크된 로비방이 있습니다.
			iStringIndex = 5342;
		}
		break;

	case RC_ROOM_NEED2_CLEAR_PRE_MISSION:
		{
			//5343	선행 미션을 완료하셔야 입장이 가능합니다.
			iStringIndex = 5343;
		}
		break;

	case RC_ROOM_PREVIOUS_QUEST_CLEAR:
		{
			//5344	퀘스트를 완료해야만 입장이 가능합니다.
			iStringIndex = 5344;
		}
		break;

	case RC_ROOM_CHAR_BLOCK:
		{
			// 블럭된 캐릭터입니다.
			iStringIndex = 5231;
		}
		break;

	case RC_ROOM_PARTY_MEMBER_ACCESS:
		{
			//파티멤버수가 초과해서 입장할 수 없습니다
			iStringIndex = 5329;
		}
		break;

	case RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM:
		{
			// 5868	슈퍼방장 아이템을 가지고 있지 않습니다.
			iStringIndex = 5868;
		}
		break;

	case RC_ROOM_ISNOT_PCBANG_USER:
		{
			//5869	가맹 PC방이 아닙니다.
			iStringIndex = 5869;
		}
		break;

	case RC_ROOM_CANT_RETURN_TO_TUTORIAL_FIELD_IN_LOBBY:
		{
			//5068	튜터리얼 미션 로비에서는 방 나가기가 가능하지 않습니다. 	
			iStringIndex = 5068;
		}
		break;

	case RC_ROOM_CANT_RETURN_TO_VILLAGE_IN_TUTORIAL_MISSION:
		{
			//5069 튜토리얼 미션에서는 마을로 돌아갈 수 없다.
			iStringIndex = 5069;
		}
		break;

	case RC_ROOM_MISMATCH_LEVEL_IN_PARTY:
		{
			//< 던전에 입장가능한 레벨이 부합하는 파티원이 있다.
			iStringIndex = 5744;	
		}
		break;

	case RC_ROOM_MISMATCH_NUMBER_IN_PARTY:
		{
			//< 5742 던전에 입장가능한 인원이 아니다. 많거나 적거나...
			iStringIndex = 5742;	
		}break;
/*
	case RC_ROOM_CAN_NOT_CREATE_PLACE:
		{
		}
		break;
*/
	case RC_ROOM_MISMATCH_SAME_CLASS_IN_PARTY:
		{
			//< 5741 던전에 입장가능한 클래스 조합이 아니다.
			iStringIndex = 5741;
		}
		break;

	case RC_ROOM_MISMATCH_LEVEL_10_WITH_MASTER:
		{
			//< 5743 파티장과 레벨 10이상 차이 남
			iStringIndex = 5743;
		}
		break;

	case RC_ROOM_NOT_OPEN_TIME:
		{
			//< 저주받은 탑 입장이 가능한 시간이 아닙니다.
			iStringIndex = 5597;
		}
		break;



#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	case RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD:
		{
			//5190	길드에 가입되어 있지 않아 길드전 방을 생성할 수 없습니다.
			iStringIndex = 5190;
		}
		break;

	case RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP:
		{
			//5191	길드전 입장권이 부족합니다.
			iStringIndex = 5191;
		}
		break;

	case RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD:
		{
			//5192	같은 길드원이 아니라 입장할 수 없습니다.
			iStringIndex = 5192;
		}
		break;

	case RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT_USER:
		{
			//5193	유저 수(5명)가 맞지 않아 길드전 방을 생성할 수 없습니다.
			iStringIndex = 5193;
		}
		break;
#endif


	case RC_ROOM_CANNOT_USE_ROOM_TITLE:
		{
			//70022 사용이 금지된 단어입니다.
			iStringIndex = eST_CANNOT_USE_WORD;
		}
		break;
	

	case RC_ROOM_CANNOT_WAYPOINT_PLAYERSTATE:
		{	
			//6007 상점을 연 상태에서 웨이포인트를 이용할 수 없습니다.
			iStringIndex =  6007;
		}
		break;

	case RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL:
		{
			iStringIndex = 6421;
		}
		break;

	case RC_ROOM_CANNOT_ENTER_BEFORE_REWARD_OR_REFUND:
		{
			iStringIndex = 6422;
		}
		break;


	case RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER:
		{
			//저탑 입장이 불가능한상태입니다.
			iStringIndex = 6429;
		}
		break;

	case RC_ROOM_NEED_UNEQUIP_WEAPON_FOR_ENTER:
		{
			//5753: 무기착용된상태에서는 입장할수없습니다.
			iStringIndex = 5753;
		}
		break;

		
	case RC_ROOM_CANNOT_CREATE_RELAY_ROOM_BY_PORTAL_ENTERED:
		{
			//5078: 포탈을 통한 입장시에는 재시작되지 않습니다.
			iStringIndex = 5078;
		}
		break;

	case RC_ROOM_LIMIT_MAX_GUILDMEMBER:
		{
			// 3340: 참여 제한 인원수가 초과되어 입장하실 수 없습니다.
			iStringIndex = 3340;
		}
		break;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    case RC_ROOM_CANNOT_BETAKEY:
        {
            // 70546: 조건이 맞지 않아 해당 지역으로 이동 할 수 없습니다.
            iStringIndex = 70546;
        }
        break;
#endif//_NA_20100307_BETAKEY_SYSTEM
    case RC_ROOM_BATTLE_GROUND_RESERVATION:
        {
            // 2703: 전장 예약중엔 사용불가....
            iStringIndex = 2703;
        }
        break;
    case RC_ROOM_MISMATCH_JOIN_GUILD:
        {
            // 3472 입장 할 수 없는 길드입니다.
            iStringIndex = 3472;
        }
        break;
    case RC_ROOM_NOT_INCLUDE_GUILD:
        {
            // 3471 길드가 존재하지 않습니다.
            iStringIndex = 3471;
        }
        break;
	default:
		{
			Sprintf(pszOutMessage, _T("Room : NOT FOUND ERROR CODE(%d)"), dwErrorCode);
			bRet = FALSE;
			return bRet;
		}
		break;
	};

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return bRet;
}


//----------------------------------------------------------------------------
/** ePVP_RESULT 참조 
*/
/*
BOOL								
InterfaceManager::GetStringPVPError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen)
{
	int iStringIndex = 0;
	BOOL bRet = TRUE;

	switch ( dwErrorCode)
	{
	case RC_PVP_NOT_AT_ZONE:
		{
			//iStringIndex = ;
		}
		break;

	case RC_PVP_INVALID_USER_STATE:
		{
			//iStringIndex = ;
		}
		break;

	default:
		{
			//iStringIndex = ;
			bRet = FALSE;
		}
		break;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return bRet;
}
*/

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::GetStringQuestError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen)
{
	if ( !pszOutMessage )
		return FALSE;

	BOOL bRet = TRUE;

	int iStringIndex = -1;

    switch (dwErrorCode)
	{
	case RC_QUEST_FAILED:
		{
			// 5356	보상을 받을 수 없습니다.
			iStringIndex = 5356;
		}
		break;

	case RC_QUEST_INSUFFICIENT_SPACE:
		{
			//5357	인벤토리가 가득찼습니다.
			iStringIndex = 5357;
		}
		break;

	case RC_QUEST_FULL_MONEY:
		{
			//5358	돈을 더 이상 소유 할 수 없습니다.
			iStringIndex = 5358;
		}
		break;

	case RC_QUEST_EMPTY:
		{
			//5359	보상이 존재하지 않습니다.
			iStringIndex = 5359;
		}
		break;

	case RC_QUEST_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			//// 카오 상태에서는 이용이 가능하지 않습니다.
			iStringIndex = eST_CHAO_IS_INAVALIBLE_STATUS;
		}
		break;

	case RC_QUEST_TOCOMPLETED:				//< 퀘스트가 완료되지 않는다.CanComplete()실패
		{
			//5366	퀘스트를 완료할 수 없습니다.
			iStringIndex = 5365;
		}
		break;

	case RC_QUEST_CANNOTREWARD_CHILDQ:		//< 자식퀘스트는 보상받을 수 없다.
		{
			//5367	하위 퀘스트는 개별적으로 보상받을 수 없습니다.
			iStringIndex = 5367;
		}
		break;

	case RC_QUEST_NOT_COMPLETED_STATE:
	case RC_QUEST_CAN_NOT_INCOMPLET:
		return FALSE;

		//< 피로시간 때문에 보상받을 수 없다.
	case RC_QUEST_CANNOTREWARD_FATIGUE:
		 iStringIndex = 6045;
		 break;


	case RC_QUEST_CANNOTQUEST_MAXCOUNT:
		{
			//5363	퀘스트를 더 이상 수락 할 수 없습니다.
			iStringIndex = 5363;
		}
		break;
#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
	case RC_QUEST_MUST_UNEQUIP_ALL_ITEM:			
		{
			//5327	장비를 착용한 상태에선,퀘스트를 완료할 수 없습니다.
			iStringIndex = 5327;	
		}
		break;
#endif//__NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP

	case RC_QUEST_ALREADY_PROGRESS_TIME_QUEST:
		{
			//5390 타임퀘스트는 두개이상받을수없습니다.
			iStringIndex = 5390;
		}
		break;//< 이미 진행중인 타임 퀘스트가 있다.	

    case RC_QUEST_DISTANCE_DIFF_TOO_LONG:
        {
            // 5391 너무 멀리 떨어져 있습니다.
            iStringIndex = 5391;
        }
        break;
	case RC_QUEST_EXIST:
		{
			// 5466 이미 수락된 퀘스트입니다.
			iStringIndex = 5466;	
		}
		break;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    case RC_QUEST_CANNOT_BETAKEY:
        {   // 70541 조건이 맞지 않아 해당 퀘스트를 받을 수 없습니다.
            iStringIndex = 70541;
        }
#endif//_NA_20100307_BETAKEY_SYSTEM
        
    case RC_QUEST_COMPLETED_DAILY_QUEST://_NA_003027_20111013_HONOR_SYSTEM
        {
            // 6826 이미 완료한 진영의 일일퀘스트입니다.
            iStringIndex = 6826;
        }
        break;
    case RC_QUEST_MAX_DAILY_QUEST_COUNT://_NA_003027_20111013_HONOR_SYSTEM
        {
            iStringIndex = 6496;
        }
        break;
    case RC_QUEST_OVER_REPEAT:
        {
            // 70621 해당 퀘스트의 반복횟수를 초과하였습니다.
            iStringIndex = 70621;
        }
        break;
    case RC_QUEST_MISMATCH_CHARACTER_LV:
        {
            // 70622 해당 퀘스트를 수행할 수 있는 레벨이 아닙니다.
            iStringIndex = 70622;
        }
        break;
    case RC_QUEST_NOT_CHAO:
        {
            // 70623 카오상태가 아니어서 해당 퀘스트를 수행할 수 없습니다.
            iStringIndex = 70623;
        }
        break;
    case RC_QUEST_MISMATCH_CHARACTER_CLASS:
        {
            // 70624 해당 퀘스트를 수행할 수 있는 클래스가 아닙니다.
            iStringIndex = 70624;
        }
        break;
    case RC_QUEST_LESS_MONEY:
        {
            // 70625 하임이 모자라 해당 퀘스트를 수행할 수 없습니다.
            iStringIndex = 70625;
        }
        break;
    case RC_QUEST_NOT_EXIST_NECESSARY_QUEST:
        {
            // 70626 해당 퀘스트를 수행하기 위해 필요한 퀘스트가 없습니다.
            iStringIndex = 70626;
        }
        break;
    case RC_QUEST_MUST_NOT_EXIST_QUEST:
        {
            // 70627 해당 퀘스트와 함께 수행할 수 없는 퀘스트를 수행중입니다.
            iStringIndex = 70627;
        }
        break;
    case RC_QUEST_NOT_EXIST_NECESSARY_ITEM:
        {
            // 70628 해당 퀘스트를 수행하기 위해 필요한 아이템이 없습니다.
            iStringIndex = 70628;
        }
        break;
    case RC_QUEST_MUST_NOT_EXIST_ITEM:
        {
            // 70629 해당 퀘스트를 수행할 수 없는 아이템을 가지고 있습니다.
            iStringIndex = 70629;
        }
        break;
    case RC_QUEST_NOT_KILL_NECESSARY_MONSTER:
        {
            // 70630 퀘스트 완료에 필요한 몬스터 처치 횟수를 만족하지 못했습니다.
            iStringIndex = 70630;
        }
        break;
    case RC_QUEST_NOT_LESS_MISSION_POINT:
        {
            // 70631 퀘스트 완료에 필요한 미션 클리어 횟수를 만족하지 못했습니다.
            iStringIndex = 70631;
        }
        break;
    case RC_QUEST_NOT_CRASH_AREA:
        {
            // 70632 해당 지역은 존재하지 않는 필드입니다.
            iStringIndex = 70632;
        }
        break;
    case RC_QUEST_PLAYER_BEHAVE_UNEXPECTED_WAITING:
        {
            // 70633 플레이이가 비정상 접속 종료 중일 때는 퀘스트를 수행할 수 없습니다.
            iStringIndex = 70633;
        }
        break;
    case RC_QUEST_CHILD:
        {
            // 70634 연속된 퀘스트를 진행하기 위해서는 선행 퀘스트를 진행하셔야 합니다.
            iStringIndex = 70634;
        }
        break;
    case RC_QUEST_NOT_PLAYER:
        {
            // 70635 플레이어 정보를 찾을 수 없습니다.
            iStringIndex = 70635;
        }
        break;
    case RC_QUEST_NOT_DAILY:
        {
            // 70636 일일퀘스트가 아닙니다.
            iStringIndex = 70636;
        }
        break;
    case RC_QUEST_NOT_TODAY:
        {
            // 70637 오늘 가능한 퀘스트가 없습니다.
            iStringIndex = 70637;
        }
        break;
    case RC_QUEST_NOT_FIELD:
        {
            // 70638 해당 지역은 존재하지 않는 필드 입니다.
            iStringIndex = 70638;
        }
        break;
    case RC_QUEST_LESS_HONOR:
        {
            // 70639 해당 퀘스트를 수행하기 위한 명예나 평판이 부족합니다.
            iStringIndex = 70639;
        }
        break;
    case RC_QUEST_NOT_EMPTY_ITEM_SPACE:
        {
            // 70650 아이템 빈 공간이 없습니다.
            iStringIndex = 70650;
        }
        break;
    case RC_QUEST_INCORRECT_POSITION:
        {
            // 70651 위치 정보가 올바르지 않습니다.
            iStringIndex = 70651;
        }
        break;
    case RC_QUEST_NOT_AREAID:
        {
            // 70652 해당 퀘스트에 동일 영역ID가 존재하지 않습니다.
            iStringIndex = 70652;
        }
        break;
    case RC_QUEST_MAX_GUILD_DAILY_QUEST_COUNT:
        {
            // 6902 오늘 수락 받을 수 있는 길드 일일 퀘스트를 모두 받은 상태입니다. 계속해서 이 퀘스트를 진행 하시려면 가지고 있는 길드 일일 퀘스트 중 하나를 포기해주세요.
            iStringIndex = 6902;
        }
        break;
	default:
		{
			Sprintf(pszOutMessage, _T("Quest : NOT FOUND ERROR CODE(%d)"), dwErrorCode);
			bRet = FALSE;
		}
		return FALSE;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::GetStringRewardError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen)
{
	if ( !pszOutMessage )
		return FALSE;

	int iStringIndex = -1;

	switch (dwErrorCode)
	{
	case RC_REWARD_FAILED:
		{
			// 5356	보상을 받을 수 없습니다.
			iStringIndex = 5356;
		}
		break;

	case RC_REWARD_INSUFFICIENT_SPACE:
		{
			//5357	인벤토리가 가득찼습니다.
			iStringIndex = 5357;
		}
		break;

	case RC_REWARD_FULL_MONEY:
		{
			//5358	돈을 더 이상 소유 할 수 없습니다.
			iStringIndex = 5358;
		}
		break;

	case RC_REWARD_EMPTY:
		{
			//5359	보상이 존재하지 않습니다.
			iStringIndex = 5359;			
		}
		break;

		//< 피로시간 때문에 보상받을 수 없다.
	case RC_REWARD_FATIGUE_TIME:
		iStringIndex = 6045;			
		break;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    case RC_REWARD_CANNOT_BETAKEY:
        iStringIndex = 70545;   // 70545: 조건이 맞지 않아 보상을 받을 수 없습니다.
        break;
#endif//_NA_20100307_BETAKEY_SYSTEM
	default:
		Sprintf(pszOutMessage, _T("Reward : NOT FOUND ERROR CODE(%d)"), dwErrorCode);
		return FALSE;
		break;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::GetStringWarehouseError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen )
{
	if ( !pszOutMessage )
		return FALSE;

	int iStringIndex = -1;

	switch (dwErrorCode)
	{
	case RC_WAREHOUSE_NOT_AVAILABLE_SERVICE:
		{
			// 창고 서비스를 이용하실 수 없습니다.

			// 창고를 사용할 수 있는 상태가 아닙니다. : 5064
			GetInterfaceString( 5064, pszOutMessage, iLen );
		}
		break;

	case RC_WAREHOUSE_INSUFFICIENT_MONEY:
		{
			// 하임이 부족 합니다. : 551
			GetInterfaceString(551, pszOutMessage, iLen);
		}
		break;

	case RC_WAREHOUSE_INVALID_STATE:
		{
			// 창고를 사용할 수 있는 상태가 아닙니다. : 5064
			GetInterfaceString( 5064, pszOutMessage, iLen );
		}
		break;

	case RC_WAREHOUSE_UNABLE_FUNCTION_FOR_CURR_LEVEL:
		{
			// 현재 레벨에서 이용할 수 없는 기능입니다. : 5065
			GetInterfaceString( eST_NOT_USE_CURRENT_LEVEL, pszOutMessage, iLen );
		}
		break;

	case RC_WAREHOUSE_UNAVAILABLE_LENGTH:
		{
			// 5141	거리가 멀어 이용이 가능하지 않습니다.
			GetInterfaceString( 5141 , pszOutMessage , iLen );
		}
		break;

	case RC_WAREHOUSE_NEED_PASSWORD:						//< 비번 필요합니다.
		{	
			GetInterfaceString( 5554, pszOutMessage , iLen );
		}
		break;
	case RC_WAREHOUSE_INVALID_PASSWORD:						//< 잘못된 비번입니다.
		{
			GetInterfaceString( 5555, pszOutMessage , iLen );
		}
		break;


	default:
		{
			Sprintf(pszOutMessage, _T("WAREHOUSE : NOT FOUND ERROR CODE(%d)"), dwErrorCode);
			return FALSE;
		}
		break;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::GetStringEnterVillageError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen )
{
	if ( !pszOutMessage )
		return FALSE;

	int iStringIndex = -1;

	switch (dwErrorCode)
	{
	case RC_CHARSELECT_OUTOFRANGEINDEX:		//< 캐릭선택인덱스가 잘못넘어옴
		{
			iStringIndex = 5221;
		}
		break;
	case RC_CHARINFO_DUPLICATEDREQ:
		{
			iStringIndex = 5222;
		}
		break;
	case RC_CONNECTION_NOTYETSERVICE:		//< 아직 서비스 준비중이라서 조금후에 다시 접속하3
		{
			iStringIndex = 5223;
		}
		break;
	case RC_CONNECTION_REQUESTFROMGAMESERVER:	//< 게임 서버에서 강제로 자름
		{
			iStringIndex = 5224;
		}
		break;
	case RC_CONNECTION_DONOTEXIST_LINKSERVER:
		{
			iStringIndex = 5225;	
		}
		break;
	case RC_CONNECTION_ROOM_ALREADY_DOING_TRANSACTION:
		{
			iStringIndex = 5226;
		}
		break;
	case RC_CHARSELECT_MAPCODE_WRONG:			//< 선택한 캐릭터의 맵코드가 잘못됨
		{
			iStringIndex = 5227;
		}
		break;

		// 현 패킷 플로우에 예기치 못한 패킷이 수신되었습니다.
		// 캐릭터 선택창에서 이 에러가 발생하면 C/S간 카테고리,프로토콜 불일치 문제일 수 있습니다.
	case RC_CONNECTION_UNEXPECTED_PACKET:
		{
			iStringIndex = 5228;
		}
		break;
	case RC_CONNECTION_INVALID_CHAR_STATE:		//< 캐릭터 상태가 이상하다.
		{
			iStringIndex = 5229;
		}
		break;
	case RC_CONNECTION_DUPLICATED_CHAR_NAME:	//< 중복되는 캐릭터 이름이 존재한다.
		{
			iStringIndex = 5230;
		}
		break;

	case RC_CONNECTION_CHAR_BLOCK:	//	운영 정책 위반으로 제재 중인 캐릭터입니다. 접속이 가능하지 않습니다.
		{
			iStringIndex = 5231;
		}
		break;
	case RC_CONNECTION_DEL_CHAR:	//< 삭제된 캐릭터 임 
		{			
			iStringIndex = 5040;	// "삭제 대기 중인 캐릭터는 접속할 수 없습니다."
		}
		break;

	default:
		{
			//	마을로 들어가기 실패했습니다!!
			iStringIndex = 70264;
		}
		break;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL								
InterfaceManager::GetStringACError(DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen )
{
	if ( !pszOutMessage )
		return FALSE;

	int iStringIndex = -1;

	switch(dwErrorCode)
	{
	case RC_AC_INVALD_PARAM:
		// 5346 잘못된 정보 입니다.	
		iStringIndex = 5346;
		break;
	case RC_AC_INSUFFICIENT_POINT:
		// 5651	달성 포인트를 넘기지 못했습니다.
		iStringIndex = 5651;
		break;
	case RC_AC_INSUFFICIENT_SPACE:
		// 5652	인벤토리 여유 공간이 없습니다.
		iStringIndex = 5652;
		break;
	case RC_AC_ALREADY_REWARD:
		// 5653	이미 보상을 받았습니다.
		iStringIndex = 5653;
		break;
	case RC_AC_FAILED:
		// 5654	보상 받기에 실패했습니다.
		iStringIndex = 5654;
		break;

	case RC_AC_REWARD_FAIL_FATIGUE:
		// 6046 피로시간이여서 AC보상을 받을수 없습니다.
		iStringIndex = 6046;
		break;

	default:
		{
			Snprintf( pszOutMessage, iLen, _T("AC Result Error(%d)"), dwErrorCode);
			return FALSE;
		}
		break;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return TRUE;
}



BOOL InterfaceManager::GetStringStatusError(DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen )
{
	if ( !pszOutMessage )
		return FALSE;

	int iStringIndex = -1;

	switch(dwErrorCode)
	{
	case RC_STATUS_TRADE_INTERCEPTION_FAILED:
	case RC_STATUS_WHISPER_INTERCEPTION_FAILED:
	default:
		{
			Snprintf( pszOutMessage, iLen, _T("STATUS Result Error(%d)"), dwErrorCode);
			return FALSE;
		}
		break;
	}

	GetInterfaceString(iStringIndex, pszOutMessage, iLen);

	return TRUE;
}




void InterfaceManager::UpdateQuestInfo()
{
	uiQuestMan * pQuestMan = (uiQuestMan *)GetUserInterfaceManager( UIMAN_QUEST );
	if (pQuestMan)
	{
		pQuestMan->UpdateQuestInfo();
	}
}

#ifdef _GS_ADD_BIGEVENT_MSG_1122
void InterfaceManager::ShowBigEventMsg()
{
	uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);

	if(!pQuestMan) return; 

	static BOOL bShow = FALSE;

	//메시지 랜더조건:1.이벤트활성화(program.ini) 2.이벤트퀘스트와동일 3.퀘스트완료가 아닐때
	if(bShow==FALSE && QUEST_REWARD!=pQuestMan->GetStateInQuestListForForward(PROGRAMPARAM->GetQuestCode()))
	{	
		bShow = TRUE;
		WzColor NoticeColor		= WzColor_RGBA(129,254,82,255);
		ShowMessageBox(GetInterfaceString(PROGRAMPARAM->GetStringCode()), PROGRAMPARAM->GetDisplayTime(),&NoticeColor, 0);
	}
	
}
#endif//_GS_ADD_BIGEVENT_MSG_1122
//------------------------------------------------------------------------------
/**
*/
void InterfaceManager::Process(DWORD dwTick)
{
    if( !m_pDialogHashTable )
        return;

    // 아이템슬롯 쿨타임 연출용
    ItemCooltimeManager::Instance()->Process(dwTick);

    for( int i = 0; i < UIMAN_MAX; ++i )
    {
        uiBaseMan* baseMan = this->GetUserInterfaceManager( i );

        if( baseMan )
            baseMan->Process( dwTick );
    }

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr)
        {
            dialog_ptr->Process(dwTick);
        }
        ++dialog_current_iterator;
    }

    GameMessage::Process( dwTick );

	m_dwInterfaceTick += dwTick;
	
}

void InterfaceManager::RenderRect2(RECT rc, WzColor color, DWORD eSpecialTooltip /*= eSPECIAL_TOOLTIP_NONE*/)
{
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;
	BOOL                bZWrite;
	BOOL                bZTest;
	// 괴이한 좌표계(Y가 Z로 바뀌고 범위가 0~ -(해상도))에따라 Mesh를 만든다 -.-
	FRECT fRect;

	fRect.left	= (float)rc.left;
	fRect.right	= (float)rc.right;
	fRect.top	= (float)rc.top;
	fRect.bottom= (float)rc.bottom;

	m_pMessageMesh->m_pVertices[0].x = fRect.left;
	m_pMessageMesh->m_pVertices[0].y = 0.f;
	m_pMessageMesh->m_pVertices[0].z = -fRect.top;

	m_pMessageMesh->m_pVertices[2].x = rc.right;
	m_pMessageMesh->m_pVertices[2].y = 0.f;
	m_pMessageMesh->m_pVertices[2].z = -rc.top;

	m_pMessageMesh->m_pVertices[1].x = rc.left;
	m_pMessageMesh->m_pVertices[1].y = 0.f;
	m_pMessageMesh->m_pVertices[1].z = -rc.bottom;

	m_pMessageMesh->m_pVertices[3].x = rc.right;
	m_pMessageMesh->m_pVertices[3].y = 0.f;
	m_pMessageMesh->m_pVertices[3].z = -rc.top;

	m_pMessageMesh->m_pVertices[4].x = rc.left;
	m_pMessageMesh->m_pVertices[4].y = 0.f;
	m_pMessageMesh->m_pVertices[4].z = -rc.bottom;

	m_pMessageMesh->m_pVertices[5].x = rc.right;
	m_pMessageMesh->m_pVertices[5].y = 0.f;
	m_pMessageMesh->m_pVertices[5].z = -rc.bottom;


	eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	eZFunc = g_pSunRenderer->GetZFunc();


	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest  = g_pSunRenderer->IsEnableZTest();

	g_pSunRenderer->SetZBufferWrite( FALSE );	
	g_pSunRenderer->SetZBufferTest(FALSE);

	g_pSunRenderer->SetColor(color);

	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);

	g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);
	g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);

	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );

	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bZTest );

	// 외각선
	FRECT rcExternal = fRect;

	rcExternal.left-= 0.5f;
	rcExternal.right-= 0.5f;
	rcExternal.top-= 0.5f;
	rcExternal.bottom-= 0.5f;

	g_pSunRenderer->Render2DLine(rcExternal.left, rcExternal.top, rcExternal.right, rcExternal.top, WzColor_RGBA(0,0,0,255));			// 위
	g_pSunRenderer->Render2DLine(rcExternal.left, rcExternal.top, rcExternal.left, rcExternal.bottom, WzColor_RGBA(0,0,0,255));		// 왼쪽
	g_pSunRenderer->Render2DLine(rcExternal.right, rcExternal.top, rcExternal.right, rcExternal.bottom, WzColor_RGBA(0,0,0,255));		// 오른쪽
	g_pSunRenderer->Render2DLine(rcExternal.left, rcExternal.bottom, rcExternal.right, rcExternal.bottom, WzColor_RGBA(0,0,0,255));	// 아래쪽

	// 내부선
	FRECT rcInternal = rcExternal;

	rcInternal.left-= 1.5f;
	rcInternal.right-= 1.5f;
	rcInternal.top-= 1.5f;
	rcInternal.bottom-= 1.5f;

    g_pSunRenderer->Render2DLine(rcInternal.left, rcInternal.top, rcInternal.right, rcInternal.top, WzColor_RGBA(112,90,60,255));			// 위
    g_pSunRenderer->Render2DLine(rcInternal.left, rcInternal.top, rcInternal.left, rcInternal.bottom, WzColor_RGBA(112,90,60,255));		// 왼쪽
    g_pSunRenderer->Render2DLine(rcInternal.right, rcInternal.top, rcInternal.right, rcInternal.bottom, WzColor_RGBA(111,89,59,255));		// 오른쪽
    g_pSunRenderer->Render2DLine(rcInternal.left, rcInternal.bottom, rcInternal.right, rcInternal.bottom, WzColor_RGBA(111,89,59,255));	// 아래쪽
}


void InterfaceManager::RenderRect(RECT rc,WzColor color)
{


    ENUM_CMP_FUNC		eZFunc;
    ENUM_ALPHABLEND		eAlphablendMode;
    ENUM_CULL_FACE      eCullFaceMode;
    BOOL                bZWrite;
    BOOL                bZTest;
    // 괴이한 좌표계(Y가 Z로 바뀌고 범위가 0~ -(해상도))에따라 Mesh를 만든다 -.-

    m_pMessageMesh->m_pVertices[0].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[0].y = 0.f;
    m_pMessageMesh->m_pVertices[0].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[2].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[2].y = 0.f;
    m_pMessageMesh->m_pVertices[2].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[1].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[1].y = 0.f;
    m_pMessageMesh->m_pVertices[1].z = (float)-rc.bottom;

    m_pMessageMesh->m_pVertices[3].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[3].y = 0.f;
    m_pMessageMesh->m_pVertices[3].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[4].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[4].y = 0.f;
    m_pMessageMesh->m_pVertices[4].z = (float)-rc.bottom;

    m_pMessageMesh->m_pVertices[5].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[5].y = 0.f;
    m_pMessageMesh->m_pVertices[5].z = (float)-rc.bottom;


    eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
    eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
    eZFunc = g_pSunRenderer->GetZFunc();


    bZWrite = g_pSunRenderer->IsEnableZWrite();
    bZTest  = g_pSunRenderer->IsEnableZTest();

    g_pSunRenderer->SetZBufferWrite( FALSE );	
    g_pSunRenderer->SetZBufferTest(FALSE);



	WzColor oldColor = g_pSunRenderer->GetDiffuseColor();
    g_pSunRenderer->SetColor(color);	

    DWORD alpharef;
    ENUM_CMP_FUNC alphafunc;
    g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);

    g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);    
    g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

    g_pSunRenderer->SetZFunc(eZFunc);
    g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);

    g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );

    g_pSunRenderer->SetZBufferWrite( bZWrite );	
    g_pSunRenderer->SetZBufferTest( bZTest );

	g_pSunRenderer->SetColor(oldColor);
   
}

void InterfaceManager::RenderRectWithZTest(RECT rc,WzColor color,float fDepth)
{


    ENUM_CMP_FUNC		eZFunc;
    ENUM_ALPHABLEND		eAlphablendMode;
    ENUM_CULL_FACE      eCullFaceMode;
    BOOL                bZWrite;
    BOOL                bZTest;
    // 괴이한 좌표계(Y가 Z로 바뀌고 범위가 0~ -(해상도))에따라 Mesh를 만든다 -.-

    m_pMessageMesh->m_pVertices[0].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[0].y = fDepth;
    m_pMessageMesh->m_pVertices[0].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[2].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[2].y = fDepth;
    m_pMessageMesh->m_pVertices[2].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[1].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[1].y = fDepth;
    m_pMessageMesh->m_pVertices[1].z = (float)-rc.bottom;

    m_pMessageMesh->m_pVertices[3].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[3].y = fDepth;
    m_pMessageMesh->m_pVertices[3].z = (float)-rc.top;

    m_pMessageMesh->m_pVertices[4].x = (float)rc.left;
    m_pMessageMesh->m_pVertices[4].y = fDepth;
    m_pMessageMesh->m_pVertices[4].z = (float)-rc.bottom;

    m_pMessageMesh->m_pVertices[5].x = (float)rc.right;
    m_pMessageMesh->m_pVertices[5].y = fDepth;
    m_pMessageMesh->m_pVertices[5].z = (float)-rc.bottom;


    eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
    eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
    eZFunc = g_pSunRenderer->GetZFunc();


    bZWrite = g_pSunRenderer->IsEnableZWrite();
    bZTest  = g_pSunRenderer->IsEnableZTest();

    g_pSunRenderer->SetZBufferWrite(TRUE);	
    g_pSunRenderer->SetZBufferTest(TRUE);



    g_pSunRenderer->SetColor(color);

    DWORD alpharef;
    ENUM_CMP_FUNC alphafunc;
    g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);

    g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);    
    g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

    g_pSunRenderer->SetZFunc(eZFunc);
    g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);

    g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );

    g_pSunRenderer->SetZBufferWrite( bZWrite );	
    g_pSunRenderer->SetZBufferTest( bZTest );

  
   
}

void InterfaceManager::RenderRectWithTextureAndZTest(RECT area, float fDepth, HANDLE tex_handle)
{
    // LT
    m_pMessageMesh->m_pVertices[0].x = (float)area.left;
    m_pMessageMesh->m_pVertices[0].y = fDepth;
    m_pMessageMesh->m_pVertices[0].z = (float)-area.top;
    m_pMessageMesh->m_pUVVerts[0].u = 0;	
    m_pMessageMesh->m_pUVVerts[0].v = 0;

    // RT
    m_pMessageMesh->m_pVertices[2].x = (float)area.right;
    m_pMessageMesh->m_pVertices[2].y = fDepth;
    m_pMessageMesh->m_pVertices[2].z = (float)-area.top;
    m_pMessageMesh->m_pUVVerts[2].u = 1;
    m_pMessageMesh->m_pUVVerts[2].v = 0;

    // LB
    m_pMessageMesh->m_pVertices[1].x = (float)area.left;
    m_pMessageMesh->m_pVertices[1].y = fDepth;
    m_pMessageMesh->m_pVertices[1].z = (float)-area.bottom;
    m_pMessageMesh->m_pUVVerts[1].u = 0;
    m_pMessageMesh->m_pUVVerts[1].v = 1;

    //RT
    m_pMessageMesh->m_pVertices[3].x = (float)area.right;
    m_pMessageMesh->m_pVertices[3].y = fDepth;
    m_pMessageMesh->m_pVertices[3].z = (float)-area.top;
    m_pMessageMesh->m_pUVVerts[3].u = 1;
    m_pMessageMesh->m_pUVVerts[3].v = 0;

    //LB
    m_pMessageMesh->m_pVertices[4].x = (float)area.left;
    m_pMessageMesh->m_pVertices[4].y = fDepth;
    m_pMessageMesh->m_pVertices[4].z = (float)-area.bottom;
    m_pMessageMesh->m_pUVVerts[4].u = 0 ;
    m_pMessageMesh->m_pUVVerts[4].v = 1;

    //LT
    m_pMessageMesh->m_pVertices[5].x = (float)area.right;
    m_pMessageMesh->m_pVertices[5].y = fDepth;
    m_pMessageMesh->m_pVertices[5].z = (float)-area.bottom;
    m_pMessageMesh->m_pUVVerts[5].u = 1;
    m_pMessageMesh->m_pUVVerts[5].v = 1;

    m_pMessageMesh->m_hTexDiffuse =  tex_handle;


    //------------------------------------------------------------------------------ 
    ENUM_CULL_FACE eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
    g_pSunRenderer->SetCullFaceMode(CF_DRAW_ALL);
    BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
    BOOL bZTest  = g_pSunRenderer->IsEnableZTest();

    g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
    g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

    g_pSunRenderer->SetZBufferWrite(TRUE);	
    g_pSunRenderer->SetZBufferTest(TRUE);
    g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

    m_pMessageMesh->m_hTexDiffuse = INVALID_HANDLE_VALUE;
    g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
    g_pSunRenderer->SetZBufferWrite( bZWrite );	
    g_pSunRenderer->SetZBufferTest( bZTest );
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderRectWithTextureAndZTest(RECT rc, float fDepth,int iRow, int iColumn)
{
	ENUM_CULL_FACE      eCullFaceMode;
	BOOL                bZWrite;
	BOOL                bZTest;

	float minU = (0.0625f/2.f) * (float)iRow + 0.0005f;
	float minV = (0.0625f/2.f) * (float)iColumn + 0.0005f;
	const float cPerUV = (0.0625f/2.f) + 0.0005f;

	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	// LT
	m_pMessageMesh->m_pVertices[0].x = (float)rc.left;
	m_pMessageMesh->m_pVertices[0].y = fDepth;
	m_pMessageMesh->m_pVertices[0].z = (float)-rc.top;
	m_pMessageMesh->m_pUVVerts[0].u = minU;	
	m_pMessageMesh->m_pUVVerts[0].v = minV;

	// RT
	m_pMessageMesh->m_pVertices[2].x = (float)rc.right;
	m_pMessageMesh->m_pVertices[2].y = fDepth;
	m_pMessageMesh->m_pVertices[2].z = (float)-rc.top;
	m_pMessageMesh->m_pUVVerts[2].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[2].v = minV;

	// LB
	m_pMessageMesh->m_pVertices[1].x = (float)rc.left;
	m_pMessageMesh->m_pVertices[1].y = fDepth;
	m_pMessageMesh->m_pVertices[1].z = (float)-rc.bottom;
	m_pMessageMesh->m_pUVVerts[1].u = minU;
	m_pMessageMesh->m_pUVVerts[1].v = minV + cPerUV;

	//RT
	m_pMessageMesh->m_pVertices[3].x = (float)rc.right;
	m_pMessageMesh->m_pVertices[3].y = fDepth;
	m_pMessageMesh->m_pVertices[3].z = (float)-rc.top;
	m_pMessageMesh->m_pUVVerts[3].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[3].v = minV;

	//LB
	m_pMessageMesh->m_pVertices[4].x = (float)rc.left;
	m_pMessageMesh->m_pVertices[4].y = fDepth;
	m_pMessageMesh->m_pVertices[4].z = (float)-rc.bottom;
	m_pMessageMesh->m_pUVVerts[4].u = minU ;
	m_pMessageMesh->m_pUVVerts[4].v = minV + cPerUV;

	//LT
	m_pMessageMesh->m_pVertices[5].x = (float)rc.right;
	m_pMessageMesh->m_pVertices[5].y = fDepth;
	m_pMessageMesh->m_pVertices[5].z = (float)-rc.bottom;
	m_pMessageMesh->m_pUVVerts[5].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[5].v = minV + cPerUV;

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    m_pMessageMesh->m_hTexDiffuse = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
    uiGuildMan * pGuildMan = (uiGuildMan *)this->GetUserInterfaceManager(UIMAN_GUILD);
    if( pGuildMan )
    {
        m_pMessageMesh->m_hTexDiffuse =  pGuildMan->GetGuildMarkTexture();
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
	
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest  = g_pSunRenderer->IsEnableZTest();

	g_pSunRenderer->SetZBufferWrite(TRUE);	
	g_pSunRenderer->SetZBufferTest(TRUE);
	g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

	m_pMessageMesh->m_hTexDiffuse = INVALID_HANDLE_VALUE;
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bZTest );
}

void InterfaceManager::RenderGuildMark4MapObject(WzVector* pPos1, WzVector* pPos2, int iRow, int iColumn)
{
	float minU = (0.0625f/2.f) * (float)iRow + 0.0005f;
	float minV = (0.0625f/2.f) * (float)iColumn + 0.0005f;
	const float cPerUV = (0.0625f/2.f) + 0.0005f;

	WzVector wvHeight = *pPos1 - *pPos2;
	float fLen = VectorLength(&wvHeight);
	fLen = fLen*0.5f;

	// LT
	m_pMessageMesh->m_pVertices[0].x = pPos1->x;
	m_pMessageMesh->m_pVertices[0].y = pPos1->y;
	m_pMessageMesh->m_pVertices[0].z = pPos1->z + fLen;
	m_pMessageMesh->m_pUVVerts[0].u = minU;	
	m_pMessageMesh->m_pUVVerts[0].v = minV;

	// LB
	m_pMessageMesh->m_pVertices[1].x = pPos1->x;
	m_pMessageMesh->m_pVertices[1].y = pPos1->y;
	m_pMessageMesh->m_pVertices[1].z = pPos1->z - fLen;
	m_pMessageMesh->m_pUVVerts[1].u = minU;
	m_pMessageMesh->m_pUVVerts[1].v = minV + cPerUV;

	// RT
	m_pMessageMesh->m_pVertices[2].x = pPos2->x;
	m_pMessageMesh->m_pVertices[2].y = pPos2->y;
	m_pMessageMesh->m_pVertices[2].z = pPos1->z + fLen;
	m_pMessageMesh->m_pUVVerts[2].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[2].v = minV;

	//RT
	m_pMessageMesh->m_pVertices[3].x = pPos2->x;
	m_pMessageMesh->m_pVertices[3].y = pPos2->y;
	m_pMessageMesh->m_pVertices[3].z = pPos1->z + fLen;
	m_pMessageMesh->m_pUVVerts[3].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[3].v = minV;

	//LB
	m_pMessageMesh->m_pVertices[4].x = pPos1->x;
	m_pMessageMesh->m_pVertices[4].y = pPos1->y;
	m_pMessageMesh->m_pVertices[4].z = pPos1->z - fLen;
	m_pMessageMesh->m_pUVVerts[4].u = minU ;
	m_pMessageMesh->m_pUVVerts[4].v = minV + cPerUV;

	//RB
	m_pMessageMesh->m_pVertices[5].x = pPos2->x;
	m_pMessageMesh->m_pVertices[5].y = pPos2->y;
	m_pMessageMesh->m_pVertices[5].z = pPos1->z - fLen;
	m_pMessageMesh->m_pUVVerts[5].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[5].v = minV + cPerUV;

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    m_pMessageMesh->m_hTexDiffuse = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
    uiGuildMan * pGuildMan = (uiGuildMan *)this->GetUserInterfaceManager(UIMAN_GUILD);
    if( pGuildMan )
    {
        m_pMessageMesh->m_hTexDiffuse =  pGuildMan->GetGuildMarkTexture();
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

	g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

	m_pMessageMesh->m_hTexDiffuse = INVALID_HANDLE_VALUE;
}

//이미지 추가나 수정,응용 등의 추가작업이 없을것으로 생각되어 각 이미지들의 RECT를 하드코딩
//혹여나 추가작업이 발생하였을 경우에는 스크립트로 빼야할수도.
void InterfaceManager::RenderGuildMarkBackGround(RECT rc, float fDepth, eGUILD_MARK_RELATION eRelation)
{
	ENUM_CULL_FACE      eCullFaceMode;
	BOOL                bZWrite;
	BOOL                bZTest;
	RECT				rtBack = rc;

	//길드마크의 크기가 24X24(rc == 길드마크의 크기)
	//길드마크 테두리의 크기가 38X38
	rtBack.top -= 7;
	rtBack.bottom += 7;
	rtBack.left -= 7;
	rtBack.right += 7;

	float minU = 0.f;
	float minV = 0.f;
	float cPerUV =  0.f;

	//guild_back.tga상에서의 각 이미지들 좌표값
	switch(eRelation)
	{
	case eGUILD_MARK_RELATION_BLUE:
		minU = 2.5f/256.f;
		minV = 46.5f/256.f;
		cPerUV = 38.f/256.f;
		break;
	case eGUILD_MARK_RELATION_RED:
		minU = 2.5f/256.f;
		minV = 2.5f/256.f;
		cPerUV = 38.f/256.f;
		break;
	case eGUILD_MARK_RELATION_ORANGE:
		minU = 2.5f/256.f;
		minV = 90.5f/256.f;
		cPerUV = 38.f/256.f;
		break;
	case eGUILD_MARK_RELATION_EMPTY:
		minU = 2.5f/256.f;
		minV = 134.5f/256.f;
		cPerUV = 38.f/256.f;
		break;
	}

	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	// LT
	m_pMessageMesh->m_pVertices[0].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[0].y = fDepth;
	m_pMessageMesh->m_pVertices[0].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[0].u = minU;	
	m_pMessageMesh->m_pUVVerts[0].v = minV;

	// RT
	m_pMessageMesh->m_pVertices[2].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[2].y = fDepth;
	m_pMessageMesh->m_pVertices[2].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[2].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[2].v = minV;

	// LB
	m_pMessageMesh->m_pVertices[1].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[1].y = fDepth;
	m_pMessageMesh->m_pVertices[1].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[1].u = minU;
	m_pMessageMesh->m_pUVVerts[1].v = minV + cPerUV;

	//RT
	m_pMessageMesh->m_pVertices[3].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[3].y = fDepth;
	m_pMessageMesh->m_pVertices[3].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[3].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[3].v = minV;

	//LB
	m_pMessageMesh->m_pVertices[4].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[4].y = fDepth;
	m_pMessageMesh->m_pVertices[4].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[4].u = minU ;
	m_pMessageMesh->m_pUVVerts[4].v = minV + cPerUV;

	//LT
	m_pMessageMesh->m_pVertices[5].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[5].y = fDepth;
	m_pMessageMesh->m_pVertices[5].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[5].u = minU + cPerUV;
	m_pMessageMesh->m_pUVVerts[5].v = minV + cPerUV;

	uiGuildMan * pGuildMan = (uiGuildMan *)this->GetUserInterfaceManager(UIMAN_GUILD);
	if( pGuildMan )
	{
		m_pMessageMesh->m_hTexDiffuse =  g_pApplication->GetTextureOnHeadHandle();
	}

	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest  = g_pSunRenderer->IsEnableZTest();

	g_pSunRenderer->SetZBufferWrite(TRUE);	
	g_pSunRenderer->SetZBufferTest(TRUE);
	g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

	m_pMessageMesh->m_hTexDiffuse = INVALID_HANDLE_VALUE;
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bZTest );
}

//이미지 추가나 수정,응용 등의 추가작업이 없을것으로 생각되어 각 이미지들의 RECT를 하드코딩
//혹여나 추가작업이 발생하였을 경우에는 스크립트로 빼야할수도.
void InterfaceManager::RenderCharNameBackGround(LONG lx, LONG ly, LONG lTextx, LONG lTexty, float fDepth, eGUILD_MARK_RELATION eRelation, bool bGuild, bool bGuildMark)
{
	ENUM_CULL_FACE      eCullFaceMode;
	BOOL                bZWrite;
	BOOL                bZTest;
	RECT				rtBack;

	float minU = 0.f;
	float minV = 0.f;
	float fHeight = 0.f;
	float fWidth  = 0.f;

	if(true == bGuildMark)
	{
		//lx는 길드마크의 right값, ly는 길드마크의 top값
		//lTextx,lTexty는 길드명의 사이즈
		//+30은 좌우대칭을 위한 상수.
		rtBack.top = ly - 7;
		rtBack.bottom = ly + lTexty + 7;
		rtBack.left = lx + 7;
		rtBack.right = lx + lTextx + 30;

		//guild_back.tga상에서의 각 이미지들 좌표값
		switch(eRelation)
		{
		case eGUILD_MARK_RELATION_BLUE:
			minU = 43.f/256.f;
			minV = 46.5f/256.f;
			fHeight = 38.f/256.f;
			fWidth	= 97.5f/256.f;
			break;
		case eGUILD_MARK_RELATION_RED:
			minU = 43.f/256.f;
			minV = 2.5f/256.f;
			fHeight = 38.f/256.f;
			fWidth	= 97.5f/256.f;
			break;
		case eGUILD_MARK_RELATION_ORANGE:
			minU = 43.f/256.f;
			minV = 90.5f/256.f;
			fHeight = 38.f/256.f;
			fWidth	= 97.5f/256.f;
			break;
		case eGUILD_MARK_RELATION_EMPTY:
			minU = 43.f/256.f;
			minV = 134.5f/256.f;
			fHeight = 38.f/256.f;
			fWidth	= 97.5f/256.f;
			break;
		}
	}
	else
	{
		if(true == bGuild)
		{
			rtBack.top = ly - 7;
			rtBack.bottom = ly + lTexty + 7;
		}
		else
		{
			rtBack.top = ly + 8;
			rtBack.bottom = ly + lTexty + 8;
		}
		
		rtBack.left = lx;
		rtBack.right = lx + lTextx + 20;

		minU = 141.f/256.f;
		minV = 134.5f/256.f;
		fHeight = 38.f/256.f;
		fWidth	= 111.f/256.f;
	}

	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	// LT
	m_pMessageMesh->m_pVertices[0].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[0].y = fDepth;
	m_pMessageMesh->m_pVertices[0].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[0].u = minU;	
	m_pMessageMesh->m_pUVVerts[0].v = minV;

	// RT
	m_pMessageMesh->m_pVertices[2].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[2].y = fDepth;
	m_pMessageMesh->m_pVertices[2].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[2].u = minU + fWidth;
	m_pMessageMesh->m_pUVVerts[2].v = minV;

	// LB
	m_pMessageMesh->m_pVertices[1].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[1].y = fDepth;
	m_pMessageMesh->m_pVertices[1].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[1].u = minU;
	m_pMessageMesh->m_pUVVerts[1].v = minV + fHeight;

	//RT
	m_pMessageMesh->m_pVertices[3].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[3].y = fDepth;
	m_pMessageMesh->m_pVertices[3].z = (float)-rtBack.top;
	m_pMessageMesh->m_pUVVerts[3].u = minU + fWidth;
	m_pMessageMesh->m_pUVVerts[3].v = minV;

	//LB
	m_pMessageMesh->m_pVertices[4].x = (float)rtBack.left;
	m_pMessageMesh->m_pVertices[4].y = fDepth;
	m_pMessageMesh->m_pVertices[4].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[4].u = minU ;
	m_pMessageMesh->m_pUVVerts[4].v = minV + fHeight;

	//LT
	m_pMessageMesh->m_pVertices[5].x = (float)rtBack.right;
	m_pMessageMesh->m_pVertices[5].y = fDepth;
	m_pMessageMesh->m_pVertices[5].z = (float)-rtBack.bottom;
	m_pMessageMesh->m_pUVVerts[5].u = minU + fWidth;
	m_pMessageMesh->m_pUVVerts[5].v = minV + fHeight;

	uiGuildMan * pGuildMan = (uiGuildMan *)this->GetUserInterfaceManager(UIMAN_GUILD);
	if( pGuildMan )
	{
		m_pMessageMesh->m_hTexDiffuse =  g_pApplication->GetTextureOnHeadHandle();
	}

	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest  = g_pSunRenderer->IsEnableZTest();

	g_pSunRenderer->SetZBufferWrite(TRUE);	
	g_pSunRenderer->SetZBufferTest(TRUE);
	g_pSunRenderer->RealizeRenderSimpleMesh(m_pMessageMesh);

	m_pMessageMesh->m_hTexDiffuse = INVALID_HANDLE_VALUE;
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->SetZBufferTest( bZTest );
}

//------------------------------------------------------------------------------
/**
*/
static DWORD GetIconIDForStatus(DWORD dwStatus)
{
	BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(dwStatus);
	if (pInfo)
	{
		return pInfo->m_dwIconCode;
	}
	else
	{
		return 0xFFFFFFFF;
	}


}

BOOL	InterfaceManager::IsHaveFocusAtEdit()
{
	return TRUE;
}

LPCTSTR  InterfaceManager::GetInterfaceString( int iStringIndex )
{	
	TCHAR	str[INTERFACE_STRING_LENGTH+1] = {0,};
	GetInterfaceString( iStringIndex, str );
	m_tempString.clear();
	m_tempString = str;

	return m_tempString.c_str();	
}


//----------------------------------------------------------------------------
/**
*/
void  
InterfaceManager::GetInterfaceString(int iStringIndex,TCHAR *szText,int bufsize)
{
	assert(bufsize <= INTERFACE_STRING_LENGTH && bufsize > 0);

    if (m_pDialogManager->FindStringTable(iStringIndex,szText) == FALSE)
	{
		Sprintf( szText, _T("ID(%ld) Not Found"), iStringIndex );
		return;
	}

#ifdef _DEV_VER
//#ifdef	_DEBUG_STRINGIDEX_SHOW
    if (CGeneralGameParam::Instance()->IsShowStringId())
    {
	    char szCharText[INTERFACE_STRING_LENGTH] ={0,};
	    StrnCopy( szCharText, szText, INTERFACE_STRING_LENGTH-1 );
	    Sprintf( szText, _T("ID(%ld) %s"), iStringIndex, szCharText );
    }
#endif//_DEBUG_STRINGIDEX_SHOW

}

//------------------------------------------------------------------------------ 
void InterfaceManager::GetInterfaceStringFormat(OUT TCHAR* msg, int msg_size, int format_string_idx, ...)
{
    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };

    if (m_pDialogManager->FindStringTable(format_string_idx, string_format) == FALSE)
    {
        Sprintf(msg, _T("ID(%ld) Not Found"), format_string_idx);
        return;
    }

    va_list args; 
    va_start(args, format_string_idx); 
    _vsntprintf(msg, msg_size, string_format, args); 
    va_end(args); 
}

//----------------------------------------------------------------------------
/**
*/
bool
InterfaceManager::GetInterfaceStringWithCheck(int iStringIndex,TCHAR *szText,int bufsize)
{
    assert(bufsize <= INTERFACE_STRING_LENGTH && bufsize > 0);

    if (m_pDialogManager->FindStringTable(iStringIndex,szText) == FALSE)
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
/**
*/
LPCTSTR 
InterfaceManager::GetItemName( CODETYPE ItemCode )
{
    static TCHAR name[MAX_ITEMNAME_LENGTH];
    if( GetItemName( ItemCode, name, MAX_ITEMNAME_LENGTH ) )
    {
        return name;
    }

    wsprintf(name, "invalid itemcode [%d]", ItemCode );
    return name;
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
InterfaceManager::GetItemName( CODETYPE ItemCode, TCHAR * OUT pszOutNameResult, int iLen )
{
	TCHAR szName1[MAX_ITEMNAME_LENGTH];
	TCHAR szName2[MAX_ITEMNAME_LENGTH];
	TCHAR szResultName[512];

	if (!pszOutNameResult)
	{
		return FALSE;
	}

	if (iLen <= 0)
	{
		return FALSE;
	}

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCode);
	if (!pItemInfo )
	{
		return FALSE;
	}

	if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() )
	{
		GetInterfaceString(pItemInfo->m_NCode, szName1, MAX_ITEMNAME_LENGTH);
		GetInterfaceString(pItemInfo->m_SCNCode, szName2, MAX_ITEMNAME_LENGTH);
#ifdef _RUSSIA
        Sprintf(szResultName, _T("%s %s"), szName2, szName1);
#else
        Sprintf(szResultName, _T("%s %s"), szName1, szName2);
#endif //_RUSSIA
	}
	else
	{
        if(pItemInfo->m_wType == eITEMTYPE_SOCKET_ZARD && pItemInfo->zard_grade > 0)
        {
            TCHAR string_copy_name[512] = {0,};
            GetInterfaceString(pItemInfo->m_NCode, string_copy_name, 512);  // 일반 이름
            TCHAR string_temp_name[128] = {0,};
            // 12119 : 하급, 12120 중급 12121 상급 12122 최상급
            int string_code = 12118 + pItemInfo->zard_grade;
            sprintf(string_temp_name, "%s %s", 
                g_InterfaceManager.GetInterfaceString(string_code), string_copy_name);
            strcpy(szResultName, string_temp_name);
        }
        else
        {
		    GetInterfaceString(pItemInfo->m_NCode, szResultName, 512);
        }
	}

	_tcscpy( pszOutNameResult, szResultName);

	return TRUE;
}

BOOL InterfaceManager::GetShowFlag()
{
	return m_bRender;
}

void InterfaceManager::ShowInterface(BOOL bFlag)
{
	SetRenderState(bFlag);
}

//------------------------------------------------------------------------------
/**
*/
bool
InterfaceManager::InitInterfaceManagerGroups()
{
    for (int i=0; i<UIMAN_MAX; ++i)
    {
        switch (i)
        {
        case UIMAN_RANDOM:
            {
                // base / random
                uiBaseMan* pUIBaseMan = new uiBaseMan(this);
                m_vUserInterfaceMans[UIMAN_RANDOM] = pUIBaseMan;
            }
            break;

        case UIMAN_LOGIN:
            {
                // login
                uiLoginMan* pUIManLogin = new uiLoginMan(this);
                m_vUserInterfaceMans[UIMAN_LOGIN] = pUIManLogin;
            }
            break;

        case UIMAN_PARTY:
            {
                // party
                uiPartyMan* pUIManParty = new uiPartyMan(this);
                m_vUserInterfaceMans[UIMAN_PARTY] = pUIManParty;
            }
            break;

        case UIMAN_BATTLEZONE:
            {
                // battlezone
                uiBattleZoneMan* pUIManBattleZone = new uiBattleZoneMan(this);
                m_vUserInterfaceMans[UIMAN_BATTLEZONE] = pUIManBattleZone;
            }
            break;

        case UIMAN_GUILD:
            {
                // guild
                uiGuildMan* pUIManGuild = new uiGuildMan(this);
                m_vUserInterfaceMans[UIMAN_GUILD] = pUIManGuild;
            }
            break;

        case UIMAN_SKILL:
            {
                // skill
                uiSkillMan* pUIManSkill = new uiSkillMan(this);
                m_vUserInterfaceMans[UIMAN_SKILL] = pUIManSkill;
            }
            break;

        case UIMAN_TRADE:
            {
                // trade
                uiTradeMan* pUIManTrade = new uiTradeMan(this);
                m_vUserInterfaceMans[UIMAN_TRADE] = pUIManTrade;
            }
            break;

		case UIMAN_VENDOR:
            {
                // vendor
                uiVendorMan* pUIManVendor = new uiVendorMan(this);
                m_vUserInterfaceMans[UIMAN_VENDOR] = pUIManVendor;
            }
            break;

        case UIMAN_SYSTEM:
            {
                // system
                uiSystemMan* pUIManSystem = new uiSystemMan(this);
                m_vUserInterfaceMans[UIMAN_SYSTEM] = pUIManSystem;
            }
            break;

		case UIMAN_CHAR_POPUP:
			{
				// char popup
				uiCharPopupMan* pUIManCharPopup = new uiCharPopupMan(this);
				m_vUserInterfaceMans[UIMAN_CHAR_POPUP] = pUIManCharPopup;
			}
			break;

		case UIMAN_HELP:
			{
				// Help
				uiHelpMan* pUIManHelp = new uiHelpMan(this);
				m_vUserInterfaceMans[UIMAN_HELP] = pUIManHelp;
			}
			break;

		case UIMAN_ITEM_COMPOSITE:
			{
				uiItemCompositeMan * pUIManItemComposite = new uiItemCompositeMan(this);
				m_vUserInterfaceMans[UIMAN_ITEM_COMPOSITE] = pUIManItemComposite;
			}
			break;

		case UIMAN_QUEST:
			{
				uiQuestMan * pUIQuestMan = new uiQuestMan(this);
				m_vUserInterfaceMans[UIMAN_QUEST] = pUIQuestMan;
			}
			break;

		case UIMAN_EVENT:
			{
				uiEventMan* pUIManEvent = new uiEventMan(this);
				m_vUserInterfaceMans[UIMAN_EVENT] = pUIManEvent;
			}
			break;

#ifdef _KIKI_UI_WEB_DIALOG
		case UIMAN_WEB:
			{
				uiWebDialogMan* pUIManWeb = new uiWebDialogMan(this);
				m_vUserInterfaceMans[UIMAN_WEB] = pUIManWeb;
			}
			break;
#endif
        case UIMAN_ITEM_DIV:
            {
                uiItemDivMan* pUIManItemDiv = new uiItemDivMan(this);
                m_vUserInterfaceMans[UIMAN_ITEM_DIV] = pUIManItemDiv;
            }
            break;

		case UIMAN_WAYPOINT:
			{
				uiWayPointMan* pUIWayPoint = new uiWayPointMan(this);
				m_vUserInterfaceMans[UIMAN_WAYPOINT] = pUIWayPoint;
			}
			break;

		case UIMAN_PCBANG_EVENT_TIME:
			{
				uiPcbangPlayTimeMan * pUIPcbangPlayTimeMan = new uiPcbangPlayTimeMan(this);
				m_vUserInterfaceMans[UIMAN_PCBANG_EVENT_TIME] = pUIPcbangPlayTimeMan;
			}
			break;

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
		case UIMAN_MISSION_RANK:
			{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
                uiMissionRankMan2* ui_mission_rank_manager2_ptr = new uiMissionRankMan2(this);
                m_vUserInterfaceMans[UIMAN_MISSION_RANK] = ui_mission_rank_manager2_ptr;
#else
				uiMissionRankMan* pUIMissionRank = new uiMissionRankMan(this);
				m_vUserInterfaceMans[UIMAN_MISSION_RANK] = pUIMissionRank;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
			}
			break;
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

		case UIMAN_COMMUNITY:
			{
				uiCommunityMan* pUICommunity = new uiCommunityMan(this);
				m_vUserInterfaceMans[UIMAN_COMMUNITY] = pUICommunity;
			}
			break;

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            //! 2011.7.22 / i4u4me / 캐시샵 작업 

#else
		case UIMAN_CASH_ITEM_SHOP:
			{

				uiCashItemShopMan* pUICashItemShopMan = new uiCashItemShopMan(this);
				m_vUserInterfaceMans[UIMAN_CASH_ITEM_SHOP] = pUICashItemShopMan;
			}
			break;
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

		case UIMAN_PASSWORD:
			{
				uiPasswordMan* pUIPasswordMan = new uiPasswordMan(this);
				m_vUserInterfaceMans[UIMAN_PASSWORD] = pUIPasswordMan;
			}
			break;

		case UIMAN_SOCKET_REMOVE:
			{
				uiSocketRemoveMan * pUISocketRemoveMan = new uiSocketRemoveMan(this);
				m_vUserInterfaceMans[UIMAN_SOCKET_REMOVE] = pUISocketRemoveMan;
			}
			break;

#ifdef __CN_269_MATRIX_CARD_AUTH
		case UIMAN_MATRIX_CARD:
			{
				uiMatrixCardMan * pUIMatrixCardMan = new uiMatrixCardMan(this);
				m_vUserInterfaceMans[UIMAN_MATRIX_CARD] = pUIMatrixCardMan;
			}
			break;
#endif
		case UIMAN_MISSION_RELAY:
			{
				uiMissionRelayMan* pUiMan = new uiMissionRelayMan(this);
				m_vUserInterfaceMans[UIMAN_MISSION_RELAY] = pUiMan;
			}break;


		case UIMAN_RADAR:
			{
				uiRadarMan* pUiMan = new uiRadarMan(this);
				m_vUserInterfaceMans[UIMAN_RADAR] = pUiMan;
			}
			break;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		case UIMAN_OPTIONMENU:
			{
				uiOptionMenuMan* pUiMan = new uiOptionMenuMan(this);
				m_vUserInterfaceMans[UIMAN_OPTIONMENU] = pUiMan;

			}
			break;
#endif 

		case UIMAN_CHAT:
			{
				uiChatMan* pUiMan = new uiChatMan(this);
				m_vUserInterfaceMans[UIMAN_CHAT] = pUiMan;
			}
			break;

		case UIMAN_INVENTORY:
			{
				uiInventoryMan* pUiMan = new uiInventoryMan(this);
				m_vUserInterfaceMans[UIMAN_INVENTORY] = pUiMan;
			}
			break;

		case UIMAN_ASSIST:
			{
				uiAssistMan* pUiMan = new uiAssistMan(this);
				m_vUserInterfaceMans[UIMAN_ASSIST] = pUiMan;
			}
			break;

		case UIMAN_MENU:
			{
				uiMenuMan* pUiMan = new uiMenuMan(this);
				m_vUserInterfaceMans[UIMAN_MENU] = pUiMan;
			}
			break;

		case UIMAN_QUICKCONTAINER:
			{
				uiQuickContainerMan* pUiMan = new uiQuickContainerMan(this);
				m_vUserInterfaceMans[UIMAN_QUICKCONTAINER] = pUiMan;
			}
			break;

		case UIMAN_HEROGAUGE:
			{
				uiHeroGaugeMan* pUiMan = new uiHeroGaugeMan(this);
				m_vUserInterfaceMans[UIMAN_HEROGAUGE] = pUiMan;
			}
			break;

		case UIMAN_BANK:
			{
				uiBankMan* pUiMan = new uiBankMan(this);
				m_vUserInterfaceMans[UIMAN_BANK] = pUiMan;
			}
			break;

		case UIMAN_CREATECHARACTER:
			{
				uiCreateCharacterMan* pUiMan = new uiCreateCharacterMan(this);
				m_vUserInterfaceMans[UIMAN_CREATECHARACTER] = pUiMan;
			}
			break;

		case UIMAN_TARGET:
			{
				uiTargetMan* pUiMan = new uiTargetMan(this);
				m_vUserInterfaceMans[UIMAN_TARGET] = pUiMan;
			}
			break;

		case UIMAN_SUMMONTARGET:
			{
				uiSummonTargetMan* pUiMan = new uiSummonTargetMan(this);
				m_vUserInterfaceMans[UIMAN_SUMMONTARGET] = pUiMan;
			}
			break;

		case UIMAN_REWARD:
			{
				uiRewardMan* pUiMan = new uiRewardMan(this);
				m_vUserInterfaceMans[UIMAN_REWARD] = pUiMan;
			}
			break;

		case UIMAN_ENCHANT:
			{
				uiEnchantMan* pUiMan = new uiEnchantMan(this);
				m_vUserInterfaceMans[UIMAN_ENCHANT] = pUiMan;
			}
			break;
		case UIMAN_COMPOSITE:
			{
				uiCompositeMan* pUiMan = new uiCompositeMan(this);
				m_vUserInterfaceMans[UIMAN_COMPOSITE] = pUiMan;
			}
			break;

		case UIMAN_SUMMONSKILL:
			{
				uiSummonSkillMan* pUiMan = new uiSummonSkillMan(this);
				m_vUserInterfaceMans[UIMAN_SUMMONSKILL] = pUiMan;
			}
			break;

		case UIMAN_CAST:
			{
				uiCastMan* pUiMan = new uiCastMan(this);
				m_vUserInterfaceMans[UIMAN_CAST] = pUiMan;
			}
			break;		

		case UIMAN_BATTLELOBBY:
			{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
				uiBattleLobbyMan* pUiMan = new uiBattleLobbyMan(this);
				m_vUserInterfaceMans[UIMAN_BATTLELOBBY] = pUiMan;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
			}
			break;		

		case UIMAN_SKILLQUEUE:
			{
				uiSkillQueueMan* pUiMan = new uiSkillQueueMan(this);
				m_vUserInterfaceMans[UIMAN_SKILLQUEUE] = pUiMan;
			}
			break;

		case UIMAN_NPCPORTAL:
			{
				uiNPCPortalMan* pUiMan = new uiNPCPortalMan(this);
				m_vUserInterfaceMans[UIMAN_NPCPORTAL] = pUiMan;
			}
			break;
		case UIMAN_TIREDSYSTEM:
			{
				uiTiredSystemMan* pUiMan = new uiTiredSystemMan(this);
				m_vUserInterfaceMans[UIMAN_TIREDSYSTEM] = pUiMan;
			}
			break;

		case UIMAN_CHAO:
			{
				uiChaoSystemMan* pUiMan = new uiChaoSystemMan(this);
				m_vUserInterfaceMans[UIMAN_CHAO] = pUiMan;
			}
			break;

		case UIMAN_AREACONQUEST:
			{
				uiAreaConquestMan* pUiMan = new uiAreaConquestMan(this);
				m_vUserInterfaceMans[UIMAN_AREACONQUEST] = pUiMan;
			}
			break;

		case UIMAN_ENDURANCE:
			{
				uiEnduranceMan* pUiMan = new uiEnduranceMan(this);
				m_vUserInterfaceMans[UIMAN_ENDURANCE] = pUiMan;
			}
			break;
		case UIMAN_SHOP:
			{
				uiShopMan* pUiMan = new uiShopMan(this);
				m_vUserInterfaceMans[UIMAN_SHOP] = pUiMan;
			}
			break;
		case UIMAN_CHARACTERSELECT:
			{
				uiCharacterSelectMan* pUiMan = new uiCharacterSelectMan(this);
				m_vUserInterfaceMans[UIMAN_CHARACTERSELECT] = pUiMan;
			}
			break;

		case UIMAN_CHARACTERSTATUS:
			{
				uiCharacterStatusMan* pUiMan = new uiCharacterStatusMan(this);
				m_vUserInterfaceMans[UIMAN_CHARACTERSTATUS] = pUiMan;
			}
			break;

		case UIMAN_TOOLTIP:
			{
				uiToolTipMan* pUiMan = new uiToolTipMan(this);
				m_vUserInterfaceMans[UIMAN_TOOLTIP] = pUiMan;
			}
			break;


		case UIMAN_TOOLTIP2:
			{
				uiToolTipMan* pUiMan = new uiToolTipMan(this);
				m_vUserInterfaceMans[UIMAN_TOOLTIP2] = pUiMan;
			}
			break;


			case UIMAN_FATE_ITEM_SYSTEM:
			{
				uiFateItemSystemMan * pUIMan = new uiFateItemSystemMan(this);
				m_vUserInterfaceMans[UIMAN_FATE_ITEM_SYSTEM] = pUIMan;
			}
			break;

#ifdef _YJ_FISHING_SYSTEM_000933
			case UIMAN_FISHING_SYSTEM:
			{
				uiFishingMan* pUIMan = new uiFishingMan(this);
				m_vUserInterfaceMans[UIMAN_FISHING_SYSTEM] = pUIMan;
			}
			break;
#endif

			case UIMAN_ITEM_STAT_CHANGE:
				{
					uiItemStatChangeSysMan* pUIMan = new uiItemStatChangeSysMan(this);
					m_vUserInterfaceMans[UIMAN_ITEM_STAT_CHANGE] = pUIMan;
				}
				break;

			case UIMAN_SSQ_SYSTEM:
				{
					uiSSQManager* pUIMan = new uiSSQManager( this );
					m_vUserInterfaceMans[UIMAN_SSQ_SYSTEM] = pUIMan;
				}
				break;

			case UIMAN_PERSONAL_WAYPOINT:
				{
					uiPersonalWayPointMan* pUIMan = new uiPersonalWayPointMan(this);
					m_vUserInterfaceMans[UIMAN_PERSONAL_WAYPOINT] = pUIMan;
				}
				break;


			case UIMAN_PET_MAN:
				{
					uiPetMan* pPetMan = new uiPetMan(this);
					m_vUserInterfaceMans[UIMAN_PET_MAN] = pPetMan;
				}
				break;


		case UIMAN_LOTTO_MAN:
			{
				uiLottoMan * pLottoMan	= new uiLottoMan( this );
				m_vUserInterfaceMans[UIMAN_LOTTO_MAN] = pLottoMan;
			}
			break;
		case UIMAN_NOTICE:
			{
				uiNoticeMan * pNoticeMan	= new uiNoticeMan( this );
				m_vUserInterfaceMans[UIMAN_NOTICE] = pNoticeMan;
			}
			break;
		case UIMAN_DOMINATIONWAR_TOWER:
			{
				uiDominationTowerMan * pDominationTowerMan	= new uiDominationTowerMan( this );
				m_vUserInterfaceMans[UIMAN_DOMINATIONWAR_TOWER] = pDominationTowerMan;
			}
			break;

			case UIMAN_BATTLE_SCORE:
				{
					uiBattleScoreMan * pBattleScoreMan	= new uiBattleScoreMan( this );
					m_vUserInterfaceMans[UIMAN_BATTLE_SCORE] = pBattleScoreMan;
				}
				break;

            case UIMAN_CHAR_CUSTOMIZE:
                {
                    uiCharCustomizeMan* char_customize_man	= new uiCharCustomizeMan(this);
                    m_vUserInterfaceMans[UIMAN_CHAR_CUSTOMIZE] = char_customize_man;
                }
                break;
            case UIMAN_MISSION_OBJECT:
                {
                    uiMissionObjectMan* pUiMan = new uiMissionObjectMan(this);
                    m_vUserInterfaceMans[UIMAN_MISSION_OBJECT] = pUiMan;
                }
                break;

#ifdef _YMS_AUTOBOT_GAME
            case UIMAN_AUTOBOT:
                {
                    m_vUserInterfaceMans[UIMAN_AUTOBOT] = new uiAutobotMan(this);
                }
                break;
#endif //_YMS_AUTOBOT_GAME

            case UIMAN_NEW_SOCKET:
                {
                    m_vUserInterfaceMans[UIMAN_NEW_SOCKET] = new uiSocketMan(this);
                }
                break;
#ifdef _DEV_VER
            case UIMAN_GM_WINDOW:
                {
                    uiGMWindowMan* pUiMan = new uiGMWindowMan(this);
                    m_vUserInterfaceMans[UIMAN_GM_WINDOW] = pUiMan;
                }
                break;
#endif//_DEV_VER// DH_GM_WINDOW
            
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            case UIMAN_CONVENIENCESTORE: m_vUserInterfaceMans[UIMAN_CONVENIENCESTORE] = new uiConvenienceStoreMan(this); break;
#endif

            case UIMAN_OPTION2:
                m_vUserInterfaceMans[UIMAN_OPTION2] = new uiOptionMan(this); break;


            case UIMAN_GUIDE_SYSTEM:
                {
                    m_vUserInterfaceMans[UIMAN_GUIDE_SYSTEM] = new uiGuideSystemMan(this);
                }
                break;

#ifdef _DH_BATTLEZONE2_
            case UIMAN_BATTLE_ZONE_MANAGER:
                {
                    m_vUserInterfaceMans[UIMAN_BATTLE_ZONE_MANAGER] = new uiBattlezone2Man(this);
                }
                break;
#endif//_DH_BATTLEZONE2_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case UIMAN_CHAOS:
                {
                    m_vUserInterfaceMans[UIMAN_CHAOS] = new uiChaosSystemMan(this);
                }
                break;
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER    // 130312 TextureViewerManager 추가 -홍성현-
			case UIMAN_TEXTURE_VIEWER_MANAGER:
				{
					m_vUserInterfaceMans[UIMAN_TEXTURE_VIEWER_MANAGER] = new uiTextureViewerMan(this);
				}
				break;
#endif
#endif

#ifdef _NA_007086_20140318_MONSTERBOOK
            case UIMAN_MONSTERBOOK:
                {
                    m_vUserInterfaceMans[UIMAN_MONSTERBOOK] = new uiMonsterBookMan(this);
                }
                break;
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case UIMAN_FPGAUGE:
                {
                    m_vUserInterfaceMans[UIMAN_FPGAUGE] = new uiFPGaugeMan(this);
                }
                break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
            case UIMAN_ATTENDANCE_CHECK:
                {
                    m_vUserInterfaceMans[UIMAN_ATTENDANCE_CHECK] = new uiAttendanceCheckMan(this);
                }
                break;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
			case UIMAN_SUNRANKING:
				{
					m_vUserInterfaceMans[UIMAN_SUNRANKING] = new uiSUNRankingMan(this);
				}
				break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
			case UIMAN_EQUIPMENT_WAKEUP:
				{
					m_vUserInterfaceMans[UIMAN_EQUIPMENT_WAKEUP] = new uiEquipmentWakeupMan(this);
				}
				break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
			case UIMAN_EQUIPMENT_EVOLUTION:
				{
					m_vUserInterfaceMans[UIMAN_EQUIPMENT_EVOLUTION] = new uiEquipmentEvolutionMan(this);
				}
				break;
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
			case UIMAN_NOTIFICATIONS_SYSTEM:
				{
					m_vUserInterfaceMans[UIMAN_NOTIFICATIONS_SYSTEM] = new uiNotificationsSystemMan(this);
				}
				break;
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
			case UIMAN_MINIGAME:
				{
					m_vUserInterfaceMans[UIMAN_MINIGAME] = new uiMiniGameMan(this);
				}
				break;
#endif // _NA_008405_20150716_MINIGAME_BINGO
		}		

        if (m_vUserInterfaceMans[i])
        {
            m_vUserInterfaceMans[i]->Init(m_pDraw);
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::ReleaseInterfaceManagerGroups()
{
    for (int i=0; i<UIMAN_MAX; ++i)
    {
        if( uiBaseMan* baseMan = GetUserInterfaceManager(i) )
        {
            baseMan->Release();
            delete baseMan;

            m_vUserInterfaceMans[i] = NULL;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::UpdateUserInterfaceManagerNetworkMessage(MSG_BASE *pMsg)
{
    for (int i=0; i<UIMAN_MAX; ++i)
    {
        uiBaseMan* baseMan = this->GetUserInterfaceManager(i);

        if (baseMan)
        {
            baseMan->NetworkProc(pMsg);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::UpdateUserInterfaceManagetNetworkMessageWorld(MSG_BASE *pMsg)
{
    for (int i=0; i<UIMAN_MAX; ++i)
    {
        uiBaseMan* baseMan = this->GetUserInterfaceManager(i);

        if (baseMan)
        {
            baseMan->NetworkProcWorld(pMsg);
        }
	}
}


//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::PreRenderInterfaceManager(DWORD dwTick)
{
	for (int i=0; i<UIMAN_MAX; ++i)
	{
		uiBaseMan* baseMan = this->GetUserInterfaceManager(i);

		if (baseMan)
		{
			baseMan->PreRenderInterface(dwTick);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::PostRenderInterfaceManager(DWORD dwTick)
{
	for (int i=0; i<UIMAN_MAX; ++i)
	{
		uiBaseMan* baseMan = this->GetUserInterfaceManager(i);

		if (baseMan)
		{
			baseMan->PostRenderInterface(dwTick);
		}
	}
}

//------------------------------------------------------------------------------
/**
    현재 사용중인 스킬정보창의 슬롯컨테이너를 반환한다.
*/
SCSlotContainer*
InterfaceManager::GetUISkillSlotContainer()
{
    if(GetUISkillCurrentDialog())
    {
        return GetUISkillCurrentDialog()->GetSlotContainer();
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
    현재 사용중인 스킬정보창의 다이얼로그 ID 값을 반환한다.
*/
uiBase* InterfaceManager::GetUISkillCurrentDialog()
{
    uiSkillMan* ui_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);

    DWORD dlg_key = ui_man->GetCurrentDialogKey();
    if (dlg_key == IM_SKILL_MANAGER::SKILL_TREE_DLG)
    {
        return GET_CAST_DIALOG(uiSkillTree, IM_SKILL_MANAGER::SKILL_TREE_DLG); 
    }
    else if (dlg_key == IM_SKILL_MANAGER::SKILL_PERK_DLG)
    {
      return GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG); 
    }
    else if (dlg_key == IM_SKILL_MANAGER::SKILL_ACTION_DLG)
    {
        return GET_CAST_DIALOG(uiSkillAction, IM_SKILL_MANAGER::SKILL_ACTION_DLG); 
    }

    return NULL;
}
//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::ReadySkillInfos()
{
    uiSkillMan *pUISkillMan =
        static_cast<uiSkillMan *>(GetUserInterfaceManager(UIMAN_SKILL));
    assert (pUISkillMan);

    pUISkillMan->ResetOnceInitSkillInfos();
}


//------------------------------------------------------------------------------
/**
*/
void                                
InterfaceManager::RefreshQuickSlotFromSkillInfos()
{
	uiSkillMan *pUISkillMan =
		static_cast<uiSkillMan *>(GetUserInterfaceManager(UIMAN_SKILL));
	assert (pUISkillMan);

	if ( !pUISkillMan )
		return;

	pUISkillMan->RefreshValidSkillFromQuick();
}


//------------------------------------------------------------------------------
/**
    스킬창에 채울 데이터를 처리한다.
*/
void
InterfaceManager::InitSkillInfos()
{
    uiSkillMan *pUISkillMan =
        static_cast<uiSkillMan *>(GetUserInterfaceManager(UIMAN_SKILL));
    assert (pUISkillMan);

    pUISkillMan->PreloadSkillInvtoryStoreDatas();
}
//------------------------------------------------------------------------------
/**
    서버로부터 내려받는 스킬정보를 한번만 채우고 이때 모든 스킬창에 채우게 된다.
*/
void
InterfaceManager::SetSkillInfos( const SKILL_TOTAL_INFO & SkillInfo)
{
    uiSkillMan *pUISkillMan =
        static_cast<uiSkillMan *>(GetUserInterfaceManager(UIMAN_SKILL));
    assert (pUISkillMan);

    pUISkillMan->SetSkillInfos(SkillInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::CloseUIPartys()
{
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::ToggleUIPartys()
{
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::ShowUIPartys()
{
}

//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::ResetUIPartys()
{
    this->CloseUIPartys();
    this->ShowUIPartys();
}

//----------------------------------------------------------------------------
/** 라이더 rect안에 그리기
*/
void InterfaceManager::RenderRiderOnRect(Rider *pRider,RECT viewport,float fVolumeWidth,float fVolumeHeight,WzColor wzColor/*=0xffffffff*/)
{
	if(!pRider) return;

	BOOL bOld2dMode,bOldRealSize;
	BOOL bZWrite,bZTest;
	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;
	ENUM_MESH_DRAW      eDrawMode;
	eDrawMode = g_pSunRenderer->GetMeshDrawMode();
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	eZFunc = g_pSunRenderer->GetZFunc();
	eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
	g_pSunRenderer->Set2DMode( FALSE );

	WzMatrix mat;
	MatrixIdentity(&mat);
	float fZ = 1.0f;
	g_pSunRenderer->SetWorldMatrix(&mat);
	WzVector vCameraFrom={0.0, 0.0, 0.0};
	WzVector vCameraTo={0.0, 100.0, 0.0};
	WzVector vUp={0.0, 0.0, 1.0};
	g_pSunRenderer->SetCameraMatrix(&vCameraFrom, &vCameraTo, &vUp);
	g_pSunRenderer->SetViewport(&viewport);
	BOOL bFog = g_pSunRenderer->IsFogEnabled();
	g_pSunRenderer->EnableFog(FALSE);
	g_pSunRenderer->ClearRender( CR_DEPTH, NULL, &fZ,NULL,&viewport);  
	g_pSunRenderer->SetOrthogonalMatrix(0.0, fVolumeWidth, 0, fVolumeHeight, 1, 220);
	g_pSunRenderer->SetZBufferWrite(TRUE);
	g_pSunRenderer->SetZBufferTest(TRUE);

	static WzVector _add={fVolumeWidth / 2.0f, 5.f, 0.f};
	pRider->SetPosition(&_add);
	//라이더 View빛적용을 안하겠다
    static int nFlag = 0;
	St_RenderCommand Command;
	InitRenderCommand(&Command);
    Command.m_iApplySunLight=nFlag;
	pRider->Render(&Command);
	g_pSunRenderer->FlushReservedDrawCall();
	g_pSunRenderer->SetViewport(NULL);
	g_Camera.RestoreProjection();
	g_pSunRenderer->Set2DMode( bOld2dMode ,bOldRealSize);			
	g_pSunRenderer->EnableFog(bFog);
	g_pSunRenderer->SetZBufferWrite(bZWrite);
	g_pSunRenderer->SetZBufferTest(bZTest);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetMeshDrawMode( eDrawMode );
	g_pSunRenderer->EnableAlphaTest(FALSE);
}

//----------------------------------------------------------------------------
/**
*/
void   
InterfaceManager::RenderObjectOnRect(Object *pObject,RECT viewport,float fVolumeWidth,float fVolumeHeight, WzColor wzColor /*= 0xffffffff*/)
{
	WzVector m_CameraFrom, m_CameraTo, m_wvZ;
	SetVector(&m_CameraFrom, 0.0, 0.0, 0.0);
	SetVector(&m_CameraTo, 0.0, 100.0, 0.0);
	SetVector(&m_wvZ, 0.0, 0.0, 1.0);

	BOOL bOld2dMode,bOldRealSize;
	BOOL bZWrite,bZTest;
	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;
	WzColor             oldcolor;
	ENUM_MESH_DRAW      eDrawMode;
	eDrawMode = g_pSunRenderer->GetMeshDrawMode();
	oldcolor = g_pSunRenderer->GetDiffuseColor();
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	eZFunc = g_pSunRenderer->GetZFunc();
	eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
	g_pSunRenderer->Set2DMode( FALSE );

	

	WzMatrix mat;
	MatrixIdentity(&mat);

	float fZ = 1.0f;

	g_pSunRenderer->SetWorldMatrix(&mat);
	g_pSunRenderer->SetCameraMatrix(&m_CameraFrom, &m_CameraTo, &m_wvZ);
	g_pSunRenderer->SetViewport(&viewport);
	BOOL bFog = g_pSunRenderer->IsFogEnabled();
	g_pSunRenderer->EnableFog(FALSE);
	g_pSunRenderer->ClearRender( CR_DEPTH, NULL, &fZ,NULL,&viewport);  

	int iWidth = viewport.right - viewport.left;
	int iHeight = viewport.bottom - viewport.top; 		

	g_pSunRenderer->SetOrthogonalMatrix(0.0, fVolumeWidth, 0, fVolumeHeight, 1, 220);
	g_pSunRenderer->SetZBufferWrite(TRUE);
	g_pSunRenderer->SetZBufferTest(TRUE);

	WzVector _add;
	SetVector(&_add, fVolumeWidth / 2.0f, 3.f, 0.f );
	
	pObject->GetBasePart()->SetPosition(_add);
	pObject->SetDirection( m_CameraFrom, m_CameraTo,TRUE);	

	WzVector vScale;

	pObject->GetScale(&vScale);
	pObject->SetScale(1.0f);	
	

	Character *pChar = (Character*)pObject;

	BYTE byOldState = pChar->GetHideSkill();
	pChar->SetForceHideSkill(0);
	pObject->Render();
	pChar->SetForceHideSkill(byOldState);
	

	pObject->SetColor( wzColor );

#ifdef __NA_1102_20080908_BUGFIX_EFFECT_WASNT_DISPLAYED_IN_UI
    /*
        UI상에서 이펙트 안 나오는 버그 수정
    
        - Move 파라미터 33은 프레임틱 
           외부에서 받아야 하나 그렇게 할 경우 많은 부분을 건드려야 하는 관계로 
           그냥 33(초당 30프레임 기준)으로 설정함
    
        - 이펙트 전체를 Move하고 렌더링 하기 때문에 성능이 크게 떨어질 수 있음
          하지만 대체로 UI상에서만 벌어질 일이라 크게 걱정 하지 않아도 될 지 모름
     */
	if(pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        g_pSunRenderer->x_pEffectManager->Move( 33 );
        g_pSunRenderer->x_pEffectManager->Render();
    }
#endif

	g_pSunRenderer->FlushReservedDrawCall();

	g_pSunRenderer->SetViewport(NULL);
	g_Camera.RestoreProjection();
	g_pSunRenderer->Set2DMode( bOld2dMode ,bOldRealSize);			
	g_pSunRenderer->EnableFog(bFog);
	g_pSunRenderer->SetZBufferWrite(bZWrite);
	g_pSunRenderer->SetZBufferTest(bZTest);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetMeshDrawMode( eDrawMode );
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->EnableAlphaTest(FALSE);

	pObject->SetScale(vScale);
}
//------------------------------------------------------------------------------ 
void Calc_BoundingVolume(IN const WzBoundingVolume& bounding_volume, OUT WzVector& gab, OUT WzVector& avg)
{
    SetVector(&gab, 0, 0, 0);
    SetVector(&avg, 0, 0, 0); 
    switch(bounding_volume.m_eBvType)
    {
    case BVT_AABB:
        {
            gab = (bounding_volume.m_aabb.m_wvMax - bounding_volume.m_aabb.m_wvMin);
            avg = (bounding_volume.m_aabb.m_wvMax + bounding_volume.m_aabb.m_wvMin) * 0.5f;
        }
        break;
    case BVT_SPHERE:
        {
            gab.z = bounding_volume.m_BvSphere.m_fRadius;
            avg.z = bounding_volume.m_BvSphere.m_fRadius;
        }
        break;                 
    }
}
//------------------------------------------------------------------------------ 
void Calc_OrthogonalRect(IN RECT& area, IN float display_length, IN FPOINT org, IN float zoom, OUT FRECT& orthogonal_rect)
{
    float screen_ratio = float (area.bottom - area.top) / float(area.right - area.left);
    float len = max(0.5f, display_length);
    float size_ratio = screen_ratio/*1.0f*/ / len;
    float width = 1.0f / size_ratio * zoom;
    float height = screen_ratio / size_ratio * zoom;

    orthogonal_rect.left   = width  * (org.x - 0.5f); 
    orthogonal_rect.right  = width  * (org.x + 0.5f);
    orthogonal_rect.top    = height * (org.y - 0.5f);
    orthogonal_rect.bottom = height * (org.y + 0.5f);
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderUnitToTexture(CWzUnitDraw* unit_draw, HANDLE tex_handle, 
                                           WzVector& pos, WzVector& rot, FPOINT org, float zoom, 
                                           DWORD clear_flags, WzColor& clear_color)
{
    if (unit_draw == NULL)
    {
        return;
    }
    St_TextureContainer* tex_info = m_pDraw->x_pManagerTexture->GetTextureInfo(tex_handle);
    if (tex_info == NULL)
    {
        return;
    }

    RECT area;
    area.left = 0;
    area.right = tex_info->m_nWidth;
    area.top = 0;
    area.bottom = tex_info->m_nHeight;

    g_pSunRenderer->x_pManagerTexture->SetRenderTarget(tex_handle);

    //! 텍스쳐 사이즈로 뷰포트를 설정한다.
    InterfaceManager::Begin2DMode(&area, clear_flags, &clear_color);
    {
        WzVector gab, avg;
#if WZENGINEVER >= 410
        Calc_BoundingVolume(unit_draw->GetBaseBoundingVolume(), gab, avg);
#else
        Calc_BoundingVolume(unit_draw->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, OUT frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);

        unit_draw->SetPosition(pos);
        unit_draw->SetRotation(rot);
        g_pSunRenderer->AnimateUnit(unit_draw);
        g_pSunRenderer->RenderUnit(unit_draw);
        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
    #ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(unit_draw->GetBaseBoundingVolume()));
    #endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();

    g_pSunRenderer->FlushReservedDrawCall(FALSE);
    g_pSunRenderer->x_pManagerTexture->SetRenderTarget();
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderUnitDrawOnRect(CWzUnitDraw* unit_draw, RECT area, float rot_radin_value, FPOINT org, float zoom)
{
    if (unit_draw == NULL)
    {
        return;
    }

    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );

    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)));
    {
        WzVector gab, avg;
#if WZENGINEVER >= 410
        Calc_BoundingVolume(unit_draw->GetBaseBoundingVolume(), gab, avg);
#else
        Calc_BoundingVolume(unit_draw->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, OUT frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);

        //WzMatrix mat_world, mat_pos, mat_rot;
        //MatrixTranslation(&mat_pos, -avg.x, -avg.y, -avg.z); 
        //MatrixRotationZ(&mat_rot, rot_radin_value);
        //MatrixMultiply(&mat_world, &mat_rot, &mat_pos);
        //unit_draw->Process(&mat_world); 
        unit_draw->SetRotation(0, 0, rot_radin_value);
        unit_draw->SetPosition(-avg);
        g_pSunRenderer->AnimateUnit(unit_draw);
        g_pSunRenderer->RenderUnit(unit_draw);

        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
#ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(unit_draw->GetBaseBoundingVolume()));
#endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();

    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderUnitDrawOnRect(CWzUnitDraw* unit_draw,
                                            RECT area, 
                                            WzVector& pos, 
                                            WzVector& rot, 
                                            FPOINT org, 
                                            float zoom, 
                                            DWORD clear_flags, 
                                            WzColor& clear_color)
{
    if (unit_draw == NULL)
    {
        return;
    }

    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );

    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)), clear_flags, &clear_color);
    {
        WzVector gab, avg;
#if WZENGINEVER >= 410
        Calc_BoundingVolume(unit_draw->GetBaseBoundingVolume(), gab, avg);
#else
        Calc_BoundingVolume(unit_draw->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, OUT frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);

        unit_draw->SetPosition(pos);
        unit_draw->SetRotation(rot);
        g_pSunRenderer->AnimateUnit(unit_draw);
        g_pSunRenderer->RenderUnit(unit_draw);
        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
    #ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(unit_draw->GetBaseBoundingVolume()));
    #endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();

    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderPetOnRect(Pet* pet, WzBoundingVolume* bounding_volume, RECT area, float rot_radin_value, FPOINT org, float zoom)
{
    if (pet == NULL || pet->GetBasePart() == NULL)
    {
        return;
    }


    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );

    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)));
    {
        WzVector gab, avg;
        WzBoundingVolume bv;
        if (bounding_volume != NULL)
        {
            bv = *bounding_volume;
        }
        else
        {
#if WZENGINEVER >= 410
            bv = pet->GetBasePart()->GetBaseBoundingVolume();

            WzVector scale = {1, 1, 1};
            if (pet->GetBasePart() && 
                pet->GetBasePart()->GetWzUnit() && 
                pet->GetBasePart()->GetWzUnit()->GetSubType(0))
            {
                pet->GetBasePart()->GetWzUnit()->GetSubType(0)->GetScale(&scale);

                bv.m_aabb.m_wvMax.x *= scale.x;
                bv.m_aabb.m_wvMax.y *= scale.y;
                bv.m_aabb.m_wvMax.z *= scale.z;
                bv.m_aabb.m_wvMin.x *= scale.x;
                bv.m_aabb.m_wvMin.y *= scale.y;
                bv.m_aabb.m_wvMin.z *= scale.z;
            }
#else
            bv = pet->GetBasePart()->GetBoundingVolume();
#endif //#if WZENGINEVER >= 410
        }
        Calc_BoundingVolume(bv, gab, avg);

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 200); 

        //pet->SetPosition(-avg);
        pet->GetBasePart()->SetPosition(-avg);
        pet->SetAngle(rot_radin_value, TRUE);
        pet->Render();
        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
    #ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(pet->GetBasePart()->GetBaseBoundingVolume()));
    #endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();

    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
}
//------------------------------------------------------------------------------ 
void InterfaceManager::RenderRiderOnRect(Rider* rider, RECT area, float rot_radin_value, FPOINT org, float zoom)
{
    if (rider == NULL || rider->GetRenderPart() == NULL)
    {
        return;
    }

    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );

    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)));
    {
        WzVector gab, avg;
#if WZENGINEVER >= 410
        Calc_BoundingVolume(rider->GetRenderPart()->GetBaseBoundingVolume(), gab, avg);
#else
        Calc_BoundingVolume(rider->GetRenderPart()->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);

        //rider->SetPosition(&avg);
        WzVector rot_axis = {0, 0, 1};
        rider->SetRotation(&rot_axis, rot_radin_value);
        rider->GetRenderPart()->SetPosition(-avg); 
        rider->AnimateUnit(NULL);
        rider->SetColor(0xffffffff);
        rider->Render();
        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
    #ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(rider->GetRenderPart()->GetBaseBoundingVolume()));
    #endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();

    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
}

//------------------------------------------------------------------------------ 
void   InterfaceManager::RenderPlayerOnRect(Player* player, RECT area, float rot_radin_value, FPOINT org, float zoom)
{
    if (player == NULL || player->GetBasePart() == NULL)
    {
        return;
    }

#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
#else
    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

    WzColor old_light_color = g_pMap->GetWorldBase()->GetSunLightColor();
    WzColor old_light_color2 = g_pMap->GetWorldBase()->GetAmbientColor();
    g_pMap->GetWorldBase()->SetSunLightColor(WzColor_RGB(255,255,255));
    g_pMap->GetWorldBase()->SetAmbientColor(g_pSunRenderer, WzColor_RGB(255,255,255));

#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
    // 플레이어의 위치와 다이렉션을 구한다.
    WzVector old_player_pos = player->GetVisiblePos();
    WzVector old_player_direction = player->GetDirection();
    
    WzVector light_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    player->SetDirection(light_direction + old_player_pos, old_player_pos, TRUE);
    InterfaceManager::BeginPlayerRender2DMode(player, &(CalcuateViewPortWithRect(area)));
#else
    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)));
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
    {
        WzVector gab, avg;
#if WZENGINEVER >= 410
        Calc_BoundingVolume(player->GetBasePart()->GetBaseBoundingVolume(), gab, avg);
#else
        Calc_BoundingVolume(player->GetBasePart()->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);


        ////미스틱에 한해서 예외처리
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
        if(player->GetObjectKey() == EQUIPMENT_CHARACTER_KEY &&
            player->GetClass() == eCHAR_MYSTIC)
            avg.z -= 0.25f;
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

        player->GetBasePart()->SetPosition(-avg); 
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
        WzVector rot = player->GetBasePart()->GetRotation();
        rot_radin_value -= PI;
        rot_radin_value += rot.z;
        if (rot_radin_value > (PI * 2.0f))
        {
            rot_radin_value -= (PI * 2.0f);
        }
        if (rot_radin_value < 0.0f)
        {
            rot_radin_value += (PI * 2.0f);
        }
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
        player->GetBasePart()->SetRotation(0, 0, rot_radin_value);

        player->Render();
        g_pSunRenderer->FlushReservedDrawCall();

#if WZENGINEVER >= 410
    #ifdef _DEBUG
        g_pSunRenderer->SetColor(0xffff0000);
        g_pSunRenderer->RenderBoundingVolume(&(player->GetBasePart()->GetBaseBoundingVolume()));
    #endif
#endif //#if WZENGINEVER >= 410
    }
    InterfaceManager::End2DMode();
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
#else
    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

    g_pMap->GetWorldBase()->SetSunLightColor(old_light_color);
    g_pMap->GetWorldBase()->SetAmbientColor(g_pSunRenderer,old_light_color2);
}
//------------------------------------------------------------------------------ 
#ifdef _NA_007086_20140318_MONSTERBOOK
void InterfaceManager::RenderMonsterOnRect(Monster* monster, RECT area, float rot_radin_value, FPOINT org, float zoom)
{
    if (monster == NULL || monster->GetBasePart() == NULL)
    {
        return;
    }

#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
#else
    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

    WzColor old_light_color = g_pMap->GetWorldBase()->GetSunLightColor();
    WzColor old_light_color2 = g_pMap->GetWorldBase()->GetAmbientColor();

    // 밑에 2개는 영향이 티가 안나는데 프레임 드랍은 많이 먹어서 빼봄.
    //old_light_color2 = g_pMap->GetWorldBase()->GetMapAmbientColor();
    //old_light_color4 = g_pMap->GetWorldBase()->GetShadowColor();

    g_pMap->GetWorldBase()->SetSunLightColor(WzColor_RGB(255,255,255));
    g_pMap->GetWorldBase()->SetAmbientColor(g_pSunRenderer, WzColor_RGB(255,255,255));
    //g_pMap->GetWorldBase()->SetMapAmbientColor(WzColor_RGB(255,255,255));
    //g_pMap->GetWorldBase()->SetShodowColor(g_pSunRenderer, WzColor_RGB(255,255,255));

    // 플레이어의 위치와 다이렉션을 구한다.
    WzVector old_monster_pos = monster->GetVisiblePos();
    WzVector old_monster_direction = monster->GetDirection();

    WzVector light_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    monster->SetDirection(light_direction + old_monster_pos, old_monster_pos, TRUE);
    InterfaceManager::BeginPlayerRender2DMode(monster, &(CalcuateViewPortWithRect(area)));
#else
    InterfaceManager::Begin2DMode(&(CalcuateViewPortWithRect(area)));
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
    {
        WzVector gab, avg;
        CWzArray<DWORD> old_render_type;
#if WZENGINEVER >= 410
        WzBoundingVolume bv;
        bv = monster->GetBasePart()->GetBaseBoundingVolume();

        WzVector scale = {1, 1, 1};
        WzVector temp_scale = {1, 1, 1};
        CWzUnit* unit = monster->GetBasePart()->GetWzUnit();
        if (unit && unit->GetSubType(0))
        {
            unit->GetSubType(0)->GetScale(&scale);
            temp_scale = monster->GetScale();

            scale.x *= temp_scale.x;
            scale.y *= temp_scale.y;
            scale.z *= temp_scale.z;

            int num = unit->GetSubType(0)->GetNumRenderInfo();
            // 원본 보존
            for (int i = 0; i < num; ++i)
            {
                DWORD& render_type = unit->GetSubType(0)->GetRenderInfo(i)->m_dwRenderType;
                old_render_type.Add(render_type);
                //EWRT_NORMAL
                if (render_type & EWRT_REFRACTION)
                {
                    render_type &= ~EWRT_REFRACTION;
                }
                if (render_type & EWRT_GLOW)
                {
                    render_type &= ~EWRT_GLOW;
                }
            }
        }
        bv.m_aabb.m_wvMax.x *= scale.x;
        bv.m_aabb.m_wvMax.y *= scale.y;
        bv.m_aabb.m_wvMax.z *= scale.z;
        bv.m_aabb.m_wvMin.x *= scale.x;
        bv.m_aabb.m_wvMin.y *= scale.y;
        bv.m_aabb.m_wvMin.z *= scale.z;

        Calc_BoundingVolume(bv, gab, avg);

#else
        Calc_BoundingVolume(monster->GetBasePart()->GetBoundingVolume(), gab, avg);
#endif //#if WZENGINEVER >= 410

        // view Projection setting
        FRECT frect;
        Calc_OrthogonalRect(area, VectorLength(&gab), org, zoom, frect);
        g_pSunRenderer->SetOrthogonalMatrix(frect.left, frect.right, frect.top, frect.bottom, 1, 220);

        avg.x = 0.0f;
        avg.y = 0.0f;

        monster->GetBasePart()->SetPosition(old_monster_pos - avg); 
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
        WzVector rot = monster->GetBasePart()->GetRotation();
        rot_radin_value -= PI;
        rot_radin_value += rot.z;
        if (rot_radin_value > (PI * 2.0f))
        {
            rot_radin_value -= (PI * 2.0f);
        }
        if (rot_radin_value < 0.0f)
        {
            rot_radin_value += (PI * 2.0f);
        }
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
        monster->GetBasePart()->SetRotation(0, 0, rot_radin_value);

        monster->Render();
        g_pSunRenderer->FlushReservedDrawCall();

        // 원본 복구
        for (int i = 0; i < old_render_type.GetCount(); ++i)
        {
            unit->GetSubType(0)->GetRenderInfo(i)->m_dwRenderType = old_render_type.Get(i);
        }
        old_render_type.Clear();
    }
    InterfaceManager::End2DMode();
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
#else
    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST

    g_pMap->GetWorldBase()->SetSunLightColor(old_light_color);
    g_pMap->GetWorldBase()->SetAmbientColor(g_pSunRenderer,old_light_color2);
    //g_pMap->GetWorldBase()->SetMapAmbientColor(old_light_color2);
    //g_pMap->GetWorldBase()->SetShodowColor(g_pSunRenderer,old_light_color4);
}
#endif //_NA_007086_20140318_MONSTERBOOK
//------------------------------------------------------------------------------ 
void   InterfaceManager::RenderCloneOnRect(Clone *pClone,RECT viewport,float fVolumeWidth,float fVolumeHeight,WzColor wzColor)
{




	pClone->SetAngle(0.0f,TRUE);
	pClone->SetColor(WzColor_RGBA(255,255,255,255));
	pClone->SetPlayAni(TRUE);
	pClone->GetBasePart()->SetInterpolationFrame(0);
	pClone->Process(0);
	pClone->SetCurrentFrame(0);	
	pClone->ProcessAnimation(10);
	pClone->AnimateUnit();

	WzVector vCameraFrom, vCameraTo, vZ;


	WzVector vBonePos = pClone->GetSpecialBonePos(StrToWzID("CAM1"));

	WzVector vDir;
	
	SetVector(&vZ, 0.0, 0.0, 1.0);

	vCameraTo = vBonePos;

	vDir.x = -1.0f;
	vDir.y = -1.0f;
	vDir.z = -0.3f;

	VectorNormalize(&vDir,&vDir);

	float fDistance = 100.0f;

	vCameraFrom = vCameraTo  + vDir * fDistance;
	
	WzVector vPos;


	ZeroMemory(&vPos,sizeof(vPos));

	static int i=0;


	pClone->SetCurPos(vPos);
	pClone->SetPosition(vPos);
	pClone->GetBasePart()->SetPosition(vPos);



	g_pSunRenderer->ClearRender(CR_COLOR); 

    WzVector old_direction = g_pMap->GetWorldBase()->GetSunLightDir();
    g_pMap->GetWorldBase()->SetSunLightDir( LOCAL_SUNLIGHT_DIRECTION, TRUE );



	BOOL bOld2dMode,bOldRealSize;
	BOOL bZWrite,bZTest;
	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;
	WzColor             oldcolor;
	ENUM_MESH_DRAW      eDrawMode;
	eDrawMode = g_pSunRenderer->GetMeshDrawMode();
	oldcolor = g_pSunRenderer->GetDiffuseColor();
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	eZFunc = g_pSunRenderer->GetZFunc();
	eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
	g_pSunRenderer->Set2DMode( FALSE);
	


	WzMatrix mat;
	MatrixIdentity(&mat);

	float fZ = 1.0f;

	g_pSunRenderer->SetWorldMatrix(&mat);
	g_pSunRenderer->SetCameraMatrix(&vCameraFrom, &vCameraTo, &vZ);
	g_pSunRenderer->SetViewport(&viewport);

	BOOL bFog = g_pSunRenderer->IsFogEnabled();
	g_pSunRenderer->EnableFog(FALSE);
	

	int iWidth = viewport.right - viewport.left;
	int iHeight = viewport.bottom - viewport.top; 		

	g_pSunRenderer->SetOrthogonalMatrix(-(fVolumeWidth * 0.5f), (fVolumeWidth * 0.5f), -(fVolumeHeight * 0.5f), (fVolumeHeight * 0.5f), 1, 220);
	g_pSunRenderer->SetZBufferWrite(TRUE);
	g_pSunRenderer->SetZBufferTest(TRUE);


	St_RenderCommand Command;

	InitRenderCommand(&Command);

	Command.m_iApplySunLight = 0;

	pClone->SetAlphaBlend(AB_NONE);

	pClone->Render(&Command);

	g_pSunRenderer->ClearRender(CR_DEFAULT,&wzColor);  

	g_pSunRenderer->FlushReservedDrawCall();
	
	g_Camera.RestoreProjection();

	g_pSunRenderer->Set2DMode( bOld2dMode ,bOldRealSize);			
	g_pSunRenderer->EnableFog(bFog);
	g_pSunRenderer->SetZBufferWrite(bZWrite);
	g_pSunRenderer->SetZBufferTest(bZTest);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetMeshDrawMode( eDrawMode );
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->EnableAlphaTest(FALSE);


    g_pMap->GetWorldBase()->SetSunLightDir( old_direction, TRUE );

}




RECT InterfaceManager::CalcuateViewPortWithRect(RECT rc)
{
	RECT *rcWindow = GetWindowRC();



	long sh = 1024;
	long sw = 768;

	if (rcWindow)
	{
		sw = (rcWindow->right - rcWindow->left);
		sh = (rcWindow->bottom - rcWindow->top);
	}

	RECT ret;

	float fXSize;
	float fYSize;

	POINT pt = g_pApplication->Get2DModeMapping();

	fXSize = pt.x;
	fYSize = pt.y;
	
	ret.left = (rc.left * sw) / fXSize;
	ret.right = (rc.right * sw) / fXSize;
	ret.top = (rc.top * sh) / fYSize;
	ret.bottom = (rc.bottom * sh) / fYSize;

	return ret;
}

//
//int InterfaceManager::GetTextureQuality( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	
//	return pDlg->GetOptionParam()->iTextureQuality;
//}
//
//int InterfaceManager::GetLOD( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->LOD;
//}
//
//int InterfaceManager::GetNormalTextureQuality( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->iNormalMapQuality;
//}
//
//int InterfaceManager::GetShadowQuality( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->iShadowQuality;
//}
//
//
//BOOL InterfaceManager::GetBlurState( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->bBlurState;
//}
//
//BOOL InterfaceManager::GetFogState( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->bFogState;
//}
//
//BOOL InterfaceManager::GetMapDistantView( void )
//{
//	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
//	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
//	return pDlg->GetOptionParam()->bMapDistantView;
//}


void InterfaceManager::ApplyOption( BOOL bCheckApply ,BOOL bProgramInit)
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	//	시스템 옵션
	OptionDialog * pDlg = GET_CAST_DIALOG( OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG );
	//OptionDialog *pDlg = static_cast<OptionDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SYSTEM));
	if( pDlg )
	{
		pDlg->Apply( bCheckApply ,bProgramInit);
	}
	else
	{
		//	다이얼로그가 로드되기 전인가? 왜 NULL 인가?
	}

	//	게임 옵션
	OptionGameDialog * pGameDlg = GET_CAST_DIALOG( OptionGameDialog, IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG );
	//OptionGameDialog *pGameDlg = static_cast<OptionGameDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_GAME));
	if( pGameDlg )
	{
		pGameDlg->Apply( bCheckApply ,bProgramInit);
	}
	else
	{
		//	다이얼로그가 로드되기 전인가? 왜 NULL 인가?
	}

	//	사운드 옵션
	OptionSoundDialog * pSoundDlg = GET_CAST_DIALOG( OptionSoundDialog, IM_OPTION_MENU_MANAGER::OPTION_SOUND_DIALOG );
	//OptionSoundDialog *pSoundDlg = static_cast<OptionSoundDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_OPTION_SOUND));
	if( pSoundDlg )
	{
		pSoundDlg->Apply( bCheckApply ,bProgramInit);
	}
	else
	{
		//	다이얼로그가 로드되기 전인가? 왜 NULL 인가?
	}
#endif
}


void  InterfaceManager::RenderActivateMark(int x,int y,int w, int h)
{
    // UI리뉴얼을 통해서 아이콘의 크기가 작아지면서 
    // 오프셋을 조정할 필요가 생김 [10/20/2010 iforall]
    x -= 3;
    y -= 3;

	const DWORD TICK_PER_FRAME = 40;
	const int TOTAL_ANIMATION_FRAME = 34;

	const int ANIM_IN_ROW = 7;
	int iAnimIndex = (m_dwInterfaceTick / TICK_PER_FRAME) % TOTAL_ANIMATION_FRAME;

	int iRow = iAnimIndex / ANIM_IN_ROW;

	int iColumn = iAnimIndex % ANIM_IN_ROW;

	g_pSunRenderer->RenderTexture(m_hTextureActivate,x,static_cast<float>(y) + 0.5f,64+w,64+h,iColumn * 65,iRow * 65,64,64);
    g_pSunRenderer->RenderTexture(m_hTextureActivate,x,static_cast<float>(y - 1) + 0.5f,64+w,64+h,iColumn * 65,iRow * 65,64,64,NULL,0,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);

}

//------------------------------------------------------------------------------ 
void InterfaceManager::RenderActiveAutobot(int x, int y)
{
    if (m_hTextureActivateAutobot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    static const DWORD kTickPerFrame = 30;
    static const int kTotalAniFrame = 45;
    static const int kColumCount = 6;
#ifdef _NA_000000_20130114_RENEWER_UI
    static const int KRowCount = 7;
    static const float kImageWidth = 73.0f;
    static const float kImageHeight = 39.0f;
#else
	static const int KRowCount = 8;
    static const float kImageWidth = 85.0f;
    static const float kImageHeight = 41.0f;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    int cur_frame = (m_dwInterfaceTick / kTickPerFrame) % kTotalAniFrame;

    int col_idx = cur_frame % kColumCount;
    int row_idx = cur_frame / kColumCount;

    if (row_idx > KRowCount)
    {
        return;
    }

    g_pSunRenderer->RenderTexture(m_hTextureActivateAutobot, 
                                x, y,                                           // dest x,y
                                kImageWidth, kImageHeight,                      // dest w,h
                                kImageWidth * col_idx, kImageHeight * row_idx,  // src x,y
                                kImageWidth, kImageHeight);                     // src w,h

    g_pSunRenderer->RenderTexture(m_hTextureActivateAutobot, 
                                x, y,                                           // dest x,y
                                kImageWidth, kImageHeight,                      // dest w,h
                                kImageWidth * col_idx, kImageHeight * row_idx,  // src x,y
                                kImageWidth, kImageHeight,                      // src w,h
                                NULL,0,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);                     
}

void InterfaceManager::RenderActivateMarkIcon(int dx,int dy, int dw, int dh, int animation_index)
{
    const int kAnimationInRow = 7;
    int index_row = animation_index / kAnimationInRow;
    int index_column = animation_index % kAnimationInRow;

    g_pSunRenderer->RenderTexture(m_hTextureActivate,
        dx, 
        static_cast<float>(dy) + 0.5f,
        dw,
        dh,
        index_column * 65,  //! 소스 이미지
        index_row * 65,
        64,
        64);
    g_pSunRenderer->RenderTexture(m_hTextureActivate,
        dx,
        static_cast<float>(dy - 1) + 0.5f,
        dw,
        dh,
        index_column * 65, //! 소스 이미지
        index_row * 65,
        64,
        64,
        NULL,
        0,
        RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);

}

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
void InterfaceManager::RenderComboIconEffect(int x,int y,int w, int h)
{
    const DWORD TICK_PER_FRAME = 30;
    const int TOTAL_ANIMATION_FRAME = 34;

    const int ANIM_IN_ROW = 7;
    int iAnimIndex = (m_dwInterfaceTick / TICK_PER_FRAME) % TOTAL_ANIMATION_FRAME;

    int iRow = iAnimIndex / ANIM_IN_ROW;

    static float icon_size = 38.f;

    int iColumn = iAnimIndex % ANIM_IN_ROW;

    static int t = 13;
    g_pSunRenderer->RenderTexture(m_hTextureLinkSkill,x-t,static_cast<float>(y-t) + 0.5f,w+t+t,h+t+t,iColumn * 65,iRow * 65,64,64);
    g_pSunRenderer->RenderTexture(m_hTextureLinkSkill,x-t,static_cast<float>(y-t - 1) + 0.5f,w+t+t,h+t+t,iColumn * 65,iRow * 65,64,64,NULL,0,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);


    // 0.25 ~ 0.75 사이를 0.6초에 반복
    static float t1 = 600.f;
    static float t2 = 0.5f;
    static float t3 = 0.0f;
    float time = (float)m_dwInterfaceTick * WZ_PI * 2.f / t1;
    float rate = cos( time ) * 0.5f * t2 + 0.25f + t3 ;

    g_pSunRenderer->ColorFill(
        WzColor_RGBA(255,140,140,(BYTE)(rate*255)),
        (float)x, (float)y, (float)w,(float)h);
}
#endif

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
WzVector InterfaceManager::GetDiceAngle(int party_index, int dice_value, DWORD animation_tick)
{
    WzVector dice_angle = {0, 0, 0};

    //  주사위 회전 각도 x,z값 (위에서부터 1~6)
    static float DiceAnimationPosition[6][2] = {
        {D3DX_PI*1/2, 0             }, //1
        {D3DX_PI*3/2, 0             }, //2
        {D3DX_PI    , 0             }, //3
        {D3DX_PI*1/2, D3DX_PI*1/2   }, //4
        {0          , 0             }, //5        
        {D3DX_PI*1/2, D3DX_PI*3/2   }  //6
    };

    if (animation_tick < DICE_ANIMATION_TIME)
    {
        if (dice_value > 0 && dice_value <= 6)
        {
            double progress = static_cast<double>(animation_tick)/(DICE_ANIMATION_TIME);

            // 진행율은 0에서 1사이의 값
            // 진행율을 서서히 커지게  y = (√x) 에 대입
            double weight = sqrt(progress);

            float angle_x = LerfFloat(dice_angle_x_[party_index],
                DiceAnimationPosition[dice_value-1][0],
                weight);
            float angle_z = LerfFloat(dice_angle_z_[party_index],
                DiceAnimationPosition[dice_value-1][1],
                weight);

            dice_angle.x = angle_x;
            dice_angle.y = 0.0f;
            dice_angle.z = angle_z;
        }
    }
    else
    {
        if (dice_value > 0 && dice_value <= 6)
        {
            dice_angle.x = DiceAnimationPosition[dice_value-1][0];
            dice_angle.y = 0.0f;
            dice_angle.z = DiceAnimationPosition[dice_value-1][1];
        }
    }

    return dice_angle;
}

WzColor InterfaceManager::GetDiceColor(DWORD player_key, DWORD high_number_player, DWORD animation_tick)
{
    WzColor dice_color;
    
    if (animation_tick < DICE_ANIMATION_TIME)
    {
        dice_color = WzColor_RGBA(255,255,255,255);
    }
    else
    {
        if (player_key == high_number_player)
        {
            dice_color = WzColor_RGBA(255,247,66,255);
        }
        else
        {
            dice_color = WzColor_RGBA(110,110,110,255);
        }
    }

    return dice_color;
}

void InterfaceManager::SetDiceAngle()
{
    for (int i = 0; i < MAX_PARTYMEMBER_NUM; ++i)
    {
        // 랜덤으로 굴러가도록
        dice_angle_x_[i] = random(4,7)*(D3DX_PI/2);
        dice_angle_z_[i] = random(4,7)*(D3DX_PI/2);
    
    }
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

void InterfaceManager::RenderCheckItem(RECT& rect)
{
    if (get_warning_check_item() == INVALID_HANDLE_VALUE)
    {
        return;
    }
    int cur_frame = (m_dwInterfaceTick / 80) % 9;
    int row_idx = cur_frame % 3;
    int column_idx = cur_frame / 3;
    static const float image_size = 38.f;
    g_pSunRenderer->RenderTexture(get_warning_check_item(), 
        rect.left, rect.top,// dest x,y
        image_size, image_size, // dest w,h
        image_size*row_idx, image_size*column_idx, // src x,y
        image_size, image_size);// src w,h
}


void  InterfaceManager::RenderChatRect(RECT rc,WzColor color, float fDepth /*= 0.f*/)
{
	int EDGE_WIDTH = 8;
	int EDGE_HEIGHT = 6;

	int TRIANGLE_LEFT = 24;
	int TRIANGLE_TOP = 0;
	
	int TRIANGLE_WIDTH = 8;
	int TRIANGLE_HEIGHT = 9;
	
	RenderRectWithZTest(rc, color, fDepth);

	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

	ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	BOOL bTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->SetZBufferTest( TRUE );
	g_pSunRenderer->SetZBufferWrite(TRUE);

	// 모서리~
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.top - EDGE_HEIGHT,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,fDepth,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.top - EDGE_HEIGHT,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,fDepth,RTO_FLIP_HORIZONTAL);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.bottom,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,fDepth,RTO_FLIP_VERTICAL);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.bottom,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,fDepth,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);

	// 위아래

	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left,rc.top - EDGE_HEIGHT,rc.right - rc.left,EDGE_HEIGHT,EDGE_WIDTH,0,1,EDGE_HEIGHT,NULL,fDepth,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left,rc.bottom,rc.right - rc.left,EDGE_HEIGHT,EDGE_WIDTH,0,1,EDGE_HEIGHT,NULL,fDepth,RTO_FLIP_VERTICAL);

	// 좌우

	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.top,EDGE_WIDTH,rc.bottom - rc.top,0,EDGE_HEIGHT,EDGE_WIDTH,1,NULL,fDepth,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.top,EDGE_WIDTH,rc.bottom - rc.top,0,EDGE_HEIGHT,EDGE_WIDTH,1,NULL,fDepth,RTO_FLIP_HORIZONTAL);

	int iTriangleX = ((rc.left + rc.right) / 2) - (TRIANGLE_WIDTH / 2);
	int iTriangleY = rc.bottom + 10;


	g_pSunRenderer->RenderTexture(m_hTextureChat,iTriangleX,iTriangleY,TRIANGLE_WIDTH,TRIANGLE_HEIGHT,TRIANGLE_LEFT,TRIANGLE_TOP,TRIANGLE_WIDTH,TRIANGLE_HEIGHT,NULL,fDepth,0);	

	g_pSunRenderer->SetZBufferTest( bTest );
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->Set2DMode( FALSE );

}

void InterfaceManager::RenderShadowRect(RECT rc,WzColor color)
{
	int EDGE_WIDTH = 8;
	int EDGE_HEIGHT = 6;

	int TRIANGLE_LEFT = 24;
	int TRIANGLE_TOP = 0;

	int TRIANGLE_WIDTH = 8;
	int TRIANGLE_HEIGHT = 9;

	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,255));

	// 모서리~
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.top - EDGE_HEIGHT,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,0,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.top - EDGE_HEIGHT,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,0,RTO_FLIP_HORIZONTAL);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.bottom,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,0,RTO_FLIP_VERTICAL);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.bottom,EDGE_WIDTH,EDGE_HEIGHT,0,0,EDGE_WIDTH,EDGE_HEIGHT,NULL,0,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);

	// 위아래

	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left,rc.top - EDGE_HEIGHT,rc.right - rc.left,EDGE_HEIGHT,EDGE_WIDTH,0,1,EDGE_HEIGHT,NULL,0,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left,rc.bottom,rc.right - rc.left,EDGE_HEIGHT,EDGE_WIDTH,0,1,EDGE_HEIGHT,NULL,0,RTO_FLIP_VERTICAL);

	// 좌우

	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.left - EDGE_WIDTH,rc.top,EDGE_WIDTH,rc.bottom - rc.top,0,EDGE_HEIGHT,EDGE_WIDTH,1,NULL,0,0);
	g_pSunRenderer->RenderTexture(m_hTextureChat,rc.right,rc.top,EDGE_WIDTH,rc.bottom - rc.top,0,EDGE_HEIGHT,EDGE_WIDTH,1,NULL,0,RTO_FLIP_HORIZONTAL);

	int iTriangleX = ((rc.left + rc.right) / 2) - (TRIANGLE_WIDTH / 2);
	int iTriangleY = rc.bottom + 10;

	g_pSunRenderer->RenderTexture(m_hTextureChat,iTriangleX,iTriangleY,TRIANGLE_WIDTH,TRIANGLE_HEIGHT,TRIANGLE_LEFT,TRIANGLE_TOP,TRIANGLE_WIDTH,TRIANGLE_HEIGHT,NULL,0,0);	
}

void InterfaceManager::SetDiplayDropItem(BOOL bProcess)
{
	Object_Map::iterator Itr = g_ObjectManager.GetBegin();
	Object_Map::iterator end   = g_ObjectManager.GetEnd();

	for( ; Itr != end; ++Itr )
	{
		Object * pObject = Itr->second;

		if( pObject->IsKindOfObject( ITEM_OBJECT ) )
		{
			Item * pItem = (Item *)pObject;
			pItem->SetDisplayTime( bProcess );
		}
	}
}

void InterfaceManager::SetShowItemName( BOOL bShow )
{
	m_bShowItemName = bShow;
}

BOOL InterfaceManager::ShowItemName()
{
	return m_bShowItemName;
}

//void InterfaceManager::UpdateAllWindowPosition()
//{
//    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
//    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
//    SolarDialog* dialog_ptr = NULL;
//
//    while (dialog_current_iterator != dialog_end_iterator)
//    {
//        dialog_ptr = (*dialog_current_iterator);
//        if (dialog_ptr != NULL)
//        {
//            dialog_ptr->UpdatePosition();
//        }
//        ++dialog_current_iterator;
//    }
//}

//------------------------------------------------------------------------------ 
void InterfaceManager::LockDlgsAll()
{
    m_bLockDlg = TRUE; 

    DIALOG_HASHTABLE_ITERATOR begin = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR end = m_pDialogHashTable->end();

    for (; begin != end; ++begin)
    {
        (*begin)->Lock();
    }
}
//------------------------------------------------------------------------------ 
void InterfaceManager::UnLockDlg(WzID dialog_id, bool is_monopoly_msg)
{
    SolarDialog* dlg = GetDialog(dialog_id); 
    if(dlg != NULL)
    {
        GetDialogManager()->RequestFocus(dlg->GetDialogWZ());
        if (is_monopoly_msg)
        {
            GetDialogManager()->LockMessageDialogID(dialog_id);
        }
        dlg->Unlock();
    }
}
//------------------------------------------------------------------------------
/** 인터페이스를 락시킨다 엔진 LockMessageDialogID 호출하며 wzid값을넣어줘야한다
*/
void InterfaceManager::LockDlgs(DWORD dwLockDlg,BOOL bWzStyle)
{
	UnLockDlgs();

    m_bLockDlg = TRUE; 
    SolarDialog* lock_dialog_ptr = GetDialog(dwLockDlg);
    if(lock_dialog_ptr != NULL)
    {
        GetDialogManager()->RequestFocus(lock_dialog_ptr->GetDialogWZ());
    }
    m_pDialogManager->LockMessageDialogID(dwLockDlg);

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    // 다른 다이얼로그 LOCK(동작을 막음)
    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr != NULL)
        {
            dialog_ptr->Lock();
        }
        ++dialog_current_iterator;
    }
    //자기 자신은 동작하도록 UNLOCK
    if(lock_dialog_ptr)
    {
    	lock_dialog_ptr->Unlock();
    }	
}
//------------------------------------------------------------------------------
/**
*/
void
InterfaceManager::UnLockDlgs()
{
    m_bLockDlg = FALSE;
    m_pDialogManager->LockMessageDialogID(NULL);

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr != NULL)
        {
            dialog_ptr->Unlock();
        }
        ++dialog_current_iterator;
    }
}

//------------------------------------------------------------------------------ 
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
void InterfaceManager::RenderToDiceTexture()
{
    if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ) ||
        (SCENE_TYPE_CHARSELECT == GameFramework::GetCurScene()) ||
        (SCENE_TYPE_NONE == GameFramework::GetCurScene()) ||
        (GetShowFlag() == FALSE))
    {
        return;
    }

    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager == NULL)
    {
        return;
    }
    if (GetDiceCastManager()->IsDiceDraw() == false)
    {
        return;
    }
    if (ui_party_manager->IsParty() == false)
    {
        GetDiceCastManager()->SetDiceDrawOff();
        return;
    }
    DWORD draw_tick = GetDiceCastManager()->GetAnimationTick();
    DWORD high_number_player = GetDiceCastManager()->high_number_player_key();

    //파티 맴버 얻어와서 출력
    for (int index = 0; index < ui_party_manager->GetPartyNum(); ++index)
    {
        uicPartyMemberInfoPlus member_info = ui_party_manager->GetParty(index);

        DWORD player_key = member_info.Get().m_dwMemberKey;
        Player* player = g_ObjectManager.GetPlayer(player_key);
        if (player != NULL)
        {
            HANDLE tex_handle = GetPartyDiceTexture(index);
            BYTE dice_number = GetDiceCastManager()->GetDiceNumber(player_key);
            WzVector dice_angle = GetDiceAngle(index, dice_number, draw_tick);
            WzColor dice_color = GetDiceColor(player_key, high_number_player, draw_tick);
            g_pSunRenderer->SetColor(dice_color);

            WzVector pos = {0, 0, 0};
            FPOINT org;
            float zoom = 0.7f;
            WzColor clear_color = 0x00000000;
            RenderUnitToTexture(dice_unit_draw(), tex_handle, pos, dice_angle, org, zoom, CR_DEFAULT, clear_color);
        }
    }
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

//------------------------------------------------------------------------------ 
void InterfaceManager::UpdateRenderTarget()
{

	const float fViewVolumeSizeW = 0.5f;
	const float fViewVolumeSizeH = 0.5f;

	BOOL bRenderTargetChanged = FALSE;

	static BOOL bTextTest = FALSE;

	float fXMultiplier;
	float fYMultiplier;

	POINT pt = g_pApplication->Get2DModeMapping();

	fXMultiplier = pt.x / 256.0f;
	fYMultiplier = pt.y / 256.0f;

	fXMultiplier = 1.0f;
	fYMultiplier = 1.0f;

	
	RECT rcMyViewport = {12,12 ,86 ,86 };

	rcMyViewport.left = 0;
	rcMyViewport.top = 0;
	rcMyViewport.right= 100;
	rcMyViewport.bottom = 100;

	RECT rcTargetViewport = {8 ,2 ,53 ,47 };
	RECT rcSummonViewport = {0,0,64 ,64 };
	RECT rcPartyViewport = {9 ,1 ,40 ,32 };

	if (m_bUpdateMyHeadTexture)
	{
		if (m_hTextureMyHead != INVALID_HANDLE_VALUE)
		{
			
			if (g_pHero)
			{

				if (g_pHero->IsCompletelyLoaded()) 
				{

					// 1회용 클론 생성
					Clone *pClone = g_CloneManager.CreateNotManagedClone(g_pHero);

					if (pClone)
					{
                        STLX_LIST<stTemp_RenderInfo> old_render_info_list;

						g_pSunRenderer->x_pManagerTexture->SetRenderTarget(m_hTextureMyHead);			

						
						pClone->SetAnimation(g_pHero->GetCurrentCamAnimation());
						pClone->SetScale(1.0f);

						WzColor wzColor=WzColor_RGBA(0,0,0,255);

						const WzColor FreeChaoColor         = WzColor_RGBA( 255, 128,0, 255 );
						const WzColor ChaoColor             = WzColor_RGBA( 255, 0, 0, 255 );
						const WzColor BoraDoriColor         = WzColor_RGBA( 128, 0, 255, 255 );

						if(g_pHero)
						{

							if(g_pHero->GetChaoState()==eCHAO_STATE_PREV_CHAO)
							{
								wzColor=FreeChaoColor;
							}

							if(g_pHero->IsAttackBeginPlayer())
							{
								wzColor=BoraDoriColor;
							}

							if(g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
							{
								wzColor=ChaoColor;
							}
						}
                        
                        for( size_t i = 0 ; i < pClone->GetPartCount() ; ++i )
                        {
                            if ( CWzUnitDraw* pPart = pClone->GetPart(i) )
                            {
                                if (pPart)
                                {
                                    CWzUnit* unit = pPart->GetWzUnit();
                                    if (unit )
                                    {
                                        int render_num = unit->GetSubType(0)->GetNumRenderInfo();
                                        for ( int j = 0; j < render_num; ++j )
                                        {
                                            WzRenderInfo* render_info_ptr = unit->GetSubType(0)->GetRenderInfo(j);

                                            if (!render_info_ptr)
                                                continue;

                                            DWORD& render_type = render_info_ptr->m_dwRenderType;
                                            if (render_type & EWRT_INV_SHADING)
                                            {
                                                stTemp_RenderInfo temp_render_info;

                                                temp_render_info.render_color_[WRIC_DIFFUSE] = render_info_ptr->m_wcRender[WRIC_DIFFUSE];
                                                temp_render_info.render_color_[WRIC_SPECULAR] = render_info_ptr->m_wcRender[WRIC_SPECULAR];
                                                temp_render_info.render_color_[WRIC_AMBIENT] = render_info_ptr->m_wcRender[WRIC_AMBIENT];
                                                temp_render_info.proto_render_info_ptr = render_info_ptr;

                                                render_type &= ~EWRT_INV_SHADING;
                                                render_info_ptr->m_wcRender[WRIC_DIFFUSE] = WzColor_RGB( 255, 255, 255 );
                                                render_info_ptr->m_wcRender[WRIC_SPECULAR] = WzColor_RGB( 255, 255, 255 );
                                                render_info_ptr->m_wcRender[WRIC_AMBIENT] = WzColor_RGB( 128, 128, 128 );

                                                old_render_info_list.push_back(temp_render_info);
                                            }
                                        }
                                    }
                                }
                            }
                        }
						
						RenderCloneOnRect(pClone,rcMyViewport,fViewVolumeSizeH,fViewVolumeSizeW,wzColor);

                        std::list<stTemp_RenderInfo>::iterator iter = old_render_info_list.begin();
                        for ( ; iter != old_render_info_list.end(); ++iter)
                        {
                            WzRenderInfo* temp_render_info_ptr = (*iter).proto_render_info_ptr;
                            if (temp_render_info_ptr)
                            {
                                temp_render_info_ptr->m_dwRenderType |= EWRT_INV_SHADING;
                                temp_render_info_ptr->m_wcRender[WRIC_DIFFUSE] = (*iter).render_color_[WRIC_DIFFUSE];
                                temp_render_info_ptr->m_wcRender[WRIC_SPECULAR] = (*iter).render_color_[WRIC_SPECULAR];
                                temp_render_info_ptr->m_wcRender[WRIC_AMBIENT] = (*iter).render_color_[WRIC_AMBIENT];
                            }
                        }
                        old_render_info_list.clear();

						// 이미 그린놈은 볼일없다.
						pClone->Destroy();
						delete pClone;

						bRenderTargetChanged = TRUE;
						g_pSunRenderer->FlushReservedDrawCall(FALSE);

						m_bUpdateMyHeadTexture = FALSE;

					}
				}
			}			
		}		
	}

	
	//

	if (bRenderTargetChanged)
	{
		
		g_pSunRenderer->x_pManagerTexture->SetRenderTarget();
	}

	g_Camera.UpdateCamera();

}

void InterfaceManager::UpdateMyHeadTexture()
{
	m_bUpdateMyHeadTexture = TRUE;

}

void InterfaceManager::UpdateTargetHeadTexture()
{
	m_bUpdateTargetHeadTexture = TRUE;

}

void InterfaceManager::UpdateSummonTargetHeadTexture()
{
	m_bUpdateSummonTargetHeadTexture = TRUE;

}

void InterfaceManager::UpdatePartyHeadTexture(int iIndex)
{

	if (iIndex >= 0 && iIndex < MAX_PARTYMEMBER_NUM)
	{
		m_bUpdatePartyHeadTexture[iIndex] = TRUE;
	}
	

}

HANDLE  InterfaceManager::GetMyHeadTexture()
{
	return m_hTextureMyHead;

}
HANDLE  InterfaceManager::GetTargetHeadTexture()
{

	return m_hTextureTargetHead;

}

HANDLE  InterfaceManager::GetSummonTargetHeadTexture()
{

	return m_hTextureSummonTargetHead;

}

HANDLE  InterfaceManager::GetPartyHeadTexture(int iIndex)
{
	if (iIndex < 0 || iIndex >= MAX_PARTYMEMBER_NUM)
	{
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		return m_hTexturePartyHead[iIndex];
	}

}

//------------------------------------------------------------------------------ 
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
HANDLE  InterfaceManager::GetPartyDiceTexture(int dice_tex_index)
{
    if (dice_tex_index < 0 || dice_tex_index >= MAX_PARTYMEMBER_NUM)
    {
        return INVALID_HANDLE_VALUE;
    }
    return m_hTexturePartyDice[dice_tex_index];
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

//------------------------------------------------------------------------------ 
void   InterfaceManager::ShowGetMoneyMessage(MONEY money)
{
    if (g_pHero == NULL)
    {
        return;
    }

    MONEY gain_money = money - g_pHero->GetMoney();
    if (gain_money == 0)
    {
        return;
    }

    TCHAR temp_string[INTERFACE_STRING_LENGTH];
    TCHAR message[INTERFACE_STRING_LENGTH];

    //	%I64u 하임를 획득하였습니다.
    g_InterfaceManager.GetInterfaceString(eST_CODE_GET_MONEY, temp_string, MAX_MESSAGE_LENGTH);
    _stprintf(message, temp_string, gain_money);

    // 돈주었다는 메시지 뛰운다.
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message, CHAT_TYPE_SYSTEM);
}


BOOL	InterfaceManager::LoadFontAndStringTable()
{
	GlobalFunc::WriteLoadingTimeLog(_T("InterfaceManager::LoadFontAndStringTable Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

    assert( g_pContactDialog_FnPtr );
	m_pDialogManager = (CDialogManager *)g_pContactDialog_FnPtr( WZ_NEW_DLG_MANAGER, 0 );

	m_pDialogManager->EnableImeSystem(true);
	if( !m_pDialogManager->OnCreate( "Data\\Interface\\", g_hWndMain, g_hInstance, g_pSunRenderer ) )
	{
		return FALSE;
	}

	char szCurDirectory[512];
	ZeroMemory(szCurDirectory,sizeof(TCHAR));
	GetCurrentDirectoryA(512, szCurDirectory); 
	sprintf( szCurDirectory, "%s\\%s", szCurDirectory, "Data\\Interface\\");
	g_pSunRenderer->x_pManagerTextOut->SetFontFolderPath(szCurDirectory);

#ifdef _NA_008364_20150623_CHANGE_TEXT_PATH
	m_pDialogManager->SetExePath("");
	CDiscAccess* disc_access = g_pApplication->GetDiscAccess();
	if ( disc_access )
	{
		disc_access->SetActiveDisc(ePFN_NOT_USE_PACKFILE);
	}
#endif // _NA_008364_20150623_CHANGE_TEXT_PATH
	m_pDialogManager->SetDiscAccess( g_pApplication->GetDiscAccess() );

    BOOL bRet =  FALSE;
#ifdef _DH_OS_LANGUAGETYPE
    LanguageType language_type = CGameOption::Instance()->GetLanguageType();
    CGameOption::Instance()->SetLanguageType(kLanguageGlobal);
    bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTable.txt") );
    bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTableForProgram.txt") );
    bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTableForName.txt") );
    bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTable_InstantHelp.txt") );
    CGameOption::Instance()->SetLanguageType(language_type);
#endif//_DH_OS_LANGUAGETYPE
	bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTable.txt") );
	bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTableForProgram.txt") );
	bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTableForName.txt") );
	//bRet = m_pDialogManager->LoadStringTable( "StringTableForVoice.txt" );
	bRet = m_pDialogManager->LoadStringTable( GlobalFunc::AddNationName("StringTable_InstantHelp.txt") );

#ifdef _NA_008364_20150623_CHANGE_TEXT_PATH
	m_pDialogManager->SetExePath("Data\\Interface\\");
	if ( disc_access && GENERALPARAM->IsUsePackFile() )
	{
		disc_access->SetActiveDisc(ePFN_RESOURCE);
	}
#endif // _NA_008364_20150623_CHANGE_TEXT_PATH

    sprintf(szCurDirectory, "Data\\Interface\\Sun_Font.wft");
#if WZENGINEVER >= 290
    BYTE character_set = GetFontCharacterSet();
    bRet = g_pSunRenderer->x_pManagerTextOut->LoadExtendedFontInfo(szCurDirectory, character_set);
#else
    bRet = g_pSunRenderer->x_pManagerTextOut->LoadExtendedFontInfo(szCurDirectory);
#endif
    
	char PACK_FILE_NAME[MAX_PATH] = {0,};

	if( GENERALPARAM->IsUsePackFile() )
	{
		strcpy( PACK_FILE_NAME, "Data\\Resource.wpk" );
		PACKER_MANAGER()->OpenPackFile("Data\\Resource.wpk");
	}
	else
	{
		ZeroMemory( PACK_FILE_NAME, MAX_PATH );
	}

	
	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("InterfaceManager::LoadFontAndStringTable End"));

    return TRUE;

}


BOOL InterfaceManager::GetItemClassName(CODETYPE ItemCode, TCHAR * OUT pszOutClassName, int iLen )
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCode);
	if (!pItemInfo )
	{
		return FALSE;
	}

	DWORD dwString = eST_LIMITCLASS_BER_STRING_INDEX;
	if( pItemInfo->m_wEqClass1 )
	{
		dwString = eST_LIMITCLASS_BER_STRING_INDEX;
	}
	else if ( pItemInfo->m_wEqClass2)
	{
		dwString = eST_LIMITCLASS_DRA_STRING_INDEX;
	}
	else if ( pItemInfo->m_wEqClass3)
	{
		dwString = eST_LIMITCLASS_SHA_STRING_INDEX;
	}
	else if ( pItemInfo->m_wEqClass4)
	{
		dwString = eST_LIMITCLASS_VAL_STRING_INDEX;
	}
	else if ( pItemInfo->m_wEqClass5)
	{
		dwString = eST_LIMITCLASS_ELM_STRING_INDEX;
	}
    else if (pItemInfo->m_wEqClass6) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    {
        dwString = eST_LIMITCLASS_MYS_STRING_INDEX;
    }
    else if (pItemInfo->m_wEqClass7) //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    {
        dwString = eST_LIMITCLASS_HEL_STRING_INDEX;
    }
    else if (pItemInfo->m_wEqClass8) //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    {
        dwString = eST_LIMITCLASS_WIT_STRING_INDEX;
    }

	g_InterfaceManager.GetInterfaceString(dwString, pszOutClassName, iLen);

	return TRUE;
}

BOOL	InterfaceManager::RegisterDialog( DWORD dwKey, SolarDialog * pDlg )
{
	if( m_pNotLoadDlgTable->Add( pDlg, dwKey ) == FALSE )
		return FALSE;
	
	return TRUE;
}

void InterfaceManager::ReleaseDialogTexture()
{
	if (m_pDialogHashTable)
	{
        DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
        DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
        SolarDialog* dialog_ptr = NULL;
        BOOL is_fixed_dialog = TRUE;
        DWORD dialog_id = 0;

        while (dialog_current_iterator != dialog_end_iterator)
        {
            dialog_ptr = (*dialog_current_iterator);
            if (dialog_ptr != NULL)
            {
                dialog_id = dialog_ptr->GetDialogWZ()->GetDlgID();
                is_fixed_dialog = ConstantDialogListParser::Instance()->IsFixedDialog(dialog_id);

                if (is_fixed_dialog != TRUE)
                {
                    dialog_ptr->ReleaseDialogTexture();
                }
            }
            ++dialog_current_iterator;
        }
	}	
}
 
//------------------------------------------------------------------------------
/**
*/
char* InterfaceManager::Trim(char *str)
{
	int i = 0, cur = 0;
	char chLcase_gap = ('a' - 'A');

	while(str[cur] != '\0')
	{		
		if(str[cur] == ' ' || str[cur] == '\t')	// 공백이면 cur만 증가
		{
			++cur;
		}
		else	// 공백이 아니면 복사후 증가
		{
			str[i] = str[cur];
			if(str[i] <= 'Z' && str[i] >= 'A')
			{
				str[i] += chLcase_gap;
			}
			++i;
			++cur;
		}
	}
	str[i] = '\0';	// 끝에 널문자 삽입
	return str;
}

#ifdef _DEV_VER

#ifdef _DH_GM_NPC_WINDOW
DWORD InterfaceManager::LoadNpcNameCode()
{
    TCHAR	szText[INTERFACE_STRING_LENGTH];
    TCHAR	szText2[INTERFACE_STRING_LENGTH];
    NPCInfoParser::Instance()->SetFirst();
    //BASE_NPCINFO *pBASE_NPCINFO = NULL;

    while( true )
    {
        const BASE_NPCINFO *pBASE_NPCINFO = (NPCInfoParser::Instance()->GetNext());
        if (pBASE_NPCINFO == NULL)
        {
            break;
        }
        g_InterfaceManager.GetInterfaceString(pBASE_NPCINFO->m_NCode, szText, INTERFACE_STRING_LENGTH);
        Trim(szText);

        sprintf(szText2, pBASE_NPCINFO->m_pszName);
        Trim(szText2);
        NPCInfoParser::Instance()->PushNpcName(szText, pBASE_NPCINFO->m_MonsterCode, szText2);
    }

    return 0;
}
#endif//_DH_GM_NPC_WINDOW
//------------------------------------------------------------------------------
/**
*/
DWORD InterfaceManager::LoadItemNameCode()
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];
    TCHAR	szText2[INTERFACE_STRING_LENGTH];
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	TCHAR	szTemp[INTERFACE_STRING_LENGTH];
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	ItemInfoParser::Instance()->SetFirst();
	BASE_ITEMINFO *pBASE_ITEMINFO = NULL;

	while( true )
	{
		pBASE_ITEMINFO = ItemInfoParser::Instance()->GetNext();
		if( pBASE_ITEMINFO == NULL )
		{
			break;
		}
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
		if( pBASE_ITEMINFO->m_wTradeSellType == eITEM_TRADESELL_DOALL )
		{
		    g_InterfaceManager.GetInterfaceString( pBASE_ITEMINFO->m_NCode, szText, INTERFACE_STRING_LENGTH );

		    sprintf(szText2, "");
		    if(pBASE_ITEMINFO->IsWeapon() || pBASE_ITEMINFO->IsArmor())
		    {
			    g_InterfaceManager.GetInterfaceString( pBASE_ITEMINFO->m_SCNCode, szText2, INTERFACE_STRING_LENGTH );
		    }
    		
		    Trim(szText);
		    Trim(szText2);
    #ifdef _RUSSIA
            sprintf(szTemp, "%s%s", szText2, szText);
    #else
            sprintf(szTemp, "%s%s", szText, szText2);
    #endif //_RUSSIA		    
            ItemInfoParser::Instance()->PushItemName(szTemp, pBASE_ITEMINFO->m_Code);
        }
#else
    #ifdef _DEV_VER
        g_InterfaceManager.GetItemName(pBASE_ITEMINFO->m_Code, szText, INTERFACE_STRING_LENGTH);
        Trim(szText);

        sprintf(szText2, pBASE_ITEMINFO->m_pszName);
        Trim(szText2);
        ItemInfoParser::Instance()->PushItemName(szText, pBASE_ITEMINFO->m_Code, szText2);
    #endif//defined(_DH_GM_ITEM_WINDOW) && defined(_DEV_VER)
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	}

	return 0;
}

#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH


void InterfaceManager::RestoreCaptionFromStringTable()
{
    if (m_pDialogHashTable)
    {
        DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
        DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
        SolarDialog* dialog_ptr = NULL;
        CControlWZ* control_ptr = NULL;
        int control_count = 0;

        while (dialog_current_iterator != dialog_end_iterator)
        {
            dialog_ptr = (*dialog_current_iterator);
            if (dialog_ptr != NULL)
            {
                control_count = dialog_ptr->GetDialogWZ()->GetControlCount();

                for (int index = 0; index < control_count; ++index)
                {
                    BOOL is_show = dialog_ptr->IsVisible();
                    if (is_show == TRUE)
                    {
                        dialog_ptr->ShowWindow(FALSE);
                    }
                    control_ptr = dialog_ptr->GetDialogWZ()->FindControlWZ(index);
                    if (control_ptr != NULL)
                    {
                        control_ptr->ChangeCaptionString();
                    }
                    if (is_show == TRUE)
                    {
                        dialog_ptr->ShowWindow(is_show);
                    }
                }
            }
            ++dialog_current_iterator;
        }
    }
}

//------------------------------------------------------------------------------
void InterfaceManager::ResetUIManager()
{
    for (int i = 0; i < UIMAN_MAX; ++i)
    {
        if (m_vUserInterfaceMans[i])
        {
            m_vUserInterfaceMans[i]->Reset();
        }        
    }
}
//------------------------------------------------------------------------------
void InterfaceManager::ResetSolarDialog()
{
    if (m_pDialogHashTable == NULL)
    {
        return;
    }

    DIALOG_HASHTABLE_ITERATOR dialog_current_iterator = m_pDialogHashTable->begin();
    DIALOG_HASHTABLE_ITERATOR dialog_end_iterator = m_pDialogHashTable->end();
    SolarDialog* dialog_ptr = NULL;

    while (dialog_current_iterator != dialog_end_iterator)
    {
        dialog_ptr = (*dialog_current_iterator);
        if (dialog_ptr != NULL)
        {
            dialog_ptr->Reset();
        }
        ++dialog_current_iterator;
    }
}
//------------------------------------------------------------------------------

void InterfaceManager::SetUIBlinkGrid(DWORD dialog_id, DWORD control_id, bool is_blink)
{
    SolarDialog* dialog_ptr = NULL;
    CControlWZ* control_ptr = NULL;
    dialog_ptr = GetDialog(dialog_id);

    if (dialog_ptr == NULL)
    {
        return;
    }

    control_ptr = dialog_ptr->GetControlWZ(control_id);
    dialog_ptr->SetBlinkGrid(control_id, is_blink);
}

void InterfaceManager::InitBlinkGrid()
{
    grid_rectangles_[0].left = 0.0f;    //┏─┐
    grid_rectangles_[0].top = 0.0f;     //│  │
    grid_rectangles_[0].right = 11.0f;  //└─┘
    grid_rectangles_[0].bottom = 11.0f;

    grid_rectangles_[1].left = 0.0f;    //┌─┐
    grid_rectangles_[1].top = 11.0f;    //┃  │
    grid_rectangles_[1].right = 11.0f;  //└─┘
    grid_rectangles_[1].bottom = 2.0f;

    grid_rectangles_[2].left = 0.0f;    //┌─┐
    grid_rectangles_[2].top = 53.0f;    //│  │
    grid_rectangles_[2].right = 11.0f;  //┗─┘
    grid_rectangles_[2].bottom = 11.0f;

    grid_rectangles_[3].left = 11.0f;   //┌━┐
    grid_rectangles_[3].top = 0.0f;     //│  │
    grid_rectangles_[3].right = 2.0f;   //└─┘
    grid_rectangles_[3].bottom = 11.0f;

    grid_rectangles_[4].left = 11.0f;   //┌─┐
    grid_rectangles_[4].top = 53.0f;    //│  │
    grid_rectangles_[4].right = 2.0f;   //└━┘
    grid_rectangles_[4].bottom = 11.0f;

    grid_rectangles_[5].left = 53.0f;   //┌─┓
    grid_rectangles_[5].top = 0.0f;     //│  │
    grid_rectangles_[5].right = 11.0f;  //└─┘
    grid_rectangles_[5].bottom = 11.0f;

    grid_rectangles_[6].left = 53.0f;   //┌─┐
    grid_rectangles_[6].top = 11.0f;    //│  ┃
    grid_rectangles_[6].right = 11.0f;  //└─┘
    grid_rectangles_[6].bottom = 2.0f;

    grid_rectangles_[7].left = 53.0f;   //┌─┐
    grid_rectangles_[7].top = 53.0f;    //│  │
    grid_rectangles_[7].right = 11.0f;  //└─┛
    grid_rectangles_[7].bottom = 11.0f;
}

void InterfaceManager::MouseMoveLock( bool Lock )
{
    m_bLockDlg = Lock;
}

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void InterfaceManager::RenderActivateMarkPerkGroup( int x, int y, int width, int height )
{
    const DWORD TICK_PER_FRAME = 40;
    const int TOTAL_ANIMATION_FRAME = 34;

    const int ANIM_IN_ROW = 7;
    int iAnimIndex = (m_dwInterfaceTick / TICK_PER_FRAME) % TOTAL_ANIMATION_FRAME;

    int iRow = iAnimIndex / ANIM_IN_ROW;

    int iColumn = iAnimIndex % ANIM_IN_ROW;

    g_pSunRenderer->RenderTexture(m_hTextureActivate,x,y,width,height,iColumn * 65,iRow * 65,64,64);
    g_pSunRenderer->RenderTexture(m_hTextureActivate,x,y,width,height,iColumn * 65,iRow * 65,64,64,NULL,0,RTO_FLIP_VERTICAL | RTO_FLIP_HORIZONTAL);
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
void InterfaceManager::BeginPlayerRender2DMode(Object* objectr_ptr, RECT* viewport /*= NULL*/,
                                                    DWORD clear_flags /*= CR_DEPTH*/,
                                                    WzColor* clear_color /*= NULL*/,
                                                    float* clear_z /*= NULL*/,
                                                    DWORD* clear_stencil /*= NULL*/)
{
    if (objectr_ptr == NULL)
    {
        return;
    }
    // 인벤토리 플레이어나 다른 플레이어를 그릴 때 셋팅
    backup_drawmode = g_pSunRenderer->GetMeshDrawMode();
    backup_oldcolor = g_pSunRenderer->GetDiffuseColor();
    backup_cullfacemode = g_pSunRenderer->GetCullFaceMode();
    backup_zfunc = g_pSunRenderer->GetZFunc();
    backup_alphablendmode = g_pSunRenderer->GetAlphaBlendMode();
    backup_zwrite = g_pSunRenderer->IsEnableZWrite();
    backup_ztest = g_pSunRenderer->IsEnableZTest();
    backup_fog = g_pSunRenderer->IsFogEnabled();
    g_pSunRenderer->GetAlphaTestMode(&backup_alpharef, &backup_alphafunc);
    g_pSunRenderer->Check2DMode(&backup_old2dmode, &backup_oldrealsize);

    g_pSunRenderer->Set2DMode(FALSE);
    g_pSunRenderer->SetZBufferWrite(TRUE);
    g_pSunRenderer->SetZBufferTest(TRUE);
    g_pSunRenderer->EnableFog(FALSE); 
    g_pSunRenderer->SetViewport(viewport);

    g_pSunRenderer->ClearRender(clear_flags, clear_color, clear_z, clear_stencil, viewport);

    WzMatrix world_matrix;
    MatrixIdentity(&world_matrix);
    g_pSunRenderer->SetWorldMatrix(&world_matrix);

    // 카메라를 플레이어에 맞춰 주어야 한다.
    // 플레이어가 어느위치에 있던간에 플레이어를 정면으로 바라봐야한다.
    WzVector object_pos, object_direction;
    SetVector(&object_pos, 0.0f, 0.0f, 0.0f);
    SetVector(&object_direction, 0.0f, 0.0f, 0.0f);
    WzVector cam_from, cam_to, cam_up;    
    
    object_pos = objectr_ptr->GetVisiblePos();
    object_direction = objectr_ptr->GetDirection();
    
    cam_from = object_pos + object_direction * 10.0f;
    SetVector(&cam_to, object_pos.x, object_pos.y, object_pos.z);
    SetVector(&cam_up, 0.0, 0.0, 1.0);
    g_pSunRenderer->SetCameraMatrix(&cam_from, &cam_to, &cam_up);
}
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
int InterfaceManager::get_gold_rush_mark_index( const WORD& gold_grade ) const
{
    std::map<WORD, int>::const_iterator iter;
    iter = gold_rush_mark_map_.find(gold_grade);
    if (iter == gold_rush_mark_map_.end())
        return -1;

    return iter->second;
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
int InterfaceManager::get_ranking_mark_index( const eSUN_RANKING_TYPE& ranking_type, const eSUN_RANKING_GRADE_TYPE& grade )
{
	if ( ranking_type < 0 || ranking_type >= eSUN_RANKING_GRADE_END )
	{
		return 0;
	}

	std::map<WORD, WORD>::iterator map_iter;
	std::map<WORD, WORD> ranking_grade_map = sun_ranking_grade_mark_vector_[ranking_type];
	map_iter = ranking_grade_map.find(grade);
	if ( map_iter == ranking_grade_map.end() )
	{
		return 0;
	}

	return map_iter->second;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
