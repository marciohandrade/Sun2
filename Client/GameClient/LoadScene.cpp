//---------------------------------------------------------------------------
// LoadScene
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "LoadScene.h"
#include "GameFramework.h"
#include "MapInfoParser.h"
#include "heroactioninput.h"
#include "Map.h"
#include "Hero.h"
#include "RadarDialog.h"
#include "RadarPlusDialog.h"
#include "RadarFullDialog.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "interfacemanager.h"

#include "SystemDialog.h"
#include "uiSystemMan/uiSystemMan.h"

#include "Application.h"
#include "uiRadarMan/uiRadarMan.h"
#include "Sound.h"

#include "ItemShareSystem.h"
#include "uiPartyMan/uiPartyMan.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatueInfoParser.h"
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP
#include <wininet.h>
#include <Psapi.h>
#include <TlHelp32.h>

#pragma comment(lib,"psapi.lib")
#pragma comment(lib, "wininet")
#endif //_NA_000000_20140520_MAPLOAD_FAIL_TO_FTP

LoadScene		g_LoadScene;
//----------------------------------------------------------------------------
/**
*/
LoadScene::LoadScene() :
m_fPercentStep (0.005f),
m_iDelayLoadCnt (0)
{
	m_GoingToLobbyMapID = 1;

	for( int i = 0 ; i < IMG_MAX_COUNT ; ++i )
	{
		m_hTexture[i] = NULL;
	}
	
	m_eLoadState = LoadScene_State_NONE;

	m_ppText = NULL;
	m_iMaxTextLine = 0;
	m_bChangeImage = FALSE;

    m_bGoingToMapPos  = FALSE;
    m_bGoingToAreaPos = FALSE;
#ifdef _NHN_USA_CHANNELING	
	m_bPortalFlag	  = FALSE;
	m_nReportZone	  = -1; 
#endif//_NHN_USA_CHANNELING
    m_SceneType = SCENE_TYPE_LOAD;
}

//----------------------------------------------------------------------------
/**
*/
LoadScene::~LoadScene()
{
}


//----------------------------------------------------------------------------
/**
*/
void 
LoadScene::Activate()
{
}
//----------------------------------------------------------------------------
/**
*/
void 
LoadScene::Release()
{
    Sound::Instance()->StopEffectSoundAll();


	ReleaseText();

	//	������ �÷��� ����
	m_bEnableRender = FALSE;


	SceneBase::ReleaseLoadingImg();


	g_pSunRenderer->x_Clock.Reset();

	m_bChangeImage = FALSE;
}


