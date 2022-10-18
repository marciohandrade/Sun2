#include "StdAfx.h"
#include ".\PetInfoParser.h"
#include <ItemOptionHeader.h>


PetInfoParser::PetInfoParser(void)
{

}

PetInfoParser::~PetInfoParser(void)
{
	Unload();
}

VOID PetInfoParser::Release()
{
	Unload();
}

VOID PetInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID PetInfoParser::Unload()
{
	PET_SCRIPT_INFO_HASH::iterator it, end( m_HashPetInfo.end() );
	for( it = m_HashPetInfo.begin() ; it != end ; ++it)
	{
		sPet_Script_Info * pInfo = (it->second);
		SAFE_DELETE( pInfo );
	}
	m_HashPetInfo.clear();	
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL PetInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		DWORD dwIndex = GetDataDWORD( "dwIndex", nRow );
		 
		sPet_Script_Info *		pInfo = NULL;

		PET_SCRIPT_INFO_HASH::iterator itr = m_HashPetInfo.find( dwIndex );
		if ( itr == m_HashPetInfo.end() )
		{
			// 없으면!
			pInfo											= new sPet_Script_Info;

			memset(pInfo, 0, sizeof(sPet_Script_Info));
			m_HashPetInfo[dwIndex]							= pInfo;			
		}
		else
		{
			pInfo = itr->second;

			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %u", dwIndex );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}				

		pInfo->m_wIndexCode			= dwIndex;
        
		strncpy( pInfo->m_pszName, GetDataString( "Name", nRow ), MAX_MONSTERNAME_LENGTH );
		pInfo->m_dwNameCode			= GetDataDWORD( "NameCode", nRow );
		pInfo->m_MonsterCode		= GetDataWORD( "wMonsterCode", nRow );

#ifdef _GS_ADD_PET_ENCHANT_EFFECT
        pInfo->effect_subtype = GetDataWORD( "EffectType", nRow );         
#endif//_GS_ADD_PET_ENCHANT_EFFECT
		
		pInfo->m_byFullnessUp		= GetDataBYTE( "byFullnessUp", nRow );
		pInfo->m_byFullnessDown[0]	= GetDataBYTE( "byFullnessDown1", nRow );
		pInfo->m_byFullnessDown[1]	= GetDataBYTE( "byFullnessDown2", nRow );

		pInfo->m_byIntimacyUp		= GetDataBYTE( "byIntimacyUp", nRow );
		pInfo->m_byIntimacyDown[0]	= GetDataBYTE( "byIntimacyDown1", nRow );
		pInfo->m_byIntimacyDown[1]	= GetDataBYTE( "byIntimacyDown2", nRow );

		pInfo->m_byAIType			= GetDataBYTE( "byAIType", nRow );		
#ifdef __CN_PET_OPTION_APPLY_FLAG
		pInfo->m_eApplyOptionFlag	= (sPet_Script_Info::ePET_OPTION_FLAG)GetDataInt( "Cumulate", nRow );
#endif

		//클라이언트 연출용(펫 고정UI & 펫시작위치)
        pInfo->m_byPdisplayMove		= GetDataBYTE( "PdisplayMove", nRow );
		pInfo->m_byPpossition		= GetDataBYTE( "Pposition", nRow );
	
		TCHAR szTemp[MAX_FIELD_NAME_SIZE];

		for( int i = 0 ; i < MAX_PET_OPTION_EXTENT_COUNT; i++)
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "byIntimacyMin%d\0", i+1 );
			pInfo->m_byIntimacyMin[i]	= GetDataBYTE( szTemp, nRow );

			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "byIntimacyMax%d\0", i+1 );
			pInfo->m_byIntimacyMax[i]	= GetDataBYTE( szTemp, nRow );

			for( int j = 0 ; j < MAX_PET_OPTION_COUNT; j++)
			{
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "wPet%dOptionKind%d\0", i+1, j+1 );
				pInfo->m_PetOptionInfo[i].m_wOptionKind[j]	= GetDataWORD( szTemp, nRow );

				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "byPet%dValueType%d\0", i+1, j+1 );
				pInfo->m_PetOptionInfo[i].m_byValueType[j]	= GetDataBYTE( szTemp, nRow );

				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "iPet%dValue%d\0", i+1, j+1 );
				pInfo->m_PetOptionInfo[i].m_iValue[j]	= GetDataInt( szTemp, nRow );
			}
		}

		if( !ValidCheck( pInfo ) )
			return FALSE;
	}

	return TRUE;
}
//#pragma warning ( pop )


BOOL PetInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_PET_INFO:
		return _Load( bReload );
	}

	return FALSE;
}

BOOL PetInfoParser::ValidCheck(sPet_Script_Info *pInfo)
{
	for( int i = 0 ; i < MAX_PET_OPTION_EXTENT_COUNT; i++)
	{
		for( int j = 0 ; j < MAX_PET_OPTION_COUNT; j++)
		{
			eATTR_TYPE AttrType = (eATTR_TYPE)pInfo->m_PetOptionInfo[i].m_wOptionKind[j];
			BYTE NumericType = pInfo->m_PetOptionInfo[i].m_byValueType[j];
			int OptionValue = pInfo->m_PetOptionInfo[i].m_iValue[j];
			if( NumericType != VALUE_TYPE_VALUE )	OptionValue = OptionValue / 10;

			// 비율타입 옵션 체크
			if( !CheckOptionValidity( AttrType, NumericType, OptionValue ) )
			{
                SUNLOG(eCRITICAL_LOG,
                       "|[PetInfo]|Invalid Option|Index=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|", 
                       pInfo->m_wIndexCode, AttrType, NumericType, OptionValue );
			}
		}
	}

#ifdef __CN_PET_OPTION_APPLY_FLAG
	//(shogen)(20090825)
	if( pInfo->m_eApplyOptionFlag < 0 || pInfo->m_eApplyOptionFlag >= sPet_Script_Info::ePET_OPTION_FLAG_MAX )
	{
		ASSERT(FALSE);
		SUNLOG( eCRITICAL_LOG, "[PetInfoParser] Index(%d), Invalid m_eApplyOptionFlag(%d)",
			pInfo->m_wIndexCode, pInfo->m_eApplyOptionFlag );
	}
#endif
	
	return TRUE;
}

sPet_Script_Info * PetInfoParser::GetPetInfo( DWORD Index )
{
	sPet_Script_Info * pPetInfo = NULL;	
	PET_SCRIPT_INFO_HASH::iterator it = m_HashPetInfo.find( Index );
	if ( it != m_HashPetInfo.end() )
	{
		pPetInfo = it->second;

		return pPetInfo;
	}

	return NULL;
}

int PetInfoParser::GetOptionLevel( WORD wPetIndex , int iCurIntimacy)
{
	sPet_Script_Info * pPetInfo = GetPetInfo(wPetIndex);

	assert(pPetInfo && "펫인덱스없음");

	if(!pPetInfo) return 0;

	for( int lv = 0 ; lv < MAX_PET_OPTION_EXTENT_COUNT; lv++)
	{
		if( pPetInfo->m_byIntimacyMin[lv] <= iCurIntimacy && iCurIntimacy <= pPetInfo->m_byIntimacyMax[lv])
			return lv;
	}
		
	return 0;
}
sPet_Script_Info* PetInfoParser::GetNext()
{
	sPet_Script_Info* pPetInfo;

	if( m_HashPetInfoIterator != m_HashPetInfo.end())
	{
		pPetInfo = m_HashPetInfoIterator->second;
		m_HashPetInfoIterator++;
		return pPetInfo;
	}

	return NULL;
}

