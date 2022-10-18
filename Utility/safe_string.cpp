#include "UtilityCommon.h"

namespace util
{
//////////////////////////////////////////////////////////////////////////

	

//void * SafeNCopy(char * pszDest, const char * pszSource, size_t Size )
//{
//#ifdef _DEBUG
//	if (IsBadWritePtr((VOID *)pszDest, Size) || IsBadReadPtr(pszDest, Size) || 
//		IsBadWritePtr((VOID *)pszSource, Size) || IsBadReadPtr(pszSource, Size))
//	{
//		__debugbreak();
//	}
//#endif 
//
//	memset(pszDest, 0, Size);
//	memcpy(pszDest, pszSource, Size);
//
//	return pszDest;
//}

char * SafeNCopy(char * pszDest, const char * pszSource, size_t Size)
{
#ifdef _DEBUG
	if (IsBadWritePtr((VOID *)pszDest, Size) || IsBadReadPtr(pszDest, Size) )
	{
		__debugbreak();
	}
#endif 

	memset(pszDest, 0, Size);		// 이 부분에 대해서 더 지켜보자.
	strncpy( pszDest, pszSource, Size );

	return pszDest;
}

wchar_t * SafeNCopy(wchar_t * pszDest, const wchar_t * pszSource, size_t Size)
{
#ifdef _DEBUG
	if (IsBadWritePtr((VOID *)pszDest, Size) || IsBadReadPtr(pszDest, Size) )
	{
		__debugbreak();
	}
#endif 

	memset(pszDest, 0, Size * sizeof(wchar_t));		// 이 부분에 대해서 더 지켜보자.
	wcsncpy( pszDest, pszSource, Size );

	return pszDest;
}



//
// 해외 버전에도 상관없게 CompareString으로 바꾸자.
//
char * GetToken(char * pszString, char * OUT pszGetToken, char * pszTokenEnd)
{
	int iLen = static_cast<int>(strlen(pszString));
	int iTokenLen = static_cast<int>(strlen(pszTokenEnd));

	if (iLen == 0)
	{
		return NULL;
	}

	if (iLen < iTokenLen)
	{
		return NULL;
	}

	pszGetToken[0] = NULL;

	//int iCurLen = 0;
	char * pszPos1 = NULL;
	if ( CSTR_EQUAL == CompareStringA(
		LOCALE_USER_DEFAULT, 
		0, 
		pszString, 
		iTokenLen, 
		pszTokenEnd, 
		iTokenLen) )
	{
		pszPos1 = &pszString[iTokenLen];
	}
	else
	{
		pszPos1 = pszString;
	}

	static char util_szToken[512] = {0,};

	memset(util_szToken, 0, strlen(util_szToken) * sizeof(char));

	for (int i = 0; i < iLen; i++)
	{
		if (i != 0 && 
			( 0 == strncmp(pszPos1, pszTokenEnd, iTokenLen ) || 
			*pszPos1 == NULL))
		{
			SafeNCopy( pszGetToken, util_szToken, i+1);

			return pszPos1;
		}

		util_szToken[i] = (*pszPos1);
		pszPos1++;
	}

	// ?
	return NULL;
}


//
// 해외 버전에도 상관없게 CompareString으로 바꾸자.
//
wchar_t * GetToken(wchar_t * pszString, wchar_t * OUT pszGetToken, wchar_t * pszTokenEnd)
{
	int iLen = static_cast<int>(wcslen(pszString));
	int iTokenLen = static_cast<int>(wcslen(pszTokenEnd));

	if (iLen <= 0)
	{
		return NULL;
	}

	if (iLen < iTokenLen)
	{
		return NULL;
	}

	pszGetToken[0] = NULL;

	//int iCurLen = 0;
	wchar_t * pszPos1 = NULL;
	if ( CSTR_EQUAL == CompareStringW(
			LOCALE_USER_DEFAULT, 
			0, 
			pszString, 
			iTokenLen, 
			pszTokenEnd, 
			iTokenLen) )
	{
		pszPos1 = &pszString[iTokenLen];
	}
	else
	{
		pszPos1 = pszString;
	}

	static wchar_t util_szToken[512] = {0,};

	memset(util_szToken, 0, wcslen(util_szToken) * sizeof(wchar_t));

	for (int i = 0; i < iLen; i++)
	{
		if (i != 0 && 
			( 0 == wcsncmp(pszPos1, pszTokenEnd, iTokenLen ) || 
			*pszPos1 == NULL))
		{
			SafeNCopy( pszGetToken, util_szToken, i+1);

			return pszPos1;
		}

		util_szToken[i] = (*pszPos1);
		pszPos1++;
	}

	// ?
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
}

//void * _strncpy(char * pszDest, const char * pszSource, size_t Size )
//{
//	return util::SafeNCopy(pszDest, pszSource, Size);
//}

//----------------------------------------------------------------------------
/**
*/
char * 
_strncpy(char * pszDest, const char * pszSource, size_t Size )
{
	return util::SafeNCopy(pszDest, pszSource, Size);
}

//----------------------------------------------------------------------------
/**
*/
wchar_t * 
_strncpy(wchar_t * pszDest, const wchar_t * pszSource, size_t Size )
{
	return util::SafeNCopy(pszDest, pszSource, Size);
}

//----------------------------------------------------------------------------
/**
*/
BOOL
_Strncmp(char * pszComp1, int iLen1, char * pszComp2, int iLen2 )
{
	if ( CSTR_EQUAL == CompareStringA(
			LOCALE_USER_DEFAULT,
			NULL, 
			pszComp1,
			iLen1, 
			pszComp2,
			iLen2) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};


//----------------------------------------------------------------------------
/**
*/
BOOL
_Strncmp(wchar_t * pszComp1, int iLen1, wchar_t * pszComp2, int iLen2 )
{
	if ( CSTR_EQUAL == CompareStringW(
		LOCALE_USER_DEFAULT,
		NULL, 
		pszComp1,
		iLen1, 
		pszComp2,
		iLen2) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};


//----------------------------------------------------------------------------
/**
*/
BOOL	
_Strincmp(char * pszComp1, int iLen1, char * pszComp2, int iLen2 )
{
	if ( CSTR_EQUAL == CompareStringA(
			LOCALE_USER_DEFAULT,
			NORM_IGNORECASE, 
			pszComp1,
			iLen1, 
			pszComp2,
			iLen2) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

//----------------------------------------------------------------------------
/**
*/
BOOL	
_Strincmp(wchar_t * pszComp1, int iLen1, wchar_t * pszComp2, int iLen2 )
{
	if ( CSTR_EQUAL == CompareStringW(
		LOCALE_USER_DEFAULT,
		NORM_IGNORECASE, 
		pszComp1,
		iLen1, 
		pszComp2,
		iLen2) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
};

