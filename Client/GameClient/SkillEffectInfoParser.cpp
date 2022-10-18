//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "SkillEffectInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------
/**
*/
SkillEffectInfoParser::SkillEffectInfoParser() 
: m_pDataTable ( NULL )
{
}


//----------------------------------------------------------------------------
/**
*/
SkillEffectInfoParser::~SkillEffectInfoParser()
{
   assert( m_pDataTable == NULL);
}


//----------------------------------------------------------------------------
/**
*/
VOID SkillEffectInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}


//----------------------------------------------------------------------------
/**
*/
VOID SkillEffectInfoParser::Init(DWORD dwPoolSize)
{
   assert( m_pDataTable == NULL );
   m_pDataTable = new util::SolarHashTable<SkillEffectInfo*>;
   m_pDataTable->Initialize( dwPoolSize );
}


//----------------------------------------------------------------------------
/**
*/
VOID SkillEffectInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


//----------------------------------------------------------------------------
/**
*/
VOID SkillEffectInfoParser::Unload()
{
	SkillEffectInfo * pInfo = NULL;

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
SkillEffectInfo* SkillEffectInfoParser::GetSkillEffectInfo(SLOTCODE SkillClassCode)
{
	return m_pDataTable->GetData( SkillClassCode );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------
/**
*/
BOOL SkillEffectInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch(ScriptCode)
	{
	case SCRIPT_SKILL_EFFECT_INFO:
		{
			return _Load(bReload);
		}
		break;
	}
	return FALSE;
}

BOOL SkillEffectInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
		SkillEffectInfo * pInfo = new SkillEffectInfo;
		pInfo->m_SkillClassCode		= GetDataNumber("SkillClassCode", nRow);
		strncpy(pInfo->m_szSkillName, GetDataString("SkillName", nRow), MAX_SKILLNAME_LENGTH);
		pInfo->m_byWeaponType		= GetDataBYTE("Weapon_Type",nRow);
		strncpy( pInfo->m_EffectInfo[eEffect_Type_Start].m_wzidEffectCode.code, GetDataString("Start_EffectCode",nRow), 4 );
		pInfo->m_EffectInfo[eEffect_Type_Start].m_bChase= GetDataNumber("Start_bChase" , nRow);
		pInfo->m_EffectInfo[eEffect_Type_Start].m_dwLifeTime= GetDataNumber("Start_LifeTime",nRow);
		pInfo->m_EffectInfo[eEffect_Type_Start].m_dwPositionType = (eSKILLEFFECT_POS)((int)GetDataNumber("Start_PosType", nRow));
		pInfo->m_EffectInfo[eEffect_Type_Start].m_wBoneIndex= GetDataNumber("Start_BoneIndex", nRow);
		pInfo->m_EffectInfo[eEffect_Type_Start].m_bRot=  GetDataNumber("Start_bRot", nRow);

		strncpy( pInfo->m_EffectInfo[eEffect_Type_End].m_wzidEffectCode.code, GetDataString("End_EffectCode", nRow), 4 );
		pInfo->m_EffectInfo[eEffect_Type_End].m_bChase= GetDataNumber("End_bChase" , nRow);
		pInfo->m_EffectInfo[eEffect_Type_End].m_dwLifeTime= GetDataNumber("End_LifeTime",nRow);
		pInfo->m_EffectInfo[eEffect_Type_End].m_dwPositionType = (eSKILLEFFECT_POS)((int)GetDataNumber("End_PosType", nRow));
		pInfo->m_EffectInfo[eEffect_Type_End].m_wBoneIndex= GetDataNumber("End_BoneIndex", nRow);
		pInfo->m_EffectInfo[eEffect_Type_End].m_bRot=  GetDataNumber("End_bRot", nRow);

		strncpy( pInfo->m_EffectInfo[eEffect_Type_Buff].m_wzidEffectCode.code, GetDataString("Buff_EffectCode", nRow), 4 );
		pInfo->m_EffectInfo[eEffect_Type_Buff].m_bChase= GetDataNumber("Buff_bChase" , nRow);
		pInfo->m_EffectInfo[eEffect_Type_Buff].m_dwLifeTime= GetDataNumber("Buff_LifeTime",nRow);
		pInfo->m_EffectInfo[eEffect_Type_Buff].m_dwPositionType = (eSKILLEFFECT_POS)((int)GetDataNumber("Buff_PosType", nRow));
		pInfo->m_EffectInfo[eEffect_Type_Buff].m_wBoneIndex= GetDataNumber("Buff_BoneIndex", nRow);
		pInfo->m_EffectInfo[eEffect_Type_Buff].m_bRot=  GetDataNumber("Buff_bRot", nRow);

		strncpy( pInfo->m_EffectInfo[eEffect_Type_Damage].m_wzidEffectCode.code, GetDataString("Damage_EffectCode", nRow), 4 );
		pInfo->m_EffectInfo[eEffect_Type_Damage].m_bChase= GetDataNumber("Damage_bChase" , nRow);
		pInfo->m_EffectInfo[eEffect_Type_Damage].m_dwLifeTime= GetDataNumber("Damage_LifeTime",nRow);
		pInfo->m_EffectInfo[eEffect_Type_Damage].m_dwPositionType = (eSKILLEFFECT_POS)((int)GetDataNumber("Damage_PosType", nRow));
		pInfo->m_EffectInfo[eEffect_Type_Damage].m_wBoneIndex= GetDataNumber("Damage_BoneIndex", nRow);
		pInfo->m_EffectInfo[eEffect_Type_Damage].m_bRot=  GetDataNumber("Damage_bRot", nRow);

		strncpy( pInfo->m_wzidEffectCode1.code, GetDataString("ThrowStartEffectCode",nRow), 4 );
		strncpy( pInfo->m_wzidEffectCode2.code, GetDataString("ThrowEndEffectCode",nRow), 4 );		
		m_pDataTable->Add( pInfo, pInfo->m_SkillClassCode );
	}	
	return TRUE;
}



#pragma warning ( pop )
