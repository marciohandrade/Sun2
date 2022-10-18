
#include "stdafx.h"
#include "Player.h"

#include <PacketStruct.h>
#include <Protocol.h>
#include <conio.h>
#include <iostream>

// Namespace util Header
#include <FunctionMap.h>
#include <IOCPserver.h>


#include "FieldServer.h"
// Handler Header
#include "GameDBProxySession.h"
#include "GameZoneManager.h"

#include <ISolarConsole_Define.h>
#include <ISolarConsole.h>
#include "ServerOptionParser.h"
#include "GameParty/NewGamePartyManager.h"

#include "GameShopManager.h"
#include <io.h>
#include "PerfCheck.h"
#include <ServerOptionParserEx.h>
#include "GameRankingManager.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop/GameShop.Manager.h"
#endif

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingGradeManager.h"
#include "SunRanking/SunRankingManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

extern ISolarConsole * g_pIConsole;

	

FieldServer::FieldServer(void)
{
	SetKey( SERVER_KEY(0, 0, FIELD_SERVER, 0) );
	SetGuid( FIELD_SERVER );
}

FieldServer::~FieldServer(void)
{
}

VOID FieldServer::Release()
{
	GameServerEx::Release();
}

#define CHECK_PARAM_NUMBER( variable_type, variable, szSectionName, keyName, defValue )		\
	variable_type variable = 0;																\
	__READ_CONFIG_NUMBER( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE
#define CHECK_PARAM_STRING( variable, szSectionName, keyName, defValue )		\
	char variable [128] = {0,};																\
	__READ_CONFIG_STRING( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE

#include "GameHuntingField.h"

BOOL FieldServer::Init()
{
	// 필드서버 풀 옵션을 로드한다.
	CHECK_PARAM_NUMBER( BYTE, byFieldServerType, "ATTRIBUTES", "FIELD_SERVER_TYPE", 0xFF );
	ServerOptionParser::Instance()->LoadPoolInfo( "ServerPoolOption.ini", byFieldServerType );

	//서버 모드에 따른 Field, Battle 의 설정파일을 체크한다.
	ASSERT( CheckServerMode( GetMode(), GetServerType(), byFieldServerType ) );

	if( !GameServerEx::Init() )		return FALSE;

	//-------------------------------------------------------------------------------------------------
	// 로그 파일 정보 초기화
	//-------------------------------------------------------------------------------------------------
	DISPMSG(  "Creating GameLog File...\n" );

	g_pLOG = new GameLog;

	GAMELOG->PreProcess();

	CHECK_PARAM_STRING( pszGameLogfilePath,		"LOGFILE", "WOPS_LOGFILE_PATH", NULL );
	if( _access( pszGameLogfilePath, 0 ) == -1 )		//폴더가 존재하지 않으면..
	{
		if( !CreateDirectory( pszGameLogfilePath, NULL) )
			printf( "CreateDirectory Fail, path = %s \n", pszGameLogfilePath  );
	}
	TCHAR szBinPath[MAX_PATH], szTextPath[MAX_PATH];
	_sntprintf( szBinPath, MAX_PATH, "%s\\\\FIELD", pszGameLogfilePath );
	_sntprintf( szTextPath, MAX_PATH, "%s\\\\FIELD_TEXT", pszGameLogfilePath );
	CHECK_PARAM_NUMBER( BYTE, byWopsLogOption, "LOGFILE", "WOPS_LOGFILE_OPTION", 0xFF );
#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
	TCHAR szFileName[MAX_PATH];
	_sntprintf( szFileName, MAX_PATH, "%d_SUN_FIELD", NATION_BUILD_VERSION );
	if( !g_pLOG->Init( byWopsLogOption, szFileName, _T(szBinPath), _T(szTextPath) ) )
#else
	if( !g_pLOG->Init( byWopsLogOption, _T("SUN_FIELD"), _T(szBinPath), _T(szTextPath) ) )
#endif
		P_FAILED;
	else
		P_SUCCESS;

// test
#ifdef _SUSUNLOVE_TEST_CODE_
	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	for( int i = 0; i < stServerOptionInfo.m_byMOVE_SYNC_WARNING_DISTANCE; ++i )
	{
//		GameHuntingField *pHunting = (GameHuntingField*)g_GameZoneManager.CreateZone( eZONETYPE_FIELD, 1000+i, 54061 );
//		pHunting->OnCreate();
	}
#endif

	return TRUE;
}

BOOL FieldServer::KeyEvent( char ch )
{
	switch(ch)
	{
	case 'P': 
		NewGamePartyManager::Instance()->Display();
		break;
	case ' ': 
		DisplayServerInfo(); 
		break;
	case 'K':
#ifdef _NA_008012_20150130_RANKING_SYSTEM
		g_GameRankingMgr.Display();
#endif // _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        SunRankingGradeManager::Instance()->Display();
        SunRankingManager::Instance()->Display();
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
		break;
	case 'T': 
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
		WaverixTestUnit();
#endif
		break;
	case 'R':
		ReloadAttributes();
		break;
	case 'U':
		RunUnitTest();
		break;
	case 'C':
        #if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	    	g_GameShopManager.Display();
        #endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
		break;
	}

	return TRUE;
}

BOOL FieldServer::Update( DWORD tick )
{
	GameServerEx::Update( tick );

	return TRUE;
}



















