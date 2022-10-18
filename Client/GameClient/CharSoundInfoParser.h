#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define SOUND_CHARNAME_LENGTH 128

#define MAX_SOUND_WEAPON_TYPE  12
#define MAX_SOUND_VARIATION    5

struct SOUND_INFO_CODE
{
    union
    {
        struct
        {
            WORD wCharacterCode;
            BYTE bWeaponType;
            BYTE bGenderType;
        };
        DWORD dwSoundCode;
    };

    SOUND_INFO_CODE()
        :dwSoundCode(0)
    {}
};

struct CHAR_SOUND_INFO
{
    BYTE bWeaponType;
    BYTE bGenderType;
    DWORD dwArmorSound[MAX_SOUND_VARIATION];
    DWORD dwMoanSound[MAX_SOUND_VARIATION];
    DWORD dwSMoanSound[MAX_SOUND_VARIATION];

    CHAR_SOUND_INFO()
    {
        ZeroMemory(this, sizeof(*this));
    }
};

struct BASE_CharSoundInfo 
{
    SOUND_INFO_CODE SoundInfoCode;
    char szCharacterName[SOUND_CHARNAME_LENGTH];
    CHAR_SOUND_INFO SoundInfo;

    BASE_CharSoundInfo()
    {
        ZeroMemory(&(szCharacterName[0]), sizeof(szCharacterName));
    }
};


class CharSoundInfoParser	: public Singleton<CharSoundInfoParser>, public IParser
{
public:
   CharSoundInfoParser();
   ~CharSoundInfoParser();

    VOID                    Init(DWORD dwPoolSize);
    VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
    VOID                    Release();
    virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

    BASE_CharSoundInfo*     GetCharSoundInfo(DWORD dwKey);

private:
    VOID		            Unload();
	BOOL					_Load( BOOL bReload );

	util::SolarHashTable<BASE_CharSoundInfo *>		*m_pDataTable;
};
