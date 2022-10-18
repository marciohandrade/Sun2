//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "map.h"
#include "MapUnitCacher.h"
#include "MapUnitCacher.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Player.h"
#include "monster.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "Item.h"
#include "npc.h"
#include "MapInfoParser.h"
#include "Application.h"
#include "AppearanceManager.h"
#include "MapResMgr.h"
#include "MapObject.h"
#include "HeroActionInput.h"
#include "SoundEffect.h"
#include "EnvironmentInfoParser.h"
#include "MouseCursor.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "LoadScene.h"
#include "uiSystemMan/uiSystemMan.h"
#include "TerrainObject.h"

#include <NPCInfoParser.h>

#ifdef MAP_EDIT_MODE
	#include "MapEditScene.h"
#endif

#include "SsqScene.h"
#include "AreaTileLoadHelper.h"
#include <TriggerGroupInfo.h>
#include <TriggerInfoManager.h>
#include "OptimizationManager.h"
#include "SoundBGM.h"
#include "Pet.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatuePlayer.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "Trigger.h"

#include "LottoNPC.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "AmbientSoundInfoParser.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"    
#include "OptionDialog.h"
#include "uiMissionObject/uiMissionObject.h"
#include "uiMissionObjectMan/uiMissionObjectMan.h"

#include "DelayLoadManager.h"
#include "Rider.h"

#include "SkillMain.h"

Map *	g_pMap = NULL;


const float c_BaseGrassVisVal = 30.f;
const float c_GrassAlphaWeightVal = 10.f;

BOOL Map::IsOccluded(WzMatrix *pmtViewTransform, const WzBoundingVolume *pBV)
{
	WzVector wvViewPos;
	WzVector wvCenter;
	WzVector wvDiagonal;
	float	 fRadius = 0.f;

	StlVDistObjBuffer_SimpOcc::iterator it_simpocc = m_pWorldBase->GetSortedSimpOcc().begin();
	while(it_simpocc != m_pWorldBase->GetSortedSimpOcc().end())
	{
		cSimpleOcclusion* pOcclusion = (cSimpleOcclusion*)(*it_simpocc).GetObject();
		if(pOcclusion && !pOcclusion->m_bOccluded)
		{
			wvDiagonal = (pBV->m_aabb.m_wvMax - pBV->m_aabb.m_wvMin)*0.5f;
			fRadius    = VectorLength(&wvDiagonal); // 반지름 구하기
			wvCenter   = pBV->m_aabb.m_wvMin + wvDiagonal; // 월드상의 중심값 구하기.
							
			// 오브젝트의 중심을 View 공간으로 변환한다.
			VectorTransformCoord(&wvViewPos, &wvCenter, pmtViewTransform);

			// 검사는 실제 지형 메쉬가 차지하고 있는 경계구를 이용한다.	
			if(pOcclusion->TestIfOccluded(&wvViewPos, fRadius))
				return TRUE;
		}
		++it_simpocc;
	}

	return FALSE;
}


//-------------------------------------------------------------------------------------------
/**
*/
Map::Map(void)
{
	m_bRenderFlag		= TRUE;
	m_pDraw				= NULL;
	m_pWorldBase		= NULL;
	m_pMapUnitCacher	= NULL;

    // initialize
    memset(&m_MapInfo,0,sizeof(m_MapInfo));
	
	m_bUseBlur			= FALSE;

	m_parVisibleObjectList	= NULL;
	m_parVisibleNodeList	= NULL;
	m_parVisibleUnitList	= NULL;
	m_parAlphaVisibleObjectList	= NULL;	
	m_parAlphaVisibleUnitList	= NULL;	
	m_parPossibleVisibleUnitList = NULL;
	m_parPossibleVisibleObjectList = NULL;
	m_pIntersecBoundUnitArray= NULL;
	m_pIntersecBoundMapObjectArray = NULL;

	// 그림자.
	s_hTextureCircleShadow = INVALID_HANDLE_VALUE;
	m_pwsmCircleShadow = NULL;
	m_dwAutoCameraTargetObjectKey = 0;
	

	m_pAutoCameraPlay = NULL;
	m_bUseAutoCamera = FALSE;

	m_bLoaded = FALSE;

	m_pTriggerInfoMan = NULL;

	m_bRenderAreaInfo = FALSE;

	m_iAmbientSoundHandle = -1;

	m_dwSectorSize = 10; 

	m_iShiftMinX = 0;
	m_iShiftMinY = 0;


	m_bApplyAnotherLight = FALSE;	
	m_fTerrainColorRate = 1.0f;


	m_hWeatherEft  = INVALID_HANDLE_VALUE;
	m_wiWeatherEft = INVALID_WZID;
	m_bOriginWeather = FALSE;

	m_bAlphaVisible = TRUE;

	m_hashmapAppearance.clear();

	m_hTargetEffect  = INVALID_HANDLE_VALUE;

	SetTriggerMan(NULL);

	for( int i = 0; i < 360; ++i )
	{
		m_fFieldTable[i] = 0.15f * sinf(float(i) * 0.017453292519943295769f);
	}

	SetbRenderTile(false);
    m_bRenderWireTerrain = false;
}

//-------------------------------------------------------------------------------------------
/**
*/
Map::~Map(void)
{
	
}

//-------------------------------------------------------------------------------------------
/**
*/
void Map::Create( CDrawBase * pDraw, char * pszRootDir )
{
	m_iAmbientSoundHandle = -1;

	StrnCopy( m_szRootDir, pszRootDir, MAX_PATH );
	if ( !m_pMapUnitCacher )
	{
		m_pMapUnitCacher	= new MapUnitCacher;
	}

	m_pMapUnitCacher->Init( pDraw );
	m_pDraw = pDraw;

	//	임시 - 그림자 이미지
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	if (s_hTextureCircleShadow == INVALID_HANDLE_VALUE)
	{
		s_hTextureCircleShadow = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\CircleShadow_32.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
	}
#endif

	m_dwCurrentTrigger = 0;

	m_dwSectorSize = 10; 

	m_bEnableEAX = FALSE;
	m_bRenderFlag = TRUE;

	m_hashmapAppearance.clear();
	m_bExistWAT=FALSE;

	SetTriggerMan(NULL);

    m_bCreated = TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::IsExsistWAT()
{
	return m_bExistWAT;
}

//-------------------------------------------------------------------------------------------
/**
*/
void Map::InitializeNodeLists()
{
	if (!m_parVisibleNodeList)
	{
		m_parVisibleNodeList = GetWorldBase()->x_pOctree->New_ArrayToEnumerate();
	}

	if (!m_parVisibleObjectList)
	{
		m_parVisibleObjectList = new CWzArray<DWORD>;
	}

	if (!m_parVisibleUnitList)
	{
		m_parVisibleUnitList = new CWzArray<DWORD>;
	}

	if (!m_parAlphaVisibleObjectList)
	{
		m_parAlphaVisibleObjectList = new CWzArray<DWORD>;
	}

	if (!m_parAlphaVisibleUnitList)
	{
		m_parAlphaVisibleUnitList = new CWzArray<DWORD>;
	}

	if (!m_parPossibleVisibleObjectList)
	{
		m_parPossibleVisibleObjectList = GetWorldBase()->x_pOctree->New_ArrayToEnumerate();
	}

	if (!m_parPossibleVisibleUnitList)
	{
		m_parPossibleVisibleUnitList = GetWorldBase()->x_pOctree->New_ArrayToEnumerate();
	}

	if (!m_pIntersecBoundUnitArray)
	{
		m_pIntersecBoundUnitArray = GetWorldBase()->x_pOctree->New_ArrayToEnumerate();
	}

	if (!m_pIntersecBoundMapObjectArray)
	{
		m_pIntersecBoundMapObjectArray = GetWorldBase()->x_pOctree->New_ArrayToEnumerate();
	}
	//////////////////////////////////////////////////////////////////////////
}


void Map::RemoveNodeLists()
{
	if (m_parVisibleNodeList)
	{
		m_pWorldBase->x_pOctree->Delete_ArrayToEnumerate(m_parVisibleNodeList);
		m_parVisibleNodeList = NULL;
	}

	if (m_parVisibleObjectList)
	{			
		delete m_parVisibleObjectList;
		m_parVisibleObjectList = NULL;
	}

	if (m_parVisibleUnitList)
	{			 
		delete m_parVisibleUnitList;
		m_parVisibleUnitList = NULL;
	}

	if (m_parAlphaVisibleObjectList)
	{			
		delete m_parAlphaVisibleObjectList;
		m_parAlphaVisibleObjectList = NULL;
	}

	if (m_parAlphaVisibleUnitList)
	{			 
		delete m_parAlphaVisibleUnitList;
		m_parAlphaVisibleUnitList = NULL;
	}

	if (m_parPossibleVisibleObjectList)
	{
		m_pWorldBase->x_pOctree->Delete_ArrayToEnumerate(m_parPossibleVisibleObjectList);
		m_parPossibleVisibleObjectList = NULL;
	}

	if (m_parPossibleVisibleUnitList)
	{
		m_pWorldBase->x_pOctree->Delete_ArrayToEnumerate(m_parPossibleVisibleUnitList);
		m_parPossibleVisibleUnitList = NULL;
	}


	if (m_pIntersecBoundUnitArray)
	{
		m_pWorldBase->x_pOctree->Delete_ArrayToEnumerate(m_pIntersecBoundUnitArray);
		m_pIntersecBoundUnitArray = NULL;
	}

	if (m_pIntersecBoundMapObjectArray)
	{
		m_pWorldBase->x_pOctree->Delete_ArrayToEnumerate(m_pIntersecBoundMapObjectArray);
		m_pIntersecBoundMapObjectArray = NULL;
	}

	if( m_pAutoCameraPlay )
	{
        Func_ContactTerrain( CT_DELETE_AUTOCAMERA, m_pAutoCameraPlay );
		m_pAutoCameraPlay = NULL;
	}
}


void Map::CreateWorldBase()
{
	assert(NULL == m_pWorldBase);

	g_LoadScene.IncreaseLoadingProcess();

    m_pLoadHelper = (CLoadHelper *)Func_ContactTerrain( CT_NEW_MAPLOADER, 0 );

	g_LoadScene.IncreaseLoadingProcess();

    g_pSunTerrain = m_pWorldBase = (CWorldBase *)Func_ContactTerrain( CT_NEW_WORLD, 0 );

	m_pWorldBase->Create();
	
	g_LoadScene.IncreaseLoadingProcess();

	m_pWorldBase->SetDiscAccess( g_pApplication->GetDiscAccess() );
}


void Map::DestroyWorldBase()
{
    Func_ContactTerrain( CT_DELETE_MAPLOADER, m_pLoadHelper );
    m_pLoadHelper = NULL;

	m_pWorldBase->Destroy( g_pSunRenderer );

    Func_ContactTerrain( CT_DELETE_WORLD, m_pWorldBase );
	g_pSunTerrain = m_pWorldBase = NULL;
}



BOOL Map::LoadBaseMapInfo(MAPID MapID, FIELDID FieldID)
{
    g_LoadScene.IncreaseLoadingProcess();

	const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(MapID);

    if( !pMapInfo )
    {
	    assert(pMapInfo);
        return FALSE;
    }

	m_MapInfo.m_pCurFieldInfo = NULL;
	m_MapInfo.m_pCurWMOInfo = NULL;
	m_MapInfo.m_pCurWUTInfo = NULL;
	m_MapInfo.m_iFieldIndex = 0;

	for (int i = 0 ; i < sMAPINFO::MAX_FIELD_NUM; i++)
	{
		if ( pMapInfo->FCode[i] == FieldID)
		{
			m_MapInfo.m_pCurFieldInfo = MapInfoParser::Instance()->GetFieldInfo(FieldID);
			m_MapInfo.m_pCurWMOInfo   = (sWMOINFO*)(const sWMOINFO*)MapInfoParser::Instance()->FindWMOInfo( pMapInfo->FWMOCode[i] );
			m_MapInfo.m_pCurWUTInfo   = (sWUTINFO*)(const sWUTINFO*)MapInfoParser::Instance()->FindWUTInfo(pMapInfo->FWUTCode[i]);
			m_MapInfo.m_iFieldIndex   = i;
			break;
		}
	}

    if( !m_MapInfo.m_pCurFieldInfo )
    {
        assert(m_MapInfo.m_pCurFieldInfo && "해당필드가 없다.!");
        return FALSE;
    }

    g_LoadScene.IncreaseLoadingProcess();

	m_MapInfo.m_MapID = MapID;
	m_MapInfo.m_iFieldID = FieldID;
	m_MapInfo.m_eRuleset = pMapInfo->Ruleset;
    m_MapInfo.m_pMapInfo = pMapInfo;


    ZeroMemory(m_MapInfo.m_szFieldName,sizeof(TCHAR)* INTERFACE_STRING_LENGTH);
    ZeroMemory(m_MapInfo.m_szTriggerGroupName,sizeof(TCHAR)* MAX_TRIGGER_GROUPNAME_LENGTH);
    ZeroMemory(m_MapInfo.m_szFullFileName, sizeof(char)*MAX_PATH);
    ZeroMemory(m_MapInfo.m_szFilePath, sizeof(char)*MAX_PATH);

	int i;
    g_InterfaceManager.GetInterfaceString( pMapInfo->dwNCode, m_MapInfo.m_szFieldName, INTERFACE_STRING_LENGTH );
	StrnCopy( m_MapInfo.m_szTriggerGroupName, pMapInfo->FGCode[i], MAX_TRIGGER_GROUPNAME_LENGTH );
    StrnCopy( m_MapInfo.m_szFullFileName, m_MapInfo.m_pCurFieldInfo->pszPath, MAX_PATH );

    // 멥데이터가 존재하는 풀디렉토리
    int length = strlen(m_MapInfo.m_szFullFileName);
    if( char* pResult = strrchr(m_MapInfo.m_szFullFileName, '\\') )
    {
        length = pResult - m_MapInfo.m_szFullFileName+1;
    }
    StrnCopy(m_MapInfo.m_szFilePath, m_MapInfo.m_szFullFileName, length );


	g_LoadScene.IncreaseLoadingProcess();

#ifdef _DEV_VER
    if (CGeneralGameParam::Instance()->IsShowTrigger())
    {
        TCHAR szMessage[2048] = {0,};
        sprintf(szMessage, "[MapLoading] MapCode(%u), FieldCode(%u)\n", m_MapInfo.m_MapID, m_MapInfo.m_iFieldID);
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, szMessage);
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
    }
#endif

    uiMissionObject* dialog = GET_CAST_DIALOG(uiMissionObject, IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT);
    if (dialog)
    {
        dialog->SetMissionObjectMapCode(m_MapInfo.m_MapID, m_MapInfo.m_iFieldID);
    }
	return TRUE;
}


