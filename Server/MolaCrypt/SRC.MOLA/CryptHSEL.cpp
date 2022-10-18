#include "stdafx.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	Crypt.cpp: implementation of the CryptHSEL class.
//
#include "CryptHSEL.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	

BOOL CryptHSEL::Create()
{
	HSEL_INITIAL eninit;
	eninit.iEncryptType		=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount		=	HSEL_DES_TRIPLE;
	eninit.iCustomize		=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag		=	HSEL_SWAP_FLAG_ON;
	
	if( !m_hEnStream.Initial(eninit) )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_HSEL__)
		ASSERTMSG(0, "CryptHSEL Create Initial() failed");
		_tprintf( _T("CryptHSEL Create Initial() failed") );
#endif
		return FALSE;
	}
	m_encKey = m_hEnStream.GetHSELCustomizeOption();	//Get HSEL Keys and Option

	if( !m_hDeStream.Initial(eninit) )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_HSEL__)
		ASSERTMSG(0, "CryptHSEL Create Initial() failed");
		_tprintf( _T("CryptHSEL Create Initial() faile") );
#endif
		return FALSE;
	}
	m_decKey = m_hDeStream.GetHSELCustomizeOption();	//Get HSEL Keys and Option

	return TRUE;
}

BOOL CryptHSEL::Init( HSEL_INITIAL	eninit, HSEL_INITIAL deinit )
{
	m_encKey = deinit;			// 서버의 de key는 클라이언트의 en key
	m_decKey = eninit;			// 서버의 en key는 클라이언트의 de key

	// 서버의 de -> en
	if( !m_hEnStream.Initial(deinit) )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_HSEL__)
		ASSERTMSG(0, "CryptHSEL EnInit Initial() failed");
		_tprintf( _T("CryptHSEL EnInit Initial() failed") );
#endif
		return FALSE;
	}

	// 서버의 en -> de
	if( !m_hDeStream.Initial(eninit) )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_HSEL__)
		ASSERTMSG(0, "CryptHSEL DeInit Initial() failed");
		_tprintf( _T("CryptHSEL DeInit Initial() failed") );
#endif
		return FALSE;
	}
	m_bInited = TRUE;

	return TRUE;
}

BOOL CryptHSEL::Encrypt( BYTE* pBuffer, INT bufSize )
{
	if( !m_bInited )
		return TRUE;
	return m_hEnStream.Encrypt( (CHAR*)pBuffer, bufSize );
}

BOOL CryptHSEL::Decrypt( BYTE* pBuffer, INT bufSize )
{
	if( !m_bInited )
		return TRUE;
	return m_hDeStream.Decrypt( (CHAR*)pBuffer, bufSize );
}	

BYTE CryptHSEL::GetEnCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hEnStream.GetCRCConvertChar();
}
BYTE CryptHSEL::GetDeCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hDeStream.GetCRCConvertChar();
}