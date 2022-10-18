/*------------------------------------------------------------------------------
 * FileReader.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by 
**----------------------------------------------------------------------------*/

#pragma once

class CFileReader
{
public:
	CFileReader(void);
	~CFileReader(void);

	BOOL				  Open(TCHAR* pszFileName);			 // 파일 오픈.
	BOOL				  SetLogFileOffset(fpos_t ulOffset); // 파일 포인터 위치를 설정.
	fpos_t				  GetLogFileOffset();				 // 현재 파일 포인터 위치 가져오기.
	LP_BINLOG_BASE_INFO   Read();							 // 파일 읽기.
	eLOG_STATE			  GetLastErrorCode() { return (eLOG_STATE)m_dwErrCode; }				// 에러코드 반환.
	//BOOL				  IsReconnectErr(INT nErrCode);
	void				  Close();							 // 모든 상태 종료.

private:
	HANDLE	m_hFileStream;
	BOOL	m_bCurrentPos;
	DWORD	m_dwErrCode;
	DWORD	m_dwSystemErrCode;
	TCHAR   m_szFileName[MAX_PATH];
	

	LP_BINLOG_BASE_INFO  SelectRealObject(LP_BINLOG_BASE_INFO pBaseInfo, DWORD dwReadLen);
	VOID				 SetConvertSystemErr();					 //최종 에러코드 설정(stdafx.h 구조체 참조.).

};
