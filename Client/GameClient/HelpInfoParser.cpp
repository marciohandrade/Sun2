#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/03/13
//------------------------------------------------------------------------------

#include "HelpInfoParser.h"
#include <SolarFileReader.h>

//----------------------------------------------------------------------------                   	
/**                    
*/
HelpInfoParser::HelpInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
HelpInfoParser::~HelpInfoParser()
{
   assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HelpInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HelpInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HelpInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );

	m_HelpIndex.clear();
	m_vecHelpIndex.clear();

}


//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HelpInfoParser::Init(DWORD dwPoolSize )
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HelpInfo*>;
	m_pDataTable->Initialize( dwPoolSize );

	m_HelpIndex.clear();
	m_vecHelpIndex.clear();

}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HelpInfoParser::Unload()
{
	BASE_HelpInfo * pInfo = NULL;
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

	m_HelpIndex.clear();
	m_vecHelpIndex.clear();

}

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_HelpInfo* HelpInfoParser::GetHelpInfo(DWORD dwHelpIndex)
{
	return m_pDataTable->GetData( dwHelpIndex );
}


#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL HelpInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch (ScriptCode)
	{
	case SCRIPT_HELP_INFO:
		return _Load(bReload);
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL HelpInfoParser::_Load(BOOL bReload)
{
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		// 스크립트에서 읽어온 정보
		BASE_HelpInfo* pInfo = new BASE_HelpInfo;
		pInfo->m_dw_HelpIndex = GetDataDWORD("HelpIndex", nRow);
		pInfo->m_bHelpMethod = GetDataBYTE("HelpMethod", nRow);
		pInfo->m_dw_CharStatus = GetDataDWORD("CharStatus", nRow);
		pInfo->m_dw_Level = GetDataDWORD("Level", nRow);
		pInfo->m_dw_Repeat = GetDataDWORD("Repeat", nRow);
		pInfo->m_bWindow_States = GetDataBYTE("WindowStates", nRow);
		pInfo->m_dw_Param = GetDataDWORD("Param", nRow);
		pInfo->m_dw_TitleStringCode = GetDataDWORD("TitleStringCode", nRow);

		pInfo->m_dw_DescStringCode[eHELP_DESC1] = GetDataDWORD("Desc1", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC2] = GetDataDWORD("Desc2", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC3] = GetDataDWORD("Desc3", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC4] = GetDataDWORD("Desc4", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC5] = GetDataDWORD("Desc5", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC6] = GetDataDWORD("Desc6", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC7] = GetDataDWORD("Desc7", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC8] = GetDataDWORD("Desc8", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC9] = GetDataDWORD("Desc9", nRow);
		pInfo->m_dw_DescStringCode[eHELP_DESC10] = GetDataDWORD("Desc10", nRow);

		pInfo->m_dw_ImageCode = GetDataDWORD("ImageCode", nRow);
		pInfo->m_bSoundControl = GetDataBYTE("SoundControl", nRow);
		pInfo->m_dwSoundValue = GetDataDWORD("SoundValue", nRow);

		assert(NULL == m_pDataTable->GetData(pInfo->m_dw_HelpIndex));
		m_pDataTable->Add(pInfo, pInfo->m_dw_HelpIndex);

		// 캐릭 상태를 기준으로 따로 저장
		BASE_HelpIndex helpIndex;
		helpIndex.m_dw_CharStatus = pInfo->m_dw_CharStatus;
		helpIndex.m_dw_HelpIndex = pInfo->m_dw_HelpIndex;
		helpIndex.m_dw_Level = pInfo->m_dw_Level;
		helpIndex.m_dw_Repeat = pInfo->m_dw_Repeat;
		helpIndex.m_bWindow_States = pInfo->m_bWindow_States;
		helpIndex.m_dw_Param = pInfo->m_dw_Param;

		helpIndex.m_bHelpMethod = pInfo->m_bHelpMethod;
		helpIndex.m_bSoundControl = pInfo->m_bSoundControl;
		helpIndex.m_dwSoundValue = pInfo->m_dwSoundValue;

		m_vecHelpIndex.push_back(pInfo->m_dw_HelpIndex);		
		m_HelpIndex.insert(HELPINDEX_PAIR(helpIndex.m_dw_CharStatus, helpIndex));

	}

	return TRUE;
}

#pragma warning ( pop )