//----------------------------------------------------------------------------
/**
*/
void  LoadScene::IncreaseLoadingProcess()
{
	m_fMaxCount=63.0f;
	SceneBase::IncreaseLoadingProcess();

	if( TRUE == g_pSunRenderer->BeginRender() )
	{
		SceneBase::RenderLoadingImg();

		SceneBase::RenderLoadingGage();
		g_pSunRenderer->x_pManagerPostEffect->Apply();
	}
	g_pSunRenderer->EndRender();
	 
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
LoadScene::Init()
{
	CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Waiting );

	StartFadeIn();

	m_dwCurFadeTime=0;
	m_fLoadingCount=0.0f;
	m_byLoadStart = 0;
	m_iDelayLoadCnt = 0;
	m_eLoadState = LoadScene_State_NONE;

	//	�۷ο� ����
	if (g_pMap)	
	{
		g_pMap->EnableBlur( FALSE );
	}

	//	�������̽� ������ ����
	g_InterfaceManager.SetRenderState( FALSE );


	//	�ε� ��� ȭ�� �̹��� �ε�
	SceneBase::LoadingImg( m_bChangeImage, m_iForceIndex );
	m_iForceIndex = -1;


	m_bChangeImage = FALSE;

	//. �ε� �ϱ� ���� ȭ�鿡 �׷��ش�.
	if(TRUE == g_pSunRenderer->BeginRender())
	{
		SceneBase::RenderLoadingImg();
		g_pSunRenderer->x_pManagerPostEffect->Apply();
	}
	g_pSunRenderer->EndRender();




	IncreaseLoadingProcess();

	m_eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();

	
	m_GoingToCreateHero.m_byClassCode = eCHAR_BERSERKER;


	//	�ڵ� ���ݰ� �ڵ� �̵� ��� ��
	g_HeroInput.SetAutoAttack( FALSE );
	g_HeroInput.SetAutoRun( FALSE );
	g_HeroInput.SetCurrentTarget( 0 );

	IncreaseLoadingProcess();

	InitText();

	IncreaseLoadingProcess();

	IncreaseLoadingProcess();
	m_bFristRender=TRUE;
	//m_bLoad=FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
LoadScene::Process( DWORD dwTick )
{
	ProcessFade(dwTick);

    // ���̵����� ������
    if( m_dwCurFadeTime < 1000.0f )
    {
        return TRUE;
    }

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    //#ifdef _KIKI_AUTOTEST

    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
    {
        if( m_bLoadMap == FALSE && m_GoingToMapID != -1 )
        {
            if( m_eNetxtSceneAfterLoaded == SCENE_TYPE_LOGIN ||
                m_eNetxtSceneAfterLoaded == SCENE_TYPE_CHARSELECT )
            {
                m_GoingToMapID = -1;
                m_bLoadMap = TRUE;

                m_eLoadState = LoadScene_State_Completed;
            }
        }
    }
    //#endif
#endif

    if( m_bLoadMap == FALSE && m_GoingToMapID != -1 )
    {
        m_eLoadState = LoadScene_State_Start;
		xxWriteSceneLog("LoadScene::LoadMap Start");
        LoadMap();
		xxWriteSceneLog("LoadScene::LoadMap Success");
        IncreaseLoadingProcess();
        m_eLoadState = LoadScene_State_Completed;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		LoadStatue();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

        m_bLoadMap = TRUE;
    }
    
    if( m_bLoadMap = TRUE )
    {
	    GameFramework::ChangeScene( m_eNetxtSceneAfterLoaded );
    }

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
LoadScene::Render( DWORD dwTick )
{
	SceneBase::RenderLoadingImg();

	g_pSunRenderer->SetAlphaBlendMode( m_eAlphablendMode );


	RenderLoadingText();


	// �ϴ� ���� �κп��� �ε��ϴ� ������ 
	// ��� �ε�ȭ���� ���� ���»��¿��� �ε��� �ؾ��ϱ� ������
	// �������� �صа��̴�.

	
	++m_iDelayLoadCnt;

	if( LoadScene_State_NONE == m_eLoadState && m_iDelayLoadCnt  > 3 )
	{
		m_iDelayLoadCnt = 0;
	
	}	


	SceneBase::RenderLoadingGage();


	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
void 
LoadScene::RenderText()
{
	// �غ� ����
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	// FPS
	DWORD dwTick;
	LONG lCount;
	g_pSunRenderer->GetFpsValues( &dwTick, &lCount );
	float fFps = ( float )lCount * 1000.0f / ( float )dwTick;

	TCHAR lpszText[256] = {0,};
	Snprintf( lpszText, 256-1, _T("%.2f fps"), fFps );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, 5, 5, WzColor_RGB( 255, 255, 255 ) );


	// ������
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}


void LoadScene::ParsePacket( MSG_BASE * pMsg )
{
    // [6/23/2010 iforall]
    // LoadScene ���� ��Ŷ�� ó���Ǹ� �ȵ�
    assert(!"Invalid Function Call - LoadScene::ParsePacket()");
    switch( pMsg->m_byCategory )
	{

	case CG_CHARINFO:
		{
			OnRecvCG_CHARINFO( pMsg );
			return;
		}
		break;
	case CG_MAP:
		{
			OnRecvCG_MAP( pMsg );
			return;
		}
		break;
	case CG_SYNC:
		{
			OnRecvCG_SYNC( pMsg );
			return;
		}
		break;


	case CG_CONVERSATION:
		{
			OnRecvCG_CONVERSATION( pMsg );
			return;
		}
		break;
	}

	SceneBase::ParsePacket(pMsg);
}

void LoadScene::OnRecvCG_CONNECTION( MSG_BASE * pMsg )
{
}

void LoadScene::OnRecvCG_CHARINFO( MSG_BASE * pMsg )
{
}

void LoadScene::OnRecvCG_MAP( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	case 0:
	default:
		break;
	}
}

void LoadScene::OnRecvCG_SYNC( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	case 0:
		break;

	default: 
		break;
	}
}

