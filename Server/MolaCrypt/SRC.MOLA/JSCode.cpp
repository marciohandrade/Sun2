
#include "stdafx.h"
#include "JSCode.h"

JSCode::JSCode()
{
	Init();
}

VOID JSCode::Init()
{
	m_bAction = 0;
	m_encKey = 0;
	m_decKey = 0;
	m_Keys.Initialize( (DWORD)'J', 'S', 'C' );
}

VOID JSCode::EnCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc )
{
	if(!m_bAction)	return;
	if( pSrc == NULL )	return;

	m_encKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_BASE );
	m_decKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_PAIR );
	m_Keys.NextRoundSNKey4Solar();

	//m_encKey++; m_decKey++;

	//BYTE local_key1 = m_encKey;
	//BYTE local_key2 = m_decKey;
	//BYTE local_key2_prev;

	//// EnCode와 CheckSum을 동시에 실행한다.
	//BYTE cCheckSum	= 0;

	//for(int i=0; i < uiSize; i++)
	//{
	//	local_key2_prev = local_key2;
	//	local_key1 ^= pSrc[i];
	//	local_key2 += pSrc[i];

	//	pSrc[i] = local_key1 + local_key2_prev;

	//	cCheckSum ^= pSrc[i];
	//}

	DWORD re = uiSize%sizeof(DWORD);
	DWORD* off = (DWORD*)pSrc;
	DWORD dwSize = (DWORD)(uiSize/sizeof(DWORD));
	DWORD local_key1
		= ((DWORD)m_encKey << 24)
		| ((DWORD)m_encKey^(DWORD)m_decKey)<<16
		| ((DWORD)m_decKey<<8)
		| (DWORD)m_encKey|(DWORD)m_decKey;
	DWORD local_key2 = ~local_key1;
	DWORD local_key2_prev;
	BYTE cCheckSum	= 0;

	for( DWORD i=0 ; i<dwSize ; ++i )
	{
		local_key2_prev = local_key2;
		local_key1 ^= *off;
		local_key2 += *off;

		*off = local_key1 + local_key2_prev;

		cCheckSum ^= (*off)&0xFF;
		off++;
	}

	for( DWORD i=(uiSize-re) ; i<uiSize ; ++i )
	{
		pSrc[i] = pSrc[i]^m_decKey;
		cCheckSum ^= pSrc[i];
	}

	crc = cCheckSum^m_decKey;
}

BOOL JSCode::DeCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc )
{
	if(!m_bAction || !pSrc )	return FALSE;

	m_encKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_BASE );
	m_decKey = m_Keys.GetSolarSNKey( RandomSNKey::SNKEY_PAIR );
	m_Keys.NextRoundSNKey4Solar();

	//m_encKey++; m_decKey++;

	//BYTE local_key1 = m_encKey;
	//BYTE local_key2 = m_decKey;

	//// DeCode를 하기전에 ChekcSum을 구한다.
	//BYTE cCheckSum	= 0;

	//for(int i=0; i < uiSize; i++)
	//{
	//	cCheckSum ^= pSrc[i];

	//	pSrc[i] = (pSrc[i] - local_key2) ^ local_key1;

	//	local_key1 ^= pSrc[i];
	//	local_key2 += pSrc[i];
	//}

	DWORD re = uiSize%sizeof(DWORD);
	DWORD* off = (DWORD*)pSrc;
	DWORD dwSize = (DWORD)(uiSize/sizeof(DWORD));
	DWORD local_key1
		= ((DWORD)m_encKey << 24)
		| ((DWORD)m_encKey^(DWORD)m_decKey)<<16
		| ((DWORD)m_decKey<<8)
		| (DWORD)m_encKey|(DWORD)m_decKey;
	DWORD local_key2 = ~local_key1;
	//DWORD local_key2_prev;
	BYTE cCheckSum	= 0;

	crc = crc^m_decKey;
	for( DWORD i=(uiSize-re) ; i<uiSize ; ++i )
	{
		cCheckSum ^= pSrc[i];
		pSrc[i] = pSrc[i]^m_decKey;
	}

	for( DWORD i=0 ; i<dwSize ; ++i )
	{
		cCheckSum ^= (*off)&0xFF;

		*off = (*off - local_key2) ^ local_key1;

		local_key1 ^= *off;
		local_key2 += *off;
		off++;
	}

	if(crc == cCheckSum)
		return TRUE;
	else
	{
		//_tprintf("JSCode : [DeCode] CheckSum Error! \n");
		return FALSE;
	}
}
