#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<PROTOTYPEs>
//		- 32BIT
#define OBJCONVERT(v)			( (*(DWORD*)(v)) /*^ MOLA_SECURE_WORKNODE_WALL2*/ )
#define OBJRESTORE(v, obj)		(*(DWORD*)(obj)) = ( ((DWORD)(v)) /*^ MOLA_SECURE_WORKNODE_WALL2*/ )

//	[0]~[63] IWorkNode [64]~ MSGINFO
#pragma pack(push,1)

struct sSECURE_WORKNODE_STRUCTURE
{
	BYTE	m_byObject[62];
	WORD	m_wPacketSize;
	BYTE	m_byRecvPacketStream[64];
	BYTE	m_bySendPacketStream[64];
};


class ISecureQueryCmdStreamBase;
struct sSECURE_WORKNODE_CMDSTREAM
{
	union {
		BYTE							HDR[sizeof(PVOID)];
		DWORD							TYPECODE;
	};
	BYTE								STREAM[252];
};

#pragma pack(pop)

union uSENDINFO
{
	sCLIENT_CRYPT_INFO*		pSendCrypt;
	DWORD					dwConvert;
};


enum eSECURELOGIC
{
	SECURE_QUERY_SIMPLEX_PATTERN01_SNK			= HEX_KEY_1_GENERATOR_001,
	SECURE_QUERY_SIMPLEX_PATTERN02_SNK			= HEX_KEY_1_GENERATOR_002,
	SECURE_QUERY_SIMPLEX_PATTERN03_SNK			= HEX_KEY_1_GENERATOR_003,
	SECURE_QUERY_SIMPLEX_PATTERN04_SNK			= HEX_KEY_1_GENERATOR_004,
	SECURE_QUERY_MOLA							= HEX_KEY_1_GENERATOR_005,
	SECURE_QUERY_CONFIG_MOLA					= HEX_KEY_1_GENERATOR_006,
	SECURE_QUERY_CONFIG_MOLA_HB					= HEX_KEY_1_GENERATOR_007,
	SECURE_QUERY_DUPLEX_SNK						= HEX_KEY_1_GENERATOR_008,
	SECURE_QUERY_RESOURCE_CHK_T1				= HEX_KEY_1_GENERATOR_009,
	SECURE_QUERY_RESOURCE_CHK_T2				= HEX_KEY_1_GENERATOR_010,
	SECURE_QUERY_RESOURCE_CHK_T3				= HEX_KEY_1_GENERATOR_011,
	SECURE_QUERY_RESOURCE_CHK_T4				= HEX_KEY_1_GENERATOR_012,
	SECURE_PATTERNS_MAX							= 16,
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<MODULE VARIABLEs>
//

//
//////////////////////////////////////////////////////////////////////////


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<5000 >


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE CMD CONTROL STRUCTUREs>
class ISecureQueryCmdStreamBase;

#pragma pack(push, 1)

//	서버가 보내는 쿼리 저장 형식
struct sSERVER_QUERY_LINKER
{
	WORD						QUERY_TYPE_KEY;		// eSECURE_QUERY::IDX_T (쿼리 종류)
	WORD						INDEX;				// 쿼리 종류 안에서 인덱스
	ISecureQueryCmdStreamBase*	QUERY_STREAM;
};

struct sRETURN_PTR_SZ
{
	enum eTYPE { VALUE = 0, PTR = 1 };
	WORD						RET_TYPE;
	WORD						RET_SZ;
	union {
		BYTE*					RET_STREAM;
		DWORD					RET_EX;
	};
};

// BuiltinQuery Function
typedef DWORD	(*BuiltinQueryFunc)( DWORD dwInputKey );
extern BuiltinQueryFunc	BUILTIN_QUERY_FUNCs[1024];

struct sBUILTIN_QUERY_NODE
{
	DWORD				INDEX;
	BuiltinQueryFunc	FUNC;
};

//	서버가 보내는 쿼리 인자값 설정 유틸
union uREG_NODE;
typedef uREG_NODE*	PARGB_T;
#define SET_PASSING_ARG( var, arg0, arg1, arg2, arg3 )	\
	{ (var).ARRAY[0].EX = (arg0); (var).ARRAY[1].EX = (arg1); (var).ARRAY[2].EX = (arg2); (var).ARRAY[3].EX = (arg3); }

//
#pragma pack(pop)


typedef STLX_LIST<MOLA_QUERY_MSG>						QUERIED_LIST;
typedef QUERIED_LIST::iterator										QUERIED_LIST_IT;
typedef STLX_HASH_MAP<DWORD/*UserKey*/, QUERIED_LIST>	QUERIED_INFO_MAP;
typedef QUERIED_INFO_MAP::iterator									QUERIED_INFO_MAP_IT;


typedef STLX_VECTOR<sSERVER_QUERY_LINKER>				QUERY_VECTOR;
typedef QUERY_VECTOR::iterator										QUERY_VECTOR_IT;
typedef STLX_MAP<DWORD/*UserKey*/, QUERY_VECTOR>			QUERY_INFO_MAP;
typedef QUERIED_INFO_MAP::iterator									QUERY_INFO_MAP_IT;


typedef STLX_LIST<ISecureQueryCmdStreamBase*>			QUEUED_CMDSTREAM;
typedef QUEUED_CMDSTREAM::iterator									QUEUED_CMDSTREAM_IT;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE CMD CONTROL STRUCTUREs>
class ISecureQueryCmdStreamBase
{
public:		ISecureQueryCmdStreamBase() {}
			virtual ~ISecureQueryCmdStreamBase() { Release(); }

public:		virtual VOID						Release() {}
#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
			virtual VOID						Init() = 0;
			virtual BYTE*						GetCmdStream( PARGB_T pARG, WORD& rwSIZE ) = 0;
			virtual sRETURN_PTR_SZ				ExecQueryNResult() = 0;
#endif //SERVER_CODE

