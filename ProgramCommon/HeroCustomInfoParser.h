#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <map>
#include <boost/shared_ptr.hpp>

#define MAX_ARMORCODE	8
#define MAX_WEAPONCODE	2

struct	HERO_VARIATION_INFO
{
	BYTE	classid;
	BYTE	level;
	DWORD	armorcode[MAX_ARMORCODE];
	DWORD	weaponcode[MAX_WEAPONCODE];

	HERO_VARIATION_INFO() {}

	HERO_VARIATION_INFO( HERO_VARIATION_INFO & rhs )
	{
		classid = rhs.classid;
		level	= rhs.level;
		memcpy( armorcode, rhs.armorcode, sizeof(armorcode) );
		memcpy( weaponcode, rhs.weaponcode, sizeof(weaponcode) );
	}

	HERO_VARIATION_INFO & operator = ( HERO_VARIATION_INFO & rhs )
	{
		classid = rhs.classid;
		level	= rhs.level;
		memcpy( armorcode, rhs.armorcode, sizeof(armorcode) );
		memcpy( weaponcode, rhs.weaponcode, sizeof(weaponcode) );
		return *this;
	}
};

typedef	boost::shared_ptr<HERO_VARIATION_INFO>		HERO_VARIATION_INFO_PTR;
typedef std::map<BYTE, HERO_VARIATION_INFO_PTR >	HERO_VARI_INFO_MAP_TYPE;				
typedef	HERO_VARI_INFO_MAP_TYPE::iterator			HERO_VARI_INFO_MAP_ITR_TYPE;

class HeroCustomInfoParser : public util::Singleton<HeroCustomInfoParser>,public IParser
{	
	typedef std::multimap<BYTE, HERO_VARIATION_INFO_PTR >	HERO_VARI_INFO_MUTILMAP_TYPE;
	typedef HERO_VARI_INFO_MUTILMAP_TYPE::iterator			HERO_VARI_INFO_MUTILMAP_ITR_TYPE;

public:
	HeroCustomInfoParser(void);
	~HeroCustomInfoParser(void);

	VOID			Init( char * pszPackFileName );
	VOID			Release();

	VOID			Unload();
	BOOL			Load( char * pszFileName, BOOL bReload = FALSE );

	VOID			Reload();

	BOOL					_Load( BOOL bReload );
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

	void			GetItemList( BYTE Class, HERO_VARI_INFO_MAP_TYPE & OutData );

protected:

private:
	BOOL			_Load( util::SolarFileReader& sr, BOOL bReload );

private:
	char							m_pszFileName[MAX_PATH];
	HERO_VARI_INFO_MUTILMAP_TYPE	m_HeroVariInfoMap;
};
