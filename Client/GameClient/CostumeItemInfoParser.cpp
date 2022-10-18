#include "SunClientPrecompiledHeader.h"
#include "CostumeItemInfoParser.h"

#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
cCostumeItemInfoParser::cCostumeItemInfoParser()
: m_pDataTable(NULL)
{

}

cCostumeItemInfoParser::~cCostumeItemInfoParser()
{

}

VOID cCostumeItemInfoParser::Init(DWORD dwPoolSize, char * pszPackFileName)
{
    Init( dwPoolSize );
    SetPackFileName( pszPackFileName );
}

VOID cCostumeItemInfoParser::Init(DWORD dwPoolSize)
{
    assert( m_pDataTable == NULL );
    m_pDataTable = new util::SolarHashTable<cCostumeItemInfo*>;
    m_pDataTable->Initialize( dwPoolSize );
}

VOID cCostumeItemInfoParser::Release()
{
    Unload();
    if (m_pDataTable)
        delete m_pDataTable;
    m_pDataTable = NULL;

}


VOID cCostumeItemInfoParser::Unload()
{
    cCostumeItemInfo * pInfo = NULL;
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


cCostumeItemInfo* cCostumeItemInfoParser::GetInfo( SLOTCODE ItemCode )
{
    return m_pDataTable->GetData( ItemCode );
}


BOOL cCostumeItemInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_COSTUMEITEM_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        int ItemCode = GetDataInt("ItemCode", row );
		const char* MaleIdleAnimation = GetDataString("MaleIdleAnimation", row );
		const char* FemaleIdleAnimation = GetDataString("FemaleIdleAnimation", row );

        cCostumeItemInfo* pInfo = GetInfo( ItemCode );
        if( pInfo == NULL )
        {
            pInfo = new cCostumeItemInfo();
			pInfo->SetIdleAnimation( StrToWzID(MaleIdleAnimation), StrToWzID(FemaleIdleAnimation) );
			pInfo->SetItemCode( ItemCode );
            m_pDataTable->Add( pInfo, ItemCode );
        }
    }

    return TRUE;
}

#endif //_NA_COSTUMEITEM_IDLE_ANIMATION