#include "stdafx.h"
#include ".\varpacket.h"

/*
template <WORD BUFFER_SIZE>
VarPacket<BUFFER_SIZE>::VarPacket()
	:	m_wSize( 0 )
{
}
template <WORD BUFFER_SIZE>
VarPacket<BUFFER_SIZE>::~VarPacket()
{
}

template <WORD BUFFER_SIZE>
BYTE * VarPacket<BUFFER_SIZE>::GetChar( BYTE * pPacket, char & cChar )
{
	cChar = (char)*pPacket;
	return ( pPacket + 1 );
}
template <WORD BUFFER_SIZE>
BYTE * VarPacket<BUFFER_SIZE>::GetShort( BYTE * pPacket, short & sShort )
{
	sShort = (short)*pPacket;
	return ( pPacket + 2 );
}
template <WORD BUFFER_SIZE>
BYTE * VarPacket<BUFFER_SIZE>::GetInteger( BYTE * pPacket, int & iInteger )
{
	iInteger = (int)*pPacket;
	return ( pPacket + 4 );
}
template <WORD BUFFER_SIZE>
BYTE * VarPacket<BUFFER_SIZE>::GetBigInteger( BYTE * pPacket, __int64 & i64BigInteger )
{
	i64BigInteger = (__int64)*pPacket;
	return ( pPacket + 8 );
}

template <WORD BUFFER_SIZE>
BYTE * VarPacket<BUFFER_SIZE>::GetStream( BYTE * pPacket, BYTE * pBuffer )
{
	WORD size = 0;
	pPacket = GetShort( pPacket, size );
	memcpy( pBuffer, pPacket, size );

	return pPacket + size;
}

template <WORD BUFFER_SIZE>
VOID VarPacket<BUFFER_SIZE>::SetChar( char cChar )
{
	m_pPacketBuffer[m_wSize] = cChar;
	m_wSize += 1;
}
template <WORD BUFFER_SIZE>
VOID VarPacket<BUFFER_SIZE>::SetShort( short sShort )
{
	(short)m_pPacketBuffer[m_wSize] = sShort;
	m_wSize += 2;
}
template <WORD BUFFER_SIZE>
VOID VarPacket<BUFFER_SIZE>::SetInteger( int iInteger )
{
	(int)m_pPacketBuffer[m_wSize] = nInteger;
	m_wSize += 4;
}
template <WORD BUFFER_SIZE>
VOID VarPacket<BUFFER_SIZE>::SetBigInteger( __int64 i64BigInteger )
{
	(__int64)m_pPacketBuffer[m_wSize] = i64BigInteger;
	m_wSize += 8;
}

template <WORD BUFFER_SIZE>
VOID VarPacket<BUFFER_SIZE>::SetStream( BYTE * pBuffer, short & sSize )
{
	SetShort( sSize );
	memcpy( &m_pPacketBuffer[m_wSize], pBuffer, sSize );
	m_wSize += sSize;
}
*/
