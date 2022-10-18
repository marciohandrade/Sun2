#include "SunClientPrecompiledHeader.h"
#include "ClientScriptParserManager.h"
#include "uiMissionRankMan/uiMissionRankMan.h"
#include "MapInfoParser.h"
#include "ItemInfoParser.h"
#include "SkillInfoParser.h"
#include "StateInfoParser.h"
#include "InterfaceManager.h"
#include "NPCInfoParser.h"
#include "ShopInfoParser.h"
#include "ItemCompositeParser.h"
#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "RandomItemParser.h"
#endif //#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "ItemComposeListParser.h"
#include "SocketOptionParser.h"
#include "SunEventInfoParser.h"
#include "StyleQuickRegistInfoParser.h"
#include "ItemTypeList.h"
#include "RewardInfoList.h"
#include "GameOptionParser.h"
#include "RankOptionParser.h"
#include "MissionRewardParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "ItemCrystalizeListParser.h"
#include "ExtraStoneOptionInfoParser.h"
#include "ExpValueInfoParser.h"
#ifdef __NA_001358_ENCHANT_UNIFICATION
#include "InstantEnchantParser.h"
#else
#include "EnchantParser.h"
#endif //__NA_001358_ENCHANT_UNIFICATION
#include "CharInfoParser.h"
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
#include "ExchangeRewardInfo.h"
#endif

#include "AttachmentOffsetInfoParser.h"
#include "BGMSoundInfoParser.h"
#include "CharSoundInfoParser.h"
#include "CompositeNPCInfoParser.h"
#include "DlgPosInfoParser.h"
#include "EnvironmentInfoParser.h"
#include "EventInfoParser.h"
//#include "HeadColorInfoParser.h"
#include "HeadMeshInfoParser.h"
#include "GuideMessageParser.h"
#include "HelpInfoParser.h"

#include "HelpMessageParser.h"
#include "HeroHeightInfoParser.h"
#include "HeroVariationInfoParser.h"
#include "HeroVoiceInfoParser.h"
#include "ItemSoundInfoParser.h"
#include "LimitedMapPortParser.h"
#include "MonsterVoiceInfoParser.h"
#include "NPCVoiceInfoParser.h"
#include "OptionListParser.h"
#include "ProjectileInfoParser.h"
#include "QuestTextInfoParser.h"
#include "RadarInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "SkillTextInfoParser.h"
#include "WeaponSoundInfoParser.h"
#include "SkillInventoryStoreParser.h"
#include "LimitedOptionParser.h"
#include "Message.h"
#include "GraphicResourceList.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "message.h"
#include "messagetext.h"
#include "questinfomanagerex.h"
#include "SetItemOptionInfoParser.h"
#include "GuildRankInfoParser.h"
#include "GuildRightInfoParser.h"
#include "GuildMarkInfoParser.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    #include "GuildInfoParser.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL
#include "FateItemInfoParser.h"
#include "PetInfoParser.h"
#include "EtherWeaponEffectInfo.h"
#include "LotteryInfoParser.h"
#include "NpcImageInfo.h"
#include "HeroCustomInfoParser.h"

#ifdef	__NA_001062_20080623_STRENGTH_PENALTY
#include "StrengthPenaltyParser.h"
#include "StrengthUIParser.h"
#endif//__NA_001062_20080623_STRENGTH_PENALTY

#include "ssq/SSQInfoParser.h"

#include "CollectionParser.h"
#include "ConstantDialogListParser.h"

#include "PKScriptParser.h"
#include "RidingInfoParser.h"
#include "DominationInfoParser.h"
#include "InDunOptionParser.h"

#include "SpecialTexutureInfoParser.h"
#include "AmbientSoundInfoParser.h"
#include "SkillSystem/PerkInfoParser.h"

#include "MissionInfoParser.h"
#include "MissionTextInfoParser.h"

#ifdef _YMS_AUTOBOT_GAME
	#include "AutomaticInfoParser.h"
    #include "AutomaticComboInfoParser.h"
#endif //_YMS_AUTOBOT_GAME

#include "CubeSystem\CubeSystemParser.h"

#include "SocketSystem/SocketItemRatioParser.h"
#include "SocketSystem/ZardComposeInfoParser.h"
#include "AchievementParser.h"

#include "TextureListInfoParser.h"
#include "TextureAnimationInfoParser.h"
#include "TextureAnimationManager.h"

#include "CreateCharCustomCameraInfoParser.h"

#include "LoadingTextureInfoParser.h"
#include "CharacterCreationInfoParser.h"
#include "ColorInfoParser.h"
#include "TreeMenuInfoParser.h"
#include "TotalGuideParser.h"
#ifdef _NA_002935_20110704_ITEM_SCORE
#include "ItemScoreParser.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_20110630_BATTLE_ZONE_RENEWAL
#include "BattlezoneMissionTreeParser.h"
#endif//_NA_20110630_BATTLE_ZONE_RENEWAL
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "FameListParser.h"
#include "ReputeListParser.h"
#include "TitleListParser.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#include "Input/HotKeyInfoParser.h"
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "CurrencyInfoParser.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#include "ResourceVersionParser.h"
#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosZoneInfoParser.h"
#include "ChaosSystem/ChaosZoneTreeParser.h"
#include "BattleGroundInfoParser.h"
#include "BattleGroundFlagInfoParser.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "SkillEventEffectInfoParser.h"
#include "CostumeItemInfoParser.h"

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
#include "RadarFullMissionInfo.h"
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_000000_20130114_RENEWER_UI
#include "EnduranceInfoParser.h"
#endif

#include "GameOptionListParser.h"

#ifdef _NA_007086_20140318_MONSTERBOOK
#include "MonsterBookParser.h"
#include "uiMonsterBookMan/MonsterBookDropOrderParser.h"
#include "uiMonsterBookMan/MonsterBookDropItemParser.h"
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilityInfoParser.h"
#include "GuildFacilityImageParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "AttendanceParser.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatueInfoParser.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#include "SpaZoneInfoParser.h"
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosSystem/GuildMissionTreeParser.h"
#include "ChaosZoneSystem/SonnenScheinRewardInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

ClientScriptParserManager::ClientScriptParserManager(void)
{
}

ClientScriptParserManager::~ClientScriptParserManager(void)
{
}