			// 주어진 포인터의 가상함수테이블 타입주소를 통해 새롭게 복사본을 만들어낸다.
			virtual ISecureQueryCmdStreamBase*	GetInstance( BYTE* IN pAllocedPtr ) = 0;
			static ISecureQueryCmdStreamBase*	GetStaticInstance();
};


#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE CMD CONTROL INTERFACEs>
class ISecureQueryProcessor
{
public:		static const DWORD	MOLA_WORK_KEY_LBND = 10000;
public:		ISecureQueryProcessor();
			~ISecureQueryProcessor();
//////////////////////////////////////////////////////////////////////////
// <User에게 Query한 경우 임시 저장 메시지>
public:		inline QUERIED_INFO_MAP&	QueriedTable() { return m_QueriedTable; }
			inline QUERIED_LIST&		GetQueriedList( DWORD dwUserKey ) { return m_QueriedTable[dwUserKey]; }
			QUERIED_LIST_IT				FindMolaQueriedMsg( QUERIED_LIST& IN rLIST, const DWORD dwWorkKey );
			VOID						DeleteUser( DWORD dwUserKey );
			DWORD						RegisterQueriedMsg( MOLA_QUERY_MSG* IN pARG );

//////////////////////////////////////////////////////////////////////////
// <User에게 Query할 메시지>
public:		inline QUERY_INFO_MAP&		QueryTable() { return m_QueryTable; }
			inline QUERY_VECTOR&		GetQueryList( DWORD dwUserKey ) { return m_QueryTable[dwUserKey]; }
			QUERY_VECTOR_IT				FindMolaQueryMsg( QUERY_VECTOR& IN rLIST, const DWORD dwWorkKey );

			sSERVER_QUERY_LINKER&		GetRandomQueryInfo( eSECURE_QUERY::IDX_T queryT );

//////////////////////////////////////////////////////////////////////////
// <UTILITY>
public:		static DWORD				GetNewWorkKey() { return ++ms_dwWORK_KEY_INC; }
			static VOID					RNGKEY_InsertKeys();
			static VOID					RNDKEY_ROLKeys();
			static DWORD				RNDKEY_CALC();
//////////////////////////////////////////////////////////////////////////
// <FIELDs>
private:	static DWORD				ms_dwWORK_KEY_INC;
			static DWORD*				ms_pGLOBAL_RND_KEY;
			QUERY_INFO_MAP				m_QueryTable;
			QUERIED_INFO_MAP			m_QueriedTable;
};
#endif //SERVER_CODE


#if	CLIENT_CODE

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE PACKET CONTROL INTERFACEs>
class ISecurePacketControl
{
public:		ISecurePacketControl() {}
			virtual ~ISecurePacketControl() {}

			//			virtual 
};

#endif //CLIENT_CODE



//
//////////////////////////////////////////////////////////////////////////


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE WORKNODEs>
class ISecureSimplexSNK01 : public ISecureWorkNode
{
public:
	ISecureSimplexSNK01() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureSimplexSNK02 : public ISecureWorkNode
{
public:
	ISecureSimplexSNK02() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureSimplexSNK03 : public ISecureWorkNode
{
public:
	ISecureSimplexSNK03() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureSimplexSNK04 : public ISecureWorkNode
{
public:
	ISecureSimplexSNK04() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureQueryMola : public ISecureWorkNode
{
public:
	ISecureQueryMola() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureConfigureMola : public ISecureWorkNode
{
public:
	ISecureConfigureMola() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureConfigureHeartbeat : public ISecureWorkNode
{
public:
	ISecureConfigureHeartbeat() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};

class ISecureDuplexSNK : public ISecureWorkNode
{
public:
	ISecureDuplexSNK() : ISecureWorkNode() {}
	virtual BOOL	DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize );
};



