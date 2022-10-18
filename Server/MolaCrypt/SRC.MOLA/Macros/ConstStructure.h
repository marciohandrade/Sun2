#pragma once

#pragma pack(push, 1)
#pragma warning(push)
#pragma warning(disable:4307)
#pragma warning(disable:4308)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <UTILITIES>
#define ALIGN32BYTE(size)			(((size) + 0x1F) & ~0x1F)
#define RANGE_RND_064_128(value)	( (((value)&(128-1))) | 64 )
#define RANGE_RND_000_004(value)	((value)&(4-1))

#define GET_ARRAY_SIZE(type, array)		((DWORD)sizeof(array)/sizeof(type))
#define EXPORTER(p_fn)				do { PVOID p = ((PVOID)&p_fn); } while(0)

//	CPSTRUCT - Crypt Packet STRUCTure
typedef struct CPSTRUCT*		PCPSTRUCT;
typedef struct MSG_STRUCT*		PMSG_STRUCT;

struct MSG_BASE_CRYPT
{
	BYTE	SNKEY;
	BYTE	CRC;
};

struct MSG_STRUCT
{
	union uPSTREAM
	{
		struct MSG_BASE
		{
			BYTE	CATEGORY;
			BYTE	PROTOCOL;
		};
		BYTE	pSTREAM[2];
	};

	inline static PMSG_STRUCT	ToMSG( BYTE* pStream )
	{
		return (PMSG_STRUCT)(pStream);
	}

	inline static BYTE*			ToBYTE( PMSG_STRUCT pMsg )
	{
		return (BYTE*)(pMsg);
	}
};

struct CPSTRUCT
{
	MSG_BASE_CRYPT		CRYPT_HDR;
	MSG_STRUCT			CPMSG;

	inline static PCPSTRUCT		ToCRYPTHDR( BYTE* pStream )
	{
		return (PCPSTRUCT)(pStream-sizeof(MSG_BASE_CRYPT));
	}

	inline static BYTE*			ToMESSAGE( PCPSTRUCT pStream )
	{
		return (BYTE*)&(pStream->CPMSG);
	}
};

//#if		(HEX_KEY_1_GENERATOR_001 == 0)
//#elif	(HEX_KEY_1_GENERATOR_001 == 1)
//#elif	(HEX_KEY_1_GENERATOR_001 == 2)
//#elif	(HEX_KEY_1_GENERATOR_001 == 3)
//#elif	(HEX_KEY_1_GENERATOR_001 == 4)
//#elif	(HEX_KEY_1_GENERATOR_001 == 5)
//#elif	(HEX_KEY_1_GENERATOR_001 == 6)
//#elif	(HEX_KEY_1_GENERATOR_001 == 7)
//#elif	(HEX_KEY_1_GENERATOR_001 == 8)
//#elif	(HEX_KEY_1_GENERATOR_001 == 9)
//#elif	(HEX_KEY_1_GENERATOR_001 == 10)
//#elif	(HEX_KEY_1_GENERATOR_001 == 11)
//#elif	(HEX_KEY_1_GENERATOR_001 == 12)
//#elif	(HEX_KEY_1_GENERATOR_001 == 13)
//#elif	(HEX_KEY_1_GENERATOR_001 == 14)
//#elif	(HEX_KEY_1_GENERATOR_001 == 15)
//#endif	//(HEX_KEY_1_GENERATOR_001 == n)

#pragma warning(pop)
#pragma pack(pop)

