#pragma once

//=============================================================================================================================
/// ScriptDataManager class
/**
@author	Lee Eun-pa <waverix@webzen.co.kr>
@since	2006. 08. 23
@remark
- ������ Script Data ���� �Ѱ����� �����ϱ� ���� Ŭ����
@note
- �������� Script Data�� �ε��ϴ� ���� Ʋ���� �̷��� �����͸� ���۹޴� ���¿��� �ʿ��ϱ� ����
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
	//�� ���� �����ϰ� ����� �� �ֵ��� ������ ��
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
	//   ���� ������ �ε��� �Ϸ���� ���� ��Ȳ���� ��Ʈ��ũ�� �����ǰų� �ϴ� ��Ȳ�� �����,
	//   LOADING ���¿� �����ϴ� ��ϵ��� ã�Ƽ� �ٽ� EMPTY�� ������� �ϴ� ��Ȳ�� �����.
	//   ���Ӱ� �����ϴ� ���� �Ű澲�� �ʾƵ� ������, ��� �������� ��쿡 ���ؼ� �� ������
	//   ����� ���� �� �ִ�.
	static BOOL		ReRegisterReloadDataList();

	static BOOL		IsLoadedData( eSCRIPT_CODE scriptCode );

	static BOOL		IsAllDataLoaded();

	// <ProcessLoadedData>
	//   ProcessLoadedData�� �ܺ� �������̽��� ������ �ش� �����͸� ó���ϴ� ��쿡 ����Ѵ�
	//   IParser�� ��ϵ��� �ʾҰų� LOADED���°� �ƴϸ� Ư���� ���� �������� �ʴ´�. 
	//   ��, �Ѱ��� �߰��Ѵ�. CRCȮ��... (060826 ����, �̱���)
	//
	//   WorkLoadManager�� ����, ������ ��/��� ���� �Ϸ�Ǹ�, ���������� AsyncReloadedDataCallback�� �����ϰ� �� ���̴�.
	//   AsyncReloadedDataCallback�� IParser�� �����ϸ� �����Ѵ�.
	// <returns>
	//   T : ������ ��/��� ���� ����
	//   F : �̹� �������̰ų�, �׷� �ʿ� ���ų�, ��� ������ ���� ó���� ������ �� ���� ��Ȳ
	static BOOL		ProcessLoadedData( eSCRIPT_CODE scriptCode );

	static BOOL		ProcessAllLoadedData();

	static BOOL		SaveDataStreamToken( eSCRIPT_CODE scriptCode, DWORD dwTokenNo, DWORD dwTokenSize, BYTE* pSTREAM );

	//--------------------------------------------------------------------------------------------------------
	// ������ �ε尡 �ʿ��� ���

	// <ForEach_NeedLoadData>
	//   ������ �ε尡 �ʿ��� ��ũ��Ʈ ��ȣ�� ���ڷ� �ؼ� �Լ��� ȣ�����ش�.(����)
	template<typename _FN, typename _ObjType>
	static VOID ForEach_NeedLoadData( _FN _funcRaw, const _ObjType& _Left )
	{
		std::for_each( _GetEmptyListBegin(), _GetEmptyListEnd(), std::bind1st(std::mem_fun(_funcRaw), _Left) );
	}

	// <GetNeedLoadDataCount>
	//   ������ �ε尡 �ʿ��� ��ũ��Ʈ ������ ��ȯ�Ѵ�.
	static DWORD	GetNeedLoadDataCount();
	//
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------
	// ������ �ε� ���� ���
	//
	//--------------------------------------------------------------------------------------------------------

	// <DataStreamCommand>
	//   (o) CMD::GET | CMD::BUFFER | CMD::BUFFER_SIZE --> ���� ������ ������ �����Ϳ� �� ũ�⸦ ��ȯ�Ѵ�.
	//   (o) CMD::GET | CMD::BUFFER_SIZE --> ���� ������ ������ ũ�⸦ ��ȯ�Ѵ�.
	//   (o) CMD::GET | CMD::BUFFER -->  ���� ������ ���� �����͸� ��ȯ�Ѵ�. (ũ�� ���� �����Ƿ� ������ ��)
	//   (o) CMD::SET | CMD::BUFFER | CMD::BUFFER_SIZE --> �־��� ���۷� �����Ѵ�. ���� ���� ���� ��� ����
	//   (x) CMD::SET | CMD::BUFFER
	//   (o) CMD::SET | CMD::BUFFER_SIZE --> CreateBuffer

	static BOOL		DataStreamCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode, BYTE*& rpSTREAM, DWORD& rdwSize );

	// <WARNING> ���� ��ȯ�� ó���Ѵ�. �����ϼ���~
	// LOADED���¿��� IParser�� �����Ű���� ProcessLoadedData�� ����� ó���ؾ� �Ѵ�.
	static BOOL		DataStatusCommand( eSCRIPT_DATA_CMD cmd, eSCRIPT_CODE scriptCode = SCRIPT_EMPTY, eSCRIPT_DATA_CMD* result = NULL );

public:
	// <CALLBACKs>
	typedef VOID	(*fnAllDataLoaded)();
	// <RegisterCallbackAllDataLoaded>
	//   �� �Լ��� EMPTY, LOADING���°� ���� ��� LOADED���¿� ���� �� ȣ���� �ش�.
	//   �ߵ� ����, Ư�� Script�� LOADED���·� ��ȯ�� �� �˻�ǰ� ȣ��ȴ�.
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

