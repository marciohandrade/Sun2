#include "stdafx.h"
#include "ReportFromMasters.h"

#include <Sessions/ServerSession.h>


////////////////////////////////////////////////////////////////////////////
//// <TIMEOUT OBJECT>
////   단순 Timeout 처리를 목적으로 한다. <- 이것 Agent에 있다. Utility로 옮기자.
//class ITimeout
//{
//public:
//	static const DWORD	UNLIMITED = (DWORD)(-1);
//	static const DWORD	INSTANCE = 0;
//
//	ITimeout() : m_dwTimeoutTick(UNLIMITED) {}
//	//================================================================================
//	// <INTERFACEs>
//public:
//	inline BOOL			IsExpired()
//	{
//		DWORD dwCurTick = GetTickCount();
//		if( dwCurTick > TimeoutTick() )
//			return TRUE;
//		return FALSE;
//	}
//
//	inline BOOL			IsDisabled()	{ return (BOOL)( ITimeout::UNLIMITED == TimeoutTick() ); }
//	inline VOID			Disabled()		{ TimeoutTick() = ITimeout::UNLIMITED; }
//	inline VOID			Instance()		{ TimeoutTick() = ITimeout::INSTANCE; }
//
//	inline VOID			SetNextTimeout( DWORD dwTimeoutTick ) { m_dwTimeoutTick = dwTimeoutTick; }
//	inline VOID			SetNextTimeoutByCurTickBased( DWORD dwTimeoutTick ) { m_dwTimeoutTick = GetTickCount() + dwTimeoutTick; }
//
//	inline DWORD&		TimeoutTick() { return m_dwTimeoutTick; }
//private:
//	DWORD				m_dwTimeoutTick;
//};



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

//////////////////////////////////////////////////////////////////////////
// <IWORKNODE INTERFACEs>
BOOL	ReportFromMasters::DoInit()
{
	m_pLoggingHandler = new CSunLog();
	if( !m_pLoggingHandler->Init( 1, eFULL_LOG, "SunLog", "REPORT" ) )
	{
		ASSERT( !"Can't Create SunLog for Reporting" );
	}
	return TRUE;
}

BOOL	ReportFromMasters::DoProcess()
{
	if( m_pLoggingHandler )
		m_pLoggingHandler->Update();

	//m_pLoggingHandler->FILES.ITEM( CSunLog::cCRITICAL_LOG, 

	return TRUE;
}

BOOL	ReportFromMasters::IsComplete()
{
	return FALSE;
}

BOOL	ReportFromMasters::DoRelease()
{
	if( m_pLoggingHandler ) m_pLoggingHandler->Flush( CSunLog::cOUTPUT_FILE );
	SAFE_DELETE( m_pLoggingHandler );
	return TRUE;
}


VOID	ReportFromMasters::OnMsg( sSIG_MSG* pSIG_MSG )
{
	if( pSIG_MSG && (eSIGMSG::REPORTFROMMASTERS == pSIG_MSG->_TYPE) )
	{
		sSIG_REPORTFROMMASTERS* pREPORT = (sSIG_REPORTFROMMASTERS*)pSIG_MSG;
		DWORD dwMasterIP = pREPORT->MasterIP();
		ServerSessionEx* pSession = pREPORT->SessionRef();
		sCACHEITEM_REPORT_INFO_MINORVER& rINFO = pREPORT->INFO();
		//_T("[%-16s]\tGUID:%u\tCUID:%u\tUID:%-41s\tPUID:%u\tITEMCODE:%u\tCATEID:%u\tPRICE:%u")
    #if SUN_CODE_RENEWAL
        MessageOutEx(m_pLoggingHandler,
                     static_cast<CSunLog::eLOGLEVEL>(1), CSunLog::cOUTPUT_FILE, CSunLog::cCATE_ITEM,
                     "|IP=%-16s|PriceGuid=%u|ItemCode=%u|Category=%u|Price=%u|",
                     pSession ? pSession->GetIP() : "127.0.0.1",
                     rINFO.dwPriceGuid,
                     rINFO.wItemCode,
                     rINFO.byCategoryID,
                     rINFO.dwPrice);
    #endif
    #if SUN_CODE_BACKUP // 오래된 (제거된) 인터페이스 사용에 따른 대체 작업
		if( m_pLoggingHandler )
			m_pLoggingHandler->FILES.ITEM(
				CSunLog::cCRITICAL_LOG,
				_T("[%-16s]\t%u\t%u\t%[-25s]\t%u\t%u\t%u\t%u"),
				pSession->GetIP(),
				rINFO.dwUserGuid,
				rINFO.dwCharGuid,
				rINFO.szUserID,
				rINFO.dwPriceGuid,
				rINFO.wItemCode,
				rINFO.byCategoryID,
				rINFO.dwPrice
				);
    #endif
	}
}