VOID	ClientScriptParserManager::InitParserPool( VOID* pPoolOption )
{

	//Todo: 파서를 찾아 초기화 
	MapInfoParser::Instance()->Init(MAX_MAP_INFO);
	ItemInfoParser::Instance()->Init( MAX_ITEM_INFO);
	SkillInfoParser::Instance()->Init( MAX_SKILL_INFO);
	StateInfoParser::Instance()->Init(100);
	NPCInfoParser::Instance()->Init( MAX_NPC_INFO, 100 );
	ShopInfoParser::Instance()->Init(MAX_SHOP_INFO);
	ItemCompositeParser::Instance()->Init(MAX_ITEM_COMPOSITE_INFO);
	ItemComposeListParser::Instance()->Init(MAX_ITEM_COMPOSITE_INFO);
	StyleQuickRegistInfoParser::Instance()->Init(MAX_STYLE_QUICK_REGISTER);
	MissionRewardParser::Instance()->Init(MAX_MISSION_REWARD_INFO);
	RequireFieldLevelInfoParser::Instance()->Init(100);
	ItemCrystalizeListParser::Instance()->Init(MAX_ITEM_CRYSTALIZE_INFO);
	ExtraStoneOptionInfoParser::Instance()->Init(100);
	ExpValueInfoParser::Instance()->Init(MAX_EXPVALUE_INFO);

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ExchangeRewardInfoParser::Instance()->Init(100);
#endif

	//2007.07.30 작업 
	EventInfoParser::Instance()->Init(100);
	SkillInventoryStoreParser::Instance()->Init(MAX_SKILL_INVENTORY_STORE_INFO);
	GraphicResourceList::Instance()->Init(MAX_GRAPHIC_INFO);
	WeaponSoundInfoParser::Instance()->Init(100);
    cSkillEventEffectInfoParser::Instance()->Init(1000);
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	cCostumeItemInfoParser::Instance()->Init(100);
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
	CharSoundInfoParser::Instance()->Init(10000);
	BGMSoundInfoParser::Instance()->Init(MAX_BGM);
	DlgPosInfoParser::Instance()->Init(MAX_DIALOG);
	MessageInfoParser::Instance()->Init(MESSAGE_TOTAL_MAX);
	EnvironmentInfoParser::Instance()->Init(100);
	ProjectileInfoParser::Instance()->Init(100);
	HeroVoiceInfoParser::Instance()->Init(10);
	NPCVoiceInfoParser::Instance()->Init(100);
	MonsterVoiceInfoParser::Instance()->Init(100);
	RadarInfoParser::Instance()->Init(100);
	HeroVariationInfoParser::Instance()->Init(100);
	HeroHeightInfoParser::Instance()->Init(100);
	ItemSoundInfoParser::Instance()->Init(MAX_ITEM_SOUND_INFO);
	LimitedMapPortParser::Instance()->Init(MAX_NPC_PORTAL_INFO);
	HeadMeshInfoParser::Instance()->Init(100);
	CompositeNPCInfoParser::Instance()->Init(MAX_ITEM_COMPOSITE_NPC_INFO);
	SkillTextInfoParser::Instance()->Init(MAX_SKILL_TEXT_INFO);
	SkillEffectInfoParser::Instance()->Init(MAX_SKILL_TEXT_INFO);
	OptionListParser::Instance()->Init(500);
    GuideMessageParser::Instance()->Init(MAX_HELP_INFO);
	HelpInfoParser::Instance()->Init(MAX_HELP_INFO);
	AttachmentOffsetInfoParser::Instance()->Init(MAX_ATTACH_ITEM_INFO);


//#ifdef HAIR_COLOR_VARIATION 
//	HeadColorInfoParser::Instance()->Init( 100 );
//#endif

	
	QuestTextInfoParser::Instance()->Init(MAX_QUEST_TEXT_INFO);
    AchievementParser::Instance()->Init(100);
	GuildMarkInfoParser::Instance()->Init(100);
    EtherWeaponEffectInfoParser::Instance()->Init(100);
	NPCImageInfoParser::Instance()->Init(20);

    InDunOptionParser::Instance()->Init(100);

    AmbientSoundInfoParser::Instance()->Init(300);

	MissionInfoParser::Instance()->Init();
	MissionTextInfoParser::Instance()->Init(MAX_MISSION_TEXT_INFO);

    MessageText::Instance()->Init(MESSAGE_TOTAL_MAX);

    TotalGuideParser::Instance()->Init(500);

    ColorInfoParser::Instance()->Init(100);

#ifdef _NA_002935_20110704_ITEM_SCORE
    ItemScoreParser::Instance()->Init(200);
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    FameListParser::Instance()->Init();
    ReputeListParser::Instance()->Init();
    TitleListParser::Instance()->Init();
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CurrencyInfoParser::Instance()->Init();
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundFlagInfoParser::Instance()->Init(20);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
	RadarFullMissionInfoParser::Instance()->Init(100);
#endif // _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_000000_20130114_RENEWER_UI
	EnduranceInfoParser::Instance()->Init(20);
#endif

    OptionItemInfoParser::Instance()->Init(0);

#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBookParser::Instance()->Init(500);
    MonsterBookDropOrderParser::Instance()->Init(200);
    MonsterBookDropItemParser::Instance()->Init(500);
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::Instance()->Init(100);
    GuildFacilityImageParser::Instance()->Init(100);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceParser::Instance()->Init(30);
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	StatueInfoParser::Instance()->Init(100);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	AwakeningInfoParser::Instance()->Init(500);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	NotificationInfoParser::Instance()->Init(500);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    SpaZoneInfoParser::Instance()->Init(100);
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    GuildMissionTreeParser::Instance()->Init();
    SonnenScheinRewardInfoParser::Instance()->Init();
#endif //_NA_008334_20150608_SONNENSCHEIN
}

