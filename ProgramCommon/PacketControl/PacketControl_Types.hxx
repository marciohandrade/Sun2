#pragma once

#include <ProgramCommon/WzDataType.h>

struct MSG_BASE;

//==================================================================================================
//
namespace nsPacket
{
	typedef PacketInfo	MsgOffset;
	#define DECL_SCONST		static const CONV_T
#pragma warning(push)
#pragma warning(disable:4244)
#pragma pack(push, 1)

//struct WzVectorPack
//{
//	SHORT	x;
//	SHORT	y;
//	SHORT	z;
//};

//==================================================================================================
//
struct WzVectorPack
{
	enum ePACK_SPEC
	{	//	USE 2BIT
		//	XX|000000
		PACK_SPEC_RESERVED		= 0x0,
		PACK_SPEC1_FP32_U16		= 0x1,	// [-15xx, 15xx], x==y==z
		PACK_SPEC2_FP32_U16		= 0x2,	// [-204x, 204x], x==y, z={ [-51x, 51x] }
	};
	enum eSPEC_DEF
	{	//	USE 2BIT
		//	00|XX|XX|XX
		SPEC1_VAL_0000			= 0x0,
		SPEC1_VAL_0512			= 0x1,
		SPEC1_VAL_1024			= 0x2,
		SPEC1_VAL_NULL			= 0x3,

		SPEC1_OFFSET_X			= 0x2,
		SPEC1_OFFSET_Y			= 0x4,
		SPEC1_OFFSET_Z			= 0x6,

		//	USE 3BIT
		//	00|XXX|XXX
		SPEC2_VAL_0000			= 0x0,
		SPEC2_VAL_0512			= 0x1,
		SPEC2_VAL_1024			= 0x2,
		SPEC2_VAL_1536			= 0x3,
		SPEC2_VAL_2048			= 0x4,
		SPEC2_OFFSET_X			= 0x2,
		SPEC2_OFFSET_Y			= 0x5,
		SPEC2_OFFSET_Z			= 0x8,	// can't use
	};

	union uCONV_FP
	{
		FLOAT	valF;
		INT		valI;
		USHORT	valW;
	};

	inline static	USHORT		FP32_16( FLOAT ft )
	{
		uCONV_FP FP;
		FP.valF = ft;

		INT s = ( (FP.valI & 0x80000000) >> 31 ) ;
		INT e = ( (FP.valI & 0x7F800000) >> 23 ) - 127;
		INT m = ( (FP.valI & 0x007FFFFF) );

		if( e < -7 )
			return 0;

		return (s<<15) | ((e+7)<<11) | (m>>(23-11));
	}

	inline static	FLOAT		FP16_32( USHORT ft )
	{
		uCONV_FP FP;
		FP.valW = ft;

		if( FP.valW == 0 )
			return 0.f;

		INT s = (( FP.valW & 0x8000 ) >> 15 );
		INT e = (( FP.valW & 0x7800 ) >> 11 ) - 7;
		INT m = (( FP.valW & 0x07FF ) );

		FP.valI = (s<<31) | ((e+127) <<23) | (m << (23-11));
		return FP.valF;
	}

