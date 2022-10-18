#ifndef __ACHIEVEMENT_ACHIEVEMENTPARSER_H
#define __ACHIEVEMENT_ACHIEVEMENTPARSER_H
#pragma once

#include "AchievementDefine.h"

#include <Singleton.h>
#include <ScriptCode.IParser.h>

struct Achievement_ParserInfo_Reward
{
    WORD    reward_type_;		//보사 타입
    WORD    reward_code_;		//보상 값
};

struct Achievement_ParserInfo_Object
{
    WORD    object_code_;		//과제 코드
    WORD    object_number_;		//과제 달성 수량
};

struct Achievement_ParserInfo
{
    WORD    index_;
    WORD    location_code_;
    DWORD   object_string_code_;
    DWORD   tooltip_string_code_;
    DWORD   icon_code_;    
    BYTE    object_type_;
#ifdef _NA_001990_ACSYSTEM_ADD
    DWORD   class_type_;
    WORD    parent_ac_code_[MAX_ACHIEVEMENT_PARENT_CODE];
#endif //_NA_001990_ACSYSTEM_ADD
    int rewards_count_;
    Achievement_ParserInfo_Reward  rewards_[MAX_ACHIEVEMENT_REWARD];
    Achievement_ParserInfo_Object  obejcts_[MAX_ACHIEVEMENT_OBJECT];

#ifdef _NA_006541_RENEWER_ACDIALOG
    int     upper_group_index_;
    DWORD   upper_group_string_;

    int     lower_group_index_;
    DWORD   lower_group_string_;
#endif
};

typedef util::SolarHashTable<Achievement_ParserInfo *>::iterator ACHIEVEMENTINFO_ITERATOR;

class AchievementParser : public util::Singleton<AchievementParser>, public IParser
{
public:
    AchievementParser(void);
    ~AchievementParser(void);

    VOID            Init(DWORD dwPoolSize);
    VOID            Init(DWORD dwPoolSize , char *pszPackFileName);
    VOID            Release();
    BOOL			_Load( BOOL bReload );
    BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL reload );

    Achievement_ParserInfo * GetAchievement_ParserInfo( WORD key );

    inline ACHIEVEMENTINFO_ITERATOR			Begin()		{ return script_data_table_->begin(); }
    inline ACHIEVEMENTINFO_ITERATOR			End()		{ return script_data_table_->end(); }    

private:
    Achievement_ParserInfo * GetRowData(WORD index, BOOL reload);
private:
    util::SolarHashTable<Achievement_ParserInfo *>*	script_data_table_;
    
};

#endif //__ACHIEVEMENT_ACHIEVEMENTPARSER_H
