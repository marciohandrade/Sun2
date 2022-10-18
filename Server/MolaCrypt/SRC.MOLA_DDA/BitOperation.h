
#ifndef		_BITOPERATION_H
#define		_BITOPERATION_H

#include "RandomSNKey.h"

class BitOp
{
public:
	struct MASK
	{
		static const DWORD	c_WORD = 0x01;
		static const DWORD	c_BYTE = 0x02;
		static const DWORD	c_4BIT = 0x04;
		static const DWORD	c_2BIT = 0x08;
		static const DWORD	c_1BIT = 0x10;
		static const DWORD	c_ALL = 0x1F;
	};
public:
	BitOp();

	VOID		SetAction( BOOL bInit ) { m_bInited = bInit; }

	void		SetKey(DWORD cKey1, DWORD cKey2)	{ m_encKey = cKey1;	m_decKey = cKey2; };
	void		EnCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc );
	BOOL		DeCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc );

	DWORD&		GetEncryptKey() { return m_encKey; }
	DWORD&		GetDecryptKey() { return m_decKey; }
public:
	static DWORD ConvertDWORD( DWORD src, DWORD mask );
	static WORD ConvertWORD( WORD src, DWORD mask );
	static BYTE ConvertBYTE( BYTE src, DWORD mask );
private:
	BOOL		m_bInited;
	RandomSNKey	m_Keys;
	DWORD		m_encKey;
	DWORD		m_decKey;
};

//private:
//	BOOL	m_bAction;
//	BYTE	m_encKey;
//	BYTE	m_decKey;
//	RandomSNKey	m_Keys;
//};

#endif		//_BITOPERATION_H
