#ifndef __SOLARAUTH_SERVER_QUERY_H__
#define __SOLARAUTH_SERVER_QUERY_H__

//=======================================================================================================================
/// ������ �߰��� �� �ؾ� �� ��!
/// 1. ���� Ŭ������ �����.
/// 2. ���� Ǯ Ŭ�������ٰ� ��ũ��Ʈ�� ���߾� ����Ѵ�.
/// 3. �ش������� ������� ��� -> Stdafx.h�� ���� Ŭ���� ���� ����� ����Ѵ�.
//=======================================================================================================================

#pragma once
#include <Define.h>
#include "StructInQuery.h"


// SolarAuth
class GlobalServerInfoQuery : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(GlobalServerInfoQuery)
public:
	enum
	{ 
		SERVERINFO_ROW_NUM = 10,
		SERVERINFO_COL_NUM = 10,
	};
	struct sGLOBALSERVERINFO
	{
		char	pszMasterIP[15];
		BYTE	byWorldID;
		BYTE	byChannelID;
		BYTE	byServerType;
		BYTE	byServerID;
		char	pszIP[15];
		char	pszNatIP[15];
		INT		iPort;
		char	pszInnerIP[15];
		INT		iInnerPort;
	} pServerInfo[SERVERINFO_ROW_NUM];

	ULONG uServerInfoLength[SERVERINFO_COL_NUM];


	/// ����ϴ� ����� �ۼ��ؾ� �ϴ� �ڵ�
	_BEGIN_BINDING_DATA(sGLOBALSERVERINFO, pServerInfo, uServerInfoLength, SERVERINFO_ROW_NUM, SERVERINFO_COL_NUM)
		_BINDING_COLUMN_PTR(0, pszMasterIP)
		_BINDING_COLUMN(1, byWorldID)
		_BINDING_COLUMN(2, byChannelID)
		_BINDING_COLUMN(3, byServerType)
		_BINDING_COLUMN(4, byServerID)
		_BINDING_COLUMN_PTR(5, pszIP)
		_BINDING_COLUMN_PTR(6, pszNatIP)
		_BINDING_COLUMN(7, iPort)
		_BINDING_COLUMN_PTR(8, pszInnerIP)
		_BINDING_COLUMN(9, iInnerPort)
		_END_BINDING_DATA()

public:
	VOID	SetCurPage(BYTE byPage) { m_byPage = byPage; }
	BYTE	GetCurPage() { return m_byPage;	}
private:
	BYTE	m_byPage;
};

#endif //__SOLARAUTH_SERVER_QUERY_H__
