#ifndef __PREPROCDEFINES_H__
#define __PREPROCDEFINES_H__

//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winbase.h>		/* needed for memory mapping of file functions */

// 4127 is constant conditional (assert)
// 4100 is unreferenced formal parameters,
// 4514 is unreferenced inline function removed, 
// 4201 is nameless struct extension used. (used by windows header files)
// 4410 illegal size for operand... ie... 	fxch st(1)
// 4611 is _setjmp warning.  Since we use setjmp alot, and we don't really use constructors or destructors, this warning doesn't really apply to us.
// 4725 is the pentium division bug warning, and I can't seem to get rid of it, even with this pragma.
//      JS: I figured out the disabling 4725 works, but not on the first function in the module.
//      So to disable this, I add in a stub function at the top of each module that does nothing.
// 4710 is inline function not expanded (who cares?)
// 4711 tells us an inline function was expanded (who cares?)
// 4702 unreachable code.  I care, but too many to deal with
// 4201 nonstandard extension used : nameless struct/union (happens a lot in Windows include headers)
// 4390 emptry control statement (triggered by nprintf and mprintf's inside of one-line if's, etc)
// 4996 depreciated strcpy, strcat, sprintf, etc. (from MSVC 2005) - taylor
#pragma warning(disable: 4127 4100 4514 4201 4410 4611 4725 4710 4711 4702 4201 4390 4996)
#pragma warning(disable: 4786)
#pragma warning(disable: 4284)

#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

#include <time.h>
#include <tchar.h>
#include <direct.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "util/util_debug.h"

//////////////////////////////////////////////////////////////////////////

#if defined( __x86_64__ ) || defined( _WIN64 )
#define IAM_64BIT 1
#endif

#if !defined BYTE_ORDER
 #define LITTLE_ENDIAN 1234
 #define BIG_ENDIAN    4321

 #if defined _M_IX86 || defined _X86_
  #define BYTE_ORDER   LITTLE_ENDIAN
 #else
  #error unknown byte order
 #endif
#endif  // BYTE_ORDER


#ifdef IAM_64BIT

typedef __int32 _fs_time_t;  // time_t here is 64-bit and we need 32-bit
typedef __int32 fix;
// PTR compatible sizes
typedef __int64 ptr_s;
typedef unsigned __int64 ptr_u;

#else

typedef long fix;
typedef	long _fs_time_t;
typedef int ptr_s;
typedef unsigned int ptr_u;

#endif // 64-bit


typedef __int64 longlong;
typedef unsigned __int64 ulonglong;
typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define PI					3.141592654f
#define PI2					(3.141592654f*2.0f)	// PI*2

//////////////////////////////////////////////////////////////////////////

// value to represent an uninitialized state in any int or uint
#define UNINITIALIZED 0x7f8e6d9c

#define USE_INLINE_ASM 1		// Define this to use inline assembly
#define STRUCT_CMP(a, b) memcmp((void *) &a, (void *) &b, sizeof(a))


//////////////////////////////////////////////////////////////////////////

#define THREADED	// to use the proper set of macros

#ifdef THREADED
	#define INITIALIZE_CRITICAL_SECTION(csc)	do { InitializeCriticalSection(&csc); } while(0)
	#define DELETE_CRITICAL_SECTION(csc)		do { DeleteCriticalSection(&csc); } while(0)
	#define ENTER_CRITICAL_SECTION(csc)			do { EnterCriticalSection(&csc); } while(0)
	#define LEAVE_CRITICAL_SECTION(csc)			do { LeaveCriticalSection(&csc); } while(0)

#else
	#define INITIALIZE_CRITICAL_SECTION(csc)	do { } while(0)
	#define DELETE_CRITICAL_SECTION(csc)		do { } while(0)
	#define ENTER_CRITICAL_SECTION(csc)			do { } while(0)
	#define LEAVE_CRITICAL_SECTION(csc)			do { } while(0)
#endif


//////////////////////////////////////////////////////////////////////////

#define LOCATION __FILE__,__LINE__
#define Assert(x) do {} while (0)


#define INTEL_INT(x)	x
#define INTEL_SHORT(x)	x
#define INTEL_FLOAT(x)	(*x)

// Some constants for stuff
#define MAX_FILENAME_LEN	256
#define MAX_PATH_LEN			128


#define	PATHNAME_LENGTH			192
#define	NAME_LENGTH				32
#define	SEXP_LENGTH				128
#define	DATE_LENGTH				32
#define	TIME_LENGTH				16
#define	DATE_TIME_LENGTH		48
#define	NOTES_LENGTH			1024


#define	MULTITEXT_LENGTH		4096
#define	FILESPEC_LENGTH			64
#define	MESSAGE_LENGTH			512


//! game_play_sound index 1000000 보다 작게
#define UISOUND_KEY_INDEX_OFFSET  (1000000)


//////////////////////////////////////////////////////////////////////////

//Stucture to store clipping codes in a word
typedef struct ccodes {
	ubyte cc_or, cc_and;		//or is low byte, and is high byte
} ccodes;

typedef struct vector {
	union {
		struct {
			float x,y,z;
		} xyz;
		float a1d[3];
	};
} vector;

// A vector referenced as an array
typedef struct vectora {
	float	xyz[3];
} vectora;

typedef struct vec2d {
	float i,j;
} vec2d;

// Used for some 2d primitives, like gr_poly
typedef struct vert2df {
	float x, y;
} vert2df;

typedef struct angles {
	float	p, b, h;
} angles_t;

typedef struct matrix {
	union {
		struct {
			vector	rvec, uvec, fvec;
		} vec;
		float a2d[3][3];
		float a1d[9];
	};
} matrix;

typedef struct uv_pair {
	float u,v;
} uv_pair;

// Used to store rotated points for mines.
// Has flag to indicate if projected.
typedef struct vertex {
	float		x, y, z;				// world space position
	float		sx, sy, sw;			// screen space position (sw == 1/z)
	float		u, v, u2, v2, u3, v3, u4, v4;					// texture position
	vector		real_pos;			// _real_ world position
	ubyte spec_a, spec_r, spec_b, spec_g;	//specular highlights -Bobboau
	ubyte		r, g, b, a;			// color.  Use b for darkening;
	ubyte		codes;				// what sides of view pyramid this point is on/off.  0 = Inside view pyramid.
	ubyte		flags;				// Projection flags.  Indicates whether it is projected or not or if projection overflowed.
	ubyte		pad[2];				// pad structure to be 4 byte aligned.
} vertex;


#ifndef D3DVECTOR_DEFINED
typedef struct _D3DVECTOR {
    float x;
    float y;
    float z;
} D3DVECTOR;
#define D3DVECTOR_DEFINED
#endif

typedef struct SND_DIRECTION
{
	DWORD      dwInsideConeAngle;
	DWORD      dwOutsideConeAngle; 
	D3DVECTOR  vConeOrientation;
	LONG       lConeOutsideVolume;
} SND_DIRECTION, *LPSND_DIRECTION;


#endif //__PREPROCDEFINES_H__