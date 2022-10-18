#pragma once

#include "IServerInfo.h"
#include <Include/FileListLoader.h>
#include <Information/ScriptLoader.h>
#include <Information/SolarAcceptableMasterServerListLoader.h>

//
//
//

//
// 파일로부터 마스터별 서버정보를 읽는다.
class ServerListFromFile : public IServerInfo
{
public:
	ServerListFromFile();
	~ServerListFromFile();

public:
	virtual VOID	Init( PSERVERLIST_ARGS arguments );
	virtual VOID	Release();

	virtual BOOL	Load( const CHAR* pszFileName, DWORD dwMasterIP, ServerTreeINFO*& OUT rPServerTreeInfo, BOOL bReload );

	VOID			LoadDataHandler( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR rINFO );

private:
	static INT		ParseInfoSection( PVOID pPassingPtr, AttrTag tagName );
private:
	BOOL				m_bInited;
	CHAR				m_TagName[32];
};

struct MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK;
struct sSERVERLISTFROMDB_ARGS : public sSERVERLIST_ARGS
{
	sSERVERLISTFROMDB_ARGS() { m_MAGIC = this->FROMDB; }

	MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK*	m_SVR_LIST;
};
//
// DB로부터 마스터별 서버정보를 읽는다.
class ServerListFromDB : public IServerInfo
{
public:
	ServerListFromDB();
	~ServerListFromDB();

public:
	virtual VOID	Init( PSERVERLIST_ARGS arguments );
	virtual VOID	Release();

	virtual BOOL	Load( const CHAR* pszFileName, DWORD dwMasterIP, ServerTreeINFO*& OUT rPServerTreeInfo, BOOL bReload );

private:
	BOOL									m_bInited;
	MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK*	m_pMsgACK;
};


