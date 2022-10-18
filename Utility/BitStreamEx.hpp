#pragma once

#if SUN_CODE_BACKUP
//=============================================================================================================================
/// BitStreamEx class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 11. 12
	@remark
		- BitStream �� Pack & Unpack ���� ���� ���
	@note
		- Metadata�� �̿��� ���� Ȯ�强 �ְ� ������ ������ �����.
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

// sPACK_BIT_METAINFO �ڷᱸ�� ���� (����)
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
// ������ �̷��� ��Ÿ������ ������ ���� ����ü�� ���δ�.
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
// ��ŷ�Ǵ� ������ BYTE�� �迭�� ����� ��.
//
// <WARNING>
//   ���� ������ ���� ������ ���ǵǾ� ���� ��� (#pragma pack���� ���� ����ü)
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
//   �̰��� ��Ÿ������ ������ ���� ���̴�.
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
// <��� ����>
//		----------------------------------------------------
//		bitStream.SetMetaInfo( c_PACK_METAINFO );
//		----------------------------------------------------
//		bitStream.Pack( (BYTE*)&s_SOURCE_STRUCT );
//		int readed = bitStream.GetNumberOfBitsUsed();
//		bitStream.SetReadOffSet( 0 );
//		bitStream.UnPack( (BYTE*)&s_SOURCE_STRUCT, sizeof(s_SOURCE_STRUCT) );
//
// <Ȯ�� ���>
//		bitStream.SetMetaInfo( c_PACK_METAINFO );
//		bitStream.Pack( (BYTE*)&s_SOURCE_STRUCT );
//		int readed = bitStream.GetNumberOfBitsUsed();
//		bitStream.SetReadOffSet( 0 );
//		----------------------------------------------------
//		bitStream.SetMetaInfo( c_PACK_METAINFO2 );
//		----------------------------------------------------
//		bitStream.UnPack( (BYTE*)&s_SOURCE_STRUCT2, sizeof(s_SOURCE_STRUCT2) );
//
//   -> ����� <��� ����>�� ������, s_SOURCE_STRUCT2�� ���� ���� ������ ���� ������ �ʴ� �κ��� �����
//   �� �ְ� �ȴ�.
//   -> �Ǵ� �ƿ�, ���� ũ�⸦ ����� ������ ������ ���� ���� ���̴�.
//	 -> �ϴ� ������ ����
//      METANODE_4PACK( DWORD, 0 ),
//      �ϳ��� �ʵ带 �߰��� ������ �θ�, ���� �۾��� �� ������ �ǳʶ� �۾��� ������ �� �ְ�, ���߿�
//      ��� ��Ʈ ���� �Է��ϰ� �̿� ���� �����͸� �غ��ϸ�, Ȯ��� ������ ���ܳ� ���̴�.
//
//      ��) Ư�� ����ü�� ������ ���� �ٸ� ó���� �ϰ� �ϰ� �ʹ�. ������ �������� ���� ������ ���� ����ü����.
//      1. ���� ���� �ʵ� �ϳ��� �߰��Ѵ�. METANODE_4PACK( BYTE, 0 ), // ���� ����
//		2. PackedData�� Unpack���� �߰��� ��Ÿ������ ����� ����ü�� �о���δ�.
//		3. ���ο� ����ü���� ������ ������ BYTE�� �ǳʶٰ� ������ ��� ���̴�.
//		4. ��Ÿ������ ����: METANODE_4PACK( BYTE, 4 )	// ���� ����, ���⼭�� 4��Ʈ�� ó���Ѵ�.
//		5. ���� ������ �����Ѵ�.
//		6. �ٽ� Pack�Ѵ�.
//		7. Pack�� ������ ���� ������ ���ȵ� ���ο� �����Ͱ� �� ���̴�.
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
	//   pPackingStorage : BYTE*	=-> MetaInfo�� �̿��� Packing�� ����
	//   wUsedBytes : WORD&			=-> ���� byte ���̰� �� ���̴�.
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
	//   pUnpackingStorage : BYTE*	=-> MetaInfo�� �̿��� Packing�� ������ UnPack�ϴ� ����
	//   bTF : BOOL					=-> pUnpackingStorage�� �ʱ�ȭ �� �������� ����,
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

	// �Ʒ��� ���� ��ɵ� �� ���̴�. ������ ��ȯ �������� ������ ���̹Ƿ� �������� �ʱ�� �Ѵ�.
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