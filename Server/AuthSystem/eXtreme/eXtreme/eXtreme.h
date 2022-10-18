#pragma once

#define _WIN32_DCOM

#include <crtdbg.h>
#include "common.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32")
#pragma comment(lib, "mpr.lib")

#define __eXtreme_Ver  0x0000


#ifdef  _CRT_DEBUG
    //  Detection memory leak
    #define _new_dbg_  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
    #define _new_dbg_  new
#endif


//  Declares dll exports
#define EXTREME_API

#if defined(__EXTREME_DLLIMPORT)

    #undef  EXTREME_API
    #define EXTREME_API     __declspec(dllimport)

#elif defined(_USRDLL)

    #undef   EXTREME_API
    #define EXTREME_API     __declspec(dllexport)

#endif



#if defined(_MSC_VER) && (_MSC_VER == 1200) //  for vs 6.0 or minor version...
    #pragma  warning(disable:4786)
    #pragma  warning(disable:4251)
#elif  defined(_MSC_VER) && (_MSC_VER > 1200) //  for vs 7.0 or higher version...
    #pragma  warning(disable:4251)
#endif

#if !defined(_MT)
    #error You must select multi-threaded libraries if you want to use eXtreme Network Library.
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition IfDebug
#ifndef IfDebug
    #ifdef  _DEBUG
        #define IfDebug()       if(true)
    #else
        #define IfDebug()       if(false)
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition IfNotDebug
#ifndef IfNotDebug
    #ifdef  _DEBUG
        #define IfNotDebug()    if(false)
    #else
        #define IfNotDebug()    if(true)
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition SafeDelete
#ifndef SafeDelete
    #define SafeDelete(expr)    \
        {                       \
            if((expr) != 0)     \
                delete (expr);  \
            expr = 0;           \
        }
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition Swap
#ifndef Swap
    #define Swap(left, right)   ((left) ^= ((right) ^= ((left) ^= (right))))
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition CompileTimeNote
#ifndef CompileTimeNote
    #define __CTN_Line(x)           #x
    #define CompileTimeNote(msg)    message(__FILE__ "(" __CTN_Line(__LINE__) "): [NOTE] " #msg)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Definition Singleton
#ifndef eSingleton
#define eSingleton(_T) \
    private:    \
        _T();   \
        virtual ~_T();  \
    public: \
        __declspec(noinline) static _T* Instance() \
        {   \
            static  _T  selfInstance; \
            return &selfInstance; \
        }
#endif

namespace eXtreme {

template<typename _tDerived, typename _tBase>
class IsSuperClass
{
private:
    class CheckTrue
    {
        unsigned char   notUsed[2];
    };

    class CheckFalse
    {
    };

    static  CheckTrue       check(_tBase*);
    static  CheckFalse      check(...);

public:
    enum
    {
        isTrue = (sizeof(check(static_cast< _tDerived* >(0))) == sizeof(CheckTrue))
    };
};

//  Error Handling
typedef void (*LPFN_ErrorHandler)(DWORD lastError, char* desc);
bool  Initialize(char* mutexName = 0, LPFN_ErrorHandler handler = 0);

void  CallbackErrorHandler(DWORD lastError, char* desc);
DWORD StringToHash(char* target);

};

#if !defined(EXT_ERROR_BASE)
// eXtreme error define
#define EXT_ERROR_BASE                  (WSABASEERR + 10000)
#define EXT_ERROR_ZERO_BYTE_RECEIVE     (EXT_ERROR_BASE + 1)
#endif
