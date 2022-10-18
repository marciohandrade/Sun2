#include "SunClientPrecompiledHeader.h"
#include "MapUnitCacher.h"
#include <SCSlotHeader.h>
#include <ProgramCommon/WzArchive.h>
#include "ObjectManager.h"
#include "SoundEffect.h"
#include "psapi.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"



#ifdef _DEBUG
extern DWORD g_AllocSize;
DWORD MemorySizeCheck(char * FuncName)
{

	return g_AllocSize;
}
#endif

MapUnitCacher::MapUnitCacher()
	:	m_pDraw ( NULL ),
		m_dwUniqueID ( 0 )
{
	m_pMapUnitDrawHashTable = new util::SolarHashTable<MapWzUnitDraw *>;
	m_pMapUnitDrawHashTable->Initialize( 10000 );



	m_pMapUnitHashTable = new util::SolarHashTable<MapWzUnit *>;
	m_pMapUnitHashTable->Initialize( 10000 );

	m_pWzUnitPool = new util::CMemoryPoolFactory<MapWzUnit>;
	m_pWzUnitPool->Initialize( 1000, 500 );
}

MapUnitCacher::~MapUnitCacher()
{
	UnloadAll();

	SAFE_DELETE( m_pMapUnitHashTable );
	SAFE_DELETE( m_pMapUnitDrawHashTable );

	if (m_pWzUnitPool) {
		m_pWzUnitPool->Release();
		delete m_pWzUnitPool;
		m_pWzUnitPool = NULL;
	}
}

VOID MapUnitCacher::UnloadAll()
{
	if( m_pMapUnitHashTable )
	{	
		MapWzUnit * pWzUnit = NULL;
		m_pMapUnitHashTable->SetFirst();
		while( pWzUnit = m_pMapUnitHashTable->GetNext() )
		{
			m_pDraw->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
			m_pWzUnitPool->Free( pWzUnit );
		}
		m_pMapUnitHashTable->RemoveAll();
	}

	// CWzUnitDraw 
	if( m_pMapUnitDrawHashTable )
	{
		MapWzUnitDraw * pWzUnitDraw = NULL;
		m_pMapUnitDrawHashTable->SetFirst();
		while( pWzUnitDraw = m_pMapUnitDrawHashTable->GetNext() )
		{
			g_pSunRenderer->DestroyResourceForWzUnitDraw(pWzUnitDraw->pUnitDraw);

			Func_ContactWzd( CW_DELETE_WZUNITDRAW, pWzUnitDraw->pUnitDraw );

			delete pWzUnitDraw;
		}
		m_pMapUnitDrawHashTable->RemoveAll();
	}
}

VOID MapUnitCacher::Init( CDrawBase * pDraw)
{
	m_pDraw			= pDraw;
}