BOOL Map::LoadMapData(CWzArchive* pMapFileArchive)
{
	//////////////////////////////////////////////////////////////////////////
    // 맵파일을 읽는다

	g_LoadScene.IncreaseLoadingProcess();

#ifdef _NA_000000_20140331_MAPLOADING_MEMORYPOOL
    class AutoPointer
    {
    public:
        BYTE* buffer_;
        int buffer_size_;

        AutoPointer() :buffer_(0), buffer_size_(0) {};

        void alloc(const CHAR* file_name, int _size)
        {
            buffer_size_ = _size;
            buffer_ = static_cast<BYTE*>( \
                SharedMemoryPool::MemoryAllocate(buffer_size_, file_name, buffer_size_));
        };

        ~AutoPointer()
        {
            buffer_size_ = 0;
            SharedMemoryPool::MemoryDeAllocate(buffer_);
        };
    };    

    AutoPointer auto_buffer;
    ;{
        CDiscAccess* disk_access = g_pApplication->GetDiscAccess();
        const CHAR* file_name = m_MapInfo.m_szFullFileName;
        DA_FILE m_dfFile = disk_access->OpenFile( \
            EDI_ACTIVE, file_name, DA_READ, SM_READ, OD_OPEN_EXISTING );
        if( m_dfFile == INVALID_DA_FILE ) {
            WZLOG( WZERR, "CWzArchive::LoadFile() - OpenFile(%s) 실패", file_name );
            return FALSE;
        }

        int buffer_size = disk_access->GetFileSize( m_dfFile );
        auto_buffer.alloc(file_name, buffer_size);
        if (auto_buffer.buffer_ == NULL ||
            pMapFileArchive->LoadFile(file_name, auto_buffer.buffer_, auto_buffer.buffer_size_) == FALSE )
        {
#if defined(_DEV_VER)
            char szError[INTERFACE_STRING_LENGTH];
            sprintf(szError, "Failed LoadFile() : %s", file_name );
            MessageBoxA( g_hWndMain, szError, "ERROR", MB_OK );
#endif
            return FALSE;
        }

        // 파일 읽기
        disk_access->ReadFile( m_dfFile, auto_buffer.buffer_, auto_buffer.buffer_size_ );
        disk_access->CloseFile(m_dfFile);
    };
#else
	pMapFileArchive->SetDiscAccess( g_pApplication->GetDiscAccess() );
    
	if( pMapFileArchive->LoadFile( m_MapInfo.m_szFullFileName ) == FALSE )
	{
		char szError[INTERFACE_STRING_LENGTH];
		sprintf(szError, "Failed LoadFile() : %s", m_MapInfo.m_szFullFileName );
		MessageBoxA( g_hWndMain, szError, "ERROR", MB_OK );
		return FALSE;
	}
#endif //_NA_000000_20140331_MAPLOADING_MEMORYPOOL

	g_LoadScene.IncreaseLoadingProcess();

    //////////////////////////////////////////////////////////////////////////
	// 엔진의 로드
	if( m_pWorldBase->Serialize( pMapFileArchive ) == FALSE )
	{
		pMapFileArchive->Close();
		return FALSE;
	}

    //////////////////////////////////////////////////////////////////////////
    // 맵파일 버전을 읽는다
    m_MapInfo.m_MapFileVersion = pMapFileArchive->GetVersion();

    //////////////////////////////////////////////////////////////////////////
    // 예외처리
	if (m_MapInfo.m_MapID == 11002 && m_MapInfo.m_iFieldID == 11002)
	{
		//캐릭터 선택신의 경우에만 텍셀을 강제시킨다.
		m_pWorldBase->ForceTexCellIdx(TRUE);
	}

	g_LoadScene.IncreaseLoadingProcess();

	if( GetMapFileVersion() > VERSION_WEBZEN_MAP)
	{
		TCHAR szError[INTERFACE_STRING_LENGTH];
		Sprintf(szError, "Invalid Map Version(%s : %d)", m_MapInfo.m_szFullFileName, GetMapFileVersion() );

        uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if(pSystemMan)
            pSystemMan->Sys_VerifyLock(szError, FALSE);

		return FALSE;
	}

	g_LoadScene.IncreaseLoadingProcess();

//    pArchive->SetVersion(m_pMapLoadArchive->GetVersion());

    //////////////////////////////////////////////////////////////////////////
    // WMO 파일 이름을 구한다
    ZeroMemory(m_MapInfo.m_szWMOFileName, MAX_PATH);
    if ( GetMapFileVersion() >= 162 )       // 162 이상 버전에서 WMO 파일을 사용한다
    {
        St_ChunkInfo ci = pMapFileArchive->ReadChunk();
        if ( 0x1792 != ci.m_nID )
        {
            assert(0);
            //return FALSE;
        }
        else
        {
            // ?
            char szBufStr[_MAX_PATH] = { 0, };
            pMapFileArchive->ReadName(szBufStr);

            // WMO 코드가 있는경우 WMOInfo에 설정된 파일명을 사용한다
            const sWMOINFO* pWMOINFO = MapInfoParser::Instance()->FindWMOInfo( GetCurrentWMOCode() );
            if ( pWMOINFO )
            {
                strncpy(m_MapInfo.m_szWMOFileName, pWMOINFO->pszPath, MAX_PATH);
            }
            else
            {
                StrCopy(m_MapInfo.m_szWMOFileName, m_MapInfo.m_szFullFileName);	
                m_MapInfo.m_szWMOFileName[strlen(m_MapInfo.m_szFullFileName)- 3] = 0;
                strcat(m_MapInfo.m_szWMOFileName, "wmo");
            }
        }
    }

#ifdef _NA_000000_20140331_MAPLOADING_MEMORYPOOL
    // 환경 효과음 정보를 로드한다
    LoadAmbientSound(pMapFileArchive);

    g_LoadScene.IncreaseLoadingProcess();    
#endif // _NA_000000_20140331_MAPLOADING_MEMORYPOOL

	return TRUE;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::LoadMapWAT(CAreaTileHelper * pAreaTileLoadHelper)
{
	char szWATFileName[MAX_PATH]= { 0, };

	StrCopy(szWATFileName, m_MapInfo.m_szFullFileName);	
	szWATFileName[strlen(m_MapInfo.m_szFullFileName)- 3] = 0;
	strcat(szWATFileName, "wat");
	
	//CDiscAccess discAcess;//파일이 있나 검사 
	//DA_FILE dfFile=discAcess.OpenFile(EDI_ACTIVE, szWATFileName, DA_READ, SM_READ | SM_WRITE , OD_OPEN_EXISTING);

	//if ( INVALID_DA_FILE == dfFile)
	//{
	//	return FALSE;
	//}

	//discAcess.CloseFile( dfFile);

	return pAreaTileLoadHelper->LoadAreaTile(szWATFileName, g_pApplication->GetDiscAccess() );
}

//-------------------------------------------------------------------------------------------
/**
*/
char * Map::GetFullFileName()
{
	return m_MapInfo.m_szFullFileName;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::LoadMapWMO(CWzArchive* pArchive)
{
    //pArchive->SetVersion(GetMapFileVersion());

    // WMO파일을 사용하는 경우
    if( strlen(m_MapInfo.m_szWMOFileName) > 0 )
    {
        //////////////////////////////////////////////////////////////////////////			
        // 파일을 연다
        pArchive->SetDiscAccess( g_pApplication->GetDiscAccess() );

        if( pArchive->LoadFile( m_MapInfo.m_szWMOFileName ) == FALSE )
        {
            TCHAR szError[INTERFACE_STRING_LENGTH];
            Sprintf(szError, _T("Failed LoadFile() : %s"), m_MapInfo.m_szWMOFileName );

            uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
            if(pSystemMan)
                pSystemMan->Sys_VerifyLock(szError, FALSE);

            return FALSE;
        }

        //////////////////////////////////////////////////////////////////////////			
        // 헤더를 읽는다
        UINT uiHead = 0;
        pArchive->Read(&uiHead, 4);
        WORD wVersion = 0;
        *pArchive >> wVersion;
        pArchive->SetVersion(wVersion);

        if ( pArchive->GetVersion() > 170 )
        {
            WORD wSecondVersion = 0;
            pArchive->Read(&wSecondVersion, sizeof(WORD));
            pArchive->Read(&checksum_wmo_, sizeof(DWORD));

        }
        else
        {
            checksum_wmo_ = 0;
        }

        //////////////////////////////////////////////////////////////////////////			
        // 영역 설정 정보를 맵에 로드한다.
		BOOL bRet = GetWorldBase()->SerializeExtraAreaInfo(pArchive, TRUE);
#ifdef _NA_007863_20141203_VOLUME_LIGHT_SCATTERING
        GetWorldBase()->x_pSkyBox->UpdateSunInfo();
#endif // _NA_007863_20141203_VOLUME_LIGHT_SCATTERING
        assert(bRet);

    } 

    //////////////////////////////////////////////////////////////////////////
    // test map area (??)
    int iNumSpecialArea = m_pWorldBase->GetNumberOfSpecialArea();

    return TRUE;
}

BOOL Map::LoadAmbientSound(CWzArchive* pMapFileArchive)
{
    const char* FilePath = GlobalFunc::ReplaceFileExt( m_MapInfo.m_szFullFileName, "sdt");

    bool bSkip = !m_pWorldBase->x_pTerrainSound2->FileExists(FilePath);

    m_pWorldBase->x_pTerrainSound2->SetFilename(FilePath);

    St_LHFileHeader fhInfo;
	BOOL bVal = m_pWorldBase->x_pTerrainSound2->Serialize(pMapFileArchive, fhInfo, bSkip);

    return TRUE;
}

DWORD MemorySizeCheck2(char * FuncName)
{
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	
	DWORD size = stat.dwTotalPhys-stat.dwAvailPhys;
	
	return size;
}


BOOL Map::LoadGrass(CWzArchive* pArchive)
{
	g_LoadScene.IncreaseLoadingProcess();
	DWORD dwOldSize = MemorySizeCheck2("haha");

	// 풀 유닛 로드
	if( !m_pLoadHelper->LoadGrass(pArchive))
	{
		MessageBoxA( g_hWndMain, "Invalid Map Version", "Error" ,MB_ICONERROR );
		return FALSE;
	}

	g_LoadScene.IncreaseLoadingProcess();
	m_pMapUnitCacher->LoadMapGrassUnit();
	g_LoadScene.IncreaseLoadingProcess();

    if( GetMapFileVersion() > VERSION_WEBZEN_MAP )
    {
        WZLOG( WzLogType::WZERR, "버전이 맞지 않습니다. (MapUnitCacher::LoadMapGrassUnitDraw)" );
        MessageBoxA( g_hWndMain, "버전이 맞지 않습니다. (MapUnitCacher::LoadMapGrassUnitDraw)", "ERROR", MB_OK );
    }
    else
    {
    	m_pMapUnitCacher->LoadMapGrassUnitDraw( m_pWorldBase );
    }

	g_LoadScene.IncreaseLoadingProcess();
	m_pLoadHelper->DeleteGrass();

    DWORD dwNewSize = MemorySizeCheck2("hoho");

	return TRUE;
}

BOOL Map::_loadMapObjectUnit()
{
	sprintf(m_szMapObjectDir, "%s", "Data\\Map\\Object\\");


	WzID wiMapUnitID = 0;
	char pszUnitFileFullPath[0xff];

	// Wzu 읽기
	int iSize = m_pLoadHelper->GetObjWzuCount();
	St_LHObjectWZU* pLHObjectWzu = m_pLoadHelper->GetObjWzu();

	g_LoadScene.IncreaseLoadingProcess();

	for ( int j=0; j < iSize; ++j )
	{
		sprintf( pszUnitFileFullPath, "%s%s", m_szMapObjectDir, pLHObjectWzu[j].m_pszFileName );	

		BOOL bRet = MAPRESMGR()->AddUnit( pLHObjectWzu[j].m_WzID, pszUnitFileFullPath, pLHObjectWzu->m_Attr);
		assert(bRet);
	}

	g_LoadScene.IncreaseLoadingProcess();
	return TRUE;
}

BOOL Map::_loadMapObjectUnitDraw(CWzArchive* pArchive)
{
	m_listMapObjectKey.clear();

	m_hashmapAppearance.clear();

	sprintf(m_szMapObjectDir, "%s\\%s", m_szRootDir, "Data\\Map\\Object\\");

	m_vecDummyObjectPos.clear();
	DWORD	dwDummyDirObject = StrToWzID("LKDM");


	m_vecDummyMonsterPos.clear();
	DWORD	dwDummyMonsterDirObject = StrToWzID("LKMO");


	g_LoadScene.IncreaseLoadingProcess();
	MapWzUnitDraw * pMapUnitDraw = NULL;
	DWORD dwUnitNum = 0;
	DWORD dwObjectKey = 0;
	APPEARANCEINFO appearInfo;
	DWORD wzDrawID = 0;		// 이 ID는 의미없다. 안쓸것이다. 후에 쓸일이 있을지도..

	g_LoadScene.IncreaseLoadingProcess();
	//오브 젝트 리스트 로딩
	int iSize = m_pLoadHelper->GetObjectCount();
	St_LHObject* pLHObject = m_pLoadHelper->GetObject();

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    uiDominationTowerMan* domination_manager_ptr = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (domination_manager_ptr)
    {
        domination_manager_ptr->domination_object_key_relation_clear();
    }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	for ( int i = 0; i < iSize; ++i )
	{
		dwObjectKey =  MAP_OBJECT_KEY + i;	//< UnitDraw 고유 아이디 할당

		memset( &appearInfo, 0, sizeof(appearInfo) );
		appearInfo.ObjectType = APPEARANCEINFO::MAPOBJECT;

		appearInfo.MapObjectID = pLHObject[i].m_WzID;
		appearInfo.wzPos	= pLHObject[i].m_wvPos;
		appearInfo.wzRot	= pLHObject[i].m_wvRot;
		appearInfo.wzScale	= pLHObject[i].m_wvScale;
		appearInfo.wzBV		= pLHObject[i].m_wzBV;
		appearInfo.dwAttr   = pLHObject[i].m_dwAttribute;	
		appearInfo.wiUserID = pLHObject[i].m_wiUser;

        //충돌오브젝트
        if( pArchive->GetVersion() >= 178 && pLHObject[i].m_iCollision)
        {
            m_vecCollisionObj.push_back(dwObjectKey);
        }

		DWORD dwCustom[4];

		for (int a = 0; a < 4; a++)
		{
			dwCustom[a] = 0;
		}

		// 추가 데이터 설정
		dwCustom[0] = pLHObject[i].m_dwParam[0];
		dwCustom[1] = pLHObject[i].m_dwParam[1];
		dwCustom[2] = pLHObject[i].m_dwParam[2];
		dwCustom[3] = pLHObject[i].m_dwParam[3];

		if (appearInfo.dwAttr & 0x01) 
        {
			// 지형 color를 얻어옴.
			float fT = 0.f;
			int iTerrainMeshStanding = GetWorldBase()->PathExp_GetMeshToStand( appearInfo.wzPos, &fT, 5.0f );
			if ( iTerrainMeshStanding < 0 || fT > 5.0f )
			{
				appearInfo.wzColor = WzColor_RGB( 255, 255, 255);
			}
			else
			{
					appearInfo.wzColor = GetWorldBase()->GetTerrainLight( appearInfo.wzPos, iTerrainMeshStanding );
			}
		}
		else
		{
			appearInfo.wzColor = pLHObject[i].m_wcDraw;
		}

		// 설치형 몬스터가 아니면
		// 코드 이상하다.
		{
			MapObject *pMapObject=(MapObject *)g_ObjectManager.AddMapObject(dwObjectKey, MAP_OBJECT, &appearInfo,(OBJECT_SUB_TYPE)dwCustom[eMAPOBJECT_TYPE]);

			if( pMapObject )
			{
				pMapObject->SetID(pLHObject[i].m_iID);//툴에서 설정된 아이디 이다.
			}

			if(dwDummyDirObject == appearInfo.MapObjectID)
			{
				m_vecDummyObjectPos.push_back(appearInfo.wzPos);
			}
			if(dwDummyMonsterDirObject == appearInfo.MapObjectID)
			{
				this->m_vecDummyMonsterPos.push_back(appearInfo.wzPos);
			}
			if (pMapObject)
			{
				for (int a = 0; a < 4; a++)
				{				
					pMapObject->SetCustomAttribute(a,dwCustom[a]);
				}

#ifdef MAP_EDIT_MODE
				if(dwCustom[eMAPOBJECT_TYPE]==OBJECT_SUB_TYPE_TERRAIN)
					g_mapEditScene.AddTerrainObjIndex(dwObjectKey);
#endif 

				if(dwCustom[eMAPOBJECT_TYPE]==OBJECT_SUB_TYPE_TERRAIN)
					g_SsqScene.AddTerrainObjIndex(dwObjectKey);

                if (dwCustom[eMAPOBJECT_TYPE] == OBJECT_SUB_TYPE_SHADOW_OBJECT)
                {
                    pMapObject->set_shadow_object(true);
                }

				if (pMapObject->IsDominationObject())
				{
					uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
					if (pUIMan)
					{
						pUIMan->SetDominationObject(dwObjectKey);
					}
				}
			}
		}

		m_listMapObjectKey.push_back(dwObjectKey);
	}

	g_LoadScene.IncreaseLoadingProcess();

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::LoadOption(CWzArchive* pMapFileArchive)
{
	g_LoadScene.IncreaseLoadingProcess();
	//. 8. 16 일짜 엔진 이후에 무조건 wot 파일 정보를 읽어야 한다. postEffect방식이 변경 되었기 때문.
	return LoadOptionSerialize(pMapFileArchive);
}

BOOL Map::LoadOptionSerialize(CWzArchive* pMapFileArchive)
{
    const char* OptionFileName = GlobalFunc::ReplaceFileExt(m_MapInfo.m_szFullFileName, "wot");

    CWzArchive MapOptionLoadArchive;
	MapOptionLoadArchive.SetDiscAccess( g_pApplication->GetDiscAccess() );

	if (MapOptionLoadArchive.LoadFile( OptionFileName )  == FALSE )
	{
		MapOptionLoadArchive.Close();

#if !defined(_NA_000000_20140331_MAPLOADING_MEMORYPOOL)
		//. 만약에 Wot파일이 없고, Map파일 버젼이 164보다 작음 Map파일 옵션 정보를 읽어온다.
		if( GetMapFileVersion() < 164 )
			return LoadOldCameraExtraInfo(pMapFileArchive);
#endif // _NA_000000_20140331_MAPLOADING_MEMORYPOOL

		//. 그 밖에는 에러 처리.
        uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if(pSystemMan)
		{
#ifdef _DEV_VER
			char szTemp[_MAX_PATH] = {0, };
			_snprintf(szTemp, INTERFACE_STRING_LENGTH, "%s Can Not Open", OptionFileName);
			pSystemMan->Sys_VerifyLock(szTemp, FALSE);
#else	//	_DEV_VER
			pSystemMan->Sys_VerifyLock(_T("Can not open a file to load Map Option."), FALSE);
#endif	//	_DEV_VER

		}

		return FALSE;
	}

	// Load...
	St_LHFileHeader fhInfo;	
	m_pLoadHelper->LoadMapOption(&MapOptionLoadArchive, fhInfo);

	// 맵의 추가 정보 로드
	St_LHMapOption* pMapOption = m_pLoadHelper->GetMapOption();

	//------------------------------------------------------------------
	// LoadHelper를 이용하여 해당 값을 가져온다.

	// 카메라 위치얻기
	m_CameraExtra.WzPos = pMapOption->vPosition;
	m_CameraExtra.WzTargetPos = pMapOption->vLookAt;

	m_CameraExtra.WzPos.y = pMapOption->vPosition.z;
	m_CameraExtra.WzPos.z = pMapOption->vPosition.y;
	m_CameraExtra.WzTargetPos.y = pMapOption->vLookAt.z;
	m_CameraExtra.WzTargetPos.z = pMapOption->vLookAt.y;


	// 공식 설정(사용안함)
    if (pMapOption->wcShadow)
    {
        m_pDraw->SetShadowColor(pMapOption->wcShadow);
    }
	// Projection 설정
	m_fMapProjectionFOV  = pMapOption->fFov;
	m_fMapProjectionNear = pMapOption->fNear;
	m_fMapProjectionFar  = pMapOption->fFar;

	m_fVisibleRange = pMapOption->fVisibleRange;
	m_fAlphaRange   = pMapOption->fAlphaRange;
	m_fVisibleRate  = pMapOption->fVisibleRate;

	// 랜더링 옵션 값 설정하기
	m_fLightmapBrightness = pMapOption->fBrightness; // 맵 밝기 값을 설정한다.
	GetWorldBase()->SetBrightness( m_fLightmapBrightness );

	if( GetMapFileVersion() >= 169 )
	{
		// Bloom 파라미터 정보 가져오기
		St_PE_BLUR BoomInfo;
		ZeroMemory(&BoomInfo, sizeof(BoomInfo));

		BoomInfo.m_iBlurCount		= pMapOption->iBlurCount;
		BoomInfo.m_fMinLuminance	= pMapOption->fMinLuminance;
		BoomInfo.m_fSelectHighLight	= pMapOption->fSelectHighLight;
		BoomInfo.m_wcSelectColor	= pMapOption->wcSelectColor;
		BoomInfo.m_fFinalColorBlend	= pMapOption->fFinalColorBlend;
		BoomInfo.m_fOriginalColorBlend = pMapOption->fOriginalColorBlend;

		GetWorldBase()->SetBloomParameters(m_pDraw, BoomInfo);
 
		m_wiWeatherEft = pMapOption->WeatherEffect;
		if(INVALID_WZID != m_wiWeatherEft)
		{
			WzVector wvPos;
			SetVector(&wvPos, 0.f, 0.f, 0.f);
			m_hWeatherEft = GlobalFunc::CreateEffectBeUpToOption(m_wiWeatherEft,wvPos);

			m_pWorldBase->SetWeatherEffectHandle(m_hWeatherEft);

			m_bOriginWeather = TRUE;
		}
		else
		{
			m_wiWeatherEft = INVALID_WZID;
			m_hWeatherEft  = INVALID_HANDLE_VALUE;
			m_pWorldBase->SetWeatherEffectHandle(m_hWeatherEft);

			m_bOriginWeather = TRUE;
		}
	}

	if( GetMapFileVersion() >= 175 )
	{
		g_pSunRenderer->x_pManagerPostEffect->SetFocusOutRange(pMapOption->fDOF_Rate);
	}

  	if( GetMapFileVersion() >= 176 )
	{
		g_pSunRenderer->x_pManagerPostEffect->SetFocusRange(pMapOption->fDOF_Range);
	}

	MapOptionLoadArchive.Close();
    
	return TRUE;
}

void Map::SetRenderWeather(BOOL bWeather)
{
	if( bWeather == FALSE )
    {
		g_pEffectManager->UnsetOption( EOPT_RENDERWEATHER );
    }
	else
    {
		g_pEffectManager->SetOption( EOPT_RENDERWEATHER );
    }
}

BOOL Map::GetRenderWeather()
{
    return g_pEffectManager->CheckOption( EOPT_RENDERWEATHER );
}

//-------------------------------------------------------------------------------------------
/** 맵이동시 일괄적으로 이펙트를 해제하기때문에 타겟팅이펙트는 Reload되어야한다.
*/
HANDLE	Map::GetTargetingEffect()
{
	return m_hTargetEffect;
}

void Map::UnLoadTargetingEffect()
{
	if(m_hTargetEffect!= INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hTargetEffect);
		m_hTargetEffect = INVALID_HANDLE_VALUE;
	}
}

void Map::LoadTargetingEffect()
{
	UnLoadTargetingEffect();
    
    m_hTargetEffect = g_pEffectManager->CreateEffect(StrToWzID("A643"), c_ZeroVector);
    
    CEffectBody* effect_body = g_pEffectManager->GetEffectPtrHandle(m_hTargetEffect); 

    if (effect_body)
    {
        effect_body->SetShow(FALSE);
    }

}

//-------------------------------------------------------------------------------------------
/**	인던(크리스마스 눈사람 이벤트맵인지체크)
*/
BOOL Map::IsSnowManEventMap()
{
#ifdef _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
    return ((m_MapInfo.m_MapID == 60018) && (m_MapInfo.m_iFieldID == 29));
#else
	return ((m_MapInfo.m_eRuleset.rules & m_MapInfo.m_eRuleset.ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK) && (m_MapInfo.m_eRuleset.rules & m_MapInfo.m_eRuleset.ZONE_RULE_INDUN_SINGLE_PLAYING_MODE));
#endif // _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
}
//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::LoadMapObject(CWzArchive* pArchive)
{
	sprintf(m_szMapObjectDir, "%s\\%s", m_szRootDir, "Data\\Map\\Object\\");	

	BOOL bRet = m_pLoadHelper->LoadObj(pArchive);
	assert(bRet);

	g_LoadScene.IncreaseLoadingProcess();
	_loadMapObjectUnit();
	g_LoadScene.IncreaseLoadingProcess();
	_loadMapObjectUnitDraw(pArchive);
	g_LoadScene.IncreaseLoadingProcess();

	m_pLoadHelper->DeleteObject();

	return TRUE;
}

BOOL Map::PreLoadCurMapUnitResource()
{
	if( m_MapInfo.m_pCurWUTInfo == NULL )
		return FALSE;


    CWzArchive Archive;
	Archive.SetDiscAccess(g_pApplication->GetDiscAccess());
	if( Archive.LoadFile(m_MapInfo.m_pCurWUTInfo->pszPath) == FALSE )
	{
		Archive.Close();  
		return FALSE;
	}

	St_LHFileHeader fhInfo;
	if( m_pLoadHelper->LoadUnit(&Archive, fhInfo) == FALSE )
	{
		Archive.Close();
		return FALSE;
	}

	CWzArray< St_LHUnit* >* pArryUnits = m_pLoadHelper->GetUnits();
	if( pArryUnits == NULL )
	{
		Archive.Close();
		return FALSE;
	}

	std::set<DWORD> setPreLoadNPCCodes;
	setPreLoadNPCCodes.clear();

	int iUnitCnt = pArryUnits->GetCount();
	for(int i = 0; i < iUnitCnt; ++i)
	{
		St_LHUnit* pLHUnit = pArryUnits->Get(i);
		if(pLHUnit)
		{
			DWORD dwNPCCode = pLHUnit->iKeyList[0];
			BASE_NPCINFO* pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
			if ((pBNI != NULL) && 
                (pBNI->m_dwICode != 0) && 
                (pBNI->m_wNpcImageIndex == 0))
			{
				setPreLoadNPCCodes.insert(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
				//ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
			}
		}
	}

	std::set<DWORD>::iterator setItBegin, setItEnd, setIt;
	setItBegin = setPreLoadNPCCodes.begin();
	setItEnd = setPreLoadNPCCodes.end();
	
	for(setIt = setItBegin; setIt != setItEnd; ++setIt)
	{
		ResourceManager::Instance()->PreLoadResource(*setIt);
	}

	m_pLoadHelper->DestroyUnit();
	Archive.Close(); 
	return TRUE;
}

BOOL Map::LoadOldCameraExtraInfo(CWzArchive* pMapFileArchive)
{
	g_LoadScene.IncreaseLoadingProcess();

	m_fMapProjectionFar  = CAMERA_DEFAULT_FAR_CLIP;
	m_fMapProjectionNear = CAMERA_DEFAULT_NEAR_CLIP;
	m_fMapProjectionFOV  = CAMERA_DEFAULT_FOV;

	BOOL bCameraChunk = FALSE;
	while( TRUE )
	{
		m_CurChunkInfo = pMapFileArchive->ReadChunk();
		if( m_CurChunkInfo.m_nID == CHUNK_CAMERA_INFO )
		{
			bCameraChunk = TRUE;
			break;
		}

		pMapFileArchive->SkipCurrentChunk(m_CurChunkInfo);
	}

	assert(bCameraChunk);

	*pMapFileArchive >> m_CameraExtra.WzPos;
	*pMapFileArchive >> m_CameraExtra.WzTargetPos;

	if(pMapFileArchive->GetVersion() < 103)
	{
		return TRUE;
	}

	float af[3];
	WzColor wzCol;
	// 이놈은 뭘까
	*pMapFileArchive>>af[0]>>af[1]>>af[2];
	*pMapFileArchive>>wzCol;	

	if(157 <= pMapFileArchive->GetVersion()) 
    {	
		*pMapFileArchive >> m_fMapProjectionFOV;
		*pMapFileArchive >> m_fMapProjectionNear;		
		*pMapFileArchive >> m_fMapProjectionFar;	
	}

	g_LoadScene.IncreaseLoadingProcess();

	return TRUE;
}

BOOL Map::LoadAutoCamera(CWzArchive* pArchive)
{
	g_LoadScene.IncreaseLoadingProcess();

	// 3. Camera 읽기
    m_pAutoCameraPlay = ( CAutoCameraPlay*)Func_ContactTerrain( CT_NEW_AUTOCAMERA, 0 );
	m_pAutoCameraPlay->Serialize(pArchive);
	
	m_pAutoCameraPlay->SetMoveSpeed( 0.003f );
	if( m_pAutoCameraPlay->IsValidLink() == FALSE )
	{
		//	파괴
        Func_ContactTerrain( CT_DELETE_AUTOCAMERA, m_pAutoCameraPlay );
		m_pAutoCameraPlay = NULL;
	}
	g_LoadScene.IncreaseLoadingProcess();

	return TRUE;
}


BOOL Map::LoadSpeedTree(CWzArchive* pArchive)
{
	UnLoadSpeedTree();

	//////////////////////////////////////////////////////////////////////////
	// SpeedTree
	char szTemp[MAX_PATH];
	g_LoadScene.IncreaseLoadingProcess();

	char szCompositeTextureRT[MAX_PATH], szCompositeShadowRT[MAX_PATH];
	DWORD dwTreeCnt = 0;
	DWORD dwTreeID = 0;

	if( !m_pLoadHelper->LoadSpeedTree( pArchive ) )
	{
		MessageBoxA( g_hWndMain, "Can't Load Speed Tree!!", "Error" ,MB_ICONERROR );
		return FALSE;
	}

	St_LHSpeedTreeHeader * pSTreeHeader = m_pLoadHelper->GetSpeedTreeHeader();

	if (strlen(pSTreeHeader->m_szCompositeTexture) > 0 && strlen(pSTreeHeader->m_szCompositeShadowTexture) > 0)
	{
		sprintf(szCompositeTextureRT, "Data\\Map\\%s", pSTreeHeader->m_szCompositeTexture);
		sprintf(szCompositeShadowRT, "Data\\Map\\%s", pSTreeHeader->m_szCompositeShadowTexture);
		BOOL bRt = m_pDraw->x_pManagerSpeedTree->LoadTexture(szCompositeTextureRT, szCompositeShadowRT);
	}

	g_LoadScene.IncreaseLoadingProcess();

	dwTreeCnt = m_pLoadHelper->GetSpeedTreeFileCount();

	// 트리 파일 로딩
	St_LHSpeedTreeFile * pSTreeFile = m_pLoadHelper->GetSpeedTreeFile();
	for( size_t i = 0; i < dwTreeCnt; ++i )
	{
		sprintf(szTemp,"Data\\Map\\SpeedTree\\%s", pSTreeFile[i].m_pszFileName);

		// 로딩
		if(m_pDraw->x_pManagerSpeedTree->LoadTreeRef(pSTreeFile[i].m_WzID, szTemp) == INVALID_WZID )
		{
			sprintf(szTemp, "Fail to load a speedtree : [%s]", szTemp);
			MessageBoxA(g_hWndMain, szTemp, "Error", MB_ICONERROR);
			return FALSE;
		}
	}

	g_LoadScene.IncreaseLoadingProcess();

	//트리 리스트 로딩
	St_LHSpeedTree *pSTree = m_pLoadHelper->GetSpeedTree();
	size_t nCount = m_pLoadHelper->GetSpeedTreeCount();
	m_dwSpeedTreeIndex = MAP_SPEED_TREE_KEY_START;
	
	WzVector	wvPos, wvScale;
	DWORD		dwAttr;
	HANDLE		hHANDLE = INVALID_HANDLE_VALUE;
	WzColor		color;
	float		fT;
	
	CSpeedTreeDrawInfo * pSpeeedTreeDrawInfo = NULL;
	for(int i = 0; i < nCount; ++i)
	{
		dwTreeID= pSTree[i].m_WzID;
		dwAttr	= pSTree[i].m_dwAttribute;
		wvPos	= pSTree[i].m_wvPos;
		wvScale = pSTree[i].m_wvScale;

		hHANDLE = m_pDraw->x_pManagerSpeedTree->AddTree(
							dwTreeID,
							&wvPos,
							WzColor_RGB(255,255,255),
							&wvScale,
							dwAttr);
		if(INVALID_HANDLE_VALUE == hHANDLE)
		{
			WzIDToString(szTemp,dwTreeID);
			szTemp[4]=NULL;

			char szError[INTERFACE_STRING_LENGTH];
			sprintf( szError, "Invalid Speed Tree. Check ID (%s)", szTemp );
			MessageBoxA(g_hWndMain, szError, "Error", MB_ICONERROR);

			return true;
		}

		pSpeeedTreeDrawInfo = m_pDraw->x_pManagerSpeedTree->GetTreeByHandle( hHANDLE );

		if(NULL == pSpeeedTreeDrawInfo)
			continue;

		// 옥트리 등록
		GetWorldBase()->x_pOctree->AddObject( 
			DTO_OBJECT, ++m_dwSpeedTreeIndex, &pSpeeedTreeDrawInfo->GetWzBoundingVolume());

		SPEED_TREE_INFO * pSpeedTreeInfo = new SPEED_TREE_INFO;
		pSpeedTreeInfo->dwKey	= m_dwSpeedTreeIndex;
		pSpeedTreeInfo->hHandle = hHANDLE;

		m_SpeedTreeMap.insert( SPEED_TREE_PAIR(m_dwSpeedTreeIndex, pSpeedTreeInfo) );

        // 2016.1.28 / naklu / 바닥이 없이 떠 있는 나무도 색상값이 지정되었다면 그 색상으로 적용되도록 수정
        if (eST_COLOR_TYPE_MODIFY == dwAttr)
        {
            color = pSTree[i].m_wcDraw;
        }
        else if(eST_COLOR_TYPE_APPLY_TERRAIN_COLOR == dwAttr)
        {
            int iTerrainMeshStanding = GetWorldBase()->PathExp_GetMeshToStand( wvPos, &fT, 5.0f );
            if ( iTerrainMeshStanding < 0 || fT > 5.0f )
            {
                color = WzColor_RGB( 255, 255, 255);
            }
            else
            {
                color = GetWorldBase()->GetTerrainLight( wvPos, iTerrainMeshStanding );
            }
        }

		pSpeeedTreeDrawInfo->SetColor(color);

	}

	g_LoadScene.IncreaseLoadingProcess();

	m_pLoadHelper->DeleteSpeedTree();

	return TRUE;
}


BOOL Map::LoadPointLight(CWzArchive* pArchive)
{
	g_LoadScene.IncreaseLoadingProcess();

	if(!m_pLoadHelper->LoadLight( pArchive ))
	{
		MessageBoxA(g_hWndMain,"Invalid Light Info!!", "Error", MB_ICONERROR);
		return false;
	}

	GetWorldBase()->x_pManagerLight->ClearAllLight_MapEdit();
	WzVector wvLight1Dir  = GetWorldBase()->GetSunLightDir();
	WzColor  wcLightColor = GetWorldBase()->GetSunLightColor();
	GetWorldBase()->x_pManagerLight->SetDirectionalLight( 0, wcLightColor, wvLight1Dir );

	WORD wCount = (WORD)m_pLoadHelper->GetLightCount();

	St_LHLight *pLHLight = m_pLoadHelper->GetLight();
	for(int i = 0; i < wCount; ++i)
	{
		GetWorldBase()->x_pManagerLight->SetPointLight(
			i + 1,
			pLHLight[i].m_wc,
			pLHLight[i].m_wvPos,
			pLHLight[i].m_byIrradianceRef,
			pLHLight[i].m_fRange,
			pLHLight[i].m_fIntensity);
	}

	m_pLoadHelper->DeleteLight();

	return TRUE;
}

BOOL Map::LoadTrigger()
{
	g_LoadScene.IncreaseLoadingProcess();
	if( GetMapFileVersion() >= 147 )
	{		
		if( m_pTriggerInfoMan == NULL )
		{
            CWzArchive Archive;
			Archive.SetDiscAccess( g_pApplication->GetDiscAccess() );
			Archive.LoadFile( m_MapInfo.m_szWMOFileName );

			UINT uiHead = 0;
			Archive.Read(&uiHead, 4);
			WORD wVersion = 0;
			Archive >> wVersion;
			Archive.SetVersion(wVersion);

			if ( Archive.GetVersion() > 170 )
			{
				WORD wSecondVersion = 0;
				Archive.Read(&wSecondVersion, sizeof(WORD));
				DWORD dwCheckSum = 0;
				Archive.Read(&dwCheckSum, sizeof(DWORD));
			}

			m_pTriggerInfoMan = new TriggerInfoManager;
			m_pTriggerInfoMan->Release();
			m_pTriggerInfoMan->LoadGroupInfos(Archive);

            Archive.Close();
		}
	}

	return TRUE;
}

BOOL Map::Load( MAPID MapID, FIELDID FieldID, BOOL bUseAutoCamera /*= FALSE*/ )
{
    //////////////////////////////////////////////////////////////////////
    // Prepare Load

	m_bApplyAnotherLight = FALSE;

	m_iAmbientSoundHandle = -1;

	g_LoadScene.IncreaseLoadingProcess();

	UnLoad();

	g_LoadScene.IncreaseLoadingProcess();

	SetUseAutoCamera(bUseAutoCamera);

    //////////////////////////////////////////////////////////////////////
    // MapID, FieldID 로부터 맵파일정보를 구성한다
	if ( LoadBaseMapInfo(MapID, FieldID) == FALSE)
	{
		TCHAR szError[INTERFACE_STRING_LENGTH];
		Sprintf(szError, _T("Failed Map Load. Check Map, Field ID(%d, %d)"), MapID, FieldID );
		
        uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if(pSystemMan)
        {
            pSystemMan->Sys_VerifyLock(szError, FALSE);
        }
		return FALSE;
	}


	g_LoadScene.IncreaseLoadingProcess();

    CreateWorldBase();

	g_LoadScene.IncreaseLoadingProcess();

	InitializeNodeLists();

	g_LoadScene.IncreaseLoadingProcess();

	g_pEffectManager->SetWorld(m_pWorldBase);

	g_LoadScene.IncreaseLoadingProcess();


    //////////////////////////////////////////////////////////////////////
    /// Load MAP File
    {
        CWzArchive MapFileArchive;
	    if ( LoadMapData(&MapFileArchive) == FALSE )
	    {
            MapFileArchive.Close();
		    return FALSE;
	    }


        g_LoadScene.IncreaseLoadingProcess();

        if ( !m_pWorldBase->Load3DFiles( m_pDraw, m_MapInfo.m_szFilePath ) )
        {
            assert( !"텍스쳐 로딩 실패" );
            return FALSE;
        }
        g_LoadScene.IncreaseLoadingProcess();

        St_FogParameter ft = m_pDraw->GetFogParameter();
        m_pDraw->SetClearColor(Red_WzColor(ft.m_wcFogColor),Green_WzColor(ft.m_wcFogColor),Blue_WzColor(ft.m_wcFogColor) ,0);

        //////////////////////////////////////////////////////////////////////////
#ifdef _NA_000000_20140331_MAPLOADING_MEMORYPOOL
        // 환경 효과음 정보를 로드하는 것은 LoadMapData안에서 같이 로드 하도록 한다.
        g_LoadScene.IncreaseLoadingProcess();
        //. 8. 16 일짜 엔진 이후에 무조건 wot 파일 정보를 읽어야 한다. postEffect방식이 변경 되었기 때문.
        LoadOptionSerialize(NULL);
#else
        // 환경 효과음 정보를 로드한다
        LoadAmbientSound(&MapFileArchive);

        g_LoadScene.IncreaseLoadingProcess();
        
        LoadOption(&MapFileArchive);
#endif // _NA_000000_20140331_MAPLOADING_MEMORYPOOL

        MapFileArchive.Close();
    }

    //////////////////////////////////////////////////////////////////////
    /// Load WMO File
    {
        CWzArchive WMOArchive;
        WMOArchive.SetVersion(GetMapFileVersion());
        if ( LoadMapWMO(&WMOArchive) == FALSE )
        {
            WMOArchive.Close();
            return FALSE;
        }

        g_LoadScene.IncreaseLoadingProcess();


	    LoadMapObject(&WMOArchive);

	    PreLoadCurMapUnitResource();

	    g_LoadScene.IncreaseLoadingProcess();

	    LoadAutoCamera(&WMOArchive);

	    g_LoadScene.IncreaseLoadingProcess();

	    LoadGrass(&WMOArchive);

	    g_LoadScene.IncreaseLoadingProcess();

	    LoadSpeedTree(&WMOArchive);
	    g_LoadScene.IncreaseLoadingProcess();

	    if ( GetMapFileVersion() < 155)
	    {
		    LoadTrigger();
		    LoadPointLight(&WMOArchive);
	    }
	    else
	    {
		    LoadPointLight(&WMOArchive);
		    LoadTrigger();
	    }
        WMOArchive.Close();
    }

	g_LoadScene.IncreaseLoadingProcess();

    //////////////////////////////////////////////////////////////////////
    /// Load LightMap File
    ApplyLightMap();

	g_LoadScene.IncreaseLoadingProcess();

	m_bLoaded = TRUE;

	g_LoadScene.IncreaseLoadingProcess();


    //////////////////////////////////////////////////////////////////////
    /// Load AreaTile File
    CAreaTileHelper areaTileLoadHelper;

    if( LoadMapWAT(&areaTileLoadHelper) == TRUE )
    {
        AREA_TILE_INFO* pAreaInfo = NULL ;
        areaTileLoadHelper.SetFirst();
        while( pAreaInfo = areaTileLoadHelper.GetNext() )
        {
            if( TerrainObject* pObj=(TerrainObject*)g_ObjectManager.GetTerrainObject(pAreaInfo->iAreaID) )
            {
                for(int i=0;i<pAreaInfo->wNumberOfTiles;i++)
                {
                    pObj->AddTileIndex(pAreaInfo->piTileIndex[i]);
                }
            }
        }

        m_bExistWAT=TRUE;
    }



    //////////////////////////////////////////////////////////////////////
    // Apply & Initialize

    if( m_pDraw->x_pManagerSpeedTree->IsUseSpeedTree())
    {
        //TODO 나무 심하게 흔들리는 버그 관련 테스트
        //m_pDraw->x_pManagerSpeedTree->SetWindStrength( 0.4f );
        m_pDraw->x_pManagerSpeedTree->SetWindStrength( 0.01f );
        m_pDraw->x_pManagerSpeedTree->SetLOD(50.0f,200.0f);
    }

	PlayAmbientSound();

	g_LoadScene.IncreaseLoadingProcess();

	ApplyOverrideEnvironmentSetting();

	g_LoadScene.IncreaseLoadingProcess();

	CalculateSectorInfo();

    g_LoadScene.IncreaseLoadingProcess();

	LoadTargetingEffect();

	return TRUE;
}


void Map::EnableBlur( BOOL bFlag )
{
	m_bUseBlur = bFlag;

	if (m_bUseBlur)
	{
		g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GLOW );
	}
	else
	{
		g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect( PET_GLOW );
	}
}

void Map::UnLoadSpeedTree()
{
	SPEED_TREE_ITR itr =  m_SpeedTreeMap.begin();
	while(itr != m_SpeedTreeMap.end())
	{
		delete (itr->second);

		itr++;
	}

	m_SpeedTreeMap.clear();

	m_RenderSpeedTreeVec.clear();

	m_pDraw->x_pManagerSpeedTree->UnloadAllSpeedTrees();
}

void Map::UnLoad()
{
    m_vecCollisionObj.clear();
        
    g_ObjectManager.DeleteObjectsByType( MAP_OBJECT );

	DetachLightMap();

	StopAmbientSound();

	m_MapInfo.m_pCurFieldInfo = NULL;
	m_MapInfo.m_pCurWMOInfo = NULL;
	m_MapInfo.m_pCurWUTInfo = NULL;

	m_dwCurrentTrigger = 0;

	if(m_pTriggerInfoMan)
	{
		m_pTriggerInfoMan->Release();
		SAFE_DELETE(m_pTriggerInfoMan);
	}

	if (m_pMapUnitCacher)
	{
		m_pMapUnitCacher->UnloadAll();
	}

	UnLoadTargetingEffect();

	if( m_pWorldBase )
	{
		g_pEffectManager->DestroyAllEffect();
		g_pEffectManager->SetWorld(NULL);

        g_pEffectManager->ClearSoundInfos();
        g_pEffectManager->ShrinkMemoryPool();

		RemoveNodeLists();
		UnLoadSpeedTree();
		DestroyWorldBase();
	}

	MAPRESMGR()->RemoveUnitAll();

	ResourceManager::Instance()->FreeMostData();

    //임시로 주석 처리
	//Sound::Instance()->UnloadSounds();

	m_bLoaded = FALSE;
}


void Map::Update( DWORD dwTick )
{
	static float fWind;

	if (m_bLoaded)
    {
        //	멥 오브젝트 처리
		ProcessMapObjects( dwTick );
		ProcessAmbientSound();
        DelayLoadManager::Instance()->Process();
	}
}

#include "Mmsystem.h"
#pragma comment(lib,"Winmm.lib")

void Map::SetMeshDrawMode(ENUM_MESH_DRAW eMeshDrawMode)
{
	if(m_pDraw)
		m_pDraw->SetMeshDrawMode(eMeshDrawMode);
}

void Map::Render(DWORD dwTick)
{
	g_dwCharRenderCount=0;

	if ( !this->IsRender() || !m_bLoaded ) 
	{
		//WzColor wzBackGround = WzColor_RGBA(0,0,0,255);

		//g_pSunRenderer->ClearRender(CR_DEFAULT, &wzBackGround  );
		return;
    }
	
	// 이것은 맵그리기전에 항상 먼저 그린다. 
	GetWorldBase()->x_pManagerLight->EnableAreaLight(TRUE);
	
	SUNPROFILE_BEGIN( _T("UpdateVisibleObject") );
	UpdateVisibleObject();
	SUNPROFILE_END( _T("UpdateVisibleObject") );

	
	SUNPROFILE_BEGIN( _T("SkyBox") );
	m_pWorldBase->x_pSkyBox->HeartBeat(dwTick );	// 일단 이렇게 픽스해둔다.
	m_pWorldBase->x_pSkyBox->Render();			
    g_pSunRenderer->SetColor(255,255,255,255);
	SUNPROFILE_END( _T("SkyBox") );
 
	SUNPROFILE_BEGIN( _T("RenderTerrainMesh") );

    if (GAMEOPTION->GetBlurState() != FALSE)
    {
        m_pWorldBase->RenderGlow(g_pSunRenderer);
    }

    if (m_bRenderWireTerrain == false)
    {
	    rendering_terrain_cnt_ = 
            m_pWorldBase->RenderTerrainMesh( m_pDraw,
                                             m_parVisibleNodeList,
                                             RTM_NORMAL,
                                             (FALSE != GAMEOPTION->IsTileBlending()),
                                             (FALSE != GAMEOPTION->GetMapDistantView()));	
    }
    else
    {
        g_pSunRenderer->SetMeshDrawMode( MD_WIREFRAME);
        rendering_terrain_cnt_ = m_pWorldBase->RenderTerrainMesh( m_pDraw,
            m_parVisibleNodeList,
            RTM_NORMAL,
            (FALSE != GAMEOPTION->IsTileBlending()),
            (FALSE != GAMEOPTION->GetMapDistantView()));	
        g_pSunRenderer->SetMeshDrawMode( MD_POLYGON );
    }

	SUNPROFILE_END( _T("RenderTerrainMesh") );

	if (GENERALPARAM->GetSpecialMode())
	{
		if (m_bRenderAreaInfo)
		{
			RenderAreaInfo();
		}
	}

	g_pSunRenderer->InitAllRenderStates();

	if (g_InterfaceManager.GetShowFlag()) 
    {
		if ( !g_Input.GetInterfaceInputMouseFlag() )
		{
			Object *pPickedObject = g_HeroInput.GetPickedObject();
			if (pPickedObject)
			{
				if (g_HeroInput.GetCurrentTarget() != pPickedObject->GetObjectKey()) 
                {
					WzColor OutlineColor = WzColor_RGBA(235,192,38,100);

					if (pPickedObject->IsKindOfObject(CHARACTER_OBJECT))
					{
						Character *pChr = (Character *)pPickedObject;

						switch( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChr, g_HeroInput.lsForceAttack() ) )
						{
                        case RELATIONKIND::Enemy:
                            if (pChr->IsKindOfObject(MONSTER_OBJECT))
                            {
                                Monster* monster_ptr = static_cast<Monster*>(pPickedObject);
                                BYTE resist_type = monster_ptr->GetMyResist();
                                switch (resist_type)
                                {
                                case NpcElementWeakness::eElementWeakness_Fire:
                                    OutlineColor = WzColor_RGBA(255,29,75,255);
                                    break;
                                case NpcElementWeakness::eElementWeakness_Water:
                                    OutlineColor = WzColor_RGBA(108,165,250,255);
                                    break;
                                case NpcElementWeakness::eElementWeakness_Wind:
                                    OutlineColor = WzColor_RGBA(79,210,226,255);
                                    break;
                                case NpcElementWeakness::eElementWeakness_Earth:
                                    OutlineColor = WzColor_RGBA(252,176,86,255);
                                    break;
                                case NpcElementWeakness::eElementWeakness_Dark:
                                    OutlineColor = WzColor_RGBA(171,153,209,200);
                                    break;
                                //case kElement_Common:  
                                default:
                                    OutlineColor = WzColor_RGBA(189,198,198,255);
                                    break;
                                }
                            }
                            else
                            {
                                OutlineColor = WzColor_RGBA(189,198,198,255);
                            }
                            break;
                        default:
                            OutlineColor = WzColor_RGBA(189,198,198,255);
                            break;
						}
					}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
					BOOL is_unit_lod = TRUE;
					BOOL enable_subtype_lod = FALSE;
					if ( pPickedObject->IsKindOfObject(STATUE_PLAYER_OBJECT))
					{
						// 동상은 LOD 적용하지 않는다.
						is_unit_lod = g_pSunRenderer->x_pManagerUnitLOD->IsUnitLOD();
						g_pSunRenderer->x_pManagerUnitLOD->EnableUnitLOD(FALSE);
						enable_subtype_lod = g_pSunRenderer->x_pManagerUnitLOD->IsUnitSubTypeLOD();
						g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(FALSE);
					}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

                    // 외각선 출력 여부
                    BOOL render_outline = TRUE;
                    if (pPickedObject->IsKindOfObject(MONSTER_OBJECT))
                    {
                        Monster* monster = static_cast<Monster*>(pPickedObject);
                        if (monster && monster->IsShadowSummon())
                        {
                            // 섀도우 소환수는 아웃라인 렌더링 제외
                            render_outline = FALSE;
                        }
                    }
                     
                    if (render_outline == TRUE)
                    {
                        // 외각선 굵기 
                        // 오브젝트와 카메라의 거리에 따라 조절할 필요가 있다!
                        float OutlineWidth = 0.0100f; 
					    pPickedObject->RenderOutline(OutlineColor,OutlineWidth);
                    }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
					if ( pPickedObject->IsKindOfObject(STATUE_PLAYER_OBJECT))
					{
						g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(enable_subtype_lod);
						g_pSunRenderer->x_pManagerUnitLOD->EnableUnitLOD(is_unit_lod);
					}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
				}

				if( GENERALPARAM->GetSpecialMode() )
				{
					if (g_Input.GetState( DIK_B, KS_DOWN ))
					{
						TCHAR lpszText[256] = {0,};

						if (pPickedObject->IsKindOfObject(MONSTER_OBJECT))
						{
							Monster *pMon = (Monster *)pPickedObject;
							Snprintf( lpszText, 256-1, _T("%s 강제랜더링[%d]"),pPickedObject->GetName(), pMon->GetMonsterInfo()->m_MonsterCode);

						}
						else
						{
							Snprintf( lpszText, 256-1, _T("%s 강제랜더링"),pPickedObject->GetName());
						}

						g_InterfaceManager.ShowMessageBox(lpszText);
						pPickedObject->Render();
					
					}
				}
			}
		}	
	}

	// 나무,풀
	SUNPROFILE_BEGIN( _T("RegisterRenderObjects") );
	RenderObjects();	
	m_pWorldBase->RenderWater(g_pSunRenderer);
	SUNPROFILE_END( _T("RegisterRenderObjects") );

	// 움직이는 캐릭터들 출력.
	SUNPROFILE_BEGIN( _T("RegisterRenderUnits") );
    RenderUnits();
	SUNPROFILE_END( _T("RegisterRenderUnits") );
		
	if(GAMEOPTION->GetShadowOn())
	{
		SUNPROFILE_BEGIN( _T("ApplyShadowMap") );
		m_pWorldBase->RenderShadowMap(g_pSunRenderer);
		g_pSunRenderer->x_pManagerShadowMap->Apply();	
		SUNPROFILE_END( _T("ApplyShadowMap") );
	}
	else
	{
		SUNPROFILE_BEGIN( _T("ReservedDrawcall") );
		g_pSunRenderer->FlushReservedDrawCall();
		SUNPROFILE_END( _T("ReservedDrawcall") );
	}

	SUNPROFILE_BEGIN( _T("RenderAlphaObjects") );
	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;	
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);
	ENUM_ALPHABLEND eBlend = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

	RenderAlphaUnits();
	RenderAlphaObjects();

	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetAlphaBlendMode(eBlend);

//  뷰포트를 재설정하지 않으면 구라 Wide모드에서 텍스트가 정상적으로 출력되지 않는다.
	g_pSunRenderer->SetViewport(NULL);
	SUNPROFILE_END( _T("RenderAlphaObjects") );

	if ( GENERALPARAM->GetSpecialMode() && (true == GetbRenderTile()) ) 
	{
		if (GameFramework::GetCurScene() != SCENE_TYPE_LOGIN)
		{
			BOOL bZTest = g_pSunRenderer->IsEnableZTest();
			BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
#ifdef MAP_EDIT_MODE
			g_pSunRenderer->SetZBufferTest(FALSE);
			g_pSunRenderer->SetZBufferWrite(FALSE);
			
#endif 
			m_pDraw->SetMeshDrawMode(MD_WIREFRAME);
			WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
			WzColor color = WzColor_RGBA(128,128,128,255);
			g_pSunRenderer->SetColor(color);
			
			m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer);

#ifdef MAP_EDIT_MODE	
			// no operation
#else
			m_pDraw->SetMeshDrawMode(MD_POLYGON);
#endif			
			m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer, PTA_NO_WALK, NULL, 0, TRUE);

            {
                WzColor old_color = g_pSunRenderer->GetDiffuseColor();
                g_pSunRenderer->SetColor( WzColor_RGBA( 80, 80, 50, 80 ) );
                m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer, PTA_PLAYER_JUMP_TILE, NULL, 0, TRUE);
                g_pSunRenderer->SetColor( old_color );
            }

            {
                WzColor old_color = g_pSunRenderer->GetDiffuseColor();
                g_pSunRenderer->SetColor( WzColor_RGBA( 100, 100, 50, 200 ) );
                m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer, PTA_JUMP_CONTROL_A, NULL, 0, TRUE);
                g_pSunRenderer->SetColor( old_color );
            }

            {
                WzColor old_color = g_pSunRenderer->GetDiffuseColor();
                g_pSunRenderer->SetColor( WzColor_RGBA( 100, 50, 100, 200 ) );
                m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer, PTA_JUMP_CONTROL_B, NULL, 0, TRUE);
                g_pSunRenderer->SetColor( old_color );
            }

			if (g_pHero) 
			{
				int iTile = g_pHero->GetPathExplorer()->GetTile();
				ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();

				if (iTile != -1) 
				{
					color = WzColor_RGBA(0,0,255,255);
					g_pSunRenderer->SetColor(color);
					m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer,0 ,&iTile,1);				
				}

				iTile = m_pWorldBase->PickPathTile(&g_wrMouseRay);
				if(-1 != iTile)
				{
					color = WzColor_RGBA(255,0,0,255);
					g_pSunRenderer->SetColor(color);
					m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer,0 ,&iTile,1);				
				}

				g_pSunRenderer->SetZFunc(eZFunc);
				g_pSunRenderer->SetZBufferTest(bZTest);
				g_pSunRenderer->SetZBufferWrite(bZWrite);
			}
			m_pDraw->SetMeshDrawMode(MD_POLYGON);
			g_pSunRenderer->SetColor(oldcolor);
		}
	}	

