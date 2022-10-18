#pragma once

#include <Sessions/ServerSession.h>
//
typedef ServerSession	TypeDefinedServerSession;
#include <Include/ServerTree.h>
//
#include <Include/BitOperation.h>
#include <PacketStruct_SM.h>

//-----------------------------------------------------------------
// Bit Operation
typedef BitOperation<MAX_SERVER>	SERVER_TYPE_FIELDS;
//-----------------------------------------------------------------

//
//
typedef struct sSERVERLIST_ARGS
{
	enum eMAGIC { NONE = 0xABABFCFC, FROMFILE = 0xABABCECE, FROMDB = 0xABABDEDE, };
	sSERVERLIST_ARGS() : m_MAGIC(NONE) {}

	eMAGIC		m_MAGIC;
} *PSERVERLIST_ARGS;

typedef ServerTree<sSERVER_INFO_GENERIC>		ServerTreeINFO;

// <INTERFACE>
class IServerInfo
{
public:
	virtual VOID	Init( PSERVERLIST_ARGS arguments ) = 0;
	virtual VOID	Release() = 0;
	virtual BOOL	Load( const CHAR* pszFileName, DWORD dwMasterIP, ServerTreeINFO*& OUT rPServerTreeInfo, BOOL bReload ) = 0;
};
