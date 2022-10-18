#ifndef SAFE_STRING_H
#define SAFE_STRING_H

/*---------------------------------------------------------------------------------------

@Author : 유재영
@Remark 
- 유니코드 변환을 위해서 TCHAR 형으로 _strcpy을 변경하였다.
	[7/5/2005]

- 유니코드 변환을 위해서 TCHAR 형으로 GetToken을 변경하였다.
	[12/12/2005]

//---------------------------------------------------------------------------------------
*/

namespace util
{
//	void * SafeNCopy(char * pszDest, const char * pszSource, size_t Size );
	TCHAR * SafeNCopy(TCHAR * pszDest, const TCHAR * pszSource, size_t Size);

	TCHAR * GetToken(TCHAR * pszString, TCHAR * OUT pszGetToken, TCHAR * pszTokenEnd);
}


TCHAR * _strncpy(TCHAR * pszDest, const TCHAR * pszSource, size_t Size );
BOOL	_Strncmp(TCHAR * pszComp1, int iLen1, TCHAR * pszComp2, int iLen2 );
BOOL	_Strincmp(TCHAR * pszComp1, int iLen1, TCHAR * pszComp2, int iLen2 );

#endif
// SAFE_STRING_H