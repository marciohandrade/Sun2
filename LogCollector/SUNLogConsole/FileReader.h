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

	BOOL				  Open(TCHAR* pszFileName);			 // ���� ����.
	BOOL				  SetLogFileOffset(fpos_t ulOffset); // ���� ������ ��ġ�� ����.
	fpos_t				  GetLogFileOffset();				 // ���� ���� ������ ��ġ ��������.
	LP_BINLOG_BASE_INFO   Read();							 // ���� �б�.
	eLOG_STATE			  GetLastErrorCode() { return (eLOG_STATE)m_dwErrCode; }				// �����ڵ� ��ȯ.
	//BOOL				  IsReconnectErr(INT nErrCode);
	void				  Close();							 // ��� ���� ����.

private:
	HANDLE	m_hFileStream;
	BOOL	m_bCurrentPos;
	DWORD	m_dwErrCode;
	DWORD	m_dwSystemErrCode;
	TCHAR   m_szFileName[MAX_PATH];
	

	LP_BINLOG_BASE_INFO  SelectRealObject(LP_BINLOG_BASE_INFO pBaseInfo, DWORD dwReadLen);
	VOID				 SetConvertSystemErr();					 //���� �����ڵ� ����(stdafx.h ����ü ����.).

};
