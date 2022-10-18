#ifndef CUNZIPPER_H
#define CUNZIPPER_H

#ifdef _WINDOWS
#include "stdafx.h"
#endif

#include <stdio.h>
#include <atlstr.h>

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib ")

#include "unzip.h"

class CUnZipper
{
	void ErrorLog(char* pErrLog, char* pPath)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char pErrorLogPath[256] = {0, };
		sprintf(pErrorLogPath, "UnzipErrLog(%d-%02d-%02d).txt", st.wYear, st.wMonth, st.wDay);
		FILE* fp = fopen(pErrorLogPath, "a");
		fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d][%s][PATH : %s]\n", st.wYear, st.wMonth, st.wDay,	st.wHour, st.wMinute,
																								st.wSecond, pErrLog, pPath);
		fclose(fp);
	}
public:
	CUnZipper(){}
	~CUnZipper(){}

	bool Unzip(char* pZipFile, char* pDest = NULL)	// NULL일 경우 현재실행경로에 압축을 푼다
	{
		HZIP hz = OpenZip(pZipFile, 0);

		if( !hz )
		{
			#ifdef _WINDOWS
               	OutputDebug("[%s][PATH : %s]\n", "FILE_NOT_OPEN", pZipFile);
			#else
				ErrorLog("FILE_NOT_OPEN", pZipFile);
			#endif

			return false;
		}

		if( pDest && strlen(pDest) && ( FALSE == PathIsDirectory(pDest)) )
		{
			if( !CreateDirectory(pDest, 0) )
			{
				CString Str(pDest);
				Str.Replace('/', '\\');

				TCHAR lastchar = pDest[_tcslen(pDest)-1];
				if (lastchar!='\\' && lastchar!='/')
				{
					//_tcscat(pDest,_T("\\"));
					Str += "\\";
				}

				if( FALSE == MakeSureDirectoryPathExists( Str.GetBuffer(Str.GetLength()) ) )
				{
					#ifdef _WINDOWS
		               	OutputDebug("[CANNOT MAKED DIRECTORY...][%s]\n", Str.GetBuffer(Str.GetLength()));
					#else
						ErrorLog("CANNOT MAKED DIRECTORY...", Str.GetBuffer(Str.GetLength()) );
					#endif

					return false;
				}
			}
		}

		ZIPENTRY ze;
		GetZipItem(hz,-1,&ze, pDest );
		int numitems=ze.index;

		for (int i=0; i<numitems; i++)
		{
			GetZipItem(hz,i,&ze);
			UnzipItem(hz,i,ze.name);
		}

		CloseZip(hz);

		return true;
	}
};

#endif