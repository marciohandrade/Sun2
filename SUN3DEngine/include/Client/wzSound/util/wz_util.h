#ifndef __CP_UTIL_H__
#define __CP_UTIL_H__
#ifdef __cplusplus
extern "C" {
#endif

// wz_util.h

//========================================
//	Constants / Defines / TypDefs
//========================================
#define SAFE_DELETE(p)       { if(p) { free(p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { free(p);   (p)=NULL; } }




//////////////////////////////////////////////////////////////////////////
// debugging facility
/*
#ifdef _DEBUG

//#ifndef _MSCP_VER

int __CrtDbgReport( 
   int reportType,
   const char *filename,
   int linenumber,
   const char *moduleName,
   const char *lpszFormat ,
   ... 
);


//#else

#include <crtdbg.h>

//#endif

#define CP_TRACE0(format) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n")
#define CP_TRACE1(format, arg1) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n", arg1)
#define CP_TRACE2(format, arg1, arg2) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n", arg1, arg2)
#define CP_TRACE3(format, arg1, arg2, arg3) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n", arg1, arg2, arg3)
#define CP_TRACE4(format, arg1, arg2, arg3, arg4) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n", arg1, arg2, arg3, arg4)
#define CP_TRACE5(format, arg1, arg2, arg3, arg4, arg5) __CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, format "\n", arg1, arg2, arg3, arg4, arg5)
//
#define CP_ASSERT(expr) if(!(expr)) { CP_TRACE1("ASSERTION %s FAILS", #expr); __asm { int 3 }  }
#define CP_FAIL(errstring) { CP_TRACE1("HARD FAILURE %s", #errstring); __asm { int 3 } }
//
#define CP_CHECKOBJECT(obj_ptr_typed) if(!obj_ptr_typed || !_CrtIsMemoryBlock(obj_ptr_typed, sizeof(*obj_ptr_typed), NULL, NULL, NULL)) { CP_TRACE1("POINTER %s is Bogus", #obj_ptr_typed); __asm { int 3 } }


#else*/

#define CP_CHECKOBJECT(obj_ptr_typed)	{	 }
#define CP_ASSERT(expr)					{	 }
#define CP_FAIL(expr)					{	 }
#define CP_TRACE0(f)					{	 }	
#define CP_TRACE1(f, e1)				{	 }		
#define CP_TRACE2(f, e1, e2)			{	 }
#define CP_TRACE3(f, e1, e2, e3)		{	 }		
#define CP_TRACE4(f, e1, e2, e3, e4)	{	 }
#define CP_TRACE5(f, e1, e2, e3, e4, e5){	 }

//#endif


void _LOG(const char* szFormat, ...);
void OpenLogFile();
void CloseLogFile();


//////////////////////////////////////////////////////////////////////////
// 
HWND MakeHelperWindow(char* pClassname, WNDPROC lpfnWndProc);

//////////////////////////////////////////////////////////////////////////
// shell command utilities

BOOL    path_is_directory(char *filename);
BOOL    path_is_relative(const char *path);
BOOL    path_remove_filespec(LPTSTR path);
void      path_add_backslash(char *path);
void      path_unquote(LPTSTR path);
DWORD   main_get_program_path(HINSTANCE hInst, LPTSTR pszBuffer,
                              DWORD dwSize);

//////////////////////////////////////////////////////////////////////////
// string utilities

char   *str_delete_substr(char *strbuf, char *strtodel);
char   *str_trim(char *string);
// Some basic string utility stuff
void STR_AllocSetString(char** ppcDest, const char* pcSource, const BOOL bFreeExisting);



//////////////////////////////////////////////////////////////////////////
// math utilities

FLOAT   ConvertLinearSliderPosToLogScale( LONG lSliderPos );
LONG    ConvertLogScaleToLinearSliderPosTo( FLOAT fValue );


#ifdef __cplusplus
}
#endif
#endif //__CP_UTIL_H__