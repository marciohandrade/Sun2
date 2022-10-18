#include "StdAfx.h"
#include ".\LimitedOptionParser.h"
#include <iostream>


LimitedOptionParser::LimitedOptionParser(void)
{
	
}

LimitedOptionParser::~LimitedOptionParser(void)
{
	Unload();
}

VOID LimitedOptionParser::Release()
{
	Unload();
}

VOID LimitedOptionParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID LimitedOptionParser::Unload()
{
	{	
		LIMITED_ITEM_OPT_HASH::iterator it, end( m_LimitedOptionListHash.end() );
		for( it = m_LimitedOptionListHash.begin() ; it != end ; ++it )
		{
			sLIMITED_ITEM_OPTION * pOptionArray = (it->second);
			SAFE_DELETE( pOptionArray );
		}
		m_LimitedOptionListHash.clear();
	}

	{
		LIMITEDOPT_HASH::iterator it, end( m_LimitedOptionKindHash.end() );
		for( it = m_LimitedOptionKindHash.begin() ; it != end ; ++it )
		{
			sLIMITED_OPTION * pOptionArray = (it->second);
			SAFE_DELETE( pOptionArray );
		}
		m_LimitedOptionKindHash.clear();
	}
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)


BOOL LimitedOptionParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{		
		WORD		wIndex = GetDataWORD( "wOptionIndex", nRow );
		sLIMITED_ITEM_OPTION * pInfo = NULL;

		LIMITED_ITEM_OPT_HASH::iterator itr = m_LimitedOptionListHash.find( wIndex );
		if ( itr == m_LimitedOptionListHash.end() )
		{
			// 없으면!
			pInfo = new sLIMITED_ITEM_OPTION;
			m_LimitedOptionListHash[wIndex]  = pInfo;
		}
		else
		{
			pInfo = itr->second;
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", wIndex );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_LimitedOptionIndex = wIndex;
		strncpy( pInfo->m_pszName, GetDataString( "szName", nRow ), MAX_LIMITED_OPTIONNAME_LENGTH );
		pInfo->m_ItemType			= GetDataBYTE( "byItemType", nRow );
		pInfo->m_OptionKind			= GetDataWORD( "wOptionKind", nRow );
		pInfo->m_NumericType		= (eVALUE_TYPE)GetDataInt( "iOptionType", nRow );

		pInfo->m_MinValue			= GetDataWORD( "wMinValue", nRow );
		pInfo->m_MaxValue			= GetDataWORD( "wMaxValue", nRow );

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];

		for( BYTE i = 0 ; i < 5 ; ++i)
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "iProbability%d\0", i+1 );
			pInfo->m_Probability[i]	= GetDataInt( szTemp, nRow );
		}
		pInfo->m_ProbabilitySum		= GetDataInt( "iProbabilitySum", nRow );		

		if( ValidCheck( pInfo ) )
		{
			ASSERT(FALSE);
		}

		///////////////////////////////////

		sLIMITED_OPTION * pOptionInfo = NULL;					
		LIMITEDOPT_HASH::iterator itr2 = m_LimitedOptionKindHash.find( pInfo->m_OptionKind );
		if ( itr2 == m_LimitedOptionKindHash.end() )
		{
			// 없으면!
			pOptionInfo = new sLIMITED_OPTION;
			m_LimitedOptionKindHash[pInfo->m_OptionKind] = pOptionInfo;
		}
		else
		{
			pOptionInfo = itr2->second;
		}

		pOptionInfo->m_AttrIndex	= pInfo->m_OptionKind;
		strncpy( pOptionInfo->m_pszAttrName, pInfo->m_pszName, MAX_LIMITED_OPTIONNAME_LENGTH );
		pOptionInfo->m_NumericType	= pInfo->m_NumericType;
		pOptionInfo->m_iValue[LIMITED_1] = 0;
		pOptionInfo->m_iValue[LIMITED_2] = 0;
		pOptionInfo->m_iValue[LIMITED_3] = 0;
	}

	return TRUE;
}
//#pragma warning ( pop )

BOOL LimitedOptionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_GAMBLE_OPTION:
		return _Load( bReload );	
	}

	return FALSE;
}


sLIMITED_OPTION * LimitedOptionParser::GetLimitedOptionByKind( WORD OptionIndex )
{
	sLIMITED_OPTION * pOptionInfo = NULL;	
	LIMITEDOPT_HASH::iterator itr = m_LimitedOptionKindHash.find( OptionIndex );
	if ( itr != m_LimitedOptionKindHash.end() )
	{
		pOptionInfo = itr->second;
		if( OptionIndex < _MAX_LIMITED_ITEM_OPTION_NUM )
			return pOptionInfo;
	}

	return NULL;
}

WORD LimitedOptionParser::ValidCheck(sLIMITED_ITEM_OPTION* item_option)
{
    int sum_of_probabilities = 0;
    for (int* it = item_option->m_Probability;
         it != &item_option->m_Probability[_countof(item_option->m_Probability)];
         ++it)
    {
        sum_of_probabilities += *it;
    };
    // 확률 체크
    if (item_option->m_ProbabilitySum != sum_of_probabilities) {
        return 1;
    };
    // 비율타입 옵션 체크
    if (ns_formula::CheckOptionValidity(item_option->m_OptionKind,
            item_option->m_NumericType, item_option->m_MaxValue) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               "[LimitedOption]|Invalid Option|Index=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|",
               item_option->m_LimitedOptionIndex,
               item_option->m_OptionKind, item_option->m_NumericType, item_option->m_MaxValue);
    };
    return 0;
}