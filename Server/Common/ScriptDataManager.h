#pragma once

//=============================================================================================================================
/// ScriptDataManager class
/**
@author	Lee Eun-pa <waverix@webzen.co.kr>
@since	2006. 08. 23
@remark
- 서버별 Script Data 들을 한곳에서 관리하기 위한 클래스
@note
- 서버별로 Script Data를 로드하는 것이 틀리고 이러한 데이터를 전송받는 형태에서 필요하기 때문
@history 
-  
*/
//=============================================================================================================================

#include <ScriptCode.IParser.h>
#include <Struct.h>

typedef DWORD	eSCRIPT_DATA_CMD;
struct SCRIPT_DATA_CMD
{
	// CMD [OUT]
	static const eSCRIPT_DATA_CMD GET				= 0x00000001;
	// CMD [IN]
	static const eSCRIPT_DATA_CMD SET				= 0x00000002;

	// MASK
	static const eSCRIPT_DATA_CMD MASK_CMD			= GET|SET;

	struct RANGE
	{
		// RANGE
		static const eSCRIPT_DATA_CMD ALL				= 0x00000010;
		// RANGE
		static const eSCRIPT_DATA_CMD ONENODE			= 0x00000100;

		//
		static const eSCRIPT_DATA_CMD MASK				= ALL|ONENODE;
	};

	struct BUFFERINFO
	{
		// DataStreamCommand
		static const eSCRIPT_DATA_CMD BUFFER			= 0x00001000;
		// DataStreamCommand
		static const eSCRIPT_DATA_CMD BUFFER_SIZE		= 0x00002000;

		//
		static const eSCRIPT_DATA_CMD MASK				= BUFFER|BUFFER_SIZE;
	};

	struct STATUS
	{
		// DataStatusCommand
		static const eSCRIPT_DATA_CMD MOVE2EMPTYDATA	= 0x00010000;
		// DataStatusCommand
		static const eSCRIPT_DATA_CMD MOVE2LOADING		= 0x00020000;
		// DataStatusCommand
		static const eSCRIPT_DATA_CMD MOVE2LOADED		= 0x00040000;

		//
		static const eSCRIPT_DATA_CMD MASK				= MOVE2EMPTYDATA|MOVE2LOADING|MOVE2LOADED;
	};

	//
	static const eSCRIPT_DATA_CMD MASK_DATASTREAM_CMD	= MASK_CMD|BUFFERINFO::MASK;
	//
	static const eSCRIPT_DATA_CMD MASK_DATASTATUS_CMD	= MASK_CMD|RANGE::MASK|STATUS::MASK;
};

typedef STLX_LIST<eSCRIPT_CODE>							SCRIPT_LIST;
typedef SCRIPT_LIST::iterator							SCRIPT_LIST_IT;

class ScriptDataManager
{
public:
	//MSG_SM_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO::MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE
	//의 값과 동일하게 사용할 수 있도록 수정할 것
	static const DWORD c_dwMaxTokenSize = 4000;
public:
	// <INTERFACEs>

	// <RegisterDataList>
	// <Usage>
	//	sSCRIPT_DATA_INFO_NODE pList =
	//	{
	//		SCRIPT_DATA_INFO_SET(        ScriptCode,               Parser*,          ),
	//		SCRIPT_DATA_INFO_SET( GroupParser::ScriptCode, GroupParser::Instance(),  ),
	//			.
	//			.
	//			.
	//		SCRIPT_DATA_INFO_SET_EMPTY(),
	//	};
	//	ScriptDataManager::RegisterDataList( pList );
	//
	static BOOL		RegisterDataList( sSCRIPT_DATA_INFO_NODE* pInfoList );

	// <ReRegisterReloadDataList>
	//   아직 데이터 로딩이 완료되지 않은 상황에서 네트워크가 단절되거나 하는 상황이 생기면,
	//   LOADING 상태에 존재하는 목록들을 찾아서 다시 EMPTY로 돌려줘야 하는 상황이 생긴다.
	//   새롭게 시작하는 경우는 신경쓰지 않아도 되지만, 계속 동작중인 경우에 대해서 이 문제는
	//   충분히 생길 수 있다.
	static BOOL		ReRegisterReloadDataList();

	static BOOL		IsLoadedData( eSCRIPT_CODE scriptCode );

	static BOOL		IsAllDataLoaded();

