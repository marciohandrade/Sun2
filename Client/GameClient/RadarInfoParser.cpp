#include "SunClientPrecompiledHeader.h"
#include "RadarInfoParser.h"

#include <SolarFileReader.h>

#include "GlobalFunc.h"


RadarInfoParser::RadarInfoParser() : m_pDataTable ( NULL )
{
	m_hRadarTexture = INVALID_HANDLE_VALUE;
	for( int i = 0 ; i < eRADAR_POINT_COUNT ; ++i )
	{
		m_pRadarTextureInfo[i] = NULL;
	}
	m_RadarTextureSize.x = 0;
	m_RadarTextureSize.y = 0;
}

RadarInfoParser::~RadarInfoParser()
{
}

VOID RadarInfoParser::Release()
{
	Unload();
	if( m_pDataTable )
	{
		delete m_pDataTable;
		m_pDataTable = NULL;
	}
}

VOID RadarInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<RECT *>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


VOID RadarInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<RECT *>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID RadarInfoParser::Unload()
{
	if( m_pDataTable == NULL )
	{
		return;
	}

	RECT	*pInfo = NULL;
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();

    if (m_hRadarTexture != INVALID_HANDLE_VALUE)
    {
	    g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hRadarTexture);
	    m_hRadarTexture = INVALID_HANDLE_VALUE;
    }

	for( int i = 0 ; i < eRADAR_POINT_COUNT ; ++i )
	{
		m_pRadarTextureInfo[i] = NULL;
	}
}

POINT RadarInfoParser::GetTextureSize( void )
{
	return m_RadarTextureSize;
}

RECT *RadarInfoParser::GetRadarImgInfo( int iIndex )
{
	return m_pDataTable->GetData( iIndex );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL RadarInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	int iIndex = 0;
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		RECT*   pInfo = new RECT;

		iIndex = GetDataNumber("Index", nRow);
		pInfo->left = GetDataNumber("X", nRow);
		pInfo->top = GetDataNumber("Y", nRow);
		pInfo->right = GetDataNumber("Width", nRow);
		pInfo->bottom = GetDataNumber("Height", nRow);

		m_pDataTable->Add(pInfo , iIndex);
	}

	//------------------------------------------------------------------------------------------
	//		텍스쳐 로딩
	//------------------------------------------------------------------------------------------


	if (m_hRadarTexture==INVALID_HANDLE_VALUE)
	{
		m_hRadarTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\all_radarpoint.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

		St_TextureContainer* pTextureInfo;

		pTextureInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(m_hRadarTexture);

		if(pTextureInfo)
		{
			m_RadarTextureSize.x = pTextureInfo->m_nImageWidth;
			m_RadarTextureSize.y = pTextureInfo->m_nImageHeight;
		}
		
	}

	return TRUE;
}



#pragma warning ( pop )
