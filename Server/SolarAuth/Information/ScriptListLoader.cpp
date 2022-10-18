
#include "stdafx.h"

//
#include "FileListLoader.h"
#include "FileDecoder.h"
//

#include "ScriptLoadManager.h"
#include "ScriptListLoader.h"
#include "ScriptLoader.h"

static BYTE			ms_ScriptNotifySignalCode[4] = { '0', '1', '2', 0 };

ScriptListLoader::ScriptListLoader()
{
	_Init();
}

ScriptListLoader::~ScriptListLoader()
{
	Release();
}

VOID	ScriptListLoader::_Init()
{
	m_bInited = FALSE;
	ASSERT( m_ScriptListInfo.size() == 0 );
	m_ScriptListInfo.clear();
	m_EmptyRecord.REFERENCE() = NULL;
}

VOID	ScriptListLoader::_ReleaseNode( SCRIPT_RECORD_INFO_PAIR record )
{
	SCRIPT_RECORD_INFO_DELEGATOR& rINFO = (SCRIPT_RECORD_INFO_DELEGATOR&)record.second;
	SAFE_DELETE( rINFO.REFERENCE() );
}

SCRIPT_RECORD_INFO_DELEGATOR&
ScriptListLoader::GetScriptInfo( DWORD scriptIndex )
{
	SCRIPT_RECORD_INFO_HASH_IT it = m_ScriptListInfo.find( scriptIndex );
	if( it == m_ScriptListInfo.end() )
		return m_EmptyRecord;

	return it->second;
}

BOOL	ScriptListLoader::Load( const CHAR* pszFileName, BOOL bReload )
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
		// Notify Signal 등록

		FileListLoader loader;

		if( !loader.Load( pszFileName, FALSE ) )
		{
			MessageOut( eFULL_LOG, "Script File List : [Can't Open File : %s]", pszFileName );
			return FALSE;
		}

		MessageOut( eFULL_LOG, "Script File List : [READING]" );

		BOOL bSuccess = TRUE;
		do
		{
			SCRIPT_RECORD_INFO* pINFO = new SCRIPT_RECORD_INFO;
			memset( pINFO, 0, sizeof(SCRIPT_RECORD_INFO) );

			BOOL bCompleteSentence = FALSE;
			if( bSuccess = loader.ReadValue( pINFO->m_byScriptIndex ) )
				if( bCompleteSentence = loader.ReadString( pINFO->m_strFileName, sizeof(pINFO->m_strFileName) ) )
				{
					pINFO->m_strFileName[sizeof(pINFO->m_strFileName)-1] = '\0';
					if( bCompleteSentence = loader.ReadValue( pINFO->m_byIsReLoad ) )
					{ ; }
				}

				if( bCompleteSentence )
				{
					SCRIPT_RECORD_INFO_DELEGATOR delegatorINFO = GetScriptInfo( pINFO->m_byScriptIndex );
					if( delegatorINFO != NULL )
					{
						ASSERT( !"키가 중복되고 있습니다\n" );
						MessageOut( eCRITICAL_LOG, "스크립트 목록 중복키 (%u:%s)가 존재합니다.",	\
							delegatorINFO->m_byScriptIndex, delegatorINFO->m_strFileName );
						SAFE_DELETE( delegatorINFO.REFERENCE() );
						break;
					}

					// 각 데이터 검증 할까 말까?
					// 요구사항이 있으면 하기로 하자...

					delegatorINFO = SCRIPT_RECORD_INFO_DELEGATOR( pINFO );

					m_ScriptListInfo.insert(
						m_ScriptListInfo.end(),
						SCRIPT_RECORD_INFO_PAIR( pINFO->m_byScriptIndex, delegatorINFO )
						);
				}
				else
				{
					SAFE_DELETE( pINFO );
				}
		} while(bSuccess);

		MessageOut( eFULL_LOG, "Script File List : [READED]" );

		//loader.Close();
		m_bInited = TRUE;
	}

	return TRUE;;
}

