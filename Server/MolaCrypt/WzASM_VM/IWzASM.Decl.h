#pragma once

#pragma pack(push, 1)
//����������������������������������������������������������������������������
// <TYPE DEFINITIONs>
//
#define IWzASM_WH	static const WzASMH
typedef WORD		WzASMH;
typedef PVOID		WzPTR;

#define OFFSETOF(_TYPE, _FIELD)		((WORD)offsetof(_TYPE, _FIELD))

//����������������������������������������������������������������������������
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

//����������������������������������������������������������������������������
//	<uREG> �⺻ ���� ����
//	[L][H]
//	[ LX ][ HX ]
//	[    EX    ]
typedef union uREG		// EX������ ���� ó��
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

//����������������������������������������������������������������������������
//	<cOFF>	�������� �ɼ� �����
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


typedef union uCONV		// EX������ ���� ó��
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

typedef union uREGA		// EX EXA������ ���� ó��
{
	typedef uREGA		REGA;
	static const DWORD	ARRAY_CNT = 4;
	//////////////////////////////////////////////////////////////////////////
	uCONV				EXA[ARRAY_CNT];
	struct { uCONV		EX0, EX1, EX2, EX3; };
} uREGA;

//����������������������������������������������������������������������������
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

//����������������������������������������������������������������������������
//	<uREG>
//		EXA���ǿ�
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

//����������������������������������������������������������������������������
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
	uREG_NODE		EAX;		// ���� ���� ����
	uREG_NODE		EBX;		// ���� ���� ���� ����/���ڿ� ó��
	uREG_NODE		ECX;		// ���� ���� ����/RES_CODE �ε� �ӽð���
	uREG_NODE		EDX;		// ���� ���� ����/---ECX�� ��ȯ---RES_DATA �ε� �ӽð���

	uREG_NODE		EBP;		// �� �޼ҵ� ���� ���̽� ������ - �ɼ� ���� ����
	//uREG_NODE		EFL;		// �ӽ� ����, �̰��� REGISTER's REGISTER������ ������ ������, �񱳹� �Ǵ� ���� ���� �����
	uREG_NODE		ERP;		// �ӽ� ����, ����� ��ȯ, �̰��� REGISTER's REGISTER������ ������ ������, ���� ��������� ����ؼ��� �ȵȴ�.
	uREG_NODE		EOP;		// �ӽ� ����, Ư�� ��ü�� ������� ���, �� ��ü�� �����͸� �����Ѵ�. �ش� �������� �ɼ� ������ �����Ѵ�.
	uREG_NODE		EFP;		// �ӽ� ����, Ư�� ��ü�� �Լ��� ȣ���� �� ����Ѵ�.

	//uREG_NODE		EIP;		// EIP ��ü ���� ������
};

//����������������������������������������������������������������������������
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

//����������������������������������������������������������������������������
//
typedef uREG_NODE (WzGResTable::*GR_HandlerRC)( uREG_NODE* );
//typedef uREG_PV (WzGResTable::*GR_HandlerRD)();

// ����) ���� ���ҽ� �������̽� ����
//typedef uREG_NODE (WzGResTable::*GResHandler)( uREG_NODE* pARGs );
// �߰�) ���� ���ҽ� �������̽��� �������̽�(MOLA ���� ����)
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

