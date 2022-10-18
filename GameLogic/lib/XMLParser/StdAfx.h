// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4A8C3B02_6171_421E_A53D_1D2F876DA420__INCLUDED_)
#define AFX_STDAFX_H__4A8C3B02_6171_421E_A53D_1D2F876DA420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#ifdef _AFX_BUILD

#include <afx.h>
#include <afxwin.h>
#include <afxconv.h>
#include <afxtempl.h>

#else

#include <Windows.h>
#include <tchar.h>

//#include "StdString.h"
//#define CString CStdString

#pragma warning (disable: 4786)
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//#define TRACE

#endif


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4A8C3B02_6171_421E_A53D_1D2F876DA420__INCLUDED_)
