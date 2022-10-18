
#if !defined(AFX_FILELOGGER_H__FDF4A6C8_5A47_40FE_8D82_804E0DCCE3FE__INCLUDED_)
#define AFX_FILELOGGER_H__FDF4A6C8_5A47_40FE_8D82_804E0DCCE3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class COptions;
class CFileLogger  
{
public:
	BOOL CheckLogFile();
	BOOL Log(LPCTSTR msg);
	CFileLogger(LPCTSTR app_fullpath, int maxSize = 10*1024*1024, BOOL bDaily = FALSE);
	virtual ~CFileLogger();

protected:
	LPTSTR m_pFileName;
	HANDLE m_hLogFile;

private:
	TCHAR m_szAppPath[MAX_PATH];
	int m_maxSize;
	BOOL m_bDaily;
};

#endif // !defined(AFX_FILELOGGER_H__FDF4A6C8_5A47_40FE_8D82_804E0DCCE3FE__INCLUDED_)
