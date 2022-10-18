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
    BYTE guild_grade_;//��� Grade
    WORD max_member_count_;//�ش� ������ �ִ� ���� ��
    
    CODETYPE item_code_levelup_;//�ش� ������ ��ũ�� �ϴµ� �ʿ��� ������.
    DWORD need_guild_coin_to_rankup_;//������ �ϴµ� �ҿ�Ǵ� Guild Coin
    GPTYPE reward_gp_by_levelup_;//���� �޼��� ���� �Ǵ� GP
    BYTE max_member_of_duty_[eGUILD_DUTY_MAX];//�� ��å�� �ɹ� �ִ��
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
	BYTE		guild_grade_;						//��巹��
	TCHAR		m_szLevelName[MAX_PATH];			//��巹���� �̸�
	WORD		max_member_count_;					//�ش� ������ �ִ� ���� ��
	DWORD		m_dwHeim;							//�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� ����.
	UPTYPE		m_UP;								//�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� UP.
	GPTYPE		m_GP;				                //�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� GP.

	//version 1.1
	CODETYPE						item_code_levelup_; 				// �ش� ������ ���ھ� �ϴµ� �ʿ��� ������.
	BYTE							m_byCreateCopsCount;				// ����â������
	DWORD							m_dwLevelStringCode;				// ��巹���� ��Ʈ���ڵ�
	GUILD_DUTY_USE_MAP				m_mapGuildDutyUseInfo;				// ��� ��å�� ���� ��� ����. 
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