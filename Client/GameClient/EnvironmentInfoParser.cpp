#include "SunClientPrecompiledHeader.h"
#include "EnvironmentInfoParser.h"
#include <SolarFileReader.h>
EnvironmentInfoParser::EnvironmentInfoParser():	m_pDataTable ( NULL )
{
}
EnvironmentInfoParser::~EnvironmentInfoParser()
{
	assert( m_pDataTable == NULL);
}
VOID EnvironmentInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}
VOID EnvironmentInfoParser::Init(DWORD dwPoolSize,char * pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_EnvironmentInfo*>;
	m_pDataTable->Initialize( dwPoolSize );

	SetPackFileName( pszPackFileName );
}

VOID EnvironmentInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_EnvironmentInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}


VOID EnvironmentInfoParser::Unload()
{
	BASE_EnvironmentInfo * pInfo = NULL;
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
BASE_EnvironmentInfo* EnvironmentInfoParser::GetEnvironmentInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL EnvironmentInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_EnvironmentInfo* pInfo = new BASE_EnvironmentInfo;

		//wCode
		pInfo->m_dwKey			=	GetDataDWORD("wCode",nRow);
		
		//bAmbientR
		pInfo->m_bAmbientR		=	GetDataBYTE("bAmbientR",nRow);	
	
		//bAmbientG
		pInfo->m_bAmbientG		=	GetDataBYTE("bAmbientG",nRow);	

		//bAmbientB
		pInfo->m_bAmbientB		=	GetDataBYTE("bAmbientB",nRow);	

		//bSunLightR
		pInfo->m_bSunLightR		=	GetDataBYTE("bSunLightR",nRow);	

		//bSunLightG
		pInfo->m_bSunLightG		=	GetDataBYTE("bSunLightG",nRow);	

		//bSunLightB
		pInfo->m_bSunLightB		=	GetDataBYTE("bSunLightB",nRow);	

		//fSunLightDirX
		pInfo->m_fSunLightDirX	=	GetDataFloat("fSunLightDirX",nRow);	

		//fSunLightDirY
		pInfo->m_fSunLightDirY	=	GetDataFloat("fSunLightDirY",nRow);	

		//fSunLightDirZ
		pInfo->m_fSunLightDirZ	=	GetDataFloat("fSunLightDirZ",nRow);	

		assert(NULL == m_pDataTable->GetData(pInfo->m_dwKey));
		m_pDataTable->Add(pInfo, pInfo->m_dwKey);
	}
    
	return TRUE;
}



#pragma warning ( pop )