#ifdef MAP_EDIT_MODE
	RenderMapObjectInfo();
#endif	
}

// 스피드 트리, 그래스
void Map::RenderObjects(void)
{
	St_Camera *pCameraInfo = m_pDraw->GetCameraInfo();

	const float fVisibleGrassRange = c_BaseGrassVisVal + GAMEOPTION->GetCullLength() * 0.5f;    // 풀이 보이는거리
	const float	fAlphaGrassRange = (c_BaseGrassVisVal + GAMEOPTION->GetCullLength() * 0.5f) + c_GrassAlphaWeightVal;

	const float fVisibleGrassRange2 = fVisibleGrassRange * fVisibleGrassRange; // 풀이 보이는거리
	const float fAlphaGrassRange2 = fAlphaGrassRange * fAlphaGrassRange; // 풀이 알파처리되서 보이는 거리
	const float fVisibleRate = 0.01f;

    grass_cnt_ = 0;

#ifdef _PERF_DEBUG
	return ;
#endif

	int iUnitNum = m_parVisibleObjectList->GetCount();
	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parVisibleObjectList->Get(i);

		if (dwUniqueID >= MAP_SPEED_TREE_KEY_START)
		{				
			SPEED_TREE_ITR itr;
			itr = m_SpeedTreeMap.find(dwUniqueID);
			SPEED_TREE_INFO * pSpeedTreeInfo = (SPEED_TREE_INFO *)itr->second;

			CSpeedTreeDrawInfo * pSpeedTreeDrawInfo = 
				m_pDraw->x_pManagerSpeedTree->GetTreeByHandle(pSpeedTreeInfo->hHandle);

			WzVector vOffset = pCameraInfo->m_wvFrom - pSpeedTreeDrawInfo->GetPos();
			float fRange = VectorLength(&vOffset);
			float fVolume = 0.0f;
			float fBase = fRange * sin(pCameraInfo->m_fFov / 2.0f) * 2.0f;

			WzBoundingVolume Volume = pSpeedTreeDrawInfo->GetWzBoundingVolume();

			switch(Volume.m_eBvType)
			{
			case BVT_AABB:
				{
					float fX = Volume.m_aabb.m_wvMax.x - Volume.m_aabb.m_wvMin.x;
					float fY = Volume.m_aabb.m_wvMax.y - Volume.m_aabb.m_wvMin.y;
					float fZ = Volume.m_aabb.m_wvMax.z - Volume.m_aabb.m_wvMin.z;
					fVolume = max(max(fX,fY),fZ);
				}
				break;
			case BVT_SPHERE:
				{
					fVolume = Volume.m_BvSphere.m_fRadius * 2.0f;
				}				
				break;
			}

			if (fRange > 0.0f) 
			{
				float fRate = fVolume / fBase;
				if (fRate < fVisibleRate)
				{
					continue;
				}
			}	

			m_RenderSpeedTreeVec.push_back(pSpeedTreeInfo->hHandle);
		}
		else
		{			
			MapWzUnitDraw * pMapUnitDraw = m_pMapUnitCacher->GetMapUnitDraw( dwUniqueID );		
			if ( !pMapUnitDraw )
				continue;

            WzVector vOffset = pCameraInfo->m_wvFrom - pMapUnitDraw->pUnitDraw->GetPosition();
            float fRange2 = VectorLength2(&vOffset);
            if (fRange2 > fVisibleGrassRange2) 
            {
                continue;
            }

            WzVector wvRotOld, wvRotWave;
            wvRotOld = pMapUnitDraw->pUnitDraw->GetRotation();

            if( pMapUnitDraw->byAttr & EGDA_WAVE )
            {
                // 흔들기 속성일때는 흔듬.
                wvRotWave = 
                    GetGrassWave( 
                    pMapUnitDraw->pUnitDraw->GetPosition(),
                    wvRotOld, 
                    m_pDraw->x_Clock.GetCurrentTick(),
                    pMapUnitDraw->iPhysicsType );

                pMapUnitDraw->pUnitDraw->SetRotation( wvRotWave.x,wvRotWave.y,wvRotWave.z ,FALSE);
            }
            else if (pMapUnitDraw->iPhysicsType == 1) {
                wvRotWave.x=0.0;
                wvRotWave.y=0.0;
                wvRotWave.z=Math_GetAngleFromVector(&g_pSunRenderer->GetCameraInfo()->m_wvDir);
                pMapUnitDraw->pUnitDraw->SetRotation( wvRotWave.x,wvRotWave.y,wvRotWave.z ,FALSE);
            }

            m_pDraw->EnableAlphaSort( FALSE );	// 속도 향상 기법!
            m_pDraw->SetColor( pMapUnitDraw->wzColor );

            m_pDraw->AnimateUnit( pMapUnitDraw->pUnitDraw );
            m_pDraw->RenderGrass( pMapUnitDraw->pUnitDraw );

            m_pDraw->EnableAlphaSort( TRUE );
            ++grass_cnt_;

			// rotate 복구
			pMapUnitDraw->pUnitDraw->SetRotation( wvRotOld.x, wvRotOld.y,wvRotOld.z,FALSE);
			g_pSunRenderer->SetColor(255,255,255);
		}		
	}

	// speedtree 렌더할수 있도록 던져주기.
	if (m_RenderSpeedTreeVec.size() > 0)
	{
		m_pDraw->SetColor(255,255,255,255);		
		m_pDraw->x_pManagerSpeedTree->Render( &m_RenderSpeedTreeVec[0], m_RenderSpeedTreeVec.size() );

        m_RenderSpeedTreeVec.resize(0); 
	}
}