VOID	ScriptListLoader::_ReleaseScriptLoader( ACCEPTABLE_MASTER_SERVER_INFO* pINFO )
{
	SAFE_DELETE( pINFO->m_ScriptLoaderHandle.REFERENCE() );
}

VOID	ScriptListLoader::Release()
{
	for_each(
		m_ScriptListInfo.begin(),
		m_ScriptListInfo.end(),
		bind1st(mem_fun(&ScriptListLoader::_ReleaseNode), this)
		);

	m_ScriptListInfo.clear();

	SolarAcceptableMasterServerListLoaderDelegator& rServerList =
		ScriptLoadManager::Instance()->SolarAcceptableMasterServerListLoaderHandle;

//	rServerList->ForeachServerList( &ScriptListLoader::_ReleaseScriptLoader, this );

	m_bInited = FALSE;
}

VOID	ScriptListLoader::_LoadData( SCRIPT_RECORD_INFO_DELEGATOR& rINFO, SCRIPT_LOADER loader, const CHAR* pszRelativePath, BOOL bReload )
{
	const eSCRIPT_CODE dwScirptCode = (eSCRIPT_CODE)rINFO->m_byScriptIndex;
	static CHAR pszRelativeFileName[512];

	_snprintf( pszRelativeFileName, sizeof(pszRelativeFileName),
		"%s\\%s", pszRelativePath, rINFO->m_strFileName );

	loader->Load( dwScirptCode, pszRelativeFileName, bReload );

	return;
}

//BOOL	ScriptListLoader::LoadData( DWORD scriptIndex, BOOL bReload )
//{
//	SCRIPT_RECORD_INFO_DELEGATOR& rINFO = GetScriptInfo( scriptIndex );
//	if( rINFO == NULL )
//		return FALSE;
//
//	ScriptLoader::Load( rINFO->m_byScriptIndex, rINFO->m_strFileName, bReload );
//	return TRUE;
//}

#if WAVERIX_CODE_TEST
	#include "CompressStreamIO.h"

	CompressStreamIO::STREAM_INFO_NODE rTotalPlain;
	CompressStreamIO::STREAM_INFO_NODE rTotalCompressed;
#endif


VOID	ScriptListLoader::_LoadDataAll( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR rINFO )
{
	SCRIPT_RECORD_INFO_HASH_IT it(m_ScriptListInfo.begin());
	SCRIPT_RECORD_INFO_HASH_IT end(m_ScriptListInfo.end());

	static CHAR pszRelativePath[256];
	_snprintf( pszRelativePath, sizeof(pszRelativePath), "%s", rINFO->m_strDataPath );

#if WAVERIX_CODE_TEST
	ZeroMemory( &rTotalPlain, sizeof(rTotalPlain) );
	ZeroMemory( &rTotalCompressed, sizeof(rTotalCompressed) );
#endif

	rINFO->m_ScriptLoaderHandle = new ScriptLoader();
	for( ; it != end ; ++it )
	{
		SCRIPT_RECORD_INFO_DELEGATOR& rRECORD = it->second;
		_LoadData( rRECORD, rINFO->GetScriptLoader(), pszRelativePath, FALSE );
	}

#if WAVERIX_CODE_TEST
	_tprintf( _T("Script Total Plain %u\tCompress %u Rate :%.2f%%"),
		rTotalPlain.SIZE, rTotalCompressed.SIZE,
		((FLOAT)rTotalCompressed.SIZE/rTotalPlain.SIZE)*100 );
	puts("");
#endif
}

BOOL	ScriptListLoader::LoadDataAll( BOOL bReload )
{
	SolarAcceptableMasterServerListLoaderDelegator& rServerList =
		ScriptLoadManager::Instance()->SolarAcceptableMasterServerListLoaderHandle;

	rServerList->ForeachServerList( &ScriptListLoader::_LoadDataAll, this );

	return TRUE;
}


