// CryptHSEL.h: interface for the CCrypt_HSEL class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CryptManager.h"
#include "HSEL.h"

class CryptHSEL //: public CryptManager
{
public:
	CryptHSEL();
	virtual ~CryptHSEL();

	// Server 초기화시, 이후 SetInit(TRUE)를 호출하면, 동작한다.
	BOOL			Create();
	// Client 초기화시, 서버로부터 Encode, Decode키를 받아 할당한다.
	// 마찬가지로 SetInit(TRUE)를 호출하면 동작한다.
	BOOL			Init( HselInit	eninit, HselInit deInit );
	// 암호화 동작 시작 설정
	void			SetInit( BOOL val ) { m_bInited = val; }
	BOOL			GetInit() { return m_bInited; }

	CHSEL_STREAM *	GetEnCHSEL_STREAM() { return &m_hEnStream; }
	CHSEL_STREAM *	GetDeCHSEL_STREAM() { return &m_hDeStream; }
	BOOL			Encrypt( char * eBuf, int bufSize );
	BOOL			Decrypt( char * eBuf, int bufSize );
	char			GetEnCRCConvertChar();
	char			GetDeCRCConvertChar();

	HselInit&		GetEncryptKey() { return m_encKey; }
	HselInit&		GetDecryptKey() { return m_decKey; }

private:
	HselInit		m_encKey;
	HselInit		m_decKey;
	CHSEL_STREAM	m_hEnStream;
	CHSEL_STREAM	m_hDeStream;
	BOOL			m_bInited;
};

