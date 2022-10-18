//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroVoiceInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
HeroVoiceInfoParser::HeroVoiceInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
HeroVoiceInfoParser::~HeroVoiceInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVoiceInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVoiceInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVoiceInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVoiceInfoParser::Unload()
{
	BASE_HeroVoiceInfo * pInfo = NULL;
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

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_HeroVoiceInfo* HeroVoiceInfoParser::GetHeroVoiceInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

void _VOICEINFO::Load(util::SolarFileReader &sr)
{
	sr.GetNextTokenType(TOKEN_NUMBER); dwVoiceID = sr.GetTokenNumber();
	sr.GetNextTokenType(TOKEN_NUMBER); byVoiceType = sr.GetTokenNumber();
	sr.GetNextTokenType(TOKEN_NUMBER); dwTextID = sr.GetTokenNumber();
	sr.GetNextTokenType(TOKEN_NUMBER); byTextType = sr.GetTokenNumber();
}

void _VOICEINFO::Load(IParser* parser, const char* prefix, int row)
{
    assert(parser);
    assert(prefix);
    assert(row >= 0);

    char colName[32];

    // voice ID
    sprintf(colName, "%s", prefix);
    dwVoiceID = parser->GetDataNumber(colName, row); 

    // voice type
    sprintf(colName, "%s_VoiceType", prefix);
    byVoiceType = parser->GetDataNumber(colName, row);

    // text id
    sprintf(colName, "%s_Text", prefix);
    dwTextID = parser->GetDataNumber(colName, row);

    // text type
    sprintf(colName, "%s_TextType", prefix);
    byTextType = parser->GetDataNumber(colName, row);
}


BOOL HeroVoiceInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_HERO_VOICE_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        BASE_HeroVoiceInfo* info = new BASE_HeroVoiceInfo;
        assert(info);

        info->m_w_CharacterCode = GetDataNumber("CharacterCode", row);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        info->m_w_CharacterGender = GetDataNumber("GenderType", row);
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        strncpy(info->m_s_Desc, GetDataString("Desc", row), 64);

        info->m_LimitUse.Load(this, "LimitUse", row);
        info->m_NoTarget1.Load(this, "NOTAR1", row);
        info->m_NoTarget2.Load(this, "NOTAR2", row);
        info->m_LackMon.Load(this, "LackMon", row);
        info->m_NOREPItem.Load(this, "NOREPItem", row);     
        info->m_LackSP.Load(this, "LackSP", row);
        info->m_LackMP.Load(this, "LackMP", row);
        info->m_LackPoint.Load(this, "LackPoi", row);
        info->m_FarTarget1.Load(this, "FarTag1", row);
        info->m_FarTarget2.Load(this, "FarTag2", row);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        info->lack_fp_.Load(this, "LackFP", row);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        HeroVoiceCode voice_code;
        voice_code.wCharacterCode = info->m_w_CharacterCode;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        voice_code.wCharacterGender = info->m_w_CharacterGender;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

        //assert(!m_pDataTable->GetData(voice_code.dwVoiceCode));
        m_pDataTable->Add(info, voice_code.dwVoiceCode);
    }

    return TRUE;
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
DWORD HeroVoiceInfoParser::GetVoiceInfoKeyCode(eCHAR_TYPE character_type, GenderType::Value gender_type)
{
    HeroVoiceCode voice_code;
    voice_code.wCharacterCode = character_type;
    voice_code.wCharacterGender = gender_type;
    
    return voice_code.dwVoiceCode;
}
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#pragma warning ( pop )
