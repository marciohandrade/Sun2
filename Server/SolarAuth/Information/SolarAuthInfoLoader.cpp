
#include "stdafx.h"

//
#include "FileListLoader.h"
#include "FileDecoder.h"
//

#include "SolarAuthInfoLoader.h"

// fnptr 객체용으로 사용하기로 하자... 일단 공유 변수
static DWORD	s_dwReloadInterval = 0;

SolarAuthInfoLoader::SolarAuthInfoLoader()
{
	_Init();
}

SolarAuthInfoLoader::~SolarAuthInfoLoader()
{
}

VOID	SolarAuthInfoLoader::_Init()
{
	m_bInited = FALSE;
	memset( &m_ServerInfo, 0, sizeof(m_ServerInfo) );
	m_ServerInfoDelegator = &m_ServerInfo;
	m_dwCurrentTick = 0;
}

VOID	SolarAuthInfoLoader::Release()
{
}

BOOL	SolarAuthInfoLoader::Load( const CHAR* pszFileName, BOOL bReload )
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
		loader.SectionDataCallbackAlloc( &ParseInfoSectionData, this );

		if( !loader.Load( pszFileName, FALSE ) )
		{
			MessageOut( eFULL_LOG, "SolarAuthServer Info : [Can't Open File : %s]", pszFileName );
			return FALSE;
		}

		MessageOut( eFULL_LOG, "SolarAuthServer Information : [READING]" );

		BOOL bCompleteSentence = FALSE;
		if( bCompleteSentence = loader.ReadValue( m_ServerInfo.m_dwMasterServerIndex ) )
			if( bCompleteSentence = loader.ReadValue( m_ServerInfo.m_byNationCode ) )
				if( bCompleteSentence = loader.ReadValue( m_ServerInfo.m_byRegionCode ) )
					if( bCompleteSentence = loader.ReadValue( m_ServerInfo.m_byServerMode ) )
						if( bCompleteSentence = loader.ReadValue( m_ServerInfo.m_byPaddingType ) )
							if( bCompleteSentence = loader.ReadString( m_ServerInfo.m_strIPAddress, sizeof(m_ServerInfo.m_strIPAddress) ) )
							{
								m_ServerInfo.m_strIPAddress[sizeof(m_ServerInfo.m_strIPAddress)-1] = '\0';
								if( bCompleteSentence = loader.ReadString( m_ServerInfo.m_strDataPath, sizeof(m_ServerInfo.m_strDataPath) ) )
								{
									m_ServerInfo.m_strDataPath[sizeof(m_ServerInfo.m_strDataPath)-1] = '\0';
								}
							}

		if( !bCompleteSentence )
		{
			MessageOut( eFULL_LOG, "Invalid Info : SolarAuthServer Global Server Key Information" );
			//loader.Close() //이것 필요 없다넹...
			return FALSE;
		}

		MessageOut( eFULL_LOG, "SolarAuthServer Information : [READED]" );

		loader.Close();
		m_bInited = TRUE;
	}

	return FALSE;
}

INT		SolarAuthInfoLoader::ParseInfoSection( PVOID pPassingPtr, AttrTag tagName )
{
	SolarAuthInfoLoader* pInfoLoader = (SolarAuthInfoLoader*)pPassingPtr;
	if( !pInfoLoader )
		return FALSE;

	if( tagName.IsMatch( "[ATTRIBUTE]" ) )
	{
		//printf( "[ATTRIBUTE]!\n" );
		return FileListLoader::SECTION_COLUMNS;
	}
	else if( tagName.IsMatch( "[SOLAR_AUTH_SERVER_GLOBAL_KEY_INFO]" ) )
	{
		return FileListLoader::SECTION_DATA;
	}
	return FileListLoader::SECTION_ERROR;
}

BOOL	SolarAuthInfoLoader::ParseInfoSectionData( PVOID pPassingPtr, AttrValueName tagValueName, AttrValue tagValue )
{
	SolarAuthInfoLoader* pInfoLoader = (SolarAuthInfoLoader*)pPassingPtr;
	if( !pInfoLoader )
		return FALSE;

	if( tagValueName.IsMatch( "CONFIGURATION_RELOAD_TIME_INTERVAL" ) )
	{
		//printf( "Section value %dwValue!\n", dwValue );
		DWORD dwValue;
		tagValue.GetValue( dwValue );
		pInfoLoader->m_ServerInfo.m_dwReloadInterval = dwValue;
		return TRUE;
	}
	return FALSE;
}

