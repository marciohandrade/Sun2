#ifndef __SERVER_SESSION_MANAGER_H__
#define __SERVER_SESSION_MANAGER_H__

#pragma once

class ServerSessionEx;

#include <Singleton.h>
#include <MemoryPoolFactory.h>

#include <ServerStruct.h>

// DWORD�� ServerKey�� ���� Ű�� ����Ѵ�.
typedef Delegator<ServerSession>							SERVER_SESSION_DELEGATOR;

typedef std::pair<DWORD, SERVER_SESSION_DELEGATOR>			SERVER_SESSION_PAIR;
//typedef CMemoryPoolFactory<SERVER_SESSION_PAIR>				SERVER_SESSION_POOL_TOKEN;

typedef STLX_MAP<DWORD, SERVER_SESSION_DELEGATOR>			SERVER_SESSION_MAP;
typedef SERVER_SESSION_MAP::iterator						SERVER_SESSION_MAP_ITER;
typedef Delegator<SERVER_SESSION_MAP>						SERVER_SESSION_MAP_DELEGATOR;

// Key is ServerSession*
typedef std::pair<DWORD, SERVER_SESSION_DELEGATOR>			SERVER_SESSION_CONTAINER_PAIR;
typedef STLX_MAP<DWORD, SERVER_SESSION_DELEGATOR>			SERVER_SESSION_CONTAINER;
typedef SERVER_SESSION_CONTAINER::iterator					SERVER_SESSION_CONTAINER_ITER;
typedef Delegator<SERVER_SESSION_CONTAINER>					SERVER_SESSION_CONTAINER_DELEGATOR;

class ServerSessionManager : public util::Singleton<ServerSessionManager>
{
public:
	ServerSessionManager();
	~ServerSessionManager();
public:

	//GLOBAL_SERVER_KEY		AddServer( ip, port, ... );

	VOID							AddServer( SERVER_SESSION_DELEGATOR pSessionDelegator );
	VOID							RemoveServer( SERVER_SESSION_DELEGATOR pSessionDelegator );

	// ������ container�� ����Ѵ�.
	SERVER_SESSION_DELEGATOR&		FindServer( SERVER_SESSION_DELEGATOR pServer );

	VOID							Update();

	// ���ӵǾ� �ִ� �����ͼ����鿡�� ��ε�ĳ����
	VOID							SendToMasterServers( BYTE* pMsg, WORD wSize );

	SERVER_SESSION_CONTAINER_DELEGATOR	GetConnectedServerList() { return ConnectedSessions; }

	template<typename _FN, typename _ObjType>
	VOID							ForEach_ConnectedSessions( _FN _funcRaw, const _ObjType& _Left )
	{
		std::for_each( ConnectedSessions->begin(), ConnectedSessions->end(), std::bind1st(mem_fun(_funcRaw), _Left) );
	}
private:
	// <INTERFACEs><PROPERTY>

	// <Type1> ���� �����(������) ������ ������, �������δ� ���� �Ӽ����� ó��
	// <Type2> ���� ���� �õ����� ������, ������ ������ �������� Ȯ���ϰ� �̸� ���� ���� ó���� ����� ���̴�.
	SERVER_SESSION_MAP_DELEGATOR		AcceptedSessions;	//MasterServerSession
	SERVER_SESSION_CONTAINER_DELEGATOR	ConnectedSessions;	//MasterServerSession+TempServerSession;
	//
	SERVER_SESSION_DELEGATOR			m_EmptySession;
private:
	VOID							_Initialize();

	// for foreach
	VOID							_UpdateSession( SERVER_SESSION_PAIR node );
	VOID							_DeleteSession( SERVER_SESSION_PAIR node );
};

#endif // __SERVER_SESSION_MANAGER_H__