//////////////////////////////////////////////////////////////////////
//
// Author: Oskar Wieland (oskar.wieland@gmx.de)
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

#ifndef STLHELPER_INCLUDED_IGFX
	#define STLHELPER_INCLUDED_IGFX

#if _MSC_VER > 1000
#pragma once
#endif

#include <iostream>

//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for vector
//////////////////////////////////////////////////////////////////////

#pragma warning(push)

#include <yvals.h>              // warning numbers get enabled in yvals.h 

#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information

#include <vector>
#include <list>
#undef max
#undef min
#include <valarray>
#define max(a,b) (((a)>(b))? (a) : (b))
#define min(a,b) (((a)<(b))? (a) : (b))
#include <stack>
#include <string>
//#include <strstream>
#include <sstream>
#include <fstream>

#ifndef _UNICODE
	namespace std
	{
		typedef string			_tstring;
		typedef ifstream		_tifstream;
		typedef ofstream		_tofstream;
		typedef istringstream	_tistringstream;
		typedef ostringstream	_tostringstream;
	}
#else
	namespace std
	{
		typedef wstring			_tstring;
		typedef wifstream		_tifstream;
		typedef wofstream		_tofstream;
		typedef wistringstream	_tistringstream;
		typedef wostringstream	_tostringstream;
	};
#endif

#pragma warning(pop)

#pragma warning(disable: 4514)  // unreferenced inline/local function has been removed
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information



//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for assert
//////////////////////////////////////////////////////////////////////

// avoid macro redefinition when using MFC
#ifndef ASSERT

#include <cassert>

// macros for tracking down errors
#ifdef _DEBUG

#define ASSERT( exp )           assert( exp )
#define VERIFY( exp )           assert( exp )
#define TRACE                   ::OutputDebugString

#else

#define ASSERT( exp )           ((void)0)
#define VERIFY( exp )           ((void)(exp))
#define TRACE                   1 ? (void)0 : ::OutputDebugString

#endif  // _DEBUG

#endif  // ASSERT

// additional macros 
#define ASSERT_BREAK( exp )             { ASSERT(exp); if( !(exp) ) break; }
#define ASSERT_CONTINUE( exp )          { ASSERT(exp); if( !(exp) ) continue; }
#define ASSERT_RETURN( exp )            { ASSERT(exp); if( !(exp) ) return; }
#define ASSERT_RETURN_NULL( exp )       { ASSERT(exp); if( !(exp) ) return 0; }
#define ASSERT_RETURN_FALSE( exp )      { ASSERT(exp); if( !(exp) ) return false; }

#endif  // STLHELPER_INCLUDED_IGFX
