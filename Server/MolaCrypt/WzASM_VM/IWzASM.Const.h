#pragma once

#include <Protocol.h>

#ifdef __IWzASM_TEST__
	#include <vector>
	#include <list>
	#include <map>
	#include <hash_map>
	#include <functional>
	#include <algorithm>

	using namespace std;
	using namespace stdext;


	// move from 'ERP-(1B)' to 'AL'
	// move from 'ERP-(2B)' to 'AX'
	// move from 'ERP-(4B)' to 'EAX'
#endif //

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <TYPE DEFINITIONs>
typedef BYTE	OPB_T;
typedef WORD	OPW_T;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM Protocols>
//
#define DECL_CONST_VAL		static const DWORD
#define IWzASM_BB			static const OPB_T
#define DECLS_SUB(name)		struct name { enum e##name
#define DECLE_SUB			}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <HI>
struct OPT
{
	IWzASM_BB	NOP			=	RGPG101;

	//--------------------------------------------------------
	// (020~
	IWzASM_BB	EAX			=	RGPG020;
	IWzASM_BB	EBX			=	RGPG021;
	IWzASM_BB	ECX			=	RGPG022;
	IWzASM_BB	EDX			=	RGPG023;
	IWzASM_BB	EFL			=	RGPG024;		//
	IWzASM_BB	ERP			=	RGPG025;
	//IWzASM_BB	EIP			=	RGPG026;
	IWzASM_BB	EOP			=	RGPG027;
	IWzASM_BB	EFP			=	RGPG028;

	IWzASM_BB	AX			=	RGPG030;
	IWzASM_BB	BX			=	RGPG031;
	IWzASM_BB	CX			=	RGPG032;
	IWzASM_BB	DX			=	RGPG033;
	IWzASM_BB	AL			=	RGPG034;
	IWzASM_BB	BL			=	RGPG035;
	IWzASM_BB	CL			=	RGPG036;
	IWzASM_BB	DL			=	RGPG037;
	IWzASM_BB	AH			=	RGPG038;
	IWzASM_BB	BH			=	RGPG039;
	IWzASM_BB	CH			=	RGPG040;
	IWzASM_BB	DH			=	RGPG041;

	IWzASM_BB	PX			=	RGPG042;		// ERP : X
	IWzASM_BB	PL			=	RGPG043;		// ERP : L

	IWzASM_BB	EAXE		=	RGPG055;		// EAX Extention (ARR4)
	IWzASM_BB	EBXE		=	RGPG056;		// EBX Extention (ARR4)
	IWzASM_BB	ECXE		=	RGPG057;		// ECX Extention (ARR4)
	IWzASM_BB	EDXE		=	RGPG058;		// EDX Extention (ARR4)
	IWzASM_BB	ERPE		=	RGPG059;		// ERP Extention (ARR4)

	IWzASM_BB	EAX0		=	RGPG060;		// EAX0
	IWzASM_BB	EAX1		=	RGPG061;		// EAX1
	IWzASM_BB	EAX2		=	RGPG062;		// EAX2
	IWzASM_BB	EAX3		=	RGPG063;		// EAX3

	IWzASM_BB	EBX0		=	RGPG064;		// EBX0
	IWzASM_BB	EBX1		=	RGPG065;		// EBX1
	IWzASM_BB	EBX2		=	RGPG066;		// EBX2
	IWzASM_BB	EBX3		=	RGPG067;		// EBX3

	IWzASM_BB	ECX0		=	RGPG068;		// ECX0
	IWzASM_BB	ECX1		=	RGPG069;		// ECX1
	IWzASM_BB	ECX2		=	RGPG070;		// ECX2
	IWzASM_BB	ECX3		=	RGPG071;		// ECX3

	IWzASM_BB	EDX0		=	RGPG072;		// ECX0
	IWzASM_BB	EDX1		=	RGPG073;		// ECX1
	IWzASM_BB	EDX2		=	RGPG074;		// ECX2
	IWzASM_BB	EDX3		=	RGPG075;		// ECX3

	IWzASM_BB	ERP0		=	RGPG088;		// ERP0
	IWzASM_BB	ERP1		=	RGPG089;		// ERP1
	IWzASM_BB	ERP2		=	RGPG090;		// ERP2
	IWzASM_BB	ERP3		=	RGPG091;		// ERP3

	IWzASM_BB	EABCDX		=	RGPG099;		//General Purpose Register Instance (EAX, EBX, ECX, EDX)
	IWzASM_BB	GPR			=	RGPG100;		//General Purpose Register

	//--------------------------------------------------------
	// (120~149) CMD STREAM에 명령어에 따른 추가 인자
	IWzASM_BB	HASH		=	RGPG120;		//예약된 주소 (여기서는 HASH)
	IWzASM_BB	B1			=	RGPG121;		//입력값 1BYTE
	IWzASM_BB	B2			=	RGPG122;		//입력값 2BYTE
	IWzASM_BB	B4			=	RGPG123;		//입력값 4BYTE
	IWzASM_BB	PTR			=	RGPG124;		//예약된 Core주소
	IWzASM_BB	OFF			=	RGPG126;		//sCAL(DT,OP)
	IWzASM_BB	HASHB4		=	RGPG127;		//HASH+B4
	IWzASM_BB	REF			=	RGPG128;		//??

	IWzASM_BB	FASTL		=	RGPG129;		//??

	IWzASM_BB	EXA4B1		=	RGPG180;		// EXA~
	IWzASM_BB	EXA4B2		=	RGPG181;		// EXA~
	IWzASM_BB	EXA4B4		=	RGPG182;		// EXA~
	IWzASM_BB	EXA4HS		=	RGPG183;		// EXA~

	IWzASM_BB	EXA3B1		=	RGPG184;		// EXA~
	IWzASM_BB	EXA3B2		=	RGPG185;		// EXA~
	IWzASM_BB	EXA3B4		=	RGPG186;		// EXA~
	IWzASM_BB	EXA3HS		=	RGPG187;		// EXA~

	IWzASM_BB	EXA2B1		=	RGPG188;		// EXA~
	IWzASM_BB	EXA2B2		=	RGPG189;		// EXA~
	IWzASM_BB	EXA2B4		=	RGPG190;		// EXA~
	IWzASM_BB	EXA2HS		=	RGPG191;		// EXA~

	//--------------------------------------------------------
	// <WARNING> MARKx의 값은 OPC::MARKx와 같이 같아야 한다.
	// (234~252)
	IWzASM_BB	MARK01		=	RGPG234;
	IWzASM_BB	MARK02		=	RGPG235;
	IWzASM_BB	MARK03		=	RGPG236;
	IWzASM_BB	MARK04		=	RGPG237;
	IWzASM_BB	MARK05		=	RGPG238;
	IWzASM_BB	MARK06		=	RGPG239;
	IWzASM_BB	MARK07		=	RGPG240;
	IWzASM_BB	MARK08		=	RGPG241;
	IWzASM_BB	MARK09		=	RGPG242;

	IWzASM_BB	MARK10		=	RGPG243;
	IWzASM_BB	MARK11		=	RGPG244;
	IWzASM_BB	MARK12		=	RGPG245;
	IWzASM_BB	MARK13		=	RGPG246;
	IWzASM_BB	MARK14		=	RGPG247;
	IWzASM_BB	MARK15		=	RGPG248;
	IWzASM_BB	MARK16		=	RGPG249;
	IWzASM_BB	MARK17		=	RGPG250;
	IWzASM_BB	MARK18		=	RGPG251;
	IWzASM_BB	MARK19		=	RGPG252;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <LO>
struct OPC
{
	IWzASM_BB	NOP			=	RGPG001;
	//
	IWzASM_BB	PUSH		=	RGPG002;
	IWzASM_BB	POP			=	RGPG003;
	//
	IWzASM_BB	XOR			=	RGPG004;
	IWzASM_BB	AND			=	RGPG005;
	IWzASM_BB	OR			=	RGPG006;
	IWzASM_BB	NOT			=	RGPG007;
	IWzASM_BB	CMPL		=	RGPG008;
	IWzASM_BB	MOD			=	RGPG009;

	IWzASM_BB	XCHG		=	RGPG013;		//WzASM_UPDATE_0x5001
	IWzASM_BB	CMPXCHG		=	RGPG014;		//WzASM_UPDATE_0x5001
	//
	IWzASM_BB	SHR			=	RGPG024;		// BIT SHIFT RIGHT
	IWzASM_BB	SHL			=	RGPG025;		// BIT SHIFT LEFT
	IWzASM_BB	SHRA		=	RGPG026;		// BIT SHIFT RIGHT / EXA(uREG_NODE)
	IWzASM_BB	SHLA		=	RGPG027;		// BIT SHIFT LEFT / EXA(uREG_NODE)
	IWzASM_BB	ROR			=	RGPG028;		// BIT Rotate Right Register EX
	IWzASM_BB	ROL			=	RGPG029;		// BIT Retate Lef Register EX
	IWzASM_BB	RORA		=	RGPG030;		// BIT Rotate Right Register EXA
	IWzASM_BB	ROLA		=	RGPG031;		// BIT Retate Lef Register EXA
	IWzASM_BB	RORB		=	RGPG032;		// EX Rotate Right Register EXA
	IWzASM_BB	ROLB		=	RGPG033;		// EX Retate Lef Register EXA
	//
	IWzASM_BB	INC			=	RGPG038;
	IWzASM_BB	DEC			=	RGPG039;

	IWzASM_BB	RET			=	RGPG046;		// 결과값은 ERP에
	IWzASM_BB	BRK			=	RGPG047;		// 코드 실행 임시 중단
	IWzASM_BB	INTR		=	RGPG048;		// 코드 실행 (임시) 중단
	IWzASM_BB	CLR			=	RGPG049;		// Clear
	IWzASM_BB	CLRA		=	RGPG050;		// Clear All

	IWzASM_BB	LEA_EAX		=	RGPG051;
	IWzASM_BB	LEA_EBX		=	RGPG052;
	IWzASM_BB	LEA_ECX		=	RGPG053;
	IWzASM_BB	LEA_EDX		=	RGPG054;
	IWzASM_BB	LEA_EOP		=	RGPG058;
	IWzASM_BB	LEA_EFP		=	RGPG059;
	IWzASM_BB	LEA_REF		=	RGPG060;

	IWzASM_BB	CALL		=	RGPG061;
	IWzASM_BB	CFFC0		=	RGPG065;		// Core Function Fast Call #0 - ASM COMPLEX CMD
	IWzASM_BB	CFFC1		=	RGPG066;		// Core Function Fast Call #1 - ASM COMPLEX CMD
	IWzASM_BB	CFFC2		=	RGPG067;		// Core Function Fast Call #2 - ASM COMPLEX CMD
	IWzASM_BB	CFFC3		=	RGPG068;		// Core Function Fast Call #3 - ASM COMPLEX CMD

	//

	//
	IWzASM_BB	ADD			=	RGPG081;
	IWzASM_BB	ADD_EBX		=	RGPG082;
	IWzASM_BB	ADD_ECX		=	RGPG083;
	IWzASM_BB	ADD_EDX		=	RGPG084;
	IWzASM_BB	SUB			=	RGPG086;
	IWzASM_BB	SUB_EBX		=	RGPG087;
	IWzASM_BB	SUB_ECX		=	RGPG088;
	IWzASM_BB	SUB_EDX		=	RGPG089;
	IWzASM_BB	MUL			=	RGPG091;
	IWzASM_BB	MUL_ECX		=	RGPG092;
	IWzASM_BB	DIV			=	RGPG094;
	IWzASM_BB	DIV_ECX		=	RGPG095;

	//
	IWzASM_BB	TEST		=	RGPG120;
	IWzASM_BB	CMP			=	RGPG121;
	IWzASM_BB	CMP_ECX		=	RGPG122;

	IWzASM_BB	JMP			=	RGPG123;		// 무조건 점프 (MARKx)
	IWzASM_BB	JZE			=	RGPG124;		// 0이면 점프 (MARKx)
	IWzASM_BB	JNZ			=	RGPG125;		// 0이 아니면 점프 (MARKx)

	IWzASM_BB	JA			=	RGPG126;		// 더 크면 점프 (MARKx)
	IWzASM_BB	JNA			=	RGPG127;		// 더 크지 않으면 점프 (MARKx)
	IWzASM_BB	JB			=	RGPG128;		// 더 작으면 점프 (MARKx)
	IWzASM_BB	JNB			=	RGPG129;		// 더 작지 않으면 점프 (MARKx)
	IWzASM_BB	JE			=	RGPG130;		// 같으면 점프 (MARKx)
	IWzASM_BB	JNE			=	RGPG131;		// 같지 않으면 점프 (MARKx)


	//--------------------------------------------------------
	// (150~
	IWzASM_BB	MOV_EAX		=	RGPG150;
	IWzASM_BB	MOV_EBX		=	RGPG151;
	IWzASM_BB	MOV_ECX		=	RGPG152;
	IWzASM_BB	MOV_EDX		=	RGPG153;
	IWzASM_BB	MOV_ERP		=	RGPG154;
	//IWzASM_BB	MOV_EIP		=	RGPG155;
	IWzASM_BB	MOV_EOP		=	RGPG156;
	IWzASM_BB	MOV_EFP		=	RGPG157;
	//
	//IWzASM_BB	MOV_PTR		=	RGPG154;	// 복구 처리용
	//IWzASM_BB	MOV_REF		=	RGPG155;	// 복구 처리용
	//

	IWzASM_BB	MOV_EAX0	=	RGPG160;
	IWzASM_BB	MOV_EAX1	=	RGPG161;
	IWzASM_BB	MOV_EAX2	=	RGPG162;
	IWzASM_BB	MOV_EAX3	=	RGPG163;

	IWzASM_BB	MOV_ERP0	=	RGPG164;
	IWzASM_BB	MOV_ERP1	=	RGPG165;
	IWzASM_BB	MOV_ERP2	=	RGPG166;
	IWzASM_BB	MOV_ERP3	=	RGPG167;

	IWzASM_BB	MOV_EAXE	=	RGPG168;
	IWzASM_BB	MOV_EBXE	=	RGPG169;
	IWzASM_BB	MOV_ECXE	=	RGPG170;
	IWzASM_BB	MOV_EDXE	=	RGPG171;
	IWzASM_BB	MOV_ERPE	=	RGPG172;

	IWzASM_BB	MOV_EABCDX	=	RGPG173;

	// * = VAL
	IWzASM_BB	MOVP_EAX	=	RGPG180;
	IWzASM_BB	MOVP_EBX	=	RGPG181;
	IWzASM_BB	MOVP_ECX	=	RGPG182;
	IWzASM_BB	MOVP_EDX	=	RGPG183;
	IWzASM_BB	MOVP_ERP	=	RGPG184;
	IWzASM_BB	MOVP_EOP	=	RGPG185;

	// VAL = *
	IWzASM_BB	MOVX_EAX	=	RGPG187;
	IWzASM_BB	MOVX_EBX	=	RGPG188;
	IWzASM_BB	MOVX_ECX	=	RGPG189;
	IWzASM_BB	MOVX_EDX	=	RGPG190;
	IWzASM_BB	MOVX_ERP	=	RGPG191;
	IWzASM_BB	MOVX_EOP	=	RGPG192;

	//--------------------------------------------------------
	// <WARNING> MARKx의 값은 OPC::MARKx와 같이 같아야 한다.
	// (234~252)
	IWzASM_BB	MARK01		=	RGPG234;
	IWzASM_BB	MARK02		=	RGPG235;
	IWzASM_BB	MARK03		=	RGPG236;
	IWzASM_BB	MARK04		=	RGPG237;
	IWzASM_BB	MARK05		=	RGPG238;
	IWzASM_BB	MARK06		=	RGPG239;
	IWzASM_BB	MARK07		=	RGPG240;
	IWzASM_BB	MARK08		=	RGPG241;
	IWzASM_BB	MARK09		=	RGPG242;

