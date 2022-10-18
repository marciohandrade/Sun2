#include "SunClientPrecompiledHeader.h"
#include "WeaponSoundInfoParser.h"


WeaponSoundInfoParser::WeaponSoundInfoParser():	m_pDataTable ( NULL )
{
}

WeaponSoundInfoParser::~WeaponSoundInfoParser()
{
	assert( m_pDataTable == NULL);
}

VOID WeaponSoundInfoParser::Release()
{
	Unload();
    if (m_pDataTable)
	    delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID WeaponSoundInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_WeaponSoundInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID WeaponSoundInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID WeaponSoundInfoParser::Unload()
{
	BASE_WeaponSoundInfo * pInfo = NULL;
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

BASE_WeaponSoundInfo* WeaponSoundInfoParser::GetWeaponSoundInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL WeaponSoundInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_WEAPON_SOUND_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        BASE_WeaponSoundInfo* info = new BASE_WeaponSoundInfo;
        assert(info);

        info->m_bWeaponType = GetDataNumber("dWeaponKind", row);

        for (int i = 0; i < MAX_WEAPON_SOUND_VARIATION; ++i)
        {
            char colName[24];
            sprintf(colName, "dSound%d", i + 1);

            info->m_dWeaponSound[i] = GetDataNumber(colName, row);            
        }

        assert(!m_pDataTable->GetData(info->m_bWeaponType));
        m_pDataTable->Add(info, info->m_bWeaponType);
    }

    return TRUE;
}


#pragma warning ( pop )
