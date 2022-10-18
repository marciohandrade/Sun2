#include "StdAfx.h"
#include ".\itemcompositeparser.h"

#include "itemInfoParser.h"

ItemCompositeParser::ItemCompositeParser(void) 
: m_pCompositeInfoHashTable(0)
{
}

ItemCompositeParser::~ItemCompositeParser(void)
{
	FASSERT( m_pCompositeInfoHashTable == NULL );
	Release();
}


VOID ItemCompositeParser::Init( DWORD dwCompositeInfoSize )
{
	FASSERT( m_pCompositeInfoHashTable == NULL );
	m_pCompositeInfoHashTable = new util::SolarHashTable<sITEMCOMPOSITE *>;
	m_pCompositeInfoHashTable->Initialize( dwCompositeInfoSize );
}

VOID ItemCompositeParser::Init( DWORD dwCompositeInfoSize, char * pszPackFileName )
{
	Init( dwCompositeInfoSize  );
	SetPackFileName( pszPackFileName );
}


VOID ItemCompositeParser::Release()
{
	Unload();

	delete m_pCompositeInfoHashTable;
	m_pCompositeInfoHashTable = NULL;
}


BOOL	ItemCompositeParser::_Load( BOOL bReload )
{
	const INT nRowSize = GetRowSize();
	for( INT nRow=0; nRow<nRowSize ; ++nRow )
	{
		const SLOTCODE code			= GetDataWORD( "Code", nRow );
		sITEMCOMPOSITE* pInfo		= m_pCompositeInfoHashTable->GetData( code );
		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new sITEMCOMPOSITE;
			m_pCompositeInfoHashTable->Add( pInfo, code );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", code );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}
		pInfo->m_Code = code;

		// kind
		pInfo->m_byResult			= GetDataBYTE( "Result", nRow );
		if( 0 != pInfo->m_byResult )
		{
			//FASSERT( GetCompositeResultInfo(pInfo->m_byResult) && "아이템 조합 조건중에 Result 넘버가 잘못들어갔다. 확인바람!!");
		}
		pInfo->m_byMatCount			= GetDataBYTE( "MaterialCount", nRow );

		BYTE debug_check_material_count = 0;
		{	// Materials 재료물...
			TCHAR materialCodeString[32] = { 0, };
			TCHAR materialCountString[32] = { 0, };

			const INT material_code_tail_idx = _sntprintf( materialCodeString, _countof(materialCodeString), "MaterialCode" );
			const INT material_count_tail_idx = _sntprintf( materialCountString, _countof(materialCountString), "MaterialCount" );
			for( char i=0 ; i<sITEMCOMPOSITE::MAX_COMPOSITE_MATERIAL ; ++i )
			{
				materialCodeString[material_code_tail_idx] = '1' + i;
				materialCountString[material_count_tail_idx] = '1' + i;
				pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode	= GetDataWORD( materialCodeString, nRow );
				pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum		= GetDataBYTE( materialCountString, nRow );
				if( 0 != pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode )
				{
					FASSERT( 0 != pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum );
					++debug_check_material_count;
				}
			}
		};
	//	FASSERT( debug_check_material_count == pInfo->m_byMatCount );

		pInfo->m_Money				= (MONEY)GetDataNumber( "Money", nRow );	// <--- 음... 이 타입은 어캐 읽는 지 확인해 볼 것.
	}

	return TRUE;
}


VOID ItemCompositeParser::Reload()
{
//	Load(this->m_pszFileName, TRUE);
}


VOID ItemCompositeParser::Unload()
{
	// 조합정보 내림
	if( m_pCompositeInfoHashTable )
	{
		m_pCompositeInfoHashTable->SetFirst();
		while(sITEMCOMPOSITE * pCompositeInfo = m_pCompositeInfoHashTable->GetNext())
		{
			SAFE_DELETE( pCompositeInfo );
		}
		m_pCompositeInfoHashTable->RemoveAll();
	}

}

BOOL		ItemCompositeParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch( scriptCode )
	{
	case SCRIPT_ITEMCOMPOSITEINFO:
		return _Load( bReload );
	}

	return FALSE;	
}

