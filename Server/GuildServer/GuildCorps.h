#pragma once

/**********************************************/
/* 길드 군단
/* 길드 군단의 최대 인원은 20명 이다.
/* 길드 군단은 1명의 군단장, 1명의 훈련대장, 5명의 훈련병, 13명의 정예병으로 이루어 질 수 있다.
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
	DWORD						m_dwGuid;			// 군단 Guid
	BYTE						m_byNumber;			// 군단 번호( 0군단(직할), 1군단, 2군단.... )
	GUILD_CORPS_MEMBER_MAP		m_mapMember;
	GUILD_CORPS_MEMBER_STR_MAP	m_mapMemberStr;	
	BOOL						m_bCreateRearCamp;	// 길드 육성캠프 존재여부.

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

	//현재 새로 길드에 가입시 이 함수를 쓴다.( GuildMember 객체 자체를 소멸시킨다. )
	GuildMember*	CreateMember( CHARGUID CharGuid, const TCHAR * tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState, BOOL bLogin );
	VOID			DestroyMember( CHARGUID CharGuid );

	//같은 길드 내에서 군단 변경시 이 함수를 쓴다.( 컨테이너에서 GuildMember 원소만 삭제 한다. )
	BOOL			InsertMember ( GuildMember* pMember );
	VOID			DeleteMember( CHARGUID CharGuid );

	GuildMember*	FindMember( CHARGUID CharGuid );
	GuildMember*	FindMember( const TCHAR* pszName );
	GuildMember*	FindCommander();									//군단장.
	GuildMember*	FindTrainingLeader();								//훈련대장.
	GuildMember*	FindMemberByDuty(const eGUILD_DUTY eDuty);	//직책에 해당하는 멤버 찾기
	BOOL			IsExistMemberDuty( eGUILD_DUTY eDuty );		
	BYTE			GetMemberCountByDuty( eGUILD_DUTY eDuty );			//직책별 멤버들의 수
	BOOL			IsExistRearCamp(){ return m_bCreateRearCamp; }		//육성캐프의 존재 여부.
	VOID			SetRearCamp( BOOL bCamp ){ m_bCreateRearCamp = bCamp; }
};

