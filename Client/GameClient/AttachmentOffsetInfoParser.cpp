//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "AttachmentOffsetInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
AttachmentOffsetInfoParser::AttachmentOffsetInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
AttachmentOffsetInfoParser::~AttachmentOffsetInfoParser()
{
    ASSERT( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID AttachmentOffsetInfoParser::Release()
{
    Unload();
    delete m_pDataTable;
    m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID AttachmentOffsetInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
    ASSERT( m_pDataTable == NULL );
    m_pDataTable = new util::SolarHashTable<BASE_AttachmentOffsetInfo*>;
    m_pDataTable->Initialize( dwPoolSize );
    SetPackFileName( pszPackFileName );
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID AttachmentOffsetInfoParser::Init(DWORD dwPoolSize)
{
	ASSERT( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_AttachmentOffsetInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID AttachmentOffsetInfoParser::Unload()
{
    BASE_AttachmentOffsetInfo * pInfo = NULL;
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
BASE_AttachmentOffsetInfo* AttachmentOffsetInfoParser::GetAttachmentOffsetInfo(DWORD dwKey)
{

    return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL AttachmentOffsetInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch (ScriptCode)
	{
	case SCRIPT_ATTACHMENT_OFFSET_INFO:
		return _Load(bReload);
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL AttachmentOffsetInfoParser::_Load(BOOL bReload)
{
	int nRowSize = GetRowSize();
	DWORD dwOffset = 0;

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_AttachmentOffsetInfo * pInfo = new BASE_AttachmentOffsetInfo;
        ZeroMemory(pInfo, sizeof(BASE_AttachmentOffsetInfo));
		pInfo->m_dwItemCode = GetDataDWORD("ItemCode", nRow);
		
        pInfo->pos_x_ = GetDataDWORD("PosX", nRow) * 0.01f;
        pInfo->pos_y_ = GetDataDWORD("PosY", nRow) * 0.01f;
        pInfo->pos_z_ = GetDataDWORD("PosZ", nRow) * 0.01f;

        pInfo->rot_x_ = GetDataFloat("RotX", nRow);
        pInfo->rot_y_ = GetDataFloat("RotY", nRow);
        pInfo->rot_z_ = GetDataFloat("RotZ", nRow);

		ASSERT(NULL == m_pDataTable->GetData(pInfo->m_dwItemCode));
		m_pDataTable->Add(pInfo, pInfo->m_dwItemCode);
	}

	return TRUE;
}


#pragma warning ( pop )
