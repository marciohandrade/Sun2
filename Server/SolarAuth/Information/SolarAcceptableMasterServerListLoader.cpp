
#include "stdafx.h"

//
#include "FileListLoader.h"
#include "FileDecoder.h"
//

#include "SolarAcceptableMasterServerListLoader.h"
#include "ServerList/ServerInfo.h"


SolarAcceptableMasterServerListLoader::SolarAcceptableMasterServerListLoader()
{
	_Init();
}

SolarAcceptableMasterServerListLoader::~SolarAcceptableMasterServerListLoader()
{
	Release();
}

VOID	SolarAcceptableMasterServerListLoader::_Init()
{
	m_bInited = FALSE;
	m_MasterServerIPs.clear();
	m_MasterServerKeys.clear();
	m_MasterServerList.clear();
	m_EmptyServerInfo.REFERENCE() = NULL;
}

VOID	SolarAcceptableMasterServerListLoader::Release()
{
	m_MasterServerIPs.clear();
	m_MasterServerKeys.clear();

	for_each(
		m_MasterServerList.begin(),
		m_MasterServerList.end(),
		std::bind1st(std::mem_fun(&SolarAcceptableMasterServerListLoader::_ReleaseNode), this)
		);

	m_MasterServerList.clear();

	m_bInited = FALSE;
	return;
}

ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR&
SolarAcceptableMasterServerListLoader::GetMasterServerByIP( DWORD dwIPAddress )
{
	ACCEPTABLE_MASTER_SERVER_HASH_IT it = m_MasterServerIPs.find( dwIPAddress );
	if( it == m_MasterServerIPs.end() )
		return m_EmptyServerInfo;

	return (it->second);
}

ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR&
SolarAcceptableMasterServerListLoader::GetMasterServerByKey( CRYPT_KEY_T cryptKey )
{
	ACCEPTABLE_MASTER_SERVER_HASH_IT it = m_MasterServerKeys.find( cryptKey );
	if( it == m_MasterServerKeys.end() )
		return m_EmptyServerInfo;

	return (it->second);
}

SCRIPT_LOADER
SolarAcceptableMasterServerListLoader::GetMasterServerDataByIP( DWORD dwIPAddress )
{
	ACCEPTABLE_MASTER_SERVER_HASH_IT it = m_MasterServerIPs.find( dwIPAddress );
	if( it == m_MasterServerIPs.end() )
		return SCRIPT_LOADER(NULL);

	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rInfo = it->second;

	SCRIPT_LOADER loader = rInfo->GetScriptLoader();
	return loader;
}

SCRIPT_LOADER
SolarAcceptableMasterServerListLoader::GetMasterServerDataByKey( CRYPT_KEY_T cryptKey )
{
	ACCEPTABLE_MASTER_SERVER_HASH_IT it = m_MasterServerKeys.find( cryptKey );
	if( it == m_MasterServerKeys.end() )
		return SCRIPT_LOADER(NULL);

	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rInfo = it->second;

	SCRIPT_LOADER loader = rInfo->GetScriptLoader();
	return loader;
}

VOID	SolarAcceptableMasterServerListLoader::_ReleaseNode( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR node )
{
	//ServerTreeINFO* pTREE = node->m_pServerTreeInfo;
	//if( pTREE )
	//{
	//	DWORD& rdwUpper;
	//	PSERVER_INFO_GENERIC pINFOs = pTREE->GetServerArrayList( rdwUpper );
	//	PSERVER_INFO_GENERIC pINFO = pINFOs;

	//	for( DWORD dwIdx=0 ; dwIdx<rdwUpper ; ++dwIdx, ++pINFO )
	//	{

	//	}
	//}
	SAFE_DELETE( node->m_pServerTreeInfo );
	SAFE_DELETE( node->m_ScriptLoaderHandle.REFERENCE() );
	SAFE_DELETE( node.REFERENCE() );
}

