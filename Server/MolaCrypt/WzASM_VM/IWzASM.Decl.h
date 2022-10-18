#pragma once

#pragma pack(push, 1)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <TYPE DEFINITIONs>
//
#define IWzASM_WH	static const WzASMH
typedef WORD		WzASMH;
typedef PVOID		WzPTR;

#define OFFSETOF(_TYPE, _FIELD)		((WORD)offsetof(_TYPE, _FIELD))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <VM GameResource Function Handler Definition>
class WzGResTable;
union uREG_NODE;

//	<VM REGISTER NODE>
//	<FOR INTER-COMMUNICATION>
union uREG_PV
{
	DWORD			VAL, EX;
	DWORD*			PTRD;
	WzPTR			PTR;
};
//typedef uREG_PV (WzGResTable::*ResFunc)( uREG_PV );
//typedef uREG_PV (WzGResTable::*ResData)();

typedef uREG_PV (*HandlerIBF)( uREG_NODE* );

//	Index of uREGA
struct sARG
{
	enum eARG
	{
		ARG0	=	0,
		EIP		=	ARG0,
		ARG1	=	1,
		ARG2	=	2,
		ARG3	=	3,
		MAXARG	=	4,
	};
};

//	EFL Bit Definition
struct sREG_NODE_BITS
{
	enum eCMP
	{
		RET_ZERO		= 0,
		RET_NOTZERO		= 1,

		BELOW			= 2,
		//BELOW_EQUAL		= 3,
		EQUAL			= 1,
		//ABOVE_EQUAL		= 5,
		ABOVE			= 4,

		CMP_MASK		= (8-1),
	};
	DWORD				TEST_RET	: 3;
	DWORD				B00			: 28;
};




#if WzASM_UPDATE_0x5001

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<uREG> 기본 연산 단위
//	[L][H]
//	[ LX ][ HX ]
//	[    EX    ]
typedef union uREG		// EX형태의 지원 처리
{
	typedef uREG			REG;
	//////////////////////////////////////////////////////////////////////////
	// REGISTER VALUE TYPE
	DWORD				EX; 		// 4
	struct {
		union {	WORD	X, LX;	};	// 2
		WORD			HX;			// 2
	};
	struct { BYTE		L, H;	};	// 1
	struct { BYTE		LXL, LXH, HXL, HXH;	};	// 1
	WzPTR				PTR;		// 4
} uREG, uEX, uEAX, uEBX, uECX, uEDX, uERP, uEOP, uEFP;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<cOFF>	레지스터 옵셋 연산용
typedef struct cOFF
{
	enum eOP {
		NONE	= RGPG011,
		ADD		= RGPG015,	// (*).EX += DT
		SUB		= RGPG040,	// (*).EX -= DT
		CADD	= RGPG047,	// (*).EX += (DT : cOFF2)
		CSUB	= RGPG055,	// (*).EX -= (DT : cOFF2)
	};
	typedef enum eOP	eOP;
	WORD				DT, OP;		// 2, 2
} cOFF;


typedef union uCONV		// EX형태의 지원 처리
{
	//////////////////////////////////////////////////////////////////////////
	uREG::REG			REG;
	//////////////////////////////////////////////////////////////////////////
	// REGISTER VALUE TYPE
	DWORD				EX; 		// 4
	struct {
		union {	WORD	X, LX;	};	// 2
		WORD			HX;			// 2
	};
	struct { BYTE		L, H;	};	// 1
	struct { BYTE		LXL, LXH, HXL, HXH;	};	// 1
	
	//////////////////////////////////////////////////////////////////////////
	// REGISTER POINTER TYPE
	CHAR*				PTRC;		// 4
	BYTE*				PTRB;		// 4
	WORD*				PTRW;		// 4
	DWORD*				PTRD;		// 4
	WzPTR				PTR;
	// REGISTER VALUE OP TYPE
	sREG_NODE_BITS		FLAG;		// 4
	uREG_PV				CONV;		// 4
	cOFF				EOPC;		// 4
	// REGISTER FUNCTION CALL TYPE
	HandlerIBF			FN_IB;		// 4	(MOLA Internal Base Function)
} uCONV;

typedef union uREGA		// EX EXA형태의 지원 처리
{
	typedef uREGA		REGA;
	static const DWORD	ARRAY_CNT = 4;
	//////////////////////////////////////////////////////////////////////////
	uCONV				EXA[ARRAY_CNT];
	struct { uCONV		EX0, EX1, EX2, EX3; };
} uREGA;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<VM General Purpose Register Definition>
union uREG_NODE
{
	static const DWORD	ARRAY_CNT = 4;
	//////////////////////////////////////////////////////////////////////////
	// REGISTER VALUE TYPE
	DWORD				EX; 		// 4
	struct {
		union {	WORD	X, LX;	};	// 2
		WORD			HX;			// 2
	};
	struct { BYTE		L, H;	};	// 1
	struct { BYTE		LXL, LXH, HXL, HXH;	};	// 1
	//////////////////////////////////////////////////////////////////////////
	uCONV				EXA[ARRAY_CNT];
	struct { uCONV		EX0, EX1, EX2, EX3; };
	//////////////////////////////////////////////////////////////////////////
	uREG::REG			REG;
	uREGA::REGA			REGA;
	//////////////////////////////////////////////////////////////////////////
	// REGISTER POINTER TYPE
	CHAR*				PTRC;		// 4
	BYTE*				PTRB;		// 4
	WORD*				PTRW;		// 4
	DWORD*				PTRD;		// 4
	WzPTR				PTR;
	// REGISTER VALUE OP TYPE
	sREG_NODE_BITS		FLAG;		// 4
	uREG_PV				CONV;		// 4
	cOFF				EOPC;		// 4
	// REGISTER FUNCTION CALL TYPE
	HandlerIBF			FN_IB;		// 4	(MOLA Internal Base Function)
};