	//////////////////////////////////////////////////////////////////////////
	BYTE	type;
	SHORT	x;
	SHORT	y;
	SHORT	z;
	//////////////////////////////////////////////////////////////////////////
};


#define CONV_FLOATtoSHORT( valF )		((SHORT)((valF)*100))
#define CONV_SHORTtoFLOAT( valW )		(((FLOAT)(valW))/100)

#define CHECK_ENABLE_DWORDtoWORD( valDW )		SAFE_NUMERIC_TYPECAST( DWORD, (valDW), WORD )
#define CHECK_ENABLE_WORDtoBYTE( valW )			SAFE_NUMERIC_TYPECAST( WORD, (valW), BYTE )

#define __PACK_OFFSET_PLUS( packInfo, __size )	{ packInfo.pMsg = (MSG_BASE*)( ((BYTE*)packInfo.pMsg) + __size ); packInfo.Size += __size; }

#define WRITEtoBYTE( destInfo, val )		{ *((BYTE*)destInfo.pMsg) = (BYTE)(val); __PACK_OFFSET_PLUS(destInfo, sizeof(BYTE)); }
#define WRITEtoWORD( destInfo, val )		{ *((WORD*)destInfo.pMsg) = (WORD)(val); __PACK_OFFSET_PLUS(destInfo, sizeof(WORD)); }
#define WRITEtoDWORD( destInfo, val )		{ *((DWORD*)destInfo.pMsg) = (DWORD)(val); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD)); }
#define WRITEtoDWORD64( destInfo, val )		{ *((DWORD64*)destInfo.pMsg) = (DWORD64)(val); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD64)); }
#define WRITEtoWzVector( destInfo, val )	{ *((WzVector*)destInfo.pMsg) = (val); __PACK_OFFSET_PLUS(destInfo, sizeof(WzVector)); }
#define WRITEtoWzVectorPack( destInfo, val ){ *((WzVectorPack*)destInfo.pMsg) = (val); __PACK_OFFSET_PLUS(destInfo, sizeof(WzVectorPack)); }
#define WRITEtoFLOAT( destInfo, val )		{ *((FLOAT*)destInfo.pMsg) = (val); __PACK_OFFSET_PLUS(destInfo, sizeof(FLOAT)); }
#define WRITEtoMSG_BASE( destInfo, val )	{ *((MSG_BASE*)destInfo.pMsg) = (val); __PACK_OFFSET_PLUS(destInfo, sizeof(MSG_BASE)); }
#define WRITEtoDEFTYPE( destInfo, ptrOfType, _type)	{ *((_type*)destInfo.pMsg) = *(ptrOfType); __PACK_OFFSET_PLUS(destInfo, sizeof(_type)); }

#define READ_BYTEfromBYTE( destInfo, srcInfo )		{ *((BYTE*)destInfo.pMsg) = *((BYTE*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(BYTE)); __PACK_OFFSET_PLUS(srcInfo, sizeof(BYTE)); }
#define READ_BYTEfromEMPTY( destInfo )				{ *((BYTE*)destInfo.pMsg) = 0; __PACK_OFFSET_PLUS(destInfo, sizeof(BYTE)); }
#define READ_WORDfromWORD( destInfo, srcInfo )		{ *((WORD*)destInfo.pMsg) = *((WORD*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(WORD)); __PACK_OFFSET_PLUS(srcInfo, sizeof(WORD)); }
#define READ_WORDfromBYTE( destInfo, srcInfo )		{ *((WORD*)destInfo.pMsg) = *((BYTE*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(WORD)); __PACK_OFFSET_PLUS(srcInfo, sizeof(BYTE)); }
#define READ_WORDfromEMPTY( destInfo )				{ *((WORD*)destInfo.pMsg) = 0; __PACK_OFFSET_PLUS(destInfo, sizeof(WORD)); }
#define READ_DWORDfromBYTE( destInfo, srcInfo )		{ *((DWORD*)destInfo.pMsg) = *((BYTE*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD)); __PACK_OFFSET_PLUS(srcInfo, sizeof(BYTE)); }
#define READ_DWORDfromWORD( destInfo, srcInfo )		{ *((DWORD*)destInfo.pMsg) = *((WORD*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD)); __PACK_OFFSET_PLUS(srcInfo, sizeof(WORD)); }
#define READ_DWORDfromDWORD( destInfo, srcInfo )	{ *((DWORD*)destInfo.pMsg) = *((DWORD*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD)); __PACK_OFFSET_PLUS(srcInfo, sizeof(DWORD)); }
#define READ_DWORDfromEMPTY( destInfo )				{ *((DWORD*)destInfo.pMsg) = 0; __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD)); }
#define READ_DWORD64fromDWORD64( destInfo, srcInfo ){ *((DWORD64*)destInfo.pMsg) = *((DWORD64*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD64)); __PACK_OFFSET_PLUS(srcInfo, sizeof(DWORD64)); }
#define READ_DWORD64fromEMPTY( destInfo )			{ *((DWORD64*)destInfo.pMsg) = 0; __PACK_OFFSET_PLUS(destInfo, sizeof(DWORD64)); }
#define READ_WzVectorfromEMPTY( destInfo )			{ SetVector( (WzVector*)destInfo.pMsg, .0f, .0f, .0f ); __PACK_OFFSET_PLUS(destInfo, sizeof(WzVector)); }
//#define READ_WzVectorfromWzVector( destInfo, srcInfo )	{ *((WzVector*)destInfo.pMsg) = *((WzVector*)srcInfo.pMsg); destInfo.Size += sizeof(WzVector); srcInfo.Size += sizeof(WzVector); }
//#define READ_WzVectorPackfromWzVectorPack( destInfo, srcInfo )	{ *((WzVectorPack*)destInfo.pMsg) = *((WzVectorPack*)srcInfo.pMsg); destInfo.Size += sizeof(WzVectorPack); srcInfo.Size += sizeof(WzVectorPack); }
#define READ_FLOATfromFLOAT( destInfo, srcInfo )		{ *((FLOAT*)destInfo.pMsg) = *((FLOAT*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(FLOAT)); __PACK_OFFSET_PLUS(srcInfo, sizeof(FLOAT)); }
#define READ_MSG_BASEfromMSG_BASE( destInfo, srcInfo )	{ *((MSG_BASE*)destInfo.pMsg) = *((MSG_BASE*)srcInfo.pMsg); __PACK_OFFSET_PLUS(destInfo, sizeof(MSG_BASE)); __PACK_OFFSET_PLUS(srcInfo, sizeof(MSG_BASE)); }
#define READ_DEFTYPEfromDEFTYPE( destInfo, srcInfo, _type)	{ *((_type*)destInfo.pMsg) = *(_type*)srcInfo.pMsg; __PACK_OFFSET_PLUS(destInfo, sizeof(_type)); __PACK_OFFSET_PLUS(srcInfo, sizeof(_type)); }

// pack info를 읽어들이는 기능: dest값 주의할 것.
#define READ_CONV_T( destValue, srcInfo )	{ destValue = *(CONV_T*)(srcInfo.pMsg); __PACK_OFFSET_PLUS(srcInfo, sizeof(CONV_T)); }
#define WRITE_CONV_T( destInfo, val )		{ *(CONV_T*)(destInfo.pMsg) = (CONV_T)val; __PACK_OFFSET_PLUS(destInfo, sizeof(CONV_T)); }

enum ePACKED_OBJECT_KEYTYPE
{
	OBJECT_KEYTYPE_ETC		= 0x00,	// 4B
	OBJECT_KEYTYPE_PLAYER	= 0x01,	// 2B
	OBJECT_KEYTYPE_MOB		= 0x02,	// 2B
	OBJECT_KEYTYPE_ITEM		= 0x03,	// 2B
	OBJECT_KEYTYPE_MASK		= OBJECT_KEYTYPE_ETC | OBJECT_KEYTYPE_PLAYER | OBJECT_KEYTYPE_MOB | OBJECT_KEYTYPE_ITEM ,
};

//inline static VOID	CHECK_OBJECTKEY( BYTE& __rOutputType, const DWORD __objectKey )
#define CHECK_OBJECTKEY( __rOutputType, __objectKey )					\
{																		\
	__rOutputType =														\
		( (PLAYER_OBJECT_KEY <= __objectKey) && (__objectKey <= MONSTER_OBJECT_KEY) ) ?	OBJECT_KEYTYPE_PLAYER	\
		: ( (MONSTER_OBJECT_KEY < __objectKey) && (__objectKey <= NPC_OBJECT_KEY) ) ?	OBJECT_KEYTYPE_MOB		\
		: ( (ITEM_OBJECT_KEY < __objectKey) && (__objectKey <= PET_OBJECT_KEY) ) ?		OBJECT_KEYTYPE_ITEM		\
		:																				OBJECT_KEYTYPE_ETC		\
		;	}

//inline static VOID	WRITEtoOBJECTKEY( MsgOffset& INOUT destInfo, const DWORD __objectKey, BYTE& __rInputType )
#define WRITEtoOBJECTKEY( destInfo, __objectKey, __rInputType )						\
{																					\
	switch( __rInputType )	{														\
	case OBJECT_KEYTYPE_PLAYER:	{													\
			*(WORD*)destInfo.pMsg = (WORD)(__objectKey - PLAYER_OBJECT_KEY);		\
			__PACK_OFFSET_PLUS( destInfo, sizeof(WORD) );							\
		} break;																	\
	case OBJECT_KEYTYPE_MOB:	{													\
			*(WORD*)destInfo.pMsg = (WORD)(__objectKey - MONSTER_OBJECT_KEY);		\
			__PACK_OFFSET_PLUS( destInfo, sizeof(WORD) );							\
		} break;																	\
	case OBJECT_KEYTYPE_ITEM:	{													\
			*(WORD*)destInfo.pMsg = (WORD)(__objectKey - ITEM_OBJECT_KEY);			\
			__PACK_OFFSET_PLUS( destInfo, sizeof(WORD) );							\
		} break;																	\
	case OBJECT_KEYTYPE_ETC:	{													\
			*(DWORD*)destInfo.pMsg = (DWORD)(__objectKey);							\
			__PACK_OFFSET_PLUS( destInfo, sizeof(DWORD) );							\
		} break;																	\
	}																				\
}

//inline static VOID	READ_OBJECTKEY( MsgOffset& INOUT destInfo, MsgOffset& IN srcInfo, BYTE& __rInputType )
#define READ_OBJECTKEY( destInfo, srcInfo, __rInputType )									\
{																							\
	switch( __rInputType )	{																\
	case OBJECT_KEYTYPE_PLAYER:	{															\
			*(DWORD*)destInfo.pMsg = ((DWORD)*(WORD*)srcInfo.pMsg + PLAYER_OBJECT_KEY);		\
			__PACK_OFFSET_PLUS( destInfo, sizeof(DWORD) );									\
			__PACK_OFFSET_PLUS( srcInfo, sizeof(WORD) );									\
		} break;																			\
	case OBJECT_KEYTYPE_MOB:	{															\
			*(DWORD*)destInfo.pMsg = ((DWORD)*(WORD*)srcInfo.pMsg + MONSTER_OBJECT_KEY);	\
			__PACK_OFFSET_PLUS( destInfo, sizeof(DWORD) );									\
			__PACK_OFFSET_PLUS( srcInfo, sizeof(WORD) );									\
		} break;																			\
	case OBJECT_KEYTYPE_ITEM:	{															\
			*(DWORD*)destInfo.pMsg = ((DWORD)*(WORD*)srcInfo.pMsg + ITEM_OBJECT_KEY);		\
			__PACK_OFFSET_PLUS( destInfo, sizeof(DWORD) );									\
			__PACK_OFFSET_PLUS( srcInfo, sizeof(WORD) );									\
		} break;																			\
	case OBJECT_KEYTYPE_ETC:	{															\
			*(DWORD*)destInfo.pMsg = (DWORD)(*(DWORD*)srcInfo.pMsg);						\
			__PACK_OFFSET_PLUS( destInfo, sizeof(DWORD) );									\
			__PACK_OFFSET_PLUS( srcInfo, sizeof(DWORD) );									\
		} break;																			\
	}																						\
}


struct WzVectorPackT1 : public WzVectorPack
{
	// WzVector -> WzVectorPackT1
	inline static VOID		Pack( MsgOffset& INOUT destInfo, WzVector* pVec )
	{
#define SPEC1_FLOAT_RANGE_TYPE_CALC( _value_ )		\
	((_value_) < -1024) || (1024 < (_value_)) ?		WzVectorPack::SPEC1_VAL_1024	\
	: ((_value_) < -512) || (512 < (_value_)) ?		WzVectorPack::SPEC1_VAL_0512	\
	:												WzVectorPack::SPEC1_VAL_0000

#define SPEC1_FLOAT_OFFSET_MINUS( _value_, _rngtype_ )						\
	( WzVectorPack::SPEC1_VAL_0000 == _rngtype_ ) ?		_value_				\
	: ( WzVectorPack::SPEC1_VAL_0512 == _rngtype_ ) ?	((_value_)<0 ? (_value_)+512.f : (_value_)-512.f)		\
	:													((_value_)<0 ? (_value_)+1024.f : (_value_)-1024.f)

		WzVectorPackT1& rDEST = *(WzVectorPackT1*)destInfo.pMsg;

		rDEST.type = WzVectorPack::PACK_SPEC1_FP32_U16;
		{
			FLOAT x = pVec->x;
			BYTE type = SPEC1_FLOAT_RANGE_TYPE_CALC( x );
			rDEST.type |= (type<<WzVectorPack::SPEC1_OFFSET_X);
			x = SPEC1_FLOAT_OFFSET_MINUS( x, type );
			rDEST.x = WzVectorPack::FP32_16( x );
		};
		{
			FLOAT y = pVec->y;
			BYTE type = SPEC1_FLOAT_RANGE_TYPE_CALC( y );
			rDEST.type |= (type<<WzVectorPack::SPEC1_OFFSET_Y);
			y = SPEC1_FLOAT_OFFSET_MINUS( y, type );
			rDEST.y = WzVectorPack::FP32_16( y );
		};
		{
			FLOAT z = pVec->z;
			BYTE type = SPEC1_FLOAT_RANGE_TYPE_CALC( z );
			rDEST.type |= (type<<WzVectorPack::SPEC1_OFFSET_Z);
			z = SPEC1_FLOAT_OFFSET_MINUS( z, type );
			rDEST.z = WzVectorPack::FP32_16( z );
		};

		__PACK_OFFSET_PLUS( destInfo, sizeof(rDEST) );
	}

