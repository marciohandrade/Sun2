#include "stdafx.h"
#include "ScriptDataManager.h"

#include <chunk_stl.h>
#include <algorithm>
#include <Struct.h>
#include <functional_custom.h>

#include <ScriptParserManager.h>

enum eSCRIPT_DATA_STATUS
{
	EMPTY_SCRIPT_DATA		= 0,
	SCRIPT_DATA_LOADING		= 1,
	SCRIPT_DATA_LOADED		= 2,
};

struct sSCRIPT_DATA_INFO_NODE_INTERNAL
{
	eSCRIPT_DATA_STATUS					m_LoadStatus;	// 비동기 처리시 동기화 유지용
	sSCRIPT_DATA_INFO_NODE				m_InfoNode;
	BYTE*								m_pDataStream;
	DWORD								m_dwDataStreamSize;
	DWORD								m_dwTotalTokens;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
typedef STLX_HASH_MAP<eSCRIPT_CODE, sSCRIPT_DATA_INFO_NODE_INTERNAL*>	SCRIPT_INSTANCE_MAP;
typedef SCRIPT_INSTANCE_MAP::value_type									SCRIPT_INSTANCE_PAIR;
typedef SCRIPT_INSTANCE_MAP::iterator									SCRIPT_INSTANCE_MAP_IT;
typedef std::pair<eSCRIPT_CODE, sSCRIPT_DATA_INFO_NODE_INTERNAL*>			SCRIPT_SEQ_PAIR;
typedef STLX_VECTOR<SCRIPT_SEQ_PAIR>									SCRIPT_SEQ_LIST;	//스크립트 의존성 순서처리
typedef SCRIPT_SEQ_LIST::iterator										SCRIPT_SEQ_LIST_IT;
typedef sSCRIPT_DATA_INFO_NODE_INTERNAL			sSCRIPT_DATA_INFO_NODE_ARRAY[SCRIPT_LIST_SIZE];

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//
struct ScriptDataManagerInternal
{
	ScriptDataManagerInternal() : m_bInited(FALSE), m_pInfoNodeBlock(NULL), m_ScriptLoadedCount(0)
	{
		m_ScriptMap.clear();
		m_ScriptDataEmptyList.clear();
		m_fnCallbackAllDataLoaded = NULL;
		m_SequencedScriptList.clear();
	}
	~ScriptDataManagerInternal()
	{
		FOREACH_CONTAINER( SCRIPT_SEQ_LIST::value_type& rNode, m_SequencedScriptList, SCRIPT_SEQ_LIST )
		{
			sSCRIPT_DATA_INFO_NODE_INTERNAL* const pInternal = rNode.second;
			if( pInternal )
				SAFE_DELETE_ARRAY( pInternal->m_pDataStream );
		}

        SAFE_DELETE_ARRAY( m_pInfoNodeBlock );
		m_ScriptLoadedCount = 0;
		m_ScriptMap.clear();
		m_ScriptDataEmptyList.clear();
		m_fnCallbackAllDataLoaded = NULL;
	}

	VOID								MoveEmptyToLoading( eSCRIPT_CODE scriptCode )
	{
		SCRIPT_LIST_IT it(m_ScriptDataEmptyList.begin()), end(m_ScriptDataEmptyList.end());

		for( ; it != end ; ++it )
		{
			eSCRIPT_CODE code = *it;
			if( code == scriptCode )
			{
				it = m_ScriptDataEmptyList.erase( it );

				SCRIPT_INSTANCE_MAP_IT it_find = m_ScriptMap.find( scriptCode );

				// 아래 조건에 걸리지 않는다는 것은 동기화가 깨졌다는 의미가 될 것이다.
				if( it_find != m_ScriptMap.end() )
				{
					sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it_find->second;
					pInternal->m_LoadStatus = SCRIPT_DATA_LOADING;
				}
				else
				{
					ASSERT( !"MoveEmptyToLoading 동기화 살펴볼 것" );
				}
				break;
			}
		} // end 'for'
	}