void LoadScene::OnRecvCG_CONVERSATION( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	case CG_CONVERSATION_FIELDCHAT_ACK:
		break;
	case CG_CONVERSATION_FIELDCHAT_BRD:
		break;
	}
}

void LoadScene::LoadMap()
{
	//-------------------------------------------------------------------------------
	//		1. ���� �б�
	//-------------------------------------------------------------------------------

	DWORD dwStartTime = GlobalFunc::StartTimeCheck();
	GlobalFunc::WriteLoadingTimeLog(_T("MapLoding Start"));

	IncreaseLoadingProcess();

	if ( g_pMap )
	{
        static bool flag = false;
        if( flag == true )
        {
		    g_pMap->Destroy();
        }
    }

	char szRootDir[0xff];

	::GetCurrentDirectoryA(0xff, szRootDir); 

	IncreaseLoadingProcess();

	g_pMap->Create( g_pSunRenderer, szRootDir );
	IncreaseLoadingProcess();

#ifdef _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP
    if (g_pMap->Load(GetGoingToMapID(), GetGoingToFieldID()) == FALSE)
    {
        // �����޼��� FTP �߼�
#ifdef _DEV_VER
#else
        SendMapLoadFailMessage();
#endif //_DEV_VER
        
        ::MessageBox(NULL, _T("Unknown Error : 0x0436"), _T("Load Fail"), MB_OK);

        GlobalFunc::LogOutGame();
    }
#else
    g_pMap->Load(GetGoingToMapID(), GetGoingToFieldID());
#endif //_NA_000000_20140520_MAPLOAD_FAIL_TO_FTP

	g_pMap->EnableBlur(FALSE);

	// ���̴� �� �ʱ�ȭ ����
    IncreaseLoadingProcess();
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ui_radar_manager->ChangeRadarMap();
    }

	IncreaseLoadingProcess();

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("MapLoding End"));
	GlobalFunc::PrintTotalLoadingTime();

}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
void LoadScene::LoadStatue()
{
	const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(m_GoingToMapID);

	if( !pMapInfo )
	{
		assert(pMapInfo);
		return;
	}

	std::list<BASE_StatueInfo*> statue_info_list;
	std::list<BASE_StatueInfo*>::iterator iter;
	int statue_count = 0;

	if ( true == StatueInfoParser::Instance()->get_statue_info_list(pMapInfo->MapCode, &statue_info_list))
	{
		for ( iter = statue_info_list.begin(); iter != statue_info_list.end(); ++iter)
		{
			uiSUNRankingMan* sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
			BASE_StatueInfo* statue_info_ptr = (*iter);
			if ( statue_info_ptr->is_create_ == 1 && 
				 sun_ranking_manager && sun_ranking_manager->IsStatueInfo((eSTATUE_INDEX)statue_count) == true )
			{
				StatueInfo statue_info;
				sun_ranking_manager->GetStatueInfo((eSTATUE_INDEX)statue_count, statue_info);
				GameFunc::MakeStatuePlayer(statue_info_ptr->position_, statue_info_ptr->direction_, statue_info_ptr->scale_value_, statue_info);
			}

			++statue_count;
		}
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

void LoadScene::SetGoingToMovePos(const WzVector * pWzVector)
{
	m_WZVGoingToMapPos = *pWzVector;
    m_bGoingToMapPos = TRUE;
}

void LoadScene::UpdateGoingToMovePos()
{
    if (g_pHero && m_bGoingToMapPos)
    {
        g_pHero->SetPosition(m_WZVGoingToMapPos);
        m_bGoingToMapPos = FALSE;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
LoadScene::SetGoingToAreaPos(DWORD dwAreaID)
{
    m_bGoingToAreaPos = TRUE;
    m_GoingToAreaCode = dwAreaID;
}

//------------------------------------------------------------------------------
/**
*/
void
LoadScene::UpdateGoingToAreaPos()
{
    if (m_bGoingToAreaPos && g_pMap && g_pHero)
    {
        WzSpecialArea *pAreaInfo = g_pMap->GetMapArea( m_GoingToAreaCode );

        if (pAreaInfo)
        {
            WzVector wzPos;
            g_pMap->GetAreaPosition(&wzPos, m_GoingToAreaCode);
            g_pHero->SetPosition(wzPos);
        }

        m_bGoingToAreaPos = FALSE;
    }
}

void LoadScene::SetNextSceneAfterLoaded( MAPID MapID, FIELDID FieldID )
{

    const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(MapID);
    assert(pMapInfo);
    if ( pMapInfo )
    {
        if ( pMapInfo->byMKind == eZONETYPE_VILLAGE)
        {
            SetNextSceneAfterLoaded(SCENE_TYPE_VILLAGE, MapID, FieldID);
        }
        else if( pMapInfo->byMKind == eZONETYPE_DOMINATION_FIELD)
        {
            SetNextSceneAfterLoaded(SCENE_TYPE_DOMINATION, MapID, FieldID);
        }
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        else if( pMapInfo->byMKind == eZONETYPE_SPA_FIELD)
        {
            SetNextSceneAfterLoaded(SCENE_TYPE_SPA, MapID, FieldID);
        }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        else
        {
            SetNextSceneAfterLoaded(SCENE_TYPE_FIELD, MapID, FieldID);
        }

        ChangeImage(TRUE);
    }
}

// FieldID �� 0�̸� ù��°�� �ʵ�� 
void LoadScene::SetNextSceneAfterLoaded(SCENE_TYPE eNextSceneType, MAPID MapID, FIELDID FieldID )
{
    SetGoingToMap( MapID, FieldID );

    m_eNetxtSceneAfterLoaded = eNextSceneType;
}

void LoadScene::SetGoingToMap(MAPID MapID, FIELDID FieldID)
{
#ifdef _NHN_USA_CHANNELING
	 m_nReportZone =  GlobalFunc::CheckMoveToZone(m_GoingToMapID,MapID,m_bPortalFlag);
	 m_bPortalFlag =  FALSE;
#endif//_NHN_USA_CHANNELING	 
	const sMAPINFO * pMapInfo= MapInfoParser::Instance()->FindMapInfo(MapID);
	if(pMapInfo == NULL)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("pMapInfo == NULL in LoadScene::SetGoingToMap - MapID(%d)"), MapID);
	}
	assert(pMapInfo);

    m_GoingToMapID		= MapID;
    if( FieldID == 0 )
    {
        m_GoingToFieldID = pMapInfo->FCode[0];	// ù��° �ʵ�� �ϴ� �Ƚ��س��´�.		
    }
    else
    {
	    sFIELDINFO * pFieldInfo = MapInfoParser::Instance()->GetFieldInfo(FieldID);
	    if(pFieldInfo == NULL)
	    {
		    WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("pFieldInfo == NULL in LoadScene::SetGoingToMap - FieldID(%d)"), FieldID);
	    }
	    assert(pFieldInfo);

        m_GoingToFieldID	= FieldID;
    }

	ItemShareSystem* pItemShare = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
	if(pItemShare)
	{
		pItemShare->SetDisplayDivOpt(true);	// ��Ƽ �й� �޽��� ��� �����ϵ��� ����
	}

    m_bLoadMap = FALSE;
}

MAPID LoadScene::GetGoingToMapID() const
{
	return m_GoingToMapID;
}

MAPID LoadScene::GetGoingToFieldID() const
{
	return m_GoingToFieldID;
}

void LoadScene::InitText()
{

	m_ppText = (TCHAR **)malloc(sizeof(TCHAR *) * MAX_LOADING_TEXT_LINE);

	for (int a = 0; a < MAX_LOADING_TEXT_LINE; a++)
	{
		m_ppText[a] = (TCHAR *)malloc(sizeof(TCHAR) * MAX_MESSAGE_LENGTH);
	}
	
}

void LoadScene::ReleaseText()
{
	if (m_ppText)
	{
		for (int a = 0; a < MAX_LOADING_TEXT_LINE; a++)
		{
			if (m_ppText[a])
			{
				free(m_ppText[a]);
				m_ppText[a] = NULL;
			}
		}
		free(m_ppText);
		m_ppText = NULL;
	}
}

void LoadScene::LoadText()
{
	m_iMaxTextLine = 0;
	
	const sMAPINFO * pMapInfo= MapInfoParser::Instance()->FindMapInfo(m_GoingToMapID);
	assert(pMapInfo);

	if (pMapInfo)
	{
		if (pMapInfo->FCode[0] == m_GoingToFieldID)
		{
			if (pMapInfo->wText1 != 0)
			{
				LoadTextString(pMapInfo->wText1);
			}

			if (pMapInfo->wText2 != 0)
			{
				LoadTextString(pMapInfo->wText2);
			}

			if (pMapInfo->wText3 != 0)
			{
				LoadTextString(pMapInfo->wText3);
			}
		}
	}
}


void 
LoadScene::LoadTextString(int index)
{
	
	const TCHAR seps[] = _T("#");

	TCHAR szBuffer[MAX_MESSAGE_LENGTH];

	ZeroMemory(szBuffer,MAX_MESSAGE_LENGTH);
	g_InterfaceManager.GetInterfaceString(index,szBuffer,MAX_MESSAGE_LENGTH);
	
	TCHAR *token = _tcstok(szBuffer, seps);
	int linecount = 0;
	while( token != NULL )
	{      
		StrnCopy(m_ppText[m_iMaxTextLine],token,MAX_MESSAGE_LENGTH);
		token = _tcstok( NULL, seps );
		m_iMaxTextLine++;
		assert(linecount < MAX_MISSION_AREA_LINE);

		if (linecount >= MAX_MISSION_AREA_LINE)
		{
			break;
		}
	}
}


void
LoadScene::RenderLoadingText()
{
	int LOADING_TEXT_Y_INTERVAL = GRAPHICPARAM->GetLoadingTextCardOut();
	int LOADING_TEXT_WIDTH = 900;

	RECT rc;

	rc.left = GRAPHICPARAM->GetLoadingTextStartPointX();
	rc.right = rc.left + LOADING_TEXT_WIDTH;

	rc.top = GRAPHICPARAM->GetLoadingTextStartPointY();
	rc.bottom = rc.top + LOADING_TEXT_Y_INTERVAL;
	
	if (m_ppText && m_iMaxTextLine > 0)
	{
		BOOL bOld2dMode,bOldRealSize;
		g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
		g_pSunRenderer->Set2DMode( TRUE );

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

		for (int a = 0; a < m_iMaxTextLine; a++)
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText(m_ppText[a] ,&rc, WzColor_RGBA(255,255,255,255),WzColor_RGBA(0,0,0,0),TP_HLEFT,ETS_SHADOW);

			rc.top += LOADING_TEXT_Y_INTERVAL;
			rc.bottom += LOADING_TEXT_Y_INTERVAL;
		}
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		g_pSunRenderer->Set2DMode( bOld2dMode,bOldRealSize );
	}	
}
#ifdef _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP
void LoadScene::SendMapLoadFailMessage()
{
    HINTERNET hInternetSession, hFtpConnection;

    // Connect
    hInternetSession = ::InternetOpen(
        NULL,
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0);

    if (hInternetSession == NULL)
    {
        return;
    }
        
    hFtpConnection = ::InternetConnect(
            hInternetSession,
            "202.14.103.223",
            10021,
            "webzen",
            "webFTP!34",
            INTERNET_SERVICE_FTP,
            0/*INTERNET_FLAG_PASSIVE*/,
            NULL);

    if (hFtpConnection == NULL)
    {
        InternetCloseHandle(hInternetSession);
        return;
    }

    SYSTEMTIME t;
    ::GetLocalTime(&t);

    // Ŭ�� ����
    TCHAR log_file_name[MAX_PATH];
    Snprintf(log_file_name,
        MAX_PATH,
        "[%02d-%02d-%02d]%d.txt",
        t.wHour, t.wMinute, t.wSecond,
        m_GoingToMapID);

    //������ �������!
    FILE* pFile = fopen(log_file_name,"wb");
    if (pFile != NULL)
    {
		
        WriteUserInfo(pFile);
        fclose(pFile);
    }

    // ���� ����
    TCHAR sever_file_path[MAX_PATH];
    Snprintf(
        sever_file_path,
        MAX_PATH,
        "2/.LoadFailMessage/%04d-%02d-%02d/[%02d-%02d-%02d]%d.txt",
        t.wYear, t.wMonth, t.wDay,
        t.wHour, t.wMinute, t.wSecond,
        m_GoingToMapID);


    // Create folder
    LPCTSTR szTempStr = sever_file_path;
    TCHAR szMakeDir[MAX_PATH];
    int nPos;

    while ((szTempStr = _tcsstr( szTempStr + 1, _T("/") )) != NULL)
    {
        nPos = static_cast< int >(szTempStr - sever_file_path + 1);
        StrNCpy(szMakeDir, sever_file_path, nPos);
        szMakeDir[nPos] = 0;

        ::FtpCreateDirectory(hFtpConnection, szMakeDir);
    }

    // Send file
    ::FtpPutFile(
        hFtpConnection,
        log_file_name,
        sever_file_path,
        FTP_TRANSFER_TYPE_BINARY,
        NULL);

    TCHAR current_dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    strcat(current_dir, "\\");
    strcat(current_dir, log_file_name);
    ::DeleteFile(current_dir);

    InternetCloseHandle(hFtpConnection);
    InternetCloseHandle(hInternetSession);

}