	// WzVectorPackT1 -> WzVector
	inline static VOID		UnPack( MsgOffset& INOUT destInfo, MsgOffset& IN srcInfo )
	{
#define SPEC1_FLOAT_OFFSET_PLUS( _value_, _rngtype_ )						\
	( WzVectorPack::SPEC1_VAL_0000 == _rngtype_ ) ?		_value_				\
	: ( WzVectorPack::SPEC1_VAL_0512 == _rngtype_ ) ?	((_value_)<0 ? (_value_)-512.f : (_value_)+512.f)		\
	:													((_value_)<0 ? (_value_)-1024.f : (_value_)+1024.f)

		WzVector& rDEST = *(WzVector*)destInfo.pMsg;
		WzVectorPackT1& rSRC = *(WzVectorPackT1*)srcInfo.pMsg;

		//rTHIS.type = WzVectorPack::PACK_SPEC1_FP32_U16;
		BYTE type_hdr = rSRC.type;
		{
			FLOAT x = WzVectorPack::FP16_32( rSRC.x );
			BYTE type = (type_hdr>>WzVectorPack::SPEC1_OFFSET_X)&0x3;
			x = SPEC1_FLOAT_OFFSET_PLUS( x, type );
			rDEST.x = x;
		};
		{
			FLOAT y = WzVectorPack::FP16_32( rSRC.y );
			BYTE type = (type_hdr>>WzVectorPack::SPEC1_OFFSET_Y)&0x3;
			y = SPEC1_FLOAT_OFFSET_PLUS( y, type );
			rDEST.y = y;
		};
		{
			FLOAT z = WzVectorPack::FP16_32( rSRC.z );
			BYTE type = (type_hdr>>WzVectorPack::SPEC1_OFFSET_Z)&0x3;
			z = SPEC1_FLOAT_OFFSET_PLUS( z, type );
			rDEST.z = z;
		};

		__PACK_OFFSET_PLUS( destInfo, sizeof(rDEST) );
		__PACK_OFFSET_PLUS( srcInfo, sizeof(rSRC) );
	}
};

struct WzVectorPackT2 : public WzVectorPack
{
	// WzVector -> WzVectorPackT1
	inline static VOID		Pack( MsgOffset& INOUT destInfo, WzVector* pVec )
	{
#define SPEC2_FLOAT_RANGE_TYPE_CALC( _value_ )		\
	((_value_) < -2048) || (2048 < (_value_)) ?		WzVectorPack::SPEC2_VAL_2048	\
	: ((_value_) < -1536) || (1536 < (_value_)) ?	WzVectorPack::SPEC2_VAL_1536	\
	: ((_value_) < -1024) || (1024 < (_value_)) ?	WzVectorPack::SPEC2_VAL_1024	\
	: ((_value_) < -512) || (512 < (_value_)) ?		WzVectorPack::SPEC2_VAL_0512	\
	:												WzVectorPack::SPEC2_VAL_0000

#define SPEC2_FLOAT_OFFSET_MINUS( _value_, _rngtype_ )						\
	( WzVectorPack::SPEC2_VAL_0000 == _rngtype_ ) ?		_value_				\
	: ( WzVectorPack::SPEC2_VAL_0512 == _rngtype_ ) ?	((_value_)<0 ? (_value_)+512.f : (_value_)-512.f)		\
	: ( WzVectorPack::SPEC2_VAL_1024 == _rngtype_ ) ?	((_value_)<0 ? (_value_)+1024.f : (_value_)-1024.f)		\
	: ( WzVectorPack::SPEC2_VAL_1536 == _rngtype_ ) ?	((_value_)<0 ? (_value_)+1536.f : (_value_)-1536.f)		\
	:													((_value_)<0 ? (_value_)+2048.f : (_value_)-2048.f)

		WzVectorPackT2& rDEST = *(WzVectorPackT2*)destInfo.pMsg;

		rDEST.type = WzVectorPack::PACK_SPEC2_FP32_U16;
		{
			FLOAT x = pVec->x;
			BYTE type = SPEC2_FLOAT_RANGE_TYPE_CALC( x );
			rDEST.type |= (type<<WzVectorPack::SPEC2_OFFSET_X);
			x = SPEC2_FLOAT_OFFSET_MINUS( x, type );
			rDEST.x = WzVectorPack::FP32_16( x );
		};
		{
			FLOAT y = pVec->y;
			BYTE type = SPEC2_FLOAT_RANGE_TYPE_CALC( y );
			rDEST.type |= (type<<WzVectorPack::SPEC2_OFFSET_Y);
			y = SPEC2_FLOAT_OFFSET_MINUS( y, type );
			rDEST.y = WzVectorPack::FP32_16( y );
		};
		{
			FLOAT z = pVec->z;
			//BYTE type = SPEC2_FLOAT_RANGE_TYPE_CALC( z );
			//rDEST.type |= (type<<WzVectorPack::SPEC2_OFFSET_Z);
			//z = SPEC2_FLOAT_OFFSET_MINUS( z, type );
			rDEST.z = WzVectorPack::FP32_16( z );
		};

		__PACK_OFFSET_PLUS( destInfo, sizeof(rDEST) );
	}