#ifdef MAP_EDIT_MODE
void Map::RenderMapObjectInfo(void)
{
	DWORD dwObjKey=g_HeroInput.GetEditMapObjKey();
	MapObject *pSeletedObj=NULL;

	int iUnitNum = m_parVisibleUnitList->GetCount();
	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parVisibleUnitList->Get(i);

		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);
		assert(pObject);
		if(!pObject)
			continue;

		if((pObject->GetSCObject().GetObjectType()==MAP_OBJECT))
		{
			MapObject * pMapObject  = static_cast<MapObject *>(pObject);

			if(dwObjKey==pMapObject->GetObjectKey())
			{
				pSeletedObj=pMapObject;
			}
			else
				pMapObject->RenderInfo();
		}
	}

	if(pSeletedObj)
		pSeletedObj->RenderInfo();

	MapObject * pMapObject = GameFunc::GetPickedMapUnitObject(&g_wrMouseRay);

	if(pMapObject)
		pMapObject->RenderFocus();

}
#endif //MAP_EDIT_MODE

//Todo:각 씬에서 관리 하는게 맞을법 한데..
void Map::RenderUnits(void)
{	
    assert(m_parVisibleUnitList);	
    BOOL enable_subtype_lod = TRUE;
    if (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE)
    {
        enable_subtype_lod = FALSE;
#ifdef _NA_000000_20130114_RENEWER_UI
#else //_NA_000000_20130114_RENEWER_UI
        OptionDialog* option_dialog_ptr = 
            GET_CAST_DIALOG(OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG);
        if (option_dialog_ptr != NULL)
        {
            int lod_option = option_dialog_ptr->GetOptionParam()->GetOptionValue( nOptionType::LOD_LEVEL );
            int check_lod_level = GameFunc::CheckLODLevelByPlayerCount(m_iVisPlayerCnt, lod_option);
            GameFunc::ApplyLODDistance(check_lod_level);
        }
#endif //_NA_000000_20130114_RENEWER_UI
    }   
    m_iVisMonCnt = 0;
	m_iNPCCnt = 0;

	int iUnitNum = m_parVisibleUnitList->GetCount();
	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parVisibleUnitList->Get(i);

		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);
		assert(pObject);
		if(!pObject)
			continue;

		if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene() || SCENE_TYPE_SSQ == GameFramework::GetCurScene())
		{
			if(g_pHero != pObject)
			{
				for (size_t i = 0; i < pObject->GetPartCount(); ++i)
				{
					if(NULL != pObject->GetPart(i))
						pObject->GetPart(i)->SetForcedLOD(2);
				}
			}
		}
#ifdef _NA_008334_20150608_SONNENSCHEIN
        else if (SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene())
        {
            if(g_pHero != pObject)
            {
                for (size_t i = 0; i < pObject->GetPartCount(); ++i)
                {
                    //if(NULL != pObject->GetPart(i))
                        //pObject->GetPart(i)->SetForcedLOD(1);
                }
            }
        }
#endif //_NA_008334_20150608_SONNENSCHEIN

		switch(pObject->GetSCObject().GetObjectType() )
		{
            case PLAYER_OBJECT:
			{
				Player * pPlayer = static_cast<Player *>(pObject);
			
				if (pPlayer->GetShow() && pPlayer->IsShowByGameOption()) 
				{
                    g_dwCharRenderCount++;
                    g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(enable_subtype_lod);
                    pPlayer->Render();
                    g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(TRUE);
				}

				if (GENERALPARAM->GetSpecialMode()) 
                {
					if (g_Input.GetState(DIK_V, KS_KEY)) 
                    {
						WzBoundingVolume * pBV = pPlayer->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}						
					}
				}
			}
			break;


		case SSQMONSTER_OBJECT:
			{
				g_dwCharRenderCount++;
				Monster * pMonster = static_cast<Monster *>(pObject);
                pMonster->Render();

				if (GENERALPARAM->GetSpecialMode()) 
				{
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{
						WzBoundingVolume* pBV = pMonster->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}
				}							
			}
			break;


		case MONSTER_OBJECT:
			{
				Monster * pMonster = static_cast<Monster *>(pObject);
				pMonster->Render();
				g_dwCharRenderCount++;
				m_iVisMonCnt++;

				if (GENERALPARAM->GetSpecialMode()) 
                {
					if (g_Input.GetState(DIK_V, KS_KEY)) 
                    {
						WzBoundingVolume * pBV = pMonster->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}

					if (g_Input.GetState(DIK_T, KS_KEY)) 
					{
						WzBoundingVolume * pBV = pMonster->GetBoundingVolume();
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}
				}							
			}
			break;
		case LOTTO_NPC_OBJECT:
		case NPC_OBJECT:
			{	
				NPC * pNPC = static_cast<NPC *>(pObject);
				pNPC->Render();
				m_iNPCCnt++;		

				if (GENERALPARAM->GetSpecialMode()) 
				{
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{
						WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

						g_pSunRenderer->SetColor( 255, 255, 255 );
						WzBoundingVolume * pBV = pNPC->GetPickingBoundingVolume(0);
						if(pBV)
						{
							g_pSunRenderer->RenderBoundingVolume(pBV);
						}
						g_pSunRenderer->SetColor( oldcolor);
						
					}
					if (g_Input.GetState(DIK_T, KS_KEY)) 
					{
						WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
						g_pSunRenderer->SetColor( 255, 255, 255 );
						g_pSunRenderer->RenderBoundingVolume(pNPC->GetBoundingVolume());
						g_pSunRenderer->SetColor( oldcolor);
					}
				}
			}
			break;

		case PET_OBJECT:
			{
				Pet * pPet = static_cast<Pet *>(pObject);
                g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(enable_subtype_lod);
				Player* pPlayer = pPet->GetMasterObject();
				if (pPlayer->IsShowByGameOption())
				{
					pPet->Render();
				}
                g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(TRUE);

				if (GENERALPARAM->GetSpecialMode()) 
				{
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{
						WzBoundingVolume * pBV = pPet->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}

					if (g_Input.GetState(DIK_T, KS_KEY)) 
					{
						WzBoundingVolume * pBV = pPet->GetBoundingVolume();
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}
				}
			}
			break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		case STATUE_PLAYER_OBJECT:
			{
				StatuePlayer* statue_player = static_cast<StatuePlayer*>(pObject);

				BOOL is_unit_lod = TRUE;
				BOOL is_subtype_unit_lod = TRUE;
				
				// 동상은 LOD 적용하지 않는다.
				is_unit_lod = g_pSunRenderer->x_pManagerUnitLOD->IsUnitLOD();
				is_subtype_unit_lod = g_pSunRenderer->x_pManagerUnitLOD->IsUnitSubTypeLOD();
				g_pSunRenderer->x_pManagerUnitLOD->EnableUnitLOD(FALSE);
				g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(enable_subtype_lod);

				statue_player->Render();
				
				g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(is_subtype_unit_lod);
				g_pSunRenderer->x_pManagerUnitLOD->EnableUnitLOD(is_unit_lod);

				if (GENERALPARAM->GetSpecialMode()) 
				{
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{
						WzBoundingVolume * pBV = statue_player->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}

					if (g_Input.GetState(DIK_T, KS_KEY)) 
					{
						WzBoundingVolume * pBV = statue_player->GetBoundingVolume();
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}
				}
			}
			break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

		case MONEY_OBJECT:
		case ITEM_OBJECT:
			{
				Item * pItem = static_cast<Item *>(pObject);			
				pItem->Render();

				if (GENERALPARAM->GetSpecialMode()) 
				{
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{
						WzBoundingVolume * pBV = pItem->GetPickingBoundingVolume(0);
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}

					if (g_Input.GetState(DIK_T, KS_KEY)) 
					{
						WzBoundingVolume * pBV = pItem->GetBoundingVolume();
						if (pBV)
						{
							WzBoundingVolume bv = *pBV;
							g_pSunRenderer->RenderBoundingVolume(&bv);
						}
					}
				}
			}
			break;

		case MAP_OBJECT:
			{
				MapObject * pMapObject  = static_cast<MapObject *>(pObject);
				if(m_pDraw)
				{
					WzVector vDiff = m_pDraw->GetCameraInfo()->m_wvFrom - pObject->GetVisiblePos();
					float    fDiffLength = VectorLength(&vDiff);
					float    fRate = 1.f;

                    // 절두체와 맵오브젝트 바운딩박스 간의 체크로 보이는 맵오브젝트라 판단되었으나
                    // 맵오브젝트의 위치가 far 거리보다 클 경우 다가감에 따라 깜박임이 발생함.
                    // (크기가 큰 맵 오브젝트의 경우 발생하게됨)
                    // 맵오브젝트의 위치가 fat 보다 멀면 그리지 않는 것으로 수정함 [11/17/2010 iforall]
                    if (fDiffLength >= m_fVisibleRange)
                    {
                        continue;
                    }
                    
                    if(m_bAlphaVisible && fDiffLength >= m_fAlphaRange)
					{
						fRate = 1.0f - (fDiffLength - m_fAlphaRange) / (m_fVisibleRange - m_fAlphaRange);
					}
					else
					{
						fRate = 1.f;
					}

                    WzColor color;
                    color = WzColor_RGBA(Red_WzColor(pMapObject->GetColor()),
                        Green_WzColor(pMapObject->GetColor()),
                        Blue_WzColor(pMapObject->GetColor()),
                        (BYTE)(255.0f * fRate));

                    pMapObject->SetColor(color);
					pMapObject->Render();

				}
				else
				{
					assert(!"case MAP_OBJECT::!m_pDraw");
				}

				if (GENERALPARAM->GetSpecialMode()) 
                {
					if (g_Input.GetState(DIK_V, KS_KEY)) 
					{

						WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

						g_pSunRenderer->SetColor( 255, 255, 255 );
						g_pSunRenderer->RenderBoundingVolume(pMapObject->GetBoundingVolume());
						g_pSunRenderer->SetColor( oldcolor);

					}
				}				
			}
			break;

		default:
			{
			}
			break;
		}
	}
}

void Map::Destroy()
{
    m_bCreated = FALSE;

	// 데칼
	if ( m_pwsmCircleShadow )
	{
		g_pSunTerrain->DestroyDecal( m_pwsmCircleShadow );
		m_pwsmCircleShadow = NULL;
	}
	
	UnLoad();

	if (m_pMapUnitCacher)
	{
		delete m_pMapUnitCacher;
		m_pMapUnitCacher = NULL;
	}

    // 그림자 텍스쳐
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	if (s_hTextureCircleShadow != INVALID_HANDLE_VALUE) 
    {
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( s_hTextureCircleShadow );
		s_hTextureCircleShadow = INVALID_HANDLE_VALUE;
	}
#endif

	if(m_pTriggerInfoMan)
	{
		m_pTriggerInfoMan->Release();
		SAFE_DELETE(m_pTriggerInfoMan);
	}
}


int	Map::GetCurrentFieldID() const
{ 
    if( m_MapInfo.m_pCurFieldInfo != NULL )
        return m_MapInfo.m_pCurFieldInfo->FieldCode;

    return -1;
}

int	Map::GetCurrentWMOCode(void) const
{
    if( m_MapInfo.m_pCurWMOInfo != NULL )
        return m_MapInfo.m_pCurWMOInfo->Code;

    return -1;
}

//------------------------------------------------------------------------------
/** 맵충돌체크(미리등록된 오브젝트리스트만돈다)
*/
BOOL Map::GetCollisionBV(WzRay *pRay,float fDist,float *pfT)
{
    if(m_vecCollisionObj.empty()) 
    {
        return FALSE;
    }

    for(unsigned int i=0;i<m_vecCollisionObj.size();++i)
    {
        MapObject* pObj= (MapObject*)g_ObjectManager.GetObject(m_vecCollisionObj[i]);

        if(!pObj) continue;

        WORD wResult = Intersect_Ray_Bv( pRay, pObj->GetBoundingVolume(), pfT);

        if(wResult!=NTCT_OUTSIDE && *pfT < fDist)
        {
            return TRUE;
        }
    }
    return FALSE;
}



