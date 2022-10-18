#pragma once

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <map>
#include "Struct.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
class GuildRankInfo
{
public:
    GuildRankInfo(void);
    ~GuildRankInfo(void){};

public:
    BYTE guild_grade_;//길드 Grade
    WORD max_member_count_;//해당 레벨별 최대 길드원 수
    
    CODETYPE item_code_levelup_;//해당 레벨로 랭크업 하는데 필요한 아이템.
    DWORD need_guild_coin_to_rankup_;//레벨업 하는데 소요되는 Guild Coin
    GPTYPE reward_gp_by_levelup_;//레벨 달성후 보상 되는 GP
    BYTE max_member_of_duty_[eGUILD_DUTY_MAX];//각 직책별 맴버 최대수
};

typedef std::map<BYTE, GuildRankInfo*> GuildRankInfoMap;

class GuildRankInfoParser : public util::Singleton<GuildRankInfoParser>, public IParser
{
public:
    GuildRankInfoParser(void);
    ~GuildRankInfoParser(void);

public:
    void Release();

    BYTE GetGuildMaxGrade() const;

    BYTE GetMaxMemberByGrade(
        const BYTE guild_grade, 
        const eGUILD_DUTY guild_duty
        ) const;
    
    const GuildRankInfo* FindGuildRankInfo(
        BYTE guild_grade
        ) const;

    BYTE GetMatchedRankInfoByGuildCoin(INT guild_coin);

    virtual	BOOL LoadScript(
        eSCRIPT_CODE script_code, 
        BOOL is_reload
        );


private:
    GuildRankInfoMap guild_rank_info_map_;
    BYTE max_guild_grade_;
};

inline BYTE
GuildRankInfoParser::GetGuildMaxGrade() const
{
    return max_guild_grade_;
}

#else//_NA_003923_20120130_GUILD_RENEWAL
typedef std::map< eGUILD_DUTY, GUILD_DUTY_USE_INFO* >		GUILD_DUTY_USE_MAP;
typedef GUILD_DUTY_USE_MAP::iterator						GUILD_DUTY_USE_MAP_ITER;


class GuildRankInfo
{
public:
	GuildRankInfo(void){};
	~GuildRankInfo(void){};

public:
	VOID						Release();
	GUILD_DUTY_USE_INFO*		Find( eGUILD_DUTY Duty );

public:
	//version 1.0
	BYTE		guild_grade_;						//길드레벨
	TCHAR		m_szLevelName[MAX_PATH];			//길드레벨별 이름
	WORD		max_member_count_;					//해당 레벨별 최대 길드원 수
	DWORD		m_dwHeim;							//해당 레벨로 랭크업 하는데 소요되는 하임.
	UPTYPE		m_UP;								//해당 레벨로 랭크업 하는데 소요되는 UP.
	GPTYPE		m_GP;				                //해당 레벨로 랭크업 하는데 소요되는 GP.

	//version 1.1
	CODETYPE						item_code_levelup_; 				// 해당 레벨로 랭코업 하는데 필요한 아이템.
	BYTE							m_byCreateCopsCount;				// 군단창설개수
	DWORD							m_dwLevelStringCode;				// 길드레벨별 스트링코드
	GUILD_DUTY_USE_MAP				m_mapGuildDutyUseInfo;				// 길드 직책별 권한 사용 정보. 
};

typedef std::map< BYTE, GuildRankInfo* >		GUILD_RANK_INFO_MAP;
typedef GUILD_RANK_INFO_MAP::iterator					GUILD_RANK_INFO_MAP_ITER;	

class GuildRankInfoParser : public util::Singleton<GuildRankInfoParser>, public IParser
{
public:
	GuildRankInfoParser(void);
	~GuildRankInfoParser(void);

public:
	VOID					Release();
	GuildRankInfo*			FindGuildRankInfo( BYTE Grade );
	GUILD_DUTY_USE_INFO *	GetGuildDutyUseInfo(BYTE Grade,eGUILD_DUTY eDuty);
	BYTE					GetGuildMaxGrade(){ return m_byMaxGuildLevel; }

	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
private:
	GUILD_RANK_INFO_MAP		m_mapGuildRankInfo;
	BYTE					m_byMaxGuildLevel;
};
#endif//_NA_003923_20120130_GUILD_RENEWAL