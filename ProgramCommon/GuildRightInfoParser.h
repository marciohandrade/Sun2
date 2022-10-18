#pragma once

// �� �ļ��� �ݵ�� GuildRankInfo�� ���� �ε��ϰ� �о�� �Ѵ�.
// �� �ļ��� ���������� GuildRankInfo�� ��� ������ �����Ѵ�.

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
    //�ش� ��å�� ��밡���� �������� üũ
    bool CanUseToThisRightByDuty(
        const eGUILD_DUTY guild_duty
        ) const;

    //�ش� ��� ������ ��� ������ �������� üũ
    bool CanUseToThisRightByGrade(
        const BYTE guild_grade
        ) const;

public:
    eGUILD_RIGHTS guild_right_;//����� ����(Key value)
    TCHAR guild_right_name_[MAX_SKILLNAME_LENGTH];//�����̸�,��ų�̸� ������� ����
    BYTE use_guild_grade_;//��밡���� ��� ����
    DWORD guild_right_string_code_;//���ѽ�Ʈ���ڵ�
    bool guild_right_of_duty_[eGUILD_DUTY_MAX];//��å�� ����
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
	eGUILD_RIGHTS					m_GuildRight;								//����� ����
	TCHAR							m_szRightName[MAX_SKILLNAME_LENGTH];		//�����̸��ε�.. ��ų�̸� ������� ����.
	GUILD_GRADE_RIGHT_USE_MAP		m_mapGuildGradeRight;						//����޺� ����
	GUILD_DUTY_RIGHT_USE_MAP		m_mapGuildDutyRight;						//�����å�� ����
	BOOL							m_bRightDelegation;							//���������� ������ �����ΰ�?
	DWORD							m_dwStringCode;								//���ѽ�Ʈ���ڵ�
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
