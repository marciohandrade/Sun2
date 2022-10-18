#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include "HeroVoiceInfoParser.h"

#define MAX_NPC_VOICE_SAMPLE 4


struct BASE_NPCVoiceInfo 
{

	WORD     m_w_NPCCode;
	char     m_s_Desc[64];
	VOICEINFO m_Wait[MAX_NPC_VOICE_SAMPLE];
	VOICEINFO m_Meet[MAX_NPC_VOICE_SAMPLE];
	VOICEINFO m_Leave[MAX_NPC_VOICE_SAMPLE];
	
};


class NPCVoiceInfoParser	: public Singleton<NPCVoiceInfoParser> ,public IParser
{
public:
	NPCVoiceInfoParser();
	~NPCVoiceInfoParser();


	VOID            Init(DWORD dwPoolSize , char *szPackfileName);
	VOID            Init(DWORD dwPoolSize);

	VOID            Release();
	virtual	BOOL    LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_NPCVoiceInfo*  GetNPCVoiceInfo(DWORD dwKey);

private:
	VOID		        Unload();
	util::SolarHashTable<BASE_NPCVoiceInfo*>*   m_pDataTable;
};