void LoadScene::WriteUserInfo(FILE* file_pointer)
{
    if (file_pointer == NULL)
    {
        return;
    }

    //dumpname.log���� ������.
    //���� ���� ���
    /*
    �� �޸�.
    ���� �޸�.
    ���� �޸�.
    IP����
    ���� ��ǻ�� �̸�
    cpu����
    ���μ��� ���� 32,64bit
    �׷���ī��
    */
    long totalmem = 0;
    long virtualmem = 0;
    long physcalmem = 0;
    long UserPersent = 0;
    TCHAR userIPAddr[20];
    memset(userIPAddr,0,sizeof(TCHAR)*20);
    TCHAR userLocalComName[256];
    memset(userLocalComName,0,sizeof(TCHAR)*256);
    TCHAR ProcessorArchitect[256];
    memset(ProcessorArchitect,0,sizeof(TCHAR)*256);
    TCHAR CPUMHz[256];
    memset(CPUMHz,0,sizeof(TCHAR)*256);
    TCHAR CPUCount[256];
    memset(CPUCount,0,sizeof(TCHAR)*256);
    TCHAR CPUModel[256];
    memset(CPUModel,0,sizeof(TCHAR)*256);

    TCHAR VGA[256];

    //�ӽ� ������ ��ȯ
    TCHAR		m_strTemp[256];		// ������ �ӽ� ��ȯ��
    DWORD		m_dwTempSize;		// ������ �ӽ� ��ȯ���� ũ��	
    m_dwTempSize = (DWORD)sizeof(m_strTemp);

    //----------------------------------------------------------------------------------------------
    MEMORYSTATUS memstatus;

    memset(&memstatus,0,sizeof(MEMORYSTATUS));
    memstatus.dwLength = sizeof(memstatus);

    GlobalMemoryStatus(&memstatus);

    //�޹���Ʈ ������ ���
    totalmem = (memstatus.dwTotalPhys / 1024)/1024;
    virtualmem = (memstatus.dwAvailVirtual / 1024)/1024;
    physcalmem = (memstatus.dwAvailPhys / 1024)/1024;
    UserPersent = memstatus.dwMemoryLoad;
    //���� �޸� ��� ��
    //----------------------------------------------------------------------------------------------
    //���� IP���� ����
    bool userIPData = true;
    TCHAR LocalName[256];

    WSADATA userWsaData;
    memset(&userWsaData,0,sizeof(WSADATA));

    if (WSAStartup(MAKEWORD(2,2),&userWsaData) != 0)
    {
        //������ ���� ���ߴٸ� ���� IP������ ���� �Ҽ� ����.
        userIPData = false;
    }

    //���� IP data
    if (userIPData)
    {

        if(gethostname(LocalName,256) != SOCKET_ERROR)
        {
            sprintf(userLocalComName,"%s",LocalName);
            HOSTENT* ptr = gethostbyname(LocalName);
            IN_ADDR addr;

            int i = 0;
            if (ptr != NULL)
            {
                while(ptr->h_addr_list[i] != NULL)
                {
                    memcpy(&addr,ptr->h_addr_list[i],ptr->h_length);
                    sprintf(userIPAddr,"%s",inet_ntoa(addr));

                    i++;

                }
            }
        }
        WSACleanup();
    }
    //���� IP data ���� ���� ����.
    //----------------------------------------------------------------------------------------------
    //���� �ý��� ���� (CPU, window ����)
    //������ ������Ʈ���� ����.
    HKEY hKey;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE ,"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",	0 ,KEY_QUERY_VALUE ,&hKey);
    RegQueryValueEx(hKey, "ProcessorNameString",  NULL, NULL, (LPBYTE)&m_strTemp, &m_dwTempSize);
    sprintf(ProcessorArchitect,"%s",m_strTemp);

    SYSTEM_INFO	sys;
    ZeroMemory(&sys, sizeof(SYSTEM_INFO));
    GetSystemInfo(&sys);
    switch(sys.dwNumberOfProcessors)
    {
    case 1:		sprintf(CPUCount,"�̱� �ھ�"); break;
    case 2:		sprintf(CPUCount,"��� �ھ�"); break;
    case 4:		sprintf(CPUCount,"���� �ھ�"); break;
    case 8:		sprintf(CPUCount,"8 �ھ�"); break;
    default:	sprintf(CPUCount,"%s �ھ�",sys.dwNumberOfProcessors); break;
    }

    long cpu_clock;
    DWORD dwSize = sizeof(long);
    RegQueryValueEx(hKey, TEXT("~MHz"),  NULL, NULL, (LPBYTE)&cpu_clock, &dwSize);
    sprintf(CPUMHz, "%ld MHz", cpu_clock);

    RegQueryValueEx(hKey, "Identifier",  NULL, NULL, (LPBYTE)&m_strTemp, &m_dwTempSize);
    sprintf(CPUModel, " %s", m_strTemp);

    //�������� ����
    RegCloseKey(hKey);

    //���� �ý��� ���� ��
    //----------------------------------------------------------------------------------------------
    //���� �׷��� ī�� ����
    DISPLAY_DEVICE disp;
    ZeroMemory(&disp, sizeof(DISPLAY_DEVICE));
    disp.cb =  sizeof(DISPLAY_DEVICE);

    int nVgaCnt = 0;
    while ( EnumDisplayDevices(NULL, nVgaCnt, &disp, 0) )
    {
        // main�� �ϰ� �� ��� 
        if(disp.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)	
        {
            sprintf(VGA,"%s",disp.DeviceString);
        }

        nVgaCnt++;
    }
    //���� �׷��� ī�� ���� ��
    //----------------------------------------------------------------------------------------------
    //������ ������ ���� �ӽ� ���� �����
    const char KeyEncrypt[21] = "!@#$%^&*())(*&^%$#@!";

    char FileText[256] = {"�޸� ����"};
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"�� �޸𸮷�: %ldMB",totalmem);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"���� �޸𸮷�: %ldMB",physcalmem);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"���� �޸𸮷�: %ldMB",virtualmem);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"�����޸� ��뷮 : %ld%%",UserPersent);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"CPU ����");
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"CPU ��: %s",CPUModel);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"CPU MHz: %s",CPUMHz);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"CPU ����: %s",CPUCount);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"���μ��� ����: %s",ProcessorArchitect);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"IP ����");
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"IP Addr: %s",userIPAddr);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"Local Name: %s",userLocalComName);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"VGA ����");
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    memset(FileText,0,sizeof(char)*256);
    sprintf(FileText,"VGA: %s",VGA);
    UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
    fwrite(FileText,sizeof(char)*256,1,file_pointer);

    PROCESS_MEMORY_COUNTERS memcounters;
    memset(&memcounters,0,sizeof(memcounters));

    //snapshot����
    HANDLE	hSnapShot;
    BOOL	bRet = false;
    hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);

    TCHAR GameName[256];
    memset(GameName,0,sizeof(TCHAR)*256);
