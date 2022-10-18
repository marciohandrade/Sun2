/**
 *	@file		GlobalData.h
 *	@brief		Global 변수들을 모아 두는 곳.
 *  @author		Unknown
 *	@version	Unknown
 */

//----------------------------------------------------------------------------
///** Global 변수들을 모아 두는 곳.
//*/
#ifndef		_GLOBALDATA_H
#define		_GLOBALDATA_H

#include "Input.h"
#include "Network.h"
#include "SunProfiler.h"
#include "../../Utility/TextFilter.h"
#include "KeyQueueManager.h"
#include "SkillQueueManager.h"
#include "ClientScriptParserManager.h"

#include "IDGenerator.h"
#include "DllTypeDef.h"


using namespace std;
using namespace stdext;

class Player;
class Monster;
class Hero;
class InterfaceManager;
class GmManager;
class AreaConquestManager;
class Character;

extern	eLOG_LEVEL				g_eLogLevel;
extern	bool					g_bNetRecvLog;
extern	bool					g_bNetSendLog;
extern  bool                    g_bShowPacket;

extern	HWND					g_hWndMain;
extern	HINSTANCE				g_hInstance;
extern	CDrawBase *				g_pSunRenderer;
extern  CEffectManager *		g_pEffectManager;
extern	CWorldBase *			g_pSunTerrain;
extern	bool					g_bExitFlag;

extern	IDGenerator				g_IDGenerator;

// xxx: 인터페이스 dll(ControlsDll.dll)은 암묵적(implicit) 링크시
// win2000에서 뻗어 버렸다. 그래서 부득이 명시적(explicit) 링크를 한다.
extern	FUNC_DialogManagerWZ	g_pContactDialog_FnPtr;

extern	Input					g_Input;
extern  Network	*				g_pNetwork;
extern  InterfaceManager		g_InterfaceManager;

extern	WORD					g_wFrameSkipping;

//	데이터 경로
extern	char					g_szRunPath[MAX_PATH];
extern	char					g_szDataPath[MAX_PATH];

extern	Hero *					g_pHero;
extern	WzRay					g_wrMouseRay;
extern	DWORD					g_CurTime;
extern Character *				g_pCurrentPickedObject;

extern BOOL						g_bShowDebugInfo;

extern float                    g_fGravity;
extern float                    g_fUpperForce;
extern float                    g_fMaxForce;
extern float					g_addGravity;
extern int						g_GravityMode;


extern CNSlangTextFilter		g_slangFilter;
extern CNSlangTextFilter		g_slangNameFilter;
extern CPathExplorer	*		g_pPathExplorer;
extern DWORD					tempId;
extern KeyQueueManager			g_KeyQueueManager;
extern SkillQueueManager		g_SkillQueue;
extern int                      g_TestSkillIndex;
extern D3DGAMMARAMP				g_D3dGammaRamp;

extern BOOL						g_bPlayerAutoMove;


extern int g_TempVariable[MAX_TEMP_VARIABLE];

#ifdef _DEV_VER
	extern CSunProfiler				g_Profiler;
	extern CSunProfiler				g_ProcessProfiler;
#endif

#ifdef _DEV_VER
	#define SUNPROFILE_BEGIN		g_Profiler.Begin
	#define SUNPROFILE_END			g_Profiler.End
	#define SUNPROCESSPROFILE_BEGIN	g_ProcessProfiler.Begin
	#define SUNPROCESSPROFILE_END	g_ProcessProfiler.End
#else
	inline void SunNoOperation_Profile( ...)	{}
	#define SUNPROFILE_BEGIN		SunNoOperation_Profile
	#define SUNPROFILE_END			SunNoOperation_Profile
	#define SUNPROCESSPROFILE_BEGIN	SunNoOperation_Profile
	#define SUNPROCESSPROFILE_END	SunNoOperation_Profile
#endif

void InitializeTempVariable();

extern const WzVector c_ZeroVector;
extern const WzVector c_UpVector;
extern const WzVector c_UnitVector;

