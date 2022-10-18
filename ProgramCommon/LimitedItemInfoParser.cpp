#include "StdAfx.h"
#include ".\LimitedItemInfoParser.h"
#include <iostream>


LimitedItemInfoParser::LimitedItemInfoParser(void)
{

}

LimitedItemInfoParser::~LimitedItemInfoParser(void)
{
	Unload();
}

VOID LimitedItemInfoParser::Release()
{
	Unload();
}


VOID LimitedItemInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );

	m_LimitedItemInfoIterator = m_HashLimitedItemInfo.begin();
}


VOID LimitedItemInfoParser::Unload()
{
	DWORD Index = 0;
	
	{
		LIMITED_ITEM_INFO_HASH::iterator it, end( m_HashLimitedItemInfo.end() );
		for( it = m_HashLimitedItemInfo.begin() ; it != end ; ++it )
		{
			sLIMITED_ITEM_INFO * pItemInfoArray = (it->second);
			
			Index = pItemInfoArray->m_dwIndex;
            
			SAFE_DELETE( m_HashDuraRatioRandomizer[Index] );
			SAFE_DELETE( m_HashLimitValueRatioRandomizer[Index] );
			SAFE_DELETE( m_HashDamageRatioRandomizer[Index] );

        #ifndef _NA_000251_20100727_SOCKET_SYSTEM
            SAFE_DELETE( m_HashSocketRatioRandomizer[Index] );
        #endif // _NA_000251_20100727_SOCKET_SYSTEM
            SAFE_DELETE( m_HashGambleRatioRandomizer[Index] );			
			
			SAFE_DELETE( pItemInfoArray );			
		}
		m_HashLimitedItemInfo.clear();		
	}
	m_HashDuraRatioRandomizer.clear();
	m_HashLimitValueRatioRandomizer.clear();
	m_HashDamageRatioRandomizer.clear();

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
    m_HashSocketRatioRandomizer.clear();
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    m_HashGambleRatioRandomizer.clear();
}


