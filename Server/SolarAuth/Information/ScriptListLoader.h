#pragma once

//=============================================================================================================================
/// ScriptListLoader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer 에서 읽어들이는 스크립트 목록을 읽어들이는 클래스
	@history 
	-  
*/
//=============================================================================================================================

#include <Information/SolarAcceptableMasterServerListLoader.h>
#include <Information/ScriptLoader.h>

//
//
//
#pragma pack(push, 1)

typedef struct sSCRIPT_RECORD_INFO
{
	BYTE	m_byScriptIndex;	// 0
	BYTE	m_byIsReLoad;			// 2
	CHAR	m_strFileName[64];	// 1
} SCRIPT_RECORD_INFO;

#pragma pack(pop)

//
//
//
typedef Delegator<SCRIPT_RECORD_INFO>						SCRIPT_RECORD_INFO_DELEGATOR;
typedef std::map<DWORD, SCRIPT_RECORD_INFO_DELEGATOR>		SCRIPT_RECORD_INFO_HASH;
typedef SCRIPT_RECORD_INFO_HASH::iterator					SCRIPT_RECORD_INFO_HASH_IT;
typedef std::pair<DWORD, SCRIPT_RECORD_INFO_DELEGATOR>		SCRIPT_RECORD_INFO_PAIR;

//
//
//
class ScriptListLoader
{
public:
	ScriptListLoader();
	~ScriptListLoader();

public:
	VOID			Release();
	BOOL			Load( const CHAR* pszFileName, BOOL bReload = FALSE );
	BOOL			LoadDataAll( BOOL bReload = FALSE );

	SCRIPT_RECORD_INFO_DELEGATOR&	GetScriptInfo( DWORD scriptIndex );
private:
	VOID			_Init();
	VOID			_ReleaseNode( SCRIPT_RECORD_INFO_PAIR record );

private:
	VOID			_ReleaseScriptLoader( ACCEPTABLE_MASTER_SERVER_INFO* pINFO );
	VOID			_LoadData( SCRIPT_RECORD_INFO_DELEGATOR& rINFO, SCRIPT_LOADER loader, const CHAR* pszRelativePath, BOOL bReload );
	VOID			_LoadDataAll( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR rINFO );

private:
	// <FIELDs>
	BOOL							m_bInited;
	SCRIPT_RECORD_INFO_HASH			m_ScriptListInfo;
	SCRIPT_RECORD_INFO_DELEGATOR	m_EmptyRecord;
};

//------------------------------------------------------------------
// Singleton by Private

//class ScriptListLoader
//{
//private:
//	ScriptListLoader();
//	~ScriptListLoader();
//
//public:
//	static ScriptListLoader*	s_Instance;
//	static ScriptListLoader*	GetInstance()
//	{
//		if( s_Instance )
//			return s_Instance;
//
//		s_Instance = new ScriptListLoader();
//		return s_Instance;
//	}
//};


