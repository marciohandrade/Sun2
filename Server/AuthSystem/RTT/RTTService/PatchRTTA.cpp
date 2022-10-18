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
		RTTAgent서비스 상태를 확실하게 SERVICE_STOPPED 상태로 만들고 다음으로 넘어가자
	*/
	int nCnt = 0;
	while( SERVICE_STOPPED != eXtreme::WinService::Instance()->QueryService(NULL, SERVICENAME_RTTA, NULL, 0) )
	{
		// RTTAgent 서비스를 중지 한다.
		eXtreme::WinService::Instance()->StopService( 0, SERVICENAME_RTTA );
		// 서비스를 10번정도 중지 명령을 내렸음에도 불구하고 중지가 안될경우 패치를 중지 한다.
		// QueryService 는 SERVICE_STOPPED/SERVICE_RUNNING/SERVICE_PAUSED 단계를 반환하므로 이런 상태가 오면 안된다.
		if(nCnt++>10) 
		{
			OutputDebug("RTTAgent Service Stopped Fail.............  \n");
			return ;
		}
	}

	OutputDebug("RTTAgent Service Stopped .............  \n");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGenericFTPClient ftpClient;	
	// FTP에 접속을 한다.
	if(ftpClient.Open(szFtpAddress, dwFtpPort, szFtpUser, szFtpPassword)==TRUE)
	{
		char szRemoteFileName[MAX_PATH] = {0, };	// FTP서버의 경로
		char szLocalFileName[MAX_PATH] = {0, };		// 클라이언트의 경로
		char szRTTAgentPath[MAX_PATH] = {0, };		// 다운받은 Zip파일 압축해제 경로

		sprintf(szRemoteFileName, "%s/%s", szFtpGetFilePath, szFtpGetFileName);

		::GetPrivateProfileString( "RTTAgentPatch", "PATH", "c:/rttagent", szRTTAgentPath, MAX_PATH, SERVERINFO_FILENAME );

		strcpy(szLocalFileName, UPDATE_DIR);
		strcat(szLocalFileName, szFtpGetFileName);

		// 업데이트 파일을 저장할 디렉토리 생성
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
/*------------------------------ 2006. 8. 3 김종호(압축해제 코드 삽입) ---------------------*/

		// 압축 해제
		CUnZipper UZip;

		if( UZip.Unzip(szLocalFileName, UPDATE_DIR) )
		{
			DeleteFile(szLocalFileName);			// 압축을 해제했으므로 파일삭제

			// 서비스를 중지 했으므로 압축해제한 파일을 지정된 폴더로 복사후 재실행한다.
			char szFindSourceFile[MAX_PATH] = {0, };
			strcpy(szFindSourceFile, UPDATE_DIR);
			strcat(szFindSourceFile, "*.*");

			char szSourceFileName[MAX_PATH] = {0, };		// 이동할 대상 파일의 Full Path
			char szDestPath[MAX_PATH] = {0, };				// 이동할 목적지의 Full Path
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

//					if( CopyFile(szSourceFileName, szDestPath, FALSE) )	// 파일복사
					if( MoveFileEx(szSourceFileName, szDestPath, MOVEFILE_REPLACE_EXISTING) )
					{
						OutputDebug("MoveFile To \"%s\"\n", szDestPath);
					}
					else
					{
						OutputDebug("File Access Error, FileName : %s, GetLastError : %d\n", szDestPath, GetLastError());
					}
				}
				memset(szSourceFileName, 0, strlen(szSourceFileName) );		// 버퍼 초기화
				memset(szDestPath, 0, strlen(szDestPath) );

				// 다음파일 검색
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
	
	// 위에서 정확하게 RTTAgent의 상태를 중지 상태로 만들었기 때문에 여기서는 무조건 RTTAgent 서비스를 시작 한다.
	DWORD dwErrorCode = eXtreme::WinService::Instance()->StartService( 0, SERVICENAME_RTTA );
	OutputDebug("RTTAgent start service(%d)\n", dwErrorCode);
}