#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <IOCPServer.h>
#include "Battleserver.h"
#include "MasterServerSession.h"
#include <ISolarConsole_Define.h>
#include <ISolarConsole.h>
#include <Define.h>
#include "GameZoneManager.h"
#include "GameHuntingRoom.h"
#include "ServerOptionParser.h"
#include "GameInstanceDungeon.h"
#include <io.h>
#include "GameMissionRoom.h"
#include "GameShopManager.h"
#include "GameRankingManager.h"

#include "GameParty/NewGamePartyManager.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop/GameShop.Manager.h"
#endif

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingGradeManager.h"
#include "SunRanking/SunRankingManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

extern ISolarConsole * g_pIConsole;

BattleServer::BattleServer()
{
	SetKey( SERVER_KEY(0, 0, BATTLE_SERVER, 0) );
	SetGuid( BATTLE_SERVER );
}

BattleServer::~BattleServer()
{
}


#define CHECK_PARAM_NUMBER( variable_type, variable, szSectionName, keyName, defValue )		\
	variable_type variable = 0;																\
	__READ_CONFIG_NUMBER( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE
#define CHECK_PARAM_STRING( variable, szSectionName, keyName, defValue )		\
	char variable [128] = {0,};																\
	__READ_CONFIG_STRING( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE


BOOL BattleServer::Init()
{
	// �������� Ǯ �ɼ��� �ε��Ѵ�.
	CHECK_PARAM_NUMBER( BYTE, byBattleServerType, "ATTRIBUTES", "BATTLE_SERVER_TYPE", 0xFF );
	ServerOptionParser::Instance()->LoadPoolInfo( "ServerPoolOption.ini", byBattleServerType );

	//���� ��忡 ���� Field, Battle �� ���������� üũ�Ѵ�.
	ASSERT( CheckServerMode( GetMode(), GetServerType(), byBattleServerType ) );

	if( !GameServerEx::Init() )		return FALSE;

	//-------------------------------------------------------------------------------------------------
	// �α� ���� ���� �ʱ�ȭ
	//-------------------------------------------------------------------------------------------------
	DISPMSG(  "Creating GameLog File...\n" );
	g_pLOG = new GameLog;

	GAMELOG->PreProcess();

	CHECK_PARAM_STRING( pszGameLogfilePath,	"LOGFILE", "WOPS_LOGFILE_PATH", NULL );
	if( _access( pszGameLogfilePath, 0 ) == -1 )		//������ �������� ������..
	{
		if( !CreateDirectory( pszGameLogfilePath, NULL) )
			printf( "CreateDirectory Fail, path = %s \n", pszGameLogfilePath  );
	}
	TCHAR szBinPath[MAX_PATH], szTextPath[MAX_PATH];
	_sntprintf( szBinPath, MAX_PATH, "%s\\\\BATTLE", pszGameLogfilePath );
	_sntprintf( szTextPath, MAX_PATH, "%s\\\\BATTLE_TEXT", pszGameLogfilePath );
	CHECK_PARAM_NUMBER( BYTE, byWopsLogOption, "LOGFILE", "WOPS_LOGFILE_OPTION", 0xFF );
#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
	TCHAR szFileName[MAX_PATH];
	_sntprintf( szFileName, MAX_PATH, "%d_SUN_BATTLE", NATION_BUILD_VERSION );
	if( !g_pLOG->Init( byWopsLogOption, szFileName, _T(szBinPath), _T(szTextPath) ) )
#else
	if( !g_pLOG->Init( byWopsLogOption, _T("SUN_BATTLE"), _T(szBinPath), _T(szTextPath) ) )
#endif
		P_FAILED;
	else
		P_SUCCESS;

	// Į���� 50�� ����
//	for( int i = 0; i < 50; ++i )
//	{
//		GameInstanceDungeon *pDungeon = (GameInstanceDungeon*)g_GameZoneManager.CreateZone( eZONETYPE_INSTANCE, 1000+i, 60301 );
//		pDungeon->OnCreate();
//	}

	// �̼ǹ� ����
//	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
//
//	for( int i = 0; i < stServerOptionInfo.m_wFORCED_WARP_MOVE_DELAY; ++i )
//	{
//		GameMissionRoom *pMission = (GameMissionRoom*)g_GameZoneManager.CreateZone( eZONETYPE_MISSION, 1000+i, 2 % (i+1) +1 );
//		pMission->OnCreate();
//	}

	return TRUE;
}

VOID BattleServer::Release()
{
	// �α�
	GameServerEx::Release();
}


BOOL BattleServer::KeyEvent( char ch )
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
