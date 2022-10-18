#pragma once

/********************************************************************
	created:	2010/11/23
	created:	23:11:2010   12:02
	filename: 	d:\Svn\trunk\Server\Common\Pms\PMSConnWrapper.h
	file path:	d:\Svn\trunk\Server\Common\Pms
	file base:	PMSConnWrapper
	file ext:	h
	author:		youngmoon
	
	purpose:	PMS DLL 의 method 를 사용하기 위한 wrapper 클래스
*********************************************************************/

#ifdef _JP_0_20101123_PMS

#include "PMSConn.h"

class PMSWrapper
{	
public:
    virtual ~PMSWrapper() {}
	// prepare for calling PMSConn API
	DWORD	Init(DWORD argc, LPTSTR argv[]) {return ::PMSInitConn(argc, argv); }
    DWORD	Run(IPMSObject* interface_pms)  {return ::PMSRunConn(interface_pms); }
	void	Stop()                          {::PMSStopConn(); }
	BOOL	SendWarningMsg(DWORD dwErrLvl, LPCSTR pszWarningMsg, LPCSTR pszTreatMsg, DWORD dwSSN, DWORD dwCategory)
	{
		return ::PMSSendWarningMsg(dwErrLvl, pszWarningMsg, pszTreatMsg, dwSSN, dwCategory);
	}	
	DWORD	GetStatus()                     {return PMSGetStatus();}
    //LPCTSTR	GetConfigFileName(); 사용하지 않음.
};

#endif //_JP_0_20101123_PMS
