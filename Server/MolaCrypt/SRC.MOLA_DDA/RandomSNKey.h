#ifndef _RND_SNKEY_H	
#define _RND_SNKEY_H

typedef DWORD	SNKeyType;

class RandomSNKey
{
public:
	static const SNKeyType c_dwBaseKey	= 0;
	static const SNKeyType c_dwPairKey	= 1;
public:
	RandomSNKey();
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