void Map::UpdateVisibleObject()
{
	m_parPossibleVisibleUnitList->InvalidateAll();
	m_parPossibleVisibleObjectList->InvalidateAll();
	m_parVisibleNodeList->InvalidateAll();
	m_parVisibleObjectList->InvalidateAll();
	m_parVisibleUnitList->InvalidateAll();
	m_parAlphaVisibleUnitList->InvalidateAll();
	m_parAlphaVisibleObjectList->InvalidateAll();

    m_iVisPlayerCnt = 0;

	SUNPROFILE_BEGIN( _T("Enumerate VisibleObject") );
	GetWorldBase()->x_pOctree->Enumerate_IntersectFrustum( DTO_TO_GET_OCTNODES, &m_pDraw->GetCameraInfo()->m_wfFrustum, m_parVisibleNodeList );

	int iNodeCount = m_parVisibleNodeList->GetCount();
	for (int i = 0; i < iNodeCount; i++)
	{
		WzOctIndex woi = m_parVisibleNodeList->Get(i);

		COctNode *pNode = m_pWorldBase->x_pOctree->GetOctNode(woi);
		if (pNode)
		{			
			pNode->GetObjectList(DTO_UNIT,m_parPossibleVisibleUnitList);
			pNode->GetObjectList(DTO_OBJECT,m_parPossibleVisibleObjectList);
		}
	}
	SUNPROFILE_END( _T("Enumerate VisibleObject") );	

	SUNPROFILE_BEGIN( _T("Calculate VisibleObject") );
	CalculateVisibleUnit();
	SUNPROFILE_END( _T("Calculate VisibleObject") );
}

Character *Map::GetPickedCharacter(WzRay* pRay,DWORD dwPickingOption)
{
	// TODO:
	// 나중엔 구분없이 오브젝트로 리턴할수있게
	// 바운딩 볼륨관련 사항을 오브젝트로 옮기자.

	m_pIntersecBoundUnitArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(DTO_UNIT, pRay, m_pIntersecBoundUnitArray);

	float fT = 0.f;
	Object* pTargetObject = NULL;
	Character* pTargetChar = NULL;
	Character* pSelectChar = NULL;

#ifdef _NA_000000_20140423_SUMMON_BACKGROUND_PICKING
    Character* select_character_is_summon = NULL;   // 가장 가까운 소환수
    float select_distance_is_summon = 1000.f;		// 가장 가까운 소환수의 거리
#endif //_NA_000000_20140423_SUMMON_BACKGROUND_PICKING

	DWORD dwKey = 0;
	int iSize = m_pIntersecBoundUnitArray->GetCount();
	
	BOOL bSkipDeadCharacter = (dwPickingOption & PICKING_IGNORE_CORPSE);	// 죽은 캐릭터 빼기 옵션
	BOOL bSkipFriend = (dwPickingOption & PICKING_IGNORE_FRIEND);     
	float fSelectDist = 1000.f;		// 현재 선택된 캐릭터와의 거리: 기본값은 max로 설정

    // 죽은 캐릭터를 타겟할수 있는가 (쉐도우가, 소울 컨트롤을 배웠을 경우)
    bool can_target_death_monster = false;

    if( g_pHero != NULL )
    {
        if( g_pHero->GetClass() == eCHAR_SHADOW )
        {
            if( SkillData* pSkillData = xGetSkillData()->GetSkillDataBySkillClassCode( eSKILL_SOUL_CONTROL ) )
            {
                if( pSkillData->IsActive() == true )
                {
                    can_target_death_monster = true;
                }
            }
        }
    }


	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundUnitArray->Get(i);
		pTargetObject = g_ObjectManager.GetObject(dwKey);

		if (!pTargetObject)
			continue;

		if (pTargetObject->IsKindOfObject(CHARACTER_OBJECT) ) 
		{
			pTargetChar = (Character*)pTargetObject;

			if (pTargetChar == g_pHero)
			{
				continue;
			}

			if (!pTargetChar->GetShow())
			{
				continue;
			}

			if (!pTargetObject->IsKindOfObject(PLAYER_OBJECT))
			{
				if (bSkipDeadCharacter && pTargetChar->IsDead())
				{
                    if( can_target_death_monster == false )
                    {
                        continue;
                    }
                    
				 //   // 죽은 캐릭터는 뺀다.(섀도우는 면제)						
					//if (!g_pHero || (g_pHero->GetClass() != eCHAR_SHADOW))
					//{
					//	continue;
					//}
					Monster* pMon = (Monster*)pTargetObject;
					if (eNPC_DOMINATION_MAPOBJECT_NPC == pMon->GetMonsterInfo()->m_byGrade)
                    {
						continue;
                    }
				}
			}

			if (pTargetChar->IsKindOfObject(PLAYER_OBJECT) && false == ((Player*)pTargetObject)->IsShowByGameOption())
			{
				continue;
			}

			if (bSkipFriend) 
            {
				if (g_pHero)
				{
					if( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTargetChar, g_HeroInput.lsForceAttack() ) == RELATIONKIND::Friend )
					{
						continue;
					}
				}
			}

			WORD wResult = NTCT_OUTSIDE;

			if (pTargetChar->GetNumPickingBoundingVolume())
			{
				WzBoundingVolume bv = *pTargetChar->GetPickingBoundingVolume(0);
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}
			else
			{
				WzBoundingVolume bv = *pTargetChar->GetBoundingVolume();
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}

			if (NTCT_OUTSIDE == wResult )  
			{
				pTargetChar = NULL;
				continue;
			}
			else
			{		

#ifdef _NA_000000_20140423_SUMMON_BACKGROUND_PICKING
                if (pTargetChar->IsKindOfObject(MONSTER_OBJECT))
                {
                    Monster* monster = static_cast<Monster*>(pTargetChar);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    if (monster && monster->IsWitchBladeSummon() == TRUE)
                    {
                        continue;
                    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    if (monster && monster->IsShadowSummon() == TRUE)
                    {
                        if (fT < select_distance_is_summon)
                        {
                            // 가장 가까운 소환수
                            select_character_is_summon = pTargetChar;
                            select_distance_is_summon = fT;

                            continue;
                        }
                        continue;
                    }
                }
#endif //_NA_000000_20140423_SUMMON_BACKGROUND_PICKING
                // 이전에 선택한 캐릭터보다 가까운 캐릭터를 선택한다.
                if (fT < fSelectDist)
                {
                    // 최종적으로 선택된 경우
                    pSelectChar = pTargetChar;
                    fSelectDist = fT;
                }
			}
		}
	}

#ifdef _NA_000000_20140423_SUMMON_BACKGROUND_PICKING
    if ((pSelectChar == NULL) &&
        (select_character_is_summon != NULL))
    {
        // 소환수만 있다면 소환수를 리턴
        return select_character_is_summon;
    }
#endif //_NA_000000_20140423_SUMMON_BACKGROUND_PICKING

	return pSelectChar;
}

Item *Map::GetPickedItem(WzRay* pRay)
{
	m_pIntersecBoundUnitArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(DTO_UNIT, pRay, m_pIntersecBoundUnitArray);

	float fT = 0.f;
	Object * pTargetObject = NULL;
	Item   * pItem = NULL;
	Item   * pTempItem = NULL;
	DWORD dwKey = 0;
	int iSize = m_pIntersecBoundUnitArray->GetCount();
	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundUnitArray->Get(i);
		pTargetObject = g_ObjectManager.GetObject(dwKey);
		assert(pTargetObject);
		if(!pTargetObject)
			continue;

		if (pTargetObject->IsKindOfObject(ITEM_OBJECT) ) {

            pTempItem = (Item *)pTargetObject;
			if (pTempItem->IsHideItem())
			{
				pTargetObject = NULL;
				continue;
			}

			if (pTempItem->IsOpened())
			{
				pTargetObject = NULL;
				continue;
			}

			WzBoundingVolume bv = *pTargetObject->GetBoundingVolume();

			if (bv.m_eBvType = BVT_SPHERE) 
            {
				// 아이템 충돌 범위 1.0으로 고정
				bv.m_BvSphere.m_fRadius = 1.0f;
			}

			if (NTCT_OUTSIDE == Intersect_Ray_Bv( pRay, &bv, &fT ) )  
			{
				pTargetObject = NULL;
				continue;
			}
			else
			{
				// 처음찾은놈을 리턴해야지.
				pItem = (Item *)pTargetObject;
				break;
			}
		}
	}

	return pItem;
}


MapObject * Map::GetPickedUnitMapObject(WzRay * pRay, int iID /*= -1*/)
{
	m_pIntersecBoundUnitArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(DTO_UNIT, pRay, m_pIntersecBoundUnitArray);

	float fT = 0.f;
	Object * pTargetObject = NULL;
	MapObject * pMapObject = NULL;
	MapObject * pTempMapObject = NULL;
	MapObject * pSelectChar = NULL;
	DWORD dwKey = 0;
	float fSelectDist = 1000.f;		// 현재 선택된 캐릭터와의 거리: 기본값은 max로 설정

	int iSize = m_pIntersecBoundUnitArray->GetCount();
	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundUnitArray->Get(i);
		pTargetObject = g_ObjectManager.GetObject(dwKey);
		assert(pTargetObject);

		if( pTargetObject == NULL )
			continue;
		if( pTargetObject->IsEnableDisplay() == FALSE )
			continue;

		if ( pTargetObject->IsKindOfObject(MAP_OBJECT) ) 
		{
#ifdef __NA_0_20080627_MAPOBJECT_PICKING_CHANGE
  #ifdef MAP_EDIT_MODE
			// no operation
  #else
			MapObject * pMapObject = (MapObject * )pTargetObject;
			if( !pMapObject->IsPickable())
				continue;

  #endif//MAP_EDIT_MODE
#endif// __NA_0_20080627_MAPOBJECT_PICKING_CHANGE

			WORD wResult = NTCT_OUTSIDE;

			for( size_t j = 0; j < pTargetObject->GetNumPickingBoundingVolume(); j++)
			{
				WzBVExtraInfo* pBvExtraInfo = pTargetObject->GetBasePart()->GetBoundingVolumeExtraPtrByIndex( i );

				if(!pBvExtraInfo)
					continue;
				
				if(!BIT_CHECK(pBvExtraInfo->m_dwType, BVT_PICKING))
					continue;
		
				wResult = Intersect_Ray_Bv( pRay, &pBvExtraInfo->m_BV, &fT );
				if(wResult != NTCT_OUTSIDE)	break;
			}

			if( NTCT_OUTSIDE == wResult )
			{
				WzBoundingVolume bv = *pTargetObject->GetBoundingVolume();
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}



			if (NTCT_OUTSIDE == wResult )  
			{				
				continue;
			}
			else
			{	
				// 이전에 선택한 캐릭터보다 가까운 캐릭터를 선택한다.
				if (iID == -1)
				{
					if (fT < fSelectDist )
					{					
						// 최종적으로 선택된 경우
						pTempMapObject = (MapObject *)pTargetObject;
						if ( pTempMapObject->IsPickable() )
						{
							pSelectChar = pTempMapObject;
							fSelectDist = fT;
						}												
					}	
				}
				else
				{
					// 특정 오브젝트가 걸렸냐?
					if (pTempMapObject->GetObjectKey() == (DWORD)iID)
					{
						pSelectChar = (MapObject *)pTargetObject;
						break;
					}
				}				
			}			
		}		
	}

	return (MapObject *)pSelectChar;
}


// 멥 드로우 오브젝트 너도 오브젝트로 만들어야한다.
// NON character로 넣자!!
// 그래서 오브젝트 메니저에서 관리하도록 하자!!
// 모든 오브젝트는 오브젝트 메니저에서 관리하도록 한다..
MapWzUnitDraw * Map::GetPickedMapObject(WzRay * pRay, int iID /*= -1*/)
{
	// 어차피 이놈은 없어질것이다.
	if (!m_bLoaded)
	{
		return NULL;
	}

	// 모두 돌면서 검사 
	m_pIntersecBoundMapObjectArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(
		DTO_OBJECT, 
		pRay, 
		m_pIntersecBoundMapObjectArray
		);

	float fT = 0.f;
	MapWzUnitDraw * pTargetObject = NULL;
	DWORD dwKey = 0;
	int iSize = m_pIntersecBoundMapObjectArray->GetCount();
	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundMapObjectArray->Get(i);
		pTargetObject = m_pMapUnitCacher->GetMapUnitDraw(dwKey);

		if (!pTargetObject)
			continue;

		if (NTCT_OUTSIDE == Intersect_Ray_Bv( pRay, 
	                    static_cast<MapWzUnitDraw *>(pTargetObject)->pUnitDraw->GetBoundingVolumePtr(), 
                        &fT ) )  
		{
			pTargetObject = NULL;
			continue;
		}
		else
		{
			if (MAP_SPEED_TREE_KEY_START >= pTargetObject->wzId)
			{ 
				pTargetObject = NULL;
			}
			else
			{	
				if (iID == -1)
				{
					// 모두 검사중에 걸린놈.
					break;
				}
				else
				{
					// 특정 오브젝트가 걸렸냐?
					if (pTargetObject->dwUniqueID == (DWORD)iID)
					{
						break;
					}
				}
			}
		}
	}

	return pTargetObject;
}

void Map::RenderCircleShadow( WzVector *wvPos )
{

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	//	GENERALPARAM 대신에 게임 옵션에 있는 그림자 퀄리티 옵션을 적용한다.
	if( GAMEOPTION->GetShadowQuality() != eOPTION_LEVEL_MIDDLE ) return;

	// 기존의 것 제거
	if ( m_pwsmCircleShadow )
	{
		g_pSunTerrain->DestroyDecal( m_pwsmCircleShadow );
		m_pwsmCircleShadow = NULL;
	}


	if ( INVALID_HANDLE_VALUE == s_hTextureCircleShadow )
	{
		s_hTextureCircleShadow = g_pSunRenderer->x_pManagerTexture->GetSpecialTexture( StrToWzID( "CSHD" ) );
	}

	// 새로 생성
	WzVector wvNormal = { 0.0f, 0.0f, 1.0f };
	WzVector wvUp = { 0.0f, 1.0f, 0.0f };
	g_pSunTerrain->CreateDecal( &m_pwsmCircleShadow, *wvPos, wvNormal, wvUp, 1.5f, 1.0f );
	m_pwsmCircleShadow->m_hTexDiffuse = s_hTextureCircleShadow;

	if( m_pwsmCircleShadow )
	{
		g_pSunRenderer->SetColor( 0, 0, 0, 180 );
		g_pSunRenderer->RealizeRenderSimpleMesh( m_pwsmCircleShadow );
		g_pSunRenderer->SetColor( 255, 255, 255 );
	}
#endif

}



BOOL Map::GetHeroPositionOnlySingleVersion(WzVector * pWzVec)
{
	if ( GetStartAreaPosition(pWzVec) )
	{
		return TRUE;
	}
	else
	{
		if (1 == m_MapInfo.m_iFieldID)
		{ 
			SetVector( 
				pWzVec, 
				-142.0f, 75.0f, 13.0f );	
		}
		else if (2 == m_MapInfo.m_iFieldID)
		{ 
			SetVector( 
				pWzVec, 
				-15.60f, -83.38f, -3.86f );	
		}
		else if (3 == m_MapInfo.m_iFieldID)
		{ 
			SetVector( 
				pWzVec, 
				9.47f, -71.66f, -2.92f );	
		}
		else if (4 == m_MapInfo.m_iFieldID)
		{ 
			SetVector( 
				pWzVec, 
				-64.07f, -10.39f, -26.16f );
		}
		else if (5 == m_MapInfo.m_iFieldID)
		{
			SetVector( 
				pWzVec, 
				42.13f, 198.03f, 21.69f );		
		}
		else if (6 == m_MapInfo.m_iFieldID)
		{
			SetVector( 
				pWzVec, 
				375.56f, 143.36f, 12.36f );	
		}
		else if (7 == m_MapInfo.m_iFieldID)
		{
			SetVector( 
				pWzVec, 
				393.84f, -49.94f, 5.04f );		
		}
		else if(61001 == m_MapInfo.m_iFieldID)
		{
			SetVector( 
				pWzVec, 
				241.76f, 179.21f, 15.14f );		
		}
        else
        {
            return FALSE;
        }
	}

	return TRUE;
}

void Map::AutoCameraPlay( DWORD dwTime )
{
	if( m_pAutoCameraPlay == NULL )
	{
		return;
	}

	WzVector wvUp = {0.0f, 0.0f, 1.0f};

	Object * pObject = (Object * )g_ObjectManager.GetObject(m_dwAutoCameraTargetObjectKey);

	if (pObject)
	{
		// 연출 카메라
		int iTile = pObject->GetPathExplorer()->GetTile();
		int iConvex = ( iTile == -1) ? 0 : m_pWorldBase->x_pPathFinder->GetConvexOfTile( iTile );
		m_wvAutoCameraTargetPos = pObject->GetPathExplorer()->GetVisiblePos();

		char szTest[0xff];
		sprintf(szTest, "Vec : X : %f, Z : %f, Y :%f\n", 
			m_wvAutoCameraTargetPos.x, m_wvAutoCameraTargetPos.z, m_wvAutoCameraTargetPos.y );

		OutputDebugStringA(szTest);

		// 마우스
		POINT MousePos;
		::GetCursorPos( &MousePos );
		MousePos.x = ( long )m_pDraw->RealXToStdX( ( float )MousePos.x );
		MousePos.y = ( long )m_pDraw->RealYToStdY( ( float )MousePos.y );
		float fScreenX = ( float)( MousePos.x - ( STD_2D_WIDTH / 2)) / ( float)( STD_2D_WIDTH / 2);
		float fScreenY = ( float)( MousePos.y - ( STD_2D_HEIGHT / 2)) / ( float)( STD_2D_HEIGHT / 2);
		// 발
		POINT ptFoot;
		m_pDraw->GetScreenPos( &ptFoot, &m_wvAutoCameraTargetPos );
		float fFootX = ( float)( ptFoot.x - ( STD_2D_WIDTH / 2)) / ( float)( STD_2D_WIDTH / 2);
		float fFootY = ( float)( ptFoot.y - ( STD_2D_HEIGHT / 2)) / ( float)( STD_2D_HEIGHT / 2);
		// 카메라 세팅
		m_pAutoCameraPlay->Play( iConvex, &m_wvAutoCameraTargetPos, &m_wvAutoCameraFrom, &m_wvAutoCameraTo,
			fScreenX, fScreenY, fFootX, fFootY,
			dwTime );

		//	카메라 설정
		m_pDraw->SetCameraMatrix( &m_wvAutoCameraFrom, &m_wvAutoCameraTo, &wvUp );
	}
}

DWORD Map::GetCurrentTriggerID()
{
	return m_dwCurrentTrigger;
}


void Map::SetUseAutoCamera(BOOL bUse)
{
	m_bUseAutoCamera = bUse;
}


BOOL Map::GetUseAutoCamera() const
{
	return m_bUseAutoCamera;
}

WzVector * Map::GetAutoCameraPos() const
{
	return (WzVector *)&m_wvAutoCameraTargetPos;
}

void Map::SetAutoCameraTargetObjectKey(DWORD dwObjectKey)
{
	m_dwAutoCameraTargetObjectKey = dwObjectKey;
}

DWORD Map::GetAutoCameraTargetObjectKey() const
{
	return m_dwAutoCameraTargetObjectKey;
}


void Map::ProcessMapObjects( DWORD dwTick )
{
	m_pDraw->x_pManagerSpeedTree->ProgressFrame(((float)dwTick * 0.001f), true);
}



WzSpecialArea * Map::GetMapArea( DWORD dwID )
{
	if (!GetWorldBase())
	{
		return NULL;
	}

	WzSpecialArea *pInfo = GetWorldBase()->GetSpecialArea(dwID);
	
	return pInfo;
}

WzSpecialArea * Map::GetMapAreaByIndex( DWORD dwIndex )
{
	if (!GetWorldBase())
	{
		return NULL;
	}

	WzSpecialArea *pInfo = GetWorldBase()->GetSpecialAreaByIndex(dwIndex);
	
	return pInfo;
}

int Map::GetNumberOfSpecialArea()
{
	if (!GetWorldBase())
	{
		return 0;
	}

	return GetWorldBase()->GetNumberOfSpecialArea();
}


BOOL Map::GetStartAreaPosition(WzVector * pwzPos)
{
	WzID id = StrToWzID("STRT");
	WzSpecialArea * pArea = GetMapArea(id);
	if (NULL == pArea)
	{
		id = StrToWzID("STR0");
		pArea = GetMapArea(id);
	}

    if (NULL != pArea)
    {
        switch ( pArea->m_wbvRegion.m_eBvType )
        {
        case BVT_AABB:	
            {
                *pwzPos = (pArea->m_wbvRegion.m_aabb.m_wvMin + pArea->m_wbvRegion.m_aabb.m_wvMax);
                pwzPos->x /= 2.0f;
                pwzPos->y /= 2.0f;
                pwzPos->z /= 2.0f;
            }
            break;

        case BVT_PATH_TILE_INDEX:
            {

                assert(pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount > 0);
                assert(pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[0] >= 0);

                *pwzPos = GetWorldBase()->GetRandomPosInTile( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[0] );
            }
            break;			

        default:
            assert(0);
            return FALSE;
        }

        return TRUE;
    }
    else
    {
        // 시작 위치 영정정보가 없음.
        // GENERAL.INI 를 통하여 설정된 타일을 이용하여 지정
        int tile_index = GENERALPARAM->GetStartTileIndex();
        *pwzPos = GetWorldBase()->GetRandomPosInTile(tile_index);
        
        return TRUE;
    }
    
    return FALSE;
}

