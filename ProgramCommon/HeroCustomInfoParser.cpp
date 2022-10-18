#include "StdAfx.h"
#include ".\HeroCustomInfoParser.h"
#include <Const.h>

HeroCustomInfoParser::HeroCustomInfoParser(void)
{
}

HeroCustomInfoParser::~HeroCustomInfoParser(void)
{
}

VOID HeroCustomInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID HeroCustomInfoParser::Release()
{
}

VOID HeroCustomInfoParser::Unload()
{

}

VOID HeroCustomInfoParser::Reload()
{

}

//#pragma warning( push )
//#pragma warning( disable:4244 )

BOOL HeroCustomInfoParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{

		HERO_VARIATION_INFO_PTR	pInfo( new HERO_VARIATION_INFO );

		BYTE classid = GetDataBYTE( "ClassID", nRow );
		BYTE level	 = GetDataBYTE( "ItemLv", nRow );

		pInfo->classid = classid;
		pInfo->level   = level; 

		DWORD armor[MAX_ARMORCODE];
		DWORD weapon[MAX_WEAPONCODE];
        static const char* FieldArmorName[MAX_ARMORCODE] = {
            "ItemCode1", "ItemCode2", "ItemCode3", "ItemCode4", 
            "ItemCode5", "ItemCode6", "ItemCode7", "ItemCode8"
        };
        static const char* FieldWeaponName[MAX_WEAPONCODE] = { "ItemCode9", "ItemCode10" };
		
		for( int i = 0; i < MAX_ARMORCODE; i++ )
		{
			armor[i] = GetDataDWORD( FieldArmorName[i], nRow );
			pInfo->armorcode[i] = armor[i];
		}

		for( int i = 0; i < MAX_WEAPONCODE; i++ )
		{
			weapon[i] = armor[i] = GetDataDWORD( FieldWeaponName[i], nRow );
			pInfo->weaponcode[i] = weapon[i];
		}
	
		m_HeroVariInfoMap.insert( HERO_VARI_INFO_MAP_TYPE::value_type( pInfo->classid, pInfo ) );
	}

	return TRUE;
}

BOOL HeroCustomInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch (ScriptCode)
	{
	    case SCRIPT_HEROVARIATION_INFO:
		    return _Load(bReload);
	}


	return FALSE;
}

//#pragma warning( pop )

void HeroCustomInfoParser::GetItemList( BYTE Class, HERO_VARI_INFO_MAP_TYPE & OutData )
{
	std::pair<HERO_VARI_INFO_MUTILMAP_ITR_TYPE, HERO_VARI_INFO_MUTILMAP_ITR_TYPE> pair = m_HeroVariInfoMap.equal_range( Class );

	HERO_VARI_INFO_MUTILMAP_ITR_TYPE itr = pair.first;
	HERO_VARI_INFO_MUTILMAP_ITR_TYPE end = pair.second;

	for( ; itr != end; ++itr )
	{
		HERO_VARIATION_INFO_PTR	info( new HERO_VARIATION_INFO( *(itr->second) ) );
		
		OutData.insert( HERO_VARI_INFO_MAP_TYPE::value_type( info->level, info ) );
	}

}




















