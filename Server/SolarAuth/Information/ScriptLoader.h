#pragma once

//=============================================================================================================================
/// ScriptLoader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 21
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer 에서 읽어들이는 스크립트 목록에 기반해 해당 파일을 읽어들이는 클래스
	@history 
	-  
*/
//=============================================================================================================================

struct sSCRIPT_DATA_STREAM_INFO
{
	DWORD		m_dwFileSize;
	DWORD		m_CompressSize;
	BYTE*		m_pDataStream;
};

//
//
//
class ScriptLoader;

//#include <ScriptCode.Definition.h>
#include <ScriptCode.IParser.h>

typedef Delegator<sSCRIPT_DATA_STREAM_INFO>					SCRIPT_DATA_STREAM_DELEGATOR;
typedef STLX_MAP<DWORD, SCRIPT_DATA_STREAM_DELEGATOR>		SCRIPT_DATA_STREAM_HASH;
typedef SCRIPT_DATA_STREAM_HASH::iterator					SCRIPT_DATA_STREAM_HASH_IT;
typedef std::pair<DWORD, SCRIPT_DATA_STREAM_DELEGATOR>		SCRIPT_DATA_STREAM_PAIR;

typedef Delegator<ScriptLoader>								SCRIPT_LOADER;


//
//
//
class ScriptLoader
{
public:
	ScriptLoader();
	~ScriptLoader();

public:
	BOOL							Load( const eSCRIPT_CODE scriptKey, const CHAR* pszFileName, BOOL bReload = FALSE );
	SCRIPT_DATA_STREAM_DELEGATOR&	GetScriptDataStream( eSCRIPT_CODE scriptKey );

public:
	VOID							UnloadAll();
private:
	VOID							_Init();
	VOID							_Release();
	VOID							_ReleaseNode( SCRIPT_DATA_STREAM_PAIR record );
	BOOL							_ReleaseNodeByKey( eSCRIPT_CODE scriptKey );

private:
	// <FIELDs>
	BOOL							m_bInited;
	SCRIPT_DATA_STREAM_HASH			m_ScriptDataList;
	SCRIPT_DATA_STREAM_DELEGATOR	m_EmptyData;
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