BOOL Map::GetAreaPosition(WzVector * pwzPos, DWORD dwAreaID, BOOL RandomTilePos /*= TRUE*/)
{
    WzID id;

    id = dwAreaID;

    WzSpecialArea * pArea = GetMapArea(id);

    if (!pArea)
    {
        return FALSE;
    }

    switch ( pArea->m_wbvRegion.m_eBvType )
    {
    case BVT_AABB:	
        {
            *pwzPos = (pArea->m_wbvRegion.m_aabb.m_wvMin + pArea->m_wbvRegion.m_aabb.m_wvMax);
            pwzPos->x /= 2.0f;
            pwzPos->y /= 2.0f;
            pwzPos->z /= 2.0f;
        }
        break;
    case BVT_PATH_TILE_INDEX:
        {

            assert(pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount > 0);
            assert(pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[0] >= 0);
			if(TRUE == RandomTilePos)
				*pwzPos = GetWorldBase()->GetRandomPosInTile( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[0] );
			else
			{
				WzVector wvPos[3];
				GetWorldBase()->GetTileVertexInfo(&wvPos[0], pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[0]);
				SetVector(pwzPos, wvPos[0].x, wvPos[0].y, wvPos[0].z);
			}
        }
        break;			

    default:
        assert(0);
        return FALSE;
    }

    return TRUE;
}

void Map::CalculateVisibleUnit(void)
{
	St_Camera *pCameraInfo = m_pDraw->GetCameraInfo();
	
	assert(m_parPossibleVisibleUnitList);
	assert(m_parPossibleVisibleObjectList);
	assert(m_parVisibleObjectList);
	assert(m_parVisibleUnitList);
	assert(m_parAlphaVisibleUnitList);
	assert(m_parAlphaVisibleObjectList);

	int iUnitNum = m_parPossibleVisibleUnitList->GetCount();

	m_iVisMapObjCnt = 0;

	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parPossibleVisibleUnitList->Get(i);

		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);
		if(!pObject)
			continue;

		BOOL bVisible = FALSE;
		WzMatrix mtViewTransform;
		g_pSunRenderer->GetViewMatrix(&mtViewTransform);

		WzBoundingVolume *pBV = pObject->GetBoundingVolume();
		
    	if (Outside_Frustum_Bv(&pCameraInfo->m_wfFrustum,pBV) != NTCT_OUTSIDE)
		{
		    if (!IsOccluded(&mtViewTransform,pBV))
			{
				bVisible = TRUE;
			}
		}
        if ((bVisible == FALSE) && 
            (pObject == g_pHero) && 
            (g_pHero != NULL) && 
            (g_pHero->IsRiding() == TRUE) &&
            (g_pHero->rider() != NULL) && 
            (g_pHero->rider()->is_spread() == true) && 
            (g_Camera.GetCameraDistance() >= 2.5f))
        {
            //! 탈것을 타고 날았을때는 바운딩 박스가 아래 있기 때문에 컬링에서 제외될 수가 있음.
            //! 예외 사항으로 날고 있을때는 카메라 거리로 체크를 다시함.
            bVisible = TRUE;
        }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		if ( pObject->GetObjectType() == STATUE_PLAYER_OBJECT )
		{
			// StatuePlayer는 무조건 보인다.
			bVisible = TRUE;
		}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

		pObject->SetEnableDisplay(bVisible);

		if (bVisible)
		{
			switch (pObject->GetSCObject().GetObjectType())
			{
			case MAP_OBJECT:
				{
					MapObject *pMapObject = (MapObject *)pObject;

					if(100 <= m_iVisMapObjCnt && 
					   FALSE == pMapObject->IsTerrain() && 
					   FALSE == pMapObject->IsTriggerObj() &&
					   FALSE == pMapObject->IsWayPointObj())
					{
						WzVector wvLen = pObject->GetVisiblePos() - pCameraInfo->m_wvFrom;
						float fLen = VectorLength2(&wvLen);
						float fSize = 0.f;
						if(BVT_AABB == pObject->GetBoundingVolume()->m_eBvType)
						{
							WzVector wvSize = pObject->GetBoundingVolume()->m_aabb.m_wvMin - pObject->GetBoundingVolume()->m_aabb.m_wvMax;
							fSize = VectorLength2(&wvSize);
						}
						else if(BVT_SPHERE == pObject->GetBoundingVolume()->m_eBvType)
						{
							fSize = pObject->GetBoundingVolume()->m_BvSphere.m_fRadius * pObject->GetBoundingVolume()->m_BvSphere.m_fRadius;
						}

						if(10000.f < fLen)
						{
							if(100.f > fSize)
							{
								pObject->SetEnableDisplay(FALSE);
								pObject->SetCurShowFlag(FALSE);
								break;
							}
						}
						else if(2500.f < fLen)
						{
							
							if(25.f > fSize)
							{
								pObject->SetEnableDisplay(FALSE);
								pObject->SetCurShowFlag(FALSE);
								break;
							}
						}
						else if(900.f < fLen)//30M
						{
							if(4.f > fSize)
							{
								pObject->SetEnableDisplay(FALSE);
								pObject->SetCurShowFlag(FALSE);
								break;
							}
						}
					}

					if(m_pDraw)
					{
						WzVector vDiff = pCameraInfo->m_wvFrom - pObject->GetVisiblePos();

						if(pMapObject->IsTerrain())
						{
							m_parVisibleUnitList->Add(dwUniqueID);
							pMapObject->SetCurShowFlag(TRUE);
						}
						else if (VectorLength2(&vDiff) <= m_fVisibleRange*m_fVisibleRange)
						{
							WzRay ray = g_Camera.GenerateCameraRay();
							float fT = 0.0f;
							WzBoundingVolume bv = *(pObject->GetBoundingVolume());

							GlobalFunc::ShrinkBoundingVolume(&bv,0.9f);

							WzVector vCenter;
							if (bv.m_eBvType == BVT_AABB)
							{
								vCenter = (bv.m_aabb.m_wvMax + bv.m_aabb.m_wvMin) * 0.5f;
							}
							else if (bv.m_eBvType == BVT_SPHERE)
							{
								vCenter = bv.m_BvSphere.m_wvCenter;
							}

							if (NTCT_OUTSIDE == Intersect_Ray_Bv(&ray,&bv,&fT) || !pMapObject->IsAlpha())
							{
								m_parVisibleUnitList->Add(dwUniqueID);
							}
							else
							{
								if (fT < g_Camera.GetCameraDistance())
								{
									// 플레이어보다 낮은위치에 있는 놈들은 알파 안한다! (좀 안좋을지도..)
									// 바운딩볼륨 조절만이 진정한 해결책
									if (g_pHero) 
									{
										if (g_pHero->GetVisiblePos().z > vCenter.z)
										{
											m_parVisibleUnitList->Add(dwUniqueID);									
										}
										else
										{									
											m_parAlphaVisibleUnitList->Add(dwUniqueID);
										}								
									}
									else
									{
										m_parAlphaVisibleUnitList->Add(dwUniqueID);
									}
								}
								else
								{
									m_parVisibleUnitList->Add(dwUniqueID);
								}
							}

							pObject->SetEnableDisplay(TRUE);
							pObject->SetCurShowFlag(TRUE);

							m_iVisMapObjCnt++;
						}						
						else
						{
							pObject->SetEnableDisplay(FALSE);
							pObject->SetCurShowFlag(FALSE);
						}
					}
					else
					{
						assert(!"NULL == m_pDraw");
					}
				}
				break;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
			case STATUE_PLAYER_OBJECT:
				{
					if ( m_pDraw )
					{
						m_parVisibleUnitList->Add(dwUniqueID);
						pObject->SetCurShowFlag(TRUE);
					}
				}
				break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

			default:
				{	
					const float c_fBaseVisibleLength = 3500.0f;
					const float c_fWeightVisibleLength = 50.0f * GAMEOPTION->GetCullLength();
					const float fVisibleLength = c_fBaseVisibleLength + c_fWeightVisibleLength;

					WzVector vDiff = pCameraInfo->m_wvFrom - pObject->GetVisiblePos();

					if (VectorLength2(&vDiff) <= fVisibleLength)
					{
                        if ((pObject->IsKindOfObject(PLAYER_OBJECT) == TRUE) ||
                            (pObject->IsKindOfObject(PET_OBJECT) == TRUE))
                        {
                            ++m_iVisPlayerCnt;
                        }
                        m_parVisibleUnitList->Add(dwUniqueID);
                        pObject->SetCurShowFlag(TRUE);

					}						
					else
					{
						pObject->SetEnableDisplay(FALSE);
						pObject->SetCurShowFlag(FALSE);
					}
				}
				break;
			}
		}
		else
		{
			if (MAP_OBJECT != pObject->GetSCObject().GetObjectType() )
			{
				pObject->SetEnableDisplay(FALSE);
				pObject->SetCurShowFlag(FALSE);
			}
		}

	}

	// 스피드 트리
	int iObjectNum = m_parPossibleVisibleObjectList->GetCount();
	for( int i = 0 ; i < iObjectNum ; ++i )
	{
		DWORD dwUniqueID = m_parPossibleVisibleObjectList->Get(i);
		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);
	
		if (dwUniqueID >= MAP_SPEED_TREE_KEY_START)
		{
			SPEED_TREE_ITR itr;
			itr = m_SpeedTreeMap.find(dwUniqueID);
			SPEED_TREE_INFO * pSpeedTreeInfo = (SPEED_TREE_INFO *)itr->second;

			CSpeedTreeDrawInfo * pSpeedTreeDrawInfo = 
				m_pDraw->x_pManagerSpeedTree->GetTreeByHandle(pSpeedTreeInfo->hHandle);			
			
			if (Outside_Frustum_Bv( &pCameraInfo->m_wfFrustum, &pSpeedTreeDrawInfo->GetWzBoundingVolume() ) != NTCT_OUTSIDE)
			{
				WzRay ray = g_Camera.GenerateCameraRay();
				float fT = 0.0f;
				int iCount = pSpeedTreeDrawInfo->GetCollisionObjectCount();
				
				WzBoundingVolume bv;

				BOOL bAlpha = FALSE;
				for (int a = 0; a < iCount; a++) 
                {
					bv = *pSpeedTreeDrawInfo->GetCollisionObject(a);

					if (NTCT_OUTSIDE != Intersect_Ray_Bv(&ray,&bv,&fT))
					{
						if (fT < g_Camera.GetCameraDistance())
						{
							bAlpha = TRUE;
							break;
						}
						else
						{
							break;
						}
				    }
				}

				if (bAlpha)
				{
					m_parAlphaVisibleObjectList->Add(dwUniqueID);
				}
				else
				{
					m_parVisibleObjectList->Add(dwUniqueID);
				}
			}
		}
		else
		{
			if (m_pMapUnitCacher && (0.0f < GAMEOPTION->GetCullLength()))
			{
				MapWzUnitDraw * pMapUnitDraw = m_pMapUnitCacher->GetMapUnitDraw( dwUniqueID );

				if (pMapUnitDraw && pMapUnitDraw->pUnitDraw &&
					(Outside_Frustum_Bv(&pCameraInfo->m_wfFrustum, pMapUnitDraw->pUnitDraw->GetBoundingVolumePtr()) != NTCT_OUTSIDE))
				{
					m_parVisibleObjectList->Add(dwUniqueID);
				}
			}
		}		
	}
}


BOOL Map::GetPlayerDirection(WzVector * pwzDir)
{
    if (m_MapInfo.m_pCurFieldInfo)
	{
		CAMERA_EXTRA_INFO * pCamExtra = g_pMap->GetExtraCameraInfo();
		assert(pCamExtra);
		if (pCamExtra && pwzDir)
		{
			*pwzDir = pCamExtra->WzTargetPos - pCamExtra->WzPos;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL Map::GetTerrainHeight(WzVector *vPos,WzVector *vResult)
{
	if (!IsLoaded())
	{
		*vResult = *vPos;
		return FALSE;
	}

	WzRay ray;
	ray.m_wvOrigin = *vPos;
	ray.m_wvOrigin.z += 1.0f;

	SetVector(&ray.m_wvDirection,0.0f,0.0f,-1.0f);
	
	float fT;
	int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT );
	if ( iTerrainMeshFocussing >= 0 )
	{
		if (fT <= 0.01f)
		{
			fT = 0.01f;
		}

		*vResult = ray.m_wvOrigin + ray.m_wvDirection * (fT - 0.01f);
		return TRUE;					
	}
	else
	{
		*vResult = *vPos;
		return FALSE;
	}
}

void Map::GetTriggerGroupName(TCHAR* szVal)
{
	if('\0' != m_MapInfo.m_szTriggerGroupName[0])
		StrnCopy(szVal,m_MapInfo.m_szTriggerGroupName,MAX_TRIGGER_GROUPNAME_LENGTH-1);
}

void Map::RenderAreaInfo()
{	
	TCHAR szInfo[256];
	TCHAR szID[5];
	St_Camera * pInfo = g_pSunRenderer->GetCameraInfo();
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

	int areanum = GetWorldBase()->GetNumberOfSpecialArea();
	for (int a = 0; a < areanum; a++)
	{
		WzSpecialArea * pArea = GetWorldBase()->GetSpecialAreaByIndex(a);

		switch ( pArea->m_wbvRegion.m_eBvType )
		{
		case BVT_AABB:
			{
				if (Outside_Frustum_Bv( &pInfo->m_wfFrustum, &pArea->m_wbvRegion ) != NTCT_OUTSIDE)
				{
					g_pSunRenderer->RenderBoundingVolume(&pArea->m_wbvRegion);				
				}
			}
			break;

		case BVT_PATH_TILE_INDEX:
			{
				g_pSunRenderer->SetMeshDrawMode(MD_WIREFRAME);
				m_pWorldBase->x_pPathFinder->RenderPathTile(
                    g_pSunRenderer,
                    0,
                    (int *)pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData,
                    pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount );
				
				g_pSunRenderer->SetMeshDrawMode(MD_POLYGON);				
			}
			break;
		}
	}

	g_pApplication->Set2DMode(TRUE);
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	POINT pt;
	RECT rc;
	
	for (int a = 0; a< areanum; a++)
	{
		WzSpecialArea *pArea = GetWorldBase()->GetSpecialAreaByIndex(a);

		memcpy(szID,&pArea->m_wiIdentity,4);
		szID[4] = 0;

		_stprintf(szInfo,_T("Index : %d ID : %d"),a,pArea->m_wiIdentity);

		switch ( pArea->m_wbvRegion.m_eBvType )
		{
		case BVT_AABB:
			{
				if (Outside_Frustum_Bv( &pInfo->m_wfFrustum, &pArea->m_wbvRegion ) != NTCT_OUTSIDE)
				{
					WzVector vCenter = pArea->m_wbvRegion.m_aabb.m_wvMax + pArea->m_wbvRegion.m_aabb.m_wvMin;
					vCenter.x *= 0.5f;
					vCenter.y *= 0.5f;
					vCenter.z *= 0.5f;

					g_pSunRenderer->GetScreenPos( &pt, &vCenter );
					
					rc.left = pt.x;
					rc.top = pt.y;
					rc.right = rc.left + 500;
					rc.bottom = rc.top + 20;
					
					g_pSunRenderer->x_pManagerTextOut->DrawText(szInfo,&rc,WzColor_RGBA(255,255,255,255));
				}
			}
			break;

		case BVT_PATH_TILE_INDEX:
			{
				g_pSunRenderer->SetMeshDrawMode(MD_WIREFRAME);
				m_pWorldBase->x_pPathFinder->RenderPathTile(g_pSunRenderer,0,(int *)pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData,pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount);
				
				g_pSunRenderer->SetMeshDrawMode(MD_POLYGON);				
			}
			break;
		}
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pApplication->Set2DMode(FALSE);
	g_pSunRenderer->SetColor(oldcolor);	
}

void Map::ToggleShowAreaInfo()
{
	m_bRenderAreaInfo ^= 1;
}

//----------------------------------------------------------------------------
/**
*/
void 
Map::SetRenderFlag(BOOL bFlag)
{
	this->m_bRenderFlag = bFlag;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
Map::IsRender()
{
	return ( m_bRenderFlag );
}

void Map::RenderUnitInfo(void)
{
    if ( !this->IsRender() || !m_bLoaded ) 
    {
        WzColor wzBackGround = WzColor_RGBA(0,0,0,255);

        g_pSunRenderer->ClearRender(CR_DEFAULT, &wzBackGround  );

        return;
    }

	assert(m_parVisibleUnitList);

	// 카메라에서 먼 순위로 정렬

	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();

	MAP_OBJECT_RENDER_INFO info;

    m_vecRenderInfoList.resize(0); 

	vector<MAP_OBJECT_RENDER_INFO>::iterator iter;

	WzVector vLength;

	int iUnitNum = m_parVisibleUnitList->GetCount();
	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parVisibleUnitList->Get(i);

		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);

		assert(pObject);

		if (pObject) 
		{
			info.dwObjectKey = dwUniqueID;
			vLength = pObject->GetVisiblePos() - pCameraInfo->m_wvFrom;
			info.fDistance = VectorLength2(&vLength);
			m_vecRenderInfoList.push_back(info);
		}		
	}

	if (m_vecRenderInfoList.size() > 0) 
    {
		sort(m_vecRenderInfoList.begin(),m_vecRenderInfoList.end());
	}

	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;	
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->SetAlphaTestMode(0,CF_ALWAYS);
	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();

	ENUM_CULL_FACE      eCullFaceMode;
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
	ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();
	
	BOOL bZTest = g_pSunRenderer->IsEnableZTest();
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	g_pSunRenderer->SetZBufferTest(FALSE);
	g_pSunRenderer->SetZBufferWrite(FALSE);

	DWORD dwPickedKey = g_HeroInput.GetPickedObjectKey();		
	for( iter = m_vecRenderInfoList.begin(); iter !=  m_vecRenderInfoList.end(); iter++ )
	{
		DWORD dwUniqueID = iter->dwObjectKey;

		Object * pObject = g_ObjectManager.GetObject(dwUniqueID);
		
		if(!pObject)
			continue;

		assert(pObject);

		switch(pObject->GetSCObject().GetObjectType() )
		{
		    case PLAYER_OBJECT:
			{
				Player * pPlayer = static_cast<Player*>(pObject);
				if (pPlayer->GetShow() && pPlayer->IsShowByGameOption()) 
                {
					pPlayer->RenderInfo();
				}
				
			}
			break;


		case SSQMONSTER_OBJECT:
		case MONSTER_OBJECT:
			{
				Monster* pMonster = static_cast<Monster*>(pObject);
				pMonster->RenderInfo();				
			}
			break;

		case LOTTO_NPC_OBJECT:
		case NPC_OBJECT:
			{				
				NPC* pNPC = static_cast<NPC*>(pObject);
				pNPC->RenderInfo();				
			}
			break;

		case PET_OBJECT:
			{				
				Pet* pPet = static_cast<Pet*>(pObject);

				Player* pPlayer = pPet->GetMasterObject();
				if (pPlayer->IsShowByGameOption())
				{
					pPet->RenderInfo();				
				}
			}
			break;			

		case ITEM_OBJECT:
			{				
				Item * pItem = static_cast<Item *>(pObject);

				if( pItem->IsEnableDisplay() )
				{
					if (dwPickedKey == pItem->GetObjectKey())
                    {
					    pItem->RenderInfo(true);
                    }
					else
                    {
					    pItem->RenderInfo(false);
                    }
				}
				else
				{
					if (dwPickedKey == pItem->GetObjectKey())
					{
						pItem->RenderInfo(false);
					}
				}
			}
			break;

		case MAP_OBJECT:
			{
				MapObject * pMapObject = static_cast<MapObject *>(pObject);
				if (dwPickedKey == pMapObject->GetObjectKey())
				{
					pMapObject->RenderInfo();
				}
			}
			break;
		}
	}

	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->SetZBufferTest(bZTest);
	g_pSunRenderer->SetZBufferWrite(bZWrite);
}

void Map::RenderAlphaUnits(void)
{
	assert(m_parAlphaVisibleUnitList);

	
	int iUnitNum = m_parAlphaVisibleUnitList->GetCount();
	for (int i = 0; i < iUnitNum; ++i)
	{
		DWORD dwUniqueID = m_parAlphaVisibleUnitList->Get(i);

		Object* pObject = g_ObjectManager.GetObject(dwUniqueID);
		if (!pObject)
        {
			continue;
        }

		assert(pObject);

		switch (pObject->GetSCObject().GetObjectType() )
		{
		    case PLAYER_OBJECT:
			{
				Player* pPlayer = static_cast<Player*>(pObject);
								
				if (pPlayer->GetShow())
				{					
                    pPlayer->Render();
				}		

				g_dwCharRenderCount++;
			}
			break;

		case SSQMONSTER_OBJECT:
		case MONSTER_OBJECT:
			{
				Monster* pMonster = static_cast<Monster*>(pObject);
				pMonster->Render();				
				g_dwCharRenderCount++;
			}
			break;

		case PET_OBJECT:
			{
				Pet* pPet= static_cast<Pet*>(pObject);
				pPet->Render();
			}
			break;

		case LOTTO_NPC_OBJECT:
		case NPC_OBJECT:
			{				
				NPC* pNPC = static_cast<NPC*>(pObject);
				pNPC->Render();				
			}
			break;

		case MONEY_OBJECT:
		case ITEM_OBJECT:
			{
				Item* pItem = static_cast<Item*>(pObject);				
				pItem->Render();				
			}
			break;

		case MAP_OBJECT:
			{
				MapObject* pMapObject = static_cast<MapObject*>(pObject);

				WzColor oldcolor = pMapObject->GetColor();
				int alpha = Alpha_WzColor(oldcolor);
				alpha = min(128, alpha);

				if (pMapObject->IsAlpha())
                {
					pMapObject->SetColor(Red_WzColor(oldcolor), Green_WzColor(oldcolor), Blue_WzColor(oldcolor), alpha);
                }

				pMapObject->Render();
				pMapObject->SetColor(oldcolor);
			}
			break;
		}
	}
}

void Map::RenderAlphaObjects(void)
{
	St_Camera *pCameraInfo = m_pDraw->GetCameraInfo();
	
	int iUnitNum = m_parAlphaVisibleObjectList->GetCount();
	for( int i = 0 ; i < iUnitNum ; ++i )
	{
		DWORD dwUniqueID = m_parAlphaVisibleObjectList->Get(i);

		if (dwUniqueID >= MAP_SPEED_TREE_KEY_START)
		{
			SPEED_TREE_ITR itr;
			itr = m_SpeedTreeMap.find(dwUniqueID);
			SPEED_TREE_INFO * pSpeedTreeInfo = (SPEED_TREE_INFO *)itr->second;

			CSpeedTreeDrawInfo * pSpeedTreeDrawInfo = 
			m_pDraw->x_pManagerSpeedTree->GetTreeByHandle(pSpeedTreeInfo->hHandle);
						
			m_RenderSpeedTreeVec.push_back(pSpeedTreeInfo->hHandle);
		}
		else
		{
			// 풀은 넣지않는다!
			assert(0);
		}
	}

	// speedtree 렌더할수 있도록 던져주기.
	if (m_RenderSpeedTreeVec.size() > 0)
	{
		ENUM_ALPHABLEND alphaMode = m_pDraw->GetAlphaBlendMode();
		m_pDraw->SetAlphaBlendMode(AB_ALPHA);
		DWORD dwRef;
		ENUM_CMP_FUNC eMode;
		g_pSunRenderer->GetAlphaTestMode(&dwRef,&eMode);
		g_pSunRenderer->SetAlphaTestMode(0);
		WzColor oldcolor = m_pDraw->GetDiffuseColor();
		m_pDraw->SetColor(255,255,255,150);		
		m_pDraw->x_pManagerSpeedTree->Render( &m_RenderSpeedTreeVec[0], m_RenderSpeedTreeVec.size());

        m_RenderSpeedTreeVec.resize(0); 

		m_pDraw->SetColor(oldcolor);		
		m_pDraw->SetAlphaBlendMode(alphaMode);
		g_pSunRenderer->SetAlphaTestMode(dwRef,eMode);
	}
}

void Map::RenderSector()
{
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetColor(WzColor_RGBA(255,255,0,255));
	g_pSunRenderer->SetMeshDrawMode(MD_WIREFRAME);
	ENUM_ALPHABLEND oldblend = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_NONE);
	g_pSunRenderer->EnableTexture(FALSE);

	St_Camera *pInfo = g_pSunRenderer->GetCameraInfo();

	BvAABB bv = m_pWorldBase->GetTerrainSize();

	float fMinX = bv.m_wvMin.x;
	float fMinY = bv.m_wvMin.y;

	DWORD dwXSize = (DWORD)(bv.m_wvMax.x - bv.m_wvMin.x);
	DWORD dwYSize = (DWORD)(bv.m_wvMax.y - bv.m_wvMin.y);

	int iXCount = dwXSize / m_dwSectorSize + ((dwXSize % m_dwSectorSize) ? 1 : 0);
	int iYCount = dwYSize / m_dwSectorSize + ((dwYSize % m_dwSectorSize) ? 1 : 0);

	WzBoundingVolume tempbv;

	tempbv.m_eBvType = BVT_AABB;
	tempbv.m_aabb.m_wvMin.z = bv.m_wvMin.z;
	tempbv.m_aabb.m_wvMax.z = bv.m_wvMax.z;

	if (g_pHero)
	{
		WzVector vPos = g_pHero->GetVisiblePos();

		tempbv.m_aabb.m_wvMin.z = vPos.z;
		tempbv.m_aabb.m_wvMax.z = vPos.z + 5.0f;
	}

	int iRenderCount = 0;

	for (int iX = 0; iX < iXCount; iX++)
	{
		for (int iY = 0; iY < iYCount; iY++)
		{
			tempbv.m_aabb.m_wvMin.x = fMinX + iX * m_dwSectorSize;
			tempbv.m_aabb.m_wvMin.y = fMinY + iY * m_dwSectorSize;
			tempbv.m_aabb.m_wvMax.x = fMinX + (iX + 1) * m_dwSectorSize;
			tempbv.m_aabb.m_wvMax.y = fMinY + (iY + 1) * m_dwSectorSize;

			if (Outside_Frustum_Bv( &pInfo->m_wfFrustum, &tempbv ) != NTCT_OUTSIDE)
			{

				g_pSunRenderer->RenderBoundingVolume(&tempbv);

				iRenderCount++;
			}
		}
	}
	
	g_pSunRenderer->SetMeshDrawMode(MD_POLYGON);
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->SetAlphaBlendMode(oldblend);
	g_pSunRenderer->EnableTexture(TRUE);
}


//----------------------------------------------------------------------------
/**
*/
void 
Map::StopAmbientSound()
{
    wzsnd_stop_all_ambi();
    m_iAmbientSoundHandle = -1;	
}


//----------------------------------------------------------------------------
/**
*/
void	
Map::SetAmbientVolume(float fVolume)
{
	wzsnd_set_volume_all_ambi( fVolume);
	Sound::Instance()->SetAmbientEffectVolume(fVolume);
}


//----------------------------------------------------------------------------
/**
*/
void 
Map::PlayAmbientSound()
{	
    int nAmbientCode = AmbientSoundInfoParser::Instance()->GetAmbientSoundCode(m_MapInfo.m_MapID);
    if (0 != nAmbientCode)
    {
        m_iAmbientSoundHandle =  g_SoundEffect.PlayAmbient(nAmbientCode);
    }
}



BOOL Map::IsApplyAnotherLight()
{
	return m_bApplyAnotherLight;
}

void Map::SetApplyAnotherLight(BOOL bFlag)
{
	m_bApplyAnotherLight = bFlag;
}

WzColor Map::GetCharacterAmbientColor()
{
	return m_CharacterAmbientColor;
}
WzColor Map::GetCharacterSunColor()
{
	return m_CharacterSunColor;
}

void Map::SetCharacterAmbientColor(WzColor color)
{
	m_CharacterAmbientColor = color;
}

void Map::SetCharacterSunColor(WzColor color)
{
	m_CharacterSunColor = color;
}

float Map::GetTerrainColorRate()
{
	return m_fTerrainColorRate;
}

void Map::SetTerrainColorRate(float fRate)
{
	m_fTerrainColorRate = min(1.0f,max(fRate,0.0f));	
}



void Map::ApplyOverrideEnvironmentSetting()
{
	const sMAPINFO* pMapInfo = GetMapInfo();
	DWORD dwKey = pMapInfo->dwEnvironmentCode[m_MapInfo.m_iFieldIndex];
	BASE_EnvironmentInfo *pInfo = EnvironmentInfoParser::Instance()->GetEnvironmentInfo(dwKey);
	if (pInfo)
	{
		m_bApplyAnotherLight = TRUE;	

		m_CharacterAmbientColor = WzColor_RGBA(pInfo->m_bAmbientR,pInfo->m_bAmbientG,pInfo->m_bAmbientB,255);
		m_CharacterSunColor = WzColor_RGBA(pInfo->m_bSunLightR,pInfo->m_bSunLightG,pInfo->m_bSunLightB,255);
		m_ChracterSunDirection.x = pInfo->m_fSunLightDirX;
		m_ChracterSunDirection.y = pInfo->m_fSunLightDirY;
		m_ChracterSunDirection.z = pInfo->m_fSunLightDirZ;

		WzColor wcLightColor = WzColor_RGBA(255,255,255,255);
		WzVector vDir = GetWorldBase()->GetSunLightDir();
		g_pSunRenderer->x_pManagerLight->SetCustomDirectionalLight(m_CharacterSunColor,vDir);
		g_pSunRenderer->x_pManagerLight->SetCustomLightStyle(CLS_DEFAULT);		
	}
	else
	{
		g_pSunRenderer->x_pManagerLight->SetCustomLightStyle(CLS_NONE);
	}
}

void Map::ApplyOptionSetting()
{
	const OptionParam& param = GAMEOPTION->GetGameOption();

    if (param.GetOptionValue( nOptionType::USE_BLUR) )
	{
#ifdef _PERF_DEBUG
		EnableBlur(FALSE);
#else
		EnableBlur(TRUE);
#endif
	}
	else
	{
		EnableBlur(FALSE);
	}

	//  포그
    g_pSunRenderer->EnableFog( param.GetOptionValue(nOptionType::USE_FOG) );
 
    if( param.GetOptionValue( nOptionType::USE_MAP_DISTANCE_VIEW ) )
		g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_DEPTH_OF_FIELD);
	else
		g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_DEPTH_OF_FIELD);

}