const int SKILL_EFFECT_BLINK_TIME  = 10000;
//const DWORD EQUIPMENT_CHARACTER_KEY = MONSTER_OBJECT_KEY - 2;
//const DWORD CUSTOMIZE_CHARACTER_KEY = MONSTER_OBJECT_KEY - 3;
const TCHAR GLOBAL_EMPTY_STRING[] = _T("");
const DWORD EQUIPMENT_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 1;
const DWORD CUSTOMIZE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 2;
const DWORD CONVENIENCESTORE_CASH_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 3;   //!< 샵 캐시 캐릭터
const DWORD CONVENIENCESTORE_PACKAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 4;   //!< 샵 패키지 캐릭터
const DWORD CONVENIENCESTORE_MYPAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 5;   //!< 샵 내정보 캐릭터
const DWORD CONVENIENCESTORE_PPCARD_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 6;   //!< 샵 PP카드 캐릭터
const DWORD CONVENIENCESTORE_MILEAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 7;   //!< 샵 마일리지 캐릭터
const DWORD OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 8; //!< 상대방 정보보기창 캐릭터
const DWORD CONVENIENCESTORE_PREVIEW_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 9;   //!< 샵 패키지 아이템 미리보기
#ifdef _NA_007086_20140318_MONSTERBOOK
const DWORD MONSTERBOOK_PREVIEW_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 10;   //!< 몬스터도감 몬스터외형 미리보기
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
const DWORD STATUE_OBJECT_CHARACTER_KEY1 =  ONLYCLIENT_OBJECT_KEY + 11;         //!< 동상 캐릭터
//const DWORD STATUE_OBJECT_CHARACTER_KEY2 =  ONLYCLIENT_OBJECT_KEY + 12;
//const DWORD STATUE_OBJECT_CHARACTER_KEY3 =  ONLYCLIENT_OBJECT_KEY + 13;
//const DWORD STATUE_OBJECT_CHARACTER_KEY4 =  ONLYCLIENT_OBJECT_KEY + 14;
//const DWORD STATUE_OBJECT_CHARACTER_KEY5 =  ONLYCLIENT_OBJECT_KEY + 15;
//const DWORD STATUE_OBJECT_CHARACTER_KEY6 =  ONLYCLIENT_OBJECT_KEY + 16;
//const DWORD STATUE_OBJECT_CHARACTER_KEY7 =  ONLYCLIENT_OBJECT_KEY + 17;
//const DWORD STATUE_OBJECT_CHARACTER_KEY8 =  ONLYCLIENT_OBJECT_KEY + 18;
//const DWORD STATUE_OBJECT_CHARACTER_KEY9 =  ONLYCLIENT_OBJECT_KEY + 19;
//const DWORD STATUE_OBJECT_CHARACTER_KEY10 = ONLYCLIENT_OBJECT_KEY + 20;
//const DWORD STATUE_OBJECT_CHARACTER_KEY11 = ONLYCLIENT_OBJECT_KEY + 21;
//const DWORD STATUE_OBJECT_CHARACTER_KEY12 = ONLYCLIENT_OBJECT_KEY + 22;
// 23번부터 사용해 주세요.
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

//------------------------------------------------------------------------------
/**
    렌덤에게 모든 도메인중 하나의 도메인으로 접속하며

    실패할경우 연결 가능한곳으로 모두 테스트 해보고

    그래서 실패한경우 시스템 메시지를 내보낸다.
*/
extern DWORD    g_dwUSER_ID;	//TODO: 사용하는 곳 확인 필요
extern bool     g_isChangeServer;
extern BOOL		g_isPkServer;

extern float    g_fSpecularVariation;
extern DWORD    m_dwTestMonsterId;

extern ClientScriptParserManager* g_pClientScriptParserManager;

extern DWORD    g_dwCharProcessCount;
extern DWORD    g_dwCharRenderCount;

extern DWORD	g_dwSSQPortalKey;

extern BOOL		g_bObserverMode;
extern BYTE		g_ObserverKey;
extern BOOL		g_IsBattle;
#ifdef _NHN_USA_CHANNELING
extern char*	g_szGameString;
#endif//_NHN_USA_CHANNELING(nhn 퍼플런처로받은 게임스트링)

#ifdef _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
extern WzRenderInfo summon_render_info[2];
#endif //_NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE

namespace clock_function
{   
#if (WZENGINEVER < 300)
    // no operation
#else
    BOOL QueryPerformanceCounter(LARGE_INTEGER *performance_count);
    BOOL QueryPerformanceFrequency(LARGE_INTEGER *performance_frequency);
    DWORD GetTickCount();
    int GetErrorCode();
#endif //(WZENGINEVER)
}

#endif	//	_GLOBALDATA_H

