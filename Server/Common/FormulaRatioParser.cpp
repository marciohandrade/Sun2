#include "stdafx.h"
#include "Struct.h"
#include "FormulaRatioParser.h"
#include <SolarFileReader.h>


FormulaRatioParser::FormulaRatioParser() : m_pRatioHashTable ( NULL )
{
}

FormulaRatioParser::~FormulaRatioParser()
{
	ASSERT( m_pRatioHashTable == NULL );
}

VOID FormulaRatioParser::Release()
{
	Unload();
	SAFE_DELETE(m_pRatioHashTable);
}

VOID FormulaRatioParser::Init( DWORD dwPoolSize )
{
	ASSERT( m_pRatioHashTable == NULL );
	m_pRatioHashTable = new util::SolarHashTable<BASE_FORMULA_RATIOINFO*>;
	m_pRatioHashTable->Initialize( dwPoolSize );
}

VOID FormulaRatioParser::Unload()
{
	BASE_FORMULA_RATIOINFO *pInfo = NULL ;
	if( m_pRatioHashTable == NULL )
	{
		return;
	}
	m_pRatioHashTable->SetFirst();
	while( pInfo = m_pRatioHashTable->GetNext() )
	{
		SAFE_DELETE( pInfo );
	}
	m_pRatioHashTable->RemoveAll();
}

VOID FormulaRatioParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

BASE_FORMULA_RATIOINFO*	FormulaRatioParser::GetFormulaRatio( WORD wCode, LEVELTYPE Level )
{
	DWORD dwKey = MAKE_FOMULAKEY( wCode, Level );

	return m_pRatioHashTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL FormulaRatioParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		WORD wCode = GetDataNumber( "NCode", nRow );
		LEVELTYPE Level = 0;
		Level = GetDataNumber( "level", nRow );
		DWORD dwKey = MAKE_FOMULAKEY( wCode, Level );

		BASE_FORMULA_RATIOINFO * pInfo = m_pRatioHashTable->GetData( dwKey );

		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new BASE_FORMULA_RATIOINFO;
			m_pRatioHashTable->Add( pInfo, dwKey );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, FormulaRatioParser, Code = %d, Level = %d", wCode, Level );
				ASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_wNCode	= wCode;
		pInfo->m_Level = Level;
		pInfo->m_fRatio = GetDataNumber( "NMul", nRow );
	}

	return TRUE;

}

#pragma warning ( pop )



//BOOL	FormulaRatioParser::LoadEx( eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bReload )
//{
//	ASSERT( m_pRatioHashTable != NULL );
//	return IParser::LoadEx( ScriptCode, pszFileName, bReload );
//}


BOOL	FormulaRatioParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_FORMULARATIO:
		return _Load( bReload );
	};

	return FALSE; 	
}



