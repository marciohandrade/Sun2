#ifndef _RND_SNKEY_H	
#define _RND_SNKEY_H

#ifndef __MOLA_VERSION__
#	define __MOLA_VERSION__	1010
//	#define __MOLA_VERSION__	1000
#endif __MOLA_VERSION__

#define MAKE_MOLA_VERSION_( major, minor, devel )	\
	( (major<<24) | (minor<<16) | (devel&0xFFFF) )
//( ( ((BYTE)~(major)-1) << 24 ) | ( ( ((BYTE)~minor)-1) << 16 ) | (devel&0xFFFF) )

#ifdef _MOLA_EXPORTS
#define EXPORTS_API		__declspec(dllexport)
#else //
#define EXPORTS_API
#endif //

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// <TYPE DEFINITIONS>
#ifndef SECURE_VARIABLE_SIZE_BLOCK
#define SECURE_VARIABLE_SIZE_BLOCK

struct sSECURE_VARIABLE_SIZE_BLOCK
{
	BYTE	m_byLength;
	BYTE	m_pSTREAM[1];
};

#endif //SECURE_VARIABLE_SIZE_BLOCK

typedef DWORD	SNKeyType;
//
//////////////////////////////////////////////////////////////////////////


class EXPORTS_API RandomSNKey
{
public:
	static const SNKeyType c_dwBaseKey	= 0;
	static const SNKeyType c_dwPairKey	= 1;
public:
	RandomSNKey();
	// Init & Duplex-Communication Random Key
	VOID	Initialize( DWORD userKey, BYTE majorVer, BYTE minorVer );
	VOID	KeyExchange( DWORD userKey );

	BYTE	GetSolarSNKey( DWORD keyType )
	{
		BYTE bySolarKey = (c_dwBaseKey == keyType) ? m_bySolarBaseKey : m_bySolarPairKey;
		return bySolarKey;
	};
	BYTE	GetLunarSNKey( DWORD keyType )
	{
		BYTE byLunarKey = (c_dwBaseKey == keyType) ? m_byLunarBaseKey : m_byLunarPairKey;
		return byLunarKey;
	}

	VOID	NextRoundSNKey4Solar();
	VOID	NextRoundSNKey4Lunar();

private:
	DWORD	m_dwUserKey;
	DWORD	m_dwVersion;		// Fake
	DWORD	m_dwSolarKeyValue;
	DWORD	m_dwSolarKeyInc;
	DWORD	m_dwLunarKeyValue;
	DWORD	m_dwLunarKeyInc;

#pragma pack(push,1)
	struct
	{
		BYTE	m_bySolarBaseKey;
		BYTE	m_bySolarPairKey;
		BYTE	m_byLunarBaseKey;
		BYTE	m_byLunarPairKey;
	};
#pragma pack(pop)

public:
#if (__MOLA_VERSION__ == 1010)
	// 패킷 종류에 따라 처리

	// Simplex-Communication Random Key In Client/Server
	VOID	KeyExchangerSimplex( DWORD dwKey );

	// similar to Initialize
	VOID	KeyExchangerDuplex( DWORD dwKey );

#endif //(__MOLA_VERSION__ == 1010)
};

class ServerSNKey : public RandomSNKey
{
public:
	BYTE	GetSendSNKey( SNKeyType keyType ) { return GetSolarSNKey(keyType); }
	BYTE	GetRecvSNKey( SNKeyType keyType ) { return GetLunarSNKey(keyType); }

	VOID	NextRoundSNKey4Send() { NextRoundSNKey4Solar(); }
	VOID	NextRoundSNKey4Recv() { NextRoundSNKey4Lunar(); }
};

class ClientSNKey : public RandomSNKey
{
public:
	BYTE	GetSendSNKey( SNKeyType keyType ) { return GetLunarSNKey(keyType); }
	BYTE	GetRecvSNKey( SNKeyType keyType ) { return GetSolarSNKey(keyType); }

	VOID	NextRoundSNKey4Send() { NextRoundSNKey4Lunar(); }
	VOID	NextRoundSNKey4Recv() { NextRoundSNKey4Solar(); }
};


#endif	//_RND_SNKEY_H


