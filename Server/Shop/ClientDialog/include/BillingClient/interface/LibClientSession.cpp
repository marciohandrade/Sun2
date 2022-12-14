
#include "stdafx.h"
#include "LibClientSession.h"

CLibClientSession::CLibClientSession(void)
{
	::InitializeCriticalSection(&m_cs);
}

CLibClientSession::~CLibClientSession(void)
{
	::DeleteCriticalSection(&m_cs);
}

void CLibClientSession::OnNetConnect(bool success, DWORD error)
{
	if (success) {
		AfxMessageBox(_T("OnConnect succeed"));
	} else {
		AfxMessageBox(_T("OnConnect fali"));
	}
}

void CLibClientSession::OnNetSend(int size)
{
	//AfxMessageBox("OnSend");
}

void CLibClientSession::OnNetClose(DWORD error)
{
	AfxMessageBox(_T("OnClose"));
}

void CLibClientSession::OnUserStatus(
	DWORD	dwAccountGUID, 
	long	dwBillingGUID, 
	DWORD	RealEndDate, 
	DWORD	EndDate, 
	double	dRestPoint, 
	double	dRestTime, 
	short	nDeductType, 
	short	nAccessCheck, 
	short	nResultCode)
{
	CString strMsg;

	struct tm tRealEndDate;
	struct tm tEndDate;
	
	tRealEndDate.tm_hour = 0;
	tRealEndDate.tm_isdst = 0;
	tRealEndDate.tm_mday = 0;
	tRealEndDate.tm_min = 0;
	tRealEndDate.tm_mon = 0;
	tRealEndDate.tm_sec = 0;
	tRealEndDate.tm_wday = 0;
	tRealEndDate.tm_yday = 0;
	tRealEndDate.tm_year = 0;

	tEndDate.tm_hour = 0;
	tEndDate.tm_isdst = 0;
	tEndDate.tm_mday = 0;
	tEndDate.tm_min = 0;
	tEndDate.tm_mon = 0;
	tEndDate.tm_sec = 0;
	tEndDate.tm_wday = 0;
	tEndDate.tm_yday = 0;
	tEndDate.tm_year = 0;

#ifdef VERSION_VS60
	struct tm* tRealEndDate1;
	struct tm* tEndDate1;

	tRealEndDate1 = localtime((time_t*)&RealEndDate);
	tEndDate1 = localtime((time_t*)&EndDate);

	if(tRealEndDate1) tRealEndDate = *tRealEndDate1;
	if(tEndDate1) tEndDate = *tEndDate1;
#elif VERSION_VS2003
	struct tm* tRealEndDate1;
	struct tm* tEndDate1;

	tRealEndDate1 = localtime((time_t*)&RealEndDate);
	tEndDate1 = localtime((time_t*)&EndDate);

	if(tRealEndDate1) tRealEndDate = *tRealEndDate1;
	if(tEndDate1) tEndDate = *tEndDate1;
#else
	_localtime32_s(&tRealEndDate, (__time32_t*)&RealEndDate);
	_localtime32_s(&tEndDate, (__time32_t*)&EndDate);
#endif

	CString strend, strRend;

	strend.Format(_T("%d-%d-%d %d:%d:%d"), tEndDate.tm_year+1900, tEndDate.tm_mon+1, tEndDate.tm_mday, tEndDate.tm_hour, tEndDate.tm_min, tEndDate.tm_sec);
	strRend.Format(_T("%d-%d-%d %d:%d:%d"), tRealEndDate.tm_year+1900, tRealEndDate.tm_mon+1, tRealEndDate.tm_mday, tRealEndDate.tm_hour, tRealEndDate.tm_min, tRealEndDate.tm_sec);

	strMsg.Format(_T("????????: %d\r\n????????: %d\r\n????????: %d\r\n????????????: %d\r\n????????????: %d\r\n??????????: %f\r\n????????: %f\r\n????????: ")
		, dwAccountGUID, dwBillingGUID, nDeductType, nAccessCheck, nResultCode, dRestPoint, dRestTime);

	strMsg += strend;
	strMsg += _T("\r\n????????????: ");
	strMsg += strRend;
	strMsg += _T("\r\n");

	WriteLog((LPSTR)(LPCTSTR)strMsg);
}

void CLibClientSession::OnInquireMultiUser(
	long	AccountID,
	long	RoomGUID,
	long	Result)
{
	CString strMsg;
	strMsg.Format(_T("???? ????: %d, ?????? GUID: %d, ????: %d\r\n"), AccountID, RoomGUID, Result);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
}

void CLibClientSession::OnInquirePersonDeduct(
	long	AccountID, 
	long	GameCode, 
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("AccountID: %d, GameCode: %d, ResultCode: %d\r\n"), 
				AccountID, GameCode, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
}

void CLibClientSession::OnInquirePCRoomPoint(
	long	AccountID, 
	long	RoomGUID, 
	long	GameCode, 
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("AccountID: %d, RoomGUID: %d, GameCode: %d, ResultCode: %d\r\n"), 
				AccountID, RoomGUID, GameCode, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
}

void CLibClientSession::OnCheckLoginUser(
	long	AccountID,
	long	BillingGUID)
{
	CString strMsg;
	strMsg.Format(_T("AccountID: %d, BillingGUID: %d\r\n"), 
				AccountID, BillingGUID);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
}

void CLibClientSession::OnPCRoomIPUpdate(
	long	RoomGUID,
	long	GameCode)
{
	CString strMsg;
	strMsg.Format(_T("[PC Room IP Upated] RoomGUID: %d, GameCode: %d\r\n"), RoomGUID, GameCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CLibClientSession::WriteLog(char* szMsg)
{
	::EnterCriticalSection(&m_cs);

	TCHAR szModule[MAX_PATH] = {0};
	TCHAR szFilePath[MAX_PATH] = {0};

	::GetModuleFileName( NULL, szModule, MAX_PATH );

	*(_tcsrchr( szModule, '\\' ) ) = 0;


	StringCchPrintf(szFilePath, STRSAFE_MAX_CCH , _T("%s\\ClientDialog_BillingClient.log"), szModule );

	CTime t = CTime::GetCurrentTime();   	
	
	CString str;
	str.Format(_T("[%s] %s"), t.Format(_T("%Y-%m-%d %H:%M:%S")), szMsg);

	
	HANDLE	hFile;	
	DWORD	dwBytesWritten, dwPos;  

	hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  //File ???? 
	if (hFile != INVALID_HANDLE_VALUE)
	{ 
		dwPos = SetFilePointer(hFile, 0, NULL, FILE_END); 

		LockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0); 
		WriteFile(hFile, str.GetBuffer(0), str.GetLength(), &dwBytesWritten, NULL); 
		str.ReleaseBuffer();
		UnlockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0);     
		CloseHandle(hFile); 
	}  	

	::LeaveCriticalSection(&m_cs);
}
