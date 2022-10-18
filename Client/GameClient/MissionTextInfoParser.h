#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#define MAX_MISSION_TEXT_LENGTH

struct MissionTextInfo 
{
     DWORD		mission_text_code;
     TCHAR*		mission_text;
};

class MissionTextInfoParser	: public Singleton<MissionTextInfoParser> ,public IParser
{
public:
	MissionTextInfoParser();
	~MissionTextInfoParser();

	VOID           Init(DWORD pool_size);
	VOID           Init(DWORD pool_size , char *pack_file_name);
	VOID           Release();
	virtual	BOOL			LoadScript(eSCRIPT_CODE script_code, BOOL reload);
	MissionTextInfo* GetMissionTextInfo(DWORD key);

private:
	VOID		   Unload();
	util::SolarHashTable<MissionTextInfo*>*	data_table_;
};
