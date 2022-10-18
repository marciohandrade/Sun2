#include "StdAfx.h"
#include "SessionMemberMgr.h"


VOID SessionMemberMgr::Release()
{
	m_mapSessions.clear();
	m_mapMembers.clear();
}

BOOL SessionMemberMgr::JoinMember( DWORD MemberKey )
{
	SESSION_MEMBER_MAP::iterator it = m_mapMembers.find( MemberKey );
	if( it != m_mapMembers.end() )
		return FALSE;

	m_mapMembers.insert( SESSION_MEMBER_MAP::value_type(MemberKey, 0) );

	return TRUE;
}

BOOL SessionMemberMgr::LeaveMember( DWORD MemberKey )
{
	SESSION_MEMBER_MAP::iterator it = m_mapMembers.find( MemberKey );
	if( it == m_mapMembers.end() )
		return FALSE;

	m_mapMembers.erase( it );

	return TRUE;
}

BOOL SessionMemberMgr::AddMemberToSession( DWORD MemberKey, DWORD ToSessionIndex )
{
	SESSION_MEMBER_MAP::iterator it = m_mapMembers.find( MemberKey );
	if( it == m_mapMembers.end() )
		return FALSE;

	// SessionIndex가 변경될 경우에만 값을 수정한다.
	DWORD FromSessionIndex = it->second;
	if( FromSessionIndex == ToSessionIndex )
		return FALSE;

	it->second = ToSessionIndex;

	IncSessionCount( ToSessionIndex );

	return TRUE;
}

BOOL SessionMemberMgr::RemoveMemberToSession( DWORD MemberKey, DWORD ToSessionIndex )
{
	// 삽입한 적이 없는 유저를 삭제할 수는 없다.
	SESSION_MEMBER_MAP::iterator it = m_mapMembers.find( MemberKey );
	if( it == m_mapMembers.end() )
		return FALSE;

	// SessionIndex가 변경될 경우에만 값을 수정한다.
	DWORD FromSessionIndex = it->second;
	if( FromSessionIndex == ToSessionIndex )
		return FALSE;

	DecSessionCount( FromSessionIndex );

	return TRUE;
}

VOID SessionMemberMgr::IncSessionCount( DWORD SessionIndex )
{
	SESSION_COUNT_MAP::iterator it = m_mapSessions.find( SessionIndex );
	if( it == m_mapSessions.end() )
	{
		m_mapSessions.insert( SESSION_COUNT_MAP::value_type(SessionIndex, 1) );
	}
	else
	{
		(it->second)++;
	}
}

VOID SessionMemberMgr::DecSessionCount( DWORD SessionIndex )
{
	SESSION_COUNT_MAP::iterator it = m_mapSessions.find( SessionIndex );
	if( it == m_mapSessions.end() )
		return;

	(it->second)--;

	if( it->second <= 0 )
		m_mapSessions.erase( it );
}

int SessionMemberMgr::GetSessionMemberNum( DWORD SessionIndex )
{
	SESSION_COUNT_MAP::iterator it = m_mapSessions.find( SessionIndex );
	if( it == m_mapSessions.end() )
		return 0;

	return it->second;
}

int	SessionMemberMgr::GetTotSessionMemberNum()
{
	int TotCount = 0;

	SESSION_COUNT_MAP::iterator it, end(m_mapSessions.end());
	for( it = m_mapSessions.begin(); it != end; it++ )
	{
		TotCount += it->second;
	}
	return TotCount;
}

int	SessionMemberMgr::GetTotMemberNum()
{
	return (int)m_mapMembers.size();
}























