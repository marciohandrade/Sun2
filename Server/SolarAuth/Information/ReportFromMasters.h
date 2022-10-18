#pragma once

//=============================================================================================================================
/// ReportFromMasters class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2007. 05. 23
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer
	@history 
	-  
*/
//=============================================================================================================================

class ReportFromMasters : public IWorkNode
{
public:
			static const DWORD			INTERVAL_SUNLOG_UPDATE			= 10*1000;

public:		ReportFromMasters() : m_pLoggingHandler(NULL) {}
			~ReportFromMasters() {}

//////////////////////////////////////////////////////////////////////////
// <IWORKNODE INTERFACEs>
public:		virtual BOOL	DoInit();
			virtual BOOL	DoProcess();
			virtual BOOL	IsComplete();
			virtual BOOL	DoRelease();

			virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

			//typedef std::map<WORD, sCACHEITEM_REPORT_INFO>		CACHEITEM_INFO_MAP;
			//typedef CACHEITEM_INFO_MAP::iterator				CACHEITEM_INFO_MAP_IT;
			//typedef std::map<DWORD, CACHEITEM_INFO_MAP>			MASTER_REPORT_INFO_MAP;
			//typedef MASTER_REPORT_INFO_MAP::iterator			MASTER_REPORT_INFO_MAP_IT;
private:
			CSunLog*					m_pLoggingHandler;
};