VOID MapUnitCacher::LoadMapGrassUnit( VOID )
{
	char PACK_FILE_NAME[]	= "Data\\Resource.wpk";
	char GRASS_FILE_PATH[]	= "Data\\Map\\Grass\\";
	char GRASS_FILE_NAME[]	= "Data\\Map\\Grass\\grass.txt";

	GlobalFunc::WriteLoadingTimeLog(_T("    MapUnitCacher::LoadMapGrassUnit Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	// open
	util::SolarFileReader	sr;
	BOOL					bResult = FALSE;

	if( GENERALPARAM->IsUsePackFile() )
	{
		bResult = sr.OpenFileFromPack(PACK_FILE_NAME, GRASS_FILE_NAME, SEPERATOR_WHITESPACE_ONLY_STRING);
	}
	else
	{
		bResult = sr.OpenFile( GRASS_FILE_NAME, SEPERATOR_WHITESPACE_ONLY_STRING );
	}

	if ( !bResult )
	{
        WZLOG( WzLogType::WZERR, "풀 스크립트 파일(grass.txt)을 읽을 수 없습니다." );
		return;
	}

	// read
	char	pszGrassID[5], pszUnitFile[MAX_PATH];
	DWORD	wiMapUnitID;



	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
			break;

		StrnCopy( pszGrassID, sr.GetTokenString(), 5 );
		wiMapUnitID = StrToWzID( pszGrassID );

		sr.GetNextTokenType();		
		sprintf( pszUnitFile, "%s%s", GRASS_FILE_PATH, sr.GetTokenString() );
        
        DWORD load_style = ULS_DEFAULT;

        load_style |= (ULS_NO_MULTITHREAD | ULS_INSTANCING);

        if (wiMapUnitID != m_pDraw->x_pManagerWzUnitLoader->LoadWzUnit(wiMapUnitID, pszUnitFile, TRUE, load_style))
		{
			TCHAR temp[INTERFACE_STRING_LENGTH] = {0, };
			sprintf(temp, "맵유닛 %s가 없음.", pszUnitFile);
            WZLOG( WzLogType::WZERR, temp);
		}

        CWzUnit * pWzUnit = m_pDraw->x_pManagerWzUnitLoader->GetWzUnit( wiMapUnitID );

		MapWzUnit * pUnit	= (MapWzUnit *) m_pWzUnitPool->Alloc();
		pUnit->m_pwzUnit	= pWzUnit;
		pUnit->m_wiID		= wiMapUnitID;

		if(pszGrassID[0]=='G' && pszGrassID[1]=='R')
		{
			pUnit->m_iPhysicsType = 1;

		}
		else
		{
			pUnit->m_iPhysicsType = 0;

		}

			
		WzAssert( NULL == m_pMapUnitHashTable->GetData( wiMapUnitID ) );
		m_pMapUnitHashTable->Add( pUnit, wiMapUnitID );
	}



	sr.CloseFile();

	GlobalFunc::EndTimeCheck(dwStartTime, false, _T("    MapUnitCacher::LoadMapGrassUnit End"));

}

//=============================================================================================================================
/**
	@remarks
			현재 위치의 color값을 가져옴
	@param	WzVector * pWvPos
			color값을 가져올 위치
*/
//=============================================================================================================================
WzColor MapUnitCacher::_getTerrainColor( CWorldBase * pWorldBase, WzVector * pWvPos )
{
	float fT = 0.f;
	int iTerrainMeshStanding = pWorldBase->PathExp_GetMeshToStand( *pWvPos, &fT, 5.0f );
	if ( iTerrainMeshStanding < 0 || fT > 5.0f )
	{
		return WzColor_RGB( 255, 255, 255);
	}
	else
	{
		return pWorldBase->GetTerrainLight( *pWvPos, iTerrainMeshStanding );
	}
}

VOID MapUnitCacher::LoadMapGrassUnitDraw( CWorldBase * pWorldBase )
{
	CLoadHelper * pLoadHelper = pWorldBase->x_pLoadHelper;

	MapWzUnitDraw * pMapUnitDraw = NULL;

	WzID wzUnitID = 0;
	DWORD dwUnitNum = 0;
	DWORD dwGrassKey = 0;
	
	dwUnitNum = pLoadHelper->GetGrassWZUCount();

	WzVector vPos,vScale;

	St_LHGrassWZU* pGrassWzu = pLoadHelper->GetGrassWZU();
	for( size_t i = 0; i < dwUnitNum; ++i )
	{
		char temp[256] = {0,};

		wzUnitID = pGrassWzu[i].m_WzID;
		int iSize = pGrassWzu[i].m_nNumGrass;
		
		MapWzUnit * pMapWzUnit = m_pMapUnitHashTable->GetData( wzUnitID );
		if( !pMapWzUnit )
		{
			WzIDToString(temp, wzUnitID);
			// %s : 파일 로드 실패 6864
			sprintf(temp, g_InterfaceManager.GetInterfaceString( 6864 ), temp);

			MessageBoxA(g_hWndMain, temp, "Error", MB_ICONERROR);
            WzAssert( !temp );
			return;
		}

		MapWzUnitDraw * pMapWzUnitDraw = NULL;

		for ( int j = 0; j < iSize; j++)
		{			
			St_LHGrass * pSrc = &pGrassWzu[i].m_pGrass[j];

			pMapWzUnitDraw = new MapWzUnitDraw;
			pMapWzUnitDraw->wzId = wzUnitID;
			pMapWzUnitDraw->dwUniqueID	= MAP_GRASS_KEY_START + dwGrassKey++;
		
			vPos = pSrc->m_wvPos;
			pMapWzUnitDraw->wzRot = pSrc->m_wvRot;
			vScale = pSrc->m_wvScale;			

			pMapWzUnitDraw->byAttr = pSrc->m_byAttribute;

            DWORD dwInitFlags = UDI_NONE;

            dwInitFlags |= UDI_UNIT_GRASS;

            pMapWzUnitDraw->pUnitDraw	= (CWzUnitDraw *) Func_ContactWzd( CW_NEW_WZUNITDRAW, &dwInitFlags );


			pMapWzUnitDraw->pUnitDraw->SetWzUnit( pMapWzUnit->m_pwzUnit, FALSE );
			pMapWzUnitDraw->pUnitDraw->SetScale( vScale );	
			pMapWzUnitDraw->pUnitDraw->SetPosition( vPos , FALSE); // 바운딩 볼륨을 쓸데없이 두번 업뎃하지 않는다!
			pMapWzUnitDraw->pUnitDraw->SetRotation( pMapWzUnitDraw->wzRot );
			pMapWzUnitDraw->pUnitDraw->EnableUpdateLight(FALSE);	
			pMapWzUnitDraw->pUnitDraw->EnableLastAlphaRender(TRUE);

			WzVector wvPos=pMapWzUnitDraw->pUnitDraw->GetPosition();

			pMapWzUnitDraw->wzColor = 
				_getTerrainColor( pWorldBase, &wvPos);

			pWorldBase->x_pOctree->AddObject( 
						DTO_OBJECT, 
						(DWORD)pMapWzUnitDraw->dwUniqueID, 
						&pMapWzUnitDraw->pUnitDraw->GetBoundingVolume());

			pMapWzUnitDraw->iPhysicsType = pMapWzUnit->m_iPhysicsType;

			WzAssert( NULL == m_pMapUnitDrawHashTable->GetData( pMapWzUnitDraw->dwUniqueID ) );
			m_pMapUnitDrawHashTable->Add( pMapWzUnitDraw, pMapWzUnitDraw->dwUniqueID );
		}
	}
}

CWzUnit * MapUnitCacher::GetMapUnit( WzID dwMapUnitID )
{
	MapWzUnit * pUnit = m_pMapUnitHashTable->GetData( dwMapUnitID );
	if( !pUnit )
	{
		return NULL;
	}
	return pUnit->m_pwzUnit;
}

MapWzUnitDraw * MapUnitCacher::GetMapUnitDraw( DWORD dwID )
{
	return m_pMapUnitDrawHashTable->GetData( dwID );
}
VOID MapUnitCacher::SetFirst()
{
	m_pMapUnitDrawHashTable->SetFirst();
}

MapWzUnitDraw * MapUnitCacher::GetNext()
{
	return m_pMapUnitDrawHashTable->GetNext();
}

