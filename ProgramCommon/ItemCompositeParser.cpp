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
			// ������!
			pInfo = new sITEMCOMPOSITE;
			m_pCompositeInfoHashTable->Add( pInfo, code );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", code );
				FASSERT( !"�����Ϳ� ������ �ֽ��ϴ�." );
			}
		}
		pInfo->m_Code = code;

		// kind
		pInfo->m_byResult			= GetDataBYTE( "Result", nRow );
		if( 0 != pInfo->m_byResult )
		{
			//FASSERT( GetCompositeResultInfo(pInfo->m_byResult) && "������ ���� �����߿� Result �ѹ��� �߸�����. Ȯ�ιٶ�!!");
		}
		pInfo->m_byMatCount			= GetDataBYTE( "MaterialCount", nRow );

		BYTE debug_check_material_count = 0;
		{	// Materials ��Ṱ...
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

		pInfo->m_Money				= (MONEY)GetDataNumber( "Money", nRow );	// <--- ��... �� Ÿ���� ��ĳ �д� �� Ȯ���� �� ��.
	}

	return TRUE;
}


VOID ItemCompositeParser::Reload()
{
//	Load(this->m_pszFileName, TRUE);
}


VOID ItemCompositeParser::Unload()
{
	// �������� ����
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

