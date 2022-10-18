#pragma once

#include "IWzASM.h"
#include "IWzASM.CmdBlk.Tools.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<PROTOTYPEs>
//
#pragma pack(push, 1)
#pragma push_macro("INLINE")
#pragma push_macro("CONST")
#pragma push_macro("STATIC")

#define INLINE		__forceinline
#define CONST		const
#define STATIC		static
#define CSTATIC		STATIC CONST

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<IL PROTOTYPEs>
struct IL
{
	union {
		OPW_T				X;
		struct { OPB_T		L, H; };
	};

	IL( CONST OPW_T cCMD ) : X(cCMD) {}
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<sIL PROTOTYPEs>
template<OPW_T _CMD>
struct sIL
{
	IL					FIELD;
	sIL() : FIELD(_CMD) {}

	INLINE VOID			Change( OPW_T cCMD )	{ FIELD.X = cCMD; }
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<sIL FAVORATE>
typedef sIL<IWzASM::RET>			sIL_RET;
typedef sIL<IWzASM::NOP>			sIL_NOP;
typedef sIL<IWzASM::MARK01>			sIL_MARK01;
typedef sIL<IWzASM::MARK02>			sIL_MARK02;
typedef sIL<IWzASM::MARK03>			sIL_MARK03;
typedef sIL<IWzASM::MARK04>			sIL_MARK04;
typedef sIL<IWzASM::MARK05>			sIL_MARK05;
typedef sIL<IWzASM::MARK06>			sIL_MARK06;
typedef sIL<IWzASM::MARK07>			sIL_MARK07;
typedef sIL<IWzASM::MARK08>			sIL_MARK08;
typedef sIL<IWzASM::MARK09>			sIL_MARK09;
typedef sIL<IWzASM::MARK10>			sIL_MARK10;
typedef sIL<IWzASM::MARK11>			sIL_MARK11;
typedef sIL<IWzASM::MARK12>			sIL_MARK12;
typedef sIL<IWzASM::MARK13>			sIL_MARK13;
typedef sIL<IWzASM::MARK14>			sIL_MARK14;
typedef sIL<IWzASM::MARK15>			sIL_MARK15;
typedef sIL<IWzASM::MARK16>			sIL_MARK16;
typedef sIL<IWzASM::MARK17>			sIL_MARK17;
typedef sIL<IWzASM::MARK18>			sIL_MARK18;
typedef sIL<IWzASM::MARK19>			sIL_MARK19;

// 1byte MARK 褻м
template<OPB_T _MARKB> struct		sIL_MARKC { sIL<DECL_WzASM_HASH(_MARKB, OPT::NOP)>	M01; };
typedef sIL_MARKC<OPC::MARK01>		sIL_MARKC01;
typedef sIL_MARKC<OPC::MARK02>		sIL_MARKC02;
typedef sIL_MARKC<OPC::MARK03>		sIL_MARKC03;
typedef sIL_MARKC<OPC::MARK04>		sIL_MARKC04;
typedef sIL_MARKC<OPC::MARK05>		sIL_MARKC05;
typedef sIL_MARKC<OPC::MARK06>		sIL_MARKC06;
typedef sIL_MARKC<OPC::MARK07>		sIL_MARKC07;
typedef sIL_MARKC<OPC::MARK08>		sIL_MARKC08;
typedef sIL_MARKC<OPC::MARK09>		sIL_MARKC09;
typedef sIL_MARKC<OPC::MARK10>		sIL_MARKC10;
typedef sIL_MARKC<OPC::MARK11>		sIL_MARKC11;
typedef sIL_MARKC<OPC::MARK12>		sIL_MARKC12;
typedef sIL_MARKC<OPC::MARK13>		sIL_MARKC13;
typedef sIL_MARKC<OPC::MARK14>		sIL_MARKC14;
typedef sIL_MARKC<OPC::MARK15>		sIL_MARKC15;
typedef sIL_MARKC<OPC::MARK16>		sIL_MARKC16;
typedef sIL_MARKC<OPC::MARK17>		sIL_MARKC17;
typedef sIL_MARKC<OPC::MARK18>		sIL_MARKC18;
typedef sIL_MARKC<OPC::MARK19>		sIL_MARKC19;

#if !WzASM_UPDATE_0x5001
	template<OPW_T _MARK>
	struct sIL_MARK
	{
		sIL<_MARK>			M01;
	};

	// 1byte MARK 褻м
	template<OPB_T _MARKB>
	struct sIL_MARK_COMPOSE
	{
		sIL<DECL_WzASM_HASH(_MARKB, OPT::NOP)>	M01;
	};
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<sDT PROTOTYPEs>
template<typename _TYPE, _TYPE _VAL>
struct sDT
{
	_TYPE				VALUE;
	sDT() : VALUE(_VAL) {}

	inline VOID			Change( _TYPE vVAL )	{	VALUE = vVAL;	}
};

struct DT1
{
	BYTE						L;
	DT1( CONST BYTE vVAL ) :	L(vVAL)	{}
};

struct DT2
{
	union {
		WORD					X;
		struct { BYTE			L, H; };
	};
	DT2( CONST WORD vVAL ) :	X(vVAL)	{}
};

struct DT4
{
	union {
		DWORD						EX;
		WORD						X;
		struct {
			union {
				struct { BYTE		L, H; };
				WORD				LX;
			};
			union {
				struct { BYTE		L, H; };
				WORD				HX;
			};
		};
	};
	DT4( CONST DWORD vVAL ) :	EX(vVAL){}
};

template<BYTE _VAL>		struct sDT1 : sDT<BYTE, _VAL> {};		// 1BYTE DATA
template<WORD _VAL>		struct sDT2 : sDT<WORD, _VAL> {};		// 2BYTE DATA
template<DWORD _VAL>	struct sDT4 : sDT<DWORD, _VAL> {};		// 4BYTE DATA
template<WzASMH _HASH>	struct sDTH : sDT<WzASMH, _HASH> {};	// 2BYTE DATA
template<OPB_T _MARK>	struct sDTM : sDT<OPB_T, _MARK> {};		// 1BYTE DATA - { OP(2B) | jmark(1B) }
template<WzPTR _PTR>	struct sDTP : sDT<WzPTR, _PTR> {};		// 4BYTE DATA

template<BYTE _CATEGORY, BYTE _PROTOCOL>
struct sDTPK : sDT<WORD, MAKEWORD(_CATEGORY, _PROTOCOL)> {};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<sDT SPECIAL>
//	<Offset Control Argument>
template<WORD _DT, WORD _OP>
struct sDT_CALC
{
	cOFF			CALC;
	sDT_CALC()
	{
		CALC.DT = _DT;//(WORD)offsetof( _TYPE, _FIELD );
		CALC.OP = _OP;
	}
};

template<typename _TYPE, typename _FIELD, typename WORD _OP>
struct sDT_CALC_T
{
	cOFF			CALC;
	sDT_CALC_T()
	{
		CALC.DT = (WORD)OFFSETOF( _TYPE, _FIELD );
		CALC.OP = _OP;
	}
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// 6 BYTE
struct sIL_CRYPT_BLK_HEADER_MAGIC
{
	//sDT1<'W'>								M01;
	//sDT1<0xF1>								M02;
	//sDT1<'A'>								M03;
	//sDT1<0x74>								M04;
	//sDT1<'m'>								M05;
	//sDT1<'4'>								M06;
	BYTE									MARRAY[6];
	sIL_CRYPT_BLK_HEADER_MAGIC()
	{
		MARRAY[0] = 'W';
		MARRAY[1] = 0xF1;
		MARRAY[2] = 'A';
		MARRAY[3] = 0x74;
		MARRAY[4] = 'm';
		MARRAY[5] = '4';
	}
};

// 6 BYTE + 7 BYTE
struct sIL_CRYPT_BLK_HEADER
{
	sIL_CRYPT_BLK_HEADER_MAGIC				M01;
	sDT2<'7'>								M02_CRYPT_KEY;
	sDT2<101>								M03_VERSION;
	sDT1<0xF7>								M04_TYPE;
	sDT1<0x90>								M05;
	sDT1<0x83>								M06;
};

// 8 BYTE
struct sIL_CRYPT_BLK_TAIL
{
	sDT1<0x71>								M01;
	sDT1<0x87>								M02;
	sDT1<'A'>								M03;
	sDT1<'s'>								M04;
	sDT1<'m'>								M05;
	sDT1<'|'>								M06;
	sDT1<'i'>								M07;
	sDT1<'x'>								M08;
};




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <CLR>
typedef sIL<IWzASM::CLR_EAX>		sIL_CLR_EAX;
typedef sIL<IWzASM::CLR_EBX>		sIL_CLR_EBX;
typedef sIL<IWzASM::CLR_ECX>		sIL_CLR_ECX;
typedef sIL<IWzASM::CLR_EDX>		sIL_CLR_EDX;
typedef sIL<IWzASM::CLR_ERP>		sIL_CLR_ERP;
typedef sIL<IWzASM::CLR_EOP>		sIL_CLR_EOP;
typedef sIL<IWzASM::CLR_EABCDX>		sIL_CLR_EABCDX;

typedef sIL<IWzASM::CLRA_EAX>		sIL_CLRA_EAX;
typedef sIL<IWzASM::CLRA_EBX>		sIL_CLRA_EBX;
typedef sIL<IWzASM::CLRA_ECX>		sIL_CLRA_ECX;
typedef sIL<IWzASM::CLRA_EDX>		sIL_CLRA_EDX;
typedef sIL<IWzASM::CLRA_ERP>		sIL_CLRA_ERP;
typedef sIL<IWzASM::CLRA_EOP>		sIL_CLRA_EOP;
typedef sIL<IWzASM::CLRA_EABCDX>	sIL_CLRA_EABCDX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <MOV>

typedef sIL<IWzASM::MOV_EAX_EBX>	sIL_MOV_EAX_EBX;
typedef sIL<IWzASM::MOV_EAX_ECX>	sIL_MOV_EAX_ECX;
typedef sIL<IWzASM::MOV_EAX_EDX>	sIL_MOV_EAX_EDX;
typedef sIL<IWzASM::MOV_EAX_ERP>	sIL_MOV_EAX_ERP;
typedef sIL<IWzASM::MOV_EAX_EOP>	sIL_MOV_EAX_EOP;

typedef sIL<IWzASM::MOV_EBX_EAX>	sIL_MOV_EBX_EAX;
typedef sIL<IWzASM::MOV_EBX_ECX>	sIL_MOV_EBX_ECX;
typedef sIL<IWzASM::MOV_EBX_EDX>	sIL_MOV_EBX_EDX;
typedef sIL<IWzASM::MOV_EBX_ERP>	sIL_MOV_EBX_ERP;
typedef sIL<IWzASM::MOV_EBX_EOP>	sIL_MOV_EBX_EOP;

typedef sIL<IWzASM::MOV_ECX_EAX>	sIL_MOV_ECX_EAX;
typedef sIL<IWzASM::MOV_ECX_EBX>	sIL_MOV_ECX_EBX;
typedef sIL<IWzASM::MOV_ECX_EDX>	sIL_MOV_ECX_EDX;
typedef sIL<IWzASM::MOV_ECX_ERP>	sIL_MOV_ECX_ERP;
typedef sIL<IWzASM::MOV_ECX_EOP>	sIL_MOV_ECX_EOP;

typedef sIL<IWzASM::MOV_EDX_EAX>	sIL_MOV_EDX_EAX;
typedef sIL<IWzASM::MOV_EDX_EBX>	sIL_MOV_EDX_EBX;
typedef sIL<IWzASM::MOV_EDX_ECX>	sIL_MOV_EDX_ECX;
typedef sIL<IWzASM::MOV_EDX_ERP>	sIL_MOV_EDX_ERP;
typedef sIL<IWzASM::MOV_EDX_EOP>	sIL_MOV_EDX_EOP;

typedef sIL<IWzASM::MOV_ERP_EAX>	sIL_MOV_ERP_EAX;
typedef sIL<IWzASM::MOV_ERP_EBX>	sIL_MOV_ERP_EBX;
typedef sIL<IWzASM::MOV_ERP_ECX>	sIL_MOV_ERP_ECX;
typedef sIL<IWzASM::MOV_ERP_EDX>	sIL_MOV_ERP_EDX;
typedef sIL<IWzASM::MOV_ERP_EOP>	sIL_MOV_ERP_EOP;

typedef sIL<IWzASM::MOV_EOP_EAX>	sIL_MOV_EOP_EAX;
typedef sIL<IWzASM::MOV_EOP_EBX>	sIL_MOV_EOP_EBX;
typedef sIL<IWzASM::MOV_EOP_ECX>	sIL_MOV_EOP_ECX;
typedef sIL<IWzASM::MOV_EOP_EDX>	sIL_MOV_EOP_EDX;
typedef sIL<IWzASM::MOV_EOP_ERP>	sIL_MOV_EOP_EOP;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <MOV><SPECIAL>
typedef sIL<IWzASM::MOV_EOP_OFF>	sIL_MOV_ERP_OFF;

// ERP[1].CALC = CALC(_DT, _OP)
template<typename WORD _DT, typename WORD _OP>
struct sIL_MOV_ERP_CALC_BLK
{
	sIL_MOV_ERP_OFF					M01;
	sDT_CALC<_DT, _OP>				M02;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
/* REG[0].EX = (_TYPE)_VAL */ 
template<typename _TYPE, OPW_T _CMD, typename _DECLTYPE, typename BYTE _SZ>
struct sIL_MOV_VAL
{
	sIL<_CMD>							M01;
	_DECLTYPE							M02;

