#include "StdAfx.h"

#ifdef _NA_008012_20150130_RANKING_SYSTEM

#include "PointRankingParser.h"

#pragma warning ( push )
#pragma warning ( disable : 4244)

PointRankingParser::PointRankingParser()
{
}

PointRankingParser::~PointRankingParser()
{
}

BOOL	PointRankingParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_POINT_RANKING:
		return _Load( bReload );
	}

	return FALSE;
}


BOOL PointRankingParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		WORD wIndex = GetDataNumber( "Index", nRow );
		st_PointRankingTable* pTable = NULL;
		if( m_PointTableHash.find(wIndex) == m_PointTableHash.end() )
		{
			pTable = new st_PointRankingTable;
			m_PointTableHash[wIndex] = pTable;
		}

		else
		{
			pTable = m_PointTableHash[wIndex];
		}		

		pTable->wIndex = wIndex;
		pTable->dwNomal_Per	 = GetDataNumber( "Normal_Per", nRow );
		pTable->dwNomal_Min	 = GetDataNumber( "Normal_Min", nRow );
		pTable->dwNomal_Max	 = GetDataNumber( "Normal_Max", nRow );

		pTable->dwLeader_Per = GetDataNumber( "Leader_Per", nRow );
		pTable->dwLeader_Min = GetDataNumber( "Leader_Min", nRow );
		pTable->dwLeader_Max = GetDataNumber( "Leader_Max", nRow );
		
		pTable->dwCaptain_Per = GetDataNumber( "Captain_Per", nRow );
		pTable->dwCaptain_Min = GetDataNumber( "Captain_Min", nRow );
		pTable->dwCaptain_Max = GetDataNumber( "Captain_Max", nRow );

		pTable->dwVeteran_Per = GetDataNumber( "Veteran_Per", nRow );
		pTable->dwVeteran_Min = GetDataNumber( "Veteran_Min", nRow );
		pTable->dwVeteran_Max = GetDataNumber( "Veteran_Max", nRow );

		pTable->dwBoss_Per	  = GetDataNumber( "Boss_Per", nRow );
		pTable->dwBoss_Min	  = GetDataNumber( "Boss_Min", nRow );
		pTable->dwBoss_Max	  = GetDataNumber( "Boss_Max", nRow );

		pTable->lvApplyMinLV  = GetDataNumber( "Amin_Lv", nRow );
		pTable->lvApplyMaxLV  = GetDataNumber( "Amax_Lv", nRow );
	}

	return TRUE;
}


VOID PointRankingParser::Release()
{	
	STLX_HASH_MAP<WORD, st_PointRankingTable*>::iterator it = m_PointTableHash.begin();	

	while(it != m_PointTableHash.end())
	{		
		SAFE_DELETE( it->second );
		it++;
	}
	m_PointTableHash.clear();
}

#pragma warning ( pop )

st_PointRankingTable* PointRankingParser::GetPointTable( WORD Index )
{
	STLX_HASH_MAP<WORD, st_PointRankingTable*>::iterator it =  m_PointTableHash.find( Index );
	
	if( it == m_PointTableHash.end() )
	{
		SUNLOG(eCRITICAL_LOG, "[PointRankingScript]Not Found Point Table[%u]", Index );
		ASSERT(FALSE);
		return NULL;
	}

	return m_PointTableHash[Index];
}
#endif // _NA_008012_20150130_RANKING_SYSTEM