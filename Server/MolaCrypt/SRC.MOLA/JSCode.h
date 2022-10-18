#ifndef _JSCODE_H	
#define _JSCODE_H	


#include "RandomSNKey.h"

class JSCode //: public CryptManager
{
public:
	JSCode();
	~JSCode() {}
	VOID	Init();

	VOID	SetAction(BOOL bFlag)			{ m_bAction = bFlag; };
	VOID	SetKey(BYTE cKey1, BYTE cKey2)	{ m_encKey = cKey1;	m_decKey = cKey2; };

	VOID	EnCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc );
	BOOL	DeCode(BYTE* IN OUT pSrc, DWORD uiSize, BYTE& crc );

	BYTE&	GetEncryptKey() { return m_encKey; }
	BYTE&	GetDecryptKey() { return m_decKey; }

private:
	BOOL	m_bAction;
	BYTE	m_encKey;
	BYTE	m_decKey;
	RandomSNKey	m_Keys;
};

#endif

