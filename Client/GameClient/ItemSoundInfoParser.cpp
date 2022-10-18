//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "ItemSoundInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
ItemSoundInfoParser::ItemSoundInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
ItemSoundInfoParser::~ItemSoundInfoParser()
{
   assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID ItemSoundInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID ItemSoundInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
   assert( m_pDataTable == NULL );
   m_pDataTable = new util::SolarHashTable<BASE_ItemSoundInfo*>;
   m_pDataTable->Initialize( dwPoolSize );
 SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID ItemSoundInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_ItemSoundInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID ItemSoundInfoParser::Unload()
{
	BASE_ItemSoundInfo * pInfo = NULL;
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

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_ItemSoundInfo* ItemSoundInfoParser::GetItemSoundInfo(DWORD dwKey)
{
	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL ItemSoundInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_ItemSoundInfo* pInfo = new BASE_ItemSoundInfo;
		
		//w_Code
		pInfo->m_w_Code  		=	GetDataWORD("w_Code", nRow);
		
		//s_desc
		strncpy(pInfo->m_sz_desc, GetDataString("s_desc", nRow), 256);
				
		//d_PickDefault
		pInfo->m_dw_PickDefault	=	GetDataDWORD("d_PickDefault", nRow);
		
		//d_PutDefault	
		pInfo->m_dw_PutDefault	=	GetDataDWORD("d_PutDefault", nRow);
		
		//d_PutWear	
		pInfo->m_dw_PutWear		=	GetDataDWORD("d_PutWear", nRow);
		
		//d_RemoveWear	
		pInfo->m_dw_RemoveWear	=	GetDataDWORD("d_RemoveWear", nRow);
		
		//d_LinkQslot	
		pInfo->m_dw_LinkQslot	=	GetDataDWORD("d_LinkQslot", nRow);
		
		//d_UnlinkQslot	
		pInfo->m_dw_UnlinkQslot	=	GetDataDWORD("d_UnlinkQslot", nRow);
		
		//d_Dump	
		pInfo->m_dw_Dump		=	GetDataDWORD("d_Dump", nRow);
				
		assert(NULL == m_pDataTable->GetData(pInfo->m_w_Code));
		m_pDataTable->Add(pInfo, pInfo->m_w_Code);

	}
	
	return TRUE;
}



#pragma warning ( pop )
