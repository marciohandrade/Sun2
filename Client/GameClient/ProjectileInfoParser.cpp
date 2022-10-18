#include "SunClientPrecompiledHeader.h"
#include "ProjectileInfoParser.h"
#include <SolarFileReader.h>
ProjectileInfoParser::ProjectileInfoParser() :	m_pDataTable ( NULL )
{

}
ProjectileInfoParser::~ProjectileInfoParser()
{
}

VOID ProjectileInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID ProjectileInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_ProjectileInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID ProjectileInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_ProjectileInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID ProjectileInfoParser::Unload()
{
	BASE_ProjectileInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}
BASE_ProjectileInfo* ProjectileInfoParser::GetProjectileInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL ProjectileInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_ProjectileInfo* pInfo = new BASE_ProjectileInfo;
		
		//dwKey
		pInfo->m_dwKey				= GetDataDWORD("dwKey", nRow);
		
		//dwImageID
		pInfo->m_dwImageID			= GetDataDWORD("dwImageID", nRow);
		
		//dwFlyingEffectID
		strncpy(pInfo->m_wzidFlyingEffectID.code, GetDataString("dwFlyingEffectID", nRow), 4);
		
		if (pInfo->m_wzidFlyingEffectID.id == StrToWzID("NULL"))
		{
			pInfo->m_wzidFlyingEffectID.id = 0;
		}
		
		//dwHitEffectID
		strncpy(pInfo->m_wzidHitEffectID.code, GetDataString("dwHitEffectID", nRow), 4);
		if (pInfo->m_wzidFlyingEffectID.id == StrToWzID("NULL"))
		{
			pInfo->m_wzidFlyingEffectID.id = 0;
		}	

		//bMakeTrail
		pInfo->m_bMakeTrail			= GetDataBYTE("bMakeTrail", nRow);

		//dwProjectileType
		pInfo->m_dwProjectileType	= GetDataDWORD("dwProjectileType", nRow);
		
		//dwMoveTime
		pInfo->m_fSpeed				= GetDataFloat("dwMoveTime",nRow) /1000.0f;
		
		assert(NULL == m_pDataTable->GetData(pInfo->m_dwKey));
		m_pDataTable->Add(pInfo, pInfo->m_dwKey);
	}

	return TRUE;
}




#pragma warning ( pop )