//------------------------------------------------------------------------------ 
int Map::GetSectorIndex(WzVector * pwzVec)
{
	//DWORD dwXIndex = 0;
	//DWORD dwYIndex = 0;

	//dwXIndex = (DWORD)( pwzVec->x - m_iShiftMinX ) / m_dwSectorSize;
	//dwYIndex = (DWORD)( pwzVec->y - m_iShiftMinY ) / m_dwSectorSize;

	//assert( dwXIndex + m_dwSectorXNum * dwYIndex < m_dwTotalSectorNum );
	//return ( dwXIndex + m_dwSectorXNum * dwYIndex );

    int iXIndex = static_cast<int>(pwzVec->x - m_iShiftMinX) / m_dwSectorSize;
    int iYIndex = static_cast<int>(pwzVec->y - m_iShiftMinY) / m_dwSectorSize;

    return (iXIndex + m_dwSectorXNum * iYIndex);
}

void Map::CalculateSectorInfo()
{
	const sMAPINFO* pMapInfo = GetMapInfo();
	if ( pMapInfo )
	{
		if (eZONETYPE_VILLAGE == pMapInfo->byMKind)
		{
			m_dwSectorSize = VILLAGE_SECTOR_SIZE;
		}
		else if(eZONETYPE_DOMINATION_FIELD == pMapInfo->byMKind)
		{
			m_dwSectorSize = DOMINATION_SECTOR_SIZE;
		}
		else
		{
			m_dwSectorSize = ROOM_SECTOR_SIZE;
		}
	}
	else
	{
		assert(0);
	}

	if (m_pWorldBase)
	{
		BvAABB box = m_pWorldBase->GetTerrainSize();

		m_iShiftMinX = (int)box.m_wvMin.x;
		m_iShiftMinY = (int)box.m_wvMin.y;

		DWORD XSize = 0, YSize = 0;
		XSize = (DWORD)(box.m_wvMax.x - box.m_wvMin.x);
		YSize = (DWORD)(box.m_wvMax.y - box.m_wvMin.y);

		BYTE byMarginX = 0, byMarginY = 0;
		XSize % m_dwSectorSize == 0 ? byMarginX = 0 : byMarginX = 1;
		YSize % m_dwSectorSize == 0 ? byMarginY = 0 : byMarginY = 1;

		m_dwSectorXNum				= XSize / m_dwSectorSize + byMarginX;
		m_dwSectorYNum				= YSize / m_dwSectorSize + byMarginY;

		m_dwTotalSectorNum			= m_dwSectorXNum * m_dwSectorYNum;
	}
}

float Map::CalcDistance(WzBoundingVolume* pbvTarget, WzVector &wvFrom, float *pfDistToCenter, float fRange)
{
	// 위치, 범위
	float    fRadius = 0.f;
	WzVector wvPos;
	if(pbvTarget->m_eBvType == BVT_AABB)
	{
		wvPos = (pbvTarget->m_aabb.m_wvMin + pbvTarget->m_aabb.m_wvMax) * 0.5f;
	}
	else if(pbvTarget->m_eBvType == BVT_SPHERE)
	{
		wvPos = pbvTarget->m_BvSphere.m_wvCenter;
	}
	else
	{
		assert(!"Not support this BoundingVolume type!");
	}

	float    fDistToInter = WZ_INFINITE; // 교차점까지의 거리
	WzVector wvDir;
	WzRay    ray;
		
	wvDir = wvPos - wvFrom;
	*pfDistToCenter = VectorLength(&wvDir);
	if(fRange < *pfDistToCenter)
		return WZ_INFINITE;

	VectorNormalize(&ray.m_wvDirection, &wvDir);
	ray.m_wvOrigin = wvFrom;

	Intersect_Ray_Bv(&ray, pbvTarget, &fDistToInter);

	return fDistToInter;
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
Map::ProcessAmbientSound()
{
	int				 total = 0;
	float			 fDistToInter = WZ_INFINITE; // 교차점까지의 거리
	float			 fDistToCenter = WZ_INFINITE; // 경계의 중심까지 거리	
	float			 fMinDistToCenter = WZ_INFINITE;
	float			 fMinDistToInter = WZ_INFINITE;
	float			 fRange = 30.f; // 사운드 유효 범위
	float			 fDrawRange = 300.f; // 화면에 그려질 사운드 볼륨의 유효 범위
	St_SoundVolume	 *pSV = NULL;
	CWzUnitDraw		 *pUDraw = NULL;
	St_Camera		 *pCameraInfo = NULL;
	WzVector		 wvFrom; // 사운드 컬링 중심점.
	D3DVECTOR		 listener_pos = {0,0,0};	
	D3DVECTOR		 listener_front = {0.0f,0.0f,1.0f};
	D3DVECTOR		 listener_top = {0.0f,1.0f,0.0f};
	D3DVECTOR		 listener_velocity = {0.0f,0.0f,0.0f};

	pCameraInfo = g_pSunRenderer->GetCameraInfo();
	
	if (g_pHero)
	{
		wvFrom = g_pHero->GetVisiblePos();
	}
	else	
	{
		wvFrom = pCameraInfo->m_wvFrom;
	}

	// Build sound list.
	m_pWorldBase->x_pTerrainSound2->BuildSoundList(
        g_pSunRenderer, 
        pCameraInfo->m_wfFrustum, 
        wvFrom, 
        fRange);

	//-------------------------------------------------------------------------
	// 사운드 영역에 대한 처리
	//
	St_EAX *pEAX = m_pWorldBase->x_pTerrainSound2->GetCurEAX();
	if(pEAX)
	{
		BOOL eax_supported = wzsnd_env_supported();
		if(eax_supported)
		{
			unsigned long ulID = pEAX->m_ulID;

			if( (pEAX->m_fVolume < 0.001f) && 
                (pEAX->m_fDamping < 0.001f)&& 
                (pEAX->m_fDecay < 0.001f) )
			{
				// 1) 첫번째 방법: 디폴트 세팅값을 사용할 경우
				wzsnd_env_set_preset(ulID);
			}
			else
			{
				// 2) 두번째 방법: 수동으로 세팅값을 지정할 경우.
				sound_env snd_env = { ulID // 환경 변수 ID
									, pEAX->m_fVolume // volume
									, pEAX->m_fDamping // damping
									, pEAX->m_fDecay }; // decay
				wzsnd_env_set(&snd_env);
			}

			m_bEnableEAX = TRUE;
		}
	}
	else
	{
		if(m_bEnableEAX)
		{
			wzsnd_env_disable(); // Turn off EAX
			m_bEnableEAX = FALSE;
		}
	}

	// 활성화 목록 처리
	StlMSoundVolume* pSndVolumes = m_pWorldBase->x_pTerrainSound2->Enum_ActiveSoundArea();
	StlMSoundVolume::iterator itor = pSndVolumes->begin();
	for(; itor != pSndVolumes->end(); ++itor)
	{
		St_SoundVolume* pSoundVolume = (*itor).second;

		StlVWzIDList::iterator itor2 = pSoundVolume->m_SoundDataList.begin();
		for(; itor2 != pSoundVolume->m_SoundDataList.end(); ++itor2)
		{
			St_SoundData* pSoundData = m_pWorldBase->x_pTerrainSound2->FindSoundData(*itor2);
			if(!pSoundData)
				continue;

			if(SDA_AMBIENT & pSoundData->m_Flags)
			{
				listener_pos.x = // listener의 위치 갱신
				listener_pos.y = 
				listener_pos.z = 0.f;
								
				StlVSoundList::iterator itor3 = pSoundData->m_pAmbient->m_SoundList.begin();
				for(; itor3 != pSoundData->m_pAmbient->m_SoundList.end(); ++itor3)
				{
					int handle = (*itor3).m_iSoundHandle;
					if(-1 == handle)
					{
						int idex = (*itor3).m_iSoundIdx;
						handle = wzsnd_play_ambi(idex, SND_PRIORITY_EXCLUSIVE, TRUE); //TRACE("\n> On\n");
						(*itor3).m_iSoundHandle = handle;
					}
				}
			}
			if(SDA_3DSOUND & pSoundData->m_Flags)
			{
				listener_pos.x = wvFrom.x; // listener의 위치 갱신
				listener_pos.y = wvFrom.z;
				listener_pos.z = wvFrom.y;
				
				StlVSoundList::iterator itor3 = pSoundData->m_p3DSound->m_SoundList.begin();
				for(; itor3 != pSoundData->m_p3DSound->m_SoundList.end(); ++itor3)
				{
					St_SoundFile* snd = (St_SoundFile*)&(*itor3);

					int handle = snd->m_iSoundHandle;
					if(-1 == handle)
					{
						// left-hand D3D coordinate system.
						D3DVECTOR source_pos = {0.0f, 0, 0};
						D3DVECTOR source_velocity = {0, 0, 0};

						// 사운드 영역의 중점 찾기
						WzVector wvCentre;
						if(BVT_AABB == pSoundVolume->m_BoundVolume.m_eBvType)
						{
							WzVector wvDiagonal = 
                                (pSoundVolume->m_BoundVolume.m_aabb.m_wvMax - pSoundVolume->m_BoundVolume.m_aabb.m_wvMin);
							wvCentre = 
                                pSoundVolume->m_BoundVolume.m_aabb.m_wvMin + (wvDiagonal * 0.5f);
						}
						else if(BVT_SPHERE == pSoundVolume->m_BoundVolume.m_eBvType)
						{
							wvCentre = pSoundVolume->m_BoundVolume.m_BvSphere.m_wvCenter;							
						}
						source_pos.x = wvCentre.x;
						source_pos.y = wvCentre.z;
						source_pos.z = wvCentre.y;

						SND_SOURCEDESC source_desc;
						source_desc.flObjectRadius = 0.0f;
						source_desc.flVolumeScale  = 1.0f;
						source_desc.flFrequency    = 0;
						source_desc.bForcePlay     = FALSE;
						source_desc.flRangeFactor  = 1.0f;
						source_desc.flMinDistance  = snd->m_fMinDist;//10.f;
						source_desc.flMaxDistance  = snd->m_fMaxDist;//100.f;

						// 3D 루프 사운드 0번을 시작 
						// 루프 사운드의 경우 핸들값을 저장해서,  이 핸들로  제어를 할 수 있다.
						int idx = snd->m_iSoundIdx;
						handle = wzsnd_play_3d( idx, // sound ID
								&listener_pos, // 리스너 위치
								&source_pos,	// 사운드 오브젝트 위치
								&source_velocity, // 사운드 오브젝트의 초당 이동 속도			
								TRUE,  //flag to indicate the sound should loop (default value 0)
								SND_INSTANCE_3,
								&source_desc);
						snd->m_iSoundHandle = handle;
					}
					else
					{
						WzVector wvCentre;
						if(BVT_AABB == pSoundVolume->m_BoundVolume.m_eBvType)
						{
							WzVector wvDiagonal = 
                                (pSoundVolume->m_BoundVolume.m_aabb.m_wvMax - pSoundVolume->m_BoundVolume.m_aabb.m_wvMin);
							wvCentre = 
                                pSoundVolume->m_BoundVolume.m_aabb.m_wvMin + (wvDiagonal * 0.5f);
						}
						else if(BVT_SPHERE == pSoundVolume->m_BoundVolume.m_eBvType)
						{
							wvCentre = pSoundVolume->m_BoundVolume.m_BvSphere.m_wvCenter;							
						}

						WzVector wvLen = wvCentre - wvFrom;
						float fDistSqr = VectorLength2(&wvLen);
						if( ((snd->m_fMaxDist*snd->m_fMaxDist) < fDistSqr) || 
                             (fDistSqr < (snd->m_fMinDist*snd->m_fMinDist)) )
						{
							wzsnd_stop(handle);
							snd->m_iSoundHandle = -1;
						}
					}
				}
			}
		}
	}

	// 비활성화 목록 처리
	pSndVolumes = m_pWorldBase->x_pTerrainSound2->Enum_DeactiveSoundArea();
	itor = pSndVolumes->begin();
	for(; itor != pSndVolumes->end(); ++itor)
	{
		St_SoundVolume* pSoundVolume = (*itor).second;
		StlVWzIDList::iterator itor2 = pSoundVolume->m_SoundDataList.begin();
		for(; itor2 != pSoundVolume->m_SoundDataList.end(); ++itor2)
		{
			St_SoundData* pSoundData = m_pWorldBase->x_pTerrainSound2->FindSoundData(*itor2);
			if(!pSoundData)
				continue;

			if(SDA_AMBIENT & pSoundData->m_Flags)
			{
				StlVSoundList::iterator itor3 = pSoundData->m_pAmbient->m_SoundList.begin();
				for(; itor3 != pSoundData->m_pAmbient->m_SoundList.end(); ++itor3)
				{
					int handle = (*itor3).m_iSoundHandle;
					if(-1 != handle)
					{
						wzsnd_stop_ambi(handle);  
						(*itor3).m_iSoundHandle = -1;
					}
				}
			}

			if(SDA_3DSOUND & pSoundData->m_Flags)
			{
				StlVSoundList::iterator itor3 = pSoundData->m_p3DSound->m_SoundList.begin();
				for(; itor3 != pSoundData->m_p3DSound->m_SoundList.end(); ++itor3)
				{
					int handle = (*itor3).m_iSoundHandle;
					if(-1 != handle)
					{
						wzsnd_stop(handle);
						(*itor3).m_iSoundHandle = -1;
					}
				}
			}
		}
	}
}

BOOL Map::IntersectPlayer_Ray_Bv(WzRay *pRay,float fLen)
{
	m_pIntersecBoundUnitArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(DTO_UNIT, pRay, m_pIntersecBoundUnitArray);

	float fT = 0.f;
	Object * pTargetObject = NULL;
	Player *pTargetPlayer = NULL;
	
	DWORD dwKey = 0;
	int iSize = m_pIntersecBoundUnitArray->GetCount();

	
	float fSelectDist = 1000.f;		// 현재 선택된 캐릭터와의 거리: 기본값은 max로 설정

	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundUnitArray->Get(i);
		pTargetObject = g_ObjectManager.GetObject(dwKey);

		if(!pTargetObject)
			continue;

		if (pTargetObject->IsKindOfObject(PLAYER_OBJECT) ) 
		{
			pTargetPlayer = (Player *)pTargetObject;

			if (pTargetPlayer == g_pHero)
			{
				continue;
			}

			if ( pTargetPlayer->IsDead())
			{
				// 죽은 캐릭터는 뺀다.						
				continue;
			}

			WORD wResult = NTCT_OUTSIDE;

			if (pTargetPlayer->GetNumPickingBoundingVolume())
			{
				WzBoundingVolume bv = *pTargetPlayer->GetPickingBoundingVolume(0);
			
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}
			else
			{
				WzBoundingVolume bv = *pTargetPlayer->GetBoundingVolume();
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}

			if (NTCT_OUTSIDE == wResult )  
			{
				pTargetPlayer = NULL;
				continue;
			}
			else
			{		
				// 특정 오브젝트가 걸렸냐?
				if(fT<=fLen)
				{
					return TRUE;
				}
			}			
		}
	}

	return FALSE;
}

