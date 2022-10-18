// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include <Global.Define.h>

#define WIN32_LEAN_AND_MEAN
#define _USING_SIMPLE_CONSOLE
#define _DEV_TAIYO_ADD_REGEN_TIME
//#define _DEV_TAIYO_PATHENGINE_CODE
//#define _SUSUNLOVE_TEST_CODE_
//#define _SUSUNLOVE_DRIVE_NOT_FOUND_		// ��ȹ���� ����̺� ���â!

//#pragma warning(push)
//#pragma warning(disable:4201)
//#pragma warning(disable:4100)
//#pragma warning(disable:4189)

#define WAVERIX_CODE_BACKUP				(0)
#define WAVERIX_CODE_RENEWAL			(1)
#define WAVERIX_CODE_TEST				(1)
#define WAVERIX_CODE_DELETE_RESERVED	(1)


#include <tchar.h>

#include <windows.h>
#include <iostream>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
#include <Define.h>
#include <Define.Build.h>
#include <ServerCommonDef.h>
#include <UtilityCommon.h>
#include <Const.h>
#include <ServerConst.h>
#include <Struct.h>
#include <ResultCode.h>
#include <chunk_stl.h>
#include <NetworkObject.h>

#include <PartyDefine.h>

#include <MemoryPoolFactory.h>
#include <MemoryPoolFactoryV.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include <SolarAssert.h>

#pragma warning(push)
#pragma warning(disable : 4100 4201)
#include <ProgramCommon/WzDataType.h>
#include <ProgramCommon/WzHashTable.h>
#include <3DEngine/Common/MathAssistant.h>
#include <3DEngine/Common/IncludeAll_NewWorld.h>
#include <3DEngine/3DTerrain/3DTerrain.h>
#include <3DEngine/3DTerrain/WorldBase.h>
#include <3DEngine/3DTerrain/PathExplorer.h>
#pragma warning(pop)
#include "MapInfo.h"

#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>
#include <PacketControl/WarMessages_Sync.h>

#include <Timer.h>


#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

// ������ �޶����� ��ġ�� ������ ����
#include <OptLogicByNation.h>


#include <SunLog.h>
#include <SecureLog.h>

// CppUnitLite
#include <TestHarness.h>


//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/WZ_MD5.lib")
#pragma comment( lib, "Psapi.lib" )

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

// <LIB><GAME SERVER>

#else //

// <LIB><GAME SERVER>

#endif //
//////////////////////////////////////////////////////////////////////////



// [min,max]

#define random(min, max)                    ((rand()%(int)(((max) + 1)-(min)))+ (min)) 

#ifdef _DEBUG

	#include <SUNConsole.h>
	extern SUNConsole g_SUNConsole;

	#ifndef _USING_SIMPLE_CONSOLE
		#define DISPMSG				g_pIConsole->OutputDisplayArg
	#else
		#define DISPMSG				printf
	#endif

	
	// ����׿� ��� �Լ�
	// ���α׷� �׽�Ʈ ��
	#define DEBUG1		printf
	#define DEBUG2		printf
	#define DEBUG3		printf

#else
	
	//#define DISPMSG(f)					__noop	

#endif

// ��������
#include <GameLog.h>
extern GameLog* g_pLOG;
#define GAMELOG		if(g_pLOG) g_pLOG

#include "GameServerStruct.h"
#include "ObjectDefine.h"
#include "GeneralGameServerFunction.h"
//
#include "./Skill_Info.h"
#include "State.h"
#include "Player.h"
#include "GameField.h"
#include "PlayerManager.h"
#include "GameShop/GameShopHeader.h"

#if _MSC_VER < 1500
    #define WAVERIX_USING_PCH_LIST
#endif
#ifdef WAVERIX_USING_PCH_LIST
    #include "./GameServerPch.hpp"
#endif
