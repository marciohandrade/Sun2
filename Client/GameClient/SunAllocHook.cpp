// SunAllocHook.cpp

#include "SunClientPrecompiledHeader.h"
#include "SunAllocHook.h"

const int ALLOC_PATTERN_LENGTH = 3;

// 왠지 없을만한걸 WILDCARD로 사용하자 정 겹칠때는 다른값을 DEFINE하면된다.
const int ALLOC_WILD_CARD = 999999;

int AllocPattern[ALLOC_PATTERN_LENGTH] = {360, 540, 16};

DWORD g_AllocSize = 0;

//------------------------------------------------------------------------------
/**
*/
int SunAllocHook( int nAllocType, void *pvData, size_t nSize, int nBlockUse, 
				  long lRequest, const unsigned char * szFileName, int nLine )
{
	static int iAllocPatternMatch = 0;
	static int iCurAllocPattern[ALLOC_PATTERN_LENGTH] = {0,};

	static long lOldRequest = 0;

	if (lOldRequest == 0)
	{
		lOldRequest = lRequest;
	}
	else
	{
		if (lRequest > lOldRequest + 1)
		{
			_asm nop;
		}

		lOldRequest = lRequest;
	}

	if (nAllocType == _HOOK_ALLOC)
	{
		g_AllocSize += nSize;
	}
	else if (nAllocType == _HOOK_FREE)
	{
		g_AllocSize -= nSize;
	}

	if (iAllocPatternMatch < ALLOC_PATTERN_LENGTH) 
	{
		iCurAllocPattern[iAllocPatternMatch] = nSize;
		iAllocPatternMatch++;
	}
	else
	{
		// 하나씩 앞으로 당긴다.
		for (int a = 0; a < ALLOC_PATTERN_LENGTH - 1; a++)
		{
			iCurAllocPattern[a] = iCurAllocPattern[a + 1];
		}

		// 맨끝에 새거를 넣는다.
		iCurAllocPattern[ALLOC_PATTERN_LENGTH - 1] = nSize;
	}

	if (iAllocPatternMatch == ALLOC_PATTERN_LENGTH)
	{
		BOOL bTest = TRUE;
		for (int a = 0; a < ALLOC_PATTERN_LENGTH; a++)
		{
			if (AllocPattern[a] == ALLOC_WILD_CARD)
			{
				continue;
			}

			if (iCurAllocPattern[a] != AllocPattern[a])
			{
				bTest = FALSE;
				break;
			}
		}

		if (bTest)
		{
			assert( !"원하는 패턴을 찾았다" );
		}
	}

	return TRUE;
}