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

// �ڷᱸ���� ���ο� �� ����
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
				SUNLOG( eCRITICAL_LOG, "StrengthUI �ε���(%ld)�� �ߺ��˴ϴ�", Recode.m_Idx );
				FASSERT( !"�����Ϳ� ������ �ֽ��ϴ�." );
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
		// �߸��� �ε��� ��.
		ASSERT(FALSE);
	}

	//��ũ��Ʈ �� üũ.

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
