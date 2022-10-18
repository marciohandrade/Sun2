#pragma once

#if SUN_CODE_BACKUP
//=============================================================================================================================
/// BitStreamEx class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 11. 12
	@remark
		- BitStream 의 Pack & Unpack 관련 개선 방안
	@note
		- Metadata를 이용한 좀더 확장성 있고 유연한 구조를 만든다.
	@history 
	- 
*/
//=============================================================================================================================

#include "BitStream.hpp"

#pragma pack( push, 1 )

struct sPACK_BIT_METAINFO
{
	BYTE	BIT_SIZE;
	BYTE	USED_BITS;
};

#define METANODE_4PACK( TypeName, UsedBits )		\
	{ BYTES_TO_BITS(sizeof(TypeName)), UsedBits }

#define METANODE_4PACK_EOR()	\
	{ 0, 0 }

// sPACK_BIT_METAINFO 자료구조 정의 (예제)
//
//	const sPACK_BIT_METAINFO	c_PACK_METAINFO =
//	{
//		METANODE_4PACK( BYTE, 3 ),
//		METANODE_4PACK( DWORD, 9 ),
//		METANODE_4PACK( WORD, 7 ),
//		METANODE_4PACK( BOOL, 1 ),
//		METANODE_4PACK( DWORD, 30 ),
//		METANODE_4PACK_EOR()
//	};
//
// 다음은 이러한 메타정보를 추출한 원본 구조체를 보인다.
//	#pragma pack( push, 1 )
//	struct sSOURCE_STRUCT s_SOURCE_STRUCT =
//	{
//		BYTE	m_byField1;
//		DWORD	m_dwField2;
//		WORD	m_wField3;
//		BOOL	m_bField4;
//		DWORD	m_dwField5;
//	};
//	#pragma pack( pop )
//
// 패킹되는 정보는 BYTE의 배열로 충분할 듯.
//
// <WARNING>
//   만약 다음과 같은 구조가 정의되어 있을 경우 (#pragma pack되지 않은 구조체)
//
//	struct sSOURCE_STRUCT s_SOURCE_STRUCT2 =
//	{
//		BYTE	m_byField1;
//		DWORD	m_dwField2;
//		WORD	m_wField3;
//		BOOL	m_bField4;
//		DWORD	m_dwField5;
//	};
//
//   이것의 메타정보는 다음과 같을 것이다.
//	const sPACK_BIT_METAINFO	c_PACK_METAINFO2 =
//	{
//		METANODE_4PACK( BYTE, 3 ),
//		METANODE_4PACK( BYTE, 0 ),
//		METANODE_4PACK( BYTE, 0 ),
//		METANODE_4PACK( BYTE, 0 ),
//		METANODE_4PACK( DWORD, 9 ),
//		METANODE_4PACK( WORD, 7 ),
//		METANODE_4PACK( WORD, 0 ),
//		METANODE_4PACK( BOOL, 1 ),
//		METANODE_4PACK( DWORD, 30 ),
//		METANODE_4PACK_EOR()
//	};
//
// <Application>
// <사용 예제>
//		----------------------------------------------------
//		bitStream.SetMetaInfo( c_PACK_METAINFO );
//		----------------------------------------------------
//		bitStream.Pack( (BYTE*)&s_SOURCE_STRUCT );
//		int readed = bitStream.GetNumberOfBitsUsed();
//		bitStream.SetReadOffSet( 0 );
//		bitStream.UnPack( (BYTE*)&s_SOURCE_STRUCT, sizeof(s_SOURCE_STRUCT) );
//
// <확장 방안>
//		bitStream.SetMetaInfo( c_PACK_METAINFO );
//		bitStream.Pack( (BYTE*)&s_SOURCE_STRUCT );
//		int readed = bitStream.GetNumberOfBitsUsed();
//		bitStream.SetReadOffSet( 0 );
//		----------------------------------------------------
//		bitStream.SetMetaInfo( c_PACK_METAINFO2 );
//		----------------------------------------------------
//		bitStream.UnPack( (BYTE*)&s_SOURCE_STRUCT2, sizeof(s_SOURCE_STRUCT2) );
//
//   -> 결과는 <사용 예제>와 같지만, s_SOURCE_STRUCT2의 실제 구조 수정을 통해 사용되지 않던 부분을 사용할
//   수 있게 된다.
//   -> 또는 아예, 기존 크기를 벗어나는 공간을 삽입할 수도 있을 것이다.
//	 -> 일단 다음과 같이
//      METANODE_4PACK( DWORD, 0 ),
//      하나의 필드를 중간에 삽입해 두면, 기존 작업은 그 공간을 건너뛴 작업을 수행할 수 있고, 나중에
//      사용 비트 수를 입력하고 이에 대한 데이터를 준비하면, 확장된 공간이 생겨날 것이다.
//
//      예) 특정 구조체에 버전에 따라 다른 처리를 하게 하고 싶다. 하지만 기존에는 버전 정보가 없는 구조체였다.
//      1. 위와 같이 필드 하나를 추가한다. METANODE_4PACK( BYTE, 0 ), // 버전 정보
//		2. PackedData를 Unpack시켜 추가된 메타정보가 적용된 구조체로 읽어들인다.
//		3. 새로운 구조체에는 버전용 공간인 BYTE가 건너뛰고 정보가 담길 것이다.
//		4. 메타정보의 수정: METANODE_4PACK( BYTE, 4 )	// 버전 정보, 여기서는 4비트로 처리한다.
//		5. 버전 정보를 삽입한다.
//		6. 다시 Pack한다.
//		7. Pack된 정보는 버전 공간이 감안된 새로운 데이터가 될 것이다.
//

