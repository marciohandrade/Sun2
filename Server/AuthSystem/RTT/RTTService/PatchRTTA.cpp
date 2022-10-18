#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib ")


#include "PatchRTTA.h"
#include "CUnZipper.h"

#pragma comment(lib, "Netapi32.lib")

#include "genericftpclient.h"

PatchRTTA::PatchRTTA()
{	
}

PatchRTTA::~PatchRTTA()
{
}

void PatchRTTA::Start( char* szFtpAddress, DWORD dwFtpPort, char* szFtpUser, char* szFtpPassword, char* szFtpGetFilePath, char* szFtpGetFileName )
{
	OutputDebug( "[RTTA Patch info]FtpAddress=%s FtpPort=%d, FtpUser=%s, FtpPassword=%s, FtpGetFilePath=%s, FtpGetFileName=%s\n",
		szFtpAddress, 
		dwFtpPort, 
		szFtpUser, 
		szFtpPassword, 
		szFtpGetFilePath, 
		szFtpGetFileName);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		20060823
		add by guiun
		RTTAgent���� ���¸� Ȯ���ϰ� SERVICE_STOPPED ���·� ����� �������� �Ѿ��
	*/
	int nCnt = 0;
	while( SERVICE_STOPPED != eXtreme::WinService::Instance()->QueryService(NULL, SERVICENAME_RTTA, NULL, 0) )
	{
		// RTTAgent ���񽺸� ���� �Ѵ�.
		eXtreme::WinService::Instance()->StopService( 0, SERVICENAME_RTTA );
		// ���񽺸� 10������ ���� ����� ���������� �ұ��ϰ� ������ �ȵɰ�� ��ġ�� ���� �Ѵ�.
		// QueryService �� SERVICE_STOPPED/SERVICE_RUNNING/SERVICE_PAUSED �ܰ踦 ��ȯ�ϹǷ� �̷� ���°� ���� �ȵȴ�.
		if(nCnt++>10) 
		{
			OutputDebug("RTTAgent Service Stopped Fail.............  \n");
			return ;
		}
	}

	OutputDebug("RTTAgent Service Stopped .............  \n");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGenericFTPClient ftpClient;	
	// FTP�� ������ �Ѵ�.
	if(ftpClient.Open(szFtpAddress, dwFtpPort, szFtpUser, szFtpPassword)==TRUE)
	{
		char szRemoteFileName[MAX_PATH] = {0, };	// FTP������ ���
		char szLocalFileName[MAX_PATH] = {0, };		// Ŭ���̾�Ʈ�� ���
		char szRTTAgentPath[MAX_PATH] = {0, };		// �ٿ���� Zip���� �������� ���

		sprintf(szRemoteFileName, "%s/%s", szFtpGetFilePath, szFtpGetFileName);

		::GetPrivateProfileString( "RTTAgentPatch", "PATH", "c:/rttagent", szRTTAgentPath, MAX_PATH, SERVERINFO_FILENAME );

		strcpy(szLocalFileName, UPDATE_DIR);
		strcat(szLocalFileName, szFtpGetFileName);

		// ������Ʈ ������ ������ ���丮 ����
		if( FALSE == PathIsDirectory(UPDATE_DIR) )
		{
			if( !CreateDirectory(UPDATE_DIR, 0) )
			{
				OutputDebug("Cannot Maked Update Directory, [DIR : %s]\n", UPDATE_DIR );
				return;
			}
		}

		ftpClient.GetInternetFile(szRemoteFileName, szLocalFileName);	
		ftpClient.Close();
/*------------------------------ 2006. 8. 3 ����ȣ(�������� �ڵ� ����) ---------------------*/

		// ���� ����
		CUnZipper UZip;

		if( UZip.Unzip(szLocalFileName, UPDATE_DIR) )
		{
			DeleteFile(szLocalFileName);			// ������ ���������Ƿ� ���ϻ���

			// ���񽺸� ���� �����Ƿ� ���������� ������ ������ ������ ������ ������Ѵ�.
			char szFindSourceFile[MAX_PATH] = {0, };
			strcpy(szFindSourceFile, UPDATE_DIR);
			strcat(szFindSourceFile, "*.*");

			char szSourceFileName[MAX_PATH] = {0, };		// �̵��� ��� ������ Full Path
			char szDestPath[MAX_PATH] = {0, };				// �̵��� �������� Full Path
			BOOL bResult = TRUE;

			WIN32_FIND_DATA wfd;
            HANDLE hSearch = FindFirstFile(szFindSourceFile, &wfd);

			while(bResult)
			{
				if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					sprintf(szSourceFileName, "%s", UPDATE_DIR);
					strcat(szSourceFileName, wfd.cFileName);
					sprintf(szDestPath, "%s/%s", szRTTAgentPath, wfd.cFileName);

//					if( CopyFile(szSourceFileName, szDestPath, FALSE) )	// ���Ϻ���
					if( MoveFileEx(szSourceFileName, szDestPath, MOVEFILE_REPLACE_EXISTING) )
					{
						OutputDebug("MoveFile To \"%s\"\n", szDestPath);
					}
					else
					{
						OutputDebug("File Access Error, FileName : %s, GetLastError : %d\n", szDestPath, GetLastError());
					}
				}
				memset(szSourceFileName, 0, strlen(szSourceFileName) );		// ���� �ʱ�ȭ
				memset(szDestPath, 0, strlen(szDestPath) );

				// �������� �˻�
				bResult = FindNextFile(hSearch, &wfd);
			}

			FindClose(hSearch);
/*----------------------------------------------------------------------------------- END --*/
		}
		else
		{
			OutputDebug("Unzip Fail \n");
		}
	}
	else
	{
		OutputDebug("FTP Server Connect Fail \n");
	}
	
	// ������ ��Ȯ�ϰ� RTTAgent�� ���¸� ���� ���·� ������� ������ ���⼭�� ������ RTTAgent ���񽺸� ���� �Ѵ�.
	DWORD dwErrorCode = eXtreme::WinService::Instance()->StartService( 0, SERVICENAME_RTTA );
	OutputDebug("RTTAgent start service(%d)\n", dwErrorCode);
}