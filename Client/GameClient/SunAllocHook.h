// SunAllocHook.h

#pragma once

int SunAllocHook( int nAllocType, void *pvData, size_t nSize, int nBlockUse, 
				  long lRequest, const unsigned char * szFileName, int nLine );