	VOID								MoveToEmpty( eSCRIPT_CODE scriptCode )
	{
		SCRIPT_INSTANCE_MAP_IT it = m_ScriptMap.find( scriptCode );

		// 아래 조건에 걸리지 않는다는 것은 동기화가 깨졌다는 의미가 될 것이다.
		if( it != m_ScriptMap.end() )
		{
			sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it->second;
			if( SCRIPT_DATA_LOADED == pInternal->m_LoadStatus )
				--m_ScriptLoadedCount;

			pInternal->m_LoadStatus = EMPTY_SCRIPT_DATA;
			m_ScriptDataEmptyList.insert( m_ScriptDataEmptyList.end(), scriptCode );
			return;
		}

		//if( SCRIPT_CODE_KEY(CONTROL_FLAG_UPPERBOUNDARY) > scriptCode )
		//	return;

		ASSERT( !"MoveToEmpty 동기화 살펴볼 것" );
	}

	VOID								MoveLoadingToEmpty( eSCRIPT_CODE scriptCode )
	{	MoveToEmpty( scriptCode/*, SCRIPT_DATA_LOADING*/ );		}

	VOID								MoveLoadedToEmpty( eSCRIPT_CODE scriptCode )
	{	MoveToEmpty( scriptCode/*, SCRIPT_DATA_LOADED*/ );	}

	VOID								MoveLoadingToLoaded( eSCRIPT_CODE scriptCode )
	{
		SCRIPT_INSTANCE_MAP_IT it = m_ScriptMap.find( scriptCode );

		// 아래 조건에 걸리지 않는다는 것은 동기화가 깨졌다는 의미가 될 것이다.
		if( it != m_ScriptMap.end() )
		{
			sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it->second;
			//if( !(SCRIPT_CODE_KEY(CONTROL_FLAG_UPPERBOUNDARY) > scriptCode) )
			//{
			if( pInternal->m_LoadStatus != SCRIPT_DATA_LOADING )
				ASSERT( pInternal->m_LoadStatus == SCRIPT_DATA_LOADING );
			//}
			pInternal->m_LoadStatus = SCRIPT_DATA_LOADED;
			++m_ScriptLoadedCount;
			return;
		}
		ASSERT( !"MoveLoadingToEmpty 동기화 살펴볼 것" );
	}

	VOID								_MoveStatusChangeEmptyToLoading( SCRIPT_INSTANCE_PAIR rPair )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = rPair.second;
		if( pInternal->m_LoadStatus == EMPTY_SCRIPT_DATA )
			pInternal->m_LoadStatus = SCRIPT_DATA_LOADING;
		return;
	}

