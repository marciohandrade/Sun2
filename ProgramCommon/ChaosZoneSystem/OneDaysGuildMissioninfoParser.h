#ifndef PROGRAMCOMMON_ONEDAYZGUILDMISSIONINFOPARSER_H_
#define PROGRAMCOMMON_ONEDAYZGUILDMISSIONINFOPARSER_H_

#if _MSC_VER > 1000
#pragma  once
#endif

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

struct OneDaysGuildMissionInfo
{
	BYTE	index;															// �� Ÿ��
	DWORD	map_code;														// �� �ڵ�
	DWORD	play_time;														// �÷��̽ð�(��)
	DWORD	start_wait_time;												// ��� ���ð�(��)
	DWORD	end_wait_time;													// ���� ��� �ð�(��)
	DWORD	start_trigger_id;												// ���� ���� Ʈ���� ID
	DWORD	base_reward_item_code;											// �⺻ ���� �ڵ�

	OneDaysGuildMissionInfo()
	{
		::ZeroMemory(this, sizeof(*this));
	}
};

class OneDaysGuildMissionInfoParser : public util::Singleton<OneDaysGuildMissionInfoParser>, public IParser
{
public:
	typedef STLX_HASH_MAP<DWORD, OneDaysGuildMissionInfo*> OneDaysGuildMissionInfo_Hash;

	OneDaysGuildMissionInfoParser(void);
	~OneDaysGuildMissionInfoParser(void);

public:
	void Init();
	void Init(char* pszPackFileName);
	
	virtual void Release();
	virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	OneDaysGuildMissionInfo_Hash& oneoaysguildmissionInfo_hash() { return OneDaysGuildMissionInfo_Hash_; }
	const OneDaysGuildMissionInfo* FindData(const MAPCODE map_code) const;

private:
	BOOL _Load(BOOL bReload);
	void Unload();

private:

	OneDaysGuildMissionInfo_Hash OneDaysGuildMissionInfo_Hash_;
};


#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#endif //(PROGRAMCOMMON_ONEDAYZGUILDMISSIONINFOPARSER_H_)