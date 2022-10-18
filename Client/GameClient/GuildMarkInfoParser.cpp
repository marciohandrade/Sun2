#include "SunClientPrecompiledHeader.h"
#include "GuildMarkInfoParser.h"


GuildMarkInfoParser::GuildMarkInfoParser():	m_pDataTable ( NULL )
{
}

GuildMarkInfoParser::~GuildMarkInfoParser()
{
	assert( m_pDataTable == NULL);
}

VOID GuildMarkInfoParser::Release()
{
	Unload();
	if (m_pDataTable)
		delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID GuildMarkInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_GuildMarkInfo *>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID GuildMarkInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID GuildMarkInfoParser::Unload()
{
	BASE_GuildMarkInfo * pInfo = NULL;
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

BASE_GuildMarkInfo * GuildMarkInfoParser::GetGuildMarkInfo(DWORD dwKey)
{
	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL GuildMarkInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	for(int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_GuildMarkInfo * pInfo = new BASE_GuildMarkInfo;

		pInfo->dwIndex = GetDataNumber("index", nRow);
		pInfo->byMainCategory = GetDataNumber("L_Category", nRow);
		pInfo->bySubCategory = GetDataNumber("S_Category", nRow);
		pInfo->iRow =	GetDataNumber("X_Coordinate", nRow);
		pInfo->iColumn = GetDataNumber("Y_Coordinate", nRow);

		assert(NULL == m_pDataTable->GetData(pInfo->dwIndex));
		m_pDataTable->Add(pInfo, pInfo->dwIndex);
	}

	return TRUE;
}


#pragma warning ( pop )

