#ifndef __SERVER_SESSION_MANAGER_H__
#define __SERVER_SESSION_MANAGER_H__


#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

class ServerSessionEx;
class BattleServerSession;

typedef util::SolarHashTable<ServerSessionEx*>				SESSION_LIST;
typedef util::SolarHashTable<ServerSessionEx*>::iterator	SESSION_ITERATOR;

class ServerSessionManager : public util::Singleton<ServerSessionManager>
{
public:
	ServerSessionManager();
	~ServerSessionManager();


	VOID								AddServer( ServerSessionEx* pServer );
	VOID								RemoveServer( ServerSessionEx* pServer );
	ServerSessionEx*					FindServer( DWORD dwSessionIndex );

	//Low Overhead First // ���� �����ص带 ���� ��Ʋ���� �켱����
	DWORD								Incr_LOF_BattleServer();							//< ������ ��Ʋ���� ���� �ٽ� ���� ��Ʋ ���
	VOID								Decr_LOF_BattleServer( DWORD dwSessionIndex );		//< �氨�ҽ�Ű�� ���� ��Ʋ ���
	VOID								Process();

	ServerSessionEx*					GetFieldServer() { return m_pFieldServer;	}

	inline SESSION_ITERATOR				Begin()		{ return m_pSessionHashTable->begin(); }
	inline SESSION_ITERATOR				End()		{ return m_pSessionHashTable->end(); }

	DWORD								GetBattleServerSessionNum();
	SESSION_LIST*						GetSessionList() { return m_pSessionHashTable; }

//////////////////////////////////////////////////////////////////////////
// <FOREACH ROUTINEs>
	template <typename ObjectOperator>
	inline	VOID							Foreach( ObjectOperator& op )
	{
		SESSION_ITERATOR it = m_pSessionHashTable->begin(), end = m_pSessionHashTable->end();
		for( ; it != end ; ++it )
		{
			ServerSessionEx* pSession = *it;
			op( pSession );
		}
	}

	// �ڷᱸ���� �ٲ�� ��������.
	//template<typename _FN, typename _ObjType>
	//inline VOID								Foreach( _FN _funcRaw, const _ObjType& _Left )
	//{
	//	std::for_each( m_UserHashTable.begin(), m_UserHashTable.end(), std::bind1st(mem_fun(_funcRaw), _Left) );
	//}

	VOID								DisconnectAllServer();

private:
	BOOL								update_LOF_BattleServer();
	ServerSessionEx*					GetFieldServerSession();

private:
	ServerSessionEx*					m_pFieldServer;
	BattleServerSession*				m_pLOFBattleServer;
	enum { _MAX_SESSION_BUCKET_SIZE = 10 };
	util::SolarHashTable<ServerSessionEx*> *	m_pSessionHashTable;
};


#endif // __SERVER_SESSION_MANAGER_H__