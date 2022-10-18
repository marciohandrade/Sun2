#ifndef __SOLARAUTH_SERVER_QUERY_H__
#define __SOLARAUTH_SERVER_QUERY_H__

//=======================================================================================================================
/// 쿼리를 추가할 때 해야 할 일!
/// 1. 쿼리 클래스를 만든다.
/// 2. 쿼리 풀 클래스에다가 스크립트에 맞추어 등록한다.
/// 3. 해더파일을 만들었을 경우 -> Stdafx.h에 쿼리 클래스 파일 헤더를 등록한다.
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


	/// 사용하는 사람이 작성해야 하는 코드
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