#if CODE_TEST(WzASM_UPDATE_0x5001_TEST)
inline VOID testtesttestcode()
{
	uREG_NODE NODE;
	NODE.EXA[sARG::ARG0].PTR = 0;
	NODE.EX0.PTR = 0;
}
#endif

#else //WzASM_UPDATE_0x5001


struct cOFF
{
	enum eOP { NONE = RGPG011, ADD = RGPG015, SUB = RGPG040, };
	WORD				DT, OP;		// 2, 2
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<uREG>
//		EXA정의용
union uREG
{
	DWORD				EX;			// 4
	struct {
		union {
			WORD		X;			// 2
			WORD		LX;			// 2
		};
		WORD			HX;			// 2
	};
	struct {
		BYTE			L;			// 1
		BYTE			H;			// 1
	};
	CHAR*				PTRC;		// 4
	BYTE*				PTRB;		// 4
	WORD*				PTRW;		// 4
	DWORD*				PTRD;		// 4
	WzPTR				PTR;
	uREG_PV				CONV;		// 4
	cOFF			EOPC;		// 4
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <VM General Purpose Register Definition>
union uREG_NODE
{
	DWORD				EX;			// 4
	struct {
		union {
			WORD		X;			// 2
			WORD		LX;			// 2
		};
		WORD			HX;			// 2
	};
	struct {
		BYTE			L;			// 1
		BYTE			H;			// 1
	};
	uREG		EXA[4];	// 16
	sREG_NODE_BITS		FLAG;		// 4
	CHAR*				PTRC;		// 4
	BYTE*				PTRB;		// 4
	WORD*				PTRW;		// 4
	DWORD*				PTRD;		// 4
	WzPTR				PTR;
	uREG_PV				CONV;		// 4
	cOFF			EOPC;		// 4
	HandlerIBF			FN_IB;		// 4	(MOLA Internal Base Function)
};

#endif //WzASM_UPDATE_0x5001

// <VM General Purpose Register Block>
struct sREGISTER_BLK
{
	uREG_NODE		EAX;		// 메인 연산 공간
	uREG_NODE		EBX;		// 메인 보조 연산 공간/문자열 처리
	uREG_NODE		ECX;		// 보조 연산 공간/RES_CODE 로드 임시공간
	uREG_NODE		EDX;		// 보조 연산 공간/---ECX로 전환---RES_DATA 로드 임시공간

	uREG_NODE		EBP;		// 현 메소드 스택 베이스 포인터 - 옵셋 연산 목적
	//uREG_NODE		EFL;		// 임시 공간, 이것은 REGISTER's REGISTER성격을 가지고 있으며, 비교문 판단 조건 정보 저장용
	uREG_NODE		ERP;		// 임시 버퍼, 결과값 반환, 이것은 REGISTER's REGISTER성격을 가지고 있으며, 정보 저장용으로 사용해서는 안된다.
	uREG_NODE		EOP;		// 임시 버퍼, 특정 객체를 사용중일 경우, 이 객체의 포인터를 보관한다. 해당 포인터의 옵셋 계산들이 존재한다.
	uREG_NODE		EFP;		// 임시 버퍼, 특정 객체의 함수를 호출할 때 사용한다.

	//uREG_NODE		EIP;		// EIP 객체 저장 포인터
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<EXECUTE RESULT>
//		FAIL, SUCCESS, PENDING
struct sRET
{
	enum eRET
	{
		FAIL, SUCCESS, PENDING, RETURN,
	};
};
typedef sRET::eRET	WzRET_T;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
typedef uREG_NODE (WzGResTable::*GR_HandlerRC)( uREG_NODE* );
//typedef uREG_PV (WzGResTable::*GR_HandlerRD)();

// 변경) 게임 리소스 인터페이스 포맷
//typedef uREG_NODE (WzGResTable::*GResHandler)( uREG_NODE* pARGs );
// 추가) 게임 리소스 인터페이스의 인터페이스(MOLA 내부 포맷)
//		[0]: reserved, [1]: GResHASH, [2]: 
typedef uREG_NODE	(*IGResHandler)( uREG_NODE* pARGs );

// <VM GameResource Function Handler>
struct IWzASM_PROC
{
	DWORD					DATASIZE;
	IGResHandler			RESFUNC;

	IWzASM_PROC() : DATASIZE(0), RESFUNC(NULL) {};
};

#pragma pack(pop)

