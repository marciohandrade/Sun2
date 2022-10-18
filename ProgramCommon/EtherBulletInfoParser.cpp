#include "StdAfx.h"
#include ".\EtherBulletInfoParser.h"
#include <ItemOptionHeader.h>

EtherBulletInfoParser::EtherBulletInfoParser(void)
{

}

EtherBulletInfoParser::~EtherBulletInfoParser(void)
{
	Unload();
}

VOID EtherBulletInfoParser::Release()
{
	Unload();
}

VOID EtherBulletInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID EtherBulletInfoParser::Reload()
{
}

VOID EtherBulletInfoParser::Unload()
{
	ETHER_BULLET_INFO_HASH::iterator it, end( m_EtherBulletInfoIndex.end() );
	for( it = m_EtherBulletInfoIndex.begin() ; it != end ; ++it)
	{
		tagEtherBulletInfo * pInfo = (it->second);
		SAFE_DELETE( pInfo );
	}
	
	m_EtherBulletInfoIndex.clear();	
	m_EtherBulletInfoItemCode.clear();
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL EtherBulletInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_ETHER_BULLET_INFO:
		{
			return _Load(bReload);
		}
		break;
	}

	return FALSE;
}
BOOL EtherBulletInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{

		DWORD dwBulletID = GetDataNumber("BulletID",nRow);		
		DWORD dwItemCode = GetDataNumber("Code", nRow);
		tagEtherBulletInfo *		pInfo = NULL;
		ETHER_BULLET_INFO_HASH::iterator itr = m_EtherBulletInfoIndex.find( dwBulletID );
		if ( itr == m_EtherBulletInfoIndex.end() )
		{
			// 없으면!
			pInfo												= new tagEtherBulletInfo;
			memset(pInfo, 0, sizeof(tagEtherBulletInfo));

			m_EtherBulletInfoIndex[dwBulletID]					= pInfo;
			m_EtherBulletInfoItemCode[dwItemCode]				= pInfo;
		}
		else
		{
			pInfo = itr->second;
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, Code = %d", m_pszFileName, dwBulletID );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_wBulletID	= dwBulletID;
		pInfo->m_ItemCode	= dwItemCode;
		pInfo->m_dwEffect	= GetDataNumber("Effect",nRow);

		pInfo->m_byEffectCode = GetDataNumber("EffectCode", nRow);

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( int i = 0 ; i < tagEtherBulletInfo::eETHER_DEVICE_COMBO_MAX ; i++)
		{
			for( int j = 0 ; j < MAX_BULLET_OPTION_COUNT; j++)
			{
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "OptionKind%d_%d\0", j+1, i+1 );
				pInfo->m_EtherBulletOption[i].m_wOptionKind[j] = GetDataWORD(szTemp, nRow);

				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "UseType%d_%d\0", j+1, i+1 );
				pInfo->m_EtherBulletOption[i].m_byValueType[j] = GetDataBYTE(szTemp, nRow);

				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "Option%d_%d\0", j+1, i+1 );
				pInfo->m_EtherBulletOption[i].m_iValue[j]		= GetDataInt(szTemp, nRow);

                //_NA_008456_20150819_ETHER_BULLET_PVP_CHECK
                if (g_ITEM_OPTION[pInfo->m_EtherBulletOption[i].m_wOptionKind[j]] == eATTR_OPTION_ETHER_DAMAGE_RATIO)
                {
                    pInfo->apply_pvp_damage_ = TRUE;
                }
                else if (g_ITEM_OPTION[pInfo->m_EtherBulletOption[i].m_wOptionKind[j]] == eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO)
                {
                    pInfo->apply_pvp_damage_ = FALSE;
                }
			}
		}

		for( int i = 0 ; i < tagEtherBulletInfo::eETHER_DEVICE_COMBO_MAX - 1 ; i++)
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "AttackSpeed%d\0", i+1 );
			pInfo->m_iAttackSpeed[i] = GetDataInt(szTemp, nRow);
		}		
	}
	return TRUE;
}

WORD EtherBulletInfoParser::ValidCheck(tagEtherBulletInfo *pInfo)
{
    for( int i = 0 ; i < tagEtherBulletInfo::eETHER_DEVICE_COMBO_MAX ; i++)
	{
		for( int j = 0 ; j < MAX_BULLET_OPTION_COUNT; j++)
		{
			// 비율타입 옵션 체크
			if( !CheckOptionValidity( pInfo->m_EtherBulletOption[i].m_wOptionKind[i],
										pInfo->m_EtherBulletOption[i].m_byValueType[i],
										pInfo->m_EtherBulletOption[i].m_iValue[i] ) )
			{
				SUNLOG(eCRITICAL_LOG,
                       "|[EtherBulletInfo]|Invalid Option|"
                       "Index=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|", 
                       pInfo->m_wBulletID,
                       pInfo->m_EtherBulletOption[i].m_wOptionKind[i],
                       pInfo->m_EtherBulletOption[i].m_byValueType[i],
                       pInfo->m_EtherBulletOption[i].m_iValue[i]);
			}
		}
	}

	return 0;
}

tagEtherBulletInfo* EtherBulletInfoParser::GetEtherBulletInfoByIndex( DWORD Index )
{
	tagEtherBulletInfo * pEtherBulletInfo = NULL;
	ETHER_BULLET_INFO_HASH::iterator it = m_EtherBulletInfoIndex.find( Index );
	if ( it != m_EtherBulletInfoIndex.end() )
	{
		pEtherBulletInfo = it->second;

		return pEtherBulletInfo;
	}

	return NULL;
}

tagEtherBulletInfo* EtherBulletInfoParser::GetEtherBulletInfoByItemCode( DWORD ItemCode )
{
	tagEtherBulletInfo * pEtherBulletInfo = NULL;
	ETHER_BULLET_INFO_HASH::iterator it = m_EtherBulletInfoItemCode.find( ItemCode );
	if ( it != m_EtherBulletInfoItemCode.end() )
	{
		pEtherBulletInfo = it->second;

		return pEtherBulletInfo;
	}

	return NULL;
}

tagEtherBulletInfo* EtherBulletInfoParser::GetNext()
{
	tagEtherBulletInfo* pEtherBulletInfo;

	if( m_HashEtherBulletInfoIterator != m_EtherBulletInfoIndex.end())
	{
		pEtherBulletInfo = m_HashEtherBulletInfoIterator->second;
		m_HashEtherBulletInfoIterator++;

		return pEtherBulletInfo;
	}

	return NULL;
}

