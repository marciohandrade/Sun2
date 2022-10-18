#pragma once
//������������������������������������������������������������������������������
//	CryptHSEL.h : interface for the CCrypt_HSEL class.
#include "HSEL.h"

//������������������������������������������������������������������������������
//	CryptHSEL
class CryptHSEL
{
public:		CryptHSEL() : m_bInited(FALSE) {}
			~CryptHSEL() { m_bInited = FALSE; }

			//////////////////////////////////////////////////////////////////////////
			//	Server �ʱ�ȭ��, ���� SetInit(TRUE)�� ȣ���ϸ�, �����Ѵ�.
			BOOL					Create();
			//	Client �ʱ�ȭ��
			//		- �����κ��� Encode, DecodeŰ�� �޾� �Ҵ��Ѵ�.
			//		- ���������� SetInit(TRUE)�� ȣ���ϸ� �����Ѵ�.
			BOOL					Init( HSEL_INITIAL	eninit, HSEL_INITIAL deInit );
			//	��ȣȭ ���� ���� ����
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

