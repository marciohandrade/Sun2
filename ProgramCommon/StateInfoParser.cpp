#include "StdAfx.h"
#include "StateInfoParser.h"
#include <Struct.h>
#include <iostream>


StateInfoParser::StateInfoParser() : m_pStateInfoHashTable( NULL )
{
}

StateInfoParser::~StateInfoParser()
{
	FASSERT( m_pStateInfoHashTable == NULL );
}

void
StateInfoParser::Init(DWORD dwPoolSize)
{
	FASSERT( m_pStateInfoHashTable == NULL );
	m_pStateInfoHashTable = new util::SolarHashTable<BASE_STATEINFO *>;
	m_pStateInfoHashTable->Initialize( dwPoolSize );
}


void
StateInfoParser::Init(DWORD dwPoolSize, char* pszPackFileName)
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

void
StateInfoParser::Release()
{
	Unload();

	SAFE_DELETE( m_pStateInfoHashTable );
}

void
StateInfoParser::Unload()
{
    if (m_pStateInfoHashTable)
    {
        // CHANGES: f110427.4L, fixed invalid iterating usages.
        // this iterating with containing object need offset control
        m_pStateInfoHashTable->SetFirst();
        while (BASE_STATEINFO* info = m_pStateInfoHashTable->GetNext()) {
            delete info;
        }
        m_pStateInfoHashTable->RemoveAll();
    }
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

void
StateInfoParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}


BOOL
StateInfoParser::_Load(BOOL bReload)
{
	int nRowSize = GetRowSize();

    // (WAVERIX) (NOTE) GetDataNumber -> GetData%Type%으로 변환
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{

        DWORD dwID = GetDataDWORD("Index", nRow);
        FASSERT(dwID <= USHRT_MAX);

		BASE_STATEINFO * pInfo = m_pStateInfoHashTable->GetData( dwID );
		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new BASE_STATEINFO;
            ZeroMemory(pInfo, sizeof(*pInfo)); // (WAVERIX) (NOTE) 클리어 추가
			m_pStateInfoHashTable->Add( pInfo, dwID );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, StateInfoParser, ID = %d", dwID );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}
        pInfo->m_wStateID = (WORD)dwID;

		strncpy( pInfo->m_szStateName, GetDataString("StateName",nRow), MAX_SKILLNAME_LENGTH );
        pInfo->m_dwNamecode = GetDataDWORD("NameCode", nRow);
        pInfo->m_dwDescCode = GetDataDWORD("DescriptionCode", nRow);
        pInfo->m_dwIconCode = GetDataDWORD("IconCode", nRow);
#ifdef _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
        pInfo->m_byIconRow = GetDataBYTE("Row", nRow);
        pInfo->m_byIconColumn = GetDataBYTE("Column", nRow);
#endif //_100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
        pInfo->m_byIconKind = GetDataBYTE("GKind", nRow);
        pInfo->m_bDelType   = GetDataBYTE("DelType", nRow);
        pInfo->m_byType     = GetDataBYTE("Type", nRow);
        // added by __NA_001290_20090525_SHIELD_SYSTEM
        // TODO, waverix, status types check each of local routine, need redesign it
        pInfo->m_bySDApply  = GetDataBYTE("SDApply", nRow); // SD 적용여부
        pInfo->m_bRidingApply = !!GetDataBYTE("RidingApply", nRow);
		strncpy( pInfo->m_wzidEffectID.code,  GetDataString("EffectCode",nRow), 4 );
		if (pInfo->m_wzidEffectID.id == StrToWzID("NULL"))
		{
			pInfo->m_wzidEffectID.id = 0;
		}

        pInfo->m_byEffectPos = GetDataBYTE("EffectPos", nRow);
        pInfo->m_wGroupCode  = GetDataWORD("GroupCode", nRow);
        // added by _NA_000587_20100928_DOMINATION_BUFF_ITEM = {
        pInfo->m_wBoneIndex = GetDataWORD("BoneIndex", nRow);
        pInfo->m_fOffsetX = GetDataFloat("OffsetX", nRow);
        pInfo->m_fOffsetY = GetDataFloat("OffsetY", nRow);
        pInfo->m_fOffsetZ = GetDataFloat("OffsetZ", nRow);
        pInfo->m_bApplyParentRotate = (GetDataInt("ApplyParentRotate", nRow) != 0);
        pInfo->m_bApplyParentScale = (GetDataInt("ApplyParentScale", nRow) != 0);
        // }
	}

	return TRUE;
}

//#pragma warning ( pop )

BOOL
StateInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch( ScriptCode )
	{
	case SCRIPT_STATEINFO:
		return _Load( bReload );
	}

	return FALSE; 
}