BOOL	LimitedItemInfoParser::_Load( BOOL bReload )
{
	const INT nRowSize = GetRowSize();
	for( INT nRow=0; nRow<nRowSize ; ++nRow )
	{
		const DWORD index				= GetDataDWORD( "Index", nRow );

		sLIMITED_ITEM_INFO* pInfo		= NULL;
		ReliableRatioRandomizerEX<MAX_DURA_RATIO_COUNT>*				pDuraRatioRandomizer		= NULL;
		ReliableRatioRandomizerEX<MAX_LIMIT_VALUE_RATIO_OPTION_COUNT>*	pLimitValueRatioRandomizer	= NULL;
		ReliableRatioRandomizerEX<MAX_DAMAGE_RATIO_COUNT>*				pDamageRatioRandomizer		= NULL;

    #ifndef _NA_000251_20100727_SOCKET_SYSTEM
        ReliableRatioRandomizer<MAX_LIMITED_SOCKET_COUNT>*				pSocketRatioRandomizer		= NULL;
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
        ReliableRatioRandomizer<MAX_LIMITED_GAMBLE_COUNT>*				pGambleRatioRandomizer		= NULL;		

		LIMITED_ITEM_INFO_HASH::iterator itr = m_HashLimitedItemInfo.find( index );
		if( itr == m_HashLimitedItemInfo.end() )
		{
			// 없으면!
			pInfo						= new sLIMITED_ITEM_INFO;

			pDuraRatioRandomizer		= new ReliableRatioRandomizerEX<MAX_DURA_RATIO_COUNT>;
			pLimitValueRatioRandomizer	= new ReliableRatioRandomizerEX<MAX_LIMIT_VALUE_RATIO_OPTION_COUNT>;
			pDamageRatioRandomizer		= new ReliableRatioRandomizerEX<MAX_DAMAGE_RATIO_COUNT>;

        #ifndef _NA_000251_20100727_SOCKET_SYSTEM
            pSocketRatioRandomizer		= new ReliableRatioRandomizer<MAX_LIMITED_SOCKET_COUNT>;
        #endif // _NA_000251_20100727_SOCKET_SYSTEM
            pGambleRatioRandomizer		= new ReliableRatioRandomizer<MAX_LIMITED_GAMBLE_COUNT>;			

			memset( pInfo, 0, sizeof(sLIMITED_ITEM_INFO) );
			m_HashLimitedItemInfo[index]				= pInfo;

			m_HashDuraRatioRandomizer[index]			= pDuraRatioRandomizer;
			m_HashLimitValueRatioRandomizer[index]		= pLimitValueRatioRandomizer;
			m_HashDamageRatioRandomizer[index]			= pDamageRatioRandomizer;

        #ifndef _NA_000251_20100727_SOCKET_SYSTEM
            m_HashSocketRatioRandomizer[index]			= pSocketRatioRandomizer;
        #endif // _NA_000251_20100727_SOCKET_SYSTEM
            m_HashGambleRatioRandomizer[index]			= pGambleRatioRandomizer;
		}
		else
		{
			pInfo = itr->second;

			pDuraRatioRandomizer		= m_HashDuraRatioRandomizer[index];
			pLimitValueRatioRandomizer	= m_HashLimitValueRatioRandomizer[index];
			pDamageRatioRandomizer		= m_HashDamageRatioRandomizer[index];

        #ifndef _NA_000251_20100727_SOCKET_SYSTEM
            pSocketRatioRandomizer		= m_HashSocketRatioRandomizer[index];
        #endif // _NA_000251_20100727_SOCKET_SYSTEM
            pGambleRatioRandomizer		= m_HashGambleRatioRandomizer[index];

			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", index );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_dwIndex			= index;

		_tcsncpy( pInfo->m_pszName, GetDataString( "Name", nRow ), MAX_LIMITED_ITEM_NAME_LENGTH );

		//	아이템 네이밍 색깔
		pInfo->m_bUse				= !!GetDataBYTE( "bUseItemNameColor", nRow );
		_tcsncpy( pInfo->m_szColor, GetDataString( "NameColorCode", nRow ), sLIMITED_ITEM_INFO::MAX_COLOR_LENGTH);

		pInfo->m_eWAAType			= (eITEM_WAA_TYPE)GetDataBYTE( "Type", nRow );

		pInfo->m_wEquipImage		= GetDataWORD( "EquipImageCode1", nRow );
		pInfo->m_dwInvenImage		= GetDataDWORD( "InvenImageCode1", nRow );

		{	//	내구도
			TCHAR durationRatioString[32] = { 0, };
			TCHAR durationMinString[32] = { 0, };
			TCHAR durationMaxString[32] = { 0, };

			const INT durationRatio_tail_idx = _sntprintf( durationRatioString, _countof(durationRatioString), "DuraRatio" );
			const INT durationMin_tail_idx = _sntprintf( durationMinString, _countof(durationMinString), "DuraMin" );
			const INT durationMax_tail_idx = _sntprintf( durationMaxString, _countof(durationMaxString), "DuraMax" );
			for( char i=0 ; i<MAX_DURA_RATIO_COUNT ; ++i )
			{
				durationRatioString[durationRatio_tail_idx] = '1' + i;
				durationMinString[durationMin_tail_idx] = '1' + i;
				durationMaxString[durationMax_tail_idx] = '1' + i;
				pInfo->m_dwDuraRatio[i]			= GetDataDWORD( durationRatioString, nRow );
				pInfo->m_iDura_Min[i]			= GetDataInt( durationMinString, nRow );
				pInfo->m_iDura_Max[i]			= GetDataInt( durationMaxString, nRow );
				pInfo->m_dwDuraRatioSum			+= pInfo->m_dwDuraRatio[i];
			}
		};

		{	//장비 제한 수치
			pInfo->m_dwLimitValueRatioSum = 0;

			TCHAR limitValueRatioString[32] = { 0, };
			TCHAR limitValueMinString[32] = { 0, };
			TCHAR limitValueMaxString[32] = { 0, };

			const INT limitValueRatio_tail_idx = _sntprintf( limitValueRatioString, _countof(limitValueRatioString), "LimitValueRatio" );
			const INT limitValueMin_tail_idx = _sntprintf( limitValueMinString, _countof(limitValueMinString), "LimitValueMin" );
			const INT limitValueMax_tail_idx = _sntprintf( limitValueMaxString, _countof(limitValueMaxString), "LimitValueMax" );
			for( char i=0 ; i<MAX_LIMIT_VALUE_RATIO_OPTION_COUNT ; ++i )
			{
				limitValueRatioString[limitValueRatio_tail_idx] = '1' + i;
				limitValueMinString[limitValueMin_tail_idx] = '1' + i;
				limitValueMaxString[limitValueMax_tail_idx] = '1' + i;
				pInfo->m_dwLimitValueRatio[i]	= GetDataDWORD( limitValueRatioString, nRow );
				pInfo->m_iLimitValue_Min[i]		= GetDataInt( limitValueMinString, nRow );
				pInfo->m_iLimitValue_Max[i]		= GetDataInt( limitValueMaxString, nRow );

				pInfo->m_dwLimitValueRatioSum	+= pInfo->m_dwLimitValueRatio[i];
			}
		};

		{	//데미지, 방어력
			pInfo->m_dwDamageRatioSum = 0;

			TCHAR damageRatioString[32] = { 0, };
			TCHAR damageMinString[32] = { 0, };
			TCHAR damageMaxString[32] = { 0, };

			const INT damageRatio_tail_idx = _sntprintf( damageRatioString, _countof(damageRatioString), "DamageRatio" );
			const INT damageMin_tail_idx = _sntprintf( damageMinString, _countof(damageMinString), "DamageMin" );
			const INT damageMax_tail_idx = _sntprintf( damageMaxString, _countof(damageMaxString), "DamageMax" );
			for( char i=0 ; i<MAX_DAMAGE_RATIO_COUNT ; ++i )
			{
				damageRatioString[damageRatio_tail_idx] = '1' + i;
				damageMinString[damageMin_tail_idx] = '1' + i;
				damageMaxString[damageMax_tail_idx] = '1' + i;
				pInfo->m_dwDamageRatio[i]		= GetDataDWORD( damageRatioString, nRow );
				pInfo->m_iDamage_Min[i]			= GetDataInt( damageMinString, nRow );
				pInfo->m_iDamage_Max[i]			= GetDataInt( damageMaxString, nRow );

				pInfo->m_dwDamageRatioSum += pInfo->m_dwDamageRatio[i];
			}
		};

		pInfo->m_byRepairLimite			= GetDataBYTE( "RepairLimitByCache", nRow );

    #ifndef _NA_000251_20100727_SOCKET_SYSTEM
    	{	//	소켓
			pInfo->m_dwSocketRatioSum = 0;

			TCHAR socketString[32] = { 0, };
			const INT socket_tail_idx = _sntprintf( socketString, _countof(socketString), "SocketRatio" );
			for( char i=0 ; i<MAX_LIMITED_SOCKET_COUNT ; ++i )
			{
				socketString[socket_tail_idx] = '0' + i;
				pInfo->m_dwSocketRatio[i] = GetDataDWORD( socketString, nRow );

				pInfo->m_dwSocketRatioSum += pInfo->m_dwSocketRatio[i];
			}
		};
    #endif // _NA_000251_20100727_SOCKET_SYSTEM

		{	//	겜블 옵션
			pInfo->m_dwGambleRatioSum = 0;

			TCHAR gambleRatioString[32] = { 0, };
			TCHAR gambleListString[32] = { 0, };
			const INT gambleRatio_tail_idx = _sntprintf( gambleRatioString, _countof(gambleRatioString), "GambleRatio" );
			const INT gambleList_row_idx = _sntprintf( gambleListString, _countof(gambleListString), "GamebleList" );
			gambleListString[gambleList_row_idx+1] = '_';
			const INT gambleList_col_idx = gambleList_row_idx+2;

			gambleRatioString[gambleRatio_tail_idx] = '0';
			pInfo->m_dwGambleRatio[0]		= GetDataDWORD( gambleRatioString, nRow );

			for( INT j=0 ; j<MAX_GAMBLE_OPTION_COUNT ; ++j )		
				pInfo->m_dwGambleList[0][j] = 0;
			pInfo->m_dwGambleRatioSum += pInfo->m_dwGambleRatio[0];

			for( char i=1 ; i<MAX_LIMITED_GAMBLE_COUNT ; ++i )
			{
				gambleRatioString[gambleRatio_tail_idx] = '0' + i;
				pInfo->m_dwGambleRatio[i]	= GetDataDWORD( gambleRatioString, nRow );
				for( char j = 0 ; j < MAX_GAMBLE_OPTION_COUNT ; ++j )
				{
					gambleListString[gambleList_row_idx] = '0' + i;
					gambleListString[gambleList_col_idx] = '1' + j;
					pInfo->m_dwGambleList[i][j] = GetDataDWORD( gambleListString, nRow );
				}

				pInfo->m_dwGambleRatioSum += pInfo->m_dwGambleRatio[i];
			}		
		};

		if (int check_result = ValidCheck(pInfo))
		{
			ASSERT(FALSE);
		}

		//랜덤 생성기에 입력
		for( BYTE i = 0 ; i < MAX_DURA_RATIO_COUNT ; i++)
		{
			pDuraRatioRandomizer->AddRatio( i+1, pInfo->m_dwDuraRatio[i], pInfo->m_dwDuraRatioSum );
			pDuraRatioRandomizer->SetMin( pInfo->m_iDura_Min[i], i+1 );
			pDuraRatioRandomizer->SetMax( pInfo->m_iDura_Max[i], i+1 );
		}

		for( BYTE i = 0 ; i < MAX_LIMIT_VALUE_RATIO_OPTION_COUNT ; i++)
		{
			pLimitValueRatioRandomizer->AddRatio( i+1, pInfo->m_dwLimitValueRatio[i], pInfo->m_dwLimitValueRatioSum );
			pLimitValueRatioRandomizer->SetMin( pInfo->m_iLimitValue_Min[i], i+1 );
			pLimitValueRatioRandomizer->SetMax( pInfo->m_iLimitValue_Max[i], i+1 );
		}

		for( BYTE i = 0 ; i < MAX_DAMAGE_RATIO_COUNT ; i++)
		{
			pDamageRatioRandomizer->AddRatio( i+1, pInfo->m_dwDamageRatio[i], pInfo->m_dwDamageRatioSum );
			pDamageRatioRandomizer->SetMin( pInfo->m_iDamage_Min[i], i+1 );
			pDamageRatioRandomizer->SetMax( pInfo->m_iDamage_Max[i], i+1 );
		}

    #ifndef _NA_000251_20100727_SOCKET_SYSTEM
        //소켓
		for( BYTE i = 0 ; i < MAX_LIMITED_SOCKET_COUNT ; i++)
		{
			pSocketRatioRandomizer->AddRatio( i+1, pInfo->m_dwSocketRatio[i], pInfo->m_dwSocketRatioSum );
		}
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
	
        //겜블옵션
		for( BYTE i = 0 ; i < MAX_LIMITED_GAMBLE_COUNT ; i++)
		{
			pGambleRatioRandomizer->AddRatio( i+1, pInfo->m_dwGambleRatio[i], 100 );
		}
	}

	return TRUE;
}

BOOL	LimitedItemInfoParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch( scriptCode )
	{
	case SCRIPT_LIMITED_ITEM_INFO:
		return _Load( bReload );
	}

	return FALSE;
}

