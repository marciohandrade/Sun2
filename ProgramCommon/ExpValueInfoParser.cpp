
#include "stdafx.h"
#include "ExpValueInfoParser.h"
#include <SolarFileReader.h>
#include <GameOptionParser.h>


ExpValueInfoParser::ExpValueInfoParser() : m_pAccumExpInfoHashTable ( NULL )
{
}

ExpValueInfoParser::~ExpValueInfoParser()
{
	FASSERT( m_pAccumExpInfoHashTable == NULL );
}

VOID ExpValueInfoParser::Release()
{
	Unload();
	SAFE_DELETE(m_pAccumExpInfoHashTable);
}

VOID ExpValueInfoParser::Init(DWORD dwPoolSize)
{
	FASSERT( m_pAccumExpInfoHashTable == NULL );
	m_pAccumExpInfoHashTable = new util::SolarHashTable<BASE_ACCUM_EXP_INFO*>;
	m_pAccumExpInfoHashTable->Initialize( dwPoolSize );
}

VOID ExpValueInfoParser::Init(DWORD dwPoolSize,char * pszPackFileName)
{
	FASSERT( m_pAccumExpInfoHashTable == NULL );
	m_pAccumExpInfoHashTable = new util::SolarHashTable<BASE_ACCUM_EXP_INFO*>;
	m_pAccumExpInfoHashTable->Initialize( dwPoolSize );

	SetPackFileName( pszPackFileName );
}

VOID ExpValueInfoParser::Unload()
{
    if (m_pAccumExpInfoHashTable == NULL) {
        return;
    }
    m_pAccumExpInfoHashTable->SetFirst();
    while (BASE_ACCUM_EXP_INFO* exp_info = m_pAccumExpInfoHashTable->GetNext())
    {
        SAFE_DELETE(exp_info);
    }
    m_pAccumExpInfoHashTable->RemoveAll();
}

VOID ExpValueInfoParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
LEVELTYPE ExpValueInfoParser::CalculateAccumExpToLevel( const EXPTYPE& accum_exp )
{
	LEVELTYPE level = 1;
	if ( accum_exp == 0 )
	{
		return level;
	}

	for ( int i = 1; i <= GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st(); ++i )
	{
		BASE_ACCUM_EXP_INFO* exp_info = m_pAccumExpInfoHashTable->GetData( i );
		BASE_ACCUM_EXP_INFO* next_exp_info = m_pAccumExpInfoHashTable->GetData( i + 1 );
		if ( next_exp_info == NULL ) // 최고 레벨
		{
			level = GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st();
			return level;
		}

		if ( (accum_exp >= exp_info->m_AccumExp) && (accum_exp < next_exp_info->m_AccumExp) )
		{
			level = i;
			return level;
		}
		level = i;
	}

	return level;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL ExpValueInfoParser::_Load( BOOL bReload )
{
	WORD wCheckLevel = 1;
	EXPTYPE CheckAccumExp = 0;

	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
		LEVELTYPE LV = GetDataNumber( "bLv", nRow );

		BASE_ACCUM_EXP_INFO * pInfo = m_pAccumExpInfoHashTable->GetData( LV );

		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new BASE_ACCUM_EXP_INFO;
			m_pAccumExpInfoHashTable->Add( pInfo, LV );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, LV = %d",  m_pszFileName, LV );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_Level											= LV;
		pInfo->m_AccumExp	= GetDataNumber( "dExpValue", nRow );

		// 누적 경험치 스크립트에 오류가 있을 경우 아주 심각한 문제이므로 체크함!!
		if( LV != wCheckLevel)
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) 레벨이 순차적이지 않음!", pInfo->m_Level );
			FASSERT(!"레벨이 순차적이지 않음");
			return FALSE;
		}
		++wCheckLevel;

		if( GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st() < LV )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) 최대 레벨값을 초과한 값!", pInfo->m_Level );
			FASSERT(!"[ExpValueInfoParser] 최대 레벨값을 초과한 값!!");
			return FALSE;
		}

		if( 0 > pInfo->m_AccumExp )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) 누적경험치값이 마이너스 값!", pInfo->m_Level );
			FASSERT(!"누적경험치값이 마이너스 값!!");
			return FALSE;
		}

		if( CheckAccumExp > pInfo->m_AccumExp )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) 레벨증가에 따른 누적경험치값이 작아졌음!", pInfo->m_Level );
			FASSERT(!"레벨증가에 따른 누적경험치값이 작아졌음!!");
			return FALSE;
		}
		CheckAccumExp = pInfo->m_AccumExp;
	}

	return TRUE;
}

BOOL ExpValueInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_EXPVALUEINFO:
		return _Load( bReload );
	}

	return FALSE;
}


#pragma warning ( pop )


