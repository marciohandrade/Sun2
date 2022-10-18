/**
 *	@file		GlobalData.h
 *	@brief		Global �������� ��� �δ� ��.
 *  @author		Unknown
 *	@version	Unknown
 */

//----------------------------------------------------------------------------
///** Global �������� ��� �δ� ��.
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

// xxx: �������̽� dll(ControlsDll.dll)�� �Ϲ���(implicit) ��ũ��
// win2000���� ���� ���ȴ�. �׷��� �ε��� �����(explicit) ��ũ�� �Ѵ�.
extern	FUNC_DialogManagerWZ	g_pContactDialog_FnPtr;

extern	Input					g_Input;
extern  Network	*				g_pNetwork;
extern  InterfaceManager		g_InterfaceManager;

extern	WORD					g_wFrameSkipping;

//	������ ���
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
const DWORD CONVENIENCESTORE_CASH_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 3;   //!< �� ĳ�� ĳ����
const DWORD CONVENIENCESTORE_PACKAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 4;   //!< �� ��Ű�� ĳ����
const DWORD CONVENIENCESTORE_MYPAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 5;   //!< �� ������ ĳ����
const DWORD CONVENIENCESTORE_PPCARD_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 6;   //!< �� PPī�� ĳ����
const DWORD CONVENIENCESTORE_MILEAGE_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 7;   //!< �� ���ϸ��� ĳ����
const DWORD OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 8; //!< ���� ��������â ĳ����
const DWORD CONVENIENCESTORE_PREVIEW_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 9;   //!< �� ��Ű�� ������ �̸�����
#ifdef _NA_007086_20140318_MONSTERBOOK
const DWORD MONSTERBOOK_PREVIEW_CHARACTER_KEY = ONLYCLIENT_OBJECT_KEY + 10;   //!< ���͵��� ���Ϳ��� �̸�����
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
const DWORD STATUE_OBJECT_CHARACTER_KEY1 =  ONLYCLIENT_OBJECT_KEY + 11;         //!< ���� ĳ����
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
// 23������ ����� �ּ���.
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

//------------------------------------------------------------------------------
/**
    �������� ��� �������� �ϳ��� ���������� �����ϸ�

    �����Ұ�� ���� �����Ѱ����� ��� �׽�Ʈ �غ���

    �׷��� �����Ѱ�� �ý��� �޽����� ��������.
*/
extern DWORD    g_dwUSER_ID;	//TODO: ����ϴ� �� Ȯ�� �ʿ�
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
#endif//_NHN_USA_CHANNELING(nhn ���÷�ó�ι��� ���ӽ�Ʈ��)

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

