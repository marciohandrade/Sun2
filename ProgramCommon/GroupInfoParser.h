#pragma once

#include <Singleton.h>
//#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

// implemented by __KR_001355_20090608_GROUPINGSYSTEM_PARSER

struct tag_GroupInfo
{
	BYTE	flExpApplyScope;
	BYTE	flHeimApplyScope;
	DWORD	dwExpRewardRatio;
	DWORD	dwHeimRewardRatio;
};

class GroupInfoParser : public util::Singleton<GroupInfoParser>, public IParser
{
public:
    enum{
        ACCEPT_ALL              = 0,                                        // 0 : 모든 옵션 적용
        ACCEPT_HUNTING_NORATIO  = 1,                                        // 1 : 사냥시 적용(보너스 미포함)
        ACCEPT_HUNTING_RATIO    = 1<<1,                                     // 2 : 사냥시 적용(보너스 포함)
        
        ACCEPT_QUEST            = 1<<2,                                     // 4 : 퀘스트 적용
        ACCEPT_QUEST_NORATIO    = ACCEPT_HUNTING_NORATIO + ACCEPT_QUEST,    // 5 : 퀘스트 + 사냥시 적용(보너스 미포함)
        ACCEPT_QUEST_RATIO      = ACCEPT_HUNTING_RATIO + ACCEPT_QUEST,      // 6 : 퀘스트 + 사냥시 적용(보너스 포함)

        ACCEPT_MISSION          = 1<<3,                                     // 8 : 미션 적용
        ACCEPT_MISSION_NORATIO    = ACCEPT_HUNTING_NORATIO + ACCEPT_MISSION,  // 9 : 미션 + 사냥시 적용(보너스 미포함)
        ACCEPT_MISSION_RATIO      = ACCEPT_HUNTING_RATIO + ACCEPT_MISSION,    //10 : 미션 + 사냥시 적용(보너스 포함)

        ACCEPT_QUEST_MISSION    = ACCEPT_QUEST + ACCEPT_MISSION,            //12 : 퀘스트 + 미션 적용
    };
	GroupInfoParser(void);
	virtual ~GroupInfoParser(void);

	VOID					Init( char * pszPackFileName );
	VOID					Release(){ Unload(); };

	VOID					Reload();
	VOID					Unload();

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );	
	inline tag_GroupInfo*	GetGroupInfo(){ return m_pGroupInfo; }
private:
	BOOL _Load( BOOL bReload );

	tag_GroupInfo* 			m_pGroupInfo;
	char					m_pszFileName[MAX_PATH];
};
