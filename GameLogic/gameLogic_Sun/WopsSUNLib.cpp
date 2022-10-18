#include "stdafx.h"
#include "CommonDef.h"
#include "RewardInfoList.h"
#include <AreaConquest.h>
#include <AreaConquestInfoParser.h>
#include <QuestManager.h>
#include <QuestInfoManager.h>
#include <QuestScriptLoader.h>
#include <RankOptionParser.h>
#include <MapInfoParser.h>
#include <MissionManager.h>
#include <SkillInfoParser.h>
#include <ItemInfoParser.h>
#include <SCSkillSlotContainer.h>
#include <SCSkillSlot.h>
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>
#include <SolarFileReader.h>
#include "MissionRewardParser.h"
#include "SkillInventoryStoreParser.h"
#include <io.h>


#define ERROR_PARSER( x, y ) if(!x){/*SUNLOG( eCRITICAL_LOG, "%s 파일이 없습니다!!", y );*/}

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

	BOOL LoadParser( char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser, BOOL bBigData=FALSE )
	{
		return pParser->Load( eCode, pszFileName, bBigData, SEPERATOR_WHITESPACE, NULL, FALSE );
	}

#else	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

	BOOL LoadParser( char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser )
	{
		util::SolarFileReader sr;
		if( !sr.OpenFile( pszFileName, SEPERATOR_WHITESPACE ) )
			return FALSE;

		pParser->_LoadEx( eCode, sr, FALSE );

		sr.CloseFile();

		return TRUE;
	}

#endif	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

int _tmain(int argc, _TCHAR* argv[])
{
	//SUNLOG( eCRITICAL_LOG, "Script Check Start !!!\n\n" );

	char szModuleName[MAX_PATH];

	GetCurrentDirectory( MAX_PATH, szModuleName );

	std::string app_path(szModuleName);
	std::string check_path(szModuleName);
	std::string check_path2(szModuleName);
	check_path += "\\_bin";
	check_path2 += "\\Data";

	// 개발자 디버깅 상황인지 유저 체크 상황인지에 따라 데이타 경로가 달라진다.
	if( _access( (char*)check_path.c_str(), 0) != -1 )
		app_path += "\\_bin\\생성SUN4WOPS\\게임로직\\Data";
	else if( _access( (char*)check_path2.c_str(), 0) != -1 )
		app_path += "\\Data";

	//스트링테이블 파서로딩	
	ERROR_PARSER(m_StringTable.Init((char*)(app_path + "\\StringTable.txt").c_str()), "StringTable.txt init");
	ERROR_PARSER(m_Stringtableforname.Init((char*)(app_path + "\\Stringtableforname.txt").c_str()), "Stringtableforname.txt init");
	ERROR_PARSER(m_StringTableForProgram.Init((char*)(app_path + "\\StringTableForProgram.txt").c_str()), "StringTableForProgram.txt init");

	////////////////////////////////////
	//Item인포 스크립트 로딩	
	ItemInfoParser::Instance()->Init(1000, "");	
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\AccessoryItemInfo.txt").c_str(), SCRIPT_ACCESSORYITEMINFO, ItemInfoParser::Instance()),"AccessoryItemInfo");	
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\WeaponItemInfo.txt").c_str(), SCRIPT_WEAPONITEMINFO, ItemInfoParser::Instance()),"WeaponItemInfo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\ArmorItemInfo.txt").c_str(), SCRIPT_ARMORITEMINFO, ItemInfoParser::Instance()),"ArmorItemInfo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\WasteItemInfo.txt").c_str(), SCRIPT_WASTEITEMINFO, ItemInfoParser::Instance()),"WasteItemInfo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\ChargeItemInfo.txt").c_str(), SCRIPT_CHARGEITEMINFO, ItemInfoParser::Instance()),"ChargeItemInfo");

	SkillInfoParser::Instance()->Init(1000, "");
	SkillInventoryStoreParser::Instance()->Init(1000, "");	

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\SkillInventoryStore.txt").c_str(), SCRIPT_SKILL_INVEN_STORE, SkillInventoryStoreParser::Instance()),"SkillInventoryStore");
#else
	ERROR_PARSER(SkillInventoryStoreParser::Instance()->Load((char*)(app_path + "\\SkillInventoryStore.txt").c_str() ),"SkillInventoryStore");
#endif

	ERROR_PARSER(LoadParser( (char*)(app_path + "\\SkillInfo.txt").c_str(), SCRIPT_SKILLINFO, SkillInfoParser::Instance()),"SkillInfo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\StyleInfo.txt").c_str(), SCRIPT_STYLEINFO, SkillInfoParser::Instance()),"StyleInfo");

	MapInfoParser::Instance()->Init(100);

