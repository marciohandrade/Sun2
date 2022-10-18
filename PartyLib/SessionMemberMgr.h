#pragma once

//=============================================================================================================================
/*
	한 파티에 소속된 파티원들이 LinkServer별로 분포된 수를 관리
	파티원이 들어올 때 해당 LinkServer에 처음으로 들어오는 경우라면 그 서버에서 파티는 생성되어야 한다.
	파티원이 떠날 때 해당 LinkServer에 소속된 파티원이 0이라면 그 서버에서 파티는 파괴되어야 한다.
*/
//=============================================================================================================================

#include <map>

class SessionMemberMgr
{
public:
	typedef std::map<DWORD,int>			SESSION_COUNT_MAP;		// <SessionIndex, Count>
	typedef std::map<DWORD,DWORD>		SESSION_MEMBER_MAP;		// <MemberKey, SessionIndex>

	SessionMemberMgr() {}
	~SessionMemberMgr() {}

	VOID		Release();

	BOOL		JoinMember( DWORD MemberKey );
	BOOL		LeaveMember( DWORD MemberKey );

	BOOL		AddMemberToSession( DWORD MemberKey, DWORD ToSessionIndex );
	BOOL		RemoveMemberToSession( DWORD MemberKey, DWORD ToSessionIndex );
	int			GetSessionMemberNum( DWORD SessionIndex );

	int			GetTotSessionMemberNum();
	int			GetTotMemberNum();

	template< class Operator > VOID For_eachSessions( Operator & Opr );

private:
	VOID		IncSessionCount( DWORD SessionIndex );
	VOID		DecSessionCount( DWORD SessionIndex );

private:
	SESSION_COUNT_MAP		m_mapSessions;
	SESSION_MEMBER_MAP		m_mapMembers;					
};

template< class Operator >
VOID SessionMemberMgr::For_eachSessions( Operator & Opr )
{
	SESSION_COUNT_MAP::iterator it, end(m_mapSessions.end());
	for( it = m_mapSessions.begin(); it != end; it++ )
	{
		Opr( it->first );
	}
}