#if defined(_DEBUG)
    sprintf(GameName,"%s","GameClient_Debug.exe");
#elif defined(_GAMECLIENT_VER)
    sprintf(GameName,"%s","GameClient.exe");
#else
    sprintf(GameName,"%s","Sungame.exe");
#endif 

    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);

    Process32First (hSnapShot,&pEntry);

    int counter = 0;
    while(1)
    {
        BOOL hRes=Process32Next (hSnapShot,&pEntry);
        if(hRes == FALSE)
            break;

        counter++;
    }

    fwrite(&counter,sizeof(int),1,file_pointer);

    PROCESSENTRY32 pEntry2;
    pEntry2.dwSize = sizeof(pEntry2);
    Process32First (hSnapShot,&pEntry2);

    while(1)
    {
        BOOL hRes=Process32Next (hSnapShot,&pEntry);
        if(hRes == FALSE)
            break;

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"%s",pEntry.szExeFile);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,file_pointer);

        //�̰� �� ���μ���
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pEntry.th32ProcessID);

        memset(GameName,0,sizeof(TCHAR)*256);
        GetProcessMemoryInfo(hProcess,&memcounters,sizeof(memcounters));

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"%s - UseMem %d",pEntry.szExeFile,memcounters.WorkingSetSize);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,file_pointer);

    }
}
void LoadScene::UserEncryptSystem(const char* Key,size_t KeyLength,char* InputData,char* OutputData)
{
    for (size_t i = 0,j = 0 ; i < strlen(InputData)-1  ; ++i,++j)
    {
        OutputData[i] = InputData[i] ^ Key[j];
        if(j >= KeyLength)
        {
            j = 0;
        }
    }
}
#endif //_NA_000000_20140520_MAPLOAD_FAIL_TO_FTP