#if NATION_BUILD_VERSION >= 810
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Worldsubinfo.txt").c_str(), SCRIPT_WORLD_SUB_INFO, MapInfoParser::Instance()),"Worldsubinfo");
#endif
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\World.txt").c_str(), SCRIPT_WORLD, MapInfoParser::Instance()),"World");

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Field.txt").c_str(), SCRIPT_FIELD, MapInfoParser::Instance(), TRUE),"Field");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Fieldwmo.txt").c_str(), SCRIPT_FIELDWMO, MapInfoParser::Instance(), TRUE),"Fieldwmo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Fieldwut.txt").c_str(), SCRIPT_FIELDWUT, MapInfoParser::Instance(), TRUE),"Fieldwut");
#else
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Field.txt").c_str(), SCRIPT_FIELD, MapInfoParser::Instance()),"Field");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Fieldwmo.txt").c_str(), SCRIPT_FIELDWMO, MapInfoParser::Instance()),"Fieldwmo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Fieldwut.txt").c_str(), SCRIPT_FIELDWUT, MapInfoParser::Instance()),"Fieldwut");
#endif

	ERROR_PARSER(LoadParser( (char*)(app_path + "\\MapEnterancePortal.txt").c_str(), SCRIPT_MAPENTERANCEPORTAL, MapInfoParser::Instance()),"MapEnterancePortal");

	ERROR_PARSER(LoadParser( (char*)(app_path + "\\RankOptionInfo.txt").c_str(), SCRIPT_RANKOPTIONINFO, RankOptionParser::Instance()),"RankOptionInfo");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\SocketOptionInfo.txt").c_str(), SCRIPT_SOCKETOPTIONINFO, SocketOptionParser::Instance()),"SocketOptionInfo");

	ExtraStoneOptionInfoParser::Instance()->Init(100);
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\ExtraStoneOptionInfo.txt").c_str(), SCRIPT_EXTRASTONEOPTIONINFO, ExtraStoneOptionInfoParser::Instance()),"ExtraStoneOptionInfo");

	g_pQuestInfoManager = new QuestInfoManager2;
	ERROR_PARSER(g_pQuestInfoManager->Load((char*)(app_path + "\\QuestInfo.txt").c_str()), "QuestInfo");	

	RewardInfoList::Instance()->Init("");
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\RewardInfoList.txt").c_str(), SCRIPT_REWARDINFOLIST, RewardInfoList::Instance()),"RewardInfoList");

	AreaConquestInfoParser::Instance()->Init( 1000 );
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\ACScript.txt").c_str(), SCRIPT_ACSCRIPT, AreaConquestInfoParser::Instance()),"ACScript");


	MissionRewardParser::Instance()->Init(100);
#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\MissionRewardInfo.txt").c_str(), SCRIPT_MISSIONREWARDINFO, MissionRewardParser::Instance()),"MissionRewardInfo");
#else
	ERROR_PARSER(MissionRewardParser::Instance()->Load((char*)(app_path + "\\MissionRewardInfo.txt").c_str()),"MissionRewardInfo");
#endif

#if NATION_BUILD_VERSION >= 810
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Limitediteminfo.txt").c_str(), SCRIPT_LIMITED_ITEM_INFO, LimitedItemInfoParser::Instance()),"Limitediteminfo");	
#endif

#if NATION_BUILD_VERSION >= 811
	ERROR_PARSER(LoadParser( (char*)(app_path + "\\Luckyiteminfo.txt").c_str(), SCRIPT_FATE_ITEM_INFO, FateItemInfoParser::Instance()),"Luckyiteminfo");
#endif


	//SUNLOG( eCRITICAL_LOG, "\n\nScript Check End !!!\n\n" );

	system( "pause" );

	return 0;
}














