#ifndef __SERVER_SESSION_MANAGER_H__
#define __SERVER_SESSION_MANAGER_H__

#pragma once

class ServerSessionEx;

#include <Singleton.h>
#include <MemoryPoolFactory.h>

#include <ServerStruct.h>

// DWORD형 ServerKey를 맵의 키로 사용한다.
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

	// 아직은 container만 사용한다.
	SERVER_SESSION_DELEGATOR&		FindServer( SERVER_SESSION_DELEGATOR pServer );

	VOID							Update();

	// 접속되어 있는 마스터서버들에게 브로드캐스팅
	VOID							SendToMasterServers( BYTE* pMsg, WORD wSize );

	SERVER_SESSION_CONTAINER_DELEGATOR	GetConnectedServerList() { return ConnectedSessions; }

	template<typename _FN, typename _ObjType>
	VOID							ForEach_ConnectedSessions( _FN _funcRaw, const _ObjType& _Left )
	{
		std::for_each( ConnectedSessions->begin(), ConnectedSessions->end(), std::bind1st(mem_fun(_funcRaw), _Left) );
	}
private:
	// <INTERFACEs><PROPERTY>

	// <Type1> 현재 연결된(인증된) 마스터 서버들, 인증여부는 세부 속성으로 처리
	// <Type2> 현재 연결 시도중인 서버들, 인증된 마스터 서버인지 확인하고 이를 위한 보안 처리가 수행될 것이다.
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