#pragma once

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

struct BASE_StatueInfo
{
	BYTE          index_;
	BOOL          is_create_;
	WzVector      position_;
	WzVector      direction_;
	float         scale_value_;
	std::string   animation_code_;
};

class StatueInfoParser : public Singleton<StatueInfoParser>, public IParser
{
public:
	StatueInfoParser(void);
	~StatueInfoParser(void);

	VOID                           Init(DWORD pool_size);
	VOID                           Init(DWORD pool_size, char* pack_file_name);
	VOID                           Release();

	virtual BOOL                   LoadScript(eSCRIPT_CODE script_code, BOOL bReload);

private:
	VOID                           Unload();

public:
	bool                           get_statue_info_list(MAPCODE map_code, OUT std::list<BASE_StatueInfo*>* statue_info_list);

private:
	std::map<MAPCODE, std::list<BASE_StatueInfo*> > statue_info_map_;
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