BOOL Map::IntersectMapObject_Ray_Bv(WzRay *pRay,float fLen,float fIgnoreHeight,WzVector & vObjPos)
{
	m_pIntersecBoundUnitArray->InvalidateAll();

	GetWorldBase()->x_pOctree->Enumerate_IntersectRay(DTO_UNIT, pRay, m_pIntersecBoundUnitArray);

	Object * pTargetObject = NULL;
	MapObject * pMapObject = NULL;
	MapObject * pTempMapObject = NULL;
	DWORD dwKey = 0;
	float fT=0.0f;

	//맵 오브젝트 처리 
	int iSize = m_pIntersecBoundUnitArray->GetCount();
	for (int i = 0; i < iSize; i++)
	{
		dwKey = m_pIntersecBoundUnitArray->Get(i);

		pTargetObject = g_ObjectManager.GetObject(dwKey);

		assert(pTargetObject);
		if(!pTargetObject)
			continue;

		if ( pTargetObject->IsKindOfObject(MAP_OBJECT) ) 
		{
			pTempMapObject = (MapObject *)pTargetObject;

			if(pTempMapObject->IsTerrain()||pTempMapObject->IsTransparencyObj())
				continue;

			
			WORD wResult = NTCT_OUTSIDE;

			if (pTargetObject->GetNumPickingBoundingVolume())
			{
				WzBoundingVolume bv = *pTargetObject->GetPickingBoundingVolume(0);

				if (bv.m_eBvType == BVT_AABB)
				{
					float fHeight = (bv.m_aabb.m_wvMax.z - bv.m_aabb.m_wvMin.z);

					if(fHeight <= fIgnoreHeight)
						continue;
				}

			    if ((pRay->m_wvOrigin.x > bv.m_aabb.m_wvMin.x) && (pRay->m_wvOrigin.x <= bv.m_aabb.m_wvMax.x) &&
					(pRay->m_wvOrigin.y > bv.m_aabb.m_wvMin.y) && (pRay->m_wvOrigin.y <= bv.m_aabb.m_wvMax.y) &&
					(pRay->m_wvOrigin.z > bv.m_aabb.m_wvMin.z) && (pRay->m_wvOrigin.z <= bv.m_aabb.m_wvMax.z))
				{
					vObjPos=pTargetObject->GetPosition();
				
					return FALSE;
				}

				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}
			else
			{
				WzBoundingVolume bv = *pTargetObject->GetBoundingVolume();
				if (bv.m_eBvType == BVT_AABB)
				{
					float fHeight = (bv.m_aabb.m_wvMax.z - bv.m_aabb.m_wvMin.z);

					if (fHeight <= fIgnoreHeight)
						continue;
				}
				if ((pRay->m_wvOrigin.x > bv.m_aabb.m_wvMin.x) && (pRay->m_wvOrigin.x <= bv.m_aabb.m_wvMax.x) &&
					(pRay->m_wvOrigin.y > bv.m_aabb.m_wvMin.y) && (pRay->m_wvOrigin.y <= bv.m_aabb.m_wvMax.y) &&
					(pRay->m_wvOrigin.z > bv.m_aabb.m_wvMin.z) && (pRay->m_wvOrigin.z <= bv.m_aabb.m_wvMax.z))
				{
					vObjPos=pTargetObject->GetPosition();
					
					return FALSE;
				}
				wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
			}

			if (NTCT_OUTSIDE == wResult )  
			{
				pTempMapObject = NULL;
				continue;
			}
			else
			{
				// 특정 오브젝트가 걸렸냐?
				if(fT<=fLen)
				{
					vObjPos=pTargetObject->GetPosition();
					
					return TRUE;
				}
			}
		}

		pTempMapObject = NULL;
	}

	return FALSE;
}

BOOL Map::IntersectTree_Ray_Bv(WzRay *pRay,float fLen)
{
	float fT=0.0f;

	//트리 검사 

	SPEED_TREE_ITR itr =  m_SpeedTreeMap.begin();
	while(itr != m_SpeedTreeMap.end())
	{
		SPEED_TREE_INFO * pSpeedTreeInfo = (SPEED_TREE_INFO *)itr->second;

		CSpeedTreeDrawInfo * pSpeedTreeDrawInfo = 
			m_pDraw->x_pManagerSpeedTree->GetTreeByHandle(pSpeedTreeInfo->hHandle);

		int iCount = pSpeedTreeDrawInfo->GetCollisionObjectCount();
		WzBoundingVolume bv;
		BOOL bAlpha = FALSE;
		for (int a = 0; a < iCount; a++) 
        {
			bv = *pSpeedTreeDrawInfo->GetCollisionObject(a);

			WORD wResult = NTCT_OUTSIDE;
			wResult = Intersect_Ray_Bv( pRay, &bv ,&fT);	
			
			if(NTCT_OUTSIDE != wResult&&fT<=fLen)
			{
				return TRUE;
			}
		}			

		itr++;
	}

	return FALSE;
}

BOOL Map::IntersectTree( WzBoundingVolume* pBoundingVolume )
{
    SPEED_TREE_ITR itr =  m_SpeedTreeMap.begin();
    while(itr != m_SpeedTreeMap.end())
    {
        SPEED_TREE_INFO* pSpeedTreeInfo = (SPEED_TREE_INFO *)itr->second;

        CSpeedTreeDrawInfo* pSpeedTreeDrawInfo = m_pDraw->x_pManagerSpeedTree->GetTreeByHandle(pSpeedTreeInfo->hHandle);

        int iCount = pSpeedTreeDrawInfo->GetCollisionObjectCount();

        WzBoundingVolume bv;
        BOOL bAlpha = FALSE;
        for (int a = 0; a < iCount; a++) 
        {
            bv = *pSpeedTreeDrawInfo->GetCollisionObject(a);

            WORD wResult = NTCT_OUTSIDE;
            wResult = Intersect_Bv_Bv( pBoundingVolume, &bv );	

            if( NTCT_OUTSIDE != wResult )
            {
                return TRUE;
            }
        }			

        ++itr;
    }

    return FALSE;

}

BOOL Map::IntersectMapObject( WzBoundingVolume* pBoundingVolume )
{
    m_pIntersecBoundUnitArray->InvalidateAll();

    GetWorldBase()->x_pOctree->Enumerate_IntersectBoundingVolume(DTO_OBJECT, pBoundingVolume, m_pIntersecBoundUnitArray);


    //맵 오브젝트 처리 
    int iSize = m_pIntersecBoundUnitArray->GetCount();
    for (int i = 0; i < iSize; i++)
    {
        DWORD dwKey = m_pIntersecBoundUnitArray->Get(i);

        if( Object* pTargetObject = g_ObjectManager.GetObject(dwKey) )
        {
            if ( pTargetObject->IsKindOfObject(MAP_OBJECT) ) 
            {
                MapObject* pTempMapObject = (MapObject *)pTargetObject;
                if(pTempMapObject->IsTerrain() || pTempMapObject->IsTransparencyObj() )
                    continue;


                WORD wResult = NTCT_OUTSIDE;

                size_t PickingBoundingVolumeCount = pTargetObject->GetNumPickingBoundingVolume();
                for( size_t j=0;j!=PickingBoundingVolumeCount;++j)
                {
                    if( Intersect_Bv_Bv( pBoundingVolume, pTargetObject->GetPickingBoundingVolume(j) ) != NTCT_OUTSIDE )
                    {
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}



//-------------------------------------------------------------------------------------------
/**
*/
BOOL Map::GetSpawnHeroDirVector(const WzVector & rPos, WzVector & OUT rOutVecDir,float fLimit)
{
	if(m_vecDummyObjectPos.size() <= 0) 
		return FALSE;

	WzVector vecOffset;
	float fLength = fLimit;
	rOutVecDir.x = 0.0f;
	rOutVecDir.y = 0.0f;
	rOutVecDir.z = 0.0f;
	for( size_t ii=0; ii < m_vecDummyObjectPos.size(); ii++)
	{
		vecOffset =  m_vecDummyObjectPos[ii] - rPos;
		float fTempLength = VectorLength(&vecOffset);
		
		if( fLength > fTempLength)
		{
			rOutVecDir = m_vecDummyObjectPos[ii];
			fLength = fTempLength;
		}
	}

	if(fLength == fLimit) return FALSE;
	return TRUE;
}


BOOL Map::GetSpawnMonsterDirVector(const WzVector & rPos, WzVector & OUT rOutVecDir,float fLimit)
{
	float fLength = fLimit;
	WzVector vecOffset;
	rOutVecDir.x = 0.0f;
	rOutVecDir.y = 0.0f;
	rOutVecDir.z = 0.0f;
	if(m_vecDummyMonsterPos.size() <= 0) 
		return FALSE;

	for( size_t ii=0; ii < m_vecDummyMonsterPos.size(); ii++)
	{
		vecOffset =  m_vecDummyMonsterPos[ii] - rPos;
		float fTempLength = VectorLength(&vecOffset);
		
		if( fLength > fTempLength)
		{
			rOutVecDir = m_vecDummyMonsterPos[ii];
			fLength = fTempLength;
		}
	}

	if(fLength == fLimit) return FALSE;

	return TRUE;
}


BOOL Map::GetMapObjectAppearinfo(DWORD dwKey,APPEARANCEINFO *pInfo)
{
	MapObject_Appearance_Map_Itr	itr;
	itr = m_hashmapAppearance.find( dwKey );
	if (itr == m_hashmapAppearance.end())
	{		
		return FALSE;
	}
	else
	{
		*pInfo = itr->second;
		return TRUE;
	}
}

BOOL Map::IsField()
{
	if( const sMAPINFO* pMapInfo = GetMapInfo() )
	{
		if (eZONETYPE_FIELD == pMapInfo->byMKind)
		{
			return TRUE;
		}
	
	}

	return FALSE;
}

BOOL Map::IsVill()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
	{
		if (eZONETYPE_VILLAGE == pMapInfo->byMKind)
		{
			return TRUE;
		}

	}

	return FALSE;
}

BOOL Map::IsMission()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
	{
		if (eZONETYPE_MISSION == pMapInfo->byMKind)
		{
			return TRUE;
		}

	}

	return FALSE;
}

BOOL Map::IsTutorialField()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
	{
		if(eZONETYPE_TUTORIAL_FIELD == pMapInfo->byMKind)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Map::IsTutorialMap()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
	{
		if(pMapInfo->FCode[0]==611)
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool Map::IsAirShipMap()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
    {
        if( pMapInfo->byMKind == eZONETYPE_AIRSHIP_FIELD )
        {
            return true;
        }
    }

    return false;
}

BOOL Map::IsEsodeMap()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
    {
		if(pMapInfo->MapCode==20206)
		{
			return TRUE;
		}
	}

	return FALSE;

}

DWORD Map::GetCurMMCode()
{
    if( const sMAPINFO* pMapInfo = GetMapInfo() )
    {
        if( m_MapInfo.m_iFieldIndex >= 0 && m_MapInfo.m_iFieldIndex <= sMAPINFO::MAX_FIELD_NUM )
        {
            return pMapInfo->MMCode[m_MapInfo.m_iFieldIndex];
        }
    }
    return 0;
}

void Map::ApplyLightMap()
{
	// 라이트 맵을 로드한다.
	LPCSTR lpLightmap = GetWorldBase()->GetLightmapFileName();
	char szDrive[256];
	char szDir[256];
	char szBufStr[256];
	ZeroMemory(szDrive,sizeof(szDrive));
	ZeroMemory(szDir,sizeof(szDir));
	if(lpLightmap[0])
	{
		_splitpath( m_MapInfo.m_szFullFileName, szDrive, szDir, NULL, NULL); // 라이트맵의 경로 얻기

		if (szDrive[0] != 0)
		{
			sprintf(szBufStr, "%s\\%s\\%s\0", szDrive, szDir, lpLightmap); // 라이트맵 파일에 대한 FullPath얻기
		}
		else
		{
			sprintf(szBufStr, "%s%s\0", szDir, lpLightmap); // 라이트맵 파일에 대한 FullPath얻기
		}

		AttachLightMap(szBufStr); // 라이트 맵을 불러온다.
	}
}

void Map::AttachLightMap(LPSTR lpFilename)
{
	CLoadHelper* pLoadHelper = GetWorldBase()->x_pLoadHelper;

	MapObject *			pMapObject = NULL;
	size_t				i;
	char				szPath[_MAX_PATH] = { 0, };
	char				szStrBuf[_MAX_PATH] = { 0, };
	St_LHMeshGroupLM *	pLMUV = NULL; // LM메쉬그룹 리스트
	DWORD				dwNumLMMeshGroup = NULL; // LM메쉬그룹의 개수
	St_LHTerrainLM *	pTLM;
	DWORD				dwNumTLM;
	int					nNumTerrainMeshGroup = 0;
	St_UnitLightMap	*	pCurUnitLightmap = NULL;
#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
	UINT				uiTerrainMeshCount = 0;
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
	// 맵에 대한 WLU 파일을 읽어온다.	
	
	BOOL bRet = pLoadHelper->ParseWLU(lpFilename, &pLMUV, dwNumLMMeshGroup, &pTLM, dwNumTLM, g_pSunRenderer);

	if (!bRet)
	{
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	St_UnitLightMap *pUnitLightmap = new St_UnitLightMap[dwNumLMMeshGroup];

	for (i = 0; i < dwNumLMMeshGroup; ++i)
	{
		pCurUnitLightmap = pUnitLightmap + i;

		// IDX_LIGHTMAP_TERRAIN(==99999)는 지형에 대한 라이트맵이다. 
		// WLU파일의 앞부분은 지형의 라이트맵이고 뒷부분은 맵의 오브젝트에 대한
		// 라이트맵 정보이다.
		if(IDX_LIGHTMAP_TERRAIN == pLMUV[ i ].NumObj)
		{
#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
            ++uiTerrainMeshCount;
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

			nNumTerrainMeshGroup = max( nNumTerrainMeshGroup, pLMUV[ i ].NumMeshGroup );

			pCurUnitLightmap->m_hTexDiffuse		= INVALID_HANDLE_VALUE;
			pCurUnitLightmap->m_iNumVertices	= (int)pLMUV[ i ].NumLMVertex;
			pCurUnitLightmap->m_pUVVerts		= pLMUV[ i ].ListLMVertex;		
			pCurUnitLightmap->m_iNumObj		    = pLMUV[ i ].NumObj;
			pCurUnitLightmap->m_iNumMeshGroup	= pLMUV[ i ].NumMeshGroup;
			pCurUnitLightmap->m_FaceTextureNum	= pLMUV[ i ].FaceTextureNum;

			continue;
		}

#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
        // 오브젝트 로딩은 패스
        continue;
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

		// WLU파일과 맵의 오브젝트 인스턴스의 배열은 1:1로 된다고 정했기 때문에
		// 굳이 할필요가 없겠지만 안전검사 차원에서 확인한다.
		int index = 0;

		list<DWORD>::iterator it = m_listMapObjectKey.begin();		

		for(; it != m_listMapObjectKey.end(); ++it, ++index)
		{
			if(index == pLMUV[i].NumObj)
				break;
		}
		if(it == m_listMapObjectKey.end())
			continue;

		pMapObject = (MapObject*)g_ObjectManager.GetObject(*it);
		if(!pMapObject)
			continue;

		// 파일 전체경로 생성
		sprintf(szStrBuf, "%s\\%s\0", m_MapInfo.m_szFilePath, pLMUV[i].LM_Filename);

		HANDLE hLM = g_pSunRenderer->x_pManagerTexture->LoadTexture( szStrBuf, TLS_NEAREST | TLS_NO_MIPMAP);

		pCurUnitLightmap->m_hTexDiffuse		= hLM;
		pCurUnitLightmap->m_iNumVertices	= (int)pLMUV[i].NumLMVertex;
		pCurUnitLightmap->m_pUVVerts		= pLMUV[i].ListLMVertex;		
		pCurUnitLightmap->m_iNumObj			= pLMUV[i].NumObj;
		pCurUnitLightmap->m_iNumMeshGroup	= pLMUV[i].NumMeshGroup;
		pCurUnitLightmap->m_FaceTextureNum	= pLMUV[i].FaceTextureNum;

		if(pLMUV[ i ].NumMeshGroup == 0)
		{
			if (pMapObject && pMapObject->GetBasePart()->GetWzUnit())
			{				
                HANDLE hHandle = pMapObject->GetBasePart()->GetWzUnit()->GetWzdHandle();
                CWzdBase * pWzdBase = m_pDraw->x_pManagerWzdLoader->GetWzd(hHandle); 
                if ( pWzdBase )
                {
                    int CountMeshGroup = pWzdBase->GetNumberOfMeshGroup();

                    m_pDraw->x_pManagerUnitLightMap->AttachLightMap(
                        pMapObject->GetBasePart(), 
                        CountMeshGroup );
                }				
			}
		}

		if (pMapObject)
		{
			m_pDraw->x_pManagerUnitLightMap->SetLightMap(
                pMapObject->GetBasePart(), 
                pCurUnitLightmap, 
                pLMUV[i].NumMeshGroup );
		}
	}
	
#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
#else
	GetWorldBase()->AttachUnitLightmap(pUnitLightmap, dwNumLMMeshGroup); // 지형에 유닛을 위한 라이트 맵 정보를 등록한다.
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

	// 메모리 해제
	SAFE_DELETE_ARRAY(pUnitLightmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWzArray< St_TerrainLightMap* > arUnitLightMap;
	CWzArray< HANDLE > arTerrainLightmapHandle;
#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
    St_TerrainLightMap* pLightMapInfo = new St_TerrainLightMap[uiTerrainMeshCount];
#else
	St_TerrainLightMap* pLightMapInfo = new St_TerrainLightMap[nNumTerrainMeshGroup];
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
    
	int nCurrIndex = 0;

	HANDLE* phTLM = new HANDLE[dwNumTLM];
	for( i=0; i<dwNumTLM; ++i )
	{
		// 파일 전체경로 생성
		sprintf(szStrBuf, "%s%s\0",  m_MapInfo.m_szFilePath, pTLM[i].TerrainLMName);

		phTLM[i] = g_pSunRenderer->x_pManagerTexture->LoadTexture( szStrBuf, TLS_NO_MIPMAP);
		assert( phTLM[i] != INVALID_HANDLE_VALUE);
		arTerrainLightmapHandle.Add( phTLM[i] );
	}

	St_TerrainLightMap* pCurrLightMapInfo;
	
	int nCount = 0;
	for(i=0; i<dwNumLMMeshGroup; ++i)
	{
		if(IDX_LIGHTMAP_TERRAIN == pLMUV[i].NumObj)
		{
			pCurrLightMapInfo = &pLightMapInfo[nCurrIndex];
			pCurrLightMapInfo->m_iNumVertices    = (int)pLMUV[i].NumLMVertex;
			nCount += (int)pLMUV[i].NumLMVertex;
			pCurrLightMapInfo->m_pUVVerts		 = pLMUV[i].ListLMVertex;
			pCurrLightMapInfo->m_piLightmapIndex = pLMUV[i].FaceTextureNum;
			for( int j = arUnitLightMap.GetCount(); j < pLMUV[i].NumMeshGroup; ++j)
			{
				arUnitLightMap.Add( NULL);
			}
			arUnitLightMap.Add( pCurrLightMapInfo);
			nCurrIndex++;
		}
	}

	GetWorldBase()->AttachLightmap( &arUnitLightMap, &arTerrainLightmapHandle ); // 지형에 라이트 맵 정보를 붙인다.

	// 메모리 해제
	SAFE_DELETE_ARRAY( phTLM );
	SAFE_DELETE_ARRAY( pLightMapInfo );

	// 로더에서 할당한 메모리 해제.
	pLoadHelper->ReleaseTerrainWLU(); // 로더에서 가지고 있는 지형을 위한 라이트 맵 정보를 해제한다.
	pLoadHelper->ReleaseUnitWLU(); // 맵 오브젝트들에 대한 라이트맵 정보를 제거한다.

	// 맵에 라이트맵 이름을 등록한다.
	GetWorldBase()->SetLightmapFilename((LPCSTR)lpFilename);
}

void Map::DetachLightMap()
{
	if( m_pWorldBase )
	{
		m_pWorldBase->DetachLightmap();
		m_pWorldBase->SetLightmapFilename(NULL); // 맵에 연결된 라이트 맵 이름을 지운다.
	}
}

WzVector Map::GetGrassWave( const WzVector& wvPos, const WzVector& wvRotOriginal, DWORD dwCurrentTick, int iType)
{
	WzVector wvGrassWave;
	wvGrassWave.x = 0.0f;

	// xy 회전
	int iAngle = FLOAT2INT(((wvPos.y * 400.0f) + dwCurrentTick) * 0.0515662f);	// y 좌표에 따라 약간씩 다르게
	wvGrassWave.y = m_fFieldTable[iAngle % 360];

	// z 회전
	switch (iType)
	{
	case 0:
		wvGrassWave.z = wvRotOriginal.z;
		break;

	case 1:
        {
            const St_Camera* pCamInfo = g_pSunRenderer->GetCameraInfo();
            assert( pCamInfo );

            if( pCamInfo )
            {
		        if ( pCamInfo->m_wvDir.x == 0.0f && pCamInfo->m_wvDir.y == 0.0f)
		        {
			        wvGrassWave.z = 0.0f;
		        }
		        else
		        {
			        wvGrassWave.z = Math_GetAngleFromVector( &pCamInfo->m_wvDir);	// 카메라 바라보게 하기
		        }
            }
        }
        break;
	}

	return wvGrassWave;
}

