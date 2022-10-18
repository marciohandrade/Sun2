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

	// Server �ʱ�ȭ��, ���� SetInit(TRUE)�� ȣ���ϸ�, �����Ѵ�.
	BOOL			Create();
	// Client �ʱ�ȭ��, �����κ��� Encode, DecodeŰ�� �޾� �Ҵ��Ѵ�.
	// ���������� SetInit(TRUE)�� ȣ���ϸ� �����Ѵ�.
	BOOL			Init( HselInit	eninit, HselInit deInit );
	// ��ȣȭ ���� ���� ����
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

