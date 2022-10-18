#include "StdAfx.h"
#include ".\extrastoneoptioninfoparser.h"

ExtraStoneOptionInfoParser::ExtraStoneOptionInfoParser(void):
m_pExtraStoneOptionInfoHash(NULL)
{
	m_pszFileName[0] = '\0';
}

ExtraStoneOptionInfoParser::~ExtraStoneOptionInfoParser(void)
{
	FASSERT( m_pExtraStoneOptionInfoHash == NULL );
	Release();
}

VOID ExtraStoneOptionInfoParser::Init( DWORD dwOptionInfoSize )
{
	FASSERT( m_pExtraStoneOptionInfoHash == NULL );
	m_pExtraStoneOptionInfoHash = new _EXTRASTONE_OPT_INFO_HASH;
	m_pExtraStoneOptionInfoHash->Initialize( dwOptionInfoSize );
}

VOID ExtraStoneOptionInfoParser::Init( DWORD dwOptionInfoSize, char * pszPackFileName )
{
	Init( dwOptionInfoSize );
	SetPackFileName( pszPackFileName );
}

VOID ExtraStoneOptionInfoParser::Release()
{
	Unload();

	SAFE_DELETE( m_pExtraStoneOptionInfoHash );
}


#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL ExtraStoneOptionInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
		SLOTCODE Index			= GetDataNumber( "bIndex", nRow );
		WORD wItemType			= (CODETYPE)GetDataNumber( "bEquipType", nRow );
		LEVELTYPE ItemLV		= (CODETYPE)GetDataNumber( "bItemLV", nRow );

		sEXTRASTONE_OPT_INFO * pInfo = const_cast<sEXTRASTONE_OPT_INFO *>(GetExtraStoneOptionInfo( wItemType ));
		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new sEXTRASTONE_OPT_INFO;
			pInfo->m_dwIndex = Index;
			pInfo->m_wItemType = wItemType;
			pInfo->m_ItemLV = ItemLV;
			m_pExtraStoneOptionInfoHash->Add( pInfo, pInfo->m_wItemType );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, ItemType = %d",  m_pszFileName, wItemType );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}
	
		pInfo->m_dwIndex		= Index;
		pInfo->m_wItemType		= wItemType;
		pInfo->m_ItemLV			= ItemLV;
		pInfo->m_wOptionIndex	= GetDataNumber( "bOptionCode", nRow );
		pInfo->m_dwOptionNCode	= GetDataNumber( "dOptionNCode", nRow );
		pInfo->m_byValueType	= GetDataNumber( "bValueType", nRow );
		pInfo->m_dwValue[0]		= GetDataNumber( "bValue0", nRow ); 
		pInfo->m_dwValue[1]		= GetDataNumber( "bValue1", nRow ); 
		pInfo->m_dwValue[2]		= GetDataNumber( "bValue2", nRow ); 
		pInfo->m_dwValue[3]		= GetDataNumber( "bValue3", nRow ); 
	}

	return TRUE;
}

BOOL ExtraStoneOptionInfoParser::_LoadExtra( BOOL bReload )
{
    __UNUSED(bReload);
	return TRUE;
}


BOOL ExtraStoneOptionInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_EXTRASTONEOPTIONINFO :
		return _Load( bReload );
	}

	return FALSE;
}

#pragma warning ( pop )

VOID ExtraStoneOptionInfoParser::Reload()
{
//	Load( this->m_pszFileName, TRUE );
}


VOID ExtraStoneOptionInfoParser::Unload()
{
    // 조합정보 내림
    if (m_pExtraStoneOptionInfoHash)
    {
        m_pExtraStoneOptionInfoHash->SetFirst();
        while (sEXTRASTONE_OPT_INFO* extra_option = m_pExtraStoneOptionInfoHash->GetNext())
        {
            SAFE_DELETE(extra_option);
        }
        m_pExtraStoneOptionInfoHash->RemoveAll();
    }
}




//BOOL		ExtraStoneOptionInfoParser::LoadEx( eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bReload )
//{
//	FASSERT( m_pExtraStoneOptionInfoHash != NULL );
//	return IParser::LoadEx( ScriptCode, pszFileName, bReload );
//}



