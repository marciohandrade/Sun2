#include "SunClientPrecompiledHeader.h"
#include "GraphicResourceList.h"

GraphicResourceList::GraphicResourceList()
{
	m_pResourceHashTable = NULL;
}

GraphicResourceList::~GraphicResourceList()
{
	;
}

VOID GraphicResourceList::Release()
{
	if( m_pResourceHashTable )
	{
		Unload();
		delete m_pResourceHashTable;
		m_pResourceHashTable = NULL;
	}
}

VOID GraphicResourceList::Init( DWORD dwItemInfoPoolSize )
{
	assert( m_pResourceHashTable == NULL );
	m_pResourceHashTable = new util::SolarHashTable<RESOURCE_INFO *>;
	m_pResourceHashTable->Initialize( dwItemInfoPoolSize );
}


VOID GraphicResourceList::Init( DWORD dwItemInfoPoolSize, char * pszPackFileName )
{
	Init( dwItemInfoPoolSize );
	SetPackFileName( pszPackFileName );
}


VOID GraphicResourceList::Unload()
{
	if( m_pResourceHashTable )
	{
		RESOURCE_INFO * pInfo = NULL ;
		m_pResourceHashTable->SetFirst();
		while( pInfo = m_pResourceHashTable->GetNext() )
		{
			delete pInfo;
		}
		m_pResourceHashTable->RemoveAll();
	}
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL GraphicResourceList::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	
	static DWORD dwCode = 0;
	
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
        dwCode = GetDataDWORD("dwCode", nRow);

        if (NULL != m_pResourceHashTable->GetData(dwCode))
        {
            TCHAR szText[512];
            Sprintf(szText, _T("GraphicResourceList : 중복 코드 %ld"), dwCode);
            MessageBox(NULL, szText, _T("GraphicResourceList : 중복 코드(기획자님 확인바랍니다!!)"), MB_OK);

            continue;
        }

		RESOURCE_INFO* pInfo = new RESOURCE_INFO;

        pInfo->dwCode = dwCode;
		
		//szFileName
		strncpy(pInfo->szFilename, GetDataString("szFileName", nRow), MAX_RESOURE_FILENAME);
				
		pInfo->dwTotalRefCount = 0;

		m_pResourceHashTable->Add(pInfo, pInfo->dwCode);
	}

	return TRUE;

}

BOOL GraphicResourceList::AddItemInfo( CODETYPE ItemCode, char* pszResourceFileName )
{
    if (NULL != m_pResourceHashTable->GetData(ItemCode))
    {
        assert(!"GraphicResourceList::AddItemInfo() ItemCode 중복");
        return FALSE;
    }

    RESOURCE_INFO* pInfo = new RESOURCE_INFO;

    pInfo->dwCode = ItemCode;
    pInfo->dwTotalRefCount = 0;
    strncpy(pInfo->szFilename, pszResourceFileName, MAX_RESOURE_FILENAME);

    m_pResourceHashTable->Add(pInfo, pInfo->dwCode);

    return TRUE;
}
