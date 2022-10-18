// Crypt.cpp: implementation of the CryptHSEL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CryptHSEL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CryptHSEL::CryptHSEL()
{
	m_bInited = FALSE;
}

CryptHSEL::~CryptHSEL()
{
}

BOOL CryptHSEL::Create()
{
	HselInit eninit;
	eninit.iEncryptType		=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount		=	HSEL_DES_TRIPLE;
	eninit.iCustomize		=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag		=	HSEL_SWAP_FLAG_ON;
	
	if( !m_hEnStream.Initial(eninit) )
	{
		//ASSERTMSG(0, "CryptHSEL Create Initial() failed");
		//_tprintf( _T("CryptHSEL Create Initial() failed") );
		return FALSE;
	}
	m_encKey = m_hEnStream.GetHSELCustomizeOption();	//Get HSEL Keys and Option

	if( !m_hDeStream.Initial(eninit) )
	{
		//ASSERTMSG(0, "CryptHSEL Create Initial() failed");
		//_tprintf( _T("CryptHSEL Create Initial() faile") );
		return FALSE;
	}
	m_decKey = m_hDeStream.GetHSELCustomizeOption();	//Get HSEL Keys and Option

	return TRUE;
}

BOOL CryptHSEL::Init( HselInit	eninit, HselInit deinit )
{
	m_encKey = deinit;			// 서버의 de key는 클라이언트의 en key
	m_decKey = eninit;			// 서버의 en key는 클라이언트의 de key

	// 서버의 de -> en
	if( !m_hEnStream.Initial(deinit) )
	{
		//ASSERTMSG(0, "CryptHSEL EnInit Initial() failed");
		//_tprintf( _T("CryptHSEL EnInit Initial() failed") );
		return FALSE;
	}

	// 서버의 en -> de
	if( !m_hDeStream.Initial(eninit) )
	{
		//ASSERTMSG(0, "CryptHSEL DeInit Initial() failed");
		//_tprintf( _T("CryptHSEL DeInit Initial() failed") );
		return FALSE;
	}
	m_bInited = TRUE;

	return TRUE;
}

BOOL CryptHSEL::Encrypt( char * eBuf, int bufSize )
{
	if( !m_bInited )
		return TRUE;
	return m_hEnStream.Encrypt( eBuf, bufSize );
}

BOOL CryptHSEL::Decrypt( char * eBuf, int bufSize )
{
	if( !m_bInited )
		return TRUE;
	return m_hDeStream.Decrypt( eBuf, bufSize );
}	

char CryptHSEL::GetEnCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hEnStream.GetCRCConvertChar();
}
char CryptHSEL::GetDeCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hDeStream.GetCRCConvertChar();
}