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
        ACCEPT_ALL              = 0,                                        // 0 : ��� �ɼ� ����
        ACCEPT_HUNTING_NORATIO  = 1,                                        // 1 : ��ɽ� ����(���ʽ� ������)
        ACCEPT_HUNTING_RATIO    = 1<<1,                                     // 2 : ��ɽ� ����(���ʽ� ����)
        
        ACCEPT_QUEST            = 1<<2,                                     // 4 : ����Ʈ ����
        ACCEPT_QUEST_NORATIO    = ACCEPT_HUNTING_NORATIO + ACCEPT_QUEST,    // 5 : ����Ʈ + ��ɽ� ����(���ʽ� ������)
        ACCEPT_QUEST_RATIO      = ACCEPT_HUNTING_RATIO + ACCEPT_QUEST,      // 6 : ����Ʈ + ��ɽ� ����(���ʽ� ����)

        ACCEPT_MISSION          = 1<<3,                                     // 8 : �̼� ����
        ACCEPT_MISSION_NORATIO    = ACCEPT_HUNTING_NORATIO + ACCEPT_MISSION,  // 9 : �̼� + ��ɽ� ����(���ʽ� ������)
        ACCEPT_MISSION_RATIO      = ACCEPT_HUNTING_RATIO + ACCEPT_MISSION,    //10 : �̼� + ��ɽ� ����(���ʽ� ����)

        ACCEPT_QUEST_MISSION    = ACCEPT_QUEST + ACCEPT_MISSION,            //12 : ����Ʈ + �̼� ����
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
