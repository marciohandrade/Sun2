#ifndef __MASTER_SERVER_QUERY_H__
#define __MASTER_SERVER_QUERY_H__

//=======================================================================================================================
/// 쿼리를 추가할 때 해야 할 일!
/// 1. 쿼리 클래스를 만든다.
/// 2. 쿼리 풀 클래스에다가 스크립트에 맞추어 등록한다.
/// 3. 해더파일을 만들었을 경우 -> Stdafx.h에 쿼리 클래스 파일 헤더를 등록한다.
//=======================================================================================================================

#pragma once
//#pragma pack(push, 1)
#include <Define.h>
#include "StructInQuery.h"


class ServerInfoQuery : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(ServerInfoQuery)
public:
	enum
	{ 
		SERVERINFO_ROW_NUM = 10,
		SERVERINFO_COL_NUM = 9,
	};
	struct tag_SERVERINFO
	{
		DWORD	dwServerGUID;
		BYTE	byWorldID;
		BYTE	byChannelID;
		BYTE	byServerType;
		BYTE	byServerID;
		char	pszIP[MAX_IPADDRESS_SIZE+1];
		INT		iPort;
		char	pszInnerIP[MAX_IPADDRESS_SIZE+1];
		INT		iInnerPort;
	} pServerInfo[SERVERINFO_ROW_NUM];

	ULONG uServerInfoLength[SERVERINFO_COL_NUM];


	/// 사용하는 사람이 작성해야 하는 코드
	_BEGIN_BINDING_DATA(tag_SERVERINFO, pServerInfo, uServerInfoLength, SERVERINFO_ROW_NUM, SERVERINFO_COL_NUM)
		_BINDING_COLUMN(0, dwServerGUID)
		_BINDING_COLUMN(1, byWorldID)
		_BINDING_COLUMN(2, byChannelID)
		_BINDING_COLUMN(3, byServerType)
		_BINDING_COLUMN(4, byServerID)
		_BINDING_COLUMN_PTR(5, pszIP)
		_BINDING_COLUMN(6, iPort)
		_BINDING_COLUMN_PTR(7, pszInnerIP)
		_BINDING_COLUMN(8, iInnerPort)
	_END_BINDING_DATA()

public:
	VOID	SetCurPage(BYTE byPage) { m_byPage = byPage; }
	BYTE	GetCurPage() { return m_byPage;	}
private:
	BYTE	m_byPage;
};

class RestrictInfoQuery : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(RestrictInfoQuery)
public:
	enum
	{ 
		RESULT_ROW_NUM	= 100,
		RESULT_COL_NUM	= 3,
	};

	struct sQUERY
	{
		DWORD	dwIndex;
		BYTE	byAllow;
		char	szDesc[64];
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN(0, dwIndex)
		_BINDING_COLUMN(1, byAllow)
		_BINDING_COLUMN_PTR(2, szDesc)
	_END_BINDING_DATA		()
};


//#pragma pack(pop)

#endif // __MASTER_SERVER_QUERY_H__