#ifndef __TSTRING_H__
#define __TSTRING_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>
#include <tchar.h>

namespace util
{

#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif

}

#endif 