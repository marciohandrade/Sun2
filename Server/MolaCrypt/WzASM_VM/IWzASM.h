//	IWzASM.h
#pragma once

#include <Protocol.h>

#include <VMOp/MOLA.chunk_stl.h>
#include "IWzASM.Const.h"
#include "IWzASM.Decl.h"

#include <BBTanStream/BBTanStream.h>

#pragma pack(push, 1)

/*
	// (8) BYTE COMMANDs
	MOV_EAX_HASH		(OP, DATA)	<- EAX에 주어진 HASH값을 넣는다.
	LEA_EDX_EAX		(OP)		<- EAX에 저장된 값을 HASH로 보고 해당 포인터를 찾아 EDX에 저장
	CALL_RD			(OP)		<- EDX에 저장된 포인터로 함수호출 (이건 데이터 값 얻기를 의미), 이후 이 결과는 EAX에 저장된다

	// (15) BYTE COMMONDs
	MOV_EAX_B1		(OP, DATA)	<- EAX에 주어진 1바이트값을 넣는다.
	PUSH_EAX		(OP)		<- EAX값을 스택에 입력
	LEA_ECX_HASH	(OP)		<- 인자로 들어온 HASH에서 해당 포인터를 찾아 ECX에 저장
	CALL_RC			(OP)		<- ECX에 저장된 포인터로 함수호출 (이건 인자입력을 통한 함수 호출 및 데이터 얻기), 이후 이 결과는 EAX에 저장
	MOV_EAX_EAX2		(OP)		<- EAX2 = EAX
	MOV_EAX_B1		(OP, DATA)	<- EAX = B1
	XOR_EAX_EAX2		(OP)		<- EAX ^= EAX2
	RET				(OP)
*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <명령어 처리 임시 저장소>
class WzASM_STACK
{
// <TYPE DEFINITIONs>
private:	typedef STLX_VECTOR<uREG_NODE>			WzASM_STACK_VEC;
			typedef WzASM_STACK_VEC::iterator					WzASM_STACK_VEC_IT;
// <INTERFACEs>
public:		WzASM_STACK() {};
			~WzASM_STACK() {};

			inline VOID			Push( PVOID	ptr )
			{	uREG_NODE pvData;	pvData.CONV.PTR = (DWORD*)ptr;	Push( pvData );	}
			inline VOID			Push( DWORD val )
			{	uREG_NODE pvData;	pvData.CONV.VAL = val;	Push( pvData );	}
			//inline VOID			Push( GR_HandlerRD fn )
			//{	uREG_NODE pvData;	pvData.FN_DT = fn;	Push( pvData );	}
			//inline VOID			Push( GR_HandlerRC fn )
			//{	uREG_NODE pvData;	pvData.FN_FN = fn;	Push( pvData );	}
			inline VOID			Push( uREG_NODE& pvData )
			{	m_STACK.push_back( pvData );	}

			inline uREG_NODE	Pop()
			{
				uREG_NODE& rData = m_STACK.back();
				uREG_NODE pvData = rData;

				ZeroMemory( &rData, sizeof(rData) );
				m_STACK.pop_back();

				return pvData;
			}

			// 최 상단 부터 n번째
			inline uREG_NODE&	Peek( DWORD _SEQ )
			{
				return *(m_STACK.rbegin() + _SEQ);
			}

#ifdef WzASM_UPDATE_0x5001
			inline VOID			OffsetPop( DWORD _SZ )
			{	// 호출자가 함수 호출후 스택 제거 책임을 맡아 처리하기 위한 함수
				for( DWORD i=0 ; i<_SZ ; ++i )
					m_STACK.pop_back();
			}
#endif

			VOID				StoreGPR( sREGISTER_BLK& IN pvBlk );
			VOID				LoadGPR( sREGISTER_BLK& OUT pvBlk );

			inline VOID			Reserve( DWORD reservedSize ) { m_STACK.reserve( reservedSize ); }
// <FIELDs>
public:		WzASM_STACK_VEC		m_STACK;
};
//
//////////////////////////////////////////////////////////////////////////



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WzASM MARK Jump Address Table (JAT)
class WzASM_JAT
{
// <TYPE DEFINITIONs>
private:	struct sJMP_INFO
			{
				OPW_T			MARK;
				WORD			ADDR_IDX;
				sJMP_INFO( OPW_T rMARK, WORD wIDX ) : MARK(rMARK), ADDR_IDX(wIDX) {}
			};
private:	typedef STLX_VECTOR<sJMP_INFO>		JUMP_ADDR;
			typedef JUMP_ADDR::iterator						JUMP_ADDR_IT;

// <INTERFACEs>
public:		WzASM_JAT() {};
			~WzASM_JAT() {};

			inline VOID		Init() { m_JumpAddr.clear(); };
			inline VOID		Add( OPW_T asmCode, WORD wAddrIDX )
			{	m_JumpAddr.push_back( sJMP_INFO( asmCode, wAddrIDX ) );		}
			WORD			FindAddIDX( OPW_T asmCode );
			// <FIELDs>
private:	JUMP_ADDR		m_JumpAddr;
};

//
//////////////////////////////////////////////////////////////////////////

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class WzASM_EIP;
typedef WzRET_T		(*IWzASM_Handler)( WzASM_EIP&, uREG_NODE* );
const WORD			MAX_CMD_STREAM_SIZE		= ((WORD)-1);

struct sWzASM_BLK
{
	WORD			OPCODE;
	BYTE			DATASIZE;
	IWzASM_Handler	HANDLER;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WzASM Instruction Ptr Block
struct sOFFSET_CTRL
{
	BYTE*		PTR;
	WORD		IDX;

	sOFFSET_CTRL() {}
	sOFFSET_CTRL( BYTE* pOFFSET, WORD wOFFSET_IDX )
		: PTR(pOFFSET), IDX(wOFFSET_IDX) {}

	inline VOID		INC( WORD wINC )
	{
		PTR += wINC;
		IDX += wINC;
	}

	inline VOID		DEC( WORD wDEC )
	{
		PTR -= wDEC;
		IDX -= wDEC;
	}

	BOOL			JMP( OPW_T* ptr );
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct sINSTANCE_OP
{
	inline IWzASM_Handler&	HANDLER() { return CODE_INFO.HANDLER; }
	inline BYTE&			DATASIZE() { return CODE_INFO.DATASIZE; }

	//INSTANCE OPERATOR
	sOFFSET_CTRL	OFFSET;
	OPW_T*			ASM_CUR;		// 현재 처리할 ASM CODE
	DWORD			DSZ_CUR;		// 현재 처리할 ASM CODE가 요구하는 연속된 DATA SIZE
	uREG_NODE*		CODE_DATA;		// 현재 처리할 ASM CODE가 요구하는 연속된 DATA PTR - SIZE==0이면 갱신되지 않는다.
	sWzASM_BLK		CODE_INFO;

	sINSTANCE_OP() {}
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WzASM Instruction Ptr
class WzASM_EIP
{
	friend class WzASM_VMCore;
	typedef STLX_VECTOR<BYTE>		CMD_STREAM_VEC;
	typedef CMD_STREAM_VEC::iterator			CMD_STREAM_VEC_IT;

public:		WzASM_EIP() {};
			~WzASM_EIP()
			{
				CMD.assign( CMD.size(), 0 );
			}

			BOOL				Init( BYTE* IN pCmdStream, WORD wSize );

			template<typename _TYPE>
			_TYPE*				InitInstance()
			{
				JAT.Init();
				ZeroMemory( &OP, sizeof(OP) );

				if( MAX_CMD_STREAM_SIZE < sizeof(_TYPE) )
					return NULL;

				CMD.assign( sizeof(_TYPE), 0 );
				new (&CMD[0]) _TYPE;

				ClearOP();

				return (_TYPE*)(&CMD[0]);
			}

			template<typename _TYPE>
			inline _TYPE*		CmdStream() { return (_TYPE*)(&CMD[0]); }
			inline WORD			CmdStreamSize() { return (WORD)CMD.size(); }
			VOID				ClearOP();
			WzRET_T				ExecuteCode();
			BOOL				Prefetch();
			uREG_NODE			Execute();
			uREG_NODE			_Execute();
			OPW_T*			FindAddress( OPW_T asmCode );
			inline VOID			Register_REF( IWzASM_PROC& rHANDLER ) { REF = rHANDLER; }

//////////////////////////////////////////////////////////////////////////
public:		CMD_STREAM_VEC		CMD;
public:		WzASM_JAT			JAT;
			IWzASM_PROC			REF;	// 임시저장된 GameResource Handler Pointer
			sINSTANCE_OP		OP;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct WzASM_VMData
{
	sREGISTER_BLK		REGISTER;
	WzASM_STACK			STACK;
	uREG_NODE			EFL;			// 상태 변수 레지스터
	WzASM_STACK			EIP_STACK;

	WzASM_VMData()
	{
		ZeroMemory( &REGISTER, sizeof(REGISTER) );
		ZeroMemory( &EFL, sizeof(EFL) );
		STACK.Reserve( 128 );
		EIP_STACK.Reserve( 16 );
	}
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 각 WzASM 핸들러들이 참조하는 Game Resource Handler Wrapper
class IWzGResTable
{
private:	WzGResTable*		m_pNode;
public:		IWzGResTable() : m_pNode(NULL) {}
			IWzGResTable( WzGResTable* pNODE ) : m_pNode(pNODE) {}
			//uREG_NODE			Find( WzASMH query );
			//uREG_NODE			QueryByHandlerFN( GR_HandlerRC fn, uREG_NODE* arg );
			////uREG_PV			QueryByHandlerDT( GR_HandlerRD fn );
			//uREG_PV				Query( WzASMH query, uREG_PV arg );
			inline WzGResTable*	REFERENCE() {
				return m_pNode;
			}
#define		CALL_GRES_OBJECT_HANDLER(name, ptrARG)	\
			(VMCORE_INTERFACE.GRES_TABLE().REFERENCE())->name( ptrARG )
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#include "IWzASM.IBF.h"
#include "IWzASM.ResHash.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WzASM Handler 호출
#define CALL_IWzASM_HANDLER( codeASM, dataSIZE )	\
	HANDLER_WzASM_##codeASM( rEIP, pNODE )

#define FNPTR_IBF_HANDLER( name )	\
	HANDLER_WzASM_IBF_##name

#define CALL_IBF_HANDLER( name, arg )	\
	HANDLER_WzASM_IBF_##name( arg )

#undef ADD_IWzASM_HANDLER
#define ADD_IWzASM_HANDLER( codeASM, dataSIZE )	\
	WzRET_T		HANDLER_WzASM_##codeASM( WzASM_EIP& rEIP, uREG_NODE* pNODE );
#	include "IWzASM.DefList.h"
#undef ADD_IWzASM_HANDLER

//

#include "IWzASM.CmdBlk.h"
#include "IWzASM.CmdBlk.Builtin.h"
//////////////////////////////////////////////////////////////////////////


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WzASM Handler Table
// Stack, Register, GameResource
class WzASM_VMCore
{
			friend class WzASM_EIP;
public:		static const DWORD		OPCODE_SIZE	= sizeof(WORD);
public:		typedef STLX_HASH_MAP<DWORD, sWzASM_BLK>			IWzASM_HASH;
			typedef IWzASM_HASH::iterator								IWzASM_HASH_IT;

			typedef STLX_HASH_MAP<DWORD, HandlerIBF>			IBF_HASH_MAP;
			typedef IBF_HASH_MAP::iterator								IBF_HASH_MAP_IT;

			struct sJAT_MATCH
			{
				OPB_T	JMPMRK;
				OPW_T		ASMOP;
			};
			typedef STLX_MAP<OPB_T, sJAT_MATCH*>			JAT_MATCH;
			typedef JAT_MATCH::iterator									JAT_MATCH_IT;
			typedef STLX_MAP<OPW_T, sJAT_MATCH*>			JAT_MATCHW;
			typedef JAT_MATCHW::iterator								JAT_MATCHW_IT;

public:		WzASM_VMCore();
			~WzASM_VMCore();

public:		// <returns> Find Result, 성공시 sWzASM_BLK에 정보가 할당된다.
			WzRET_T				Find_WzASM( OPW_T cWzASM, sWzASM_BLK& OUT rINFO );
			BOOL				CreateEIP( WzASM_EIP*& OUT rpINSTANCE, BYTE* pCmdStream, WORD wSize );
			VOID				DestroyEIP( WzASM_EIP*& IN OUT rpINSTANCE );

public:		VOID				Load( WzGResTable* pREF_Handler );
			VOID				Release();
			inline IWzASM_PROC	FindGResHandlerInfo( WzASMH wHASH )
			{ return m_GResHASH.Find( wHASH ); }

public:		inline IWzASM_HASH&		CODE_TABLE()		{ return m_WzASM_TABLE; }
			inline IBF_HASH_MAP&	IBF_TABLE()			{ return m_IBF_TABLE; }
			inline IWzGResTable&	GRES_TABLE()		{ return m_GResHandler; }
			inline GResHASH&		GRES_HASH()			{ return m_GResHASH; }
			inline JAT_MATCH&		JAT_MATCH_TABLE()	{ return m_JATMatch; }
			inline JAT_MATCHW&		JAT_MATCHW_TABLE()	{ return m_JATWMatch; }
			inline sJAT_MATCH*		JAT_MATCH_ARRAY()	{ return m_JATARRAY; }
private:	// <FIELDs>
			DWORD					m_dwSTART;
			IWzASM_HASH				m_WzASM_TABLE;
			IBF_HASH_MAP			m_IBF_TABLE;
			IWzGResTable			m_GResHandler;
			GResHASH				m_GResHASH;
			JAT_MATCH				m_JATMatch;
			JAT_MATCHW				m_JATWMatch;
			sJAT_MATCH				m_JATARRAY[20];		// 01~19 (19) - [0]은 카운터
			DWORD					m_dwEND;
			// 다음 데이터들은 따로 분리할 필요 있다.
public:		static WzASM_VMCore*			sp_VMCore;
			static WzASM_VMData*			sp_VMData;
#	if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
			static ISecureQueryProcessor*	sp_VMSQP;
#		define VM_SQP			(*WzASM_VMCore::sp_VMSQP)
#	endif

#define VMCORE_INTERFACE		(*WzASM_VMCore::sp_VMCore)
#define VM_REGISTER				((*WzASM_VMCore::sp_VMData).REGISTER)
#define VM_STACK				((*WzASM_VMCore::sp_VMData).STACK)
#define VM_EFL					((*WzASM_VMCore::sp_VMData).EFL)
#define VM_EIP_STACK			((*WzASM_VMCore::sp_VMData).EIP_STACK)


};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 


#pragma pack(pop)




