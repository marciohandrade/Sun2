
#include "stdafx.h"
#include "BitOperation.h"

BitOp::BitOp()
{
	m_bInited = FALSE;
	m_encKey = 0;
	m_decKey = 0;
	m_encKey = 0;
	m_decKey = 0;
	m_Keys.Initialize( (DWORD)'b', 'i', 't' );
}

inline DWORD BitOp::ConvertDWORD( DWORD src, DWORD mask )
{
	if( MASK::c_WORD & mask )
		src = (src<<16) | (src>>16);
	if( MASK::c_BYTE & mask )
		src = ((src<<8)&0xFF00FF00) | ((src>>8)&0x00FF00FF);
	if( MASK::c_4BIT & mask )
		src = ((src<<4)&0xF0F0F0F0) | ((src>>4)&0x0F0F0F0F);
	if( MASK::c_2BIT & mask )
		src = ((src<<2)&0xCCCCCCCC) | ((src>>2)&0x33333333);
	if( MASK::c_1BIT & mask )
		src = ((src<<1)&0xAAAAAAAA) | ((src>>1)&0x55555555);
	return src;
}

inline WORD BitOp::ConvertWORD( WORD src, DWORD mask )
{
	if( MASK::c_BYTE & mask )
		src = ((src<<8)&0xFF00FF00) | ((src>>8)&0x00FF00FF);
	if( MASK::c_4BIT & mask )
		src = ((src<<4)&0xF0F0F0F0) | ((src>>4)&0x0F0F0F0F);
	if( MASK::c_2BIT & mask )
		src = ((src<<2)&0xCCCCCCCC) | ((src>>2)&0x33333333);
	if( MASK::c_1BIT & mask )
		src = ((src<<1)&0xAAAAAAAA) | ((src>>1)&0x55555555);
	return src;
}

inline BYTE BitOp::ConvertBYTE( BYTE src, DWORD mask )
{
	if( MASK::c_4BIT & mask )
		src = ((src<<4)&0xF0F0F0F0) | ((src>>4)&0x0F0F0F0F);
	if( MASK::c_2BIT & mask )
		src = ((src<<2)&0xCCCCCCCC) | ((src>>2)&0x33333333);
	if( MASK::c_1BIT & mask )
		src = ((src<<1)&0xAAAAAAAA) | ((src>>1)&0x55555555);
	return src;
}

union U42 {
	DWORD*	pDW;
	WORD*	pW;
	BYTE*	pB;
};

void	BitOp::EnCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc )
{
	m_encKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_BASE );
	m_decKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_PAIR );
	m_Keys.NextRoundSNKey4Solar();

	DWORD re = uiSize%sizeof(DWORD);
	BYTE checkSum = 0;
	U42 off;

	DWORD dwTempKey
		= ((DWORD)m_encKey << 24)
		| ((DWORD)m_encKey^(DWORD)m_decKey)<<16
		| ((DWORD)m_decKey<<8)
		| (DWORD)m_encKey|(DWORD)m_decKey;

	off.pB = pSrc;

	DWORD dwSize = (DWORD)(uiSize/sizeof(DWORD));

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
	_tprintf( "DWORD size %d : remain %d\n", dwSize, re );
#endif

	for( DWORD i=0 ; i<dwSize ; ++i )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
		_tprintf( "[%08X]", off.pDW );
#endif
		*off.pDW ^= dwTempKey;
		*off.pDW = ConvertDWORD( *off.pDW, MASK::c_ALL );
		*off.pDW ^= dwTempKey;
		checkSum ^= *off.pB;
		++off.pDW;
	}

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
	_tprintf( "\nLAST [%08X]\n", off.pDW );
#endif

	for( DWORD i=(uiSize-re) ; i<uiSize ; ++i )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
		//off.pB[i] = off.pB[i]^m_decKey;
		_tprintf( "[%08X]", off.pB );
#endif
		*off.pB = ConvertBYTE( *off.pB, MASK::c_BYTE );
		checkSum ^= *off.pB;
		++off.pB;
	}

	crc = checkSum;
}

BOOL	BitOp::DeCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc )
{
	m_encKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_BASE );
	m_decKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_PAIR );
	m_Keys.NextRoundSNKey4Solar();

	DWORD re = uiSize%sizeof(DWORD);
	U42 off;
	BYTE checkSum = 0;

	DWORD dwSize = (DWORD)(uiSize/sizeof(DWORD));

	off.pB = pSrc;

	DWORD dwTempKey
		= ((DWORD)m_encKey << 24)
		| ((DWORD)m_encKey^(DWORD)m_decKey)<<16
		| ((DWORD)m_decKey<<8)
		| (DWORD)m_encKey|(DWORD)m_decKey;

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
	_tprintf( "\n\nDECODE\n" );
#endif
	for( DWORD i=(uiSize-re) ; i<uiSize ; ++i )
	{
		checkSum ^= off.pB[i];
		off.pB[i] = ConvertBYTE( off.pB[i], MASK::c_BYTE );
		//--off.pB;
	}
	
	//--off.pDW;
	for( DWORD i=0 ; i<dwSize ; ++i )
	{
		checkSum ^= (*off.pB);

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
		_tprintf( "[%08X]", off.pDW );
#endif
		*off.pDW ^= dwTempKey;
		*off.pDW = ConvertDWORD( *off.pDW, MASK::c_ALL );
		*off.pDW ^= dwTempKey;
		++off.pDW;
	}
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BITTEST__)
	_tprintf( "\nLAST [%08X]\n", off.pDW );
#endif

	if(crc == checkSum)
		return TRUE;
	else
	{
		return FALSE;
	}
}