	VOID								_MoveStatusChangeLoadingToEmpty( SCRIPT_INSTANCE_PAIR rPair )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = rPair.second;
		if( pInternal->m_LoadStatus == SCRIPT_DATA_LOADING )
		{
			pInternal->m_LoadStatus = EMPTY_SCRIPT_DATA;
			m_ScriptDataEmptyList.insert( m_ScriptDataEmptyList.end(), rPair.first );
		}
		return;
	}

	VOID								_MoveStatusChangeLoadedToEmpty( SCRIPT_INSTANCE_PAIR rPair )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = rPair.second;
		if( pInternal->m_LoadStatus == SCRIPT_DATA_LOADED )
		{
			--m_ScriptLoadedCount;
			pInternal->m_LoadStatus = EMPTY_SCRIPT_DATA;
			m_ScriptDataEmptyList.insert( m_ScriptDataEmptyList.end(), rPair.first );
		}
		return;
	}

	VOID								_MoveStatusChangeToEmpty( SCRIPT_INSTANCE_PAIR rPair )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = rPair.second;
		if( pInternal->m_LoadStatus == SCRIPT_DATA_LOADED )
			--m_ScriptLoadedCount;
		pInternal->m_LoadStatus = EMPTY_SCRIPT_DATA;
		m_ScriptDataEmptyList.insert( m_ScriptDataEmptyList.end(), rPair.first );
		return;
	}

	VOID								_MoveStatusChangeLoadingToLoaded( SCRIPT_INSTANCE_PAIR rPair )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = rPair.second;
		if( pInternal->m_LoadStatus == SCRIPT_DATA_LOADING )
		{
			pInternal->m_LoadStatus = SCRIPT_DATA_LOADED;
			++m_ScriptLoadedCount;
		}
		return;
	}

	VOID								MoveAllEmptyToLoading()
	{
		Foreach_Default( m_SequencedScriptList, m_SequencedScriptList.begin(), m_SequencedScriptList.end(),
			bind1st( mem_fun(&ScriptDataManagerInternal::_MoveStatusChangeEmptyToLoading), this ) );
		m_ScriptDataEmptyList.clear();
	}

	VOID								MoveAllLoadingToLoaded()
	{
		Foreach_Default( m_SequencedScriptList, m_SequencedScriptList.begin(), m_SequencedScriptList.end(),
			bind1st( mem_fun(&ScriptDataManagerInternal::_MoveStatusChangeLoadingToLoaded), this ) );
	}

	VOID								MoveAllLoadingToEmpty()
	{
		Foreach_Default( m_SequencedScriptList, m_SequencedScriptList.begin(), m_SequencedScriptList.end(),
			bind1st( mem_fun(&ScriptDataManagerInternal::_MoveStatusChangeLoadingToEmpty), this ) );
	}

	VOID								MoveAllLoadedToEmpty()
	{
		Foreach_Default( m_SequencedScriptList, m_SequencedScriptList.begin(), m_SequencedScriptList.end(),
			bind1st( mem_fun(&ScriptDataManagerInternal::_MoveStatusChangeLoadedToEmpty), this ) );
	}


	VOID								MoveAllToEmpty()
	{
		Foreach_Default( m_SequencedScriptList, m_SequencedScriptList.begin(), m_SequencedScriptList.end(),
			bind1st( mem_fun(&ScriptDataManagerInternal::_MoveStatusChangeToEmpty), this ) );

		ASSERT( m_ScriptLoadedCount == 0 );
		m_ScriptLoadedCount = 0;
	}

	// <FIELDs>
	BOOL								m_bInited;
	DWORD								m_ScriptLoadedCount;
	SCRIPT_INSTANCE_MAP					m_ScriptMap;
	SCRIPT_LIST							m_ScriptDataEmptyList;
	ScriptDataManager::fnAllDataLoaded	m_fnCallbackAllDataLoaded;

	sSCRIPT_DATA_INFO_NODE_INTERNAL*	m_pInfoNodeBlock;
	SCRIPT_SEQ_LIST						m_SequencedScriptList;
};

//----------------------------------------------------------------------------------------
// <Module Variable>
static ScriptDataManagerInternal		s_ScriptDataManagerInteral;
//
//----------------------------------------------------------------------------------------


ScriptDataManager::ScriptDataManager() {}
ScriptDataManager::~ScriptDataManager() {}

BOOL		ScriptDataManager::RegisterDataList( sSCRIPT_DATA_INFO_NODE* pInfoList )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	if( rInternal.m_bInited )
		return FALSE;

	DWORD dwListCount = 0;
	sSCRIPT_DATA_INFO_NODE* pOffset = pInfoList;
	for( ; dwListCount<SCRIPT_LIST_SIZE ; ++dwListCount, ++pOffset )
	{
		if( SCRIPT_EMPTY == pOffset->m_ScriptCode /*|| NULL == pOffset->m_pIParser*/ )
			break;
	}
	if( SCRIPT_LIST_SIZE == dwListCount )
	{
		ASSERT( !"ScriptDataManager::RegisterDataList 데이터 입력의 종결자를 확인하세요~" );
		return FALSE;
	}

	rInternal.m_pInfoNodeBlock = new sSCRIPT_DATA_INFO_NODE_ARRAY;
	ZeroMemory( rInternal.m_pInfoNodeBlock, sizeof(sSCRIPT_DATA_INFO_NODE_ARRAY) );
	sSCRIPT_DATA_INFO_NODE_ARRAY& rInfoArray = reinterpret_cast<sSCRIPT_DATA_INFO_NODE_ARRAY&>( *rInternal.m_pInfoNodeBlock );

	pOffset = pInfoList;
	for( DWORD i=0 ; i<dwListCount ; ++i, ++pOffset )
	{
		const eSCRIPT_CODE eOffset = pOffset->m_ScriptCode;

		sSCRIPT_DATA_INFO_NODE_INTERNAL& rInfoNode = rInfoArray[eOffset];
		rInfoNode.m_LoadStatus				= EMPTY_SCRIPT_DATA;
		rInfoNode.m_InfoNode.m_ScriptCode	= eOffset;
		rInfoNode.m_InfoNode.m_pIParser		= pOffset->m_pIParser;
		rInfoNode.m_InfoNode.m_bBigData		= pOffset->m_bBigData;

		const SCRIPT_INSTANCE_PAIR rPair( eOffset, &rInfoArray[eOffset] );
		rInternal.m_ScriptMap.insert( rPair );
		rInternal.m_SequencedScriptList.push_back( rPair );
		rInternal.m_ScriptDataEmptyList.push_back( eOffset );
	}

	rInternal.m_bInited = TRUE;

	return TRUE;
}