	IWzASM_BB	MARK10		=	RGPG243;
	IWzASM_BB	MARK11		=	RGPG244;
	IWzASM_BB	MARK12		=	RGPG245;
	IWzASM_BB	MARK13		=	RGPG246;
	IWzASM_BB	MARK14		=	RGPG247;
	IWzASM_BB	MARK15		=	RGPG248;
	IWzASM_BB	MARK16		=	RGPG249;
	IWzASM_BB	MARK17		=	RGPG250;
	IWzASM_BB	MARK18		=	RGPG251;
	IWzASM_BB	MARK19		=	RGPG252;

};

#if WzASM_UPDATE_0x5001
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<HI><Core Function Fast Call Enumerator
//	<TYPE0>
//		(BASE)	입력되는 인자 사용
//		(BASE)	ERP 수정
//		(OPT)	ETC REGISTERs (BY SPEC.)
//		(OPT)	NO CONTROL STACK
//	<COMMENT FORMAT>
//		(INPUT ARG), (ACTION), (RESULT) // comment
//	<GRAMMER>
//		RES			= RESERVED
//		RET			= RETURNED
//		SRC			= SOURCE
//		DST			= DESTINATION
//		SZ			= SIZE
//		REPCNT		= REPETED COUNT
//		X			= ANY VALUE (NOT USED) - NO_CONTROLLED
//		|			= L|H, LX|HX - SPLITTER
//		|			= (RESULT) -OR-
//		?			= UNEXPECTED - DISTURB
//		iA			= Is Allocated Memory
struct sCFFC0
{
	DECLS_SUB(DEF) { ERP_T=1, ERP_F=0, }; DECLE_SUB;
	
	DECLS_SUB(REPZ_REG_ARG) { RES=0, SRC=1, DST=2, SZ=3, }; DECLE_SUB;
	IWzASM_BB	REPZ_EAX_StoD	= RGPG010;	// EAX={ X, pS, pD, SZ }, COPY, ERP={ 1|0 }
	IWzASM_BB	REPZ_EBX_StoD	= RGPG011;	// EBX={ X, pS, pD, SZ }, COPY, ERP={ 1|0 }
	IWzASM_BB	REPZ_ECX_StoD	= RGPG012;	// ECX={ X, pS, pD, SZ }, COPY, ERP={ 1|0 }
	IWzASM_BB	REPZ_EDX_StoD	= RGPG013;	// EDX={ X, pS, pD, SZ }, COPY, ERP={ 1|0 }
	DECLS_SUB(REPI_REG_ARG) { RES=0, SRC=1, DST=2, MAXSZ=3, }; DECLE_SUB;
	IWzASM_BB	REPI_EAX_StoD	= RGPG014;	// EAX={ X, pS, pD, MAXSZ }, COPY(REPCNT++), ERP={ REPCNT|0 }
	IWzASM_BB	REPI_EBX_StoD	= RGPG015;	// EBX={ X, pS, pD, MAXSZ }, COPY(REPCNT++), ERP={ REPCNT|0 }
	IWzASM_BB	REPI_ECX_StoD	= RGPG016;	// ECX={ X, pS, pD, MAXSZ }, COPY(REPCNT++), ERP={ REPCNT|0 }
	IWzASM_BB	REPI_EDX_StoD	= RGPG017;	// EDX={ X, pS, pD, MAXSZ }, COPY(REPCNT++), ERP={ REPCNT|0 }

	//	<TYPE2>
	DECLS_SUB(MOV_REG_ARG) { NOARG = 0, }; DECLE_SUB;
	IWzASM_BB	MOV_EAX_ARG0	= RGPG021;	// NOARG, EAX = STACK.PEEK(0), ERP={ 1|0 } // 1st ARG
	IWzASM_BB	MOV_EAX_ARG1	= RGPG022;	// NOARG, EAX = STACK.PEEK(1), ERP={ 1|0 } // 2nd ARG
	IWzASM_BB	MOV_EAX_ARG2	= RGPG023;	// NOARG, EAX = STACK.PEEK(2), ERP={ 1|0 } // 3rd ARG
	IWzASM_BB	MOV_EAX_ARG3	= RGPG024;	// NOARG, EAX = STACK.PEEK(3), ERP={ 1|0 } // 4th ARG
	IWzASM_BB	MOV_EBX_ARG0	= RGPG025;	// NOARG, EBX = STACK.PEEK(0), ERP={ 1|0 } // 1st ARG
	IWzASM_BB	MOV_EBX_ARG1	= RGPG026;	// NOARG, EBX = STACK.PEEK(1), ERP={ 1|0 } // 2nd ARG
	IWzASM_BB	MOV_EBX_ARG2	= RGPG027;	// NOARG, EBX = STACK.PEEK(2), ERP={ 1|0 } // 3rd ARG
	IWzASM_BB	MOV_EBX_ARG3	= RGPG028;	// NOARG, EBX = STACK.PEEK(3), ERP={ 1|0 } // 4th ARG
	IWzASM_BB	MOV_ECX_ARG0	= RGPG029;	// NOARG, ECX = STACK.PEEK(0), ERP={ 1|0 } // 1st ARG
	IWzASM_BB	MOV_ECX_ARG1	= RGPG030;	// NOARG, ECX = STACK.PEEK(1), ERP={ 1|0 } // 2nd ARG
	IWzASM_BB	MOV_ECX_ARG2	= RGPG031;	// NOARG, ECX = STACK.PEEK(2), ERP={ 1|0 } // 3rd ARG
	IWzASM_BB	MOV_ECX_ARG3	= RGPG032;	// NOARG, ECX = STACK.PEEK(3), ERP={ 1|0 } // 4th ARG
	IWzASM_BB	MOV_EDX_ARG0	= RGPG033;	// NOARG, EDX = STACK.PEEK(0), ERP={ 1|0 } // 1st ARG
	IWzASM_BB	MOV_EDX_ARG1	= RGPG034;	// NOARG, EDX = STACK.PEEK(1), ERP={ 1|0 } // 2nd ARG
	IWzASM_BB	MOV_EDX_ARG2	= RGPG035;	// NOARG, EDX = STACK.PEEK(2), ERP={ 1|0 } // 3rd ARG
	IWzASM_BB	MOV_EDX_ARG3	= RGPG036;	// NOARG, EDX = STACK.PEEK(3), ERP={ 1|0 } // 4th ARG
	//
	DECLS_SUB(CLR_POP_ARG) { SZ = 1, }; DECLE_SUB;
	IWzASM_BB	CLR_POP_EAX		= RGPG037;	// EAX={ X, SZ }, STACK.OFFSET_POP(SZ), ERP={ 1|0 }
	IWzASM_BB	CLR_POP_EBX		= RGPG038;	// EBX={ X, SZ }, STACK.OFFSET_POP(SZ), ERP={ 1|0 }
	IWzASM_BB	CLR_POP_ECX		= RGPG039;	// ECX={ X, SZ }, STACK.OFFSET_POP(SZ), ERP={ 1|0 }
	IWzASM_BB	CLR_POP_EDX		= RGPG040;	// EDX={ X, SZ }, STACK.OFFSET_POP(SZ), ERP={ 1|0 }


	IWzASM_BB	FIND_CFFC_EAX	= RGPG020;	// EAX={ 0, pS, pD, MAXSZ }, CFFCx, ERP={ REPCNT|0 }
};

// 패킷 제작용으로 생각해 보자.
struct sCFFC1
{	//	{ SZ|CP, PKBODY }
	//	{ PTR, SZ|iA }
	//
	DECLS_SUB(DEF) { ERP_T=1, ERP_F=0, }; DECLE_SUB;
	DECLS_SUB(MKK_ARG) { RET=0, KEY=1 }; DECLE_SUB;			// MAKE KEY
	IWzASM_BB	MKK_B1			= RGPG040;	// EAX={ RES, KEY1|X|X|X }, EAX0 = (B4)MAKE_KEY_TOOL(KEY1), ERP={ 1 }
	IWzASM_BB	MKK_B2			= RGPG040;	// EAX={ RES, KEY1|X }, EAX0 = (B4)MAKE_KEY_TOOL(KEY1), ERP={ 1 }
	IWzASM_BB	MKK_B4			= RGPG040;	// EAX={ RES, KEY1 }, EAX0 = (B4)MAKE_KEY_TOOL(KEY1), ERP={ 1 }
	DECLS_SUB(MKW_ARG) { RET=0, LK=1, HK=2, }; DECLE_SUB;	// MAKE WORD
	IWzASM_BB	MKW_B2			= RGPG040;	// EAX={ RES, B1, B1' }, EAX0 = MAKEWORD(B1,B1'), ERP={ 1 }
	IWzASM_BB	MKW_B4			= RGPG040;	// EAX={ RES, B2, B2' }, EAX0 = MAKEWORD(B2,B2'), ERP={ 1 }
	// IN:EAX, OUT:EDX
	DECLS_SUB(MKS_ARG) { RET=0, RET1=1, HASH=1, K=2, }; DECLE_SUB;	// MAKE HASH+B4
	IWzASM_BB	MKS_HASHB4		= RGPG040;	// EAX={ RES, RES|?, HASH, B4 }, EAX0.LX=HASH|EAX0.HX=LX(B4)|EAX1.LX=HX(B4), ERP={ 1 }
	DECLS_SUB(MK_WRAPPTR_ARG) { RET=0, RET1=1, PTR=1, SZ=2, }; DECLE_SUB;	// MAKE WRAP PTR = { PTR + SZ|iA }
	IWzASM_BB	MK_WRAPPTR		= RGPG040;	// EAX={ RES, PTR, SZ }, EAX={ PTR, SZ, ? }, ERP={ 1 }

	IWzASM_BB	MK_BUILTIN		= RGPG040;	// EAX={ RES, RES, SZ }, EAX={ PTR, SZ, ? }, ERP={ 1 }
};

// EIP Prepare & Prefetch & Execute
struct sCFFC2
{
	IWzASM_BB	EIP_SET_RET		= RGPG010;
	IWzASM_BB	EIP_CLR_OP		= RGPG010;
	IWzASM_BB	EIP_CLR_JAT		= RGPG010;
	IWzASM_BB	EIP_MOVO_IDX	= RGPG010;
	IWzASM_BB	EIP_MOVO_PTR	= RGPG010;
	IWzASM_BB	EIP_MOV_CMDSZ	= RGPG010;
	IWzASM_BB	EIP_ADD_SZ2		= RGPG010;
	IWzASM_BB	EIP_FIND_CMD	= RGPG010;
	IWzASM_BB	EIP_MOV_DTSZ	= RGPG010;
	IWzASM_BB	EIP_ADD_DTSZ	= RGPG010;
	IWzASM_BB	EIP_SWT_JAT		= RGPG010;
	IWzASM_BB	EIP_CALL_CMD	= RGPG010;
	//	<EIP_CTRL_ARG>
	//		VM::EIP : PEEK
	//	<GRAMMER>
	//		X - INSERT ITEM
	//		C - COMMAND TYPE
	DECLS_SUB(EIP_CTRL_ARG)
	{
		CMD_QUERY				= RGPG004,	//[X][0][0][0] PREFIX QUERY <- PTR = (EIP*)->FLD
		CMD_CALL				= RGPG100,	//[X][0][0][0] PREFIX CALL	<- (EIP*)->METHOD( )
		FLD_CMD					= RGPG011,	//[C][X][0][0] EIP::CMD*
		FLD_JAT					= RGPG029,	//[C][X][0][0] EIP::JAT*
		FLD_REF					= RGPG077,	//[C][X][0][0] EIP::REF*
		FLD_IOP					= RGPG100,	//[C][X][0][0] EIP::IOP*
		I_INIT					= RGPG100,	//[1]EIP::Init( BYTE* IN pCmdStream, WORD wSize ) : BOOL
		I_CMDSZ					= RGPG100,	//[1]EIP::CmdStreamSize() : WORD
		I_CLROP					= RGPG100,	//[1]EIP::ClearOP() : VOID
		I_FINDADDR				= RGPG100,	//[1]EIP::FindAddress( OPW_T asmCode ) : OPW_T*
		I_REGREF				= RGPG100,	//[1]EIP::Register_REF( IWzASM_PROC& rHANDLER ) : VOID
	}; DECLE_SUB;	// 
	DECLS_SUB(IOP_CTRL_ARG)	// VM::EIP : PEEK
	{
		CMD_QUERY				= RGPG004,	//[X][0][0][0] PREFIX QUERY <- PTR = (EIP*)->FLD
		CMD_CALL				= RGPG100,	//[X][0][0][0] PREFIX CALL	<- (EIP*)->METHOD( )
		FLD_OFFC				= RGPG011,	//[1]FIELD EIP::CMD*
		FLD_ASM					= RGPG029,	//[1]FIELD EIP::JAT*
		FLD_DTSZ				= RGPG077,	//[1]FIELD EIP::REF*
		FLD_DTPTR				= RGPG080,	//[1]FIELD EIP::IOP*
		FLD_ROASM				= RGPG081,	//(IOP*)->CODE_INFO*
	}; DECLE_SUB;	// MAKE WRAP PTR = { PTR + SZ|iA }
	IWzASM_BB	EIP_CTRL		= RGPG010;	// EAX={X,EIPCMD}, ERP={IOP::FLD}, ERP
	IWzASM_BB	IOP_CTRL		= RGPG010;	// EOP={IOP,DT|X}, ERP={IOP::FLD}, ERP
	IWzASM_BB	IOP_CTRL_EAX	= RGPG010;	// EOP={IOP,DT|X}, ERP={IOP::FLD}, ERP
	IWzASM_BB	EIP_LEA_OFFC	= RGPG010;	// EOP={IOP,DT|X}, ERP={IOP::FLD}, ERP
};
#endif //WzASM_UPDATE_0x5001

#undef IWzASM_BB

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TYPE REINTERPRETER>
typedef struct OPC		OPC;	// OPCODE CMD
typedef struct OPT		OPT;	// OPCODE TYPE

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM PROTOCOL>
struct IWzASM
{
#define IWzASM_WB	static const OPW_T
#undef DECL_WzASM_HASH
#define DECL_WzASM_HASH(val1, val2)		\
			(((val1)&0x03) << 0)	|	\
			(((val2)&0x0C) << 2)	|	\
			(((val2)&0x30) << 4)	|	\
			(((val1)&0xC0) << 6)	|	\
			(((val2)&0x03) << 14)	|	\
			(((val1)&0x0C) << 8)	|	\
			(((val1)&0x30) << 2)	|	\
			(((val2)&0xC0) >> 4)
#define DECL_WzASMCMD( _CMD, _TYPE, DESC )	\
	DECL_WzASM_HASH( _CMD, _TYPE )

