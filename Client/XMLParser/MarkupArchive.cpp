// PGLMarkup.cpp: implementation of the CMarkupArchive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarkupArchive.h"
#include "zlib.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace markup
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarkupArchive::~CMarkupArchive()
{
	Close();
}

bool CMarkupArchive::Open(LPCTSTR szFileName)
{
	m_sFileName= szFileName;

	ASSERT(m_bOpened==false);

	if (m_dwMode & ArchiveModeStoring)
	{
		SetDoc(NULL);
		m_bOpened=true;
	}
	else
	{
        // Extra test: Check the file exists when reading... CMarkupArchive lacks this ;)
        if (_taccess(szFileName, 0) != 0) 
                return false;

		// Zlib can handle both zipped and non zipped files
		// ZLIB is not UNICODE compliant !!!
		#ifdef _UNICODE
			char cBuffer[512];
			VERIFY(WideCharToMultiByte(CP_THREAD_ACP, // code page
				WC_SEPCHARS,            // performance and mapping flags
				m_sFileName,    // wide-character string
				-1,          // number of chars in string
				cBuffer,     // buffer for new string
				512,          // size of buffer
				NULL,     // default for unmappable chars
				NULL  // set when default char used
				));
			gzFile gzf = gzopen(cBuffer,"rb");
		#else
			gzFile gzf = gzopen( m_sFileName, "rb");
		#endif
			
			// reading data - zlib will detect if zipped or not...
			TCHAR tInString[4096];
			int nReadChar=1;
			std::_tstring sLoaded;

			// reading chunk of data and adding to sLoaded
			while ( nReadChar != 0 && nReadChar != -1 )
			{
				// read zip file
				nReadChar=gzread(gzf, tInString, 4096 * sizeof(TCHAR) );

				// testing if load succesful
				if (nReadChar == -1)
				{
					// something went wrong, closing zip file and stopping
					gzclose(gzf);
					return m_bOpened=false;
				}
				// tInString OK, adding
				if (nReadChar != 0)
				{
					sLoaded.append(tInString,nReadChar);
					TRACE(_T("nReadChar = %d, sLoaded size = %d\n"), nReadChar,sLoaded.size());
				}
			}
			// closing zipfile
			gzclose(gzf);

			m_bOpened=SetDoc(sLoaded.c_str());
	}

	return m_bOpened;
}

bool CMarkupArchive::OpenString(LPCTSTR szXMLString)
{
	if (szXMLString==NULL)
		return false;

	m_dwMode = ~(~m_dwMode | ArchiveModeStoring);

	m_sFileName=_T("");
	m_bOpened=true;

	return SetDoc(szXMLString);
}


bool CMarkupArchive::Close()
{
	if (!m_bOpened)
		return false;

	CString str;
	LPTSTR pBuffer;
	int length;

	// --- start --- TMB! 29-08-2002 ** INSAD **
	// Initialy fill str with the XML header ;)
	str.Format(_T("<?xml version=\"1.0\" encoding=\"%s\" ?>\r\n"), m_strEncodingString);
	// --- end --- TMB! 29-08-2002 ** INSAD **

	if (IsStoring())
	{
		if (!( m_dwMode & ArchiveModeNotZipped))
		{
	#ifdef _UNICODE
			char mbFileName[512];
			VERIFY( WideCharToMultiByte(CP_THREAD_ACP, // code page
				WC_SEPCHARS,            // performance and mapping flags
				m_sFileName,    // wide-character string
				-1,          // number of chars in string
				mbFileName,     // buffer for new string
				512*sizeof(TCHAR),          // size of buffer
				NULL,     // default for unmappable chars
				NULL  // set when default char used
				) );
			gzFile gzf = gzopen(mbFileName,"wb9");
	#else
			gzFile gzf = gzopen( m_sFileName, "wb9");
	#endif

			// --- Old code --- TMB! 29-08-2002 ** INSAD ** Now already filled!
			//str=_T("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\r\n");
			// --- end --- TMB! 29-08-2002 ** INSAD **
			length=str.GetLength()*sizeof(TCHAR);

		//	pBuffer=str.LockBuffer();
			pBuffer=str.GetBuf();
			gzwrite(gzf,pBuffer,length);
		//	str.UnlockBuffer();

			// writing document
			str=GetDoc();
			length=str.GetLength();
			//pBuffer=str.LockBuffer();
			pBuffer=str.GetBuf();
			gzwrite(gzf,pBuffer, length);
			//str.UnlockBuffer();
			gzclose(gzf);
		}
		else
		{
			/*TRY
			{
				CStdioFile file(m_sFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeText);
				// --- start --- TMB! 29-08-2002 ** INSAD ** // Already filled!
				file.WriteString(str);
				// --- Old code --- TMB! 29-08-2002 ** INSAD **
				//file.WriteString(_T("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\r\n"));
				// --- end --- TMB! 29-08-2002 ** INSAD **
				file.WriteString(GetDoc());
				file.Close();
			}
			CATCH( CFileException, e )
			{
				AfxMessageBox(_T("File could not be opened ")+e->m_cause);
			}
			END_CATCH*/

			{
				HANDLE hFile; 
				
				hFile = CreateFile(m_sFileName,           // create MYFILE.TXT 
					GENERIC_WRITE,                // open for writing 
					0,                            // do not share 
					NULL,                         // no security 
					CREATE_ALWAYS,                // overwrite existing 
					FILE_ATTRIBUTE_NORMAL,
					NULL);                        // no attr. template 
				
				if (hFile == INVALID_HANDLE_VALUE) 
				{ 
				//	ErrorHandler("Could not open file.");  // process error 
					return false;
				} 
		
				DWORD dwWritten;
				WriteFile(hFile, str, str.GetLength(), &dwWritten, NULL );
				WriteFile(hFile, GetDoc().c_str(), GetDoc().length(), &dwWritten, NULL );
	
				CloseHandle(hFile);			
			}
			
		}
	}

	m_bOpened=false;

	return true;
}