BOOL			ScriptDataManager::ReRegisterReloadDataList()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

	rInternal.MoveAllLoadingToEmpty();

	eSCRIPT_DATA_STATUS status = (eSCRIPT_DATA_STATUS)_GetDataStatus(SCRIPT_NOTIFY_SIGNAL);
	if( SCRIPT_DATA_LOADED == status )
		rInternal.MoveToEmpty( SCRIPT_NOTIFY_SIGNAL );

	return TRUE;
}

DWORD			ScriptDataManager::_GetDataStatus( eSCRIPT_CODE scriptCode )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	SCRIPT_INSTANCE_MAP_IT it = rInternal.m_ScriptMap.find( scriptCode );

	if( it != rInternal.m_ScriptMap.end() )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it->second;
		return (DWORD)pInternal->m_LoadStatus;
	}

	ASSERT( !"여기 오면 안되쥐~" );
	return EMPTY_SCRIPT_DATA;
}

SCRIPT_LIST_IT	ScriptDataManager::_GetEmptyListBegin()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	return rInternal.m_ScriptDataEmptyList.begin();
}

SCRIPT_LIST_IT	ScriptDataManager::_GetEmptyListEnd()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	return rInternal.m_ScriptDataEmptyList.end();
}

DWORD		ScriptDataManager::GetNeedLoadDataCount()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	return (DWORD)rInternal.m_ScriptDataEmptyList.size();
}


BOOL		ScriptDataManager::SaveDataStreamToken( eSCRIPT_CODE scriptCode, DWORD dwTokenNo, DWORD dwTokenSize, BYTE* pSTREAM )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

	SCRIPT_INSTANCE_MAP_IT it_find = rInternal.m_ScriptMap.find( scriptCode );
	if( it_find == rInternal.m_ScriptMap.end() )
	{
		ASSERT( !"없는 스크립트 정보를 향해 명령을 내렸다!" );
		return FALSE;
	}

	sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it_find->second;
	sSCRIPT_DATA_INFO_NODE& rInfoNode = pInternal->m_InfoNode;

	if( !( SCRIPT_CONTROL_FLAG_UPPERBOUNDARY > scriptCode ) )
		if( SCRIPT_DATA_LOADED == pInternal->m_LoadStatus )
		{
			ASSERT( !"이미 완료된 스크립트 데이터를 변경하려고 하고 있다. 중복 요청 확인할 것" );
			return FALSE;
		}

		if( NULL == pInternal->m_pDataStream )
		{
			ASSERT( !"큰 일이다. 버퍼가 없다!" );
			return FALSE;
		}

		// <START LOGIC>
		if( !(dwTokenNo < pInternal->m_dwTotalTokens) )
		{
			ASSERT( !"Index Out Of Range" );
			return FALSE;
		}

		BYTE* pOffset = pInternal->m_pDataStream;
		BYTE* pBoundary = pOffset + (c_dwMaxTokenSize*pInternal->m_dwTotalTokens);	// Buffer size 고려해야 한다.
		pOffset = pOffset+(c_dwMaxTokenSize*dwTokenNo);

		if( pOffset+dwTokenSize > pBoundary )
		{
			ASSERT( !"Buffer Overrun 될 수 있는 값을 가졌다" );
			return FALSE;
		}

		memcpy( pOffset, pSTREAM, dwTokenSize );

		return TRUE;
}