	// <ProcessLoadedData>
	//   ProcessLoadedData은 외부 인터페이스로 별도로 해당 데이터를 처리하는 경우에 사용한다
	//   IParser가 등록되지 않았거나 LOADED상태가 아니면 특별한 일은 수행하지 않는다. 
	//   아, 한가지 추가한다. CRC확인... (060826 현재, 미구현)
	//
	//   WorkLoadManager를 통한, 데이터 입/출력 수행 완료되면, 내부적으로 AsyncReloadedDataCallback을 수행하게 될 것이다.
	//   AsyncReloadedDataCallback은 IParser가 존재하면 수행한다.
	// <returns>
	//   T : 데이터 입/출력 수행 시작
	//   F : 이미 수행중이거나, 그럴 필요 없거나, 어떠한 이유로 인해 처리를 수행할 수 없는 상황
	static BOOL		ProcessLoadedData( eSCRIPT_CODE scriptCode );

	static BOOL		ProcessAllLoadedData();

	static BOOL		SaveDataStreamToken( eSCRIPT_CODE scriptCode, DWORD dwTokenNo, DWORD dwTokenSize, BYTE* pSTREAM );

	//--------------------------------------------------------------------------------------------------------
	// 데이터 로드가 필요한 경우

	// <ForEach_NeedLoadData>
	//   데이터 로드가 필요한 스크립트 번호를 인자로 해서 함수를 호출해준다.(루프)
	template<typename _FN, typename _ObjType>
	static VOID ForEach_NeedLoadData( _FN _funcRaw, const _ObjType& _Left )
	{
		std::for_each( _GetEmptyListBegin(), _GetEmptyListEnd(), std::bind1st(std::mem_fun(_funcRaw), _Left) );
	}

	// <GetNeedLoadDataCount>
	//   데이터 로드가 필요한 스크립트 개수를 반환한다.
	static DWORD	GetNeedLoadDataCount();
	//
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------
	// 데이터 로딩 중인 경우
	//
	//--------------------------------------------------------------------------------------------------------

	// <DataStreamCommand>
	//   (o) CMD::GET | CMD::BUFFER | CMD::BUFFER_SIZE --> 현재 설정된 버퍼의 포인터와 그 크기를 반환한다.
	//   (o) CMD::GET | CMD::BUFFER_SIZE --> 현재 설정된 버퍼의 크기를 반환한다.
	//   (o) CMD::GET | CMD::BUFFER -->  현재 설정된 버퍼 포인터를 반환한다. (크기 정의 없으므로 주의할 것)
	//   (o) CMD::SET | CMD::BUFFER | CMD::BUFFER_SIZE --> 주어진 버퍼로 설정한다. 기존 버퍼 있을 경우 제거
	//   (x) CMD::SET | CMD::BUFFER
	//   (o) CMD::SET | CMD::BUFFER_SIZE --> CreateBuffer

	static BOOL		DataStreamCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode, BYTE*& rpSTREAM, DWORD& rdwSize );

	// <WARNING> 상태 전환만 처리한다. 주의하세용~
	// LOADED상태에서 IParser를 수행시키려면 ProcessLoadedData를 사용해 처리해야 한다.
	static BOOL		DataStatusCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode = SCRIPT_EMPTY, eSCRIPT_DATA_CMD* result = NULL );

public:
	// <CALLBACKs>
	typedef VOID	(*fnAllDataLoaded)();
	// <RegisterCallbackAllDataLoaded>
	//   이 함수는 EMPTY, LOADING상태가 없이 모두 LOADED상태에 있을 때 호출해 준다.
	//   발동 조건, 특정 Script가 LOADED상태로 전환될 때 검사되고 호출된다.
	static VOID		RegisterCallbackAllDataLoaded( fnAllDataLoaded fnHandler );

private://eSERVER_TYPE
	ScriptDataManager();
	~ScriptDataManager();

	static VOID		AsyncReloadedDataCallback( eSCRIPT_CODE scriptCode, BYTE* pSTREAM );
	//static VOID		ReloadInstruction( eSCRIPT_CODE scriptCode, BYTE* pSTREAM )

	static SCRIPT_LIST_IT	_GetEmptyListBegin();
	static SCRIPT_LIST_IT	_GetEmptyListEnd();
	static DWORD			_GetDataStatus( eSCRIPT_CODE scriptCode );
};