BOOL	ClientScriptParserManager::RegisterParser()
{

	char PACK_FILE_NAME[MAX_PATH];

	if( GENERALPARAM->IsUsePackFile() )
	{
		strcpy( PACK_FILE_NAME, "Data\\Resource.wpk" );
		PACKER_MANAGER()->OpenPackFile("Data\\Resource.wpk");
	}
	else
	{
		ZeroMemory( PACK_FILE_NAME, MAX_PATH );
	}


	if( GENERALPARAM->IsSystemPackFileName() )
	{
		PACKER_MANAGER()->OpenPackFile("System\\System.wpk");
		ERROR_PARSER( RegisterScriptData( SCRIPT_GAME_OPTION, "system\\GameOption.ini", GameOptionParser::Instance(),SEPERATOR_WHITESPACE_ONLY_STRING, "System\\System.wpk") );
	}
	else 
	{
		ERROR_PARSER( RegisterScriptData( SCRIPT_GAME_OPTION, "system\\GameOption.ini", GameOptionParser::Instance(), SEPERATOR_WHITESPACE_ONLY_STRING, NULL) );
	}

	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt", MapInfoParser::Instance() ,SEPERATOR_WHITESPACE  ,PACK_FILE_NAME) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_CHARGEITEMINFO, "data\\ChargeIteminfo.txt", ItemInfoParser::Instance(),SEPERATOR_WHITESPACE   ,PACK_FILE_NAME) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_EXPVALUEINFO, "data\\ExpValueInfo.txt", ExpValueInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ERROR_PARSER( RegisterScriptData(SCRIPT_EXCHANGE_REWARD_INFO, "data\\ExchangeRewardInfo.txt", ExchangeRewardInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
#endif

	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELD, "data\\Field.txt", MapInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME , TRUE ));
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWMO, "data\\Fieldwmo.txt", MapInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME , TRUE ));
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWUT, "data\\Fieldwut.txt", MapInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME , TRUE ));
	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD, "data\\World.txt", MapInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAPENTERANCEPORTAL, "data\\MapEnterancePortal.txt", MapInfoParser::Instance() ,SEPERATOR_WHITESPACE,PACK_FILE_NAME ) );
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONRANKINGLVAREA, "data\\MissionRankingLVArea.txt", GET_MISSION_RANK_MANAGER(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME) );
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
	ERROR_PARSER( RegisterScriptData( SCRIPT_WEAPONITEMINFO, "data\\WeaponItemInfo.txt", ItemInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ARMORITEMINFO, "data\\ArmorItemInfo.txt", ItemInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WASTEITEMINFO, "data\\WasteItemInfo.txt", ItemInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ACCESSORYITEMINFO, "data\\AccessoryItemInfo.txt", ItemInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMTYPELIST, "data\\ItemTypeList.txt", ItemTypeList::Instance(),SEPERATOR_WHITESPACE   ,PACK_FILE_NAME) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_REWARDINFOLIST, "data\\RewardInfoList.txt", RewardInfoList::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_LIMITED_ITEM_INFO, "data\\LimitedItemInfo.txt", LimitedItemInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SKILLINFO, "data\\SkillInfo.txt", SkillInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STYLEINFO, "data\\StyleInfo.txt", SkillInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STATEINFO, "data\\StateInfo.txt", StateInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_NPCINFO, "data\\NPCInfo.txt", NPCInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_NPCEXTRAINFO, "data\\NPCExtraInfo.txt", NPCInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SHOPINFO, "data\\ShopInfo.txt", ShopInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCOMPOSITEINFO, "data\\ItemCompositeInfo.txt", ItemCompositeParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCOMPOSELIST, "data\\ItemComposeList.txt", ItemComposeListParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );

