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

	// File�� ����ϴ� �Ͱ��� �ٸ��� '\r' '\n'�� �����ϵ��� �����Ѵ�.
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

		// ��Ŷ �ش� ���� �����͸� ��´�.
		BYTE* pHeaderPtr = (BYTE*)wLogOff.QueuedStream - s_wNTF_HeaderLen;

		// [20061206_xx:xx:xx]
		//           | CHAR
		// [ 2 0 0 6 1 2 0 6 _
		//           | TCHAR
		MSG_SERVERCOMMON_SECURE_LOG_NTF* pMsgNTF = (MSG_SERVERCOMMON_SECURE_LOG_NTF*)pHeaderPtr;

		// ������ ���� ���۴� �α� �����̸�, ��¥ ��ϵ� ������ �����Ѵ�.
		CopyMemory( pMsgNTF, &(pSecureLog->m_APPEND_LOG_INFO), s_wNTF_HeaderLen );

		// m_dwKey�� ����� �������� Ȱ������.
		pMsgNTF->m_byCategory = SERVERCOMMON;
		pMsgNTF->m_byProtocol = SERVERCOMMON_SECURE_LOG_NTF;
		
		wLogOff.Written -= 2;
		wLogOff.QueuedStream[wLogOff.Written++] = _T('\0');
		pMsgNTF->m_wLength = (WORD)(wLogOff.Written*sizeof(TCHAR));

		// NetworkObject�� ������ �ִ��� ������... �̰� �� ����� �ִ°�?
		pSession->Send( (BYTE*)pMsgNTF, pMsgNTF->m_wLength + s_wNTF_HeaderLen );
	}

	// �ѹ� ���� �ٽ� �ʱ�ȭ ���ѹ�����.
	wLogOff.Written = 0;

	return TRUE;
}