WORD LimitedItemInfoParser::ValidCheck(sLIMITED_ITEM_INFO *pInfo)
{
	for( BYTE i = 0 ; i < MAX_DURA_RATIO_COUNT ; i++ )
	{
		if( pInfo->m_iDura_Min[i] < -100 || pInfo->m_iDura_Min[i] > 400 )
			return 1;

		if( pInfo->m_iDura_Max[i] < -100 || pInfo->m_iDura_Max[i] > 400 )
			return 2;
	}

	for( BYTE i = 0 ; i < MAX_LIMIT_VALUE_RATIO_OPTION_COUNT ; i++ )
	{
		if( pInfo->m_iLimitValue_Min[i] < -100 || pInfo->m_iLimitValue_Min[i] > 400 )
			return 3;

		if( pInfo->m_iLimitValue_Max[i] < -100 || pInfo->m_iLimitValue_Max[i] > 400 )
			return 4;
	}

	for( BYTE i = 0 ; i < MAX_DAMAGE_RATIO_COUNT ; i++ )
	{
		if( pInfo->m_iDamage_Min[i] < -100 || pInfo->m_iDamage_Min[i] > 400 )
			return 5;

		if( pInfo->m_iDamage_Max[i] < -100 || pInfo->m_iDamage_Max[i] > 400 )
			return 6;
	}

	return 0;
}

