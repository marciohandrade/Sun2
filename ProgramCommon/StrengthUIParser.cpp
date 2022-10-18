#include "StdAfx.h"
#include ".\StrengthUIParser.h"


#include <iostream>


#ifdef __NA_001062_20080623_STRENGTH_PENALTY



StrengthUIParser::StrengthUIParser(void)
{
	m_StrengthUIHash.Initialize( 100 );

}

StrengthUIParser::~StrengthUIParser(void)
{
	Unload();
}

VOID StrengthUIParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID StrengthUIParser::Reload()
{
	//	Load( m_tszFileName, TRUE );
}

VOID StrengthUIParser::Unload()
{
	STRENGTH_UI_HASH_ITR it = m_StrengthUIHash.begin() ;
	for( ; it != m_StrengthUIHash.end() ; ++it )	
	{
		delete (*it);
	}
	m_StrengthUIHash.RemoveAll();
}

// 자료구조에 새로운 값 설정
sSTRENGTH_UI_RECODE* StrengthUIParser::_NewStrengthUI( INT Idx )
{
	sSTRENGTH_UI_RECODE* pNewElm = new sSTRENGTH_UI_RECODE;

	AddStrengthUI( pNewElm, Idx );

	return pNewElm;
}


#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL StrengthUIParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	sSTRENGTH_UI_RECODE Recode;
	TCHAR szTemp[MAX_FIELD_NAME_SIZE] = {0,};

	//.//

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		memset( &Recode, 0, sizeof(sSTRENGTH_UI_RECODE) );
		Recode.m_Idx		 = GetDataNumber( "Index", nRow );
		Recode.m_YCoordinate = GetDataNumber( "Y_Coordinate", nRow );

		for( INT i = 0; i < MAX_STRENGTH_UI_XCOORD; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "X_Coordinate%d\0", i+1 );
			Recode.m_XCoordinate[i] = GetDataNumber(szTemp, nRow);
		}

		//.//

		Validating(Recode);

		//.//

		sSTRENGTH_UI_RECODE* pRecode = FindStrengthUI( Recode.m_Idx );

		if( !pRecode ) 
		{
			pRecode = _NewStrengthUI( Recode.m_Idx );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "StrengthUI 인덱스(%ld)가 중복됩니다", Recode.m_Idx );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}
		memcpy( pRecode, &Recode, sizeof(sSTRENGTH_UI_RECODE) );
	}

	return TRUE;
}

BOOL StrengthUIParser::Validating(sSTRENGTH_UI_RECODE& Recode)
{
	if( Recode.m_Idx == 0 )  
	{
		// 잘못된 인덱스 값.
		ASSERT(FALSE);
	}

	//스크립트 값 체크.

	return TRUE;
}

#pragma warning ( pop )

BOOL	StrengthUIParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_STRENGTH_UI:
		return _Load( bReload );
		break;
	}

	return FALSE;
}

#endif //__NA_001062_20080623_STRENGTH_PENALTY
