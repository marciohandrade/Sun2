#include "UtilityCommon.h"
#include "BaseParser.h"


BaseParser::BaseParser()
{
	ZeroMemory( m_pszPackFileName, MAX_PATH );
}

BaseParser::~BaseParser()
{
}

void BaseParser::SetPackFileName( char * pszPackFileName )
{
	if( pszPackFileName )
		strcpy( m_pszPackFileName, pszPackFileName );
	else
		ZeroMemory( m_pszPackFileName, MAX_PATH );
}

void BaseParser::SetPackFileName( wchar_t * pszPackFileName)
{
	char szPackFileName[MAX_PATH +1] = {0,};
	WideCharToMultiByte( CP_ACP, MB_PRECOMPOSED, pszPackFileName, -1, szPackFileName, -1 , NULL ,NULL);

	if( szPackFileName )
		strcpy( m_pszPackFileName, szPackFileName );
	else
		ZeroMemory( m_pszPackFileName, MAX_PATH );
}

BOOL BaseParser::OpenFile( util::SolarFileReader & sr, char * pszFileName,  eSEPERATOR_MODE mode )
{
	BOOL bResult = FALSE;

	// pack
	if( m_pszPackFileName[0] )
	{
		bResult = sr.OpenFileFromPack(m_pszPackFileName, pszFileName, mode);
	}
	// unpack
	else
	{
		bResult = sr.OpenFile( pszFileName, mode );
	}

	if( !bResult )
	{
		ASSERT( !"File Open Error" );
	}

	return bResult;
}