class ScriptDataSubControl : public std::unary_function<sSCRIPT_DATA_INFO_NODE_INTERNAL*, VOID>
{
public:
	//VOID		operator() ( sSCRIPT_DATA_INFO_NODE_INTERNAL* ppp ) const
	VOID		_ProcessLoadedData( sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal ) const
	{
		sSCRIPT_DATA_INFO_NODE& rInfoNode = pInternal->m_InfoNode;

		if( SCRIPT_DATA_LOADED != pInternal->m_LoadStatus )
		{
			ASSERT( !"완료되지 않은 스크립트 데이터를 처리하려고 했다." );
			return;
		}

		if( NULL == pInternal->m_pDataStream )
		{
			ASSERT( !"큰 일이다. 버퍼가 없다!" );
			return;
		}

		if( IParser* pIParser = pInternal->m_InfoNode.m_pIParser )
		{
			//	(Waverix)(NOTE)	압축 스트림 사용시 char에 대해서 처리 확인이 필요하다 꼭 검증할 것.
			DWORD dwFileSize = pInternal->m_dwDataStreamSize;
			pIParser->SolarLoad( rInfoNode.m_ScriptCode, pInternal->m_pDataStream, dwFileSize, pInternal->m_InfoNode.m_bBigData );
		}
	}
};


BOOL		ScriptDataManager::ProcessAllLoadedData()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

    const SCRIPT_SEQ_LIST& rList = rInternal.m_SequencedScriptList;

    ScriptDataSubControl control;
    FOREACH_CONTAINER(const SCRIPT_SEQ_LIST::value_type& node, rList, SCRIPT_SEQ_LIST)
    {
        control._ProcessLoadedData(node.second);
    }

    //Foreach_Default( rList, rList.begin(), rList.end(),
    //    compose1(
    //        bind1st( mem_fun_ref(ScriptDataSubControl::_ProcessLoadedData), ScriptDataSubControl() ),
    //        select2nd<SCRIPT_SEQ_PAIR>()
    //    )
    //);

    return TRUE;
}

BOOL		ScriptDataManager::ProcessLoadedData( eSCRIPT_CODE scriptCode )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

	SCRIPT_INSTANCE_MAP_IT it_find = rInternal.m_ScriptMap.find( scriptCode );
	if( it_find == rInternal.m_ScriptMap.end() )
	{
		ASSERT( !"없는 스크립트 정보를 향해 명령을 내렸다!" );
		return FALSE;
	}

	sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it_find->second;
	sSCRIPT_DATA_INFO_NODE& rInfoNode = pInternal->m_InfoNode;

	if( SCRIPT_DATA_LOADED != pInternal->m_LoadStatus )
	{
		ASSERT( !"완료되지 않은 스크립트 데이터를 처리하려고 했다." );
		return FALSE;
	}

	if( NULL == pInternal->m_pDataStream )
	{
		ASSERT( !"큰 일이다. 버퍼가 없다!" );
		return FALSE;
	}

	// <NEED CRC Check>
	//   ...
	//OFSTRUCT pOF;
	//CHAR filename[20];
	//_snprintf( filename, sizeof(filename), "SC-%02u.txt", scriptCode );
	//HANDLE hFILE = (HANDLE)(__int64)OpenFile( filename, &pOF, OF_CREATE );
	//DWORD dwWritten;

	//if( INVALID_HANDLE_VALUE == hFILE )
	//{
	//	printf( "%u\n", GetLastError() );
	//}

	//WriteFile( hFILE, pInternal->m_pDataStream, pInternal->m_dwDataStreamSize - sizeof(CHAR), &dwWritten, NULL );
	//FlushFileBuffers( hFILE );
	//CloseHandle( (HANDLE)hFILE );

	// 실패하면--> 기존 버퍼 문제 해결하고, Empty상태로 전환할 필요가 있다.
	// 성공하면,

	if( IParser* pIParser = pInternal->m_InfoNode.m_pIParser )
	{
		const DWORD file_size = pInternal->m_dwDataStreamSize;
		return pIParser->SolarLoad( (eSCRIPT_CODE)scriptCode, pInternal->m_pDataStream, file_size, pInternal->m_InfoNode.m_bBigData );
	}

	return TRUE;
}

