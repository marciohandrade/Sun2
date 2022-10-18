#pragma once
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

struct BASE_GuildMarkInfo
{
	DWORD	dwIndex;
	BYTE	byMainCategory;
	BYTE	bySubCategory;
	int		iRow;
	int		iColumn;
};

enum
{
	eTYPE_BACKGROUND =1,
	eTYPE_PATTERN =2,
};

enum
{
	eSUB_TYPE_BACKGROUND_CIRCUL =1,
	eSUB_TYPE_BACKGROUND_SHIELD =2,
	eSUB_TYPE_BACKGROUND_ETC	=3,

	eSUB_TYPE_PATTERN_FIGURE	=4,
	eSUB_TYPE_PATTERN_SYMBOL	=5,
	eSUB_TYPE_PATTERN_ETC		=6,

    //battle_ground_team_mark = 999  (��帶ũ ���� ui�� �������� �ʵ��� �ӽ� �ε���)
};

typedef util::SolarHashTable<BASE_GuildMarkInfo *>	 HASH_TABLE_GUILD_MARK;
typedef HASH_TABLE_GUILD_MARK::iterator	HASH_TABLE_GUILD_MARK_ITR;

class GuildMarkInfoParser	: public Singleton<GuildMarkInfoParser>, public IParser
{
public:
	GuildMarkInfoParser();
	~GuildMarkInfoParser();


	VOID                    Init(DWORD dwPoolSize);
	VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID                    Release();
	virtual BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_GuildMarkInfo*     GetGuildMarkInfo(DWORD dwKey);
	HASH_TABLE_GUILD_MARK*  GetHashTable(){return m_pDataTable;}

private:
	VOID		            Unload();
	HASH_TABLE_GUILD_MARK*  m_pDataTable;
};
