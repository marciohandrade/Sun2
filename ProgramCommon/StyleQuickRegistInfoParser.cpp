//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include ".\StyleQuickRegistInfoParser.h"
#include <iostream>


//----------------------------------------------------------------------------                   	
/**                    
*/
StyleQuickRegistInfoParser::StyleQuickRegistInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
StyleQuickRegistInfoParser::~StyleQuickRegistInfoParser()
{
   FASSERT( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID StyleQuickRegistInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID StyleQuickRegistInfoParser::Init( DWORD dwPoolSize)
{
   FASSERT( m_pDataTable == NULL );
   m_pDataTable = new util::SolarHashTable<BASE_StyleQuickRegistInfo*>;
   m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------
/**
*/
VOID           
StyleQuickRegistInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
void StyleQuickRegistInfoParser::Unload()
{
    if (m_pDataTable == NULL) {
        return;
    }
    m_pDataTable->SetFirst();
    while (BASE_StyleQuickRegistInfo* info = m_pDataTable->GetNext()) {
        delete info;
    }
    m_pDataTable->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID
StyleQuickRegistInfoParser::Reload()
{
//	Load(m_pszFileName, TRUE);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_StyleQuickRegistInfo * 
StyleQuickRegistInfoParser::GetStyleQuickRegistInfo(int iClassType, int iWeaponKind)
{
	DWORD dwKey = MakeKey(iClassType, iWeaponKind);

	return m_pDataTable->GetData( dwKey );
}

//----------------------------------------------------------------------------
/**
*/
DWORD
StyleQuickRegistInfoParser::MakeKey(int iClassType, int iWeaponKind)
{
	return ( DWORD(WORD(iClassType) << 16) | DWORD( WORD(iWeaponKind) ) );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/

BOOL StyleQuickRegistInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	TCHAR szTemp[MAX_FIELD_NAME_SIZE] = {0,};

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		BYTE byType	  = GetDataNumber("Type", nRow);
		BYTE byClass  = GetDataNumber("Class", nRow);
		BYTE byWeapon = GetDataNumber("Weapon", nRow);
		DWORD dwKey	  = MakeKey(byClass, byWeapon);
		
		BASE_StyleQuickRegistInfo* pInfo = m_pDataTable->GetData( dwKey );

		if( !pInfo )
		{
			pInfo = new BASE_StyleQuickRegistInfo;
			ZeroMemory(pInfo, sizeof(BASE_StyleQuickRegistInfo));

			pInfo->m_byType   = byType;
			pInfo->m_byClass  = byClass;
			pInfo->m_byWeapon = byWeapon;
			m_pDataTable->Add( pInfo, dwKey );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, StyleQuickRegistInfoParser, Class = %d, Weapon =%d",byClass, byWeapon );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		for( int i = 0; i < MAX_SKILL_REGIST_COUNT; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "Style%d\0", i+1 );
			pInfo->m_wStyleClassCode[i]	= GetDataNumber(szTemp, nRow);
		}

		FASSERT( NULL == m_pDataTable->GetData( pInfo->m_byClass ) );
	}

	return TRUE;
}

#pragma warning ( pop )

BOOL	StyleQuickRegistInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_STYLEQUICKREGISTINFO:
		return _Load( bReload );
	}

	return FALSE; 
}