	inline VOID			ChangeVAL( _TYPE vVAL )
	{	M02.Change( vVAL );	}
};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename BYTE _VAL>
struct sIL_MOV_EAX_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EAX_B1, sDT1<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EAX_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX_B2, sDT2<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EAX_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EAX_B4, sDT4<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzASMH _HASH>
struct sIL_MOV_EAX_HASH : sIL_MOV_VAL<WzASMH, IWzASM::MOV_EAX_HASH, sDTH<_HASH>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EAX_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EAX_PTR, sDTP<_PTR>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename BYTE _VAL>
struct sIL_MOV_EBX_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EBX_B1, sDT1<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EBX_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EBX_B2, sDT2<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EBX_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EBX_B4, sDT4<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzASMH _HASH>
struct sIL_MOV_EBX_HASH : sIL_MOV_VAL<WzASMH, IWzASM::MOV_EBX_HASH, sDTH<_HASH>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EBX_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EBX_PTR, sDTP<_PTR>, 0> {};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <MOV><SPECIAL><EXA IDX>
/* REG[0].EX = (_TYPE)_VAL */ 
template<typename BYTE _VAL>
struct sIL_MOV_EAX0_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EAX0_B1, sDT1<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EAX0_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX0_B2, sDT2<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EAX0_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EAX0_B4, sDT4<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzASMH _VAL>
struct sIL_MOV_EAX0_HASH : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX0_HASH, sDTH<_VAL>, 0> {};

/* REG[0].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EAX0_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EAX0_PTR, sDTP<_PTR>, 0> {};

//
template<typename BYTE _VAL>
struct sIL_MOV_EAX1_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EAX1_B1, sDT1<_VAL>, 0> {};

/* REG[1].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EAX1_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX1_B2, sDT2<_VAL>, 0> {};

/* REG[1].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EAX1_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EAX1_B4, sDT4<_VAL>, 0> {};

/* REG[1].EX = (_TYPE)_VAL */ 
template<typename WzASMH _VAL>
struct sIL_MOV_EAX1_HASH : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX1_HASH, sDTH<_VAL>, 0> {};

/* REG[1].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EAX1_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EAX1_PTR, sDTP<_PTR>, 0> {};

//
/* REG[2].EX = (_TYPE)_VAL */ 
template<typename BYTE _VAL>
struct sIL_MOV_EAX2_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EAX2_B1, sDT1<_VAL>, 0> {};

/* REG[2].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EAX2_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX2_B2, sDT2<_VAL>, 0> {};

/* REG[2].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EAX2_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EAX2_B4, sDT4<_VAL>, 0> {};

/* REG[2].EX = (_TYPE)_VAL */ 
template<typename WzASMH _VAL>
struct sIL_MOV_EAX2_HASH : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX2_HASH, sDTH<_VAL>, 0> {};

/* REG[2].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EAX2_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EAX2_PTR, sDTP<_PTR>, 0> {};

//
/* REG[3].EX = (_TYPE)_VAL */ 
template<typename BYTE _VAL>
struct sIL_MOV_EAX3_B1 : sIL_MOV_VAL<BYTE, IWzASM::MOV_EAX3_B1, sDT1<_VAL>, 0> {};

/* REG[3].EX = (_TYPE)_VAL */ 
template<typename WORD _VAL>
struct sIL_MOV_EAX3_B2 : sIL_MOV_VAL<WORD, IWzASM::MOV_EAX3_B2, sDT2<_VAL>, 0> {};

/* REG[3].EX = (_TYPE)_VAL */ 
template<typename DWORD _VAL>
struct sIL_MOV_EAX3_B4 : sIL_MOV_VAL<DWORD, IWzASM::MOV_EAX3_B4, sDT4<_VAL>, 0> {};

/* REG[3].EX = (_TYPE)_VAL */ 
template<typename WzASMH _VAL>
struct sIL_MOV_EAX3_HASH : sIL_MOV_VAL<WzASMH, IWzASM::MOV_EAX3_HASH, sDTH<_VAL>, 0> {};

/* REG[3].EX = (_TYPE)_VAL */ 
template<typename WzPTR _PTR>
struct sIL_MOV_EAX3_PTR : sIL_MOV_VAL<WzPTR, IWzASM::MOV_EAX3_PTR, sDTP<_PTR>, 0> {};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <MOV><SPECIAL><EXA>
template<	typename _TYPE, OPW_T _CMD,
			typename _DECLTYPE1, typename _DECLTYPE2, typename _DECLTYPE3, typename _DECLTYPE4,
			typename BYTE _SZ
		>
struct sIL_MOV_EAX_ARR4_VAL
{
	sIL<_CMD>							M01;
	_DECLTYPE1							M02_ARG0;
	_DECLTYPE2							M03_ARG1;
	_DECLTYPE3							M04_ARG2;
	_DECLTYPE4							M05_ARG3;
};

template<	typename _TYPE, OPW_T _CMD,
			typename _DECLTYPE1, typename _DECLTYPE2, typename _DECLTYPE3,
			typename BYTE _SZ
		>
struct sIL_MOV_EAX_ARR3_VAL
{
	sIL<_CMD>							M01;
	_DECLTYPE1							M02_ARG0;
	_DECLTYPE2							M03_ARG1;
	_DECLTYPE3							M04_ARG2;
};

template<	typename _TYPE, OPW_T _CMD,
			typename _DECLTYPE1, typename _DECLTYPE2,
			typename BYTE _SZ
		>
struct sIL_MOV_EAX_ARR2_VAL
{
	sIL<_CMD>							M01;
	_DECLTYPE1							M02_ARG0;
	_DECLTYPE2							M03_ARG1;
};

template<typename BYTE _B1, BYTE _B2, BYTE _VAL3, BYTE _B4>
struct sIL_MOV_EAX_EXA4B1 : sIL_MOV_EAX_ARR4_VAL<BYTE, IWzASM::MOV_EAX_EXA4B1, sDT1<_B1>, sDT1<_B2>, sDT1<_VAL3>, sDT1<_B4>, 0> {};

template<typename WORD _B1, WORD _B2, WORD _VAL3, WORD _B4>
struct sIL_MOV_EAX_EXA4B2 : sIL_MOV_EAX_ARR4_VAL<WORD, IWzASM::MOV_EAX_EXA4B2, sDT2<_B1>, sDT2<_B2>, sDT2<_VAL3>, sDT2<_B4>, 0> {};

template<typename DWORD _B1, DWORD _B2, DWORD _VAL3, DWORD _B4>
struct sIL_MOV_EAX_EXA4B4 : sIL_MOV_EAX_ARR4_VAL<DWORD, IWzASM::MOV_EAX_EXA4B4, sDT4<_B1>, sDT4<_B2>, sDT4<_VAL3>, sDT4<_B4>, 0> {};

template<typename WzASMH _B1, WzASMH _B2, WzASMH _VAL3, WzASMH _B4>
struct sIL_MOV_EAX_EXA4HS : sIL_MOV_EAX_ARR4_VAL<WzASMH, IWzASM::MOV_EAX_EXA4HS, sDTH<_B1>, sDTH<_B2>, sDTH<_VAL3>, sDTH<_B4>, 0> {};

//////////////////////////////////////////////////////////////////////////

template<typename BYTE _B1, BYTE _B2, BYTE _VAL3>
struct sIL_MOV_EAX_EXA3B1 : sIL_MOV_EAX_ARR3_VAL<BYTE, IWzASM::MOV_EAX_EXA3B1, sDT1<_B1>, sDT1<_B2>, sDT1<_VAL3>, 0> {};

template<typename WORD _B1, WORD _B2, WORD _VAL3>
struct sIL_MOV_EAX_EXA3B2 : sIL_MOV_EAX_ARR3_VAL<WORD, IWzASM::MOV_EAX_EXA3B2, sDT2<_B1>, sDT2<_B2>, sDT2<_VAL3>, 0> {};

template<typename DWORD _B1, DWORD _B2, DWORD _VAL3>
struct sIL_MOV_EAX_EXA3B4 : sIL_MOV_EAX_ARR3_VAL<DWORD, IWzASM::MOV_EAX_EXA3B4, sDT4<_B1>, sDT4<_B2>, sDT4<_VAL3>, 0> {};

template<typename WzASMH _B1, WzASMH _B2, WzASMH _VAL3>
struct sIL_MOV_EAX_EXA3HS : sIL_MOV_EAX_ARR3_VAL<WzASMH, IWzASM::MOV_EAX_EXA3HS, sDTH<_B1>, sDTH<_B2>, sDTH<_VAL3>, 0> {};

//////////////////////////////////////////////////////////////////////////

template<typename BYTE _B1, BYTE _B2>
struct sIL_MOV_EAX_EXA2B1 : sIL_MOV_EAX_ARR2_VAL<BYTE, IWzASM::MOV_EAX_EXA2B1, sDT1<_B1>, sDT1<_B2>, 0> {};

template<typename WORD _B1, WORD _B2>
struct sIL_MOV_EAX_EXA2B2 : sIL_MOV_EAX_ARR2_VAL<WORD, IWzASM::MOV_EAX_EXA2B2, sDT2<_B1>, sDT2<_B2>, 0> {};

template<typename DWORD _B1, DWORD _B2>
struct sIL_MOV_EAX_EXA2B4 : sIL_MOV_EAX_ARR2_VAL<DWORD, IWzASM::MOV_EAX_EXA2B4, sDT4<_B1>, sDT4<_B2>, 0> {};

template<typename WzASMH _B1, WzASMH _B2>
struct sIL_MOV_EAX_EXA2HS : sIL_MOV_EAX_ARR2_VAL<WzASMH, IWzASM::MOV_EAX_EXA2HS, sDTH<_B1>, sDTH<_B2>, 0> {};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// MOV & PUSH
//	(1)	E(A/D)X = E(A/D)X
//		PUSH E(A/D)X
//	(2)	E(A/B)X = VAL_xx
//		PUSH E(A/B)X
template<typename _MOVCMD, typename OPW_T _PUSHCMD>
struct sIL_MOVnPUSH
{
	_MOVCMD								M01;
	sIL<_PUSHCMD>						M02;
};

//	(2)	E(A/B)X = VAL_xx
//		PUSH E(A/B)X
template<typename _TYPE, OPW_T _CMD, typename _DECLTYPE, typename OPW_T _PUSHCMD>
struct sIL_MOVnPUSH_REG_VAL : sIL_MOVnPUSH<sIL_MOV_VAL<_TYPE, _CMD, _DECLTYPE, 0>, _PUSHCMD> {};

//////////////////////////////////////////////////////////////////////////

template<typename BYTE _VAL>
struct sIL_MOVnPUSH_EAX_B1 : sIL_MOVnPUSH_REG_VAL<BYTE, IWzASM::MOV_EAX_B1, sDT1<_VAL>, IWzASM::PUSH_EAX> {};

template<typename WORD _VAL>
struct sIL_MOVnPUSH_EAX_B2 : sIL_MOVnPUSH_REG_VAL<WORD, IWzASM::MOV_EAX_B2, sDT2<_VAL>, IWzASM::PUSH_EAX> {};

template<typename DWORD _VAL>
struct sIL_MOVnPUSH_EAX_B4 : sIL_MOVnPUSH_REG_VAL<DWORD, IWzASM::MOV_EAX_B4, sDT4<_VAL>, IWzASM::PUSH_EAX> {};

template<typename WzASMH _HASH>
struct sIL_MOVnPUSH_EAX_HASH : sIL_MOVnPUSH_REG_VAL<WzASMH, IWzASM::MOV_EAX_HASH, sDTH<_HASH>, IWzASM::PUSH_EAX> {};

template<typename WzPTR _PTR>
struct sIL_MOVnPUSH_EAX_PTR : sIL_MOVnPUSH_REG_VAL<WzPTR, IWzASM::MOV_EAX_PTR, sDTP<_PTR>, IWzASM::PUSH_EAX> {};

//
template<typename BYTE _VAL>
struct sIL_MOVnPUSH_EBX_B1 : sIL_MOVnPUSH_REG_VAL<BYTE, IWzASM::MOV_EBX_B1, sDT1<_VAL>, IWzASM::PUSH_EBX> {};

template<typename WORD _VAL>
struct sIL_MOVnPUSH_EBX_B2 : sIL_MOVnPUSH_REG_VAL<WORD, IWzASM::MOV_EBX_B2, sDT2<_VAL>, IWzASM::PUSH_EBX> {};

template<typename DWORD _VAL>
struct sIL_MOVnPUSH_EBX_B4 : sIL_MOVnPUSH_REG_VAL<DWORD, IWzASM::MOV_EBX_B4, sDT4<_VAL>, IWzASM::PUSH_EBX> {};

template<typename WzASMH _HASH>
struct sIL_MOVnPUSH_EBX_HASH : sIL_MOVnPUSH_REG_VAL<WzASMH, IWzASM::MOV_EBX_HASH, sDTH<_HASH>, IWzASM::PUSH_EBX> {};

template<typename WzPTR _PTR>
struct sIL_MOVnPUSH_EBX_PTR : sIL_MOVnPUSH_REG_VAL<WzPTR, IWzASM::MOV_EBX_PTR, sDTP<_PTR>, IWzASM::PUSH_EBX> {};

//////////////////////////////////////////////////////////////////////////

struct sIL_MOVnPUSH_EAX_EBX : sIL_MOVnPUSH<sIL_MOV_EAX_EBX, IWzASM::PUSH_EAX> {};
struct sIL_MOVnPUSH_EAX_ECX : sIL_MOVnPUSH<sIL_MOV_EAX_ECX, IWzASM::PUSH_EAX> {};
struct sIL_MOVnPUSH_EAX_EDX : sIL_MOVnPUSH<sIL_MOV_EAX_EDX, IWzASM::PUSH_EAX> {};
struct sIL_MOVnPUSH_EAX_ERP : sIL_MOVnPUSH<sIL_MOV_EAX_ERP, IWzASM::PUSH_EAX> {};

struct sIL_MOVnPUSH_EBX_EAX : sIL_MOVnPUSH<sIL_MOV_EBX_EAX, IWzASM::PUSH_EBX> {};
struct sIL_MOVnPUSH_EBX_ECX : sIL_MOVnPUSH<sIL_MOV_EBX_ECX, IWzASM::PUSH_EBX> {};
struct sIL_MOVnPUSH_EBX_EDX : sIL_MOVnPUSH<sIL_MOV_EBX_EDX, IWzASM::PUSH_EBX> {};
struct sIL_MOVnPUSH_EBX_ERP : sIL_MOVnPUSH<sIL_MOV_EBX_ERP, IWzASM::PUSH_EBX> {};

struct sIL_MOVnPUSH_ECX_EAX : sIL_MOVnPUSH<sIL_MOV_ECX_EAX, IWzASM::PUSH_ECX> {};
struct sIL_MOVnPUSH_ECX_EBX : sIL_MOVnPUSH<sIL_MOV_ECX_EBX, IWzASM::PUSH_ECX> {};
struct sIL_MOVnPUSH_ECX_EDX : sIL_MOVnPUSH<sIL_MOV_ECX_EDX, IWzASM::PUSH_ECX> {};
struct sIL_MOVnPUSH_ECX_ERP : sIL_MOVnPUSH<sIL_MOV_ECX_ERP, IWzASM::PUSH_ECX> {};

struct sIL_MOVnPUSH_EDX_EAX : sIL_MOVnPUSH<sIL_MOV_EDX_EAX, IWzASM::PUSH_EDX> {};
struct sIL_MOVnPUSH_EDX_EBX : sIL_MOVnPUSH<sIL_MOV_EDX_EBX, IWzASM::PUSH_EDX> {};
struct sIL_MOVnPUSH_EDX_ECX : sIL_MOVnPUSH<sIL_MOV_EDX_ECX, IWzASM::PUSH_EDX> {};
struct sIL_MOVnPUSH_EDX_ERP : sIL_MOVnPUSH<sIL_MOV_EDX_ERP, IWzASM::PUSH_EDX> {};

struct sIL_MOVnPUSH_ERP_EAX : sIL_MOVnPUSH<sIL_MOV_ERP_EAX, IWzASM::PUSH_ERP> {};
struct sIL_MOVnPUSH_ERP_EBX : sIL_MOVnPUSH<sIL_MOV_ERP_EBX, IWzASM::PUSH_ERP> {};
struct sIL_MOVnPUSH_ERP_ECX : sIL_MOVnPUSH<sIL_MOV_ERP_ECX, IWzASM::PUSH_ERP> {};
struct sIL_MOVnPUSH_ERP_EDX : sIL_MOVnPUSH<sIL_MOV_ERP_EDX, IWzASM::PUSH_ERP> {};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// STORE
//	溯雖蝶攪蒂 餌辨ж雖 彊堅 霜蕾 殮溘
//		PUSH VAL_xx
template<typename _TYPE, typename _DECLTYPE, typename OPW_T _PUSHCMD>
struct sIL_PUSH_VAL
{
	sIL<_PUSHCMD>						M01;
	_DECLTYPE							M02;

