#pragma once


#include "SolarFileReader.h"

class BaseParser
{
public :

	BaseParser();
	virtual ~BaseParser();

	void	SetPackFileName( char * pszPackFileName );
	void	SetPackFileName( wchar_t * pszPackFileName);

protected :

	BOOL	OpenFile( util::SolarFileReader & sr, char * pszFileName,  eSEPERATOR_MODE mode );	

private :

	char	m_pszPackFileName[MAX_PATH];
};