	IWzASM_WB	NOP				= DECL_WzASMCMD( OPC::NOP,		OPT::NOP,		"NO-OPERATION" );																				// NEXT OP
	IWzASM_WB	MOV_EAX_HASH	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::HASH,		"EAX = HASH" );																					// EAX = HASH
	IWzASM_WB	MOV_EAX_B1		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::B1,		"EAX = B1" );																					// EAX = B1
	IWzASM_WB	MOV_EAX_B2		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::B2,		"EAX = B2" );																					// EAX = B2
	IWzASM_WB	MOV_EAX_B4		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::B4,		"EAX = B4" );																					// EAX = B4
	IWzASM_WB	MOV_EAX_PTR		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::PTR,		"EAX = PTR" );																					// EAX = PTR
																																												
	IWzASM_WB	MOV_EBX_HASH	= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::HASH,		"EBX = HASH" );																					// EBX = HASH
	IWzASM_WB	MOV_EBX_B1		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::B1,		"EBX = B1" );																					// EBX = B1
	IWzASM_WB	MOV_EBX_B2		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::B2,		"EBX = B2" );																					// EBX = B2
	IWzASM_WB	MOV_EBX_B4		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::B4,		"EBX = B4" );																					// EBX = B4
	IWzASM_WB	MOV_EBX_PTR		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::PTR,		"EAX = PTR" );																					// EAX = PTR
																																												
	IWzASM_WB	MOV_EAX_EBX		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EBX,		"EAX = EBX" );																					// EAX = EBX
	IWzASM_WB	MOV_EAX_ECX		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::ECX,		"EAX = ECX" );																					// EAX = ECX
	IWzASM_WB	MOV_EAX_EDX		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EDX,		"EAX = EDX" );																					// EAX = EDX
																																												
	IWzASM_WB	MOV_EBX_EAX		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::EAX,		"EBX = EAX" );																					// EBX = EAX
	IWzASM_WB	MOV_EBX_ECX		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::ECX,		"EBX = ECX" );																					// EBX = ECX
	IWzASM_WB	MOV_EBX_EDX		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::EDX,		"EBX = EDX" );																					// EBX = EDX
																																												
	IWzASM_WB	MOV_ECX_EAX		= DECL_WzASMCMD( OPC::MOV_ECX,	OPT::EAX,		"ECX = EAX" );																					// ECX = EAX
	IWzASM_WB	MOV_ECX_EBX		= DECL_WzASMCMD( OPC::MOV_ECX,	OPT::EBX,		"ECX = EBX" );																					// ECX = EBX
	IWzASM_WB	MOV_ECX_EDX		= DECL_WzASMCMD( OPC::MOV_ECX,	OPT::EDX,		"ECX = EDX" );																					// ECX = EDX
																																												
	IWzASM_WB	MOV_EDX_EAX		= DECL_WzASMCMD( OPC::MOV_EDX,	OPT::EAX,		"EDX = EAX" );																					// EDX = EAX
	IWzASM_WB	MOV_EDX_EBX		= DECL_WzASMCMD( OPC::MOV_EDX,	OPT::EBX,		"EDX = EBX" );																					// EDX = EBX
	IWzASM_WB	MOV_EDX_ECX		= DECL_WzASMCMD( OPC::MOV_EDX,	OPT::ECX,		"EDX = ECX" );																					// EDX = ECX
																																												
	IWzASM_WB	MOV_EAX_ERP		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::ERP,		"EAX = ERP" );																					// EAX = ERP
	IWzASM_WB	MOV_EBX_ERP		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::ERP,		"EBX = ERP" );																					// EBX = ERP
	IWzASM_WB	MOV_ECX_ERP		= DECL_WzASMCMD( OPC::MOV_ECX,	OPT::ERP,		"ECX = ERP" );																					// ECX = ERP
	IWzASM_WB	MOV_EDX_ERP		= DECL_WzASMCMD( OPC::MOV_EDX,	OPT::ERP,		"EDX = ERP" );																					// EDX = ERP
																																												
	IWzASM_WB	MOV_ERP_EAX		= DECL_WzASMCMD( OPC::MOV_ERP,	OPT::EAX,		"ERP = EAX" );																					// ERP = EAX
	IWzASM_WB	MOV_ERP_EBX		= DECL_WzASMCMD( OPC::MOV_ERP,	OPT::EBX,		"ERP = EBX" );																					// ERP = EBX
	IWzASM_WB	MOV_ERP_ECX		= DECL_WzASMCMD( OPC::MOV_ERP,	OPT::ECX,		"ERP = ECX" );																					// ERP = ECX
	IWzASM_WB	MOV_ERP_EDX		= DECL_WzASMCMD( OPC::MOV_ERP,	OPT::EDX,		"ERP = EDX" );																					// ERP = EDX
																																												
	IWzASM_WB	MOV_EAX0_B1		= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::B1,		"EAX.EXA[0] = B1" );																			// EAX.EXA[0] = B1
	IWzASM_WB	MOV_EAX1_B1		= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::B1,		"EAX.EXA[1] = B1" );																			// EAX.EXA[1] = B1
	IWzASM_WB	MOV_EAX2_B1		= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::B1,		"EAX.EXA[2] = B1" );																			// EAX.EXA[2] = B1
	IWzASM_WB	MOV_EAX3_B1		= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::B1,		"EAX.EXA[3] = B1" );																			// EAX.EXA[3] = B1
																																												
	IWzASM_WB	MOV_EAX0_B2		= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::B2,		"EAX.EXA[0] = B2" );																			// EAX.EXA[0] = B2
	IWzASM_WB	MOV_EAX1_B2		= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::B2,		"EAX.EXA[1] = B2" );																			// EAX.EXA[1] = B2
	IWzASM_WB	MOV_EAX2_B2		= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::B2,		"EAX.EXA[2] = B2" );																			// EAX.EXA[2] = B2
	IWzASM_WB	MOV_EAX3_B2		= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::B2,		"EAX.EXA[3] = B2" );																			// EAX.EXA[3] = B2
																																												
	IWzASM_WB	MOV_EAX0_B4		= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::B4,		"EAX.EXA[0] = B4" );																			// EAX.EXA[0] = B4
	IWzASM_WB	MOV_EAX1_B4		= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::B4,		"EAX.EXA[1] = B4" );																			// EAX.EXA[1] = B4
	IWzASM_WB	MOV_EAX2_B4		= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::B4,		"EAX.EXA[2] = B4" );																			// EAX.EXA[2] = B4
	IWzASM_WB	MOV_EAX3_B4		= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::B4,		"EAX.EXA[3] = B4" );																			// EAX.EXA[3] = B4
																																												
	IWzASM_WB	MOV_EAX0_HASH	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::HASH,		"EAX.EXA[0] = HASH" );																			// EAX.EXA[0] = HASH
	IWzASM_WB	MOV_EAX1_HASH	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::HASH,		"EAX.EXA[1] = HASH" );																			// EAX.EXA[1] = HASH
	IWzASM_WB	MOV_EAX2_HASH	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::HASH,		"EAX.EXA[2] = HASH" );																			// EAX.EXA[2] = HASH
	IWzASM_WB	MOV_EAX3_HASH	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::HASH,		"EAX.EXA[3] = HASH" );																			// EAX.EXA[3] = HASH
																																												
	IWzASM_WB	MOV_EAX0_PTR	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::PTR,		"EAX.EXA[0] = PTR" );																			// EAX.EXA[0] = PTR
	IWzASM_WB	MOV_EAX1_PTR	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::PTR,		"EAX.EXA[1] = PTR" );																			// EAX.EXA[1] = PTR
	IWzASM_WB	MOV_EAX2_PTR	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::PTR,		"EAX.EXA[2] = PTR" );																			// EAX.EXA[2] = PTR
	IWzASM_WB	MOV_EAX3_PTR	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::PTR,		"EAX.EXA[3] = PTR" );																			// EAX.EXA[3] = PTR
																																												
	IWzASM_WB	MOV_EAX_EXA4B1	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA4B1,	"EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = B1, EAX.EXA[3] = B1" );							// EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = B1, EAX.EXA[3] = B1
	IWzASM_WB	MOV_EAX_EXA4B2	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA4B2,	"EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = B2, EAX.EXA[3] = B2" );							// EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = B2, EAX.EXA[3] = B2
	IWzASM_WB	MOV_EAX_EXA4B4	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA4B4,	"EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = B4, EAX.EXA[3] = B4" );							// EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = B4, EAX.EXA[3] = B4
	IWzASM_WB	MOV_EAX_EXA4HS	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA4HS,	"EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = HASH, EAX.EXA[3] = HASH" );					// EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = HASH, EAX.EXA[3] = HASH
																																												
	IWzASM_WB	MOV_EAX_EXA3B1	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA3B1,	"EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = B1, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = B1, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA3B2	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA3B2,	"EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = B2, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = B2, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA3B4	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA3B4,	"EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = B4, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = B4, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA3HS	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA3HS,	"EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = HASH, EAX.EXA[3] = 0" );					// EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = HASH, EAX.EXA[3] = 0
																																												
	IWzASM_WB	MOV_EAX_EXA2B1	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA2B1,	"EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = 0, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B1, EAX.EXA[1] = B1, EAX.EXA[2] = 0, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA2B2	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA2B2,	"EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = 0, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B2, EAX.EXA[1] = B2, EAX.EXA[2] = 0, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA2B4	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA2B4,	"EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = 0, EAX.EXA[3] = 0" );							// EAX.EXA[0] = B4, EAX.EXA[1] = B4, EAX.EXA[2] = 0, EAX.EXA[3] = 0
	IWzASM_WB	MOV_EAX_EXA2HS	= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EXA2HS,	"EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = 0, EAX.EXA[3] = 0" );						// EAX.EXA[0] = HASH, EAX.EXA[1] = HASH, EAX.EXA[2] = 0, EAX.EXA[3] = 0
																																												
	IWzASM_WB	MOV_EAX0_EAX1	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EAX1,		"EAX.EXA[0] = EAX.EXA[1]" );																	// EAX.EXA[0] = EAX.EXA[1]
	IWzASM_WB	MOV_EAX0_EAX2	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EAX2,		"EAX.EXA[0] = EAX.EXA[2]" );																	// EAX.EXA[0] = EAX.EXA[2]
	IWzASM_WB	MOV_EAX0_EAX3	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EAX3,		"EAX.EXA[0] = EAX.EXA[3]" );																	// EAX.EXA[0] = EAX.EXA[3]
																																												
	IWzASM_WB	MOV_EAX0_EBX0	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EBX0,		"EAX.EXA[0] = EBX.EXA[0]" );																	// EAX.EXA[0] = EBX.EXA[0]
	IWzASM_WB	MOV_EAX0_EBX1	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EBX1,		"EAX.EXA[0] = EBX.EXA[1]" );																	// EAX.EXA[0] = EBX.EXA[1]
	IWzASM_WB	MOV_EAX0_EBX2	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EBX2,		"EAX.EXA[0] = EBX.EXA[2]" );																	// EAX.EXA[0] = EBX.EXA[2]
	IWzASM_WB	MOV_EAX0_EBX3	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::EBX3,		"EAX.EXA[0] = EBX.EXA[3]" );																	// EAX.EXA[0] = EBX.EXA[3]
	IWzASM_WB	MOV_EAX0_ERP0	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::ERP0,		"EAX.EXA[0] = ERP.EXA[0]" );																	// EAX.EXA[0] = ERP.EXA[0]
	IWzASM_WB	MOV_EAX0_ERP1	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::ERP1,		"EAX.EXA[0] = ERP.EXA[1]" );																	// EAX.EXA[0] = ERP.EXA[1]
	IWzASM_WB	MOV_EAX0_ERP2	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::ERP2,		"EAX.EXA[0] = ERP.EXA[2]" );																	// EAX.EXA[0] = ERP.EXA[2]
	IWzASM_WB	MOV_EAX0_ERP3	= DECL_WzASMCMD( OPC::MOV_EAX0,	OPT::ERP3,		"EAX.EXA[0] = ERP.EXA[3]" );																	// EAX.EXA[0] = ERP.EXA[3]
																																												
	IWzASM_WB	MOV_EAX1_EAX0	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EAX0,		"EAX.EXA[1] = EAX.EXA[0]" );																	// EAX.EXA[1] = EAX.EXA[0]
	IWzASM_WB	MOV_EAX1_EAX2	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EAX2,		"EAX.EXA[1] = EAX.EXA[2]" );																	// EAX.EXA[1] = EAX.EXA[2]
	IWzASM_WB	MOV_EAX1_EAX3	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EAX3,		"EAX.EXA[1] = EAX.EXA[3]" );																	// EAX.EXA[1] = EAX.EXA[3]
																																												
	IWzASM_WB	MOV_EAX1_EBX0	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EBX0,		"EAX.EXA[1] = EBX.EXA[0]" );																	// EAX.EXA[1] = EBX.EXA[0]
	IWzASM_WB	MOV_EAX1_EBX1	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EBX1,		"EAX.EXA[1] = EBX.EXA[1]" );																	// EAX.EXA[1] = EBX.EXA[1]
	IWzASM_WB	MOV_EAX1_EBX2	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EBX2,		"EAX.EXA[1] = EBX.EXA[2]" );																	// EAX.EXA[1] = EBX.EXA[2]
	IWzASM_WB	MOV_EAX1_EBX3	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::EBX3,		"EAX.EXA[1] = EBX.EXA[3]" );																	// EAX.EXA[1] = EBX.EXA[3]
	IWzASM_WB	MOV_EAX1_ERP0	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::ERP0,		"EAX.EXA[1] = ERP.EXA[0]" );																	// EAX.EXA[1] = ERP.EXA[0]
	IWzASM_WB	MOV_EAX1_ERP1	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::ERP1,		"EAX.EXA[1] = ERP.EXA[1]" );																	// EAX.EXA[1] = ERP.EXA[1]
	IWzASM_WB	MOV_EAX1_ERP2	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::ERP2,		"EAX.EXA[1] = ERP.EXA[2]" );																	// EAX.EXA[1] = ERP.EXA[2]
	IWzASM_WB	MOV_EAX1_ERP3	= DECL_WzASMCMD( OPC::MOV_EAX1,	OPT::ERP3,		"EAX.EXA[1] = ERP.EXA[3]" );																	// EAX.EXA[1] = ERP.EXA[3]
																																												
	IWzASM_WB	MOV_EAX2_EAX0	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EAX0,		"EAX.EXA[2] = EAX.EXA[0]" );																	// EAX.EXA[2] = EAX.EXA[0]
	IWzASM_WB	MOV_EAX2_EAX1	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EAX1,		"EAX.EXA[2] = EAX.EXA[1]" );																	// EAX.EXA[2] = EAX.EXA[1]
	IWzASM_WB	MOV_EAX2_EAX3	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EAX3,		"EAX.EXA[2] = EAX.EXA[3]" );																	// EAX.EXA[2] = EAX.EXA[3]
																																												
	IWzASM_WB	MOV_EAX2_EBX0	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EBX0,		"EAX.EXA[2] = EBX.EXA[0]" );																	// EAX.EXA[2] = EBX.EXA[0]
	IWzASM_WB	MOV_EAX2_EBX1	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EBX1,		"EAX.EXA[2] = EBX.EXA[1]" );																	// EAX.EXA[2] = EBX.EXA[1]
	IWzASM_WB	MOV_EAX2_EBX2	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EBX2,		"EAX.EXA[2] = EBX.EXA[2]" );																	// EAX.EXA[2] = EBX.EXA[2]
	IWzASM_WB	MOV_EAX2_EBX3	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::EBX3,		"EAX.EXA[2] = EBX.EXA[3]" );																	// EAX.EXA[2] = EBX.EXA[3]
	IWzASM_WB	MOV_EAX2_ERP0	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::ERP0,		"EAX.EXA[2] = ERP.EXA[0]" );																	// EAX.EXA[2] = ERP.EXA[0]
	IWzASM_WB	MOV_EAX2_ERP1	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::ERP1,		"EAX.EXA[2] = ERP.EXA[1]" );																	// EAX.EXA[2] = ERP.EXA[1]
	IWzASM_WB	MOV_EAX2_ERP2	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::ERP2,		"EAX.EXA[2] = ERP.EXA[2]" );																	// EAX.EXA[2] = ERP.EXA[2]
	IWzASM_WB	MOV_EAX2_ERP3	= DECL_WzASMCMD( OPC::MOV_EAX2,	OPT::ERP3,		"EAX.EXA[2] = ERP.EXA[3]" );																	// EAX.EXA[2] = ERP.EXA[3]
																																												
	IWzASM_WB	MOV_EAX3_EAX0	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EAX0,		"EAX.EXA[3] = EAX.EXA[0]" );																	// EAX.EXA[3] = EAX.EXA[0]
	IWzASM_WB	MOV_EAX3_EAX1	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EAX1,		"EAX.EXA[3] = EAX.EXA[1]" );																	// EAX.EXA[3] = EAX.EXA[1]
	IWzASM_WB	MOV_EAX3_EAX2	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EAX2,		"EAX.EXA[3] = EAX.EXA[2]" );																	// EAX.EXA[3] = EAX.EXA[2]
																																												
	IWzASM_WB	MOV_EAX3_EBX0	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EBX0,		"EAX.EXA[3] = EBX.EXA[0]" );																	// EAX.EXA[3] = EBX.EXA[0]
	IWzASM_WB	MOV_EAX3_EBX1	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EBX1,		"EAX.EXA[3] = EBX.EXA[1]" );																	// EAX.EXA[3] = EBX.EXA[1]
	IWzASM_WB	MOV_EAX3_EBX2	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EBX2,		"EAX.EXA[3] = EBX.EXA[2]" );																	// EAX.EXA[3] = EBX.EXA[2]
	IWzASM_WB	MOV_EAX3_EBX3	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::EBX3,		"EAX.EXA[3] = EBX.EXA[3]" );																	// EAX.EXA[3] = EBX.EXA[3]
	IWzASM_WB	MOV_EAX3_ERP0	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::ERP0,		"EAX.EXA[3] = ERP.EXA[0]" );																	// EAX.EXA[3] = ERP.EXA[0]
	IWzASM_WB	MOV_EAX3_ERP1	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::ERP1,		"EAX.EXA[3] = ERP.EXA[1]" );																	// EAX.EXA[3] = ERP.EXA[1]
	IWzASM_WB	MOV_EAX3_ERP2	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::ERP2,		"EAX.EXA[3] = ERP.EXA[2]" );																	// EAX.EXA[3] = ERP.EXA[2]
	IWzASM_WB	MOV_EAX3_ERP3	= DECL_WzASMCMD( OPC::MOV_EAX3,	OPT::ERP3,		"EAX.EXA[3] = ERP.EXA[3]" );																	// EAX.EXA[3] = ERP.EXA[3]
																																												
	IWzASM_WB	MOV_ERP0_EAX0	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EAX0,		"EAX.EXA[0] = EAX.EXA[0]" );																	// EAX.EXA[0] = EAX.EXA[0]
	IWzASM_WB	MOV_ERP0_EAX1	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EAX1,		"EAX.EXA[0] = EAX.EXA[1]" );																	// EAX.EXA[0] = EAX.EXA[1]
	IWzASM_WB	MOV_ERP0_EAX2	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EAX2,		"EAX.EXA[0] = EAX.EXA[2]" );																	// EAX.EXA[0] = EAX.EXA[2]
	IWzASM_WB	MOV_ERP0_EAX3	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EAX3,		"EAX.EXA[0] = EAX.EXA[3]" );																	// EAX.EXA[0] = EAX.EXA[3]
	IWzASM_WB	MOV_ERP0_EBX0	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EBX0,		"EAX.EXA[0] = EBX.EXA[0]" );																	// EAX.EXA[0] = EBX.EXA[0]
	IWzASM_WB	MOV_ERP0_EBX1	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EBX1,		"EAX.EXA[0] = EBX.EXA[1]" );																	// EAX.EXA[0] = EBX.EXA[1]
	IWzASM_WB	MOV_ERP0_EBX2	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EBX2,		"EAX.EXA[0] = EBX.EXA[2]" );																	// EAX.EXA[0] = EBX.EXA[2]
	IWzASM_WB	MOV_ERP0_EBX3	= DECL_WzASMCMD( OPC::MOV_ERP0,	OPT::EBX3,		"EAX.EXA[0] = EBX.EXA[3]" );																	// EAX.EXA[0] = EBX.EXA[3]
																																												
	IWzASM_WB	MOV_ERP1_EAX0	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EAX0,		"EAX.EXA[1] = EAX.EXA[0]" );																	// EAX.EXA[1] = EAX.EXA[0]
	IWzASM_WB	MOV_ERP1_EAX1	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EAX1,		"EAX.EXA[1] = EAX.EXA[1]" );																	// EAX.EXA[1] = EAX.EXA[1]
	IWzASM_WB	MOV_ERP1_EAX2	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EAX2,		"EAX.EXA[1] = EAX.EXA[2]" );																	// EAX.EXA[1] = EAX.EXA[2]
	IWzASM_WB	MOV_ERP1_EAX3	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EAX3,		"EAX.EXA[1] = EAX.EXA[3]" );																	// EAX.EXA[1] = EAX.EXA[3]
	IWzASM_WB	MOV_ERP1_EBX0	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EBX0,		"EAX.EXA[1] = EBX.EXA[0]" );																	// EAX.EXA[1] = EBX.EXA[0]
	IWzASM_WB	MOV_ERP1_EBX1	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EBX1,		"EAX.EXA[1] = EBX.EXA[1]" );																	// EAX.EXA[1] = EBX.EXA[1]
	IWzASM_WB	MOV_ERP1_EBX2	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EBX2,		"EAX.EXA[1] = EBX.EXA[2]" );																	// EAX.EXA[1] = EBX.EXA[2]
	IWzASM_WB	MOV_ERP1_EBX3	= DECL_WzASMCMD( OPC::MOV_ERP1,	OPT::EBX3,		"EAX.EXA[1] = EBX.EXA[3]" );																	// EAX.EXA[1] = EBX.EXA[3]
																																												
	IWzASM_WB	MOV_ERP2_EAX0	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EAX0,		"EAX.EXA[2] = EAX.EXA[0]" );																	// EAX.EXA[2] = EAX.EXA[0]
	IWzASM_WB	MOV_ERP2_EAX1	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EAX1,		"EAX.EXA[2] = EAX.EXA[1]" );																	// EAX.EXA[2] = EAX.EXA[1]
	IWzASM_WB	MOV_ERP2_EAX2	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EAX2,		"EAX.EXA[2] = EAX.EXA[2]" );																	// EAX.EXA[2] = EAX.EXA[2]
	IWzASM_WB	MOV_ERP2_EAX3	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EAX3,		"EAX.EXA[2] = EAX.EXA[3]" );																	// EAX.EXA[2] = EAX.EXA[3]
	IWzASM_WB	MOV_ERP2_EBX0	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EBX0,		"EAX.EXA[2] = EBX.EXA[0]" );																	// EAX.EXA[2] = EBX.EXA[0]
	IWzASM_WB	MOV_ERP2_EBX1	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EBX1,		"EAX.EXA[2] = EBX.EXA[1]" );																	// EAX.EXA[2] = EBX.EXA[1]
	IWzASM_WB	MOV_ERP2_EBX2	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EBX2,		"EAX.EXA[2] = EBX.EXA[2]" );																	// EAX.EXA[2] = EBX.EXA[2]
	IWzASM_WB	MOV_ERP2_EBX3	= DECL_WzASMCMD( OPC::MOV_ERP2,	OPT::EBX3,		"EAX.EXA[2] = EBX.EXA[3]" );																	// EAX.EXA[2] = EBX.EXA[3]
																																												
	IWzASM_WB	MOV_ERP3_EAX0	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EAX0,		"EAX.EXA[3] = EAX.EXA[0]" );																	// EAX.EXA[3] = EAX.EXA[0]
	IWzASM_WB	MOV_ERP3_EAX1	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EAX1,		"EAX.EXA[3] = EAX.EXA[1]" );																	// EAX.EXA[3] = EAX.EXA[1]
	IWzASM_WB	MOV_ERP3_EAX2	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EAX2,		"EAX.EXA[3] = EAX.EXA[2]" );																	// EAX.EXA[3] = EAX.EXA[2]
	IWzASM_WB	MOV_ERP3_EAX3	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EAX3,		"EAX.EXA[3] = EAX.EXA[3]" );																	// EAX.EXA[3] = EAX.EXA[3]
	IWzASM_WB	MOV_ERP3_EBX0	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EBX0,		"EAX.EXA[3] = EBX.EXA[0]" );																	// EAX.EXA[3] = EBX.EXA[0]
	IWzASM_WB	MOV_ERP3_EBX1	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EBX1,		"EAX.EXA[3] = EBX.EXA[1]" );																	// EAX.EXA[3] = EBX.EXA[1]
	IWzASM_WB	MOV_ERP3_EBX2	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EBX2,		"EAX.EXA[3] = EBX.EXA[2]" );																	// EAX.EXA[3] = EBX.EXA[2]
	IWzASM_WB	MOV_ERP3_EBX3	= DECL_WzASMCMD( OPC::MOV_ERP3,	OPT::EBX3,		"EAX.EXA[3] = EBX.EXA[3]" );																	// EAX.EXA[3] = EBX.EXA[3]
																																												
	IWzASM_WB	MOV_EAX_EOP		= DECL_WzASMCMD( OPC::MOV_EAX,	OPT::EOP,		"EAX = EOP" );																					// EAX = EOP
	IWzASM_WB	MOV_EBX_EOP		= DECL_WzASMCMD( OPC::MOV_EBX,	OPT::EOP,		"EBX = EOP" );																					// EBX = EOP
	IWzASM_WB	MOV_ECX_EOP		= DECL_WzASMCMD( OPC::MOV_ECX,	OPT::EOP,		"ECX = EOP" );																					// ECX = EOP
	IWzASM_WB	MOV_EDX_EOP		= DECL_WzASMCMD( OPC::MOV_EDX,	OPT::EOP,		"EDX = EOP" );																					// EDX = EOP
	IWzASM_WB	MOV_ERP_EOP		= DECL_WzASMCMD( OPC::MOV_ERP,	OPT::EOP,		"EDX = EOP" );																					// EDX = EOP
	IWzASM_WB	MOV_EOP_EAX		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::EAX,		"EOP = EAX" );																					// EOP = EAX
	IWzASM_WB	MOV_EOP_EBX		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::EBX,		"EOP = EBX" );																					// EOP = EBX
	IWzASM_WB	MOV_EOP_ECX		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::ECX,		"EOP = ECX" );																					// EOP = ECX
	IWzASM_WB	MOV_EOP_EDX		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::EDX,		"EOP = EDX" );																					// EOP = EDX
	IWzASM_WB	MOV_EOP_ERP		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::ERP,		"EOP = ERP" );																					// EOP = ERP
	IWzASM_WB	MOV_EOP_OFF		= DECL_WzASMCMD( OPC::MOV_EOP,	OPT::OFF,		"EOP1 = sCAL(DT, OP)" );																		// EOP1 = sCAL(DT, OP)
																																												
	IWzASM_WB	MOV_EFP_EAX		= DECL_WzASMCMD( OPC::MOV_EFP,	OPT::EAX,		"EFP = EAX" );																					// EFP = EAX
	IWzASM_WB	MOV_EFP_EBX		= DECL_WzASMCMD( OPC::MOV_EFP,	OPT::EBX,		"EFP = EBX" );																					// EFP = EBX
	IWzASM_WB	MOV_EFP_ECX		= DECL_WzASMCMD( OPC::MOV_EFP,	OPT::ECX,		"EFP = ECX" );																					// EFP = ECX
	IWzASM_WB	MOV_EFP_EDX		= DECL_WzASMCMD( OPC::MOV_EFP,	OPT::EDX,		"EFP = EDX" );																					// EFP = EDX
	IWzASM_WB	MOV_EFP_ERP		= DECL_WzASMCMD( OPC::MOV_EFP,	OPT::ERP,		"EFP = ERP" );																					// EFP = ERP
																																												
	IWzASM_WB	MOV_EAXE_EABCDX	= DECL_WzASMCMD( OPC::MOV_EAXE,	OPT::EABCDX,	"MOV_ERPE_EABCDX + MOV_EAX_ERP" );																// MOV_ERPE_EABCDX + MOV_EAX_ERP
	IWzASM_WB	MOV_EBXE_EABCDX	= DECL_WzASMCMD( OPC::MOV_EBXE,	OPT::EABCDX,	"MOV_ERPE_EABCDX + MOV_EBX_ERP" );																// MOV_ERPE_EABCDX + MOV_EBX_ERP
	IWzASM_WB	MOV_ECXE_EABCDX	= DECL_WzASMCMD( OPC::MOV_ECXE,	OPT::EABCDX,	"MOV_ERPE_EABCDX + MOV_ECX_ERP" );																// MOV_ERPE_EABCDX + MOV_ECX_ERP
	IWzASM_WB	MOV_EDXE_EABCDX	= DECL_WzASMCMD( OPC::MOV_EDXE,	OPT::EABCDX,	"MOV_ERPE_EABCDX + MOV_EDX_ERP" );																// MOV_ERPE_EABCDX + MOV_EDX_ERP
	IWzASM_WB	MOV_ERPE_EABCDX	= DECL_WzASMCMD( OPC::MOV_ERPE,	OPT::EABCDX,	"ERP.EXA[0] = EAX.EX, ERP.EXA[1] = EBX.EX, ERP.EXA[2] = ECX.EX, ERP.EXA[3] = EDX.EX" );			// ERP.EXA[0] = EAX.EX, ERP.EXA[1] = EBX.EX, ERP.EXA[2] = ECX.EX, ERP.EXA[3] = EDX.EX
																																												
	IWzASM_WB	MOV_EABCDX_EAXE	= DECL_WzASMCMD( OPC::MOV_EABCDX,	OPT::EAXE,	"MOV_ERP_EAX + MOV_EABCDX_ERPE" );																// MOV_ERP_EAX + MOV_EABCDX_ERPE
	IWzASM_WB	MOV_EABCDX_EBXE	= DECL_WzASMCMD( OPC::MOV_EABCDX,	OPT::EBXE,	"MOV_ERP_EBX + MOV_EABCDX_ERPE" );																// MOV_ERP_EBX + MOV_EABCDX_ERPE
	IWzASM_WB	MOV_EABCDX_ECXE	= DECL_WzASMCMD( OPC::MOV_EABCDX,	OPT::ECXE,	"MOV_ERP_ECX + MOV_EABCDX_ERPE" );																// MOV_ERP_ECX + MOV_EABCDX_ERPE
	IWzASM_WB	MOV_EABCDX_EDXE	= DECL_WzASMCMD( OPC::MOV_EABCDX,	OPT::EDXE,	"MOV_ERP_EDX + MOV_EABCDX_ERPE" );																// MOV_ERP_EDX + MOV_EABCDX_ERPE
	IWzASM_WB	MOV_EABCDX_ERPE	= DECL_WzASMCMD( OPC::MOV_EABCDX,	OPT::ERPE,	"EAX = ERP.EXA[0], EBX = ERP.EXA[1], ECX = ERP.EXA[2], EDX = ERP.EXA[3]" );						// EAX = ERP.EXA[0], EBX = ERP.EXA[1], ECX = ERP.EXA[2], EDX = ERP.EXA[3]
																																												
	IWzASM_WB	MOVP_EAX_B1		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::B1,		"*(BYTE*)EAX = B1" );																			// *(BYTE*)EAX = B1
	IWzASM_WB	MOVP_EAX_B2		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::B2,		"*(WORD*)EAX = B2" );																			// *(WORD*)EAX = B2
	IWzASM_WB	MOVP_EAX_B4		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::B4,		"*(DWORD*)EAX = B4" );																			// *(DWORD*)EAX = B4
	IWzASM_WB	MOVP_EAX_EBX	= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::EBX,		"*(DWORD*)EAX = EBX" );																			// *(DWORD*)EAX = EBX
	IWzASM_WB	MOVP_EAX_BX		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::BX,		"*(WORD*)EAX = BX" );																			// *(WORD*)EAX = BX
	IWzASM_WB	MOVP_EAX_BL		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::BL,		"*(BYTE*)EAX = BL" );																			// *(BYTE*)EAX = BL
	IWzASM_WB	MOVP_EAX_ECX	= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::ECX,		"*(DWORD)EAX = ECX" );																			// *(DWORD)EAX = ECX
	IWzASM_WB	MOVP_EAX_CX		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::CX,		"*(WORD*)EAX = CX" );																			// *(WORD*)EAX = CX
	IWzASM_WB	MOVP_EAX_CL		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::CL,		"*(BYTE*)EAX = CL" );																			// *(BYTE*)EAX = CL
	IWzASM_WB	MOVP_EAX_EDX	= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::EDX,		"*(DWORD*)EAX = EDX" );																			// *(DWORD*)EAX = EDX
	IWzASM_WB	MOVP_EAX_DX		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::DX,		"*(WORD*)EAX = DX" );																			// *(WORD*)EAX = DX
	IWzASM_WB	MOVP_EAX_DL		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::DL,		"*(BYTE*)EAX = DL" );																			// *(BYTE*)EAX = DL
	IWzASM_WB	MOVP_EAX_ERP	= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::ERP,		"*(DWORD*)EAX = ERP" );																			// *(DWORD*)EAX = ERP
	IWzASM_WB	MOVP_EAX_PX		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::PX,		"*(WORD*)EAX = PX" );																			// *(WORD*)EAX = PX
	IWzASM_WB	MOVP_EAX_PL		= DECL_WzASMCMD( OPC::MOVP_EAX,	OPT::PL,		"*(BYTE*)EAX = PL" );																			// *(BYTE*)EAX = PL
																																												
	IWzASM_WB	MOVP_EBX_B1		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::B1,		"*(BYTE*)EBX = B1" );																			// *(BYTE*)EBX = B1
	IWzASM_WB	MOVP_EBX_B2		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::B2,		"*(WORD*)EBX = B2" );																			// *(WORD*)EBX = B2
	IWzASM_WB	MOVP_EBX_B4		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::B4,		"*(DWORD*)EBX = B4" );																			// *(DWORD*)EBX = B4
	IWzASM_WB	MOVP_EBX_EAX	= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::EAX,		"*(DWORD*)EBX = EAX" );																			// *(DWORD*)EBX = EAX
	IWzASM_WB	MOVP_EBX_AX		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::AX,		"*(WORD*)EBX = AX" );																			// *(WORD*)EBX = AX
	IWzASM_WB	MOVP_EBX_AL		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::AL,		"*(BYTE*)EBX = AL" );																			// *(BYTE*)EBX = AL
	IWzASM_WB	MOVP_EBX_ECX	= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::ECX,		"*(DWORD*)EBX = ECX" );																			// *(DWORD*)EBX = ECX
	IWzASM_WB	MOVP_EBX_CX		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::CX,		"*(WORD*)EBX = CX" );																			// *(WORD*)EBX = CX
	IWzASM_WB	MOVP_EBX_CL		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::CL,		"*(BYTE*)EBX = CL" );																			// *(BYTE*)EBX = CL
	IWzASM_WB	MOVP_EBX_EDX	= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::EDX,		"*(DWORD*)EBX = EDX" );																			// *(DWORD*)EBX = EDX
	IWzASM_WB	MOVP_EBX_DX		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::DX,		"*(WORD*)EBX = DX" );																			// *(WORD*)EBX = DX
	IWzASM_WB	MOVP_EBX_DL		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::DL,		"*(BYTE*)EBX = DL" );																			// *(BYTE*)EBX = DL
	IWzASM_WB	MOVP_EBX_ERP	= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::ERP,		"*(DWORD*)EBX = ERP" );																			// *(DWORD*)EBX = ERP
	IWzASM_WB	MOVP_EBX_PX		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::PX,		"*(WORD*)EBX = PX" );																			// *(WORD*)EBX = PX
	IWzASM_WB	MOVP_EBX_PL		= DECL_WzASMCMD( OPC::MOVP_EBX,	OPT::PL,		"*(BYTE*)EBX = PL" );																			// *(BYTE*)EBX = PL
																																												
	IWzASM_WB	MOVP_ECX_B1		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::B1,		"*(BYTE*)ECX = B1" );																			// *(BYTE*)ECX = B1
	IWzASM_WB	MOVP_ECX_B2		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::B2,		"*(WORD*)ECX = B2" );																			// *(WORD*)ECX = B2
	IWzASM_WB	MOVP_ECX_B4		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::B4,		"*(DWORD*)ECX = B4" );																			// *(DWORD*)ECX = B4
	IWzASM_WB	MOVP_ECX_EAX	= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::EAX,		"*(DWORD*)ECX = EAX" );																			// *(DWORD*)ECX = EAX
	IWzASM_WB	MOVP_ECX_AX		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::AX,		"*(WORD*)ECX = AX" );																			// *(WORD*)ECX = AX
	IWzASM_WB	MOVP_ECX_AL		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::AL,		"*(BYTE*)ECX = AL" );																			// *(BYTE*)ECX = AL
	IWzASM_WB	MOVP_ECX_EBX	= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::EBX,		"*(DWORD*)ECX = EBX" );																			// *(DWORD*)ECX = EBX
	IWzASM_WB	MOVP_ECX_BX		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::BX,		"*(WORD*)ECX = BX" );																			// *(WORD*)ECX = BX
	IWzASM_WB	MOVP_ECX_BL		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::BL,		"*(BYTE*)ECX = BL" );																			// *(BYTE*)ECX = BL
	IWzASM_WB	MOVP_ECX_EDX	= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::EDX,		"*(DWORD*)ECX = EDX" );																			// *(DWORD*)ECX = EDX
	IWzASM_WB	MOVP_ECX_DX		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::DX,		"*(WORD*)ECX = DX" );																			// *(WORD*)ECX = DX
	IWzASM_WB	MOVP_ECX_DL		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::DL,		"*(BYTE*)ECX = DL" );																			// *(BYTE*)ECX = DL
	IWzASM_WB	MOVP_ECX_ERP	= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::ERP,		"*(DWORD*)ECX = ERP" );																			// *(DWORD*)ECX = ERP
	IWzASM_WB	MOVP_ECX_PX		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::PX,		"*(WORD*)ECX = PX" );																			// *(WORD*)ECX = PX
	IWzASM_WB	MOVP_ECX_PL		= DECL_WzASMCMD( OPC::MOVP_ECX,	OPT::PL,		"*(BYTE*)ECX = PL" );																			// *(BYTE*)ECX = PL
																																												
	IWzASM_WB	MOVP_EDX_B1		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::B1,		"*(BYTE*)EDX = B1" );																			// *(BYTE*)EDX = B1
	IWzASM_WB	MOVP_EDX_B2		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::B2,		"*(WORD*)EDX = B2" );																			// *(WORD*)EDX = B2
	IWzASM_WB	MOVP_EDX_B4		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::B4,		"*(DWORD*)EDX = B4" );																			// *(DWORD*)EDX = B4
	IWzASM_WB	MOVP_EDX_EAX	= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::EAX,		"*(DWORD*)EDX = EAX" );																			// *(DWORD*)EDX = EAX
	IWzASM_WB	MOVP_EDX_AX		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::AX,		"*(WORD*)EDX = AX" );																			// *(WORD*)EDX = AX
	IWzASM_WB	MOVP_EDX_AL		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::AL,		"*(BYTE*)EDX = AL" );																			// *(BYTE*)EDX = AL
	IWzASM_WB	MOVP_EDX_EBX	= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::EBX,		"*(DWORD*)EDX = EBX" );																			// *(DWORD*)EDX = EBX
	IWzASM_WB	MOVP_EDX_BX		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::BX,		"*(WORD*)EDX = BX" );																			// *(WORD*)EDX = BX
	IWzASM_WB	MOVP_EDX_BL		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::BL,		"*(BYTE*)EDX = BL" );																			// *(BYTE*)EDX = BL
	IWzASM_WB	MOVP_EDX_ECX	= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::ECX,		"*(DWORD*)EDX = ECX" );																			// *(DWORD*)EDX = ECX
	IWzASM_WB	MOVP_EDX_CX		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::CX,		"*(WORD*)EDX = CX" );																			// *(WORD*)EDX = CX
	IWzASM_WB	MOVP_EDX_CL		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::CL,		"*(BYTE*)EDX = CL" );																			// *(BYTE*)EDX = CL
	IWzASM_WB	MOVP_EDX_ERP	= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::ERP,		"*(DWORD*)EDX = ERP" );																			// *(DWORD*)EDX = ERP
	IWzASM_WB	MOVP_EDX_PX		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::PX,		"*(WORD*)EDX = PX" );																			// *(WORD*)EDX = PX
	IWzASM_WB	MOVP_EDX_PL		= DECL_WzASMCMD( OPC::MOVP_EDX,	OPT::PL,		"*(BYTE*)EDX = PL" );																			// *(BYTE*)EDX = PL
																																												
	IWzASM_WB	MOVP_ERP_B1		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::B1,		"*(BYTE*)ERP = B1" );																			// *(BYTE*)ERP = B1
	IWzASM_WB	MOVP_ERP_B2		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::B2,		"*(WORD*)ERP = B2" );																			// *(WORD*)ERP = B2
	IWzASM_WB	MOVP_ERP_B4		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::B4,		"*(DWORD*)ERP = B4" );																			// *(DWORD*)ERP = B4
	IWzASM_WB	MOVP_ERP_EAX	= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::EAX,		"*(DWORD*)ERP = EAX" );																			// *(DWORD*)ERP = EAX
	IWzASM_WB	MOVP_ERP_AX		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::AX,		"*(WORD*)ERP = AX" );																			// *(WORD*)ERP = AX
	IWzASM_WB	MOVP_ERP_AL		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::AL,		"*(BYTE*)ERP = AL" );																			// *(BYTE*)ERP = AL
	IWzASM_WB	MOVP_ERP_EBX	= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::EBX,		"*(DWORD*ERP = EBX" );																			// *(DWORD*ERP = EBX
	IWzASM_WB	MOVP_ERP_BX		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::BX,		"*(WORD*)ERP = BX" );																			// *(WORD*)ERP = BX
	IWzASM_WB	MOVP_ERP_BL		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::BL,		"*(BYTE*)ERP = BL" );																			// *(BYTE*)ERP = BL
	IWzASM_WB	MOVP_ERP_ECX	= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::ECX,		"*(DWORD*)ERP = ECX" );																			// *(DWORD*)ERP = ECX
	IWzASM_WB	MOVP_ERP_CX		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::CX,		"*(WORD*)ERP = CX" );																			// *(WORD*)ERP = CX
	IWzASM_WB	MOVP_ERP_CL		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::CL,		"*(BYTE*)ERP = CL" );																			// *(BYTE*)ERP = CL
	IWzASM_WB	MOVP_ERP_EDX	= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::EDX,		"*(DWORD*)ERP = EDX" );																			// *(DWORD*)ERP = EDX
	IWzASM_WB	MOVP_ERP_DX		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::DX,		"*(WORD*)ERP = DX" );																			// *(WORD*)ERP = DX
	IWzASM_WB	MOVP_ERP_DL		= DECL_WzASMCMD( OPC::MOVP_ERP,	OPT::DL,		"*(BYTE*)ERP = DL" );																			// *(BYTE*)ERP = DL
																																												
	IWzASM_WB	MOVP_EOP_B1		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::B1,		"*(BYTE*)EOP = B1" );																			// *(BYTE*)EOP = B1
	IWzASM_WB	MOVP_EOP_B2		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::B2,		"*(WORD*)EOP = B2" );																			// *(WORD*)EOP = B2
	IWzASM_WB	MOVP_EOP_B4		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::B4,		"*(DWORD*)EOP = B4" );																			// *(DWORD*)EOP = B4
	IWzASM_WB	MOVP_EOP_EAX	= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::EAX,		"*(DWORD*)EOP = EAX" );																			// *(DWORD*)EOP = EAX
	IWzASM_WB	MOVP_EOP_AX		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::AX,		"*(WORD*)EOP = AX" );																			// *(WORD*)EOP = AX
	IWzASM_WB	MOVP_EOP_AL		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::AL,		"*(BYTE*)EOP = AL" );																			// *(BYTE*)EOP = AL
	IWzASM_WB	MOVP_EOP_EBX	= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::EBX,		"*(DWORD*)EOP = EBX" );																			// *(DWORD*)EOP = EBX
	IWzASM_WB	MOVP_EOP_BX		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::BX,		"*(WORD*)EOP = BX" );																			// *(WORD*)EOP = BX
	IWzASM_WB	MOVP_EOP_BL		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::BL,		"*(BYTE*)EOP = BL" );																			// *(BYTE*)EOP = BL
	IWzASM_WB	MOVP_EOP_ECX	= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::ECX,		"*(DWORD*)EOP = ECX" );																			// *(DWORD*)EOP = ECX
	IWzASM_WB	MOVP_EOP_CX		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::CX,		"*(WORD*)EOP = CX" );																			// *(WORD*)EOP = CX
	IWzASM_WB	MOVP_EOP_CL		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::CL,		"*(BYTE*)EOP = CL" );																			// *(BYTE*)EOP = CL
	IWzASM_WB	MOVP_EOP_EDX	= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::EDX,		"*(DWORD*)EOP = EDX" );																			// *(DWORD*)EOP = EDX
	IWzASM_WB	MOVP_EOP_DX		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::DX,		"*(WORD*)EOP = DX" );																			// *(WORD*)EOP = DX
	IWzASM_WB	MOVP_EOP_DL		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::DL,		"*(BYTE*)EOP = DL" );																			// *(BYTE*)EOP = DL
	IWzASM_WB	MOVP_EOP_ERP	= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::ERP,		"*(DWORD*)EOP = ERP" );																			// *(DWORD*)EOP = ERP
	IWzASM_WB	MOVP_EOP_PX		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::PX,		"*(WORD*)EOP = PX" );																			// *(WORD*)EOP = PX
	IWzASM_WB	MOVP_EOP_PL		= DECL_WzASMCMD( OPC::MOVP_EOP,	OPT::PL,		"*(BYTE*)EOP = PL" );																			// *(BYTE*)EOP = PL
																																												
	IWzASM_WB	MOVX_EAX_EAX	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::EAX,		"EAX = *(EAX.PTRD)" );																			// EAX = *(EAX.PTRD)
	IWzASM_WB	MOVX_EAX_AX		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::AX,		"EAX = *(EAX.PTRW)" );																			// EAX = *(EAX.PTRW)
	IWzASM_WB	MOVX_EAX_AL		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::AL,		"EAX = *(EAX.PTRB)" );																			// EAX = *(EAX.PTRB)
	IWzASM_WB	MOVX_EAX_EBX	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::EBX,		"EAX = *(EBX.PTRD)" );																			// EAX = *(EBX.PTRD)
	IWzASM_WB	MOVX_EAX_BX		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::BX,		"EAX = *(EBX.PTRW)" );																			// EAX = *(EBX.PTRW)
	IWzASM_WB	MOVX_EAX_BL		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::BL,		"EAX = *(EBX.PTRB)" );																			// EAX = *(EBX.PTRB)
	IWzASM_WB	MOVX_EAX_ECX	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::ECX,		"EAX = *(ECX.PTRD)" );																			// EAX = *(ECX.PTRD)
	IWzASM_WB	MOVX_EAX_CX		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::CX,		"EAX = *(ECX.PTRW)" );																			// EAX = *(ECX.PTRW)
	IWzASM_WB	MOVX_EAX_CL		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::CL,		"EAX = *(ECX.PTRB)" );																			// EAX = *(ECX.PTRB)
	IWzASM_WB	MOVX_EAX_EDX	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::EDX,		"EAX = *(EDX.PTRD)" );																			// EAX = *(EDX.PTRD)
	IWzASM_WB	MOVX_EAX_DX		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::DX,		"EAX = *(EDX.PTRW)" );																			// EAX = *(EDX.PTRW)
	IWzASM_WB	MOVX_EAX_DL		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::DL,		"EAX = *(EDX.PTRB)" );																			// EAX = *(EDX.PTRB)
	IWzASM_WB	MOVX_EAX_ERP	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::ERP,		"EAX = *(ERP.PTRD)" );																			// EAX = *(ERP.PTRD)
	IWzASM_WB	MOVX_EAX_PX		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::PX,		"EAX = *(ERP.PTRW)" );																			// EAX = *(ERP.PTRW)
	IWzASM_WB	MOVX_EAX_PL		= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::PL,		"EAX = *(ERP.PTRB)" );																			// EAX = *(ERP.PTRB)
	IWzASM_WB	MOVX_EAX_EOP	= DECL_WzASMCMD( OPC::MOVX_EAX,	OPT::EOP,		"EAX = *(EOP.PTRD)	// EOP자체는 객체 포인터" );												// EAX = *(EOP.PTRD)	// EOP자체는 객체 포인터
																																												
	IWzASM_WB	MOVX_EBX_EAX	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::EAX,		"EBX = *(EAX.PTRD)" );																			// EBX = *(EAX.PTRD)
	IWzASM_WB	MOVX_EBX_AX		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::AX,		"EBX = *(EAX.PTRW)" );																			// EBX = *(EAX.PTRW)
	IWzASM_WB	MOVX_EBX_AL		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::AL,		"EBX = *(EAX.PTRB)" );																			// EBX = *(EAX.PTRB)
	IWzASM_WB	MOVX_EBX_EBX	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::EBX,		"EBX = *(EBX.PTRD)" );																			// EBX = *(EBX.PTRD)
	IWzASM_WB	MOVX_EBX_BX		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::BX,		"EBX = *(EBX.PTRW)" );																			// EBX = *(EBX.PTRW)
	IWzASM_WB	MOVX_EBX_BL		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::BL,		"EBX = *(EBX.PTRB)" );																			// EBX = *(EBX.PTRB)
	IWzASM_WB	MOVX_EBX_ECX	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::ECX,		"EBX = *(ECX.PTRD)" );																			// EBX = *(ECX.PTRD)
	IWzASM_WB	MOVX_EBX_CX		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::CX,		"EBX = *(ECX.PTRW)" );																			// EBX = *(ECX.PTRW)
	IWzASM_WB	MOVX_EBX_CL		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::CL,		"EBX = *(ECX.PTRB)" );																			// EBX = *(ECX.PTRB)
	IWzASM_WB	MOVX_EBX_EDX	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::EDX,		"EBX = *(EDX.PTRD)" );																			// EBX = *(EDX.PTRD)
	IWzASM_WB	MOVX_EBX_DX		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::DX,		"EBX = *(EDX.PTRW)" );																			// EBX = *(EDX.PTRW)
	IWzASM_WB	MOVX_EBX_DL		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::DL,		"EBX = *(EDX.PTRB)" );																			// EBX = *(EDX.PTRB)
	IWzASM_WB	MOVX_EBX_ERP	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::ERP,		"EBX = *(ERP.PTRD)" );																			// EBX = *(ERP.PTRD)
	IWzASM_WB	MOVX_EBX_PX		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::PX,		"EBX = *(ERP.PTRW)" );																			// EBX = *(ERP.PTRW)
	IWzASM_WB	MOVX_EBX_PL		= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::PL,		"EBX = *(ERP.PTRB)" );																			// EBX = *(ERP.PTRB)
	IWzASM_WB	MOVX_EBX_EOP	= DECL_WzASMCMD( OPC::MOVX_EBX,	OPT::EOP,		"EBX = *(EOP.PTRD)	// EOP자체는 객체 포인터" );												// EBX = *(EOP.PTRD)	// EOP자체는 객체 포인터
																																												
	IWzASM_WB	MOVX_ECX_EAX	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::EAX,		"ECX = *(EAX.PTRD)" );																			// ECX = *(EAX.PTRD)
	IWzASM_WB	MOVX_ECX_AX		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::AX,		"ECX = *(EAX.PTRW)" );																			// ECX = *(EAX.PTRW)
	IWzASM_WB	MOVX_ECX_AL		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::AL,		"ECX = *(EAX.PTRB)" );																			// ECX = *(EAX.PTRB)
	IWzASM_WB	MOVX_ECX_EBX	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::EBX,		"ECX = *(EBX.PTRD)" );																			// ECX = *(EBX.PTRD)
	IWzASM_WB	MOVX_ECX_BX		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::BX,		"ECX = *(EBX.PTRW)" );																			// ECX = *(EBX.PTRW)
	IWzASM_WB	MOVX_ECX_BL		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::BL,		"ECX = *(EBX.PTRB)" );																			// ECX = *(EBX.PTRB)
	IWzASM_WB	MOVX_ECX_ECX	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::ECX,		"ECX = *(ECX.PTRD)" );																			// ECX = *(ECX.PTRD)
	IWzASM_WB	MOVX_ECX_CX		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::CX,		"ECX = *(ECX.PTRW)" );																			// ECX = *(ECX.PTRW)
	IWzASM_WB	MOVX_ECX_CL		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::CL,		"ECX = *(ECX.PTRB)" );																			// ECX = *(ECX.PTRB)
	IWzASM_WB	MOVX_ECX_EDX	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::EDX,		"ECX = *(EDX.PTRD)" );																			// ECX = *(EDX.PTRD)
	IWzASM_WB	MOVX_ECX_DX		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::DX,		"ECX = *(EDX.PTRW)" );																			// ECX = *(EDX.PTRW)
	IWzASM_WB	MOVX_ECX_DL		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::DL,		"ECX = *(EDX.PTRB)" );																			// ECX = *(EDX.PTRB)
	IWzASM_WB	MOVX_ECX_ERP	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::ERP,		"ECX = *(ERP.PTRD)" );																			// ECX = *(ERP.PTRD)
	IWzASM_WB	MOVX_ECX_PX		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::PX,		"ECX = *(ERP.PTRW)" );																			// ECX = *(ERP.PTRW)
	IWzASM_WB	MOVX_ECX_PL		= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::PL,		"ECX = *(ERP.PTRB)" );																			// ECX = *(ERP.PTRB)
	IWzASM_WB	MOVX_ECX_EOP	= DECL_WzASMCMD( OPC::MOVX_ECX,	OPT::EOP,		"ECX = *(EOP.PTRD)	// EOP자체는 객체 포인터" );												// ECX = *(EOP.PTRD)	// EOP자체는 객체 포인터
																																												
	IWzASM_WB	MOVX_EDX_EAX	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::EAX,		"EDX = *(EAX.PTRD)" );																			// EDX = *(EAX.PTRD)
	IWzASM_WB	MOVX_EDX_AX		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::AX,		"EDX = *(EAX.PTRW)" );																			// EDX = *(EAX.PTRW)
	IWzASM_WB	MOVX_EDX_AL		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::AL,		"EDX = *(EAX.PTRB)" );																			// EDX = *(EAX.PTRB)
	IWzASM_WB	MOVX_EDX_EBX	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::EBX,		"EDX = *(EBX.PTRD)" );																			// EDX = *(EBX.PTRD)
	IWzASM_WB	MOVX_EDX_BX		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::BX,		"EDX = *(EBX.PTRW)" );																			// EDX = *(EBX.PTRW)
	IWzASM_WB	MOVX_EDX_BL		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::BL,		"EDX = *(EBX.PTRB)" );																			// EDX = *(EBX.PTRB)
	IWzASM_WB	MOVX_EDX_ECX	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::ECX,		"EDX = *(ECX.PTRD)" );																			// EDX = *(ECX.PTRD)
	IWzASM_WB	MOVX_EDX_CX		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::CX,		"EDX = *(ECX.PTRW)" );																			// EDX = *(ECX.PTRW)
	IWzASM_WB	MOVX_EDX_CL		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::CL,		"EDX = *(ECX.PTRB)" );																			// EDX = *(ECX.PTRB)
	IWzASM_WB	MOVX_EDX_EDX	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::EDX,		"EDX = *(EDX.PTRD)" );																			// EDX = *(EDX.PTRD)
	IWzASM_WB	MOVX_EDX_DX		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::DX,		"EDX = *(EDX.PTRW)" );																			// EDX = *(EDX.PTRW)
	IWzASM_WB	MOVX_EDX_DL		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::DL,		"EDX = *(EDX.PTRB)" );																			// EDX = *(EDX.PTRB)
	IWzASM_WB	MOVX_EDX_ERP	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::ERP,		"EDX = *(ERP.PTRD)" );																			// EDX = *(ERP.PTRD)
	IWzASM_WB	MOVX_EDX_PX		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::PX,		"EDX = *(ERP.PTRW)" );																			// EDX = *(ERP.PTRW)
	IWzASM_WB	MOVX_EDX_PL		= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::PL,		"EDX = *(ERP.PTRB)" );																			// EDX = *(ERP.PTRB)
	IWzASM_WB	MOVX_EDX_EOP	= DECL_WzASMCMD( OPC::MOVX_EDX,	OPT::EOP,		"EDX = *(EOP.PTRD)	// EOP자체는 객체 포인터" );												// EDX = *(EOP.PTRD)	// EOP자체는 객체 포인터
																																												
	IWzASM_WB	PUSH_EAX		= DECL_WzASMCMD( OPC::PUSH,		OPT::EAX,		"PUSH STACK EAX" );																				// PUSH STACK EAX
	IWzASM_WB	PUSH_EBX		= DECL_WzASMCMD( OPC::PUSH,		OPT::EBX,		"PUSH STACK EBX" );																				// PUSH STACK EBX
	IWzASM_WB	PUSH_ECX		= DECL_WzASMCMD( OPC::PUSH,		OPT::ECX,		"PUSH STACK ECX" );																				// PUSH STACK ECX
	IWzASM_WB	PUSH_EDX		= DECL_WzASMCMD( OPC::PUSH,		OPT::EDX,		"PUSH STACK EDX" );																				// PUSH STACK EDX
	IWzASM_WB	PUSH_ERP		= DECL_WzASMCMD( OPC::PUSH,		OPT::ERP,		"PUSH STACK ERP" );																				// PUSH STACK ERP
	IWzASM_WB	PUSH_EFP		= DECL_WzASMCMD( OPC::PUSH,		OPT::EFP,		"PUSH STACK EFP" );																				// PUSH STACK EFP
	IWzASM_WB	PUSH_EOP		= DECL_WzASMCMD( OPC::PUSH,		OPT::EOP,		"PUSH STACK EOP" );																				// PUSH STACK EOP
																																												
	IWzASM_WB	PUSH_GPR		= DECL_WzASMCMD( OPC::PUSH,		OPT::GPR,		"PUSH STACK GPR" );																				// PUSH STACK GPR
	IWzASM_WB	PUSH_EABCDX		= DECL_WzASMCMD( OPC::PUSH,		OPT::EABCDX,	"PUSH STACK GPR (EAX, EBX, ECX, EDX)" );														// PUSH STACK GPR (EAX, EBX, ECX, EDX)
																																												
	IWzASM_WB	PUSH_B1			= DECL_WzASMCMD( OPC::PUSH,		OPT::B1,		"SPCMD PUSH STACK B1" );																		// SPCMD PUSH STACK B1
	IWzASM_WB	PUSH_B2			= DECL_WzASMCMD( OPC::PUSH,		OPT::B2,		"SPCMD PUSH STACK B2" );																		// SPCMD PUSH STACK B2
	IWzASM_WB	PUSH_B4			= DECL_WzASMCMD( OPC::PUSH,		OPT::B4,		"SPCMD PUSH STACK B4" );																		// SPCMD PUSH STACK B4
	IWzASM_WB	PUSH_HASH		= DECL_WzASMCMD( OPC::PUSH,		OPT::HASH,		"SPCMD PUSH STACK HASH" );																		// SPCMD PUSH STACK HASH
																																												
	IWzASM_WB	POP_EAX			= DECL_WzASMCMD( OPC::POP,		OPT::EAX,		"POP STACK EAX" );																				// POP STACK EAX
	IWzASM_WB	POP_EBX			= DECL_WzASMCMD( OPC::POP,		OPT::EBX,		"POP STACK EBX" );																				// POP STACK EBX
	IWzASM_WB	POP_ECX			= DECL_WzASMCMD( OPC::POP,		OPT::ECX,		"POP STACK ECX" );																				// POP STACK ECX
	IWzASM_WB	POP_EDX			= DECL_WzASMCMD( OPC::POP,		OPT::EDX,		"POP STACK EDX" );																				// POP STACK EDX
	IWzASM_WB	POP_ERP			= DECL_WzASMCMD( OPC::POP,		OPT::ERP,		"POP STACK ERP" );																				// POP STACK ERP
	IWzASM_WB	POP_EFP			= DECL_WzASMCMD( OPC::POP,		OPT::EFP,		"POP STACK EFP" );																				// POP STACK EFP
	IWzASM_WB	POP_EOP			= DECL_WzASMCMD( OPC::POP,		OPT::EOP,		"POP STACK EOP" );																				// POP STACK EOP
																																												
	IWzASM_WB	POP_GPR			= DECL_WzASMCMD( OPC::POP,		OPT::GPR,		"POP STACK GPR" );																				// POP STACK GPR
	IWzASM_WB	POP_EABCDX		= DECL_WzASMCMD( OPC::POP,		OPT::EABCDX,	"PUSH STACK GPR (EDX, ECX, EBX, EAX)" );														// PUSH STACK GPR (EDX, ECX, EBX, EAX)
																																												
	// 피연산 대상 및 결과 공간은 EAX																																			 
	IWzASM_WB	CLR_EAX			= DECL_WzASMCMD( OPC::CLR,		OPT::EAX,		"CLEAR EAX - EX" );																				// CLEAR EAX - EX
	IWzASM_WB	CLR_EBX			= DECL_WzASMCMD( OPC::CLR,		OPT::EBX,		"CLEAR EBX - EX" );																				// CLEAR EBX - EX
	IWzASM_WB	CLR_ECX			= DECL_WzASMCMD( OPC::CLR,		OPT::ECX,		"CLEAR ECX - EX" );																				// CLEAR ECX - EX
	IWzASM_WB	CLR_EDX			= DECL_WzASMCMD( OPC::CLR,		OPT::EDX,		"CLEAR EDX - EX" );																				// CLEAR EDX - EX
	IWzASM_WB	CLR_ERP			= DECL_WzASMCMD( OPC::CLR,		OPT::ERP,		"CLEAR ERP - EX" );																				// CLEAR ERP - EX
	IWzASM_WB	CLR_EOP			= DECL_WzASMCMD( OPC::CLR,		OPT::EOP,		"CLEAR EOP - uREG_NODE" );																		// CLEAR EOP - uREG_NODE
	IWzASM_WB	CLR_EABCDX		= DECL_WzASMCMD( OPC::CLR,		OPT::EABCDX,	"CLEAR (EAX, EBX, ECX, EDX)" );																	// CLEAR (EAX, EBX, ECX, EDX)
																																												
	IWzASM_WB	CLRA_EAX		= DECL_WzASMCMD( OPC::CLRA,		OPT::EAX,		"CLEAR EAX - ARR" );																			// CLEAR EAX - ARR
	IWzASM_WB	CLRA_EBX		= DECL_WzASMCMD( OPC::CLRA,		OPT::EBX,		"CLEAR EBX - ARR" );																			// CLEAR EBX - ARR
	IWzASM_WB	CLRA_ECX		= DECL_WzASMCMD( OPC::CLRA,		OPT::ECX,		"CLEAR ECX - ARR" );																			// CLEAR ECX - ARR
	IWzASM_WB	CLRA_EDX		= DECL_WzASMCMD( OPC::CLRA,		OPT::EDX,		"CLEAR EDX - ARR" );																			// CLEAR EDX - ARR
	IWzASM_WB	CLRA_ERP		= DECL_WzASMCMD( OPC::CLRA,		OPT::ERP,		"CLEAR ERP - ARR" );																			// CLEAR ERP - ARR
	IWzASM_WB	CLRA_EOP		= DECL_WzASMCMD( OPC::CLRA,		OPT::EOP,		"CLEAR EOP - uREG_NODE" );																		// CLEAR EOP - uREG_NODE
	IWzASM_WB	CLRA_EABCDX		= DECL_WzASMCMD( OPC::CLRA,		OPT::EABCDX,	"CLEAR (EAX, EBX, ECX, EDX) - ARR" );															// CLEAR (EAX, EBX, ECX, EDX) - ARR
																																												
	IWzASM_WB	XOR_EAX_EAX		= DECL_WzASMCMD( OPC::XOR,		OPT::EAX,		"EAX ^= EAX" );																					// EAX ^= EAX
	IWzASM_WB	XOR_EAX_EBX		= DECL_WzASMCMD( OPC::XOR,		OPT::EBX,		"EAX ^= EBX" );																					// EAX ^= EBX
	IWzASM_WB	XOR_EAX_ECX		= DECL_WzASMCMD( OPC::XOR,		OPT::ECX,		"EAX ^= ECX" );																					// EAX ^= ECX
	IWzASM_WB	XOR_EAX_EDX		= DECL_WzASMCMD( OPC::XOR,		OPT::EDX,		"EAX ^= EDX" );																					// EAX ^= EDX
	IWzASM_WB	AND_EAX_EBX		= DECL_WzASMCMD( OPC::AND,		OPT::EBX,		"EAX &= EBX" );																					// EAX &= EBX
	IWzASM_WB	AND_EAX_ECX		= DECL_WzASMCMD( OPC::AND,		OPT::ECX,		"EAX &= ECX" );																					// EAX &= ECX
	IWzASM_WB	AND_EAX_EDX		= DECL_WzASMCMD( OPC::AND,		OPT::EDX,		"EAX &= EDX" );																					// EAX &= EDX
	IWzASM_WB	OR_EAX_EBX		= DECL_WzASMCMD( OPC::OR,		OPT::EBX,		"EAX |= EBX" );																					// EAX |= EBX
	IWzASM_WB	OR_EAX_ECX		= DECL_WzASMCMD( OPC::OR,		OPT::ECX,		"EAX |= ECX" );																					// EAX |= ECX
	IWzASM_WB	OR_EAX_EDX		= DECL_WzASMCMD( OPC::OR,		OPT::EDX,		"EAX |= EDX" );																					// EAX |= EDX
	IWzASM_WB	NOT_EAX_EAX		= DECL_WzASMCMD( OPC::NOT,		OPT::EAX,		"EAX = !EAX" );																					// EAX = !EAX
	IWzASM_WB	NOT_EAX_EBX		= DECL_WzASMCMD( OPC::NOT,		OPT::EBX,		"EAX = !EBX" );																					// EAX = !EBX
	IWzASM_WB	NOT_EAX_ECX		= DECL_WzASMCMD( OPC::NOT,		OPT::ECX,		"EAX = !ECX" );																					// EAX = !ECX
	IWzASM_WB	NOT_EAX_EDX		= DECL_WzASMCMD( OPC::NOT,		OPT::EDX,		"EAX = !EDX" );																					// EAX = !EDX
	IWzASM_WB	CMPL_EAX_EAX	= DECL_WzASMCMD( OPC::CMPL,		OPT::EAX,		"EAX ~= EAX" );																					// EAX ~= EAX
	IWzASM_WB	CMPL_EAX_EBX	= DECL_WzASMCMD( OPC::CMPL,		OPT::EBX,		"EAX ~= EBX" );																					// EAX ~= EBX
	IWzASM_WB	CMPL_EAX_ECX	= DECL_WzASMCMD( OPC::CMPL,		OPT::ECX,		"EAX ~= ECX" );																					// EAX ~= ECX
	IWzASM_WB	CMPL_EAX_EDX	= DECL_WzASMCMD( OPC::CMPL,		OPT::EDX,		"EAX ~= EDX" );																					// EAX ~= EDX
																																												
	IWzASM_WB	SHR_EAX_B1		= DECL_WzASMCMD( OPC::SHR,		OPT::B1,		"EAX>>=B1" );																					// EAX>>=B1
	IWzASM_WB	SHR_EAX_EBX		= DECL_WzASMCMD( OPC::SHR,		OPT::EBX,		"EAX>>=EBX" );																					// EAX>>=EBX
	IWzASM_WB	SHR_EAX_ECX		= DECL_WzASMCMD( OPC::SHR,		OPT::ECX,		"EAX>>=ECX" );																					// EAX>>=ECX
	IWzASM_WB	SHR_EAX_EDX		= DECL_WzASMCMD( OPC::SHR,		OPT::EDX,		"EAX>>=EDX" );																					// EAX>>=EDX
	IWzASM_WB	SHL_EAX_B1		= DECL_WzASMCMD( OPC::SHL,		OPT::B1,		"EAX<<=B1" );																					// EAX<<=B1
	IWzASM_WB	SHL_EAX_EBX		= DECL_WzASMCMD( OPC::SHL,		OPT::EBX,		"EAX<<=EBX" );																					// EAX<<=EBX
	IWzASM_WB	SHL_EAX_ECX		= DECL_WzASMCMD( OPC::SHL,		OPT::ECX,		"EAX<<=ECX" );																					// EAX<<=ECX
	IWzASM_WB	SHL_EAX_EDX		= DECL_WzASMCMD( OPC::SHL,		OPT::EDX,		"EAX<<=EDX" );																					// EAX<<=EDX
																																												
	IWzASM_WB	RORB_EAX		= DECL_WzASMCMD( OPC::RORB,		OPT::EAX,		"EAX[(n+1)%4] = EAX[(n)%4]" );																	// EAX[(n+1)%4] = EAX[(n)%4]
	IWzASM_WB	RORB_EBX		= DECL_WzASMCMD( OPC::RORB,		OPT::EBX,		"EBX[(n+1)%4] = EBX[(n)%4]" );																	// EBX[(n+1)%4] = EBX[(n)%4]
	IWzASM_WB	RORB_ECX		= DECL_WzASMCMD( OPC::RORB,		OPT::ECX,		"ECX[(n+1)%4] = ECX[(n)%4]" );																	// ECX[(n+1)%4] = ECX[(n)%4]
	IWzASM_WB	RORB_EDX		= DECL_WzASMCMD( OPC::RORB,		OPT::EDX,		"EDX[(n+1)%4] = EDX[(n)%4]" );																	// EDX[(n+1)%4] = EDX[(n)%4]
																																												
	IWzASM_WB	ROLB_EAX		= DECL_WzASMCMD( OPC::ROLB,		OPT::EAX,		"EAX[(n+3)%4] = EAX[(n)%4]" );																	// EAX[(n+3)%4] = EAX[(n)%4]
	IWzASM_WB	ROLB_EBX		= DECL_WzASMCMD( OPC::ROLB,		OPT::EBX,		"EBX[(n+3)%4] = EBX[(n)%4]" );																	// EBX[(n+3)%4] = EBX[(n)%4]
	IWzASM_WB	ROLB_ECX		= DECL_WzASMCMD( OPC::ROLB,		OPT::ECX,		"ECX[(n+3)%4] = ECX[(n)%4]" );																	// ECX[(n+3)%4] = ECX[(n)%4]
	IWzASM_WB	ROLB_EDX		= DECL_WzASMCMD( OPC::ROLB,		OPT::EDX,		"EDX[(n+3)%4] = EDX[(n)%4]" );																	// EDX[(n+3)%4] = EDX[(n)%4]
																																												
	IWzASM_WB	ADD_EAX			= DECL_WzASMCMD( OPC::ADD,		OPT::EAX,		"EAX += EAX" );																					// EAX += EAX
	IWzASM_WB	ADD_EAX_EBX		= DECL_WzASMCMD( OPC::ADD,		OPT::EBX,		"EAX += EBX" );																					// EAX += EBX
	IWzASM_WB	ADD_EAX_B1		= DECL_WzASMCMD( OPC::ADD,		OPT::B1,		"EAX += B1" );																					// EAX += B1
	IWzASM_WB	ADD_EAX_B2		= DECL_WzASMCMD( OPC::ADD,		OPT::B2,		"EAX += B2" );																					// EAX += B2
	IWzASM_WB	ADD_EAX_B4		= DECL_WzASMCMD( OPC::ADD,		OPT::B4,		"EAX += B4" );																					// EAX += B4
																																												
	IWzASM_WB	ADD_EBX_EAX		= DECL_WzASMCMD( OPC::ADD_EBX,	OPT::EAX,		"EBX += EAX" );																					// EBX += EAX
	IWzASM_WB	ADD_EBX_B1		= DECL_WzASMCMD( OPC::ADD_EBX,	OPT::B1,		"EBX += B1" );																					// EBX += B1
	IWzASM_WB	ADD_EBX_B2		= DECL_WzASMCMD( OPC::ADD_EBX,	OPT::B2,		"EBX += B2" );																					// EBX += B2
	IWzASM_WB	ADD_EBX_B4		= DECL_WzASMCMD( OPC::ADD_EBX,	OPT::B4,		"EBX += B4" );																					// EBX += B4
																																												
	IWzASM_WB	ADD_ECX_EAX		= DECL_WzASMCMD( OPC::ADD_ECX,	OPT::EAX,		"ECX += EAX" );																					// ECX += EAX
	IWzASM_WB	ADD_ECX_EBX		= DECL_WzASMCMD( OPC::ADD_ECX,	OPT::EBX,		"ECX += EBX" );																					// ECX += EBX
	IWzASM_WB	ADD_ECX_EDX		= DECL_WzASMCMD( OPC::ADD_ECX,	OPT::EDX,		"ECX += EDX" );																					// ECX += EDX
	//																																											
	IWzASM_WB	ADD_ECX_B2		= DECL_WzASMCMD( OPC::ADD_ECX,	OPT::B2,		"ECX += B2" );																					// ECX += B2
	IWzASM_WB	ADD_EDX_B2		= DECL_WzASMCMD( OPC::ADD_EDX,	OPT::B2,		"EDX += B2" );																					// EDX += B2
																																												
	IWzASM_WB	SUB_ECX_EAX		= DECL_WzASMCMD( OPC::SUB_ECX,	OPT::EAX,		"ECX -= EAX" );																					// ECX -= EAX
	IWzASM_WB	SUB_ECX_EBX		= DECL_WzASMCMD( OPC::SUB_ECX,	OPT::EBX,		"ECX -= EBX" );																					// ECX -= EBX
	IWzASM_WB	SUB_ECX_EDX		= DECL_WzASMCMD( OPC::SUB_ECX,	OPT::EDX,		"ECX -= EDX" );																					// ECX -= EDX
																																												
	IWzASM_WB	MUL_ECX_EAX		= DECL_WzASMCMD( OPC::MUL_ECX,	OPT::EAX,		"ECX *= EAX" );																					// ECX *= EAX
	IWzASM_WB	MUL_ECX_EBX		= DECL_WzASMCMD( OPC::MUL_ECX,	OPT::EBX,		"ECX *= EBX" );																					// ECX *= EBX
	IWzASM_WB	MUL_ECX_EDX		= DECL_WzASMCMD( OPC::MUL_ECX,	OPT::EDX,		"ECX *= EDX" );																					// ECX *= EDX
																																												
	IWzASM_WB	DIV_ECX_EAX		= DECL_WzASMCMD( OPC::DIV_ECX,	OPT::EAX,		"ECX /= EAX" );																					// ECX /= EAX
	IWzASM_WB	DIV_ECX_EBX		= DECL_WzASMCMD( OPC::DIV_ECX,	OPT::EBX,		"ECX /= EBX" );																					// ECX /= EBX
	IWzASM_WB	DIV_ECX_EDX		= DECL_WzASMCMD( OPC::DIV_ECX,	OPT::EDX,		"ECX /= EDX" );																					// ECX /= EDX
																																												
	IWzASM_WB	SUB_EAX			= DECL_WzASMCMD( OPC::SUB,		OPT::EAX,		"EAX -= EAX" );																					// EAX -= EAX
	IWzASM_WB	SUB_EAX_EBX		= DECL_WzASMCMD( OPC::SUB,		OPT::EBX,		"EAX -= EBX" );																					// EAX -= EBX
	IWzASM_WB	SUB_EAX_B1		= DECL_WzASMCMD( OPC::SUB,		OPT::B1,		"EAX -= B1" );																					// EAX -= B1
	IWzASM_WB	SUB_EAX_B2		= DECL_WzASMCMD( OPC::SUB,		OPT::B2,		"EAX -= B2" );																					// EAX -= B2
	IWzASM_WB	SUB_EAX_B4		= DECL_WzASMCMD( OPC::SUB,		OPT::B4,		"EAX -= B4" );																					// EAX -= B4
																																												
	IWzASM_WB	SUB_EBX_EAX		= DECL_WzASMCMD( OPC::SUB_EBX,	OPT::EAX,		"EBX -= EAX" );																					// EBX -= EAX
	IWzASM_WB	SUB_EBX_B1		= DECL_WzASMCMD( OPC::SUB_EBX,	OPT::B1,		"EBX -= B1" );																					// EBX -= B1
	IWzASM_WB	SUB_EBX_B2		= DECL_WzASMCMD( OPC::SUB_EBX,	OPT::B2,		"EBX -= B2" );																					// EBX -= B2
	IWzASM_WB	SUB_EBX_B4		= DECL_WzASMCMD( OPC::SUB_EBX,	OPT::B4,		"EBX -= B4" );																					// EBX -= B4
	//																																											
	IWzASM_WB	SUB_ECX_B2		= DECL_WzASMCMD( OPC::SUB_ECX,	OPT::B2,		"ECX -= B2" );																					// ECX -= B2
	IWzASM_WB	SUB_EDX_B2		= DECL_WzASMCMD( OPC::SUB_EDX,	OPT::B2,		"EDX -= B2" );																					// EDX -= B2
																																												
	IWzASM_WB	MUL_EAX			= DECL_WzASMCMD( OPC::MUL,		OPT::EAX,		"EAX *= EAX" );																					// EAX *= EAX
	IWzASM_WB	MUL_EAX_EBX		= DECL_WzASMCMD( OPC::MUL,		OPT::EBX,		"EAX *= EBX" );																					// EAX *= EBX
	IWzASM_WB	MUL_EAX_B1		= DECL_WzASMCMD( OPC::MUL,		OPT::B1,		"EAX *= B1" );																					// EAX *= B1
	IWzASM_WB	MUL_EAX_B2		= DECL_WzASMCMD( OPC::MUL,		OPT::B2,		"EAX *= B2" );																					// EAX *= B2
	IWzASM_WB	MUL_EAX_B4		= DECL_WzASMCMD( OPC::MUL,		OPT::B4,		"EAX *= B4" );																					// EAX *= B4
																																												
	IWzASM_WB	DIV_EAX			= DECL_WzASMCMD( OPC::DIV,		OPT::EAX,		"EAX /= EAX" );																					// EAX /= EAX
	IWzASM_WB	DIV_EAX_EBX		= DECL_WzASMCMD( OPC::DIV,		OPT::EBX,		"EAX /= EBX" );																					// EAX /= EBX
	IWzASM_WB	DIV_EAX_B1		= DECL_WzASMCMD( OPC::DIV,		OPT::B1,		"EAX /= B1" );																					// EAX /= B1
	IWzASM_WB	DIV_EAX_B2		= DECL_WzASMCMD( OPC::DIV,		OPT::B2,		"EAX /= B2" );																					// EAX /= B2
	IWzASM_WB	DIV_EAX_B4		= DECL_WzASMCMD( OPC::DIV,		OPT::B4,		"EAX /= B4" );																					// EAX /= B4
																																												
	IWzASM_WB	INC_EAX			= DECL_WzASMCMD( OPC::INC,		OPT::EAX,		"EAX += 1" );																					// EAX += 1
	IWzASM_WB	INC_EBX			= DECL_WzASMCMD( OPC::INC,		OPT::EBX,		"EBX += 1" );																					// EBX += 1
																																												
	IWzASM_WB	DEC_EAX			= DECL_WzASMCMD( OPC::DEC,		OPT::EAX,		"EAX -= 1" );																					// EAX -= 1
	IWzASM_WB	DEC_EBX			= DECL_WzASMCMD( OPC::DEC,		OPT::EBX,		"EBX -= 1" );																					// EBX -= 1
																																												
	IWzASM_WB	TEST_EAX		= DECL_WzASMCMD( OPC::TEST,		OPT::EAX,		"EAX != 0, (T) EFL:TEST_RET = 1" );																// EAX != 0, (T) EFL:TEST_RET = 1
	IWzASM_WB	TEST_EBX		= DECL_WzASMCMD( OPC::TEST,		OPT::EBX,		"EBX != 0, (T) EFL:TEST_RET = 1" );																// EBX != 0, (T) EFL:TEST_RET = 1
	IWzASM_WB	TEST_ECX		= DECL_WzASMCMD( OPC::TEST,		OPT::ECX,		"ECX != 0, (T) EFL:TEST_RET = 1" );																// ECX != 0, (T) EFL:TEST_RET = 1
	IWzASM_WB	TEST_EDX		= DECL_WzASMCMD( OPC::TEST,		OPT::EDX,		"EDX != 0, (T) EFL:TEST_RET = 1" );																// EDX != 0, (T) EFL:TEST_RET = 1
	IWzASM_WB	TEST_ERP		= DECL_WzASMCMD( OPC::TEST,		OPT::ERP,		"ERP != 0, (T) EFL:TEST_RET = 1" );																// ERP != 0, (T) EFL:TEST_RET = 1
	IWzASM_WB	TEST_EOP		= DECL_WzASMCMD( OPC::TEST,		OPT::EOP,		"EOP != 0, (T) EFL:TEST_RET = 1" );																// EOP != 0, (T) EFL:TEST_RET = 1
																																												
	IWzASM_WB	CMP_EAX_EBX		= DECL_WzASMCMD( OPC::CMP,		OPT::EBX,		"EAX == EBX, (T) EFL:TEST_RET = 1" );															// EAX == EBX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_EAX_ECX		= DECL_WzASMCMD( OPC::CMP,		OPT::ECX,		"EAX == ECX, (T) EFL:TEST_RET = 1" );															// EAX == ECX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_EAX_EDX		= DECL_WzASMCMD( OPC::CMP,		OPT::EDX,		"EAX == EDX, (T) EFL:TEST_RET = 1" );															// EAX == EDX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_EAX_ERP		= DECL_WzASMCMD( OPC::CMP,		OPT::ERP,		"EAX == ERP, (T) EFL:TEST_RET = 1" );															// EAX == ERP, (T) EFL:TEST_RET = 1
																																												
	IWzASM_WB	CMP_ECX_EAX		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::EAX,		"ECX == EAX, (T) EFL:TEST_RET = 1" );															// ECX == EAX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_EBX		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::EBX,		"ECX == EBX, (T) EFL:TEST_RET = 1" );															// ECX == EBX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_EDX		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::EDX,		"ECX == EDX, (T) EFL:TEST_RET = 1" );															// ECX == EDX, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_ERP		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::ERP,		"ECX == ERP, (T) EFL:TEST_RET = 1" );															// ECX == ERP, (T) EFL:TEST_RET = 1
																																												
	IWzASM_WB	CMP_EAX_B1		= DECL_WzASMCMD( OPC::CMP,		OPT::B1,		"EAX == B1, (T) EFL:TEST_RET = 1" );															// EAX == B1, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_EAX_B2		= DECL_WzASMCMD( OPC::CMP,		OPT::B2,		"EAX == B2, (T) EFL:TEST_RET = 1" );															// EAX == B2, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_EAX_B4		= DECL_WzASMCMD( OPC::CMP,		OPT::B4,		"EAX == B4, (T) EFL:TEST_RET = 1" );															// EAX == B4, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_B1		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::B1,		"ECX == B1, (T) EFL:TEST_RET = 1" );															// ECX == B1, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_B2		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::B2,		"ECX == B2, (T) EFL:TEST_RET = 1" );															// ECX == B2, (T) EFL:TEST_RET = 1
	IWzASM_WB	CMP_ECX_B4		= DECL_WzASMCMD( OPC::CMP_ECX,	OPT::B4,		"ECX == B4, (T) EFL:TEST_RET = 1" );															// ECX == B4, (T) EFL:TEST_RET = 1
																																												
	IWzASM_WB	JZEROEQUAL		= DECL_WzASMCMD( OPC::JZE,		OPT::EFL,		"if( EFL == 0 ) goto MARK(x) else NEXT" );														// if( EFL == 0 ) goto MARK(x) else NEXT
	IWzASM_WB	JZERONOTEQUAL	= DECL_WzASMCMD( OPC::JNZ,		OPT::EFL,		"if( EFL != 0 ) goto MARK(x) else NEXT" );														// if( EFL != 0 ) goto MARK(x) else NEXT
																																												
	IWzASM_WB	JBELOW			= DECL_WzASMCMD( OPC::JB,		OPT::EFL,		"if( EFL == BELOW ) goto MARK(x) else NEXT" );													// if( EFL == BELOW ) goto MARK(x) else NEXT
	IWzASM_WB	JNOTBELOW		= DECL_WzASMCMD( OPC::JNB,		OPT::EFL,		"if( EFL != BELOW ) goto MARK(x) else NEXT" );													// if( EFL != BELOW ) goto MARK(x) else NEXT
	IWzASM_WB	JABOVE			= DECL_WzASMCMD( OPC::JA,		OPT::EFL,		"if( EFL == ABOVE ) goto MARK(x) else NEXT" );													// if( EFL == ABOVE ) goto MARK(x) else NEXT
	IWzASM_WB	JNOTABOVE		= DECL_WzASMCMD( OPC::JNA,		OPT::EFL,		"if( EFL != ABOVE ) goto MARK(x) else NEXT" );													// if( EFL != ABOVE ) goto MARK(x) else NEXT
	IWzASM_WB	JEQUAL			= DECL_WzASMCMD( OPC::JE,		OPT::EFL,		"if( EFL == EQUAL ) goto MARK(x) else NEXT" );													// if( EFL == EQUAL ) goto MARK(x) else NEXT
	IWzASM_WB	JNOTEQUAL		= DECL_WzASMCMD( OPC::JNE,		OPT::EFL,		"if( EFL != EQUAL ) goto MARK(x) else NEXT" );													// if( EFL != EQUAL ) goto MARK(x) else NEXT
																																												
	IWzASM_WB	JUMP			= DECL_WzASMCMD( OPC::JMP,		OPT::B1,		"goto MARK(x)" );																				// goto MARK(x)
	IWzASM_WB	JUMP1			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK01,	"goto MARK01" );																				// goto MARK01
	IWzASM_WB	JUMP2			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK02,	"goto MARK02" );																				// goto MARK02
	IWzASM_WB	JUMP3			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK03,	"goto MARK03" );																				// goto MARK03
	IWzASM_WB	JUMP4			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK04,	"goto MARK04" );																				// goto MARK04
	IWzASM_WB	JUMP5			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK05,	"goto MARK05" );																				// goto MARK05
	IWzASM_WB	JUMP6			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK06,	"goto MARK06" );																				// goto MARK06
	IWzASM_WB	JUMP7			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK07,	"goto MARK07" );																				// goto MARK07
	IWzASM_WB	JUMP8			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK08,	"goto MARK08" );																				// goto MARK08
	IWzASM_WB	JUMP9			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK09,	"goto MARK09" );																				// goto MARK09
	IWzASM_WB	JUMP10			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK10,	"goto MARK10" );																				// goto MARK10
																																												
	IWzASM_WB	JUMP11			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK11,	"goto MARK11" );																				// goto MARK11
	IWzASM_WB	JUMP12			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK12,	"goto MARK12" );																				// goto MARK12
	IWzASM_WB	JUMP13			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK13,	"goto MARK13" );																				// goto MARK13
	IWzASM_WB	JUMP14			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK14,	"goto MARK14" );																				// goto MARK14
	IWzASM_WB	JUMP15			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK15,	"goto MARK15" );																				// goto MARK15
	IWzASM_WB	JUMP16			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK16,	"goto MARK16" );																				// goto MARK16
	IWzASM_WB	JUMP17			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK17,	"goto MARK17" );																				// goto MARK17
	IWzASM_WB	JUMP18			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK18,	"goto MARK18" );																				// goto MARK18
	IWzASM_WB	JUMP19			= DECL_WzASMCMD( OPC::JMP,		OPT::MARK19,	"goto MARK19" );																				// goto MARK19
	//																																											
																																												
	//IWzASM_WB	LEA_ECX			= DECL_WzASMCMD( OPC::LEA_ECX,	OPT::EAX,	"FIND_FUNCTION_HANDLER(EAX), ECX(ret)																// FIND_FUNCTION_HANDLER(EAX), ECX(ret)
	//IWzASM_WB	LEA_ECX_HASH	= DECL_WzASMCMD( OPC::LEA_ECX,	OPT::HASH,	"FIND_FUNCTION_HANDLER(HASH), ECX(ret)																// FIND_FUNCTION_HANDLER(HASH), ECX(ret)
	//IWzASM_WB	LEA_EDX			= DECL_WzASMCMD( OPC::LEA_EDX,	OPT::EAX,	"FIND_DATA_HANDLER(EAX), EDX(ret)																	// FIND_DATA_HANDLER(EAX), EDX(ret)
	//IWzASM_WB	LEA_EDX_HASH	= DECL_WzASMCMD( OPC::LEA_EDX,	OPT::HASH,	"FIND_DATA_HANDLER(HASH), EDX(ret)																	// FIND_DATA_HANDLER(HASH), EDX(ret)
	IWzASM_WB	LEA_EFP_HASH	= DECL_WzASMCMD( OPC::LEA_EFP,	OPT::HASH,		"FIND_IBF_HANDLER(HASH), EFP0 = (HANDLER*)" );													// FIND_IBF_HANDLER(HASH), EFP0 = (HANDLER*)
	IWzASM_WB	LEA_REF_HASH	= DECL_WzASMCMD( OPC::LEA_REF,	OPT::HASH,		"FIND_GRES_HANDLER(HASH), EIP.REF = (HANDLER*)" );												// FIND_GRES_HANDLER(HASH), EIP.REF = (HANDLER*)
																																												
	IWzASM_WB	LEA_EAX_EOP		= DECL_WzASMCMD( OPC::LEA_EAX,	OPT::EOP,		"EAX = calc(EOP), calc(EOP) = EOP0±DT" );														// EAX = calc(EOP), calc(EOP) = EOP0±DT
	IWzASM_WB	LEA_EBX_EOP		= DECL_WzASMCMD( OPC::LEA_EBX,	OPT::EOP,		"EBX = calc(EOP), calc(EOP) = EOP0±DT" );														// EBX = calc(EOP), calc(EOP) = EOP0±DT
	IWzASM_WB	LEA_ECX_EOP		= DECL_WzASMCMD( OPC::LEA_ECX,	OPT::EOP,		"ECX = calc(EOP), calc(EOP) = EOP0±DT" );														// ECX = calc(EOP), calc(EOP) = EOP0±DT
	IWzASM_WB	LEA_EDX_EOP		= DECL_WzASMCMD( OPC::LEA_EDX,	OPT::EOP,		"EDX = calc(EOP), calc(EOP) = EOP0±DT" );														// EDX = calc(EOP), calc(EOP) = EOP0±DT
	IWzASM_WB	LEA_EOP_HASH	= DECL_WzASMCMD( OPC::LEA_EOP,	OPT::HASH,		"PTR = 'FIND HASH', EOP0 = PTR" );																// PTR = 'FIND HASH', EOP0 = PTR
	IWzASM_WB	LEA_EOP_B4		= DECL_WzASMCMD( OPC::LEA_EOP,	OPT::B4,		"EOP = B4(*)" );																				// EOP = B4(*)
	IWzASM_WB	LEA_EOP_HASHB4	= DECL_WzASMCMD( OPC::LEA_EOP,	OPT::HASHB4,	"PTR = 'FIND HASH', EOP0 = PTR, EOP1 = sCAL(OP,DT)" );											// PTR = 'FIND HASH', EOP0 = PTR, EOP1 = sCAL(OP,DT)
	IWzASM_WB	LEA_EOP_EXA2B4	= DECL_WzASMCMD( OPC::LEA_EOP,	OPT::EXA2B4,	"EOP0 = B4(*), EOP1 = sCAL(OP,DT)" );															// EOP0 = B4(*), EOP1 = sCAL(OP,DT)
	//																																											
	IWzASM_WB	CALL_RC			= DECL_WzASMCMD( OPC::CALL,		OPT::REF,		"CALL EIP.REF -- ERP(RET)" );																	// CALL EIP.REF -- ERP(RET)
	//IWzASM_WB	CALL_RD			= DECL_WzASMCMD( OPC::CALL,		OPT::EDX );			//																							//
	IWzASM_WB	CALL_IBF		= DECL_WzASMCMD( OPC::CALL,		OPT::PTR,		"CALL	PTR -- ERP(RET)" );																		// CALL	PTR -- ERP(RET)
	IWzASM_WB	CALL_IBF_EFP	= DECL_WzASMCMD( OPC::CALL,		OPT::EFP,		"CALL	EFP -- ERP(RET)" );																		// CALL	EFP -- ERP(RET)
	IWzASM_WB	RET				= DECL_WzASMCMD( OPC::RET,		OPT::NOP,		"RETURN EAX" );																					// RETURN EAX
																																												
	IWzASM_WB	MARK01			= DECL_WzASMCMD( OPC::MARK01,	OPT::NOP,		"LABEL MARK01" );																				// LABEL MARK01
	IWzASM_WB	MARK02			= DECL_WzASMCMD( OPC::MARK02,	OPT::NOP,		"LABEL MARK02" );																				// LABEL MARK02
	IWzASM_WB	MARK03			= DECL_WzASMCMD( OPC::MARK03,	OPT::NOP,		"LABEL MARK03" );																				// LABEL MARK03
	IWzASM_WB	MARK04			= DECL_WzASMCMD( OPC::MARK04,	OPT::NOP,		"LABEL MARK04" );																				// LABEL MARK04
	IWzASM_WB	MARK05			= DECL_WzASMCMD( OPC::MARK05,	OPT::NOP,		"LABEL MARK05" );																				// LABEL MARK05
	IWzASM_WB	MARK06			= DECL_WzASMCMD( OPC::MARK06,	OPT::NOP,		"LABEL MARK06" );																				// LABEL MARK06
	IWzASM_WB	MARK07			= DECL_WzASMCMD( OPC::MARK07,	OPT::NOP,		"LABEL MARK07" );																				// LABEL MARK07
	IWzASM_WB	MARK08			= DECL_WzASMCMD( OPC::MARK08,	OPT::NOP,		"LABEL MARK08" );																				// LABEL MARK08
	IWzASM_WB	MARK09			= DECL_WzASMCMD( OPC::MARK09,	OPT::NOP,		"LABEL MARK09" );																				// LABEL MARK09
	IWzASM_WB	MARK10			= DECL_WzASMCMD( OPC::MARK10,	OPT::NOP,		"LABEL MARK10" );																				// LABEL MARK10
																																												
	// TEMPLATE으로 구성된 명령어 집합에서 사용한다. 충분히 주의할 필요 있다.																									  
	IWzASM_WB	MARK11			= DECL_WzASMCMD( OPC::MARK11,	OPT::NOP,		"LABEL MARK11" );																				// LABEL MARK11
	IWzASM_WB	MARK12			= DECL_WzASMCMD( OPC::MARK12,	OPT::NOP,		"LABEL MARK12" );																				// LABEL MARK12
	IWzASM_WB	MARK13			= DECL_WzASMCMD( OPC::MARK13,	OPT::NOP,		"LABEL MARK13" );																				// LABEL MARK13
	IWzASM_WB	MARK14			= DECL_WzASMCMD( OPC::MARK14,	OPT::NOP,		"LABEL MARK14" );																				// LABEL MARK14
	IWzASM_WB	MARK15			= DECL_WzASMCMD( OPC::MARK15,	OPT::NOP,		"LABEL MARK15" );																				// LABEL MARK15
	IWzASM_WB	MARK16			= DECL_WzASMCMD( OPC::MARK16,	OPT::NOP,		"LABEL MARK16" );																				// LABEL MARK16
	IWzASM_WB	MARK17			= DECL_WzASMCMD( OPC::MARK17,	OPT::NOP,		"LABEL MARK17" );																				// LABEL MARK17
	IWzASM_WB	MARK18			= DECL_WzASMCMD( OPC::MARK18,	OPT::NOP,		"LABEL MARK18" );																				// LABEL MARK18
	IWzASM_WB	MARK19			= DECL_WzASMCMD( OPC::MARK19,	OPT::NOP,		"LABEL MARK19" );																				// LABEL MARK19

#undef IWzASM_WB
};

