#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

typedef struct _VOICEINFO
{
	DWORD dwVoiceID;
	BYTE  byVoiceType;
	DWORD dwTextID;
	BYTE  byTextType;


	void Load(util::SolarFileReader &sr);
    void Load(IParser* parser, const char* prefix, int row);

} VOICEINFO;


struct BASE_HeroVoiceInfo 
{
     WORD m_w_CharacterCode; // 클래스코드
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
     WORD m_w_CharacterGender; // 성별코드
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
     char m_s_Desc[64];
	 VOICEINFO m_LimitUse;
	 VOICEINFO m_NoTarget1;
	 VOICEINFO m_NoTarget2;
	 VOICEINFO m_LackMon;
	 VOICEINFO m_NOREPItem;	 
	 VOICEINFO m_LackSP;
	 VOICEINFO m_LackMP;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
     VOICEINFO lack_fp_;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	 VOICEINFO m_LackPoint;
	 VOICEINFO m_FarTarget1;
	 VOICEINFO m_FarTarget2;
     
};

struct HeroVoiceCode
{
    union
    {
        struct
        {
            WORD wCharacterCode; // 클래스코드
            WORD wCharacterGender; // 성별코드
        };
        DWORD dwVoiceCode;
    };

    HeroVoiceCode()
        :dwVoiceCode(0)
    {}
};


class HeroVoiceInfoParser	: public Singleton<HeroVoiceInfoParser> ,public IParser
{
public:
    HeroVoiceInfoParser();
    ~HeroVoiceInfoParser();


    VOID                Init(DWORD dwPoolSize , char *szPackfileName);
    VOID                Init(DWORD dwPoolSize);

    VOID                Release();

    virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_HeroVoiceInfo* GetHeroVoiceInfo(DWORD dwKey);

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    static DWORD GetVoiceInfoKeyCode(eCHAR_TYPE character_type, GenderType::Value gender_type);
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT    

private:
    VOID		        Unload();
    util::SolarHashTable<BASE_HeroVoiceInfo*>*  m_pDataTable;
};
