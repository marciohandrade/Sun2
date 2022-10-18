#include "SunClientPrecompiledHeader.h"

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

#include "SpaZoneInfoParser.h"
#include <SolarFileReader.h>
SpaZoneInfoParser::SpaZoneInfoParser() :	m_pDataTable ( NULL )
{

}
SpaZoneInfoParser::~SpaZoneInfoParser()
{
}

VOID SpaZoneInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID SpaZoneInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<SpaZoneInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID SpaZoneInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<SpaZoneInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID SpaZoneInfoParser::Unload()
{
	SpaZoneInfo * pInfo = NULL;
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

SpaZoneInfo* SpaZoneInfoParser::GetSpaZoneInfo(DWORD map_code)
{
    return m_pDataTable->GetData(map_code);
}

SpaZoneInfoParser::SpaAnimationStyle SpaZoneInfoParser::GetSpaAnimationStyle(DWORD map_code)
{
    SpaZoneInfo* spa_info = m_pDataTable->GetData(map_code);
    
    if (spa_info)
    {
        if (spa_info->style_type > AnimationStyle_None &&
            spa_info->style_type < AnimationStyle_End)
        {
            return static_cast<SpaZoneInfoParser::SpaAnimationStyle>(spa_info->style_type);
        }
    }

    return AnimationStyle_None;
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL SpaZoneInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		SpaZoneInfo* pInfo = new SpaZoneInfo;
		
		pInfo->index				= GetDataBYTE("Index", nRow);
		pInfo->map_code             = GetDataDWORD("MapCode", nRow);
        pInfo->style_type           = GetDataDWORD("Type", nRow);
		
        for (int i = 0; i < 4; i++)
        {
            TCHAR temp_string[16];
            _snprintf(temp_string, 16, "AnimationCode%d", i+1);

            strncpy(pInfo->special_animation[i].code, GetDataString(temp_string, nRow), 4);

            if (pInfo->special_animation[i].id == StrToWzID("NULL"))
            {
                pInfo->special_animation[i].id = 0;
            }
        }
		
        pInfo->base_costume_code    = GetDataDWORD("CostumeItem", nRow);


		assert(NULL == m_pDataTable->GetData(pInfo->map_code));
		m_pDataTable->Add(pInfo, pInfo->map_code);
	}

	return TRUE;
}




#pragma warning ( pop )

#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION