
#include "stdafx.h"
#include "RandomSNKey.h"

#pragma pack(push, 1)
union Fcode {
	DWORD	dwVal;
	float	fVal;
	struct {
		WORD	wVal;
		BYTE	byDummy1;
		BYTE	byDummy2;
	};
};
#pragma pack(pop)


static int _cKeyCheck = 0xFF000000;

//struct UserKeyInfo {
//	DWORD	m_dwUserKey;// = 0;
//	DWORD	m_dwVersion;
//	//DWORD	m_dwLunarKeyValue;// = 0xA96A96A9;
//	//DWORD	m_dwLunarKeyInc;// = 32;
//	//BYTE	m_byLunarBaseKey;// = ~0xA6;
//	//BYTE	m_byLunarPairKey;// = ~0x96;
//} tagUserSN_Info[4];

//class RandomSNKey {
//private:
//	int		m_dwLunarKeyValue;
//	DWORD	m_dwLunarKeyInc;
//	BYTE	m_byLunarBaseKey;
//	BYTE	m_byLunarPairKey;
//
//public:
//	RandomSNKey( DWORD userKey, BYTE proto_High, BYTE proto_Minor, BYTE proto_Devel );
//	static VOID NextRound( DWORD userKey );
//	static BYTE	GetServerKey();
//	static BYTE GetClientKey();
//};

RandomSNKey::RandomSNKey() {}

VOID RandomSNKey::Initialize( DWORD userKey, BYTE majorVer, BYTE minorVer )
{
	const	DWORD dwRndUserKeyBase		= 0x0319A267;
	const	DWORD dwRndUserKeyByCrypt	= 0x116EF35F;
	DWORD dwRndUserKey			= (userKey)%0x0000FFFF;

	dwRndUserKey = ( (dwRndUserKeyBase*(dwRndUserKey))+(dwRndUserKeyByCrypt+((dwRndUserKey)>>16)) );
	m_dwUserKey = dwRndUserKey;
	majorVer += 7;
	minorVer += majorVer;
	m_dwVersion = majorVer<<8 | minorVer;
	//(proto_Major<<24) | (proto_Minor<<16) | (proto_Devel<<8) | 0;

	//m_dwSolarKeyValue = m_dwUserKey*m_dwVersion + (m_dwUserKey>>16);
	//m_dwLunarKeyValue = m_dwUserKey*m_dwVersion + (m_dwUserKey>>16);
	//m_dwSolarKeyInc = ~m_dwSolarKeyValue;
	//m_dwLunarKeyInc = ~m_dwSolarKeyValue;
	//m_bySolarBaseKey = (BYTE)(m_dwUserKey * majorVer)>>16;
	//m_bySolarPairKey = (BYTE)((m_dwUserKey>>24)*majorVer)>>16;
	//m_byLunarBaseKey = (BYTE)(m_dwUserKey * majorVer)>>16;
	//m_byLunarPairKey = (BYTE)((m_dwUserKey>>24)*majorVer)>>16;

	m_dwSolarKeyValue = m_dwUserKey*m_dwVersion + (m_dwUserKey>>16);
	m_dwLunarKeyValue = m_dwUserKey*m_dwVersion + (m_dwSolarKeyValue>>16);
	m_dwSolarKeyInc = ~m_dwSolarKeyValue;
	m_dwLunarKeyInc = ~m_dwLunarKeyValue;
	m_bySolarBaseKey = (BYTE)(m_dwUserKey * majorVer)>>16;
	m_bySolarPairKey = (BYTE)((m_dwUserKey>>24)*majorVer)>>16;
	m_byLunarBaseKey = (BYTE)(~m_dwUserKey * minorVer)>>16;
	m_byLunarPairKey = (BYTE)((~m_dwUserKey>>24)*minorVer)>>16;

	NextRoundSNKey4Solar();
	NextRoundSNKey4Lunar();
}

