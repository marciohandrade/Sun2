
#include "stdafx.h"
#include "CompressStreamIO.h"
//

#include "ScriptLoader.h"


ScriptLoader::ScriptLoader()
{
	_Init();
}

ScriptLoader::~ScriptLoader()
{
	_Release();
}

VOID	ScriptLoader::_Init()
{
	m_bInited = FALSE;
	ASSERT( m_ScriptDataList.size() == 0 );
	m_ScriptDataList.clear();
	m_EmptyData.REFERENCE() = NULL;
}

VOID	ScriptLoader::_Release()
{
	for_each(
		m_ScriptDataList.begin(),
		m_ScriptDataList.end(),
		bind1st(mem_fun(&ScriptLoader::_ReleaseNode), this)
		);

	m_ScriptDataList.clear();

	m_bInited = FALSE;
}

VOID	ScriptLoader::_ReleaseNode( SCRIPT_DATA_STREAM_PAIR record )
{
	SCRIPT_DATA_STREAM_DELEGATOR& rINFO = (SCRIPT_DATA_STREAM_DELEGATOR&)record.second;
	SAFE_DELETE_ARRAY( rINFO->m_pDataStream );
	SAFE_DELETE( rINFO.REFERENCE() );
}

BOOL	ScriptLoader::_ReleaseNodeByKey( eSCRIPT_CODE scriptKey )
{
	SCRIPT_DATA_STREAM_HASH_IT it = m_ScriptDataList.find( scriptKey );
	if( it == m_ScriptDataList.end() )
		return FALSE;

	SCRIPT_DATA_STREAM_DELEGATOR& rINFO = (SCRIPT_DATA_STREAM_DELEGATOR&)it->second;
	SAFE_DELETE_ARRAY( rINFO->m_pDataStream );
	SAFE_DELETE( rINFO.REFERENCE() );

	m_ScriptDataList.erase( it );

	return TRUE;
}

SCRIPT_DATA_STREAM_DELEGATOR&
ScriptLoader::GetScriptDataStream( eSCRIPT_CODE scriptKey )
{
	SCRIPT_DATA_STREAM_HASH_IT it = m_ScriptDataList.find( scriptKey );
	if( it == m_ScriptDataList.end() )
		return m_EmptyData;

	return it->second;
}

#if WAVERIX_CODE_TEST
	extern CompressStreamIO::STREAM_INFO_NODE rTotalPlain;
	extern CompressStreamIO::STREAM_INFO_NODE rTotalCompressed;
#endif

BOOL	ScriptLoader::Load( const eSCRIPT_CODE scriptKey, const CHAR* pszFileName, BOOL bReload )
{
	if( bReload )
		_ReleaseNodeByKey( scriptKey );

	SCRIPT_DATA_STREAM_DELEGATOR& rData = GetScriptDataStream( scriptKey );
	if( rData != NULL )
		return TRUE;

	// Decode & Read Buffer
	typedef CompressStreamIO::STREAM_INFO_NODE		STREAM_INFO_NODE;
	const DWORD		ERR_FAIL_RESOURCE_ALLOC	= 10;
	const DWORD		ERR_FAIL_COMPRESS		= 20;
	const DWORD		ERR_FAIL_UNCOMPRESS		= 30;

	STREAM_INFO_NODE rPlain;
	STREAM_INFO_NODE rCompressed;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(CompressStreamIO::ResourceAlloc(pszFileName, rPlain),
                        ERR_FAIL_RESOURCE_ALLOC);
        IfFailThenBreak(CompressStreamIO::Compress(rPlain, rCompressed), ERR_FAIL_COMPRESS);
		//MessageOut( eCRITICAL_LOG, _T("Script [%u]:\"%s\" Plain %u\tCompress %u Rate :%u"),
		//	scriptKey, pszFileName, rPlain.SIZE, rCompressed.SIZE, (DWORD)((FLOAT)rPlain.SIZE/rCompressed.SIZE) );
#if WAVERIX_CODE_TEST
		rTotalPlain.SIZE += rPlain.SIZE;
		rTotalCompressed.SIZE += rCompressed.SIZE;
		_tprintf( _T("Script [%u]:\"%s\" Plain:Compress = {%5u:%5u} Rate Node:%.2f%% = Total:%.2f%%\n"),
			scriptKey, pszFileName, rPlain.SIZE, rCompressed.SIZE
			, ((FLOAT)rCompressed.SIZE/rPlain.SIZE)*100
			, ((FLOAT)rTotalCompressed.SIZE/rTotalPlain.SIZE)*100
			);
#endif

		sSCRIPT_DATA_STREAM_INFO* pINFO = new sSCRIPT_DATA_STREAM_INFO();
		//plain size & compressed size 양쪽 모두 정보를 설정 필요함.
		pINFO->m_dwFileSize = rPlain.SIZE;
		pINFO->m_CompressSize = rCompressed.SIZE;
		pINFO->m_pDataStream = rCompressed.STREAM;

		// Align Table
		m_ScriptDataList.insert( SCRIPT_DATA_STREAM_PAIR(scriptKey, pINFO) );

		return TRUE;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case ERR_FAIL_COMPRESS:
		CompressStreamIO::ResourceFree( rCompressed );
	default:
		break;
	}

	MessageOut( eCRITICAL_LOG, _T("Script Load Fail [%u]:\"%s\" %u"), scriptKey, pszFileName, flow.GetErrorCode() );
	
	return FALSE;
}

VOID	ScriptLoader::UnloadAll()
{
	//이거 사용하게 해야 하나?
	_Release();
}


