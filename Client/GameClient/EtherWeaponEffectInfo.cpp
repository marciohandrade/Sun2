#include "SunClientPrecompiledHeader.h"


#include "EtherWeaponEffectInfo.h"


EtherWeaponEffectInfoParser::EtherWeaponEffectInfoParser():	m_pDataTable ( NULL )
{
}

EtherWeaponEffectInfoParser::~EtherWeaponEffectInfoParser()
{
	assert( m_pDataTable == NULL);
}

VOID EtherWeaponEffectInfoParser::Release()
{
	Unload();
	if (m_pDataTable)
		delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID EtherWeaponEffectInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_EtherWeaponEffect*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID EtherWeaponEffectInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID EtherWeaponEffectInfoParser::Unload()
{
	BASE_EtherWeaponEffect * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}

BASE_EtherWeaponEffect* EtherWeaponEffectInfoParser::GetEtherWeponEffectInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL EtherWeaponEffectInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch(ScriptCode)
	{
	case SCRIPT_ETHER_WEAPON_EFFECT:
		{
			return _Load(bReload);
		}
		break;
	}
	return FALSE;
}
BOOL EtherWeaponEffectInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
		const char * pTemp  = GetDataString("WeaponName", nRow);
		BASE_EtherWeaponEffect * pInfo = new BASE_EtherWeaponEffect;
		pInfo->byWeaponType = GetDataNumber("WeaponKind",nRow);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pInfo->weapon_sub_type_ = GetDataNumber("DualWeaponEffect", nRow);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		pInfo->dwTargetBone = StrToWzID(GetDataString("BoneName",nRow));

		char szTemp[MAX_FIELD_NAME_SIZE] = {0,};
		for (int a = 0; a < MAX_ETHER_EFFECT; a++)
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "EffectCode%d\0", a+1 );
			pInfo->dwEffectCode[a] = StrToWzID ( GetDataString(szTemp,nRow));
		}

		pInfo->wvOffsetPos.x = GetDataNumber("PosX",nRow);
		pInfo->wvOffsetPos.y = GetDataNumber("PosY",nRow);
		pInfo->wvOffsetPos.z = GetDataNumber("PosZ",nRow);

		pInfo->wvOffsetRot.x = (float) -GetDataNumber("RotX",nRow);
		pInfo->wvOffsetRot.y = (float) -GetDataNumber("RotY",nRow);
		pInfo->wvOffsetRot.z = (float) -GetDataNumber("RotZ",nRow);

		pInfo->wvScale.x = (float) GetDataNumber("ScaleX",nRow);
		pInfo->wvScale.y = (float) GetDataNumber("ScaleY",nRow);
		pInfo->wvScale.z = (float) GetDataNumber("ScaleZ",nRow);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        assert( NULL == m_pDataTable->GetData( MAKEDWORD(pInfo->byWeaponType, pInfo->weapon_sub_type_) ) );
        m_pDataTable->Add( pInfo, MAKEDWORD( pInfo->byWeaponType, pInfo->weapon_sub_type_) );
#else
		assert( NULL == m_pDataTable->GetData( pInfo->byWeaponType ) );
		m_pDataTable->Add( pInfo, pInfo->byWeaponType );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	}
	return TRUE;
}


#pragma warning ( pop )
