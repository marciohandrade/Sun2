#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include "HeroVoiceInfoParser.h"

const int MONSTER_VOICE_LENGTH = 64;

const int MAX_MONSTER_IDLE_VOICE = 4;
const int MAX_MONSTER_BATTLE_VOICE = 4;
const int MAX_MONSTER_SKILL_VOICE = 4;
const int MAX_MONSTER_DIE_VOICE = 4;
const int MAX_MONSTER_STATE_VOICE = 2;

typedef struct _MONSTERSTATEVOICEINFO
{
	DWORD dwVoiceID;
	BYTE  byCondition;
	BYTE  byVoiceType;
	DWORD dwTextID;
	BYTE  byTextType;

	void Load(IParser* parser, const char* prefix, int row);

} MONSTERSTATEVOICEINFO;


struct BASE_MonsterVoiceInfo 
{

     WORD     m_w_Code;
     char     m_s_Desc[MONSTER_VOICE_LENGTH];

	 VOICEINFO m_Idle[MAX_MONSTER_IDLE_VOICE];
	 VOICEINFO m_Battle[MAX_MONSTER_BATTLE_VOICE];
	 VOICEINFO m_Skill[MAX_MONSTER_SKILL_VOICE];
	 VOICEINFO m_Die[MAX_MONSTER_DIE_VOICE];
	 MONSTERSTATEVOICEINFO m_State[MAX_MONSTER_STATE_VOICE];

     

};


class MonsterVoiceInfoParser	: public Singleton<MonsterVoiceInfoParser> ,public IParser
{
public:
    MonsterVoiceInfoParser();
    ~MonsterVoiceInfoParser();


    VOID                    Init(DWORD dwPoolSize , char *szPackfileName);
    VOID                    Init(DWORD dwPoolSize);

    VOID                    Release();

    virtual BOOL			LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_MonsterVoiceInfo*  GetMonsterVoiceInfo(DWORD dwKey);

private:
    VOID		   Unload();
    util::SolarHashTable<BASE_MonsterVoiceInfo*>*  m_pDataTable;
};
