#include "StdAfx.h"
#include <ProgramCommon/wzDataType.h>
#include <ProgramCommon/DiscAccess.h>
#include <ProgramCommon/WzArchive.h>
#include "AreaTileLoadHelper.h"
#include "Define.h"


//----------------------------------------------------------------------------
/**
*/
CAreaTileHelper::CAreaTileHelper()
{
	m_pAreaTileInfoHashTable = new util::SolarHashTable<AREA_TILE_INFO *>;
	m_pAreaTileInfoHashTable->Initialize( 100 );
	m_TotalTileCount = 0;
}

//----------------------------------------------------------------------------
/**
*/
CAreaTileHelper::~CAreaTileHelper()
{
    if (m_pAreaTileInfoHashTable)
    {
        m_pAreaTileInfoHashTable->SetFirst();
        while (AREA_TILE_INFO* tile_info = m_pAreaTileInfoHashTable->GetNext())
        {
            SAFE_DELETE(tile_info);
        }
        m_pAreaTileInfoHashTable->RemoveAll();

        SAFE_DELETE(m_pAreaTileInfoHashTable);
    }
    m_TotalTileCount = 0;
}

//----------------------------------------------------------------------------
/**
*/
BOOL CAreaTileHelper::SaveAreaTile( TCHAR* szFileName )
{
	CWzArchive saveArchive;

	if(!saveArchive.SaveFile(szFileName))
	{
		return FALSE;
	}
	
	DWORD dwVersion = VERSION_AREA_TILE;
	saveArchive << dwVersion;

	int iAreaNum=m_pAreaTileInfoHashTable->GetDataNum();

	(saveArchive) << iAreaNum;							//공간 갯수 

    m_pAreaTileInfoHashTable->SetFirst();
    while (AREA_TILE_INFO* area_info = m_pAreaTileInfoHashTable->GetNext())
    {
        saveArchive << area_info->iAreaID;               //공간 번호 
        saveArchive << area_info->wNumberOfTiles;        //타일 갯수 
        for (int i = 0; i < area_info->wNumberOfTiles; ++i)
        {
            saveArchive << area_info->piTileIndex[i];    //타일 인덱스
        }
    }
    saveArchive.Close();
    return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL CAreaTileHelper::LoadAreaTile( TCHAR* szFileName, CDiscAccess* pDiscAccess )
{
	DWORD			dwVersion;
	int             iAreaNum;
	CWzArchive		loadArchive;

    if( pDiscAccess != NULL )
    {
        loadArchive.SetDiscAccess( pDiscAccess );
    }

	if( !loadArchive.LoadFile( szFileName))
		return FALSE;

	loadArchive >> dwVersion;
	loadArchive >> iAreaNum;							//공간 갯수 
	for(int i=0;i<iAreaNum;i++)
	{
		AREA_TILE_INFO * pAreaInfo = new AREA_TILE_INFO;
		loadArchive >>pAreaInfo->iAreaID;				//공간 번호 
		loadArchive >>pAreaInfo->wNumberOfTiles;		//타일 갯수 
		pAreaInfo->piTileIndex = new int[pAreaInfo->wNumberOfTiles];
		for(int j=0;j<pAreaInfo->wNumberOfTiles;j++)
		{
			loadArchive >>pAreaInfo->piTileIndex[j];	//타일 인덱스
		}
		m_TotalTileCount += pAreaInfo->wNumberOfTiles;
		m_pAreaTileInfoHashTable->Add( pAreaInfo, pAreaInfo->iAreaID );

	}
	loadArchive.Close();
	return TRUE;
}

/*
BOOL Map::LoadMapWAT(CAreaTileHelper * pAreaTileLoadHelper)
{
	char szBufStr[_MAX_PATH] = { 0, };
	char szDrive[_MAX_PATH] = { 0, };
	char szDir[_MAX_PATH] = { 0, };
	char szFName[_MAX_FNAME] = { 0, };
	char szExt[_MAX_EXT] = { 0, };
	char szWATFileName[MAX_PATH]= { 0, };

	_splitpath(GetFullFileName(), szDrive, szDir, NULL, NULL);
	ZeroMemory(m_szFilePath,sizeof(m_szFilePath));

	if (szDrive[0] == 0)
	{
	_stprintf(m_szFilePath,_T("%s"),szDir);
	}
	else 
	{
	_stprintf(m_szFilePath,_T("%s\\%s"),szDrive,szDir);
	}

	StrCopy(szWATFileName, m_szFullFileName);	
	szWATFileName[strlen(m_szFullFileName)- 3] = 0;
	strcat(szWATFileName, "wat");

	CDiscAccess discAcess;//파일이 있나 검사 
	DA_FILE dfFile=discAcess.OpenFile(EDI_ACTIVE, szWATFileName, DA_READ, DA_READ, SM_READ | SM_WRITE , OD_OPEN_EXISTING);

	if ( INVALID_DA_FILE == dfFile)
	{
	return FALSE;
	}

	discAcess.CloseFile( dfFile);

	return pAreaTileLoadHelper->LoadAreaTile(szWATFileName);
}
*/