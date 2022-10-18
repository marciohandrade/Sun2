#include "SunClientPrecompiledHeader.h"

////#include "nedmalloc.h"
//
//#include <stdio.h>
//#include <tchar.h>
//
//
//#include <windows.h>
//
//#include <iostream>
//#include <tchar.h>
//#include <stdio.h>
//#include <conio.h>
//#include <string.h>


#include "WGPLAllocator.h"

namespace WGPL
{
    fn_CallbackWGPL _Callback_WGPL = NULL;

    //------------------------------------------------------------------------------
    void SetCallbackWGPL(fn_CallbackWGPL fn)
    {
        _Callback_WGPL = fn;
    }

    //------------------------------------------------------------------------------
	void* Alloc(size_t size)
	{
		void* p = malloc(size); //nedalloc::nedmalloc(size);
#ifdef _DEBUG
		printf("Alloc 0x%X\n", p);
#endif
		return p;
	}

    //------------------------------------------------------------------------------
	void Free(void* p, size_t size)
	{
		if (p)
		{
#ifdef _DEBUG
			printf("Free 0x%X\n", p);
#endif
			//nedalloc::nedfree(p);
            free(p);

            p = NULL;
        }
	}
}