BOOL	SolarAcceptableMasterServerListLoader::Load( const CHAR* pszFileName, BOOL bReload )
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

	if( bLoad )
	{
		FileListLoader loader;
		loader.SectionCallbackAlloc( &ParseInfoSection, this );
		//loader.SectionDataCallbackAlloc( &ParseInfoSectionData, this );

		if( !loader.Load( pszFileName, FALSE ) )
		{
			MessageOut( eFULL_LOG, "SolarAuthServer::Acceptable Master Server List : [Can't Open File : %s]", pszFileName );
			return FALSE;
		}

		MessageOut( eFULL_LOG, "Acceptable Master Server List : [READING]" );

		BOOL bSuccess = TRUE;
		do
		{
			ACCEPTABLE_MASTER_SERVER_INFO* pINFO = new ACCEPTABLE_MASTER_SERVER_INFO;
			memset( pINFO, 0, sizeof(ACCEPTABLE_MASTER_SERVER_INFO) );

			BOOL bCompleteSentence = FALSE;
			if( bSuccess = loader.ReadValue( pINFO->m_dwMasterServerIndex ) )
				if( bCompleteSentence = loader.ReadValue( pINFO->m_byNationCode ) )
					if( bCompleteSentence = loader.ReadValue( pINFO->m_byRegionCode ) )
						if( bCompleteSentence = loader.ReadValue( pINFO->m_byServerMode ) )
							if( bCompleteSentence = loader.ReadValue( pINFO->m_byPaddingType ) )
							{
								CHAR strIPAddress[32];
								// Public IP Address
								if( bCompleteSentence = loader.ReadString( strIPAddress, sizeof(strIPAddress) ) )
								{
									strIPAddress[sizeof(strIPAddress)-1] = '\0';
									pINFO->ConvertIPAddress( TRUE, strIPAddress );

									// Server IP Address
									if( bCompleteSentence = loader.ReadString( strIPAddress, sizeof(strIPAddress) ) )
									{
										strIPAddress[sizeof(strIPAddress)-1] = '\0';
										pINFO->ConvertIPAddress( FALSE, strIPAddress );

										if( bCompleteSentence = loader.ReadString( pINFO->m_strDataPath, sizeof(pINFO->m_strDataPath) ) )
										{
											pINFO->m_strDataPath[sizeof(pINFO->m_strDataPath)-1] = '\0';
										}
									}
								}
							}

			if( bCompleteSentence )
			{
				pINFO->ConvertKeys();
				CRYPT_KEY_T dwCryptKey = pINFO->GetCryptKey();
				ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR deleKey_INFO = GetMasterServerByKey( dwCryptKey );
				DWORD dwIPAddress = pINFO->GetIPAddress();
				ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR deleIP_INFO = GetMasterServerByIP( dwIPAddress );
				if( deleKey_INFO != NULL || deleIP_INFO != NULL )
				{
					ASSERT( !"키가 중복되고 있습니다\n" );
					MessageOut( eCRITICAL_LOG, "인증된 마스터 서버 목록 중복키 (%u) 또는 IP (%s)가 존재합니다.",
						dwCryptKey, /*pINFO->m_strIPAddress*/inet_ntoa( *((in_addr*)&(pINFO->m_dwNetworkIP_Public)) ) );
					SAFE_DELETE( pINFO );
					break;
				}

				// 각 데이터 검증 할까 말까?
				// 요구사항이 있으면 하기로 하자...

				ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR delegatorINFO = ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR( pINFO );

				m_MasterServerList.insert(
									m_MasterServerList.end(),
									delegatorINFO
									);
				m_MasterServerKeys.insert(
									m_MasterServerKeys.end(),
									ACCEPTABLE_MASTER_SERVER_PAIR( dwCryptKey, delegatorINFO )
									);
				m_MasterServerIPs.insert(
									m_MasterServerIPs.end(),
									ACCEPTABLE_MASTER_SERVER_PAIR( dwIPAddress, delegatorINFO )
									);
			}
			else
			{
				SAFE_DELETE( pINFO );
			}
		} while(bSuccess);

		MessageOut( eFULL_LOG, "Acceptable Master Server List : [READED]" );

		loader.Close();
		m_bInited = TRUE;
	}

	return TRUE;
}

INT		SolarAcceptableMasterServerListLoader::ParseInfoSection( PVOID pPassingPtr, AttrTag tagName )
{
	SolarAcceptableMasterServerListLoader* pInfoLoader = (SolarAcceptableMasterServerListLoader*)pPassingPtr;
	if( !pInfoLoader )
		return FALSE;

	if( tagName.IsMatch( "[MASTER_SERVER_LIST]" ) )
	{
		//printf( "[MASTER_SERVER_LIST]!\n" );
		return FileListLoader::SECTION_DATA;
	}
	return FileListLoader::SECTION_ERROR;
}

//BOOL	SolarAcceptableMasterServerListLoader::ParseInfoSectionData( PVOID pPassingPtr, const CHAR* pszSectionName, const DWORD dwBufSize, const DWORD dwValue )
//{
//	SolarAcceptableMasterServerListLoader* pInfoLoader = (SolarAcceptableMasterServerListLoader*)pPassingPtr;
//	if( !pInfoLoader )
//		return FALSE;
//
//	if( strncmp( pszSectionName, "CONFIGURATION_RELOAD_TIME_INTERVAL", dwBufSize ) == 0 )
//	{
//		printf( "Section value %dwValue!\n", dwValue );
//		pInfoLoader->m_dwReloadInterval = dwValue;
//		return TRUE;
//	}
//	return FALSE;
//}

