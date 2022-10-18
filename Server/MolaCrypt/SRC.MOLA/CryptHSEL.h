#pragma once
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	CryptHSEL.h : interface for the CCrypt_HSEL class.
#include "HSEL.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	CryptHSEL
class CryptHSEL
{
public:		CryptHSEL() : m_bInited(FALSE) {}
			~CryptHSEL() { m_bInited = FALSE; }

			//////////////////////////////////////////////////////////////////////////
			//	Server 초기화시, 이후 SetInit(TRUE)를 호출하면, 동작한다.
			BOOL					Create();
			//	Client 초기화시
			//		- 서버로부터 Encode, Decode키를 받아 할당한다.
			//		- 마찬가지로 SetInit(TRUE)를 호출하면 동작한다.
			BOOL					Init( HSEL_INITIAL	eninit, HSEL_INITIAL deInit );
			//	암호화 동작 시작 설정
			inline VOID				SetInit( BOOL val )	{ m_bInited = val; }
			inline BOOL				GetInit()			{ return m_bInited; }

#if CODE_BACKUP
			//////////////////////////////////////////////////////////////////////////
			//	OPERATOR
			CHSEL_STREAM *	GetEnCHSEL_STREAM() { return &m_hEnStream; }
			CHSEL_STREAM *	GetDeCHSEL_STREAM() { return &m_hDeStream; }
			BOOL			Encrypt( char * eBuf, int bufSize );
			BOOL			Decrypt( char * eBuf, int bufSize );
			char			GetEnCRCConvertChar();
			char			GetDeCRCConvertChar();

			HSEL_INITIAL&		GetEncryptKey() { return m_encKey; }
			HSEL_INITIAL&		GetDecryptKey() { return m_decKey; }

private:
			HSEL_INITIAL		m_encKey;
			HSEL_INITIAL		m_decKey;
			CHSEL_STREAM	m_hEnStream;
			CHSEL_STREAM	m_hDeStream;
			BOOL			m_bInited;
#endif //

public:		//////////////////////////////////////////////////////////////////////////
			//	<OPERATOR>
			inline HSEL_INITIAL&		GetEncryptKey()		{ return m_encKey; }
			inline HSEL_INITIAL&		GetDecryptKey()		{ return m_decKey; }

			inline CHSEL_STREAM*	GetEnCHSEL_STREAM() { return &m_hEnStream; }
			inline CHSEL_STREAM*	GetDeCHSEL_STREAM() { return &m_hDeStream; }

			BOOL					Encrypt( BYTE* pBuffer, INT iBufSize );
			BOOL					Decrypt( BYTE* pBuffer, INT iBufSize );
			BYTE					GetEnCRCConvertChar();
			BYTE					GetDeCRCConvertChar();

private:	//	<FIELDs>
			BOOL					m_bInited;		// TrippleDES Algorithm Start/Stop
			HSEL_INITIAL				m_encKey;
			HSEL_INITIAL				m_decKey;
			CHSEL_STREAM			m_hEnStream;
			CHSEL_STREAM			m_hDeStream;
};

