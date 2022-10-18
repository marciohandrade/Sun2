/**
 *	@file		GlobalData.cpp
 *	@brief		Global �������� ��� �δ� ��.
 *  @author		Unknown
 *	@version	Unknown
 */

#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "GmManager.h"
#include "KeyQueueManager.h"
#include "SkillQueueManager.h"
#include "ClientScriptParserManager.h"


// �������� ��� ���Ӽ����� ���� �ο��޾� �����س��Ҵ� c_dwUSER_ID�� �̿��ؼ� �������� �޾ƾ� �Ѵ�.
DWORD                   g_dwUSER_ID             = 0;    // Instead of GENERALPARAM->GetUserID()
bool                    g_isChangeServer        = false;
BOOL					g_isPkServer			= TRUE; // pk�� ������ ���� �ΰ� 
HWND					g_hWndMain				= NULL;
HINSTANCE				g_hInstance				= NULL;
bool					g_bExitFlag				= false;

CEffectManager *		g_pEffectManager		= NULL;
CDrawBase *				g_pSunRenderer			= NULL;
CWorldBase *			g_pSunTerrain			= NULL;


// xxx: �������̽� dll(ControlsDll.dll)�� �Ϲ���(implicit) ��ũ��
// win2000���� ���� ���ȴ�. �׷��� �ε��� �����(explicit) ��ũ�� �Ѵ�.
FUNC_DialogManagerWZ	g_pContactDialog_FnPtr	= NULL;

eLOG_LEVEL				g_eLogLevel				= eLOG_LEVEL_NONE;
bool					g_bNetRecvLog			= false;
bool					g_bNetSendLog			= false;
bool                    g_bShowPacket           = false;

Hero *					g_pHero;
Input					g_Input;

Network	*				g_pNetwork				= NULL;
InterfaceManager		g_InterfaceManager;

IDGenerator				g_IDGenerator;

//	������ ���
char					g_szRunPath[MAX_PATH] = { NULL, };
char					g_szDataPath[MAX_PATH] = { NULL, };

WzRay					g_wrMouseRay;
DWORD					g_CurTime	= 0;
Character *				g_pCurrentPickedObject = NULL;

const WzVector c_ZeroVector       = {0.0f,0.0f,0.0f};
const WzVector c_UpVector         = {0.0f,0.0f,1.0f};
const WzVector c_UnitVector       = {1.0f,1.0f,1.0f};

// ���� ���� ǥ�� �÷���

BOOL	g_bShowDebugInfo = FALSE;

DWORD tempId;

float g_fGravity          =0.002f;
float g_fUpperForce       =0.0246f;


float g_fMaxForce         =0.156f;
float g_addGravity        =0.001f;
int   g_GravityMode       =0;

// ���� ���� 
CNSlangTextFilter		g_slangFilter;
CNSlangTextFilter		g_slangNameFilter;

// ���� ��ã�� 
CPathExplorer *			g_pPathExplorer=NULL;

KeyQueueManager			g_KeyQueueManager;
SkillQueueManager		g_SkillQueue;

int						g_TempVariable[MAX_TEMP_VARIABLE];
int                     g_TestSkillIndex=-3;

ClientScriptParserManager* g_pClientScriptParserManager=NULL;

#ifdef _DEV_VER
	CSunProfiler g_Profiler;
	CSunProfiler g_ProcessProfiler;
#endif

float	g_fSpecularVariation;
DWORD	m_dwTestMonsterId = 0;

void InitializeTempVariable()
{
	for (int a = 0; a < MAX_TEMP_VARIABLE; a++)
	{
		g_TempVariable[a] = 0;
	}
}

D3DGAMMARAMP		g_D3dGammaRamp;

BOOL                g_bPlayerAutoMove=FALSE;
DWORD               g_dwCharProcessCount=0;
DWORD               g_dwCharRenderCount=0;

DWORD				g_dwSSQPortalKey = 0;;

BOOL				g_bObserverMode = FALSE;
BYTE				g_ObserverKey	= 0;
BOOL				g_IsBattle		= FALSE;
#ifdef _NHN_USA_CHANNELING
char*				g_szGameString=NULL;
#endif//_NHN_USA_CHANNELING(nhn ���÷�ó�ι��� ���ӽ�Ʈ��)

#ifdef _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
WzRenderInfo summon_render_info[2];
#endif //_NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE

namespace clock_function
{
#if (WZENGINEVER >= 300)
    BOOL QueryPerformanceCounter(LARGE_INTEGER *performance_count)
    {
        if (g_pSunRenderer != NULL)
        {
            return g_pSunRenderer->x_Clock.QueryPerformanceCounterSecure(performance_count);
        }
        return ::QueryPerformanceCounter(performance_count);
    }

    BOOL QueryPerformanceFrequency(LARGE_INTEGER *performance_frequency)
    {
        if (g_pSunRenderer != NULL)
        {
            return g_pSunRenderer->x_Clock.QueryPerformanceFrequencySecure(performance_frequency);
        }
        return ::QueryPerformanceFrequency(performance_frequency);
    }

    DWORD GetTickCount()
    {
        if (g_pSunRenderer != NULL)
        {
            return g_pSunRenderer->x_Clock.GetTickCountSecure();
        }
        return ::GetTickCount();
    }

    int GetErrorCode()
    {
        if (g_pSunRenderer != NULL)
        {
            return g_pSunRenderer->x_Clock.GetErrorCode();
        }
        return 0;
    }
#else
    // no operation

#endif //(WZENGINEVER)
}