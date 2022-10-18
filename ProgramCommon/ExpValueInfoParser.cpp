
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
		if ( next_exp_info == NULL ) // �ְ� ����
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
			// ������!
			pInfo = new BASE_ACCUM_EXP_INFO;
			m_pAccumExpInfoHashTable->Add( pInfo, LV );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, LV = %d",  m_pszFileName, LV );
				FASSERT( !"�����Ϳ� ������ �ֽ��ϴ�." );
			}
		}

		pInfo->m_Level											= LV;
		pInfo->m_AccumExp	= GetDataNumber( "dExpValue", nRow );

		// ���� ����ġ ��ũ��Ʈ�� ������ ���� ��� ���� �ɰ��� �����̹Ƿ� üũ��!!
		if( LV != wCheckLevel)
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) ������ ���������� ����!", pInfo->m_Level );
			FASSERT(!"������ ���������� ����");
			return FALSE;
		}
		++wCheckLevel;

		if( GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st() < LV )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) �ִ� �������� �ʰ��� ��!", pInfo->m_Level );
			FASSERT(!"[ExpValueInfoParser] �ִ� �������� �ʰ��� ��!!");
			return FALSE;
		}

		if( 0 > pInfo->m_AccumExp )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) ��������ġ���� ���̳ʽ� ��!", pInfo->m_Level );
			FASSERT(!"��������ġ���� ���̳ʽ� ��!!");
			return FALSE;
		}

		if( CheckAccumExp > pInfo->m_AccumExp )
		{
			SUNLOG( eCRITICAL_LOG, "[ExpValueInfoParser] Level(%u) ���������� ���� ��������ġ���� �۾�����!", pInfo->m_Level );
			FASSERT(!"���������� ���� ��������ġ���� �۾�����!!");
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


