//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroVoiceInfoParser.h"
#include "MonsterVoiceInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
MonsterVoiceInfoParser::MonsterVoiceInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
MonsterVoiceInfoParser::~MonsterVoiceInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MonsterVoiceInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MonsterVoiceInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_MonsterVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MonsterVoiceInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_MonsterVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MonsterVoiceInfoParser::Unload()
{
	BASE_MonsterVoiceInfo * pInfo = NULL;
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
BASE_MonsterVoiceInfo* MonsterVoiceInfoParser::GetMonsterVoiceInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL MonsterVoiceInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		BASE_MonsterVoiceInfo * pInfo = new BASE_MonsterVoiceInfo;
		pInfo->m_w_Code = GetDataNumber("w_Code", nRow);
		strncpy( pInfo->m_s_Desc, GetDataString("s_Desc", nRow), MONSTER_VOICE_LENGTH );

		int a;
		char szTemp[256] = {0,};

		//////////////////////////////////////////////////////////////////////////
		//VOICEINFO
		for (a = 0; a < MAX_MONSTER_IDLE_VOICE; a++)
		{
			sprintf(szTemp, "Idle%d", a + 1);
			pInfo->m_Idle[a].Load( this, szTemp, nRow);
		}

		for (a = 0; a < MAX_MONSTER_BATTLE_VOICE; a++)
		{
			sprintf(szTemp, "Battle%d", a + 1);
			pInfo->m_Battle[a].Load( this, szTemp, nRow);
		}

		for (a = 0; a < MAX_MONSTER_SKILL_VOICE; a++)
		{
			sprintf(szTemp, "Skill%d", a + 1);
			pInfo->m_Skill[a].Load( this, szTemp, nRow);
		}

		for (a = 0; a < MAX_MONSTER_DIE_VOICE; a++)
		{
			sprintf(szTemp, "Die%d", a + 1);
			pInfo->m_Die[a].Load( this, szTemp, nRow);
		}

		//////////////////////////////////////////////////////////////////////////
		//_MONSTERSTATEVOICEINFO
		for (a = 0; a < MAX_MONSTER_STATE_VOICE; a++)
		{
			sprintf(szTemp, "Sta%d", a + 1);
			pInfo->m_State[a].Load( this, szTemp, nRow);
		}


		assert( NULL == m_pDataTable->GetData( pInfo->m_w_Code ) );
		BASE_MonsterVoiceInfo * pPreInfo = pInfo;
		m_pDataTable->Add( pInfo, pInfo->m_w_Code );
	}


	return TRUE;
}



void _MONSTERSTATEVOICEINFO::Load(IParser* parser, const char* prefix, int row)
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


#pragma warning ( pop )
