
#include "stdafx.h"

#include "ScriptLoadManager.h"

#include "SolarAuthInfoLoader.h"
#include "SolarAcceptableMasterServerListLoader.h"
#include "ScriptListLoader.h"
#include "ServerList/ServerInfo.h"


ScriptLoadManager::ScriptLoadManager()
{
	_Init();
}

ScriptLoadManager::~ScriptLoadManager()
{
	SAFE_DELETE( SolarAuthInfoLoaderHandle.REFERENCE() );
	// 아래 두 개는 순서에 영향을 받는다. 왜냐? ScriptListLoader는 각 마스터서버별 가지고 있으며,
	// 그 정보를 토대로 리소스들을 받아들인다.
	SAFE_DELETE( ScriptListLoaderHandle.REFERENCE() );
	SAFE_DELETE( SolarAcceptableMasterServerListLoaderHandle.REFERENCE() );
}

VOID	ScriptLoadManager::_Init()
{
	SolarAuthInfoLoaderHandle.REFERENCE() = new SolarAuthInfoLoader();
	SolarAcceptableMasterServerListLoaderHandle.REFERENCE() = new SolarAcceptableMasterServerListLoader();
	ScriptListLoaderHandle.REFERENCE() = new ScriptListLoader();
}

BOOL	ScriptLoadManager::Initialize(
									  const TCHAR* pszSolarAuthInfo,
									  const TCHAR* pszSolarAcceptableMasterList,
									  const TCHAR* pszScriptList
									  )
{
	if( !( pszSolarAuthInfo && pszSolarAcceptableMasterList && pszScriptList ) )
		return FALSE;

	// 해당 스크립트 로더를 불러 처리하기로 한다.
	// 해당 로더는 다음과 같은 처리를 수행한다.
	// 1: 해당 파일의 암호화 여부
	//   T : 해당 파일을 읽어들여 복호화 한다.
	//   F : 해당 파일을 읽어들인다.
	// 2: 메모리 스트림을 분석해 의미있게 조각낸다.
	// 3: 해당 로더의 인터페이스에 맞게 조직화한다.

	memcpy( m_pszSolarAuthInfo, pszSolarAuthInfo, sizeof(m_pszSolarAuthInfo) );
	m_pszSolarAuthInfo[sizeof(m_pszSolarAuthInfo)-1] = _T('\0');
//배열 옵셋 확인할 것
	memcpy( m_pszSolarAcceptableMasterList, pszSolarAcceptableMasterList, sizeof(m_pszSolarAcceptableMasterList) );
	m_pszSolarAcceptableMasterList[sizeof(m_pszSolarAcceptableMasterList)-1] = _T('\0');

	memcpy( m_pszScriptList, pszScriptList, sizeof(m_pszScriptList) );
	m_pszScriptList[sizeof(m_pszScriptList)-1] = _T('\0');

	//SolarAuthInfoLoader
	SolarAuthInfoLoaderHandle->Load( m_pszSolarAuthInfo );

	return TRUE;
}

BOOL	ScriptLoadManager::LoadSolarAuthInfo( BOOL bReload )
{
	if( bReload || !SolarAuthInfoLoaderHandle() )
	{
		SAFE_DELETE( SolarAuthInfoLoaderHandle.REFERENCE() );
		SolarAuthInfoLoaderHandle.REFERENCE() = new SolarAuthInfoLoader();
	}

	SolarAuthInfoLoaderHandle->Load( m_pszSolarAuthInfo );
	//TTT = SolarAuthInfoLoaderHandle->GetInfo();

	return TRUE;
}

BOOL	ScriptLoadManager::LoadAcceptableMasterServerList( BOOL bReload )
{
	if( bReload || !SolarAcceptableMasterServerListLoaderHandle() )
	{
		SAFE_DELETE( SolarAcceptableMasterServerListLoaderHandle.REFERENCE() );
		SolarAcceptableMasterServerListLoaderHandle.REFERENCE() = new SolarAcceptableMasterServerListLoader();
	}

	SolarAcceptableMasterServerListLoaderHandle->Load( m_pszSolarAcceptableMasterList );

	return TRUE;
}

BOOL	ScriptLoadManager::LoadData( BOOL bReload )
{
	if( bReload || !ScriptListLoaderHandle() )
	{
		SAFE_DELETE( ScriptListLoaderHandle.REFERENCE() );
		ScriptListLoaderHandle.REFERENCE() = new ScriptListLoader();
	}

	ScriptListLoaderHandle->Load( m_pszScriptList );
	ScriptListLoaderHandle->LoadDataAll();

	return TRUE;
}

BOOL	ScriptLoadManager::LoadServerListPerMaster( BOOL bReload )
{
	if( bReload || !SolarAcceptableMasterServerListLoaderHandle() )
	{
		//SAFE_DELETE( ServerListHandle.REFERENCE() );
		//ServerListHandle.REFERENCE() = new ServerListFromFile();
	}

	SolarAcceptableMasterServerListLoaderDelegator& rSERVER_LIST =
		SolarAcceptableMasterServerListLoaderHandle;

	// 서버 목록 부르는 객체 두개를 택일 할 수 있도록 구현할 것.
	ServerListFromFile* pInstance = new ServerListFromFile();
	rSERVER_LIST->ForeachServerList(
							&ServerListFromFile::LoadDataHandler,
							pInstance
							);

	SAFE_DELETE( pInstance );

	return TRUE;
}

// 각 로더로 분산된 것 중 자주 사용되는 것 도출
SCRIPT_LOADER	ScriptLoadManager::GetScriptLoaderByKey( CRYPT_KEY_T cryptKey )
{
	ScriptLoadManager* pLoadManager = ScriptLoadManager::Instance();

	SolarAcceptableMasterServerListLoaderDelegator& rServerListLoader =
		pLoadManager->SolarAcceptableMasterServerListLoaderHandle;

	SCRIPT_LOADER hScriptLoader = rServerListLoader->GetMasterServerDataByKey( cryptKey );

	return hScriptLoader;
}

SCRIPT_LOADER	ScriptLoadManager::GetScriptLoaderByIP( DWORD dwIPAddress )
{
	ScriptLoadManager* pLoadManager = ScriptLoadManager::Instance();

	SolarAcceptableMasterServerListLoaderDelegator& rServerListLoader =
		pLoadManager->SolarAcceptableMasterServerListLoaderHandle;

	SCRIPT_LOADER hScriptLoader = rServerListLoader->GetMasterServerDataByIP( dwIPAddress );

	return hScriptLoader;
}

SCRIPT_DATA_STREAM_DELEGATOR
ScriptLoadManager::GetScriptDataByKey( CRYPT_KEY_T cryptKey, eSCRIPT_CODE scriptCode )
{
	SCRIPT_LOADER& hScriptLoader = GetScriptLoaderByKey( cryptKey );
	if( hScriptLoader == NULL )
		return NULL;

	return hScriptLoader->GetScriptDataStream(scriptCode);
}

SCRIPT_DATA_STREAM_DELEGATOR
ScriptLoadManager::GetScriptDataByIP( DWORD dwIPAddress, eSCRIPT_CODE scriptCode )
{
	SCRIPT_LOADER& hScriptLoader = GetScriptLoaderByIP( dwIPAddress );
	if( hScriptLoader == NULL )
		return NULL;

	return hScriptLoader->GetScriptDataStream(scriptCode);
}