BOOL		ScriptDataManager::DataStreamCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode, BYTE*& rpSTREAM, DWORD& rdwSize )
{
	if( (SCRIPT_DATA_CMD::MASK_DATASTREAM_CMD & cmd) != cmd )
		return FALSE;

	const eSCRIPT_DATA_CMD cmdGetSet	= cmd & SCRIPT_DATA_CMD::MASK_CMD;
	const eSCRIPT_DATA_CMD cmdBuffer	= cmd & SCRIPT_DATA_CMD::BUFFERINFO::MASK;

	if( SCRIPT_DATA_CMD::MASK_CMD == cmdGetSet )
		return FALSE;

	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

	SCRIPT_INSTANCE_MAP_IT it_find = rInternal.m_ScriptMap.find( scriptCode );
	if( it_find == rInternal.m_ScriptMap.end() )
	{
		ASSERT( !"없는 스크립트 정보를 향해 명령을 내렸다!" );
		return FALSE;
	}

	sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it_find->second;
	//sSCRIPT_DATA_INFO_NODE& rInfoNode = pInternal->m_InfoNode;

	if( SCRIPT_DATA_CMD::SET == cmdGetSet )
	{
		//   (o) CMD::SET | CMD::BUFFER | CMD::BUFFER_SIZE --> 주어진 버퍼로 설정한다. 기존 버퍼 있을 경우 제거
		//   (x) CMD::SET | CMD::BUFFER
		//   (o) CMD::SET | CMD::BUFFER_SIZE --> CreateBuffer
		DWORD dwBufferSize = 0;
		BYTE* pSTREAM = NULL;

		if( SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE&cmdBuffer )
			dwBufferSize = rdwSize;
		if( SCRIPT_DATA_CMD::BUFFERINFO::BUFFER&cmdBuffer )
			pSTREAM = rpSTREAM;

		if( dwBufferSize )
		{
			// 실수해서 같은 포인터를 설정하는 경우도 있을 것이다. 이점 찾아야 한다.
			if( rpSTREAM && pInternal->m_pDataStream && pInternal->m_pDataStream == rpSTREAM )
			{
				if( pInternal->m_dwDataStreamSize == dwBufferSize )
					return TRUE;
				else
				{
					ASSERT( !"동일한 버퍼 포인터를 사용하려고 했어요." );
					return FALSE;
				}
			}

			const DWORD alloc_stream_sz = dwBufferSize+10;
			SAFE_DELETE_ARRAY( pInternal->m_pDataStream );
			pInternal->m_pDataStream = pSTREAM ? pSTREAM : new BYTE[alloc_stream_sz];

			{	// calculate size & tokens
				const DWORD tokens = (DWORD)(dwBufferSize/c_dwMaxTokenSize);
				const DWORD remainSz = (DWORD)(dwBufferSize%c_dwMaxTokenSize);
				const DWORD total_tokens = tokens + (DWORD)(!!remainSz);
				pInternal->m_dwDataStreamSize = dwBufferSize;
				pInternal->m_dwTotalTokens = total_tokens;
			};

			return TRUE;
		}
		else
		{
			ASSERT( !"잘못된 명령을 내렸다" );
			return FALSE;
		}
	}
	else//if( SCRIPT_DATA_CMD::GET == cmdGetSet )
	{
		//   (o) CMD::GET | CMD::BUFFER | CMD::BUFFER_SIZE --> 현재 설정된 버퍼의 포인터와 그 크기를 반환한다.
		//   (o) CMD::GET | CMD::BUFFER_SIZE --> 현재 설정된 버퍼의 크기를 반환한다.
		//   (o) CMD::GET | CMD::BUFFER -->  현재 설정된 버퍼 포인터를 반환한다. (크기 정의 없으므로 주의할 것)
		rdwSize = 0;
		rpSTREAM = NULL;

		if( SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE&cmdBuffer )
			rdwSize = pInternal->m_dwDataStreamSize;
		if( SCRIPT_DATA_CMD::BUFFERINFO::BUFFER&cmdBuffer )
			rpSTREAM = pInternal->m_pDataStream;

		return TRUE;
	}
	return FALSE;
}

