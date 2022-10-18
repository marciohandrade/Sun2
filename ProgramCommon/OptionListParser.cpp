//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/02/23
//------------------------------------------------------------------------------
//#include "SunClientPrecompiledHeader.h"
#include "StdAfx.h"
#include "OptionListParser.h"
#include <SolarFileReader.h>
#include "ItemOptionHeader.h"

//----------------------------------------------------------------------------                   	
/**                    
*/
OptionListParser::OptionListParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
OptionListParser::~OptionListParser()
{
   assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID OptionListParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID OptionListParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_OptionList*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID OptionListParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_OptionList*>;
	m_pDataTable->Initialize( dwPoolSize );
}
//----------------------------------------------------------------------------                   	
/**                    
*/
void OptionListParser::Unload()
{
    if (m_pDataTable == NULL) {
        return;
    };
    m_pDataTable->SetFirst();
    while (BASE_OptionList* info = m_pDataTable->GetNext()) {
        delete info;
    }
    m_pDataTable->RemoveAll();
    option_list_map_.clear();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_OptionList* OptionListParser::GetOptionList(DWORD dwIndex)
{
	return m_pDataTable->GetData( dwIndex );
}

//----------------------------------------------------------------------------                   	
/**
*/
BASE_OptionList* OptionListParser::GetOptionListByAttr(const eATTR_TYPE attr_type)
{
    if (option_list_map_.find(attr_type) == option_list_map_.end())
    {
        return NULL;
    }
    return static_cast<BASE_OptionList*>(option_list_map_[attr_type]);
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL OptionListParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch (ScriptCode)
	{
	case SCRIPT_OPTION_LIST_INFO:
		return _Load(bReload);
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL OptionListParser::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    option_list_map_.clear();

	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		// 스크립트에서 읽어온 정보
		BASE_OptionList* pInfo = new BASE_OptionList;

		pInfo->mdwIndex = GetDataDWORD("Index", nRow);
		pInfo->mdwNameCode = GetDataDWORD("Ncode", nRow);
#ifdef _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED
        pInfo->valuetype = GetDataBYTE("ValueType", nRow);
        if (pInfo->valuetype > eOPTION_VALUE_TYPE_SECOND)
        {
            assert(!"OptionList.txt 에 ValueType 값이 범위를 초과하였습니다. 확인해 주시기 바랍니다.");
        }
#else
    #ifdef _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED
        pInfo->m_byRatioValue = GetDataBYTE("RatioValue", nRow);
    #endif //_NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED
#endif //_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED

#ifdef _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO
        pInfo->valuetype2 = GetDataBYTE("ValueType2", nRow);
        if (pInfo->valuetype2 > eOPTION_VALUE_TYPE_TWO_REVERSE_BENEFIT)
        {
            assert(!"OptionList.txt 에 ValueType2 값이 범위를 초과하였습니다. 확인해 주시기 바랍니다.");
        }
#endif //_SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO

		assert(NULL == m_pDataTable->GetData(pInfo->mdwIndex));
		m_pDataTable->Add(pInfo, pInfo->mdwIndex);

        if (pInfo->mdwIndex > _countof(g_ITEM_OPTION))
        {
            assert(!"옵션 인덱스값의 범위가 초과 되었습니다.");
            continue;
        }
        const eATTR_TYPE attr_type = g_ITEM_OPTION[pInfo->mdwIndex];
        option_list_map_[attr_type] = pInfo;
	}

	return TRUE;
}



#pragma warning ( pop )