#ifdef _NA_003740_20111122_RANDOM_ITEM
    // 순서에 주의!
    ERROR_PARSER( RegisterScriptData( SCRIPT_RANDOM_ITEM_VALUE, "data\\RandomItemValue.txt", RandomItemParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_RANDOM_ITEM_OPTION_TYPE, "data\\RandomItemOptionType.txt", RandomItemParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_RANDOM_ITEM_INFO, "data\\RandomItemInfo.txt", RandomItemParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_RANDOM_ITEM_KEY, "data\\RandomItemKey.txt", RandomItemParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
#endif // _NA_003740_20111122_RANDOM_ITEM

    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_OPTION_INFO, "data\\ZardOptionInfo.txt", SocketOptionParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_POINT_INFO, "data\\ZardPointInfo.txt", SocketOptionParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER( RegisterScriptData( SCRIPT_SUNEVENTINFO, "data\\SunEventInfo.txt", SunEventInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STYLEQUICKREGISTINFO, "data\\StyleQuickRegistInfo.txt", StyleQuickRegistInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_RANKOPTIONINFO, "data\\RankOptionInfo.txt", RankOptionParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONREWARDINFO, "data\\MissionRewardInfo.txt", MissionRewardParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_REQUIREFIELDLEVELINFO, "data\\RequireFieldLevelInfo.txt", RequireFieldLevelInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCRYSTALIZELIST, "data\\ItemCrystalizeList.txt", ItemCrystalizeListParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_EXTRASTONEOPTIONINFO, "data\\ExtraStoneOptionInfo.txt", ExtraStoneOptionInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
#ifdef __NA_001358_ENCHANT_UNIFICATION
	ERROR_PARSER(RegisterScriptData(SCRIPT_INSTANT_ENCHANT, "data\\InstantEnchant.txt", InstantEnchantParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#else
	ERROR_PARSER( RegisterScriptData( SCRIPT_ENCHANT, "data\\Enchant.txt", EnchantParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
#endif // __NA_001358_ENCHANT_UNIFICATION
	ERROR_PARSER( RegisterScriptData( SCRIPT_INITCHARINFO, "data\\InitCharInfoList.txt", CharInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );

	//2007.07.30 작업 

	ERROR_PARSER( RegisterScriptData( SCRIPT_ATTACHMENT_OFFSET_INFO, "Data\\AttachmentOffset.txt", AttachmentOffsetInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_BGM_SOUND_INFO, "Data\\BGMInfo.txt", BGMSoundInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_CHAR_SOUND_INFO, "Data\\CharSoundInfo.txt", CharSoundInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_COMPOSITE_NPC_INFO, "Data\\composite_NPC.txt", CompositeNPCInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_DLG_POS_INFO, "Data\\DialogPosInfo.txt", DlgPosInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ENVIRONMENT_INFO, "Data\\WorldConfig.txt", EnvironmentInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_EVENT_INFO, "Data\\EventInfo.txt", EventInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_HEAD_MESH_INFO, "Data\\HeadMeshInfo.txt", HeadMeshInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_TOTAL_GUIDE_MESSAGE, GlobalFunc::AddNationName("Data\\LibraryText.txt") , GuideMessageParser::Instance(),SEPERATOR_QUOTATIONMARKFORSTR  ,PACK_FILE_NAME ,TRUE) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_HELP_INFO, "Data\\HelpInfo.txt", HelpInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_HERO_HEIGHT_INFO, "Data\\HeroHeightsInfo.txt", HeroHeightInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_HERO_VARIATION_INFO, "Data\\HeroVariationInfo.txt", HeroVariationInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_HERO_VOICE_INFO, "Data\\HeroVoiceInfo.txt", HeroVoiceInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEM_SOUND_INFO, "Data\\ItemSoundInfo.txt", ItemSoundInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_LIMITED_MAP_PORT, "Data\\LimitedMapPort.txt", LimitedMapPortParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MONSTER_VOICE_INFO, "Data\\MonsterVoiceInfo.txt", MonsterVoiceInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_NPC_VOICE_INFO, "Data\\NPCVoiceInfo.txt", NPCVoiceInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_OPTION_LIST_INFO, "Data\\OptionList.txt", OptionListParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_PROJECTILE_INFO, "Data\\ProjectileInfo.txt", ProjectileInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_RADAR_INFO, "Data\\RadarInfo.txt", RadarInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SKILL_EFFECT_INFO, "Data\\Effect\\skilleffect.txt", SkillEffectInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SKILL_TEXT_INFO, "Data\\Skilltext.txt", SkillTextInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WEAPON_SOUND_INFO, "Data\\WeaponSoundInfo.txt", WeaponSoundInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
	ERROR_PARSER( RegisterScriptData( SCRIPt_GRAPHIC_RESOURCE_LIST, "Data\\GraphicResourceList.txt", GraphicResourceList::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SKILL_INVEN_STORE, "data\\SkillInventoryStore.txt", SkillInventoryStoreParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_SKILLEVENTEFFECT_INFO, "Data\\SkillEventEffectInfo.txt", cSkillEventEffectInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	ERROR_PARSER( RegisterScriptData( SCRIPT_COSTUMEITEM_INFO, "Data\\CostumeItemInfo.txt", cCostumeItemInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ERROR_PARSER( RegisterScriptData( SCRIPT_BATTLE_GROUND_FLAG_INFO, "Data\\BattleGroundFlagInfo.txt", BattleGroundFlagInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//#ifdef HAIR_COLOR_VARIATION
//	ERROR_PARSER( RegisterScriptData( SCRIPT_HEAD_COLOR_INFO, "Data\\HeadColorInfo.txt", HeadColorInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
//#endif
    ERROR_PARSER(RegisterScriptData(SCRIPT_MESSAGE, "data\\message.txt" , MessageInfoParser::Instance() ,SEPERATOR_WHITESPACE ,PACK_FILE_NAME ,TRUE));
#ifdef _NA_008364_20150623_CHANGE_TEXT_PATH
	ERROR_PARSER(RegisterScriptData(SCRIPT_MESSAGE_TEXT, GlobalFunc::AddNationName("messagetext.txt"), MessageText::Instance() ,SEPERATOR_QUOTATIONMARKFORSTR ,NULL ,TRUE));
#else
    ERROR_PARSER(RegisterScriptData(SCRIPT_MESSAGE_TEXT, GlobalFunc::AddNationName("data\\messagetext.txt"), MessageText::Instance() ,SEPERATOR_QUOTATIONMARKFORSTR ,PACK_FILE_NAME ,TRUE));
#endif // _NA_008364_20150623_CHANGE_TEXT_PATH
#ifdef _NA_008364_20150623_CHANGE_TEXT_PATH
	ERROR_PARSER(RegisterScriptData(SCRIPT_HELP_MESSAGE, GlobalFunc::AddNationName("HelpLoadingMessage.txt"), HelpMessageParser::Instance() ,SEPERATOR_QUOTATIONMARKFORSTR ,NULL ,TRUE));
#else
	ERROR_PARSER(RegisterScriptData(SCRIPT_HELP_MESSAGE, GlobalFunc::AddNationName("Data\\HelpLoadingMessage.txt"), HelpMessageParser::Instance() ,SEPERATOR_QUOTATIONMARKFORSTR ,PACK_FILE_NAME ,TRUE));
#endif // _NA_008364_20150623_CHANGE_TEXT_PATH
#ifdef _NA_008364_20150623_CHANGE_TEXT_PATH
	ERROR_PARSER(RegisterScriptData(SCRIPT_QUEST_TEXT_INFO,GlobalFunc::AddNationName("QuestText.txt"), QuestTextInfoParser::Instance(), SEPERATOR_QUOTATIONMARKFORSTR ,NULL ,TRUE));
#else
	ERROR_PARSER(RegisterScriptData(SCRIPT_QUEST_TEXT_INFO,GlobalFunc::AddNationName("Data\\QuestText.txt"), QuestTextInfoParser::Instance(), SEPERATOR_QUOTATIONMARKFORSTR ,PACK_FILE_NAME ,TRUE));
#endif // _NA_008364_20150623_CHANGE_TEXT_PATH
	ERROR_PARSER(RegisterScriptData(SCRIPT_QUESTINFO_MANAGER, "Data\\QuestInfo.txt", &g_QuestInfoManager,SEPERATOR_WHITESPACE ,PACK_FILE_NAME));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ACHIEVEMENT_INFO, "data\\ACScript.txt", AchievementParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILDINFO, "Data\\GuildInfo.txt" , GuildInfoParser::Instance() ,SEPERATOR_WHITESPACE, PACK_FILE_NAME));
#endif //_NA_003923_20120130_GUILD_RENEWAL
	ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_RANK_INFO, "Data\\GuildRankInfo.txt" , GuildRankInfoParser::Instance() ,SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_RIGHT_INFO,"Data\\GuildRightInfo.txt", GuildRightInfoParser::Instance(),SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_SETITEM_OPTION_INFO, "data\\SetItemoptioninfo.txt", SetItemOptionInfoParser::Instance() ,SEPERATOR_WHITESPACE ,PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_MARK, "Data\\GuildMarkInfo.txt", GuildMarkInfoParser::Instance() , SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_FATE_ITEM_INFO, "Data\\LuckyIteminfo.txt", FateItemInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_PET_INFO, "data\\PetManageInfo.txt", PetInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME));
    ERROR_PARSER( RegisterScriptData( SCRIPT_RIDING_INFO, "data\\RidingInfo.txt", RidingInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );

    ERROR_PARSER( RegisterScriptData( SCRIPT_SPECIAL_TEXTURE, "data\\SpecialTexture\\SpecialTexture.txt", SpecialTexutureInfoParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	
	ERROR_PARSER( RegisterScriptData( SCRIPT_GAMBLE_OPTION, "data\\GambleOptionList.txt", LimitedOptionParser::Instance(),SEPERATOR_WHITESPACE  ,PACK_FILE_NAME ) );
	ERROR_PARSER(RegisterScriptData(SCRIPT_ETHER_WEAPON_EFFECT, "data\\EtherWeaponEffectInfo.txt", EtherWeaponEffectInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
	ERROR_PARSER(RegisterScriptData(SCRIPT_LOTTO_INFO, "data\\LotteryInfo.txt", LotteryInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

#ifdef	__NA_001062_20080623_STRENGTH_PENALTY
	ERROR_PARSER(RegisterScriptData(SCRIPT_STRENGTH_PENALTY, "data\\StrengthPenalty.txt", StrengthPenaltyParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
	ERROR_PARSER(RegisterScriptData(SCRIPT_STRENGTH_UI, "data\\StrengthUI.txt", StrengthUIParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif//__NA_001062_20080623_STRENGTH_PENALTY(내구도패널티)

	ERROR_PARSER(RegisterScriptData(SCRIPT_NPC_IMAGE_INFO, "data\\NPCImageInfo.txt", NPCImageInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_HEROVARIATION_INFO, "data\\ItemtryonInfo.txt", HeroCustomInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_SSQINFO, "data\\SSQinfo.txt", SSQInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));
	ERROR_PARSER(RegisterScriptData(SCRIPT_SSQPATTERN, "data\\SSQPattern.txt", SSQInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));

	ERROR_PARSER(RegisterScriptData(SCRIPT_COLLECT, "data\\CollectInfo.txt", CollectionParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

#ifdef __NA_001358_ENCHANT_UNIFICATION
	ERROR_PARSER(RegisterScriptData(SCRIPT_INSTANT_ENCHANT, "data\\InstantEnchant.txt", InstantEnchantParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif // __NA_001358_ENCHANT_UNIFICATION
	ERROR_PARSER(RegisterScriptData(SCRIPT_CONSTANT_DIALOG, "data\\ConstantDialogList.txt", ConstantDialogListParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

	ERROR_PARSER(RegisterScriptData(SCRIPT_PKSCRIPT, "data\\PKScript.txt", PKScriptParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
	ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATIONINFO, "data\\DominationInfo.txt", DominationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
	ERROR_PARSER(RegisterScriptData(SCRIPT_INDUN_OPTION, "data\\Topfloors.txt", InDunOptionParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

    ERROR_PARSER(RegisterScriptData(SCRIPT_AMBIENT_SOUND_INFO, "data\\AmbientInfo.txt", AmbientSoundInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

    ERROR_PARSER(RegisterScriptData(SCRIPT_PERKINFO, "data\\PerkInfo.txt", PerkInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME));

	ERROR_PARSER(RegisterScriptData(SCRIPT_DISPLAY_FOR_MISSION_OBJECT, "data\\MissionInformation.txt", MissionInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSION_TEXT_INFO,GlobalFunc::AddNationName("Data\\MissionText.txt"), MissionTextInfoParser::Instance(), SEPERATOR_QUOTATIONMARKFORSTR ,PACK_FILE_NAME ,TRUE) );

#ifdef _YMS_AUTOBOT_GAME
	ERROR_PARSER(RegisterScriptData(SCRIPT_AUTOMATIC_INFO, "data\\AutomaticInfo.txt", AutomaticInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_AUTOMATIC_COMBO_INFO, "data\\AutomaticComboInfo.txt", AutomaticComboInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_YMS_AUTOBOT_GAME

    ERROR_PARSER(RegisterScriptData(SCRIPT_CUBE_COMPOSE, "data\\CubeComposeInfo.txt", CubeSystemParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_CUBE_DECOMPOSE, "data\\CubeDeCompositionInfo.txt", CubeSystemParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

    ERROR_PARSER(RegisterScriptData(SCRIPT_SOCKET_ITEM_RATIO_INFO, "data\\SocketItemRatio.txt", SocketItemRatioParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_COMPOSE_INFO, "data\\ZardComposeInfo.txt", ZardComposeInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

#ifdef _CHINA
    // TextureListInfo.txt TextureAnimationInfo.txt 는 _cn.txt를 사용할 필요가 없다.
    ERROR_PARSER(RegisterScriptData(SCRIPT_TEXTURE_LIST_INFO, "data\\TextureListInfo.txt", TextureListInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TEXTURE_ANIMATION_INFO, "data\\TextureAnimationInfo.txt", TextureAnimationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#else
    ERROR_PARSER(RegisterScriptData(SCRIPT_TEXTURE_LIST_INFO, GlobalFunc::AddNationName("data\\TextureListInfo.txt"), TextureListInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TEXTURE_ANIMATION_INFO, GlobalFunc::AddNationName("data\\TextureAnimationInfo.txt"), TextureAnimationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif

    ERROR_PARSER(RegisterScriptData(SCRIPT_CREATECHAR_CUSTOMCAMERA_INFO, "data\\CreateCharCustomCamera.txt", CreateCharCustomCameraInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_LOADINGTEXTURE_INFO, "data\\LoadingTextureInfo.txt", LoadingTextureInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

    #ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        ERROR_PARSER(RegisterScriptData(SCRIPT_CHARACTER_CREATION_INFO, "data\\NewChaCreation.txt", CharacterCreationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    #else
        ERROR_PARSER(RegisterScriptData(SCRIPT_CHARACTER_CREATION_INFO, "data\\ChaCreation.txt", CharacterCreationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    #endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    

    HANDLE fileHandle = CreateFile("data\\ColorInfo.txt",GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        ERROR_PARSER(RegisterScriptData(SCRIPT_COLOR_INFO, "data\\ColorInfo.txt", ColorInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
		::CloseHandle(fileHandle);
    }

#ifdef _YMS_TREECONTROL
    ERROR_PARSER(RegisterScriptData(SCRIPT_TOTAL_GUIDE_TREE_CONTROL, "data\\Interface\\TreeMenu_Library.txt", TreeMenuInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TREE_CONTROL, "data\\Interface\\TreeMenuInfoParser.txt", TreeMenuInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    //ERROR_PARSER(RegisterScriptData(SCRIPT_TREE_CONTROL, "data\\Interface\\TreeMenuInfoParser_Shop.txt", TreeMenuInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    //ERROR_PARSER(RegisterScriptData(SCRIPT_TREE_CONTROL_ICON, "data\\TextureListInfoTreeMenu.txt", TextureListInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_YMS_TREECONTROL

    ERROR_PARSER(RegisterScriptData(SCRIPT_TOTAL_GUIDE, "data\\LibraryList.txt", TotalGuideParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TOTAL_GUIDE_PAGE, "data\\LibraryPage.txt", TotalGuideParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TOTAL_GUIDE_NOTICE, "data\\Notice.txt", TotalGuideParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TOTAL_GUIDE_IMAGE, "data\\LibraryImageList.txt", TotalGuideParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#ifdef _NA_002935_20110704_ITEM_SCORE
    ERROR_PARSER(RegisterScriptData(SCRIPT_ITEM_SCORE_COMMON, "data\\ItemScoreInfo.txt", ItemScoreParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_20110630_BATTLE_ZONE_RENEWAL
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLEZONE_TREE, "data\\BattlezoneMissiontree.txt", BattlezoneMissionTreeParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif//_NA_20110630_BATTLE_ZONE_RENEWAL

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_FAME_LIST, "data\\FameList.txt", FameListParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_REPUTE_LIST, "data\\ReputeList.txt", ReputeListParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_TITLE_LIST, "data\\TitleList.txt", TitleListParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_NA_003027_20111013_HONOR_SYSTEM

    ERROR_PARSER(RegisterScriptData(SCRIPT_HOT_KEY_INFO, "data\\HotKeyInfo.txt", HotKeyInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_CURRENCY_INFO, "data\\CurrencyInfo.txt", CurrencyInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    ERROR_PARSER(RegisterScriptData(SCRIPT_RESOURCE_VERSION, "data\\ResourceVersion.txt", ResourceVersionParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ERROR_PARSER(RegisterScriptData(SCRIPT_CHAOSZONE_INFO, "data\\ChaosZoneInfo.txt", ChaosZoneInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_CHAOSZONE_TREE, "data\\ChaosZoneTree.txt", ChaosZoneTreeParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLE_GROUND_INFO, "data\\BattleGroundInfo.txt", BattleGroundInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
	ERROR_PARSER(RegisterScriptData(SCRIPT_MISSIONHELP_INFO, "data\\MissionHelp.txt", RadarFullMissionInfoParser::Instance(), SEPERATOR_QUOTATIONMARKFORSTR, PACK_FILE_NAME,TRUE) );
#endif // _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_000000_20130114_RENEWER_UI
	ERROR_PARSER(RegisterScriptData(SCRIPT_ENDURANCE_INFO, "data\\EnduranceInfo.txt", EnduranceInfoParser::Instance(), SEPERATOR_QUOTATIONMARKFORSTR, PACK_FILE_NAME,TRUE) );
#endif

    ERROR_PARSER(RegisterScriptData(SCRIPT_OPTION_CATEGORY_INFO, "data\\GameOptionList.txt", OptionItemInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME,TRUE) );

#ifdef _NA_007086_20140318_MONSTERBOOK
    ERROR_PARSER(RegisterScriptData(SCRIPT_MONSTER_BOOK, "data\\MonsterBook.txt", MonsterBookParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME,TRUE) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_MONSTER_BOOK_DROP_ORDER_INFO, "data\\MonsterBookDropOrder.txt", MonsterBookDropOrderParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME,TRUE) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_MONSTER_BOOK_DROP_ITEM, "data\\MonsterBookDropItem.txt", MonsterBookDropItemParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME,TRUE) );
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_FACILITY_INFO, "data\\GuildFacilityInfo.txt", GuildFacilityInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_FACILITY_IMAGE_INFO, "data\\GuildFacilityImage.txt", GuildFacilityImageParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION  
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_ATTENDANCE, "data\\Attendance.txt", AttendanceParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	ERROR_PARSER(RegisterScriptData(SCRIPT_STATUE_INFO, "data\\StatueInfo.txt", StatueInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	ERROR_PARSER(RegisterScriptData(SCRIPT_AWAKENING_INFO, "data\\AwakeningInfo.txt", AwakeningInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	ERROR_PARSER(RegisterScriptData(SCRIPT_NOTIFICATION_INFO, "data\\NotificationInfo.txt", NotificationInfoParser::Instance(), SEPERATOR_WHITESPACE, PACK_FILE_NAME) );
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    ERROR_PARSER( RegisterScriptData(SCRIPT_SPA_ZONE_INFO, "Data\\SpaZoneInfo.txt", SpaZoneInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    ERROR_PARSER( RegisterScriptData(SCRIPT_GUILDMISSIONTREE, "Data\\GuildMissionTree.txt", GuildMissionTreeParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
    ERROR_PARSER( RegisterScriptData(SCRIPT_SONNENSCHEIN_REWARDINFO, "Data\\SonnenScheinReward.txt", SonnenScheinRewardInfoParser::Instance(),SEPERATOR_WHITESPACE ,PACK_FILE_NAME ) );
#endif //_NA_008334_20150608_SONNENSCHEIN

    return TRUE;
}

VOID	ClientScriptParserManager::DestroyInstanceEx()
{
    OptionItemInfoParser::Instance()->DestroyInstance();
	MapInfoParser::Instance()->DestroyInstance();
	ItemInfoParser::Instance()->DestroyInstance();
	SkillInfoParser::Instance()->DestroyInstance();
	StateInfoParser::Instance()->DestroyInstance();
	NPCInfoParser::Instance()->DestroyInstance();
	ShopInfoParser::Instance()->DestroyInstance();

#ifdef _NA_003740_20111122_RANDOM_ITEM
    RandomItemParser::Instance()->DestroyInstance();
#endif // _NA_003740_20111122_RANDOM_ITEM

	ItemCompositeParser::Instance()->DestroyInstance();
	ItemComposeListParser::Instance()->DestroyInstance();
	SocketOptionParser::Instance()->DestroyInstance();
	SunEventInfoParser::Instance()->DestroyInstance();
	StyleQuickRegistInfoParser::Instance()->DestroyInstance();
	ItemTypeList::Instance()->DestroyInstance();
	RewardInfoList::Instance()->DestroyInstance();
    LimitedItemInfoParser::Instance()->DestroyInstance();
	RankOptionParser::Instance()->DestroyInstance();
	MissionRewardParser::Instance()->DestroyInstance();
	RequireFieldLevelInfoParser::Instance()->DestroyInstance();
	ItemCrystalizeListParser::Instance()->DestroyInstance();
	ExtraStoneOptionInfoParser::Instance()->DestroyInstance();
	ExpValueInfoParser::Instance()->DestroyInstance();
#ifdef __NA_001358_ENCHANT_UNIFICATION
	InstantEnchantParser::Instance()->DestroyInstance();
#else
	EnchantParser::Instance()->DestroyInstance();
#endif // __NA_001358_ENCHANT_UNIFICATION

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ExchangeRewardInfoParser::Instance()->DestroyInstance();
#endif


	GameOptionParser::Instance()->DestroyInstance();


	////2007.07.30 작업 
	AttachmentOffsetInfoParser::Instance()->DestroyInstance();
	BGMSoundInfoParser::Instance()->DestroyInstance();
	CharSoundInfoParser::Instance()->DestroyInstance(); 
	CompositeNPCInfoParser::Instance()->DestroyInstance();
	DlgPosInfoParser::Instance()->DestroyInstance();
	EnvironmentInfoParser::Instance()->DestroyInstance();
	EventInfoParser::Instance()->DestroyInstance();

//#ifdef HAIR_COLOR_VARIATION
//	HeadColorInfoParser::Instance()->DestroyInstance();
//#endif

	HeadMeshInfoParser::Instance()->DestroyInstance();
    GuideMessageParser::Instance()->DestroyInstance();
	HelpInfoParser::Instance()->DestroyInstance();
	HelpMessageParser::Instance()->DestroyInstance();
	HeroHeightInfoParser::Instance()->DestroyInstance();
	HeroVariationInfoParser::Instance()->DestroyInstance();
	HeroVoiceInfoParser::Instance()->DestroyInstance();
	ItemSoundInfoParser::Instance()->DestroyInstance();
	LimitedMapPortParser::Instance()->DestroyInstance();
	MonsterVoiceInfoParser::Instance()->DestroyInstance();
	NPCVoiceInfoParser::Instance()->DestroyInstance();
	OptionListParser::Instance()->DestroyInstance();
	ProjectileInfoParser::Instance()->DestroyInstance();
	QuestTextInfoParser::Instance()->DestroyInstance();
	RadarInfoParser::Instance()->DestroyInstance();
	SkillEffectInfoParser::Instance()->DestroyInstance();
	SkillTextInfoParser::Instance()->DestroyInstance();
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	cCostumeItemInfoParser::Instance()->DestroyInstance();
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
    cSkillEventEffectInfoParser::Instance()->DestroyInstance();
	WeaponSoundInfoParser::Instance()->DestroyInstance();
	GraphicResourceList::Instance()->DestroyInstance();
	SkillInventoryStoreParser::Instance()->DestroyInstance();
    AchievementParser::Instance()->DestroyInstance();
	GuildRankInfoParser::Instance()->DestroyInstance();
	GuildRightInfoParser::Instance()->DestroyInstance();
	MessageInfoParser::Instance()->DestroyInstance();
	SetItemOptionInfoParser::Instance()->DestroyInstance();
	GuildMarkInfoParser::Instance()->DestroyInstance();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::Instance()->DestroyInstance();
    GuildFacilityImageParser::Instance()->DestroyInstance();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceParser::Instance()->DestroyInstance();
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
	FateItemInfoParser::Instance()->DestroyInstance();
	PetInfoParser::Instance()->DestroyInstance();
    RidingInfoParser::Instance()->DestroyInstance();

    SpecialTexutureInfoParser::Instance()->DestroyInstance();

	LimitedOptionParser::Instance()->DestroyInstance();
	EtherWeaponEffectInfoParser::Instance()->DestroyInstance();
	LotteryInfoParser::Instance()->DestroyInstance();

#ifdef	__NA_001062_20080623_STRENGTH_PENALTY
	StrengthPenaltyParser::Instance()->DestroyInstance();
	StrengthUIParser::Instance()->DestroyInstance();
#endif//__NA_001062_20080623_STRENGTH_PENALTY(내구도패널티)

	NPCImageInfoParser::Instance()->DestroyInstance();
	HeroCustomInfoParser::Instance()->DestroyInstance();
	SSQInfoParser::Instance()->DestroyInstance();

	CollectionParser::Instance()->DestroyInstance();

	//SolarSlot 내부에서 이 인스턴스를 사용중이라서, 리미티드 아이템 소지한 캐릭터 관련해서 메모리릭 발생 
	LimitedOptionParser::Instance()->DestroyInstance();	//TODO: 서버팀에서 SolarSlot 수정하면, 이 라인은 삭제하도록 한다.(필요없으므로) by kiki

	ConstantDialogListParser::Instance()->DestroyInstance();

	PKScriptParser::Instance()->DestroyInstance();
	DominationInfoParser::Instance()->DestroyInstance();
	InDunOptionParser::Instance()->DestroyInstance();

    AmbientSoundInfoParser::Instance()->DestroyInstance();

    PerkInfoParser::Instance()->DestroyInstance();

	MissionInfoParser::Instance()->DestroyInstance();
	MissionTextInfoParser::Instance()->DestroyInstance();

#ifdef _YMS_AUTOBOT_GAME
    AutomaticInfoParser::Instance()->DestroyInstance();
    AutomaticComboInfoParser::Instance()->DestroyInstance();
#endif //_YMS_AUTOBOT_GAME

    CubeSystemParser::Instance()->DestroyInstance();

    SocketItemRatioParser::Instance()->DestroyInstance();
    ZardComposeInfoParser::Instance()->DestroyInstance();

    TextureListInfoParser::Instance()->DestroyInstance();
    TextureAnimationInfoParser::Instance()->DestroyInstance();
    TextureAnimationManager::Instance()->DestroyInstance();

    MessageText::Instance()->DestroyInstance();

    CreateCharCustomCameraInfoParser::Instance()->DestroyInstance();
    LoadingTextureInfoParser::Instance()->DestroyInstance();

    CharacterCreationInfoParser::Instance()->DestroyInstance();

    ColorInfoParser::Instance()->DestroyInstance();
    TreeMenuInfoParser::Instance()->DestroyInstance();

#ifdef _NA_002935_20110704_ITEM_SCORE
    ItemScoreParser::Instance()->DestroyInstance();
#endif //_NA_002935_20110704_ITEM_SCORE

    TotalGuideParser::Instance()->DestroyInstance();

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    FameListParser::Instance()->DestroyInstance();
    ReputeListParser::Instance()->DestroyInstance();
    TitleListParser::Instance()->DestroyInstance();
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_20110630_BATTLE_ZONE_RENEWAL
    ChaosZoneInfoParser::Instance()->DestroyInstance();
	ChaosZoneTreeParser::Instance()->DestroyInstance();
	BattleGroundInfoParser::Instance()->DestroyInstance();
    BattlezoneMissionTreeParser::Instance()->DestroyInstance();
#endif//_NA_20110630_BATTLE_ZONE_RENEWAL

    HotKeyInfoParser::Instance()->DestroyInstance();

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CurrencyInfoParser::Instance()->DestroyInstance();
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    ResourceVersionParser::Instance()->DestroyInstance();
#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GuildInfoParser::Instance()->DestroyInstance();
#endif //_NA_003923_20120130_GUILD_RENEWAL
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundFlagInfoParser::Instance()->DestroyInstance();
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
	RadarFullMissionInfoParser::Instance()->DestroyInstance();
#endif // _NA_000000_20120911_FULL_MISSION_HELPER
#ifdef _NA_000000_20130114_RENEWER_UI
	EnduranceInfoParser::Instance()->DestroyInstance();
#endif
#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBookParser::Instance()->DestroyInstance();
    MonsterBookDropOrderParser::Instance()->DestroyInstance();
    MonsterBookDropItemParser::Instance()->DestroyInstance();
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	StatueInfoParser::Instance()->DestroyInstance();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	AwakeningInfoParser::Instance()->DestroyInstance();
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	NotificationInfoParser::Instance()->DestroyInstance();
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    SpaZoneInfoParser::Instance()->DestroyInstance();
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    GuildMissionTreeParser::Instance()->DestroyInstance();
    SonnenScheinRewardInfoParser::Instance()->DestroyInstance();
#endif //_NA_008334_20150608_SONNENSCHEIN
}




//------------------------------------------------------------------------------

bool ClientScriptParserManager::ReloadScript(const char* old_filename, const char* new_filename, int pool_size)
{
    TableData::CreateEnvironment();

    SCRIPT_DATA* script_data = FindScript(old_filename);
    if (script_data && (strlen(new_filename) > 0))
    {
        IParser* parser = script_data->m_pIParser;
        if (parser)
        {
            strcpy(script_data->m_szFileName, new_filename);

            parser->Release();
            parser->Init(pool_size);
            if (parser->Load(script_data->m_ScriptCode
                                ,script_data->m_szFileName
                                ,script_data->m_bBigData
                                ,script_data->m_SeperatorMode
                                ,script_data->m_szPackFileName
                                ,FALSE)
                )
            {
#ifdef _DH_OS_LANGUAGETYPE
                char string[256] = {0,};
                char temp[256] = {0,};
                static util::_tstring file_name;
                file_name.clear();
                strcpy(string, old_filename);
                sprintf(temp, "%s.txt", strtok(string, "_."));
                strcpy(string, GlobalFunc::AddNationName(kLanguageGlobal, temp, file_name));
                parser->Load(script_data->m_ScriptCode
                    ,file_name.c_str()
                    ,script_data->m_bBigData
                    ,script_data->m_SeperatorMode
                    ,script_data->m_szPackFileName
                    ,FALSE);
#endif//_DH_OS_LANGUAGETYPE
                strncpy(script_data->m_szVersion, parser->GetVersion(), _countof(script_data->m_szVersion));
                script_data->m_szVersion[_countof(script_data->m_szVersion) - 1] = '\0';
                WRITELOG(eLOG_LEVEL_INF, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("SCRIPT_DATA::Version = %s"), script_data->m_szVersion);

                TableData::DestroyEnvironment();
                return true;
            }
        }
    }

    TableData::DestroyEnvironment();

    return false;
}

#ifdef _DH_OS_LANGUAGETYPE
bool ClientScriptParserManager::UpdateMessageScript()
{
    bool result = FALSE;

    LanguageType old_type = CGameOption::Instance()->GetLanguageType();
    LanguageType new_type = kLanguageGlobal;

    util::_tstring old_file_name;
    util::_tstring new_file_name;

    GlobalFunc::AddNationName(old_type, "data\\messagetext.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "data\\messagetext.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_QUEST_TEXT_INFO);
    SUN_ASSERT_MSG(result, "MessageText.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\HelpLoadingMessage.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\HelpLoadingMessage.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), 100);
    SUN_ASSERT_MSG(result, "HelpLoadingMessage.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\QuestText.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\QuestText.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_QUEST_TEXT_INFO);
    SUN_ASSERT_MSG(result, "QuestText.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\MissionText.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\MissionText.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_QUEST_TEXT_INFO);
    SUN_ASSERT_MSG(result, "MissionText.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\LibraryText.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\LibraryText.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_HELP_INFO);
    SUN_ASSERT_MSG(result, "LibraryText.txt load failed");

    return result;
}
#endif//_DH_OS_LANGUAGETYPE
//------------------------------------------------------------------------------