BOOL		ScriptDataManager::DataStatusCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode, eSCRIPT_DATA_CMD* result )
{
	if( (SCRIPT_DATA_CMD::MASK_DATASTATUS_CMD & cmd) != cmd )
		return FALSE;

	const eSCRIPT_DATA_CMD cmdGetSet	= cmd & SCRIPT_DATA_CMD::MASK_CMD;
	const eSCRIPT_DATA_CMD cmdRange		= cmd & SCRIPT_DATA_CMD::RANGE::MASK;
	const eSCRIPT_DATA_CMD cmdSubCmd	= cmd & SCRIPT_DATA_CMD::STATUS::MASK;

	if( SCRIPT_DATA_CMD::MASK_CMD == cmdGetSet )
		return FALSE;

	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;

	if( SCRIPT_DATA_CMD::SET == cmdGetSet )
	{
		switch( cmdSubCmd )
		{
		case SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA:
			{
				if( SCRIPT_DATA_CMD::RANGE::ONENODE == cmdRange )
					rInternal.MoveToEmpty( scriptCode );
				else //( SCRIPT_DATA_CMD::RANGE::ALL == cmdRange )
					rInternal.MoveAllToEmpty();
			}
			break;

		case SCRIPT_DATA_CMD::STATUS::MOVE2LOADED:
			{
				if( SCRIPT_DATA_CMD::RANGE::ONENODE == cmdRange )
				{
					rInternal.MoveLoadingToLoaded( scriptCode );
					// callback 처리하자.
					if( rInternal.m_ScriptMap.size() == rInternal.m_ScriptLoadedCount )
					{
						ASSERT( rInternal.m_fnCallbackAllDataLoaded );
						if( rInternal.m_fnCallbackAllDataLoaded )
                        {
							rInternal.m_fnCallbackAllDataLoaded();
                        #if defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
                            ScriptParserManager::FinishServerDataLoad();
                        #endif
                        }
					}
				}
				else
				{
					ASSERT( !"정상적인 명령이 아닙니다." );
				}
			}
			break;

		case SCRIPT_DATA_CMD::STATUS::MOVE2LOADING:
			{
				if( SCRIPT_DATA_CMD::RANGE::ONENODE == cmdRange )
					rInternal.MoveEmptyToLoading( scriptCode );
				// 아래와 같은 경우는 일괄 명령을 내린 상태에서 가능할 것이다.
				else if( SCRIPT_DATA_CMD::RANGE::ALL == cmdRange )
					rInternal.MoveAllEmptyToLoading();
			}
			break;
		}
	}
	else //if( SCRIPT_DATA_CMD::GET == cmdGetSet )
	{
	}
	return TRUE;
}

BOOL		ScriptDataManager::IsLoadedData( eSCRIPT_CODE scriptCode )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	SCRIPT_INSTANCE_MAP_IT it = rInternal.m_ScriptMap.find( scriptCode );

	if( it != rInternal.m_ScriptMap.end() )
	{
		sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it->second;
		if( SCRIPT_DATA_LOADED == pInternal->m_LoadStatus )
			return TRUE;
	}
	return FALSE;
}

BOOL		ScriptDataManager::IsAllDataLoaded()
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	return (BOOL)(rInternal.m_ScriptMap.size() == rInternal.m_ScriptLoadedCount);
}

VOID		ScriptDataManager::RegisterCallbackAllDataLoaded( fnAllDataLoaded fnHandler )
{
	ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
	rInternal.m_fnCallbackAllDataLoaded = fnHandler;
}

VOID		ScriptDataManager::AsyncReloadedDataCallback( eSCRIPT_CODE scriptCode, BYTE* pSTREAM )
{
	if( pSTREAM )
	{
		ScriptDataManagerInternal& rInternal = s_ScriptDataManagerInteral;
		SCRIPT_INSTANCE_MAP_IT it = rInternal.m_ScriptMap.find( scriptCode );

		if( it != rInternal.m_ScriptMap.end() )
		{
			sSCRIPT_DATA_INFO_NODE_INTERNAL* pInternal = it->second;
			sSCRIPT_DATA_INFO_NODE& rInfoNode = pInternal->m_InfoNode;
			//rInfoNode.m_pILoader->ILoad( rInfoNode.m_addin_arg0, pSTREAM );
			// <WARNING!>
			delete pSTREAM;
		}
		return;
	}
	ASSERT( !"ScriptDataManager::AsyncReloadedDataCallback 여기 오면 안되쥐~" );
	return;	// ASSERTION?
}
