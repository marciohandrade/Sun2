
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
	// �Ʒ� �� ���� ������ ������ �޴´�. �ֳ�? ScriptListLoader�� �� �����ͼ����� ������ ������,
	// �� ������ ���� ���ҽ����� �޾Ƶ��δ�.
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

	// �ش� ��ũ��Ʈ �δ��� �ҷ� ó���ϱ�� �Ѵ�.
	// �ش� �δ��� ������ ���� ó���� �����Ѵ�.
	// 1: �ش� ������ ��ȣȭ ����
	//   T : �ش� ������ �о�鿩 ��ȣȭ �Ѵ�.
	//   F : �ش� ������ �о���δ�.
	// 2: �޸� ��Ʈ���� �м��� �ǹ��ְ� ��������.
	// 3: �ش� �δ��� �������̽��� �°� ����ȭ�Ѵ�.

	memcpy( m_pszSolarAuthInfo, pszSolarAuthInfo, sizeof(m_pszSolarAuthInfo) );
	m_pszSolarAuthInfo[sizeof(m_pszSolarAuthInfo)-1] = _T('\0');
//�迭 �ɼ� Ȯ���� ��
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

	// ���� ��� �θ��� ��ü �ΰ��� ���� �� �� �ֵ��� ������ ��.
	ServerListFromFile* pInstance = new ServerListFromFile();
	rSERVER_LIST->ForeachServerList(
							&ServerListFromFile::LoadDataHandler,
							pInstance
							);

	SAFE_DELETE( pInstance );

	return TRUE;
}

// �� �δ��� �л�� �� �� ���� ���Ǵ� �� ����
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