	inline VOID			Change( _TYPE vVAL )	{	M01.Change( vVAL );	}
};

template<typename BYTE _VAL>
struct sIL_PUSH_B1 : sIL_PUSH_VAL<BYTE, sDT1<_VAL>, IWzASM::PUSH_B1> {};

template<typename WORD _VAL>
struct sIL_PUSH_B2 : sIL_PUSH_VAL<WORD, sDT2<_VAL>, IWzASM::PUSH_B2> {};

template<typename DWORD _VAL>
struct sIL_PUSH_B4 : sIL_PUSH_VAL<DWORD, sDT4<_VAL>, IWzASM::PUSH_B4> {};

template<typename WzASMH _HASH>
struct sIL_PUSH_HASH : sIL_PUSH_VAL<WzASMH, sDTH<_HASH>, IWzASM::PUSH_HASH> {};


////收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
////		EBX = VAL_xx
////		EAX += EBX
//template<typename _TYPE, OPW_T _CMD, typename _DECLTYPE, typename BYTE _SZ>
//struct sIL_ADD32_EAX_VAL
//{
//	sIL<_CMD>							M01;
//	_DECLTYPE							M02;
//	sIL<IWzASM::ADD_EAX_EBX>				M03;
//
//	inline VOID			Change( _TYPE vVAL )
//	{	M02.Change( vVAL );	}
//};
//
//template<typename BYTE _VAL>
//struct sIL_ADD32_EAX_B1 : sIL_ADD32_EAX_VAL<BYTE, IWzASM::MOV_EBX_B1, sDT1<_VAL>, 0> {};
//
//template<typename WORD _VAL>
//struct sIL_ADD32_EAX_B2 : sIL_ADD32_EAX_VAL<WORD, IWzASM::MOV_EBX_B2, sDT2<_VAL>, 0> {};
//
//template<typename DWORD _VAL>
//struct sIL_ADD32_EAX_B4 : sIL_ADD32_EAX_VAL<DWORD, IWzASM::MOV_EBX_B4, sDT4<_VAL>, 0> {};



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (窗撬擎 牖憮縑 鼻婦橈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEBX : EBX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EAX += EBX
template<typename _LDEBX, typename BYTE _TMP>
struct sIL_OP_EAX_PLUS_XXX
{
	_LDEBX						M01;
	sIL<IWzASM::ADD_EAX_EBX>	M02;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (窗撬擎 牖憮縑 鼻婦橈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEBX : EAX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EBX += EAX
template<typename _LDEAX, typename BYTE _TMP>
struct sIL_OP_EBX_PLUS_XXX
{
	_LDEAX						M01;
	sIL<IWzASM::ADD_EBX_EAX>	M02;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename BYTE _VAL>			struct sIL_OP_EAX_PLUS_B1 : sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_B1<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename WORD _VAL>			struct sIL_OP_EAX_PLUS_B2 : sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_B2<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename DWORD _VAL>			struct sIL_OP_EAX_PLUS_B4 : sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_B4<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename BYTE _VAL>			struct sIL_OP_EBX_PLUS_B1 : sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_B1<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename WORD _VAL>			struct sIL_OP_EBX_PLUS_B2 : sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_B2<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<EXPRESSION>
//		EAX = EAX + VAL_xx
//		1) EBX = VAL_xx	(M01.M02)
//		2) EAX += EBX
template<typename DWORD _VAL>
struct sIL_OP_EBX_PLUS_B4 : sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_B4<_VAL>, 0>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EAX += ExX
typedef sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_ECX, 0>		sIL_OP_EAX_PLUS_ECX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EAX += ExX
typedef sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_EDX, 0>		sIL_OP_EAX_PLUS_EDX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EAX += ExX
typedef sIL_OP_EAX_PLUS_XXX<sIL_MOV_EBX_ERP, 0>		sIL_OP_EAX_PLUS_ERP;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EBX += ExX
typedef sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_ECX, 0>		sIL_OP_EBX_PLUS_ECX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EBX += ExX
typedef sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_EDX, 0>		sIL_OP_EBX_PLUS_EDX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	EBX += ExX
typedef sIL_OP_EBX_PLUS_XXX<sIL_MOV_EAX_ERP, 0>		sIL_OP_EBX_PLUS_ERP;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (牖憮 鼻婦 氈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEBX : EBX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EAX -= EBX
// <EXPRESSION>
//   EAX = EAX - VAL_xx
//		1) EBX = VAL_xx
//		2) EAX -= EBX
template<typename _LDEBX, typename BYTE _TMP>
struct sIL_OP_EAX_MINUS_XXX
{
	_LDEBX						M01;
	sIL<IWzASM::SUB_EAX_EBX>	M02;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (牖憮 鼻婦 氈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEAX : EBX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EBX -= EAX
// <EXPRESSION>
//   EBX = EBX - VAL_xx
//		1) EAX = VAL_xx
//		2) EBX -= EAX
template<typename _LDEAX, typename BYTE _TMP>
struct sIL_OP_EBX_MINUS_XXX
{
	_LDEAX						M01;
	sIL<IWzASM::SUB_EBX_EAX>	M02;
};

// <EXPRESSION>
//   EAX = EAX - VAL_xx
//		1) EBX = VAL_xx
//		2) EAX -= EBX
template<typename BYTE _VAL>			struct sIL_OP_EAX_MINUS_B1 : sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_B1<_VAL>, 0>	{};
template<typename WORD _VAL>			struct sIL_OP_EAX_MINUS_B2 : sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_B2<_VAL>, 0>	{};
template<typename DWORD _VAL>			struct sIL_OP_EAX_MINUS_B4 : sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_B4<_VAL>, 0>	{};

// <EXPRESSION>
//   EBX = EBX - VAL_xx
//		1) EAX = VAL_xx
//		2) EBX -= EAX
template<typename BYTE _VAL>			struct sIL_OP_EBX_MINUS_B1 : sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_B1<_VAL>, 0>	{};
template<typename WORD _VAL>			struct sIL_OP_EBX_MINUS_B2 : sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_B2<_VAL>, 0>	{};
template<typename DWORD _VAL>			struct sIL_OP_EBX_MINUS_B4 : sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_B4<_VAL>, 0>	{};

//	EAX -= ExX
typedef sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_ECX, 0>		sIL_OP_EAX_MINUS_ECX;
typedef sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_EDX, 0>		sIL_OP_EAX_MINUS_EDX;
typedef sIL_OP_EAX_MINUS_XXX<sIL_MOV_EBX_ERP, 0>		sIL_OP_EAX_MINUS_ERP;
//	EBX -= ExX
typedef sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_ECX, 0>		sIL_OP_EBX_MINUS_ECX;
typedef sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_EDX, 0>		sIL_OP_EBX_MINUS_EDX;
typedef sIL_OP_EBX_MINUS_XXX<sIL_MOV_EAX_ERP, 0>		sIL_OP_EBX_MINUS_ERP;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (牖憮 鼻婦 橈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEBX : EBX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EAX *= EBX, -or- EBX *= EAX
// <EXPRESSION>
//   EAX = EAX * VAL_xx
//		1) EBX = VAL_xx
//		2) EAX *= VAL_xx
template<typename _LDEBX, typename BYTE _TMP>
struct sIL_OP_EAX_MUL_XXX
{
	_LDEBX						M01;
	sIL<IWzASM::MUL_EAX_EBX>	M02;
};

//template<typename _LDEBX, typename BYTE _TMP>
//struct sIL_OP_EBX_MUL_XXX
//{
//	_LDEBX						M01;
//	sIL<IWzASM::MUL_EBX_EAX>	M02;
//};

// <EXPRESSION>
//		1) EBX = VAL_xx
//		2) EAX *= EBX
template<typename BYTE _VAL>			struct sIL_OP_EAX_MUL_B1 : sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_B1<_VAL>, 0>	{};
template<typename WORD _VAL>			struct sIL_OP_EAX_MUL_B2 : sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_B2<_VAL>, 0>	{};
template<typename DWORD _VAL>			struct sIL_OP_EAX_MUL_B4 : sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_B4<_VAL>, 0>	{};

//// <EXPRESSION>
////		1) EAX = VAL_xx
////		2) EBX *= EAX
//template<typename BYTE _VAL>			struct sIL_OP_EBX_MUL_B1 : sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_B1<_VAL>, 0>	{};
//template<typename WORD _VAL>			struct sIL_OP_EBX_MUL_B2 : sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_B2<_VAL>, 0>	{};
//template<typename DWORD _VAL>			struct sIL_OP_EBX_MUL_B4 : sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_B4<_VAL>, 0>	{};

//	EAX *= ExX
typedef sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_ECX, 0>		sIL_OP_EAX_MUL_ECX;
typedef sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_EDX, 0>		sIL_OP_EAX_MUL_EDX;
typedef sIL_OP_EAX_MUL_XXX<sIL_MOV_EBX_ERP, 0>		sIL_OP_EAX_MUL_ERP;
////	EBX *= ExX
//typedef sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_ECX, 0>		sIL_OP_EBX_MUL_ECX;
//typedef sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_EDX, 0>		sIL_OP_EBX_MUL_EDX;
//typedef sIL_OP_EBX_MUL_XXX<sIL_MOV_EAX_ERP, 0>		sIL_OP_EBX_MUL_ERP;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> EAX朝 詭檣 翱骯 奢除 (牖憮 鼻婦 橈擠)
// <USED REG>	EAX, EBX
//		EAX : 翱骯脹 濰模
// <ARG>
//		_LDEBX : EBX縑 煎萄ж朝 ⑽鷓 CMD BLOCK
// <COMMENT>
//		EAX /= EBX, -or- EBX /= EAX
// <EXPRESSION>
//   EAX = EAX / VAL_xx
//		1) EBX = VAL_xx
//		2) EAX /= VAL_xx
template<typename _LDEBX, typename BYTE _TMP>
struct sIL_OP_EAX_DIV_XXX
{
	_LDEBX						M01;
	sIL<IWzASM::DIV_EAX_EBX>	M02;
};

//template<typename _LDEBX, typename BYTE _TMP>
//struct sIL_OP_EBX_DIV_XXX
//{
//	_LDEBX						M01;
//	sIL<IWzASM::DIV_EBX_EAX>	M02;
//};

// <EXPRESSION>
//		1) EBX = VAL_xx
//		2) EAX /= EBX
template<typename BYTE _VAL>			struct sIL_OP_EAX_DIV_B1 : sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_B1<_VAL>, 0>	{};
template<typename WORD _VAL>			struct sIL_OP_EAX_DIV_B2 : sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_B2<_VAL>, 0>	{};
template<typename DWORD _VAL>			struct sIL_OP_EAX_DIV_B4 : sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_B4<_VAL>, 0>	{};

//// <EXPRESSION>
////		1) EAX = VAL_xx
////		2) EBX /= EAX
//template<typename BYTE _VAL>			struct sIL_OP_EBX_DIV_B1 : sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_B1<_VAL>, 0>	{};
//template<typename WORD _VAL>			struct sIL_OP_EBX_DIV_B2 : sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_B2<_VAL>, 0>	{};
//template<typename DWORD _VAL>			struct sIL_OP_EBX_DIV_B4 : sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_B4<_VAL>, 0>	{};

//	EAX /= ExX
typedef sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_ECX, 0>		sIL_OP_EAX_DIV_ECX;
typedef sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_EDX, 0>		sIL_OP_EAX_DIV_EDX;
typedef sIL_OP_EAX_DIV_XXX<sIL_MOV_EBX_ERP, 0>		sIL_OP_EAX_DIV_ERP;
////	EBX /= ExX
//typedef sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_ECX, 0>		sIL_OP_EBX_DIV_ECX;
//typedef sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_EDX, 0>		sIL_OP_EBX_DIV_EDX;
//typedef sIL_OP_EBX_DIV_XXX<sIL_MOV_EAX_ERP, 0>		sIL_OP_EBX_DIV_ERP;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄>
//   ++EAX, ++EBX
typedef sIL<IWzASM::INC_EAX>	sIL_INC_EAX;
typedef sIL<IWzASM::INC_EBX>	sIL_INC_EBX;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄>
//   --EAX, --EBX
typedef sIL<IWzASM::DEC_EAX>	sIL_DEC_EAX;
typedef sIL<IWzASM::DEC_EBX>	sIL_DEC_EBX;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄>
//		(TEST_ExX != 0),	EFL:TEST_RET = { != 0, == 0 }
//		(ExX compare ExX),	EFL:TEST_RET = { <, >, ==, !(<), !(>) }

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄>
//	(ExX != 0), (TRUE) EFL:TEST_RET = { <, >, == }
template<typename OPW_T _CMD>
struct sIL_CMP
{
	sIL<_CMD>							M01;
};

typedef sIL_CMP<IWzASM::TEST_EAX>		sIL_TEST_EAX;
typedef sIL_CMP<IWzASM::TEST_EBX>		sIL_TEST_EBX;
typedef sIL_CMP<IWzASM::TEST_ECX>		sIL_TEST_ECX;
typedef sIL_CMP<IWzASM::TEST_EDX>		sIL_TEST_EDX;
typedef sIL_CMP<IWzASM::TEST_ERP>		sIL_TEST_ERP;
typedef sIL_CMP<IWzASM::TEST_EOP>		sIL_TEST_EOP;
typedef sIL_CMP<IWzASM::CMP_EAX_EBX>	sIL_CMP_EAX_EBX;
typedef sIL_CMP<IWzASM::CMP_EAX_ECX>	sIL_CMP_EAX_ECX;
typedef sIL_CMP<IWzASM::CMP_EAX_EDX>	sIL_CMP_EAX_EDX;
typedef sIL_CMP<IWzASM::CMP_EAX_ERP>	sIL_CMP_EAX_ERP;
typedef sIL_CMP<IWzASM::CMP_ECX_EAX>	sIL_CMP_ECX_EBX;
typedef sIL_CMP<IWzASM::CMP_ECX_EBX>	sIL_CMP_ECX_ECX;
typedef sIL_CMP<IWzASM::CMP_ECX_EDX>	sIL_CMP_ECX_EDX;
typedef sIL_CMP<IWzASM::CMP_ECX_ERP>	sIL_CMP_ECX_ERP;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄>
//	(EFL:TEST_RET (cond.) (val) )
// switch( EFL ) { >, <, >= <=, ==, !=, == 0, != 0 }
template<typename OPW_T _CMD, OPB_T _JMARK>
struct sIL_COND_JUMP
{
	sIL<_CMD>							M01;
	sDTM<_JMARK>						M02;

	inline VOID			ChangeCMD( OPW_T wCMD )
	{	M01.Change( wCMD );	}
	inline VOID			ChangeMARK( OPB_T jMARK )
	{	M02.Change( jMARK );	}
};

template<typename OPB_T _JMARK>
struct sIL_COND_ZERO_EQUAL :		sIL_COND_JUMP<IWzASM::JZEROEQUAL, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_ZERO_NOT_EQUAL :	sIL_COND_JUMP<IWzASM::JZERONOTEQUAL, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_EQUAL :				sIL_COND_JUMP<IWzASM::JEQUAL, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_NOT_EQUAL :			sIL_COND_JUMP<IWzASM::JNOTEQUAL, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_BELOW :				sIL_COND_JUMP<IWzASM::JBELOW, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_NOT_BELOW :			sIL_COND_JUMP<IWzASM::JNOTBELOW, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_ABOVE :				sIL_COND_JUMP<IWzASM::JABOVE, _JMARK>	{};
template<typename OPB_T _JMARK>
struct sIL_COND_NOT_ABOVE :			sIL_COND_JUMP<IWzASM::JNOTABOVE, _JMARK>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <綠掖僥>
//		<, >, ==, !=, == 0, != 0
template<typename _EXPRCOND, typename OPW_T _CMPCMD, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE
{
	_EXPRCOND							M01;
	sIL_COND_JUMP<_CMPCMD, _JMPMARK>	M02;
};

template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_ZERO :		sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JZEROEQUAL, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_NOT_ZERO :	sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JZERONOTEQUAL, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_EQUAL :		sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JEQUAL, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_NOT_EQUAL :	sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JNOTEQUAL, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_BELOW :		sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JBELOW, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_NOT_BELOW :	sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JNOTBELOW, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_ABOVE :		sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JABOVE, _JMPMARK>	{};
template<typename _EXPRCOND, typename OPB_T _JMPMARK>
struct sIL_IF_THEN_ELSE_IS_NOT_ABOVE :	sIL_IF_THEN_ELSE<_EXPRCOND, IWzASM::JNOTABOVE, _JMPMARK>	{};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <晦獄> 鼠褻勒 碟晦
//	goto MARK(x)
template<typename OPB_T _JMARK>
struct sIL_JUMP
{
	sIL<IWzASM::JUMP>					M01;
	sDT1<_JMARK>						M02;
};

//
template<typename OPW_T _CMD>
struct sIL_JUMPn
{
	sIL<_CMD>							M01;
};

typedef sIL_JUMPn<IWzASM::JUMP1> 	sIL_JUMP1;
typedef sIL_JUMPn<IWzASM::JUMP2> 	sIL_JUMP2;
typedef sIL_JUMPn<IWzASM::JUMP3> 	sIL_JUMP3;
typedef sIL_JUMPn<IWzASM::JUMP4> 	sIL_JUMP4;
typedef sIL_JUMPn<IWzASM::JUMP5> 	sIL_JUMP5;
typedef sIL_JUMPn<IWzASM::JUMP6> 	sIL_JUMP6;
typedef sIL_JUMPn<IWzASM::JUMP7> 	sIL_JUMP7;
typedef sIL_JUMPn<IWzASM::JUMP8> 	sIL_JUMP8;
typedef sIL_JUMPn<IWzASM::JUMP9> 	sIL_JUMP9;
typedef sIL_JUMPn<IWzASM::JUMP10> 	sIL_JUMP10;

typedef sIL_JUMPn<IWzASM::JUMP11> 	sIL_JUMP11;
typedef sIL_JUMPn<IWzASM::JUMP12> 	sIL_JUMP12;
typedef sIL_JUMPn<IWzASM::JUMP13> 	sIL_JUMP13;
typedef sIL_JUMPn<IWzASM::JUMP14> 	sIL_JUMP14;
typedef sIL_JUMPn<IWzASM::JUMP15> 	sIL_JUMP15;
typedef sIL_JUMPn<IWzASM::JUMP16> 	sIL_JUMP16;
typedef sIL_JUMPn<IWzASM::JUMP17> 	sIL_JUMP17;
typedef sIL_JUMPn<IWzASM::JUMP18> 	sIL_JUMP18;
typedef sIL_JUMPn<IWzASM::JUMP19> 	sIL_JUMP19;

template<typename OPB_T _JMARK>
struct sIL_JUMP_COMPOSE : sIL_JUMPn<DECL_WzASM_HASH(_JMARK, OPT::NOP)> {};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 褻勒 ル⑷衝 橈檜 EFL 0檣雖 嬴棋雖曖 罹睡 っ欽
template<typename OPB_T _JMPMARK>
struct sIL_IFZERO_THEN_ELSE
{
	sIL<IWzASM::JZEROEQUAL>				M01;
	sDTM<_JMPMARK>					M02;

	inline VOID			Change( OPB_T mMARK )
	{	M02.Change( mMARK ); }
};

template<typename OPB_T _JMPMARK>
struct sIL_IFNZERO_THEN_ELSE
{
	sIL<IWzASM::JZERONOTEQUAL>			M01;
	sDTM<_JMPMARK>					M02;

	inline VOID			Change( OPB_T mMARK )
	{	M02.Change( mMARK ); }
};

//template<typename _EBXIL, typename _USRCODE, typename OPB_T _MARK4LOOP>
//struct sIL_FOR_LOOP
//{
//	sIL_CLR_EAX													M01;
//	sIL_CLR_ECX													M02;
//	_EBXIL														M03;
//	sIL_JUMP11													M04;
//	//sIL_MARK01												M05;
//	sIL_MARKC<_MARK4LOOP>										M05;
//	sIL<IWzASM::ADD_ECX_EAX>									M06;
//	sIL<IWzASM::INC_EAX>										M07;
//	sIL<IWzASM::PUSH_EABCDX>									M08;
//	_USRCODE													M09;
//	sIL<IWzASM::POP_EABCDX>										M10;
//	sIL_MARK11													M11;
//	sIL_IF_THEN_ELSE_IS_BELOW<sIL_CMP_EAX_EBX, _MARK4LOOP>		M12;
//};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <FOR LOOP>
//
template<
	typename _INIT, typename _EXPR, typename _ITR,
	typename _LOGIC,
	typename OPB_T _EXPRMARK, typename OPB_T _LOOPMARK
	>
struct sIL_FORLOOP_RAW
{
	_INIT										M01;
	sIL_JUMP_COMPOSE<_EXPRMARK>					M02;
	sIL_MARKC<_LOOPMARK>						M03;
	_ITR										M04;
	sIL<IWzASM::PUSH_EABCDX>					M05;
	_LOGIC										M06;
	sIL<IWzASM::POP_EABCDX>						M07;
	sIL_MARKC<_EXPRMARK>						M08;
	_EXPR										M09;
};

// LOAD to EBX
template<typename _CMD, typename WORD _SZ>
struct sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_INIT_REG
{
	sIL_CLR_EAX													M01;
	sIL_CLR_ERP													M02;
	_CMD														M03;
};

struct sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_ITERATOR
{
	sIL<IWzASM::INC_EAX>										M02;
};

// <NOTE>
//	EBX : UPPER LIMIT
//	EAX : INCREASE COUNT
//	ECX
// 檜 瑞ず戲煎 霞殮衛 EDX 奢除虜檜 餌辨 陛棟ж棻. 輿曖й 匙.
template<typename _USRCODE, typename DWORD _UPRVAL, typename OPB_T _EXPRMARK, typename OPB_T _LOOPMARK>
struct sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP :
	sIL_FORLOOP_RAW<
		sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_INIT_REG<sIL_MOV_EBX_B4<_UPRVAL>, 0>,
		sIL_IF_THEN_ELSE_IS_BELOW<sIL_CMP_EAX_EBX, _LOOPMARK>,
		sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_ITERATOR,
		_USRCODE,
		_EXPRMARK, _LOOPMARK
		>
{
};


// 檜 瑞ず戲煎 霞殮衛 EDX 奢除虜檜 餌辨 陛棟ж棻. 輿曖й 匙.
template<typename _INITCODE, typename _USRCODE, typename _ITR, typename DWORD _UPRVAL, typename OPB_T _EXPRMARK, typename OPB_T _LOOPMARK>
struct sIL_FORLOOP_USR_DEFINE_COUNTING_LOOP :
	sIL_FORLOOP_RAW<
	_INITCODE,
	sIL_IF_THEN_ELSE_IS_BELOW<sIL_CMP_EAX_EBX, _LOOPMARK>,
	_ITR,
	_USRCODE,
	_EXPRMARK, _LOOPMARK
	>
{
};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	CALL_IBF	-> Call Reserved Internal Base Function
//		- IBF л熱朝 薑п霞 滌紫曖 薑п霞 EFP陛 襄營и棻.
//
//	<EXPRESSION>
//		ERP = (IBF*)( POP() );
template<typename HandlerIBF _FNPTR>
struct sIL_CALL_IBF_PTR
{
	sIL<IWzASM::CALL_IBF>				M01;	//	Call Base Function
	sDT<HandlerIBF, _FNPTR>				M02;	//	Internal Base Function

	inline VOID			Change( HandlerIBF fnPTR )
	{	M02.Change( fnPTR ); }
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	CALL_IBF ⑽衝
//		- 檣濠朝 檜嘐 蝶鷗縑 PUSH腎橫 氈戲貊,
//		[0]曖 嬪纂縑朝 EFP煎 �倏諻●囟� л熱 ん檣攪 蛔煙腎橫 氈橫撿 и棻.
//		- 蝶鷗 PUSH,POP戲煎 か薑 溯雖蝶攪陛 嬴棋 嬪纂縑 渠п GENERAL CALL擊 熱чи棻.
//
//	<EXPRESSION>
//		EFP = POP();
//		PUSH( EFP );
//		ERP = (EFP[0]*)( POP() );
struct sIL_CALL_IBF_STK_EFP
{
	sIL<IWzASM::POP_EFP>				M01;
	sIL<IWzASM::PUSH_EFP>				M02;
	sIL<IWzASM::CALL_IBF_EFP>			M03;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	CALL_IBF ⑽衝
//		- 檣濠朝 檜嘐 蝶鷗縑 PUSH腎橫 氈棻.
//
//	<EXPRESSION>
//		EFP[0] = FIND_HASH(_HASH)
//		ERP = (EFP[0]*)( POP() );
template<typename WzASMH _HASH>
struct sIL_CALL_IBF_HASH
{
	sIL<IWzASM::LEA_EFP_HASH>			M01;
	sDTH<_HASH>						M02;
	sIL<IWzASM::CALL_IBF_EFP>			M03;

	inline VOID		ChangeHASH( WzASM_IBF_HASH_T wHASH_IBF )
	{
		M02.Change( wHASH_IBF );
	};
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	CALL_IBF ⑽衝
//		- 檣濠朝 餌辨濠陛 霜蕾 殮溘и棻.
//
//	<EXPRESSION>
//		(_PUSHCODES)
//		EFP[0] = FIND_HASH(_HASH)
//		ERP = (EFP[0]*)( POP() );
template<typename _PUSHCODES, typename WzASMH _HASH>
struct sIL_CALL_IBF_HASH_USRINPUT
{
	_PUSHCODES							M01;
	sIL_CALL_IBF_HASH<_HASH>			M02;

	inline VOID		ChangeHASH( WzASM_IBF_HASH_T wHASH_IBF )
	{
		M02.ChangeHASH( wHASH_IBF );
	};
};

////收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//// <NOT SUPPORTED>
//// 檜瞪 唸婁高 EAX蒂 檣濠煎 嫡嬴菟檣棻.
//// 唸婁高 EAX高擎 嶸雖脹棻.
//struct sIL_FAST_CALL_RIB
//{
//	sIL<IWzASM::LEA_ECX>				M01;	//	Find Base Function by EAX
//	sIL<IWzASM::CALL_IBF>				M02;	//	Call Base Function
//};
//
////收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//// 檜瞪 唸婁高 EAX蒂 檣濠煎 嫡嬴菟檣棻.
//// 唸婁高 EAX高擎 嶸雖ж雖 跤и棻.
//template<typename WzASMH _HASHCODE, typename _INPUTARG, typename WORD _SZ>
//struct sIL_CALL_RC
//{
//	_INPUTARG							M01;	//	л熱��轎 衛 檣濠煎 厥擊 高
//	sIL<IWzASM::LEA_ECX_HASH>			M02;	//	Find Base Function by HASH
//	sDT<WzASMH, _HASHCODE>		M03;	//	HASHCODE
//	sIL<IWzASM::CALL_RC>				M04;	//	Call GRES C Function
//
//	inline VOID			Change( WzASMH hHASH )
//	{	M02.Change( hHASH ); }
//};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	sIL_CALL_RC
//		- 檜瞪 唸婁高 EAX蒂 檣濠煎 嫡嬴菟檣棻.
//		- 唸婁高 EAX高擎 嶸雖ж雖 跤и棻.
//
//	<EXPRESSION>
//		(_INPUTARG)
//		REF = FIND_GRES_HASH(_HASH)
//		ERP = (REF)( POP() );
template<typename WzASMH _HASHCODE, typename _INPUTARG, typename WORD _SZ>
struct sIL_CALL_RC
{
	_INPUTARG							M01;	//	л熱��轎 衛 檣濠煎 厥擊 高
	sIL<IWzASM::LEA_REF_HASH>			M02;	//	Find Base Function by HASH
	sDTH<_HASHCODE>					M03;	//	HASHCODE
	sIL<IWzASM::CALL_RC>				M04;	//	Call GRES C Function

	inline VOID			Change( WzASMH hHASH )
	{	M02.Change( hHASH ); }
};


//
//struct sIL_FAST_CALL_RC
//{
//	sIL<IWzASM::LEA_ECX>				M01;	//	Find Base Function by EAX
//	sIL<IWzASM::CALL_RC>				M02;	//	Call GRES C Function
//};
//
////收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//// 檜瞪 唸婁高 EAX蒂 檣濠煎 嫡嬴菟檣棻.
//// 唸婁高 EAX高擎 嶸雖ж雖 跤и棻.
//template<typename WzASMH _HASHCODE>
//struct sIL_CALL_RD
//{
//	sIL<IWzASM::LEA_EDX_HASH>			M01;	//	Find Base Function by HASH
//	sDT<WzASMH, _HASHCODE>		M02;	//	HASHCODE
//	sIL<IWzASM::CALL_RD>				M03;	//	Call GRES D Function
//
//	inline VOID			Change( WzASMH hHASH )
//	{	M02.Change( hHASH ); }
//};
//
//struct sIL_FAST_CALL_RD
//{
//	sIL<IWzASM::LEA_EDX>				M01;	//	Find Base Function by EAX
//	sIL<IWzASM::CALL_RD>				M02;	//	Call GRES C Function
//};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//		檜匙擎 HASH煎 蛔煙脹 л熱 閨虜 嬴棲塭 霜蕾 詭賅葬 ん檣攪蒂 殮溘п憮
//		餌辨ж嘎煎 HASH縑 蛔煙脹 л熱陛 嬴棲渦塭紫 ��轎檜 陛棟ж棻.
struct sIL_CALL_IBF_PTR_MAKE_ARGS
{
	sIL_CLRA_EAX									M01_CLRA_EAX;
	struct sINTER_make_arguments {
		sIL_MOV_EAX0_PTR<NULL>				M01_EIP_OBJ;
		sIL_MOV_EAX1_PTR<NULL>				M02_ARG1_CMDPTR;	// { 0, (ARG1*), 0, 0 }
		sIL_MOV_EAX2_B2<0>					M03_ARG2_VAL;		// { 0, (ARG1*), (ARG2), 0 }
	}												M02_make_arguments;
	sIL<IWzASM::PUSH_EAX>							M04_INPUTARG;	// Input ARG
	sIL_CALL_IBF_PTR<NULL>							M05_CALL_IBF_HANDLER;

	inline VOID		ChangeARG( BYTE* IN pCMDSTREAM, WORD wSIZE )
	{
		M02_make_arguments.M02_ARG1_CMDPTR.ChangeVAL( pCMDSTREAM );
		M02_make_arguments.M03_ARG2_VAL.ChangeVAL( wSIZE );
	}

	inline VOID		RegisterIBF( HandlerIBF fnIBF )
	{	M05_CALL_IBF_HANDLER.Change( fnIBF );	}

	inline VOID		RegisterEIP( WzASM_EIP* pEIP )
	{
		//uREG_NODE& rNODE = *(uREG_NODE*)&M01.M02_ARG0;
		//rNODE.PTR = pEIP;

		M02_make_arguments.M01_EIP_OBJ.ChangeVAL( (WzPTR)pEIP );
	};
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <sMETHOD_CALL_IBF_PTR>
//		檜匙擎 絮董脹 л熱 - 掘碟 寞徹 (sIL_RET陛 襄營ж朝 唳辦)
struct sMETHOD_CALL_IBF_PTR
{
	sIL<IWzASM::PUSH_EAX>						M01;
	sIL_CALL_IBF_PTR_MAKE_ARGS					M02;
	sIL<IWzASM::POP_EAX>						M03;
	sIL_RET										M04;

	inline VOID		ChangeARG( BYTE* IN pCMDSTREAM, WORD wSIZE )
	{	M02.ChangeARG( pCMDSTREAM, wSIZE );	}

	inline VOID		RegisterIBF( HandlerIBF fnIBF )
	{	M02.RegisterIBF( fnIBF );	}

	inline VOID		RegisterEIP( WzASM_EIP* pEIP )
	{	M02.RegisterEIP( pEIP );	};
};



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//{
//		sUREG_NODE uArg0( NULL, _ALLOC_SZ );
//		BYTE* pPTR = HandlerIBF_(malloc)(&uArg0.NODE);
//		sUREG_NODE uArg1( NULL, pPTR, _ALLOC_SZ );
//		HandlerIBF_(ZeroMemory)( &uArg1.NODE );
//		return pPTR;
//}
template<typename _DECLTYPE1, typename _DECLTYPE2, typename WORD _ALLOC_SZ=1>
struct sMETHOD_ALLOCnCLEAR_MEMORY_BASE
{
	sIL<IWzASM::PUSH_EAX>						M01;

	sIL_MOV_EAX_EXA2B4<0, 0>					M02;
	sIL_MOV_EAX1_B2<_ALLOC_SZ>					M03;	// 罹晦梱雖陛 ん裝 虜萇 匙
	sIL_MOV_EBX_EAX								M04;	// 歜衛 盪濰

	sIL<IWzASM::PUSH_EAX>						M05;
	_DECLTYPE1									M06;
	sIL_MOV_EAX_EBX								M07;	// 等檜攪 犒錳
	sIL<IWzASM::MOV_EAX2_EAX1>					M08;	// ん裝 營薑溺
	sIL<IWzASM::MOV_EAX1_ERP0>					M09;	// 唸婁高檣 ん檣攪 й渡
	sIL<IWzASM::PUSH_EAX>						M10;
	_DECLTYPE2									M11;
	sIL<IWzASM::MOV_ERP0_EAX1>					M12;
	sIL<IWzASM::POP_EAX>						M13;
	sIL_RET										M14;


	inline VOID		RegisterEIP( WzASM_EIP* pEIP )
	{
		uREG_NODE& rNODE = *(uREG_NODE*)&M02.M02_ARG0;
		rNODE.PTR = pEIP;
	};

	inline VOID		ChangeSize( WORD wSIZE )
	{	M03.ChangeVAL( wSIZE );	}
};

template<typename WORD _ALLOC_SZ=1>
struct sMETHOD_ALLOCnCLEAR_MEMORY_PTR
	: sMETHOD_ALLOCnCLEAR_MEMORY_BASE<
		sIL_CALL_IBF_PTR<NULL>, sIL_CALL_IBF_PTR<NULL>, _ALLOC_SZ
		>
{
	sMETHOD_ALLOCnCLEAR_MEMORY_PTR()
	{
		M06.Change( FNPTR_IBF_HANDLER(malloc) );
		M11.Change( FNPTR_IBF_HANDLER(ZeroMemory) );
	}
};

template<typename WORD _ALLOC_SZ=1>
struct sMETHOD_ALLOCnCLEAR_MEMORY_HASH
	: sMETHOD_ALLOCnCLEAR_MEMORY_BASE<
	sIL_CALL_IBF_HASH<IBF_HASH::IBF_malloc>, sIL_CALL_IBF_HASH<IBF_HASH::IBF_ZeroMemory>, _ALLOC_SZ
	>
{
};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE>
//		й渡脹 詭賅葬蒂 蛔煙脹 IBFс菟楝曖 檣濠煎 剩啖遽棻.
// <WARNING>
//		й渡脹 詭賅葬蒂 п薯ж朝 瑞ず擎 襄營ж雖 彊朝棻. 闊, IBFс菟楝縑憮 п薯
//		衛酈剪釭 ERP高戲煎 雖錳п撿 й 匙檜棻.
//
// <SAMPLE>
//
//		BYTE* pPTR = malloc(_ALLOC_SZ);
//		ZeroMemory( pPTR, _ALLOC_SZ );
//		sUREG_NODE uArg( WzASM_EIP*, pPTR, _ALLOC_SZ );
//		HandlerIBF_(REGISTERED)( &uArg.NODE );
//
template<typename _DECLTYPE1, typename WORD _ALLOC_SZ = 1>
struct sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE
{
	sIL<IWzASM::PUSH_EABCDX>							M01;
	sIL_MOV_EAX_EXA2B1<0, 0>							M02;
	sIL_MOV_EAX1_B2<_ALLOC_SZ>							M03;	// 罹晦梱雖陛 ん裝 虜萇 匙
	sIL<IWzASM::PUSH_EAX>								M04;	// 歜衛 盪濰 (ん裝)
	sIL<IWzASM::PUSH_EAX>								M05;	// IBF 檣濠
	_DECLTYPE1											M06;	// ALLOCnCLEAR_MEMORY( NULL, [SZ] )
	sIL<IWzASM::POP_EAX>								M07;	// 等檜攪 犒錳 (ん裝)
	sIL<IWzASM::MOV_EAX2_EAX1>							M08;	// [EIP*], [(SZ)], [SZ]
	sIL<IWzASM::MOV_EAX1_ERP0>							M09;	// [EIP*], [PTR], [SZ] 唸婁高檣 ん檣攪 й渡
	sIL<IWzASM::PUSH_EAX>								M10;	// IBF 檣濠
	sIL_CALL_IBF_PTR<NULL>								M11;	// RegisterIBFHandler (п渡 ん檣攪縑 new & cmd execute)
	sIL<IWzASM::POP_EABCDX>								M12;
	sIL_RET												M13;

	sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE() {};

	inline VOID		Register( WzASM_EIP* pEIP, HandlerIBF fnPTR )
	{
		//uREG_NODE& rNODE = *(uREG_NODE*)&M02.M02;
		//rNODE.PTR = pEIP;
		M11.Change( fnPTR );
	};

	inline VOID		ChangeSize( WORD wSIZE )
	{
		M03.Change( wSIZE );
	}
};

//////////////////////////////////////////////////////////////////////////
//	<sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR>
//		sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE + POINTER IBF HANDLER 蛔煙
template<typename WORD _ALLOC_SZ = 1>
struct sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR
	: public sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE<
	sIL_CALL_IBF_PTR<NULL>, _ALLOC_SZ
	>
{
	sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR()
	{
		M06.Change( FNPTR_IBF_HANDLER(ALLOCnCLEAR_MEMORY) );
	};
};

//////////////////////////////////////////////////////////////////////////
//	<sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR>
//		sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE + HASH IBF HANDLER 蛔煙
template<typename WORD _ALLOC_SZ = 1>
struct sMETHOD_CALL_WITH_ALLOCEDMEMORY_HASH
	: public sMETHOD_CALL_WITH_ALLOCEDMEMORY_BASE<
	sIL_CALL_IBF_HASH<IBF_HASH::IBF_ALLOCnCLEAR_MEMORY>, _ALLOC_SZ
	>
{
};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <sIL_WzEIP_CALL_WITH_FREE_BASE>
//		輿橫霞 詭賅葬蒂 п薯и棻. // [EIP*], [PTR]
//		檜匙擊 ��轎 ж晦 瞪
//		uNODE.EXA[sARG::ARG1]縑 輿橫霞 高檜 襄營п撿 и棻.
//
// <SAMPLE>
//		uREG_NODE uNODE;
//		uNODE.EXA[sARG::ARG1].PTR = (freeing ptr);
//		free( uNODE.EXA[sARG::ARG1].PTR )
//
template<typename _LDEAXCMD, typename DWORD _SZ=1>
struct sIL_WzEIP_CALL_WITH_FREE_BASE
{
	_LDEAXCMD											M01;	// EAX[1]縑 п薯й ん檣攪 襄營ж朝 ⑽鷓煎 滲⑽
	sIL<IWzASM::PUSH_EAX>								M02;	// 歜衛 盪濰 (ん裝)
	sIL_CALL_IBF_HASH<IBF_HASH::IBF_free>				M03;	// IBF_free( NULL, [PTR] ) -> ERP[0]朝 鼠婦
};




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<GET OFFSET_POINTER on OBJECT>
//
//	<EXPRESSION>
//		MEMBER* pMEMBER = &((OBJECT*)->MEMBER(by _OFFSET));
//
//		EAX = (OBJECT*)
//		EOP[0] = EAX;
//		EOP[1] = sCALC( _OFFSET, ADD )
//		EAX = EOP[0] + EOP[1]
//
//	<USED REG>
//		EAX = { ~, pMEMBER, ~ }
//		EOP = { OBJECT*, sCALC(OBJECT, MEMBER) }
//
template<typename WORD _OFFSET>
struct sIL_GET_OFFSET_PTR
{
	sIL<IWzASM::MOV_EOP_EAX>							M01;		// EOP = EAX
	sIL<IWzASM::MOV_EOP_OFF>							M02;		// EOP[1] = NEXT-B4(sCALC)
	sDT_CALC<_OFFSET, cOFF::ADD>						M03_OFFSET;	// -(P)-sCALC(DT,OP)
	sIL<IWzASM::LEA_EAX_EOP>							M04;		// EAX = EOP + -(P)- 

	// 檜嘐 й渡脹 偌羹曖 褫撢啗骯 等檜攪 滲唳
	inline VOID		ChangeOFFSETofPTR( WORD wOffset, cOFF::eOP eCMD )
	{
		M03_OFFSET.CALC.DT = wOffset;
		M03_OFFSET.CALC.OP = eCMD;
	}
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<GET OFFSET_POINTER on OBJECT>
//
//	<EXPRESSION>
//		MEMBER* pMEMBER = &((OBJECT*)->MEMBER(by _OFFSET));
//
//		EAX = (OBJECT*)
//		EOP[0] = EAX;
//		EOP[1] = sCALC( _OFFSET, ADD )
//		EAX = EOP[0] + EOP[1]
//
//	<USED REG>
//		EAX = { pMEMBER, ~ }
//		EOP = { OBJECT*, sCALC(OBJECT, MEMBER) }
//
template<typename WzPTR _ROOTPTR, typename WORD _OFFSET>
struct sIL_GET_OFFSET_PTR_ON_ROOT {
	sIL_MOV_EAX_PTR<_ROOTPTR>							M01;
	sIL_GET_OFFSET_PTR<_OFFSET>							M02_OFFSET;

	// 晦遽 EOP高 滲唳
	inline VOID		ChangeBaseObject( WzPTR pPTR )
	{	M01.ChangeVAL( pPTR );	}

	// 檜嘐 й渡脹 偌羹曖 褫撢啗骯 等檜攪 滲唳
	inline VOID		ChangeOFFSETofPTR( WORD wOffset, cOFF::eOP eCMD )
	{	M02_OFFSET.ChangeOFFSETofPTR( wOffset, eCMD );	}
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<CHECK BOOLEAN>
//
//	<EXPRESSION>
//		MEMBER* pMEMBER = &((OBJECT*)->MEMBER(by _OFFSET));
//		BOOL bCHECK = CALL_HANDLER( 0, pMEMBER, 0, 0 );
//		if( bCHECK )
//		{ 滌紫 囀萄 雖錳 в蹂 }
//		else
//			goto _JMPFAILCASE;
//
//
//	<USED REG>
//		EAX = { ~, pMEMBER, ~ }
//		EOP = { OBJECT*, sCALC(OBJECT, MEMBER) }
//		ERP = bCHECK
//		EFL
//
template<typename WzASM_IBF_HASH_T _IBF_HASH, typename WORD _OFFSET, typename OPB_T _JMPFAILCASE>
struct sIL_CHECK_BOOL
{
	sIL_GET_OFFSET_PTR_ON_ROOT<NULL, _OFFSET>						M01;
	sIL<IWzASM::MOV_EAX1_EAX0>										M02;

	sIL_CALL_IBF_HASH_USRINPUT<sIL<IWzASM::PUSH_EAX>, _IBF_HASH>	M04_CHECK_BOOL;
	sIL_IF_THEN_ELSE_IS_ZERO<sIL_TEST_ERP, _JMPFAILCASE>			M09;

	// 晦遽 EOP高 滲唳
	inline VOID		ChangeBaseObject( WzPTR pPTR )
	{	M01.ChangeBaseObject( pPTR );	}

	// 檜嘐 й渡脹 偌羹曖 褫撢啗骯 等檜攪 滲唳
	inline VOID		ChangeOFFSETofPTR( WORD wOffset, cOFF::eOP eCMD )
	{	M01.ChangeOFFSETofPTR( wOffset, eCMD );	}

	// [?][PTR] - ⑽衝曖 IBF
	inline VOID		ChangeCompareRoutine( WzASM_IBF_HASH_T wHASH_IBF )
	{	M04_CHECK_BOOL.ChangeHASH( wHASH_IBF );	}
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<CHECK BOOLEAN>
//
//	<EXPRESSION>
//		--PUSH GPR--
//		MEMBER* pMEMBER = &((OBJECT*)->(_OFFSET MEMBER));
//		BOOL bCHECK = CALL_HANDLER( 0, pMEMBER, 0, 0 );
//		if( bCHECK )
//		{
//			EAX = { ~, pMEMBER, _INTERVAL, ~ }
//			CALL_HANDLER( EAX );
//			--POP GPR--
//			return TRUE;
//		}
//		//if( !bCHECK ) goto _JMPFAILCASE;
//	_JMPFAILCASE:
//		--POP GPR--
//		return FALSE;
//
//	<USED REG>
//		EAX = { ~, pMEMBER, _INTERVAL, ~ }
//		EOP = { OBJECT*, sCALC(OBJECT, MEMBER) }
//		ERP = bCHECK
//		EFL
//
template<typename WzASM_IBF_HASH_T _CHK_IBF_HASH, typename WzASM_IBF_HASH_T _ACT_IBF_HASH, typename _USRCODE, typename WORD _OFFSET, typename DWORD _INTERVAL>
struct sMETHOD_INTERVAL_WORKNODE_CODES
{
	sIL<IWzASM::PUSH_EABCDX>												M01;	// STK(n)
	sIL_CHECK_BOOL<_CHK_IBF_HASH, _OFFSET, OPT::MARK01>				M02;
	// 褻勒 虜褶衛 (衛除檜 腎歷擊 陽...)
	// (ITimeout*)->SetNextTimeoutCurTickBased( Pop() );

	
	//sIL_CALL_IBF_HASH_USRINPUT<sIL<IWzASM::PUSH_EAX>, _ACT_IBF_HASH>		M04_if_is_true_than_call_IBF_handler
	struct sINTER_if_is_true_than_call_IBF_handler {
		sIL_MOV_EAX2_B2<_INTERVAL>												M01;
		sIL_CALL_IBF_HASH_USRINPUT<sIL<IWzASM::PUSH_EAX>, _ACT_IBF_HASH>		M02;
	}																		M04_if_is_true_than_call_IBF_handler;
	// User Code Block
	_USRCODE																M06;
	// 唸婁籀葬 (TRUE)
	// EBX = TRUE;
	// ERP = EBX
	struct sINTER_make_return_true_result {
		sIL<IWzASM::MOV_EBX_B1>										M01;
		sDT1<TRUE>													M02;
		sIL<IWzASM::MOV_ERP_EBX>									M03;
	}																		M09;
	sIL_MARK01																M10;
	sIL<IWzASM::POP_EABCDX>													M11;
	sIL_RET																	M12;

	inline VOID		ChangeBaseObject( WzPTR pPTR )
	{	M02.ChangeBaseObject( pPTR );	}

	inline VOID		Change_if_is_true_than_call_IBF_handler( WzASM_IBF_HASH_T wHASH_IBF )
	{	M04_if_is_true_than_call_IBF_handler.M02.ChangeHASH( wHASH_IBF );	}
};

#pragma pop_macro("STATIC")
#pragma pop_macro("CONST")
#pragma pop_macro("INLINE")
#pragma pack(pop)