bool CMarkupArchive::FindElemAttrib( LPCTSTR szName, LPCTSTR szAttrib, LPCTSTR szAttribValue )
{
	if ( szName == NULL || !szName[0] || szAttrib == NULL || !szAttrib[0])
		return false;

	// Otherwise go to next sibling element with matching tag name and attrib value
	// If the current position is valid, start looking from next
	// Change current position only if found
	int iPos = m_iPos;
	if ( ! iPos )
	{
		if ( m_aPos.GetSize() )
			iPos = m_aPos[0].iElemChild;
	}
	else
	{
		iPos = m_aPos[iPos].iElemNext;
	}

	BOOL bGotIt;											// --- Added --- TMB! 22-08-2002 ** INSAD **
	while ( iPos )
	{
		// Compare tag name unless szName is not specified
		// --- start --- TMB! 22-08-2002 ** INSAD **
		bGotIt = (x_GetTagName(iPos) == m_sNameSpace+szName);
		if (bGotIt && szAttribValue == NULL) 
		{
			bGotIt = x_GetAttrib(iPos, szAttrib) != "";		// Assume empty value is same as attribute not there...
		}
		else 
		{
			bGotIt = (x_GetAttrib(iPos, szAttrib) == szAttribValue);
		}
		if (bGotIt)
		// --- oldcode --- TMB! 22-08-2002 ** INSAD **
		//if ( x_GetTagName(iPos) == m_sNameSpace+szName && x_GetAttrib(iPos, szAttrib) )
		// --- end --- TMB! 22-08-2002 ** INSAD **
		{
			// Assign new position
			m_iPos = iPos;
			m_iPosChild = 0;
			return true;
		}
		iPos = m_aPos[iPos].iElemNext;
	}
	return false;
}



bool CMarkupArchive::FindChildElemAttrib( LPCTSTR szName , LPCTSTR szAttrib, LPCTSTR szAttribValue )
{
	if ( szName == NULL || !szName[0] || szAttrib == NULL || !szAttrib[0])
		return false;

	// If szName is NULL or empty, go to next sibling child element
	// Otherwise go to next sibling child element with matching tag name
	// If the current child position is valid, start looking from next
	// Change current child position only if found
	//
	// Shorthand: call this with no current position means under root element
	if ( ! m_iPos )
		FindElem();

	// Is main position valid and not empty?
	if ( ! m_iPos || m_aPos[m_iPos].IsEmptyElement() )
		return false;

	// Is current child position valid?
	int iPosChild = m_iPosChild;
	if ( iPosChild )
		iPosChild = m_aPos[iPosChild].iElemNext;
	else
		iPosChild = m_aPos[m_iPos].iElemChild;

	// Search
	BOOL bGotIt;											// --- Added --- TMB! 22-08-2002 ** INSAD **
	while ( iPosChild )
	{
		// Compare tag name unless szName is not specified
		// --- start --- TMB! 22-08-2002 ** INSAD **
		bGotIt = (x_GetTagName(iPosChild) == m_sNameSpace+szName);
		if (bGotIt && szAttribValue == NULL) 
		{
			bGotIt = x_GetAttrib(iPosChild, szAttrib) != _T("");		// Assume empty value is same as attribute not there...
		}
		else 
		{
			bGotIt = (x_GetAttrib(iPosChild, szAttrib) == szAttribValue);
		}
		if (bGotIt)
		// --- oldcode --- TMB! 22-08-2002 ** INSAD **
		//if ((x_GetTagName(iPosChild) == m_sNameSpace+szName) && (x_GetAttrib(iPosChild, szAttrib) == szAttribValue))
		// --- end --- TMB! 22-08-2002 ** INSAD **
		{
			// Assign new position
			m_iPosChild = iPosChild;
			return true;
		}
		iPosChild = m_aPos[iPosChild].iElemNext;
	}
	return false;
}
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////::
/// GetData functions
///////////////////////////////////////////////////////////////////////////////////////////////////////::
COleDateTimeSpan CMarkupArchive::GetDataDateTimeSpan() const
{
	int d=0,h=0,m=0,s=0; 
	_stscanf(GetData(),_T("%d,%d:%d:%d"),&d,&h,&m,&s);
	return COleDateTimeSpan(d,h,m,s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////::
/// GetChildData functions
///////////////////////////////////////////////////////////////////////////////////////////////////////::
COleDateTimeSpan CMarkupArchive::GetChildDataDateTimeSpan() const
{
	int d=0,h=0,m=0,s=0; 
	_stscanf(GetChildData(),_T("%d,%d:%d:%d"),&d,&h,&m,&s);
	return COleDateTimeSpan(d,h,m,s);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////::
/// FindGetData functions
///////////////////////////////////////////////////////////////////////////////////////////////////////::
bool CMarkupArchive::FindGetData(LPCTSTR szName, std::string& _string)
{
	bool found;
	ResetPos();
	if  (found = FindElem(szName))
		_string = GetData();
	return found;
}

bool CMarkupArchive::FindGetData(LPCTSTR szName, bool& _bool)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_bool = GetDataBool();
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, int& _int)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_int = GetDataInt();
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, USHORT& _USHORT)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_USHORT = GetDataUSHORT();
	return found;
}

