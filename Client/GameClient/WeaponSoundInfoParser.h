#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define MAX_WEAPON_SOUND_VARIATION 5

struct BASE_WeaponSoundInfo 
{

     BYTE     m_bWeaponType;
     DWORD     m_dWeaponSound[MAX_WEAPON_SOUND_VARIATION];
     
};

enum eWEAPONSOUNDKIND
{
	eWEAPONSOUNDKIND_TWOHANDED      = 0,
	eWEAPONSOUNDKIND_ONEHANDSWORD   = 1,
	eWEAPONSOUNDKIND_SPEAR          = 2,
	eWEAPONSOUNDKIND_DAGGER			= 3,
	eWEAPONSOUNDKIND_WHIP           = 4,
	eWEAPONSOUNDKIND_ARROW          = 5,
	eWEAPONSOUNDKIND_ETHERBULLET    = 6,
	eWEAPONSOUNDKIND_STAFF          = 7,
	eWEAPONSOUNDKIND_MAGICBULLET    = 8,
	eWEAPONSOUNDKIND_UNDEFINED      = 9,
	eWEAPONSOUNDKIND_MONSTERPHYSICAL= 10,
	eWEAPONSOUNDKIND_MONSTERMAGICAL = 11,
    eWEAPONSOUNDKIND_GUNTLET        = 12, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONSOUNDKIND_POLEARM        = 13, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONSOUNDKIND_HELLROIDWEAPON_1       = 14, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eWEAPONSOUNDKIND_ARCBLADE       = 15, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	eWEAPONSOUNDKIND_MAX 
};


class WeaponSoundInfoParser	: public Singleton<WeaponSoundInfoParser>, public IParser
{
public:
    WeaponSoundInfoParser();
    ~WeaponSoundInfoParser();


    VOID                    Init(DWORD dwPoolSize);
    VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
    VOID                    Release();
    virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_WeaponSoundInfo*   GetWeaponSoundInfo(DWORD dwKey);

private:
    VOID		            Unload();
    util::SolarHashTable<BASE_WeaponSoundInfo*>*    m_pDataTable;
};
