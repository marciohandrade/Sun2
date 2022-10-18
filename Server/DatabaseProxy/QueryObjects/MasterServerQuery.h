#ifndef __MASTER_SERVER_QUERY_H__
#define __MASTER_SERVER_QUERY_H__

//=======================================================================================================================
/// ������ �߰��� �� �ؾ� �� ��!
/// 1. ���� Ŭ������ �����.
/// 2. ���� Ǯ Ŭ�������ٰ� ��ũ��Ʈ�� ���߾� ����Ѵ�.
/// 3. �ش������� ������� ��� -> Stdafx.h�� ���� Ŭ���� ���� ����� ����Ѵ�.
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


	/// ����ϴ� ����� �ۼ��ؾ� �ϴ� �ڵ�
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