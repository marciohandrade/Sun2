#pragma once

/**********************************************/
/* ��� ����
/* ��� ������ �ִ� �ο��� 20�� �̴�.
/* ��� ������ 1���� ������, 1���� �Ʒô���, 5���� �Ʒú�, 13���� ���������� �̷�� �� �� �ִ�.
/**********************************************/

#include <map>
#include "Guild\GuildMember.h"

typedef		std::map< CHARGUID, GuildMember* >				GUILD_CORPS_MEMBER_MAP;
typedef		GUILD_CORPS_MEMBER_MAP::iterator				GUILD_CORPS_MEMBER_MAP_ITER;
typedef		std::map< std::string, GuildMember* >			GUILD_CORPS_MEMBER_STR_MAP;
typedef		GUILD_CORPS_MEMBER_STR_MAP::iterator			GUILD_CORPS_MEMBER_STR_MAP_ITER;

class GuildCorps
{
public:
	GuildCorps(void);
	~GuildCorps(void);

private:
	DWORD						m_dwGuid;			// ���� Guid
	BYTE						m_byNumber;			// ���� ��ȣ( 0����(����), 1����, 2����.... )
	GUILD_CORPS_MEMBER_MAP		m_mapMember;
	GUILD_CORPS_MEMBER_STR_MAP	m_mapMemberStr;	
	BOOL						m_bCreateRearCamp;	// ��� ����ķ�� ���翩��.

public:
	VOID			Destroy();
	VOID			SerializeBaseMemberTotalInfo( INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );
	VOID			SerializeMemberTotalInfo( INOUT GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );
	VOID			SerializeServerMemberTotalInfo( INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );

public:
	VOID			SetGuid( DWORD dwGuid ){ m_dwGuid = dwGuid; }
	DWORD			GetGuid(){ return m_dwGuid; }
	VOID			SetNumber( BYTE byNumber ){ m_byNumber = byNumber; }
	BYTE			GetNumber(){ return m_byNumber; }
	BYTE			GetMemberCount(){ return (BYTE)m_mapMember.size(); }

	//���� ���� ��忡 ���Խ� �� �Լ��� ����.( GuildMember ��ü ��ü�� �Ҹ��Ų��. )
	GuildMember*	CreateMember( CHARGUID CharGuid, const TCHAR * tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState, BOOL bLogin );
	VOID			DestroyMember( CHARGUID CharGuid );

	//���� ��� ������ ���� ����� �� �Լ��� ����.( �����̳ʿ��� GuildMember ���Ҹ� ���� �Ѵ�. )
	BOOL			InsertMember ( GuildMember* pMember );
	VOID			DeleteMember( CHARGUID CharGuid );

	GuildMember*	FindMember( CHARGUID CharGuid );
	GuildMember*	FindMember( const TCHAR* pszName );
	GuildMember*	FindCommander();									//������.
	GuildMember*	FindTrainingLeader();								//�Ʒô���.
	GuildMember*	FindMemberByDuty(const eGUILD_DUTY eDuty);	//��å�� �ش��ϴ� ��� ã��
	BOOL			IsExistMemberDuty( eGUILD_DUTY eDuty );		
	BYTE			GetMemberCountByDuty( eGUILD_DUTY eDuty );			//��å�� ������� ��
	BOOL			IsExistRearCamp(){ return m_bCreateRearCamp; }		//����ĳ���� ���� ����.
	VOID			SetRearCamp( BOOL bCamp ){ m_bCreateRearCamp = bCamp; }
};