	// WzVectorPackT2 -> WzVector
	inline static VOID		UnPack( MsgOffset& INOUT destInfo, MsgOffset& IN srcInfo )
	{
#define SPEC2_FLOAT_OFFSET_PLUS( _value_, _rngtype_ )						\
	( WzVectorPack::SPEC2_VAL_0000 == _rngtype_ ) ?		_value_				\
	: ( WzVectorPack::SPEC2_VAL_0512 == _rngtype_ ) ?	((_value_)<0 ? (_value_)-512.f : (_value_)+512.f)		\
	: ( WzVectorPack::SPEC2_VAL_1024 == _rngtype_ ) ?	((_value_)<0 ? (_value_)-1024.f : (_value_)+1024.f)		\
	: ( WzVectorPack::SPEC2_VAL_1536 == _rngtype_ ) ?	((_value_)<0 ? (_value_)-1536.f : (_value_)+1536.f)		\
	:													((_value_)<0 ? (_value_)-2048.f : (_value_)+2048.f)

		WzVector& rDEST = *(WzVector*)destInfo.pMsg;
		WzVectorPackT2& rSRC = *(WzVectorPackT2*)srcInfo.pMsg;

		//rTHIS.type = WzVectorPack::PACK_SPEC1_FP32_U16;
		BYTE type_hdr = rSRC.type;
		{
			FLOAT x = WzVectorPack::FP16_32( rSRC.x );
			BYTE type = (type_hdr>>WzVectorPack::SPEC2_OFFSET_X)&0x7;
			x = SPEC2_FLOAT_OFFSET_PLUS( x, type );
			rDEST.x = x;
		};
		{
			FLOAT y = WzVectorPack::FP16_32( rSRC.y );
			BYTE type = (type_hdr>>WzVectorPack::SPEC2_OFFSET_Y)&0x7;
			y = SPEC2_FLOAT_OFFSET_PLUS( y, type );
			rDEST.y = y;
		};
		{
			FLOAT z = WzVectorPack::FP16_32( rSRC.z );
			//BYTE type = (type_hdr>>WzVectorPack::SPEC2_OFFSET_Z)&0x7;
			//z = SPEC2_FLOAT_OFFSET_PLUS( z, type );
			rDEST.z = z;
		};

		__PACK_OFFSET_PLUS( destInfo, sizeof(rDEST) );
		__PACK_OFFSET_PLUS( srcInfo, sizeof(rSRC) );
	}
};

//#define WRITEtoWzVectorPackFromWzVector( destInfo, _pVec )	\
//	WzVectorPackT1::Pack( destInfo, _pVec )
//
//#define READ_WzVectorfromWzVectorPack( destInfo, srcInfo )	\
//	WzVectorPackT1::UnPack( destInfo, srcInfo )

//static VOID		WRITEtoWzVectorPackFromWzVector( MsgOffset& INOUT destInfo, WzVector* IN _pVec )
#define WRITEtoWzVectorPackFromWzVector( destInfo, _pVec )			\
	{																\
		WzVector* __pvector = _pVec;								\
		if( ( (__pvector->x < -1536) || (1536 < __pvector->x) )		\
			|| ( (__pvector->y < -1536) || (1536 < __pvector->y) )	\
			)														\
			WzVectorPackT2::Pack( destInfo, __pvector );				\
		else														\
			WzVectorPackT1::Pack( destInfo, __pvector );			\
	}

#define READ_WzVectorfromWzVectorPack( destInfo, srcInfo )		\
{																\
	BYTE type = *(BYTE*)srcInfo.pMsg;							\
	switch( type&0x3 )											\
	{															\
	case WzVectorPack::PACK_SPEC1_FP32_U16:						\
        WzVectorPackT1::UnPack( destInfo, srcInfo );			\
		break;													\
	case WzVectorPack::PACK_SPEC2_FP32_U16:						\
		WzVectorPackT2::UnPack( destInfo, srcInfo );			\
		break;													\
	}															\
}

////Conv_WzVector_to_WzVectorPack( MsgOffset& INOUT destInfo, WzVector* IN src )
//static VOID		WRITEtoWzVectorPackFromWzVector( MsgOffset& INOUT destInfo, WzVector* IN src )
//{
//	WzVectorPack& pack = *(WzVectorPack*)destInfo.pMsg;
//	pack.x = CONV_FLOATtoSHORT( src->x );
//	pack.y = CONV_FLOATtoSHORT( src->y );
//	pack.z = CONV_FLOATtoSHORT( src->z );
//	__PACK_OFFSET_PLUS( destInfo, sizeof(pack) );
//}
//
////Conv_WzVectorPack_to_WzVector( MsgOffset& INOUT destInfo, WzVectorPack* IN src )
//static VOID		READ_WzVectorfromWzVectorPack( MsgOffset& INOUT destInfo, MsgOffset& IN srcInfo )
//{
//	WzVector& dest = *(WzVector*)destInfo.pMsg;
//	WzVectorPack& src = *(WzVectorPack*)srcInfo.pMsg;
//	dest.x = CONV_SHORTtoFLOAT( src.x );
//	dest.y = CONV_SHORTtoFLOAT( src.y );
//	dest.z = CONV_SHORTtoFLOAT( src.z );
//	__PACK_OFFSET_PLUS( destInfo, sizeof(dest) );
//	__PACK_OFFSET_PLUS( srcInfo, sizeof(src) );
//}

#pragma pack(pop)
#pragma warning(pop)
}; //end of namespace


