#pragma once

//=============================================================================================================================
/*
	�� ��Ƽ�� �Ҽӵ� ��Ƽ������ LinkServer���� ������ ���� ����
	��Ƽ���� ���� �� �ش� LinkServer�� ó������ ������ ����� �� �������� ��Ƽ�� �����Ǿ�� �Ѵ�.
	��Ƽ���� ���� �� �ش� LinkServer�� �Ҽӵ� ��Ƽ���� 0�̶�� �� �������� ��Ƽ�� �ı��Ǿ�� �Ѵ�.
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