VOID			RandomSNKey::KeyExchange( DWORD userKey )
{
	m_dwUserKey = userKey;
	//(proto_Major<<24) | (proto_Minor<<16) | (proto_Devel<<8) | 0;
	DWORD tmp;
	tmp = m_dwSolarKeyInc;
	m_dwSolarKeyInc = m_dwLunarKeyInc;
	m_dwLunarKeyInc = tmp;
}


void RandomSNKey::NextRoundSNKey4Solar()
{
	if( m_dwVersion & 0x00FF0000 )
	{
		Fcode fcode;
		fcode.dwVal = m_dwSolarKeyInc + m_dwUserKey;
		fcode.fVal = ((float)fcode.dwVal/7.f);

		m_dwSolarKeyValue = m_dwSolarKeyValue ^ fcode.dwVal;
	}

	m_dwSolarKeyInc = ( (m_bySolarBaseKey*(m_dwSolarKeyInc))+(m_dwSolarKeyValue+((m_dwSolarKeyInc)>>16)) );

	m_bySolarBaseKey = (BYTE)(((m_dwSolarKeyInc)>>24)&0xFF);
	m_bySolarPairKey = (BYTE)(((m_dwSolarKeyInc)>>16)&0xFF);

	if( m_dwVersion & 0xFF000000 )
	{
		Fcode fcode;
		fcode.dwVal = m_dwSolarKeyInc + m_dwUserKey;
		fcode.fVal = ((float)fcode.dwVal/7.f);
		//_tprintf( "fcode %08X %04X\t", fcode.dwVal, fcode.wVal );

		// enc, dec 처음 값을 항상 같게 하려면 아래 주석 제거하고 뒤 코드 없엘것.
		//m_bySolarBaseKey = m_dwSolarKeyValue ^ m_bySolarBaseKey;
		//m_bySolarPairKey = m_bySolarBaseKey ^ bitop;

		DWORD tmp = m_dwSolarKeyValue ^ fcode.dwVal;
		//switch( /*fcode.dwVal*/tmp%5 )
		//{
		//case 0:
		//	(tmp&0x80000000) ? tmp<<=1, tmp^=1 : tmp>>=1, tmp|=1;
		//	break;
		//case 1:
		//	(tmp&0x80000000) ? tmp>>=1, tmp|=0x80000000 : tmp<<=1, tmp^=1;
		//	//break;
		//case 2:
		//	(tmp&0x80000000) ? tmp<<=2, tmp|=1 : tmp>>=2, tmp^=0x80000000;
		//	break;
		//case 3:
		//	(tmp&0x80000000) ? tmp>>=2, tmp|=0xC0000000 : tmp<<=2, tmp^=0x3;
		//	//break;
		//case 4:
		//	(tmp&0x80000000) ? tmp<<=3, tmp|=0x7 : tmp>>=3, tmp^=0xE0000000;
		//	break;
		//}

		//m_dwSolarKeyValue = tmp;
		//bitop = m_dwSolarKeyValue;

		//if( (tmp&_cKeyCheck) == _cKeyCheck )
		//	tmp ^= ((DWORD)(BYTE)fcode.wVal)<<24;
		//if( (tmp&(_cKeyCheck>>8)) == (_cKeyCheck>>8) )
		//	tmp ^= (~(DWORD)(BYTE)fcode.wVal)<<16;
		//if( (tmp&(_cKeyCheck>>16)) == (_cKeyCheck>>16) )
		//	tmp ^= ((DWORD)(BYTE)fcode.wVal)<<8;
		//if( (tmp&(_cKeyCheck>>24) ) == (_cKeyCheck>>24) )
		//	tmp ^= (~(DWORD)(BYTE)fcode.wVal);

		//m_dwSolarKeyInc += /*2427*/m_dwVersion+0x0069AA69;
		m_dwSolarKeyInc += m_dwUserKey*0x9AA69 + 0x3C6Ef35F;
		m_dwSolarKeyValue = tmp;// ^ m_dwSolarKeyInc;

		//m_bySolarBaseKey = (BYTE)(m_dwSolarKeyValue ^ m_bySolarBaseKey);
		////m_bySolarPairKey = m_bySolarBaseKey ^ bitop;
		//m_bySolarPairKey = (BYTE)(fcode.wVal ^ m_bySolarPairKey);
		m_bySolarBaseKey = (BYTE)((m_dwSolarKeyInc ^ m_bySolarBaseKey)>>24);
		m_bySolarPairKey = (BYTE)((m_dwSolarKeyInc ^ m_dwSolarKeyValue)>>24);

		// 줄무늬 난수
		//m_bySolarBaseKey = (BYTE)(m_dwSolarKeyValue%256 ^ m_bySolarBaseKey);
		//m_bySolarPairKey = (BYTE)(m_dwSolarKeyInc%256 ^ m_bySolarPairKey);


		//_tprintf( "KeyContainer %08X\tSN1 [%02X] SN2 [%02X]\n",
		//	m_dwSolarKeyValue,
		//	m_bySolarBaseKey, m_bySolarPairKey );

		//_tprintf( "%d\t%d\n",
		//	m_bySolarBaseKey, m_bySolarPairKey );
	}
}