sLIMITED_ITEM_INFO * LimitedItemInfoParser::GetLimitedItemInfo( DWORD Index )
{
	sLIMITED_ITEM_INFO * pItemInfo = NULL;	
	LIMITED_ITEM_INFO_HASH::iterator it = m_HashLimitedItemInfo.find( Index );
	if ( it != m_HashLimitedItemInfo.end() )
	{
		pItemInfo = it->second;
		
		return pItemInfo;
	}

	return NULL;
}

INT	LimitedItemInfoParser::GetRandomkeyFromDuraRatio( DWORD Index )
{ 
	if( m_HashDuraRatioRandomizer.find( Index ) == m_HashDuraRatioRandomizer.end() )
		return 0;

	return m_HashDuraRatioRandomizer[Index]->GetRandomizedValue();
}
INT LimitedItemInfoParser::GetRandomkeyFromLimitValueRatio( DWORD Index )
{
	if( m_HashLimitValueRatioRandomizer.find( Index ) == m_HashLimitValueRatioRandomizer.end() )
		return 0;

	return m_HashLimitValueRatioRandomizer[Index]->GetRandomizedValue(); 
}
INT LimitedItemInfoParser::GetRandomkeyFromDamageRatio( DWORD Index )
{ 
	if( m_HashDamageRatioRandomizer.find( Index ) == m_HashDamageRatioRandomizer.end() )
		return 0;

	return m_HashDamageRatioRandomizer[Index]->GetRandomizedValue();
}

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
INT LimitedItemInfoParser::GetRandomkeyFromSocketRatio( DWORD Index )
{ 
	if( m_HashSocketRatioRandomizer.find( Index ) == m_HashSocketRatioRandomizer.end() )
		return 0;

	return m_HashSocketRatioRandomizer[Index]->RandomizedKey() - 1; 
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

INT LimitedItemInfoParser::GetRandomkeyFromGambleRatio( DWORD Index )
{
	if( m_HashGambleRatioRandomizer.find( Index ) == m_HashGambleRatioRandomizer.end() )
		return 0;

	return m_HashGambleRatioRandomizer[Index]->RandomizedKey(); 
}

sLIMITED_ITEM_INFO*	LimitedItemInfoParser::GetNext()
{
	sLIMITED_ITEM_INFO* pRetInfo;

	if( m_LimitedItemInfoIterator != m_HashLimitedItemInfo.end())
	{
		pRetInfo = m_LimitedItemInfoIterator->second;
		m_LimitedItemInfoIterator++;
		return pRetInfo;
	}

	return NULL;
}