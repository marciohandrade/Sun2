#include "stdafx.h"
#include "ServerInfo.h"

#include <PacketStruct_SX.h>
#include <Information/SolarAcceptableMasterServerListLoader.h>
#include <Information/ScriptLoadManager.h>

//
// 파일로부터 마스터별 서버정보를 읽는다.
ServerListFromDB::ServerListFromDB()
{
}

ServerListFromDB::~ServerListFromDB()
{
}


VOID	ServerListFromDB::Init( PSERVERLIST_ARGS arguments )
{
	sSERVERLISTFROMDB_ARGS* pARG = (sSERVERLISTFROMDB_ARGS*)arguments;
	if( pARG->FROMDB == pARG->m_MAGIC )
	{
		m_pMsgACK = pARG->m_SVR_LIST ? pARG->m_SVR_LIST : NULL;
		m_bInited = FALSE;
	}
}

VOID	ServerListFromDB::Release()
{
}

BOOL	ServerListFromDB::Load( const CHAR* pszFileName, DWORD dwMasterIP, ServerTreeINFO*& OUT rPServerTreeInfo, BOOL bReload )
{
	BOOL bLoad = FALSE;

	if( !m_pMsgACK ) return FALSE;

	if( m_bInited )
	{
		if( bReload )
		{
			//Unload
			bLoad = TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		bLoad = TRUE;
	}

	if( bLoad )
	{
		DWORD dwCounts = m_pMsgACK->m_dwNumberOfServerInfo;
		GLOBAL_SERVER_INFO_STRUCT* pINFO_STRUCT = m_pMsgACK->m_ServerInfo;

		for( DWORD dwInc=0 ; dwInc<dwCounts ; ++dwInc, ++pINFO_STRUCT )
		{
			dwMasterIP = IPOperation::ConvertStrIP2DWORD( pINFO_STRUCT->szMasterIP );

			SolarAcceptableMasterServerListLoaderDelegator& rHandle =
				ScriptLoadManager::Instance()->SolarAcceptableMasterServerListLoaderHandle;
			ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rMASTER = rHandle->GetMasterServerByIP( dwMasterIP );

			if( rMASTER == NULL )
			{
				MessageOut( eCRITICAL_LOG, "인증되지 않은 마스터 서버가 서버 정보에 있습니다. MasterIP {%s}", pINFO_STRUCT->szMasterIP );
				continue;
			}

			ServerTreeINFO* pTREE = rMASTER->GetServerTree();
			if( !pTREE )
			{
				rMASTER->m_pServerTreeInfo = new ServerTreeINFO;
				pTREE = rMASTER->GetServerTree();
			}

			SERVER_KEY& rKey = pINFO_STRUCT->skServerKey;

			if( pTREE->FindServer( rKey ) )
			{
				ASSERT( !"키가 중복되고 있습니다\n" );
				MessageOut( eCRITICAL_LOG, "서버 목록 중에 중복된 서버키가 존재합니다. MasterIP {%s} {%s} [%02u][%02u][%02u][%02u]",
					pINFO_STRUCT->szMasterIP, GetServerType2String((eSERVER_TYPE)rKey.GetServerType()), rKey.GetWorldID(), rKey.GetChannelID(), rKey.GetServerType(), rKey.GetServerID() );
				continue;
			}

			sSERVER_INFO_GENERIC pSERVER_INFO[1];
			memset( pSERVER_INFO, 0, sizeof(sSERVER_INFO_GENERIC) );
			pSERVER_INFO->m_dwMasterIP = dwMasterIP;
			pSERVER_INFO->m_ServerKey = rKey;
			pSERVER_INFO->m_ServerType = (eSERVER_TYPE)rKey.GetServerType();
			pSERVER_INFO->m_dwPublicIP = IPOperation::ConvertStrIP2DWORD( pINFO_STRUCT->szPublicIP );
			pSERVER_INFO->m_dwNAT_IP = IPOperation::ConvertStrIP2DWORD( pINFO_STRUCT->szNatIP );
			pSERVER_INFO->m_dwInnerIP = IPOperation::ConvertStrIP2DWORD( pINFO_STRUCT->szInnerIP );
			pSERVER_INFO->m_wPublicPort = pINFO_STRUCT->wPublicPort;
			pSERVER_INFO->m_wInnerPort = pINFO_STRUCT->wInnerPort;

			pTREE->AddServer( rKey, pSERVER_INFO );
		}
	}

	rPServerTreeInfo = NULL;

	return TRUE;
}
