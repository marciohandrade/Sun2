#pragma once

// 이 파서는 반드시 GuildRankInfo를 먼저 로드하고 읽어야 한다.
// 이 파서는 내부적으로 GuildRankInfo의 길드 레벨을 참조한다.

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <map>
#include "Struct.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL 
class GuildRightInfo
{
public:
    GuildRightInfo();
    ~GuildRightInfo();

public:
    //해당 직책이 사용가능한 권한인지 체크
    bool CanUseToThisRightByDuty(
        const eGUILD_DUTY guild_duty
        ) const;

    //해당 길드 레벨이 사용 가능한 권한인지 체크
    bool CanUseToThisRightByGrade(
        const BYTE guild_grade
        ) const;

public:
    eGUILD_RIGHTS guild_right_;//길드의 권한(Key value)
    TCHAR guild_right_name_[MAX_SKILLNAME_LENGTH];//권한이름,스킬이름 사이즈와 동일
    BYTE use_guild_grade_;//사용가능한 길드 레벨
    DWORD guild_right_string_code_;//권한스트링코드
    bool guild_right_of_duty_[eGUILD_DUTY_MAX];//직책별 권한
};

typedef std::map<eGUILD_RIGHTS, GuildRightInfo*>GuildRightInfoMap;

class GuildRightInfoParser : public util::Singleton<GuildRightInfoParser>, public IParser
{
public:
    GuildRightInfoParser(){};
    ~GuildRightInfoParser(){};

public:

    virtual	BOOL LoadScript(
        eSCRIPT_CODE script_code,
        BOOL is_reload
        );

    virtual	void Release();

public:
    const GuildRightInfo* Find(
        const eGUILD_RIGHTS guild_right
        ) const;

    bool CheckGradeForRight( 
        const eGUILD_RIGHTS guild_right, 
        const BYTE guild_grade 
        ) const ;

    bool CheckDutyForRight(
        const eGUILD_RIGHTS guild_right, 
        const eGUILD_DUTY guild_duty 
        ) const;

    GuildRightInfoMap& GetGuildRightInfoMap() { return guild_right_info_map_; }

private:
    GuildRightInfoMap guild_right_info_map_;
};
#else//_NA_003923_20120130_GUILD_RENEWAL

typedef	std::map< BYTE, GUILD_GRADE_RIGHT_USE* >		GUILD_GRADE_RIGHT_USE_MAP;
typedef	GUILD_GRADE_RIGHT_USE_MAP::iterator						GUILD_GRADE_RIGHT_USE_MAP_ITER;

typedef	std::map< eGUILD_DUTY, GUILD_DUTY_RIGHT_USE* >			GUILD_DUTY_RIGHT_USE_MAP;
typedef	GUILD_DUTY_RIGHT_USE_MAP::iterator						GUILD_DUTY_RIGHT_USE_MAP_ITER;


class GuildRightInfo
{
public:
	GuildRightInfo(void){}
	~GuildRightInfo(void){}

public:
	VOID					Release();
	GUILD_GRADE_RIGHT_USE*	FindGuildGradeRight( BYTE Grade );
	GUILD_DUTY_RIGHT_USE*	FindGuildDutyRight( eGUILD_DUTY Duty );

public:
	eGUILD_RIGHTS					m_GuildRight;								//길드의 권한
	TCHAR							m_szRightName[MAX_SKILLNAME_LENGTH];		//권한이름인데.. 스킬이름 사이즈로 하자.
	GUILD_GRADE_RIGHT_USE_MAP		m_mapGuildGradeRight;						//길드등급별 권한
	GUILD_DUTY_RIGHT_USE_MAP		m_mapGuildDutyRight;						//길드직책별 권한
	BOOL							m_bRightDelegation;							//권한위임이 가능한 권한인가?
	DWORD							m_dwStringCode;								//권한스트링코드
};



typedef std::map< eGUILD_RIGHTS, GuildRightInfo* >		GUILD_RIGHT_INFO_MAP;
typedef GUILD_RIGHT_INFO_MAP::iterator					GUILD_RIGHT_INFO_MAP_ITER;

class GuildRightInfoParser : public util::Singleton<GuildRightInfoParser>, public IParser
{
public:
	GuildRightInfoParser(void);
	~GuildRightInfoParser(void);

public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	virtual	VOID			Release();

public:
	GuildRightInfo*			Find( eGUILD_RIGHTS	Right );
	BOOL					CheckDutyForRight( eGUILD_RIGHTS Right, eGUILD_DUTY Duty );
	BOOL					CheckGradeForRight( eGUILD_RIGHTS Right, BYTE Grade );
    BOOL					CheckGradeDelegateRight(  eGUILD_RIGHTS Right, BYTE Grade );
    VOID					GetDelegateRight( OUT eGUILD_RIGHTS* pGuildRight, OUT BYTE& byCount );
    VOID					GetGradeDelegateRight( BYTE Grade, OUT GuildRightInfo** ppGuildRightInfo, OUT BYTE& byCount );

    GUILD_RIGHT_INFO_MAP *	GetGuildRightInfoMap() {return &m_mapGuildRightInfo;}

private:
	GUILD_RIGHT_INFO_MAP	m_mapGuildRightInfo;

};
#endif//_NA_003923_20120130_GUILD_RENEWAL