bool CMarkupArchive::FindGetData(LPCTSTR szName, UINT& _UINT)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_UINT = GetDataUINT();
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, BYTE& _BYTE)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_BYTE = GetDataBYTE();
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, DWORD& _DWORD)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_DWORD = GetDataDWORD();
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, float& _float)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_float = GetDataFloat();
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, double& _double)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		_double = GetDataDouble();
	return found;
};
/*
bool CMarkupArchive::FindGetData(LPCTSTR szName, COleDateTime& _t)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
	{
		_t = GetDataDateTime();
	}
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, COleDateTimeSpan& _ts)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
	{
		_ts = GetDataDateTimeSpan();
	}
	return found;
};*/

bool CMarkupArchive::FindGetData(LPCTSTR szName, std::vector<double>& _vDouble)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vDouble);
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, std::vector<float>& _vFloat)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vFloat);
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, std::vector<int>& _vInt)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vInt);
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, std::vector<BYTE>& _vBYTE)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vBYTE);
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, std::valarray<double>& _vDouble)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vDouble);
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, std::valarray<float>& _vFloat)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vFloat);
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, std::valarray<int>& _vInt)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vInt);
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, std::valarray<BYTE>& _vBYTE)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_vBYTE);
	return found;
};

bool CMarkupArchive::FindGetData(LPCTSTR szName, std::list<double>& _lDouble)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_lDouble);
	return found;
};


bool CMarkupArchive::FindGetData(LPCTSTR szName, std::list<float>& _lDouble)
{
	bool found;

	ResetPos();
	if  (found = FindElem(szName))
		GetDataEx(_lDouble);
	return found;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////::
/// FindGetChildData functions
///////////////////////////////////////////////////////////////////////////////////////////////////////::
bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::string& _string)
{
	bool found;
	ResetChildPos();
	if  (found = FindChildElem(szName))
		_string = GetChildData();
	return found;
}

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, bool& _bool)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_bool = GetChildDataBool();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, int& _int)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_int = GetChildDataInt();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, UINT& _UINT)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_UINT = GetChildDataUINT();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, USHORT& _USHORT)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_USHORT = GetChildDataUSHORT();
	return found;
}

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, BYTE& _BYTE)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_BYTE = GetChildDataBYTE();
	return found;
}
	
bool CMarkupArchive::FindGetChildData(LPCTSTR szName, long& _long)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_long = GetChildDataLong();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, DWORD& _DWORD)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_DWORD = GetChildDataDWORD();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, float& _float)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_float = GetChildDataFloat();
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, double& _double)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		_double = GetChildDataDouble();
	return found;
};
/*
bool CMarkupArchive::FindGetChildData(LPCTSTR szName, COleDateTime& _t)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
	{
		_t = GetChildDataDateTime();
	}
	return found;
};


bool CMarkupArchive::FindGetChildData(LPCTSTR szName, COleDateTimeSpan& _ts)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
	{
		_ts = GetChildDataDateTimeSpan();
	}
	return found;
};*/

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::vector<double>& _vDouble)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vDouble);
	return found;
};


bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::vector<float>& _vFloat)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vFloat);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::vector<int>& _vInt)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vInt);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::vector<BYTE>& _vBYTE)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vBYTE);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::valarray<double>& _vDouble)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vDouble);
	return found;
};


bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::valarray<float>& _vFloat)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vFloat);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::valarray<int>& _vInt)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vInt);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::valarray<BYTE>& _vBYTE)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_vBYTE);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::list<double>& _lDouble)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_lDouble);
	return found;
};

bool CMarkupArchive::FindGetChildData(LPCTSTR szName, std::list<float>& _lFloat)
{
	bool found;

	ResetChildPos();
	if  (found = FindChildElem(szName))
		GetChildDataEx(_lFloat);
	return found;
};

};
