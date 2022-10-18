#include "stdafx.h"

#include "SecureLog.h"
#include "PacketStruct_ServerCommon.h"
#include <SolarLog.h>

const WORD s_wNTF_HeaderLen = sizeof(MSG_SERVERCOMMON_SECURE_LOG_NTF)-1;

SecureLog::~SecureLog()
{
	FILE_LOG.QueuedStream = (TCHAR*)((BYTE*)FILE_LOG.QueuedStream - s_wNTF_HeaderLen);
	CSunLog::~CSunLog();
}

BOOL	SecureLog::Init(
						const BYTE byLogOption,
						const BYTE byLogLevel,
						const TCHAR* pszFilePath,
						const TCHAR* pszFileName,
						BOOL bUseSolarLog
						)
{
	BOOL bRet = CSunLog::Init( byLogOption, byLogLevel, pszFilePath, pszFileName, bUseSolarLog );

	if( !bRet )
		return bRet;

	FILE_LOG.QueuedStream = (TCHAR*)((BYTE*)FILE_LOG.QueuedStream + s_wNTF_HeaderLen);

	return TRUE;
}

BOOL	SecureLog::WriteMessage_Network( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist )
{
	const DWORD c_NetLogDataSize = 1023;

	CSunLog::sFILE_LOG& wLogOff = (CSunLog::sFILE_LOG&)((CSunLog*)pSunLog)->GetFILE_LOG();

	INT iWritten = _vsntprintf(
						wLogOff.QueuedStream + wLogOff.Written,
						c_NetLogDataSize - wLogOff.Written,
						pMsg, vlist );

	if( iWritten < 0 )
		return FALSE;

	wLogOff.Written += (DWORD)iWritten;

	// File에 기록하는 것과는 다르게 '\r' '\n'은 무시하도록 설정한다.
	if( wLogOff.Written > 1 )	//2
	{
		if( _T('\n') == wLogOff.QueuedStream[wLogOff.Written-1] )
			wLogOff.Written --;
		else if( _T('\r') == wLogOff.QueuedStream[wLogOff.Written-2] )
			wLogOff.Written -= 2;
	}

	wLogOff.QueuedStream[wLogOff.Written++] = _T('\r');
	wLogOff.QueuedStream[wLogOff.Written++] = _T('\n');

	SecureLog* pSecureLog = (SecureLog*)pSunLog;

	if( pSecureLog && pSecureLog->m_pSession )
	{
		NetworkObject* pSession = pSecureLog->m_pSession;

		// 패킷 해더 버퍼 포인터를 얻는다.
		BYTE* pHeaderPtr = (BYTE*)wLogOff.QueuedStream - s_wNTF_HeaderLen;

		// [20061206_xx:xx:xx]
		//           | CHAR
		// [ 2 0 0 6 1 2 0 6 _
		//           | TCHAR
		MSG_SERVERCOMMON_SECURE_LOG_NTF* pMsgNTF = (MSG_SERVERCOMMON_SECURE_LOG_NTF*)pHeaderPtr;

		// 보내기 위한 버퍼는 로그 버퍼이며, 날짜 기록된 공간을 재사용한다.
		CopyMemory( pMsgNTF, &(pSecureLog->m_APPEND_LOG_INFO), s_wNTF_HeaderLen );

		// m_dwKey는 예약된 공간으로 활용하자.
		pMsgNTF->m_byCategory = SERVERCOMMON;
		pMsgNTF->m_byProtocol = SERVERCOMMON_SECURE_LOG_NTF;
		
		wLogOff.Written -= 2;
		wLogOff.QueuedStream[wLogOff.Written++] = _T('\0');
		pMsgNTF->m_wLength = (WORD)(wLogOff.Written*sizeof(TCHAR));

		// NetworkObject에 문제가 있는지 없는지... 이걸 알 방법이 있는가?
		pSession->Send( (BYTE*)pMsgNTF, pMsgNTF->m_wLength + s_wNTF_HeaderLen );
	}

	// 한번 쓰고 다시 초기화 시켜버린다.
	wLogOff.Written = 0;

	return TRUE;
}