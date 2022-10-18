#pragma once

//=============================================================================================================================
/// ScriptLoadManager class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer ���� �о���̴� ��� ������ ������ �����ϴ� Ŭ����
	- ���⼭ �����Ǵ� �͵��� ��ȣȭ �Ǿ� ���� �� �ִ�.
	@history 
	-  
*/
//=============================================================================================================================


#include <Singleton.h>
#include <Information/ScriptLoader.h>

class SolarAuthInfoLoader;
class SolarAcceptableMasterServerListLoader;
class ScriptListLoader;
class IServerInfo;

typedef Delegator<SolarAuthInfoLoader>						SolarAuthInfoLoaderDelegator;
typedef Delegator<SolarAcceptableMasterServerListLoader>	SolarAcceptableMasterServerListLoaderDelegator;
typedef Delegator<ScriptListLoader>							ScriptListLoaderDelegator;
typedef Delegator<IServerInfo>								IServerInfoDelegator;
	
//	ScriptListLoader

class ScriptLoadManager : public util::Singleton<ScriptLoadManager>
{
public:
	ScriptLoadManager();
	~ScriptLoadManager();

public:
	// <INTERFACEs>
	BOOL						Initialize(
										const TCHAR* pszSolarAuthInfo,
										const TCHAR* pszSolarAcceptableMasterList,
										const TCHAR* pszScriptList
										);
	// <INTERFACEs><PROPERTY>
	SolarAuthInfoLoaderDelegator					SolarAuthInfoLoaderHandle;
	SolarAcceptableMasterServerListLoaderDelegator	SolarAcceptableMasterServerListLoaderHandle;
	ScriptListLoaderDelegator						ScriptListLoaderHandle;
	IServerInfoDelegator							ServerListHandle;

	BOOL						LoadSolarAuthInfo( BOOL bReload = FALSE );
	BOOL						LoadAcceptableMasterServerList( BOOL bReload = FALSE );
	BOOL						LoadData( BOOL bReload = FALSE );
	BOOL						LoadServerListPerMaster( BOOL bReload = FALSE );

	// �� �δ��� �л�� �� �� ���� ���Ǵ� �� ����
	static SCRIPT_LOADER		GetScriptLoaderByKey( CRYPT_KEY_T cryptKey );
	static SCRIPT_LOADER		GetScriptLoaderByIP( DWORD dwIPAddress );
	static SCRIPT_DATA_STREAM_DELEGATOR		GetScriptDataByKey( CRYPT_KEY_T cryptKey, eSCRIPT_CODE scriptCode );
	static SCRIPT_DATA_STREAM_DELEGATOR		GetScriptDataByIP( DWORD dwIPAddress, eSCRIPT_CODE scriptCode );

private:
	// <INTERNALs>
	VOID						_Init();

private:
	// <FIELDs>

	// <FIELD::TAIL>
	TCHAR						m_pszSolarAuthInfo[ 128 /*find path size*/ ];
	TCHAR						m_pszSolarAcceptableMasterList[ 128 /*find path size*/ ];
	TCHAR						m_pszScriptList[ 128 /*find path size*/ ];
};

