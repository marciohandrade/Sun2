#include "stdafx.h"
#include "ServerInfo.h"
#include <Information/SolarAcceptableMasterServerListLoader.h>


//
// ���Ϸκ��� �����ͺ� ���������� �д´�.
ServerListFromFile::ServerListFromFile()
{
}

ServerListFromFile::~ServerListFromFile()
{
}


VOID	ServerListFromFile::Init( PSERVERLIST_ARGS arguments )
{
	m_bInited = FALSE;
}

VOID	ServerListFromFile::Release()
{
	m_bInited = FALSE;
}

VOID	ServerListFromFile::LoadDataHandler( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR rINFO )
{
	DWORD dwMasterIP = rINFO->m_dwNetworkIP_Public;

	CHAR pszRelativePath[64] = ".\\SolarAuthServer.EncryptedInfo.ini";

	ServerTreeINFO* rpINFO = NULL;
	ServerListFromFile* pLoader = new ServerListFromFile();
	
	pLoader->Init( NULL );
	pLoader->Load( pszRelativePath, dwMasterIP, rpINFO, TRUE );

	rINFO->m_pServerTreeInfo = rpINFO;
	
	SAFE_DELETE(pLoader);
}

BOOL	ServerListFromFile::Load( const CHAR* pszFileName, DWORD dwMasterIP, ServerTreeINFO*& OUT rPServerTreeInfo, BOOL bReload )
{
	BOOL bLoad = FALSE;

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

	ServerTreeINFO* pTREE = NULL;

	if( bLoad )
	{
		CHAR* pIP = IPOperation::ConvertDWORD2StrIP(dwMasterIP);
		_snprintf( m_TagName, sizeof(m_TagName), "[%s]", pIP );

		FileListLoader loader;
		loader.SectionCallbackAlloc( &ParseInfoSection, this );

		if( !loader.Load( pszFileName, FALSE ) )
		{
			MessageOut( eFULL_LOG, "SolarAuthServer::Server List In Master : [Can't Open File : %s]", pszFileName );
			return FALSE;
		}

		pTREE = new ServerTreeINFO();
		MessageOut( eCRITICAL_LOG, "Server List In Master : [READING]" );

		CHAR pszServerName[64];
		CHAR pszTempString[16];
		BYTE skFields[4];
		BOOL bSuccess = TRUE;
		DWORD dwTableIdx;
		do
		{
			PSERVER_INFO_GENERIC pINFO = new sSERVER_INFO_GENERIC;
			memset( pINFO, 0, sizeof(sSERVER_INFO_GENERIC) );

			pINFO->m_dwMasterIP = dwMasterIP;
			BOOL bCompleteSentence = FALSE;
			if( bSuccess = loader.ReadValue( dwTableIdx ) )
			if( bCompleteSentence = loader.ReadString( pszServerName, sizeof(pszServerName) ) )
			if( bCompleteSentence = loader.ReadValue( skFields[0] ) )
			if( bCompleteSentence = loader.ReadValue( skFields[1] ) )
			if( bCompleteSentence = loader.ReadValue( skFields[2] ) )
			if( bCompleteSentence = loader.ReadValue( skFields[3] ) )
			{
				pINFO->m_ServerKey.Set( skFields[0], skFields[1], skFields[2], skFields[3] );

				if( bCompleteSentence = loader.ReadString( pszTempString, sizeof(pszTempString) ) )
				{
					pINFO->m_dwPublicIP = IPOperation::ConvertStrIP2DWORD( pszTempString );
					if( bCompleteSentence = loader.ReadValue( pINFO->m_wPublicPort ) )
					if( bCompleteSentence = loader.ReadString( pszTempString, sizeof(pszTempString) ) )
					{
						pINFO->m_dwNAT_IP = IPOperation::ConvertStrIP2DWORD( pszTempString );
						if( bCompleteSentence = loader.ReadString( pszTempString, sizeof(pszTempString) ) )
						{
							pINFO->m_dwInnerIP = IPOperation::ConvertStrIP2DWORD( pszTempString );
							if( bCompleteSentence = loader.ReadValue( pINFO->m_wInnerPort ) )
							{
							}
						}
					}
				}
			}

			if( bCompleteSentence )
			{
				SERVER_KEY& rKey = pINFO->m_ServerKey;

				if( pTREE->FindServer( rKey ) )
				{
					ASSERT( !"Ű�� �ߺ��ǰ� �ֽ��ϴ�\n" );
					MessageOut( eCRITICAL_LOG, "���� ��� �߿� �ߺ��� ����Ű�� �����մϴ�. {%s} [%02u][%02u][%02u][%02u]",
						pszServerName, rKey.GetWorldID(), rKey.GetChannelID(), rKey.GetServerType(), rKey.GetServerID() );
					SAFE_DELETE( pINFO );
					continue;
				}
				pINFO->m_ServerType = (eSERVER_TYPE)rKey.GetServerType();
				pTREE->AddServer( rKey, pINFO );
			}
			else
			{
				if( bSuccess )
					MessageOut( eCRITICAL_LOG, "���� ���� ��Ͽ� ������ �ֽ��ϴ�. {%s}", pszServerName );
			}
			// �̰��� ServerTree���� �����ϴ� ������. �����ؾ� �Ѵ�.
			SAFE_DELETE( pINFO );
		} while( bSuccess );

		MessageOut( eCRITICAL_LOG, "Server List In Master : [READED]" );

		loader.Close();
		m_bInited = TRUE;
	}

	rPServerTreeInfo = pTREE;

	return TRUE;
}


INT		ServerListFromFile::ParseInfoSection( PVOID pPassingPtr, AttrTag tagName )
{
	ServerListFromFile* pInfoLoader = (ServerListFromFile*)pPassingPtr;
	if( !pInfoLoader )
		return FALSE;

	if( tagName.IsMatch( pInfoLoader->m_TagName ) )
	{
		//printf( "[MASTER_SERVER_LIST]!\n" );
		return FileListLoader::SECTION_DATA;
	}
	return FileListLoader::SECTION_ERROR;
}


