#include "StdAfx.h"
#include ".\rankoptionparser.h"

RankOptionParser::RankOptionParser(void)
{
}

RankOptionParser::~RankOptionParser(void)
{
	Unload();
}

VOID RankOptionParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID RankOptionParser::Reload()
{
//	Load( m_tszFileName, TRUE );
}

VOID RankOptionParser::Unload()
{
	RANKOPT_HASH::iterator it, end( m_ItemRankOptionHash.end() );
	for( it = m_ItemRankOptionHash.begin() ; it != end ; ++it )
	{
		sRANK_OPTION * pOptionArray = (it->second);
		SAFE_DELETE_ARRAY( pOptionArray );
	}
	m_ItemRankOptionHash.clear();

	UnloadSelector();

}

VOID RankOptionParser::UnloadSelector()
{
	RANKOPT_SEL_HASH::iterator it2, end2( m_RankOptionSelectorHash.end() );
	for( it2 = m_RankOptionSelectorHash.begin() ; it2 != end2 ; ++it2 )
	{
		sPTION_SELECTOR_FOR_RANK * pOptionArray = (it2->second);
		SAFE_DELETE_ARRAY( pOptionArray );
	}
	m_RankOptionSelectorHash.clear();
}


//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL RankOptionParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch( ScriptCode )
	{
	case SCRIPT_RANKOPTIONINFO:
		return _Load( bReload );
	}

	return FALSE;
}

BOOL RankOptionParser::_Load(BOOL bReload )
{
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		sRANK_OPTION* pRankOptionArray = 0;
		WORD wItemType		= GetDataWORD( "kind", nRow);
		int iOptionIndex	= GetDataInt( "code", nRow);

		FASSERT( iOptionIndex < MAX_ITEM_OPTION_KIND_NUM );

		RANKOPT_HASH::iterator itr = m_ItemRankOptionHash.find( wItemType);
		if ( itr == m_ItemRankOptionHash.end() )
		{
			// 없으면!
			pRankOptionArray = new sRANK_OPTION[MAX_ITEM_OPTION_KIND_NUM];
			memset(pRankOptionArray, 0, sizeof(sRANK_OPTION)*MAX_ITEM_OPTION_KIND_NUM );
			m_ItemRankOptionHash[wItemType] = pRankOptionArray;
		}
		else
		{
			pRankOptionArray = itr->second;
			if( FALSE == bReload )
			{
				//FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pRankOptionArray[iOptionIndex].m_iOptionIndex	= iOptionIndex;
		strncpy( pRankOptionArray[iOptionIndex].m_tszOptionName, GetDataString( "name", nRow), MAX_OPTIONNAME_LENGTH);
		pRankOptionArray[iOptionIndex].m_OptionNCode	= GetDataDWORD( "ncode", nRow);
		pRankOptionArray[iOptionIndex].m_NumericType	= GetDataDWORD( "type", nRow);

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( int i = RANK_D ; i < RANK_MAX ; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "rank_%d\0", i );
			pRankOptionArray[iOptionIndex].m_Value[i] = GetDataDWORD( szTemp, nRow);
		}
	
		CheckValidity( &pRankOptionArray[iOptionIndex], wItemType );
	}

	CalculateOptionSelector();
	return TRUE;
}

//#pragma warning ( pop )

VOID RankOptionParser::CheckValidity( sRANK_OPTION* pInfo, WORD wItemType )
{
	for( int Rank = RANK_D ; Rank < RANK_MAX ; ++Rank )
	{
		if( !CheckOptionValidity( pInfo->m_iOptionIndex, (BYTE)pInfo->m_NumericType, pInfo->m_Value[Rank] ) )
		{
            SUNLOG(eCRITICAL_LOG,
                   "|[RankOption]|wItemType=%d|"
                   "|OptionIndex=%d|OptionType=%d|OptionValue=%d|",
                   wItemType, pInfo->m_iOptionIndex, pInfo->m_NumericType, pInfo->m_Value[Rank]);
		}
	}
}

VOID RankOptionParser::CalculateOptionSelector()
{
	UnloadSelector();

	RANKOPT_HASH::iterator it, end( m_ItemRankOptionHash.end() );
	for( it = m_ItemRankOptionHash.begin() ; it != end ; ++it )
	{
		sRANK_OPTION * pOptionArray = (it->second);


		for( int OptionIndex = 0 ; OptionIndex < MAX_ITEM_OPTION_KIND_NUM ; ++OptionIndex )
		{
			for( int rank = 0 ; rank < RANK_MAX ; ++rank )
			{
				if( 0 != pOptionArray[OptionIndex].m_Value[rank] )
				{
					sPTION_SELECTOR_FOR_RANK * pRankOptionSelArray = NULL;
					RANKOPT_SEL_HASH::iterator its = m_RankOptionSelectorHash.find(it->first);
					if ( its == m_RankOptionSelectorHash.end() )
					{
						pRankOptionSelArray = new sPTION_SELECTOR_FOR_RANK[RANK_MAX];
						memset(pRankOptionSelArray, 0, sizeof(sPTION_SELECTOR_FOR_RANK)*RANK_MAX );
						m_RankOptionSelectorHash[it->first] = pRankOptionSelArray;
					}
					else
					{
						pRankOptionSelArray = its->second;
					}

					pRankOptionSelArray[rank].m_OptionIndex[pRankOptionSelArray[rank].m_OptionCount] = OptionIndex;
					++pRankOptionSelArray[rank].m_OptionCount;
				}
			}
		}
	}
}

