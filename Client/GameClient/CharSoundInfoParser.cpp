#include "SunClientPrecompiledHeader.h"
#include "CharSoundInfoParser.h"
#include <SolarFileReader.h>


CharSoundInfoParser::CharSoundInfoParser():	m_pDataTable ( NULL )
{
}

CharSoundInfoParser::~CharSoundInfoParser()
{
	assert( m_pDataTable == NULL);
}

VOID CharSoundInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID CharSoundInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_CharSoundInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID CharSoundInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID CharSoundInfoParser::Unload()
{
	BASE_CharSoundInfo * pInfo = NULL;
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

BASE_CharSoundInfo* CharSoundInfoParser::GetCharSoundInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL CharSoundInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_CHAR_SOUND_INFO:
		{
			return _Load(bReload);
		}
		break;
	}
	return FALSE;
}

BOOL CharSoundInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
        // 사운드인포 키 만들기
        SOUND_INFO_CODE SoundInfoCode;
        SoundInfoCode.wCharacterCode = GetDataWORD("ResCode",nRow);
        SoundInfoCode.bWeaponType = GetDataBYTE("WeaponType",nRow);
        SoundInfoCode.bGenderType = GetDataBYTE("GenderType",nRow);

		BASE_CharSoundInfo* pInfo = NULL;
		pInfo = m_pDataTable->GetData(SoundInfoCode.dwSoundCode);
		if (pInfo != NULL)
		{
            assert("CharSoundInfoParser duplicated");
            continue;
		}
        pInfo = new BASE_CharSoundInfo;
        m_pDataTable->Add(pInfo, SoundInfoCode.dwSoundCode);

        // 사운드인포 키
        pInfo->SoundInfoCode = SoundInfoCode;

        pInfo->SoundInfo.bWeaponType = SoundInfoCode.bWeaponType;
        pInfo->SoundInfo.bGenderType = SoundInfoCode.bGenderType;

        // 디버그용 이름
        strncpy(pInfo->szCharacterName, GetDataString("CharName", nRow), SOUND_CHARNAME_LENGTH);
        pInfo->szCharacterName[SOUND_CHARNAME_LENGTH - 1] = '\0';

        // 사운드코드
        TCHAR szFieldName[MAX_FIELD_NAME_SIZE] = {0,};
        for (int index = 0; index < MAX_SOUND_VARIATION; index++)
        {
            _snprintf(szFieldName, MAX_FIELD_NAME_SIZE, "ArmorSound%d\0", index + 1);
            pInfo->SoundInfo.dwArmorSound[index] = GetDataNumber(szFieldName , nRow);
        }

        for (int index = 0; index < MAX_SOUND_VARIATION; index++)
        {
            _snprintf(szFieldName, MAX_FIELD_NAME_SIZE, "MoanSound%d\0", index + 1);
            pInfo->SoundInfo.dwMoanSound[index]= GetDataNumber(szFieldName , nRow);
        }

        for (int index = 0; index < MAX_SOUND_VARIATION; index++)
        {

            _snprintf(szFieldName, MAX_FIELD_NAME_SIZE, "SMoanSound%d\0", index+1);
            pInfo->SoundInfo.dwSMoanSound[index] = GetDataNumber(szFieldName, nRow);
        }
	}
	return TRUE;
}


#pragma warning ( pop )