void RandomSNKey::NextRoundSNKey4Lunar()
{
	if( m_dwVersion & 0x00FF0000 )
	{
		Fcode fcode;
		fcode.dwVal = m_dwLunarKeyInc + m_dwUserKey;
		fcode.fVal = ((float)fcode.dwVal/7.f);

		m_dwLunarKeyValue = m_dwLunarKeyValue ^ fcode.dwVal;
	}

	m_dwLunarKeyInc = ( (m_byLunarBaseKey*(m_dwLunarKeyInc))+(m_dwLunarKeyValue+((m_dwLunarKeyInc)>>16)) );

	m_byLunarBaseKey = (BYTE)(((m_dwLunarKeyInc)>>24)&0xFF);
	m_byLunarPairKey = (BYTE)(((m_dwLunarKeyInc)>>16)&0xFF);

	if( m_dwVersion & 0xFF000000 )
	{
		Fcode fcode;
		fcode.dwVal = m_dwLunarKeyInc + m_dwUserKey;
		fcode.fVal = ((float)fcode.dwVal/7.f);
		//_tprintf( "fcode %08X %04X\t", fcode.dwVal, fcode.wVal );

		// enc, dec 처음 값을 항상 같게 하려면 아래 주석 제거하고 뒤 코드 없엘것.
		//m_byLunarBaseKey = m_dwLunarKeyValue ^ m_byLunarBaseKey;
		//m_byLunarPairKey = m_byLunarBaseKey ^ bitop;

		DWORD tmp = m_dwLunarKeyValue ^ fcode.dwVal;

		//m_dwLunarKeyInc += /*2427*/m_dwVersion+0x0069AA69;
		m_dwLunarKeyInc += m_dwUserKey*0x9AA69 + 0x3C6Ef35F;
		m_dwLunarKeyValue = tmp;// ^ m_dwLunarKeyInc;

		//m_byLunarBaseKey = (BYTE)(m_dwLunarKeyValue ^ m_byLunarBaseKey);
		////m_byLunarPairKey = m_byLunarBaseKey ^ bitop;
		//m_byLunarPairKey = (BYTE)(fcode.wVal ^ m_byLunarPairKey);
		m_byLunarBaseKey = (BYTE)((m_dwLunarKeyInc ^ m_byLunarBaseKey)>>24);
		m_byLunarPairKey = (BYTE)((m_dwLunarKeyInc ^ m_dwLunarKeyValue)>>24);

		// 줄무늬 난수
		//m_byLunarBaseKey = (BYTE)(m_dwLunarKeyValue%256 ^ m_byLunarBaseKey);
		//m_byLunarPairKey = (BYTE)(m_dwLunarKeyInc%256 ^ m_byLunarPairKey);


		//_tprintf( "KeyContainer %08X\tSN1 [%02X] SN2 [%02X]\n",
		//	m_dwLunarKeyValue,
		//	m_byLunarBaseKey, m_byLunarPairKey );

		//_tprintf( "%d\t%d\n",
		//	m_byLunarBaseKey, m_byLunarPairKey );


		/*
		Fcode fcode;
		fcode.dwVal = m_dwLunarKeyInc + m_dwUserKey;
		fcode.fVal = ((float)fcode.dwVal/7.f);
		//_tprintf( "fcode %08X %04X\t", fcode.dwVal, fcode.wVal );

		// enc, dec 처음 값을 항상 같게 하려면 아래 주석 제거하고 뒤 코드 없엘것.
		//m_byLunarBaseKey = m_dwLunarKeyValue ^ m_byLunarBaseKey;
		//m_byLunarPairKey = m_byLunarBaseKey ^ bitop;

		DWORD tmp = m_dwLunarKeyValue ^ fcode.wVal;
		switch( tmp%5 )
		{
		case 0:
			(tmp&0x80000000) ? tmp<<=1, tmp^=1 : tmp>>=1, tmp|=1;
			break;
		case 1:
			(tmp&0x80000000) ? tmp>>=1, tmp|=0x80000000 : tmp<<=1, tmp^=1;
			//break;
		case 2:
			(tmp&0x80000000) ? tmp<<=2, tmp|=1 : tmp>>=2, tmp^=0x80000000;
			break;
		case 3:
			(tmp&0x80000000) ? tmp>>=2, tmp|=0xC0000000 : tmp<<=2, tmp^=0x3;
			//break;
		case 4:
			(tmp&0x80000000) ? tmp<<=3, tmp|=0x7 : tmp>>=3, tmp^=0xE0000000;
			break;
		}

		//m_dwLunarKeyValue = tmp;
		//bitop = m_dwLunarKeyValue;

		if( (tmp&_cKeyCheck) == _cKeyCheck )
		tmp ^= ((DWORD)(BYTE)fcode.wVal)<<24;

		if( (tmp&(_cKeyCheck>>8)) == (_cKeyCheck>>8) )
		tmp ^= (~(DWORD)(BYTE)fcode.wVal)<<16;

		if( (tmp&(_cKeyCheck>>16)) == (_cKeyCheck>>16) )
		tmp ^= ((DWORD)(BYTE)fcode.wVal)<<8;

		if( (tmp&(_cKeyCheck>>24) ) == (_cKeyCheck>>24) )
		tmp ^= (~(DWORD)(BYTE)fcode.wVal);

		m_dwLunarKeyInc += m_dwVersion+0x0069AA69;//2427
		m_dwLunarKeyValue = tmp;// ^ m_dwLunarKeyInc;

		m_byLunarBaseKey = (BYTE)(m_dwLunarKeyValue ^ m_byLunarBaseKey);
		//m_byLunarPairKey = m_byLunarBaseKey ^ bitop;
		m_byLunarPairKey = (BYTE)(fcode.wVal ^ m_byLunarPairKey);

		// 줄무늬 난수
		//m_byLunarBaseKey = (BYTE)(m_dwLunarKeyValue%256 ^ m_byLunarBaseKey);
		//m_byLunarPairKey = (BYTE)(m_dwLunarKeyInc%256 ^ m_byLunarPairKey);


		//_tprintf( "KeyContainer %08X\tSN1 [%02X] SN2 [%02X]\n",
		//	m_dwLunarKeyValue,
		//	m_byLunarBaseKey, m_byLunarPairKey );

		//_tprintf( "%d\t%d\n",
		//	m_byLunarBaseKey, m_byLunarPairKey );
		*/
	}
}