#pragma pack( pop )

//
// class BitStreamEx
//
class BitStreamEx : public BitStream
{
public:
	BitStreamEx( BYTE* IN OUT pStorage, WORD wCountOfBytes, const sPACK_BIT_METAINFO* IN pMetaInfo ) :
	  BitStream( pStorage, wCountOfBytes, TRUE ), m_pMetaInfo( (sPACK_BIT_METAINFO*)pMetaInfo ) {}
	~BitStreamEx() {}

public:
	// <INTERFACEs>
	inline VOID		SetMetaInfo( const sPACK_BIT_METAINFO* IN pMetaInfo )
	{
		m_pMetaInfo = (sPACK_BIT_METAINFO*)pMetaInfo;
		//DWORD dwByteCount = GetBytesByMetaInfo();
	}

	//
	// <BitStreamEx::Pack>
	// <Arguments>
	//   pPackingStorage : BYTE*	=-> MetaInfo를 이용해 Packing할 정보
	//   wUsedBytes : WORD&			=-> 사용된 byte 길이가 될 것이다.
	//
	VOID		Pack( BYTE* IN pPackingStorage )
	{
		sPACK_BIT_METAINFO* pMETA = m_pMetaInfo;
		BYTE* pSrc = (BYTE*)pPackingStorage;

		DWORD dwByteCount = 0;
		DWORD dwBitCount;

		while( dwBitCount = (DWORD)pMETA->BIT_SIZE )
		{
			DWORD dwBitRange = (DWORD)pMETA->USED_BITS;
			if( dwBitRange )
			{
				BYTE* pSrcPtr = &pSrc[dwByteCount];
				Write( pSrcPtr, dwBitRange );
			}

			dwByteCount += dwBitCount>>3;

			++pMETA;
		}
	}

	//
	// <BitStreamEx::UnPack>
	// <Arguments>
	//   pUnpackingStorage : BYTE*	=-> MetaInfo를 이용해 Packing된 정보를 UnPack하는 공간
	//   bTF : BOOL					=-> pUnpackingStorage을 초기화 할 것인지의 여부,
	//
	VOID	UnPack( BYTE* OUT pUnpackingStorage, WORD wTypeSize, BOOL bTF = TRUE )
	{
		if( bTF )
			memset( pUnpackingStorage, 0, wTypeSize );

		sPACK_BIT_METAINFO* pMETA = m_pMetaInfo;
		BYTE* pTarget = (BYTE*)pUnpackingStorage;

		DWORD dwByteCount = 0;
		DWORD dwBitOffset = 0;

		DWORD dwBitCount;

		while( dwBitCount = pMETA->BIT_SIZE )
		{
			DWORD dwBitRange = (DWORD)pMETA->USED_BITS;
			if( dwBitRange )
			{
				BYTE* pTargetPtr = &pTarget[dwByteCount];
				Read( pTargetPtr, dwBitRange );
			}

			dwByteCount += dwBitCount>>3;

			++pMETA;
		}
	}

	// 아래와 같은 기능도 될 것이다. 하지만 변환 툴에서나 유용한 것이므로 구현하지 않기로 한다.
	//VOID	ConvertPacked( BYTE* OUT pUnpackingStorage, const sPACK_BIT_METAINFO* pConvertMetaInfo );
	//VOID	ConvertUnPacked( BYTE* OUT pUnpackingStorage, const sPACK_BIT_METAINFO* pConvertMetaInfo );

	DWORD	GetBytesByMetaInfo( const sPACK_BIT_METAINFO* pMetaInfo = NULL )
	{
		sPACK_BIT_METAINFO* pMETA;
		pMETA = pMetaInfo ? pMetaInfo : m_pMetaInfo;

		DWORD dwBitCount;
		DWORD dwBitSum = 0;

		while( dwBitCount = pMETA->BIT_SIZE )
		{
			DWORD dwBitRange = (DWORD)pMETA->USED_BITS;
			dwBitSum += dwBitRange;
		}

		DWORD dwByteCount = BITS_TO_BYTES(dwBitSum);

		return dwByteCount;
	}

private:
	sPACK_BIT_METAINFO*		m_pMetaInfo;
};

#endif //SUN_CODE_BACKUP